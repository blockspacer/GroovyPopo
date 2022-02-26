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
 * @brief Common data used by Multimedia module
 */
#pragma once

namespace movie {

/**
 * @brief
 * Enumeration of movie buffer flags.
 *
 * @details
 * Buffer flags are used to convey buffer payload properties. An application can
 * send buffers to the movie decoder with buffer flags to convey certain information.
 */
enum BufferFlags
{
    BufferFlags_None            = 0,                //!< None                  
    BufferFlags_SyncFrame       = 1,                //!< Buffer payload has video data which is a sync frame.
                                                    //!< A sync frame can be decoded independently.
                                                    //!< Typically this information is conveyed to muxer.
    BufferFlags_CodecConfig     = 2,                //!< Buffer payload has audio or video data which is decoder or 
                                                    //!< encoder configuration data. If a new configuration data is
                                                    //!< sent to decoder or encoder, a format changed event is sent
                                                    //!< to the application.
    BufferFlags_EndOfStream     = 4,                //!< This flag indicates last media frame. Movie decoder will propagate 
                                                    //!< end of stream buffer flag from input buffer to the output buffer.
                                                    //!< The last output buffer will have end of stream buffer flag.
};

/**
 * @brief Supported video color space
 *
 * @details
 * This information can be used to convert video data from YUV to RGB format. These enumerations indicate the
 * color space used by the decoder. Applications need to use appropriate coefficients recommended by ITU-R
 * (International Telecommunication Union Radiocommunication Sector) for correct color reproduction.
 * Coefficients can also be found in SDK multimedia samples.
 */
enum ColorSpace
{
    ColorSpace_YCbCr601,                            //!< ITU - R Recommendation BT.601
    ColorSpace_YCbCr601_ER,                         //!< ITU - R Recommendation BT.601 Extended Range
    ColorSpace_YCbCr709,                            //!< ITU - R Recommendation BT.709
    ColorSpace_YCbCr709_ER,                         //!< ITU - R Recommendation BT.709 Extended Range
    ColorSpace_YCbCr2020,                           //!< ITU - R Recommendation BT.2020
};

}
