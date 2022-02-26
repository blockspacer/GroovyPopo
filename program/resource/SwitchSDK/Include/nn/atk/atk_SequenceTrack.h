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

#include <nn/atk/atk_Channel.h>
#include <nn/atk/atk_CurveLfo.h>
#include <nn/atk/atk_MoveValue.h>
#include <nn/atk/atk_Global.h>      // SequenceMute

namespace nn {
namespace atk {
namespace detail {
namespace driver {

/* ========================================================================
        typename declaration
   ======================================================================== */

class SequenceSoundPlayer;

/* ========================================================================
        class definition
   ======================================================================== */

class SequenceTrack
{
    /* ------------------------------------------------------------------------
            type definition
       ------------------------------------------------------------------------ */
public:
    static const int CallStackDepth = 10;

    struct ParserTrackParam
    {
        const uint8_t* baseAddr;
        const uint8_t* currentAddr;

        bool cmpFlag;
        bool noteWaitFlag;
        bool tieFlag;
        bool monophonicFlag;

        struct CallStack
        {
            uint8_t loopFlag;
            uint8_t loopCount;
            uint8_t padding[2];
            const uint8_t* address;
        };
        CallStack callStack[ CallStackDepth ];
        uint8_t callStackDepth;
        bool frontBypassFlag;
        bool muteFlag;
        bool silenceFlag;

        int32_t wait;

        bool noteFinishWait;
        bool portaFlag;
        bool damperFlag;
        uint8_t  bankIndex;      // 0-3 のシーケンス内バンクインデックスと見る

        int prgNo;

        float sweepPitch;

        MoveValue<uint8_t,int16_t> volume;
        MoveValue<uint8_t,int16_t> volume2;
        MoveValue<int8_t,int16_t> pan;
        MoveValue<int8_t,int16_t> surroundPan;
        MoveValue<int8_t,int16_t> pitchBend;

        CurveLfoParam lfoParam[Channel::ModCount];
        uint8_t lfoTarget[Channel::ModCount]; // enum Channel::LfoTarget;

        uint8_t velocityRange;
        uint8_t bendRange;
        int8_t initPan;
        uint8_t padding1[1];

        int8_t transpose;
        uint8_t priority;
        uint8_t portaKey;
        uint8_t portaTime;

        uint8_t attack;
        uint8_t decay;
        uint8_t sustain;
        uint8_t release;

        int16_t envHold;
        int8_t biquadType;
        uint8_t mainSend;

        uint8_t fxSend[ AuxBus_Count ];
        uint8_t padding2[1];

        float lpfFreq;
        float biquadValue;
        int32_t outputLine;
    };

    enum ParseResult
    {
        ParseResult_Continue,
        ParseResult_Finish
    };

    /* ------------------------------------------------------------------------
            constant variable
       ------------------------------------------------------------------------ */
public:
    static const int DefaultPriority    = 64;
    static const int DefaultBendRange   = 2;
    static const int DefaultPortaKey    = 60; /* cn4 */
    static const int InvalidEnvelope    = 0xff;
    static const int MaxEnvelopeValue   = 0x7f;
    static const int ParserParamSize    = 32;
    static const int TrackVariableCount = 16;

    static const int PauseReleaseValue  = 127;
    static const int MuteReleaseValue   = 127;

    /* ------------------------------------------------------------------------
            static member
       ------------------------------------------------------------------------ */
public:
    static void ChannelCallbackFunc(
        Channel* dropChannel,
        Channel::ChannelCallbackStatus status,
        void* userData
    ) NN_NOEXCEPT;

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */

public:
    // 初期化・終了処理
    SequenceTrack() NN_NOEXCEPT;
    virtual ~SequenceTrack() NN_NOEXCEPT;

    void InitParam() NN_NOEXCEPT;
    void SetSeqData( const void* seqBase, int seqOffset ) NN_NOEXCEPT;
    void Open() NN_NOEXCEPT;
    void Close() NN_NOEXCEPT;
    bool IsOpened() const NN_NOEXCEPT { return m_OpenFlag; }

    // 更新処理
    int ParseNextTick( bool doNoteOn ) NN_NOEXCEPT;
    void UpdateChannelLength() NN_NOEXCEPT;
    void UpdateChannelParam() NN_NOEXCEPT;

    // チャンネル操作
    Channel* NoteOn(
        int key,
        int velocity,
        int32_t length,
        bool tieFlag
    ) NN_NOEXCEPT;
    void StopAllChannel() NN_NOEXCEPT;
    void ReleaseAllChannel( int release ) NN_NOEXCEPT;
    void FreeAllChannel() NN_NOEXCEPT;
    void PauseAllChannel( bool flag ) NN_NOEXCEPT;

    // 情報取得
    int GetChannelCount() const NN_NOEXCEPT;
    const ParserTrackParam& GetParserTrackParam() const NN_NOEXCEPT { return m_ParserTrackParam; }
    ParserTrackParam& GetParserTrackParam() NN_NOEXCEPT { return m_ParserTrackParam; }

