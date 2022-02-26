/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

// このファイルは自動生成ファイルです。
// 手動で変更しないでください。

// NOLINT(build/header_guard)

// package 'public'
//! @defresult{profiler,An argument was NULL.,150,1,1,ResultNullArgument}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNullArgument, 150, 1, 2);
//! @defresult{profiler,An argument was invalid.,150,2,2,ResultInvalidArgument}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidArgument, 150, 2, 3);
//! @defresult{profiler,A problem occurred with memory allocation.,150,3,3,ResultMemoryAllocationFailure}
NN_DEFINE_ERROR_RANGE_RESULT(ResultMemoryAllocationFailure, 150, 3, 4);
//! @defresult{profiler,The requested action has already been performed.,150,4,4,ResultAlreadyDone}
NN_DEFINE_ERROR_RANGE_RESULT(ResultAlreadyDone, 150, 4, 5);
//! @defresult{profiler,The profiler skipped recording the data.,150,5,5,ResultSkippedRecord}
NN_DEFINE_ERROR_RANGE_RESULT(ResultSkippedRecord, 150, 5, 6);
//! @defresult{profiler,The profiler is not initialized.,150,6,6,ResultNotInitialized}
NN_DEFINE_ERROR_RANGE_RESULT(ResultNotInitialized, 150, 6, 7);
//! @defresult{profiler,Attempting to perform this operation is invalid with the current status.,150,7,7,ResultInvalidProfilerStatus}
NN_DEFINE_ERROR_RANGE_RESULT(ResultInvalidProfilerStatus, 150, 7, 8);
//! @defresult{profiler,This operation requires that the graphics hooks be installed.,150,8,8,ResultGraphicsHooksNotInstalled}
NN_DEFINE_ERROR_RANGE_RESULT(ResultGraphicsHooksNotInstalled, 150, 8, 9);
//! @defresult{profiler,Could not attach to the target application.,150,540,549,ResultCouldNotAttach}
NN_DEFINE_ERROR_RANGE_RESULT(ResultCouldNotAttach, 150, 540, 550);
    //! @defresult{profiler,A debugger is already attached to the process.,150,541,541,ResultDebuggerAttached\, ResultCouldNotAttach}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultDebuggerAttached, 150, 541, 542);
    //! @defresult{profiler,Out of process profiling is unavailable in 4GB memory mode.,150,543,543,ResultOutOfProcessUnavailable\, ResultCouldNotAttach}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultOutOfProcessUnavailable, 150, 543, 544);
//! @defresult{profiler,An unknown error occurred.,150,1023,1023,ResultUnknownError}
NN_DEFINE_ERROR_RANGE_RESULT(ResultUnknownError, 150, 1023, 1024);
