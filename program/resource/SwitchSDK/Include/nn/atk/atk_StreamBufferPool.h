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

#include <cstddef>
#include <cstdint>
#include <nn/nn_Macro.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

class StreamBufferPool
{
public:
    void Initialize( void* buffer, size_t size, int blockCount ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    void* Alloc() NN_NOEXCEPT;
    void Free( void* pPtr ) NN_NOEXCEPT;

    size_t GetBlockSize() const NN_NOEXCEPT { return m_BlockSize; }

private:
    static const int BlockMax = 32; // 8の倍数
    static const int BitPerByte = 8;

    void*  m_Buffer;            //< バッファのアドレス
    size_t m_BufferSize;        //< バッファの全体サイズ
    size_t m_BlockSize;         //< 1 ブロックあたりのバイト数
                                //  (バッファは m_BlockCount 個のブロックに分けて管理される)
    int    m_BlockCount;        //< ブロック数 (最大ストリームチャンネル数と同じ数になる)
    int    m_AllocCount;        //< ブロックの確保数
    uint8_t     m_AllocFlags[ BlockMax / BitPerByte ];
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

