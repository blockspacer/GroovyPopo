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

#include <nvn/nvn.h>

#include <nn/nn_Macro.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Variation-api.nvn.h>
#include <nn/gfx/gfx_Enum.h>

#include <nn/gfx/detail/gfx_Declare.h>

namespace nn {
namespace gfx {

/**
* @brief NVN API との相互運用を行うためのクラスです。
*
* @details
* 元のオブジェクトの作成者がオブジェクトの所有権をもちます。@n
* Initialize が呼ばれていない gfx オブジェクトに対して Finalize を呼ぶことはできません。@n
*
* 元のオブジェクトを作成したコンテキストとそれを使用するコンテキストは同じまたは共有関係にある必要があります。
*/
template<>
class TInteroperation< ApiVariation< ApiTypeNvn > >
{
    NN_DISALLOW_COPY( TInteroperation );
    TInteroperation() NN_NOEXCEPT;

public:
    /**
    * @brief 低レベルグラフィックス API のターゲットです。
    */
    typedef ApiVariation< ApiTypeNvn > Target;

    /**
    * @brief 指定された NVN のデバイスを gfx のデバイスに変換します。
    *
    * @param[out] pOutGfxDevice 変換された gfx のデバイスを受けとるためのポインター
    * @param[in] pNvnDevice 変換する NVN のデバイスへのポインター
    *
    * @pre
    * - pOutGfxDevice != NULL
    * - pOutGfxDevice が初期化されていない
    * - pNvnDevice != NULL
    */
    static void ConvertToGfxDevice( TDevice< Target >* pOutGfxDevice, NVNdevice* pNvnDevice ) NN_NOEXCEPT;

    /**
    * @brief 指定された NVN のキューを gfx のキューに変換します。
    *
    * @param[out] pOutGfxQueue 変換された gfx のキューを受けとるためのポインター
    * @param[in] pNvnQueue 変換する NVN のキューへのポインター
    * @param[in] pGfxDevice デバイスへのポインター
    *
    * @pre
    * - pOutGfxQueue != NULL
    * - pOutGfxQueue が初期化されていない
    * - pNvnQueue != NULL
    * - pGfxDevice != NULL の場合、pGfxDevice が初期化されている
    *
    * @details
    * pGfxDevice に NULL を指定した場合は変換後のキューで nn::gfx::Queue::CopyToScanBuffer を使用できません。
    */
    static void ConvertToGfxQueue( TQueue< Target >* pOutGfxQueue,
        NVNqueue* pNvnQueue, TDevice< Target >* pGfxDevice ) NN_NOEXCEPT;

    /**
    * @brief 指定された NVN のメモリープールを gfx のメモリープールに変換します。
    *
    * @param[out] pOutGfxMemoryPool 変換された gfx のメモリープールを受けとるためのポインター
    * @param[in] pNvnMemoryPool 変換する NVN のメモリープールへのポインター
    * @param[in] pMemory pNvnMemoryPool を作成した際の CPU メモリー
    *
    * @pre
    * - pOutGfxMemoryPool != NULL
    * - pOutGfxMemoryPool が初期化されていない
    * - pNvnMemoryPool != NULL
    *
    * @details
    * pMemory に NULL を指定した場合は以下の用途に使用できません。
    * - バッファー用のデスクリプタープールが使うメモリープール
    * - 実機向けにタイリングされているテクスチャーを PC 上で使う際の、NVN_MEMORY_POOL_FLAGS_CPU_CACHED 属性でないメモリープール
    */
    static void ConvertToGfxMemoryPool( TMemoryPool< Target >* pOutGfxMemoryPool,
        NVNmemoryPool* pNvnMemoryPool, void* pMemory ) NN_NOEXCEPT;

