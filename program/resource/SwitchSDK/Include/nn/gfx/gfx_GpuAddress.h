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
* @brief GPU アドレスに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

/**
* @brief GPU アドレスを表す構造体です。
*/
struct GpuAddressData
{
    uint64_t value; //!< GPU アドレスの値です
    uint64_t impl; //!< 内部的に使われる値です
};

/**
* @brief GPU アドレスを表すクラスです。
*/
class GpuAddress
    : public detail::DataContainer< GpuAddressData >
{
public:
    /**
    * @brief コンストラクターです。
    */
    GpuAddress() NN_NOEXCEPT
    {
    }

    /**
    * @brief アドレスをオフセットします。
    *
    * @param[in] offset バイトでのオフセット値
    */
    void Offset( ptrdiff_t offset ) NN_NOEXCEPT
    {
        this->value += offset;
    }
};

}
}
