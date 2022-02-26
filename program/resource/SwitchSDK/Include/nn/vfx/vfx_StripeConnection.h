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

#include <nn/vfx/vfx_StripeCommon.h>

namespace nn {
namespace vfx {
namespace detail {

//---------------------------------------------------------------------------
//! @brief  連結式ストライプを管理するクラスです。
//---------------------------------------------------------------------------
class ConnectionStripeSystem
{
    NN_DISALLOW_COPY( ConnectionStripeSystem );
public:
    typedef      ResStripeConnection ResourceType;                  //!< 使用するバイナリリソース構造体
    static const EmitterPluginId     PluginId = EmitterPluginId_1;  //!< エミッタプラグインID

public:
    //---------------------------------------------------------------------------
    //! @brief 連結式ストライプの端処理タイプ
    //---------------------------------------------------------------------------
    enum TailConnectionType
    {
        TailConnectionType_NoConnection     = 0,        //!< 「通常」
        TailConnectionType_Loop             = 1,        //!< 「先端に繋ぐ」
        TailConnectionType_ConnectToEmitter = 2,        //!< 「エミッタに繋ぐ」
    };

    //------------------------------------------------------------------------------
    //! @brief  エミッタ単位で記憶する必要があるデータ
    //------------------------------------------------------------------------------
    struct EmitterPluginUserData
    {
        nn::util::Float4    staticParam0;               //!< 静的パラメータ（ConstantBufferObject::param0）
        nn::util::Float4    random;                     //!< ランダム値（エミッタ単位）
        bool                vertexBufferInitialized;    //!< 頂点バッファが初期化済みかどうか
        int32_t             renderVertexCount;          //!< 描画する個数
        BufferSide          bufferSide;                 //!< 現在のバッファサイド（Emitterとは独立で持つ）
    };

    //---------------------------------------------------------------------------
    //! @brief 連結式ストライプ頂点バッファ
    //---------------------------------------------------------------------------
    struct VertexAttribute
    {
        nn::util::Float4    pos;                        //!< 背骨の位置 xyz / 羽根の長さ
        nn::util::Float4    dir;                        //!< 背骨の向き xyz / Texture V
        nn::util::Float4    emat;                       //!< 背骨のエミッタマトリクス / 頂点Index
        nn::util::Float4    dummy[2];                   //!< Float4 * 5 本にするためのパディング
    };

