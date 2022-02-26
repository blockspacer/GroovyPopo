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

#include <stdint.h>
#include <cstdlib>

// nn_Allocator.h を include すると FontConverter に取り込んだ際の依存関係が複雑になるので、
// 必要なものだけを抜き出して定義する。
namespace nn {
    typedef void* (*AlignedAllocateFunctionWithUserData)(size_t size, size_t alignment, void* pUserData);
    typedef void(*FreeFunctionWithUserData)(void* ptr, void* pUserData);
};

/*!
  @namespace nn::fontll
  @brief スケーラブルフォントエンジンの名前空間です。
*/
namespace nn { namespace fontll {

//! @brief  16.16 固定小数点数(32ビット中、16ビットを小数部とする)です。
typedef int32_t Fixed32;

//---------------------------------------------------------------------------
// リザルトコード
//---------------------------------------------------------------------------

//! @brief  成功を表すリザルトコードです。
const int Success = 0;

//! @brief  フォントパスが不正です。
const int ErrorFontNotFound  = 301;

//! @brief  現在のフォントが読み込まれていません。
const int ErrorNoCurrentFontLoaded = 303;

//! @brief  メモリの確保に失敗しました。
const int ErrorMemoryAllocationFail  = 201;

//! @brief  指定されたフォントは正しい TrueType フォントではありません。
const int ErrorNotTtf = 403;

//! @brief  別のフォントが、同一名称で登録されています。
const int ErrorFontNameInUse = 314;

//! @brief  同じフォントが、別の名称で登録されています。
const int ErrorFontNameNotUnique = 311;

//! @brief  指定したスケール値が、制限値（+/- 16384）を超えています。
const int ErrorScaleLimit = 502;

//! @brief  指定したスケール値が、小さすぎます。
const int ErrorScaleDegenerate = 503;

//! @brief  指定したパーセント指定が範囲(0-100%)を超えています。
const int ErrorBadPercent = 613;

//---------------------------------------------------------------------------


//! @brief  内部定数です。
const int MappingLengthMax = 20;

//! @brief  フォント名の最大文字数です。
const int FontNameLengthMax = 50;

//! @brief  フォントファミリー名の最大文字数です。
const int FontFamilyNameLengthMax = 50;

//! @brief  コピーライト文字列の最大文字数です。
const int CopyrightLengthMax = 2048;

//! @brief  8ビットグレーマップフォーマットです。
const int FormatGrayMap8 = 0x0008;

//---------------------------------------------------------------------------
//! @brief  スケーラブルフォントエンジンの管理情報です。
//!
struct Handle
{
    //! @brief  内部メンバーです。
    uint8_t     pad[176];
};

//---------------------------------------------------------------------------
//! @brief  内部情報を管理する構造体です。
//!
struct CharacterMapping {
    //! @brief  内部メンバーです。
    uint16_t platform;
    //! @brief  内部メンバーです。
    uint16_t encoding;
    //! @brief  内部メンバーです。
    uint32_t offset;
};

//---------------------------------------------------------------------------
//! @brief  境界領域情報です。
//!
struct BoundingBox {
    //! @brief  Xの最小値です。
    int16_t xMin;
    //! @brief  Yの最小値です。
    int16_t yMin;
    //! @brief  Xの最大値です。
    int16_t xMax;
    //! @brief  Yの最大値です。
    int16_t yMax;
};


//---------------------------------------------------------------------------
//! @brief  グリフ情報です。
//!
struct GlyphMap{
    //! @brief  内部バッファへのポインタです。
    void*     pInternal;
    //! @brief  グリフ情報構造体のバイトサイズです。グリフビットマップのバイト配列を含むサイズです。
    int32_t size;
    //! @brief  グリフ原点(通常、ベースライン付近)を原点とした、左上位置のX値です。
    int16_t loX;
    //! @brief  グリフ原点(通常、ベースライン付近)を原点とした、左上位置のY値です。
    int16_t hiY;
    //! @brief  整数でのX方向の文字送り量です。
    int16_t idX;
    //! @brief  整数でのY方向の文字送り量です。
    int16_t idY;
    //! @brief  固定少数でのX方向の文字送り量です。
    Fixed32 dX;
    //! @brief  固定少数でのY方向の文字送り量です。
    Fixed32 dY;
    //! @brief  グリフビットマップのピクセル幅です。
    int16_t width;
    //! @brief  グリフビットマップのピクセル高さです。
    int16_t height;
    //! @brief  グリフビットマップの横1ラインのバイトサイズです。
    int16_t bpl;
    //! @brief  内部メンバーです。
    uint8_t embedded;
    //! @brief  ピクセルあたりのビット数です。
    int16_t bitsPerPixel;
    //! @brief  内部メンバーです。
    uint16_t type;
    //! @brief  グリフビットマップデータを格納するバイト配列です。
    uint8_t bits[1];
};

//---------------------------------------------------------------------------
//! @brief  フォント情報です。
//!
struct Metrics {

