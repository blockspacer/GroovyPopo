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
 * @brief Defines settings for use with layers on creation.
 */

#pragma once

#include <cstdint>
#include <nn/nn_Macro.h>

namespace nn { namespace vi {

    /**
     * @brief Contains initial layer settings.
     *
     * @details Some settings may have different behavior depending
     *          on the platform. Please review the nn::vi platform-specific
     *          documentation.
     */
    class LayerCreationSettings
    {
    public:
        /**
         * @brief Layer settings are initialized to their defaults.
         */
        LayerCreationSettings() NN_NOEXCEPT;

        /**
         * @brief Sets width and height for a layer and other settings
         *        to their defaults.
         *
         * @param[in] width  Requested width for the layer.
         * @param[in] height Requested height for the layer.
         */
        LayerCreationSettings(int width, int height) NN_NOEXCEPT;

        /**
         * @brief Retrieves the requested width.
         *
         * @returns The requested width.
         */
        int GetWidth() const NN_NOEXCEPT;

        /**
         * @brief Sets the requested width.
         *
         * @param[in] width Requested width for the layer.
         */
        void SetWidth(int width) NN_NOEXCEPT;

        /**
         * @brief Retrieves the requested height.
         *
         * @returns The requested height.
         */
        int GetHeight() const NN_NOEXCEPT;

        /**
         * @brief Sets the requested height.
         *
         * @param[in] height Requested height for the layer.
         */
        void SetHeight(int height) NN_NOEXCEPT;

        /**
         * @brief Sets whether the requested layer is fullscreen.
         *
         * @param[in] isFullscreen Whether or not the layer is fullscreen.
         *
         * @details By default, this setting is false. However, specific platforms
         *          may have different behavior regarding this setting. Please review
         *          the nn::vi platform-specific documentation for further details.
         */
        void SetFullscreen(bool isFullscreen) NN_NOEXCEPT;

        /**
         * @brief Retrieves the requested fullscreen state.
         *
         * @returns The requested fullscreen state.
         */
        bool IsFullscreen() const NN_NOEXCEPT;
    private:
        int m_Width;
        int m_Height;

        std::uint32_t m_Flags;
    };

}}
