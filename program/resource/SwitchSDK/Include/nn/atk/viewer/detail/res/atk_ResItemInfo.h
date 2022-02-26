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
#ifdef NN_ATK_CONFIG_ENABLE_DEV

#if defined(NN_BUILD_CONFIG_OS_WIN)
#pragma warning(push)
#pragma warning(disable:4200)
#endif

#include <climits>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_WaveSoundFileReader.h>
#include <nn/atk/fnd/basis/atkfnd_Memory.h>
#include <nn/atk/viewer/detail/res/atk_ResTypes.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

static const uint32_t UserParameterCount      = SoundArchive::UserParamIndexMax + 1;
static const uint32_t StreamTrackCount        = SoundArchive::StreamTrackCount;
static const uint32_t StreamTrackChannelCount = atk::WaveChannelMax;
static const uint32_t SeqBankCount            = SoundArchive::SequenceBankMax;
static const uint32_t FxSendCount             = AuxBus_Count;

//---------------------------------------------------------------------------
//! @internal
//! @brief  リソースデータの種別です。
//---------------------------------------------------------------------------
enum ResDataType
{
    ResDataType_Unknown = 0,

    ResDataType_StreamSound = 1,
    ResDataType_WaveSound,
    ResDataType_SequenceSound,
    ResDataType_Bank,
    RedDataType_SoundMin = ResDataType_StreamSound,
    RedDataType_SoundMax = ResDataType_Bank,

    ResDataType_StreamSoundFile = 100,
    ResDataType_WaveSoundFile,
    ResDataType_SequenceFile,
    ResDataType_BankFile,
    ResDataType_WaveArchiveFile,
    ResDataType_StreamPreftchSoundFile,
    RedDataType_FileMin = ResDataType_StreamSoundFile,
    RedDataType_FileMax = ResDataType_StreamPreftchSoundFile
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  サウンドの基本パラメータ情報です。
//---------------------------------------------------------------------------
struct SoundBasicParam
{
    atk::detail::fnd::BinU8  volume;                              //!< 音量
    atk::detail::fnd::BinU8  panMode;                             //!< パンモード
    atk::detail::fnd::BinU8  panCurve;                            //!< パンカーブ
    atk::detail::fnd::BinU8  playerPriority;                      //!< プレイヤー優先度
    Offset                     playerNameOffset;                    //!< プレイヤー名
    atk::detail::fnd::BinU8  actorPlayerId;                       //!< アクタープレイヤーID
    atk::detail::fnd::BinU8  remoteFilter;                        //!< リモコンフィルタ
    atk::detail::fnd::BinU8  flags;                               //!< フラグ
    atk::detail::fnd::BinU8  singlePlayType;                      //!< 単一再生の種類
    atk::detail::fnd::BinU16 singlePlayEffectiveDuration;         //!< 単一再生の効果時間
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t _padding[2];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
    atk::detail::fnd::BinU32 userParams[UserParameterCount];    //!< ユーザーデータ

    //! @brief  構造体の内容をすべて上書きします。
    //! @param source TBD
    void Set(const SoundBasicParam& source) NN_NOEXCEPT
    {
        volume = source.volume;
        panMode = source.panMode;
        panCurve = source.panCurve;
        playerPriority = source.playerPriority;
        playerNameOffset = source.playerNameOffset;
        actorPlayerId = source.actorPlayerId;
        remoteFilter = source.remoteFilter;
        flags = source.flags;
        singlePlayType = source.singlePlayType;
        singlePlayEffectiveDuration = source.singlePlayEffectiveDuration;

        for(uint32_t i = 0; i < UserParameterCount; ++i)
        {
            userParams[i] = source.userParams[i];
        }
    }

