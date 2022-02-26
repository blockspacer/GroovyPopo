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

#include <nn/util/util_BinTypes.h>

#include <nn/gfx/gfx_MemoryPoolInfoData.h>

struct NVNdebugDataHash;

namespace nn {
namespace gfx {

struct NvnDecomposedControlSection
{
    nn::util::BinTPtr< const void > pMetaData;
    nn::util::BinTPtr< const void > pAssemblyData;
    nn::util::BinTPtr< const void > pSpecializationData;
    nn::util::BinTPtr< const void > pPragmaData;
    nn::util::BinTPtr< const void > pAssemblyLocalsData;
    nn::util::BinTPtr< const void > pUniform64InfoData;

    char reserved[ 16 ];
};

struct NvnShaderCode
{
    uint64_t dataAddress;
    nn::util::BinTPtr< const void > pControl;

    nn::util::BinTPtr< const void > pData;
    uint32_t dataSize;
    uint32_t controlSize;

    nn::util::BinTPtr< const NvnDecomposedControlSection > pDecomposedControlSection;
    uint32_t scratchMemoryRecommended;
    uint32_t scratchMemoryPerWarp;

    nn::util::BinTPtr< const NVNdebugDataHash > pNvnDebugDataHash;

    char reserved[ 8 ];
};

struct NvnShaderPool
{
    nn::gfx::MemoryPoolInfoData memoryPoolInfo;
    nn::util::BinTPtr< void > pMemoryPool;
    char reserved2[ 8 ]; //!< 予約領域です。

    nn::util::BinTPtr< void > pCurrentMemoryPool;

    char reserved[ 24 ];
};

}
}
