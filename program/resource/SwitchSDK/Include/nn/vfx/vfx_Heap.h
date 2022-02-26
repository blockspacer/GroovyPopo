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

#include <nn/vfx/vfx_TargetDef.h>
#include <nn/mem/mem_StandardAllocator.h>

namespace nn {
namespace vfx {

//---------------------------------------------------
//! @brief エフェクト専用のヒープクラスです。
//!
//! メモリ確保、メモリ解放メソッドを派生クラスにて実装する必要があります。
//! エフェクトシステム生成時に渡す、nw::vfx::Config に実体を設定する必要があります。
//---------------------------------------------------
class Heap
{
    NN_DISALLOW_COPY( Heap );
public:
    //----------------------------------------
    //! @name コンストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief        メモリ管理を行う為のインターフェースクラスです。
    //!
    //!               各ご使用の環境に合わせて派生クラス側にて実装をお願いします。
    //---------------------------------------------------------------------------
    Heap() NN_NOEXCEPT
    {
    }

    //---------------------------------------------------------------------------
    //! @brief        デストラクタです。
    //---------------------------------------------------------------------------
    virtual ~Heap() NN_NOEXCEPT
    {
    }

    //@}


    //----------------------------------------
    //! @name メモリ確保/破棄
    //@{

    //! @brief メモリ確保時のアライメントです。
    enum Alignment
    {
        Alignment_Default = 128    //!< デフォルトのアライメントサイズ
    };

    //---------------------------------------------------------------------------
    //! @brief                  メモリ確保を行います。
    //!                         各ご使用の環境に合わせて派生クラス側にて実装をお願いします。
    //! @param[in] size         確保するメモリサイズ
    //! @param[in] alignment    アライメント
    //! @return                 確保したメモリ領域へのポインタ
    //---------------------------------------------------------------------------
    virtual void* Alloc( size_t size, size_t alignment = Alignment_Default ) NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief          メモリ破棄を行います。
    //!
    //!                 各ご使用の環境に合わせて派生クラス側にて実装をお願いします。
    //! @param[in] ptr  破棄するメモリアドレス
    //---------------------------------------------------------------------------
    virtual void Free( void* ptr ) NN_NOEXCEPT = 0;

    //@}
};

//---------------------------------------------------------------------------
//! @brief  nn::mem::StandardAllocatorを使用したヒープ
//---------------------------------------------------------------------------
class StandardHeap : public nn::vfx::Heap
{
    NN_DISALLOW_COPY( StandardHeap );
public:
    //---------------------------------------------------------------------------
    //! @brief        コンストラクタです。
    //---------------------------------------------------------------------------
    StandardHeap() NN_NOEXCEPT
                   : m_IsCreated(false)
                   , m_StartAddress(nullptr) {}

    //---------------------------------------------------------------------------
    //! @brief  デストラクタです。
    //---------------------------------------------------------------------------
    ~StandardHeap() NN_NOEXCEPT
    {
        Finalize();
    }

    //---------------------------------------------------------------------------
    //! @brief                ヒープを生成する
    //! @param[in] ptr        確保したメモリアドレス
    //! @param[in] memSize    確保したメモリサイズ
    //---------------------------------------------------------------------------
    void Initialize( void* ptr, size_t memSize ) NN_NOEXCEPT
    {
        m_StartAddress = ptr;
        NN_SDK_ASSERT_NOT_NULL(m_StartAddress);

        m_StandardAllocator.Initialize(m_StartAddress, memSize);
        m_IsCreated = true;
    }

    //---------------------------------------------------------------------------
    //! @brief                  メモリを確保します。
    //! @param[in] size         確保するメモリサイズ
    //! @param[in] alignment    アライメントサイズ
    //! @return                 確保した領域の先頭アドレス
    //---------------------------------------------------------------------------
    void* Alloc( size_t size, size_t alignment = nn::vfx::Heap::Alignment_Default ) NN_NOEXCEPT
    {
       if (m_IsCreated == false)
        {
            return nullptr;
        }
        return m_StandardAllocator.Allocate(size, alignment);
    }

