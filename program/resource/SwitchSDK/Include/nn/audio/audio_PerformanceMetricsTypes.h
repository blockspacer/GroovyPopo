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

#include <cstdint>
#include <nn/audio/audio_Common.h>

namespace nn {
namespace audio {

/**
 * @brief オーディオレンダラの１オーディオフレーム単位のパフォーマンス情報のヘッダです。
 */
struct PerformanceFrameHeader
{
    uint32_t frameMagic;               //!< PerformanceFrameHeader を識別する ID です。
    int32_t entryCount;                //!< PerformanceFrameHeader に後続する PerformanceEntry の個数
    int32_t detailCount;               //!< PerformanceEntry 群に後続する PerformanceDetail の個数
    int32_t offsetToNextHeader;        //!< 次の PerformanceFrameHeader までのオフセット [Byte]
    int32_t totalProcessingTime;       //!< この PerformanceFrame が対象とするオーディオフレームの処理にかかった時間 [MicroSeconds]
    int32_t voiceDropCount;            //!< ボイスドロップが発生したボイスの個数
    int64_t startTime;                 //!< オーディオフレームの処理開始時間 [チック数]
    uint32_t frameIndex;               //!< PerformanceFrame のインデックス
    bool isRenderingTimeLimitExceeded; //!< レンダリング時間の上限を超えたかどうか
    int8_t _padding[11];
};

/**
 * @brief パフォーマンス情報の種類を表す列挙型です。
 */
enum PerformanceEntryType
{
    PerformanceEntryType_Unknown = 0, //!< 不明なパフォーマンス情報
    PerformanceEntryType_Voice,       //!< VoiceType のパフォーマンス情報
    PerformanceEntryType_SubMix,      //!< SubMixType のパフォーマンス情報
    PerformanceEntryType_FinalMix,    //!< FinalMixType のパフォーマンス情報
    PerformanceEntryType_Sink,        //!< Sink のパフォーマンス情報
    PerformanceEntryType_Count,       //!< パフォーマンス情報の種類の個数
};

/**
 * @brief 詳細なパフォーマンス情報の種類を表す列挙型です。
 */
enum PerformanceDetailType
{
    PerformanceDetailType_Unknown = 0,        //!< 不明なパフォーマンス情報
    PerformanceDetailType_PcmInt16DataSource, //!< 16 bit 符号付き整数 PCM の読み込みについて情報
    PerformanceDetailType_AdpcmDataSource,    //!< ADPCM データの読み込みについての情報
    PerformanceDetailType_Volume,             //!< ボリューム処理についての情報
    PerformanceDetailType_BiquadFilter,       //!< VoiceType に施す Biquad フィルタについての情報
    PerformanceDetailType_Mix,                //!< VoiceType / SubMixType / FinalMixType 間のミックスおよび BufferMixer エフェクトによるミックスについての情報
    PerformanceDetailType_Delay,              //!< ディレイエフェクトについての情報
    PerformanceDetailType_Aux,                //!< ユーザー定義エフェクトについての情報
    PerformanceDetailType_Reverb,             //!< リバーブエフェクトについての情報
    PerformanceDetailType_I3dl2Reverb,        //!< I3dl2 リバーブエフェクトについての情報
    PerformanceDetailType_Count,              //!< 詳細なパフォーマンス情報の種類の個数
};

#pragma pack(1)
/**
 * @brief 各 VoiceType / SubMixType / FinalMixType のパフォーマンス情報です。
 * @details
 * id は nn::audio::GetVoiceNodeId() / nn::audio::GetSubMixNodeId() / nn::audio::GetFinalMixNodeId() / nn::audio::GetSinkNodeId() で取得される 各 VoiceType / SubMixType / FinalMixType の ID が格納されています。@n
 * startTime および processingTime は、それぞれオーディオレンダラ上で「処理を開始した時間」と「処理に掛かった時間」を保持しています。
 * ただし nn::audio::PerformanceEntry で取得される時間以外に、オーディオレンダラはシステムとして必要な処理を行う都合、
 * (nn::audio::PerformanceEntry.startTime + nn::audio::PerformanceEntry.processingTime) の時間が、
 * 必ずしも次の nn::audio::PerformanceEntry.startTime の時間に一致せず、時間的な隙間が空く可能性があります。
 *
 * 特に PC 環境上では、同一 PC 上の他のプロセスにより処理がブロックされ、
 * nn::audio::PerformanceEntry 間で時間的な隙間が空く場合がありますので、ご注意ください。
 *
 * また同じ理由で nn::audio::PerformanceFrameHeader.totalProcessingTime で取得される時間の合計が、
 * 全ての nn::audio::PerformanceEntry.processingTime の合計値と異なる場合があります。
 */
struct PerformanceEntry
{
    NodeId id;                   //!< type で示された VoiceType / SubMixType / FinalMixType / DeviceSinkType / CircularBufferSinkType の NodeId
    int32_t startTime;           //!< 各オーディオフレームの処理開始を 0 とした処理開始時間 [MicroSeconds]
    int32_t processingTime;      //!< 処理にかかった時間 [MicroSeconds]
    int8_t entryType;            //!< このエントリーが保持するパフォーマンス情報の対象の種類 (nn::audio::PerformanceEntryType を保持)
    int8_t _padding[11];
};

/**
 * @brief 各 VoiceType / SubMixType / FinalMixType に紐づく詳細なパフォーマンス情報です。
 * @details
 * PerformanceDetail では nn::audio::SetPerformanceDetailTarget() で指定した VoiceType / SubMixType / FinalMixType にまつわる情報が取得されます。
 * parentId には nn::audio::GetVoiceNodeId() / nn::audio::GetSubMixNodeId() / nn::audio::GetFinalMixNodeId() で取得される 各 VoiceType / SubMixType / FinalMixType の ID が格納されています。
 * ここで得られる NodeId は nn::audio::SetPerformanceDetailTarget() で指定した  VoiceType / SubMixType / FinalMixType の NodeId と一致します。
 * startTime および processingTime は、それぞれオーディオレンダラ上で「処理を開始した時間」と「処理に掛かった時間」を保持しています。
 * detailType には対象に指定した VoiceType / SubMixType / FinalMixType で実行される処理の種類が格納されます。
 * parentEntryType は nn::audio::SetPerformanceDetailTarget() で指定した  VoiceType / SubMixType / FinalMixType の種類が格納されます。
 * ユーザーは parentEntryType と parentId から、この PerformanceDetail がオーディオレンダラ上の紐づく VoiceType / SubMixType / FinalMixType を一意に特定することができます。
 */
struct PerformanceDetail
{
    NodeId parentId;             //!< 本処理が紐づく VoiceType / SubMixType / FinalMixType の ID
    int32_t startTime;           //!< 各オーディオフレームの処理開始を起点とした処理開始時間 [MicroSeconds]
    int32_t processingTime;      //!< 処理にかかった時間 [MicroSeconds]
    int8_t detailType;           //!< この詳細なパフォーマンス情報の対象の種類 (nn::audio::PerformanceDetailType を保持)
    int8_t parentEntryType;      //!< 本処理が紐づく VoiceType / SubMixType / FinalMixType の種類 (nn::audio::PerformanceEntryType を保持)
    int8_t _padding[10];
};

#pragma pack()

}  // namespace audio
}  // namespace nn
