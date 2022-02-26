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

#include <nn/ui2d/ui2d_Types.h>
#include <nn/ui2d/ui2d_Layout.h>
#include <nn/ui2d/ui2d_DrawInfo.h>
#include <nn/ui2d/ui2d_Resources.h>

namespace nn
{
namespace ui2d
{

namespace detail
{

/*!
    @brief ベクターグラフィックスシーン描画中にステンシルバッファをクリアする際に呼び出されるコールバックです。

    @details
    この関数ポインタが VectorGraphicsDrawInfo に設定されている場合、ステンシルバッファクリア処理の代わりにこの関数が呼び出されます。
    呼び出し後はステンシルバッファが 0 でクリアされていることを期待しています。

    @param[in]   drawInfo   描画に使用している DrawInfo クラス。
    @param[in]   commandBuffer  コマンドバッファです。
    @param[in]   pUserData  nn::ui2d::VectorGraphicsDrawInfo::SetClearDepthStencilCallback で設定したユーザーデータのポインタ。
*/
typedef void (*VectorGraphicsTextureClearDepthStencilCallback)(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer, void* pUserData);

//---------------------------------------------------------------------------
//! @brief  Vector Graphics シーンです。
//---------------------------------------------------------------------------
class VectorGraphicsScene
{
public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief  コンストラクタです。
    //!
    VectorGraphicsScene();

    //! @brief  コピーコンストラクタです。
    //!
    //! @details
    //! このコピーコンストラクタを使用した場合はマスクテクスチャが初期化されません。
    //! マスクテクスチャを使用した VectorGraphicsScene を正しく初期化するためにはレイアウトクラスのポインタを受け取るコピーコンストラクタを使用してください。
    //!
    //! @param[in]  src コピー元の VectorGraphicsScene です。
    //! @param[in]  pDevice デバイスです。
    //!
    VectorGraphicsScene(const VectorGraphicsScene& src, nn::gfx::Device* pDevice);

    //! @brief  コピーコンストラクタです。
    //!
    //! @param[in]  src コピー元の VectorGraphicsScene です。
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  pLayout レイアウトインスタンスのポインタです。
    //!
    VectorGraphicsScene(const VectorGraphicsScene& src, nn::gfx::Device* pDevice, const Layout* pLayout);

    //! @brief  デストラクタです。
    //!
    ~VectorGraphicsScene();

    //@}

    //! @brief  バイナリデータセットアップ処理。
    //!
    //! @details
    //! マスク対応に伴い内部で動的レンダリングテクスチャを作成するようになったため Build 時に pLayout が必要となりました。
    //! マスクを適用するにはレイアウトインスタンスを受け取る Build() を呼び出してください。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  pResAcsr リソースにアクセスするためのリソースアクセッサです。
    //! @param[in]  pBnvgBinary bnvg バイナリデータへのポインタです。
    //!
    //! @return データが正常に初期化されたかどうかを返します。
    //!
    bool Build(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        ResourceAccessor* pResAcsr,
        const BnvgFileHeader* pBnvgBinary);

    //! @brief  バイナリデータセットアップ処理。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  pLayout レイアウトです。
    //! @param[in]  pResAcsr リソースにアクセスするためのリソースアクセッサです。
    //! @param[in]  pBnvgBinary bnvg バイナリデータへのポインタです。
    //!
    //! @return データが正常に初期化されたかどうかを返します。
    //!
    bool Build(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        const Layout* pLayout,
        ResourceAccessor* pResAcsr,
        const BnvgFileHeader* pBnvgBinary);

    //! @brief  終了処理です。
    //!
    //! @details
    //! マスク対応に伴い内部で動的レンダリングテクスチャを作成するようになったため終了処理時にデバイスのポインタが必要となりました。
    //! レイアウトインスタンスを指定する Build() を呼び出した場合は Finalize() でデバイスのポインタを渡すようにしてください。
    //!
    void Finalize();

    //! @brief  終了処理です。
    //!
    //! @param[in]  pDevice デバイスです。
    //!
    void Finalize(nn::gfx::Device* pDevice);

    //! @brief  更新処理です。
    //!
    //! @param[in]  drawInfo    描画情報です。
    //! @param[in]  time        時刻です。
    //!
    void Calculate(DrawInfo& drawInfo, float time);

    //! @brief  描画処理です。
    //!
    //! @param[in]  drawInfo    描画情報です。
    //! @param[in]  commandBuffer   コマンドバッファです。
    //!
    void Draw(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    //! @brief  Bnvg のバイナリポインタを取得します。
    //!
    //! @return Bnvg バイナリのポインタです。
    //!
    const BnvgFileHeader* GetBinaryData() const
    {
        return m_pFileHeader;
    }

    //! @brief  Bnvg のヘッダを取得します。
    //!
    //! @return Bnvg のヘッダ情報です。
    //!
    const ResBnvgInfoHeader& GetVectorGraphicsHeader() const
    {
        NN_SDK_ASSERT_NOT_NULL(m_pFileHeader);

        return m_pFileHeader->vgHeader;
    }

    //! @brief ベクターグラフィックステクスチャ描画環境設定コールバック情報を設定します。
    //!
    //! @param[in]  pClearStencilCallback  ステンシルバッファクリア時コールバックです。
    //! @param[in]  pUserData       ユーザーデータです。
    //!
    void SetClearDepthStencilCallback(
        VectorGraphicsTextureClearDepthStencilCallback   pClearStencilCallback,
        void*                       pUserData)
    {
        NN_UNUSED(pClearStencilCallback);
        NN_UNUSED(pUserData);
    }
private:

    const BnvgFileHeader*   m_pFileHeader;
};

//------------------------------------------------------------------------------------------------------------
//! @brief  Vector Graphics シーンをテクスチャ化して使用するためのインターフェイスを定義するクラスです。
//------------------------------------------------------------------------------------------------------------
class VectorGraphicsTexture : public DynamicRenderingTexture
{
    typedef DynamicRenderingTexture Base;
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(VectorGraphicsTexture::Base);

public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief  コンストラクタです。
    //!
    explicit VectorGraphicsTexture(const char* pTextureName);

