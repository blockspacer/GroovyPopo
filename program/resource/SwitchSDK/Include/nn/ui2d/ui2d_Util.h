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

#include <nn/ui2d/ui2d_Types.h>
#include <nn/util/util_MathTypes.h>
#include <nn/util/util_Arithmetic.h>
#include <nn/util/util_Matrix.h>
#include <nn/font/font_TextureCache.h>
#include <nn/gfx/util/gfx_PrimitiveShape.h>

#include <atomic>

namespace nn
{
namespace ui2d
{

class Pane;
class Layout;
class DrawInfo;
class Group;
class AnimTransform;
class TexMap;
class Material;
class ShaderInfo;
class ResourceAccessor;
struct ResHermiteKey;
struct ResParameterizedAnim;
struct ResParameterizedAnimParameter;
struct ResExtUserDataList;
struct ResCaptureTextureList;
struct ResCaptureTexture;
struct ResVectorGraphicsTextureList;
struct ResVectorGraphicsTexture;
struct BuildArgSet;
class ResExtUserData;

namespace detail
{
class BuildPaneTreeContext;
}

//----------------------------------------
//! @name アニメーション
//@{

//---------------------------------------------------------------------------
//! @brief グループ内の個々のペインにアニメーションを関連付けます。
//!
//! @param[in] pAnimTrans       アニメーションへのポインタです。
//! @param[in] pGroup           グループへのポインタです。
//! @param[in] bEnabled          アニメーションを無効状態で関連付ける場合は false を指定します。
//!
//! @details
//! bEnabled に false を渡して呼び出した場合は、アニメーションを無効状態で
//! 関連付けます。
//! 有効にする場合は、 AnimTransform::SetEnabled() を使用してください。
//!
//! @sa nn::ui2d::UnbindAnimation
//! @sa nn::ui2d::Pane::BindAnimation
//! @sa nn::ui2d::AnimTransform::SetEnabled
//!
//---------------------------------------------------------------------------
void                BindAnimation(
                        AnimTransform*  pAnimTrans,
                        Group*          pGroup,
                        bool            bEnabled = true);

//---------------------------------------------------------------------------
//! @brief グループ内の個々のペインからアニメーションの関連付けを解除します。
//!
//! @param[in] pAnimTrans   アニメーションへのポインタです。
//! @param[in] pGroup       グループへのポインタです。
//!
//! @sa nn::ui2d::BindAnimation
//! @sa nn::ui2d::Pane::UnbindAnimation
//!
//---------------------------------------------------------------------------
void                UnbindAnimation(
                        AnimTransform*  pAnimTrans,
                        Group*          pGroup);

//@}

//----------------------------------------
//! @name ヒットチェック
//@{

//---------------------------------------------------------------------------
//! @brief ペインが指定した点を含むかどうかを判定します。
//!
//! @param[in] pPane    ペインへのポインタです。
//! @param[in] pos      チェックする点の位置です。
//!
//! @return ペインが pos で指定した点を含む場合は true を返します。
//!
//! @details
//! ペインの で指定した点を含むかどうかを判別します。
//!
//! Pane::Calculate() によって計算された行列の値を使用するため、
//! この関数を呼び出す前に、 Pane::Calculate() が呼び出されている
//! 必要があります。
//!
//! この関数は、ペインの回転の x, y の値が 0 であることを前提にしています。
//! 0 以外の値の場合は正しく判定できません。
//!
//! @sa nn::ui2d::FindHitPane
//!
//---------------------------------------------------------------------------
bool IsContain(const Pane* pPane, const nn::util::Float2& pos);

//---------------------------------------------------------------------------
//! @brief 指定した点を含むペインを検索します。
//!
//! @param[in] pPane    ペインへのポインタです。
//! @param[in] pos      チェックする点の位置です。
//!
//! @return 指定した点を含む境界ペインが見つかった場合は境界ペインへの
//! ポインタを、見つからなかった場合は NULL を返します。
//!
//! @details
//! pos で指定した点を含む境界ペインを検索します。検索順は、描画順と逆の
//! 順序になります。非表示のペインとその子階層は検索対象になりません。
//!
//! pPane で指定したペインをルートとして検索します。
//!
//---------------------------------------------------------------------------
Pane* FindHitPane(Pane* pPane, const nn::util::Float2& pos);
const Pane* FindHitPane(const Pane* pPane, const nn::util::Float2& pos);

//---------------------------------------------------------------------------
//! @brief 指定した点を含むペインを検索します。
//!
//! @param[in] pLayout  レイアウトへのポインタです。
//! @param[in] pos      チェックする点の位置です。
//!
//! @return 指定した点を含む境界ペインが見つかった場合は境界ペインへの
//! ポインタを、見つからなかった場合は NULL を返します。
//!
//! @details
//! pos で指定した点を含む境界ペインを検索します。検索順は、描画順と逆の
//! 順序になります。非表示のペインとその子階層は検索対象になりません。
//!
//! レイアウト全てのペインを検索対象とします。
//!
//! Pane::Calculate() によって計算された行列の値を使用するため、
//! この関数を呼び出す前に、 Pane::Calculate() が呼び出されている
//! 必要があります。
//!
//! この関数は、ペインの回転の x, y の値が 0 であることを前提にしています。」
//! 0 以外の値の場合は正しく判定できません。
//!
//---------------------------------------------------------------------------
Pane* FindHitPane(Layout* pLayout,const nn::util::Float2& pos);
const Pane* FindHitPane(const Layout* pLayout,const nn::util::Float2& pos);

//@}

//---------------------------------------------------------------------------
//! @brief ペインの子供、あるいは兄弟のペインのポインタを返します。
//!
//! @param[in] pPane    ペインへのポインタです。
//!
//! @return ペインの子供、あるいは兄弟のペインのポインタを返します。
//! 該当するペインが無い場合は NULL を返します。
//!
//! @details
//! 引数 pPane で指定されたペインの子供、あるいは兄弟のペインを返します。
//! 最初にルートペインを引数にして呼び出し、以後 NULL が返るまで返り値を引数に
//! 設定して呼び出すことで、全てのペインを列挙することが出来ます。
//!
//---------------------------------------------------------------------------
Pane* GetNextPane(Pane* pPane);
inline const Pane* GetNextPane(const Pane* pPane)
{
    return GetNextPane(const_cast<Pane*>(pPane));
}

//---------------------------------------------------------------------------
//! @brief ペインを、その子ペインも一緒にコピーします。
//!
//! @param[in] pPane    コピー元のペインへのポインタです。
//! @param[in] pDevice  デバイスです。
//!
//! @return コピーしたペインを返します。
//!
//! @details
//! このメソッドは、与えられたペインの子ペインに再帰的にコピーコンストラクタを
//! 呼び出して、子ペインも一緒にコピーします。
//!
//! ペインを単体でコピーしたい場合は、各ペインのコピーコンストラクタを使用してください。
//!
//! このメソッドはレイアウトの標準のペインクラスのみサポートしますので、アプリケーション
//! で独自に各ペインを継承して拡張している場合は、このメソッドを再実装する必要があります。
//!
//! 返り値のペインの親は何も指定されていない状態になっていますので、
//! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
//!
//! また、アニメーションについては、何もバインドされていない状態になります。
//!
//! この関数ではキャプチャテクスチャの参照情報の更新が行われず
//! コピー元のペインの所属しているレイアウトが管理しているキャプチャテクスチャへのポインタがそのままコピーされます。
//! コピー元のレイアウトインスタンスが削除された場合などに不正なポインタとなりますので注意が必要です。
//!
//---------------------------------------------------------------------------
Pane*               ClonePaneTree(const Pane* pPane, nn::gfx::Device* pDevice);

//---------------------------------------------------------------------------
//! @brief ペインを、その子ペインも一緒にコピーします。
//!
//! @param[in] pPane    コピー元のペインへのポインタです。
//! @param[in] pDevice  デバイスです。
//! @param[in] pLayout  コピーしたペインが所属するレイアウトです。動的レンダリングテクスチャのリソースがこのインスタンスで管理されるようになります。
//!
//! @return コピーしたペインを返します。
//!
//! @details
//! このメソッドは、与えられたペインの子ペインに再帰的にコピーコンストラクタを
//! 呼び出して、子ペインも一緒にコピーします。
//!
//! ペインを単体でコピーしたい場合は、各ペインのコピーコンストラクタを使用してください。
//!
//! このメソッドはレイアウトの標準のペインクラスのみサポートしますので、アプリケーション
//! で独自に各ペインを継承して拡張している場合は、このメソッドを再実装する必要があります。
//!
//! 返り値のペインの親は何も指定されていない状態になっていますので、
//! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
//!
//! また、アニメーションについては、何もバインドされていない状態になります。
//!
//! この関数ではペインが参照している動的レンダリングテクスチャをコピーして pPartsParentLayout 以下で管理します。
//!
//---------------------------------------------------------------------------
Pane*               ClonePaneTree(const Pane* pPane, nn::gfx::Device* pDevice, Layout* pLayout);

//---------------------------------------------------------------------------
//! @brief ペインを、その子ペインも一緒にコピーします。
//!
//! @param[in] pPane    コピー元のペインへのポインタです。
//! @param[in] pDevice  デバイスです。
//! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
//! @param[in] pCaptureTextureNewRootName   キャプチャテクスチャ識別用の文字列を指定します。
//!                                         重複するとキャプチャテクスチャが区別できなくなるため、ペインの所属するレイアウトのルートペイン名などレイアウトインスタンスごとに重複しない文字列を設定してください。
//!
//! @return コピーしたペインを返します。
//!
//! @details
//! このメソッドは、与えられたペインの子ペインに再帰的にコピーコンストラクタを
//! 呼び出して、子ペインも一緒にコピーします。
//!
//! ペインを単体でコピーしたい場合は、各ペインのコピーコンストラクタを使用してください。
//!
//! このメソッドはレイアウトの標準のペインクラスのみサポートしますので、アプリケーション
//! で独自に各ペインを継承して拡張している場合は、このメソッドを再実装する必要があります。
//!
//! 返り値のペインの親は何も指定されていない状態になっていますので、
//! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
//!
//! このコピーコンストラクタでは渡された引数をもとに新たなキャプチャテクスチャを参照するようになります。
//!
//! また、アニメーションについては、何もバインドされていない状態になります。
//!
//! この関数ではキャプチャテクスチャの参照情報の更新が行われず
//! コピー元のペインの所属しているレイアウトが管理しているキャプチャテクスチャへのポインタがそのままコピーされます。
//! コピー元のレイアウトインスタンスが削除された場合などに不正なポインタとなりますので注意が必要です。
//!
//! @deprecated ui2d ランタイムでキャプチャテクスチャリソース管理方法が変更されたため、この関数は非推奨となりました。
//!             ClonePaneTree(const Pane* pPane, nn::gfx::Device* pDevice) と同様の挙動となっています。
//!
//---------------------------------------------------------------------------
NN_DEPRECATED Pane*               ClonePaneTree(const Pane* pPane, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pCaptureTextureNewRootName);

//---------------------------------------------------------------------------
//! @brief ペインを、その子ペインも一緒にコピーします。
//!
//! @param[in] pPane    コピー元のペインへのポインタです。
//! @param[in] pDevice  デバイスです。
//! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
//! @param[in] pCaptureTextureNewRootName   キャプチャテクスチャ識別用の文字列を指定します。
//!                                         重複するとキャプチャテクスチャが区別できなくなるため、ペインの所属するレイアウトのルートペイン名などレイアウトインスタンスごとに重複しない文字列を設定してください。
//! @param[in] pLayout  コピー元のペインを管理しているレイアウトです。
//!
//! @return コピーしたペインを返します。
//!
//! @details
//! このメソッドは、与えられたペインの子ペインに再帰的にコピーコンストラクタを
//! 呼び出して、子ペインも一緒にコピーします。
//!
//! ペインを単体でコピーしたい場合は、各ペインのコピーコンストラクタを使用してください。
//!
//! このメソッドはレイアウトの標準のペインクラスのみサポートしますので、アプリケーション
//! で独自に各ペインを継承して拡張している場合は、このメソッドを再実装する必要があります。
//!
//! 返り値のペインの親は何も指定されていない状態になっていますので、
//! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
//!
//! このコピーコンストラクタでは渡された引数をもとに新たなキャプチャテクスチャを参照するようになります。
//!
//! また、アニメーションについては、何もバインドされていない状態になります。
//!
//! この関数ではキャプチャテクスチャの参照情報の更新が行われず
//! コピー元のペインの所属しているレイアウトが管理しているキャプチャテクスチャへのポインタがそのままコピーされます。
//! コピー元のレイアウトインスタンスが削除された場合などに不正なポインタとなりますので注意が必要です。
//!
//! @deprecated ui2d ランタイムでキャプチャテクスチャリソース管理方法が変更されたため、この関数は非推奨となりました。
//!             ClonePaneTree(const Pane* pPane, nn::gfx::Device* pDevice) と同様の挙動となっています。
//!
//---------------------------------------------------------------------------
NN_DEPRECATED Pane*               ClonePaneTree(const Pane* pPane, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pCaptureTextureNewRootName, const Layout* pLayout);

//---------------------------------------------------------------------------
//! @brief ペインを、ペインツリー階層ごと(部品レイアウトも)コピーします。
//!
//! @details
//! このメソッドは、与えられたペインの子ペインに再帰的にコピーコンストラクタを
//! 呼び出して、子ペインも一緒にコピーします。
//!
//! ClonePaneTreeの方は、ペインツリー階層の中に部品レイアウトがあってもレイアウトの
//! コピーは行いませんが、こちらは部品レイアウトもコピーします。
//!
//! このメソッドはレイアウトの標準のペインクラスのみサポートしますので、アプリケーション
//! で独自に各ペインを継承して拡張している場合は、このメソッドを再実装する必要があります。
//!
//! 返り値のペインの親は何も指定されていない状態になっていますので、
//! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
//!
//! また、アニメーションについては、何もバインドされていない状態になります。
//! この関数ではキャプチャテクスチャの参照情報の更新が行われず
//! 初期化時に設定されたレイアウトが保持しているキャプチャテクスチャへのポインタがそのままコピーされます。
//!
//! この関数ではペインが参照している動的レンダリングテクスチャをコピーして pPartsParentLayout 以下で管理します。
//!
//! @param[in] pPane         コピー元のペインへのポインタです。
//! @param[in] pPartsLayout コピーするペインが部品ペインのとき、セットする部品レイアウトを指定します。
//! @param[in] pDevice  デバイスです。
//! @param[in] pPartsParentLayout クローンした際に作成されるパーツレイアウトや動的レンダリングテクスチャリソースを管理する親レイアウトを指定します。
//!
//! @return コピーしたペインを返します。
//!
//---------------------------------------------------------------------------
Pane*               ClonePaneTreeWithPartsLayout(const Pane* pPane, Layout* pPartsLayout, nn::gfx::Device* pDevice, Layout* pPartsParentLayout);

//---------------------------------------------------------------------------
//! @brief ペインを、ペインツリー階層ごと(部品レイアウトも)コピーします。
//!
//! @details
//! このメソッドは、与えられたペインの子ペインに再帰的にコピーコンストラクタを
//! 呼び出して、子ペインも一緒にコピーします。
//!
//! ClonePaneTreeの方は、ペインツリー階層の中に部品レイアウトがあってもレイアウトの
//! コピーは行いませんが、こちらは部品レイアウトもコピーします。
//!
//! このメソッドはレイアウトの標準のペインクラスのみサポートしますので、アプリケーション
//! で独自に各ペインを継承して拡張している場合は、このメソッドを再実装する必要があります。
//!
//! 返り値のペインの親は何も指定されていない状態になっていますので、
//! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
//!
//! また、アニメーションについては、何もバインドされていない状態になります。
//!
//! この関数でコピーされたペインではドロップシャドウ機能で使用するレンダーターゲットテクスチャの作成コールバック関数にレイアウトインスタンスのポインタが設定されません。
//! レンダーターゲットテクスチャ作成コールバック関数でレイアウトインスタンスのポインタが必要な場合は 7 引数版の ClonePaneTreeWithPartsLayout() を使用してください。
//!
//! @param[in] pPane         コピー元のペインへのポインタです。
//! @param[in] pPartsLayout コピーするペインが部品ペインのとき、セットする部品レイアウトを指定します。
//! @param[in] pDevice  デバイスです。
//! @param[in] pPartsParentLayout クローンした際に作成されるパーツレイアウトを管理する親レイアウトを指定します。
//! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
//! @param[in] pCaptureTextureNewRootName   キャプチャテクスチャ識別用の文字列を指定します。
//!                                         重複するとキャプチャテクスチャが区別できなくなるため、ペインの所属するレイアウトのルートペイン名などレイアウトインスタンスごとに重複しない文字列を設定してください。
//!
//! @return コピーしたペインを返します。
//!
//! @deprecated ui2d ランタイムでキャプチャテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
//!             ClonePaneTreeWithPartsLayout(const Pane* pPane, Layout* pPartsLayout, nn::gfx::Device* pDevice, Layout* pPartsParentLayout) と同様の挙動となっています。
//!
//---------------------------------------------------------------------------
NN_DEPRECATED Pane*               ClonePaneTreeWithPartsLayout(const Pane* pPane, Layout* pPartsLayout, nn::gfx::Device* pDevice, Layout* pPartsParentLayout, ResourceAccessor* pResAccessor, const char* pCaptureTextureNewRootName);

//---------------------------------------------------------------------------
//! @brief ペインを、ペインツリー階層ごと(部品レイアウトも)コピーします。
//!
//! @details
//! このメソッドは、与えられたペインの子ペインに再帰的にコピーコンストラクタを
//! 呼び出して、子ペインも一緒にコピーします。
//!
//! ClonePaneTreeの方は、ペインツリー階層の中に部品レイアウトがあってもレイアウトの
//! コピーは行いませんが、こちらは部品レイアウトもコピーします。
//!
//! このメソッドはレイアウトの標準のペインクラスのみサポートしますので、アプリケーション
//! で独自に各ペインを継承して拡張している場合は、このメソッドを再実装する必要があります。
//!
//! 返り値のペインの親は何も指定されていない状態になっていますので、
//! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
//!
//! また、アニメーションについては、何もバインドされていない状態になります。
//!
//! @param[in] pPane         コピー元のペインへのポインタです。
//! @param[in] pPartsLayout コピーするペインが部品ペインのとき、セットする部品レイアウトを指定します。
//! @param[in] pDevice  デバイスです。
//! @param[in] pPartsParentLayout クローンした際に作成されるパーツレイアウトを管理する親レイアウトを指定します。
//! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
//! @param[in] pCaptureTextureNewRootName Layout クラスのコンストラクタの pRootPaneName と同じく、レイアウトごとのキャプチャテクスチャを区別するための文字列です。
//!                         重複するとキャプチャテクスチャが区別できなくなるためペインの所属するレイアウトインスタンスごとに重複しない文字列を設定してください。
//! @param[in] pLayout  コピー元のペインを管理しているレイアウトです。
//!
//! @return コピーしたペインを返します。
//!
//! @deprecated ui2d ランタイムでキャプチャテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
//!             ClonePaneTreeWithPartsLayout(const Pane* pPane, Layout* pPartsLayout, nn::gfx::Device* pDevice, Layout* pPartsParentLayout) と同様の挙動となっています。
//!
//---------------------------------------------------------------------------
NN_DEPRECATED Pane*               ClonePaneTreeWithPartsLayout(const Pane* pPane, Layout* pPartsLayout, nn::gfx::Device* pDevice, Layout* pPartsParentLayout, ResourceAccessor* pResAccessor, const char* pCaptureTextureNewRootName, const Layout* pLayout);

//---------------------------------------------------------------------------
//! @brief コピーされたペインツリーが同等になっているか比較します。
//!
//! @details
//! このメソッドは、各ペインクラスの CompareCopiedInstanceTest を使用してペインツリー同士が
//! 同等かどうかチェックします。
//! ui2d ランタイムのテストのために作成された関数のため、アプリケーションコードでの使用は非推奨となります。
//!
//! @param[in] pPaneLhs     比較するペインツリーのルートペインです。
//! @param[in] pPaneRhs     比較するペインツリーのルートペインです。
//!
//! @return 比較結果を返します。
//!
//---------------------------------------------------------------------------
bool ComparePaneTreeTest(const Pane* pPaneLhs, const Pane* pPaneRhs);

//---------------------------------------------------------------------------
//! @brief テクスチャをデバイスにロードします。
//!
//! @param[out] pResTextureInfo ロードしたテクスチャの情報を格納します。
//! @param[in] pDevice デバイスです。
//! @param[in] pImgRes リソースです。
//!
//! @return ロードが成功した場合は true が返ります。
//!
//---------------------------------------------------------------------------
bool LoadTexture(ResourceTextureInfo* pResTextureInfo, nn::gfx::Device* pDevice, const void* pImgRes);

//---------------------------------------------------------------------------
//! @brief アーカイブシェーダをデバイスにロードします。
//!
//! @param[out] pShaderInfo ロードしたアーカイブシェーダの情報を格納します。
//! @param[in] pDevice  デバイスです。
//! @param[in] pShaderRes リソースです。
//! @param[in] pVariationTable バリエーションテーブルです。
//! @param[in] pMemoryPool メモリプールです。バイナリをメモリプール内に配置した場合にそのメモリプールを指定します。未配置の場合は nullptr を指定してください。
//! @param[in] memoryPoolOffset 配置先メモリプールの先頭からリソース配置位置へのオフセットです。
//! @param[in] memoryPoolSize 配置先メモリプールのサイズです。
//! @param[in] textureSlotCount テクスチャのスロット数です。
//!
//---------------------------------------------------------------------------
void LoadArchiveShader(ShaderInfo* pShaderInfo, nn::gfx::Device* pDevice, void* pShaderRes, const void* pVariationTable,
    nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize, int textureSlotCount);

//---------------------------------------------------------------------------
//! @brief アーカイブシェーダをデバイスにロードします。
//!
//! @param[out] pShaderInfo ロードしたアーカイブシェーダの情報を格納します。
//! @param[in] pDevice  デバイスです。
//! @param[in] pShaderRes リソースです。
//! @param[in] pVariationTable バリエーションテーブルです。
//! @param[in] pMemoryPool メモリプールです。バイナリをメモリプール内に配置した場合にそのメモリプールを指定します。未配置の場合は nullptr を指定してください。
//! @param[in] memoryPoolOffset 配置先メモリプールの先頭からリソース配置位置へのオフセットです。
//! @param[in] memoryPoolSize 配置先メモリプールのサイズです。
//!
//---------------------------------------------------------------------------
inline void LoadArchiveShader(ShaderInfo* pShaderInfo, nn::gfx::Device* pDevice, void* pShaderRes, const void* pVariationTable,
    nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize)
{
    LoadArchiveShader(pShaderInfo, pDevice, pShaderRes, pVariationTable, pMemoryPool, memoryPoolOffset, memoryPoolSize, TexMapMax);
}

//---------------------------------------------------------------------------
//! @brief アーカイブシェーダをデバイスにロードします。
//!
//! @param[out] pShaderInfo ロードしたアーカイブシェーダの情報を格納します。
//! @param[in] pDevice  デバイスです。
//! @param[in] pShaderRes リソースです。
//! @param[in] pVariationTable バリエーションテーブルです。
//!
//---------------------------------------------------------------------------
inline void LoadArchiveShader(ShaderInfo* pShaderInfo, nn::gfx::Device* pDevice, void* pShaderRes, const void* pVariationTable)
{
    LoadArchiveShader(pShaderInfo, pDevice, pShaderRes, pVariationTable, NULL, 0, 0, TexMapMax);
}

//---------------------------------------------------------------------------
//! @brief アーカイブシェーダを解放します。
//!
//! @param[in] pDevice  デバイスです。
//! @param[in] pShaderInfo ロードしたアーカイブシェーダの情報を格納します。
//!
//---------------------------------------------------------------------------
void FreeArchiveShader(nn::gfx::Device* pDevice, ShaderInfo* pShaderInfo);

//---------------------------------------------------------------------------
//! @brief シェーダバリエーションテーブルからバリエーションインデックスを検索します。
//!
//! @param[in] pVariationTable バリエーションテーブルです。
//! @param[in] signature シグネチャ
//! @param[in] keyLength 検索キーの長さ
//! @param[in] pKey 検索キー
//! @return バリエーションインデックスです。見つからなかった場合は -1 が返ります。
//!
//---------------------------------------------------------------------------
int SearchShaderVariationIndexFromTable(const void* pVariationTable, uint32_t signature, size_t keyLength, const uint32_t* pKey);

//---------------------------------------------------------------------------
//! @brief 指定したペインツリー以下のヌルペインと境界ペインを枠線で描画します。
//!
//! @param[in] commandBuffer    コマンドバッファです。
//! @param[in] drawInfo         描画情報です。
//! @param[in] material         マテリアルです。
//! @param[in] pPane            コピー元のペインへのポインタです。
//! @param[in] nullColor        ヌルペインの枠線の色です。
//! @param[in] boundingColor    境界ペインの枠線の色です。
//!
//---------------------------------------------------------------------------
void DrawNullAndBoundingPane(nn::gfx::CommandBuffer& commandBuffer, DrawInfo& drawInfo, Material& material, const Pane* pPane, const nn::util::Unorm8x4& nullColor, const nn::util::Unorm8x4& boundingColor);

//---------------------------------------------------------------------------
//! @brief エルミート形式カーブの特定フレームでの値を取得します。
//!
//! @param[in] frame    値を取得するフレームです。
//! @param[in] pKeys    エルミート形式キーの配列です。
//! @param[in] keySize  キー数です。
//!
//! @return 値です。
//!
//---------------------------------------------------------------------------
float GetHermiteCurveValue(float frame, const ResHermiteKey*  pKeys, int  keySize);

//---------------------------------------------------------------------------
//! @brief パラメタライズドアニメーションの特定フレームでの値を取得します。
//!
//! @param[in] frame        値を取得するフレームです。
//! @param[in] current      現在の値です。
//! @param[in] pParameterizedAnim パラメタライズドアニメーションのパラメータです。
//!
//! @return 値です。
//!
//---------------------------------------------------------------------------
float GetParameterizedAnimValue(float frame, float current, const ResParameterizedAnim* pParameterizedAnim);

//---------------------------------------------------------------------------
//! @brief パラメタライズドアニメーションの特定フレームでの値を取得します。
//!
//! @param[in] frame        値を取得するフレームです。
//! @param[in] pParameter   パラメタライズドアニメーションのパラメータです。
//!
//! @return 値です。
//!
//---------------------------------------------------------------------------
float GetParameterizedAnimValueAtFrame(float frame, const ResParameterizedAnimParameter* pParameter);

//---------------------------------------------------------------------------
//! @brief パラメタライズドアニメーションの特定フレームでの値を取得します。内部で正規化したフレームを強制的に 0.0 - 1.0 の範囲にクランプします。
//!
//! @param[in] frame        値を取得するフレームです。
//! @param[in] pParameter   パラメタライズドアニメーションのパラメータです。
//!
//! @return 値です。
//!
//---------------------------------------------------------------------------
float GetParameterizedAnimValueAtFrameClamped(float frame, const ResParameterizedAnimParameter* pParameter);


//---------------------------------------------------------------------------
//! @brief フレームが指定区間内かどうかを取得します。
//!
//! @param[in] frame        値を取得するフレームです。
//! @param[in] offset       開始フレームです。
//! @param[in] duration     フレーム期間です。
//!
//! @return 範囲に入っていれば true です。
//!
//---------------------------------------------------------------------------
inline bool IsFrameInRange(float frame, float offset, float duration)
{
    return (frame >= offset) && (offset + duration >= frame);
}

//---------------------------------------------------------------------------
//! @brief ResExtUserDataList から特定の名前を持った ResExtUserData を取得します。
//!
//! @param[in] pExtUserDataList データを探す ResExtUserDataList
//! @param[in] pName    探す ResExtUserData の名前。
//!
//! @return 見つかった ResExtUserData。見つからなければ NULL を返します。
//!
//---------------------------------------------------------------------------
const ResExtUserData* GetExtUserData(const ResExtUserDataList* pExtUserDataList, const char* pName);

//---------------------------------------------------------------------------
//! @brief  nn::gfx::Buffer のアライメントを考慮したサイズを計算します。
//!
//! @param[in]  pDevice デバイスです。
//! @param[in]  gpuAccess GPU からどのようにアクセスされるか。
//! @param[in]  size    コンスタントバッファのサイズです。
//!
//! @return アライメントを考慮したサイズ。
//---------------------------------------------------------------------------
size_t GetAlignedBufferSize(nn::gfx::Device* pDevice, nn::gfx::GpuAccess gpuAccess, const size_t size);

//---------------------------------------------------------------------------
//! @brief デフォルトコンストラクタで初期化したペインのマテリアルのシェーダ ID を設定します。
//!
//! @param[in] pMaterial    シェーダ ID を設定するマテリアルです。
//! @param[in] texCount     使用するテクスチャの数です。
//---------------------------------------------------------------------------
void SetDefaultShaderId(nn::ui2d::Material* pMaterial, int texCount);

//---------------------------------------------------------------------------
//! @brief  nn::gfx::ResShaderContainer で使用されているメモリプールが初期化済みかどうかをチェックします。
//!
//! @param[in] pContainer   チェックする ResShaderContainer
//!
//! @return 初期化済みならば true が返ります。
//---------------------------------------------------------------------------
bool IsResShaderContainerInitialized(nn::gfx::ResShaderContainer* pContainer);

//---------------------------------------------------------------------------
//! @brief  nn::gfx::ResShaderProgram が初期化済みかどうかをチェックします。
//!
//! @param[in] pResShaderProgram    チェックする ResShaderProgram
//!
//! @return 初期化済みならば true が返ります。
//---------------------------------------------------------------------------
bool IsResShaderProgramInitialized(nn::gfx::ResShaderProgram* pResShaderProgram);

//---------------------------------------------------------------------------
//! @brief  キャプチャテクスチャの階層構造を含めた名前を作成します。
//!
//! @param[out] pNewName    名前を出力するバッファです。
//! @param[in]  resultSize  pNewName バッファのサイズです。
//! @param[in]  pPrefix     名前に接続するプリフィックスです。
//! @param[in]  pName       名前です。
//---------------------------------------------------------------------------
NN_DEPRECATED void MakeCaptureTextureName(char* pNewName, size_t resultSize, const char* pPrefix, const char* pName);

//---------------------------------------------------------------------------
//! @brief  動的生成テクスチャの階層構造を含めた名前を作成します。
//!
//! @param[out] pNewName    名前を出力するバッファです。
//! @param[in]  resultSize  pNewName バッファのサイズです。
//! @param[in]  pPrefix     名前に接続するプリフィックスです。
//! @param[in]  pName       名前です。
//---------------------------------------------------------------------------
void MakeDynamicGenerateTextureName(char* pNewName, size_t resultSize, const char* pPrefix, const char* pName);

//---------------------------------------------------------------------------
//! @brief BuildArgSet に設定されているキャプチャテクスチャ修飾用文字列スタックの特定の階層までを接続した場合の文字列の長さ(null 文字含む)を計算します。
//!
//! @param[in]  buildArgSet         計算に使用する BuildArgSet です。
//! @param[in]  concatenateDepth    計算する階層の深さです。
//---------------------------------------------------------------------------
NN_DEPRECATED size_t CalcCaptureTexturePrefixLength(const BuildArgSet& buildArgSet, int concatenateDepth);

//---------------------------------------------------------------------------
//! @brief BuildArgSet に設定されている動的生成テクスチャ修飾用文字列スタックの特定の階層までを接続した場合の文字列の長さ(null 文字含む)を計算します。
//!
//! @param[in]  buildArgSet         計算に使用する BuildArgSet です。
//! @param[in]  concatenateDepth    計算する階層の深さです。
//---------------------------------------------------------------------------
size_t CalcDynamicGenerateTexturePrefixLength(const BuildArgSet& buildArgSet, int concatenateDepth);

//---------------------------------------------------------------------------
//! @brief BuildArgSet に設定されているキャプチャテクスチャ修飾用文字列を接続した文字列を作成します。
//! @param[out] pResult     名前を出力するバッファです。
//! @param[in]  resultSize  pResult バッファのサイズです。
//! @param[in]  buildArgSet         計算に使用する BuildArgSet です。
//! @param[in]  concatenateDepth    計算する階層の深さです。
//---------------------------------------------------------------------------
NN_DEPRECATED void ConcatCaptureTexturePrefixString(char* pResult, size_t resultSize, const BuildArgSet& buildArgSet, int concatenateDepth);

//---------------------------------------------------------------------------
//! @brief BuildArgSet に設定されている動的生成テクスチャ修飾用文字列を接続した文字列を作成します。
//! @param[out] pResult     名前を出力するバッファです。
//! @param[in]  resultSize  pResult バッファのサイズです。
//! @param[in]  buildArgSet         計算に使用する BuildArgSet です。
//! @param[in]  concatenateDepth    計算する階層の深さです。
//---------------------------------------------------------------------------
void ConcatDynamicGenerateTexturePrefixString(char* pResult, size_t resultSize, const BuildArgSet& buildArgSet, int concatenateDepth);

//---------------------------------------------------------------------------
//! @brief BuildArgSet のパーツ階層修飾情報と名前からキャプチャテクスチャ名を生成して、リソースアクセッサから TextureInfo を取得します。
//!
//! @details
//! pNewNameCopy は不要な場合は NULL を指定することができます。
//!
//! @param[out] pNewNameCopy 最終的な名前の文字列が必要な場合は書き込み先のバッファを設定します。NULL を設定すると何も処理されません。
//! @param[in]  newNameCopyBufferSize pNewNameCopy バッファのサイズを指定します。
//! @param[in]  buildArgSet BuildArgSet への参照です。
//! @param[in]  overrided 上書きされたキャプチャテクスチャかどうかを指定します。
//! @param[in]  pDevice デバイスです。
//! @param[in]  pResAccessor テクスチャを問い合わせるリソースアクセッサを指定します。
//! @param[in]  pName キャプチャテクスチャの名前を指定します。
//!
//! @return 見つかった TextureInfo を返します。
//!
//---------------------------------------------------------------------------
NN_DEPRECATED const TextureInfo* AcquireCaptureTextureWithResolvePrefix(char* pNewNameCopy, int newNameCopyBufferSize, const BuildArgSet& buildArgSet, bool overrided, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pName);

//---------------------------------------------------------------------------
//! @brief BuildArgSet のパーツ階層修飾情報と名前から動的生成テクスチャ名を生成して、リソースアクセッサから TextureInfo を取得します。
//!
//! @details
//! pNewNameCopy は不要な場合は NULL を指定することができます。
//!
//! @param[out] pNewNameCopy 最終的な名前の文字列が必要な場合は書き込み先のバッファを設定します。NULL を設定すると何も処理されません。
//! @param[in]  newNameCopyBufferSize pNewNameCopy バッファのサイズを指定します。
//! @param[in]  buildArgSet BuildArgSet への参照です。
//! @param[in]  overrided 上書きされた動的生成テクスチャかどうかを指定します。
//! @param[in]  pDevice デバイスです。
//! @param[in]  pResAccessor テクスチャを問い合わせるリソースアクセッサを指定します。
//! @param[in]  pName 動的生成テクスチャの名前を指定します。
//!
//! @return 見つかった TextureInfo を返します。
//!
//---------------------------------------------------------------------------
NN_DEPRECATED const TextureInfo* AcquireDynamicGenerateTextureWithResolvePrefix(char* pNewNameCopy, int newNameCopyBufferSize, const BuildArgSet& buildArgSet, bool overrided, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pName);

//---------------------------------------------------------------------------
//! @brief プリフィックスとテクスチャ名からキャプチャテクスチャ名を生成して、リソースアクセッサから TextureInfo を取得します。
//!
//! @details
//! pNewNameCopy は不要な場合は NULL を指定することができます。
//!
//! @param[out] pNewNameCopy 最終的な名前の文字列が必要な場合は書き込み先のバッファを設定します。NULL を設定すると何も処理されません。
//! @param[in]  newNameCopyBufferSize pNewNameCopy バッファのサイズを指定します。
//! @param[in]  pDevice デバイスです。
//! @param[in]  pResAccessor テクスチャを問い合わせるリソースアクセッサを指定します。
//! @param[in]  pPrefix プリフィックスです。
//! @param[in]  pName キャプチャテクスチャの名前を指定します。
//!
//! @return 見つかった TextureInfo を返します。
//!
//---------------------------------------------------------------------------
NN_DEPRECATED const TextureInfo* AcquireCaptureTexture(char* pNewNameCopy, int newNameCopyBufferSize, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pPrefix, const char* pName);

//---------------------------------------------------------------------------
//! @brief プリフィックスとテクスチャ名から動的生成テクスチャ名を生成して、リソースアクセッサから TextureInfo を取得します。
//!
//! @details
//! pNewNameCopy は不要な場合は NULL を指定することができます。
//!
//! @param[out] pNewNameCopy 最終的な名前の文字列が必要な場合は書き込み先のバッファを設定します。NULL を設定すると何も処理されません。
//! @param[in]  newNameCopyBufferSize pNewNameCopy バッファのサイズを指定します。
//! @param[in]  pDevice デバイスです。
//! @param[in]  pResAccessor テクスチャを問い合わせるリソースアクセッサを指定します。
//! @param[in]  pPrefix プリフィックスです。
//! @param[in]  pName テクスチャの名前を指定します。
//!
//! @return 見つかった TextureInfo を返します。
//!
//---------------------------------------------------------------------------
NN_DEPRECATED const TextureInfo* AcquireDynamicGenerateTexture(char* pNewNameCopy, int newNameCopyBufferSize, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pPrefix, const char* pName);



//---------------------------------------------------------------------------
//! @brief  シェーダーの初期化を試行し、初期化に成功した nn::gfx::ShaderCodeType を返します。
//!
//! @param[in] pDevice デバイスです。
//! @param[in] pVariation  初期化を試行する ResShaderVariation。
//!
//! @return 初期化に成功した際の nn::gfx::ShaderCodeType。失敗した場合 Debug 版ではアサートし、Release 版では不定の値を返します。
//---------------------------------------------------------------------------
nn::gfx::ShaderCodeType TryInitializeAndGetShaderCodeType(nn::gfx::Device* pDevice, nn::gfx::ResShaderVariation* pVariation);

namespace detail
{
//  キャプチャする際のルートグローバル行列を取得します。

//---------------------------------------------------------------------------
//! @brief 内部用機能のため使用禁止です。
//!
//! @details
//! DrawInfo のウインドウ原点指定によって行列の Y 軸を反転させます。
//!
//! @param[out] mtx 計算結果の行列が書き込まれます。
//! @param[in]  drawInfo 描画情報です。
//!
//---------------------------------------------------------------------------
void CalculateCaptureRootMatrix(nn::util::MatrixT4x3fType& mtx, const DrawInfo& drawInfo);

//  ResCaptureTextureList から指定の名前のキャプチャテクスチャの ResCaptureTexture を取得します。

//---------------------------------------------------------------------------
//! @brief 内部用機能のため使用禁止です。
//!
//! @param[in] pResList リソースを探す ResCaptureTextureList のポインタです。
//! @param[in] pTextureName 探すテクスチャの名前です。
//!
//! @return 見つかった場合は ResCaptureTexture のポインタを返します。見つからなかった場合は NULL を返します。
//!
//---------------------------------------------------------------------------
const ResCaptureTexture* FindCaptureTextureResource(const ResCaptureTextureList* pResList, const char* pTextureName);

//  ResVectorGraphicsTextureList から指定の名前のキャプチャテクスチャの ResVectorGraphicsTexture を取得します。

//---------------------------------------------------------------------------
//! @brief 内部用機能のため使用禁止です。
//!
//! @param[in] pResList リソースを探す ResVectorGraphicsTextureList のポインタです。
//! @param[in] pTextureName 探すテクスチャの名前です。
//!
//! @return 見つかった場合は ResVectorGraphicsTexture のポインタを返します。見つからなかった場合は NULL を返します。
//!
//---------------------------------------------------------------------------
const ResVectorGraphicsTexture* FindVectorGraphicsTextureResource(const ResVectorGraphicsTextureList* pResList, const char* pTextureName);

//  キャプチャテクスチャの上書き文字列データからキャプチャテクスチャ名のみ取得します。

//---------------------------------------------------------------------------
//! @brief 内部用機能のため使用禁止です。
//!
//! @details
//! 引数で渡された文字列内のポインタを返します。
//!
//! @param[in]  pOverwriteString 上書き設定文字列です。
//!
//! @return キャプチャテクスチャ名の先頭を表すポインタを返します。
//!
//---------------------------------------------------------------------------
const char* FindCaptureTextureBaseNameFromOverwriteString(const char* pOverwriteString);

//  キャプチャテクスチャの上書き文字列データから上書き階層の深さを取得します。

//---------------------------------------------------------------------------
//! @brief 内部用機能のため使用禁止です。
//!
//! @param[in]  pOverwriteString 上書き設定文字列です。
//!
//! @return 上書き階層の深さを返します。
//!
//---------------------------------------------------------------------------
int CalcCaptureTextureNameOverwriteDepth(const char* pOverwriteString);

//---------------------------------------------------------------------------
//! @brief 内部用機能のため使用禁止です。
//!
//! @details
//! 値を min, max でクランプします。
//!
//! @param[out] value 調整する値を設定します。
//! @param[in] min  値の最小値を設定します。
//! @param[in] max  値の最大値を設定します。
//!
//---------------------------------------------------------------------------
void ClampValue(float& value, float min, float max);

//  通常のペイン描画以外の描画のために頂点バッファコンスタントバッファを確保してデフォルト値を設定します。

//---------------------------------------------------------------------------
//! @brief 内部用機能のため使用禁止です。
//!
//! @details
//! drawInfo からコンスタントバッファ用の領域を確保して、デフォルトの描画設定で初期化します。
//!
//! @param[in]  drawInfo 描画情報です。
//! @param[in]  pPane デフォルト値設定に使用するペインです。
//!
//! @return 確保したコンスタントバッファのオフセットを返します。
//!
//---------------------------------------------------------------------------
size_t AllocAndSetupVertexShaderConstantBuffer(DrawInfo& drawInfo, const Pane* pPane);

//  シェーダーキャッシュを考慮してシェーダーのセットアップ処理を行います。

//---------------------------------------------------------------------------
//! @brief 内部用機能のため使用禁止です。
//!
//! @details
//! シェーダーキャッシュを使用して必要に応じてシェーダーのセットアップ処理を行います。
//!
//! @param[in]  drawInfo 描画情報です。
//! @param[in]  commandBuffer    コマンドバッファです。
//! @param[in]  pShaderInfo セットアップするシェーダーです。
//! @param[in]  variationIndex セットアップするシェーダーのバリエーションインデックスです。
//!
//---------------------------------------------------------------------------
void SetupShaderWithShaderCache(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer, const ShaderInfo* pShaderInfo, int variationIndex);

//---------------------------------------------------------------------------
//! @brief 内部用機能のため使用禁止です。
//!
//! @details
//! Scale、Rotate、Transの設定
//!
//---------------------------------------------------------------------------
inline void
MakeMatrixSrt(nn::util::MatrixT4x3fType& outMatrix, const nn::util::Float2& scale, const float rotateZ, const nn::util::Float3& trans)
{
    float sinValue, cosValue;

    const nn::util::AngleIndex index = nn::util::DegreeToAngleIndex(rotateZ);
    nn::util::SinCosTable(&sinValue, &cosValue, index);

    const float tmp0 = scale.v[0] * cosValue;
    const float tmp1 = -scale.v[1] * sinValue;
    const float tmp2 = scale.v[0] * sinValue;
    const float tmp3 = scale.v[1] * cosValue;

    nn::util::MatrixSet(
        &outMatrix,

        tmp0,
        tmp2,
        0.f,

        tmp1,
        tmp3,
        0.f,

        0.f,
        0.f,
        1.f,

        trans.v[0],
        trans.v[1],
        trans.v[2]
    );
}

//---------------------------------------------------------------------------
//! @brief 内部用機能のため使用禁止です。
//!
//! @details
//! Scale、Rotate、Transの設定
//!
//---------------------------------------------------------------------------
inline void
MakeMatrixSrt(nn::util::MatrixT4x3fType& outMatrix, const nn::util::Float2& scale, const nn::util::Float3& rotate, const nn::util::Float3& trans)
{
    float sinX, cosX;
    float sinY, cosY;
    float sinZ, cosZ;

    nn::util::SinCosTable(&sinX, &cosX, nn::util::DegreeToAngleIndex(rotate.v[0]));
    nn::util::SinCosTable(&sinY, &cosY, nn::util::DegreeToAngleIndex(rotate.v[1]));
    nn::util::SinCosTable(&sinZ, &cosZ, nn::util::DegreeToAngleIndex(rotate.v[2]));

    const float opt1 = cosX * cosZ;
    const float opt2 = sinX * sinY;
    const float opt3 = cosX * sinZ;

    const float tmp00 = scale.v[0] * (cosY * cosZ);
    const float tmp10 = scale.v[0] * (cosY * sinZ);
    const float tmp20 = scale.v[0] * (-sinY);

    const float tmp01 = scale.v[1] * ((opt2 * cosZ) - (opt3));
    const float tmp11 = scale.v[1] * ((opt2 * sinZ) + (opt1));
    const float tmp21 = scale.v[1] * ((sinX * cosY));

    const float tmp02 = (opt1 * sinY) + (sinX * sinZ);
    const float tmp12 = (opt3 * sinY) - (sinX * cosZ);
    const float tmp22 = (cosX * cosY);

    nn::util::MatrixSet(
        &outMatrix,

        tmp00,
        tmp10,
        tmp20,

        tmp01,
        tmp11,
        tmp21,

        tmp02,
        tmp12,
        tmp22,

        trans.v[0],
        trans.v[1],
        trans.v[2]
    );
}

//  AcquireDynamicGenerateTextureWithResolvePrefix が deprecated なため、内部で呼び出すことができない。
//  互換性維持のために一時的に別の名前の関数を作成して実装をうつします。
//  deprecated な関数がなくなったタイミングでこの関数も削除します。
const TextureInfo* AcquireDynamicGenerateTextureWithResolvePrefixImpl(char* pNewNameCopy, int newNameCopyBufferSize, const BuildArgSet& buildArgSet, bool overrided, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pName);
//  AcquireDynamicGenerateTextureImpl が deprecated なため、内部で呼び出すことができない。
//  互換性維持のために一時的に別の名前の関数を作成して実装をうつします。
//  deprecated な関数がなくなったタイミングでこの関数も削除します。
const TextureInfo* AcquireDynamicGenerateTextureImpl(char* pNewNameCopy, int newNameCopyBufferSize, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pPrefix, const char* pName);

// ClonePaneTree の実装です。 BuildPaneTreeContext とその使用メモリを関数内でスタック上に確保して BuildPaneTreeContext のポインタを受け取る ClonePaneTreeImpl_ を呼びだします。
Pane* ClonePaneTreeImpl_(const Pane* pPane, nn::gfx::Device* pDevice, Layout* pLayout);
// ClonePaneTree の実装です。
Pane* ClonePaneTreeImpl_(const Pane* pPane, nn::gfx::Device* pDevice, Layout* pLayout, detail::BuildPaneTreeContext* pBuildPaneTreeContext);

// ClonePaneTreeWithPartsLayout の実装です。BuildPaneTreeContext とその使用メモリを関数内でスタック上に確保して BuildPaneTreeContext のポインタを受け取る ClonePaneTreeImpl_ を呼びだします。
Pane* ClonePaneTreeWithPartsLayoutImpl_(const Pane* pPane, Layout* pPartsLayout, nn::gfx::Device* pDevice, Layout* pPartsParentLayout);
// ClonePaneTreeWithPartsLayout の実装です。
Pane* ClonePaneTreeWithPartsLayoutImpl_(const Pane* pPane, Layout* pPartsLayout, nn::gfx::Device* pDevice, Layout* pPartsParentLayout, detail::BuildPaneTreeContext* pBuildPaneTreeContext);

//---------------------------------------------------------------------------
//! @brief 内部用機能のため使用禁止です。
//!
//! @details
//! Layout::AllocateMemory で渡された文字列に必要なサイズ分メモリを確保し、その領域へ文字列をコピーします。
//!
//---------------------------------------------------------------------------
char* AllocateAndCopyString(const char* pString);

}

//---------------------------------------------------------------------------
//! @brief 複合フォントのヘルパークラスです。
//!
class ComplexFontHelper
{
public:

