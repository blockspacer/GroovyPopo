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

#include <nn/nn_Allocator.h>

#include <nn/gfx/gfx_Variation-api.gl.h>

#include <nn/util/util_IntrusiveList.h>
#include <nn/font/font_Font.h>
#include <nn/font/font_GlyphTreeMap.h>

// #define NN_FONT_USE_FONTLL2

namespace nn
{
#if defined(NN_FONT_USE_FONTLL2)
namespace fontll2
{
    class ScalableFontEngine;
    struct OtfKerningTable;
}
#else
namespace fontll
{
    class ScalableFontEngine;
    struct OtfKerningTable;
} // namespace nn::fontll
#endif
} // namespace nn

namespace nn
{
namespace font
{

//---------------------------------------------------------------------------
//! @brief スケーラブルフォントのテクスチャキャッシュを表すクラスです。
//!
//! @details
//! 複数のスケラーブルフォントクラスが一つのテクスチャキャッシュを
//! 利用することができます。その場合、複数のサイズのフォントが一つの
//! テクスチャキャッシュに混在することになります。
//---------------------------------------------------------------------------
class TextureCache
{
public:
    //! @brief フォントデータの形式を表す列挙型です。
    //!
    enum FontDataType
    {
        FontDataType_Ttf,                         //!< ttf そのままのデータ、もしくはデコード済みの bfttf 形式のデータです。
        FontDataType_Otf,                         //!< otf そのままのデータ、もしくはデコード済みの bfotf 形式のデータです。
        FontDataType_Bfttf,                       //!< デコード前の bfttf 形式のデータです。
        FontDataType_Bfotf                        //!< デコード前の bfotf 形式のデータです。
    };

    //! @brief ラインの種類を表す列挙型です。
    //!
    //! @details
    //! nn::font::TextureCache::InitializeArg::pCalculateLineHeightFunction に
    //! 関数を登録しなかった際のデフォルトのラインの種類です。
    //! 自前で関数を登録した場合はラインの種類もそれに従います。
    //!
    enum LineHeight
    {
        LineHeight_16 = 4,     //!< 高さが16のラインです。
        LineHeight_32 = 5,     //!< 高さが32のラインです。
        LineHeight_64 = 6,     //!< 高さが64のラインです。
        LineHeight_128 = 7,    //!< 高さが128のラインです。
        LineHeight_256 = 8,    //!< 高さが256のラインです。
        LineHeight_512 = 9,    //!< 高さが512のラインです。
        LineHeight_1024 = 10,  //!< 高さが1024のラインです。
    };

    //! @brief テクスチャキャッシュやグリフが枯渇した際のエラー情報です。
    enum NoSpaceError
    {
        NoSpaceError_NoError, //!< エラーは発生していません。
        NoSpaceError_LackOfGlyphNodeSpace, //!< グリフノードに空きがありません。
        NoSpaceError_TextureCacheLineIsOverMax, //!< テクスチャキャッシュのラインの数が上限を超えました。
        NoSpaceError_LackOfTextureCacheSpace, //!< テクスチャキャッシュに空きがありません。
    };

    //! @brief FontFace の最大の個数です。
    static const int FontFaceCountMax = 32;

    //! @brief 1 つの FontFace に格納できるマルチスケーラブルフォントの最大個数です。
    static const int MultiScalableFontCountMax = 16;

    //! @brief 1 つのスケーラブルフォントに設定できる使用文字コード範囲のセットの最大個数です。
    static const int CharCodeRangeCountMax = 16;

    //----------------------------------------------------
    //! @name コンストラクタ／デストラクタ/初期化/終了処理
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 実質的な初期化はInitializeで行われます。ここでは、
    //! メンバ変数の初期化のみ行っています。
    //!
    TextureCache();

    //! @brief デストラクタです。
    //!
    //! @details
    //! このクラスでは何も行いません。メモリやWindowsでのGLのオブジェクトの
    //! 破棄を行うにはFinalizeを呼び出す必要がありますので、ご注意ください。
    //!
    virtual ~TextureCache();

    //! @brief 初期化のパラメータを指定するための構造体です。
    //!
    //! @details
    //! この構造体のサイズは大きいため、スタック上ではなくヒープ上に構築することを推奨します。
    //!
    struct InitializeArg {

        //! テクスチャキャッシュの横幅です。必ず2の累乗としてください。
        //! デフォルトでは1024です。
        uint32_t textureCacheWidth;
        //! テクスチャキャッシュの縦幅です。
        //! デフォルトでは1024です。
        uint32_t textureCacheHeight;
        //! メモリを確保するためのアロケータです。メモリの確保はInitializeの中のみで行われます。
        //! 確保したメモリはFinalizeで解放されます。
        //! デフォルト値はありません。必ず指定してください。
        nn::AlignedAllocateFunctionWithUserData allocateFunction;
        //! メモリを確保するためのアロケータに引数として渡されるユーザーデータです。
        void* pUserDataForAllocateFunction;
        //! フォントデータへのポインタの配列を渡します。(ポインタが示す先はttf(otf)ファイルもしくはbfttfファイルのバイナリデータです。)
        //! 要素数はfontCountに等しくなるようにしてください。
        //! デフォルト値はありません。必ず指定してください。
        const void* pFontDatas[FontFaceCountMax][MultiScalableFontCountMax];

