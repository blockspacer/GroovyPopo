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

#include <nn/atk/atk_Config.h>
#ifdef NN_ATK_CONFIG_ENABLE_DEV

#include <nn/atk/atk_SoundArchiveParametersHook.h>
#include <nn/atk/viewer/atk_Result.h>
#include <nn/atk/viewer/detail/res/atk_ResItemInfo.h>

namespace nn {
namespace atk {

class SoundArchive;

namespace detail {

class SoundArchiveManager;

}

namespace viewer {
namespace detail {

class SoundArchiveEditController;
class ResourceManager;

//---------------------------------------------------------------------------
//! @brief    サウンドアーカイブのパラメータ編集機能を提供するクラスです。
//---------------------------------------------------------------------------
class SoundArchiveParameterEditor
{
    NN_DISALLOW_COPY(SoundArchiveParameterEditor);

public:
    //! @brief  サウンドアーカイブのパラメータアクセスをフックし、パラメータを指し替えます。
    class Hook : public atk::detail::SoundArchiveParametersHook
    {
        NN_DISALLOW_COPY(Hook);

    public:
        explicit Hook() NN_NOEXCEPT : m_Owner(NULL) { }

    public:
        void Initialize(SoundArchiveParameterEditor& owner) NN_NOEXCEPT
        {
            m_Owner = &owner;
        }

    protected:
        //---------------------------------------------------------------------------
        //! @brief         指定アイテムラベルがフック対象かどうかを調べます。
        //!
        //! @param[in]     itemLabel     アイテムラベルを指定します。
        //!
        //! @return        対象の場合は true、対象でない場合は false を返します。
        //---------------------------------------------------------------------------
        virtual bool IsTargetItemImpl(const char* itemLabel) NN_NOEXCEPT NN_OVERRIDE;

        virtual const char* GetItemLabelImpl(SoundArchive::ItemId id) const NN_NOEXCEPT NN_OVERRIDE;
        virtual SoundArchive::ItemId GetItemIdImpl(const char* itemLabel) const NN_NOEXCEPT NN_OVERRIDE;

        virtual SoundArchive::SoundType GetSoundTypeImpl(const char* itemLabel) NN_NOEXCEPT NN_OVERRIDE;

        virtual bool ReadSoundInfoImpl(
            SoundArchive::ItemId soundId,
            SoundArchive::SoundInfo* info ) const NN_NOEXCEPT NN_OVERRIDE;
        virtual bool ReadSoundInfoImpl(
            const char* soundName,
            SoundArchive::SoundInfo* info ) const NN_NOEXCEPT;
        virtual bool ReadSound3DInfoImpl(
            SoundArchive::ItemId soundId,
            SoundArchive::Sound3DInfo* info ) const NN_NOEXCEPT NN_OVERRIDE;
        virtual bool ReadSound3DInfoImpl(
            const char* soundName,
            SoundArchive::Sound3DInfo* info ) const NN_NOEXCEPT;
        virtual bool ReadSequenceSoundInfoImpl(
            SoundArchive::ItemId soundId,
            SoundArchive::SequenceSoundInfo* info ) const NN_NOEXCEPT NN_OVERRIDE;
        virtual bool ReadSequenceSoundInfoImpl(
            const char* soundName,
            SoundArchive::SequenceSoundInfo* info ) const NN_NOEXCEPT;
        virtual bool ReadStreamSoundInfoImpl(
            SoundArchive::ItemId soundId,
            SoundArchive::StreamSoundInfo* info ) const NN_NOEXCEPT NN_OVERRIDE;
        virtual bool ReadStreamSoundInfoImpl(
            const char* soundName,
            SoundArchive::StreamSoundInfo* info ) const NN_NOEXCEPT;
        virtual bool ReadStreamSoundInfo2Impl(
            SoundArchive::ItemId soundId,
            SoundArchive::StreamSoundInfo2* info ) const NN_NOEXCEPT NN_OVERRIDE;
        virtual bool ReadStreamSoundInfo2Impl(
            const char* soundName,
            SoundArchive::StreamSoundInfo2* info ) const NN_NOEXCEPT;
        virtual bool ReadWaveSoundInfoImpl(
            SoundArchive::ItemId soundId,
            SoundArchive::WaveSoundInfo* info ) const NN_NOEXCEPT NN_OVERRIDE;
        virtual bool ReadWaveSoundInfoImpl(
            const char* soundName,
            SoundArchive::WaveSoundInfo* info ) const NN_NOEXCEPT;
        virtual bool ReadSoundUserParamImpl(
            uint32_t* pOutValue,
            SoundArchive::ItemId soundId,
            int index) const NN_NOEXCEPT NN_OVERRIDE;
        virtual bool ReadSoundUserParamImpl(
            uint32_t* pOutValue,
            const char* soundName,
            int index) const NN_NOEXCEPT;
        virtual uint32_t GetSoundUserParamImpl(
            SoundArchive::ItemId soundId,
            uint32_t userParam) const NN_NOEXCEPT NN_OVERRIDE;
        virtual uint32_t GetSoundUserParamImpl(
            const char* soundName,
            uint32_t userParam) const NN_NOEXCEPT;

    private:
        inline const char* GetItemName(SoundArchive::ItemId itemID) const NN_NOEXCEPT;

        bool IsValidDataType(SoundArchive::SoundType soundType, ResDataType dataType) const NN_NOEXCEPT;

        SoundArchive::SoundType ResDataTypeToSoundType(ResDataType dataType) const NN_NOEXCEPT;

    private:
        SoundArchiveParameterEditor* m_Owner;
    };

public:
    SoundArchiveParameterEditor() NN_NOEXCEPT;
    ~SoundArchiveParameterEditor() NN_NOEXCEPT {}

public:
    //! @brief  初期化します。
    //! @param editController TBD
    //! @param resourceManager TBD
    //! @param soundArchive TBD
    //! @return TBD
    Result Initialize(
        SoundArchiveEditController* editController,
        ResourceManager* resourceManager,
        nn::atk::detail::SoundArchiveManager* soundArchiveManager) NN_NOEXCEPT;

    //! @brief  終了処理を行います。
    void Finalize() NN_NOEXCEPT;

    bool IsInitialized() const NN_NOEXCEPT;

    //! @brief  パラメータ編集を開始します。
    void Start() NN_NOEXCEPT;

    //! @brief  パラメータ編集を終了します。
    void Stop() NN_NOEXCEPT;

private:
    Hook                        m_SoundArchiveHook;
    SoundArchiveEditController* m_SoundArchiveEditController;
    ResourceManager*            m_ResourceManager;
    nn::atk::detail::SoundArchiveManager*        m_SoundArchiveManager;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
