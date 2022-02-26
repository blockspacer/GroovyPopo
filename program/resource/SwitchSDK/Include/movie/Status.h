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
 * @file Status.h
 * @brief Movie error codes
 */

#pragma once



namespace movie {

/**
 * @brief
 * Enumeration of various status and error codes used throughout the ::movie library.
 * @details
 * The ::movie::Status enum (defined in @ref Status.h "movie/Status.h") is used to return the status of operations 
 * in the ::movie library. Generally, a ::Status_Success result indicates success and any other result indicates 
 * failure, but users are advised to consult specific API documentation for details. 
 */

enum Status
{
    Status_Success                  = 0x0000,           //!< Success
    Status_UnknownError             = 0x1000,           //!< A generic error is encountered
    Status_InvalidRange             = 0x1001,           //!< The argument is out of allowed range
    Status_OutOfMemory              = 0x1002,           //!< The system does not have enough memory
    Status_OperationFailed          = 0x1003,           //!< The requested operation failed
    Status_OperationDenied          = 0x1004,           //!< System policy has prevented the operation
    Status_BufferTooSmall           = 0x1005,           //!< The buffer is too small to hold data
    Status_NotSupported             = 0x1006,           //!< The platform cannot support the request
    Status_NotImplemented           = 0x1007,           //!< The feature is not implemented
    Status_NotFound                 = 0x1008,           //!< The search returned no results
    Status_IoError                  = 0x1009,           //!< System Input or Output error occurred
    Status_ErrorTimeOut             = 0x100a,           //!< Request timed out
    Status_ErrorBadValue            = 0x100b,           //!< The parameter has bad value
    Status_FailedToConfigure        = 0x100c,           //!< Failed to configure player or decoder
    Status_FailedToCreateThread     = 0x100d,           //!< Failed to create thread
    Status_OutputBufferNotAvailable = 0x100e,           //!< No outputbuffers available
    Status_InvalidStateTransition   = 0x100f,           //!< The state transition is illegal and not allowed
    Status_OutputBuffersChanged     = 0x1010,           //!< Output buffers changed
    Status_InputBufferNotAvailable  = 0x1011,           //!< Empty input buffers are not available.

    Status_EndOfStream              = 0x2000,           //!< End of stream is reached
    Status_InvalidConfiguration     = 0x2001,           //!< The configuration is invalid
    Status_FailedToStart            = 0x2002,           //!< Player or decoder start failed
    Status_FailedToStop             = 0x2003,           //!< Player or decoder stop failed
    Status_NullDecoder              = 0x2004,           //!< No decoder is created
    Status_FailedToInitialize       = 0x2005,           //!< Player or decoder initialize failed
    Status_FailedToPrepare          = 0x2006,           //!< Failed to prepare Player or decoder
    Status_FailedToCancelSetDataSource = 0x2007,        //!< Failed to cancel SetDataSource
    Status_SetDataSourceCancelled   = 0x2008,           //!< SetDataSource cancelled by client
    Status_PauseNotAllowed          = 0x2009,           //!< Pausing not allowed due to end of stream(EOS) processing
    Status_SeekNotAllowed           = 0x200a,           //!< Seeking not allowed due to end of stream(EOS) processing

    Status_TrackNotFound            = 0x3000,           //!< The requested track is not found in the media file
    Status_UnknownTrackType         = 0x3001,           //!< The track type is not known to extractor
    Status_InvalidSequence          = 0x3002,           //!< The operation sequence is wrong
    Status_NullExtractor            = 0x3003,           //!< Extractor is not created

    Status_UnsupportedMediaType     = 0x4000,           //!< Given media type is not supported
    Status_UnsupportedSampleRate    = 0x4001,           //!< Given audio sample rate is not supported
    Status_UnsupportedBitRate       = 0x4002,           //!< Given audio or video bit rate is not supported
    Status_UnsupportedProfile       = 0x4003,           //!< Audio or video profile is not supported
    Status_FormatChanged            = 0x4004,           //!< Audio or video format has been changed
    Status_InvalidDimensions        = 0x4005,           //!< The platform is unable to support the requested video dimensions

    Status_NotConnected             = 0x5000,           //!< There is no connection
    Status_AlreadyConnected         = 0x5001,           //!< Already connected to the service
    Status_UnknownHost              = 0x5002,           //!< The host is unknown
    Status_NotEnoughData            = 0x5003,           //!< The data is insufficient
    Status_Malformed                = 0x5004,           //!< The data is malformed
    Status_Discontinuity            = 0x5005,           //!< Data discontinuity is observed
    Status_CanNotConnect            = 0x5006,           //!< Not able to connect
    Status_ConnectionLost           = 0x5007,           //!< Connection lost
    Status_OutOfRange               = 0x5008,           //!< Out of range

    Status_LastError                = 0xffff,           //!< Last error in the list
};


/** 
 * @brief 
 * Return the string corresponding to status enum 
 *
 * @param[in]  status             status enum value
 *
 * @retval char*                  String corresponding to status enum
 *
 * @details 
 * The string value of the status enumeration is returned for the client. This can be used for debug logging.
 */
const char* StatusToString(Status status);

}
