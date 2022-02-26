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
//! @brief    エフェクトの抽象クラスです。
//!
//!           エフェクトの使用方法については、@ref SoundSystem::AppendEffect
//!           などを参照してください。
//!
//!           ユーザー定義の信号処理を行うエフェクトを実装したい場合は @ref nn::atk::EffectAux を参照してください。
//!
//! @see SoundSystem クラス
//!
//---------------------------------------------------------------------------
class EffectBase
{
public:
    static const int EffectAlignment = 8; //!< エフェクトのアラインメントです。

    //---------------------------------------------------------------------------
    //! @brief    エフェクトのチャンネル数の定義です。
    //---------------------------------------------------------------------------
    enum ChannelMode
    {
        ChannelMode_1Ch, //!< 1 チャンネルに対しエフェクト処理を行います。
        ChannelMode_2Ch, //!< 2 チャンネルに対しエフェクト処理を行います。
        ChannelMode_4Ch, //!< 4 チャンネルに対しエフェクト処理を行います。
        ChannelMode_6Ch  //!< 6 チャンネルに対しエフェクト処理を行います。
    };

    //---------------------------------------------------------------------------
    //! @brief    エフェクトのサンプルレートの定義です。
    //---------------------------------------------------------------------------
    enum SampleRate
    {
        SampleRate_32000,   //!< 32 kHz のサンプルレートです。
        SampleRate_48000    //!< 48 kHz のサンプルレートです。
    };


    //! @name コンストラクタ／デストラクタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //---------------------------------------------------------------------------
    EffectBase() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    デストラクタです。
    //---------------------------------------------------------------------------
    virtual ~EffectBase() NN_NOEXCEPT;
    //@}

    //! @name メモリ割り当て
    //@{

    //---------------------------------------------------------------------------
    //! @brief    エフェクトが必要とするメモリのサイズを取得します。
    //!
    //!           現在のパラメータ設定でエフェクト処理を行うために必要とする
    //!           メモリのサイズを取得します。
    //!
    //! @return   エフェクトが必要とするメモリのサイズを返します。
    //!
    //! @see  SetParam
    //---------------------------------------------------------------------------
    virtual size_t GetRequiredMemSize() const NN_NOEXCEPT = 0;

    //@}

    //! @internal
    //! @param[in] pConfig :private
    //! @param[in] pOutputMixer :private
    //! @return :private
    virtual bool AddEffect(nn::audio::AudioRendererConfig* pConfig, nn::atk::OutputMixer* pOutputMixer) NN_NOEXCEPT = 0;

    //! @internal
    //! @param[in] input :private
    //! @param[in] output :private
    //! @param[in] inputCount :private
    //! @param[in] outputCount :private
    virtual void SetEffectInputOutput(const int8_t* input, const int8_t* output, int inputCount, int outputCount) NN_NOEXCEPT = 0;

    //! @internal
    //! @param[in] pConfig :private
    //! @param[in] pOutputMixer :private
    virtual void RemoveEffect(nn::audio::AudioRendererConfig* pConfig, nn::atk::OutputMixer* pOutputMixer) NN_NOEXCEPT = 0;

    //! @name エフェクト
    //@{

    //---------------------------------------------------------------------------
    //! @brief    エフェクトが削除可能かどうかを取得します。
    //!
    //!           @ref nn::atk::SoundSystem::ClearEffect または @ref nn::atk::SoundSystem::RemoveEffect を呼び出す前に、
    //!           本関数で削除可能であることを確認してください。
    //!
    //! @return   エフェクトが削除可能であれば true を返します。
    //---------------------------------------------------------------------------
    virtual bool IsRemovable() const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief    エフェクト処理を行います。
    //!
    //!           この関数はエフェクトに送られてくる
    //!           AUX　バスのバッファの更新が必要なタイミングで呼び出されます。
    //!
    //! @param[in]      channelCount バッファのチャンネル数です。
    //! @param[in,out]  buffer      エフェクト処理を行う波形データのバッファへのポインタ配列です。
    //! @param[in]      bufferSize  1 チャンネルあたりのバッファサイズです。
    //! @param[in]      format      data のサンプルフォーマットです。
    //! @param[in]      sampleRate  data のサンプルレートです。
    //! @param[in]      mode        出力モードです。
    //!
    //! @see SampleFormat
    //! @see OutputMode
    //---------------------------------------------------------------------------
    virtual void UpdateBuffer(
        int channelCount,
        void* buffer[],
        size_t bufferSize,
        SampleFormat format,
        int sampleRate,
        OutputMode mode
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    サンプルレートを取得します。
    //!
    //! @return   現在のサンプルレートです。
    //!
    //! @see  SetSampleRate
    //---------------------------------------------------------------------------
    SampleRate GetSampleRate() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    サンプルレートを設定します。
    //!
    //! @param[in] rate サンプルレートを指定する列挙型です。
    //!
    //! @return   エフェクト処理を開始する前は true を返しますが、
    //!           エフェクト処理中は false を返し、サンプルレートの設定に失敗します。
    //!
    //! @see  GetSampleRate
    //---------------------------------------------------------------------------
    bool SetSampleRate( SampleRate rate ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    現在のチャンネル設定を取得します。
    //!
    //!           EffectBase では空実装になっているため、使用できません。
    //!
    //! @param[out] pChannel エフェクトが適用されているチャンネルです
    //! @param[in]  channelCount pChannel の要素数です
    //!
    //! @pre
    //!   - pChannel != nullptr
    //---------------------------------------------------------------------------
    virtual void GetChannelIndex(ChannelIndex* pChannel, int channelCount) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    チャンネル設定の最大値を取得します。
    //!
    //! @return   チャンネル設定の最大値です。
    //---------------------------------------------------------------------------
    virtual int GetChannelSettingCountMax() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //!
    //! @brief  出力モードが変更されたときに呼ばれる仮想関数です。
    //---------------------------------------------------------------------------
    virtual void OnChangeOutputMode() NN_NOEXCEPT;
    //@}

    virtual void SetEffectBuffer(void* effectBuffer, size_t effectBufferSize) NN_NOEXCEPT;


    //! @internal
    util::IntrusiveListNode m_Link;

protected:
    static int ConvertChannelModeToInt(ChannelMode mode) NN_NOEXCEPT;

protected:
    bool       m_IsActive;
    SampleRate m_SampleRate;
    void* m_EffectBuffer;
    size_t m_EffectBufferSize;

    static const int ChannelModeCountMax = 6;

private:
    NN_DISALLOW_COPY( EffectBase );
};

} // namespace nn::atk
} // namespace nn

