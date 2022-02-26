#ifndef __nvn_OldTypes_h_
#define __nvn_OldTypes_h_

/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvn_OldTypes.h
///     Definitions of basic types that were found in previous versions of the
///     NVN API, but have been removed.  This header is provided for backwards
///     compatibility for applications that may still contain code using the
///     older types.

#include <stddef.h>

// Clients using older MSVC versions without support for stdint.h need
// to supply their own versions.
#if (defined(_MSC_VER) && (_MSC_VER >= 1600)) || defined(__GNUC__)
#include <stdint.h>
#endif

typedef uint32_t NVNbitfield;
typedef int NVNint;
typedef int NVNsizei;
typedef uint32_t NVNuint;
typedef float NVNfloat;
typedef uintptr_t NVNuintptr;
typedef intptr_t NVNsizeiptr;
typedef int64_t NVNint64;
typedef uint64_t NVNuint64;
typedef const char * NVNstring;

typedef int8_t NVNbyte;
typedef int16_t NVNshort;
typedef uint8_t NVNubyte;
typedef uint16_t NVNushort;
typedef double NVNdouble;
typedef char NVNchar;
typedef intptr_t NVNintptr;
typedef void NVNvoid;

#endif // #ifndef __nvn_OldTypes_h_
