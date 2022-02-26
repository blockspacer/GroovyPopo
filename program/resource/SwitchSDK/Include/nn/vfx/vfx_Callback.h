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
#include <nn/vfx/vfx_CallbackPtclCalcArg.h>
#include <nn/vfx/vfx_Enum.h>

//------------------------------------------------------------------------------
namespace nn {
namespace vfx {

class System;
class Emitter;
class EmitterSet;

namespace detail {

class Shader;

//---------------------------------------------------
//! @brief パーティクル情報構造体です。
//---------------------------------------------------
struct Particle
{
    float                   time;           //!< パーティクル時間
    nn::util::Vector3fType  localDiff;      //!< ローカル系の移動差分
    nn::util::Vector3fType  localPos;       //!< ローカル位置
    nn::util::Vector3fType  worldPos;       //!< ワールド位置
    nn::util::Vector3fType  localVec;       //!< ローカル速度
    nn::util::Vector3fType  worldVec;       //!< ワールド速度
    nn::util::Vector3fType  localRotate;    //!< 初期回転量
    nn::util::Vector3fType  localScale;     //!< 初期スケール値
    nn::util::Vector3fType  worldScale;     //!< ワールドスケール値
    nn::util::Vector4fType  localColor0;    //!< カラー0（R/G/B/A）
    nn::util::Vector4fType  localColor1;    //!< カラー1（R/G/B/A）
    nn::util::Vector4fType  random;         //!< ランダム値（X/Y/Z/Wの4つ）
};

}

//----------------------------------------
//! @name エミッタ コールバック
//@{

//---------------------------------------------------
//! @brief  エミッタ生成後コールバックに引数として渡される構造体です。
//!         ここから生成したエミッタにアクセスできます。
//---------------------------------------------------
struct EmitterInitializeArg
{
    Emitter*            pEmitter;        //!< エミッタ
};

//---------------------------------------------------
//! @brief  エミッタ生成後コールバックの定義です。
//!         エミッタ生成後にコールされます。
//! @param[in] arg コールバック引数
//---------------------------------------------------
typedef bool( *EmitterInitializeCallback )( EmitterInitializeArg& arg );


//---------------------------------------------------
//! @brief  エミッタ計算前コールバックに引数として渡される構造体です。
//!         ここから操作対象のエミッタにアクセスできます。
//---------------------------------------------------
struct EmitterPreCalculateArg
{
    Emitter*        pEmitter;           //!< エミッタインスタンス
    bool            isBufferSwap;       //!< エミッタがバッファスワップしたかどうか
};

//---------------------------------------------------
//! @brief  エミッタ計算前コールバックの定義です。
//!         エミッタ計算処理前にコールされます。
//! @param[in] arg コールバック引数
//---------------------------------------------------
typedef void( *EmitterPreCalculateCallback )( EmitterPreCalculateArg& arg );


//---------------------------------------------------
//! @brief  エミッタマトリクスセットコールバックに引数として渡される構造体です。
//!         ここから操作対象のエミッタにアクセスできます。
//---------------------------------------------------
struct EmitterMatrixSetArg
{
    Emitter* pEmitter;           //!< エミッタインスタンス

    //---------------------------------------------------
    //! @brief 引数ビューマトリクスからエミッタビルボード用マトリクスを生成し設定します。
    //! @param[in] viewMatrix ビュー行列
    //---------------------------------------------------
    void SetEmitterBillboardMatrix( const nn::util::Matrix4x3fType& viewMatrix ) NN_NOEXCEPT;

