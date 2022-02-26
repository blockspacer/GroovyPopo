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

#include <nn/util/util_MathTypes.h>

#include <nn/font/font_WideTextWriter.h>
#include <nn/font/font_GpuBuffer.h>

#include <nn/ui2d/ui2d_Util.h>
#include <nn/ui2d/ui2d_Types.h>

namespace nn
{
namespace ui2d
{

class GraphicsResource;
class Layout;
class ShaderInfo;

/*!
    @brief ユーザーシェーダーのコンスタントバッファ等を更新するためのコールバック関数の型です。

    @details
    ユーザーシェーダーに割り当てられたコンスタントバッファのメモリ領域は
    このコールバック関数内でのみ書き換えることが出来ます。
    コールバック関数外でアクセスした場合、不正な領域にアクセスする可能性があります。

    @param[in]   drawInfo   描画に使用している DrawInfo クラス。
    @param[in]   pPane  更新対象のペイン。
    @param[in]   pUserData  nn::ui2d::DrawInfo::SetUpdateUserShaderCallback で設定したユーザーデータのポインタ。
*/
typedef void (*UpdateUserShaderCallback)(const DrawInfo& drawInfo, Pane* pPane, void* pUserData);

/*!
    @brief  キャプチャペインでキャプチャした結果へユーザーが任意のシェーダーを適用するためのコールバックです。

    @param[in]  commandBuffer   コマンドバッファ。
    @param[in]  pColorTarget    描画先の ColorTarget です。
    @param[in]  destTexture     描画先テクスチャのディスクリプタースロットです。
    @param[in]  srcTexture      キャプチャ結果テクスチャのディスクリプタースロットです。
    @param[in]  viewportScissorState    ビューポートシザーステートです。
    @param[in]  pPane           キャプチャを行ったペイン。
    @param[in]  pUserData       nn::ui2d::DrawInfo::SetApplyCaptureTextureFilterCallback で設定したユーザーデータのポインタ。

    @return コールバックの中で描画が行われたかどうかを返します。描画が行われなかった場合はキャプチャ画像がコピーされます。
*/
typedef bool (*ApplyCaptureTextureFilterCallback)(nn::gfx::CommandBuffer& commandBuffer, const nn::gfx::ColorTargetView* pColorTarget, const nn::gfx::DescriptorSlot& destTexture, const nn::gfx::DescriptorSlot& srcTexture, const nn::gfx::ViewportScissorState& viewportScissorState, Pane* pPane, void* pUserData);


//---------------------------------------------------------------------------
//! @brief 描画情報クラスです。
//!
//! @details
//! 全体的な描画方法の制御や、描画中の情報の受け渡しに使用されます。
//!
//! 描画の前に SetGraphicsResource() により GraphicsResource を
//! 設定しておく必要があります。
//!
//! 処理中に、内容が更新されるので、複数スレッドでインスタンスを共有することができません。スレッド毎に複製を作成して利用してください。
//! @sa Layout
//!
//---------------------------------------------------------------------------
class DrawInfo
{

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO_ROOT();

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    DrawInfo();

    //! @brief デストラクタです。
    //!
    virtual ~DrawInfo();

    //! @brief コピーコンストラクタです。
    //!
    //! @param[in] src    コピー元です。
    DrawInfo(const DrawInfo& src)
    {
        Set(src);
    }

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief DrawInfo の代入演算子です。
    //!
    //! @param[in] src    代入元です。
    //! @return *this を返します。
    //!
    DrawInfo& operator=(const DrawInfo& src)
    {
        Set(src);
        return *this;
    }

