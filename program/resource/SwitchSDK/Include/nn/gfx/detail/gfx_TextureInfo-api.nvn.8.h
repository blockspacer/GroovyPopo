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
#include <nn/gfx/gfx_TextureInfoData-api.nvn.8.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

class TextureInfo;

namespace detail {

template<>
class TextureInfoImpl< ApiVariationNvn8 >
    : public DataContainer< TextureInfoImplData< ApiVariationNvn8 > >
{
public:
    typedef ApiVariationNvn8 Target;

    static void ConvertFromInfo( TextureInfoImpl< Target >* pDstInfo, const TextureInfo& info ) NN_NOEXCEPT;

    TextureInfoImpl() NN_NOEXCEPT;

    void SetDefault() NN_NOEXCEPT;

    void SetImageStorageDimension( ImageStorageDimension value ) NN_NOEXCEPT;

    void SetImageFormat( ImageFormat value ) NN_NOEXCEPT;

    void SetGpuAccessFlags( int value ) NN_NOEXCEPT;

    void SetTileMode( TileMode value ) NN_NOEXCEPT;

    void SetWidth( int value ) NN_NOEXCEPT;

    void SetHeight( int value ) NN_NOEXCEPT;

    void SetDepth( int value ) NN_NOEXCEPT;

    void SetArrayLength( int value ) NN_NOEXCEPT;

    void SetSwizzle( int value ) NN_NOEXCEPT;

    void SetMultiSampleCount( int value ) NN_NOEXCEPT;

    void SetMipCount( int value ) NN_NOEXCEPT;
};

}
}
}
