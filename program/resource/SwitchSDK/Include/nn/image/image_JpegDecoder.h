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
@brief JPEGデータのデコード機能に関わる定義
*/

#include <nn/image/image_JpegCommon.h>

namespace nn { namespace image {

/**
    @brief      JPEGデータのデコードを行います。

    @details    与えられたJPEGデータをデコードし、ピクセルデータを出力します。
                このときダウンスケーリングのパラメータを指定することで、狭小なバッファへのデコードも可能です。
                また、与えられたJPEGデータからExif情報を格納したバイナリの場所を特定する機能を提供します。

                ### デコード可能なJPEGデータ

                デコード対象のJPEGデータとして次のフォーマットに対応します。
                ただし、現存するすべてのJPEGデータをデコードできることを保証するものではありません。
                - スキャン方式は、ベースライン方式とプログレッシブ方式に対応します。
                - ロスレスJPEGには非対応です。
                - ピクセルサンプリング比は4:0:0を含め、原則すべてのパラメータに対応します。

                ### ワークメモリサイズの見積もり

                JPEGデータのデコード時に使用するワークバッファの大きさは、画像の大きさ(幅と高さ)およびデコードパラメータから計算されます。
                一般的なJPEGデータのほとんどについて、下記のパラメータでエンコードされたJPEGデータをデコードするときに、もっとも多くのワークバッファを必要とします。
                従って、そのワークバッファ量をアプリケーションで予約すべき量と見積もることができます。
                この見積もりの際には、アプリケーションが取り扱う最大の画素数のピクセルデータを使用してください。

                |エンコードパラメータ | ベースラインJPEGでの設定値 | プログレッシブJPEGでの設定値|
                |-|-:|-:|
                |色空間             | YCbCr | YCbCr|
                |サンプリング比     | 4:2:0 | 4:4:4|
                |ハフマンテーブル数 | 4     | 8    |
                |量子化テーブル数   | 4     | 4    |

                プログレッシブ方式のJPEGデータのデコードには、ベースライン方式と比べてより多くのワークメモリを要します。
                従って、もしアプリケーションがベースラインJPEGだけを対象とする場合、プログレッシブJPEG用のワークメモリサイズを考慮する必要はありません。
                一方でプログレッシブJPEGも対象とする場合は、プログレッシブJPEG用のワークメモリサイズを予約してください。

                また不特定のJEPGデータをデコードする際には、エンコードされたときのパラメータの組み合わせにより、上記の方法で得られたワークバッファ量では
                不足することがあります。
                不特定のJPEGデータをデコード対象に含める場合、この可能性を考慮してアプリケーションを設計してください。

                ### オブジェクトの内部状態

                JpegDecoder オブジェクトはその内部で次の3つの状態を持ちます。
                - JPEGデータ未登録状態
                    - JpegDecoder オブジェクトにJPEGデータが設定されていない状態です。
                - JPEGデータ登録済み状態
                    - JpegDecoder オブジェクトにJPEGデータが設定されている一方、JPEGデータやパラメータが解析されていない状態です。
                - JPEGデータ解析済み状態
                    - JpegDecoder オブジェクトに最後に設定されたJPEGデータとパラメータが解析されたあとの状態です。

                ### 並列処理について
                JpegDecoder クラスのオブジェクトを複数生成して、複数のJPEGデータを並行してデコードすることが可能です。
*/
class JpegDecoder
{
    NN_DISALLOW_COPY(JpegDecoder);
public:
    /**
        @brief      JpegDecoder クラスのインスタンスを作成します。

        @post       SetResolutionDenominator() で設定可能なスケーリング設定は1に初期化されます。
                    作成されるオブジェクトの内部状態は「JPEGデータ未登録状態」となります。
    */
    JpegDecoder() NN_NOEXCEPT;

    /**
        @brief  JpegDecoder クラスのインスタンスを破棄します。
    */
    virtual ~JpegDecoder() NN_NOEXCEPT;