    //----------------------------------------------------------
    //! @brief ファイル名からフォントを取得するコールバック関数です。
    //! @param[out] pOutFontDataSize         データサイズです。
    //! @param[in] pFontName     フォント名です。
    //! @param[in] resType       リソースの種類です。
    //! @param[in] pUserData     コールバック関数に渡されるユーザーデータです。
    //! @return フォントデータへのポインタです。データがない場合はNULLを返します。
    typedef void* (*AcquireFontFunction)(size_t* pOutFontDataSize, const char* pFontName, nn::ui2d::ResType resType, void* pUserData);

    //----------------------------------------------------------
    //! @brief 複合フォントリソースを元に nn::font::TextureCache::InitializeArg を設定します。
    //! @details
    //! fcpx ファイル内で .bfttf を設定したフォントに対し、コールバック関数内で
    //! nn::pl::GetSharedFontAddress() の戻り値のデータをそのまま返しても読み込むことができます。
    //!
    //! @param[in] pTextureCacheArg                     テクスチャキャッシュに渡す初期化パラメータです。
    //! @param[in] pAcquireFontFunction                 ファイル名からフォントを取得するコールバック関数です。
    //! @param[in] pAcquireFontFunctionUserData         コールバック関数に渡されるユーザーデータです。
    //! @param[in] pCpxData                             複合フォントリソースのポインタです。
    //!
    //! @return フォントを登録した先頭のフォントフェースが返ります。 nn::ui2d::ComplexFontHelper::InitializeComplexFontTree() の呼び出し時にこの値を渡してください。
    static int SetupTextureCacheArg(
        nn::font::TextureCache::InitializeArg* pTextureCacheArg,
        AcquireFontFunction pAcquireFontFunction,
        void* pAcquireFontFunctionUserData,
        const void* pCpxData);

