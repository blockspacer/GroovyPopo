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

#include <nn/ui2d/ui2d_WindowFoundation.h>
#include <nn/ui2d/ui2d_Util.h>
#include <nn/ui2d/ui2d_Pane.h>
#include <nn/ui2d/ui2d_Material.h>

namespace nn
{
namespace ui2d
{

struct ResWindow;
class DrawInfo;


//---------------------------------------------------------------------------
//! @brief ウィンドウの表示を行うペインです。
//!
//---------------------------------------------------------------------------
class Window : public Pane
{
    typedef Pane Base;

    //! @brief 内部用機能のため使用禁止です。
    class Frame
    {
    public:// TODO:アクセサ
        Frame()
        :   textureFlip(0),
            pMaterial(0)
        {}

        ~Frame();

        TextureFlip GetTextureFlip() const
        {
            return (TextureFlip) this->textureFlip;
        }

        uint8_t  textureFlip;
        Material* pMaterial;
    };

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(Window::Base);

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! コンテンツ領域とフレーム(4辺共通)でそれぞれ指定したテクスチャの
    //! 枚数分のメモリを確保して、オブジェクトを生成します。
    //!
    //! @param[in] contentTexCount    コンテンツ領域で使用するテクスチャの最大数です。
    //! @param[in] frameTexCount      フレームで使用するテクスチャの最大数です。
    //!
    Window(
        int   contentTexCount,
        int   frameTexCount);

    //! @brief コンストラクタです。
    //!
    //! @details
    //! コンテンツ領域とフレーム(4辺)でそれぞれ指定したテクスチャの
    //! 枚数分のメモリを確保して、オブジェクトを生成します。
    //!
    //! @param[in] contentTexCount    コンテンツ領域で使用するテクスチャの最大数です。
    //! @param[in] frameLTTexCount    左上辺フレームで使用するテクスチャの最大数です。
    //! @param[in] frameRTTexCount    右上辺フレームで使用するテクスチャの最大数です。
    //! @param[in] frameRBTexCount    右下辺フレームで使用するテクスチャの最大数です。
    //! @param[in] frameLBTexCount    左下辺フレームで使用するテクスチャの最大数です。
    //!
    Window(
        int   contentTexCount,
        int   frameLTTexCount,
        int   frameRTTexCount,
        int   frameRBTexCount,
        int   frameLBTexCount);

    //! @brief コンストラクタです。
    //!
    //! @details
    //! コンテンツ領域とフレーム(4隅と4辺)でそれぞれ指定したテクスチャの
    //! 枚数分のメモリを確保して、オブジェクトを生成します。
    //!
    //! @param[in] contentTexCount    コンテンツ領域で使用するテクスチャの最大数です。
    //! @param[in] cornerLTTexCount   左上隅フレームで使用するテクスチャの最大数です。
    //! @param[in] cornerRTTexCount   右上隅フレームで使用するテクスチャの最大数です。
    //! @param[in] cornerRBTexCount   右下隅フレームで使用するテクスチャの最大数です。
    //! @param[in] cornerLBTexCount   左下隅フレームで使用するテクスチャの最大数です。
    //! @param[in] frameLTexCount     左辺フレームで使用するテクスチャの最大数です。
    //! @param[in] frameTTexCount     上辺フレームで使用するテクスチャの最大数です。
    //! @param[in] frameRTexCount     右辺フレームで使用するテクスチャの最大数です。
    //! @param[in] frameBTexCount     下辺フレームで使用するテクスチャの最大数です。
    //!
    Window(
        int   contentTexCount,
        int   cornerLTTexCount,
        int   cornerRTTexCount,
        int   cornerRBTexCount,
        int   cornerLBTexCount,
        int   frameLTexCount,
        int   frameTTexCount,
        int   frameRTexCount,
        int   frameBTexCount);

