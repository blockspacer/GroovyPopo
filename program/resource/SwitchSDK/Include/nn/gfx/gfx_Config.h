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

/**
*   @file
*   @brief  gfx API ライブラリー共通の config 入力
*/

#include <nn/TargetConfigs/build_Base.h>

#define NN_GFX_CONFIG_API_GL NN_GFX_MAKE_API_TYPE_VERSION( NN_GFX_CONFIG_API_TYPE_GL, NN_GFX_CONFIG_GL_DEFAULT_VERSION )
#define NN_GFX_CONFIG_API_GX NN_GFX_MAKE_API_TYPE_VERSION( NN_GFX_CONFIG_API_TYPE_GX, NN_GFX_CONFIG_GX_DEFAULT_VERSION )
#define NN_GFX_CONFIG_API_D3D NN_GFX_MAKE_API_TYPE_VERSION( NN_GFX_CONFIG_API_TYPE_D3D, NN_GFX_CONFIG_D3D_DEFAULT_VERSION )
#define NN_GFX_CONFIG_API_NVN NN_GFX_MAKE_API_TYPE_VERSION( NN_GFX_CONFIG_API_TYPE_NVN, NN_GFX_CONFIG_NVN_DEFAULT_VERSION )
#define NN_GFX_CONFIG_API_VK NN_GFX_MAKE_API_TYPE_VERSION( NN_GFX_CONFIG_API_TYPE_VK, NN_GFX_CONFIG_VK_DEFAULT_VERSION )

#define NN_GFX_CONFIG_API_GL4 NN_GFX_MAKE_API_TYPE_VERSION( NN_GFX_CONFIG_API_TYPE_GL, 4 )
#define NN_GFX_CONFIG_API_GX2 NN_GFX_MAKE_API_TYPE_VERSION( NN_GFX_CONFIG_API_TYPE_GX, 2 )
#define NN_GFX_CONFIG_API_D3D11 NN_GFX_MAKE_API_TYPE_VERSION( NN_GFX_CONFIG_API_TYPE_D3D, 11 )
#define NN_GFX_CONFIG_API_NVN8 NN_GFX_MAKE_API_TYPE_VERSION( NN_GFX_CONFIG_API_TYPE_NVN, 8 )
#define NN_GFX_CONFIG_API_VK1 NN_GFX_MAKE_API_TYPE_VERSION( NN_GFX_CONFIG_API_TYPE_VK, 1 )

// マクロ
#define NN_GFX_MAKE_MASK(bits) ((1<<(bits))-1)
#define NN_GFX_GET_BITS(name, value) (((value)>>(name##_SHIFT))&(name##_MASK))
#define NN_GFX_CHECK_BITS(name, value, check) (NN_GFX_GET_BITS(name, value)==(check))
#define NN_GFX_COMBINE(a, b) a##b

// 低レベル API の種類
#define NN_GFX_CONFIG_API_TYPE_COMMON 0
#define NN_GFX_CONFIG_API_TYPE_GL 1
#define NN_GFX_CONFIG_API_TYPE_GX 2
#define NN_GFX_CONFIG_API_TYPE_D3D 3
#define NN_GFX_CONFIG_API_TYPE_NVN 4
#define NN_GFX_CONFIG_API_TYPE_VK 5

#define NN_GFX_CONFIG_API_TYPE_SHIFT 0
#define NN_GFX_CONFIG_API_TYPE_BITS 8
#define NN_GFX_CONFIG_API_TYPE_MASK NN_GFX_MAKE_MASK( NN_GFX_CONFIG_API_TYPE_BITS )
#define NN_GFX_TARGET_API_TYPE NN_GFX_GET_BITS( NN_GFX_CONFIG_API_TYPE, NN_GFX_CONFIG_TARGET_API )
#define NN_GFX_IS_API_TYPE(check) NN_GFX_CHECK_BITS( NN_GFX_CONFIG_API_TYPE, NN_GFX_CONFIG_TARGET_API, check )
#define NN_GFX_IS_TARGET_GL NN_GFX_IS_API_TYPE( NN_GFX_CONFIG_API_TYPE_GL )
#define NN_GFX_IS_TARGET_GX NN_GFX_IS_API_TYPE( NN_GFX_CONFIG_API_TYPE_GX )
#define NN_GFX_IS_TARGET_D3D NN_GFX_IS_API_TYPE( NN_GFX_CONFIG_API_TYPE_D3D )
#define NN_GFX_IS_TARGET_NVN NN_GFX_IS_API_TYPE( NN_GFX_CONFIG_API_TYPE_NVN )
#define NN_GFX_IS_TARGET_VK NN_GFX_IS_API_TYPE( NN_GFX_CONFIG_API_TYPE_VK )

