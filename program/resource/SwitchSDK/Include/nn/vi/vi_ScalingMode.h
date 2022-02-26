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
 * @brief Defines all scaling modes that may be supported by the display server.
 */

#pragma once

namespace nn { namespace vi {

    /**
     * @brief   Scaling modes for use with the display server.
     * @details Not all platforms will support every scaling mode.
     */
    enum ScalingMode
    {
        ScalingMode_None,                //!< All content outside the layer is cropped.
        ScalingMode_Exact,               //!< Only content that matches the layer dimensions will be used for updates.
        ScalingMode_FitToLayer,          //!< Content is scaled to match the layer dimensions.
        ScalingMode_ScaleAndCrop,        //!< Content is scaled to the layer's smaller dimension then cropped.
        ScalingMode_PreserveAspectRatio, //!< Content is scaled while preserving the aspect ratio.
                                         //!< A pillarbox or letterbox may be added as appropriate.
    };

}}
