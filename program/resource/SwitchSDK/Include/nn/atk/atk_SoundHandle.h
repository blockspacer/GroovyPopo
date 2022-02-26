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
 * :include nn/atk/atk_SoundHandle.h
 *
 * @file atk_SoundHandle.h
 */

#pragma once

#include <nn/atk/atk_BasicSound.h>
#include <nn/atk/atk_Debug.h>
#include <nn/atk/atk_Global.h>      // BiquadFilterType_UserMax
#include <nn/atk/atk_ChannelMixVolume.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @brief  再生したサウンドの操作を行うためのハンドルクラスです。
//!
//!
//!         ■ 概要
//!
//!         サウンドハンドルは、 @ref SoundArchivePlayer::PrepareSound または
//!         @ref SoundArchivePlayer::StartSound を呼び出すことにより、
//!         サウンドと関連付けられます。
//!
//!         その際、ハンドルが既に別のサウンドと関連付けられていた場合は、
//!         古いサウンドとの関連は外され、新たに再生するサウンドと関連付けられます。
//!
//!         下記の状態になると、ハンドルとサウンドの関連が外れ、
//!         その後はサウンドに対しての操作が行えないようになります。
//!
//!         - サウンドが終端に達して停止したとき
//!         - フェードフレーム 0 で @ref Stop をコールしたとき
//!         - フェードフレームを 1 以上に指定して @ref Stop をコールし、
//!           そのフレーム数だけ時間が経過して停止したとき
//!         - 明示的に SoundHandle::DetachSound を呼び出したとき
//!         - プレイヤープライオリティの判定でサウンドが停止したとき
//!
//!         したがって、@ref IsAttachedSound を毎フレーム監視することで、
//!         サウンドが終端に達して停止したかどうか、確認することができます。
//!         ただし、@ref Stop での停止や明示的な @ref DetachSound 、
//!         プレイヤープライオリティ判定でのサウンド停止の影響を考慮しておく必要があります。
//!
//!         プレイヤープライオリティの判定については、サウンドシステムマニュアルの
//!         「プライオリティの動作」の章をご参照ください。
//!
//!         @n
//!
//!         ■ サウンドハンドルの種類
//!
//!         サウンドにはシーケンスサウンド・ストリームサウンド・ウェーブサウンドの
//!         3 種類があります。
//!         SoundHandle クラスはこれらすべてのサウンドの共通の操作を扱うことができます。
//!
//!         SoundHandle クラスでは扱えない、各サウンド専用の操作を行うために、
//!         @ref SequenceSoundHandle クラス、
//!         @ref StreamSoundHandle クラス、
//!         @ref WaveSoundHandle クラスが用意されています。
//!
//!         @n
//!
//!         ■ 出力先別パラメータ
//!
//!         サウンドハンドルでは、音量・ピッチ・パン
//!         などのパラメータを設定することができますが、
//!         具体的には下記のような API の影響を受けます。
//!
//!         (全体)
//!         - 音量 ... @ref SetVolume, @ref FadeIn, @ref Pause, @ref Stop
//!         - パン ... @ref SetPan
//!         - サラウンドパン ... @ref SetSurroundPan
//!         - メインセンド ... @ref SetMainSend
//!         - エフェクトセンド ... @ref SetEffectSend
//!         - ピッチ ... @ref SetPitch
//!         - LPF ... @ref SetLowPassFilterFrequency
//!         - Biquad フィルタ ... @ref SetBiquadFilter
//!         - 出力先 ... @ref SetOutputLine, @ref ResetOutputLine
//!
//!         (TV 出力向け)
//!         - 音量 ... 「全体」の音量に加え、@ref SetOutputVolume
//!         - パン ... 「全体」のパンに加え、@ref SetOutputPan
//!         - サラウンドパン ... 「全体」のサラウンドパンに加え、@ref SetOutputSurroundPan
//!         - メインセンド ... 「全体」のメインセンドに加え、@ref SetOutputMainSend
//!         - エフェクトセンド ... 「全体」のエフェクトセンドに加え、@ref SetOutputEffectSend
//!
//!         @n
//!
//!         各パラメータは、上記の影響の他に、下記の影響を受けます。
//!         - SoundMaker で指定したデータの影響
//!         - @ref SoundPlayer や @ref SoundActor による影響
//!         - 3D サウンド計算 (@ref Sound3DEngine) による影響
//!
//! @see SoundArchivePlayer クラス
//! @see SequenceSoundHandle クラス
//! @see StreamSoundHandle クラス
//! @see WaveSoundHandle クラス
//---------------------------------------------------------------------------
class SoundHandle
{
public:
    //---------------------------------------------------------------------------
    //! @brief  ポーズ状態をあらわす列挙体です。
    //!
    //!         - PauseState_Normal のときに、@ref SoundHandle::Pause( true, ... )
    //!           を呼び出すと、状態は PauseState_Pausing になります。
    //!         - PauseState_Pausing のときに、
    //!           - @ref SoundHandle::Pause( true, ... ) を呼び出すと、
    //!             状態は PauseState_Pausing のままです。
    //!           - @ref SoundHandle::Pause( false, ... ) を呼び出すと、
    //!             状態は PauseState_Unpausing になります。
    //!           - フェード処理が終了すると、状態は PauseState_Paused になります。
    //!         - PauseState_Paused のとき、@ref SoundHandle::Pause( false, ... )
    //!           を呼び出すと、状態は PauseState_Unpausing になります。
    //!         - PauseState_Unpausing のときに、
    //!           - @ref SoundHandle::Pause( true, ... ) を呼び出すと、
    //!             状態は PauseState_Pausing になります。
    //!           - @ref SoundHandle::Pause( false, ... ) を呼び出すと、
    //!             状態は PauseState_Unpausing のままです。
    //!           - フェード処理が終了すると、状態は PauseState_Normal になります。
    //!
    //! @see GetPauseState
    //---------------------------------------------------------------------------
    enum PauseState
    {
        PauseState_Normal,     //!< 非ポーズ状態です。
        PauseState_Pausing,    //!< ポーズへの移行中をあらわします。
        PauseState_Paused,     //!< ポーズ中をあらわします。
        PauseState_Unpausing,  //!< ポーズ解除への移行中をあらわします。
        PauseState_Invalid //!< 無効な状態です。ハンドルにサウンドが関連付けられていないことをあらわします。
    };

    //---------------------------------------------------------------------------
    //! @brief  ミュート状態をあらわす列挙体です。
    //!
    //!         - MuteState_Normal のときに、@ref SoundHandle::Mute( true, ... )
    //!           を呼び出すと、状態は MuteState_Muting になります。
    //!         - MuteState_Muting のときに、
    //!           - @ref SoundHandle::Mute( true, ... ) を呼び出すと、
    //!             状態は MuteState_Muting のままです。
    //!           - @ref SoundHandle::Mute( false, ... ) を呼び出すと、
    //!             状態は MuteState_Unmuting になります。
    //!           - フェード処理が終了すると、状態は MuteState_Muted になります。
    //!         - MuteState_Muted のとき、@ref SoundHandle::Mute( false, ... )
    //!           を呼び出すと、状態は MuteState_Muting になります。
    //!         - MuteState_Unmuting のときに、
    //!           - @ref SoundHandle::Mute( true, ... ) を呼び出すと、
    //!             状態は MuteState_Muting になります。
    //!           - @ref SoundHandle::Mute( false, ... ) を呼び出すと、
    //!             状態は MuteState_Unmuting のままです。
    //!           - フェード処理が終了すると、状態は MuteState_Normal になります。
    //!
    //! @see GetMuteState
    //---------------------------------------------------------------------------
    enum MuteState
    {
        MuteState_Normal,    //!< 非ミュート状態です。
        MuteState_Muting,    //!< ミュートへの移行中をあらわします。
        MuteState_Muted,     //!< ミュート中をあらわします。
        MuteState_Unmuting,  //!< ミュート解除への移行中をあらわします。
        MuteState_Invalid //!< 無効な状態です。ハンドルにサウンドが関連付けられていないことをあらわします。
    };

