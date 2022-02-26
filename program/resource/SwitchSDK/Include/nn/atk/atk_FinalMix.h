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

#include <nn/atk/atk_OutputMixer.h>

namespace nn {
namespace atk {

//! @brief ファイナルミックスを表すクラスです。
//!        ファイナルミックスは 6ch のバス 1 つから構成されます。
//!        エフェクト追加などバス指定が必要な操作にはそのバスが指定されるため、明示的にバス指定を行う必要はありません。
class FinalMix : public OutputMixer
{
public:
    //! @internal
    static size_t GetRequiredMemorySize(bool isEffectEnabled) NN_NOEXCEPT;
    //! @internal
    bool Initialize(nn::audio::AudioRendererConfig* pConfig, int channelCount, bool isEffectEnabled, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @internal
    void Finalize(nn::audio::AudioRendererConfig* pConfig) NN_NOEXCEPT;

    //! @brief 指定したエフェクトをバスに追加します。
    //! @param[in] pEffect    エフェクトです。
    //! @param[in] buffer     エフェクトで利用するバッファです。
    //! @param[in] bufferSize buffer に指定したバッファのサイズです。
    //! @return エフェクトの追加に成功したら true を、失敗したら false を返します。
    //! @pre
    //! - pEffect != nullptr
    //! - buffer != nullptr
    //! - buffer が nn::audio::BufferAlignSize にアラインしている
    bool AppendEffect(EffectBase* pEffect, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @brief 指定したユーザエフェクトをバスに追加します。
    //! @param[in] pEffect    ユーザエフェクトです。
    //! @param[in] buffer     ユーザエフェクトで利用するバッファです。
    //! @param[in] bufferSize buffer に指定したバッファのサイズです。
    //! @return エフェクトの追加に成功したら true を、失敗したら false を返します。
    //! @pre
    //! - pEffect != nullptr
    //! - buffer != nullptr
    //! - buffer が nn::audio::BufferAlignSize にアラインしている
    bool AppendEffect(EffectAux* pEffect, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @brief バスに登録されているエフェクトを削除します。
    //! @param[in] pEffect 削除するエフェクトです。
    //! @return エフェクトの削除に成功した場合は true を、失敗したら false を返します。
    //! @pre
    //! - pEffect != nullptr
    bool RemoveEffect(EffectBase* pEffect) NN_NOEXCEPT;
    //! @brief バスに登録されているユーザエフェクトを削除します。
    //! @param[in] pEffect 削除するユーザエフェクトです。
    //! @return エフェクトの削除に成功した場合は true を、失敗したら false を返します。
    //! @pre
    //! - pEffect != nullptr
    bool RemoveEffect(EffectAux* pEffect) NN_NOEXCEPT;
    //! @brief バスに登録されているエフェクトを全てバスから削除します。
    //! @return エフェクトの削除に成功した場合は true を、失敗したら false を返します。
    bool ClearEffect() NN_NOEXCEPT;
    //! @internal
    bool IsEffectEnabled() const NN_NOEXCEPT;

    //! @internal
    virtual ReceiverType GetReceiverType() const NN_NOEXCEPT NN_OVERRIDE;
    //! @brief チャンネル数を取得します。
    //! @return チャンネル数です。
    virtual int GetChannelCount() const NN_NOEXCEPT NN_OVERRIDE;
    //! @brief バス数を取得します。
    //! @brief バス数です。
    virtual int GetBusCount() const NN_NOEXCEPT NN_OVERRIDE;
    //! @internal
    inline nn::audio::FinalMixType* GetAudioFinalMixInstance() NN_NOEXCEPT
    {
        return &m_FinalMix;
    }
    //! @internal
    virtual void AddReferenceCount(int value) NN_NOEXCEPT NN_OVERRIDE;
    //! @internal
    bool IsSoundSendClampEnabled(int bus) const NN_NOEXCEPT NN_OVERRIDE;
private:
    nn::audio::FinalMixType m_FinalMix;
    std::atomic<uint32_t> m_ReferenceCount;
    int m_ChannelCount;
};

} // namesapace nn::atk
} // namesapace nn