    //------------------------------------------------------------------------------
    //! @brief   連結式ストライプ定数バッファ構造体
    //! @details 履歴式ストライプとシェーダを共用しているので、同一の構造体。
    //------------------------------------------------------------------------------
    struct ConstantBufferObject
    {
        nn::util::Float4    random;                     //!< （静的パラメータ）random
        nn::util::Float4    param0;                     //!< （静的パラメータ）先端α / 末端α/ ...  / ...
        nn::util::Float4    color0;                     //!< （動的パラメータ）Color0
        nn::util::Float4    color1;                     //!< （動的パラメータ）Color1
        nn::util::Float4    param1;                     //!< （動的パラメータ）ストライプtime / 総頂点数 / CrossMode / ストライプ寿命
        nn::util::Float4    param2;                     //!< （動的パラメータ）ストライプ先端座標(xyz) / ...
    };

public:
    //------------------------------------------------------------------------------
    //! @brief                  連結式ストライプシステムの初期化処理を行います。
    //! @param[in] pHeap        ヒープへのポインタ
    //! @param[in] pSystem      Systemへのポインタ
    //! @param[in] bufferMode   バッファリングモード
    //------------------------------------------------------------------------------
    static void InitializeSystem( nn::vfx::Heap* pHeap, nn::vfx::System* pSystem, BufferingMode bufferMode ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief                  連結式ストライプシステムの終了処理を行います。
    //! @param[in] pHeap        ヒープへのポインタ
    //------------------------------------------------------------------------------
    static void FinalizeSystem( nn::vfx::Heap* pHeap ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief                  エミッタ生成後コールバック
    //! @param[in] arg          コールバック引数
    //! @return                 成功した場合はtrue
    //------------------------------------------------------------------------------
    static bool InitializeStripeEmitter( EmitterInitializeArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief                  エミッタ計算処理後コールバック
    //! @param[in] arg          コールバック引数
    //------------------------------------------------------------------------------
    static void EmitterPostCalculateCallback( EmitterPostCalculateArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief                  エミッタ描画コールバック
    //! @param[in] arg          コールバック引数
    //! @return                 成功した場合はtrue
    //------------------------------------------------------------------------------
    static bool EmitterDrawCallback( EmitterDrawArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief                  エミッタ破棄後コールバック
    //! @param[in] arg          コールバック引数
    //------------------------------------------------------------------------------
    static void FinalizeStripeEmitter( EmitterFinalizeArg& arg ) NN_NOEXCEPT;

private:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief                  連結式ストライプシステムの生成を行います。
    //! @param[in] pSystem      Systemへのポインタ
    //! @param[in] bufferMode   バッファリングモード
    //---------------------------------------------------------------------------
    ConnectionStripeSystem( nn::vfx::System* pSystem, BufferingMode bufferMode ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  デフォルトコンストラクタ（Private宣言のみ）
    //---------------------------------------------------------------------------
    ConnectionStripeSystem() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  連結式ストライプシステムの破棄を行います。
    //---------------------------------------------------------------------------
    virtual ~ConnectionStripeSystem() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  ポリゴン用ワークを確保します。
    //! @param[in] pEmitter     エミッタへのポインタ
    //! @return                 true で確保成功、 false で確保失敗
    //---------------------------------------------------------------------------
    bool AllocStripeSystemVertexBuffer( Emitter* pEmitter ) NN_NOEXCEPT;

    //@}

    //---------------------------------------------------------------------------
    //! @brief                  ストライプのポリゴン生成を行います。
    //! @param[in] pEmitter     エミッタへのポインタ
    //---------------------------------------------------------------------------
    void UpdateStripePolygon( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                                  頂点バッファに頂点情報をセットします。
    //! @param[out]     pOutVertexBuffer        頂点バッファ
    //! @param[in,out]  pOutAttrIndex           書き込み位置
    //! @param[out]     pOutRenderVertexCount   描画する頂点数
    //! @param[in]      vertexCount             全ての頂点数
    //! @param[in]      vertexWorldPos          ワールド座標
    //! @param[in]      vertexWorldDir          ワールド方向
    //! @param[in]      emitterMatY             エミッタ行列Y軸
    //! @param[in]      scaleValue              スケール値
    //---------------------------------------------------------------------------
    void AddVertexDataToBuffer(
        VertexAttribute* pOutVertexBuffer,
        int* pOutAttrIndex,
        int* pOutRenderVertexCount,
        int vertexCount,
        const nn::util::Vector3fType& vertexWorldPos,
        const nn::util::Vector3fType& vertexWorldDir,
        const nn::util::Vector3fType& emitterMatY,
        float scaleValue ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                      ConstantBuffer の中身を書き込みます。
    //! @param[in] pConstantBuffer  ConstantBuffer へのポインタ
    //! @param[in] pEPUserData      エミッタ単位のストライプ設定
    //! @param[in] pEmitter         エミッタ
    //! @param[in] meshType         ストライプ形状（通常／クロス）
    //---------------------------------------------------------------------------
    void MakeConstantBufferObject(
        ConstantBufferObject* const pConstantBuffer,
        const EmitterPluginUserData* pEPUserData,
        const Emitter* pEmitter,
        StripeMeshType meshType ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                          エミッタ描画処理を行います。
    //! @param[in] pCommandBuffer       コマンドバッファ
    //! @param[in] pEmitter             エミッタへのポインタ
    //! @param[in] shaderType           シェーダタイプ
    //! @param[in] pUserParam           ユーザーパラメータへのポインタ
    //! @param[in] pDrawParameterArg    描画パラメータへのポインタ
    //---------------------------------------------------------------------------
    void Draw( nn::gfx::CommandBuffer* pCommandBuffer, Emitter* pEmitter, ShaderType shaderType, void* pUserParam, DrawParameterArg* pDrawParameterArg ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  エミッタプラグインユーザーデータを取得します。
    //! @param[in] pEmitter     エミッタへのポインタ
    //! @return                 エミッタプラグインユーザーデータへのポインタ
    //---------------------------------------------------------------------------
    static EmitterPluginUserData* GetEmitterUserData( Emitter* const pEmitter ) NN_NOEXCEPT
    {
        return reinterpret_cast< EmitterPluginUserData* >( pEmitter->GetEmitterPluginUserData() );
    }

    //---------------------------------------------------------------------------
    //! @brief                  ストライプデータを取得します
    //! @param[in] pEmitter     エミッタへのポインタ
    //! @return                 ストライプデータへのポインタ
    //---------------------------------------------------------------------------
    static ResStripeConnection* GetStripeData( Emitter* const pEmitter ) NN_NOEXCEPT
    {
        return reinterpret_cast< ResStripeConnection* >( pEmitter->GetEmitterResource()->m_pEmitterPluginData );
    }

    //------------------------------------------------------------------------------
    //! @brief  ストライプシステムで確保したワークサイズを取得します。
    //! @return ワークサイズ
    //------------------------------------------------------------------------------
    static size_t GetWorkSize() NN_NOEXCEPT;

private:
    static ConnectionStripeSystem*  g_pStripeSystem;        //!< ストライプシステムへのポインタ
private:
    nn::util::Float4                m_DefaultColor;         //!< 連結式ストライプのシェーダに渡すカラー（白）
    nn::vfx::System* const          m_pSystem;              //!< エフェクトシステム
    const BufferingMode             m_BufferMode;           //!< バッファモード
    const size_t                    m_StripeWorkSize;       //!< ストライプワークサイズ
};

//------------------------------------------------------------------------------
//! @brief  ストライプシステムで確保したワークサイズを取得します。
//! @return ワークサイズ
//------------------------------------------------------------------------------
inline size_t ConnectionStripeSystem::GetWorkSize() NN_NOEXCEPT
{
    if( g_pStripeSystem == NULL )
    {
        return 0;
    }
    return g_pStripeSystem->m_StripeWorkSize;
}

//------------------------------------------------------------------------------
//! @brief  エミッタ計算処理後コールバック
//------------------------------------------------------------------------------
inline void ConnectionStripeSystem::EmitterPostCalculateCallback( EmitterPostCalculateArg& arg ) NN_NOEXCEPT
{
    if( arg.pEmitter->GetCalculationType() == EmitterCalculationMode_Cpu )
    {
        //------------------------------------------------------------------------------
        // 連結式ストライプのポリゴンを生成
        //------------------------------------------------------------------------------
        EmitterPluginUserData* epData = ConnectionStripeSystem::GetEmitterUserData( arg.pEmitter );
        StripeSystemUtility::SwapBufferSide( &epData->bufferSide, arg.pEmitter );
        g_pStripeSystem->UpdateStripePolygon( arg.pEmitter );
    }
}

//------------------------------------------------------------------------------
//! @brief  エミッタ描画コールバック
//------------------------------------------------------------------------------
inline bool ConnectionStripeSystem::EmitterDrawCallback( EmitterDrawArg& arg ) NN_NOEXCEPT
{
    // 連結式ストライプを描画！
    if( arg.pEmitter->GetCalculationType() == EmitterCalculationMode_Cpu )
    {
        g_pStripeSystem->Draw( arg.pCommandBuffer, arg.pEmitter, arg.shaderType, arg.pUserParam, arg.pDrawParameterArg );
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
//! @brief  ConstantBuffer を作成します。
//---------------------------------------------------------------------------
inline void ConnectionStripeSystem::MakeConstantBufferObject(
    ConstantBufferObject* const pConstantBuffer,
    const EmitterPluginUserData* pEPUserData,
    const Emitter* pEmitter,
    StripeMeshType meshType ) NN_NOEXCEPT
{
    // 静的な要素
    pConstantBuffer->random = pEPUserData->random;
    pConstantBuffer->param0 = pEPUserData->staticParam0;

    // 動的な要素
    // シェーダに渡すカラーはRGBA(1,1,1,1)を渡しておく。
    // シェーダ側でエミッタカラーと乗算される
    pConstantBuffer->color0 = g_pStripeSystem->m_DefaultColor;
    pConstantBuffer->color1 = g_pStripeSystem->m_DefaultColor;
    pConstantBuffer->param1.x = pEmitter->GetTime();
    pConstantBuffer->param1.y = static_cast< float >( pEPUserData->renderVertexCount * 0.5 );
    pConstantBuffer->param1.z = static_cast< float >( meshType );
    pConstantBuffer->param1.w = static_cast< float >( pEmitter->GetResEmitter()->ptcl.life );
}

} // namespace detail
} // namespace vfx
} // namespace nn
