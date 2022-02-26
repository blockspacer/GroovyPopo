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
#include <nn/gfx/gfx_SamplerInfoData-api.nvn.8.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

class SamplerInfo;

namespace detail {

template<>
class SamplerInfoImpl< ApiVariationNvn8 >
    : public DataContainer< SamplerInfoImplData< ApiVariationNvn8 > >
{
public:
    typedef ApiVariationNvn8 Target;

    static void ConvertFromInfo( SamplerInfoImpl< Target >* pDstInfo, const SamplerInfo& info ) NN_NOEXCEPT;

    SamplerInfoImpl() NN_NOEXCEPT;

    void SetDefault() NN_NOEXCEPT;

    void SetFilterMode( FilterMode value ) NN_NOEXCEPT;

    void SetAddressU( TextureAddressMode value ) NN_NOEXCEPT;

    void SetAddressV( TextureAddressMode value ) NN_NOEXCEPT;

    void SetAddressW( TextureAddressMode value ) NN_NOEXCEPT;

    void SetComparisonFunction( ComparisonFunction value ) NN_NOEXCEPT;

    void SetBorderColorType( TextureBorderColorType value ) NN_NOEXCEPT;

    void SetMaxAnisotropy( int value ) NN_NOEXCEPT;

    void SetMinLod( float value ) NN_NOEXCEPT;

    void SetMaxLod( float value ) NN_NOEXCEPT;

    void SetLodBias( float value ) NN_NOEXCEPT;
};

}
}
}
