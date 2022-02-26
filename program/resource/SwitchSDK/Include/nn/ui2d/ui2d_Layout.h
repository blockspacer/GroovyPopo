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

#include <new>

#include <nn/nn_Allocator.h>

#include <nn/util/util_IntrusiveList.h>

#include <nn/ui2d/ui2d_Animation.h>
#include <nn/ui2d/ui2d_Animator.h>
#include <nn/ui2d/ui2d_Pane.h>
#include <nn/ui2d/ui2d_Group.h>
#include <nn/ui2d/ui2d_Types.h>
#include <nn/ui2d/ui2d_Parts.h>

#include <nn/font/font_Types.h>
#include <nn/font/font_DispStringBuffer.h>

namespace nn
{
namespace ui2d
{

struct ResAnimShareInfoList;
struct ResPartsProperty;
struct ResExtUserDataList;
struct ResVec2;

class ResourceAccessor;
struct BuildResSet;
class DrawInfo;
class PaneAnimator;
class GroupAnimator;
class ControlCreator;
class TextSearcher;
class CaptureTexture;

namespace detail
{
class VectorGraphicsTexture;
class DynamicTextureShareInfo;
}


//! @brief アニメーションの一覧を保持します。
//!
typedef nn::util::IntrusiveList<AnimTransform, nn::util::IntrusiveListMemberNodeTraits<AnimTransform, &AnimTransform::m_Link> > AnimTransformList;

//! @brief 部品ペインの一覧を保持するリストの定義です。
//!
typedef nn::util::IntrusiveList<Parts, nn::util::IntrusiveListMemberNodeTraits<Parts, &Parts::m_Link> > PartsPaneList;

//---------------------------------------------------------------------------
//! @brief レイアウト全体を管理するためのクラスです。
//!
//! @sa Pane
//! @sa AnimTransform
//!
//---------------------------------------------------------------------------
class Layout
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO_ROOT();

    static const int LayoutNameLengthMax = 64; //!< レイアウト名の上限。リソース的には上限はないが、Layoutクラスの実装の都合上上限を設けている
    static const size_t DefaultAligment = 4;
    static const size_t AlignmentForPaneInstance = 16; //!< SIMD 命令で処理される可能性のあるメンバーを保持する Pane クラスのアライメント

    //----------------------------------------
    //! @name メモリ管理
    //@{

    //! @brief 現在のメモリ確保関数ポインタを取得します。
    //!
    //! @return 現在のメモリ確保関数ポインタを返します。
    //!
    //! @sa GetFreeFunction
    //!
    static nn::AlignedAllocateFunctionWithUserData GetAllocateFunction()
    {
        return g_pAllocateFunction;
    }

    //! @brief 現在のメモリ解放関数ポインタを取得します。
    //!
    //! @return 現在のメモ解放関数ポインタを返します。
    //!
    //! @sa GetUserDataForAllocator
    //!
    static nn::FreeFunctionWithUserData GetFreeFunction()
    {
        return g_pFreeFunction;
    }

    //! @brief 現在のメモリ確保関数用のユーザーデータを取得します。
    //!
    //! @return メモリ確保関数用のユーザーデータを返します。
    //!
    //! @sa GetAllocateFunction
    //!
    static void* GetUserDataForAllocator()
    {
        return g_pUserDataForAllocator;
    }

    //! @brief メモリブロックの確保に使用する関数ポインタを設定します。
    //!
    //! @param[in] pAllocateFunction   メモリ確保関数のポインタです。
    //! @param[in] pFreeFunction       メモリ解放関数のポインタです。
    //! @param[in] pUserData           メモリ確保・確保関数に引数として渡されるユーザーデータです。
    //!
    //! @sa AllocateMemory
    //! @sa GetAllocateFunction
    //!
    static void SetAllocator(nn::AlignedAllocateFunctionWithUserData pAllocateFunction, nn::FreeFunctionWithUserData pFreeFunction, void* pUserData);

    //! @brief メモリブロックを確保します。
    //!
    //! @details
    //! size バイトのメモリブロックを確保します。
    //! この関数を使用する前に Layout::SetAllocator() によりメモリ確保に使用する関数ポインタを
    //! 設定してください。
    //!
    //! @param[in] size         確保するメモリブロックのサイズです。
    //! @param[in] alignment    確保するメモリブロックのアライメント値です。
    //!
    //! @return
    //! メモリブロックを確保できた場合はメモリブロックの先頭アドレスを、
    //! 確保できなかった場合は NULL を返します。
    //!
    //! @sa SetAllocator
    //! @sa FreeMemory
    //!
    static void* AllocateMemory(size_t  size, size_t alignment);

    //! @brief デフォルトのアライメントでメモリブロックを確保します。
    //!
    //! @details
    //! size バイトのメモリブロックを確保します。
    //! この関数を使用する前に Layout::SetAllocator() によりメモリ確保に使用する関数ポインタを
    //! 設定してください。
    //!
    //! @param[in] size         確保するメモリブロックのサイズです。
    //!
    //! @return
    //! メモリブロックを確保できた場合はメモリブロックの先頭アドレスを、
    //! 確保できなかった場合は NULL を返します。
    //!
    //! @sa SetAllocator
    //! @sa FreeMemory
    //!
    static void* AllocateMemory(size_t  size);

    //! @brief メモリブロックを開放します。
    //!
    //! @param[in] pMemory  開放するメモリブロックの先頭アドレスです。
    //!
    //! @sa AllocateMemory
    //!
    static void FreeMemory(void* pMemory);

    //! @brief 動的レンダリングテクスチャの初期化に関するメモリ関連の設定を行います。
    //!
    //! @details
    //! 通常は設定する必要はありませんが、レイアウト内で使用しているキャプチャテクスチャやベクターグラフィックステクスチャなどの数が多く、初期化時にエラーとなるような場合に設定してください。
    //! Layout::Build() や Layout クラスのコピーコンストラクタ、ClonePaneTree 内で参照される値のため、それらの機能の呼び出し前に設定する必要があります。
    //! またこのメモリはスタック上から確保されるためスタックのメモリサイズや設定する値には注意してください。
    //!
    //! @param[in] captureTextureShareInfoCount         BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、１つのレイアウト内で初期化可能な CaptureTexture の最大数です。64bit 環境において 1 要素 48 byte 使用し、デフォルト値は 8 個です。
    //! @param[in] vectorGraphicsTextureShareInfoCount  BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、１つのレイアウト内で初期化可能な VectorGraphicsTexture の最大数です。64bit 環境において 1 要素 24 byte 使用し、デフォルト値は 8 個です。
    //! @param[in] dynamicTextureShareInfoCount         BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、１つのレイアウト内で初期化可能なレイアウトインスタンス(パーツ＋ボディ)の最大数です。64bit 環境において 1 要素 104 byte 使用し、デフォルト値は 16 個です。
    //! @param[in] dynamicTextureShareInfoPartsStackMax BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、動的レンダリングテクスチャの上書き設定が可能なパーツ階層の最大数です。64bit 環境において 1 要素 8 byte 使用し、デフォルト値は 4 です。
    //!
    static void SetDynamicTextureInitializationMemoryInfo(int captureTextureShareInfoCount, int vectorGraphicsTextureShareInfoCount, int dynamicTextureShareInfoCount, int dynamicTextureShareInfoPartsStackMax);

    // BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、１つのレイアウト内で初期化可能な CaptureTexture の最大数を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、１つのレイアウト内で初期化可能な CaptureTexture の最大数を返します。
    //!
    static int GetCaptureTextureShareInfoCountMax()
    {
        return g_CaptureTextureShareInfoCountMax;
    }

    // BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、１つのレイアウト内で初期化可能な VectorGraphicsTexture の最大数を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、１つのレイアウト内で初期化可能な VectorGraphicsTexture の最大数を返します。
    //!
    static int GetVectorGraphicsTextureShareInfoCountMax()
    {
        return g_VectorGraphicsTextureShareInfoCountMax;
    }

    // BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、１つのレイアウト内で初期化可能なレイアウトインスタンス(パーツ＋ボディ)の最大数を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、１つのレイアウト内で初期化可能なレイアウトインスタンス(パーツ＋ボディ)の最大数を返します。
    //!
    static int GetDynamicTextureShareInfoCountMax()
    {
        return g_DynamicTextureShareInfoCountMax;
    }

    // BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、動的レンダリングテクスチャの上書き設定が可能なパーツ階層の最大数を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、動的レンダリングテクスチャの上書き設定が可能なパーツ階層の最大数を返します。
    //!
    static int GetDynamicTextureShareInfoPartsStackMax()
    {
        return g_DynamicTextureShareInfoPartsStackMax;
    }

    //@}

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    Layout();

    /**
     *  @brief コピーコンストラクタです。
     *
     *  @details
     *  ペインツリーごとコピーします。そのペインツリー内に部品レイアウトがあった場合は、その
     *  部品レイアウトもコピーします。
     *
     *  部品レイアウトをコピーした場合、そのルートペインはペインのツリー階層にぶら下がって
     *  いませんので、任意の位置にAppendChild等でぶら下げてください。
     *
     *  コピー元のレイアウトでアニメーションを作成していてもここではコピーしませんのでご注意ください。
     *  グループについてはコピーします。
     *
     *  アプリケーションで独自にペインを継承して拡張している場合は、このメソッドの内部で使用
     *  しているClonePaneTreeWithPartsLayoutメソッドを含めて、Layoutクラスの継承先でコピー
     *  コンストラクタを再実装する必要がありますので、ご注意ください。
     *
     *  @param[in] pDevice          デバイスです。
     *  @param[in] src              コピー元のレイアウト
     *  @param[in] pParentLayout    部品レイアウトをコピーする場合、親のレイアウトのポインタを渡すと、
     *                              親のレイアウトの部品ペインリストにコピーしたレイアウトのルートを
     *                              登録します。
     *  @param[in] pRootPaneName    ルートペインの名前を変更する場合に指定します。
     *                              ルートペインの名前を変更しない場合はNULLを指定してください。
     *                              特に部品レイアウトをコピーする場合は、元と同じ名前だとペインの
     *                              検索を行う場合等に問題が発生するため、名前を変更することを推奨します。
     */
    explicit Layout(nn::gfx::Device* pDevice, const Layout& src, Layout* pParentLayout, const char* pRootPaneName)
    {
        CopyLayoutInstanceImpl(pDevice, src, pParentLayout, pRootPaneName);
    }

    // 動的レンダリングテクスチャのリソースをペインで管理する方式に対応したコピーコンストラクタです。ClonePaneTree などの内部で呼び出される機能のため非公開となっています。
    /**
     *  @brief 内部用機能のため使用しないでください。
     *
     *  @details
     *  機能としては公開されているコピーコンストラクタと同様です。
     *
     *  @param[in] pDevice          デバイスです。
     *  @param[in] src              コピー元のレイアウト
     *  @param[in] pParentLayout    部品レイアウトをコピーする場合、親のレイアウトのポインタを渡すと、
     *                              親のレイアウトの部品ペインリストにコピーしたレイアウトのルートを
     *                              登録します。
     *  @param[in] pRootPaneName    ルートペインの名前を変更する場合に指定します。
     *                              ルートペインの名前を変更しない場合はNULLを指定してください。
     *                              特に部品レイアウトをコピーする場合は、元と同じ名前だとペインの
     *                              検索を行う場合等に問題が発生するため、名前を変更することを推奨します。
     *  @param[in] pBuildPaneTreeContext ペインツリー構築時にレイアウトやパーツをまたいで情報を受け渡しするためのデータです。
     *
     */
    Layout(nn::gfx::Device* pDevice, const Layout& src, Layout* pParentLayout, const char* pRootPaneName, detail::BuildPaneTreeContext* pBuildPaneTreeContext)
    {
        CopyLayoutInstanceImpl(pDevice, src, pParentLayout, pRootPaneName, pBuildPaneTreeContext);
    }

    /**
     *  @brief コピーコンストラクタです。
     *
     *  @details
     *  ペインツリーごとコピーします。そのペインツリー内に部品レイアウトがあった場合は、その
     *  部品レイアウトもコピーします。
     *
     *  部品レイアウトをコピーした場合、そのルートペインはペインのツリー階層にぶら下がって
     *  いませんので、任意の位置にAppendChild等でぶら下げてください。
     *
     *  コピー元のレイアウトでアニメーションを作成していてもここではコピーしませんのでご注意ください。
     *  グループについてはコピーします。
     *
     *  アプリケーションで独自にペインを継承して拡張している場合は、このメソッドの内部で使用
     *  しているClonePaneTreeWithPartsLayoutメソッドを含めて、Layoutクラスの継承先でコピー
     *  コンストラクタを再実装する必要がありますので、ご注意ください。
     *
     *  @param[in] pDevice          デバイスです。
     *  @param[in] src              コピー元のレイアウト
     *  @param[in] pParentLayout    部品レイアウトをコピーする場合、親のレイアウトのポインタを渡すと、
     *                              親のレイアウトの部品ペインリストにコピーしたレイアウトのルートを
     *                              登録します。
     *  @param[in] pRootPaneName    ルートペインの名前を変更する場合に指定します。
     *                              ルートペインの名前を変更しない場合はNULLを指定してください。
     *                              特に部品レイアウトをコピーする場合は、元と同じ名前だとペインの
     *                              検索を行う場合等に問題が発生するため、名前を変更することを推奨します。
     * @param[in]  pCaptureTextureNewRootName キャプチャテクスチャをリソース中で識別するための新たなルート名を指定します。
     *                                        レイアウト毎にユニークな名前である必要があります。
     *                                        pRootPaneName を指定してレイアウトインスタンスを区別している場合は同じものを指定すれば問題ありません。
     *                                        ルート名が重複していると同名のレンダーターゲットテクスチャが複数登録されるため正しく動作しません。
     *
     * @deprecated ui2d ランタイムでキャプチャテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
     *             使用した場合は 4 引数版のコピーコンストラクタと同様の動作となります。
     *
     */
    NN_DEPRECATED explicit Layout(nn::gfx::Device* pDevice, const Layout& src, Layout* pParentLayout, const char* pRootPaneName, const char* pCaptureTextureNewRootName)
    {
        NN_UNUSED(pCaptureTextureNewRootName);
        CopyLayoutInstanceImpl(pDevice, src, pParentLayout, pRootPaneName);
    }

