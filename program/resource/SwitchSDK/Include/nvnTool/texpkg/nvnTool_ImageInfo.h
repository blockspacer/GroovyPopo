/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_ImageInfo.h
///       Helper module for storing and finding format information for images
///       used by the NVN texture packager tool.

#ifndef NVNTOOL_IMAGEINFO_H
#define NVNTOOL_IMAGEINFO_H

#include <assert.h>
#include <nvn/nvn.h>
#include "nvnTool_TexpkgCommon.h"

namespace nvnTool {
namespace texpkg {

/// Meta-information about a single NVN format.
struct FormatInfo {
    NVNformat fmt;

    uint32_t bpp;
    uint32_t numElements;

    uint32_t blockWidth;
    uint32_t blockHeight;
    uint32_t blockSize;

    const char* str;
    const wchar_t* wstr;

    bool isCompressed;
    bool isInt;
    bool isStencilDepth;
    bool isVertexFormat;
};

#ifdef NVNIMG_BUILD

/// \brief Get the information structure for given NVN format.
///
/// \param[in] format       The NVN format to retrieve information for.
///
/// \return The information structure if format match, NULL otherwise.
///         (weak ref, no ownership transfer)
///
const FormatInfo* const GetInfo(NVNformat format);

/// \brief Convert C string to matching NVNformat enum.
///
/// \param[in] formatStr   NVN format string to search for.
/// \return Matching NVN format if found, NVN_FORMAT_NONE otherwise.
///
NVNformat FormatFromStr(const wchar_t* formatStr);

#endif // NVNIMG_BUILD

} // namespace texpkg
} // namespace nvnTool

extern "C" {

    /// \brief Get the information structure for given NVN format.
    ///
    /// \param[in] format       The NVN format to retrieve information for.
    ///
    /// \return The information structure if format match, NULL otherwise.
    ///         (weak ref, no ownership transfer)
    ///
    DLL_API const nvnTool::texpkg::FormatInfo* const texpkgGetInfo(NVNformat format);

    TP_DLL_EXPORTED_FUNC(
        const nvnTool::texpkg::FormatInfo* const,
        texpkgGetInfo,
        (NVNformat),
        GET_INFO_FUNC
    );

    /// \brief Convert C string to matching NVNformat enum.
    ///
    /// \param[in] formatStr   NVN format string to search for.
    /// \return Matching NVN format if found, NVN_FORMAT_NONE otherwise.
    ///
    DLL_API NVNformat texpkgFormatFromStr(const wchar_t* formatStr);

    TP_DLL_EXPORTED_FUNC(
        NVNformat,
        texpkgFormatFromStr,
        (const wchar_t* formatStr),
        GET_FORMAT_FROM_STR_FUNC
    );

} // extern "C"

#endif // NVNTOOL_IMAGEINFO_H
