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
#include <nn/util/util_IntrusiveList.h>

namespace nn  {
namespace vfx {
namespace detail {

class BufferAllocator
{

public:
    //! @brief Initialize() の引数クラスです。
    class InitializeArg
    {
    public:
        //! @brief コンストラクタです。
        InitializeArg() NN_NOEXCEPT
            : m_pGfxDevice( NULL )
            , m_MemoryPoolProperty( nn::gfx::MemoryPoolProperty_CpuUncached | nn::gfx::MemoryPoolProperty_GpuCached )
            , m_GpuAccessFlag( nn::gfx::GpuAccess_VertexBuffer )
            , m_pManagementBuffer( NULL )
            , m_ManagementBufferSize( 0 )
            , m_pMemoryPoolBuffer( NULL )
            , m_MemoryPoolBufferSize( 0 )
        {}

        nn::gfx::Device*            m_pGfxDevice;
        int                         m_MemoryPoolProperty;
        int                         m_GpuAccessFlag;
        void*                       m_pManagementBuffer;
        size_t                      m_ManagementBufferSize;
        void*                       m_pMemoryPoolBuffer;
        size_t                      m_MemoryPoolBufferSize;


        //---------------------------------------------------------------------------
        //! @brief  ヒープの管理領域のサイズを取得する
        //! @param[in]  allocatedCountMax 同時に確保可能な回数。
        //---------------------------------------------------------------------------
        static size_t CalculateManagementBufferSize( int allocatedCountMax ) NN_NOEXCEPT
        {
            return sizeof( BufferAllocator::Block ) * allocatedCountMax + 32;
        }

        //---------------------------------------------------------------------------
        //! @brief  メモリプールバッファのメモリアライメントを取得する
        //! @param[in]  pGfxDevice nn::gfx グラフィックスデバイス
        //! @param[in]  cpuPageProperty nn::gfx グラフィックスデバイス
        //---------------------------------------------------------------------------
        static size_t CalculateMemoryPoolBufferAlignment( nn::gfx::Device* pGfxDevice, int memoryPoolProperty ) NN_NOEXCEPT
        {
            nn::gfx::MemoryPool::InfoType mpInfo;
            mpInfo.SetDefault();
            mpInfo.SetMemoryPoolProperty( memoryPoolProperty );
            return nn::gfx::MemoryPool::GetPoolMemoryAlignment( pGfxDevice, mpInfo );
        }

        //---------------------------------------------------------------------------
        //! @brief  メモリプールバッファのサイズを取得する
        //---------------------------------------------------------------------------
        static size_t CalculateMemoryPoolBufferSize( nn::gfx::Device* pGfxDevice, int memoryPoolProperty, size_t memoryPoolBufferSize ) NN_NOEXCEPT
        {
            nn::gfx::MemoryPool::InfoType mpInfo;
            mpInfo.SetDefault();
            mpInfo.SetMemoryPoolProperty( memoryPoolProperty );
            return nn::util::align_up( memoryPoolBufferSize, nn::gfx::MemoryPool::GetPoolMemorySizeGranularity( pGfxDevice, mpInfo ) );
        }
    };


public:
    //---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //---------------------------------------------------------------------------
    BufferAllocator() NN_NOEXCEPT
    {
        m_BlockArray    = NULL;
        m_Start         = NULL;
        m_AllocatedCount    = 0;
        m_FreeBlock     = NULL;
    }

    //---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //---------------------------------------------------------------------------
    virtual ~BufferAllocator() NN_NOEXCEPT {}

