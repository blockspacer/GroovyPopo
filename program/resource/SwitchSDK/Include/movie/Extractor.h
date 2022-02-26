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
 * @brief Extractor API to extract media data from containers.
 */

#pragma once

#include <movie/IExtractor.h>
#include <movie/ExtractorCommon.h>
#include <movie/Status.h>

namespace movie {
   /**
    * @brief
    * ClientStreamReader class
    *
    * @details
    * ClientStreamReader can be used as a data source for movie extractor. An application can use this interface
    * to implement custom media data reader. Applications can use this interface for implementing network read or
    * other application specific read optimizations.
    */
    class ClientStreamReader
    {
    public:
       /**
        * @brief ClientStreamReader destructor.
        */
        virtual ~ClientStreamReader() {}

       /**
        * @brief Open given stream.
        *
        * @param[in] stream                             Stream to open.
        * @param[out] streamHandle                      Stream handle created by the application.
        *
        * @retval true                                  Stream open success.
        * @retval false                                 Stream open failed.
        *
        * @details
        * Application must implement this method. Movie extractor will call Open() method using the application
        * provided ClientStreamReader object. Application needs to return a valid, non-null handle. Movie extractor
        * will pass this handle to the application when ReadAT(), Close() and GetSize() APIs are called. Stream is 
        * the uri set by the application using SetDataSource() movie extractor API, and may be nullptr. Movie extractor
        * does not use uri, it simply passes it back to the application.
        */
        virtual bool Open(const char *stream, void** streamHandle) = 0;

       /**
        * @brief Read given size of data from the stream.
        *
        * @param[in] streamHandle                       Valid stream handle.
        * @param[in] offset                             Stream offset from where data is needed.
        * @param[out] data                              Application buffer to read data.
        * @param[in] size                               Amount of data to read.
        *
        * @return Number of bytes read.
        *
        * @pre
        *  The stream is open; a valid stream handle is available.
        *
        * @details
        * Application must implement this method. Movie extractor will call ReadAt() API using the application
        * provided ClientStreamReader object. Stream reader implementation needs to copy requested amount of data at
        * the specified offset. This API needs to return the size of data copied. If the return value is zero, 
        * movie extractor marks end of stream.
        */

        virtual size_t ReadAt(void* streamHandle, int64_t offset, void *data, size_t size) = 0;
       /**
        * @brief Close stream.
        *
        * @param[in] streamHandle                       Valid stream handle.
        *
        * @retval true                                  Stream close success.
        * @retval false                                 Stream close failed.
        *
        * @pre
        *  The stream is open; a valid stream handle is available.
        *
        * @details
        * Application must implement this method. Movie extractor will call this API using the application
        * provided ClientStreamReader object. Movie extractor will call this API when media playback is complete.
        * Movie extractor can also call this API when internal errors prevent parsing or playback.
        *
        */

        virtual bool Close(void* streamHandle) = 0;
       /**
        * @brief Get stream size.
        *
        * @param[in] streamHandle                       Valid stream handle.
        * @param[out] size                              Stream size.
        *
        * @pre
        *  The stream is open; a valid stream handle is available.
        *
        * @details
        * Application must implement this method. Movie extractor will call GetSize() API to find total data
        * available in the source stream. This value is typically the media file size.
        */
       virtual void GetSize(void* streamHandle, int64_t *size)= 0;   
};

/**
 * @brief Media extractor class
 * @details 
 */

class Extractor : public IExtractor
{
public:

    /**
     * @brief Extractor constructor.
     *
     * @param[in] eContainerType                Type of container to create extractor
     * 
     * @details 
     * Create extractor instance with requested container type. Refer to @a ContainerType for supported containers.
     * Extractor will create a data cache of size up to movie::CacheSize::CacheSize_20MB (default size).
     */
    Extractor(ContainerType eContainerType);

    /**
    * @brief Extractor constructor.
    *
    * @param[in] eContainerType                Type of container to create extractor.
    * @param[in] eHighCacheThreshold           High cache threshold for DataSource.
    *
    * @details
    * Create extractor instance with requested container type and maximum cache size.
    * Extractor will create a data cache of size eHighCacheThreshold (movie::CacheSize).
    */
    Extractor(ContainerType eContainerType, CacheSize eHighCacheThreshold);
    
    /**
     * @brief Extractor destructor.
     *
     * @details 
     * Extractor instance is destroyed.
     */
    virtual ~Extractor();

