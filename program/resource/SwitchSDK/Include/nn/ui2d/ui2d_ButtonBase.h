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

#include <nn/nn_SdkAssert.h>
#include <nn/util/util_BitPack.h>

#include <nn/ui2d/ui2d_Types.h>

namespace nn
{
namespace ui2d
{

//----------------------------------------------------------
class ControlBase
{
public:
    NN_UI2D_RUNTIME_TYPEINFO_ROOT();

    ControlBase()
        : m_pLayout(nullptr)
    {
    }

    virtual ~ControlBase()
    {
    }

    virtual void Initialize(nn::gfx::Device* pDevice, nn::ui2d::Layout* pLayout)
    {
        NN_UNUSED(pDevice);
        NN_UNUSED(pLayout);
        m_pLayout = pLayout;
    }

    virtual void Finalize(nn::gfx::Device* pDevice)
    {
        NN_UNUSED(pDevice);
        m_pLayout = nullptr;
    }

    virtual void UpdateControl(float step)
    {
        NN_SDK_ASSERT(false);
        NN_UNUSED(step);
    }

    virtual void UpdateControlUserInput(const nn::util::Float2* pPos, bool isDown, bool isRelease = false)
    {
        NN_SDK_ASSERT(false);
        NN_UNUSED(pPos);
        NN_UNUSED(isDown);
        NN_UNUSED(isRelease);
    }


    //----------------------------------------------------------

    nn::ui2d::Layout* GetLayout() { return m_pLayout; }
    const nn::ui2d::Layout* GetLayout() const { return m_pLayout; }

    //----------------------------------------------------------

    nn::ui2d::Layout* m_pLayout;
    nn::util::IntrusiveListNode m_Link;
};

typedef nn::util::IntrusiveList<ControlBase, nn::util::IntrusiveListMemberNodeTraits<ControlBase, &ControlBase::m_Link> > ControlList;

//---------------------------------------------------------------------------
//! @brief ボタンの状態遷移を抽象化したクラスです。
//!
//---------------------------------------------------------------------------
class ButtonBase // : public ControlBase
{
public:
    //! ステートを示します。
    enum State {
        State_OffIdle,     //!< OFF状態で止まっている
        State_On,           //!< ONアニメーション再生中
        State_Off,          //!< OFFアニメーション再生中
        State_OnIdle,      //!< ON状態で止まっている
        State_Down,         //!< DOWNアニメーション再生中
        State_DownIdle,    //!< DOWN状態で止まっている
        State_Cancel        //!< CANCELアニメーション再生中
    };

    /// ボタンへの操作を示します。
    enum Action {
        Action_On,      //!< カーソルがボタンに乗ったときのアクション
        Action_Off,     //!< カーソルがボタンから離れたときのアクション
        Action_Down,    //!< カーソルがボタンに乗った状態で決定ボタンを押したときのアクション
        Action_Cancel   //!< 押されたままの状態になっているボタンを押されていない状態に戻すときのアクション
    };

    //----------------------------------------
    //! @name 構築／破棄
    //@{
    //! @brief コンストラクタです。
    //!
    //! @details
    //! 状態の初期化を行います。
    //!
    ButtonBase();

    //! @brief デストラクタです。デフォルトでは何もしません。
    //!
    virtual ~ButtonBase();
    //@}

    //----------------------------------------
    //! @name アクションの発行
    //@{
    //! @brief onアクションを発行します。
    //!
    //! @details
    //! 無効状態では、この関数は何もしません。
    //!
    virtual void On();

    //! @brief offアクションを発行します。
    //!
    //! @details
    //! 無効状態でも、このアクションは受け付けます。
    //!
    virtual void Off();

    //! @brief downアクションを発行します。
    //!
    //! @details
    //! 無効状態では、この関数は何もしません。
    //!
    virtual void Down();

    //! @brief cancelアクションを発行します。
    //!
    //! @details
    //! 例えばラジオボタンのように「複数あるうちの一つしか有効にならない」挙動の場合に、
    //! ボタンが押されたらそれ以外の全てのボタンにcancelを発行するというような用途で使用します。
    //!
    virtual void Cancel();
    //@}

    //----------------------------------------
    //! @name ステートの強制的な変更
    //@{
    //! @brief ボタンのステートを強制的にSTATE_OFF_IDLEに変更します。
    //!
    //! @details
    //! アクションキューは何も入っていない状態になります。
    //! ステートの強制的な変更を行うと、アニメーションが繋がらなくなる恐れがありますので、注意してご使用ください。
    //!
    //! なお、このメソッドはボタンの有効/無効に関わらずステートを変更することができます。
    //!
    virtual void ForceOff();

