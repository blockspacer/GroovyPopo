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

#include <nn/ui2d/ui2d_Animator.h>

namespace nn
{
namespace ui2d
{

class GroupContainer;
class LayoutEx;

//---------------------------------------------------------------------------
//! @brief アニメーションを管理するクラスです。
//!
//! @details
//! 通常の Animator と以下のような違いがあります。
//!
//! - 自身が所属する Layout や Screen への参照を持っています。
//! - 再生が完了したら Screen が管理しているアニメーションの更新リストから抜ける処理が実装されています。
//!
//! 注意：試験実装中のため、利用しないでください。
//!
//---------------------------------------------------------------------------
class AnimatorEx : public Animator
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(AnimTransformBasic);

    AnimatorEx();
    virtual ~AnimatorEx();

    //----------------------------------------
    //! @name セットアップ
    //@{

    //! @brief 基本的なセットアップを行います。
    //!
    //! @param[in] animRes 構築するアニメのアニメーションリソース
    //! @param[in] pLayout このアニメを保持するレイアウト
    //! @param[in] isEnabled   開始時にアニメを有効状態にするか
    //!
    void SetupBasic(const nn::ui2d::AnimResource& animRes, nn::ui2d::LayoutEx* pLayout, bool isEnabled);

    //! @brief 指定したペインをバインドしてセットアップします。
    //!
    //! @param[in] animRes 構築するアニメのアニメーションリソース
    //! @param[in] pLayout このアニメを保持するレイアウト
    //! @param[in] pPane     バインドしたいペイン
    //! @param[in] isEnabled   開始時にアニメを有効状態にするか
    //!
    void SetupWithPane(const nn::ui2d::AnimResource& animRes, nn::ui2d::LayoutEx* pLayout, nn::ui2d::Pane* pPane, bool isEnabled);

    //! @brief 指定したグループをバインドしてセットアップします。
    //!
    //! @param[in] animRes 構築するアニメのアニメーションリソース
    //! @param[in] pLayout このアニメを保持するレイアウト
    //! @param[in] pGroup    バインドしたいグループ
    //! @param[in] isEnabled   開始時にアニメを有効状態にするか
    //!
    void SetupWithGroup(const nn::ui2d::AnimResource& animRes, nn::ui2d::LayoutEx* pLayout, nn::ui2d::Group* pGroup, bool isEnabled);

    //! @brief 指定したアニメーションリソースに関連づけられているグループのうち、index番目のグループをバインドしてセットアップします。
    //!
    //! @param[in] animRes         構築するアニメのアニメーションリソース
    //! @param[in] pLayout   このアニメを保持するレイアウト
    //! @param[in] groupContainer   グループのコンテナ。この中から関連づけるグループを捜します。
    //! @param[in] groupIndex       何番目のグループをバインドするか。
    //! @param[in] isEnabled           開始時にアニメを有効状態にするか。
    //!
    void SetupWithGroupIndex(const nn::ui2d::AnimResource& animRes, nn::ui2d::LayoutEx* pLayout, nn::ui2d::GroupContainer* groupContainer, uint32_t groupIndex, bool isEnabled);

    //! @brief 指定したアニメーションリソースに関連づけられているグループ全てをバインドしてセットアップします。
    //!
    //! @param[in] animRes          構築するアニメのアニメーションリソース
    //! @param[in] pLayout   このアニメを保持するレイアウト
    //! @param[in] groupContainer   グループのコンテナ。この中から関連づけるグループを捜します。
    //! @param[in] isEnabled           開始時にアニメを有効状態にするか。
    //!
    void SetupWithGroupAll(const nn::ui2d::AnimResource& animRes, nn::ui2d::LayoutEx* pLayout, nn::ui2d::GroupContainer* groupContainer, bool isEnabled);

    //@}

    virtual void SetEnabled(bool isEnabled) NN_OVERRIDE;

