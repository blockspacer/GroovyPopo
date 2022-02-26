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
#include <nn/vfx/vfx_Heap.h>
#include <nn/vfx/vfx_UniformBlock.h>

namespace nn {
namespace vfx {
namespace detail {



//---------------------------------------------------------------------------
//! @brief  アトリビュート
//---------------------------------------------------------------------------
class Attribute : public DynamicAllocateBuffer
{
    NN_DISALLOW_COPY( Attribute );
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //---------------------------------------------------------------------------
    Attribute() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  初期化
    //! @param[in] bufferAllocator       バッファアロケータ
    //! @param[in] blockCount            バッファ数
    //! @param[in] blockSize             1バッファのサイズ
    //---------------------------------------------------------------------------
    bool Initialize( nn::vfx::detail::BufferAllocator* bufferAllocator, int blockCount, size_t blockSize ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  終了処理
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  Gpuアドレスを取得します。
    //! @return Gpuアドレスへの参照
    //---------------------------------------------------------------------------
    const nn::gfx::GpuAddress& GetGpuAddress( BufferSide side ) const;

    //---------------------------------------------------------------------------
    //! @brief  指定サイドのバッファを取得します。
    //! @param[in] side             取得する面
    //! @return バッファアドレス
    //---------------------------------------------------------------------------
    void* Map( BufferSide side ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  指定サイドのバッファを有効化します。
    //---------------------------------------------------------------------------
    void Unmap() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ブロックサイズを取得します。
    //---------------------------------------------------------------------------
    size_t GetBlockSize() const NN_NOEXCEPT
    {
        return m_BlockSize;
    }

    //---------------------------------------------------------------------------
    //! @brief  有効かどうか
    //! @return trueで有効、falseで無効
    //---------------------------------------------------------------------------
    bool IsValidate() const NN_NOEXCEPT
    {
        if( m_BufferAddr[BufferSide_FrontBuffer] )
        {
            return true;
        }
        return false;
    }

private:
    nn::gfx::GpuAddress     m_GpuAddress[BufferSide_MaxBufferSide];     //!< Gfx gpuアドレス
    void*                   m_BufferAddr[BufferSide_MaxBufferSide];     //!< cpuがアクセス可能なアドレス
    uint32_t                m_Side;                                     //!< 使用中のバッファインデックス
    size_t                  m_BlockSize;                                //!< ブロックサイズ
    int                     m_BlockCount;                               //!< ブロック数
};


} // namespace detail
} // namespace nn
} // namespace vfx
