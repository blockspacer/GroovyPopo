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
@brief Exif情報の解析機能に関わる定義
*/

namespace nn { namespace image {

/**
    @brief      Exif情報の解析を行います。

    @details    JPEG画像のメタ情報を取得するための、Exifフォーマットのバイナリを解析するクラスです。
                対応するExifフォーマットのバージョンは2.3です。

                ### 取得される情報の正当性について
                ExifExtractor クラスで取得可能な文字列やバイナリデータについて、 ExifExtractor クラスは内容の正当性を保証しません。
                文字列の'\0'による終端および、値域が定められた要素の値域のみを検査します。
                従って、外部/内部でのJPEGデータの改変に伴う情報の破損や脅威の検査はアプリケーションが行う必要があります。
                各値について ExifExtractor クラスがどのような検査を行うか、詳細は各関数の説明を参照してください。

                ### オブジェクトの内部状態
                ExifExtractor オブジェクトはその内部で次の3つの状態を持ちます。
                - Exifデータ未登録状態
                    - ExifExtractor オブジェクトにExif情報のバイナリデータが設定されていない状態です。
                - Exifデータ登録済み状態
                    - ExifExtractor オブジェクトにExif情報のバイナリデータが設定されており、解析されていない状態です。
                - Exifデータ解析済み状態
                    - ExifExtractor オブジェクトに最後に設定されたExif情報のバイナリデータが解析されたあとの状態です。
*/
class ExifExtractor
{
    NN_DISALLOW_COPY(ExifExtractor);
public:
    /**
        @brief      ExifExtractor オブジェクトが生存中に使用するワークメモリの大きさを取得します。

        @return     コンストラクタに与える必要のあるワークメモリの大きさ(バイト数)が返ります。

        @details    ワークメモリの大きさは固定です。
                    ただし、SDK のバージョンによって異なることがあります。
    */
    static size_t GetWorkBufferSize() NN_NOEXCEPT;

    /**
        @brief      ExifExtractor クラスのインスタンスを作成します。

        @param[in]  workBuf     ワークメモリを指定します。
                                このワークメモリは ExifBuilderオ ブジェクトの生存中は解放したり内容を変更しないでください。
        @param[in]  workBufSize ワークメモリの大きさ(バイト数)を指定します。

        @pre        次の条件をすべて満たすことが必要です。
                    - 'workBuf' は GetWorkBufferSize() で取得できる値以上の大きさである。
                    - 'workBuf' のアドレスが std::max_align_t のアラインメントに沿っている。
        @post       作成される ExifExtractor の内部状態は「Exif情報未登録状態」となります。
    */
    ExifExtractor(
        void *workBuf,
        const size_t workBufSize) NN_NOEXCEPT;

    /**
        @brief  ExifExtractor クラスのインスタンスを破棄します。
    */
    virtual ~ExifExtractor() NN_NOEXCEPT;

    //! @name メタ情報の解析
    //! @{
    /**
        @brief      解析を行うExif情報のバイナリを登録します。

        @param[in]  exifData    解析対象のExifデータを指定します。
        @param[in]  exifSize    'exifData'の大きさ(バイト数)を指定します。

        @pre        Exifの仕様上、 'exifData' は最大で 65527 バイトに収まる必要があります。
        @post       「Exifデータ登録済み状態」となります。

        @details    解析を行うExif情報のバイナリデータを指定します。
                    本関数呼び出し後、インスタンスを破棄するか再度本関数を呼び出すまで、ここで登録したexifDataの
                    内容を変更しないでください。
    */
    void SetExifData(const void *exifData, const size_t exifSize) NN_NOEXCEPT;