    //! @name デコード設定
    //! @{
    /**
        @brief      デコードを行うJPEGデータを登録します。

        @param[in]  jpegData    デコード対象のJPEGデータを指定します。
        @param[in]  jpegSize    'jpegData' の大きさ(バイト数)を指定します。

        @post       「JPEGデータ登録済み状態」となり、 Analyze() を呼ぶことができます。
        @details    デコード対象のJPEGデータを指定します。
                    指定したJPEGデータは Analyze() および Decode() で参照されます。
                    このJPEGデータを外部から変更すると、これらの関数の挙動に影響を及ぼすことに注意してください。
    */
    void SetImageData(const void *jpegData, const size_t jpegSize) NN_NOEXCEPT;

    /**
        @brief      デコード時のスケーリングを設定します。

        @param[in]  denom   スケーリングの分母として、 1, 2, 4, 8, 16 のいずれかを指定します。

        @pre        'denom' として 1, 2, 4, 8, 16 のいずれかを指定する必要があります。
        @post       もし JpegDecoder オブジェクトが「JPEGデータ解析済み状態」であれば、「JPEGデータ登録済み状態」となります。
                    それ以外の状態からは変化しません。

        @details    画像のスケーリングの分母 N を指定すると、 Decode() で出力されるピクセルデータは本来の大きさに比べ
                    縦横にそれぞれ 1/N 倍されます。
                    ただし、スケーリング値の実効値は、 SetImageData() で設定したJPEGデータのブロックサイズまでとなります。
                    つまり、ブロックサイズが 8 のJPEGデータのスケーリング値に 16 を指定しデコードした場合は、縦横それぞれ
                    が 1/8 倍されるに留まります。

                    デフォルト値である 1 を使用する場合や Exif 情報の取得が目的の場合、この設定は必須ではありません。
    */
    void SetResolutionDenominator(const int denom) NN_NOEXCEPT;

    /**
        @brief      デコード時のピクセルフォーマットを設定します。

        @param[in]  pxFormat    デコード後のピクセルデータのピクセルフォーマットを nn::image::PixelFormat 列挙子で指定します。

        @pre        'pxFormat' として nn::image::PixelFormat 列挙子で定義済みの値を指定する必要があります。
        @post       もし JpegDecoder オブジェクトが「JPEGデータ解析済み状態」であれば、「JPEGデータ登録済み状態」となります。
                    それ以外の状態からは変化しません。

        @details    Decode() で出力されるピクセルデータが、本関数で指定されたピクセルフォーマットになります。
                    本関数を呼ばない場合の初期値は nn::image::PixelFormat_Rgba32 です。

                    ピクセルフォーマットに nn::image::PixelFormat_Rgb24 を指定すると、それ以外のピクセルフォーマットを指定した場合と比べ必要なワークメモリが小さくなります。
    */
    void SetPixelFormat(PixelFormat pxFormat) NN_NOEXCEPT;
    //! @}
    // ~デコード設定

    //! @name 中間情報の取得
    //! @{
    /**
        @brief      設定されたJPEGデータおよびスケーリング設定を解析し、内部パラメータを構築します。

        @return     処理結果を返します。
        @retval     nn::image::JpegStatus_Ok            処理が正常に終了しました。
        @retval     nn::image::JpegStatus_WrongFormat   JPEGデータとして不正なフォーマットのデータが設定されています。
                                                        このJPEGデータをデコードすることはできません。
        @retval     nn::image::JpegStatus_UnsupportedFormat
                                                        JpegDecoder クラスがサポートしていないフォーマットのデータが設定されています。
                                                        このJPEGデータには非対応のためデコードすることはできません。

        @pre        「JPEGデータ登録済み状態」か「JPEGデータ解析済み状態」であることが必要です。
        @post       JpegStatus_Ok が返るとき 「JPEGデータ解析済み状態」 となります。

        @details    設定されたJPEGデータとスケーリング設定を解析し、JPEGデータのデコードに必要な情報を抽出することで
                    デコードに備えます。
    */
    JpegStatus Analyze() NN_NOEXCEPT;

