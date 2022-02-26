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

#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_Config.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn {
namespace atk {

class OutputMixer;

//---------------------------------------------------------------------------
//! @brief  ユーザーエフェクトを扱うための抽象クラスです。
//!
//!         EffectAux は nn::audio::AuxType に関連する API をラッピングしたクラスです。
//!         EffectAux は SoundSystem::AppendEffect で追加されたのち、
//!         SoundSystem::ClearEffect か SoundSystem::RemoveEffect によって
//!         エフェクトが削除されるまでクラスを保持しておく必要があります。
//!
//!         このクラスを継承し、@ref UpdateSamples で引数として渡される波形に対して、
//!         信号処理を行うことで任意のエフェクトを作成することができます。
//!
//!         この型は std::atomic 型のメンバを持っているため
//!         メモリ上に確保する場合にはアライメントを満たす必要があります。
//!         @ref nn::atk::EffectAux::EffectAuxAlignment バイト境界に配置するようにしてください。
//!
//! @see UpdateSamples
//! @see UpdateSamplesArg
//---------------------------------------------------------------------------
class EffectAux
{
public:
    static const int ChannelCountMax = 6; //!< EffectAux に指定できる最大のチャンネル数です。
    static const int EffectAuxAlignment = 8; //!< ユーザエフェクトのアラインメントです。

    //---------------------------------------------------------------------------
    //! @brief サンプル書き換え関数で渡される引数です。
    //---------------------------------------------------------------------------
    struct UpdateSamplesArg
    {
        //! @brief １ オーディオフレームあたりのサンプル数です
        int sampleCountPerAudioFrame;
        //! @brief サンプルレートです。
        int sampleRate;
        //! @brief １ 回の更新で渡されるサンプル数が最大でどれだけのオーディオフレーム数分であるかの値です。
        int audioFrameCount;
        //! @brief チャンネル数です。
        int channelCount;
        //! @brief 実際に読み込まれたデータのサンプル数です。書き換え可能なサンプルの数と同値になります。
        int readSampleCount;
        //! @brief サウンドシステムの出力モードです。
        OutputMode outputMode;
    };

private:
    struct BufferSet
    {
        void* sendBuffer;
        void* returnBuffer;
        void* readBuffer;
    };

public:
    //! @name コンストラクタ/デストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //!
    //!         デフォルトのチャンネル数は 4 で、
    //!         nn::atk::ChannelIndex の小さな値から順にエフェクトが適用されます。
    //---------------------------------------------------------------------------
    EffectAux() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    デストラクタです。
    //---------------------------------------------------------------------------
    virtual ~EffectAux() NN_NOEXCEPT;

    //@}

    //! @name 初期化・終了処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief エフェクトの初期化処理を行います。
    //!
    //!        この関数は、@ref SoundSystem クラスから、
    //!        エフェクトの開始処理が必要となるタイミングで呼び出されます。
    //!
    //!        ユーザーが呼び出す必要はありません。
    //!
    //! @return   初期化に成功すると true を、失敗すると false を返します。
    //---------------------------------------------------------------------------
    virtual bool Initialize() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    エフェクトの終了処理を行います。
    //!
    //!           この関数は、@ref SoundSystem クラスから、
    //!           エフェクトの終了処理が必要となるタイミングで呼び出されます。
    //!
    //!           ユーザーが呼び出す必要はありません。
    //---------------------------------------------------------------------------
    virtual void Finalize() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  出力モードが変更されたときに呼ばれる仮想関数です。
    //---------------------------------------------------------------------------
    virtual void OnChangeOutputMode() NN_NOEXCEPT;

    //@}

    //! @name 設定・取得
    //@{