    /**
        @brief      設定されたExif情報のバイナリを解析し、Exif情報の構造を計算します。

        @return     処理結果を返します。
        @retval     nn::image::JpegStatus_Ok            処理が正常に終了しました。
        @retval     nn::image::JpegStatus_WrongFormat   不正なフォーマットのExif情報のバイナリが設定されています。
                                                        このバイナリのメタ情報は取得できません。

        @pre        「Exifデータ登録済み状態」か「Exifデータ解析済み状態」であることが必要です。
        @post       JpegStatus_Ok が返るとき、「Exifデータ解析済み状態」となります。

        @details    設定されたExif情報のバイナリを解析し、各メタデータの取得に必要な、Exif情報の構造を取得します。
                    このとき不正なExif情報のバイナリが渡されていると、 JpegStatus_WrongFormat を返します。

                    この関数はExif情報の構造についてのみ検査を行い、内容は考慮しません。
                    従ってこの関数が JpegStatus_Ok を返した場合でも、 ExtractMaker() など Extract 系の関数で
                    有効な情報を取得できることは保証されません。
    */
    JpegStatus Analyze() NN_NOEXCEPT;
    //! @}
    // ~メタ情報の解析

    //! @name 解析したメタ情報の取得
    //! @{
    /**
        @brief      JPEGデータの撮影機器のメーカー名を取得します。

        @param[out] pOutSize    終端の'\0'を含むメーカー名の大きさ(バイト数)が格納されます。

        @return     メーカー名を指すポインタが返ります。取得できない場合、 nullptr が返ります。

        @pre        「Exifデータ解析済み状態」であることが必要です。

        @details    0th IFD TIFFタグの「メーカー名(0x010F)」を取得します。
                    取得できる文字列は次の観点で検査されています。
                    - 全ての文字が0x00から0x7Fの範囲に含まれること。
                    - 終端が'\0'であること。

                    この検査に適合しない場合、値は存在しないと見なされます。
    */
    const char* ExtractMaker(size_t *pOutSize) const NN_NOEXCEPT;

    /**
        @brief      JPEGデータの撮影機器の機器名を取得します。

        @param[out] pOutSize    終端の'\0'を含む機器名の大きさ(バイト数)が格納されます。

        @return     機器名を指すポインタが返ります。取得できない場合、nullptrが返ります。

        @pre        「Exifデータ解析済み状態」であることが必要です。

        @details    0th IFD TIFFタグの「撮影機器名(0x0110)」を取得します。
                    取得できる文字列は次の観点で検査されています。
                    - 全ての文字が0x00から0x7Fの範囲に含まれること。
                    - 終端が'\0'であること。

                    この検査に適合しない場合、値は存在しないと見なされます。
    */
    const char* ExtractModel(size_t *pOutSize) const NN_NOEXCEPT;

    /**
        @brief      画像の撮影時点での、撮影機器の向きを取得します。

        @param[out] pOutValue   画像の向きが格納されます。

        @return     値が存在すればtrueを、そうでなければfalseを返します。

        @pre        「Exifデータ解析済み状態」であることが必要です。

        @details    0th IFD TIFFタグの「画像方向(0x0112)」を取得します。
                    画像に適切な方向が設定されてる場合、この値から正立画像を計算することができます。
                    たとえば撮影機器を時計回りに90度回転し撮影された画像には、通常
                    ExifOrientation_Rotate270 が設定されており、これを時計回りに -270 度回転する
                    ことで正立画像を取得できます。

                    「画像方向(0x0112)」に未定義の値が設定されている場合、存在しないと見なされます。
    */
    bool ExtractOrientation(ExifOrientation *pOutValue) const NN_NOEXCEPT;

    /**
        @brief      JPEGデータの撮影に使用されたソフトウェアの名称を取得します。

        @param[out] pOutSize    終端の'\0'を含むソフトウェア名の大きさ(バイト数)が格納されます。

        @return     ソフトウェア名を指すポインタが返ります。取得できない場合、nullptrが返ります。

        @pre        「Exifデータ解析済み状態」であることが必要です。

        @details    0th IFD TIFFタグの「作成ソフトウェア名(0x0131)」を文字列として取得します。
                    取得できる文字列は次の観点で検査されています。
                    - 全ての文字が0x00から0x7Fの範囲に含まれること。
                    - 終端が'\0'であること。

                    この検査に適合しない場合、値は存在しないと見なされます。
    */
    const char* ExtractSoftware(size_t *pOutSize) const NN_NOEXCEPT;

