/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_ImageMipmap.h
///        Definitions used for generating mipmap images in the NVN texture
///        packager tool.

#ifndef NVNTOOL_IMAGEMIPMAP_H
#define NVNTOOL_IMAGEMIPMAP_H

#include <nvn/nvn.h>
#include "nvnTool_TexpkgCommon.h"
#include "nvnTool_DataTypes.h"
#include "nvnTool_Image.h"
#include "nvnTool_ImageConvert.h"

namespace nvnTool {
namespace texpkg {

enum MipFilter {
    MIPFILTER_INVALID = -1,
    MIPFILTER_BOX = 0
};

/// \brief This module is responsible for mipmap generation. Currently, only box filtering
/// is supported.
///
class RawImageMipmapGenerator {
public:
    virtual ~RawImageMipmapGenerator() {}

    /// \brief Generate mipmap for N levels, starting from baseLevel = 0.
    ///
    /// \param[in] src         Source image with baseLevel 0 defined to generate
    ///                        mipmaps from.
    /// \param[out] dst        Destination image to store finished image. baseLevel 0 will be
    ///                        bitwise-copied. Alignment / padding will be exactly the same
    ///                        as the source image.
    /// \param[in] numLevels   Number of mipmap levels to generate.
    ///
    /// \return TP_OK if successful, TP_UNSUPPORTED_FORMAT if source format is not supported,
    ///         TP_ERROR_UNKNOWN if src or dst is NULL, TP_INVALID_ARGUMENT_FORMAT if filter
    ///         is unsupported, other < 0 error code otherwise.
    ///
    virtual TPError GenerateMipmaps(RawImage *src, RawImage *dst, uint32_t numLevels) = 0;

    /// \brief Generate mipmap for until all dimensions are below minSize x minSize x minSize,
    /// stating from baseLevel = 0. A minSize = 1 would generate until 1 x 1 x 1.
    ///
    /// \param[in] src         Source image (baseLevel 0 must be defined).
    /// \param[out] dst        Destination image to store finished image. baseLevel 0 will be
    ///                        bitwise-copied. Alignment / padding will be exactly the same
    ///                        as the source image.
    /// \param[in] minSize     Generate texture mipmaps until size is < minSize.
    ///
    /// \return TP_OK if successful, TP_UNSUPPORTED_FORMAT if source format is not supported,
    ///         TP_ERROR_UNKNOWN if src or dst is NULL, TP_INVALID_ARGUMENT_FORMAT if filter
    ///         is unsupported, other < 0 error code otherwise.
    ///
    virtual TPError GenerateMipmapsUntilSize(RawImage *src, RawImage *dst, uint32_t minSize) = 0;
};

} // namespace texpkg
} // namespace nvnTool

extern "C" {

    /// \brief Create a RawImageMipmapGenerator object.
    ///
    /// \param[in] filter       Which filter to use. Must be MIPFILTER_BOX.
    /// \param[in] converter    Reference to a image converter object.
    /// \return Created RawImageMipmapGenerator object.
    ///         This must be released using texpkgReleaseRawImageMipmapGenerator().
    /// \see texpkgReleaseRawImageMipmapGenerator.
    ///
    DLL_API nvnTool::texpkg::RawImageMipmapGenerator* texpkgCreateRawImageMipmapGenerator(
                                                    nvnTool::texpkg::MipFilter filter,
                                                    nvnTool::texpkg::RawImageConverter *converter);

    TP_DLL_EXPORTED_FUNC(
        nvnTool::texpkg::RawImageMipmapGenerator*,
        texpkgCreateRawImageMipmapGenerator,
        (nvnTool::texpkg::MipFilter, nvnTool::texpkg::RawImageConverter *),
        CREATE_RAWIMAGE_MIPMAP_GENERATOR_FUNC
    );

    /// \brief Release a RawImageMipmapGenerator object.
    /// \param[in] rimg       RawImageMipmapGenerator object created by texpkgCreateRawImageMipmapGenerator().
    /// \see texpkgCreateRawImageMipmapGenerator.
    ///
    DLL_API void texpkgReleaseRawImageMipmapGenerator(nvnTool::texpkg::RawImageMipmapGenerator* rimg);

    TP_DLL_EXPORTED_FUNC(
        void,
        texpkgReleaseRawImageMipmapGenerator,
        (nvnTool::texpkg::RawImageMipmapGenerator*),
        RELEASE_RAWIMAGE_MIPMAP_GENERATOR_FUNC
    );

} // extern "C"

#endif // NVNTOOL_IMAGEMIPMAP_H
