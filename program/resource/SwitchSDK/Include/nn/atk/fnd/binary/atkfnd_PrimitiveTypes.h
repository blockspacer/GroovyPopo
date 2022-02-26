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
#include <nn/atk/fnd/basis/atkfnd_Endian.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
// ターゲット向け数値型の定義
//---------------------------------------------------------------------------
#if defined(NN_ATK_FND_SHOULD_REVERSE_ENDIAN)

// 実機向け数値へのアクセサ
typedef bool                          BinBool;
typedef uint8_t                            BinU8;
typedef int8_t                            BinS8;
typedef endian::ReverseEndianNum<uint16_t> BinU16;
typedef endian::ReverseEndianNum<int16_t> BinS16;
typedef endian::ReverseEndianNum<uint32_t> BinU32;
typedef endian::ReverseEndianNum<int32_t> BinS32;
typedef endian::ReverseEndianNum<float> BinF32;
typedef endian::ReverseEndianNum<uint64_t> BinU64;
typedef endian::ReverseEndianNum<int64_t> BinS64;
typedef endian::ReverseEndianNum<double> BinF64;

#else

// 実機向け数値へのアクセサ
typedef bool BinBool;
typedef uint8_t   BinU8;
typedef int8_t   BinS8;
typedef uint16_t  BinU16;
typedef int16_t  BinS16;
typedef uint32_t  BinU32;
typedef int32_t  BinS32;
typedef float  BinF32;
typedef uint32_t  BinU64;
typedef int32_t  BinS64;
typedef double  BinF64;

#endif // NN_ATK_FND_SHOULD_REVERSE_ENDIAN

//---------------------------------------------------------------------------
// PC向け数値型の定義
// 実行環境が BIG_ENDIAN ならエンディアン変換を有効化する
//---------------------------------------------------------------------------
#if defined(NN_ATK_FND_IS_LITTLE_ENDIAN)

// PC 上のリトルエンディアン数値へのアクセサ
typedef bool PcBinBool;
typedef uint8_t   PcBinU8;
typedef int8_t   PcBinS8;
typedef uint16_t  PcBinU16;
typedef int16_t  PcBinS16;
typedef uint32_t  PcBinU32;
typedef int32_t  PcBinS32;
typedef float  PcBinF32;
typedef uint32_t  PcBinU64;
typedef int32_t  PcBinS64;
typedef double  PcBinF64;

// PC 上のビッグエンディアン数値へのアクセサ
typedef bool                          PcBeBinBool;
typedef uint8_t                            PcBeBinU8;
typedef int8_t                            PcBeBinS8;
typedef endian::ReverseEndianNum<uint16_t> PcBeBinU16;
typedef endian::ReverseEndianNum<int16_t> PcBeBinS16;
typedef endian::ReverseEndianNum<uint32_t> PcBeBinU32;
typedef endian::ReverseEndianNum<int32_t> PcBeBinS32;
typedef endian::ReverseEndianNum<float> PcBeBinF32;
typedef endian::ReverseEndianNum<uint64_t> PcBeBinU64;
typedef endian::ReverseEndianNum<int64_t> PcBeBinS64;
typedef endian::ReverseEndianNum<double> PcBeBinF64;

#else

// PC 上のリトルエンディアン数値へのアクセサ
typedef bool                          PcBinBool;
typedef uint8_t                            PcBinU8;
typedef int8_t                            PcBinS8;
typedef endian::ReverseEndianNum<uint16_t> PcBinU16;
typedef endian::ReverseEndianNum<int16_t> PcBinS16;
typedef endian::ReverseEndianNum<uint32_t> PcBinU32;
typedef endian::ReverseEndianNum<int32_t> PcBinS32;
typedef endian::ReverseEndianNum<float> PcBinF32;
typedef endian::ReverseEndianNum<uint64_t> PcBinU64;
typedef endian::ReverseEndianNum<int64_t> PcBinS64;
typedef endian::ReverseEndianNum<double> PcBinF64;

// PC 上のビッグエンディアン数値へのアクセサ
typedef bool PcBeBinBool;
typedef uint8_t   PcBeBinU8;
typedef int8_t   PcBeBinS8;
typedef uint16_t  PcBeBinU16;
typedef int16_t  PcBeBinS16;
typedef uint32_t  PcBeBinU32;
typedef int32_t  PcBeBinS32;
typedef float  PcBeBinF32;
typedef uint32_t  PcBeBinU64;
typedef int32_t  PcBeBinS64;
typedef double  PcBeBinF64;

#endif // NN_ATK_FND_IS_LITTLE_ENDIAN

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
