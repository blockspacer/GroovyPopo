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

namespace nn {
namespace vfx {

class System;
struct DrawParameterArg;
struct EmitterSetInitializeArg;
struct EmitterSetFinalizeArg;
struct EmitterCalculateLodArg;
struct EmitterDrawCullArg;
struct DrawEmitterProfilerArg;
typedef void ( *EmitterSetInitializeCallback )( EmitterSetInitializeArg& arg );
typedef void ( *EmitterSetFinalizeCallback )( EmitterSetFinalizeArg& arg );
typedef EmitterCalculationResult ( *EmitterCalculateLodCallback )( EmitterCalculateLodArg& arg );
typedef bool ( *EmitterDrawCullingCallback )( EmitterDrawCullArg& arg );
typedef void ( *DrawEmitterProfileCallback )( DrawEmitterProfilerArg& arg );



//---------------------------------------------------------------------------
//! @brief エミッタセット実行中の統計情報を保持する構造体
//---------------------------------------------------------------------------
struct ProcessingInfo
{
    int     emitterSetCount;                    //!< 処理されたエミッタセット数
    int     emitterCount;                       //!< 処理されたエミッタ数
    int     emitterCountSkippedCalculation;     //!< 計算処理をスキップしたエミッタ数
    int     emitterAnimCount;                   //!< 処理されたエミッタアニメーション数
    int     cpuParticleCount;                   //!< 処理されたパーティクル数
    int     gpuParticleCount;                   //!< 処理されたパーティクル数
    int     gpusoParticleCount;                 //!< 処理されたパーティクル数
    int     cpuEmitterCount;                    //!< 処理されたCpuエミッタ数
    int     gpuEmitterCount;                    //!< 処理されたGpuエミッタ数
    int     computeEmitterCount;                //!< 処理されたComputeエミッタ数
    int     stripeCount;                        //!< 処理された 履歴式ストライプ の本数
    int     superStripeCount;                   //!< 処理された 履歴式ストライプ2 の本数
    int     connectionStripeCount;              //!< 処理された 連結式ストライプ の本数
    size_t  allocatedDynamicHeapSize;           //!< 確保された動的ヒープサイズ
    size_t  allocatedGpuBufferSize;             //!< 確保されたGpuバッファサイズ


