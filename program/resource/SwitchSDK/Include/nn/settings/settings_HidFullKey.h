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
 * @brief       NpadStyleFullKey に対応したコントローラーをエミュレーションする設定に関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/settings/settings_GenericPadAxis.h>
#include <nn/settings/settings_GenericPadButton.h>

namespace nn { namespace settings {

/**
 * @brief       エミュレーションするコントローラーを一意に識別するための識別子
 *
 * @details     NpadStyleFullKey に対応したコントローラーは最大 nn::settings::FullKeyDeviceCountMax 個エミュレーションすることができます。
 *              エミュレーションする対象となるコントローラーを一意に特定するために本列挙子を使用します。
 *              列挙子の値や順番に意味はありません。
 *              Npad に対しては、割り当てられた汎用ゲームパッドのボタンもしくはキーボードのキーの押下が検出されたタイミングであいている若い Npad に対して順に割り当てが行われます。
 */
enum FullKeyIndex
{
    FullKeyIndex_0 = 0,
    FullKeyIndex_1,
    FullKeyIndex_2,
    FullKeyIndex_3,
    FullKeyIndex_4,
    FullKeyIndex_5,
    FullKeyIndex_6,
    FullKeyIndex_7,
};

/**
 * @brief       エミュレーションするコントローラーの最大数
 */
const int FullKeyDeviceCountMax = 8;

/**
 * @brief       NpadStyleFullKey に対応したコントローラーをエミュレーションするための、汎用ゲームパッドの割り当て設定を表す構造体です。
 */
struct FullKeyGenericPadMap
{
    int32_t genericPadId;       //!< NpadStyleFullKey に対応したコントローラーに割り当てる汎用ゲームパッドの識別子です。
    int32_t buttonA;            //!< NpadStyleFullKey に対応したコントローラーの A ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonB;            //!< NpadStyleFullKey に対応したコントローラーの B ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonX;            //!< NpadStyleFullKey に対応したコントローラーの X ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonY;            //!< NpadStyleFullKey に対応したコントローラーの Y ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonStickL;       //!< NpadStyleFullKey に対応したコントローラーの L スティックボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonStickR;       //!< NpadStyleFullKey に対応したコントローラーの R スティックボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonL;            //!< NpadStyleFullKey に対応したコントローラーの L ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonR;            //!< NpadStyleFullKey に対応したコントローラーの R ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonZL;           //!< NpadStyleFullKey に対応したコントローラーの ZL ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonZR;           //!< NpadStyleFullKey に対応したコントローラーの ZR ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonStart;        //!< NpadStyleFullKey に対応したコントローラーの Start ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonSelect;       //!< NpadStyleFullKey に対応したコントローラーの Select ボタンに割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonLeft;         //!< NpadStyleFullKey に対応したコントローラーの十字ボタン左に割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonUp;           //!< NpadStyleFullKey に対応したコントローラーの十字ボタン上に割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonRight;        //!< NpadStyleFullKey に対応したコントローラーの十字ボタン右に割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t buttonDown;         //!< NpadStyleFullKey に対応したコントローラーの十字ボタン下に割り当てる汎用ゲームパッドのデジタルボタンです。
    int32_t axisAnalogStickRX;  //!< NpadStyleFullKey に対応したコントローラーの右アナログスティックの x 軸に割り当てる汎用ゲームパッドの座標軸です。
    int32_t axisAnalogStickRY;  //!< NpadStyleFullKey に対応したコントローラーの右アナログスティックの y 軸に割り当てる汎用ゲームパッドの座標軸です。
    int32_t axisAnalogStickLX;  //!< NpadStyleFullKey に対応したコントローラーの左アナログスティックの x 軸に割り当てる汎用ゲームパッドの座標軸です。
    int32_t axisAnalogStickLY;  //!< NpadStyleFullKey に対応したコントローラーの左アナログスティックの y 軸に割り当てる汎用ゲームパッドの座標軸です。
    GenericPadAxisAttributeSet attributes;  //!< 汎用ゲームパッドの座標軸の属性です。
};

/**
 * @brief       NpadStyleFullKey に対応したコントローラーをエミュレーションするための、キーボードの割り当て設定を表す構造体です。
 */
struct FullKeyKeyboardMap
{
    int32_t buttonA;        //!< NpadStyleFullKey に対応したコントローラーの A ボタンに割り当てるキーボードのキーです。
    int32_t buttonB;        //!< NpadStyleFullKey に対応したコントローラーの B ボタンに割り当てるキーボードのキーです。
    int32_t buttonX;        //!< NpadStyleFullKey に対応したコントローラーの X ボタンに割り当てるキーボードのキーです。
    int32_t buttonY;        //!< NpadStyleFullKey に対応したコントローラーの Y ボタンに割り当てるキーボードのキーです。
    int32_t buttonStickL;   //!< NpadStyleFullKey に対応したコントローラーの L スティックボタンに割り当てるキーボードのキーです。
    int32_t buttonStickR;   //!< NpadStyleFullKey に対応したコントローラーの R スティックボタンに割り当てるキーボードのキーです。
    int32_t buttonL;        //!< NpadStyleFullKey に対応したコントローラーの L ボタンに割り当てるキーボードのキーです。
    int32_t buttonR;        //!< NpadStyleFullKey に対応したコントローラーの R ボタンに割り当てるキーボードのキーです。
    int32_t buttonZL;       //!< NpadStyleFullKey に対応したコントローラーの ZL ボタンに割り当てるキーボードのキーです。
    int32_t buttonZR;       //!< NpadStyleFullKey に対応したコントローラーの ZR ボタンに割り当てるキーボードのキーです。
    int32_t buttonStart;    //!< NpadStyleFullKey に対応したコントローラーの Start ボタンに割り当てるキーボードのキーです。
    int32_t buttonSelect;   //!< NpadStyleFullKey に対応したコントローラーの Select ボタンに割り当てるキーボードのキーです。
    int32_t buttonLeft;     //!< NpadStyleFullKey に対応したコントローラーの十字ボタン左に割り当てるキーボードのキーです。
    int32_t buttonUp;       //!< NpadStyleFullKey に対応したコントローラーの十字ボタン上に割り当てるキーボードのキーです。
    int32_t buttonRight;    //!< NpadStyleFullKey に対応したコントローラーの十字ボタン右に割り当てるキーボードのキーです。
    int32_t buttonDown;     //!< NpadStyleFullKey に対応したコントローラーの十字ボタン下に割り当てるキーボードのキーです。
};

/**
 * @brief       NpadStyleFullKey に対応したコントローラーをエミュレーションするための、汎用ゲームパッドの割り当て設定を取得します。
 *
 * @details     FullKeyIndex で一意に特定されるコントローラーに割り当てられている汎用ゲームパッドは識別子 FullKeyGenericPadMap::genericPadId から識別することができます。
 *              識別子はプラットフォーム固有の値であり、プラットフォーム毎に規定されます。
 *              FullKeyGenericPadMap::buttonA の様な各デジタルボタンへの割り当ての指定には GenericPadButton で定義された値が使用されます。
 *              例として、FullKeyGenericPadMap::buttonA に GenericPadButton_1 が指定されていた場合、
 *              識別子が FullKeyGenericPadMap::genericPadId である汎用ゲームパッドのボタン 1 の押下状態が NpadStyleFullKey に対応したコントローラーのボタン A に対応付けられます。
 *              設定値として 0 が指定された場合は、そのデジタルボタンに対応する汎用ゲームパッドのボタンは無いものとして扱われます。
 *              FullKeyGenericPadMap::axisAnalogStickRX の様な各座標軸への割り当ての指定には GenericPadAxis で定義された値が使用されます。
 *              例として、 FullKeyGenericPadMap::axisAnalogStickRX に GenericPadAxis_X が指定されていた場合、
 *              識別子が FullKeyGenericPadMap::genericPadId である汎用ゲームパッドの x 軸の座標がエミュレーションする対象のコントローラーの右アナログスティックの x 軸に対応付けられます。
 *              設定値として 0 が指定された場合は、その座標軸に対応する汎用ゲームパッドの座標軸は無いものとして扱われます。
 *              座標値の増減方向が反転されているか否かは FullKeyGenericPadMap::attributes から判定することができます。
 *              例として、FullKeyGenericPadMap::attributes.Test<GenericPadAxisAttribute::IsXAxisInverted>() の値が真である場合、
 *              汎用ゲームパッドの x 軸は座標値の増減方向を反転して利用されています。
 *              FullKeyIndex で指定したコントローラーに対する汎用ゲームパッドの割り当てが行われていない状態で当該関数を呼び出した場合は、
 *              内部的に解決した割り当てを返します。
 *
 * @param[out]  pOutValue                   NpadStyleFullKey に対応したコントローラーに対する汎用ゲームパッドの割り当てを読み出すバッファ
 * @param[in]   index                       NpadStyleFullKey に対応したコントローラーを一意に特定するための FullKeyIndex
 *
 * @pre
 *              - pOutValue != nullptr
 */
void GetFullKeyGenericPadMap(FullKeyGenericPadMap* pOutValue, FullKeyIndex index
                          ) NN_NOEXCEPT;

/**
 * @brief       NpadStyleFullKey に対応したコントローラーに対する汎用ゲームパッドの割り当てを設定します。
 *
 * @details     エミュレーションする対象のコントローラーを FullKeyIndex に指定します。
 *              FullKeyGenericPadMap の各メンバの扱いについては GetFullKeyGenericPadMap() を参照してください。
 *              割り当ての設定は当該関数の呼び出し完了直後から反映されます。
 *              不正な割り当てが指定された場合、設定自体は行われますが、割り当て関係は無いものとして扱われます。
 *              設定の解除には ResetFullKeyGenericPadMap() を使用してください。
 *
 * @param[in]   value                       NpadStyleFullKey に対応したコントローラーに対する汎用ゲームパッドの割り当て
 * @param[in]   index                       NpadStyleFullKey に対応したコントローラーを一意に特定するための FullKeyIndex
 */
void SetFullKeyGenericPadMap(const FullKeyGenericPadMap& value, FullKeyIndex index
                          ) NN_NOEXCEPT;

/**
 * @brief       NpadStyleFullKey に対応したコントローラーに対する汎用ゲームパッドの割り当てをリセットします。
 *
 * @details     FullKeyIndex で指定しコントローラーに対して割り当てが設定されている場合はこれを削除し、
 *              内部的な割り当ての解決を有効に戻します。
 *              割り当てのリセットは当該関数の呼び出し完了直後から反映されます。
 *
 * @param[in]   index                       NpadStyleFullKey に対応したコントローラーを一意に特定するための FullKeyIndex
 */
void ResetFullKeyGenericPadMap(FullKeyIndex index) NN_NOEXCEPT;

/**
 * @brief       NpadStyleFullKey に対応したコントローラーに対するキーボードの割り当てを取得します。
 *
 * @details     FullKeyKeyboardMap::buttonA の様な各デジタルボタンへの割り当ての指定には ::nn::hid::KeyboardKey のビット位置が使用されます。
 *              例として、 FullKeyGenericPadMap::buttonA に ::nn::hid::KeyboardKey::A の Index が指定されていた場合、
 *              キーボードのキー A の押下状態が NpadStyleFullKey に対応したコントローラーのボタン A に対応付けられます。
 *              設定値として 0 が指定された場合は、そのデジタルボタンに対応するキーボードのキーは無いものとして扱われます。
 *              NpadStyleFullKey に対応したコントローラーに対するキーボードの割り当てが行われていない状態で当該関数を呼び出した場合は、
 *              全てのデジタルボタンについて割り当てが無いことを示す値を返します。
 *
 * @param[out]  pOutValue                   NpadStyleFullKey に対応したコントローラーに対するキーボードの割り当てを読み出すバッファ
 * @param[in]   index                       NpadStyleFullKey に対応したコントローラーを一意に特定するための FullKeyIndex
 *
 * @pre
 *              - pOutValue != nullptr
 */
void GetFullKeyKeyboardMap(FullKeyKeyboardMap* pOutValue, FullKeyIndex index
                        ) NN_NOEXCEPT;

/**
 * @brief       NpadStyleFullKey に対応したコントローラーに対するキーボードの割り当てを設定します。
 *
 * @details     FullKeyIndex で指定した NpadStyleFullKey に対応したコントローラーに対するキーボードの割り当てを設定します。
 *              NpadStyleFullKey に対応したコントローラーID の再解決後も割り当ての設定は維持されます。
 *              FullKeyKeyboardMap の各メンバの扱いについては GetFullKeyKeyboardMap() を参照してください。
 *              割り当ての設定は当該関数の呼び出し完了直後から反映されます。
 *              不正な割り当てが指定された場合、設定自体は行われますが、割り当て関係は無いものとして扱われます。
 *              設定の解除には ResetFullKeyKeyboardMap() を使用してください。
 *
 * @param[in]   value                       NpadStyleFullKey に対応したコントローラーに対するキーボードの割り当て
 * @param[in]   index                       NpadStyleFullKey に対応したコントローラーを一意に特定するための FullKeyIndex
 */
void SetFullKeyKeyboardMap(const FullKeyKeyboardMap& value, FullKeyIndex index
                        ) NN_NOEXCEPT;

/**
 * @brief       NpadStyleFullKey に対応したコントローラーに対するキーボードの割り当てをリセットします。
 *
 * @details     FullKeyIndex で指定した NpadStyleFullKey に対応したコントローラーに対して割り当てが設定されている場合はこれを削除します。
 *              割り当てのリセットは当該関数の呼び出し完了直後から反映されます。
 *
 * @param[in]   index                       NpadStyleFullKey に対応したコントローラーを一意に特定するための FullKeyIndex
 */
void ResetFullKeyKeyboardMap(FullKeyIndex index) NN_NOEXCEPT;

}} // namespace nn::settings
