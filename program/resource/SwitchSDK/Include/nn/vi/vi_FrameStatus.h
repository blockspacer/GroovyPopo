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
 * @brief Defines for frame status queries.
 */

#pragma once

#include <cstdint>

namespace nn { namespace vi {

    /**
     * @brief States for frames to be presented on the display.
     *
     * @details Once the application is finished programming rendering commands and
     *          invokes a presentation API, then the frame is enqueued for use on the
     *          display. When rendering is complete, then it will be presented to the display.
     */
    enum FrameStatus : std::uint64_t
    {
        FrameStatus_Unknown,   //!< The current status of the frame is unknown.
        FrameStatus_Enqueued,  //!< The frame will be presented to the display after rendering is complete.
        FrameStatus_Presented, //!< The frame has been shown on the display.
    };
}}

