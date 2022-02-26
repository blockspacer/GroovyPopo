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
 * @brief gll API の各機能で共通に使われる定義
 */

#pragma once

#include <stddef.h>
#include <nn/nn_Macro.h>

#if defined( NN_BUILD_CONFIG_OS_WIN32 )
    #if defined( NN_GLL_CONFIG_DLL )
        #if defined( NN_GLL_CONFIG_DLL_EXPORTS )
            #define NN_GLL_API extern __declspec( dllexport )
        #else
            #define NN_GLL_API extern __declspec( dllimport )
        #endif
    #else
        #define NN_GLL_API extern
    #endif
#else
    #define NN_GLL_API extern
#endif

/**
* @brief 指定された機能または拡張がサポートされているかどうかを取得します。
*/
#define NN_GLL_IS_SUPPORTED( x ) ( nngllIsSupported_ ## x () != 0 )

/**
* @brief 結果コードです。
*/
enum nngllResult
{
    nngllResult_Succeeded, //!< 成功しました。
    nngllResult_Failed, //!< 失敗しました。
    nngllResult_InvalidGlVersion, //!< GL のバージョンが無効です。
    nngllResult_FailedToGetGlVersion, //!< GL のバージョンの取得に失敗しました。
    nngllResult_FailedToGetGlExtensions //!< GL の拡張の取得に失敗しました。
};