    //! @brief DrawInfo を設定します。
    //!
    //! @param[in] src    設定元です。
    //!
    void Set(const DrawInfo& src)
    {
        m_ProjMtx = src.m_ProjMtx;
        m_ViewMtx = src.m_ViewMtx;
        m_ModelViewMtx = src.m_ModelViewMtx;
        m_LocationAdjustScale = src.m_LocationAdjustScale;

        m_pGraphicsResource = src.m_pGraphicsResource;
        m_pLayout = src.m_pLayout;
        m_pUi2dConstantBuffer = src.m_pUi2dConstantBuffer;
        m_pFontConstantBuffer = src.m_pFontConstantBuffer;
        m_pFramebufferTexture = src.m_pFramebufferTexture;
        m_FramebufferWidth = src.m_FramebufferWidth;
        m_FramebufferHeight = src.m_FramebufferHeight;
        m_pFramebufferTextureDescriptorSlot = src.m_pFramebufferTextureDescriptorSlot;
        m_pFramebufferSamplerDescriptorSlot = src.m_pFramebufferSamplerDescriptorSlot;

        m_pDefaultColorTargetView = src.m_pDefaultColorTargetView;
        m_DefaultViewportInfo = src.m_DefaultViewportInfo;
        m_DefaultScissorInfo = src.m_DefaultScissorInfo;

        m_pDefaultDepthStencilView = src.m_pDefaultDepthStencilView;
        m_pDefaultDepthStencilState = src.m_pDefaultDepthStencilState;

        m_pDefaultRasterizerState = src.m_pDefaultRasterizerState;

        m_pUpdateUserShaderCallback = src.m_pUpdateUserShaderCallback;
        m_pUpdateUserShaderCallbackUserData = src.m_pUpdateUserShaderCallbackUserData;

        m_pApplyCaptureTextureFilterCallback = src.m_pApplyCaptureTextureFilterCallback;
        m_pApplyCaptureTextureFilterCallbackUserData = src.m_pApplyCaptureTextureFilterCallbackUserData;

        memcpy(m_TexCoordSrc, src.m_TexCoordSrc, sizeof(m_TexCoordSrc));

        m_ModelViewMtxLoaded = src.m_ModelViewMtxLoaded;
        m_FirstDraw = src.m_FirstDraw;
        m_AllowableTextureQuantity = src.m_AllowableTextureQuantity;

        m_Flag = src.m_Flag;
        m_pCurrentShaderInfo = src.m_pCurrentShaderInfo;
        m_CurrentShaderVariation = src.m_CurrentShaderVariation;
    }

    //! @brief 描画前の DrawInfo の構成を行います。
    //!
    //! @details
    //! nn::ui2d::Layout::Draw を行う場合は内部的に呼ばれるため明示的に呼ぶ必要はありません。
    //! nn::ui2d::Pane::Draw を直接実行する場合には直前に呼び出してください。
    //!
    //! @param[in] pLayout 描画に使用するレイアウトです。
    //!
    void ConfigureBeforeDrawing(Layout* pLayout);

    //! @brief 描画後の DrawInfo の構成を行います。
    //!
    //! @details
    //! nn::ui2d::Layout::Draw を行う場合は内部的に呼ばれるため明示的に呼ぶ必要はありません。
    //! nn::ui2d::Pane::Draw を直接実行する場合には直後に呼び出してください。
    //!
    void ConfigureAfterDrawing();

    //! @brief 現在のビュー行列を取得します。
    //!
    //! @return ビュー行列を返します。
    //!
    //! @sa SetViewMtx
    //!
    const nn::util::MatrixT4x3fType&  GetViewMtx() const
    {
        return m_ViewMtx;
    }

    //! @brief ビュー行列を設定します。
    //!
    //! @param[in] value    ビュー行列です。
    //!
    //! @sa GetViewMtx
    //!
    void SetViewMtx(const nn::util::MatrixT4x3fType& value)
    {
        m_ViewMtx = value;
    }

    //! @brief 位置調整処理を行うかどうかの設定を取得します。
    //!
    //! @return 位置調整処理を行う場合は true を返します。
    //!
    //! @sa SetLocationAdjustEnabled
    //!
    bool IsLocationAdjustEnabled() const
    {
        return m_Flag.isLocationAdjustEnabled;
    }

    //! @brief 位置調整処理を行うかどうかを設定します。
    //!
    //! @param[in] bEnabled  位置調整処理を行う場合には true を指定します。
    //!
    //! @details
    //! 位置調整処理を有効にした場合、Pane::IsLocationAdjust() が true を返すペインに位置調整処理のスケールが適用されます。
    //!
    //! 該当ペインの子孫ペインも位置調整処理のスケールの影響を受けます。
    //!
    //! @sa IsLocationAdjustEnabled
    //! @sa SetLocationAdjustScale
    //! @sa Pane::IsLocationAdjustEnabled
    //!
    void SetLocationAdjustEnabled(bool bEnabled)
    {
        m_Flag.isLocationAdjustEnabled = bEnabled;
    }

