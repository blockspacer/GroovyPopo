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
#include <nn/atk/detail/atk_OpusDecoderImpl.h>

namespace nn { namespace atk {

//---------------------------------------------------------------------------
//! @brief  HardwareOpusDecoder を初期化します。
//!
//!         ハードウェアを利用してデコード処理を行います。
//!         OpusDecoder と併用が可能です。
//!         @ref GetRequiredHardwareOpusDecoderBufferSize() で取得したサイズのバッファを設定してください。
//!         こちらの関数で初期化した場合には、デコードできるサウンドインスタンス数は 1 つになります。
//!         2 つ以上のデコード処理を同時に行う場合は、 decoderCount を引数に持つ @ref InitializeHardwareOpusDecoder をご利用ください。
//!
//! @param[in]    buffer        HardwareOpusDecoder が内部で使用するバッファです。
//! @param[in]    bufferSize    HardwareOpusDecoder が使用するバッファのサイズです。
//!
//! @pre
//! - buffer != nullptr
//! - bufferSize == GetRequiredHardwareOpusDecoderBufferSize
//!
//! @see FinalizeHardwareOpusDecoder
//! @see GetRequiredOpusDecoderBufferSize
//! @see InitializeOpusDecoder
//---------------------------------------------------------------------------
void InitializeHardwareOpusDecoder(void* buffer, size_t bufferSize) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief  HardwareOpusDecoder を初期化します。
//!
//!         ハードウェアを利用してデコード処理を行います。
//!         OpusDecoder と併用が可能で、それぞれデコーダー数を指定することができます。
//!         @ref GetRequiredHardwareOpusDecoderBufferSize() で取得したサイズのバッファを設定してください。
//!         1 つのサウンドインスタンスにつき 1 つのデコーダーを利用します。
//!         同時に使用するデコーダーの数を decoderCount で指定してください。
//!
//! @param[in]    buffer        HardwareOpusDecoder が内部で使用するバッファです。
//! @param[in]    bufferSize    HardwareOpusDecoder が使用するバッファのサイズです。
//! @param[in]    decoderCount  HardwareOpusDecoder の数です。
//!
//! @pre
//! - buffer != nullptr
//! - bufferSize == GetRequiredHardwareOpusDecoderBufferSize
//! - decoderCount > 0
//!
//! @see FinalizeHardwareOpusDecoder
//! @see GetRequiredHardwareOpusDecoderBufferSize
//! @see InitializeOpusDecoder
//---------------------------------------------------------------------------
void InitializeHardwareOpusDecoder(void* buffer, size_t bufferSize, int decoderCount) NN_NOEXCEPT;


//---------------------------------------------------------------------------
//! @brief  HardwareOpusDecoder の終了処理を行います。
//!         これによりデコーダーのインスタンスが破棄され、
//!         InitializeHardwareOpusDecoder 時に渡したバッファを解放することができるようになります。
//!
//! @see InitializeHardwareOpusDecoder
//---------------------------------------------------------------------------
void FinalizeHardwareOpusDecoder() NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief  HardwareOpusDecoder が内部で使用するバッファのサイズを取得します。
//!
//!         こちらの関数では、デコーダーが １ つの場合に必要となるバッファサイズを取得することができます。
//!
//! @return 必要となるバッファサイズ(byte)を返します。
//!
//! @see InitializeHardwareOpusDecoder
//---------------------------------------------------------------------------
size_t GetRequiredHardwareOpusDecoderBufferSize() NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief  HardwareOpusDecoder が内部で使用するバッファのサイズを取得します。
//!
//!         同時に使用するデコーダーの数を decoderCount で指定してください。
//!         ここで指定した decoderCount と違う値を InitializeOpusDecoder() に渡すと初期化に失敗します。
//!         また、decoderCount で指定する数に比例して必要となるバッファサイズが大きくなる点にご注意ください。
//!
//! @param[in]    decoderCount  HardwareOpusDecoder の数です。
//!
//! @return 必要となるバッファサイズ(byte)を返します。
//!
//! @pre
//! - decoderCount > 0
//!
//! @see InitializeHardwareOpusDecoder
//---------------------------------------------------------------------------
size_t GetRequiredHardwareOpusDecoderBufferSize(int decoderCount) NN_NOEXCEPT;

}}

namespace nn { namespace atk { namespace detail { namespace driver {

class HardwareOpusDecoder : public IStreamDataDecoder
{
public:
    HardwareOpusDecoder() NN_NOEXCEPT;
    ~HardwareOpusDecoder() NN_NOEXCEPT;

    void Initialize(void* buffer, size_t bufferSize) NN_NOEXCEPT;
    void* Finalize() NN_NOEXCEPT;

    virtual bool  ReadDataInfo(DataInfo* pOutValue, detail::fnd::FileStream* pFileStream) NN_NOEXCEPT NN_OVERRIDE;
    virtual bool  Skip(detail::fnd::FileStream* pFileStream) NN_NOEXCEPT NN_OVERRIDE;
    virtual bool  Decode(int16_t* pOutDecodedBufferAddresses[], detail::fnd::FileStream* pFileStream, int channelCount, DecodeType decodeType) NN_NOEXCEPT NN_OVERRIDE;
    virtual void  Reset() NN_NOEXCEPT NN_OVERRIDE;

    virtual void ResetDecodeProfile() NN_NOEXCEPT NN_OVERRIDE;
    virtual DecodeProfile GetDecodeProfile() const NN_NOEXCEPT NN_OVERRIDE;

private:
    void* m_Buffer;

    nn::codec::HardwareOpusDecoder m_Decoder;
    nn::atk::detail::driver::OpusDecoderImpl m_OpusDecoderImpl;
};

class HardwareOpusDecoderManager : public IStreamDataDecoderManager
{
public:
    HardwareOpusDecoderManager() NN_NOEXCEPT;
    virtual ~HardwareOpusDecoderManager() NN_NOEXCEPT NN_OVERRIDE;

    void Initialize(void* buffer, size_t bufferSize, int decoderCount) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;
    size_t GetRequiredMemorySize(int decoderCount) NN_NOEXCEPT;
    bool IsInitialized() const NN_NOEXCEPT;

    virtual IStreamDataDecoder* AllocDecoder() NN_NOEXCEPT NN_OVERRIDE;
    virtual void FreeDecoder(IStreamDataDecoder* pDecoder) NN_NOEXCEPT NN_OVERRIDE;
    int GetStreamFileType() const NN_NOEXCEPT NN_OVERRIDE;
    int GetDecodeMode() const NN_NOEXCEPT NN_OVERRIDE;

private:
    size_t GetWorkBufferSizePerDecoder() NN_NOEXCEPT;

public:
    static const int DefaultOpusDecoderCount = 1;

private:
    OpusDecoderManagerImpl<HardwareOpusDecoder> m_DecoderManagerImpl;
};

}}}} // nn::atk::detail::driver
