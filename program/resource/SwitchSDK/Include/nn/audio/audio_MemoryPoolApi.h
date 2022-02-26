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

#include <nn/nn_Macro.h>
#include <nn/audio/audio_AudioRendererTypes.h>
#include <nn/audio/audio_MemoryPoolTypes.h>

namespace nn { namespace audio {

/**
 * @name        オーディオレンダラ メモリプール 関連 API
 * @{
 */

/**
 * @brief           オーディオレンダラ用のメモリプールを取得します。
 * @param[out]      pOutConfig  AudioRendererConfig
 * @param[out]      pOutPool    メモリプール
 * @param[in]       address     メモリプールで管理するメモリへのアドレス
 * @param[in]       size        メモリプールで管理するメモリのサイズ
 * @return          取得に成功したら true を返します。
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pOutPool != nullptr
 * - address != nullptr
 * - address は MemoryPoolType::AddressAlignment にアラインされている
 * - size は MemoryPoolType::SizeGranularity にアラインされている
 * - address, size で渡されたメモリ領域は別のメモリプールで利用されている領域を含まない
 * @details
 * オーディオレンダラが直接参照するリソースデータは、メモリプールで指定したメモリに含まれている必要があります。@n
 * このメモリプールに含める必要のあるリソースデータの一覧は nn::audio::MemoryPoolType の説明を参照してください。@n
 * @n
 * これらのメモリをオーディオレンダラで利用する場合、本関数でメモリプールを取得し nn::audio::RequestAttachMemoryPool() でオーディオレンダラへアタッチしてください。@n
 * アタッチ状態中のメモリプールが管理するメモリへのアクセスは避けてください。@n
 * ただしその場合でも nn::audio::CopyMemoryPoolData() による操作手段を提供しています。詳しくは nn::audio::CopyMemoryPoolData() の説明を参照してください。@n
 * @n
 * メモリプールは address, size で指定されたメモリ領域内を書き換えることはありません。@n
 * @n
 * この関数の利用中 nn::audio は pOutConfig へのアクセスを一部ロックします。@n
 * 同一の pOutConfig を利用する以下の関数はブロックされる可能性がありますので注意してください。@n
 * - @ref nn::audio::AcquireMemoryPool "nn::audio::AcquireMemoryPool(AudioRendererConfig* pOutConfig, MemoryPoolType* pOutPool, void* address, size_t size)"
 * - @ref nn::audio::ReleaseMemoryPool "nn::audio::ReleaseMemoryPool(AudioRendererConfig* pOutConfig, MemoryPoolType* pPool)"
 * - @ref nn::audio::GetReleasedMemoryPoolCount "nn::audio::GetReleasedMemoryPoolCount(const AudioRendererConfig* pOutConfig)"
 *
 * この関数と RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。@n
 */
bool AcquireMemoryPool(AudioRendererConfig* pOutConfig, MemoryPoolType* pOutPool, void* address, size_t size) NN_NOEXCEPT;

/**
 * @brief           オーディオレンダラ用に登録されているメモリプールを除去します。
 * @param[out]      pOutConfig  AudioRendererConfig
 * @param[in]       pPool       メモリプール
 * @pre
 * - pOutConfig != nullptr
 * - pOutConfig は初期化されている
 * - pPool は初期化されている
 * - @ref nn::audio::IsMemoryPoolAttached "nn::audio::IsMemoryPoolAttached(pPool)" == false
 * - pPool はアタッチ状態/デタッチ状態への遷移中でない。
 * @details
 * pPool で指定したメモリプールがオーディオレンダラから取り除かれます。@n
 * 本関数を呼び出す前に nn::audio::IsMemoryPoolAttached() で削除可能かどうか、確認してください。@n
 * @n
 * この関数の利用中 nn::audio は pOutConfig へのアクセスを一部ロックします。@n
 * 同一の pOutConfig を利用する以下の関数はブロックされる可能性がありますので注意してください。@n
 * - @ref nn::audio::AcquireMemoryPool "nn::audio::AcquireMemoryPool(AudioRendererConfig* pOutConfig, MemoryPoolType* pOutPool, void* address, size_t size)"
 * - @ref nn::audio::ReleaseMemoryPool "nn::audio::ReleaseMemoryPool(AudioRendererConfig* pOutConfig, MemoryPoolType* pPool)"
 * - @ref nn::audio::GetReleasedMemoryPoolCount "nn::audio::GetReleasedMemoryPoolCount(const AudioRendererConfig* pOutConfig)"
 *
 * この関数と RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。@n
 */
void ReleaseMemoryPool(AudioRendererConfig* pOutConfig, MemoryPoolType* pPool) NN_NOEXCEPT;

/**
 * @brief           メモリプールをオーディオレンダラへアタッチします。
 * @param[in]       pPool       メモリプール
 * @return          成功したら true を返します。
 * @pre
 * - pPool != nullptr
 * - pPool は初期化されている
 * @details
 * pPool が保持しているメモリ全体をオーディオレンダラへアタッチする要求をします。@n
 *
 * この関数は pPool の状態に応じて以下の挙動をします。
 * - @ref nn::audio::GetMemoryPoolState "nn::audio::GetMemoryPoolState(pPool)" == MemoryPoolType::State_Detached の場合、
 *   この関数は pPool をアタッチ要求状態 (State_RequestAttach) に遷移させ true を返します。
 *   アタッチ要求状態になった pPool は次の nn::audio::RequestUpdateAudioRenderer() の呼び出し時にアタッチ状態 (State_Attached) に遷移します。
 * - @ref nn::audio::GetMemoryPoolState "nn::audio::GetMemoryPoolState(pPool)" == MemoryPoolType::State_RequestDetach の場合、
 *   この関数は pPool はアタッチ状態 (State_Attached) に遷移させ true を返します。
 *   この挙動は nn::audio::RequestDetachMemoryPool() によるデタッチ要求のキャンセルに相当します。
 * - @ref nn::audio::GetMemoryPoolState "nn::audio::GetMemoryPoolState(pPool)" が上記以外の状態にある場合、
 *   この関数は pPool に一切の処理を施さず false を返します。
 *
 * 各状態の詳しい説明は nn::audio::GetMemoryPoolState() の説明および @confluencelink{124010227, NintendoSDK ドキュメントのメモリプールの説明を参照して下さい。}
 *
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pPool への操作は行わないでください。@n
 * この関数と RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。@n
 */
bool RequestAttachMemoryPool(MemoryPoolType* pPool) NN_NOEXCEPT;

/**
 * @brief           メモリプールをオーディオレンダラからデタッチします。
 * @param[in]       pPool       メモリプール
 * @return          成功したら true を返します。
 * @pre
 * - pPool != nullptr
 * - pPool は初期化されている。
 * @details
 * pPool が保持しているメモリ全体をオーディオレンダラからデタッチする要求をします。@n
 *
 * この関数は pPool の状態に応じて以下の挙動をします。
 * - @ref nn::audio::GetMemoryPoolState "nn::audio::GetMemoryPoolState(pPool)" == MemoryPoolType::State_Attached の場合、
 *   この関数は pPool をデタッチ要求状態 (State_RequestDetach) に遷移させ true を返します。
 *   デタッチ要求状態になった pPool は次回以降の nn::audio::RequestUpdateAudioRenderer() の呼び出し時に、後述する条件を満たすとデタッチ状態 (State_Detached) に遷移します。
 * - @ref nn::audio::GetMemoryPoolState "nn::audio::GetMemoryPoolState(pPool)" == MemoryPoolType::State_RequestAttach の場合、
 *   この関数は pPool をデタッチ状態 (State_Detached) に遷移させ true を返します。
 *   この挙動は nn::audio::RequestAttachMemoryPool() によるアタッチ要求のキャンセルに相当します。
 * - @ref nn::audio::GetMemoryPoolState "nn::audio::GetMemoryPoolState(pPool)" が上記以外の状態にある場合、
 *   この関数は pPool に一切の処理を施さず false を返します。
 *
 * 各状態の詳しい説明は nn::audio::GetMemoryPoolState() の説明および @confluencelink{124010227, NintendoSDK ドキュメントのメモリプールの説明を参照して下さい。}
 *
 * デタッチ要求状態 (State_RequestDetach) のメモリプールは nn::audio::RequestUpdateAudioRenderer() 呼び出し時に、システムによってオーディオレンダラが直接参照するリソースデータのうち利用中のデータを含んでいないかどうかをチェックされます。
 * このチェックで利用中のデータが存在しない場合に限り、 メモリプールはデタッチ状態 (State_Detached) に遷移します。
 * ここで「オーディオレンダラが直接参照するリソースデータ」 に該当するデータは nn::audio::MemoryPoolType の説明を参照してください。
 * メモリプール内のリソースをオーディオレンダラが利用中の場合、デタッチは完了せず、メモリプールはデタッチ要求状態 (State_RequestDetach) を維持します。
 * 期待通りにデタッチが完了しない場合は nn::audio::MemoryPoolType の説明で「オーディオレンダラが直接参照するリソースデータ」として列挙されている各種データが pPool に含まれていない、およびそれをオーディオレンダラから利用中でないかを確認してください。
 *
 * デタッチが完了するまでの間も、デタッチ要求状態の設定は維持されます。
 * そのため nn::audio はこの関数の呼び出し後 nn::audio::RequestUpdateAudioRenderer() の呼び出しがある度に pPool がデタッチ状態へ遷移可能かどうかを判断し、可能となったタイミングで pPool をデタッチ状態へと遷移させます。
 *
 * デタッチ状態への遷移が完了したかどうかは、 nn::audio::IsMemoryPoolAttached() もしくは nn::audio::GetMemoryPoolState() で確認することができます。@n
 * @n
 * アタッチ状態であるメモリプールが保持するメモリは、 nn::fs のファイル読み込みや nn::audio::CopyMemoryPoolData() によって書き換えが可能です。
 * ただしオーディオレンダラがアクセスしている領域を同時に書きかえるとノイズや動作が不安定となる原因になります。
 * メモリプールが保持する領域へのアクセスをする場合は、事前に本関数でデタッチを行うか、オーディオレンダラが利用中の領域へのアクセスは避けるようにして下さい。
 * @n
 * アタッチ状態のメモリプールは、それが追加されているオーディオレンダラが nn::audio::CloseAudioRenderer() で破棄されると、
 * 自動的にデタッチに相当する処理をオーディオレンダラ内部で行います。@n
 * そのためオーディオレンダラを破棄するタイミングでは、明示的にデタッチ処理、および除去処理を行う必要はありません。
 * ただし MemoryPoolType の各インスタンスはオーディオレンダラが破棄されたタイミングでオーディオレンダラとの紐づけが切れますので、
 * 各ステートは状態を正しく反映しなくなりますのでご注意ください。
 * 具体的にはアタッチ状態でオーディオレンダラが破棄されると、
 * nn::audio::IsMemoryPoolAttached() は True を返す状態のまま、
 * また nn::audio::GetMemoryPoolState() は State_Attached を返す状態のままですが、
 * 内部的には MemoryPoolType はデタッチされた状態となり、 pPool で利用していた領域を安全に再利用することが可能です。
 *
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pPool への操作は行わないでください。@n
 * この関数と RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。@n
 */
bool RequestDetachMemoryPool(MemoryPoolType* pPool) NN_NOEXCEPT;

/**
 * @brief           メモリプールがアタッチ状態か取得します。
 * @param[in]       pPool       メモリプール
 * @return          アタッチ状態であれば true 、デタッチ状態であれば false を返します。
 * @pre
 * - pPool != nullptr
 * - pPool は初期化されている
 * @details
 * nn::audio::RequestAttachMemoryPool() によってアタッチが要求された pPool に対してこの関数を呼び出すと true を返します。
 * nn::audio::RequestUpdateAudioRenderer() によってアタッチ状態への遷移が完了する前に nn::audio::RequestAttachMemoryPool() の呼び出し完了時点で即座に true を返ようになることにご注意ください。@n
 * また nn::audio::RequestDetachMemoryPool() によってデタッチされ pPool で指定したメモリ領域を利用するすべての処理が停止状態になった時に false を返します。@n
 * nn::audio::ReleaseMemoryPool() を呼び出す前に本関数で削除可能かどうか確認をしてください。
 *
 * この関数に加え nn::audio::GetMemoryPoolState() でも同様に、メモリプールの状態を取得することができます。
 * この関数はメモリプールが State_RequestAttach / State_Attached / State_RequestDetach な状態にある場合に true を返します。
 * 各状態の説明は nn::audio::GetMemoryPoolState() および @confluencelink{124010227, NintendoSDK ドキュメントのメモリプールの説明を参照して下さい。}
 * @n
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pPool への操作は行わないでください。@n
 * この関数と RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。@n
 */
bool IsMemoryPoolAttached(const MemoryPoolType* pPool) NN_NOEXCEPT;

/**
 * @brief           メモリプールの状態を取得します。
 * @param[in]       pPool       メモリプール
 * @return          メモリプールの状態を表す列挙型を返します。
 * @pre
 * - pPool != nullptr
 * - pPool は初期化されている
 * @details
 * nn::audio::RequestAttachMemoryPool() / nn::audio::RequestDetachMemoryPool() によって設定されたメモリプールの状態を取得します。
 * 各状態の意味はそれぞれ以下の通りです。
 *
 * - State_RequestAttach は  nn::audio::RequestAttachMemoryPool() の呼び出しが完了し、アタッチ要求されている状態を表します。
 *   この状態にあるメモリプールは次の nn::audio::RequestUpdateAudioRenderer() 時に State_Attached に遷移します。
 * - State_Attached はアタッチが完了した状態です。
 * - State_RequestDetach は nn::audio::RequestDetachMemoryPool() を呼び出しが完了し、デタッチ要求されている状態を表します。
 *   この状態にあるメモリプールは次の nn::audio::RequestUpdateAudioRenderer() 呼び出し時に pPool をオーディオレンダラが利用中でなければ State_Detached に遷移します。
 * - State_Detached はデタッチが完了した状態です。 nn::audio::AcquireMemoryPool() で初期化した直後のメモリプールはこの状態です。
 *
 * 各状態の説明については @confluencelink{124010227, NintendoSDK ドキュメントのメモリプールの説明も併せてご確認ください。}
 *
 * @n
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pPool への操作は行わないでください。@n
 * この関数と RequestUpdateAudioRenderer() を同時に呼び出すことは禁止されています。@n
 */
MemoryPoolType::State GetMemoryPoolState(const MemoryPoolType* pPool) NN_NOEXCEPT;

/**
 * @brief           取得可能なメモリプールの数を取得します。
 * @param[in]       pConfig     AudioRendererConfig
 * @return          メモリプールの数
 * @pre
 * - pConfig != nullptr
 * - pConfig は初期化されている
 * @details
 * メモリプールの数は pConfig の初期化時に nn::audio::AudioRendererParameter によって決定されます。@n
 * 本 API が返す数のメモリプールを取得することができますが、パフォーマンスの観点ではメモリプールの数は少なく保つ程効率が高まります。@n
 * メモリプールの数が不足する場合は、リソースの配置を検討してください。
 * @n
 * この関数の利用中 nn::audio は pConfig へのアクセスを一部ロックします。@n
 * 同一の pConfig を利用する以下の関数はブロックされる可能性がありますので注意してください。@n
 * - @ref nn::audio::AcquireMemoryPool "nn::audio::AcquireMemoryPool(AudioRendererConfig* pConfig, MemoryPoolType* pOutPool, void* address, size_t size)"
 * - @ref nn::audio::ReleaseMemoryPool "nn::audio::ReleaseMemoryPool(AudioRendererConfig* pConfig, MemoryPoolType* pPool)"
 * - @ref nn::audio::GetReleasedMemoryPoolCount "nn::audio::GetReleasedMemoryPoolCount(const AudioRendererConfig* pConfig)"
 */
int GetReleasedMemoryPoolCount(const AudioRendererConfig* pConfig) NN_NOEXCEPT;

/**
 * @brief           メモリプールで管理しているメモリのアドレスを取得します。
 * @param[in]       pPool       メモリプール
 * @return          アドレスを返します。
 * @pre
 * - pPool != nullptr
 * - pPool は初期化されている
 * @details
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pPool への操作は行わないでください。@n
 */
void* GetMemoryPoolAddress(const MemoryPoolType* pPool) NN_NOEXCEPT;

/**
 * @brief           メモリプールで管理しているメモリのサイズを取得します。
 * @param[in]       pPool       メモリプール
 * @return          サイズを返します。
 * @pre
 * - pPool != nullptr
 * - pPool は初期化されている
 * @details
 * この関数はスレッドセーフではありません。@n
 * この関数の呼び出し中に pPool への操作は行わないでください。@n
 */
size_t GetMemoryPoolSize(const MemoryPoolType* pPool) NN_NOEXCEPT;

/**
 * @brief           メモリプールが管理するメモリへの読み書きを行います。
 * @param[in]       pPool     メモリプール
 * @param[out]      dst       コピー先のメモリ
 * @param[in]       src       コピー元のメモリ
 * @param[in]       size      コピーするサイズ [byte]
 * @return dst を返します。
 * @pre
 * - pPool != nullptr
 * - pPool は初期化されている
 * - dst および src のさす先のメモリのどちらか一方、もしくはその両方が pPool に含まれるメモリである
 * - dst != nullptr
 * - src != nullptr
 * - dst と src のメモリ間で領域に重なりが無い
 * @details
 * アタッチ状態のメモリプールに対して安全にアクセスするための機能を提供します。@n
 * 例えば nn::audio::GetReleasedWaveBuffer() で返された WaveBuffer に含まれる buffer が、アタッチ状態のメモリプールに含まれる場合でも、本 API を用いた書き換えは可能です。@n
 * ただしアタッチ状態のメモリプール内でも、オーディオレンダラで利用中のメモリに対しての操作は避けてください。@n
 */
void* CopyMemoryPoolData(const MemoryPoolType* pPool, void* dst, const void* src, size_t size) NN_NOEXCEPT;

/**
 * @}
 */

}}