    /**
    * @brief 指定された NVN のデスクリプタープールを gfx のデスクリプタープールに変換します。
    *
    * @param[out] pOutGfxDescriptorPool 変換された gfx のデスクリプタープールを受けとるためのポインター
    * @param[in] pNvnTexturePool 変換する NVN のテクスチャープールへのポインター
    *
    * @pre
    * - pOutGfxDescriptorPool != NULL
    * - pOutGfxDescriptorPool が初期化されていない
    * - pNvnTexturePool != NULL
    */
    static void ConvertToGfxDescriptorPool( TDescriptorPool< Target >* pOutGfxDescriptorPool,
        NVNtexturePool* pNvnTexturePool ) NN_NOEXCEPT;

    /**
    * @brief 指定された NVN のサンプラープールを gfx のデスクリプタープールに変換します。
    *
    * @param[out] pOutGfxDescriptorPool 変換された gfx のデスクリプタープールを受けとるためのポインター
    * @param[in] pNvnSamplerPool 変換する NVN のサンプラープールへのポインター
    *
    * @pre
    * - pOutGfxDescriptorPool != NULL
    * - pOutGfxDescriptorPool が初期化されていない
    * - pNvnSamplerPool != NULL
    */
    static void ConvertToGfxDescriptorPool( TDescriptorPool< Target >* pOutGfxDescriptorPool,
        NVNsamplerPool* pNvnSamplerPool ) NN_NOEXCEPT;


    /**
    * @brief 指定された NVN のバッファーを gfx のバッファーに変換します。
    *
    * @param[out] pOutGfxBuffer 変換された gfx のバッファーを受けとるためのポインター
    * @param[in] pNvnBuffer 変換する NVN のバッファーへのポインター
    *
    * @pre
    * - pOutGfxBuffer != NULL
    * - pOutGfxBuffer が初期化されていない
    * - pNvnBuffer != NULL
    */
    static void ConvertToGfxBuffer( TBuffer< Target >* pOutGfxBuffer, NVNbuffer* pNvnBuffer ) NN_NOEXCEPT;

    /**
    * @brief 指定された NVN のコマンドバッファーを gfx のコマンドバッファーに変換します。
    *
    * @param[out] pOutGfxCommandBuffer 変換された gfx のコマンドバッファーを受けとるためのポインター
    * @param[in] pDevice デバイスへのポインター（この引数は将来なくなる可能性があります）
    * @param[in] pNvnCommandBuffer 変換する NVN のコマンドバッファーへのポインター
    *
    * @pre
    * - pOutGfxCommandBuffer != NULL
    * - pOutGfxCommandBuffer が初期化されていない
    * - pDevice != NULL
    * - pNvnCommandBuffer != NULL
    */
    static void ConvertToGfxCommandBuffer( TCommandBuffer< Target >* pOutGfxCommandBuffer,
        TDevice< Target >* pDevice, NVNcommandBuffer* pNvnCommandBuffer ) NN_NOEXCEPT;

    /**
    * @brief 指定された NVN のサンプラーを gfx のサンプラーに変換します。
    *
    * @param[out] pOutGfxSampler 変換された gfx のサンプラーを受けとるためのポインター
    * @param[in] pNvnSampler 変換する NVN のサンプラーへのポインター
    *
    * @pre
    * - pOutGfxSampler != NULL
    * - pOutGfxSampler が初期化されていない
    * - pNvnSampler != NULL
    */
    static void ConvertToGfxSampler( TSampler< Target >* pOutGfxSampler,
        NVNsampler* pNvnSampler ) NN_NOEXCEPT;

    /**
    * @brief 指定された NVN のプログラムを gfx のシェーダーに変換します。
    *
    * @param[out] pOutGfxShader 変換された gfx のシェーダーを受けとるためのポインター
    * @param[in] pNvnProgram 変換する NVN のプログラムへのポインター
    * @param[in] isSeparable pNvnProgram がセパレートとして作成されているかどうかの真偽値
    * @param[in] stageBits nn::gfx::ShaderStageBit の組み合わせで表される、pNvnProgram が所持しているステージ
    *
    * @pre
    * - pOutGfxShader != NULL
    * - pOutGfxShader が初期化されていない
    * - pNvnProgram != NULL
    */
    static void ConvertToGfxShader( TShader< Target >* pOutGfxShader,
        NVNprogram* pNvnProgram, bool isSeparable, int stageBits ) NN_NOEXCEPT;