        //! フォントデータのサイズの配列を渡します。
        //! 要素数はfontCountに等しくなるようにしてください。
        //! このメンバは、フォントの形式がFontDataType_BfttfかFontDataType_Bfotfのときのみ設定する必要があります。
        size_t pFontDataSizes[FontFaceCountMax][MultiScalableFontCountMax];
        //! ボールドエフェクトの重み値です。
        //! 0.0 ～ 1.0 の間で指定します。
        //! デフォルト値は 0.0 になっています。
        float pBoldWeights[FontFaceCountMax][MultiScalableFontCountMax];
        //! 縁取りの幅です。
        //! 0 ～ 10 の間で指定します。
        //! デフォルト値は 0 になっています。
        uint8_t pBorderWidths[FontFaceCountMax][MultiScalableFontCountMax];
        //! スケールの幅です。
        //! デフォルト値は 1.0f になっています。
        float scaleWidths[FontFaceCountMax][MultiScalableFontCountMax];
        //! スケールの高さです。
        //! デフォルト値は 1.0f になっています。
        float scaleHeights[FontFaceCountMax][MultiScalableFontCountMax];
        //! フォントデータの形式を指定します。
        //! デフォルト値は FontDataType_Ttf になっています。
        FontDataType pFontDataTypes[FontFaceCountMax][MultiScalableFontCountMax];
        //! OTF の GPOS テーブルに含まれる Palt 情報(全角文字をプロポーショナルにする機能)を無視するか否かです。
        //! true を指定すると Palt 情報が無視されます。
        //! デフォルトでは false です。
        bool ignorePalt[FontFaceCountMax][MultiScalableFontCountMax];
        //! true にすると、グリフの左右の余白を削除します。
        //! グリフの左右が開きすぎているフォントを詰めたいときにお使いください。
        //! またカーニング情報も無視されるようになります。
        //! デフォルトでは false です。
        bool deleteBearingX[FontFaceCountMax][MultiScalableFontCountMax];
        //! 文字の左右の余白にオフセットを適用します。
        //! デフォルトでは 0 です。
        int16_t bearingOffsetX[FontFaceCountMax][MultiScalableFontCountMax];
        //! true にすると、強制的に等幅フォントのように描画します。
        //! デフォルトでは false です。
        bool forceMonospacedEnabled[FontFaceCountMax][MultiScalableFontCountMax];
        //! forceMonospacedEnabled が true のときの文字幅を指定します。
        //! デフォルトでは 0 です。
        int16_t forceMonospacedWidth[FontFaceCountMax][MultiScalableFontCountMax];
        //! ベースラインを調整するオフセット値です。
        //! デフォルトでは 0 です。
        int baselineOffset[FontFaceCountMax][MultiScalableFontCountMax];
        //! 0 以外を指定すると、フォントメトリクスのアセントをその値で強制的に上書きします。
        //! デフォルトでは 0 です。
        uint16_t overwrittenMetricsAscent[FontFaceCountMax][MultiScalableFontCountMax];
        //! 0 以外を指定すると、フォントメトリクスのディセントをその値で強制的に上書きします。
        //! デフォルトでは 0 です。
        uint16_t overwrittenMetricsDescent[FontFaceCountMax][MultiScalableFontCountMax];
        //! 使用文字コード範囲の数を指定します。
        //! 0を指定するとすべての文字コードを使用します。
        //! デフォルトでは0です。
        int charCodeRangeCount[FontFaceCountMax][MultiScalableFontCountMax];
        //! 使用文字コード範囲の先頭を指定します。
        //! デフォルトでは0です。
        uint32_t charCodeRangeFirst[FontFaceCountMax][MultiScalableFontCountMax][CharCodeRangeCountMax];
        //! 使用文字コード範囲の終端を指定します。
        //! デフォルトでは0です。
        uint32_t charCodeRangeLast[FontFaceCountMax][MultiScalableFontCountMax][CharCodeRangeCountMax];
        //! 扱うフォントの数です。1～32の数を渡してください。
        //! デフォルトでは1です。
        uint32_t fontCount;
        //! 扱うフォントリストの数です。
        //! デフォルトでは1です。
        uint32_t fontListCount[FontFaceCountMax];
        //! スケーラブルフォントエンジンに渡すワークメモリのサイズです。大きいほどエンジン内で文字を
        //! キャッシュしてくれます。
        //! デフォルトでは512KBです。
        size_t workMemorySize;
        //! プロットと並行して処理ができるように、プロット用とは別に用意しているスケーラブルフォントエンジン
        //! のインスタンスのためのワークメモリのサイズです。通常はデフォルトのサイズで問題ありませんが、
        //! フォントによってはサイズが足りなくなる場合がありますので、その場合は増やしてください。
        //! サイズが足りなくなった場合、アサートに失敗し、"error: 201"というメッセージが表示されます。
        //! デフォルトでは50KBです。
        size_t noPlotWorkMemorySize;
        //! テクスチャキャッシュ内でグリフを管理するためのノードの最大数です。
        //! 管理するグリフの数が多いほど(テクスチャキャッシュが大きいほど)大きい数を指定する必要があります。
        //! デフォルトでは1024個です。
        uint32_t glyphNodeCountMax;
        //! ttc ファイルを使用する際に、レンダリングに使用するフォントのインデックスを指定します。
        //! その他の形式のフォントファイルを使用する場合は 0 を指定します。
        //! デフォルトは 0 です。
        uint32_t fontIndex[FontFaceCountMax][MultiScalableFontCountMax];
        //! このテクスチャキャッシュを使用して、マルチコアのdrawを行うか否かです。
        //! マルチコアのdrawを行う場合は、並列処理のためにスケーラブルフォントエンジンのインスタンスをコア数
        //! ぶん持たなければいけないため、消費メモリが増加します。デフォルトではfalseです。
        bool isDrawMultiCore;
        //! マルチコアのdrawを行う場合に、動作するプラットフォームのコア数を指定します。
        //! pGetCoreNumberFunction() < coreCount となるようにしてください。
        //! 主に実機でマルチコア処理を行うための機能ですので、
        //! PC 上で実行させるときには実機のコア数以下で処理が行われるようにするか、
        //! マルチコア機能をオフにしてください。
        //! デフォルトでは 1 です。
        int coreCount;
        //! 現在のスレッドが動作するコア ID を返す関数を登録します。
        //! NULL を指定すると nn::os::GetCurrentCoreNumber が登録されます。
        //! デフォルトは NULL です。
        int(*pGetCoreNumberFunction)();
        //! 文字の高さからラインの種類に変換する関数を登録します。
        //! デフォルトではテクスチャキャッシュのラインの高さは、文字の高さを 2 の累乗に切り上げたものが
        //! 使用されますが、本メンバを設定することで、ラインの高さを自由にカスタマイズすることができます。
        //! テクスチャキャッシュの使われ方がより効率的になる場合に設定してください。
        //! NULL を指定すると nn::font::GlyphNode::CalculateLineKind が登録されます。
        //! デフォルトは NULL です。
        uint8_t(*pCalculateLineKindFunction)(uint16_t height);
        //! ラインの種類からラインの高さに変換する関数を登録します。
        //! pCalculateLineKindFunction を設定した場合には本メンバも設定してください。
        //! NULL を指定すると nn::font::GlyphNode::CalculateLineHeight が登録されます。
        //! デフォルトは NULL です。
        uint32_t(*pCalculateLineHeightFunction)(uint8_t kind);
        //! ヒンティングを強制するか否かです。
        //! 通常は常に false に設定されています。
        //! 特定のフォントで描画が崩れたりクラッシュする場合、
        //! true に設定すると直ります。
        //! ただし true にすることで、描画結果がピクセル単位で若干変わります。
        //! 本来フォントが持つヒント情報に基づいて
        //! フォント描画の計算していますが、そのときの演算の丸め誤差が原因して
        //! 特定のフォントでクラッシュすることが確認されています。
        //! true に設定することでヒント情報をランタイム側で計算するようになり、
        //! これらのクラッシュを回避することができます。デフォルトではfalseです。
        bool isAutoHint;
        //! nn::font::ScalableFont::GetCharWidth や nn::font::ScalableFont::GetCharWidths で取得した文字幅は
        //! プロット完了前は疑似的に計算しているため、実際にレンダリングしたときの幅と比べて 1px 程度の誤差が生じることがあります。
        //! 本パラメータに true を指定すると、プロット完了後はレンダリング結果に対して正確な幅を返すようになります。
        //! false を指定すると、プロット完了後も完了前と同じ幅の値を返します。
        //! 文字幅をもとに折り返し処理をしているときなど、プロット前後で同じ幅を返してほしい場合には false を指定してください。
        //! デフォルトでは true です。
        bool isExactWidthReturnedAfterPlot;

