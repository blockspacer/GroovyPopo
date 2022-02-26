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

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/detail/atk_AddonSoundArchiveContainer.h>
#include <nn/atk/detail/atk_IntrusiveList.h>

namespace nn { namespace atk {
    class SoundArchive;
    class SoundDataManager;
}}

namespace nn { namespace atk { namespace detail {

class SoundArchiveManager
{
public:
    class SnapShot
    {
    public:
        SnapShot(
            const nn::atk::SoundArchive& mainSoundArchive,
            const nn::atk::SoundDataManager& mainSoundDataManager,
            const nn::atk::SoundArchive& currentSoundArchive,
            const nn::atk::SoundDataManager& currentSoundDataManager
        ) NN_NOEXCEPT
        : m_MainSoundArchive(mainSoundArchive)
        , m_MainSoundDataManager(mainSoundDataManager)
        , m_CurrentSoundArchive(currentSoundArchive)
        , m_CurrentSoundDataManager(currentSoundDataManager)
        {
        }

        const nn::atk::SoundArchive& GetMainSoundArchive() const NN_NOEXCEPT
        {
            return m_MainSoundArchive;
        }

        const nn::atk::SoundDataManager& GetMainSoundDataManager() const NN_NOEXCEPT
        {
            return m_MainSoundDataManager;
        }

        const nn::atk::SoundArchive& GetCurrentSoundArchive() const NN_NOEXCEPT
        {
            return m_CurrentSoundArchive;
        }

        const nn::atk::SoundDataManager& GetCurrentSoundDataManager() const NN_NOEXCEPT
        {
            return m_CurrentSoundDataManager;
        }

    private:
        const nn::atk::SoundArchive& m_MainSoundArchive;
        const nn::atk::SoundDataManager& m_MainSoundDataManager;
        const nn::atk::SoundArchive& m_CurrentSoundArchive;
        const nn::atk::SoundDataManager& m_CurrentSoundDataManager;
    };

public:
    SoundArchiveManager() NN_NOEXCEPT;
    ~SoundArchiveManager() NN_NOEXCEPT;

    void Initialize(const nn::atk::SoundArchive* pSoundArchive, const nn::atk::SoundDataManager* pSoundDataManager) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    void Add(AddonSoundArchiveContainer& container) NN_NOEXCEPT;
    void Remove(AddonSoundArchiveContainer& container) NN_NOEXCEPT;

    bool IsAvailable() const NN_NOEXCEPT;

    void ChangeTargetArchive(const char* soundArchiveName = nullptr) NN_NOEXCEPT;

    SoundArchiveManager::SnapShot GetSnapShot() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(m_pMainSoundArchive);
        NN_SDK_ASSERT_NOT_NULL(m_pMainSoundDataManager);
        NN_SDK_ASSERT_NOT_NULL(m_pCurrentSoundArchive);
        NN_SDK_ASSERT_NOT_NULL(m_pCurrentSoundDataManager);

        SnapShot snapShot(
            *m_pMainSoundArchive,
            *m_pMainSoundDataManager,
            *m_pCurrentSoundArchive,
            *m_pCurrentSoundDataManager
        );

        return snapShot;
    }

    const nn::atk::SoundArchive* GetMainSoundArchive() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(m_pMainSoundArchive);
        return m_pMainSoundArchive;
    }

    const nn::atk::SoundDataManager* GetMainSoundDataManager() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(m_pMainSoundDataManager);
        return m_pMainSoundDataManager;
    }

    const nn::atk::SoundArchive* GetCurrentSoundArchive() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(m_pCurrentSoundArchive);
        return m_pCurrentSoundArchive;
    }

    const nn::atk::SoundDataManager* GetCurrentSoundDataManager() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(m_pCurrentSoundDataManager);
        return m_pCurrentSoundDataManager;
    }

    const nn::atk::SoundArchive* GetAddonSoundArchive(const char* soundArchiveName) const NN_NOEXCEPT;

    int GetAddonSoundArchiveCount() const NN_NOEXCEPT
    {
        return m_ContainerList.Count();
    }

    const detail::AddonSoundArchiveContainer* GetAddonSoundArchiveContainer(int index) const NN_NOEXCEPT;

    detail::AddonSoundArchiveContainer* GetAddonSoundArchiveContainer(int index) NN_NOEXCEPT;

    const nn::atk::SoundDataManager* GetAddonSoundDataManager(const char* soundArchiveName) const NN_NOEXCEPT;

    void SetParametersHook(detail::SoundArchiveParametersHook* parametersHook) NN_NOEXCEPT;
    detail::SoundArchiveParametersHook* GetParametersHook() const NN_NOEXCEPT;

private:
    typedef nn::atk::detail::IntrusiveList<AddonSoundArchiveContainer> ContainerList;

private:
    const nn::atk::SoundArchive* m_pMainSoundArchive;
    const nn::atk::SoundDataManager* m_pMainSoundDataManager;

    ContainerList m_ContainerList;

    const nn::atk::SoundArchive* m_pCurrentSoundArchive;
    const nn::atk::SoundDataManager* m_pCurrentSoundDataManager;

    detail::SoundArchiveParametersHook* m_pParametersHook;
};

}}}

