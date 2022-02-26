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

#include <nn/image/image_JpegCommon.h>

/**
@file
@brief Exif情報の生成機能に関わる定義
*/

namespace nn { namespace image {

/**
    @brief      Exif情報の構築を行います。

    @details    JPEG画像のメタ情報を保持するための、Exifフォーマットのバイナリを生成するクラスです。
                あらかじめ定義されたフィールドを任意に選択し、アプリケーション固有の情報を設定することができます。
                対応するExifフォーマットのバージョンは2.3です。

                ### 作成可能なExifバイナリのサイズ上限の見積もり
                Exifフォーマットの仕様上、出力されるバイナリは最大で65527バイトになります。
                最終的な出力サイズがこれを超えないよう、設定するメタ情報を選択してください。

                ### オブジェクトの内部状態
                ExifBuilder オブジェクトはその内部で次の2つの状態を持ちます。
                - メタ情報未解析状態
                    - ExifBuilder に設定されたメタ情報が解析されていない状態です。
                - メタ情報解析済み状態
                    - ExifBuilder に設定されたメタ情報が解析されたあとの状態です。
                        この状態のオブジェクトは、 nn::image::JpegEncoder::Encode() の引数に指定できます。
*/
class ExifBuilder
{
    NN_DISALLOW_COPY(ExifBuilder);
public:
    /**
        @brief      ExifBuilder オブジェクトが生存中に使用するワークメモリの大きさを取得します。

        @return     コンストラクタに与える必要のあるワークメモリの大きさ(バイト数)が返ります。

        @details    ワークメモリの大きさは固定です。
                    ただし、SDK のバージョンによって異なることがあります。
    */
    static size_t GetWorkBufferSize() NN_NOEXCEPT;

    /**
        @brief      ExifBuilder クラスのインスタンスを作成します。

        @param[in]  workBuf     ワークメモリを指定します。
                                このワークメモリは ExifBuilder オブジェクトの生存中は解放したり内容を変更しないでください。
        @param[in]  workBufSize ワークメモリの大きさ(バイト数)を指定します。

        @pre        次の条件をすべて満たすことが必要です。
                    - 'workBuf' は GetWorkBufferSize() で取得できる値以上の大きさである。
                    - 'workBuf' のアドレスが std::max_align_t のアラインメントに沿っている。
        @post       作成されるオブジェクトの内部状態は「メタ情報未解析状態」となります。
    */
    ExifBuilder(
        void *workBuf,
        const size_t workBufSize) NN_NOEXCEPT;

    /**
        @brief  ExifBuilder クラスのインスタンスを破棄します。
    */
    virtual ~ExifBuilder() NN_NOEXCEPT;


    //! @name メタ情報設定
    //! @{
    /**
        @brief      画像の撮影時点での、撮影機器の向きを設定します。

        @param[in]  orientation 画像の向きを指定します。

        @post       「メタ情報未解析状態」となります。

        @details    0th IFD TIFFタグの「画像方向(0x0112)」を設定します。
                    画像方向は一般的な表示アプリケーションから参照され得るため、
                    撮影機器の向きにもっとも近い値を設定することを推奨します。
                    たとえば撮影機器を時計回りに 90 度回転し撮影した場合には、
                    ExifOrientation_Rotate270 (画像が時計回りに270度回転) を指定してください。

                    撮影機器の向きを記録しない場合、この設定は必須ではありません。
                    その場合は一般に、画像は回転していないものとして取り扱われます。
    */
    void SetOrientation(const ExifOrientation orientation) NN_NOEXCEPT;

    /**
        @brief      JPEGデータを作成したソフトウェアの名称を設定します。

        @param[in]  software    ソフトウェアの名称を格納した文字列配列を指定します。 nullptr を指定すると設定を削除します。
        @param[in]  size        終端文字を含めた 'software' の大きさ(バイト数)を指定します。

        @pre        'software' が非 nullptr である場合、
                    'software' が指す文字列は ASCII(0x00～0x7F) で構成され '\0' で終端されている必要があります。
        @post       「メタ情報未解析状態」となります。

        @details    0th IFD TIFFタグの「作成ソフトウェア名(0x0131)」を設定します。
                    任意長の文字列を設定できますが、Exifバイナリのサイズ上限に注意してください。

                    ExifBuilder オブジェクトはこの関数で指定した文字列バッファのアドレスを保持します。
                    バッファの内容を変更する際には注意が必要です。

                    ソフトウェア名称を記録しない場合、この設定は必須ではありません。
    */
    void SetSoftware(const char *software, const size_t size) NN_NOEXCEPT;

