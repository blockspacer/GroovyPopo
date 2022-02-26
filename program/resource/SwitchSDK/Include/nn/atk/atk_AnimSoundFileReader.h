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

#include <nn/atk/atk_AnimSoundFile.h>

namespace nn {
namespace atk {
namespace detail {

class AnimSoundFileReader
{
public:
    AnimSoundFileReader() NN_NOEXCEPT;
    bool Initialize( const void* bfasdFile ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;
    bool IsAvailable() const NN_NOEXCEPT { return m_pAnimEventTable != NULL; }

    uint32_t GetFrameSize() const NN_NOEXCEPT
    {
        if ( IsAvailable() )
        {
            return m_FrameSize;
        }
        return 0;
    }

    uint32_t GetEventCount() const NN_NOEXCEPT
    {
        if ( IsAvailable() )
        {
            return m_pAnimEventTable->count;
        }
        return 0;
    }

    const AnimSoundFile::AnimEvent* GetAnimEvent( uint32_t eventIndex ) const NN_NOEXCEPT
    {
        if ( IsAvailable() )
        {
            if ( eventIndex >= m_pAnimEventTable->count )
            {
                return NULL;
            }
            return &m_pAnimEventTable->item[eventIndex];
        }
        return NULL;
    }

    void Dump() NN_NOEXCEPT;

private:
    const AnimSoundFile::AnimEventTable* m_pAnimEventTable;
    uint32_t m_FrameSize;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