        //! @brief デフォルト値を設定します。
        void SetDefault();
    };

    //! @brief 内部テクスチャ用のメモリプールインフォを設定します。
    //!
    //! @param[in] pInfo      メモリプールインフォです。
    //!
    static void SetMemoryPoolInfo(nn::gfx::MemoryPoolInfo* pInfo);

    //! @brief 内部テクスチャ用のメモリプールアライメントを計算します。
    //!
    //! @param[in] pDevice      デバイスです。
    //! @param[in] width        テクスチャの幅です。
    //! @param[in] height       テクスチャの高さです。
    //! @return 内部テクスチャ用のメモリプールアライメントが返ります。
    //!
    static size_t CalculateMemoryPoolAlignment(nn::gfx::Device* pDevice, int width, int height);

    //! @brief 内部テクスチャ用のメモリプールサイズを計算します。
    //!
    //! @param[in] pDevice      デバイスです。
    //! @param[in] width        テクスチャの幅です。
    //! @param[in] height       テクスチャの高さです。
    //! @return 内部テクスチャ用のメモリプールサイズが返ります。
    //!
    static size_t CalculateMemoryPoolSize(nn::gfx::Device* pDevice, int width, int height);

    //! @brief 初期化を行います。
    //!
    //! @details
    //! テクスチャの初期化なども行いますので、描画コマンドを発行可能な場所で呼んでください。
    //! また、このメソッドを呼んでからフォントでGetGlyphを利用して描画を行うまでに、
    //! 一回はUpdateTextureCache及びCompleteTextureCacheを行うようにしてください。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] arg  パラメータを指定する構造体
    //!
    void Initialize(nn::gfx::Device* pDevice, const InitializeArg& arg)
    {
        Initialize(pDevice, arg, NULL, 0, 0);
    }

    //! @brief 初期化を行います。
    //!
    //! @details
    //! テクスチャの初期化なども行いますので、描画コマンドを発行可能な場所で呼んでください。
    //! また、このメソッドを呼んでからフォントでGetGlyphを利用して描画を行うまでに、
    //! 一回はUpdateTextureCache及びCompleteTextureCacheを行うようにしてください。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] arg  パラメータを指定する構造体
    //! @param[in] pMemoryPool メモリプールです。内部テクスチャを配置するメモリプールを指定します。nullptr を指定した場合、内部で作成します。
    //! @param[in] memoryPoolOffset メモリプールの先頭から内部テクスチャ位置へのオフセットです。
    //! @param[in] memoryPoolSize メモリプールのサイズです。
    //!
    void Initialize(nn::gfx::Device* pDevice, const InitializeArg& arg, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize);

    //! @brief 終了処理を行います。
    //!
    //! @details
    //! Initializeで確保したメモリを開放します。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] freeFunction    メモリを解放するアロケータ
    //! @param[in] pUserDataForFreeFunction メモリを解放するアロケータに渡すユーザーデータです。
    //!
    void Finalize(nn::gfx::Device* pDevice, nn::FreeFunctionWithUserData freeFunction, void* pUserDataForFreeFunction);

    //! @brief テクスチャービューをディスクリプタプールに登録します。
    //!
    //! @param[in] registerTextureViewSlot     ディスクリプタスロット確保関数です。
    //! @param[in] pUserData     関数に渡されるユーザーデーターです。
    //!
    void RegisterTextureViewToDescriptorPool(nn::font::RegisterTextureViewSlot registerTextureViewSlot, void* pUserData);

    //! @brief テクスチャービューをディスクリプタプールから解放します。
    //!
    //! @param[in] unregisterTextureViewSlot     ディスクリプタスロット解放関数です。
    //! @param[in] pUserData     関数に渡されるユーザーデーターです。
    //!
    void UnregisterTextureViewFromDescriptorPool(nn::font::UnregisterTextureViewSlot unregisterTextureViewSlot, void* pUserData);
    //@}

    //----------------------------------------------------
    //! @name テクスチャキャッシュの操作
    //@{

    //! @brief 指定した文字を、描画する文字として登録します。
    //!
    //! @details
    //! 描画する文字として登録した文字は、テクスチャキャッシュになかった場合は、新たに
    //! プロットする必要がある文字として登録されます。この文字は、UpdateTextureCacheと
    //! CompleteTextureCacheを行った段階で使えるようになります。
    //!
    //! 描画する文字として登録した文字が、テクスチャキャッシュに既にあった場合は、
    //! プロット等は行われず、引き続き使用できます。また、その文字は必要な文字として
    //! 登録されますので、削除対象になりません。
    //!
    //! 文字をロックする際のグループは、0から31までの32個あります。
    //! ロックはグループ毎に行われますので、例えばグル―プ0で登録された文字がグループ1
    //! でもう一度登録された場合、その両方でロックされたことになり、両方についてロック
    //! がクリアされないと削除対象になりません。
    //!
    //! @param[in] code         描画する文字として登録する文字
    //! @param[in] fontSize     フォントサイズ
    //! @param[in] fontFace     フォント字形を指定する数値
    //! @param[in] lockGroup    文字をロックする際のグループ。0から31までの数値で指定してください。-1を指定した場合はロックしません。
    //! @return 登録した文字のプロットが必要な場合にtrueが返されます。つまり、falseが返った場合はその文字は
    //!         UpdateTextureCacheを行わなくても描画できます。
    //!
    bool RegisterGlyph(uint32_t code, uint32_t fontSize, uint16_t fontFace, int lockGroup)
    {
        return RegisterGlyph(code, fontSize, fontFace, lockGroup, false);
    }