    //---------------------------------------------------
    //! @brief 引数ビューマトリクスからエミッタY軸ビルボード用マトリクスを生成し設定します。
    //! @param[in] viewMatrix ビュー行列
    //---------------------------------------------------
    void SetEmitterYAxisBillboardMatrix( const nn::util::Matrix4x3fType& viewMatrix ) NN_NOEXCEPT;
};

//---------------------------------------------------
//! @brief  エミッタマトリクスセットコールバックの定義です。
//!         エミッタマトリクスセット後にコールされます。
//!         エミッタビルボードするためのマトリクス操作などを行えます。
//! @param[in] arg コールバック引数
//---------------------------------------------------
typedef void( *EmitterMatrixSetCallback )( EmitterMatrixSetArg& arg );


//---------------------------------------------------
//! @brief  エミッタ計算後コールバックに引数として渡される構造体です。
//!         ここから操作対象のエミッタにアクセスできます。
//---------------------------------------------------
struct EmitterPostCalculateArg
{
    Emitter* pEmitter;           //!< エミッタインスタンス
};

//---------------------------------------------------
//! @brief  エミッタ計算後コールバックの定義です。
//!         エミッタ計算処理後にコールされます。
//! @param[in] arg コールバック引数
//---------------------------------------------------
typedef void( *EmitterPostCalculateCallback )( EmitterPostCalculateArg& arg );


//---------------------------------------------------
//! @brief  エミッタ描画処理コールバックに引数として渡される構造体です。
//!         ここから操作対象のエミッタにアクセスできます。
//---------------------------------------------------
struct EmitterDrawArg
{
    nn::gfx::CommandBuffer* pCommandBuffer;     //!< コマンドバッファ
    Emitter*                pEmitter;           //!< エミッタインスタンス
    ShaderType              shaderType;         //!< シェーダタイプ
    void*                   pUserParam;         //!< ユーザーデータ
    DrawParameterArg*       pDrawParameterArg;  //!< 描画パラメータ

    System* GetSystem() NN_NOEXCEPT;
};

//---------------------------------------------------
//! @brief  エミッタ描画処理コールバックの定義です。
//!         エミッタ描画処理時にコールされます。
//!         返り値に true を返すことで、通常のパーティクル描画をスキップできます。
//! @param[in] arg コールバック引数
//---------------------------------------------------
typedef bool( *EmitterDrawCallback )( EmitterDrawArg& arg );


//---------------------------------------------------
//! @brief  エミッタ破棄後コールバックに引数として渡される構造体です。
//!         ここから操作対象のエミッタにアクセスできます。
//---------------------------------------------------
struct EmitterFinalizeArg
{
    Emitter*    pEmitter;        //!< エミッタ
};

//---------------------------------------------------
//! @brief  エミッタ破棄後コールバックの定義です。
//!         エミッタ破棄後にコールされます。
//! @param[in] arg コールバック引数
//---------------------------------------------------
typedef void( *EmitterFinalizeCallback )( EmitterFinalizeArg& arg );

//@}

//----------------------------------------
//! @name エミッタセット コールバック
//@{

//---------------------------------------------------
//! @brief  エミッタセット初期化時コールバックに引数として渡される構造体です。
//!         ここから操作対象のエミッタセット・エフェクトシステムにアクセスできます。
//---------------------------------------------------
struct EmitterSetInitializeArg
{
    System*     pSystem;        //!< システムへのポインタ
    EmitterSet* pEmitterSet;    //!< エミッタセットへのポインタ
};
//---------------------------------------------------
//! @brief      エミッタセット初期化時コールバックの定義です。
//! @param[in]  arg コールバック引数
//---------------------------------------------------
typedef void( *EmitterSetInitializeCallback )( EmitterSetInitializeArg& arg );

//---------------------------------------------------
//! @brief  エミッタセット破棄時コールバックに引数として渡される構造体です。
//!         ここから操作対象のエミッタセット・エフェクトシステムにアクセスできます。
//---------------------------------------------------
struct EmitterSetFinalizeArg
{
    System*     pSystem;        //!< システムへのポインタ
    EmitterSet* pEmitterSet;    //!< エミッタセットへのポインタ
};
//---------------------------------------------------
//! @brief      エミッタセット破棄コールバックの定義です。
//! @param[in]  arg コールバック引数
//---------------------------------------------------
typedef void( *EmitterSetFinalizeCallback )( EmitterSetFinalizeArg& arg );

//@}


//----------------------------------------
//! @name 描画設定後 コールバック
//@{

//---------------------------------------------------
//! @brief  描画設定後コールバックに引数として渡される構造体です。
//!         ここから操作対象のエミッタやシェーダなどにアクセスできます。
//---------------------------------------------------
struct RenderStateSetArg
{
    //------------------------------------------------------------------------------
    //! @brief      シェーダを取得します。
    //! @return     シェーダクラスへのポインタ
    //------------------------------------------------------------------------------
    detail::Shader* GetShader() NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief      Systemクラスへのポインタを取得します。
    //! @return     Systemクラスへのポインタ
    //------------------------------------------------------------------------------
    System* GetSystem() NN_NOEXCEPT;