    //! @brief  パラメータを SoundInfo に適用します。
    //! @param target TBD
    //! @param soundArchive TBD
    void ApplyTo(SoundArchive::SoundInfo& target, const SoundArchive& soundArchive) const NN_NOEXCEPT
    {
        target.volume = volume;
        target.panMode = static_cast<PanMode>(panMode);
        target.panCurve = static_cast<PanCurve>(panCurve);
        target.playerPriority = playerPriority;

        const ResName* playerName = playerNameOffset.to_ptr<ResName>();

        if(playerName != NULL && playerName->GetLength() > 0)
        {
            SoundArchive::ItemId playerId = soundArchive.GetItemId(playerName->GetName());

            target.playerId = playerId;
            if(playerId == SoundArchive::InvalidId)
            {
                NN_ATK_WARNING("[sndedit] player '%s' not found.", playerName->GetName());
            }
        }

        target.actorPlayerId = actorPlayerId;
        target.remoteFilter = remoteFilter;
        target.isFrontBypass = ((flags << 0) & 1) == 1;

        target.singlePlayType = static_cast<SinglePlayType>( singlePlayType );
        target.singlePlayEffectiveDuration = singlePlayEffectiveDuration;
    }
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  3Dサウンドのパラメータ情報です。
//---------------------------------------------------------------------------
struct Sound3DParam
{
    atk::detail::fnd::BinU32 flags;           //!< [0]Vol [1]Priority [2]Pan [3]SPan [4]Filter
    atk::detail::fnd::BinF32 decayRatio;      //!< 減衰率
    atk::detail::fnd::BinU8  decayCurve;      //!< 減衰カーブタイプ (DecayCurve が入る)
    atk::detail::fnd::BinU8  dopplerFactor;   //!< ドップラーファクター
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t _padding[2];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif

    //! @brief  構造体の内容をすべて上書きします。
    //! @param source TBD
    void Set(const Sound3DParam& source) NN_NOEXCEPT
    {
        flags = source.flags;
        decayRatio = source.decayRatio;
        decayCurve = source.decayCurve;
        dopplerFactor = source.dopplerFactor;
    }

    //! @brief  パラメータを Sound3DInfo に適用します。
    //! @param target TBD
    void ApplyTo(SoundArchive::Sound3DInfo& target) const NN_NOEXCEPT
    {
        target.flags = flags;
        target.decayRatio = decayRatio;
        target.decayCurve = decayCurve;
        target.dopplerFactor = dopplerFactor;
    }
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  センドパラメータ情報です。
//---------------------------------------------------------------------------
struct SendsParam
{
    atk::detail::fnd::BinU8 mainSend;
    atk::detail::fnd::BinU8 fxSend[FxSendCount];
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  ストリームサウンドトラックのパラメータ情報です。
//---------------------------------------------------------------------------
struct StreamSoundTrackParam
{
    atk::detail::fnd::BinU8 volume;                                       //!< 音量
    atk::detail::fnd::BinU8 pan;                                          //!< パン
    atk::detail::fnd::BinU8 span;                                         //!< サラウンドパン
    atk::detail::fnd::BinU8 channelCount;                                 //!< チャンネル数
    atk::detail::fnd::BinU8 globalChannelIndex[StreamTrackChannelCount];  //!< グローバルチャンネルインデックス
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t _padding[2];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
    atk::detail::fnd::BinU8 lpfFreq;
    atk::detail::fnd::BinU8 biquadType;
    atk::detail::fnd::BinU8 biquadValue;
    atk::detail::fnd::BinU8 flags;
    SendsParam                sends;

    //! @brief  構造体の内容をすべて上書きします。
    //! @param source TBD
    void Set(const StreamSoundTrackParam& source) NN_NOEXCEPT
    {
        volume = source.volume;
        pan = source.pan;
        span = source.span;
        channelCount = source.channelCount;

        for(uint32_t i = 0; i < StreamTrackChannelCount; ++i)
        {
            globalChannelIndex[i] = source.globalChannelIndex[i];
        }

        lpfFreq = source.lpfFreq;
        biquadType = source.biquadType;
        biquadValue = source.biquadValue;
        flags = source.flags;

        sends = source.sends;
    }