// 低レベル API のバージョン
#define NN_GFX_CONFIG_API_VERSION_SHIFT ( NN_GFX_CONFIG_API_TYPE_SHIFT + NN_GFX_CONFIG_API_TYPE_BITS )
#define NN_GFX_CONFIG_API_VERSION_WIDTH 8
#define NN_GFX_CONFIG_API_VERSION_MASK NN_GFX_MAKE_MASK( NN_GFX_CONFIG_API_TYPE_BITS )
#define NN_GFX_TARGET_VERSION NN_GFX_GET_BITS( NN_GFX_CONFIG_API_VERSION, NN_GFX_CONFIG_TARGET_API )
#define NN_GFX_IS_TARGET_VERSION(check) NN_GFX_CHECK_BITS( NN_GFX_CONFIG_API_VERSION, NN_GFX_CONFIG_TARGET_API, check )

#define NN_GFX_MAKE_API_TYPE_VERSION(type, version) \
    (((type)<<NN_GFX_CONFIG_API_TYPE_SHIFT) | ((version)<<NN_GFX_CONFIG_API_VERSION_SHIFT))

// デフォルト値
#define NN_GFX_CONFIG_TARGET_WN32_DEFAULT NN_GFX_CONFIG_API_GL
#define NN_GFX_CONFIG_TARGET_COS_DEFAULT NN_GFX_CONFIG_API_GX
#define NN_GFX_CONFIG_TARGET_NX_DEFAULT NN_GFX_CONFIG_API_NVN

#define NN_GFX_CONFIG_GL_DEFAULT_VERSION 4
#define NN_GFX_CONFIG_GX_DEFAULT_VERSION 2
#define NN_GFX_CONFIG_D3D_DEFAULT_VERSION 11
#define NN_GFX_CONFIG_NVN_DEFAULT_VERSION 8
#define NN_GFX_CONFIG_VK_DEFAULT_VERSION 1

// NN_GFX_CONFIG_TARGET_API が未定義の場合
#if !defined( NN_GFX_CONFIG_TARGET_API )
    #if defined( NN_BUILD_CONFIG_SPEC_NX )
        #define NN_GFX_CONFIG_TARGET_API NN_GFX_CONFIG_TARGET_NX_DEFAULT
    #elif defined( NN_BUILD_CONFIG_OS_SUPPORTS_WIN32 )
        #define NN_GFX_CONFIG_TARGET_API NN_GFX_CONFIG_TARGET_WN32_DEFAULT
    #elif defined( NN_BUILD_CONFIG_OS_SUPPORTS_COS )
        #define NN_GFX_CONFIG_TARGET_API NN_GFX_CONFIG_TARGET_COS_DEFAULT
    #else
        #error "適切な低レベルグラフィックス API が見つかりません。"
    #endif
#endif

// 対象 API は必ずインクルード
#if NN_GFX_IS_TARGET_GL
    #if !defined( NN_GFX_CONFIG_INCLUDE_GL )
        #define NN_GFX_CONFIG_INCLUDE_GL 1
    #endif
    #if NN_GFX_IS_TARGET_VERSION( 4 )
        #if !defined( NN_GFX_CONFIG_INCLUDE_GL4 )
            #define NN_GFX_CONFIG_INCLUDE_GL4 1
        #endif
        #if !defined( NN_GFX_CONFIG_GL_TARGET_VERSION )
            #define NN_GFX_CONFIG_GL_TARGET_VERSION 4
        #endif
    #endif
#endif

#if NN_GFX_IS_TARGET_GX
    #if !defined( NN_GFX_CONFIG_INCLUDE_GX )
        #define NN_GFX_CONFIG_INCLUDE_GX 1
    #endif
    #if NN_GFX_IS_TARGET_VERSION( 2 )
        #if !defined( NN_GFX_CONFIG_INCLUDE_GX2 )
            #define NN_GFX_CONFIG_INCLUDE_GX2 1
        #endif
        #if !defined( NN_GFX_CONFIG_GX_TARGET_VERSION )
            #define NN_GFX_CONFIG_GX_TARGET_VERSION 2
        #endif
    #endif
#endif

