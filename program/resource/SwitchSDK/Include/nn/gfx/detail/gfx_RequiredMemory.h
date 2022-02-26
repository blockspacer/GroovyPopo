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

#include <nn/gfx/detail/gfx_Misc.h>

namespace nn {
namespace gfx {
namespace detail {

template< typename TImpl >
class RequiredMemory
{
private:
    NN_GFX_OPTION( RequiredMemoryInfo );
    NN_GFX_OPTION_MEMBER_FUNCTION( void, SetMemory, , void*, size_t );
    NN_GFX_OPTION_MEMBER_FUNCTION( void*, GetMemory, );

protected:
    enum RequiredMemoryInfo
    {
        RequiredMemoryInfo_Alignment = 1
    };

    static size_t GetRequiredMemorySize( ... ) NN_NOEXCEPT
    {
        return 0;
    }

    static void SetMemory( void*, size_t ) NN_NOEXCEPT
    {
    }

    static void* GetMemory() NN_NOEXCEPT
    {
        return NULL;
    }

    typedef typename Conditional< NN_GFX_OPTION_EXISTS( TImpl, RequiredMemoryInfo ),
        TImpl, RequiredMemory >::Type MemoryInfoImpl;
    typedef typename Conditional< NN_GFX_OPTION_EXISTS( TImpl, SetMemory ),
        TImpl, RequiredMemory >::Type MemoryFuncImpl;
};

}
}
}
