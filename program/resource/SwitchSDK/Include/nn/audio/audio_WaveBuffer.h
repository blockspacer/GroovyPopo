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

#include <cstddef>
#include <cstdint>

namespace nn {
namespace audio {

/**
 * @brief       VoiceType に与える入力波形を扱うための構造体です。
 * @details
 * この構造体ではで再生する波形データを扱います。@n
 * VoiceType で再生させるデータを buffer および size に指定します。@n
 * マルチチャンネル形式のデータを扱う場合は、各サンプルフォーマット毎に規定されている形式に従い、チャンネル毎のサンプルデータを格納する必要があります。@n
 * PCM フォーマットの場合、チャンネル毎に 1 サンプルずつ並べるインターリーブ形式で格納してください。@n
 * それ以外のフォーマットは、現在のリリースではマルチチャンネル再生に非対応です。@n
 * startSampleOffset および endSampleOffset で、buffer に指定した波形データの再生開始位置と再生終了位置を指定することができます。単位は"サンプル"です。@n
 * startSampleOffset と endSampleOffset は (endSampleOffset - startSampleOffset) が再生したいサンプル数となるように指定してください。
 * 例えば 1000 サンプルの波形データを buffer に指定し、その波形全体を再生する場合は、
 * startSampleOffset を「0」に、endSampleOffsetを「1000」に、それぞれ指定してください。@n
 * loop を true に指定すると、 VoiceType は該当の WaveBuffer をループ再生します。@n
 * isEndOfStream を true に指定すると、 VoiceType の再生サンプル数、具体的には nn::audio::GetVoicePlayedSampleCount() で取得できる値が再生完了時に 0 にクリアされます。@n
 * pContext には、この波形データに応じて必要とされるパラメータなどの各種コンテキストデータを指定するのに用います。@n
 * 現在のリリースでは ADPCM 波形データのループ再生のために nn::audio::AdpcmContext を指定するのに利用します。@n
 * 指定が不要な波形データの場合には、 nullptr を指定してください。@n
 * @n
 * 以下の 2 つのメモリが指す領域は nn::audio::MemoryPoolType が保持する領域に含まれている必要があります。
 * - buffer に指定するバッファ
 * - pContext にデータを指定する場合はそのデータ
 *
 * 詳しくは nn::audio::MemoryPoolType および nn::audio::AcquireMemoryPool() の説明を参照してください。@n
 */
struct WaveBuffer
{
    const void* buffer;         //!< 再生する波形のデータを保持するバッファのアドレス
    size_t size;                //!< buffer に指定したバッファのサイズ [bytes]
    int32_t startSampleOffset;  //!< 再生開始位置オフセット [サンプル]
    int32_t endSampleOffset;    //!< 再生終了位置オフセット [サンプル]
    bool loop;                  //!< ループ再生のためのフラグ
    bool isEndOfStream;         //!< 再生完了後に VoiceType の再生サンプル数をクリアするためのフラグ

    const void* pContext;       //!< コンテキストデータ
    size_t contextSize;         //!< pContext に指定しているデータのサイズ
};

}  // namespace audio
}  // namespace nn
