/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

/**
 * @file Decoder.h
 * @brief Decoder Interface
 */

#include <nn/nn_Macro.h>
#include <movie/IDecoder.h>
#include <movie/Status.h>
#include <movie/Utils.h>

namespace movie { 

/**
 * @brief  
 * Decoder API to decode audio and video data
 *
 */
class Decoder : public IDecoder
{           
public:

    /**
     * @brief Decoder constructor.
     *
     * @param[in] eDecoderType              Type of decoder to create. See ::DecoderType for supported types.
     * @param[in] eDecoderOutputFormat      Required output format. See ::DecoderOutputFormat for supported formats.
     * @param[in] eDecoderMode              Decoder mode to use. See ::DecoderMode for supported modes.
     * @param[in] audioThreadPriority       Priority for all threads created by Audio decoder.
     *
     * @details
     * Decoder is constructed but not configured.
     * Note: Change audio decoder priority with caution! 
     * If left unspecified here, the priority is determined by the movie::GetThreadPriority() API, 
     * which returns either nn::os::DefaultThreadPriority (by default) or the value specified with
     * the movie::SetThreadPriority() API.
     * If you wish to specify the priority here, nn::os::DefaultThreadPriority is recommended.
     */    
    Decoder(DecoderType eDecoderType,
        DecoderOutputFormat eDecoderOutputFormat,
        DecoderMode eDecoderMode = DecoderMode_Cpu,
        int audioThreadPriority = movie::GetThreadPriority()) NN_NOEXCEPT;

    /**
     * @brief Decoder destructor.
     *
     */
    virtual ~Decoder();

    /**
     * @brief Configure decoder with initialization data.
     *
     * @param[in]   config                  Decoder configuration (initialization) data
     *
     * @return ::movie::Status 
     *
     * @retval ::Status_Success        
     * @retval ::Status_UnsupportedMediaType  (Failure) Media type is not supported, see ::DecoderType for supported types.
     * @retval Other (Failure) See ::movie::Status for details.
     * 
     * @pre
     * - Decoder has been created successfully.
     * @post 
     * - Decoder is configured.
     *
     * @details 
     * Decoder is setup with configuration data and initialized. Configuration data is passed as [key, value] pairs
     * using MediaData structure. Decoders require one or more decoder specific data for successful initialization.
     * Decoder specific data is available from Extractor using the GetTrackConfiguration() API.
     *
     * Decoder input buffer size is calculated based on the information provided by Extractor (or the default value).
     * If a client desires to change this value, update the configuration as follows:
     * @code
     * int32_t maxInputSize = 64 * 1024;
     * config.SetInt32("max-input-size", maxInputSize);
     * @endcode
     * Note: If "maxInputSize" is less than 64KB, a 64KB buffer is allocated (in case of video decoder).
     * Decoder allocates five(5) input buffers of requested size.
     */
    virtual Status Configure(MediaData* config) NN_NOEXCEPT;

    /**
     * @brief 
     * API to register events with decoder.
     *
     * @param[in]   decoderEvents             Decoder events from client.
     *
     * @return ::movie::Status
     * @retval ::Status_Success     
     * @retval Other    (Failure) See ::movie::Status for details.

     * @details 
     * 
     * Application creates and registers "nn::os::EventType" events with decoder using ::movie::DecoderEvents structure.
     * Available decoder events:
     * - nn::os::EventType* inputBufferAvailableEvent  
     *      - Triggered when input buffers available from decoder.
     *      - Client use GetInputBufferIndex() API to get buffer indices.
     * - nn::os::EventType* outputBufferAvailableEvent 
     *      - Triggered when output buffers area available from decoder.
     *      - Client use GetOutputBufferIndex() API to get buffer indices.
     * - nn::os::EventType* formatChangedEvent         
     *      - Triggered when media format is changed.
     *      - Client uses the GetOutputFormat() API to get new format.
     * - nn::os::EventType* errorEvent                 
     *      - Triggered when any error is detected by decoder.
     *      - Client use GetLastError() API to get error details.
     */
    virtual Status RegisterClientEvents(DecoderEvents* decoderEvents) NN_NOEXCEPT;