    //! @brief 位置調整処理のスケールを取得します。
    //!
    //! @return 位置調整処理のスケールを返します。
    //!
    //! @sa SetLocationAdjustScale
    //!
    const nn::util::Float2& GetLocationAdjustScale() const
    {
        return m_LocationAdjustScale;
    }

    //! @brief 位置調整処理のスケールを設定します。
    //!
    //! @param[in] scale    スケールです。
    //!
    //! @sa GetLocationAdjustScale
    //!
    void SetLocationAdjustScale(const nn::util::Float2& scale)
    {
        m_LocationAdjustScale = scale;
    }

    //! @brief 非表示のペインに対して行列計算を行うかどうかを判定します。
    //!
    //! @return 非表示のペインに対して行列計算を行う場合は true を返します。
    //!
    //! @sa SetInvisiblePaneCalculated
    //!
    bool IsInvisiblePaneCalculated() const
    {
        return m_Flag.invisiblePaneCalculateMtx;
    }

    //! @brief 非表示のペインに対して行列計算を行うかどうかを設定します。
    //!
    //! @param[in] bEnabled  非表示のペインに対して行列計算を行う場合には true を指定します。
    //!
    //! @details
    //! bEnabled に true を指定した場合、非表示のペインに対しても Pane::Calculate() を
    //! 呼び出して行列計算を行います。
    //!
    //! @sa IsInvisiblePaneCalculated
    //! @sa Pane::Calculate
    //! @sa Pane::IsVisible
    //!
    void SetInvisiblePaneCalculated(bool bEnabled)
    {
        m_Flag.invisiblePaneCalculateMtx = bEnabled;
    }

    //! @brief アルファがゼロのペインに対して行列計算を行うかどうかを判定します。
    //!
    //! @return アルファがゼロのペインに対して行列計算を行う場合は true を返します。
    //!
    //! @sa SetInvisiblePaneCalculated
    //!
    bool IsAlphaZeroPaneCalculated() const
    {
        return m_Flag.alphaZeroPaneCalculateMtx;
    }

    //! @brief アルファがゼロのペインに対して行列計算を行うかどうかを設定します。
    //!
    //! @param[in] bEnabled  アルファがゼロのペインに対して行列計算を行う場合には true を指定します。
    //!
    //! @details
    //! bEnabled に true を指定した場合、アルファがゼロのペインに対しても Pane::Calculate() を
    //! 呼び出して行列計算を行います。
    //!
    //! @sa IsInvisiblePaneCalculated
    //! @sa Pane::Calculate
    //! @sa Pane::IsVisible
    //!
    void SetAlphaZeroPaneCalculated(bool bEnabled)
    {
        m_Flag.alphaZeroPaneCalculateMtx = bEnabled;
    }

    //! @brief ウインドウ原点が左上として内部計算を行うかどうかを取得します。
    //!
    //! @return 左上を原点として扱う場合は true を返します。
    //!
    //! @sa SetLeftTopWindowOrigin
    //!
    bool IsLeftTopWindowOrigin() const
    {
        return m_Flag.isLeftTopWindowOrigin;
    }

    //! @brief ウインドウ原点が左上として内部計算を行うかどうかを設定します。
    //!
    //! @details
    //! ウインドウ原点の変更に対応していない環境では何も行いません。
    //!
    //! @param[in] bEnabled  左上を原点として扱う場合には true を指定します。
    //!
    //! @sa IsLefTopWindowOrigin
    //!
    void SetLeftTopWindowOrigin(bool bEnabled)
    {
        m_Flag.isLeftTopWindowOrigin = bEnabled;
    }

    //! @brief %GraphicsResource オブジェクトを取得します。
    //!
    //! @return GraphicsResource オブジェクトへのポインタを返します。
    //!
    //! @sa SetGraphicsResource
    //!
    GraphicsResource* GetGraphicsResource() const
    {
        return m_pGraphicsResource;
    }

    //! @brief %GraphicsResource オブジェクトを設定します。
    //!
    //! @param[in] pGraphicsResource    GraphicsResource オブジェクトへのポインタです。
    //!
    //! @sa GetGraphicsResource
    //!
    void SetGraphicsResource(GraphicsResource* pGraphicsResource)
    {
        m_pGraphicsResource = pGraphicsResource;
    }

    //! @brief 射影行列を設定します。
    //!
    //! @param[in] mtx  射影行列です。
    //!
    void SetProjectionMtx(const nn::util::MatrixT4x4fType& mtx);

