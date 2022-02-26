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
#include <nn/vfx/vfx_EmitterRes.h>

namespace nn {
namespace vfx {
namespace detail {

//---------------------------------------------------
//! @brief  シェーダフラグ。
//---------------------------------------------------
class ShaderFlag
{
    NN_DISALLOW_COPY( ShaderFlag );
private:

    //---------------------------------------------------------------------------
    //! @brief  シェーダフラグ0
    //---------------------------------------------------------------------------
    enum ShaderBitFlag0
    {
        BIT_FLAG_FLUCTUATION_TYPE_SIN       = ( 0x01 << 0 ),       //!< TBD
        BIT_FLAG_FLUCTUATION_TYPE_SAW_TOOTH = ( 0x01 << 1 ),       //!< TBD
        BIT_FLAG_FLUCTUATION_TYPE_RECT      = ( 0x01 << 2 ),       //!< TBD
        BIT_FLAG_WORLD_GRAVITY_ENABLED      = ( 0x01 << 3 ),       //!< TBD
        BIT_FLAG_TEX0_PTN_LIFE_FIT          = ( 0x01 << 4 ),       //!< TBD
        BIT_FLAG_TEX0_PTN_LIFE_CLAMP        = ( 0x01 << 5 ),       //!< TBD
        BIT_FLAG_TEX0_PTN_LIFE_LOOP         = ( 0x01 << 6 ),       //!< TBD
        BIT_FLAG_TEX0_PTN_LIFE_RANDOM       = ( 0x01 << 7 ),       //!< TBD
        BIT_FLAG_TEX1_PTN_LIFE_FIT          = ( 0x01 << 8 ),       //!< TBD
        BIT_FLAG_TEX1_PTN_LIFE_CLAMP        = ( 0x01 << 9 ),       //!< TBD
        BIT_FLAG_TEX1_PTN_LIFE_LOOP         = ( 0x01 << 10 ),      //!< TBD
        BIT_FLAG_TEX1_PTN_LIFE_RANDOM       = ( 0x01 << 11 ),      //!< TBD
        BIT_FLAG_TEX2_PTN_LIFE_FIT          = ( 0x01 << 12 ),      //!< TBD
        BIT_FLAG_TEX2_PTN_LIFE_CLAMP        = ( 0x01 << 13 ),      //!< TBD
        BIT_FLAG_TEX2_PTN_LIFE_LOOP         = ( 0x01 << 14 ),      //!< TBD
        BIT_FLAG_TEX2_PTN_LIFE_RANDOM       = ( 0x01 << 15 ),      //!< TBD
        BIT_FLAG_ROTATE_DIR_RANDOM_X        = ( 0x01 << 16 ),      //!< TBD
        BIT_FLAG_ROTATE_DIR_RANDOM_Y        = ( 0x01 << 17 ),      //!< TBD
        BIT_FLAG_ROTATE_DIR_RANDOM_Z        = ( 0x01 << 18 ),      //!< TBD
        BIT_FLAG_TEX0_INV_RANDOM_U          = ( 0x01 << 19 ),      //!< TBD
        BIT_FLAG_TEX0_INV_RANDOM_V          = ( 0x01 << 20 ),      //!< TBD
        BIT_FLAG_TEX1_INV_RANDOM_U          = ( 0x01 << 21 ),      //!< TBD
        BIT_FLAG_TEX1_INV_RANDOM_V          = ( 0x01 << 22 ),      //!< TBD
        BIT_FLAG_TEX2_INV_RANDOM_U          = ( 0x01 << 23 ),      //!< TBD
        BIT_FLAG_TEX2_INV_RANDOM_V          = ( 0x01 << 24 ),      //!< TBD
        BIT_FLAG_TEX0_PTN_LOOP_RANDOM       = ( 0x01 << 25 ),      //!< TBD
        BIT_FLAG_TEX1_PTN_LOOP_RANDOM       = ( 0x01 << 26 ),      //!< TBD
        BIT_FLAG_TEX2_PTN_LOOP_RANDOM       = ( 0x01 << 27 ),      //!< TBD
        BIT_FLAG_PRIMITIVE_SCALE_Y_TO_Z     = ( 0x01 << 28 ),      //!< TBD
        BIT_FLAG_OFFSET_TYPE_DEPTH          = ( 0x01 << 29 ),      //!< TBD
        BIT_FLAG_OFFSET_TYPE_CAMERA         = ( 0x01 << 30 ),      //!< TBD
    };

