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

#include <nn/nn_SdkLog.h>
#include <nn/ui2d/ui2d_Types.h>
#include <nn/util/util_MathTypes.h>
#include <nn/ui2d/ui2d_GraphicsResource.h>
#include <nn/ui2d/ui2d_DrawInfo.h>
#include <nn/ui2d/detail/ui2d_Log.h>

namespace nn { namespace ui2d {
struct BuildArgSet;
} }
#include <nn/ui2d/ui2d_Material.h>

#include <functional>

// 以下のマクロを定義すると、
// 「子階層に透明度の影響を与える」を無効にしたときに親が有効なら有効化されてしまう不具合が修正されます。
// NintendoSDK 5.0.0 以降で修正される不具合ですが、4.x.x では互換性維持のためにデフォルトでは未修正となっています。
// 5.0.0 と同様の修正を行いたい場合は有効化してリビルドしてください。
//
#define NN_UI2D_FIX_INFLUENCE_ALPHA_BUG

// 以下のマクロを定義すると、
// nn::gfx::BlendState の チャンネルマスク（カラーマスク）設定を適用しません。
// 互換性の問題が生じる移植タイトルで利用します。
// デフォルトで無効になっています。利用する場合は、有効化してリビルドしてください。
// #define NN_UI2D_DONOT_APPLY_NNGFX_CHANNEL_MASK


// ビルドターゲットにかかわらず出力します。

#define NN_DETAIL_UI2D_WARNING(exp, ...)  if(!((void)0, exp)) { NN_DETAIL_UI2D_WARN("%s:%d Warning:", __FILE__, __LINE__); NN_DETAIL_UI2D_WARN(__VA_ARGS__); NN_DETAIL_UI2D_WARN("\n"); }

namespace nn
{
namespace ui2d
{

class Material;
struct Size;
class DrawInfo;
class GraphicsResource;
class Layout;
class ResourceAccessor;
class TextSearcher;
class ControlCreator;
class Pane;

struct ResTextureList;
struct ResFontList;
struct ResMaterialList;
struct ResPartsPaneBasicInfo;
struct ResExtUserDataList;
struct ResMaterial;

namespace detail
{
class BuildPaneTreeContext;
}

//---------------------------------------------------------------------------
//! @brief ユーザー拡張データから取得するユーザーシェーダー情報です。
//!
//---------------------------------------------------------------------------
struct UserShaderInformation
{
    //! ユーザーシェーダーの名前。ArchiveShader-XXXXX.bnsh の「XXXXX」に該当する部分。
    char    userShaderName[ArchiveShaderNameMax];
    //! 頂点シェーダーコンスタントバッファの拡張領域のサイズ
    uint32_t    vertexShaderConstantBufferExtendSize;
    //! ジオメトリシェーダーコンスタントバッファの拡張領域のサイズ
    uint32_t    geometryShaderConstantBufferExtendSize;
    //! ピクセルシェーダーコンスタントバッファの拡張領域のサイズ
    uint32_t    pixelShaderConstantBufferExtendSize;

    //! デフォルト値を設定する
    void SetDefault();

    //! @brief  ユーザーシェーダーの名前を設定します。
    //!
    //! @param[in]  pShaderName 設定するシェーダー名文字列。
    //!
    //! @return 有効なシェーダー名が設定されると true を返します。
    bool SetShaderName(const char* pShaderName);
};

/*!
    @brief ユーザー拡張データからユーザーシェーダーを取得するコールバック関数の型です。

    @param[out]  userShaderInformation  ユーザー拡張データから取得されたユーザーシェーダー情報。
    @param[in]   pExtUserDataList       ユーザー拡張データ。
*/
typedef bool (*GetUserShaderInformationFromUserData)(UserShaderInformation& userShaderInformation, const ResExtUserDataList* pExtUserDataList, void* pUserData);

//---------------------------------------------------------------------------
//! @brief 構築時に必要なリソースへのポインタを持つ構造体です。
//!
//---------------------------------------------------------------------------
struct BuildResSet
{
    //! テクスチャリソースのリストです。
    const ResTextureList* pTextureList;
    //! フォントリソースのリストです。
    const ResFontList* pFontList;
    //! マテリアルリソースのリストです。
    const ResMaterialList* pMaterialList;
    //! シェイプ情報リソースのリストです。
    const ResShapeInfoList* pShapeInfoList;
    //! キャプチャテクスチャ情報リソースのリストです。
    const ResCaptureTextureList* pCaptureTextureList;
    //! ベクターグラフィックステクスチャ情報リソースのリストです。
    const ResVectorGraphicsTextureList* pVectorGraphicsTextureList;
    //! リソースアクセサです。
    ResourceAccessor* pResAccessor;
    //! このリソースを保持するレイアウトクラスです。
    Layout* pLayout;
};

/*!
    @brief  レンダーターゲットとなるテクスチャ作成のためのコールバック関数の型です。

    @param[out] pTexture        ユーザーが作成した nn::gfx::Texture のインスタンスを受け取るためのポインタです。
    @param[out] pTextureView    ユーザーが作成した nn::gfx::TextureView のインスタンスを受け取るためのポインタです。
    @param[out] pSlot           ユーザーが作成した nn::gfx::DescriptorSlot のインスタンスを受け取るためのポインタです。
    @param[in]  info            作成するテクスチャの情報です。
    @param[in]  pUserData       ユーザーデータのポインタです。
    @param[in]  lifetimeHint    作成するテクスチャの寿命ヒントです。

    @deprecated このコールバックは廃止予定です。 CreateRenderTargetTextureResourceCallback を使用してください。

    @return 作成したテクスチャの実際の Lifetime を返します。ui2d ランタイムはこの情報に基づいて初期化処理、終了処理を呼び出すタイミングを決定します。
*/
typedef RenderTargetTextureLifetime (*CreateRenderTargetTextureCallback)(nn::gfx::Texture** pTexture, nn::gfx::TextureView** pTextureView, nn::gfx::DescriptorSlot** pSlot, const nn::gfx::TextureInfo& info, void* pUserData, RenderTargetTextureLifetime lifetimeHint);

/*!
    @brief  レンダーターゲットとなるテクスチャ破棄のためのコールバック関数の型です。

    @param[out] pTexture     破棄する nn::gfx::Texture のポインタです。
    @param[out] pTextureView 破棄する nn::gfx::TextureView のポインタです。
    @param[out] pSlot        破棄するテクスチャの DescriotrSlot のポインタです。
    @param[in]  pUserData   ユーザーデータのポインタです。
    @param[in]  lifetimeHint    作成するテクスチャの寿命ヒントです。

    @deprecated このコールバックは廃止予定です。 DestroyRenderTargetTextureResourceCallback を使用してください。
*/
typedef void (*DestroyRenderTargetTextureCallback)(nn::gfx::Texture* pTexture, nn::gfx::TextureView* pTextureView, nn::gfx::DescriptorSlot* pSlot, void* pUserData, RenderTargetTextureLifetime lifetimeHint);

/*!
    @brief  レンダーターゲットとなるテクスチャ作成のためのコールバック関数の型です。

    @param[out] pTexture        ユーザーが作成した nn::gfx::Texture のインスタンスを受け取るためのポインタです。
    @param[out] pTextureView    ユーザーが作成した nn::gfx::TextureView のインスタンスを受け取るためのポインタです。
    @param[out] pSlot           ユーザーが作成した nn::gfx::DescriptorSlot のインスタンスを受け取るためのポインタです。
    @param[in]  pLayout         作成したレンダーターゲットテクスチャを管理するレイアウトインスタンスへのポインタです。
    @param[in]  info            作成するテクスチャの情報です。
    @param[in]  pUserData       ユーザーデータのポインタです。
    @param[in]  lifetimeHint    作成するテクスチャの寿命ヒントです。

    @return 作成したテクスチャの実際の Lifetime を返します。ui2d ランタイムはこの情報に基づいて初期化処理、終了処理を呼び出すタイミングを決定します。
*/
typedef RenderTargetTextureLifetime (*CreateRenderTargetTextureResourceCallback)(nn::gfx::Texture** pTexture, nn::gfx::TextureView** pTextureView, nn::gfx::DescriptorSlot** pSlot, const Layout* pLayout, const nn::gfx::TextureInfo& info, void* pUserData, RenderTargetTextureLifetime lifetimeHint);

/*!
    @brief  レンダーターゲットとなるテクスチャ破棄のためのコールバック関数の型です。

    @param[out] pTexture     破棄する nn::gfx::Texture のポインタです。
    @param[out] pTextureView 破棄する nn::gfx::TextureView のポインタです。
    @param[out] pSlot        破棄するテクスチャの DescriotrSlot のポインタです。
    @param[in]  pLayout         作成したレンダーターゲットテクスチャを管理するレイアウトインスタンスへのポインタです。
    @param[in]  pUserData   ユーザーデータのポインタです。
    @param[in]  lifetimeHint    作成するテクスチャの寿命ヒントです。
*/
typedef void (*DestroyRenderTargetTextureResourceCallback)(nn::gfx::Texture* pTexture, nn::gfx::TextureView* pTextureView, nn::gfx::DescriptorSlot* pSlot, const Layout* pLayout, void* pUserData, RenderTargetTextureLifetime lifetimeHint);


//---------------------------------------------------------------------------
//! @brief 構築時に必要なデータを保持する構造体です。
//!
//---------------------------------------------------------------------------
struct BuildArgSet
{
    //! ペインのサイズに全て共通でかける倍率です。
    nn::util::Float2 magnify;
    //! 部品としてのサイズです。
    nn::util::Float2 partsSize;
    //! コントロールを作成するクラスです。
    ControlCreator* pControlCreator;
    //! テキストの検索を行うクラスです。
    TextSearcher* pTextSearcher;
    //! 現在構築を行っているレイアウトクラスです。
    Layout* pLayout;
    //! 現在構築を行っているボディレイアウトクラスです。
    Layout* pBodyLayout;
    //! 現在構築を行っているレイアウトのリソースセットです。
    const BuildResSet* pCurrentBuildResSet;
    //! 上書きを行っているレイアウトのリソースセットです。パーツレイアウトでない場合はNULLになります。
    const BuildResSet* pOverrideBuildResSet;
    //! 部分的な上書きを行う場合のフラグです。
    uint16_t  overrideUsageFlag;
    //! 基本情報の上書きを行う場合のフラグです。
    uint16_t  overrideBasicUsageFlag;
    //! マテリアルの部分上書きを行う場合のフラグです。
    uint16_t  overrideMaterialUsageFlag;
    //! ペイン基本情報を上書きする場合に、データの参照先を指します。
    const ResPartsPaneBasicInfo* pOverridePartsPaneBasicInfo;
    //! キャプチャテクスチャの名前修飾用のパーツ名階層構造のスタックです。
    const char*   pCaptureTexturePrefixStack[CaptureTexturePartsLayerMax];
    //! キャプチャテクスチャ名前修飾用文字列スタックの現在の階層です。
    int     captureTexturePrefixStackPosition;
    //! キャプチャテクスチャリソースの上書き時のパーツレイヤーの階層です。
    int     captureTextureOverridePosition;
    //! 親ペインへのポインタです。
    Pane* pParentPane;

