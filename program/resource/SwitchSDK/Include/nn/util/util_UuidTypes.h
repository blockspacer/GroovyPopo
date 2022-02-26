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

#include <nn/util/util_TFormatString.h>

#include <cstdlib>
#include <cstring>

namespace nn
{
namespace util
{

    /**
     * @brief   RFC 4122 に規定された Universally Unique Identifier (UUID) を表します。
     */
    struct Uuid
    {
        static const uint32_t Size = 16;
        static const uint32_t StringSize = sizeof("12345678-1234-1234-1234-123456789012");

        Bit8 data[Size];

        /**
         * @brief   等価比較演算子のオーバーロードです。
         *
         * @param[in]   lhs     比較対象の Uuid を指定します。
         * @param[in]   rhs     比較対象の Uuid を指定します。
         *
         * @return
         *  比較した両者が等しければ true を、等しくなければ false を返します。
         */
        friend bool operator ==(const Uuid& lhs, const Uuid& rhs) NN_NOEXCEPT
        {
            return std::memcmp(lhs.data, rhs.data, Uuid::Size) == 0;
        }

        /**
         * @brief   非等価比較演算子のオーバーロードです。
         *
         * @param[in]   lhs     比較対象の Uuid を指定します。
         * @param[in]   rhs     比較対象の Uuid を指定します。
         *
         * @return
         *  比較した両者が等しくなければ true を、等しければ false を返します。
         */
        friend bool operator !=(const Uuid& lhs, const Uuid& rhs) NN_NOEXCEPT
        {
            return !(lhs == rhs);
        }

        /**
         * @brief   UUID の文字列表現を取得します。
         *
         * @param[out]  pOutString  文字列を代入するバッファへのポインタを指定します。
         * @param[in]   size        pOutString に渡したバッファのサイズを指定します。
         *
         * @pre
         *  - pOutString != nullptr
         *
         * @return  pOutString を返します。
         *
         * @details
         *  pOutString で渡されたバッファに UUID の文字列表現を書き込みます。
         *  size に 1 以上が渡されているとき pOutString はヌル終端されることが保証されます。
         *  完全な文字列表現を取得するためには inCount に StringSize 以上の値を指定する必要があります。
         */
        const char* ToString(char* pOutString, size_t size) const NN_NOEXCEPT
        {
            util::TSNPrintf(
                pOutString, size, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
                data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]
            );

            return pOutString;
        }

        // for private use
        void FromString(const char* pInString) NN_NOEXCEPT
        {
            // TODO: エラー処理

            char buf[3] = {};
            char *endPtr;
            int i = 0;
            for( ; i<4; ++i, pInString+=2 )
            {
                std::memcpy( buf, pInString, 2 );
                data[i] = static_cast<Bit8>(strtoul( buf, &endPtr, 16 ));
            }
            ++pInString;
            for( ; i<6; ++i, pInString+=2 )
            {
                std::memcpy( buf, pInString, 2 );
                data[i] = static_cast<Bit8>(strtoul( buf, &endPtr, 16 ));
            }
            ++pInString;
            for( ; i<8; ++i, pInString+=2 )
            {
                std::memcpy( buf, pInString, 2 );
                data[i] = static_cast<Bit8>(strtoul( buf, &endPtr, 16 ));
            }
            ++pInString;
            for( ; i<10; ++i, pInString+=2 )
            {
                std::memcpy( buf, pInString, 2 );
                data[i] = static_cast<Bit8>(strtoul( buf, &endPtr, 16 ));
            }
            ++pInString;
            for( ; i<16; ++i, pInString+=2 )
            {
                std::memcpy( buf, pInString, 2 );
                data[i] = static_cast<Bit8>(strtoul( buf, &endPtr, 16 ));
            }
        }
    };

    static const Uuid InvalidUuid = {};

}
}
