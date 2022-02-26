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

#include <nn/gfx/gfx_DataAccessorConverter.h>

namespace nn {
namespace gfx {
namespace detail {

template< typename TTarget, template< typename > class TObj >
void SetUserPtrImpl( TObj< TTarget >* pObj, void* userPtr )
{
    typename TObj< TTarget >::DataType* pData = AccessorToData( pObj );
    pData->userPtr = userPtr;
}

template< typename TTarget, template< typename > class TObj >
void* GetUserPtrImpl( TObj< TTarget >* pObj )
{
    typename TObj< TTarget >::DataType* pData = AccessorToData( pObj );
    return pData->userPtr;
}

template< typename TTarget, template< typename > class TObj >
const void* GetUserPtrImpl( const TObj< TTarget >* pObj )
{
    const typename TObj< TTarget >::DataType* pData = AccessorToData( pObj );
    return pData->userPtr;
}

}
}
}
