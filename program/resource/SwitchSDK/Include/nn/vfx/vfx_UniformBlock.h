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
#include <nn/vfx/vfx_Enum.h>
#include <nn/vfx/vfx_BufferAllocator.h>

namespace nn {
namespace vfx {

class Heap;

namespace detail {

//---------------------------------------------------------------------------
//! @brief  定数バッファスロット
//---------------------------------------------------------------------------
class ConstantBufferSlot
{
    NN_DISALLOW_COPY( ConstantBufferSlot );
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //---------------------------------------------------------------------------
    ConstantBufferSlot() NN_NOEXCEPT
    {
        Invalidate();
    }

    //---------------------------------------------------------------------------
    //! @brief  定数バッファ名から初期化します。
    //! @param[in] pShader             シェーダ
    //! @param[in] samplerName         定数バッファ名
    //---------------------------------------------------------------------------
    void Initialize( nn::gfx::Shader* pShader, const char* samplerName ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  終了処理
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT
    {
        Invalidate();
    }

    //---------------------------------------------------------------------------
    //! @brief  頂点シェーダでのスロットを取得します。
    //---------------------------------------------------------------------------
    int GetVertexShaderSlot() const NN_NOEXCEPT
    {
        return m_VertexShaderSlot;
    }

    //---------------------------------------------------------------------------
    //! @brief  ピクセルシェーダでのスロットを取得します。
    //---------------------------------------------------------------------------
    int GetPixelShaderSlot() const NN_NOEXCEPT
    {
        return m_PixelShaderSlot;
    }

    //---------------------------------------------------------------------------
    //! @brief  無効化
    //---------------------------------------------------------------------------
    void Invalidate() NN_NOEXCEPT
    {
        m_VertexShaderSlot = m_PixelShaderSlot = InvalidValueId_ConstantBufferSlotId;
    }

private:
    int m_VertexShaderSlot;             //!< 頂点シェーダでのスロット
    int m_PixelShaderSlot;           //!< ピクセルシェーダでのスロット
};


//---------------------------------------------------------------------------
//! @brief  コンスタントバッファのサイズを計算する
//---------------------------------------------------------------------------
class BufferSizeCalculator
{
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //---------------------------------------------------------------------------
    BufferSizeCalculator( nn::gfx::Device* pGfxDevice, int gpuAccess ) NN_NOEXCEPT
    {
        m_CalcedBufferSize      = 0;

        nn::gfx::Buffer::InfoType bfInfo;
        bfInfo.SetDefault();
        bfInfo.SetGpuAccessFlags( gpuAccess );
        m_BufferAlign = nn::gfx::Buffer::GetBufferAlignment( pGfxDevice, bfInfo );
    }

    //---------------------------------------------------------------------------
    //! @brief  計算するバッファサイズを追加します。
    //---------------------------------------------------------------------------
    void AddBufferSize( size_t size ) NN_NOEXCEPT
    {
        m_CalcedBufferSize += nn::util::align_up( size, m_BufferAlign );
    }

    //---------------------------------------------------------------------------
    //! @brief  計算するバッファサイズを追加します。
    //---------------------------------------------------------------------------
    void AddBufferSize( size_t size, int count ) NN_NOEXCEPT
    {
        m_CalcedBufferSize += ( nn::util::align_up( size, m_BufferAlign ) * count );
    }

    //---------------------------------------------------------------------------
    //! @brief  バッファサイズを取得します。
    //---------------------------------------------------------------------------
    size_t GetBufferSize() const NN_NOEXCEPT
    {
        return m_CalcedBufferSize;
    }

private:
    size_t          m_CalcedBufferSize;
    size_t          m_BufferAlign;
};


//---------------------------------------------------------------------------
//! @brief  バッファ
//!
//!         ランタイム上でワークから生成するバッファ
//---------------------------------------------------------------------------
class Buffer
{
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //---------------------------------------------------------------------------a
    Buffer() NN_NOEXCEPT : m_IsInitialized( false ) {}