    //----------------------------------------------------------
    //! @brief 複合フォントリソースを元に Font クラスの木構造を構築します。
    //! @details
    //! この関数が呼ばれる前にテクスチャキャッシュの初期化が完了している必要があります。
    //!
    //! @param[in] pDevice                              デバイスです。
    //! @param[in] pRegisterTextureViewSlot             複合フォント内のビットマップフォントの初期化に使用するテクスチャビュースロットです。
    //! @param[in] pUserDataForDescriptorSlotAllocator  DescriptorSlotAllocator に渡すユーザデータです。
    //! @param[in] pTextureCache                        複合フォント内のスケーラブルフォントの初期化に使用するテクスチャキャッシュです。
    //! @param[in] fontFaceHead                         フォントフェースの先頭です。 nn::ui2d::ComplexFontHelper::SetupTextureCacheArg() の戻り値を渡してください。
    //! @param[in] pAcquireFontFunction                 ファイル名からフォントを取得するコールバック関数です。
    //! @param[in] pAcquireFontFunctionUserData         コールバック関数に渡されるユーザーデータです。
    //! @param[in] pCpxData                             複合フォントリソースのポインタです。
    //!
    //! @return Font クラスの木構造のルートです。
    static nn::font::Font* InitializeComplexFontTree(
        nn::gfx::Device* pDevice,
        nn::font::RegisterTextureViewSlot pRegisterTextureViewSlot,
        void* pUserDataForDescriptorSlotAllocator,
        nn::font::TextureCache* pTextureCache,
        int fontFaceHead,
        AcquireFontFunction pAcquireFontFunction,
        void* pAcquireFontFunctionUserData,
        const void* pCpxData);