    /**
     *  @brief コピーコンストラクタです。
     *
     *  @details
     *  ペインツリーごとコピーします。そのペインツリー内に部品レイアウトがあった場合は、その
     *  部品レイアウトもコピーします。
     *
     *  部品レイアウトをコピーした場合、そのルートペインはペインのツリー階層にぶら下がって
     *  いませんので、任意の位置にAppendChild等でぶら下げてください。
     *
     *  コピー元のレイアウトでアニメーションを作成していてもここではコピーしませんのでご注意ください。
     *  グループについてはコピーします。
     *
     *  アプリケーションで独自にペインを継承して拡張している場合は、このメソッドの内部で使用
     *  しているClonePaneTreeWithPartsLayoutメソッドを含めて、Layoutクラスの継承先でコピー
     *  コンストラクタを再実装する必要がありますので、ご注意ください。
     *
     *  @param[in] pDevice          デバイスです。
     *  @param[in] src              コピー元のレイアウト
     *  @param[in] pParentLayout     部品レイアウトをコピーする場合、親のレイアウトのポインタを渡すと、
     *                              親のレイアウトの部品ペインリストにコピーしたレイアウトのルートを
     *                              登録します。
     */
    explicit Layout(nn::gfx::Device* pDevice, const Layout& src, Layout* pParentLayout)
    {
        CopyLayoutInstanceImpl(pDevice, src, pParentLayout, NULL);
    }

    /**
     *  @brief コピーコンストラクタです。
     *
     *  @details
     *  ペインツリーごとコピーします。そのペインツリー内に部品レイアウトがあった場合は、その
     *  部品レイアウトもコピーします。
     *
     *  部品レイアウトをコピーした場合、そのルートペインはペインのツリー階層にぶら下がって
     *  いませんので、任意の位置にAppendChild等でぶら下げてください。
     *
     *  コピー元のレイアウトでアニメーションを作成していてもここではコピーしませんのでご注意ください。
     *  グループについてはコピーします。
     *
     *  アプリケーションで独自にペインを継承して拡張している場合は、このメソッドの内部で使用
     *  しているClonePaneTreeWithPartsLayoutメソッドを含めて、Layoutクラスの継承先でコピー
     *  コンストラクタを再実装する必要がありますので、ご注意ください。
     *
     *  @param[in] pDevice          デバイスです。
     *  @param[in] src              コピー元のレイアウト
     */

    explicit Layout(nn::gfx::Device* pDevice, const Layout& src)
    {
        CopyLayoutInstanceImpl(pDevice, src, NULL, NULL);
    }


    //! @brief デストラクタです。
    //!
    virtual ~Layout();

    //! @brief リソースを開放します。
    //!
    //! @details
    //! デストラクタの呼び出し以前に呼び出す必要があります。
    //!
    //! @param[in] pDevice          デバイスです。
    //!
    void Finalize(nn::gfx::Device* pDevice);

    //@}

    //----------------------------------------
    //! @name 初期化
    //@{

    //! @brief 初期化のオプションを表す構造体です。
    //!
    struct BuildOption
    {
        //! @brief ルートペインを部品ペインにするか否か
        //!
        //! @details
        //! デフォルトではfalseになっています。
        //!
        //! 通常、レイアウトのルートペインはnn::ui2d::Pane(ヌルペイン)になりますが、この
        //! オプションをtrueにすると、ルートペインがnn::ui2d::Parts(部品ペイン)になります。
        //! これにより、構築したレイアウトを何か他のレイアウトにくっつけたときに、部品
        //! としてレイアウトエディタ上で配置したのと同じ状態にすることができ、
        //! FindPaneByNameしたときに、くっつけたレイアウトの中を探しに行かないようになります。
        //!
        //! なお、構築したレイアウトを他のレイアウトにくっつける場合、部品として配置した
        //! のと同じ状態にするには、ペインのChildListに入れるだけでなく、レイアウトの
        //! PartsPaneListに入れる必要があることにご注意ください。詳しくはGetPartsPaneList
        //! を参照してください。
        //!
        //! @sa FindPaneByName
        //! @sa GetPartsPaneList
        //!
        bool isRootPaneParts;

        //! @brief デフォルト値を設定します。
        //!
        void SetDefault()
        {
            isRootPaneParts = false;
            pGetUserShaderInformationFromUserDataCallback = NULL;
            pGetUserShaderInformationFromUserDataCallbackUserData = NULL;
            isLinefeedByCharacterHeightEnabled = false;
            isDummyVectorGraphicsTextureInfoUsed = false;
        }

        //! @brief ユーザーシェーダー情報取得コールバック関数です。
        //!
        //! @details
        //! デフォルトでは NULL になっています。
        //!
        //! このコールバック関数が設定されていると、Build を呼び出した際のマテリアルの初期化時に
        //! この関数を呼び出してユーザーシェーダーに関する情報の取得を試みます。
        //!
        GetUserShaderInformationFromUserData    pGetUserShaderInformationFromUserDataCallback;

        //! @brief ユーザーシェーダー情報取得コールバック関数へ渡されるユーザーポインタです。
        //!
        //! @details
        //! デフォルトでは NULL になっています。
        //!
        void*                                   pGetUserShaderInformationFromUserDataCallbackUserData;

        //! @brief テキストボックスが文字の高さに応じて改行幅を自動調整するかどうかです。
        //!
        //! @details
        //! デフォルトでは false になっています。
        //!
        bool isLinefeedByCharacterHeightEnabled;

        //! @brief ベクターグラフィックステクスチャを作成する際に、実際のベクターグラフィックステクスチャは作成せずにダミーのテクスチャを作成します。(内部用機能のため使用しないでください)
        //!
        //! @details
        //! デフォルトでは false になっています。
        //!
        bool isDummyVectorGraphicsTextureInfoUsed;
    };

    //----------------------------------------------------------------------
    //! レイアウト構築処理の状態記憶をする構造体です。
    struct LayoutBuildContext
    {
        BuildResSet                 buildResSet;
        BuildArgSet                 buildArgSet;

        // 現在処理中のバイナリブロック
        int                         index;
        const void*                 pData;
        const ResExtUserDataList*   pResExtUserDataList;
        uint32_t                    kind;

        // 次に処理されるバイナリブロック
        int                         indexNext;
        const void*                 pDataNext;

        // 最後に作成されたペイン
        Pane*                       pLastBuiltPane;

        // グループネストレベル
        int                         groupNestLevel;
    };

    //! @brief レイアウトリソースからペインリソースを名前で検索します。
    //!
    //! @details
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! @param[out] ppOutResPane            発見されたペインリソース。発見されてなければ NULL が設定されます。
    //! @param[out] ppOutExtUserDataList    発見されたペインリソースに紐づく、拡張ユーザー情報リソース。発見されてなければ NULL が設定されます。
    //! @param[in] pLayoutResource          レイアウトリソースです。GetLayoutResource() で取得します。
    //! @param[in] pName                    検索するペインの名前です。
    //! @param[in] pSearchStartPane         検索を開始するペインです。指定したペインの次のペインから検索を開始します。
    //!
    static void FindResPaneByName(
        const nn::ui2d::ResPane** ppOutResPane, const nn::ui2d::ResExtUserDataList** ppOutExtUserDataList,
        const void* pLayoutResource, const char* pName, const nn::ui2d::ResPane* pSearchStartPane);

    //! @brief リソースをもとにレイアウトの初期化を行います。
    //!
    //! @details
    //! リソースのフォーマットが正しくなければ失敗します。
    //! なお、このメソッドで指定したリソースアクセサは内部に保持され、
    //! CreateAnimTransformやGetAnimResourceDataで使われます。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報
    //! @param[in] pDevice              デバイスです。
    //! @param[in] pResAcsr         リソースアクセサへのポインタです。
    //! @param[in] pControlCreator  コントロールの作成を行うオブジェクトへのポインタです。
    //!                                 NULLだったときは、レイアウトリソースにコントロールが含まれて
    //!                                 いてもコントロールを作成しません。デフォルトではNULLです。
    //! @param[in] pTextSearcher    TextBoxにテキストIDが設定されているときに、対応するテキスト
    //!                                 を検索するためのオブジェクトへのポインタです。NULLだったときは、
    //!                                 リソースのテキストがそのまま設定されます。デフォルトはNULLです。
    //! @param[in] pLayoutResource      レイアウトリソースへのポインタです。
    //! @param[in] buildOption          構築時のオプションです。
    //! @param[in] utf8                 UTF-8 で構築するか否かです。
    //!
    //! @return 初期化に成功した場合は true を、失敗した場合は false を返します。
    //!
    bool Build(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        ResourceAccessor* pResAcsr,
        ControlCreator* pControlCreator,
        TextSearcher* pTextSearcher,
        const void* pLayoutResource,
        const BuildOption& buildOption,
        bool utf8);

    bool Build(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        ResourceAccessor* pResAcsr,
        ControlCreator* pControlCreator,
        TextSearcher* pTextSearcher,
        const void* pLayoutResource,
        const BuildOption& buildOption)
    {
        return Build(pOutBuildResultInformation, pDevice, pResAcsr, pControlCreator, pTextSearcher, pLayoutResource, buildOption, false);
    }

    bool Build(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        ResourceAccessor* pResAcsr,
        ControlCreator* pControlCreator,
        const void* pLayoutResource)
    {
        BuildOption buildOption = BuildOption();
        buildOption.SetDefault();
        return Build(pOutBuildResultInformation, pDevice, pResAcsr, pControlCreator, NULL, pLayoutResource, buildOption);
    }

    bool Build(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        ResourceAccessor* pResAcsr,
        const void* pLayoutResource)
    {
        BuildOption buildOption = BuildOption();
        buildOption.SetDefault();
        return Build(pOutBuildResultInformation, pDevice, pResAcsr, NULL, NULL, pLayoutResource, buildOption);
    }


    //! @brief
    //! 指定されたレイアウトファイル名でリソースアクセサからリソースを取得し、
    //! それをもとにレイアウトの初期化を行います。
    //!
    //! @details
    //! レイアウトファイル名が正しくなければ失敗します。
    //! なお、このメソッドで指定したリソースアクセサは内部に保持され、
    //! CreateAnimTransformやGetAnimResourceDataで使われます。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報
    //! @param[in] pDevice              デバイスです。
    //! @param[in] pResAcsr         リソースアクセサへのポインタです。
    //! @param[in] pControlCreator  コントロールの作成を行うオブジェクトへのポインタです。
    //!                                 NULLだったときは、レイアウトリソースにコントロールが含まれて
    //!                                 いてもコントロールを作成しません。デフォルトではNULLです。
    //! @param[in] pTextSearcher    TextBoxにテキストIDが設定されているときに、対応するテキスト
    //!                                 を検索するためのオブジェクトへのポインタです。NULLだったときは、
    //!                                 リソースのテキストがそのまま設定されます。デフォルトはNULLです。
    //! @param[in] buildOption          構築時のオプションです。
    //! @param[in] pLayoutFileName      レイアウトファイル名です。コンバート後の拡張子(bflyt)付きで指定してください。
    //! @param[in] utf8                 UTF-8 で構築するか否かです。
    //!
    //! @return 初期化に成功した場合は true を、失敗した場合は false を返します。
    //!
    bool BuildWithName(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        ResourceAccessor* pResAcsr,
        ControlCreator* pControlCreator,
        TextSearcher* pTextSearcher,
        const BuildOption& buildOption,
        const char* pLayoutFileName,
        bool utf8);
    bool BuildWithName(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        ResourceAccessor* pResAcsr,
        ControlCreator* pControlCreator,
        TextSearcher* pTextSearcher,
        const BuildOption& buildOption,
        const char* pLayoutFileName)
    {
        return BuildWithName(pOutBuildResultInformation, pDevice, pResAcsr, pControlCreator, pTextSearcher, buildOption, pLayoutFileName, false);
    }

    //! @brief
    //! 指定されたレイアウトファイル名でリソースアクセサからリソースを取得し、
    //! それをもとにレイアウトの初期化を行います。
    //!
    //! @details
    //! レイアウトファイル名が正しくなければ失敗します。
    //! なお、このメソッドで指定したリソースアクセサは内部に保持され、
    //! CreateAnimTransformやGetAnimResourceDataで使われます。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報
    //! @param[in] pDevice              デバイスです。
    //! @param[in] pResAcsr             リソースアクセサへのポインタです。
    //! @param[in] pLayoutFileName      レイアウトファイル名です。コンバート後の拡張子(bflyt)付きで指定してください。
    //!
    //! @return 初期化に成功した場合は true を、失敗した場合は false を返します。
    //!
    bool BuildWithName(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        ResourceAccessor* pResAcsr,
        const char* pLayoutFileName)
    {
        BuildOption buildOption = BuildOption();
        buildOption.SetDefault();
        return BuildWithName(pOutBuildResultInformation, pDevice, pResAcsr, NULL, NULL, buildOption, pLayoutFileName);
    }

    //! @brief レイアウトに関連づいたアニメーションの、区間タグ名の数を取得します。
    //!
    //! @details
    //! バイナリコンバーターで出力を抑止した場合、取得できず 0 が返ります。
    //!
    int AcquireAnimTagNameCount() const;

    //! @brief レイアウトに関連づいたアニメーションの、区間タグ名を取得します。
    //!
    //! @details
    //! 無効な index が指定された場合、アサートに失敗します。
    //! 利用前に AcquireAnimTagNameCount() で取得可能な数を確認してください。
    //!
    //! @param[in] index
    const char* AcquireAnimTagNameByIndex(int index) const;

    //! @brief 全ペインのグローバルマトリックスを計算します。
    //!
    //! @details
    //! ペインを初期化した直後のグローバルマトリックスは単位行列が設定されていますが、
    //! このメソッドを初期化直後に呼び出すことで 0 フレーム目のグローバルマトリックスの状態にできます。
    //! アニメーションを再生したり SetTranslate 等で SRT を設定していた際には
    //! それらの SRT が反映された状態でグローバルマトリックスが計算されます。
    //!
    //! 本メソッドは、全てのグローバルマトリックスが再計算されます。
    //! 変更があったグローバルマトリックスだけを計算したい場合には同名のオーバーロード関数を使用してください。
    //!
    //! @param[in] drawInfo 描画情報です。
    void CalculateGlobalMatrix(DrawInfo& drawInfo)
    {
        CalculateGlobalMatrix(drawInfo, true);
    }

