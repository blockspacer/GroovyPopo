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
 * @brief   振動子に関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/hid/hid_Npad.h>
#include <nn/hid/hid_VibrationValueTypes.h>

namespace nn { namespace hid {

/**
 * @brief 振動子のハンドルです。
 */
struct VibrationDeviceHandle
{
    uint32_t _storage;
};

/**
 * @brief   振動子の物理的な種類を表す列挙型です。
 */
enum VibrationDeviceType
{
    VibrationDeviceType_Unknown = 0,            //!< 不明な種類
    VibrationDeviceType_LinearResonantActuator, //!< LRA (リニア振動モーター)
};

/**
 * @brief   振動子の物理的な搭載位置を表す列挙型です。
 */
enum VibrationDevicePosition
{
    VibrationDevicePosition_None = 0,   //!< 位置情報無し
    VibrationDevicePosition_Left,       //!< 左
    VibrationDevicePosition_Right,      //!< 右
};

/**
 * @brief   振動子の情報を表す構造体です。
 */
struct VibrationDeviceInfo
{
    VibrationDeviceType deviceType;     //!< 振動子の物理的な種類
    VibrationDevicePosition position;   //!< 振動子の搭載位置
};

//! @name 振動子の制御関連 API
//! @{

/**
* @brief       Npad に搭載されている振動子を操作するためのハンドルを取得します。
*
* @details     Npad ID で指定された Npad に対応する振動子を操作するためのハンドルを取得します。
*              振動子に対するハンドルは Npad Id に対して操作スタイル(NpadStyleSet)毎に取得します。
*              操作スタイルは NpadStyleSet で指定しますが、複数の NpadStyle を同時に指定することはできません。
*              NpadStyleSet によって取得できるハンドルの数が異なるため、取得できたハンドルの数が返り値として取得できます。
*
* @param[out]  pOutValues      振動子のハンドルの格納先
* @param[in]   count           pOutValues で取得するハンドルの最大数
* @param[in]   id              Npad ID
* @param[in]   style           ハンドルを取得する Npad の操作スタイル
*
* @return      取得されたハンドルの数
*
* @pre
*              - style.IsAnyOn() == true
*              - style.CountPopulation() == 1
*/
int GetVibrationDeviceHandles(VibrationDeviceHandle* pOutValues, int count, const NpadIdType& id, NpadStyleSet style) NN_NOEXCEPT;

/**
 * @brief       振動子の情報を取得します。
 *
 * @param[out]  pOutValue       振動子の情報の格納先
 * @param[in]   handle          振動子のハンドル
 *
 * @pre
 *              - 指定の handle が GetVibrationDeviceHandles() 関数の呼び出しによって取得されている
 */
void GetVibrationDeviceInfo(VibrationDeviceInfo* pOutValue, const VibrationDeviceHandle& handle) NN_NOEXCEPT;

/**
 * @brief       振動子を初期化します。
 *
 * @param[in]   handle          振動子のハンドル
 *
 * @pre
 *              - 指定の handle が GetVibrationDeviceHandles() 関数の呼び出しによって取得されている
 *
 * @details
 *  振動子に対する初期化処理を行います。
 *  SendVibrationValue() 関数や GetActualVibrationValue() 関数を利用するためには、
 *  事前にこの関数を実行して振動子を初期化済みの状態にしておく必要があります。
 *
 *  対象デバイスが初期化されていない場合には、この関数の呼び出しをトリガとしてデバイス内の初期化処理が始まります。
 *  デバイス内の初期化処理が始まってからデバイスが振動可能な状態になるまで、最大で数百ミリ秒程度の時間がかかります。
 *  振動可能な状態になるまでに送信された振動値は無視されます。
 */
void InitializeVibrationDevice(const VibrationDeviceHandle& handle) NN_NOEXCEPT;

/**
 * @brief       振動子に振動値を送信します。
 *
 * @param[in]   handle          振動子のハンドル
 * @param[in]   value           振動値
 *
 * @pre
 *              - 対応する振動子について InitializeVibrationDevice() の呼び出しが完了している
 *
 * @details
 *  振動子に振動値を送信します。
 *  送信した振動値は別の値で上書きされるまで保持されます。
 *  そのため、振動を止めるためには明示的に振幅ゼロの振動値を送信する必要があります。
 */
void SendVibrationValue(const VibrationDeviceHandle& handle, const VibrationValue& value) NN_NOEXCEPT;

/**
 * @brief       振動子で発生している実際の振動値を取得します。
 *
 * @param[out]  pOutValue       実際の振動値の格納先
 * @param[in]   handle          振動子のハンドル
 *
 * @pre
 *              - 対応する振動子について InitializeVibrationDevice() の呼び出しが完了している
 *
 * @details
 *  振動子で発生している実際の振動値を取得します。
 *
 *  プログラム上で生成された振動値は符号化された状態で振動子に送信されるため、
 *  実際の振動値はプログラム上で生成された振動値と必ずしも一致しません。@n
 *  またシステム側の設定により、プログラム上で生成された振動値に調整が
 *  加えられる場合もあります。
 *
 *  SendVibrationValue() 関数で指定した振動値が実際に振動子に送信されるまで、
 *  最大で数十ミリ秒程度の時間がかかります。
 *  一方この関数で取得される振動値は、実際に振動子に送信された最新の振動値です。
 *  したがって、 SendVibrationValue() 関数で送信した振動値が
 *  この関数で取得される振動値に反映されるまでの間に、
 *  最大で数十ミリ秒程度のタイミングのずれが発生します。
 */
void GetActualVibrationValue(VibrationValue* pOutValue, const VibrationDeviceHandle& handle) NN_NOEXCEPT;


//! @}

//! @name 振動機能の設定関連 API
//! @{

/**
* @brief       本体設定において振動機能が許可されているかを取得します。
*
* @return      振動機能が許可されていれば true を、禁止されていれば false を返します。
*
* @details
*  HOMEメニューの「設定」→「コントローラーとセンサー」→「コントローラーの振動」の設定値を取得します。
*
*  振動機能が禁止されている場合であっても振動関連の関数を呼び出すことはできますが、
*  振動子を実際に振動させることはできません。
*/
bool IsVibrationPermitted() NN_NOEXCEPT;

//! @}

}} // namespace nn::hid
