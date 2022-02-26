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
#include <nn/atk/atk_EffectBase.h>
#include <nn/atk/atk_EffectAux.h>
#include <nn/atk/atk_MoveValue.h>

namespace nn {
namespace atk {

//! @brief サブミックスを表すクラスです
class SubMix : public OutputMixer
{
public:
    //! @brief SubMix クラスが初期化時に要求するアラインメントです。
    static const int SubMixAlignment = OutputMixer::RequiredAlignment;

    //! @brief サブミックスの初期化パラメーターです。
    class SubMixParam
    {
    public:
        //! @brief コンストラクタです。
        SubMixParam() NN_NOEXCEPT;

        //! @brief 出力元のバス数を指定します。
        //! @param[in] count 出力元のバス数です。
        //! @pre 1 <= count <= nn::atk::OutputReceiver::BusCountMax
        void SetSrcBusCount(int count) NN_NOEXCEPT;
        //! @brief 出力元のバス数を取得します。
        //! @return 出力元のバス数です。
        int GetSrcBusCount() const NN_NOEXCEPT;
        //! @brief 出力元のチャンネル数を指定します。
        //! @param[in] count 出力元のチャンネル数です。
        //! @pre 1 <= count <= nn::audio::MixBufferCountMax
        void SetSrcChannelCount(int count) NN_NOEXCEPT;
        //! @brief 出力元のチャンネル数を取得します。
        //! @return count 出力元のチャンネル数です。
        int GetSrcChannelCount() const NN_NOEXCEPT;
        //! @brief 出力先のバス数を指定します。
        //! @param[in] count 出力先のバス数です。
        //! @pre 1 <= count <= nn::atk::OutputReceiver::BusCountMax
        void SetDstBusCount(int count) NN_NOEXCEPT;
        //! @brief 出力先のバス数を取得します。
        //! @return 出力先のバス数です。
        int GetDstBusCount() const NN_NOEXCEPT;
        //! @brief 出力先のチャンネル数を指定します。
        //! @param[in] count 出力先のチャンネル数です。
        //! @pre 1 <= count <= nn::audio::MixBufferCountMax
        void SetDstChannelCount(int count) NN_NOEXCEPT;
        //! @brief 出力先のバス数を取得します。
        //! @return 出力先のチャンネル数です。
        int GetDstChannelCount() const NN_NOEXCEPT;
        //! @briefprivate
        //! @brief 出力先の OutputReceiver を指定します。
        //!        デフォルトでは nullptr ですが、この API に nullptr でない値を指定した場合、
        //!        SetDstBusCount(), SetDstChannelCount() で設定した値を無視し、
        //!        指定した OutputReceiver が持つバス数、チャンネル数を利用して初期化を行います。
        //! @param[in] pOutputReceiver 出力先の OutputReceiver です。
        void SetOutputReceiver(const OutputReceiver* pOutputReceiver) NN_NOEXCEPT;
        //! @briefprivate
        //! @brief 出力先の OutputReceiver を取得します。
        //! @return 出力先の OutputReceiver です。
        const OutputReceiver* GetOutputReceiver() const NN_NOEXCEPT;

        //! @internal
        void SetEffectEnabled(bool isEnabled) NN_NOEXCEPT;

        //! @internal
        bool IsEffectEnabled() const NN_NOEXCEPT;

        //! @briefprivate
        //! @brief サウンドからサブミックスへのセンド量の上限値を 1.0f でクランプするかを指定します。
        //!        true の場合、サウンドからサブミックスへのセンド量は 0.0f ～ 1.0f の範囲でクランプされます。
        //!        false の場合、サウンドからサブミックスへのセンド量の指定が 0.0f 以上の範囲になります。
        //!        デフォルトではすべてのバスで true の設定となります。
        //! @param[in] bus 設定を行うバスです。
        //! @param[in] isEnabled クランプ処理の有効、無効を指定します。
        //! @pre   0 <= bus <= nn::atk::OutputReceiver::BusCountMax
        void SetSoundSendClampEnabled(int bus, bool isEnabled) NN_NOEXCEPT;
        //! @briefprivate
        //! @brief サウンドからサブミックスへのセンド量の上限値を 1.0f でクランプするかを取得します。
        //! @param[in] bus 設定を取得するバスです。
        //! @return クランプ処理が有効かどうかを取得します。
        //! @pre   0 <= bus <= nn::atk::OutputReceiver::BusCountMax
        bool IsSoundSendClampEnabled(int bus) const NN_NOEXCEPT;