    //! ペインツリー構築中に階層構造を超えて受け渡される情報です。ui2d ランタイムが自動的に設定する項目のためユーザーが設定する必要はありません。
    detail::BuildPaneTreeContext*    pBuildPaneTreeContext;
    //! VectorGraphicsTexture 関連のインスタンスを作成せず、代わりに DummyRenderTargetTextureInfo を生成し TexMap へ設定します。(内部用機能のため使用しないでください)
    bool    useDummyVectorGraphicsTextureInfo;

    //! ユーザーデータからユーザーシェーダー情報を取り出すためのコールバック関数。
    GetUserShaderInformationFromUserData pGetUserShaderInformationFromUserDataCallback;
    //! ユーザーデータからユーザーシェーダー情報を取り出すためのコールバック関数に渡すユーザデータ。
    void* pGetUserShaderInformationFromUserDataCallbackUserData;
    //! 拡張ユーザーデータリストです。
    const ResExtUserDataList* pExtUserDataList;
    //! ルートペインを部品ペインにするか否かです。
    bool isRootPaneParts;
    //! UTF-8で構築するか否かです。
    bool isUtf8;
    //! テキストボックスが文字の高さに応じて改行幅を自動調整するかどうかです。
    bool isLinefeedByCharacterHeightEnabled;
    //! リソースのバージョンです。
    uint32_t resourceVersion;
#if ! defined(NN_SDK_BUILD_RELEASE)
    //! 親レイアウトの影響で、ビューアで非表示状態になっているか否かです。
    bool isViewerInvisibleByParent;
#endif
};


//---------------------------------------------------------------------------
//! @brief レイアウトデータを初期化した際に収集する統計情報です。
//!
//---------------------------------------------------------------------------
struct BuildResultInformation
{
    //! 初期化したレイアウトデータをレンダリングするために必要な ui2d 用のコンスタントバッファのサイズです。
    //! 四角形以外の形状を設定している場合はメッシュ用の頂点バッファ、インデックスバッファのサイズも含まれます。
    size_t  requiredUi2dConstantBufferSize;
    //! 初期化したレイアウトデータをレンダリングするために必要な font 用のコンスタントバッファのサイズです。
    size_t  requiredFontConstantBufferSize;