    //! @brief 全ペインのグローバルマトリックスを計算します。
    //!
    //! @details
    //! ペインを初期化した直後のグローバルマトリックスは単位行列が設定されていますが、
    //! このメソッドを初期化直後に呼び出すことで 0 フレーム目のグローバルマトリックスの状態にできます。
    //! アニメーションを再生したり SetTranslate 等で SRT を設定していた際には
    //! それらの SRT が反映された状態でグローバルマトリックスが計算されます。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] forceGlobalMatrixDirty 全てのグローバルマトリックスを再計算するか否かです。
    void CalculateGlobalMatrix(DrawInfo& drawInfo, bool forceGlobalMatrixDirty);

    //@}

    //----------------------------------------
    //! @name アニメーション
    //@{

    //! @brief アニメーションを作成します。
    //!
    //! @details
    //! アニメーションオブジェクトの作成のみ行います。
    //!
    //! @return 生成したアニメーションへのポインタを返します。
    //!
    template <typename T>
    T* CreateAnimTransform()
    {
        NN_SDK_ASSERT_NOT_NULL(g_pAllocateFunction);

        T *const pAnimTrans = AllocateAndConstruct<T>();
        if (pAnimTrans)
        {
            this->GetAnimTransformList().push_back(*pAnimTrans);
        }
        return pAnimTrans;
    }

    //! @brief アニメーションを作成します。
    //!
    //! @details
    //! アニメーションオブジェクトを作成し、指定されたリソースを関連付けます。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! アニメーションリソースにアニメーション共有情報が含まれる場合は、
    //! BindAnimationAuto() を使用してください。
    //!
    //! @param[in] pDevice      デバイスです。パターンアニメーションのテクスチャをロードするのに使用されます。
    //! @param[in] pAnimResource   アニメーションリソースへのポインタです。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合は NULL を返します。
    //!
    //! @sa BindAnimationAuto
    //!
    template <typename T>
    T* CreateAnimTransform(nn::gfx::Device* pDevice, const void* pAnimResource)
    {
        return CreateAnimTransform<T>(pDevice, AnimResource(pAnimResource));
    }

    //! @brief アニメーションを作成します。
    //!
    //! @details
    //! アニメーションオブジェクトを作成し、指定されたリソースを関連付けます。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! アニメーションリソースにアニメーション共有情報が含まれる場合は、
    //! BindAnimationAuto() を使用してください。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] animRes  アニメーションリソースオブジェクトです。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合は NULL を返します。
    //!
    //! @sa BindAnimationAuto
    //!
    template <typename T>
    T* CreateAnimTransform(nn::gfx::Device* pDevice, const AnimResource& animRes)
    {
        const ResAnimationBlock *const pAnimBlock = animRes.GetResourceBlock();
        if (! pAnimBlock)
        {
            return NULL;
        }

        T *const pAnimTrans = CreateAnimTransform<T>();
        if (pAnimTrans)
        {
            pAnimTrans->SetResource(pDevice, m_pResourceAccessor, pAnimBlock);
        }

        return pAnimTrans;
    }

    //! @brief アニメーションを作成します。
    //!
    //! @details
    //! アニメーションオブジェクトを作成し、指定されたタグ名のリソースを関連付けます。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! アニメーションリソースにアニメーション共有情報が含まれる場合は、
    //! BindAnimationAuto() を使用してください。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pTagName アニメーションのタグ名です。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    //! @sa BindAnimationAuto
    //!
    template <typename T>
    T* CreateAnimTransform(nn::gfx::Device* pDevice, const char* pTagName)
    {
        const void*   pPtr = GetAnimResourceData(pTagName);
        return CreateAnimTransform<T>(pDevice, pPtr);
    }

    //! @brief アニメーションを作成します。
    //!
    //! @details
    //! アニメーションオブジェクトの作成のみ行います。
    //!
    //! @return 生成したアニメーションへのポインタを返します。
    //!
    AnimTransformBasic* CreateAnimTransformBasic();

    //! @brief アニメーションを作成します。
    //!
    //! @details
    //! アニメーションオブジェクトを作成し、指定されたリソースを関連付けます。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! アニメーションリソースにアニメーション共有情報が含まれる場合は、
    //! BindAnimationAuto() を使用してください。
    //!
    //! @param[in] pDevice      デバイスです。
    //! @param[in] pAnimResource   アニメーションリソースへのポインタです。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合は NULL を返します。
    //!
    //! @sa BindAnimationAuto
    //!
    AnimTransformBasic* CreateAnimTransformBasic(nn::gfx::Device* pDevice, const void* pAnimResource);

    //! @brief アニメーションを作成します。
    //!
    //! @details
    //! アニメーションオブジェクトを作成し、指定されたリソースを関連付けます。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! アニメーションリソースにアニメーション共有情報が含まれる場合は、
    //! BindAnimationAuto() を使用してください。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] animRes  アニメーションリソースオブジェクトです。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合は NULL を返します。
    //!
    //! @sa BindAnimationAuto
    //!
    AnimTransformBasic* CreateAnimTransformBasic(nn::gfx::Device* pDevice, const AnimResource& animRes);

    //! @brief アニメーションを作成します。
    //!
    //! @details
    //! アニメーションオブジェクトを作成し、指定されたタグ名のリソースを関連付けます。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! アニメーションリソースにアニメーション共有情報が含まれる場合は、
    //! BindAnimationAuto() を使用してください。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pTagName アニメーションのタグ名です。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    //! @sa BindAnimationAuto
    //!
    AnimTransformBasic* CreateAnimTransformBasic(nn::gfx::Device* pDevice, const char* pTagName);

    //! @brief タグ名と関連づけたいペインへのポインタからPaneAnimatorを作成します。
    //!
    //! @details
    //! PaneAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、指定されたペインにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pTagName アニメーションのタグ名です。
    //! @param[in] pPane     関連づけるペインへのポインタです。
    //! @param[in] enabled   開始時にアニメーションを有効状態にするか。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    PaneAnimator* CreatePaneAnimator(nn::gfx::Device* pDevice, const char* pTagName, Pane* pPane, bool enabled);

    //! @brief タグ名と関連づけたいペインへのポインタからPaneAnimatorを作成します。
    //!
    //! @details
    //! PaneAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、指定されたペインにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //! また、開始時にアニメーションが有効状態になります。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pTagName  アニメーションのタグ名です。
    //! @param[in] pPane     関連づけるペインへのポインタです。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    PaneAnimator* CreatePaneAnimator(nn::gfx::Device* pDevice, const char* pTagName, Pane* pPane)
    {
        return CreatePaneAnimator(pDevice, pTagName, pPane, true);
    }

    //! @brief タグ名と関連づけたいペイン名からPaneAnimatorを作成します。
    //!
    //! @details
    //! PaneAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、指定されたペインにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pTagName アニメーションのタグ名です。
    //! @param[in] pPaneName 関連づけるペインの名前です。
    //! @param[in] enabled   開始時にアニメーションを有効状態にするか。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    PaneAnimator* CreatePaneAnimator(nn::gfx::Device* pDevice, const char* pTagName, const char* pPaneName, bool enabled)
    {
        return CreatePaneAnimator(pDevice, pTagName, m_pRootPane->FindPaneByName(pPaneName), enabled);
    }

    //! @brief タグ名と関連づけたいペイン名からPaneAnimatorを作成します。
    //!
    //! @details
    //! PaneAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、指定されたペインにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //! また、開始時にアニメーションが有効状態になります。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pTagName  アニメーションのタグ名です。
    //! @param[in] pPaneName 関連づけるペインの名前です。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    PaneAnimator* CreatePaneAnimator(nn::gfx::Device* pDevice, const char* pTagName, const char* pPaneName)
    {
        return CreatePaneAnimator(pDevice, pTagName, pPaneName, true);
    }

    //! @brief タグ名と関連づけたいグループへのポインタからGroupAnimatorを作成します。
    //!
    //! @details
    //! GroupAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、指定されたグループにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pTagName アニメーションのタグ名です。
    //! @param[in] pGroup   関連づけるグループへのポインタです。
    //! @param[in] enabled  開始時にアニメーションを有効状態にするか。
    //!
    //! @return
    //! アニメーションへのポインタを返します。
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    GroupAnimator* CreateGroupAnimator(nn::gfx::Device* pDevice, const char* pTagName, Group* pGroup, bool enabled);

    //! @brief タグ名と関連づけたいグループへのポインタからGroupAnimatorを作成します。
    //!
    //! @details
    //! GroupAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、指定されたグループにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //! また、開始時にアニメーションが有効状態になります。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pTagName  アニメーションのタグ名です。
    //! @param[in] pGroup    関連づけるグループへのポインタです。
    //!
    //! @return
    //! アニメーションへのポインタを返します。
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    GroupAnimator* CreateGroupAnimator(nn::gfx::Device* pDevice, const char* pTagName, Group* pGroup)
    {
        return CreateGroupAnimator(pDevice, pTagName, pGroup, true);
    }

    //! @brief タグ名と関連づけたいグループ名からGroupAnimatorを作成します。
    //!
    //! @details
    //! GroupAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、指定されたグループにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! @param[in] pDevice      デバイスです。
    //! @param[in] pTagName     アニメーションのタグ名です。
    //! @param[in] pGroupName    関連づけるグループの名前です。
    //! @param[in] enabled       開始時にアニメーションを有効状態にするか。
    //!
    //! @return
    //! アニメーションへのポインタを返します。
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    GroupAnimator* CreateGroupAnimator(nn::gfx::Device* pDevice, const char* pTagName, const char* pGroupName, bool enabled)
    {
        return CreateGroupAnimator(pDevice, pTagName, m_pGroupContainer->FindGroupByName(pGroupName), enabled);
    }

    //! @brief タグ名と関連づけたいグループ名からGroupAnimatorを作成します。
    //!
    //! @details
    //! GroupAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、指定されたグループにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //! また、開始時にアニメーションが有効状態になります。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! @param[in] pDevice      デバイスです。
    //! @param[in] pTagName      アニメーションのタグ名です。
    //! @param[in] pGroupName    関連づけるグループの名前です。
    //!
    //! @return
    //! アニメーションへのポインタを返します。
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    GroupAnimator* CreateGroupAnimator(nn::gfx::Device* pDevice, const char* pTagName, const char* pGroupName)
    {
        return CreateGroupAnimator(pDevice, pTagName, pGroupName, true);
    }

    //! @brief 指定したタグ名のGroupAnimatorを作成し、アニメーションタグに関連づけられたグループにバインドします。
    //!
    //! @details
    //! GroupAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、タグの関連グループの先頭のグループにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pTagName アニメーションのタグ名です。
    //! @param[in] enabled   開始時にアニメーションを有効状態にするか。
    //!
    //! @return
    //! アニメーションへのポインタを返します。
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    GroupAnimator* CreateGroupAnimator(nn::gfx::Device* pDevice, const char* pTagName, bool enabled)
    {
        return CreateGroupAnimatorWithIndex(pDevice, pTagName, 0, enabled);
    }

    //! @brief 指定したタグ名のGroupAnimatorを作成し、アニメーションタグに関連づけられたグループにバインドします。
    //!
    //! @details
    //! GroupAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、タグの関連グループの先頭のグループにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //! また、開始時にアニメーションが有効状態になります。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pTagName  アニメーションのタグ名です。
    //!
    //! @return
    //! アニメーションへのポインタを返します。
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    GroupAnimator* CreateGroupAnimator(nn::gfx::Device* pDevice, const char* pTagName)
    {
        return CreateGroupAnimator(pDevice, pTagName, true);
    }

    //! @brief タグ名と関連づけたいグループのインデックスからGroupAnimatorを作成します。
    //!
    //! @details
    //! GroupAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、タグの関連グループのindex番目のグループにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! アニメーションタグに複数のグループを関連づけているときに、バインドしたいグループを
    //! インデックスを指定する場合に使用するメソッドです。
    //!
    //! @param[in] pDevice      デバイスです。
    //! @param[in] pTagName     アニメーションのタグ名です。
    //! @param[in] groupIndex   アニメーションタグの関連グループ内で、関連づけたいグループが何番目にあるかを指定します。
    //! @param[in] enabled       開始時にアニメーションを有効状態にするか。
    //!
    //! @return
    //! アニメーションへのポインタを返します。
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    GroupAnimator* CreateGroupAnimatorWithIndex(nn::gfx::Device* pDevice, const char* pTagName, int  groupIndex, bool enabled);

    //! @brief タグ名と関連づけたいグループのインデックスからGroupAnimatorを作成します。
    //!
    //! @details
    //! GroupAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、タグの関連グループのindex番目のグループにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //! また、開始時にアニメーションが有効状態になります。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! アニメーションタグに複数のグループを関連づけているときに、バインドしたいグループを
    //! インデックスを指定する場合に使用するメソッドです。
    //!
    //! @param[in] pDevice      デバイスです。
    //! @param[in] pTagName      アニメーションのタグ名です。
    //! @param[in] groupIndex   アニメーションタグの関連グループ内で、関連づけたいグループが何番目にあるかを指定します。
    //!
    //! @return
    //! アニメーションへのポインタを返します。
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    GroupAnimator* CreateGroupAnimatorWithIndex(nn::gfx::Device* pDevice, const char* pTagName, int  groupIndex)
    {
        return CreateGroupAnimatorWithIndex(pDevice, pTagName, groupIndex, true);
    }

    //! @brief 指定したタグ名のGroupArrayAnimatorを作成し、アニメーションタグに関連づけられた全てのグループにバインドします。
    //!
    //! @details
    //! GroupArrayAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、アニメーションタグに関連づけられた全てのグループにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! アニメーションタグにグループが一つも設定されていない場合は構築に失敗し、
    //! NULLを返します。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pTagName アニメーションのタグ名です。
    //! @param[in] enabled   開始時にアニメーションを有効状態にするか。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    GroupArrayAnimator* CreateGroupArrayAnimator(nn::gfx::Device* pDevice, const char* pTagName, bool enabled);

