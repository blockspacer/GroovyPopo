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
 * @brief       Mouse に関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/util/util_BitFlagSet.h>

namespace nn { namespace hid {

const int MouseStateCountMax = 16;   //!< 内部的に保持される Mouse の入力状態の最大数

/**
 * @brief       Mouse のボタン定義です。
 */
struct MouseButton
{
    typedef ::nn::util::BitFlagSet<32, MouseButton>::Flag<0> Left;      //!< Mouse の左ボタン
    typedef ::nn::util::BitFlagSet<32, MouseButton>::Flag<1> Right;     //!< Mouse の右ボタン
    typedef ::nn::util::BitFlagSet<32, MouseButton>::Flag<2> Middle;    //!< Mouse の中央ボタン
    typedef ::nn::util::BitFlagSet<32, MouseButton>::Flag<3> Forward;   //!< Mouse のフォワードボタン
    typedef ::nn::util::BitFlagSet<32, MouseButton>::Flag<4> Back;      //!< Mouse のバックボタン
};

/**
 * @brief       Mouse のデジタルボタンの集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<32, MouseButton> MouseButtonSet;

/**
 * @brief       Mouse の入力状態の属性定義です。
 */
struct MouseAttribute
{
    typedef ::nn::util::BitFlagSet<32, MouseAttribute>::Flag<0>
            Transferable;   //!< 上位システムへマウス操作を移譲可能か否か
    typedef ::nn::util::BitFlagSet<32, MouseAttribute>::Flag<1>
            IsConnected;    //!< Mouse がシステムと接続状態にあるか否か
};

/**
 * @brief       Mouse の入力状態の属性集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<32, MouseAttribute> MouseAttributeSet;

/**
 * @brief        Mouse の入力状態を表す構造体です。
 */
struct MouseState
{
    int64_t samplingNumber;         //!< Mouse の入力状態がサンプリングされる度に増加する値です。
    int32_t x;                      //!< カーソルの x 座標です。
    int32_t y;                      //!< カーソルの y 座標です。
    int32_t deltaX;                 //!< カーソルの x 座標の移動差分です。
    int32_t deltaY;                 //!< カーソルの y 座標の移動差分です。
    int32_t wheelDelta;             //!< ホイールの回転差分です。
    ::nn::Bit8 _reserved[4];
    MouseButtonSet buttons;         //!< ボタンの状態です。
    MouseAttributeSet attributes;   //!< Mouse の入力状態の属性です。
};

//! @name Mouse 関連 API
//! @{

/**
 * @brief       Mouse を初期化します。
 */
void InitializeMouse() NN_NOEXCEPT;

/**
 * @brief       Mouse の最新の入力状態を取得します。
 *
 * @details     GetMouseStates() で 1 つの入力状態を読み出した時と同じ値が返ります。
 *              最低 1 回の入力状態の更新が保証されるため、InitializeMouse() の呼び出し完了直後から利用可能です。
 *
 * @param[out]  pOutValue                   入力状態を読み出すバッファ
 *
 * @pre
 *              - InitializeMouse() の呼び出しが完了している
 *              - pOutValue != nullptr
 */
void GetMouseState(MouseState* pOutValue) NN_NOEXCEPT;

/**
 * @brief       Mouse の入力状態を過去に遡って読み出します。
 *
 * @details     最新のものから過去に遡って利用可能な数だけ順に入力状態を読み出します。
 *              利用可能な入力状態の数より大きなバッファ（配列）が指定された場合、余った領域に対しては何も行いません。
 *              読み出し可能な入力状態の最大数は MouseStateCountMax 個です。
 *              利用可能な入力状態には読み出し済みのものも含まれます。
 *              差分だけを利用したい場合は MouseState::samplingNumber を参照してください。
 *
 * @param[out]  outValues                   入力状態を読み出すバッファ（配列）
 * @param[in]   count                       outValues の数
 *
 * @return      読み出した入力状態の数を返します。
 *
 * @pre
 *              - InitializeMouse() の呼び出しが完了している
 *              - outValues != nullptr
 *              - count >= 0
 * @post
 *              - 戻り値 n について、n >= 0
 */
int GetMouseStates(MouseState outValues[], int count) NN_NOEXCEPT;

//! @}

}} // namespace nn::hid