    /**
        @brief      JPEGデータの撮影日時を文字列として取得します。

        @param[out] pOutSize    20が格納されます。

        @return     日時情報を指すポインタが返ります。取得できない場合、nullptrが返ります。

        @pre        「Exifデータ解析済み状態」であることが必要です。

        @details    0th IFD TIFFタグの「作成日時(0x0132)」を文字列として取得します。
                    取得できる文字列は次の観点で検査されています。
                    - 全ての文字が0x00から0x7Fの範囲に含まれること。
                    - 終端が'\0'であること。
                    - 大きさ(バイト数)が20であること。

                    この検査に適合しない場合、値は存在しないと見なされます。
    */
    const char* ExtractDateTime(size_t *pOutSize) const NN_NOEXCEPT;

    /**
        @brief      JPEGデータに付与されたメーカーノートを取得します。

        @param[out] pOutSize    メーカーノートの大きさ(バイト数)が格納されます。

        @return     メーカーノートを指すポインタが返ります。取得できない場合、nullptrが返ります。

        @pre        「Exifデータ解析済み状態」であることが必要です。

        @details    0th IFD Exifタグの「メーカーノート(0x927C)」を取得します。
                    撮影に使用されたソフトウェアにより、任意のバイナリデータがメーカーノートとして設定され得ます。
                    アプリケーションでメーカーノートを参照する場合、改変のリスクを考慮した検査を行ってください。
    */
    const void* ExtractMakerNote(size_t *pOutSize) const NN_NOEXCEPT;

    /**
        @brief      JPEGデータの実効的な大きさ(幅と高さ)を取得します。

        @param[out] pOutDimension   JPEGデータの実効的な大きさ(幅と高さ)が格納されます。

        @return     値が存在すればtrueを、そうでなければfalseを返します。

        @pre        「Exifデータ解析済み状態」であることが必要です。

        @details    0th IFD Exifタグの「実効画像幅(0xA002)」および「実効画像高さ(0xA003)」を取得します。
                    両方あるいはいずれかのタグが記録されていない場合、存在しないと見なされます。

                    一般的なJPEGデータを取り扱う場合、実効的な大きさは
                    nn::image::JpegDecoder::GetAnalyzedDimension() で取得できる値と異なる場合があります。
    */
    bool ExtractEffectiveDimension(Dimension *pOutDimension) const NN_NOEXCEPT;

    /**
        @brief      JPEGデータのユニークIDを文字列として取得します。

        @param[out] pOutSize    33が格納されます。

        @return     ユニークIDを指すポインタが返ります。取得できない場合、nullptrが返ります。

        @pre        「Exifデータ解析済み状態」であることが必要です。

        @details    0th IFD Exifタグの「ユニークID(0xA420)」を文字列として取得します。
                    取得できる文字列は次の観点で検査されています。
                    - 全ての文字が0x00から0x7Fの範囲に含まれること。
                    - 終端が'\0'であること。
                    - 大きさ(バイト数)が33であること。

                    この検査に適合しない場合、値は存在しないと見なされます。
    */
    const char* ExtractUniqueId(size_t *pOutSize) const NN_NOEXCEPT;

    /**
        @brief      JPEGデータに付与されたサムネイル画像を取得します。

        @param[in]  pOutSize    サムネイル画像の大きさ(バイト数)が格納されます。

        @return     サムネイル画像を指すポインタが返ります。取得できない場合、nullptrが返ります。

        @pre        「Exifデータ解析済み状態」であることが必要です。

        @details    1st IFD TIFFタグの「サムネイル画像のオフセット(0x0201)」および「サムネイル画像のサイズ(0x0202)」を取得します。
                    サムネイル画像がJPEGデータでない場合、サムネイル画像は存在しないと見なされます。
    */
    const void* ExtractThumbnail(size_t *pOutSize) const NN_NOEXCEPT;
    //! @}
    // ~解析したメタ情報の取得

private:
    enum Stage
    {
        Stage_Unregistered = 0,
        Stage_Registered = 1,
        Stage_Analyzed = 2
    };
    Stage m_Stage;

    void *m_WorkBuf;

    const void *m_ExifData;
    uint16_t m_ExifSize;
};

}}
