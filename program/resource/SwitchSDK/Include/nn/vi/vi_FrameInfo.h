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
 * @brief Defines for tracking frame status.
 */

#include <cstdint>
#include <nn/vi/vi_FrameStatus.h>
#include <nn/os/os_TickTypes.h>

namespace nn { namespace vi {

    /**
     * @brief Contains frame status information.
     *
     * @details Applications should check the status member to determine
     *          what fields are available:
     *
     *          - FrameStatus_Unknown:   No fields are valid.
     *          - FrameStatus_Enqueued:  frameNumber and enqueueTime are valid.
     *          - FrameStatus_Presented: All fields are valid.
     *
     *          Values of unavailable fields are undefined.
     */
    struct FrameInfo
    {
        FrameStatus status;            //!< The frame's current status.
        std::uint64_t frameNumber;     //!< A frame's unique identifier that is generated at queue time.
                                       //!< This is a monotonic increasing value.
        nn::os::Tick enqueueTime;      //!< Timestamp when the frame was queued by the application.
        nn::os::Tick presentationTime; //!< Timestamp when the frame was presented on the display.
        nn::os::Tick vblankTime;       //!< Timestamp from the beginning of a vblank period. It is the closest
                                       //!< vblank that is less than or equal to the presentation time.
    };
}}