    /**
     * @brief Get decoder output format.
     *
     * @param[in]   format                  Decoder format 
     *
     * @return ::movie::Status
     * @retval ::Status_Success          
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre
     * - Decoder is started
     *
     * @details 
     * Decoder output format is available in [key, value] pairs using MediaData structure. 
     * All the information may not be available for a given decoder.
     * <br>Query output buffer format using movie::MediaData 
     *  - "mime"           - Mime type of the track, data type -> char string
     *  - "durationUs"     - Playback duration of current media file in microseconds, data type -> int64_t
     *  - "bit-rate"       - Media bitrate(bits per second), data type -> int32_t
     *  - "width"          - Video width in number of lines, data type -> int32_t
     *  - "height"         - Video height in number of lines, data type -> int32_t (see note below)
     *  - "color-format"   - Video color format, data type -> int32_t
     *  - "crop"           - Video crop information, data type -> int32_t
     *  - "frame-rate"     - Video presentation frame rate, frames per seconds, data type -> int32_t
     *  - "is-sync-frame"  - A sync frame, data type -> int32_t
     *  - "channel-count"  - Number of audio channels, data type -> int32_t
     *  - "sample-rate"    - Audio sample rate in Hertz, data type -> int32_t
     *  - "max-input-size" - Maximum input size, data type -> int32_t
     *
     * Example:
     * @code
     * movie::MediaData format;
     * GetOutputFormat(&format);
     * const char *mime;
     * format.FindString("mime", &mime);
     *
     * int64_t durationUs = 0ll;
     * format.FindInt64("durationUs", &durationUs);
     *
     * int32_t bitRate = 0;
     * format.FindInt32("bit-rate", &bitRate);
     *
     * int32_t cropLeft, cropTop, cropRight, cropBottom;
     * format.FindRect("crop", &cropLeft, &cropTop, &cropRight, &cropBottom);
     * @endcode
     * 
     * @note
     * Prior to NintendoSDK 6.0.0 the "height" value returned by GetOutputFormat() reflected the underlying 
     * decoder's height, which is not exactly relevant to the client and could result in values that are too large
     * for most purposes.
     * For example, with 720p content, "height" would correctly report 720. But with 1080p content, "height" 
     * would report 1088 (it should be 1080), which is too large for certain purposes like the sizing of buffers
     * and textures, calculating the span of blit operations, and so on.
     * This value is also inconsistent with the "crop" rect returned by GetOutputFormat(), which in the case of 
     * 1080p content is {0,0,1919,1079} and from which a height of 1080 can be derived (1079 - 0 + 1).
     * It is also inconsistent with the "height" value returned by the GetOutputBufferFormat() API, which correctly 
     * returns 1080 with 1080p content.
     * GetOutputFormat() now returns a height that is computed directly from the "crop" rect dimensions.
     */
    virtual Status GetOutputFormat(MediaData* format) NN_NOEXCEPT;

    /**
     * @brief Start decoder.
     *
     * @return ::movie::Status
     * @retval ::Status_Success          
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre
     * - Decoder is configured.
     * @post 
     * - Decoder starts sending empty input buffer available events.
     * - Decoder starts sending filled output buffer available events.
     * - Decoder may send error and format changed events.
     * 
     * @details 
     * Decoder will start sending input buffer available events to client. Client needs to get empty buffers from
     * decoder and fill them with compressed media data. The filled buffers are sent to decoder for decoding. Once 
     * decoded buffers are available, decoder sends an output buffer available event. Client need to get filled 
     * output buffers and consume them. Client has to release the buffers once they are consumed. 
     */
    virtual Status Start() NN_NOEXCEPT;

    /**
     * @brief Stop decoder.
     *
     * @return ::movie::Status
     * @retval ::Status_Success
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre
     * - Decoder is started.
     * @post 
     * - Decoder stops (decoding stops).
     * 
     * @details 
     * Decoder will stop decoding. Decoder stops sending buffer availability events. Client can still send buffers to 
     * the decoder (previously retrieved from decoder).
     */
    virtual Status Stop() NN_NOEXCEPT;

    /**
     * @brief Reset decoder to created status.
     *
     * @return ::movie::Status
     * @retval ::Status_Success  
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre
     * - Decoder is created.
     * @post 
     * - Decoder internal state is reset.
     * 
     * @details 
     * Decoder is reset and can be configured again. Any buffer indices obtained earlier are invalid and should be
     * discarded.
     */
    virtual Status Reset() NN_NOEXCEPT;

    /**
     * @brief Decoder input and output buffers are flushed.
     *
     * @return ::movie::Status 
     * @retval ::Status_Success
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre
     * - Decoder is created.
     * @post 
     * - Decoder is flushed. Internal buffers are returned to the allocators.
     * 
     * @details 
     * Decoder is flushed and buffers used by decoders are sent back to allocators.
     * 
     */
    virtual Status Flush() NN_NOEXCEPT;

    /**
     * @brief Acquire input buffer index from the decoder.
     *
     * @param[out] index                    Available input buffer index
     * @param[out] remainingBufferIndices   Remaining buffer indices available
     *
     * @return ::movie::Status 
     * @retval ::Status_Success  
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre
     * - Decoder is started.
     * - Input buffer available event is received.
     * @post 
     * - Available input buffer index and number of remaining input buffer indices are available.
     * 
     * @details 
     * Client can use this API to get an available input buffer index after receiving the input buffer available event
     * from the decoder. This API also returns remaining buffer indices available. Client can get all available 
     * indices in a loop. 
     */
    virtual Status AcquireInputBufferIndex(int32_t* index, int32_t* remainingBufferIndices) NN_NOEXCEPT;

