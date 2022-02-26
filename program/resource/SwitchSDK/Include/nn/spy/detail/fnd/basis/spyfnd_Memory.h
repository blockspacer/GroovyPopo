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

#include <nn/spy/detail/fnd/basis/spyfnd_Config.h>

#include <nn/spy/detail/fnd/basis/spyfnd_Inlines.h>

namespace nn {
namespace spy {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @brief  メモリ特性です。
//---------------------------------------------------------------------------
class MemoryTraits
{
public:
    static const int DefaultAlignment = 4; //!< デフォルトのアライメント
};

//---------------------------------------------------------------------------
//! @brief  前から順にスタック方式でメモリを割り当てるヒープクラスです。
//---------------------------------------------------------------------------
class FrameHeap
{
    NN_DISALLOW_COPY(FrameHeap);

public: // コンストラクタ
    FrameHeap() NN_NOEXCEPT :
        m_Buffer(NULL),
        m_BufferLength(0),
        m_UsedLength(0)
    {
    }

    virtual ~FrameHeap() NN_NOEXCEPT
    {
        Finalize();
    }

public: // メソッド
    void Initialize(void* buffer, size_t length) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    bool IsInitialized() const NN_NOEXCEPT
    {
        return m_Buffer != NULL;
    }

    size_t GetUsedLength() const NN_NOEXCEPT { return m_UsedLength; }

    size_t GetFreeLength(int alignment = MemoryTraits::DefaultAlignment) const NN_NOEXCEPT;

    virtual void* Allocate(size_t size, int alignment = MemoryTraits::DefaultAlignment) NN_NOEXCEPT;

    void Clear() NN_NOEXCEPT
    {
        m_UsedLength = 0;
    }

private: // メンバ変数
    void*  m_Buffer;         //!< バッファです。
    size_t m_BufferLength;   //!< バッファの長さです。
    size_t m_UsedLength;     //!< 使用中のメモリサイズです。
};

} // namespace nn::spy::detail::fnd
} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn
