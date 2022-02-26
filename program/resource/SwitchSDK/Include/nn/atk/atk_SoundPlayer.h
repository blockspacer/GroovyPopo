/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * :include nn/atk/atk_SoundPlayer.h
 *
 * @file atk_SoundPlayer.h
 */

#pragma once

#include <nn/util/util_IntrusiveList.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_BasicSound.h>
#include <nn/atk/atk_PlayerHeap.h>
#include <nn/atk/atk_SoundPlayer.h>
#include <nn/atk/atk_SoundHandle.h>
#include <nn/atk/atk_OutputAdditionalParam.h>

namespace nn {
namespace atk {


//---------------------------------------------------------------------------
//! @brief    サウンドを再生するためのプレイヤークラスです。
//!
//!           SoundPlayer クラスのインスタンスは
//!           @ref SoundArchivePlayer::GetSoundPlayer を呼び出して取得してください。
//!
//!           一つの SoundPlayer で複数のサウンドを再生することができます。
//!           同時に再生できるサウンド数を設定することができ、
//!           そのサウンド数を超えた場合には、
//!           各サウンドに設定されたプレイヤープライオリティを比較して、
//!           もっとも優先度が低いサウンドが停止します。
//!
//!         @n
//!
//!         SoundPlayer では、プレイヤーで再生するサウンドに関して、
//!         下記の影響を与えることができます。
//!
//!         - 音量 ... @ref SetVolume, @ref StopAllSound, @ref PauseAllSound, @ref SetOutputVolume
//!         - LPF ... @ref SetLowPassFilterFrequency
//!         - Biquad フィルタ ... @ref SetBiquadFilter
//!         - 出力先 ... SetDefaultOutputLine
//!
//!         @n
//!
//!         各パラメータは、上記の影響の他に、下記の影響を受けます。
//!         - SoundMaker で指定したデータの影響
//!         - @ref SoundHandle や @ref SoundActor による影響
//!         - 3D サウンド計算 (@ref Sound3DEngine) による影響
//!
//! @see SoundArchivePlayer クラス
//! @see SoundArchivePlayer::GetSoundPlayer
//---------------------------------------------------------------------------
class SoundPlayer
{
public:
    //! @internal
    typedef util::IntrusiveList<
        detail::PlayerHeap,
        util::IntrusiveListMemberNodeTraits<detail::PlayerHeap,&detail::PlayerHeap::m_Link>
    > PlayerHeapList;

    //! @internal
    typedef util::IntrusiveList<
        detail::BasicSound,
        util::IntrusiveListMemberNodeTraits<detail::BasicSound,&detail::BasicSound::m_SoundPlayerPlayLink>
    > SoundList;

    //! @internal
    typedef util::IntrusiveList<
        detail::BasicSound,
        util::IntrusiveListMemberNodeTraits<detail::BasicSound,&detail::BasicSound::m_SoundPlayerPriorityLink>
    > PriorityList;

public:

    //----------------------------------------
    //! @name コンストラクタ/デストラクタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief        コンストラクタです。
    //
    //---------------------------------------------------------------------------
    SoundPlayer() NN_NOEXCEPT;

    //@}
    //---------------------------------------------------------------------------
    //! @internal
    //! @brief        コンストラクタです。
    //
    //---------------------------------------------------------------------------
    explicit SoundPlayer(detail::OutputAdditionalParam* pParam) NN_NOEXCEPT;

    //----------------------------------------
    //! @name コンストラクタ/デストラクタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief        デストラクタです。
    //
    //---------------------------------------------------------------------------
    ~SoundPlayer() NN_NOEXCEPT;
    //@}

    //----------------------------------------
    //! @name 更新
    //@{
    //---------------------------------------------------------------------------
    //! @brief        サウンドプレイヤーの更新処理を行います。
    //!
    //!               @ref SoundArchivePlayer クラスを使用している場合は、
    //!               この関数は @ref SoundArchivePlayer::Update から呼び出されます。
    //!
    //! @see SoundArchivePlayer クラス
    //! @see SoundArchivePlayer::Update
    //!
    //---------------------------------------------------------------------------
    void Update() NN_NOEXCEPT;
    //@}