    nn::gfx::CommandBuffer*     pCommandBuffer;         //!< Gfxコマンドバッファ
    Emitter*                    pEmitter;               //!< エミッタインスタンス
    ShaderType                  shaderType;             //!< シェーダタイプ
    void*                       pUserParam;             //!< ユーザーパラメータ
    bool                        isComputeShaderMode;    //!< コンピュートシェーダを計算させるかどうか。
    DrawParameterArg*           pDrawParameterArg;      //!< 描画パラメータ
};

//---------------------------------------------------
//! @brief  描画設定後コールバックの定義です。
//!
//!         描画設定後にコールされます。
//! @param[in] arg コールバック引数
//---------------------------------------------------
typedef bool( *RenderStateSetCallback )( RenderStateSetArg& arg );

//@}



//----------------------------------------
//! @name エミッタ描画プロファイラコールバック
//@{

//---------------------------------------------------
//! @brief エミッタ描画プロファイラコールバックに渡される構造体です。
//---------------------------------------------------
struct DrawEmitterProfilerArg
{
    nn::gfx::CommandBuffer* pCommandBuffer;          //!< コマンドバッファ
    System*                 pSystem;                 //!< システム
    EmitterSet*             pEmitterSet;             //!< エミッタセットインスタンス
    Emitter*                pEmitter;                //!< エミッタインスタンス
    bool                    beforeRenderEmitter;     //!< エミッタセットコール時のエミッタ描画処理前かどうか
    ShaderType              shaderType;              //!< シェーダタイプ
    void*                   pUserParam;              //!< カスタムシェーダコールバックへ渡されるアドレス
    int                     resourceId;              //!< リソースID
    bool                    isComputeShaderMode;     //!< コンピュートシェーダを計算させるかどうか。
    DrawParameterArg*       pDrawParameterArg;       //!< 描画パラメータ
};

//---------------------------------------------------
//! @brief  エミッタ描画処理プロファイルコールバック。
//!
//!         エミッタ単位でのGPU処理負荷計測などに利用するコールバックです。
//! @param[in] arg コールバック引数
//---------------------------------------------------
typedef void( *DrawEmitterProfileCallback )( DrawEmitterProfilerArg& arg );

//@}

//----------------------------------------
//! @name エンディアンフリップ コールバック
//@{

//---------------------------------------------------
//! @brief  エンディアンフリップ コールバックに引数として渡される構造体です。
//!         ここからカスタムシェーダパラメータにアクセスできます。
//---------------------------------------------------
struct EndianFlipArg
{
    void*       pCustomShaderParam;         //!< カスタムシェーダパラメータ
    size_t      customShaderParamSize;      //!< カスタムシェーダパラメータサイズ
};

//---------------------------------------------------
//! @brief  エンディアンフリップコールバックの定義です。
//!
//!         リソース初期化時にコールされます。
//! @param[in] arg コールバック引数
//---------------------------------------------------
typedef bool( *EndianFlipCallback )( EndianFlipArg& arg );

//@}


//----------------------------------------
//! @name 描画パス コールバック
//@{

//---------------------------------------------------
//! @brief  描画パス 描画ステート設定後コールバックの定義です。
//!
//!         描画ステート設定後にコールされます。
//!         各描画パスで特定の描画ステート設定などを行う場合に利用します。
//!         詳しくは、カスタムシェーダドキュメントをご覧ください。
//! @param[in] arg コールバック引数
//---------------------------------------------------
typedef void( *DrawPathRenderStateSetCallback )( RenderStateSetArg& arg );

//@}

//----------------------------------------
//! @name カリング コールバック
//@{

//---------------------------------------------------
//! @brief  エミッタ LOD コールバックに引数として渡される構造体です。
//!         ここから操作対象のエミッタにアクセスできます。
//---------------------------------------------------
struct EmitterCalculateLodArg
{
    Emitter*        pEmitter;            //!< エミッタインスタンス
};

//---------------------------------------------------
//! @brief  エミッタ LOD コールバックの定義です。
//!
//!         コールバック内では、Emitter::calcSkipFlag/drawViewFlag の操作を行います。
//! @param[in] arg コールバック引数
//---------------------------------------------------
typedef EmitterCalculationResult ( *EmitterCalculateLodCallback )( EmitterCalculateLodArg& arg );

//---------------------------------------------------
//! @brief  エミッタカリングコールバックに引数として渡される構造体です。
//!         ここから操作対象のエミッタやユーザーパラメータにアクセスできます。
//---------------------------------------------------
struct EmitterDrawCullArg
{
    Emitter*        pEmitter;            //!< エミッタインスタンス
    void*           pUserParam;          //!< ユーザーパラメータ
};

//---------------------------------------------------
//! @brief  エミッタカリングコールバックの定義です。
//!
//!         コールバック内では、Emitter::calcSkipFlag/drawViewFlag の操作を行います。
//! @param[in] arg コールバック引数
//---------------------------------------------------
typedef bool( *EmitterDrawCullingCallback )( EmitterDrawCullArg& arg );

//@}

//---------------------------------------------------
//! @brief コールバックセット構造体です。
//---------------------------------------------------
class CallbackSet
{
public:
    // エンディアンフリップ
    EndianFlipCallback              endianFlip;             //!< エンディアンフリップ時コールバック

