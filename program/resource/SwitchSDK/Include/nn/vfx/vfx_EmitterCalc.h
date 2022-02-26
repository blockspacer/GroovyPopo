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

#include <nn/vfx/vfx_Emitter.h>
#include <nn/vfx/vfx_UniformBlock.h>

namespace nn {
namespace vfx {

class System;
class Emitter;
struct DrawParameterArg;

namespace detail {

struct Particle;
struct ParticleProperty;

//---------------------------------------------------------------------------
//! @brief  パーティクル挙動計算関数ポインタ定義です。
//---------------------------------------------------------------------------
typedef void( *ParticleBehavior )( nn::util::Vector4fType* pOutPos, nn::util::Vector4fType* pOutVec, Emitter* pEmitter );

//---------------------------------------------------------------------------
//! @brief  エミッタ挙動計算クラス
//---------------------------------------------------------------------------
class EmitterCalculator
{
    NN_DISALLOW_COPY( EmitterCalculator );
public:
    //---------------------------------------------------------------------------
    //! @brief      コンストラクタ。
    //! @param[in]  pSystem             システムへのポインタ
    //---------------------------------------------------------------------------
    explicit EmitterCalculator( System* pSystem ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      デストラクタ。
    //---------------------------------------------------------------------------
    ~EmitterCalculator() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エミッタ計算処理をします。
    //! @param[in]  pEmitter            エミッタへのポインタ
    //! @param[in]  frameRate           フレームレート値
    //! @param[in]  bufferSwapMode      バッファスワップモード
    //! @param[in]  isFade              フェード中か
    //! @param[in]  isEmission          TBD
    //! @param[in]  isForceCalc         エミッタタイプによらずパーティクル計算処理を行う
    //! @return     成功した場合trueを返す。
    //---------------------------------------------------------------------------
    bool Calculate( Emitter* pEmitter, float frameRate, BufferSwapMode bufferSwapMode, bool isFade, bool isEmission, bool isForceCalc ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      パーティクル計算処理をします。
    //! @param[in]  pEmitter            エミッタへのポインタ
    //---------------------------------------------------------------------------
    void CalculateParticle( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      パーティクルを初期化します。
    //! @param[in]  pEmitter                エミッタへのポインタ
    //! @param[in]  attrIndex               アトリビュートインデックス
    //! @param[in]  pParticleData           パーティクルデータへのポインタ
    //! @param[in]  pParentParticleData     親パーティクル固有のパーティクルデータへのポインタ
    //! @param[in]  pParticleProperty       パーティクルアトリビュートへのポインタ
    //! @param[in]  particleIndex           パーティクルのインデックス
    //! @param[in]  emitIndex               自身が格納されるパーティクル配列のインデックス
    //! @param[in]  maxEmitCount            放出の最大数
    //! @param[in]  randomValue             ランダム値
    //! @param[in]  pEmitReservationInfo    マニュアル放出用の放出予約データ
    //! @return     成功した場合trueを返す。
    //---------------------------------------------------------------------------
    bool InitializeParticle(
        Emitter*                   pEmitter,
        int                        attrIndex,
        ParticleData*              pParticleData,
        ParentParticleData*        pParentParticleData,
        ParticleProperty*          pParticleProperty,
        int                        particleIndex,
        int                        emitIndex,
        int                        maxEmitCount,
        float                      randomValue,
        const EmitReservationInfo* pEmitReservationInfo ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エミッタ描画処理をします。
    //! @param[in]  pCommandBuffer      コマンドバッファ
    //! @param[in]  pEmitter            エミッタへのポインタ
    //! @param[in]  pUserParam          ユーザーパラメータへのポインタ
    //! @param[in] pDrawParameterArg    描画パラメータへのポインタ
    //! @return     成功した場合trueを返す。
    //---------------------------------------------------------------------------
    bool Draw( nn::gfx::CommandBuffer* pCommandBuffer, Emitter* pEmitter, void* pUserParam, DrawParameterArg* pDrawParameterArg ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      既にシェーダがバインド済みの前提で、エミッタ描画処理をします。（複数回描画処理をするため）
    //! @param[in]  pCommandBuffer      コマンドバッファ
    //! @param[in]  pEmitter            エミッタへのポインタ
    //! @param[in]  pShader             シェーダ
    //! @param[in]  pUserParam          ユーザーパラメータへのポインタ
    //! @param[in]  pDrawParameterArg   描画パラメータへのポインタ
    //! @return     成功した場合trueを返す。
    //---------------------------------------------------------------------------
    bool DrawEmitterUsingBoundShader( nn::gfx::CommandBuffer* pCommandBuffer, Emitter* pEmitter, Shader* pShader, void* pUserParam, DrawParameterArg* pDrawParameterArg ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エミッタの動的定数バッファを生成します。
    //! @param[in]  pEmitter            エミッタへのポインタ
    //! @param[in]  frameRate           フレームレート
    //! @param[in]  savingFrameRate     貯留したフレームレート
    //---------------------------------------------------------------------------
    void MakeDynamicConstantBuffer( Emitter* pEmitter, float frameRate, float savingFrameRate ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エミッタストリームアウト計算処理をします。
    //! @param[in]  pCommandBuffer          コマンドバッファ
    //! @param[in]  pEmitter                エミッタへのポインタ
    //! @param[in]  pComputeShader          コンピュートシェーダへのポインタ
    //! @param[in]  globalCounter           グローバルカウンタ
    //! @param[in]  doComputeShaderProcess  コンピュートシェーダの計算をするかどうか
    //! @param[in]  pUserParam              ユーザーパラメータへのポインタ
    //! @param[in]  doFlush                 GPUキャッシュをフラッシュするか
    //! @return     成功した場合trueを返す。
    //---------------------------------------------------------------------------
    bool CalculateComputeShader( nn::gfx::CommandBuffer* pCommandBuffer, Emitter* pEmitter, const detail::ComputeShader* pComputeShader, int globalCounter, bool doComputeShaderProcess, void* pUserParam, bool doFlush ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エミッタが用いる通常テクスチャ（テクスチャ0/1/2、フレームバッファ、デプスバッファ、カールノイズ）をまとめて設定します。
    //! @param[in]  pCommandBuffer      コマンドバッファ
    //! @param[in]  pEmitter            エミッタへのポインタ
    //! @param[in]  shader              シェーダへのポインタ
    //! @param[in]  pDrawParameterArg   描画パラメータ
    //! @return     成功した場合trueを返す。
    //---------------------------------------------------------------------------
    bool SetDefaultEmitterTextures( nn::gfx::CommandBuffer* pCommandBuffer, Emitter* pEmitter, Shader* shader, DrawParameterArg* pDrawParameterArg ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エミッタが用いる通常定数バッファ（ビュー、静的、動的、フィールド）をまとめて設定します。
    //! @param[in]  pCommandBuffer      コマンドバッファ
    //! @param[in]  pEmitter            エミッタへのポインタ
    //! @param[in]  pShader             シェーダへのポインタ
    //! @param[in]  pDrawParameterArg   描画パラメータ
    //! @return     成功した場合trueを返す。
    //---------------------------------------------------------------------------
    bool SetDefaultEmitterConstantBuffers( nn::gfx::CommandBuffer* pCommandBuffer, Emitter* pEmitter, Shader* pShader, DrawParameterArg* pDrawParameterArg ) NN_NOEXCEPT;

    //---------------------------------------------------
    //! @brief      回転行列を作成する（回転の適用順: Y->Z->X）
    //! @param[out] pOutMatrix   出力を格納する Matrix4x4fType へのポインタ
    //! @param[in]  rotate       入力となる回転値を収めた Vector3fType
    //---------------------------------------------------
    static void MakeRotationMatrixYZX( nn::util::Matrix4x4fType* pOutMatrix, const nn::util::Vector3fType& rotate ) NN_NOEXCEPT;
    //---------------------------------------------------
    //! @brief      回転行列を作成する（回転の適用順: X->Y->Z）
    //! @param[out] pOutMatrix   出力を格納する Matrix4x4fType へのポインタ
    //! @param[in]  rotate       入力となる回転値を収めた Vector3fType
    //---------------------------------------------------
    static void MakeRotationMatrixXYZ( nn::util::Matrix4x4fType* pOutMatrix, const nn::util::Vector3fType& rotate ) NN_NOEXCEPT;
    //---------------------------------------------------
    //! @brief      回転行列を作成する（回転の適用順: Z->X->Y）
    //! @param[out] pOutMatrix  出力を格納する Matrix4x4fType へのポインタ
    //! @param[in]  rotate      入力となる回転値を収めた Vector3fType
    //---------------------------------------------------
    static void MakeRotationMatrixZXY( nn::util::Matrix4x4fType* pOutMatrix, const nn::util::Vector3fType& rotate ) NN_NOEXCEPT;

    //---------------------------------------------------
    //! @brief      現在のパーティクル回転値を計算する
    //! @param[out] pOutValue           出力を収めるVEC4へのポインタ
    //! @param[in]  pEmitterResource    エミッタリソースへのポインタ
    //! @param[in]  initialRotate       初期回転値
    //! @param[in]  random              ランダム
    //! @param[in]  time                現在時刻
    //---------------------------------------------------
    void CalculateRotationMatrix(
        nn::util::Vector3fType*         pOutValue,
        const EmitterResource*          pEmitterResource,
        const nn::util::Vector4fType&   initialRotate,
        const nn::util::Vector4fType&   random,
        float                           time ) NN_NOEXCEPT;

    //---------------------------------------------------
    //! @brief      現在のパーティクル回転値を計算する
    //! @param[out] pOutValue           TBD
    //! @param[out] pEmitterResource    TBD
    //! @param[out] random             TBD
    //---------------------------------------------------
    void CalculateRotationMatrix(
        nn::util::Vector3fType*         pOutValue,
        const EmitterResource*          pEmitterResource,
        const nn::util::Vector4fType&   random ) NN_NOEXCEPT;


    //---------------------------------------------------
    //! @brief      現在のパーティクルスケールを計算する
    //! @param[out] pOutValue           出力を収めるVEC4へのポインタ
    //! @param[in]  pEmitterResource    エミッタリソースへのポインタ
    //! @param[in]  initialScale       初期スケール
    //! @param[in]  random             ランダム値
    //! @param[in]  life                寿命
    //! @param[in]  time                現在時刻
    //---------------------------------------------------
    void CalculateParticleScaleVecFromTime(
        nn::util::Vector3fType*         pOutValue,
        const EmitterResource*          pEmitterResource,
        const nn::util::Vector4fType&   initialScale,
        const nn::util::Vector4fType&   random,
        float                           life,
        float                           time ) NN_NOEXCEPT;

    //---------------------------------------------------
    //! @brief      現在のパーティクルスケールを計算する
    //! @param[out] pOutValue           出力を収めるVEC4へのポインタ
    //! @param[in]  pEmitterResource    エミッタリソースへのポインタ
    //! @param[in]  initialScale       初期スケール
    //! @param[in]  random             ランダム値
    //! @param[in]  life                寿命
    //! @param[in]  frameCount          現在フレーム
    //---------------------------------------------------
    void CalculateParticleScaleVecFromFrame(
        nn::util::Vector3fType*         pOutValue,
        const EmitterResource*          pEmitterResource,
        const nn::util::Vector4fType&   initialScale,
        const nn::util::Vector4fType&   random,
        float                           life,
        float                           frameCount ) NN_NOEXCEPT;

    //---------------------------------------------------
    //! @brief      現在のパーティクルカラー0を計算する
    //! @param[out] pOutValue           出力を収めるVEC4へのポインタ
    //! @param[in]  pEmitterResource    エミッタリソースへのポインタ
    //! @param[in]  random              ランダム値
    //! @param[in]  emitterColor        エミッタカラー
    //! @param[in]  emitterColorAnim    カラーのエミッタアニメーション
    //! @param[in]  emitterAlphaAnim    アルファのエミッタアニメーション
    //! @param[in]  life                寿命
    //! @param[in]  time                現在時刻
    //---------------------------------------------------
    void CalculateParticleColor0VecFromTime(
        nn::util::Vector4fType*         pOutValue,
        const EmitterResource*          pEmitterResource,
        const nn::util::Vector4fType&   random,
        const nn::util::Vector4fType&   emitterColor,
        const nn::util::Vector3fType&   emitterColorAnim,
        float                           emitterAlphaAnim,
        float                           life,
        float                           time ) NN_NOEXCEPT;

    //---------------------------------------------------
    //! @brief      現在のパーティクルカラー1を計算する
    //! @param[out] pOutValue           出力を収めるVEC4へのポインタ
    //! @param[in]  pEmitterResource    エミッタリソースへのポインタ
    //! @param[in]  random              ランダム値
    //! @param[in]  emitterColor        エミッタカラー
    //! @param[in]  emitterColorAnim    カラーのエミッタアニメーション
    //! @param[in]  emitterAlphaAnim    アルファのエミッタアニメーション
    //! @param[in]  life                寿命
    //! @param[in]  time                現在時刻
    //---------------------------------------------------
    void CalculateParticleColor1VecFromTime(
        nn::util::Vector4fType*         pOutValue,
        const EmitterResource*          pEmitterResource,
        const nn::util::Vector4fType&   random,
        const nn::util::Vector4fType&   emitterColor,
        const nn::util::Vector3fType&   emitterColorAnim,
        float                           emitterAlphaAnim,
        float                           life,
        float                           time ) NN_NOEXCEPT;

    //---------------------------------------------------
    //! @brief      現在のパーティクルカラー0 の、エミッタカラー、エミッタセットカラー、カラースケールを除いた生の値を計算する
    //! @param[out] pOutValue           出力を収めるVEC4へのポインタ
    //! @param[in]  pEmitterResource    エミッタリソースへのポインタ
    //! @param[in]  random              ランダム値
    //! @param[in]  life                寿命
    //! @param[in]  time                現在時刻
    //---------------------------------------------------
    void CalculateParticleColor0RawValue(
        nn::util::Vector4fType*         pOutValue,
        const EmitterResource*          pEmitterResource,
        const nn::util::Vector4fType&   random,
        float                           life,
        float                           time ) NN_NOEXCEPT;

    //---------------------------------------------------
    //! @brief      現在のパーティクルカラー1 の、エミッタカラー、エミッタセットカラー、カラースケールを除いた生の値を計算する
    //! @param[out] pOutValue           出力を収めるVEC4へのポインタ
    //! @param[in]  pEmitterResource    エミッタリソースへのポインタ
    //! @param[in]  random              ランダム値
    //! @param[in]  life                寿命
    //! @param[in]  time                現在時刻
    //---------------------------------------------------
    void CalculateParticleColor1RawValue(
        nn::util::Vector4fType*         pOutValue,
        const EmitterResource*          pEmitterResource,
        const nn::util::Vector4fType&   random,
        float                           life,
        float                           time ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エミッタ行列（Emitter::m_MatrixSrt, Emitter::m_MatrixRt）を更新します。
    //! @param[in]  pEmitter    エミッタへのポインタ
    //---------------------------------------------------------------------------
    void UpdateEmitterMatrix( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      パーティクル用の GPU バッファを最新の状態にします（※CPUエミッタ専用）
    //! @param[in]  pEmitter    エミッタへのポインタ
    //---------------------------------------------------------------------------
    void UpdateCurrentParticleGpuBufferForCpuEmitter( Emitter* pEmitter ) NN_NOEXCEPT;

private:
    //---------------------------------------------------------------------------
    //! @brief      放出処理を行います。
    //! @param[out] pOutEmitDone            放出終了フラグ
    //! @param[in]  pEmitter                エミッタへのポインタ
    //! @param[in]  emitCount               放出数
    //! @param[in]  forceSearch             空きパーティクルを強制的に検索するかどうか
    //! @param[in]  pEmitReservationInfo    マニュアル放出用の放出予約データ
    //! @return     最後に追加されたパーティクルのインデックス値
    //---------------------------------------------------------------------------
    int Emit( uint8_t* pOutEmitDone, Emitter* pEmitter, int emitCount, bool forceSearch = false, const EmitReservationInfo* pEmitReservationInfo = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          エミッタの放出条件を満たしているならパーティクルを放出します。
    //! @param[in,out]  pOutEmitCount       放出カウンタ
    //! @param[in,out]  pOutEmitSaving      放出貯蓄（前回放出した余剰）
    //! @param[in,out]  pOutEmitInterval    放出間隔
    //! @param[out]     pOutEmitDone        放出を終えたかどうかのフラグ
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      frameRate           フレームレート
    //! @param[in]      forceSearch         空きパーティクルを強制的に検索するかどうか
    //---------------------------------------------------------------------------
    void TryEmitParticle( float* pOutEmitCount, float* pOutEmitSaving, float* pOutEmitInterval, uint8_t* pOutEmitDone, Emitter* pEmitter, float frameRate, bool forceSearch = false ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      パーティクル情報を計算する
    //! @param[out] pOutDstParticle     出力先のパーティクル情報
    //! @param[in]  pEmitter            エミッタへのポインタ
    //! @param[in]  time                時刻
    //! @param[in]  life                寿命
    //! @param[in]  index               自身が格納されるパーティクル配列のインデックス
    //---------------------------------------------------------------------------
    void CalculateParticleInfo( Particle* pOutDstParticle, Emitter* pEmitter, float time, float life, int index ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      親パーティクルからの継承処理を行う
    //! @param[in]  pEmitter            エミッタへのポインタ
    //! @param[in]  writeIndex          自身が格納されるパーティクル配列のインデックス
    //---------------------------------------------------------------------------
    void InheritParentParticleInfo( Emitter* pEmitter, int writeIndex ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      検索して死亡枠があればそこに追加、死亡枠が無い場合は後続に追加。
    //! @param[out] pOutEmitDone            放出を終えたかどうかのフラグ
    //! @param[in]  pEmitter                エミッタへのポインタ
    //! @param[in]  emitCount               放出数
    //! @param[in]  forceSearch             空きパーティクルを強制的に検索するかどうか
    //! @param[in]  pEmitReservationInfo    マニュアル放出用の放出予約データ
    //! @return     最後に追加されたパーティクルのインデックス値
    //---------------------------------------------------------------------------
    int EmitBySearchOrder( uint8_t* pOutEmitDone, Emitter* pEmitter, int emitCount, bool forceSearch, const EmitReservationInfo* pEmitReservationInfo = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      生成順に詰め込む。
    //! @param[in]  pEmitter            エミッタへのポインタ
    //! @param[in]  emitCount           放出数
    //! @return     最後に追加されたパーティクルのインデックス値
    //---------------------------------------------------------------------------
    int EmitBySortOrder( Emitter* pEmitter, int emitCount ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      ソート済みパーティクル描画処理を行います。
    //! @param[in]  pCommandBuffer      エミッタへのポインタ
    //! @param[in]  pEmitter            エミッタへのポインタ
    //! @param[in]  pShader             シェーダへのポインタ
    //! @param[in]  pPrimitive          プリミティブへのポインタ
    //! @param[in]  pDrawParameterArg   描画パラメータへのポインタ
    //! @return     成功した場合trueを返す。
    //---------------------------------------------------------------------------
    bool EntrySortedParticle( nn::gfx::CommandBuffer* pCommandBuffer, Emitter* pEmitter, Shader* pShader, IPrimitive* pPrimitive, DrawParameterArg* pDrawParameterArg ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          点エミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitPoint( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          円エミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitCircle( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          円等分割エミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitCircleEquallyDivided( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          円フィルエミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitCircleFill( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  球エミッタ計算処理を行います。
    //! @param[in,out] pOutLocalPos ローカル位置
    //! @param[in,out] pOutLocalVec ローカル速度
    //! @param[in] pEmitter      エミッタへのポインタ
    //! @param[in] emitIndex    自身が格納されるパーティクル配列のインデックス
    //! @param[in] maxEmitCount      放出される最大数
    //! @param[in] randomValue  ランダム値
    //! @param[in] pEmitterAnimData  エミッタアニメーションへのポインタ
    //! @return                 成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitSphere( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          球 32等分割エミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitSphereEqually32Divided( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          球 64等分割エミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitSphereEqually64Divided( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          球 フィルエミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitSphereFill( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          シリンダー エミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitCylinder( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          シリンダーフィル エミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitCylinderFill( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          ボックス エミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitBox( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          ボックス フィルエミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitBoxFill( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          ライン エミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitLine( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          ライン 等分割エミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitLineEquallyDivided( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          矩形 エミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitRectangle( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          プリミティブ エミッタ計算処理を行います。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------------------------------
    static bool CalculateEmitPrimitive( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData ) NN_NOEXCEPT;

    //---------------------------------------------------
    //! @briefprivate   エミッタ形状算出 関数ポインタ定義です。
    //! @param[in,out]  pOutLocalPos        ローカル位置
    //! @param[in,out]  pOutLocalVec        ローカル速度
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      emitIndex           自身が格納されるパーティクル配列のインデックス
    //! @param[in]      maxEmitCount        放出される最大数
    //! @param[in]      randomValue         ランダム値
    //! @param[in]      pEmitterAnimData    エミッタアニメーションへのポインタ
    //! @return         成功した場合trueを返す。
    //---------------------------------------------------
    typedef bool( *EmitFunction )( nn::util::Vector3fType* pOutLocalPos, nn::util::Vector3fType* pOutLocalVec, Emitter* pEmitter, int emitIndex, int maxEmitCount, float randomValue, EmitterAnimValue* pEmitterAnimData );

    //---------------------------------------------------------------------------
    //! @brief          パーティクルの挙動計算を行う 積算版
    //! @param[in,out]  pOutPos         位置
    //! @param[in,out]  pOutVec         速度
    //! @param[in,out]  pOutBirthTime   パーティクル生成時刻
    //! @param[in,out]  pOutLife        パーティクル寿命
    //! @param[in]      pEmitter        エミッタへのポインタ
    //! @param[in]      index           パーティクルのインデックス
    //! @param[in]      time            パーティクル時刻
    //! @param[in]      posBack         位置（バックバッファ）
    //! @param[in]      vecBack         速度（バックバッファ）
    //---------------------------------------------------------------------------
    void CalculateParticleBehavior(
        nn::util::Vector3fType* pOutPos,
        nn::util::Vector3fType* pOutVec,
        float* pOutBirthTime,
        float* pOutLife,
        Emitter* pEmitter,
        int index,
        float time,
        const nn::util::Vector3fType& posBack,
        const nn::util::Vector3fType& vecBack ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          フィールド 位置に加算 の挙動計算をします。
    //! @param[in,out]  pOutPos             TBD
    //! @param[in]      pEmitter            TBD
    //! @param[in]      pParticleProperty   TBD
    //! @param[in]      particleIndex       TBD
    //! @param[in]      time                TBD
    //------------------------------------------------------------------------------
    void CalculateParticleBehaviorFieldPosAdd( nn::util::Vector3fType* pOutPos, Emitter* pEmitter, const ParticleProperty* pParticleProperty, int particleIndex, float time ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          フィールド 収束 の挙動計算をします。
    //! @param[in]      pEmitter            TBD
    //! @param[in]      pOutPos             TBD
    //! @param[in]      pParticleProperty   TBD
    //! @param[in]      particleIndex       TBD
    //! @param[in]      time                TBD
    //------------------------------------------------------------------------------
    void CalculateParticleBehaviorFieldConvergence( nn::util::Vector3fType* pOutPos, Emitter* pEmitter, const ParticleProperty* pParticleProperty, int particleIndex, float time ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief      フィールド コリジョン の挙動計算をします。
    //! @param[in,out]  pOutPos             TBD
    //! @param[in,out]  pOutVec             TBD
    //! @param[in,out]  pOutLife            TBD
    //! @param[in]      pEmitter            TBD
    //! @param[in]      pParticleProperty   TBD
    //! @param[in]      particleIndex       TBD
    //! @param[in]      pParticleData       TBD
    //! @param[in]      time                TBD
    //------------------------------------------------------------------------------
    void CalculateParticleBehaviorFieldCollision( nn::util::Vector3fType* pOutPos, nn::util::Vector3fType* pOutVec, float* pOutLife, Emitter* pEmitter, ParticleProperty* pParticleProperty, int particleIndex, ParticleData* pParticleData, float time ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          フィールド スピン の挙動計算をします。
    //! @param[in]      pEmitter            TBD
    //! @param[in]      pOutPos             TBD
    //! @param[in]      pParticleProperty   TBD
    //! @param[in]      particleIndex       TBD
    //! @param[in]      time                TBD
    //------------------------------------------------------------------------------
    void CalculateParticleBehaviorFieldSpin( nn::util::Vector3fType* pOutPos, const Emitter* pEmitter, ParticleProperty* pParticleProperty, int particleIndex, float time ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          フィールド 磁力 の挙動計算をします。
    //! @param[in]      pEmitter            TBD
    //! @param[in]      pOutPos             TBD
    //! @param[in]      pOutVec             TBD
    //! @param[in]      pParticleProperty   TBD
    //! @param[in]      particleIndex       TBD
    //! @param[in]      time                TBD
    //------------------------------------------------------------------------------
    void CalculateParticleBehaviorFieldMagnet( nn::util::Vector3fType* pOutPos, nn::util::Vector3fType* pOutVec, Emitter* pEmitter, const ParticleProperty* pParticleProperty, int particleIndex, float time ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief      フィールド ランダム の挙動計算をします。
    //! @param[in]  pOutVec             TBD
    //! @param[in]  pEmitter            TBD
    //! @param[in]  particleIndex       TBD
    //! @param[in]  time                TBD
    //------------------------------------------------------------------------------
    void CalculateParticleBehaviorFieldRandomSimple( nn::util::Vector3fType* pOutVec, Emitter* pEmitter, int particleIndex, float time ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief      フィールド GPUノイズ の挙動計算をします。
    //! @param[in]      pEmitter            TBD
    //! @param[in]      pOutPos             TBD
    //! @param[in]      pParticleProperty   TBD
    //! @param[in]      particleIndex       TBD
    //! @param[in]      time                TBD
    //------------------------------------------------------------------------------
    void CalculateParticleBehaviorFieldGpuNoise( nn::util::Vector3fType* pOutPos, Emitter* pEmitter, const ParticleProperty* pParticleProperty, int particleIndex, float time ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief     8キーアニメーションの計算
    //! @param[in] pOutValue    TBD
    //! @param[in] animData     TBD
    //! @param[in] keyCount     TBD
    //! @param[in] random       TBD
    //! @param[in] time         TBD
    //! @param[in] loopRate     TBD
    //! @param[in] isLoopRandom TBD
    //! @param[in] life         TBD
    //------------------------------------------------------------------------------
    void Calculate8KeyAnim(
        nn::util::Float3*                           pOutValue,
        const nn::vfx::detail::ResAnim8KeyParam&    animData,
        int                                         keyCount,
        float                                       random,
        float                                       time,
        float                                       loopRate,
        float                                       isLoopRandom,
        float                                       life ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief      フィールド 8キーアニメーションの計算
    //! @param[in]  pOutValue       TBD
    //! @param[in]  pEmitter        TBD
    //! @param[in]  animData        TBD
    //! @param[in]  particleIndex   TBD
    //! @param[in]  time            TBD
    //------------------------------------------------------------------------------
    void CalculateField8KeyAnim(
        nn::util::Float3*                           pOutValue,
        const Emitter*                              pEmitter,
        const nn::vfx::detail::ResAnim8KeyParamSet& animData,
        int                                         particleIndex,
        float                                       time ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief      平方根を計算
    //! @param[in]  v           入力
    //! @return     平方根の値
    //------------------------------------------------------------------------------
    static inline float SafeSqrt( float v ) NN_NOEXCEPT
    {
        if( v <= 0.0f )
        {
            return 0.0f;
        }
        return ::std::sqrtf( v );
    }

    //------------------------------------------------------------------------------
    //! @brief      EmitterLocalVec の更新処理
    //! @param[out] pOutEmitterLocalVec     更新するEmitterLocalVecへのポインタ
    //! @param[in]  pEmitter                エミッタへのポインタ
    //------------------------------------------------------------------------------
    void UpdateEmitterLocalVec( nn::util::Vector3fType* const pOutEmitterLocalVec, Emitter* const pEmitter ) const NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief      エミッタアニメーション行列の計算
    //! @param[in]  pEmitter        TBD
    //! @param[in]  pAnimMatrixSrt  TBD
    //! @param[in]  pAnimMatrixRt   TBD
    //------------------------------------------------------------------------------
    void ApplyEmitterAnimation(
        Emitter*  pEmitter,
        nn::util::Matrix4x3fType* pAnimMatrixSrt,
        nn::util::Matrix4x3fType* pAnimMatrixRt ) NN_NOEXCEPT;


private:
    static EmitFunction         g_EmitFunctions[];                      //!< エミッタ形状処理 関数テーブル
    System* const               m_pSystem;                              //!< エフェクトシステム
    TextureSampler*             m_pTextureSamplerForCurlNoise;          //!< フレームバッファ/デプス テクスチャ用のテクスチャサンプラオブジェクト
    TextureSampler*             m_pTextureSamplerForColorAndDepth;      //!< フレームバッファ/デプス テクスチャ用のテクスチャサンプラオブジェクト
    detail::ResTextureSampler   m_TextureSamplerDataForCurlNoise;       //!< フレームバッファ/デプス テクスチャ用のテクスチャサンプラリソース
    detail::ResTextureSampler   m_TextureSamplerDataForColorAndDepth;   //!< フレームバッファ/デプス テクスチャ用のテクスチャサンプラリソース

    enum
    {
        _COMPUTE_SHADER_DISPATCH_COUNT  = 32
    };
};

} // namespace detail
} // namespace vfx
} // namespace nn