    //! @brief 指定した文字を、描画する文字として登録します。
    //!
    //! @details
    //! 描画する文字として登録した文字は、テクスチャキャッシュになかった場合は、新たに
    //! プロットする必要がある文字として登録されます。この文字は、UpdateTextureCacheと
    //! CompleteTextureCacheを行った段階で使えるようになります。
    //!
    //! 描画する文字として登録した文字が、テクスチャキャッシュに既にあった場合は、
    //! プロット等は行われず、引き続き使用できます。また、その文字は必要な文字として
    //! 登録されますので、削除対象になりません。
    //!
    //! 文字をロックする際のグループは、0から31までの32個あります。
    //! ロックはグループ毎に行われますので、例えばグル―プ0で登録された文字がグループ1
    //! でもう一度登録された場合、その両方でロックされたことになり、両方についてロック
    //! がクリアされないと削除対象になりません。
    //!
    //! @param[in] code         描画する文字として登録する文字
    //! @param[in] fontSize     フォントサイズ
    //! @param[in] fontFace     フォント字形を指定する数値
    //! @param[in] lockGroup    文字をロックする際のグループ。0から31までの数値で指定してください。-1を指定した場合はロックしません。
    //! @param[in] isDrawingHyphenMinusInsteadOfNonBreakingHyphen NON-BREAKING HYPHEN(U+2011) の文字を描画する際に、HYPHEN-MINUS(U+002D) のグリフを使うかどうかです。
    //! @return 登録した文字のプロットが必要な場合にtrueが返されます。つまり、falseが返った場合はその文字は
    //!         UpdateTextureCacheを行わなくても描画できます。
    //!
    bool RegisterGlyph(uint32_t code, uint32_t fontSize, uint16_t fontFace, int lockGroup, bool isDrawingHyphenMinusInsteadOfNonBreakingHyphen);

    //! @brief 指定した文字列に含まれる指定した文字数だけの文字を、描画する文字として登録します。UTF-16版。
    //!
    //! @details
    //! 登録する文字を文字列として指定できる以外は @ref RegisterGlyph と同じです。
    //!
    //! @param[in] pCode        描画する文字として登録する文字列
    //! @param[in] codeLength   文字列の文字数。この文字数より短い文字数でNULL終端が来た場合は、登録はそこで終了します。
    //! @param[in] fontSize     フォントサイズ
    //! @param[in] fontFace     フォント字形を指定する数値
    //! @param[in] lockGroup    文字をロックする際のグループ。0から31までの数値で指定してください。-1を指定した場合はロックしません。
    //! @return プロットが必要な文字が何文字あったかを返します。つまり0が返された場合は、
    //!         全ての文字がUpdateTextureCacheを行わなくても描画できることを示します。
    //!
    uint32_t RegisterGlyphsWithLength(const uint16_t* pCode, uint32_t codeLength, uint32_t fontSize, uint16_t fontFace, int lockGroup)
    {
        return RegisterGlyphsWithLength(pCode, codeLength, fontSize, fontFace, lockGroup, false);
    }

    //! @brief 指定した文字列に含まれる指定した文字数だけの文字を、描画する文字として登録します。UTF-16版。
    //!
    //! @details
    //! 登録する文字を文字列として指定できる以外は @ref RegisterGlyph と同じです。
    //!
    //! @param[in] pCode        描画する文字として登録する文字列
    //! @param[in] codeLength   文字列の文字数。この文字数より短い文字数でNULL終端が来た場合は、登録はそこで終了します。
    //! @param[in] fontSize     フォントサイズ
    //! @param[in] fontFace     フォント字形を指定する数値
    //! @param[in] lockGroup    文字をロックする際のグループ。0から31までの数値で指定してください。-1を指定した場合はロックしません。
    //! @param[in] isDrawingHyphenMinusInsteadOfNonBreakingHyphen NON-BREAKING HYPHEN(U+2011) の文字を描画する際に、HYPHEN-MINUS(U+002D) のグリフを使うかどうかです。
    //! @return プロットが必要な文字が何文字あったかを返します。つまり0が返された場合は、
    //!         全ての文字がUpdateTextureCacheを行わなくても描画できることを示します。
    //!
    uint32_t RegisterGlyphsWithLength(const uint16_t* pCode, uint32_t codeLength, uint32_t fontSize, uint16_t fontFace, int lockGroup, bool isDrawingHyphenMinusInsteadOfNonBreakingHyphen);

    //! @brief 指定した文字列に含まれる指定した文字数だけの文字を、描画する文字として登録します。UTF-8版。
    //!
    //! @details
    //! 登録する文字を文字列として指定できる以外は @ref RegisterGlyph と同じです。
    //!
    //! @param[in] pCode        描画する文字として登録する文字列
    //! @param[in] codeLength   文字列のバイト数。この文字数より短い文字数でNULL終端が来た場合は、登録はそこで終了します。
    //! @param[in] fontSize     フォントサイズ
    //! @param[in] fontFace     フォント字形を指定する数値
    //! @param[in] lockGroup    文字をロックする際のグループ。0から31までの数値で指定してください。-1を指定した場合はロックしません。
    //! @return プロットが必要な文字が何文字あったかを返します。つまり0が返された場合は、
    //!         全ての文字がUpdateTextureCacheを行わなくても描画できることを示します。
    //!
    uint32_t RegisterGlyphsWithLengthUtf8(const char* pCode, uint32_t codeLength, uint32_t fontSize, uint16_t fontFace, int lockGroup)
    {
        return RegisterGlyphsWithLengthUtf8(pCode, codeLength, fontSize, fontFace, lockGroup, false);
    }

    //! @brief 指定した文字列に含まれる指定した文字数だけの文字を、描画する文字として登録します。UTF-8版。
    //!
    //! @details
    //! 登録する文字を文字列として指定できる以外は @ref RegisterGlyph と同じです。
    //!
    //! @param[in] pCode        描画する文字として登録する文字列
    //! @param[in] codeLength   文字列のバイト数。この文字数より短い文字数でNULL終端が来た場合は、登録はそこで終了します。
    //! @param[in] fontSize     フォントサイズ
    //! @param[in] fontFace     フォント字形を指定する数値
    //! @param[in] lockGroup    文字をロックする際のグループ。0から31までの数値で指定してください。-1を指定した場合はロックしません。
    //! @param[in] isDrawingHyphenMinusInsteadOfNonBreakingHyphen NON-BREAKING HYPHEN(U+2011) の文字を描画する際に、HYPHEN-MINUS(U+002D) のグリフを使うかどうかです。
    //! @return プロットが必要な文字が何文字あったかを返します。つまり0が返された場合は、
    //!         全ての文字がUpdateTextureCacheを行わなくても描画できることを示します。
    //!
    uint32_t RegisterGlyphsWithLengthUtf8(const char* pCode, uint32_t codeLength, uint32_t fontSize, uint16_t fontFace, int lockGroup, bool isDrawingHyphenMinusInsteadOfNonBreakingHyphen);

    //! @brief テクスチャキャッシュを更新し、新規にプロットが必要な場合はプロットを行います。
    //!
    //! @details
    //! この関数の処理は非常に長くなる可能性があるため、暗転していない、ゲームが動いている
    //! タイミングでは、処理落ちが発生しないようにサブスレッドで呼び出す等の対策を取ってください。
    //!
    //! UpdateTextureCacheとGetGlyph（及び描画）は並行して行うことができます。ただし、
    //! RegisterGlyph、RegisterGlyphWithLength、CompleteTextureCacheはUpdateTextureCacheを
    //! 行っている最中は呼び出さないようにしてください。
    //!
    //! このメソッドは最大20KBのスタックを消費しますので、スレッドで動作させる場合はスタック
    //! サイズにご注意ください。
    //!
    void UpdateTextureCache();

