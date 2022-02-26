/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_HwTextureExport.h
///       Class for converting textures to and from the hardware formats
///       supported by the NVN texture packager.

#ifndef NVNTOOL_HWTEXTUREEXPORT_H
#define NVNTOOL_HWTEXTUREEXPORT_H

#include <assert.h>
#include <stdlib.h>
#include <nvn/nvn.h>
#include "nvnTool_HwTexture.h"
#include "nvnTool_TexpkgCommon.h"
#include "nvnTool_Image.h"
#include "nvnTool_ImageDll.h"

namespace nvnTool {
namespace texpkg {

/// \brief Hardware texture conversion flags.
enum RawImageHardwareTextureFlagsEnum {

    /// \brief Whether to align the image for use with NX sparse.
    ///        This is similar to nvn::TextureFlags::SPARSE.
    ///        Cannot be used with NVN_TEXPKG_FLAGS_MINIMAL_LAYOUT.
    NVN_TEXPKG_STORAGE_FLAGS_SPARSE = 0x1,

    /// \brief Whether choose minimal texture storage layout in order to save memory.
    ///        Note that this may impact performance.
    ///        This is similar to nvn::TextureFlags::MINIMAL_LAYOUT.
    ///        Cannot be used with NVN_TEXPKG_FLAGS_SPARSE.
    NVN_TEXPKG_STORAGE_FLAGS_MINIMAL_LAYOUT = 0x2

};

/// This module is responsible for converting from RawImages to hardware dependent texture formats.
class RawImageHardwareTextureExporter {
public:
    virtual ~RawImageHardwareTextureExporter() {}

    /// \brief Convert to hardware format texture for NX.
    ///
    /// \param[in] inImage            Input image.
    /// \param[out] outTexture        Destination HW Texture structure.
    /// \param[in] flags              Hardware texture storage flags.
    ///                               See RawImageHardwareTextureFlagsEnum.
    ///
    /// \return TP_OK if the operation was successful or an error code otherwise.
    ///
    virtual TPError Convert(RawImage* inImage, NVNHWTexture* outTexture, uint32_t flags = 0U) = 0;

    /// \brief Unconvert from a hardware format texture back into a pitch-linear NVN image.
    ///
    /// \param[out] outImage          Output image. This image must be empty (ie. RawImage::Create*()
    //                                must not have been previously called on it).
    /// \param[in] inTexture          Source HW texture. The header must at least have target,
    ///                               width, height, depth, format, mipmapLevels, sparse, layout,
    ///                               and subImagePitch fields set correctly.
    ///
    /// \return TP_OK if the operation was successful or an error code otherwise.
    ///
    virtual TPError Unconvert(RawImage* outImage,  NVNHWTexture* inTexture) = 0;

    /// \brief Dump binary hardware texture data to a file without additional formatting or headers.
    ///
    /// \param[in] fileName           The output file name.
    /// \param[in] texture            Texture to dump.
    ///
    /// \return TP_OK if success, < 0 error code otherwise.
    ///
    virtual TPError DumpToFile(const wchar_t* fileName, const NVNHWTexture* texture) = 0;

    /// \brief Release the storage data associated with a HW texture.
    ///
    /// \param[in] hwTexture            The HW texture containing storage data to release.
    ///
    virtual void ReleaseTextureData(NVNHWTexture *hwTexture) = 0;

    /// \brief Get HW texture swizzled data size. This size may be bigger than the pitch linear
    ///        image size. See NVN programming guide for details on the block format.
    ///
    /// \param[in] target             The texture target.
    /// \param[in] width              Width of the texture.
    /// \param[in] height             Height of the texture.
    /// \param[in] depth              Depth of the texture.
    /// \param[in] format             Texel format of the texture.
    /// \param[in] mipLevels          Number of mipmaps the texture contains.
    /// \param[in] subimages          Number of subimages (layers) the texture contains.
    /// \param[in] flags              Hardware texture storage flags.
    ///                               See RawImageHardwareTextureFlagsEnum.
    ///
    /// \return Swizzles texture data size in bytes, or 0 if failure.
    ///
    virtual uint64_t GetDataSize(NVNtextureTarget target, uint32_t width, uint32_t height, uint32_t depth,
                                 NVNformat format, uint32_t mipLevels, uint32_t subimages, uint32_t flags) const = 0;
};

} // namespace texpkg
} // namespace nvnTool

extern "C" {

    /// \brief Create a RawImageHardwareTextureExporter object to export images to HW texture format.
    ///
    /// \param[in] imglib             Loaded NVN image library.
    /// \return Created RawImageHardwareTextureExporter object.
    ///         This must be released using texpkgReleaseRawImageHardwareTextureExporter().
    /// \see texpkgReleaseRawImageHardwareTextureExporter.
    ///
    DLL_API nvnTool::texpkg::RawImageHardwareTextureExporter*
    texpkgCreateRawImageHardwareTextureExporter(nvnTool::texpkg::RawImageLib *imglib);

    TP_DLL_EXPORTED_FUNC(
        nvnTool::texpkg::RawImageHardwareTextureExporter*,
        texpkgCreateRawImageHardwareTextureExporter,
        (nvnTool::texpkg::RawImageLib*),
        CREATE_RAWIMAGE_HW_EXPORTER_FUNC
    );

    /// \brief Release a RawImageHardwareTextureExporter object.
    /// \param[in] hwexp       RawImageHardwareTextureExporter object created by
    ///                        texpkgCreateRawImageHardwareTextureExporter().
    /// \see texpkgCreateRawImageHardwareTextureExporter.
    ///
    DLL_API void texpkgReleaseRawImageHardwareTextureExporter (nvnTool::texpkg::RawImageHardwareTextureExporter* hwexp);

    TP_DLL_EXPORTED_FUNC(
        void,
        texpkgReleaseRawImageHardwareTextureExporter,
        (nvnTool::texpkg::RawImageHardwareTextureExporter*),
        RELEASE_RAWIMAGE_HW_EXPORTER_FUNC
    );

} // extern "C"

#endif // NVNTOOL_HWTEXTUREEXPORT_H
