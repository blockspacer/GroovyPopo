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

#include <cstdarg>
#include <nn/nn_Common.h>
#include <nn/util/util_FormatString.h>

namespace nn { namespace util {

void TFormatString(
    FormatStringOutputFunction pOutputFunction,
    uintptr_t outputFunctionArg,
    const char* pFormat,
    ...) NN_NOEXCEPT;

void TVFormatString(
    FormatStringOutputFunction pOutputFunction,
    uintptr_t outputFunctionArg,
    const char* pFormat,
    std::va_list va) NN_NOEXCEPT;

int TSNPrintf(char* buffer, size_t bufferLength, const char* pFormat, ...) NN_NOEXCEPT;
int TVSNPrintf(char* buffer, size_t bufferLength, const char* pFormat, std::va_list formatArg) NN_NOEXCEPT;

}}