    //! @brief デザイン単位を emサイズ（通常アルファベット M の大きさ）で割った値です。
    uint16_t unitsPerEm;

    //! @brief head テーブルにおける MacStyle フラグ（表示形式に関するフラグ）です。
    uint16_t headMacStyle;

    //! @brief hhea テーブルにおける Windows形式のアセント（ベースラインからグリフ上端まで）量です。デザイン単位における値が格納されています。
    int16_t os2WinAscent;
    //! @brief hhea テーブルにおけるWindows形式のディセント（ベースラインからグリフ下端まで）量です。デザイン単位における値が格納されています。
    int16_t os2WinDescent;
    //! @brief os2 テーブルにおけるアセント（ベースラインからグリフ上端まで）量です。デザイン単位における値が格納されています。
    int16_t os2Ascent;
    //! @brief os2 テーブルにおけるディセント（ベースラインからグリフ下端まで）量です。デザイン単位における値が格納されています。
    int16_t os2Descent;
    //! @brief os2 テーブルにおけるフォントセレクションフラグ（表示形式に関するフラグ）です。
    uint16_t os2FsSelection;
    //! @brief os2 テーブルにおけるレディング（行と行の間隔）量です。デザイン単位における値が格納されています。
    uint16_t os2Leading;
    //! @brief hhea テーブルにおけるアセント（ベースラインからグリフ上端まで）量です。デザイン単位における値が格納されています。
    int16_t hheaAscent;
    //! @brief hhea テーブルにおけるディセント（ベースラインからグリフ下端まで）量です。デザイン単位における値が格納されています。
    int16_t hheaDescent;
    //! @brief hhea テーブルにおけるレディング（行と行の間隔）量です。デザイン単位における値が格納されています。
    uint16_t hheaLeading;
    //! @brief  予約されたメンバです。
    uint16_t embeddingBits;
    //! @brief  境界矩形情報です。デザイン単位における値が格納されています。
    BoundingBox fontBoundingBox;

    //! @brief  フォントの種類を表す内部メンバーです。
    uint16_t fontType;

    //! @brief  内部メンバーです。
    uint16_t cmapTableCount;
    //! @brief  内部メンバーです。
    CharacterMapping mappings[MappingLengthMax];
    //! @brief  TrueType collection 中に含まれるフォント数です。
    uint32_t fontInTtcCount;
    //! @brief  TrueType collection でのフォント番号です。
    uint32_t indexInTtc;

    //! @brief  フォント名です。
    char fontName[FontNameLengthMax];
    //! @brief  フォントファミリー名です。
    char fontFamilyName[FontFamilyNameLengthMax];
    //! @brief  著作権情報文字列です。
    char copyright[CopyrightLengthMax];
    //! @brief  フォント解像度です。デザイン単位で格納されている値に乗算してピクセルサイズに変換する場合に利用します。
    uint16_t metricsResolution;
    //! @brief  フォントが持つグリフ数です。
    uint32_t glyphCount;
    //! @brief  フォントが持つアイコン数です。
    uint32_t iconCount;
};

//---------------------------------------------------------------------------
//! @brief OTF 用のカーニングテーブルです。
//!
struct OtfKerningTable
{
    //! @brief  内部メンバーです。
    struct KerningPair
    {
        //! @brief  内部メンバーです。
        uint16_t first;
        //! @brief  内部メンバーです。
        uint16_t second;
        //! @brief  内部メンバーです。
        int16_t advance;
        //! @brief  内部メンバーです。
        int16_t placement;
    };
    //! @brief  内部メンバーです。
    int kerningPairCount;
    //! @brief  内部メンバーです。
    KerningPair* pKerningPairs;
    //! @brief  内部メンバーです。
    float reciprocalUnitsPerEm;
};

//---------------------------------------------------------------------------
//! @brief スケーラブルフォントエンジンを表すクラスです。
//!
//! @details
//---------------------------------------------------------------------------
class ScalableFontEngine
{
public:

    //! @brief 文字効果フラグです。
    enum Flags
    {
        //! @brief 効果無し
        Flags_NoEffect,
        //! @brief 縁付きフォント、塗りつぶし無し
        Flags_OutlinedUnFilled,
        //! @brief 縁付きフォント、塗りつぶし有り
        Flags_OutlinedFilled,
    };

    //----------------------------------------------------
    //! @name コンストラクタ／デストラクタ/初期化/終了処理
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 実質的な初期化はInitializeで行われます。ここではメンバ変数の初期化のみ行っています。<br>
    //! <br>
    //! 本クラスは複数スレッドからの利用を考慮した排他制御を内部で行いません。<br>
    //! 複数スレッドから同時に利用する必要がある場合、ユーザコード側で排他制御を行うかスレッド毎にインスタンスを作成するようにしてください。<br>
    //!
    ScalableFontEngine();

    //! @brief デストラクタです。
    //!
    ~ScalableFontEngine();

    //! @brief 初期化を行います。<br>
    //!
    //! @param[in] pHeap    ワークメモリへのポインタ
    //! @param[in] heapSize ワークメモリのサイズ
    //!
    //! @details
    //! ワークメモリは以下の用途に利用されます。<br>
    //! <br>
    //! ・読み込まれたフォントのリスト<br>
    //! ・読み込まれたフォント情報<br>
    //! ・スケール処理された、フォント情報とキャッシュ<br>
    //! ・グリフ（文字）ビットマップのキャッシュ<br>
    //! <br>
    //! メモリが不足した場合は、自動的に内部キャッシュを開放して柔軟にやりくりをします。<br>
    //! 完全にメモリが枯渇した場合には、201(nn::fontll::ErrorMemoryAllocationFail)のエラーコードが返ります。
    //! ワークメモリの適切サイズは利用状況（フォントの種類、同時利用文字数、フォントサイズ）によるので一概に記述しにくいです。<br>
    //! 64ピクセル程度のサイズなら 最低 100kバイト以下でも動作します。<br>
    //! パフォーマンスを実測しながら試行錯誤されることをお勧めします。<br>
    //!
    //! @return リザルトコードです。<br>
    //! 成功すると Success が返ります。
    //!
    int Initialize(void* pHeap, uint32_t heapSize);

    //! @brief 終了処理を行います。
    //!
    //! @details
    //! ワークメモリの解放は行いません。GetPointerToWorkBuffer で取得し別途解放してください。
    //!
    //! @return リザルトコードです。必ず Success が返ります。
    //!
    int Finalize();
    //@}

    //----------------------------------------------------
    //! @name フォント設定
    //@{

    //! @brief フォントを読み込みます。
    //!
    //! @param[in] pNameBuffer 内部管理用のnull終端されたフォント名文字列を出力するバッファです。
    //! @param[in] pMem     フォントファイルデータへのポインタ。
    //! @param[in] index    フォントの番号。ttc の場合、使用するフォントのインデックスを指定します。その他の場合は必ず 0 を指定します。
    //! @param[in] nameBufferLen    フォント名文字列を出力するバッファ（pNameBuffer）の長さです。最小で2、最大で FontNameLengthMax を指定します。
    //!
    //! @details 対応するスケーラブルフォントの種類は以下です。
    //!      - 本体内蔵フォント(bfttf)
    //!      - TrueType(ttf)
    //!      - TrueType Collection(ttc)
    //!      - OpenType(otf)
    //!
    //! <br>
    //! bfttf ファイルを読み込む場合は、@ref ScalableFontEngineHelper::Decode でデコードしてから渡すようにしてください。
    //!
    //! @return リザルトコードです。<br>
    //!  Success 以外では、以下のエラーコードが返ります。<br>
    //!  ErrorFontNotFound <br>
    //!  ErrorFontNameInUse <br>
    //!  ErrorFontNameNotUnique <br>
    //!
    int LoadFont(char* pNameBuffer, const void* pMem, uint32_t index, uint32_t nameBufferLen);

