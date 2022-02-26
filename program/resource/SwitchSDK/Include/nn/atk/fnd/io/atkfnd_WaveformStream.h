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
#include <nn/atk/fnd/model/atkfnd_WaveFormat.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief  波形バイナリストリームを入力します。
//---------------------------------------------------------------------------
class WaveformStreamReader
{
    NN_DISALLOW_COPY(WaveformStreamReader);

public: // デストラクタ
    virtual ~WaveformStreamReader() NN_NOEXCEPT { }

protected: // コンストラクタ
    WaveformStreamReader() NN_NOEXCEPT { }

public: // メソッド
    virtual void Close() NN_NOEXCEPT = 0;

    virtual bool IsOpened() const NN_NOEXCEPT = 0;

    virtual bool IsHeaderRead() const NN_NOEXCEPT = 0;

    virtual FndResult ReadHeader() NN_NOEXCEPT = 0;
    virtual size_t ReadFrames(void* buffer, size_t frames, FndResult* result = NULL) NN_NOEXCEPT = 0;

    virtual const WaveFormat& GetWaveFormat() const NN_NOEXCEPT = 0;
    virtual uint8_t GetBlockAlignment() const NN_NOEXCEPT = 0;
    virtual size_t GetDataSize() const NN_NOEXCEPT = 0;
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
