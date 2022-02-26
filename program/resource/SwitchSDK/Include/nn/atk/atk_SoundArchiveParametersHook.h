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

namespace nn {
namespace atk {
namespace detail {

//! @brief  サウンドアーカイブパラメータへのアクセスをフックします。
class SoundArchiveParametersHook
{
protected:
    SoundArchiveParametersHook() NN_NOEXCEPT : m_IsEnabled(true) { }
    virtual ~SoundArchiveParametersHook() NN_NOEXCEPT { }

public:
    //---------------------------------------------------------------------------
    //! @brief         フックが有効かどうかを調べます。
    //!
    //! @return        有効な場合は true、無効な場合は false を返します。
    //---------------------------------------------------------------------------
    bool GetIsEnable() const NN_NOEXCEPT { return m_IsEnabled; }

    //---------------------------------------------------------------------------
    //! @brief         フックの有効/無効を設定します。
    //!
    //! @param[in]     value     有効性を指定します。
    //---------------------------------------------------------------------------
    void SetIsEnable(bool value) NN_NOEXCEPT { m_IsEnabled = value; }

    //---------------------------------------------------------------------------
    //! @brief         指定アイテムラベルがフック対象かどうかを調べます。
    //!
    //! @param[in]     itemLabel     アイテムラベルを指定します。
    //!
    //! @return        対象の場合は true、対象でない場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsTargetItem(const char* itemLabel) NN_NOEXCEPT
    {
        return m_IsEnabled ? IsTargetItemImpl(itemLabel) : false;
    }

    const char* GetItemLabel(SoundArchive::ItemId id) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return GetItemLabelImpl(id);
        }

        return NULL;
    }

    SoundArchive::ItemId GetItemId(const char* itemLabel) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return GetItemIdImpl(itemLabel);
        }

        return SoundArchive::InvalidId;
    }

    SoundArchive::SoundType GetSoundType(const char* itemLabel) NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return GetSoundTypeImpl(itemLabel);
        }

        return SoundArchive::SoundType_Invalid;
    }

    bool ReadSoundInfo(SoundArchive::ItemId soundId, SoundArchive::SoundInfo* info) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return ReadSoundInfoImpl(soundId, info);
        }

        return false;
    }

    bool ReadBankInfo(SoundArchive::ItemId bankId, SoundArchive::BankInfo* info) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return ReadBankInfoImpl(bankId, info);
        }

        return false;
    }

    bool ReadPlayerInfo(SoundArchive::ItemId playerId, SoundArchive::PlayerInfo* info) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return ReadPlayerInfoImpl(playerId, info);
        }

        return false;
    }

    bool ReadSoundGroupInfo(
        SoundArchive::ItemId soundGroupId,
        SoundArchive::SoundGroupInfo* info) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return ReadSoundGroupInfoImpl(soundGroupId, info);
        }

        return false;
    }

    bool ReadGroupInfo(SoundArchive::ItemId groupId, SoundArchive::GroupInfo* info) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return ReadGroupInfoImpl(groupId, info);
        }

        return false;
    }

    bool ReadWaveArchiveInfo(
        SoundArchive::ItemId warcId,
        SoundArchive::WaveArchiveInfo* info) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return ReadWaveArchiveInfoImpl(warcId, info);
        }

        return false;
    }

    void ReadFileInfo(
        SoundArchive::FileId id,
        SoundArchive::FileInfo* info,
        int index = 0) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            ReadFileInfoImpl(id, info, index);
        }
    }

    bool ReadSoundArchivePlayerInfo(SoundArchive::SoundArchivePlayerInfo* info) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return ReadSoundArchivePlayerInfoImpl(info);
        }

        return false;
    }

    bool ReadSound3DInfo(
        SoundArchive::ItemId soundId,
        SoundArchive::Sound3DInfo* info) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return ReadSound3DInfoImpl(soundId, info);
        }

        return false;
    }

    bool ReadSequenceSoundInfo(
        SoundArchive::ItemId soundId,
        SoundArchive::SequenceSoundInfo* info) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return ReadSequenceSoundInfoImpl(soundId, info);
        }

        return false;
    }

    bool ReadStreamSoundInfo(
        SoundArchive::ItemId soundId,
        SoundArchive::StreamSoundInfo* info) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return ReadStreamSoundInfoImpl(soundId, info);
        }

        return false;
    }

    bool ReadStreamSoundInfo2(
        SoundArchive::ItemId soundId,
        SoundArchive::StreamSoundInfo2* info ) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return ReadStreamSoundInfo2Impl(soundId, info);
        }

        return false;
    }

    bool ReadWaveSoundInfo(
        SoundArchive::ItemId soundId,
        SoundArchive::WaveSoundInfo* info) const NN_NOEXCEPT
    {
        if(m_IsEnabled)
        {
            return ReadWaveSoundInfoImpl(soundId, info);
        }

        return false;
    }

    bool ReadSoundUserParam(
        uint32_t* pOutValue,
        SoundArchive::ItemId soundId,
        int index) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(pOutValue);
        return m_IsEnabled ? ReadSoundUserParamImpl(pOutValue, soundId, index) : false;
    }

    uint32_t GetSoundUserParam(
        SoundArchive::ItemId soundId,
        uint32_t userParam) const NN_NOEXCEPT
    {
        return m_IsEnabled ? GetSoundUserParamImpl(soundId, userParam) : userParam;
    }

