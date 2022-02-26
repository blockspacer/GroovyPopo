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
#include <nn/vfx/vfx_TemporaryBuffer.h>

namespace nn {
namespace vfx {
namespace detail {

//---------------------------------------------------------------------------
//! @brief  ストライプを管理するクラスです。
//---------------------------------------------------------------------------
class StripeSystem
{
    NN_DISALLOW_COPY( StripeSystem );
public:
    typedef      ResStripeHistory ResourceType;                   //!< 使用するバイナリリソース構造体
    static const EmitterPluginId  PluginId = EmitterPluginId_2;   //!< エミッタプラグインID

public:
    //------------------------------------------------------------------------------
    //! @brief  ストライプ履歴（リングバッファとして用いる）
    //------------------------------------------------------------------------------
    struct History
    {
        typedef nn::util::Vector4fType PosVectorType;   //!< 位置ベクトルの型
        typedef nn::util::Vector3fType DirVectorType;   //!< 方向ベクトルの型
        nn::util::Vector4fType  pos;                    //!< エミッタ（or ワールド）座標系での座標xyz/ スケール(w)
        nn::util::Vector3fType  dir;                    //!< 進行方向
        nn::util::Vector3fType  emat;                   //!< エミッタSRT行列のローカルY軸成分
        History*                pNext;                  //!< 次の履歴点へのポインタ
        History*                pPrev;                  //!< 前の履歴点へのポインタ
    };
private:
    //------------------------------------------------------------------------------
    //! @brief  ストライプの実体
    //------------------------------------------------------------------------------
    struct StripeInstance
    {
        nn::util::Vector4fType  color0;                 //!< カラー0（ストライプ単位で共通）
        nn::util::Vector4fType  color1;                 //!< カラー1（ストライプ単位で共通）
        nn::util::Vector4fType  random;                 //!< ランダム値（ストライプ単位で共通）
        nn::util::Vector3fType  vInterpolate;           //!< 補間ベクトル
        nn::util::Vector3fType  headPos;                //!< 先端の位置
        History*                historyArray;           //!< ヒストリ配列の先頭
        History*                pHistoryHead;           //!< 描画対象のヒストリ先頭
        History*                pHistoryTail;           //!< 描画対象のヒストリ最後尾
        StripeInstance*         pNextDelayedStripe;     //!< 遅延終了処理用のリスト
        int                     historyCount;           //!< ヒストリが何個分あるか？
        int                     indexOffset;            //!< 頂点バッファ上の位置
        int                     renderVertexCount;      //!< 頂点バッファに書き込んだ数
        float                   time;                   //!< 生成時間
        float                   life;                   //!< 寿命
        bool                    used;                   //!< 利用中かどうか
        float                   prevFrame;              //!< 前回のフレーム値
    };

    //---------------------------------------------------------------------------
    //! @brief  頂点バッファ構造体
    //---------------------------------------------------------------------------
    struct VertexAttribute
    {
        nn::util::Float4    pos;                        //!< 背骨の位置 xyz / 羽根の長さ
        nn::util::Float4    dir;                        //!< 背骨の向き xyz / Texture V
        nn::util::Float4    emat;                       //!< 背骨のエミッタマトリクス / 頂点Index
        nn::util::Float4    dummy[2];                   //!< Float4 * 5 本にするためのパディング
    };

    //---------------------------------------------------
    //! @brief   ConstantBuffer構造体
    //! @details 履歴式ストライプとシェーダを共用しているので、同一の構造体。
    //---------------------------------------------------
    struct ConstantBufferObject
    {
        nn::util::Float4    random;                     //!< （静的パラメータ）random
        nn::util::Float4    param0;                     //!< （静的パラメータ）先端α / 末端α/ ...  / ...
        nn::util::Float4    color0;                     //!< （動的パラメータ）Color0
        nn::util::Float4    color1;                     //!< （動的パラメータ）Color1
        nn::util::Float4    param1;                     //!< （動的パラメータ）ストライプtime / 総頂点数 / CrossMode / ストライプ寿命
        nn::util::Float4    param2;                     //!< （動的パラメータ）ストライプ先端座標(xyz) / ...
    };