    //! @brief テクスチャキャッシュの更新を完了します。
    //!
    //! @details
    //! このメソッドはUpdateTextureCacheが完了した後に呼び出してください。
    //! 描画と同時に行うことはできません。
    //!
    //! またこのメソッドを呼ぶと、以下の条件に一つも当てはまらない文字が使用不可能となります。
    //!
    //! - 前回CompleteTextureCacheを呼んでから一回以上描画されている(GetGlyphが呼ばれている)
    //! - 前回CompleteTextureCacheを呼んだ後にRegisterGlyphなどで描画する文字として登録された
    //! - 何れかのグループでロックされている
    //!
    void CompleteTextureCache();

    //! @brief 指定したグループのロックを解除します。
    //!
    //! @param[in] lockGroup    ロックを解除するグループ。0から31までの数値で指定してください。
    //!
    void ClearLockAllGlyphs(int lockGroup);

    //! @brief 指定した長さの文字列に含まれる文字のロックを解除します。UTF-16版。
    //!
    //! @param[in] pCode        ロックを解除する文字が含まれる文字列。
    //! @param[in] codeLength   文字列の文字数。この文字数より短い文字数でNULL終端が来た場合は、そこで終了します。
    //! @param[in] fontSize     フォントサイズ
    //! @param[in] fontFace     フォント字形を指定する数値
    //! @param[in] lockGroup    ロックを解除するグループ。0から31までの数値で指定してください。
    //!
    void ClearLockGlyphsWithLength(const uint16_t* pCode, uint32_t codeLength, uint32_t fontSize, uint16_t fontFace, int lockGroup)
    {
        ClearLockGlyphsWithLength(pCode, codeLength, fontSize, fontFace, lockGroup, false);
    }

    //! @brief 指定した長さの文字列に含まれる文字のロックを解除します。UTF-16版。
    //!
    //! @param[in] pCode        ロックを解除する文字が含まれる文字列。
    //! @param[in] codeLength   文字列の文字数。この文字数より短い文字数でNULL終端が来た場合は、そこで終了します。
    //! @param[in] fontSize     フォントサイズ
    //! @param[in] fontFace     フォント字形を指定する数値
    //! @param[in] lockGroup    ロックを解除するグループ。0から31までの数値で指定してください。
    //! @param[in] isDrawingHyphenMinusInsteadOfNonBreakingHyphen NON-BREAKING HYPHEN(U+2011) の文字を描画する際に、HYPHEN-MINUS(U+002D) のグリフを使うかどうかです。
    //!
    void ClearLockGlyphsWithLength(const uint16_t* pCode, uint32_t codeLength, uint32_t fontSize, uint16_t fontFace, int lockGroup, bool isDrawingHyphenMinusInsteadOfNonBreakingHyphen);

    //! @brief 指定した長さの文字列に含まれる文字のロックを解除します。UTF-8版。
    //!
    //! @param[in] pCode        ロックを解除する文字が含まれる文字列。
    //! @param[in] codeLength   文字列のバイト数。この文字数より短い文字数でNULL終端が来た場合は、そこで終了します。
    //! @param[in] fontSize     フォントサイズ
    //! @param[in] fontFace     フォント字形を指定する数値
    //! @param[in] lockGroup    ロックを解除するグループ。0から31までの数値で指定してください。
    //!
    void ClearLockGlyphsWithLengthUtf8(const char* pCode, uint32_t codeLength, uint32_t fontSize, uint16_t fontFace, int lockGroup)
    {
        ClearLockGlyphsWithLengthUtf8(pCode, codeLength, fontSize, fontFace, lockGroup, false);
    }

    //! @brief 指定した長さの文字列に含まれる文字のロックを解除します。UTF-8版。
    //!
    //! @param[in] pCode        ロックを解除する文字が含まれる文字列。
    //! @param[in] codeLength   文字列のバイト数。この文字数より短い文字数でNULL終端が来た場合は、そこで終了します。
    //! @param[in] fontSize     フォントサイズ
    //! @param[in] fontFace     フォント字形を指定する数値
    //! @param[in] lockGroup    ロックを解除するグループ。0から31までの数値で指定してください。
    //! @param[in] isDrawingHyphenMinusInsteadOfNonBreakingHyphen NON-BREAKING HYPHEN(U+2011) の文字を描画する際に、HYPHEN-MINUS(U+002D) のグリフを使うかどうかです。
    //!
    void ClearLockGlyphsWithLengthUtf8(const char* pCode, uint32_t codeLength, uint32_t fontSize, uint16_t fontFace, int lockGroup, bool isDrawingHyphenMinusInsteadOfNonBreakingHyphen);

    //! @brief テクスチャキャッシュの内容を初期化し、Initialize直後の状態に戻します。
    //!
    //! @details
    //! このメソッドを呼び出すと、フォントの初期化時に登録された代替文字も消えてしまいます。
    //! フォントの描画を呼び出す前に、このキャッシュを使用している各フォントで
    //! RegisterAlternateCharGlyphメソッドを呼び出すようにしてください。
    //!
    //! その後、Initializeと同様、フォントでGetGlyphを利用して描画を行うまでに、
    //! 一回はUpdateTextureCache及びCompleteTextureCacheを行うようにしてください。
    //!
    void ResetTextureCache();

    //! @brief テクスチャキャッシュ内に新しいラインを作成します。
    //!
    //! @details
    //! テクスチャキャッシュ内のラインは、足りなくなったら自動的に作られるため、
    //! 通常はこのメソッドを使用する必要はありません。
    //!
    //! このメソッドが必要になるのは、ほぼ32のラインが入るが、稀に64のラインが必要に
    //! なるというような場合です。(例えばセディーユ付きのラテン文字など、背の高い文字が
    //! 来た場合。)
    //! このような場合にテクスチャキャッシュが既に32のラインだけで埋まっていると、
    //! 64のラインを新たに作ることができず、64のラインを必要とする文字を描画できません。
    //!
    //! このような場合に、TextureCacheの初期化直後にこの関数で64のラインを一つ作って
    //! 置くことで、問題を回避することができます。
    //!
    //! 詳しくは、オーバービューのFAQの「テクスチャキャッシュに消すことのできる文字が
    //! 十分あるにも関わらず、文字を展開できないことがある」をご参照ください。
    //!
    //! @param[in] lineHeight   新しく作るラインの高さ
    //! @return ラインを作成できたらtrue。スペースが足りず、作成できなかったらfalse。
    //!
    bool CreateNewLine(LineHeight lineHeight)
    {
        return (CreateNewLineImpl(static_cast<uint8_t>(lineHeight)) != NULL);
    }