    //! @brief ボタンのステートを強制的にSTATE_ON_IDLEに変更します。
    //!
    //! @details
    //! その他の注意点等は@ref ForceOff と同じです。
    //!
    virtual void ForceOn();

    //! @brief ボタンのステートを強制的にSTATE_DOWN_IDLEに変更します。
    //!
    //! @details
    //! その他の注意点等は@ref ForceOff と同じです。
    //!
    virtual void ForceDown();
    //@}

    //----------------------------------------
    //! @name 状態の更新
    //@{
    //! @brief 状態の更新を行います。
    //!
    //! @details
    //! ゲームフレームを進めるたびに1回呼び出してください。
    //!
    virtual void Update();
    //@}

    //----------------------------------------
    //! @name 状態の設定／取得
    //@{
    //! @brief 現在の状態を取得します。
    //!
    //! @return 現在の状態
    //!
    State GetState() const { return m_State; }

    //! @brief 現在有効な状態か否かを取得します。
    //!
    //! @retval true    有効な状態。全てのアクションを受け付ける。
    //! @retval false   無効な状態。offアクションとcancelアクションを受け付ける。
    //!
    bool IsActive() const { return m_Flag.IsAllBitOn(FlagMask_Active); }

    //! @brief 現在有効な状態か否かを設定します。
    //!
    //! @details
    //! ボタンが無効な状態とは、「ONアクション及びDOWNアクションを受け付けない状態」を示します。
    //! つまり、無効な状態でもOFFアクションは受け付けます。これはボタンにカーソルが当たった状態で
    //! 無効になったときに、その後自然にボタンをOFF状態に移行させるためです。
    //!
    //! 一切のアクションを受け付けないようにするには、SetAcceptActionAll()を使用してください。
    //!
    //! また、無効な状態でも、ForceOn()等の強制的な変更は受け付けます。
    //!
    //! @param[in] active   有効にするか否か
    //!
    virtual void SetActive(bool active);

    //! @brief 現在指定したアクションを受け付けるか否かを取得します。
    //!
    //! @details
    //! この設定はIsEnabled/SetEnabledで扱う状態とは独立であることに注意してください。
    //! つまり、ONアクションが受け付けられるには、(SetEnabled() && IsAcceptAction(ACTION_ON))である必要があります。
    //!
    //! デフォルトでは全てのアクションを受け付けます。
    //!
    //! ただしForceOn()等の強制的な変更はこの設定に関係なく受け付けます。
    //!
    //! @param[in] action   受け付けるか調べたいアクション
    //!
    bool IsAcceptAction(Action action) const { return m_Flag.GetBit(action); }

    //! @brief 現在指定したアクションを受け付けるか否かを設定します。
    //!
    //! @details
    //! このメソッドを使用して部分的にアクションを受け付ける設定を変更すると、
    //! ボタンが不自然な挙動を示すことがあります。十分注意してご使用ください。
    //!
    //! @param[in] action   受け付けるかを指定したいアクション
    //! @param[in] accept   受け付けるか否か
    //!
    void SetAcceptAction(Action action, bool accept) { m_Flag.SetBit(action, accept); }

    //! @brief アクションを受け付けるか否かの設定を、全てのアクションに対して一度に設定します。
    //!
    //! @param[in] accept   アクションを受け付けるか否か
    //!
    void SetAcceptActionAll(bool accept)
    {
        const int mask = FlagMask_AcceptOn | FlagMask_AcceptOff | FlagMask_AcceptDown | FlagMask_AcceptCancel;
        m_Flag.SetMaskedBits(mask, accept ? mask : 0);
    }

    //! @brief ボタンのステートがSTATE_DOWNまたはSTATE_DOWN_IDLEであるか、またはアクションキューにDOWNアクションが入っているかを取得します。
    //!
    //! @details
    //! この関数がtrueを返すということは、ForceOff等で強制的にステートが変更されない限り、そのボタンは将来的に押された状態になる
    //! ということを示します。
    //!
    bool IsDowning() const;
    //@}

protected:
    static const int    FlagBitMax = 4;    //!< m_Flagで使用しているビットの最大値

    enum FlagMask {
        FlagMask_AcceptOn = 1 << 0,
        FlagMask_AcceptOff = 1 << 1,
        FlagMask_AcceptDown = 1 << 2,
        FlagMask_AcceptCancel = 1 << 3,
        FlagMask_Active = 1 << 4,
    };

    //! ボタンのアクションを管理するためのキューです。
    class ActionQueue
    {
    public:
        static const int    QueueSize = 4;

