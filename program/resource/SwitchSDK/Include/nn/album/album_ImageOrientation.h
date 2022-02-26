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
 * @file
 * @brief  アルバムファイル操作に関する定義
 */

namespace nn{ namespace album{

    //! @brief 画像の回転を表す値です。
    enum ImageOrientation
    {
        //! @brief 画像の向きとディスプレイの向きが一致していることを表します。
        ImageOrientation_None = 0,
        //! @brief 画像が 90 度回転していることを表します。撮影時にディスプレイの右側がカメラの上方向に一致していたことを表します。
        ImageOrientation_Rotate90 = 1,
        //! @brief 画像が 180 度回転していることを表します。撮影時にディスプレイの下側がカメラの上方向に一致していたことを表します。
        ImageOrientation_Rotate180 = 2,
        //! @brief 画像が 270 度回転していることを表します。撮影時にディスプレイの左側がカメラの上方向に一致していたことを表します。
        ImageOrientation_Rotate270 = 3,
    };

}}