    //! @brief 指定したタグ名のGroupArrayAnimatorを作成し、アニメーションタグに関連づけられた全てのグループにバインドします。
    //!
    //! @details
    //! GroupArrayAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、アニメーションタグに関連づけられた全てのグループにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //! また、開始時にアニメーションが有効状態になります。
    //!
    //! アニメーションタグにグループが一つも設定されていない場合は構築に失敗し、
    //! NULLを返します。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pTagName  アニメーションのタグ名です。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    GroupArrayAnimator* CreateGroupArrayAnimator(nn::gfx::Device* pDevice, const char* pTagName)
    {
        return CreateGroupArrayAnimator(pDevice, pTagName, true);
    }

    //! @brief 指定したタグ名のAnimatorを作成し、アニメーションタグに関連づけられたグループ全てにバインドします。
    //!
    //! @details
    //! 指定されたタグの関連グループが1つの場合はGroupAnimatorを、2つ以上の場合は
    //! GroupArrayAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、アニメーションタグに関連づけられた全てのグループにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! 作られるAnimatorがGroupAnimatorかGroupArrayAnimatorかわからないという問題点が
    //! ありますが、CreateGroupArrayAnimatorと比較してメモリ消費を抑えることができます。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pTagName アニメーションのタグ名です。
    //! @param[in] enabled   開始時にアニメーションを有効状態にするか。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    Animator* CreateGroupAnimatorAuto(nn::gfx::Device* pDevice, const char* pTagName, bool enabled);

    //! @brief 指定したタグ名のAnimatorを作成し、アニメーションタグに関連づけられたグループ全てにバインドします。
    //!
    //! @details
    //! 指定されたタグの関連グループが1つの場合はGroupAnimatorを、2つ以上の場合は
    //! GroupArrayAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、アニメーションタグに関連づけられた全てのグループにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //! また、開始時にアニメーションが有効状態になります。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! 作られるAnimatorがGroupAnimatorかGroupArrayAnimatorかわからないという問題点が
    //! ありますが、CreateGroupArrayAnimatorと比較してメモリ消費を抑えることができます。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pTagName  アニメーションのタグ名です。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合の動作は未定義であり、ASSERT で停止します。
    //!
    Animator* CreateGroupAnimatorAuto(nn::gfx::Device* pDevice, const char* pTagName)
    {
        return CreateGroupAnimatorAuto(pDevice, pTagName, true);
    }

    //! @brief アニメーションを削除します。
    //!
    //! @param[in] pAnimTransform   アニメーションへのポインタです。
    //!
    virtual void DeleteAnimTransform(
        AnimTransform* pAnimTransform);

    //! @brief アニメーションを関連付けます。
    //!
    //! @details
    //! レイアウトに属する全てのペインについて関連付けを行います。
    //!
    //! @param[in] pAnimTrans   アニメーションへのポインタです。
    //!
    //! @sa UnbindAnimation
    //! @sa UnbindAllAnimation
    //!
    virtual void BindAnimation(AnimTransform* pAnimTrans);

    //! @brief アニメーションの関連付けを解除します。
    //!
    //! @details
    //! レイアウトに属する全てのペインについて関連付けを解除します。
    //!
    //! @param[in] pAnimTrans   アニメーションへのポインタです。
    //!
    //! @sa BindAnimation
    //! @sa UnbindAllAnimation
    //!
    virtual void UnbindAnimation(AnimTransform* pAnimTrans);

    //! @brief 指定したペインのアニメーションの関連付けを解除します。
    //!
    //! @details
    //! レイアウトに属する全てのアニメーションについて、指定したペインの
    //! 関連付けを解除します。
    //!
    //! @param[in] pPane    ペインへのポインタです。
    //!
    //! @sa BindAnimation
    //! @sa UnbindAllAnimation
    //!
    virtual void UnbindAnimation(Pane* pPane);

    //! @brief アニメーションとペインの関連付けをすべて解除します。
    //!
    //! @sa BindAnimation
    //! @sa UnbindAnimation
    //!
    virtual void UnbindAllAnimation();

    //! @brief アニメーションの作成と関連付けを一度に行います。
    //!
    //! @details
    //! アニメーションリソースオブジェクトの内容に応じて、
    //! アニメーションオブジェクト(AnimTransform)を作成し、
    //! ペインに関連付けます。
    //!
    //! アニメーションファイルにアニメーション共有情報が含まれている場合は、
    //! 共有対象のペインごとにアニメーションオブジェクト(AnimTransform)が
    //! 作成されます。
    //!
    //! また、アニメーション区間タグが含まれていて、関連グループが
    //! 指定されている場合は、アニメーションを関連付けるペインを関連グループに限定します。
    //!
    //! この関数では、アニメーションは無効状態で関連付けます。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] animRes  アニメーションリソースオブジェクトです。
    //!
    //! @return 関数が成功した場合は true、失敗した場合は false を返します。
    //!
    //! @sa CreateAnimTransform
    //!
    virtual bool BindAnimationAuto(nn::gfx::Device* pDevice, const AnimResource& animRes);

    //! @brief 全てのアニメーションの結果を計算します。
    //!
    virtual void Animate();

    //! @brief 全てのアニメーションのフレームを進めます。
    //!
    //! @details
    //! レイアウトが部品を持っている場合は、そのレイアウトのアニメーションのフレームも進めます。
    //!
    //! @param[in] progressFrame 進めるフレーム数です。
    //!
    virtual void UpdateAnimFrame(float progressFrame = 1.0f);

    //! @brief 全てのアニメーションについて、結果を計算した後にフレームを進めます。
    //!
    //! @details
    //! レイアウトが部品を持っている場合は、そのレイアウトの結果の計算とアニメーションのフレーム
    //! を進める処理も行います。
    //!
    //! AnimateとUpdateAnimFrameを別々に行うよりも処理が速くなります。別々のタイミングで行う必要
    //! がないのであれば、こちらを使用してください。
    //!
    //! @param[in] progressFrame 進めるフレーム数です。
    //!
    virtual void AnimateAndUpdateAnimFrame(float progressFrame = 1.0f);

    //! @brief 指定したタグ名のアニメーションリソースデータを取得します。
    //!
    //! @param[in] pTagName  アニメーションのタグ名
    //!
    const void* GetAnimResourceData(const char* pTagName) const;

    //@}

    //----------------------------------------
    //! @name 描画
    //@{

    //! @brief 描画のための計算をします。
    //!
    //! @details
    //! このメソッドの実装は nn::ui2d::Layout::CalculateImpl() で行っています。
    //! このメソッドの処理をオーバーライドしたい場合は、nn::ui2d::Layout::CalculateImpl() をオーバーライドしてお使いください。
    //!
    //! @param[in] drawInfo         描画情報です。
    //!
    void Calculate(DrawInfo& drawInfo)
    {
        const bool forceGlbMtxDirty = false;
        CalculateImpl(drawInfo, forceGlbMtxDirty);
    }

    //! @brief 描画のための計算をします。
    //!
    //! @details
    //! 引数drawInfoで与える DrawInfo は、関数内部で更新されます。複数のスレッドで共有できないので注意してください。
    //! 複数スレッドで、処理を行う場合は、DrawInfo をスレッド毎に複製して実行するようにしてください。
    //!
    //! 引数forceGlbMtxDirtyは、ペインのグローバルマトリックスを強制的に再計算させる場合に
    //! trueを指定します。ペインは、初回計算時もしくは自分自身と親のSRT及びサイズが変更された
    //! 場合のみグローバルマトリックスを再計算する仕組みになっていますが、DrawInfoに設定した
    //! ビュー行列が変更された場合も再計算が必要になります。
    //!
    //! DrawInfoに設定したビュー行列が変更された場合はforceGlbMtxDirtyにtrueを与えるように
    //! してください。
    //!
    //! この引数は、正射影で描画している場合はほぼ指定する必要はありません。透視射影でカメラを
    //! 動かしているような場合に指定する必要があります。
    //!
    //! このメソッドの実装は nn::ui2d::Layout::CalculateImpl() で行っています。
    //! このメソッドの処理をオーバーライドしたい場合は、nn::ui2d::Layout::CalculateImpl() をオーバーライドしてお使いください。
    //!
    //! @param[in] drawInfo         描画情報です。
    //! @param[in] forceGlbMtxDirty 全てのグローバルマトリックスを再計算するか否かです。
    //!
    //! @sa TextBox::ClearDirtyFlag
    //!
    void Calculate(DrawInfo& drawInfo, bool forceGlbMtxDirty)
    {
        CalculateImpl(drawInfo, forceGlbMtxDirty);
    }

