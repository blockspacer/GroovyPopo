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

#include <nn/util/util_IntrusiveList.h>

#include <nn/ui2d/ui2d_Util.h>
#include <nn/font/font_ResourceFormat.h>
#include <nn/ui2d/ui2d_Types.h>

namespace nn
{
namespace ui2d
{

struct BinaryFileHeader;
struct ResAnimationBlock;
struct ResAnimationTagBlock;
struct ResAnimationShareBlock;
struct ResAnimationGroupRef;
struct ResAnimationShareInfo;
struct ResAnimationContent;

namespace detail
{

class AnimPaneTreeLink;

} // namespace detail

class Pane;
class Layout;
class Group;
class Material;
class ResourceAccessor;
class TextureInfo;

//---------------------------------------------------------------------------
//! @brief アニメーションの基底クラスです。
//!
//---------------------------------------------------------------------------
class AnimTransform
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO_ROOT();

    //---------------------------------------------------------------------------
    //! @brief バインドしたアニメーション内容と適用させるターゲットを保持する構造体です。
    //!
    //! @details
    //! targetは、ペインの場合とマテリアルの場合があります。
    //! animCont->typeで区別してください。
    //!
    //---------------------------------------------------------------------------
    class BindPair
    {
    public:// TODO:アクセサ
        void* target;
        const ResAnimationContent* animCont;

        BindPair() : target(NULL), animCont(NULL) {}
    };

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    AnimTransform();

    //! @brief デストラクタです。
    //!
    virtual ~AnimTransform();

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief 現在のフレーム位置を取得します。
    //!
    //! @return 現在のフレーム位置を返します。
    //!
    //! @sa SetFrame
    //!
    float GetFrame() const
    {
        return m_Frame;
    }

    //! @brief フレーム位置を設定します。
    //!
    //! @param[in] frame  フレーム位置です。
    //!
    //! @sa GetFrame
    //!
    void SetFrame(float frame)
    {
        m_Frame = frame;
    }

    //! @brief フレームの長さを取得します。
    //!
    //! @return フレームの長さを返します。
    //!
    uint16_t  GetFrameSize() const;

    //! @brief フレーム位置の最大値を取得します。
    //!
    //! @return フレーム位置の最大値を返します。
    //!
    float GetFrameMax() const
    {
        return static_cast<float>(GetFrameSize());
    }

    //! @brief アニメーションのフレームを進めます。
    //!
    //! @details
    //! このメソッドは、デフォルトでは何も行いません。
    //! 必要な場合に派生クラスで実装を行います。
    //!
    //! @param[in] progressFrame 進めるフレーム数です。
    //!
    virtual void UpdateFrame(float progressFrame = 1.0f);

    //! @brief アニメーションブロックを取得します。
    //!
    //! @return アニメーションブロックへのポインタを返します。
    //!
    //! @sa SetResource
    //!
    const ResAnimationBlock* GetAnimResource() const
    {
        return m_pRes;
    }

    //! @brief 有効か否かを取得します。
    //!
    //! @return 有効か否か
    //!
    bool IsEnabled() const
    {
        return m_IsEnabled;
    }

    //! @brief 有効か否かを設定します。
    //!
    //! @param[in] bEnabled  有効か否か
    //!
    virtual void SetEnabled(bool bEnabled);
    //@}

    //----------------------------------------
    //! @name アニメーション
    //@{

    //! @brief ループ用のアニメーションデータか判定します。
    //!
    //! @details
    //! アニメーションデータがループ用としてマークされているかどうかを
    //! 判定します。
    //!
    //! @return
    //! アニメーションデータがループ用としてマークされている場合は true を
    //! 返します。
    //!
    bool IsLoopData() const;

    //! @brief Waitのアニメーションデータか判定します。
    //!
    //! @details
    //! アニメーションデータがWaitのアニメーションデータか判定します。
    //! Waitのアニメーションは、フレーム長が0のデータで、レイアウト
    //! エディタのアニメーション区間タグ設定の再生モードで「ウェイト」
    //! が設定されたものです。
    //!
    //! @return
    //! アニメーションがWaitアニメーションである場合はtrueを返します。
    //!
    bool IsWaitData() const;

    //! @brief 関連付けられているペイン及びマテリアルのアニメーションを全て再生します。
    //!
    //! @sa Bind
    //!
    virtual void Animate() = 0;

