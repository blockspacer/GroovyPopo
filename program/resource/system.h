#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <nn/nn_Windows.h>

#if defined( NN_BUILD_TARGET_PLATFORM_OS_NN ) && defined( NN_BUILD_APISET_NX )
#include <GLES3/gl32.h>
#else
#include "GL/glew.h"
#include "GL/wglew.h"
#define GL_APIENTRY GLAPIENTRY
#endif

#include <nn/nn_Assert.h>
#include <nn/nn_Log.h>
#include <nn/os.h>
#include <nn/fs.h>
#include <nn/util/util_Vector.h>
#include <nn/util/util_Color.h>
#include <nn/util/util_MathTypes.h>



#include "GraphicsHelper.h"

using namespace nn::util;



GLuint GetShaderProgramId();

void InitSystem();
void UninitSystem();

void SwapBuffers();


