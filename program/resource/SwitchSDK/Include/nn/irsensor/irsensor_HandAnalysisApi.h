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
 * @brief API declaration for HandAnalysis
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/irsensor/irsensor_Result.h>
#include <nn/irsensor/irsensor_IrCameraTypes.h>
#include <nn/irsensor/irsensor_HandAnalysisConstants.h>
#include <nn/irsensor/irsensor_HandAnalysisTypes.h>

namespace nn { namespace irsensor {

/**
 * @name HandAnalysis
 * @{
 */

/**
 *  @brief              Start HandAnalysis mode of IR sensor
 *  @param[in] handle   Handle of IR camera
 *  @param[in] config   Configuration of HandAnalysis mode
 *  @return             Return whether the camera successfully switched to HandAnalysis mode
 *  @pre
 *                      - Handle is already initialized via Initialize API
 *                      - For given handle, IR camera is not running yet
 *                      - config can be one of following:
 *                          - HandAnalysisMode_Silhouette mode gives hand silhouette, hand information, and finger information
 *                          - HandAnalysisMode_Image mode gives compressed raw image (40x30)
 *                          - HandAnalysisMode_SilhouetteAndImage gives hand information and coarse image in interleaved sequence
 *  @details            User sets a configuration to HandAnalysis and IR sensor starts processing
 *                      User keeps getting data via Get**() API while HandAnalysis is running
 *                      When IR sensor is already running another mode, it stops running mode and starts HandAnalysis mode with given configuration.
 */
::nn::Result RunHandAnalysis(const IrCameraHandle& handle, const HandAnalysisConfig& config) NN_NOEXCEPT;

/**
 *  @brief                           Retrieve latest HandAnalysis silhouette mode data
 *
 *  @param[out] pOutValueArray       User allocated array that will receive the processed data
 *  @param[out] pReturnCount         Number of elements written to pOutValueArray
 *  @param[in]  maxCount             Maximum number of elements that can be written to pOutValueArray
 *  @param[in]  infSamplingNumber    Lowest sampling number of elements you want to retrieve
 *  @param[in]  handle               Handle of IR sensor
 *
 * @details                          This function retrieves silhouette states sent by the IR sensor. You can
 *                                   retrieve several states at the same time. The function will return all
 *                                   the states available where infSamplingNumber <= state.samplingNumber. If your
 *                                   array is not large enough it will return the more recent silhouette
 *                                   states, i.e. the highest sampling numbers.
 *                                   Usually you want infSamplingNumber == 0 for the first call and having
 *                                   infSamplingNumber == lastSamplingNumber + 1 for the next one.
 *
 *                                   Unlike other irsensor modes (like clustering or moment modes), getting states
 *                                   in the silhouette mode will copy and decode data, which is time consuming.
 *                                   The benefit of using infSamplingNumber is to avoid extra computation for states
 *                                   you already received.
 *
 *                                   When the IR sensor is not functioning properly, the system will display a dialog
 *                                   with an error message.
 *                                   This function blocks the calling thread while the message is displayed.
 *                                   After the user closes the error dialog, the function returns nn::irsensor::ResultIrsensorDeviceError.
 *
 *                                   When the controller firmware needs an update,
 *                                   the system calls the controller support applet to update it.
 *                                   While updating, this function blocks the calling thread.
 *                                   If the update succeeds this function returns nn::irsensor::ResultIrsensorDeviceNotReady,
 *                                   and otherwise returns nn::irsensor::ResultIrsensorDeviceError.
 *
 *                                   As mentioned above, since this API can block the thread which calls it,
 *                                   we recommend to call it from other thread than main loop.
 *
 *                                   IR sensor is not available when the application uses NFC functions on at least one controller.
 *                                   When there are more than 4 connected controllers, only the first IR sensor is available.
 *                                   If these constraints are not satisfied, this function returns nn::irsensor::ResultIrsensorDeviceResourceNotAvailable.
 *
 *  @retresult
 *   @handleresult{nn::irsensor::ResultIrsensorDeviceNotReady}
 *   @handleresult{nn::irsensor::ResultIrsensorUnavailable}
 *   @handleresult{nn::irsensor::ResultIrsensorDeviceError}
 *   @handleresult{nn::irsensor::ResultHandAnalysisModeIncorrect}
 *   @handleresult{nn::irsensor::ResultIrsensorDeviceResourceNotAvailable}
 *  @endretresult
 *  @pre
 *                         - Handle is already initialized via Initialize API.
 *                         - For a given handle, IR camera is running as silhouette mode of HandAnalysis
 *                         - pOutValue != nullptr
 *                         - pReturnCount != nullptr
 *  @post
 *                         - For all pOutValue in pOutValueArray .. pOutValueArray + pReturnCount - 1:
 *                             - 0 <= pOutValue->shapeCount && pOutValue->shapeCount <= IrHandAnalysisShapeCountMax
 */
::nn::Result GetHandAnalysisSilhouetteState(HandAnalysisSilhouetteState* pOutValueArray, int *pReturnCount, int maxCount, int64_t infSamplingNumber, const IrCameraHandle& handle) NN_NOEXCEPT;

/**
 *  @brief                           Retrieve reduced compressed IR camera images.
 *
 *  @param[out] pOutValueArray       User allocated buffer that will receive the image
 *  @param[out] pReturnCount         Number of elements written to pOutValueArray
 *  @param[in]  maxCount             pOutValueArray size
 *  @param[in]  infSamplingNumber    Lowest sampling number of elements you want to retrieve
 *  @param[in]  handle               Handle of IR sensor
 *
 *  @details                         The image resolution is (IrHandAnalysisImageWidth, IrHandAnalysisImageHeight).
 *                                   The IR camera image is converted to a 16-bit range in order to
 *                                   compensate for internal camera parameters, so that intensity can be compared between
 *                                   images without taking internal camera parameters into account.
 *
 *                                   This function retrieves images sent by the IR sensor. You can
 *                                   retrieve several images at the same time. The function will return all
 *                                   the images available where infSamplingNumber <= image.samplingNumber.
 *                                   If your array is not large enough it will return the more recent images,
 *                                   i.e. the highest sampling numbers.
 *                                   Usually you want infSamplingNumber == 0 for the first call and having
 *                                   infSamplingNumber == lastSamplingNumber + 1 for the next one.
 *
 *                                   Unlike other irsensor modes (like clustering or moment modes), getting states
 *                                   in the silhouette mode will copy and decode data, which is time consuming.
 *                                   The benefit of using infSamplingNumber is to avoid extra computation for states
 *                                   you already received.
 *
 *                                   When the IR sensor is not functioning properly, the system will display a dialog
 *                                   with an error message.
 *                                   This function blocks the calling thread while the message is displayed.
 *                                   After the user closes the error dialog, the function returns nn::irsensor::ResultIrsensorDeviceError.
 *
 *                                   When the controller firmware needs an update,
 *                                   the system calls the controller support applet to update it.
 *                                   While updating, this function blocks the calling thread.
 *                                   If the update succeeds this function returns nn::irsensor::ResultIrsensorDeviceNotReady,
 *                                   and otherwise returns nn::irsensor::ResultIrsensorDeviceError.
 *
 *                                   As mentioned above, since this API can block the thread which calls it,
 *                                   we recommend to call it from other thread than main loop.
 *
 *                                   IR sensor is not available when the application uses NFC functions on at least one controller.
 *                                   When there are more than 4 connected controllers, only the first IR sensor is available.
 *                                   If these constraints are not satisfied, this function returns nn::irsensor::ResultIrsensorDeviceResourceNotAvailable.
 *
 *  @retresult
 *   @handleresult{nn::irsensor::ResultIrsensorDeviceNotReady}
 *   @handleresult{nn::irsensor::ResultIrsensorUnavailable}
 *   @handleresult{nn::irsensor::ResultIrsensorDeviceError}
 *   @handleresult{nn::irsensor::ResultIrsensorDeviceResourceNotAvailable}
 *  @endretresult
 *  @pre
 *                         - Handle is already initialized via Initialize API.
 *                         - For a given handle, IR camera is running as image mode of HandAnalysis
 *                         - pOutValue != nullptr
 *                         - pReturnCount != nullptr
 *  @post
 *                         - For all pOutValue in pOutValueArray .. pOutValueArray + pReturnCount - 1:
 *                             - 0 <= pOutValue->image[j] && pOutValue->image[j] <= 65535
 *                             - pOutValue->image[j] indicates 16 bit grayscale value where j is pixel location of (x + y * IrHandAnalysisImageWidth)
 *                             - valid pixel locations (x, y) are between (0, 0) and (IrHandAnalysisImageWidth, IrHandAnalysisImageHeight)
 */
::nn::Result GetHandAnalysisImageState(HandAnalysisImageState* pOutValueArray, int *pReturnCount, int maxCount, int64_t infSamplingNumber, const IrCameraHandle& handle) NN_NOEXCEPT;

/**
 * @}
 */

} } // namespace nn::irsensor
