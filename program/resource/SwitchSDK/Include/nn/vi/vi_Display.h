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
 * @brief   Display enumeration for the VI library.
 * @details These functions can also be used to query the capabilities of the
 *          available displays.
 */

#pragma once

#include <nn/nn_Result.h>
#include <nn/os/os_SystemEventTypes.h>
#include <nn/vi/vi_Types.h>

namespace nn { namespace vi {

    /**
     * @brief List all available displays.
     *
     * @param[out] pOutDisplays Buffer to write display information.
     * @param[in]  count        The maximum number of DisplayInfo objects that exist
     *                          in the buffer.
     *
     * @return The number of valid DisplayInfo objects written to @a pOutDisplays.
     *
     * @pre  VI library is initialized. @n
             @a count >= 0
     * @post Information about the available displays is written to @a pOutDisplays and
     *       the number of displays returned.
     */
    int ListDisplays(DisplayInfo* pOutDisplays, int count) NN_NOEXCEPT;

    /**
     * @brief Open the platform's default display.
     *
     * @param[out] pOutDisplay Display handle.
     *
     * @return                       Whether the display was opened.
     * @retval ResultSuccess         The default display has been opened.
     * @retval ResultOperationFailed An unknown error occurred.
     *
     * @pre  VI library is initialized. @n
     *       @a pOutDisplay != nullptr
     * @post A valid handle is written to @a pOutDisplay.
     */
    Result OpenDefaultDisplay(Display** pOutDisplay) NN_NOEXCEPT;

    /**
     * @brief Open a display by name.
     *
     * @param[out] pOutDisplay Display handle.
     * @param[in]  name        Name of the display to open.
     *
     * @return                       Whether the display was opened.
     * @retval ResultSuccess         The requested display has been opened.
     * @retval ResultNotFound        A display with @a name could not be found.
     * @retval ResultOperationFailed An unknown error occurred.
     *
     * @pre  VI library is initialized. @n
     *       @a pOutDisplay != nullptr @n
     *       @a name != nullptr
     * @post A valid handle is written to @a pOutDisplay.
     *
     * @details Use ListDisplays() to query the display names available.
     */
    Result OpenDisplay(Display** pOutDisplay, const char* name) NN_NOEXCEPT;

    /**
     * @brief Close the display.
     *
     * @param[in] pDisplay Display handle.
     *
     * @pre  VI library is initialized. @n
     *       @a pDisplay != nullptr and @a pDisplay is a valid handle.
     * @post @a pDisplay invalidated.
     *
     * @details When a particular display is closed, all layers created by this process on that
     *          display are destroyed.  The display is effectively blanked for the process.
     *
     *          After closing, @a pDisplay should not be used.
     */
    void CloseDisplay(Display* pDisplay) NN_NOEXCEPT;

    /**
     * @brief Initializes an event that will signal on VSYNC.
     *
     * @param[out] pOutEvent Event to initialize.
     * @param[in]  pDisplay  Display that will trigger VSYNC events.
     *
     * @return                       Whether the event was initialized successfully.
     * @retval ResultSuccess         The event was initialized successfully.
     * @retval ResultNotSupported    @a pDisplay cannot support these notifications.
     * @retval ResultDenied          Not allowed to open any more events of this type.
     * @retval ResultOperationFailed An unknown error occurred.
     *
     * @pre
     *      - VI library is initialized.
     *      - @a pOutEvent != nullptr
     *      - @a pDisplay != nullptr and @a pDisplay is a valid handle.
     * @post @a pOutEvent will signal on @a pDisplay VSYNC events.
     *
     * @details @a pOutEvent will autoclear.  Only one thread will wake up when the event is signalled.
     */
    nn::Result GetDisplayVsyncEvent(nn::os::SystemEventType* pOutEvent, Display* pDisplay) NN_NOEXCEPT;

}}
