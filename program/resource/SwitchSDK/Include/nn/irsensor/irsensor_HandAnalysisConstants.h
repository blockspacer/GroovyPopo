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

namespace nn { namespace irsensor {

const int HandAnalysisProcessorStateCountMax = 5;       //!< maximum number of buffered HandAnalysisProcessorState

const int IrHandAnalysisShapePointCountMax = 512;       //!< maximum number of points in shape
const int IrHandAnalysisShapeCountMax = 16;             //!< maximum number of shapes
const int IrHandAnalysisProtrusionCountMax = 8;         //!< maximum number of protrusions in one hand
const int IrHandAnalysisHandCountMax = 2;               //!< maximum number of hands

const int IrHandAnalysisImageWidth  = 40; //!< width of image in HandAnalysis Image mode
const int IrHandAnalysisImageHeight = 30; //!< height of image in HandAnalysis Image mode

/**
 * @brief HandAnalysis modes
 */
enum HandAnalysisMode
{
    HandAnalysisMode_None = 0,                  //!< invalid mode
    HandAnalysisMode_Silhouette = 1,            //!< silhouette mode which gives hand information
    HandAnalysisMode_Image = 2,                 //!< image mode which returns compressed 40x30 sensor image
    HandAnalysisMode_SilhouetteAndImage = 3,    //!< silhouette and compressed image in interleaved sequence
    HandAnalysisMode_SilhouetteOnly = 4,        //!< silhouette mode which gives only silhouette information with faster framerate
};

/**
 * @brief Hand chirality
 */
enum HandChirality
{
    HandChirality_Left = 1,     //!< left hand
    HandChirality_Right = 2,    //!< right hand
    HandChirality_Unknown = 3,  //!< left or right hand
};

/**
 * @brief Hand fingers
 */
enum HandFinger
{
    HandFinger_Thumb = 0,  //!< thumb
    HandFinger_Index = 1,  //!< index
    HandFinger_Middle = 2, //!< middle
    HandFinger_Ring = 3,   //!< ring
    HandFinger_Little = 4, //!< little

    HandFinger_Count = 5,  //!< number of fingers
};

/**
* @brief Hand touching fingers enumeration
*/
enum HandTouchingFingers
{
    HandTouchingFingers_IndexMiddle = 0,    //!< index-middle
    HandTouchingFingers_MiddleRing = 1,     //!< middle-ring
    HandTouchingFingers_RingLittle = 2,     //!< ring-little

    HandTouchingFingers_Count = 3,          //!< count
};

}} // namespace nn::irsensor
