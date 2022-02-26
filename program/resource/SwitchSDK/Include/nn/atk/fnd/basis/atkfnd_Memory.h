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
#include <nn/lmem/lmem_ExpHeap.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief  メモリ特性です。
//---------------------------------------------------------------------------
class MemoryTraits
{
public:
    static const int DefaultAlignment = 4; //!< デフォルトのアライメント
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  メモリアロケータのインターフェイスです。
//---------------------------------------------------------------------------
class IAllocator
{
public:
    virtual ~IAllocator() NN_NOEXCEPT { }

public:
    virtual void* Alloc(size_t size, int alignment = MemoryTraits::DefaultAlignment) NN_NOEXCEPT = 0;
    virtual void Free(void* ptr) NN_NOEXCEPT = 0;
};

//---------------------------------------------------------------------------
//! @internal
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

    virtual void* Alloc(size_t size, int alignment = MemoryTraits::DefaultAlignment) NN_NOEXCEPT;

    void Clear() NN_NOEXCEPT
    {
        m_UsedLength = 0;
    }

private: // メンバ変数
    void*  m_Buffer;         //!< バッファです。
    size_t m_BufferLength;   //!< バッファの長さです。
    size_t m_UsedLength;     //!< 使用中のメモリサイズです。
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  固定長ユニット単位でメモリを割り当てるヒープクラスです。
//---------------------------------------------------------------------------
class UnitHeap : public IAllocator
{
    NN_DISALLOW_COPY(UnitHeap);

public: // コンストラクタ
    UnitHeap() NN_NOEXCEPT :
        m_Buffer(NULL),
        m_BufferLength(0),
        m_UnitLength(0),
        m_AllocatedUnitCount(0),
        m_FirstFreeUnit(NULL)
    {
    }

    virtual ~UnitHeap() NN_NOEXCEPT NN_OVERRIDE
    {
        Finalize();
    }

public: // メソッド
    void Initialize(
        void* buffer,
        size_t length,
        size_t unitLength,
        int unitCount,
        int unitAlignment = MemoryTraits::DefaultAlignment) NN_NOEXCEPT;

    void Finalize() NN_NOEXCEPT;

    bool IsInitialized() const NN_NOEXCEPT
    {
        return m_Buffer != NULL;
    }

    size_t GetBufferLength() const NN_NOEXCEPT
    {
        return m_BufferLength;
    }

    size_t GetUsedLength() const NN_NOEXCEPT { return m_AllocatedUnitCount * m_UnitLength; }

    virtual void* Alloc(size_t size = 0, int alignment = MemoryTraits::DefaultAlignment) NN_NOEXCEPT NN_OVERRIDE;
    virtual void Free(void* ptr) NN_NOEXCEPT NN_OVERRIDE;

private: // メソッド
    void* GetNextFreeUnit(void* unit) const NN_NOEXCEPT;

private: // メンバ変数
    void*  m_Buffer;               //!< バッファです。
    size_t m_BufferLength;         //!< バッファの長さです。
    size_t m_UnitLength;           //!< ユニットサイズです。
    int    m_AllocatedUnitCount;   //!< 割り当て済みユニット数です。
    void*  m_FirstFreeUnit;        //!< 最初の空きユニットです。
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  拡張ヒープマネージャを利用したヒープクラスです。
//---------------------------------------------------------------------------
class ExpandedHeap : public IAllocator
{
    NN_DISALLOW_COPY(ExpandedHeap);

public: // コンストラクタ
    ExpandedHeap() NN_NOEXCEPT : m_HeapHandle(NULL) { }

    virtual ~ExpandedHeap() NN_NOEXCEPT NN_OVERRIDE
    {
        Finalize();
    }

public: // メソッド
    bool Initialize(void* buffer, size_t length) NN_NOEXCEPT;

    void Finalize() NN_NOEXCEPT;

    bool IsInitialized() const NN_NOEXCEPT
    {
        return m_HeapHandle != NULL;
    }

    virtual void* Alloc(size_t size, int alignment = MemoryTraits::DefaultAlignment) NN_NOEXCEPT NN_OVERRIDE;
    virtual void Free(void* ptr) NN_NOEXCEPT NN_OVERRIDE;

public:
    static const size_t BlockMdSize =
        sizeof(nn::lmem::detail::ExpHeapMemoryBlockHead);

    // 先頭のRoundUp + 末尾のRoundDown + フリーリストのブロックヘッダサイズ + 拡張ヒープ全体のヘッダサイズ
    static const size_t HeapMdSize =
        4 + 4 +
        BlockMdSize +
        sizeof(nn::lmem::detail::HeapHead) -
        sizeof(nn::lmem::detail::SpecificHeapHead) +
        sizeof(nn::lmem::detail::ExpHeapHead);

private: // メンバ変数
     nn::lmem::HeapHandle m_HeapHandle;
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
