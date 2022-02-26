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

#include <cstddef>

#include <nn/ui2d/ui2d_Util.h>
#include <nn/ui2d/ui2d_Common.h>
#include <nn/ui2d/ui2d_Types.h>

namespace nn
{
namespace ui2d
{

class CaptureTexture;

namespace detail
{

//! @brief  ドロップシャドウのぼかし描画用ピクセルシェーダーコンスタントバッファです。
//!
struct DropShadowBlurConstantBuffer
{
    //! ドロップシャドウのカラーです。RGBA の順に格納されており、アルファは使用されません。
    float   color[4];
    //! ドロップシャドウのスプレッド値です。0.0 - 1.0 の値が設定されます。
    float   spread;
    //! ぼかす際の中心ピクセルのウエイト値です。
    float   centerWeight;
    //! ペインの透明度です。
    float   paneTransparency;
    //! ペインの透明度の逆数です。
    float   inversePaneTransparency;
    //! ウエイトを考慮した横方向のサンプリングオフセットです。
    float   offsetsH[MaxDropShadowBlurRadius / 2];
    //! ウエイトを考慮した縦方向のサンプリングオフセットです。
    float   offsetsV[MaxDropShadowBlurRadius / 2];
    //! ウエイトテーブルです。
    float   weights[MaxDropShadowBlurRadius / 2];
};

//! @brief  ブラー用コンスタントバッファの計算の入力データです。
//!
struct BlurParams
{
    //! 角度です。
    float   angle;
    //! 距離です。
    float   distance;
    //! カラーです。
    float   color[4];
    //! スプレッドです。
    float   spread;
    //! ぼかしのサイズです。
    float   size;
};

//---------------------------------------------------------------------------
//! @brief マスクやドロップシャドウなどのペインエフェクトの処理を行うクラスです。
//!
//---------------------------------------------------------------------------
class PaneEffect
{
    NN_DISALLOW_COPY(PaneEffect);
public:
    // コンストラクタ

    //! @brief 内部用機能のため使用禁止です。
    //!
    PaneEffect();

    // コピーコンストラクタです。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] src コピー元の PaneEffect です。
    //! @param[in] pTargetPane この PaneEffect が適用される Pane です。
    //! @param[in] pDevice デバイスです。
    //! @param[in] pLayout このペインエフェクトを使用しているレイアウトへのポインタです。
    //! @param[in] pBuildPaneTreeContext ペイン間のキャプチャテクスチャの共有など、ペインやパーツ階層をまたいで受け渡す必要のあるデータを保持します。
    //!
    explicit PaneEffect(const PaneEffect& src, Pane* pTargetPane, nn::gfx::Device* pDevice, const Layout* pLayout, detail::BuildPaneTreeContext* pBuildPaneTreeContext);

    //! デストラクタ
    ~PaneEffect();

    // 初期化処理です。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報です。
    //! @param[in] pDevice          デバイスです。
    //! @param[in] pTargetPane この PaneEffect が適用される Pane です。
    //! @param[in] buildArgSet  構築時の引数への参照です。
    //!
    void Initialize(BuildResultInformation* pOutBuildResultInformation, nn::gfx::Device* pDevice, Pane* pTargetPane, const BuildArgSet& buildArgSet);

    // ペインエフェクトに必要なコンスタントバッファのサイズを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details
    //! 初期化済みのペインエフェクトで使用するコンスタントバッファのサイズを計算して取得します。
    //!
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  pMaskData マスクデータです。
    //! @param[in]  pDropShadowData ドロップシャドウデータです。
    //!
    //! @return エフェクトが使用するコンスタントバッファのサイズ。
    //!
    static size_t GetRequiredConstantBufferSize(nn::gfx::Device* pDevice, const SystemDataMaskTexture* pMaskData, const SystemDataDropShadow* pDropShadowData);

    // マスク機能が有効かどうか取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return マスク機能が有効なら true、無効なら false を返します。
    bool IsMaskFunctionEnabled() const
    {
        return m_MaskInfo.pMaskData != NULL;
    }

    // ドロップシャドウ機能が有効かどうか取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return ドロップシャドウ機能が有効なら true、無効なら false を返します。
    bool IsDropShadowFunctionEnabled() const
    {
        return m_DropShadowInfo.pDropShadowData != NULL;
    }