    //! @brief フォントリストの優先順位を変更します。
    //!
    //! @details
    //! フォントフェースに対し、複数のフォントが設定されているときに
    //! その優先順位を変更することができます。
    //! 例えば言語設定によって本体内蔵フォントの構築順を変えたいときなどに使用してください。
    //! 構築後は nn::font::ScalableFont::RegisterGlyph でグリフを再登録し、
    //! nn::font::TextureCache::UpdateTextureCache でテクスチャキャッシュを再構築する必要があります。
    //! デフォルトでは 0 から順番に値が入っています。
    //!
    //! @param[in] fontFaceIndex フォントフェースの番号
    //! @param[in] pNewFontList 新しいフォントリスト
    //!
    void ChangeFontListOrder(uint32_t fontFaceIndex, uint32_t* pNewFontList);

    //@}

    //----------------------------------------------------
    //! @name テクスチャ情報の取得
    //@{

    //! @brief  テクスチャオブジェクトバッファへのポインタを取得します。
    //!
    //! @return テクスチャオブジェクトバッファへのポインタを返します。
    //!
    font::TextureObject* GetTextureObject()
    {
        return &m_TexObj;
    }

    //! @brief  テクスチャオブジェクトバッファへのポインタを取得します。
    //!
    //! @return テクスチャオブジェクトバッファへのポインタを返します。
    //!
    const font::TextureObject* GetTextureObject() const
    {
        return &m_TexObj;
    }

    //! @brief  テクスチャキャッシュの幅を取得します。
    //!
    //! @return テクスチャキャッシュの幅
    //!
    uint32_t GetTextureCacheWidth() const
    {
        return m_TextureCacheWidth;
    }

    //! @brief  テクスチャキャッシュの高さを取得します。
    //!
    //! @return テクスチャキャッシュの高さ
    //!
    uint32_t GetTextureCacheHeight() const
    {
        return m_TextureCacheHeight;
    }

    //! @brief  縁取り効果の有無を取得します。
    //!
    //! @param[in]  fontFace    フォント字形を指定する数値
    //!
    //! @return 縁取り効果の有無
    //!
    bool IsBorderEffectEnabled(uint16_t fontFace) const;


    //! @brief テクスチャキャッシュにプロットされているグリフの数を数えます。
    //!
    //! @details
    //! マルチスレッドでプロットを行う際に、プロットが完了したかどうかを本関数で判定することができます。
    //! pCode で渡された文字コードに対し、プロットされているグリフの数を数えて返します。
    //! pCode にヌル文字が存在した場合、そこで処理を打ち切ります。
    //!
    //! @param[in] pCode 対象とする文字コードの配列です。
    //! @param[in] codeLength pCode で渡される文字コードの個数です。
    //! @param[in] fontSize フォントサイズです。
    //! @param[in] fontFace フォントフェースです。
    //! @return プロットされているグリフの数です。
    //!
    uint32_t CountPlottingGlyph(const uint16_t* pCode, uint32_t codeLength, uint32_t fontSize, uint16_t fontFace);

    //@}

    //----------------------------------------------------
    //! @name 開発向け機能
    //@{

    //! @brief  テクスチャキャッシュ内の平衡二分木の内容をダンプします。
    //!
    //! @details
    //! このメソッドを実行すると、以下のような文字列が、キャッシュに含まれている文字数と
    //! 同じ行数だけコンソールに出力されます。
    //!
    //!   0 0x32 40 rp lock:0x1800
    //!
    //! この一行が一文字を表しています。一つ目の数値はフォントの種類(0～31の数値)、
    //! 二つ目の数値が文字コード、三つ目の数値がサイズ、次の文字列が内部的なフラグ値、
    //! 最後のlock:の後に続く数値は、ロック状態を32ビットの数値で表します。
    //! 例えばこの場合は、グループ11と12でロックされていることになります。
    //!
    //! インデントは平衡二分木にどのように格納されているかを表しています。
    //!
    //! また、最後に
    //!
    //! dump 102 nodes.
    //!
    //! のように表示されます。これは、テクスチャキャッシュで管理しているグリフの数を
    //! 示します。
    //!
    //! この数が、InitializeArgのglyphNodeCountMaxで指定した数値を超えると新たにグリフを
    //! 登録できなくなってしまいますので、ある程度余裕があるようにしてください。
    //!
    //! Releaseビルドでは何もしません。
    //!
    void DumpGlyphTreeMap() const
    {
        m_GlyphTreeMap.Dump();
    }

    //! @brief  テクスチャキャッシュ内の平衡二分木の内容をダンプします。
    //!
    //! @details
    //! テクスチャキャッシュ内のノードごとに callback 関数を呼び出します。
    //! Releaseビルドでは何もしません。
    //!
    //! @param[in] callback コールバック関数です。
    //! @param[in] pUserData コールバック関数に渡されるユーザデータです。
    //!
    void DumpGlyphTreeMapToFunction(GlyphTreeMap::DumpCallbackFunction callback, void* pUserData) const
    {
        m_GlyphTreeMap.DumpToFunction(callback, pUserData);
    }

    //! @brief  フォントの中に指定した文字コードの文字が含まれているかを返します。
    //!
    //! @details
    //! このメソッドが返す値は、文字がプロットされているかどうかには関係しません。
    //! 初期化時に与えたフォントデータ内に文字が含まれているかのみに依存します。
    //!
    //! @param[in] code         幅を求めたい文字
    //! @param[in] fontFace     フォント字形を指定する数値
    //! @return 含まれるか否か
    //!
    bool IsGlyphExistInFont(uint32_t code, uint16_t fontFace);

    //! @brief このテクスチャキャッシュを利用して、マルチコアでの描画が行えるか否かを返します。
    //!
    //! @return マルチコアでの描画が行えるか否か
    //!
    bool IsDrawMultiCore() const { return m_IsDrawMultiCore; }
    //@}

    //----------------------------------------------------
    //! @name エラー状態の取得・クリア
    //@{

