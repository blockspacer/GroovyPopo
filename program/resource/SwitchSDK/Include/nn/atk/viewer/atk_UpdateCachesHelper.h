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

#include <nn/atk/atk_Config.h>
#ifdef NN_ATK_CONFIG_ENABLE_DEV

#include <nn/atk/fnd/os/atkfnd_Thread.h>
#include <nn/atk/viewer/atk_Result.h>
#include <nn/atk/viewer/atk_SoundArchiveEditor.h>

namespace nn {
namespace atk {
namespace viewer {

//---------------------------------------------------------------------------
//! @brief  【β版】キャッシュ更新を別スレッドで実行するためのヘルパークラスです。
//!
//!         このクラスを利用すると、SoundArchiveEditor のキャッシュ更新を
//!         簡単に別スレッドで処理させることができます。
//---------------------------------------------------------------------------
class UpdateCachesHelper
{
    NN_DISALLOW_COPY(UpdateCachesHelper);

public: // 定数の定義
    static const size_t DefaultThreadStackSize = 16 * 1024; //!< スレッドスタックサイズのデフォルト値です。

public: // 型の定義
    //! @brief  スレッドのアフィニティマスクです。
    enum AffinityMask
    {
        AffinityMask_CoreDefault = atk::detail::fnd::Thread::AffinityMask_CoreDefault,   //!< コア指定を行いません。

        AffinityMask_Core0  = atk::detail::fnd::Thread::AffinityMask_Core0,              //!< コア0 を使用します。
        AffinityMask_Core1  = atk::detail::fnd::Thread::AffinityMask_Core1,              //!< コア1 を使用します。
        AffinityMask_Core2  = atk::detail::fnd::Thread::AffinityMask_Core2               //!< コア2 を使用します。
    };

public: // コンストラクタ
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //---------------------------------------------------------------------------
    UpdateCachesHelper() NN_NOEXCEPT;

public: // メソッド
    //---------------------------------------------------------------------------
    //! @brief      ワーカースレッドを新規作成し、キャッシュ更新を開始します。
    //!
    //! @param[in]  soundArchiveEditor  更新対象の SoundArchiveEditor を指定します。
    //! @param[in]  threadStack         スレッドのスタックを指定します。
    //! @param[in]  threadStackSize     スレッドのスタックサイズを指定します。
    //! @param[in]  affinityMask        スレッドのアフィニティマスクを指定します。
    //! @param[in]  priority            スレッドのプライオリティを指定します。
    //! @param[in]  idealCoreNumber     スレッドが優先して動作するコアの番号を指定します。
    //!
    //! @return     処理結果を返します。
    //!
    //! @see        SoundArchiveEditor
    //! @see        Finalize()
    //! @see        IsInitialized()
    //! @see        DefaultThreadStackSize
    //---------------------------------------------------------------------------
    Result Start(
        SoundArchiveEditor* soundArchiveEditor,
        void* threadStack,
        size_t threadStackSize,
        AffinityMask affinityMask = AffinityMask_CoreDefault,
        uint32_t priority = atk::detail::fnd::Thread::DefaultThreadPriority,
        int idealCoreNumber = 0) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ワーカースレッドを停止し、キャッシュ更新も停止します。
    //---------------------------------------------------------------------------
    void Stop() NN_NOEXCEPT;

#if defined(NN_ATK_CONFIG_ENABLE_THREAD_CORE_NUMBER_OBSERVATION)
    //---------------------------------------------------------------------------
    //! @brief  ワーカースレッドの動作コア番号を取得します。
    //---------------------------------------------------------------------------
    int GetThreadCoreNumber() const NN_NOEXCEPT;
#endif

private: // メソッド
    uint32_t ThreadMain(void* parameter) NN_NOEXCEPT;

private: // メンバ変数
    SoundArchiveEditor* m_SoundArchiveEditor;
    Canceller           m_UpdateCachesCanceller;

    atk::detail::fnd::Thread m_Thread;
    bool                       m_IsExitThread;

    atk::detail::fnd::Thread::HandlerDelegate<UpdateCachesHelper> m_ThreadMainDelegate;
};

} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
