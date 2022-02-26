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
#include <nn/nn_Macro.h>
#include <nn/os/os_MemoryHeapCommon.h>
#include <nn/audio/audio_AudioRendererTypes.h>

namespace nn { namespace audio {

class MemoryPoolInfo; // 前方宣言

/**
 * @brief       オーディオレンダラが利用するメモリ領域を表現するための構造体です。
 *
 * @details
 * メモリプールはオーディオレンダラが利用する、つまり参照・操作するメモリ領域を定義します。@n
 * アドレスとサイズを指定して初期化し、オーディオレンダラにアタッチすることでメモリプールはアタッチ状態となります。この状態のメモリ領域はオーディオレンダラからの利用が可能となります。@n
 * 逆にデタッチしデタッチ状態となったメモリ領域に対しては、オーディオレンダラはその領域を利用することはできません。@n
 * オーディオレンダラが利用するリソースデータ（下記）は、必ず何れかのメモリプールが保持する領域に保持されている必要があります。@n
 *
 * 指定する領域のアライメントとサイズは、実行環境に応じて制限されることがあるので、注意してください。
 *
 * ここでオーディオレンダラが直接参照するリソースデータには、以下のものが該当します。これらのデータすべてがメモリプールの保持する領域に含まれている必要があります。@n
 * - WaveBuffer.buffer および WaveBuffer.pContext に指定するバッファ
 * - nn::audio::AcquireVoiceSlot() で pParameter に渡すデータを格納するメモリ
 * - nn::audio::AddDelay() 、 nn::audio::AddReverb() 、 nn::audio::AddI3dl2Reverb() で buffer として指定するワークバッファ
 * - nn::audio::AddAux() で pSendBuffer および pReturnBuffer として指定するワークバッファ
 * - nn::audio::AddCircularBufferSink() で buffer に指定するバッファ
 *
 * メモリプールを部分的にアタッチ/デタッチすることはできません。
 * アタッチ/デタッチのコントロールが必要な粒度に応じて個別にメモリプールを作成してください。
 *
 * また NintendoSDK ドキュメントの @confluencelink{124010227, メモリプールの説明} も併せてご確認ください。
 *
 * メンバー変数を直接変更・参照することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * (初期化・終了）
 * - @ref AcquireMemoryPool()
 * - @ref ReleaseMemoryPool()
 *
 * (アタッチ/デタッチの設定）
 * - @ref RequestAttachMemoryPool()
 * - @ref RequestDetachMemoryPool()
 *
 * (メモリプールの状態/設定の取得）
 * - @ref IsMemoryPoolAttached()
 * - @ref GetMemoryPoolState()
 * - @ref GetMemoryPoolAddress()
 * - @ref GetMemoryPoolSize()
 */
struct MemoryPoolType
{
    /**
     * @brief       メモリプールの状態を表す列挙型です。
     */
    enum State
    {
        State_RequestAttach, //!< アタッチ要求状態
        State_Attached,      //!< アタッチ状態
        State_RequestDetach, //!< デタッチ要求状態
        State_Detached,      //!< デタッチ状態
    };

    static const size_t AddressAlignment = nn::os::MemoryPageSize; //!< MemoryPoolType 用のバッファに要求されるアライメント
    static const size_t SizeGranularity = nn::os::MemoryPageSize;  //!< MemoryPoolType 用のバッファに要求されるサイズの粒度

    MemoryPoolInfo* _pMemoryPoolInfo;
};

/**
 * @brief   オーディオ用メモリプールのアライメントを指定するマクロです。
 */
#define NN_AUDIO_ALIGNAS_MEMORY_POOL_ALIGN NN_ALIGNAS(4096)

}} // namespace nn::audio
