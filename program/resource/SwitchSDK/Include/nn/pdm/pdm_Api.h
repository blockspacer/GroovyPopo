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

#include <nn/nn_ApplicationId.h>
#include <nn/account/account_Types.h>
#include <nn/pdm/pdm_Types.h>

namespace nn { namespace pdm {

/**
* @brief        指定したアプリケーションのプレイ情報を取得します。
*
* @param[out]   outValues       プレイ情報の格納先
* @param[in]    applicationIds  プレイ情報を取得するアプリケーションのアプリケーション ID
* @param[in]    count           outValues, applicationIds の要素数
*
* @return       outValues に格納されたプレイ情報の数
*
* @pre
*               - outValues != null
*               - applicationIds != null
*               - count > 0
*               - nmeta ファイルの PlayLogQueryCapability が正しく設定されている
*
* @post
*               - 戻り値 >= 0 && 戻り値 <= count
*               - count > 0 の場合
*                   - outValues[0] ~ outValues[戻り値 - 1] が有効な @ref ApplicationPlayStatistics を保持
*
* @details
*  この API が実行された本体上での指定したアプリケーションのプレイ情報を取得します。@n
*  プレイ情報を取得するには nmeta ファイルの Application/PlayLogQueryCapability を設定する必要があります。
*  nmeta ファイルの書き方については @confluencelink{166503043, 「nmeta ファイルの書き方 」}を参照してください。
*/
int QueryApplicationPlayStatistics(ApplicationPlayStatistics outValues[], const ApplicationId applicationIds[], int count) NN_NOEXCEPT;

/**
* @brief        指定したユーザーの指定したアプリケーションのプレイ情報を取得します。
*
* @param[out]   outValues       プレイ情報の格納先
* @param[in]    uid             プレイ情報を取得するユーザー
* @param[in]    applicationIds  プレイ情報を取得するアプリケーションのアプリケーション ID
* @param[in]    count           outValues, applicationIds の要素数
*
* @return       outValues に格納されたプレイ情報の数
*
* @pre
*               - outValues != null
*               - uid で指定したユーザーが本体上に存在する。
*               - applicationIds != null
*               - count > 0
*               - nmeta ファイルの PlayLogQueryCapability が正しく設定されている
*
* @post
*               - 戻り値 >= 0 && 戻り値 <= count
*               - count > 0 の場合
*                   - outValues[0] ~ outValues[戻り値 - 1] が有効な @ref ApplicationPlayStatistics を保持
*
* @details
*  この API が実行された本体上での指定したユーザーの指定したアプリケーションのプレイ情報を取得します。@n
*  プレイ情報を取得するには nmeta ファイルの Application/PlayLogQueryCapability を設定する必要があります。
*  nmeta ファイルの書き方については @confluencelink{166503043, 「nmeta ファイルの書き方 」}を参照してください。
*/
int QueryApplicationPlayStatistics(ApplicationPlayStatistics outValues[], const account::Uid& uid, const ApplicationId applicationIds[], int count) NN_NOEXCEPT;

}}
