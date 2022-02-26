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
#include <nn/atk/atk_SoundHandle.h>

namespace nn {
namespace atk {

class SoundStartable;

namespace detail {

class AnimEventPlayer
{
public:
    AnimEventPlayer() NN_NOEXCEPT;
    ~AnimEventPlayer() NN_NOEXCEPT;

    void Initialize() NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    bool IsPlaying( const AnimSoundFile::EventInfo& eventInfo ) const NN_NOEXCEPT
    {
        return &eventInfo == m_pEventInfo;
    }
    bool IsAttachedSound() const NN_NOEXCEPT { return m_Handle.IsAttachedSound(); }
    int GetPlayingSoundPriority() const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() )
        {
            return 0;
        }
        return m_Handle.detail_GetAttachedSound()->GetPlayerPriority();
    }

    void UpdateFrame() NN_NOEXCEPT;

    bool StartEvent(
            const AnimSoundFile::EventInfo& eventInfo,
            SoundStartable& starter,
            bool isStopWhenFinalize ) NN_NOEXCEPT;
    bool HoldEvent(
            const AnimSoundFile::EventInfo& eventInfo,
            SoundStartable& starter,
            bool isStopWhenFinalize ) NN_NOEXCEPT;
    void StopEvent( const AnimSoundFile::EventInfo& eventInfo ) NN_NOEXCEPT
    {
        if ( m_pEventInfo == &eventInfo )
        {
            ForceStop();
        }
    }

    void ForceStop() NN_NOEXCEPT;
    void WritePlaySpeedToSequenceVariable( uint8_t sequenceVariableNo, float speed ) NN_NOEXCEPT;

private:
    void InitParam( const AnimSoundFile::EventInfo& eventInfo, bool isStopWhenFinalize ) NN_NOEXCEPT;

    SoundHandle m_Handle;
    const AnimSoundFile::EventInfo* m_pEventInfo;
    bool m_IsStopWhenFinalize;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

