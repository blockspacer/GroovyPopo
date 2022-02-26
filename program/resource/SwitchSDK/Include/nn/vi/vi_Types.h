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
 * @brief Definitions for all types used for the VI library.
 */

#pragma once

#include <nn/nn_Common.h>

namespace nn { namespace vi {

    /**
     * @class nn::vi::Display
     * @brief Opaque handle to a display.
     */
    class Display;

    /**
     * @class nn::vi::Layer
     * @brief Opaque handle to a layer.
     */
    class Layer;

    /**
     * @brief   Platform-dependent handle to a window.
     * @details The exact type will be HWND on Windows and ANativeWindow on HOS.
     */
    typedef void* NativeWindowHandle;

    /**
     * @brief Provides information about the display and its layering capabilities.
     */
    struct DisplayInfo
    {
        static const int NameLengthMax = 64; //!< Max length of a display's name including terminator.
        char name[NameLengthMax];            //!< The enumerated display's name.

        bool hasLayerLimit;                  //!< Whether the platform imposes a limit on the number of layers.
        NN_PADDING7;

        int64_t layerCountMax;               //!< The maximum number of layers the platform can support.
                                             //!< Only valid when @a hasLayerLimit is true.

        int64_t layerWidthPixelCountMax;     //!< The maximum width supported for a layer on this display.
        int64_t layerHeightPixelCountMax;    //!< The maximum height supported for a layer on this display.
    };

}}
