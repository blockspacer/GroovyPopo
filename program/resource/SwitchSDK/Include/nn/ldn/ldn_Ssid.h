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
#include <nn/nn_Common.h>
#include <nn/nn_SdkAssert.h>

namespace nn { namespace ldn
{
    /**
     * @brief         SSID の最大バイト長です。終端文字は含みません。
     */
    const size_t SsidLengthMax = 32;

    /**
     * @brief         SSID です。生成には @ref MakeSsid 関数を使用してください。
     */
    struct Ssid
    {
        //! SSID のバイト長です。終端文字は含みません。
        uint8_t length;

        //! SSID の生値です。必ず 0 で終端していなければなりません。
        char    raw[SsidLengthMax + 1];
    };
    NN_STATIC_ASSERT(sizeof(Ssid) == 34);

    //! @name SSID 関連
    //! @{

    /**
     * @brief         空の SSID を生成します。
     *
     * @return        生成された SSID です。
     */
    Ssid MakeSsid() NN_NOEXCEPT;

    /**
     * @brief         SSID を生成します。
     *
     * @param[in]     raw           SSID の生値です。 0 で終端された英数字を指定します。
     *
     * @return        生成された SSID です。
     */
    Ssid MakeSsid(const char* raw) NN_NOEXCEPT;

    /**
     * @brief         SSID を生成します。
     *
     * @param[in]     raw           SSID の生値です。
     * @param[in]     length        SSID のバイト長です。
     *
     * @return        生成された SSID です。
     */
    Ssid MakeSsid(const void* raw, size_t length) NN_NOEXCEPT;

    /**
     * @brief         SSID の有効性を検証します。
     *
     * @param[in]     ssid        対象の SSID
     *
     * @return        有効な SSID であれば true です。
     */
    bool IsValidSsid(const Ssid& ssid) NN_NOEXCEPT;

    /**
     * @brief         SSID を比較します。
     *
     * @param[in]     lhs         比較対象の SSID
     * @param[in]     rhs         比較対象の SSID
     *
     * @return        SSID が等しい場合に true です。
     */
    inline bool operator == (const Ssid &lhs, const Ssid &rhs) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(IsValidSsid(lhs));
        NN_SDK_REQUIRES(IsValidSsid(rhs));
        return std::strncmp(lhs.raw, rhs.raw, SsidLengthMax) == 0;
    }

    /**
     * @brief         SSID を比較します。
     *
     * @param[in]     lhs         比較対象の SSID
     * @param[in]     rhs         比較対象の SSID
     *
     * @return        SSID が等しくない場合に true です。
     */
    inline bool operator != (const Ssid &lhs, const Ssid &rhs) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(IsValidSsid(lhs));
        NN_SDK_REQUIRES(IsValidSsid(rhs));
        return std::strncmp(lhs.raw, rhs.raw, SsidLengthMax) != 0;
    }

    //! @}

}} // end of namespace nn::ldn

namespace nn { namespace ldn { namespace detail
{
    // 文字列に変換します。
    size_t ConvertToString(const Ssid& ssid, char* buffer, size_t bufferSize) NN_NOEXCEPT;

    // 文字列に変換されたアドレスの文字数です。
    const size_t StringizedSsidLengthMax = 32;

}}} // namespace nn::ldn::detail