    //! @brief ui2d が描画に使用するコンスタントバッファを設定します。
    //!
    //! @param[in] pConstantBuffer ui2d が使用するコンスタントバッファへのポインタです。
    //!
    //! @details
    //! Layout::Build 時に取得できる BuildResultInformation::requiredUi2dConstantBufferSize 以上のサイズを確保した GpuBuffer を指定してください。
    //! また、ピクチャペインの特殊形状で「四角形」以外を設定されているデータを表示する場合
    //! コンスタントバッファとして渡された GpuBuffer 内の領域にメッシュ表示のための頂点バッファとインデックスバッファを確保しますので
    //! GpuAccessFlag へ nn::gfx::GpuAccess_ConstantBuffer に加えて nn::gfx::GpuAccess_VertexBuffer と nn::gfx::GpuAccess_IndexBuffer を設定してください。
    //!
    void SetUi2dConstantBuffer(nn::font::GpuBuffer* pConstantBuffer)
    {
        m_pUi2dConstantBuffer = pConstantBuffer;
    }

    //! @brief 現在設定されている ui2d が使用するコンスタントバッファを取得します。
    //!
    //! @return ui2d が使用しているコンスタントバッファへのポインタです。
    //!
    nn::font::GpuBuffer* GetUi2dConstantBuffer() const
    {
        return m_pUi2dConstantBuffer;
    }

    //! @brief フォントの描画に使用するコンスタントバッファを設定します。
    //!
    //! @param[in] pGpuBuffer コンスタントバッファへのポインタです。
    //!
    void SetFontConstantBuffer(nn::font::GpuBuffer* pGpuBuffer)
    {
        m_pFontConstantBuffer = pGpuBuffer;
    }

    //! @brief フォントの描画に使用するコンスタントバッファを取得します。
    //!
    //! @return コンスタントバッファへのポインタです。
    //!
    nn::font::GpuBuffer* GetFontConstantBuffer() const
    {
        return m_pFontConstantBuffer;
    }

    //! @brief ユーザーシェーダー更新のためのコールバック関数を設定します。
    //!
    //! @param[in] pCallback    ユーザーシェーダーを更新するためのコールバック関数です。
    //! @param[in] pUserData    コールバック関数へ渡すユーザーデータのポインタです。
    //!
    void SetUpdateUserShaderCallback(UpdateUserShaderCallback pCallback, void* pUserData)
    {
        m_pUpdateUserShaderCallback = pCallback;
        m_pUpdateUserShaderCallbackUserData = pUserData;
    }

    //! @brief ユーザーシェーダー更新のためのコールバック関数を取得します。
    //!
    //! @return ユーザーシェーダーを更新するためのコールバック関数です。
    //!
    UpdateUserShaderCallback GetUpdateUserShaderCallback() const
    {
        return m_pUpdateUserShaderCallback;
    }

    //! @brief ユーザーシェーダー更新のためのコールバック関数へ渡すユーザーデータのポインタを取得します。
    //!
    //! @return コールバック関数へ渡すユーザーデータのポインタ。
    //!
    void* GetUpdateUserShaderCallbackUserData() const
    {
        return m_pUpdateUserShaderCallbackUserData;
    }

    //! @brief キャプチャペインでキャプチャした結果へユーザーが任意のフィルターを適用するためのコールバック関数を設定します。
    //!
    //! @param[in] pCallback    キャプチャペインでキャプチャした結果へフィルターを適用するためのコールバック関数です。
    //! @param[in] pUserData    コールバック関数へ渡すユーザーデータのポインタです。
    //!
    void SetApplyCaptureTextureFilterCallback(ApplyCaptureTextureFilterCallback pCallback, void* pUserData)
    {
        m_pApplyCaptureTextureFilterCallback = pCallback;
        m_pApplyCaptureTextureFilterCallbackUserData = pUserData;
    }

    //! @brief キャプチャペインでキャプチャした結果へユーザーが任意のフィルターを適用するためのコールバック関数を取得します。
    //!
    //! @return フィルターを適用するためのコールバック関数です。
    //!
    ApplyCaptureTextureFilterCallback GetApplyCaptureTextureFilterCallback() const
    {
        return m_pApplyCaptureTextureFilterCallback;
    }