    //! @brief  パラメータを SoundArchive::StreamTrackInfo に適用します。
    //! @param target TBD
    void ApplyTo(SoundArchive::StreamTrackInfo& target) const NN_NOEXCEPT
    {
        target.volume = volume;
        target.pan = pan;
        target.surroundPan = span;
        target.channelCount = channelCount;

        for(uint32_t i = 0; i < StreamTrackChannelCount; ++i)
        {
            target.globalChannelIndex[i] = globalChannelIndex[i];
        }

        target.lowPassFilterFrequency = lpfFreq;
        target.biquadType = biquadType;
        target.biquadValue = biquadValue;
        target.flags = flags;

        target.mainSend = sends.mainSend;
        for(uint32_t i = 0; i < FxSendCount; ++i)
        {
            target.fxSend[i] = sends.fxSend[i];
        }
    }
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  ストリームサウンドのパラメータ情報です。
//---------------------------------------------------------------------------
struct StreamSoundParam
{
    StreamSoundTrackParam trackParams[StreamTrackCount];    //!< トラックのパラメータ情報
    atk::detail::fnd::BinU16 trackAllocationFlags;        //!< トラックアロケーションフラグ
    atk::detail::fnd::BinU16 totalChannelCount;           //!< 総チャンネル数
    atk::detail::fnd::BinS32 fileSize;                    //!< bxstm ファイルサイズ
    Offset filePathOffset;                                  //!< bxstm ファイルパス
    atk::detail::fnd::BinF32 pitch;
    SendsParam                 sends;
    atk::detail::fnd::BinU8  streamFileType;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t _padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
    atk::detail::fnd::BinS32 streamPrefetchFileSize;       //!< bxstp ファイルサイズ
    Offset                     streamPrefetchFilePathOffset; //!< bxstp ファイルパス
    ResHash32                  streamPrefetchHashCode;       //!< bxstp ハッシュ値

    //! @brief  構造体の内容をすべて上書きします。
    //! @param source TBD
    void Set(const StreamSoundParam& source) NN_NOEXCEPT
    {
        for(uint32_t i = 0; i < StreamTrackCount; ++i)
        {
            trackParams[i].Set(source.trackParams[i]);
        }

        trackAllocationFlags = source.trackAllocationFlags;
        totalChannelCount = source.totalChannelCount;
        fileSize = source.fileSize;
        filePathOffset = source.filePathOffset;
        pitch = source.pitch;
        sends = source.sends;
        streamFileType = source.streamFileType;
        streamPrefetchFileSize = source.streamPrefetchFileSize;
        streamPrefetchFilePathOffset = source.streamPrefetchFilePathOffset;
    }

    //! @brief  パラメータを SoundArchive::StreamSoundInfo に適用します。
    //! @param target TBD
    void ApplyTo(SoundArchive::StreamSoundInfo& target) const NN_NOEXCEPT
    {
        for(uint32_t i = 0; i < StreamTrackCount; ++i)
        {
            trackParams[i].ApplyTo(target.trackInfo[i]);
        }

        target.allocateTrackFlags = trackAllocationFlags;
        target.allocateChannelCount = totalChannelCount;
        target.pitch = pitch;

        target.mainSend = sends.mainSend;
        for(uint32_t i = 0; i < FxSendCount; ++i)
        {
            target.fxSend[i] = sends.fxSend[i];
        }

        target.streamFileType = (SoundArchive::StreamFileType)streamFileType;

        // ファイル関連のパラメータは、ここでは利用しません。
        // SoundArchivePlayer のファイルアクセスをフックする際に利用します。
    }
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  ストリームサウンドのパラメータ情報２です。
//---------------------------------------------------------------------------
struct StreamSoundParam2
{
    atk::detail::fnd::BinBool isLoop;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t _padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
    atk::detail::fnd::BinU32  loopStartFrame;
    atk::detail::fnd::BinU32  loopEndFrame;

    //! @brief  構造体の内容をすべて上書きします。
    //! @param source TBD
    void Set(const StreamSoundParam2& source) NN_NOEXCEPT
    {
        isLoop = source.isLoop;
        loopStartFrame = source.loopStartFrame;
        loopEndFrame = source.loopEndFrame;
    }

