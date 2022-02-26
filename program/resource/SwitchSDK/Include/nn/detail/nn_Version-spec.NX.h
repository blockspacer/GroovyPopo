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

#define NN_NX_ADDON_VERSION_MAJOR          7
#define NN_NX_ADDON_VERSION_MINOR          4
#define NN_NX_ADDON_VERSION_MICRO          0
#define NN_NX_ADDON_VERSION_RELSTEP        0

#ifndef NN_NX_ADDON_VERSION_NUMBER
#define NN_NX_ADDON_VERSION_NUMBER(major, minor, micro, relstep) \
    (((major) << 24) | ((minor) << 16)  | ((micro) << 8) | (relstep << 0))

#define NN_NX_ADDON_CURRENT_VERSION_NUMBER \
    NN_NX_ADDON_VERSION_NUMBER(NN_NX_ADDON_VERSION_MAJOR, NN_NX_ADDON_VERSION_MINOR, NN_NX_ADDON_VERSION_MICRO, NN_NX_ADDON_VERSION_RELSTEP)
#endif