    //! @brief キャプチャペインでキャプチャした結果へユーザーが任意のフィルターを適用するためのコールバック関数へ渡すユーザーデータのポインタを取得します。
    //!
    //! @return コールバック関数へ渡すユーザーデータのポインタ。
    //!
    void* GetApplyCaptureTextureFilterCallbackUserData() const
    {
        return m_pApplyCaptureTextureFilterCallbackUserData;
    }


    //! @brief 描画状態の情報をリセットします。
    //!
    //! @details
    //! ペインツリーを走査して描画する際に、設定回数を削減するために描画状態を保持しています。
    //! もし各ペインの描画の間にアプリ側で別の描画を行った場合には、描画状態に不整合が発生して
    //! 描画結果がおかしくなりますので、ペインの描画に戻る前に本関数を呼んで
    //! 描画状態の情報をリセットしてください。
    //!
    void ResetDrawState();

    //@}

    //! @brief レイアウトデータの描画に使用する各種バッファをマップして操作可能な状態にします。
    //!
    //! @param[in]  bufferIndex Map するバッファのインデックスです。
    //!
    void Map(int bufferIndex);

    //! @brief レイアウトデータの描画に使用する各種バッファを案マップして操作を完了します。
    //!
    void Unmap();

    //! @brief レイアウトデータの描画に GPU が参照するバッファのインデックスを設定します。
    //!
    //! @param[in]  bufferIndex 設定するバッファのインデックスです。
    //!
    void SetGpuAccessBufferIndex(int bufferIndex);

    //! @brief  キャプチャテクスチャやシザーペインが参照するフレームバッファのテクスチャを設定します。
    //!
    //! @param[in]  pTexture    参照するテクスチャです。
    //! @param[in]  width       テクスチャの幅です。
    //! @param[in]  height      テクスチャの高さです。
    //!
    void SetFramebufferTexture(const nn::gfx::Texture* pTexture, int width, int height);

    //! @brief  キャプチャテクスチャが参照するフレームバッファのテクスチャを取得します。
    //!
    //! @return  参照するテクスチャ。
    //!
    const nn::gfx::Texture* GetFramebufferTexture() const
    {
        return m_pFramebufferTexture;
    }

    //! @brief  設定されているフレームバッファテクスチャの幅を取得します。
    //!
    //! @return  フレームバッファテクスチャの幅。
    //!
    int GetFramebufferWidth() const
    {
        return m_FramebufferWidth;
    }

    //! @brief  設定されているフレームバッファテクスチャの高さを取得します。
    //!
    //! @return  フレームバッファテクスチャの高さ。
    //!
    int GetFramebufferHeight() const
    {
        return m_FramebufferHeight;
    }

    //! @brief  レイアウトランタイム内部でビューポートやシザー設定を変更した後にリセットされるビューポートやシザー設定の情報を設定します。
    //!
    //! @details
    //! シザーペインでシザー設定を変更した後にこのメソッドで設定された情報でシザー設定がリセットされます。
    //! 設定のリセット以外にビューポート情報は以下の用途に使用されるため、これらの機能を使用する場合はこのメソッドで有効な値を設定してください。
    //! - シザーペインの描画位置の計算
    //! - 透視投影テクスチャマッピングの投影位置の計算
    //!
    //! @param[in]  viewportInfo    ビューポート情報です。
    //! @param[in]  scissorInfo     シザー情報です。
    //!
    void SetDefaultViewportScissorInfo(const nn::gfx::ViewportStateInfo& viewportInfo, const nn::gfx::ScissorStateInfo& scissorInfo)
    {
        m_DefaultViewportInfo = viewportInfo;
        m_DefaultScissorInfo = scissorInfo;
    }

    //! @brief  デフォルトのビューポート設定を取得します。
    //!
    //! @return デフォルトのビューポート設定です。
    //!
    const nn::gfx::ViewportStateInfo& GetDefaultViewportInfo() const
    {
        return m_DefaultViewportInfo;
    }

    //! @brief  デフォルトのシザー設定を取得します。
    //!
    //! @return デフォルトのシザー設定です。
    //!
    const nn::gfx::ScissorStateInfo& GetDefaultScissorInfo() const
    {
        return m_DefaultScissorInfo;
    }

