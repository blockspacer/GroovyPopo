/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * @file
 * @brief       Npad に関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/os/os_SystemEventTypes.h>
#include <nn/hid/hid_NpadCommonTypes.h>

namespace nn { namespace hid {

//! @name Npad 共通の API
//! @{

/**
 * @brief       Npad を初期化します。
 *
 * @details     Npad のすべての API を呼び出すよりも前に、必ず本関数を呼び出してください
 *              Npad を利用するためには、本関数を呼び出したあとに SetSupportedNpadStyleSet() と SetSupportedNpadIdType() を呼ぶ必要があります。
 */
void InitializeNpad() NN_NOEXCEPT;

/**
 * @brief       Npad で有効にする操作スタイルを設定します。
 *
 * @details     Npad で有効にする操作スタイルを設定します。
 *              Npad の使用を開始する前に、アプリケーションで使用する操作スタイルを本関数で設定してください。
 *
 *              操作スタイルとは、コントローラーの機能・形状・持ち方などによって決められるコントローラーの操作方法を規定したものです。
 *              1つの操作スタイルは複数の種類のコントローラーをサポートすることが可能です。
 *              例えば、NpadStyleFullKey は、Switch Pro Controller をモデルとした操作スタイルです。
 *              アプリケーションは NpadStyleFullKey をサポートすることで、Switch Pro Controller と同等の機能やレイアウトを持ったコントローラーにまとめて対応することが可能です。
 *
 *              アプリケーションは複数の操作スタイルをサポートすることが可能です。
 *              アプリケーションは最低 1つ以上の操作スタイルを有効にする必要があります。
 *              現在アプリケーションがサポートしている操作スタイルは、GetSupportedNpadStyleSet() で取得します。
 *
 *              コントローラーが接続されていない状態では、有効な操作スタイルが存在しない状態となります。
 *              コントローラーが接続され Npad ID に対して割り当てられると、
 *              GetNpadStyleSet() を使用して、有効な Npad の操作スタイルを Npad ID 毎に取得することができます。
 *
 *              コントローラーが対応している操作スタイルがアプリケーションで有効になっていない場合、該当のコントローラーは使用できません。
 *              無線コントローラーの場合、対応する操作スタイルが無効な状態では無線接続ができなくなります。
 *
 *              有効にする操作スタイルを指定する引数 style について、style.Set() で全ビットフラグを有効にした状態のものを渡すことは禁止です。
 *              有効にするスタイルを必要なフラグを個別に有効にします。
 *
 * @param[in]   style                      有効にする Npad の操作スタイル
 *
 * @pre
 *              - InitializeNpad() の呼び出しが完了している
 *              - style.CountPopulation() > 0
 *              - style.IsAllOn() == false
 */
void SetSupportedNpadStyleSet(NpadStyleSet style) NN_NOEXCEPT;

/**
 * @brief       アプリケーションがサポートしている Npad の操作スタイルを取得します。
 *
 * @return      アプリケーションのサポートしている Npad 操作スタイルを取得します。
 *              SetSupportedNpadStyleSet() が呼び出されていないときは、返り値 r について r.IsAnyOff() == true となります。
 *
 * @pre
 *              - InitializeNpad() の呼び出しが完了している
 */
NpadStyleSet GetSupportedNpadStyleSet() NN_NOEXCEPT;

/**
 * @brief       アプリケーションでサポートする Npad ID を設定します。
 *
 * @details     アプリケーションでサポートする Npad ID を設定します。
 *              Npad の使用を開始する前に、アプリケーションで使用する Npad ID を本関数で設定してください。
 *
 *              アプリケーションは複数のコントローラーまたはユーザーからの入力を Npad ID 単位で取得します。
 *              本関数を使用して有効にする Npad ID を 1つ以上設定する必要があります。
 *              複数の Npad ID を指定する場合は、本関数で同時に指定してください。
 *              アプリケーション中に使用する Npad ID を更新することが可能です
 *
 *              本関数で指定されなかった Npad ID はすべて無効化されます。
 *              無線コントローラーの場合、Npad ID に空きがないときはコントローラーは本体に接続することができません。
 *              有効だった Npad が無効化された場合は、その Npad に対して接続されていたコントローラーは切断されます。
 *
 * @param[in]   pIds                    有効にする Npad ID のリスト
 * @param[in]   count                   有効にする Npad の数
 *
 * @pre
 *              - InitializeNpad() の呼び出しが完了している
 */
void SetSupportedNpadIdType(const NpadIdType* pIds, size_t count) NN_NOEXCEPT;

/**
 * @brief       Npad の操作スタイルに更新があった際に受け取る通知をバインドします。
 *
 * @details     Npad ID で指定した Npad に対して、操作スタイルの更新の通知をイベントオブジェクトにバインドします。
 *              操作スタイルは、コントローラーの接続や切断または本体機能を通じて変更されます。
 *              コントローラーの接続及び切断時には必ず操作スタイルの変更が起きるため、本関数で登録したイベントオブジェクトをコントローラーの接続/切断の通知を受けるために用いることができます。
 *              同時に複数のイベントオブジェクトに対してバインドすることはできません。
 *
 * @param[in]   id                      Npad ID
 * @param[in]   pEvent                  イベントオブジェクトを指すポインタ
 * @param[in]   clearMode               イベントオブジェクトのクリアモード
 *
 * @pre
 *              - InitializeNpad() の呼び出しが完了している
 *              - pEvent != nullptr
 *              - *pEvent は未初期化状態
 * @post
 *              - *pEvent は初期化状態
 *              - *pEvent はシグナル状態
 */
void BindNpadStyleSetUpdateEvent(const NpadIdType& id,
                     ::nn::os::SystemEventType* pEvent,
                     ::nn::os::EventClearMode clearMode) NN_NOEXCEPT;

/**
 * @brief       任意の Npad ID に対して現在有効になっている操作スタイルを取得します。
 *
 * @details     Npad では、各 Npad ID 毎に接続されたコントローラーの種類と SetSupportedNpadStyleSet() で指定した内容に応じて、有効な操作スタイルが決定されます。
 *              本関数を用いることで各 Npad ID 毎に現在有効になっている操作スタイルを取得することができます。
 *
 *              Npad ID に対してコントローラーが接続されていない場合は、返り値 r について r.IsAllOff() == true となります。
 *              r.IsAnyOn() == true の場合は何かしらのコントローラーが接続され操作スタイルが有効な状態です。
 *              返り値の有効なビットフラグの有無を確認することで、コントローラーの接続状態を確認することが可能です。
 *
 * @param[in]   id                      Npad ID
 *
 * @return      現在有効になっている Npad の操作スタイルです。
 *
 * @pre
 *              - InitializeNpad() の呼び出しが完了している
 *
 */
NpadStyleSet GetNpadStyleSet(const NpadIdType& id) NN_NOEXCEPT;

/**
 * @brief       Npad を切断します。
 *
 * @details     Npad ID で指定した Npad に割り当てられているコントローラーを切断します。
 *              本関数は、NpadId::No1 ～ NpadId::No8 までの Npad に対して有効です。
 *              他の Npad ID に対して本関数が呼ばれた場合は何も作用しません。
 *              また、もともと Npad に対してコントローラーが接続されていない場合も、何も作用しません。
 *
 *              コントローラーの切断が行われると、 GetNpadStyleSet() で取得される返り値 r について r.IsAllOff() == true となります。
 *
 * @platformbegin{NX}
 *              本関数は、アプリがインフォーカス状態 ( nn::oe::FocusState_InFocus ) にあるときのみ有効です。
 *              インフォーカス状態以外のときは要求は無視されます。
 * @platformend
 *
 * @param[in]   id                      Npad ID
 *
 * @pre
 *              - InitializeNpad() の呼び出しが完了している
 *
 */
void DisconnectNpad(const NpadIdType& id) NN_NOEXCEPT;

/**
 * @brief       Npad のプレイヤーランプの点灯パターンを取得します
 *
 * @details
 *              Npad ID で指定した Npad のプレイヤーランプの点灯パターンを取得します。
 *
 *              点灯パターンは Bit8 の 下位 4 bit を使用して表現されます。
 *              最下位 Bit が左端のプレイヤーランプの点灯状況を表します。
 *
 *              プレイヤーランプの取得できる Npad は NpadId::No1 ～ NpadId::No8 です。
 *              他の Npad に対して本関数が呼ばれた場合は、返り値として 0 が返されます。
 *              また、Npad に対してコントローラーが接続されていない場合も、0 が返されます。
 *
 * @param[in]   id                      Npad ID
 *
 * @pre
 *              - InitializeNpad() の呼び出しが完了している
 *
 */
Bit8 GetPlayerLedPattern(const NpadIdType& id) NN_NOEXCEPT;

//! @}

}} // namespace nn::hid
