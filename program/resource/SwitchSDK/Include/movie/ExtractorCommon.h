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
 * @brief Extractor API's to extract media data from containers.
 */

#pragma once

namespace movie {

/**
 * @brief
 * Enumeration of container types used by Extractor.
 *
 * @details 
 */
enum ContainerType
{
    ContainerType_Unknown,                      //!< Unknown container
    ContainerType_Mpeg4,                        //!< Mpeg4 container
    ContainerType_Matroska,                     //!< Matroska container
    ContainerType_WebM,                         //!< WebM container
    ContainerType_Ogg,                          // Ogg container (don't use)
};

/**
 * @brief
 * Enumeration of cache sizes available to the DataSource.
 *
 * @details
*/
enum CacheSize
{
    CacheSize_zero = 0,     //! No cache     
    CacheSize_5MB  = 1,     //!< 5 Megabytes
    CacheSize_10MB = 2,     //!< 10 Megabytes
    CacheSize_15MB = 3,     //!< 15 Megabytes
    CacheSize_20MB = 4,     //!< 20 Megabytes
    CacheSize_25MB = 5,     //!< 25 Megabytes
    CacheSize_30MB = 6,     //!< 30 Megabytes
};

}