    //---------------------------------------------------------------------------
    //! @brief      バッファを初期化する
    //! @param[in]  pGfxDevice      fxデバイス
    //! @param[in]  pHeap           ヒープへのポインタ
    //! @param[in]  gpuAccess       バッファタイプ
    //! @param[in]  bufferSize      1バッファのサイズ
    //---------------------------------------------------------------------------
    bool Initialize( nn::gfx::Device* pGfxDevice, nn::vfx::Heap* pHeap, int gpuAccess, size_t bufferSize ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      終了処理
    //! @param[in]  pDevice         gfxデバイス
    //! @param[in]  pHeap           ヒープへのポインタ
    //---------------------------------------------------------------------------
    void Finalize( nn::gfx::Device* pDevice, nn::vfx::Heap* pHeap ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  有効かどうか
    //! @return 有効かどうかを示すbool値
    //---------------------------------------------------------------------------
    bool IsValidate() const NN_NOEXCEPT
    {
        return m_IsInitialized;
    }

    //---------------------------------------------------------------------------
    //! @brief  バッファを取得します。
    //---------------------------------------------------------------------------
    const nn::gfx::Buffer& GetGfxBuffer() const NN_NOEXCEPT
    {
        return m_GfxBuffer;
    }

    //---------------------------------------------------------------------------
    //! @brief  メモリプールサイズを取得します。
    //---------------------------------------------------------------------------
    size_t GetMemoryPoolSize() const NN_NOEXCEPT
    {
        return m_MemoryPoolSize;
    }

    //---------------------------------------------------------------------------
    //! @brief  バッファサイズを取得します。
    //---------------------------------------------------------------------------
    size_t GetBufferSize() const NN_NOEXCEPT
    {
        return m_BufferSize;
    }

    //---------------------------------------------------------------------------
    //! @brief  バッファアラインを取得します。
    //---------------------------------------------------------------------------
    size_t GetBufferAlign() const NN_NOEXCEPT
    {
        return m_BufferAlign;
    }

private:
    nn::gfx::Buffer         m_GfxBuffer;                  //!< gfx バッファ
    size_t                  m_BufferSize;                 //!< バッファバッファサイズ
    bool                    m_IsInitialized;              //!< 初期化済みかどうか
    nn::gfx::MemoryPool     m_GfxMemoryPool;              //!< gfx メモリプール
    void*                   m_pBuffer;                    //!< バッファ用に確保した領域の先頭アドレス
    size_t                  m_MemoryPoolSize;             //!< メモリプールサイズ
    size_t                  m_BufferAlign;                //!< バッファアライン
};


//---------------------------------------------------------------------------
//! @brief  コンスタントバッファ
//!
//!         内容が書き換わらない１つのバッファをワークから生成する
//---------------------------------------------------------------------------
class ConstantBuffer
{
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //---------------------------------------------------------------------------
    ConstantBuffer() NN_NOEXCEPT
    {
        m_MappedPtr = NULL;
        m_Offset    = 0;
    }

    //---------------------------------------------------------------------------
    //! @brief      定数バッファバッファを初期化する
    //! @param[in]  pGfxDevice      fxデバイス
    //! @param[in]  heap            ヒープへのポインタ
    //! @param[in]  gpuAccess       gpu アクセスフラグ
    //! @param[in]  bufferSize      定数バッファ型のサイズ
    //---------------------------------------------------------------------------
    bool Initialize( nn::gfx::Device* pGfxDevice, nn::vfx::Heap* heap, int gpuAccess, size_t bufferSize ) NN_NOEXCEPT
    {
        if ( !m_Buffer.Initialize( pGfxDevice, heap, gpuAccess, bufferSize ) )
        {
            return false;
        }
        m_Buffer.GetGfxBuffer().GetGpuAddress( &m_GpuAddress );
        return true;
    }

    //---------------------------------------------------------------------------
    //! @brief      終了処理
    //! @param[in]  pDevice         gfxデバイス
    //! @param[in]  pHeap           ヒープへのポインタ
    //---------------------------------------------------------------------------
    void Finalize( nn::gfx::Device* pDevice, nn::vfx::Heap* pHeap ) NN_NOEXCEPT
    {
        m_Buffer.Finalize( pDevice, pHeap );
    }

    //---------------------------------------------------------------------------
    //! @brief  指定サイドのバッファを取得します。
    //! @return バッファアドレス
    //---------------------------------------------------------------------------
    void* Map() NN_NOEXCEPT
    {
        return m_Buffer.GetGfxBuffer().Map();
    }

    //---------------------------------------------------------------------------
    //! @brief  指定サイドのバッファを取得します。
    //! @return バッファアドレス
    //---------------------------------------------------------------------------
    void* Map( ptrdiff_t offset ) NN_NOEXCEPT
    {
        void* ptr = m_Buffer.GetGfxBuffer().Map();
        return nn::util::BytePtr( ptr ).Advance( offset ).Get();
    }

    //---------------------------------------------------------------------------
    //! @brief  指定サイドの定数バッファを有効化します。
    //---------------------------------------------------------------------------
    void Unmap() NN_NOEXCEPT
    {
        m_Buffer.GetGfxBuffer().FlushMappedRange(0, m_Buffer.GetBufferSize());
        m_Buffer.GetGfxBuffer().Unmap();
    }

    //---------------------------------------------------------------------------
    //! @brief  指定サイドの定数バッファを有効化します。
    //---------------------------------------------------------------------------
    void Unmap( ptrdiff_t offset, size_t size ) NN_NOEXCEPT
    {
        m_Buffer.GetGfxBuffer().FlushMappedRange(offset, size);
        m_Buffer.GetGfxBuffer().Unmap();
    }

    //---------------------------------------------------------------------------
    //! @brief  Gpuアドレスを取得します。
    //---------------------------------------------------------------------------
    inline const nn::gfx::GpuAddress& GetGpuAddress() NN_NOEXCEPT
    {
        return m_GpuAddress;
    }

    //---------------------------------------------------------------------------
    //! @brief  Gpuアドレスを取得します。
    //---------------------------------------------------------------------------
    inline void GetGpuAddress( nn::gfx::GpuAddress* address ) const NN_NOEXCEPT
    {
        *address = m_GpuAddress;
    }

    //---------------------------------------------------------------------------
    //! @brief  Gpuアドレスを取得します。
    //---------------------------------------------------------------------------
    inline void GetGpuAddress( nn::gfx::GpuAddress* address, void* addr ) const NN_NOEXCEPT
    {
        *address = m_GpuAddress;
        address->Offset( GetOffset( addr ) );
    }

    //---------------------------------------------------------------------------
    //! @brief  バッファサイズを取得します。
    //---------------------------------------------------------------------------
    size_t GetBufferSize() const NN_NOEXCEPT
    {
        if ( m_Buffer.IsValidate() )
        {
            return m_Buffer.GetBufferSize();
        }

        return 0;
    }

    //---------------------------------------------------------------------------
    //! @brief  指定されたアドレスと先頭のアドレスのオフセットを取得します。
    //---------------------------------------------------------------------------
    ptrdiff_t GetOffset( void* ptr ) const NN_NOEXCEPT
    {
        return nn::util::BytePtr( m_MappedPtr ).Distance( ptr );
    }

    //---------------------------------------------------------------------------
    //! @brief  有効かどうか
    //! @return 有効かどうかを示すbool値
    //---------------------------------------------------------------------------
    bool IsValidate() const NN_NOEXCEPT
    {
        return m_Buffer.IsValidate();
    }

    //---------------------------------------------------------------------------
    //! @brief  バッファ切り出しを開始する
    //---------------------------------------------------------------------------
    void Begin() NN_NOEXCEPT
    {
        m_Offset = 0;
        m_MappedPtr = Map( 0 );
    }

    //---------------------------------------------------------------------------
    //! @brief  サイズを指定してバッファを切り出す
    //---------------------------------------------------------------------------
    void* Cut( size_t cutSize ) NN_NOEXCEPT
    {
        void* retPtr    = nn::util::BytePtr( m_MappedPtr ).Advance( m_Offset ).Get();
        size_t size     = nn::util::align_up( cutSize, m_Buffer.GetBufferAlign() );
        if ( static_cast<size_t>( m_Offset ) + size > m_Buffer.GetBufferSize() )
        {
            return NULL;
        }
        m_Offset       += size;
        return retPtr;
    }

    //---------------------------------------------------------------------------
    //! @brief  現在のバッファを切り出し位置を取得する
    //---------------------------------------------------------------------------
    ptrdiff_t GetCurrentOffset() const NN_NOEXCEPT
    {
        return m_Offset;
    }

    //---------------------------------------------------------------------------
    //! @brief  バッファアライメント値を取得する
    //---------------------------------------------------------------------------
    size_t GetBufferAlign() const NN_NOEXCEPT
    {
        return m_Buffer.GetBufferAlign();
    }

    //---------------------------------------------------------------------------
    //! @brief  バッファ切り出しを終了する
    //---------------------------------------------------------------------------
    void End() NN_NOEXCEPT
    {
        NN_SDK_ASSERT( static_cast<size_t>( m_Offset ) <= m_Buffer.GetBufferSize() );
        Unmap();    // TODO : 不必要かも
    }

private:
    Buffer                              m_Buffer;               //!< バッファクラス
    nn::gfx::GpuAddress                 m_GpuAddress;           //!< Gfx gpuアドレス
    ptrdiff_t                           m_Offset;               //!< 切り出し用オフセット
    void*                               m_MappedPtr;            //!< 切り出し中にマップしたアドレス
};


//---------------------------------------------------------------------------
//! @brief  コンスタントバッファ
//!
//!         BufferAllocator からバッファから生成する
//---------------------------------------------------------------------------
class DynamicAllocateBuffer
{
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //---------------------------------------------------------------------------
    DynamicAllocateBuffer() NN_NOEXCEPT {}

    //---------------------------------------------------------------------------
    //! @brief      定数バッファバッファを初期化する
    //! @param[in]  bufferAllocator     Gpuバッファアロケータ
    //! @param[in]  bufferSize          定数バッファ型のサイズ
    //---------------------------------------------------------------------------
    bool Initialize( nn::vfx::detail::BufferAllocator* bufferAllocator, size_t bufferSize ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( bufferAllocator );

        m_pBufferAllocator  = bufferAllocator;
        m_AllocatedBufferSize = bufferSize;
        m_MappedPtr         = m_pBufferAllocator->Alloc( m_AllocatedBufferSize, m_pBufferAllocator->GetBufferAlign() );

        if ( m_MappedPtr )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //---------------------------------------------------------------------------
    //! @brief      終了処理
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT
    {
        if ( m_MappedPtr )
        {
            m_pBufferAllocator->Free( m_MappedPtr, false );
            m_MappedPtr = NULL;
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  バッファ切り出しを開始する
    //---------------------------------------------------------------------------
    void Begin() NN_NOEXCEPT
    {
        m_Offset = 0;
    }

    //---------------------------------------------------------------------------
    //! @brief  サイズを指定してバッファを切り出す
    //---------------------------------------------------------------------------
    void* Cut( size_t cutSize ) NN_NOEXCEPT
    {
        void* retPtr    = nn::util::BytePtr( m_MappedPtr ).Advance( m_Offset ).Get();
        size_t size     = nn::util::align_up( cutSize, m_pBufferAllocator->GetBufferAlign() );
        if ( static_cast<size_t>( m_Offset ) + size > m_AllocatedBufferSize )
        {
            return NULL;
        }
        m_Offset       += size;
        return retPtr;
    }

    //---------------------------------------------------------------------------
    //! @brief  現在のバッファを切り出し位置を取得する
    //---------------------------------------------------------------------------
    ptrdiff_t GetCurrentOffset() const NN_NOEXCEPT
    {
        return m_Offset;
    }

    //---------------------------------------------------------------------------
    //! @brief  バッファ切り出しを終了する
    //---------------------------------------------------------------------------
    void End() NN_NOEXCEPT
    {
        NN_SDK_ASSERT( static_cast<size_t>( m_Offset ) <= m_AllocatedBufferSize );
    }

    //---------------------------------------------------------------------------
    //! @brief  Gpuアドレスを取得します。
    //---------------------------------------------------------------------------
    void GetGpuAddress( nn::gfx::GpuAddress* address, void* addr ) const NN_NOEXCEPT
    {
        m_pBufferAllocator->GetGpuAddress( address, addr );
    }

    //---------------------------------------------------------------------------
    //! @brief  確保したバッファサイズを取得します。
    //! @return     確保したバッファサイズ
    //---------------------------------------------------------------------------
    size_t GetAllocatedBufferSize() const NN_NOEXCEPT
    {
        return m_AllocatedBufferSize;
    }

    //---------------------------------------------------------------------------
    //! @brief      （※旧 API ）確保したバッファサイズを取得します。
    //! @deprecated 旧型式の API 名です。nn::vfx::detail::DynamicAllocateBuffer::GetAllocatedBufferSize() に乗り換えてください。
    //! @return     確保したバッファサイズ
    //! @see        nn::vfx::detail::DynamicAllocateBuffer::GetAllocatedBufferSize()
    //---------------------------------------------------------------------------
    NN_DEPRECATED size_t GetAllocedBufferSize() const NN_NOEXCEPT
    {
        return GetAllocatedBufferSize();
    }


private:
    void*                               m_MappedPtr;            //!< 切り出し中にマップしたアドレス
    ptrdiff_t                           m_Offset;               //!< 切り出し用オフセット
    nn::vfx::detail::BufferAllocator*   m_pBufferAllocator;     //!< バッファアロケータ( 指定時に利用 )
    size_t                              m_AllocatedBufferSize;    //!< 切り出したバッファサイズ
};

} // namespace detail


} // namespace nn
} // namespace vfx
