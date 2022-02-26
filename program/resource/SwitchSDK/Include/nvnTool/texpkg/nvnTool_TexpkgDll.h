/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_TexpkgDll.h
///       Helper for loading entry points from the core NVN texture packager
///       library. 

#ifndef NVNTOOL_TEXPKG_DLL_H
#define NVNTOOL_TEXPKG_DLL_H

#include "nvnTool_ImageLoader.h"
#include "nvnTool_HwTextureExport.h"
#include "nvnTool_TexpkgCore.h"
#include "nvnTool_FileIo.h"

#ifndef _MSC_VER
    #error "Texture packager DLL loader not supported on this compiler."
#endif
#include <Windows.h>

namespace nvnTool {
namespace texpkg {

/// \brief NVN Texpkg library entry points.
///
/// Simple structore storing all the entry points exposed by the NVN Texpkg DLL library.
///
struct TexpkgLib {
    HMODULE module;

    texpkgCreateRawImageFormatManagerFunc createRawImageFormatManager;
    texpkgReleaseRawImageFormatManagerFunc releaseRawImageFormatManager;
    texpkgCreateRawImageHardwareTextureExporterFunc createRawImageHardwareTextureExporter;
    texpkgReleaseRawImageHardwareTextureExporterFunc releaseRawImageHardwareTextureExporter;
    texpkgCreateTexturePackagerCoreFunc createTexturePackagerCore;
    texpkgReleaseTexturePackagerCoreFunc releaseTexturePackagerCore;
    texpkgCreateArchiveFileWriterFunc createArchiveFileWriter;
    texpkgReleaseArchiveFileWriterFunc releaseArchiveFileWriter;
};

/// \brief Load NVN Texpkg library and its entry points. For compatibility, this function simply points
/// the function pointers at the statically linked functions when TEXPKG_STATIC is defined.
///
/// \param[out] tpLib     Texpkg library struct.
/// \param[in] dllName    NvnTexpkg DLL file name.
///
/// \return true if the operation was successful.
///
inline bool LoadTexpkgLibrary(TexpkgLib *tpLib, const wchar_t* dllName)
{
#ifndef TEXPKG_STATIC
    if (!dllName || !tpLib) {
        dprintf("NULL parameter passed to LoadTexpkgLibrary.\n");
        return false;
    }

    // Load the DLL file.
    tpLib->module = LoadLibraryW(dllName);
    if (!tpLib->module) {
        dwprintf(L"LoadTexpkgLibrary could not load DLL %s.\n", dllName);
        return false;
    }
#endif

    // Load function entry points.
    TEXPKG_LOAD_ENTRY(tpLib, createRawImageFormatManager, texpkgCreateRawImageFormatManager, CREATE_RAWIMAGE_FORMAT_MANAGER_FUNC)
    TEXPKG_LOAD_ENTRY(tpLib, releaseRawImageFormatManager, texpkgReleaseRawImageFormatManager, RELEASE_RAWIMAGE_FORMAT_MANAGER_FUNC)
    TEXPKG_LOAD_ENTRY(tpLib, createRawImageHardwareTextureExporter, texpkgCreateRawImageHardwareTextureExporter, CREATE_RAWIMAGE_HW_EXPORTER_FUNC)
    TEXPKG_LOAD_ENTRY(tpLib, releaseRawImageHardwareTextureExporter, texpkgReleaseRawImageHardwareTextureExporter, RELEASE_RAWIMAGE_HW_EXPORTER_FUNC)
    TEXPKG_LOAD_ENTRY(tpLib, createTexturePackagerCore, texpkgCreateTexturePackagerCore, CREATE_TEXPKG_CORE_FUNC)
    TEXPKG_LOAD_ENTRY(tpLib, releaseTexturePackagerCore, texpkgReleaseTexturePackagerCore, RELEASE_TEXPKG_CORE_FUNC)
    TEXPKG_LOAD_ENTRY(tpLib, createArchiveFileWriter, texpkgCreateArchiveFileWriter, CREATE_ARCHIVE_FILE_WRITER_FUNC)
    TEXPKG_LOAD_ENTRY(tpLib, releaseArchiveFileWriter, texpkgReleaseArchiveFileWriter, RELEASE_ARCHIVE_FILE_WRITER_FUNC)

    return true;
}

} // namespace texpkg
} // namespace nvnTool

#endif // NVNTOOL_TEXPKGDLL_H