    //----------------------------------------
    //! @name 再生制御
    //@{
    //---------------------------------------------------------------------------
    //! @brief        すべてのサウンドを停止します。
    //!
    //!               プレイヤーで再生中のすべてのサウンドを停止します。
    //!               各サウンドに対して、
    //!               ハンドルクラスを通して停止させたときと同様の処理を行います。
    //!
    //!               fadeFrames で指定したフレーム数をかけて、
    //!               フェードアウトさせることができます。
    //!               0 を指定した場合は、フェードアウトを行いません。
    //!               ただし、シーケンスサウンドで発音中の音は、
    //!               エンベロープのリリースを発音しすべての減衰が完了した後に
    //!               サウンドが停止します。
    //!
    //!               フェードアウトの音量制御は、フェードインと共有されます。
    //!               フェードアウトにかかるフレーム数は、
    //!               最大音量から音が消えるまでにかかる変化速度を表しますので、
    //!               フェードイン中にフェードアウトを指定した時などは、
    //!               指定したフレーム数よりも短い時間で
    //!               フェードアウトが完了する可能性があります。
    //!
    //! @param[in]    fadeFrames  フェードアウトにかけるフレーム数です。
    //!                           フレーム数は @ref SoundArchivePlayer::Update
    //!                           の呼び出し回数で換算されます。
    //!                           fadeFrames には 0 以上の値を指定してください。
    //!                           負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //---------------------------------------------------------------------------
    void StopAllSound( int fadeFrames ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        すべてのサウンドを一時停止・再開します。
    //!
    //!               プレイヤーで再生中のすべてのサウンドを一時停止または再開します。
    //!               各サウンドに対して、
    //!               ハンドルクラスを通して一時停止・再開させたときと同様の処理を行います。
    //!
    //!               一時停止・再開時のフェードは、再生開始時のフェードイン、
    //!               停止時のフェードアウトとは独立してはたらきます。
    //!
    //! @param[in]    flag        true なら一時停止、false なら再開。
    //! @param[in]    fadeFrames  フェードイン・フェードアウトにかけるフレーム数。
    //!                           フレーム数は @ref SoundArchivePlayer::Update
    //!                           の呼び出し回数で換算されます。
    //!                           負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //---------------------------------------------------------------------------
    void PauseAllSound( bool flag, int fadeFrames ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief        すべてのサウンドを一時停止・再開します。
    //!
    //!               プレイヤーで再生中のすべてのサウンドを一時停止または再開します。
    //!               各サウンドに対して、
    //!               ハンドルクラスを通して一時停止・再開させたときと同様の処理を行います。
    //!
    //!               一時停止・再開時のフェードは、再生開始時のフェードイン、
    //!               停止時のフェードアウトとは独立してはたらきます。
    //!
    //! @param[in]    flag        true なら一時停止、false なら再開。
    //! @param[in]    fadeFrames  フェードイン・フェードアウトにかけるフレーム数。
    //!                           フレーム数は @ref SoundArchivePlayer::Update
    //!                           の呼び出し回数で換算されます。
    //!                           負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //! @param[in]    pauseMode   ポーズの設定です。
    //---------------------------------------------------------------------------
    void PauseAllSound(bool flag, int fadeFrames, PauseMode pauseMode) NN_NOEXCEPT;
    //@}

    //----------------------------------------
    //! @name パラメータ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    プレイヤーの音量を変更します。
    //!
    //!           プレイヤーの音量は、
    //!           プレイヤーで再生中のすべてのサウンドに対して効果があります。
    //!
    //!           この関数で指定する値は、
    //!           サウンドに設定される他のどの音量パラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           音量 volume は、0.0 以上の倍率で指定します。
    //!           すなわち、1.0 を指定すると音量に影響を与えません。
    //!           0.0 を指定すると発音されなくなります。
    //!           デフォルト値は 1.0 です。
    //!
    //!           他の音量パラメータと重ね合わされたあと、
    //!           最終的な音量は nn::audio::VoiceType::GetVolumeMin() ～ nn::audio::VoiceType::GetVolumeMax() の範囲でクランプされます。
    //!           音量の重ね合わせの詳細につきましては @confluencelink{83955829, サウンドパラメータ制御の動作} を参照ください。
    //!
    //! @param[in]    volume    プレイヤーの音量の倍率（ 0.0 ～ ）です。
    //!
    //! @see GetVolume
    //!
    //---------------------------------------------------------------------------
    void SetVolume( float volume ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    プレイヤーの音量を取得します。
    //!
    //! @return   プレイヤーに設定されている現在の音量を返します。
    //!
    //! @see SetVolume
    //!
    //---------------------------------------------------------------------------
    float GetVolume() const NN_NOEXCEPT { return m_Volume; }

    //---------------------------------------------------------------------------
    //! @brief  プレイヤーのローパスフィルタカットオフ値を変更します。
    //!
    //!         プレイヤーのローパスフィルタは、
    //!         プレイヤーで再生中のすべてのサウンドに対して効果があります。
    //!
    //!         この関数で指定する値は、サウンドに設定される
    //!         他のどのローパスフィルタカットオフパラメータとも独立して動作し、
    //!         それらはすべて重ね合わされます。
    //!
    //!         lowPassFilterFrequency は、カットオフの相対変化の値を指定します。
    //!         0.0 を指定するとカットオフの値を変更しません。
    //!         -1.0 を指定すると、フィルタがかかっていない状態から、
    //!         もっともフィルタがかかっている状態（カットオフ周波数が下がる方向）に変更します。
    //!
    //!         SetBiquadFilter で与えられる Biquad フィルタと併用して使用した場合重ねがけされます。
    //!
    //! @param[in] lowPassFilterFrequency    0.0 を基準としたフィルタカットオフの相対変化の値です。
    //!
    //! @see GetLowPassFilterFrequency
    //!
    //---------------------------------------------------------------------------
    void SetLowPassFilterFrequency( float lowPassFilterFrequency ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  プレイヤーに設定されているローパスフィルタのカットオフ周波数を取得します。
    //!
    //! @return プレイヤーに設定されている、
    //!         現在のローパスフィルタのカットオフ周波数を返します。
    //!
    //---------------------------------------------------------------------------
    float GetLowPassFilterFrequency() const NN_NOEXCEPT { return m_LpfFreq; }

    //---------------------------------------------------------------------------
    //! @brief  プレイヤーの Biquad フィルタの設定を変更します。
    //!
    //!         プレイヤーの Biquad フィルタは、プレイヤーで再生中の
    //!         すべてのサウンドに対して効果があります。
    //!
    //!         Biquad フィルタは複数の箇所での設定が重ね合わされず、
    //!         以下の優先度に従って設定されます。
    //!         優先度が高い箇所でパラメータの設定がされた場合、
    //!         それより下位の設定は上書きされます。
    //!
    //!         -# サウンドハンドルでの設定
    //!         -# サウンドプレイヤーでの設定
    //!         -# アンビエントパラメータ構造体での設定
    //!         -# シーケンスデータでの設定
    //!
    //!         フィルタの種類 type は @ref BiquadFilterType の値を使用します。
    //!         プリセットで用意されているフィルタの種類の他、
    //!         ユーザーが登録したフィルタを選択することができます。
    //!
    //!         フィルタのかかり具合を指定する value は、
    //!         0.0f ～ 1.0f の値で指定します。
    //!         値の意味はフィルタの係数の種類によって変化します。
    //!
    //!         SetLowPassFilterFrequency で与えられるローパスフィルタと併用して使用した場合重ねがけされます。
    //!
    //! @param[in] type     フィルタの種類です (BiquadFilterType_Min ～ BiquadFilterType_Max)。
    //! @param[in] value    フィルタのかかり具合です (0.0f ～ 1.0f)。
    //!
    //! @see BiquadFilterType
    //! @see GetBiquadFilterType
    //! @see GetBiquadFilterValue
    //!
    //---------------------------------------------------------------------------
    void SetBiquadFilter( int type, float value ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  プレイヤーに設定されている Biquad フィルタの種類を取得します。
    //!
    //!         返される値については @ref BiquadFilterType を参照してください。
    //!
    //! @return 現在の Biquad フィルタの種類を返します。
    //!
    //! @see BiquadFilterType
    //! @see SetBiquadFilter
    //!
    //---------------------------------------------------------------------------
    int GetBiquadFilterType() const NN_NOEXCEPT { return m_BiquadType; }

    //---------------------------------------------------------------------------
    //! @brief  プレイヤーに設定されている Biquad フィルタの設定値を取得します。
    //!
    //! @return 現在の Biquad フィルタの設定値を返します。
    //!
    //! @see SetBiquadFilter
    //!
    //---------------------------------------------------------------------------
    float GetBiquadFilterValue() const NN_NOEXCEPT { return m_BiquadValue; }

    //---------------------------------------------------------------------------
    //! @brief  プレイヤーで再生するサウンドの出力先を設定します。
    //!
    //!         サウンドを再生する際、サウンドの出力先はこの関数で設定された値になります。
    //!         @ref SoundHandle::SetOutputLine を呼び出して、
    //!         サウンドごとに異なる出力先を設定することも可能です。
    //!
    //!         この関数は、プレイヤーで再生中のサウンドの出力先は変更しません。
    //!         この関数で出力先を設定した後にプレイヤーで再生されるサウンドについて、
    //!         新しい出力先が反映されます。
    //!         プレイヤーで再生中のサウンドの出力先を変更するには、
    //!         それぞれのサウンドに対して @ref SoundHandle::SetOutputLine を呼び出してください。
    //!
    //!         出力先は @ref OutputLine のビットフラグの論理和で指定します。
    //!         指定した値は、それまで設定されていた値を上書きします。
    //!         初期状態では、@ref nn::atk::OutputLine_Main のみが指定されています。
    //!
    //! @param[in] lineFlag     出力先のビットフラグです。
    //!
    //! @see OutputLine
    //! @see GetDefaultOutputLine
    //! @see SoundHandle::SetOutputLine
    //!
    //---------------------------------------------------------------------------
    void SetDefaultOutputLine( uint32_t lineFlag ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  プレイヤーに設定されている、サウンドの出力先を取得します。
    //!
    //! @return 現在設定されているサウンドの出力先を返します。
    //!
    //! @see OutputLine
    //! @see SetDefaultOutputLine
    //!
    //---------------------------------------------------------------------------
    uint32_t GetDefaultOutputLine() const NN_NOEXCEPT { return m_OutputLineFlag; }

    //! @brief  プレイヤーのメインセンド量を変更します。
    //!
    //! @details プレイヤーのセンド量は、プレイヤーで再生中のすべてのサウンドに対して効果があります。
    //!          この関数で指定する値は、
    //!          サウンドに設定される他のどの音量パラメータとも独立して動作し、
    //!          それらはすべて重ね合わされます。
    //!
    //!          メインセンドは、メイン出力に送るサウンドの音量を調節するパラメータです。
    //!          主に、エフェクトのドライ・ウェット成分のバランスを調整するために使用されます。
    //!
    //!          センド量 send は相対変化の値を指定し、最終的なセンド量は、
    //!          各 API や SoundMaker で指定した値の合計値を 0.0 ～ 1.0 の範囲でクランプしたものとなります。
    //!          また、 SoundMaker で設定できる値は、ランタイム側で 0.0 ～ 1.0 の値の範囲として解釈されます。
    //!
    //!          すなわち、 0.0 のセンド量の指定は各 API や SoundMaker で指定したセンド量を変更しない設定を表します。
    //!          1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスに最大のセンド量が送られるようなセンド量の加算を表します。
    //!          -1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスにセンド量が送られないようなセンド量の減算を表します。
    //!
    //!          デフォルト値は 0.0 です。
    //!
    //! @param[in] send 0.0 を基準としたセンド量の相対変化の値です。
    //!
    //! @see GetMainSend
    //! @see SetEffectSend
    void SetMainSend( float send ) NN_NOEXCEPT;

    //! @brief  プレイヤーに設定されているメインセンド量を取得します。
    //! @return 現在のメイン出力へのセンド量を返します。
    //! @see SetMainSend
    float GetMainSend() const NN_NOEXCEPT;

    //! @brief  プレイヤーのエフェクトセンド量を変更します。
    //!
    //! @details プレイヤーのエフェクトへのセンド量は、
    //!          プレイヤーで再生中のすべてのサウンドに対して効果があります。
    //!
    //!          この関数で指定する値は、
    //!          サウンドに設定される他のどの音量パラメータとも独立して動作し、
    //!          それらはすべて重ね合わされます。
    //!
    //!          センド量 send は相対変化の値を指定し、最終的なセンド量は、
    //!          各 API や SoundMaker で指定した値の合計値を 0.0 ～ 1.0 の範囲でクランプしたものとなります。
    //!          また、 SoundMaker で設定できる値は、ランタイム側で 0.0 ～ 1.0 の値の範囲として解釈されます。
    //!
    //!          すなわち、 0.0 のセンド量の指定は各 API や SoundMaker で指定したセンド量を変更しない設定を表します。
    //!          1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスに最大のセンド量が送られるようなセンド量の加算を表します。
    //!          -1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスにセンド量が送られないようなセンド量の減算を表します。
    //!
    //!          デフォルト値は 0.0 です。
    //!
    //! @param[in] bus  センド量を設定する AUX バスです。
    //! @param[in] send 0.0 を基準としたセンド量の相対変化の値です。
    //! @see AuxBus
    //! @see GetEffectSend
    //! @see SetMainSend
    //! @pre 0 <= bus < AuxBus_Count
    void SetEffectSend( AuxBus bus, float send ) NN_NOEXCEPT;

    //! @brief  プレイヤーに設定されているエフェクトセンド量を取得します。
    //! @return 指定したエフェクトバスへのセンド量を返します。
    //! @param[in] bus センド量を設定する AUX バスです。
    //! @see AuxBus
    //! @see SetFxSend
    //! @pre 0 <= bus < AuxBus_Count
    float GetEffectSend( AuxBus bus ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief   プレイヤーのバスごとに設定するセンド量を変更します。
    //!
    //! @details プレイヤーのエフェクトへのセンド量は、
    //!          プレイヤーで再生中のすべてのサウンドに対して効果があります。
    //!
    //!          この関数で指定する値は、他のどのエフェクトセンド量とも独立して動作し、
    //!          それらはすべて重ね合わされます。
    //!
    //!          センド量 send は相対変化の値を指定し、最終的なセンド量は、
    //!          各 API や SoundMaker で指定した値の合計値を 0.0 ～ 1.0 の範囲でクランプしたものとなります。
    //!          また、 SoundMaker で設定できる値は、ランタイム側で 0.0 ～ 1.0 の値の範囲として解釈されます。
    //!
    //!          すなわち、 0.0 のセンド量の指定は各 API や SoundMaker で指定したセンド量を変更しない設定を表します。
    //!          1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスに最大のセンド量が送られるようなセンド量の加算を表します。
    //!          -1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスにセンド量が送られないようなセンド量の減算を表します。
    //!
    //!          SoundMaker で指定する、バスに対するセンド量の設定との対応関係については、
    //!          センドMAIN がバス 0 のセンド量, センドAUX_A がバス 1 のセンド量, センドAUX_B がバス 2 のセンド量, センドAUX_C がバス 3 のセンド量に対応します。
    //!          デフォルト値は 0.0 です。
    //!
    //! @param[in] subMixBus send を適用するバスです
    //! @param[in] send      センド量の相対変化の値です。
    //!
    //! @pre
    //!        - (カスタムサブミックス機能を有効にしない場合)
    //!            - 0 <= subMixBus < nn::atk::AuxBus_Count + 1
    //!        - (カスタムサブミックス機能を有効にした場合)
    //!            - 0 <= subMixBus < nn::atk::AuxBus_Count + 1 || 0 <= subMixBus < (nn::atk::SoundSystem の初期化時に設定した busCountMax)
    //!
    //! @see GetSend
    //---------------------------------------------------------------------------
    void SetSend( int subMixBus, float send ) NN_NOEXCEPT;

    //! @brief   プレイヤーのバスごとに設定するセンド量を取得します。
    //! @return 指定したエフェクトバスへのセンド量を返します。
    //!
    //! @param[in] subMixBus センド量を設定するバスです。
    //!
    //! @pre
    //!        - (カスタムサブミックス機能を有効にしない場合)
    //!            - 0 <= subMixBus < nn::atk::AuxBus_Count + 1
    //!        - (カスタムサブミックス機能を有効にした場合)
    //!            - 0 <= subMixBus < nn::atk::AuxBus_Count + 1 || 0 <= subMixBus < (nn::atk::SoundSystem の初期化時に設定した busCountMax)
    //!
    //! @see SetSend
    float GetSend( int subMixBus ) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 出力先別パラメータ
    //@{
    //---------------------------------------------------------------------------
    //! @brief  指定した出力先について、プレイヤーの音量を変更します。
    //!
    //!         プレイヤーの音量は、
    //!         プレイヤーで再生中のすべてのサウンドに対して効果があります。
    //!
    //!         この関数で指定する値は、
    //!         サウンドに設定される他のどの音量パラメータとも独立して動作し、
    //!         それらはすべて重ね合わされます。
    //!
    //!         この出力音量は、
    //!         指定した番号の指定した出力先に対してのみ影響する音量です。
    //!
    //!         音量 volume は、0.0 以上の倍率で指定します。
    //!         すなわち、1.0 を指定すると音量に影響を与えません。
    //!         0.0 を指定すると発音されなくなります。デフォルト値は 1.0 です。
    //!
    //!         他の出力音量と重ね合わされたあと、
    //!         最終的な音量は nn::audio::VoiceType::GetVolumeMin() ～ nn::audio::VoiceType::GetVolumeMax() の範囲でクランプされます。
    //!         音量の重ね合わせの詳細につきましては @confluencelink{83955829, サウンドパラメータ制御の動作} を参照ください。
    //!
    //! @param[in] device       出力先のデバイスです。
    //! @param[in] volume       変更する音量の倍率です (0.0～)。
    //!
    //! @see GetOutputVolume
    //---------------------------------------------------------------------------
    void SetOutputVolume( OutputDevice device,float volume ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  指定した出力先について、プレイヤーに設定されている音量を取得します。
    //!
    //! @param[in] device   出力先のデバイスです。
    //!
    //! @return 指定した出力先の現在の音量を返します。
    //!         出力先のデバイスが存在しない場合、0.0f を返します。
    //!
    //! @see SetOutputVolume
    //---------------------------------------------------------------------------
    float GetOutputVolume( OutputDevice device ) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE( device, 0, OutputDevice_Count );
        switch ( device )
        {
        case nn::atk::OutputDevice_Main:
            return m_TvParam.volume;
        default:
            NN_SDK_ASSERT( false, "Invalid device\n" );
            break;
        }
        return 0.0f;
    }

    //! @briefprivate
    //! @param device :private
    //! @param send :private
    void SetOutputMainSend( OutputDevice device, float send ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE( device, 0, OutputDevice_Count );
        switch ( device )
        {
        case nn::atk::OutputDevice_Main:
            m_TvParam.mainSend = send;
            break;
        default:
            NN_SDK_ASSERT( false, "Invalid device\n" );
            break;
        }
    }
    //! @briefprivate
    //! @param device :private
    //! @return :private
    float GetOutputMainSend( OutputDevice device ) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE( device, 0, OutputDevice_Count );
        switch ( device )
        {
        case nn::atk::OutputDevice_Main:
            return m_TvParam.mainSend;
        default:
            NN_SDK_ASSERT( false, "Invalid device\n" );
            break;
        }
        return 0.0f;
    }

    //! @briefprivate
    //! @param device :private
    //! @param bus :private
    //! @param send :private
    void SetOutputFxSend( OutputDevice device, AuxBus bus, float send ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE( device, 0, OutputDevice_Count );
        NN_SDK_ASSERT_RANGE( bus, 0, AuxBus_Count );
        switch ( device )
        {
        case nn::atk::OutputDevice_Main:
            m_TvParam.fxSend[bus] = send;
            break;
        default:
            NN_SDK_ASSERT( false, "Invalid device\n" );
            break;
        }
    }
    //! @briefprivate
    //! @param device :private
    //! @param bus :private
    //! @return :private
    float GetOutputFxSend( OutputDevice device, AuxBus bus ) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE( device, 0, OutputDevice_Count );
        NN_SDK_ASSERT_RANGE( bus, 0, AuxBus_Count );
        switch ( device )
        {
        case nn::atk::OutputDevice_Main:
            return m_TvParam.fxSend[bus];
        default:
            NN_SDK_ASSERT( false, "Invalid device\n" );
            break;
        }
        return 0.0f;
    }
    //@}

    //----------------------------------------
    //! @name サウンド数
    //@{
    //---------------------------------------------------------------------------
    //! @brief  プレイヤーで現在再生中のサウンドの個数を取得します。
    //!
    //! @return プレイヤーで再生中のサウンド数を返します。
    //!
    //---------------------------------------------------------------------------
    int GetPlayingSoundCount() const NN_NOEXCEPT { return static_cast<int>( m_SoundList.size()); }

    //---------------------------------------------------------------------------
    //! @brief  同時に再生可能なサウンド数を設定します。
    //!
    //!         設定したサウンド数を超えるサウンドを再生しようとすると、
    //!         各サウンドに設定されたプレイヤープライオリティを比較して、
    //!         もっとも優先度が低いサウンドが停止します。
    //!
    //!         @ref SoundArchivePlayer クラスを使用する場合は、
    //!         サウンドアーカイブ中で指定されている同時再生数が初期化時に自動的に設定されます。
    //!
    //!         プレイヤーヒープを使用している場合、
    //!         この関数で指定できる同時再生数の上限値は、
    //!         セットアップ時に設定された同時再生数の値に制限されます。
    //!         上限値を超える値を指定しても上限値に丸め込まれます。
    //!
    //! @param[in] count        同時に再生可能なサウンド数です。
    //!
    //! @see SoundArchivePlayer クラス
    //! @see GetPlayableSoundCount
    //!
    //---------------------------------------------------------------------------
    void SetPlayableSoundCount( int count ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  プレイヤーで同時に再生可能なサウンド数を取得します。
    //!
    //! @return プレイヤーで同時に再生可能なサウンド数を返します。
    //!
    //! @see SetPlayableSoundCount
    //!
    //---------------------------------------------------------------------------
    int GetPlayableSoundCount() const NN_NOEXCEPT { return m_PlayableCount; }
    //@}

    //----------------------------------------
    //! @name その他
    //@{
    //---------------------------------------------------------------------------
    //! @brief  プレイヤーで再生中のすべてのサウンドに対して処理を行います。
    //!
    //!         @ref ForEachSound のFunctionの参照渡し版となります。
    //!
    //! @param[in] function 関数ポインタ、または関数オブジェクトです。
    //! @param[in] reverse  処理順を逆にする場合は true を指定します。
    //!
    //! @see SoundHandle クラス
    //! @see ForEachSoundPriorityOrderReference
    //---------------------------------------------------------------------------
    template< class Function >
    void ForEachSoundReference( Function& function, bool reverse = false ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  プレイヤーで再生中のすべてのサウンドに対して処理を行います。
    //!
    //!         プレイヤーで再生中のすべてのサウンドに対して、
    //!
    //! @code
    //! function( nn::atk::SoundHandle& handle )
    //! @endcode
    //!
    //!         を呼び出します。 function には、
    //!         再生中のサウンドに関連付けられたサウンドハンドル handle が渡されます。
    //!         これは一時的なハンドルですので、ハンドルを後で使用することはできません。
    //!
    //!         関数 function は、サウンドの再生が古い順に呼び出されます。
    //!         reverse に true を指定すると、サウンドの再生が新しい順に呼び出されます。
    //!
    //!         function には関数ポインタ、または関数オブジェクトを渡します。
    //!         関数ポインタを渡す例を以下に示します。
    //!
    //! @code
    //! void ReportSoundId( nn::atk::SoundHandle& handle )
    //! {
    //!     NN_LOG( "%d\n", handle.GetId() );
    //! }
    //! soundPlayer.ForEachSound( ReportSoundId );
    //! @endcode
    //!
    //! @param[in] function 関数ポインタ、または関数オブジェクトです。
    //! @param[in] reverse  処理順を逆にする場合は true を指定します。
    //!
    //! @return 引数に指定された関数ポインタ、または関数オブジェクトを返します。
    //!
    //! @see SoundHandle クラス
    //! @see ForEachSoundPriorityOrder
    //!
    //---------------------------------------------------------------------------
    template< class Function >
    Function ForEachSound( Function function, bool reverse = false ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  プレイヤーで再生中のすべてのサウンドに対してプレイヤープライオリティ順で処理を行います。
    //!
    //!         その他の動作仕様は @ref ForEachSoundReference と同じです。
    //!
    //! @param[in] function 関数ポインタ、または関数オブジェクトです。
    //! @param[in] reverse  処理順を逆にする場合は true を指定します。
    //!
    //! @see SoundHandle クラス
    //! @see ForEachSoundReference
    //---------------------------------------------------------------------------
    template< class Function >
    void ForEachSoundPriorityOrderReference( Function& function, bool reverse = false ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  プレイヤーで再生中のすべてのサウンドに対してプレイヤープライオリティ順で処理を行います。
    //!
    //!         その他の動作仕様は @ref ForEachSound と同じです。
    //!
    //! @param[in] function 関数ポインタ、または関数オブジェクトです。
    //! @param[in] reverse  処理順を逆にする場合は true を指定します。
    //!
    //! @return 引数に指定された関数ポインタ、または関数オブジェクトを返します。
    //!
    //! @see SoundHandle クラス
    //! @see ForEachSound
    //!
    //---------------------------------------------------------------------------
    template< class Function >
    Function ForEachSoundPriorityOrder( Function function, bool reverse = false ) NN_NOEXCEPT;
    //@}

    //-----------------------------------------------------------------------------
    //  detail functions

    //! @internal
    //! @param limit :private
    void detail_SetPlayableSoundLimit( int limit ) NN_NOEXCEPT;

    // サウンドの登録
    //! @internal
    //! @param startPriority :private
    //! @return :private
    bool detail_CanPlaySound( int startPriority ) NN_NOEXCEPT;
    //! @internal
    //! @param pSound :private
    //! @return :private
    bool detail_AppendSound( detail::BasicSound* pSound ) NN_NOEXCEPT;
    //! @internal
    //! @param pSound :private
    void detail_RemoveSound( detail::BasicSound* pSound ) NN_NOEXCEPT;

    // プライオリティリスト
    //! @internal
    //! @param reverse :private
    void detail_SortPriorityList( bool reverse = false ) NN_NOEXCEPT;
    //! @internal
    //! @param pSound :private
    void detail_SortPriorityList( detail::BasicSound* pSound ) NN_NOEXCEPT;

    // プレイヤーヒープ
    //! @internal
    //! @param pHeap :private
    void detail_AppendPlayerHeap( detail::PlayerHeap* pHeap ) NN_NOEXCEPT;

    // void detail_RemovePlayerHeap( detail::PlayerHeap* pHeap );

    //! @internal
    //! @return :private
    bool detail_CanUsePlayerHeap() const NN_NOEXCEPT
    {
        if (m_PlayerHeapCount > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //! @internal
    //! @return :private
    detail::PlayerHeap* detail_AllocPlayerHeap() NN_NOEXCEPT;
    //! @internal
    //! @param pHeap :private
    void detail_FreePlayerHeap(detail::PlayerHeap* pHeap) NN_NOEXCEPT;

    //! @briefprivate
    //! @return :private
    bool IsFirstComeBased() NN_NOEXCEPT { return m_IsFirstComeBased; }
    //! @briefprivate
    //! @param enabled :private
    void SetFirstComeBased( bool enabled ) NN_NOEXCEPT
    {
        if ( m_IsFirstComeBased != enabled )
        {
            m_IsFirstComeBased = enabled;
            detail_SortPriorityList( true );
        }
    }

private:
    //-----------------------------------------------------------------------------
    //  private functions

    detail::BasicSound* GetLowestPrioritySound() NN_NOEXCEPT { return &m_PriorityList.front(); }
    void InsertPriorityList( detail::BasicSound* pSound ) NN_NOEXCEPT;
    void RemovePriorityList( detail::BasicSound* pSound ) NN_NOEXCEPT;
    void RemoveSoundList( detail::BasicSound* pSound ) NN_NOEXCEPT;

    void DoFreePlayerHeap() NN_NOEXCEPT;

    //-----------------------------------------------------------------------------
    //  private variables

    SoundList       m_SoundList;
    PriorityList    m_PriorityList;
    PlayerHeapList  m_PlayerHeapFreeList;
    PlayerHeapList  m_PlayerHeapFreeReqList;

    int m_PlayableCount;
    int m_PlayableLimit;
    uint32_t m_PlayerHeapCount;

    float m_Volume;
    float m_LpfFreq;
    int m_BiquadType;
    float m_BiquadValue;

    uint32_t m_OutputLineFlag;

    struct OutputParam
    {
        float volume;
        float mainSend;
        float fxSend[AuxBus_Count];

        void Initialize() NN_NOEXCEPT
        {
            volume = 1.0f;
            mainSend = 0.0f;
            for ( int i = 0; i < AuxBus_Count; i++ )
            {
                fxSend[i] = 0.0f;
            }
        }
    };
    OutputParam m_TvParam;
    detail::OutputAdditionalParam* m_pOutputAdditionalParam;

    bool m_IsFirstComeBased;
};

template< class Function >
inline void SoundPlayer::ForEachSoundReference( Function& function, bool reverse ) NN_NOEXCEPT
{
    if ( reverse )
    {
        // 再生の新しい順
        for ( SoundList::reverse_iterator itr = m_SoundList.rbegin();
              itr != m_SoundList.rend();
            )
        {
            SoundList::reverse_iterator curItr = itr;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle( &( *curItr ) );
            function( handle );
            if ( handle.IsAttachedSound() ) itr++;
        }
    }
    else
    {
        // 再生の古い順
        for ( SoundList::iterator itr = m_SoundList.begin();
              itr != m_SoundList.end();
            )
        {
            SoundList::iterator curItr = itr++;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle( &( *curItr ) );
            function( handle );
        }
    }
}

template< class Function >
inline Function SoundPlayer::ForEachSound( Function function, bool reverse ) NN_NOEXCEPT
{
    if ( reverse )
    {
        // 再生の新しい順
        for ( SoundList::reverse_iterator itr = m_SoundList.rbegin();
              itr != m_SoundList.rend();
            )
        {
            SoundList::reverse_iterator curItr = itr;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle( &( *curItr ) );
            function( handle );
            if ( handle.IsAttachedSound() ) itr++;
        }
    }
    else
    {
        // 再生の古い順
        for ( SoundList::iterator itr = m_SoundList.begin();
              itr != m_SoundList.end();
            )
        {
            SoundList::iterator curItr = itr++;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle( &( *curItr ) );
            function( handle );
        }
    }
    return function;
}

template< class Function >
void SoundPlayer::ForEachSoundPriorityOrderReference( Function& function, bool reverse ) NN_NOEXCEPT
{
    if ( reverse )
    {
        // プライオリティの低い順
        for ( PriorityList::iterator itr = m_PriorityList.begin();
              itr != m_PriorityList.end();
            )
        {
            PriorityList::iterator curItr = itr++;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle( &( *curItr ) );
            function( handle );
        }
    }
    else
    {
        // プライオリティの高い順
        for ( PriorityList::reverse_iterator itr = m_PriorityList.rbegin();
              itr != m_PriorityList.rend();
            )
        {
            PriorityList::reverse_iterator curItr = itr;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle( &( *curItr ) );
            function( handle );
            if ( handle.IsAttachedSound() ) itr++;
        }
    }
}

template< class Function >
Function SoundPlayer::ForEachSoundPriorityOrder( Function function, bool reverse ) NN_NOEXCEPT
{
    if ( reverse )
    {
        // プライオリティの低い順
        for ( PriorityList::iterator itr = m_PriorityList.begin();
              itr != m_PriorityList.end();
            )
        {
            PriorityList::iterator curItr = itr++;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle( &( *curItr ) );
            function( handle );
        }
    }
    else
    {
        // プライオリティの高い順
        for ( PriorityList::reverse_iterator itr = m_PriorityList.rbegin();
              itr != m_PriorityList.rend();
            )
        {
            PriorityList::reverse_iterator curItr = itr;
            SoundHandle handle;
            handle.detail_AttachSoundAsTempHandle( &( *curItr ) );
            function( handle );
            if ( handle.IsAttachedSound() ) itr++;
        }
    }
    return function;
}

} // namespace nn::atk
} // namespace nn