    // ドロップシャドウの境界線が有効化どうか取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return ドロップシャドウの境界線が有効なら true、無効なら false を返します。
    bool IsDropShadowStrokeEnabled() const
    {
        NN_SDK_ASSERT_NOT_NULL(m_DropShadowInfo.pDropShadowData);
        return detail::TestBit(m_DropShadowInfo.pDropShadowData->flags, SystemDataDropShadow::DropShadowFlag_StrokeEnabled);
    }

    // ドロップシャドウの光彩が有効化どうか取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return ドロップシャドウの光彩が有効なら true、無効なら false を返します。
    bool IsDropShadowOuterGlowEnabled() const
    {
        NN_SDK_ASSERT_NOT_NULL(m_DropShadowInfo.pDropShadowData);
        return detail::TestBit(m_DropShadowInfo.pDropShadowData->flags, SystemDataDropShadow::DropShadowFlag_OuterGlowEnabled);
    }

    // ドロップシャドウが有効化どうか取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return ドロップシャドウの光彩が有効なら true、無効なら false を返します。
    bool IsDropShadowEnabled() const
    {
        NN_SDK_ASSERT_NOT_NULL(m_DropShadowInfo.pDropShadowData);
        return detail::TestBit(m_DropShadowInfo.pDropShadowData->flags, SystemDataDropShadow::DropShadowFlag_DropShadowEnabled);
    }

    // ドロップシャドウのエフェクトのみレンダリングが有効化どうか取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return ドロップシャドウのエフェクトのみレンダリングが有効なら true、無効なら false を返します。
    bool IsDropShadowEffectOnlyDrawEnabled() const
    {
        NN_SDK_ASSERT_NOT_NULL(m_DropShadowInfo.pDropShadowData);
        return detail::TestBit(m_DropShadowInfo.pDropShadowData->flags, SystemDataDropShadow::DropShadowFlag_DrawEffectOnlyEnabled);
    }

    // ドロップシャドウのノックアウトが有効化どうか取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return ドロップシャドウのノックアウトが有効なら true、無効なら false を返します。
    bool IsDropShadowKnockoutEnabled() const
    {
        NN_SDK_ASSERT_NOT_NULL(m_DropShadowInfo.pDropShadowData);
        return detail::TestBit(m_DropShadowInfo.pDropShadowData->flags, SystemDataDropShadow::DropShadowFlag_KnockoutEnabled);
    }

    // ドロップシャドウの静的レンダリング機能が有効かどうか取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return ドロップシャドウの静的レンダリング機能が有効なら true、無効なら false を返します。
    //!
    bool IsDropShadowStaticRenderingEnabled() const
    {
        if (IsDropShadowFunctionEnabled())
        {
            return detail::TestBit(m_DropShadowInfo.pDropShadowData->flags, SystemDataDropShadow::DropShadowFlag_StaticRendering);
        }

        return false;
    }

    // ドロップシャドウの静的レンダリングのキャッシュ作成が必要かどうかを返します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return ドロップシャドウの静的レンダリングのキャッシュ作成が必要かどうかを返します。
    //!
    bool IsDropShadowCacheRenderingNeeded() const
    {
        //  静的レンダリング設定が有効でキャッシュがされていない場合に有効になる。
        return IsDropShadowStaticRenderingEnabled() && !m_DropShadowInfo.staticRenderingCached;
    }


    // ドロップシャドウのキャッシュを利用した静的レンダリングが有効かどうかを返します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return キャッシュを利用した静的レンダリングが有効かどうかを返します。
    //!
    bool IsDropShadowStaticRenderingReady() const
    {
        //  静的レンダリング設定が有効でキャッシュ済みの場合に有効になる。
        return IsDropShadowStaticRenderingEnabled() && m_DropShadowInfo.staticRenderingCached;
    }

    // ドロップシャドウ効果のレンダリングキャッシュの内容を更新するフラグをリセットします。

    //! @brief 内部用機能のため使用禁止です。
    //!
    void ResetDropShadowStaticRenderingCacheFlag()
    {
        if (IsDropShadowStaticRenderingEnabled())
        {
            m_DropShadowInfo.staticRenderingCached = false;
        }
    }

    // マスクの静的レンダリング機能が有効かどうか取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return マスクの静的レンダリング機能が有効なら true、無効なら false を返します。
    //!
    bool IsMaskStaticRenderingEnabled() const
    {
        if (IsMaskFunctionEnabled())
        {
            return detail::TestBit(m_MaskInfo.pMaskData->flags, SystemDataMaskTexture::MaskFlag_StaticRenderingEnabled);
        }

        return false;
    }

