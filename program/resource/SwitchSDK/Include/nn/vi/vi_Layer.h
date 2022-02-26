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
 * @brief Layer manipulation functions for the VI library.
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/vi/vi_Types.h>
#include <nn/vi/vi_PixelFormat.h>
#include <nn/vi/vi_ScalingMode.h>
#include <nn/vi/vi_LayerCreationSettings.h>

namespace nn { namespace vi {

    /**
     * @brief Creates a new layer on a particular display.
     *
     * @param[out] pOutLayer Layer handle.
     * @param[in]  pDisplay  Display handle.
     *
     * @return                          Whether the layer was created.
     * @retval ResultSuccess            Layer created successfully.
     * @retval ResultOperationFailed    An error occurred.
     *
     * @pre  VI library is initialized. @n
     *       @a pOutLayer != nullptr @n
     *       @a pDisplay != nullptr and @a pDisplay is a valid handle.
     * @post A valid handle is written to @a pOutLayer.
     *
     * @details The layer is visible by default.  Until a frame is submitted by the process, the layer will
     *          be blank.
     *
     *          The textures associated with this layer have restrictions due to a limitation with downscaling: @n
     *          @a width  <= 1920 @n
     *          @a height <= 1080 @n
     *          This restriction may be lifted in the future.
     *
     *          A particular display may only support a limited number of layers system-wide.  If a process exceeds
     *          its quota then this function will return ResultOperationFailed.  At minimum, a process can use a
     *          single layer to display content.  To query how many layers can be created by the process, use ListDisplays().
     *
     *          If this API is used with a graphics API that does not have an explicit swap chain, like EGL,
     *          then the rendering dimensions will use default values of 1280x720.
     */
    Result CreateLayer(Layer** pOutLayer, Display* pDisplay) NN_NOEXCEPT;

    /**
    * @brief Creates a new layer on a particular display.
    *
    * @param[out] pOutLayer Layer handle.
    * @param[in]  pDisplay  Display handle.
    * @param[in]  pSettings Creation settings for this layer.
    *
    * @return                          Whether the layer was created.
    * @retval ResultSuccess            Layer created successfully.
    * @retval ResultOperationFailed    An error occurred.
    *
    * @pre  VI library is initialized. @n
    *       @a pOutLayer != nullptr @n
    *       @a pDisplay != nullptr and @a pDisplay is a valid handle.
    *       @a pSettings != nullptr
    * @post A valid handle is written to @a pOutLayer.
    *
    * @details The layer is visible by default.  Until a frame is submitted by the process, the layer will
    *          be blank.
    *
    *          The textures associated with this layer have restrictions due to a limitation with downscaling: @n
    *          @a width  <= 1920 @n
    *          @a height <= 1080 @n
    *          This restriction may be lifted in the future.
    *
    *          A particular display may only support a limited number of layers system-wide.  If a process exceeds
    *          its quota then this function will return ResultOperationFailed.  At minimum, a process can use a
    *          single layer to display content.  To query how many layers can be created by the process, use ListDisplays().
    *
    *          If this API is used with a graphics API that does not have an explicit swap chain, like EGL,
    *          then the rendering dimensions will use default values of 1280x720.
    */
    Result CreateLayer(Layer** pOutLayer, Display* pDisplay, const LayerCreationSettings* pSettings) NN_NOEXCEPT;

    /**
     * @brief Creates a new layer on a particular display.
     *
     * @param[out] pOutLayer Layer handle.
     * @param[in]  pDisplay  Display handle.
     * @param[in]  width     The default width of the layer in pixels.
     * @param[in]  height    The default height of the layer in pixels.
     *
     * @return                          Whether the layer was created.
     * @retval ResultSuccess            Layer created successfully.
     * @retval ResultInvalidDimensions  The valid range is @a width > 0 and @a height > 0.
     * @retval ResultOperationFailed    An unknown error occurred.
     *
     * @pre  VI library is initialized. @n
     *       @a pOutLayer != nullptr @n
     *       @a pDisplay != nullptr and @a pDisplay is a valid handle.
     * @post A valid handle is written to @a pOutLayer.
     *
     * @details The layer is visible by default.  Until a frame is submitted by the process, the layer will
     *          be blank.
     *
     *          The textures associated with this layer have restrictions due to a limitation with downscaling: @n
     *          @a width  <= 1920 @n
     *          @a height <= 1080 @n
     *          This restriction may be lifted in the future.
     *
     *          A particular display may only support a limited number of layers system-wide.  If a process exceeds
     *          its quota then this function will return ResultOperationFailed.  At minimum, a process can use a
     *          single layer to display content.  To query how many layers can be created by the process, use ListDisplays().
     *
     *          The @a width and @a height arguments specify the default width and height of the layer in pixels.
     *          For graphics APIs that do not have an explicit swap chain, like EGL, these dimensions will be used
     *          as the rendering dimensions.
     *
     *          On Windows or other platforms, the @a width and @a height may be used in different ways.  Please check
     *          the VI Library reference for further details.
     */
    Result CreateLayer(Layer** pOutLayer, Display* pDisplay, int width, int height) NN_NOEXCEPT;

