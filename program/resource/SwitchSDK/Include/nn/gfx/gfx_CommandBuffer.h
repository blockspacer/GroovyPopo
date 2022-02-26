/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
* @file
* @brief コマンドバッファーに関する API の宣言
*/

#pragma once

#include <nn/nn_Macro.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_CommandBufferInfo.h>
#include <nn/gfx/gfx_TextureInfo.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_RequiredMemory.h>
#include <nn/gfx/detail/gfx_UserPtr.h>

#if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
    #include <nn/gfx/detail/gfx_CommandBuffer-api.gl.4.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
    #include <nn/gfx/detail/gfx_CommandBuffer-api.nvn.8.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
    #include <nn/gfx/detail/gfx_CommandBuffer-api.gx.2.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
    #include <nn/gfx/detail/gfx_CommandBuffer-api.d3d.11.h>
#endif

#if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
    #include <nn/gfx/detail/gfx_CommandBuffer-api.vk.1.h>
#endif

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_DEPRECATED_DECLARATIONS

namespace nn {
namespace gfx {

class GpuAddress;
class DescriptorSlot;
class ViewportStateInfo;
class ScissorStateInfo;

/**
* @brief コマンドバッファーを表すグラフィックス抽象レイヤーです。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
class TCommandBuffer
    : public detail::CommandBufferImpl< typename detail::TargetVariation< TTarget >::Type >
{
    NN_DISALLOW_COPY( TCommandBuffer );

    typedef detail::CommandBufferImpl< typename detail::TargetVariation< TTarget >::Type > Impl;

public:
    /**
    * @brief 対象の低レベルグラフィックス API です。
    */
    typedef typename Impl::Target Target;

    /**
    * @brief コマンドバッファーを初期化するための情報クラスです。
    */
    typedef CommandBufferInfo InfoType;

    /**
    * @brief メモリー不足のイベントを受けとるコールバックの型です。
    */
    typedef void ( *OutOfMemoryEventCallback )(
        TCommandBuffer< Target >* pCommandBuffer, const OutOfMemoryEventArg& arg );

    /**
    * @brief コマンドメモリーに要求されるアライメントを取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @return コマンドメモリーに要求されるアライメントを返します。
    */
    static size_t GetCommandMemoryAlignment( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::GetCommandMemoryAlignment( pDevice );
    }

    /**
    * @brief コントロールメモリーに要求されるアライメントを取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @return コントロールメモリーに要求されるアライメントを返します。
    */
    static size_t GetControlMemoryAlignment( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::GetControlMemoryAlignment( pDevice );
    }

    /**
    * @brief コンストラクターです。
    */
    TCommandBuffer() NN_NOEXCEPT
    {
    }

    /**
    * @brief コマンドバッファーを初期化します。
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] info 初期化用の情報
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - コマンドバッファーが初期化されていない
    *
    * @post
    * - コマンドバッファーが初期化されている
    */
    void Initialize( TDevice< Target >* pDevice, const InfoType& info ) NN_NOEXCEPT
    {
        return Impl::Initialize( pDevice, info );
    }

    /**
    * @brief コマンドバッファーを破棄します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - コマンドバッファーが初期化されている
    * - コマンドの追加が開始されていない
    *
    * @post
    * コマンドバッファーが初期化されていない
    */
    void Finalize( TDevice< Target >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( pDevice );
    }

    /**
    * @brief コマンドメモリーを追加します。
    *
    * @param[in] pMemoryPool コマンドメモリーを配置するメモリープールへのポインター
    * @param[in] memoryPoolOffset バイトでのコマンドメモリーを配置するメモリープールへのオフセット
    * @param[in] memorySize バイトでの追加するコマンドメモリーのサイズ
    *
    * @pre
    * - コマンドバッファーが初期化されている
    * - pMemoryPool != NULL
    *
    * @post
    * - コマンドメモリーが追加されている
    */
    void AddCommandMemory( TMemoryPool< Target >* pMemoryPool,
        ptrdiff_t memoryPoolOffset, size_t memorySize ) NN_NOEXCEPT
    {
        return Impl::AddCommandMemory( pMemoryPool, memoryPoolOffset, memorySize );
    }

    /**
    * @brief コントロールメモリーを追加します。
    *
    * @param[in] pMemory 追加するコントロールメモリーへのポインター
    * @param[in] memorySize バイトでの追加するコントロールメモリーのサイズ
    *
    * @pre
    * - コマンドバッファーが初期化されている
    * - pMemory != NULL
    *
    * @post
    * - コントロールメモリーが追加されている
    */
    void AddControlMemory( void* pMemory, size_t memorySize ) NN_NOEXCEPT
    {
        return Impl::AddControlMemory( pMemory, memorySize );
    }

    /**
    * @brief コマンドメモリーが不足したイベントを受けとるためのコールバックを設定します。
    *
    * @param[in] pEventCallback コマンドメモリーが不足したイベントを受けとるためのコールバックへのポインター
    *
    * @pre
    * - コマンドバッファーが初期化されている
    *
    * @post
    * - pEventCallback != NULL の場合、コマンドメモリーが不足したイベントを受け取るためのコールバックが設定されている
    * - pEventCallback == NULL の場合、コマンドメモリーが不足したイベントを受け取るためのコールバックが設定されていない
    */
    void SetOutOfCommandMemoryEventCallback( OutOfMemoryEventCallback pEventCallback ) NN_NOEXCEPT
    {
        return Impl::SetOutOfCommandMemoryEventCallback( pEventCallback );
    }

    /**
    * @brief コントロールメモリーが不足したイベントを受けとるためのコールバックを設定します。
    *
    * @param[in] pEventCallback コントロールメモリーが不足したイベントを受けとるためのコールバックへのポインター
    *
    * @pre
    * - コマンドバッファーが初期化されている
    *
    * @post
    * - pEventCallback != NULL の場合、コントロールメモリーが不足したイベントを受け取るためのコールバックが設定されている
    * - pEventCallback == NULL の場合、コントロールメモリーが不足したイベントを受け取るためのコールバックが設定されていない
    */
    void SetOutOfControlMemoryEventCallback( OutOfMemoryEventCallback pEventCallback ) NN_NOEXCEPT
    {
        return Impl::SetOutOfControlMemoryEventCallback( pEventCallback );
    }

    /**
    * @brief コマンドメモリーとコントロールメモリーを初期状態に戻します。
    *
    * @post
    * - コマンドメモリーとコントロールメモリーが初期状態に戻っている
    *
    * @details
    * 内部で自動的にメモリーを確保する低レベルグラフィックス API のために使います。@n
    * この関数を呼ぶ前に、以前に提出されたすべてのコマンドが GPU によって実行を終えていることを確実にする必要があります。
    */
    void Reset() NN_NOEXCEPT
    {
        return Impl::Reset();
    }

    /**
    * @brief コマンドバッファーへのコマンドの追加を開始します。
    *
    * @pre
    * - コマンドバッファーが初期化されている
    * - コマンドの追加が開始されていない
    *
    * @post
    * - コマンドの追加が開始されている
    */
    void Begin() NN_NOEXCEPT
    {
        return Impl::Begin();
    }

    /**
    * @brief コマンドバッファーへのコマンドの追加を終了します。
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - コマンドの追加が開始されていない
    */
    void End() NN_NOEXCEPT
    {
        return Impl::End();
    }

