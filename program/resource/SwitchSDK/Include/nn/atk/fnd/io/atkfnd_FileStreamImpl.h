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

#include <nn/fs.h>

#include <nn/atk/fnd/io/atkfnd_FileStream.h>
#include <nn/atk/fnd/io/atkfnd_StreamCache.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief  ファイルストリームです。
//---------------------------------------------------------------------------
class FileStreamImpl : public FileStream
{
    NN_DISALLOW_COPY(FileStreamImpl);

private: // クラスの定義
    class DirectStream : public Stream
    {
    public:
        DirectStream() NN_NOEXCEPT : m_Owner(NULL) {}

        void Initialize(FileStreamImpl& owner) NN_NOEXCEPT
        {
            m_Owner = &owner;
        }

        virtual void Close() NN_NOEXCEPT NN_OVERRIDE {}
        virtual bool IsOpened() const NN_NOEXCEPT NN_OVERRIDE { return m_Owner->IsOpened(); }
        virtual bool CanRead() const NN_NOEXCEPT NN_OVERRIDE { return m_Owner->CanRead(); }
        virtual bool CanWrite() const NN_NOEXCEPT NN_OVERRIDE { return m_Owner->CanWrite(); }
        virtual bool CanSeek() const NN_NOEXCEPT NN_OVERRIDE { return m_Owner->CanSeek(); }
        virtual size_t GetSize() const NN_NOEXCEPT NN_OVERRIDE { return m_Owner->GetSize(); }
        virtual size_t Read(void* buf, size_t length, FndResult* result = NULL) NN_NOEXCEPT NN_OVERRIDE
        {
            return m_Owner->ReadDirect(buf, length, result);
        }
        virtual size_t Write(const void* buf, size_t length, FndResult* result = NULL) NN_NOEXCEPT NN_OVERRIDE
        {
            return m_Owner->WriteDirect(buf, length, result);
        }
        virtual FndResult Seek(position_t offset, SeekOrigin origin) NN_NOEXCEPT NN_OVERRIDE
        {
            return m_Owner->SeekDirect(offset, origin);
        }
        virtual position_t GetCurrentPosition() const NN_NOEXCEPT NN_OVERRIDE { return m_Owner->m_CurrentPosition; }

    private:
        FileStreamImpl* m_Owner;
    };

public: // コンストラクタ
    FileStreamImpl() NN_NOEXCEPT;

    explicit FileStreamImpl(void* fileHandle) NN_NOEXCEPT;

    virtual ~FileStreamImpl() NN_NOEXCEPT NN_OVERRIDE { }

public: // メソッド
    virtual FndResult Open(const char* filePath, AccessMode accessMode) NN_NOEXCEPT NN_OVERRIDE;

    virtual void Close() NN_NOEXCEPT NN_OVERRIDE;

    virtual void Flush() NN_NOEXCEPT NN_OVERRIDE;

    virtual bool IsOpened() const NN_NOEXCEPT NN_OVERRIDE;

    virtual bool CanRead() const NN_NOEXCEPT NN_OVERRIDE;

    virtual bool CanWrite() const NN_NOEXCEPT NN_OVERRIDE;

    virtual bool CanSeek() const NN_NOEXCEPT NN_OVERRIDE;

    virtual size_t GetSize() const NN_NOEXCEPT NN_OVERRIDE;

    virtual size_t Read(void* buf, size_t length, FndResult* result = NULL) NN_NOEXCEPT NN_OVERRIDE;

    virtual size_t Write(const void* buf, size_t length, FndResult* result = NULL) NN_NOEXCEPT NN_OVERRIDE;

    virtual FndResult Seek(position_t offset, SeekOrigin origin) NN_NOEXCEPT NN_OVERRIDE;

    virtual position_t GetCurrentPosition() const NN_NOEXCEPT NN_OVERRIDE
    {
        return IsCacheEnabled() ? m_StreamCache.GetCurrentPosition() : m_CurrentPosition;
    }

    virtual void EnableCache(void* buffer, size_t length) NN_NOEXCEPT NN_OVERRIDE;

    virtual void DisableCache() NN_NOEXCEPT NN_OVERRIDE;

    virtual bool IsCacheEnabled() const NN_NOEXCEPT NN_OVERRIDE
    {
        return m_StreamCache.IsInitialized();
    }

    virtual int GetIoBufferAlignment() const NN_NOEXCEPT NN_OVERRIDE;

    virtual bool CanSetFsAccessLog() const NN_NOEXCEPT NN_OVERRIDE
    {
        return true;
    }

    virtual void* SetFsAccessLog( FsAccessLog* pFsAccessLog ) NN_NOEXCEPT NN_OVERRIDE
    {
        m_pAccessLog = pFsAccessLog;
        return this;
    }

    virtual position_t GetCachePosition() NN_NOEXCEPT NN_OVERRIDE
    {
        if (IsCacheEnabled())
        {
            return m_StreamCache.GetCachePosition();
        }
        return 0;
    }

    virtual size_t GetCachedLength() NN_NOEXCEPT NN_OVERRIDE
    {
        if (IsCacheEnabled())
        {
            return m_StreamCache.GetCachedLength();
        }
        return 0;
    }

private: // メソッド
    size_t ReadDirect(void* buf, size_t length, FndResult* result = NULL) NN_NOEXCEPT;

    size_t WriteDirect(const void* buf, size_t length, FndResult* result = NULL) NN_NOEXCEPT;

    FndResult SeekDirect(position_t offset, SeekOrigin origin) NN_NOEXCEPT;

    void ValidateAlignment(const void* buf) const NN_NOEXCEPT;

private: // メンバ変数
    nn::fs::FileHandle m_FileHandle;
    bool               m_IsOpened;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t            m_Padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif

    mutable size_t m_FileSize;
    position_t     m_CurrentPosition;

    StreamCache  m_StreamCache;
    DirectStream m_DirectStream;    //!< StreamCache からの直接入出力するためのアダプタ
    FsAccessLog* m_pAccessLog;
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
