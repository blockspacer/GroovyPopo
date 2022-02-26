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

#include <algorithm>
#include <nn/atk/atk_Config.h>
#include <nn/atk/fnd/io/atkfnd_FileStream.h>

namespace nn { namespace atk { namespace detail {

//! @internal
class MemoryFileStream : public fnd::FileStream
{
public:
    MemoryFileStream( const void* buffer, size_t size ) NN_NOEXCEPT;

    virtual ~MemoryFileStream() NN_NOEXCEPT NN_OVERRIDE
    {
    }

    virtual detail::fnd::FndResult Open(const char* filePath, AccessMode openMode) NN_NOEXCEPT NN_OVERRIDE
    {
        NN_UNUSED(filePath);
        NN_UNUSED(openMode);
        NN_SDK_ASSERT(false, "Not supported.");

        return detail::fnd::FndResult(detail::fnd::FndResultType_Failed);
    }

    virtual void Close() NN_NOEXCEPT NN_OVERRIDE;
    virtual void Flush() NN_NOEXCEPT NN_OVERRIDE
    {
    }

    virtual bool IsOpened() const NN_NOEXCEPT NN_OVERRIDE
    {
        return m_pBuffer != NULL;
    }

    virtual bool CanRead() const NN_NOEXCEPT NN_OVERRIDE
    {
        return true;
    }

    virtual bool CanWrite() const NN_NOEXCEPT NN_OVERRIDE
    {
        return false;
    }

    virtual bool CanSeek() const NN_NOEXCEPT NN_OVERRIDE
    {
        return true;
    }

    virtual size_t GetSize() const NN_NOEXCEPT NN_OVERRIDE
    {
        return m_Size;
    }

    virtual size_t Read( void* buf, size_t length, fnd::FndResult* result = nullptr ) NN_NOEXCEPT NN_OVERRIDE;

    virtual size_t Write(const void* buf, size_t length, fnd::FndResult* result = nullptr) NN_NOEXCEPT NN_OVERRIDE
    {
        NN_UNUSED(buf);
        NN_UNUSED(length);
        NN_UNUSED(result);
        NN_SDK_ASSERT(false, "Not supported.");

        return 0;
    }

    virtual fnd::FndResult Seek(position_t offset, fnd::Stream::SeekOrigin origin) NN_NOEXCEPT NN_OVERRIDE;
    virtual position_t GetCurrentPosition() const NN_NOEXCEPT NN_OVERRIDE
    {
        return m_Position;
    }

    virtual void EnableCache(void* buffer, size_t length) NN_NOEXCEPT NN_OVERRIDE
    {
        NN_UNUSED(buffer);
        NN_UNUSED(length);
        NN_SDK_ASSERT(false, "Not supported.");
    }

    virtual void DisableCache() NN_NOEXCEPT NN_OVERRIDE
    {
        NN_SDK_ASSERT(false, "Not supported.");
    }

    virtual bool IsCacheEnabled() const NN_NOEXCEPT NN_OVERRIDE
    {
        return false;
    }

    virtual int GetIoBufferAlignment() const NN_NOEXCEPT NN_OVERRIDE
    {
        return 1;
    }

    virtual bool CanSetFsAccessLog() const NN_NOEXCEPT NN_OVERRIDE
    {
        return false;
    }

    virtual void* SetFsAccessLog(fnd::FsAccessLog* pFsAccessLog) NN_NOEXCEPT NN_OVERRIDE
    {
        NN_UNUSED(pFsAccessLog);
        NN_SDK_ASSERT(false, "Not supported.");
        return nullptr;
    }

    virtual position_t GetCachePosition() NN_NOEXCEPT NN_OVERRIDE
    {
        return 0;
    }

    virtual size_t GetCachedLength() NN_NOEXCEPT NN_OVERRIDE
    {
        return 0;
    }

private:
    const void* m_pBuffer;
    size_t m_Size;
    position_t m_Position;
};

}}}

