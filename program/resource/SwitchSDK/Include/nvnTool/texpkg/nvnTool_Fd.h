/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_Fd.h
///       Definitions of common structures used for loading and saving images
///       using NVNFD (XTX) format archives produced by the NVN texture
///       packager tool.

#ifndef __NVNTOOL_FD_H__
#define  __NVNTOOL_FD_H__

#include <stdint.h>

namespace nvnTool {
namespace texpkg {

#define NVNFD_VERSION_MAJOR 1
#define NVNFD_VERSION_MINOR 1

#if defined(_MSC_VER) || defined(__clang__) || defined(__GNUC__)
    #pragma pack(push, 4)
#else
    #error Add logic to control structure packing for an unsupported compiler.
#endif

/// Binary representation of "NvFD".
#define NVNFD_ARCHIVE_MAGIC 0x4e764644

/// Archive header structure.
struct ArchiveHeader {
    uint32_t magic;           ///< Must be NVNFD_ARCHIVE_MAGIC
    uint32_t headerSize;      ///< Must match sizeof(ArchiveHeader).
    uint32_t majorVersion;
    uint32_t minorVersion;
};

enum BlockType {
    NVNFD_BLOCK_PADDING                    = 0,
    NVNFD_BLOCK_UNUSED                     = 1,
    NVNFD_BLOCK_TEXTURE_HEADER             = 2,
    NVNFD_BLOCK_TEXTURE                    = 3,
    NVNFD_BLOCK_USER                       = 4,
    NVNFD_BLOCK_END                        = 5,
    NVNFD_BLOCK_MAX
};

/// Binary representation of "NvBH".
#define NVNFD_BLOCKHEADER_MAGIC 0x4e764248

/// Block header structure.
struct BlockHeader {
    uint32_t magic;               ///< Should be little-endian binary representation of "NvBH"
    uint32_t blockHeaderSize;     ///< Must match sizeof(BlockHeader).
    uint64_t blockSize;           ///< Size of the block in bytes.
    uint64_t offset;              ///< Offset from the start of this header.
    int32_t  blockType;           ///< Type of block (see BlockType enum).
    uint32_t index;               ///< Global block index.
    uint32_t typeIndex;           ///< Monotonically increasing block type index.
};

#if defined(_MSC_VER) || defined(__clang__) || defined(__GNUC__)
    #pragma pack(pop)
#endif

} // namespace texpkg
} // namespace nvnTool

#endif // __NVNTOOL_FD_H__
