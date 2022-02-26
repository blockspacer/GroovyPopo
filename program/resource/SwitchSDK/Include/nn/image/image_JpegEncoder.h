/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

/**
@file
@brief JPEGデータへのエンコード機能に関わる定義
*/

#include <nn/image/image_JpegCommon.h>
#include <nn/image/image_ExifBuilder.h>

namespace nn { namespace image {

/**
    @brief      JPEGデータへのエンコードを行います。

    @details    与えられたピクセルデータを指定されたパラメータに基づきエンコードし、JPEGデータを出力します。
                また、与えられた nn::image::ExifBuilder オブジェクトからExif情報を生成し、JPEGデータに付加する機能も提供します。

                ### 出力されるJPEGデータの仕様
                出力されるピクセルデータはベースライン方式でスキャンされ、YCbCr色空間で符号化されてJPEGデータとなります。
                ピクセルデータのサンプリング比は4:2:0(デフォルト)、4:2:2、4:4:4から選ぶことができます。
                また、エンコードの品質を指定することで、画質と出力サイズを調整できます。

                ### ワークメモリサイズと出力サイズ

                JPEGデータへのエンコード時に必要なワークメモリの大きさと出力されるJPEGデータの大きさは、入力するピクセルデータや
                エンコードのパラメータによって変動します。

                必要なワークメモリの大きさは、ピクセルデータの内容に関わらず、ピクセルデータの大きさとサンプリング比によって
                一意に定まります。
                もしアプリケーションが不特定のサンプリング比でエンコードを行う場合、予約すべきワークメモリの大きさの上限は、
                事前に次のパラメータでエンコードを行うことで取得することができます。

                | ピクセルサイズ | サンプリング比 |
                |-|-:|
                |アプリケーションが取り扱う最大値|4:2:0|

                一方で出力されるJPEGデータの大きさは、ピクセルデータの内容(色や模様)によって変動するため、
                事前に計算することが困難です。
                参考のため、代表的なピクセルデータの大きさとエンコードパラメータに対する出力バッファの大きさを記します。
                この数値を目安に、十分な大きさの出力バッファを確保してください。
                ただし下表の数値には、Exif 情報の大きさは含まれていません。

                |画像サイズ|サンプリング比|エンコード品質|出力サイズ(バイト数)|
                |-|-:|-:|-:|
                |1080p| 4:4:4| 100| 3349964|
                |〃   | 〃   |  75| 1228552|
                |〃   | 〃   |  50| 955057|
                |〃   | 4:2:2| 100| 3334076|
                |〃   | 〃   |  75| 1212352|
                |〃   | 〃   |  50| 938857|
                |〃   | 4:2:0| 100| 3326408|
                |〃   | 〃   |  75| 1204493|
                |〃   | 〃   |  50| 931044|
                |720p | 4:4:4| 100| 1512857|
                |〃   | 〃   |  75| 546301|
                |〃   | 〃   |  50| 424125|
                |〃   | 4:2:2| 100| 1505771|
                |〃   | 〃   |  75| 539101|
                |〃   | 〃   |  50| 416925|
                |〃   | 4:2:0| 100| 1502424|
                |〃   | 〃   |  75| 535549|
                |〃   | 〃   |  50| 413358|
                |VGA  | 4:4:4| 100| 498617|
                |〃   | 〃   |  75| 183302|
                |〃   | 〃   |  50| 142665|
                |〃   | 4:2:2| 100| 496256|
                |〃   | 〃   |  75| 180902|
                |〃   | 〃   |  50| 140265|
                |〃   | 4:2:0| 100| 495013|
                |〃   | 〃   |  75| 179681|
                |〃   | 〃   |  50| 139076|

                ### オブジェクトの内部状態
                JpegEncoder オブジェクトはその内部で次の3つの状態を持ちます。
                - ピクセルデータ未登録状態
                    - JpegEncoder オブジェクトにピクセルデータが設定されていない状態です。
                - ピクセルデータ登録済み状態
                    - JpegEncoder オブジェクトにピクセルデータが設定されている一方、ピクセルデータやパラメータが解析されていない状態です。
                - ピクセルデータ解析済み状態
                    - JpegEncoder オブジェクトに最後に設定されたピクセルデータとパラメータが解析されたあとの状態です。

                ### 並列処理について
                JpegEncoder クラスのオブジェクトを複数生成して、複数のピクセルデータを並行してJPEGエンコードすることが可能です。
*/
class JpegEncoder
{
    NN_DISALLOW_COPY(JpegEncoder);
public:
    /**
        @brief      JpegEncoder クラスのインスタンスを作成します。

        @post       SetQuality() で設定可能な画質設定は 100 に初期化されます。
                    また、 SetSamplingRatio() で設定可能なサンプリング比は 4:2:0 に初期化されます。
                    作成されるオブジェクトの内部状態は「ピクセルデータ未登録状態」となります。
    */
    JpegEncoder() NN_NOEXCEPT;

