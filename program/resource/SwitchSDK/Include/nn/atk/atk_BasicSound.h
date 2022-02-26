/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * :include nn/atk/atk_BasicSound.h
 *
 * @file atk_BasicSound.h
 */

#pragma once

#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_MoveValue.h>
#include <nn/atk/fnd/basis/atkfnd_Inlines.h>
#include <nn/atk/fnd/basis/atkfnd_RuntimeTypeInfo.h>
#include <nn/util/util_IntrusiveList.h>
#include <nn/atk/atk_ChannelMixVolume.h>
#include <nn/atk/atk_ValueArray.h>
#include <nn/atk/atk_OutputAdditionalParam.h>

namespace nn {
namespace atk {

class SoundHandle;
class SoundPlayer;
class SoundActor;
class SoundArchive;
class OutputReceiver;

// ------------------------------------------------------------------------
// アンビエントパラメータ

//---------------------------------------------------------------------------
//! @brief  出力先別のアンビエントパラメータ構造体です。
//!
//! @see SoundAmbientParam
//! @see AuxBus
//---------------------------------------------------------------------------
class OutputAmbientParam
{
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //!
    //!         内部で @ref Initialize を呼び出します。
    //---------------------------------------------------------------------------
    OutputAmbientParam() NN_NOEXCEPT
    {
        Initialize();
    }

    //---------------------------------------------------------------------------
    //! @brief  メンバー変数を初期化します。
    //!
    //!         下記のように初期化されます。
    //!         - volume = 1.0f;
    //!         - pan = 0.0f;
    //!         - surroundPan = 0.0f;
    //!         - effectSend[...] = 0.0f;
    //---------------------------------------------------------------------------
    void Initialize() NN_NOEXCEPT
    {
        m_Volume = 1.0f;
        m_Pan = m_SurroundPan = 0.0f;
        for ( int i = 0; i < AuxBus_Count; i++ )
        {
            m_FxSend[i] = 0.0f;
        }
    };

    //---------------------------------------------------------------------------
    //! @brief  当該出力先に対する音量を取得します。
    //---------------------------------------------------------------------------
    float GetVolume() const NN_NOEXCEPT
    {
        return m_Volume;
    }

    //---------------------------------------------------------------------------
    //! @brief  当該出力先に対する音量を設定します。
    //!
    //!         0.0 以上の倍率で指定します。他のメイン出力音量と掛け合わされます。
    //!
    //! @param[in] volume 当該出力先に対する音量です。
    //---------------------------------------------------------------------------
    void SetVolume(float volume) NN_NOEXCEPT
    {
        m_Volume = volume;
    }

    //---------------------------------------------------------------------------
    //! @brief  当該出力先に対するパン (左右の定位) の相対変化量を取得します。
    //---------------------------------------------------------------------------
    float GetPan() const NN_NOEXCEPT
    {
        return m_Pan;
    }

    //---------------------------------------------------------------------------
    //! @brief  当該出力先に対するパン (左右の定位) の相対変化量を設定します。
    //!
    //!         0.0 を指定すると変化しません。
    //!         1.0 を指定すると中央に定位していた音が右端に定位するようになり、
    //!         -1.0 を指定すると中央に定位していた音が左端に定位するようになります。
    //!
    //! @param[in] pan 当該出力先に対するパン (左右の定位) の相対変化量です。
    //---------------------------------------------------------------------------
    void SetPan(float pan) NN_NOEXCEPT
    {
        m_Pan = pan;
    }

    //---------------------------------------------------------------------------
    //! @brief  当該出力先に対するサラウンドパン（前後の定位）の相対変化量を取得します。
    //---------------------------------------------------------------------------
    float GetSurroundPan() const NN_NOEXCEPT
    {
        return m_SurroundPan;
    }

    //---------------------------------------------------------------------------
    //! @brief  当該出力先に対するサラウンドパン（前後の定位）の相対変化量を設定します。
    //!
    //!         0.0 を指定すると変化しません。
    //!         1.0 を指定すると最前方に定位していた音が中央に定位するようになり、
    //!         2.0 を指定すると最前方に定位していた音が最後方に定位するようになります。
    //!         前方へ定位を移動させたい場合は負の値を指定します。
    //!
    //! @param[in] surroundPan 当該出力先に対するサラウンドパン（前後の定位）の相対変化量です。
    //---------------------------------------------------------------------------
    void SetSurroundPan(float surroundPan) NN_NOEXCEPT
    {
        m_SurroundPan = surroundPan;
    }

    //---------------------------------------------------------------------------
    //! @brief  当該出力先に対するエフェクトセンドの相対変化量を取得します。
    //!
    //! @param[in] auxBusIndex 対象となる Aux バスのインデックスを指定します。
    //---------------------------------------------------------------------------
    float GetEffectSend(int auxBusIndex) const NN_NOEXCEPT
    {
        return m_FxSend[auxBusIndex];
    }

