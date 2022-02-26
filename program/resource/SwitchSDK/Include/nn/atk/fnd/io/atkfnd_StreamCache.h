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

#include <climits>
#include <nn/atk/fnd/basis/atkfnd_Config.h>
#include <nn/atk/fnd/basis/atkfnd_Result.h>
#include <nn/atk/fnd/io/atkfnd_Stream.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

class FsAccessLog;

//---------------------------------------------------------------------------
//! @internal
//! @brief  ストリームのキャッシュ機能を提供します。
//---------------------------------------------------------------------------
class StreamCache
{
    NN_DISALLOW_COPY(StreamCache);

private: // 定数
    static const position_t InvalidPosition = UINT_MAX;

private: // 型の定義
    enum CacheState
    {
        CacheState_None = 0,
        CacheState_ForRead = 1,
        CacheState_ForWrite = 2
    };

public: // コンストラクタ
    StreamCache() NN_NOEXCEPT;

    StreamCache(Stream* stream, void* buffer, size_t length) NN_NOEXCEPT;

    virtual ~StreamCache() NN_NOEXCEPT { }

public: // メソッド
    void Initialize(Stream* stream, void* buffer, size_t length) NN_NOEXCEPT;

    void Finalize() NN_NOEXCEPT;

    bool IsInitialized() const NN_NOEXCEPT
    {
        return m_Stream != NULL && m_CacheBuffer != NULL && m_CacheBufferLength > 0;
    }

    bool CanRead() const NN_NOEXCEPT { return IsInitialized() && m_Stream->CanRead(); }

    bool CanWrite() const NN_NOEXCEPT { return IsInitialized() && m_Stream->CanWrite(); }

    bool CanSeek() const NN_NOEXCEPT { return IsInitialized() && m_Stream->CanSeek(); }

    size_t Read(void* buf, size_t length, FndResult* result = NULL, FsAccessLog* log = nullptr, void* pFileStream = nullptr) NN_NOEXCEPT;

    size_t Write(const void* buf, size_t length, FndResult* result = NULL) NN_NOEXCEPT;

    FndResult Seek(position_t offset, Stream::SeekOrigin origin) NN_NOEXCEPT;

    size_t GetCurrentPosition() const NN_NOEXCEPT
    {
        return m_CurrentPosition;
    }

    void ClearCache() NN_NOEXCEPT;

    position_t GetCachePosition() NN_NOEXCEPT
    {
        return m_CachePosition;
    }

    size_t GetCachedLength() NN_NOEXCEPT
    {
        return m_CachedLength;
    }

private: // メソッド
    size_t GetReadCacheHitLength(size_t length) const NN_NOEXCEPT;

    size_t GetWritableCacheLength(size_t length) const NN_NOEXCEPT;

    //! @brief  対象 Stream の現在位置を StreamCache の現在位置に合わせます。
    FndResult SyncStreamCurrentPosition(position_t position) NN_NOEXCEPT;

    //! @brief  キャッシュの内容を書き込みます。
    FndResult FlushWriteCache() NN_NOEXCEPT;

private: // メンバ変数
    Stream* m_Stream;
    position_t  m_CurrentPosition;

    void*       m_CacheBuffer;
    size_t      m_CacheBufferLength;
    position_t  m_CachePosition;
    size_t      m_CachedLength;

    uint8_t     m_CacheState;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t     m_Padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