    /**
        @brief  JpegEncoder クラスのインスタンスを破棄します。
    */
    virtual ~JpegEncoder() NN_NOEXCEPT;

    //! @name エンコード設定
    //! @{
    /**
        @brief      JPEGエンコードを行うピクセルデータを、ピクセルフォーマットを指定して登録します。

        @param[in]  pixelData       エンコード対象のピクセルデータを指定します。
                                    このピクセルデータの各ピクセルは、 'pxFormat' で指定したピクセルフォーマットであることが必要です。
                                    ピクセルデータがアルファチャンネルを含む場合、アルファチャンネルの値は無視されます。
                                    また、アラインメントのためのパディング領域は無視されます。
        @param[in]  pxFormat        エンコード対象のピクセルデータのピクセルフォーマットを指定します。
        @param[in]  dimension       'pixelData'の大きさ(幅,高さ)
        @param[in]  lineAlignment   ピクセルデータの行アラインメントを指定します。
                                    例えば129px幅の画像が130px幅のピクセルバッファに格納されている場合、2などを指定します。

        @pre        次の条件をすべて満たすことが必要です。
                    - 'pixelData' は 'dimension' で指定される大きさで、 'lineAlignment' の値で行アラインメントされている必要があります。
                    - 'pxFormat' として nn::image::PixelFormat 列挙子で定義済みの値を指定する必要があります。
        @post       「ピクセルデータ登録済み状態」となり、 Analyze() を呼ぶことができます。

        @details    エンコード対象のピクセルデータを、ピクセルフォーマットを指定して登録します。
                    ピクセルフォーマットに nn::image::PixelFormat_Rgb24 を指定すると、それ以外のピクセルフォーマットを指定した場合と比べ必要なワークメモリが小さくなります。

                    本関数呼び出し後、インスタンスを破棄するか再度本関数を呼び出すまで、ここで登録した 'pixelData' の
                    内容を変更しないでください。
    */
    void SetPixelData(
        const void *pixelData,
        PixelFormat pxFormat,
        const ::nn::image::Dimension &dimension,
        const int lineAlignment) NN_NOEXCEPT;

    /**
        @brief      JPEGエンコードを行うピクセルデータを登録します。

        @param[in]  pixelData       エンコード対象の32bitのRGBAピクセルデータを指定します。
                                    アルファチャンネルやアラインメントのためのパディング領域は無視されます。
        @param[in]  dimension       'pixelData'の大きさ(幅,高さ)
        @param[in]  lineAlignment   ピクセルデータの行アラインメントを指定します。
                                    例えば129px幅の画像が130px幅のピクセルバッファに格納されている場合、2などを指定します。

        @pre        'pixelData' は 'dimension' で指定される大きさで、 'lineAlignment' の値で行アラインメントされている必要があります。
        @post       「ピクセルデータ登録済み状態」となり、 Analyze() を呼ぶことができます。

        @details    エンコード対象のピクセルデータを指定します。
                    本関数呼び出し後、インスタンスを破棄するか再度本関数を呼び出すまで、ここで登録した 'pixelData' の
                    内容を変更しないでください。
    */
    void SetPixelData(
        const void *pixelData,
        const ::nn::image::Dimension &dimension,
        const int lineAlignment) NN_NOEXCEPT
    {
        SetPixelData(pixelData, PixelFormat_Rgba32, dimension, lineAlignment);
    }

    /**
        @brief      JPEGエンコード時の画質を設定します。

        @param[in]  quality     画質を [1, 100] の範囲で指定します。一般に値が大きくなるほど画質が向上し、エンコード後のサイズが増加します。

        @pre        'quality' には {1..100} に含まれる値を指定してください。
        @post       もし JpegEncoder オブジェクトが「ピクセルデータ解析済み状態」であれば、「ピクセルデータ登録済み状態」となります。
                    それ以外の状態からは変化しません。

        @details    JPEGエンコード時の画質を設定します。
                    この設定はエンコード後のJPEGデータの画質と大きさに影響します。

                    デフォルト値である 100 を使用する場合、この設定は必須ではありません。
    */
    void SetQuality(const int quality) NN_NOEXCEPT;