    //! @brief  パラメータを SoundArchive::StreamSoundInfo2 に適用します。
    //! @param target TBD
    void ApplyTo(SoundArchive::StreamSoundInfo2& target) const NN_NOEXCEPT
    {
        target.isLoop = isLoop;
        target.loopStartFrame = loopStartFrame;
        target.loopEndFrame = loopEndFrame;
    }
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  ウェーブサウンドのパラメータ情報です。
//---------------------------------------------------------------------------
struct WaveSoundParam
{
    atk::detail::fnd::BinU8   channelPriority;            //!< チャンネル優先度
    atk::detail::fnd::BinBool isReleasePriorityFix;       //!< リリース時優先度固定
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t _padding[2];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
    atk::detail::fnd::BinU32  index;                      //!< 波形インデックス
    atk::detail::fnd::BinS32  fileSize;                   //!< bxwsd ファイルサイズ
    Offset                      filePathOffset;             //!< bxwsd ファイルパス
    ResHash32                   hashCode;                   //!< bxwsd ハッシュ値
    atk::detail::fnd::BinS32  waveArchiveFileSize;        //!< bxwar ファイルサイズ
    Offset                      waveArchiveFilePathOffset;  //!< bxwar ファイルパス
    ResHash32                   waveArchiveHashCode;        //!< bxwar ハッシュ値

    //! @brief  構造体の内容をすべて上書きします。
    //! @param source TBD
    void Set(const WaveSoundParam& source) NN_NOEXCEPT
    {
        channelPriority = source.channelPriority;
        isReleasePriorityFix = source.isReleasePriorityFix;
        index = source.index;
        fileSize = source.fileSize;
        filePathOffset = source.filePathOffset;
        waveArchiveFileSize = source.waveArchiveFileSize;
        waveArchiveFilePathOffset = source.waveArchiveFilePathOffset;
    }

    //! @brief  パラメータを SoundArchive::WaveSoundInfo に適用します。
    //! @param target TBD
    void ApplyTo(SoundArchive::WaveSoundInfo& target) const NN_NOEXCEPT
    {
        target.channelPriority = channelPriority;
        target.isReleasePriorityFix = isReleasePriorityFix;
        target.index = index;

        // ファイル関連のパラメータは、ここでは利用しません。
        // SoundArchivePlayer のファイルアクセスをフックする際に利用します。
    }
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  シーケンスサウンドのパラメータ情報です。
//---------------------------------------------------------------------------
struct SequenceSoundParam
{
    atk::detail::fnd::BinU32  startOffset;                        //!< 再生開始位置
    atk::detail::fnd::BinU8   channelPriority;                    //!< チャンネル優先度
    atk::detail::fnd::BinBool isReleasePriorityFix;               //!< リリース時優先度固定
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t _padding[2];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
    atk::detail::fnd::BinU32  trackAllocationFlags;               //!< トラックアロケーションフラグ
    atk::detail::fnd::BinS32  fileSize;                           //!< bxseq ファイルサイズ
    Offset                      filePathOffset;                     //!< bxseq ファイルパス
    ResHash32                   hashCode;                           //!< bxseq ハッシュ値
    Offset                      bankNameOffsets[SeqBankCount];    //!< バンク名テーブル

    //! @brief  構造体の内容をすべて上書きします。
    //! @param source TBD
    void Set(const SequenceSoundParam& source) NN_NOEXCEPT
    {
        startOffset = source.startOffset;
        channelPriority = source.channelPriority;
        isReleasePriorityFix = source.isReleasePriorityFix;
        trackAllocationFlags = source.trackAllocationFlags;
        fileSize = source.fileSize;
        filePathOffset = source.filePathOffset;

        for(uint32_t i = 0; i < SeqBankCount; ++i)
        {
            bankNameOffsets[i] = source.bankNameOffsets[i];
        }
    }

