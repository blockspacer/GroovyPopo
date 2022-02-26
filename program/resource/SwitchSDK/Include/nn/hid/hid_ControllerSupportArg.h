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
 * @brief   コントローラーサポートの UI 呼び出しの起動パラメータの宣言
 */

#pragma once

#include <nn/hid/hid_NpadCommonTypes.h>
#include <nn/util/util_MathTypes.h>

namespace nn { namespace hid {

#if defined (NN_BUILD_CONFIG_COMPILER_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic warning "-Wpadded"
#endif

/**
 * @brief   コントローラーサポートの UI で表示するコントローラー番号ごとの説明文の最大文字数です。
 */
const int32_t ExplainTextMaxLength = 32;
const int32_t Utf8ByteSize = 4;
const int32_t ExplainTextMaxBufferSize = ExplainTextMaxLength * Utf8ByteSize + 1;

/**
 * @brief コントローラーサポートの UI 呼び出しの起動パラメータです。
 */
struct ControllerSupportArg
{
    //! @brief 無線コントローラーの接続を受け付ける最少人数です。
    //!        1人用特殊モードでは無効です。
    int8_t playerCountMin;

    //! @brief 無線コントローラーの接続を受け付ける最大人数です。
    //!        1人用特殊モードでは無効です。
    int8_t playerCountMax;

    //! @brief 既に接続済みのコントローラーの接続を維持するかを指定します。
    //!        false を指定すると全てのコントローラーが切断されます。
    bool enableTakeOverConnection;

    //! @brief コントローラーサポートの終了時にコントローラー番号を前詰めするかを指定します。
    //!        false を指定するとコントローラー番号に空きがある状態で終了する場合があります。
    //!        1人用特殊モードでは無効です。
    bool enableLeftJustify;

    //! @brief 2本持ちのコントローラーがある状態での決定を許可するかを指定します。
    //!        false を指定すると2本持ちのコントローラーがある状態で決定できなくなります。
    //!        ローカル通信中など、2本持ちを禁止したい場合に使用することを想定しています。
    bool enablePermitJoyDual;

    //! @brief コントローラーサポートを1人用特殊モードで起動するかを指定します。
    //!        1人用特殊モードを有効にした場合、本体装着状態のコントローラーの接続も受け付けます。
    bool enableSingleMode;

    //! @brief コントローラーサポートの UI で表示するコントローラー番号ごとの識別用の色を使用するか指定します。
    bool enableIdentificationColor;

    //! @brief コントローラーサポートの UI で表示するコントローラー番号ごとの識別用の色を指定します。
    //!        値を指定していても、enableIdentificationColor が false の場合は反映されません。
    nn::util::Color4u8Type identificationColor[ 4 ];

    //! @brief コントローラーサポートの UI で表示するコントローラー番号ごとの文字列を使用するか指定します。
    bool enableExplainText;

    //! @brief コントローラーサポートの UI で表示するコントローラー番号ごとの文字列です。
    //!        最大 nn::hid::ExplainTextMaxLength 文字が指定可能です。
    //!        値を指定していても、enableExplainText が false の場合は反映されません。
    //!        文字列の幅などが適切に表示されているかは実際の表示を見て確認してください。
    char explainText[ 4 ][ ExplainTextMaxBufferSize ];

    /**
     * @brief   コントローラーサポートの UI 呼び出しの起動パラメータにデフォルト値を設定します。
     *
     * @details 下記のデフォルト値が設定されます。
     *          - playerCountMin              : 0
     *          - playerCountMax              : 4
     *          - enableTakeOverConnection    : true
     *          - enableLeftJustify           : true
     *          - enablePermitJoyDual         : true
     *          - enableSingleMode            : false
     *          - enableIdentificationColor   : false
     *          - enableExplainText           : false
     */
    void SetDefault()
    {
        this->playerCountMin                = 0;
        this->playerCountMax                = 4;
        this->enableTakeOverConnection      = true;
        this->enableLeftJustify             = true;
        this->enablePermitJoyDual           = true;
        this->enableSingleMode              = false;
        this->enableIdentificationColor     = false;
        this->enableExplainText             = false;
    }
};

/**
* @brief コントローラーのファームウェア更新の UI 呼び出しの起動パラメータです。
*/
struct ControllerFirmwareUpdateArg
{
    //! @brief 強制更新するかを指定します。
    //!        true を指定すると、ユーザーに更新するかどうか確認しません。
    //!        デフォルトは false です。
    bool enableForceUpdate;

    uint8_t _padding0[ 3 ];

    /**
    * @brief   コントローラーのファームウェア更新の UI 呼び出しの起動パラメータにデフォルト値を設定します。
    *
    * @details 下記のデフォルト値が設定されます。
    *          - enableForceUpdate           : false
    */
    void SetDefault()
    {
        this->enableForceUpdate = false;
    }
};

/**
 * @brief コントローラーサポートの UI 呼び出しの結果を格納する構造体です。
 */
struct ControllerSupportResultInfo
{
    //! @brief コントローラーサポートで確定したプレイヤーの人数です。
    //!        コントローラーサポートがキャンセルされた場合は 0 を返します。
    //!        1人用特殊モードでは無効です。
    int8_t playerCount;

    uint8_t _padding0[ 3 ];

    //! @brief 1人用特殊モードで選ばれた NpadIdType です。
    //!        キャンセルされた場合は、キャンセル操作をした NpadIdType を返します。
    //!        1人用特殊モードの場合のみ有効です。
    nn::hid::NpadIdType selectedId;
};

#if defined (NN_BUILD_CONFIG_COMPILER_CLANG)
#pragma clang diagnostic pop
#endif

}} // namespace nn::hid

