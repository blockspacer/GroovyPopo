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

#include <cstdint>
#include <nn/atk/atk_Command.h>
#include <nn/atk/atk_Config.h>

#include <nn/os/os_MessageQueueTypes.h>

#include <atomic>

namespace nn {
namespace atk {
namespace detail {

class CommandBuffer
{
public:
    CommandBuffer() NN_NOEXCEPT;
    ~CommandBuffer() NN_NOEXCEPT;

    void Initialize( void* commandBuffer, size_t commandBufferSize ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    void* AllocMemory( size_t size ) NN_NOEXCEPT;
    void FreeMemory( Command* lastCommand ) NN_NOEXCEPT;

    size_t GetCommandBufferSize() const NN_NOEXCEPT;
    size_t GetAllocatableCommandSize() const NN_NOEXCEPT;
    size_t GetAllocatedCommandBufferSize() const NN_NOEXCEPT;

private:
    uint32_t* m_CommandMemoryArea;
    size_t m_CommandMemoryAreaSize;

    uintptr_t m_CommandMemoryAreaBegin;
    uintptr_t m_CommandMemoryAreaEnd;

    // m_CommandMemoryAreaBegin == m_CommandMemoryAreaBegin の時、
    // エリアサイズがゼロなのか、最大なのかを判定するフラグ
    bool m_CommandMemoryAreaZeroFlag;
};

class CommandManager
{
public:
    // コマンド処理の実装関数
    typedef void (*ProcessCommandListFunc)( Command* commandList );

    // コマンド処理をリクエストする関数
    typedef void (*RequestProcessCommandFunc)();

    CommandManager() NN_NOEXCEPT;
    ~CommandManager() NN_NOEXCEPT;

    bool IsAvailable() const NN_NOEXCEPT { return m_Available; }

    static size_t GetRequiredMemSize(size_t commandBufferSize, int queueCount);
    void Initialize( void* buffer, size_t bufferSize, size_t commandBufferSize, int queueCount, ProcessCommandListFunc func ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    void SetRequestProcessCommandFunc( RequestProcessCommandFunc func ) NN_NOEXCEPT { m_pRequestProcessCommandFunc = func; }

    template< typename CommandType >
    CommandType* AllocCommand() NN_NOEXCEPT
    {
        return reinterpret_cast<CommandType*>( AllocMemory( sizeof(CommandType), true ) );
    }
    template< typename CommandType >
    CommandType* AllocCommand( bool forceProcessCommandFlag ) NN_NOEXCEPT
    {
        return reinterpret_cast<CommandType*>( AllocMemory( sizeof(CommandType), forceProcessCommandFlag ) );
    }

    uint32_t PushCommand( Command* command ) NN_NOEXCEPT;

    uint32_t FlushCommand( bool forceFlag ) NN_NOEXCEPT;
    uint32_t FlushCommand( bool forceFlag, bool forceProcessCommandFlag ) NN_NOEXCEPT;
    void RecvCommandReply() NN_NOEXCEPT;
    Command* RecvCommandReplySync() NN_NOEXCEPT;

    void WaitCommandReply( uint32_t tag ) NN_NOEXCEPT;
    bool IsFinishCommand( uint32_t tag ) const NN_NOEXCEPT;

    size_t GetCommandBufferSize() const NN_NOEXCEPT;
    size_t GetAllocatableCommandSize() const NN_NOEXCEPT;
    size_t GetAllocatedCommandBufferSize() const NN_NOEXCEPT;
    int GetAllocatedCommandCount() const NN_NOEXCEPT;
    uint32_t GetCommandListCount() const NN_NOEXCEPT
    {
        return static_cast<const uint32_t>(m_CommandListCount);
    }

    bool ProcessCommand() NN_NOEXCEPT;

private:
    static const uint32_t InvalidCommand = 0xffffffff;

    void* AllocMemory( size_t size, bool forceProcessCommandFlag ) NN_NOEXCEPT;
    void* TryAllocMemory( size_t size ) NN_NOEXCEPT;
    void FinalizeCommandList( Command* command ) NN_NOEXCEPT;

    bool m_Available;
    ProcessCommandListFunc m_pProcessCommandListFunc;
    RequestProcessCommandFunc m_pRequestProcessCommandFunc;

    nn::os::MessageQueueType   m_SendCommandQueue;
    uintptr_t* m_pSendCommandQueueBuffer;
    bool m_IsInitializedSendMessageQueue;
    nn::os::MessageQueueType   m_RecvCommandQueue;
    uintptr_t* m_pRecvCommandQueueBuffer;
    bool m_IsInitializedRecvMessageQueue;

    Command* m_CommandListBegin;
    Command* m_CommandListEnd;

    std::atomic<int> m_CommandListCount;

    uint32_t m_CommandTag;
    uint32_t m_FinishCommandTag;

    CommandBuffer m_CommandBuffer;

    // 充填状況のモニタリング用
    int m_AllocatedCommandCount;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