    /**
     * @brief Destroys the layer and disables content.
     *
     * @param[in] pLayer Layer handle.
     *
     * @pre  VI library is initialized. @n
     *       @a pLayer != nullptr and @a pLayer is a valid handle.
     * @post @a pLayer invalidated.
     *
     * @details No content from @a pLayer will be seen on the display after this call.
     *          The handle is invalidated and should not be used.
     */
    void DestroyLayer(Layer* pLayer) NN_NOEXCEPT;

    /**
     * @deprecated This function is deprecated. There is no longer any need to call this API.
     *             The system always applies scaling so that content matches the current display resolution,
     *             which is the same behavior as nn::vi::ScalingMode_FitToLayer. Scaling mode cannot be changed.
     *
     * @brief This function has no effect.
     *
     * @param[in] pLayer Unused.
     * @param[in] mode   Unused.
     *
     * @return                       Always successful.
     * @retval ResultSuccess         No effect.
     *
     * @pre  VI library is initialized. @n
     *       @a pLayer != nullptr and @a pLayer is a valid handle.
     * @post This function has no effect.
     *
     * @details This function is deprecated. Please do not use.
     */
    Result NN_DEPRECATED SetLayerScalingMode(Layer* pLayer, ScalingMode mode) NN_NOEXCEPT;

    /**
     * @brief Defines the rectangle inside the layer's textures that will be displayed.
     *
     * @param[in] pLayer Layer handle.
     * @param[in] x      X-coordinate of the rectangle in texture coordinates.
     * @param[in] y      Y-coordinate of the rectangle in texture coordinates.
     * @param[in] width  Width of the rectangle in texture coordinates.
     * @param[in] height Height of the rectangle in texture coordinates.
     *
     * @pre
     *       - VI library is initialized.
     *       - @a pLayer != nullptr and @a pLayer is a valid handle.
     *       - @a x >= 0
     *       - @a y >= 0
     *       - @a width >= 0
     *       - @a height >= 0
     * @post Only the rectangle specified by (@a x, @a y), (@a x + @a width, @a y),
     *       (@a x, @a y + @a height), and (@a x + @a width, @a y + @a height) will be
     *       displayed from the textures.
     *
     * @details Passing a width or height of 0 will reset the crop rectangle
     *          to match the dimensions of the texture.
     *
     *          This function serves the same purpose as nvnWindowSetCrop.
     */
    void SetLayerCrop(Layer* pLayer, int x, int y, int width, int height) NN_NOEXCEPT;

    /**
     * @brief Retrieves a handle to the platform's native window type.
     *
     * @param[out] pOutWindow Native window handle for use with nvn::WindowBuilder.
     * @param[in]  pLayer     Layer handle.
     *
     * @return                       Whether a handle was written to @a pOutWindow.
     * @retval ResultSuccess         @a pOutWindow contains the native window handle.
     * @retval ResultOperationFailed An unknown error occurred.
     *
     * @pre  VI library is initialized. @n
     *       @a pLayer != nullptr and @a pLayer is a valid handle. @n
     *       @a pOutWindow != nullptr
     * @post On nn::ResultSuccess, the window handle is written to @a pOutWindow.
     *
     * @details This handle may be safely used by other APIs.  Its exact type will depend
     *          on the platform.
     */
    Result GetNativeWindow(NativeWindowHandle* pOutWindow, Layer* pLayer) NN_NOEXCEPT;

}}
