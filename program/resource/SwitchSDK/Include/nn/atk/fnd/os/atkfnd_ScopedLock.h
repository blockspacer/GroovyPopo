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

namespace nn { namespace atk { namespace detail { namespace fnd {

    //---------------------------------------------------------------------------
    //! @internal
    //! @brief        スコープ内でのみ、ロックをおこなうクラスです。
    //---------------------------------------------------------------------------
    template<typename TLockObject>
    class ScopedLock
    {
    private:
        NN_DISALLOW_COPY(ScopedLock);

    public:
        //---------------------------------------------------------------------------
        //! @brief        コンストラクタでロックを行います。
        //!
        //! @param[out]   lockObj 使用するロックオブジェクトです。
        //---------------------------------------------------------------------------
        explicit ScopedLock(TLockObject& lockObj) NN_NOEXCEPT
        : m_LockObj(lockObj)
        {
            m_LockObj.Lock();
        }

        //---------------------------------------------------------------------------
        //! @brief        デストラクタでアンロックをおこないます。
        //---------------------------------------------------------------------------
        ~ScopedLock() NN_NOEXCEPT
        {
            m_LockObj.Unlock();
        }

    private:
        TLockObject& m_LockObj;
    };

}}}} // namespace nn::atk::detail::fnd