    //! デフォルト値を設定する
    void SetDefault() NN_NOEXCEPT;
};

namespace detail
{

enum FrameSpecFlag
{
    FrameSpecFlag_VertexEffectTexcoordEnabled = 1 << 0,

    FrameSpecFlag_VertexEffectFrame = 1 << 1,
    FrameSpecFlag_VertexEffectTexcoordAlignRight = 1 << 2,
    FrameSpecFlag_VertexEffectTexcoordAlignBottom = 1 << 3,
    FrameSpecFlag_VertexEffectDotByDotU = 1 << 4,
    FrameSpecFlag_VertexEffectDotByDotV = 1 << 5,

    FrameSpecFlag_VertexEffectFrameIdShift = 8,
    FrameSpecFlag_VertexEffectFrameIdMask = 0xf << FrameSpecFlag_VertexEffectFrameIdShift, // 9 ～ 12 ビット目
    FrameSpecFlag_VertexEffectTexCoordSwap = 1 << 16,
    FrameSpecFlag_VertexEffectTexCoordHFlip = 1 << 17,
    FrameSpecFlag_VertexEffectTexCoordVFlip = 1 << 18,

    FrameSpecFlag_VertexEffectConstColor = 1 << 30,

    FrameSpecFlag_FrameIdLt = WindowFrame_LeftTop << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_FrameIdRt = WindowFrame_RightTop << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_FrameIdLb = WindowFrame_LeftBottom << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_FrameIdRb = WindowFrame_RightBottom << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_FrameIdL = WindowFrame_Left << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_FrameIdR = WindowFrame_Right << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_FrameIdT = WindowFrame_Top << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_FrameIdB = WindowFrame_Bottom << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_Frame4IdLt = (WindowFrame_Bottom + 1) << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_Frame4IdRt = (WindowFrame_Bottom + 2) << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_Frame4IdLb = (WindowFrame_Bottom + 3) << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_Frame4IdRb = (WindowFrame_Bottom + 4) << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_Frame2IdL = (WindowFrame_Bottom + 5) << FrameSpecFlag_VertexEffectFrameIdShift,
    FrameSpecFlag_Frame2IdC = (WindowFrame_Bottom + 6) << FrameSpecFlag_VertexEffectFrameIdShift, // Content

