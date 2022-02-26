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
#include <nn/vfx/vfx_UniformBlock.h>

namespace nn {
namespace vfx {

//---------------------------------------------------
//! @brief  テンポラリバッファ
//!
//!         エフェクトランタイム内部で利用されるGPUが参照する一時バッファを管理します。
//---------------------------------------------------
class TemporaryBuffer
{
    NN_DISALLOW_COPY( TemporaryBuffer );

public:
    //---------------------------------------------------------------------------
    //! @brief      バッファアライン
    //---------------------------------------------------------------------------
    enum BufferAlignment
    {
        BufferAlignment_DefaultAlignment = 0x100    //!< バッファのデフォルトアライメント値
    };

    //---------------------------------------------------------------------------
    //! @brief      コンストラクタ
    //---------------------------------------------------------------------------
    TemporaryBuffer() NN_NOEXCEPT
    {
        Invalidate();
    }

    //---------------------------------------------------------------------------
    //! @brief      初期化処理を行います。
    //!
    //! @param[in]  pDevice     gfxデバイス。
    //! @param[in]  size        初期化するテンポラリバッファサイズ。
    //! @param[in]  bufferMode  バッファリングモード
    //!
    //! @return                 成功した場合 true
    //---------------------------------------------------------------------------
    bool Initialize( nn::gfx::Device* pDevice, size_t size, BufferingMode bufferMode ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      無効化処理を行います。
    //---------------------------------------------------------------------------
    void Invalidate() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      有効かどうか
    //! @return     有効かどうかを示す bool 値
    //---------------------------------------------------------------------------
    bool IsValidate() const NN_NOEXCEPT
    {
        return m_IsInitialized;
    }

    //---------------------------------------------------------------------------
    //! @brief      終了処理を行います。
    //---------------------------------------------------------------------------
    void Finalize( nn::gfx::Device* pGfxDevice ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      テンポラリバッファを切り替えします。
    //---------------------------------------------------------------------------
    void Swap() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      テンポラリバッファからメモリを確保する。
    //!             確保できない場合はNULLが返ります。
    //! @param[in]  gpuAddress      GPU アドレス
    //! @param[in]  size            確保するバッファサイズ
    //! @return     先頭アドレス
    //---------------------------------------------------------------------------
    void* Map( nn::gfx::GpuAddress* gpuAddress, size_t size ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      テンポラリバッファからメモリを確保されたワークを確定する。
    //---------------------------------------------------------------------------
    void Unmap() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      テンポラリバッファのサイズを取得します。
    //! @return     テンポラリバッファのサイズ
    //---------------------------------------------------------------------------
    size_t GetSize() const NN_NOEXCEPT
    {
        return m_BufferSize;
    }

    //---------------------------------------------------------------------------
    //! @brief      呼び出し時のテンポラリバッファの消費サイズを取得します。
    //! @return     テンポラリバッファの消費サイズ
    //---------------------------------------------------------------------------
    size_t GetUsedSize() const NN_NOEXCEPT
    {
        return m_AllocatedSize[ m_BufferSide ];
    }

    //---------------------------------------------------------------------------
    //! @brief              呼び出し時のテンポラリバッファの消費サイズを取得します。
    //!
    //! @param[in]  side    取得するバッファサイド。
    //! @return             テンポラリバッファの消費サイズ
    //---------------------------------------------------------------------------
    size_t GetUsedSize( BufferSide side ) const NN_NOEXCEPT
    {
        return m_AllocatedSize[ side ];
    }

    //---------------------------------------------------------------------------
    //! @brief      テンポラリバッファの最大消費サイズを取得します。
    //! @return     テンポラリバッファの最大消費サイズ
    //---------------------------------------------------------------------------
    size_t GetMaxUsedSize() const NN_NOEXCEPT
    {
        return m_MaxAllocatedSize;
    }

private:
    int                     m_BufferSide;                                   //!< 現在使用しているバッファサイド
    BufferingMode           m_BufferMode;                                   //!< バッファリングモード（ダブル／トリプル）
    size_t                  m_MaxAllocatedSize;                             //!< 最大メモリ確保サイズ
    size_t                  m_AllocatedSize[ BufferSide_MaxBufferSide ];    //!< ダブルバッファメモリ消費サイズ
    size_t                  m_BufferSize;                                   //!< ダブルバッファメモリ消費サイズ
    bool                    m_IsInitialized;                                //!< 初期化済みかどうか
    detail::ConstantBuffer  m_ConstantBuffer[ BufferSide_MaxBufferSide ];   //!< コンスタントバッファ
};


} // namespace vfx
} // namespace nn
