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
 * @brief OpenGL ローダーライブラリの API を宣言します。
 */

#pragma once

 /**
 * @namespace nn::gll
 * @brief OpenGL ローダーライブラリの名前空間です。
 */

#include <nn/nn_Macro.h>

#include <nn/gll/gll_Gl.h>
#if defined( NN_BUILD_CONFIG_OS_WIN32 )
    #include <nn/gll/gll_Wgl.h>
#endif