    /**
    * @brief 演算処理をディスパッチするコマンドを追加します。
    *
    * @param[in] groupCountX x 次元のグループ数
    * @param[in] groupCountY y 次元のグループ数
    * @param[in] groupCountZ z 次元のグループ数
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - 演算処理をディスパッチするコマンドが追加されている
    *
    * @details
    * グループ数に指定できる値には制限があることがあります。詳細は低レベルグラフィックス API のドキュメントを参照してください。
    *
    * @platformbegin{GX}
    * この関数はサポートされません。
    * @platformend
    */
    void Dispatch( int groupCountX, int groupCountY, int groupCountZ ) NN_NOEXCEPT
    {
        return Impl::Dispatch( groupCountX, groupCountY, groupCountZ );
    }

    /**
    * @brief 描画コマンドを追加します。
    *
    * @param[in] primitiveTopology 描画対象のプリミティブトポロジー
    * @param[in] vertexCount 描画する頂点数
    * @param[in] vertexOffset 開始頂点へのオフセット
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - 描画コマンドが追加されている
    */
    void Draw( PrimitiveTopology primitiveTopology, int vertexCount, int vertexOffset ) NN_NOEXCEPT
    {
        return Impl::Draw( primitiveTopology, vertexCount, vertexOffset );
    }

    /**
    * @brief インスタンス化された描画コマンドを追加します。
    *
    * @param[in] primitiveTopology 描画対象のプリミティブトポロジー
    * @param[in] vertexCountPerInstance 描画するインスタンスあたりの頂点数
    * @param[in] vertexOffset 開始頂点へのオフセット
    * @param[in] instanceCount 描画するインスタンス数
    * @param[in] baseInstance 開始インスタンス
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - インスタンス化された描画コマンドが追加されている
    */
    void Draw( PrimitiveTopology primitiveTopology, int vertexCountPerInstance,
        int vertexOffset, int instanceCount, int baseInstance ) NN_NOEXCEPT
    {
        return Impl::Draw( primitiveTopology, vertexCountPerInstance, vertexOffset,
            instanceCount, baseInstance );
    }

    /**
    * @brief インデックス化された描画コマンドを追加します。
    *
    * @param[in] primitiveTopology 描画対象のプリミティブトポロジー
    * @param[in] indexFormat インデックスバッファーのフォーマット
    * @param[in] indexBufferAddress インデックスバッファーへの GPU アドレス
    * @param[in] indexCount 描画するインデックス数
    * @param[in] baseVertex それぞれのインデックスに加算されるベース頂点
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - インデックス化された描画コマンドが追加されている
    */
    void DrawIndexed( PrimitiveTopology primitiveTopology, IndexFormat indexFormat,
        const GpuAddress& indexBufferAddress, int indexCount, int baseVertex ) NN_NOEXCEPT
    {
        return Impl::DrawIndexed( primitiveTopology, indexFormat, indexBufferAddress, indexCount, baseVertex );
    }

    /**
    * @brief インデックスされインスタンス化された描画コマンドを追加します。
    *
    * @param[in] primitiveTopology 描画対象のプリミティブトポロジー
    * @param[in] indexFormat インデックスバッファーのフォーマット
    * @param[in] indexBufferAddress インデックスバッファーへの GPU アドレス
    * @param[in] indexCountPerInstance 描画するインスタンスあたりのインデックス数
    * @param[in] baseVertex それぞれのインデックスに加算されるベース頂点
    * @param[in] instanceCount 描画するインスタンス数
    * @param[in] baseInstance 開始インスタンス
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - インデックス化されインスタンス化された描画コマンドが追加されている
    */
    void DrawIndexed( PrimitiveTopology primitiveTopology, IndexFormat indexFormat,
        const GpuAddress& indexBufferAddress, int indexCountPerInstance,
        int baseVertex, int instanceCount, int baseInstance ) NN_NOEXCEPT
    {
        return Impl::DrawIndexed( primitiveTopology, indexFormat, indexBufferAddress,
            indexCountPerInstance, baseVertex, instanceCount, baseInstance );
    }

    /**
    * @brief バッファーからパラメーターをとり演算処理をディスパッチするコマンドを追加します。
    *
    * @param[in] indirectBufferAddress パラメーターが格納されているバッファーの GPU アドレス
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - バッファーからパラメーターをとり演算処理をディスパッチするコマンドが追加されている
    */
    void DispatchIndirect( const GpuAddress& indirectBufferAddress ) NN_NOEXCEPT
    {
        return Impl::DispatchIndirect( indirectBufferAddress );
    }

    /**
    * @brief バッファーからパラメーターをとるインスタンス化された描画コマンドを追加します。
    *
    * @param[in] primitiveTopology 描画対象のプリミティブトポロジー
    * @param[in] indirectBufferAddress パラメーターが格納されているバッファーの GPU アドレス
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - バッファーからパラメーターをとるインスタンス化された描画コマンドが追加されている
    */
    void DrawIndirect( PrimitiveTopology primitiveTopology, const GpuAddress& indirectBufferAddress ) NN_NOEXCEPT
    {
        return Impl::DrawIndirect( primitiveTopology, indirectBufferAddress );
    }

    /**
    * @brief バッファーからパラメータをとるインデックスされインスタンス化された描画コマンドを追加します。
    *
    * @param[in] primitiveTopology 描画対象のプリミティブトポロジー
    * @param[in] indexFormat インデックスバッファーのフォーマット
    * @param[in] indexBufferAddress インデックスバッファーの GPU アドレス
    * @param[in] indirectBufferAddress パラメーターが格納されているバッファーの GPU アドレス
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - バッファーからパラメーターをとるインデックス化されインスタンス化された描画コマンドが追加されている
    *
    * @details
    * GL 版では indexBufferAddress は Buffer から取得した先頭アドレスである必要があります。
    * オフセットについては indirectBufferAddress のパラメーターにおいて指定してください。
    *
    */
    void DrawIndexedIndirect( PrimitiveTopology primitiveTopology, IndexFormat indexFormat,
        const GpuAddress& indexBufferAddress, const GpuAddress& indirectBufferAddress ) NN_NOEXCEPT
    {
        return Impl::DrawIndexedIndirect( primitiveTopology, indexFormat, indexBufferAddress, indirectBufferAddress );
    }

    /**
    * @brief パイプラインをセットするコマンドを追加します。
    *
    * @param[in] pPipeline 対象のパイプラインへのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pPipeline != NULL
    * - pPipeline が初期化されている
    *
    * @post
    * - パイプラインをセットするコマンドが追加されている
    */
    void SetPipeline( const TPipeline< Target >* pPipeline ) NN_NOEXCEPT
    {
        return Impl::SetPipeline( pPipeline );
    }

    /**
    * @brief レンダーターゲットをセットするコマンドを追加します。
    *
    * @param[in] colorTargetCount カラーターゲットの数
    * @param[in] ppColorTargets 対象のカラーターゲットのポインターの配列へのポインター
    * @param[in] pDepthStencil 対象の深度ステンシルへのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - ppColorTargets の colorTargetCount 要素それぞれについて、NULL でない場合、そのカラーターゲットが初期化されている
    * - pDepthStencil != NULL の場合、pDepthStencil が初期化されている
    *
    * @post
    * - レンダーターゲットをセットするコマンドが追加されている
    */
    void SetRenderTargets( int colorTargetCount, const TColorTargetView< Target >* const * ppColorTargets,
        const TDepthStencilView< Target >* pDepthStencil ) NN_NOEXCEPT
    {
        return Impl::SetRenderTargets( colorTargetCount, reinterpret_cast< const
            detail::ColorTargetViewImpl< Target >* const * >( ppColorTargets ), pDepthStencil );
    }