    /**
     * @brief Get input buffer from the decoder.
     *
     * @param[in] index                     Input buffer index
     * @param[out] buffer                   Buffer info associated with index
     *
     * @return ::movie::Status 
     * @retval ::Status_Success  
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre
     * - Decoder is started.
     * - Input buffer available event is received.
     * - Input buffer index in retrieved successfully.
     * @post 
     * - Client gets input buffer.
     * 
     * @details 
     * Client can use this API to get an available input buffer after getting a buffer index. Client can get all available 
     * input buffers in a loop. Client can fill the buffer with compressed media data for decoding.
     */
    virtual Status GetInputBuffer(int32_t index, Buffer* buffer) NN_NOEXCEPT;

    /**
     * @brief Send input buffer to the decoder for decoding media data.
     *
     * @param[in] index                     Input buffer index
     * @param[in] offset                    Data offset(0 or any offset to valid data)
     * @param[in] size                      Size of valid data in the buffer
     * @param[in] presentationTimeUs        Presentation time associated with this data
     * @param[in] flags                     Buffer flags
     *
     * @return ::movie::Status 
     * @retval ::Status_Success  
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre
     * - Decoder is started.
     * - Input buffer is retrieved from decoder.
     * @post 
     * - Filled input buffer is sent to decoder.
     * 
     * @details 
     * Client can use this API to send filled input buffer to decoder. Client can send all available 
     * input buffers in a loop. Client can fill the buffer with compressed media data for decoding.
     */
    virtual Status SendInputBufferForDecode(int32_t index, int32_t offset, int32_t size, int64_t presentationTimeUs, uint32_t flags) NN_NOEXCEPT;

    /**
     * @brief Acquire output buffer index from the decoder.
     *
     * @param[out] index                    Output buffer index
     * @param[out] presentationTimeUs       Presentation time associated with this index
     * @param[out] flags                    Buffer flags associated with this index
     * @param[out] remainingBufferIndices   Remaining buffer indices available
     *
     * @return ::movie::Status 
     * @retval ::Status_Success  
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre
     * - Decoder is started.
     * - Output buffer available event is received.
     * @post 
     * - Available output buffer index and number of remaining output buffer indices are available.
     * 
     * @details 
     * Client can use this API to get an available output buffer index after receiving a buffer available event
     * from the decoder. This API also returns remaining buffer indices available. Client can get all available 
     * indices in a loop. 
     */
    virtual Status AcquireOutputBufferIndex(int32_t* index, int64_t* presentationTimeUs, uint32_t* flags, int32_t* remainingBufferIndices) NN_NOEXCEPT;

    /**
     * @brief Get output buffer size for current track.
     *
     * @param[out] bufferSize               Output buffer size in bytes for current decoding session.
     * @param[out] alignment                Output buffer alignment in bytes.
     *
     * @return ::movie::Status 
     * @retval ::Status_Success  
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre
     * - Decoder is created and configured successfully.    
     * 
     * @details 
     * Application can use this API to get output buffer size and alignment. The buffer size can change after format change.
     * When video decoder is created, if movie::DecoderMode is set to movie::DecoderMode_NativeTexture, application has to take 
     * care of texture alignment. Setting NVN_TEXTURE_FLAGS_VIDEO_DECODE_BIT flag to NVN texture builder creates correct
     * alignment needed by decoder.
     *
     * NVNtextureBuilder builder;
     * nvnTextureBuilderSetFlags(&builder, NVN_TEXTURE_FLAGS_VIDEO_DECODE_BIT);
     *
     */
    virtual Status GetOutputBufferSize(int32_t* bufferSize, int32_t* alignment) NN_NOEXCEPT;

    /**
     * @brief Get output buffer from the decoder.
     *
     * @param[in] index                     Output buffer index
     * @param[in] buffer                    Client provided buffer to copy output data.
     *
     * @return ::movie::Status 
     * @retval ::Status_Success  
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre     
     * - Decoder is started.
     * - Output buffer available event is received.
     * - Output buffer index is retrieved successfully.
     * @post 
     * - Client gets output buffer.
     * 
     * @details 
     * Client can use this API to get an available output buffer after getting a buffer index. Client can get all available 
     * output buffers in a loop. Client can consume decoded data from the buffer.
     */
    virtual Status GetOutputBuffer(int32_t index, Buffer* buffer) NN_NOEXCEPT;
    /**
     * @brief Release output buffer.
     *
     * @param[in] index                     Output buffer index to release
     *
     * @return ::movie::Status 
     * @retval ::Status_Success  
     * @retval Other    (Failure) See ::movie::Status for details.
     *
     * @pre
     * - Decoder is started.
     * - Output buffer is retrieved successfully.
     * @post 
     * - Output buffer is sent to decoder.
     * - Data in output buffer is invalid.
     * 
     * @details 
     * Client can use this API to release an output buffer back to decoder. Once released, the data the buffer points to 
     * is invalid. Once decoded data is consumed, client has to call this API to release buffer.
     */
    virtual Status ReleaseOutputBufferIndex(int32_t index) NN_NOEXCEPT;

