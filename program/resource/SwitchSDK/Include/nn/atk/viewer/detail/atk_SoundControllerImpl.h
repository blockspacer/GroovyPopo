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

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>
#include <nn/atk/fnd/binary/atkfnd_PrimitiveTypes.h>
#include <nn/atk/viewer/atk_Result.h>
#include <nn/atk/viewer/atk_Config.h>
#include <nn/atk/viewer/atk_SoundController.h>
#include <nn/atk/viewer/atk_SoundParameters.h>

namespace nn {
namespace atk {

class SoundHandle;
class SequenceSoundHandle;
class SoundArchivePlayer;

namespace viewer {
namespace detail {

//! @brief  サウンドを制御するクラスです。
class SoundControllerImpl : public viewer::SoundController
{
public: // 型の定義
    // 再生前のアクション
    typedef void (*PreplayAction)(SoundController& target, SoundArchive::ItemId soundID, void* userParam);

private:
    // シーケンス変数操作用関数
    typedef bool (*ReadVariableFunc)(
        SequenceSoundHandle* handle,
        atk::detail::fnd::BinS32 trackNo,
        atk::detail::fnd::BinS32 varNo,
        atk::detail::fnd::BinS32* var
    );
    typedef bool (*WriteVariableFunc)(
        SequenceSoundHandle* handle,
        atk::detail::fnd::BinS32 trackNo,
        atk::detail::fnd::BinS32 varNo,
        atk::detail::fnd::BinS32 var
    );

public: // コンストラクタ
    SoundControllerImpl() NN_NOEXCEPT;
    virtual ~SoundControllerImpl() NN_NOEXCEPT NN_OVERRIDE { }

public: // メソッド
    viewer::Result Initialize(SoundHandle* soundHandle, SoundArchivePlayer* soundArchivePlayer) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    virtual bool IsInitialized() const NN_NOEXCEPT NN_OVERRIDE { return m_SoundHandle != NULL; }

    virtual const SoundHandle* GetSoundHandle() const NN_NOEXCEPT NN_OVERRIDE { return m_SoundHandle; }
    virtual SoundHandle* GetSoundHandle() NN_NOEXCEPT NN_OVERRIDE { return m_SoundHandle; }

    virtual SoundArchive::ItemId GetSoundId() const NN_NOEXCEPT NN_OVERRIDE { return m_SoundId; }
    viewer::Result SetSoundId(SoundArchive::ItemId soundId) NN_NOEXCEPT;

    virtual const char* GetLabel() const NN_NOEXCEPT NN_OVERRIDE;
    viewer::Result SetLabel(const char* label) NN_NOEXCEPT;

    uint32_t GetSoundIndex() const NN_NOEXCEPT;
    viewer::Result SetSoundIndex(uint32_t index) NN_NOEXCEPT;

    virtual State GetState() const NN_NOEXCEPT NN_OVERRIDE { return m_State; }

    virtual viewer::Result Play(const SoundStartable::StartInfo * pStartInfo = NULL) NN_NOEXCEPT NN_OVERRIDE;
    virtual viewer::Result Stop() NN_NOEXCEPT NN_OVERRIDE;
    virtual viewer::Result Pause() NN_NOEXCEPT NN_OVERRIDE;

    void Update() NN_NOEXCEPT;

    virtual const SoundParameters& GetParameters() const NN_NOEXCEPT NN_OVERRIDE { return m_Parameters; }
    virtual SoundParameters& GetParameters() NN_NOEXCEPT NN_OVERRIDE { return m_Parameters; }

    virtual void SetParameterDirty(bool isDirty) NN_NOEXCEPT NN_OVERRIDE;

    void SetPreplayAction(PreplayAction action, void* userParam) NN_NOEXCEPT
    {
        m_PreplayAction = action;
        m_UserParam = userParam;
    }

    // シーケンス変数の操作
    bool GetVariables(ToolSoundSequenceVariableContainer* pVariables) NN_NOEXCEPT;
    bool SetVariables(const ToolSoundSequenceVariableContainer* pVariables) NN_NOEXCEPT;
    void InvalidateAllVariables() NN_NOEXCEPT;

private:
    void ApplySequenceVariables() NN_NOEXCEPT;

    // シーケンス変数の制御
    bool GetVariables(ToolSoundSequenceVariableContainer* pVariables, ReadVariableFunc pFunc) NN_NOEXCEPT;
    bool SetVariables(SequenceVariableContainer& Variables, WriteVariableFunc pFunc) NN_NOEXCEPT;
    void InvalidateVariables(ToolSoundSequenceVariableContainer* pVariables) NN_NOEXCEPT;

private:
    void UpdateState() NN_NOEXCEPT;
    void UpdateParameters() NN_NOEXCEPT;


private: // メンバ変数
    SoundHandle*         m_SoundHandle;
    SoundArchivePlayer*  m_SoundArchivePlayer;
    SoundArchive::ItemId m_SoundId;
    State                m_State;
    bool                 m_IsParameterDirty;
    SoundParameters      m_Parameters;
    atk::detail::fnd::CriticalSection   m_Lock;

    PreplayAction m_PreplayAction;
    void*         m_UserParam;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