    /**
    * @brief 頂点バッファーをセットするコマンドを追加します。
    *
    * @param[in] bufferIndex セットする対象のバッファーインデックス
    * @param[in] vertexBufferAddress 対象の頂点バッファーの GPU アドレス
    * @param[in] stride バッファーにおける頂点あたりのストライド
    * @param[in] size 対象の頂点バッファーのサイズ
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - 頂点バッファーをセットするコマンドが追加されている
    */
    void SetVertexBuffer( int bufferIndex, const GpuAddress& vertexBufferAddress,
        ptrdiff_t stride, size_t size ) NN_NOEXCEPT
    {
        return Impl::SetVertexBuffer( bufferIndex, vertexBufferAddress, stride, size );
    }

    /**
    * @brief ビューポートシザーステートをセットするコマンドを追加します。
    *
    * @param[in] pViewportScissorState 対象のビューポートシザーステートへのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pViewportScissorState != NULL
    * - pViewportScissorState が初期化されている
    *
    * @post
    * - ビューポートシザーステートをセットするコマンドが追加されている
    */
    void SetViewportScissorState( const TViewportScissorState< Target >* pViewportScissorState ) NN_NOEXCEPT
    {
        return Impl::SetViewportScissorState( pViewportScissorState );
    }

    /**
    * @brief バッファーをコピーするコマンドを追加します。
    *
    * @param[in] pDstBuffer コピー先のバッファーへのポインター
    * @param[in] dstOffset コピー先のバッファーの先頭からのオフセット
    * @param[in] pSrcBuffer コピー元のバッファーへのポインター
    * @param[in] srcOffset コピー元のバッファーの先頭からのオフセット
    * @param[in] size バイトでのコピーするサイズ
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pDstBuffer != NULL
    * - pSrcBuffer != NULL
    * - pDstBuffer が初期化されている
    * - pSrcBuffer が初期化されている
    *
    * @post
    * - バッファーをコピーするコマンドが追加されている
    *
    * @details
    * コピー先とコピー元の範囲は重なることができません。
    *
    */
    void CopyBuffer( TBuffer< Target >* pDstBuffer, ptrdiff_t dstOffset,
        const TBuffer< Target >* pSrcBuffer, ptrdiff_t srcOffset, size_t size ) NN_NOEXCEPT
    {
        return Impl::CopyBuffer( pDstBuffer, dstOffset, pSrcBuffer, srcOffset, size );
    }

    /**
    * @brief イメージをコピーするコマンドを追加します。
    *
    * @param[in] pDstTexture コピー先のテクスチャーへのポインター
    * @param[in] dstSubresource コピー先の特定のイメージを表すパラメータへの参照
    * @param[in] dstOffsetU コピー先の U 座標オフセット
    * @param[in] dstOffsetV コピー先の V 座標オフセット
    * @param[in] dstOffsetW コピー先の W 座標オフセット
    * @param[in] pSrcTexture コピー元のテクスチャーへのポインター
    * @param[in] srcCopyRegion コピー範囲を表すパラメータへの参照
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pDstTexture != NULL
    * - pSrcTexture != NULL
    * - pDstTexture が初期化されている
    * - pSrcTexture が初期化されている
    *
    * @post
    * - イメージをコピーするコマンドが追加されている
    *
    * @details
    * コピー先とコピー元の範囲は重なることができません。
    */
    void CopyImage( TTexture< Target >* pDstTexture, const TextureSubresource& dstSubresource,
        int dstOffsetU, int dstOffsetV, int dstOffsetW, const TTexture< Target >* pSrcTexture,
        const TextureCopyRegion& srcCopyRegion ) NN_NOEXCEPT
    {
        return Impl::CopyImage( pDstTexture, dstSubresource, dstOffsetU,
            dstOffsetV, dstOffsetW, pSrcTexture, srcCopyRegion );
    }

    /**
    * @brief バッファーからイメージにコピーするコマンドを追加します。
    *
    * @param[in] pDstTexture コピー先のテクスチャーへのポインター
    * @param[in] pSrcBuffer コピー元のバッファーへのポインター
    * @param[in] copyRegion コピー範囲を表すパラメータへの参照
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pDstTexture != NULL
    * - pSrcBuffer != NULL
    * - pDstTexture が初期化されている
    * - pSrcBuffer が初期化されている
    *
    * @post
    * - バッファーからイメージにコピーするコマンドが追加されている
    */
    void CopyBufferToImage( TTexture< Target >* pDstTexture, const TBuffer< Target >* pSrcBuffer,
        const BufferTextureCopyRegion& copyRegion  ) NN_NOEXCEPT
    {
        return Impl::CopyBufferToImage( pDstTexture, pSrcBuffer, copyRegion );
    }

    /**
    * @brief イメージからバッファーにコピーするコマンドを追加します。
    *
    * @param[in] pDstBuffer コピー先のバッファーへのポインター
    * @param[in] pSrcTexture コピー元のテクスチャーへのポインター
    * @param[in] copyRegion コピー範囲を表すパラメータへの参照
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pDstBuffer != NULL
    * - pSrcTexture != NULL
    * - pDstBuffer が初期化されている
    * - pSrcTexture が初期化されている
    *
    * @post
    * - イメージからバッファーにコピーするコマンドが追加されている
    */
    void CopyImageToBuffer( TBuffer< Target >* pDstBuffer, const TTexture< Target >* pSrcTexture,
        const BufferTextureCopyRegion& copyRegion ) NN_NOEXCEPT
    {
        return Impl::CopyImageToBuffer( pDstBuffer, pSrcTexture, copyRegion );
    }

    /**
    * @brief バッファーからイメージにコピーするコマンドを追加します。
    *
    * @deprecated 廃止予定です。3 引数版の CopyBufferToImage を使用してください。
    *
    * @param[in] pDstTexture コピー先のテクスチャーへのポインター
    * @param[in] dstRegion コピー範囲を表すパラメーターへの参照
    * @param[in] pSrcBuffer コピー元のバッファーへのポインター
    * @param[in] srcOffset バイトでのコピー元のバッファーの先頭からのオフセット
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pDstTexture != NULL
    * - pSrcBuffer ! =NULL
    * - pDstTexture が初期化されている
    * - pSrcTexture が初期化されている
    *
    * @post
    * - バッファーからイメージにコピーするコマンドが追加されている
    */
    void CopyBufferToImage( TTexture< Target >* pDstTexture, const TextureCopyRegion& dstRegion,
        const TBuffer< Target >* pSrcBuffer, ptrdiff_t srcOffset ) NN_NOEXCEPT
    {
        return Impl::CopyBufferToImage( pDstTexture, dstRegion, pSrcBuffer, srcOffset );
    }

    /**
    * @brief イメージからバッファーにコピーするコマンドを追加します。
    *
    * @deprecated 廃止予定です。3 引数版の CopyImageToBuffer を使用してください。
    *
    * @param[in] pDstBuffer コピー先のバッファーへのポインター
    * @param[in] dstOffset バイトでのコピー先のバッファーの先頭からのオフセット
    * @param[in] pSrcTexture コピー元のテクスチャーへのポインター
    * @param[in] srcRegion コピー範囲を表すパラメーターへの参照
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pDstBuffer != NULL
    * - pSrcTexture != NULL
    * - pDstTexture が初期化されている
    * - pSrcTexture が初期化されている
    *
    * @post
    * - イメージからバッファーにコピーするコマンドが追加されている
    */
    void CopyImageToBuffer( TBuffer< Target >* pDstBuffer, ptrdiff_t dstOffset,
        const TTexture< Target >* pSrcTexture, const TextureCopyRegion& srcRegion ) NN_NOEXCEPT
    {
        return Impl::CopyImageToBuffer( pDstBuffer, dstOffset, pSrcTexture, srcRegion );
    }

