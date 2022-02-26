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

#include <nn/vfx/vfx_Enum.h>
#include <nn/vfx/vfx_Misc.h>
#include <nn/vfx/vfx_Particle.h>
#include <nn/vfx/vfx_Random.h>
#include <nn/vfx/vfx_Binary.h>
#include <nn/vfx/vfx_Resource.h>
#include <nn/vfx/vfx_Shader.h>
#include <nn/vfx/vfx_EmitterRes.h>
#include <nn/vfx/vfx_BufferAllocator.h>

namespace nn {
namespace vfx {

class  System;
class EmitterSet;
class CallbackSet;
struct RenderStateSetArg;
typedef bool( *RenderStateSetCallback )( RenderStateSetArg& arg );
typedef void( *DrawPathRenderStateSetCallback )( RenderStateSetArg& arg );

namespace detail {
class  EmitterCalculator;
//------------------------------------------------------------------------------
//! @brief      エミッタ挙動計算タイプの enum を文字列に変換します。
//! @param[in]  mode エミッタ計算タイプ
//! @return     エミッタ計算の文字列
//------------------------------------------------------------------------------
inline const char* ToString( EmitterCalculationMode mode ) NN_NOEXCEPT
{
    NN_SDK_REQUIRES_RANGE( mode, 0, EmitterCalculationMode_MaxMode );
    switch( mode )
    {
    case EmitterCalculationMode_Cpu:          return "CPU";
    case EmitterCalculationMode_Gpu:          return "GPU";
    case EmitterCalculationMode_GpuStreamOut: return "SO";
    default:
        NN_SDK_ASSERT( 0 );
    }
    return NULL;
}
}

//---------------------------------------------------
//! @brief  マニュアルエミッタ用 放出予約用構造体
//---------------------------------------------------
struct EmitReservationInfo
{
    bool                     isEmitMatrix;  //!< エミッタマトリクスが設定された
    nn::util::Matrix4x3fType emitParam;     //!< 放出パラメータ
    void*                    pUserData;     //!< ユーザーデータへのポインタ
    float                    emissionRatio; //!< EmitParticle()を呼んだ時点での放出レート補正値
};

//------------------------------------------------------------------------------
//! @brief  エミッタ時間アニメーション 計算結果格納用構造体
//------------------------------------------------------------------------------
struct EmitterAnimValue
{
    nn::util::Float3        scale;                  //!< スケール
    nn::util::Float3        rotate;                 //!< 回転
    nn::util::Float3        translate;              //!< 平行移動
    nn::util::Float3        color0;                 //!< カラー0
    nn::util::Float3        color1;                 //!< カラー1
    union
    {
        nn::util::Float3    emissionRateVec;        //!< 放出レート
        float               emissionRate;           //!< 放出レート
    };
    union
    {
        nn::util::Float3    particleLifeVec;        //!< パーティクル寿命
        float               particleLife;           //!< パーティクル寿命
    };
    nn::util::Float3        alpha0;                 //!< アルファ0
    nn::util::Float3        alpha1;                 //!< アルファ1
    union
    {
        nn::util::Float3    allDirVelocityVec;      //!< 全方向初速
        float               allDirVelocity;         //!< 全方向初速
    };
    union
    {
        nn::util::Float3    designatedDirScaleVec;  //!< 指定方向初速
        float               designatedDirScale;     //!< 指定方向初速
    };
    nn::util::Float3        particleScale;          //!< パーティクルスケール
    nn::util::Float3        emitterVolumeScale;     //!< エミッタ形状スケール
    union
    {
        nn::util::Float3    gravityScaleVec;        //!< 重力スケール
        float               gravityScale;           //!< 重力スケール
    };
};

//---------------------------------------------------------------------------
//! @brief  エミッタで利用する動的ヒープ
//---------------------------------------------------------------------------
class DynamicHeap : public nn::vfx::Heap
{
    NN_DISALLOW_COPY( DynamicHeap );
public:
    //---------------------------------------------------------------------------
    //! @brief        コンストラクタです。
    //---------------------------------------------------------------------------
    DynamicHeap() NN_NOEXCEPT
    {
        Initialize();
    }

    //---------------------------------------------------------------------------
    //! @brief        初期化。
    //---------------------------------------------------------------------------
    void Initialize() NN_NOEXCEPT
    {
        m_AllocatedSize = 0;
        m_AllocatedCount = 0;
    }

    //---------------------------------------------------------------------------
    //! @brief        デストラクタです。
    //---------------------------------------------------------------------------
    ~DynamicHeap() NN_NOEXCEPT
    {
        NN_SDK_ASSERT( m_AllocatedCount == 0 );
    }

    //---------------------------------------------------------------------------
    //! @brief                  メモリを確保します。
    //! @param[in] size         確保するメモリサイズ
    //! @param[in] alignment    アライメントサイズ
    //! @return                 確保した領域の先頭アドレス
    //---------------------------------------------------------------------------
    virtual void* Alloc( size_t size, size_t alignment = nn::vfx::Heap::Alignment_Default ) NN_NOEXCEPT
    {
        void* ret = detail::AllocFromDynamicHeap( size, alignment, 0x100 );
        if ( ret )
        {
            m_AllocatedSize += nn::util::align_up( size, 0x100 );
            m_AllocatedCount++;
        }
        return ret;
    }

    //---------------------------------------------------------------------------
    //! @brief  メモリを解放します。
    //! @param[in] ptr   解放する領域の先頭アドレス
    //---------------------------------------------------------------------------
    virtual void Free( void* ptr ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( ptr );
        NN_SDK_ASSERT( m_AllocatedCount != 0 );
        m_AllocatedCount--;
        return detail::FreeFromDynamicHeap( ptr );
    }

    //---------------------------------------------------------------------------
    //! @brief  メモリを解放します。
    //! @param[in] ptr   解放する領域の先頭アドレス
    //! @param[in] size  確保したメモリサイズ
    //---------------------------------------------------------------------------
    void Free( void* ptr, size_t size ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( ptr );
        NN_SDK_ASSERT( m_AllocatedCount != 0 );
        m_AllocatedCount--;
        m_AllocatedSize -= nn::util::align_up( size, 0x100 );
        return detail::FreeFromDynamicHeap( ptr );
    }

    //---------------------------------------------------------------------------
    //! @brief  メモリ確保サイズを取得します。
    //! @return 確保済みのメモリサイズ
    //---------------------------------------------------------------------------
    size_t GetAllocatedSize() const NN_NOEXCEPT
    {
        return m_AllocatedSize;
    }

    //---------------------------------------------------------------------------
    //! @brief  メモリ確保回数を取得します。
    //! @return メモリ確保回数
    //---------------------------------------------------------------------------
    size_t GetAllocatedCount() const NN_NOEXCEPT
    {
        return m_AllocatedCount;
    }

private:
    size_t          m_AllocatedSize;    //!< 確保済みメモリサイズ
    int             m_AllocatedCount;   //!< 確保回数
};

//------------------------------------------------------------------------------
//! @brief  エミッタ構造体
//------------------------------------------------------------------------------
class Emitter
{
    NN_DISALLOW_COPY( Emitter );
public:
    class GfxObjects
    {
        NN_DISALLOW_COPY( GfxObjects );
    public:
        void*                           m_DynamicConstantBufferAddr[ BufferSide_MaxBufferSide ];                    //!< エミッタ定数バッファ内
        detail::ConstantBuffer*         m_DynamicConstantBuffer;
        detail::DynamicAllocateBuffer   m_DynamicAllocateBuffer;                                                    //!< エミッタが動的に確保するバッファ
        detail::BufferAllocator*        m_pBufferAllocator;                                                         //!< 動的バッファアロケータ
        detail::DynamicAllocateBuffer   m_CustomConstantBuffer[ CustomShaderConstantBufferIndex_MaxIndexCount ];    //!< エミッタカスタム定数バッファ
        detail::Attribute               m_ParticleEmitterPluginAttribute;                                           //!< エミッタプラグイン用頂点アトリビュート

        //---------------------------------------------------------------------------
        //! @briefprivate コンストラクタ
        //---------------------------------------------------------------------------
        GfxObjects() NN_NOEXCEPT
        {
            m_DynamicConstantBufferAddr[0] = NULL;
            m_DynamicConstantBufferAddr[1] = NULL;
            m_DynamicConstantBufferAddr[2] = NULL;
            m_pBufferAllocator             = NULL;
        }
    };

    //---------------------------------------------------------------------------
    //! @briefprivate コンストラクタ
    //---------------------------------------------------------------------------
    Emitter() NN_NOEXCEPT
    {
    }

    //----------------------------------------
    //! @name Getter
    //@{

    //---------------------------------------------------------------------------
    //! @brief  gfxオブジェクトを取得します。
    //! @return gfxオブジェクト
    //---------------------------------------------------------------------------
    inline GfxObjects* GetGfxObjects() NN_NOEXCEPT
    {
        return &m_GfxObjects;
    }

