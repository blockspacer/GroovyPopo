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
#include <nn/vfx/vfx_EmitterSet.h>
#include <nn/vfx/vfx_EmitterCalc.h>
#include <nn/vfx/vfx_MemUtil.h>

namespace nn {
namespace vfx {

namespace detail {
struct SuperStripeHistory;
}

//---------------------------------------------------------------------------
//! @brief  カスタムフィールドコールバック
//---------------------------------------------------------------------------
typedef bool( *CustomFieldCallback )( nn::util::Vector3fType* pOutPos, nn::util::Vector3fType* pOutVec, float* pOutLife, float* pOutBirthTime, Emitter* pEmitter, const detail::ParticleProperty* pParticleProperty, const detail::ResFieldCustom* pCustomFieldData, int particleIndex );

//---------------------------------------------------------------------------
//! @brief  パーティクル処理用コールバックに引数として渡される構造体です。
//!         ここから操作対象のエミッタにアクセスできます。
//---------------------------------------------------------------------------
class ParticleCalculateArgImpl
{
    NN_DISALLOW_COPY( ParticleCalculateArgImpl );
public:
    void*                   pUserData;                   //!< ユーザーデータ(パーティクル生成コールバックで返したアドレス)
    void*                   pEmitterPluginData;          //!< エミッタプラグインデータ(パーティクル生成コールバックで返したアドレス)
    Emitter*                pEmitter;                    //!< エミッタインスタンス

    //---------------------------------------------------------------------------
    //! @brief  パーティクル時間を取得
    //! @return パーティクル時間
    //---------------------------------------------------------------------------
    float GetTime() const NN_NOEXCEPT
    {
        return m_ParticleTime;
    }

    //---------------------------------------------------------------------------
    //! @brief  パーティクル寿命を取得
    //! @return パーティクル寿命
    //---------------------------------------------------------------------------
    float GetLife() const NN_NOEXCEPT
    {
        return m_ParticleLife;
    }

