/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_FileIo.h
///       Class for writing NVNFD (XTX) format archive files produced by the
///       NVN texture packager tool.

#ifndef NVNTOOL_FILEIO_H
#define  NVNTOOL_FILEIO_H

#include <stdio.h>
#include <wchar.h>
#include "nvnTool_Fd.h"
#include "nvnTool_TexpkgCommon.h"

namespace nvnTool {
namespace texpkg {

/// This module is responsible for writing and appending to an NVNFD archive.
class ArchiveFileWriter {
public:
    virtual ~ArchiveFileWriter() {}

    /// \brief Whether opened file handle is valid.
    ///
    /// If append is disabled, this returns whether the file has been successfully created.
    /// Otherwise returns whether the archive was successfully read, or created if no previous
    /// archive exists.
    ///
    /// \return true if the file is ready to be written to, false otherwise.
    ///
    virtual bool Valid(void) const = 0;

    /// \brief Write the archive file header. Only do this when append mode is off.
    ///
    /// \return true if write successful, false otherwise.
    ///
    virtual bool WriteFileHeader(void) = 0;

    /// \brief Append a data block to a opened archive.
    ///
    /// \param[in] type             The block type of the block to write.
    /// \param[in] align            Alignment requirement of the data of this block, relative to the
    ///                             start of the archive. Set to 0 to disable.
    /// \param[in] data             The block data.
    /// \param[in] size             Size of given block data, in bytes.
    ///
    /// \return true if write successful, false otherwise.
    ///
    virtual bool WriteAppendBlock(BlockType type, uint32_t align, const uint8_t * data, uint64_t size) = 0;

    /// \brief Write the archive file footer. Need to do this for both append and overwrite modes.
    ///
    /// \return true if write successful, false otherwise.
    ///
    virtual bool WriteEndBlock(void) = 0;

    /// \brief Close file and write everything to disk.
    /// This will automatically be done if not explicitly called.
    ///
    /// \return true if write successful, false otherwise.
    ///
    virtual void Close(void) = 0;
};

} // namespace texpkg
} // namespace nvnTool

extern "C" {

    /// \brief Create a ArchiveFileWriter object to write HW textures to an archive.
    ///
    /// \param[in] filename  File name of archive to write or append to.
    /// \param[in] append    Whether to append to existing archive. If existing archive isn't found,
    ///                      a new one will be created.
    /// \return Created ArchiveFileWriter object.
    ///         This must be released using texpkgReleaseArchiveFileWriter().
    /// \see texpkgReleaseArchiveFileWriter.
    ///
    DLL_API nvnTool::texpkg::ArchiveFileWriter* texpkgCreateArchiveFileWriter(const wchar_t* filename, bool append);

    TP_DLL_EXPORTED_FUNC(
        nvnTool::texpkg::ArchiveFileWriter*,
        texpkgCreateArchiveFileWriter,
        (const wchar_t*, bool),
        CREATE_ARCHIVE_FILE_WRITER_FUNC
    );

    /// \brief Release a ArchiveFileWriter object.
    /// \param[in] afw         ArchiveFileWriter object created by
    ///                        texpkgCreateArchiveFileWriter().
    /// \see texpkgCreateArchiveFileWriter.
    ///
    DLL_API void texpkgReleaseArchiveFileWriter(nvnTool::texpkg::ArchiveFileWriter* afw);

    TP_DLL_EXPORTED_FUNC(
        void,
        texpkgReleaseArchiveFileWriter,
        (nvnTool::texpkg::ArchiveFileWriter*),
        RELEASE_ARCHIVE_FILE_WRITER_FUNC
    );

} // extern "C"

#endif // NVNTOOL_FILEIO_H
