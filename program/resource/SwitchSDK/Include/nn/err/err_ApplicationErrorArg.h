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
 * @brief アプリケーション固有のエラーを表示するためのクラスの定義。
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/err/err_Types.h>
#include <nn/settings/settings_Language.h>
#include <nn/util/util_TypedStorage.h>

namespace nn { namespace err {

struct ErrorViewerStartupParamForApplicationError;

/**
* @brief    アプリケーション固有のエラーを表示するための情報を構築するクラスです。
* @details  必要な情報を設定した後、@ref ShowApplicationError に渡すことでエラーを表示することができます。@n
*           本クラスのデータサイズは @ref ApplicationErrorArg::DataSize バイトと大きいため、
*           スタックに配置する場合にはそのスレッドのスタックサイズにご注意ください。
*/
class ApplicationErrorArg
{
public:
    /**
    * @brief        情報を格納するために使用するデータのサイズ（バイト）です。
    */
    static const size_t DataSize = 4116;

    /**
    * @brief        NULL終端を含めたメッセージの最大バイト数です。
    */
    static const size_t MessageLengthMax = 2048;

    /**
    * @brief        有効な情報を持たない状態でクラスを作成します。
    * @details      無効な ApplicationErrorCodeNumber を持ち、ダイアログ表示用のメッセージと全画面表示用のメッセージの両方に
    *               空文字列が設定された状態（それぞれの表示をサポートしない状態）でクラスを作成します。
    */
    ApplicationErrorArg() NN_NOEXCEPT;

    /**
    * @brief        エラーの情報を設定してクラスを作成します。
    * @param[in]    number エラーコード番号。
    * @param[in]    dialogViewMessage ダイアログ表示用のメッセージ（UTF-8）。最大長は NULL終端を含めて MessageLengthMax です。
    * @param[in]    fullScreenViewMessage 全画面表示用のメッセージ（UTF-8）。最大長は NULL終端を含めて MessageLengthMax です。
    * @param[in]    languageCode エラー表示時のボタン等の UI 部品に使用する言語。
    * @pre
    *               - number <= 9999
    *               - dialogViewMessage != nullptr
    *               - fullScreenViewMessage != nullptr
    *               - languageCode が言語設定として利用可能な言語コード（@ref settings::GetAvailableLanguageCodes で取得可能）に含まれている。
    * @details      ダイアログ表示を行わないエラーの場合、空の文字列を設定します。
    *               同様に、全画面表示を行わないエラーの場合、空の文字列を設定します。@n
    *               メッセージはこのクラスのデータ領域にコピーされるので、入力に使用したバッファを呼び出し側で保持し続けておく必要はありません。
    */
    ApplicationErrorArg(ApplicationErrorCodeNumber number, const char* dialogViewMessage, const char* fullScreenViewMessage, const nn::settings::LanguageCode& languageCode) NN_NOEXCEPT;

    /**
    * @brief        エラーコード番号を設定します。
    * @param[in]    number エラーコード番号。
    * @pre
    *               - number <= 9999
    */
    void SetApplicationErrorCodeNumber(ApplicationErrorCodeNumber number) NN_NOEXCEPT;

    /**
    * @brief        ダイアログ表示用のメッセージを設定します。
    * @param[in]    message メッセージ（UTF-8）。最大長は NULL終端を含めて MessageLengthMax です。
    * @pre
    *               - message != nullptr
    * @details      ダイアログ表示を行わない場合、空文字列を設定します。@n
    *               メッセージはこのクラスのデータ領域にコピーされるので、入力に使用したバッファを呼び出し側で保持し続けておく必要はありません。
    */
    void SetDialogMessage(const char* message) NN_NOEXCEPT;

    /**
    * @brief        全画面表示用のメッセージを設定します。
    * @param[in]    message メッセージ（UTF-8）。最大長は NULL終端を含めて MessageLengthMax です。
    * @pre
    *               - message != nullptr
    * @details      全画面表示を行わない場合、空文字列を設定します。@n
    *               メッセージはこのクラスのデータ領域にコピーされるので、入力に使用したバッファを呼び出し側で保持し続けておく必要はありません。
    */
    void SetFullScreenMessage(const char* message) NN_NOEXCEPT;

    /**
    * @brief        エラー表示時のボタン等の UI 部品に表示するメッセージに使用する言語を設定します。
    * @param[in]    languageCode エラー表示時のボタン等の UI 部品上に表示するメッセージに使用する言語。
    * @pre
    *               - languageCode が言語設定として利用可能な言語コード（@ref settings::GetAvailableLanguageCodes で取得可能）に含まれている。
    * @details      ここで設定された言語は「エラー表示をとじるためのボタン」や
    *               「エラー表示をダイアログ表示から全画面表示に切り替えるためのボタン」（ダイアログ表示と全画面表示の両方をサポートする場合）
    *               に表示されるメッセージなどに使用されます。
    */
    void SetLanguageCode(const nn::settings::LanguageCode& languageCode) NN_NOEXCEPT;

    /**
    * @brief        設定されたエラーコード番号を返します。
    * @return       エラーコード番号。
    */
    ApplicationErrorCodeNumber GetApplicationErrorCodeNumber() const NN_NOEXCEPT;

    /**
    * @brief        設定されたダイアログ表示用のメッセージ（UTF-8）を返します。メッセージは NULL終端されています。
    * @return       ダイアログ表示用のメッセージ。
    */
    const char* GetDialogMessage() const NN_NOEXCEPT;

    /**
    * @brief        設定された全画面表示用のメッセージ（UTF-8）を返します。メッセージは NULL終端されています。
    * @return       全画面表示用のメッセージ。
    */
    const char* GetFullScreenMessage() const NN_NOEXCEPT;

    /**
    * @brief        設定されたエラー表示時のボタン等の UI 部品に使用する言語を返します。
    * @return       エラー表示時のボタン等の UI 部品に使用する言語。
    */
    nn::settings::LanguageCode GetLanguageCode() const NN_NOEXCEPT;
private:
    util::TypedStorage<ErrorViewerStartupParamForApplicationError, DataSize, 4> m_Data;
};

}} // namespace nn::err