    //! @brief 描画処理を行います。
    //!
    //! @details
    //! drawInfo には、あらかじめ DrawInfo::SetGraphicsResource() により
    //! GraphicsResource を設定しておく必要があります。
    //!
    //! GLの以下の設定はユーザに任せています(lytでは設定しません)。
    //!
    //! ・カリング\n
    //! ・シザー処理\n
    //! ・ステンシルテスト\n
    //! ・マスク処理\n
    //! ・ポリゴンオフセット\n
    //! ・デプステスト\n
    //! ・アーリーデプステスト\n
    //! ・フレームバッファオブジェクト
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    //! @sa GraphicsResource
    //!
    virtual void Draw(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    //! @brief キャプチャテクスチャの描画処理を行います。
    //!
    //! @details
    //! キャプチャテクスチャへ内容物を描画してテクスチャを更新します。
    //! drawInfo には、あらかじめ DrawInfo::SetGraphicsResource() により
    //! GraphicsResource を設定しておく必要があります。
    //! また、このメソッド内では以下の設定が変更されるため、呼び出し完了後にアプリケーション側で必要な設定に戻す必要があります。
    //!
    //! ・レンダーターゲット
    //! ・ビューポートシザーステート
    //! ・ブレンドステート
    //!
    //! GLの以下の設定はユーザに任せています(lytでは設定しません)。
    //!
    //! ・カリング\n
    //! ・シザー処理\n
    //! ・ステンシルテスト\n
    //! ・マスク処理\n
    //! ・ポリゴンオフセット\n
    //! ・デプステスト\n
    //! ・アーリーデプステスト\n
    //! ・フレームバッファオブジェクト
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    void DrawCaptureTexture(nn::gfx::Device* pDevice, DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    //! @brief ドロップシャドウキャッシュの描画処理を行います。
    //!
    //! @details
    //! ドロップシャドウキャッシュへ内容物を描画してテクスチャを更新します。
    //! 一度キャッシュを作成すればその後はキャッシュテクスチャの内容を元にドロップシャドウを描画するため描画の GPU 負荷を軽減することができます。
    //! ドロップシャドウキャッシュおよびドロップシャドウ描画時のペイン本体は「初回フレームキャプチャ」設定されたキャプチャテクスチャから作成されるため
    //! マテリアルアニメーションなど、キャプチャ内容に影響するようなアニメーションは反映されません。
    //!
    //! ドロップシャドウの描画にはキャプチャテクスチャが使用されるため、このメソッドの呼び出し前に DrawCaptureTexture を呼び出しキャプチャテクスチャの内容を更新する必要があります。
    //!
    //! drawInfo には、あらかじめ DrawInfo::SetGraphicsResource() により
    //! GraphicsResource を設定しておく必要があります。
    //! また、このメソッド内では以下の設定が変更されるため、呼び出し完了後にアプリケーション側で必要な設定に戻す必要があります。
    //!
    //! ・レンダーターゲット
    //! ・ビューポートシザーステート
    //! ・ブレンドステート
    //! ・シザー処理
    //!
    //! GLの以下の設定はユーザに任せています(lytでは設定しません)。
    //!
    //! ・カリング\n
    //! ・ステンシルテスト\n
    //! ・マスク処理\n
    //! ・ポリゴンオフセット\n
    //! ・デプステスト\n
    //! ・アーリーデプステスト\n
    //! ・フレームバッファオブジェクト
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    //! @deprecated
    //! このメソッドは廃止予定です。 代わりに DrawPaneEffectStaticCache を使用してください。
    //!
    NN_DEPRECATED void DrawDropShadowStaticCache(nn::gfx::Device* pDevice, DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    //! @brief キャプチャ効果で使用する静的レンダリングキャッシュの描画処理を行います。
    //!
    //! @details
    //! キャプチャ効果で使用する静的レンダリングキャッシュへ内容物を描画してテクスチャを更新します。
    //! 一度キャッシュを作成すればその後はキャッシュテクスチャの内容を元にキャプチャ効果を描画するため描画の GPU 負荷を軽減することができます。
    //! マスク、ドロップシャドウキャッシュおよびドロップシャドウ描画時のペイン本体は「初回フレームキャプチャ」設定されたキャプチャテクスチャから作成されるため
    //! マテリアルアニメーションなど、キャプチャ内容に影響するようなアニメーションは反映されません。
    //!
    //! キャプチャ効果の描画にはキャプチャテクスチャが使用されるため、このメソッドの呼び出し前に DrawCaptureTexture を呼び出しキャプチャテクスチャの内容を更新する必要があります。
    //!
    //! drawInfo には、あらかじめ DrawInfo::SetGraphicsResource() により
    //! GraphicsResource を設定しておく必要があります。
    //! また、このメソッド内では以下の設定が変更されるため、呼び出し完了後にアプリケーション側で必要な設定に戻す必要があります。
    //!
    //! ・レンダーターゲット
    //! ・ビューポートシザーステート
    //! ・ブレンドステート
    //! ・シザー処理
    //!
    //! GLの以下の設定はユーザに任せています(lytでは設定しません)。
    //!
    //! ・カリング\n
    //! ・ステンシルテスト\n
    //! ・マスク処理\n
    //! ・ポリゴンオフセット\n
    //! ・デプステスト\n
    //! ・アーリーデプステスト\n
    //! ・フレームバッファオブジェクト
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    void DrawPaneEffectStaticCache(nn::gfx::Device* pDevice, DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief 部品となっているレイアウトか否かを取得します。
    //!
    //! @details
    //! ルートペインが親を持っているか否かという方法で判定していますので、
    //! ライブラリユーザ側で部品以外の理由でルートペインの親を使用している
    //! 場合は、正しく判定できないことがありますのでご注意ください。
    //!
    //! @return
    //! 部品となっている場合はtrue、そうでない場合はfalseを返します。
    //!
    bool IsPartsLayout() const
    {
        return (m_pRootPane->GetParent() != NULL);
    }

    //! @brief ルートペインを取得します。
    //!
    //! @return
    //! ルートペインへのポインタを返します。
    //! ルートペインが存在しない場合は NULL を返します。
    //!
    Pane* GetRootPane() const
    {
        return m_pRootPane;
    }

    //! @brief グループコンテナを取得します。
    //!
    //! @return
    //! グループコンテナへのポインタを返します。
    //! グループコンテナが存在しない場合は NULL を返します。
    //!
    GroupContainer* GetGroupContainer() const
    {
        return m_pGroupContainer;
    }

    //! @brief 表示領域のサイズを取得します。
    //!
    //! @return サイズを返します。
    //!
    const Size& GetLayoutSize() const
    {
        return m_LayoutSize;
    }

    //! @brief リソースアクセサーを取得します。
    //!
    //! @return リソースアクセサーを返します。
    //!
    const ResourceAccessor* GetResourceAccessor() const
    {
        return m_pResourceAccessor;
    }

    //! @brief リソースアクセサーを取得します。
    //!
    //! @return リソースアクセサーを返します。
    //!
    ResourceAccessor* GetResourceAccessor()
    {
        return m_pResourceAccessor;
    }

    //! @brief レイアウトの表示領域を取得します。
    //!
    //! @return 表示領域の矩形を返します。
    //!
    const nn::font::Rectangle GetLayoutRect() const;

    //! @brief レイアウト名を取得します。
    //!
    //! @details
    //! レイアウト名は、レイアウトのファイル名から拡張子を除いたものになります。
    //! 長さは63文字までです。
    //!
    //! @return レイアウト名を返します。
    //!
    const char* GetName() const
    {
        return m_Name;
    }

    //! @brief レイアウト内の全てのテキストボックスペインにタグプロセッサを設定します。
    //!
    //! @param[in] pTagProcessor    タグプロセッサへのポインタです。
    //!
    //! @sa TextBox::GetTagProcessor
    //! @sa TextBox::SetTagProcessor
    //!
    virtual void SetTagProcessor(nn::font::TagProcessorBase<uint16_t>* pTagProcessor);

    //! @brief レイアウトに関連付けているアニメーションのリストを取得します。
    //!
    //! @return AnimTransform オブジェクトのリストを返します。
    //!
    const AnimTransformList& GetAnimTransformList() const
    {
        return m_AnimTransList;
    }

    //! @brief レイアウトに関連付けているアニメーションのリストを取得します。
    //!
    //! @return AnimTransform オブジェクトのリストを返します。
    //!
    AnimTransformList& GetAnimTransformList()
    {
        return m_AnimTransList;
    }

    //! @brief レイアウトに含まれる部品ペインのリストを取得します。
    //!
    //! @return PartsPaneList 部品ペインのリストを返します。
    //!
    const PartsPaneList& GetPartsPaneList() const
    {
        return m_PartsPaneList;
    }

    //! @brief レイアウトに含まれる部品ペインのリストを取得します。
    //!
    //! @details
    //! 構築したレイアウトを他のレイアウトにくっつけるときに、このメソッドで取得した
    //! リストに追加すると、Animate等のアニメーションを更新するメソッドで更新されるようになり、
    //! またFindPartsPaneByNameで検索対象になります。逆に言うとこれらのメソッドで追加
    //! の処理がかかる事になりますので、機能が必要なければリストに追加する必要はありません。
    //!
    //! また、構築したレイアウトを他のレイアウトにくっつけるときは、BuildOptionの
    //! isRootPanePartsについての説明も参照してください。
    //!
    //! @sa BuildOption::isRootPaneParts
    //!
    //! @return PartsPaneList 部品ペインのリストを返します。
    //!
    PartsPaneList& GetPartsPaneList()
    {
        return m_PartsPaneList;
    }

    //! @brief レイアウトに含まれる拡張ユーザー情報のリストを取得します。
    //!
    //! @return 拡張ユーザー情報のリストを返します。
    //!
    const ResExtUserDataList* GetExtUserDataList() const
    {
        return m_pResExtUserDataList;
    }

    //! @brief レイアウトの生成元のリソースを取得します。
    //!
    //! @return レイアウトリソースを返します。
    //!
    const void* GetLayoutResource() const
    {
        return this->GetLayoutResourceData(this->GetName());
    }

    //! @brief アーカイブシェーダを取得します。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] pName シェーダ名
    //!
    //! @return アーカイブシェーダ
    //!
    const ShaderInfo* AcquireArchiveShader(nn::gfx::Device* pDevice, const char* pName) const;

    //! @brief アーカイブシェーダを取得します。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] signature シグネチャ
    //! @param[in] keyLength 検索キーの長さ
    //! @param[in] pKey 検索キー
    //!
    //! @return アーカイブシェーダ
    //!
    const ShaderInfo* AcquireArchiveShader(nn::gfx::Device* pDevice, uint32_t signature, size_t keyLength, const uint32_t* pKey) const;

    //! @brief  レイアウトインスタンスが保持しているキャプチャテクスチャの数を取得します。
    //!
    //! @return 保持しているキャプチャテクスチャの数
    //!
    int GetCaptureTextureCount() const
    {
        if (m_pDynamicRenderingTextureList != NULL)
        {
            return m_pDynamicRenderingTextureList->captureTextureCount;
        }

        return 0;
    }

    //! @brief  レイアウトインスタンスが保持しているベクターグラフィックステクスチャの数を取得します。
    //!
    //! @return 保持しているベクターグラフィックステクスチャの数
    //!
    int GetVectorGraphicsTextureCount() const
    {
        if (m_pDynamicRenderingTextureList != NULL)
        {
            return m_pDynamicRenderingTextureList->vectorGraphicsTextureCount;
        }

        return 0;
    }

    //! @brief  レイアウトインスタンスが保持している動的レンダリングテクスチャの数を取得します。
    //!
    //! @return 保持している動的レンダリングテクスチャの数
    //!
    int GetDynamicRenderingTextureCount() const
    {
        return GetCaptureTextureCount() + GetVectorGraphicsTextureCount();
    }

    //! @brief 指定した名前の部品ペインをリストから検索し、取得します。
    //!
    //! @details
    //! レイアウトが部品ペインのみを管理しているリストを使用しますので、
    //! ルートペインからFindPaneByNameするよりも高速に検索できます。
    //!
    //! @param[in] pFindName 検索するペイン名です。
    //!
    //! @return
    //! pFindName のペイン名を持つ部品ペインが見つかった場合は部品ペインへのポインタを、
    //! 見つからなかった場合は NULL を返します。
    //!
    Parts* FindPartsPaneByName(const char* pFindName);
    //! @brief 指定した名前の部品ペインをリストから検索し、取得します。
    //!
    //! @details
    //! レイアウトが部品ペインのみを管理しているリストを使用しますので、
    //! ルートペインからFindPaneByNameするよりも高速に検索できます。
    //!
    //! @param[in] pFindName 検索するペイン名です。
    //!
    //! @return
    //! pFindName のペイン名を持つ部品ペインが見つかった場合は部品ペインへのポインタを、
    //! 見つからなかった場合は NULL を返します。
    //!
    const Parts* FindPartsPaneByName(const char* pFindName) const;

    //! @brief 指定したペインのポインタから作成されているキャプチャテクスチャのポインタを取得します。
    //!
    //! @details
    //! キャプチャペインか FramebufferCaptureTexture を使用しているペインのポインタを指定します。
    //!
    //! @param[in] pTargetPane 検索に使用するペインのポインタ。
    //!
    //! @return
    //! pFindName から作成された CaptureTexture が見つかった場合はそのポインタを、
    //! 見つからなかった場合は NULL を返します。
    //!
    const CaptureTexture* FindCaptureTextureByPanePtr(const Pane* pTargetPane) const
    {
        // const_cast で this ポインタから const を外して、非 const 版の FindCaptureTextureByPanePtr() を呼び出します。
        // FindCaptureTextureByPanePtr() は 40 行ほどあり、const 版専用に同じコードを重複して持つのは保守性が悪くなるため非 const 版の実装を共有するようにしています。
        // FindPartsPaneByName() が同様の実装となっています。
        return const_cast<Layout*>(this)->FindCaptureTextureByPanePtr(pTargetPane);
    }

    //! @brief 指定したペインのポインタから作成されているキャプチャテクスチャのポインタを取得します。
    //!
    //! @details
    //! キャプチャペインか FramebufferCaptureTexture を使用しているペインのポインタを指定します。
    //!
    //! @param[in] pTargetPane 検索に使用するペインのポインタ。
    //!
    //! @return
    //! pFindName から作成された CaptureTexture が見つかった場合はそのポインタを、
    //! 見つからなかった場合は NULL を返します。
    //!
    CaptureTexture* FindCaptureTextureByPanePtr(const Pane* pTargetPane);

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
    void ResetFirstFrameCaptureUpdatedFlag();

    //! @brief RenderTargetTexture 作成/破棄コールバックを設定します。
    //!
    //! @param[in] pCreateCallback RenderTargetTexture 作成コールバック関数へのポインタを指定します。
    //! @param[in] pDestroyCallback RenderTargetTexture 破棄コールバック関数へのポインタを指定します。
    //! @param[in] pUserData コールバック関数へ渡すユーザーデータのポインタです。
    //!
    static void SetRenderTargetTextureResourceManagementCallback(CreateRenderTargetTextureResourceCallback pCreateCallback, DestroyRenderTargetTextureResourceCallback pDestroyCallback, void* pUserData)
    {
        g_pCreateRenderTargetTextureResourceCallback = pCreateCallback;
        g_pDestroyRenderTargetTextureResourceCallback = pDestroyCallback;
        g_pRenderTargetTextureCallbackUserData = pUserData;
    }

    //! @brief  RenderTargetTexture を作成するためのコールバック関数を取得します。
    //!
    //! @return コールバック関数を返します。
    //!
    static CreateRenderTargetTextureResourceCallback GetCreateRenderTargetTextureResourceCallback()
    {
        return g_pCreateRenderTargetTextureResourceCallback;
    }

    //! @brief  RenderTargetTexture を破棄するためのコールバック関数を取得します。
    //!
    //! @return コールバック関数を返します。
    //!
    static DestroyRenderTargetTextureResourceCallback GetDestroyRenderTargetTextureResourceCallback()
    {
        return g_pDestroyRenderTargetTextureResourceCallback;
    }

    //! @brief  RenderTargetTexture 関連コールバック関数のユーザーデータを取得します。
    //!
    //! @return ユーザーデータのポインタを返します。
    //!
    static void* GetRenderTargetTextureCallbackUserData()
    {
        return g_pRenderTargetTextureCallbackUserData;
    }

    //! @brief  ステートマシンを取得します。（内部用機能のため使用しないでください）
    //! @return ステートマシンのポインタを返します。
    StateMachine* GetStateMachine()
    {
        Pane* pRootPane = GetRootPane();
        return pRootPane->GetStateMachine();
    }

    //! @brief  ステートマシンを取得します。（内部用機能のため使用しないでください）
    //! @return ステートマシンのポインタを返します。
    const StateMachine* GetStateMachine() const
    {
        Pane* pRootPane = GetRootPane();
        return pRootPane->GetStateMachine();
    }

    //! @brief レイアウト内の指定した名前のペインのドロップシャドウ静的レンダリングキャッシュテクスチャを破棄して、再生成可能な状態にします。
    //!
    //! @details
    //! レイアウトインスタンス内のキャプチャテクスチャを作成しているペインのリストから指定した名前のペインを検索し、
    //! ペインがドロップシャドウ効果の静的レンダリングキャッシュを使用している場合
    //! キャッシュ済みフラグをリセットして再度キャッシュ用テクスチャがレンダリングされる状態に設定します。
    //!
    //! @param[in] pPaneName 対象ペイン名。
    //!
    //! @deprecated
    //! このメソッドは廃止予定です。 代わりに DiscardPaneEffectStaticCachedTexture を使用してください。
    //!
    NN_DEPRECATED void DiscardDropShadowStaticCachedTexture(const char* pPaneName);

    //! @brief レイアウト内の指定した名前のペインのキャプチャ効果の静的レンダリングキャッシュテクスチャを破棄して、再生成可能な状態にします。
    //!
    //! @details
    //! レイアウトインスタンス内のキャプチャテクスチャを作成しているペインのリストから指定した名前のペインを検索し、
    //! ペインがキャプチャ効果の静的レンダリングキャッシュを使用している場合
    //! キャッシュ済みフラグをリセットして再度キャッシュ用テクスチャがレンダリングされる状態に設定します。
    //!
    //! @param[in] pPaneName 対象ペイン名。
    //!
    void DiscardPaneEffectStaticCachedTexture(const char* pPaneName);

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
    bool CompareCopiedInstanceTest(const Layout& target) const;

    //----------------------------------------
    //! @name オブジェクト生成／破棄
    //@{

    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T>
    static T* AllocateAndConstruct()
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T)))
        {
            return new (pMem) T();
        }
        return 0;
    }

    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //!
    //! @param[in] alignment 確保するメモリのアライメント指定。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T>
    static T* AllocateAndConstruct(size_t alignment)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T), alignment))
        {
            return new (pMem) T();
        }
        return 0;
    }


    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //! @tparam Param1 コンストラクタの第1引数の型です。
    //!
    //! @param[in] param1   コンストラクタの第1引数です。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T, typename Param1>
    static T* AllocateAndConstruct(Param1 param1)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T)))
        {
            return new (pMem) T(param1);
        }
        return 0;
    }

    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //! @tparam Param1 コンストラクタの第1引数の型です。
    //!
    //! @param[in] param1   コンストラクタの第1引数です。
    //! @param[in] alignment 確保するメモリのアライメント指定。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T, typename Param1>
    static T* AllocateAndConstruct(Param1 param1, size_t alignment)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T), alignment))
        {
            return new (pMem) T(param1);
        }
        return 0;
    }


    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //! @tparam Param1 コンストラクタの第1引数の型です。
    //! @tparam Param2 コンストラクタの第2引数の型です。
    //!
    //! @param[in] param1   コンストラクタの第1引数です。
    //! @param[in] param2   コンストラクタの第2引数です。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T, typename Param1, typename Param2>
    static T* AllocateAndConstruct(Param1 param1, Param2 param2)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T)))
        {
            return new (pMem) T(param1, param2);
        }
        return 0;
    }

    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //! @tparam Param1 コンストラクタの第1引数の型です。
    //! @tparam Param2 コンストラクタの第2引数の型です。
    //!
    //! @param[in] param1   コンストラクタの第1引数です。
    //! @param[in] param2   コンストラクタの第2引数です。
    //! @param[in] alignment 確保するメモリのアライメント指定。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T, typename Param1, typename Param2>
    static T* AllocateAndConstruct(Param1 param1, Param2 param2, size_t alignment)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T), alignment))
        {
            return new (pMem) T(param1, param2);
        }
        return 0;
    }


    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //! @tparam Param1 コンストラクタの第1引数の型です。
    //! @tparam Param2 コンストラクタの第2引数の型です。
    //! @tparam Param3 コンストラクタの第3引数の型です。
    //!
    //! @param[in] param1   コンストラクタの第1引数です。
    //! @param[in] param2   コンストラクタの第2引数です。
    //! @param[in] param3   コンストラクタの第3引数です。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T, typename Param1, typename Param2, typename Param3>
    static T* AllocateAndConstruct(Param1 param1, Param2 param2, Param3 param3)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T)))
        {
            return new (pMem) T(param1, param2, param3);
        }
        return 0;
    }

    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //! @tparam Param1 コンストラクタの第1引数の型です。
    //! @tparam Param2 コンストラクタの第2引数の型です。
    //! @tparam Param3 コンストラクタの第3引数の型です。
    //!
    //! @param[in] param1   コンストラクタの第1引数です。
    //! @param[in] param2   コンストラクタの第2引数です。
    //! @param[in] param3   コンストラクタの第3引数です。
    //! @param[in] alignment 確保するメモリのアライメント指定。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T, typename Param1, typename Param2, typename Param3>
    static T* AllocateAndConstruct(Param1 param1, Param2 param2, Param3 param3, size_t alignment)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T), alignment))
        {
            return new (pMem) T(param1, param2, param3);
        }
        return 0;
    }


    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //! @tparam Param1 コンストラクタの第1引数の型です。
    //! @tparam Param2 コンストラクタの第2引数の型です。
    //! @tparam Param3 コンストラクタの第3引数の型です。
    //! @tparam Param4 コンストラクタの第4引数の型です。
    //!
    //! @param[in] param1   コンストラクタの第1引数です。
    //! @param[in] param2   コンストラクタの第2引数です。
    //! @param[in] param3   コンストラクタの第3引数です。
    //! @param[in] param4   コンストラクタの第4引数です。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T, typename Param1, typename Param2, typename Param3, typename Param4>
    static T* AllocateAndConstruct(Param1 param1, Param2 param2, Param3 param3, Param4 param4)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T)))
        {
            return new (pMem) T(param1, param2, param3, param4);
        }
        return 0;
    }

    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //! @tparam Param1 コンストラクタの第1引数の型です。
    //! @tparam Param2 コンストラクタの第2引数の型です。
    //! @tparam Param3 コンストラクタの第3引数の型です。
    //! @tparam Param4 コンストラクタの第4引数の型です。
    //!
    //! @param[in] param1   コンストラクタの第1引数です。
    //! @param[in] param2   コンストラクタの第2引数です。
    //! @param[in] param3   コンストラクタの第3引数です。
    //! @param[in] param4   コンストラクタの第4引数です。
    //! @param[in] alignment 確保するメモリのアライメント指定。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T, typename Param1, typename Param2, typename Param3, typename Param4>
    static T* AllocateAndConstruct(Param1 param1, Param2 param2, Param3 param3, Param4 param4, size_t alignment)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T), alignment))
        {
            return new (pMem) T(param1, param2, param3, param4);
        }
        return 0;
    }


    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //! @tparam Param1 コンストラクタの第1引数の型です。
    //! @tparam Param2 コンストラクタの第2引数の型です。
    //! @tparam Param3 コンストラクタの第3引数の型です。
    //! @tparam Param4 コンストラクタの第4引数の型です。
    //! @tparam Param5 コンストラクタの第5引数の型です。
    //!
    //! @param[in] param1   コンストラクタの第1引数です。
    //! @param[in] param2   コンストラクタの第2引数です。
    //! @param[in] param3   コンストラクタの第3引数です。
    //! @param[in] param4   コンストラクタの第4引数です。
    //! @param[in] param5   コンストラクタの第5引数です。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
    static T* AllocateAndConstruct(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T)))
        {
            return new (pMem) T(param1, param2, param3, param4, param5);
        }
        return 0;
    }

    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //! @tparam Param1 コンストラクタの第1引数の型です。
    //! @tparam Param2 コンストラクタの第2引数の型です。
    //! @tparam Param3 コンストラクタの第3引数の型です。
    //! @tparam Param4 コンストラクタの第4引数の型です。
    //! @tparam Param5 コンストラクタの第5引数の型です。
    //!
    //! @param[in] param1   コンストラクタの第1引数です。
    //! @param[in] param2   コンストラクタの第2引数です。
    //! @param[in] param3   コンストラクタの第3引数です。
    //! @param[in] param4   コンストラクタの第4引数です。
    //! @param[in] param5   コンストラクタの第5引数です。
    //! @param[in] alignment 確保するメモリのアライメント指定。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
    static T* AllocateAndConstruct(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5, size_t alignment)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T), alignment))
        {
            return new (pMem) T(param1, param2, param3, param4, param5);
        }
        return 0;
    }

    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //! @tparam Param1 コンストラクタの第1引数の型です。
    //! @tparam Param2 コンストラクタの第2引数の型です。
    //! @tparam Param3 コンストラクタの第3引数の型です。
    //! @tparam Param4 コンストラクタの第4引数の型です。
    //! @tparam Param5 コンストラクタの第5引数の型です。
    //! @tparam Param6 コンストラクタの第6引数の型です。
    //!
    //! @param[in] param1   コンストラクタの第1引数です。
    //! @param[in] param2   コンストラクタの第2引数です。
    //! @param[in] param3   コンストラクタの第3引数です。
    //! @param[in] param4   コンストラクタの第4引数です。
    //! @param[in] param5   コンストラクタの第5引数です。
    //! @param[in] param6   コンストラクタの第6引数です。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
    static T* AllocateAndConstruct(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5, Param6 param6)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T)))
        {
            return new (pMem) T(param1, param2, param3, param4, param5, param6);
        }
        return 0;
    }

    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //! @tparam Param1 コンストラクタの第1引数の型です。
    //! @tparam Param2 コンストラクタの第2引数の型です。
    //! @tparam Param3 コンストラクタの第3引数の型です。
    //! @tparam Param4 コンストラクタの第4引数の型です。
    //! @tparam Param5 コンストラクタの第5引数の型です。
    //! @tparam Param6 コンストラクタの第6引数の型です。
    //!
    //! @param[in] param1   コンストラクタの第1引数です。
    //! @param[in] param2   コンストラクタの第2引数です。
    //! @param[in] param3   コンストラクタの第3引数です。
    //! @param[in] param4   コンストラクタの第4引数です。
    //! @param[in] param5   コンストラクタの第5引数です。
    //! @param[in] param6   コンストラクタの第6引数です。
    //! @param[in] alignment 確保するメモリのアライメント指定。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
    static T* AllocateAndConstruct(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5, Param6 param6, size_t alignment)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T), alignment))
        {
            return new (pMem) T(param1, param2, param3, param4, param5, param6);
        }
        return 0;
    }

    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //! @tparam Param1 コンストラクタの第1引数の型です。
    //! @tparam Param2 コンストラクタの第2引数の型です。
    //! @tparam Param3 コンストラクタの第3引数の型です。
    //! @tparam Param4 コンストラクタの第4引数の型です。
    //! @tparam Param5 コンストラクタの第5引数の型です。
    //! @tparam Param6 コンストラクタの第6引数の型です。
    //! @tparam Param7 コンストラクタの第7引数の型です。
    //!
    //! @param[in] param1   コンストラクタの第1引数です。
    //! @param[in] param2   コンストラクタの第2引数です。
    //! @param[in] param3   コンストラクタの第3引数です。
    //! @param[in] param4   コンストラクタの第4引数です。
    //! @param[in] param5   コンストラクタの第5引数です。
    //! @param[in] param6   コンストラクタの第6引数です。
    //! @param[in] param7   コンストラクタの第7引数です。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7>
    static T* AllocateAndConstruct(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5, Param6 param6, Param7 param7)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T)))
        {
            return new (pMem) T(param1, param2, param3, param4, param5, param6, param7);
        }
        return 0;
    }

    //! @brief オブジェクトを生成します。
    //!
    //! @tparam T 生成するオブジェクトのクラスです。
    //! @tparam Param1 コンストラクタの第1引数の型です。
    //! @tparam Param2 コンストラクタの第2引数の型です。
    //! @tparam Param3 コンストラクタの第3引数の型です。
    //! @tparam Param4 コンストラクタの第4引数の型です。
    //! @tparam Param5 コンストラクタの第5引数の型です。
    //! @tparam Param6 コンストラクタの第6引数の型です。
    //! @tparam Param7 コンストラクタの第7引数の型です。
    //!
    //! @param[in] param1   コンストラクタの第1引数です。
    //! @param[in] param2   コンストラクタの第2引数です。
    //! @param[in] param3   コンストラクタの第3引数です。
    //! @param[in] param4   コンストラクタの第4引数です。
    //! @param[in] param5   コンストラクタの第5引数です。
    //! @param[in] param6   コンストラクタの第6引数です。
    //! @param[in] param7   コンストラクタの第7引数です。
    //! @param[in] alignment 確保するメモリのアライメント指定。
    //!
    //! @return 生成されたオブジェクトへのポインタを返します。
    //!
    //! @sa DeleteObj
    //!
    template<typename T, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7>
    static T* AllocateAndConstruct(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5, Param6 param6, Param7 param7, size_t alignment)
    {
        if (void* pMem = Layout::AllocateMemory(sizeof(T), alignment))
        {
            return new (pMem) T(param1, param2, param3, param4, param5, param6, param7);
        }
        return 0;
    }

    // 配列を生成します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] count  要素数です。
    //!
    //! @return 生成した配列です。
    //!
    template<typename T>
    static T* NewArray(int  count)
    {
        void* pMem = Layout::AllocateMemory(sizeof(T) * count);
        if (! pMem)
        {
            return 0;
        }

        T *const objAry = static_cast<T*>(pMem);

        for (int  i = 0; i < count; ++i)
        {
            new (&objAry[i]) T();
        }

        return objAry;
    }

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] count  要素数です。
    //! @param[in] alignment    確保するメモリブロックのアライメント値です。
    //!
    //! @return 生成した配列です。
    //!
    template<typename T>
    static T* NewArray(int  count, size_t  alignment)
    {
        void* pMem = Layout::AllocateMemory(sizeof(T) * count, alignment);
        if (! pMem)
        {
            return 0;
        }

        T *const objAry = static_cast<T*>(pMem);

        for (int  i = 0; i < count; ++i)
        {
            new (&objAry[i]) T();
        }

        return objAry;
    }

    //! @brief オブジェクトを廃棄します。
    //!
    //! @tparam T オブジェクトのクラスです。
    //!
    //! @param[in] pObj オブジェクトへのポインタです。
    //!
    template<typename T>
    static void DeleteObj(T* pObj)
    {
        if (pObj)
        {
            pObj->~T();
            FreeMemory(pObj);
        }
    }

    // 配列を廃棄します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] objAry   廃棄する配列です。
    //! @param[in] count      要素数です。
    //!
    template<typename T>
    static void DeleteArray(T objAry[], int  count)
    {
        if (objAry)
        {
            for (int  i = 0; i < count; ++i)
            {
                objAry[i].~T();
            }
            FreeMemory(objAry);
        }
    }

    // 配列を廃棄します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] objAry   廃棄する配列です。
    //!
    template<typename T>
    static void DeletePrimArray(T objAry[])
    {
        if (objAry)
        {
            FreeMemory(objAry);
        }
    }

    //@}

    /**
    *  部品を構築するためのデータにアクセスするためのクラスです。
    */
    class PartsBuildDataAccessor
    {
    public:
        //! @brief コンストラクタです。
        explicit PartsBuildDataAccessor(const ResParts* pResParts);
        //! @brief 上書きプロパティを名前で検索します。
        const ResPartsProperty* FindPartsPropertyFromName(const char* pName) const;
        //! @brief 上書きプロパティリソースを取得します。
        const void* GetPropertyResBlock(const ResPartsProperty* pResPartsProperty) const;
        //! @brief 拡張ユーザー情報ブロックを取得します。
        const ResExtUserDataList* GetExtUserDataListResBlock(bool* pIsOverride, const ResPartsProperty* pResPartsProperty) const;
        //! @brief ペイン基本情報リソースを取得します。
        const ResPartsPaneBasicInfo* GetPartsPaneBasicInfoResBlock(const ResPartsProperty* pResPartsProperty) const;
        //! @brief このプロパティでペインの上書きが行われているかどうかを取得します。
        bool IsOverwriting() const;

    private:
        int32_t  m_PropertyCount;
        const ResPartsProperty* m_pPropertyTable;
        const ResParts* m_pResParts;
    };

    // DynamicTextureShareInfo に収集された動的レンダリングテクスチャをレイアウトインスタンスへ集約します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pShareInfo レイアウトごとの動的テクスチャ共有情報です。
    //!
    void AggregateDynamicTextureList(detail::DynamicTextureShareInfo* pShareInfo);
