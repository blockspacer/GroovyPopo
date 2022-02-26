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

#include <nn/atk/atk_SoundThread.h>
#include <nn/atk/atk_SoundSystem.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn {
namespace atk {
namespace detail {

template < typename Instance >
class LoaderManager : public driver::SoundThread::SoundFrameCallback
{
public:
    typedef util::IntrusiveList<Instance, util::IntrusiveListMemberNodeTraits<Instance, &Instance::m_LinkForLoaderManager>> List;
    typedef typename List::iterator Iterator;

    LoaderManager() NN_NOEXCEPT : m_pBuffer(NULL), m_BufferSize(0) {}
    ~LoaderManager() NN_NOEXCEPT {}

    static size_t GetRequiredMemSize(int count) NN_NOEXCEPT
    {
        return sizeof(Instance) * count;
    }

    int Create(void* buffer, size_t size) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( buffer );

        char* ptr = static_cast<char*>(buffer);
        const int objectCount = static_cast<int>(size / sizeof(Instance));
        for ( auto i = 0; i < objectCount; i++ )
        {
            Instance* instance = new( ptr ) Instance;
            m_FreeList.push_back( *instance );
            ptr += sizeof(Instance);
        }

        m_pBuffer = buffer;
        m_BufferSize = size;
        return objectCount;
    }

    void Destroy() NN_NOEXCEPT
    {
        // m_FreeReqList に Finalize すべき対象が残っている場合は、
        // それらがすべて Finalize できるまで FrameProcess を実行する必要がある
        while (!m_FreeReqList.empty())
        {
#if defined(NN_ATK_CONFIG_ENABLE_VOICE_COMMAND)
            nn::atk::SoundSystem::VoiceCommandProcess(1);
            if (!nn::atk::SoundSystem::detail_IsSoundThreadEnabled())
            {
                nn::atk::SoundSystem::VoiceCommandUpdate(); // SoundThread 無効時は手動でオーディオフレーム更新を行う
            }
#endif
            nn::os::SleepThread(nn::TimeSpan::FromMilliSeconds(detail::driver::HardwareManager::SoundFrameIntervalMsec));
        }

        char* ptr = static_cast<char*>(m_pBuffer);
        const int objectCount = static_cast<int>(m_BufferSize / sizeof(Instance));
        for ( auto i = 0; i < objectCount; i++ )
        {
            Instance* instance = reinterpret_cast<Instance*>(ptr);
            instance->~Instance();
            ptr += sizeof(Instance);
        }

        m_FreeList.clear();
    }

    Instance* Alloc() NN_NOEXCEPT
    {
        if (m_FreeList.empty())
        {
            return NULL;
        }

        Instance* instance = &m_FreeList.front();
        m_FreeList.pop_front();

        return instance;
    }

    void Free(Instance* instance) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(instance);
        if (instance->IsInUse())
        {
            m_FreeReqList.push_back(*instance);
        }
        else
        {
            m_FreeList.push_back(*instance);
        }
    }

    void UpdateFreeReqList() NN_NOEXCEPT
    {
        for (Iterator itr = m_FreeReqList.begin();
             itr != m_FreeReqList.end(); )
        {
            Iterator curItr = itr++;
            if (curItr->IsInUse() == false)  // 使用していない
            {
                m_FreeReqList.erase(curItr);
                m_FreeList.push_back(*curItr);
            }
        }
    }

    int GetFreeCount() const NN_NOEXCEPT { return m_FreeList.GetSize(); }

    void OnBeginSoundFrame() NN_NOEXCEPT NN_OVERRIDE { UpdateFreeReqList(); }

private:
    void* m_pBuffer;
    size_t m_BufferSize;
    List m_FreeList;
    List m_FreeReqList;
}; // class LoaderManager

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