    //---------------------------------------------------------------------------
    //! @brief  シェーダフラグ1
    //---------------------------------------------------------------------------
    enum ShaderBitFlag1
    {
        BIT_FLAG_TEX_ANIM_FIXED_RANDOM      = ( 0x01 << 0 ),       //!< TBD
        BIT_FLAG_FIELD_RANDOM               = ( 0x01 << 1 ),       //!< TBD
        BIT_FLAG_FIELD_POSADD               = ( 0x01 << 2 ),       //!< TBD
        BIT_FLAG_FIELD_MAGNET               = ( 0x01 << 3 ),       //!< TBD
        BIT_FLAG_FIELD_CONVERGENCE          = ( 0x01 << 4 ),       //!< TBD
        BIT_FLAG_FIELD_SPIN                 = ( 0x01 << 5 ),       //!< TBD
        BIT_FLAG_FIELD_COLLISION            = ( 0x01 << 6 ),       //!< TBD
        BIT_FLAG_FIELD_CURLNOISE            = ( 0x01 << 7 ),       //!< TBD
        BIT_FLAG_FIELD_RANDOM_FE1           = ( 0x01 << 8 ),       //!< TBD

        BIT_FLAG_EMITTER_FOLLOW_ALL         = ( 0x01 << 9 ),       //!< エミッタ追従ライプ : 完全
        BIT_FLAG_EMITTER_FOLLOW_POS         = ( 0x01 << 10 ),      //!< エミッタ追従ライプ : 位置のみ
        BIT_FLAG_EMITTER_FOLLOW_NONE        = ( 0x01 << 11 ),      //!< エミッタ追従ライプ : 位置なし
    };

public:
    ShaderFlag() NN_NOEXCEPT
    {
    }
    //---------------------------------------------------------------------------
    //! @brief  無効化
    //---------------------------------------------------------------------------
    void Invalidate() NN_NOEXCEPT
    {
        m_Flag0 = 0; m_Flag1 = 0;
    }

    //---------------------------------------------------------------------------
    //! @brief  シェーダフラグ0を取得します。
    //! @return シェーダフラグ0
    //---------------------------------------------------------------------------
    uint32_t GetFlag0() const NN_NOEXCEPT
    {
        return m_Flag0;
    }

    //---------------------------------------------------------------------------
    //! @brief  シェーダフラグ1を取得します。
    //! @return シェーダフラグ1
    //---------------------------------------------------------------------------
    uint32_t GetFlag1() const NN_NOEXCEPT
    {
        return m_Flag1;
    }

