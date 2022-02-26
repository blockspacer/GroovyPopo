/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_TexpkgCore.h
///       Helper class to wrap higher level functionality in the NVN texture
///       packager tool.

#ifndef NVNUTIL_TEXPKGCORE_H
#define NVNUTIL_TEXPKGCORE_H

#include <nvn/nvn.h>
#include <string.h>
#include <vector>
#include "nvnTool_HwTexture.h"
#include "nvnTool_TexpkgCommon.h"
#include "nvnTool_ImageDll.h"

namespace nvnTool {
namespace texpkg {

class RawImageFormatManager;
class RawImageHardwareTextureExporter;
struct HWTextureStorage;

/// Helper class providing a high level API to texpkg core functionality.
class TexturePackagerCore {
public:
    virtual ~TexturePackagerCore() {}

    /// \brief Load images from files on disk into pitch-linear RawImages.
    ///
    /// \param[in] formatManager  Format loader & saver.
    /// \param[in] fileNames      Array of numImages filename strings.
    /// \param[in] numImages      Number of images.
    /// \param[out] dstImages     Array of pointers to numImages destination RawImages.
    ///
    /// \return TP_OK if the operation was successful or an error code otherwise.
    ///
    virtual TPError LoadImagesFromFile (
        RawImageFormatManager* formatManager,
        std::wstring *fileNames, int numImages, RawImage **dstImages) = 0;

    /// \brief Process images.
    ///
    /// \param[in] imglib             NVN image library entry points.
    /// \param[in] converter          Format converter.
    /// \param[in] mipmapGenerator    Mipmap generator.
    /// \param[in] combiner           Image stitcher / combiner.
    /// \param[in] srcImages          Array of numImages pointers to source images.
    /// \param[out] dstImages         Array of pointers to destination images.
    ///                               Each pointer either points to an allocated destination image, or
    ///                               points directly to a given source image.
    ///                               Take care not to double-free images on the latter case.
    /// \param[in] numImages          Number of source & destination images to process.
    /// \param[in] mipFilter          Which mipfilter to use. To disable mipmap generation, set this to
    ///                               MIPFILTER_INVALID.
    /// \param[in] minMipSize         Generate mipmaps until N x N where N = minMipSize.
    /// \param[in] srcFormat          Reinterpret source images as this format. BPP must match between
    ///                               the real image format and the reinterpreted format.
    ///                               Set to NVN_FORMAT_NONE to disable.
    /// \param[in] dstFormat          Convert input images into this format.
    ///                               Set to NVN_FORMAT_NONE to disable conversion.
    /// \param[in] dither             Whether to enable dithering when converting.
    /// \param[in] stitchTextureArray Stitch all input textures into a single 2D texture array.
    /// \param[in] assembleMipmaps    Combine all input textures into a single image with mipmap chain.
    ///
    /// \return TP_OK if the operation was successful or an error code otherwise.
    ///
    virtual TPError ProcessImages (
        RawImageLib* imglib,
        RawImageConverter *converter,
        RawImageMipmapGenerator *mipmapGenerator,
        RawImageCombine *combiner,
        RawImage **srcImages, RawImage **dstImages, int numImages,
        MipFilter mipFilter, uint32_t minMipSize,
        NVNformat srcFormat, NVNformat dstFormat,
        bool dither,
        bool stitchTextureArray, bool assembleMipmaps) = 0;

    /// \brief Convert pitch-linear RawImages to hardware dependent texture format.
    ///
    /// \param[out] exporter       HW texture exporter. The converted hardware textures will be stored
    ///                            and managed by this exporter.
    /// \param[in] srcImages       Array of numImages source RawImages.
    /// \param[in] numImages       Number of images.
    /// \param[out] destTextureList Destination texture storage list.
    /// \param[in] storageFlags    Storage flags (see RawImageHardwareTextureFlagsEnum).
    ///
    /// \return TP_OK if the operation was successful or an error code otherwise.
    ///
    virtual TPError ConvertImagesToTexture (
        RawImageHardwareTextureExporter *exporter,
        RawImage **srcImages, int numImages,
        std::vector< NVNHWTexture >& destTextureList,
        uint32_t storageFlags) = 0;
};

} // namespace texpkg
} // namespace nvnTool

extern "C" {

    /// \brief Create a TexturePackagerCore object.
    ///
    /// \return Created TexturePackagerCore object.
    ///         This must be released using texpkgReleaseTexturePackagerCore().
    /// \see texpkgReleaseTexturePackagerCore.
    ///
    DLL_API nvnTool::texpkg::TexturePackagerCore* texpkgCreateTexturePackagerCore(void);

    TP_DLL_EXPORTED_FUNC(
        nvnTool::texpkg::TexturePackagerCore*,
        texpkgCreateTexturePackagerCore,
        (void),
        CREATE_TEXPKG_CORE_FUNC
    );

    /// \brief Release a TexturePackagerCore object.
    /// \param[in] c           TexturePackagerCore object created by
    ///                        texpkgCreateTexturePackagerCore().
    /// \see texpkgCreateTexturePackagerCore.
    ///
    DLL_API void texpkgReleaseTexturePackagerCore(nvnTool::texpkg::TexturePackagerCore* c);

    TP_DLL_EXPORTED_FUNC(
        void,
        texpkgReleaseTexturePackagerCore,
        (nvnTool::texpkg::TexturePackagerCore*),
        RELEASE_TEXPKG_CORE_FUNC
    );

} // extern "C"

#endif // NVNUTIL_TEXPKGCORE_H
