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

#include <nn/nn_Common.h>

namespace nn { namespace util {

//! @brief  アクセサの基底クラスです。
//! @tparam T   データ構造体の型です。
template< typename T >
class AccessorBase : protected T
{
protected:
    AccessorBase();

public:
    //! @brief データ構造体の型です。
    typedef T value_type;

    //! @brief  データ構造体への参照を返します。
    //! @return データ構造体への参照を返します。
    value_type& ToData() NN_NOEXCEPT
    {
        return *this;
    }

    //! @brief  データ構造体への参照を返します。
    //! @return データ構造体への参照を返します。
    const value_type& ToData() const NN_NOEXCEPT
    {
        return *this;
    }
};

}} // namespace nn::util
