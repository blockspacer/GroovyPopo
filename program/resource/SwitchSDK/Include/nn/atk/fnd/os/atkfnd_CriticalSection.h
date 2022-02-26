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

#include <nn/os/os_Mutex.h>

#include <nn/atk/fnd/basis/atkfnd_Config.h>

namespace nn { namespace atk { namespace detail { namespace fnd {

    //---------------------------------------------------------------------------
    //! @internal
    //! @brief    クリティカルセクション用の実装です。内部では Mutex を使用しています。
    //---------------------------------------------------------------------------
    class CriticalSection
    {
    public:
        //---------------------------------------------------------------------------
        //! @brief    コンストラクタです。
        //---------------------------------------------------------------------------
        CriticalSection() NN_NOEXCEPT
        : m_Mutex(true)
        {
        }

        //---------------------------------------------------------------------------
        //! @brief    デストラクタです。
        //---------------------------------------------------------------------------
        ~CriticalSection() NN_NOEXCEPT
        {
        }

        //! @brief    この関数は廃止予定です。代わりに @ref Lock を使用してください。
        NN_DEPRECATED void Enter() NN_NOEXCEPT
        {
            this->Lock();
        }

        //! @brief    この関数は廃止予定です。代わりに @ref TryLock を使用してください。
        NN_DEPRECATED bool TryEnter() NN_NOEXCEPT
        {
            return this->TryLock();
        }

        //! @brief    この関数は廃止予定です。代わりに @ref Unlock を使用してください。
        NN_DEPRECATED void Leave() NN_NOEXCEPT
        {
            this->Unlock();
        }

        void Lock() NN_NOEXCEPT
        {
            m_Mutex.Lock();
        }

        bool TryLock() NN_NOEXCEPT
        {
            return m_Mutex.TryLock();
        }

        void Unlock() NN_NOEXCEPT
        {
            m_Mutex.Unlock();
        }

    private:
        static const uint32_t CriticalSectionSize = 24;
        static const uint32_t CriticalSectionAlign = 4;

    private:
        nn::os::Mutex m_Mutex;
    };

}}}} // namespace nn::atk::detail::fnd
