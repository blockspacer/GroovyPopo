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

#include <nn/nn_Common.h>
#include <nn/oe/oe_NotificationMessage.h>
#include <nn/oe/oe_Performance.h>

namespace nn { namespace oe {

/**
 * @brief   携帯／据置の動作モードに変更があったことを示す通知メッセージです。
 *
 * @details
 *  動作モードに変更があったことを示します。
 *  現在の動作モードは nn::oe::GetOperationMode() で取得できます。
 *
 */
const Message MessageOperationModeChanged = 30;


/**
 * @brief   性能モードに変更があったことを示す通知メッセージです。
 *
 * @details
 *  性能モードに変更があったことを示します。
 *  現在の性能モードは nn::oe::GetPerformanceMode() で取得できます。
 *
 */
const Message MessagePerformanceModeChanged = 31;

/**
 * @brief   動作モードを示す列挙型です。
 *
 */
enum OperationMode
{
    OperationMode_Handheld, //!< 携帯モードです。
    OperationMode_Console,  //!< 据置モードです。
};

//-----------------------------------------------------------------------------

//! @name   動作モードや性能モードの制御に関する API
//! @{

/**
 * @brief   現在の動作モードを取得します。
 *
 * @details
 *  現在の動作モードを取得します。
 *  動作モードによって、デフォルトの映像・音声の出力先が変化します。
 *
 *  OperationMode_Handheld の場合は、ユーザは本体 LCD および内蔵スピーカーを通じてゲームをプレイします。
 *  デフォルトの映像出力は本体 LCD になります。
 *  デフォルトの音声出力は本体内蔵スピーカーになります。本体にヘッドフォンが接続されている場合は、ヘッドフォンのみから出力されます。
 *
 *  OperationMode_Console の場合は、ユーザは本体が接続された TV を通じてゲームをプレイします。
 *  デフォルトの映像出力は本体が接続された TV になります。
 *  デフォルトの音声出力は本体が接続された TV になります。本体にヘッドフォンが接続されている場合は、TV およびヘッドフォンから出力されます。
 *
 *  vi ライブラリにおいて、デフォルトの映像出力を使用するには nn::vi::OpenDefaultDisplay() を使用してください。
 *  audio ライブラリにおいて、デフォルトの音声出力を使用するには nn::audio::OpenDefaultAudioOut() を使用してください。
 *  AudioRenderer において、デフォルトの音声出力を使用するには "MainAudioOut" という名前の nn::audio::DeviceSinkType を使用してください。
 *
 *  システムに与える負荷が大きいため、本 API の頻繁な呼び出しは推奨しません。
 *  現在の動作モードの変更を監視したい場合は nn::oe::PopNotificationMessage() 等で nn::oe::MessageOperationModeChanged の通知を受け取った際に本 API を呼び出すようにして下さい。
 *
 */
OperationMode GetOperationMode() NN_NOEXCEPT;


/**
 * @brief   現在の性能モードを取得します。
 *
 * @details
 *  現在の性能モードを取得します。
 *
 *  システムに与える負荷が大きいため、本 API の頻繁な呼び出しは推奨しません。
 *  現在の性能モードの変更を監視したい場合は nn::oe::PopNotificationMessage() 等で nn::oe::MessagePerformanceModeChanged の通知を受け取った際に本 API を呼び出すようにして下さい。
 *
 */
PerformanceMode GetPerformanceMode() NN_NOEXCEPT;


/**
 * @brief   動作モード変更メッセージを通知するか否かを指定します。
 *
 * @param[in]   enabled   動作モード変更通知の有無を bool で指定します。
 *
 * @details
 *  動作モードの変更があった場合に、プログラムに対して
 *  nn::oe::MessageOperationModeChanged を通知するか否かを指定します。
 *  デフォルトでは false となっており通知されません。
 *
 *  プログラムとして動作モードの変更を検知したい場合は、
 *  本 API で true を指定してください。
 *
 */
void SetOperationModeChangedNotificationEnabled(bool enabled) NN_NOEXCEPT;


/**
 * @brief   性能モード変更メッセージを通知するか否かを指定します。
 *
 * @param[in]   enabled   性能モード変更通知の有無を bool で指定します。
 *
 * @details
 *  性能モードの変更があった場合に、プログラムに対して
 *  nn::oe::MessagePerformanceModeChanged を通知するか否かを指定します。
 *  デフォルトでは false となっており通知されません。
 *
 *  プログラムとして性能モードの変更を検知したい場合は、
 *  本 API で true を指定してください。
 *
 */
void SetPerformanceModeChangedNotificationEnabled(bool enabled) NN_NOEXCEPT;


//! @}

}} // namespace nn::oe

