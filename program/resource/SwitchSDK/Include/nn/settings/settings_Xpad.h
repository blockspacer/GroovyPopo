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
 * @brief       Xpad のエミュレーション設定に関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/settings/settings_GenericPadAxis.h>
#include <nn/settings/settings_GenericPadButton.h>

namespace nn { namespace settings {

/**
 * @brief       基本的な構成を持つ Xpad に対する汎用ゲームパッドの割り当てを表す構造体です。
 */
struct BasicXpadGenericPadMap
{
    int32_t genericPadId;       //!< 基本的な構成を持つ Xpad に割り当てる汎用ゲームパッドの識別子です。
    int32_t buttonA;            //!< 基本的な構成を持つ Xpad の A ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonB;            //!< 基本的な構成を持つ Xpad の B ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonX;            //!< 基本的な構成を持つ Xpad の X ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonY;            //!< 基本的な構成を持つ Xpad の Y ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonStickL;       //!< 基本的な構成を持つ Xpad の L スティックボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonStickR;       //!< 基本的な構成を持つ Xpad の R スティックボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonL;            //!< 基本的な構成を持つ Xpad の L ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonR;            //!< 基本的な構成を持つ Xpad の R ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonZL;           //!< 基本的な構成を持つ Xpad の ZL ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonZR;           //!< 基本的な構成を持つ Xpad の ZR ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonStart;        //!< 基本的な構成を持つ Xpad の Start ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonSelect;       //!< 基本的な構成を持つ Xpad の Select ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonLeft;         //!< 基本的な構成を持つ Xpad の十字ボタン左に割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonUp;           //!< 基本的な構成を持つ Xpad の十字ボタン上に割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonRight;        //!< 基本的な構成を持つ Xpad の十字ボタン右に割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonDown;         //!< 基本的な構成を持つ Xpad の十字ボタン下に割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t axisAnalogStickRX;  //!< 基本的な構成を持つ Xpad の右アナログスティックの x 軸に割り当てる汎用ゲームパッドの座標軸です。
    int32_t axisAnalogStickRY;  //!< 基本的な構成を持つ Xpad の右アナログスティックの y 軸に割り当てる汎用ゲームパッドの座標軸です。
    int32_t axisAnalogStickLX;  //!< 基本的な構成を持つ Xpad の左アナログスティックの x 軸に割り当てる汎用ゲームパッドの座標軸です。
    int32_t axisAnalogStickLY;  //!< 基本的な構成を持つ Xpad の左アナログスティックの y 軸に割り当てる汎用ゲームパッドの座標軸です。
    GenericPadAxisAttributeSet attributes;  //!< 汎用ゲームパッドの座標軸の属性です。
};

/**
 * @brief       基本的な構成を持つ Xpad に対するキーボードの割り当てを表す構造体です。
 */
struct BasicXpadKeyboardMap
{
    int32_t buttonA;        //!< 基本的な構成を持つ Xpad の A ボタンに割り当てるキーボードのキーです。
    int32_t buttonB;        //!< 基本的な構成を持つ Xpad の B ボタンに割り当てるキーボードのキーです。
    int32_t buttonX;        //!< 基本的な構成を持つ Xpad の X ボタンに割り当てるキーボードのキーです。
    int32_t buttonY;        //!< 基本的な構成を持つ Xpad の Y ボタンに割り当てるキーボードのキーです。
    int32_t buttonStickL;   //!< 基本的な構成を持つ Xpad の L スティックボタンに割り当てるキーボードのキーです。
    int32_t buttonStickR;   //!< 基本的な構成を持つ Xpad の R スティックボタンに割り当てるキーボードのキーです。
    int32_t buttonL;        //!< 基本的な構成を持つ Xpad の L ボタンに割り当てるキーボードのキーです。
    int32_t buttonR;        //!< 基本的な構成を持つ Xpad の R ボタンに割り当てるキーボードのキーです。
    int32_t buttonZL;       //!< 基本的な構成を持つ Xpad の ZL ボタンに割り当てるキーボードのキーです。
    int32_t buttonZR;       //!< 基本的な構成を持つ Xpad の ZR ボタンに割り当てるキーボードのキーです。
    int32_t buttonStart;    //!< 基本的な構成を持つ Xpad の Start ボタンに割り当てるキーボードのキーです。
    int32_t buttonSelect;   //!< 基本的な構成を持つ Xpad の Select ボタンに割り当てるキーボードのキーです。
    int32_t buttonLeft;     //!< 基本的な構成を持つ Xpad の十字ボタン左に割り当てるキーボードのキーです。
    int32_t buttonUp;       //!< 基本的な構成を持つ Xpad の十字ボタン上に割り当てるキーボードのキーです。
    int32_t buttonRight;    //!< 基本的な構成を持つ Xpad の十字ボタン右に割り当てるキーボードのキーです。
    int32_t buttonDown;     //!< 基本的な構成を持つ Xpad の十字ボタン下に割り当てるキーボードのキーです。
};

/**
 * @brief       Xpad に対する汎用ゲームパッドの割り当てを取得します。
 *
 * @deprecated  本関数は廃止予定です。 nn::settings::GetFullKeyGenericPadMap() を使用してください。
 *
 * @param[out]  outValue                    Xpad に対する汎用ゲームパッドの割り当てを読み出すバッファ
 * @param[in]   playerNumber                Xpad のプレイヤー番号
 *
 * @pre
 *              - outValue != nullptr
 */
NN_DEPRECATED
void GetXpadGenericPadMap(BasicXpadGenericPadMap* outValue, int playerNumber
                          ) NN_NOEXCEPT;

/**
 * @brief       Xpad に対する汎用ゲームパッドの割り当てを設定します。 <b>廃止予定です。</b>
 *
 * @deprecated  本関数は廃止予定です。 nn::settings::SetFullKeyGenericPadMap() を使用してください。
 *
 * @param[in]   value                       Xpad に対する汎用ゲームパッドの割り当て
 * @param[in]   playerNumber                Xpad のプレイヤー番号
 */
NN_DEPRECATED
void SetXpadGenericPadMap(const BasicXpadGenericPadMap& value, int playerNumber
                          ) NN_NOEXCEPT;

/**
 * @brief       Xpad に対する汎用ゲームパッドの割り当てをリセットします。 <b>廃止予定です。</b>
 *
 * @deprecated  本関数は廃止予定です。 nn::settings::ResetFullKeyGenericPadMap() を使用してください。
 *
 * @param[in]   playerNumber                Xpad のプレイヤー番号
 */
NN_DEPRECATED
void ResetXpadGenericPadMap(int playerNumber) NN_NOEXCEPT;

/**
 * @brief       Xpad に対するキーボードの割り当てを取得します。 <b>廃止予定です。</b>
 *
 * @deprecated  本関数は廃止予定です。 nn::settings::GetFullKeyKeyboardMap() を使用してください。
 *
 * @param[out]  outValue                    Xpad に対するキーボードの割り当てを読み出すバッファ
 * @param[in]   playerNumber                Xpad のプレイヤー番号
 *
 * @pre
 *              - outValue != nullptr
 */
NN_DEPRECATED
void GetXpadKeyboardMap(BasicXpadKeyboardMap* outValue, int playerNumber
                        ) NN_NOEXCEPT;

/**
 * @brief       Xpad に対するキーボードの割り当てを設定します。 <b>廃止予定です。</b>
 *
 * @deprecated  本関数は廃止予定です。 nn::settings::SetFullKeyKeyboardMap() を使用してください。
 *
 * @param[in]   value                       Xpad に対するキーボードの割り当て
 * @param[in]   playerNumber                Xpad のプレイヤー番号
 */
NN_DEPRECATED
void SetXpadKeyboardMap(const BasicXpadKeyboardMap& value, int playerNumber
                        ) NN_NOEXCEPT;

/**
 * @brief       Xpad に対するキーボードの割り当てをリセットします。 <b>廃止予定です。</b>
 *
 * @deprecated  本関数は廃止予定です。 nn::settings::ResetFullKeyKeyboardMap() を使用してください。
 *
 * @param[in]   playerNumber                Xpad のプレイヤー番号
 */
NN_DEPRECATED
void ResetXpadKeyboardMap(int playerNumber) NN_NOEXCEPT;

}} // namespace nn::settings