    /**
     * @brief Sets data source for media data extraction.
     *
     * @param[in] uri                           Media path 
     *
     * @return ::movie::Status
     * @retval ::Status_Success
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre  
     * - Extractor is created successfully.
     * @post 
     * - Extractor is ready to extract media data from the container file.
     *
     * @details 
     * Clients can use this API to set data source for extracting media data. The extractor will probe
     * the uri to check whether data source has valid media tracks which it can extract. Extractor 
     * will return an error if it cannot find valid or supported tracks in the container.
     */
    virtual Status SetDataSource(const char* uri) NN_NOEXCEPT;
    
    
    /**
     * @brief Sets data source for media data extraction.
     *
     * @param[in] streamReader                  A pointer to the client reader class 
     * @param[in] uri                           Media path 
     *
     * @return ::movie::Status
     * @retval ::Status_Success
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre  
     * - Extractor is created successfully.
     * @post 
     * - Extractor is ready to extract media data from the container file.
     *
     * @details 
     * Clients can use this API to set data source for extracting media data. The extractor will probe
     * the uri to check whether data source has valid media tracks which it can extract. Extractor 
     * will return an error if it cannot find valid or supported tracks in the container.
     */
    virtual Status SetDataSource(ClientStreamReader* streamReader, const char* uri) NN_NOEXCEPT;

    /**
     * @brief Sets memory data source for media data extraction.
     *
     * @param[in] buffer                        Memory containing media data 
     * @param[in] length                        Media data size in bytes in buffer
     *
     * @return ::movie::Status
     * @retval ::Status_Success
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre  
     * - Extractor is created successfully.
     * @post 
     * - Extractor is ready to extract media data from the memory data source.
     *
     * @details 
     * Clients can use this API to set data source for extracting media data. The extractor will probe
     * the data in the buffer to check whether data source has valid media tracks which it can extract.
     * Extractor will return an error if it cannot find valid or supported tracks in the container.
     */
    virtual Status SetDataSource(const void* buffer, int32_t length) NN_NOEXCEPT;
	
    /**
     * @brief Get number of tracks present in a media file.
     *
     * @param[out] tracks                       Number of tracks present.
     *
     * @return ::movie::Status
     * @retval ::Status_Success
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre  
     * - Extractor is created.
     * - A valid data source is set successfully.
     *
     * @details 
     * The total number of tracks present in the current media file is retrieved and returned 
     * to the client. The track numbers are from 0 to (*tracks - 1).
     */
    virtual Status GetTrackCount(int32_t* tracks) NN_NOEXCEPT;

    /**
     * @brief Get track configuration data.
     *
     * @param[in]  index                        Track index for which format information is requested
     * @param[out] config                       Configuration information for the given track
     *
     * @return ::movie::Status
     * @retval ::Status_Success
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre  Extractor is created. @n
     * @pre  Valid data source is set. @n
     *
     * @details 
     * This API can be used to get track configuration data. This configuration data is needed by "Decoder" to
     * successfully initialize them. The data is in [key, value] pair format and may be different for different containers.
     * "config" is filled with the media meta data. 
     *
     * - "mime"            (char*)   - Mime type for the track
     * - "durationUs"      (int64_t) - Track duration in microseconds
     * - "is-sync-frame"   (int32_t) - Whether the frame is a sync frame
     * - "bit-rate"        (int32_t) - Bitrate(bits per second)
     * - "width"           (int32_t) - Video height in number of lines
     * - "height"          (int32_t) - Video height in number of lines
     * - "crop"            (int32_t, int32_t, int32_t, int32_t) - Crop rectangle of the output video
     * - "frame-rate"      (int32_t) - Video presentation frame rate, frames per seconds
     * - "channel-count"   (int32_t) - Number of audio channels
     * - "sample-rate"     (int32_t) - Audio sample rate in hertz
     * - "max-input-size"  (int32_t) - Maximum input size of the input frame
     * - "nv12-colorspace" (int32_t) - color space information. enum defined in movie::ColorSpace
     * - "media-language"  (char*)   - language type for the track

     * <br>Example:  
     * <br>Reading the meta data:
     * @code
     * movie::MediaData metaData;
     * int64_t durationUs;
     * metaData->FindInt64("durationUs", &durationUs);
     *
     * const char *mime;
     * metaData->FindString("mime", &mime);
     *
     * int32_t cropLeft, cropTop, cropRight, cropBottom;
     * metaData->FindRect("crop", &cropLeft, &cropTop, &cropRight, &cropBottom);
     * @endcode
     */
    virtual Status GetTrackConfiguration(int32_t index, MediaData *config) NN_NOEXCEPT;

