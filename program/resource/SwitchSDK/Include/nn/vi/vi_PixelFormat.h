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
 * @brief Defines all pixel formats that may be supported by the display server.
 */

#pragma once

namespace nn { namespace vi {

    /**
     * @brief   Pixel formats for use with the display server.
     * @details Not all formats may be supported on all platforms.
     */
    enum PixelFormat
    {
        PixelFormat_Rgba8888,         //!< 32-bit R8G8B8A8
        PixelFormat_Rgbx8888,         //!< 32-bit R8G8B8X8
        PixelFormat_Rgb888,           //!< 24-bit R8G8B8
        PixelFormat_Rgb565,           //!< 16-bit R5G6B5
        PixelFormat_Bgra8888,         //!< 32-bit B8G8R8A8
        PixelFormat_Rgba5551,         //!< 16-bit R5G5B5A1
        PixelFormat_Rgba4444,         //!< 16-bit R4G4B4A4
        PixelFormat_StandardRgba8888, //!< 32-bit R8G8B8A8 (sRGB)
        PixelFormat_StandardRgbx8888, //!< 32-bit R8G8B8X8 (sRGB)
        PixelFormat_Unknown,          //!< Display server is unable to support this format.
    };

}}
