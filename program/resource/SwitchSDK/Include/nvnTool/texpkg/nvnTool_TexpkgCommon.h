/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_TexpkgCommon.h
///       Common definitions used by the NVN texture packager tool.

#ifndef NVNTOOL_TEXPKGCOMMON_H
#define NVNTOOL_TEXPKGCOMMON_H

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <wchar.h>
#include <nvn/nvn.h>
#include <new>
#include <string>

#define NVN_TEXPKG_VERSION_MAJOR 7
#define NVN_TEXPKG_VERSION_MINOR 1

#ifdef _DEBUG
    #define dprintf(x, ...) TPPrintLog(__FUNCTION__, x, __VA_ARGS__)
    #define dvprintf(...)
    #define dwprintf(x, ...) TPPrintLog(__FUNCTION__, x, __VA_ARGS__)
#else
    #define dprintf(x, ...) TPPrintLog(NULL, x, __VA_ARGS__)
    #define dvprintf(...)
    #define dwprintf(x, ...) TPPrintLog(NULL, x, __VA_ARGS__)
#endif

#ifdef _MSC_VER
    #ifndef CDECL_API
        #define CDECL_API __cdecl
    #endif
    #ifndef DLL_EXPORT
        #define DLL_EXPORT   __declspec( dllexport )
    #endif
    #ifndef DLL_IMPORT
        #define DLL_IMPORT   __declspec( dllimport )
    #endif
#else
    #ifndef CDECL_API
        #define CDECL_API
    #endif
    #ifndef DLL_EXPORT
        #define DLL_EXPORT
    #endif
    #ifndef DLL_IMPORT
        #define DLL_IMPORT
    #endif

    // TODO: perform wchar conversions on non-Windows platforms.
    //#define wprintf(x, ...) printf((const char*) x, __VA_ARGS__)
    #error "Platform not supported."

#endif

#ifdef TEXPKG_BUILD_DLL
    #define DLL_API DLL_EXPORT
    #define DLL_MODE /* TODO: remove this */
#else
    #define DLL_API
#endif

#if defined(__GNUC__)
    #define TP_PACKED_START
    #define TP_PACKED_END
    #define TP_PACKED __attribute__((packed))
    #define TP_PACKED1 __attribute__((aligned(1)))
#elif defined(_MSC_VER)
    #define TP_PACKED_START #pragma pack(push,1)
    #define TP_PACKED_END #pragma pack(pop)
    #define TP_PACKED
    #define TP_PACKED1
#else
    #error "Unsupported compiler."
#endif

enum TPError {
    TP_OK = 0,
    TP_ERROR_UNKNOWN = -1,
    TP_INVALID_FORMAT = -2,
    TP_UNSUPPORTED_FORMAT = -3,
    TP_MISSING_FILE = -4,
    TP_NOMEM = -5,
    TP_INVALID_ARGUMENT = -6,
    TP_UNSUPPORTED_TARGET = -7,
    TP_INVALID_IMAGE_WIDTH = -8,
    TP_INVALID_IMAGE_HEIGHT = -9,
    TP_INVALID_IMAGE_DEPTH = -10,
    TP_INVALID_IMAGE_SUBIMAGES = -11
};

inline const char* TPErrorToStr(int err)
{
#define TP_A(e) case e: return #e;
    switch (err) {
    TP_A(TP_OK)
    TP_A(TP_ERROR_UNKNOWN)
    TP_A(TP_INVALID_FORMAT)
    TP_A(TP_UNSUPPORTED_FORMAT)
    TP_A(TP_MISSING_FILE)
    TP_A(TP_NOMEM)
    TP_A(TP_INVALID_ARGUMENT)
    TP_A(TP_UNSUPPORTED_TARGET)
    TP_A(TP_INVALID_IMAGE_WIDTH)
    TP_A(TP_INVALID_IMAGE_HEIGHT)
    TP_A(TP_INVALID_IMAGE_DEPTH)
    TP_A(TP_INVALID_IMAGE_SUBIMAGES)
    default: break;
    }
#undef TP_A
    return "Unknown error code";
}

inline int TPNextMipMapDim(int x) {
    return (x > 1) ? (x >> 1) : 1;
}

inline void TPPrintLog(const char* func, const char* fmt, ...)
{
    if (func) {
        fprintf(stderr, "%s: ", func);
    }
    va_list argptr;
    va_start(argptr, fmt);
    vfprintf(stderr, fmt, argptr);
    va_end(argptr);
}

inline void TPPrintLog(const char* func, const wchar_t* fmt, ...)
{
    if (func) {
        fprintf(stderr, "%s: ", func);
    }
    va_list argptr;
    va_start(argptr, fmt);
    vfwprintf(stderr, fmt, argptr);
    va_end(argptr);
}

#define TP_DLL_EXPORTED_FUNC(RET, FNAME, ARGS, STRDEF) \
    DLL_API RET FNAME ARGS; \
    typedef RET (*FNAME##Func) ARGS; \
    static const char* TEXPKG_##STRDEF = #FNAME;

#ifndef _M_IX86
    #define NVN_IMAGE_DLL_FILENAME L"NvnImage.dll"
    #define NVN_TEXPKG_DLL_FILENAME L"NvnTexpkg.dll"
#else
    #define NVN_IMAGE_DLL_FILENAME L"NvnImage32.dll"
    #define NVN_TEXPKG_DLL_FILENAME L"NvnTexpkg32.dll"
#endif

// Load function entry points.
#ifndef TEXPKG_STATIC
    #define TEXPKG_LOAD_ENTRY(LIB, FN, TYPE, STR) \
        LIB->FN = reinterpret_cast<TYPE##Func>(\
                GetProcAddress(LIB->module, TEXPKG_##STR)\
        );\
        if (!LIB->FN) {\
            dprintf(__FUNCTION__ " could not load entry %s.\n", TEXPKG_##STR);\
            return false;\
        }
#else
    #define TEXPKG_LOAD_ENTRY(LIB, FN, TYPE, STR) LIB->FN = TYPE;
#endif

#endif // NVNTOOL_TEXPKGCOMMON_H
