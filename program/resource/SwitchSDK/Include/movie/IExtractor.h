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

#include <movie/Status.h>
#include <movie/Utils.h>

namespace movie {

/**
 * @brief Media extractor class
 * @details 
 */

class IExtractor
{
public:
    virtual ~IExtractor() {}

    virtual Status SetDataSource(const char* uri) = 0;

    virtual Status SetDataSource(const void* buffer, int32_t length) = 0;
	
    virtual Status GetTrackCount(int32_t* tracks) = 0;

    virtual Status GetTrackConfiguration(int32_t index, MediaData *config) = 0;

    virtual Status SelectTrack(int32_t index) = 0;

    virtual Status UnselectTrack(int32_t index) = 0;

    virtual Status SeekTo(int64_t timeUs) = 0;

    virtual Status GetTrackIndexForAvailableData(size_t* index) = 0;

    virtual Status Read(Buffer *buffer) = 0;

    virtual Status Advance() = 0;

    virtual Status GetSampleTime(int64_t* sampleTimeUs) = 0;

    virtual Status GetSampleMetaData(MediaData *sampleMetaData) = 0;

    virtual Status GetBufferedRange(int64_t *startTimeUs, int64_t *endTimeUs) = 0;
};

}