#if NN_GFX_IS_TARGET_D3D
    #if !defined( NN_GFX_CONFIG_INCLUDE_D3D )
        #define NN_GFX_CONFIG_INCLUDE_D3D 1
    #endif
    #if NN_GFX_IS_TARGET_VERSION( 11 )
        #if !defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
            #define NN_GFX_CONFIG_INCLUDE_D3D11 1
        #endif
        #if !defined( NN_GFX_CONFIG_D3D_TARGET_VERSION )
            #define NN_GFX_CONFIG_D3D_TARGET_VERSION 11
        #endif
    #endif
#endif

#if NN_GFX_IS_TARGET_NVN
    #if !defined( NN_GFX_CONFIG_INCLUDE_NVN )
        #define NN_GFX_CONFIG_INCLUDE_NVN 1
    #endif
    #if NN_GFX_IS_TARGET_VERSION( 8 )
        #if !defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
            #define NN_GFX_CONFIG_INCLUDE_NVN8 1
        #endif
        #if !defined( NN_GFX_CONFIG_NVN_TARGET_VERSION )
            #define NN_GFX_CONFIG_NVN_TARGET_VERSION 8
        #endif
    #endif
#endif

#if NN_GFX_IS_TARGET_VK
    #if !defined( NN_GFX_CONFIG_INCLUDE_VK )
        #define NN_GFX_CONFIG_INCLUDE_VK 1
    #endif
    #if NN_GFX_IS_TARGET_VERSION( 1 )
        #if !defined( NN_GFX_CONFIG_INCLUDE_VK1 )
            #define NN_GFX_CONFIG_INCLUDE_VK1 1
        #endif
        #if !defined( NN_GFX_CONFIG_VK_TARGET_VERSION )
            #define NN_GFX_CONFIG_VK_TARGET_VERSION 1
        #endif
    #endif
#endif

 /*ひとつだけバージョン指定している場合はそれが対象バージョン
#if !defined(NN_GFX_CONFIG_D3D_TARGET_VERSION)
    #if defined(NN_GFX_CONFIG_INCLUDE_D3D11) == 1
        #if defined(NN_GFX_CONFIG_INCLUDE_D3D11)
            #define NN_GFX_CONFIG_D3D_TARGET_VERSION 11
        #endif
    #endif
#endif*/

// NN_GFX_CONFIG_*_TARGET_VERSION が未定義の場合
#if !defined( NN_GFX_CONFIG_GL_TARGET_VERSION )
    #define NN_GFX_CONFIG_GL_TARGET_VERSION NN_GFX_CONFIG_GL_DEFAULT_VERSION
#endif
#if !defined( NN_GFX_CONFIG_GX_TARGET_VERSION )
    #define NN_GFX_CONFIG_GX_TARGET_VERSION NN_GFX_CONFIG_GX_DEFAULT_VERSION
#endif
#if !defined( NN_GFX_CONFIG_D3D_TARGET_VERSION )
    #define NN_GFX_CONFIG_D3D_TARGET_VERSION NN_GFX_CONFIG_D3D_DEFAULT_VERSION
#endif
#if !defined( NN_GFX_CONFIG_NVN_TARGET_VERSION )
    #define NN_GFX_CONFIG_NVN_TARGET_VERSION NN_GFX_CONFIG_NVN_DEFAULT_VERSION
#endif
#if !defined( NN_GFX_CONFIG_VK_TARGET_VERSION )
    #define NN_GFX_CONFIG_VK_TARGET_VERSION NN_GFX_CONFIG_VK_DEFAULT_VERSION
#endif

// バージョン付きで指定されている場合はバージョンなし版を定義
#if !defined( NN_GFX_CONFIG_INCLUDE_GL )
    #if defined( NN_GFX_CONFIG_INCLUDE_GL4 )
        #define NN_GFX_CONFIG_INCLUDE_GL
    #endif
#endif
#if !defined( NN_GFX_CONFIG_INCLUDE_GX )
    #if defined( NN_GFX_CONFIG_INCLUDE_GX2 )
        #define NN_GFX_CONFIG_INCLUDE_GX 1
    #endif
#endif
#if !defined( NN_GFX_CONFIG_INCLUDE_D3D )
    #if defined( NN_GFX_CONFIG_INCLUDE_D3D11 )
        #define NN_GFX_CONFIG_INCLUDE_D3D 1
    #endif