    //! @brief  テクスチャキャッシュに空きがないエラーが発生したか否かを取得します。
    //!
    //! @details
    //! このエラーは、発生した後はClearNoSpaceErrorを呼ばない限りクリアされません。
    //! エラーが発生した際の対処方法は、下記の通りです。
    //!
    //! @li NoSpaceError_NoError
    //! エラーは発生していません。
    //!
    //! @li NoSpaceError_LackOfGlyphNodeSpace
    //! グリフノードに空きがありません。
    //! InitializeArg::glyphNodeCountMax の値を増やしてください。
    //!
    //! @li NoSpaceError_TextureCacheLineIsOverMax
    //! テクスチャキャッシュのラインの数が上限を超えました。
    //! InitializeArg::textureCacheWidth の値を増やすか、テクスチャキャッシュで管理するフォントやサイズの種類を減らしてください。
    //!
    //! @li NoSpaceError_LackOfTextureCacheSpace
    //! テクスチャキャッシュに空きがありません。
    //! InitializeArg::textureCacheWidth や InitializeArg::textureCacheHeight の値を増やしてください。
    //!
    //! @return エラーが発生しなかった場合は NoSpaceError_NoError、発生した場合はエラーに応じた値が返ります。
    //!
    NoSpaceError GetNoSpaceError() const
    {
        return m_NoSpaceError;
    }

    //! @brief  テクスチャキャッシュに空きがないエラーが発生したか否かをクリアします。
    //!
    void ClearNoSpaceError()
    {
        m_NoSpaceError = NoSpaceError_NoError;
    }

    //! @brief テクスチャキャッシュのワークメモリが枯渇したか否かを取得します。
    //!
    //! @details
    //! このエラーは、発生した後はClearMemoryExhaustedを呼ばない限りクリアされません。
    //! エラーが発生した際には、InitializeArg::noPlotWorkMemorySize の値を増やしてください。
    //!
    //! @return テクスチャキャッシュのワークメモリが枯渇したか否かです。
    //!
    bool GetWorkMemoryExhausted() const
    {
        return m_IsWorkMemoryExhausted;
    }

    //! @brief テクスチャキャッシュのワークメモリが枯渇したか否かをクリアします。
    //!
    void ClearWorkMemoryExhausted()
    {
        m_IsWorkMemoryExhausted = false;
    }

    //@}

    //----------------------------------------------------
    //! @name ScalableFontクラスの内部実装用
    //@{

    // フォントの属性値です。

    //! @brief 内部用機能のため使用禁止です。
    struct FontMetrics {
        float ascent_ratio;     // アセントの割合。ベースラインとも呼びます。
        float height_ratio;     // 高さの割合。これからアセントを引いた値がディセントになります。
        float boundingBoxAscentRatio; // 実際の文字が収まる矩形から算出したアセントの割合です。
        float boundingBoxHeightRatio; // 実際の文字が収まる矩形から算出した高さの割合です。
        float scaleWidth; // 文字の幅のスケールです。
        float scaleHeight; // 文字の高さのスケールです。
        int baselineOffset; // ベースラインを調整するオフセット値です。
    };

    // フォントの属性値を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] fontFace フォント字形を指定する数値
    //!
    //! @return フォントの属性値です。
    //!
    const FontMetrics& GetFontMetrics(uint16_t fontFace) const;

    // 指定した文字の幅を求めます。
    // スケーラブルフォントエンジンから文字の情報を検索して求めますので、処理コストがかかる
    // ことにご注意ください。
    // 指定された文字がフォントに含まれていない場合は0が返されます。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] code         幅を求めたい文字
    //! @param[in] fontSize     フォントサイズ
    //! @param[in] fontFace     フォント字形を指定する数値
    //! @return 文字の幅
    //!
    int CalculateCharWidth(uint32_t code, uint32_t fontSize, uint16_t fontFace);

    // カーニングの値を求めます。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  c0      カーニングの値を求める、進行方向と逆側の文字です。
    //! @param[in]  c1      カーニングの値を求める、進行方向側の文字です。
    //! @param[in] fontSize フォントサイズです。
    //! @param[in] fontFace フォント字形を指定する数値です。
    //!
    //! @return     カーニングの値を返します。
    //!
    int CalculateKerning(uint32_t c0, uint32_t c1, uint32_t fontSize, uint16_t fontFace);

    // 文字の平衡二分木内のノードを取得します。
    // 文字のノードをプロット中に書き換えると誤動作する可能性がありますので、
    // このメソッドを使用するときは、UpdateTextureCacheを行っていないときに
    // するか、値を参照するだけにするよう注意してください。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] code         幅を求めたい文字
    //! @param[in] fontSize     フォントサイズ
    //! @param[in] fontFace     フォント字形を指定する数値
    //! @return 文字のノード。平衡二分木内にノードがない場合はNULL
    //!
    GlyphNode* FindGlyphNode(uint32_t code, uint32_t fontSize, uint16_t fontFace);

    // テクスチャキャッシュの実メモリへのポインタを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return テクスチャキャッシュの実メモリへのポインタ
    //!
    const void* GetTextureCacheBitMap() const
    {
        return m_pTextureBitmap;
    }

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pCode        調査をする文字列
    //! @param[in] codeLength   文字列の文字数。この文字数より短い文字数でNULL終端が来た場合は、登録はそこで終了します。
    //! @param[in] fontSize     フォントサイズ
    //! @param[in] fontFace     フォント字形を指定する数値
    //! @return プロットが必要な文字が何文字あったかを返します。つまり0が返された場合は、
    //!         全ての文字がUpdateTextureCacheを行わなくても描画できることを示します。
    //!
    uint32_t CountUnusableGlyph(const uint16_t* pCode, uint32_t codeLength, uint32_t fontSize, uint16_t fontFace);
    //@}

private:

    static const int FontNameLengthMax = 50;
    static const int WorkMemorySizeDefault = 512 * 1024;
    static const int WorkMemorySizeNoPlotDefault = 50 * 1024;
    static const int GlyphNodeCountMaxDefault = 1024;
    static const int TextureCacheLineCountMax = 256;
    static const int GlyphPadding = 2;
    static const int CoreCountMax = 8; // 多めに確保して後で実際のコア数だけ使用します

    struct LineInfo {
        GlyphLineList list;
        uint16_t y;
        uint16_t currentX;
        uint8_t kind;
        uint8_t no;
        uint8_t dummy[2];
    };

    //----------------------------------------------------------

    LineInfo* CreateNewLineImpl(uint8_t lineKind);
    GlyphNode* FindAndReserveEraseGlyph(uint8_t lineKind, uint16_t glyphWidth);
    void SetFontFace(uint32_t innerFontFace);
    void SetFontFaceNoPlot(uint32_t innerFontFace, uint32_t coreId);
    void AssertFsError(const char* pApiName) const;
    void AssertFsErrorNoPlot(const char* pApiName, uint32_t no) const;

