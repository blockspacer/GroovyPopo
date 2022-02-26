/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/** @file
    @brief      NFP(amiibo)タグへのアクセスに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>
#include <nn/nfp/nfp_Result.h>
#include <nn/nfp/nfp_Types.h>

namespace nn {
namespace nfp {

/**
 * @name    NFP タグアクセス
 * @{
 */

/**
 * @brief   NFC デバイスのハンドルのリストを取得します。
 *
 * @param[out] pOutBuffer  取得したハンドルのリストを格納するバッファへのポインタを指定します。
 * @param[out] pOutCount   実際に取得したハンドルの数を格納するバッファへのポインタを指定します。
 * @param[in]  bufferCount 取得するハンドルの最大数（バッファの要素数）を指定します。
 *
 * @retresult
 *     @handleresult{ResultSuccess,               取得に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     NFC デバイスが見つかりません。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、再試行する必要があります。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *  - @a pOutBuffer はアクセス可能なアドレスを指している必要があります。
 *  - @a pOutCount はアクセス可能なアドレスを指している必要があります。
 *  - 0 < @a bufferCount
 *
 * @details
 *  NFC デバイスのハンドルが必要な API を呼ぶには、事前にこの API を実行し、NFC デバイスのハンドルを取得しておく必要があります。取得したハンドルでその API の実行対象となる NFC デバイスを指定することができます。@n
 *  Switch Pro コントローラーを有線 USB 通信で使用している場合、そのコントローラーに搭載されている NFC デバイスは使用できないため、本 API でそのハンドルは取得されません。
 */
nn::Result ListDevices(DeviceHandle* pOutBuffer, int* pOutCount, int bufferCount) NN_NOEXCEPT;

/**
 * @brief   タグの検知を開始します。
 *
 * @param[in] deviceHandle NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               検知を開始しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、再試行する必要があります。}
 *     @handleresult{ResultMaxNfcDeviceActivated, NFC 機能を利用中の NFC デバイスが最大数に達しているため、指定された NFC デバイスで NFC 機能を利用できません。NFC 機能を利用中の NFC デバイスが減った後、再試行する必要があります。}
 *     @handleresult{ResultConflictFunction,      競合する他の機能が利用されているため、NFC 機能を利用できません。競合する他の機能の利用が終わった後、再試行する必要があります。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *  - GetDeviceState() == @ref DeviceState_Init || GetDeviceState() == @ref DeviceState_Deactive
 *
 * @post
 *  - GetDeviceState() == @ref DeviceState_Search
 *
 * @details
 * 指定した NFC デバイスにて、タグの検知を開始します。@n
 * ライブラリを初期化直後、あるいは検出したタグ喪失直後である必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。) @n
 * @ref AttachActivateEvent や @ref AttachDeactivateEvent で設定したイベントがシグナル状態の場合、この API によって必ず非シグナル状態にクリアされます。
 *
 */