    FrameSpecFlag_Content = FrameSpecFlag_Frame2IdC,
    FrameSpecFlag_FrameHorizontalL = FrameSpecFlag_FrameIdL | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame,
    FrameSpecFlag_FrameHorizontalR = FrameSpecFlag_FrameIdR | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame,
    FrameSpecFlag_FrameHorizontalNoContentL = FrameSpecFlag_Frame2IdL | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame | FrameSpecFlag_VertexEffectDotByDotU,
    FrameSpecFlag_FrameHorizontalNoContentR = FrameSpecFlag_FrameIdR | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame,
    FrameSpecFlag_FrameLt = FrameSpecFlag_FrameIdLt | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame,
    FrameSpecFlag_FrameRt = FrameSpecFlag_FrameIdRt | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame,
    FrameSpecFlag_FrameLb = FrameSpecFlag_FrameIdLb | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame,
    FrameSpecFlag_FrameRb = FrameSpecFlag_FrameIdRb | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame,
    FrameSpecFlag_FrameL = FrameSpecFlag_FrameIdL | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame | FrameSpecFlag_VertexEffectDotByDotV | FrameSpecFlag_VertexEffectTexcoordAlignBottom,
    FrameSpecFlag_FrameR = FrameSpecFlag_FrameIdR | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame | FrameSpecFlag_VertexEffectDotByDotV,
    FrameSpecFlag_FrameT = FrameSpecFlag_FrameIdT | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame | FrameSpecFlag_VertexEffectDotByDotU,
    FrameSpecFlag_FrameB = FrameSpecFlag_FrameIdB | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame | FrameSpecFlag_VertexEffectDotByDotU | FrameSpecFlag_VertexEffectTexcoordAlignRight,
    FrameSpecFlag_Frame4Lt = FrameSpecFlag_Frame4IdLt | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame | FrameSpecFlag_VertexEffectDotByDotU,
    FrameSpecFlag_Frame4Rt = FrameSpecFlag_Frame4IdRt | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame | FrameSpecFlag_VertexEffectDotByDotV,
    FrameSpecFlag_Frame4Lb = FrameSpecFlag_Frame4IdLb | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame | FrameSpecFlag_VertexEffectDotByDotV | FrameSpecFlag_VertexEffectTexcoordAlignBottom,
    FrameSpecFlag_Frame4Rb = FrameSpecFlag_Frame4IdRb | FrameSpecFlag_VertexEffectTexcoordEnabled | FrameSpecFlag_VertexEffectFrame | FrameSpecFlag_VertexEffectDotByDotU | FrameSpecFlag_VertexEffectTexcoordAlignRight,
    FrameSpecFlag_FlipHFlip = FrameSpecFlag_VertexEffectTexCoordHFlip,
    FrameSpecFlag_FlipVFlip = FrameSpecFlag_VertexEffectTexCoordVFlip,
    FrameSpecFlag_FlipR90 = FrameSpecFlag_VertexEffectTexCoordHFlip | FrameSpecFlag_VertexEffectTexCoordSwap,
    FrameSpecFlag_FlipR180 = FrameSpecFlag_VertexEffectTexCoordHFlip | FrameSpecFlag_VertexEffectTexCoordVFlip,
    FrameSpecFlag_FlipR270 = FrameSpecFlag_VertexEffectTexCoordVFlip | FrameSpecFlag_VertexEffectTexCoordSwap,
    FrameSpecFlag_Normal = 0
};

template <int StrMax>
bool EqualsName(const char* pName1, const char* pName2)
{
    for (int i = 0; i < StrMax; i++) {
        if (pName1[i] != pName2[i]) {
            return false;
        } else if (pName1[i] == '\0') {
            return true;
        }
    }
    return true;
}

inline bool EqualsResName(const char* pName1, const char* pName2)
{
    return EqualsName<ResourceNameStrMax>(pName1, pName2);
}

inline bool EqualsMaterialName(const char* pName1, const char* pName2)
{
    return EqualsName<MaterialNameStrMax>(pName1, pName2);
}

inline bool EqualsFontName(const char* pName1, const char* pName2)
{
    return EqualsName<FontNameMax>(pName1, pName2);
}

inline bool EqualsTexImageName(const char* pName1, const char* pName2)
{
    return EqualsName<TexImageNameMax>(pName1, pName2);
}

inline bool EqualsShaderName(const char* pName1, const char* pName2)
{
    return EqualsName<ShaderNameMax>(pName1, pName2);
}

inline bool EqualsLayoutName(const char* pName1, const char* pName2)
{
    return EqualsName<LayoutNameStrMax>(pName1, pName2);
}

inline bool EqualsGroupName(const char* pName1, const char* pName2)
{
    return EqualsName<GroupNameStrMax>(pName1, pName2);
}

inline const char*
GetStrTableStr(const void* pStrTable, int index)
{
    const uint32_t* pOffsets = static_cast<const uint32_t*>(pStrTable);
    const char* pStringPool = static_cast<const char*>(pStrTable);

    return &pStringPool[pOffsets[index]];
}

// 動的レンダリングテクスチャのための基底クラスです。
class DynamicRenderingTexture
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO_ROOT();

public:
    //! @brief  コンストラクタです。
    //!
    DynamicRenderingTexture();