    //! @brief アニメーションを再生します。
    //!
    //! @details
    //! pPane で指定したペインのアニメーションを再生します。
    //!
    //! @param[in] pPane    ペインです。
    //!
    //! @sa Bind
    //!
    virtual void AnimatePane(Pane* pPane) = 0;

    //! @brief アニメーションを再生します。
    //!
    //! @details
    //! pMaterial で指定したマテリアルのアニメーションを再生します。
    //!
    //! @param[in] pMaterial    マテリアルです。
    //!
    //! @sa Bind
    //!
    virtual void AnimateMaterial(Material* pMaterial) = 0;

    //@}

    //----------------------------------------
    //! @name リソース
    //@{

    //! @brief アニメーションが使用するリソースを設定します。
    //!
    //! @param[in] pDevice           デバイスです。
    //! @param[in] pResAccessor  リソースアクセサです。
    //! @param[in] pRes              リソースのアニメーションブロックへのポインタです。
    //!
    //! @details
    //! Bind() でアニメーションに関連付けるアニメーションブロックデータと
    //! アニメーションデータに必要なリソースを設定します。
    //!
    //! また、 Bind() で必要になるBindPairの配列 を
    //! アニメーションブロックに含まれるアニメーション対象の個数分確保します。
    //!
    //! この関数の実装では SetAnimResource() を使用して、アニメーション
    //! ブロックデータを登録しなければなりません。
    //!
    //! @sa Bind
    //! @sa SetAnimResource
    //!
    virtual void SetResource(
        nn::gfx::Device* pDevice,
        ResourceAccessor* pResAccessor,
        const ResAnimationBlock* pRes) = 0;

    //! @brief アニメーションが使用するリソースを設定します。
    //!
    //! @param[in] pDevice          デバイスです。
    //! @param[in] pResAccessor リソースアクセサです。
    //! @param[in] pRes             リソースのアニメーションブロックへのポインタです。
    //! @param[in] animCount          確保するポインタ配列の要素数です。
    //!
    //! @details
    //! Bind() でアニメーションに関連付けるアニメーションブロックデータと
    //! アニメーションデータに必要なリソースを設定します。
    //!
    //! また、 Bind() で必要になるBindPairの配列 を
    //! animCount で指定された個数、確保します。
    //!
    //! 本関数の実装では SetAnimResource() を使用して、アニメーション
    //! ブロックデータを登録しなければなりません。
    //!
    //! @sa Bind
    //! @sa SetAnimResource
    //!
    virtual void SetResource(
        nn::gfx::Device* pDevice,
        ResourceAccessor* pResAccessor,
        const ResAnimationBlock* pRes,
        uint16_t  animCount) = 0;

    //! @brief アニメーションの関連付けを行います。
    //!
    //! @details
    //! pPane で指定したペインにアニメーションを関連付けます。
    //!
    //! bRecursive に true を渡して呼び出した場合は、ペインを pPane の子からも
    //! 検索します。
    //!
    //! @param[in] pPane        ペインへのポインタです。
    //! @param[in] bRecursive   再帰的に検索する場合は true を指定します。
    //!
    virtual void  BindPane(
        Pane* pPane,
        bool bRecursive) = 0;

    //! @brief アニメーションの関連付けを行います。
    //!
    //! @details
    //! pGroup で指定したグループに所属するペインにアニメーションを関連付けます。
    //!
    //! @param[in] pGroup   グループへのポインタです。
    //!
    virtual void BindGroup(
        Group* pGroup) = 0;

    //! @brief アニメーションの関連付けを行います。
    //!
    //! @details
    //! pMaterial で指定したマテリアルにアニメーションを関連付けます。
    //!
    //! @param[in] pMaterial   マテリアルへのポインタです。
    //!
    virtual void BindMaterial(Material* pMaterial) = 0;

    //! @brief 強制的に別のペインのアニメーションの関連付けを行います。
    //!
    //! @details
    //! pDstPane で指定したペインに、pSrcPaneで指定したペインの
    //! アニメーションを関連付けます。
    //!
    //! 本来異なるペインに付いているアニメーションをバインドすることになりますので、
    //! 例えばペインの種類が異なっていたりすると不正な動作になってしまいます。
    //! 十分に注意して使用してください。
    //!
    //! なお、関連付けたアニメーションはUnbindPaneで解除できます。
    //!
    //! @param[in] pDstPane 強制的に関連付けたい先のペインへのポインタです。
    //! @param[in] pSrcPane 強制的に関連付けたい元のアニメーションを持つペインの名前です。
    //!
    virtual void  ForceBindPane(
        Pane* pDstPane,
        const Pane* pSrcPane) = 0;