    //! @brief フォントを設定します。
    //!
    //! @param[in] name 内部管理用のフォント名
    //!
    //! @return リザルトコードです。<br>
    //! Success 以外では、以下のエラーコードが返ります。<br>
    //! ErrorFontNotFound <br>
    //! ErrorMemoryAllocationFail <br>
    //! ErrorNotTtf <br>
    //!
    int SetFont(char* name);

    //! @brief フォントスケールを設定します。
    //!
    //! @param[in] s00  2X2 変換行列の (0,0) 要素。固定小数点数で指定します。
    //! @param[in] s01  2X2 変換行列の (0,1) 要素。固定小数点数で指定します。
    //! @param[in] s10  2X2 変換行列の (1,0) 要素。固定小数点数で指定します。
    //! @param[in] s11  2X2 変換行列の (1,1) 要素。固定小数点数で指定します。
    //!
    //! @return リザルトコードです。<br>
    //! Success 以外では、以下のエラーコードが返ります。<br>
    //!   ErrorNoCurrentFontLoaded <br>
    //!   ErrorScaleLimit <br>
    //!   ErrorScaleDegenerate <br>
    //!
    int SetScale(Fixed32 s00, Fixed32 s01, Fixed32 s10, Fixed32 s11);

    //! @brief 文字のウエイト（太さ）を指定します。
    //!
    //! @param[in] weight   文字のウエイト（太さ）です。増加量を(0 - 100%)で指定します。
    //!
    //! @return リザルトコードです。<br>
    //!  Success 以外では、以下のエラーコードが返ります。<br>
    //! ErrorBadPercent
    //!
    int SetBoldWeight(Fixed32 weight);

    //! @brief 各種描画効果フラグを設定します
    //!
    //! @param[in] flag 描画効果フラグ  ScalableFontEngine::Flags です。
    //!
    //! @return リザルトコードです。
    //!
    int SetFlags(Flags flag);

    //! @brief 縁取り機能における縁の幅を設定します
    //!
    //! @param[in] width 縁の幅です。ピクセル値で指定します。
    //!
    //! @return リザルトコードです。成功すると  Success が返ります。
    //!
    int SetOutlineWidth(uint16_t width);
    //@}

    //----------------------------------------------------
    //! @name グリフの取得
    //@{

    //! @brief グリフ情報を取得します。
    //!
    //! @param[in] id グリフ情報を取得する文字コードです。
    //! @param[in] type フォーマット種類です。 nn::fontll::MAP_GRAYMAP8 だけが指定できます。
    //!
    //! @return グリフ情報へのポインタです。エラーが起こった場合は NULL が返ります。
    //!
    GlyphMap* AcquireGlyphmap(uint32_t id, uint16_t type);

    //! @brief グリフ情報の有無を調査します。
    //!
    //! @param[in] id グリフ情報の有無を調査する文字コードです。
    //!
    //! @return グリフ情報が存在すれば true を返します。
    //!
    bool CheckGlyphExist(uint32_t id);

    //! @brief グリフ情報をエンジン内部から解放します。
    //!
    //! @param[in] pGlyph グリフ情報へのポインタです。
    //!
    //! @return リザルトコードです。成功すると  Success が返ります。
    //!
    int ReleasesGlyph(void* pGlyph);

    //! @brief ヒンティングを強制するかどうかを設定します。
    //!
    //! 通常は常に false に設定されています。
    //! 特定のフォントで描画が崩れたりクラッシュする場合、
    //! AcquireGlyphmap メソッドの直前で true に設定すると直ります。
    //! ただし true にすることで、描画結果がピクセル単位で若干変わります。
    //! 本来フォントが持つヒント情報に基づいて
    //! フォント描画の計算していますが、そのときの演算の丸め誤差が原因して
    //! 特定のフォントでクラッシュすることが確認されています。
    //! このメソッドに true を渡すことで
    //! ヒント情報をランタイム側で計算するようになり、
    //! これらのクラッシュを回避することができます。
    //!
    //! @param[in] isAutoHint ヒンティングを強制するかどうかです。
    //!
    void SetAutoHint(bool isAutoHint);

    //@}

