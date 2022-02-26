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

/**
* @file
* @brief   ログ出力APIの実装に利用する関数、マクロの定義
*/

#include <nn/nn_Common.h>
#include <nn/diag/diag_LogTypes.h>

namespace nn { namespace diag {

namespace detail {

    // ログオブザーバをすべて呼び出す
    void LogImpl(const LogMetaData& logMetaData, const char* format, ...) NN_NOEXCEPT;
    void VLogImpl(const LogMetaData& logMetaData, const char* format, std::va_list formatArg) NN_NOEXCEPT;
    void PutImpl(const LogMetaData& logMetaData, const char* text, size_t length) NN_NOEXCEPT;

}

}}

