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

#include <nn/nn_Common.h>
#include <nn/account/account_Types.h>

namespace nn { namespace account {

/**
    @brief Open状態のユーザーアカウントを、状態を維持したまま異なるプログラム間で利用可能にします。

    @pre
        - 呼び出し元がマルチプログラムアプリケーションを構成するプログラムである。
    @post
        - PopOpenUsers() で、現在Open状態のユーザーの UserHandle を再取得できる。
        - 現在Open状態のユーザーアカウントの、取得済みの UserHandle が利用不能になる。

    @details
        マルチプログラムアプリケーションを構成するプログラムであれば、Open状態の全ユーザーアカウントを、状態を維持したまま異なるプログラム間で利用可能にします。
        プログラム間でOpen状態を維持しているユーザーアカウントの UserHandle は、 PopOpenUsers() を使用して取得できます。

        本関数を複数回呼ぶと、すでに状態を維持しているものを含め、呼び出し時点でOpen状態の全ユーザーアカウントの状態が維持されます。
*/
void PushOpenUsers() NN_NOEXCEPT;

/**
    @brief プログラム間でOpen状態を維持しているユーザーアカウントの UserHandle を取得します。

    @param[out] pOut 取得した UserHandle の個数を格納する領域
    @param[out] outHandles 取得した UserHandle を格納する領域
    @param[in] outHandleCount outHandles の要素数

    @pre
        - 呼び出し元がマルチプログラムアプリケーションを構成するプログラムである。
        - outHandleCount >= "プログラム間でOpen状態を維持しているユーザーアカウント数"
    @post
        - *pOut <= UserCountMax
        - outHandles[0], ..., outHandles[*pOut - 1] が有効な UserHandle を保持している。
        - PopOpenUsers() で、現在Open状態のユーザーの UserHandle を再取得できない。

    @details
        マルチプログラムアプリケーションを構成するプログラムであれば、 プログラム間でOpen状態を維持している全ユーザーアカウントの UserHandle を取得します。
        どのプログラムでも PushOpenUsers() が呼ばれていなければ UserHandle を取得できませんが、エラーとはなりません。

        本関数は、事前に PushOpenUsers() を呼んだ回数に関わらず、その時点でOpen状態を維持されている全ユーザーアカウントの UserHandle を取得します。
*/
void PopOpenUsers(int* pOut, UserHandle outHandles[], int outHandleCount) NN_NOEXCEPT;

}}  // ~namespace nn::account
