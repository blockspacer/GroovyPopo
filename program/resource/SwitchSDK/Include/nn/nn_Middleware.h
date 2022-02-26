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

#include <nn/nn_Common.h>
#include <nn/util/util_Symbol.h>

#if defined(NN_BUILD_CONFIG_OS_HORIZON)
    #define NN_MAKE_MIDDLEWARE_STRING( vender, module ) \
        "SDK MW+" vender "+" module

    #define NN_MAKE_DEBUG_API_STRING( vender, module ) \
        "SDK Debug+" vender "+" module

    #define NN_MAKE_PRIVATE_API_STRING( vender, module ) \
        "SDK Private+" vender "+" module

    #define NN_DEFINE_MIDDLEWARE( name, vender, module ) \
        NN_UTIL_DEFINE_API_INFO_STRING( name, NN_MAKE_MIDDLEWARE_STRING( vender, module ))

    #define NN_DEFINE_DEBUG_API( name, vender, module ) \
        NN_UTIL_DEFINE_API_INFO_STRING( name, NN_MAKE_DEBUG_API_STRING( vender, module ))

    #define NN_DEFINE_PRIVATE_API( name, vender, module ) \
        NN_UTIL_DEFINE_API_INFO_STRING( name, NN_MAKE_PRIVATE_API_STRING( vender, module ))

    #define NN_USING_MIDDLEWARE( name ) \
        NN_UTIL_REFER_SYMBOL( name )

    #define NN_USING_DEBUG_API( name ) \
        NN_UTIL_REFER_SYMBOL( name )

    #define NN_USING_PRIVATE_API( name ) \
        NN_UTIL_REFER_SYMBOL( name )
#else
    #define NN_DEFINE_MIDDLEWARE( name, vender, module )
    #define NN_DEFINE_DEBUG_API( name, vender, module )
    #define NN_DEFINE_PRIVATE_API( name, vender, module )
    #define NN_USING_MIDDLEWARE( name )
    #define NN_USING_DEBUG_API( name )
    #define NN_USING_PRIVATE_API( name )
#endif