    /**
        @brief      Analyze() によって計算された、デコード後のピクセルデータの大きさ(幅と高さ)を取得します。

        @return     画像の大きさ(幅,高さ)を返します。

        @pre        「JPEGデータ解析済み状態」であることが必要です。

        @details    デコードで得られる出力画像のピクセル単位の実寸を返します。
                    SetResolutionDenominator() でスケーリングを設定している場合、スケーリング後の値が返ります。
    */
    const Dimension GetAnalyzedDimension() const NN_NOEXCEPT;

    /**
        @brief      Analyze() によって計算された、 Decode() 用のワークメモリの大きさ(バイト数)を取得します。

        @return     Decode() に与える必要のあるワークメモリの大きさ(バイト数)が返ります。

        @pre        「JPEGデータ解析済み状態」であることが必要です。

        @details    ワークメモリはデコード対象のJPEGデータおよびスケーリング設定によって大きく変動します。
                    JpegDecoder クラスの説明欄にワークメモリの大きさについて説明を記載しています。
                    その説明に従って、必要なワークメモリの大きさを計算し、予約してください。
    */
    size_t GetAnalyzedWorkBufferSize() const NN_NOEXCEPT;

    /**
        @brief      Analyze() によって発見された、Exif情報が格納された領域(アドレスと大きさ)を取得します。

        @param[out] pOutExifSize    Exif情報バイナリの大きさ(バイト数)が格納されます。
                                    Exifの仕様上、大きさは最大65527バイトであることが保証されます。

        @return     nn::image::ExifExtractor クラスでの解析対象となるExifバイナリの場所が返ります。
                    JPEGデータ中にExifバイナリが存在しない場合は nullptr が返ります。

        @pre        「JPEGデータ解析済み状態」であることが必要です。

        @details    この関数は、 SetImageData() で指定されたバッファ中のExif情報バイナリのアドレスと大きさ(バイト数)を返します。
                    アドレスが指すデータはコピーされないため、 nn::image::ExifExtractor クラスでの使用の完了前に
                    JPEGデータを変更すると正しい解析結果が保証されなくなります。
    */
    const void* GetAnalyzedExifData(size_t *pOutExifSize) const NN_NOEXCEPT;
    //! @}
    // ~中間情報の取得

    //! @name デコード処理
    //! @{
    /**
        @brief      JPEGデータをデコードし、与えられた出力バッファにピクセルデータを書き込みます。

        @param[out] outPixelBuf     出力されるピクセルデータの格納先を指定します。
                                    1ピクセルは SetPixelFormat() で指定したピクセルフォーマットで出力されます。
                                    SetPixelFormat() を呼んでいない場合、32bitのRGBAデータとして出力されます。
        @param[in]  pixelBufSize    outPixelBuf の大きさ(バイト数)を指定します。
        @param[in]  lineAlignment   outPixelBuf の行アラインメントを指定します。
                                    たとえば 129px 幅の画像を 130px 幅のピクセルバッファに出力する場合は2を指定します。
        @param[in]  workBuf         ワークメモリを指定します。
        @param[in]  workBufSize     ワークメモリの大きさ(バイト数)を指定します。

        @return     処理結果を返します。
        @retval     nn::image::JpegStatus_Ok          処理が正常に終了しました。
        @retval     nn::image::JpegStatus_WrongFormat JPEGデータとして不正なフォーマットのデータが設定されています。
                                                        このデータをデコードすることはできません。
        @retval     nn::image::JpegStatus_UnsupportedFormat
                                                        JpegDecoder クラスがサポートしていないフォーマットのデータが設定されています。
                                                        このJPEGデータには非対応のためデコードすることはできません。
        @retval     nn::image::JpegStatus_OutOfMemory デコード中にワークメモリが不足しました。
                                                        想定していないJPEGデータが入力された可能性があります。
                                                        このJPEGデータには非対応のためデコードすることはできません。

        @pre        次の条件をすべて満たすことが必要です。
                    - 'outPixelBuf' は「画像幅のlineAlignmentアラインメント」×「画像高さ」×「1ピクセルあたりのバイト数」以上の大きさである。
                    - 'workBuf' は GetAnalyzedWorkBufferSize() で取得できる値以上の大きさである。
                    - 'workBuf' のアドレスが std::max_align_t のアラインメントに沿っている。
                    - 「JPEGデータ解析済み状態」である。
        @post       本関数がアルファチャンネルを含むピクセルフォーマットでピクセルデータを出力する場合、 'outPixelBuf' のアルファチャンネルの値は 255 に固定されます。
                    また、アラインメントのためのパディング領域には何も書き込まれません。

        @details    設定されたJPEGデータとスケーリング設定を使用し、JPEGデータをデコードします。
                    設定されたJPEGデータが不正な場合やサポートしていない形式の場合、 JpegStatus_Ok 以外の値を返します。

                    Analyze() で nn::image::JpegStatus_Ok となったJPEGデータが、 Decode() でエラーを返すことがあります。
                    これは Analyze() が参照しない情報を Decode() が使用しているためです。
                    従って Analyze() と Decode() の両方でエラーチェックを行う必要があります。
    */
    JpegStatus Decode(
        void *outPixelBuf,
        const size_t pixelBufSize,
        const int lineAlignment,
        void *workBuf,
        const size_t workBufSize) NN_NOEXCEPT;
    //! @}
    // ~デコード処理

