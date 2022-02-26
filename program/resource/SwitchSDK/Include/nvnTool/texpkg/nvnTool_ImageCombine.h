/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_ImageCombine.h
///       This module is for stitching images into array images, and
///       assembling images into mipmap chains used by the NVN texture
///       packager tool.

#ifndef NVNTOOL_IMAGECOMBINE_H
#define NVNTOOL_IMAGECOMBINE_H

#include <nvn/nvn.h>
#include "nvnTool_TexpkgCommon.h"
#include "nvnTool_Image.h"
#include "nvnTool_ImageConvert.h"

namespace nvnTool {
namespace texpkg {

/// Class for assembling mipmaps or stitching images into array.
class RawImageCombine {
public:
    virtual ~RawImageCombine() {}

    /// \brief Assemble a series of images into a single mipmap chain. The images must be given in order
    /// (level0, level1, level 2...), and must be sized correctly.
    ///
    /// \param[in] src             The array of input images to assemble into mip chain.
    /// \param[in] numSourceImages Number of images in the src[] array; also indicates the number of
    ///                            mip levels in the resulting image.
    /// \param[out] dst            Destination output mipmapped image.
    ///
    /// \return TP_OK if the operation was successful or an error code otherwise.
    ///
    virtual TPError AssembleMipmap(RawImage **src, uint32_t numSourceImages, RawImage *dst) = 0;

    /// \brief Stitch a series of images into a single image array.
    ///
    /// \param[in] src             The array of input images to stitch into image array.
    /// \param[in] numSourceImages Number of images in the src[] array; also indicates the number of
    ///                            array layers in the resulting image.
    /// \param[out] dst            Destination output array image.
    ///
    /// \return TP_OK if the operation was successful or an error code otherwise.
    ///
    virtual TPError StitchArray(RawImage **src, uint32_t numSourceImages, RawImage *dst) = 0;
};

} // namespace texpkg
} // namespace nvnTool

extern "C" {

    /// \brief Create a RawImageCombine object, to assemble or stitch images.
    ///
    /// \return Created RawImageCombine object. This must be released using texpkgReleaseRawImageCombine().
    /// \see texpkgReleaseRawImageCombine.
    ///
    DLL_API nvnTool::texpkg::RawImageCombine* texpkgCreateRawImageCombine(void);

    TP_DLL_EXPORTED_FUNC(
        nvnTool::texpkg::RawImageCombine*,
        texpkgCreateRawImageCombine,
        (void),
        CREATE_RAWIMAGE_COMBINE_FUNC
    );

    /// \brief Release a RawImageCombine object.
    ///
    /// \param[in] ric       RawImageCombine object created by texpkgCreateRawImageCombine().
    /// \see texpkgCreateRawImageCombine
    ///
    DLL_API void texpkgReleaseRawImageCombine(nvnTool::texpkg::RawImageCombine* ric);

    TP_DLL_EXPORTED_FUNC(void,
        texpkgReleaseRawImageCombine,
        (nvnTool::texpkg::RawImageCombine*),
        RELEASE_RAWIMAGE_COMBINE_FUNC
    );

} // extern "C"

#endif // NVNTOOL_IMAGECOMBINE_H
