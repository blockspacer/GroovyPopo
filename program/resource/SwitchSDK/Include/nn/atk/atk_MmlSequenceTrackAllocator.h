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

#include <nn/atk/atk_SequenceTrackAllocator.h>
#include <nn/atk/atk_MmlSequenceTrack.h>
#include <nn/atk/atk_InstancePool.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

class MmlParser;

/* ========================================================================
        class definition
   ======================================================================== */

class MmlSequenceTrackAllocator : public SequenceTrackAllocator
{
    typedef InstancePool<MmlSequenceTrack> MmlSequenceTrackPool;

public:
    explicit MmlSequenceTrackAllocator( MmlParser* parser ) NN_NOEXCEPT : m_pParser( parser ) {}
    void SetMmlParser( MmlParser* parser ) NN_NOEXCEPT { m_pParser = parser; }
    MmlParser* GetMmlParser() NN_NOEXCEPT { return m_pParser; }
    virtual SequenceTrack* AllocTrack( SequenceSoundPlayer* player ) NN_NOEXCEPT NN_OVERRIDE;
    virtual void FreeTrack( SequenceTrack* track ) NN_NOEXCEPT NN_OVERRIDE;
    virtual int GetAllocatableTrackCount() const NN_NOEXCEPT NN_OVERRIDE { return m_TrackPool.Count(); }

    int Create( void* buffer, size_t size ) NN_NOEXCEPT;
    void Destroy() NN_NOEXCEPT;

private:
    MmlParser* m_pParser;
    MmlSequenceTrackPool m_TrackPool;
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

