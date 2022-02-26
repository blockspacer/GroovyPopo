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

#include <nn/util/util_BitFlagSet.h>

namespace nn { namespace util {

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x04>::Mask = {{
    0x0000000000000010ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x05>::Mask = {{
    0x0000000000000020ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x06>::Mask = {{
    0x0000000000000040ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x07>::Mask = {{
    0x0000000000000080ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x08>::Mask = {{
    0x0000000000000100ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x09>::Mask = {{
    0x0000000000000200ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x0A>::Mask = {{
    0x0000000000000400ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x0B>::Mask = {{
    0x0000000000000800ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x0C>::Mask = {{
    0x0000000000001000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x0D>::Mask = {{
    0x0000000000002000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x0E>::Mask = {{
    0x0000000000004000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x0F>::Mask = {{
    0x0000000000008000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x10>::Mask = {{
    0x0000000000010000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x11>::Mask = {{
    0x0000000000020000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x12>::Mask = {{
    0x0000000000040000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x13>::Mask = {{
    0x0000000000080000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x14>::Mask = {{
    0x0000000000100000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x15>::Mask = {{
    0x0000000000200000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x16>::Mask = {{
    0x0000000000400000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x17>::Mask = {{
    0x0000000000800000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x18>::Mask = {{
    0x0000000001000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x19>::Mask = {{
    0x0000000002000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x1A>::Mask = {{
    0x0000000004000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x1B>::Mask = {{
    0x0000000008000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x1C>::Mask = {{
    0x0000000010000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x1D>::Mask = {{
    0x0000000020000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x1E>::Mask = {{
    0x0000000040000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x1F>::Mask = {{
    0x0000000080000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x20>::Mask = {{
    0x0000000100000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x21>::Mask = {{
    0x0000000200000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x22>::Mask = {{
    0x0000000400000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x23>::Mask = {{
    0x0000000800000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x24>::Mask = {{
    0x0000001000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x25>::Mask = {{
    0x0000002000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x26>::Mask = {{
    0x0000004000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x27>::Mask = {{
    0x0000008000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x28>::Mask = {{
    0x0000010000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x29>::Mask = {{
    0x0000020000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x2A>::Mask = {{
    0x0000040000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x2B>::Mask = {{
    0x0000080000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x2C>::Mask = {{
    0x0000100000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x2D>::Mask = {{
    0x0000200000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x2E>::Mask = {{
    0x0000400000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x2F>::Mask = {{
    0x0000800000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x30>::Mask = {{
    0x0001000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x31>::Mask = {{
    0x0002000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x32>::Mask = {{
    0x0004000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x33>::Mask = {{
    0x0008000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x34>::Mask = {{
    0x0010000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x35>::Mask = {{
    0x0020000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x36>::Mask = {{
    0x0040000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x37>::Mask = {{
    0x0080000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x38>::Mask = {{
    0x0100000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x39>::Mask = {{
    0x0200000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x3A>::Mask = {{
    0x0400000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x3B>::Mask = {{
    0x0800000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x3C>::Mask = {{
    0x1000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x3D>::Mask = {{
    0x2000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x3E>::Mask = {{
    0x4000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x3F>::Mask = {{
    0x8000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x40>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000001ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x41>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000002ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x42>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000004ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x43>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000008ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x44>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000010ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x45>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000020ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x46>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000040ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x47>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000080ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x48>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000100ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x49>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000200ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x4A>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000400ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x4B>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000800ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x4C>::Mask = {{
    0x0000000000000000ULL, 0x0000000000001000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x4D>::Mask = {{
    0x0000000000000000ULL, 0x0000000000002000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x4E>::Mask = {{
    0x0000000000000000ULL, 0x0000000000004000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x4F>::Mask = {{
    0x0000000000000000ULL, 0x0000000000008000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x50>::Mask = {{
    0x0000000000000000ULL, 0x0000000000010000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x51>::Mask = {{
    0x0000000000000000ULL, 0x0000000000020000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x52>::Mask = {{
    0x0000000000000000ULL, 0x0000000000040000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x53>::Mask = {{
    0x0000000000000000ULL, 0x0000000000080000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x54>::Mask = {{
    0x0000000000000000ULL, 0x0000000000100000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x55>::Mask = {{
    0x0000000000000000ULL, 0x0000000000200000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x56>::Mask = {{
    0x0000000000000000ULL, 0x0000000000400000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x57>::Mask = {{
    0x0000000000000000ULL, 0x0000000000800000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x58>::Mask = {{
    0x0000000000000000ULL, 0x0000000001000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x59>::Mask = {{
    0x0000000000000000ULL, 0x0000000002000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x5A>::Mask = {{
    0x0000000000000000ULL, 0x0000000004000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x5B>::Mask = {{
    0x0000000000000000ULL, 0x0000000008000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x5C>::Mask = {{
    0x0000000000000000ULL, 0x0000000010000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x5D>::Mask = {{
    0x0000000000000000ULL, 0x0000000020000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x5E>::Mask = {{
    0x0000000000000000ULL, 0x0000000040000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x5F>::Mask = {{
    0x0000000000000000ULL, 0x0000000080000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x60>::Mask = {{
    0x0000000000000000ULL, 0x0000000100000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x61>::Mask = {{
    0x0000000000000000ULL, 0x0000000200000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x62>::Mask = {{
    0x0000000000000000ULL, 0x0000000400000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x63>::Mask = {{
    0x0000000000000000ULL, 0x0000000800000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x64>::Mask = {{
    0x0000000000000000ULL, 0x0000001000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x65>::Mask = {{
    0x0000000000000000ULL, 0x0000002000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x66>::Mask = {{
    0x0000000000000000ULL, 0x0000004000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x67>::Mask = {{
    0x0000000000000000ULL, 0x0000008000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x68>::Mask = {{
    0x0000000000000000ULL, 0x0000010000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x69>::Mask = {{
    0x0000000000000000ULL, 0x0000020000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x6A>::Mask = {{
    0x0000000000000000ULL, 0x0000040000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x6B>::Mask = {{
    0x0000000000000000ULL, 0x0000080000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x6C>::Mask = {{
    0x0000000000000000ULL, 0x0000100000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x6D>::Mask = {{
    0x0000000000000000ULL, 0x0000200000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x6E>::Mask = {{
    0x0000000000000000ULL, 0x0000400000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x6F>::Mask = {{
    0x0000000000000000ULL, 0x0000800000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x70>::Mask = {{
    0x0000000000000000ULL, 0x0001000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x71>::Mask = {{
    0x0000000000000000ULL, 0x0002000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x72>::Mask = {{
    0x0000000000000000ULL, 0x0004000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x73>::Mask = {{
    0x0000000000000000ULL, 0x0008000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x85>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000020ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x87>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000080ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x88>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000100ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x89>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000200ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x8A>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000400ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x8B>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000800ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x8C>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000001000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x90>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000010000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x91>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000020000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x92>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000040000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x93>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000080000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0x94>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000100000ULL, 0x0000000000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0xE0>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000100000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0xE1>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000200000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0xE2>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000400000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0xE3>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000800000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0xE4>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000001000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0xE5>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000002000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0xE6>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000004000000000ULL,
}};

template<>
template<>
const BitFlagSet<256, ::nn::hid::KeyboardKey>
      BitFlagSet<256, ::nn::hid::KeyboardKey>::Flag<0xE7>::Mask = {{
    0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000008000000000ULL,
}};

}} // namespace nn::util
