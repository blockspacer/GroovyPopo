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

namespace nn {
namespace vfx {

class Emitter;

namespace detail {


//------------------------------------------------------------------------------
//! @brief        パーティクルプロパティ
//!               GPUに送られる1パーティクル( スケール / カラー / 回転 / 生成時親マトリクス )情報。
//------------------------------------------------------------------------------
struct ParticleProperty
{
    nn::util::Float4*   localPos;               //!< ローカル座標 ( xyz / life )
    nn::util::Float4*   localVec;               //!< ローカル速度 ( xyz / birth time )
    nn::util::Float4*   localDiff;              //!< ローカル差分
    nn::util::Float4*   scale;                  //!< スケール（ x / y / birth time / 運動量ランダム ）
    nn::util::Float4*   random;                 //!< ランダム
    nn::util::Float4*   initRotate;             //!< 初期回転値
    nn::util::Float4*   initColor0;             //!< 初期カラー0
    nn::util::Float4*   initColor1;             //!< 初期カラー1
    nn::util::Float4*   emitterMatrixSrt0;      //!< エミッタマトリクス0（列優先）
    nn::util::Float4*   emitterMatrixSrt1;      //!< エミッタマトリクス1（列優先）
    nn::util::Float4*   emitterMatrixSrt2;      //!< エミッタマトリクス2（列優先）
};


//------------------------------------------------------------------------------
//! @brief        パーティクルデータ
//!
//!               アトリビュート以外のパーティクルの情報。
//------------------------------------------------------------------------------
struct ParticleData
{
    int                 createId;               //!< 生成ID
    int                 collisionCount;         //!< [フィールド]コリジョン回数
    float               createTime;             //!< 生成時刻（CPUアクセス用）
    float               life;                   //!< 寿命（CPUアクセス用）
    void*               pUserData;              //!< ユーザーデータ
    void*               pEmitterPluginData;     //!< エミッタプラグインが利用するユーザーデータ

    //------------------------------------------------------------------------------
    //! @brief                  パーティクル時刻を取得
    //! @param[in] emitterTime  エミッタ時刻
    //! @return                 パーティクル時刻
    //------------------------------------------------------------------------------
    float GetTime( float emitterTime ) const NN_NOEXCEPT
    {
        return ( emitterTime - createTime );
    }

    //------------------------------------------------------------------------------
    //! @brief      パーティクルの寿命を取得
    //! @return     パーティクルの寿命
    //------------------------------------------------------------------------------
    float GetLife() const NN_NOEXCEPT
    {
        return life;
    }

    //------------------------------------------------------------------------------
    //! @brief                  パーティクルが生存しているかどうかチェック
    //! @param[in] emitterTime      エミッタ時間
    //! @return                 生存している場合true
    //------------------------------------------------------------------------------
    bool IsAlive( float emitterTime ) const NN_NOEXCEPT
    {
        return ( GetTime( emitterTime ) < GetLife() );
    }
};

//------------------------------------------------------------------------------
//! @brief        親パーティクルデータ
//!
//!               チャイルドを持つパーティクル固有のパーティクルの情報。必要なときのみ ParticleData と同じサイズの配列として確保される。
//------------------------------------------------------------------------------
struct ParentParticleData
{
    Emitter*    pChildEmitter[ SystemParameters_MaxEmitterInclusionCount ];             //!< チャイルドエミッタ
    float       lightChildEmitterTime;                                                  //!< 軽量版チャイルドのエミッタ時間
    float       lightChildEmitterLife;                                                  //!< 軽量版チャイルドのエミッタ（パーティクル）寿命
    float       lightChildEmitCount[ SystemParameters_MaxEmitterInclusionCount ];       //!< 軽量版チャイルドの放出カウンタ
    float       lightChildEmitSaving[ SystemParameters_MaxEmitterInclusionCount ];      //!< 軽量版チャイルドの放出貯蓄
    float       lightChildEmitInterval[ SystemParameters_MaxEmitterInclusionCount ];    //!< 軽量版チャイルドの放出間隔（初回放出のための特殊処理に必要）
    uint8_t     lightChildEmitDone[ SystemParameters_MaxEmitterInclusionCount ];        //!< 軽量版チャイルド: 放出終了フラグ
};

} // namespace detail
} // namespace vfx
} // namespace nn