    //! @brief アニメーションの関連付けを解除します。
    //!
    //! @details
    //! pPane で指定したペインのアニメーションの関連付けを解除します。
    //!
    //! @param[in] pPane    ペインへのポインタです。
    //!
    virtual void UnbindPane(const Pane* pPane) = 0;

    //! @brief アニメーションの関連付けを解除します。
    //!
    //! @details
    //! pGroup で指定したグループのアニメーションの関連付けを解除します。
    //!
    //! @param[in] pGroup   グループへのポインタです。
    //!
    virtual void UnbindGroup(const Group* pGroup) = 0;

    //! @brief アニメーションの関連付けを解除します。
    //!
    //! @details
    //! pMaterial で指定したマテリアルのアニメーションの関連付けを解除します。
    //!
    //! @param[in] pMaterial    マテリアルへのポインタです。
    //!
    virtual void UnbindMaterial(const Material* pMaterial) = 0;

    //! @brief 全てのアニメーションの関連付けを解除します。
    //!
    virtual void UnbindAll() = 0;

    //@}

    // レイアウトクラスが管理するためのリンク情報です。

    //! @brief 内部用機能のため使用禁止です。
    nn::util::IntrusiveListNode m_Link;

protected:
    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief アニメーションリソースへのポインタを設定します。
    //!
    //! @details
    //! SetResource() の実装内で使用され、 GetAnimResource() の戻り値を設定します。
    //!
    //! @param[in] pRes アニメーションブロックへのポインタです。
    //!
    //! @sa SetResource
    //! @sa GetAnimResource
    //!
    void SetAnimResource(const ResAnimationBlock* pRes)
    {
        m_pRes = pRes;
    }

    //@}

private:
    // アニメーションリソースです。
    const ResAnimationBlock* m_pRes;

    // カレントフレーム値です。
    float m_Frame;

    // 有効か否かです。
    bool m_IsEnabled;
};

//---------------------------------------------------------------------------
//! @brief アニメーションの基礎クラスです。
//!
//---------------------------------------------------------------------------
class AnimTransformBasic : public AnimTransform
{
private:
    typedef AnimTransform Base;

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(AnimTransform);

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    AnimTransformBasic();

    //! @brief デストラクタです。
    //!
    virtual ~AnimTransformBasic();

    //@}

    //----------------------------------------
    //! @name アニメーション
    //@{

    virtual void Animate() NN_OVERRIDE;

    virtual void AnimatePane(Pane* pPane) NN_OVERRIDE;

    virtual void AnimateMaterial(Material* pMaterial) NN_OVERRIDE;

    //@}

    //----------------------------------------
    //! @name リソース
    //@{

    virtual void SetResource(
        nn::gfx::Device* pDevice,
        ResourceAccessor* pResAccessor,
        const ResAnimationBlock* pRes) NN_OVERRIDE;

    virtual void SetResource(
        nn::gfx::Device* pDevice,
        ResourceAccessor* pResAccessor,
        const ResAnimationBlock* pRes,
        uint16_t  animCount) NN_OVERRIDE;

    virtual void BindPane(
        Pane* pPane,
        bool bRecursive) NN_OVERRIDE;

    virtual void BindGroup(
        Group* pGroup) NN_OVERRIDE;

    virtual void BindMaterial(
        Material* pMaterial) NN_OVERRIDE;

    virtual void  ForceBindPane(
        Pane* pDstPane,
        const Pane* pSrcPane) NN_OVERRIDE;

    virtual void UnbindPane(const Pane* pPane) NN_OVERRIDE;

    virtual void UnbindGroup(const Group* pGroup) NN_OVERRIDE;

    virtual void UnbindMaterial(const Material* pMaterial) NN_OVERRIDE;

    virtual void UnbindAll() NN_OVERRIDE;

    //@}

    // ペインのアニメーションの関連付けを行います。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pTarget  関連付けを行う対象です。
    //! @param[in] pAnimContent アニメーションです。
    //!
    //! @return 成功したら true を返します。
    //!
    bool BindPaneImpl(
        Pane* pTarget,
        const ResAnimationContent* pAnimContent);