    //---------------------------------------------------------------------------
    //! @brief              ローカル位置を取得
    //! @param[out] pOutPos  ローカル位置を格納するVEC3へのポインタ
    //---------------------------------------------------------------------------
    void GetLocalPos( nn::util::Vector3fType* pOutPos ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ワールド位置を取得
    //! @param[out] pOutPos  ワールド位置を格納するVEC3へのポインタ
    //---------------------------------------------------------------------------
    void GetWorldPos( nn::util::Vector3fType* pOutPos ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ローカル速度を取得
    //! @param[out] pOutVec  ローカル速度を格納するVEC3へのポインタ
    //---------------------------------------------------------------------------
    void GetLocalVec( nn::util::Vector3fType* pOutVec ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ワールド速度を取得
    //! @param[out] pOutVec  ワールド速度を格納するVEC3へのポインタ
    //---------------------------------------------------------------------------
    void GetWorldVec( nn::util::Vector3fType* pOutVec ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ローカル移動量差分を取得
    //! @param[out] pOutDiff ローカル移動量差分を格納するVEC3へのポインタ
    //---------------------------------------------------------------------------
    void GetLocalDiff( nn::util::Vector3fType* pOutDiff ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  回転初期値を取得（Radian）
    //! @param[out] pOutRotate   回転初期値を格納するVEC3へのポインタ
    //---------------------------------------------------------------------------
    void GetInitialRotateRadian( nn::util::Vector3fType* pOutRotate ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  回転初期値を取得（Degree）
    //! @param[out] pOutRotate   回転初期値を格納するVEC3へのポインタ
    //---------------------------------------------------------------------------
    void GetInitialRotateDegree( nn::util::Vector3fType* pOutRotate ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  Particle ごとのランダム値を取得
    //! @param[out] pOutRandom   Particle ごとのランダム値を格納するVEC4へのポインタ
    //---------------------------------------------------------------------------
    void GetRandom( nn::util::Vector4fType* pOutRandom ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  現在の回転量を計算（Radian）
    //! @param[out] pOutRotate   現在の回転量を格納するVEC3へのポインタ
    //---------------------------------------------------------------------------
    void CalculateCurrentRotateRadian( nn::util::Vector3fType* pOutRotate ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  現在の回転量を計算（Degree）
    //! @param[out] pOutRotate   現在の回転量を格納するVEC3へのポインタ
    //---------------------------------------------------------------------------
    void CalculateCurrentRotateDegree( nn::util::Vector3fType* pOutRotate ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  ワールドスケール値を計算
    //! @param[out] pOutScale    ワールドスケール値を格納するVEC3へのポインタ
    //---------------------------------------------------------------------------
    void GetInitialScale( nn::util::Vector3fType* pOutScale ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  ワールドスケール値を計算
    //! @param[out] pOutScale    ワールドスケール値を格納するVEC3へのポインタ
    //---------------------------------------------------------------------------
    void CalculateWorldScale( nn::util::Vector3fType* pOutScale ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  カラー0／アルファ0を計算
    //! @param[out] pOutColor    カラー0／アルファ0を格納するVEC4へのポインタ
    //---------------------------------------------------------------------------
    void CalculateLocalColor0( nn::util::Vector4fType* pOutColor ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  カラー1／アルファ1を計算
    //! @param[out] pOutColor    カラー1／アルファ1を格納するVEC4へのポインタ
    //---------------------------------------------------------------------------
    void CalculateLocalColor1( nn::util::Vector4fType* pOutColor ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ローカル位置を取得。VEC4版。
    //! @param[out] pOutPos  ローカル位置を格納するVEC4へのポインタ
    //---------------------------------------------------------------------------
    void GetLocalPos( nn::util::Vector4fType* pOutPos ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ワールド位置を取得
    //! @param[out] pOutPos  ワールド位置
    //---------------------------------------------------------------------------
    void GetWorldPos( nn::util::Vector4fType* pOutPos ) const NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief                  パーティクル位置をワールド座標系に変換するための、追従設定を加味した変換行列を取得します。
    //! @param[out] pOutMatrix  変換用の行列へのポインタ
    //------------------------------------------------------------------------------
    void GetEmitterTransformMatrix( nn::util::Matrix4x3f* pOutMatrix ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          ローカル位置を設定
    //! @param[in] pos  ローカル位置
    //---------------------------------------------------------------------------
    void SetLocalPos( const nn::util::Vector3fType& pos ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          ローカル位置を設定
    //! @param[in] x    x座標
    //! @param[in] y    y座標
    //! @param[in] z    z座標
    //---------------------------------------------------------------------------
    void SetLocalPos( float x, float y, float z ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          ワールド位置を設定
    //! @param[in] pos  ワールド位置
    //---------------------------------------------------------------------------
    void SetWorldPos( const nn::util::Vector3fType& pos ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          ワールド位置を設定
    //! @param[in] x    x座標
    //! @param[in] y    y座標
    //! @param[in] z    z座標
    //---------------------------------------------------------------------------
    void SetWorldPos( float x, float y, float z ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          ローカル速度を設定
    //! @param[in] vec  ローカル速度
    //---------------------------------------------------------------------------
    void SetLocalVec( const nn::util::Vector3fType& vec ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          ローカル速度を設定
    //! @param[in] x    速度のx成分
    //! @param[in] y    速度のy成分
    //! @param[in] z    速度のz成分
    //---------------------------------------------------------------------------
    void SetLocalVec( float x, float y, float z ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          ワールド速度を設定
    //! @param[in] vec  ワールド速度
    //---------------------------------------------------------------------------
    void SetWorldVec( const nn::util::Vector3fType& vec ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          ワールド速度を設定
    //! @param[in] x    速度のx成分
    //! @param[in] y    速度のy成分
    //! @param[in] z    速度のz成分
    //---------------------------------------------------------------------------
    void SetWorldVec( float x, float y, float z ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              初期スケールを設定
    //! @param[in] scale    初期スケール
    //---------------------------------------------------------------------------
    void SetInitialScale( const nn::util::Vector3fType& scale ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              初期スケールXを設定
    //! @param[in] scaleX   初期スケールX
    //---------------------------------------------------------------------------
    void SetInitialScaleX( float scaleX ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              初期スケールYを設定
    //! @param[in] scaleY   初期スケールY
    //---------------------------------------------------------------------------
    void SetInitialScaleY( float scaleY ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              回転量を設定（Radian）
    //! @param[in] rotate   回転量
    //---------------------------------------------------------------------------
    void SetInitialRotateRadian( const nn::util::Vector3fType& rotate ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              回転量を設定（Degree）
    //! @param[in] rotate   回転量
    //---------------------------------------------------------------------------
    void SetInitialRotateDegree( const nn::util::Vector3fType& rotate ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      パーティクルを削除します
    //! @param[in]  keepChildEmitter    true の場合、チャイルドエミッタがいたときに、それらを消さずに残します。
    //---------------------------------------------------------------------------
    void Kill( bool keepChildEmitter = false ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ローカル速度に変換
    //! @param[out] pOutDst  出力を格納するVEC3へのポインタ
    //! @param[in]  src     ワールド速度
    //---------------------------------------------------------------------------
    void TransformToLocalVec( nn::util::Vector3fType* pOutDst, const nn::util::Vector3fType& src ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ワールド速度に変換
    //! @param[out] pOutDst  出力を格納するVEC3へのポインタ
    //! @param[in]  src     ローカル速度
    //---------------------------------------------------------------------------
    void TransformToWorldVec( nn::util::Vector3fType* pOutDst, const nn::util::Vector3fType& src ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ローカル位置に変換
    //! @param[out] pOutDst  出力を格納するVEC3へのポインタ
    //! @param[in] src      ワールド位置
    //---------------------------------------------------------------------------
    void TransformToLocalPos( nn::util::Vector3fType* pOutDst, const nn::util::Vector3fType& src ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ローカル位置に変換
    //! @param[out] pOutDst  出力を格納するVEC4へのポインタ
    //! @param[in] src      ワールド位置
    //---------------------------------------------------------------------------
    void TransformToLocalPos( nn::util::Vector4fType* pOutDst, const nn::util::Vector4fType& src ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ワールド位置に変換
    //! @param[out] pOutDst  出力を格納するVEC3へのポインタ
    //! @param[in] src      ローカル位置
    //---------------------------------------------------------------------------
    void TransformToWorldPos( nn::util::Vector3fType* pOutDst, const nn::util::Vector3fType& src ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ワールド位置に変換
    //! @param[out] pOutDst  出力を格納するVEC4へのポインタ
    //! @param[in] src      ローカル位置
    //---------------------------------------------------------------------------
    void TransformToWorldPos( nn::util::Vector4fType* pOutDst, const nn::util::Vector4fType& src ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  コンストラクタ
    //! @param[in] pEmitter      エミッタへのポインタ
    //! @param[in] time         パーティクル時間
    //! @param[in] life         パーティクル寿命
    //! @param[in] index    自身が格納されているパーティクル配列のインデックス
    //---------------------------------------------------------------------------
    ParticleCalculateArgImpl( Emitter* pEmitter, float time, float life, int index ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @internal
    //! @briefprivate   履歴式ストライプ2 での履歴点の配列への参照を取得します。
    //!                 この API は予告なく仕様が変更される可能性があります。
    //! @param[out]     pOutHistoryHead 履歴点（ nw::eft2::SuperStripeHistory ）の配列の先頭へのポインタ
    //! @param[out]     pOutHistoryNum  履歴点の総数
    //---------------------------------------------------------------------------
    void GetSuperStripeHistoryBuffer( detail::SuperStripeHistory** pOutHistoryHead, int* pOutHistoryNum ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief                  パーティクルの計算済みのSRT行列を取得します。
    //! @param[out] pOutMatrix  パーティクルの行列へのポインタ
    //------------------------------------------------------------------------------
    void CalculateParticleMatrix( nn::util::Matrix4x3fType* pOutMatrix ) NN_NOEXCEPT;

private:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //---------------------------------------------------------------------------
    ParticleCalculateArgImpl() NN_NOEXCEPT;

    float                   m_ParticleTime;     //!< パーティクル時間
    float                   m_ParticleLife;     //!< パーティクル寿命
    int                     m_ParticleIndex;    //!< パーティクルのインデックス
};

//---------------------------------------------------------------------------
//! @brief  コンストラクタ
//---------------------------------------------------------------------------
inline ParticleCalculateArgImpl::ParticleCalculateArgImpl( Emitter* pEmitter, float time, float life, int index ) NN_NOEXCEPT
    : pUserData( NULL )
    , pEmitterPluginData( NULL )
    , pEmitter( pEmitter )
    , m_ParticleTime( time )
    , m_ParticleLife( life )
    , m_ParticleIndex( index )
{
    NN_SDK_ASSERT_NOT_NULL( pEmitter );
}

//---------------------------------------------------------------------------
//! @brief  パーティクル放出コールバックに渡される構造体の定義
//---------------------------------------------------------------------------
typedef ParticleCalculateArgImpl ParticleEmitArg;
//---------------------------------------------------------------------------
//! @brief  パーティクル生成後にコールされるコールバックです。
//!         パーティクル生成をキャンセルしたい場合は、falseを返すようにしてください。
//---------------------------------------------------------------------------
typedef bool( *ParticleEmitCallback )( ParticleEmitArg& arg );

//---------------------------------------------------------------------------
//! @brief  パーティクル計算コールバックに渡される構造体の定義
//---------------------------------------------------------------------------
typedef ParticleCalculateArgImpl ParticleCalculateArg;
//---------------------------------------------------------------------------
//! @brief  パーティクル計算処理コールバックの定義です。
//!         パーティクル挙動の計算処理終了時にコールされます。
//---------------------------------------------------------------------------
typedef void( *ParticleCalculateCallback )( ParticleCalculateArg& arg );

//---------------------------------------------------------------------------
//! @brief  パーティクル削除コールバックに渡される構造体の定義
//---------------------------------------------------------------------------
typedef ParticleCalculateArgImpl ParticleRemoveArg;

//---------------------------------------------------------------------------
//! @brief  パーティクル削除時にコールされるコールバックです。
//---------------------------------------------------------------------------
typedef bool( *ParticleRemoveCallback )( ParticleRemoveArg& arg );

//---------------------------------------------------------------------------
//! @brief  移動量差分を取得
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::GetLocalDiff( nn::util::Vector3fType* pOutDiff ) const NN_NOEXCEPT
{
    nn::util::VectorLoad( pOutDiff, pEmitter->m_CpuParticleProperty.localDiff[ m_ParticleIndex ].v );
}

//---------------------------------------------------------------------------
//! @brief  ローカル位置を取得
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::GetLocalPos( nn::util::Vector3fType* pOutPos ) const NN_NOEXCEPT
{
    nn::util::VectorLoad( pOutPos, pEmitter->m_CpuParticleProperty.localPos[ m_ParticleIndex ].v );
}

//---------------------------------------------------------------------------
//! @brief  ローカル位置を取得
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::GetLocalPos( nn::util::Vector4fType* pOutPos ) const NN_NOEXCEPT
{
    nn::util::VectorLoad( pOutPos, pEmitter->m_CpuParticleProperty.localPos[ m_ParticleIndex ] );
}

//---------------------------------------------------------------------------
//! @brief  ワールド位置を取得
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::GetWorldPos( nn::util::Vector3fType* pOutPos ) const NN_NOEXCEPT
{
    nn::util::Vector3fType localPos;
    nn::util::VectorLoad( &localPos, pEmitter->m_CpuParticleProperty.localPos[ m_ParticleIndex ].v );

    // ワールド座標に変換して返す。
    TransformToWorldPos( pOutPos, localPos );
}

//---------------------------------------------------------------------------
//! @brief  ワールド位置を取得
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::GetWorldPos( nn::util::Vector4fType* pOutPos ) const NN_NOEXCEPT
{
    // ワールド座標に変換して返す。
    nn::util::Vector4fType localPos;
    nn::util::VectorLoad( &localPos, pEmitter->m_CpuParticleProperty.localPos[ m_ParticleIndex ] );
    nn::util::VectorSetW( &localPos, 0 );

    TransformToWorldPos( pOutPos, localPos );
}

//---------------------------------------------------------------------------
//! @brief  ローカル速度を取得
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::GetLocalVec( nn::util::Vector3fType* pOutVec ) const NN_NOEXCEPT
{
    nn::util::VectorLoad( pOutVec, pEmitter->m_CpuParticleProperty.localVec[ m_ParticleIndex ].v );
}

//---------------------------------------------------------------------------
//! @brief  ワールド速度を取得
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::GetWorldVec( nn::util::Vector3fType* pOutVec ) const NN_NOEXCEPT
{
    nn::util::Vector3fType localVec;
    nn::util::VectorLoad( &localVec, pEmitter->m_CpuParticleProperty.localVec[ m_ParticleIndex ].v );

    // ワールド速度に変換して返す。
    TransformToWorldVec( pOutVec, localVec );
}

//---------------------------------------------------------------------------
//! @brief  回転量を計算
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::CalculateCurrentRotateRadian( nn::util::Vector3fType* pOutRotate ) const NN_NOEXCEPT
{
    detail::EmitterCalculator* pCalculator = pEmitter->GetEmitterCalculator();
    nn::util::Vector4fType rotate, random;
    nn::util::VectorLoad( &rotate, pEmitter->m_CpuParticleProperty.initRotate[ m_ParticleIndex ] );
    nn::util::VectorLoad( &random, pEmitter->m_CpuParticleProperty.random[ m_ParticleIndex ] );
    pCalculator->CalculateRotationMatrix( pOutRotate,
        pEmitter->m_pEmitterRes,
        rotate,
        random,
        m_ParticleTime );
}

//---------------------------------------------------------------------------
//! @brief  回転量を計算
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::CalculateCurrentRotateDegree( nn::util::Vector3fType* pOutRotate ) const NN_NOEXCEPT
{
    CalculateCurrentRotateRadian( pOutRotate );
    nn::util::VectorSetX( pOutRotate, nn::util::RadianToDegree( nn::util::VectorGetX( *pOutRotate ) ) );
    nn::util::VectorSetY( pOutRotate, nn::util::RadianToDegree( nn::util::VectorGetY( *pOutRotate ) ) );
    nn::util::VectorSetZ( pOutRotate, nn::util::RadianToDegree( nn::util::VectorGetZ( *pOutRotate ) ) );
}

//---------------------------------------------------------------------------
//! @brief  初期スケールを取得
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::GetInitialScale( nn::util::Vector3fType* pOutScale ) const NN_NOEXCEPT
{
    nn::util::VectorLoad( pOutScale, pEmitter->m_CpuParticleProperty.scale[ m_ParticleIndex ].v );
}

//---------------------------------------------------------------------------
//! @brief  ワールドスケール値を計算
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::CalculateWorldScale( nn::util::Vector3fType* pOutScale ) const NN_NOEXCEPT
{
    nn::util::Vector4fType random;
    nn::util::VectorLoad( &random, pEmitter->m_CpuParticleProperty.random[ m_ParticleIndex ] );
    nn::util::Vector4fType scale;
    nn::util::VectorLoad( &scale, pEmitter->m_CpuParticleProperty.scale[ m_ParticleIndex ] );

    detail::EmitterCalculator* calc = pEmitter->GetEmitterCalculator();
    calc->CalculateParticleScaleVecFromTime( pOutScale, pEmitter->m_pEmitterRes, scale, random, m_ParticleLife, m_ParticleTime );
    nn::util::VectorMultiply( pOutScale, *pOutScale, pEmitter->m_pEmitterSet->GetParticleScaleForCalculation() );
}

//---------------------------------------------------------------------------
//! @brief  カラー0を計算
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::CalculateLocalColor0( nn::util::Vector4fType* pOutColor ) const NN_NOEXCEPT
{
    nn::util::Vector4fType random;
    nn::util::VectorLoad( &random, pEmitter->m_CpuParticleProperty.random[ m_ParticleIndex ] );
    detail::EmitterCalculator* pCalculator = pEmitter->GetEmitterCalculator();

    nn::util::Vector3fType color0Anim;
    nn::util::VectorLoad( &color0Anim, pEmitter->m_EmitterAnimation.color0 );

    pCalculator->CalculateParticleColor0VecFromTime( pOutColor,
        pEmitter->m_pEmitterRes,
        random,
        pEmitter->m_Color0,
        color0Anim,
        pEmitter->m_EmitterAnimation.alpha0.x,
        m_ParticleLife, m_ParticleTime );
}

//---------------------------------------------------------------------------
//! @brief  カラー1を計算
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::CalculateLocalColor1( nn::util::Vector4fType* pOutColor ) const NN_NOEXCEPT
{
    nn::util::Vector4fType random;
    nn::util::VectorLoad( &random, pEmitter->m_CpuParticleProperty.random[ m_ParticleIndex ] );
    detail::EmitterCalculator* pCalculator = pEmitter->GetEmitterCalculator();

    nn::util::Vector3fType color1Anim;
    nn::util::VectorLoad( &color1Anim, pEmitter->m_EmitterAnimation.color1 );

    pCalculator->CalculateParticleColor1VecFromTime( pOutColor,
        pEmitter->m_pEmitterRes,
        random,
        pEmitter->m_Color1,
        color1Anim,
        pEmitter->m_EmitterAnimation.alpha1.x,
        m_ParticleLife, m_ParticleTime );
}

//---------------------------------------------------------------------------
//! @brief  ランダムを取得
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::GetRandom( nn::util::Vector4fType* pOutRandom ) const NN_NOEXCEPT
{
    nn::util::VectorLoad( pOutRandom, pEmitter->m_CpuParticleProperty.random[ m_ParticleIndex ] );
}

//---------------------------------------------------------------------------
//! @brief パーティクルを削除する
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::Kill( bool keepChildEmitter ) NN_NOEXCEPT
{
    // 寿命を縮める
    pEmitter->m_pParticleData[ m_ParticleIndex ].life = 0;
    pEmitter->m_CpuParticleProperty.localPos[ m_ParticleIndex ].w = 0.0f;

    // チャイルドを残す場合は、以降何もしない
    if( keepChildEmitter )
    {
        return;
    }

    // 削除されたパーティクルが親パーティクルであれば、
    // 付随するチャイルドエミッタも削除する
    detail::ParentParticleData* pParentParticleData = &pEmitter->m_pParentParticleData[ m_ParticleIndex ];
    if ( !pParentParticleData )
    {
        return;
    }

    for ( int j = 0; j < pEmitter->m_pEmitterRes->m_ChildEmitterResCount; j++ )
    {
        Emitter* pChildEmitter = pParentParticleData->pChildEmitter[ j ];
        if ( !pChildEmitter )
        {
            continue;
        }

        pChildEmitter->Kill();
    }
}

//---------------------------------------------------------------------------
//! @brief  ローカル速度に変換
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::TransformToLocalVec( nn::util::Vector3fType* pOutDst, const nn::util::Vector3fType& src ) const NN_NOEXCEPT
{
    pEmitter->TransformToLocalVec( pOutDst, src, m_ParticleIndex );
}

//---------------------------------------------------------------------------
//! @brief  ワールド速度に変換
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::TransformToWorldVec( nn::util::Vector3fType* pOutDst, const nn::util::Vector3fType& src ) const NN_NOEXCEPT
{
    pEmitter->TransformToWorldVec( pOutDst, src, m_ParticleIndex );
}

//---------------------------------------------------------------------------
//! @brief  ローカル位置に変換
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::TransformToLocalPos( nn::util::Vector3fType* pOutDst, const nn::util::Vector3fType& src ) const NN_NOEXCEPT
{
    pEmitter->TransformToLocalPos( pOutDst, src, m_ParticleIndex );
}

//---------------------------------------------------------------------------
//! @brief  ローカル位置に変換
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::TransformToLocalPos( nn::util::Vector4fType* pOutDst, const nn::util::Vector4fType& src ) const NN_NOEXCEPT
{
    nn::util::Vector3fType d, s;
    detail::Vector3fCopy( &s, src );
    pEmitter->TransformToLocalPos( &d, s, m_ParticleIndex );
    detail::Vector3fCopy( pOutDst, d );
}

//---------------------------------------------------------------------------
//! @brief  ワールド位置に変換
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::TransformToWorldPos( nn::util::Vector3fType* pOutDst, const nn::util::Vector3fType& src ) const NN_NOEXCEPT
{
    pEmitter->TransformToWorldPos( pOutDst, src, m_ParticleIndex );
}

//---------------------------------------------------------------------------
//! @brief  ワールド位置に変換
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::TransformToWorldPos( nn::util::Vector4fType* pOutDst, const nn::util::Vector4fType& src ) const NN_NOEXCEPT
{

    nn::util::Vector3fType d, s;
    detail::Vector3fCopy( &s, src );
    pEmitter->TransformToWorldPos( &d, s, m_ParticleIndex );
    detail::Vector3fCopy( pOutDst, d );
}

//---------------------------------------------------------------------------
//! @brief  ローカル位置を設定
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::SetLocalPos( float x, float y, float z ) NN_NOEXCEPT
{
    pEmitter->m_CpuParticleProperty.localPos[ m_ParticleIndex ].x = x;
    pEmitter->m_CpuParticleProperty.localPos[ m_ParticleIndex ].y = y;
    pEmitter->m_CpuParticleProperty.localPos[ m_ParticleIndex ].z = z;
}

//---------------------------------------------------------------------------
//! @brief  ローカル位置を設定
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::SetLocalPos( const nn::util::Vector3fType& localPos ) NN_NOEXCEPT
{
    nn::util::VectorStore( pEmitter->m_CpuParticleProperty.localPos[ m_ParticleIndex ].v, localPos );
}

//---------------------------------------------------------------------------
//! @brief  ワールド位置を設定
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::SetWorldPos( float worldX, float worldY, float worldZ ) NN_NOEXCEPT
{
    // ローカル位置に戻して保存
    nn::util::Vector3fType localPos;
    nn::util::Vector3fType worldPos = NN_UTIL_VECTOR_3F_INITIALIZER( worldX, worldY, worldZ );
    TransformToLocalPos( &localPos, worldPos );
    nn::util::VectorStore( pEmitter->m_CpuParticleProperty.localPos[ m_ParticleIndex ].v, localPos );
}

//---------------------------------------------------------------------------
//! @brief  ワールド位置を設定
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::SetWorldPos( const nn::util::Vector3fType& worldPos ) NN_NOEXCEPT
{
    // ローカル位置に戻して保存
    nn::util::Vector3fType localPos;
    TransformToLocalPos( &localPos, worldPos );
    nn::util::VectorStore( pEmitter->m_CpuParticleProperty.localPos[ m_ParticleIndex ].v, localPos );
}

//---------------------------------------------------------------------------
//! @brief  ローカル速度を設定
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::SetLocalVec( float x, float y, float z ) NN_NOEXCEPT
{
    pEmitter->m_CpuParticleProperty.localVec[ m_ParticleIndex ].x = x;
    pEmitter->m_CpuParticleProperty.localVec[ m_ParticleIndex ].y = y;
    pEmitter->m_CpuParticleProperty.localVec[ m_ParticleIndex ].z = z;
}

//---------------------------------------------------------------------------
//! @brief  ローカル速度を設定
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::SetLocalVec( const nn::util::Vector3fType& localVec ) NN_NOEXCEPT
{
    nn::util::VectorStore( pEmitter->m_CpuParticleProperty.localVec[ m_ParticleIndex ].v, localVec );
}

//---------------------------------------------------------------------------
//! @brief  ワールド速度を設定
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::SetWorldVec( float x, float y, float z ) NN_NOEXCEPT
{
    nn::util::Vector3fType localVec;
    nn::util::Vector3fType worldVec = NN_UTIL_VECTOR_3F_INITIALIZER( x, y, z );
    TransformToLocalVec( &localVec, worldVec );
    nn::util::VectorStore( pEmitter->m_CpuParticleProperty.localVec[ m_ParticleIndex ].v, localVec );
}

//---------------------------------------------------------------------------
//! @brief  ワールド速度を設定
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::SetWorldVec( const nn::util::Vector3fType& worldVec ) NN_NOEXCEPT
{
    // ローカル速度に戻して保存。
    nn::util::Vector3fType localVec;
    TransformToLocalVec( &localVec, worldVec );
    nn::util::VectorStore( pEmitter->m_CpuParticleProperty.localVec[ m_ParticleIndex ].v, localVec );
}

//---------------------------------------------------------------------------
//! @brief  初期スケールを設定
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::SetInitialScale( const nn::util::Vector3fType& scale ) NN_NOEXCEPT
{
    nn::util::VectorStore( pEmitter->m_CpuParticleProperty.scale[ m_ParticleIndex ].v, scale );
    detail::MemUtil::FlushCache( &pEmitter->m_CpuParticleProperty.scale[ m_ParticleIndex ], sizeof( nn::util::Float3 ) );
}

//---------------------------------------------------------------------------
//! @brief  初期スケールXを設定
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::SetInitialScaleX( float scaleX ) NN_NOEXCEPT
{
    pEmitter->m_CpuParticleProperty.scale[ m_ParticleIndex ].x = scaleX;
    detail::MemUtil::FlushCache( &pEmitter->m_CpuParticleProperty.scale[ m_ParticleIndex ], sizeof( nn::util::Float2 ) );
}

//---------------------------------------------------------------------------
//! @brief  初期スケールYを設定
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::SetInitialScaleY( float scaleY ) NN_NOEXCEPT
{
    pEmitter->m_CpuParticleProperty.scale[ m_ParticleIndex ].y = scaleY;
    detail::MemUtil::FlushCache( &pEmitter->m_CpuParticleProperty.scale[ m_ParticleIndex ], sizeof( nn::util::Float2 ) );
}

//---------------------------------------------------------------------------
//! @brief  初期回転量を設定
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::SetInitialRotateRadian( const nn::util::Vector3fType& rotate ) NN_NOEXCEPT
{
    nn::util::VectorStore( pEmitter->m_CpuParticleProperty.initRotate[ m_ParticleIndex ].v, rotate );
    detail::MemUtil::FlushCache( &pEmitter->m_CpuParticleProperty.initRotate[ m_ParticleIndex ], sizeof( nn::util::Float3 ) );
}

//---------------------------------------------------------------------------
//! @brief  初期回転量を設定
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::SetInitialRotateDegree( const nn::util::Vector3fType& rotate ) NN_NOEXCEPT
{
    pEmitter->m_CpuParticleProperty.initRotate[ m_ParticleIndex ].x = nn::util::DegreeToRadian( nn::util::VectorGetX( rotate ) );
    pEmitter->m_CpuParticleProperty.initRotate[ m_ParticleIndex ].y = nn::util::DegreeToRadian( nn::util::VectorGetY( rotate ) );
    pEmitter->m_CpuParticleProperty.initRotate[ m_ParticleIndex ].z = nn::util::DegreeToRadian( nn::util::VectorGetZ( rotate ) );
    detail::MemUtil::FlushCache( &pEmitter->m_CpuParticleProperty.initRotate[ m_ParticleIndex ], sizeof( nn::util::Float3 ) );
}

//---------------------------------------------------------------------------
//! @brief  初期回転量を取得
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::GetInitialRotateRadian( nn::util::Vector3fType* pOutRotate ) const NN_NOEXCEPT
{
    nn::util::VectorLoad( pOutRotate, pEmitter->m_CpuParticleProperty.initRotate[ m_ParticleIndex ].v );
}

//---------------------------------------------------------------------------
//! @brief  初期回転量を取得
//---------------------------------------------------------------------------
inline void ParticleCalculateArg::GetInitialRotateDegree( nn::util::Vector3fType* pOutRotate ) const NN_NOEXCEPT
{
    GetInitialRotateRadian( pOutRotate );
    nn::util::VectorSetX( pOutRotate, nn::util::RadianToDegree( nn::util::VectorGetX( *pOutRotate ) ) );
    nn::util::VectorSetY( pOutRotate, nn::util::RadianToDegree( nn::util::VectorGetY( *pOutRotate ) ) );
    nn::util::VectorSetZ( pOutRotate, nn::util::RadianToDegree( nn::util::VectorGetZ( *pOutRotate ) ) );
}

//------------------------------------------------------------------------------
//  パーティクル位置をワールド座標系に変換するための、追従設定を加味した変換行列を取得します。
//------------------------------------------------------------------------------
inline void ParticleCalculateArg::GetEmitterTransformMatrix( nn::util::Matrix4x3f* pOutMatrix ) const NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL( pOutMatrix );
    pEmitter->GetEmitterTransformMatrix( pOutMatrix, m_ParticleIndex );
}

} // namespace vfx
} // namespace nn