    //---------------------------------------------------------------------------
    //! @brief  gfxオブジェクトを取得します。( const 版 )
    //! @return gfxオブジェクト
    //---------------------------------------------------------------------------
    const inline GfxObjects* GetGfxObjects() const NN_NOEXCEPT
    {
        return &m_GfxObjects;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタ計算クラスを取得します。
    //! @return エミッタ計算クラス
    //---------------------------------------------------------------------------
    inline detail::EmitterCalculator* GetEmitterCalculator() NN_NOEXCEPT
    {
        return m_pEmitterCalculator;
    }

    //---------------------------------------------------------------------------
    //! @brief  生死判定を取得します。
    //! @return 生存中の場合trueを返す。
    //---------------------------------------------------------------------------
    inline bool IsAlive() const NN_NOEXCEPT
    {
        return ( m_pEmitterCalculator ? true : false );
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタが初期化済みかどうかを取得します。
    //! @return 初期化済みの場合trueを返す。
    //---------------------------------------------------------------------------
    inline bool IsInitialized() const NN_NOEXCEPT
    {
        return IsAlive();
    }

    //---------------------------------------------------------------------------
    //! @brief  表示/非表示フラグを取得します。
    //! @return 表示中の場合trueを返す。
    //---------------------------------------------------------------------------
    inline bool IsVisible() const NN_NOEXCEPT
    {
        return ( m_IsVisible != 0 );
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタがフレームバッファテクスチャを要求するか。
    //! @return 要求する場合trueを返す。
    //---------------------------------------------------------------------------
    bool IsRequestFrameBufferTexture() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  エミッタがデプスバッファテクスチャを要求するか。
    //! @return 要求する場合trueを返す。
    //---------------------------------------------------------------------------
    bool IsRequestDepthTexture() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  描画可能な状態かどうか。
    //! @return 描画可能な場合trueを返す。
    //---------------------------------------------------------------------------
    inline bool IsRenderAvailable() const NN_NOEXCEPT
    {
        if( m_pEmitterData->emitter.isParticleDraw &&
            m_IsVisible &&
            m_FadeOutAlphaValue > 0 &&
            ( m_ParticleCount > 0 || ( m_ParticleCount == 0 && ( 1 <= m_pEmitterRes->m_EmitterPluginIndex && m_pEmitterRes->m_EmitterPluginIndex <= 3 ) ) ) )
        {
            return true;
        }
        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief  マニュアルエミッタが消えても大丈夫な状態かを取得します。
    //! @return 待機時間を過ぎている、もしくは常駐型のエミッタで、処理したパーティクルが無ければ true それ以外は false
    //---------------------------------------------------------------------------
    bool IsManualEmitterReadyToExit() const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name Setter
    //@{

    //---------------------------------------------------------------------------
    //! @brief  エミッタをフェードさせます。
    //---------------------------------------------------------------------------
    void Fade() NN_NOEXCEPT
    {
        m_IsSoloFade = true;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタを削除します。
    //!         削除処理は、次のエミッタ計算処理内で行われます。
    //---------------------------------------------------------------------------
    void Kill() NN_NOEXCEPT
    {
        m_IsKillReservation = true;
    }

    //---------------------------------------------------------------------------
    //! @brief     カラー0 RGBA値(乗算値)を設定します。
    //! @param[in] color0 カラー0値
    //---------------------------------------------------------------------------
    void SetColor0( const nn::util::Vector4fType& color0 ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     カラー1 RGBA値(乗算値)を設定します。
    //! @param[in] color1 カラー1値
    //---------------------------------------------------------------------------
    void SetColor1( const nn::util::Vector4fType& color1 ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              計算処理スキップフラグを設定します。
    //! @param[in] flag     フラグ
    //---------------------------------------------------------------------------
    void SetSkipCalculationFlag( bool flag ) NN_NOEXCEPT
    {
        m_SkipCalculationFlag = ( flag ) ? 1 : 0;
    }

    //---------------------------------------------------------------------------
    //! @brief              放出処理の有効無効を切り替える。
    //!
    //!                     放出処理を無効に切り替え、放出済みパーティクルの再生が終えた場合、
    //!                     エミッタの再生は終了しません。
    //! @param[in] enable   有効無効の指定
    //---------------------------------------------------------------------------
    inline void SetEmissionProcessing( bool enable ) NN_NOEXCEPT
    {
        m_IsEnabledEmit = enable;
    }

    //---------------------------------------------------------------------------
    //! @brief              放出レートスケール設定を行います。
    //!
    //!                     ratio に、1.0以上の値は指定できません。
    //! @param[in] ratio    放出レートスケール値（0.0～1.0）
    //---------------------------------------------------------------------------
    void SetEmissionRatio( float ratio ) NN_NOEXCEPT
    {
        m_EmitRatio = ratio;
        if( m_EmitRatio > 1.0f )
        {
            m_EmitRatio = 1.0f;
        }
    }

    //---------------------------------------------------------------------------
    //! @brief              放出間隔スケール設定を行います。
    //! @param[in] ratio    放出間隔スケール値（0.0～1.0）
    //---------------------------------------------------------------------------
    void SetEmissionInterval( float ratio ) NN_NOEXCEPT
    {
        m_EmitIntervalRatio = ratio;
        if( m_EmitIntervalRatio < 1.0f )
        {
            m_EmitIntervalRatio = 1.0f;
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  パーティクル寿命のスケール値を設定します。
    //! @param[in] ratio 寿命スケール値（0.0～1.0）
    //---------------------------------------------------------------------------
    void SetParticleLifeScale( float ratio ) NN_NOEXCEPT
    {
        m_ParticleLifeScale = ratio;
        if( m_ParticleLifeScale > 1.0f )
        {
            m_ParticleLifeScale = 1.0f;
        }
    }

    //---------------------------------------------------------------------------
    //! @brief          描画パスを設定します。
    //! @param[in] drawPath 描画パスの値
    //---------------------------------------------------------------------------
    void SetDrawPath( uint32_t drawPath ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief           パーティクルユーザデータを設定します。
    //! @param[in] index パーティクルのインデックス
    //! @param[in] pUserData  設定するデータ
    //---------------------------------------------------------------------------
    inline void SetUserData( int index, void* pUserData ) NN_NOEXCEPT
    {
        m_pParticleData[ index ].pUserData = pUserData;
    }

    //---------------------------------------------------------------------------
    //! @brief           パーティクルエミッタプラグインデータを設定します。
    //! @param[in] index パーティクルのインデックス
    //! @param[in] pUserData  設定するデータ
    //---------------------------------------------------------------------------
    inline void SetEmitterPluginData( int index, void* pUserData ) NN_NOEXCEPT
    {
        m_pParticleData[ index ].pEmitterPluginData = pUserData;
    }

    //---------------------------------------------------------------------------
    //! @brief  アルファフェードアウト値を設定します。（※旧API）
    //! @deprecated 旧型式の API 名です。nn::vfx::Emitter::SetFadeOutAlpha() に乗り換えてください。
    //! @param[in] fadeAlpha アルファフェードアウト値
    //---------------------------------------------------------------------------
    NN_DEPRECATED inline void SetFadeAlpha( float fadeAlpha ) NN_NOEXCEPT
    {
        SetFadeOutAlpha( fadeAlpha );
    }

    //---------------------------------------------------------------------------
    //! @brief  アルファフェードアウト値を設定します。
    //! @param[in] fadeAlpha アルファフェードアウト値
    //---------------------------------------------------------------------------
    inline void SetFadeOutAlpha( float fadeAlpha ) NN_NOEXCEPT
    {
        m_FadeOutAlphaValue = fadeAlpha;
    }

    //---------------------------------------------------------------------------
    //! @brief  描画ビューフラグ値を設定します。
    //! @param[in] viewFlag 描画ビューフラグ値
    //---------------------------------------------------------------------------
    inline void SetDrawViewFlag( DrawViewFlag viewFlag ) NN_NOEXCEPT
    {
        m_DrawViewFlag = viewFlag;
    }

    //---------------------------------------------------------------------------
    //! @brief          エミッタプラグインユーザーデータを設定します。
    //! @param[in] ptr  エミッタプラグインユーザーデータへのポインタ
    //---------------------------------------------------------------------------
    inline void SetEmitterPluginUserData( void* ptr ) NN_NOEXCEPT
    {
        m_pEmitterPluginUserData = ptr;
    }

    //---------------------------------------------------------------------------
    //! @brief          カスタムシェーダユーザーデータを設定します。
    //! @param[in] ptr  カスタムシェーダユーザーデータへのポインタ
    //---------------------------------------------------------------------------
    inline void SetCustomShaderUserData( void* ptr ) NN_NOEXCEPT
    {
        m_pCustomShaderUserData = ptr;
    }

    //---------------------------------------------------------------------------
    //! @brief          カスタムアクションユーザーデータを設定します。
    //! @param[in] ptr  カスタムアクションユーザーデータへのポインタ
    //---------------------------------------------------------------------------
    inline void SetCustomActionUserData( void* ptr ) NN_NOEXCEPT
    {
        m_pCustomActionUserData = ptr;
    }

    //---------------------------------------------------------------------------
    //! @brief  SRTマトリクスを設定します。
    //! @param[in] srtMatrix  SRTマトリクス
    //---------------------------------------------------------------------------
    inline void SetMatrixSrt( const nn::util::Matrix4x3fType& srtMatrix ) NN_NOEXCEPT
    {
        m_MatrixSrt = srtMatrix;
    }

    //---------------------------------------------------------------------------
    //! @brief  STマトリクスを設定します。
    //! @param[in] rtMatrix  STマトリクス
    //---------------------------------------------------------------------------
    inline void SetMatrixRt( const nn::util::Matrix4x3fType& rtMatrix ) NN_NOEXCEPT
    {
        m_MatrixRt = rtMatrix;
    }

    //---------------------------------------------------------------------------
    //! @brief  処理するストリームアウトエミッタのフラグ値を設定します。
    //! @param[in] flag     処理するストリームアウトエミッタのフラグの値
    //---------------------------------------------------------------------------
    inline void SetProcessEmitterFlag( uint32_t flag ) NN_NOEXCEPT
    {
        m_ProcessEmitterFlag = flag;
    }

    //---------------------------------------------------------------------------
    //! @brief  描画に利用するプリミティブのLODレベルを設定します。
    //! @param[in] lodLevel     設定するLodレベル。範囲外のレベルを設定した場合は、そのプリミティブの最大レベルが適用されます。
    //---------------------------------------------------------------------------
    inline void SetPrimitiveLodLevel( uint8_t lodLevel ) NN_NOEXCEPT
    {
        m_PrimitiveLodLevel = lodLevel;
    }

    //@}

    //----------------------------------------
    //! @name Getter
    //@{

    //---------------------------------------------------------------------------
    //! @brief  計算処理したパーティクル数を取得します。
    //!         CPUエミッタ時のみ有効な値が返ります。
    //! @return 計算処理を行ったパーティクル数
    //---------------------------------------------------------------------------
    int GetProcessingParticleCount() const NN_NOEXCEPT
    {
        return m_ParticleProcessingCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタが扱うパーティクル数を取得します。
    //!         GPU/Compute エミッタ時は、正確なパーティクル処理数が計算できない為、こちらの値が利用してください。
    //! @return パーティクル数
    //---------------------------------------------------------------------------
    int GetParticleCount() const NN_NOEXCEPT
    {
        return m_ParticleCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  ダイナミックヒープを取得します。
    //! @return ダイナミックヒープ
    //---------------------------------------------------------------------------
    DynamicHeap& GetDynamicHeap() NN_NOEXCEPT
    {
        return m_DynamicHeap;
    }

    //---------------------------------------------------------------------------
    //! @brief      （※旧 API ）ダイナミックヒープから確保したメモリサイズを取得します。
    //! @deprecated 旧型式の API 名です。nn::vfx::Emitter::GetAllocatedFromDynamicHeapSize() に乗り換えてください。
    //! @return     ダイナミックヒープから確保したメモリサイズ
    //! @see        nn::vfx::Emitter::GetAllocatedFromDynamicHeapSize()
    //---------------------------------------------------------------------------
    NN_DEPRECATED size_t GetAllocedFromDynamicHeapSize() const NN_NOEXCEPT
    {
        return GetAllocatedFromDynamicHeapSize();
    }

    //---------------------------------------------------------------------------
    //! @brief      （※旧 API ）GPUバッファから確保したメモリサイズを取得します。
    //! @deprecated 旧型式の API 名です。nn::vfx::Emitter::GetAllocatedFromGpuBufferSize() に乗り換えてください。
    //! @return     GPUバッファから確保したメモリサイズ
    //! @see        nn::vfx::Emitter::GetAllocatedFromGpuBufferSize()
    //---------------------------------------------------------------------------
    NN_DEPRECATED size_t GetAllocedFromGpuBufferSize() const NN_NOEXCEPT
    {
        return GetAllocatedFromGpuBufferSize();
    }

    //---------------------------------------------------------------------------
    //! @brief  ダイナミックヒープから確保したメモリサイズを取得します。
    //! @return ダイナミックヒープから確保したメモリサイズ
    //---------------------------------------------------------------------------
    size_t GetAllocatedFromDynamicHeapSize() const NN_NOEXCEPT
    {
        return m_DynamicHeap.GetAllocatedSize();
    }

    //---------------------------------------------------------------------------
    //! @brief  GPUバッファから確保したメモリサイズを取得します。
    //! @return GPUバッファから確保したメモリサイズ
    //---------------------------------------------------------------------------
    size_t GetAllocatedFromGpuBufferSize() const NN_NOEXCEPT
    {
        return m_GfxObjects.m_DynamicAllocateBuffer.GetAllocatedBufferSize();
    }

    //---------------------------------------------------------------------------
    //! @brief  処理したエミッタ時間アニメ数を取得します。
    //! @return 処理したエミッタ時間アニメ数
    //---------------------------------------------------------------------------
    int GetCalculatedEmitterAnimationCount() const NN_NOEXCEPT
    {
        return m_CalculatedEmitterAnimationCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  計算処理スキップフラグを取得します。
    //! @return 計算処理をスキップする場合は true
    //---------------------------------------------------------------------------
    bool GetSkipCalculationFlag() const NN_NOEXCEPT
    {
        return ( m_SkipCalculationFlag != 0 );
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタの持つ乱数生成器を取得します。
    //! @return 乱数クラスへのポインタ
    //---------------------------------------------------------------------------
    inline detail::Random* GetRandomGenerator() NN_NOEXCEPT
    {
        return &m_Random;
    }

    //---------------------------------------------------------------------------
    //! @brief  カスタムシェーダパラメータを取得します。
    //! @return カスタムシェーダパラメータ値
    //---------------------------------------------------------------------------
    inline const void* GetCustomShaderParameter() const NN_NOEXCEPT
    {
        return m_pEmitterRes->m_pCustomShaderParam;
    }

    //---------------------------------------------------------------------------
    //! @brief  カスタムデータ パラメータを取得します。
    //! @return カスタムデータ パラメータへのポインタ
    //---------------------------------------------------------------------------
    inline const void* GetCustomDataParameter() const NN_NOEXCEPT
    {
        return m_pEmitterRes->m_pCustomDataParam;
    }

    //---------------------------------------------------------------------------
    //! @brief  カスタムアクション パラメータを取得します。
    //! @return カスタムアクション パラメータへのポインタ
    //---------------------------------------------------------------------------
    inline const void* GetCustomActionParameter() const NN_NOEXCEPT
    {
        return m_pEmitterRes->m_pCustomActionParam;
    }

    //---------------------------------------------------------------------------
    //! @brief          カスタムシェーダパラメータをインデックスを指定して取得します。
    //! @param[in] index  カスタムシェーダパラメータのインデックス値
    //! @return         index で指定したカスタムシェーダパラメータ
    //---------------------------------------------------------------------------
    float GetCustomShaderParameter( int index ) const NN_NOEXCEPT
    {
        if( !m_pEmitterRes->m_pCustomShaderParam )
        {
            return 0.0f;
        }
        float* fparam = reinterpret_cast< float* >( m_pEmitterRes->m_pCustomShaderParam );
        return fparam[ index ];
    }

    //---------------------------------------------------------------------------
    //! @brief  カスタムシェーダフラグを取得します。
    //! @return カスタムシェーダフラグ
    //---------------------------------------------------------------------------
    inline uint64_t GetCustomShaderFlag() const NN_NOEXCEPT
    {
        return m_pEmitterRes->m_pResEmitter->shader.customShaderFlag;
    }

    //---------------------------------------------------------------------------
    //! @brief  カスタムシェーダスイッチを取得します。
    //! @return カスタムシェーダスイッチ
    //---------------------------------------------------------------------------
    inline uint64_t GetCustomShaderSwitch() const NN_NOEXCEPT
    {
        return m_pEmitterRes->m_pResEmitter->shader.customShaderSwitch;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタプラグインユーザーデータを取得します。
    //! @return エミッタプラグインユーザーデータへのポインタ
    //---------------------------------------------------------------------------
    inline void* GetEmitterPluginUserData() const NN_NOEXCEPT
    {
        return m_pEmitterPluginUserData;
    }

    //---------------------------------------------------------------------------
    //! @brief  カスタムシェーダユーザーデータを取得します。
    //! @return カスタムシェーダユーザーデータへのポインタ
    //---------------------------------------------------------------------------
    inline void* GetCustomShaderUserData() const NN_NOEXCEPT
    {
        return m_pCustomShaderUserData;
    }

    //---------------------------------------------------------------------------
    //! @brief  カスタムアクションユーザーデータを取得します。
    //! @return カスタムアクションユーザーデータへのポインタ
    //---------------------------------------------------------------------------
    inline void* GetCustomActionUserData() const NN_NOEXCEPT
    {
        return m_pCustomActionUserData;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタセットを取得します。
    //! @return エミッタセットへのポインタ
    //---------------------------------------------------------------------------
    inline EmitterSet* GetEmitterSet() NN_NOEXCEPT
    {
        return m_pEmitterSet;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタセットを取得します。
    //! @return エミッタセットへのポインタ
    //---------------------------------------------------------------------------
    inline const EmitterSet* GetEmitterSet() const NN_NOEXCEPT
    {
        return m_pEmitterSet;
    }

    //---------------------------------------------------------------------------
    //! @brief      チャイルドエミッタの先頭のポインタを取得します。
    //! @param[in]  childIndex チャイルドのインデックス
    //! @return     チャイルドエミッタの先頭のポインタ
    //---------------------------------------------------------------------------
    NN_DEPRECATED inline const Emitter* GetChildHead( int childIndex ) const NN_NOEXCEPT
    {
        return m_pChildHead[ childIndex ];
    }

    //---------------------------------------------------------------------------
    //! @brief  次のエミッタへのポインタを取得します。チャイルドエミッタの走査に利用します。
    //! @return 次のエミッタへのポインタ
    //---------------------------------------------------------------------------
    inline const Emitter* GetNextEmitter() const NN_NOEXCEPT
    {
        return m_pNextEmitter;
    }

    //---------------------------------------------------------------------------
    //! @brief  次のエミッタへのポインタを取得します。チャイルドエミッタの走査に利用します。
    //! @return 次のエミッタへのポインタ
    //---------------------------------------------------------------------------
    inline Emitter* GetNextEmitter() NN_NOEXCEPT
    {
        return m_pNextEmitter;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタバイナリを取得します。
    //! @return エミッタバイナリへのポインタ
    //---------------------------------------------------------------------------
    inline const detail::ResEmitter* GetResEmitter() const NN_NOEXCEPT
    {
        return m_pEmitterData;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタリソースセットを取得します。
    //! @return エミッタリソースセットへのポインタ
    //---------------------------------------------------------------------------
    inline const EmitterResource* GetEmitterResource() const NN_NOEXCEPT
    {
        return m_pEmitterRes;
    }

    //---------------------------------------------------------------------------
    //! @brief  バッファ面を取得します。
    //! @return バッファ面
    //---------------------------------------------------------------------------
    inline BufferSide GetBufferSide() const NN_NOEXCEPT
    {
        return m_BufferSide;
    }

    //---------------------------------------------------------------------------
    //! @brief  描画パスを取得します。
    //! @return 描画パス値
    //---------------------------------------------------------------------------
    inline uint32_t GetDrawPath() const NN_NOEXCEPT
    {
        return m_DrawPathFlag;
    }

    //---------------------------------------------------------------------------
    //! @brief  グループIDを取得します。
    //! @return グループID
    //---------------------------------------------------------------------------
    inline int GetGroupID() const NN_NOEXCEPT
    {
        return m_GroupId;
    }

    //---------------------------------------------------------------------------
    //! @brief  カスタムシェーダインデックスを取得します。
    //! @return カスタムシェーダインデックス
    //---------------------------------------------------------------------------
    inline int GetCustomShaderIndex() const NN_NOEXCEPT
    {
        return m_pEmitterRes->m_pResEmitter->shader.customShaderIndex;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタ計算タイプタイプを取得します。
    //! @return エミッタ計算タイプ
    //---------------------------------------------------------------------------
    inline detail::EmitterCalculationMode GetCalculationType() const NN_NOEXCEPT
    {
        return static_cast< detail::EmitterCalculationMode >( m_pEmitterData->emitter.calcType );
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタ追従タイプを取得します。
    //! @return エミッタ追従タイプ
    //---------------------------------------------------------------------------
    inline detail::ParticleFollowType GetFollowType() const NN_NOEXCEPT
    {
        return static_cast< detail::ParticleFollowType >( m_pEmitterData->emitter.followType );
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタの名前を取得します。
    //! @return エミッタの名前
    //---------------------------------------------------------------------------
    inline const char* GetName() const NN_NOEXCEPT
    {
        return m_pEmitterData->name;
    }

    //---------------------------------------------------
    //! @brief  エミッタアニメーション計算結果を取得する
    //! @return エミッタアニメーション計算結果
    //---------------------------------------------------
    inline const EmitterAnimValue& GetEmitterAnimValue() const NN_NOEXCEPT
    {
        return m_EmitterAnimation;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタ時間を取得します。
    //! @return エミッタ時間
    //---------------------------------------------------------------------------
    inline float GetTime() const NN_NOEXCEPT
    {
        return m_Time;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタ時間を取得します。
    //! @return エミッタ時間
    //---------------------------------------------------------------------------
    inline float GetFrameRate() const NN_NOEXCEPT
    {
        return m_FrameRate;
    }

    //---------------------------------------------------------------------------
    //! @brief              指定インデックスのパーティクル寿命を取得します。
    //! @param[in] index    パーティクルのインデックス
    //! @return             パーティクル寿命
    //---------------------------------------------------------------------------
    inline float GetParticleLife( int index ) const NN_NOEXCEPT
    {
        return m_pParticleData[ index ].GetLife();
    }

    //---------------------------------------------------------------------------
    //! @brief              指定インデックスのパーティクル時間を取得します。
    //! @param[in] index    パーティクルのインデックス
    //! @return             パーティクル時間
    //---------------------------------------------------------------------------
    inline float GetParticleTime( int index ) const NN_NOEXCEPT
    {
        return m_pParticleData[ index ].GetTime( m_Time );
    }

    //---------------------------------------------------------------------------
    //! @brief              指定インデックスのパーティクルのローカル位置を取得します。
    //! @param[in] index    パーティクルのインデックス
    //! @return             パーティクルローカル位置
    //---------------------------------------------------------------------------
    inline const nn::util::Float4* GetParticleLocalPosition( int index ) const NN_NOEXCEPT
    {
        if ( m_ParticleCount < index ) return NULL;
        return &m_CpuParticleProperty.localPos[index];
    }

    //---------------------------------------------------------------------------
    //! @brief              指定インデックスのパーティクルのローカル速度を取得します。
    //! @param[in] index    パーティクルのインデックス
    //! @return             パーティクルローカル速度
    //---------------------------------------------------------------------------
    inline const nn::util::Float4* GetParticleLocalVelocity( int index ) const NN_NOEXCEPT
    {
        if ( m_ParticleCount < index ) return NULL;
        return &m_CpuParticleProperty.localVec[index];
    }

    //---------------------------------------------------------------------------
    //! @brief              指定インデックスのパーティクルの放出時エミッタSRTマトリクスの0行目を取得します。
    //! @param[in] index    パーティクルのインデックス
    //! @return             パーティクル放出時エミッタSRTマトリクスの0行目
    //---------------------------------------------------------------------------
    inline const nn::util::Float4* GetParticleEmitterMatrixSrtRow0( int index ) const NN_NOEXCEPT
    {
        if ( !m_CpuParticleProperty.emitterMatrixSrt0 ) return NULL;
        if ( m_ParticleCount < index ) return NULL;
        return &m_CpuParticleProperty.emitterMatrixSrt0[index];
    }

    //---------------------------------------------------------------------------
    //! @brief              指定インデックスのパーティクルの放出時エミッタSRTマトリクスの1行目を取得します。
    //! @param[in] index    パーティクルのインデックス
    //! @return             パーティクル放出時エミッタSRTマトリクスの1行目
    //---------------------------------------------------------------------------
    inline const nn::util::Float4* GetParticleEmitterMatrixSrtRow1( int index ) const NN_NOEXCEPT
    {
        if ( !m_CpuParticleProperty.emitterMatrixSrt1 ) return NULL;
        if ( m_ParticleCount < index ) return NULL;
        return &m_CpuParticleProperty.emitterMatrixSrt1[index];
    }

    //---------------------------------------------------------------------------
    //! @brief              指定インデックスのパーティクルの放出時エミッタSRTマトリクスの2行目を取得します。
    //! @param[in] index    パーティクルのインデックス
    //! @return             パーティクル放出時エミッタSRTマトリクスの2行目
    //---------------------------------------------------------------------------
    inline const nn::util::Float4* GetParticleEmitterMatrixSrtRow2( int index ) const NN_NOEXCEPT
    {
        if ( !m_CpuParticleProperty.emitterMatrixSrt2 ) return NULL;
        if ( m_ParticleCount < index ) return NULL;
        return &m_CpuParticleProperty.emitterMatrixSrt2[index];
    }

    //---------------------------------------------------------------------------
    //! @brief                  指定インデックスのパーティクルの放出時エミッタSRTマトリクスを取得します。
    //! @param[out] pOutMatrix  取得先マトリクス
    //! @param[in] index        パーティクルのインデックス
    //! @return                 パーティクル放出時エミッタSRTマトリクス
    //---------------------------------------------------------------------------
    inline const bool GetParticleEmitterMatrixSrt( nn::util::Matrix4x3fType* pOutMatrix, int index ) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( pOutMatrix );

        if ( !m_CpuParticleProperty.emitterMatrixSrt0 ) return false;
        if ( m_ParticleCount < index ) return false;

        const nn::util::Float4& v0 = m_CpuParticleProperty.emitterMatrixSrt0[ index ];
        const nn::util::Float4& v1 = m_CpuParticleProperty.emitterMatrixSrt1[ index ];
        const nn::util::Float4& v2 = m_CpuParticleProperty.emitterMatrixSrt2[ index ];

        nn::util::MatrixSet( pOutMatrix,
                            v0.x, v1.x, v2.x,
                            v0.y, v1.y, v2.y,
                            v0.z, v1.z, v2.z,
                            v0.w, v1.w, v2.w );
        return true;
    }

    //---------------------------------------------------------------------------
    //! @brief  放出レートの最大値（エミッタ時間アニメーションを含むピーク値）を取得します。
    //! @return 放出レートの最大値
    //---------------------------------------------------------------------------
    inline float GetEmissionRateMax() const NN_NOEXCEPT
    {
        return GetAnimKeyMaxValueX( m_pEmitterRes->m_EmitterAnimationArray[ detail::EmitterAnimationType_EmissionRate ], m_pEmitterData->emission.rate );
    }

    //---------------------------------------------------------------------------
    //! @brief  寿命の取りうる最大値（エミッタ時間アニメーションを含むピーク値）を取得します。
    //! @return 寿命の取りうる最大値
    //---------------------------------------------------------------------------
    inline int GetParticleLifeMax() const NN_NOEXCEPT
    {
        // アニメーションのピーク値と、
        return static_cast< int >( GetAnimKeyMaxValueX( m_pEmitterRes->m_EmitterAnimationArray[ detail::EmitterAnimationType_ParticleLife ], static_cast< float >( m_pEmitterData->ptcl.life ) ) );
    }

    //---------------------------------------------------------------------------
    //! @brief  αフェードアウト値を取得します。（※旧API）
    //! @deprecated 旧型式の API 名です。nn::vfx::Emitter::GetFadeOutAlpha() に乗り換えてください。
    //! @return αフェードアウト値
    //---------------------------------------------------------------------------
    NN_DEPRECATED inline float GetFadeAlpha() const NN_NOEXCEPT
    {
        return GetFadeOutAlpha();
    }

    //---------------------------------------------------------------------------
    //! @brief  αフェードアウト値を取得します。
    //! @return αフェードアウト値
    //---------------------------------------------------------------------------
    inline float GetFadeOutAlpha() const NN_NOEXCEPT
    {
        return m_FadeOutAlphaValue;
    }

    //---------------------------------------------------------------------------
    //! @brief  αフェードイン値を取得します。
    //! @return αフェードイン値
    //---------------------------------------------------------------------------
    inline float GetFadeInAlpha() const NN_NOEXCEPT
    {
        return m_FadeInAlphaValue;
    }

    //---------------------------------------------------------------------------
    //! @brief  フェードイン・アウトを考慮した現在のスケールフェード値を取得します。
    //! @return フェードイン・アウトを考慮した現在のスケールフェード値
    //---------------------------------------------------------------------------
    inline float CalculateCurrentScaleFadeValue() const NN_NOEXCEPT
    {
        float fadeValue = 1.0f;
        if( IsScaleFadeIn() )
        {
            fadeValue *= GetFadeInAlpha();
        }
        if( IsScaleFadeOut() )
        {
            fadeValue *= GetFadeOutAlpha();
        }
        return fadeValue;
    }

    //---------------------------------------------------------------------------
    //! @brief  フェードイン・アウトを考慮した現在のアルファフェード値を取得します。
    //! @return フェードイン・アウトを考慮した現在のアルファフェード値
    //---------------------------------------------------------------------------
    inline float CalculateCurrentAlphaFadeValue() const NN_NOEXCEPT
    {
        float fadeValue = 1.0f;
        if( IsAlphaFadeIn() )
        {
            fadeValue *= GetFadeInAlpha();
        }
        if( IsAlphaFadeOut() )
        {
            fadeValue *= GetFadeOutAlpha();
        }
        return fadeValue;
    }

    //---------------------------------------------------------------------------
    //! @brief  αフェードインするかどうかを取得します。
    //! @return αフェードインするかどうか
    //---------------------------------------------------------------------------
    inline bool IsAlphaFadeIn() const NN_NOEXCEPT
    {
        return ( m_pEmitterRes->m_pResEmitter->emitter.isAlphaFadeIn != 0 );
    }

    //---------------------------------------------------------------------------
    //! @brief  αフェードアウトするかどうかを取得します。
    //! @return αフェードアウトするかどうか
    //---------------------------------------------------------------------------
    inline bool IsAlphaFadeOut() const NN_NOEXCEPT
    {
        return ( m_pEmitterRes->m_pResEmitter->emitter.isFadeAlphaFade != 0 );
    }

    //---------------------------------------------------------------------------
    //! @brief  スケールフェードインするかどうかを取得します。
    //! @return スケールフェードインするかどうか
    //---------------------------------------------------------------------------
    inline bool IsScaleFadeIn() const NN_NOEXCEPT
    {
        return ( m_pEmitterRes->m_pResEmitter->emitter.isScaleFadeIn != 0 );
    }

    //---------------------------------------------------------------------------
    //! @brief  スケールフェードアウトするかどうかを取得します。
    //! @return スケールフェードアウトするかどうか
    //---------------------------------------------------------------------------
    inline bool IsScaleFadeOut() const NN_NOEXCEPT
    {
        return ( m_pEmitterRes->m_pResEmitter->emitter.isScaleFade != 0 );
    }

    //---------------------------------------------------------------------------
    //! @brief  有効なパーティクルバッファ数を取得します。
    //! @return バッファ数。現在処理しているパーティクル数とは異なります。
    //---------------------------------------------------------------------------
    inline int GetEnableBufferCount() const NN_NOEXCEPT
    {
        return m_ParticleCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  パーティクルバッファの確保数を取得します。
    //! @deprecated 旧型式の API 名です。nn::vfx::Emitter::GetParticleAttrFillMax() に乗り換えてください。
    //! @return 最大バッファ数。現在処理しているパーティクル数とは異なります。
    //---------------------------------------------------------------------------
    NN_DEPRECATED inline int GetPartilceAttrFillMax() const NN_NOEXCEPT
    {
        return m_MaxParticleAttrFill;
    }

    //---------------------------------------------------------------------------
    //! @brief  パーティクルバッファの確保数を取得します。
    //! @return 最大バッファ数。現在処理しているパーティクル数とは異なります。
    //---------------------------------------------------------------------------
    inline int GetParticleAttrFillMax() const NN_NOEXCEPT
    {
        return m_MaxParticleAttrFill;
    }

    //---------------------------------------------------------------------------
    //! @brief              シェーダへのポインタを取得します。
    //! @param[in] type     シェーダ番号。通常は nn::vfx::ShaderType::ShaderType_Normal のシェーダが使われますが、
    //!                     プロジェクトコンフィグの描画パス設定で指定していれば、
    //!                     ShaderType_DefaultPath1, ShaderType_DefaultPath2 のシェーダも利用できます。
    //!                     詳細はドキュメントを参照してください。
    //! @return             シェーダへのポインタ
    //---------------------------------------------------------------------------
    detail::Shader* GetShader( ShaderType type ) const NN_NOEXCEPT
    {
        if( type == nn::vfx::ShaderType_MaxShaderType )
        {
            return NULL;
        }
        return m_pEmitterRes->m_pShader[ type ];
    }

    //---------------------------------------------------------------------------
    //! @brief              パーティクルユーザデータを取得します。
    //! @param[in] index    パーティクルのインデックス
    //! @return             ユーザーデータへのポインタ
    //---------------------------------------------------------------------------
    inline void* GetUserData( int index ) const NN_NOEXCEPT
    {
        return m_pParticleData[ index ].pUserData;
    }

    //---------------------------------------------------------------------------
    //! @brief              パーティクルデータを取得します。
    //! @param[in] index    パーティクルのインデックス
    //! @return             パーティクルデータへのポインタ
    //---------------------------------------------------------------------------
    inline detail::ParticleData* GetParticleData( int index ) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT( index < m_MaxParticleAttrFill );
        return &m_pParticleData[ index ];
    }

    //---------------------------------------------------------------------------
    //! @brief           チャイルドエミッタかどうかを取得します
    //! @return          チャイルドエミッタなら true
    //---------------------------------------------------------------------------
    inline bool IsChildEmitter() const NN_NOEXCEPT
    {
        return m_IsChild > 0;
    }

    //---------------------------------------------------------------------------
    //! @brief           親エミッタへのポインタを取得します
    //! @return          親エミッタへのポインタ
    //---------------------------------------------------------------------------
    inline const Emitter* GetParentEmitter() const NN_NOEXCEPT
    {
        return m_pParentEmitter;
    }

    //---------------------------------------------------------------------------
    //! @brief           パーティクルエミッタプラグインデータを取得します。
    //! @param[in] index パーティクルのインデックス
    //! @return          パーティクエミッタプラグインデータ
    //---------------------------------------------------------------------------
    inline void* GetEmitterPluginData( int index ) const NN_NOEXCEPT
    {
        return m_pParticleData[ index ].pEmitterPluginData;
    }

    //---------------------------------------------------------------------------
    //! @brief              エミッタカスタム定数バッファを取得します。
    //! @param[in] index    nn::vfx::CustomShaderConstantBufferIndex で与えられるカスタムシェーダ用定数バッファのインデックス
    //! @return             定数バッファへのポインタ
    //---------------------------------------------------------------------------
    inline detail::DynamicAllocateBuffer* GetEmitterCustomConstantBuffer( CustomShaderConstantBufferIndex index ) NN_NOEXCEPT
    {
        return &m_GfxObjects.m_CustomConstantBuffer[ static_cast< int >( index ) ];
    }

    //---------------------------------------------------------------------------
    //! @brief              設定されているコールバックセットを取得します。
    //! @param[in] type     使用するバッファの先頭アドレス（バッファリング数分確保済み）
    //---------------------------------------------------------------------------
    inline const CallbackSet* GetCallbackSet( detail::CallbackSetType type ) const NN_NOEXCEPT
    {
        return m_pCallbackSet[ type ];
    }

    //---------------------------------------------------------------------------
    //! @brief  SRTマトリクスを取得します。
    //! @return SRTマトリクス
    //---------------------------------------------------------------------------
    inline const nn::util::Matrix4x3fType& GetMatrixSrt() const NN_NOEXCEPT
    {
        return m_MatrixSrt;
    }

    //---------------------------------------------------------------------------
    //! @brief  SRTマトリクスを取得します。
    //! @return SRTマトリクス
    //---------------------------------------------------------------------------
    inline const nn::util::Matrix4x3fType& GetMatrixRt() const NN_NOEXCEPT
    {
        return m_MatrixRt;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタの前フレームの位置を取得します。
    //!
    //!         エミッタ生成直後、初回の Calculate 処理が終わるまでは、原点を返します。
    //! @return エミッタの前フレームの位置
    //---------------------------------------------------------------------------
    inline const nn::util::Vector3fType& GetEmitterPrevPos() const NN_NOEXCEPT
    {
        return m_EmitterPrevPos;
    }

    //---------------------------------------------------------------------------
    //! @brief  描画ビューフラグ値を取得します。
    //! @return 描画ビューフラグ値
    //---------------------------------------------------------------------------
    inline DrawViewFlag GetDrawViewFlag() const NN_NOEXCEPT
    {
        return m_DrawViewFlag;
    }

    //---------------------------------------------------------------------------
    //! @brief  処理するストリームアウトエミッタのフラグ値を取得します。
    //! @return 処理するストリームアウトエミッタのフラグの値
    //---------------------------------------------------------------------------
    inline uint32_t GetProcessEmitterFlag() const NN_NOEXCEPT
    {
        return m_ProcessEmitterFlag;
    }

    //---------------------------------------------------------------------------
    //! @brief  今の時点でエミッタが持つ正確なパーティクルの数を計測します。
    //!
    //!         GPU / GPU+SO エミッタに対してこの API を呼んだ場合、
    //!         内部で保持するパーティクルを走査して計測を行うため、
    //!         処理負荷が高くなることに注意してください。
    //!         アプリケーションのデバッグ用途以外にこの API を利用することは推奨しません。
    //! @return エミッタの持つパーティクルの総数
    //---------------------------------------------------------------------------
    int CalculatePreciseParticleCount() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              チャイルドエミッタの先頭を取得します。
    //! @param[in]  index   チャイルドのインデックス
    //! @return             チャイルドエミッタの先頭のポインタ
    //---------------------------------------------------------------------------
    inline nn::vfx::Emitter* GetChildEmitterHead( int index ) const NN_NOEXCEPT
    {
        return m_pChildHead[index];
    }

    //@}

    //---------------------------------------------------------------------------
    //! @brief                      エミッタカスタム定数バッファを初期化します。
    //! @param[in] index            定数バッファインデックス
    //! @param[in] customConstantBufferSize    定数バッファ型のサイズ
    //! @return                     成功した場合 true
    //---------------------------------------------------------------------------
    bool InitializeCustomConstantBuffer( int index, size_t customConstantBufferSize ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                          エミッタカスタムAttributeを初期化します。
    //! @param[in] bufferCount          バッファカウント
    //! @param[in] bufferSize           バッファサイズ
    //! @return                         成功した場合 true
    //---------------------------------------------------------------------------
    bool InitializeCustomAttribute( int bufferCount, size_t bufferSize ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief              方向ベクトルのローカル座標変化を行います。
    //!
    //!                     pSrc をエミッタマトリクス空間へ変換し pDst に出力します。
    //! @param[out] pOutVec    変換後のVEC3へのポインタ
    //! @param[in] src      入力となるVEC3
    //! @param[in] particleIndex     変換するパーティクルのインデックス
    //------------------------------------------------------------------------------
    void TransformToLocalVec( nn::util::Vector3fType* pOutVec, const nn::util::Vector3fType& src, int particleIndex ) const NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief              方向ベクトルのワールド座標変換を行います。
    //!
    //!                     pSrc をワールド空間へ変換し pDst に出力します。
    //! @param[out] pOutVec    変換後のVEC3へのポインタ
    //! @param[in] src      入力となるVEC3
    //------------------------------------------------------------------------------
    void TransformToWorldVec( nn::util::Vector3fType* pOutVec, const nn::util::Vector3fType& src ) const NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief              方向ベクトルのワールド座標変換を行います。
    //!
    //!                     pSrc をワールド空間へ変換し pDst に出力します。
    //! @param[out] pOutVec    変換後のVEC3へのポインタ
    //! @param[in] src      入力となるVEC3
    //! @param[in] particleIndex     変換するパーティクルのインデックス
    //------------------------------------------------------------------------------
    void TransformToWorldVec( nn::util::Vector3fType* pOutVec, const nn::util::Vector3fType& src, int particleIndex ) const NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief              位置ベクトルのローカル座標変換を行います。
    //!
    //!                     pSrc をエミッタマトリクス空間へ変換し pDst に出力します。
    //! @param[out] pOutPos    変換後のVEC3へのポインタ
    //! @param[in] src      入力となるVEC3
    //------------------------------------------------------------------------------
    void TransformToLocalPos( nn::util::Vector3fType* pOutPos, const nn::util::Vector3fType& src ) const NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief              位置ベクトルのローカル座標変換を行います。
    //!
    //!                     pSrc をエミッタマトリクス空間へ変換し pDst に出力します。
    //! @param[out] pOutPos         変換後のVEC3へのポインタ
    //! @param[in] src              入力となるVEC3
    //! @param[in] particleIndex     変換するパーティクルのインデックス
    //------------------------------------------------------------------------------
    void TransformToLocalPos( nn::util::Vector3fType* pOutPos, const nn::util::Vector3fType& src, int particleIndex ) const NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief              位置ベクトルのワールド座標変換を行います。
    //!
    //!                     pSrc をワールド空間へ変換し pDst に出力します。
    //! @param[out] pOutPos    変換後のVEC3へのポインタ
    //! @param[in] src      入力となるVEC3
    //! @param[in] particleIndex     変換するパーティクルのインデックス
    //------------------------------------------------------------------------------
    void TransformToWorldPos( nn::util::Vector3fType* pOutPos, const nn::util::Vector3fType& src, int particleIndex ) const NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief                  パーティクル位置をワールド座標系に変換するための、追従設定を加味した変換行列を取得します。
    //! @param[out] pOutMatrix  変換用の行列へのポインタ
    //! @param[in]  index       パーティクルのインデックス
    //------------------------------------------------------------------------------
    void GetEmitterTransformMatrix( nn::util::Matrix4x3f* pOutMatrix, int index ) const NN_NOEXCEPT;

private:
    //---------------------------------------------------------------------------
    //! @brief  メンバを再初期化します。
    //---------------------------------------------------------------------------
    void Reset() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  バッファをスワップします。
    //! @param[in]  bufferSwapMode       バッファスワップモード
    //! @return Swapした場合trueを返す。
    //---------------------------------------------------------------------------
    bool SwapBuffer( BufferSwapMode bufferSwapMode ) NN_NOEXCEPT;


    //---------------------------------------------------------------------------
    //  今フレームにCPUから書き込むアトリビュートをアンマップします。
    //---------------------------------------------------------------------------
    void Unmap() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                      エミッタ初期化を行います。
    //! @param[in] pDevice          デバイスへのポインタ
    //! @param[in] emitterSet       親エミッタセット
    //! @param[in] pEmitterResource エミッタリソースへのポインタ
    //! @param[in] maxParticleCount パーティクル最大数
    //! @param[in] pHeap            ヒープへのポインタ
    //! @param[in] pParentEmitter   親エミッタへのポインタ
    //! @return                     成功した場合trueを返す。
    //---------------------------------------------------------------------------
    bool Initialize( nn::gfx::Device* pDevice, EmitterSet* emitterSet, const EmitterResource* pEmitterResource, int maxParticleCount, Heap* pHeap, Emitter* pParentEmitter = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  リソース更新時処理を行います。
    //! @param[in] pDevice          デバイスへのポインタ
    //! @param[in] pParentEmitter   親エミッタへのポインタ
    //! @return 成功した場合trueを返す。
    //---------------------------------------------------------------------------
    bool ResourceUpdate( nn::gfx::Device* pDevice, Emitter* pParentEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  確保しているバッファの解放処理を行います。
    //! @param[in] pDevice          デバイスへのポインタ
    //---------------------------------------------------------------------------
    void ReleaseBuffer( nn::gfx::Device* pDevice ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  エミッタ終了処理を行います。
    //! @param[in] pDevice          デバイスへのポインタ
    //! @param[in] invokeCallback   コールバック関数を呼び出すか。コールバック関数を呼び出さずにFinalizeしたい場合に利用します。
    //---------------------------------------------------------------------------
    void Finalize( nn::gfx::Device* pDevice, bool invokeCallback = true ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          放出毎に更新が必要な情報を更新します。
    //! @param[in,out]  pOutEmitInterval    放出レート
    //---------------------------------------------------------------------------
    void UpdateByEmit( float* pOutEmitInterval ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  マトリクス情報を作成します。
    //---------------------------------------------------------------------------
    void CreateResMatrix() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  リソース変更時に更新が必要な情報を更新します。
    //---------------------------------------------------------------------------
    void UpdateByChangeRes() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              子エミッタを追加します。
    //! @param[in] pEmitter  エミッタへのポインタ
    //---------------------------------------------------------------------------
    void AddChildEmitter( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              子エミッタを削除します。
    //! @param[in] pEmitter  エミッタへのポインタ
    //---------------------------------------------------------------------------
    void RemoveChildEmitter( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                      8キーアニメーションのx値のピーク値を取得
    //! @param[in] pAnimationData   アニメーションデータ
    //! @param[in] defaultValue     アニメーション無しの場合の値
    //! @return                     8キーアニメーションのx値のピーク値
    //---------------------------------------------------------------------------
    float GetAnimKeyMaxValueX( const detail::ResAnimEmitterKeyParamSet* pAnimationData, float defaultValue ) const NN_NOEXCEPT
    {
        float maxRate = defaultValue;
        if( pAnimationData )
        {
            const int keyCount = pAnimationData->keyNum;

            for( int i = 0; i < keyCount; i++ )
            {
                if( maxRate < pAnimationData->keyValue[ i ].x )
                {
                    maxRate = pAnimationData->keyValue[ i ].x;
                }
            }
        }
        return maxRate;
    }

    //---------------------------------------------------------------------------
    //! @brief  アトリビュートのダブルバッファリング状態のチェック
    //---------------------------------------------------------------------------
    void CheckParticleAttributeDoubleBufferingState() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エミッタが必要とするパーティクルの最大確保数を計算する
    //! @param[in]  pParentEmitter  親エミッタへのポインタ
    //! @return     エミッタが必要とするパーティクルの最大確保数
    //---------------------------------------------------------------------------
    int CalculateRequiredParticleAsignmentCount( const Emitter* pParentEmitter ) const NN_NOEXCEPT;


private:
    uint8_t                         m_IsChild;                                                              //!< 子エミッタかどうか
    uint8_t                         m_IsEmitDone;                                                           //!< 放出済みかどうか？
    uint8_t                         m_IsVisible;                                                            //!< 表示/非表示
    uint8_t                         m_IsEnabledEmit;                                                        //!< 放出が有効かどうか
    uint8_t                         m_IsAliveChildren;                                                      //!< 子エミッタが生存してるかどうか
    uint8_t                         m_LastCalculateResult;                                                  //!< 最後の Calculate() 処理の返り値
    uint8_t                         m_IsEmitterInitializeFailed;                                            //!< vfxランタイム側でエミッタ初期化失敗したか
    uint8_t                         m_IsKillReservation;                                                    //!< 削除予約
    uint8_t                         m_PrimitiveLodLevel;                                                    //!< プリミティブLODレベル
    uint8_t                         m_IsDrawExecuted;                                                       //!< 描画コマンド発行が実行されたか。
    uint8_t                         m_IsComputeShaderExecuted;                                              //!< コンピュートシェーダ実行コマンドが発行されたか。
    uint8_t                         m_Dummy;                                                                //!< ダミー
    int                             m_CalculatedEmitterAnimationCount;                                      //!< 計算処理されたエミッタアニメーション数
    int                             m_GroupId;                                                              //!< グループID
    int                             m_EmitterCreateId;                                                      //!< エミッタ生成ID
    int                             m_EmitterSetCreateId;                                                   //!< 親エミッタセット生成ID
    int                             m_ParticleCreateId;                                                     //!< エミッタローカルなパーティクル生成ID
    int                             m_ChildResIndex;                                                        //!< チャイルドリソースインデックス
    int                             m_MaxParticleAssignmentCount;                                           //!< アプリ側から指定されたパーティクル最大数
    int                             m_ParticleCount;                                                        //!< 放出中のパーティクル数
    int                             m_ParticleProcessingCount;                                              //!< 今フレーム計算処理したパーティクル数
    BufferSide                      m_BufferSide;                                                           //!< バッファサイド
    int                             m_ComputeShaderCounter;                                                 //!< １フレーム内にコンピュートシェーダを二重実行しない為のカウンタ
    int                             m_EmitByPrimitiveCounter;                                               //!< 放出形状 プリミティブ用カウンタ
    uint32_t                        m_SkipCalculationFlag;                                                  //!< 計算処理フラグ
    DrawViewFlag                    m_DrawViewFlag;                                                         //!< ビューフラグ
    float                           m_Time;                                                                 //!< エミッタ時間
    float                           m_GpuTime;                                                              //!< GPU に送る時間のカウンター
    float                           m_FrameRate;                                                            //!< フレームレート
    float                           m_SavingFrameRate;                                                      //!< 貯留フレームレート
    float                           m_EmitCount;                                                            //!< エミッタカウンタ
    float                           m_EmitSaving;                                                           //!< 放出貯留
    float                           m_EmitInterval;                                                         //!< 放出間隔
    float                           m_EmitRatio;                                                            //!< 放出レート係数
    float                           m_EmitIntervalRatio;                                                    //!< 放出間隔係数
    float                           m_EmitLastFrame;                                                        //!< 最終放出フレーム
    float                           m_ParticleLifeScale;                                                    //!< パーティクルライフスケール
    float                           m_FadeOutAlphaValue;                                                    //!< フェードアウト値
    float                           m_FadeInAlphaValue;                                                     //!< フェードイン値
    EmitterSet*                     m_pEmitterSet;                                                          //!< 親エミッタセット
    Emitter*                        m_pNextEmitter;                                                         //!< Next Emitter
    Emitter*                        m_pPrevEmitter;                                                         //!< Prev Emitter
    Emitter*                        m_pNextUseComputeShaderEmitter;                                         //!< コンピュートシェーダエミッタリスト用
    detail::EmitterCalculator*      m_pEmitterCalculator;                                                   //!< エミッタ挙動計算クラス
    detail::ResEmitter*             m_pEmitterData;                                                         //!< エミッタリソース
    detail::Random                  m_Random;                                                               //!< ランダム
    detail::ParticleData*           m_pParticleData;                                                        //!< アトリビュート以外のパーティクルの情報
    detail::ParentParticleData*     m_pParentParticleData;                                                  //!< 親パーティクル固有のパーティクルの情報。
    detail::ParticleProperty*       m_ParticlePropertyFront;                                                //!< パーティクルアトリビュート( CPUアクセス可 )
    detail::ParticleProperty        m_ParticleProperty[ BufferSide_MaxBufferSide ];                         //!< パーティクル情報アトリビュート（表：現在使用するバッファ）
    detail::ParticleProperty        m_CpuParticleProperty;                                                  //!< CPUパーティクル向けアトリビュート
    void*                           m_ParticleDynamicBuffer;                                                //!< CPUパーティクル向けアトリビュートを確保したアドレス
    int                             m_MaxParticleAttrFill;                                                  //!< パーティクル描画アトリビュートバッファ Fill可能数
    int                             m_ParticleAttrFillIndex;                                                //!< パーティクル描画アトリビュートバッファ 現状のFillインデックス
    const EmitterResource*          m_pEmitterRes;                                                          //!< エミッタリソースセット
    const EmitterResource*          m_pChildEmitterResSet[ SystemParameters_MaxEmitterInclusionCount ];     //!< 子エミッタリソースセット
    nn::util::Matrix4x3fType        m_ResMatrixSrt;                                                         //!< エミッタマトリクス
    nn::util::Matrix4x3fType        m_ResMatrixRt;                                                          //!< エミッタマトリクスRT
    nn::util::Matrix4x3fType        m_MatrixSrt;                                                            //!< エミッタマトリクス
    nn::util::Matrix4x3fType        m_MatrixRt;                                                             //!< エミッタマトリクスRT
    nn::util::Vector3fType          m_EmitterPrevPos;                                                       //!< １フレーム前のエミッタ位置
    nn::util::Vector3fType          m_EmitterLocalVec;                                                      //!< １フレーム前のエミッタ速度
    CallbackSet*                    m_pCallbackSet[ detail::CallbackSetType_MaxCallbackSetType ];           //!< コールバック 0:CustomAction 1:CustomShader 2:EmitterPlugin
    DrawPathRenderStateSetCallback  m_DrawPathCallback;                                                     //!< 描画パスコールバック
    uint32_t                        m_DrawPathFlag;                                                         //!< 描画パス
    void*                           m_pCustomActionUserData;                                                //!< カスタムアクションユーザーデータ
    void*                           m_pCustomShaderUserData;                                                //!< カスタムシェーダユーザーデータ
    void*                           m_pEmitterPluginUserData;                                               //!< エミッタプラグインユーザーデータ
    nn::util::Vector4fType          m_Color0;                                                               //!< エミッタカラー0
    nn::util::Vector4fType          m_Color1;                                                               //!< エミッタカラー1

    // TODO : なるべく : 親エミッタに関するデータを外部化
    Emitter*                        m_pChildHead[ SystemParameters_MaxEmitterInclusionCount ];              //!< チャイルドエミッタリスト
    Emitter*                        m_pChildTail[ SystemParameters_MaxEmitterInclusionCount ];              //!< チャイルドエミッタリスト
    Emitter*                        m_pNextChild;                                                           //!< チャイルドエミッタ
    Emitter*                        m_pParentEmitter;                                                       //!< 親エミッタ
    int                             m_ParentEmitterCreateId;                                                //!< 親エミッタの生成ID
    int                             m_ParentParticleCreateId;                                               //!< 親パーティクルの生成ID
    int                             m_ParentParticleAttrIndex;                                              //!< 親となるパーティクルのインデックス
    float                           m_ParentParticleLife;                                                   //!< 親パーティクルの寿命
    float                           m_ParentParticleBirthTime;                                              //!< 親パーティクルの生成時間
    float                           m_ParentParticleTime;                                                   //!< 親となるパーティクルの時間
    nn::util::Vector3fType          m_ParentParticleLocalPos;                                               //!< 親となるパーティクルのローカル座標
    nn::util::Vector3fType          m_ParentParticleLocalVec;                                               //!< 親となるパーティクルのローカル速度
    nn::util::Vector4fType          m_ParentParticleScale;                                                  //!< 親となるパーティクルのスケール値
    nn::util::Vector4fType          m_ParentParticleRotate;                                                 //!< 親となるパーティクルの回転値
    nn::util::Vector4fType          m_ParentParticleRandom;                                                 //!< 親となるパーティクルのランダム値
    nn::util::Vector3fType          m_ParentParticleWorldPos;                                               //!< 親となるパーティクルのワールド座標
    nn::util::Vector3fType          m_ParentParticleWorldVec;                                               //!< 親となるパーティクルのワールドベクトル
    bool                            m_IsParticleAttributeDoubleBuffer;                                      //!< Particle Attributeがダブルバッファかどうか
    uint32_t                        m_ProcessEmitterFlag;                                                   //!< 処理するストリームアウトエミッタのフラグ値
    nn::util::Vector3fType*         m_pChildEmitterTempWorldPos;                                            //!< 軽量版チャイルド専用：親パーティクルの位置
    nn::util::Vector3fType*         m_pChildEmitterTempWorldVec;                                            //!< 軽量版チャイルド専用：親パーティクルの速度
    int                             m_ChildEmitterTempParentIndex;                                          //!< 軽量版チャイルド専用：親パーティクルのIndex
    bool                            m_IsSoloFade;                                                           //!< （エミッタ単独で） フェード中かどうか

    union
    {
        EmitterAnimValue            m_EmitterAnimation;                                                         //!< エミッタ時間アニメーション計算結果格納変数
        nn::util::Float3            m_EmitterAnimationVec[ detail::EmitterAnimationType_MaxAnimationType ];     //!< エミッタ時間アニメーション計算結果格納変数
    };
    bool                            m_EmitterAnimationEndFlag[ detail::EmitterAnimationType_MaxAnimationType ]; //!< エミッタ時間アニメーションの終了判定
    int                             m_LastIndexParticleCreation;                                            //!< 最後に作ったパーティクルのインデックス
    bool                            m_AbortEmitSearch;                                                      //!< サーチを諦めるフラグ
    bool                            m_ForceAbortEmitSearch;                                                 //!< 強制的にサーチを諦めるフラグ

private:
    //---------------------------------------------------------------------------
    // FillZeroされるので必ず最後段
    //---------------------------------------------------------------------------
    GfxObjects m_GfxObjects;
    DynamicHeap                     m_DynamicHeap;                                                          //!< 動的ヒープ

    friend class System;
    friend class detail::EmitterCalculator;
    friend class EmitterSet;
    friend class ParticleCalculateArgImpl;
};

//---------------------------------------------------------------------------
//  ワールド座標系への変換
//---------------------------------------------------------------------------
inline void Emitter::TransformToWorldPos( nn::util::Vector3fType* pOutPos, const nn::util::Vector3fType& src, int particleIndex ) const NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL( pOutPos );
    NN_SDK_REQUIRES_RANGE( particleIndex, 0, m_MaxParticleAttrFill );

    nn::util::Matrix4x3f matrix;
    GetEmitterTransformMatrix( &matrix, particleIndex );

    nn::util::VectorTransform( pOutPos, src, matrix );
}

//---------------------------------------------------------------------------
//  ローカル座標系への変換
//---------------------------------------------------------------------------
inline void Emitter::TransformToLocalPos( nn::util::Vector3fType* pOutPos, const nn::util::Vector3fType& src, int particleIndex ) const NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL( pOutPos );
    NN_SDK_REQUIRES_RANGE( particleIndex, 0, m_MaxParticleAttrFill );

    nn::util::Matrix4x3f matrix;
    GetEmitterTransformMatrix( &matrix, particleIndex );

    // 逆行列に変換して適用
    nn::util::MatrixInverse( &matrix, matrix );
    nn::util::VectorTransform( pOutPos, src, matrix );
}

//---------------------------------------------------------------------------
//  ローカル座標系への変換
//---------------------------------------------------------------------------
inline void Emitter::TransformToLocalVec( nn::util::Vector3fType* pOutVec, const nn::util::Vector3fType& src, int particleIndex ) const NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL( pOutVec );
    NN_SDK_REQUIRES_RANGE( particleIndex, 0, m_MaxParticleAttrFill );

    nn::util::Matrix4x3f matrix;
    GetEmitterTransformMatrix( &matrix, particleIndex );

    // オフセット成分を消す。
    const nn::util::Vector3f zero( 0, 0, 0 );
    nn::util::MatrixSetAxisW( &matrix, zero );

    // 逆行列に変換して適用
    nn::util::MatrixInverse( &matrix, matrix );
    nn::util::VectorTransform( pOutVec, src, matrix );
}

//---------------------------------------------------------------------------
//  ローカル座標系への変換
//---------------------------------------------------------------------------
inline void Emitter::TransformToWorldVec( nn::util::Vector3fType* pOutVec, const nn::util::Vector3fType& src, int particleIndex ) const NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL( pOutVec );
    NN_SDK_REQUIRES_RANGE( particleIndex, 0, m_MaxParticleAttrFill );

    nn::util::Matrix4x3f matrix;
    GetEmitterTransformMatrix( &matrix, particleIndex );

    // オフセット成分を消す。
    const nn::util::Vector3f zero( 0, 0, 0 );
    nn::util::MatrixSetAxisW( &matrix, zero );

    nn::util::VectorTransform( pOutVec, src, matrix );
}

//------------------------------------------------------------------------------
//  パーティクル位置をワールド座標系に変換するための、追従設定を加味した変換行列を取得します。
//------------------------------------------------------------------------------
inline void Emitter::GetEmitterTransformMatrix( nn::util::Matrix4x3f* pOutMatrix, int index ) const NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL( pOutMatrix );
    NN_SDK_REQUIRES_RANGE( index, 0, m_MaxParticleAttrFill );

    // 放出時のエミッタマトリクス
    const nn::util::Float4* pEmitterSrt0 = &m_CpuParticleProperty.emitterMatrixSrt0[ index ];
    const nn::util::Float4* pEmitterSrt1 = &m_CpuParticleProperty.emitterMatrixSrt1[ index ];
    const nn::util::Float4* pEmitterSrt2 = &m_CpuParticleProperty.emitterMatrixSrt2[ index ];

    // 追従設定を見て、適切に行列を作成して返す。
    if( m_pEmitterData->emitter.followType == detail::ParticleFollowType_EmitterFull )
    {
        // 完全追従
        *pOutMatrix = m_MatrixSrt;
    }
    else if( m_pEmitterData->emitter.followType == detail::ParticleFollowType_EmitterPosition )
    {
        // 位置のみ追従
        // エミッタSRT行列の translate 成分
        nn::util::Vector3fType translate;
        nn::util::MatrixGetAxisW( &translate, m_MatrixSrt );

        *pOutMatrix = nn::util::Matrix4x3f(
            pEmitterSrt0->x, pEmitterSrt1->x, pEmitterSrt2->x,
            pEmitterSrt0->y, pEmitterSrt1->y, pEmitterSrt2->y,
            pEmitterSrt0->z, pEmitterSrt1->z, pEmitterSrt2->z,
            nn::util::VectorGetX( translate ), nn::util::VectorGetY( translate ), nn::util::VectorGetZ( translate ) );
    }
    else
    {
        // 追従なし
        *pOutMatrix = nn::util::Matrix4x3f(
            pEmitterSrt0->x, pEmitterSrt1->x, pEmitterSrt2->x,
            pEmitterSrt0->y, pEmitterSrt1->y, pEmitterSrt2->y,
            pEmitterSrt0->z, pEmitterSrt1->z, pEmitterSrt2->z,
            pEmitterSrt0->w, pEmitterSrt1->w, pEmitterSrt2->w );
    }
}

// バッファカッター
class BufferCutter
{
    NN_DISALLOW_COPY( BufferCutter );
public:
    BufferCutter() NN_NOEXCEPT
    {
        m_Buffer = NULL;
        m_AllocatedSize = 0;
    }

    void Initialize( void* buffer, size_t bufferSize ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( buffer );
        m_Buffer = buffer;
        m_BufferSize = bufferSize;
    }

    void* Cut( size_t size ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( m_AllocatedSize + size <= m_BufferSize );
        void* ptr = nn::util::BytePtr( m_Buffer ).Advance( m_AllocatedSize ).Get();
        m_AllocatedSize += size;
        return ptr;
    }

    void* Cut( size_t size, size_t align ) NN_NOEXCEPT
    {
        size_t alignedStart = nn::util::align_up( m_AllocatedSize, align );
        NN_SDK_ASSERT( alignedStart + size <= m_BufferSize );
        void* ptr = nn::util::BytePtr( m_Buffer ).Advance( alignedStart ).Get();
        m_AllocatedSize = alignedStart + size;
        return ptr;
    }

private:
    void*                   m_Buffer;           //!< フレームヒープバッファ
    size_t                  m_BufferSize;       //!< バッファサイズ
    size_t                  m_AllocatedSize;    //!< 確保したサイズ
};

} // namespace vfx
} // namespace nn