    //! @brief  パラメータを SequenceSoundInfo に適用します。
    //! @param target TBD
    //! @param soundArchive TBD
    void ApplyTo(SoundArchive::SequenceSoundInfo& target, const atk::detail::SoundArchiveManager& soundArchiveManager) const NN_NOEXCEPT
    {
        if(startOffset != UINT_MAX)
        {
            target.startOffset = startOffset;
        }

        target.channelPriority = channelPriority;
        target.isReleasePriorityFix = isReleasePriorityFix;
        target.allocateTrackFlags = trackAllocationFlags;

        for(uint32_t i = 0; i < SeqBankCount; ++i)
        {
            const ResName* name = bankNameOffsets[i].to_ptr<const ResName>();

            if(name == NULL || name->GetLength() == 0)
            {
                continue;
            }

            SoundArchive::ItemId itemId = soundArchiveManager.GetMainSoundArchive()->GetItemId(name->GetName());

            if(itemId == SoundArchive::InvalidId)
            {
                // 追加サウンドアーカイブに存在するシーケンスサウンドがインゲーム編集の対象となったとき、
                // バンクは追加サウンドアーカイブに存在する可能性があるため、追加サウンドアーカイブのアイテムも確認する。
                if (soundArchiveManager.GetCurrentSoundArchive() != nullptr)
                {
                    itemId = soundArchiveManager.GetCurrentSoundArchive()->GetItemId(name->GetName());
                }

                if(itemId == SoundArchive::InvalidId)
                {
                    continue;
                }
            }

            target.bankIds[i] = itemId;
        }

        // ファイル関連のパラメータは、ここでは利用しません。
        // SoundArchivePlayer のファイルアクセスをフックする際に利用します。
    }
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  バンクのパラメータ情報です。
//---------------------------------------------------------------------------
struct BankParam
{
    atk::detail::fnd::BinS32 fileSize;                //!< bfbnk ファイルサイズ
    Offset     filePathOffset;                          //!< bfbnk ファイルパス
    ResHash32  hashCode;                                //!< bfbnk ハッシュ値
    atk::detail::fnd::BinS32 waveArchiveFileSize;     //!< bfwar ファイルサイズ
    Offset     waveArchiveFilePathOffset;               //!< bfwar ファイルパス
    ResHash32  waveArchiveHashCode;                     //!< bfwar ハッシュ値

    //! @brief  構造体の内容をすべて上書きします。
    //! @param source TBD
    void Set(const BankParam& source) NN_NOEXCEPT
    {
        fileSize = source.fileSize;
        filePathOffset = source.filePathOffset;
        waveArchiveFileSize = source.waveArchiveFileSize;
        waveArchiveFilePathOffset = source.waveArchiveFilePathOffset;
    }

    //! @brief  パラメータを BankInfo に適用します。
    //! @param target TBD
    //! @param soundArchive SoundArchive TBD
    void ApplyTo(SoundArchive::BankInfo& target, const SoundArchive& soundArchive) const NN_NOEXCEPT
    {
        (void)target;
        (void)soundArchive;
        // ファイル関連のパラメータは、ここでは利用しません。
    }
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  サウンド情報です。
//---------------------------------------------------------------------------
struct ResSoundInfo
{
    SoundBasicParam soundBasicParam;
    Sound3DParam sound3DParam;

    //! @brief  構造体の内容をすべて上書きします。
    //! @param source TBD
    void Set(const ResSoundInfo& source) NN_NOEXCEPT
    {
        soundBasicParam.Set(source.soundBasicParam);
        sound3DParam.Set(source.sound3DParam);
    }
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  ストリームサウンド情報です。
//!         HACK : ※この構造体の直後に ResName の実体が続いていることが前提です。
//---------------------------------------------------------------------------
struct ResStreamSoundInfo : public ResSoundInfo
{
    StreamSoundParam streamSoundParam;
    StreamSoundParam2 streamSoundParam2;

    static size_t GetRequiredSize(int maxItemName, int maxPath) NN_NOEXCEPT
    {
        // 固定パケットサイズ + プレイヤー名の長さ + bxstm ファイルパスの長さ
        return sizeof(ResStreamSoundInfo) +
            ResName::GetRequiredSize(maxItemName) +
            ResName::GetRequiredSize(maxPath);
    }

    //! @brief  構造体の内容をすべて上書きします。
    //! @param source TBD
    void Set(const ResStreamSoundInfo& source) NN_NOEXCEPT
    {
        ResSoundInfo::Set(source);
        streamSoundParam.Set(source.streamSoundParam);
        streamSoundParam2.Set(source.streamSoundParam2);
    }
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  ウェーブサウンド情報です。
//!         HACK : ※この構造体の直後に ResName の実体が続いていることが前提です。
//---------------------------------------------------------------------------
struct ResWaveSoundInfo : public ResSoundInfo
{
    WaveSoundParam waveSoundParam;