    //---------------------------------------------------------------------------
    //! @brief         メモリを解放します。
    //! @param[in] ptr 解放する領域の先頭アドレス
    //---------------------------------------------------------------------------
    void Free( void* ptr ) NN_NOEXCEPT
    {
        if (m_IsCreated == false)
        {
            return;
        }
        m_StandardAllocator.Free(ptr);
    }

    //---------------------------------------------------------------------------
    //! @brief  終了処理
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT
    {
        if (m_IsCreated)
        {
            m_StandardAllocator.Finalize();
            m_IsCreated = false;
            m_StartAddress = nullptr;
        }
    }

private:
    nn::mem::StandardAllocator  m_StandardAllocator;    //!< スタンダードアロケータ
    bool                        m_IsCreated;            //!< 生成済みかどうか
    void*                       m_StartAddress;         //!< ヒープ先頭アドレス
};
namespace detail {

//---------------------------------------------------------------------------
//! @brief  確保合計サイズをカウントアップするヒープ
//---------------------------------------------------------------------------
class CalculateAllocatedSizeHeap : public nn::vfx::Heap
{
    NN_DISALLOW_COPY( CalculateAllocatedSizeHeap );
public:
    //---------------------------------------------------------------------------
    //! @brief        コンストラクタです。
    //---------------------------------------------------------------------------
    CalculateAllocatedSizeHeap() NN_NOEXCEPT
    {
        m_pBaseHeap     = NULL;
        m_AllocatedSize   = 0;
        m_AllocatedCount  = 0;
    }

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //! @param[in] pHeap ヒープへのポインタ
    //---------------------------------------------------------------------------
    explicit CalculateAllocatedSizeHeap( nn::vfx::Heap* pHeap ) NN_NOEXCEPT
    {
        m_pBaseHeap     = pHeap;
        m_AllocatedSize   = 0;
        m_AllocatedCount  = 0;
    }

    //---------------------------------------------------------------------------
    //! @brief  ヒープを設定します。
    //! @param[in] pHeap ヒープへのポインタ
    //---------------------------------------------------------------------------
    void SetHeap( nn::vfx::Heap* pHeap ) NN_NOEXCEPT
    {
        m_pBaseHeap = pHeap;
    }

    //---------------------------------------------------------------------------
    //! @brief                  メモリを確保します。
    //! @param[in] size         確保するメモリサイズ
    //! @param[in] alignment    アライメントサイズ
    //! @return                 確保した領域の先頭アドレス
    //---------------------------------------------------------------------------
    virtual void* Alloc( size_t size, size_t alignment = nn::vfx::Heap::Alignment_Default ) NN_NOEXCEPT
    {
        void* ptr = m_pBaseHeap->Alloc( size, alignment );
        m_AllocatedSize += size;
        m_AllocatedCount++;
        return ptr;
    }

    //---------------------------------------------------------------------------
    //! @brief  メモリを解放します。
    //! @param[in] ptr 解放する領域の先頭アドレス
    //---------------------------------------------------------------------------
    virtual void Free( void* ptr ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( ptr );
        NN_SDK_ASSERT( m_AllocatedCount != 0 );
        m_AllocatedCount--;
        return m_pBaseHeap->Free( ptr );
    }

    //---------------------------------------------------------------------------
    //! @brief  メモリ確保サイズを取得します。
    //! @return 確保済みのメモリサイズ
    //---------------------------------------------------------------------------
    size_t GetAllocatedSize() const NN_NOEXCEPT
    {
        return m_AllocatedSize;
    }

    //---------------------------------------------------------------------------
    //! @brief  メモリ確保回数を取得します。
    //! @return メモリ確保回数
    //---------------------------------------------------------------------------
    size_t GetAllocatedCount() const NN_NOEXCEPT
    {
        return m_AllocatedCount;
    }

private:
    nn::vfx::Heap*  m_pBaseHeap;        //!< ヒープ
    size_t          m_AllocatedSize;    //!< 確保済みメモリサイズ
    int             m_AllocatedCount;   //!< 確保回数
};

} // namespace detail

} // namespace vfx
} // namespace nn

