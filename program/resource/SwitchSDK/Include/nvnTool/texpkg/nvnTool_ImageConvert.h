/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_ImageConvert.h
///       This module supports format conversion between pitch-linear images
///       in the NVN texture packager tool.

#ifndef NVNTOOL_IMAGECONVERT_H
#define NVNTOOL_IMAGECONVERT_H

#include <nvn/nvn.h>
#include "nvnTool_Image.h"
#include "nvnTool_DataTypes.h"
#include "nvnTool_TexpkgCommon.h"

namespace nvnTool {
namespace texpkg {

typedef double NVNtexel;

/// \brief This module is responsible for format conversion. Conversion uses doubles as the
/// intermediate format, and supports Floyd-Steinberg dithering.
///
class RawImageConverter {
public:
    virtual ~RawImageConverter() {}

    /// \brief Convenience helper function to fetch a pixel from a mipmap level. double[4] is used as
    /// the intermediate format.
    ///
    /// \param[in] lp       Texture mipmap level ref to fetch pixel from.
    /// \param[in] x        X (width) co-ordinates to fetch pixel from.
    /// \param[in] y        Y (Height) co-ordinates to fetch pixel from.
    /// \param[in] z        Z (Depth) co-ordinates to fetch pixel from.
    ///
    /// \return Pixel colour value in RGBA double[4] vector.
    ///
    virtual dt::dvec4 GetPixel(RawMipMapLevel *lp, uint32_t x, uint32_t y, uint32_t z) = 0;

    /// \brief Convenience helper function to set a pixel in a mipmap level.
    ///
    /// \param[in] lp            Texture mipmap level ref to set pixel at.
    /// \param[in] x             X (width) co-ordinates to set pixel at.
    /// \param[in] y             Y (Height) co-ordinates to set pixel at.
    /// \param[in] z             Z (Depth) co-ordinates to set pixel at.
    /// \param[in] texelValue    Normalised double[4] pixel colour value to set.
    ///
    /// \return True if the given mipmap level and co-ordinates are valid and the set pixel
    ///         succeeded, False otherwise.
    ///
    virtual bool SetPixel(RawMipMapLevel *lp, uint32_t x, uint32_t y, uint32_t z,
                          dt::dvec4 texelValue) = 0;

    /// \brief Convenience helper function to fetch a pixel from an image. double[4] is used as
    /// the intermediate format.
    ///
    /// \param[in] img      Image to fetch pixel from.
    /// \param[in] subimage Sub-image index to fetch pixel from.
    /// \param[in] lvl      Mipmap level inside sub-image to fetch pixel from.
    /// \param[in] x        X (width) co-ordinates to fetch pixel from.
    /// \param[in] y        Y (Height) co-ordinates to fetch pixel from.
    /// \param[in] z        Z (Depth) co-ordinates to fetch pixel from.
    ///
    /// \return Pixel colour value in RGBA double[4] vector.
    ///
    virtual dt::dvec4 GetPixel(RawImage *img, uint32_t subimage,
                               uint32_t lvl, uint32_t x, uint32_t y, uint32_t z) = 0;

    /// \brief Convert format from a single image mipmap level to another mipmap level.
    ///
    /// Convert format from a single image mipmap level to another mipmap level. double[4] is used as
    /// the intermediate conversion format. Must source and dest must have space already allocated,
    /// and dimensions & format matching. Does NOT do scaling.
    ///
    /// \param[in] src      Source image to convert from.
    /// \param[out] dst     Destination image to convert to.
    /// \param[in] dither   Whether to enable dithering during format conversion.
    ///
    /// \return TP_OK if the operation was successful or an error code otherwise.
    ///
    virtual TPError Convert(RawMipMapLevel *src, RawMipMapLevel *dst, bool dither = true) = 0;

    /// \brief Convert image format. double[4] is used as the intermediate conversion format.
    ///
    /// \param[in] src        Source image mipmap level to convert from.
    /// \param[out] dst       Destination image mipmap level to convert to. This must be an empty
    ///                       unallocated image.
    /// \param[in] dstFormat  NVN Format to convert to.
    /// \param[in] dither     Whether to enable dithering during format conversion.
    ///
    /// \return TP_OK if the operation was successful or an error code otherwise.
    ///
    virtual TPError Convert(RawImage *src, RawImage *dst, NVNformat dstFormat, bool dither = true) = 0;

};

} // namespace texpkg
} // namespace nvnTool

extern "C" {

    /// \brief Create a RawImageConverter object, to convert images between formats.
    ///
    /// \return Created RawImageConverter object. This must be released using texpkgReleaseRawImageConverter().
    /// \see texpkgReleaseRawImageConverter.
    ///
    DLL_API nvnTool::texpkg::RawImageConverter* texpkgCreateRawImageConverter(void);

    TP_DLL_EXPORTED_FUNC(
        nvnTool::texpkg::RawImageConverter*,
        texpkgCreateRawImageConverter,
        (void),
        CREATE_RAWIMAGE_CONVERTER_FUNC
    );

    /// \brief Release a RawImageConverter object.
    ///
    /// \param[in] ric       RawImageConverter object created by texpkgCreateRawImageConverter().
    /// \see texpkgCreateRawImageConverter
    ///
    DLL_API void texpkgReleaseRawImageConverter(nvnTool::texpkg::RawImageConverter* ric);

    TP_DLL_EXPORTED_FUNC(
        void,
        texpkgReleaseRawImageConverter,
        (nvnTool::texpkg::RawImageConverter*),
        RELEASE_RAWIMAGE_CONVERTER_FUNC
    );

} // extern "C"

#endif // NVNTOOL_IMAGECONVERT_H