    /**
    * @brief イメージを伸縮つきでコピーするコマンドを追加します。
    *
    * @param[in] pDstTexture コピー先のテクスチャーへのポインター
    * @param[in] dstRegion コピー先のコピー範囲
    * @param[in] pSrcTexture コピー元のテクスチャーへのポインター
    * @param[in] srcRegion コピー元のコピー範囲
    * @param[in] copyFlags nn::gfx::ImageCopyFlag の組み合わせで表されるコピーフラグ
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pDstTexture != NULL
    * - pSrcTetxure != NULL
    * - pDstTexture が初期化されている
    * - pSrcTexture が初期化されている
    *
    * @post
    * - イメージを伸縮つきでコピーするコマンドが追加されている
    */
    void BlitImage( TTexture< Target >* pDstTexture, const TextureCopyRegion& dstRegion,
        const TTexture< Target >* pSrcTexture, const TextureCopyRegion& srcRegion, int copyFlags ) NN_NOEXCEPT
    {
        return Impl::BlitImage( pDstTexture, dstRegion, pSrcTexture, srcRegion, copyFlags );
    }

    /**
    * @brief バッファーをクリアするコマンドを追加します。
    *
    * @param[in] pBuffer クリアするバッファーへのポインター
    * @param[in] offset クリアを開始するバッファーの先頭からのバイトでのオフセット
    * @param[in] size バイトでのクリアするサイズ
    * @param[in] value クリアする値
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pBuffer != NULL
    * - offset >= 0
    * - offset が 4 の倍数である
    * - size が 4 の倍数である
    *
    * @post
    * - バッファーをクリアするコマンド追加されている
    */
    void ClearBuffer( TBuffer< Target >* pBuffer, ptrdiff_t offset, size_t size, uint32_t value ) NN_NOEXCEPT
    {
        return Impl::ClearBuffer( pBuffer, offset, size, value );
    }

    /**
    * @brief カラーターゲットをクリアするコマンドを追加します。
    *
    * @param[in] pColorTarget 対象のカラーターゲットへのポインター
    * @param[in] red クリアする色の赤要素
    * @param[in] green クリアする色の緑要素
    * @param[in] blue クリアする色の青要素
    * @param[in] alpha クリアする色のアルファ要素
    * @param[in] pArrayRange クリアする配列範囲へのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pColorTarget  != NULL
    * - pColorTarget が初期化されている
    *
    * @post
    * - カラーターゲットをクリアするコマンドが追加されている
    *
    * @details
    * 浮動小数点数フォーマットでのクリアにのみ使用できます。@n
    * 符号付き整数や符号なし整数のフォーマットでのクリアについては nn::gfx::CommandBuffer::ClearColorTarget を使用してください。@n
    * pArrayRange で指定される配列範囲をすべてクリアします。@n
    * pArrayRange に NULL を指定した場合は全配列範囲をクリアします。
    */
    void ClearColor( TColorTargetView< Target >* pColorTarget, float red, float green,
        float blue, float alpha, const TextureArrayRange* pArrayRange ) NN_NOEXCEPT
    {
        return Impl::ClearColor( pColorTarget, red, green, blue, alpha, pArrayRange );
    }

    /**
    * @brief カラーターゲットをクリアするコマンドを追加します。
    *
    * @param[in] pColorTarget 対象のカラーターゲットへのポインター
    * @param[in] clearColor クリアする色
    * @param[in] pArrayRange クリアする配列範囲へのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pColorTarget  != NULL
    * - pColorTarget が初期化されている
    *
    * @post
    * - カラーターゲットをクリアするコマンドが追加されている
    *
    * @details
    * pArrayRange で指定される配列範囲をすべてクリアします。@n
    * pArrayRange に NULL を指定した場合は全配列範囲をクリアします。
    */
    void ClearColorTarget( TColorTargetView< Target >* pColorTarget,
        const ClearColorValue& clearColor, const TextureArrayRange* pArrayRange ) NN_NOEXCEPT
    {
        return Impl::ClearColorTarget( pColorTarget, clearColor, pArrayRange );
    }

    /**
    * @brief 深度ステンシルをクリアするコマンドを追加します。
    *
    * @param[in] pDepthStencil 対象の深度ステンシルへのポインター
    * @param[in] depth クリアする深度値
    * @param[in] stencil クリアするステンシル値
    * @param[in] clearMode 深度とステンシルのどちらかまたは両方をクリアするかを指定するクリアモード
    * @param[in] pArrayRange クリアする配列範囲へのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pDepthStencil != NULL
    * - pDepthStencil が初期化されている
    *
    * @post
    * - 深度ステンシルをクリアするコマンドが追加されている
    *
    * @details
    * pArrayRange で指定される配列範囲をすべてクリアします。@n
    * pArrayRange に NULL を指定した場合は全配列範囲をクリアします。
    */
    void ClearDepthStencil( TDepthStencilView< Target >* pDepthStencil, float depth,
        int stencil, DepthStencilClearMode clearMode, const TextureArrayRange* pArrayRange ) NN_NOEXCEPT
    {
        return Impl::ClearDepthStencil( pDepthStencil, depth, stencil, clearMode, pArrayRange );
    }

    /**
    * @brief マルチサンプルカラーターゲットを単一サンプルテクスチャーに解決します。
    *
    * @param[in] pDstTexture 解決先のテクスチャーへのポインター
    * @param[in] dstMipLevel 解決先のテクスチャーのミップマップレベル
    * @param[in] dstStartArrayIndex 解決先のテクスチャーの配列の開始インデックス
    * @param[in] pSrcColorTarget 解決元のカラーターゲットへのポインター
    * @param[in] pSrcArrayRange 解決元のカラーターゲットの配列範囲へのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pDstTexture != NULL
    * - pSrcColorTarget != NULL
    * - pDstTexture が初期化されている
    * - pSrcColorTarget が初期化されている
    *
    * @post
    * - マルチサンプルカラーターゲットを解決するコマンドが追加されている
    *
    * @details
    * pSrcArrayRange で指定される配列範囲をすべて、dstStartArrayIndex から始まる配列範囲に解決します。@n
    * pSrcArrayRange が NULL の場合は全配列範囲を解決します。
    */
    void Resolve( TTexture< Target >* pDstTexture, int dstMipLevel, int dstStartArrayIndex,
        const TColorTargetView< Target >* pSrcColorTarget, const TextureArrayRange* pSrcArrayRange ) NN_NOEXCEPT
    {
        return Impl::Resolve( pDstTexture, dstMipLevel, dstStartArrayIndex, pSrcColorTarget, pSrcArrayRange );
    }

    /**
    * @brief メモリーの GPU 書き込みキャッシュをフラッシュします。
    *
    * @param[in] gpuAccessFlags nn::gfx::GpuAccess の組み合わせによる、フラッシュしたい GPU がアクセスするメモリー
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - メモリーキャッシュをフラッシュするコマンドが追加されている
    */
    void FlushMemory( int gpuAccessFlags ) NN_NOEXCEPT
    {
        return Impl::FlushMemory( gpuAccessFlags );
    }

    /**
    * @brief メモリーの GPU 読み込みキャッシュを無効化します。
    *
    * @param[in] gpuAccessFlags nn::gfx::GpuAccess の組み合わせによる、無効化したい GPU がアクセスするメモリー
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - メモリーキャッシュを無効化するコマンドが追加されている
    */
    void InvalidateMemory( int gpuAccessFlags ) NN_NOEXCEPT
    {
        return Impl::InvalidateMemory( gpuAccessFlags );
    }