protected:

    /**
     *  部品を構築するためのデータを集めたクラスです。
     */
    class PartsBuildDataSet
    {
    public:
        PartsBuildDataSet(Parts* pPartsPane, const ResParts* pResParts, const BuildResSet* pBuildResSet, const ResVec2* pOriginalSize);

        Parts* GetPartsPane() const { return m_pPartsPane; }
        const nn::util::Float2& GetMagnify() const { return m_Magnify; }
        const BuildResSet* GetPropertyBuildResSet() const { return m_pPropertyBuildResSet; }

        const ResPartsProperty* FindPartsPropertyFromName(const char* pName) const
        {
            return m_PartsBuildDataAccessor.FindPartsPropertyFromName(pName);
        }

        const void* GetPropertyResBlock(const ResPartsProperty* pResPartsProperty) const
        {
            return m_PartsBuildDataAccessor.GetPropertyResBlock(pResPartsProperty);
        }

        const ResExtUserDataList* GetExtUserDataListResBlock(bool* pIsOverride, const ResPartsProperty* pResPartsProperty) const
        {
            return m_PartsBuildDataAccessor.GetExtUserDataListResBlock(pIsOverride, pResPartsProperty);
        }

        const ResPartsPaneBasicInfo* GetPartsPaneBasicInfoResBlock(const ResPartsProperty* pResPartsProperty) const
        {
            return m_PartsBuildDataAccessor.GetPartsPaneBasicInfoResBlock(pResPartsProperty);
        }

        bool IsOverwriting() const
        {
            return m_PartsBuildDataAccessor.IsOverwriting();
        }

        // PartsBuildDataSet が使用している PartsBuildDataAccessor を取得します。

        //! @brief 内部用機能のため使用禁止です。
        const PartsBuildDataAccessor* GetPartsBuildDataAccessor() const
        {
            return &m_PartsBuildDataAccessor;
        }
    private:
        PartsBuildDataAccessor m_PartsBuildDataAccessor;
        Parts* m_pPartsPane;
        const BuildResSet* m_pPropertyBuildResSet;
        nn::util::Float2 m_Magnify;
    };

    /**
     * 動的にレンダリングされるテクスチャを管理する構造体です。
     */
    struct DynamicRenderingTextureList
    {
        // キャプチャテクスチャ数
        int                 captureTextureCount;
        // ベクターグラフィックステクスチャ数
        int                 vectorGraphicsTextureCount;
        // 動的レンダリングテクスチャへのポインタ
        detail::DynamicRenderingTexture**    pTextures;
    };

    //! @name 初期化
    // @{

    virtual bool BuildImpl(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        const void* pLayoutResource,
        ResourceAccessor* pResAcsr,
        const BuildArgSet& parentBuildArgSet,
        const PartsBuildDataSet* pPartsBuildDataSet);

    void PrepareBuildArgSet(BuildArgSet& buildArgSet, const BuildArgSet& parentBuildArgSet, const PartsBuildDataSet* pPartsBuildDataSet);

    void SetByResLayout(LayoutBuildContext& buildContext);
    void BuildControl(LayoutBuildContext& buildContext, nn::gfx::Device* pDevice);
    void BuildPaneByResPane(LayoutBuildContext& buildContext, BuildResultInformation* pOutBuildResultInformation, nn::gfx::Device* pDevice, const PartsBuildDataSet* pPartsBuildDataSet);
    void BuildStateMachine(LayoutBuildContext& buildContext, nn::gfx::Device* pDevice, const PartsBuildDataSet* pPartsBuildDataSet);

    void RegisteredStateMachine(StateMachine* pStateMachine)
    {
        Pane* pRootPane = GetRootPane();

        SystemDataLayoutStateMachine systemData;
        systemData.type = PaneSystemDataType_StateRuntimeInfo;
        systemData.pStateMachine = pStateMachine;

        pRootPane->AddDynamicSystemExtUserData(PaneSystemDataType_StateRuntimeInfo, &systemData, sizeof(systemData));
    }

    void BuildGroup(LayoutBuildContext& buildContext);
    void BuildAlignment(LayoutBuildContext& buildContext);

    virtual Pane* BuildPartsImpl(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        const void* pData,
        const PartsBuildDataSet* pPartsBuildDataSet,
        BuildArgSet& buildArgSet,
        BuildResSet& buildResSet,
        const uint32_t kind
    );

    virtual Layout* DoCreatePartsLayout_(const char* pName, const PartsBuildDataSet& partsBuildDataSet, const nn::ui2d::BuildArgSet& buildArgSet);

    void CopyLayoutInstanceImpl(
        nn::gfx::Device* pDevice,
        const Layout& src,
        Layout* pParentLayout,
        const char* pRootPaneName
    );

    void CopyLayoutInstanceImpl(
        nn::gfx::Device* pDevice,
        const Layout& src,
        Layout* pParentLayout,
        const char* pRootPaneName,
        detail::BuildPaneTreeContext* pBuildPaneTreeContext
    );


    //! @brief リソースからペインを生成します。
    //!
    //! @details
    //! kind には以下のいずれかのデータブロック・シグニチャが指定されます。
    //!
    //! ・DATABLOCKKIND_PANE\n
    //! ・DATABLOCKKIND_PICTURE\n
    //! ・DATABLOCKKIND_TEXTBOX\n
    //! ・DATABLOCKKIND_WINDOW\n
    //! ・DATABLOCKKIND_BOUNDING\n
    //! ・DATABLOCKKIND_PARTS
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報
    //! @param[in] pDevice          デバイスです。
    //! @param[in] kind             データブロックのシグニチャです。
    //! @param[in] pBlock           データブロックです。
    //! @param[in] pOverrideBlock   オーバーライドするデータブロックです。
    //! @param[in] buildArgSet      構築時の引数のセットです。
    //!
    //! @return 生成されたペインを返します。
    //!
    virtual Pane* BuildPaneObj(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        uint32_t kind,
        const void* pBlock,
        const void* pOverrideBlock,
        const BuildArgSet& buildArgSet);

    virtual Layout* BuildPartsLayout(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        const char* pName,
        const PartsBuildDataSet& partsBuildDataSet,
        const BuildArgSet& buildArgSet);

    //@}

    //! @name 設定／取得
    // @{

    //! @brief ルートペインを設定します。
    //!
    //! @param[in] pPane    ペインです。
    //!
    void SetRootPane(Pane* pPane)
    {
        m_pRootPane = pPane;
    }

    //! @brief グループコンテナを設定します。
    //!
    //! @param[in] pGroupContainer  グループコンテナです。
    //!
    void SetGroupContainer(GroupContainer* pGroupContainer)
    {
        m_pGroupContainer = pGroupContainer;
    }

    //! @brief 表示領域のサイズを設定します。
    //!
    //! @param[in] size 表示領域のサイズです。
    //!
    void SetLayoutSize(const Size& size)
    {
        m_LayoutSize = size;
    }

    //! @brief 名前を設定します。
    //!
    //! @param[in] pName 名前です。
    //!
    void SetName(const char* pName)
    {
        m_Name = pName;
    }

    //! @brief リソースアクセサーを設定します。
    //!
    //! @param[in] pResourceAccessor リソースアクセサーです。
    //!
    void SetResourceAccessor(ResourceAccessor* pResourceAccessor)
    {
        m_pResourceAccessor = pResourceAccessor;
    }

    //! @brief 指定したレイアウト名のレイアウトリソースデータを取得します。
    //!
    //! @param[in] pLayoutName   レイアウト名
    //!
    const void* GetLayoutResourceData(const char* pLayoutName) const;

    //! @brief 指定したタグ名のGroupArrayAnimatorを作成し、アニメーションタグに関連づけられた全てのグループにバインドします。
    //!
    //! @details
    //! GroupArrayAnimatorを作成し、指定されたタグ名のリソースを関連付けます。
    //! さらに、アニメーションタグに関連づけられた全てのグループにアニメーションをバインドします。
    //! リソースのフォーマットが正しくなければ失敗します。
    //!
    //! このメソッドを使用するには、バイナリコンバータで-gオプションを使用し、
    //! アニメーションをアニメーションタグ個別に出力している必要があります。
    //!
    //! 内部実装の共有のために用意していますので、publicにはしていません。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] animRes  アニメーションリソースです。
    //! @param[in] enabled   開始時にアニメーションを有効状態にするか。
    //!
    //! @return
    //! アニメーションの作成に成功した場合はアニメーションへのポインタを、
    //! 失敗した場合は NULL を返します。
    //!
    GroupArrayAnimator* CreateGroupArrayAnimator(nn::gfx::Device* pDevice, const AnimResource& animRes, bool enabled);

    //! @brief レイアウトに含まれる拡張ユーザー情報のリストを設定します。
    //!
    //! @param[in] pResExtUserDataList 拡張ユーザー情報のリストです。
    //!
    void SetExtUserDataList(const ResExtUserDataList* pResExtUserDataList)
    {
        m_pResExtUserDataList = pResExtUserDataList;
    }

    // @}


    //! @brief 描画のための計算をします。
    //!
    //! @details
    //! 引数drawInfoで与える DrawInfo は、関数内部で更新されます。複数のスレッドで共有できないので注意してください。
    //! 複数スレッドで、処理を行う場合は、DrawInfo をスレッド毎に複製して実行するようにしてください。
    //!
    //! 引数forceGlbMtxDirtyは、ペインのグローバルマトリックスを強制的に再計算させる場合に
    //! trueを指定します。ペインは、初回計算時もしくは自分自身と親のSRT及びサイズが変更された
    //! 場合のみグローバルマトリックスを再計算する仕組みになっていますが、DrawInfoに設定した
    //! ビュー行列が変更された場合も再計算が必要になります。
    //!
    //! DrawInfoに設定したビュー行列が変更された場合はforceGlbMtxDirtyにtrueを与えるように
    //! してください。
    //!
    //! この引数は、正射影で描画している場合はほぼ指定する必要はありません。透視射影でカメラを
    //! 動かしているような場合に指定する必要があります。
    //!
    //! @param[in] drawInfo         描画情報です。
    //! @param[in] forceGlbMtxDirty 全てのグローバルマトリックスを再計算するか否かです。
    //!
    //! @sa TextBox::ClearDirtyFlag
    //!
    virtual void CalculateImpl(DrawInfo& drawInfo, bool forceGlbMtxDirty);