    static size_t GetRequiredSize(int maxItemName, int maxPath) NN_NOEXCEPT
    {
        // 固定パケットサイズ +
        // プレイヤー名の長さ +
        // bxwsd, bxwar ファイルパスの長さ
        return sizeof(ResWaveSoundInfo) +
            ResName::GetRequiredSize(maxItemName) +
            ResName::GetRequiredSize(maxPath) * 2;
    }

    //! @brief  構造体の内容をすべて上書きします。
    //! @param source TBD
    void Set(const ResWaveSoundInfo& source) NN_NOEXCEPT
    {
        ResSoundInfo::Set(source);
        waveSoundParam.Set(source.waveSoundParam);
    }
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  シーケンスサウンド情報です。
//!         HACK : ※この構造体の直後に ResName の実体が続いていることが前提です。
//---------------------------------------------------------------------------
struct ResSequenceSoundInfo : public ResSoundInfo
{
    SequenceSoundParam sequenceSoundParam;

    static size_t GetRequiredSize(int maxItemName, int maxPath) NN_NOEXCEPT
    {
        // 固定パケットサイズ +
        // プレイヤー名、バンク名の長さ +
        // bxseq ファイルパスの長さ
        return sizeof(ResSequenceSoundInfo) +
            ResName::GetRequiredSize(maxItemName) * (1 + SeqBankCount) +
            ResName::GetRequiredSize(maxPath);
    }

    //! @brief  構造体の内容をすべて上書きします。
    //! @param source TBD
    void Set(const ResSequenceSoundInfo& source) NN_NOEXCEPT
    {
        ResSoundInfo::Set(source);
        sequenceSoundParam.Set(source.sequenceSoundParam);
    }
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  バンク情報です。
//---------------------------------------------------------------------------
struct ResBankInfo
{
    BankParam bankParam;

    static size_t GetRequiredSize(int maxPath) NN_NOEXCEPT
    {
        // 固定パケットサイズ +
        // bxbnk, bxwar ファイルパスの長さ
        return sizeof(ResBankInfo) +
            ResName::GetRequiredSize(maxPath) * 2;
    }

    //! @brief  構造体の内容をすべて上書きします。
    //! @param source TBD
    void Set(const ResBankInfo& source) NN_NOEXCEPT
    {
        bankParam.Set(source.bankParam);
    }
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  ResSoundInfo のユーティリティクラスです。
//---------------------------------------------------------------------------
class ResItemInfoUtility
{
public:
    //----------------------------------------------------------
    //! @brief  アイテム情報の上限値を取得します。
    //! @param maxItemName TBD
    //! @param maxPath TBD
    //! @return TBD
    static size_t GetMaxItemInfoSize(int maxItemName, int maxPath) NN_NOEXCEPT
    {
        size_t result = 0;

        // TODO : アイテム情報の種類が増えたら、ここに追加します。
        // TODO : アイテム情報のサイズが変わったら、ここを編集します。
        result = (std::max)(result, detail::ResStreamSoundInfo::GetRequiredSize(maxItemName, maxPath));
        result = (std::max)(result, detail::ResWaveSoundInfo::GetRequiredSize(maxItemName, maxPath));
        result = (std::max)(result, detail::ResSequenceSoundInfo::GetRequiredSize(maxItemName, maxPath));
        result = (std::max)(result, detail::ResBankInfo::GetRequiredSize(maxPath));

        return nn::util::align_up(result, atk::detail::fnd::MemoryTraits::DefaultAlignment);
    }

    //----------------------------------------------------------
    //! @brief  サウンド情報データタイプかどうかを調べます。
    //! @param dataType TBD
    //! @return TBD
    static bool IsSoundInfoDataType(uint32_t dataType) NN_NOEXCEPT
    {
        return
            RedDataType_SoundMin <= dataType &&
            dataType <= RedDataType_SoundMax &&
            dataType != ResDataType_Bank;
    }

