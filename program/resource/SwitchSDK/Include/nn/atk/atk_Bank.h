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

#include <nn/atk/atk_NoteOnCallback.h>      // NoteOnInfo
#include <nn/atk/atk_Channel.h>
#include <nn/atk/atk_PlayerHeapDataManager.h>

namespace nn {
namespace atk {

class SoundArchive;
class SoundArchivePlayer;

namespace detail {

class BankFileReader;
class WaveArchiveFileReader;

namespace driver {

/* ========================================================================
        class definition
   ======================================================================== */

class Bank
{
public:
    /* ------------------------------------------------------------------------
            class members
       ------------------------------------------------------------------------ */
    Bank() NN_NOEXCEPT;
    ~Bank() NN_NOEXCEPT;

    Channel* NoteOn(
        const BankFileReader& bankReader,
        const WaveArchiveFileReader& warcReader,
        const NoteOnInfo& noteOnInfo
    ) const NN_NOEXCEPT;

    static float CalcChannelVelocityVolume(uint8_t velocity) NN_NOEXCEPT;
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