    // エミッタ単位のコールバック
    EmitterInitializeCallback       emitterInitialize;      //!< エミッタ初期化時コールバック
    EmitterPreCalculateCallback     emitterPreCalculate;    //!< エミッタ計算前コールバック
    EmitterMatrixSetCallback        emitterMatrixSet;       //!< エミッタ行列セットコールバック
    EmitterPostCalculateCallback    emitterPostCalculate;   //!< エミッタ計算後コールバック
    EmitterDrawCallback             emitterDraw;            //!< エミッタ描画コールバック
    EmitterFinalizeCallback         emitterFinalize;        //!< エミッタ破棄コールバック

    // パーティクル単位のコールバック
    ParticleEmitCallback            particleEmit;           //!< パーティクル放出コールバック
    ParticleRemoveCallback          particleRemove;         //!< パーティクル削除コールバック
    ParticleCalculateCallback       particleCalculate;      //!< パーティクル計算コールバック
    RenderStateSetCallback          renderStateSet;         //!< レンダーステートセットコールバック

    //------------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //------------------------------------------------------------------------------
    CallbackSet() NN_NOEXCEPT
        : endianFlip( NULL )
        , emitterInitialize( NULL )
        , emitterPreCalculate( NULL )
        , emitterMatrixSet( NULL )
        , emitterPostCalculate( NULL )
        , emitterDraw( NULL )
        , emitterFinalize( NULL )
        , particleEmit( NULL )
        , particleRemove( NULL )
        , particleCalculate( NULL )
        , renderStateSet( NULL )
    {
    }