    // マテリアルのアニメーションの関連付けを行います。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pTarget  関連付けを行う対象です。
    //! @param[in] pAnimContent アニメーションです。
    //!
    //! @return 成功したら true を返します。
    //!
    bool BindMaterialImpl(
        Material* pTarget,
        const ResAnimationContent* pAnimContent);

    // 拡張ユーザーデータのアニメーションの関連付けを行います。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pTarget  関連付けを行う対象です。
    //! @param[in] pAnimContent アニメーションです。
    //!
    //! @return 成功したら true を返します。
    //!
    bool BindExtUserDataImpl(
        ResExtUserData* pTarget,
        const ResAnimationContent* pAnimContent);

protected:
    virtual void AnimatePaneImpl(Pane* pPane, const ResAnimationContent* pAnimContent);

    virtual void AnimateMaterialImpl(Material* pMaterial, const ResAnimationContent* pAnimContent);

    virtual void AnimateExtUserDataImpl(ResExtUserData* pExtUserData, const ResAnimationContent* pAnimContent);

    virtual void AnimatePartsStateLayerImpl(Pane* pPane, const ResAnimationContent* pAnimContent);


    bool BindExtUserDataToPane(Pane* pPane, const ResAnimationContent& animContent);

    bool CheckBindAnimationDoubly(const void* target, const ResAnimationContent* pAnimContent) const;

    void EraseBindPair(int pos);

private:
    //! テクスチャの情報です。
    const TextureInfo** m_pTextures;

    //! BindPairの配列です。
    BindPair* m_pBindPairs;

    //! バインドされているBindPairの数です。
    uint16_t  m_BindPairCount;

    //! BindPairの最大数です。
    uint16_t  m_BindPairCountMax;

};

//---------------------------------------------------------------------------
//! @brief アニメーションリソースクラスです。
//!
//---------------------------------------------------------------------------
class AnimResource
{
public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! リソースの関連付けは行われません。
    //! 別途 Set() を使用して関連付けを行います。
    //!
    //! @sa Set
    //!
    AnimResource();

    //! @brief コンストラクタです。
    //!
    //! @details
    //! anmResBuf で指定したアニメーションリソースが関連付けられます。
    //!
    //! @param[in] pAnimResBuf    アニメーションリソースへのポインタです。
    //!
    explicit AnimResource(const void* pAnimResBuf)
    {
        Set(pAnimResBuf);
    }

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief アニメーションリソースを関連付けます。
    //!
    //! @param[in] pAnimResBuf    アニメーションリソースへのポインタです。
    //!
    void Set(const void* pAnimResBuf);

    //! @brief アニメーションリソースを取得します。
    //!
    //! @return ファイルヘッダへのポインタを返します。
    //!
    const nn::font::detail::BinaryFileHeader* GetFileHeader() const
    {
        return m_pFileHeader;
    }

    //! @brief アニメーションブロックを取得します。
    //!
    //! @return アニメーションブロックへのポインタを返します。
    //!
    const ResAnimationBlock* GetResourceBlock() const
    {
        return m_pResBlock;
    }

    //! @brief アニメーション区間タグブロックを取得します。
    //!
    //! @return アニメーション区間タグブロックへのポインタを返します。
    //!
    const ResAnimationTagBlock* GetTagBlock() const
    {
        return m_pTagBlock;
    }

    //! @brief アニメーション区間タグの通し番号を取得します。
    //!
    //! @return アニメーション区間タグの0から始まる通し番号を返します。
    //!
    uint16_t  GetTagOrder() const;

    //! @brief アニメーション区間タグの名前を取得します。
    //!
    //! @return アニメーション区間タグの名前を返します。
    //!
    const char* GetTagName() const;

    //! @brief アニメーション区間タグのグループ数を取得します。
    //!
    //! @return アニメーション区間タグに設定されているグループ数を返します。
    //!
    uint16_t  GetGroupCount() const;

    //! @brief アニメーション区間タグのグループ配列を取得します。
    //!
    //! @return
    //! アニメーション区間タグに設定されているグループ配列の
    //! 先頭ポインタを返します。
    //!
    const ResAnimationGroupRef* GetGroupArray() const;