    // パラメータ設定
    void SetMute( SequenceMute mute ) NN_NOEXCEPT;
    void SetSilence( bool silenceFlag, int fadeTimes ) NN_NOEXCEPT;
    void SetVolume( float volume ) NN_NOEXCEPT { m_ExtVolume = volume; }
    void SetPitch( float pitch ) NN_NOEXCEPT { m_ExtPitch = pitch; }
    void SetPanRange( float panRange ) NN_NOEXCEPT { m_PanRange = panRange; }
    void SetLpfFreq( float lpfFreq ) NN_NOEXCEPT { m_ParserTrackParam.lpfFreq = lpfFreq; }
    void SetBiquadFilter( int type, float value ) NN_NOEXCEPT;
    void SetBankIndex( int bankIndex ) NN_NOEXCEPT;
    void SetTranspose( int8_t transpose ) NN_NOEXCEPT;
    void SetVelocityRange( uint8_t range ) NN_NOEXCEPT;
    void SetOutputLine( int32_t outputLine ) NN_NOEXCEPT;

    void SetTvVolume( float volume ) NN_NOEXCEPT { m_TvParam.volume = volume; }
    void SetTvMixParameter( uint32_t srcChNo, int32_t mixChNo, float param ) NN_NOEXCEPT;
    void SetTvPan( float pan ) NN_NOEXCEPT { m_TvParam.pan = pan; }
    void SetTvSurroundPan( float span ) NN_NOEXCEPT { m_TvParam.span = span; }
    void SetTvMainSend( float send ) NN_NOEXCEPT { m_TvParam.send[Util::GetSubMixBusFromMainBus()] = send; }
    void SetTvFxSend( AuxBus bus, float send ) NN_NOEXCEPT { m_TvParam.send[Util::GetSubMixBus(bus)] = send; }

    // パラメータ取得
    float GetVolume() const NN_NOEXCEPT { return m_ExtVolume; }
    float GetPitch() const NN_NOEXCEPT { return m_ExtPitch; }
    float GetPanRange() const NN_NOEXCEPT { return m_PanRange; }
    float GetLpfFreq() const NN_NOEXCEPT { return m_ParserTrackParam.lpfFreq; }
    int GetBiquadType() const NN_NOEXCEPT { return m_ParserTrackParam.biquadType; }
    float GetBiquadValue() const NN_NOEXCEPT { return m_ParserTrackParam.biquadValue; }

    // シーケンス処理
    int16_t GetTrackVariable( int varNo ) const NN_NOEXCEPT;
    void SetTrackVariable( int varNo, int16_t var ) NN_NOEXCEPT;
    volatile int16_t* GetVariablePtr( int varNo ) NN_NOEXCEPT;

    void SetSequenceSoundPlayer( SequenceSoundPlayer* player ) NN_NOEXCEPT { m_pSequenceSoundPlayer = player; }
    const SequenceSoundPlayer* GetSequenceSoundPlayer() const NN_NOEXCEPT { return m_pSequenceSoundPlayer; }
    SequenceSoundPlayer* GetSequenceSoundPlayer() NN_NOEXCEPT { return m_pSequenceSoundPlayer; }

    void SetPlayerTrackNo( int playerTrackNo ) NN_NOEXCEPT;
    uint8_t GetPlayerTrackNo() const NN_NOEXCEPT { return m_PlayerTrackNo; }

    void UpdateChannelRelease( Channel* channel ) NN_NOEXCEPT;

    nn::os::Tick GetProcessTick(const SoundProfile& profile) NN_NOEXCEPT
    {
        nn::os::Tick totalTick(0);
        if ( m_OpenFlag )
        {
            // 内部で使用する nn::audio:::AudioRenderer 上の全チャンネルの負荷の総和を取得
            Channel* channel = m_pChannelList;
            while( channel != nullptr )
            {
                totalTick += channel->GetProcessTick(profile);
                channel = channel->GetNextTrackChannel();
            }
        }
        return totalTick;
    }

    // (Atktool 用)
    // トラックを強制的にミュートします。シーケンスコマンドによる設定より優先されます。
    // (初期化後、使用開始の前に一度だけ設定される想定です)
    void ForceMute() NN_NOEXCEPT;

protected:
    virtual ParseResult Parse( bool doNoteOn ) NN_NOEXCEPT = 0;

private:
    Channel* GetLastChannel() const NN_NOEXCEPT { return m_pChannelList; }
    void AddChannel( Channel* channel ) NN_NOEXCEPT;

    uint8_t m_PlayerTrackNo;
    bool m_OpenFlag;
    bool m_ForceMute;
    float m_ExtVolume;
    float m_ExtPitch;
    float m_PanRange;

    OutputParam m_TvParam;

    ParserTrackParam m_ParserTrackParam;
    volatile int16_t m_TrackVariable[ TrackVariableCount ];

    SequenceSoundPlayer* m_pSequenceSoundPlayer;
    Channel* m_pChannelList;
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