    // マスクの静的レンダリングのキャッシュ作成が必要かどうかを返します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return マスクの静的レンダリングのキャッシュ作成が必要かどうかを返します。
    //!
    bool IsMaskCacheRenderingNeeded() const
    {
        //  静的レンダリング設定が有効でキャッシュがされていない場合に有効になる。
        return IsMaskStaticRenderingEnabled() && !m_MaskInfo.staticRenderingCached;
    }


    // マスクのキャッシュを利用した静的レンダリングが有効かどうかを返します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return キャッシュを利用した静的レンダリングが有効かどうかを返します。
    //!
    bool IsMaskStaticRenderingReady() const
    {
        //  静的レンダリング設定が有効でキャッシュ済みの場合に有効になる。
        return IsMaskStaticRenderingEnabled() && m_MaskInfo.staticRenderingCached;
    }

    // マスクのレンダリングキャッシュの内容を更新するフラグをリセットします。

    //! @brief 内部用機能のため使用禁止です。
    //!
    void ResetMaskStaticRenderingCacheFlag()
    {
        if (IsMaskStaticRenderingEnabled())
        {
            m_MaskInfo.staticRenderingCached = false;
        }
    }

    //  終了処理です。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pDevice      デバイスです。
    void Finalize(nn::gfx::Device* pDevice);

    // 更新処理です。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  drawInfo 描画情報です。
    //!
    void Calculate(DrawInfo& drawInfo);