    //------------------------------------------------------------------------------
    //! @brief  エミッタ単位で記憶する必要があるデータ
    //------------------------------------------------------------------------------
    struct EmitterPluginUserData
    {
        nn::util::Float4    staticParam0;               //!< 静的パラメータ（ConstantBufferObject::param0）
        bool                vertexBufferInitialized;    //!< 頂点バッファが初期化済みか
        StripeInstance*     pDelayedStripeListHead;     //!< 遅延ストライプリストの先頭
        int                 vertexCountPerStripe;       //!< ストライプ一本当たりの頂点数
        int                 vertexBufferCount;          //!< 頂点バッファの個数
        int                 maxStripeCount;             //!< 頂点アトリビュートの本数
        int                 attributeIndex;             //!< 最後に割り当てたアトリビュートインデックス
        int                 maxEmitRatio;               //!< エミッタ時間アニメーションも考慮した最大放出レート
        int                 maxParticleLife;            //!< エミッタ時間アニメーションも考慮した最大パーティクル寿命
        BufferSide          bufferSide;                 //!< 現在のバッファサイド（Emitterとは独立で持つ）
    };

public:
    //------------------------------------------------------------------------------
    //! @brief                  ストライプシステムの初期化処理を行います。
    //! @param[in] pHeap        ヒープへのポインタ
    //! @param[in] pSystem      Systemへのポインタ
    //! @param[in] bufferMode   バッファリングモード
    //! @param[in] stripeCount  ストライプ最大数
    //------------------------------------------------------------------------------
    static void InitializeSystem( nn::vfx::Heap* pHeap, nn::vfx::System* pSystem, BufferingMode bufferMode, int stripeCount ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief              ストライプシステムの終了処理を行います。
    //! @param[in] pHeap    ヒープへのポインタ
    //------------------------------------------------------------------------------
    static void FinalizeSystem( nn::vfx::Heap* pHeap ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief  ストライプシステムで確保したワークサイズを取得します。
    //! @return ワークサイズ
    //------------------------------------------------------------------------------
    static size_t GetWorkSize() NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief      ワンタイムエミッタ用の追加のエミッタ寿命を取得
    //! @param[in]  pEmitter    エミッタへのポインタ
    //! @return                 ワンタイムエミッタ用の追加のエミッタ寿命
    //------------------------------------------------------------------------------
    static int GetExtendedEndTimeForOneTimeEmitter( nn::vfx::Emitter* pEmitter ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief      ストライプの正確な再生本数を取得
    //! @param[in]  pEmitter    エミッタへのポインタ
    //! @return                 再生終了待ちのストライプを考慮した正確な再生本数
    //------------------------------------------------------------------------------
    static int GetActualStripeCalclationCount( const nn::vfx::Emitter* pEmitter ) NN_NOEXCEPT;


    //----------------------------------------
    //! @name エミッタコールバック
    //@{

    //------------------------------------------------------------------------------
    //! @brief          エミッタ生成後コールバック
    //! @param[in] arg  コールバック引数
    //! @return         成功した場合true
    //------------------------------------------------------------------------------
    static bool InitializeStripeEmitter( EmitterInitializeArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          パーティクル生成コールバック
    //! @param[in] arg  コールバック引数
    //! @return         成功した場合true
    //------------------------------------------------------------------------------
    static bool EmitStripe( ParticleEmitArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          パーティクル削除コールバック
    //! @param[in] arg  コールバック引数
    //! @return         成功した場合true
    //------------------------------------------------------------------------------
    static bool KillStripe( ParticleRemoveArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          パーティクル計算コールバック
    //! @param[in] arg  コールバック引数
    //------------------------------------------------------------------------------
    static void ParticleCalculateCallback( ParticleCalculateArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          エミッタ計算処理前コールバック
    //! @param[in] arg  コールバック引数
    //------------------------------------------------------------------------------
    static void EmitterPreCalculateCallback( EmitterPreCalculateArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          エミッタ計算処理後コールバック
    //! @param[in] arg  コールバック引数
    //------------------------------------------------------------------------------
    static void EmitterPostCalculateCallback( EmitterPostCalculateArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          エミッタ描画コールバック
    //! @param[in] arg  コールバック引数
    //! @return         成功した場合true
    //------------------------------------------------------------------------------
    static bool EmitterDrawCallback( EmitterDrawArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          エミッタ破棄後コールバック
    //! @param[in] arg  コールバック引数
    //------------------------------------------------------------------------------
    static void FinalizeStripeEmitter( EmitterFinalizeArg& arg ) NN_NOEXCEPT;

    //@}

private:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief                  ストライプシステムの生成を行います。
    //! @param[in] pHeap        ヒープへのポインタ
    //! @param[in] pSystem      Systemへのポインタ
    //! @param[in] bufferMode   バッファリングモード
    //! @param[in] stripeCount  ストライプ最大数
    //---------------------------------------------------------------------------
    StripeSystem( nn::vfx::Heap* pHeap, nn::vfx::System* pSystem, BufferingMode bufferMode, int stripeCount ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  デフォルトコンストラクタ（Private宣言のみ）
    //---------------------------------------------------------------------------
    StripeSystem() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        ストライプシステムの破棄を行います。
    //---------------------------------------------------------------------------
    virtual ~StripeSystem() NN_NOEXCEPT;

    //@}

    //---------------------------------------------------------------------------
    //! @brief              ポリゴン用ワークを確保します。
    //! @param[in] pEmitter エミッタへのポインタ
    //! @return    true で確保成功、 false で確保失敗
    //---------------------------------------------------------------------------
    bool AllocStripeSystemVertexBuffer( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ストライプ計算処理を行います。
    //! @param[in] arg      コールバック引数
    //! @param[in] pStripe  ストライプへのポインタ
    //---------------------------------------------------------------------------
    void CalculateStripe( ParticleCalculateArg& arg, StripeInstance* pStripe ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              遅延解放ストライプ計算処理を行います。
    //! @param[in] pEmitter エミッタへのポインタ
    //---------------------------------------------------------------------------
    void CalculateDelayedStripe( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                      ストライプのポリゴン生成を行います。
    //! @param[in] pStripe          ストライプへのポインタ
    //! @param[in] pStripeData      ストライプ設定へのポインタ
    //! @param[in] pVertexBuffer    頂点バッファへのポインタ
    //! @param[in] textureBaseValue テクスチャ座標計算のための値
    //! @param[in] scaleFadeValue   スケールフェードのための値
    //---------------------------------------------------------------------------
    void MakeStripePolygon( StripeInstance* pStripe, ResStripeHistory* pStripeData, VertexAttribute* pVertexBuffer, float textureBaseValue, float scaleFadeValue ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                      ストライプのポリゴン生成を行います。（ストライプ分割版）
    //! @param[in] pStripe          ストライプへのポインタ
    //! @param[in] pStripeData      ストライプ設定へのポインタ
    //! @param[in] pVertexBuffer    頂点バッファへのポインタ
    //! @param[in] textureBaseValue テクスチャ座標計算のための値
    //! @param[in] scaleFadeValue   スケールフェードのための値
    //---------------------------------------------------------------------------
    void MakeStripePolygonWithDivision( StripeInstance* pStripe, ResStripeHistory* pStripeData, VertexAttribute* pVertexBuffer, float textureBaseValue, float scaleFadeValue ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        現在のストライプ数取得を行います。
    //! @return 現在のストライプ数
    //---------------------------------------------------------------------------
    static int GetProcessingStripeCount() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  エミッタプラグインユーザーデータを取得します。
    //! @param[in] pEmitter  エミッタへのポインタ
    //! @return エミッタプラグインユーザーデータへのポインタ
    //---------------------------------------------------------------------------
    static EmitterPluginUserData* GetEmitterUserData( const Emitter* pEmitter ) NN_NOEXCEPT
    {
        return reinterpret_cast< EmitterPluginUserData* >( pEmitter->GetEmitterPluginUserData() );
    }

    //---------------------------------------------------------------------------
    //! @brief      ストライプデータを取得します
    //! @param[in] pEmitter  エミッタへのポインタ
    //! @return     ストライプデータへのポインタ
    //---------------------------------------------------------------------------
    static ResStripeHistory* GetStripeData( Emitter* pEmitter ) NN_NOEXCEPT
    {
        return reinterpret_cast< ResStripeHistory* >( pEmitter->GetEmitterResource()->m_pEmitterPluginData );
    }

    //---------------------------------------------------------------------------
    //! @brief                      ConstantBuffer の中身を書き込みます。
    //! @param[in] pConstantBuffer  ConstantBuffer へのポインタ
    //! @param[in] pStripeData      ストライプ設定
    //! @param[in] pEpData          エミッタ単位のストライプ設定
    //! @param[in] pStripe          ストライプ
    //! @param[in] pEmitter         エミッタへのポインタ
    //! @param[in] meshType         ストライプ形状（通常／クロス）
    //---------------------------------------------------------------------------
    static void MakeConstantBufferObject(
        ConstantBufferObject* pConstantBuffer,
        const ResStripeHistory* pStripeData,
        const EmitterPluginUserData* pEpData,
        const StripeInstance* pStripe,
        const Emitter* pEmitter,
        StripeMeshType meshType ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                      ストライプのカラーを更新します。
    //! @param[in] arg              コールバック引数
    //! @param[in] pStripe          ストライプインスタンスへのポインタ
    //---------------------------------------------------------------------------
    static void UpdateStripeColor( ParticleCalculateArgImpl& arg, detail::StripeSystem::StripeInstance* pStripe ) NN_NOEXCEPT;
private:
    static StripeSystem*    g_pStripeSystem;        //!< ストライプシステムへのポインタ
private:
    nn::vfx::System* const  m_pSystem;              //!< Systemへのポインタ
    nn::vfx::Heap* const    m_pHeap;                //!< ヒープへのポインタ
    const BufferingMode     m_BufferMode;           //!< バッファリングモード
    const int               m_StripeArrayCount;     //!< システム全体で共有するストライプ配列のサイズ
    const size_t            m_StripeWorkSize;       //!< ストライプワークサイズ
    int                     m_StripeArrayIndex;     //!< 最後に割り当てたストライプの配列位置
    int                     m_StripeCount;          //!< 現在使用中のストライプ数（遅延ストライプを含む）
    StripeInstance*         m_StripeArray;          //!< システム全体で共有するストライプ配列

    //------------------------------------------------------------------------------
    friend StripeSystemUtility;
};

//------------------------------------------------------------------------------
//! @brief パーティクル計算コールバック
//------------------------------------------------------------------------------
inline void StripeSystem::ParticleCalculateCallback( ParticleCalculateArg& arg ) NN_NOEXCEPT
{
    StripeInstance* stripe = reinterpret_cast< StripeInstance* >( arg.pEmitterPluginData );
    EmitterPluginUserData* epData = StripeSystem::GetEmitterUserData( arg.pEmitter );
    if( stripe == NULL )
    {
        OutputWarning( "Stripe instance is null\n" );
        return;
    }
    else if( stripe->used == false )
    {
        OutputWarning( "Stripe instance is not used\n" );
        return;
    }
    if( epData == NULL )
    {
        OutputWarning( "EmitterPluginUserData is empty\n" );
        return;
    }
    else if( epData->vertexBufferInitialized == false )
    {
        OutputWarning( "Vertex buffer for stripe has not allocated\n" );
        return;
    }
    g_pStripeSystem->CalculateStripe( arg, stripe );
}

//------------------------------------------------------------------------------
//! @brief エミッタ計算処理前コールバック
//------------------------------------------------------------------------------
inline void StripeSystem::EmitterPreCalculateCallback( EmitterPreCalculateArg& arg ) NN_NOEXCEPT
{
    if ( arg.isBufferSwap == false ) return;

    if( arg.pEmitter->GetCalculationType() == EmitterCalculationMode_Cpu )
    {
        EmitterPluginUserData* epData = StripeSystem::GetEmitterUserData( arg.pEmitter );
        StripeSystemUtility::SwapBufferSide( &epData->bufferSide, arg.pEmitter );
    }
}

//------------------------------------------------------------------------------
//! @brief エミッタ計算処理後コールバック
//------------------------------------------------------------------------------
inline void StripeSystem::EmitterPostCalculateCallback( EmitterPostCalculateArg& arg ) NN_NOEXCEPT
{
    if( arg.pEmitter->GetCalculationType() == EmitterCalculationMode_Cpu )
    {
        g_pStripeSystem->CalculateDelayedStripe( arg.pEmitter );
    }
}

//------------------------------------------------------------------------------
//! @brief エミッタ描画コールバック
//------------------------------------------------------------------------------
inline bool StripeSystem::EmitterDrawCallback( EmitterDrawArg& arg ) NN_NOEXCEPT
{
    if( arg.pEmitter->GetCalculationType() == EmitterCalculationMode_Cpu )
    {
        StripeSystemUtility::DrawParticleStripeEmitter< StripeSystem >(
            arg.pCommandBuffer, g_pStripeSystem->m_pSystem, arg.pEmitter, arg.shaderType, arg.pUserParam, arg.pDrawParameterArg );
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
//! @brief  ストライプシステムで確保したワークサイズを取得します。
//------------------------------------------------------------------------------
inline size_t StripeSystem::GetWorkSize() NN_NOEXCEPT
{
    if( g_pStripeSystem == NULL )
    {
        return 0;
    }
    return g_pStripeSystem->m_StripeWorkSize;
}

//---------------------------------------------------------------------------
//! @brief  現在のストライプ数取得を行います。
//---------------------------------------------------------------------------
inline int StripeSystem::GetProcessingStripeCount() NN_NOEXCEPT
{
    if( g_pStripeSystem == NULL )
    {
        return 0;
    }
    return g_pStripeSystem->m_StripeCount;
}

} // namespace detail
} // namespace vfx
} // namespace nn
