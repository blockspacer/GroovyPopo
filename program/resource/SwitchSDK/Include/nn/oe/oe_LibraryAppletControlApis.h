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
#include <nn/oe/oe_LibraryAppletControlTypes.h>

namespace nn { namespace oe {

//! @name   ライブラリアプレット連携用 API
//! @{


/**
 * @brief   SDK が表示する UI に期待するテーマカラーを設定します。
 *
 * @param[in] themeColorType SDK が表示する UI に期待するテーマカラー
 *
 * @details
 *  SDK が表示する UI に期待するテーマカラーを、プリセット値である ThemeColorType 型の列挙子から設定します。@n
 *  指定したプリセットによってどのような色合いになるかは、本体設定の「テーマ」から
 *  ThemeColorType 型の列挙子と同じ色を指定することで確認できます。@n
 *  本 API を呼んでも、すべての SDK が表示する UI が期待したテーマカラーを反映するわけではありません。@n
 *  例えば Mii 作成・編集アプレットではテーマカラーは反映されません。@n
 *  本 API を呼ばなかったときのデフォルトのプリセット値は ThemeColorType_User です。
 */
void SetExpectedThemeColor( ThemeColorType themeColorType ) NN_NOEXCEPT;


/**
 * @brief   SDK 機能を呼び出した際のボリュームバランスの期待値を設定します。
 *
 * @param[in] applicationVolume   自プログラムに対するボリューム期待値
 * @param[in] sdkVolume           SDK 機能に対するボリューム期待値
 *
 * @pre
 *  - nn::oe::VolumeValueMin <= applicationVolume  && applicationVolume <= nn::oe::VolumeValueMax
 *  - nn::oe::VolumeValueMin <= sdkVolume && sdkVolume <= nn::oe::VolumeValueMax
 *
 * @details
 *  SDK の API には UI の表示を伴うものがあり、
 *  API によっては UI 表示中にはアプリケーションが再生している音声と
 *  SDK の UI が再生している音声とが混ざって出力されることがあります。
 *  本 API では、このときの両者のボリュームを設定します。
 *
 *  なお、本 API での設定値が適用されるタイミングは UI を表示する API により異なり、
 *  API によっては本 API での設定を無視する場合もあります。
 *
 *  デフォルトではシステムで定義された値が設定されています。
 *
 */
void SetExpectedVolumeBalance(float applicationVolume, float sdkVolume) NN_NOEXCEPT;


/**
 * @brief   SDK 機能を呼び出した際のボリュームバランスの期待値を取得します。
 *
 * @param[out] pOutApplicationVolume   自プログラムに対するボリューム期待値の格納先
 * @param[out] pOutSdkVolume           SDK 機能に対するボリューム期待値の格納先
 *
 * @details
 *  自プログラムおよび SDK 機能に対する
 *  ボリュームバランス期待値をそれぞれ float 型で取得します。
 *
 *  ボリュームバランス期待値については、
 *  nn::oe::SetExpectedVolumeBalance() を参照して下さい。
 *
 */
void GetExpectedVolumeBalance(float* pOutApplicationVolume, float* pOutSdkVolume) NN_NOEXCEPT;


//! @}

}} // namespace nn::oe

