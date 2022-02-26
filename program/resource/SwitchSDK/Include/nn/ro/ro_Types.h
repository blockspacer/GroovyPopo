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
 * @brief   RO ライブラリで使用する共通の型や定数の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/util/util_TypedStorage.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn { namespace ro {

namespace detail {
class RoModule;
} // namespace detail

//--------------------------------------------------------------------------
/**
 * @brief   RO オブジェクトのシンボル解決に関するフラグを表す列挙型です。
 */
enum BindFlag
{
    BindFlag_Now    = 1 << 0, //!< 即時解決を行います。
    BindFlag_Lazy   = 1 << 1, //!< 遅延解決を行います。
};

//--------------------------------------------------------------------------
/**
 * @brief   RO オブジェクトを定義する構造体です。
 */
struct Module
{
    //----------------------------------------------------------------------
    //  RO オブジェクトの状態を表す列挙型です。
    enum State
    {
        State_Unloaded,
        State_Loaded,
    };

    // メンバ変数
    detail::RoModule*   _module;
    State               _state;
    uintptr_t           _fileAddress;
    uintptr_t           _bufferAddress;
};

//--------------------------------------------------------------------------
/**
 * @brief   登録オブジェクトを定義する構造体です。
 */
struct RegistrationInfo
{
    //----------------------------------------------------------------------
    //  登録 オブジェクトの状態を表す列挙型です。
    enum State
    {
        State_Unregistered,
        State_Registered,
    };

    // メンバ変数
    State               _state;
    uintptr_t           _fileAddress;
    util::IntrusiveListNode _listNode;
};


//--------------------------------------------------------------------------

}} // namespace nn::ro
