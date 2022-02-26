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

#include <cstring>

#include <nn/spy/spy_Config.h>
#ifdef NN_BUILD_CONFIG_SPY_ENABLED

#include <nn/nn_StaticAssert.h>

#include <nn/spy/detail/fnd/binary/spyfnd_Binary.h>
#include <nn/spy/detail/fnd/string/spyfnd_String.h>

#define NN_SPY_PACKET_VERSION(major, minor, micro, patch) \
    static_cast<uint32_t >(                          \
          (static_cast<uint8_t>(major) <<  0)        \
        | (static_cast<uint8_t>(minor) <<  8)        \
        | (static_cast<uint8_t>(micro) << 16)        \
        | (static_cast<uint8_t>(patch) << 24)        \
    )

namespace nn {
namespace spy {
namespace detail {

//! @briefprivate
//! @brief 文字列データ
template<typename TLength, int MaxLength>
struct StringData
{
    TLength length;
    char text[MaxLength];

    void Set(const char* srcText) NN_NOEXCEPT
    {
        if(srcText == NULL)
        {
            length = 0;
            text[0] = '\0';
        }
        else
        {
            nn::spy::detail::fnd::String::Copy(
                this->text,
                MaxLength,
                srcText);
            length = static_cast<TLength>(strlen(this->text));
        }
    }

    //! @brief 設定された文字列を送信するのに必要な最小のデータサイズを求めます。
    //!
    //! @details
    //! データサイズに終端文字は含まれません。
    //!
    //! @return 最小のデータサイズ(バイト数)を返します。
    //!
    size_t GetMinimumDataSize() const NN_NOEXCEPT
    {
        return sizeof(TLength) + sizeof(char) * length;
    }
};

//! @briefprivate
//! @brief 色データ
struct ColorData
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    void Set(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) NN_NOEXCEPT
    {
        this->r = red;
        this->g = green;
        this->b = blue;
        this->a = alpha;
    }
};

//! @briefprivate
//! @brief ID データ
//! パケットサイズが 64bit アーキテクチャの影響を受けないようにするため、データを 32 ビット単位に分割します。
//! バイトオーダーはアーキテクチャの通りです。
struct IdData
{
    uint32_t data[2];

    void Set(const void* value) NN_NOEXCEPT
    {
        NN_STATIC_ASSERT(sizeof(void*) <= sizeof(uint64_t));
        NN_STATIC_ASSERT(sizeof(uint64_t) == 2 * sizeof(uint32_t));

        uint64_t value_ = reinterpret_cast<uint64_t>(value);
        std::memcpy(data, &value_, sizeof(data));
    }
};

//! @briefprivate
//! @brief double 型データ
//! パケットサイズが 64bit アーキテクチャの影響を受けないようにするため、データを 32 ビット単位に分割します。
//! バイトオーダーはアーキテクチャの通りです。
struct DoubleData
{
    uint32_t data[2];

    void Set(double value) NN_NOEXCEPT
    {
        NN_STATIC_ASSERT(sizeof(double) == 2 * sizeof(uint32_t));

        std::memcpy(data, &value, sizeof(data));
    }
};

} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn

#endif // NN_BUILD_CONFIG_SPY_ENABLED