    /**
     * @brief Returns the last error (if any) stored by the decoder just prior to signalling a client ErrorEvent.     
     *
     * @pre
     * - Decoder is created.
     * @post 
     * - Last decoder error is retrieved.
     * 
     * @details 
     * When the decoder encounters an error it stores the error code internally and signals to the client that an error has occurred. 
     * Since this signal does not include details of the error, the client can use this API to query the exact error code from the decoder.
     */
    virtual Status GetLastError() NN_NOEXCEPT;

    /**
    * @brief Get output buffer from the decoder.
    *
    * @param[in] index                     Output buffer index
    * @param[in] buffer                    Client provided buffer to copy output data.
    * @param[in] format                    Decoder output buffer properties
    *
    * @return ::movie::Status
    * @retval ::Status_Success   
    * @retval Other    (Failure) See ::movie::Status for details.
    *
    * @pre
    * - Decoder is started.
    * - Output buffer available event is received.
    * - Output buffer index is retrieved successfully.
    * @post
    * - Client gets output buffer.
    *
    * @details
    * Client can use this API to get an available output buffer after getting a buffer index. Client can get all available
    * output buffers in a loop. Client can consume decoded data from the buffer.
    * Refer to the GetOutputBufferFormat() API documentation for "format" details.
    */
    virtual Status GetOutputBuffer(int32_t index, Buffer* buffer, MediaData* format) NN_NOEXCEPT;

    /**
    * @brief Get decoder output buffer properties.
    *
    * @param[in] index                     Output buffer index
    * @param[in] format                    Decoder output buffer properties
    *
    * @return ::movie::Status
    * @retval ::Status_Success  
    * @retval Other    (Failure) See ::movie::Status for details.
    *
    * @pre
    * - Decoder is started
    *
    * @details
    * Decoder output format is available in [key, value] pairs using the MediaData structure.
    * All the information may not be available for a given decoder. <br>
    * Query output buffer format using movie::MediaData: 
    *  - "width"          - Video width in number of lines, data type -> int32_t
    *  - "height"         - Video height in number of lines, data type -> int32_t
    *  - "crop"           - Video crop information, data type -> int32_t (see note below)
    *  - "is-sync-frame"  - A sync frame, data type -> int32_t
    *  - "channel-count"  - Number of audio channels, data type -> int32_t
    *  - "sample-rate"    - Audio sample rate in Hertz, data type -> int32_t
    *  - "nv12-y-offset"  - Offset for Y-data from start of the output buffer address, data type -> int32_t
    *  - "nv12-uv-offset" - Offset for UV-data from start of the output buffer address, data type -> int32_t
    *  - "nv12-y-stride"  -  Stride for Y-rows, data type -> int32_t
    *  - "nv12-uv-stride" - Stride for UV-rows, data type -> int32_t </p>
    *
    * Example:
    * @code
    * movie::MediaData format;
    * int32_t index = 0;
    * GetOutputBufferFormat(index, &format);
    *
    * int32_t cropLeft, cropTop, cropRight, cropBottom;
    * format.FindRect("crop", &cropLeft, &cropTop, &cropRight, &cropBottom);
    *
    * int32_t nv12YOffset = 0;
    * format.FindInt32("nv12-y-offset", &nv12YOffset);
    * @endcode
    *
    * @note
    * Prior to NintendoSDK 6.0.0 the "crop" entry of the movie::MediaData instance filled by GetOutputBufferFormat()
    * would return a rect with values corresponding to {left, top, total_width, total_height} instead of 
    * {left, top, right, bottom}. For example, if the content being decoded was 1080p (resolution 1920x1080), 
    * the "crop" rect would contain {0, 0, 1920, 1080}. This was inconsistent with the "crop" entry returned 
    * by the alternative GetOutputFormat() API, which returns values corresponding to {left, top, right, bottom} 
    * (and in the case of 1920x1080 content, {0, 0, 1919, 1079}). The "crop" rect entry that is now returned by both 
    * GetOutputBufferFormat() and GetOutputFormat() contains coordinates that correspond to {left, top, right, bottom}.  
    */
    virtual Status GetOutputBufferFormat(int32_t index, MediaData* format) NN_NOEXCEPT;

private:
    void* m_pDecoderImpl;    
};

}  // namespace movie
