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

/*!
  @namespace  nn::font
  @brief      フォントライブラリの名前空間です。

  @attention  本名前空間の API を利用するには、
              NintendoSDK NintendoWare パッケージをインストールする必要があります。
*/

#include <nn/nn_Common.h>

#include <nn/font/font_CharStrmReader.h>
#include <nn/font/font_CharWriter.h>
#include <nn/font/font_Font.h>
#include <nn/font/font_GlyphTreeMap.h>
#include <nn/font/font_PairFont.h>
#include <nn/font/font_RectDrawer.h>
#include <nn/font/font_ResFont.h>
#include <nn/font/font_ScalableFont.h>
#include <nn/font/font_ScalableFontUtil.h>
#include <nn/font/font_TagProcessor.h>
#include <nn/font/font_TextureCache.h>
#include <nn/font/font_TextWriter.h>
#include <nn/font/font_WideTagProcessor.h>
#include <nn/font/font_WideTextWriter.h>
#include <nn/font/font_WordWrapping.h>