private:

    // システム用拡張ユーザーデータの数を取得します
    bool HasSystemExtData() const
    {
        if (m_pResExtUserDataList != NULL)
        {
            const ResExtUserData* pFirstData = nn::util::ConstBytePtr(m_pResExtUserDataList, sizeof(*m_pResExtUserDataList)).Get<const ResExtUserData>();
            return pFirstData->GetType() == ExtUserDataType::ExtUserDataType_SystemData;
        }

        return false;
    }

    // システム用拡張ユーザーデータの数を取得します
    uint32_t GetSystemExtDataCount() const
    {
        if (HasSystemExtData())
        {
            const ResExtUserData* pData = nn::util::ConstBytePtr(m_pResExtUserDataList, sizeof(*m_pResExtUserDataList)).Get<const ResExtUserData>();
            return pData->GetSystemDataCount();
        }

        return 0;
    }

    // システム用拡張ユーザーデータに含まれているバイナリデータをインデックス指定で取得します。
    const void* GetSystemExtDataByIndex(int index) const
    {
        if (HasSystemExtData())
        {
            const ResExtUserData* pData = nn::util::ConstBytePtr(m_pResExtUserDataList, sizeof(*m_pResExtUserDataList)).Get<const ResExtUserData>();
            return pData->GetSystemData(index);
        }

        return NULL;
    }

    // システム用拡張ユーザーデータに含まれているバイナリデータをタイプを指定して取得します。
    const void* FindSystemExtDataByType(LayoutSystemDataType type) const
    {
        if (HasSystemExtData())
        {
            const int systemDataCount = GetSystemExtDataCount();
            for (int i = 0; i < systemDataCount; ++i)
            {
                const void* pData = GetSystemExtDataByIndex(i);
                if (*(static_cast<const LayoutSystemDataType*>(pData)) == type)
                {
                    return pData;
                }
            }
        }

        return NULL;
    }

    virtual void BuildVectorGraphicsTextureList(BuildResultInformation* pOutBuildResultInformation, nn::gfx::Device* pDevice, const nn::ui2d::ResVectorGraphicsTextureList* pResVectorGraphicsTextureList, const char* pNamePrefix);
    virtual void CalculateVectorGraphicsTexture(DrawInfo& drawInfo);

    static nn::AlignedAllocateFunctionWithUserData  g_pAllocateFunction;
    static nn::FreeFunctionWithUserData             g_pFreeFunction;
    static void*                                    g_pUserDataForAllocator;

    static CreateRenderTargetTextureResourceCallback       g_pCreateRenderTargetTextureResourceCallback;
    static DestroyRenderTargetTextureResourceCallback      g_pDestroyRenderTargetTextureResourceCallback;
    static void*                                   g_pRenderTargetTextureCallbackUserData;

    static size_t                                   g_CosntantBufferReservedSize;

    // BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、１つのレイアウト内で初期化可能な CaptureTexture の最大数です。
    static int g_CaptureTextureShareInfoCountMax;
    // BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、１つのレイアウト内で初期化可能な VectorGraphicsTexture の最大数です。
    static int g_VectorGraphicsTextureShareInfoCountMax;
    // BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、１つのレイアウト内で初期化可能なレイアウトインスタンス(パーツ＋ボディ)の最大数です。
    static int g_DynamicTextureShareInfoCountMax;
    // BuildPaneTreeContext を使用してペインツリー中の動的レンダリングテクスチャを初期化中に、動的レンダリングテクスチャの上書き設定が可能なパーツ階層の最大数です。
    static int g_DynamicTextureShareInfoPartsStackMax;

    AnimTransformList m_AnimTransList;
    Pane* m_pRootPane;
    GroupContainer* m_pGroupContainer;
    Size m_LayoutSize;

    const char* m_Name;
    const ResExtUserDataList* m_pResExtUserDataList;

    ResourceAccessor* m_pResourceAccessor;
    // 部品ペインのリスト。効率のため、他のペインとは別に管理します。このリストではペインの親子関係は関係なく、
    // 一次元のリストになっています。また、部品ペインがリストに登録される順番は不定です。
    PartsPaneList m_PartsPaneList;

    DynamicRenderingTextureList*    m_pDynamicRenderingTextureList;
private:
    // コピー演算子を禁止します。
    const Layout& operator=(const Layout&);
};