    //---------------------------------------------------------------------------
    //! @brief  当該出力先に対するエフェクトセンドの相対変化量を設定します。
    //!
    //!         0.0 を指定するとセンド量を変更しません。
    //!         1.0 を指定すると AUX バスに送られていなかったサウンドが
    //!         最大のセンド量で送られるようになります。
    //!
    //! @param[in] auxBusIndex 対象となる Aux バスのインデックスを指定します。
    //! @param[in] effectSend 当該出力先に対するエフェクトセンドの相対変化量です。
    //---------------------------------------------------------------------------
    void SetEffectSend(int auxBusIndex, float effectSend) NN_NOEXCEPT
    {
        m_FxSend[auxBusIndex] = effectSend;
    }

private:
    float m_Volume;
    float m_Pan;
    float m_SurroundPan;
    float m_FxSend[AuxBus_Count];
};

//! @briefprivate
class SoundParam
{
public:
    SoundParam() NN_NOEXCEPT
    {
        Initialize();
    }

    void Initialize() NN_NOEXCEPT
    {
        m_Volume            = 1.0f;
        m_Pitch             = 1.0f;
        m_Lpf               = 0.0f;
        m_BiquadFilterValue = 0.0f;
        m_BiquadFilterType  = BiquadFilterType_Inherit;
        m_Priority          = 0;
        m_UserData          = 0;

        m_OutputLineFlag    = -1;

        m_TvParam.Initialize();
    }

    float GetVolume() const NN_NOEXCEPT
    {
        return m_Volume;
    }

    void SetVolume(float volume) NN_NOEXCEPT
    {
        m_Volume = volume;
    }

    float GetPitch() const NN_NOEXCEPT
    {
        return m_Pitch;
    }

    void SetPitch(float pitch) NN_NOEXCEPT
    {
        m_Pitch = pitch;
    }

    float GetLpf() const NN_NOEXCEPT
    {
        return m_Lpf;
    }

    void SetLpf(float lpf) NN_NOEXCEPT
    {
        m_Lpf = lpf;
    }

    float GetBiquadFilterValue() const NN_NOEXCEPT
    {
        return m_BiquadFilterValue;
    }

    void SetBiquadFilterValue(float biquadFilterValue) NN_NOEXCEPT
    {
        m_BiquadFilterValue = biquadFilterValue;
    }

    int GetBiquadFilterType() const NN_NOEXCEPT
    {
        return m_BiquadFilterType;
    }

    void SetBiquadFilterType(int biquadFilterType) NN_NOEXCEPT
    {
        m_BiquadFilterType = biquadFilterType;
    }

    int GetPriority() const NN_NOEXCEPT
    {
        return m_Priority;
    }

    void SetPriority(int priority) NN_NOEXCEPT
    {
        m_Priority = priority;
    }

    int GetOutputLineFlag() const NN_NOEXCEPT
    {
        return m_OutputLineFlag;
    }

    void SetOutputLineFlag(int outputLineFlag) NN_NOEXCEPT
    {
        m_OutputLineFlag = outputLineFlag;
    }

    uint32_t GetUserData() const NN_NOEXCEPT
    {
        return m_UserData;
    }

    void SetUserData(uint32_t userData) NN_NOEXCEPT
    {
        m_UserData = userData;
    }

    OutputAmbientParam& GetTvParam() NN_NOEXCEPT
    {
        return m_TvParam;
    }

    const OutputAmbientParam& GetTvParam() const NN_NOEXCEPT
    {
        return m_TvParam;
    }