    //----------------------------------------------------------
    //! @brief 複合フォントリソースを元に構築した Font クラスの木構造を解放します。
    //!
    //! @param[in] pDevice                                  デバイスです。
    //! @param[in] pComplexFontTree                         解放するフォントクラスのルートです。
    //! @param[in] pUnregisterTextureViewSlot                テクスチャビュースロットです。
    //! @param[in] pUserDataForDescriptorSlotDeallocator    DescriptorSlotDeallocator に渡すユーザデータです。
    static void FinalizeComplexFontTree(
        nn::gfx::Device* pDevice,
        nn::font::Font* pComplexFontTree,
        nn::font::UnregisterTextureViewSlot pUnregisterTextureViewSlot,
        void* pUserDataForDescriptorSlotDeallocator);

private:
    // ファイル名が指定した拡張子になっているかどうかを判別
    static bool CheckExt(const char* pName, const char* ext);

    // テクスチャキャッシュに渡す arg を構築
    template <typename T1, typename T2>
    static void BuildTextureCacheArg(
        const void* pRes,
        int* pFontFaceCount,
        nn::font::TextureCache::InitializeArg* pTextureCacheInitArg,
        AcquireFontFunction pAcquireFontFunction,
        void* pAcquireFontFunctionUserData,
        uint32_t version);