    /**
        @brief      JPEGエンコード時のピクセルサンプリング比を設定します。

        @param[in]  samplingRatio   サンプリング比を指定します。
                                    一般に 4:2:0, 4:2:2, 4:4:4 の順に画質が向上し、エンコード後のサイズが増加します。

        @post       もし JpegEncoder オブジェクトが「ピクセルデータ解析済み状態」であれば、「ピクセルデータ登録済み状態」となります。
                    それ以外の状態からは変化しません。

        @details    JPEGエンコード時のピクセルサンプリング比を設定します。
                    この設定はエンコード後のJPEGデータの画質と大きさに影響します。

                    デフォルト値である 4:2:0 を使用する場合、この設定は必須ではありません。
    */
    void SetSamplingRatio(const JpegSamplingRatio samplingRatio) NN_NOEXCEPT;
    //! @}
    // ~エンコード設定

    //! @name 中間情報の取得
    //! @{
    /**
        @brief      設定されたピクセルデータ、画質設定、サンプリング比を解析し、内部パラメータを構築します。

        @return     処理結果を返します。
        @retval     nn::image::JpegStatus_Ok    処理が正常に終了しました。

        @pre        「ピクセルデータ登録済み状態」か「ピクセルデータ解析済み状態」であることが必要です。
        @post       「ピクセルデータ解析済み状態」となります。

        @details    設定されたピクセルデータ、画質設定、サンプリング比を解析し、ピクセルデータのエンコードに
                    必要な情報を抽出することでエンコードに備えます。

                    現在の実装では JpegStatus_Ok のみ返ります。
    */
    JpegStatus Analyze() NN_NOEXCEPT;

    /**
        @brief      Analyze() によって取得された、 Encode() 用ワークメモリの大きさを取得します。

        @return     Encode() に与える必要のあるワークメモリの大きさ(バイト数)が返ります。

        @pre        「ピクセルデータ解析済み状態」であること。

        @details    必要なワークメモリの大きさはエンコード対象のピクセルデータおよびサンプリング比によって変動します。
                    JpegEncoder クラスの説明欄にワークメモリの大きさについて説明を記載しています。
                    その説明に従って、必要なワークメモリの大きさを計算し、予約してください。
    */
    size_t GetAnalyzedWorkBufferSize() const NN_NOEXCEPT;
    //! @}
    // ~中間情報の取得

    //! @name エンコード処理
    //! @{
    /**
        @brief      ピクセルデータをJPEGエンコードします。

        @param[out] pOutActualCodedSize 出力されるJPEGデータの実際の大きさ(バイト数)の格納先を指定します。
        @param[out] outJpegBuf      出力されるJPEGデータの格納先を指定します。
                                    出力されるJPEGデータを格納するための領域の大きさは、エンコード対象のピクセルデータ、
                                    画質設定およびサンプリング比によって変動し、不定です。
        @param[in]  jpegBufSize     'outJpegBuf' の大きさ(バイト数)を指定します。
        @param[in]  workBuf         ワークメモリを指定します。
        @param[in]  workBufSize     ワークメモリの大きさ(バイト数)を指定します。
        @param[in]  pExifBuilder    JPEGデータ に付加するExif情報を生成するための nn::image::ExifBuilder オブジェクトを指定します。
                                    nullptr を指定するとExif情報の出力を省略します。

        @return     処理結果を返します。
        @retval     nn::image::JpegStatus_Ok            処理が正常に終了しました。
        @retval     nn::image::JpegStatus_ShortOutput   エンコード中に出力バッファが不足しました。
                                                        より大きなバッファを出力バッファに指定するか、画質設定および
                                                        サンプリング比を調整してください。
                                                        画質設定やサンプリング比を下げると必要な出力サイズは小さく
                                                        なりますが、画質は劣化します。

        @pre        次の条件をすべて満たすことが必要です。
                    - 'workBuf' は GetAnalyzedWorkBufferSize() で取得した値以上の大きさである。
                    - 'workBuf' のアドレスが std::max_align_t のアラインメントに沿っている。
                    - 'pExifBuilder' に非 nullptr を指定する場合、 nn::image::ExifBuilder::IsBuildable() が
                        true を返す状態である。
                    - 「ピクセルデータ解析済み状態」である。
        @post       'pExifBuilder' に nullptr 以外の値を与えると、'outJpegBuf' に出力されるJPEGデータにExif情報が付加されます。
                    このとき ExifBuilder オブジェクト内部のワークメモリに変更が加えられます。

        @details    設定されたピクセルデータ、画質設定、サンプリング比を使用し、ピクセルデータをエンコードします。
                    このとき、 'pExifBuilder' に有効な値が指定されていると、出力されるJPEGデータにExif情報が付加されます。

                    Analyze() で JpegStatus_Ok となったピクセルデータが、 Encode() でエラーを返すことがあります。
                    これは Analyze() が参照しない情報を Encode() が使用しているためです。
                    従って Analyze() と Encode() の両方でエラーチェックを行う必要があります。

                    出力バッファの大きさは事前に計算できないため、用途に沿った十分な大きさを確保してください。
                    目安となるように、代表的なピクセルサイズとエンコード設定について、出力バッファの大きさの例を JpegEncoder クラスの説明欄に記載しています。

        @overloadlist{nn_image_JpegEncoder_Encode}
    */
    JpegStatus Encode(
        size_t *pOutActualCodedSize,
        void *outJpegBuf,
        const size_t jpegBufSize,
        void *workBuf,
        const size_t workBufSize,
        ExifBuilder *pExifBuilder) NN_NOEXCEPT;

