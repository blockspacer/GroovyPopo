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

#include <nn/atk/atk_Config.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_Adpcm.h>
#include <nn/atk/atk_LowLevelVoice.h>
#include <nn/atk/atk_ProfileReader.h>
#include <nn/util/util_BitFlagSet.h>

/*! @file
    @brief      Voice に関する関数、およびクラス定義
*/

namespace nn  { namespace atk { namespace detail {

/*!
    @brief      Voice を操作するクラスです。
 */
class Voice
{
    friend class LowLevelVoice;

private:
    NN_DISALLOW_COPY(Voice);

public:
    static const uint32_t PriorityMin = 0;
    static const uint32_t PriorityMax = 255;
    static const uint32_t PriorityNoDrop = PriorityMax;

    Voice() NN_NOEXCEPT;
    ~Voice() NN_NOEXCEPT;

    bool AllocVoice( uint32_t priority ) NN_NOEXCEPT;
    void Free() NN_NOEXCEPT;

    bool IsAvailable() const NN_NOEXCEPT;

    /*!
        @brief      ボイスの状態を設定します。
        @param[in]  state       状態
     */
    void SetState(VoiceState state) NN_NOEXCEPT;

    /*!
        @name       バッファ追加
        @{
     */
    /*!
        @brief      ボイスに対してサンプルデータ情報を追加します。
        @param[in]  waveBuffer  バッファ情報構造体のポインタ
     */
    void AppendWaveBuffer( WaveBuffer* waveBuffer ) NN_NOEXCEPT;
    /*!
        @}
     */

    /*!
        @name       パラメータ設定
        @{
     */
    /*
        @brief      ボイスのチャンネル数を設定します。
        @param[in]  channelCount    チャンネル数
     */
//    void SetChannelCount(int channelCount);

    /*!
        @brief      ボイスにリンクされるサンプルの形式を設定します。
        @param[in]  format      サンプルの形式
     */
    void SetSampleFormat(SampleFormat format) NN_NOEXCEPT { m_SampleFormat = format; }

    /*
        @brief      再生開始時にボリューム 0 からの短いフェードインを用いるかどうかを指定します。
        @param[in]  flag        On/Off フラグ
     */
//    void SetStartFrameFadeInFlag(bool flag);

    /*!
        @brief      ボイスのサンプルレートを設定します。
        @param[in]  sampleRate  サンプルレート
     */
    void SetSampleRate(uint32_t sampleRate) NN_NOEXCEPT { m_SampleRate = sampleRate; }

    /*!
        @brief      Adpcm の係数を設定します。
        @param[in]  param Adpcm 係数構造体への参照
     */
    void SetAdpcmParam(const AdpcmParam& param) NN_NOEXCEPT { m_AdpcmParam = param; }

    /*!
        @brief      ボイスの優先度を設定します。
        @param[in]  priority    優先度
     */
    void SetPriority(uint32_t priority) NN_NOEXCEPT;

    /*!
        @brief      ボイスのボリュームを設定します。
        @param[in]  volume      ボリューム値
     */
    void SetVolume(float volume) NN_NOEXCEPT { m_VoiceParam.m_Volume = volume; }

    /*!
        @brief      ボイスのピッチを設定します。
        @param[in]  pitch       サンプルレートに対する再生速度比
     */
    void SetPitch(float pitch) NN_NOEXCEPT { m_VoiceParam.m_Pitch = pitch; }

    /*!
        @brief      ボイスの各チャンネルのゲインを設定します。
        @param[in]  mix    ゲイン構造体への参照
     */
    void SetTvMix( const OutputMix& mix ) NN_NOEXCEPT { m_VoiceParam.m_TvMix = mix; }

    void SetInterpolationType( uint8_t interpolationType ) NN_NOEXCEPT { m_VoiceParam.m_InterpolationType = interpolationType; }

    void SetMonoFilter( bool enable, uint16_t cutoff = 0 ) NN_NOEXCEPT;
    void SetBiquadFilter( bool enable, const BiquadFilterCoefficients* coef = NULL ) NN_NOEXCEPT;

    void SetOutputReceiver( OutputReceiver* pOutputReceiver ) NN_NOEXCEPT
    {
        m_pOutputReceiver = pOutputReceiver;
    }

    void UpdateParam() NN_NOEXCEPT;

    // #ifdef NN_ATK_CONFIG_ENABLE_VOICE_COMMAND
    void UpdateVoiceStatus() NN_NOEXCEPT;
    // #endif

    /*!
        @name       再生状況取得
        @{
     */
    /*!
        @brief      ボイスの状態を取得します。
        @return     状態を返します。
     */
    VoiceState GetState() const NN_NOEXCEPT { return m_State; }

    /*!
        @brief      使用中のバッファ内での再生位置を取得します。
        @return     再生位置をサンプル数で返します。
     */
    position_t GetPlayPosition() const NN_NOEXCEPT;

    /*!
        @brief      ボイスの優先度を取得します。
        @return     優先度を返します。
     */
    uint32_t GetPriority() const NN_NOEXCEPT { return m_Priority; }