    /**
    * @brief 別のコマンドバッファーを呼び出すコマンドを追加します。
    *
    * @param[in] pNestedCommandBuffer 対象のネストされるコマンドバッファー
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pNestedCommandBuffer != NULL
    * - pNestedCommandBuffer が初期化されている
    * - pNestedCommandBuffer がコマンドの追加を終了している
    * - pNestedCommandBuffer がネストされるコマンドバッファーである
    *
    * @post
    * - 別のコマンドバッファーを呼び出すコマンドが追加されている
    */
    void CallCommandBuffer( const TCommandBuffer< Target >* pNestedCommandBuffer ) NN_NOEXCEPT
    {
        return Impl::CallCommandBuffer( pNestedCommandBuffer );
    }

    /**
    * @brief 別のコマンドバッファーに追加されているコマンドを追加します。
    *
    * @param[in] pNestedCommandBuffer 対象のネストされるコマンドバッファー
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pNestedCommandBuffer != NULL
    * - pNestedCommandBuffer が初期化されている
    * - pNestedCommandBuffer がコマンドの追加を終了している
    * - pNestedCommandBuffer がネストされるコマンドバッファーである
    *
    * @post
    * - 別のコマンドバッファーに追加されているコマンドが追加されている
    */
    void CopyCommandBuffer( const TCommandBuffer< Target >* pNestedCommandBuffer ) NN_NOEXCEPT
    {
        return Impl::CopyCommandBuffer( pNestedCommandBuffer );
    }

    /**
    * @brief バッファーステートを遷移させるコマンドを追加します。
    *
    * @param[in] pBuffer 対象のバッファーへのポインター
    * @param[in] oldState nn::gfx::BufferState の組み合わせで表現される以前のバッファーステート
    * @param[in] oldStageBits nn::gfx::PipelineStageBit の組み合わせで表現される以前のシェーダーステージ
    * @param[in] newState nn::gfx::BufferState の組み合わせで表現される遷移対象のバッファーステート
    * @param[in] newStageBits nn::gfx::PipelineStageBit の組み合わせで表現される遷移後のシェーダーステージ
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pBuffer != NULL
    * - pBuffer が初期化されている
    *
    * @post
    * - バッファーステートを遷移させるコマンドが追加されている
    */
    void SetBufferStateTransition( TBuffer< Target >* pBuffer, int oldState,
        int oldStageBits, int newState, int newStageBits ) NN_NOEXCEPT
    {
        return Impl::SetBufferStateTransition( pBuffer, oldState, oldStageBits, newState, newStageBits );
    }

    /**
    * @brief テクスチャーステートを遷移させるコマンドを追加します。
    *
    * @param[in] pTexture 対象のテクスチャーへのポインター
    * @param[in] pRange 対象のサブリソースの範囲
    * @param[in] oldState nn::gfx::TextureState の組み合わせで表現される以前のテクスチャーステート
    * @param[in] oldStageBits nn::gfx::PipelineStageBit の組み合わせで表現される以前のシェーダーステージ
    * @param[in] newState nn::gfx::TextureState の組み合わせで表現される遷移対象のテクスチャーステート
    * @param[in] newStageBits nn::gfx::PipelineStageBit の組み合わせで表現される遷移後のシェーダーステージ
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pTexture != NULL
    * - pTexture が初期化されている
    *
    * @post
    * - テクスチャーステートを遷移させるコマンドが追加されている
    *
    * @details
    * pRange == NULL の場合は全範囲が対象と解釈されます。
    */
    void SetTextureStateTransition( TTexture< Target >* pTexture, TextureSubresourceRange* pRange,
        int oldState, int oldStageBits, int newState, int newStageBits ) NN_NOEXCEPT
    {
        return Impl::SetTextureStateTransition( pTexture, pRange, oldState, oldStageBits, newState, newStageBits );
    }

    /**
    * @brief デスクリプタープールをセットします。
    *
    * @param[in] pDescriptorPool 対象のデスクリプタープールへのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - デスクリプタープールがセットされている
    */
    void SetDescriptorPool( const TDescriptorPool< Target >* pDescriptorPool ) NN_NOEXCEPT
    {
        return Impl::SetDescriptorPool( pDescriptorPool );
    }

    /**
    * @brief ルートシグネチャーをセットします。
    *
    * @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
    *
    * @param[in] pipelineType 対象のパイプラインの種類
    * @param[in] pRootSignature ルートシグネチャーへのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - ルートシグネチャーがセットされている
    *
    * @details
    * ルートシグネチャーの設定はコマンドバッファーの間で継承されないため、ネストされるコマンドバッファーにおいてもセットする必要があります。
    */
    NN_DEPRECATED void SetRootSignature( PipelineType pipelineType, TRootSignature< Target >* pRootSignature ) NN_NOEXCEPT
    {
        return Impl::SetRootSignature( pipelineType, pRootSignature );
    }

    /**
    * @brief バッファーのデスクリプターテーブルをセットします。
    *
    * @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
    *
    * @param[in] pipelineType 対象のパイプラインの種類
    * @param[in] indexDescriptorTable ルートシグネチャーにおける対象のデスクリプターテーブルのインデックス
    * @param[in] startBufferDescriptorSlot バッファーのデスクリプタースロットの開始位置
    *
    * @pre
    * - コマンドの追加が開始されている
    * - indexDescriptorTable >= 0
    * - indexDescriptorTable がルートシグネチャーにおけるデスクリプターテーブルの数より小さい
    *
    * @post
    * - バッファーのデスクリプターテーブルがセットされている
    */
    NN_DEPRECATED void SetRootBufferDescriptorTable( PipelineType pipelineType, int indexDescriptorTable,
        const DescriptorSlot& startBufferDescriptorSlot ) NN_NOEXCEPT
    {
        return Impl::SetRootBufferDescriptorTable( pipelineType, indexDescriptorTable, startBufferDescriptorSlot );
    }

    /**
    * @brief テクスチャーとサンプラーのデスクリプターテーブルをセットします。
    *
    * @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
    *
    * @param[in] pipelineType 対象のパイプラインの種類
    * @param[in] indexDescriptorTable ルートシグネチャーにおける対象のデスクリプターテーブルのインデックス
    * @param[in] startTextureDescriptorSlot テクスチャーのデスクリプタースロットの開始位置
    * @param[in] startSamplerDescriptorSlot サンプラーのデスクリプタースロットの開始位置
    *
    * @pre
    * - コマンドの追加が開始されている
    * - indexDescriptorTable >= 0
    * - indexDescriptorTable がルートシグネチャーにおけるデスクリプターテーブルの数より小さい
    *
    * @post
    * - テクスチャーとサンプラーのデスクリプターテーブルがセットされている
    */
    NN_DEPRECATED void SetRootTextureAndSamplerDescriptorTable( PipelineType pipelineType, int indexDescriptorTable,
        const DescriptorSlot& startTextureDescriptorSlot, const DescriptorSlot& startSamplerDescriptorSlot ) NN_NOEXCEPT
    {
        return Impl::SetRootTextureAndSamplerDescriptorTable( pipelineType, indexDescriptorTable,
            startTextureDescriptorSlot, startSamplerDescriptorSlot );
    }

    /**
    * @brief 動的な定数バッファーをセットします。
    *
    * @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
    *
    * @param[in] pipelineType 対象のパイプラインの種類
    * @param[in] indexDynamicDescriptor ルートシグネチャーにおける対象の動的デスクリプターのインデックス
    * @param[in] constantBufferAddress 対象の定数バッファーの GPU アドレス
    * @param[in] size バイトでのセットする定数バッファーのサイズ
    *
    * @pre
    * - コマンドの追加が開始されている
    * - indexDynamicDescriptor >= 0
    * - indexDynamicDescriptor がルートシグネチャーにおける動的デスクリプターの数より小さい
    *
    * @post
    * - 動的な定数バッファーがセットされている
    */
    NN_DEPRECATED void SetRootConstantBuffer( PipelineType pipelineType, int indexDynamicDescriptor,
        const GpuAddress& constantBufferAddress, size_t size ) NN_NOEXCEPT
    {
        return Impl::SetRootConstantBuffer( pipelineType, indexDynamicDescriptor, constantBufferAddress, size );
    }