    /**
        @brief      ピクセルデータをJPEGエンコードします。

        @param[out] pOutActualCodedSize 出力されるJPEGデータの実際の大きさ(バイト数)の格納先を指定します。
        @param[out] outJpegBuf      出力されるJPEGデータの格納先を指定します。
                                    出力されるJPEGデータを格納するための領域の大きさは、エンコード対象のピクセルデータ、
                                    画質設定およびサンプリング比によって変動し、不定です。
        @param[in]  jpegBufSize     'outJpegBuf' の大きさ(バイト数)を指定します。
        @param[in]  workBuf         ワークメモリを指定します。
        @param[in]  workBufSize     ワークメモリの大きさ(バイト数)を指定します。

        @return     処理結果を返します。
        @retval     nn::image::JpegStatus_Ok            処理が正常に終了しました。
        @retval     nn::image::JpegStatus_ShortOutput   エンコード中に出力バッファが不足しました。
                                                        より大きなバッファを出力バッファに指定するか、画質設定および
                                                        サンプリング比を調整してください。
                                                        画質設定やサンプリング比を下げると必要な出力サイズは小さく
                                                        なりますが、画質は劣化します。

        @pre        次の条件をすべて満たすことが必要です。
                    - 'workBuf' は GetAnalyzedWorkBufferSize() で取得した値以上の大きさである。
                    - 'workBuf' のアドレスが std::max_align_t のアラインメントに沿っている。
                    - 「ピクセルデータ解析済み状態」である。

        @details    設定されたピクセルデータ、画質設定、サンプリング比を使用し、ピクセルデータをエンコードします。
                    この関数の呼び出しでは、出力されるJPEGデータにExif情報は付加されません。

                    Analyze() で JpegStatus_Ok となったピクセルデータが、 Encode() でエラーを返すことがあります。
                    これは Analyze() が参照しない情報を Encode() が使用しているためです。
                    従って Analyze() と Encode() の両方でエラーチェックを行う必要があります。

                    出力バッファの大きさは事前に計算できないため、用途に沿った十分な大きさを確保してください。
                    目安となるように、代表的なピクセルサイズとエンコード設定について、出力バッファの大きさの例を JpegEncoder クラスの説明欄に記載しています。

        @overloadlist{nn_image_JpegEncoder_Encode}
    */
    JpegStatus Encode(
        size_t *pOutActualCodedSize,
        void *outJpegBuf,
        const size_t jpegBufSize,
        void *workBuf,
        const size_t workBufSize) NN_NOEXCEPT
    {
        return Encode(pOutActualCodedSize, outJpegBuf, jpegBufSize, workBuf, workBufSize, nullptr);
    }
    //! @}
    // ~エンコード処理

private:
    enum Stage
    {
        Stage_Unregistered = 0,
        Stage_Registered = 1,
        Stage_Analyzed = 2
    };
    Stage m_Stage;

    const void *m_PixelData;
    PixelFormat m_PixelFormat;
    Dimension m_Dim;
    int m_LineAlignment;
    int m_Quality;
    JpegSamplingRatio m_SamplingRatio;

    size_t m_BufSize;
};

}}
