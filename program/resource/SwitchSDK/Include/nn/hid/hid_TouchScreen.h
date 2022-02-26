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
 * @brief       TouchScreen に関する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/nn_StaticAssert.h>
#include <nn/nn_TimeSpan.h>
#include <nn/util/util_BitFlagSet.h>

namespace nn { namespace hid {

const int TouchStateCountMax = 16;  //!< タッチの最大数

const int TouchScreenStateCountMax = 16;    //!< 内部的に保持される TouchScreen の入力状態の最大数

/**
 * @brief       タッチの属性定義です。
 */
struct TouchAttribute
{
    typedef ::nn::util::BitFlagSet<32, TouchAttribute>::Flag<0>
            Start;  //!< タッチが開始されたか否か
    typedef ::nn::util::BitFlagSet<32, TouchAttribute>::Flag<1>
            End;    //!< タッチが解放されたか否か
};

/**
 * @brief       タッチの属性集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<32, TouchAttribute> TouchAttributeSet;

/**
 * @brief       タッチを表す構造体です。
 */
struct TouchState
{
    ::nn::TimeSpanType deltaTime;   //!< タッチが最後にサンプリングされてから経過した時間です。
    TouchAttributeSet attributes;   //!< タッチの属性です。
    int32_t fingerId;               //!< タッチの識別子です。
    int32_t x;                      //!< タッチの x 座標です。
    int32_t y;                      //!< タッチの y 座標です。
    int32_t diameterX;              //!< 楕円で表された接触範囲の x 軸直径です。(TouchState::x, TouchState::y) を楕円の中心座標とします。
    int32_t diameterY;              //!< 楕円で表された接触範囲の y 軸直径です。(TouchState::x, TouchState::y) を楕円の中心座標とします。
    int32_t rotationAngle;          //!< 楕円で表された接触範囲の時計回りを正とする回転の度数です。(TouchState::x, TouchState::y) を楕円の中心座標とします。
    ::nn::Bit8 _reserved[4];
};

/**
 * @brief       TouchScreen の入力状態を表す構造体です。
 *
 * @tparam      N                           格納するタッチの数
 *
 * @pre
 *              - 0 < N && N <= TouchStateCountMax
 */
template<size_t N>
struct TouchScreenState
{
    NN_STATIC_ASSERT(0 < N);
    NN_STATIC_ASSERT(N <= TouchStateCountMax);
    int64_t samplingNumber; //!< TouchScreen の入力状態がサンプリングされる度に増加する値です。
    int32_t count;          //!< タッチの数です。
    ::nn::Bit8 _reserved[4];
    TouchState touches[N];  //!< 接触を開始した時点が古いものから順にタッチを格納します。
};

//! @name TouchScreen 関連 API
//! @{

/**
 * @brief       TouchScreen を初期化します。
 */
void InitializeTouchScreen() NN_NOEXCEPT;

/**
 * @brief       TouchScreen の最新の入力状態を取得します。
 *
 * @details     GetTouchScreenStates() で 1 つの入力状態を読みだした時と同じ値が返ります。
 *              最低 1 回の入力状態の更新が保証されますので、 InitializeTouchScreen() の呼び出し完了直後から利用可能です。
 *
 * @param[out]  pOutValue                   入力状態を読み出すバッファ
 *
 * @tparam      N                           格納するタッチの数
 *
 * @pre
 *              - InitializeTouchScreen() の呼び出しが完了している
 *              - pOutValue != nullptr
 *              - 0 < N && N <= TouchStateCountMax
 * @post
 *              - pOutValue->count <= N
 */
template<size_t N>
void GetTouchScreenState(TouchScreenState<N>* pOutValue) NN_NOEXCEPT;

/**
 * @brief       TouchScreen の入力状態を過去に遡って読み出します。
 *
 * @details     最新のものから過去に遡って利用可能な数だけ順に入力状態を読み出します。
 *              利用可能な入力状態の数より大きなバッファ（配列）が指定された場合、余った領域に対しては何も行いません。
 *              読み出し可能な入力状態の最大数は TouchScreenStateCountMax 個です。
 *              利用可能な入力状態には読み出し済みのものも含まれます。
 *              差分だけを利用したい場合は TouchScreenState::samplingNumber を参照してください。
 *              TouchScreenState::count 個のタッチの情報を TouchScreenState::touches から参照できます。
 *              読み出し可能なタッチの最大数は TouchStateCountMax 個です。
 *              タッチの最大数より検出されたタッチの数が少ない場合であっても、 TouchScreenState::touches は全て更新され、
 *              余った領域は不定値となります。
 *
 * @param[out]  outStates                   入力状態を読み出すバッファ（配列）
 * @param[in]   count                       outStates の数
 *
 * @tparam      N                           格納するタッチの数
 *
 * @return      読み出した入力状態の数を返します。
 *
 * @pre
 *              - InitializeTouchScreen() の呼び出しが完了している
 *              - outStates != nullptr
 *              - count >= 0
 *              - 0 < N && N <= TouchStateCountMax
 * @post
 *              - 戻り値 n について、n >= 0
 *              - 読み出した入力状態 outStates[i] について、outStates[i].count <= N
 */
template<size_t N>
int GetTouchScreenStates(TouchScreenState<N>* outStates, int count) NN_NOEXCEPT;

//! @}

}} // namespace nn::hid
