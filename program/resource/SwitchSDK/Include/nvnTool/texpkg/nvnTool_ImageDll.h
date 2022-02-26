/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_ImageDll.h
///       Helper for loading entry points for the NVN texture packager tool's
///       image library.

#ifndef NVNTOOL_IMAGEDLL_H
#define NVNTOOL_IMAGEDLL_H

#include "nvnTool_Image.h"
#include "nvnTool_ImageCombine.h"
#include "nvnTool_ImageConvert.h"
#include "nvnTool_ImageInfo.h"
#include "nvnTool_ImageMipmap.h"

#ifndef _MSC_VER
    #error "NVN Image DLL loader not supported on this compiler."
#endif
#include <Windows.h>

namespace nvnTool {
namespace texpkg {

/// \brief NVN RawImage library entry points.
///
/// Simple structure storing all the entry points exposed by the NVN RawImage DLL library.
///
struct RawImageLib {
    HMODULE module;

    texpkgCreateRawImageFunc createRawImage;
    texpkgReleaseRawImageFunc releaseRawImage;
    texpkgCreateRawImageCombineFunc createRawImageCombine;
    texpkgReleaseRawImageCombineFunc releaseRawImageCombine;
    texpkgCreateRawImageConverterFunc createRawImageConverter;
    texpkgReleaseRawImageConverterFunc releaseRawImageConverter;
    texpkgCreateRawImageMipmapGeneratorFunc createRawImageMipmapGenerator;
    texpkgReleaseRawImageMipmapGeneratorFunc releaseRawImageMipmapGenerator;
    texpkgRawMipMapLevelDataSliceFunc rawMipMapLevelDataSlice;
    texpkgRawMipMapLevelSlicePixelOffsetFunc rawMipMapLevelSlicePixelOffset;
    texpkgGetInfoFunc getInfo;
    texpkgFormatFromStrFunc formatFromStr;
};

/// \brief Load NVN Raw Image library and its entry points.
///
/// Load NVN Raw Image library and its entry points. For compatibility, this function simply points
/// the function pointers at the statically linked functions when TEXPKG_STATIC is defined.
///
/// \param[out] riLib     RawImage library struct.
/// \param[in] dllName    NvnImage DLL file name.
///
/// \return true if the operation was successful.
///
inline bool LoadNVNImageLibrary(RawImageLib *riLib, const wchar_t* dllName)
{
#ifndef TEXPKG_STATIC
    if (!dllName || !riLib) {
        dprintf("NULL parameter passed to LoadNVNImageLibrary.\n");
        return false;
    }

    // Load the DLL file.
    riLib->module = LoadLibraryW(dllName);
    if (!riLib->module) {
        dwprintf(L"LoadNVNImageLibrary could not load DLL %s.\n", dllName);
        return false;
    }
#endif

    // Load function entry points.
    TEXPKG_LOAD_ENTRY(riLib, createRawImage, texpkgCreateRawImage, CREATE_RAWIMAGE_FUNC)
    TEXPKG_LOAD_ENTRY(riLib, releaseRawImage, texpkgReleaseRawImage, RELEASE_RAWIMAGE_FUNC)
    TEXPKG_LOAD_ENTRY(riLib, createRawImageCombine, texpkgCreateRawImageCombine, CREATE_RAWIMAGE_COMBINE_FUNC)
    TEXPKG_LOAD_ENTRY(riLib, releaseRawImageCombine, texpkgReleaseRawImageCombine, RELEASE_RAWIMAGE_COMBINE_FUNC)
    TEXPKG_LOAD_ENTRY(riLib, createRawImageConverter, texpkgCreateRawImageConverter, CREATE_RAWIMAGE_CONVERTER_FUNC)
    TEXPKG_LOAD_ENTRY(riLib, releaseRawImageConverter, texpkgReleaseRawImageConverter, RELEASE_RAWIMAGE_CONVERTER_FUNC)
    TEXPKG_LOAD_ENTRY(riLib, createRawImageMipmapGenerator, texpkgCreateRawImageMipmapGenerator, CREATE_RAWIMAGE_MIPMAP_GENERATOR_FUNC)
    TEXPKG_LOAD_ENTRY(riLib, releaseRawImageMipmapGenerator, texpkgReleaseRawImageMipmapGenerator, RELEASE_RAWIMAGE_MIPMAP_GENERATOR_FUNC)
    TEXPKG_LOAD_ENTRY(riLib, rawMipMapLevelDataSlice, texpkgRawMipMapLevelDataSlice, RAWMIPMAP_LEVEL_DATASLICE_FUNC)
    TEXPKG_LOAD_ENTRY(riLib, rawMipMapLevelSlicePixelOffset, texpkgRawMipMapLevelSlicePixelOffset, RAWMIPMAP_LEVEL_SLICE_PIXEL_OFFSET_FUNC)
    TEXPKG_LOAD_ENTRY(riLib, getInfo, texpkgGetInfo, GET_INFO_FUNC)
    TEXPKG_LOAD_ENTRY(riLib, formatFromStr, texpkgFormatFromStr, GET_FORMAT_FROM_STR_FUNC)

    return true;
}

} // namespace texpkg
} // namespace nvnTool

#endif // NVN_IMAGE_DLL_H
