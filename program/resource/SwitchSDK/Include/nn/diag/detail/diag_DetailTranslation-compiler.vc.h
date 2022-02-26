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

namespace nn { namespace diag { namespace detail {

/**
*   @brief      与えられたワイド文字列を UTF8 マルチバイト文字列に変換します。
*
*   @param[in]  pWideString ワイド文字列
*   @return                 変換後の UTF8 マルチバイト文字列
*/
const char* GetSdkMultiByteStringFromWideString(const wchar_t* pWideString) NN_NOEXCEPT;

}}}
