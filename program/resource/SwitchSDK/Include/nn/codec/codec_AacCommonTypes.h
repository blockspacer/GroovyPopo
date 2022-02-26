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

namespace nn { namespace codec {

/**
 * @name        Aac
 * @{
 */

/**
 * @brief       Mpeg-4 Audio Object Type です。
 */
enum Mpeg4AudioObjectType
{
    Mpeg4AudioObjectType_Invalid = -1,  //!< Invalid
    Mpeg4AudioObjectType_AacLc = 2,     //!< AAC Low Complexity
};

/**
 * @}
 */

}}  // namespace nn::codec
