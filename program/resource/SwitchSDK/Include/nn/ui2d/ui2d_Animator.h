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

#include <nn/ui2d/ui2d_Animation.h>
#include <nn/ui2d/ui2d_Resources.h>

#include <nn/util/util_BitPack.h>

namespace nn
{
namespace ui2d
{

class GroupContainer;

//---------------------------------------------------------------------------
//! @brief アニメーションフレームを制御する機能を持ったAnimTransformです。
//!
//---------------------------------------------------------------------------
class Animator : public AnimTransformBasic
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(AnimTransformBasic);

    //---------------------------------------------------------------------------
    //! @brief アニメーションをどのように再生するかを表します。
    //---------------------------------------------------------------------------
    enum PlayType {
        PlayType_OneShot,   //!< 最後（先頭）までいって停止
        PlayType_Loop,      //!< 最後（先頭）までいったら先頭（最後）から
        PlayType_RoundTrip  //!< 最後（先頭）までいったら往復
    };

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! アニメーションは停止状態です。
    //!
    Animator();

    //! @brief デストラクタです。
    //!
    virtual ~Animator();

    //@}

    //----------------------------------------
    //! @name 再生
    //@{
    //! @brief アニメーションを先頭から再生します。
    //!
    //! @details
    //! speedに負の値を入れた場合は、先頭から再生するのではなく、最後から逆再生します。
    //!
    //! 現在のフレームがどのような値であっても、順再生の場合は0に、逆再生の場合は最大
    //! フレーム値にセットされます。
    //!
    //! また、アニメーションがWaitアニメーションのときは、再生タイプはPlayType_OneShotのみ指定
    //! できます。speedの値がどのような値でも1フレームで再生が完了し、フレーム位置は0
    //! になります。
    //!
    //! @param[in] type     再生タイプを指定する
    //! @param[in] speed    再生スピードを指定する。単位は一回のUpdateFrameで何フレーム進めるかになる。
    //!                     つまり、1.fを指定するとデザイナが指定したのと同じスピード。
    //!                     負の値を指定すると逆再生になる。
    //!                     なお、スピードの絶対値はGetFrameMax()の数値以上にすることはできません。
    //!                     そのような場合はアサートされます。製品版では再生が行われません。
    //!
    virtual void Play(PlayType type, float speed);

    //! @brief アニメーションを、レイアウトエディタで指定された再生タイプで先頭から再生します。
    //!
    //! @details
    //! レイアウトエディタでループが選ばれたとき(IsLoopData()がtrueを返すとき)はPlayType_Loop
    //! になり、それ以外はPlayType_OneShotになります。
    //!
    //! speedに負の値を入れた場合は、先頭から再生するのではなく、最後から逆再生します。
    //!
    //! 現在のフレームがどのような値であっても、順再生の場合は0に、逆再生の場合は最大
    //! フレーム値にセットされます。
    //!
    //! また、アニメーションがWaitアニメーションのときは、speedの値がどのような値でも1フレームで
    //! 再生が完了し、フレーム位置は0になります。
    //!
    //! @param[in] speed    再生スピードを指定する。単位は一回のUpdateFrameで何フレーム進めるかになる。
    //!                     つまり、1.fを指定するとデザイナが指定したのと同じスピード。
    //!                     負の値を指定すると逆再生になる。
    //!                     なお、スピードの絶対値はGetFrameMax()の数値以上にすることはできません。
    //!                     そのような場合はアサートされます。製品版では再生が行われません。
    //!
    virtual void PlayAuto(float speed);

    //! @brief アニメーションを現在のフレームから再生します。
    //!
    //! @details
    //! speedに負の値を入れた場合は逆再生になります。
    //!
    //! このメソッドでは、playのように現在のフレームが変わることはありません。
    //!
    //! また、アニメーションがWaitアニメーションのときは、再生タイプはPlayType_OneShotのみ指定
    //! できます。speedの値がどのような値でも1フレームで再生が完了し、フレーム位置は0
    //! になります。
    //!
    //! @param[in] type     再生タイプを指定する
    //! @param[in] speed    再生スピードを指定する。単位は一回のUpdateFrameで何フレーム進めるかになる。
    //!                     つまり、1.fを指定するとデザイナが指定したのと同じスピード。
    //!                     負の値を指定すると逆再生になる。
    //!                     なお、スピードの絶対値はGetFrameMax()の数値以上にすることはできません。
    //!                     そのような場合はアサートされます。製品版では再生が行われません。
    //!
    virtual void PlayFromCurrent(PlayType type, float speed);
    //@}

