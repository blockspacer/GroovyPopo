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

#include <nn/atk/fnd/basis/atkfnd_Config.h>
#include <nn/atk/fnd/basis/atkfnd_Result.h>
#include <nn/atk/fnd/io/atkfnd_SimpleRingBuffer.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief  HIO入出力ストリームのクラスです。
//---------------------------------------------------------------------------
class HioStream
{
public:
    typedef const char* ChannelType;

    NN_DISALLOW_COPY(HioStream);

public:
    HioStream() NN_NOEXCEPT;
    virtual ~HioStream() NN_NOEXCEPT;

    void Initialize(void* workBuffer, size_t workBufferLength) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    bool Open(ChannelType channel, void* buffer, size_t length) NN_NOEXCEPT;
    virtual void Close() NN_NOEXCEPT;

    virtual size_t Read(void* buffer, size_t length) NN_NOEXCEPT;
    size_t Peek(void* buffer, size_t length) NN_NOEXCEPT;
    size_t Skip(size_t length) NN_NOEXCEPT;
    virtual size_t Write(const void* buffer, size_t length) NN_NOEXCEPT;

    bool Connect() NN_NOEXCEPT;
    void Disconnect() NN_NOEXCEPT;

    bool IsOpened() const NN_NOEXCEPT { return m_IsOpened; }
    virtual bool IsAvailable() const NN_NOEXCEPT;
    ChannelType GetChannel() const NN_NOEXCEPT { return m_Channel; }

    size_t GetReadableBytes() NN_NOEXCEPT;
    size_t GetWritableBytes() const NN_NOEXCEPT;

    virtual bool CanAsync() const NN_NOEXCEPT { return false; }
    virtual bool CanRead() const NN_NOEXCEPT { return IsAvailable(); }
    virtual bool CanWrite() const NN_NOEXCEPT { return IsAvailable(); }

private:
    void OpenImpl(ChannelType channel) NN_NOEXCEPT;
    void CloseImpl() NN_NOEXCEPT;
    size_t WriteImpl(const void* buffer, size_t length) NN_NOEXCEPT;
    void ReadBuffer() NN_NOEXCEPT;

private:
    bool m_IsOpened;
    ChannelType m_Channel;

    SimpleRingBuffer m_RingBuffer;
    void* m_WorkBuffer;
    size_t m_WorkBufferLength;
    size_t m_ReadSize;

    int m_Socket;
    int m_ClientSocket;

    nn::atk::detail::fnd::CriticalSection m_ReadingSection;
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