    /**
        @brief      画像の撮影日時を設定します。

        @param[in]  dateTime    画像の撮影日時を規定されたフォーマットで指定します。nullptrを指定すると設定を削除します。
        @param[in]  size        終端文字を含めた'dateTime'の大きさ(バイト数)を指定します。

        @pre        次の条件をすべて満たすことが必要です。
                    - 'dateTime' が非 nullptr である場合、
                        'dateTime' が指す文字列は ASCII(0x00～0x7F)が 19 個連続し '\0' で終端されている。
                    - 'dateTime' が非 nullptr である場合、 'size' は 20 以上の値である。
        @post       「メタ情報未解析状態」となります。

        @details    0th IFD TIFFタグの「作成日時(0x0132)」を設定します。
                    ExifBuilderクラスは終端文字の有無のみを検査し、内容は考慮しません。

                    Exif情報の仕様に適合するために、文字列は "YYYY:MM:DD hh:mm:ss" のフォーマットに従うことを推奨します。
                    ただし、 ExifBuilder クラスはフォーマットの検査を行いません。
                    - YYYY … 撮影年(西暦)、4桁の整数で指定します。
                    - MM … 撮影月(01～12)、2桁の整数で指定します。
                    - DD … 撮影日(01～31)、2桁の整数で指定します。
                    - hh … 撮影時(00～23)、2桁の整数で指定します。
                    - mm … 撮影分(00～59)、2桁の整数で指定します。
                    - ss … 撮影秒(00～59)、2桁の整数で指定します。

                    上記のフィールドのいずれの要素も省略可能です。
                    @htmlonly <pre style="font-family:monospace;"> @endhtmlonly "2015:04:09 12:00:  " @htmlonly </pre> @endhtmlonly
                    省略する場合はこのように、省略するフィールドに空白(0x20)を書き込んでください。

                    ExifBuilder オブジェクトはこの関数で指定した撮影日時バッファのアドレスを保持します。
                    バッファの内容を変更する際には注意が必要です。

                    撮影日時を記録しない場合、この設定は必須ではありません。
    */
    void SetDateTime(const char *dateTime, const size_t size) NN_NOEXCEPT;

    /**
        @brief      任意のメーカーノートを記録します。

        @param[in]  note    任意のフォーマットのバイナリデータを指定します。nullptrを指定すると設定を削除します。
        @param[in]  size    メーカーノートの大きさ(バイト数)

        @post       「メタ情報未解析状態」となります。

        @details    0th IFD Exifタグの「メーカーノート(0x927C)」を設定します。
                    任意長のバイナリ列を設定できますが、Exifバイナリのサイズ上限に注意してください。

                    ExifBuilder オブジェクトはこの関数で指定したメーカーノートバッファのアドレスを保持します。
                    バッファの内容を変更する際には注意が必要です。

                    メーカーノート記録しない場合、この設定は必須ではありません。
    */
    void SetMakerNote(const void *note, const size_t size) NN_NOEXCEPT;

    /**
        @brief      画像のユニークIDを設定します。

        @param[in]  uniqueId    画像固有のIDを規定されたフォーマットで指定します。 nullptr を指定すると設定を削除します。
        @param[in]  size        終端文字を含めた'uniqueId'の大きさ(バイト数)を指定します。

        @pre        次の条件をすべて満たすことが必要です。
                    - 'uniqueId' が非nullptrである場合、
                        'uniqueId' が指す文字列は ASCII(0x00～0x7F) が 32 個連続し '\0' で終端されている。
                    - 'uniqueId' が非nullptrである場合、 'size' は 33 以上の値である。
        @post       「メタ情報未解析状態」となります。

        @details    0th IFD Exifタグの「ユニークID(0xA420)」を設定します。
                    このときExif情報の仕様に適合するために、指定する文字列は 128bit ユニーク値の16進数表記(大小文字は問わず)であることを推奨します。
                    ただし、 ExifBuilder クラスはフォーマットやユニーク性の検査を行いません。

                    ExifBuilder オブジェクトはこの関数で指定したユニークIDバッファのアドレスを保持します。
                    バッファの内容を変更する際には注意が必要です。

                    ユニークIDを記録しない場合、この設定は必須ではありません。
    */
    void SetUniqueId(const char *uniqueId, const size_t size) NN_NOEXCEPT;

    /**
        @brief      画像のサムネイルを記録します。

        @param[in]  thumbnail   サムネイル画像のJPEGバイナリデータを指定します。nullptrを指定すると設定を削除します。
        @param[in]  size        サムネイル画像の大きさ(バイト数)

        @post       「メタ情報未解析状態」となります。

        @details    1st IFD TIFFタグの「サムネイル画像のオフセット(0x0201)」および「サムネイル画像のサイズ(0x0202)」を設定します。
                    任意長のバイナリ列を設定できますが、Exifバイナリのサイズ上限に注意してください。
                    このときExif情報の仕様に適合するために、サムネイル画像にはExif情報を含めないことを推奨します。
                    ただし、 ExifBuilder クラスは JPEG フォーマットの検査を行いません。

                    ExifBuilder オブジェクトはこの関数で指定したサムネイルバッファのアドレスを保持します。
                    バッファの内容を変更する際には注意が必要です。

                    サムネイルを記録しない場合、この設定は必須ではありません。
    */
    void SetThumbnail(const void *thumbnail, const size_t size) NN_NOEXCEPT;
    //! @}
    // ~メタ情報設定