    private:
        int m_SrcBusCount;
        int m_SrcChannelCount;
        int m_DstBusCount;
        int m_DstChannelCount;

        const OutputReceiver* m_pOutputReceiver;
        bool m_IsEffectEnabled;
        bool m_IsSoundSendClampEnabledArray[OutputReceiver::BusCountMax];
    };

private:
    //! @internal
    //! バス, チャンネルなどのボリュームを扱う構造体
    class VolumeData
    {
    public:
        //! @internal
        VolumeData() NN_NOEXCEPT;
        //! @internal
        //! 更新します。ボリューム値が変わったときは true を返します。
        bool Update() NN_NOEXCEPT;
        //! @internal
        inline void SetMute(bool isMute) NN_NOEXCEPT
        {
            m_IsDirtyFlag = true;
            m_IsMute = isMute;
        }
        //! @internal
        inline bool IsMute() const NN_NOEXCEPT
        {
            return m_IsMute;
        }
        //! @internal
        inline void SetVolume(float volume, int fadeFrame) NN_NOEXCEPT
        {
            m_IsDirtyFlag = true;
            m_Volume.SetTarget( volume, fadeFrame );
        }
        //! @internal
        inline float GetVolume() const NN_NOEXCEPT
        {
            return m_IsMute ? 0.0f : m_Volume.GetValue();
        }

    private:
        detail::MoveValue<float,int> m_Volume;
        bool m_IsMute;
        bool m_IsPrevMute;
        bool m_IsDirtyFlag;
    };

public:
    //! @brief コンストラクタです。
    SubMix() NN_NOEXCEPT;