    // 複合フォントの木構造を構築
    template <typename T>
    static nn::font::Font* BuildFontTree(
        nn::gfx::Device* pDevice,
        nn::font::RegisterTextureViewSlot pRegisterTextureViewSlot,
        void* pUserDataForDescriptorSlotAllocator,
        const void* pRes,
        int* pFontFaceCount,
        nn::font::TextureCache* pTextureCache,
        AcquireFontFunction pAcquireFontFunction,
        void* pAcquireFontFunctionUserData,
        uint32_t version);

    // 複合フォントの木構造を破棄
    static void DestroyFontTree(
        nn::gfx::Device* pDevice,
        nn::font::Font* pFont,
        nn::font::UnregisterTextureViewSlot pUnregisterTextureViewSlot,
        void* pUserDataForDescriptorSlotDeallocator);
};

//---------------------------------------------------------------------------
//! @brief 角丸矩形の頂点情報について計算します。
//---------------------------------------------------------------------------
class RoundRectShape : public nn::gfx::util::PrimitiveShape
{
    NN_DISALLOW_COPY(RoundRectShape);

public:

    //---------------------------------------------------------------------------
    //! @brief コンストラクタです。
    //!
    //! @param[in]   vertexFormat       頂点バッファのフォーマット
    //! @param[in]   primitiveTopology  プリミティブトポロジーです。
    //!                                  　 ポリゴン: PrimitiveTopology_TriangleList
    //! @param[in]   cornerRadiusH      角丸の幅
    //! @param[in]   cornerRadiusV      角丸の高さ
    //! @param[in]   cornerSliceCount   角丸の分割数です。
    //!
    //! @details 原点を中心とした指定された幅高さの角丸矩形を作成します。
    //---------------------------------------------------------------------------
    RoundRectShape(
        nn::gfx::util::PrimitiveShapeFormat       vertexFormat,
        nn::gfx::PrimitiveTopology primitiveTopology,
        float cornerRadiusH,
        float cornerRadiusV,
        uint32_t cornerSliceCount);

