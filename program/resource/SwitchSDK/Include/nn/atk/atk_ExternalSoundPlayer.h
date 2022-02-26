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

#include <nn/util/util_IntrusiveList.h>
#include <nn/atk/atk_BasicSound.h>
#include <nn/atk/atk_SoundHandle.h>

namespace nn {
namespace atk {

class SoundActor;

namespace detail {

/* ========================================================================
        class definition
   ======================================================================== */

class ExternalSoundPlayer
{
    /* ------------------------------------------------------------------------
            typename definition
       ------------------------------------------------------------------------ */
public:
    typedef util::IntrusiveList<BasicSound, util::IntrusiveListMemberNodeTraits<BasicSound,&BasicSound::m_ExtSoundPlayerPlayLink>> SoundList;

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    ExternalSoundPlayer() NN_NOEXCEPT;
    virtual ~ExternalSoundPlayer() NN_NOEXCEPT;

    void StopAllSound( int fadeFrames ) NN_NOEXCEPT;
    void PauseAllSound( bool flag, int fadeFrames ) NN_NOEXCEPT;
    void PauseAllSound( bool flag, int fadeFrames, PauseMode pauseMode ) NN_NOEXCEPT;

    // 同時再生数
    int GetPlayingSoundCount() const NN_NOEXCEPT { return static_cast<int>( m_SoundList.size()); }
    void SetPlayableSoundCount( int count ) NN_NOEXCEPT;
    int GetPlayableSoundCount() const NN_NOEXCEPT { return m_PlayableCount; }

    // プレイヤーでサウンドを再生可能かどうかを調べる
    virtual bool CanPlaySound( int startPriority ) NN_NOEXCEPT;

    // サウンド登録
    bool AppendSound( detail::BasicSound* sound ) NN_NOEXCEPT;
    void RemoveSound( detail::BasicSound* sound ) NN_NOEXCEPT;

    // 全てのサウンドに対する処理
    template< class Function >
    void ForEachSoundRef( Function& function, bool reverse = false ) NN_NOEXCEPT;

    template< class Function >
    Function ForEachSound( Function function, bool reverse = false ) NN_NOEXCEPT;

    void Finalize( SoundActor* actor ) NN_NOEXCEPT;

protected:
    detail::BasicSound* GetLowestPrioritySound() NN_NOEXCEPT;

private:
    SoundList m_SoundList;
    int m_PlayableCount;
};

template< class Function >
inline void ExternalSoundPlayer::ForEachSoundRef( Function& function, bool reverse ) NN_NOEXCEPT
{
    if ( reverse )
    {
        // 再生の新しい順
        for ( SoundList::reverse_iterator itr = m_SoundList.rbegin();
              itr != m_SoundList.rend();
            )
        {
            SoundList::reverse_iterator curItr = itr;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle( &( *curItr ) );
            function( handle );
            if ( handle.IsAttachedSound() ) itr++;
        }
    }
    else
    {
        // 再生の古い順
        for ( SoundList::iterator itr = m_SoundList.begin();
              itr != m_SoundList.end();
            )
        {
            SoundList::iterator curItr = itr++;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle( &( *curItr ) );
            function( handle );
        }
    }
}

template< class Function >
inline Function ExternalSoundPlayer::ForEachSound( Function function, bool reverse ) NN_NOEXCEPT
{
    if ( reverse )
    {
        // 再生の新しい順
        for ( SoundList::reverse_iterator itr = m_SoundList.rbegin();
              itr != m_SoundList.rend();
            )
        {
            SoundList::reverse_iterator curItr = itr;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle( &( *curItr ) );
            function( handle );
            if ( handle.IsAttachedSound() ) itr++;
        }
    }
    else
    {
        // 再生の古い順
        for ( SoundList::iterator itr = m_SoundList.begin();
              itr != m_SoundList.end();
            )
        {
            SoundList::iterator curItr = itr++;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle( &( *curItr ) );
            function( handle );
        }
    }
    return function;
}

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