    //----------------------------------------------------
    //! @name 情報の取得
    //@{

    //! @brief 内部ワークメモリのポインタを取得します。
    //!
    //! @return 内部ワークメモリのポインタです。
    //!
    void* GetPointerToWorkBuffer();

    //! @brief フォント情報を取得します。
    //!
    //! @param[out] pM フォント情報へのポインタです。
    //!
    //! @return リザルトコードです。成功すると  Success が返ります。
    //!
    int GetFontMetrics(Metrics* pM);

    //! @brief 次の文字への文字送り量を取得します。
    //!
    //! @param[out] idx 整数での文字送り量のX値です。
    //! @param[out] idy 整数での文字送り量のY値です。
    //! @param[out] dx 固定小数点数での文字送り量のX値です。
    //! @param[out] dy 固定小数点数での文字送り量のY値です。
    //! @param[in] id 情報を取得する文字コードです。
    //! @param[in] type グリフの種類です。必ず  nn::fontll::MAP_GRAYMAP8 を指定します。
    //!
    //! @return リザルトコードです。成功すると  Success が返ります。
    //!
    int GetAdvance(int16_t* idx, int16_t* idy, Fixed32* dx, Fixed32* dy, uint32_t id, uint16_t type);

    //! @brief 二つの文字間のカーニング情報を取得します。
    //!
    //! @param[out] dx 2文字間のカーニング情報の X 値です。
    //! @param[out] dy 2文字間のカーニング情報の Y 値です。
    //! @param[in] id1 1文字目の文字コードです。
    //! @param[in] id2 2文字目の文字コードです。
    //!
    //! @return リザルトコードです。成功すると  Success が返ります。
    //!
    int GetKerning(Fixed32* dx, Fixed32* dy, uint32_t id1, uint32_t id2);

    //! @brief TTF 用のカーニングテーブルが存在するか確認します。
    //!
    //! @details
    //! フォント内部のデーター構造を走査する重い処理なので、頻繁に呼び出さないようにしてください。
    //!
    //! @return TTF 用のカーニングテーブルが存在すれば true を返します。
    //!
    bool CheckTtfKernTableExist();

    //! @brief OTF 用のカーニングテーブルを取得します。
    //!
    //! @param[in] allocateFunction メモリを確保するアロケータです。
    //! @param[in] pUserDataForAllocateFunction メモリを確保するアロケータに渡すユーザーデータです。
    //!
    //! @return OTF 用のカーニングテーブルです。
    //!
    OtfKerningTable* InitializeOtfKerningTable(nn::AlignedAllocateFunctionWithUserData allocateFunction, void* pUserDataForAllocateFunction);

    //! @brief OTF 用のカーニングテーブルを取得します。
    //!
    //! @param[in] allocateFunction メモリを確保するアロケータです。
    //! @param[in] pUserDataForAllocateFunction メモリを確保するアロケータに渡すユーザーデータです。
    //! @param[in] ignorePalt Palt 情報を読み込まない場合は true を指定します。
    //!
    //! @return OTF 用のカーニングテーブルです。
    //!
    OtfKerningTable* InitializeOtfKerningTable(nn::AlignedAllocateFunctionWithUserData allocateFunction, void* pUserDataForAllocateFunction, bool ignorePalt);

    //! @brief OTF 用のカーニングテーブルを解放します。
    //!
    //! @param[in] pOtfKerningTable OTF 用のカーニングです。
    //! @param[in] freeFunction メモリを解放するアロケータです。
    //! @param[in] pUserDataForFreeFunction メモリを解放するアロケータに渡すユーザーデータです。
    //!
    void FinalizeOtfKerningTable(OtfKerningTable* pOtfKerningTable, nn::FreeFunctionWithUserData freeFunction, void* pUserDataForFreeFunction);

    //! @brief OTF 用のカーニングテーブルから二つの文字間のカーニング情報を取得します。
    //!
    //! @details
    //! OTF形式のフォントはGetKerningでカーニングが取得できませんので、
    //! こちらの関数をお使いください。
    //!
    //! @param[in] pOtfKerningTable OTF 用のカーニングテーブルです。
    //! @param[in] id1 1文字目の文字コードです。
    //! @param[in] id2 2文字目の文字コードです。
    //! @param[in] fontSize フォントサイズです。
    //!
    //! @return フォントサイズによって調整されたカーニングの値です。
    //!
    int AcquireOtfKerning(const OtfKerningTable* pOtfKerningTable, uint32_t id1, uint32_t id2, uint32_t fontSize);

    //! @brief OTF 用のカーニングテーブルから先頭の文字のカーニング情報を取得します。
    //!
    //! @param[in] pOtfKerningTable OTF 用のカーニングテーブルです。
    //! @param[in] id 文字コードです。
    //! @param[in] fontSize フォントサイズです。
    //!
    //! @return フォントサイズによって調整されたカーニングの値です。
    //!
    int AcquireOtfKerningFirst(const OtfKerningTable* pOtfKerningTable, uint32_t id, uint32_t fontSize);

    //! @brief OTF 用のカーニングテーブルから二つの文字間のカーニング情報を取得します。
    //!
    //! @details
    //! OTF形式のフォントはGetKerningでカーニングが取得できませんので、
    //! こちらの関数をお使いください。
    //!
    //! @param[in] pOtfKerningTable OTF 用のカーニングテーブルです。
    //! @param[in] id 文字コードです。
    //! @param[in] fontSize フォントサイズです。
    //!
    //! @return フォントサイズによって調整されたカーニングの値です。
    //!
    int AcquireOtfKerningLast(const OtfKerningTable* pOtfKerningTable, uint32_t id, uint32_t fontSize);

    //! @brief OTF 用のカーニングから二つの文字間のカーニング情報を生情報として取得します。
    //!
    //! @param[in] pOtfKerningTable OTF 用のカーニングテーブルです。
    //! @param[in] id1 1文字目の文字コードです。
    //! @param[in] id2 2文字目の文字コードです。
    //!
    //! @return カーニングの値です。
    //!
    int AcquireRawOtfKerning(const OtfKerningTable* pOtfKerningTable, uint32_t id1, uint32_t id2);

    //! @brief OTF 用のカーニングから先頭の文字のカーニング情報を生情報として取得します。
    //!
    //! @param[in] pOtfKerningTable OTF 用のカーニングテーブルです。
    //! @param[in] id 文字コードです。
    //!
    //! @return カーニングの値です。
    //!
    int AcquireRawOtfKerningFirst(const OtfKerningTable* pOtfKerningTable, uint32_t id);

    //! @brief OTF 用のカーニングから末尾の文字のカーニング情報を生情報として取得します。
    //!
    //! @param[in] pOtfKerningTable OTF 用のカーニングテーブルです。
    //! @param[in] id 文字コードです。
    //!
    //! @return カーニングの値です。
    //!
    int AcquireRawOtfKerningLast(const OtfKerningTable* pOtfKerningTable, uint32_t id);

    //! @brief エラーコードを取得します。
    //!
    //! @return エラーコードです。
    //!
    int GetError();
    //@}

private:
    //! @brief GPOSテーブルからカーニングを抜き出します。
    //!
    //! @param[in] pOtfKerningTable 抜き出したカーニングを格納するカーニングテーブルです。NULL を指定すると格納しません。
    //! @param[in] pGposTable GPOSテーブルです。
    //! @param[in] ignorePalt Palt 情報を読み込まない場合は true を指定します。
    //!
    //! @return カーニングテーブルの要素数です。
    //!
    int PickKerningFromGposTable(OtfKerningTable* pOtfKerningTable, const void* pGposTable, bool ignorePalt);

    bool BinarySearchForOtfKerning(int* pAdvance, int* pPlacement, const OtfKerningTable* pOtfKerningTable, uint32_t c1, uint32_t c2);

    Handle m_Handle;
    void*  m_pInternalBuffer;
};

//---------------------------------------------------------------------------
//! @brief スケーラブルフォントエンジンのヘルパークラスです。
//!
//---------------------------------------------------------------------------
class ScalableFontEngineHelper
{
public:
    //! @brief bfttf 形式のフォントファイルデータを復号して、利用できるようにします。
    //!
    //! @param[in] pData 復号前のデータへのポインタです。
    //! @param[in] dataSize 復号前のデータのサイズです。
    //!
    //! @return    復号されたフォントファイルデータです。
    //!
    static void* Decode(const void* pData, uint32_t dataSize);
};

} } // namespace nn::fontll