    //! @brief                     初期化に必要なバッファサイズを取得します。
    //! @param[in] srcBusCount     出力元のバスの数です。
    //! @param[in] srcChannelCount 出力元のバスのチャンネル数です。
    //! @param[in] dstBusCount     出力先のバスの数です。出力先を途中で変更する場合は、出力先のうち最大のバス数を指定してください。
    //! @param[in] dstChannelCount 出力先のバスのチャンネル数です。出力先を途中で変える場合は、出力先のうち最大のチャンネル数を指定してください。
    //! @return                    初期化に必要なバッファサイズです。
    //! @pre
    //! - SoundSystem が初期化済み
    //! - 1 <= srcBusCount <= (SoundSystem の初期化時に設定した busCountMax)
    //! - 1 <= srcChannelCount <= nn::audio::MixBufferCountMax
    //! - srcChannelCount * srcBusCount <= nn::audio::MixBufferCountMax
    //! - 1 <= dstBusCount <= (SoundSystem の初期化時に設定した busCountMax)
    //! - 1 <= dstChannelCount <= nn::audio::MixBufferCountMax
    //! - dstChannelCount * dstBusCount <= nn::audio::MixBufferCountMax
    static size_t GetRequiredMemorySize(int srcBusCount, int srcChannelCount, int dstBusCount, int dstChannelCount) NN_NOEXCEPT;
    //! @internal
    //! @brief 初期化に必要なバッファサイズを取得します。
    static size_t GetRequiredMemorySize(int srcBusCount, int srcChannelCount, int dstBusCount, int dstChannelCount, bool isEffectEnabled, bool isInternalCall) NN_NOEXCEPT;
    //! @briefprivate
    //! @brief                     初期化に必要なバッファサイズを取得します。
    //! @details                   出力先を途中で変える場合は、出力先のバス数とチャンネル数を直接指定する関数を使用ください。
    //! @param[in] srcBusCount     出力元のバスの数です。
    //! @param[in] srcChannelCount 出力元のバスのチャンネルの数です。
    //! @param[in] pDstReceiver    出力先です。初期化済みの出力先を渡す必要があります。
    //! @return                    初期化に必要なバッファサイズです。
    //! @pre
    //! - SoundSystem が初期化済み
    //! - 1 <= srcBusCount <= (SoundSystem の初期化時に設定した busCountMax)
    //! - 1 <= srcChannelCount <= nn::audio::MixBufferCountMax
    //! - srcChannelCount * srcBusCount <= nn::audio::MixBufferCountMax
    //! - pDstReceiver が初期化されている
    //! - 1 <= pDstReceiver->GetBusCount() <= (SoundSystem の初期化時に設定した busCountMax)
    //! - 1 <= pDstReceiver->GetChannelCount() <= nn::audio::MixBufferCountMax
    //! - pDstReceiver->GetChannelCount() * pDstReceiver->GetBusCount() <= nn::audio::MixBufferCountMax
    static size_t GetRequiredMemorySize(int srcBusCount, int srcChannelCount, const OutputReceiver* pDstReceiver) NN_NOEXCEPT;
    //! @internal
    //! @brief                     初期化に必要なバッファサイズを取得します。
    //! @details                   出力先を途中で変える場合は、出力先のバス数とチャンネル数を直接指定する関数を使用ください。
    //! @param[in] srcBusCount     出力元のバスの数です。
    //! @param[in] srcChannelCount 出力元のバスのチャンネルの数です。
    //! @param[in] pDstReceiver    出力先です。初期化済みの出力先を渡す必要があります。
    //! @param[in] isEffectEnabled エフェクトを有効にするかどうかのフラグです。
    //! @return                    初期化に必要なバッファサイズです。
    //! @pre
    //! - SoundSystem が初期化済み
    //! - 1 <= srcBusCount <= (SoundSystem の初期化時に設定した busCountMax)
    //! - 1 <= srcChannelCount <= nn::audio::MixBufferCountMax
    //! - srcChannelCount * srcBusCount <= nn::audio::MixBufferCountMax
    //! - pDstReceiver が初期化されている
    //! - 1 <= pDstReceiver->GetBusCount() <= (SoundSystem の初期化時に設定した busCountMax)
    //! - 1 <= pDstReceiver->GetChannelCount() <= nn::audio::MixBufferCountMax
    //! - pDstReceiver->GetChannelCount() * pDstReceiver->GetBusCount() <= nn::audio::MixBufferCountMax
    static size_t GetRequiredMemorySize(int srcBusCount, int srcChannelCount, const OutputReceiver* pDstReceiver, bool isEffectEnabled) NN_NOEXCEPT;
    //! @brief                     初期化に必要なバッファサイズを取得します。
    //! @param[in] param           サブミックスの初期化パラメータです。
    //! @return                    初期化に必要なバッファサイズです。
    static size_t GetRequiredMemorySize(const SubMixParam& param) NN_NOEXCEPT;
    //! @brief                     サブミックスとして初期化します。
    //! @details                   バス数 srcBusCount, チャンネル数 srcChannelCount
    //!                            出力先のバス数 dstBusCount, 出力先のチャンネル数 dstChannelCount の
    //!                            サブミックスとして初期化します。
    //!                            サブミックスの出力先を途中で変更する場合は、出力先のうち最大のバス数、最大のチャンネル数を指定してください。
    //! @param[in] srcBusCount     出力元のバスの数です。
    //! @param[in] srcChannelCount 出力元のバスのチャンネルの数です。
    //! @param[in] dstBusCount     出力先のバスの数です。出力先を途中で変更する場合は、出力先のうち最大のバス数を指定してください。
    //! @param[in] dstChannelCount 出力先のチャンネルの数です。出力先を途中で変更する場合は、出力先のうち最大のバス数を指定してください。
    //! @param[in] buffer          初期化に使用するバッファです。
    //! @param[in] bufferSize      初期化に使用するバッファのサイズです。
    //! @return                    初期化に成功した場合は true、失敗した場合は false を返します。
    //! @pre
    //! - SoundSystem が初期化済み
    //! - SubMix が未初期化である
    //! - 1 <= srcBusCount <= (SoundSystem の初期化時に設定した busCountMax)
    //! - 1 <= srcChannelCount <= nn::audio::MixBufferCountMax
    //! - srcChannelCount * srcBusCount <= nn::audio::MixBufferCountMax
    //! - 1 <= dstBusCount <= (SoundSystem の初期化時に設定した busCountMax)
    //! - 1 <= dstChannelCount <= nn::audio::MixBufferCountMax
    //! - dstChannelCount * dstBusCount <= nn::audio::MixBufferCountMax
    //! - buffer != nullptr
    //! - buffer は nn::atk::SubMix::SubMixAlignment にアラインされている
    //! - bufferSize が GetRequiredMemorySize() で取得された値よりも大きい
    //! @post
    //! - SubMix が初期化されている
    bool Initialize(int srcBusCount, int srcChannelCount, int dstBusCount, int dstChannelCount, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @internal
    //! @brief   サブミックスとして初期化します。
    //! @details バス数 srcBusCount, チャンネル数 srcChannelCount
    //!          出力先のバス数の最大数 dstBusCount, 出力先のチャンネルの最大数 dstChannelCount の
    //!          サブミックスとして初期化します。
    bool Initialize(int srcBusCount, int srcChannelCount, int dstBusCount, int dstChannelCount, bool isEffectEnabled, bool isInternalCall, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @briefprivate
    //! @brief   サブミックスとして初期化します。
    //! @details バス数 srcBusCount, チャンネル数 srcChannelCount,
    //!          出力先のバス数とチャンネル数は pDstReceiver から参照するサブミックスとして初期化します。
    //!          サブミックスの出力先を途中で変更する場合は、出力先のバス数とチャンネル数を直接指定する API を使用ください。
    //! @param[in] srcBusCount     出力元のバスの数です。
    //! @param[in] srcChannelCount 出力元のバスのチャンネル数です。
    //! @param[in] pDstReceiver    出力先です。バス数とチャンネル数を参照するために使用します。
    //! @param[in] buffer          初期化に使用するバッファです。
    //! @param[in] bufferSize      初期化に使用するバッファのサイズです。
    //! @return                    初期化に成功した場合は true、失敗した場合は false を返します。
    //! @pre
    //! - SoundSystem が初期化済み
    //! - SubMix が未初期化である
    //! - 1 <= srcBusCount <= (SoundSystem の初期化時に設定した busCountMax)
    //! - 1 <= srcChannelCount <= nn::audio::MixBufferCountMax
    //! - srcChannelCount * srcBusCount <= nn::audio::MixBufferCountMax
    //! - pDstReceiver が初期化されている
    //! - 1 <= pDstReceiver->GetBusCount() <= (SoundSystem の初期化時に設定した busCountMax)
    //! - 1 <= pDstReceiver->GetChannelCount() <= nn::audio::MixBufferCountMax
    //! - pDstReceiver->GetChannelCount() * pDstReceiver->GetBusCount() <= nn::audio::MixBufferCountMax
    //! - buffer != nullptr
    //! - buffer は nn::atk::SubMix::SubMixAlignment にアラインされている
    //! - bufferSize が GetRequiredMemorySize() で取得された値よりも大きい
    //! @post SubMix が初期化されている
    bool Initialize(int srcBusCount, int srcChannelCount, const OutputReceiver* pDstReceiver, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @internal
    //! @brief   サブミックスとして初期化します。
    //! @details バス数 srcBusCount, チャンネル数 srcChannelCount,
    //!          出力先のバス数とチャンネル数を pDstReceiver 参照するサブミックスとして初期化します。
    //!          サブミックスの出力先を途中で変更する場合は、出力先のバス数とチャンネル数を直接指定する API を使用ください。
    //! @param[in] srcBusCount      出力元のバスの数です。
    //! @param[in] srcChannelCouhnt 出力元のバスのチャンネル数です。
    //! @param[in] pDstReceiver     出力先です。バス数とチャンネル数を参照するために使用します。
    //! @param[in] isEffectEnabled  エフェクトを利用するかどうかのフラグです。
    //! @param[in] buffer           初期化に使用するバッファです。
    //! @param[in] bufferSize       初期化に使用するバッファのサイズです。
    //! @return 初期化に成功した場合は true、失敗した場合は false を返します。
    //! @pre
    //! - SoundSystem が初期化済み
    //! - SubMix が未初期化である
    //! - 1 <= srcBusCount <= (SoundSystem の初期化時に設定した busCountMax)
    //! - 1 <= srcChannelCount <= nn::audio::MixBufferCountMax
    //! - srcChannelCount * srcBusCount <= nn::audio::MixBufferCountMax
    //! - pDstReceiver が初期化されている
    //! - 1 <= pDstReceiver->GetBusCount() <= (SoundSystem の初期化時に設定した busCountMax)
    //! - 1 <= pDstReceiver->GetChannelCount() <= nn::audio::MixBufferCountMax
    //! - pDstReceiver->GetChannelCount() * pDstReceiver->GetBusCount() <= nn::audio::MixBufferCountMax
    //! - buffer != nullptr
    //! - buffer は nn::atk::SubMix::SubMixAlignment にアラインされている
    //! - bufferSize が GetRequiredMemorySize() で取得された値よりも大きい
    //! @post SubMix が初期化されている
    bool Initialize(int srcBusCount, int srcChannelCount, const OutputReceiver* pDstReceiver, bool isEffectEnabled, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @brief   サブミックスとして初期化します。
    //! @param[in] param      サブミックスの初期化パラメーターです。
    //! @param[in] buffer           初期化に使用するバッファです。
    //! @param[in] bufferSize       初期化に使用するバッファのサイズです。
    //! @return 初期化に成功した場合は true、失敗した場合は false を返します。
    //! @pre
    //! - SoundSystem が初期化済み
    //! - SubMix が未初期化である
    //! - param.GetSrcBusCount() <= (SoundSystem の初期化時に設定した busCountMax)
    //! - param.GetSrcChannelCount() * param.GetSrcBusCount() <= nn::audio::MixBufferCountMax
    //! - (param.GetDstReceiver() == nullptr の場合)
    //!     - param.GetDstBusCount() <= (SoundSystem の初期化時に設定した busCountMax)
    //!     - param.GetDstChannelCount() * param.GetDstBusCount() <= nn::audio::MixBufferCountMax
    //! - (param.GetDstReceiver() != nullptr の場合)
    //!     - param.GetDstReceiver()->GetBusCount() <= (SoundSystem の初期化時に設定した busCountMax)
    //!     - param.GetDstReceiver()->GetChannelCount() *  param.GetDstReceiver()->GetBusCount() <= nn::audio::MixBufferCountMax
    //! - buffer != nullptr
    //! - buffer は nn::atk::SubMix::SubMixAlignment にアラインされている
    //! - bufferSize が GetRequiredMemorySize() で取得された値よりも大きい
    //! @post SubMix が初期化されている
    bool Initialize(const SubMixParam& param, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @brief   終了処理を行います。
    //! @details 終了処理では、このサブミックスが内部で確保したオーディオレンダラのサブミックスインスタンスとミックスバッファを解放します。
    //!          解放されたオーディオレンダラのサブミックスインスタンスとミックスバッファは、他のサブミックスで再利用することができます。
    //! @pre IsFinalizable() が true を返す
    //! @post SubMix が未初期化である
    //! @see IsFinalizable()
    void Finalize() NN_NOEXCEPT;
    //! @internal
    //! @deprecated この関数は廃止予定です。代わりに @ref IsFinalizable() を使用してください。
    //! @brief サブミックスを削除可能かどうかを取得します。
    //! @return 削除可能であれば true を返します。
    NN_DEPRECATED bool IsRemovable() const NN_NOEXCEPT;
    //! @brief サブミックスを終了可能かどうかを取得します。
    //! @return 終了可能であれば true を返します。
    bool IsFinalizable() const NN_NOEXCEPT;

    //! @internal
    //! 更新します。
    void Update() NN_NOEXCEPT;
    //! @internal
    //! bus に関連する MixVolume を更新します。
    void UpdateBusMixVolume(int bus) NN_NOEXCEPT;
    //! @internal
    //! channel に関連する MixVolume を更新します。
    void UpdateChannelMixVolume(int channel) NN_NOEXCEPT;
    //! @internal
    //! MixVolume を更新します。
    void UpdateMixVolume(int srcBus, int srcChannel, int dstBus, int dstChannel) NN_NOEXCEPT;

    //! @brief 出力先を設定します。
    //! @details 設定は遅延評価されるため、この関数の呼び出し後はしばらく IsFinalizale() が必ず false を返すようになります。
    //! @param[in] pReceiver 設定する出力先です。
    //! @pre
    //! - SubMix が初期化されている
    //! - pReceiver != nullptr
    void SetDestination(OutputReceiver* pReceiver) NN_NOEXCEPT;
    //! @internal
    //! 出力先を適用します。
    void ApplyDestination() NN_NOEXCEPT;

    //! @brief                センド量を取得します。
    //! @param[in] srcBus     センド量を取得する出力元のバスです
    //! @param[in] dstBus     センド量を取得する出力先のバスです
    //! @return               センド量です。
    //! @pre
    //! - SubMix は初期化されている
    //! - 0 <= srcBus < SubMix 初期化時の srcBusCount
    //! - SetDestination() により出力先が設定されている
    //! - 0 <= dstBus < SetDestination() で指定した出力先のバス数
    float GetSend(int srcBus, int dstBus) const NN_NOEXCEPT;
    //! @brief            バスからバスへの、センド量を設定します。
    //! @details          初期値は、 srcBus == dstBus の時 1.0f, srcBus != dstBus の時 0.0f となります。
    //!                   設定は遅延評価されるため、この関数の呼び出し後はしばらく IsFinalizable() が必ず false を返すようになります。
    //! @param[in] srcBus センド量を設定する出力元のバスです。
    //! @param[in] dstBus センド量を設定する出力先のバスです。
    //! @param[in] send センド量です。
    //! @pre
    //! - SubMix は初期化されている
    //! - 0 <= srcBus < SubMix 初期化時の srcBusCount
    //! - SetDestination() により出力先が設定されている
    //! - 0 <= dstBus < SetDestination() で指定した出力先のバス数
    void SetSend(int srcBus, int dstBus, float send) NN_NOEXCEPT;

    //@ internal
    void SetSendImpl(int srcBus, int srcChannel, int dstBus, int dstChannel, float volume) NN_NOEXCEPT;
    //@ internal
    float GetSendImpl(int srcBus, int srcChannel, int dstBus, int dstChannel) const NN_NOEXCEPT;

    //! @brief   バスのボリュームを設定します。音量の初期値は 1.0f です。
    //! @details 1.0 に設定した場合には等倍で出力されますが、
    //!          最終的な値はこの値と Send、ChannelVolume の値を掛け合わせた値が使用されます。
    //!          設定できるボリュームの値に制限はありませんが、上記で示す最終的な値は、
    //!          nn::audio::SubMixType::GetVolumeMin() から nn::audio::SubMixType::GetVolumeMax() の間の値にクランプされた上で使用されます。
    //! @param[in] bus ボリュームを設定するバスです。
    //! @param[in] volume ボリュームです。
    //! @param[in] fadeFrame 音量変化にかけるフレーム数です。
    //! @pre
    //! - SubMix が初期化されている
    //! - 0 <= bus < SubMix 初期化時の srcBusCount
    //! - fadeFrame >= 0
    void SetBusVolume(int bus, float volume, int fadeFrame) NN_NOEXCEPT;
    //! @brief バスのボリュームを取得します。
    //! @param[in] bus ボリュームを取得するバスです。
    //! @pre
    //! - SubMix が初期化されている
    //! - 0 <= bus < SubMix 初期化時の srcBusCount
    //! @return バスのボリュームです。
    float GetBusVolume(int bus) const NN_NOEXCEPT;
    //! @brief バスをミュートするかどうかを設定します。
    //! @param[in] bus ミュートするバスです。
    //! @param[in] isMute ミュート状態です。true ならミュートを行い、false ならミュートを解除します。
    //! @pre
    //! - SubMix が初期化されている
    //! - 0 <= bus < SubMix 初期化時の srcBusCount
    void SetBusMute(int bus, bool isMute) NN_NOEXCEPT;
    //! @brief バスをミュートしているかどうかを取得します。
    //! @param[in] bus ミュートしているかどうかを取得するバスです。
    //! @return ミュート状態であれば true、ミュート状態でなければ false が返ります。
    //! @pre
    //! - SubMix が初期化されている
    //! - 0 <= bus < SubMix 初期化時の srcBusCount
    bool IsBusMuted(int bus) const NN_NOEXCEPT;

    //! @brief   チャンネルのボリュームを設定します。音量の初期値は 1.0f です。
    //! @details 1.0 に設定した場合には等倍で出力されますが、最終的な割合はこの値と Send、BusVolume の値を掛け合わせた値が使用されます。
    //!          この API で設定する値は、SubMix に存在するすべてのバスの同じインデックスのチャンネルすべてに影響を与える点にご注意ください。
    //!
    //!          設定できるボリュームの値に制限はありませんが、上記で示す最終的な値は、
    //!          nn::audio::SubMixType::GetVolumeMin() から nn::audio::SubMixType::GetVolumeMax() の間の値にクランプされた上で使用されます。
    //! @param[in] channel ボリュームを設定するチャンネルです。
    //! @param[in] volume ボリュームです。
    //! @param[in] fadeFrame 音量変化にかけるフレーム数です。
    //! @pre
    //! - SubMix が初期化されている
    //! - 0 <= channel < SubMix 初期化時の srcChannelCount
    //! - fadeFrame >= 0
    void SetChannelVolume(int channel, float volume, int fadeFrame) NN_NOEXCEPT;
    //! @brief チャンネルのボリュームを取得します。
    //! @param[in] channel ボリュームを取得するチャンネルです。
    //! @pre
    //! - SubMix が初期化されている
    //! - 0 <= channel < SubMix 初期化時の srcChannelCount
    float GetChannelVolume(int channel) const NN_NOEXCEPT;
    //! @brief チャンネルをミュートするかどうかを設定します。
    //! @param[in] channel ミュートを設定するチャンネルです。
    //! @param[in] isMute ミュート状態です。true ならミュートを行い、false ならミュートを解除します。
    //! @pre
    //! - SubMix が初期化されている
    //! - 0 <= channel < SubMix 初期化時の srcChannelMax
    void SetChannelMute(int channel, bool isMute) NN_NOEXCEPT;
    //! @brief チャンネルをミュートしているかどうかを取得します。
    //! @param[in] channel ミュートしているかどうかを取得するチャンネルです。
    //! @return ミュート状態であれば true、ミュート状態でなければ false が返ります。
    //! @pre
    //! - SubMix が初期化されている
    //! - 0 <= channel < SubMix 初期化時の srcChannelCount
    bool IsChannelMuted(int channel) const NN_NOEXCEPT;

    //! @brief   サブミックス全体のボリュームを設定します。音量の初期値は 1.0f です。
    //! @details この値は nn::audio::SubMixType::GetVolumeMin() から nn::audio::SubMixType::GetVolumeMax() の間の値にクランプされた上で使用されます。
    //! @param[in] volume ボリュームです。
    //! @param[in] fadeFrame 音量変化にかけるフレーム数です。
    //! @pre
    //! - SubMix が初期化されている
    //! - 0 <= fadeFrame
    void SetSubMixVolume(float volume, int fadeFrame) NN_NOEXCEPT;
    //! @brief サブミックス全体のボリュームを取得します。
    //! @return ボリュームです。
    //! @pre SubMix が初期化されている
    float GetSubMixVolume() const NN_NOEXCEPT;
    //! @brief サブミックス全体をミュートするかどうかを設定します。
    //! @param[in] isMute ミュート状態です。true ならミュートを行い、false ならミュートを解除します。
    //! @pre SubMix が初期化されている
    void SetSubMixMute(bool isMute) NN_NOEXCEPT;
    //! @brief サブミックス全体をミュートしているかどうかを取得します。
    //! @return ミュート状態であれば true、ミュート状態でなければ false が返ります。
    //! @pre SubMix が初期化されている
    bool IsSubMixMuted() const NN_NOEXCEPT;

    //! @briefprivate
    //! @brief   エフェクトの未使用チャンネルをミュートするかどうかのフラグを設定します。
    //! @details デフォルトは false が設定されています。
    //!          設定前に追加されたエフェクトには設定が適用されないため、フラグの設定はエフェクトの追加前に行うようにしてください。
    //! @param[in] isUnusedEffectChannelMuted 未使用チャンネルをミュートするかどうかです。true ならミュートを行い、false ならミュートを解除します。
    void SetMuteUnusedEffectChannel(bool isUnusedEffectChannelMuted) NN_NOEXCEPT;
    //! @briefprivate
    //! @brief エフェクトの未使用チャンネルをミュートするかどうかのフラグを取得します。
    //! @return ミュート状態であれば true、ミュート状態でなければ false が返ります。
    bool IsUnusedEffectChannelMuted() const NN_NOEXCEPT;

    //! @brief 指定したエフェクトをバスに追加します。
    //! @param[in] pEffect    エフェクトです。
    //! @param[in] bus        エフェクトを追加するバスです。
    //! @param[in] buffer     エフェクトで利用するバッファです。
    //! @param[in] bufferSize buffer に指定したバッファのサイズです。
    //! @return エフェクトの追加に成功したら true を、失敗したら false を返します。
    //! @pre
    //! - pEffect != nullptr
    //! - buffer != nullptr
    //! - buffer が nn::audio::BufferAlignSize にアラインしている
    bool AppendEffect(EffectBase* pEffect, int bus, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @brief 指定したエフェクトをバスに追加します。
    //! @param[in] pEffect    ユーザエフェクトです。
    //! @param[in] bus        ユーザエフェクトを追加するバスです。
    //! @param[in] buffer     ユーザエフェクトで利用するバッファです。
    //! @param[in] bufferSize buffer に指定したバッファのサイズです。
    //! @return エフェクトの追加に成功したら true を、失敗したら false を返します。
    //! @pre
    //! - pEffect != nullptr
    //! - buffer != nullptr
    //! - buffer が nn::audio::BufferAlignSize にアラインしている
    bool AppendEffect(EffectAux* pEffect, int bus, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @brief バスに登録されているエフェクトを削除します。
    //! @param[in] pEffect 削除するエフェクトです。
    //! @param[in] bus エフェクトを削除するバスです。
    //! @return 削除に成功した場合は true を、失敗したら false を返します。
    //! @pre
    //! - pEffect != nullptr
    bool RemoveEffect(EffectBase* pEffect, int bus) NN_NOEXCEPT;
    //! @brief バスに登録されているユーザエフェクトを削除します。
    //! @param[in] pEffect 削除するユーザエフェクトです。
    //! @param[in] bus ユーザエフェクトを削除するバスです。
    //! @return 削除に成功した場合は true を、失敗したら false を返します。
    //! @pre
    //! - pEffect != nullptr
    bool RemoveEffect(EffectAux* pEffect, int bus) NN_NOEXCEPT;
    //! @brief バスに登録されているエフェクトを全てバスから削除します。
    //! @param[in] bus エフェクトを削除するバスです。
    //! @return 削除に成功した場合は true を、失敗したら false を返します。
    bool ClearEffect(int bus) NN_NOEXCEPT;
    //! @internal
    bool IsEffectEnabled() const NN_NOEXCEPT;
    //! @internal
    virtual void AppendEffectImpl(EffectBase* pEffect, int bus, void* buffer, size_t bufferSize) NN_NOEXCEPT NN_OVERRIDE;
    //! @internal
    virtual void AppendEffectImpl(EffectAux* pEffect, int bus, void* buffer, size_t bufferSize) NN_NOEXCEPT NN_OVERRIDE;

    //! @internal
    virtual ReceiverType GetReceiverType() const NN_NOEXCEPT NN_OVERRIDE;
    //! @brief チャンネル数を取得します。
    //! @pre SubMix が初期化されている
    virtual int GetChannelCount() const NN_NOEXCEPT NN_OVERRIDE;
    //! @brief バス数を取得します。
    //! @pre SubMix が初期化されている
    virtual int GetBusCount() const NN_NOEXCEPT NN_OVERRIDE;
    //! @internal
    inline nn::audio::SubMixType* GetAudioSubMixInstance() NN_NOEXCEPT
    {
        NN_SDK_ASSERT( m_IsInitialized );
        return &m_SubMix;
    }
    //! @internal
    inline const nn::audio::SubMixType* GetAudioSubMixInstance() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT( m_IsInitialized );
        return &m_SubMix;
    }
    //! @internal
    virtual void AddReferenceCount(int value) NN_NOEXCEPT NN_OVERRIDE;

    //! @internal
    util::IntrusiveListNode m_Link;

    //! @internal
    bool IsSoundSendClampEnabled(int bus) const NN_NOEXCEPT NN_OVERRIDE;

private:
    static size_t GetRequiredMemorySizeImpl(const SubMixParam& param, bool isInternalCall) NN_NOEXCEPT;
    bool InitializeImpl(const SubMixParam& param, void* buffer, size_t bufferSize, bool isInternalCall) NN_NOEXCEPT;

    //  未使用のチャンネルをミュートします。
    void MuteUnusedEffectChannel(ChannelIndex* effectChannelIndex, int effectChannelCount, int bus) NN_NOEXCEPT;
    //  srcIndex から dstIndex の MixBuffer へ接続しているか
    inline bool IsMixBufferLinked(int srcBus, int srcChannel, int dstBus, int dstChannel) const NN_NOEXCEPT
    {
        return m_ppSendVolume[srcBus * m_ChannelCount + srcChannel][dstBus * m_ReceiverChannelCountMax + dstChannel] > 0.0f;
    }

private:
    nn::audio::SubMixType m_SubMix;

    // Voice や SubMix の出力先に設定されたり、DriverCommand の処理対象だったりするなど、
    // このクラスのインスタンスが参照されているカウンタです。
    std::atomic<uint32_t> m_ReferenceCount;

    OutputReceiver* m_pReceiver;

    VolumeData* m_pBusVolume;
    VolumeData* m_pChannelVolume;
    VolumeData m_SubMixVolume;

    bool* m_pIsSoundSendClampEnabledArray;

    float** m_ppSendVolume;

    int m_ChannelCount;
    int m_BusCount;
    int m_ReceiverChannelCountMax;

    detail::fnd::CriticalSection m_VolumeLock;
    detail::fnd::CriticalSection m_DestinationLock;

    bool m_IsInitialized;
    bool m_IsUnusedEffectChannelMuted;
    bool m_IsAppliedOutputReceiver;
};

} // namesapace nn::atk
} // namesapace nn
