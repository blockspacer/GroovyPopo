/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

#include <nn/nn_Result.h>
#include <nn/err/err_Types.h>

namespace nn { namespace err {

/**
* @brief    ErrorResultVariant の状態を表す列挙子です。
*/
enum ErrorResultVariantState
{
    ErrorResultVariantState_HasNone,        //!< リザルト、エラーコードが格納されていません。
    ErrorResultVariantState_HasResult,      //!< リザルトが格納されています。
    ErrorResultVariantState_HasErrorCode,   //!< エラーコードが格納されています。
};

/**
* @brief    エラー表示に利用することができる nn::Result と nn::err::ErrorCode を排他的に格納可能なクラスです。
*           nn::err::ShowError 関数の引数に利用することができます。
*/
class ErrorResultVariant {
public:
    /**
    * @brief        有効なリザルト、エラーコードを持たない状態でインスタンスを作成します。
    * @post
    *               - GetState() == ErrorResultVariant_HasNone
    */
    ErrorResultVariant() NN_NOEXCEPT;

    /**
    * @brief        リザルトを保持した状態でインスタンスを作成します。
    * @param[in]    result リザルト。
    * @post
    *               - GetState() == ErrorResultVariant_HasResult
    */
    NN_IMPLICIT ErrorResultVariant(const Result& result) NN_NOEXCEPT;

    /**
    * @brief        エラーコードを保持した状態でインスタンスを作成します。
    * @param[in]    errorCode エラーコード。
    * @post
    *               - GetState() == ErrorResultVariant_HasErrorCode
    */
    NN_IMPLICIT ErrorResultVariant(const ErrorCode& errorCode) NN_NOEXCEPT;

    /**
    * @brief        リザルトとの代入演算子です。
    * @param[in]    result リザルト。
    * @return       自身への参照。
    * @post
    *               - GetState() == ErrorResultVariant_HasResult
    */
    ErrorResultVariant& operator=(const Result& result) NN_NOEXCEPT;

    /**
    * @brief        エラーコードとの代入演算子です。
    * @param[in]    errorCode エラーコード。
    * @return       自身への参照。
    * @post
    *               - GetState() == ErrorResultVariant_HasErrorCode
    */
    ErrorResultVariant& operator=(const ErrorCode& errorCode) NN_NOEXCEPT;

    /**
    * @brief        リザルト、エラーコードの格納状態を取得します。
    * @return       状態。
    */
    ErrorResultVariantState GetState() const NN_NOEXCEPT;

    /**
    * @brief        格納されたリザルトを取得します。
    * @return       リザルト。
    * @pre
    *               - GetState() == ErrorResultVariantState_HasResult
    */
    NN_IMPLICIT operator Result() const NN_NOEXCEPT;

    /**
    * @brief        格納されたエラーコードを取得します。
    * @return       エラーコード。
    * @pre
    *               - GetState() == ErrorResultVariantState_HasErrorCode
    */
    NN_IMPLICIT operator ErrorCode() const NN_NOEXCEPT;

private:
    ErrorResultVariantState  m_State;
    union
    {
        ErrorCode   errorCode;
        uint32_t    resultValue;
    } m_Data;
};

}} // nn::err