protected:
    //---------------------------------------------------------------------------
    //! @brief         指定アイテムラベルがフック対象かどうかを調べます。
    //!
    //! @param[in]     itemLabel     アイテムラベルを指定します。
    //!
    //! @return        対象の場合は true、対象でない場合は false を返します。
    //---------------------------------------------------------------------------
    virtual bool IsTargetItemImpl(const char* itemLabel) NN_NOEXCEPT
    {
        (void)itemLabel;
        return false;
    }

    virtual const char* GetItemLabelImpl(SoundArchive::ItemId id) const NN_NOEXCEPT
    {
        (void)id;
        return NULL;
    }

    virtual SoundArchive::ItemId GetItemIdImpl(const char* itemLabel) const NN_NOEXCEPT
    {
        (void)itemLabel;
        return SoundArchive::InvalidId;
    }

    virtual SoundArchive::SoundType GetSoundTypeImpl(const char* itemLabel) NN_NOEXCEPT
    {
        (void)itemLabel;
        return SoundArchive::SoundType_Invalid;
    }

    virtual bool ReadSoundInfoImpl(SoundArchive::ItemId soundId, SoundArchive::SoundInfo* info) const NN_NOEXCEPT
    {
        (void)soundId; (void)info;
        return false;
    }

    virtual bool ReadBankInfoImpl(SoundArchive::ItemId bankId, SoundArchive::BankInfo* info) const NN_NOEXCEPT
    {
        (void)bankId; (void)info;
        return false;
    }

    virtual bool ReadPlayerInfoImpl(SoundArchive::ItemId playerId, SoundArchive::PlayerInfo* info) const NN_NOEXCEPT
    {
        (void)playerId; (void)info;
        return false;
    }

    virtual bool ReadSoundGroupInfoImpl(
        SoundArchive::ItemId soundGroupId,
        SoundArchive::SoundGroupInfo* info) const NN_NOEXCEPT
    {
        (void)soundGroupId; (void)info;
        return false;
    }

    virtual bool ReadGroupInfoImpl(SoundArchive::ItemId groupId, SoundArchive::GroupInfo* info) const NN_NOEXCEPT
    {
        (void)groupId; (void)info;
        return false;
    }

    virtual bool ReadWaveArchiveInfoImpl(
        SoundArchive::ItemId warcId,
        SoundArchive::WaveArchiveInfo* info) const NN_NOEXCEPT
    {
        (void)warcId; (void)info;
        return false;
    }

    virtual void ReadFileInfoImpl(
        SoundArchive::FileId id,
        SoundArchive::FileInfo* info,
        int index = 0) const NN_NOEXCEPT
    {
        (void)id; (void)info; (void)index;
    }

    virtual bool ReadSoundArchivePlayerInfoImpl(SoundArchive::SoundArchivePlayerInfo* info) const NN_NOEXCEPT
    {
        (void)info;
        return false;
    }

    virtual bool ReadSound3DInfoImpl(
        SoundArchive::ItemId soundId,
        SoundArchive::Sound3DInfo* info) const NN_NOEXCEPT
    {
        (void)soundId; (void)info;
        return false;
    }

    virtual bool ReadSequenceSoundInfoImpl(
        SoundArchive::ItemId soundId,
        SoundArchive::SequenceSoundInfo* info) const NN_NOEXCEPT
    {
        (void)soundId; (void)info;
        return false;
    }

    virtual bool ReadStreamSoundInfoImpl(
        SoundArchive::ItemId soundId,
        SoundArchive::StreamSoundInfo* info) const NN_NOEXCEPT
    {
        (void)soundId; (void)info;
        return false;
    }

    virtual bool ReadStreamSoundInfo2Impl(
        SoundArchive::ItemId soundId,
        SoundArchive::StreamSoundInfo2* info) const NN_NOEXCEPT
    {
        (void)soundId; (void)info;
        return false;
    }

    virtual bool ReadWaveSoundInfoImpl(
        SoundArchive::ItemId soundId,
        SoundArchive::WaveSoundInfo* info) const NN_NOEXCEPT
    {
        (void)soundId; (void)info;
        return false;
    }

    virtual bool ReadSoundUserParamImpl(
        uint32_t* pOutValue,
        SoundArchive::ItemId soundId,
        int index) const NN_NOEXCEPT
    {
        (void)soundId; (void)index; (void)pOutValue;
        return false;
    }

    virtual uint32_t GetSoundUserParamImpl(
        SoundArchive::ItemId soundId,
        uint32_t userParam) const NN_NOEXCEPT
    {
        (void)soundId; (void)userParam;
        return userParam;
    }


private:
    bool    m_IsEnabled;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t m_Padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
