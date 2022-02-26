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
 * @brief Struct declaration for HandAnalysis
 */

#pragma once

#include <nn/util/util_MathTypes.h>
#include <nn/irsensor/irsensor_HandAnalysisConstants.h>
#include <nn/irsensor/irsensor_IrCameraTypes.h>

namespace nn { namespace irsensor {

/**
 * @brief Structure for HandAnalysis mode configuration
 */
struct HandAnalysisConfig
{
    HandAnalysisMode mode; //!< setting of HandAnalysis mode.
};

/**
 * @brief Structure for storing protrusion information
 *
 * @details  a protrusion is the part of a shape protruding from what has been
 *           identified as a palm. For a shape corresponding to a hand,
 *           a protrusion would probably be a group of extended fingers or the
 *           arm.
 */
struct Protrusion
{
    int firstPointIndex;  //!< index of first point of silhouette of the protrusion in shape
    int pointCount;       //!< number of points in silhouette of the protrusion. points are HandAnalysisSilhouetteState::points[Shape::firstPointIndex + (firstPointIndex + i) % Shape::pointCount] with 0 <= i < pointCount. if pointCount is 0, protrusion is not valid
};

/**
 * @brief Structure for storing extended finger information
 */
struct Finger
{
    bool isValid;             //!< is this finger valid? If false, the remaining fields have no meaning. it can be false when the finger hasn't been identified or is folded
    ::nn::util::Float2 tip;   //!< position of the tip of the finger
    float tipDepthFactor;     //!< estimation of tip depth relative to hand. value is in [0,1], 0 when finger is away, 1 when it is closest to camera
    ::nn::util::Float2 root;  //!< position where the finger meets the palm
    int protrusionIndex;      //!< index in Hand::protrusions array corresponding to the protrusion where this finger has been identified
};

/**
 * @brief Structure for storing palm information
 */
struct Palm
{
    ::nn::util::Float2 center;  //!< center of palm
    float area;                 //!< area of palm
    float depthFactor;          //!< estimation of palm depth (should evolve as real depth). Value is in [0,1]
};

/**
* @brief Structure for storing arm information
*/
struct Arm
{
    bool isValid;                     //!< is the arm valid? if false, the remaining fields have no meaning. it can be false when the arm hasn't been identified
    ::nn::util::Float2 wristPosition; //!< wrist position
    ::nn::util::Float2 armDirection;  //!< arm direction vector, directed from wrist to elbow.
    int protrusionIndex;              //!< index in Hand::protrusions array corresponding to the protrusion where the arm has been identified
};

/**
 * @brief Structure for storing hand information
 */
struct Hand
{
    int shapeId;                                              //!< index of the shape in HandAnalysisSilhouetteState::shapes array it corresponds to
    int protrusionCount;                                      //!< number of protrusions (0-8)
    Protrusion protrusions[IrHandAnalysisProtrusionCountMax]; //!< protrusions. Only the first protrusionCount items are valid (the others being set to a default value)
    HandChirality chirality;                                  //!< left or right hand
    Finger fingers[HandFinger_Count];                         //!< list of identified extended fingers. it should be indexed by HandFinger values
    bool areFingersTouching[HandTouchingFingers_Count];       //!< should be indexed by HandTouchingFingers values. for extended fingers, true means that fingers are touching. if one or both fingers are not valid (e.g. folded), the associated boolean has no meaning.
    Palm palm;                                                //!< palm information
    Arm arm;                                                  //!< arm information
};


/**
 * @brief Structure for storing shape information
 *
 * @details  A shape is represented by its polygonal silhouette. This is a set
 *           of contiguous points stored in HandAnalysisSilhouetteState::points
 *           array. The order of these points tells whether the shape is filled
 *           or empty (e.g. a hole in another shape): the interior of the shape
 *           is always to the right of the oriented edges.
 *           The average and centroid of the intensity of the the camera frame
 *           inside the shape are also available.
 */
struct Shape
{
    int firstPointIndex;                    //!< index of first point of shape silhouette in HandAnalysisSilhouetteState::points
    int pointCount;                         //!< number of points in shape silhouette
    float intensityAverage;                 //!< average of intensity over the shape
    ::nn::util::Float2 intensityCentroid;   //!< centroid of intensity
};

/**
 * @brief Structure for storing silhouette state
 *
 * @details  All coordinates are expressed in centered unit frame coordinates.
 *           A point in camera frame has x in [-4/3, 4/3] and y in [-1, 1].
 *           A value of (0,0) means that the point is at the center.
 *           IRSENSOR documentation includes a picture explaining each member.
 *           This struct size is almost 5 kB. If you are running it on a
 *           thread, be careful about the stack size.
 */
struct HandAnalysisSilhouetteState
{
    int64_t samplingNumber;                                      //!< sampling number that increments when new data arrives to system (more than 0).
    IrCameraAmbientNoiseLevel ambientNoiseLevel;                 //!< noise level from surrounding environment
    ::nn::util::Float2 points[IrHandAnalysisShapePointCountMax]; //!< list of points containing polygonal silhouettes of multiple shapes.
    int shapeCount;                                              //!< number of shapes (0-16)
    Shape shapes[IrHandAnalysisShapeCountMax];                   //!< list of shapes. Only the first shapeCount items are valid
    int handCount;                                               //!< number of identified hands (0-2)
    Hand hands[IrHandAnalysisHandCountMax];                      //!< list of hands. Only the first handCount items are valid
};

/**
 * @brief Structure for storing image information
 *
 * @details This struct size is almost 2.5 kB. If you are running it on a thread, be careful about the stack size.
 */
struct HandAnalysisImageState
{
    int64_t samplingNumber;                                               //!< sampling number that increments when new data arrives to system (more than 0).
    IrCameraAmbientNoiseLevel ambientNoiseLevel;                          //!< noise level from surrounding environment
    uint16_t image[IrHandAnalysisImageWidth * IrHandAnalysisImageHeight]; //!< lossy compressed 40x30 image
};

}} // namespace nn::irsensor