    //----------------------------------------
    //! @name コンストラクタ/デストラクタ
    //@{
    //! @brief コンストラクタです。
    SoundHandle() NN_NOEXCEPT : m_pSound( NULL ) { }
    //! @brief デストラクタです。
    ~SoundHandle() NN_NOEXCEPT { DetachSound(); }
    //@}

    //----------------------------------------
    //! @name 再生/停止/一時停止/再開
    //@{
    //---------------------------------------------------------------------------
    //! @brief    再生準備が完了したサウンドを再生します。
    //!
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           サウンドの再生を開始するためには、
    //!           @ref SoundArchivePlayer::PrepareSound を呼び出した後、
    //!           再生準備が完了していなければなりません。
    //!           この関数は、再生準備が完了したサウンドの再生を開始します。
    //!           再生準備が完了していないサウンドは、
    //!           完了するまで待ってから再生を開始します。
    //!
    //! @see SoundArchivePlayer::PrepareSound
    //!
    //---------------------------------------------------------------------------
    void StartPrepared() NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->StartPrepared();
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    サウンドを停止します。
    //!
    //!           ハンドルに関連付けられたサウンドを停止します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           fadeFrames で指定したフレーム数をかけて
    //!           フェードアウトさせることができます。
    //!           0 を指定した場合は、フェードアウトを行いません。
    //!           ただし、シーケンスサウンドで発音中の音は、
    //!           エンベロープのリリースを発音しすべての減衰が完了した後に
    //!           サウンドが停止します。
    //!
    //!           フェードアウトの音量制御は、フェードインと共有されます。
    //!           フェードアウトにかかるフレーム数は、
    //!           最大音量から音が消えるまでにかかる変化速度を表しますので、
    //!           フェードイン中にフェードアウトを指定した時などは、
    //!           指定したフレーム数よりも短い時間で
    //!           フェードアウトが完了する可能性があります。
    //!
    //! @param[in] fadeFrames   フェードアウトにかけるフレーム数です。
    //!                         フレーム数は @ref SoundArchivePlayer::Update
    //!                         の呼び出し回数で換算されます。
    //!                         fadeFrames には 0 以上の値を指定してください。
    //!                         負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //!
    //! @see Pause
    //---------------------------------------------------------------------------
    void Stop( int fadeFrames ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            NN_ATK_LOG_APICALL_ATK("SoundHandle::Stop [ID:%08x]", m_pSound->GetId());
            m_pSound->Stop( fadeFrames );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    サウンドを一時停止または再開します。
    //!
    //!           ハンドルに関連付けられたサウンドを一時停止または再開します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           fadeFrames で指定したフレーム数をかけてフェードアウトしながら一時停止、
    //!           またはフェードインしながら再開させることができます。
    //!
    //!           一時停止・再開時のフェードは、再生開始時のフェードイン、
    //!           停止時のフェードアウトとは独立してはたらきます。
    //!
    //!           fadeFrames に 0 を指定した場合、
    //!           一時停止状態に変更する際には、 1 回目の @ref SoundArchivePlayer::Update 後に音量だけがフェードアウトなしに 0 になり、
    //!           2 回目の @ref SoundArchivePlayer::Update 後に一時停止状態となります。
    //!           このとき、 @ref SoundArchivePlayer::Update の 1 回目と 2 回目の呼び出し間隔分だけ、
    //!           無音状態で再生時間が進む点にご注意ください。
    //!           一方、再開時には 1 回目の @ref SoundArchivePlayer::Update 後にフェードインなしに再開するため注意する必要はありません。
    //!
    //!           また、フェードにかかるフレーム数は、最大音量から音が消えるまで、
    //!           あるいは発音していない状態から最大音量に達するまでにかかる変化速度を表しますので、
    //!           フェードイン中にさらにフェードアウトを指定した時などは、
    //!           指定したフレーム数よりも短い時間でフェードが完了する可能性があります。
    //!
    //!           ポーズ状態については、@ref GetPauseState および @ref PauseState
    //!           の説明をごらんください。
    //!
    //! @param[in]    flag        true なら一時停止、false なら再開します。
    //! @param[in]    fadeFrames  フェードイン・フェードアウトにかけるフレーム数です。
    //!                           フレーム数は @ref SoundArchivePlayer::Update
    //!                           の呼び出し回数で換算されます。
    //!                           負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //!
    //! @see IsPause
    //! @see GetPauseState
    //! @see PauseState
    //---------------------------------------------------------------------------
    void Pause( bool flag, int fadeFrames ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            NN_ATK_LOG_APICALL_ATK("SoundHandle::Pause [ID:%08x flag:%s frame:%d]", m_pSound->GetId(),flag?"true":"false",fadeFrames);
            m_pSound->Pause( flag, fadeFrames );
        }
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    サウンドを一時停止または再開します。
    //!
    //!           ハンドルに関連付けられたサウンドを一時停止または再開します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           fadeFrames で指定したフレーム数をかけてフェードアウトしながら一時停止、
    //!           またはフェードインしながら再開させることができます。
    //!
    //!           一時停止・再開時のフェードは、再生開始時のフェードイン、
    //!           停止時のフェードアウトとは独立してはたらきます。
    //!
    //!           また、フェードにかかるフレーム数は、最大音量から音が消えるまで、
    //!           あるいは発音していない状態から最大音量に達するまでにかかる変化速度を表しますので、
    //!           フェードイン中にさらにフェードアウトを指定した時などは、
    //!           指定したフレーム数よりも短い時間でフェードが完了する可能性があります。
    //!
    //!           ポーズ状態については、@ref GetPauseState および @ref PauseState
    //!           の説明をごらんください。
    //!
    //! @param[in]    flag           true なら一時停止、false なら再開します。
    //! @param[in]    fadeFrames     フェードイン・フェードアウトにかけるフレーム数です。
    //!                              フレーム数は @ref SoundArchivePlayer::Update
    //!                              の呼び出し回数で換算されます。
    //!                              負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //! @param[in]    pauseMode      ポーズの設定です。
    //!
    //! @see IsPause
    //! @see GetPauseState
    //! @see PauseState
    //---------------------------------------------------------------------------
    void Pause(bool flag, int fadeFrames, PauseMode pauseMode) NN_NOEXCEPT
    {
        if (IsAttachedSound())
        {
            NN_ATK_LOG_APICALL_ATK("SoundHandle::Pause [ID:%08x flag:%s frame:%d]", m_pSound->GetId(), flag ? "true" : "false", fadeFrames);
            m_pSound->Pause(flag, fadeFrames, pauseMode);
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドをミュートまたは再開します。
    //!
    //!         ハンドルに関連付けられたサウンドをミュートまたは再開します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         fadeFrames で指定したフレーム数をかけてフェードアウトしながらミュート、
    //!         またはフェードインしながら再開させることができます。
    //!         0 を指定した場合は、即座にミュートまたは再開します。
    //!
    //!         ミュート・再開時のフェードは、再生開始や一時停止のフェードイン、再生/停止開始時や一時停止/再開のフェードイン、
    //!         フェードアウトとは独立してはたらきます。
    //!         フェードにかかるフレーム数は、最大音量から音が消えるまで、
    //!         あるいは発音していない状態から最大音量に達するまでに
    //!         かかる変化速度を表しますので、
    //!         フェード中にさらにフェードを指定した時などは、
    //!         指定したフレーム数よりも短い時間でフェードが完了する可能性があります。
    //!
    //!         ミュート状態については、@ref GetMuteState および @ref MuteState
    //!         の説明をごらんください。
    //!
    //! @param[in]    flag        true ならミュート、false なら再開します。
    //! @param[in]    fadeFrames  フェードイン・フェードアウトにかけるフレーム数です。
    //!                           フレーム数は @ref SoundArchivePlayer::Update
    //!                           の呼び出し回数で換算されます。
    //!                           fadeFrames には 0 以上の値を指定してください。
    //!                           負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //!
    //! @see IsMute
    //! @see GetMuteState
    //! @see MuteState
    //---------------------------------------------------------------------------
    void Mute( bool flag, int fadeFrames ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() ) m_pSound->Mute( flag, fadeFrames );
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドの再生準備が完了しているかどうかを調べます。
    //!
    //!         @ref SoundArchivePlayer::PrepareSound を呼び出した後、
    //!         そのサウンドの再生準備が完了しているかどうかを調べます。
    //!         再生準備が完了したサウンドは、
    //!         @ref SoundHandle::StartPrepared を呼び出した際に、
    //!         すぐに再生を始めることができます。
    //!
    //!         @ref SoundArchivePlayer::StartSound や HoldSound を呼び出した場合も、
    //!         内部で PrepareSound を呼び出しているため、内部呼び出しの PrepareSound
    //!         が完了した時点で、本関数は true を返します。
    //!
    //! @return   サウンドの再生準備が完了していれば true を返します。
    //!
    //! @see SoundArchivePlayer::PrepareSound
    //! @see StartPrepared
    //---------------------------------------------------------------------------
    bool IsPrepared() const NN_NOEXCEPT
    {
        if ( IsAttachedSound() && m_pSound->IsPrepared() )
        {
            return true;
        }
        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief    サウンドが一時停止中かどうかを調べます。
    //!
    //! @return   一時停止状態へのフェード中および、一時停止中の場合に true を返します。
    //!           これは、 @ref GetPauseState で PauseState_Pausing および、
    //!           PauseState_Paused が返っている状態と同じです。
    //!
    //! @see GetPauseState
    //---------------------------------------------------------------------------
    bool IsPause() const NN_NOEXCEPT
    {
        if ( IsAttachedSound() && m_pSound->IsPause() )
        {
            return true;
        }
        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief    サウンドがミュート中かどうかを調べます。
    //!
    //! @return   ミュート状態へのフェード中および、ミュート中の場合に true を返します。
    //---------------------------------------------------------------------------
    bool IsMute() const NN_NOEXCEPT
    {
        if ( IsAttachedSound() && m_pSound->IsMute() )
        {
            return true;
        }
        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドのポーズ状態を調べます。
    //!
    //! @return ポーズ状態を返します。
    //!
    //! @see IsPause
    //! @see PauseState
    //---------------------------------------------------------------------------
    PauseState GetPauseState() const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() )
        {
            return PauseState_Invalid;
        }

        detail::BasicSound::PauseState state = m_pSound->GetPauseState();
        return static_cast<PauseState>(state);
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドのミュート状態を調べます。
    //!
    //! @return ミュート状態を返します。
    //!
    //! @see IsMute
    //! @see MuteState
    //---------------------------------------------------------------------------
    MuteState GetMuteState() const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() )
        {
            return MuteState_Invalid;
        }

        detail::BasicSound::MuteState state = m_pSound->GetMuteState();
        return static_cast<MuteState>(state);
    }

    //---------------------------------------------------------------------------
    //! @brief    サウンドを再生開始時にフェードインさせます。
    //!
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数でフェードインの指定を行うと、
    //!           @ref SoundArchivePlayer::StartSound または
    //!           @ref SoundHandle::StartPrepared を呼び出して再生を開始してから、
    //!           最初の @ref SoundArchivePlayer::Update が呼び出されたときに
    //!           フェードインが設定されます。
    //!
    //!           サウンドの再生が開始され、最初の @ref SoundArchivePlayer::Update
    //!           が呼ばれた後にこの関数を呼び出しても効果がありません。
    //!
    //!           フェードインの音量制御は、停止時のフェードアウトと共有されます。
    //!
    //! @param[in]    frames  フェードインにかけるフレーム数です。
    //!                       フレーム数は @ref SoundArchivePlayer::Update
    //!                       の呼び出し回数で換算されます。
    //!                       frames には 0 以上の値を指定してください。
    //!                       負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //!
    //! @see StartPrepared
    //! @see SoundArchivePlayer::StartSound
    //---------------------------------------------------------------------------
    void FadeIn( int frames ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->FadeIn( frames );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    フェードイン・フェードアウト完了までの残りフレーム数を取得します。
    //!
    //!           フェード中でない場合は 0 を返します。
    //!
    //! @return   フェード完了までの残りフレーム数を返します。
    //!           フレーム数は @ref SoundArchivePlayer::Update
    //!           の呼び出し回数で換算されます。
    //!
    //! @see FadeIn
    //! @see Stop
    //!
    //---------------------------------------------------------------------------
    int GetRemainingFadeFrames() const NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            return m_pSound->GetRemainingFadeFrames();
        }
        return 0;
    }

    //---------------------------------------------------------------------------
    //! @brief    一時停止時のフェード完了までの残りフレーム数を取得します。
    //!
    //!           フェード中でない場合は 0 を返します。
    //!
    //! @return   フェード完了までの残りフレーム数を返します。
    //!           フレーム数は @ref SoundArchivePlayer::Update
    //!           の呼び出し回数で換算されます。
    //!
    //! @see Pause
    //!
    //---------------------------------------------------------------------------
    int GetRemainingPauseFadeFrames() const NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            return m_pSound->GetRemainingPauseFadeFrames();
        }
        return 0;
    }

