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
#include <nn/nn_Macro.h>
#include <nn/nn_SdkAssert.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_RootSignatureData-api.common.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_DEPRECATED_DECLARATIONS

namespace nn {
namespace gfx {

class RootSignatureInfo;

namespace detail {

template< typename TTarget >
class RootSignatureImplCommon
    : public DataContainer< RootSignatureImplData< ApiType< LowLevelApi_Common > > >
{
public:
    typedef TTarget Target;
    typedef RootSignatureInfo InfoType;
    typedef RootSignatureImplCommon< Target > MemoryImplClass;

    enum RequiredMemoryInfo
    {
        RequiredMemoryInfo_Alignment = 1
    };

    static size_t GetRequiredMemorySize( const InfoType& info ) NN_NOEXCEPT;

    RootSignatureImplCommon() NN_NOEXCEPT;

    ~RootSignatureImplCommon() NN_NOEXCEPT;

    void SetMemory( void* pMemory, size_t size ) NN_NOEXCEPT;

    void* GetMemory() NN_NOEXCEPT;
    const void* GetMemory() const NN_NOEXCEPT;

    void Initialize( DeviceImpl< TTarget >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< TTarget >* pDevice ) NN_NOEXCEPT;
};

}
}
}

NN_PRAGMA_POP_WARNINGS
