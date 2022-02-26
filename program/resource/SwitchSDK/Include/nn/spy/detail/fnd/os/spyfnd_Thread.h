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

#include <atomic>

#include <nn/spy/detail/fnd/basis/spyfnd_Config.h>
#include <nn/spy/detail/fnd/basis/spyfnd_Time.h>

#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
#include <cafe/os/OSThread.h>
#else // NN_BUILD_CONFIG_SPEC_GENERIC
#include <nn/os/os_Thread.h>
#endif

namespace nn {
namespace spy {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @brief  スレッドを制御するクラスです。
//---------------------------------------------------------------------------
class Thread
{
public: // 定数の定義
    static const int DefaultThreadPriority = 16;      //!< プライオリティのデフォルト値です。
    static const int MinThreadPriority = 0;           //!< プライオリティの最小値です。
    static const int MaxThreadPriority = 31;          //!< プライオリティの最大値です。

private: // 定数の定義
#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
    static const int OsThreadAlignment = 8;            //!< OSThread 構造体のアライメントです。
#endif

public: // 型の定義
#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
    typedef OSThread* Handle;
#else // NN_BUILD_CONFIG_SPEC_GENERIC
    typedef nn::os::ThreadType Handle;
#endif

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
        virtual void Run(void* param) NN_NOEXCEPT = 0;
    };

    //! @brief  スレッドハンドラのデリゲートです。
    //!         スレッド処理を指定メソッドに委譲します。
    template<class TTarget>
    class HandlerDelegate : public Handler
    {
    public:
        typedef void (TTarget::*HandlerPtr)(void* param);

        HandlerDelegate() NN_NOEXCEPT : m_Target(NULL), m_HandlerFunc(NULL) { }

        //---------------------------------------------------------------------------
        //! @brief         初期化します。
        //!
        //! @param[in]     target       handlerFunc を所有するインスタンスを指定します。
        //! @param[in]     handlerFunc  &TTarget::HandlerFunc のように書きます。
        //---------------------------------------------------------------------------
        void Initialize(TTarget* target, HandlerPtr handlerFunc) NN_NOEXCEPT
        {
            NN_SDK_ASSERT_NOT_NULL(target);
            NN_SDK_ASSERT_NOT_NULL(handlerFunc);
            m_Target = target;
            m_HandlerFunc = handlerFunc;
        }

        virtual void Run(void* param) NN_NOEXCEPT NN_OVERRIDE
        {
            NN_SDK_ASSERT_NOT_NULL(m_Target);
            NN_SDK_ASSERT_NOT_NULL(m_HandlerFunc);
            (m_Target->*this->m_HandlerFunc)(param);
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
        //! @return パラメータが有効なら true を返します。
        bool IsValid() const NN_NOEXCEPT;

        const char*   name;             //!< スレッドの名前です。
        void*         stack;            //!< スレッドのスタックです。
        size_t        stackSize;        //!< スレッドのスタックサイズです。
        AffinityMask  affinityMask;     //!< スレッドの Affinity Mask です。
        int           priority;         //!< スレッドのプライオリティです。
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

    //! @brief  プライオリティを設定します。
    //! @param[in] value プライオリティです。
    void SetPriority(int value) NN_NOEXCEPT;

    //! @brief  スレッドの状態を取得します。
    //! @return スレッドの状態を返します。
    State GetState() const NN_NOEXCEPT;

    //! @brief  カレントスレッドをスリープします。
    //! @param[in] timeSpan スリープから復帰するまでの時間です。
    static void Sleep(const TimeSpan& timeSpan) NN_NOEXCEPT;

private:
    bool Create(Handle& handle, const RunArgs& args) NN_NOEXCEPT;
    void Detach() NN_NOEXCEPT;
    void SetName(const char* name) NN_NOEXCEPT;
    void SetAffinityMask(AffinityMask value) NN_NOEXCEPT;
    void Resume() NN_NOEXCEPT;
    void Join() NN_NOEXCEPT;
    bool IsTerminated() const NN_NOEXCEPT;

    void SetState(State value) NN_NOEXCEPT;

    void OnRun() NN_NOEXCEPT;
    void OnExit() NN_NOEXCEPT;

private:
    State       m_State;        //!< スレッドの状態です。
    Handle      m_Handle;       //!< スレッドハンドルです。
    int         m_Priority;     //!< スレッドのプライオリティです。
    void*       m_Param;        //!< ユーザーパラメータです。

    Handler*    m_Handler;      //!< スレッドハンドラです。

    std::atomic<bool> m_IsTerminated;

#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
    //! @brief  アライメントのためのスレッドハンドルバッファです。
    uint8_t m_HandleBuffer[sizeof(OSThread) + OsThreadAlignment];
#endif
};

} // namespace nn::spy::detail::fnd
} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn
