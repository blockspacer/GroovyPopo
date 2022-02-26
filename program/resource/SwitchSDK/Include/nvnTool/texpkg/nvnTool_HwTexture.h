/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_HwTexture.h
///       Definitions of common structures used when loading and saving images
///       in the hardware formats supported by the NVN texture packager tool.

#ifndef NVNTOOL_HWTEXTURE_H
#define NVNTOOL_HWTEXTURE_H

#include <stdint.h>

#define TEXPKG_MAX_MIPMAP_LEVEL               16

namespace nvnTool {
namespace texpkg {

#if defined(_MSC_VER) || defined(__clang__) || defined(__GNUC__)
    #pragma pack(push, 4)
#else
    #error Add logic to control structure packing for an unsupported compiler.
#endif

/// Texture Builder information for an NVN hardware format texture.
struct NVNHWTextureHeader {
    uint64_t dataSize;
    uint32_t align;

    uint32_t width;
    uint32_t height;
    uint32_t depth;

    uint32_t target;
    uint32_t format;
    uint32_t mipmapLevels;

    /// \brief Number of bytes between each sub-image. For 2D & 3D textures this will be the size of the
    /// entire image. For array, cubemap and cubemap array textures, this will be the size of
    /// one image layer (for these targets, the image is stored per-layer and then per mip).
    uint32_t subImagePitch;

    /// \brief Number of bytes from the beginning of an image layer (ie. subimage) at which the
    /// mipmap level starts.
    uint32_t mipmapLevelOffset[TEXPKG_MAX_MIPMAP_LEVEL + 1];

    /// \brief Stores texture layout data.
    NVNpackagedTextureLayout layout;

    /// \brief Whether the texture was stored in sparse format. This should be the same as the sparseTiled
    /// flag passed into RawImageHardwareTextureExporter::Convert().
    NVNboolean sparse;
};

/// An NVN hardware format texture.
struct NVNHWTexture {
    NVNHWTextureHeader header;
    uint8_t* data;
};

#if defined(_MSC_VER) || defined(__clang__) || defined(__GNUC__)
    #pragma pack(pop)
#endif

} // namespace texpkg
} // namespace nvnTool

#endif // NVNTOOL_HWTEXTURE_H
