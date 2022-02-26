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

/**
 * @file
 * @brief       オーディオ出力に関する型や定数を定義します。
 */

#include <nn/audio/audio_Config.h>
#include <nn/os/os_MemoryHeapCommon.h>
#include <nn/os/os_Mutex.h>
#include <nn/audio/audio_Result.h>
#include <nn/audio/audio_SampleFormat.h>
#include <nn/audio/audio_AudioInOutCommonTypes.h>
#include <nn/os/os_SystemEvent.h>

namespace nn {
namespace audio {

/**
 * @brief       オーディオ出力の状態を表す列挙型です。
 */
enum AudioOutState
{
    AudioOutState_Started,        //!< 動作しています
    AudioOutState_Stopped,        //!< 停止しています
};

/**
 * @brief       オーディオ出力用のパラメータです。
 */
struct AudioOutParameter
{
    int32_t sampleRate;                 //!< サンプルレート（0 指定時はデフォルト値）
    uint16_t channelCount;              //!< チャンネル数
    uint16_t _reserved;
};

/**
 * @brief       オーディオ出力バッファを表す構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref SetAudioOutBufferInfo()
 * - @ref GetAudioOutBufferDataPointer()
 * - @ref GetAudioOutBufferDataSize()
 * - @ref AppendAudioOutBuffer()
 * - @ref GetReleasedAudioOutBuffer()
 * - @ref ContainsAudioOutBuffer()
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct AudioOutBuffer
{
    static const size_t AddressAlignment = nn::os::MemoryPageSize; //!< AudioOutBuffer に要求されるアライメント
    static const size_t SizeGranularity = nn::os::MemoryPageSize;  //!< AudioOutBuffer に要求されるサイズの粒度

    union
    {
        AudioOutBuffer* pNext;
        uint64_t _pNextU64;
    };
    union
    {
        void* buffer;
        uint64_t _bufferU64;
    };
    union
    {
        size_t bufferSize;
        uint64_t _bufferSizeU64;
    };
    union
    {
        size_t size;
        uint64_t _sizeU64;
    };
    union
    {
        size_t offset;
        uint64_t _offsetU64;
    };
};

/**
 * @brief   オーディオ出力バッファのアライメントを指定するマクロです。
 */
#define NN_AUDIO_ALIGNAS_AUDIO_OUT_BUFFER_ALIGN NN_ALIGNAS(4096)

/**
 * @brief       オーディオ出力を管理するための構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref OpenDefaultAudioOut(AudioOut* pOutAudioOut, const AudioOutParameter& parameter)
 * - @ref OpenDefaultAudioOut(AudioOut* pOutAudioOut, nn::os::SystemEvent* pOutSystemEvent, const AudioOutParameter& parameter)
 * - @ref OpenAudioOut(AudioOut* pOutAudioOut, const char* name, const AudioOutParameter& parameter)
 * - @ref OpenAudioOut(AudioOut* pOutAudioOut, nn::os::SystemEvent* pOutSystemEvent, const char* name, const AudioOutParameter& parameter)
 * - @ref CloseAudioOut()
 * - @ref StartAudioOut()
 * - @ref StopAudioOut()
 * - @ref GetAudioOutState()
 * - @ref GetAudioOutName()
 * - @ref GetAudioOutSampleRate()
 * - @ref GetAudioOutChannelCount()
 * - @ref GetAudioOutSampleFormat()
 * - @ref AppendAudioOutBuffer()
 * - @ref GetReleasedAudioOutBuffer()
 * - @ref ContainsAudioOutBuffer()
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct AudioOut
{
    int32_t sampleRate;
    int32_t channelCount;
    SampleFormat sampleFormat;
    AudioOutState state;

    static const int NameLength = 256;
    char name[NameLength];

    int32_t internalBufferSampleCount;

    bool isMutexInitialized;
    mutable os::MutexType mutex;
    AudioOutBufferList appendList;
    AudioOutBufferList releaseList;

    void* pDevice;
#if defined(NN_BUILD_CONFIG_OS_WIN)
    void* pAudioClient;
    void* pAudioRenderClient;
    void* hEvent;
#endif
    int8_t _reserved[8];


    /**
    * @brief       設定可能なボリュームの最大値を返します。
    */
    static inline float GetVolumeMax()
    {
        return 128.0f;
    }

    /**
    * @brief       設定可能なボリュームの最小値を返します。
    */
    static inline float GetVolumeMin()
    {
        return 0.0f;
    }
};

/**
 * @brief        サポートするオーディオ出力の最大同時接続数です。
 */
const int AudioOutCountMax = 2;

/**
 * @brief        オーディオ出力に追加できる AudioOutBuffer の上限数です。
 */
const int AudioOutBufferCountMax = 32;

/**
 * @brief       オーディオ出力情報を表す構造体です。
 *
 * @details
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref ListAudioOuts()
 *
 * 上記の関数を利用するには nn/audio.h をインクルードする必要があります。
 */
struct AudioOutInfo
{
    char name[AudioOut::NameLength];    //!< UTF-8 で表現された、オーディオ出力の名前です。
};


/**
 * @}
 */
}  // namespace audio
}  // namespace nn