    /**
        @brief      指定した JpegDecoder オブジェクトに構築済みの内部パラメータを使用してJPEGデータをデコードします。

        @param[out] outPixelBuf     出力されるピクセルデータの格納先を指定します。
                                    1ピクセルは 'decoder' の SetPixelFormat() で指定したピクセルフォーマットで出力されます。
                                    'decoder' の SetPixelFormat() を呼んでいない場合、32bitのRGBAデータとして出力されます。
        @param[in]  pixelBufSize    outPixelBuf の大きさ(バイト数)を指定します。
        @param[in]  lineAlignment   outPixelBuf の行アラインメントを指定します。
                                    たとえば 129px 幅の画像を 130px 幅のピクセルバッファに出力する場合は2を指定します。
        @param[in]  jpegData        デコード対象のJPEGデータを指定します。
        @param[in]  jpegSize        'jpegData' の大きさ(バイト数)を指定します。
        @param[in]  decoder         「JPEGデータ解析済み状態」の nn::image::JpegDecoder オブジェクトを指定します。
        @param[in]  workBuf         ワークメモリを指定します。
        @param[in]  workBufSize     ワークメモリの大きさ(バイト数)を指定します。

        @return     処理結果を返します。
        @retval     nn::image::JpegStatus_Ok          処理が正常に終了しました。
        @retval     nn::image::JpegStatus_WrongFormat JPEGデータとして不正なフォーマットのデータが指定されました。
                                                        このデータをデコードすることはできません。
        @retval     nn::image::JpegStatus_UnsupportedFormat
                                                        JpegDecoder クラスがサポートしていないフォーマットのデータが指定されました。
                                                        このJPEGデータには非対応のためデコードすることはできません。
        @retval     nn::image::JpegStatus_OutOfMemory
                                                        デコード中にワークメモリが不足しました。
                                                        想定していないJPEGデータが入力された可能性があります。

        @pre        次の条件をすべて満たすことが必要です。
                    - 'outPixelBuf' は「画像幅のlineAlignmentアラインメント」×「画像高さ」×「1ピクセルあたりのバイト数」以上の大きさである。
                    - 'jpegData' は 'decoder' の SetImageData() に指定したJPEGデータと同じ大きさ(幅と高さ)である。
                    - 'decoder' は「JPEGデータ解析済み状態」である。
                    - 'workBuf' は 'decoder' の GetAnalyzedWorkBufferSize() で取得できる値以上の大きさである。
                    - 'workBuf' のアドレスが std::max_align_t のアラインメントに沿っている。
        @post       本関数がアルファチャンネルを含むピクセルフォーマットでピクセルデータを出力する場合、 'outPixelBuf' のアルファチャンネルの値は 255 に固定されます。
                    また、アラインメントのためのパディング領域には何も書き込まれません。

        @details    指定された 'decoder' がもつ内部パラメータやデコード設定に基づきJPEGデータをデコードします。
                    デコード時に必要なJPEGデータとデコード設定の解析を省略することで、 複数の画像を効率的にデコードできます。

                    本関数は 'decoder' の SetImageData() に指定されたJPEGデータと同じ大きさ(幅と高さ)で同様のエンコードパラメータをもつJPEGデータが入力されることを想定しています。
                    このようなJPEGデータに対して、本関数は通常 GetAnalyzedWorkBufferSize() で得られる大きさ(バイト数)のワークメモリでデコードを完了します。
                    そうでないJPEGデータを指定すると本関数内部でワークメモリが不足し、 nn::image::JpegStatus_OutOfMemory を返す場合があることに留意してください。

                    'decoder' の Analyze() で nn::image::JpegStatus_Ok となったJPEGデータに対し、本関数がエラーを返すことがあります。
                    これは Analyze() が参照しない情報を本関数が使用しているためです。
                    従って本関数の返り値についてもエラーチェックを行う必要があります。
    */
    static JpegStatus DecodeWithPreconfiguredDecoder(
        void* outPixelBuf,
        const size_t pixelBufSize,
        int lineAlignment,
        const void* jpegData,
        size_t jpegSize,
        const JpegDecoder& decoder,
        void* workBuf,
        size_t workBufSize) NN_NOEXCEPT;