    //! @brief  コンストラクタです。
    //!
    //! @param[in] pTextureName テクスチャ名の文字列です。
    //!
    explicit DynamicRenderingTexture(const char* pTextureName);

    //! @brief  デストラクタです。
    //!
    virtual ~DynamicRenderingTexture();

    //! @brief 終了処理です。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    virtual void Finalize(nn::gfx::Device* pDevice);

    //! @brief  更新処理です。
    //!
    //! @param[in]  drawInfo    描画情報です。
    //!
    virtual void Calculate(DrawInfo& drawInfo)
    {
        NN_UNUSED(drawInfo);
    }

    //! @brief テクスチャの描画処理です。
    //!
    //! @details
    //! 内容物を描画してテクスチャを更新します。
    //!
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  drawInfo 描画情報です。
    //! @param[in]  commandBuffer   描画コマンドを設定するコマンドバッファです。
    //!
    virtual void Draw(nn::gfx::Device* pDevice, DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer)
    {
        NN_UNUSED(pDevice);
        NN_UNUSED(drawInfo);
        NN_UNUSED(commandBuffer);
    }

    //! @brief 描画結果を使用するためのテクスチャへのポインタを取得します。
    //!
    //! @return 描画結果のテクスチャです。
    //!
    RenderTargetTextureInfo* GetTextureInfo() const
    {
        return m_pRenderTargetTexture;
    }

    //! @brief このテクスチャの登録名を取得します。
    //!
    //! @return 名前文字列へのポインタです。
    //!
    const char* GetName() const
    {
        return m_pTextureName;
    }

    //! @brief この動的レンダリングテクスチャに関連付けられたペインを取得します。
    //!
    //! @details
    //! もともと CaptureTexture でしか使用しない想定のインターフェイスだが、実装の簡略化のために基底クラスで定義します。
    //!
    //! @return 関連付けられたペインのポインタです。
    //!
    virtual const Pane* GetTargetPane() const
    {
        return NULL;
    }

    //! @brief 初回フレームテクスチャ更新済みフラグをリセットします。
    //!
    //! @details
    //! もともと CaptureTexture でしか使用しない想定のインターフェイスだが、実装の簡略化のために基底クラスで定義します。
    //!
    virtual void ResetFirstFrameCaptureUpdatedFlag() {}
protected:
    // 名前用のメモリを確保してコピーします。
    void SetName(const char* pName);

private:
    RenderTargetTextureInfo*    m_pRenderTargetTexture;
    char*                       m_pTextureName;
};

// 矩形のテクスチャ座標を複数個保持するクラスです。
class TexCoordArray
{
public:
    // 初期化処理
    void Initialize();

    // 保持可能なセット数が０か調べます。
    bool IsEmpty() const
    {
        return m_Cap == 0;
    }

    // メンバーを解放し、保持可能なセット数を０に設定します。
    void Free();

    // 指定のセット数を保持するのに十分なメモリを確保します。
    void Reserve(int  count);

    // 保持しているセット数を取得します。
    int  GetSize() const
    {
        return m_Count;
    }

    // 保持するセット数を設定し、座標を初期化します。
    void SetSize(int  count);

    // テクスチャ座標配列の先頭アドレスを取得します。
    const TexCoordQuad* GetArray() const
    {
        return m_pData;
    }

