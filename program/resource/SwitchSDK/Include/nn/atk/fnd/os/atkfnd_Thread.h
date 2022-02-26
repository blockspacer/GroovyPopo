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

#include <nn/atk/fnd/basis/atkfnd_Config.h>
#include <nn/atk/fnd/basis/atkfnd_Time.h>

#include <nn/os/os_Thread.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief  スレッドを制御するクラスです。
//---------------------------------------------------------------------------
class Thread
{
public: // 定数の定義
    static const int64_t InvalidId = UINT_MAX;        //!< 無効なIDです。
    static const int DefaultThreadPriority = 16;      //!< プライオリティのデフォルト値です。
    static const int MinThreadPriority = 0;           //!< プライオリティの最小値です。
    static const int MaxThreadPriority = 31;          //!< プライオリティの最大値です。
    static const int StackAlignment = nn::os::ThreadStackAlignment; //!< スタック用メモリのアライメントです。
#if defined(NN_ATK_FND_CONFIG_ENABLE_THREAD_CORE_NUMBER_OBSERVATION)
    static const int InvalidCoreNumber = -1;    //!< 無効なスレッド動作コア番号です。
#endif

public: // 型の定義
    typedef nn::os::ThreadType Handle;

    //! @brief スレッドの fs アクセス優先度です。
    enum FsPriority
    {
        FsPriority_RealTime,
        FsPriority_Normal,
        FsPriority_Low
    };

    //! @brief  スレッドの Affinity Mask です。
    enum AffinityMask
    {
        AffinityMask_CoreDefault = 0,
        AffinityMask_CoreAll = 0xffffffff,

        AffinityMask_Core0  = 1 << 0,
        AffinityMask_Core1  = 1 << 1,
        AffinityMask_Core2  = 1 << 2,
        AffinityMask_Core3  = 1 << 3,
        AffinityMask_Core4  = 1 << 4,
        AffinityMask_Core5  = 1 << 5,
        AffinityMask_Core6  = 1 << 6,
        AffinityMask_Core7  = 1 << 7,
        AffinityMask_Core8  = 1 << 8,
        AffinityMask_Core9  = 1 << 9,
        AffinityMask_Core10 = 1 << 10,
        AffinityMask_Core11 = 1 << 11,
        AffinityMask_Core12 = 1 << 12,
        AffinityMask_Core13 = 1 << 13,
        AffinityMask_Core14 = 1 << 14,
        AffinityMask_Core15 = 1 << 15,
        AffinityMask_Core16 = 1 << 16,
        AffinityMask_Core17 = 1 << 17,
        AffinityMask_Core18 = 1 << 18,
        AffinityMask_Core19 = 1 << 19,
        AffinityMask_Core20 = 1 << 20,
        AffinityMask_Core21 = 1 << 21,
        AffinityMask_Core22 = 1 << 22,
        AffinityMask_Core23 = 1 << 23,
        AffinityMask_Core24 = 1 << 24,
        AffinityMask_Core25 = 1 << 25,
        AffinityMask_Core26 = 1 << 26,
        AffinityMask_Core27 = 1 << 27,
        AffinityMask_Core28 = 1 << 28,
        AffinityMask_Core29 = 1 << 29,
        AffinityMask_Core30 = 1 << 30,
        AffinityMask_Core31 = 0x80000000
    };

    //! @brief  スレッドハンドラです。
    class Handler
    {
    public:
        virtual ~Handler() NN_NOEXCEPT { }

    public:
        virtual uint32_t Run(void* param) NN_NOEXCEPT = 0;
    };

    //! @brief  スレッドハンドラのデリゲートです。
    //!         スレッド処理を指定メソッドに委譲します。
    template<class TTarget>
    class HandlerDelegate : public Handler
    {
    public:
        typedef uint32_t (TTarget::*HandlerPtr)(void* param);

        HandlerDelegate() NN_NOEXCEPT : m_Target(NULL), m_HandlerFunc(NULL) { }

        void Initialize(TTarget* target, HandlerPtr handlerFunc) NN_NOEXCEPT
        {
            NN_SDK_ASSERT_NOT_NULL(target);
            NN_SDK_ASSERT_NOT_NULL(handlerFunc);
            m_Target = target;
            m_HandlerFunc = handlerFunc;
        }

        virtual uint32_t Run(void* param) NN_NOEXCEPT NN_OVERRIDE
        {
            NN_SDK_ASSERT_NOT_NULL(m_Target);
            NN_SDK_ASSERT_NOT_NULL(m_HandlerFunc);
            return (m_Target->*this->m_HandlerFunc)(param);
        }

    private:
        TTarget*   m_Target;
        HandlerPtr m_HandlerFunc;
    };

