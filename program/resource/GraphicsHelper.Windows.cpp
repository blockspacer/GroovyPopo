/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#include <nn/nn_Assert.h>
#include <nn/nn_Log.h>
#include <nn/vi.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <nn/nn_Windows.h>

#include "GL/glew.h"
#include "GL/wglew.h"

#include "GraphicsHelper.h"

#pragma warning(disable:4996)


namespace
{
    HWND      s_Hwnd;
    HDC       s_Hdc;
    HGLRC     s_Hglrc;

    nn::vi::NativeWindowHandle  s_NativeWindowHandle;
    nn::vi::Display*            s_pDisplay;
    nn::vi::Layer*              s_pLayer;
}

void GraphicsHelper::Initialize()
{
        /*
         * Initialize Video Interface (VI) system to display
         * to the target's screen
         */
    nn::vi::Initialize();

    nn::Result result = nn::vi::OpenDefaultDisplay(&s_pDisplay);
    NN_ASSERT(result.IsSuccess());

    result = nn::vi::CreateLayer(&s_pLayer, s_pDisplay);
    NN_ASSERT(result.IsSuccess());

    nn::vi::SetLayerScalingMode(s_pLayer, nn::vi::ScalingMode_FitToLayer);

    result = nn::vi::GetNativeWindow(&s_NativeWindowHandle, s_pLayer);
    NN_ASSERT(result.IsSuccess());

        /*
         * Initialize WGL
         */
    BOOL wglResult;

    s_Hwnd = static_cast<HWND>(s_NativeWindowHandle);
    s_Hdc = ::GetDC(s_Hwnd);
    s_Hglrc = ::wglCreateContext(s_Hdc);
    wglResult = ::wglMakeCurrent(s_Hdc, s_Hglrc);
    NN_ASSERT(wglResult, "wglMakeCurrent failed.");

        /*
         * Initialize WGLEW
         */
	//::wglewInit();
	::glewInit();

        /*
         * Create new context and set it as current
         */
    GLint attribs[] = {
            /*
             * Require OpenGL ES 3.2 compatibility.
             * If you need other GL version, you should specify it.
             */
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            /*
             * Require OpenGL ES2/ES3 profile
             * If you need OpenGL (not ES), you should specify WGL_CONTEXT_CORE_PROFILE_BIT_ARB or WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB instead of WGL_CONTEXT_ES2_PROFILE_BIT_EXT.
             */
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
            /* For debug callback */
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
        0
    };
    HGLRC hglrcPrev = s_Hglrc;
    s_Hglrc = ::wglCreateContextAttribsARB(s_Hdc, 0, attribs);

    wglResult = ::wglMakeCurrent(NULL, NULL);
    NN_ASSERT(wglResult, "wglMakeCurrent failed.");

    wglResult = ::wglDeleteContext(hglrcPrev);
    NN_ASSERT(wglResult, "wglDeleteContext failed.");

    wglResult = ::wglMakeCurrent(s_Hdc, s_Hglrc);
    NN_ASSERT(wglResult, "wglMakeCurrent failed.");

        /*
        * Initialize GLEW
        */
    ::glewInit();
}

void GraphicsHelper::Finalize()
{
    BOOL wglResult;

    wglResult = ::wglMakeCurrent(NULL, NULL);
    NN_ASSERT(wglResult, "wglMakeCurrent failed.");
    wglResult = ::wglDeleteContext(s_Hglrc);
    NN_ASSERT(wglResult, "wglDeleteContext failed.");

    nn::vi::DestroyLayer(s_pLayer);
    nn::vi::CloseDisplay(s_pDisplay);
    nn::vi::Finalize();
}

void GraphicsHelper::SwapBuffers()
{
    ::SwapBuffers(s_Hdc);
}