    //! @brief  有効なデフォルトビューポート情報が設定されているかどうかを取得します。
    //!
    //! @return デフォルトビューポート情報に初期値でない値が設定されていれば true を、それ以外の場合は false を返します。
    //!
    bool IsValidDefaultViewportInfoSet() const
    {
        return m_DefaultViewportInfo.GetWidth() > 0 && m_DefaultViewportInfo.GetHeight() > 0;
    }

    //! @brief  有効なデフォルトシザー情報が設定されているかどうかを取得します。
    //!
    //! @return デフォルトシザー情報に初期値でない値が設定されていれば true を、それ以外の場合は false を返します。
    //!
    bool IsValidDefaultScissorInfoSet() const
    {
        return m_DefaultScissorInfo.GetWidth() > 0 && m_DefaultScissorInfo.GetHeight() > 0;
    }

    //! @brief  デフォルトのカラーターゲットビューを設定します。
    //!
    //! @details
    //! レイアウトランタイム内部でレンダーターゲットを切り替えた際に復帰するカラーターゲットビューです。
    //!
    //! @param[in]  pColorTargetView    デフォルトのカラーターゲットビューです。
    //!
    void SetDefaultColorTargetView(nn::gfx::ColorTargetView* pColorTargetView)
    {
        m_pDefaultColorTargetView = pColorTargetView;
    }

    //! @brief  デフォルトのカラーターゲットビューを取得します。
    //!
    //! @return デフォルトのカラーターゲットビューです。
    //!
    nn::gfx::ColorTargetView* GetDefaultColorTargetView() const
    {
        return m_pDefaultColorTargetView;
    }

    //! @brief  デフォルトのデプスステンシルビューを設定します。
    //!
    //! @details
    //! レイアウトランタイム内部でデプスバッファを切り替えた際に復帰するデプスステンシルビューです。
    //! 以下の機能を使用する場合は設定する必要があります。
    //! - ベクターグラフィックステクスチャ
    //!
    //! @param[in]  pDepthStencilView   デフォルトのデプスステンシルビューです。
    //!
    void SetDefaultDepthStencilView(nn::gfx::DepthStencilView* pDepthStencilView)
    {
        m_pDefaultDepthStencilView = pDepthStencilView;
    }

    //! @brief  デフォルトのデプスステンシルビューを取得します。
    //!
    //! @return デフォルトのデプスステンシルビューです。
    //!
    nn::gfx::DepthStencilView* GetDefaultDepthStencilView() const
    {
        return m_pDefaultDepthStencilView;
    }

    //! @brief  デフォルトのデプスステンシルステートを設定します。
    //!
    //! @details
    //! レイアウトランタイム内部でデプスステンシルステートを切り替えた際に復帰するデプスステンシルステートです。
    //! 以下の機能を使用する場合は設定する必要があります。
    //! - ベクターグラフィックステクスチャ
    //!
    //! @param[in]  pDepthStencilState   デフォルトのデプスステンシルステートです。
    //!
    void SetDefaultDepthStencilState(nn::gfx::DepthStencilState* pDepthStencilState)
    {
        m_pDefaultDepthStencilState = pDepthStencilState;
    }

    //! @brief  デフォルトのデプスステンシルステートを取得します。
    //!
    //! @return デフォルトのデプスステンシルステートです。
    //!
    nn::gfx::DepthStencilState* GetDefaultDepthStencilState() const
    {
        return m_pDefaultDepthStencilState;
    }

    //! @brief  デフォルトのラスタライザーステートを設定します。
    //!
    //! @details
    //! レイアウトランタイム内部でラスタライザーステートを切り替えた際に復帰するラスタライザーステートです。
    //! 以下の機能を使用する場合は設定する必要があります。
    //! - ベクターグラフィックステクスチャの MSAA
    //!
    //! @param[in]  pRasterizerState    デフォルトのラスタライザーステートです。
    //!
    void SetDefaultRasterizerState(const nn::gfx::RasterizerState* pRasterizerState)
    {
        m_pDefaultRasterizerState = pRasterizerState;
    }

    //! @brief  デフォルトのラスタライザーステートを取得します。
    //!
    //! @return デフォルトのラスタライザーステートです。
    //!
    const nn::gfx::RasterizerState* GetDefaultRasterizerState() const
    {
        return m_pDefaultRasterizerState;
    }

    // RenderTarget, ScissorState, ViewportState を DrawInfo に設定されているデフォルトのものに設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  commandBuffer   コマンドバッファ。
    //!
    void ResetRenderTarget(nn::gfx::CommandBuffer& commandBuffer) const;