    //----------------------------------------
    //! @name 停止
    //@{
    //! @brief アニメーションの再生を指定したフレームで停止します。
    //!
    //! @details
    //! これはアニメーションが再生されないわけではなく、「停止した状態が再生されている」
    //! ことにご注意ください。
    //!
    //! @param[in] frame    停止したいフレーム
    //!
    virtual void StopAt(float frame);

    //! @brief アニメーションの再生を現在のフレームで停止します。
    //!
    virtual void StopAtCurrentFrame();

    //! @brief アニメーションの再生を先頭のフレームで停止します。
    //!
    virtual void StopAtStartFrame();

    //! @brief アニメーションの再生を最後のフレームで停止します。
    //!
    virtual void StopAtEndFrame();
    //@}

    //----------------------------------------
    //! @name フレーム進行
    //@{
    //! @brief アニメーションのフレームを進めます。
    //!
    //! @param[in] progress_frame 進めるフレーム数です。
    //!
    virtual void UpdateFrame(float progress_frame = 1.0f) NN_OVERRIDE;
    //@}

    //----------------------------------------
    //! @name メンバへのアクセス
    //@{
    // 現在のフレームへのアクセスは、AnimTransformクラスのGetFrame及びSetFrameを利用します。
    // ただし、アニメーションの再生中にSetFrameを呼ぶとアニメーションを再生したままフレームを飛ばすような挙動に
    // なりますので、特定のフレームで止めたい場合はStopメソッドを使用してください。

    //! @brief 現在のスピードを返します。
    //!
    //! @return 現在のスピード。アニメーションを再生していないときは0.fが返されます。
    //!
    float GetSpeed() const { return m_Speed; }

    //! @brief アニメーションが再生中か否かを返します。
    //!
    //! @return アニメーションが再生中のときはtrue。そうでないときはfalse。
    //!
    bool IsPlaying() const { return (m_Speed != 0.f); }

    //! @brief 現在のフレームが先頭か否かを返します。
    //!
    //! @return 現在のフレームが先頭であればtrue。そうでなければfalse。
    //!
    bool IsMinFrame() const { return (GetFrame() == 0.f); }

    //! @brief 現在のフレームが先頭か否かを返します。
    //!
    //! @return 現在のフレームが先頭であればtrue。そうでなければfalse。
    //!
    bool IsStartFrame() const { return IsMinFrame(); }

    //! @brief 現在のフレームが最後か否かを返します。
    //!
    //! @return 現在のフレームが最後であればtrue。そうでなければfalse。
    //!
    bool IsMaxFrame() const { return (GetFrame() == GetFrameSize()); }

    //! @brief 現在のフレームが最後か否かを返します。
    //!
    //! @return 現在のフレームが最後であればtrue。そうでなければfalse。
    //!
    bool IsEndFrame() const { return IsMaxFrame(); }

    //! @brief 現在のフレームが先頭でも最後でもないときにtrueを返します。
    //!
    //! @details
    //! ( ! IsMinFrame() && ! IsMaxFrame()) のショートカットです。
    //!
    //! @return 現在のフレームが先頭でも最後でもないときにtrue。そうでなければfalse。
    //!
    bool IsFrameMidway() const { return (GetFrame() != 0.f && GetFrame() != GetFrameSize()); }

    //! @brief 再生のタイプを返します。
    //!
    //! @return 再生のタイプ
    //!
    PlayType GetPlayType() const { return m_PlayType; }

    //! @brief アニメタグの名前を返します。
    //!
    //! @details
    //! 複製することができるボタンを作る際に必要となります。派生クラスで SetupAnimationResource() と合わせて処理をオーバーライドして機能を実装してください。
    //!
    //! @return アニメタグの名前
    //!
    virtual const char* GetTagName() const
    {
        return nullptr;
    };

    //! @brief 無効化
    //!
    //! @details
    //! SetEnable(false)と同じ処理ですが、こちらの方が軽量です。
    //!
    void Disable()
    {
        nn::ui2d::AnimTransform::SetEnabled(false);
        SetSpeed(0.0f);
    }
    //@}

    //----------------------------------------
    //! @name 再生状態トリガ検出
    //@{
    //! @brief UpdateFrameの後、PlayTypeがPlayType_LoopかPlayType_RoundTripのときに、最後のフレームを回ったことを示します。
    //!
    //! @details
    //! 次にUpdateFrameしたときにはこのメソッドはfalseを返しますのでご注意ください。
    //!
    //! @return 最後のフレームを回ったか否か
    //!
    bool IsTurnedMaxTrigger() const { return m_Flag.IsAllBitOn(FlagMask_PlayStatusTurnedMax); }