    //----------------------------------------------------------
    //! @brief  ストリームサウンド情報データタイプかどうかを調べます。
    //! @param dataType TBD
    //! @return TBD
    static bool IsStreamSoundInfoDataType(uint32_t dataType) NN_NOEXCEPT
    {
        return dataType == ResDataType_StreamSound;
    }

    //----------------------------------------------------------
    //! @brief  ウェーブサウンド情報データタイプかどうかを調べます。
    //! @param dataType TBD
    //! @return TBD
    static bool IsWaveSoundInfoDataType(uint32_t dataType) NN_NOEXCEPT
    {
        return dataType == ResDataType_WaveSound;
    }

    //----------------------------------------------------------
    //! @brief  シーケンスサウンド情報データタイプかどうかを調べます。
    //! @param dataType TBD
    //! @return TBD
    static bool IsSequenceSoundInfoDataType(uint32_t dataType) NN_NOEXCEPT
    {
        return dataType == ResDataType_SequenceSound;
    }

    //----------------------------------------------------------
    //! @brief  バンク情報データタイプかどうかを調べます。
    //! @param dataType TBD
    //! @return TBD
    static bool IsBankInfoDataType(uint32_t dataType) NN_NOEXCEPT
    {
        return dataType == ResDataType_Bank;
    }

    //----------------------------------------------------------
    //! @brief  サウンド情報から指定したファイルパスを取得します。
    //! @param soundInfo TBD
    //! @param soundType TBD
    //! @param fileDataType TBD
    //! @return TBD
    static const ResName* GetFilePath(const ResSoundInfo& soundInfo, ResDataType soundType, ResDataType fileDataType) NN_NOEXCEPT
    {
        switch(soundType)
        {
        case ResDataType_StreamSound:
            {
                const ResStreamSoundInfo* streamSoundInfo =
                    reinterpret_cast<const ResStreamSoundInfo*>(&soundInfo);

                switch(fileDataType)
                {
                case ResDataType_StreamSoundFile:
                    return streamSoundInfo->streamSoundParam.filePathOffset.to_ptr<const ResName>();

                case ResDataType_StreamPreftchSoundFile:
                    return streamSoundInfo->streamSoundParam.streamPrefetchFilePathOffset.to_ptr<const ResName>();

                default:
                    break;
                }

                return NULL;
            }

        case ResDataType_WaveSound:
            {
                const ResWaveSoundInfo* waveSoundInfo =
                    reinterpret_cast<const ResWaveSoundInfo*>(&soundInfo);

                switch(fileDataType)
                {
                case ResDataType_WaveSoundFile:
                    return waveSoundInfo->waveSoundParam.filePathOffset.to_ptr<const ResName>();

                case ResDataType_WaveArchiveFile:
                    return waveSoundInfo->waveSoundParam.waveArchiveFilePathOffset.to_ptr<const ResName>();

                default:
                    break;
                }

                return NULL;
            }

        case ResDataType_SequenceSound:
            {
                const ResSequenceSoundInfo* sequenceSoundInfo =
                    reinterpret_cast<const ResSequenceSoundInfo*>(&soundInfo);

                switch(fileDataType)
                {
                case ResDataType_SequenceFile:
                    return sequenceSoundInfo->sequenceSoundParam.filePathOffset.to_ptr<const ResName>();

                default:
                    break;
                }

                return NULL;
            }

        default:
            break;
        }

        return NULL;
    }

    //----------------------------------------------------------
    //! @brief  バク情報から指定したファイルパスを取得します。
    //! @param bankInfo TBD
    //! @param fileDataType TBD
    //! @return TBD
    static const ResName* GetFilePath(const ResBankInfo& bankInfo, ResDataType fileDataType) NN_NOEXCEPT
    {
        switch(fileDataType)
        {
        case ResDataType_BankFile:
            return bankInfo.bankParam.filePathOffset.to_ptr<const ResName>();

        case ResDataType_WaveArchiveFile:
            return bankInfo.bankParam.waveArchiveFilePathOffset.to_ptr<const ResName>();

        default:
            break;
        }

        return NULL;
    }
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#if defined(NN_BUILD_CONFIG_OS_WIN)
#pragma warning(pop)
#endif

#endif // NN_ATK_CONFIG_ENABLE_DEV
