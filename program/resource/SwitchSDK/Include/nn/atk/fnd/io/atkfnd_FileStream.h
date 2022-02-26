/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

#include <nn/atk/fnd/io/atkfnd_Stream.h>
#include <nn/atk/fnd/basis/atkfnd_Result.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//! @internal
class FsAccessLog
{
public:
    virtual void BeginRead(void* arg) = 0;
    virtual void EndRead(void* arg) = 0;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  ファイルストリームです。
//---------------------------------------------------------------------------
class FileStream : public Stream
{
public:
    enum AccessMode
    {
        AccessMode_None,
        AccessMode_Read = 1 << 0,
        AccessMode_Write = 1 << 1,
        AccessMode_AllowAppend = 1 << 2,
        AccessMode_ReadAndWrite = AccessMode_Read | AccessMode_Write,
        AccessMode_AllowAppendAndWrite = AccessMode_Write | AccessMode_AllowAppend
    };

public:
    virtual ~FileStream() NN_NOEXCEPT NN_OVERRIDE {}

public: // メソッド
    virtual FndResult Open(const char* filePath, AccessMode openMode) NN_NOEXCEPT = 0;

    virtual void Close() NN_NOEXCEPT NN_OVERRIDE = 0;

    virtual void Flush() NN_NOEXCEPT = 0;

    virtual bool IsOpened() const NN_NOEXCEPT NN_OVERRIDE = 0;

    virtual bool CanRead() const NN_NOEXCEPT NN_OVERRIDE = 0;

    virtual bool CanWrite() const NN_NOEXCEPT NN_OVERRIDE = 0;

    virtual bool CanSeek() const NN_NOEXCEPT NN_OVERRIDE = 0;

    virtual size_t GetSize() const NN_NOEXCEPT NN_OVERRIDE = 0;

    virtual size_t Read(void* buf, size_t length, FndResult* result = NULL) NN_NOEXCEPT NN_OVERRIDE = 0;

    virtual size_t Write(const void* buf, size_t length, FndResult* result = NULL) NN_NOEXCEPT NN_OVERRIDE = 0;

    virtual FndResult Seek(position_t offset, SeekOrigin origin) NN_NOEXCEPT NN_OVERRIDE = 0;

    virtual position_t GetCurrentPosition() const NN_NOEXCEPT NN_OVERRIDE = 0;

    virtual void EnableCache(void* buffer, size_t length) NN_NOEXCEPT = 0;

    virtual void DisableCache() NN_NOEXCEPT = 0;

    virtual bool IsCacheEnabled() const NN_NOEXCEPT = 0;

    virtual int GetIoBufferAlignment() const NN_NOEXCEPT = 0;

    virtual bool CanSetFsAccessLog() const NN_NOEXCEPT = 0;

    virtual void* SetFsAccessLog(FsAccessLog* pFsAccessLog) NN_NOEXCEPT = 0;

    virtual position_t GetCachePosition() NN_NOEXCEPT = 0;

    virtual size_t GetCachedLength() NN_NOEXCEPT = 0;

public:
    static const int BufferAlignSize = 64; // 適当です
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
