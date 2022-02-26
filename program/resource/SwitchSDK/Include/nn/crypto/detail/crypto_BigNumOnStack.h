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

#include <nn/crypto/detail/crypto_BigNum.h>

namespace nn { namespace crypto { namespace detail {

template <size_t bits>
class BigNumOnStack : public BigNum
{
public:
    static const size_t BitsCount = bits;
    static const size_t DigitsCount = (bits + sizeof(Digit) * 8 - 1) / (sizeof(Digit) * 8);
    static const size_t BytesCount = DigitsCount * sizeof(Digit);

public:
    BigNumOnStack() NN_NOEXCEPT
    {
        ReserveStatic( m_Buffer, DigitsCount );
    }

private:
    Digit m_Buffer[DigitsCount];
};

}}} // namespace nn::crypto::detail
