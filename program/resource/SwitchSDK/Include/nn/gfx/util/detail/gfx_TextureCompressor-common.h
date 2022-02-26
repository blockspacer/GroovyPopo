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

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/gfx.h>
#include <nn/gfx/util/gfx_ObjectHolder.h>

namespace nn {
namespace gfx {
namespace util {
namespace detail {

    //-------------------------------------------------------------
    //! @brief テクスチャ圧縮時に利用する共通のステートクラスです。
    //-------------------------------------------------------------
    class TextureCompressorCommonState
    {
        NN_DISALLOW_COPY(TextureCompressorCommonState);
    public:

        /**
        * @brief コンストラクタです。
        */
        TextureCompressorCommonState() NN_NOEXCEPT;

        /**
        * @brief デストラクタです。
        */
        ~TextureCompressorCommonState() NN_NOEXCEPT;

        /**
        * @brief テクスチャ圧縮に必要なステートオブジェクトの初期化を行います。
        *
        * @param[in] pDevice デバイスのポインタです。
        *
        * @pre
        * - pDevice != NULL
        * - nn::gfx::IsInitialized(*pDevice)
        *
        * @post
        * - IsInitialized()
        */
        void Initialize( nn::gfx::Device* pDevice) NN_NOEXCEPT;

        /**
        * @brief テクスチャ圧縮に必要なステートオブジェクトを破棄します。
        *
        * @pre
        * - IsInitialized()
        * @post
        * - !IsInitialized()
        */
        void Finalize() NN_NOEXCEPT;

        /**
        * @brief コマンドバッファにテクスチャ圧縮に使用するステートのコマンドを書き込みます。
        *
        * @param[in] pCommandBuffer	コマンドを書き込むコマンドバッファのポインタです。
        * @param[in] textureDescriptorSlot 圧縮元のテクスチャのテクスチャビューが登録されているデスクリプタスロットです。
        * @param[in] samplerDescriptorSlot 圧縮元のテクスチャの値を読み込む際に使用するサンプラが登録されているデスクリプタスロットです。
        *
        * @pre
        * - pCommandBuffer != NULL
        * - nn::gfx::IsInitialized(*pCommandBuffer)
        * - IsInitialized()
        *
        * @post
        * - テクスチャ圧縮を行う際のステートをセットするコマンドが pCommandBuffer で渡されたコマンドバッファに記録された状態になります。
        */
        void MakeCommand(
            nn::gfx::CommandBuffer* pCommandBuffer,
            const nn::gfx::DescriptorSlot& textureDescriptorSlot,
            const nn::gfx::DescriptorSlot& samplerDescriptorSlot
        ) const NN_NOEXCEPT;

        /**
        * @brief 初期化済みかどうかを取得します。
        * @return  初期化済みかどうかを返します。
        */
        bool IsInitialized() const NN_NOEXCEPT;

        /**
        * @brief	デバイスを取得します。
        * @return	デバイスを返します。
        * @pre      IsInitialized()
        */
        nn::gfx::Device* GetDevice() const NN_NOEXCEPT;


    private:

        void InitializeRasterizerState() NN_NOEXCEPT;
        void InitializeBlendState() NN_NOEXCEPT;
        void InitializeDepthStencilState() NN_NOEXCEPT;

    private:

        bool                                        m_IsInitialized;
        nn::gfx::Device*                            m_pDevice;

        nn::gfx::RasterizerState                    m_RasterizerState;
        nn::gfx::util::BlendStateHolder<1>          m_BlendStateHolder;
        nn::gfx::DepthStencilState                  m_DepthStencilState;
    };

}

}
}
}