namespace detail
{

//---------------------------------------------------------------------------
//! @brief  キャプチャテクスチャの共有と遅延初期化のための情報です
//---------------------------------------------------------------------------
struct CaptureTextureShareInfo
{
    //! 初期化対象の CaptureTexture のインスタンスです。
    CaptureTexture* pTexture;
    //! 初期化のためのリソースです。リソースを使用して初期化する場合にポインタを設定します。
    const ResCaptureTexture* pResCaptureTexture;
    //! 初期化のためのキャプチャテクスチャのインスタンスです。CaptureTexture のインスタンスを使用して初期化する場合にポインタを設定します。
    const CaptureTexture* pTextureInstance;
    //! キャプチャテクスチャに関連付けるペインの名前です。
    const char* pTargetPaneName;
    //! IntrusiveList のためのリンクデータ
    nn::util::IntrusiveListNode m_Link;
};


//---------------------------------------------------------------------------
//! @brief  VectorGraphicsTexture インスタンスの IntrusiveList 化のための構造体です。
//---------------------------------------------------------------------------
struct VectorGraphicsTextureShareInfo
{
    //! VectorGraphicsTexture のポインタです。
    detail::VectorGraphicsTexture*  pTexture;
    //! IntrusiveList のためのリンクデータ
    nn::util::IntrusiveListNode m_Link;
};

class BuildPaneTreeContextInstancePool;

// DynamicTextureShareInfo で可変長可変長配列として扱うため IntrusiveList を使用する
typedef nn::util::IntrusiveList<CaptureTextureShareInfo, nn::util::IntrusiveListMemberNodeTraits<CaptureTextureShareInfo, &CaptureTextureShareInfo::m_Link, CaptureTextureShareInfo> > CaptureTextueShareInfoList;
// DynamicTextureShareInfo で可変長可変長配列として扱うため IntrusiveList を使用する
typedef nn::util::IntrusiveList<VectorGraphicsTextureShareInfo, nn::util::IntrusiveListMemberNodeTraits<VectorGraphicsTextureShareInfo, &VectorGraphicsTextureShareInfo::m_Link, VectorGraphicsTextureShareInfo> > VectorGraphicsTextueShareInfoList;

//---------------------------------------------------------------------------
//! @brief  動的生成テクスチャの共有のための情報です。
//!
//! @details
//! 動的生成テクスチャはペインの所属するレイアウト内で共有されます。
//! このデータ構造ではレイアウト単位でのテクスチャ共有状況を扱います。
//! また、同時にパーツ間をまたいだキャプチャテクスチャの初期化のための情報も保持しています。
//---------------------------------------------------------------------------
class DynamicTextureShareInfo
{
public:
    // IntrusiveList のためのリンクデータ
    nn::util::IntrusiveListNode m_Link;
public:
    // コンストラクタ
    DynamicTextureShareInfo();
    // デストラクタ
    ~DynamicTextureShareInfo();

    // 初期化処理
    void Initialize(Layout* pLayout, const Layout::PartsBuildDataAccessor* pAccessor, BuildPaneTreeContextInstancePool* pInstancePool);
    // 終了処理
    void Finalize();

    // CaptureTexture の共有情報を登録します(初期化情報未設定版)。
    CaptureTextureShareInfo* AddCaptureTexture(CaptureTexture* pTexture);
    // CaptureTexture の共有情報を登録します(インスタンス指定版)。
    CaptureTextureShareInfo* AddCaptureTexture(CaptureTexture* pTexture, const CaptureTexture* pSourceTexture, const char* targetPaneName);
    // VectorGraphicsTexture の共有情報を登録します。
    VectorGraphicsTextureShareInfo* AddVectorGraphicsTexture(detail::VectorGraphicsTexture* pTexture);

    // 指定した名前で CaptureTexture を作成し、共有テクスチャとして登録します。
    CaptureTexture* CreateCaptureTexture(const char* pCaptureTextureName);
    // 指定した名前で VectorGraphicsTexture を作成します。
    VectorGraphicsTexture* CreateVectorGraphicsTexture(const char* pTextureName);

    // 指定した名前で CaptureTexture を作成し、共有テクスチャとして登録します。
    CaptureTexture* CreateAndSetupCaptureTexture(const char* pCaptureTextureName);

    // 指定したインスタンスのコピーを共有テクスチャとして登録します。すでに存在した場合は作成済みのインスタンスのポインタを返します。
    CaptureTexture* CopyCaptureTexture(bool* pCraeted, const CaptureTexture* pSourceTexture);
    // 指定したインスタンスのコピーを共有テクスチャとして登録します。
    VectorGraphicsTexture* CopyVectorGraphicsTexture(nn::gfx::Device* pDevice, const Layout* pLayout, const VectorGraphicsTexture* pSourceTexture);

    // 指定した名前の登録済み CaptureTexture を検索しリソースでの初期化情報を設定します。
    void SetupCaptureTextureInitializeResource(const char* pCaptureTextureName);
    // 指定した名前で登録済みの VectorGraphicsTExture を初期化します。
    void InitializeVectorGraphicsTexture(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        Layout* pLayout,
        ResourceAccessor* pResAccessor,
        const char* pVectorGraphicsTextureName);

    // 登録されている CaptureTexture のリストから指定した名前で検索します。
    CaptureTexture* FindCaptureTexture(const char* name) const;
    // 登録されている VectorGraphicsTexture のリストから指定した名前で検索します。
    VectorGraphicsTexture* FindVectorGraphicsTexture(const char* name) const;

    // CaptureTexture の初期化のためのリソース取得元を設定します。
    void SetResCaptureTextureList(const ResCaptureTextureList* pResCaptureTextureList);
    // VectorGraphicsTexture の初期化のためのリソース取得元を設定します。
    void SetResVectorGraphicsTextureList(const ResVectorGraphicsTextureList* pResVectorGraphicsTextureList);

    // 関連付けられているレイアウトを取得します。
    Layout* GetLayout() const;

    // 登録されている CaptureTexture の共有情報をインデックスを指定して取得します。
    const CaptureTextureShareInfo* GetCaptureTextureShareInfoByIndex(int index) const;
    // 登録されている VectorGraphicsTexture をインデックスを指定して取得します。
    detail::VectorGraphicsTexture* GetVectorGraphicsTextureShareInfoByIndex(int index) const;
    // 登録されている CaptureTexture の共有情報です。
    int GetCaptureTextureShareInfoCount() const;
    // 登録されている VectorGraphicsTexture の情報です。
    int GetVectorGraphicsTextureShareInfoCount() const;
private:

    // 指定した名前で共有 CaptureTexture が登録されていないか検索します。
    CaptureTextureShareInfo* FindCaptureTextureShareInfo(const char* name) const;
    // キャプチャテクスチャの上書き用データを取得します。
    const ResCaptureTexture* FindCapturePaneOverrideResource(const char* pPaneName) const;

    // 指定した共有情報インスタンスのリソースでの初期化情報を設定します。
    void SetupCaptureTextureInitializeResource(CaptureTextureShareInfo* pShareInfo);

    // この共有情報を使用するレイアウトインスタンス
    Layout*     m_pLayout;
    // レイアウトインスタンスの初期化に使用する ResCaptureTextureList
    const ResCaptureTextureList*  m_pResCaptureTextureList;
    // レイアウトインスタンスの初期化に使用する ResVectorGraphicsTextureList
    const ResVectorGraphicsTextureList*  m_pResVectorGraphicsTextureList;
    // レイアウトインスタンスの初期化に使用する PartsBuildDataAccessor。パーツレイアウトの時のみ設定されます。
    const Layout::PartsBuildDataAccessor*   m_pPartsBuildDataAccessor;

    // 事前にまとめて確保した CaptureTextueShareInfoList を管理するクラス。階層ごとに無駄なメモリを使用しないようにまとめて確保しています。
    BuildPaneTreeContextInstancePool*    m_pInstancePool;

    // CaptureTextueShareInfoList の IntrusiveList。可変長配列としてクラスの固定サイズを削減しています。
    CaptureTextueShareInfoList  m_CaptureTextureShareInfoList;
    // IntrusiveList に要素がない場合に操作ができないため、別途使用数をカウントしています。
    int     m_CaptureTextureShareInfoCount;
    // VectorGraphicsTextueShareInfoList の IntrusiveList。可変長配列としてクラスの固定サイズを削減しています。
    VectorGraphicsTextueShareInfoList  m_VectorGraphicsTextureShareInfoList;
    // IntrusiveList に要素がない場合に操作ができないため、別途使用数をカウントしています。
    int     m_VectorGraphicsTextureShareInfoCount;
};

typedef nn::util::IntrusiveList<DynamicTextureShareInfo, nn::util::IntrusiveListMemberNodeTraits<DynamicTextureShareInfo, &DynamicTextureShareInfo::m_Link, DynamicTextureShareInfo> > DynamicTextureShareInfoList;

//---------------------------------------------------------------------------
//! @brief  動的生成テクスチャ初期化の情報を収集するためのデータ構造のインスタンスプールです。
//!
//! @details
//!     確保可能なインスタンスは IntrusiveList へ追加することができ、
//!     DynamicTextureShareInfo ではこれらのインスタンスの可変長リストを使用することにより
//!     ポインタやインスタンス保存のための固定メモリ領域を削減しています。
//---------------------------------------------------------------------------
class BuildPaneTreeContextInstancePool
{
public:
    // コンストラクタ
    explicit BuildPaneTreeContextInstancePool(void* pReservedMemoryPtr);
    // デストラクタ
    ~BuildPaneTreeContextInstancePool();

    // CaptureTextureShareInfo のインスタンスを確保します。
    CaptureTextureShareInfo* AllocateCaptureTextureShareInfo();
    // VectorGraphicsTextureShareInfo のインスタンスを確保します。
    VectorGraphicsTextureShareInfo* AllocateVectorGraphicsTextureShareInfo();
    // DynamicTextureShareInfo のインスタンスを確保します。
    DynamicTextureShareInfo* AllocateDynamicTextureShareInfo();
private:
    CaptureTextureShareInfo*    m_pCaptureTextureShareInfo;
    int         m_CaptureTextureShareInfoInstanceUseCount;
    int         m_CaptureTextureShareInfoInstanceMax;
    VectorGraphicsTextureShareInfo* m_pVectorGraphicsTextureShareInfo;
    int         m_VectorGraphicsTextureShareInfoInstanceUseCount;
    int         m_VectorGraphicsTextureShareInfoInstanceMax;
    DynamicTextureShareInfo*    m_pDynamicTextureShareInfo;
    int         m_DynamicTextureShareInfoInstanceUseCount;
    int         m_DynamicTextureShareInfoInstanceMax;
};



//---------------------------------------------------------------------------
//! @brief  このデータ構造ではレイアウト内でのテクスチャ共有および、上書きによるテクスチャの共有を実現するための情報を保持しています。
//---------------------------------------------------------------------------
class BuildPaneTreeContext
{
public:
    // BuildPaneTreeContext 内部で動的確保するメモリサイズを計算します。
    static size_t CalculateContextRequireMemorySize();
public:
    // コンストラクタです。ui2d ランタイムではスタック上から確保したメモリを渡して初期化しています。
    BuildPaneTreeContext(void* pContextRequireMemory, size_t allocatedSize);
    // デストラクタです。
    ~BuildPaneTreeContext();

    // BuildPaneTreeContext を初期化します。内部で使用するインスタンスキャッシュの作成などを行います。
    void Initialize();
    // 終了処理です。
    void Finalize();
    // 初期化済みかどうか判定します。
    bool IsInitialized() const;

    // レイアウト単位の共有情報スタックをプッシュします。
    void PushCache(Layout* pLayout, const Layout::PartsBuildDataAccessor* pAccessor);
    // レイアウト単位の共有情報スタックをポップします。
    void PopCache();

    // 現在の共有情報スタックの位置に共有情報を作成します。
    void CreateCahceInfoToCurrentStackIndex(Layout* pLayout, const Layout::PartsBuildDataAccessor* pAccessor);

    // 現在の共有情報スタックが表す共有情報を取得します。
    DynamicTextureShareInfo* GetCurrentTextureShareInfo() const;
    // 共有情報スタックの最上位からのオフセットを指定して共有情報を取得します。
    DynamicTextureShareInfo* GetTextureShareInfoFromRootOffset(int offset) const;
    // 現在の共有情報スタックの深さを取得します。
    int GetCurrentShareInfoStackDepth() const;
    // 現在の共有情報スタックのルートからのオフセットを取得します。
    int GetCurrentShareInfoStackOffset() const;

    // 現在の共有情報スタックの深さから指定したオフセット分上位の共有情報を取得します。
    detail::DynamicTextureShareInfo* GetUpperOffsetedShareInfo(int offset);

    // スタックとは別に内部で管理されている一連のインスタンスリストでのインデックスを指定して共有情報を取得します。
    DynamicTextureShareInfo* GetTextureShareInfoInstanceIndex(int index) const;
    // スタックとは別に内部で管理されている一連のインスタンスリストでのインデックスを指定して共有情報を取得します。
    int GetTextureShareInfoInstanceUsedCount() const;

    // この BuildPaneTreeContext が保持している CaptureTexture の初期化情報について、すべて初期化処理を実行します。
    void InitializeCaptureTexturesAfterPaneTreeBuilt(nn::gfx::Device* pDevice);
private:

    // 共有情報の階層構造を表すスタックです。ペインツリー初期化中に別のレイアウト(部品)に入るたびにひとつづつ深くなります。
    DynamicTextureShareInfo**   m_pDynamicTextureShareInfoStack;
    // 共有情報スタックの現在の深さです。
    int m_StackDepthIndex;
    // 共有情報の階層構造を表すスタックの最大の深さです。
    int m_StackDepthMax;

    // この BuildPaneTreeContext で確保した DynamicTextureShareInfo のインスタンスリストです。
    DynamicTextureShareInfoList* m_pDynamicTextureShareInfoInstanceList;
    // IntrusiveList に要素がない場合に操作ができないため、別途使用数をカウントしています。
    int m_InstanceUsedCount;

    // BuildPaneTreeContext 全体でまとめて事前に確保したインスタンスのプールです。
    BuildPaneTreeContextInstancePool*   m_pInstancePool;

    // 外部から渡してもらった BuildPaneTreeContext が動的に使用するメモリ領域です。
    void*   m_pReservedMemory;
};

} // namespace nn::ui2d::detail


} // namespace nn::ui2d
} // namespace nn
