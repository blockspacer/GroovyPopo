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

#include <nn/diag/detail/diag_DetailTranslation.h>


/*
* @file
* @brief    アサート・ログメッセージ翻訳用の辞書にアクセスする API を宣言します。
*           このファイルは自動生成されたものなので、変更しないでください。
*/

namespace nn {
namespace diag {
namespace detail {
    /*
    * アサート・ログメッセージ翻訳機能の内部実装で使うメソッドです。
    * 翻訳機能の利用者は、このメソッドを直接呼び出さないでください。
    */
    const char* nnText_g3dviewer(const char* str);
}
}
}


/*
* @brief    アサート・ログメッセージ用の文字列を翻訳します。
*
* @param[in]    string  翻訳前の文字列リテラル
* @return               翻訳後の UTF-8 文字列
*/
#define NN_TEXT_G3DVIEWER(string) NN_DETAIL_DIAG_SDK_TEXT(nn::diag::detail::nnText_g3dviewer, string)
