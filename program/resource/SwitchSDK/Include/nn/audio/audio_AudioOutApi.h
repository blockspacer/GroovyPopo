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
 * @brief       オーディオ出力に関する API を定義します。
 */

#include <nn/audio/audio_Config.h>
#include <nn/os/os_SystemEvent.h>
#include <nn/audio/audio_Result.h>
#include <nn/audio/audio_SampleFormat.h>
#include <nn/audio/audio_AudioInOutCommonTypes.h>
#include <nn/audio/audio_AudioOutTypes.h>

namespace nn {
namespace audio {

/**
 * @name        オーディオ出力
 * @{
 */

/**
 * @brief       利用可能なオーディオ出力のリストを取得します。
 * @param[out]  outAudioOuts            オーディオ出力情報を取得するバッファ
 * @param[in]   count                   outAudioOuts に取得できるオーディオ出力情報の数
 * @return      outAudioOuts に取得されたオーディオ出力情報の数を返します。
 * @pre
 *              - 0 <= count
 * @post
 *              - 戻り値 n に対し、outAudioOuts[0] から outAudioOuts[n-1] に情報が書き込まれている
 */

int ListAudioOuts(AudioOutInfo* outAudioOuts, int count) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力パラメータを初期化します。
 * @param[out]  pOutParameter           パラメータ
 * @pre
 * - pOutParameter != nullptr
 * @post
 * - pOutParameter->sampleRate == 0
 * - pOutParameter->channelCount == 0
 * @details
 * pOutParameter で指定されるオーディオ出力パラメータを初期化します。
 * 初期化後のオーディオ出力パラメータには、事後条件に規定された初期値が代入されます。
 * オーディオ出力パラメータを利用する前に、必ず、本 API を呼び出して初期化してください。
 *
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pOutParameter への操作は行わないでください。@n
 */
void InitializeAudioOutParameter(AudioOutParameter* pOutParameter) NN_NOEXCEPT;

/**
 * @brief       デフォルトのオーディオ出力を初期化し、アクセッサを取得します。
 * @param[out]  pOutAudioOut            AudioOut 構造体へのポインタ
 * @param[in]   parameter               パラメータ
 * @retresult
 *   @handleresult{nn::audio::ResultOperationFailed}
 *   @handleresult{nn::audio::ResultInvalidSampleRate}
 *   @handleresult{nn::audio::ResultInvalidChannelCount}
 * @endretresult
 * @pre
 *              - pAudioOut != nullptr
 * @post
 *              - デフォルトのオーディオ出力が利用可能であれば、下記の通り:
 *                  - pAudioOut が初期化される
 *                  - @ref nn::audio::GetAudioOutState "GetAudioOutState(pAudioOut)" == nn::audio::AudioOutState_Stopped
 *              - デフォルトのオーディオ出力が利用可能でなければ、なし
 * @overloadlist{nn_audio_OpenDefaultAudioOut}
 */
Result OpenDefaultAudioOut(AudioOut* pOutAudioOut, const AudioOutParameter& parameter) NN_NOEXCEPT;

/**
 * @brief       デフォルトのオーディオ出力を初期化し、アクセッサを取得し、通知用同期オブジェクトを初期化します。
 * @param[out]  pOutAudioOut            AudioOut 構造体へのポインタ
 * @param[out]  pOutSystemEvent         同期用 SystemEvent
 * @param[in]   parameter               パラメータ
 * @retresult
 *   @handleresult{nn::audio::ResultOperationFailed}
 *   @handleresult{nn::audio::ResultInvalidSampleRate}
 *   @handleresult{nn::audio::ResultInvalidChannelCount}
 * @endretresult
 * @pre
 *              - pAudioOut != nullptr
 *              - pOutSystemEvent != nullptr
 *              - pOutSystemEvent は未初期化
 * @post
 *              - デフォルトのオーディオ出力が利用可能であれば、下記の通り:
 *                  - pAudioOut が初期化される
 *                  - @ref nn::audio::GetAudioOutState "GetAudioOutState(pAudioOut)" == nn::audio::AudioOutState_Stopped
 *                  - *pOutSystemEvent が初期化される
 *              - デフォルトのオーディオ出力が利用可能でなければ、なし
 * @overloadlist{nn_audio_OpenDefaultAudioOut}
 */
Result OpenDefaultAudioOut(AudioOut* pOutAudioOut, nn::os::SystemEvent* pOutSystemEvent, const AudioOutParameter& parameter) NN_NOEXCEPT;

/**
 * @brief       name により指定したオーディオ出力を初期化し、アクセッサを取得します。
 * @param[out]  pOutAudioOut            AudioOut 構造体へのポインタ
 * @param[in]   name                    オーディオ出力の名前
 * @param[in]   parameter               パラメータ
 * @retresult
 *   @handleresult{nn::audio::ResultNotFound}
 *   @handleresult{nn::audio::ResultOperationFailed}
 *   @handleresult{nn::audio::ResultInvalidSampleRate}
 *   @handleresult{nn::audio::ResultInvalidChannelCount}
 * @endretresult
 * @pre
 *              - pAudioOut != nullptr
 *              - name != nullptr
 * @post
 *              - 指定したオーディオ出力が利用可能であれば、下記の通り:
 *                  - pAudioOut が初期化される
 *                  - @ref nn::audio::GetAudioOutState "nn::audio::GetAudioOutState(pAudioOut)" == nn::audio::AudioOutState_Stopped
 *              - 指定したオーディオ出力が利用可能でなければ、なし
 * @overloadlist{nn_audio_OpenAudioOut}
 */
Result OpenAudioOut(AudioOut* pOutAudioOut, const char* name, const AudioOutParameter& parameter) NN_NOEXCEPT;

/**
 * @brief       name により指定したオーディオ出力を初期化し、アクセッサを取得し、通知用同期オブジェクトを初期化します。
 * @param[out]  pOutAudioOut            AudioOut 構造体へのポインタ
 * @param[out]  pOutSystemEvent         同期用 SystemEvent
 * @param[in]   name                    オーディオ出力の名前
 * @param[in]   parameter               パラメータ
 * @retresult
 *   @handleresult{nn::audio::ResultNotFound}
 *   @handleresult{nn::audio::ResultOperationFailed}
 *   @handleresult{nn::audio::ResultInvalidSampleRate}
 *   @handleresult{nn::audio::ResultInvalidChannelCount}
 * @endretresult
 * @pre
 *              - pAudioOut != nullptr
 *              - pOutSystemEvent != nullptr
 *              - pOutSystemEvent は未初期化
 *              - name != nullptr
 * @post
 *              - 指定したオーディオ出力が利用可能であれば、下記の通り:
 *                  - pAudioOut が初期化される
 *                  - @ref nn::audio::GetAudioOutState "GetAudioOutState(pAudioOut)" == nn::audio::AudioOutState_Stopped
 *                  - *pOutSystemEvent が初期化される
 *              - 指定したオーディオ出力が利用可能でなければ、なし
 * @overloadlist{nn_audio_OpenAudioOut}
 */
Result OpenAudioOut(AudioOut* pOutAudioOut, nn::os::SystemEvent* pOutSystemEvent, const char* name, const AudioOutParameter& parameter) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力のアクセッサを破棄し、終了します。
 * @param[in]   pAudioOut               AudioOut 構造体へのポインタ
 * @pre
 *              - pAudioOut != nullptr
 *              - pAudioOut により管理されるオーディオ出力はオープンされている
 *              - @ref nn::audio::GetAudioOutState "GetAudioOutState(pAudioOut)" == nn::audio::AudioOutState_Stopped
 * @post
 *              - pAudioOut はどのオーディオ出力も管理しなくなる
 */
void CloseAudioOut(AudioOut* pAudioOut) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力からの波形再生を開始します。
 * @param[in]   pAudioOut               AudioOut 構造体へのポインタ
 * @retresult
 *   @handleresult{nn::audio::ResultOperationFailed}
 * @endretresult
 * @pre
 *              - pAudioOut != nullptr
 *              - pAudioOut により管理されるオーディオ出力はオープンされている
 *              - @ref nn::audio::GetAudioOutState "GetAudioOutState(pAudioOut)" == nn::audio::AudioOutState_Stopped
 * @post
 *              - @ref nn::audio::GetAudioOutState "GetAudioOutState(pAudioOut)" == nn::audio::AudioOutState_Started
 */
Result StartAudioOut(AudioOut* pAudioOut) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力からの波形再生を停止します。
 * @param[in]   pAudioOut               AudioOut 構造体へのポインタ
 * @pre
 *              - pAudioOut != nullptr
 *              - pAudioOut により管理されるオーディオ出力はオープンされている
 *              - @ref nn::audio::GetAudioOutState "GetAudioOutState(pAudioOut)" == nn::audio::AudioOutState_Started
 * @post
 *              - @ref nn::audio::GetAudioOutState "GetAudioOutState(pAudioOut)" == nn::audio::AudioOutState_Stopped
 */
void StopAudioOut(AudioOut* pAudioOut) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力の再生状態を取得します。
 * @param[in]   pAudioOut               AudioOut 構造体へのポインタ
 * @return      再生状態を返します。
 * @pre
 *              - pAudioOut != nullptr
 *              - pAudioOut により管理されるオーディオ出力はオープンされている
 */
AudioOutState GetAudioOutState(const AudioOut* pAudioOut) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力の名前を取得します。
 * @param[in]   pAudioOut               AudioOut 構造体へのポインタ
 * @return      オーディオ出力の名前バッファへのポインタを返します。
 * @pre
 *              - pAudioOut != nullptr
 *              - pAudioOut により管理されるオーディオ出力はオープンされている
 */
const char* GetAudioOutName(const AudioOut* pAudioOut) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力のサンプルレートを取得します。
 * @param[in]   pAudioOut               AudioOut 構造体へのポインタ
 * @return      サンプルレートを Hz 単位で返します。
 * @pre
 *              - pAudioOut != nullptr
 *              - pAudioOut により管理されるオーディオ出力はオープンされている
 */
int GetAudioOutSampleRate(const AudioOut* pAudioOut) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力のチャンネル数を取得します。
 * @param[in]   pAudioOut               AudioOut 構造体へのポインタ
 * @return      チャンネル数を返します。
 * @pre
 *              - pAudioOut != nullptr
 *              - pAudioOut により管理されるオーディオ出力はオープンされている
 */
int GetAudioOutChannelCount(const AudioOut* pAudioOut) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力のサンプルフォーマットを取得します。
 * @param[in]   pAudioOut               AudioOut 構造体へのポインタ
 * @return      サンプルフォーマットを返します。
 * @pre
 *              - pAudioOut != nullptr
 *              - pAudioOut により管理されるオーディオ出力はオープンされている
 */
SampleFormat GetAudioOutSampleFormat(const AudioOut* pAudioOut) NN_NOEXCEPT;

/**
 * @brief       この関数は非推奨です。オーディオ出力バッファに情報をセットします。
 * @deprecated  buffer へのアライメント要求が必要になりました。 nn::audio::SetAudioOutBufferInfo(AudioOutBuffer* pOutAudioOutBuffer, void* buffer, size_t bufferSize, size_t dataSize) の利用に切り替えてください。
 *
 * @param[out]  pOutAudioOutBuffer      オーディオ出力バッファ
 * @param[in]   buffer                  データバッファ
 * @param[in]   size                    データバッファのサイズ
 * @pre
 *              - pAudioOutBuffer != nullptr
 *              - buffer != nullptr
 * @post
 *              - pOutAudioOutBuffer に buffer, size の情報が設定される
 */
NN_DEPRECATED void SetAudioOutBufferInfo(AudioOutBuffer* pOutAudioOutBuffer, void* buffer, size_t size) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力バッファに情報をセットします。
 * @param[out]  pOutAudioOutBuffer      オーディオ出力バッファ
 * @param[in]   buffer                  データバッファ
 * @param[in]   bufferSize              データバッファのサイズ
 * @param[in]   dataSize                再生するデータサイズ [Byte]
 * @pre
 *              - pAudioOutBuffer != nullptr
 *              - buffer != nullptr
 *              - buffer は AudioOutBuffer::AddressAlignment にアラインされている
 *              - bufferSize > 0 かつ、bufferSize は AudioOutBuffer::SizeGranularity にアラインされている
 *              - dataSize > 0 かつ、dataSize <= bufferSize
 * @post
 *              - pOutAudioOutBuffer に buffer, size, startOffset, endOffset の情報が設定される
 */
void SetAudioOutBufferInfo(AudioOutBuffer* pOutAudioOutBuffer, void* buffer, size_t bufferSize, size_t dataSize) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力バッファのデータバッファポインタを取得します。
 * @param[in]   pAudioOutBuffer         オーディオ出力バッファ
 * @return      データバッファポインタを返します。
 * @pre
 *              - pAudioOutBuffer != nullptr
 */
void* GetAudioOutBufferDataPointer(const AudioOutBuffer* pAudioOutBuffer) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力バッファのデータバッファサイズを取得します。
 * @param[in]   pAudioOutBuffer         オーディオ出力バッファ
 * @return      データバッファサイズを返します。
 * @pre
 *              - pAudioOutBuffer != nullptr
 */
size_t GetAudioOutBufferBufferSize(const AudioOutBuffer* pAudioOutBuffer) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力バッファの再生するデータサイズを取得します。
 * @param[in]   pAudioOutBuffer         オーディオ出力バッファ
 * @return      再生するデータサイズを返します。 [Byte]
 * @pre
 *              - pAudioOutBuffer != nullptr
 */
size_t GetAudioOutBufferDataSize(const AudioOutBuffer* pAudioOutBuffer) NN_NOEXCEPT;

/**
 * @brief       オーディオ出力バッファを登録します。
 * @param[in]   pAudioOut               AudioOut 構造体へのポインタ
 * @param[in]   pAudioOutBuffer         オーディオ出力バッファ
 * @return      登録に成功したら true を、そうでなければ false を返します。
 * @pre
 *              - pAudioOut != nullptr
 *              - pAudioOutBuffer != nullptr
 *              - @ref nn::audio::GetAudioOutBufferDataPointer "GetAudioOutBufferDataPointer(pAudioOutBuffer)" != nullptr
 *              - @ref nn::audio::GetAudioOutBufferDataPointer "GetAudioOutBufferDataPointer(pAudioOutBuffer)" は nn::audio::AudioOutBuffer::AddressAlignment にアラインしている
 *              - @ref nn::audio::ContainsAudioOutBuffer "ContainsAudioOutBuffer(pAudioOutBuffer, pAudioOut)" == false
 *              - @ref nn::audio::GetAudioOutBufferDataSize "GetAudioOutBufferDataSize(pAudioOutBuffer)" % (@ref nn::audio::GetAudioOutChannelCount "nn::audio::GetAudioOutChannelCount(pAudioOut)" * @ref nn::audio::GetSampleByteSize "nn::audio::GetSampleByteSize" (@ref nn::audio::GetAudioOutSampleFormat "nn::audio::GetAudioOutSampleFormat(pAudioOut)")) == 0
 * @post
 *              - pAudioOutBuffer の内容が再生されるまで、pAudioOut の管理下に置かれる
 */
bool AppendAudioOutBuffer(AudioOut* pAudioOut, AudioOutBuffer* pAudioOutBuffer) NN_NOEXCEPT;

/**
 * @brief       再生済みのオーディオ出力バッファを取得します。
 * @param[in]   pAudioOut               AudioOut 構造体へのポインタ
 * @return      再生済みのオーディオ出力バッファを返します。
 * @pre
 *              - pAudioOut != nullptr
 */
AudioOutBuffer* GetReleasedAudioOutBuffer(AudioOut* pAudioOut) NN_NOEXCEPT;

/**
 * @brief       AudioOutBuffer が AudioOut に Append されているかどうかを確認します。
 * @param[in]   pAudioOut               AudioOut 構造体へのポインタ
 * @param[in]   pAudioOutBuffer         オーディオ入力バッファ
 * @return      AudioOutBuffer が AudioOut に Append されていれば true を、そうでなければ false を返します。
 * @pre
 *              - pAudioOut != nullptr
 *              - pAudioOutBuffer != nullptr
 */
bool ContainsAudioOutBuffer(const AudioOut* pAudioOut, const AudioOutBuffer* pAudioOutBuffer) NN_NOEXCEPT;

/**
 * @brief       AudioOut に登録されているオーディオ出力バッファの個数を取得します。
 * @param[in]   pAudioOut                  AudioOut 構造体へのポインタ
 * @return      登録されているオーディオ出力バッファの個数を返します。
 * @pre
 * - pAudioOut != nullptr
 * - pAudioOut は初期化されている
 */
int GetAudioOutBufferCount(const AudioOut* pAudioOut) NN_NOEXCEPT;

/**
 * @brief       AudioOut により再生したデータのサンプル数を取得します。
 * @param[in]   pAudioOut                  AudioOut 構造体へのポインタ
 * @returns     StartAudioOut() を呼び出した以降に pAudioOut により再生したサンプル数を返します。
 * @pre
 * - pAudioOut != nullptr
 * - pAudioOut は初期化されている
 *
 * @details
 * サンプル数情報は一定の間隔でのみ更新されます。時間間隔はプラットフォームによって異なります。
 *
 * @platformbegin{Windows}
 * Windows 環境では、サンプル数情報の更新は 10ms ごとに行われます。
 * @platformend
 * @platformbegin{NX}
 * NX 実機環境では、サンプル数情報の更新は 5ms ごとに行われます。
 * @platformend
 */
int64_t GetAudioOutPlayedSampleCount(const nn::audio::AudioOut* pAudioOut) NN_NOEXCEPT;

/**
 * @brief       AudioOut に登録されている未再生のオーディオ出力バッファを全て再生済みにします。
 * @param[in]   pAudioOut              AudioOut 構造体へのポインタ
 * @return      登録されているバッファを全て再生済みに出来たかどうか
 * @pre
 * - pAudioOut != nullptr
 * - pAudioOut は初期化されている
 * - @ref nn::audio::GetAudioOutState "GetAudioOutState(pAudioOut)" == nn::audio::AudioOutState_Stopped
 * @details
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pAudioOut への操作は行わないでください。
 */
bool FlushAudioOutBuffers(AudioOut* pAudioOut) NN_NOEXCEPT;

/**
 * @brief           AudioOut のボリュームを設定します。
 * @param[in]       pAudioOut     AudioOut 構造体へのポインタ
 * @param[in]       volume        設定するボリューム値
 * @pre
 * - nn::audio::AudioOut::GetVolumeMin() <= volume <= nn::audio::AudioOut::GetVolumeMax()
 * - pAudioOut != nullptr
 * - pAudioOut は初期化されている
 * @post
 * - volume == nn::audio::GetAudioOutVolume()
 */
void SetAudioOutVolume(AudioOut* pAudioOut, float volume) NN_NOEXCEPT;

/**
 * @brief           AudioOut のボリュームを取得します。
 * @param[in]       pAudioOut     AudioOut 構造体へのポインタ
 * @pre
 * - pAudioOut != nullptr
 * - pAudioOut は初期化されている
 * @return          ボリューム値
 */
float GetAudioOutVolume(const AudioOut* pAudioOut) NN_NOEXCEPT;


/**
 * @}
 */
}  // namespace audio
}  // namespace nn