    //! @name 中間情報の取得
    //! @{
    /**
        @brief      設定されたメタ情報を解析し、内部パラメータを構築します。

        @return     処理結果を返します。
        @retval     nn::image::JpegStatus_Ok    処理が正常に終了しました。
        @retval     nn::image::JpegStatus_OutputOverabounds
                                                指定したソフトウェア名やメーカーノート、サムネイルが大きすぎます。

        @post       「メタ情報解析済み状態」となります。
                    nn::image::JpegEncoder::Encode() の引数として ExifBuilder オブジェクトを指定できます。

        @details    設定されたメタ情報を解析し、Exif情報のバイナリ列の出力に必要な内部パラメータを計算します。
                    このとき、設定されたメタ情報によって出力バイナリ列があふれることがわかると、 JpegStatus_OutputOverabounds を返します。
                    Exifフォーマットの仕様上、作成可能なバイナリの大きさは最大65527バイトです。

                    またこのとき ExifBuilder は、設定されたメタ情報の数や大きさのみを検査し、内容は考慮しません。
                    従って全てのフィールドについてサイズが既知のとき、はじめにバッファのみを登録して Analyze() を実施し、
                    フィールドの内容を Build() の直前に変更することも可能です。
                    これにより Analyze() に係るアプリケーション全体の計算コストを最小化できます。
    */
    JpegStatus Analyze() NN_NOEXCEPT;

    /**
        @brief      Analyze()によって計算された、Exif情報の出力サイズ(バイト数)を取得します。

        @return     Exif情報の出力サイズ(バイト数)が返ります。
                    Exifの仕様上、出力サイズは最大65527バイトであることが保証されます。

        @pre        「メタ情報解析済み状態」であることが必要です。
    */
    size_t GetAnalyzedOutputSize() const NN_NOEXCEPT;

    /**
        @brief      Build()を呼び出し可能かを取得します。

        @return     Build()の呼び出しが可能な状態であればtrueを返し、そうでなければfalseを返します。
    */
    bool IsBuildable() const NN_NOEXCEPT
    {
        return m_Stage >= Stage_Analyzed;
    }
    //! @}
    // ~中間情報の取得

    //! @name Exif情報のバイナリの出力
    //! @{
    /**
        @brief      指定されたバッファにExif情報を出力します。

        @param[out] outExifBuf      Exif情報の出力先バッファを指定します。
        @param[in]  exifBufSize     outExifBufの大きさ(バイト数)を指定します。
        @param[in]  dimension       Exif情報が言及するJPEGデータの大きさ(幅と高さ)を指定します。
                                    この情報は0th IFD Exifタグの「実効画像幅(0xA002)」および「実効画像高さ(0xA003)」として
                                    出力されます。

        @pre        次の条件をすべて満たすことが必要です。
                    - 'outExifBuf' は GetAnalyzedOutputSize() で取得できる値以上の大きさである。
                    - 「メタ情報解析済み状態」である。

        @details    下記に示す標準のメタ情報と Analyze() の呼び出し以前に設定されたメタ情報からExif情報のバイナリを生成し、
                    指定されたバッファに出力します。
                    このとき、文字列データはすべて'\0'で終端され、また 0x7F より値の大きな文字は 0x20 に置換され出力されます。

                    - 0th IFD TIFFタグ
                        - メーカー名(0x010F) = "Nintendo co., ltd"
                        - モデル名(0x0110) = (未定)
                        - X方向の解像度(0x011A) = 72
                        - Y方向の解像度(0x011B) = 72
                        - 解像度の単位(0x0128) = dpi
                        - YCbCrサンプリング位置(0x0213) = 中央
                    - 0th IFD Exifタグ
                        - Exifバージョン(0x9000) = 2.3.0
                        - ピクセル構成(0x9101) = YCbCr
                        - FlashPixバージョン(0xA000) = 1.0.0
                        - 色空間(0xA001) = sRGB

                    これらの標準のメタ情報は変更できません。
                    またサムネイル画像が設定されている場合は、サムネイル画像について下記のメタ情報が付加されます。

                    - 1st IFD TIFFタグ
                        - 圧縮形式(0x0103) = JPEG
                        - X方向の解像度(0x011A) = 72
                        - Y方向の解像度(0x011B) = 72
                        - 解像度の単位(0x0128) = dpi
    */
    void Build(
        void *outExifBuf,
        const size_t exifBufSize,
        const Dimension &dimension) NN_NOEXCEPT;
    //! @}
    // ~Exif情報のバイナリの出力

private:
    enum Stage
    {
        Stage_Registered = 1,
        Stage_Analyzed = 2
    };
    Stage m_Stage;

    void *m_WorkBuf;

    uint16_t m_OutputSize;
};

}}
