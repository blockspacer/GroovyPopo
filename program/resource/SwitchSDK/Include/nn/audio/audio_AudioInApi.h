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
 * @brief       オーディオ入力に関する関数を定義します。
 */

#include <nn/os/os_SystemEvent.h>

#include <nn/audio/audio_Config.h>
#include <nn/audio/audio_Result.h>
#include <nn/audio/audio_SampleFormat.h>
#include <nn/audio/audio_AudioInTypes.h>

namespace nn {
namespace audio {

/**
 * @name オーディオ入力
 * @{
 */

/**
 * @brief       利用可能なオーディオ入力のリストを取得します。
 * @param[out]  outAudioIns             オーディオ入力情報を取得するバッファ
 * @param[in]   count                   outAudioIns に取得できるオーディオ入力情報の数
 * @return      outAudioIns に取得されたオーディオ入力情報の数を返します。
 * @pre
 *              - 0 <= count
 * @post
 *              - 戻り値 n に対し、outAudioIns[0] から outAudioIns[n-1] に情報が書き込まれている
 */
int ListAudioIns(AudioInInfo* outAudioIns, int count) NN_NOEXCEPT;

/**
 * @brief       オーディオ入力パラメータを初期化します。
 * @param[out]  pOutParameter           パラメータ
 * @pre
 * - pOutParameter != nullptr
 * @post
 * - pOutParameter->sampleRate == 0
 * @details
 * pOutParameter で指定されるオーディオ入力パラメータを初期化します。
 * 初期化後のオーディオ入力パラメータには、事後条件に規定された初期値が代入されます。
 * オーディオ入力パラメータを利用する前に、必ず、本 API を呼び出して初期化してください。
 *
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pOutParameter への操作は行わないでください。@n
 */
void InitializeAudioInParameter(AudioInParameter* pOutParameter) NN_NOEXCEPT;

/**
 * @brief       デフォルトのオーディオ入力を初期化し、アクセッサを取得します。
 * @param[out]  pOutAudioIn             AudioIn 構造体へのポインタ
 * @param[in]   parameter               パラメータ
 * @retresult
 *   @handleresult{nn::audio::ResultOperationFailed}
 *   @handleresult{nn::audio::ResultInvalidSampleRate}
 * @endretresult
 * @pre
 *              - pOutAudioIn != nullptr
 * @post
 *              - デフォルトのオーディオ入力が利用可能であれば、下記の通り:
 *                  - pOutAudioIn が初期化される
 *                  - @ref nn::audio::GetAudioInState "nn::audio::GetAudioInState(pOutAudioIn)" == nn::audio::AudioInState_Stopped
 *              - デフォルトのオーディオ入力が利用可能でなければ、なし
 * @overloadlist{nn_audio_OpenDefaultAudioIn}
 */
Result OpenDefaultAudioIn(AudioIn* pOutAudioIn, const AudioInParameter& parameter) NN_NOEXCEPT;

/**
 * @brief       name により指定したオーディオ入力を初期化し、アクセッサを取得します。
 * @param[out]  pOutAudioIn             AudioIn 構造体へのポインタ
 * @param[in]   name                    オーディオ入力の名前
 * @param[in]   parameter               パラメータ
 * @retresult
 *   @handleresult{nn::audio::ResultNotFound}
 *   @handleresult{nn::audio::ResultOperationFailed}
 *   @handleresult{nn::audio::ResultInvalidSampleRate}
 * @endretresult
 * @pre
 *              - pOutAudioIn != nullptr
 *              - name != nullptr
 * @post
 *              - 指定したオーディオ入力が利用可能であれば、下記の通り:
 *                  - pOutAudioIn が初期化される
 *                  - @ref nn::audio::GetAudioInState "nn::audio::GetAudioInState(pOutAudioIn)" == nn::audio::AudioInState_Stopped
 *              - 指定したオーディオ入力が利用可能でなければ、なし
 * @overloadlist{nn_audio_OpenAudioIn}
 */
Result OpenAudioIn(AudioIn* pOutAudioIn, const char* name, const AudioInParameter& parameter) NN_NOEXCEPT;

/**
 * @brief       オーディオ入力のアクセッサを破棄し、終了します。
 * @param[in]   pAudioIn                AudioIn 構造体へのポインタ
 * @pre
 *              - pAudioIn != nullptr
 *              - pAudioIn により管理されるオーディオ入力はオープンされている
 *              - @ref nn::audio::GetAudioInState "nn::audio::GetAudioInState(pAudioIn)" == nn::audio::AudioInState_Stopped
 * @post
 *              - pAudioIn はどのオーディオ入力も管理しなくなる
 */
void CloseAudioIn(AudioIn* pAudioIn) NN_NOEXCEPT;

/**
 * @brief       オーディオ入力からの波形録音を開始します。
 * @param[in]   pAudioIn                AudioIn 構造体へのポインタ
 * @retresult
 *   @handleresult{nn::audio::ResultOperationFailed}
 * @endretresult
 * @pre
 *              - pAudioIn != nullptr
 *              - pAudioIn により管理されるオーディオ入力はオープンされている
 *              - @ref nn::audio::GetAudioInState "nn::audio::GetAudioInState(pAudioIn)" == nn::audio::AudioInState_Stopped
 * @post
 *              - @ref nn::audio::GetAudioInState "nn::audio::GetAudioInState(pAudioIn)" == nn::audio::AudioInState_Started
 */
Result StartAudioIn(AudioIn* pAudioIn) NN_NOEXCEPT;

/**
 * @brief       オーディオ入力からの波形録音を停止します。
 * @param[in]   pAudioIn                AudioIn 構造体へのポインタ
 * @pre
 *              - pAudioIn != nullptr
 *              - pAudioIn により管理されるオーディオ入力はオープンされている
 *              - @ref nn::audio::GetAudioInState "nn::audio::GetAudioInState(pAudioIn)" == nn::audio::AudioInState_Started
 * @post
 *              - @ref nn::audio::GetAudioInState "nn::audio::GetAudioInState(pAudioIn)" == nn::audio::AudioInState_Stopped
 */
void StopAudioIn(AudioIn* pAudioIn) NN_NOEXCEPT;

/**
 * @brief       オーディオ入力の状態を取得します。
 * @param[in]   pAudioIn                AudioIn 構造体へのポインタ
 * @return      状態を返します。
 * @pre
 *              - pAudioIn != nullptr
 *              - pAudioIn により管理されるオーディオ入力はオープンされている
 */
AudioInState GetAudioInState(const AudioIn* pAudioIn) NN_NOEXCEPT;

/**
 * @brief       オーディオ入力の名前を取得します。
 * @param[in]   pAudioIn                AudioIn 構造体へのポインタ
 * @return      オーディオ入力の名前バッファへのポインタを返します。
 * @pre
 *              - pAudioIn != nullptr
 *              - pAudioIn により管理されるオーディオ入力はオープンされている
 */
const char* GetAudioInName(const AudioIn* pAudioIn) NN_NOEXCEPT;

/**
 * @brief       オーディオ入力のサンプルレートを取得します。
 * @param[in]   pAudioIn                AudioIn 構造体へのポインタ
 * @return      サンプルレートを Hz 単位で返します。
 * @pre
 *              - pAudioIn != nullptr
 *              - pAudioIn により管理されるオーディオ入力はオープンされている
 */
int GetAudioInSampleRate(const AudioIn* pAudioIn) NN_NOEXCEPT;

/**
 * @brief       オーディオ入力のチャンネル数を取得します。
 * @param[in]   pAudioIn                AudioIn 構造体へのポインタ
 * @return      チャンネル数を返します。
 * @pre
 *              - pAudioIn != nullptr
 *              - pAudioIn により管理されるオーディオ入力はオープンされている
 */
int GetAudioInChannelCount(const AudioIn* pAudioIn) NN_NOEXCEPT;

/**
 * @brief       オーディオ入力のサンプルフォーマットを取得します。
 * @param[in]   pAudioIn                AudioIn 構造体へのポインタ
 * @return      サンプルフォーマットを返します。
 * @pre
 *              - pAudioIn != nullptr
 *              - pAudioIn により管理されるオーディオ入力はオープンされている
 */
SampleFormat GetAudioInSampleFormat(const AudioIn* pAudioIn) NN_NOEXCEPT;

/**
 * @brief       この関数は非推奨です。オーディオ入力バッファに情報をセットします。
 * @deprecated  buffer へのアライメント要求が必要になりました。 @ref nn::audio::SetAudioInBufferInfo "nn::audio::SetAudioInBufferInfo(AudioInBuffer* pOutAudioInBuffer, void* buffer, size_t bufferSize, size_t dataSize)" の利用に切り替えてください。
 *
 * @param[out]  pOutAudioInBuffer       オーディオ入力バッファ
 * @param[in]   buffer                  データバッファ
 * @param[in]   size                    データバッファのサイズ
 * @pre
 *              - pAudioInBuffer != nullptr
 *              - buffer != nullptr
 * @post
 *              - pOutAudioInBuffer に buffer, size の情報が設定される
 */
NN_DEPRECATED void SetAudioInBufferInfo(AudioInBuffer* pOutAudioInBuffer, void* buffer, size_t size) NN_NOEXCEPT;

/**
 * @brief       オーディオ入力バッファに情報をセットします。
 * @param[out]  pOutAudioInBuffer       オーディオ入力バッファ
 * @param[in]   buffer                  データバッファ
 * @param[in]   bufferSize              データバッファのサイズ
 * @param[in]   dataSize                入力するデータサイズ [Byte]
 * @pre
 *              - pAudioInBuffer != nullptr
 *              - buffer != nullptr
 *              - buffer は AudioInBuffer::AddressAlignment にアラインされている
 *              - bufferSize > 0 かつ、bufferSize は AudioInBuffer::SizeGranularity にアラインされている
 *              - dataSize > 0 かつ、dataSize <= bufferSize
 * @post
 *              - pOutAudioInBuffer に buffer, bufferSize, dataSize の情報が設定される
 */
void SetAudioInBufferInfo(AudioInBuffer* pOutAudioInBuffer, void* buffer, size_t bufferSize, size_t dataSize) NN_NOEXCEPT;

/**
 * @brief       オーディオ入力バッファのデータバッファポインタを取得します。
 * @param[in]   pAudioInBuffer          オーディオ入力バッファ
 * @return      データバッファポインタを返します。
 * @pre
 *              - pAudioInBuffer != nullptr
 */
void* GetAudioInBufferDataPointer(const AudioInBuffer* pAudioInBuffer) NN_NOEXCEPT;

/**
 * @brief       オーディオ入力バッファのデータバッファサイズを取得します。
 * @param[in]   pAudioInBuffer          オーディオ入力バッファ
 * @return      データバッファサイズを返します。
 * @pre
 *              - pAudioInBuffer != nullptr
 */
size_t GetAudioInBufferBufferSize(const AudioInBuffer* pAudioInBuffer) NN_NOEXCEPT;

/**
 * @brief       オーディオ入力バッファの入力するデータサイズを取得します。
 * @param[in]   pAudioInBuffer          オーディオ入力バッファ
 * @return      入力するデータサイズを返します。
 * @pre
 *              - pAudioInBuffer != nullptr
 */
size_t GetAudioInBufferDataSize(const AudioInBuffer* pAudioInBuffer) NN_NOEXCEPT;

/**
 * @brief       オーディオ入力バッファを登録します。
 * @param[in]   pAudioIn                AudioIn 構造体へのポインタ
 * @param[in]   pAudioInBuffer          オーディオ入力バッファ
 * @return      登録に成功したら true を、そうでなければ false を返します。
 * @pre
 *              - pAudioIn != nullptr
 *              - pAudioInBuffer != nullptr
 *              - @ref nn::audio::GetAudioInBufferDataPointer "GetAudioInBufferDataPointer(pAudioInBuffer)" != nullptr
 *              - @ref nn::audio::GetAudioInBufferDataPointer "GetAudioInBufferDataPointer(pAudioInBuffer)" は nn::audio::AudioInBuffer::AddressAlignment にアラインしている
 *              - @ref nn::audio::ContainsAudioInBuffer "ContainsAudioInBuffer(pAudioInBuffer, pAudioIn)" == false
 *              - @ref nn::audio::GetAudioInBufferDataSize "GetAudioInBufferDataSize(pAudioInBuffer)" % (@ref nn::audio::GetAudioInChannelCount "nn::audio::GetAudioInChannelCount(pAudioIn)" * @ref nn::audio::GetSampleByteSize "nn::audio::GetSampleByteSize" (@ref nn::audio::GetAudioInSampleFormat "nn::audio::GetAudioInSampleFormat(pAudioIn)")) == 0
 * @post
 *              - pAudioInBuffer に録音されるまで、pAudioIn の管理下に置かれる
 */
bool AppendAudioInBuffer(AudioIn* pAudioIn, AudioInBuffer* pAudioInBuffer) NN_NOEXCEPT;

/**
 * @brief       録音済みのオーディオ入力バッファを取得します。
 * @param[in]   pAudioIn                AudioIn 構造体へのポインタ
 * @return      録音済みのオーディオ入力バッファを返します。
 * @pre
 *              - pAudioIn != nullptr
 */
AudioInBuffer* GetReleasedAudioInBuffer(AudioIn* pAudioIn) NN_NOEXCEPT;

/**
 * @brief       AudioInBuffer が AudioIn に Append されているかどうかを確認します。
 * @param[in]   pAudioIn                AudioIn 構造体へのポインタ
 * @param[in]   pAudioInBuffer          オーディオ入力バッファ
 * @return      AudioInBuffer が AudioIn に Append されていれば true を、そうでなければ false を返します。
 * @pre
 *              - pAudioIn != nullptr
 *              - pAudioInBuffer != nullptr
 */
bool ContainsAudioInBuffer(const AudioIn* pAudioIn, const AudioInBuffer* pAudioInBuffer) NN_NOEXCEPT;

/**
 * @brief       デフォルトのオーディオ入力を初期化し、アクセッサを取得し、通知用同期オブジェクトを初期化します。
 * @param[out]  pOutAudioIn             AudioIn 構造体へのポインタ
 * @param[out]  pOutSystemEvent         同期用 SystemEvent
 * @param[in]   parameter               パラメータ
 * @retresult
 *   @handleresult{nn::audio::ResultOperationFailed}
 *   @handleresult{nn::audio::ResultInvalidSampleRate}
 * @endretresult
 * @pre
 *              - pOutAudioIn != nullptr
 *              - pOutSystemEvent != nullptr
 *              - pOutSystemEvent は未初期化
 * @post
 *              - デフォルトのオーディオ入力が利用可能であれば、下記の通り:
 *                  - pOutAudioIn が初期化される
 *                  - @ref nn::audio::GetAudioInState "nn::audio::GetAudioInState(pOutAudioIn)" == nn::audio::AudioInState_Stopped
 *                  - *pOutSystemEvent が初期化される
 *              - デフォルトのオーディオ入力が利用可能でなければ、なし
 * @overloadlist{nn_audio_OpenDefaultAudioIn}
 */
Result OpenDefaultAudioIn(AudioIn* pOutAudioIn, nn::os::SystemEvent* pOutSystemEvent, const AudioInParameter& parameter) NN_NOEXCEPT;

/**
 * @brief       name により指定したオーディオ入力を初期化し、アクセッサを取得し、通知用同期オブジェクトを初期化します。
 * @param[out]  pOutAudioIn             AudioIn 構造体へのポインタ
 * @param[out]  pOutSystemEvent         同期用 SystemEvent
 * @param[in]   name                    オーディオ入力の名前
 * @param[in]   parameter               パラメータ
 * @retresult
 *   @handleresult{nn::audio::ResultNotFound}
 *   @handleresult{nn::audio::ResultOperationFailed}
 *   @handleresult{nn::audio::ResultInvalidSampleRate}
 * @endretresult
 * @pre
 *              - pOutAudioIn != nullptr
 *              - name != nullptr
 *              - pOutSystemEvent != nullptr
 *              - pOutSystemEvent は未初期化
 * @post
 *              - 指定したオーディオ入力が利用可能であれば、下記の通り:
 *                  - pOutAudioIn が初期化される
 *                  - @ref nn::audio::GetAudioInState "nn::audio::GetAudioInState(pOutAudioIn)" == nn::audio::AudioInState_Stopped
 *                  - *pOutSystemEvent が初期化される
 *              - 指定したオーディオ入力が利用可能でなければ、なし
 * @overloadlist{nn_audio_OpenAudioIn}
 */
Result OpenAudioIn(AudioIn* pOutAudioIn, nn::os::SystemEvent* pOutSystemEvent, const char* name, const AudioInParameter& parameter) NN_NOEXCEPT;

/**
 * @brief       AudioIn に登録されているオーディオ入力バッファの個数を取得します。
 * @param[in]   pAudioIn                  AudioIn 構造体へのポインタ
 * @return      登録されているオーディオ入力バッファの個数を返します。
 * @pre
 * - pAudioIn != nullptr
 * - pAudioIn は初期化されている
 */
int GetAudioInBufferCount(const AudioIn* pAudioIn) NN_NOEXCEPT;

/**
 * @brief       AudioIn に登録されている未録音のオーディオ出力バッファを全て録音済みにします。
 * @param[in]   pAudioIn              AudioIn 構造体へのポインタ
 * @return      登録されているバッファを全て録音済みに出来たかどうか
 * @pre
 * - pAudioIn != nullptr
 * - pAudioIn は初期化されている
 * - @ref nn::audio::GetAudioInState "GetAudioInState(pAudioIn)" == nn::audio::AudioInState_Stopped
 * @details
 * pAudioIn に登録されている未録音のオーディオ入力バッファを全て録音済みにします。@n
 * 本関数により録音済みにされたバッファには無音が記録されます。@n
 * pAudioIn が動作状態の時は、録音中のオーディオ入力バッファは録音済みにはなりません。@n
 * pAudioIn が停止状態の時は、録音中のオーディオ入力バッファを含むすべての登録されているバッファが録音済みになります。@n
 * @n
 * pAudioIn に登録されているオーディオ入力バッファを全て録音済みに出来た場合、関数は true を返します。@n
 * そうでない場合には false を返します。@n
 * @n
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pAudioIn への操作は行わないでください。
 */
bool FlushAudioInBuffers(AudioIn* pAudioIn) NN_NOEXCEPT;

/**
 * @}
 */

}  // namespace audio
}  // namespace nn
