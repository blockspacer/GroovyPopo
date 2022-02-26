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
 * @brief       DebugPad のエミュレーション設定に関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/settings/settings_GenericPadAxis.h>
#include <nn/settings/settings_GenericPadButton.h>

namespace nn { namespace settings {

/**
 * @brief       DebugPad に対する汎用ゲームパッドの割り当てを表す構造体です。
 */
struct DebugPadGenericPadMap
{
    int32_t genericPadId;       //!< DebugPad に割り当てる汎用ゲームパッドの識別子です。
    int32_t buttonA;            //!< DebugPad の A ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonB;            //!< DebugPad の B ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonX;            //!< DebugPad の X ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonY;            //!< DebugPad の Y ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonL;            //!< DebugPad の L ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonR;            //!< DebugPad の R ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonZL;           //!< DebugPad の ZL ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonZR;           //!< DebugPad の ZR ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonStart;        //!< DebugPad の Start ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonSelect;       //!< DebugPad の Select ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonLeft;         //!< DebugPad の十字ボタン左に割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonUp;           //!< DebugPad の十字ボタン上に割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonRight;        //!< DebugPad の十字ボタン右に割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonDown;         //!< DebugPad の十字ボタン下に割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t axisAnalogStickRX;  //!< DebugPad の右アナログスティックの x 軸に割り当てる汎用ゲームパッドの座標軸です。
    int32_t axisAnalogStickRY;  //!< DebugPad の右アナログスティックの y 軸に割り当てる汎用ゲームパッドの座標軸です。
    int32_t axisAnalogStickLX;  //!< DebugPad の左アナログスティックの x 軸に割り当てる汎用ゲームパッドの座標軸です。
    int32_t axisAnalogStickLY;  //!< DebugPad の左アナログスティックの y 軸に割り当てる汎用ゲームパッドの座標軸です。
    GenericPadAxisAttributeSet attributes;  //!< DebugPad に対する汎用ゲームパッドの割り当ての属性です。
};

/**
 * @brief       DebugPad に対するキーボードの割り当てを表す構造体です。
 */
struct DebugPadKeyboardMap
{
    int32_t buttonA;        //!< DebugPad の A ボタンに割り当てるキーボードのキーです。
    int32_t buttonB;        //!< DebugPad の B ボタンに割り当てるキーボードのキーです。
    int32_t buttonX;        //!< DebugPad の X ボタンに割り当てるキーボードのキーです。
    int32_t buttonY;        //!< DebugPad の Y ボタンに割り当てるキーボードのキーです。
    int32_t buttonL;        //!< DebugPad の L ボタンに割り当てるキーボードのキーです。
    int32_t buttonR;        //!< DebugPad の R ボタンに割り当てるキーボードのキーです。
    int32_t buttonZL;       //!< DebugPad の ZL ボタンに割り当てるキーボードのキーです。
    int32_t buttonZR;       //!< DebugPad の ZR ボタンに割り当てるキーボードのキーです。
    int32_t buttonStart;    //!< DebugPad の Start ボタンに割り当てるキーボードのキーです。
    int32_t buttonSelect;   //!< DebugPad の Select ボタンに割り当てるキーボードのキーです。
    int32_t buttonLeft;     //!< DebugPad の十字ボタン左に割り当てるキーボードのキーです。
    int32_t buttonUp;       //!< DebugPad の十字ボタン上に割り当てるキーボードのキーです。
    int32_t buttonRight;    //!< DebugPad の十字ボタン右に割り当てるキーボードのキーです。
    int32_t buttonDown;     //!< DebugPad の十字ボタン下に割り当てるキーボードのキーです。
};

/**
 * @brief       DebugPad に対する汎用ゲームパッドの割り当てを取得します。
 *
 * @details     DebugPad に割り当てられている汎用ゲームパッドは識別子 DebugPadGenericPadMap::genericPadId から識別することができます。
 *              識別子はプラットフォーム固有の値であり、プラットフォーム毎に規定されます。
 *              DebugPadGenericPadMap::buttonA の様な各デジタルボタンへの割り当ての指定には GenericPadButton で定義された値が使用されます。
 *              例として、 DebugPadGenericPadMap::buttonA に GenericPadButton_1 が指定されていた場合、
 *              識別子が DebugPadGenericPadMap::genericPadId である汎用ゲームパッドのボタン 1 の押下状態が DebugPad のボタン A に対応付けられます。
 *              設定値として 0 が指定された場合は、そのデジタルボタンに対応する汎用ゲームパッドのボタンは無いものとして扱われます。
 *              DebugPadGenericPadMap::axisAnalogStickRX の様な各座標軸への割り当ての指定には GenericPadAxis で定義された値が使用されます。
 *              例として、 DebugPadGenericPadMap::axisAnalogStickRX に GenericPadAxis_X が指定されていた場合、
 *              識別子が DebugPadGenericPadMap::genericPadId である汎用ゲームパッドの x 軸の座標が DebugPad の右アナログスティックの x 軸に対応付けられます。
 *              設定値として 0 が指定された場合は、その座標軸に対応する汎用ゲームパッドの座標軸は無いものとして扱われます。
 *              座標値の増減方向が反転されているか否かは DebugPadGenericPadMap::attributes から判定することができます。
 *              例として、DebugPadGenericPadMap::attributes.Get<DebugPadGenericPadMapAttribute::IsXAxisInverted>() の値が真である場合、
 *              汎用ゲームパッドの x 軸は座標値の増減方向を反転して利用されています。
 *              DebugPad に対する汎用ゲームパッドの割り当てが行われていない状態で当該関数を呼び出した場合は、内部的に解決した割り当てを返します。
 *
 * @param[out]  outValue                    DebugPad に対する汎用ゲームパッドの割り当てを読み出すバッファ
 *
 * @pre
 *              - outValue != nullptr
 */
void GetDebugPadGenericPadMap(DebugPadGenericPadMap* outValue) NN_NOEXCEPT;

/**
 * @brief       DebugPad に対する汎用ゲームパッドの割り当てを設定します。
 *
 * @details     DebugPadGenericPadMap の各メンバの扱いについては GetDebugPadGenericPadMap() を参照してください。
 *              割り当ての設定は当該関数の呼び出し完了直後から反映されます。
 *              不正な割り当てが指定された場合、設定自体は行われますが、割り当て関係は無いものとして扱われます。
 *              設定の解除には ResetDebugPadGenericPadMap() を使用してください。
 *
 * @param[in]   value                       DebugPad に対する汎用ゲームパッドの割り当て
 */
void SetDebugPadGenericPadMap(const DebugPadGenericPadMap& value) NN_NOEXCEPT;

/**
 * @brief       DebugPad に対する汎用ゲームパッドの割り当てをリセットします。
 *
 * @details     割り当てが設定されている場合はこれを削除し、内部的な割り当ての解決を有効に戻します。
 *              割り当てのリセットは当該関数の呼び出し完了直後から反映されます。
 */
void ResetDebugPadGenericPadMap() NN_NOEXCEPT;

/**
 * @brief       DebugPad に対するキーボードの割り当てを取得します。
 *
 * @details     DebugPadKeyboardMap::buttonA の様な各デジタルボタンへの割り当ての指定には ::nn::hid::KeyboardKey のビット位置が使用されます。
 *              例として、 DebugPadGenericPadMap::buttonA に ::nn::hid::KeyboardKey::A の Index が指定されていた場合、
 *              キーボードのキー A の押下状態が DebugPad のボタン A に対応付けられます。
 *              設定値として 0 が指定された場合は、そのデジタルボタンに対応するキーボードのキーは無いものとして扱われます。
 *              DebugPad に対するキーボードの割り当てが行われていない状態で当該関数を呼び出した場合は、
 *              全てのデジタルボタンについて割り当てが無いことを示す値を返します。
 *
 * @param[out]  outValue                    DebugPad に対するキーボードの割り当てを読み出すバッファ
 *
 * @pre
 *              - outValue != nullptr
 */
void GetDebugPadKeyboardMap(DebugPadKeyboardMap* outValue) NN_NOEXCEPT;

/**
 * @brief       DebugPad に対するキーボードの割り当てを設定します。
 *
 * @details     DebugPadKeyboardMap の各メンバの扱いについては GetDebugPadKeyboardMap() を参照してください。
 *              割り当ての設定は当該関数の呼び出し完了直後から反映されます。
 *              不正な割り当てが指定された場合、設定自体は行われますが、割り当て関係は無いものとして扱われます。
 *              設定の解除には ResetDebugPadKeyboardMap() を使用してください。
 *
 * @param[in]   value                       DebugPad に対するキーボードの割り当て
 */
void SetDebugPadKeyboardMap(const DebugPadKeyboardMap& value) NN_NOEXCEPT;

/**
 * @brief       DebugPad に対するキーボードの割り当てをリセットします。
 *
 * @details     割り当てが設定されている場合はこれを削除します。
 *              割り当てのリセットは当該関数の呼び出し完了直後から反映されます。
 */
void ResetDebugPadKeyboardMap() NN_NOEXCEPT;

}} // namespace nn::settings