    //! @brief  スレッド実行のパラメータです。
    struct RunArgs
    {
        //! @brief  コンストラクタです。
        RunArgs() NN_NOEXCEPT;

        //! @brief  パラメータの有効性を調べます。
        //! @return パラメータが有効であれば true を返します。
        bool IsValid() const NN_NOEXCEPT;

        const char*   name;             //!< スレッドの名前です。
        void*         stack;            //!< スレッドのスタックです。
        size_t        stackSize;        //!< スレッドのスタックサイズです。
        int           idealCoreNumber;  //!< スレッドが優先して動作するコアの番号です。
        AffinityMask  affinityMask;     //!< スレッドの Affinity Mask です。
        int           priority;         //!< スレッドのプライオリティです。
        FsPriority    fsPriority;       //!< スレッドの fs アクセス優先度です。
        void*         param;            //!< スレッドに渡すユーザーパラメータです。

        Handler*      handler;          //!< スレッドハンドラです。
    };

    //! @brief  スレッドの状態を示します。
    enum State
    {
        State_NotRun = 0,  //!< スレッドは一度も実行されていません。
        State_Running,      //!< スレッドは実行中です。
        State_Exited,       //!< スレッドは終了済みです。
        State_Released      //!< スレッドのリソースが解放されており再利用可能な状態です。
    };

private:
    class ThreadMain;

public:
    Thread() NN_NOEXCEPT;
    ~Thread() NN_NOEXCEPT;

public:
    //! @brief  スレッドを実行します。
    //! @param[in] args パラメータです。
    //! @return スレッドの実行に成功した場合は true を返します。
    bool Run(const RunArgs& args) NN_NOEXCEPT;

    //! @brief  スレッドが終了するまで待機します。
    void WaitForExit() NN_NOEXCEPT;

    //! @brief  スレッドの所有権を開放し、再度実行可能な状態にします。
    void Release() NN_NOEXCEPT;

    //! @brief  プライオリティを取得します。
    //! @return プライオリティを返します。
    int GetPriority() const NN_NOEXCEPT;

    //! @brief fs アクセス優先度を取得します。
    FsPriority GetFsPriority() const NN_NOEXCEPT;

#if defined(NN_ATK_FND_CONFIG_ENABLE_THREAD_CORE_NUMBER_OBSERVATION)
    //! @briefprivate
    //! @brief  スレッドが動作するコア番号を取得します。
    //!         スレッドが動作していないときは InvalidCoreNumber を返します。
    //! @return スレッドが動作するコア番号を返します。
    int GetCoreNumber() const NN_NOEXCEPT
    {
        return m_CoreNumber;
    }
#endif

    //! @brief  プライオリティを設定します。
    //! @param[in] value プライオリティです。
    void SetPriority(int32_t value) NN_NOEXCEPT;

    //! @brief  スレッドの状態を取得します。
    //! @return スレッドの状態を返します。
    State GetState() const NN_NOEXCEPT;

    //! @brief  カレントスレッドをスリープします。
    //! @param[in] timeSpan スリープから復帰するまでの時間です。
    static void Sleep(const TimeSpan& timeSpan) NN_NOEXCEPT;

private:
    bool Create(Handle& handle, int64_t& id, const RunArgs& args) NN_NOEXCEPT;
    void Detach() NN_NOEXCEPT;
    void SetName(const char* name) NN_NOEXCEPT;
    void SetAffinityMask(int idealCoreNumber, AffinityMask value) NN_NOEXCEPT;
    void SetFsPriority(FsPriority value) NN_NOEXCEPT;
    void Resume() NN_NOEXCEPT;
    void Join() NN_NOEXCEPT;
    bool IsTerminated() const NN_NOEXCEPT;

    void SetState(State value) NN_NOEXCEPT;

    void OnRun() NN_NOEXCEPT;
    void OnExit() NN_NOEXCEPT;

private:
    uint32_t    m_State;        //!< スレッドの状態です。
    Handle      m_Handle;       //!< スレッドハンドルです。
    int64_t     m_Id;           //!< スレッドIDです。
    int         m_Priority;     //!< スレッドのプライオリティです。
    FsPriority  m_FsPriority;   //!< スレッドの fs アクセス優先度です。
#if defined(NN_BUILD_CONFIG_OS_WIN32)
    const char* m_Name;         //!< スレッドの名前です。
#endif
    void*       m_Param;        //!< ユーザーパラメータです。

    Handler*    m_Handler;      //!< スレッドハンドラです。
#if defined(NN_ATK_FND_CONFIG_ENABLE_THREAD_CORE_NUMBER_OBSERVATION)
    int         m_CoreNumber;   //!< スレッドが動作しているコア番号です。
#endif

    volatile bool m_IsTerminated;
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
