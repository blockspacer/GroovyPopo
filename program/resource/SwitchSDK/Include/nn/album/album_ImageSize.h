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

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn{ namespace album{

    //! @brief 画像のサイズを表す値です。
    enum ImageSize
    {
        //! @brief 幅 1280 ピクセル、高さ 720 ピクセルです。
        ImageSize_1280x720 = 0,
    };


    //! @brief 指定した ImageSize が表す画像の幅（ピクセル）を取得します。
    //! @param[in] size 幅を取得する ImageSize の値
    //! @pre size は ImageSize で定義された値
    //! @return size が表す画像の幅を返します。
    //! @details
    //! 同じ size に対しては常に一定の値を返します。
    int GetImageWidth(ImageSize size) NN_NOEXCEPT;

    //! @brief 指定した ImageSize が表す画像の高さ（ピクセル）を取得します。
    //! @param[in] size 高さを取得する ImageSize の値
    //! @pre size は ImageSize で定義された値
    //! @return size が表す画像の高さを返します。
    //! @details
    //! 同じ size に対しては常に一定の値を返します。
    int GetImageHeight(ImageSize size) NN_NOEXCEPT;


}}
