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

namespace nn { namespace gfx { namespace util { namespace detail {

// Windows では、NN_ALIGNAS に対しテンプレートパラメータを与えられないため、
// 常に特殊化して使うこと
template<size_t TLength, size_t TAlignment> struct AlignedStorage;

template<size_t TLength>
struct AlignedStorage<TLength, 1>
{
    struct NN_ALIGNAS(1) Type
    {
        unsigned char _data[TLength];
    };
};

template<size_t TLength>
struct AlignedStorage<TLength, 2>
{
    struct NN_ALIGNAS(2) Type
    {
        unsigned char _data[TLength];
    };
};

template<size_t TLength>
struct AlignedStorage<TLength, 4>
{
    struct NN_ALIGNAS(4) Type
    {
        unsigned char _data[TLength];
    };
};

template<size_t TLength>
struct AlignedStorage<TLength, 8>
{
    struct NN_ALIGNAS(8) Type
    {
        unsigned char _data[TLength];
    };
};

template<size_t TLength>
struct AlignedStorage<TLength, 16>
{
    struct NN_ALIGNAS(16) Type
    {
        unsigned char _data[TLength];
    };
};

}}}} // namespace nn::gfx::util::detail