    //! @brief UpdateFrameの後、PlayTypeがPlayType_LoopかPlayType_RoundTripのときに、先頭のフレームを回ったことを示します。
    //!
    //! @details
    //! 次にUpdateFrameしたときにはこのメソッドはfalseを返しますのでご注意ください。
    //!
    //! @return 先頭のフレームを回ったか否か
    //!
    bool IsTurnedMinTrigger() const { return m_Flag.IsAllBitOn(FlagMask_PlayStatusTurnedMin); }

    //! @brief UpdateFrameの後、PlayTypeがPlayType_OneShotのときに、再生が終了したことを示します。
    //!
    //! @details
    //! IsMaxFrameやIsMinFrameとの違いは、speedに正の値を与えたときも、負の値を与えたときも、
    //! このメソッドで終了したことを検知できることです。
    //!
    //! 明示的にStop等のアニメーション停止メソッドを呼んだときには、このフラグは立たないことにご注意ください。
    //! アニメーションが再生中か否かを知りたい場合は、IsPlayingメソッドを使用してください。
    //!
    //! 次にUpdateFrameしたときにはこのメソッドはfalseを返しますのでご注意ください。
    //!
    //! @return 再生が終了したか否か
    //!
    bool IsEndTrigger() const { return m_Flag.IsAllBitOn(FlagMask_PlayStatusEnd); }

    //! @brief UpdateFrame の後、PlayTypeが PlayType_OneShot のときに、IsEndTrigger() が true になった次のフレームにtrueになります。
    //!
    //! @details
    //! アニメーションの結果がペインのGlobalMtxに反映された状態を取得したい場合等に使用することができます。
    //!
    //! 次にUpdateFrameしたときにはこのメソッドはfalseを返しますのでご注意ください。
    //!
    //! @return IsEndTrigger()が true になった次のフレームか否か
    //!
    bool IsEndNextTrigger() const { return m_Flag.IsAllBitOn(FlagMask_PlayStatusEndNext); }

    //! :private
    //! @brief IsEndNextTrigger()がtrueを返す状態にします。
    void SetEndNextTriggerOn() { m_Flag.SetAllBitOn(FlagMask_PlayStatusEndNext); }

    //@}

    //----------------------------------------
    //! @name バインド解除
    //@{
    //! @brief アニメーションータのペイン等へのバインドを解除します。
    //!
    //! @details
    //! Animatorクラスではバインドの解除はデストラクタでは行いませんので、明示的に
    //! このメソッドを呼び出す必要があります。
    //!
    virtual void Unbind() = 0;
    //@}

    //!
    //! @brief 再生状態のフラグをクリアします。
    //!
    void ForceClearPlayStatusFlag()
    {
        ClearPlayStatusFlag();
    }

protected:
    //---------------------------------------------------------------------------
    //! @brief 再生状態のフラグです。
    //---------------------------------------------------------------------------
    enum FlagMask {
        FlagMask_PlayStatusEnd = 0x1,         //!< PlayType_OneShot再生が終了した
        FlagMask_PlayStatusTurnedMax = 0x2,  //!< PlayType_LoopまたはPlayType_RoundTripで、最後のフレームを回った
        FlagMask_PlayStatusTurnedMin = 0x4,  //!< PlayType_LoopまたはPlayType_RoundTripで、先頭のフレームを回った
        FlagMask_PlayStatusEndNext = 0x8     //!< PlayType_OneShot再生が終了した次のフレームである
    };

    //!
    //! 再生状態のフラグをクリアします。
    //!
    void ClearPlayStatusFlag()
    {
        m_Flag.SetAllBitOff(FlagMask_PlayStatusEnd | FlagMask_PlayStatusTurnedMax | FlagMask_PlayStatusTurnedMin | FlagMask_PlayStatusEndNext);
    }

    //! アニメーションリソース関連のセットアップをします。
    //!
    virtual void SetupAnimationResource(const AnimResource& animRes);

    //! スピードを設定します。
    //!
    //! @param[in] speed スピードです。
    //!
    void SetSpeed(float speed)
    {
        m_Speed = speed;
    }

private:
    float m_Speed;                  //!< 再生スピード
    PlayType m_PlayType;            //!< 再生タイプ
    nn::util::BitPack32 m_Flag;     //!< 再生状態フラグ。FlagMaskのマスク値となります。
};

//---------------------------------------------------------------------------
//! @brief ペインをアニメーションさせるAnimatorです。
//!
//---------------------------------------------------------------------------
class PaneAnimator : public Animator
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(Animator);

