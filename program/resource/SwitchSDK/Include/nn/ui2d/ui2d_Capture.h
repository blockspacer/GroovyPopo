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

#include <nn/ui2d/ui2d_Util.h>

#include <nn/ui2d/ui2d_Pane.h>

namespace nn
{
namespace ui2d
{

struct ResCapture;
struct BuildResSet;
class DrawInfo;

#if NN_GFX_IS_TARGET_GL || NN_GFX_IS_TARGET_VK
struct TextureCopyInfo
{
    const nn::gfx::Texture*   pSrcTexture;
    const nn::gfx::Texture*   pDstTexture;
    int32_t             srcX0;
    int32_t             srcY0;
    int32_t             srcX1;
    int32_t             srcY1;
    int32_t             dstX0;
    int32_t             dstY0;
    int32_t             dstX1;
    int32_t             dstY1;
};
#endif

//---------------------------------------------------------------------------
//! @brief キャプチャペインです。
//!
//---------------------------------------------------------------------------
class Capture : public Pane
{
    typedef Pane Base;

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(Capture::Base);

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    Capture()
    {}

    //! @brief コンストラクタです。
    //!
    //! @details
    //! size で指定したサイズのペインを生成します。
    //!
    //! @param[in] size ペインのサイズです。
    //!
    explicit Capture(const Size& size)
    {
        SetSize(size);
    }

    //! @brief コンストラクタです。
    //!
    //! @details
    //! pBlock で指定したリソースからペインを生成します。
    //!
    //! @param[in] pBaseBlock       このペインが所属するレイアウトが保持しているリソースへのポインタです。
    //! @param[in] pOverrideBlock   上書きを行ったリソースへのポインタです。上書きが行われていないときはNULLになります。
    //! @param[in] buildArgSet      構築時の引数への参照です。
    //!
    Capture(
        const ResCapture* pBaseBlock,
        const ResCapture* pOverrideBlock,
        const BuildArgSet& buildArgSet);

    //! @brief コピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! @param[in] capture コピー元のペインです。
    //!
    explicit Capture(const Capture& capture);

    //! @brief デストラクタです。
    //!
    virtual ~Capture();

    //@}

    //! @brief コピーコンストラクタでコピーされたインスタンスが正しくコピーされているか確認します。
    //!
    //! @details
    //! このメソッドは ui2d ランタイムのテストのために作成されたメソッドです。
    //! アプリケーションコードでの使用は非推奨となります。
    //!
    //! @param[in]  target  比較対象のペインです。
    //!
    //! @return 同等とみなせる場合は true を返します。
    //!
    bool CompareCopiedInstanceTest(const Capture& target) const;

    //! @brief 描画のための行列を計算します。
    //!
    //! @param[in] drawInfo         DrawInfoです。
    //! @param[in] context          描画情報と内部で使用する変数を含むコンテキストです。
    //! @param[in] isDirtyParentMtx 親の行列がDirtyか否かです。
    //!
    virtual void Calculate(DrawInfo& drawInfo, Pane::CalculateContext& context, bool isDirtyParentMtx) NN_OVERRIDE;
protected:
    virtual void Draw(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer) NN_OVERRIDE;

private:
    // コピー演算子を禁止します。
    const Capture& operator=( const Capture& );
};


//---------------------------------------------------------------------------
//! @brief キャプチャテクスチャクラスです。
//!
//---------------------------------------------------------------------------
class CaptureTexture : public detail::DynamicRenderingTexture
{
    typedef detail::DynamicRenderingTexture Base;
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(CaptureTexture::Base);

public:
    //! @brief コンストラクタです。
    CaptureTexture();

    //! @brief コンストラクタです。
    //!
    //! @param[in]  pName キャプチャテクスチャの名前です。
    //!
    explicit CaptureTexture(const char* pName);

    //! @brief コピーコンストラクタです。
    //!
    //! @param[in] src コピー元のキャプチャテクスチャです。
    //! @param[in] pResAccessor 新しく作成されるキャプチャテクスチャを登録する ResourceAccessor です。
    //! @param[in] pPrefix 新たに作成するキャプチャテクスチャのプリフィックスです。
    //!
    //! @deprecated ui2d ランタイムでキャプチャテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
    //!
    NN_DEPRECATED CaptureTexture(const CaptureTexture& src, ResourceAccessor* pResAccessor, const char* pPrefix);

    //! @brief デストラクタです。
    //!
    ~CaptureTexture();

    //! @brief マテリアルから参照される TextureInfo を登録します。
    //!
    //! @details
    //! ペインツリーの初期化中にマテリアルから参照されるため、他のリソースに先立って初期化します。
    //!
    //! @param[in]  pResAcsr    キャプチャテクスチャが登録されるリソースアクセッサです。
    //! @param[in]  pPaneName   参照するペインの名前です。
    //! @param[in]  pNamePrefix キャプチャリソースの名前修飾のためのプリフィックス文字列です。
    //!
    //! @deprecated ui2d ランタイムでキャプチャテクスチャリソース管理方法が変更されたため、このメソッドは非推奨となりました。
    //!             リソース管理に影響しなくなったためこのメソッドを呼び出す必要はありません。
    //!
    NN_DEPRECATED void RegisterMaterialReferencedTextureInfo(ResourceAccessor* pResAcsr, const char* pPaneName, const char* pNamePrefix);

