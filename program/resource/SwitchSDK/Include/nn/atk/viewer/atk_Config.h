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

#include <nn/atk/atk_Config.h>
#ifdef NN_ATK_CONFIG_ENABLE_DEV

#include <nn/audio/audio_MemoryPool.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

class Limits
{
public:
    static const int MaxFilePath = 256; //!< パスの長さの最大値です。
};

class Alignments
{
public:
    static const size_t FileIoBuffer = 256; //!< ファイル入出力バッファのアライメントです。
    static const int WaveBuffer = nn::audio::BufferAlignSize; //!< WaveBuffer が要求するアラインメントです。
    static const int AudioMemoryPool = nn::audio::MemoryPoolType::AddressAlignment; //!< メモリプール管理することが必要なバッファが満たすべきアライメントです。
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

namespace nn {
namespace atk {
namespace viewer {

//---------------------------------------------------------------------------
//! @brief  【β版】sndctrl の制限値をまとめるクラスです。
//---------------------------------------------------------------------------
class Limits
{
public:
    //---------------------------------------------------------------------------
    // エフェクトバス数
    //---------------------------------------------------------------------------
    static const int EffectBusCountMax = 3;  //!< エフェクトバスの最大数です。

    //---------------------------------------------------------------------------
    // トラック数
    //---------------------------------------------------------------------------
    static const int StreamTrackCountMax = 4;    //!< ストリームトラックの最大数です。
    static const int SequenceTrackCountMax = 16; //!< シーケンストラックの最大数です。
};

} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
