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

#include <nn/nn_Macro.h>

/**
* @namespace nn::diag
* @brief プログラム診断用ライブラリの名前空間です。
*/

#if !defined(NN_BUILD_CONFIG_SPEC_CAFE)
#include <nn/diag/diag_AbortObserver.h>
#include <nn/diag/diag_Backtrace.h>
#include <nn/diag/diag_Break.h>
#include <nn/diag/diag_Debugger.h>
#include <nn/diag/diag_Symbol.h>
#include <nn/diag/diag_Module.h>
#endif
#include <nn/diag/diag_AbortTypes.h>
#include <nn/diag/diag_AssertionFailureHandler.h>
#include <nn/diag/diag_AssertionTypes.h>
#include <nn/diag/diag_Config.h>
#include <nn/diag/diag_LogObserver.h>
#include <nn/diag/diag_LogTypes.h>