nn::Result StartDetection(const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @brief   タグの検知を終了します。
 *
 * @param[in] deviceHandle NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               検知を終了しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。既に検知は終了しています。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。そのため、既に検知は終了しています。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *
 * @post
 *  - GetDeviceState() == @ref DeviceState_Init
 *
 * @details
 * 指定した NFC デバイスのタグの検知を終了します。@n
 * @ref StartDetection を呼び出した後である必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 * タグがマウントされている場合は、内部で自動的に @ref Unmount() が実行されます。
 */
nn::Result StopDetection(const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @brief   タグをマウントします。
 *
 * @param[in] deviceHandle NFC デバイスのハンドル
 * @param[in] modelType    マウントするタグの製品タイプを指定します。
 *
 * @retresult
 *     @handleresult{ResultSuccess,               マウントに成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRetry,             マウントに失敗しました。再試行によって成功する可能性があります。再試行にて 3 回失敗した場合は、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           マウントに失敗しました。マウント対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestore,           タグのデータが壊れています。@ref Restore() によるタグの復旧を行う必要があります。}
 *     @handleresult{ResultNeedFormat,            タグのデータが壊れていますが、バックアップデータが存在しません。タグのフォーマットを行う必要があります。}
 *     @handleresult{ResultNotSupported,          NFP タグではありません。使用しているタグを確認する必要があります。}
 *     @handleresult{ResultInvalidFormatVersion,  対応していないバージョンのタグです。使用しているタグを確認する必要があります。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *
 * @post
 *  - タグの ROM 領域および RAM 領域へアクセス可能な状態になります。
 *  - GetDeviceState() == @ref DeviceState_Mount
 *
 * @details
 *  タグが検知済みでマウントも喪失もされていない必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  マウントが必要な API を呼ぶには、事前にこの API を実行する必要があります。この API の実行すると、アクセス対象となるタグのデータを反映したバッファがライブラリ内部に生成されます。@n
 *  マウントするタグが初めて読み込むタグ、もしくは別の本体で書き換えられたタグの場合、バックアップデータの更新を行います。@n
 *  この API は @ref Mount(const DeviceHandle& deviceHandle, ModelType modelType, MountTarget mountTarget) にて、マウント先に @ref MountTarget_All を指定した場合と機能は同じです。
 */
nn::Result Mount(const DeviceHandle& deviceHandle, ModelType modelType) NN_NOEXCEPT;

/**
 * @brief   タグ内の指定した領域をマウントします。
 *
 * @param[in] deviceHandle NFC デバイスのハンドル
 * @param[in] modelType    マウントするタグの製品タイプを指定します。
 * @param[in] mountTarget  マウントするタグ内の領域を指定します。
 *
 * @retresult
 *     @handleresult{ResultSuccess,               マウントに成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRetry,             マウントに失敗しました。再試行によって成功する可能性があります。再試行にて 3 回失敗した場合は、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           マウントに失敗しました。マウント対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestore,           タグのデータが壊れています。@ref Restore() によるタグの復旧を行う必要があります。}
 *     @handleresult{ResultNeedFormat,            タグのデータが壊れていますが、バックアップデータが存在しません。タグのフォーマットを行う必要があります。}
 *     @handleresult{ResultNotSupported,          NFP タグではありません。使用しているタグを確認する必要があります。}
 *     @handleresult{ResultInvalidFormatVersion,  対応していないバージョンのタグです。使用しているタグを確認する必要があります。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *
 * @post
 *  - タグ内の指定した領域へアクセス可能な状態になります。
 *  - GetDeviceState() == @ref DeviceState_Mount
 *
 * @details
 *  タグが検知済みでマウントも喪失もされていない必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  マウントが必要な API を呼ぶには、事前にこの API を実行する必要があります。この API の実行すると、アクセス対象となるタグのデータを反映したバッファがライブラリ内部に生成されます。@n
 *  指定する @ref MountTarget はマウントが必要な各 API の説明に従ってください。
 *  マウントするタグが初めて読み込むタグ、もしくは別の本体で書き換えられたタグの場合、バックアップデータの更新を行います。@n
 *  ただし、@a mountTarget に @ref MountTarget_Rom だけを指定した場合、
 *  - タグのデータが壊れている ( RAM 領域をマウントした場合に @ref ResultNeedRestore/@ref ResultNeedFormat を返す) 状態でも実行できます。
 *  - バックアップデータの更新は行いません。
 */
nn::Result Mount(const DeviceHandle& deviceHandle, ModelType modelType, MountTarget mountTarget) NN_NOEXCEPT;

/**
 * @brief   タグをアンマウントします。
 *
 * @param[in] deviceHandle NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               アンマウントに成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。そのため、アンマウントする必要はありません。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。そのため、アンマウントする必要はありません。}
 *     @handleresult{ResultNeedRestart,           アンマウントに失敗しました。アンマウント対象のタグを喪失した可能性があります。そのため、アンマウントする必要はありません。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *
 * @post
 *  - GetDeviceState() == @ref DeviceState_Active
 *
 * @details
 *  タグをマウントしておく必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  アンマウントを実行すると、マウントが必要な API を使用できなくなります。
 *  マウントが必要な API を使用する場合は、再度マウントしてください。
 *
 *  アンマウントせずに @ref StopDetection() を呼び出した場合やタグが喪失した場合は、自動的にアンマウントが実行されます。
 */
nn::Result Unmount(const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @brief   マウント時に用意した内部バッファのアプリケーション専用領域へのアクセスを有効化します。
 *
 * @param[in]  deviceHandle       NFC デバイスのハンドル
 * @param[in]  accessId           アプリケーション専用領域へのアクセスに使用する ID です。
 *
 * @retresult
 *     @handleresult{ResultSuccess,               アプリケーション専用領域へのアクセスが可能です。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           有効化に失敗しました。有効化対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultAccessIdMisMatch,      アクセス ID が一致しないため、アプリケーション専用領域へのアクセスができません。}
 *     @handleresult{ResultNeedCreate,            アプリケーション専用領域が作成されていません。@ref CreateApplicationArea() によるアプリケーション専用領域の作成を行う必要があります。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *
 * @details
 *  タグをマウントしておく必要があります。その際、マウント対象に @ref MountTarget_Ram を含んでいる必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  アクセス ID には、アプリケーション毎に任天堂から発行する ID を指定します。
 */
nn::Result OpenApplicationArea(const DeviceHandle& deviceHandle, nn::Bit32 accessId) NN_NOEXCEPT;

/**
 * @brief   アプリケーション専用領域のデータを、マウント時に用意した内部バッファから取得します。
 *
 * @param[out] pOutBuffer         データの取得先バッファです。
 * @param[out] pOutSize           実際に取得できたデータのサイズです。
 * @param[in]  deviceHandle       NFC デバイスのハンドル
 * @param[in]  bufferSize         取得するデータのサイズです。
 *
 * @retresult
 *     @handleresult{ResultSuccess,               取得に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           取得に失敗しました。取得対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 * @endretresult
 *
 * @pre
 *  - @a pOutBuffer はアクセス可能なアドレスを指している必要があります。
 *  - @a pOutSize はアクセス可能なアドレスを指している必要があります。
 *  - 0 < @a bufferSize
 *
 * @details
 *  @ref OpenApplicationArea() を呼び出してしておく必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  取得できるデータの最大サイズは @ref CommonInfo::applicationAreaSize を参照してください。
 *  アプリケーション専用領域サイズ よりも小さい @a bufferSize を指定した場合、アプリケーション専用領域の先頭から一部だけが取得されます。
 */
nn::Result GetApplicationArea(void* pOutBuffer, size_t* pOutSize, const DeviceHandle& deviceHandle, size_t bufferSize) NN_NOEXCEPT;

/**
 * @brief   アプリケーション専用領域のデータを、マウント時に用意した内部バッファに設定します。
 *
 * @param[in]  deviceHandle       NFC デバイスのハンドル
 * @param[in]  pData              設定するデータです。
 * @param[in]  dataSize           設定するデータのサイズです。
 *
 * @retresult
 *     @handleresult{ResultSuccess,               設定に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           設定に失敗しました。設定対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 * @endretresult
 *
 * @pre
 *  - @a pData はアクセス可能なアドレスを指している必要があります。
 *  - 0 < @a dataSize && @a dataSize <= アプリケーション専用領域サイズ
 *
 * @details
 *  @ref OpenApplicationArea() を呼び出してしておく必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  設定するデータのサイズが @ref CommonInfo::applicationAreaSize より小さい場合、残りの領域は乱数で埋められます。
 *
 *  データの書き込みを確定するには、@ref Flush() を呼ぶ必要があります。
 *
 * @sa     Flush
 * @sa     Mount
 * @sa     OpenApplicationArea
 */
nn::Result SetApplicationArea(const DeviceHandle& deviceHandle, const void* pData, size_t dataSize) NN_NOEXCEPT;

/**
 * @brief   マウント時に用意した内部バッファ内のアプリケーション専用領域のデータとアクセス ID を作成情報に基づいて再作成します。
 *
 * @param[in]  deviceHandle            NFC デバイスのハンドル
 * @param[in]  createInfo              アプリケーション領域の作成情報。
 *
 * @retresult
 *     @handleresult{ResultSuccess,               作成に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           作成に失敗しました。作成対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 * @endretresult
 *
 * @pre
 *  - 0 < @a createInfo.initialDataSize && @a createInfo.initialDataSize <= アプリケーション専用領域サイズ
 *
 * @details
 *  この API によって、既に作成済みのアプリケーション専用領域のデータとアクセス ID を変更することができます。ただし、変更前のアクセス ID にて、@n
 *  @ref OpenApplicationArea() を呼び出してしておく必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  変更を確定するには、@ref Flush() を呼ぶ必要があります。@n
 *  なお、アクセス ID が変更されるため、変更後のアクセス ID を知らないアプリケーションからはそのタグのアプリケーション専用領域にアクセスできなくなります。@n
 *  ユーザーの意思に反してアクセスできなくなることがないように、ユーザの意思を確認する文言を表示したうえで、本 API を使用してください。
 */
nn::Result RecreateApplicationArea(const DeviceHandle& deviceHandle, const ApplicationAreaCreateInfo& createInfo) NN_NOEXCEPT;

/**
 * @brief   マウント時に用意した内部バッファの内容をタグに書き込みます。
 *
 * @param[in]  deviceHandle NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               書き込みに成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRetry,             書き込みに失敗しました。再試行によって成功する可能性があります。再試行にて 3 回失敗した場合は、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           書き込みに失敗しました。書き込み対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *
 * @details
 *  タグをマウントしておく必要があります。その際、マウント対象に @ref MountTarget_Ram を含んでいる必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  この API を実行するまでは、タグへの書き込み操作は反映されません。
 *  この API を実行すると、タグへの書き込みと同時にバックアップデータへの書き込みも行います。
 */
nn::Result Flush(const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @brief   データが破損したタグをフォーマットし、バックアップデータから復旧します。
 *
 * @param[in]  deviceHandle NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               復旧に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRetry,             復旧に失敗しました。再試行によって成功する可能性があります。再試行にて 3 回失敗した場合は、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           復旧に失敗しました。復旧対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNotBroken,             タグが壊れていないため、復旧は不要です。}
 *     @handleresult{ResultNotSupported,          NFP タグではありません。使用しているタグを確認する必要があります。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *
 * @details
 *  タグに書かれたデータが破損してしまった場合、データの復旧が必要になります。
 *  この API では、タグをフォーマットし、バックアップデータをタグに書き込みます。
 *  このとき、同時に @ref Flush() を行います。@n
 *  タグが検知済みでマウントも喪失もされていない必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)
 */
nn::Result Restore(const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @brief   タグのアプリケーション専用領域を作成します。
 *          ブランクタグを使用する場合、最初にアプリケーション専用領域を作成する必要があります。
 *
 * @param[in]  deviceHandle            NFC デバイスのハンドル
 * @param[in]  createInfo              アプリケーション領域の作成情報。
 *
 * @retresult
 *     @handleresult{ResultSuccess,               作成に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRetry,             作成に失敗しました。再試行によって成功する可能性があります。再試行にて 3 回失敗した場合は、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           作成に失敗しました。作成対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultAlreadyCreated,        既にアプリケーション専用領域が作成されています。}
 * @endretresult
 *
 * @pre
 *  - GetState() == @ref State_Init
 *  - 0 < @a createInfo.initialDataSize && @a createInfo.initialDataSize <= アプリケーション専用領域サイズ
 *
 * @details
 *  タグをマウントしておく必要があります。その際、マウント対象に @ref MountTarget_Ram を含んでいる必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  アプリケーション領域の有無は、@ref OpenApplicationArea() の戻り値で確認できます。
 *  アプリケーション領域の作成を行うと、同時に @ref Flush() も行います。
 */
nn::Result CreateApplicationArea(const DeviceHandle& deviceHandle, const ApplicationAreaCreateInfo& createInfo) NN_NOEXCEPT;

/**
 * @brief   タグの情報を取得します。
 *
 * @param [out] pOutTagInfo       @ref TagInfo の取得先へのポインタです。
 * @param[in]  deviceHandle       NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               取得に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           取得に失敗しました。取得対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 * @endretresult
 *
 * @pre
 *  - @a pOutTagInfo はアクセス可能なアドレスを指している必要があります。
 *  - GetState() == @ref State_Init
 *
 * @details
 *  タグが検出された状態、あるいはマウントされた状態で実行してください。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  この API は NFP 以外のタグに対しても実行できます。
 */
nn::Result GetTagInfo(TagInfo* pOutTagInfo, const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @brief   NFP タグのオーナー登録情報を、マウント時に用意した内部バッファから取得します。
 *
 * @param[out] pOutRegisterInfo        @ref RegisterInfo の取得先へのポインタ
 * @param[in]  deviceHandle            NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               取得に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           取得に失敗しました。取得対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRegister,          登録情報が設定されていません。amiibo 設定での設定が必要です。}
 * @endretresult
 *
 * @pre
 *  - @a pOutRegisterInfo はアクセス可能なアドレスを指している必要があります。
 *  - GetState() == @ref State_Init
 *
 * @details
 *  タグをマウントしておく必要があります。その際、マウント対象に @ref MountTarget_Ram を含んでいる必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  タグのニックネームには、アプリケーションが対応していない文字が含まれている場合があります。
 *  その場合でも、画面表示が崩れたり、アプリケーションの進行に支障が出ないように、対処してください。
 */
nn::Result GetRegisterInfo(RegisterInfo* pOutRegisterInfo, const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @brief   NFP タグの共用領域の情報を、マウント時に用意した内部バッファから取得します。
 *
 * @param[out] pOutCommonInfo          @ref CommonInfo の取得先へのポインタ
 * @param[in]  deviceHandle            NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               取得に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           取得に失敗しました。取得対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 * @endretresult
 *
 * @pre
 *  - @a pOutCommonInfo はアクセス可能なアドレスを指している必要があります。
 *  - GetState() == @ref State_Init
 *
 * @details
 *  タグをマウントしておく必要があります。その際、マウント対象に @ref MountTarget_Ram を含んでいる必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)@n
 *  @ref GetApplicationArea() や @ref SetApplicationArea() で使用するアプリケーション領域のサイズは、@ref CommonInfo::applicationAreaSize を参照してください。
 */
nn::Result GetCommonInfo(CommonInfo* pOutCommonInfo, const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @brief   タグの ROM 領域の情報を、マウント時に用意した内部バッファから取得します。
 *
 * @param[out] pOutModelInfo           ROM 情報の取得先です。
 * @param[in]  deviceHandle            NFC デバイスのハンドル
 *
 * @retresult
 *     @handleresult{ResultSuccess,               取得に成功しました。}
 *     @handleresult{ResultNfcDeviceNotFound,     正しいステートの NFC デバイスが見つかりません。@ref ListDevices() による NFC デバイスのハンドル取得からやり直す必要があります。}
 *     @handleresult{ResultNfcDisabled,           NFC 機能が無効になっています。NFC 機能が有効になった後、@ref StartDetection() によるタグの検知からやり直す必要があります。}
 *     @handleresult{ResultNeedRestart,           取得に失敗しました。取得対象のタグを喪失した可能性があります。@ref StartDetection() によるタグの検知からやり直す必要があります。}
 * @endretresult
 *
 * @pre
 *  - @a pOutModelInfo はアクセス可能なアドレスを指している必要があります。
 *  - GetState() == @ref State_Init
 *
 * @details
 *  タグをマウントしておく必要があります。その際、マウント対象に @ref MountTarget_Rom を含んでいる必要があります。(状態が正しくない場合、@ref ResultNfcDeviceNotFound が返ります。)
 */
nn::Result GetModelInfo(ModelInfo* pOutModelInfo, const DeviceHandle& deviceHandle) NN_NOEXCEPT;

/**
 * @}
 */

}  // nfp
}  // nn