    // 指定したセットのテクスチャ座標配列の先頭アドレスを取得します。
    void GetCoord(
        TexCoordQuad coord,
        int  idx
        ) const;

    // 指定したセットのテクスチャ座標を設定します。
    void SetCoord(
        int  idx,
        const TexCoordQuad coord);

    // リソースのテクスチャ座標をコピーします。
    void Copy(
        const void* pResTexCoord,
        int  texCoordCount);

    // TexCoordArray のインスタンスが同等の状態かチェックします。
    bool CompareCopiedInstanceTest(const TexCoordArray& target) const;

private:
    uint8_t  m_Cap;
    uint8_t  m_Count;

    nn::util::Float2 (*m_pData)[PaneVertex_MaxPaneVertex];
};

//----------------------------------------
const ResMaterial*
GetResMaterial(
        const BuildResSet* pBuildResSet,
        uint16_t  materialIdx);

//----------------------------------------
const ResShapeInfo*
GetResShapeInfo(
        const BuildResSet* pBuildResSet,
        uint32_t  shapeInfoIdx);

//----------------------------------------
template<std::size_t TSize>
inline bool
TestVertexColorEnabled(const nn::util::Unorm8x4 (&colors)[TSize])
{
    NN_STATIC_ASSERT(4 <= TSize);
    const uint8_t max = std::numeric_limits<uint8_t>::max();
    for (int i = 0; i < TSize; i++)
    {
        if (colors[i].v[0] != max || colors[i].v[1] != max || colors[i].v[2] != max || colors[i].v[3] != max)
        {
            return true;
        }
    }
    return false;
}

//----------------------------------------
template<std::size_t TSize> inline void SetupVertexColor(const DrawInfo& drawInfo, Material& material, const nn::util::Unorm8x4 (&colors)[TSize])
{
    NN_STATIC_ASSERT(4 <= TSize);

    Material::ConstantBufferForVertexShader* pVertexShaderConstantBuffer = material.GetConstantBufferForVertexShader(drawInfo);

    pVertexShaderConstantBuffer->vertexColor[0][0] = static_cast<float>(colors[0].v[0]);
    pVertexShaderConstantBuffer->vertexColor[0][1] = static_cast<float>(colors[0].v[1]);
    pVertexShaderConstantBuffer->vertexColor[0][2] = static_cast<float>(colors[0].v[2]);
    pVertexShaderConstantBuffer->vertexColor[0][3] = static_cast<float>(colors[0].v[3]);
    pVertexShaderConstantBuffer->vertexColor[1][0] = static_cast<float>(colors[1].v[0]);
    pVertexShaderConstantBuffer->vertexColor[1][1] = static_cast<float>(colors[1].v[1]);
    pVertexShaderConstantBuffer->vertexColor[1][2] = static_cast<float>(colors[1].v[2]);
    pVertexShaderConstantBuffer->vertexColor[1][3] = static_cast<float>(colors[1].v[3]);
    pVertexShaderConstantBuffer->vertexColor[2][0] = static_cast<float>(colors[2].v[0]);
    pVertexShaderConstantBuffer->vertexColor[2][1] = static_cast<float>(colors[2].v[1]);
    pVertexShaderConstantBuffer->vertexColor[2][2] = static_cast<float>(colors[2].v[2]);
    pVertexShaderConstantBuffer->vertexColor[2][3] = static_cast<float>(colors[2].v[3]);
    pVertexShaderConstantBuffer->vertexColor[3][0] = static_cast<float>(colors[3].v[0]);
    pVertexShaderConstantBuffer->vertexColor[3][1] = static_cast<float>(colors[3].v[1]);
    pVertexShaderConstantBuffer->vertexColor[3][2] = static_cast<float>(colors[3].v[2]);
    pVertexShaderConstantBuffer->vertexColor[3][3] = static_cast<float>(colors[3].v[3]);
}

//----------------------------------------
inline void SetupFrameSize(const DrawInfo& drawInfo, Material& material, const Size& size, const WindowFrameSize& frameSize)
{
    Material::ConstantBufferForVertexShader* pVertexShaderConstantBuffer = material.GetConstantBufferForVertexShader(drawInfo);
    pVertexShaderConstantBuffer->paneSize[0] = size.width;
    pVertexShaderConstantBuffer->paneSize[1] = size.height;
    pVertexShaderConstantBuffer->frameSize[0] = frameSize.left;
    pVertexShaderConstantBuffer->frameSize[1] = frameSize.right;
    pVertexShaderConstantBuffer->frameSize[2] = frameSize.top;
    pVertexShaderConstantBuffer->frameSize[3] = frameSize.bottom;
}

//----------------------------------------
void
                CalculateQuad(
                    DrawInfo& drawInfo,
                    Material::ConstantBufferForVertexShader* pConstantBufferForVertexShader,
                    const nn::util::Float2& basePt,
                    const Size& size);
//----------------------------------------
void
                SetupMaterialRenderState(
                    nn::gfx::CommandBuffer& commandBuffer,
                    DrawInfo&     drawInfo,
                    Material&     material);

//----------------------------------------
void            DrawQuad(
                    nn::gfx::CommandBuffer& commandBuffer,
                    DrawInfo& drawInfo);

//----------------------------------------
void
                CalculateQuadWithTexCoords(
                    DrawInfo& drawInfo,
                    Material::ConstantBufferForVertexShader* pVertexShaderConstantBuffer,
                    const nn::util::Float2& basePt,
                    const Size& size,
                    int  texCoordCount,
                    const nn::util::Float2 (*texCoords)[PaneVertex_MaxPaneVertex],
                    float offsetU, float offsetV);

//----------------------------------------
void
                CalculateQuadWithTexCoords(
                    DrawInfo& drawInfo,
                    Material::ConstantBufferForVertexShader* pVertexShaderConstantBuffer,
                    const nn::util::Float2& basePt,
                    const Size& size,
                    int  texCoordCount,
                    const nn::util::Float2 (*texCoords)[PaneVertex_MaxPaneVertex]);


//----------------------------------------
void            DrawBox(
                    nn::gfx::CommandBuffer& commandBuffer,
                    DrawInfo&           drawInfo,
                    Material&           material,
                    const nn::util::Float2&   pos,
                    const Size&         size,
                    const nn::util::Unorm8x4& color);

//----------------------------------------
inline
uint8_t
GetVertexColorElement(
    const nn::util::Unorm8x4 cols[],
    int              idx
)
{
    NN_SDK_ASSERT(idx < AnimTargetPaneColor_MaxVertex);

    const int elementSize = 4; // rgba の計4つ
    return cols[idx / elementSize].v[idx % elementSize];
}

//----------------------------------------
inline
void
SetVertexColorElement(
    nn::util::Unorm8x4 cols[],
    int          idx,
    int           value
)
{
    NN_SDK_ASSERT(idx < AnimTargetPaneColor_MaxVertex);

    const int elementSize = 4; // rgba の計4つ
    cols[idx / elementSize].v[idx % elementSize] = static_cast<uint8_t>(value);
}

//----------------------------------------
inline
HorizontalPosition
GetHorizontalPosition(uint8_t  var)
{
    return static_cast<HorizontalPosition>(var & 0x3);
}

//----------------------------------------
inline
VerticalPosition
GetVerticalPosition(uint8_t  var)
{
    return static_cast<VerticalPosition>((var >> 2) & 0x3);
}

//----------------------------------------
inline
void
SetHorizontalPosition(uint8_t * pVar, uint8_t  newVal)
{
    NN_SDK_ASSERT(newVal < HorizontalPosition_MaxHorizontalPosition);

    *pVar = (*pVar & (~0x3)) | newVal;
}

//----------------------------------------
inline
void
SetVerticalPosition(uint8_t * pVar, uint8_t  newVal)
{
    NN_SDK_ASSERT(newVal < VerticalPosition_MaxVerticalPosition);

    *pVar = (*pVar & (~0xc)) | (newVal << 2);
}

} // namespace nn::ui2d::detail
} // namespace nn::ui2d
} // namespace nn