    PaneAnimator()
     : Animator()
     , m_pPane(NULL)
    {}

    //! @brief 指定したペインにアニメーションをバインドし、内部にペインのポインタを保持します。
    //!
    //! @param[in] pPane     バインドしたいペイン
    //! @param[in] enabled   開始時にアニメーションを有効状態にするか
    //!
    void Setup(Pane* pPane, bool enabled);

    //! @brief Setupで関連付けたアニメーションをバインド解除します。
    //!
    virtual void Unbind() NN_OVERRIDE;

    //! @brief アニメーションがバインドされているペインを取得します。
    //!
    //! @return バインドされているペイン
    //!
    Pane* GetPane() const { return m_pPane; }

private:
    Pane* m_pPane;        //!< 関連づけられたペイン
};

//---------------------------------------------------------------------------
//! @brief グループをアニメーションさせるAnimatorです。
//!
//---------------------------------------------------------------------------
class GroupAnimator : public Animator
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(Animator);

    GroupAnimator()
     : Animator()
     , m_pGroup(NULL)
    {}

    //! @brief 指定したグループにアニメーションをバインドし、内部にグループのポインタを保持します。
    //!
    //! @param[in] pGroup    バインドしたいグループ
    //! @param[in] enabled   開始時にアニメーションを有効状態にするか
    //!
    void Setup(Group* pGroup, bool enabled);

    //! @brief Setupで関連付けたアニメーションをバインド解除します。
    //!
    virtual void Unbind() NN_OVERRIDE;

    //! @brief 指定したアニメーションリソースに関連づけられているグループのうち、index番目のグループに
    //!        アニメーションをバインドし、内部にグループのポインタを保持します。
    //!
    //! @param[in] animRes          構築するアニメーションのアニメーションリソース
    //! @param[in] pGroupContainer   グループのコンテナ。この中から関連づけるグループを捜します。
    //! @param[in] groupIndex       何番目のグループをバインドするか。
    //! @param[in] enabled           開始時にアニメーションを有効状態にするか。
    //!
    void Setup(const AnimResource& animRes, GroupContainer* pGroupContainer, int groupIndex, bool enabled);

    //! @brief アニメーションがバインドされているグループを取得します。
    //!
    //! @return バインドされているグループ
    //!
    Group* GetGroup() const { return m_pGroup; }

private:
    Group* m_pGroup;      //!< 関連づけられたグループ
};

//---------------------------------------------------------------------------
//! @brief アニメーションリソースに関連付けられたすべてのグループをアニメーションさせるAnimatorです。
//!
//---------------------------------------------------------------------------
class GroupArrayAnimator : public Animator
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(Animator);

    GroupArrayAnimator()
     : Animator()
     , m_pGroupArray(NULL)
     , m_GroupCount(0)
    {}

    //! @brief 指定したアニメーションリソースに関連づけられているグループのうち、index番目のグループに
    //!        アニメーションをバインドし、内部にグループのポインタを保持します。
    //!
    //! @param[in] animRes          構築するアニメーションのアニメーションリソース
    //! @param[in] pGroupContainer   グループのコンテナ。この中から関連づけるグループを捜します。
    //! @param[in] pGroupBuffer      関連づけられているグループのポインタを格納するためのバッファ。(animRes.GetGroupCount() * sizeof(Group*))の
    //!                             長さの配列を指定してください。このバッファは、このオブジェクトが破棄されるまで破棄しないでください。
    //! @param[in] enabled           開始時にアニメーションを有効状態にするか。
    //!
    void Setup(const AnimResource& animRes, GroupContainer* pGroupContainer, Group** pGroupBuffer, bool enabled);

    //! @brief Setupで関連付けたアニメーションをバインド解除します。
    //!
    virtual void Unbind() NN_OVERRIDE;

    //! @brief アニメーションがバインドされているグループのポインタの配列を取得します。
    //!
    //! 配列の長さはGetGroupCount()で取得できる数となります。
    //!
    //! @return バインドされているグループの配列
    //!
    Group** GetGroups() const { return m_pGroupArray; }

    //! @brief アニメーションがバインドされているグループの数を取得します。
    //!
    //! @return バインドされているグループの数
    //!
    int GetGroupCount() const { return m_GroupCount; }

private:
    Group** m_pGroupArray;    //!< 関連づけられたグループの配列
    int32_t  m_GroupCount;      //!< 関連付けられたグループの数

};

} // namespace nn::ui2d
} // namespace nn
