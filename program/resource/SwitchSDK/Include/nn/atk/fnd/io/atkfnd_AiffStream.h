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

#include <nn/atk/fnd/basis/atkfnd_Result.h>
#include <nn/atk/fnd/io/atkfnd_Stream.h>
#include <nn/atk/fnd/io/atkfnd_WaveformStream.h>
#include <nn/atk/fnd/model/atkfnd_WaveFormat.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief  AIFF バイナリストリームを入力します。
//---------------------------------------------------------------------------
class AiffStreamReader : public WaveformStreamReader
{
    NN_DISALLOW_COPY(AiffStreamReader);

public: // コンストラクタ
    AiffStreamReader() NN_NOEXCEPT;
    explicit AiffStreamReader(Stream* stream) NN_NOEXCEPT;
    virtual ~AiffStreamReader() NN_NOEXCEPT NN_OVERRIDE { }

public: // メソッド
    FndResult Open(Stream* stream) NN_NOEXCEPT;
    virtual void Close() NN_NOEXCEPT NN_OVERRIDE;

    virtual bool IsOpened() const NN_NOEXCEPT NN_OVERRIDE
    {
        return m_Stream != NULL && m_Stream->IsOpened();
    }

    virtual bool IsHeaderRead() const NN_NOEXCEPT NN_OVERRIDE
    {
        return m_DataOffset > 0 && m_DataSize > 0;
    }

    virtual FndResult ReadHeader() NN_NOEXCEPT NN_OVERRIDE;
    virtual size_t ReadFrames(void* buffer, size_t frames, FndResult* result = NULL) NN_NOEXCEPT NN_OVERRIDE;

    virtual const WaveFormat& GetWaveFormat() const NN_NOEXCEPT NN_OVERRIDE { return m_WaveFormat; }
    virtual uint8_t GetBlockAlignment() const NN_NOEXCEPT NN_OVERRIDE { return m_BlockAlignment; }
    virtual size_t GetDataSize() const NN_NOEXCEPT NN_OVERRIDE { return m_DataSize; }

private:
    FndResult ReadFormChunk() NN_NOEXCEPT;

    bool ReadImpl(void* pBuffer, uint32_t size) NN_NOEXCEPT;

private: // メンバ変数
    Stream* m_Stream;

    WaveFormat m_WaveFormat;
    uint8_t         m_BlockAlignment;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t         m_Padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
    position_t      m_DataOffset;
    size_t          m_DataSize;
    position_t      m_CurrentDataPosition;
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
