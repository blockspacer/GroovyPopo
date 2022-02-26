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

#include <nn/atk/fnd/io/atkfnd_FileStream.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief  ファイルストリームです。
//---------------------------------------------------------------------------
class FileStreamProxy : public FileStream
{
public:
    FileStreamProxy(FileStream* fileStream, position_t offset, size_t fileSize) NN_NOEXCEPT;
    virtual ~FileStreamProxy() NN_NOEXCEPT NN_OVERRIDE;

    virtual FndResult Open(const char* filePath, AccessMode openMode) NN_NOEXCEPT NN_OVERRIDE;
    virtual void Close() NN_NOEXCEPT NN_OVERRIDE;
    virtual void Flush() NN_NOEXCEPT NN_OVERRIDE;

    virtual bool IsOpened() const NN_NOEXCEPT NN_OVERRIDE;

    virtual bool CanRead() const NN_NOEXCEPT NN_OVERRIDE;
    virtual bool CanWrite() const NN_NOEXCEPT NN_OVERRIDE;
    virtual bool CanSeek() const NN_NOEXCEPT NN_OVERRIDE;

    virtual size_t GetSize() const NN_NOEXCEPT NN_OVERRIDE;
    virtual size_t Read(void* buffer, size_t length, FndResult* result = NULL) NN_NOEXCEPT NN_OVERRIDE;
    virtual size_t Write(const void* buffer, size_t length, FndResult* result = NULL) NN_NOEXCEPT NN_OVERRIDE;
    virtual FndResult Seek(position_t offset, SeekOrigin origin) NN_NOEXCEPT NN_OVERRIDE;
    virtual position_t GetCurrentPosition() const NN_NOEXCEPT NN_OVERRIDE;

    virtual void EnableCache(void* buffer, size_t length) NN_NOEXCEPT NN_OVERRIDE;
    virtual void DisableCache() NN_NOEXCEPT NN_OVERRIDE;
    virtual bool IsCacheEnabled() const NN_NOEXCEPT NN_OVERRIDE;

    virtual int GetIoBufferAlignment() const NN_NOEXCEPT NN_OVERRIDE;
    virtual bool CanSetFsAccessLog() const NN_NOEXCEPT NN_OVERRIDE;
    virtual void* SetFsAccessLog(FsAccessLog* pFsAccessLog) NN_NOEXCEPT NN_OVERRIDE;
    virtual position_t GetCachePosition() NN_NOEXCEPT NN_OVERRIDE;
    virtual size_t GetCachedLength() NN_NOEXCEPT NN_OVERRIDE;

private:
    FileStream* m_pFileStream;
    position_t m_Offset;
    size_t m_FileSize;
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