    /**
        @brief      与えられたJPEGデータのヘッダを解析し、 Exif情報が格納された領域(アドレスと大きさ)を取得します。

        @param[out] pOutPointer     nn::image::ExifExtractor クラスでの解析対象となるExifバイナリの先頭アドレスが格納されます。
        @param[out] pOutExifSize    Exif情報バイナリの大きさ(バイト数)が格納されます。
                                    Exifの仕様上、大きさは最大65527バイトであることが保証されます。
        @param[in]  jpegData        解析対象のJPEGデータを指定します。
        @param[in]  jpegSize        'jpegData' の大きさ(バイト数)を指定します。

        @return     処理結果を返します。
        @retval     nn::image::JpegStatus_Ok            処理が正常に終了しました。
        @retval     nn::image::JpegStatus_WrongFormat   JPEGデータからExif情報が格納された領域を特定できません。
                                                        JpegDecoder クラスがサポートしていないフォーマットのデータが指定されているか、
                                                        JPEGデータがExif情報を含んでいません。

        @details    与えられた JPEG データのヘッダを解析し、 Exif情報が格納された領域(アドレスと大きさ)を取得します。
                    ただしExif情報が格納された領域の取得に必要な解析以外は行いません。

                    この関数は JpegDecoder インスタンスを生成し Analyze() 関数を使用する場合と比べ高速です。
                    JPEG データから Exif情報の領域のみを取り出したい場合には、本関数の使用を推奨します。
    */
    static JpegStatus GetExifData(
        const void** pOutPointer,
        size_t* pOutExifSize,
        const void *jpegData,
        size_t jpegSize) NN_NOEXCEPT;

private:
    enum Stage
    {
        Stage_Unregistered = 0,
        Stage_Registered = 1,
        Stage_Analyzed = 2
    };
    Stage m_Stage;

    const void *m_JpegData;
    size_t m_JpegSize;
    int m_Denominator;
    PixelFormat m_PixelFormat;

    Dimension m_Dim;
    size_t m_BufSize;
    bool m_IsExifFound;
    const void *m_ExifData;
    uint16_t m_ExifSize;
};

}}
