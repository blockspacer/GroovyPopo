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
    @file
    @brief マルチプログラムアプリケーションに関連する API 宣言
*/

#pragma once

#include <nn/nn_Common.h>

namespace nn { namespace oe {

//! @name マルチプログラムアプリケーションに関連する API
//! @{

/**
    @brief 現在のプログラムを終了し、指定したプログラムを起動します。

    @param[in] programIndex 移行対象のプログラムのインデックスを指定します。
    @param[in] data 受け渡すデータへのポインタ
    @param[in] dataSize 受け渡すデータのサイズ

    @pre 0 <= programIndex && programIndex <= 15
    @pre dataSize <= 4096
    @pre dataSize == 0 || data != nullptr
    @pre dataSize > 0 のとき、 data を先頭とするサイズが dataSize の領域を読み込み可能である

    @details
     マルチプログラムアプリケーションにおいて、
     現在のプログラムを終了し programIndex で指定されたプログラムを起動します。この関数から返ることはありません。

     programIndex で指定されたプログラムが存在しなかった場合にも、本関数の呼び出しから返ることはありませんが、
     次のプログラムの起動に失敗しアプリケーションが異常終了します。

     data と dataSize を指定することで、指定した領域に書かれていたデータ(最大 4096 バイト)を起動するプログラムに渡すことができ、
     起動されたプログラムでは TryPopLaunchParameter() を使用してデータを参照することができます。
     data に nullptr を指定した場合は TryPopLaunchParameter() は false を返します。

     起動されたプログラムからは GetPreviousProgramIndex() によって、起動元のプログラムのプログラムインデックスを取得することができます。

     詳しくは @confluencelink{309702322,マルチプログラムアプリケーションに関する詳細} を参照してください。
*/
NN_NORETURN void ExecuteProgram(int programIndex, const void* data, size_t dataSize) NN_NOEXCEPT;

/**
    @brief 直前に走っていたプログラムのインデックスを取得します。

    @return 直前に走っていたプログラムのインデックスが返ります。

    @details
     ExecuteProgram() 関数で起動されたプログラムで本関数を呼び出すと、移行前のプログラムのインデックスが返ります。
     本関数を呼び出したプログラムが、アプリケーションで最初に起動したプログラムである場合、-1 が返ります。
*/
int GetPreviousProgramIndex() NN_NOEXCEPT;

//! @}

}}