    //---------------------------------------------------------------------------
    //! @brief  初期化
    //! @param[in] pEmitterResource EmitterResourceへのポインタ
    //---------------------------------------------------------------------------
    void Initialize( EmitterResource* pEmitterResource ) NN_NOEXCEPT
    {
        ResEmitter* pResEmitter = pEmitterResource->m_pResEmitter;

        // 重力世界方向
        if( pResEmitter->emission.isWorldGravity )
        {
            m_Flag0 |= BIT_FLAG_WORLD_GRAVITY_ENABLED;
        }

        // 波形タイプ
        ParticleFluctuationWaveType waveType = static_cast< ParticleFluctuationWaveType >( pResEmitter->ptclFluctuation.isWaveType >> 4 );
        if( waveType == ParticleFluctuationWaveType_Sin )
        {
            m_Flag0 |= BIT_FLAG_FLUCTUATION_TYPE_SIN;
        }
        if( waveType == ParticleFluctuationWaveType_SawTooth )
        {
            m_Flag0 |= BIT_FLAG_FLUCTUATION_TYPE_SAW_TOOTH;
        }
        if( waveType == ParticleFluctuationWaveType_Rectangle )
        {
            m_Flag0 |= BIT_FLAG_FLUCTUATION_TYPE_RECT;
        }

        // テクスチャ パタンアニメーションタイプ
        if( pResEmitter->textureAnim0.patternAnimType == TexturePatternAnimationType_LifeFit )
        {
            m_Flag0 |= BIT_FLAG_TEX0_PTN_LIFE_FIT;
        }
        if( pResEmitter->textureAnim0.patternAnimType == TexturePatternAnimationType_Clamp )
        {
            m_Flag0 |= BIT_FLAG_TEX0_PTN_LIFE_CLAMP;
        }
        if( pResEmitter->textureAnim0.patternAnimType == TexturePatternAnimationType_Loop )
        {
            m_Flag0 |= BIT_FLAG_TEX0_PTN_LIFE_LOOP;
        }
        if( pResEmitter->textureAnim0.patternAnimType == TexturePatternAnimationType_Random )
        {
            for( int i = 0; i < pResEmitter->staticUbo.texPtnAnim0.ptnNum; i++ )
            {
                pResEmitter->staticUbo.texPtnAnim0.ptnTable[ i ] = i;
            }
            pResEmitter->staticUbo.texPtnAnim0.ptnTableNum =
                pResEmitter->staticUbo.texPtnAnim0.ptnNum;
            m_Flag0 |= BIT_FLAG_TEX0_PTN_LIFE_RANDOM;
        }

        if( pResEmitter->textureAnim1.patternAnimType == TexturePatternAnimationType_LifeFit )
        {
            m_Flag0 |= BIT_FLAG_TEX1_PTN_LIFE_FIT;
        }
        if( pResEmitter->textureAnim1.patternAnimType == TexturePatternAnimationType_Clamp )
        {
            m_Flag0 |= BIT_FLAG_TEX1_PTN_LIFE_CLAMP;
        }
        if( pResEmitter->textureAnim1.patternAnimType == TexturePatternAnimationType_Loop )
        {
            m_Flag0 |= BIT_FLAG_TEX1_PTN_LIFE_LOOP;
        }
        if( pResEmitter->textureAnim1.patternAnimType == TexturePatternAnimationType_Random )
        {
            for( int i = 0; i < pResEmitter->staticUbo.texPtnAnim1.ptnNum; i++ )
            {
                pResEmitter->staticUbo.texPtnAnim1.ptnTable[ i ] = i;
            }
            pResEmitter->staticUbo.texPtnAnim1.ptnTableNum =
                pResEmitter->staticUbo.texPtnAnim1.ptnNum;
            m_Flag0 |= BIT_FLAG_TEX1_PTN_LIFE_RANDOM;
        }

        if( pResEmitter->textureAnim2.patternAnimType == TexturePatternAnimationType_LifeFit )
        {
            m_Flag0 |= BIT_FLAG_TEX2_PTN_LIFE_FIT;
        }
        if( pResEmitter->textureAnim2.patternAnimType == TexturePatternAnimationType_Clamp )
        {
            m_Flag0 |= BIT_FLAG_TEX2_PTN_LIFE_CLAMP;
        }
        if( pResEmitter->textureAnim2.patternAnimType == TexturePatternAnimationType_Loop )
        {
            m_Flag0 |= BIT_FLAG_TEX2_PTN_LIFE_LOOP;
        }
        if( pResEmitter->textureAnim2.patternAnimType == TexturePatternAnimationType_Random )
        {
            for( int i = 0; i < pResEmitter->staticUbo.texPtnAnim2.ptnNum; i++ )
            {
                pResEmitter->staticUbo.texPtnAnim2.ptnTable[ i ] = i;
            }
            pResEmitter->staticUbo.texPtnAnim2.ptnTableNum =
                pResEmitter->staticUbo.texPtnAnim2.ptnNum;
            m_Flag0 |= BIT_FLAG_TEX2_PTN_LIFE_RANDOM;
        }

        // 回転方向ランダム
        if( pResEmitter->ptcl.rotRevRandX )
        {
            m_Flag0 |= BIT_FLAG_ROTATE_DIR_RANDOM_X;
        }
        if( pResEmitter->ptcl.rotRevRandY )
        {
            m_Flag0 |= BIT_FLAG_ROTATE_DIR_RANDOM_Y;
        }
        if( pResEmitter->ptcl.rotRevRandZ )
        {
            m_Flag0 |= BIT_FLAG_ROTATE_DIR_RANDOM_Z;
        }

        // テクスチャ反転ランダム
        if( pResEmitter->textureAnim0.invRandU )
        {
            m_Flag0 |= BIT_FLAG_TEX0_INV_RANDOM_U;
        }
        if( pResEmitter->textureAnim0.invRandV )
        {
            m_Flag0 |= BIT_FLAG_TEX0_INV_RANDOM_V;
        }
        if( pResEmitter->textureAnim1.invRandU )
        {
            m_Flag0 |= BIT_FLAG_TEX1_INV_RANDOM_U;
        }
        if( pResEmitter->textureAnim1.invRandV )
        {
            m_Flag0 |= BIT_FLAG_TEX1_INV_RANDOM_V;
        }
        if( pResEmitter->textureAnim2.invRandU )
        {
            m_Flag0 |= BIT_FLAG_TEX2_INV_RANDOM_U;
        }
        if( pResEmitter->textureAnim2.invRandV )
        {
            m_Flag0 |= BIT_FLAG_TEX2_INV_RANDOM_V;
        }

        // テクスチャパタン ループランダム
        if( pResEmitter->textureAnim0.isPatAnimLoopRandom )
        {
            m_Flag0 |= BIT_FLAG_TEX0_PTN_LOOP_RANDOM;
        }
        if( pResEmitter->textureAnim1.isPatAnimLoopRandom )
        {
            m_Flag0 |= BIT_FLAG_TEX1_PTN_LOOP_RANDOM;
        }
        if( pResEmitter->textureAnim2.isPatAnimLoopRandom )
        {
            m_Flag0 |= BIT_FLAG_TEX2_PTN_LOOP_RANDOM;
        }

        // プリミティブスケール適用
        if( pResEmitter->ptcl.primitiveScaleType )
        {
            m_Flag0 |= BIT_FLAG_PRIMITIVE_SCALE_Y_TO_Z;
        }

        // オフセットタイプ
        if( pResEmitter->ptcl.offsetType == ParticleOffsetType_Camera )
        {
            m_Flag0 |= BIT_FLAG_OFFSET_TYPE_CAMERA;
        }
        if( pResEmitter->ptcl.offsetType == ParticleOffsetType_Depth )
        {
            m_Flag0 |= BIT_FLAG_OFFSET_TYPE_DEPTH;
        }

        // flag1
        // テクスチャ共通ランダム
        if( pResEmitter->ptcl.isTextureCommonRandom )
        {
            m_Flag1 |= BIT_FLAG_TEX_ANIM_FIXED_RANDOM;
        }

        // フィールド
        if( pEmitterResource->m_pFieldGpuNoiseData )
        {
            m_Flag1 |= BIT_FLAG_FIELD_RANDOM;
        }
        if( pEmitterResource->m_pFieldRandomSimpleData )
        {
            m_Flag1 |= BIT_FLAG_FIELD_RANDOM_FE1;
        }
        if( pEmitterResource->m_pFieldPosAddData )
        {
            m_Flag1 |= BIT_FLAG_FIELD_POSADD;
        }
        if( pEmitterResource->m_pFieldMagnetData )
        {
            m_Flag1 |= BIT_FLAG_FIELD_MAGNET;
        }
        if( pEmitterResource->m_pFieldConvergenceData )
        {
            m_Flag1 |= BIT_FLAG_FIELD_CONVERGENCE;
        }
        if( pEmitterResource->m_pFieldSpinData )
        {
            m_Flag1 |= BIT_FLAG_FIELD_SPIN;
        }
        if( pEmitterResource->m_pFieldCollisionData )
        {
            m_Flag1 |= BIT_FLAG_FIELD_COLLISION;
        }
        if( pEmitterResource->m_pFieldCurlNoiseData )
        {
            m_Flag1 |= BIT_FLAG_FIELD_CURLNOISE;
        }

        // エミッタ追従
        switch( pResEmitter->emitter.followType )
        {
        case ParticleFollowType_EmitterFull:
            m_Flag1 |= BIT_FLAG_EMITTER_FOLLOW_ALL;
            break;
        case ParticleFollowType_EmitterPosition:
            m_Flag1 |= BIT_FLAG_EMITTER_FOLLOW_POS;
            break;
        case ParticleFollowType_None:
            m_Flag1 |= BIT_FLAG_EMITTER_FOLLOW_NONE;
            break;
        default:
            NN_SDK_ASSERT( 0 );
            break;
        }

    }   // NOLINT(readability/fn_size)

private:
    uint32_t     m_Flag0;    //!< TBD
    uint32_t     m_Flag1;    //!< TBD
};

} // namespace detail
} // namespace vfx
} // namespace nn
