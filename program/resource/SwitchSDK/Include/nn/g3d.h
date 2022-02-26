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
 * @file
 * @brief g3d の API を宣言します。
 */

#pragma once

/**
 * @namespace nn::g3d
 * @brief g3d の名前空間です。
 *
 * @attention
 * 本名前空間の API を利用するには、
 * NintendoSDK NintendoWare パッケージをインストールする必要があります。
 */

#include <nn/g3d/g3d_ResFile.h>
#include <nn/g3d/g3d_ModelObj.h>
#include <nn/g3d/g3d_SkeletonObj.h>
#include <nn/g3d/g3d_ShapeObj.h>
#include <nn/g3d/g3d_MaterialObj.h>
#include <nn/g3d/g3d_Billboard.h>

#include <nn/g3d/g3d_AnimObj.h>
#include <nn/g3d/g3d_SkeletalAnimObj.h>
#include <nn/g3d/g3d_MaterialAnimObj.h>
#include <nn/g3d/g3d_BoneVisibilityAnimObj.h>
#include <nn/g3d/g3d_ShapeAnimObj.h>
#include <nn/g3d/g3d_SceneAnimObj.h>

#include <nn/g3d/g3d_ShaderObj.h>
#include <nn/g3d/g3d_ShaderUtility.h>

#include <nn/g3d/detail/g3d_Perf.h>