    //! @brief  コンストラクタです。
    //!
    VectorGraphicsTexture(const VectorGraphicsTexture& src, nn::gfx::Device* pDevice, const Layout* pLayout);

    //! @brief  デストラクタです。
    //!
    virtual ~VectorGraphicsTexture();

    //@}

    //! @brief  ベクターグラフィックステクスチャ初期化処理。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  pResAcsr テクスチャを登録するリソースアクセッサです。
    //! @param[in]  pLayout レイアウトポインタです。
    //! @param[in]  pResVgTexture ベクターグラフィックステクスチャの初期化情報です。
    //! @param[in]  pBnvgFileName bnvg のファイル名です。
    //! @param[in]  pBnvgBinary bnvg バイナリデータへのポインタです。リロケート済みのバイナリが入力されることを期待しています。
    //!
    //! @return データが正常に初期化されたかどうかを返します。
    //!
    bool Initialize(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        ResourceAccessor* pResAcsr,
        const Layout* pLayout,
        const ResVectorGraphicsTexture* pResVgTexture,
        const char* pBnvgFileName,
        const BnvgFileHeader* pBnvgBinary);

    //! @brief  終了処理です。
    //!
    //! @param[in]  pDevice デバイスです。
    //!
    virtual void Finalize(nn::gfx::Device* pDevice) NN_OVERRIDE;

    //! @brief  更新処理です。
    //!
    //! @param[in]  drawInfo    描画情報です。
    //!
    virtual void Calculate(DrawInfo& drawInfo) NN_OVERRIDE;

    //! @brief  このテクスチャへベクターグラフィックスデータのシーンをレンダリングします。
    //!
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  drawInfo    描画情報です。
    //! @param[in]  commandBuffer   コマンドバッファです。
    //!
    virtual void Draw(nn::gfx::Device* pDevice, DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer) NN_OVERRIDE;

    //! @brief  構築されたシーンを取得します。
    //!
    //! @return シーンへの参照です。
    //!
    const VectorGraphicsScene& GetScene() const
    {
        return m_Scene;
    }

    //! @brief このテクスチャの MSAA のサンプル数を取得します。
    //!
    //! @return MSAA のサンプル数です。
    //!
    int GetMsaaSampleCount() const
    {
        return m_MsaaSamples;
    }

    //! @brief BodyMovin シーンの評価フレームを指定します。
    //!
    //! @param[in]  time 評価フレーム数です。
    //!
    void SetTime(float time)
    {
        if (m_Time != time)
        {
            m_Time = time;

            // 時間が更新された時のみテクスチャを更新する
            m_RenderSceneRequired = true;
        }
    }

    //! @brief ベクターグラフィックステクスチャのクリアカラーを指定します。
    //!
    //! @param[in] clearColor クリアカラーです。
    //!
    void SetClearColor(const nn::util::Float4& clearColor)
    {
        m_ClearColor = clearColor;
    }

public:
    //! @brief  ベクターグラフィックステクスチャの内部参照名を作成します。
    //!
    //! @details
    //! ベクターグラフィックステクスチャでは同じベクターグラフィックステクスチャを複数の場所で使用したときに
    //! それぞれ状態が異なる可能性があるためテクスチャのインスタンス自体も別々に分ける必要があります。
    //! レイアウトデータのバイナリコンバート時に重複しない名称を設定しているため、リソースからその名前を復元します。
    //!
    //! @param[out] pResult 出力バッファです。resultSize 分のサイズを持っているとみなします。
    //! @param[in] resultSize pResult の最大サイズです。
    //! @param[in] pBnvgFileName 参照している bnvg ファイル名です。
    //! @param[in] pRes ベクタグラフィックステクスチャリソースです。
    //!
    static void MakeRefTextureName(char* pResult, int resultSize, const char* pBnvgFileName, const ResVectorGraphicsTexture* pRes);

private:
    // 最終出力先の RenderTargetTextureInfo を作成します。
    RenderTargetTextureInfo* CreateRenderTargetTexture();
    // 初期化時のテクスチャリソース作成処理です。
    void InitializeResources(nn::gfx::Device* pDevice, const Layout* pLayout);
    // 終了処理時のテクスチャ等リソース破棄処理です。
    void FinalizeResources(nn::gfx::Device* pDevice);
    // ベクターグラフィックスシーンの描画開始前処理です。
    void BeginRendering(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);
    // ベクターグラフィックスシーンの描画終了後処理です。
    void EndRendering(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    // マルチサンプルタイプからマルチサンプルカウント数へ変換します。
    int ConvertMultiSampleTypeToMultiSampleCount(int type) const;

    VectorGraphicsScene         m_Scene;

    nn::util::Float4            m_ClearColor;

    float                       m_Time;
    int                         m_MsaaSamples;
    bool                        m_RenderSceneRequired;

    nn::gfx::Texture*           m_pDepthStencilTexture;
    nn::gfx::TextureView*       m_pDepthStencilTextureView;
    nn::gfx::DescriptorSlot*    m_pDepthStencilTextureSlot;
    nn::gfx::DepthStencilView   m_DepthStencilView;

    RenderTargetTextureInfo*    m_pMsaaRenderTargetTexture;
    nn::gfx::RasterizerState*   m_pMsaaRasterizerState;
};

} // namespace nn::ui2d::detail

} // namespace nn::ui2d
} // namespace nn
