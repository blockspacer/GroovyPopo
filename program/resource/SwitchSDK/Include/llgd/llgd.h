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

#include <nvn/nvn.h>

#define LLGDAPI extern "C"

//--------------------------------------------------------------------------
// LlgdApiError
//--------------------------------------------------------------------------
enum LlgdApiError
{
    LlgdApiError_None,
    LlgdApiError_NotConnected,
    LlgdApiError_InvalidState,
    LlgdApiError_InvalidParameter,
    LlgdApiError_DevtoolsAllocator,
};

//--------------------------------------------------------------------------
// llgdIsConnected
//
// Returns true if the host has connected to the target.
//--------------------------------------------------------------------------
LLGDAPI bool llgdIsConnected();

//--------------------------------------------------------------------------
// llgdBeginCapture
//
// Begins capturing.  Host initiated captures operate on Present boundaries.
// Target API initiated captures operate on arbitrary boundaries.  Call
// EndCapture to end capturing after calling BeginCapture.
//
// Will return an error if the host is not connected, or if this function
// is called while already capturing.
//--------------------------------------------------------------------------
LLGDAPI LlgdApiError llgdBeginCapture();

//--------------------------------------------------------------------------
// llgdEndCapture
//
// Ends capturing.  Host initiated captures operate on Present boundaries.
// Target API initiated captures operate on arbitrary boundaries.
// EndCapture should be called after calling BeginCapture to end a capture.
//
// Will return an error if the host is not connected, or if this function
// is called without first calling BeginCapture.
//--------------------------------------------------------------------------
LLGDAPI LlgdApiError llgdEndCapture();

//--------------------------------------------------------------------------
// LlgdBindlessFlag
//--------------------------------------------------------------------------
enum LlgdBindlessFlag
{
    LlgdBindlessFlag_Read =                 1 << 0,
    LlgdBindlessFlag_Write =                1 << 1,
};

//--------------------------------------------------------------------------
// llgdRegisterBindlessTexture
//
// Registers a bindless texture with LLGD.  LLGD will only scan registered
// bindless resources for memory reads / writes during capture.
//
// Will return an error if the resource has previously been registered or
// if the parameters are invalid.
//
// WARNING: llgdRegisterBindlessTexture requires the application to have
// set the devtools allocator.
//
// WARNING: This API is not thread-safe.  The application is responsible
// for ensuring that only a single thread calls any of the bindless
// APIs at a time.
//--------------------------------------------------------------------------
LLGDAPI LlgdApiError llgdRegisterBindlessTexture(const NVNtexture* texture, int flags);

//--------------------------------------------------------------------------
// llgdUnregisterBindlessTexture
//
// Unregisters a previously registered texture with LLGD.  LLGD will no
// longer scan the unregistered resource for memory reads / writes during
// capture.
//
// Will return an error if the resource has not previously been registered
// of if the parameters are invalid.
//
// WARNING: This API is not thread-safe.  The application is responsible
// for ensuring that only a single thread calls any of the bindless
// APIs at a time.
//--------------------------------------------------------------------------
LLGDAPI LlgdApiError llgdUnregisterBindlessTexture(const NVNtexture* texture);

//--------------------------------------------------------------------------
// llgdRegisterBindlessBufferRange
//
// Registers a bindless buffer range with LLGD.  LLGD will only scan
// registered bindless resources for memory reads / writes during capture.
//
// Will return an error if the resource has previously been registered or
// if the parameters are invalid.
//
// WARNING: llgdRegisterBindlessTexture requires the application to have
// set the devtools allocator.
//
// WARNING: This API is not thread-safe.  The application is responsible
// for ensuring that only a single thread calls any of the bindless
// APIs at a time.
//--------------------------------------------------------------------------
LLGDAPI LlgdApiError llgdRegisterBindlessBufferRange(const NVNbufferRange* bufferRange, int flags);

//--------------------------------------------------------------------------
// llgdUnregisterBindlessBufferRange
//
// Unregisters a previously registered buffer range with LLGD.  LLGD will no
// longer scan the unregistered resource for memory reads / writes during
// capture.
//
// Will return an error if the resource has not previously been registered
// of if the parameters are invalid.
//
// WARNING: This API is not thread-safe.  The application is responsible
// for ensuring that only a single thread calls any of the bindless
// APIs at a time.
//--------------------------------------------------------------------------
LLGDAPI LlgdApiError llgdUnregisterBindlessBufferRange(const NVNbufferRange* bufferRange);