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

/*!
    @file
    @brief <Windows.h> を安全にインクルードするためのヘッダーファイル
    @details <Windows.h> の代わりにインクルードして使用します。
*/
#ifndef WIN32_LEAN_AND_MEAN
# pragma message(__FILE__ ": WIN32_LEAN_AND_MEAN not defined. Add \"#define WIN32_LEAN_AND_MEAN\" before \"#include <nn/nn_Windows.h>\"")
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
# pragma message(__FILE__ ": NOMINMAX not defined. Add \"#define NOMINMAX\" before \"#include <nn/nn_Windows.h>\"")
#define NOMINMAX
#endif

#include <Windows.h>
#include <nn/nn_Macro.h>

//! @cond
namespace nn { namespace detailWinProxy {

#if defined( GetCharWidth ) // NOLINT
#undef GetCharWidth
NN_FORCEINLINE
BOOL
WINAPI
GetCharWidth(
    HDC hdc,
    UINT iFirst,
    UINT iLast,
    LPINT lpBuffer
    )
{
#ifdef UNICODE // NOLINT
    return GetCharWidthW(
#else
    return GetCharWidthA(
#endif
        hdc,
        iFirst,
        iLast,
        lpBuffer
        );
}
#endif // defined( GetCharWidth )

#if defined( CreateDirectory ) // NOLINT
#undef CreateDirectory
NN_FORCEINLINE
BOOL
WINAPI
CreateDirectory(
    LPCTSTR lpPathName,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes
    )
{
#ifdef UNICODE // NOLINT
    return CreateDirectoryW(
#else
    return CreateDirectoryA(
#endif
        lpPathName,
        lpSecurityAttributes
        );
}
#endif // defined( CreateDirectory )


#if defined( CreateFile ) // NOLINT
#undef CreateFile
NN_FORCEINLINE
HANDLE
WINAPI
CreateFile(
    LPCTSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
    )
{
#ifdef UNICODE // NOLINT
    return CreateFileW(
#else
    return CreateFileA(
#endif
        lpFileName,
        dwDesiredAccess,
        dwShareMode,
        lpSecurityAttributes,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        hTemplateFile
        );
}
#endif // defined( CreateFile )


#if defined( DeleteFile ) // NOLINT
#undef DeleteFile
NN_FORCEINLINE
BOOL
WINAPI
DeleteFile(
    LPCTSTR lpFileName
    )
{
#ifdef UNICODE // NOLINT
    return DeleteFileW(
#else
    return DeleteFileA(
#endif
        lpFileName
        );
}
#endif // defined( CreateFile )

#if defined( CopyFile ) // NOLINT
#undef CopyFile
NN_FORCEINLINE
BOOL
WINAPI
CopyFile(
    LPCTSTR lpExistingFileName,
    LPCTSTR lpNewFileName,
    BOOL bFailIfExists
    )
{
#ifdef UNICODE // NOLINT
    return CopyFileW(
#else
    return CopyFileA(
#endif
        lpExistingFileName,
        lpNewFileName,
        bFailIfExists
        );
}
#endif // defined( CopyFile )

#if defined( CreateWindow ) // NOLINT
#undef CreateWindow
NN_FORCEINLINE
HWND
WINAPI
CreateWindow(
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName,
    DWORD dwStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam )
{
#ifdef UNICODE // NOLINT
    return CreateWindowExW(
#else
    return CreateWindowExA(
#endif
        0L,
        lpClassName,
        lpWindowName,
        dwStyle,
        x,
        y,
        nWidth,
        nHeight,
        hWndParent,
        hMenu,
        hInstance,
        lpParam );
}
#endif // defined( CreateWindow )

#if defined( FindWindow ) // NOLINT
#undef FindWindow
NN_FORCEINLINE
HWND
WINAPI
FindWindow(
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName )
{
#ifdef UNICODE // NOLINT
    return FindWindowW(
#else
    return FindWindowA(
#endif
        lpClassName,
        lpWindowName );
}
#endif // defined( FindWindow )

#if defined( GetEnvironmentVariable ) // NOLINT
#undef GetEnvironmentVariable
NN_FORCEINLINE
DWORD
WINAPI
GetEnvironmentVariable(
    LPCTSTR lpName,
    LPTSTR lpBuffer,
    DWORD nSize
    )
{
#ifdef UNICODE
    return GetEnvironmentVariableW(
#else
    return GetEnvironmentVariableA(
#endif // !UNICODE
        lpName,
        lpBuffer,
        nSize );
}
#endif // defined( GetEnvironmentVariable )

#if defined( GetCurrentTime ) // NOLINT
#undef GetCurrentTime
NN_FORCEINLINE
DWORD
WINAPI
GetCurrentTime()
{
    return GetTickCount();
}
#endif // defined( GetCurrentTime )

#if defined( GetUserName ) // NOLINT
#undef GetUserName
NN_FORCEINLINE
BOOL
WINAPI
GetUserName(
    LPTSTR lpBuffer,
    LPDWORD nSize )
{
#ifdef UNICODE
    return GetUserNameW(
#else
    return GetUserNameA(
#endif // !UNICODE
        lpBuffer,
        nSize );
}
#endif // defined( GetUserName )

#if defined( LoadImage ) // NOLINT
#undef LoadImage

NN_FORCEINLINE
HANDLE
WINAPI
LoadImage(
    _In_opt_ HINSTANCE hInst,
#ifdef UNICODE
    _In_ LPCWSTR name,
#else
    _In_ LPCSTR name,
#endif // !UNICODE
    _In_ UINT type,
    _In_ int cx,
    _In_ int cy,
    _In_ UINT fuLoad)
{
#ifdef UNICODE
    return LoadImageW(
#else
    return LoadImageA(
#endif // !UNICODE
        hInst,
        name,
        type,
        cx,
        cy,
        fuLoad);
}
#endif // defined( LoadImage )

#if defined( GetObject )
#undef GetObject

NN_FORCEINLINE
int
GetObject(
    HANDLE h,
    int c,
    LPVOID pv
    )
{
#ifdef UNICODE
    return GetObjectW(
#else
    return GetObjectA(
#endif
        h,
        c,
        pv
        );
}
#endif // defined( GetObject )

#if defined( SendMessage )
#undef SendMessage

NN_FORCEINLINE
LRESULT
WINAPI
SendMessage(
    HWND hWnd,
    UINT Msg,
    WPARAM wParam,
    LPARAM lParam)
{
#ifdef UNICODE
    return SendMessageW(
#else
    return SendMessageA(
#endif
        hWnd,
        Msg,
        wParam,
        lParam);
}
#endif // defined( SendMessage )

}} // namespace nn::detailWinProxy
//! @endcond

// 同様のプロクシ関数が定義されていても ODR 違反とならないよう名前空間内に定義したものを使用します。
using namespace nn::detailWinProxy;