    //! @brief 拡張ユーザーデーターリストを取得します。
    //!
    //! @return
    //! 拡張ユーザーデーターリストのポインタを返します。存在しない場合は、NULL が返ります。
    //!
    const ResExtUserDataList* GetExtUserDataList() const;

    //! @brief
    //! アニメーションをバインドするとき、ペインの子孫となるペインも対象に
    //! するかどうかを判定します。
    //!
    //! @return
    //! アニメーションをバインドするとき、子孫のペインも対象にするときは
    //! true を返します。
    //!
    bool IsDescendingBind() const;

    //! @brief アニメーション共有情報の個数を取得します。
    //!
    //! @return アニメーション共有情報の個数を返します。
    //!
    uint16_t  GetAnimationShareInfoCount() const;

    //! @brief アニメーション共有情報の配列を取得します。
    //!
    //! @return ResAnimationShareInfo オブジェクト配列の先頭ポインタを返します。
    //!
    const ResAnimationShareInfo* GetAnimationShareInfoArray() const;

    //! @brief バインドされるアニメーションの個数を計算します。
    //!
    //! @details
    //! pPane で指定したペインにバインドされるアニメーションの個数を計算します。
    //!
    //! bRecursive に true を指定した場合は、関連付けるペインを子ペインからも
    //! 検索します。
    //!
    //! @param[in] pPane        ペインへのポインタです。
    //! @param[in] bRecursive   子ペインも検索するか指定します。
    //!
    //! @return バインドされるアニメーションの個数を返します。
    //!
    uint16_t  CalculateAnimationCount(Pane* pPane, bool bRecursive) const;

    //! @brief バインドされるアニメーションの個数を計算します。
    //!
    //! @details
    //! pMaterial で指定されたマテリアルにバインドされるアニメーションの
    //! 個数を計算します。
    //!
    //! @param[in] pMaterial    マテリアルへのポインタです。
    //!
    //! @return バインドされるアニメーションの個数を返します。
    //!
    uint16_t  CalculateAnimationCount(Material* pMaterial) const;

    //! @brief バインドされるアニメーションの個数を計算します。
    //!
    //! @details
    //! pGroup で指定されたグループに含まれるペインにバインドされる
    //! アニメーションの個数を計算します。
    //!
    //! bRecursive に true を指定した場合は、関連付けるペインを子ペインからも
    //! 検索します。
    //!
    //! @param[in] pGroup       グループへのポインタです。
    //! @param[in] bRecursive   子ペインも検索するか指定します。
    //!
    //! @return バインドされるアニメーションの個数を返します。
    //!
    uint16_t  CalculateAnimationCount(Group* pGroup, bool bRecursive) const;

    //@}

private:
    void Initialize();

    //! @brief アニメーションリソースが設定されているか確認します。
    //!
    //! @return 設定されていれば true を返します。
    //!
    bool CheckResource() const;

    const nn::font::detail::BinaryFileHeader* m_pFileHeader;
    const ResAnimationBlock* m_pResBlock;
    const ResAnimationTagBlock* m_pTagBlock;
    const ResAnimationShareBlock* m_pShareBlock;
};

namespace detail
{

class AnimPaneTree
{
public:
    AnimPaneTree();

    AnimPaneTree(
        Pane* pTargetPane,
        const AnimResource& animRes);

    void Set(
        Pane* pTargetPane,
        const AnimResource& animRes);

    AnimTransform* Bind(
        nn::gfx::Device* pDevice,
        Layout* pLayout,
        Pane* pTargetPane,
        ResourceAccessor* pResAccessor) const;

    bool IsEnabled() const
    {
        return m_LinkCount > 0;
    }

    const AnimResource& GetAnimResource() const
    {
        return m_AnimRes;
    }

private:
    static const ResAnimationContent* FindAnimContent(
        const ResAnimationBlock* pAnimBlock,
        const char* pAnimContentName,
        uint8_t  animContentType);

    void Initialize();

    static const int MaterialCountMax = 1 + 4 + 4;

    AnimResource m_AnimRes;

    const ResAnimationContent* m_pPaneAnimContent;
    const ResAnimationContent* m_pMatAnimContents[MaterialCountMax];

    uint16_t  m_LinkCount;
    uint8_t  m_AnimMatCnt;

};

} // namespace nn::ui2d::detail
} // namespace nn::ui2d
} // namespace nn
