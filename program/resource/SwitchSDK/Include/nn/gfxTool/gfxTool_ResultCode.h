/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

// NOLINT(build/header_guard)

#if !defined( NN_GFXTOOL_RESULTCODE_H_ )
#define NN_GFXTOOL_RESULTCODE_H_

#if defined( NN_GFXTOOL )
    #include <nn/gfxTool/gfxTool_EnumHelper.h>
#else
    #define NN_GFXTOOL_DEFINE_ENUM_BEGIN( x ) enum x {
    #define NN_GFXTOOL_DEFINE_ENUM( x ) x,
    #define NN_GFXTOOL_DEFINE_ENUM_END( x ) };
#endif

NN_GFXTOOL_DEFINE_ENUM_BEGIN( nngfxToolResultCode )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_Succeeded )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_Failed )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_InternalError )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_Unsupported )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_CastError )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_DllNotFound )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_DllFunctionNotFound )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_InvalidHandle )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_InvalidArgument )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_LackArgument )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_InvalidApiCall )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_NotEnoughSize )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_XmlParseError )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_XmlNodeNotFound )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_XmlAttributeNotFound )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_XmlTextNotFound )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_FailedToCreateGlContext )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_FailedToGetGlFunction )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_FailedToCreateWindow )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_CompileError )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_SyntaxError )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_InvalidPlatform )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_InvalidApiType )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_InvalidApiVersion )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_InvalidCodeType )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_FailedToLoadFile )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_FailedToCompress )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_FailedToPreprocess )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_InconsistentSpecilizationUniform )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_InvalidVariation )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_InconsistentShaderFile )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_InvalidSourceFormat )
    NN_GFXTOOL_DEFINE_ENUM( nngfxToolResultCode_FailedToWriteFile )
NN_GFXTOOL_DEFINE_ENUM_END( nngfxToolResultCode )

#endif
