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

#include <nn/vfx/vfx_Data.h>
#include <nn/vfx/vfx_VertexBuffer.h>
#include <nn/vfx/vfx_Callback.h>
#include <nn/vfx/vfx_System.h>

namespace nn {
namespace vfx {

// 前方宣言
class Heap;
class Emitter;

namespace detail {

//------------------------------------------------------------------------------
//! @brief  ストライプ定数
//------------------------------------------------------------------------------
enum StripeConfiguration
{
    StripeConfiguration_MaxHistoryCount = 256,                      //!< 最大履歴数
};

//------------------------------------------------------------------------------
//! @brief  ストライプ定数
//------------------------------------------------------------------------------
enum StripeMeshType
{
    StripeMeshType_StandardStripe   = 0,                            //!< 通常ストライプ
    StripeMeshType_CrossStripe      = 1,                            //!< クロスストライプ
};

//---------------------------------------------------------------------------
//! @brief ストライプ展開方向
//---------------------------------------------------------------------------
enum StripeOrientationType
{
    StripeOrientationType_Billboard         = 0,                    //!< 「ビルボード」
    StripeOrientationType_EmitterMatrix     = 1,                    //!< 「エミッタ行列」
    StripeOrientationType_EmitterUpright    = 2,                    //!< 「エミッタ上下に伸ばす」
    StripeOrientationType_Ribbon            = 3,                    //!< 「リボン」
};

//---------------------------------------------------------------------------
//! @brief テクスチャリング
//---------------------------------------------------------------------------
enum StripeTexturingOption
{
    StripeTexturingOption_Fill = 0,                                 //!< 「全て貼る」
    StripeTexturingOption_Tile = 1,                                 //!< 「描画範囲に合わせて貼る」
    StripeTexturingOption_MaxType = 2,                              //!< タイプ総数
};

//------------------------------------------------------------------------------
//! @brief      ストライプの基本機能をまとめたクラスです。
//! @details    共通する処理フローをここに集約して部品化します。
//------------------------------------------------------------------------------
class StripeSystemUtility
{
public:
    //---------------------------------------------------------------------------
    //! @brief                      空きストライプインスタンスを確保します。
    //! @tparam    StripeSystemType ストライプシステムクラス
    //! @param[in] pStripeSystem    ストライプシステムへのポインタ
    //! @param[in] pEmitter         エミッタ
    //---------------------------------------------------------------------------
    template< class StripeSystemType >
    static typename StripeSystemType::StripeInstance* AllocStripe( StripeSystemType* pStripeSystem, Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                      ストライプインスタンスを返却します。
    //! @tparam    StripeSystemType ストライプシステムクラス
    //! @param[in] pStripeSystem    ストライプシステムへのポインタ
    //! @param[in] pStripe          ストライプインスタンス
    //! @param[in] pEmitter         エミッタ
    //---------------------------------------------------------------------------
    template< class StripeSystemType >
    static void FreeStripe( StripeSystemType* pStripeSystem, typename StripeSystemType::StripeInstance* pStripe, Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                      ストライプを遅延リストに移動させます。
    //! @tparam    StripeSystemType ストライプシステムクラス
    //! @param[in] ppEpData         エミッタ固有データ
    //! @param[in] pStripe          ストライプインスタンス
    //---------------------------------------------------------------------------
    template< class StripeSystemType >
    static void MoveStripeToDelayList(
        typename StripeSystemType::EmitterPluginUserData** ppEpData,
        typename StripeSystemType::StripeInstance* pStripe ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                      ストライプを遅延リストに移動させます。
    //! @tparam    StripeSystemType ストライプシステムクラス
    //! @param[in] pStripeSystem    ストライプシステムへのポインタ
    //! @param[in] ppEpData         エミッタ固有データ
    //! @param[in] pEmitter         エミッタ
    //---------------------------------------------------------------------------
    template< class StripeSystemType >
    static void FreeAllDelayedStripe( StripeSystemType* pStripeSystem, typename StripeSystemType::EmitterPluginUserData** ppEpData, Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                          デフォルトのシェーダ設定処理を行います。
    //! @param[in] pCommandBuffer       コマンドバッファ
    //! @param[in] pEmitter             エミッタ
    //! @param[in] shaderType           シェーダタイプ
    //! @param[in] pShader              シェーダ
    //! @param[in] pUserParam           ユーザーパラメータへのポインタ
    //! @param[in] pDrawParameterArg    描画パラメータへのポインタ
    //! @return                         成功した場合true
    //---------------------------------------------------------------------------
    static bool SetupDefaultShaderSetting( nn::gfx::CommandBuffer* pCommandBuffer, Emitter* pEmitter, ShaderType shaderType, Shader* pShader, void* pUserParam, DrawParameterArg* pDrawParameterArg ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                          ストライプを遅延リストに移動させます。
    //! @tparam    StripeSystemType     ストライプシステムクラス
    //! @param[in] pCommandBuffer       gfx コマンドバッファ
    //! @param[in] pSystem              Systemクラスへのポインタ
    //! @param[in] pEmitter             エミッタ
    //! @param[in] shaderType           シェーダタイプ
    //! @param[in] pUserParam           ユーザーパラメータへのポインタ
    //! @param[in] pDrawParameterArg    描画パラメータへのポインタ
    //---------------------------------------------------------------------------
    template< class StripeSystemType >
    static void DrawParticleStripeEmitter(
        nn::gfx::CommandBuffer* pCommandBuffer,
        System* const pSystem,
        Emitter* pEmitter,
        ShaderType shaderType,
        void* pUserParam,
        DrawParameterArg* pDrawParameterArg ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                          ストライプを遅延リストに移動させます。
    //! @tparam    StripeSystemType     ストライプシステムクラス
    //! @param[in] pCommandBuffer       gfx コマンドバッファ
    //! @param[in] pDevice              gfx デバイス
    //! @param[in] pTempBuffer          テンポラリバッファ
    //! @param[in] pEmitter             エミッタ
    //! @param[in] pShader              シェーダ
    //! @param[in] pStripeData          ストライプ設定
    //! @param[in] pEPUserData          エミッタ固有データ
    //! @param[in] pStripe              ストライプインスタンス
    //---------------------------------------------------------------------------
    template< class StripeSystemType >
    static void DrawParticleStripe(
        nn::gfx::CommandBuffer* pCommandBuffer,
        nn::gfx::Device* pDevice,
        TemporaryBuffer* pTempBuffer,
        Emitter* pEmitter,
        Shader* pShader,
        const typename StripeSystemType::ResourceType* pStripeData,
        const typename StripeSystemType::EmitterPluginUserData* pEPUserData,
        const typename StripeSystemType::StripeInstance* pStripe ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                          ConstantBuffer をシェーダに設定します。
    //! @tparam    ConstantBufferType   セットする ConstantBuffer 構造体の型
    //! @param[in] pCommandBuffer       コマンドバッファ
    //! @param[in] pDevice              gfx デバイス
    //! @param[in] pTempBuffer          テンポラリバッファ
    //! @param[in] pShader              シェーダ
    //! @param[in] pConstantBuffer      ConstantBuffer へのポインタ
    //! @return                         成功した場合 true
    //---------------------------------------------------------------------------
    template< class ConstantBufferType >
    static bool SetConstantBufferObject(
        nn::gfx::CommandBuffer* pCommandBuffer,
        nn::gfx::Device* pDevice,
        TemporaryBuffer* pTempBuffer,
        Shader* pShader,
        ConstantBufferType* pConstantBuffer ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                                  ストライプ描画コマンドを発行します。
    //! @tparam    StripeType                   ストライプインスタンス型
    //! @tparam    EmitterPluginUserDataType    エミッタ固有データ型
    //! @param[in] pCommandBuffer               コマンドバッファ
    //! @param[in] pStripe                      ストライプ
    //! @param[in] pEPUserData                  エミッタ単位のストライプ設定
    //---------------------------------------------------------------------------
    template< class StripeType, class EmitterPluginUserDataType >
    static void SetDrawCommandForParticleStripe( nn::gfx::CommandBuffer* pCommandBuffer, const StripeType* pStripe, const EmitterPluginUserDataType* pEPUserData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  遅延ストライプの最大本数を計算します。
    //! @param[in] pEmitter     エミッタへのポインタ
    //! @param[in] numHistory   履歴数
    //! @return                 遅延ストライプの最大本数
    //---------------------------------------------------------------------------
    static int CalculateDelayStripeCount( const Emitter* pEmitter, int numHistory ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      履歴式ストライプ系で、エルミート補間で履歴点の間をつなぐ曲線上の位置を求める
    //! @tparam     StripeType  ストライプシステム型
    //! @param[out] pOutPos         位置
    //! @param[out] pOutDir         方向
    //! @param[in]  pHistory        履歴点へのポインタ
    //! @param[in]  histPos         履歴点の位置
    //! @param[in]  historyCount    履歴数
    //! @param[in]  innerPos        内部位置
    //---------------------------------------------------------------------------
    template< class StripeSystem >
    static void CalculateHermiteInterpolatedCurveVec(
        nn::util::Vector3fType* pOutPos,
        nn::util::Vector3fType* pOutDir,
        const typename StripeSystem::History* pHistory,
        int histPos,
        int historyCount,
        float innerPos ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      バッファサイドを更新するユーティリティ関数
    //! @param[out] pOutBufferSide  更新するバッファサイドへのポインタ
    //! @param[in]  pEmitter        エミッタへのポインタ
    //---------------------------------------------------------------------------
    static void SwapBufferSide( BufferSide* pOutBufferSide, Emitter* pEmitter ) NN_NOEXCEPT
    {
        const int maxBufferCount = pEmitter->GetEmitterSet()->GetSystem()->IsEnableTripleBuffer() ? 3 : 2;
        *pOutBufferSide = static_cast< nn::vfx::BufferSide >( ( *pOutBufferSide + 1 ) % maxBufferCount );
    }
};

//------------------------------------------------------------------------------
//! @brief                  NULL 代入付き動的ヒープからの解放
//! @param[in] ptr          アドレスへのポインタ
//! @param[in] pEmitter     エミッタへのポインタ
//------------------------------------------------------------------------------
inline void FreeFromDynamicHeapSafely( void** ptr, Emitter* pEmitter ) NN_NOEXCEPT
{
    if( *ptr != NULL )
    {
        pEmitter->GetDynamicHeap().Free( *ptr );
        *ptr = NULL;
    }
}

//------------------------------------------------------------------------------
//! @brief 描画設定後コールバック ダミー用
//------------------------------------------------------------------------------
inline bool DummyRenderStateSetCallback( nn::vfx::RenderStateSetArg& arg ) NN_NOEXCEPT
{
    NN_UNUSED( arg );
    return true;
}

//---------------------------------------------------------------------------
//! @brief  空きストライプインスタンスを確保します。
//---------------------------------------------------------------------------
template< class StripeSystemType >
typename StripeSystemType::StripeInstance* StripeSystemUtility::AllocStripe( StripeSystemType* pStripeSystem, Emitter* pEmitter ) NN_NOEXCEPT
{
    // 前回確保したインデックスから配列線形探査
    for( int i = 0; i < pStripeSystem->m_StripeArrayCount; ++i )
    {
        typename StripeSystemType::StripeInstance* stripe = &pStripeSystem->m_StripeArray[ pStripeSystem->m_StripeArrayIndex ];

        if( stripe->used == false )
        {
            // ヒストリ領域をこのタイミングで確保
            const typename StripeSystemType::ResourceType* stripeData = StripeSystemType::GetStripeData( pEmitter );
            const uint32_t maxHistory = static_cast< uint32_t >( stripeData->numHistory );
            stripe->historyArray = reinterpret_cast< typename StripeSystemType::History* >( pEmitter->GetDynamicHeap().Alloc( sizeof( typename StripeSystemType::History ) * maxHistory ) );
            if( stripe->historyArray == NULL )
            {
                Warning( pEmitter, detail::RuntimeWarningId_StripeDynamicHeapAllocationFailed );
                OutputWarning( "Buffer for stripe history is not enough\n" );
                return NULL;
            }

            // 双方向リングバッファを形成
            for( uint32_t j = 0; j < maxHistory - 1; ++j )
            {
                stripe->historyArray[ j ].pNext = &stripe->historyArray[ j + 1 ];
            }
            for( uint32_t j = 1; j < maxHistory; ++j )
            {
                stripe->historyArray[ j ].pPrev = &stripe->historyArray[ j - 1 ];
            }
            stripe->historyArray[ maxHistory - 1 ].pNext = &stripe->historyArray[ 0 ];
            stripe->historyArray[ 0 ].pPrev = &stripe->historyArray[ maxHistory - 1 ];
            stripe->pHistoryHead = &stripe->historyArray[ 0 ];
            stripe->pHistoryTail = &stripe->historyArray[ 0 ];

            stripe->used = true;
            stripe->time = 0;
            stripe->historyCount = 0;
            stripe->renderVertexCount = 0;
            stripe->pNextDelayedStripe = NULL;

            pStripeSystem->m_StripeCount++;
            return stripe;
        }
        if( ++pStripeSystem->m_StripeArrayIndex >= pStripeSystem->m_StripeArrayCount )
        {
            pStripeSystem->m_StripeArrayIndex = 0;
        }
    }

    //---------------------------------------------------------------------------
    // 空きインスタンスが見つからなかった場合
    //---------------------------------------------------------------------------
    Warning( pEmitter, detail::RuntimeWarningId_StripeAllocationFailed );
    OutputWarning( "There is no available Stripe instance.\n" );
    return NULL;
}


//---------------------------------------------------------------------------
//! @brief  ストライプインスタンスを返却します。
//---------------------------------------------------------------------------
template< class StripeSystemType >
void StripeSystemUtility::FreeStripe( StripeSystemType* pStripeSystem, typename StripeSystemType::StripeInstance* pStripe, Emitter* pEmitter ) NN_NOEXCEPT
{
    // TODO : 履歴の確保、解放がエミッタ生成破棄時に限る対応後、FreeFromDynamicHeapSafely で Free するように変更する
    //detail::FreeFromDynamicHeapSafely( reinterpret_cast< void** >( &pStripe->historyArray ), pEmitter );
    const typename StripeSystemType::ResourceType* stripeData = StripeSystemType::GetStripeData( pEmitter );
    const uint32_t maxHistory = static_cast< uint32_t >( stripeData->numHistory );
    pEmitter->GetDynamicHeap().Free( pStripe->historyArray, sizeof( typename StripeSystemType::History ) * maxHistory );
    pStripe->historyArray = nullptr;
    {
        // 値のクリア
        pStripe->used = false;
    }
    pStripeSystem->m_StripeCount--;
}

//---------------------------------------------------------------------------
//! @brief  ストライプインスタンスを返却します。
//---------------------------------------------------------------------------
template< class StripeSystemType >
inline void StripeSystemUtility::MoveStripeToDelayList(
    typename StripeSystemType::EmitterPluginUserData** ppEPUserData,
    typename StripeSystemType::StripeInstance* pStripe ) NN_NOEXCEPT
{
    // 遅延リストにストライプを繋ぐ
    if( ( *ppEPUserData )->pDelayedStripeListHead == NULL )
    {
        ( *ppEPUserData )->pDelayedStripeListHead = pStripe;
    }
    else
    {
        typename StripeSystemType::StripeInstance* temp = reinterpret_cast< typename StripeSystemType::StripeInstance* >( ( *ppEPUserData )->pDelayedStripeListHead );
        typename StripeSystemType::StripeInstance* tail = NULL;

        while( temp )
        {
            tail = temp;
            temp = temp->pNextDelayedStripe;
        }

        if( tail )
        {
            tail->pNextDelayedStripe = pStripe;
        }
    }
}

//---------------------------------------------------------------------------
//! @brief  全ての遅延解放ストライプを破棄します。
//---------------------------------------------------------------------------
template< class StripeSystemType >
void StripeSystemUtility::FreeAllDelayedStripe( StripeSystemType* pStripeSystem, typename StripeSystemType::EmitterPluginUserData** ppEPUserData, Emitter* pEmitter ) NN_NOEXCEPT
{
    // 遅延ストライプを解放
    if( ( *ppEPUserData )->pDelayedStripeListHead == NULL )
    {
        return;
    }

    typename StripeSystemType::StripeInstance* delayedStripe = ( *ppEPUserData )->pDelayedStripeListHead;
    while( delayedStripe )
    {
        FreeStripe( pStripeSystem, delayedStripe, pEmitter );
        delayedStripe = delayedStripe->pNextDelayedStripe;
    }
    ( *ppEPUserData )->pDelayedStripeListHead = NULL;
}

//---------------------------------------------------------------------------
//! @brief  ストライプ描画処理を行います。
//---------------------------------------------------------------------------
template< class StripeSystemType >
inline void StripeSystemUtility::DrawParticleStripeEmitter(
    nn::gfx::CommandBuffer* pCommandBuffer,
    System* const pSystem,
    Emitter* pEmitter,
    ShaderType shaderType,
    void* pUserParam,
    DrawParameterArg* pDrawParameterArg ) NN_NOEXCEPT
{
    const typename StripeSystemType::EmitterPluginUserData* pEpData = StripeSystemType::GetEmitterUserData( pEmitter );
    const typename StripeSystemType::ResourceType*          pStripeData = StripeSystemType::GetStripeData( pEmitter );
    nn::gfx::Device*                               pGfxDevice = pSystem->GetGfxDevice();
    TemporaryBuffer*                               pTempBuffer = pDrawParameterArg->m_pDrawTempBuffer;
    Shader* const                                  pShader = pEmitter->GetShader( shaderType );

    //---------------------------------------------------------------------------
    // シェーダのストライプ共通設定のセットアップ
    //---------------------------------------------------------------------------
    if( SetupDefaultShaderSetting( pCommandBuffer, pEmitter, shaderType, pShader, pUserParam, pDrawParameterArg ) == false )
    {
        return;
    }

    //---------------------------------------------------------------------------
    // ストライプが使用する頂点バッファを設定
    // ※アトリビュートは Vec4 を 5本分でストライドさせる
    //---------------------------------------------------------------------------
    int slot = pEmitter->GetEmitterResource()->m_ParticlePropertyBufferSlot[detail::ParticlePropertyAttributeBufferIndex_EmitterPlugin ];

    pCommandBuffer->SetVertexBuffer( slot,
        pEmitter->GetGfxObjects()->m_ParticleEmitterPluginAttribute.GetGpuAddress( pEpData->bufferSide ),
        sizeof( typename StripeSystemType::VertexAttribute ),
        pEmitter->GetGfxObjects()->m_ParticleEmitterPluginAttribute.GetBlockSize() );

    //---------------------------------------------------------------------------
    // 遅延ストライプを描画
    //---------------------------------------------------------------------------
    if( pEpData->pDelayedStripeListHead )
    {
        const typename StripeSystemType::StripeInstance* pStripe = pEpData->pDelayedStripeListHead;
        while( pStripe )
        {
            if( pStripe->renderVertexCount > 0 )
            {
                DrawParticleStripe< StripeSystemType >(
                    pCommandBuffer, pGfxDevice, pTempBuffer, pEmitter, pShader, pStripeData, pEpData, pStripe );
            }
            pStripe = pStripe->pNextDelayedStripe;
        }
    }

    //---------------------------------------------------------------------------
    // パーティクルインスタンスを巡回して、生存中のストライプを描画
    //---------------------------------------------------------------------------
    for( int i = 0; i < pEmitter->GetParticleAttrFillMax(); ++i )
    {
        const typename StripeSystemType::StripeInstance* pStripe =
            reinterpret_cast< typename StripeSystemType::StripeInstance* >( pEmitter->GetParticleData( i )->pEmitterPluginData );
        if( !pStripe || !pStripe->used )
        {
            continue;
        }

        if( pStripe->renderVertexCount > 0 )
        {
            DrawParticleStripe< StripeSystemType >(
                pCommandBuffer, pGfxDevice, pTempBuffer, pEmitter, pShader, pStripeData, pEpData, pStripe );
        }
    }
}

//---------------------------------------------------------------------------
//! @brief  パーティクル方式ストライプを一本描画します。
//---------------------------------------------------------------------------
template< class StripeSystemType >
inline void StripeSystemUtility::DrawParticleStripe(
    nn::gfx::CommandBuffer* pCommandBuffer,
    nn::gfx::Device* pGfxDevice,
    TemporaryBuffer* pTempBuffer,
    Emitter* pEmitter,
    Shader* pShader,
    const typename StripeSystemType::ResourceType* pStripeData,
    const typename StripeSystemType::EmitterPluginUserData* pEpData,
    const typename StripeSystemType::StripeInstance* pStripe ) NN_NOEXCEPT
{
    typedef typename StripeSystemType::ConstantBufferObject ConstantBufferType;
    {
        //---------------------------------------------------------------------------
        // 通常ストライプの描画
        //---------------------------------------------------------------------------
        ConstantBufferType constantBuffer;
        StripeSystemType::MakeConstantBufferObject( &constantBuffer, pStripeData, pEpData, pStripe, pEmitter, StripeMeshType_StandardStripe );
        bool result = SetConstantBufferObject< ConstantBufferType >( pCommandBuffer, pGfxDevice, pTempBuffer, pShader, &constantBuffer );
        if( result )
        {
            SetDrawCommandForParticleStripe( pCommandBuffer, pStripe, pEpData );
        }
    }
    if( pStripeData->option == StripeMeshType_CrossStripe )
    {
        //---------------------------------------------------------------------------
        // クロスの場合、定数バッファの一部を差し替えて再度描画する。
        //---------------------------------------------------------------------------
        ConstantBufferType constantBufferForCross;
        StripeSystemType::MakeConstantBufferObject( &constantBufferForCross, pStripeData, pEpData, pStripe, pEmitter, StripeMeshType_CrossStripe );
        bool result = SetConstantBufferObject< ConstantBufferType >( pCommandBuffer, pGfxDevice, pTempBuffer, pShader, &constantBufferForCross );
        if( result )
        {
            SetDrawCommandForParticleStripe( pCommandBuffer, pStripe, pEpData );
        }
    }
}

//---------------------------------------------------------------------------
//! @brief  ConstantBuffer を設定します。
//---------------------------------------------------------------------------
template< class ConstantBufferType >
inline bool StripeSystemUtility::SetConstantBufferObject(
    nn::gfx::CommandBuffer* const pCommandBuffer,
    nn::gfx::Device* const pGfxDevice,
    TemporaryBuffer* const pTempBuffer,
    Shader* const pShader,
    ConstantBufferType* const constantBuffer ) NN_NOEXCEPT
{
    NN_UNUSED( pGfxDevice );

    //---------------------------------------------------------------------------
    // MEMO: TemporaryBuffer のままだと、パフォーマンス上の問題があるので要改良
    //---------------------------------------------------------------------------
    nn::gfx::GpuAddress address;
    void* ptr = pTempBuffer->Map( &address, sizeof( ConstantBufferType ) );

    if( ptr == NULL )
    {
        // MEMO: TemporaryBuffer が取れなかった場合、描画を行わない
        Warning( nullptr, RuntimeWarningId_StripeTemporaryBufferAllocationFailed );
        return false;
    }
    else
    {
        memcpy( ptr, constantBuffer, sizeof( ConstantBufferType ) );

        const int emitterPluginConstantBufferSlotV = pShader->GetVertexConstantBufferSlot( Shader::ConstantBufferType_EmitterPlugin );
        const int emitterPluginConstantBufferSlotP = pShader->GetPixelConstantBufferSlot( Shader::ConstantBufferType_EmitterPlugin );
        if( emitterPluginConstantBufferSlotV != InvalidValueId_ConstantBufferSlotId )
        {
            pCommandBuffer->SetConstantBuffer( emitterPluginConstantBufferSlotV, nn::gfx::ShaderStage_Vertex, address, sizeof( ConstantBufferType ) );
        }
        if( emitterPluginConstantBufferSlotP != InvalidValueId_ConstantBufferSlotId )
        {
            pCommandBuffer->SetConstantBuffer( emitterPluginConstantBufferSlotP, nn::gfx::ShaderStage_Pixel, address, sizeof( ConstantBufferType ) );
        }

        pTempBuffer->Unmap();
    }

    return true;
}

//---------------------------------------------------------------------------
//! @brief  描画コマンドを発行します。
//---------------------------------------------------------------------------
template< class StripeType, class EmitterPluginUserDataType >
inline void StripeSystemUtility::SetDrawCommandForParticleStripe( nn::gfx::CommandBuffer* pCommandBuffer, const StripeType* pStripe, const EmitterPluginUserDataType* pEpData ) NN_NOEXCEPT
{
    const uint32_t vertexCount = pStripe->renderVertexCount;
    const uint32_t startOffset = pStripe->indexOffset * pEpData->vertexCountPerStripe;
    pCommandBuffer->Draw( nn::gfx::PrimitiveTopology_TriangleStrip, vertexCount, startOffset );
}

//---------------------------------------------------------------------------
//! @briefprivate   CalculateHermiteInterpolatedCurveVec() の内部処理。 入力ベクトルの差分を半分にして返す。
//! @tparam         VectorType  ストライプシステム型
//! @param[out]     pOutValue   出力ベクトル。
//! @param[in]      v0          入力0
//! @param[in]      v1          入力1
//---------------------------------------------------------------------------
template < class VectorType >
inline void GetSubAndHalfVector( VectorType* pOutValue, const VectorType& v0, const VectorType& v1 ) NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL( pOutValue );
    nn::util::VectorSubtract( pOutValue, v0, v1 );
    nn::util::VectorMultiply( pOutValue, *pOutValue, 0.5f );
}

//---------------------------------------------------------------------------
//! @briefprivate   CalculateHermiteInterpolatedCurveVec() の内部処理。
//!                 3点のうち、真ん中の点へのベクトル2本を足し合わせて、係数を乗算して返す。
//! @tparam         VectorType  ストライプシステム型
//! @param[out]     pOutValue   出力ベクトル。
//! @param[in]      v0          入力0
//! @param[in]      v1          入力1
//! @param[in]      v2          入力2
//! @param[in]      ratio       乗算する係数
//---------------------------------------------------------------------------
template < class VectorType >
inline void GetQuarterVectorFrom3Pos( VectorType* pOutValue, const VectorType& v0, const VectorType& v1, const VectorType& v2, float ratio ) NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL( pOutValue );
    VectorType temp[ 2 ];
    nn::util::VectorSubtract( &temp[ 0 ], v1, v0 );
    nn::util::VectorSubtract( &temp[ 1 ], v1, v2 );
    nn::util::VectorAdd( pOutValue, temp[ 0 ], temp[ 1 ] );
    nn::util::VectorMultiply( pOutValue, *pOutValue, ratio );
}

//---------------------------------------------------------------------------
//! @briefprivate   CalculateHermiteInterpolatedCurveVec() の内部処理。
//!                 方向ベクトルを補間する。
//! @tparam         VectorType  ストライプシステム型
//! @param[out]     pOutValue   出力ベクトル。
//! @param[in]      v0          入力0
//! @param[in]      v1          入力1
//! @param[in]      innerPos    v0 と v1 の間の相対位置
//---------------------------------------------------------------------------
template < class VectorType >
inline void GetInterpolatedDirVector( VectorType* pOutValue, const VectorType& v0, const VectorType& v1, float innerPos ) NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL( pOutValue );
    VectorType temp[ 2 ];
    nn::util::VectorMultiply( &temp[ 0 ], v0, 1.0f - innerPos );
    nn::util::VectorMultiply( &temp[ 1 ], v1, innerPos );
    nn::util::VectorAdd( pOutValue, temp[ 0 ], temp[ 1 ] );
}

//---------------------------------------------------------------------------
//! @brief  履歴式ストライプ系で、エルミート補間で履歴点の間をつなぐ曲線上の位置を求める
//---------------------------------------------------------------------------
template< class StripeSystem >
void StripeSystemUtility::CalculateHermiteInterpolatedCurveVec(
    nn::util::Vector3fType* pOutPos,
    nn::util::Vector3fType* pOutDir,
    const typename StripeSystem::History* pHistory,
    int histPos,
    int historyCount,
    float innerPos ) NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL( pOutPos );
    NN_SDK_REQUIRES_NOT_NULL( pOutDir );
    NN_SDK_REQUIRES_NOT_NULL( pHistory );

    nn::util::Vector3fType startPos, endPos;    // エルミート補間:始点・終点
    nn::util::Vector3fType startDir, endDir;    // エルミート補間:開始ベクトル、終了ベクトル（点を通過する向き）

    if( histPos == 0 )
    {
        // 先頭部分（3点で近似補間）
        const typename StripeSystem::History* pStart = pHistory;
        const typename StripeSystem::History* pNext = pStart->pNext;
        const typename StripeSystem::History* pNext2 = pNext->pNext;

        Vector3fCopy( &startPos, pStart->pos );
        Vector3fCopy( &endPos, pNext->pos );

        typename StripeSystem::History::PosVectorType end;
        typename StripeSystem::History::PosVectorType start;
        GetSubAndHalfVector( &end, pNext2->pos, pStart->pos );
        Vector3fCopy( &endDir, end );
        GetQuarterVectorFrom3Pos( &start, pStart->pos, pNext->pos, pNext2->pos, 0.25f );
        Vector3fCopy( &startDir, start );

        // 進行方向
        GetInterpolatedDirVector( pOutDir, pStart->dir, pNext->dir, innerPos );
    }
    else if( histPos == historyCount - 1 )
    {
        // ストライプの末端（3点で近似補間）
        const typename StripeSystem::History* pEnd = pHistory;
        const typename StripeSystem::History* pPrev = pEnd->pPrev;
        const typename StripeSystem::History* pPrev2 = pPrev->pPrev;
        innerPos = 1.0f;

        // 末端部分
        Vector3fCopy( &startPos, pPrev->pos );
        Vector3fCopy( &endPos, pEnd->pos );

        typename StripeSystem::History::PosVectorType start;
        typename StripeSystem::History::PosVectorType end;
        GetSubAndHalfVector( &start, pEnd->pos, pPrev2->pos );
        Vector3fCopy( &startDir, start );
        GetQuarterVectorFrom3Pos( &end, pEnd->pos, pPrev->pos, pPrev2->pos, -0.25f );
        Vector3fCopy( &endDir, end );

        // 進行方向
        GetInterpolatedDirVector( pOutDir, pPrev->dir, pEnd->dir, innerPos );
    }
    else if( histPos == historyCount - 2 )
    {
        // ストライプの末端のひとつ前（3点で近似補間）
        const typename StripeSystem::History* pEnd = pHistory->pNext;
        const typename StripeSystem::History* pPrev = pEnd->pPrev;
        const typename StripeSystem::History* pPrev2 = pPrev->pPrev;

        // 末端部分
        Vector3fCopy( &startPos, pPrev->pos );
        Vector3fCopy( &endPos, pEnd->pos );

        typename StripeSystem::History::PosVectorType start;
        typename StripeSystem::History::PosVectorType end;

        GetSubAndHalfVector( &start, pEnd->pos, pPrev2->pos );
        Vector3fCopy( &startDir, start );
        GetQuarterVectorFrom3Pos( &end, pEnd->pos, pPrev->pos, pPrev2->pos, -0.25f );
        Vector3fCopy( &endDir, end );

        // 進行方向
        GetInterpolatedDirVector( pOutDir, pPrev->dir, pEnd->dir, innerPos );
    }
    else
    {
        // 中間部（4点で補間）
        const typename StripeSystem::History* pNow = pHistory;
        const typename StripeSystem::History* pNext = pNow->pNext;
        const typename StripeSystem::History* pNext2 = pNext->pNext;
        const typename StripeSystem::History* pPrev = pNow->pPrev;

        Vector3fCopy( &startPos, pNow->pos );
        Vector3fCopy( &endPos, pNext->pos );

        typename StripeSystem::History::PosVectorType start;
        typename StripeSystem::History::PosVectorType end;

        GetSubAndHalfVector( &start, pNext->pos, pPrev->pos );
        Vector3fCopy( &startDir, start );
        GetSubAndHalfVector( &end, pNext2->pos, pNow->pos );
        Vector3fCopy( &endDir, end );

        // 進行方向
        GetInterpolatedDirVector( pOutDir, pNow->dir, pNext->dir, innerPos );
    }
    detail::HermiteInterpolationOnCubic( pOutPos, startPos, startDir, endPos, endDir, innerPos );

    return;
} //NOLINT(impl/function_size)

} // namespace detail
} // namespace vfx
} // namespace nn