    //------------------------------------------------------------------------------
    //! @brief  有効なコールバックが入っているかを取得
    //! @return コールバックが入っていればtrue
    //------------------------------------------------------------------------------
    bool IsValid() const NN_NOEXCEPT
    {
        bool ret = false;

        if( endianFlip || particleEmit || particleRemove || emitterInitialize || particleCalculate ||
            emitterPreCalculate || emitterPostCalculate || emitterDraw || renderStateSet || emitterFinalize )
        {
            ret = true;
        }
        return ret;
    }
};

//------------------------------------------------------------------------------
//! @brief          エンディアン反転 コールバック
//! @param[in] arg  コールバック引数
//! @return         正常に終了した場合trueを返します。
//------------------------------------------------------------------------------
bool EndianFlipCallbackImpl( nn::vfx::EndianFlipArg& arg ) NN_NOEXCEPT;

//------------------------------------------------------------------------------
//! @brief          描画設定後コールバック
//! @param[in] arg  コールバック引数
//! @return         正常に終了した場合trueを返します。
//------------------------------------------------------------------------------
bool BindReservedCustomShaderConstantBuffer( nn::vfx::RenderStateSetArg& arg ) NN_NOEXCEPT;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//! @brief コールバックの呼び出し処理の集約（RenderStateSet）
//! @param[in] arg  コールバック引数
//! @return         正常に終了した場合trueを返します。
//------------------------------------------------------------------------------
bool InvokeRenderStateSetCallback( nn::vfx::RenderStateSetArg& arg ) NN_NOEXCEPT;

//------------------------------------------------------------------------------
//! @brief          コールバックの呼び出し処理の集約（エミッタ初期化時）
//! @param[in] arg  コールバック引数
//! @return         正常に終了した場合trueを返します。
//------------------------------------------------------------------------------
bool InvokeEmitterInitializeCallback( nn::vfx::EmitterInitializeArg& arg ) NN_NOEXCEPT;

//------------------------------------------------------------------------------
//! @brief          コールバックの呼び出し処理の集約（エミッタ解放時）
//! @param[in] arg  コールバック引数
//------------------------------------------------------------------------------
void InvokeEmitterFinalizeCallback( nn::vfx::EmitterFinalizeArg& arg ) NN_NOEXCEPT;

//------------------------------------------------------------------------------
//! @brief              エミッタマトリクス設定後のコールバック呼び出し
//! @param[in] pEmitter  エミッタへのポインタ
//------------------------------------------------------------------------------
void InvokeEmitterMatrixSetCallback( Emitter* pEmitter ) NN_NOEXCEPT;

//------------------------------------------------------------------------------
//! @brief          コールバックの呼び出し処理の集約（パーティクル計算前）
//! @param[in] arg  コールバック引数
//------------------------------------------------------------------------------
void InvokeEmitterPreCalculateCallback( nn::vfx::EmitterPreCalculateArg& arg ) NN_NOEXCEPT;

//------------------------------------------------------------------------------
//! @brief              コールバックの呼び出し処理の集約（パーティクル計算後）
//! @param[in] pEmitter  エミッタへのポインタ
//------------------------------------------------------------------------------
void InvokeEmitterPostCalculateCallback( Emitter* pEmitter ) NN_NOEXCEPT;

//------------------------------------------------------------------------------
//! @brief          コールバックの呼び出し処理の集約（エミッタ描画時）
//! @param[in] arg  コールバック引数
//! @return         正常に終了した場合trueを返します。
//------------------------------------------------------------------------------
bool InvokeEmitterDrawCallback( nn::vfx::EmitterDrawArg& arg ) NN_NOEXCEPT;

//------------------------------------------------------------------------------
//! @brief              コールバックの呼び出し処理の集約（全パーティクル削除時）
//! @param[in] pEmitter  エミッタへのポインタ
//------------------------------------------------------------------------------
void InvokeParticleRemoveCallbackAll( Emitter* pEmitter ) NN_NOEXCEPT;

//------------------------------------------------------------------------------
} // namespace vfx
} // namespace nn

