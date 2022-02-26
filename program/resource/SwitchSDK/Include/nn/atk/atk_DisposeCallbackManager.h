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
#include <nn/atk/atk_DisposeCallback.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

class DisposeCallbackManager
{
    /* ------------------------------------------------------------------------
            typename definition
       ------------------------------------------------------------------------ */
public:
    typedef util::IntrusiveList<DisposeCallback, util::IntrusiveListMemberNodeTraits<DisposeCallback,&DisposeCallback::m_DisposeLink>> CallbackList;

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    static NN_NOINLINE DisposeCallbackManager& GetInstance() NN_NOEXCEPT;

    void Dispose( const void* mem, size_t size ) NN_NOEXCEPT;

    void RegisterDisposeCallback( DisposeCallback* callback ) NN_NOEXCEPT;
    void UnregisterDisposeCallback( DisposeCallback* callback ) NN_NOEXCEPT;

    unsigned long GetCallbackCount() const NN_NOEXCEPT;

private:
    DisposeCallbackManager() NN_NOEXCEPT;

    CallbackList m_CallbackList;
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