    //! @brief フォントにない文字のグリフ情報を削除します。
    //!
    //! @details
    //! TextureCacheは、登録されているフォントにない文字がリクエストされた場合、
    //! RegisterGlyphのタイミングではいったんプロットが必要な文字として受付して
    //! おき、UpdateTextureCacheの中で改めて文字がフォント内にあるか判別して、
    //! ない場合は「フォントにない文字」としてフラグを立て、情報を保持します。
    //!
    //! これは、RegisterGlyphのときにフォントに文字があるかどうかを判別して
    //! しまうと、判別の処理によってRegisterGlyphの処理量が増えてしまうためです。
    //! つまり、グリフ情報の中に「この文字はフォントに含まれない」という情報を
    //! キャッシュすることになります。
    //!
    //! ただ、この情報は、テクスチャキャッシュを圧迫してしまうため、大量にたまる
    //! と(つまり、フォントにない文字をたくさんRegisterGlyphすると)グリフ情報を
    //! 保持するためのノードが足りなくなる可能性があります。
    //!
    //! この関数は、そのような状況でノードが足りなくなった場合に、フォントにない文字
    //! のグリフ情報を削除するためのものです。
    //!
    //! 今のところは、RegisterGlyph内でグリフの数が足りなくなった場合に内部的に
    //! 呼ぶ仕組みになっています。
    //!
    //! 必要な状況があれば、公開する可能性があります。
    //!
    //! @return 削除した文字数
    //!
    uint32_t EraseNotInFontGlyphs();

    char* GetFontNameBuf(uint32_t innerFontFace) const
    {
        return &m_pFontNameBuffers[innerFontFace * FontNameLengthMax];
    }

    uint32_t GetFontEngineNoPlotCount() const
    {
        return (m_IsDrawMultiCore ? m_CoreCount : 1);
    }

    uint32_t GetCoreId() const;

    // 内部用 FontFace テーブルの初期化。 FontFace の総数が返ります。
    int InitializeFontFaceTable(const InitializeArg& arg);

    // 外部用 FontFace を内部用 FontFace に変換します。 存在しない場合は false が返ります。
    bool GetInnerFontFace(uint32_t* pInnerFontFace, uint32_t outerFontFace, uint32_t code);

    // 指定した文字コードが CharCodeRange の中に含まれているかを判別します。
    bool CheckCharCodeRange(uint32_t fontFace, uint32_t code) const;

    // 内部メモリープールが利用されているかどうか
    bool IsInternalMemoryPoolUsed() const
    {
        return m_pActiveMemoryPoolForTexture == &m_MemoryPoolForTexture;
    }

    // OTF のカーニングテーブルを読み込む必要があるかどうかを判別します。
    bool CheckNecessaryToReadOtfKerningTable(const InitializeArg& arg, uint32_t fontFace, uint32_t idx) const
    {
        return (arg.pFontDataTypes[fontFace][idx] == FontDataType_Otf || arg.pFontDataTypes[fontFace][idx] == FontDataType_Bfotf) &&
            !arg.deleteBearingX[fontFace][idx] &&
            !arg.forceMonospacedEnabled[fontFace][idx];
    }

    //----------------------------------------------------------

#if defined(NN_FONT_USE_FONTLL2)
    nn::fontll2::ScalableFontEngine* m_pFontEngine;
    nn::fontll2::ScalableFontEngine* m_pFontEngineNoPlot;
#else
    nn::fontll::ScalableFontEngine* m_pFontEngine;
    nn::fontll::ScalableFontEngine* m_pFontEngineNoPlot;
#endif
    char* m_pFontNameBuffers;
    uint8_t* m_pTextureBitmap;
    uint32_t m_TextureCacheWidth;
    uint32_t m_TextureCacheHeight;
    GlyphTreeMap m_GlyphTreeMap;
    GlyphList m_NeedPlotGlyphList;
    GlyphList m_NeedEraseGlyphList;
    GlyphList m_NotInFontGlyphList;

    // m_GlyphTreeMap のワークメモリ中に作られたノードが
    // m_LineInfos[].list のリストに追加されるため、
    // m_GlyphTreeMap のワークメモリを解放する前に m_LineInfos[].list.clear() を呼ばなければならない。
    LineInfo m_LineInfos[TextureCacheLineCountMax];

    uint32_t m_LineCurrentPos;
    uint32_t m_FontCount;
    uint8_t m_FontListCount[FontFaceCountMax];
    uint8_t* m_InnerFontFaceHeadTable[FontFaceCountMax]; // 外部フォントフェースから内部フォントフェースの先頭ポインタに変換するテーブル
    uint8_t m_InnerFontFaceTable[FontFaceCountMax * MultiScalableFontCountMax]; // 内部フォントフェースを並べたテーブル
    uint32_t m_CurrentFontFace;
    uint32_t m_CurrentFontFacesNoPlot[CoreCountMax];
    NoSpaceError m_NoSpaceError;
    bool m_IsWorkMemoryExhausted;
    bool m_IsDrawMultiCore;
    int m_CoreCount;
    int (*m_pGetCoreNumberFunction)();
    uint8_t(*m_pCalculateLineKindFunction)(uint16_t height);
    uint32_t(*m_pCalculateLineHeightFunction)(uint8_t kind);
    FontMetrics* m_pFontMetrics;
    int32_t* m_pBoldWeights;
    uint8_t* m_pBorderWidths;
    bool* m_pDeleteBearingX;
    int16_t* m_pBearingOffsetX;
    bool* m_pForceMonospacedEnabled;
    int16_t* m_pForceMonospacedWidth;
    int* m_pCharCodeRangeCount;
    uint32_t (*m_pCharCodeRangeFirst)[CharCodeRangeCountMax];
    uint32_t (*m_pCharCodeRangeLast)[CharCodeRangeCountMax];
#if defined(NN_FONT_USE_FONTLL2)
    nn::fontll2::OtfKerningTable** m_pOtfKerningTable;
#else
    nn::fontll::OtfKerningTable** m_pOtfKerningTable;
#endif
    bool m_IsExactWidthReturnedAfterPlot;

    nn::gfx::Texture                m_Texture;
    nn::gfx::TextureView            m_TextureView;
    nn::gfx::MemoryPool             m_MemoryPoolForTexture;
    nn::gfx::MemoryPool*            m_pActiveMemoryPoolForTexture;
    ptrdiff_t                       m_OffsetOfMemoryPoolForTexture;
    font::PlacementTextureObject    m_TexObj;       //!< テクスチャオブジェクト

#if NN_GFX_IS_TARGET_VK || NN_GFX_IS_TARGET_GL
    nn::gfx::Device*                m_pDevice;
    nn::gfx::MemoryPool             m_TemporalMemoryPool;
    nn::gfx::CommandBuffer          m_TemporalCommandBuffer;
    nn::gfx::Buffer                 m_TemporalBuffer;
    nn::gfx::Queue                  m_TemporalQueue;
    void*                           m_pTemporalCommandMemory;
    void*                           m_pTemporalControlMemory;
    static const int                TemporalCommandMemorySize = 1024;
    static const int                TemporalControlMemorySize = 64;
#endif

};

} // namespace nn::font
} // namespace nn