    //---------------------------------------------------------------------------
    //! @brief コンストラクタです。
    //!
    //! @param[in]   vertexFormat       頂点バッファのフォーマット
    //! @param[in]   primitiveTopology  プリミティブトポロジーです。
    //!                                  　 ポリゴン: PrimitiveTopology_TriangleList
    //! @details    初期化するのみのコンストラクタです。
    //---------------------------------------------------------------------------
    RoundRectShape(
        nn::gfx::util::PrimitiveShapeFormat       vertexFormat,
        nn::gfx::PrimitiveTopology primitiveTopology);

    //---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //---------------------------------------------------------------------------
    virtual ~RoundRectShape();

    //---------------------------------------------------------------------------
    //! @brief  ポインタ以外のパラメータをコピーします。
    //---------------------------------------------------------------------------
    void CopyParams(const RoundRectShape& src);
protected:

    //---------------------------------------------------------------------------
    //! @brief  頂点数を計算します。
    //!
    //! @return 頂点数を返します。
    //---------------------------------------------------------------------------
    int CalculateVertexCount();

    //---------------------------------------------------------------------------
    //! @brief  インデックス数を計算します。
    //!
    //! @return インデックス数を返します。
    //---------------------------------------------------------------------------
    int CalculateIndexCount();

    //---------------------------------------------------------------------------
    //! @brief  CalculateメソッドのSphereShapeクラスでの実装
    //!
    //! @param[out]  pVertexMemory  頂点情報を書き込むメモリのポインタ
    //! @param[in]   vertexSize     確保済みのメモリサイズ(バイト単位)
    //! @param[out]  pIndexMemory   インデックス情報を書き込むメモリのポインタ
    //! @param[in]   indexSize      確保済みのメモリサイズ(バイト単位)
    //---------------------------------------------------------------------------
    virtual void CalculateImpl(void* pVertexMemory, size_t vertexSize, void* pIndexMemory, size_t indexSize);

private:

    //---------------------------------------------------------------------------
    //! @brief   頂点バッファを計算します。
    //!
    //! @details SetVertexBufferメソッドで、バッファをセットしておく必要があります。
    //!
    //! @return  頂点バッファの末尾を返します。
    //---------------------------------------------------------------------------
    void* CalculateVertexBuffer();

    //---------------------------------------------------------------------------
    //! @brief  インデックスバッファを計算します。
    //!
    //! @details SetIndexBufferメソッドで、バッファをセットしておく必要があります。
    //---------------------------------------------------------------------------
    template <typename T>
    void CalculateIndexBuffer();

    //--------------------------------------------------------------------------
    //! @brief  テクスチャ座標の計算処理。
    //!
    //! @param[in]  vertexX 頂点の X 座標。
    //! @param[in]  halfWidth   全体の幅の半分。
    //!
    //! @return テクスチャの U 座標。
    //--------------------------------------------------------------------------
    float CalculateTexMapU(float vertexX, float halfWidth)
    {
        return 0.5f + (vertexX / halfWidth * 0.5f);
    }

    //--------------------------------------------------------------------------
    //! @brief  テクスチャ座標の計算処理。
    //!
    //! @param[in]  vertexY 頂点の Y 座標。
    //! @param[in]  halfHeight   全体の高さの半分。
    //!
    //! @return テクスチャの V 座標。
    //--------------------------------------------------------------------------
    float CalculateTexMapV(float vertexY, float halfHeight)
    {
        // 3D 座標系の Y 軸とテクスチャ座標系の V が逆のため変換する。
        return 1.0f - (0.5f + (vertexY / halfHeight * 0.5f));
    }