    /**
     * @brief Select media track.
     *
     * @param[in] index                         Track number to select
     *
     * @return ::movie::Status
     * @retval ::Status_Success
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre  
     * - Extractor is created.
     * - Valid data source is set. @n
     * - Track count is retrieved with GetTrackCount().
     *
     * @details 
     * Use this API to select a particular track present in the media files. 
     */
    virtual Status SelectTrack(int32_t index) NN_NOEXCEPT;

    /**
     * @brief Media track is unselected, no data is read from this track.
     *
     * @param[in] index                         Track number to unselect
     *
     * @return ::movie::Status
     * @retval ::Status_Success 
     * @retval ::Status_ErrorBadValue         (Failure) Invalid track number
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre  
     * - Extractor is created.
     * - Valid data source is set. @n
     * - Track count is retrieved with GetTrackCount().
     * @post 
     * - Data from unselected tracks are not sent to decoder.
     *
     * @details 
     * Use this API to unselect a particular track present in the media files. 
     */
    virtual Status UnselectTrack(int32_t index) NN_NOEXCEPT;

    /**
     * @brief Jump to specific time in the media file
     *
     * @param[in] timeUs                      Time in microseconds to seek.
     *
     * @return ::movie::Status
     * @retval ::Status_Success
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre
     * - Extractor is created.
     * - Valid data source is set. @n
     * @post
     * - Extractor points to new playback position.
     * 
     * @details 
     * Extractor will seek the data source using the seek value (time) provided. The next Read() will return 
     * data from the new position.
     */
    virtual Status SeekTo(int64_t timeUs) NN_NOEXCEPT;

    /**
     * @brief Get track index for available data
     * @param[out] index                      Return track index of current sample
     *
     * @return ::movie::Status
     * @retval ::Status_Success
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre  
     * - Extractor is created.
     * - A valid data source is set successfully.
     */
    virtual Status GetTrackIndexForAvailableData(size_t* index) NN_NOEXCEPT;

    /**
     * @brief Read media data
     *
     * @param[in] buffer                        Buffer to read media sample
     *
     * @return ::movie::Status
     * @retval ::Status_Success
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre  
     * - Extractor is created.
     * - A valid data source is set successfully.
     */
    virtual Status Read(Buffer *buffer) NN_NOEXCEPT;

    /**
     * @brief Advance extractor
     *
     * @return ::movie::Status
     * @retval ::Status_Success
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre  
     * - Extractor is created.
     * - A valid data source is set successfully.
     * @post 
     * Extractor points to new sample position.
     */
    virtual Status Advance() NN_NOEXCEPT;

    /**
     * @brief Get sample time
     * @param[out] sampleTimeUs                 Return current sample timestamp
     *
     * @return ::movie::Status
     * @retval ::Status_Success
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre  
     * - Extractor is created.
     * - A valid data source is set successfully.
     */
    virtual Status GetSampleTime(int64_t* sampleTimeUs) NN_NOEXCEPT;

    /**
     * @brief Get sample meta data
     * @param[out] sampleMetaData               Return current sample meta data
     *
     * @return ::movie::Status
     * @retval ::Status_Success
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre  
     * - Extractor is created.
     * - A valid data source is set successfully.
     */
    virtual Status GetSampleMetaData(MediaData *sampleMetaData) NN_NOEXCEPT;

    /**
    * @brief Returns the buffered data range.
    *
    * @param[out] startTimeUs                   The start time of the buffered range in microseconds
    * @param[out] endTimeUs                     The end time of the buffered range in microseconds
    *
    * @return ::movie::Status
    * @retval ::Status_Success
    * @retval Other    (Failure) See ::movie::Status for details.
    *
    * @pre  
    *  - Extractor is created.
    *  - A valid data source is set successfully.
    */
    virtual Status GetBufferedRange(int64_t *startTimeUs, int64_t *endTimeUs) NN_NOEXCEPT;

private:
    void Init(ContainerType eContainerType, CacheSize eHighCacheThreshold);
    void* m_pExtractorImpl;
};

}