        //! コンストラクタです。要素0個で初期化します。
        ActionQueue() : m_ActionCount(0)
        {
            memset(m_Actions, 0, sizeof(m_Actions));
        }

        //! キューを空にします。
        void Clear()
        {
            m_ActionCount = 0;
        }

        //! キューの中にDOWNアクションがあるか調査します。
        bool HasDownAction() const;

        //! キューの先頭の要素を取得します。
        Action Peek() const
        {
            NN_SDK_ASSERT( ! IsEmpty());
            return m_Actions[0];
        }

        //! キューが空か否かを取得します。
        bool IsEmpty() const
        {
            return (m_ActionCount == 0);
        }

        //! キューがいっぱいか否かを取得します。
        bool IsFull() const
        {
            return (m_ActionCount >= QueueSize);
        }

        //! キューの最後尾に要素を追加し、処理しても処理しなくても同じことになるアクションを削除します。
        void PushWithOmit(Action action);

        //! キューの先頭から要素を削除します。
        void Pop();

    private:

        Action m_Actions[QueueSize];
        int m_ActionCount;
    };

    //! フラグを取得します。
    nn::util::BitPack32& GetFlag()
    {
        return m_Flag;
    }

    //! フラグを取得します。
    const nn::util::BitPack32 GetFlag() const
    {
        return m_Flag;
    }

    //! 状態を設定します。
    void SetState(State state)
    {
        m_State = state;
    }

    //! Onアクションに対する処理を記述します。trueを返したときは、処理したアクションをキューからpopすることを示します。
    virtual bool ProcessOn();
    //! Offアクションに対する処理を記述します。trueを返したときは、処理したアクションをキューからpopすることを示します。
    virtual bool ProcessOff();
    //! Downアクションに対する処理を記述します。trueを返したときは、処理したアクションをキューからpopすることを示します。
    virtual bool ProcessDown();
    //! Cancelアクションに対する処理を記述します。trueを返したときは、処理したアクションをキューからpopすることを示します。
    virtual bool ProcessCancel();

    //! STATE_ONの状態を進めます。trueを返したときは、STATE_ON状態を終了し、STATE_ON_IDLEに移ります。
    virtual bool UpdateOn();
    //! STATE_OFFの状態を進めます。trueを返したときは、STATE_OFF状態を終了し、STATE_OFF_IDLEに移ります。
    virtual bool UpdateOff();
    //! STATE_DOWNの状態を進めます。trueを返したときは、STATE_DOWN状態を終了し、STATE_DOWN_IDLEに移ります。
    virtual bool UpdateDown();
    //! STATE_CANCELの状態を進めます。trueを返したときは、STATE_CANCEL状態を終了し、STATE_OFF_IDLEに移ります。
    virtual bool UpdateCancel();

    //! STATE_ONが始まったときに呼び出されるメソッドです。デフォルトでは何も行いません。
    virtual void StartOn();
    //! STATE_OFFが始まったときに呼び出されるメソッドです。デフォルトでは何も行いません。
    virtual void StartOff();
    //! STATE_DOWNが始まったときに呼び出されるメソッドです。デフォルトでは何も行いません。
    virtual void StartDown();
    //! STATE_CANCELが始まったときに呼び出されるメソッドです。デフォルトでは何も行いません。
    virtual void StartCancel();

    //! STATE_ONが終わったときに呼び出されるメソッドです。デフォルトではステートをSTATE_ON_IDLEに移行します。
    virtual void FinishOn();
    //! STATE_OFFが終わったときに呼び出されるメソッドです。デフォルトではステートをSTATE_OFF_IDLEに移行します。
    virtual void FinishOff();
    //! STATE_DOWNが終わったときに呼び出されるメソッドです。デフォルトではステートをSTATE_DOWN_IDLEに移行します。
    virtual void FinishDown();
    //! STATE_CANCELが終わったときに呼び出されるメソッドです。デフォルトではステートをSTATE_OFF_IDLEに移行します。
    virtual void FinishCancel();

    //! ステートを変更します。
    virtual void ChangeState(State state);
    //! ステートを強制的に変更した場合は、ChangeStateではなくこちらが呼ばれます。
    virtual void ForceChangeState(State state);

    //! アクションキューから要素を一つ取出し、処理します。キューにアクションがない場合は何もしません。
    void ProcessActionFromQueue();

private:

    State m_State;
    nn::util::BitPack32 m_Flag;
    ActionQueue m_ActionQueue;
};

} // namespace nn::ui2d
} // namespace nn