    /**
    * @brief 動的なアンオーダードアクセスバッファーをセットします。
    *
    * @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
    *
    * @param[in] pipelineType 対象のパイプラインの種類
    * @param[in] indexDynamicDescriptor ルートシグネチャーにおける対象の動的デスクリプターのインデックス
    * @param[in] unorderedAccessBufferAddress 対象のアンオーダードバッファーの GPU アドレス
    * @param[in] size バイトでのセットする定数バッファーのサイズ
    *
    * @pre
    * - コマンドの追加が開始されている
    * - indexDynamicDescriptor >= 0
    * - indexDynamicDescriptor がルートシグネチャーにおける動的デスクリプターの数より小さい
    *
    * @post
    * - 動的なアンオーダードバッファーがセットされている
    */
    NN_DEPRECATED void SetRootUnorderedAccessBuffer( PipelineType pipelineType, int indexDynamicDescriptor,
        const GpuAddress& unorderedAccessBufferAddress, size_t size ) NN_NOEXCEPT
    {
        return Impl::SetRootUnorderedAccessBuffer( pipelineType,
            indexDynamicDescriptor, unorderedAccessBufferAddress, size );
    }

    /**
    * @brief 動的なテクスチャーとサンプラーをセットします。
    *
    * @deprecated 廃止予定です。nn::gfx::CommandBuffer::SetTextureAndSampler 等のバインド API を使用してください。
    *
    * @param[in] pipelineType 対象のパイプラインの種類
    * @param[in] indexDynamicDescriptor ルートシグネチャーにおける対象の動的デスクリプターのインデックス
    * @param[in] pTextureView 動的なテクスチャーへのポインター
    * @param[in] pSampler 動的なサンプラーへのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - indexDynamicDescriptor >= 0
    * - indexDynamicDescriptor がルートシグネチャーにおける動的デスクリプターの数より小さい
    * - pTextureView != NULL
    * - pTextureView が初期化されている
    * - pSampler != NULL
    * - pSampler が初期化されている
    *
    * @post
    * - 動的なテクスチャーとサンプラーがセットされている
    */
    NN_DEPRECATED void SetRootTextureAndSampler( PipelineType pipelineType, int indexDynamicDescriptor,
        const TTextureView< Target >* pTextureView, const TSampler< Target >* pSampler ) NN_NOEXCEPT
    {
        return Impl::SetRootTextureAndSampler( pipelineType, indexDynamicDescriptor, pTextureView, pSampler );
    }

    /**
    * @brief クエリーを開始するコマンドを追加します。
    *
    * @param[in] target 開始するクエリーターゲット
    *
    * @pre
    * - コマンドの追加が開始されている
    * - target が QueryTarget_Timestamp でない
    *
    * @post
    * - クエリーを開始するコマンドが追加されている
    */
    void BeginQuery( QueryTarget target ) NN_NOEXCEPT
    {
        return Impl::BeginQuery( target );
    }

    /**
    * @brief クエリーを終了し結果をバッファーに書き込むコマンドを追加します。
    *
    * @param[in] dstBufferAddress 結果を書き込むバッファーのアドレス
    * @param[in] target 終了するクエリーターゲット
    *
    * @pre
    * - コマンドの追加が開始されている
    * - target が QueryTarget_Timestamp でない
    *
    * @post
    * - クエリーを終了し結果をバッファーに書き込むコマンドが追加されている
    *
    * @details
    * 書き込まれる値やそのフォーマットは使用している低レベルグラフィックス API に依存します。
    */
    void EndQuery( const GpuAddress& dstBufferAddress, QueryTarget target ) NN_NOEXCEPT
    {
        return Impl::EndQuery( dstBufferAddress, target );
    }

    /**
    * @brief 深度境界テストの範囲を設定します。
    *
    * @param[in] minDepthBounds 深度境界テストに使われる範囲の最小値
    * @param[in] maxDepthBounds 深度境界テストに使われる範囲の最大値
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - 深度境界テストの範囲が設定されている
    *
    * @details
    * 深度境界テストを有効にした深度ステンシルステートを設定した場合は、
    * その後、ドローコールを行う前にこの関数を呼ぶ必要があります。@n
    * 深度境界テストを無効にした深度ステンシルステートを設定した場合は、
    * 次に有効にするまでこの関数を呼んではいけません。
    */
    void SetDepthBounds( float minDepthBounds, float maxDepthBounds ) NN_NOEXCEPT
    {
        return Impl::SetDepthBounds( minDepthBounds, maxDepthBounds );
    }

    /**
    * @brief 線プリミティブの幅を設定します。
    *
    * @param[in] lineWidth ピクセル単位での線プリミティブの幅
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - 線プリミティブの幅が設定されている
    */
    void SetLineWidth( float lineWidth ) NN_NOEXCEPT
    {
        return Impl::SetLineWidth( lineWidth );
    }

    /**
    * @brief ビューポートをセットするコマンドを追加します。
    *
    * @param[in] firstViewport セットする最初のビューポートのインデックス
    * @param[in] viewportCount セットする連続的なビューポートの数
    * @param[in] pViewports viewportCount 個の要素をもつビューポート情報の配列へのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - ビューポートをセットするコマンドが追加されている
    *
    * @details
    * firstViewport から firstViewport + viewportCount - 1 までのインデックスのビューポートを更新します。
    * pViewports が指す配列の i 番目の要素で firstViewport + i のインデックスのビューポートを更新します。
    */
    void SetViewports( int firstViewport, int viewportCount,
        const ViewportStateInfo* pViewports ) NN_NOEXCEPT
    {
        return Impl::SetViewports( firstViewport, viewportCount, pViewports );
    }

    /**
    * @brief シザーをセットするコマンドを追加します。
    *
    * @param[in] firstScissor セットする最初のシザーのインデックス
    * @param[in] scissorCount セットする連続的なシザーの数
    * @param[in] pScissors scissorCount 個の要素をもつシザー情報の配列へのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - シザーをセットするコマンドが追加されている
    *
    * @details
    * firstScissor から firstScissor + scissorCount - 1 までのインデックスのシザーを更新します。
    * pScissors が指す配列の i 番目の要素で firstScissor + i のインデックスのシザーを更新します。
    */
    void SetScissors( int firstScissor, int scissorCount,
        const ScissorStateInfo* pScissors ) NN_NOEXCEPT
    {
        return Impl::SetScissors( firstScissor, scissorCount, pScissors );
    }

    /**
    * @brief タイムスタンプの値をバッファーに書き込むコマンドを追加します。
    *
    * @param[in] dstBufferAddress 結果を書き込むバッファーのアドレス
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - タイムスタンプの値をバッファーに書き込むコマンドが追加されている
    *
    * @details
    * 書き込まれる値やそのフォーマットは使用している低レベルグラフィックス API に依存します。
    */
    void WriteTimestamp( const GpuAddress& dstBufferAddress ) NN_NOEXCEPT
    {
        return Impl::WriteTimestamp( dstBufferAddress );
    }

