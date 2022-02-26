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

#include <nn/nn_Macro.h>

//#define NN_ATK_LOG_CATEGORY_APICALL_AUDIO_LOW_ENABLED

#define NN_ATK_LOG_CATEGORY_APICALL_ATK "ATK>"
#define NN_ATK_LOG_CATEGORY_APICALL_AUDIO "AUDIO>"
#define NN_ATK_LOG_CATEGORY_APICALL_AUDIO_LOW "AUDIOL>"

#if !defined( NN_SDK_BUILD_RELEASE )

#define NN_ATK_LOG(category, ... ) ::nn::atk::detail::Debug_OutputLog( category, __VA_ARGS__ )

#else

#define NN_ATK_LOG( category, ... ) ((void)0)

#endif

#define NN_ATK_LOG_APICALL_ATK( ... ) NN_ATK_LOG( NN_ATK_LOG_CATEGORY_APICALL_ATK, __VA_ARGS__ )
#define NN_ATK_LOG_APICALL_AUDIO( ... ) NN_ATK_LOG( NN_ATK_LOG_CATEGORY_APICALL_AUDIO, __VA_ARGS__ )

#if defined(NN_ATK_LOG_CATEGORY_APICALL_AUDIO_LOW_ENABLED)
#define NN_ATK_LOG_APICALL_AUDIO_LOW( ... ) NN_ATK_LOG( NN_ATK_LOG_CATEGORY_APICALL_AUDIO_LOW, __VA_ARGS__ )
#else
#define NN_ATK_LOG_APICALL_AUDIO_LOW( ... ) ((void)0)
#endif

namespace nn {
namespace atk {

//! @briefprivate
enum DebugWarningFlag
{
    DebugWarningFlag_NotEnoughInstance,
    DebugWarningFlag_NotEnoughSeqsound,
    DebugWarningFlag_NotEnoughStrmsound,
    DebugWarningFlag_NotEnoughWavesound,
    DebugWarningFlag_NotEnoughSeqtrack,
    DebugWarningFlag_NotEnoughStrmchannel
};

//! @briefprivate
//! @param warning :private
//! @param enable :private
void Debug_SetWarningFlag( DebugWarningFlag warning, bool enable ) NN_NOEXCEPT;


/* ========================================================================
        非公開
   ======================================================================== */

namespace detail {

enum DebugSoundType
{
    DebugSoundType_Seqsound,
    DebugSoundType_Strmsound,
    DebugSoundType_Wavesound
};

bool Debug_GetWarningFlag( DebugWarningFlag warning ) NN_NOEXCEPT;
DebugWarningFlag Debug_GetDebugWarningFlagFromSoundType( DebugSoundType type ) NN_NOEXCEPT;
const char* Debug_GetSoundTypeString( DebugSoundType type ) NN_NOEXCEPT;

typedef void (*DebugLogFunc)( const char* text );

extern DebugLogFunc g_DebugLogHookFunc;

inline void Debug_HookLogFunc( DebugLogFunc func ) NN_NOEXCEPT { g_DebugLogHookFunc = func; }

#if !defined( NN_SDK_BUILD_RELEASE )
void Debug_OutputLog( const char* category, const char* message, ... ) NN_NOEXCEPT;
#endif

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