    //! @brief 初期化処理です。
    //!
    //! @details リソースデータを使用してキャプチャテクスチャを初期化します。
    //!
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  pLayout キャプチャテクスチャで作成されるレンダーターゲットテクスチャが所属するレイアウトです。
    //! @param[in]  pResCaptureTexture  初期化するキャプチャテクスチャのリソースです。
    //! @param[in]  pTargetPane キャプチャテクスチャが参照するペインのポインタです。
    //!
    void Initialize(nn::gfx::Device* pDevice, const Layout* pLayout, const ResCaptureTexture* const pResCaptureTexture, Pane* pTargetPane);

    //! @brief 初期化処理です。
    //!
    //! @details 構築済みの CaptureTexture のインスタンスをコピーして初期化します。
    //!
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  pLayout キャプチャテクスチャで作成されるレンダーターゲットテクスチャが所属するレイアウトです。
    //! @param[in]  src 初期化に使用するキャプチャテクスチャクラスの参照です。
    //! @param[in]  pTargetPane キャプチャテクスチャが参照するペインのポインタです。
    //!
    void Initialize(nn::gfx::Device* pDevice, const Layout* pLayout, const CaptureTexture& src, Pane* pTargetPane);

    //! @brief キャプチャテクスチャの描画処理です。
    //!
    //! @details
    //! キャプチャテクスチャへ内容物を描画してテクスチャを更新します。
    //!
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  drawInfo 描画情報です。
    //! @param[in]  commandBuffer   描画コマンドを設定するコマンドバッファです。
    //!
    virtual void Draw(nn::gfx::Device* pDevice, DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer) NN_OVERRIDE;

    //! @brief 終了処理です。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] pResAcsr リソースアクセッサです。
    //!
    //! @deprecated ui2d ランタイムでキャプチャテクスチャリソース管理方法が変更されたため、このメソッドは非推奨となりました。
    //!
    NN_DEPRECATED void Finalize(nn::gfx::Device* pDevice, ResourceAccessor* pResAcsr);

    //! @brief 終了処理です。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    virtual void Finalize(nn::gfx::Device* pDevice) NN_OVERRIDE;

    //! @brief 初回フレームキャプチャテクスチャの更新済みフラグをリセットします。
    //!
    //! @deprecated ResetFirstFrameCaptureUpdatedFlag() に名称が修正されたため廃止される予定です。
    //!
    NN_DEPRECATED void ResetFirstFrameCaptureUpdatdFlag()
    {
        ResetFirstFrameCaptureUpdatedFlag();
    }

    //! @brief 初回フレームキャプチャテクスチャの更新済みフラグをリセットします。
    //!
    virtual void ResetFirstFrameCaptureUpdatedFlag() NN_OVERRIDE
    {
        detail::SetBit(&m_Flags, Flags_CaptureOnlyFirstFrameUpdated, 0);
    }

    //! @brief 対象ペインのポインタを取得します。
    //!
    //! @return 対象ペインのポインタです。
    //!
    virtual const Pane* GetTargetPane() const NN_OVERRIDE
    {
        return m_pTargetPane;
    }

    //! @brief キャプチャする領域に関する情報を取得します。
    //!
    //! @param[out] pX  キャプチャする X 座標です。
    //! @param[out] pY  キャプチャする Y 座標です。
    //! @param[out] pWidth  キャプチャする幅です。
    //! @param[out] pHeight  キャプチャする高さです。
    //!
    void GetCaptureAreaInfo(float* pX, float* pY, float* pWidth, float* pHeight) const;

    //! @brief キャプチャテクスチャのスケールを取得します。
    //!
    //! @return キャプチャテクスチャのスケールです。
    //!
    float GetTextureScale() const
    {
        return m_Scale;
    }

    //! @brief キャプチャテクスチャのフォーマットを取得します。
    //!
    //! @return キャプチャテクスチャのフォーマットです。
    //!
    nn::gfx::ImageFormat GetTextureFormat() const
    {
        return m_ImageFormat;
    }

    //! @brief キャプチャテクスチャのクリアカラーを取得します。
    //!
    //! @return キャプチャテクスチャのクリアカラーです。
    //!
    const nn::util::Vector4f* GetClearColor() const
    {
        return &m_ClearColor;
    }

    //! @brief このキャプチャテクスチャが初期化済みかどうか判断します。
    //!
    //! @return 初期化済みならば true が返ります。
    //!
    bool IsInitialized() const
    {
        return m_pTargetPane != NULL;
    }

private:

    enum Flags
    {
        Flags_CaptureFrameBuffer,
        Flags_CaptureOnlyFirstFrame,
        Flags_NotNeedPerFrameReconstruct,
        Flags_CaptureOnlyFirstFrameUpdated,
        Flags_Max
    };

    void InitializeParams();
    bool InitializeResources(nn::gfx::Device* pDevice, const Layout* pLayout, RenderTargetTextureLifetime lifetimeHint);
    void CopyFramebufferTexture(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);
    void ApplyFilterToCaptureTexture(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    RenderTargetTextureInfo*    m_pCaptureTexture;
    float                       m_Scale;

    const Layout*               m_pOwnerLayout;

    Pane*                       m_pTargetPane;
    int                         m_FilterCount;
    nn::gfx::ImageFormat        m_ImageFormat;
    nn::util::Vector4f          m_ClearColor;

    uint16_t                    m_Flags;
    NN_PADDING2;
#if NN_GFX_IS_TARGET_GL || NN_GFX_IS_TARGET_VK
    TextureCopyInfo             m_TextureCopyInfo;
#endif
};


} // namespace nn::ui2d
} // namespace nn
