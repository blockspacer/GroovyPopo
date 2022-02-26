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

#include <nn/codec.h>
#include <nn/atk/atk_StreamSoundLoader.h> // for StreamDataDecoder
#include <nn/atk/atk_InstancePool.h>
#include <nn/atk/fnd/basis/atkfnd_Memory.h>
#include <nn/atk/detail/atk_Macro.h>

namespace nn { namespace atk { namespace detail { namespace driver {

class OpusDecoderImpl : public IStreamDataDecoder
{
public:
    static const size_t InputBufferSize = 4096;
    static const size_t DecodedBufferSizeMax;

    OpusDecoderImpl() NN_NOEXCEPT;
    virtual ~OpusDecoderImpl() NN_NOEXCEPT;

    void Initialize(void* buffer, size_t bufferSize, size_t workBufferAlignment, nn::codec::IOpusDecoder* decoder) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    virtual bool  ReadDataInfo( DataInfo* pOutValue, detail::fnd::FileStream* pFileStream ) NN_NOEXCEPT NN_OVERRIDE;
    virtual bool  Skip( detail::fnd::FileStream* pFileStream ) NN_NOEXCEPT NN_OVERRIDE;
    virtual bool  Decode( int16_t* pOutDecodedBufferAddresses[], detail::fnd::FileStream* pFileStream, int channelCount, DecodeType decodeType ) NN_NOEXCEPT NN_OVERRIDE;
    virtual void  Reset() NN_NOEXCEPT NN_OVERRIDE;

    virtual void ResetDecodeProfile() NN_NOEXCEPT NN_OVERRIDE;
    virtual DecodeProfile GetDecodeProfile() const NN_NOEXCEPT NN_OVERRIDE;

private:
    bool MultiBlockDecode( int16_t* pOutDecodedBufferAddresses[], detail::fnd::FileStream* pFileStream, int channelCount, DecodeType decodeType ) NN_NOEXCEPT;
    bool SingleBlockDecode( int16_t* pOutDecodedBufferAddresses[], detail::fnd::FileStream* pFileStream, int channelCount ) NN_NOEXCEPT;
    bool SingleBlockSkip( detail::fnd::FileStream* pFileStream ) NN_NOEXCEPT;

private:
    void* m_InputBuffer;
    int16_t* m_DecodedSampleBuffer;
    void* m_WorkBuffer;
    size_t m_WorkBufferSize;
    int m_SampleCount;

    nn::codec::IOpusDecoder* m_pDecoder;
    DecodeProfile m_DecodeProfile;
};

template <typename OpusDecoderType>
class OpusDecoderManagerImpl : public IStreamDataDecoderManager
{
public:
    OpusDecoderManagerImpl() NN_NOEXCEPT
        : m_Pool()
        , m_WorkBufferUnitSize(0)
        , m_IsInitialized(false)
        , m_StreamFileType(detail::StreamFileType_Bfstm)
        , m_DecodeMode(detail::DecodeMode_Invalid)
    {
    }

    virtual ~OpusDecoderManagerImpl() NN_NOEXCEPT NN_OVERRIDE
    {
    }

    void Initialize(void* buffer, size_t bufferSize, size_t unitBufferSize, int decoderCount, detail::StreamFileType type, detail::DecodeMode decodeMode) NN_NOEXCEPT
    {
        NN_UNUSED(bufferSize);
        NN_SDK_REQUIRES_NOT_NULL(buffer);
        NN_SDK_REQUIRES_GREATER(bufferSize, 0u);
        NN_SDK_REQUIRES_MINMAX(unitBufferSize, 0u, bufferSize);
        NN_SDK_REQUIRES_GREATER(decoderCount, 0);
        NN_SDK_REQUIRES_GREATER_EQUAL(bufferSize, unitBufferSize * decoderCount);

        if (IsInitialized())
        {
            return;
        }

        nn::util::BytePtr pMemory(buffer);

        size_t poolSize = sizeof(OpusDecoderType) * decoderCount;
        m_Pool.Create(pMemory.Get(), poolSize);
        pMemory.Advance(poolSize);

        m_WorkBufferUnitSize = unitBufferSize;
        size_t workBufferWholeSize = m_WorkBufferUnitSize * decoderCount;
        m_WorkBufferHeap.Initialize(pMemory.Get(), workBufferWholeSize, m_WorkBufferUnitSize, decoderCount);

        m_StreamFileType = type;
        m_DecodeMode = decodeMode;
        m_IsInitialized = true;
    }

    void Finalize() NN_NOEXCEPT
    {
        if (!IsInitialized())
        {
            return;
        }
        m_WorkBufferHeap.Finalize();
        m_WorkBufferUnitSize = 0;
        m_Pool.Destroy();

        m_StreamFileType = detail::StreamFileType_Bfstm;
        m_DecodeMode = detail::DecodeMode_Invalid;

        m_IsInitialized = false;
    }

    bool IsInitialized() const NN_NOEXCEPT
    {
        return m_IsInitialized;
    }

    virtual IStreamDataDecoder* AllocDecoder() NN_NOEXCEPT NN_OVERRIDE
    {
        OpusDecoderType* pDecoder = m_Pool.Alloc();
        if (pDecoder != nullptr)
        {
            void* workBuffer = m_WorkBufferHeap.Alloc();
            if (workBuffer == nullptr)
            {
                m_Pool.Free(pDecoder);
                return nullptr;
            }
            pDecoder->Initialize(workBuffer, m_WorkBufferUnitSize);
        }
        return pDecoder;
    }

    virtual void FreeDecoder(IStreamDataDecoder* pDecoder) NN_NOEXCEPT NN_OVERRIDE
    {
        NN_SDK_ASSERT_NOT_NULL(pDecoder);
        OpusDecoderType* pOpusDecoder = static_cast<OpusDecoderType*>(pDecoder);

        void* workBuffer = pOpusDecoder->Finalize();
        NN_SDK_ASSERT_NOT_NULL(workBuffer);
        m_WorkBufferHeap.Free(workBuffer);
        m_Pool.Free(pOpusDecoder);
    }

    int GetStreamFileType() const NN_NOEXCEPT NN_OVERRIDE
    {
        return m_StreamFileType;
    }

    int GetDecodeMode() const NN_NOEXCEPT NN_OVERRIDE
    {
        return m_DecodeMode;
    }

private:
    typedef InstancePool<OpusDecoderType> OpusDecoderPool;

    OpusDecoderPool m_Pool;
    detail::fnd::UnitHeap m_WorkBufferHeap;
    size_t m_WorkBufferUnitSize;
    bool m_IsInitialized;
    detail::StreamFileType m_StreamFileType;
    detail::DecodeMode m_DecodeMode;
};


}}}} // nn::atk::detail::driver