    void SetTvParam(const OutputAmbientParam& tvParam) NN_NOEXCEPT
    {
        m_TvParam = tvParam;
    }

private:
    float m_Volume;
    float m_Pitch;
    float m_Lpf;
    float m_BiquadFilterValue;
    int m_BiquadFilterType;
    int m_Priority;
    int m_OutputLineFlag;
    uint32_t m_UserData;
    OutputAmbientParam m_TvParam;
};

//---------------------------------------------------------------------------
//! @brief  アンビエントパラメータ構造体です。
//!
//!         3D サウンドを使用している際には、
//!         前回計算された 3D サウンド計算結果を参照するため、そして、
//!         3D サウンドエンジンクラスの計算結果を格納するために用いられます。
//!         詳しくは、@ref Sound3DEngine::UpdateAmbientParam 関数のリファレンスをご参照ください。
//!
//!         Biquad フィルタは複数の箇所での設定が重ね合わされず、
//!         以下の優先度に従って設定されます。
//!         優先度が高い箇所でパラメータの設定がされた場合、それより下位の設定は上書きされます。
//!
//!         - サウンドハンドルでの設定
//!         - サウンドプレイヤーでの設定
//!         - 当アンビエントパラメータ構造体での設定
//!         - シーケンスデータでの設定
//!
//! @see Sound3DEngine::UpdateAmbientParam
//! @see SoundHandle::SetEffectSend
//! @see OutputAmbientParam
//---------------------------------------------------------------------------
class SoundAmbientParam
{
public:
    //---------------------------------------------------------------------------
    //! @brief  サウンドの出力先が SoundHandle などの設定を継承することを示す値です。
    //!
    //! @see SetOutputLineFlag
    //---------------------------------------------------------------------------
    static const int OutputLineFlagInherit = -1;

    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //---------------------------------------------------------------------------
    SoundAmbientParam() NN_NOEXCEPT
    : m_Volume(1.0f),
      m_Pitch(1.0f),
      m_Lpf(0.0f),
      m_BiquadFilterValue(0.0f),
      m_BiquadFilterType(BiquadFilterType_Inherit),
      m_Priority(0),
      m_UserData(0),
      m_OutputLineFlag(OutputLineFlagInherit)
    {
        m_TvParam.Initialize();
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドの音量の倍率を取得します。
    //---------------------------------------------------------------------------
    float GetVolume() const NN_NOEXCEPT
    {
        return m_Volume;
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドの音量の倍率を設定します。0.0 を指定すると発音されません。
    //!
    //! @param[in] volume サウンドの音量の倍率です。
    //---------------------------------------------------------------------------
    void SetVolume(float volume) NN_NOEXCEPT
    {
        m_Volume = volume;
    }
    //---------------------------------------------------------------------------
    //! @brief  サウンドの音程の周波数比率を取得します。
    //---------------------------------------------------------------------------
    float GetPitch() const NN_NOEXCEPT
    {
        return m_Pitch;
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドの音程の周波数比率を設定します。
    //!
    //!         1.0 を指定すると変化しません。
    //!         2.0 を指定すると再生される周波数が 2 倍になり、1 オクターブ高い音程になります。
    //!
    //! @param[in] pitch サウンドの音程の周波数比率です。
    //---------------------------------------------------------------------------
    void SetPitch(float pitch) NN_NOEXCEPT
    {
        m_Pitch = pitch;
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドのローパスフィルタのカットオフの相対変化量を取得します。
    //---------------------------------------------------------------------------
    float GetLowPassFilter() const NN_NOEXCEPT
    {
        return m_Lpf;
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドのローパスフィルタのカットオフの相対変化量を設定します。
    //!
    //!         0.0 を指定するとカットオフの値を変更しません。
    //!         -1.0 を指定すると、フィルタがかかっていない状態から、
    //!         最もフィルタがかかっている状態（カットオフ周波数が下がる方向）に変更します。
    //!
    //! @param[in] lowPassFilter サウンドのローパスフィルタのカットオフの相対変化量です。
    //---------------------------------------------------------------------------
    void SetLowPassFilter(float lowPassFilter) NN_NOEXCEPT
    {
        m_Lpf = lowPassFilter;
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドの Biquad フィルタのかかり具合を表す値を取得します。
    //!         値の意味はフィルタの係数の種類によって変化します。
    //---------------------------------------------------------------------------
    float GetBiquadFilterValue() const NN_NOEXCEPT
    {
        return m_BiquadFilterValue;
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドの Biquad フィルタのかかり具合を表す値を設定します。
    //!         値の意味はフィルタの係数の種類によって変化します。
    //!
    //! @param[in] biquadFilterValue サウンドの Biquad フィルタのかかり具合を表す値です。
    //---------------------------------------------------------------------------
    void SetBiquadFilterValue(float biquadFilterValue) NN_NOEXCEPT
    {
        m_BiquadFilterValue = biquadFilterValue;
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドの Biquad フィルタの種類を取得します。
    //---------------------------------------------------------------------------
    int GetBiquadFilterType() const NN_NOEXCEPT
    {
        return m_BiquadFilterType;
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドの Biquad フィルタの種類を設定します。
    //!
    //!         @ref BiquadFilterType の値を使用します。
    //!         プリセットで用意されているフィルタの種類のほか、
    //!         ユーザーが登録したフィルタの種類の値をとります。
    //!
    //! @param[in] biquadFilterType サウンドの Biquad フィルタの種類です。
    //---------------------------------------------------------------------------
    void SetBiquadFilterType(int biquadFilterType) NN_NOEXCEPT
    {
        m_BiquadFilterType = biquadFilterType;
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドのプレイヤープライオリティの相対変化量を取得します。
    //---------------------------------------------------------------------------
    int GetPriority() const NN_NOEXCEPT
    {
        return m_Priority;
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドのプレイヤープライオリティの相対変化量を設定します。
    //!         もともとのプレイヤープライオリティに加算されます。
    //!
    //! @param[in] priority サウンドのプレイヤープライオリティの相対変化量です。
    //---------------------------------------------------------------------------
    void SetPriority(int priority) NN_NOEXCEPT
    {
        m_Priority = priority;
    }

    //---------------------------------------------------------------------------
    //! @brief  ユーザーが自由に利用できるパラメータを取得します。
    //---------------------------------------------------------------------------
    uint32_t GetUserData() const NN_NOEXCEPT
    {
        return m_UserData;
    }

    //---------------------------------------------------------------------------
    //! @brief  ユーザーが自由に利用できるパラメータを設定します。
    //!         サウンド再生時に 0 にリセットされます。
    //!
    //! @param[in] userData ユーザーが自由に利用できるパラメータです。
    //---------------------------------------------------------------------------
    void SetUserData(uint32_t userData) NN_NOEXCEPT
    {
        m_UserData = userData;
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドの出力先を取得します。
    //---------------------------------------------------------------------------
    int GetOutputLineFlag() const NN_NOEXCEPT
    {
        return m_OutputLineFlag;
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドの出力先を設定します。
    //!         @ref OutputLineFlagInherit を指定すると SoundHandle などの設定を継承します。
    //!
    //! @param[in] outputLineFlag サウンドの出力先です。
    //---------------------------------------------------------------------------
    void SetOutputLineFlag(int outputLineFlag) NN_NOEXCEPT
    {
        m_OutputLineFlag = outputLineFlag;
    }

    //---------------------------------------------------------------------------
    //! @brief  メイン (TV) 出力に対する音量などのパラメータ群を取得します。
    //---------------------------------------------------------------------------
    OutputAmbientParam& GetTvParam() NN_NOEXCEPT
    {
        return m_TvParam;
    }

    //---------------------------------------------------------------------------
    //! @brief  メイン (TV) 出力に対する音量などのパラメータ群を設定します。
    //!
    //! @param[in] tvParam メイン (TV) 出力に対する音量などのパラメータ群です。
    //---------------------------------------------------------------------------
    void SetTvParam(const OutputAmbientParam& tvParam) NN_NOEXCEPT
    {
        m_TvParam = tvParam;
    }

private:
    float m_Volume;
    float m_Pitch;
    float m_Lpf;
    float m_BiquadFilterValue;
    int m_BiquadFilterType;
    int m_Priority;
    uint32_t m_UserData;
    int m_OutputLineFlag;
    OutputAmbientParam m_TvParam;
};

//! @briefprivate
struct SoundParamCalculationValues
{
    //! @brief サウンドアーカイブで設定されたパラメータ
    struct SoundArchiveParam
    {
        float volume;
    };

    //! @brief SoundPlayer で設定されたパラメータ
    struct SoundPlayerParam
    {
        float volume;
        float lpf;
        int   bqfType;
        float bqfValue;
        float outputVolume[OutputDevice_Count];
        float outputMainSend[OutputDevice_Count];
        float outputEffectSend[OutputDevice_Count][AuxBus_Count];
    };

    //! @brief Sound3D で計算されたパラメータ
    struct Sound3DParam
    {
        float volume;
        float pitch;
        float lpf;
        int   bqfType;
        float bqfValue;
        uint32_t outputLineFlag;
        float outputVolume[OutputDevice_Count];
        float outputPan[OutputDevice_Count];
        float outputSurroundPan[OutputDevice_Count];
        float outputEffectSend[OutputDevice_Count][AuxBus_Count];
        int playerPriority;
    };

    //! @brief SoundActor で設定されたパラメータ
    struct SoundActorParam
    {
        float volume;
        float pitch;
        float lpf;
        float outputVolume[OutputDevice_Count];
        float outputPan[OutputDevice_Count];
    };

    //! @brief SoundHandle で設定されたパラメータ
    struct SoundHandleParam
    {
        float volume;
        float pitch;
        float lpf;
        int   bqfType;
        float bqfValue;
        uint32_t outputLineFlag;
        float outputVolume[OutputDevice_Count];
        float outputPan[OutputDevice_Count];
        float outputSurroundPan[OutputDevice_Count];
        float outputMainSend[OutputDevice_Count];
        float outputEffectSend[OutputDevice_Count][AuxBus_Count];
        MixParameter outputMixParameter[OutputDevice_Count][WaveChannelMax];
        MixMode mixMode;
        float   pan;
        float   surroundPan;
        float   mainSend;
        float   effectSend[AuxBus_Count];
        int playerPriority;
    };

    //! @brief BasicSound までのパラメータの計算結果
    struct ResultParam
    {
        float volume;
        float pitch;
        float lpf;
        int bqfType;
        float bqfValue;
        uint32_t outputLineFlag;
        detail::OutputParam outputParamResult[OutputDevice_Count];
        int playerPriority;
    };

    //! @brief フェード Volume に関するパラメータ
    struct FadeVolumeParam
    {
        float stopFadeVolume;  //!< Stop, FadeIn によるフェード Volume
        float pauseFadeVolume; //!< Pause によるフェード Volume
        float muteFadeVolume;  //!< Mute によるフェード Volume
        bool  isMuted;         //!< ミュート中であるか
    };

    SoundArchiveParam soundArchiveParam; //!< サウンドアーカイブで設定されたパラメータ
    SoundPlayerParam  soundPlayerParam;  //!< SoundPlayer で設定されたパラメータ
    Sound3DParam      sound3DParam;      //!< Sound3D で計算されたパラメータ
    SoundActorParam   soundActorParam;   //!< SoundActor で設定されたパラメータ
    SoundHandleParam  soundHandleParam;  //!< SoundHandle で設定されたパラメータ
    ResultParam       resultParam;       //!< BasicSound までのパラメータの計算結果
    FadeVolumeParam   fadeVolumeParam;   //!< ボリューム操作に関するパラメータ
};

namespace detail {

struct SoundActorParam
{
    float volume;
    float pitch;

    float tvVolume;
    float tvPan;

    float lpf;
    int biquadFilterType;
    float biquadFilterValue;

    SoundActorParam() NN_NOEXCEPT
    {
        Reset();
    }
    void Reset() NN_NOEXCEPT
    {
        volume = pitch = tvVolume = 1.0f;
        tvPan = 0.0f;
        lpf = 0.0f;
        biquadFilterType = BiquadFilterType_Inherit;
        biquadFilterValue = 0.0f;
    }
};

/* ========================================================================
        typename declaration
   ======================================================================== */

namespace driver {
class BasicSoundPlayer;
} // nn::atk::detail::driver

class PlayerHeap;
class ExternalSoundPlayer;

/* ========================================================================
        class definition
   ======================================================================== */

class BasicSound
{
    friend class nn::atk::SoundHandle;

public:
    NN_ATK_DETAIL_RUNTIME_TYPEINFO_ROOT()

    /* ------------------------------------------------------------------------
            type definition
       ------------------------------------------------------------------------ */
public:
    class AmbientParamUpdateCallback;
    class AmbientArgUpdateCallback;
    class AmbientArgAllocatorCallback;

    struct AmbientInfo
    {
        AmbientParamUpdateCallback* paramUpdateCallback;
        AmbientArgUpdateCallback* argUpdateCallback;
        AmbientArgAllocatorCallback* argAllocatorCallback;
        void* arg;
        unsigned long argSize;
    };

    enum PlayerState
    {
        PlayerState_Init,
        PlayerState_Play,
        PlayerState_Stop
    };

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    BasicSound() NN_NOEXCEPT;
    virtual ~BasicSound() NN_NOEXCEPT { m_State = State_Destructed; }
    void Update() NN_NOEXCEPT;
    void StartPrepared() NN_NOEXCEPT;
    void Stop( int fadeFrames ) NN_NOEXCEPT;
    void ForceStop() NN_NOEXCEPT;
    void Pause(bool flag, int fadeFrames) NN_NOEXCEPT;
    void Pause( bool flag, int fadeFrames, PauseMode pauseMode ) NN_NOEXCEPT;
    void Mute( bool flag, int fadeFrames ) NN_NOEXCEPT;
    void SetAutoStopCounter( int frames ) NN_NOEXCEPT;
    void FadeIn( int frames ) NN_NOEXCEPT;

    virtual bool Initialize(OutputReceiver* pOutputReceiver) NN_NOEXCEPT;
    virtual void Finalize() NN_NOEXCEPT;

    virtual bool IsPrepared() const NN_NOEXCEPT = 0;
    bool IsPause() const NN_NOEXCEPT;
    bool IsMute() const NN_NOEXCEPT;
    bool IsStarted() const NN_NOEXCEPT { return m_StartedFlag; }

    //------------------------------------------------------------------
    // サウンド共通パラメータ
    void SetPriority( int priority, int ambientPriority ) NN_NOEXCEPT;
    void GetPriority( int* priority, int* ambientPriority ) const NN_NOEXCEPT;
    void SetInitialVolume( float volume ) NN_NOEXCEPT;
    float  GetInitialVolume() const NN_NOEXCEPT;
    void SetVolume( float volume, int frames = 0 ) NN_NOEXCEPT;
    float  GetVolume() const NN_NOEXCEPT;
    void SetPitch( float pitch ) NN_NOEXCEPT;
    float  GetPitch() const NN_NOEXCEPT;
    void SetLpfFreq( float lpfFreq ) NN_NOEXCEPT;
    float  GetLpfFreq() const NN_NOEXCEPT;
    void SetBiquadFilter( int type, float value ) NN_NOEXCEPT;
    void GetBiquadFilter( int* type, float* value ) const NN_NOEXCEPT;
    void SetOutputLine( uint32_t lineFlag ) NN_NOEXCEPT;
    uint32_t GetOutputLine() const NN_NOEXCEPT;
    void ResetOutputLine() NN_NOEXCEPT;
    void SetPlayerPriority( int priority ) NN_NOEXCEPT;
    void SetMixMode( MixMode mixMode ) NN_NOEXCEPT;
    MixMode GetMixMode() NN_NOEXCEPT;
    void SetPan( float pan ) NN_NOEXCEPT;
    float  GetPan() const NN_NOEXCEPT;
    void SetSurroundPan( float pan ) NN_NOEXCEPT;
    float  GetSurroundPan() const NN_NOEXCEPT;
    void SetMainSend( float send ) NN_NOEXCEPT;
    float  GetMainSend() const NN_NOEXCEPT;
    void SetFxSend( AuxBus bus, float send ) NN_NOEXCEPT;
    float GetFxSend( AuxBus bus ) const NN_NOEXCEPT;
    void SetSend( int bus, float send ) NN_NOEXCEPT;
    float GetSend( int bus ) const NN_NOEXCEPT;
    void SetVolumeThroughMode( int bus, uint8_t modeBitFlag ) NN_NOEXCEPT;
    uint8_t GetVolumeThroughMode( int bus ) NN_NOEXCEPT;
    bool IsVolumeThroughModeUsed() const NN_NOEXCEPT;

    int GetSendBusCount() NN_NOEXCEPT;
    int GetSendChannelCount() NN_NOEXCEPT;

    void SetPanMode( PanMode mode ) NN_NOEXCEPT;
    void SetPanCurve( PanCurve curve ) NN_NOEXCEPT;

    void SetOutputAdditionalParamAddr( OutputDevice device, OutputAdditionalParam* addr, OutputAdditionalParam* addrForPlayer ) NN_NOEXCEPT;

    void SetOutputVolume     (OutputDevice device, float volume) NN_NOEXCEPT;
    void SetOutputPan        (OutputDevice device, float pan) NN_NOEXCEPT;
    void SetOutputSurroundPan(OutputDevice device, float span) NN_NOEXCEPT;
    void SetOutputMainSend   (OutputDevice device, float send) NN_NOEXCEPT;
    void SetOutputFxSend     (OutputDevice device, AuxBus bus, float send) NN_NOEXCEPT;
    void SetOutputAdditionalSend( OutputDevice device, int bus, float send) NN_NOEXCEPT;
    void SetOutputBusMixVolumeEnabled(OutputDevice device, int subMixBus, bool isEnabled) NN_NOEXCEPT;
    void SetOutputBusMixVolume (OutputDevice device, int srcChNo, int subMixBus, ChannelMixVolume param) NN_NOEXCEPT;
    void SetOutputChannelMixParameter (OutputDevice device, uint32_t srcChNo, MixParameter param) NN_NOEXCEPT;
    void SetOutputVolumeThroughMode( OutputDevice device, int bus, uint8_t modeBitFlag ) NN_NOEXCEPT;
    float GetOutputVolume     (OutputDevice device) const NN_NOEXCEPT;
    float GetOutputPan        (OutputDevice device) const NN_NOEXCEPT;
    float GetOutputSurroundPan(OutputDevice device) const NN_NOEXCEPT;
    float GetOutputMainSend   (OutputDevice device) const NN_NOEXCEPT;
    float GetOutputFxSend     (OutputDevice device, AuxBus bus) const NN_NOEXCEPT;
    bool IsOutputBusMixVolumeEnabled(OutputDevice device, int subMixBus) const NN_NOEXCEPT;
    ChannelMixVolume GetOutputBusMixVolume (OutputDevice device, int srcChNo, int subMixBus) const NN_NOEXCEPT;
    float GetOutputAdditionalSend(OutputDevice device, int bus) const NN_NOEXCEPT;
    uint8_t GetOutputVolumeThroughMode(OutputDevice device, int bus) const NN_NOEXCEPT;
    MixParameter GetOutputChannelMixParameter (OutputDevice device, uint32_t srcChNo) const NN_NOEXCEPT;

    //------------------------------------------------------------------
    // 情報取得
    int GetRemainingFadeFrames() const NN_NOEXCEPT;
    int GetRemainingPauseFadeFrames() const NN_NOEXCEPT;
    int GetRemainingMuteFadeFrames() const NN_NOEXCEPT;
    int GetPlayerPriority() const NN_NOEXCEPT { return m_Priority; } // for AnimSound
    void CalculateSoundParamCalculationValues(SoundParamCalculationValues* pOutValue) const NN_NOEXCEPT;

    //------------------------------------------------------------------
    // その他のパラメータ
    void SetId( uint32_t id ) NN_NOEXCEPT;
    uint32_t GetId() const NN_NOEXCEPT { return m_Id; }
    uint32_t GetInstanceId() const NN_NOEXCEPT { return m_InstanceId; }
    uint32_t GetPlayFrameCount() const NN_NOEXCEPT { return m_PlayingCounter; }
    void SetSetupTick(const nn::os::Tick& tick) NN_NOEXCEPT;
    nn::os::Tick GetSetupTick() const NN_NOEXCEPT { return m_SetupTick; }

    //------------------------------------------------------------------
    int CalcCurrentPlayerPriority() const NN_NOEXCEPT
    {
        return nn::atk::detail::fnd::Clamp(
             static_cast<int>( m_Priority ) + static_cast<int>( m_AmbientParam.GetPriority() ),
             nn::atk::PlayerPriorityMin,
             nn::atk::PlayerPriorityMax
        );
    }

    // サウンドプレイヤー
    SoundPlayer* GetSoundPlayer() NN_NOEXCEPT { return m_pSoundPlayer; }
    const SoundPlayer* GetSoundPlayer() const NN_NOEXCEPT { return m_pSoundPlayer; }

    void AttachSoundPlayer( SoundPlayer* player ) NN_NOEXCEPT;
    void DetachSoundPlayer( SoundPlayer* player ) NN_NOEXCEPT;

    const SoundActor* GetSoundActor() const NN_NOEXCEPT { return m_pSoundActor; }

    void AttachSoundActor( SoundActor* actor ) NN_NOEXCEPT;
    void DetachSoundActor( SoundActor* actor ) NN_NOEXCEPT;

    void AttachExternalSoundPlayer( ExternalSoundPlayer* extPlayer ) NN_NOEXCEPT;
    void DetachExternalSoundPlayer( ExternalSoundPlayer* extPlayer ) NN_NOEXCEPT;

    // プレイヤーヒープ
    void AttachPlayerHeap( PlayerHeap* pHeap ) NN_NOEXCEPT;
    void DetachPlayerHeap( PlayerHeap* pHeap ) NN_NOEXCEPT;
    PlayerHeap* GetPlayerHeap() NN_NOEXCEPT { return m_pPlayerHeap; }

    // アンビエントパラメータ
    void SetAmbientInfo( const AmbientInfo& info ) NN_NOEXCEPT;
    void ClearAmbientArgUpdateCallback() NN_NOEXCEPT { m_AmbientInfo.argUpdateCallback = NULL; }
    void ClearAmbientParamUpdateCallback() NN_NOEXCEPT { m_AmbientInfo.paramUpdateCallback = NULL; }
    void ClearAmbientArgAllocatorCallback() NN_NOEXCEPT { m_AmbientInfo.argAllocatorCallback = NULL; }
    const SoundParam& GetAmbientParam() const NN_NOEXCEPT { return m_AmbientParam; }
    static int GetAmbientPriority( const AmbientInfo& ambientInfo, uint32_t soundId  ) NN_NOEXCEPT;

    // ハンドル関数
    bool IsAttachedGeneralHandle() NN_NOEXCEPT;
    bool IsAttachedTempGeneralHandle() NN_NOEXCEPT;
    virtual bool IsAttachedTempSpecialHandle() NN_NOEXCEPT = 0;
    void DetachGeneralHandle() NN_NOEXCEPT;
    void DetachTempGeneralHandle() NN_NOEXCEPT;
    virtual void DetachTempSpecialHandle() NN_NOEXCEPT = 0;

    void SetUserParamBuffer(void* buffer, size_t size) NN_NOEXCEPT { m_pUserParam = buffer; m_UserParamSize = size; }
    void* GetUserParam() NN_NOEXCEPT { return m_pUserParam; }

    void SetSoundStopCallback(const SoundStopCallback callback) NN_NOEXCEPT { m_SoundStopCallback = callback; }

    void SetSoundArchive( const SoundArchive* soundArchive ) NN_NOEXCEPT;
    const SoundArchive* GetSoundArchive() const NN_NOEXCEPT { return m_pSoundArchive; }

    //-----------------------------------------------------------------------------
    // PauseState状態遷移
    //
    // state \ event | pause   | unpause   | fade-finish
    // --------------+---------+-----------+--------------
    // normal        | pausing |  -        |  -
    // pausing       | pausing | unpausing | paused
    // paused        |  -      | unpausing |  -
    // unpausing     | pausing | unpausing | normal
    enum PauseState
    {
        PauseState_Normal,
        PauseState_Pausing,
        PauseState_Paused,
        PauseState_Unpausing
    };
    PauseState GetPauseState() const NN_NOEXCEPT { return static_cast<PauseState>(m_PauseState); }

    enum MuteState
    {
        MuteState_Normal,
        MuteState_Muting,
        MuteState_Muted,
        MuteState_Unmuting
    };
    MuteState GetMuteState() const NN_NOEXCEPT { return static_cast<MuteState>(m_MuteState); }

protected:
    virtual driver::BasicSoundPlayer* GetBasicSoundPlayerHandle() NN_NOEXCEPT = 0;

    virtual void OnUpdatePlayerPriority() NN_NOEXCEPT {}

    virtual void UpdateMoveValue() NN_NOEXCEPT;
    virtual void OnUpdateParam() NN_NOEXCEPT {}

    bool IsPlayerAvailable() const NN_NOEXCEPT { return m_PlayerAvailableFlag; }

private:
    enum State
    {
        State_Constructed,
        State_Initialized,
        State_Finalized,
        State_Destructed
    };

    void UpdateParam() NN_NOEXCEPT;
    void ClearIsFinalizedForCannotAllocatedResourceFlag() NN_NOEXCEPT;

    float CalculateVolume() const NN_NOEXCEPT;
    float CalculatePitch() const NN_NOEXCEPT;
    float CalculateLpfFrequency() const NN_NOEXCEPT;
    void CalculateBiquadFilter(int* pOutBiquadType, float* pOutBiquadValue) const NN_NOEXCEPT;
    uint32_t CalculateOutLineFlag() const NN_NOEXCEPT;
    void CalculateOutputParam(OutputParam* pOutParam, OutputDevice device) const NN_NOEXCEPT;
    void CalculateOutputBusMixVolume(OutputBusMixVolume* pOutParam, OutputDevice device) const NN_NOEXCEPT;

    PlayerHeap* m_pPlayerHeap;
    SoundHandle* m_pGeneralHandle;
    SoundHandle* m_pTempGeneralHandle;
    SoundPlayer* m_pSoundPlayer;
    SoundActor* m_pSoundActor;
    ExternalSoundPlayer* m_pExtSoundPlayer;
    const SoundArchive* m_pSoundArchive;

    AmbientInfo m_AmbientInfo;
    SoundParam m_AmbientParam;
    SoundActorParam m_ActorParam;

    MoveValue<float, int> m_FadeVolume;
    MoveValue<float, int> m_PauseFadeVolume;
    MoveValue<float, int> m_MuteFadeVolume;

    bool m_StartFlag;
    bool m_StartedFlag;
    bool m_AutoStopFlag;
    bool m_FadeOutFlag;

    bool m_PlayerAvailableFlag;
    bool m_UnPauseFlag;
    PauseMode m_PauseMode;
    uint8_t m_Priority;
    int8_t m_BiquadFilterType;

    State m_State;

    uint8_t m_PlayerState;   // PlayerState (以下、キャッシュ効率のため uint8_t で保持)
    uint8_t m_PauseState;    // PauseState
    uint8_t m_MuteState;     // MuteState
    uint8_t m_Padding[1];

    int32_t m_AutoStopCounter;
    uint32_t m_UpdateCounter;
    uint32_t m_PlayingCounter;
    uint32_t m_Id;
    uint32_t m_InstanceId;
    nn::os::Tick m_SetupTick;

    float m_InitVolume;
    float m_Pitch;
    float m_LpfFreq;
    float m_BiquadFilterValue;
    uint32_t m_OutputLineFlag;

    OutputReceiver* m_pOutputReceiver;

    // OutputParam の volume が MoveValue になっただけバージョン
    struct CommonParam
    {
        MoveValue<float, int> volume;
        MixMode mixMode;
        float pan;
        float span;
        float send[ DefaultBusCount ];

        void Initialize() NN_NOEXCEPT
        {
            volume.InitValue( 1.0f );
            pan = span = 0.0f;
            mixMode = MixMode_Pan;
            for (int i = 0; i < DefaultBusCount; ++i)
            {
                send[i] = 0.0f;
            }
        }
        void Update() NN_NOEXCEPT { volume.Update(); }
        float GetVolume() const NN_NOEXCEPT { return volume.GetValue(); }
        void SetVolume( float target, int frame ) NN_NOEXCEPT { volume.SetTarget(target, frame); }
    };
    void ApplyCommonParam( OutputParam& param ) const NN_NOEXCEPT;
    CommonParam m_CommonParam;

    OutputParam m_OutputParam[OutputDevice_Count];     // TV 向けパラメータ
    OutputAdditionalParam* m_pOutputAdditionalParam[OutputDevice_Count];

    void* m_pUserParam;
    size_t m_UserParamSize;
    SoundStopCallback m_SoundStopCallback;

    static uint32_t g_LastInstanceId;

public:
    util::IntrusiveListNode m_SoundPlayerPlayLink;
    util::IntrusiveListNode m_SoundPlayerPriorityLink;
    util::IntrusiveListNode m_ExtSoundPlayerPlayLink;
};



class BasicSound::AmbientParamUpdateCallback
{
public:
    virtual ~AmbientParamUpdateCallback() NN_NOEXCEPT {}
    virtual void detail_UpdateAmbientParam(
        const void* arg,
        uint32_t soundId,
        SoundAmbientParam* param
    ) NN_NOEXCEPT = 0;
    virtual int detail_GetAmbientPriority(
        const void* arg,
        uint32_t soundId
    ) NN_NOEXCEPT = 0;
};

class BasicSound::AmbientArgUpdateCallback
{
public:
    virtual ~AmbientArgUpdateCallback() NN_NOEXCEPT {}
    virtual void detail_UpdateAmbientArg(
        void* arg,
        const detail::BasicSound* sound
    ) NN_NOEXCEPT = 0;
};

class BasicSound::AmbientArgAllocatorCallback
{
public:
    virtual ~AmbientArgAllocatorCallback() NN_NOEXCEPT {}
    virtual void* detail_AllocAmbientArg( size_t argSize ) NN_NOEXCEPT = 0;
    virtual void detail_FreeAmbientArg(
        void* arg,
        const detail::BasicSound* sound
    ) NN_NOEXCEPT = 0;
};


} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

