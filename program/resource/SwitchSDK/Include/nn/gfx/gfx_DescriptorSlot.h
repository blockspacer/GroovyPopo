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
* @brief デスクリプタースロットに関する API の宣言
*/

#pragma once

#include <nn/nn_Common.h>

#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

/**
* @brief デスクリプタースロットを表す構造体です。
*/
struct DescriptorSlotData
{
    uint64_t value; //!< デスクリプタースロットの値です
};

/**
* @brief デスクリプタースロットを表すクラスです。
*/
class DescriptorSlot
    : public detail::DataContainer< DescriptorSlotData >
{
public:
    /**
    * @brief コンストラクターです。
    */
    DescriptorSlot() NN_NOEXCEPT
    {
        this->value = InvalidValue;
    }

    /**
    * @brief デスクリプタースロットをオフセットして別のデスクリプターを指すようにします。
    *
    * @param[in] offset オフセット値
    *
    * @details
    * offset には nn::gfx::DescriptorPool::GetDescriptorSlotIncrementSize で取得される値の倍数を指定してください。
    */
    void Offset( ptrdiff_t offset ) NN_NOEXCEPT
    {
        this->value += offset;
    }

    /**
    * @brief 取得された有効なスロットであるかどうかを取得します。
    *
    * @return 取得された有効なスロットであるかどうかの真偽値を返します。
    *
    * @details
    * 指しているデスクリプターの有効性については判定しません。
    */
    bool IsValid() const NN_NOEXCEPT
    {
        return this->value != InvalidValue;
    }

    /**
    * @brief デスクリプタースロットを無効にします。
    *
    * @post
    * - デスクリプタースロットが無効である
    */
    void Invalidate() NN_NOEXCEPT
    {
        this->value = InvalidValue;
    }

private:
    static const uint64_t InvalidValue = static_cast< uint64_t >( -1 );
};

}
}
