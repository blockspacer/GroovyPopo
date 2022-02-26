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

#define NN_ATK_FND_IS_LITTLE_ENDIAN    //!< リトルエンディアン

// エンディアン変換の有無
#if defined(NN_ATK_FND_TARGET_IS_BIG_ENDIAN)
#define NN_ATK_FND_SHOULD_REVERSE_ENDIAN    //!< エンディアン変換が必要
#endif