    // 描画処理です。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  drawInfo 描画情報です。
    //! @param[in]  commandBuffer コマンドバッファです。
    //!
    void Draw(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    //  ドロップシャドウ機能の影描画を行います。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details
    //! 内部では静的レンダリングが必要かどうかを判定し必要に応じてキャッシュを作成します。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    void DrawDropShadow(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    // マスクを適用した結果を m_MaskInfo.pMaskCaptureRenderTarget へ描画します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    void CaptureMaskedImage(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);
protected:
private:
    // スプレッド計算時の最低分母
    static const float SpreadDenomMin;

    //  デフォルトセットアップされた頂点シェーダーコンスタントバッファへマスク描画用の設定を反映します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pConstantBuffer パラメータを設定するコンスタントバッファへのポインタです。
    //!
    void ApplyMaskParameters(Material::ConstantBufferForVertexShader* pConstantBuffer);

    //  マスク機能の最終描画用にコンスタントバッファを設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] drawInfo 描画情報です。
    //!
    void CalculateMaskConstantBuffer(DrawInfo& drawInfo);

    //  マスク機能用のシェーダーを準備します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    void SetupMaskShader(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    //  マスク機能の最終描画を行います。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    void DrawMaskedImage(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    //  ドロップシャドウのためのコンスタントバッファ用メモリを確保します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] type     コンスタントバッファメモリを確保するドロップシャドウエフェクトのタイプです。
    //!
    void AllocateDropShadowEffectConstantBufferSet(DrawInfo& drawInfo, SystemDataDropShadow::DropShadowType type);

    //  マスクの描画結果をキャプチャするためのコンスタントバッファを更新します。。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] captureMaskDrawCBOffset  マスク描画結果キャプチャのためのコンスタントバッファオフセットです。
    //!
    void CalculateMaskCaptureDrawConstantBuffer(DrawInfo& drawInfo, int captureMaskDrawCBOffset);

    //   BlurParams を作成します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[out] pBlurParams             作成結果を書き込む BlurParams へのポインタです。
    //! @param[in] pDropShadowSystemData    コンスタントバッファメモリを確保するドロップシャドウエフェクトのタイプです。
    //! @param[in] type                     コンスタントバッファメモリを確保するドロップシャドウエフェクトのタイプです。
    //!
    void MakeBlurParams(BlurParams* pBlurParams, const SystemDataDropShadow* pDropShadowSystemData, SystemDataDropShadow::DropShadowType type) const;

    //  ドロップシャドウ機能の最終描画用にコンスタントバッファを設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] drawInfo 描画情報です。
    //!
    void CalculateDropShadowConstantBuffer(DrawInfo& drawInfo);

    //  ViewportStateInfo と ScissorStateInfo を指定のテクスチャサイズ用に準備します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[out] pViewportStateInfo  値を設定する ViewportStateInfo へのポインタです。
    //! @param[out] pScissorStateInfo  値を設定する ScissorStateInfo へのポインタです。
    //! @param[in]  texSize 設定するテクスチャサイズの情報です。
    //!
    void SetupViewportScissorStateInfo(nn::gfx::ViewportStateInfo* pViewportStateInfo, nn::gfx::ScissorStateInfo* pScissorStateInfo, const TexSize& texSize);

    //  テクスチャを描画します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //! @param[in] vertexShaderCBOffset 頂点シェーダー CB オフセットです。
    //! @param[in] textureDescriptor    テクスチャのディスクリプタースロットです。
    //! @param[in] samplerDescriptor    サンプラーのディスクリプタースロットです。
    //!
    void DrawImage(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer, int vertexShaderCBOffset, const nn::gfx::DescriptorSlot& textureDescriptor, const nn::gfx::DescriptorSlot& samplerDescriptor);

    // ペインエフェクト共通で使用するシェーダーを初期化します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  pLayout リソースアクセッサへアクセスするためのレイアウトインスタンスへのポインタです。
    //! @param[in]  pMaskData マスクデータです。
    //! @param[in]  pDropShadowData ドロップシャドウデータです。
    //!
    void InitializeCommonShader(nn::gfx::Device* pDevice, const Layout* pLayout, const SystemDataMaskTexture* pMaskData, const SystemDataDropShadow* pDropShadowData);

    //　ペインエフェクトに使用する中間レンダーターゲットを作成します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  pLayout レンダーターゲットテクスチャを管理するレイアウトのポインタです。レンダーターゲットテクスチャ作成コールバックへ渡されます。
    //! @param[in]  pMaskData マスクデータです。
    //! @param[in]  pDropShadowData ドロップシャドウデータです。
    //!
    void CreatePaneEffectTempTextures(nn::gfx::Device* pDevice, const Layout* pLayout, const SystemDataMaskTexture* pMaskData, const SystemDataDropShadow* pDropShadowData);

    //  横ブラー用の頂点シェーダーコンスタントバッファを作成します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  drawInfo 描画情報です。
    //! @param[in]  cbOffset コンスタントバッファオフセットです。
    //! @param[in]  blurTextureCenter ブラー用テクスチャの中心座標です。
    //! @param[in]  blurTextureSize ブラー用テクスチャのサイズです。
    //! @param[in]  marginOffset ブラー用テクスチャサイズでオリジナルテクスチャのサイズを扱う際の UV オフセットです。
    //!
    void MakeHorizontalBlurConstantBuffer(
        DrawInfo& drawInfo,
        int cbOffset,
        const nn::util::Float2& blurTextureCenter,
        const Size& blurTextureSize,
        const nn::util::Float2& marginOffset) const;

    //  縦ブラー＆最終描画用の頂点シェーダーコンスタントバッファを作成します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  drawInfo 描画情報です。
    //! @param[in]  cbOffset コンスタントバッファオフセットです。
    //! @param[in]  blurParams ブラー情報です。
    //! @param[in]  blurTextureCenter ブラー用テクスチャの中心座標です。
    //! @param[in]  blurTextureSize ブラー用テクスチャのサイズです。
    //! @param[in]  blurTextureScale 元のキャプチャ画像を基準としたブラー用テクスチャの縦横のスケールです。
    //! @param[in]  marginOffset ブラー用テクスチャサイズでオリジナルテクスチャのサイズを扱う際の UV オフセットです。
    //!
    void MakeVerticalBlurConstantBuffer(
        DrawInfo& drawInfo,
        int cbOffset,
        const BlurParams& blurParams,
        const nn::util::Float2& blurTextureCenter,
        const Size& blurTextureSize,
        const nn::util::Float2& blurTextureScale,
        const nn::util::Float2& marginOffset) const;

    // ブラーピクセルシェーダーのコンスタントバッファを作成します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  drawInfo 描画情報です。
    //! @param[in]  cbOffset コンスタントバッファオフセットです。
    //! @param[in]  blurParams ブラーパラメータです。
    //!
    //! @return テーブルからシェーダーの有効タップサイズを返します。
    //!
    int MakeBlurPixelShaderConstantBuffer(DrawInfo& drawInfo, int cbOffset, const detail::BlurParams& blurParams) const;

    //  静的レンダリング用の頂点シェーダーコンスタントバッファを作成します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  drawInfo 描画情報です。
    //! @param[in]  cbOffset コンスタントバッファオフセットです。
    //! @param[in]  blurParams ブラーパラメータです。
    //! @param[in]  blurTextureSize ブラー用テクスチャのサイズです。
    //!
    void MakeStaticRenderingConstantBuffer(DrawInfo& drawInfo, int cbOffset, const BlurParams& blurParams, const Size& blurTextureSize) const;

    // ドロップシャドウ描画用コンスタントバッファを作成します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  drawInfo 描画情報です。
    //! @param[in]  type     コンスタントバッファを作成するドロップシャドウエフェクトタイプです。
    //!
    //! @return ドロップシャドウのぼかしシェーダーバリエーションオフセットを返します。
    //!
    int MakeDropShadowConstantBufferSet(DrawInfo& drawInfo, SystemDataDropShadow::DropShadowType type);

    // 縦ブラー用のシェーダー ID を選択します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details
    //! 縦ブラー用シェーダーは同時にノックアウトや乗算などを適用するバリエーションが存在する為、引数からバリエーション ID を計算します。
    //!
    //! @param[in] blendId 影描画時のブレンド設定です。
    //! @param[in] knockout ノックアウト描画を行うかどうかのフラグです。
    //!
    int CalculateVerticalBlurShaderId(PresetBlendStateId blendId, bool knockout) const;

    // ぼかした影を描画します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //! @param[in] type     コンスタントバッファを作成するドロップシャドウエフェクトタイプです。
    //! @param[in] blendId 影描画時のブレンド設定です。
    //! @param[in] knockout ノックアウト描画を行うかどうかのフラグです。
    //!
    void DrawBluredShadow(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer, SystemDataDropShadow::DropShadowType type, PresetBlendStateId blendId, bool knockout);

    // キャッシュを使用した静的レンダリングを行います。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    void DrawStaticCachedShadow(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    // ペインエフェクト系の描画共通処理です。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //! @param[in] pShaderInfo  描画に使用するシェーダー情報です。
    //! @param[in] variationIndex シェーダーバリエーションのインデックスです。
    //! @param[in] vertexShaderCBOffset 頂点シェーダー CB オフセットです。
    //!
    void DrawCommonImpl(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer, const ShaderInfo* pShaderInfo, int variationIndex, int vertexShaderCBOffset);

    //  ドロップシャドウのブレンドモードからプリセットのブレンド ID に変換します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  mode    ドロップシャドウのブレンドモードを指定します。
    //!
    //! @return 対応するプリセットのブレンド ID を返します。
    PresetBlendStateId ConvertBlendType(detail::DropShadowBlendMode mode) const;

    //  ペインのサイズとブラーテクスチャサイズから描画位置を計算します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  textureSize ブラーテクスチャサイズです。
    //!
    //! @return 調整された描画位置です。
    //!
    nn::util::Float2 CalculateDropShadowEffectVertexPos(const Size& textureSize) const;
private:

    enum DynamicTextureType
    {
        DynamicTextureType_DropShadow,
        DynamicTextureType_MaskSource,
        DynamicTextureType_Mask,

        DynamicTextureType_Max
    };

    //  マスク機能の初期化処理を実行します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報です。
    //! @param[in] pDevice      デバイスです。
    //! @param[in] pMaskData    マスクデータへのポインタです。
    //! @param[in] buildArgSet  構築時の引数への参照です。
    //!
    void InitializeMaskFunction(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        const SystemDataMaskTexture* pMaskData,
        const BuildArgSet& buildArgSet);

    //  ドロップシャドウ機能の初期化処理を実行します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pDevice      デバイスです。
    //! @param[in] pDropShadowData    ドロップシャドウデータへのポインタです。
    //! @param[in] buildArgSet  構築時の引数への参照です。
    //!
    void InitializeDropShadowFunction(
        nn::gfx::Device* pDevice,
        const SystemDataDropShadow* pDropShadowData,
        const BuildArgSet& buildArgSet);

    // TexMap が DynamicRenderingTexture へのポインタを持っている場合は取得します。
    DynamicRenderingTexture* GetPrivateTexturePtr(const TexMap& texMap);
    // TexMap が参照している DynamicRenderingTexture の終了処理を行います。
    void FinalizeTexMapDynamicRenderingTexture(nn::gfx::Device* pDevice, const TexMap& texMap);
    // ペインエフェクトがオーナーとなっている動的レンダリングテクスチャの終了処理を行います。
    void FinalizeDynamicRenderingTexture(nn::gfx::Device* pDevice);

    // 指定したテクスチャインデックスのテクスチャが参照しているキャプチャテクスチャを新規作成し、ペインとテクスチャ共有情報へ登録します。
    CaptureTexture* CreateCaptureTexture(const BuildArgSet& buildArgSet, int refTextureIndex) const;

    //! ペインエフェクトが設定されているペイン
    const Pane* m_pTargetPane;

    struct MaskInfo
    {
        //! マスクデータ
        const SystemDataMaskTexture*    pMaskData;
        //! マスク用に設定されたテクスチャの情報
        TexMap  maskTexMap;
        //! マスクするテクスチャの情報
        TexMap  maskCaptureTexMap;
        //! 頂点シェーダーのコンスタントバッファオフセット
        uint32_t    vertexShaderConstantBufferOffset;
        //! マスクで使用するシェーダー情報
        const nn::ui2d::ShaderInfo* pShaderInfo;
        //! マスク用シェーダーバリエーションインデックス
        uint32_t    shaderVariationIndex;

        //! マスクの描画結果キャプチャ用のバッファ
        RenderTargetTextureInfo*    pMaskCaptureRenderTarget;
        //! マスク描画をキャプチャするための描画用コンスタントバッファ
        uint32_t    captureMaskDrawCB;

        //! キャッシュレンダリング時のキャッシュ済みフラグ
        bool        staticRenderingCached;
    };

    struct DropShadowInfo
    {
        //! ドロップシャドウデータ
        const SystemDataDropShadow*    pDropShadowData;

        //! ドロップシャドウ生成元のキャプチャテクスチャ
        TexMap  captureTexMap;

        //! ドロップシャドウ用のブラーバッファ
        RenderTargetTextureInfo*    pBlurRenderTarget;
        //! ドロップシャドウの描画結果キャッシュテクスチャ
        RenderTargetTextureInfo*    pStaticRenderingTextures[SystemDataDropShadow::DropShadowType_Max];

        //! キャッシュレンダリング時のキャッシュ済みフラグ
        bool        staticRenderingCached;

        //! ブラー用描画のコンスタントバッファオフセット
        uint32_t    horizontalBlurVertexShaderCB[SystemDataDropShadow::DropShadowType_Max];
        //! 影最終描画用の頂点シェーダーコンスタントバッファオフセット
        uint32_t    verticalBlurAndShadowVertexShaderCB[SystemDataDropShadow::DropShadowType_Max];
        //! キャッシュテクスチャ描画用頂点シェーダーコンスタントバッファオフセット
        uint32_t    staticRenderingVertexShaderCB[SystemDataDropShadow::DropShadowType_Max];
        //! ブラー描画用ピクセルシェーダーコンスタントバッファ
        uint32_t    blurPixelShaderCB[SystemDataDropShadow::DropShadowType_Max];
        //! ぼかしシェーダーのシェーダーバリエーションオフセット
        int         blurShaderVariationOffset[SystemDataDropShadow::DropShadowType_Max];

        //! 本体最終描画用の頂点シェーダーコンスタントバッファオフセット
        uint32_t    constantBufferForFinalDraw;

        //! ドロップシャドウで使用するシェーダー情報
        const nn::ui2d::ShaderInfo* pShaderInfo;
        //! ドロップシャドウで使用するシェーダーのバリエーション情報
        uint32_t    dropShadowShaderVariationIndex[detail::DropShadowShaderVariation_Max];
        //! ドロップシャドウで使用するぼかしピクセルシェーダーのコンスタントバッファスロット
        int         dropShadowBlurPixelShaderCB[detail::DropShadowShaderVariation_Max];
    };

    //! マスク情報
    MaskInfo    m_MaskInfo;
    //! ドロップシャドウ情報
    DropShadowInfo  m_DropShadowInfo;

    //! 静的レンダリングなどで使用するコピーシェーダーの ShaderInfo
    const nn::ui2d::ShaderInfo* m_pCopyShaderInfo;
    //! コピーシェーダーのバリエーションオフセット
    int m_CopyShaderVariationOffset;
    //! コピーシェーダーのテクスチャスロット
    int m_CopyShaderTextureSlot;

    //! マスク用テクスチャのオーナーとなっているかどうか。
    bool    m_MaskTextureOwner;
    //! padding
    uint8_t m_Padding[3];
};

} // namespace nn::ui2d::detail
} // namespace nn::ui2d
} // namespace nn