namespace nn {
namespace vfx {

//---------------------------------------------------------------------------
//! @brief      パーティクル放出時のコールバック呼び出し
//! @param[in]  pEmitter                エミッタへのポインタ
//! @param[in]  writeIndex              自身が格納されるパーティクル配列のインデックス
//! @param[in]  particleEmitCallbackEP  エミッタプラグインコールバックへのポインタ
//! @param[in]  particleEmitCallbackCA  カスタムアクションコールバックへのポインタ
//! @param[in]  particleEmitCallbackCS  カスタムシェーダコールバックへのポインタ
//! @param[in]  pUserData               ユーザーデータへのポインタ
//---------------------------------------------------------------------------
inline void InvokeParticleEmitCallback(
    Emitter*                pEmitter,
    int                     writeIndex,
    ParticleEmitCallback    particleEmitCallbackEP,
    ParticleEmitCallback    particleEmitCallbackCA,
    ParticleEmitCallback    particleEmitCallbackCS,
    void*                   pUserData ) NN_NOEXCEPT
{
    //放出された瞬間なのでタイムは0
    const float time = 0;
    const float life = pEmitter->GetParticleLife( writeIndex );
    ParticleEmitArg arg( pEmitter, time, life, writeIndex );
    arg.pUserData = pUserData;

    if( particleEmitCallbackEP )
    {
        particleEmitCallbackEP( arg );
    }
    if( particleEmitCallbackCA )
    {
        particleEmitCallbackCA( arg );
    }
    else if( particleEmitCallbackCS )
    {
        particleEmitCallbackCS( arg );
    }

    // 返り値のデータを保持する
    pEmitter->SetEmitterPluginData( writeIndex, arg.pEmitterPluginData );
    pEmitter->SetUserData( writeIndex, arg.pUserData );
}

//---------------------------------------------------------------------------
//! @brief パーティクル計算時のコールバック呼び出し
//! @param[in] pEmitter      エミッタへのポインタ
//! @param[in] index        自身が格納されているパーティクル配列のインデックス
//! @param[in] time         パーティクル時間
//! @param[in] life         パーティクル寿命
//! @param[in] particleCalculateCallbackEP  エミッタプラグインコールバックへのポインタ
//! @param[in] particleCalculateCallbackCA  カスタムアクションコールバックへのポインタ
//! @param[in] particleCalculateCallbackCS  カスタムシェーダコールバックへのポインタ
//---------------------------------------------------------------------------
inline void InvokeParticleCalculateCallback(
    Emitter* pEmitter,
    int index,
    float time,
    float life,
    ParticleCalculateCallback particleCalculateCallbackEP,
    ParticleCalculateCallback particleCalculateCallbackCA,
    ParticleCalculateCallback particleCalculateCallbackCS ) NN_NOEXCEPT
{
    ParticleCalculateArg arg( pEmitter, time, life, index );
    arg.pUserData = pEmitter->GetUserData( index );
    arg.pEmitterPluginData = pEmitter->GetEmitterPluginData( index );

    if( particleCalculateCallbackEP )
    {
        particleCalculateCallbackEP( arg );
    }
    if( particleCalculateCallbackCA )
    {
        particleCalculateCallbackCA( arg );
    }
    if( particleCalculateCallbackCS )
    {
        particleCalculateCallbackCS( arg );
    }

    // TODO :
    pEmitter->SetUserData( index, arg.pUserData );
}

//---------------------------------------------------------------------------
//! @brief パーティクル削除時のコールバック呼び出し
//! @param[in] pEmitter      エミッタへのポインタ
//! @param[in] index        自身が格納されているパーティクル配列のインデックス
//! @param[in] time         パーティクル時間
//! @param[in] life         パーティクル寿命
//! @param[in] particleRemoveCallbackEP  エミッタプラグインコールバックへのポインタ
//! @param[in] particleRemoveCallbackCA  カスタムアクションコールバックへのポインタ
//! @param[in] particleRemoveCallbackCS  カスタムシェーダコールバックへのポインタ
//---------------------------------------------------------------------------
inline void InvokeParticleRemoveCallback(
    Emitter* pEmitter,
    int index,
    float time,
    float life,
    ParticleRemoveCallback particleRemoveCallbackEP,
    ParticleRemoveCallback particleRemoveCallbackCA,
    ParticleRemoveCallback particleRemoveCallbackCS ) NN_NOEXCEPT
{
    ParticleRemoveArg arg( pEmitter, time, life, index );
    arg.pUserData = pEmitter->GetUserData( index );
    arg.pEmitterPluginData = pEmitter->GetEmitterPluginData( index );

    if( particleRemoveCallbackEP )
    {
        particleRemoveCallbackEP( arg );
    }
    if( particleRemoveCallbackCA )
    {
        particleRemoveCallbackCA( arg );
    }
    if( particleRemoveCallbackCS )
    {
        particleRemoveCallbackCS( arg );
    }

    // ユーザーデータの参照を消去。
    pEmitter->SetEmitterPluginData( index, NULL );
    pEmitter->SetUserData( index, NULL );
}

//------------------------------------------------------------------------------
// エミッタ描画時のコールバック呼び出し
//------------------------------------------------------------------------------
inline bool InvokeEmitterDrawCallback( nn::vfx::EmitterDrawArg& arg ) NN_NOEXCEPT
{
    const CallbackSet* pCallbackSetEP = arg.pEmitter->GetCallbackSet( detail::CallbackSetType_EmitterPlugin );
    const CallbackSet* pCallbackSetCA = arg.pEmitter->GetCallbackSet( detail::CallbackSetType_CustomAction );
    const CallbackSet* pCallbackSetCS = arg.pEmitter->GetCallbackSet( detail::CallbackSetType_CustomShader );

    if( pCallbackSetEP && pCallbackSetEP->emitterDraw )
    {
        if( pCallbackSetEP->emitterDraw( arg ) )
        {
            return true;
        }
    }
    if( pCallbackSetCA && pCallbackSetCA->emitterDraw )
    {
        if( pCallbackSetCA->emitterDraw( arg ) )
        {
            return true;
        }
    }
    if( pCallbackSetCS && pCallbackSetCS->emitterDraw )
    {
        if( pCallbackSetCS->emitterDraw( arg ) )
        {
            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
// エミッタ計算前のコールバック呼び出し
//------------------------------------------------------------------------------
inline void InvokeEmitterPreCalculateCallback( nn::vfx::EmitterPreCalculateArg& arg ) NN_NOEXCEPT
{
    const CallbackSet* pCallbackSetEP = arg.pEmitter->GetCallbackSet( detail::CallbackSetType_EmitterPlugin );
    const CallbackSet* pCallbackSetCA = arg.pEmitter->GetCallbackSet( detail::CallbackSetType_CustomAction );
    const CallbackSet* pCallbackSetCS = arg.pEmitter->GetCallbackSet( detail::CallbackSetType_CustomShader );

    if( pCallbackSetEP && pCallbackSetEP->emitterPreCalculate )
    {
        pCallbackSetEP->emitterPreCalculate( arg );
    }
    if( pCallbackSetCA && pCallbackSetCA->emitterPreCalculate )
    {
        pCallbackSetCA->emitterPreCalculate( arg );
    }
    if( pCallbackSetCS && pCallbackSetCS->emitterPreCalculate )
    {
        pCallbackSetCS->emitterPreCalculate( arg );
    }
}

//------------------------------------------------------------------------------
// エミッタマトリクス設定後のコールバック呼び出し
//------------------------------------------------------------------------------
inline void InvokeEmitterMatrixSetCallback( Emitter* pEmitter ) NN_NOEXCEPT
{
    nn::vfx::EmitterMatrixSetArg arg;
    arg.pEmitter = pEmitter;

    const CallbackSet* pCallbackSetEP = pEmitter->GetCallbackSet( detail::CallbackSetType_EmitterPlugin );
    const CallbackSet* pCallbackSetCA = pEmitter->GetCallbackSet( detail::CallbackSetType_CustomAction );
    const CallbackSet* pCallbackSetCS = pEmitter->GetCallbackSet( detail::CallbackSetType_CustomShader );

    if( pCallbackSetEP && pCallbackSetEP->emitterMatrixSet )
    {
        pCallbackSetEP->emitterMatrixSet( arg );
    }
    if( pCallbackSetCA && pCallbackSetCA->emitterMatrixSet )
    {
        pCallbackSetCA->emitterMatrixSet( arg );
    }
    if( pCallbackSetCS && pCallbackSetCS->emitterMatrixSet )
    {
        pCallbackSetCS->emitterMatrixSet( arg );
    }
}

//------------------------------------------------------------------------------
// エミッタ計算後のコールバック呼び出し
//------------------------------------------------------------------------------
inline void InvokeEmitterPostCalculateCallback( Emitter* pEmitter ) NN_NOEXCEPT
{
    nn::vfx::EmitterPostCalculateArg arg;
    arg.pEmitter = pEmitter;

    const CallbackSet* pCallbackSetEP = pEmitter->GetCallbackSet( detail::CallbackSetType_EmitterPlugin );
    const CallbackSet* pCallbackSetCA = pEmitter->GetCallbackSet( detail::CallbackSetType_CustomAction );
    const CallbackSet* pCallbackSetCS = pEmitter->GetCallbackSet( detail::CallbackSetType_CustomShader );

    if( pCallbackSetEP && pCallbackSetEP->emitterPostCalculate )
    {
        pCallbackSetEP->emitterPostCalculate( arg );
    }
    if( pCallbackSetCA && pCallbackSetCA->emitterPostCalculate )
    {
        pCallbackSetCA->emitterPostCalculate( arg );
    }
    if( pCallbackSetCS && pCallbackSetCS->emitterPostCalculate )
    {
        pCallbackSetCS->emitterPostCalculate( arg );
    }
}

} // namespace vfx
} // namespace nn
