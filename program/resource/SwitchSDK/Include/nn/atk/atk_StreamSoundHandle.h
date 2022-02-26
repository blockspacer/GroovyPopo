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
 * :include nn/atk/atk_StreamSoundHandle.h
 *
 * @file atk_StreamSoundHandle.h
 */

#pragma once

#include <nn/atk/atk_StreamSound.h>
#include <nn/atk/atk_ChannelMixVolume.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @brief    再生したストリームサウンドの操作を行うためのハンドルクラスです。
//!
//!           サウンドは再生時に汎用サウンドハンドル
//!           @ref SoundHandle と関連付けられますが、
//!           ストリームサウンドに固有の処理は汎用サウンドハンドルには実装されていません。
//!
//!           ストリームサウンドに固有の処理を行いたい場合は、
//!           汎用サウンドハンドルを引数にとるコンストラクタを用いて、
//!           ストリームサウンドハンドルを生成し、使用してください。
//!
//!           ハンドルの使い方は、汎用サウンドハンドル @ref SoundHandle と同じです。
//!
//! @see      SoundHandle クラス
//!
//---------------------------------------------------------------------------
class StreamSoundHandle
{
public:
    //! @brief    ストリームサウンドのトラックを表すビットフラッグを扱う型です。
    typedef nn::util::BitFlagSet<SoundArchive::StreamTrackCount> TrackBitFlagSet;

public:
    //! @name コンストラクタ/デストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //!           引数の無いコンストラクタは、
    //!           どのサウンドとも関連付けられていないハンドルを構築します。
    //!
    //---------------------------------------------------------------------------
    StreamSoundHandle() NN_NOEXCEPT : m_pSound( NULL ) { }

    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //!           引数付きコンストラクタは、
    //!           引数 handle で指定したサウンドハンドルが参照しているサウンドを用いて、
    //!           ストリームサウンドハンドルを構築します。
    //!           ストリームサウンド以外のサウンドがサウンドハンドルに関連付けられているときは、
    //!           引数の無いコンストラクタと同様に構築します。
    //!
    //!           引数付きコンストラクタで構築した時、
    //!           サウンドが既に別のストリームサウンドハンドルと関連付けられている場合は、
    //!           その関連付けが切られる点に注意してください。
    //!           ただし、引数に渡したサウンドハンドルとの関連付けは切れません。
    //!
    //! @param[in] pHandle ストリームサウンドと関連付けられたサウンドハンドル。
    //!
    //! @see      SoundHandle クラス
    //!
    //---------------------------------------------------------------------------
    explicit StreamSoundHandle( SoundHandle* pHandle ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    デストラクタです。
    //!
    //---------------------------------------------------------------------------
    ~StreamSoundHandle() NN_NOEXCEPT { DetachSound(); }

    //@}


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
    //!           再生準備が完了していないサウンドは、完了するまで待ってから再生を開始します。
    //!
    //! @see nn::atk::SoundArchivePlayer::PrepareSound
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
    //!           fadeFrames で指定したフレーム数をかけてフェードアウトさせることができます。
    //!           0 を指定した場合はフェードアウトを行わず、
    //!           エンベロープのリリースを発音しすべての減衰が完了した後にサウンドが停止します。
    //!
    //!           フェードアウトの音量制御は、フェードインと共有されます。
    //!           フェードアウトにかかるフレーム数は、
    //!           最大音量から音が消えるまでにかかる変化速度を表しますので、
    //!           フェードイン中にフェードアウトを指定した時などは、
    //!           指定したフレーム数よりも短い時間でフェードアウトが完了する可能性があります。
    //!
    //! @param[in] fadeFrames フェードアウトにかけるフレーム数です。
    //!                       フレーム数は @ref SoundArchivePlayer::Update
    //!                       の呼び出し回数で換算されます。
    //!                       fadeFrames には 0 以上の値を指定してください。
    //!                       負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //---------------------------------------------------------------------------
    void Stop( int fadeFrames ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
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
    //!           fadeFrames に 0 を指定した場合、
    //!           一時停止状態に変更する際には、 1 回目の @ref SoundArchivePlayer::Update 後に音量だけがフェードアウトなしに 0 になり、
    //!           2 回目の @ref SoundArchivePlayer::Update 後に一時停止状態となります。
    //!           このとき、 @ref SoundArchivePlayer::Update の 1 回目と 2 回目の呼び出し間隔分だけ、
    //!           無音状態で再生時間が進む点にご注意ください。
    //!           一方、再開時には 1 回目の @ref SoundArchivePlayer::Update 後にフェードインなしに再開するため注意する必要はありません。
    //!
    //!           一時停止・再開時のフェードは、再生開始時のフェードイン、
    //!           停止時のフェードアウトとは独立してはたらきます。
    //!
    //!           また、フェードにかかるフレーム数は、最大音量から音が消えるまで、
    //!           あるいは発音していない状態から最大音量に達するまでにかかる変化速度を表しますので、
    //!           フェードイン中にさらにフェードアウトを指定した時などは、
    //!           指定したフレーム数よりも短い時間でフェードが完了する可能性があります。
    //!
    //! @param[in]    flag        true なら一時停止、false なら再開します。
    //! @param[in]    fadeFrames  フェードイン・フェードアウトにかけるフレーム数です。
    //!                           フレーム数は @ref SoundArchivePlayer::Update
    //!                           の呼び出し回数で換算されます。
    //!                           負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //!
    //! @see  IsPause
    //---------------------------------------------------------------------------
    void Pause( bool flag, int fadeFrames ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->Pause( flag, fadeFrames );
        }
    }

    //---------------------------------------------------------------------------
    //! @briefprivate     サウンドを一時停止または再開します。
    //!
    //!                   ハンドルに関連付けられたサウンドを一時停止または再開します。
    //!                   ハンドルが無効の場合は、何もしません。
    //!
    //!                   fadeFrames で指定したフレーム数をかけてフェードアウトしながら一時停止、
    //!                   またはフェードインしながら再開させることができます。
    //!
    //!                   一時停止・再開時のフェードは、再生開始時のフェードイン、
    //!                   停止時のフェードアウトとは独立してはたらきます。
    //!                   フェードにかかるフレーム数は、最大音量から音が消えるまで、
    //!                   あるいは発音していない状態から最大音量に達するまでに
    //!                   かかる変化速度を表しますので、
    //!                   フェード中にさらにフェードを指定した時などは、
    //!                   指定したフレーム数よりも短い時間でフェードが完了する可能性があります。
    //!
    //! @param[in]    flag           true なら一時停止、false なら再開します。
    //! @param[in]    fadeFrames     フェードイン・フェードアウトにかけるフレーム数です。
    //!                              フレーム数は @ref SoundArchivePlayer::Update
    //!                              の呼び出し回数で換算されます。
    //!                              負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //! @param[in]    pauseMode      ポーズの設定です。
    //!
    //! @see IsPause
    //---------------------------------------------------------------------------
    void Pause(bool flag, int fadeFrames, PauseMode pauseMode) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->Pause(flag, fadeFrames, pauseMode);
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    サウンドの再生準備が完了しているかどうかを調べます。
    //!
    //!           @ref SoundArchivePlayer::PrepareSound を呼び出した後、
    //!           そのサウンドの再生準備が完了しているかどうかを調べます。
    //!           再生準備が完了したサウンドは、@ref StartPrepared を呼び出した際に、
    //!           すぐに再生を始めることができます。
    //!
    //! @return   サウンドの再生準備が完了していれば true を返します。
    //!
    //! @see  SoundArchivePlayer::PrepareSound
    //! @see  StartPrepared
    //!
    //---------------------------------------------------------------------------
    bool IsPrepared() const NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            return m_pSound->IsPrepared();
        }
        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief    サウンドが一時停止中かどうかを調べます。
    //!
    //! @return   サウンドが一時停止状態であれば true を返します。
    //!
    //! @see Pause
    //!
    //---------------------------------------------------------------------------
    bool IsPause() const NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            return m_pSound->IsPause();
        }
        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief    ストリームがロードの遅延によって停止しているかどうかを調べます。
    //!
    //! @return   サウンドがロードの遅延によって停止していれば true を返します。
    //!
    //! @see GetFilledBufferPercentage
    //!
    //---------------------------------------------------------------------------
    bool IsSuspendByLoadingDelay() const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() )
        {
            return false;
        }
        return m_pSound->IsSuspendByLoadingDelay();
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    ストリームのロード遅延が発生しているかどうかを調べます。
    //!
    //! @return   ロード遅延が発生していれば true を返します。
    //---------------------------------------------------------------------------
    bool IsLoadingDelayState() const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() )
        {
            return false;
        }
        return m_pSound->IsLoadingDelayState();
    }

    //---------------------------------------------------------------------------
    //! @brief    サウンドを再生開始時にフェードインさせます。
    //!
    //!           この関数でフェードインの指定を行うと、
    //!           @ref SoundArchivePlayer::StartSound または
    //!           @ref StartPrepared を呼び出して再生を開始してから、
    //!           最初の @ref SoundArchivePlayer::Update
    //!           が呼び出されたときにフェードインが設定されます。
    //!
    //!           サウンドの再生が開始され、最初の @ref SoundArchivePlayer::Update
    //!           が呼ばれた後に、この関数を呼び出しても効果がありません。
    //!
    //!           フェードインの音量制御は、停止時のフェードアウトと共有されます。
    //!
    //! @param[in] frames フェードインにかけるフレーム数です。
    //!                   フレーム数は @ref SoundArchivePlayer::Update
    //!                   の呼び出し回数で換算されます。
    //!                   frames には 0 以上の値を指定してください。
    //!                   負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //!
    //! @see  SoundArchivePlayer::StartSound
    //! @see  StartPrepared
    //---------------------------------------------------------------------------
    void FadeIn( int frames ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->FadeIn( frames );
        }
    }

    //@}


    //! @name パラメータ変更
    //@{

    //---------------------------------------------------------------------------
    //! @brief    サウンドの音量を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドの音量を変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどの音量パラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           音量 volume は、0.0 以上の倍率で指定します。
    //!           すなわち、1.0 を指定すると音量に影響を与えません。
    //!           0.0 を指定すると発音されなくなります。デフォルト値は 1.0 です。
    //!
    //!           他の音量パラメータと重ね合わされたあと、
    //!           最終的な音量は nn::audio::VoiceType::GetVolumeMin() ～ nn::audio::VoiceType::GetVolumeMax() の範囲でクランプされます。
    //!           音量の重ね合わせの詳細につきましては @confluencelink{83955829, サウンドパラメータ制御の動作} を参照ください。
    //!
    //!           音量の変化は frames で指定したフレーム数をかけて行われます。
    //!           音量の変化途中にさらにこの関数を呼び出した場合は、
    //!           その時点での変化途中の音量値を基点として、
    //!           新しく指定したフレーム数をかけて音量を変化させます。
    //!
    //! @param[in] volume 変更する音量の倍率 (0.0～) です。
    //! @param[in] frames 音量変化にかけるフレーム数です。
    //!                   フレーム数は @ref SoundArchivePlayer::Update
    //!                   の呼び出し回数で換算されます。
    //!                   frames には 0 以上の値を指定してください。
    //!                   負値を指定した場合は、0 を指定した場合と同じ挙動になります。
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
    //!           デフォルト値は 1.0 です。
    //!
    //!           他の音程パラメータと重ね合わされたあと、
    //!           最終的な音程は nn::audio::VoiceType::GetPitchMin() ～ nn::audio::VoiceType::GetPitchMax() の範囲でクランプされます。
    //!           音程の重ね合わせの詳細につきましては @confluencelink{83955829, サウンドパラメータ制御の動作} を参照ください。
    //!
    //! @param[in] pitch  変更する音程の周波数比率です。
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
    //@}

    //----------------------------------------
    //! @name メイン (TV) 出力用パラメータ
    //@{

    //---------------------------------------------------------------------------
    //! @brief    メイン (TV) 出力に対するパン (左右の定位) を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのメイン出力に対するパンを変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどのパンパラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           pan は、定位の相対変化の値を設定します。
    //!           0.0 を指定するとデータで設定されたパンの値から変化しません。
    //!           1.0 を指定すると中央に定位していた音が右端に定位するようになり、
    //!           -1.0 を指定すると中央に定位していた音が左端に定位するようになります。
    //!           デフォルト値は 0.0 です。
    //!
    //!           @ref SetMixMode で @ref MixMode_MixVolume が指定されている場合は
    //!           この値を指定しても無視されます。
    //!
    //! @param[in] pan    0.0 を基準としたパンの相対変化の値です。
    //!
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
    //! @brief    メイン (TV) 出力に対するサラウンドパン (前後の定位) を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのメイン出力に対するサラウンドパンを変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどのサラウンドパンパラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           surroundPan は、定位の相対変化の値を設定します。
    //!           0.0 を指定するとデータで設定されたパンの値から変化しません。
    //!           1.0 を指定すると最前方に定位していた音が中央に定位するようになり、
    //!           2.0 を指定すると最前方に定位していた音が最後方に定位するようになります。
    //!           前方へ定位を移動させたい場合は負の値を指定してください。
    //!           デフォルト値は 0.0 です。
    //!
    //!           @ref SetMixMode で @ref MixMode_MixVolume が指定されている場合は
    //!           この値を指定しても無視されます。
    //!
    //!           サラウンドパンに設定した値は、出力モードがサラウンドの場合だけ反映されます。
    //!           詳しくは @ref nn::atk::SoundSystem::SetOutputMode を参照ください。
    //!
    //! @param[in] surroundPan    0.0 を基準としたサラウンドパンの相対変化の値です。
    //!
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
    //!         @ref MixMode_MixVolume を指定した場合は、、
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
    //! @see SetTrackMixVolume
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
    //! @see SetTrackMixVolume
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
    //! @brief    波形のチャンネル別に、サウンドの5.1chの各スピーカーの音量をトラック毎に変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのストリームトラックの音量を変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!
    //!           mixVolume には5.1chの各スピーカーの音量の値を設定します。
    //!           全てのチャンネルの音量を指定する必要があります。
    //!
    //!           @ref SetMixMode で @ref MixMode_Pan が指定されている場合は
    //!           この値を指定しても無視されます。
    //!
    //!           @ref SetMixVolume で設定した値はこれとは別に扱われ、
    //!           最終的なサラウンドボリュームは、掛け合わせた結果になります。
    //!
    //! @param[in] trackBitFlag       設定を行うトラックを表すビットフラグです。
    //! @param[in] sourceChannelIndex mixVolume を適用する波形のチャンネル番号です。
    //! @param[in] mixVolume          5.1chの各スピーカーの音量です。
    //! @pre       0 <= sourceChannelIndex < WaveChannelMax
    //!
    //! @see MixVolume
    //! @see SetMixVolume
    //! @see SetMixMode
    //---------------------------------------------------------------------------
    void SetTrackMixVolume( TrackBitFlagSet trackBitFlag, int sourceChannelIndex, const MixVolume& mixVolume ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(sourceChannelIndex, 0, static_cast<int>(WaveChannelMax));
        MixParameter parameter;
        for ( auto i = 0; i < ChannelIndex_Count; ++i )
        {
            parameter.ch[i] = mixVolume.channel[i];
        }
        uint32_t bitFlag = 0;
        for ( auto trackIndex = 0; trackIndex < SoundArchive::StreamTrackCount; ++trackIndex )
        {
            if ( trackBitFlag[trackIndex] )
            {
                bitFlag += 1 << trackIndex;
            }
        }
        // TODO: 内部処理も util::BitFlagSet を使用するよう修正
        SetTrackChannelMixParameter( bitFlag, static_cast<uint32_t>(sourceChannelIndex), parameter );
    }

    //---------------------------------------------------------------------------
    //! @brief    波形の全チャンネルについて、サウンドの5.1chの各スピーカーの音量を全トラック変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのストリームトラックの音量を変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           mixVolume には5.1chの各スピーカーの音量の値を設定します。
    //!           全てのチャンネルの音量を指定する必要があります。
    //!
    //!           @ref SetMixMode で @ref MixMode_Pan が指定されている場合は
    //!           この値を指定しても無視されます。
    //!
    //!           @ref SetMixVolume で設定した値はこれとは別に扱われ、
    //!           最終的なサラウンドボリュームは、掛け合わせた結果になります。
    //!
    //! @param[in] mixVolume          5.1chの各スピーカーの音量です。
    //!
    //! @see MixVolume
    //! @see SetMixVolume
    //! @see SetMixMode
    //---------------------------------------------------------------------------
    void SetTrackMixVolume( const MixVolume& mixVolume ) NN_NOEXCEPT
    {
        MixParameter parameter;
        for ( auto i = 0; i < ChannelIndex_Count; ++i )
        {
            parameter.ch[i] = mixVolume.channel[i];
        }
        SetTrackChannelMixParameter( parameter );
    }

    //---------------------------------------------------------------------------
    //! @brief    メイン出力に対するメインセンド量を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのメイン出力に対するメインセンド量を変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどのメインセンド量とも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           メインセンドは、
    //!           出力に送るサウンドの音量をエフェクトセンドの後で調節するパラメータです。
    //!           主に、エフェクトのドライ・ウェット成分のバランスを調整するために使用されます。
    //!
    //!           センド量 send は相対変化の値を指定し、最終的なセンド量は、
    //!           各 API や SoundMaker で指定した値の合計値を 0.0 ～ 1.0 の範囲でクランプしたものとなります。
    //!           また、 SoundMaker で設定できる値は、ランタイム側で 0.0 ～ 1.0 の値の範囲として解釈されます。
    //!
    //!           すなわち、 0.0 のセンド量の指定は各 API や SoundMaker で指定したセンド量を変更しない設定を表します。
    //!           1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスに最大のセンド量が送られるようなセンド量の加算を表します。
    //!           -1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスにセンド量が送られないようなセンド量の減算を表します。
    //!
    //!           デフォルト値は 0.0 です。
    //!
    //! @param[in] send   0.0 を基準としたセンド量の相対変化の値です。
    //!
    //! @see SetEffectSend
    //---------------------------------------------------------------------------
    void SetMainSend( float send ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetMainSend( send );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    メイン出力に対するエフェクトセンド量を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのメイン出力に対するエフェクトセンド量を変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどのエフェクトセンド量とも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           センド量 send は相対変化の値を指定し、最終的なセンド量は、
    //!           各 API や SoundMaker で指定した値の合計値を 0.0 ～ 1.0 の範囲でクランプしたものとなります。
    //!           また、 SoundMaker で設定できる値は、ランタイム側で 0.0 ～ 1.0 の値の範囲として解釈されます。
    //!
    //!           すなわち、 0.0 のセンド量の指定は各 API や SoundMaker で指定したセンド量を変更しない設定を表します。
    //!           1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスに最大のセンド量が送られるようなセンド量の加算を表します。
    //!           -1.0 のセンド量の指定は、 SoundMaker で設定されたセンド量に関わらず、バスにセンド量が送られないようなセンド量の減算を表します。
    //!
    //!           デフォルト値は 0.0 です。
    //!
    //! @param[in] bus    センド量を設定する AUX のバスです。
    //! @param[in] send   0.0 を基準としたセンド量の相対変化の値です。
    //!
    //! @see AuxBus
    //! @see SetMainSend
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

    //@}


    //! @name トラックパラメータ変更
    //@{

    //---------------------------------------------------------------------------
    //! @brief    ストリームサウンドのトラックの音量を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのストリームトラックの音量を変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどの音量パラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、音量を変更します。
    //!           例えば、トラック 2 とトラック 5 の音量を変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           音量 volume は、0.0 以上の倍率で指定します。
    //!           すなわち、1.0 を指定すると音量に影響を与えません。
    //!           0.0 を指定すると発音されなくなります。デフォルト値は 1.0 です。
    //!
    //!           他の音量パラメータと重ね合わされたあと、
    //!           最終的な音量は nn::audio::VoiceType::GetVolumeMin() ～ nn::audio::VoiceType::GetVolumeMax() の範囲でクランプされます。
    //!           音量の重ね合わせの詳細につきましては @confluencelink{83955829, サウンドパラメータ制御の動作} を参照ください。
    //!
    //! @param[in] trackBitFlag   トラックごとのビットフラグです。
    //! @param[in] volume         変更する音量の倍率です。
    //! @param[in] frames         音量変化にかけるフレーム数。
    //!                           フレーム数は @ref SoundArchivePlayer::Update
    //!                           の呼び出し回数で換算されます。
    //!                           frames には 0 以上の値を指定してください。
    //!                           負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //---------------------------------------------------------------------------
    void SetTrackVolume( uint32_t trackBitFlag, float volume, int frames = 0 ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetTrackVolume( trackBitFlag, volume, frames );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  ストリームサウンドのトラックの出力先を変更します。
    //!
    //!         ハンドルに関連付けられたサウンドのストリームトラックの出力先を変更します。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         出力先の設定は複数の箇所での設定が重ね合わされず、
    //!         以下の優先度に従って設定されます。
    //!         優先度が高い箇所でパラメータ設定がされた場合、
    //!         それより下位の設定は上書きされます。
    //!
    //!         -# 本関数での設定
    //!         -# サウンドハンドルでの設定 (@ref SoundHandle::SetOutputLine)
    //!         -# サウンドプレイヤーでの設定 (@ref SoundPlayer::SetDefaultOutputLine)
    //!
    //!         trackBitFlag で、設定するトラックを指定します。
    //!         下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!         ビットが立っているトラックすべてに対して、出力先を変更します。
    //!         例えば、トラック 2 とトラック 5 の出力先を変更したい場合は
    //!         (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!         出力先は @ref OutputLine のビットフラグの論理和で指定します。
    //!         指定した値は、それまで設定されていた値を上書きします。
    //!         初期状態では、サウンドハンドル・サウンドプレイヤーの設定が引き継がれます。
    //!         一度指定した値を無効にする (＝サウンドハンドル・サウンドプレイヤーの設定
    //!         を引き継ぐ) 場合は @ref ResetTrackOutputLine を呼び出してください。
    //!
    //! @param[in] trackBitFlag   トラックごとのビットフラグです。
    //! @param[in] lineFlag     出力先のビットフラグです。
    //!
    //! @see ResetTrackOutputLine
    //! @see SoundHandle::SetOutputLine
    //! @see SoundPlayer::SetDefaultOutputLine
    //---------------------------------------------------------------------------
    void SetTrackOutputLine( uint32_t trackBitFlag, uint32_t lineFlag ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetTrackOutputLine( trackBitFlag, lineFlag );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  ストリームサウンドのトラックの出力先の設定を無効にします。
    //!
    //!         ハンドルに関連付けられたサウンドのストリームトラックの出力先の設定を無効にします。
    //!         ハンドルが無効の場合は、何もしません。
    //!
    //!         この関数は @ref SetTrackOutputLine で設定された出力先を無効にして、
    //!         デフォルトの出力先へ戻します。デフォルトの出力先は、
    //!         @ref SoundPlayer::SetDefaultOutputLine で設定された値か、
    //!         @ref SoundHandle::SetOutputLine で設定された値です
    //!         (後者の設定が優先されます)。
    //!
    //!         trackBitFlag で、設定するトラックを指定します。
    //!         下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!         ビットが立っているトラックすべてに対して、出力先の設定を無効にします。
    //!         例えば、トラック 2 とトラック 5 の出力先の設定を無効にしたい場合は
    //!         (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //! @param[in] trackBitFlag   トラックごとのビットフラグです。
    //!
    //! @see SetTrackOutputLine
    //! @see SoundHandle::SetOutputLine
    //! @see SoundPlayer::SetDefaultOutputLine
    //---------------------------------------------------------------------------
    void ResetTrackOutputLine( uint32_t trackBitFlag ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->ResetTrackOutputLine( trackBitFlag );
        }
    }
    //@}

    //! @name 出力先別トラックパラメータ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    出力先別のトラックの音量を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドの
    //!           メイン (TV) に対するシーケンストラックの音量を変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどのパンパラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、音量を変更します。
    //!           例えば、トラック 2 とトラック 5 の音量を変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           音量 volume は、0.0 以上の倍率で指定します。
    //!           すなわち、1.0 を指定すると音量に影響を与えません。
    //!           0.0 を指定すると発音されなくなります。デフォルト値は 1.0 です。
    //!
    //!           他の音量パラメータと重ね合わされたあと、
    //!           最終的な音量は nn::audio::VoiceType::GetVolumeMin() ～ nn::audio::VoiceType::GetVolumeMax() の範囲でクランプされます。
    //!           音量の重ね合わせの詳細につきましては @confluencelink{83955829, サウンドパラメータ制御の動作} を参照ください。
    //!
    //! @param[in] device   出力先です。
    //! @param[in] trackBitFlag   トラックごとのビットフラグです。
    //! @param[in] volume         変更する音量の倍率です (0.0 ～) 。
    //---------------------------------------------------------------------------
    void SetTrackOutputVolume( OutputDevice device, uint32_t trackBitFlag, float volume ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            switch (device)
            {
            case OutputDevice_Main:
                m_pSound->SetTrackMainOutVolume( trackBitFlag, volume );
                break;
            default:
                NN_SDK_ASSERT( false, "Invalid device\n" );
                break;
            }
        }
    }

    // --------------------------------------------------------------------------
    //! @brief    出力先別のトラックのパン (左右の定位) を設定します。
    //!
    //!           ハンドルに関連付けられたサウンドの
    //!           メイン (TV) に対するストリームトラックのパンを変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどのパンパラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、パンを変更します。
    //!           例えば、トラック 2 とトラック 5 のパンを変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           pan は、定位の相対変化の値を設定します。
    //!           0.0 を指定するとデータで設定されたパンの値から変化しません。
    //!           1.0 を指定すると中央に定位していた音が右端に定位するようになり、
    //!           -1.0 を指定すると中央に定位していた音が左端に定位するようになります。
    //!           デフォルト値は 0.0 です。
    //!
    //!           @ref SetMixMode で @ref MixMode_MixVolume が指定されている場合は
    //!           この値を指定しても無視されます。
    //!
    //! @param[in] device   出力先です。
    //! @param[in] trackBitFlag   トラックごとのビットフラグです。
    //! @param[in] pan            0.0 を基準としたパンの相対変化の値です。
    // --------------------------------------------------------------------------
    void SetTrackOutputPan( OutputDevice device, uint32_t trackBitFlag, float pan ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            switch (device)
            {
            case OutputDevice_Main:
                m_pSound->SetTrackPan( trackBitFlag, pan );
                break;
            default:
                NN_SDK_ASSERT( false, "Invalid device\n" );
                break;
            }
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    出力先別のトラックのサラウンドパン (前後の定位) を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのシーケンストラックの
    //!           メイン (TV) に対するサラウンドパンを変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどのサラウンドパンパラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、サラウンドパンを変更します。
    //!           例えば、トラック 2 とトラック 5 のサラウンドパンを変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           surroundPan は、定位の相対変化の値を設定します。
    //!           0.0 を指定するとデータで設定されたパンの値から変化しません。
    //!           1.0 を指定すると最前方に定位していた音が中央に定位するようになり、
    //!           2.0 を指定すると最前方に定位していた音が最後方に定位するようになります。
    //!           前方へ定位を移動させたい場合は負の値を指定してください。
    //!           デフォルト値は 0.0 です。
    //!
    //!           @ref SetMixMode で @ref MixMode_MixVolume が指定されている場合は
    //!           この値を指定しても無視されます。
    //!
    //! @param[in] device   出力先です。
    //! @param[in] trackBitFlag   トラックごとのビットフラグです。
    //! @param[in] surroundPan    0.0 を基準としたサラウンドパンの相対変化の値です。
    //---------------------------------------------------------------------------
    void SetTrackOutputSurroundPan( OutputDevice device, uint32_t trackBitFlag, float surroundPan ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            switch (device)
            {
            case OutputDevice_Main:
                m_pSound->SetTrackSurroundPan( trackBitFlag, surroundPan );
                break;
            default:
                NN_SDK_ASSERT( false, "Invalid device\n" );
                break;
            }
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    出力先別のトラックのメインセンド量を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのシーケンストラックの
    //!           メイン (TV) に対するメインセンド量を変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどのサラウンドパンパラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、メインセンド量を変更します。
    //!           例えば、トラック 2 とトラック 5 のメインセンド量を変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           センド量 send は、相対変化の値を指定します。
    //!           すなわち、0.0 を指定するとセンド量を変更しません。
    //!           -1.0 を指定するとメインバスに最大のセンド量で送られていたサウンドが、
    //!           メインバスに送られないようになります。 デフォルト値は 0.0 です。
    //!
    //! @param[in] device   出力先です。
    //! @param[in] trackBitFlag   トラックごとのビットフラグです。
    //! @param[in] send   0.0 を基準としたセンド量の相対変化の値です。
    //---------------------------------------------------------------------------
    void SetTrackOutputMainSend( OutputDevice device, uint32_t trackBitFlag, float send ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            switch (device)
            {
            case OutputDevice_Main:
                m_pSound->SetTrackMainSend( trackBitFlag, send );
                break;
            default:
                NN_SDK_ASSERT( false, "Invalid device\n" );
                break;
            }
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    出力先別のトラックのエフェクトセンド量を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのシーケンストラックの
    //!           メイン (TV) に対するエフェクトセンド量を変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどのサラウンドパンパラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、エフェクトセンド量を変更します。
    //!           例えば、トラック 2 とトラック 5 のエフェクトセンド量を変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           センド量 send は、相対変化の値を指定します。
    //!           すなわち、0.0 を指定するとセンド量を変更しません。
    //!           1.0 を指定すると AUX バスに送られていなかったサウンドが、
    //!           最大のセンド量で送られるようになります。 デフォルト値は 0.0 です。
    //!
    //! @param[in] device   出力先です。
    //! @param[in] trackBitFlag   トラックごとのビットフラグです。
    //! @param[in] bus    センド量を設定する AUX のバスです。
    //! @param[in] send   0.0 を基準としたセンド量の相対変化の値です。
    //---------------------------------------------------------------------------
    void SetTrackOutputEffectSend( OutputDevice device, uint32_t trackBitFlag, AuxBus bus, float send ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            switch (device)
            {
            case OutputDevice_Main:
                m_pSound->SetTrackFxSend( trackBitFlag, bus, send );
                break;
            default:
                NN_SDK_ASSERT( false, "Invalid device\n" );
                break;
            }
        }
    }
    //@}

    //! @name ハンドル操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief    ハンドルにサウンドが関連付けられているかどうかを調べます。
    //!
    //! @return   ハンドルに関連付けられているサウンドがあれば true を返します。
    //!
    //---------------------------------------------------------------------------
    bool IsAttachedSound() const NN_NOEXCEPT { return m_pSound != NULL; }

    //---------------------------------------------------------------------------
    //! @brief    ハンドルからサウンドを解放します。
    //!
    //!           ハンドルから開放されたサウンドは、
    //!           その後ハンドルを通して操作できないようになります。
    //!
    //---------------------------------------------------------------------------
    void DetachSound() NN_NOEXCEPT;

    //@}


    //! @name 情報設定 / 取得
    //@{

    //---------------------------------------------------------------------------
    //! @brief    サウンドの ID を設定します。
    //!
    //!           ハンドルに関連付けられたサウンドの ID を設定します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           @ref SoundArchivePlayer でサウンドを再生すると、
    //!           再生開始時にサウンド ID が自動的に設定されます。
    //!           この関数を呼び出すと、ID を上書きして変更します。
    //!
    //!           設定した ID を取得するためには @ref GetId を呼び出します。
    //!
    //! @param[in] id サウンドを識別する ID です。
    //!
    //! @see SoundArchivePlayer クラス
    //! @see GetId
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
    //! @brief    サウンドの ID を取得します。
    //!
    //!           ハンドルに関連付けられたサウンドに設定されている ID を取得します。
    //!           ハンドルが無効の場合は @ref nn::atk::InvalidSoundId を返します。
    //!
    //!           この関数で取得できる ID は @ref SetId で設定された ID です。
    //!
    //! @return   サウンドに設定されている ID を返します。
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
    //! @brief    ストリームデータの情報を取得します。
    //!
    //!           ハンドルに関連付けられたサウンドで再生している
    //!           ストリームデータの情報を取得します。
    //!           ハンドルが無効の場合は false を返します。
    //!
    //!           ストリームデータの情報は、
    //!           ストリームのプリペア処理が完了するまでは取得することができず、
    //!           その場合は false を返します。
    //!           プリペア処理が完了しているかどうかは @ref IsPrepared で取得できます。
    //!
    //! @param[out] pInfo 結果を格納するストリームデータ情報構造体。
    //!
    //! @return   ストリームデータの情報を取得に成功したら true を、
    //!           取得に失敗したら false を返します。
    //!
    //! @see StreamSoundDataInfo 構造体
    //! @see IsPrepared
    //!
    //---------------------------------------------------------------------------
    bool ReadStreamSoundDataInfo( StreamSoundDataInfo* pInfo ) const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() )
        {
            return false;
        }
        return m_pSound->ReadStreamSoundDataInfo( pInfo );
    }

    //---------------------------------------------------------------------------
    //! @brief    ストリームデータの情報を取得します。
    //!
    //! @deprecated この関数は廃止予定です。代わりに @ref ReadStreamSoundDataInfo を使用してください。
    //!
    //! @param[out] pInfo 結果を格納するストリームデータ情報構造体。
    //! @return   ストリームデータの情報を取得に成功したら true を、
    //!           取得に失敗したら false を返します。
    //---------------------------------------------------------------------------
    NN_DEPRECATED bool ReadStreamDataInfo( StreamSoundDataInfo* pInfo ) const NN_NOEXCEPT
    {
        return ReadStreamSoundDataInfo( pInfo );
    }

    //---------------------------------------------------------------------------
    //! @brief    再生中のストリームデータのループ回数を取得します。
    //!
    //!           ハンドルに関連付けられたサウンドで再生している、
    //!           ストリームデータのループ回数を取得します。
    //!           ハンドルが無効の場合は、負の値を返します。
    //!
    //!           ストリームジャンプ機能を使用するストリームサウンドの場合は
    //!           ループ回数をカウントできないため、この関数は正しい値を返さない点にご注意ください。
    //!
    //! @return   ストリームデータのループ回数を返します。
    //!
    //---------------------------------------------------------------------------
    int GetPlayLoopCount() const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() )
        {
            return -1;
        }
        return m_pSound->GetPlayLoopCount();
    }

    //---------------------------------------------------------------------------
    //! @brief    再生中のストリームデータの再生位置を取得します。
    //!
    //!           ハンドルに関連付けられたサウンドで再生している、
    //!           ストリームデータの再生位置をサンプル数で取得します。
    //!           ハンドルが無効の場合は、負の値を返します。
    //!
    //! @return   ストリームデータの再生位置をサンプル数で返します。
    //---------------------------------------------------------------------------
    int64_t GetPlaySamplePosition() const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() )
        {
            return -1;
        }
        return m_pSound->GetPlaySamplePosition(true);
    }

    //---------------------------------------------------------------------------
    //! @brief    ストリームバッファに波形データが満たされている割合を取得します。
    //!
    //!           ストリームサウンドは、バッファ中の波形データが消費しながら再生します。
    //!           バッファが空になる前にデータをバッファにロードして、
    //!           次の波形データ補充しています。ロードが間に合わない場合、
    //!           データのロードが完了するまでサウンドの再生は停止します。
    //!
    //!           この関数で取得できる値は、
    //!           ストリームサウンドの再生に対して、
    //!           データのロードが間に合っているかどうかを判断する指標となります。
    //!           この値が 0.0f になると、
    //!           ストリームサウンドの再生がロードの遅延によって停止してしまいます。
    //!           値が大きいほど、
    //!           ストリームサウンドがロード遅延によって停止する危険性が低いことを表します。
    //!
    //! @return   ストリームバッファに波形データが満たされている割合を
    //!           0.0f ～ 100.0f の値で返します。
    //!
    //! @see IsSuspendByLoadingDelay
    //!
    //---------------------------------------------------------------------------
    float GetFilledBufferPercentage() const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() )
        {
            return 0.0f;
        }
        return m_pSound->GetFilledBufferPercentage();
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief     ストリーム再生に使用するバッファのブロックのうち、特定の状態のブロックの数を取得します。
    //! @return    ストリーム再生に使用するバッファのブロックのうち、特定の状態のブロックの数です。
    //!            @ref GetTotalBufferBlockCount と組み合わせる事で、特定の状態のバッファブロックの割合を知る事ができます。
    //! @see GetTotalBufferBlockCount
    //---------------------------------------------------------------------------
    int GetBufferBlockCount(nn::atk::WaveBuffer::Status status) const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() )
        {
            return 0;
        }
        return m_pSound->GetBufferBlockCount(status);
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief     ストリーム再生に使用するバッファの総ブロック数を取得します。
    //! @return    ストリーム再生に使用するバッファの総ブロック数です。
    //!            総ブロック数は、ストリームで使用するチャンネルの数に比例して増加します。
    //!            １チャンネルあたりのバッファのブロックの数は @ref SoundArchivePlayer::SetStreamBlockCount で指定した数になります。
    //! @see GetBufferBlockCount
    //---------------------------------------------------------------------------
    int GetTotalBufferBlockCount() const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() )
        {
            return 0;
        }
        return m_pSound->GetTotalBufferBlockCount();
    }
    //@}


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
    //!         @ref SetTrackChannelMixParameter で設定した値はこれとは別に扱われ、
    //!         最終的なサラウンドボリュームは、掛け合わせた結果になります。
    //!
    //! @param[in]    srcChNo サラウンドボリュームを指定するチャンネルです。
    //! @param[in]    param   サラウンドボリュームです。
    //!
    //! @see SetOutputChannelMixParameter
    //! @see SetTrackChannelMixParameter
    //! @see SetTrackChannelOutputMixParameter
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
    //!         ハンドルに関連付けられたサウンドの
    //!         メイン (TV) に対するサラウンドボリュームを変更します。
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
    //!         @ref SetTrackChannelMixParameter で設定した値はこれとは別に扱われ、
    //!         最終的なサラウンドボリュームは、掛け合わせた結果になります。
    //!
    //! @param[in] device  出力先です。
    //! @param[in] srcChNo サラウンドボリュームを指定するチャンネルです。
    //! @param[in] param   サラウンドボリュームです。
    //!
    //! @see SetChannelMixParameter
    //! @see SetTrackChannelMixParameter
    //! @see SetTrackChannelOutputMixParameter
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

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    波形のチャンネル別に、サウンドのサラウンドボリューム(5.1chの各スピーカーの音量)をトラック毎に変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのシーケンストラックのサラウンドボリュームを変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、サラウンドボリュームを変更します。
    //!           例えば、トラック 2 とトラック 5 のサラウンドボリュームを変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           param は、サラウンドボリュームの値を設定します。
    //!           全てのチャンネルのサラウンドボリュームを指定する必要があります。
    //!
    //!           デフォルト値は fL, fR, rL, rR, fC, lfe ともに 1.0f です。
    //!
    //!           ミックスモードにパンが指定されている場合は
    //!           この値を指定しても無視されます。
    //!
    //!           @ref SetChannelMixParameter で設定した値はこれとは別に扱われ、
    //!           最終的なサラウンドボリュームは、掛け合わせた結果になります。
    //!
    //! @param[in] trackBitFlag トラックごとのビットフラグです。
    //! @param[in] srcChNo      サラウンドボリュームを指定するチャンネルです。
    //! @param[in] param        サラウンドボリュームです。
    //!
    //! @see SetChannelMixParameter
    //! @see SetOutputChannelMixParameter
    //! @see SetTrackChannelOutputMixParameter
    //---------------------------------------------------------------------------
    void SetTrackChannelMixParameter( uint32_t trackBitFlag, uint32_t srcChNo, const MixParameter& param ) NN_NOEXCEPT
    {
        if( IsAttachedSound() )
        {
            m_pSound->SetTrackChannelMixParameter( trackBitFlag, srcChNo, param );
        }
    }
    //! @briefprivate
    //! @brief    波形の有効なチャンネルに、同一のサラウンドボリュームを設定します。
    //! @param param :private
    void SetTrackChannelMixParameter( const MixParameter& param ) NN_NOEXCEPT
    {
        if( IsAttachedSound() )
        {
            // 有効なチャンネルで同じ設定を使用
            for ( uint32_t srcChNo = 0; srcChNo < WaveChannelMax; srcChNo++)
            {
                uint32_t trackBitFlag = m_pSound->GetAvailableTrackBitFlag( srcChNo );
                if ( trackBitFlag != 0x0 )
                {
                    m_pSound->SetTrackChannelMixParameter( trackBitFlag, srcChNo, param );
                }
            }
        }
    }
    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    波形のチャンネル別に、サウンドのサラウンドボリューム(5.1chの各スピーカーの音量)をトラック毎に出力先別で変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのシーケンストラックの
    //!           メイン (TV) に対するサラウンドボリュームを変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、サラウンドボリュームを変更します。
    //!           例えば、トラック 2 とトラック 5 のサラウンドボリュームを変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           param は、サラウンドボリュームの値を設定します。
    //!           全てのチャンネルのサラウンドボリュームを指定する必要があります。
    //!
    //!           デフォルト値は fL, fR, rL, rR, fC, lfe ともに 1.0f です。
    //!
    //!           ミックスモードにパンが指定されている場合は
    //!           この値を指定しても無視されます。
    //!
    //!           @ref SetChannelMixParameter で設定した値はこれとは別に扱われ、
    //!           最終的なサラウンドボリュームは、掛け合わせた結果になります。
    //!
    //! @param[in] device       出力先です。
    //! @param[in] trackBitFlag トラックごとのビットフラグです。
    //! @param[in] srcChNo      サラウンドボリュームを指定するチャンネルです。
    //! @param[in] param        サラウンドボリューム量です。
    //!
    //! @see SetChannelMixParameter
    //! @see SetOutputChannelMixParameter
    //! @see SetTrackChannelMixParameter
    //---------------------------------------------------------------------------
    void SetTrackChannelOutputMixParameter( OutputDevice device, uint32_t trackBitFlag, uint32_t srcChNo, const MixParameter& param ) NN_NOEXCEPT
    {
        if( IsAttachedSound() )
        {
            switch (device)
            {
            case OutputDevice_Main:
                m_pSound->SetTrackChannelMixParameter( trackBitFlag, srcChNo, param );
                break;
            default:
                NN_SDK_ASSERT( false, "Invalid device\n" );
                break;
            }
        }
    }
    //! @briefprivate
    //! @brief    出力先指定で、波形の有効なチャンネルに、同一のサラウンドボリュームを設定します。
    //! @param device :private
    //! @param param :private
    void SetTrackChannelOutputMixParameter( OutputDevice device, const MixParameter& param ) NN_NOEXCEPT
    {
        if( IsAttachedSound() )
        {
            // 有効なチャンネルで同じ設定を使用
            switch (device)
            {
            case OutputDevice_Main:
                for ( uint32_t srcChNo = 0; srcChNo < WaveChannelMax; srcChNo++)
                {
                    uint32_t trackBitFlag = m_pSound->GetAvailableTrackBitFlag( srcChNo );
                    if ( trackBitFlag != 0x0 )
                    {
                        m_pSound->SetTrackChannelMixParameter( trackBitFlag, srcChNo, param );
                    }
                }
                break;
            default:
                NN_SDK_ASSERT( false, "Invalid device\n" );
                break;
            }
        }
    }

    //! @briefprivate
    //! @brief    再生中のストリームサウンドデータの指定したチャンネルについて、有効なトラックを全てオンにしたビットフラグを取得します。
    //! @param srcChNo :private
    //! @return :private
    uint32_t GetAvailableTrackBitFlag(uint32_t srcChNo) const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() || srcChNo >= WaveChannelMax ) return 0x0;
        return m_pSound->GetAvailableTrackBitFlag(srcChNo);
    }

    //! @briefprivate
    //! @brief  内部で使用する nn::audio のオーディオレンダラでのサウンドの処理負荷を取得します。
    nn::os::Tick GetProcessTick(const SoundProfile& profile) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            return m_pSound->GetProcessTick(profile);
        }
        return nn::os::Tick(0);
    }

    // ハンドルをサウンドに関連付ける
    //! @internal
    //! @param pSound :private
    void detail_AttachSoundAsTempHandle( detail::StreamSound* pSound ) NN_NOEXCEPT;

    //! @internal
    //! @return :private
    detail::StreamSound* detail_GetAttachedSound() NN_NOEXCEPT { return m_pSound; }

    //! @internal
    //! @return :private
    const detail::StreamSound* detail_GetAttachedSound() const NN_NOEXCEPT { return m_pSound; }

    //! @internal
    //! @brief ファイルリードキャッシュ機能使用時のログ出力を設定します
    void* detail_SetFsAccessLog( detail::fnd::FsAccessLog* pFsAccessLog ) NN_NOEXCEPT
    {
        if ( m_pSound != nullptr )
        {
            return m_pSound->detail_SetFsAccessLog(pFsAccessLog);
        }

        return nullptr;
    }

    //! @internal
    //! @brief ハンドルに関連付けられたサウンドインスタンスが
    //!        指定したサウンドプレイヤーインスタンスを持っているかを取得します。
    //! @param[in] pPlayer サウンドプレイヤーインスタンスです。
    //! @return 指定したサウンドプレイヤーインスタンスを持っている場合 true を返します。
    //!
    bool HasStreamSoundPlayer(const detail::driver::StreamSoundPlayer* pPlayer) const NN_NOEXCEPT
    {
        if ( m_pSound == nullptr || pPlayer == nullptr )
        {
            return false;
        }

        return &(m_pSound->m_PlayerInstance) == pPlayer; // NOTE: friend class の利用
    }

private:
    NN_DISALLOW_COPY( StreamSoundHandle );

    detail::StreamSound* m_pSound;
};

} // namespace nn::atk
} // namespace nn

