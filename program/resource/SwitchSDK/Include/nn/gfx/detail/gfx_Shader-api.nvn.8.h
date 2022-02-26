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

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_ShaderData-api.nvn.8.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

class ShaderInfo;

namespace detail {

template<>
class ShaderImpl< ApiVariationNvn8 >
    : public DataContainer< ShaderImplData< ApiVariationNvn8 > >
{
    NN_DISALLOW_COPY( ShaderImpl );

public:
    typedef ApiVariationNvn8 Target;
    typedef ShaderInfo InfoType;

    static size_t GetBinaryCodeAlignment( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;

    ShaderImpl() NN_NOEXCEPT;

    ~ShaderImpl() NN_NOEXCEPT;

    ShaderInitializeResult Initialize( DeviceImpl< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT;

    void Finalize( DeviceImpl< Target >* pDevice ) NN_NOEXCEPT;

    int GetInterfaceSlot( ShaderStage stage,
        ShaderInterfaceType shaderInterfaceType, const char* pName ) const NN_NOEXCEPT;

    void GetWorkGroupSize( int* pWorkGroupSizeX, int* pWorkGroupSizeY,
        int* pWorkGroupSizeZ ) const NN_NOEXCEPT;
};

}
}
}
