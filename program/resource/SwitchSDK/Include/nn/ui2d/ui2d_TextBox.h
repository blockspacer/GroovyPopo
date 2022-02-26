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

#include <nn/font/font_Types.h>
#include <nn/font/font_DispStringBuffer.h>
#include <nn/font/font_WideTextWriter.h>
#include <nn/font/font_TextWriter.h>

#include <nn/ui2d/ui2d_Util.h>
#include <nn/ui2d/ui2d_TextSearcher.h>
#include <nn/ui2d/ui2d_Pane.h>

namespace nn
{
namespace font
{

class Font;

template <typename CharType>
class TagProcessorBase;

template <typename CharType>
class TextWriterBase;

} // namespace nn::font
}

namespace nn
{
namespace ui2d
{

struct ResTextBox;
struct ResAnimationTarget;
struct ResAnimationInfo;

class DrawInfo;
struct BuildResSet;
class Layout;

//---------------------------------------------------------------------------
//! @brief テキストの表示を行うペインです。
//!
//---------------------------------------------------------------------------
class TextBox : public Pane
{
    typedef Pane Base;

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(TextBox::Base);

    //! TextBox が使用するタグプロセッサの定義です。
    typedef nn::font::TagProcessorBase<uint16_t> TagProcessor;
    typedef nn::font::TagProcessorBase<char> TagProcessorUtf8;

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! オブジェクトを生成します。
    //! 文字列用バッファの確保、フォントのセット等は行いません。
    //!
    explicit TextBox();

    //! @brief コンストラクタです。
    //!
    //! @details
    //! オブジェクトを生成します。
    //! 文字列用バッファの確保、フォントのセット等は行いません。
    //!
    //! @param[in] isUtf8 UTF-8 モードで初期化するかどうかです。
    //!
    explicit TextBox(bool isUtf8);

    //! @brief InitStringメソッドにコンストラクタから必要なパラメータを渡すための構造体です。
    //!
    struct InitializeStringParam {
        int  textBoxFlag;             //!< TextBox::textBoxFlagの値
        Layout* pResourceLayout;     //!< リソースを保持しているレイアウト
        const uint16_t* pBlockText;   //!< レイアウトエディタで指定された文字列
        size_t  allocStrBufLen;         //!< レイアウトエディタで指定されたバッファサイズ
        int  resStrLen;              //!< レイアウトエディタで指定された文字列長
    };

    //! @brief コンストラクタです。
    //!
    //! @details
    //! リソースからオブジェクトを生成します。
    //! 文字列関連の初期化は、ここではなくInitStringで行われます。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報です。
    //! @param[in]  pDevice         デバイスです。
    //! @param[in]  pInitStringParam InitializeString を呼び出す際のパラメータを書き出します。
    //! @param[in]  pBaseBlock      このペインが所属するレイアウトが保持しているリソースへのポインタです。
    //! @param[in]  pOverrideBlock  上書きを行ったリソースへのポインタです。上書きが行われていないときはNULLになります。
    //! @param[in]  buildArgSet     構築時の引数への参照です。
    //!
    TextBox(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        InitializeStringParam* pInitStringParam,
        const ResTextBox* pBaseBlock,
        const ResTextBox* pOverrideBlock,
        const BuildArgSet& buildArgSet);

    //! @brief 文字列関連の初期化を行います。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報です。
    //! @param[in] pDevice         デバイスです。
    //! @param[in] buildArgSet     構築時の引数への参照です。
    //! @param[in] initStringParam コンストラクタから渡されるパラメータです。
    //!
    virtual void InitializeString(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        const BuildArgSet& buildArgSet,
        const InitializeStringParam& initStringParam);

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
    //! ペインツリーに登録するためには、明示的にペインに AppendChild() してください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! 新しく生成されるペインのために、font 用のコンスタントバッファを、
    //! レイアウト構築時に自動計算される requiredFontConstantBufferSize の値よりも、
    //! 多めに確保して作成しておく必要があります。
    //!
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Window::Window(const Pane& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    //! @param[in] textBox  コピー元のペインです。
    //! @param[in] pDevice  デバイスです。
    //!
    explicit TextBox(const TextBox& textBox, nn::gfx::Device* pDevice)
        : Pane(textBox)
    {
        CopyImpl(textBox, pDevice);
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
    //! ペインツリーに登録するためには、明示的にペインに AppendChild() してください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! 新しく生成されるペインのために、font 用のコンスタントバッファを、
    //! レイアウト構築時に自動計算される requiredFontConstantBufferSize の値よりも、
    //! 多めに確保して作成しておく必要があります。
    //!
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Window::Window(const Pane& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    //! @param[in] textBox  コピー元のペインです。
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //!
    //! @deprecated ui2d ランタイムで動的レンダリングテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
    //!             動的レンダリングテクスチャ関連機能を使用したペインをコピーする場合は TextBox::TextBox(const TextBox& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    NN_DEPRECATED explicit TextBox(const TextBox& textBox, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pNewRootName)
        : Pane(textBox)
    {
        NN_UNUSED(pResAccessor);
        NN_UNUSED(pNewRootName);
        CopyImpl(textBox, pDevice);
    }

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
    //! ペインツリーに登録するためには、明示的にペインに AppendChild() してください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! 新しく生成されるペインのために、font 用のコンスタントバッファを、
    //! レイアウト構築時に自動計算される requiredFontConstantBufferSize の値よりも、
    //! 多めに確保して作成しておく必要があります。
    //!
    //! @param[in] textBox  コピー元のペインです。
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pLayout  ペインの所属するレイアウトインスタンスです。
    //! @param[in] pBuildPaneTreeContext 動的レンダリングテクスチャの共有情報や初期化情報などランタイム内部で使用する情報をペインツリーをまたいで受け渡しするためのデータです。
    //!
    TextBox(const TextBox& textBox, nn::gfx::Device* pDevice, const Layout* pLayout, detail::BuildPaneTreeContext* pBuildPaneTreeContext)
        : Pane(textBox, pDevice, pLayout, pBuildPaneTreeContext)
    {
        CopyImpl(textBox, pDevice);
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
    //! ペインツリーに登録するためには、明示的にペインに AppendChild() してください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! 新しく生成されるペインのために、font 用のコンスタントバッファを、
    //! レイアウト構築時に自動計算される requiredFontConstantBufferSize の値よりも、
    //! 多めに確保して作成しておく必要があります。
    //!
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Window::Window(const Pane& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    //! @param[in] textBox  コピー元のペインです。
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //! @param[in] pLayout このペインを管理しているレイアウトです。
    //!
    //! @deprecated ui2d ランタイムで動的レンダリングテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
    //!             動的レンダリングテクスチャ関連機能を使用したペインをコピーする場合は TextBox::TextBox(const TextBox& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    NN_DEPRECATED TextBox(const TextBox& textBox, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pNewRootName, const Layout* pLayout)
        : Pane(textBox)
    {
        NN_UNUSED(pResAccessor);
        NN_UNUSED(pNewRootName);
        NN_UNUSED(pLayout);
        CopyImpl(textBox, pDevice);
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
    //! ペインツリーに登録するためには、明示的にペインに AppendChild() してください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! 新しく生成されるペインのために、font 用のコンスタントバッファを、
    //! レイアウト構築時に自動計算される requiredFontConstantBufferSize の値よりも、
    //! 多めに確保して作成しておく必要があります。
    //!
    //! キャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャなどの動的レンダリングテクスチャを使用している場合は
    //! このコピーコンストラクタを使用してコピーしてください。
    //! pLayout で指定したレイアウトクラスへ動的レンダリングテクスチャのリソースが登録されます。
    //!
    //! @param[in] textBox  コピー元のペインです。
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pLayout 動的レンダリングテクスチャリソースを登録するレイアウトインスタンス。通常、ペインの所属するレイアウトインスタンスを指定します。
    //!
    TextBox(const TextBox& textBox, nn::gfx::Device* pDevice, Layout* pLayout)
        : Pane(textBox, pDevice, pLayout)
    {
        CopyImpl(textBox, pDevice);
    }

    //! @brief バッファサイズを明示的に指定するコピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! この関数では、バッファサイズはコピーされず、指定したサイズが使われます。
    //! 文字バッファの内容もコピーされませんのでご注意ください。bufferStrLenに0が指定
    //! された場合は、バッファの確保が行われていない状態になります。
    //!
    //! また、コピー元のテキストボックスが文字属性バッファを共有する設定であった
    //! としても、指定したbuferStrlenがコピー元のテキストボックスの文字バッファ
    //! サイズよりも大きい場合は、コピー先は文字属性バッファを共有しない設定に
    //! なりますので、ご注意ください。
    //! レイアウトが保持している共有のバッファが不足する恐れがあるためです。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインに AppendChild() してください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! 新しく生成されるペインのために、font 用のコンスタントバッファを、
    //! レイアウト構築時に自動計算される requiredFontConstantBufferSize の値よりも、
    //! 多めに確保して作成しておく必要があります。
    //!
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Window::Window(const Pane& pane, nn::gfx::Device* pDevice, uint16_t bufferStrlen, Layout* pLayout) を使用してください。
    //!
    //! @param[in] textBox      コピー元のペインです。
    //! @param[in] pDevice      デバイスです。
    //! @param[in] bufferStrlen 確保する文字列用バッファの長さ(文字数)です。
    //!
    TextBox(const TextBox& textBox, nn::gfx::Device* pDevice, uint16_t  bufferStrlen)
        : Pane(textBox)
    {
        CopyImpl(textBox, pDevice, bufferStrlen);
    }

    //! @brief バッファサイズを明示的に指定するコピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! この関数では、バッファサイズはコピーされず、指定したサイズが使われます。
    //! 文字バッファの内容もコピーされませんのでご注意ください。bufferStrLenに0が指定
    //! された場合は、バッファの確保が行われていない状態になります。
    //!
    //! また、コピー元のテキストボックスが文字属性バッファを共有する設定であった
    //! としても、指定したbuferStrlenがコピー元のテキストボックスの文字バッファ
    //! サイズよりも大きい場合は、コピー先は文字属性バッファを共有しない設定に
    //! なりますので、ご注意ください。
    //! レイアウトが保持している共有のバッファが不足する恐れがあるためです。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインに AppendChild() してください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! 新しく生成されるペインのために、font 用のコンスタントバッファを、
    //! レイアウト構築時に自動計算される requiredFontConstantBufferSize の値よりも、
    //! 多めに確保して作成しておく必要があります。
    //!
    //! キャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャなどの動的レンダリングテクスチャを使用している場合は
    //! このコピーコンストラクタを使用してコピーしてください。
    //! pLayout で指定したレイアウトクラスへ動的レンダリングテクスチャのリソースが登録されます。
    //!
    //! @param[in] textBox      コピー元のペインです。
    //! @param[in] pDevice      デバイスです。
    //! @param[in] bufferStrlen 確保する文字列用バッファの長さ(文字数)です。
    //! @param[in] pLayout 動的レンダリングテクスチャリソースを登録するレイアウトインスタンス。通常、ペインの所属するレイアウトインスタンスを指定します。
    //!
    TextBox(const TextBox& textBox, nn::gfx::Device* pDevice, uint16_t  bufferStrlen, Layout* pLayout)
        : Pane(textBox, pDevice, pLayout)
    {
        CopyImpl(textBox, pDevice, bufferStrlen);
    }

    //! @brief バッファサイズを明示的に指定するコピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! この関数では、バッファサイズはコピーされず、指定したサイズが使われます。
    //! 文字バッファの内容もコピーされませんのでご注意ください。bufferStrLenに0が指定
    //! された場合は、バッファの確保が行われていない状態になります。
    //!
    //! また、コピー元のテキストボックスが文字属性バッファを共有する設定であった
    //! としても、指定したbuferStrlenがコピー元のテキストボックスの文字バッファ
    //! サイズよりも大きい場合は、コピー先は文字属性バッファを共有しない設定に
    //! なりますので、ご注意ください。
    //! レイアウトが保持している共有のバッファが不足する恐れがあるためです。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインに AppendChild() してください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! 新しく生成されるペインのために、font 用のコンスタントバッファを、
    //! レイアウト構築時に自動計算される requiredFontConstantBufferSize の値よりも、
    //! 多めに確保して作成しておく必要があります。
    //!
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Window::Window(const Pane& pane, nn::gfx::Device* pDevice, uint16_t bufferStrlen, Layout* pLayout) を使用してください。
    //!
    //! @param[in] textBox      コピー元のペインです。
    //! @param[in] pDevice      デバイスです。
    //! @param[in] bufferStrlen 確保する文字列用バッファの長さ(文字数)です。
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //!
    //! @deprecated ui2d ランタイムで動的レンダリングテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
    //!             動的レンダリングテクスチャ関連機能を使用したペインをコピーする場合は TextBox::TextBox(const TextBox& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    NN_DEPRECATED TextBox(const TextBox& textBox, nn::gfx::Device* pDevice, uint16_t  bufferStrlen, ResourceAccessor* pResAccessor, const char* pNewRootName)
        : Pane(textBox)
    {
        NN_UNUSED(pResAccessor);
        NN_UNUSED(pNewRootName);
        CopyImpl(textBox, pDevice, bufferStrlen);
    }

    //! @brief バッファサイズを明示的に指定するコピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! この関数では、バッファサイズはコピーされず、指定したサイズが使われます。
    //! 文字バッファの内容もコピーされませんのでご注意ください。bufferStrLenに0が指定
    //! された場合は、バッファの確保が行われていない状態になります。
    //!
    //! また、コピー元のテキストボックスが文字属性バッファを共有する設定であった
    //! としても、指定したbuferStrlenがコピー元のテキストボックスの文字バッファ
    //! サイズよりも大きい場合は、コピー先は文字属性バッファを共有しない設定に
    //! なりますので、ご注意ください。
    //! レイアウトが保持している共有のバッファが不足する恐れがあるためです。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインに AppendChild() してください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! 新しく生成されるペインのために、font 用のコンスタントバッファを、
    //! レイアウト構築時に自動計算される requiredFontConstantBufferSize の値よりも、
    //! 多めに確保して作成しておく必要があります。
    //!
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Window::Window(const Pane& pane, nn::gfx::Device* pDevice, uint16_t bufferStrlen, Layout* pLayout) を使用してください。
    //!
    //! @param[in] textBox      コピー元のペインです。
    //! @param[in] pDevice      デバイスです。
    //! @param[in] bufferStrlen 確保する文字列用バッファの長さ(文字数)です。
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //! @param[in] pLayout このペインを管理しているレイアウトです。
    //!
    //! @deprecated ui2d ランタイムで動的レンダリングテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
    //!             動的レンダリングテクスチャ関連機能を使用したペインをコピーする場合は TextBox::TextBox(const TextBox& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    NN_DEPRECATED TextBox(const TextBox& textBox, nn::gfx::Device* pDevice, uint16_t  bufferStrlen, ResourceAccessor* pResAccessor, const char* pNewRootName, const Layout* pLayout)
        : Pane(textBox)
    {
        NN_UNUSED(pResAccessor);
        NN_UNUSED(pNewRootName);
        NN_UNUSED(pLayout);
        CopyImpl(textBox, pDevice, bufferStrlen);
    }

    //! @brief デストラクタです。
    //!
    //! @details
    //! 設定されているフォントオブジェクトは破棄しません。
    //!
    //! @date 2010/01/26 フォントは破棄しない仕様に変更しました。
    virtual ~TextBox();

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
    //! @name 文字列操作
    //@{

    //! @brief 格納している文字列を取得します。UTF-16版。
    //!
    //! @return
    //! 格納している文字列の先頭アドレスを返します。
    //! 文字列用バッファが確保されていなければ NULL を返します。
    //!
    //! @sa SetString
    //! @sa GetStringLength
    //!
    const uint16_t* GetString() const
    {
        // 白黒カラーまで対応
        NN_SDK_ASSERT(!m_IsUtf8, "Invalid function for Utf8 Textbox! Use GetStringUtf8() instead. name[%s]", GetName());
        return m_TextBuf.utf16;
    }

    //! @brief 格納している文字列を取得します。UTF-8版。
    //!
    //! @return
    //! 格納している文字列の先頭アドレスを返します。
    //! 文字列用バッファが確保されていなければ NULL を返します。
    //!
    //! @sa SetString
    //! @sa GetStringLength
    //!
    const char* GetStringUtf8() const
    {
        NN_SDK_ASSERT(m_IsUtf8, "Invalid function for Utf16 Textbox! Use GetString() instead. name[%s]", GetName());
        return m_TextBuf.utf8;
    }

    //! @brief 文字列用バッファを取得します。UTF-16版。
    //!
    //! @details
    //! 文字列用バッファを確保するには AllocateStringBuffer() を呼び出してください。
    //!
    //! @return
    //! 文字列用バッファの先頭アドレスを返します。
    //! 文字列用バッファが確保されていなければ NULL を返します。
    //!
    //! @sa GetStringBufferLength
    //! @sa AllocateStringBuffer
    //!
    const uint16_t* GetStringBuffer() const
    {
        NN_SDK_ASSERT(!m_IsUtf8, "Invalid function for Utf8 Textbox! Use GetStringBufferUtf8() instead. name[%s]", GetName());
        return m_TextBuf.utf16;
    }

    //! @brief 文字列用バッファを取得します。UTF-8版。
    //!
    //! @details
    //! 文字列用バッファを確保するには AllocateStringBuffer() を呼び出してください。
    //!
    //! @return
    //! 文字列用バッファの先頭アドレスを返します。
    //! 文字列用バッファが確保されていなければ NULL を返します。
    //!
    //! @sa GetStringBufferLength
    //! @sa AllocateStringBuffer
    //!
    const char* GetStringBufferUtf8() const
    {
        NN_SDK_ASSERT(m_IsUtf8, "Invalid function for Utf16 Textbox! Use GetStringBuffer() instead. name[%s]", GetName());
        return m_TextBuf.utf8;
    }

    //! @brief 格納している文字列の文字数を取得します。
    //!
    //! @return 格納している文字列の長さ(文字数)を返します。
    //!
    //! @sa SetString
    //! @sa GetString
    //!
    uint16_t  GetStringLength() const
    {
        return m_TextLen;
    }

    //! @brief 文字列用バッファの格納可能な文字数を取得します。
    //!
    //! @return 文字列用バッファの長さを返します。
    //!
    //! @sa GetStringBuffer
    //! @sa AllocateStringBuffer
    //!
    uint16_t  GetStringBufferLength() const;

    //! @brief 描画用文字列バッファの長さを取得します。
    //!
    //! @return 描画用文字列バッファの長さを返します。
    //!
    //! @sa AllocateStringBuffer
    //!
    uint16_t  GetDrawStringBufferLength() const;

    //! @brief 文字列用バッファを確保します。
    //!
    //! @details
    //! 既に確保しているバッファの長さが minLen で指定した
    //! 長さより大きい場合は何もしません。
    //!
    //! 文字列バッファの長さと描画する際に必要なバッファの長さは
    //! 同じ値が指定されます。
    //!
    //! UTF-16 の場合は 1 文字あたり 2byte、UTF-8 の場合は 1 文字あたり 4byte 確保されます。
    //!
    //! @param[in] pDevice       デバイスです。
    //! @param[in] minLen   確保する文字列用バッファの長さ(文字数)です。
    //!
    //! @sa FreeStringBuffer
    //! @sa GetStringBufferLength
    //!
    //! @date 2016/04/04 引数 isShareCharAttributeBuffer を削除しました。
    //! @date 2010/05/10 引数 isDoubleBuffer を削除しました。
    //! @date 2010/02/19 引数 isDoubleBuffer を追加しました。
    virtual void AllocateStringBuffer(
        nn::gfx::Device* pDevice,
        uint16_t  minLen);

    //! @brief 文字列用バッファを確保します。
    //!
    //! @details
    //! 引数一つのバージョンと異なり、文字列バッファの長さと、実際に
    //! 描画する際に必要なバッファの長さを分けて指定することができます。
    //!
    //! UTF-16 の場合は 1 文字あたり 2byte、UTF-8 の場合は 1 文字あたり 4byte 確保されます。
    //!
    //! @param[in] pDevice       デバイスです。
    //! @param[in] minStrLen     確保する文字列用バッファの長さ(文字数)です。
    //! @param[in] minDrawStrLen 確保する描画用文字列バッファの長さ(文字数)です。
    //!
    //! @sa FreeStringBuffer
    //! @sa GetStringBufferLength
    //! @sa GetDrawStringBufferLength
    //!
    virtual void AllocateStringBuffer(nn::gfx::Device* pDevice,uint16_t  minStrLen, uint16_t  minDrawStrLen);

    //! @brief 文字列用バッファを解放します。
    //!
    //! @param[in] pDevice       デバイスです。
    //!
    //! @sa AllocateStringBuffer
    //!
    virtual void FreeStringBuffer(nn::gfx::Device* pDevice);

    //! @brief 文字列バッファに文字列を格納します。UTF-16版。
    //!
    //! @details
    //! 指定した文字列の長さが文字列用バッファの長さを超える場合は、
    //! 文字列用バッファの長さを超える分が切り捨てられます。
    //!
    //! @param[in] pStr     格納する文字列です。
    //! @param[in] dstIdx   格納位置です。
    //!
    //! @return 格納した文字列の長さを返します。
    //!
    //! @sa GetString
    //! @sa GetStringLength
    //!
    virtual uint16_t  SetString(
        const uint16_t*  pStr,
        uint16_t              dstIdx = 0);

    //! @brief 文字列バッファに文字列を格納します。UTF-8版。
    //!
    //! @details
    //! 指定した文字列の長さが文字列用バッファの長さを超える場合は、
    //! 文字列用バッファの長さを超える分が切り捨てられます。
    //!
    //! @param[in] pStr     格納する文字列です。
    //! @param[in] dstIdx   格納位置です。
    //!
    //! @return 格納した文字列の長さを返します。
    //!
    //! @sa GetString
    //! @sa GetStringLength
    //!
    virtual uint16_t  SetStringUtf8(
        const char*  pStr,
        uint16_t              dstIdx = 0);

    //! @brief 文字列バッファに文字列を格納します。UTF-16版。
    //!
    //! @details
    //! 文字列の長さは strLen で指定された値です。終端文字を判断しません。
    //!
    //! 指定した文字列の長さが文字列用バッファの長さを超える場合は、
    //! 文字列用バッファの長さを超える分が切り捨てられます。
    //!
    //! @param[in] pStr     格納する文字列です。
    //! @param[in] dstIdx   格納位置です。
    //! @param[in] strLen   格納する文字列の長さです。
    //!
    //! @return 格納した文字列の長さを返します。
    //!
    //! @sa GetString
    //! @sa GetStringLength
    //!
    //! @date 2010/04/09 strLen の値が 0 だった場合の記述に誤りがありました。値 0 は終端文字までを意味しません。
    virtual uint16_t  SetString(
        const uint16_t*  pStr,
        uint16_t              dstIdx,
        uint16_t              strLen);

    //! @brief 文字列バッファに文字列を格納します。UTF-8版。
    //!
    //! @details
    //! 文字列の長さは strLen で指定された値です。終端文字を判断しません。
    //!
    //! 指定した文字列の長さが文字列用バッファの長さを超える場合は、
    //! 文字列用バッファの長さを超える分が切り捨てられます。
    //!
    //! @param[in] pStr     格納する文字列です。
    //! @param[in] dstIdx   格納位置です。
    //! @param[in] strLen   格納する文字列のバイト数です。
    //!
    //! @return 格納した文字列の長さを返します。
    //!
    //! @sa GetString
    //! @sa GetStringLength
    //!
    //! @date 2010/04/09 strLen の値が 0 だった場合の記述に誤りがありました。値 0 は終端文字までを意味しません。
    virtual uint16_t  SetStringUtf8(
        const char*  pStr,
        uint16_t              dstIdx,
        uint16_t              strLen);

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief テキストの表示色を取得します。
    //!
    //! @param[in] type 表示色の種類です。 nn::ui2d::TextColor を指定します。
    //!
    //! @return 表示色を返します。
    //!
    //! @sa SetTextColor
    //! @sa nn::ui2d::TextColor
    //!
    const nn::util::Unorm8x4& GetTextColor(int  type) const
    {
        NN_SDK_ASSERT(type < TextColor_MaxTextColor, "out of bounds: type[%u] < TextColor_MaxTextColor for TextBox[%s]", type, GetName());
        return m_TextColors[type];
    }

    //! @brief テキストの表示色を設定します。
    //!
    //! @details
    //! 指定の表示色を設定します。
    //!
    //! @param[in] type     表示色の種類です。 nn::ui2d::TextColor を指定します。
    //! @param[in] value    表示色です。
    //!
    //! @sa GetTextColor
    //! @sa nn::ui2d::TextColor
    //!
    void SetTextColor(int  type, const nn::util::Unorm8x4& value)
    {
        NN_SDK_ASSERT(type < TextColor_MaxTextColor, "out of bounds: type[%u] < TextColor_MaxTextColor for TextBox[%s]", type, GetName());

        UpdatePTDirty(m_TextColors[type], value);
        m_TextColors[type] = value;
    }

    //! @brief テキストの表示色を設定します。
    //!
    //! @details
    //! テキストの表示色の上端と下端を同時に設定します。
    //!
    //! @param[in] top      上端の表示色です。
    //! @param[in] bottom   下端の表示色です。
    //!
    //! @sa GetTextColor
    //!
    void SetTextColor(
        const nn::util::Unorm8x4& top,
        const nn::util::Unorm8x4& bottom
    )
    {
        if (UpdatePTDirty(m_TextColors[TextColor_Top], top) || UpdatePTDirty(m_TextColors[TextColor_Bottom], bottom))
        {
            m_TextColors[TextColor_Top   ] = top;
            m_TextColors[TextColor_Bottom] = bottom;
        }
    }

    //! @brief テキストのフォントを取得します。
    //!
    //! @return フォントです。
    //!
    //! @sa SetFont
    //!
    //! @date 2010/01/26 フォントは破棄しない仕様に変更しました。
    const nn::font::Font* GetFont() const;

    //! @brief テキストのフォントを設定します。
    //!
    //! @details
    //! 同時に、フォントサイズを指定されたフォントオブジェクトのサイズに設定します。
    //!
    //! NULL を渡した場合はフォントの設定を解除します。
    //!
    //! @param[in] pFont    フォントオブジェクトへのポインタです。
    //!
    //! @sa GetFont
    //!
    //! @date 2010/01/26 フォントは破棄しない仕様に変更しました。
    void SetFont(const nn::font::Font* pFont);

    //! @brief テキストのフォントサイズを取得します。
    //!
    //! @return テキストのフォントサイズを返します。
    //!
    //! @sa SetFontSize
    //!
    const Size& GetFontSize() const
    {
        return m_FontSize;
    }

    //! @brief テキストのフォントサイズを設定します。
    //!
    //! @param[in] fontSize フォントサイズです。
    //!
    //! @sa GetFontSize
    //!
    void SetFontSize(const Size& fontSize);

    //! @brief テキストの行間隔を取得します。
    //!
    //! @return テキストの行間隔を返します。
    //!
    //! @sa SetLineSpace
    //!
    float GetLineSpace() const
    {
        return m_LineSpace;
    }

    //! @brief テキストの行間隔を設定します。
    //!
    //! @param[in] space    行間です。
    //!
    //! @sa GetLineSpace
    //!
    void SetLineSpace(float space)
    {
        if (UpdatePTDirty(m_LineSpace != space))
        {
            m_LineSpace = space;
        }
    }

    //! @brief テキストの文字間隔を取得します。
    //!
    //! @return テキストの文字間隔を返します。
    //!
    //! @sa SetCharSpace
    //!
    float GetCharSpace() const
    {
        return m_CharSpace;
    }

    //! @brief テキストの文字間隔を設定します。
    //!
    //! @param[in] space    文字間隔です。
    //!
    //! @sa GetCharSpace
    //!
    void SetCharSpace(float space)
    {
        if (UpdatePTDirty(m_CharSpace != space))
        {
            m_CharSpace = space;
        }
    }

    //! @brief テキスト表示基準位置設定の水平位置指定を取得します。
    //!
    //! @return テキスト表示基準位置設定の水平位置指定を返します。
    //!
    //! @sa SetTextPositionX
    //! @sa GetTextPositionY
    //!
    HorizontalPosition GetTextPositionX() const
    {
        return detail::GetHorizontalPosition(m_TextPosition);
    }

    //! @brief テキスト表示基準位置設定の水平位置指定を設定します。
    //!
    //! @param[in] val  テキスト表示基準位置指定です。
    //!
    //! @sa GetTextPositionX
    //! @sa SetTextPositionY
    //!
    void SetTextPositionX(HorizontalPosition val)
    {
        if (UpdatePTDirty(GetTextPositionX() != val))
        {
            detail::SetHorizontalPosition(&m_TextPosition, uint8_t (val));
        }
    }

    //! @brief テキスト表示基準位置設定の垂直位置指定を取得します。
    //!
    //! @return テキスト表示基準位置設定の垂直位置指定を返します。
    //!
    //! @sa SetTextPositionY
    //! @sa GetTextPositionX
    //!
    VerticalPosition GetTextPositionY() const
    {
        if (m_Bits.perCharacterTransformOriginToCenter)
        {
            // 円形配置の機能を使っているときは文字揃えを中央に固定する。
            return VerticalPosition_Center;
        }
        return detail::GetVerticalPosition(m_TextPosition);
    }

    //! @brief テキスト表示基準位置設定の垂直位置指定を設定します。
    //!
    //! @param[in] val  テキスト表示基準位置指定です。
    //!
    //! @sa GetTextPositionY
    //! @sa SetTextPositionX
    //!
    void SetTextPositionY(VerticalPosition val)
    {
        if (UpdatePTDirty(GetTextPositionY() != val))
        {
            detail::SetVerticalPosition(&m_TextPosition, uint8_t (val));
        }
    }

    //! @brief 行そろえ指定を取得します。
    //!
    //! @return 行そろえ指定を返します。
    //!
    //! @sa SetTextAlignment
    //!
    TextAlignment GetTextAlignment() const
    {
        return static_cast<TextAlignment>(m_Bits.textAlignment);
    }

    //! @brief 行そろえ指定を設定します。
    //!
    //! @param[in] val  行そろえの指定です。
    //!
    //! @sa GetTextAlignment
    //!
    void SetTextAlignment(TextAlignment val)
    {
        if (UpdatePTDirty(GetTextAlignment() != val))
        {
            m_Bits.textAlignment = val;
        }
    }

    //! @brief タグプロセッサを取得します。UTF-16版。
    //!
    //! @return
    //! タグプロセッサを返します。
    //! タグプロセッサが設定されていない場合は NULL を返します。
    //!
    //! @sa SetTagProcessor
    //!
    TagProcessor* GetTagProcessor() const
    {
        NN_SDK_ASSERT(!m_IsUtf8, "Invalid function for Utf8 Textbox! Use GetTagProcessorUtf8() instead. name[%s]", GetName());

        return m_pTagProcessor.utf16;
    }

    //! @brief タグプロセッサを取得します。UTF-8版。
    //!
    //! @return
    //! タグプロセッサを返します。
    //! タグプロセッサが設定されていない場合は NULL を返します。
    //!
    //! @sa SetTagProcessor
    //!
    TagProcessorUtf8* GetTagProcessorUtf8() const
    {
        NN_SDK_ASSERT(m_IsUtf8, "Invalid function for Utf16 Textbox! Use GetTagProcessor() instead. name[%s]", GetName());
        return m_pTagProcessor.utf8;
    }

    //! @brief タグプロセッサを設定します。UTF-16版。
    //!
    //! @details
    //! NULL を渡した場合はタグプロセッサの設定を解除します。
    //!
    //! @param[in] pTagProcessor    タグプロセッサへのポインタです。
    //!
    //! @sa GetTagProcessor
    //!
    void SetTagProcessor(TagProcessor* pTagProcessor)
    {
        NN_SDK_ASSERT(!m_IsUtf8, "Invalid function for Utf8 Textbox! Use SetTagProcessorUtf8() instead. name[%s]", GetName());

        if (UpdatePTDirty(m_pTagProcessor.utf16 != pTagProcessor))
        {
            m_pTagProcessor.utf16 = pTagProcessor;
        }
    }

    //! @brief タグプロセッサを設定します。UTF-8版。
    //!
    //! @details
    //! NULL を渡した場合はタグプロセッサの設定を解除します。
    //!
    //! @param[in] pTagProcessor    タグプロセッサへのポインタです。
    //!
    //! @sa GetTagProcessor
    //!
    void SetTagProcessorUtf8(TagProcessorUtf8* pTagProcessor)
    {
        NN_SDK_ASSERT(m_IsUtf8, "Invalid function for Utf16 Textbox! Use SetTagProcessor() instead. name[%s]", GetName());
        if (UpdatePTDirty(m_pTagProcessor.utf8 != pTagProcessor))
        {
            m_pTagProcessor.utf8 = pTagProcessor;
        }
    }

    //! @brief テキストの表示色（頂点カラー）を取得します。
    //!
    //! @details
    //! idx が nn::ui2d::VertexColor_LeftTop のときは上端の色、
    //! idx が nn::ui2d::VertexColor_LeftBottom のときは下端の色を返します。
    //!
    //! idx が nn::ui2d::VertexColor_RightTop または nn::ui2d::VertexColor_RightBottom のときは
    //! 対応する頂点カラーを返しますが表示には使用されていません。
    //!
    //! @param[in] idx  インデックスです。 nn::ui2d::VertexColor を指定します。
    //!
    //! @return テキストの表示色を返します。
    //!
    //! @sa SetVertexColor
    //! @sa GetVertexColorElement
    //! @sa nn::ui2d::VertexColor
    //!
    virtual const nn::util::Unorm8x4 GetVertexColor(int  idx) const NN_OVERRIDE;

    //! @brief テキストの表示色（頂点カラー）を設定します。
    //!
    //! @details
    //! idx が nn::ui2d::VertexColor_LeftTop のときは上端の色を、
    //! idx が nn::ui2d::VertexColor_LeftBottom のときは下端の色を変更します。
    //!
    //! idx が nn::ui2d::VertexColor_RightTop、 nn::ui2d::VertexColor_RightBottom のときは
    //! 対応する頂点カラーを変更しますが表示には使用されていません。
    //!
    //! @param[in] idx      インデックスです。 nn::ui2d::VertexColor を指定します。
    //! @param[in] value    テキストの表示色です。
    //!
    //! @sa GetVertexColor
    //! @sa SetVertexColorElement
    //! @sa nn::ui2d::VertexColor
    //!
    virtual void SetVertexColor(int  idx, const nn::util::Unorm8x4& value) NN_OVERRIDE;

    //! @brief テキストの表示色（頂点カラー）の成分を取得します。
    //!
    //! @details
    //! idx が nn::ui2d::AnimTargetPaneColor_VertexLt* のときは上端の色、
    //! idx が nn::ui2d::AnimTargetPaneColor_VertexLb* のときは下端の色の各成分を返します。
    //!
    //! idx が nn::ui2d::AnimTargetPaneColor_VertexRt*、 nn::ui2d::AnimTargetPaneColor_VertexRb* のときは
    //! 対応する頂点カラーの各成分を返しますが表示には使用されていません。
    //!
    //! このメソッドは、R,G,B など色成分を個別に取得します。色成分をまとめて取得する場合は GetVertexColor を利用してください。
    //!
    //! @param[in] idx  インデックスです。 nn::ui2d::AnimTargetPaneColor を指定します。
    //!
    //! @return テキストの表示色の成分を返します。
    //!
    //! @sa SetVertexColorElement
    //! @sa GetVertexColor
    //! @sa nn::ui2d::AnimTargetPaneColor
    //!
    virtual uint8_t  GetVertexColorElement(int  idx) const NN_OVERRIDE;

    //! @brief テキストの表示色（頂点カラー）の成分を設定します。
    //!
    //! @details
    //! idx が nn::ui2d::AnimTargetPaneColor_VertexLt* のときは上端の色、
    //! idx が nn::ui2d::AnimTargetPaneColor_VertexLb* のときは下端の色の各成分を変更します。
    //!
    //! idx が nn::ui2d::AnimTargetPaneColor_VertexRt*、 nn::ui2d::AnimTargetPaneColor_VertexRb* のときは
    //! 対応する頂点カラーの各成分を変更しますが表示には使用されていません。
    //!
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

    //! @brief テキストが描画される矩形を取得します。
    //!
    //! @return 矩形を返します。
    //!
    const nn::font::Rectangle GetTextDrawRect() const;

    using Base::GetMaterial;

    //! @brief ペインが持つマテリアルの数を取得します。
    //!
    //! @return マテリアルの数を返します。
    //!
    virtual uint8_t  GetMaterialCount() const NN_OVERRIDE;

    //! @brief ペインのマテリアルを取得します。
    //!
    //! @details
    //! idx には 0 のみ指定できます。
    //!
    //! @param[in] idx  インデックスです。
    //!
    //! @return マテリアルへのポインタを返します。
    //!
    //! @sa SetMaterial
    //! @sa UnsetMaterial
    //!
    virtual Material* GetMaterial(int  idx) const NN_OVERRIDE;

    //! @brief マテリアルを設定します。
    //!
    //! @details
    //! 現在設定されているマテリアルが TextBox オブジェクトの
    //! 生成時に同時に生成されたものだった場合には、
    //! そのマテリアルは破棄されます。
    //!
    //! @param[in] pMaterial    マテリアルへのポインタです。
    //!
    //! @sa GetMaterial
    //! @sa UnsetMaterial
    //!
    void SetMaterial(Material* pMaterial);

    //! @brief マテリアルの設定を解除します。
    //!
    //! @details
    //! マテリアルを破棄することなく、マテリアルの設定を解除します。
    //! TextBoxの解放処理の直前で呼ぶことで、
    //! デストラクタ内でマテリアルが解放されることを防ぎます。
    //!
    //! @sa GetMaterial
    //! @sa SetMaterial
    //!
    void UnsetMaterial();

    //! @brief テキストIDを取得します。
    //!
    //! @details
    //! リソースでテキストIDが指定されていた場合は、構築後に
    //! このメソッドでテキストIDを取得することができます。
    //!
    //! テキストIDが指定されていなかった場合はNULLが返されます。
    //!
    //! @return テキストID。null終端されています。
    //!
    const char* GetTextId() const { return m_pTextId; }

    //! @brief テキストボックスの幅で折り返す処理が有効か否かを取得します。
    //!
    //! @return テキストボックスの幅で折り返す処理が有効か否か
    //!
    bool IsWidthLimitEnabled() const { return m_Bits.widthLimitEnabled; }

    //! @brief テキストボックスの幅で折り返す処理が有効か否かを設定します。
    //!
    //! @details
    //! デフォルトでは有効になっています。
    //!
    //! @param[in] enabled   テキストボックスの幅で折り返す処理を有効にするか否か
    //!
    void SetWidthLimitEnabled(bool enabled) { m_Bits.widthLimitEnabled = enabled; }

    //! @brief 斜体の度合いを比率で取得します。
    //!
    //! @details
    //! フォントの幅に対する斜体の比率です。
    //! 例えば 1.0f の場合、フォントの幅の分だけ右に傾きます。
    //!
    //! @return 斜体の度合いの比率を返します。
    //!
    //! @sa SetItalicRatio
    //!
    float GetItalicRatio() const
    {
        return m_ItalicRatio;
    }

    //! @brief 斜体の度合いを比率で設定します。
    //!
    //! @details
    //! フォントの幅に対する斜体の比率です。
    //! 例えば 1.0f の場合、フォントの幅の分だけ右に傾きます。
    //!
    //! @param[in] ratio    斜体の度合いの比率です。
    //!
    //! @sa GetItalicRatio
    //!
    void SetItalicRatio(float ratio)
    {
        if (UpdatePTDirty(m_ItalicRatio != ratio))
        {
            m_ItalicRatio = ratio;
        }
    }

    //! @brief 斜体の度合いを角度で取得します。
    //!
    //! @return 斜体の度合いを角度(ラジアン)で返します。
    //!
    //! @sa SetAngleToItalicRatio
    //!
    float GetAngleFromItalicRatio() const;

    //! @brief 斜体の度合いを角度で設定します。
    //!
    //! @param[in] angle 斜体の度合いを角度(ラジアン)で設定します。
    //!
    //! @sa GetAngleFromItalicRatio
    //!
    void SetAngleToItalicRatio(float angle);

    //! @brief 影のオフセットを取得します。
    //!
    //! @return 影のオフセットを返します。
    //!
    //! @sa SetShadowOffset
    //!
    const nn::util::Float2& GetShadowOffset() const
    {
        return m_ShadowOffset;
    }

    //! @brief 影のオフセットを設定します。
    //!
    //! @param[in] offset    影のオフセットです。
    //!
    //! @sa GetShadowOffset
    //!
    void SetShadowOffset(const nn::util::Float2& offset)
    {
        if (UpdatePTDirty(m_ShadowOffset.v[0] != offset.v[0] || m_ShadowOffset.v[1] != offset.v[1]))
        {
            m_ShadowOffset = offset;
        }
    }

    //! @brief 影のスケールを取得します。
    //!
    //! @return 影のスケールを返します。
    //!
    //! @sa SetShadowScale
    //!
    const nn::util::Float2& GetShadowScale() const
    {
        return m_ShadowScale;
    }

    //! @brief 影のスケールを設定します。
    //!
    //! @param[in] scale    影のスケールです。
    //!
    //! @sa GetShadowScale
    //!
    void SetShadowScale(const nn::util::Float2& scale)
    {
        if (UpdatePTDirty(m_ShadowScale.v[0] != scale.v[0] || m_ShadowScale.v[1] != scale.v[1]))
        {
            m_ShadowScale = scale;
        }
    }

    //! @brief 影の上端の表示色を取得します。
    //!
    //! @return 影の上端の表示色を返します。
    //!
    //! @sa SetShadowTopColor
    //!
    const nn::util::Unorm8x4& GetShadowTopColor() const
    {
        return m_ShadowTopColor;
    }

    //! @brief 影の上端の表示色を設定します。
    //!
    //! @param[in] color    影の上端の表示色です。
    //!
    //! @sa GetShadowTopColor
    //!
    void SetShadowTopColor(const nn::util::Unorm8x4& color)
    {
        if (UpdatePTDirty(m_ShadowTopColor, color))
        {
            m_ShadowTopColor = color;
        }
    }

    //! @brief 影の下端の表示色を取得します。
    //!
    //! @return 影の下端の表示色を返します。
    //!
    //! @sa SetShadowBottomColor
    //!
    const nn::util::Unorm8x4& GetShadowBottomColor() const
    {
        return m_ShadowBottomColor;
    }

    //! @brief 影の下端の表示色を設定します。
    //!
    //! @param[in] color    影の下端の表示色です。
    //!
    //! @sa GetShadowBottomColor
    //!
    void SetShadowBottomColor(const nn::util::Unorm8x4& color)
    {
        if (UpdatePTDirty(m_ShadowBottomColor, color))
        {
            m_ShadowBottomColor = color;
        }
    }

    //! @brief 影の表示色を設定します。
    //!
    //! @param[in] top       影の上端の表示色です。
    //! @param[in] bottom    影の下端の表示色です。
    //!
    void SetShadowColor(const nn::util::Unorm8x4& top, const nn::util::Unorm8x4& bottom)
    {
        if (UpdatePTDirty(m_ShadowTopColor, top) || UpdatePTDirty(m_ShadowBottomColor, bottom))
        {
            m_ShadowTopColor = top;
            m_ShadowBottomColor = bottom;
        }
    }

    //! @brief 影の斜体の度合いを取得します。
    //!
    //! @return 影の斜体の度合いを返します。
    //!
    //! @sa SetShadowItalicRatio
    //!
    float GetShadowItalicRatio() const
    {
        return m_ShadowItalicRatio;
    }

    //! @brief 影の斜体の度合いを設定します。
    //!
    //! @param[in] ratio    影の斜体の度合いです。
    //!
    //! @sa GetShadowItalicRatio
    //!
    void SetShadowItalicRatio(float ratio)
    {
        if (UpdatePTDirty(m_ShadowItalicRatio != ratio))
        {
            m_ShadowItalicRatio = ratio;
        }
    }

    //! @brief 影の斜体の度合いを角度で取得します。
    //!
    //! @return 影の斜体の度合いを角度(ラジアン)で返します。
    //!
    //! @sa SetAngleToShadowItalicRatio
    //!
    float GetAngleFromShadowItalicRatio() const;

    //! @brief 影の斜体の度合いを角度で設定します。
    //!
    //! @param[in] angle 影の斜体の度合いを角度(ラジアン)で設定します。
    //!
    //! @sa GetAngleFromShadowItalicRatio
    //!
    void SetAngleToShadowItalicRatio(float angle);

    //! @brief ダーティフラグを設定します。
    //!
    //! @details
    //! ペインのサイズ変更など、テキストを再配置する場合などに
    //! isDirty に true を設定してください。
    //!
    //! isDirty 引数に true を設定することで、
    //! 描画前にテキスト用頂点バッファのキャッシュを作成し直します。
    //!
    //! @param[in] isDirty  true を設定すると頂点バッファのキャッシュを作成し直します。
    //!
    void SetDirtyFlag(bool isDirty)
    {
        UpdatePTDirty(isDirty);
    }

    //! @brief ダーティフラグを false にします。
    //!
    //! @details
    //! ダーティフラグを false にすることで、
    //! Calculate() において頂点バッファのキャッシュを作成し直さなくなります。
    void ClearDirtyFlag() { m_Bits.isPTDirty = 0; }

    //! @brief ダーティフラグを取得します。
    //!
    //! @details
    //! Calculate() において頂点バッファのキャッシュを作成し直す場合 true が返ります。
    //!
    //! @return ダーティフラグです。
    //!
    bool GetDirtyFlag() const { return m_Bits.isPTDirty; }

    //! @brief TextWriter の設定を行います。UTF-16版。
    //!
    //! @param[out] pWriter  文字列の描画に必要な設定が行われます。
    //!
    virtual void SetupTextWriter(nn::font::WideTextWriter* pWriter);

    //! @brief TextWriter の設定を行います。UTF-8版。
    //!
    //! @param[out] pWriter  文字列の描画に必要な設定が行われます。
    //!
    virtual void SetupTextWriterUtf8(nn::font::TextWriter* pWriter);

    //! @brief 文字単位アニメーションのオフセットを設定します。
    //!
    //! @param[in] offset オフセットを指定します。
    //!
    void SetPerCharacterTransformOffset(float offset)
    {
        SetPerCharacterTransform(AnimTargetPerCharacterTransform_EvalTypeOffset, offset);
    }

    //! @brief 文字単位アニメーションのオフセットや時間幅を設定します。
    //!
    //! @param[in] idx 成分のインデックスです。
    //! @param[in] value 成分の値です。
    //!
    void SetPerCharacterTransform(int idx, float value);

    //! @brief 文字単位アニメーションのオフセットや時間幅を取得します。
    //!
    //! @param[in] idx 成分のインデックスです。
    //!
    //! @return 成分の値です。
    //!
    float GetPerCharacterTransform(int idx) const;

    //! @brief UTF-8 モードで初期化されたかどうかを取得します。
    //!
    //! @return UTF-8 モードで初期化されている場合は true が、それ以外の場合は false が返ります。
    //!
    bool GetUtf8() const
    {
        return m_IsUtf8;
    }

    //! @brief 特殊フチ形式のフチを非表示にするかどうかを設定します。
    //!
    //! @details
    //! SetDoubleDrawnBorderEnabled を true に設定した場合、SetInvisibleBorderEnabled の設定は
    //! 無視されて、必ずフチが 2 回に分けて描かれます。
    //!
    //! @param[in] isInvisibleBorderEnabled 特殊フチ形式のフチを非表示にするかどうかです。
    //!
    void SetInvisibleBorderEnabled(bool isInvisibleBorderEnabled)
    {
        m_Bits.invisibleBorderEnabled = isInvisibleBorderEnabled ? 1 : 0;
    }

    //! @brief 特殊フチ形式のフチを非表示にするかどうかを取得します。
    //!
    //! @return 特殊フチ形式のフチを非表示にするかどうかです。
    //!
    bool IsInvisibleBorderEnabled() const
    {
        return m_Bits.invisibleBorderEnabled != 0;
    }

    //! @brief 特殊フチ形式のフチを 2 回に分けて描くかどうかを設定します。
    //!
    //! @details
    //! 「フチを 2 回に分けて描く」に設定していなかったテキストボックスペインに対して
    //! SetDoubleDrawnBorderEnabled を true にした場合、コンスタントバッファの消費サイズが
    //! 増えるため、あらかじめ nn::ui2d::BuildResultInformation::requiredFontConstantBufferSize で
    //! 計算した領域をオーバーすることがあります。この問題を防ぐために、
    //! SetDoubleDrawnBorderEnabled を切り替えるテキストボックスペインにはあらかじめ
    //! 「フチを 2 回に分けて描く」に設定して初期化してください。
    //!
    //! @param[in] isDoubleDrawnBorderEnabled 特殊フチ形式のフチを 2 回に分けて描くかどうかです。
    //!
    void SetDoubleDrawnBorderEnabled(bool isDoubleDrawnBorderEnabled)
    {
        m_Bits.doubleDrawnBorderEnabled = isDoubleDrawnBorderEnabled ? 1 : 0;
        if (m_pDispStringBuf != NULL)
        {
            m_pDispStringBuf->SetDoubleDrawnBorderEnabled(isDoubleDrawnBorderEnabled);
        }
    }

    //! @brief 特殊フチ形式のフチを 2 回に分けて描くかどうかを取得します。
    //!
    //! @return 特殊フチ形式のフチを 2 回に分けて描くかどうかです。
    //!
    bool IsDoubleDrawnBorderEnabled() const
    {
        return m_Bits.doubleDrawnBorderEnabled != 0;
    }

    //! @brief 中央揃えのにじみ防止機能が有効かどうかを設定します。
    //!
    //! @param[in] isCenterCeilingEnabled 中央揃えのにじみ防止機能が有効かどうかです。
    //!
    void SetCenterCeilingEnabled(bool isCenterCeilingEnabled)
    {
        m_Bits.centerCeilingEnabled = isCenterCeilingEnabled;
    }

    //! @brief 中央揃えのにじみ防止機能が有効かどうかを取得します。
    //!
    //! @return 中央揃えのにじみ防止機能が有効かどうかです。
    //!
    bool IsCenterCeilingEnabled() const
    {
        return m_Bits.centerCeilingEnabled;
    }

    //! @brief 右側の文字から描画するかどうかを設定します。
    //!
    //! @param[in] isDrawFromRightToLeftEnabled 右側の文字から描画するかどうかです。
    //!
    void SetDrawFromRightToLeftEnabled(bool isDrawFromRightToLeftEnabled)
    {
        m_Bits.drawFromRightToLeft = isDrawFromRightToLeftEnabled;
    }

    //! @brief 右側の文字から描画するかどうかを取得します。
    //!
    //! @return 右側の文字から描画するかどうかです。
    //!
    bool IsDrawFromRightToLeftEnabled() const
    {
        return m_Bits.drawFromRightToLeft;
    }

    //@}

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return 表示文字列バッファです。
    //!
    nn::font::DispStringBuffer* GetDispStringBuffer() const
    {
        return m_pDispStringBuf;
    }

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[out] pMtx    行列です。
    //! @param[in]  drawInfo    描画の情報です。
    //!
    void GetTextGlobalMtx(nn::util::MatrixT4x3fType* pMtx, const DrawInfo& drawInfo) const;

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
    bool CompareCopiedInstanceTest(const TextBox& target) const;

protected:
    virtual void DrawSelf(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer) NN_OVERRIDE;

    virtual void LoadMtx(DrawInfo& drawInfo) NN_OVERRIDE;

    virtual bool InitializeStringWithTextSearcherInfo(nn::gfx::Device* pDevice, const BuildArgSet& buildArgSet, const TextSearcher::TextInfo& textInfo);
    virtual bool InitializeStringWithTextSearcherInfoUtf8(nn::gfx::Device* pDevice, const BuildArgSet& buildArgSet, const TextSearcher::TextInfoUtf8& textInfo);

    void InitializePerCharacterTransformCurves(const ResAnimationInfo* animInfo);

    //! @brief コピーコンストラクタの実装です。
    //!
    //! @param[in] textBox コピー元のテキストボックスペインです。
    //! @param[in] pDevice デバイスです。
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //!
    //! @deprecated ui2d ランタイムでキャプチャテクスチャリソース管理方法が変更されたため、このコピーコンストラクタ実装は非推奨となりました。
    //!
    NN_DEPRECATED void CopyImpl(const TextBox& textBox, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pNewRootName)
    {
        NN_UNUSED(pResAccessor);
        NN_UNUSED(pNewRootName);
        CopyImpl(textBox, pDevice);
    }

    //! @brief コピーコンストラクタの実装です。
    //!
    //! @param[in] textBox コピー元のテキストボックスペインです。
    //! @param[in] pDevice デバイスです。
    //!
    void CopyImpl(const TextBox& textBox, nn::gfx::Device* pDevice);

    //! @brief コピーコンストラクタの実装です。
    //!
    //! @param[in] textBox コピー元のテキストボックスペインです。
    //! @param[in] pDevice デバイスです。
    //! @param[in] bufferStrlen 確保する文字列用バッファの長さ(文字数)です。
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //!
    //! @deprecated ui2d ランタイムでキャプチャテクスチャリソース管理方法が変更されたため、このコピーコンストラクタ実装は非推奨となりました。
    //!
    NN_DEPRECATED void CopyImpl(const TextBox& textBox, nn::gfx::Device* pDevice, uint16_t bufferStrlen, ResourceAccessor* pResAccessor, const char* pNewRootName)
    {
        NN_UNUSED(pResAccessor);
        NN_UNUSED(pNewRootName);
        CopyImpl(textBox, pDevice, bufferStrlen);
    }

    //! @brief コピーコンストラクタの実装です。
    //!
    //! @param[in] textBox コピー元のテキストボックスペインです。
    //! @param[in] pDevice デバイスです。
    //! @param[in] bufferStrlen 確保する文字列用バッファの長さ(文字数)です。
    //!
    void CopyImpl(const TextBox& textBox, nn::gfx::Device* pDevice, uint16_t bufferStrlen);

    //! @brief コピーコンストラクタの共通処理です。
    //!
    //! @param[in] textBox コピー元のテキストボックスペインです。
    //!
    void CopyCommonImpl(const TextBox& textBox);

private:
    void SetFontInfo(nn::font::WideTextWriter* pWriter) const;
    void SetFontInfoUtf8(nn::font::TextWriter* pWriter) const;
    void SetTextPos(nn::font::WideTextWriter* pWriter) const;
    void SetTextPosUtf8(nn::font::TextWriter* pWriter) const;

    nn::util::Float2  AdjustTextPos(
                    const Size& size,
                    bool        isCeil
                ) const;

    void Initialize();
    void InitializeMaterial();

    void CopyLineWidthOffset(const TextBox& textBox);
    void CopyPerCharacterTransform(const TextBox& textBox);

    void SetupConstantBufferAdditionalContent(nn::font::ConstantBufferAdditionalContent& drawContent) const;
    void BuildConstantBuffer(DrawInfo& drawInfo) const;

    uint16_t  SetStringImpl(
        const uint16_t*  pStr,
        uint16_t         dstIdx,
        int              strLen);

    uint16_t  SetStringImplUtf8(
        const char*  pStr,
        uint16_t         dstIdx,
        int              strLen);

    // 文字の位置・テクスチャが変更になったかどうかの状態を更新します。
    bool UpdatePTDirty(bool isChanged)
    {
        m_Bits.isPTDirty |= isChanged ? 1: 0;
        return isChanged;
    }
    bool UpdatePTDirty(const nn::util::Unorm8x4& vector1, const nn::util::Unorm8x4& vector2)
    {
        return UpdatePTDirty(vector1.v[0] != vector2.v[0] || vector1.v[1] != vector2.v[1] || vector1.v[2] != vector2.v[2] || vector1.v[3] != vector2.v[3]);
    }

    // コピー演算子を禁止します。
    const TextBox& operator=( const TextBox& );

    float GetLoopFrame(float frame, float begin, float end) const
    {
        if (frame > end)
            return ::std::fmod(frame - begin, end - begin) + begin;
        else if (frame < begin)
            return end - ::std::fmod(begin - frame, end - begin);
        return frame;
    }

    // 文字毎の変換により値を更新します。
    template <typename T>
    void UpdatePerCharacterTransform(nn::font::TagProcessorBase<T>* pTagProcessor);

    // 文字毎の変換の 1 文字あたりのアニメーションフレームの移動量を計算します。
    template <typename T>
    float CalculateStepTime(float* pTotalTime, nn::font::TagProcessorBase<T>* pTagProcessor);

    // 文字毎の変換の開始位置を計算します。
    float CalculateBeginTime(float stepTime, float totalTime);

    // 次の描画可能文字を取得します。
    template <typename T>
    const void* AcquireNextPrintableChar(uint32_t* pChar, nn::font::TagProcessorBase<T>* pTagProcessor, const void* pStr);

    // TagProcessor の AcquireNextPrintableChar が正しい値を返しているかチェックします
    bool ValidateNextPrintableChar(const char* pOld, const char* pNew);
    bool ValidateNextPrintableChar(const uint16_t* pOld, const uint16_t* pNew);

    // 文字列から 1 文字取得します。
    uint32_t GetCharFromPointer(const char* pStr);
    uint32_t GetCharFromPointer(const uint16_t* pStr);

    // 文字毎の変換のアニメーションカーブを各パラメータに適用します。
    void ApplyPerCharacterTransformCurve(nn::font::PerCharacterTransformInfo* pPerCharacterTransformInfo, float frame);

    bool IsPerCharacterTransformFixSpaceEnabled()
    {
        return m_Bits.perCharacterTransformFixSpace && m_Bits.perCharacterTransformOriginToCenter;
    }

    bool IsPerCharTransformSplitByCharWidthInsertSpaceEnabled()
    {
        return m_Bits.perCharacterTransformSplitByCharWidthInsertSpaceEnabled && m_Bits.perCharacterTransformOriginToCenter;
    }
private:
    union
    {
        uint16_t* utf16;
        char* utf8;
        void* neutral; // UTF-16 と UTF-8 とで共通の処理を行う場合に使用
    } m_TextBuf;
    const char* m_pTextId;
    nn::util::Unorm8x4 m_TextColors[TextColor_MaxTextColor];
    const nn::font::Font* m_pFont;
    Size m_FontSize;
    float m_LineSpace;
    float m_CharSpace;

    union
    {
        TagProcessor* utf16;
        TagProcessorUtf8* utf8;
        void* neutral; // UTF-16 と UTF-8 とで共通の処理を行う場合に使用
    } m_pTagProcessor;

    uint16_t  m_TextBufLen;
    uint16_t  m_TextLen;

    //! @brief 内部用機能のため使用禁止です。
    struct Bits
    {
        uint8_t  textAlignment: 2;
        uint8_t  isPTDirty    : 1;    // 位置・テクスチャが変更になったら真
        bool shadowEnabled: 1;
        bool invisibleBorderEnabled: 1;
        bool doubleDrawnBorderEnabled: 1;
        bool widthLimitEnabled: 1;
        bool perCharacterTransformEnabled: 1;
        bool centerCeilingEnabled: 1;
        bool perCharacterTransformSplitByCharWidth: 1;
        bool perCharacterTransformAutoShadowAlpha: 1;
        bool drawFromRightToLeft : 1;
        bool perCharacterTransformOriginToCenter : 1;
        bool perCharacterTransformFixSpace: 1;
        bool linefeedByCharacterHeightEnabled : 1;
        bool perCharacterTransformSplitByCharWidthInsertSpaceEnabled: 1;
    };
    NN_STATIC_ASSERT(sizeof(Bits) == 2);

    Bits m_Bits;
    uint8_t  m_TextPosition;

    bool m_IsUtf8;

    float m_ItalicRatio;

    nn::util::Float2 m_ShadowOffset;
    nn::util::Float2 m_ShadowScale;
    nn::util::Unorm8x4 m_ShadowTopColor;
    nn::util::Unorm8x4 m_ShadowBottomColor;
    float m_ShadowItalicRatio;

    struct LineWidthOffset
    {
        float* pLineWidth;
        float* pLineOffset;
    };
    LineWidthOffset* m_pLineWidthOffset;

    Material* m_pMaterial;
    nn::font::DispStringBuffer* m_pDispStringBuf;

    struct CurveInfo {
        const nn::ui2d::ResAnimationTarget* pCurve;
        AnimTargetPerCharacterTransformCurve CurveType;
    };

    struct PerCharacterTransform
    {
        float Offset;
        float Width;
        nn::font::PerCharacterTransformInfo* pPerCharacterTransformInfos;
        uint8_t  LoopType;
        uint8_t  OriginV;
        uint8_t  CurveNumber;
        uint8_t  Dummy;
        float OriginVOffset;
        float FixSpaceWidth;
        HorizontalAlignment FixSpaceOrigin;
        CurveInfo CurveInfos[1];    // この配列は、CurveNumberの長さがあるものとしてメモリ確保されます。
    };
    PerCharacterTransform* m_pPerCharacterTransform;
};

} // namespace nn::ui2d
} // namespace nn
