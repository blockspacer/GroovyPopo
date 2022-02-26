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
 * @brief mii の API を宣言します。
 * @details 似顔絵ライブラリに関する機能が含まれています。
 */

#pragma once

/**
 * @namespace nn::mii
 * @brief mii の名前空間です。
 * @details 似顔絵ライブラリに関する機能が含まれています。
 */

#include <nn/mii/mii_Result.h>
 
#include <nn/mii/mii_Common.h>
#include <nn/mii/mii_DrawCommon.h>
#include <nn/mii/mii_CharInfoAccessor.h>

#include <nn/mii/mii_Resource.h>

#include <nn/mii/mii_DrawParam.h>

#include <nn/mii/mii_Faceline.h>
#include <nn/mii/mii_Mask.h>
#include <nn/mii/mii_CharModel.h>

#include <nn/mii/mii_TextureShader.h>
#include <nn/mii/mii_FacelineGpuBuffer.h>
#include <nn/mii/mii_MaskGpuBuffer.h>

#include <nn/mii/mii_IconShader.h>
#include <nn/mii/mii_VariableIconBodyShader.h>
#include <nn/mii/mii_IconBody.h>
#include <nn/mii/mii_IconGpuBuffer.h>
#include <nn/mii/mii_IconBodyGpuBuffer.h>
#include <nn/mii/mii_VariableIconBody.h>
#include <nn/mii/mii_VariableIconBodyDrawParam.h>
#include <nn/mii/mii_VariableIconBodyGpuBuffer.h>

#include <nn/mii/mii_RandomDesc.h>
#include <nn/mii/mii_Database.h>

#include <nn/mii/mii_TransparentAdjuster.h>

#include <nn/mii/mii_Material.h>

#include <nn/mii/mii_Applet.h>
