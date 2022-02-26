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

#include <nn/audio/audio_MemoryPoolTypes.h>
#include <nn/atk/atk_SoundMemoryAllocatable.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn {
namespace atk {

/* ========================================================================
        type declaration
   ======================================================================== */

class SoundPlayer;

namespace detail {

/* ========================================================================
        class definition
   ======================================================================== */

//! @internal
//! @brief コールバックをリスト管理するためのノードクラスです
class CallbackNode
{
public:
    void SetCallback( SoundMemoryAllocatable::DisposeCallback callback ) NN_NOEXCEPT
    {
        m_Callback = callback;
    }
    void SetCallbackArg( void* arg ) NN_NOEXCEPT
    {
        m_CallbackArg = arg;
    }

    SoundMemoryAllocatable::DisposeCallback GetCallback() NN_NOEXCEPT
    {
        return m_Callback;
    }

    void* GetCallbackArg() NN_NOEXCEPT
    {
        return m_CallbackArg;
    }

public:
    nn::util::IntrusiveListNode m_Link;
private:
    SoundMemoryAllocatable::DisposeCallback m_Callback;
    void* m_CallbackArg;
};

class PlayerHeap : public SoundMemoryAllocatable
{
public:
    PlayerHeap() NN_NOEXCEPT;
    virtual ~PlayerHeap() NN_NOEXCEPT NN_OVERRIDE;

    void AttachSoundPlayer( SoundPlayer* player ) NN_NOEXCEPT { m_pPlayer = player; }

    bool Create( void* startAddress, size_t size ) NN_NOEXCEPT;
    void Destroy() NN_NOEXCEPT;

    // メモリブロックを確保する
    virtual void* Allocate( size_t size ) NN_NOEXCEPT NN_OVERRIDE;
    virtual void* Allocate( size_t size, SoundMemoryAllocatable::DisposeCallback callback, void* callbackArg ) NN_NOEXCEPT NN_OVERRIDE;

    virtual size_t GetAllocateSize(size_t size, bool needMemoryPool) NN_NOEXCEPT NN_OVERRIDE;

    // 確保したメモリブロックを全て解放する
    void Clear() NN_NOEXCEPT;

    // ヒープが有効かどうかを調べる
    bool IsValid() const NN_NOEXCEPT { return m_pAllocAddress != NULL; }

    // ヒープの空き容量を取得する
    size_t GetFreeSize() const NN_NOEXCEPT;

    enum State
    {
        State_Constructed,
        State_TaskRegistered,
        State_TaskFinished,
        State_Destructed
    };
    State GetState() const NN_NOEXCEPT { return static_cast<State>(m_State); }
    void SetState(State state) NN_NOEXCEPT { m_State = static_cast<uint8_t>(state); }

private:
    typedef nn::util::IntrusiveList<CallbackNode, nn::util::IntrusiveListMemberNodeTraits<CallbackNode, &CallbackNode::m_Link>> CallbackList;
private:
    SoundPlayer* m_pPlayer;
    void* m_pStartAddress;
    void* m_pEndAddress;
    void* m_pAllocAddress;
    uint8_t m_State; // enum State
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t m_Padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif


public:
    util::IntrusiveListNode m_Link; // for SoundPlayer::m_PlayerHeapList
    CallbackList m_CallbackList;
};


} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