    /**
    * @brief 指定されたバッファーの指定された範囲をコマンドから更新します。
    *
    * @param[in] dstBufferAddress 更新するバッファーのアドレス
    * @param[in] bufferSize 更新するバッファーのバインドされているバイトサイズ
    * @param[in] dstOffset 更新を開始するバッファーへのバイトオフセット
    * @param[in] dataSize 更新するデータのバイト数
    * @param[in] pData 更新するソースデータへのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - dstOffset >= 0
    * - dstOffset + dataSize <= bufferSize
    * - dataSize > 0 のとき pData != NULL
    * - pData の指し先が dataSize 以上のデータをもつ
    *
    * @post
    * - バッファーの範囲をコマンドから更新するコマンドが追加されている
    */
    void UpdateBuffer( const GpuAddress& dstBufferAddress, size_t bufferSize,
        ptrdiff_t dstOffset, size_t dataSize, const void* pData ) NN_NOEXCEPT
    {
        return Impl::UpdateBuffer( dstBufferAddress, bufferSize, dstOffset, dataSize, pData );
    }

    /**
    * @brief デバッグ用に特定の範囲を注釈するためのデバッググループをデバッググループスタックにプッシュします。
    *
    * @param[in] description NULL で終端されるデバッググループの注釈文章の文字列へのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - description != NULL
    *
    * @details
    * PushDebugGroup と PopDebugGroup で区切られる範囲を注釈します。@n
    * デバッググループは階層化することができ、PushDebugGroup と PopDebugGroup は交互に現れる必要はありません。@n
    * 最終的にキューに提出されるとき、PushDebugGroup と PopDebugGroup は釣り合いがとれている必要があります。
    */
    void PushDebugGroup( const char* description ) NN_NOEXCEPT
    {
        return Impl::PushDebugGroup( description );
    }

    /**
    * @brief デバッググループスタックから現在アクティブなデバッググループをポップします。
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @details
    * PushDebugGroup と PopDebugGroup で区切られる範囲を注釈します。@n
    * デバッググループは階層化することができ、PushDebugGroup と PopDebugGroup は交互に現れる必要はありません。@n
    * 最終的にキューに提出されるとき、PushDebugGroup と PopDebugGroup は釣り合いがとれている必要があります。
    */
    void PopDebugGroup() NN_NOEXCEPT
    {
        return Impl::PopDebugGroup();
    }

    //! @name ルートシグネチャーを使わない場合
    // @{

    /**
    * @brief 定数バッファーをセットするコマンドを追加します。
    *
    * @param[in] slot セットする対象のスロット
    * @param[in] stage セットする対象のシェーダーステージ
    * @param[in] constantBufferDescriptor 対象の定数バッファーのデスクリプター
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - 定数バッファーをセットするコマンドが追加されている
    */
    void SetConstantBuffer( int slot, ShaderStage stage, const DescriptorSlot& constantBufferDescriptor ) NN_NOEXCEPT
    {
        return Impl::SetConstantBuffer( slot, stage, constantBufferDescriptor );
    }

    /**
    * @brief アンオーダードアクセスバッファーをセットするコマンドを追加します。
    *
    * @param[in] slot セットする対象のスロット
    * @param[in] stage セットする対象のシェーダーステージ
    * @param[in] unorderedAccessBufferDescriptor 対象のアンオーダードアクセスバッファーのデスクリプター
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - アンオーダードアクセスバッファーをセットするコマンドが追加されている
    */
    void SetUnorderedAccessBuffer( int slot, ShaderStage stage, const DescriptorSlot& unorderedAccessBufferDescriptor ) NN_NOEXCEPT
    {
        return Impl::SetUnorderedAccessBuffer( slot, stage, unorderedAccessBufferDescriptor );
    }

    /**
    * @brief テクスチャーとサンプラーをセットするコマンドを追加します。
    *
    * @param[in] slot セットする対象のスロット
    * @param[in] stage セットする対象のシェーダーステージ
    * @param[in] textureDescriptor 対象のテクスチャーのデスクリプター
    * @param[in] samplerDescriptor 対象のサンプラーのデスクリプター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - slot >= 0
    *
    * @post
    * - テクスチャーとサンプラーをセットするコマンドが追加されている
    */
    void SetTextureAndSampler( int slot, ShaderStage stage,
        const DescriptorSlot& textureDescriptor, const DescriptorSlot& samplerDescriptor ) NN_NOEXCEPT
    {
        return Impl::SetTextureAndSampler( slot, stage, textureDescriptor, samplerDescriptor );
    }

    /**
    * @brief テクスチャーをセットするコマンドを追加します。
    *
    * @param[in] slot セットする対象のスロット
    * @param[in] stage セットする対象のシェーダーステージ
    * @param[in] textureDescriptor 対象のテクスチャーのデスクリプター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - slot >= 0
    *
    * @post
    * - テクスチャーをセットするコマンドが追加されている
    *
    * @details
    * サンプラーと組み合わせずにテクスチャーを単独でセットします。
    * GLSL シェーダー言語における texelFetch に使うテクスチャーをセットするために使用できます。
    * サンプラーと組み合わせるテクスチャーについては SetTextureAndSampler を使用してください。
    */
    void SetTexture( int slot, ShaderStage stage, const DescriptorSlot& textureDescriptor ) NN_NOEXCEPT
    {
        return Impl::SetTexture( slot, stage, textureDescriptor );
    }

    /**
    * @brief イメージをセットするコマンドを追加します。
    *
    * @param[in] slot セットする対象のスロット
    * @param[in] stage セットする対象のシェーダーステージ
    * @param[in] imageDescriptor 対象のイメージのデスクリプター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - slot >= 0
    *
    * @post
    * - イメージをセットするコマンドが追加されている
    */
    void SetImage( int slot, ShaderStage stage, const DescriptorSlot& imageDescriptor ) NN_NOEXCEPT
    {
        return Impl::SetImage( slot, stage, imageDescriptor );
    }

    // @}

    //! @name デスクリプタープールを使わない場合
    // @{

    /**
    * @brief テクスチャーとサンプラーをセットするコマンドを追加します。
    *
    * @param[in] slot セットする対象のスロット
    * @param[in] stage セットする対象のシェーダーステージ
    * @param[in] pTextureView 対象のテクスチャービューへのポインター
    * @param[in] pSampler 対象のサンプラーへのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pTextureView != NULL
    * - pTextureView が初期化されている
    * - pSampler != NULL
    * - pSampler が初期化されている
    *
    * @post
    * - テクスチャーとサンプラーをセットするコマンドが追加されている
    *
    * @details
    * slot へはシェーダーから問い合わせた番号を指定します。
    */
    void SetTextureAndSampler( int slot, ShaderStage stage,
        const TTextureView< Target >* pTextureView,
        const TSampler< Target >* pSampler ) NN_NOEXCEPT
    {
        return Impl::SetTextureAndSampler( slot, stage, pTextureView, pSampler );
    }

    /**
    * @brief イメージをセットするコマンドを追加します。
    *
    * @param[in] slot セットする対象のスロット
    * @param[in] stage セットする対象のシェーダーステージ
    * @param[in] pImage 対象のイメージへのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pImage != NULL
    * - pImage が初期化されている
    *
    * @post
    * - イメージをセットするコマンドが追加されている
    *
    * @details
    * slot へはシェーダーから問い合わせた番号を指定します。
    */
    void SetImage( int slot, ShaderStage stage, const TTextureView< Target >* pImage ) NN_NOEXCEPT
    {
        return Impl::SetImage( slot, stage, pImage );
    }

    /**
    * @brief 定数バッファーをセットするコマンドを追加します。
    *
    * @param[in] slot セットする対象のスロット
    * @param[in] stage セットする対象のシェーダーステージ
    * @param[in] constantBufferAddress 対象の定数バッファーの GPU アドレス
    * @param[in] size 対象の定数バッファーのサイズ
    *
    * @pre
    * - コマンドの追加が開始されている
    *
    * @post
    * - 定数バッファーをセットするコマンドが追加されている
    *
    * @details
    * slot へはシェーダーから問い合わせた番号を指定します。
    */
    void SetConstantBuffer( int slot, ShaderStage stage,
        const GpuAddress& constantBufferAddress, size_t size ) NN_NOEXCEPT
    {
        return Impl::SetConstantBuffer( slot, stage, constantBufferAddress, size );
    }