    //---------------------------------------------------------------------------
    //! @brief  値をクリアする
    //---------------------------------------------------------------------------
    void Clear() NN_NOEXCEPT
    {
        emitterSetCount = 0;
        emitterCount = 0;
        emitterCountSkippedCalculation = 0;
        emitterAnimCount = 0;
        cpuParticleCount = 0;
        gpuParticleCount = 0;
        gpusoParticleCount = 0;
        cpuEmitterCount = 0;
        gpuEmitterCount = 0;
        computeEmitterCount = 0;
        stripeCount = 0;
        superStripeCount = 0;
        connectionStripeCount = 0;
        allocatedDynamicHeapSize = 0;
        allocatedGpuBufferSize = 0;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate       パーティクル処理数／消費メモリ量などの情報更新
    //! @param[in] pEmitter エミッタへのポインタ
    //---------------------------------------------------------------------------
    void UpdateProcessingInfo( const Emitter* pEmitter ) NN_NOEXCEPT;
};

//------------------------------------------------------------------------------
//! @brief  エミッタセット
//!
//!         プログラマはエミッタセットを介してエフェクト再生操作を行います。
//------------------------------------------------------------------------------
class EmitterSet
{
    NN_DISALLOW_COPY( EmitterSet );
public:
    //----------------------------------------
    //! @name マトリクス操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief        マトリクスを設定します。
    //!
    //!               設定するマトリクス情報は、計算処理内でエミッタに対して伝播されます。
    //! @param[in]    matrixSrt 設定するマトリクス
    //---------------------------------------------------------------------------
    void SetMatrix( const nn::util::Matrix4x3fType& matrixSrt ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        マトリクスを高速に設定します。
    //!
    //!               スケール込みのマトリクスを渡した場合表示が壊れます。
    //!               スケール込のマトリクスを扱うときは SetMatrix を利用してください。
    //! @param[in]    matrixRt     エミッタ行列
    //---------------------------------------------------------------------------
    void SetMatrixFast( const nn::util::Matrix4x3fType& matrixRt ) NN_NOEXCEPT
    {
        m_MatrixSrt = m_MatrixRt = matrixRt;
        nn::util::VectorSet( &m_AutoCalculatedScale, 1.f, 1.f, 1.f );
        UpdateParticleScale();
        m_IsEmitterSrtDirty = 1;
    }

    //---------------------------------------------------------------------------
    //! @brief        マトリクスを高速に設定します。
    //!
    //!               この関数はスケール抜きのRT行列と、それとは別にスケールを指定できます。
    //!               スケール込みの行列を渡した場合表示が壊れます。
    //! @param[in]    matrixRt RTマトリクス
    //! @param[in]    scale    スケール
    //---------------------------------------------------------------------------
    void SetMatrixFast( const nn::util::Matrix4x3fType& matrixRt, float scale ) NN_NOEXCEPT
    {
        nn::util::Vector3fType translate;
        nn::util::MatrixGetAxisW( &translate, matrixRt );

        nn::util::Matrix4x3fType m;
        nn::util::MatrixMultiply( &m, matrixRt, scale );
        nn::util::MatrixSetAxisW( &m, translate );

        m_MatrixSrt = m;
        m_MatrixRt = matrixRt;
        nn::util::VectorSet( &m_AutoCalculatedScale, scale, scale, scale );

        UpdateParticleScale();
        m_IsEmitterSrtDirty = 1;
    }

    //---------------------------------------------------------------------------
    //! @brief        マトリクスを高速に設定します。
    //!
    //!               この関数はスケール抜きのRT行列と、それとは別にスケールを指定できます。
    //!               スケール込みの行列を渡した場合表示が壊れます。
    //! @param[in]    matrixRt RTマトリクス
    //! @param[in]    scale    スケール
    //---------------------------------------------------------------------------
    void SetMatrixFast( const nn::util::Matrix4x3fType& matrixRt, const nn::util::Vector3fType& scale ) NN_NOEXCEPT
    {
        nn::util::Vector3fType translate, ax, ay, az;
        nn::util::MatrixGetAxisX( &ax, matrixRt );
        nn::util::MatrixGetAxisY( &ay, matrixRt );
        nn::util::MatrixGetAxisZ( &az, matrixRt );
        nn::util::MatrixGetAxisW( &translate, matrixRt );

        nn::util::Float3 s;
        nn::util::VectorStore( &s, scale );

        nn::util::VectorMultiply( &ax, ax, s.x );
        nn::util::VectorMultiply( &ay, ay, s.y );
        nn::util::VectorMultiply( &az, az, s.z );

        nn::util::Matrix4x3fType m;
        nn::util::MatrixSetAxisX( &m, ax );
        nn::util::MatrixSetAxisY( &m, ay );
        nn::util::MatrixSetAxisZ( &m, az );
        nn::util::MatrixSetAxisW( &m, translate );

        m_MatrixSrt = m;
        m_MatrixRt = matrixRt;
        m_AutoCalculatedScale = scale;

        UpdateParticleScale();
        m_IsEmitterSrtDirty = 1;
    }

    //---------------------------------------------------------------------------
    //! @brief        位置を指定してマトリクスを設定します。
    //!
    //! @param[in] pos 座標
    //---------------------------------------------------------------------------
    void SetPos( const nn::util::Vector3fType& pos ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        位置とスケールを指定してマトリクスを設定します。
    //!
    //! @param[in]    pos     座標
    //! @param[in]    scale   スケール
    //---------------------------------------------------------------------------
    void SetPos( const nn::util::Vector3fType& pos, float scale ) NN_NOEXCEPT
    {
        nn::util::MatrixIdentity( &m_MatrixRt );
        nn::util::MatrixSetAxisW( &m_MatrixRt, pos );
        m_MatrixSrt = m_MatrixRt;

        nn::util::Matrix4x3fType matScale;
        nn::util::Vector3fType vScale = NN_UTIL_VECTOR_3F_INITIALIZER( scale, scale, scale );
        nn::util::MatrixSetScale( &matScale, vScale );
        nn::util::MatrixMultiply( &m_MatrixSrt, m_MatrixSrt, matScale );

        nn::util::VectorSet( &m_AutoCalculatedScale, scale, scale, scale );
        UpdateParticleScale();
        m_IsEmitterSrtDirty = 1;
    }

    //---------------------------------------------------------------------------
    //! @brief        位置とスケールを指定してマトリクスを設定します。
    //!
    //! @param[in]    pos     座標
    //! @param[in]    scale   スケール
    //---------------------------------------------------------------------------
    void SetPos( const nn::util::Vector3fType& pos, const nn::util::Vector3fType& scale ) NN_NOEXCEPT
    {
        nn::util::MatrixIdentity( &m_MatrixRt );
        nn::util::MatrixSetAxisW( &m_MatrixRt, pos );
        m_MatrixSrt = m_MatrixRt;

        nn::util::Matrix4x3fType matScale;
        nn::util::MatrixSetScale( &matScale, scale );
        nn::util::MatrixMultiply( &m_MatrixSrt, m_MatrixSrt, matScale );

        m_AutoCalculatedScale = scale;
        UpdateParticleScale();
        m_IsEmitterSrtDirty = 1;
    }

    //---------------------------------------------------------------------------
    //! @brief        SRTマトリクスを取得します。
    //!
    //! @return       SRTエミッタセットマトリクス
    //---------------------------------------------------------------------------
    const nn::util::Matrix4x3fType& GetMatrixSrt() const NN_NOEXCEPT
    {
        return m_MatrixSrt;
    }

    //---------------------------------------------------------------------------
    //! @brief        RTマトリクスを取得します。
    //!
    //! @return       RTエミッタセットマトリクス
    //---------------------------------------------------------------------------
    const nn::util::Matrix4x3fType& GetMatrixRt() const NN_NOEXCEPT
    {
        return m_MatrixRt;
    }

    //---------------------------------------------------------------------------
    //! @brief          位置を取得します。
    //!
    //! @param[out]     pOutPos 座標
    //---------------------------------------------------------------------------
    void GetPos( nn::util::Vector3fType* pOutPos ) const NN_NOEXCEPT
    {
        nn::util::MatrixGetAxisW( pOutPos, m_MatrixSrt );
    }

    //@}


    //----------------------------------------
    //! @name エミッタ形状操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief        エミッタ形状スケール値を設定します。
    //!
    //! @param[in] scale スケール
    //---------------------------------------------------------------------------
    void SetEmitterVolumeScale( const nn::util::Vector3fType& scale ) NN_NOEXCEPT
    {
        m_EmitterVolumeScale = scale;
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタ形状スケール値を取得します。
    //!
    //! @return       スケール
    //---------------------------------------------------------------------------
    const nn::util::Vector3fType& GetEmitterVolumeScale() const NN_NOEXCEPT
    {
        return m_EmitterVolumeScale;
    }

    //@}


    //----------------------------------------
    //! @name パーティクル放出量制御
    //@{

    //---------------------------------------------------------------------------
    //! @brief        パーティクル放出レートのスケール値を設定します。
    //!
    //! @param[in]    ratio     スケール値。1.0より大きい値が指定できません。
    //---------------------------------------------------------------------------
    void SetEmissionRatioScale( float ratio ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        距離放出のエミッタの放出処理の有効／無効を設定します。
    //!
    //! @param[in]    flag     距離放出を有効にする場合は true 、無効にする場合は false を指定してください。
    //---------------------------------------------------------------------------
    void SetDistanceBasedEmissionEnabled( bool flag ) NN_NOEXCEPT
    {
        m_EnableDistanceBasedEmission = flag;
    }

    //---------------------------------------------------------------------------
    //! @brief        パーティクル放出レートのスケール値を取得します。
    //!
    //! @return       放出間隔のスケール値
    //---------------------------------------------------------------------------
    float GetEmissionRatioScale() const NN_NOEXCEPT
    {
        return m_EmissionRatioScale;
    }

    //---------------------------------------------------------------------------
    //! @brief        パーティクル放出間隔のスケール値を設定します。
    //!
    //! @param[in]    ratio     スケール値。1.0未満の値が指定できません。
    //---------------------------------------------------------------------------
    void SetEmissionIntervalScale( float ratio ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        パーティクル放出間隔のスケール値を取得します。
    //!
    //! @return       放出間隔のスケール値
    //---------------------------------------------------------------------------
    float GetEmissionIntervalScale() const NN_NOEXCEPT
    {
        return m_EmissionIntervalScale;
    }

    //@}


    //----------------------------------------
    //! @name パーティクル寿命制御
    //@{

    //---------------------------------------------------------------------------
    //! @brief        パーティクル寿命のスケール値を設定します。
    //!
    //! @param[in]    ratio スケール値。1.0より大きい値が指定できません。
    //---------------------------------------------------------------------------
    void SetParticleLifeScale( float ratio ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        パーティクル寿命のスケール値を取得します。
    //!
    //! @return       寿命のスケール値。
    //---------------------------------------------------------------------------
    float GetParticleLifeScale() const NN_NOEXCEPT
    {
        return m_ParticleLifeScale;
    }

    //@}


    //----------------------------------------
    //! @name パーティクルスケール制御
    //@{

    //---------------------------------------------------------------------------
    //! @brief      放出時のパーティクルの大きさを設定します。
    //!
    //!             XY スケールをまとめて設定します。Z スケール値は設定されません。
    //!             プリミティブパーティクルを使用し Z スケール値の設定が必要な場合は、
    //!             SetEmissionParticleScale( const nn::util::Vector3fType& scale ) を使用してください。
    //! @param[in]  scale   XY スケール値。
    //---------------------------------------------------------------------------
    void SetEmissionParticleScale( float scale ) NN_NOEXCEPT
    {
        nn::util::VectorSetX( &m_ParticleEmissionScale, scale );
        nn::util::VectorSetY( &m_ParticleEmissionScale, scale );
    }

    //---------------------------------------------------------------------------
    //! @brief      放出時のパーティクルの大きさを設定します。
    //!
    //!             XY スケールを個別に設定します。Z スケール値は設定されません。
    //!             プリミティブパーティクルを使用し Z スケール値の設定が必要な場合は、
    //!             SetEmissionParticleScale( const nn::util::Vector3fType& scale ) を使用してください。
    //! @param[in]  scaleX  X スケール値。
    //! @param[in]  scaleY  Y スケール値。
    //---------------------------------------------------------------------------
    void SetEmissionParticleScale( float scaleX, float scaleY ) NN_NOEXCEPT
    {
        nn::util::VectorSetX( &m_ParticleEmissionScale, scaleX );
        nn::util::VectorSetY( &m_ParticleEmissionScale, scaleY );
    }

    //---------------------------------------------------------------------------
    //! @brief      パーティクル放出時のスケール値を設定します。
    //!
    //!             XY スケールを個別に設定します。Z スケール値は設定されません。
    //!             プリミティブパーティクルを使用し Z スケール値の設定が必要な場合は、
    //!             SetEmissionParticleScale( const nn::util::Vector3fType& scale ) を使用してください。
    //! @param[in]  scale   XY スケール値。
    //---------------------------------------------------------------------------
    void SetEmissionParticleScale( const nn::util::Float2& scale ) NN_NOEXCEPT
    {
        SetEmissionParticleScale( scale.x, scale.y );
    }

    //---------------------------------------------------------------------------
    //! @brief      パーティクル放出時のスケール値を設定します。
    //!
    //!             XYZ スケールを個別に設定します。
    //!             通常のパーティクルは板ポリゴンで奥行きを持たないので、Z スケールは無視されます。
    //!             Z スケール値の設定が不要な場合は、XY だけを設定するオーバーロード版の API も使用可能です。
    //! @param[in]  scale   XYZ スケール値。
    //---------------------------------------------------------------------------
    void SetEmissionParticleScale( const nn::util::Vector3fType& scale ) NN_NOEXCEPT
    {
        m_ParticleEmissionScale = scale;
    }

    //---------------------------------------------------------------------------
    //! @brief      放出時のパーティクルの大きさを取得します。
    //!
    //! @return     放出時のパーティクルのスケール値
    //---------------------------------------------------------------------------
    const nn::util::Vector3fType& GetEmissionParticleScale() const NN_NOEXCEPT
    {
        return m_ParticleEmissionScale;
    }


    //---------------------------------------------------------------------------
    //! @brief      パーティクルの大きさを設定します。
    //!
    //!             XYZ スケールを個別に設定します。
    //!             通常のパーティクルは板ポリゴンで奥行きを持たないので、Z スケールは無視されます。
    //!             Z スケール値の設定が不要な場合は、XY だけを設定するオーバーロード版の API も使用可能です。
    //! @param[in]  scale   XYZ スケール値
    //---------------------------------------------------------------------------
    void SetParticleScale( float scale ) NN_NOEXCEPT
    {
        SetParticleScale( scale, scale, scale );
        UpdateParticleScale();
    }

    //---------------------------------------------------------------------------
    //! @brief      パーティクルの大きさを設定します。
    //!
    //!             XY スケールを個別に設定します。Z スケール値は設定されません。
    //!             プリミティブパーティクルを使用し Z スケール値の設定が必要な場合は、
    //!             SetParticleScale( const nn::util::Vector3fType& scale ) などのオーバーロード版 API を使用してください。
    //! @param[in]  scaleX  Xスケール値
    //! @param[in]  scaleY  Yスケール値
    //---------------------------------------------------------------------------
    void SetParticleScale( float scaleX, float scaleY ) NN_NOEXCEPT
    {
        nn::util::VectorSetX( &m_ParticleScale, scaleX );
        nn::util::VectorSetY( &m_ParticleScale, scaleY );
        UpdateParticleScale();
    }

    //---------------------------------------------------------------------------
    //! @brief      パーティクルの大きさを設定します。
    //!
    //!             XY スケールを個別に設定します。Z スケール値は設定されません。
    //!             プリミティブパーティクルを使用し Z スケール値の設定が必要な場合は、
    //!             SetParticleScale( const nn::util::Vector3fType& scale ) などのオーバーロード版 API を使用してください。
    //! @param[in]  scale   XYスケール値。
    //---------------------------------------------------------------------------
    void SetParticleScale( const nn::util::Float2& scale ) NN_NOEXCEPT
    {
        SetParticleScale( scale.x, scale.y );
        UpdateParticleScale();
    }

    //---------------------------------------------------------------------------
    //! @brief      パーティクルの大きさを設定します。
    //!
    //!             XYZ スケールを個別に設定します。
    //!             通常のパーティクルは板ポリゴンで奥行きを持たないので、Z スケールは無視されます。
    //!             Z スケール値の設定が不要な場合は、XY だけを設定するオーバーロード版の API も使用可能です。
    //! @param[in]  scaleX  Xスケール値
    //! @param[in]  scaleY  Yスケール値
    //! @param[in]  scaleZ  Zスケール値
    //---------------------------------------------------------------------------
    void SetParticleScale( float scaleX, float scaleY, float scaleZ ) NN_NOEXCEPT
    {
        nn::util::VectorSetX( &m_ParticleScale, scaleX );
        nn::util::VectorSetY( &m_ParticleScale, scaleY );
        nn::util::VectorSetZ( &m_ParticleScale, scaleZ );
        UpdateParticleScale();
    }

    //---------------------------------------------------------------------------
    //! @brief      パーティクルの大きさを設定します。
    //!
    //!             XYZ スケールを個別に設定します。
    //!             通常のパーティクルは板ポリゴンで奥行きを持たないので、Z スケールは無視されます。
    //!             Z スケール値の設定が不要な場合は、XY だけを設定するオーバーロード版の API も使用可能です。
    //! @param[in]  scale   XYZスケール値。
    //---------------------------------------------------------------------------
    void SetParticleScale( const nn::util::Vector3fType& scale ) NN_NOEXCEPT
    {
        m_ParticleScale = scale;
        UpdateParticleScale();
    }

    //---------------------------------------------------------------------------
    //! @brief      パーティクルの大きさを取得します。
    //!
    //! @return     パーティクルスケール値
    //---------------------------------------------------------------------------
    const nn::util::Vector3fType& GetParticleScale() const NN_NOEXCEPT
    {
        return m_ParticleScale;
    }

    //---------------------------------------------------------------------------
    //! @brief      パーティクルの大きさを取得します。
    //!
    //! @return     パーティクルスケール値
    //---------------------------------------------------------------------------
    const nn::util::Vector3fType& GetParticleScaleForCalculation() const NN_NOEXCEPT
    {
        return m_ParticleScaleForCalculation;
    }

    //@}


    //----------------------------------------
    //! @name パーティクル初速制御
    //@{

    //---------------------------------------------------------------------------
    //! @brief        全方向速度のスケール値を設定します。
    //!
    //! @param[in] velScale 全方向速度スケール値
    //---------------------------------------------------------------------------
    void SetAllDirectionalVel( float velScale ) NN_NOEXCEPT
    {
        m_FigureVel = velScale;
    }

    //---------------------------------------------------------------------------
    //! @brief        全方向速度のスケール値を取得します。
    //!
    //! @return 速度スケール値
    //---------------------------------------------------------------------------
    float GetAllDirectionalVel() const NN_NOEXCEPT
    {
        return m_FigureVel;
    }

    //---------------------------------------------------------------------------
    //! @brief        初期速度ランダムのスケール値を設定します。
    //!
    //! @param[in] velScale 初期速度ランダムスケール値
    //---------------------------------------------------------------------------
    void SetRandomVel( float velScale ) NN_NOEXCEPT
    {
        m_RandomVel = velScale;
    }

    //---------------------------------------------------------------------------
    //! @brief        初期速度ランダムのスケール値を取得します。
    //!
    //! @return 初期速度ランダムスケール値
    //---------------------------------------------------------------------------
    float GetRandomVel() const NN_NOEXCEPT
    {
        return m_RandomVel;
    }

    //---------------------------------------------------------------------------
    //! @brief        最終的な速度に加算する値（ワールド座標系）を設定します。
    //!
    //! @param[in] velAdd 加算する速度
    //---------------------------------------------------------------------------
    void SetAddVel( const nn::util::Vector3fType& velAdd ) NN_NOEXCEPT
    {
        m_AdditionalVel = velAdd;
    }

    //---------------------------------------------------------------------------
    //! @brief        最終的な速度に加算する値（ワールド座標系）を取得します。
    //!
    //! @return     加算する速度
    //---------------------------------------------------------------------------
    const nn::util::Vector3fType& GetVelAdd() const NN_NOEXCEPT
    {
        return m_AdditionalVel;
    }

    //---------------------------------------------------------------------------
    //! @brief        指定方向速度のスケール値を設定します。
    //!
    //! @param[in] velScale 指定方向速度のスケール値
    //---------------------------------------------------------------------------
    void SetDirectionalVel( float velScale ) NN_NOEXCEPT
    {
        m_DirectionalVel = velScale;
    }

    //---------------------------------------------------------------------------
    //! @brief        指定方向速度のスケール値を取得します。
    //!
    //! @return 指定方向速度のスケール値
    //---------------------------------------------------------------------------
    float GetDirectionalVel() const NN_NOEXCEPT
    {
        return m_DirectionalVel;
    }

    //---------------------------------------------------------------------------
    //! @brief        指定方向の方向を設定します。
    //!
    //! @param[in] dir 指定方向の方向
    //---------------------------------------------------------------------------
    void SetDirectional( const nn::util::Vector3fType& dir ) NN_NOEXCEPT
    {
        m_Directional = dir;
        m_IsSetDirectional = true;
    }

    //---------------------------------------------------------------------------
    //! @brief        指定方向の方向を取得します。
    //!
    //! @return 指定方向の方向
    //---------------------------------------------------------------------------
    const nn::util::Vector3fType& GetDirectional() const NN_NOEXCEPT
    {
        return m_Directional;
    }

    //---------------------------------------------------------------------------
    //! @brief        指定方向の方向設定を解除します。
    //---------------------------------------------------------------------------
    void DisableDirectional() NN_NOEXCEPT
    {
        m_IsSetDirectional = false;
    }

    //---------------------------------------------------------------------------
    //! @brief        指定方向の方向を設定しているか？
    //! @return       true で設定済み、false で未設定
    //---------------------------------------------------------------------------
    bool IsSetDirectional() const NN_NOEXCEPT
    {
        return ( m_IsSetDirectional != 0 );
    }

    //@}


    //----------------------------------------
    //! @name フレーム操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief   呼び出し開始フレームを設定します。
    //! @details まったく同じ内容のループエミッタセットを放出する時は、
    //!          この関数でエミッタ放出開始をずらすと、ワーストケースが減る可能性があります。
    //! @param[in] frame フレーム数
    //---------------------------------------------------------------------------
    void SetStartFrame( int frame ) NN_NOEXCEPT
    {
        m_StartFrame = frame;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate   呼び出し開始フレーム値を取得します。
    //!
    //! @return         呼び出し開始フレーム値
    //---------------------------------------------------------------------------
    int GetStartFrame() const NN_NOEXCEPT
    {
        return m_StartFrame;
    }

    //@}


    //----------------------------------------
    //! @name パーティクル マニュアル放出制御
    //@{

    //---------------------------------------------------------------------------
    //! @brief        パーティクルを手動で放出するモードに設定します。
    //! @deprecated   エミッタセット生成後に手動放出モードへの切り替えを非サポートにしますので、
    //!               CreateManualEmitterSetId にて生成時に手動放出モードのエミッタセットを生成するようお願いします。
    //---------------------------------------------------------------------------
    NN_DEPRECATED void SetManualParticleEmission() NN_NOEXCEPT
    {
        m_IsManualEmission = true;
    }

    //---------------------------------------------------------------------------
    //! @brief        個数を指定してパーティクルを手動で放出するモードに設定します。
    //! @deprecated   エミッタセット生成後に手動放出モードへの切り替えを非サポートにしますので、
    //!               CreateManualEmitterSetId にて生成時に手動放出モードのエミッタセットを生成するようお願いします。
    //! @param[in]    particleCount      予約するパーティクルの個数。
    //---------------------------------------------------------------------------
    NN_DEPRECATED void SetManualParticleEmissionWithParticleCount( int particleCount ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        パーティクルを手動で放出するモードに設定します。
    //! @deprecated   エミッタセット生成後に手動放出モードへの切り替えを非サポートにしますので、
    //!               CreateManualEmitterSetId にて生成時に手動放出モードのエミッタセットを生成するようお願いします。
    //! @param[in] emissionEnable trueで切り替える
    //---------------------------------------------------------------------------
    NN_DEPRECATED void SetManualParticleEmission( bool emissionEnable ) NN_NOEXCEPT
    {
        m_IsManualEmission = emissionEnable;
    }
    //---------------------------------------------------------------------------
    //! @brief      パーティクル手動放出モードか取得します。
    //!
    //! @return     trueで手動放出モード、falseで手動放出モード未設定
    //---------------------------------------------------------------------------
    bool IsManualParticleEmission() const NN_NOEXCEPT
    {
        return m_IsManualEmission ? true : false;
    }

    //---------------------------------------------------------------------------
    //! @brief      常駐型のマニュアルエミッタセットかどうかを取得します。
    //!
    //! @return     常駐型の場合は true 非常駐型の場合は false
    //---------------------------------------------------------------------------
    bool IsResidentManualEmitterSet() const NN_NOEXCEPT
    {
        return ( m_IsManualEmission ) && ( m_ResidentEmitterTime < 0 );
    }

    //---------------------------------------------------------------------------
    //! @brief      マニュアルエミッタセットの待機エミッタ時間を取得します。
    //!
    //! @return     粒の数が0でも消滅せずに待機する待ち時間。常駐型・マニュアルエミッタセットでないの場合は -1 が返ります。
    //---------------------------------------------------------------------------
    int GetResidentEmitterTime() const NN_NOEXCEPT
    {
        if ( !IsManualParticleEmission() || IsResidentManualEmitterSet() )
        {
            return -1;
        }
        return m_ResidentEmitterTime;
    }

    //---------------------------------------------------------------------------
    //! @brief      放出位置を指定してパーティクルを手動で放出します。
    //!
    //! @details    第二引数として、ユーザーデータのポインタを与えることができます。
    //!             パーティクルの生成処理が実行されるまで、ユーザーデータのポインタの指す内容が破棄されないように注意してください。
    //!             1フレーム中に 2つ以上のパーティクルをマニュアル放出し、かつパーティクルごとに異なるユーザーデータを持たせたい場合は、
    //!             「1フレーム中に放出する最大数（ nn::vfx::System::CreateManualEmitterSetId() の引数として与えています ）」分のユーザーデータを用意して、
    //!             それぞれのアドレスを EmitParticle() に渡す必要があります。
    //!
    //! @param[in]  pos         放出位置
    //! @param[in]  pUserData   ユーザーデータへのポインタ（任意）
    //---------------------------------------------------------------------------
    void EmitParticle( const nn::util::Vector3fType& pos, void* pUserData = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      放出時マトリクスを指定してパーティクルを手動で放出します。
    //!
    //! @details    第二引数として、ユーザーデータのポインタを与えることができます。
    //!             パーティクルの生成処理が実行されるまで、ユーザーデータのポインタの指す内容が破棄されないように注意してください。
    //!             1フレーム中に 2つ以上のパーティクルをマニュアル放出し、かつパーティクルごとに異なるユーザーデータを持たせたい場合は、
    //!             「1フレーム中に放出する最大数（ nn::vfx::System::CreateManualEmitterSetId() の引数として与えています ）」分のユーザーデータを用意して、
    //!             それぞれのアドレスを EmitParticle() に渡す必要があります。
    //!             指定する放出マトリクスのスケール成分は、パーティクルの大きさではなく、エミッタの大きさに作用しますのでご注意ください。
    //!             一般的な用途としては、スケール成分を含まない回転＋平行移動成分を含めた行列を与えてください。
    //!             また、利用するエミッタは　パーティクル追従タイプを「エミッタに追従しない」に設定したエフェクトデータを利用する必要があります。
    //!
    //! @param[in]  emitMatrix  放出時マトリクス
    //! @param[in]  pUserData   ユーザーデータへのポインタ（任意）
    //---------------------------------------------------------------------------
    void EmitParticle( const nn::util::Matrix4x3fType& emitMatrix, void* pUserData ) NN_NOEXCEPT;


    //---------------------------------------------------------------------------
    //! @brief      マニュアル放出の現在の放出予約数を取得します。
    //!
    //! @details    この値は nn::vfx::EmitterSet::EmitParticle() で放出予約され、
    //!             まだ放出が行われていないパーティクルの数を返します。
    //!             次の計算処理で予約された放出処理が実行されると 0 に戻ります。
    //!
    //! @return     マニュアル放出の現在の放出予約数
    //---------------------------------------------------------------------------
    int GetManualEmissionCount() const NN_NOEXCEPT
    {
        return m_ManualEmissionCount;
    }

    //@}


    //----------------------------------------
    //! @name パーティクル回転制御
    //@{

    //---------------------------------------------------------------------------
    //! @brief        初期角度の設定をします。
    //!
    //! @param[in] rotate 回転角度( ラジアン指定 )
    //---------------------------------------------------------------------------
    void SetInitRotate( const nn::util::Vector3fType& rotate ) NN_NOEXCEPT
    {
        m_InitialRoate = rotate;
    }

    //@}


    //----------------------------------------
    //! @name エミッタセットカラー制御
    //@{

    //---------------------------------------------------------------------------
    //! @brief        カラー A値(乗算値)を設定します。
    //!
    //! @param[in] alpha A値
    //---------------------------------------------------------------------------
    void SetAlpha( float alpha ) NN_NOEXCEPT
    {
        nn::util::VectorSetW( &m_Color, alpha );
    }

    //---------------------------------------------------------------------------
    //! @brief        カラー RGB値(乗算値)を設定します。
    //!
    //! @param[in] r R値
    //! @param[in] g G値
    //! @param[in] b B値
    //---------------------------------------------------------------------------
    void SetColor( float r, float g, float b ) NN_NOEXCEPT
    {
        nn::util::VectorSetX( &m_Color, r );
        nn::util::VectorSetY( &m_Color, g );
        nn::util::VectorSetZ( &m_Color, b );
    }

    //---------------------------------------------------------------------------
    //! @brief        カラー RGBA値(乗算値)を設定します。
    //!
    //! @param[in] color RGBAカラー値
    //---------------------------------------------------------------------------
    void SetColor( const nn::util::Vector4fType& color ) NN_NOEXCEPT
    {
        m_Color = color;
    }

    //---------------------------------------------------------------------------
    //! @brief      カラー RGBA値(乗算値)を取得します。
    //! @return     RGBAカラー値
    //---------------------------------------------------------------------------
    const nn::util::Vector4fType& GetColor() const NN_NOEXCEPT
    {
        return m_Color;
    }

    //@}


    //----------------------------------------
    //! @name エミッタカラー制御
    //@{

    //---------------------------------------------------------------------------
    //! @brief     カラー RGBA値(乗算値)を設定します。
    //! @param[in] color0 カラー0値
    //! @param[in] color1 カラー1値
    //---------------------------------------------------------------------------
    void SetEmitterColor( const nn::util::Vector4fType& color0, const nn::util::Vector4fType& color1 ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     カラー0 RGBA値(乗算値)を設定します。
    //! @param[in] color0 カラー0値
    //---------------------------------------------------------------------------
    void SetEmitterColor0( const nn::util::Vector4fType& color0 ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     カラー1 RGBA値(乗算値)を設定します。
    //! @param[in] color1 カラー1値
    //---------------------------------------------------------------------------
    void SetEmitterColor1( const nn::util::Vector4fType& color1 ) NN_NOEXCEPT;

    //@}


    //----------------------------------------
    //! @name 表示 / 非表示 操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief        エミッタセット描画処理の有効/無効を設定します。
    //!
    //! @param[in]    flag       有効/無効。
    //---------------------------------------------------------------------------
    void SetVisible( bool flag ) NN_NOEXCEPT
    {
        m_IsDraw = flag;
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタセットの描画処理を停止します。
    //!
    //! @param[in]    isStopDraw       描画処理を止めるか？。
    //---------------------------------------------------------------------------
    void SetStopDraw( bool isStopDraw ) NN_NOEXCEPT
    {
        return SetVisible( !isStopDraw );
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタセット描画処理の有効/無効を取得します。
    //!
    //! @return 有効/無効フラグ
    //---------------------------------------------------------------------------
    bool IsVisible() const NN_NOEXCEPT
    {
        return ( m_IsDraw != 0 );
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタセット描画処理が停止状態かどうかチェックします。
    //! @return     停止状態であればtrue、そうでなければfalse
    //---------------------------------------------------------------------------
    bool IsStopDraw() const NN_NOEXCEPT
    {
        return !IsVisible();
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタ描画処理の有効/無効を設定します。
    //!
    //! @param[in]  emitterName エミッタセット名
    //! @param[in]  flag        有効/無効。
    //---------------------------------------------------------------------------
    void SetEmitterVisible( const char* emitterName, bool flag ) NN_NOEXCEPT;

    //@}


    //----------------------------------------
    //! @name 情報の取得
    //@{

    //---------------------------------------------------------------------------
    //! @brief      エフェクトシステムの取得を行います。
    //!
    //! @return     システムクラスのインスタンス
    //---------------------------------------------------------------------------
    inline System* GetSystem() NN_NOEXCEPT
    {
        return m_pSystem;
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタセットIDを取得します。
    //!
    //! @return     エミッタセットID
    //---------------------------------------------------------------------------
    inline int GetEmitterSetId() const NN_NOEXCEPT
    {
        return m_EmitterSetId;
    };

    //---------------------------------------------------------------------------
    //! @brief      生成IDを取得します。
    //!
    //! @return     生成ID
    //---------------------------------------------------------------------------
    inline int GetCreateId() const NN_NOEXCEPT
    {
        return m_EmitterSetCreateId;
    }

    //---------------------------------------------------------------------------
    //! @brief      所属するグループIDを取得します。
    //!
    //! @return     所属するグループID
    //---------------------------------------------------------------------------
    inline int GetGroupId() const NN_NOEXCEPT
    {
        return m_GroupId;
    }

    //---------------------------------------------------------------------------
    //! @brief      リソースIDを取得します。
    //!
    //! @return     リソースID
    //---------------------------------------------------------------------------
    inline int GetResourceId() const NN_NOEXCEPT
    {
        return m_ResourceId;
    }

    //---------------------------------------------------------------------------
    //! @brief      データ上の親エミッタ数を取得します。
    //!
    //! @return     データ上の親エミッタ数。子エミッタの数を含みません。
    //---------------------------------------------------------------------------
    inline int GetEmitterCount() const NN_NOEXCEPT
    {
        return m_pEmitterSetResource->emitterCount;
    }

    //---------------------------------------------------------------------------
    //! @brief      データ上の全エミッタ数を取得します。
    //!
    //! @return     データ上の全エミッタ数。子エミッタの数も含みます。
    //---------------------------------------------------------------------------
    inline int GetEmitterAllCount() const NN_NOEXCEPT
    {
        return m_pEmitterSetResource->emitterAllCount;
    }

    //---------------------------------------------------------------------------
    //! @brief      再生中のエミッタ数を取得します。
    //! @return     再生中のエミッタ数。0であれば、再生を終了しています。
    //---------------------------------------------------------------------------
    inline int GetProcessingEmitterCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.emitterCount;
    }

    //---------------------------------------------------------------------------
    //! @brief      再生をスキップしたエミッタ数を取得します。
    //!
    //! @return     スキップしたエミッタ数。
    //---------------------------------------------------------------------------
    inline int GetEmitterCountSkippedCalculation() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.emitterCountSkippedCalculation;
    }

    //---------------------------------------------------------------------------
    //! @brief      再生中のエミッタアニメーション数を取得します。
    //!
    //! @return     再生中のエミッタアニメーション数。
    //---------------------------------------------------------------------------
    inline int GetProcessingEmitterAnimCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.emitterAnimCount;
    }

    //---------------------------------------------------------------------------
    //! @brief      再生中のCpuエミッタ数を取得します。
    //!
    //! @return     再生中のCpuエミッタ数。
    //---------------------------------------------------------------------------
    inline int GetProcessingCpuEmitterCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.cpuEmitterCount;
    }

    //---------------------------------------------------------------------------
    //! @brief      再生中のGpuエミッタ数を取得します。
    //!
    //! @return     再生中のCpuエミッタ数。
    //---------------------------------------------------------------------------
    inline int GetProcessingGpuEmitterCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.gpuEmitterCount;
    }

    //---------------------------------------------------------------------------
    //! @brief      再生中のComputeエミッタ数を取得します。
    //!
    //! @return     再生中のComputeエミッタ数。
    //---------------------------------------------------------------------------
    inline int GetProcessingComputeEmitterCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.computeEmitterCount;
    }

    //---------------------------------------------------------------------------
    //! @brief      再生中の 履歴式ストライプ の本数を取得します。
    //!
    //! @return     再生中の 履歴式ストライプ の本数。
    //---------------------------------------------------------------------------
    inline int GetProcessingStripeCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.stripeCount;
    }

    //---------------------------------------------------------------------------
    //! @brief      再生中の 履歴式ストライプ2 の本数を取得します。
    //!
    //! @return     再生中の 履歴式ストライプ2 の本数。
    //---------------------------------------------------------------------------
    inline int GetProcessingSuperStripeCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.superStripeCount;
    }

    //---------------------------------------------------------------------------
    //! @brief      再生中の 連結式ストライプ の本数を取得します。
    //!
    //! @return     再生中の 連結式ストライプ の本数。
    //---------------------------------------------------------------------------
    inline int GetProcessingConnectionStripeCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.connectionStripeCount;
    }

    //---------------------------------------------------------------------------
    //! @brief        所属エミッタを取得します。
    //!
    //! @param[in]    index     取得するエミッタインデックス。
    //! @return       エミッタインスタンス。
    //---------------------------------------------------------------------------
    Emitter* GetEmitter( int index ) const NN_NOEXCEPT
    {
        return GetAliveEmitter( index );
    }

    //---------------------------------------------------------------------------
    //! @brief        所属エミッタを取得します。
    //!
    //! @param[in]    index     取得するエミッタインデックス。
    //! @return       エミッタインスタンス。
    //---------------------------------------------------------------------------
    Emitter* GetAliveEmitter( int index ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        エミッタセット生成時の所属エミッタ数取得。
    //! @return       エミッタ数。
    //---------------------------------------------------------------------------
    int GetCreatedEmitterCount() const NN_NOEXCEPT
    {
        return m_EmitterFirstCount;
    }

    //---------------------------------------------------------------------------
    //! @brief        再生中のCpuパーティクル数を取得します。
    //!
    //! @return       再生中のCpuパーティクル数。
    //---------------------------------------------------------------------------
    inline int GetProcessingCpuParticleCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.cpuParticleCount;
    }

    //---------------------------------------------------------------------------
    //! @brief        再生中のGpuパーティクル数を取得します。
    //!
    //! @return       再生中のGpuパーティクル数。
    //---------------------------------------------------------------------------
    inline int GetProcessingGpuParticleCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.gpuParticleCount;
    }

    //---------------------------------------------------------------------------
    //! @brief        再生中のGpuSoパーティクル数を取得します。
    //!
    //! @return       再生中のGpuSoパーティクル数。
    //---------------------------------------------------------------------------
    inline int GetProcessingGpuSoParticleCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.gpusoParticleCount;
    }

    //---------------------------------------------------------------------------
    //! @brief      （※旧 API ）動的ヒープから確保したメモリサイズを取得します。
    //! @deprecated 旧型式の API 名です。nn::vfx::EmitterSet::GetAllocatedFromDynamicHeapSize() に乗り換えてください。
    //! @return     動的ヒープから確保したメモリサイズ
    //! @see        nn::vfx::EmitterSet::GetAllocatedFromDynamicHeapSize()
    //---------------------------------------------------------------------------
    NN_DEPRECATED inline size_t GetAllocedFromDynamicHeapSize() const NN_NOEXCEPT
    {
        return GetAllocatedFromDynamicHeapSize();
    }

    //---------------------------------------------------------------------------
    //! @brief      （※旧 API ）Gpuバッファから確保したメモリサイズを取得します。
    //! @deprecated 旧型式の API 名です。nn::vfx::EmitterSet::GetAllocatedFromGpuBufferSize() に乗り換えてください。
    //! @return     Gpuバッファから確保したメモリサイズ
    //! @see        nn::vfx::EmitterSet::GetAllocatedFromGpuBufferSize()
    //---------------------------------------------------------------------------
    NN_DEPRECATED inline size_t GetAllocedFromGpuBufferSize() const NN_NOEXCEPT
    {
        return GetAllocatedFromGpuBufferSize();
    }

    //---------------------------------------------------------------------------
    //! @brief      動的ヒープから確保したメモリサイズを取得します。
    //!
    //! @return     確保されたメモリサイズ。
    //---------------------------------------------------------------------------
    inline size_t GetAllocatedFromDynamicHeapSize() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.allocatedDynamicHeapSize;
    }

    //---------------------------------------------------------------------------
    //! @brief      Gpuバッファから確保したメモリサイズを取得します。
    //!
    //! @return     確保されたメモリサイズ。
    //---------------------------------------------------------------------------
    inline size_t GetAllocatedFromGpuBufferSize() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.allocatedGpuBufferSize;
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタセット生死判定を取得します。
    //!
    //! @return     再生死判定。
    //---------------------------------------------------------------------------
    inline bool IsAlive() const NN_NOEXCEPT
    {
        return ( m_ProcessingInfo.emitterCount > 0 && m_IsUsage );
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタセットがループエフェクトかどうか取得します。
    //!
    //! @return     true でループエフェクト
    //---------------------------------------------------------------------------
    bool IsLoopEffect() const NN_NOEXCEPT
    {
        return ( m_IsLoopEffect != 0 );
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタセットが保持するランダムの種を取得します。
    //!
    //! @return     ランダムシード
    //---------------------------------------------------------------------------
    uint32_t GetEmitterSetRandomSeed() const NN_NOEXCEPT
    {
        return m_RandomSeed;
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタセットが保持するランダムの種を設定します。
    //!             エミッタの乱数タイプが「Eset内共通」の生成済みエミッタに対して適用されます。
    //!             またエミッタマトリクスの更新も行われます。
    //! @param[in]    randomSeed     設定するランダムの種
    //---------------------------------------------------------------------------
    void SetEmitterSetRandomSeed( uint32_t  randomSeed ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      再生中のエミッタの描画パス論理和を取得します。
    //!
    //! @return     描画パス論理輪
    //---------------------------------------------------------------------------
    uint32_t GetDrawPath() const NN_NOEXCEPT
    {
        return m_DrawPathFlag;
    }

    //---------------------------------------------------------------------------
    //! @brief      保持するエミッタ内でフレームバッファを要求する描画パスを論理和を取得します。
    //!
    //! @return     フレームバッファを要求する描画パスの論理和
    //---------------------------------------------------------------------------
    uint32_t GetRequestFrameBufferTextureDrawPath() const NN_NOEXCEPT
    {
        return m_ReqFrameBufferTexturePath;
    }

    //---------------------------------------------------------------------------
    //! @brief      保持するエミッタ内でデプスバッファを要求する描画パスを論理和を取得します。
    //!
    //! @return     デプスバッファを要求する描画パスの論理和
    //---------------------------------------------------------------------------
    uint32_t GetRequestDepthBufferTextureDrawPath() const NN_NOEXCEPT
    {
        return m_ReqDepthBufferTexturePath;
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタセット名を取得します。
    //!
    //! @return     エミッタセット名
    //---------------------------------------------------------------------------
    const char* GetName() const NN_NOEXCEPT
    {
        return m_pEmitterSetResource->pResEmitterSet->name;
    }

    //---------------------------------------------------------------------------
    //! @brief      無限寿命のエミッタ( パーティクル )を含むかどうかチェックします。
    //!
    //! @return     trueで無限寿命エミッタ( パーティクル )を含む
    //---------------------------------------------------------------------------
    bool IsHaveInfinityEmitter() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エミッタセットリソースを取得します。
    //!
    //! @return     エミッタセットリソース
    //---------------------------------------------------------------------------
    inline const EmitterSetResource* GetEmitterSetResource() const NN_NOEXCEPT
    {
        return m_pEmitterSetResource;
    }

    //@}


    //----------------------------------------
    //! @name コールバック
    //@{

    //---------------------------------------------------------------------------
    //! @brief      エミッタセット初期化時に呼び出されるコールバック関数を設定します。
    //!
    //! @param[in]  callback 登録するコールバック関数
    //---------------------------------------------------------------------------
    void SetInitializeCallback( EmitterSetInitializeCallback callback ) NN_NOEXCEPT
    {
        m_InitializeCallback = callback;
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタセット初期化時に呼び出されるコールバック関数を取得します。
    //!
    //! @return     登録済みコールバック関数
    //---------------------------------------------------------------------------
    EmitterSetInitializeCallback GetInitializeCallback() const NN_NOEXCEPT
    {
        return m_InitializeCallback;
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタセット破棄時に呼び出されるコールバック関数を設定します。
    //!
    //! @param[in]  callback 登録するコールバック関数
    //---------------------------------------------------------------------------
    void SetFinalizeCallback( EmitterSetFinalizeCallback callback ) NN_NOEXCEPT
    {
        m_FinalizeCallback = callback;
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタセット破棄時に呼び出されるコールバック関数を取得します。
    //!
    //! @return     登録済みコールバック関数
    //---------------------------------------------------------------------------
    EmitterSetFinalizeCallback GetFinalizeCallback() const NN_NOEXCEPT
    {
        return m_FinalizeCallback;
    }

    //@}


    //----------------------------------------
    //! @name フェード処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief      エミッタの放出を停止します。
    //!
    //!             データ上で指定されたフェード処理を行い、エミッタセットの再生を終了します。
    //---------------------------------------------------------------------------
    void Fade() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      フェード処理が設定されたか？。
    //!
    //! @return     設定されていればtrue、されていなければfalseが返ります。
    //---------------------------------------------------------------------------
    bool IsFadeRequest() const NN_NOEXCEPT
    {
        return ( m_IsFade != 0 );
    }

    //---------------------------------------------------------------------------
    //! @brief      配下のエミッタが一つでもフェード中かどうかを取得します。
    //!
    //! @return     配下のエミッタが一つでもフェード中の場合、 true を返します。
    //---------------------------------------------------------------------------
    bool IsAnyEmitterFadeRequest() const NN_NOEXCEPT
    {
        return m_IsAnyEmitterFade;
    }

    //---------------------------------------------------------------------------
    //! @brief        フェード処理をキャンセルします。
    //---------------------------------------------------------------------------
    void CancelFade() NN_NOEXCEPT
    {
        m_IsFade = false;
    }

    //@}


    //----------------------------------------
    //! @name 削除
    //@{

    //---------------------------------------------------------------------------
    //! @brief      エミッタセットを削除します。
    //!
    //!             瞬時に保持しているエミッタを殺します。
    //!             引数の immediate を下げることで、削除タイミングを nn::vfx::System::BeginFrame() 時に移動できます。
    //!             瞬時に消すのではなく、エミッタの放出を止めるだけなら nn::vfx::EmitterSet::Fade() を使用してください。
    //!
    //! @param[in]  immediate 即時に削除するかどうか。
    //---------------------------------------------------------------------------
    void Kill( bool immediate = true ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        無限寿命のエミッタを削除します。
    //---------------------------------------------------------------------------
    void KillInfinityEmitter() NN_NOEXCEPT;

    //@}


    //----------------------------------------
    //! @name 描画プライオリティ
    //@{

    //---------------------------------------------------------------------------
    //! @brief      描画プライオリティを設定します。
    //!
    //!             システムのエミッタセット描画ソート機能を利用する場合、
    //!             プライオリティ値とカメラからの距離でソートされます。
    //!             （同プライオリティの場合はカメラからの距離が遠いものから先に描画されます）
    //! @param[in]  priority プライオリティ値 (デフォルト値は 128 で、0～255 の範囲で設定できます) を設定します。
    //---------------------------------------------------------------------------
    void SetDrawPriority( uint8_t priority ) NN_NOEXCEPT
    {
        m_DrawPriority = priority;
    }

    //---------------------------------------------------------------------------
    //! @brief      描画プライオリティを取得します。(デフォルト値は 128 で、0～255 の範囲で設定できます)
    //!
    //! @return     プライオリティ値
    //---------------------------------------------------------------------------
    uint8_t GetDrawPriority() const NN_NOEXCEPT
    {
        return m_DrawPriority;
    }

    //@}


    //----------------------------------------
    //! @name 描画ビュー操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief      エミッタの描画ビューを指定します。
    //!
    //! @param[in]  flag       ビューフラグ。
    //---------------------------------------------------------------------------
    void SetViewFlag( DrawViewFlag flag ) NN_NOEXCEPT
    {
        m_DrawViewFlag = flag;
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタの描画処ビューフラグを取得します。
    //!
    //! @return     設定済みビューフラグ
    //---------------------------------------------------------------------------
    DrawViewFlag GetDrawViewFlag() const NN_NOEXCEPT
    {
        return m_DrawViewFlag;
    }

    //@}


    //----------------------------------------
    //! @name リスト操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief  リスト構造上の次のEmitterSetを取得します。
    //! @return 次のエミッタセットへのポインタ
    //---------------------------------------------------------------------------
    EmitterSet* GetNext() const NN_NOEXCEPT
    {
        return m_pNextEmitterSet;
    }

    //---------------------------------------------------------------------------
    //! @brief  リスト構造上の前のEmitterSetを取得します。
    //! @return 前のエミッタセットへのポインタ
    //---------------------------------------------------------------------------
    EmitterSet* GetPrev() const NN_NOEXCEPT
    {
        return m_pNextEmitterSet;
    }

    //@}


    //----------------------------------------
    //! @name 定期処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief      エミッタセットの計算処理を行います。
    //! @param[in]  frameRate フレームレート( 1.0で標準 )
    //! @param[in]  bufferSwapMode バッファスワップモード
    //! @param[in]  emitterCalculateLod LOD処理用コールバック
    //---------------------------------------------------------------------------
    void Calculate( float frameRate, BufferSwapMode bufferSwapMode, EmitterCalculateLodCallback emitterCalculateLod ) NN_NOEXCEPT;

private:
    //---------------------------------------------------------------------------
    //! @briefprivate      エミッタセットの計算処理を行います。
    //! @param[in]  frameRate フレームレート( 1.0で標準 )
    //! @param[in]  bufferSwapMode バッファスワップモード
    //! @param[in]  isForceCalc  強制計算処理モードを利用するか
    //! @param[in]  emitterCalculateLod LOD処理用コールバック
    //---------------------------------------------------------------------------
    void Calculate( float frameRate, BufferSwapMode bufferSwapMode, bool isForceCalc, EmitterCalculateLodCallback emitterCalculateLod ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate      エミッタセットの描画コマンド発行処理を行います。
    //! @param[in]  pCommandBuffer             コマンドバッファ
    //! @param[in]  drawPathFlag               描画するパス
    //! @param[in]  doComputeShaderProcess     コンピュートシェーダ処理を行うかどうか
    //! @param[in]  pUserParam                 コールバックに渡すユーザーパラメータ
    //! @param[in]  pDrawParameterArg          描画パラメータへのポインタ
    //! @param[in]  emitterDrawCullingCallback 描画カリングコールバック
    //! @param[in]  emitterDrawProfileCallback GPU描画処理コスト計測用コールバック
    //---------------------------------------------------------------------------
    void Draw( nn::gfx::CommandBuffer*    pCommandBuffer,
               uint32_t                   drawPathFlag,
               bool                       doComputeShaderProcess,
               void*                      pUserParam,
               DrawParameterArg*          pDrawParameterArg,
               EmitterDrawCullingCallback emitterDrawCullingCallback,
               DrawEmitterProfileCallback emitterDrawProfileCallback ) NN_NOEXCEPT;

public:
    //---------------------------------------------------------------------------
    //! @brief      エミッタセットの描画コマンド発行処理を行います。
    //! @param[in]  pCommandBuffer             コマンドバッファ
    //! @param[in]  processingIndex            処理インデックス。nn::vfx::Config で指定されたインデックス数までが利用可能です。
    //! @param[in]  drawPathFlag               描画するパス
    //! @param[in]  doComputeShaderProcess     コンピュートシェーダ処理を行うかどうか
    //! @param[in]  pUserParam                 コールバックに渡すユーザーパラメータ
    //! @param[in]  emitterDrawCullingCallback 描画カリングコールバック
    //! @param[in]  emitterDrawProfileCallback GPU描画処理コスト計測用コールバック
    //---------------------------------------------------------------------------
    void Draw( nn::gfx::CommandBuffer*    pCommandBuffer,
               int                        processingIndex,
               uint32_t                   drawPathFlag,
               bool                       doComputeShaderProcess,
               void*                      pUserParam,
               EmitterDrawCullingCallback emitterDrawCullingCallback,
               DrawEmitterProfileCallback emitterDrawProfileCallback ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      計算処理を指定回数回す。
    //!             再生開始フレームを指定した場合に利用します。
    //!
    //! @param[in]  loopCount 回す回数。
    //---------------------------------------------------------------------------
    void ForceCalculate( int loopCount ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      計算処理の有効/無効を設定します。
    //!
    //! @param[in]  flag 有効/無効。
    //---------------------------------------------------------------------------
    void SetCalculationEnable( bool flag ) NN_NOEXCEPT
    {
        m_IsCalculationEnabled = flag;
    }

    //---------------------------------------------------------------------------
    //! @brief      計算処理の有効/無効を取得します。
    //!
    //! @return     trueで描画処理有効、falseで無効
    //---------------------------------------------------------------------------
    bool IsCalculationEnabled() const NN_NOEXCEPT
    {
        return ( m_IsCalculationEnabled == 1 );
    }

    //---------------------------------------------------------------------------
    //! @brief      計算処理と描画処理を同時に停止します。
    //!
    //!             止めると計算が回らないので処理が早くはなりますが、
    //!             内部ではエミッタを確保したままなので、エミッタ数は減りません。
    //! @param[in]  isStopCalculationAndDraw       true で計算/描画処理を停止します。
    //---------------------------------------------------------------------------
    void SetStopCalcAndDraw( bool isStopCalculationAndDraw ) NN_NOEXCEPT
    {
        m_IsCalculationEnabled = !isStopCalculationAndDraw;
        m_IsDraw = !isStopCalculationAndDraw;
    }

    //---------------------------------------------------------------------------
    //! @brief      計算処理と描画処理が停止状態かどうかチェックします。
    //!
    //! @return     trueで描画処理有効、falseで無効
    //---------------------------------------------------------------------------
    bool IsStopCalcAndDraw() const NN_NOEXCEPT
    {
        return ( !m_IsCalculationEnabled && !m_IsDraw );
    }

    //---------------------------------------------------------------------------
    //! @brief      再生中エミッタの計算処理を停止します。
    //!
    //! @param[in]  isStopCalculation       計算処理を止めるか？。
    //---------------------------------------------------------------------------
    void SetStopCalculation( bool isStopCalculation ) NN_NOEXCEPT
    {
        SetCalculationEnable( !isStopCalculation );
    }

    //---------------------------------------------------------------------------
    //! @brief      計算処理が停止状態かどうかチェックします。
    //!
    //! @return     trueで計算処理停止中、falseで再生中
    //---------------------------------------------------------------------------
    bool IsStopCalculation() const NN_NOEXCEPT
    {
        return ( m_IsCalculationEnabled == 0 );
    }

    //@}


    //----------------------------------------
    //! @name ユーザーデータ
    //@{

    //---------------------------------------------------------------------------
    //! @brief      ユーザーデータを取得します。
    //!
    //!             現状動作していません。
    //! @return     ユーザーデータ
    //---------------------------------------------------------------------------
    uint32_t GetUserData() const NN_NOEXCEPT
    {
        return 0;
    }

    //---------------------------------------------------------------------------
    //! @brief      ユーザーデータを取得します。
    //!
    //! @return     ユーザーデータ
    //---------------------------------------------------------------------------
    uint32_t GetUserData1() const NN_NOEXCEPT
    {
        return m_pEmitterSetResource->pResEmitterSet->UserDataValue1;
    }

    //---------------------------------------------------------------------------
    //! @brief      ユーザーデータを取得します。
    //!
    //! @return     ユーザーデータ
    //---------------------------------------------------------------------------
    uint32_t GetUserData2() const NN_NOEXCEPT
    {
        return m_pEmitterSetResource->pResEmitterSet->UserDataValue2;
    }

    //---------------------------------------------------------------------------
    //! @brief      ユーザーデータを取得します。
    //!
    //! @return     ユーザーデータ
    //---------------------------------------------------------------------------
    uint32_t GetUserData3() const NN_NOEXCEPT
    {
        return m_pEmitterSetResource->pResEmitterSet->UserDataValue3;
    }

    //---------------------------------------------------------------------------
    //! @brief      ユーザーデータを取得します。
    //!
    //! @return     ユーザーデータ
    //---------------------------------------------------------------------------
    uint32_t GetUserData4() const NN_NOEXCEPT
    {
        return m_pEmitterSetResource->pResEmitterSet->UserDataValue4;
    }

    //---------------------------------------------------------------------------
    //! @brief      16bit ユーザーデータを取得します。
    //!
    //! @return     16bit ユーザーデータ
    //---------------------------------------------------------------------------
    uint16_t GetUserDataBit() const NN_NOEXCEPT
    {
        return m_pEmitterSetResource->pResEmitterSet->UserDataBit;
    }

    //@}

    //----------------------------------------
    //! @name ランタイムユーザーデータ
    //@{

    //---------------------------------------------------------------------------
    //! @brief      ランタイムユーザーポインタを取得します。
    //! @return     ランタイムユーザーポインタ
    //---------------------------------------------------------------------------
    void* GetRuntimeUserPtr() const NN_NOEXCEPT
    {
        return m_RuntimeUserPtr;
    }

    //---------------------------------------------------------------------------
    //! @brief        ランタイムユーザーポインタ(リソースには含まれず、アプリケーションの都合で設定できる変数)を設定します。
    //!               初期値は0になっています。
    //! @param[in] data ランタイムユーザーポインタ
    //---------------------------------------------------------------------------
    void SetRuntimeUserPtr( void* data ) NN_NOEXCEPT
    {
        m_RuntimeUserPtr = data;
    }

    //---------------------------------------------------------------------------
    //! @brief          ユーザーデータへのポインタを取得します。
    //! @deprecated 旧型式の API 名です。nn::vfx::EmitterSet::GetRuntimeUserPtr() に乗り換えてください。
    //! @return         ユーザーデータへのポインタ
    //---------------------------------------------------------------------------
    NN_DEPRECATED void* GetUserPtr() const NN_NOEXCEPT
    {
        return GetRuntimeUserPtr();
    }

    //---------------------------------------------------------------------------
    //! @brief          ユーザーデータへのポインタを設定します。
    //! @deprecated   旧型式の API 名です。nn::vfx::EmitterSet::SetRuntimeUserPtr() に乗り換えてください。
    //! @param[in] ptr  ユーザーデータへのポインタ
    //---------------------------------------------------------------------------
    NN_DEPRECATED void SetUserPtr( void* ptr ) NN_NOEXCEPT
    {
        SetRuntimeUserPtr( ptr );
    }

    //---------------------------------------------------------------------------
    //! @brief      ランタイムユーザーデータを取得します。
    //! @deprecated 旧型式の API 名です。nn::vfx::EmitterSet::GetRuntimeUserPtr() に乗り換えてください。
    //! @return     ランタイムユーザーデータ
    //---------------------------------------------------------------------------
    NN_DEPRECATED void* GetRuntimeUserData() const NN_NOEXCEPT
    {
        return GetRuntimeUserPtr();
    }

    //---------------------------------------------------------------------------
    //! @brief        ランタイムユーザーデータ(リソースには含まれず、アプリケーションの都合で設定できる変数)を設定します。
    //!               初期値は 0 になっています。
    //! @deprecated   旧型式の API 名です。nn::vfx::EmitterSet::SetRuntimeUserPtr() に乗り換えてください。
    //! @param[in] data ランタイムユーザーデータ
    //---------------------------------------------------------------------------
    NN_DEPRECATED void SetRuntimeUserData( void* data ) NN_NOEXCEPT
    {
        SetRuntimeUserPtr( data );
    }

    //---------------------------------------------------------------------------
    //! @brief      配下のエミッタが持つカスタムアクション用 CallbackSet を上書きします。
    //! @param[in]  pCustomActionCallbackSet    カスタムアクション用コールバックセットへのポインタ
    //---------------------------------------------------------------------------
    void OverwriteCustomActionCallbackSet( CallbackSet* pCustomActionCallbackSet ) NN_NOEXCEPT;

    //@}



private:
    //---------------------------------------------------------------------------
    //! @briefprivate               エミッタを生成します。
    //! @param[in] pEmitterResource エミッタリソースへのポインタ
    //! @param[in] maxParticleCount パーティクルの最大数
    //! @param[in] pParentEmitter   親エミッタへのポインタ
    //! @param[in] childIndex       チャイルドの場合、自身のインデックス
    //! @return                     生成されたエミッタへのポインタ
    //---------------------------------------------------------------------------
    Emitter* CreateEmitter( const EmitterResource* pEmitterResource, int maxParticleCount, Emitter* pParentEmitter = NULL, int childIndex = -1 ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate           エミッタを削除します。
    //! @param[in] pEmitter     エミッタへのポインタ
    //---------------------------------------------------------------------------
    void KillEmitter( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate               初期化処理を行います。
    //! @param[in] emitterSetId     エミッタセットID
    //! @param[in] createId         エミッタセット番号
    //! @param[in] resourceId       リソースID
    //! @param[in] groupId          グループID
    //! @param[in] maxParticleCount パーティクル最大数
    //! @param[in] pHeap            ヒープへのポインタ
    //! @return                     成功した場合 true
    //---------------------------------------------------------------------------
    bool Initialize( int emitterSetId, int createId, int resourceId, int groupId, int maxParticleCount, Heap* pHeap ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate   終了処理を行います。
    //! @return         成功した場合 true
    //---------------------------------------------------------------------------
    bool Finalize() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate       エフェクトシステムの設定を行います。
    //! @param[in] pSystem  システムへのポインタ
    //---------------------------------------------------------------------------
    void SetSystem( System *pSystem ) NN_NOEXCEPT
    {
        m_pSystem = pSystem;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate               エミッタをリストへ追加します。
    //! @param[in]  pEmitter        エミッタへのポインタ
    //! @param[in]  pParentEmitter  親エミッタへのポインタ
    //---------------------------------------------------------------------------
    void AddEmitterToList( Emitter* pEmitter, Emitter* pParentEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate           エミッタをリストから削除します。
    //! @param[in]  pEmitter    エミッタへのポインタ
    //---------------------------------------------------------------------------
    void RemoveEmitterFromList( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate           エミッタを削除します。
    //! @param[in] pEmitter     エミッタへのポインタ
    //---------------------------------------------------------------------------
    void KillEmitterImpl( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate   リソース更新に伴うアップデートを行います。
    //! @param[in]      pEmitterResource エミッタリソースへのポインタ
    //! @return         成功した場合 true
    //---------------------------------------------------------------------------
    bool UpdateFromResource( EmitterResource* pEmitterResource ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate   リセット処理を行います。
    //---------------------------------------------------------------------------
    void Reset() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate   Initialize にも共通するリセット処理です。
    //---------------------------------------------------------------------------
    void ResetCommon() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate   スケール更新
    //---------------------------------------------------------------------------
    inline void UpdateParticleScale() NN_NOEXCEPT
    {
        nn::util::VectorMultiply( &m_ParticleScaleForCalculation, m_ParticleScale, m_AutoCalculatedScale );
    }


    //---------------------------------------------------------------------------
    //! @briefprivate   Calculate 内で必須な処理を行います。
    //---------------------------------------------------------------------------
    void RequiredProcessOnCalculate() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate                           エミッタ計算処理
    //! @param[in]  pEmitter                    エミッタへのポインタ
    //! @param[in]  bufferSwapMode              バッファスワップモード
    //! @param[in]  forceCalc                   強制的にパーティクル計算を行うかどうかのフラグ
    //! @param[in]  emitterCalculateLodCallback Lodコールバック
    //! @return                                 計算結果を示す nn::vfx::EmitterCalculationResult の値
    //---------------------------------------------------------------------------
    EmitterCalculationResult CalculateEmitter( Emitter* pEmitter, BufferSwapMode bufferSwapMode, bool forceCalc, EmitterCalculateLodCallback emitterCalculateLodCallback ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate                           エミッタ描画処理
    //! @param[in] pCommandBuffer               コマンドバッファへのポインタ
    //! @param[in] pEmitter                     エミッタへのポインタ
    //! @param[in] drawPathFlag                 描画パスフラグ
    //! @param[in] doComputeShaderProcess       コンピュートシェーダを処理するかどうかのフラグ
    //! @param[in] pUserParam                   ユーザーパラメータへのポインタ
    //! @param[in] pDrawParameterArg            描画パラメータ引数
    //! @param[in] emitterDrawCullingCallback   エミッタカリングコールバック
    //! @param[in] profieCallback               プロファイルコールバック
    //---------------------------------------------------------------------------
    void DrawEmitter( nn::gfx::CommandBuffer*     pCommandBuffer,
                      Emitter*                    pEmitter,
                      uint32_t                    drawPathFlag,
                      bool                        doComputeShaderProcess,
                      void*                       pUserParam,
                      DrawParameterArg*           pDrawParameterArg,
                      EmitterDrawCullingCallback  emitterDrawCullingCallback,
                      DrawEmitterProfileCallback  profieCallback ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate                           チャイルドエミッタセットの描画処理を行います。
    //! @param[in] pCommandBuffer               コマンドバッファへのポインタ
    //! @param[in] beforeThanParent             親より先に描画するかどうかのフラグ
    //! @param[in] pParentEmitter               親エミッタへのポインタ
    //! @param[in] drawPathFlag                 描画パスフラグ
    //! @param[in] doComputeShaderProcess       コンピュートシェーダを処理するかどうかのフラグ
    //! @param[in] pUserParam                   ユーザーパラメータへのポインタ
    //! @param[in] pDrawParameterArg            描画パラメータ引数
    //! @param[in] emitterDrawCullingCallback   エミッタカリングコールバック
    //! @param[in] profileCallback              プロファイルコールバック
    //---------------------------------------------------------------------------
    void DrawChildEmitter( nn::gfx::CommandBuffer*     pCommandBuffer,
                           bool                        beforeThanParent,
                           Emitter*                    pParentEmitter,
                           uint32_t                    drawPathFlag,
                           bool                        doComputeShaderProcess,
                           void*                       pUserParam,
                           DrawParameterArg*           pDrawParameterArg,
                           EmitterDrawCullingCallback  emitterDrawCullingCallback,
                           DrawEmitterProfileCallback  profileCallback ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate       パーティクル処理数／消費メモリ量などの情報更新
    //! @param[in] pEmitter エミッタへのポインタ
    //---------------------------------------------------------------------------
    void UpdateProcessingInfo( const Emitter* pEmitter ) NN_NOEXCEPT
    {
        m_ProcessingInfo.UpdateProcessingInfo( pEmitter );
    }

private:
    //---------------------------------------------------------------------------
    //! @briefprivate           引数で渡されるエミッタリストの終了処理を行います。
    //! @param[in] pEmitterHead 先頭エミッタへのポインタ
    //! @return                 成否判定。
    //---------------------------------------------------------------------------
    bool FinalizeEmitterList( Emitter* pEmitterHead ) NN_NOEXCEPT;


private:
    nn::gfx::Device*            m_pDevice;                          //!< Gfxデバイス
    uint8_t                     m_IsLoopEffect;                     //!< ループエフェクトかどうか
    uint8_t                     m_IsFade;                           //!< フェード中かどうか
    uint8_t                     m_IsCalculationEnabled;             //!< 計算処理を行うかどうか
    uint8_t                     m_IsDraw;                           //!< 描画処理を行うかどうか
    uint8_t                     m_IsUsage;                          //!< 利用されているか
    uint8_t                     m_IsSetDirectional;                 //!< 指定方向が設定されたか？
    uint8_t                     m_IsManualEmission;                 //!< マニュアル放出モードか？
    uint8_t                     m_IsDelayCreate;                    //!< 遅延生成されたか？
    uint8_t                     m_DrawPriority;                     //!< 描画プライオリティ
    int                         m_GroupId;                          //!< グループID
    int                         m_IsEmitterSrtDirty;                //!< SRT行列のダーティフラグ
    System*                     m_pSystem;                          //!< システムクラス
    int                         m_EmitterSetId;                     //!< エミッタセットID
    int                         m_EmitterSetCreateId;               //!< エミッタセット生成ID
    int                         m_ResourceId;                       //!< リソースID
    uint32_t                    m_RandomSeed;                       //!< ランダム種
    int                         m_EmitterCreateId;                  //!< エミッタセット内、エミッタ生成ID
    uint32_t                    m_ReqFrameBufferTexturePath;        //!< フレームバッファテクスチャを要求するパス
    uint32_t                    m_ReqDepthBufferTexturePath;        //!< デプスバッファテクスチャを要求するパス
    int                         m_ManualEmissionAssignCount;        //!< マニュアル放出でアサインされた数
    DrawViewFlag                m_DrawViewFlag;                     //!< ビューフラグ
    float                       m_FrameRate;                        //!< フレームレート
    float                       m_EmissionRatioScale;               //!< 放出レートスケール
    float                       m_EmissionIntervalScale;            //!< 放出間隔スケール
    float                       m_ParticleLifeScale;                //!< パーティクル寿命スケール
    nn::util::Matrix4x3fType    m_MatrixSrt;                        //!< エミッタセットSRT行列
    nn::util::Matrix4x3fType    m_MatrixRt;                         //!< エミッタセットRT行列
    nn::util::Vector3fType      m_EmitterVolumeScale;               //!< エミッタ形状のスケール
    nn::util::Vector3fType      m_AutoCalculatedScale;              //!< 自動計算されるスケール
    nn::util::Vector4fType      m_Color;                            //!< エミッタセットカラー
    nn::util::Vector3fType      m_InitialRoate;                     //!< 初期角度
    nn::util::Vector3fType      m_ParticleScale;                    //!< パーティクルスケール
    nn::util::Vector3fType      m_ParticleEmissionScale;            //!< 放出時のみに効くパーティクルスケール
    nn::util::Vector3fType      m_ParticleScaleForCalculation;      //!< 計算用のパーティクルスケール
    Emitter*                    m_pEmitterHead;                     //!< エミッタツリー(子も含む)
    Emitter*                    m_pEmitterTail;                     //!< エミッタツリー
    Emitter*                    m_pResetEmitterHead;                //!< エミッタリセットツリー(子も含む)
    int                         m_ResetEmitterCount;                //!< エミッタリセットカウンタ
    int                         m_EmitterCount;                     //!< データ上のエミッタ数
    int                         m_StartFrame;                       //!< 開始フレーム
    int                         m_GlobalCounter;                    //!< エミッタセット内カウンタ
    ProcessingInfo              m_ProcessingInfo;                   //!< 処理されたエミッタ数、パーティクル数などの情報
    void*                       m_RuntimeUserPtr;                   //!< ランタイムで自由に設定してよいユーザデータ
    int                         m_EmitterFirstCount;                //!< セット生成時に作成したエミッタ数
    EmitterSet*                 m_pNextEmitterSet;                  //!< EmitterSet リスト
    EmitterSet*                 m_pPrevEmitterSet;                  //!< EmitterSet リスト
    Heap*                       m_pHeap;                            //!< 生成時にメモリを確保するヒープ
    float                       m_FigureVel;                        //!< 形状で決まる速度のスケール値
    float                       m_RandomVel;                        //!< 初期速度ランダムのスケール値
    nn::util::Vector3fType      m_AdditionalVel;                    //!< 最終的な初速に加算する値
    nn::util::Vector3fType      m_EmitterScale;                     //!< エミッタ形状のスケール
    float                       m_DirectionalVel;                   //!< 指定方向速度のスケール値
    nn::util::Vector3fType      m_Directional;                      //!< 指定方向
    uint32_t                    m_DrawPathFlag;                     //!< 保持する全てのエミッタ描画パスフラグ
    EmitterSetResource*         m_pEmitterSetResource;              //!< エミッタセットリソース
    EmitterSetInitializeCallback  m_InitializeCallback;             //!< Initialize時に呼ばれるコールバック
    EmitterSetFinalizeCallback  m_FinalizeCallback;                 //!< Finalize時に呼ばれるコールバック
    EmitReservationInfo*        m_pEmitReservationListHead;         //!< マニュアル放出用予約リストの先頭アドレス
    int                         m_MaxEmitCountPerFrame;             //!< マニュアル放出用 1フレームあたりの最大放出数
    int                         m_ManualEmissionCount;              //!< マニュアル放出用 予約カウンタ
    int                         m_ResidentEmitterTime;              //!< マニュアル放出エミッタで最低限待機するフレーム数
    bool                        m_IsAnyEmitterFade;                 //!< 配下のエミッタが一つでもフェードしているかどうか
    bool                        m_EnableDistanceBasedEmission;     //!< 距離放出を一時停止するかどうか
    friend class System;
    friend class detail::EmitterCalculator;
};

} // namespace vfx
} // namespace nn