    // 描画中のレイアウトを取得します。
    // Layout::Draw() の実行中に現在描画しているレイアウトを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return レイアウトへのポインタを返します。
    //!
    Layout* GetLayout() { return m_pLayout; }

    // 描画中のレイアウトを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return 描画中のレイアウトです。
    //!
    const Layout* GetLayout() const { return m_pLayout; }

    // これから描画するレイアウトを設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pLayout  これから描画するレイアウトです。
    //!
    void SetLayout(Layout* pLayout)
    {
        m_pLayout = pLayout;
    }

    // 射影行列を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return 射影行列です。
    //!
    const nn::util::MatrixT4x4fType& GetProjectionMtx() const
    {
        return m_ProjMtx;
    }

    // 射影行列を設定します。

    //! @brief 内部用機能のため使用禁止です。
    //! @param[in] pProjectionMtx   射影行列
    void LoadProjectionMtx(float pProjectionMtx[4][4]);

    // モデルビュー行列を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return モデルビュー行列を返します。
    //!
    const nn::util::MatrixT4x3fType&  GetModelViewMtx() const
    {
        return m_ModelViewMtx;
    }

    // モデルビュー行列を設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] m    モデルビューです。
    //!
    void SetModelViewMtx(const nn::util::MatrixT4x3fType& m)
    {
        m_ModelViewMtx = m;
        m_ModelViewMtxLoaded = false;
    }

    // テクスチャが使用するテクスチャ座標の番号を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] index    テクスチャのインデックスです。
    //!
    //! @return 使用するテクスチャ座標の番号です。
    //!
    int GetTexCoordSrc(int index) const
    {
        return m_TexCoordSrc[index];
    }

    // テクスチャが使用するテクスチャ座標の番号を設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] index    テクスチャのインデックスです。
    //! @param[in] src      使用するテクスチャ座標の番号です。
    //!
    void SetTexCoordSrc(int index, int src)
    {
        m_TexCoordSrc[index] = static_cast<int8_t>(src);
    }

    // モデルビュー行列をシェーダーにロードします。

    //! @brief 内部用機能のため使用禁止です。
    //! @param[in] pModelView   モデルビュー行列
    void LoadMtxModelView(float pModelView[3][4]);

    // プログラムオブジェクトを設定します。

    //! @brief 内部用機能のため使用禁止です。
    void SetupProgram(nn::gfx::CommandBuffer* pCommandBuffer);

    // 頂点バッファの設定状態をリセットします。

    //! @brief 内部用機能のため使用禁止です。
    void ResetVertexBufferState()
    {
        m_FirstDraw = true;
    }

    // 使用中のシェーダを記録します。

    //! @brief 内部用機能のため使用禁止です。
    //! @return 更新があった場合は true、以前と同じだった場合は false が返ります。
    bool RecordCurrentShader(const ShaderInfo* pShaderInfo, uint16_t shaderVariation);

    // 使用中のシェーダの記録をリセットします。

    //! @brief 内部用機能のため使用禁止です。
    void ResetCurrentShader();

    // 許容可能なテクスチャ数を設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] quantity 許容可能なテクスチャ数です。
    //!
    void SetAllowableTextureQuantity(int quantity)
    {
        m_AllowableTextureQuantity = static_cast<int8_t>(quantity);
    }

    // 許容可能なテクスチャ数を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return 許容可能なテクスチャ数です。
    //!
    int GetAllowableTextureQuantity() const
    {
        return m_AllowableTextureQuantity;
    }

    // モデルビュー行列のロード状態をリセットします。

    //! @brief 内部用機能のため使用禁止です。
    //!
    void ResetModelViewMtxLoaded()
    {
        m_ModelViewMtxLoaded = false;
    }

    // フレームバッファテクスチャのデスクリプタスロットを設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  pTextureDescriptorSlot フレームバッファテクスチャのデスクリプタスロットです。
    //!
    //! @details CheckFrameBufferTextureDescriptorSlotRequired でデスクリプタスロットが必要か調べる事が出来ます。
    //!
    //! @sa nn::ui2d::CheckFrameBufferTextureDescriptorSlotRequired
    //! @sa nn::ui2d::DrawInfo::GetFramebufferTextureDescriptorSlot
    //!
    void SetFramebufferTextureDescriptorSlot(const nn::gfx::DescriptorSlot* pTextureDescriptorSlot);