#endif
#if !defined( NN_GFX_CONFIG_INCLUDE_NVN )
    #if defined( NN_GFX_CONFIG_INCLUDE_NVN8 )
        #define NN_GFX_CONFIG_INCLUDE_NVN 1
    #endif
#endif
#if !defined( NN_GFX_CONFIG_INCLUDE_VK )
    #if defined( NN_GFX_CONFIG_INCLUDE_VK1 )
        #define NN_GFX_CONFIG_INCLUDE_VK 1
    #endif
#endif

// 対象バージョンは必ずインクルード
#if defined( NN_GFX_CONFIG_INCLUDE_GL )
    #if !defined( NN_GFX_CONFIG_INCLUDE_GL4 ) && NN_GFX_CONFIG_GL_TARGET_VERSION == 4
        #define NN_GFX_CONFIG_INCLUDE_GL4 1
    #endif
#endif
#if defined( NN_GFX_CONFIG_INCLUDE_GX )
    #if !defined( NN_GFX_CONFIG_INCLUDE_GX2 ) && NN_GFX_CONFIG_GX_TARGET_VERSION == 2
        #define NN_GFX_CONFIG_INCLUDE_GX2 1
    #endif
#endif
#if defined( NN_GFX_CONFIG_INCLUDE_D3D )
    #if !defined( NN_GFX_CONFIG_INCLUDE_D3D11 ) && NN_GFX_CONFIG_D3D_TARGET_VERSION == 11
        #define NN_GFX_CONFIG_INCLUDE_D3D11 1
    #endif
#endif
#if defined( NN_GFX_CONFIG_INCLUDE_NVN )
    #if !defined( NN_GFX_CONFIG_INCLUDE_NVN8 ) && NN_GFX_CONFIG_NVN_TARGET_VERSION == 8
        #define NN_GFX_CONFIG_INCLUDE_NVN8 1
    #endif
#endif
#if defined( NN_GFX_CONFIG_INCLUDE_VK )
    #if !defined( NN_GFX_CONFIG_INCLUDE_VK1 ) && NN_GFX_CONFIG_VK_TARGET_VERSION == 1
        #define NN_GFX_CONFIG_INCLUDE_VK1 1
    #endif
#endif

// 特殊版の名前
#define NN_GFX_GL_SUFFIX Gl
#define NN_GFX_GX_SUFFIX Gx
#define NN_GFX_D3D_SUFFIX D3d
#define NN_GFX_NVN_SUFFIX Nvn
#define NN_GFX_VK_SUFFIX Vk
#define NN_GFX_MAKE_SUFFIX(api, version) NN_GFX_COMBINE( api, version )
#if NN_GFX_IS_TARGET_GL
    #define NN_GFX_TARGET_API_SUFFIX NN_GFX_GL_SUFFIX
#elif NN_GFX_IS_TARGET_GX
    #define NN_GFX_TARGET_API_SUFFIX NN_GFX_GX_SUFFIX
#elif NN_GFX_IS_TARGET_D3D
    #define NN_GFX_TARGET_API_SUFFIX NN_GFX_D3D_SUFFIX
#elif NN_GFX_IS_TARGET_NVN
    #define NN_GFX_TARGET_API_SUFFIX NN_GFX_NVN_SUFFIX
#elif NN_GFX_IS_TARGET_VK
    #define NN_GFX_TARGET_API_SUFFIX NN_GFX_VK_SUFFIX
#endif
#define NN_GFX_GL_TARGET_SUFFIX \
    NN_GFX_MAKE_SUFFIX(NN_GFX_GL_SUFFIX, NN_GFX_CONFIG_GL_TARGET_VERSION)
#define NN_GFX_GX_TARGET_SUFFIX \
    NN_GFX_MAKE_SUFFIX(NN_GFX_GX_SUFFIX, NN_GFX_CONFIG_GX_TARGET_VERSION)
#define NN_GFX_D3D_TARGET_SUFFIX \
    NN_GFX_MAKE_SUFFIX(NN_GFX_D3D_SUFFIX, NN_GFX_CONFIG_D3D_TARGET_VERSION)
#define NN_GFX_NVN_TARGET_SUFFIX \
    NN_GFX_MAKE_SUFFIX(NN_GFX_NVN_SUFFIX, NN_GFX_CONFIG_NVN_TARGET_VERSION)
#define NN_GFX_VK_TARGET_SUFFIX \
    NN_GFX_MAKE_SUFFIX(NN_GFX_VK_SUFFIX, NN_GFX_CONFIG_VK_TARGET_VERSION)