    float GetVolume() const NN_NOEXCEPT { return m_VoiceParam.m_Volume; }
    float GetPitch() const NN_NOEXCEPT { return m_VoiceParam.m_Pitch; }

    const OutputMix& GetTvMix() const NN_NOEXCEPT { return m_VoiceParam.m_TvMix; }

    nn::os::Tick GetProcessTick(const SoundProfile& profile) NN_NOEXCEPT
    {
        if ( m_pLowLevelVoice != nullptr )
        {
            // NodeId が一致するボイスの内部で使用する nn::audio:::AudioRenderer 上での処理負荷を、プロファイル情報から取得する
            nn::audio::NodeId voiceNodeId = m_pLowLevelVoice->GetNodeId();
            for (auto voiceIndex = 0U; voiceIndex < profile.rendererVoiceCount; voiceIndex++)
            {
                if (profile._voiceIdTable[voiceIndex] == voiceNodeId)
                {
                    return profile._voiceProcessTable[voiceIndex].end - profile._voiceProcessTable[voiceIndex].begin;
                }
            }
        }
        return nn::os::Tick(0);
    }

    /*!
        @brief      ボイスの再生状態を取得します。
        @return     ボイスの再生状態を返します。
     */
//    bool IsPlaying() const;
    /*!
        @}
     */

    /*!
        @}
     */

private:
    void Initialize( uint32_t priority ) NN_NOEXCEPT;
    void FreeAllWaveBuffer() NN_NOEXCEPT;

    //-------------------------
    // プライオリティ
    //-------------------------
    uint32_t                 m_Priority;

    //-------------------------
    // パラメータ
    //-------------------------
    VoiceState m_State;
    VoiceParam m_VoiceParam;

    SampleFormat        m_SampleFormat;
    uint32_t            m_SampleRate;
    AdpcmParam          m_AdpcmParam;
    OutputReceiver*     m_pOutputReceiver;

    //-------------------------
    // ボイス実装への参照
    //-------------------------
    // #ifdef NN_ATK_CONFIG_ENABLE_VOICE_COMMAND
    uint32_t m_VoiceId;
    position_t m_PlayPosition;
    uint32_t m_VoiceInfoEnableFlag;
    uint32_t m_CommandTag;
    WaveBuffer*         m_WaveBufferListBegin;
    WaveBuffer*         m_WaveBufferListEnd;
    // #else
    LowLevelVoice* m_pLowLevelVoice;
    // #endif
}; // class Voice

class VirtualVoiceManager
{
public:
    static const uint32_t InvalidVoiceId = 0xffffffff;

    static const int VirtualVoiceCount = 256; // NOTE: 32の倍数

    static NN_NOINLINE VirtualVoiceManager& GetInstance() NN_NOEXCEPT;

    void Initialize() NN_NOEXCEPT;

    uint32_t AllocVirtualVoice() NN_NOEXCEPT;
    void FreeVirtualVoice(uint32_t id) NN_NOEXCEPT;

    void SetLowLevelVoice( uint32_t voiceId, LowLevelVoice* lowLevelVoice ) NN_NOEXCEPT { m_LowLevelVoiceTable[voiceId] = lowLevelVoice; }

    LowLevelVoice* GetLowLevelVoice( uint32_t voiceId ) NN_NOEXCEPT { return m_LowLevelVoiceTable[voiceId]; }

    VoiceInfo* GetVoiceInfo( uint32_t voiceId ) NN_NOEXCEPT { return &m_VoiceInfoTable[m_VoiceInfoTableRead][voiceId]; }
    const VoiceInfo* GetVoiceInfo( uint32_t voiceId ) const NN_NOEXCEPT { return &m_VoiceInfoTable[m_VoiceInfoTableRead][voiceId]; }

    void UpdateVoiceInfo() NN_NOEXCEPT;

    // 確保済みの仮想ボイスの数を取得します。実際に発音中のボイスの数ではないためご注意ください。
    int GetAllocatedVirtualVoiceCount() const NN_NOEXCEPT;

    // 未解放の LowLevelVoice の数を取得します。
    // GetAllocatedVirtualVoiceCount() が 0 でも VoiceCommand が残っている場合 false が返ってくることがあるため注意してください。
    int GetUnreleasedLowLevelVoiceCount() const NN_NOEXCEPT;

private:
    VirtualVoiceManager() NN_NOEXCEPT
    {
        Initialize();
    }

    static const int VirtualVoiceElementCount = VirtualVoiceCount / 32;
    uint32_t m_VirtualVoiceAllocationTable[VirtualVoiceElementCount];

    uint32_t m_VoiceInfoTableRead;
    LowLevelVoice* m_LowLevelVoiceTable[ VirtualVoiceCount ];
    VoiceInfo m_VoiceInfoTable[2][ VirtualVoiceCount ];
    nn::util::BitFlagSet<VirtualVoiceCount> m_VoiceInfoDirtyTable[2];
};

}}} // namespace nn::atk::detail