    // フレームバッファサンプラのデスクリプタスロットを設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  pSamplerDescriptorSlot フレームバッファサンプラのデスクリプタスロットです。
    //!
    //! @details 事前に CheckFrameBufferTextureDescriptorSlotRequired でデスクリプタスロットが必要か調べる事が出来ます。
    //!
    //! @sa nn::ui2d::CheckFrameBufferTextureDescriptorSlotRequired
    //! @sa nn::ui2d::DrawInfo::GetFramebufferSamplerDescriptorSlot
    //!
    void SetFramebufferSamplerDescriptorSlot(const nn::gfx::DescriptorSlot* pSamplerDescriptorSlot);

    // フレームバッファテクスチャのデスクリプタスロットを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details CheckFrameBufferTextureDescriptorSlotRequired でデスクリプタスロットが必要か調べる事が出来ます。
    //!
    //! @return  フレームバッファテクスチャのデスクリプタスロットを取得します。
    //!
    //! @sa nn::ui2d::CheckFrameBufferTextureDescriptorSlotRequired
    //! @sa nn::ui2d::DrawInfo::SetFramebufferTextureDescriptorSlot
    //!
    const nn::gfx::DescriptorSlot* GetFramebufferTextureDescriptorSlot() const;

    // フレームバッファサンプラのデスクリプタスロットを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details CheckFrameBufferTextureDescriptorSlotRequired でデスクリプタスロットが必要か調べる事が出来ます。
    //!
    //! @return  フレームバッファサンプラのデスクリプタスロットを取得します。
    //!
    //! @sa nn::ui2d::CheckFrameBufferTextureDescriptorSlotRequired
    //! @sa nn::ui2d::DrawInfo::SetFramebufferSamplerDescriptorSlot
    //!
    const nn::gfx::DescriptorSlot* GetFramebufferSamplerDescriptorSlot() const;

private:
    nn::util::MatrixT4x4fType m_ProjMtx;
    nn::util::MatrixT4x3fType m_ViewMtx;
    nn::util::MatrixT4x3fType m_ModelViewMtx;
    nn::util::Float2 m_LocationAdjustScale;
    GraphicsResource* m_pGraphicsResource;
    Layout* m_pLayout;
    nn::font::GpuBuffer*  m_pUi2dConstantBuffer;
    nn::font::GpuBuffer*  m_pFontConstantBuffer;

    const nn::gfx::Texture*         m_pFramebufferTexture;
    uint32_t                        m_FramebufferWidth;
    uint32_t                        m_FramebufferHeight;
    const nn::gfx::DescriptorSlot*  m_pFramebufferTextureDescriptorSlot;
    const nn::gfx::DescriptorSlot*  m_pFramebufferSamplerDescriptorSlot;

    nn::gfx::ColorTargetView*   m_pDefaultColorTargetView;
    nn::gfx::ViewportStateInfo  m_DefaultViewportInfo;
    nn::gfx::ScissorStateInfo   m_DefaultScissorInfo;

    nn::gfx::DepthStencilView*  m_pDefaultDepthStencilView;
    nn::gfx::DepthStencilState* m_pDefaultDepthStencilState;
    const nn::gfx::RasterizerState*   m_pDefaultRasterizerState;

    UpdateUserShaderCallback    m_pUpdateUserShaderCallback;
    void*                       m_pUpdateUserShaderCallbackUserData;

    ApplyCaptureTextureFilterCallback   m_pApplyCaptureTextureFilterCallback;
    void*                               m_pApplyCaptureTextureFilterCallbackUserData;

    int8_t m_TexCoordSrc[TexMapMax];
    bool m_ModelViewMtxLoaded;
    bool m_FirstDraw;
    int8_t m_AllowableTextureQuantity;

    struct Flag
    {
        uint8_t  isLocationAdjustEnabled : 1;
        uint8_t  invisiblePaneCalculateMtx : 1;
        uint8_t  alphaZeroPaneCalculateMtx : 1;
        uint8_t  isLeftTopWindowOrigin : 1;
    };

    Flag m_Flag;

    const ShaderInfo* m_pCurrentShaderInfo;
    uint16_t m_CurrentShaderVariation;
};

} // namespace nn::ui2d
} // namespace nn
