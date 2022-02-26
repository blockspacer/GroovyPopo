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

#include <nn/atk/atk_Global.h>

namespace nn {
namespace atk {
namespace detail {

class DspadpcmReader
{
public:
    DspadpcmReader() NN_NOEXCEPT;

    void Initialize( const void* dspadpcmData ) NN_NOEXCEPT
    {
        m_pDspadpcmData = dspadpcmData;
    }
    bool ReadWaveInfo( WaveInfo* info ) const NN_NOEXCEPT;

private:
    const void* m_pDspadpcmData;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

