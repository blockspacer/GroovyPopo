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
 * @brief   ファイバに関する型や定数の宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/os/os_Macro.h>

#include <nn/nn_Common.h>
#include <nn/util/util_TypedStorage.h>
#include <nn/os/os_FiberCommon.h>
#include <nn/os/detail/os_MacroImpl.h>

#if defined(NN_BUILD_CONFIG_OS_WIN32)
    #include <nn/os/detail/os_FiberTypesImpl-os.win32.h>
#elif defined(NN_BUILD_CONFIG_OS_HORIZON)
    #include <nn/os/detail/os_FiberTypesImpl-os.horizon.h>
#else
    #error   "未サポートの OS 種別が選択されています"
#endif


namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   ファイバに付随するフラグを表す列挙型です。
 */
enum FiberFlag
{
    FiberFlag_NoStackGuard      = 0x1,  //!< スタックガードを設置しません
};

//--------------------------------------------------------------------------
/**
 * @brief   ファイバオブジェクトを定義する構造体です。
 *
 * @details
 *  ファイバオブジェクトは以下の内部状態を持ちます。
 *
 *  - NotInitialized 状態： 明確な未初期化状態（ nn::os::FinalizeFiber() 直後など）
 *  - Ready          状態： ファイバ関数が実行可能な状態（初期化直後もしくは停止中）
 *  - Running        状態： ファイバ関数がまさに実行中の状態
 *  - Completed      状態： ファイバ関数の実行を完了し、関数からリターンした状態
 *
 * メンバー変数を直接変更することは禁止です。 @n
 * 次の関数を介して、本構造体を利用してください。
 *
 * - @ref InitializeFiber()
 * - @ref FinalizeFiber()
 * - @ref SwitchToFiber()
 * - @ref GetCurrentFiber()
 *
 * 上記の関数を利用するには nn/os.h をインクルードする必要があります。
 */
struct  FiberType
{
    enum State
    {
        State_NotInitialized    = 0,
        State_Ready             = 1,
        State_Running           = 2,
        State_Completed         = 4,
    };
/*
    NN_OS_DETAIL_PADDING_POINTER;
    NN_OS_DETAIL_PADDING_POINTER;
    NN_OS_DETAIL_PADDING_POINTER;
    NN_OS_DETAIL_PADDING_POINTER;
*/
    uint8_t             _state;
    bool                _stackIsAliased;

    FiberFunction       _function;
    void*               _argument;

    void*               _originalStack;
    void*               _stack;
    size_t              _stackSize;

    // 環境依存
    detail::FiberTypeImpl   _impl;
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(FiberType);

}} // namespace nn::os

