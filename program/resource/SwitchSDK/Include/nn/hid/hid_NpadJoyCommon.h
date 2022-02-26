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
#include <nn/nn_Result.h>
#include <nn/hid/hid_NpadCommonTypes.h>

namespace nn { namespace hid {

/**
 * @brief       Joy-Con 固有な Npad のデジタルボタン定義です。
 */
struct NpadJoyButton : public NpadButton
{
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<24> LeftSL;       //!< Npad の Joy-Con (L) SL ボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<25> LeftSR;       //!< Npad の Joy-Con (L) SL ボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<26> RightSL;      //!< Npad の Joy-Con (R) SR ボタン
    typedef ::nn::util::BitFlagSet<64, NpadButton>::Flag<27> RightSR;      //!< Npad の Joy-Con (R) SR ボタン
};

/**
 * @brief       Joy-Con 固有な Npad の状態を表す属性値です。
 */
struct NpadJoyAttribute : public NpadAttribute
{
    typedef ::nn::util::BitFlagSet<32, NpadAttribute>::Flag<2> IsLeftConnected;    //!< Joy-Con (L) の物理的な接続状態
    typedef ::nn::util::BitFlagSet<32, NpadAttribute>::Flag<3> IsLeftWired;        //!< Joy-Con (L) がケーブルで何か対応デバイスに接続されているかどうか
    typedef ::nn::util::BitFlagSet<32, NpadAttribute>::Flag<4> IsRightConnected;   //!< Joy-Con (R) の物理的な接続状態
    typedef ::nn::util::BitFlagSet<32, NpadAttribute>::Flag<5> IsRightWired;       //!< Joy-Con (R) がケーブルで何か対応デバイスに接続されているかどうか
};

/**
 * @brief       Npad に対する Joy-Con の割り当てモードの定義です。
 */
enum NpadJoyAssignmentMode
{
    NpadJoyAssignmentMode_Dual,     //!< 1つの Npad に対して Joy-Con が2本セットで割り当てられます。
    NpadJoyAssignmentMode_Single,   //!< 1つの Npad に対して Joy-Con が1本ずつ割り当てられます。
};

/**
 * @brief       Handheld 操作スタイルを有効化するモードの定義です。
 */
enum NpadHandheldActivationMode
{
    NpadHandheldActivationMode_Dual,     //!< Joy-Con が 2 本セットでジョイントされると Handheld 操作スタイルが有効化されます。
    NpadHandheldActivationMode_Single,   //!< Joy-Con 左右いずれか 1 本がジョイントされると Handheld 操作スタイルが有効化されます。
    NpadHandheldActivationMode_None,     //!< Joy-Con が 1 本もジョイントされていない場合でも Handheld 操作スタイルが有効化されます。
};

/**
 * @brief       Joy-Con の種類を表す定義です。
 */
enum NpadJoyDeviceType
{
    NpadJoyDeviceType_Left,        //!< Joy-Con (L) です。
    NpadJoyDeviceType_Right,       //!< Joy-Con (R) です
};

/**
 * @brief       Joy-Con の持ち方を示す定義です。 Joy-Con の割り当てモードが、NpadJoyAssignmentMode_Single の時に有効です。
 */
enum NpadJoyHoldType
{
    NpadJoyHoldType_Vertical,       //!< Joy-Con の縦持ちです
    NpadJoyHoldType_Horizontal,     //!< Joy-Con の横持ちです
};

/**
 * @brief       Joy-Con の通信モードの定義です。 <b>廃止予定です。</b>
 *
 * @details     通信周期や接続台数変更をテストするための暫定機能です。
 *              製品相当の機能実装が完了し次第、近いSDKで廃止予定です。
 */
enum NpadCommunicationMode
{
    NpadCommunicationMode_5ms,     //!< 5ms 周期の通信モードです。 Joy-Con 2台までの接続が可能です。
    NpadCommunicationMode_10ms,    //!< 10ms 周期の通信モードです。 Joy-Con 6台までの接続が可能です。
    NpadCommunicationMode_15ms,    //!< 15ms 周期の通信モードです。 Joy-Con 8台までの接続が可能です。
    NpadCommunicationMode_Default, //!< 15ms 周期のデフォルトの通信モードです。 Joy-Con 6台までの接続が可能です。モーションIRカメラ・NFC・ローカル通信の利用が可能です。
};

//! @name Npad の Joy-Con に関連する API
//! @{

/**
 * @brief       Npad に対する Joy-Con の割り当てモードを取得します。
 *
 * @details
 *              Npad には "2本セット割り当て" と "1本ずつ割り当て" の2つの割り当てモードがあります。
 *
 *              - 2本セット割り当て NpadJoyAssignmentMode_Dual
 *              Joy-Con が 1つの Npad に対して左右セットで割り当てられます。
 *              左右の Joy-Con は同じコントローラー番号でプレイヤーランプが点灯します。
 *              2本セット割り当ての時は、NpadStyleSet_JoyDual の操作スタイルが有効です。
 *              片方の Joy-Con だけが接続されている場合であっても、NpadStyleSet_JoyDual の入力として取得します。
 *
 *              - 1本ずつ割り当て NpadJoyAssignmentMode_Single
 *              1つの Npad に対して Joy-Con が Joy-Con (R) , Joy-Con (L) のいずれか 1つが割り当てられます。
 *              左右の Joy-Con に対して異なるコントローラー番号と異なるプレイヤーランプの点灯が行われます。
 *              1本ずつ割り当ての時は、NpadStyleSet_JoyRight もしくは NpadStyleSet_JoyLeft の操作スタイルが有効です。
 *              接続されている Joy-Con の種類に応じて有効な NpadStyleSet が変わります。
 *
 * @param[in]   id                     割り当てモードを取得する Npad ID
 *
 * @return      現在の割り当てモード
 *
 * @pre
 *              - 指定されたNpadIdType は NpadId::No* のいずれか
 *
 */
NpadJoyAssignmentMode GetNpadJoyAssignment(const NpadIdType& id) NN_NOEXCEPT;

/**
 * @brief       Npad の Joy-Con の割り当てモードを "1本ずつ割り当て" に変更します。
 *
 * @details
 *              Npad には "2本セット割り当て" と "1本ずつ割り当て" の2つの割り当てモードがあります。
 *
 *              本関数では、指定の Npad に対して接続された Joy-Con を1本ずつ割り当てます。
 *              Joy-Con (R) と Joy-Con (L) がそれぞれ異なる NpadIdType となり、
 *              左右の Joy-Con のプレイヤー番号も違うものとなります。
 *
 *              割り当てを変更する際は1つの NpadIdType を指定します。
 *              1つの NpadIdType に対してあらかじめ2本の Joy-Con が接続されている場合は、
 *              Joy-Con (R) が Npad から切断され、 Joy-Con がまったく接続されていない Npad に対して再接続されます
 *
 *              もともと Joy-Con が1本しか接続されていない場合は、そのまま "1本ずつ割り当て" のモードに変更されます。
 *
 *              Npad への割り当てモードは、本体機能によって外部で変更される可能性があります。
 *              現在の割り当てモードを取得するためには、GetNpadAssignmentMode() を使用してください
 *
 *              本関数は、アプリがインフォーカス状態 ( nn::oe::FocusState_InFocus ) にあるときのみ有効です。
 *              インフォーカス状態以外のときは要求は無視されます。
 *
 * @param[in]   id                     割り当てを変更する Npad ID
 *
 * @pre
 *              - 指定されたNpadIdType は NpadId::No* のいずれか
 *
 */
void SetNpadJoyAssignmentModeSingle(const NpadIdType& id) NN_NOEXCEPT;

/**
 * @brief       Npad の Joy-Con の割り当てモードを "1本ずつ割り当て" に変更します。
 *
 * @details
 *              Npad には "2本セット割り当て" と "1本ずつ割り当て" の2つの割り当てモードがあります。
 *
 *              本関数では、指定の Npad に対して接続された Joy-Con を1本ずつ割り当てます。
 *              Joy-Con (R) と Joy-Con (L) がそれぞれ異なる Npad ID となり、
 *              左右の Joy-Con のプレイヤー番号も違うものとなります。
 *
 *              割り当てを変更する際は1つの Npad ID を指定します。
 *              本関数では Npad ID に対してあらかじめ2本の Joy-Con が接続されている場合に、
 *              どちらの Joy-Con を残すかを、NpadJoyDeviceType で指定することができます。
 *              指定されなかったもう一方の Joy-Con は Npad から切断され、
 *              Joy-Con がまったく接続されていない Npad に対して再接続されます
 *
 *              もともと Joy-Con が1本しか接続されていない場合は、type の値は無視され
 *              そのまま "1本ずつ割り当て" のモードに変更されます。
 *
 *              Npad への割り当てモードは、本体機能によって外部で変更される可能性があります。
 *              現在の割り当てモードを取得するためには、GetNpadAssignmentMode() を使用してください
 *
 *              本関数は、アプリがインフォーカス状態 ( nn::oe::FocusState_InFocus ) にあるときのみ有効です。
 *              インフォーカス状態以外のときは要求は無視されます。
 *
 * @param[in]   id                     割り当てを変更する Npad ID
 * @param[in]   type                   2本接続されている場合に、 Joy-Con (R) / Joy-Con (L) どちらを残すかを指定する
 *
 * @pre
 *              - 指定されたNpadIdType は NpadId::No* のいずれか
 *
 */
void SetNpadJoyAssignmentModeSingle(const NpadIdType& id, NpadJoyDeviceType type) NN_NOEXCEPT;

/**
 * @brief       Npad の Joy-Con の割り当てモードを "1本ずつ割り当て" に変更し、別の Npad に割り当てられたもう1本の Joy-Con について新たに割り当てられた Npad の Id を取得します。
 *
 * @details
 *              Npad には "2本セット割り当て" と "1本ずつ割り当て" の2つの割り当てモードがあります。
 *
 *              本関数では、指定の Npad に対して接続された Joy-Con を1本ずつ割り当てます。
 *              Joy-Con (R) と Joy-Con (L) がそれぞれ異なる Npad ID となり、
 *              左右の Joy-Con のプレイヤー番号も違うものとなります。
 *
 *              割り当てを変更する際は1つの Npad ID を指定します。
 *              本関数では Npad ID に対してあらかじめ2本の Joy-Con が接続されている場合に、
 *              どちらの Joy-Con を残すかを、NpadJoyDeviceType で指定することができます。
 *              指定されなかったもう一方の Joy-Con は Npad から切断され、
 *              Joy-Con がまったく接続されていない Npad に対して再接続されます
 *
 *              本関数では、別の Npad に対して再接続された Joy-Con が存在する場合に、返り値として true を返し新たに割り当てられた Npad の NpadIdType を取得することができます。
 *
 *              もともと Joy-Con が1本しか接続されていない場合は、type の値は無視され
 *              そのまま "1本ずつ割り当て" のモードに変更されます。
 *              この場合は、返り値が false となります。
 *
 *              Npad への割り当てモードは、本体機能によって外部で変更される可能性があります。
 *              現在の割り当てモードを取得するためには、GetNpadAssignmentMode() を使用してください
 *
 *              本関数は、アプリがインフォーカス状態 ( nn::oe::FocusState_InFocus ) にあるときのみ有効です。
 *              インフォーカス状態以外のときは要求は無視されます。
 *
 * @param[out]  pOutValue              再割り当てされた Joy-Con が存在する場合に、割り当て先の Npad の NpadIdType がセットされます。再割り当てが行われた Joy-Con が存在しない場合は、変更されません。
 * @param[in]   id                     割り当てを変更する Npad ID
 * @param[in]   type                   2本接続されている場合に、 Joy-Con (R) / Joy-Con (L) どちらを残すかを指定する
 *
 * @pre
 *              - 指定されたNpadIdType は NpadId::No* のいずれか
 *
 * @return      新しい Npad に再割り当てが行われた Joy-Con が存在する場合は、true を返します。それ以外の場合は、false を返します。
 *
 */
bool SetNpadJoyAssignmentModeSingle(NpadIdType* pOutValue, const NpadIdType& id, NpadJoyDeviceType type) NN_NOEXCEPT;

/**
 * @brief       Npad の Joy-Con の割り当てモードを "2本セット割り当て" に変更します。
 *
 * @details
 *              Npad には "2本セット割り当て" と "1本ずつ割り当て" の2つの割り当てモードがあります。
 *
 *              本関数では、指定の Npad に対して接続された Joy-Con を2本セットで割り当てます。
 *              Joy-Con (R) と Joy-Con (L) がそれぞれ異なる Npad ID となり、
 *              左右の Joy-Con のプレイヤー番号も違うものとなります。
 *
 *              割り当てを変更する際は1つの Npad ID を指定します。
 *              割り当てモードが "2本セット割り当て" になると、新たに Joy-Con が接続された際にペアになるように割り当てられます。
 *
 *              Npad への割り当てモードは、本体機能によって外部で変更される可能性があります。
 *              現在の割り当てモードを取得するためには、GetNpadAssignmentMode() を使用してください
 *
 *              本関数は、アプリがインフォーカス状態 ( nn::oe::FocusState_InFocus ) にあるときのみ有効です。
 *              インフォーカス状態以外のときは要求は無視されます。
 *
 * @param[in]   id                     割り当てを変更する Npad ID
 *
 * @pre
 *              - 指定されたNpadIdType は NpadId::No* のいずれか
 *
 */
void SetNpadJoyAssignmentModeDual(const NpadIdType& id) NN_NOEXCEPT;

/**
 * @brief       Joy-Con が片方ずつ割り当てられた 2つの Npad をマージして1つのNpadに割り当てます。
 *
 * @details     本関数では、あらかじめ割り当てを変更する際は2つの Npad の Npad ID を指定することで、
 *              Joy-Con が片方ずつ割り当てられた 2つの Npad をマージして1つのNpadに割り当てます。
 *              2つの Npad は、左右の Joy-Con がそれぞれに接続されている必要があります。
 *              一方の Npad に Joy-Con が 2本接続されていたり、両方に Joy-Con (R) が接続されていたりすると変更に失敗します。
 *
 *              変更が行われると両方の NpadIdType の割り当てモードが "2本セット割り当て" モードに変更されます。
 *
 *              本関数は、アプリがインフォーカス状態 ( nn::oe::FocusState_InFocus ) にあるときのみ有効です。
 *              インフォーカス状態以外のときは要求は無視されます。
 *
 * @param[in]   id1                    マージを行う Joy-Con が片方だけ接続された1つめの Npad Id
 * @param[in]   id2                    マージを行う Joy-Con が片方だけ接続された1つめの Npad Id
 *
 * @retresult
 *   @handleresult{nn::ResultSuccess,                      処理に成功しました。}
 *   @handleresult{nn::hid::ResultNpadDualConnected,       Joy-Con が2本接続された Npad が指定されたためマージできません。}
 *   @handleresult{nn::hid::ResultNpadSameJoyTypeConnected,同じ種類の Joy-Con が接続された2つのNpadが指定されたためマージできません。}
 * @endretresult
 *
 * @pre
 *              - 指定されたNpadIdType は NpadId::No* のいずれか
 *              - id1 に Joy-Con (R) ( Joy-Con (L) ), id2 に Joy-Con (L) ( Joy-Con (R) )のみが割り当てられている
 *
 * @post
 *              - id1 に対して Joy-Con 2本が割り当てられ、id2 には1本も割り当てられていない
 *
 */
Result MergeSingleJoyAsDualJoy(const NpadIdType& id1, const NpadIdType& id2) NN_NOEXCEPT;

/**
 * @brief       指定した 2つの Npad について物理的なコントローラーとの対応関係をいれかえます。
 *
 * @details     指定した 2つの Npad に対応づけられた物理的なコントローラーをいれかえます。
 *              物理的なコントローラーが対応づけられていない場合でも、本関数は処理を実行します
 *              id1 にあるコントローラーが対応づけられており、id2 には何も対応づけられていない場合、
 *              id1 のコントローラーが、id2 に対して対応づけられます。
 *
 *              本関数は、アプリがインフォーカス状態 ( nn::oe::FocusState_InFocus ) にあるときのみ有効です。
 *              インフォーカス状態以外のときは要求は無視されます。
 *
 * @param[in]   id1                     1つめの Npad ID
 * @param[in]   id2                     2つめの Npad ID
 *
 * @pre         - id1 及び id2 には NpadId::No1 ～ NpdaId::No8 のいずれかのみがセットできます。
 *
 */
void SwapNpadAssignment(const NpadIdType& id1, const NpadIdType& id2) NN_NOEXCEPT;

/**
 * @brief       Joy-Con の持ち方を設定します。
 *
 * @details     Joy-Con の持ち方を設定します。
 *              持ち方を設定することでアプリが指定する操作スタイルのまま本体機能を操作することができます。
 *
 *              Joy-Con の持ち方は NpadJoyAssignmentMode_Single となっている Joy-Con に対して有効です。
 *              Joy-Con の持ち方は全ての Npad に対して共通で設定され、個別の Npad に対して設定することはできません。
 *              デフォルトでは、NpadJoyHoldType_Vertical に設定されています。
 *
 * @param[in]   holdType                    設定する Joy-Con の持ち方
 *
 */
void SetNpadJoyHoldType(NpadJoyHoldType holdType) NN_NOEXCEPT;

/**
 * @brief       Joy-Con の持ち方を取得します。
 *
 * @details     設定されている Joy-Con の持ち方を取得します。
 *              Joy-Con の持ち方に応じて本体機能の操作方法が決定されます。
 *
 * @return      取得する Joy-Con の持ち方
 *
 */
NpadJoyHoldType GetNpadJoyHoldType() NN_NOEXCEPT;

/**
 * @brief       Joy-Con の L/R ボタン押しによる操作スタイル割り当てモードを開始します。
 *
 * @details     本関数を呼ぶと、 Joy-Con のボタンの押下のされ方に応じて、NpadStyleSet 及び NpadJoyAssignmentMode を決定します。
 *              StopLrAssignmentMode() が呼ばれるまで有効です。
 *
 *              左 Joy-Con の Lボタン及び右 Joy-Con の Rボタンが同時に押された場合、左右の Joy-Con が対となり Npad に対して割り当てが行われます。
 *              既に対となっている Joy-Con の L ボタン / Rボタンを押しても変化はありません。
 *
 *              NpadStyleRight または NpadStyleLeft が有効で NpadJoyHoldType が NpadJoyHoldType_Horizontal の場合
 *              Joy-Con の SL / SR ボタンを同時に押すと、NpadStyleSet が NpadStyleRight または NpadStyleLeft となります。
 *              該当の Npad において割り当てが NpadJoyAssignmentMode_Single に変化します。
 *              対となっている Joy-Con の一方の Joy-Con で SL/SR ボタンが押下されると、もう一方の Joy-Con は切断されます。
 *
 *              NpadStyleRight または NpadStyleLeft が有効で NpadJoyHoldType が NpadJoyHoldType_Vertical の場合
 *              Joy-Con は優先的に、NpadStyleRight または NpadStyleLeft として接続され、割り当てが NpadJoyAssignmentMode_Single に変化します。
 *
 *              既に StartLrAssignmentMode が呼ばれている場合は、動作に対して何も影響を与えません。
 *
 */
void StartLrAssignmentMode() NN_NOEXCEPT;

/**
 * @brief       Joy-Con の L/R ボタン押しによる操作スタイル割り当てモードを終了します。
 *
 * @details     StartLrAssignmentMode() で開始された L/R ボタン押しによる操作スタイル割り当てモードを終了します。
 *              StartLrAssignmentMode() が呼ばれていない場合、本関数は動作に対して何も影響を与えません。
 *
 */
void StopLrAssignmentMode() NN_NOEXCEPT;

/**
 * @brief       Handheld 操作スタイルを有効化するモードを設定します。
 *
 * @details
 *              Handheld 操作スタイルが有効になる条件によって、以下 2 つのモードがあります。
 *
 *              - NpadHandheldActivationMode_Dual
 *              左右の Joy-Con が本体にジョイントされると Handheld 操作スタイルが有効になるモードです。
 *              デフォルト状態ではこちらのモード設定になります。
 *
 *              - NpadHandheldActivationMode_Single
 *              左右いずれか少なくとも一方の Joy-Con が本体にジョイントされると Handheld 操作スタイルが有効になるモードです。
 *              片方だけ本体にジョイントしている場合、ジョイントされていない Joy-Con の入力は無入力（デジタルボタンの押下は無く、アナログスティックはニュートラル位置）となります。
 *              6 軸センサーの入力状態は、ジョイントされている Joy-Con から取得されます。
 *
 *              - NpadHandheldActivationMode_None
 *              Joy-Con が本体にジョイントされてない場合でも Handheld 操作スタイルが有効になるモードです。
 *              ジョイントされていない Joy-Con の入力は無入力（デジタルボタンの押下は無く、アナログスティックはニュートラル位置）となります。
 *              6 軸センサーの入力状態は、本体の 6 軸センサーから取得されます。
 *
 * @param[in]   mode                    設定する Handheld 操作スタイルを認識するモード
 *
 */
void SetNpadHandheldActivationMode(NpadHandheldActivationMode mode) NN_NOEXCEPT;

/**
 * @brief       Handheld 操作スタイルを認識するモードを取得します。
 *
 * @details     現在の Handheld 操作スタイルを認識するモードを取得します。
 *
 * @return      現在の Handheld 操作スタイルを認識するモード
 *
 */
NpadHandheldActivationMode GetNpadHandheldActivationMode() NN_NOEXCEPT;

/**
 * @brief       Joy-Con の通信モードを設定します。 <b>廃止予定です。</b>
 *
 * @details     Joy-Con の通信モードを設定します。
 *              モード変更時に既に接続されているコントローラーは全て切断されます。
 *              本関数は通信周期や接続台数変更をテストするための暫定機能です。
 *              製品相当の機能実装が完了し次第、近いSDKで廃止予定です。
 *
 * @param[in]   mode                    設定する通信モード
 *
 */
void SetNpadCommunicationMode(NpadCommunicationMode mode) NN_NOEXCEPT;

/**
 * @brief       Joy-Con の通信モードを取得します。 <b>廃止予定です。</b>
 *
 * @details     設定されている Joy-Con の通信モードを取得します。
 *              本関数は通信周期や接続台数変更をテストするための暫定機能です。
 *              製品相当の機能実装が完了し次第、近いSDKで廃止予定です。
 *
 * @return      現在の Joy-Con の通信モード
 *
 */
NpadCommunicationMode GetNpadCommunicationMode() NN_NOEXCEPT;


//! @}

}} // namespace nn::hid
