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
 * @brief   ユーザ例外ハンドラに関する型の宣言
 */

#pragma once

#include <nn/os/os_Config.h>
#include <nn/nn_Common.h>

#if defined(NN_BUILD_CONFIG_OS_WIN32)
    #include "detail/os_UserExceptionHandlerTypes-os.win32.h"
#elif defined(NN_BUILD_CONFIG_OS_HORIZON)
    #include "detail/os_UserExceptionHandlerTypes-os.horizon.h"
#endif


namespace nn { namespace os {

//-----------------------------------------------------------------------------
/**
 * @brief   例外種別を定義した列挙体です。
 */
enum UserExceptionType
{
    UserExceptionType_None                       = 0x0000,
    UserExceptionType_InvalidInstructionAccess   = 0x0100, //!< 不正なメモリ領域への命令アクセスを表します
    UserExceptionType_InvalidDataAccess          = 0x0101, //!< 不正なメモリ領域へのデータアクセスを表します
    UserExceptionType_UnalignedInstructionAccess = 0x0102, //!< 不正なアライメントへの命令アクセスを表します
    UserExceptionType_UnalignedDataAccess        = 0x0103, //!< 不正なアライメントへのデータアクセスを表します
    UserExceptionType_UndefinedInstruction       = 0x0104, //!< 未定義命令の実行を表します
    UserExceptionType_ExceptionalInstruction     = 0x0105, //!< 例外命令の実行を表します
    UserExceptionType_MemorySystemError          = 0x0106, //!< メモリシステムでのエラーを表します
    UserExceptionType_FloatingPointException     = 0x0200, //!< 浮動小数点数演算例外を表します
    UserExceptionType_InvalidSystemCall          = 0x0301, //!< 無効なシステムコール呼び出しを表します
};


#if defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
//-----------------------------------------------------------------------------
/**
 * @brief   例外情報を格納する構造体です。
 *
 * @details
 *  この構造体は各ターゲット環境に合わせて、以下の表に示すように別途定義
 *  されている別名の構造体から typedef されています。@n
 *  詳細はリンク先にある各構造体の定義を参照して下さい。
 *
 *  |ターゲット環境個別の構造体名|説明|
 *  |:-----:|:----|
 *  |nn::os::UserExceptionInfoDetailAarch32Status32|ARMv7A 動作環境の場合|
 *  |nn::os::UserExceptionInfoDetailAarch32Status64|ARMv8A 上で AArch32 動作環境の場合|
 *  |nn::os::UserExceptionInfoDetailAarch64|ARMv8A 上で AArch64 動作環境の場合|
 *  |nn::os::UserExceptionInfoDetailWin32|Windows 環境の場合|
 *
 */
struct UserExceptionInfoDetail
{
};
#endif


//-----------------------------------------------------------------------------
/**
 * @brief   例外情報を格納する構造体です。
 *
 * @details
 *  内部メンバである nn::os::UserExceptionInfoDetail 構造体は、動作ターゲット環境
 *  によって定義が異なりますので注意して下さい。
 */
struct UserExceptionInfo
{
    Bit32                   exceptionType;  //!< 例外種別が格納されます。
    UserExceptionInfoDetail detail;         //!< 例外情報の詳細が格納されます。
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(UserExceptionInfo);


//-----------------------------------------------------------------------------
/**
 * @brief   ユーザ例外ハンドラの関数を表す型です。
 *
 * @details
 *  例外ハンドラの関数を表す型です。
 *  info には発生した例外の情報が渡されます。
 *
 *  例外ハンドラから return した場合はシステムは panic で停止します。@n
 *  例外ハンドラの詳細は SetUserExceptionHandler() を参照してください
 */
typedef void (*UserExceptionHandler)(UserExceptionInfo* info);


//-----------------------------------------------------------------------------

}} // namespace nn::os

