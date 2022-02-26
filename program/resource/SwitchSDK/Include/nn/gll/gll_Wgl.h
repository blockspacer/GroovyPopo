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
* @file gll_Wgl.h
*/

#pragma once

#include <stddef.h>

#include <nn/nn_Macro.h>

#include <nn/nn_Windows.h>
#include <nn/gll/gll_Common.h>
#include <nn/gll/gll_Gl.h>

#ifdef __cplusplus
extern "C" {
#endif

NN_GLL_API nngllResult nngllInitializeWgl() NN_NOEXCEPT;

#if !defined(WGL_VERSION_1_0)
#define WGL_VERSION_1_0

#define WGL_FONT_LINES 0 // NOLINT
#define WGL_FONT_POLYGONS 1 // NOLINT
#define WGL_SWAP_MAIN_PLANE 0x00000001 // NOLINT
#define WGL_SWAP_OVERLAY1 0x00000002 // NOLINT
#define WGL_SWAP_OVERLAY2 0x00000004 // NOLINT
#define WGL_SWAP_OVERLAY3 0x00000008 // NOLINT
#define WGL_SWAP_OVERLAY4 0x00000010 // NOLINT
#define WGL_SWAP_OVERLAY5 0x00000020 // NOLINT
#define WGL_SWAP_OVERLAY6 0x00000040 // NOLINT
#define WGL_SWAP_OVERLAY7 0x00000080 // NOLINT
#define WGL_SWAP_OVERLAY8 0x00000100 // NOLINT
#define WGL_SWAP_OVERLAY9 0x00000200 // NOLINT
#define WGL_SWAP_OVERLAY10 0x00000400 // NOLINT
#define WGL_SWAP_OVERLAY11 0x00000800 // NOLINT
#define WGL_SWAP_OVERLAY12 0x00001000 // NOLINT
#define WGL_SWAP_OVERLAY13 0x00002000 // NOLINT
#define WGL_SWAP_OVERLAY14 0x00004000 // NOLINT
#define WGL_SWAP_OVERLAY15 0x00008000 // NOLINT
#define WGL_SWAP_UNDERLAY1 0x00010000 // NOLINT
#define WGL_SWAP_UNDERLAY2 0x00020000 // NOLINT
#define WGL_SWAP_UNDERLAY3 0x00040000 // NOLINT
#define WGL_SWAP_UNDERLAY4 0x00080000 // NOLINT
#define WGL_SWAP_UNDERLAY5 0x00100000 // NOLINT
#define WGL_SWAP_UNDERLAY6 0x00200000 // NOLINT
#define WGL_SWAP_UNDERLAY7 0x00400000 // NOLINT
#define WGL_SWAP_UNDERLAY8 0x00800000 // NOLINT
#define WGL_SWAP_UNDERLAY9 0x01000000 // NOLINT
#define WGL_SWAP_UNDERLAY10 0x02000000 // NOLINT
#define WGL_SWAP_UNDERLAY11 0x04000000 // NOLINT
#define WGL_SWAP_UNDERLAY12 0x08000000 // NOLINT
#define WGL_SWAP_UNDERLAY13 0x10000000 // NOLINT
#define WGL_SWAP_UNDERLAY14 0x20000000 // NOLINT
#define WGL_SWAP_UNDERLAY15 0x40000000 // NOLINT
typedef int ( WINAPI *ChoosePixelFormatType )( HDC hDc, const PIXELFORMATDESCRIPTOR *pPfd );
typedef int ( WINAPI *DescribePixelFormatType )( HDC hdc, int ipfd, UINT cjpfd, const PIXELFORMATDESCRIPTOR *ppfd );
typedef UINT ( WINAPI *GetEnhMetaFilePixelFormatType )( HENHMETAFILE hemf, const PIXELFORMATDESCRIPTOR *ppfd );
typedef int ( WINAPI *GetPixelFormatType )( HDC hdc );
typedef BOOL ( WINAPI *SetPixelFormatType )( HDC hdc, int ipfd, const PIXELFORMATDESCRIPTOR *ppfd );
typedef BOOL ( WINAPI *SwapBuffersType )( HDC hdc );
typedef BOOL ( WINAPI *wglCopyContextType )( HGLRC hglrcSrc, HGLRC hglrcDst, UINT mask );
typedef HGLRC ( WINAPI *wglCreateContextType )( HDC hDc );
typedef HGLRC ( WINAPI *wglCreateLayerContextType )( HDC hDc, int level );
typedef BOOL ( WINAPI *wglDeleteContextType )( HGLRC oldContext );
typedef BOOL ( WINAPI *wglDescribeLayerPlaneType )( HDC hDc, int pixelFormat, int layerPlane, UINT nBytes, const LAYERPLANEDESCRIPTOR *plpd );
typedef HGLRC ( WINAPI *wglGetCurrentContextType )(  );
typedef HDC ( WINAPI *wglGetCurrentDCType )(  );
typedef int ( WINAPI *wglGetLayerPaletteEntriesType )( HDC hdc, int iLayerPlane, int iStart, int cEntries, const COLORREF *pcr );
typedef PROC ( WINAPI *wglGetProcAddressType )( LPCSTR lpszProc );
typedef BOOL ( WINAPI *wglMakeCurrentType )( HDC hDc, HGLRC newContext );
typedef BOOL ( WINAPI *wglRealizeLayerPaletteType )( HDC hdc, int iLayerPlane, BOOL bRealize );
typedef int ( WINAPI *wglSetLayerPaletteEntriesType )( HDC hdc, int iLayerPlane, int iStart, int cEntries, const COLORREF *pcr );
typedef BOOL ( WINAPI *wglShareListsType )( HGLRC hrcSrvShare, HGLRC hrcSrvSource );
typedef BOOL ( WINAPI *wglSwapLayerBuffersType )( HDC hdc, UINT fuFlags );
typedef BOOL ( WINAPI *wglUseFontBitmapsType )( HDC hDC, DWORD first, DWORD count, DWORD listBase );
typedef BOOL ( WINAPI *wglUseFontBitmapsAType )( HDC hDC, DWORD first, DWORD count, DWORD listBase );
typedef BOOL ( WINAPI *wglUseFontBitmapsWType )( HDC hDC, DWORD first, DWORD count, DWORD listBase );
typedef BOOL ( WINAPI *wglUseFontOutlinesType )( HDC hDC, DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf );
typedef BOOL ( WINAPI *wglUseFontOutlinesAType )( HDC hDC, DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf );
typedef BOOL ( WINAPI *wglUseFontOutlinesWType )( HDC hDC, DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf );
#if 0
#define ChoosePixelFormat g_nnglloosePixelFormat
#define DescribePixelFormat g_nngllscribePixelFormat
#define GetEnhMetaFilePixelFormat g_nnglltEnhMetaFilePixelFormat
#define GetPixelFormat g_nnglltPixelFormat
#define SetPixelFormat g_nnglltPixelFormat
#define SwapBuffers g_nngllapBuffers
#define wglCopyContext g_nnglllCopyContext
#define wglCreateContext g_nnglllCreateContext
#define wglCreateLayerContext g_nnglllCreateLayerContext
#define wglDeleteContext g_nnglllDeleteContext
#define wglDescribeLayerPlane g_nnglllDescribeLayerPlane
#define wglGetCurrentContext g_nnglllGetCurrentContext
#define wglGetCurrentDC g_nnglllGetCurrentDC
#define wglGetLayerPaletteEntries g_nnglllGetLayerPaletteEntries
#define wglGetProcAddress g_nnglllGetProcAddress
#define wglMakeCurrent g_nnglllMakeCurrent
#define wglRealizeLayerPalette g_nnglllRealizeLayerPalette
#define wglSetLayerPaletteEntries g_nnglllSetLayerPaletteEntries
#define wglShareLists g_nnglllShareLists
#define wglSwapLayerBuffers g_nnglllSwapLayerBuffers
#define wglUseFontBitmaps g_nnglllUseFontBitmaps
#define wglUseFontBitmapsA g_nnglllUseFontBitmapsA
#define wglUseFontBitmapsW g_nnglllUseFontBitmapsW
#define wglUseFontOutlines g_nnglllUseFontOutlines
#define wglUseFontOutlinesA g_nnglllUseFontOutlinesA
#define wglUseFontOutlinesW g_nnglllUseFontOutlinesW
NN_GLL_API ChoosePixelFormatType g_nnglloosePixelFormat;
NN_GLL_API DescribePixelFormatType g_nngllscribePixelFormat;
NN_GLL_API GetEnhMetaFilePixelFormatType g_nnglltEnhMetaFilePixelFormat;
NN_GLL_API GetPixelFormatType g_nnglltPixelFormat;
NN_GLL_API SetPixelFormatType g_nnglltPixelFormat;
NN_GLL_API SwapBuffersType g_nngllapBuffers;
NN_GLL_API wglCopyContextType g_nnglllCopyContext;
NN_GLL_API wglCreateContextType g_nnglllCreateContext;
NN_GLL_API wglCreateLayerContextType g_nnglllCreateLayerContext;
NN_GLL_API wglDeleteContextType g_nnglllDeleteContext;
NN_GLL_API wglDescribeLayerPlaneType g_nnglllDescribeLayerPlane;
NN_GLL_API wglGetCurrentContextType g_nnglllGetCurrentContext;
NN_GLL_API wglGetCurrentDCType g_nnglllGetCurrentDC;
NN_GLL_API wglGetLayerPaletteEntriesType g_nnglllGetLayerPaletteEntries;
NN_GLL_API wglGetProcAddressType g_nnglllGetProcAddress;
NN_GLL_API wglMakeCurrentType g_nnglllMakeCurrent;
NN_GLL_API wglRealizeLayerPaletteType g_nnglllRealizeLayerPalette;
NN_GLL_API wglSetLayerPaletteEntriesType g_nnglllSetLayerPaletteEntries;
NN_GLL_API wglShareListsType g_nnglllShareLists;
NN_GLL_API wglSwapLayerBuffersType g_nnglllSwapLayerBuffers;
NN_GLL_API wglUseFontBitmapsType g_nnglllUseFontBitmaps;
NN_GLL_API wglUseFontBitmapsAType g_nnglllUseFontBitmapsA;
NN_GLL_API wglUseFontBitmapsWType g_nnglllUseFontBitmapsW;
NN_GLL_API wglUseFontOutlinesType g_nnglllUseFontOutlines;
NN_GLL_API wglUseFontOutlinesAType g_nnglllUseFontOutlinesA;
NN_GLL_API wglUseFontOutlinesWType g_nnglllUseFontOutlinesW;
#endif // defined( NN_BUILD_CONFIG_OS_WIN32 )
NN_GLL_API GLboolean nngllIsSupported_WGL_VERSION_1_0();
#endif // WGL_VERSION_1_0

#if !defined(WGL_3DFX_multisample)
#define WGL_3DFX_multisample

#define WGL_SAMPLE_BUFFERS_3DFX 0x2060 // NOLINT
#define WGL_SAMPLES_3DFX 0x2061 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_3DFX_multisample();
#endif // WGL_3DFX_multisample

#if !defined(WGL_3DL_stereo_control)
#define WGL_3DL_stereo_control

#define WGL_STEREO_EMITTER_ENABLE_3DL 0x2055 // NOLINT
#define WGL_STEREO_EMITTER_DISABLE_3DL 0x2056 // NOLINT
#define WGL_STEREO_POLARITY_NORMAL_3DL 0x2057 // NOLINT
#define WGL_STEREO_POLARITY_INVERT_3DL 0x2058 // NOLINT
typedef BOOL ( WINAPI *wglSetStereoEmitterState3DLType )( HDC hDC, UINT uState );
#define wglSetStereoEmitterState3DL g_nnglllSetStereoEmitterState3DL
NN_GLL_API wglSetStereoEmitterState3DLType g_nnglllSetStereoEmitterState3DL;
NN_GLL_API GLboolean nngllIsSupported_WGL_3DL_stereo_control();
#endif // WGL_3DL_stereo_control

#if !defined(WGL_AMD_gpu_association)
#define WGL_AMD_gpu_association

#define WGL_GPU_VENDOR_AMD 0x1F00 // NOLINT
#define WGL_GPU_RENDERER_STRING_AMD 0x1F01 // NOLINT
#define WGL_GPU_OPENGL_VERSION_STRING_AMD 0x1F02 // NOLINT
#define WGL_GPU_FASTEST_TARGET_GPUS_AMD 0x21A2 // NOLINT
#define WGL_GPU_RAM_AMD 0x21A3 // NOLINT
#define WGL_GPU_CLOCK_AMD 0x21A4 // NOLINT
#define WGL_GPU_NUM_PIPES_AMD 0x21A5 // NOLINT
#define WGL_GPU_NUM_SIMD_AMD 0x21A6 // NOLINT
#define WGL_GPU_NUM_RB_AMD 0x21A7 // NOLINT
#define WGL_GPU_NUM_SPI_AMD 0x21A8 // NOLINT
typedef UINT ( WINAPI *wglGetGPUIDsAMDType )( UINT maxCount, UINT *ids );
typedef INT ( WINAPI *wglGetGPUInfoAMDType )( UINT id, int property, GLenum dataType, UINT size, void *data );
typedef UINT ( WINAPI *wglGetContextGPUIDAMDType )( HGLRC hglrc );
typedef HGLRC ( WINAPI *wglCreateAssociatedContextAMDType )( UINT id );
typedef HGLRC ( WINAPI *wglCreateAssociatedContextAttribsAMDType )( UINT id, HGLRC hShareContext, const int *attribList );
typedef BOOL ( WINAPI *wglDeleteAssociatedContextAMDType )( HGLRC hglrc );
typedef BOOL ( WINAPI *wglMakeAssociatedContextCurrentAMDType )( HGLRC hglrc );
typedef HGLRC ( WINAPI *wglGetCurrentAssociatedContextAMDType )(  );
typedef VOID ( WINAPI *wglBlitContextFramebufferAMDType )( HGLRC dstCtx, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter );
#define wglGetGPUIDsAMD g_nnglllGetGPUIDsAMD
#define wglGetGPUInfoAMD g_nnglllGetGPUInfoAMD
#define wglGetContextGPUIDAMD g_nnglllGetContextGPUIDAMD
#define wglCreateAssociatedContextAMD g_nnglllCreateAssociatedContextAMD
#define wglCreateAssociatedContextAttribsAMD g_nnglllCreateAssociatedContextAttribsAMD
#define wglDeleteAssociatedContextAMD g_nnglllDeleteAssociatedContextAMD
#define wglMakeAssociatedContextCurrentAMD g_nnglllMakeAssociatedContextCurrentAMD
#define wglGetCurrentAssociatedContextAMD g_nnglllGetCurrentAssociatedContextAMD
#define wglBlitContextFramebufferAMD g_nnglllBlitContextFramebufferAMD
NN_GLL_API wglGetGPUIDsAMDType g_nnglllGetGPUIDsAMD;
NN_GLL_API wglGetGPUInfoAMDType g_nnglllGetGPUInfoAMD;
NN_GLL_API wglGetContextGPUIDAMDType g_nnglllGetContextGPUIDAMD;
NN_GLL_API wglCreateAssociatedContextAMDType g_nnglllCreateAssociatedContextAMD;
NN_GLL_API wglCreateAssociatedContextAttribsAMDType g_nnglllCreateAssociatedContextAttribsAMD;
NN_GLL_API wglDeleteAssociatedContextAMDType g_nnglllDeleteAssociatedContextAMD;
NN_GLL_API wglMakeAssociatedContextCurrentAMDType g_nnglllMakeAssociatedContextCurrentAMD;
NN_GLL_API wglGetCurrentAssociatedContextAMDType g_nnglllGetCurrentAssociatedContextAMD;
NN_GLL_API wglBlitContextFramebufferAMDType g_nnglllBlitContextFramebufferAMD;
NN_GLL_API GLboolean nngllIsSupported_WGL_AMD_gpu_association();
#endif // WGL_AMD_gpu_association

#if !defined(WGL_ARB_buffer_region)
#define WGL_ARB_buffer_region

#define WGL_FRONT_COLOR_BUFFER_BIT_ARB 0x00000001 // NOLINT
#define WGL_BACK_COLOR_BUFFER_BIT_ARB 0x00000002 // NOLINT
#define WGL_DEPTH_BUFFER_BIT_ARB 0x00000004 // NOLINT
#define WGL_STENCIL_BUFFER_BIT_ARB 0x00000008 // NOLINT
typedef HANDLE ( WINAPI *wglCreateBufferRegionARBType )( HDC hDC, int iLayerPlane, UINT uType );
typedef VOID ( WINAPI *wglDeleteBufferRegionARBType )( HANDLE hRegion );
typedef BOOL ( WINAPI *wglSaveBufferRegionARBType )( HANDLE hRegion, int x, int y, int width, int height );
typedef BOOL ( WINAPI *wglRestoreBufferRegionARBType )( HANDLE hRegion, int x, int y, int width, int height, int xSrc, int ySrc );
#define wglCreateBufferRegionARB g_nnglllCreateBufferRegionARB
#define wglDeleteBufferRegionARB g_nnglllDeleteBufferRegionARB
#define wglSaveBufferRegionARB g_nnglllSaveBufferRegionARB
#define wglRestoreBufferRegionARB g_nnglllRestoreBufferRegionARB
NN_GLL_API wglCreateBufferRegionARBType g_nnglllCreateBufferRegionARB;
NN_GLL_API wglDeleteBufferRegionARBType g_nnglllDeleteBufferRegionARB;
NN_GLL_API wglSaveBufferRegionARBType g_nnglllSaveBufferRegionARB;
NN_GLL_API wglRestoreBufferRegionARBType g_nnglllRestoreBufferRegionARB;
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_buffer_region();
#endif // WGL_ARB_buffer_region

#if !defined(WGL_ARB_context_flush_control)
#define WGL_ARB_context_flush_control

#define WGL_CONTEXT_RELEASE_BEHAVIOR_ARB 0x2097 // NOLINT
#define WGL_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB 0 // NOLINT
#define WGL_CONTEXT_RELEASE_BEHAVIOR_FLUSH_ARB 0x2098 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_context_flush_control();
#endif // WGL_ARB_context_flush_control

#if !defined(WGL_ARB_create_context)
#define WGL_ARB_create_context

#define WGL_CONTEXT_DEBUG_BIT_ARB 0x00000001 // NOLINT
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002 // NOLINT
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091 // NOLINT
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092 // NOLINT
#define WGL_CONTEXT_LAYER_PLANE_ARB 0x2093 // NOLINT
#define WGL_CONTEXT_FLAGS_ARB 0x2094 // NOLINT
#define ERROR_INVALID_VERSION_ARB 0x2095 // NOLINT
typedef HGLRC ( WINAPI *wglCreateContextAttribsARBType )( HDC hDC, HGLRC hShareContext, const int *attribList );
#define wglCreateContextAttribsARB g_nnglllCreateContextAttribsARB
NN_GLL_API wglCreateContextAttribsARBType g_nnglllCreateContextAttribsARB;
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_create_context();
#endif // WGL_ARB_create_context

#if !defined(WGL_ARB_create_context_no_error)
#define WGL_ARB_create_context_no_error

#define WGL_CONTEXT_OPENGL_NO_ERROR_ARB 0x31B3 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_create_context_no_error();
#endif // WGL_ARB_create_context_no_error

#if !defined(WGL_ARB_create_context_profile)
#define WGL_ARB_create_context_profile

#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126 // NOLINT
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001 // NOLINT
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002 // NOLINT
#define ERROR_INVALID_PROFILE_ARB 0x2096 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_create_context_profile();
#endif // WGL_ARB_create_context_profile

#if !defined(WGL_ARB_create_context_robustness)
#define WGL_ARB_create_context_robustness

#define WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB 0x00000004 // NOLINT
#define WGL_LOSE_CONTEXT_ON_RESET_ARB 0x8252 // NOLINT
#define WGL_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB 0x8256 // NOLINT
#define WGL_NO_RESET_NOTIFICATION_ARB 0x8261 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_create_context_robustness();
#endif // WGL_ARB_create_context_robustness

#if !defined(WGL_ARB_extensions_string)
#define WGL_ARB_extensions_string

typedef const char *( WINAPI *wglGetExtensionsStringARBType )( HDC hdc );
#define wglGetExtensionsStringARB g_nnglllGetExtensionsStringARB
NN_GLL_API wglGetExtensionsStringARBType g_nnglllGetExtensionsStringARB;
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_extensions_string();
#endif // WGL_ARB_extensions_string

#if !defined(WGL_ARB_framebuffer_sRGB)
#define WGL_ARB_framebuffer_sRGB

#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB 0x20A9 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_framebuffer_sRGB();
#endif // WGL_ARB_framebuffer_sRGB

#if !defined(WGL_ARB_make_current_read)
#define WGL_ARB_make_current_read

#define ERROR_INVALID_PIXEL_TYPE_ARB 0x2043 // NOLINT
#define ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB 0x2054 // NOLINT
typedef BOOL ( WINAPI *wglMakeContextCurrentARBType )( HDC hDrawDC, HDC hReadDC, HGLRC hglrc );
typedef HDC ( WINAPI *wglGetCurrentReadDCARBType )(  );
#define wglMakeContextCurrentARB g_nnglllMakeContextCurrentARB
#define wglGetCurrentReadDCARB g_nnglllGetCurrentReadDCARB
NN_GLL_API wglMakeContextCurrentARBType g_nnglllMakeContextCurrentARB;
NN_GLL_API wglGetCurrentReadDCARBType g_nnglllGetCurrentReadDCARB;
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_make_current_read();
#endif // WGL_ARB_make_current_read

#if !defined(WGL_ARB_multisample)
#define WGL_ARB_multisample

#define WGL_SAMPLE_BUFFERS_ARB 0x2041 // NOLINT
#define WGL_SAMPLES_ARB 0x2042 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_multisample();
#endif // WGL_ARB_multisample

#if !defined(WGL_ARB_pbuffer)
#define WGL_ARB_pbuffer

DECLARE_HANDLE(HPBUFFERARB);
#define WGL_DRAW_TO_PBUFFER_ARB 0x202D // NOLINT
#define WGL_MAX_PBUFFER_PIXELS_ARB 0x202E // NOLINT
#define WGL_MAX_PBUFFER_WIDTH_ARB 0x202F // NOLINT
#define WGL_MAX_PBUFFER_HEIGHT_ARB 0x2030 // NOLINT
#define WGL_PBUFFER_LARGEST_ARB 0x2033 // NOLINT
#define WGL_PBUFFER_WIDTH_ARB 0x2034 // NOLINT
#define WGL_PBUFFER_HEIGHT_ARB 0x2035 // NOLINT
#define WGL_PBUFFER_LOST_ARB 0x2036 // NOLINT
typedef HPBUFFERARB ( WINAPI *wglCreatePbufferARBType )( HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList );
typedef HDC ( WINAPI *wglGetPbufferDCARBType )( HPBUFFERARB hPbuffer );
typedef int ( WINAPI *wglReleasePbufferDCARBType )( HPBUFFERARB hPbuffer, HDC hDC );
typedef BOOL ( WINAPI *wglDestroyPbufferARBType )( HPBUFFERARB hPbuffer );
typedef BOOL ( WINAPI *wglQueryPbufferARBType )( HPBUFFERARB hPbuffer, int iAttribute, int *piValue );
#define wglCreatePbufferARB g_nnglllCreatePbufferARB
#define wglGetPbufferDCARB g_nnglllGetPbufferDCARB
#define wglReleasePbufferDCARB g_nnglllReleasePbufferDCARB
#define wglDestroyPbufferARB g_nnglllDestroyPbufferARB
#define wglQueryPbufferARB g_nnglllQueryPbufferARB
NN_GLL_API wglCreatePbufferARBType g_nnglllCreatePbufferARB;
NN_GLL_API wglGetPbufferDCARBType g_nnglllGetPbufferDCARB;
NN_GLL_API wglReleasePbufferDCARBType g_nnglllReleasePbufferDCARB;
NN_GLL_API wglDestroyPbufferARBType g_nnglllDestroyPbufferARB;
NN_GLL_API wglQueryPbufferARBType g_nnglllQueryPbufferARB;
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_pbuffer();
#endif // WGL_ARB_pbuffer

#if !defined(WGL_ARB_pixel_format)
#define WGL_ARB_pixel_format

#define WGL_NUMBER_PIXEL_FORMATS_ARB 0x2000 // NOLINT
#define WGL_DRAW_TO_WINDOW_ARB 0x2001 // NOLINT
#define WGL_DRAW_TO_BITMAP_ARB 0x2002 // NOLINT
#define WGL_ACCELERATION_ARB 0x2003 // NOLINT
#define WGL_NEED_PALETTE_ARB 0x2004 // NOLINT
#define WGL_NEED_SYSTEM_PALETTE_ARB 0x2005 // NOLINT
#define WGL_SWAP_LAYER_BUFFERS_ARB 0x2006 // NOLINT
#define WGL_SWAP_METHOD_ARB 0x2007 // NOLINT
#define WGL_NUMBER_OVERLAYS_ARB 0x2008 // NOLINT
#define WGL_NUMBER_UNDERLAYS_ARB 0x2009 // NOLINT
#define WGL_TRANSPARENT_ARB 0x200A // NOLINT
#define WGL_TRANSPARENT_RED_VALUE_ARB 0x2037 // NOLINT
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038 // NOLINT
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039 // NOLINT
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A // NOLINT
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B // NOLINT
#define WGL_SHARE_DEPTH_ARB 0x200C // NOLINT
#define WGL_SHARE_STENCIL_ARB 0x200D // NOLINT
#define WGL_SHARE_ACCUM_ARB 0x200E // NOLINT
#define WGL_SUPPORT_GDI_ARB 0x200F // NOLINT
#define WGL_SUPPORT_OPENGL_ARB 0x2010 // NOLINT
#define WGL_DOUBLE_BUFFER_ARB 0x2011 // NOLINT
#define WGL_STEREO_ARB 0x2012 // NOLINT
#define WGL_PIXEL_TYPE_ARB 0x2013 // NOLINT
#define WGL_COLOR_BITS_ARB 0x2014 // NOLINT
#define WGL_RED_BITS_ARB 0x2015 // NOLINT
#define WGL_RED_SHIFT_ARB 0x2016 // NOLINT
#define WGL_GREEN_BITS_ARB 0x2017 // NOLINT
#define WGL_GREEN_SHIFT_ARB 0x2018 // NOLINT
#define WGL_BLUE_BITS_ARB 0x2019 // NOLINT
#define WGL_BLUE_SHIFT_ARB 0x201A // NOLINT
#define WGL_ALPHA_BITS_ARB 0x201B // NOLINT
#define WGL_ALPHA_SHIFT_ARB 0x201C // NOLINT
#define WGL_ACCUM_BITS_ARB 0x201D // NOLINT
#define WGL_ACCUM_RED_BITS_ARB 0x201E // NOLINT
#define WGL_ACCUM_GREEN_BITS_ARB 0x201F // NOLINT
#define WGL_ACCUM_BLUE_BITS_ARB 0x2020 // NOLINT
#define WGL_ACCUM_ALPHA_BITS_ARB 0x2021 // NOLINT
#define WGL_DEPTH_BITS_ARB 0x2022 // NOLINT
#define WGL_STENCIL_BITS_ARB 0x2023 // NOLINT
#define WGL_AUX_BUFFERS_ARB 0x2024 // NOLINT
#define WGL_NO_ACCELERATION_ARB 0x2025 // NOLINT
#define WGL_GENERIC_ACCELERATION_ARB 0x2026 // NOLINT
#define WGL_FULL_ACCELERATION_ARB 0x2027 // NOLINT
#define WGL_SWAP_EXCHANGE_ARB 0x2028 // NOLINT
#define WGL_SWAP_COPY_ARB 0x2029 // NOLINT
#define WGL_SWAP_UNDEFINED_ARB 0x202A // NOLINT
#define WGL_TYPE_RGBA_ARB 0x202B // NOLINT
#define WGL_TYPE_COLORINDEX_ARB 0x202C // NOLINT
typedef BOOL ( WINAPI *wglGetPixelFormatAttribivARBType )( HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues );
typedef BOOL ( WINAPI *wglGetPixelFormatAttribfvARBType )( HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues );
typedef BOOL ( WINAPI *wglChoosePixelFormatARBType )( HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats );
#define wglGetPixelFormatAttribivARB g_nnglllGetPixelFormatAttribivARB
#define wglGetPixelFormatAttribfvARB g_nnglllGetPixelFormatAttribfvARB
#define wglChoosePixelFormatARB g_nnglllChoosePixelFormatARB
NN_GLL_API wglGetPixelFormatAttribivARBType g_nnglllGetPixelFormatAttribivARB;
NN_GLL_API wglGetPixelFormatAttribfvARBType g_nnglllGetPixelFormatAttribfvARB;
NN_GLL_API wglChoosePixelFormatARBType g_nnglllChoosePixelFormatARB;
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_pixel_format();
#endif // WGL_ARB_pixel_format

#if !defined(WGL_ARB_pixel_format_float)
#define WGL_ARB_pixel_format_float

#define WGL_TYPE_RGBA_FLOAT_ARB 0x21A0 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_pixel_format_float();
#endif // WGL_ARB_pixel_format_float

#if !defined(WGL_ARB_render_texture)
#define WGL_ARB_render_texture

#define WGL_BIND_TO_TEXTURE_RGB_ARB 0x2070 // NOLINT
#define WGL_BIND_TO_TEXTURE_RGBA_ARB 0x2071 // NOLINT
#define WGL_TEXTURE_FORMAT_ARB 0x2072 // NOLINT
#define WGL_TEXTURE_TARGET_ARB 0x2073 // NOLINT
#define WGL_MIPMAP_TEXTURE_ARB 0x2074 // NOLINT
#define WGL_TEXTURE_RGB_ARB 0x2075 // NOLINT
#define WGL_TEXTURE_RGBA_ARB 0x2076 // NOLINT
#define WGL_NO_TEXTURE_ARB 0x2077 // NOLINT
#define WGL_TEXTURE_CUBE_MAP_ARB 0x2078 // NOLINT
#define WGL_TEXTURE_1D_ARB 0x2079 // NOLINT
#define WGL_TEXTURE_2D_ARB 0x207A // NOLINT
#define WGL_MIPMAP_LEVEL_ARB 0x207B // NOLINT
#define WGL_CUBE_MAP_FACE_ARB 0x207C // NOLINT
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x207D // NOLINT
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x207E // NOLINT
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x207F // NOLINT
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x2080 // NOLINT
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x2081 // NOLINT
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x2082 // NOLINT
#define WGL_FRONT_LEFT_ARB 0x2083 // NOLINT
#define WGL_FRONT_RIGHT_ARB 0x2084 // NOLINT
#define WGL_BACK_LEFT_ARB 0x2085 // NOLINT
#define WGL_BACK_RIGHT_ARB 0x2086 // NOLINT
#define WGL_AUX0_ARB 0x2087 // NOLINT
#define WGL_AUX1_ARB 0x2088 // NOLINT
#define WGL_AUX2_ARB 0x2089 // NOLINT
#define WGL_AUX3_ARB 0x208A // NOLINT
#define WGL_AUX4_ARB 0x208B // NOLINT
#define WGL_AUX5_ARB 0x208C // NOLINT
#define WGL_AUX6_ARB 0x208D // NOLINT
#define WGL_AUX7_ARB 0x208E // NOLINT
#define WGL_AUX8_ARB 0x208F // NOLINT
#define WGL_AUX9_ARB 0x2090 // NOLINT
typedef BOOL ( WINAPI *wglBindTexImageARBType )( HPBUFFERARB hPbuffer, int iBuffer );
typedef BOOL ( WINAPI *wglReleaseTexImageARBType )( HPBUFFERARB hPbuffer, int iBuffer );
typedef BOOL ( WINAPI *wglSetPbufferAttribARBType )( HPBUFFERARB hPbuffer, const int *piAttribList );
#define wglBindTexImageARB g_nnglllBindTexImageARB
#define wglReleaseTexImageARB g_nnglllReleaseTexImageARB
#define wglSetPbufferAttribARB g_nnglllSetPbufferAttribARB
NN_GLL_API wglBindTexImageARBType g_nnglllBindTexImageARB;
NN_GLL_API wglReleaseTexImageARBType g_nnglllReleaseTexImageARB;
NN_GLL_API wglSetPbufferAttribARBType g_nnglllSetPbufferAttribARB;
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_render_texture();
#endif // WGL_ARB_render_texture

#if !defined(WGL_ARB_robustness_application_isolation)
#define WGL_ARB_robustness_application_isolation

#define WGL_CONTEXT_RESET_ISOLATION_BIT_ARB 0x00000008 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_robustness_application_isolation();
#endif // WGL_ARB_robustness_application_isolation

#if !defined(WGL_ARB_robustness_share_group_isolation)
#define WGL_ARB_robustness_share_group_isolation

NN_GLL_API GLboolean nngllIsSupported_WGL_ARB_robustness_share_group_isolation();
#endif // WGL_ARB_robustness_share_group_isolation

#if !defined(WGL_ATI_pixel_format_float)
#define WGL_ATI_pixel_format_float

#define WGL_TYPE_RGBA_FLOAT_ATI 0x21A0 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_ATI_pixel_format_float();
#endif // WGL_ATI_pixel_format_float

#if !defined(WGL_EXT_colorspace)
#define WGL_EXT_colorspace

#define WGL_COLORSPACE_EXT 0x3087 // NOLINT
#define WGL_COLORSPACE_SRGB_EXT 0x3089 // NOLINT
#define WGL_COLORSPACE_LINEAR_EXT 0x308A // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_EXT_colorspace();
#endif // WGL_EXT_colorspace

#if !defined(WGL_EXT_create_context_es_profile)
#define WGL_EXT_create_context_es_profile

#define WGL_CONTEXT_ES_PROFILE_BIT_EXT 0x00000004 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_EXT_create_context_es_profile();
#endif // WGL_EXT_create_context_es_profile

#if !defined(WGL_EXT_create_context_es2_profile)
#define WGL_EXT_create_context_es2_profile

#define WGL_CONTEXT_ES2_PROFILE_BIT_EXT 0x00000004 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_EXT_create_context_es2_profile();
#endif // WGL_EXT_create_context_es2_profile

#if !defined(WGL_EXT_depth_float)
#define WGL_EXT_depth_float

#define WGL_DEPTH_FLOAT_EXT 0x2040 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_EXT_depth_float();
#endif // WGL_EXT_depth_float

#if !defined(WGL_EXT_display_color_table)
#define WGL_EXT_display_color_table

typedef GLboolean ( WINAPI *wglCreateDisplayColorTableEXTType )( GLushort id );
typedef GLboolean ( WINAPI *wglLoadDisplayColorTableEXTType )( const GLushort *table, GLuint length );
typedef GLboolean ( WINAPI *wglBindDisplayColorTableEXTType )( GLushort id );
typedef VOID ( WINAPI *wglDestroyDisplayColorTableEXTType )( GLushort id );
#define wglCreateDisplayColorTableEXT g_nnglllCreateDisplayColorTableEXT
#define wglLoadDisplayColorTableEXT g_nnglllLoadDisplayColorTableEXT
#define wglBindDisplayColorTableEXT g_nnglllBindDisplayColorTableEXT
#define wglDestroyDisplayColorTableEXT g_nnglllDestroyDisplayColorTableEXT
NN_GLL_API wglCreateDisplayColorTableEXTType g_nnglllCreateDisplayColorTableEXT;
NN_GLL_API wglLoadDisplayColorTableEXTType g_nnglllLoadDisplayColorTableEXT;
NN_GLL_API wglBindDisplayColorTableEXTType g_nnglllBindDisplayColorTableEXT;
NN_GLL_API wglDestroyDisplayColorTableEXTType g_nnglllDestroyDisplayColorTableEXT;
NN_GLL_API GLboolean nngllIsSupported_WGL_EXT_display_color_table();
#endif // WGL_EXT_display_color_table

#if !defined(WGL_EXT_extensions_string)
#define WGL_EXT_extensions_string

typedef const char *( WINAPI *wglGetExtensionsStringEXTType )(  );
#define wglGetExtensionsStringEXT g_nnglllGetExtensionsStringEXT
NN_GLL_API wglGetExtensionsStringEXTType g_nnglllGetExtensionsStringEXT;
NN_GLL_API GLboolean nngllIsSupported_WGL_EXT_extensions_string();
#endif // WGL_EXT_extensions_string

#if !defined(WGL_EXT_framebuffer_sRGB)
#define WGL_EXT_framebuffer_sRGB

#define WGL_FRAMEBUFFER_SRGB_CAPABLE_EXT 0x20A9 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_EXT_framebuffer_sRGB();
#endif // WGL_EXT_framebuffer_sRGB

#if !defined(WGL_EXT_make_current_read)
#define WGL_EXT_make_current_read

#define ERROR_INVALID_PIXEL_TYPE_EXT 0x2043 // NOLINT
typedef BOOL ( WINAPI *wglMakeContextCurrentEXTType )( HDC hDrawDC, HDC hReadDC, HGLRC hglrc );
typedef HDC ( WINAPI *wglGetCurrentReadDCEXTType )(  );
#define wglMakeContextCurrentEXT g_nnglllMakeContextCurrentEXT
#define wglGetCurrentReadDCEXT g_nnglllGetCurrentReadDCEXT
NN_GLL_API wglMakeContextCurrentEXTType g_nnglllMakeContextCurrentEXT;
NN_GLL_API wglGetCurrentReadDCEXTType g_nnglllGetCurrentReadDCEXT;
NN_GLL_API GLboolean nngllIsSupported_WGL_EXT_make_current_read();
#endif // WGL_EXT_make_current_read

#if !defined(WGL_EXT_multisample)
#define WGL_EXT_multisample

#define WGL_SAMPLE_BUFFERS_EXT 0x2041 // NOLINT
#define WGL_SAMPLES_EXT 0x2042 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_EXT_multisample();
#endif // WGL_EXT_multisample

#if !defined(WGL_EXT_pbuffer)
#define WGL_EXT_pbuffer

DECLARE_HANDLE(HPBUFFEREXT);
#define WGL_DRAW_TO_PBUFFER_EXT 0x202D // NOLINT
#define WGL_MAX_PBUFFER_PIXELS_EXT 0x202E // NOLINT
#define WGL_MAX_PBUFFER_WIDTH_EXT 0x202F // NOLINT
#define WGL_MAX_PBUFFER_HEIGHT_EXT 0x2030 // NOLINT
#define WGL_OPTIMAL_PBUFFER_WIDTH_EXT 0x2031 // NOLINT
#define WGL_OPTIMAL_PBUFFER_HEIGHT_EXT 0x2032 // NOLINT
#define WGL_PBUFFER_LARGEST_EXT 0x2033 // NOLINT
#define WGL_PBUFFER_WIDTH_EXT 0x2034 // NOLINT
#define WGL_PBUFFER_HEIGHT_EXT 0x2035 // NOLINT
typedef HPBUFFEREXT ( WINAPI *wglCreatePbufferEXTType )( HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList );
typedef HDC ( WINAPI *wglGetPbufferDCEXTType )( HPBUFFEREXT hPbuffer );
typedef int ( WINAPI *wglReleasePbufferDCEXTType )( HPBUFFEREXT hPbuffer, HDC hDC );
typedef BOOL ( WINAPI *wglDestroyPbufferEXTType )( HPBUFFEREXT hPbuffer );
typedef BOOL ( WINAPI *wglQueryPbufferEXTType )( HPBUFFEREXT hPbuffer, int iAttribute, int *piValue );
#define wglCreatePbufferEXT g_nnglllCreatePbufferEXT
#define wglGetPbufferDCEXT g_nnglllGetPbufferDCEXT
#define wglReleasePbufferDCEXT g_nnglllReleasePbufferDCEXT
#define wglDestroyPbufferEXT g_nnglllDestroyPbufferEXT
#define wglQueryPbufferEXT g_nnglllQueryPbufferEXT
NN_GLL_API wglCreatePbufferEXTType g_nnglllCreatePbufferEXT;
NN_GLL_API wglGetPbufferDCEXTType g_nnglllGetPbufferDCEXT;
NN_GLL_API wglReleasePbufferDCEXTType g_nnglllReleasePbufferDCEXT;
NN_GLL_API wglDestroyPbufferEXTType g_nnglllDestroyPbufferEXT;
NN_GLL_API wglQueryPbufferEXTType g_nnglllQueryPbufferEXT;
NN_GLL_API GLboolean nngllIsSupported_WGL_EXT_pbuffer();
#endif // WGL_EXT_pbuffer

#if !defined(WGL_EXT_pixel_format)
#define WGL_EXT_pixel_format

#define WGL_NUMBER_PIXEL_FORMATS_EXT 0x2000 // NOLINT
#define WGL_DRAW_TO_WINDOW_EXT 0x2001 // NOLINT
#define WGL_DRAW_TO_BITMAP_EXT 0x2002 // NOLINT
#define WGL_ACCELERATION_EXT 0x2003 // NOLINT
#define WGL_NEED_PALETTE_EXT 0x2004 // NOLINT
#define WGL_NEED_SYSTEM_PALETTE_EXT 0x2005 // NOLINT
#define WGL_SWAP_LAYER_BUFFERS_EXT 0x2006 // NOLINT
#define WGL_SWAP_METHOD_EXT 0x2007 // NOLINT
#define WGL_NUMBER_OVERLAYS_EXT 0x2008 // NOLINT
#define WGL_NUMBER_UNDERLAYS_EXT 0x2009 // NOLINT
#define WGL_TRANSPARENT_EXT 0x200A // NOLINT
#define WGL_TRANSPARENT_VALUE_EXT 0x200B // NOLINT
#define WGL_SHARE_DEPTH_EXT 0x200C // NOLINT
#define WGL_SHARE_STENCIL_EXT 0x200D // NOLINT
#define WGL_SHARE_ACCUM_EXT 0x200E // NOLINT
#define WGL_SUPPORT_GDI_EXT 0x200F // NOLINT
#define WGL_SUPPORT_OPENGL_EXT 0x2010 // NOLINT
#define WGL_DOUBLE_BUFFER_EXT 0x2011 // NOLINT
#define WGL_STEREO_EXT 0x2012 // NOLINT
#define WGL_PIXEL_TYPE_EXT 0x2013 // NOLINT
#define WGL_COLOR_BITS_EXT 0x2014 // NOLINT
#define WGL_RED_BITS_EXT 0x2015 // NOLINT
#define WGL_RED_SHIFT_EXT 0x2016 // NOLINT
#define WGL_GREEN_BITS_EXT 0x2017 // NOLINT
#define WGL_GREEN_SHIFT_EXT 0x2018 // NOLINT
#define WGL_BLUE_BITS_EXT 0x2019 // NOLINT
#define WGL_BLUE_SHIFT_EXT 0x201A // NOLINT
#define WGL_ALPHA_BITS_EXT 0x201B // NOLINT
#define WGL_ALPHA_SHIFT_EXT 0x201C // NOLINT
#define WGL_ACCUM_BITS_EXT 0x201D // NOLINT
#define WGL_ACCUM_RED_BITS_EXT 0x201E // NOLINT
#define WGL_ACCUM_GREEN_BITS_EXT 0x201F // NOLINT
#define WGL_ACCUM_BLUE_BITS_EXT 0x2020 // NOLINT
#define WGL_ACCUM_ALPHA_BITS_EXT 0x2021 // NOLINT
#define WGL_DEPTH_BITS_EXT 0x2022 // NOLINT
#define WGL_STENCIL_BITS_EXT 0x2023 // NOLINT
#define WGL_AUX_BUFFERS_EXT 0x2024 // NOLINT
#define WGL_NO_ACCELERATION_EXT 0x2025 // NOLINT
#define WGL_GENERIC_ACCELERATION_EXT 0x2026 // NOLINT
#define WGL_FULL_ACCELERATION_EXT 0x2027 // NOLINT
#define WGL_SWAP_EXCHANGE_EXT 0x2028 // NOLINT
#define WGL_SWAP_COPY_EXT 0x2029 // NOLINT
#define WGL_SWAP_UNDEFINED_EXT 0x202A // NOLINT
#define WGL_TYPE_RGBA_EXT 0x202B // NOLINT
#define WGL_TYPE_COLORINDEX_EXT 0x202C // NOLINT
typedef BOOL ( WINAPI *wglGetPixelFormatAttribivEXTType )( HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, int *piValues );
typedef BOOL ( WINAPI *wglGetPixelFormatAttribfvEXTType )( HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, FLOAT *pfValues );
typedef BOOL ( WINAPI *wglChoosePixelFormatEXTType )( HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats );
#define wglGetPixelFormatAttribivEXT g_nnglllGetPixelFormatAttribivEXT
#define wglGetPixelFormatAttribfvEXT g_nnglllGetPixelFormatAttribfvEXT
#define wglChoosePixelFormatEXT g_nnglllChoosePixelFormatEXT
NN_GLL_API wglGetPixelFormatAttribivEXTType g_nnglllGetPixelFormatAttribivEXT;
NN_GLL_API wglGetPixelFormatAttribfvEXTType g_nnglllGetPixelFormatAttribfvEXT;
NN_GLL_API wglChoosePixelFormatEXTType g_nnglllChoosePixelFormatEXT;
NN_GLL_API GLboolean nngllIsSupported_WGL_EXT_pixel_format();
#endif // WGL_EXT_pixel_format

#if !defined(WGL_EXT_pixel_format_packed_float)
#define WGL_EXT_pixel_format_packed_float

#define WGL_TYPE_RGBA_UNSIGNED_FLOAT_EXT 0x20A8 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_EXT_pixel_format_packed_float();
#endif // WGL_EXT_pixel_format_packed_float

#if !defined(WGL_EXT_swap_control)
#define WGL_EXT_swap_control

typedef BOOL ( WINAPI *wglSwapIntervalEXTType )( int interval );
typedef int ( WINAPI *wglGetSwapIntervalEXTType )(  );
#define wglSwapIntervalEXT g_nnglllSwapIntervalEXT
#define wglGetSwapIntervalEXT g_nnglllGetSwapIntervalEXT
NN_GLL_API wglSwapIntervalEXTType g_nnglllSwapIntervalEXT;
NN_GLL_API wglGetSwapIntervalEXTType g_nnglllGetSwapIntervalEXT;
NN_GLL_API GLboolean nngllIsSupported_WGL_EXT_swap_control();
#endif // WGL_EXT_swap_control

#if !defined(WGL_EXT_swap_control_tear)
#define WGL_EXT_swap_control_tear

NN_GLL_API GLboolean nngllIsSupported_WGL_EXT_swap_control_tear();
#endif // WGL_EXT_swap_control_tear

#if !defined(WGL_I3D_digital_video_control)
#define WGL_I3D_digital_video_control

#define WGL_DIGITAL_VIDEO_CURSOR_ALPHA_FRAMEBUFFER_I3D 0x2050 // NOLINT
#define WGL_DIGITAL_VIDEO_CURSOR_ALPHA_VALUE_I3D 0x2051 // NOLINT
#define WGL_DIGITAL_VIDEO_CURSOR_INCLUDED_I3D 0x2052 // NOLINT
#define WGL_DIGITAL_VIDEO_GAMMA_CORRECTED_I3D 0x2053 // NOLINT
typedef BOOL ( WINAPI *wglGetDigitalVideoParametersI3DType )( HDC hDC, int iAttribute, int *piValue );
typedef BOOL ( WINAPI *wglSetDigitalVideoParametersI3DType )( HDC hDC, int iAttribute, const int *piValue );
#define wglGetDigitalVideoParametersI3D g_nnglllGetDigitalVideoParametersI3D
#define wglSetDigitalVideoParametersI3D g_nnglllSetDigitalVideoParametersI3D
NN_GLL_API wglGetDigitalVideoParametersI3DType g_nnglllGetDigitalVideoParametersI3D;
NN_GLL_API wglSetDigitalVideoParametersI3DType g_nnglllSetDigitalVideoParametersI3D;
NN_GLL_API GLboolean nngllIsSupported_WGL_I3D_digital_video_control();
#endif // WGL_I3D_digital_video_control

#if !defined(WGL_I3D_gamma)
#define WGL_I3D_gamma

#define WGL_GAMMA_TABLE_SIZE_I3D 0x204E // NOLINT
#define WGL_GAMMA_EXCLUDE_DESKTOP_I3D 0x204F // NOLINT
typedef BOOL ( WINAPI *wglGetGammaTableParametersI3DType )( HDC hDC, int iAttribute, int *piValue );
typedef BOOL ( WINAPI *wglSetGammaTableParametersI3DType )( HDC hDC, int iAttribute, const int *piValue );
typedef BOOL ( WINAPI *wglGetGammaTableI3DType )( HDC hDC, int iEntries, USHORT *puRed, USHORT *puGreen, USHORT *puBlue );
typedef BOOL ( WINAPI *wglSetGammaTableI3DType )( HDC hDC, int iEntries, const USHORT *puRed, const USHORT *puGreen, const USHORT *puBlue );
#define wglGetGammaTableParametersI3D g_nnglllGetGammaTableParametersI3D
#define wglSetGammaTableParametersI3D g_nnglllSetGammaTableParametersI3D
#define wglGetGammaTableI3D g_nnglllGetGammaTableI3D
#define wglSetGammaTableI3D g_nnglllSetGammaTableI3D
NN_GLL_API wglGetGammaTableParametersI3DType g_nnglllGetGammaTableParametersI3D;
NN_GLL_API wglSetGammaTableParametersI3DType g_nnglllSetGammaTableParametersI3D;
NN_GLL_API wglGetGammaTableI3DType g_nnglllGetGammaTableI3D;
NN_GLL_API wglSetGammaTableI3DType g_nnglllSetGammaTableI3D;
NN_GLL_API GLboolean nngllIsSupported_WGL_I3D_gamma();
#endif // WGL_I3D_gamma

#if !defined(WGL_I3D_genlock)
#define WGL_I3D_genlock

#define WGL_GENLOCK_SOURCE_MULTIVIEW_I3D 0x2044 // NOLINT
#define WGL_GENLOCK_SOURCE_EXTERNAL_SYNC_I3D 0x2045 // NOLINT
#define WGL_GENLOCK_SOURCE_EXTERNAL_FIELD_I3D 0x2046 // NOLINT
#define WGL_GENLOCK_SOURCE_EXTERNAL_TTL_I3D 0x2047 // NOLINT
#define WGL_GENLOCK_SOURCE_DIGITAL_SYNC_I3D 0x2048 // NOLINT
#define WGL_GENLOCK_SOURCE_DIGITAL_FIELD_I3D 0x2049 // NOLINT
#define WGL_GENLOCK_SOURCE_EDGE_FALLING_I3D 0x204A // NOLINT
#define WGL_GENLOCK_SOURCE_EDGE_RISING_I3D 0x204B // NOLINT
#define WGL_GENLOCK_SOURCE_EDGE_BOTH_I3D 0x204C // NOLINT
typedef BOOL ( WINAPI *wglEnableGenlockI3DType )( HDC hDC );
typedef BOOL ( WINAPI *wglDisableGenlockI3DType )( HDC hDC );
typedef BOOL ( WINAPI *wglIsEnabledGenlockI3DType )( HDC hDC, BOOL *pFlag );
typedef BOOL ( WINAPI *wglGenlockSourceI3DType )( HDC hDC, UINT uSource );
typedef BOOL ( WINAPI *wglGetGenlockSourceI3DType )( HDC hDC, UINT *uSource );
typedef BOOL ( WINAPI *wglGenlockSourceEdgeI3DType )( HDC hDC, UINT uEdge );
typedef BOOL ( WINAPI *wglGetGenlockSourceEdgeI3DType )( HDC hDC, UINT *uEdge );
typedef BOOL ( WINAPI *wglGenlockSampleRateI3DType )( HDC hDC, UINT uRate );
typedef BOOL ( WINAPI *wglGetGenlockSampleRateI3DType )( HDC hDC, UINT *uRate );
typedef BOOL ( WINAPI *wglGenlockSourceDelayI3DType )( HDC hDC, UINT uDelay );
typedef BOOL ( WINAPI *wglGetGenlockSourceDelayI3DType )( HDC hDC, UINT *uDelay );
typedef BOOL ( WINAPI *wglQueryGenlockMaxSourceDelayI3DType )( HDC hDC, UINT *uMaxLineDelay, UINT *uMaxPixelDelay );
#define wglEnableGenlockI3D g_nnglllEnableGenlockI3D
#define wglDisableGenlockI3D g_nnglllDisableGenlockI3D
#define wglIsEnabledGenlockI3D g_nnglllIsEnabledGenlockI3D
#define wglGenlockSourceI3D g_nnglllGenlockSourceI3D
#define wglGetGenlockSourceI3D g_nnglllGetGenlockSourceI3D
#define wglGenlockSourceEdgeI3D g_nnglllGenlockSourceEdgeI3D
#define wglGetGenlockSourceEdgeI3D g_nnglllGetGenlockSourceEdgeI3D
#define wglGenlockSampleRateI3D g_nnglllGenlockSampleRateI3D
#define wglGetGenlockSampleRateI3D g_nnglllGetGenlockSampleRateI3D
#define wglGenlockSourceDelayI3D g_nnglllGenlockSourceDelayI3D
#define wglGetGenlockSourceDelayI3D g_nnglllGetGenlockSourceDelayI3D
#define wglQueryGenlockMaxSourceDelayI3D g_nnglllQueryGenlockMaxSourceDelayI3D
NN_GLL_API wglEnableGenlockI3DType g_nnglllEnableGenlockI3D;
NN_GLL_API wglDisableGenlockI3DType g_nnglllDisableGenlockI3D;
NN_GLL_API wglIsEnabledGenlockI3DType g_nnglllIsEnabledGenlockI3D;
NN_GLL_API wglGenlockSourceI3DType g_nnglllGenlockSourceI3D;
NN_GLL_API wglGetGenlockSourceI3DType g_nnglllGetGenlockSourceI3D;
NN_GLL_API wglGenlockSourceEdgeI3DType g_nnglllGenlockSourceEdgeI3D;
NN_GLL_API wglGetGenlockSourceEdgeI3DType g_nnglllGetGenlockSourceEdgeI3D;
NN_GLL_API wglGenlockSampleRateI3DType g_nnglllGenlockSampleRateI3D;
NN_GLL_API wglGetGenlockSampleRateI3DType g_nnglllGetGenlockSampleRateI3D;
NN_GLL_API wglGenlockSourceDelayI3DType g_nnglllGenlockSourceDelayI3D;
NN_GLL_API wglGetGenlockSourceDelayI3DType g_nnglllGetGenlockSourceDelayI3D;
NN_GLL_API wglQueryGenlockMaxSourceDelayI3DType g_nnglllQueryGenlockMaxSourceDelayI3D;
NN_GLL_API GLboolean nngllIsSupported_WGL_I3D_genlock();
#endif // WGL_I3D_genlock

#if !defined(WGL_I3D_image_buffer)
#define WGL_I3D_image_buffer

#define WGL_IMAGE_BUFFER_MIN_ACCESS_I3D 0x00000001 // NOLINT
#define WGL_IMAGE_BUFFER_LOCK_I3D 0x00000002 // NOLINT
typedef LPVOID ( WINAPI *wglCreateImageBufferI3DType )( HDC hDC, DWORD dwSize, UINT uFlags );
typedef BOOL ( WINAPI *wglDestroyImageBufferI3DType )( HDC hDC, LPVOID pAddress );
typedef BOOL ( WINAPI *wglAssociateImageBufferEventsI3DType )( HDC hDC, const HANDLE *pEvent, const LPVOID *pAddress, const DWORD *pSize, UINT count );
typedef BOOL ( WINAPI *wglReleaseImageBufferEventsI3DType )( HDC hDC, const LPVOID *pAddress, UINT count );
#define wglCreateImageBufferI3D g_nnglllCreateImageBufferI3D
#define wglDestroyImageBufferI3D g_nnglllDestroyImageBufferI3D
#define wglAssociateImageBufferEventsI3D g_nnglllAssociateImageBufferEventsI3D
#define wglReleaseImageBufferEventsI3D g_nnglllReleaseImageBufferEventsI3D
NN_GLL_API wglCreateImageBufferI3DType g_nnglllCreateImageBufferI3D;
NN_GLL_API wglDestroyImageBufferI3DType g_nnglllDestroyImageBufferI3D;
NN_GLL_API wglAssociateImageBufferEventsI3DType g_nnglllAssociateImageBufferEventsI3D;
NN_GLL_API wglReleaseImageBufferEventsI3DType g_nnglllReleaseImageBufferEventsI3D;
NN_GLL_API GLboolean nngllIsSupported_WGL_I3D_image_buffer();
#endif // WGL_I3D_image_buffer

#if !defined(WGL_I3D_swap_frame_lock)
#define WGL_I3D_swap_frame_lock

typedef BOOL ( WINAPI *wglEnableFrameLockI3DType )(  );
typedef BOOL ( WINAPI *wglDisableFrameLockI3DType )(  );
typedef BOOL ( WINAPI *wglIsEnabledFrameLockI3DType )( BOOL *pFlag );
typedef BOOL ( WINAPI *wglQueryFrameLockMasterI3DType )( BOOL *pFlag );
#define wglEnableFrameLockI3D g_nnglllEnableFrameLockI3D
#define wglDisableFrameLockI3D g_nnglllDisableFrameLockI3D
#define wglIsEnabledFrameLockI3D g_nnglllIsEnabledFrameLockI3D
#define wglQueryFrameLockMasterI3D g_nnglllQueryFrameLockMasterI3D
NN_GLL_API wglEnableFrameLockI3DType g_nnglllEnableFrameLockI3D;
NN_GLL_API wglDisableFrameLockI3DType g_nnglllDisableFrameLockI3D;
NN_GLL_API wglIsEnabledFrameLockI3DType g_nnglllIsEnabledFrameLockI3D;
NN_GLL_API wglQueryFrameLockMasterI3DType g_nnglllQueryFrameLockMasterI3D;
NN_GLL_API GLboolean nngllIsSupported_WGL_I3D_swap_frame_lock();
#endif // WGL_I3D_swap_frame_lock

#if !defined(WGL_I3D_swap_frame_usage)
#define WGL_I3D_swap_frame_usage

typedef BOOL ( WINAPI *wglGetFrameUsageI3DType )( float *pUsage );
typedef BOOL ( WINAPI *wglBeginFrameTrackingI3DType )(  );
typedef BOOL ( WINAPI *wglEndFrameTrackingI3DType )(  );
typedef BOOL ( WINAPI *wglQueryFrameTrackingI3DType )( DWORD *pFrameCount, DWORD *pMissedFrames, float *pLastMissedUsage );
#define wglGetFrameUsageI3D g_nnglllGetFrameUsageI3D
#define wglBeginFrameTrackingI3D g_nnglllBeginFrameTrackingI3D
#define wglEndFrameTrackingI3D g_nnglllEndFrameTrackingI3D
#define wglQueryFrameTrackingI3D g_nnglllQueryFrameTrackingI3D
NN_GLL_API wglGetFrameUsageI3DType g_nnglllGetFrameUsageI3D;
NN_GLL_API wglBeginFrameTrackingI3DType g_nnglllBeginFrameTrackingI3D;
NN_GLL_API wglEndFrameTrackingI3DType g_nnglllEndFrameTrackingI3D;
NN_GLL_API wglQueryFrameTrackingI3DType g_nnglllQueryFrameTrackingI3D;
NN_GLL_API GLboolean nngllIsSupported_WGL_I3D_swap_frame_usage();
#endif // WGL_I3D_swap_frame_usage

#if !defined(WGL_NV_copy_image)
#define WGL_NV_copy_image

typedef BOOL ( WINAPI *wglCopyImageSubDataNVType )( HGLRC hSrcRC, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, HGLRC hDstRC, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth );
#define wglCopyImageSubDataNV g_nnglllCopyImageSubDataNV
NN_GLL_API wglCopyImageSubDataNVType g_nnglllCopyImageSubDataNV;
NN_GLL_API GLboolean nngllIsSupported_WGL_NV_copy_image();
#endif // WGL_NV_copy_image

#if !defined(WGL_NV_delay_before_swap)
#define WGL_NV_delay_before_swap

typedef BOOL ( WINAPI *wglDelayBeforeSwapNVType )( HDC hDC, GLfloat seconds );
#define wglDelayBeforeSwapNV g_nnglllDelayBeforeSwapNV
NN_GLL_API wglDelayBeforeSwapNVType g_nnglllDelayBeforeSwapNV;
NN_GLL_API GLboolean nngllIsSupported_WGL_NV_delay_before_swap();
#endif // WGL_NV_delay_before_swap

#if !defined(WGL_NV_DX_interop)
#define WGL_NV_DX_interop

#define WGL_ACCESS_READ_ONLY_NV 0x00000000 // NOLINT
#define WGL_ACCESS_READ_WRITE_NV 0x00000001 // NOLINT
#define WGL_ACCESS_WRITE_DISCARD_NV 0x00000002 // NOLINT
typedef BOOL ( WINAPI *wglDXSetResourceShareHandleNVType )( void *dxObject, HANDLE shareHandle );
typedef HANDLE ( WINAPI *wglDXOpenDeviceNVType )( void *dxDevice );
typedef BOOL ( WINAPI *wglDXCloseDeviceNVType )( HANDLE hDevice );
typedef HANDLE ( WINAPI *wglDXRegisterObjectNVType )( HANDLE hDevice, void *dxObject, GLuint name, GLenum type, GLenum access );
typedef BOOL ( WINAPI *wglDXUnregisterObjectNVType )( HANDLE hDevice, HANDLE hObject );
typedef BOOL ( WINAPI *wglDXObjectAccessNVType )( HANDLE hObject, GLenum access );
typedef BOOL ( WINAPI *wglDXLockObjectsNVType )( HANDLE hDevice, GLint count, HANDLE *hObjects );
typedef BOOL ( WINAPI *wglDXUnlockObjectsNVType )( HANDLE hDevice, GLint count, HANDLE *hObjects );
#define wglDXSetResourceShareHandleNV g_nnglllDXSetResourceShareHandleNV
#define wglDXOpenDeviceNV g_nnglllDXOpenDeviceNV
#define wglDXCloseDeviceNV g_nnglllDXCloseDeviceNV
#define wglDXRegisterObjectNV g_nnglllDXRegisterObjectNV
#define wglDXUnregisterObjectNV g_nnglllDXUnregisterObjectNV
#define wglDXObjectAccessNV g_nnglllDXObjectAccessNV
#define wglDXLockObjectsNV g_nnglllDXLockObjectsNV
#define wglDXUnlockObjectsNV g_nnglllDXUnlockObjectsNV
NN_GLL_API wglDXSetResourceShareHandleNVType g_nnglllDXSetResourceShareHandleNV;
NN_GLL_API wglDXOpenDeviceNVType g_nnglllDXOpenDeviceNV;
NN_GLL_API wglDXCloseDeviceNVType g_nnglllDXCloseDeviceNV;
NN_GLL_API wglDXRegisterObjectNVType g_nnglllDXRegisterObjectNV;
NN_GLL_API wglDXUnregisterObjectNVType g_nnglllDXUnregisterObjectNV;
NN_GLL_API wglDXObjectAccessNVType g_nnglllDXObjectAccessNV;
NN_GLL_API wglDXLockObjectsNVType g_nnglllDXLockObjectsNV;
NN_GLL_API wglDXUnlockObjectsNVType g_nnglllDXUnlockObjectsNV;
NN_GLL_API GLboolean nngllIsSupported_WGL_NV_DX_interop();
#endif // WGL_NV_DX_interop

#if !defined(WGL_NV_DX_interop2)
#define WGL_NV_DX_interop2

NN_GLL_API GLboolean nngllIsSupported_WGL_NV_DX_interop2();
#endif // WGL_NV_DX_interop2

#if !defined(WGL_NV_float_buffer)
#define WGL_NV_float_buffer

#define WGL_FLOAT_COMPONENTS_NV 0x20B0 // NOLINT
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_R_NV 0x20B1 // NOLINT
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RG_NV 0x20B2 // NOLINT
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGB_NV 0x20B3 // NOLINT
#define WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGBA_NV 0x20B4 // NOLINT
#define WGL_TEXTURE_FLOAT_R_NV 0x20B5 // NOLINT
#define WGL_TEXTURE_FLOAT_RG_NV 0x20B6 // NOLINT
#define WGL_TEXTURE_FLOAT_RGB_NV 0x20B7 // NOLINT
#define WGL_TEXTURE_FLOAT_RGBA_NV 0x20B8 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_NV_float_buffer();
#endif // WGL_NV_float_buffer

#if !defined(WGL_NV_gpu_affinity)
#define WGL_NV_gpu_affinity

DECLARE_HANDLE(HGPUNV);
struct _GPU_DEVICE {
    DWORD  cb;
    CHAR   DeviceName[32];
    CHAR   DeviceString[128];
    DWORD  Flags;
    RECT   rcVirtualScreen;
};
typedef struct _GPU_DEVICE *PGPU_DEVICE;
#define ERROR_INCOMPATIBLE_AFFINITY_MASKS_NV 0x20D0 // NOLINT
#define ERROR_MISSING_AFFINITY_MASK_NV 0x20D1 // NOLINT
typedef BOOL ( WINAPI *wglEnumGpusNVType )( UINT iGpuIndex, HGPUNV *phGpu );
typedef BOOL ( WINAPI *wglEnumGpuDevicesNVType )( HGPUNV hGpu, UINT iDeviceIndex, PGPU_DEVICE lpGpuDevice );
typedef HDC ( WINAPI *wglCreateAffinityDCNVType )( const HGPUNV *phGpuList );
typedef BOOL ( WINAPI *wglEnumGpusFromAffinityDCNVType )( HDC hAffinityDC, UINT iGpuIndex, HGPUNV *hGpu );
typedef BOOL ( WINAPI *wglDeleteDCNVType )( HDC hdc );
#define wglEnumGpusNV g_nnglllEnumGpusNV
#define wglEnumGpuDevicesNV g_nnglllEnumGpuDevicesNV
#define wglCreateAffinityDCNV g_nnglllCreateAffinityDCNV
#define wglEnumGpusFromAffinityDCNV g_nnglllEnumGpusFromAffinityDCNV
#define wglDeleteDCNV g_nnglllDeleteDCNV
NN_GLL_API wglEnumGpusNVType g_nnglllEnumGpusNV;
NN_GLL_API wglEnumGpuDevicesNVType g_nnglllEnumGpuDevicesNV;
NN_GLL_API wglCreateAffinityDCNVType g_nnglllCreateAffinityDCNV;
NN_GLL_API wglEnumGpusFromAffinityDCNVType g_nnglllEnumGpusFromAffinityDCNV;
NN_GLL_API wglDeleteDCNVType g_nnglllDeleteDCNV;
NN_GLL_API GLboolean nngllIsSupported_WGL_NV_gpu_affinity();
#endif // WGL_NV_gpu_affinity

#if !defined(WGL_NV_multisample_coverage)
#define WGL_NV_multisample_coverage

#define WGL_COVERAGE_SAMPLES_NV 0x2042 // NOLINT
#define WGL_COLOR_SAMPLES_NV 0x20B9 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_NV_multisample_coverage();
#endif // WGL_NV_multisample_coverage

#if !defined(WGL_NV_present_video)
#define WGL_NV_present_video

DECLARE_HANDLE(HVIDEOOUTPUTDEVICENV);
#define WGL_NUM_VIDEO_SLOTS_NV 0x20F0 // NOLINT
typedef int ( WINAPI *wglEnumerateVideoDevicesNVType )( HDC hDC, HVIDEOOUTPUTDEVICENV *phDeviceList );
typedef BOOL ( WINAPI *wglBindVideoDeviceNVType )( HDC hDC, unsigned int uVideoSlot, HVIDEOOUTPUTDEVICENV hVideoDevice, const int *piAttribList );
typedef BOOL ( WINAPI *wglQueryCurrentContextNVType )( int iAttribute, int *piValue );
#define wglEnumerateVideoDevicesNV g_nnglllEnumerateVideoDevicesNV
#define wglBindVideoDeviceNV g_nnglllBindVideoDeviceNV
#define wglQueryCurrentContextNV g_nnglllQueryCurrentContextNV
NN_GLL_API wglEnumerateVideoDevicesNVType g_nnglllEnumerateVideoDevicesNV;
NN_GLL_API wglBindVideoDeviceNVType g_nnglllBindVideoDeviceNV;
NN_GLL_API wglQueryCurrentContextNVType g_nnglllQueryCurrentContextNV;
NN_GLL_API GLboolean nngllIsSupported_WGL_NV_present_video();
#endif // WGL_NV_present_video

#if !defined(WGL_NV_render_depth_texture)
#define WGL_NV_render_depth_texture

#define WGL_BIND_TO_TEXTURE_DEPTH_NV 0x20A3 // NOLINT
#define WGL_BIND_TO_TEXTURE_RECTANGLE_DEPTH_NV 0x20A4 // NOLINT
#define WGL_DEPTH_TEXTURE_FORMAT_NV 0x20A5 // NOLINT
#define WGL_TEXTURE_DEPTH_COMPONENT_NV 0x20A6 // NOLINT
#define WGL_DEPTH_COMPONENT_NV 0x20A7 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_NV_render_depth_texture();
#endif // WGL_NV_render_depth_texture

#if !defined(WGL_NV_render_texture_rectangle)
#define WGL_NV_render_texture_rectangle

#define WGL_BIND_TO_TEXTURE_RECTANGLE_RGB_NV 0x20A0 // NOLINT
#define WGL_BIND_TO_TEXTURE_RECTANGLE_RGBA_NV 0x20A1 // NOLINT
#define WGL_TEXTURE_RECTANGLE_NV 0x20A2 // NOLINT
NN_GLL_API GLboolean nngllIsSupported_WGL_NV_render_texture_rectangle();
#endif // WGL_NV_render_texture_rectangle

#if !defined(WGL_NV_swap_group)
#define WGL_NV_swap_group

typedef BOOL ( WINAPI *wglJoinSwapGroupNVType )( HDC hDC, GLuint group );
typedef BOOL ( WINAPI *wglBindSwapBarrierNVType )( GLuint group, GLuint barrier );
typedef BOOL ( WINAPI *wglQuerySwapGroupNVType )( HDC hDC, GLuint *group, GLuint *barrier );
typedef BOOL ( WINAPI *wglQueryMaxSwapGroupsNVType )( HDC hDC, GLuint *maxGroups, GLuint *maxBarriers );
typedef BOOL ( WINAPI *wglQueryFrameCountNVType )( HDC hDC, GLuint *count );
typedef BOOL ( WINAPI *wglResetFrameCountNVType )( HDC hDC );
#define wglJoinSwapGroupNV g_nnglllJoinSwapGroupNV
#define wglBindSwapBarrierNV g_nnglllBindSwapBarrierNV
#define wglQuerySwapGroupNV g_nnglllQuerySwapGroupNV
#define wglQueryMaxSwapGroupsNV g_nnglllQueryMaxSwapGroupsNV
#define wglQueryFrameCountNV g_nnglllQueryFrameCountNV
#define wglResetFrameCountNV g_nnglllResetFrameCountNV
NN_GLL_API wglJoinSwapGroupNVType g_nnglllJoinSwapGroupNV;
NN_GLL_API wglBindSwapBarrierNVType g_nnglllBindSwapBarrierNV;
NN_GLL_API wglQuerySwapGroupNVType g_nnglllQuerySwapGroupNV;
NN_GLL_API wglQueryMaxSwapGroupsNVType g_nnglllQueryMaxSwapGroupsNV;
NN_GLL_API wglQueryFrameCountNVType g_nnglllQueryFrameCountNV;
NN_GLL_API wglResetFrameCountNVType g_nnglllResetFrameCountNV;
NN_GLL_API GLboolean nngllIsSupported_WGL_NV_swap_group();
#endif // WGL_NV_swap_group

#if !defined(WGL_NV_video_capture)
#define WGL_NV_video_capture

DECLARE_HANDLE(HVIDEOINPUTDEVICENV);
#define WGL_UNIQUE_ID_NV 0x20CE // NOLINT
#define WGL_NUM_VIDEO_CAPTURE_SLOTS_NV 0x20CF // NOLINT
typedef BOOL ( WINAPI *wglBindVideoCaptureDeviceNVType )( UINT uVideoSlot, HVIDEOINPUTDEVICENV hDevice );
typedef UINT ( WINAPI *wglEnumerateVideoCaptureDevicesNVType )( HDC hDc, HVIDEOINPUTDEVICENV *phDeviceList );
typedef BOOL ( WINAPI *wglLockVideoCaptureDeviceNVType )( HDC hDc, HVIDEOINPUTDEVICENV hDevice );
typedef BOOL ( WINAPI *wglQueryVideoCaptureDeviceNVType )( HDC hDc, HVIDEOINPUTDEVICENV hDevice, int iAttribute, int *piValue );
typedef BOOL ( WINAPI *wglReleaseVideoCaptureDeviceNVType )( HDC hDc, HVIDEOINPUTDEVICENV hDevice );
#define wglBindVideoCaptureDeviceNV g_nnglllBindVideoCaptureDeviceNV
#define wglEnumerateVideoCaptureDevicesNV g_nnglllEnumerateVideoCaptureDevicesNV
#define wglLockVideoCaptureDeviceNV g_nnglllLockVideoCaptureDeviceNV
#define wglQueryVideoCaptureDeviceNV g_nnglllQueryVideoCaptureDeviceNV
#define wglReleaseVideoCaptureDeviceNV g_nnglllReleaseVideoCaptureDeviceNV
NN_GLL_API wglBindVideoCaptureDeviceNVType g_nnglllBindVideoCaptureDeviceNV;
NN_GLL_API wglEnumerateVideoCaptureDevicesNVType g_nnglllEnumerateVideoCaptureDevicesNV;
NN_GLL_API wglLockVideoCaptureDeviceNVType g_nnglllLockVideoCaptureDeviceNV;
NN_GLL_API wglQueryVideoCaptureDeviceNVType g_nnglllQueryVideoCaptureDeviceNV;
NN_GLL_API wglReleaseVideoCaptureDeviceNVType g_nnglllReleaseVideoCaptureDeviceNV;
NN_GLL_API GLboolean nngllIsSupported_WGL_NV_video_capture();
#endif // WGL_NV_video_capture

#if !defined(WGL_NV_video_output)
#define WGL_NV_video_output

DECLARE_HANDLE(HPVIDEODEV);
#define WGL_BIND_TO_VIDEO_RGB_NV 0x20C0 // NOLINT
#define WGL_BIND_TO_VIDEO_RGBA_NV 0x20C1 // NOLINT
#define WGL_BIND_TO_VIDEO_RGB_AND_DEPTH_NV 0x20C2 // NOLINT
#define WGL_VIDEO_OUT_COLOR_NV 0x20C3 // NOLINT
#define WGL_VIDEO_OUT_ALPHA_NV 0x20C4 // NOLINT
#define WGL_VIDEO_OUT_DEPTH_NV 0x20C5 // NOLINT
#define WGL_VIDEO_OUT_COLOR_AND_ALPHA_NV 0x20C6 // NOLINT
#define WGL_VIDEO_OUT_COLOR_AND_DEPTH_NV 0x20C7 // NOLINT
#define WGL_VIDEO_OUT_FRAME 0x20C8 // NOLINT
#define WGL_VIDEO_OUT_FIELD_1 0x20C9 // NOLINT
#define WGL_VIDEO_OUT_FIELD_2 0x20CA // NOLINT
#define WGL_VIDEO_OUT_STACKED_FIELDS_1_2 0x20CB // NOLINT
#define WGL_VIDEO_OUT_STACKED_FIELDS_2_1 0x20CC // NOLINT
typedef BOOL ( WINAPI *wglGetVideoDeviceNVType )( HDC hDC, int numDevices, HPVIDEODEV *hVideoDevice );
typedef BOOL ( WINAPI *wglReleaseVideoDeviceNVType )( HPVIDEODEV hVideoDevice );
typedef BOOL ( WINAPI *wglBindVideoImageNVType )( HPVIDEODEV hVideoDevice, HPBUFFERARB hPbuffer, int iVideoBuffer );
typedef BOOL ( WINAPI *wglReleaseVideoImageNVType )( HPBUFFERARB hPbuffer, int iVideoBuffer );
typedef BOOL ( WINAPI *wglSendPbufferToVideoNVType )( HPBUFFERARB hPbuffer, int iBufferType, unsigned long *pulCounterPbuffer, BOOL bBlock );
typedef BOOL ( WINAPI *wglGetVideoInfoNVType )( HPVIDEODEV hpVideoDevice, unsigned long *pulCounterOutputPbuffer, unsigned long *pulCounterOutputVideo );
#define wglGetVideoDeviceNV g_nnglllGetVideoDeviceNV
#define wglReleaseVideoDeviceNV g_nnglllReleaseVideoDeviceNV
#define wglBindVideoImageNV g_nnglllBindVideoImageNV
#define wglReleaseVideoImageNV g_nnglllReleaseVideoImageNV
#define wglSendPbufferToVideoNV g_nnglllSendPbufferToVideoNV
#define wglGetVideoInfoNV g_nnglllGetVideoInfoNV
NN_GLL_API wglGetVideoDeviceNVType g_nnglllGetVideoDeviceNV;
NN_GLL_API wglReleaseVideoDeviceNVType g_nnglllReleaseVideoDeviceNV;
NN_GLL_API wglBindVideoImageNVType g_nnglllBindVideoImageNV;
NN_GLL_API wglReleaseVideoImageNVType g_nnglllReleaseVideoImageNV;
NN_GLL_API wglSendPbufferToVideoNVType g_nnglllSendPbufferToVideoNV;
NN_GLL_API wglGetVideoInfoNVType g_nnglllGetVideoInfoNV;
NN_GLL_API GLboolean nngllIsSupported_WGL_NV_video_output();
#endif // WGL_NV_video_output

#if !defined(WGL_NV_vertex_array_range)
#define WGL_NV_vertex_array_range

typedef void *( WINAPI *wglAllocateMemoryNVType )( GLsizei size, GLfloat readfreq, GLfloat writefreq, GLfloat priority );
typedef void ( WINAPI *wglFreeMemoryNVType )( void *pointer );
#define wglAllocateMemoryNV g_nnglllAllocateMemoryNV
#define wglFreeMemoryNV g_nnglllFreeMemoryNV
NN_GLL_API wglAllocateMemoryNVType g_nnglllAllocateMemoryNV;
NN_GLL_API wglFreeMemoryNVType g_nnglllFreeMemoryNV;
NN_GLL_API GLboolean nngllIsSupported_WGL_NV_vertex_array_range();
#endif // WGL_NV_vertex_array_range

#if !defined(WGL_OML_sync_control)
#define WGL_OML_sync_control

typedef BOOL ( WINAPI *wglGetSyncValuesOMLType )( HDC hdc, INT64 *ust, INT64 *msc, INT64 *sbc );
typedef BOOL ( WINAPI *wglGetMscRateOMLType )( HDC hdc, INT32 *numerator, INT32 *denominator );
typedef INT64 ( WINAPI *wglSwapBuffersMscOMLType )( HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder );
typedef INT64 ( WINAPI *wglSwapLayerBuffersMscOMLType )( HDC hdc, int fuPlanes, INT64 target_msc, INT64 divisor, INT64 remainder );
typedef BOOL ( WINAPI *wglWaitForMscOMLType )( HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder, INT64 *ust, INT64 *msc, INT64 *sbc );
typedef BOOL ( WINAPI *wglWaitForSbcOMLType )( HDC hdc, INT64 target_sbc, INT64 *ust, INT64 *msc, INT64 *sbc );
#define wglGetSyncValuesOML g_nnglllGetSyncValuesOML
#define wglGetMscRateOML g_nnglllGetMscRateOML
#define wglSwapBuffersMscOML g_nnglllSwapBuffersMscOML
#define wglSwapLayerBuffersMscOML g_nnglllSwapLayerBuffersMscOML
#define wglWaitForMscOML g_nnglllWaitForMscOML
#define wglWaitForSbcOML g_nnglllWaitForSbcOML
NN_GLL_API wglGetSyncValuesOMLType g_nnglllGetSyncValuesOML;
NN_GLL_API wglGetMscRateOMLType g_nnglllGetMscRateOML;
NN_GLL_API wglSwapBuffersMscOMLType g_nnglllSwapBuffersMscOML;
NN_GLL_API wglSwapLayerBuffersMscOMLType g_nnglllSwapLayerBuffersMscOML;
NN_GLL_API wglWaitForMscOMLType g_nnglllWaitForMscOML;
NN_GLL_API wglWaitForSbcOMLType g_nnglllWaitForSbcOML;
NN_GLL_API GLboolean nngllIsSupported_WGL_OML_sync_control();
#endif // WGL_OML_sync_control

#ifdef __cplusplus
} // extern "C"
#endif