    //---------------------------------------------------------------------------
    //! @brief  初期化
    //---------------------------------------------------------------------------
    bool Initialize( InitializeArg& arg ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  破棄
    //---------------------------------------------------------------------------
    virtual void Finalize( nn::gfx::Device* pGfxDevice ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ヒープからメモリ確保を行います。
    //! @param[in]  size 確保サイズ。
    //! @param[in]  alignment アライメント。
    //---------------------------------------------------------------------------
    void* Alloc( size_t size, size_t alignment ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  メモリブロックをヒープへ返却します。
    //! @param[in]  ptr 返却するメモリのポインタ
    //! @param[in]  immediate Freeを即時に実行するかどうか
    //---------------------------------------------------------------------------
    void Free( void* ptr, bool immediate ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  返却リストブロックをヒープへ返却します。
    //---------------------------------------------------------------------------
    void FlushFreeList() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  Gpuアドレスを取得します。
    //! @param[in]  gpuAddress Gpuアドレス値
    //---------------------------------------------------------------------------
    void GetGpuAddress( nn::gfx::GpuAddress* gpuAddress ) const NN_NOEXCEPT
    {
        m_GfxBuffer.GetGpuAddress( gpuAddress );
    }

    //---------------------------------------------------------------------------
    //! @brief  Gpuアドレスを取得します。
    //! @param[in]  gpuAddress Gpuアドレス値
    //! @param[in]  addr Gpuアドレス値を取得するアドレス
    //---------------------------------------------------------------------------
    inline void GetGpuAddress( nn::gfx::GpuAddress* gpuAddress, void* addr ) const NN_NOEXCEPT
    {
        *gpuAddress = m_GpuAddress;
        gpuAddress->Offset( nn::util::BytePtr( m_Start ).Distance( addr ) );
    }

    //---------------------------------------------------------------------------
    //! @brief  バッファアラインを取得します。
    //---------------------------------------------------------------------------
    size_t GetBufferAlign() const NN_NOEXCEPT
    {
        return m_BufferAlign;
    }

    //---------------------------------------------------------------------------
    //! @brief  現在のメモリ確保数を取得します。
    //! @return 現在のメモリ確保数
    //---------------------------------------------------------------------------
    int GetAllocatedCount() const NN_NOEXCEPT
    {
        return m_AllocatedCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  現在のメモリ確保サイズを取得します。
    //! @return 現在のメモリ確保サイズ
    //---------------------------------------------------------------------------
    size_t GetAllocatedSize() const NN_NOEXCEPT
    {
        return m_AllocatedSize;
    }

    //---------------------------------------------------------------------------
    //! @brief  確保可能なメモリサイズを取得します。
    //---------------------------------------------------------------------------
    size_t GetSize() const NN_NOEXCEPT
    {
        return m_Size;
    }

    //---------------------------------------------------------------------------
    //! @brief  リリースカウンタを設定します。
    //! @param[in]  releaseCounter 設定するリリースカウンタ。デフォルト値は 2 になっています。
    //---------------------------------------------------------------------------
    void SetReleaseCounter( int releaseCounter ) NN_NOEXCEPT
    {
        m_ReleaseCounter = releaseCounter;
    }

private:

    // リリースカウンタのデフォルト値
    enum
    {
        _RELEASE_COUNTER = 2,
    };

    //---------------------------------------------------------------------------
    //! @brief  メモリブロックをヒープへ返却します。
    //! @param[in]  ptr 返却するメモリのポインタ
    //---------------------------------------------------------------------------
    void _Free( void* ptr ) NN_NOEXCEPT;

    class Block
    {
    public:
        Block() NN_NOEXCEPT
        {
            m_Addr  = NULL;
            m_Size  = 0;
            m_ReleaseCounter = _RELEASE_COUNTER;
        }

        int                         m_ReleaseCounter;               //!< リリースカウンタ
        void*                       m_Addr;                         //!< ブロックを開始するアドレス
        size_t                      m_Size;                         //!< ブロックのサイズ
        nn::util::IntrusiveListNode m_Link;                         //!< 利用中リスト
        nn::util::IntrusiveListNode m_FreeLink;                     //!< 返却リスト
    };

    typedef nn::util::IntrusiveList<Block, nn::util::IntrusiveListMemberNodeTraits<Block, &Block::m_Link> >     BlockList;
    typedef nn::util::IntrusiveList<Block, nn::util::IntrusiveListMemberNodeTraits<Block, &Block::m_FreeLink> > FreeBlockList;


    nn::gfx::Buffer         m_GfxBuffer;            //!< gfx バッファ
    nn::gfx::MemoryPool     m_GfxMemoryPool;        //!< gfx メモリプール
    void*                   m_Start;                //!< ヒープの開始位置アドレス
    size_t                  m_Size;                 //!< ヒープサイズ
    BlockList               m_BlockList;            //!< 使用中ブロック配列リスト
    FreeBlockList           m_FreeBlockList;        //!< 返却リスト
    int                     m_ReleaseCounter;       //!< リリースカウンタ
    Block*                  m_BlockArray;           //!< ブロックの配列
    int                     m_AllocatedCount;       //!< 確保数
    int                     m_AllocatedCountMax;    //!< 最大アロック数
    size_t                  m_AllocatedSize;        //!< 現在の確保したメモリサイズ
    int                     m_AllocatedPtr;         //!< Allocしたポインタのインデックス
    size_t                  m_BufferAlign;          //!< バッファアライン
    Block*                  m_FreeBlock;            //!< 空き領域
    nn::gfx::GpuAddress     m_GpuAddress;           //!< 先頭Gpuアドレス
};


} // namespace detail
} // namespace vfx
} // namespace nn