    float   m_CornerRadiusH;
    float   m_CornerRadiusV;
    uint32_t    m_CornerSliceCount;
};

class Ui2dCircleShape : public nn::gfx::util::CircleShape
{
public:
    //---------------------------------------------------------------------------
    //! @brief コンストラクタです。
    //!
    //! @param[in]   vertexFormat          頂点バッファのフォーマット
    //! @param[in]   primitiveTopology     プリミティブトポロジーです。
    //!                                     ポリゴン: PrimitiveTopology_TriangleList
    //!                                   　ワイヤー: PrimitiveTopology_LineStrip
    //! @param[in]   sliceCount            円の分割数です。
    //!
    //! @details XY 平面上に原点を中心とした半径 1.0 の円を作成します。
    //---------------------------------------------------------------------------
    Ui2dCircleShape(
        nn::gfx::util::PrimitiveShapeFormat vertexFormat,
        nn::gfx::PrimitiveTopology  primitiveTopology,
        int                        sliceCount)
        : nn::gfx::util::CircleShape(vertexFormat, primitiveTopology, sliceCount)
    {
    }

    //---------------------------------------------------------------------------
    //! @brief  ポインタ以外のパラメータをコピーします。
    //---------------------------------------------------------------------------
    void CopyParams(const Ui2dCircleShape& src);
};

//---------------------------------------------------------------------------
// レイアウトがフレームバッファテクスチャのデスクリプタスロットを要求するかを取得します。

//! @brief 内部用機能のため使用禁止です。
//!
//! @param[in]  pLayout レイアウトへのポインタです。
//!
//! @return フレームバッファテクスチャのデスクリプタスロットが必要な場合は true を返します。
//!
//! @details 与えられたレイアウトでフレームバッファテクスチャデスクリプタスロットが必要かを再帰的に調べます。
//!
//! @sa nn::ui2d::DrawInfo::SetFramebufferTextureDescriptorSlot
//! @sa nn::ui2d::DrawInfo::SetFramebufferSamplerDescriptorSlot
//! @sa nn::ui2d::DrawInfo::GetFramebufferTextureDescriptorSlot
//! @sa nn::ui2d::DrawInfo::GetFramebufferSamplerDescriptorSlot
//!
//---------------------------------------------------------------------------
bool CheckFrameBufferTextureDescriptorSlotRequired(const Layout* pLayout);

//---------------------------------------------------------------------------
//! @brief ExtraParameter を利用する イージング形式かどうかを判定します。
//!
//! @param[in]  type イージング形式です。
//!
//! @return ExtraParameter を利用する場合、true を返します。
//!
static inline bool IsUseExtraParameterParameterizedAnimType(ParameterizedAnimType type)
{
    NN_UNUSED(type);
    return false;
}

} // namespace nn::ui2d
} // namespace nn