    /**
    * @brief アンオーダードアクセスバッファーをセットするコマンドを追加します。
    *
    * @param[in] slot セットする対象のスロット
    * @param[in] stage セットする対象のシェーダーステージ
    * @param[in] unorderedAccessBufferAddress 対象のアンオーダードアクセスバッファーの GPU アドレス
    * @param[in] size 対象のアンオーダードアクセスバッファーのサイズ
    *
    * @pre
    * - コマンドの追加が開始されている

    * @post
    * - アンオーダードアクセスバッファーをセットするコマンドが追加されている
    *
    * @details
    * slot へはシェーダーから問い合わせた番号を指定します。
    */
    void SetUnorderedAccessBuffer( int slot, ShaderStage stage,
        const GpuAddress& unorderedAccessBufferAddress, size_t size ) NN_NOEXCEPT
    {
        return Impl::SetUnorderedAccessBuffer( slot, stage, unorderedAccessBufferAddress, size );
    }

    // @}

    //! @name パイプラインを使わない場合
    // @{

    /**
    * @brief シェーダーをセットするコマンドを追加します。
    *
    * @param[in] pShader 対象のシェーダーへのポインター
    * @param[in] stageBits nn::gfx::ShaderStageBit の組み合わせで表現されるセットするシェーダーステージ
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pShader != NULL の場合、pShader が初期化されている
    *
    * @post
    * - シェーダーをセットするコマンドが追加されている
    *
    * @details
    * pShader == NULL または stageBits で示されるステージを pShader が所有していない場合は対象のステージを無効化します。
    *
    * @platformbegin{GX}
    * ステージを無効化する機能は扱われません。適切に GX2 API の GX2SetShaderMode 関数を直接呼び出してください。
    * @platformend
    */
    void SetShader( const TShader< Target >* pShader, int stageBits ) NN_NOEXCEPT
    {
        return Impl::SetShader( pShader, stageBits );
    }

    /**
    * @brief ラスタライザーステートをセットするコマンドを追加します。
    *
    * @param[in] pRasterizerState 対象のラスタライザーステートへのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pRasterizerState != NULL
    * - pRasterizerState が初期化されている
    *
    * @post
    * - ラスタライザーステートをセットするコマンドが追加されている
    */
    void SetRasterizerState( const TRasterizerState< Target >* pRasterizerState ) NN_NOEXCEPT
    {
        return Impl::SetRasterizerState( pRasterizerState );
    }

    /**
    * @brief ブレンドステートをセットするコマンドを追加します。
    *
    * @param[in] pBlendState 対象のブレンドステートへのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pBlendState != NULL
    * - pBlendState が初期化されている
    *
    * @post
    * - ブレンドステートをセットするコマンドが追加されている
    */
    void SetBlendState( const TBlendState< Target >* pBlendState ) NN_NOEXCEPT
    {
        return Impl::SetBlendState( pBlendState );
    }

    /**
    * @brief 深度ステンシルステートをセットするコマンドを追加します。
    *
    * @param[in] pDepthStencilState 対象の深度ステンシルステートへのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pDepthStencilState != NULL
    * - pDepthStencilState が初期化されている
    *
    * @post
    * - 深度ステンシルステートをセットするコマンドが追加されている
    */
    void SetDepthStencilState( const TDepthStencilState< Target >* pDepthStencilState ) NN_NOEXCEPT
    {
        return Impl::SetDepthStencilState( pDepthStencilState );
    }

    /**
    * @brief 頂点ステートをセットするコマンドを追加します。
    *
    * @param[in] pVertexState 対象の頂点ステートへのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pVertexState != NULL
    * - pVertexState が初期化されている
    *
    * @post
    * 頂点ステートをセットするコマンドが追加されている。
    */
    void SetVertexState( const TVertexState< Target >* pVertexState ) NN_NOEXCEPT
    {
        return Impl::SetVertexState( pVertexState );
    }

    /**
    * @brief テッセレーションステートをセットするコマンドを追加します。
    *
    * @param[in] pTessellationState 対象のテッセレーションステートへのポインター
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pTessellationState != NULL
    * - pTessellationState が初期化されている
    *
    * @post
    * テッセレーションステートをセットするコマンドが追加されている。
    */
    void SetTessellationState( const TTessellationState< Target >* pTessellationState ) NN_NOEXCEPT
    {
        return Impl::SetTessellationState( pTessellationState );
    }

    // @}

    /**
    * @brief 任意のユーザーポインターを設定します。
    *
    * @param[in] value 設定するユーザーポインター
    *
    * @post
    * - ユーザーポインターが設定されている
    */
    void SetUserPtr( void* value ) NN_NOEXCEPT
    {
        return SetUserPtrImpl( this, value );
    }

    /**
    * @brief 設定されている任意のユーザーポインターを取得します。
    *
    * @return 設定されている任意のユーザーポインターを返します。
    */
    void* GetUserPtr() NN_NOEXCEPT
    {
        return GetUserPtrImpl( this );
    }

    /**
    * @brief 設定されている任意のユーザーポインターを取得します。
    *
    * @return 設定されている任意のユーザーポインターを返します。
    */
    const void* GetUserPtr() const NN_NOEXCEPT
    {
        return GetUserPtrImpl( this );
    }

    //! @name GL4 版専用
    // @{

    /**
    * @brief ユーザー定義のコマンドのコールバックの型です。この型定義は GL4 版専用です。
    */
    typedef void ( *Gl4UserCommandCallbackType )( const void* pParam );

    /**
    * @brief ユーザー定義のコマンドを追加します。この関数は GL4 版専用です。
    *
    * @param[in] pCallback 実行されるコマンドのコールバックへのポインター
    * @param[in] pParam 実行されるコールバックコマンドへの引数
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pCallback != NULL
    *
    * @post
    * ユーザ定義のコマンドが追加されている。
    *
    * @details
    * コマンド実行の間に直接 GL API を呼び出したいときにこの関数を使います。@n
    * GL 版以外ではこの関数を呼ぶことはできません。
    */
    void Gl4SetUserCommand( Gl4UserCommandCallbackType pCallback, const void* pParam ) NN_NOEXCEPT
    {
        return Impl::Gl4SetUserCommand( pCallback, pParam );
    }

    /**
    * @brief ユーザー定義のコマンドとパラメーターを追加します。この関数は GL4 版専用です。
    *
    * @param[in] pCallback 実行されるコマンドのコールバックへのポインター
    * @param[in] pParam パラメーターへのポインター
    * @param[in] paramSize パラメーターのサイズ
    *
    * @pre
    * - コマンドの追加が開始されている
    * - pCallback != NULL
    *
    * @post
    * ユーザ定義のコマンドとパラメーターが追加されている。
    *
    * @details
    * コマンド実行の間に直接 GL API を呼び出したいときにこの関数を使います。@n
    * GL 版以外ではこの関数を呼ぶことはできません。@n
    * pParam の指し先から paramSize 分のパラメーターがコマンドメモリーにコピーされて追加され、コールバック関数への引数として使用されます。
    */
    void Gl4SetUserCommandDynamic( Gl4UserCommandCallbackType pCallback, const void* pParam, size_t paramSize ) NN_NOEXCEPT
    {
        return Impl::Gl4SetUserCommandDynamic( pCallback, pParam, paramSize );
    }

    // @}
};

}
}

NN_PRAGMA_POP_WARNINGS