    /**
    * @brief 指定された NVN のテクスチャーを gfx のテクスチャーに変換します。
    *
    * @param[out] pOutGfxTexture 変換された gfx のテクスチャーを受けとるためのポインター
    * @param[in] pNvnTexture 変換する NVN のテクスチャーへのポインター
    *
    * @pre
    * - pOutGfxTexture != NULL
    * - pOutGfxTexture が初期化されていない
    * - pNvnTexture != NULL
    */
    static void ConvertToGfxTexture( TTexture< Target >* pOutGfxTexture,
        NVNtexture* pNvnTexture ) NN_NOEXCEPT;

    /**
    * @brief 指定された NVN のテクスチャービューを gfx のテクスチャービューに変換します。
    *
    * @param[out] pOutGfxTextureView 変換された gfx のテクスチャービューを受けとるためのポインター
    * @param[in] pNvnTexture 変換する NVN のテクスチャーへのポインター
    * @param[in] pNvnTextureView 変換する NVN のテクスチャービューへのポインター
    *
    * @pre
    * - pOutGfxTextureView != NULL
    * - pOutGfxTextureView が初期化されていない
    * - pNvnTexture != NULL
    * - pNvnTextureView != NULL
    */
    static void ConvertToGfxTextureView( TTextureView< Target >* pOutGfxTextureView,
        NVNtexture* pNvnTexture, NVNtextureView* pNvnTextureView ) NN_NOEXCEPT;

    /**
    * @brief 指定された NVN のテクスチャービューを gfx のカラーターゲットビューに変換します。
    *
    * @param[out] pOutGfxColorTargetView 変換された gfx のカラーターゲットビューを受けとるためのポインター
    * @param[in] pNvnTexture 変換する NVN のテクスチャーへのポインター
    * @param[in] pNvnTextureView 変換する NVN のテクスチャービューへのポインター
    *
    * @pre
    * - pOutGfxColorTargetView != NULL
    * - pOutGfxColorTargetView が初期化されていない
    * - pNvnTexture != NULL
    * - pNvnTextureView != NULL
    */
    static void ConvertToGfxColorTargetView( TColorTargetView< Target >* pOutGfxColorTargetView,
        NVNtexture* pNvnTexture, NVNtextureView* pNvnTextureView ) NN_NOEXCEPT;

    /**
    * @brief 指定された NVN のテクスチャービューを gfx の深度ステンシルビューに変換します。
    *
    * @param[out] pOutGfxDepthStencilView 変換された gfx の深度ステンシルビューを受けとるためのポインター
    * @param[in] pNvnTexture 変換する NVN のテクスチャーへのポインター
    * @param[in] pNvnTextureView 変換する NVN のテクスチャービューへのポインター
    *
    * @pre
    * - pOutGfxDepthStencilView != NULL
    * - pOutGfxDepthStencilView が初期化されていない
    * - pNvnTexture != NULL
    * - pNvnTextureView != NULL
    */
    static void ConvertToGfxDepthStencilView( TDepthStencilView< Target >* pOutGfxDepthStencilView,
        NVNtexture* pNvnTexture, NVNtextureView* pNvnTextureView ) NN_NOEXCEPT;

    /**
    * @brief 指定された gfx のイメージフォーマットを nvn のフォーマットに変換します。
    *
    * @param[in] gfxImageFormat 変換する gfx のイメージフォーマット
    *
    * @return 変換された nvn のフォーマットを返します。
    */
    static NVNformat ConvertToNvnFormat( ImageFormat gfxImageFormat ) NN_NOEXCEPT;

    /**
    * @brief 指定された gfx の属性フォーマットを nvn のフォーマットに変換します。
    *
    * @param[in] gfxAttributeFormat 変換する gfx の属性フォーマット
    *
    * @return 変換された nvn のフォーマットを返します。
    */
    static NVNformat ConvertToNvnFormat( AttributeFormat gfxAttributeFormat ) NN_NOEXCEPT;
};

}
}