    //---------------------------------------------------------------------------
    //! @brief    サウンドの再生を開始した時から現在までの再生経過フレーム数を取得します。
    //!
    //!           一時停止時(PauseState::PauseState_Paused)のフレーム数は
    //!           カウントされませんが、一時停止や再開のためのフェードの区間はカウントされます。
    //!
    //! @return   再生経過フレーム数を返します。
    //!
    //! @see Pause
    //! @see PauseState
    //---------------------------------------------------------------------------
    uint32_t GetPlayFrameCount() const NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            return m_pSound->GetPlayFrameCount();
        }
        return 0;
    }

    //---------------------------------------------------------------------------
    //! @brief    ミュート時のフェード完了までの残りフレーム数を取得します。
    //!
    //!           フェード中でない場合は 0 を返します。
    //!
    //! @return   フェード完了までの残りフレーム数を返します。
    //!           フレーム数は @ref SoundArchivePlayer::Update
    //!           の呼び出し回数で換算されます。
    //!
    //! @see Mute
    //---------------------------------------------------------------------------
    int GetRemainingMuteFadeFrames() const NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            return m_pSound->GetRemainingMuteFadeFrames();
        }
        return 0;
    }
    //@}

    //----------------------------------------
    //! @name サウンド共通パラメータ
    //@{
    //---------------------------------------------------------------------------
    //! @brief  サウンドの音量を変更します。
    //!
    //!         ハンドルに関連付けられたサウンドの音量を変更します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         この関数で指定する値は、他のどの音量パラメータとも独立して動作し、
    //!         それらはすべて重ね合わされます。
    //!
    //!         音量 volume は、0.0 以上の倍率で指定します。
    //!         すなわち、1.0 を指定すると音量に影響を与えません。
    //!         0.0 を指定すると発音されなくなります。デフォルト値は 1.0 です。
    //!
    //!         他の音量パラメータと重ね合わされたあと、
    //!         最終的な音量は nn::audio::VoiceType::GetVolumeMin() ～ nn::audio::VoiceType::GetVolumeMax() の範囲でクランプされます。
    //!         音量の重ね合わせの詳細につきましては @confluencelink{83955829, サウンドパラメータ制御の動作} を参照ください。
    //!
    //!         音量の変化は frames で指定したフレーム数をかけて行われます。
    //!         音量の変化途中にさらにこの関数を呼び出した場合は、
    //!         その時点での変化途中の音量値を基点として、
    //!         新しく指定したフレーム数をかけて音量を変化させます。
    //!
    //!         出力先別に設定したい場合は、@ref SetOutputVolume を利用してください。
    //!
    //! @param[in]    volume  変更する音量の倍率 ( 0.0 ～ ) です。
    //! @param[in]    frames  音量変化にかけるフレーム数です。
    //!                       フレーム数は @ref SoundArchivePlayer::Update
    //!                       の呼び出し回数で換算されます。
    //!                       frames には 0 以上の値を指定してください。
    //!                       負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //! @pre          volume >= 0.0f
    //!
    //! @see SetOutputVolume
    //---------------------------------------------------------------------------
    void SetVolume( float volume, int frames = 0 ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetVolume( volume, frames );
        }
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  ハンドルに関連付けられたサウンドの音量設定を無視するかどうかを設定します。
    //!         バスごとに音量の設定を無視するかどうかを選択することができます。
    //!         パンの設定やミックスボリュームの設定は反映されます。
    //!
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         この API を利用して設定を行った場合、波形の出力結果が一致しなくなることがあります。
    //!         また、音量無視の設定をしていないバスについても同様に、
    //!         波形の出力結果が完全一致しなくなる点にご注意ください。
    //!
    //! @param[in]    subMixBus    設定を変更するバスです。
    //! @param[in]    modeBitFlag  何の音量を無視するかを選択するビットフラグです。
    //!                            無視できる音量値は @ref VolumeThroughModeBitFlag にある項目となります。
    //!
    //! @pre
    //!        - nn::atk::SoundSystem の初期化時に enableVolumeThroughMode を true にして初期化する
    //!        - 0 <= subMixBus < (SoundHandle に紐づく Sound が持つバス数)
    //!            - カスタムサブミックス機能を有効にしない場合、 (SoundHandle に紐づく Sound が持つバス数)  は nn::atk::AuxBus_Count + 1 となります。
    //!            - カスタムサブミックス機能を有効にした場合、 (SoundHandle に紐づく Sound が持つバス数) は @ref nn::atk::SoundArchivePlayer::StartSound などで指定した @ref nn::atk::OutputReceiver の、
    //!              @ref nn::atk::OutputReceiver::GetBusCount() で取得できる値となります。
    //---------------------------------------------------------------------------
    void SetVolumeThroughMode( int subMixBus, uint8_t modeBitFlag ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetVolumeThroughMode( subMixBus, modeBitFlag );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドのパン (左右の定位) を変更します。
    //!
    //!         ハンドルに関連付けられたサウンドのパンを変更します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         この関数で指定する値は、他のどのパンパラメータとも独立して動作し、
    //!         それらはすべて重ね合わされます。
    //!
    //!         pan は、定位の相対変化の値を設定します。
    //!         入力となる波形が、モノラル波形の場合は以下の挙動になります。
    //!         - 0.0 を指定：データで設定されたパンの値から変化しません。
    //!         - 1.0 を指定：中央に定位していた音が右端に定位するようになります。
    //!         - -1.0 を指定：中央に定位していた音が左端に定位するようになります。
    //!
    //!         入力となる波形が、ステレオ波形で、
    //!         パンモードがデュアルパンに設定されている場合には以下の挙動になります。
    //!         - 0.0 を指定：左チャンネルの波形は左端、右チャンネルの波形は右端に定位します。
    //!         - 2.0 を指定：両チャンネルの波形が右端に定位します。
    //!         - -2.0 を指定：両チャンネルの波形が左端に定位します。
    //!         - 1.0、-1.0 の場合には、どちらかのチャンネルの波形が中央に定位します。
    //!
    //!         また、パンモードがバランスパンに設定されている場合には以下の挙動になります。
    //!         - 1.0 を指定すると右チャンネルの波形のみが聞こえるようになります。
    //!         - -1.0 を指定すると左チャンネルの波形のみが聞こえるようになります。
    //!
    //!         デフォルト値は 0.0 です。
    //!
    //!         @ref SetMixMode で @ref MixMode_MixVolume が指定されている場合は
    //!         この値を指定しても無視されます。
    //!
    //!         出力先別に設定したい場合は、@ref SetOutputPan を利用してください。
    //!
    //! @param[in]    pan  0.0 を基準としたパンの相対変化の値です。
    //!
    //! @see SetOutputPan
    //! @see SetMixMode
    //---------------------------------------------------------------------------
    void SetPan( float pan ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetPan( pan );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドのサラウンドパン (前後の定位) を変更します。
    //!
    //!         ハンドルに関連付けられたサウンドのサラウンドパンを変更します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         この関数で指定する値は、他のどのサラウンドパンパラメータとも独立して動作し、
    //!         それらはすべて重ね合わされます。
    //!
    //!         surroundPan は、定位の相対変化の値を設定します。
    //!         0.0 を指定するとデータで設定されたパンの値から変化しません。
    //!         1.0 を指定すると最前方に定位していた音が中央に定位するようになり、
    //!         2.0 を指定すると最前方に定位していた音が最後方に定位するようになります。
    //!         前方へ定位を移動させたい場合は負の値を指定してください。
    //!
    //!         デフォルト値は 0.0 です。
    //!
    //!         @ref SetMixMode で @ref MixMode_MixVolume が指定されている場合は
    //!         この値を指定しても無視されます。
    //!
    //!         サラウンドパンに設定した値は、出力モードがサラウンドの場合だけ反映されます。
    //!         詳しくは @ref nn::atk::SoundSystem::SetOutputMode を参照ください。
    //!
    //! @param[in] surroundPan 0.0 を基準としたサラウンドパンの相対変化の値です。
    //!
    //! @see SetOutputSurroundPan
    //! @see SetMixMode
    //---------------------------------------------------------------------------
    void SetSurroundPan( float surroundPan ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetSurroundPan( surroundPan );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドの各スピーカーに対する音量配分の設定方法を変更します。
    //!
    //!         ハンドルに関連付けられたサウンドの音量配分の設定方法を変更します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         mixMode には @ref MixMode_Pan か @ref MixMode_MixVolume を設定します。
    //!         @ref MixMode_Pan を指定した場合は、パンとサラウンドパンの値を設定する事で音量配分を行います。
    //!         この場合は @ref SetMixVolume で音量を設定していても無視されます。
    //!
    //!         @ref MixMode_MixVolume を指定した場合は、
    //!         5.1chの各スピーカーの音量を指定する必要があります。
    //!         この場合はパンやサラウンドパンに値を設定していても無視されます。
    //!
    //! @param[in] mixMode     音量配分の設定方法です。
    //!
    //! @see MixMode
    //! @see SetPan
    //! @see SetSurroundPan
    //! @see SetMixVolume
    //---------------------------------------------------------------------------
    void SetMixMode( MixMode mixMode ) NN_NOEXCEPT
    {
        if( IsAttachedSound() )
        {
            m_pSound->SetMixMode( mixMode );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  波形のチャンネル別に、サウンドの5.1chの各スピーカーの音量を設定します。
    //!
    //!         ハンドルに関連付けられたサウンドの音量を変更します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         mixVolume には5.1chの各スピーカーの音量の値を設定します。
    //!         全てのチャンネルの音量を指定する必要があります。
    //!
    //!         @ref SetMixMode で @ref MixMode_Pan が指定されている場合は
    //!         この値を指定しても無視されます。
    //!
    //! @param[in]    sourceChannelIndex mixVolume を適用する波形のチャンネル番号です。
    //! @param[in]    mixVolume          5.1chの各スピーカーの音量です。
    //! @pre          0 <= sourceChannelIndex < WaveChannelMax
    //!
    //! @see MixVolume
    //! @see SetMixMode
    //---------------------------------------------------------------------------
    void SetMixVolume( int sourceChannelIndex, const MixVolume& mixVolume ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(sourceChannelIndex, 0, static_cast<int>(WaveChannelMax));
        MixParameter parameter;
        for ( auto i = 0; i < ChannelIndex_Count; ++i )
        {
            parameter.ch[i] = mixVolume.channel[i];
        }
        SetChannelMixParameter( static_cast<uint32_t>(sourceChannelIndex), parameter );
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  サウンドの各チャンネルに音量を分配する機能を、バスごとに有効化するかどうかを設定します。
    //!         この API により有効にしたバスのみ、 @ref SetBusMixVolume の API で設定した音量が有効になります。
    //!         有効でない場合は、 @ref SetMixMode で指定した音量分配の方法で計算が行われます。
    //!
    //!         SoundSystem の初期化時、 enableBusMixVolume を true にして初期化しないと利用できない点にご注意ください。
    //!
    //! @param[in]    subMixBus          mixVolume を適用するバスです。
    //! @param[in]    isEnabled          BusMixVolume の有効無効を表します。
    //! @pre
    //!        - nn::atk::SoundSystem の初期化時に enableBusMixVolume を true にして初期化する
    //!        - 0 <= subMixBus < (SoundHandle に紐づく Sound が持つバス数)
    //!            - カスタムサブミックス機能を有効にしない場合、 (SoundHandle に紐づく Sound が持つバス数)  は nn::atk::AuxBus_Count + 1 となります。
    //!            - カスタムサブミックス機能を有効にした場合、 (SoundHandle に紐づく Sound が持つバス数) は @ref nn::atk::SoundArchivePlayer::StartSound などで指定した @ref nn::atk::OutputReceiver の、
    //!              @ref nn::atk::OutputReceiver::GetBusCount() で取得できる値となります。
    //!
    //! @see SetBusMixVolume
    //---------------------------------------------------------------------------
    void SetBusMixVolumeEnabled( int subMixBus, bool isEnabled ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(subMixBus, 0, OutputReceiver::BusCountMax);
        if( IsAttachedSound() )
        {
            m_pSound->SetOutputBusMixVolumeEnabled( OutputDevice_Main, subMixBus, isEnabled );
        }
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  波形のチャンネル別に、サウンドの各チャンネルに分配する音量を設定します。
    //!         バスごとに、 ChannelMixVolume::ChannelCountMax までのチャンネルの設定を行うことができます。
    //!
    //!         ハンドルに関連付けられたサウンドの音量を変更します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         mixVolume には各チャンネルに分配する音量の値を設定します。
    //!         全てのチャンネルの音量を指定する必要があります。
    //!
    //!         @ref SetBusMixVolumeEnabled で有効化されたバスのみ、設定した音量が適用されます。
    //!         また、SoundSystem の初期化時、 enableBusMixVolume を true にして初期化しないと利用できない点にご注意ください。
    //!
    //! @param[in]    sourceChannelIndex mixVolume を適用する波形のチャンネル番号です。
    //! @param[in]    subMixBus          mixVolume を適用するバスです。
    //! @param[in]    mixVolume          各チャンネルに分配する音量です。
    //! @pre
    //!        - 0 <= sourceChannelIndex < WaveChannelMax
    //!        - nn::atk::SoundSystem の初期化時に enableBusMixVolume を true にして初期化する
    //!        - 0 <= subMixBus < (SoundHandle に紐づく Sound が持つバス数)
    //!            - カスタムサブミックス機能を有効にしない場合、 (SoundHandle に紐づく Sound が持つバス数)  は nn::atk::AuxBus_Count + 1 となります。
    //!            - カスタムサブミックス機能を有効にした場合、 (SoundHandle に紐づく Sound が持つバス数) は @ref nn::atk::SoundArchivePlayer::StartSound などで指定した @ref nn::atk::OutputReceiver の、
    //!              @ref nn::atk::OutputReceiver::GetBusCount() で取得できる値となります。
    //!
    //! @see ChannelMixVolume
    //! @see SetBusMixVolumeEnabled
    //---------------------------------------------------------------------------
    void SetBusMixVolume( int sourceChannelIndex, int subMixBus, const ChannelMixVolume& mixVolume ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(sourceChannelIndex, 0, static_cast<int>(WaveChannelMax));
        NN_SDK_REQUIRES_RANGE(subMixBus, 0, OutputReceiver::BusCountMax);
        if( IsAttachedSound() )
        {
            m_pSound->SetOutputBusMixVolume( OutputDevice_Main, sourceChannelIndex, subMixBus, mixVolume );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief 波形の全チャンネルに、サウンドの5.1chの各スピーカーの音量を設定します。
    //!
    //!         ハンドルに関連付けられたサウンドの音量を変更します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         mixVolume には5.1chの各スピーカーの音量の値を設定します。
    //!         全てのチャンネルの音量を指定する必要があります。
    //!
    //!         @ref SetMixMode で @ref MixMode_Pan が指定されている場合は
    //!         この値を指定しても無視されます。
    //!
    //! @param[in]    mixVolume          5.1chの各スピーカーの音量です。
    //!
    //! @see MixVolume
    //! @see SetMixMode
    //---------------------------------------------------------------------------
    void SetMixVolume( const MixVolume& mixVolume ) NN_NOEXCEPT
    {
        MixParameter parameter;
        for ( auto i = 0; i < ChannelIndex_Count; ++i )
        {
            parameter.ch[i] = mixVolume.channel[i];
        }
        SetChannelMixParameter( parameter );
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  波形の全チャンネルに、サウンドの各チャンネルに分配する音量を設定します。
    //!         バスごとに、 ChannelMixVolume::ChannelCountMax までのチャンネルの設定を行うことができます。
    //!
    //!         ハンドルに関連付けられたサウンドの音量を変更します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         mixVolume には各チャンネルに分配する音量の値を設定します。
    //!         全てのチャンネルの音量を指定する必要があります。
    //!
    //!         @ref SetBusMixVolumeEnabled で有効化されたバスのみ、設定した音量が適用されます。
    //!         また、SoundSystem の初期化時、 enableBusMixVolume を true にして初期化しないと利用できない点にご注意ください。
    //!
    //! @param[in]    subMixBus          mixVolume を適用するバスです。
    //! @param[in]    mixVolume          各チャンネルに分配する音量です。
    //! @pre
    //!        - nn::atk::SoundSystem の初期化時に enableBusMixVolume を true にして初期化する
    //!        - 0 <= subMixBus < (SoundHandle に紐づく Sound が持つバス数)
    //!            - カスタムサブミックス機能を有効にしない場合、 (SoundHandle に紐づく Sound が持つバス数)  は nn::atk::AuxBus_Count + 1 となります。
    //!            - カスタムサブミックス機能を有効にした場合、 (SoundHandle に紐づく Sound が持つバス数) は @ref nn::atk::SoundArchivePlayer::StartSound などで指定した @ref nn::atk::OutputReceiver の、
    //!              @ref nn::atk::OutputReceiver::GetBusCount() で取得できる値となります。
    //!
    //! @see MixVolume
    //! @see SetBusMixVolumeEnabled
    //---------------------------------------------------------------------------
    void SetBusMixVolume( int subMixBus, const ChannelMixVolume& mixVolume ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(subMixBus, 0, OutputReceiver::BusCountMax);
        if( IsAttachedSound() )
        {
            for (int sourceChannelIndex = 0; sourceChannelIndex < WaveChannelMax; sourceChannelIndex++)
            {
                m_pSound->SetOutputBusMixVolume( OutputDevice_Main, sourceChannelIndex, subMixBus, mixVolume );
            }
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドのメインセンド量を変更します。
    //!
    //!         ハンドルに関連付けられたサウンドのメインセンド量を変更します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         この関数で指定する値は、他のどのメインセンド量とも独立して動作し、
    //!         それらはすべて重ね合わされます。
    //!
    //!         メインセンドは、
    //!         出力に送るサウンドの音量をエフェクトセンドの後で調節するパラメータです。
    //!         主に、エフェクトのドライ・ウェット成分のバランスを調整するために使用されます。
    //!
    //!         センド量 send は相対変化の値を指定し、最終的なセンド量は、
    //!         各 API や SoundMaker で指定した値の合計値を 0.0 ～ 1.0 の範囲でクランプしたものとなります。
    //!         また、 SoundMaker で設定できる値は、ランタイム側で 0.0 ～ 1.0 の値の範囲として解釈されます。
    //!
    //!         すなわち、 0.0 のセンド量の指定は各 API や SoundMaker で指定したセンド量を変更しない設定を表します。
    //!         1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスに最大のセンド量が送られるようなセンド量の加算を表します。
    //!         -1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスにセンド量が送られないようなセンド量の減算を表します。
    //!
    //!         デフォルト値は 0.0 です。
    //!
    //!         出力先別に設定したい場合は、@ref SetOutputMainSend を利用してください。
    //!
    //! @param[in] send   0.0 を基準としたセンド量の相対変化の値です。
    //!
    //! @see SetEffectSend
    //! @see SetOutputMainSend
    //---------------------------------------------------------------------------
    void SetMainSend( float send ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetMainSend( send );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドのエフェクトセンド量を変更します。
    //!
    //!         ハンドルに関連付けられたサウンドのエフェクトセンド量を変更します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         この関数で指定する値は、他のどのエフェクトセンド量とも独立して動作し、
    //!         それらはすべて重ね合わされます。
    //!
    //!         センド量 send は相対変化の値を指定し、最終的なセンド量は、
    //!         各 API や SoundMaker で指定した値の合計値を 0.0 ～ 1.0 の範囲でクランプしたものとなります。
    //!         また、 SoundMaker で設定できる値は、ランタイム側で 0.0 ～ 1.0 の値の範囲として解釈されます。
    //!
    //!         すなわち、 0.0 のセンド量の指定は各 API や SoundMaker で指定したセンド量を変更しない設定を表します。
    //!         1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスに最大のセンド量が送られるようなセンド量の加算を表します。
    //!         -1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスにセンド量が送られないようなセンド量の減算を表します。
    //!
    //!         デフォルト値は 0.0 です。
    //!
    //!         出力先別に設定したい場合は、@ref SetOutputEffectSend を利用してください。
    //!
    //! @param[in] bus    センド量を設定する AUX のバスです。
    //! @param[in] send   0.0 を基準としたセンド量の相対変化の値です。
    //!
    //! @see AuxBus
    //! @see SetMainSend
    //! @see SetOutputEffectSend
    //---------------------------------------------------------------------------
    void SetEffectSend( AuxBus bus, float send ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetFxSend( bus, send );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドのバスごとに設定するボリュームを変更します。
    //!
    //!         ハンドルに関連付けられたサウンドの、指定したバスの音量を変更します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         この関数で指定する値は、他のどのエフェクトセンド量とも独立して動作し、
    //!         それらはすべて重ね合わされます。
    //!
    //!         センド量 send は相対変化の値を指定し、最終的なセンド量は、
    //!         各 API や SoundMaker で指定した値の合計値を 0.0 ～ 1.0 の範囲でクランプしたものとなります。
    //!         また、 SoundMaker で設定できる値は、ランタイム側で 0.0 ～ 1.0 の値の範囲として解釈されます。
    //!
    //!         すなわち、 0.0 のセンド量の指定は各 API や SoundMaker で指定したセンド量を変更しない設定を表します。
    //!         1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスに最大のセンド量が送られるようなセンド量の加算を表します。
    //!         -1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスにセンド量が送られないようなセンド量の減算を表します。
    //!
    //!         SoundMaker で指定する、バスに対するセンド量の設定との対応関係については、
    //!         センドMAIN がバス 0 のセンド量, センドAUX_A がバス 1 のセンド量, センドAUX_B がバス 2 のセンド量, センドAUX_C がバス 3 のセンド量に対応します。
    //!         デフォルト値は 0.0 です。
    //!
    //! @param[in] subMixBus send を適用するバスです
    //! @param[in] send      センド量の相対変化の値です。
    //!
    //! @pre
    //!        - (カスタムサブミックス機能を有効にしない場合)
    //!            - 0 <= subMixBus < nn::atk::AuxBus_Count + 1
    //!        - (カスタムサブミックス機能を有効にした場合)
    //!            - 0 <= subMixBus < nn::atk::AuxBus_Count + 1 || 0 <= subMixBus < (SoundHandle に紐づく Sound が持つバス数)
    //!                - (SoundHandle に紐づく Sound が持つバス数) は @ref nn::atk::SoundArchivePlayer::StartSound などで指定した @ref nn::atk::OutputReceiver の、
    //!                  @ref nn::atk::OutputReceiver::GetBusCount() で取得できる値となります。
    //---------------------------------------------------------------------------
    void SetSend( int subMixBus, float send ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetSend( subMixBus, send );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    サウンドの音程を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドの音程を変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどの音程パラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           音程 pitch は、周波数の比率で指定します。
    //!           すなわち、1.0 を指定すると音程に影響を与えません。
    //!           2.0 を指定すると再生される周波数が 2 倍になり、
    //!           1 オクターブ高い音程になります。
    //!           0.5 を指定すると 1 オクターブ低い音程になります。
    //!
    //!           デフォルト値は 1.0 です。
    //!
    //!           他の音程パラメータと重ね合わされたあと、
    //!           最終的な音程は nn::audio::VoiceType::GetPitchMin() ～ nn::audio::VoiceType::GetPitchMax() の範囲でクランプされます。
    //!           音程の重ね合わせの詳細につきましては @confluencelink{83955829, サウンドパラメータ制御の動作} を参照ください。
    //!
    //! @param[in]    pitch  変更する音程の周波数比率です。
    //! @pre          pitch >= 0.0f
    //!
    //---------------------------------------------------------------------------
    void SetPitch( float pitch ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetPitch( pitch );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    サウンドのローパスフィルタカットオフ値を変更します。
    //!
    //!           ハンドルに関連付けられたローパスフィルタカットオフ値を変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、
    //!           他のどのローパスフィルタカットオフパラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           lowPassFilterFrequency は、カットオフの相対変化の値を指定します。
    //!           0.0 を指定するとカットオフの値を変更しません。
    //!           -1.0 を指定すると、フィルタがかかっていない状態から、
    //!           もっともフィルタがかかっている状態 (カットオフ周波数が下がる方向)
    //!           に変更します。
    //!
    //!           SetBiquadFilter で与えられる Biquad フィルタと併用して使用した場合重ねがけされます。
    //!
    //! @param[in] lowPassFilterFrequency    0.0 を基準としたフィルタカットオフの相対変化の値です。
    //!
    //---------------------------------------------------------------------------
    void SetLowPassFilterFrequency( float lowPassFilterFrequency ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetLpfFreq( lowPassFilterFrequency );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドの Biquad フィルタの設定を変更します。
    //!
    //!         ハンドルに関連付けられたサウンドの Biquad フィルタの設定を変更します。
    //!         ハンドルが無効の場合は、何もしません。
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
    //!         type は BiquadFilterType_Min ～ BiquadFilterType_Max の値を指定します。
    //!         BiquadFilterType_Min、BiquadFilterType_Max については @ref BiquadFilterType をご参照ください。
    //!         範囲外の値を設定すると、Debug 版 / Development 版ではアサートで停止します。
    //!         Release 版では無視されます。
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
    //! @pre       0.0f <= value <= 1.0f
    //! @see BiquadFilterType
    //!
    //---------------------------------------------------------------------------
    void SetBiquadFilter( int type, float value ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( type >= BiquadFilterType_Min && type <= BiquadFilterType_Max );
        if ( IsAttachedSound() )
        {
            m_pSound->SetBiquadFilter( type, value );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    プレイヤープライオリティを変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのプレイヤープライオリティを変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数は、サウンドアーカイブ中のデータで指定されている
    //!           プレイヤープライオリティの値を変更します。
    //!
    //!           priority の値の範囲は @ref nn::atk::PlayerPriorityMin ～ @ref nn::atk::PlayerPriorityMax で、大きいほど優先度が高くなります。
    //!
    //! @param[in] priority   プレイヤープライオリティの値です。
    //!
    //---------------------------------------------------------------------------
    void SetPlayerPriority( int priority ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetPlayerPriority( priority );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドの出力先を設定します。
    //!
    //!         ハンドルに関連付けられたサウンドの出力先を設定します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         出力先は @ref OutputLine のビットフラグの論理和で指定します。
    //!         指定した値は、それまでに設定されていた値を上書きします。
    //!         一度指定した値を無効にする場合は @ref ResetOutputLine を呼び出してください。
    //!
    //! @param[in] lineFlag     出力先のビットフラグです。
    //!
    //! @see OutputLine
    //! @see ResetOutputLine
    //!
    //---------------------------------------------------------------------------
    void SetOutputLine( uint32_t lineFlag ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetOutputLine( lineFlag );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  サウンドの出力先の設定を無効にします。
    //!
    //!         ハンドルに関連付けられたサウンドの出力先の設定を無効にします。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         この関数は @ref SetOutputLine で設定された出力先を無効にして、
    //!         デフォルトの出力先へ戻します。デフォルトの出力先は、
    //!         @ref SoundPlayer::SetDefaultOutputLine で設定された値です。
    //!
    //! @see SetOutputLine
    //! @see SoundPlayer::SetDefaultOutputLine
    //!
    //---------------------------------------------------------------------------
    void ResetOutputLine() NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->ResetOutputLine();
        }
    }
    //@}

    //----------------------------------------
    //! @name 出力先別パラメータ
    //@{

    //---------------------------------------------------------------------------
    //! @brief  出力先別の音量を設定します。
    //!
    //!         @ref SetVolume などで設定された値との積が、当該出力先の音量になります。
    //!
    //! @param[in] device   出力先です。
    //! @param[in] volume   音量です。
    //!
    //! @see SetVolume
    // --------------------------------------------------------------------------
    void SetOutputVolume(OutputDevice device, float volume) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetOutputVolume(device, volume);
        }
    }

    // --------------------------------------------------------------------------
    //! @brief  出力先別のパンを設定します。
    //!
    //!         @ref SetPan で設定された値との和が、当該出力先のパンになります。
    //!
    //!         @ref SetMixMode で @ref MixMode_MixVolume が指定されている場合は
    //!         この値を指定しても無視されます。
    //!
    //! @param[in] device   出力先です。
    //! @param[in] pan      パンです。
    //!
    //! @see SetPan
    // --------------------------------------------------------------------------
    void SetOutputPan(OutputDevice device, float pan) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetOutputPan(device, pan);
        }
    }

    // --------------------------------------------------------------------------
    //! @brief  出力先別のサラウンドパンを設定します。
    //!
    //!         @ref SetSurroundPan で設定された値との和が、
    //!         当該出力先のサラウンドパンになります。
    //!
    //!         @ref SetMixMode で @ref MixMode_MixVolume が指定されている場合は
    //!         この値を指定しても無視されます。
    //!
    //! @param[in] device   出力先です。
    //! @param[in] surroundPan     サラウンドパンです。
    //!
    //! @see SetSurroundPan
    // --------------------------------------------------------------------------
    void SetOutputSurroundPan(OutputDevice device, float surroundPan) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetOutputSurroundPan(device, surroundPan);
        }
    }

    // --------------------------------------------------------------------------
    //! @brief  出力先別のメインセンドを設定します。
    //!
    //!         @ref SetMainSend で設定された値との和が、
    //!         当該出力先のメインセンド量になります。
    //!
    //! @param[in] device   出力先です。
    //! @param[in] send     センド量です。
    //!
    //! @see SetMainSend
    // --------------------------------------------------------------------------
    void SetOutputMainSend(OutputDevice device, float send) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetOutputMainSend(device, send);
        }
    }

    // --------------------------------------------------------------------------
    //! @brief  出力先別のエフェクトセンドを設定します。
    //!
    //!         @ref SetEffectSend で設定された値との和が、
    //!         当該出力先のエフェクトセンド量になります。
    //!
    //! @param[in] device   出力先です。
    //! @param[in] bus      AUX バスです。
    //! @param[in] send     センド量です。
    //!
    //! @see SetEffectSend
    // --------------------------------------------------------------------------
    void SetOutputEffectSend(OutputDevice device, AuxBus bus, float send) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetOutputFxSend(device, bus, send);
        }
    }
    //@}

    //----------------------------------------
    //! @name ハンドル操作
    //@{
    //---------------------------------------------------------------------------
    //! @brief    ハンドルにサウンドが関連付けられているかどうかを調べます。
    //!
    //! @return   ハンドルに関連付けられているサウンドがあれば true を返します。
    //!
    //---------------------------------------------------------------------------
    bool IsAttachedSound() const NN_NOEXCEPT
    {
        if ( m_pSound != NULL )
        {
            return true;
        }
        return false;
    }

    // サウンドハンドルからサウンドを解放
    //---------------------------------------------------------------------------
    //! @brief    ハンドルからサウンドを解放します。
    //!
    //!           ハンドルから解放されたサウンドは、
    //!           その後ハンドルを通して操作できないようになります。
    //---------------------------------------------------------------------------
    void DetachSound() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    サウンドの ID を設定します。
    //!
    //!           ハンドルに関連付けられたサウンドの ID を設定します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           @ref SoundArchivePlayer でサウンドを再生すると、
    //!           再生開始時にサウンド ID が自動的に設定されます。
    //!           この関数を呼び出すと、 ID を上書きして変更します。
    //!
    //!           設定した ID を取得するためには @ref GetId を呼び出します。
    //!
    //! @param[in]    id      サウンドを識別する ID です。
    //!
    //! @see      @ref SoundArchivePlayer, @ref GetId
    //!
    //---------------------------------------------------------------------------
    void SetId( uint32_t id ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetId( id );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    サウンドに設定されている ID を取得します。
    //!
    //!           ハンドルに関連付けられたサウンドに設定されている ID を取得します。
    //!           ハンドルが無効の場合は @ref nn::atk::InvalidSoundId を返します。
    //!
    //!           この関数で取得できる ID は @ref SetId で設定された ID です。
    //!
    //! @return   SetId
    //!
    //---------------------------------------------------------------------------
    uint32_t GetId() const NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            return m_pSound->GetId();
        }
        return nn::atk::InvalidSoundId;
    }

    //---------------------------------------------------------------------------
    //! @brief    アンビエントパラメータの値を取得します。
    //!
    //!           3D サウンドを利用している際は、
    //!           3D サウンドで計算された結果がアンビエントパラメータに格納されますので、
    //!           アンビエントパラメータを取得して 3D
    //!           サウンドによるサウンドパラメータの値を知ることができます。
    //!
    //! @return   現在のアンビエントパラメータの値を返します。
    //!           ハンドルが無効の場合は NULL を返します。
    //!
    //! @see      nn::atk::SoundParam 構造体
    //!
    //---------------------------------------------------------------------------
    const SoundParam* GetAmbientParam() const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() )
        {
            return NULL;
        }
        return &m_pSound->GetAmbientParam();
    }

    //---------------------------------------------------------------------------
    //! @brief  ユーザーパラメータを取得します。
    //!
    //!         各サウンドインスタンスにユーザーパラメータ
    //!         (ユーザーが自由に読み書きできる領域) を持たせることができます。
    //!
    //!         SoundArchivePlayer::Initialize の userParamSizePerSound に 1
    //!         以上の値を渡していると、ユーザーパラメータを void* 型で取得できます。
    //!         0 を渡すと NULL が返ります。
    //!         つまり、0 を渡した場合はユーザーパラメータは利用できません。
    //!
    //!         取得した値は、アプリケーションプログラム側で、
    //!         適当な型にキャストするなどして利用してください。
    //!
    //!         NULL 以外の値が取得できた場合、そのアドレスには、
    //!         @ref SoundArchivePlayer::GetSoundUserParamSize
    //!         バイトのデータが保持されています。
    //!
    //!         ユーザーパラメータはサウンド再生開始時にゼロクリアされます。
    //!
    //! @return ユーザーパラメータを返します。
    //!
    //! @see SoundArchivePlayer::Initialize
    //! @see SoundArchivePlayer::GetUserParam
    //---------------------------------------------------------------------------
    void* GetUserParam() const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() )
        {
            return NULL;
        }
        return m_pSound->GetUserParam();
    }
    //@}

    //! @briefprivate
    //! @brief      サウンドパラメータを計算する値を取得します
    //! @param[out] pOutValue 値が格納される構造体のポインタです
    //! @return     値が pOutValue に格納されたときは true を返します
    bool CalculateSoundParamCalculationValues(SoundParamCalculationValues* pOutValue) const NN_NOEXCEPT;

    // -----------------------------------------------------------------
    // 非公開関数

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  波形のチャンネル別に、サウンドのサラウンドボリューム(5.1chの各スピーカーの音量)を変更します。
    //!
    //!         ハンドルに関連付けられたサウンドのサラウンドボリュームを変更します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         param は、サラウンドボリュームの値を設定します。
    //!         全てのチャンネルのサラウンドボリュームを指定する必要があります。
    //!
    //!         デフォルト値は fL, fR, rL, rR, fC, lfe ともに 1.0f です。
    //!
    //!         ミックスモードにパンが指定されている場合は
    //!         この値を指定しても無視されます。
    //!
    //!         この関数でサラウンドボリュームを指定すると、全体の出力に適用されます。
    //!         出力先別に設定したい場合は、@ref SetOutputChannelMixParameter を利用してください。
    //!
    //! @param[in]    srcChNo サラウンドボリュームを指定するチャンネルです。
    //! @param[in]    param   サラウンドボリュームです。
    //!
    //! @see SetOutputChannelMixParameter
    //---------------------------------------------------------------------------
    void SetChannelMixParameter( uint32_t srcChNo, const MixParameter& param ) NN_NOEXCEPT
    {
        if( IsAttachedSound() )
        {
            m_pSound->SetOutputChannelMixParameter( OutputDevice_Main, srcChNo, param );
        }
    }
    //! @briefprivate
    //! @brief    波形の全チャンネルに、同一のサラウンドボリュームを設定します。
    //! @param param :private
    void SetChannelMixParameter( const MixParameter& param ) NN_NOEXCEPT
    {
        if( IsAttachedSound() )
        {
            // 全チャンネルで同じ設定を使用
            for ( uint32_t srcChNo = 0; srcChNo < WaveChannelMax; srcChNo++ )
            {
                m_pSound->SetOutputChannelMixParameter( OutputDevice_Main, srcChNo, param );
            }
        }
    }
    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief  波形のチャンネル別に、サウンドのサラウンドボリューム(5.1chの各スピーカーの音量)を出力先別で変更します。
    //!
    //!         ハンドルに関連付けられたサウンドの各出力に対するサラウンドボリュームを変更します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         param は、サラウンドボリュームの値を設定します。
    //!         全てのチャンネルのサラウンドボリュームを指定する必要があります。
    //!
    //!         デフォルト値は fL, fR, rL, rR, fC, lfe ともに 1.0f です。
    //!
    //!         ミックスモードにパンが指定されている場合は
    //!         この値を指定しても無視されます。
    //!
    //!         全体の出力にサラウンドボリュームを設定したい場合、@ref SetChannelMixParameter を利用してください。
    //!
    //! @param[in] device  出力先です。
    //! @param[in] srcChNo サラウンドボリュームを指定するチャンネルです。
    //! @param[in] param   サラウンドボリュームです。
    //!
    //! @see SetChannelMixParameter
    //---------------------------------------------------------------------------
    void SetOutputChannelMixParameter( OutputDevice device, uint32_t srcChNo, const MixParameter& param ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetOutputChannelMixParameter( device, srcChNo, param );
        }
    }
    //! @briefprivate
    //! @brief    出力先指定で、波形の全チャンネルに同一のサラウンドボリュームを設定します。
    //! @param device :private
    //! @param param :private
    void SetOutputChannelMixParameter( OutputDevice device, const MixParameter& param ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            // 全チャンネルで同じ設定を使用
            for ( uint32_t srcChNo = 0; srcChNo < WaveChannelMax; srcChNo++)
            {
                m_pSound->SetOutputChannelMixParameter( device, srcChNo, param );
            }
        }
    }

    // ハンドルをサウンドに関連付ける
    //! @internal
    //! @param sound :private
    void detail_AttachSound( detail::BasicSound* sound ) NN_NOEXCEPT;
    //! @internal
    //! @param sound :private
    void detail_AttachSoundAsTempHandle( detail::BasicSound* sound ) NN_NOEXCEPT;

    //! @internal
    //! @return :private
    detail::BasicSound* detail_GetAttachedSound() NN_NOEXCEPT
    {
        return m_pSound;
    }

    //! @internal
    //! @return :private
    const detail::BasicSound* detail_GetAttachedSound() const NN_NOEXCEPT
    {
        return m_pSound;
    }

    // 一時ハンドルを生成する
    //! @internal
    //! @param handle :private
    void detail_DuplicateHandle( SoundHandle* handle ) NN_NOEXCEPT;

private:
    NN_DISALLOW_COPY( SoundHandle );

    detail::BasicSound* m_pSound;
};

} // namespace nn::atk
} // namespace nn

