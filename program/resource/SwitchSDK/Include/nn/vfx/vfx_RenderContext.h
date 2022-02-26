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
#include <nn/vfx/vfx_Data.h>
#include <nn/vfx/vfx_Heap.h>

namespace nn {
namespace vfx {
namespace detail {

//---------------------------------------------------------------------------
// レンダコンテキストクラス
//---------------------------------------------------------------------------
class Rendercontext
{
    NN_DISALLOW_COPY( Rendercontext );
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //---------------------------------------------------------------------------
    Rendercontext() NN_NOEXCEPT
    {
    }
    //---------------------------------------------------------------------------
    //! @brief      初期化
    //! @param[in]  pDevice         gfxデバイス
    //! @param[in]  pRenderState    レンダーステートへのポインタ
    //---------------------------------------------------------------------------
    void Initialize( nn::gfx::Device* pDevice, detail::ResRenderState* pRenderState ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      終了処理
    //! @param[in]  pDevice         gfxデバイス
    //---------------------------------------------------------------------------
    void Finalize( nn::gfx::Device* pDevice ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief                      表示面のセットアップ
    //! @param[in] displaySideType  表示面のタイプ
    //------------------------------------------------------------------------------
    void SetDisplaySideType( DisplaySideType displaySideType ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  無効化
    //---------------------------------------------------------------------------
    void Invalidate() NN_NOEXCEPT
    {
        m_pResRenderState = NULL;
        //m_pBlendState = NULL;
    }

    //---------------------------------------------------------------------------
    //! @brief  有効かどうか
    //! @return trueで有効、falseで無効
    //---------------------------------------------------------------------------
    bool IsValidate() const NN_NOEXCEPT
    {
        if( m_pResRenderState )// && m_pBlendState )
        {
            return true;
        }
        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief  ブレンドステートを取得する
    //! @return ブレンドステートへの参照
    //---------------------------------------------------------------------------
    const nn::gfx::BlendState& GetGfxBelndState() const NN_NOEXCEPT
    {
        return m_BlendState;
    }

    //---------------------------------------------------------------------------
    //! @brief  デプスステートを取得する
    //! @return デプスステートへの参照
    //---------------------------------------------------------------------------
    const nn::gfx::DepthStencilState& GetGfxDepthState() const NN_NOEXCEPT
    {
        return m_DepthStencilState;
    }

    //---------------------------------------------------------------------------
    //! @brief  デプスステートを取得する
    //! @return デプスステートへの参照
    //---------------------------------------------------------------------------
    const nn::gfx::RasterizerState& GetGfxRasterizerState() const NN_NOEXCEPT
    {
        return m_RasterizerState;
    }


    enum
    {
        RequiredMemorySize_BlendState = 32    //!< ブレンドステート用のバッファサイズ
    };

private:
    detail::ResRenderState*         m_pResRenderState;                                      //!< レンダーステートへのポインタ
    nn::gfx::BlendState             m_BlendState;                                           //!< gfx ブレンドステート
    nn::gfx::DepthStencilState      m_DepthStencilState;                                    //!< gfx デプスステート
    nn::gfx::RasterizerState        m_RasterizerState;                                      //!< gfx ラスタライザステート
    NN_ALIGNAS( 8 ) uint8_t         m_BlendStateBuffer[ RequiredMemorySize_BlendState ];    //!< ブレンドステート用バッファ
};

} // namespace detail
} // namespace nn
} // namespace vfx
