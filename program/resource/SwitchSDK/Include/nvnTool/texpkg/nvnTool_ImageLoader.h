/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_ImageLoader.h
///       This module provides a common interface for loading and saving
///       images in formats supported by the NVN texture packager tool.

#ifndef NVNTOOL_IMAGE_LOADER_H
#define NVNTOOL_IMAGE_LOADER_H

#include <stdlib.h>
#include <nvn/nvn.h>
#include "nvnTool_Texpkgcommon.h"
#include "nvnTool_Image.h"
#include "nvnTool_ImageDll.h"

#ifdef _MSC_VER
#include <Windows.h>
#endif

namespace nvnTool {
namespace texpkg {

/// Info for a image format loader.
struct RawImageFormatHandlerLibrary {
    /// Which extension (eg. dds) does this format handler handle.
    const wchar_t* extension;
    /// The DLL file name of the format handler library.
    const wchar_t* dllName;
    /// The DLL load function entry.
    const char* loadFuncName;
};

/// Number of default image format loaders.
#define TEXPKG_NUM_DEFAULT_FORMAT_LOADERS 2

/// The list of default image format loaders.
static RawImageFormatHandlerLibrary
RawImageFormatDefaultHandlerLibraries[] = {
#ifndef _M_IX86
    { L".tga", L"TgaLoader.dll", "TGALoadFile" },
    { L".dds", L"DdsLoader.dll", "DDSLoadFile" }
#else
    { L".tga", L"TgaLoader32.dll", "TGALoadFile" },
    { L".dds", L"DdsLoader32.dll", "DDSLoadFile" }
#endif
};

/// Image load manager that handles all supported formats.
class RawImageFormatManager {
public:
    virtual ~RawImageFormatManager() {}

    /// \brief Load an image file from disk. The file format will be determined from the filename
    /// extension.
    ///
    /// \param[in] fileName       The file to load.
    /// \param[out] img           The destination raw image to store loaded data into.
    ///
    /// \return TP_OK if the operation was successful or an error code otherwise.
    ///
    virtual TPError Load(const wchar_t* fileName, RawImage* img) = 0;
};

} // namespace texpkg
} // namespace nvnTool

extern "C" {

    /// \brief Create a RawImageFormatManager object to load images into RawImage objects.
    ///
    /// \param[in] imglib        NVN Image library.
    /// \param[in] numLoaders    Number of image loaders.
    /// \param[in] loaders       Image loader DLLs.
    /// \return Created RawImageFormatManager object.
    ///         This must be released using texpkgReleaseRawImageFormatManager().
    /// \see texpkgReleaseRawImageFormatManager.
    ///
    DLL_API nvnTool::texpkg::RawImageFormatManager*
    texpkgCreateRawImageFormatManager(nvnTool::texpkg::RawImageLib *imglib, int numLoaders, nvnTool::texpkg::RawImageFormatHandlerLibrary* loaders);

    TP_DLL_EXPORTED_FUNC(
        nvnTool::texpkg::RawImageFormatManager*,
        texpkgCreateRawImageFormatManager,
        (nvnTool::texpkg::RawImageLib *, int, nvnTool::texpkg::RawImageFormatHandlerLibrary*),
        CREATE_RAWIMAGE_FORMAT_MANAGER_FUNC
    );

    /// \brief Release a RawImageFormatManager object.
    /// \param[in] rifm       RawImageFormatManager object created by
    ///                       texpkgCreateRawImageFormatManager().
    /// \see texpkgCreateRawImageFormatManager.
    ///
    DLL_API void texpkgReleaseRawImageFormatManager(nvnTool::texpkg::RawImageFormatManager* rifm);

    TP_DLL_EXPORTED_FUNC(
        void,
        texpkgReleaseRawImageFormatManager,
        (nvnTool::texpkg::RawImageFormatManager*),
        RELEASE_RAWIMAGE_FORMAT_MANAGER_FUNC
    );

} // extern "C"

#endif // NVNTOOL_IMAGE_LOADER_H
