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

namespace nn { namespace atk { namespace detail {

class SoundRuntimeUtility
{
public:
    template< typename Sound >
    static Sound* AllocSound(
        detail::SoundInstanceManager< Sound >* manager,
        SoundArchive::ItemId soundId,
        int playerPriority,
        int ambientPriority,
        detail::BasicSound::AmbientInfo* ambientArgInfo,
        OutputReceiver* pOutputReceiver
    ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( manager );

        // サウンドの確保
        Sound* sound = manager->Alloc( playerPriority, ambientPriority, pOutputReceiver );
        if ( sound == NULL ) return NULL;

        // ID設定
        sound->SetId( soundId );

        // アンビエント設定
        if ( ambientArgInfo != NULL )
        {
            sound->SetAmbientInfo( *ambientArgInfo );
        }

        return sound;
    }

};

}}} // namespace nn::atk::detail