    //----------------------------------------
    //! @name バインド解除
    //@{
    //! @brief アニメーションータのペイン等へのバインドを解除します。
    //!
    //! @details
    //! Animatorクラスではバインドの解除はデストラクタでは行いませんので、明示的に
    //! このメソッドを呼び出す必要があります。
    //!
    virtual void Unbind() NN_OVERRIDE
    {
        // 何も処理をしません。
    }

    //! @brief 無効化し、Screenのアクティブリストから削除します。
    //!
    //! @details
    //! アニメの処理順が問題になる場合に使用するメソッドです。
    //! 通常は使用しないでください。
    //!
    void DisableAndEraseFromActiveList();

    //! @brief 再生時(Play、PlayAuto、PlayFromCurrent)に、最初のフレームを飛ばして再生するか否かを設定します。
    //!
    //! @details
    //! 通常レイアウトのアニメは、再生開始したフレームでは、アニメの最初のフレームを表示します。これは、通常の
    //! アニメでは問題ないのですが、ボタンのアニメなど、レスポンスが要求される場合は、例えば1フレームのアニメで
    //! あれば、アニメを再生したフレームに最終フレームまで行ってしまった方がよい場合があります。このメソッドで
    //! trueを指定すると、最初のフレームを飛ばして再生するため、1フレームのアニメであれば、Playを呼び出した
    //! フレームに最終状態になります。初期状態ではfalseです。
    //!
    //! @param isSkip 最初のフレームを飛ばして再生するか
    //!
    void SetSkipFirstFrame(bool isSkip) { m_Flag2.SetMaskedBits(FlagMask_PlayOptionSkipFirstFrame, isSkip); }

    //! @brief 再生時(Play、PlayAuto、PlayFromCurrent)に、最初のフレームを飛ばして再生するか否かを返します。
    //!
    //! @return 最初のフレームを飛ばして再生するか
    //!
    bool IsSkipFirstFrame() const { return m_Flag2.IsAllBitOn(FlagMask_PlayOptionSkipFirstFrame); }

    //! @brief アニメタグの名前を返します。
    //!
    //! @return アニメタグの名前
    //!
    virtual const char* GetTagName() const NN_OVERRIDE
    {
        return nn::util::ConstBytePtr(m_pResAnimationTagBlock, m_pResAnimationTagBlock->nameOffset).Get<const char>();
    };

    //! @brief スクリーン側のアクティブなアニメを管理しているリストに接続されているかを取得します。
    //!
    bool IsActive() const
    {
        return m_ActivenessLink.IsLinked();
    }

    //! @brief このアニメを保持しているレイアウトを取得します。
    //!
    //! @return レイアウト
    //!
    const LayoutEx* GetLayout() const { return m_pLayout; };

    //! @brief このアニメを保持しているレイアウトを取得します。
    //!
    //! @return レイアウト
    //!
    LayoutEx* GetLayout() { return m_pLayout; };

    //! :private
    //! @brief スクリーンクラスがアクティブ状態を管理するためのリンク情報です。
    nn::util::IntrusiveListNode m_ActivenessLink;

protected:
    //---------------------------------------------------------------------------
    //! @brief 再生状態のフラグです。(Animator::FlagMaskと重ならないように定義します)
    //---------------------------------------------------------------------------
    enum FlagMask
    {
        FlagMask_PlayOptionSkipFirstFrame = 0x8,         //!< 再生時(Play、PlayAuto、PlayFromCurrent)に、最初のフレームを飛ばして再生するか否か
    };

    virtual void SetupAnimationResource(const AnimResource& animRes) NN_OVERRIDE
    {
        m_pResAnimationTagBlock = animRes.GetTagBlock();
    }

private:
    nn::ui2d::LayoutEx*                     m_pLayout;                  //!< レイアウトです。
    const nn::ui2d::ResAnimationTagBlock*   m_pResAnimationTagBlock;    //!< 関連するアニメーションリソースです。
    nn::util::BitPack32                     m_Flag2;                    //!< 再生状態フラグ。FlagMaskのマスク値となります。
};

} // namespace nn::ui2d
} // namespace nn
