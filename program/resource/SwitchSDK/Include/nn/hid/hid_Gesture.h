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
 * @brief       ジェスチャに関する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/util/util_BitFlagSet.h>
#include <nn/util/util_MathTypes.h>

namespace nn { namespace hid {

const int GesturePointCountMax = 4;     //!< ジェスチャを構成する点の最大数

const int GestureStateCountMax = 16;    //!< 内部的に保持されるジェスチャの状態の最大数

/**
 * @brief       ジェスチャの種類を表す列挙体です。
 */
enum GestureType
{
    GestureType_Idle,       //!< 待機
    GestureType_Complete,   //!< 完了
    GestureType_Cancel,     //!< キャンセル
    GestureType_Touch,      //!< タッチ
    GestureType_Press,      //!< ロングプレス
    GestureType_Tap,        //!< タップ
    GestureType_Pan,        //!< パン
    GestureType_Swipe,      //!< スワイプ
    GestureType_Pinch,      //!< ピンチ
    GestureType_Rotate,     //!< 回転
};

/**
 * @brief       ジェスチャの方向を表す列挙体です。
 */
enum GestureDirection
{
    GestureDirection_None,  //!< 無し
    GestureDirection_Left,  //!< 左
    GestureDirection_Up,    //!< 上
    GestureDirection_Right, //!< 右
    GestureDirection_Down,  //!< 下
};

/**
 * @brief       ジェスチャの属性定義です。
 */
struct GestureAttribute
{
    typedef ::nn::util::BitFlagSet<32, GestureAttribute>::Flag<4>
            IsNewTouch;     //!< 持ち越しを含まない新規のタッチか否か
    typedef ::nn::util::BitFlagSet<32, GestureAttribute>::Flag<8>
            IsDoubleTap;    //!< ダブルタップか否か
};

/**
 * @brief       ジェスチャの属性集合を扱う型です。
 */
typedef ::nn::util::BitFlagSet<32, GestureAttribute> GestureAttributeSet;

/**
 * @brief       ジェスチャを構成する点を表す構造体です。
 */
struct GesturePoint
{
    int32_t x;  //!< ジェスチャを構成する点の x 座標です。
    int32_t y;  //!< ジェスチャを構成する点の y 座標です。
};

/**
 * @brief       ジェスチャの状態を表す構造体です。
 */
struct GestureState
{
    int64_t eventNumber;                        //!< ジェスチャの状態が更新される度に増加する値です。
    int64_t contextNumber;                      //!< ジェスチャの検出が開始される度に増加する値です。
    int32_t _type;
    int32_t _direction;
    int32_t x;                                  //!< ジェスチャの x 座標です。
    int32_t y;                                  //!< ジェスチャの y 座標です。
    int32_t deltaX;                             //!< ジェスチャの x 座標の移動差分です。
    int32_t deltaY;                             //!< ジェスチャの y 座標の移動差分です。
    ::nn::util::Float2 velocity;                //!< ジェスチャの速度 ([/s]) です。
    GestureAttributeSet attributes;             //!< ジェスチャの属性です。
    float scale;                                //!< ジェスチャのスケールです。
    float rotationAngle;                        //!< ジェスチャの時計回りを正とする回転の度数です。
    int32_t pointCount;                         //!< ジェスチャを構成する点の数です。
    GesturePoint points[GesturePointCountMax];  //!< ジェスチャを構成する点です。

    /**
     * @brief   ジェスチャの種類を返します。
     *
     * @return  ジェスチャの種類です。
     */
    GestureType GetGestureType() const NN_NOEXCEPT
    {
        return static_cast<GestureType>(_type);
    }

    /**
     * @brief   ジェスチャの方向を返します。
     *
     * @return  ジェスチャの方向です。
     */
    GestureDirection GetGestureDirection() const NN_NOEXCEPT
    {
        return static_cast<GestureDirection>(_direction);
    }

    /**
     * @brief   ダブルタップか否かを表す値を返します。
     *
     * @return  ダブルタップか否かを表す値です。
     */
    bool IsDoubleTap() const NN_NOEXCEPT
    {
        return attributes.Test<GestureAttribute::IsDoubleTap>();
    }
};

//! @name ジェスチャ関連 API
//! @{

/**
 * @brief       ジェスチャ関連 API を初期化します。
 */
void InitializeGesture() NN_NOEXCEPT;

/**
 * @brief       ジェスチャの状態を過去に遡って読み出します。
 *
 * @details     最新のものから過去に遡って利用可能な数だけ順にジェスチャの状態を読み出します。
 *              利用可能な値の数より大きなバッファ（配列）が指定された場合、
 *              余った領域に対しては何も行いません。
 *              読み出し可能な値の最大数は GestureStateCountMax 個です。
 *              利用可能な値には読み出し済みのものも含まれます。
 *              差分だけを利用したい場合は GestureState::eventNumber を参照してください。
 *
 * @param[out]  outValues                   入力状態を読み出すバッファ（配列）
 * @param[in]   count                       outValues の数
 *
 * @return      読み出した入力状態の数を返します。
 *
 * @pre
 *              - InitializeGesture() の呼び出しが完了している
 *              - outValues != nullptr
 *              - count >= 0
 * @post
 *              - 戻り値 n について、n >= 0
 */
int GetGestureStates(GestureState* outValues, int count) NN_NOEXCEPT;

//! @}

}} // namespace nn::hid