    //---------------------------------------------------------------------------
    //! @brief チャンネル数を設定します。
    //!
    //!        この関数でチャンネル数を指定した場合、
    //!        nn::atk::ChannelIndex の小さな値から順に、設定したチャンネル数分のエフェクトが適用されます。
    //!        エフェクトを適用するチャンネルも併せて設定したい場合は、この関数ではなく @ref SetChannelIndex を利用してください。
    //!
    //! @param channelCount 設定するチャンネル数です。
    //!
    //! @return 設定に成功すれば true を返します。
    //!
    //! @pre
    //!   - 0 < channelCount <= ChannelCountMax
    //!
    //! @see     SetChannelIndex
    //!
    //! @details  nn::atk::SoundSystem::AppendEffect 前はチャンネルの設定に成功しますが、
    //!           nn::atk::SoundSystem::AppendEffect 後は設定に失敗し、
    //!           nn::atk::SoundSystem::RemoveEffect によってエフェクトが削除されるまで失敗します。
    //!           @ref SetChannelIndex と併用した場合は、後から呼んだ関数の設定が適用されます。
    //---------------------------------------------------------------------------
    bool SetChannelCount(int channelCount) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief チャンネル数を取得します。
    //!
    //! @return チャンネル数を返します。
    //---------------------------------------------------------------------------
    int GetChannelCount() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    チャンネルの設定を行います。
    //!
    //!           エフェクトを適用するチャンネル、およびエフェクトのチャンネル数を指定することができます。
    //!           この関数を利用する場合は、 @ref SetChannelCount によるチャンネル数の設定は必要ありません。
    //!
    //! @param[in] pChannel エフェクトを適用するチャンネルです。 ChannelIndex 型の配列で指定します。
    //! @param[in] channelCount エフェクトを適用するチャンネル数です。 pChannel に与えた配列の要素数を指定してください。
    //!
    //! @return   チャンネルの設定に成功すれば true を返します
    //!
    //! @pre
    //!   - pChannel != nullptr
    //!   - (pChannel で指定する値) < nn::atk::ChannelIndex_Count
    //!   - pChannel で指定する値に 2 つ以上同じチャンネル番号が存在しない
    //!   - 0 < channelCount <= ChannelCountMax
    //!
    //! @see     SetChannelCount
    //!
    //! @details  nn::atk::SoundSystem::AppendEffect 前はチャンネルの設定に成功しますが、
    //!           nn::atk::SoundSystem::AppendEffect 後は設定に失敗し、
    //!           nn::atk::SoundSystem::RemoveEffect によってエフェクトが削除されるまで失敗します。
    //!           @ref SetChannelCount と併用した場合は、後から呼んだ関数の設定が適用されます。
    //---------------------------------------------------------------------------
    bool SetChannelIndex(const ChannelIndex* pChannel, int channelCount) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    現在のチャンネル設定を取得します。
    //!
    //!           この関数でチャンネル数を取得する場合には、
    //!           事前に @ref GetChannelCount 関数によりチャンネル数を取得し、
    //!           その大きさ以上の nn::atk::ChannelIndex の配列を用意してください。
    //!
    //! @param[out] pChannel エフェクトが適用されているチャンネルです
    //! @param[in]  channelCount pChannel の要素数です
    //!
    //! @pre
    //!   - pChannel != nullptr
    //!   - channelCount >= GetChannelCount
    //---------------------------------------------------------------------------
    void GetChannelIndex(ChannelIndex* pChannel, int channelCount) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief 一度に扱うオーディオフレーム数を設定します。
    //!
    //! @param audioFrameCount 一度に扱うオーディオフレーム数です。
    //!
    //! @return 設定に成功すれば true を返します。
    //---------------------------------------------------------------------------
    bool SetAudioFrameCount(int audioFrameCount) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief 一度に扱うオーディオフレーム数を取得します。
    //!
    //! @return 一度に扱うオーディオフレーム数を返します。
    //---------------------------------------------------------------------------
    int GetAudioFrameCount() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    エフェクトが削除可能かどうかを取得します。
    //!
    //!           @ref SoundSystem::ClearEffect または @ref SoundSystem::RemoveEffect を呼び出す前に、
    //!           まず @ref SetEnabled でエフェクトを無効に設定した後、本関数で削除可能であることを確認してください。
    //!           @ref SetEnabled で無効にした後すぐに削除可能になるわけではない点に注意してください。
    //!
    //! @return   エフェクトが削除可能であれば true を返します。
    //!           エフェクトがサウンドシステムに追加されていない場合は false を返します。
    //!
    //! @see      SetEnabled
    //---------------------------------------------------------------------------
    bool IsRemovable() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    エフェクトが削除可能かどうかを取得します。
    //!
    //! @deprecated この関数は廃止予定です。代わりに @ref IsRemovable を使用してください。
    //---------------------------------------------------------------------------
    NN_DEPRECATED bool IsClearable() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    エフェクトが有効であるかを調べます。
    //!
    //! @return   エフェクトが有効であれば true を返します。
    //!
    //! @see  SetEnabled
    //---------------------------------------------------------------------------
    bool IsEnabled() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    エフェクトの有効・無効を設定します。
    //!           初期値は false です。
    //!
    //!           @ref SoundSystem::ClearEffect または @ref SoundSystem::RemoveEffect を呼び出す前には、
    //!           まず本関数で無効に設定した後に、@ref IsRemovable で削除可能であることを確認する必要があります。
    //!           本関数で無効にした後すぐに削除可能になるわけではない点に注意してください。
    //!
    //! @param[in] isEnabled エフェクトの有効・無効です。
    //!
    //! @see  IsEnabled
    //---------------------------------------------------------------------------
    void SetEnabled( bool isEnabled ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    チャンネル設定の最大値を取得します。
    //!
    //! @return   チャンネル設定の最大値です。
    //---------------------------------------------------------------------------
    int GetChannelSettingCountMax() const NN_NOEXCEPT
    {
        return ChannelCountMax;
    }

    //@}

    //! @internal
    //! @param[in] parameter :private
    //! @return :private
    size_t GetRequiredMemSize(const nn::audio::AudioRendererParameter& parameter) const NN_NOEXCEPT;

    //! @internal
    //! @param[in] pConfig :private
    //! @param[in] parameter :private
    //! @param[in] pOutputMixer :private
    //! @param[in] bus :private
    //! @return :private
    bool AddEffect(nn::audio::AudioRendererConfig* pConfig, const nn::audio::AudioRendererParameter& parameter, nn::atk::OutputMixer* pOutputMixer) NN_NOEXCEPT;

    //! @internal
    //! @param[in] input :private
    //! @param[in] output :private
    //! @param[in] inputCount :private
    //! @param[in] outputCount :private
    void SetEffectInputOutput(const int8_t* input, const int8_t* output, int inputCount, int outputCount) NN_NOEXCEPT;

    //! @internal
    //! @param[in] pConfig :private
    //! @param[in] pOutputMixer :private
    void RemoveEffect(nn::audio::AudioRendererConfig* pConfig, nn::atk::OutputMixer* pOutputMixer) NN_NOEXCEPT;

    //! @internal
    //! @param[in] effectBuffer :private
    //! @param[in] effectBufferSize :private
    void SetEffectBuffer( void* effectBuffer, size_t effectBufferSize ) NN_NOEXCEPT;

    //! @internal
    void Update() NN_NOEXCEPT;

protected:
    //---------------------------------------------------------------------------
    //! @brief    サンプルブロックに対して更新処理を行います
    //!
    //!           この関数内部でサンプルブロックに対して処理を施すことができます。
    //!           pSamples には、今から再生しようとしている波形の情報が入った状態で渡されるため、
    //!           何もしなければそのまま元の波形が再生されます。
    //!
    //!           pSamples が実際にどのような構造になっているかは、@ref nn::audio::ReadAuxSendBuffer
    //!           の pOutData の説明を参照してください。
    //!
    //! @param[in,out] pSamples 処理対象のサンプルブロックです。
    //! @param[in]     arg      サンプル更新のためにヒントとなる情報です。
    //!
    //! @see  UpdateSamplesArg
    //---------------------------------------------------------------------------
    virtual void UpdateSamples(int32_t* pSamples, const UpdateSamplesArg& arg) NN_NOEXCEPT = 0;

private:
    void SplitEffectBuffer(BufferSet* pOutBufferSet, void* effectBuffer, size_t sendReturnBufferSize) NN_NOEXCEPT;
    void ResetChannelIndex() NN_NOEXCEPT;

public:
    //! @internal
    util::IntrusiveListNode m_AuxLinkNode;

private:
    nn::audio::AuxType      m_AuxType;
    std::atomic<uint64_t>   m_AudioRendererUpdateCountWhenAddedAux;

    int                     m_AudioFrameCount;
    int                     m_ChannelCount;
    bool                    m_IsActive;
    bool                    m_IsEnabled;

    void*                   m_EffectBuffer;
    size_t                  m_EffectBufferSize;
    int32_t*                m_AuxReadBuffer;
    ChannelIndex m_ChannelSetting[ChannelCountMax];
};

} // namespace nn::atk
} // namespace nn