    //! @brief コンストラクタです。
    //!
    //! @details
    //! リソースからオブジェクトを生成します。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報です。
    //! @param[in] pDevice          デバイス
    //! @param[in] pBaseBlock       このペインが所属するレイアウトが保持しているリソースへのポインタです。
    //! @param[in] pOverrideBlock   上書きを行ったリソースへのポインタです。上書きが行われていないときはNULLになります。
    //! @param[in] buildArgSet      構築時の引数への参照です。
    //!
    Window(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        const ResWindow* pBaseBlock,
        const ResWindow* pOverrideBlock,
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
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Window::Window(const Window& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    //! @param[in] window   コピー元のペインです。
    //! @param[in] pDevice          デバイス
    //!
    explicit Window(const Window& window, nn::gfx::Device* pDevice)
        : Base(window)
    {
        CopyImpl(window, pDevice, NULL, NULL);
    }

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
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Window::Window(const Window& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    //! @param[in] window   コピー元のペインです。
    //! @param[in] pDevice          デバイス
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //!
    //! @deprecated ui2d ランタイムで動的レンダリングテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
    //!             動的レンダリングテクスチャ関連機能を使用したペインをコピーする場合は Window::Window(const Window& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    NN_DEPRECATED explicit Window(const Window& window, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pNewRootName)
        : Base(window)
    {
        NN_UNUSED(pResAccessor);
        NN_UNUSED(pNewRootName);
        CopyImpl(window, pDevice, NULL, NULL);
    }

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
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Window::Window(const Window& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    //! @param[in] window   コピー元のペインです。
    //! @param[in] pDevice          デバイス
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //! @param[in] pLayout このペインを管理しているレイアウトです。
    //!
    //! @deprecated ui2d ランタイムで動的レンダリングテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
    //!             動的レンダリングテクスチャ関連機能を使用したペインをコピーする場合は Window::Window(const Window& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    NN_DEPRECATED explicit Window(const Window& window, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pNewRootName, const Layout* pLayout)
        : Base(window)
    {
        NN_UNUSED(pResAccessor);
        NN_UNUSED(pNewRootName);
        NN_UNUSED(pLayout);
        CopyImpl(window, pDevice, NULL, NULL);
    }

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
    //! キャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャなどの動的レンダリングテクスチャを使用している場合は
    //! このコピーコンストラクタを使用してコピーしてください。
    //! pLayout で指定したレイアウトクラスへ動的レンダリングテクスチャのリソースが登録されます。
    //!
    //! @param[in] window   コピー元のペインです。
    //! @param[in] pDevice          デバイス
    //! @param[in] pLayout 動的レンダリングテクスチャリソースを登録するレイアウトインスタンス。通常、ペインの所属するレイアウトインスタンスを指定します。
    //!
    Window(const Window& window, nn::gfx::Device* pDevice, Layout* pLayout);

    // ClonePaneTree から呼び出される内部用のコピーコンストラクタです。

    //! @brief 内部用機能のため使用禁止です。
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
    //! @param[in] window   コピー元のペインです。
    //! @param[in] pDevice          デバイス
    //! @param[in] pLayout このペインを管理しているレイアウトです。
    //! @param[in] pBuildPaneTreeContext 動的レンダリングテクスチャの共有情報や初期化情報などランタイム内部で使用する情報をペインツリーをまたいで受け渡しするためのデータです。
    //!
    explicit Window(const Window& window, nn::gfx::Device* pDevice, const Layout* pLayout, detail::BuildPaneTreeContext* pBuildPaneTreeContext)
        : Base(window, pDevice, pLayout, pBuildPaneTreeContext)
    {
        CopyImpl(window, pDevice, pLayout, pBuildPaneTreeContext);
    }

    //! @brief デストラクタです。
    //!
    virtual ~Window();

    //! @brief リソースを開放します。
    //!
    //! @details
    //! デストラクタの呼び出し以前に呼び出す必要があります。
    //!
    //! @param[in] pDevice          デバイスです。
    //!
    virtual void Finalize(nn::gfx::Device* pDevice) NN_OVERRIDE;

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief 頂点カラーを取得します。
    //!
    //! @param[in] idx  インデックスです。 nn::ui2d::VertexColor を指定します。
    //!
    //! @return ペインの四隅の頂点カラーを返します。
    //!
    //! @sa SetVertexColor
    //! @sa GetVertexColorElement
    //! @sa nn::ui2d::VertexColor
    //!
    virtual const nn::util::Unorm8x4 GetVertexColor(int  idx) const NN_OVERRIDE;

    //! @brief 頂点カラーを設定します。
    //!
    //! @param[in] idx      インデックスです。 nn::ui2d::VertexColor を指定します。
    //! @param[in] value    頂点カラーです。
    //!
    //! @sa GetVertexColor
    //! @sa SetVertexColorElement
    //! @sa nn::ui2d::VertexColor
    //!
    virtual void SetVertexColor(int  idx, const nn::util::Unorm8x4& value) NN_OVERRIDE;

    //! @brief テキストの表示色（頂点カラー）の各カラー成分を取得します。
    //!
    //! @details
    //! このメソッドは、R,G,B など色成分を個別に取得します。色成分をまとめて取得する場合は GetVertexColor を利用してください。
    //!
    //! @param[in] idx  インデックスです。 nn::ui2d::AnimTargetPaneColor を指定します。
    //!
    //! @return テキストの表示色の各成分を返します。
    //!
    //! @sa SetVertexColorElement
    //! @sa GetVertexColor
    //! @sa nn::ui2d::AnimTargetPaneColor
    //!
    virtual uint8_t  GetVertexColorElement(int  idx) const NN_OVERRIDE;

    //! @brief テキストの表示色（頂点カラー）の各カラー成分を設定します。
    //!
    //! @details
    //! このメソッドは、R,G,B など色成分を個別に設定します。色成分をまとめて設定する場合は SetVertexColor を利用してください。
    //!
    //! @param[in] idx      インデックスです。 nn::ui2d::AnimTargetPaneColor を指定します。
    //! @param[in] value    カラーの成分値です。
    //!
    //! @sa GetVertexColorElement
    //! @sa SetVertexColor
    //! @sa nn::ui2d::AnimTargetPaneColor
    //!
    virtual void SetVertexColorElement(int  idx, uint8_t  value) NN_OVERRIDE;

    using Base::GetMaterial;

    //! @brief ペインが持つマテリアルの数を取得します。
    //!
    //! @details
    //! Window ペインはコンテンツ領域＋フレーム数のマテリアルを持ちます。
    //!
    //! @return フレーム数 + 1 を返します。
    //!
    //! @sa GetMaterial
    //!
    virtual uint8_t  GetMaterialCount() const NN_OVERRIDE;

    //! @brief ペインのマテリアルを取得します。
    //!
    //! @details
    //! idx に指定する値は GetMaterialCount() の返り値未満でなければなりません。
    //!
    //! @param[in] idx  インデックスです。
    //!
    //! @return マテリアルへのポインタを返します。
    //!
    //! @sa GetMaterialCount
    //! @sa GetContentMaterial
    //! @sa SetContentMaterial
    //! @sa GetFrameMaterial
    //! @sa SetFrameMaterial
    //!
    virtual Material* GetMaterial(int  idx) const NN_OVERRIDE;

    //! @brief コンテント領域のマテリアルを取得します。
    //!
    //! @return マテリアルへのポインタを返します。
    //!
    //! @sa GetMaterial
    //! @sa SetContentMaterial
    //! @sa GetFrameMaterial
    //! @sa SetFrameMaterial
    //!
    Material* GetContentMaterial() const;

    //! @brief コンテント領域のマテリアルを設定します。
    //!
    //! @details
    //! 現在設定されているマテリアルが Window オブジェクトの生成時に
    //! 同時に生成されたものだった場合には、そのマテリアルは破棄されます。
    //!
    //! @param[in] pMaterial    マテリアルへのポインタです。
    //!
    //! @sa GetMaterial
    //! @sa GetContentMaterial
    //! @sa GetFrameMaterial
    //! @sa SetFrameMaterial
    //!
    void SetContentMaterial(Material* pMaterial);

    //! @brief ウィンドウフレーム領域のマテリアルを取得します。
    //!
    //! @param[in] frameIdx フレームのインデックスです。
    //!
    //! @return マテリアルへのポインタを返します。
    //!
    //! @sa GetMaterial
    //! @sa GetContentMaterial
    //! @sa SetContentMaterial
    //! @sa SetFrameMaterial
    //!
    Material* GetFrameMaterial(WindowFrame frameIdx) const;

    //! @brief ウィンドウフレーム領域のマテリアルを設定します。
    //!
    //! @details
    //! 現在設定されているマテリアルが Window オブジェクトの生成時に
    //! 同時に生成されたものだった場合には、そのマテリアルは破棄されます。
    //!
    //! @param[in] frameIdx     フレームのインデックスです。
    //! @param[in] pMaterial    マテリアルへのポインタです。
    //!
    //! @sa GetMaterial
    //! @sa GetContentMaterial
    //! @sa SetContentMaterial
    //! @sa GetFrameMaterial
    //!
    void SetFrameMaterial(WindowFrame frameIdx, Material* pMaterial);

    //! @brief コンテント領域で使うテクスチャ座標を保持するためのメモリ領域を確保します。
    //!
    //! @details
    //! 保持可能なテクスチャ座標の数の初期値はコンテント領域のテクスチャ数と同じです。
    //!
    //! @param[in] count  テクスチャ座標の保持数です。
    //!
    //! @sa Window::Window
    //!
    void ReserveTexCoord(int  count);

    //! @brief コンテント領域で使うテクスチャ座標の保持数を取得します。
    //!
    //! @return 保持しているテクスチャ座標の数を返します。
    //!
    //! @sa SetTexCoordCount
    //!
    uint8_t  GetTexCoordCount() const;

    //! @brief コンテント領域で使うテクスチャ座標の保持数を設定します。
    //!
    //! @details
    //! count に指定できる値は保持可能なテクスチャ座標数以下でなければなりません。
    //!
    //! @param[in] count  テクスチャ座標の保持数です。
    //!
    //! @sa ReserveTexCoord
    //! @sa GetTexCoord
    //!
    void SetTexCoordCount(int  count);

    //! @brief コンテント領域で使うテクスチャ座標を取得します。
    //!
    //! @param[out] coords  テクスチャ座標を格納する領域へのポインタです。
    //! @param[in] idx      インデックスです。
    //!
    void GetTexCoord(TexCoordQuad coords, int  idx) const;

    //! @brief コンテント領域で使うテクスチャ座標を設定します。
    //!
    //! @param[in] idx      インデックスです。
    //! @param[in] coords   テクスチャ座標です。
    //!
    void SetTexCoord(int  idx, const TexCoordQuad coords);

    //! @brief ウィンドウフレーム数を取得します。
    //!
    //! @details
    //! ウィンドウフレームの数はオブジェクト生成時に決定されます。
    //!
    //! @return オブジェクトの生成方法によって 1, 2, 4, 8 のいずれかを返します。
    //!
    //! @sa Window::Window
    //!
    uint8_t  GetFrameCount() const
    {
        return m_FrameCount;
    }

    //! @brief ウィンドウフレームのサイズを取得します。
    //!
    //! @details
    //! HORIZONTAL_MODE の場合、左右のフレームはそれぞれ WINDOWFRAME_LT, WINDOWFRAME_RT
    //! になります。
    //!
    //! @param[out] pSize    フレームのサイズです。
    //!
    //! @sa Window::Window
    //!
    void CalculateFrameSize(WindowFrameSize* pSize)
    {
        NN_SDK_ASSERT(pSize, "size must not be NULL for Window[%s]", GetName());

        pSize->left = m_WindowSize.frameSize.left;
        pSize->right = m_WindowSize.frameSize.right;
        pSize->top = m_WindowSize.frameSize.top;
        pSize->bottom = m_WindowSize.frameSize.bottom;
    }

    //! @brief ウィンドウフレームのサイズを設定します。
    //!
    //! @param[in] index      インデックスです。nn::ui2d::AnimTargetWindow で指定します。
    //! @param[in] newSize    フレーム値です。ゼロ以下の値を指定した場合、ゼロにクランプされます。
    //!
    //! @sa Window::Window
    //!
    void SetWindowFrameSize(int index, int newSize)
    {
        switch (index)
        {
        case AnimTargetWindow_FrameTop: m_WindowSize.frameSize.top = static_cast<uint16_t>(std::max(newSize, 0)); break;
        case AnimTargetWindow_FrameBottom: m_WindowSize.frameSize.bottom = static_cast<uint16_t>(std::max(newSize, 0)); break;
        case AnimTargetWindow_FrameLeft: m_WindowSize.frameSize.left = static_cast<uint16_t>(std::max(newSize, 0)); break;
        case AnimTargetWindow_FrameRight: m_WindowSize.frameSize.right = static_cast<uint16_t>(std::max(newSize, 0)); break;
        default:
            NN_SDK_ASSERT(false);
            break;
        }
    }

    //! @brief ウィンドウフレームのモードを取得します。
    //!
    //! @return ウィンドウフレームのモード
    //!
    WindowFrameMode GetWindowFrameMode() const
    {
        return static_cast<WindowFrameMode>(m_FrameMode);
    }

    //@}

    //----------------------------------------
    //! @name 検索
    //@{

    //! @brief 名前でマテリアルを検索します。
    //!
    //! @details
    //! pFindName のマテリアル名を持つマテリアルを検索します。
    //!
    //! bRecursive に true を渡した場合は子ペインと、子ペインが子を
    //! 持つ場合はさらにその子ペインからも検索します。
    //!
    //! @param[in] pFindName     検索するマテリアル名です。
    //! @param[in] bRecursive   子ペインも検索する場合は true を指定します。
    //!
    //! @return マテリアルが見つかった場合はそのマテリアルへのポインタを、
    //! 見つからなかった場合は NULL を返します。
    //!
    virtual Material* FindMaterialByName(const char* pFindName, bool bRecursive = true) NN_OVERRIDE;
    //! @brief 名前でマテリアルを検索します。
    //!
    //! @details
    //! pFindName のマテリアル名を持つマテリアルを検索します。
    //!
    //! bRecursive に true を渡した場合は子ペインと、子ペインが子を
    //! 持つ場合はさらにその子ペインからも検索します。
    //!
    //! @param[in] pFindName     検索するマテリアル名です。
    //! @param[in] bRecursive   子ペインも検索する場合は true を指定します。
    //!
    //! @return マテリアルが見つかった場合はそのマテリアルへのポインタを、
    //! 見つからなかった場合は NULL を返します。
    //!
    virtual const Material* FindMaterialByName(const char* pFindName, bool bRecursive = true) const NN_OVERRIDE;

    //@}

    virtual void Calculate(DrawInfo& drawInfo, Pane::CalculateContext& context, bool isDirtyParentMtx) NN_OVERRIDE;

    //! @brief  ペインエフェクトのキャプチャ結果を最終的に描画する際に描画状態を設定します。
    //!
    //! @details
    //! ペインエフェクトを使用した際は最終的にキャプチャされたペインのテクスチャが画面にレンダリングされます。
    //! このメソッドではその描画の際の描画設定を行います。
    //!
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    virtual void SetupPaneEffectSourceImageRenderState(nn::gfx::CommandBuffer& commandBuffer) const NN_OVERRIDE;

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
    bool CompareCopiedInstanceTest(const Window& target) const;
protected:
    virtual void DrawSelf(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer) NN_OVERRIDE;

    //! @brief コピーコンストラクタの実装です。
    //!
    //! @param[in] window コピー元のウインドウペインです。
    //! @param[in] pDevice デバイスです。
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //! @param[in] pLayout コピーされたレイアウトへのポインタです。
    //!
    void CopyImpl(const Window& window, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pNewRootName, const Layout* pLayout)
    {
        NN_UNUSED(pResAccessor);
        NN_UNUSED(pNewRootName);

        CopyImpl(window, pDevice, pLayout, NULL);
    }

    void CopyImpl(const Window& window, nn::gfx::Device* pDevice, const Layout* pLayout, detail::BuildPaneTreeContext* pBuildPaneTreeContext);
private:
    void InitializeTexCount(
        int  contentTexCount,
        int  frameTexCount[],
        int  frameCount);
    void InitializeUseLeftTopMaterialEmulation(BuildResultInformation* pOutBuildResultInformation, nn::gfx::Device* pDevice);
    void InitializeSize(const ResWindow* pBlock);

    void InitializeContent(int  texCount);

    void InitializeFrame(int  frameCount);

    void CalculateAroundFrameWindow(DrawInfo& drawInfo);
    void CalculateHorizontalFrameWindow(DrawInfo& drawInfo);
    void CalculateHorizontalFrameNocontentWindow(DrawInfo& drawInfo);

    void CalculateContent(
        DrawInfo& draInfo,
        const nn::util::Float2& basePt,
        const WindowFrameSize& frameSize,
        uint8_t  alpha);

    void CalculateFrame(
        DrawInfo& draInfo,
        const nn::util::Float2& basePt,
        const Frame& frame,
        const WindowFrameSize& frameSize,
        uint8_t  alpha);

    void CalculateHorizontalFrame(
        DrawInfo& draInfo,
        const nn::util::Float2& basePt,
        const Frame& frame,
        const WindowFrameSize& frameSize,
        uint8_t  alpha);

    void CalculateHorizontalNocontentFrame(
        DrawInfo& draInfo,
        const nn::util::Float2& basePt,
        const Frame& frame,
        const WindowFrameSize& frameSize,
        uint8_t  alpha);

    void CalculateHorizontalFrame2(
        DrawInfo& draInfo,
        const nn::util::Float2& basePt,
        const Frame* frames,
        const WindowFrameSize& frameSize,
        uint8_t  alpha);

    void CalculateHorizontalNocontentFrame2(
        DrawInfo& draInfo,
        const nn::util::Float2& basePt,
        const Frame* frames,
        const WindowFrameSize& frameSize,
        uint8_t  alpha);

    void CalculateFrame4(
        DrawInfo& draInfo,
        const nn::util::Float2& basePt,
        const Frame* frames,
        const WindowFrameSize& frameSize,
        uint8_t  alpha);

    void CalculateFrame8(
        DrawInfo& draInfo,
        const nn::util::Float2& basePt,
        const Frame* frames,
        const WindowFrameSize& frameSize,
        uint8_t  alpha);

    void DrawSharedMaterialImpl(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);
    void DrawNormalImpl(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    // コピー演算子を禁止します。
    const Window& operator=( const Window& );

private:
    void Initialize();

    enum WindowFlags
    {
        WindowFlags_OverallVertexColorEnabled = (1 << 0),
        WindowFlags_OneMaterialForAllFrameEnabled = (1 << 1),
        WindowFlags_NotDrawContentEnabled = (1 << 2),
        WindowFlags_ContentInflationEnabled = (1 << 3)
    };

    enum DynamicGenerateTextureCopyTarget
    {
        DynamicGenerateTextureCopyTarget_Content,
        DynamicGenerateTextureCopyTarget_Frame0,
        DynamicGenerateTextureCopyTarget_Frame1,
        DynamicGenerateTextureCopyTarget_Frame2,
        DynamicGenerateTextureCopyTarget_Frame3,
        DynamicGenerateTextureCopyTarget_Frame4,
        DynamicGenerateTextureCopyTarget_Frame5,
        DynamicGenerateTextureCopyTarget_Frame6,
        DynamicGenerateTextureCopyTarget_Frame7,

        DynamicGenerateTextureCopyTarget_Max
    };

    WindowSize m_WindowSize;
    WindowContent m_Content;
    uint8_t  m_FrameMode;
    int8_t  m_FrameCount;
    uint8_t  m_WindowFlags;
    Frame* m_pFrames;
    Material* m_pMaterial;

    uint32_t*   m_pUseLeftTopEmulationConstantBuffersOffset;
    uint32_t    m_UseLeftTopEmulationConstantBufferCount;
};

//----------------------------------------
inline void
Window::ReserveTexCoord(int  count)
{
    m_Content.texCoordArray.Reserve(count);
}

//----------------------------------------
inline uint8_t
Window::GetTexCoordCount() const
{
    return static_cast<uint8_t>(m_Content.texCoordArray.GetSize());
}

//----------------------------------------
inline void
Window::SetTexCoordCount(int  count)
{
    m_Content.texCoordArray.SetSize(count);
}

//----------------------------------------
inline void
Window::GetTexCoord(
    TexCoordQuad coords,
    int  idx
) const
{
    m_Content.texCoordArray.GetCoord(coords, idx);
}

//----------------------------------------
inline void
Window::SetTexCoord(
    int  idx,
    const TexCoordQuad coords
)
{
    m_Content.texCoordArray.SetCoord(idx, coords);
}

//----------------------------------------
inline Material*
Window::GetFrameMaterial(WindowFrame frameIdx) const
{
    NN_SDK_ASSERT(frameIdx < WindowFrame_MaxWindowFrame, "out of bounds: frameIdx[%d] < WindowFrame_MaxWindowFrame for Window[%s]", frameIdx, GetName());

    if (frameIdx >= m_FrameCount)
    {
        return NULL;
    }

    return m_pFrames[frameIdx].pMaterial;
}

//----------------------------------------
inline Material*
Window::GetContentMaterial() const
{
    return m_pMaterial;
}


} // namespace nn::ui2d
} // namespace nn
