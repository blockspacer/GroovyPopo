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
 * :include nn/atk/atk_SequenceSoundHandle.h
 *
 * @file atk_SequenceSoundHandle.h
 */

#pragma once

#include <nn/atk/atk_SequenceSound.h>
#include <nn/atk/atk_ChannelMixVolume.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @brief    再生したシーケンスサウンドの操作を行うためのハンドルクラスです。
//!
//!           サウンドは再生時に汎用サウンドハンドル
//!           @ref SoundHandle と関連付けられますが、
//!           シーケンスサウンドに固有の処理は汎用サウンドハンドルには実装されていません。
//!
//!           シーケンスサウンドに固有の処理を行いたい場合は、
//!           汎用サウンドハンドルを引数にとるコンストラクタを用いて、
//!           シーケンスサウンドハンドルを生成し、使用してください。
//!
//!           ハンドルの使い方は、汎用サウンドハンドル @ref SoundHandle と同じです。
//!
//! @see      SoundHandle クラス
//!
//---------------------------------------------------------------------------
class SequenceSoundHandle
{
public:
    //! @ref SetTrackBankIndex で index として指定できる最小値です。
    static const int BankIndexMin = detail::SequenceSound::BankIndexMin;
    //! @ref SetTrackBankIndex で index として指定できる最大値です。
    static const int BankIndexMax = detail::SequenceSound::BankIndexMax;
    //! @ref SetTrackTranspose で transpose として指定できる最小値です。
    static const int8_t TransposeMin = detail::SequenceSound::TransposeMin;
    //! @ref SetTrackTranspose で transpose として指定できる最大値です。
    static const int8_t TransposeMax = detail::SequenceSound::TransposeMax;
    //! @ref SetTrackVelocityRange で range として指定できる最小値です。
    static const uint8_t VelocityRangeMin = detail::SequenceSound::VelocityRangeMin;
    //! @ref SetTrackVelocityRange で range として指定できる最大値です。
    static const uint8_t VelocityRangeMax = detail::SequenceSound::VelocityRangeMax;

    //---------------------------------------------------------------------------
    //! @brief  シーケンス変数の変数番号の最大値です。
    //---------------------------------------------------------------------------
    static const int VariableIndexMax = 15;

    //---------------------------------------------------------------------------
    //! @brief  シーケンス変数のトラック番号の最大値です。
    //---------------------------------------------------------------------------
    static const int TrackIndexMax = 15;

public:
    //! @brief    シーケンスサウンドのトラックを表すビットフラッグを扱う型です。
    typedef nn::util::BitFlagSet<TrackIndexMax + 1> TrackBitFlagSet;

public:
    //----------------------------------------
    //! @name コンストラクタ/デストラクタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //!           引数の無いコンストラクタは、
    //!           どのサウンドとも関連付けられていないハンドルを構築します。
    //!
    //---------------------------------------------------------------------------
    SequenceSoundHandle() NN_NOEXCEPT : m_pSound( NULL ) { }

    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //!           引数付きコンストラクタは、引数 handle
    //!           で指定したサウンドハンドルが参照しているサウンドを用いて、
    //!           シーケンスサウンドハンドルを構築します。
    //!           シーケンスサウンド以外のサウンドがサウンドハンドルに関連付けられているときは、
    //!           引数の無いコンストラクタと同様に構築します。
    //!
    //!           引数付きコンストラクタで構築したときに、
    //!           サウンドが既に別のシーケンスサウンドハンドルと関連付けられている場合は、
    //!           その関連付けが切られる点に注意してください。
    //!           ただし、引数に渡したサウンドハンドルとの関連付けは切れません。
    //!
    //! @param[in] handle シーケンスサウンドと関連付けられたサウンドハンドルです。
    //!
    //! @see      SoundHandle クラス
    //!
    //---------------------------------------------------------------------------
    explicit SequenceSoundHandle( SoundHandle* handle ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    デストラクタです。
    //!
    //---------------------------------------------------------------------------
    ~SequenceSoundHandle() NN_NOEXCEPT { DetachSound(); }
    //@}

    // -----------------------------------------------------------------
    //! @name 再生/停止/一時停止/再開
    //@{

    //---------------------------------------------------------------------------
    //! @brief    再生準備の完了を待ってサウンドを再生します。
    //!
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           サウンドの再生を開始するためには、@ref SoundArchivePlayer::PrepareSound
    //!           を呼び出した後、再生準備が完了していなければなりません。
    //!           この関数は、再生準備が完了したサウンドの再生を開始します。
    //!           再生準備が完了していないサウンドは、完了するまで待ってから再生を開始します。
    //!
    //! @see  nn::atk::SoundArchivePlayer::PrepareSound
    //!
    //---------------------------------------------------------------------------
    void StartPrepared() NN_NOEXCEPT
    {
        if ( IsAttachedSound() ) m_pSound->StartPrepared();
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
    //! @param[in] fadeFrames   フェードアウトにかけるフレーム数です。
    //!                         フレーム数は @ref SoundArchivePlayer::Update
    //!                         の呼び出し回数で換算されます。
    //!                         fadeFrames には 0 以上の値を指定してください。
    //!                         負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //!
    //---------------------------------------------------------------------------
    void Stop( int fadeFrames ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() ) m_pSound->Stop( fadeFrames );
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
    //! @param[in]    fadeFrames    フェードイン・フェードアウトにかけるフレーム数です。
    //!                             フレーム数は @ref SoundArchivePlayer::Update
    //!                             の呼び出し回数で換算されます。
    //!                             負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //!
    //! @see  IsPause
    //!
    //---------------------------------------------------------------------------
    void Pause( bool flag, int fadeFrames ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->Pause(flag, fadeFrames);
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
    //!
    //!                   また、フェードにかかるフレーム数は、最大音量から音が消えるまで、
    //!                   あるいは発音していない状態から最大音量に達するまでにかかる変化速度を表しますので、
    //!                   フェードイン中にさらにフェードアウトを指定した時などは、
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
    //!
    //---------------------------------------------------------------------------
    void Pause(bool flag, int fadeFrames, PauseMode pauseMode) NN_NOEXCEPT
    {
        if (IsAttachedSound())
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
    bool IsPrepared() const NN_NOEXCEPT { return IsAttachedSound() && m_pSound->IsPrepared(); }

    //---------------------------------------------------------------------------
    //! @brief    サウンドが一時停止中かどうかを調べます。
    //!
    //! @return   サウンドが一時停止状態であれば true を返します。
    //!
    //! @see Pause
    //!
    //---------------------------------------------------------------------------
    bool IsPause() const NN_NOEXCEPT { return IsAttachedSound() && m_pSound->IsPause(); }

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
    //! @param[in] frames   フェードインにかけるフレーム数です。
    //!                     フレーム数は @ref SoundArchivePlayer::Update
    //!                     の呼び出し回数で換算されます。
    //!                     frames には 0 以上の値を指定してください。
    //!                     負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //!
    //! @see  SoundArchivePlayer::StartSound
    //! @see  StartPrepared
    //!
    //---------------------------------------------------------------------------
    void FadeIn( int frames ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() ) m_pSound->FadeIn( frames );
    }
    //@}

    // -----------------------------------------------------------------
    //! @name   パラメータ変更
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
    //! @param[in] frames   音量変化にかけるフレーム数です。
    //!                     フレーム数は @ref SoundArchivePlayer::Update
    //!                     の呼び出し回数で換算されます。
    //!                     frames には 0 以上の値を指定してください。
    //!                     負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //!
    //---------------------------------------------------------------------------
    void SetVolume( float volume, int frames = 0 ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() ) m_pSound->SetVolume( volume, frames );
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
        if ( IsAttachedSound() ) m_pSound->SetPitch( pitch );
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
        if ( IsAttachedSound() ) m_pSound->SetLpfFreq( lowPassFilterFrequency );
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
    //!           priority の値の範囲は nn::atk::PlayerPriorityMin ～ nn::atk::PlayerPriorityMax で、大きいほど優先度が高くなります。
    //!
    //! @param[in] priority   プレイヤープライオリティの値です。
    //!
    //---------------------------------------------------------------------------
    void SetPlayerPriority( int priority ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() ) m_pSound->SetPlayerPriority( priority );
    }

    //---------------------------------------------------------------------------
    //! @brief    発音プライオリティを変更します。
    //!
    //!           ハンドルに関連付けられたサウンドの発音プライオリティを変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数は、
    //!           サウンドアーカイブ中のデータで指定されている、
    //!           発音プライオリティの値を変更します。
    //!
    //!           priority の値の範囲は nn::atk::ChannelPriorityMin ～ nn::atk::ChannelPriorityMax で、大きいほど優先度が高くなります。
    //!
    //! @param[in] priority   発音プライオリティの値です。
    //!
    //---------------------------------------------------------------------------
    void SetChannelPriority( int priority ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() ) m_pSound->SetChannelPriority( priority );
    }

    //---------------------------------------------------------------------------
    //! @brief    シーケンスサウンドのテンポの倍率を変更します。
    //!
    //!           ハンドルに関連付けられたシーケンスサウンドのテンポを変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           テンポ tempoRatio は、シーケンスデータのテンポに対する倍率で指定します。
    //!           すなわち、1.0 を指定するとテンポを変更しません。
    //!
    //! @param[in] tempoRatio 変更するテンポの倍率です。
    //!
    //---------------------------------------------------------------------------
    void SetTempoRatio( float tempoRatio ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() ) m_pSound->SetTempoRatio( tempoRatio );
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
    //! @brief  メイン (TV) 出力に対するサラウンドパン (前後の定位) を変更します。
    //!
    //!         ハンドルに関連付けられたサウンドのメイン出力に対するサラウンドパンを変更します。
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
    //!         デフォルト値は 0.0 です。
    //!
    //!         @ref SetMixMode で @ref MixMode_MixVolume が指定されている場合は
    //!         この値を指定しても無視されます。
    //!
    //!         サラウンドパンに設定した値は、出力モードがサラウンドの場合だけ反映されます。
    //!         詳しくは @ref nn::atk::SoundSystem::SetOutputMode を参照ください。
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
    //!           ハンドルに関連付けられたサウンドのシーケンストラックの音量を変更します。
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
    //! @param[in] trackBitFlag            設定を行うトラックを表すビットフラグです。
    //! @param[in] sourceChannelIndex      mixVolume を適用する波形のチャンネル番号です。
    //! @param[in] mixVolume               5.1chの各スピーカーの音量です。
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
        for ( auto trackIndex = 0; trackIndex <= TrackIndexMax; ++trackIndex )
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
    //!           ハンドルに関連付けられたサウンドのシーケンストラックの音量を変更します。
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
    //! @brief    メイン (TV) 出力に対するメインセンド量を変更します。
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
    //!
    //---------------------------------------------------------------------------
    void SetMainSend( float send ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetMainSend( send );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    メイン (TV) 出力に対するエフェクトセンド量を変更します。
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
    //!
    //---------------------------------------------------------------------------
    void SetFxSend( AuxBus bus, float send ) NN_NOEXCEPT
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

    // -----------------------------------------------------------------
    //! @name シーケンス変数
    //@{

    //---------------------------------------------------------------------------
    //! @brief    シーケンスローカル変数を読み込みます。
    //!
    //!           読み込みに成功すると pOutValue で指定したアドレスへ変数の値が書き込まれます。
    //!
    //!           サウンドハンドルが無効の場合は、読み込みに失敗して false を返します。
    //!
    //! @param[out] pOutValue   読み込む値を格納する変数へのポインタです。
    //! @param[in] variableNum  変数番号 ( 0 ～ @ref VariableIndexMax ) です。
    //!
    //! @return   読み込みに成功したら true を、失敗したら false を返します。
    //!
    //---------------------------------------------------------------------------
    bool ReadVariable( int16_t* pOutValue, int variableNum ) const NN_NOEXCEPT
    {
        return IsAttachedSound() && m_pSound->ReadVariable( variableNum, pOutValue );
    }

    //---------------------------------------------------------------------------
    //! @brief    シーケンスグローバル変数を読み込みます。
    //!
    //!           読み込みに成功すると pOutValue で指定したアドレスへ変数の値が書き込まれます。
    //!
    //! @param[out] pOutValue   読み込む値を格納する変数へのポインタです。
    //! @param[in] variableNum  変数番号 ( 0 ～ @ref VariableIndexMax ) です。
    //!
    //! @return   読み込みに成功したら true を、失敗したら false を返します。
    //!
    //---------------------------------------------------------------------------
    static bool ReadGlobalVariable( int16_t* pOutValue, int variableNum ) NN_NOEXCEPT
    {
        return detail::SequenceSound::ReadGlobalVariable( variableNum, pOutValue );
    }

    //---------------------------------------------------------------------------
    //! @brief    シーケンストラック変数を読み込みます。
    //!
    //!           読み込みに成功すると pOutValue で指定したアドレスへ変数の値が書き込まれます。
    //!
    //!           サウンドハンドルが無効の場合は、読み込みに失敗して false を返します。
    //!
    //! @param[out] pOutValue       読み込む値を格納する変数へのポインタです。
    //! @param[in] trackNum         トラック番号 ( 0 ～ @ref TrackIndexMax ) です。
    //! @param[in] variableNum      変数番号 ( 0 ～ @ref VariableIndexMax ) です。
    //!
    //! @return   読み込みに成功したら true を、失敗したら false を返します。
    //!
    //---------------------------------------------------------------------------
    bool ReadTrackVariable( int16_t* pOutValue, int trackNum, int variableNum ) const NN_NOEXCEPT
    {
        return IsAttachedSound() && m_pSound->ReadTrackVariable( trackNum, variableNum, pOutValue );
    }

    //---------------------------------------------------------------------------
    //! @brief    シーケンスローカル変数を書き込みます。
    //!
    //!           サウンドハンドルが無効の場合は、書き込みに失敗して false を返します。
    //!
    //! @param[in] variableNum  変数番号 ( 0 ～ @ref VariableIndexMax ) です。
    //! @param[in] variable     書き込む変数値です。
    //!
    //! @return   書き込みに成功したら true を、失敗したら false を返します。
    //!
    //---------------------------------------------------------------------------
    bool WriteVariable( int variableNum, int16_t variable ) NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() ) return false;
        m_pSound->WriteVariable( variableNum, variable );
        return true;
    }

    //---------------------------------------------------------------------------
    //! @brief    シーケンスグローバル変数を書き込みます。
    //!
    //! @param[in] variableNum  変数番号（ 0 ～ @ref VariableIndexMax ) です。
    //! @param[in] variable     書き込む変数値です。
    //!
    //! @return   書き込みに成功したら true を、失敗したら false を返します。
    //!
    //---------------------------------------------------------------------------
    static bool WriteGlobalVariable( int variableNum, int16_t variable ) NN_NOEXCEPT
    {
        detail::SequenceSound::WriteGlobalVariable( variableNum, variable );
        return true;
    }

    //---------------------------------------------------------------------------
    //! @brief    シーケンストラック変数を書き込みます。
    //!
    //!           サウンドハンドルが無効の場合は、書き込みに失敗して false を返します。
    //!
    //! @param[in] trackNum        トラック番号 ( 0 ～ @ref TrackIndexMax ) です。
    //! @param[in] variableNum     変数番号 ( 0 ～ @ref VariableIndexMax ) です。
    //! @param[in] variable        書き込む変数値です。
    //!
    //! @return   書き込みに成功したら true を、失敗したら false を返します。
    //!
    //---------------------------------------------------------------------------
    bool WriteTrackVariable( int trackNum, int variableNum, int16_t variable ) NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() ) return false;
        m_pSound->WriteTrackVariable( trackNum, variableNum, variable );
        return true;
    }
    //@}

    // -----------------------------------------------------------------
    //! @name トラックパラメータ設定
    //@{

    //---------------------------------------------------------------------------
    //! @brief    シーケンスサウンドのトラックをミュートします。
    //!           または、ミュート状態を解除します。
    //!
    //!           ハンドルに関連付けられたサウンドのシーケンストラックのミュート状態を変更します。
    //!           サウンドハンドルが無効の場合は、何もしません。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、ミュートの設定を変更します。
    //!           例えば、トラック 2 とトラック 5 のミュート設定を変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           トラックをミュートすると、
    //!           それ以降は指定されたトラックでノートオンが行われなくなります。
    //!
    //!           発音中の音が停止するかどうかは @ref SequenceMute での指定によります。
    //!           ミュートを解除すると発音が行われるようになりますが、
    //!           すぐに音が鳴り出すわけではありません。
    //!           次のノートオンから鳴り始めることに注意してください。
    //!
    //!           ミュート状態によく似た、トラックを無音にするサイレント状態
    //!           (@ref SetTrackSilence) があります。
    //!           ミュート状態とサイレント状態はそれぞれ個別に設定できます。
    //!           両方を設定した場合は、両方を解除するまでトラックは発音しなくなります。
    //!
    //! @param[in] trackBitFlag   トラックごとのビットフラグです。
    //! @param[in] mute           ミュート状態です。
    //!
    //! @see SequenceMute
    //! @see SetTrackSilence
    //!
    //---------------------------------------------------------------------------
    void SetTrackMute( uint32_t trackBitFlag, SequenceMute mute ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetTrackMute( trackBitFlag, mute );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    シーケンスサウンドのトラックをミュートします。
    //!           または、ミュート状態を解除します。
    //!
    //!           ハンドルに関連付けられたサウンドのシーケンストラックのミュート状態を変更します。
    //!           サウンドハンドルが無効の場合は、何もしません。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、ミュートの設定を変更します。
    //!           例えば、トラック 2 とトラック 5 のミュート設定を変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           トラックをミュートすると、
    //!           それ以降は指定されたトラックでノートオンが行われなくなります。
    //!
    //!           muteFlag を true にするとシーケンストラックをミュート状態にします。
    //!           同時に、そのトラックで発音中の音を直ちに停止します
    //!           (@ref SequenceMute の MUTE_STOP と同じです)。
    //!
    //!           muteFlag を false にすると、ミュートを解除します。
    //!
    //!           ミュートを解除すると発音が行われるようになりますが、
    //!           すぐに音が鳴り出すわけではありません。
    //!           次のノートオンから鳴り始めることに注意してください。
    //!
    //!           ミュート状態によく似た、トラックを無音にするサイレント状態
    //!           (@ref SetTrackSilence) があります。
    //!           ミュート状態とサイレント状態はそれぞれ個別に設定できます。
    //!           両方を設定した場合は、両方を解除するまでトラックは発音しなくなります。
    //!
    //! @param[in] trackBitFlag   トラックごとのビットフラグです。
    //! @param[in] muteFlag       ミュートするときは true、
    //!                           ミュートを解除するときは false を指定します。
    //!
    //! @see SequenceMute
    //! @see SetTrackSilence
    //!
    //---------------------------------------------------------------------------
    void SetTrackMute( uint32_t trackBitFlag, bool muteFlag ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetTrackMute( trackBitFlag, muteFlag );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    シーケンスサウンドのトラックをサイレント状態にします。
    //!           または、サイレント状態を解除します。
    //!
    //!           ハンドルに関連付けられたサウンドのシーケンストラックのサイレント状態を変更します。
    //!           サウンドハンドルが無効の場合は、何もしません。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、サイレント状態を変更します。
    //!           例えば、トラック 2 とトラック 5 のサイレント状態を変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           サイレント状態はミュート状態 (@ref SetTrackMute) と似ています。
    //!           ミュート状態のトラックではノートオン処理が行われないので、
    //!           そのトラックが発音しないようなります。
    //!           一方、サイレンス状態のトラックではノートオン処理は行われますが、
    //!           音量をゼロにすることによって発音しないようになります。
    //!           そのため、サイレンス状態を解除したときに、
    //!           ノートを途中から即座に再生することが可能になります。
    //!
    //!           fadeTimes は、この関数を呼び出したときに発音中の音に対してのみ、
    //!           効果があることに注意してください。
    //!           そのためサイレント状態に移行するフェード期間中に、
    //!           新たに発音した音は、鳴り始めから無音状態になります。
    //!
    //!           サイレント状態では、
    //!           ノートオンされたボイスが無音の状態で消費されてしまうので注意してください。
    //!
    //!           ミュート状態とサイレント状態はそれぞれ個別に設定できます。
    //!           両方を設定した場合は、両方を解除するまでトラックは発音しなくなります。
    //!
    //! @param[in] trackBitFlag   トラックごとのビットフラグです。
    //! @param[in] silenceFlag    サイレント状態にするときは true、
    //!                           サイレント状態を解除する時は false を指定します。
    //! @param[in] fadeTimes      サイレント状態を変更する時にかかるフェードの時間 (ミリ秒) です。
    //!
    //! @see SetTrackMute
    //!
    //---------------------------------------------------------------------------
    void SetTrackSilence( uint32_t trackBitFlag, bool silenceFlag, int fadeTimes ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetTrackSilence( trackBitFlag, silenceFlag, fadeTimes );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    シーケンスサウンドのトラックの音量を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのシーケンストラックの音量を変更します。
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
    //!
    //---------------------------------------------------------------------------
    void SetTrackVolume( uint32_t trackBitFlag, float volume ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetTrackVolume( trackBitFlag, volume );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    シーケンスサウンドのトラックの音程を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのシーケンストラックの音程を変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどの音程パラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、音程を変更します。
    //!           例えば、トラック 2 とトラック 5 の音程を変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           音程 pitch は、周波数の比率で指定します。
    //!           すなわち、1.0 を指定すると音程に影響を与えません。
    //!           2.0 を指定すると再生される周波数が 2 倍になり、
    //!           1 オクターブ高い音程になります。
    //!           0.5 を指定すると 1 オクターブ低い音程になります。
    //!           デフォルト値は 1.0 です。
    //!
    //! @param trackBitFlag   トラックごとのビットフラグです。
    //! @param pitch          変更する音程の周波数比率です。
    //!
    //---------------------------------------------------------------------------
    void SetTrackPitch( uint32_t trackBitFlag, float pitch ) NN_NOEXCEPT
    {
        if ( IsAttachedSound() )
        {
            m_pSound->SetTrackPitch( trackBitFlag, pitch );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    シーケンスサウンドのトラックのバンクを変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのシーケンストラックのバンクを変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどの音量パラメータとも独立して動作し、
    //!           後着優先で有効になります。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、バンクを変更します。
    //!           例えば、トラック 2 とトラック 5 のバンクを変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           バンク番号 bankIndex は、0 ～ 3 の数値で指定します。
    //!           0 ～ 3 は SoundMaker の「マルチバンクの設定」
    //!           で表示されるダイアログ内のリストの行番号に該当します。
    //!           それ以外の番号を指定すると、
    //!           (Debug 版、Development 版) アサートで停止します。
    //!           (Release 版) false を返します。
    //!
    //!           シーケンス内に bank_select コマンドを入れていた場合、
    //!           本関数での設定と bank_select コマンドは、
    //!           後から設定されたものが有効になります。
    //!
    //! @param[in] trackBitFlag   トラックごとのビットフラグです。
    //! @param[in] bankIndex      変更するバンクの番号です。
    //!
    //! @return   bankIndex が BankIndexMin ～ BankIndexMax
    //!           の範囲内の場合は true を返します。範囲外の場合は false を返します
    //!           (Release 版のみ。Debug 版、Development 版はアサートで停止します)。
    //!
    //---------------------------------------------------------------------------
    bool SetTrackBankIndex( uint32_t trackBitFlag, int bankIndex ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( bankIndex >= BankIndexMin && bankIndex <= BankIndexMax );
        if ( bankIndex < BankIndexMin || bankIndex > BankIndexMax )
        {
            return false;
        }

        if ( IsAttachedSound() )
        {
            m_pSound->SetTrackBankIndex( trackBitFlag, bankIndex );
        }
        return true;
    }

    //---------------------------------------------------------------------------
    //! @brief    シーケンスサウンドのトラックのトランスポーズの値を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのシーケンストラックの
    //!           トランスポーズの値を変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどのトランスポーズの値とも独立して動作し、
    //!           後着優先で有効になります。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、トランスポーズの値を変更します。
    //!           例えば、トラック 2 とトラック 5 のトランスポーズの値を変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           トランスポーズの値として、@ref TransposeMin ～ @ref TransposeMax
    //!           の値を設定することができます。
    //!           それ以外の値を指定すると、
    //!           (Debug 版、Development 版) アサートで停止します。
    //!           (Release 版) false を返します。
    //!
    //!           シーケンス内に transpose コマンドを入れていた場合、
    //!           本関数での設定と transpose コマンドは、
    //!           後から設定されたものが有効になります。
    //!
    //! @param[in] trackBitFlag トラックごとのビットフラグです。
    //! @param[in] transpose    トランスポーズの値です。
    //!
    //! @return   transpose が @ref TransposeMin ～ @ref TransposeMax
    //!           の範囲内の場合は true を返します。範囲外の場合は false を返します
    //!           (Release 版のみ。Debug 版、Development 版はアサートで停止します)。
    //!
    //! @see TransposeMin
    //! @see TransposeMax
    //!
    //---------------------------------------------------------------------------
    bool SetTrackTranspose( uint32_t trackBitFlag, int8_t transpose ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( transpose >= TransposeMin && transpose <= TransposeMax );
        if ( transpose < TransposeMin || transpose > TransposeMax )
        {
            return false;
        }

        if ( IsAttachedSound() )
        {
            m_pSound->SetTrackTranspose( trackBitFlag, transpose );
        }
        return true;
    }

    //---------------------------------------------------------------------------
    //! @brief    シーケンスサウンドのトラックのベロシティレンジの値を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドのシーケンストラックの
    //!           ベロシティレンジの値を変更します。
    //!           ハンドルが無効の場合は、何もしません。
    //!
    //!           この関数で指定する値は、他のどのベロシティレンジの値とも独立して動作し、
    //!           後着優先で有効になります。
    //!
    //!           trackBitFlag で、設定するトラックを指定します。
    //!           下位ビットから順にトラック 0、トラック 1、トラック 2 … を表し、
    //!           ビットが立っているトラックすべてに対して、ベロシティレンジの値を変更します。
    //!           例えば、トラック 2 とトラック 5 のベロシティレンジを変更したい場合は
    //!           (1 << 2) | (1 << 5) すなわち、0x0024 とします。
    //!
    //!           ベロシティレンジの値として、@ref VelocityRangeMin ～ @ref VelocityRangeMax
    //!           の値を設定することができます。
    //!           それ以外の値を指定すると、
    //!           (Debug 版、Development 版) アサートで停止します。
    //!           (Release 版) false を返します。
    //!
    //!           シーケンス内に velocity_range コマンドを入れていた場合、
    //!           本関数での設定と velocity_range コマンドは、
    //!           後から設定されたものが有効になります。
    //!
    //! @param[in] trackBitFlag トラックごとのビットフラグです。
    //! @param[in] range        ベロシティレンジの値です。
    //!
    //! @return   velocity_range が @ref VelocityRangeMin ～ @ref VelocityRangeMax
    //!           の範囲内の場合は true を返します。範囲外の場合は false を返します
    //!           (Release 版のみ。Debug 版、Development 版はアサートで停止します)。
    //!
    //! @see VelocityRangeMin
    //! @see VelocityRangeMax
    //---------------------------------------------------------------------------
    bool SetTrackVelocityRange( uint32_t trackBitFlag, uint8_t range ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( range <= VelocityRangeMax );
        if ( range > VelocityRangeMax )
        {
            return false;
        }

        if ( IsAttachedSound() )
        {
            m_pSound->SetTrackVelocityRange( trackBitFlag, range );
        }
        return true;
    }

    //---------------------------------------------------------------------------
    //! @brief  シーケンスサウンドのトラックの出力先を変更します。
    //!
    //!         ハンドルに関連付けられたサウンドのシーケンストラックの出力先を変更します。
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
    //!         初期状態では、@ref nn::atk::OutputLine_Main のみが指定されています。
    //!
    //! @param[in] trackBitFlag   トラックごとのビットフラグです。
    //! @param[in] lineFlag       出力先のビットフラグです。
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
    //! @brief  シーケンスサウンドのトラックの出力先の設定を無効にします。
    //!
    //!         ハンドルに関連付けられたサウンドのシーケンストラックの出力先の設定を無効にします。
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

    //---------------------------------------------------------------------------
    //! @brief    出力先別のトラックのパン (左右の定位) を変更します。
    //!
    //!           ハンドルに関連付けられたサウンドの
    //!           メイン (TV) に対するシーケンストラックのパンを変更します。
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
    //---------------------------------------------------------------------------
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
    void SetTrackOutputFxSend( OutputDevice device, uint32_t trackBitFlag, AuxBus bus, float send ) NN_NOEXCEPT
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

    // -----------------------------------------------------------------
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

    // -----------------------------------------------------------------
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
        if ( IsAttachedSound() ) m_pSound->SetId( id );
    }

    //---------------------------------------------------------------------------
    //! @brief    サウンドの ID を取得します。
    //!
    //!           ハンドルに関連付けられたサウンドに設定されている ID を取得します。
    //!           ハンドルが無効の場合は nn::atk::InvalidSoundId を返します。
    //!
    //!           この関数で取得できる ID は @ref SetId で設定された ID です。
    //!
    //! @return   サウンドに設定されている ID を返します。
    //!
    //---------------------------------------------------------------------------
    uint32_t GetId() const NN_NOEXCEPT
    {
        if ( IsAttachedSound() ) return m_pSound->GetId();
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
    //! @brief  再生開始からのティック数を取得します。
    //!
    //!         サウンドハンドルが無効の場合は、0 を返します。
    //!
    //!         ティック数のカウンタが 32 ビットを越えると、
    //!         関数が返す値は 0 に戻ります。
    //!
    //!         ティックとは時間を表現する単位で、タイムベースを基準としています
    //!         (タイムベースについては、
    //!         シーケンスデータマニュアルの「タイムベース」の章をご覧ください)。
    //!         たとえば、タイムベースが 48 の場合は、四分音符の長さが 48 ティックに相当します。
    //!
    //! @return   再生開始からのティック数を返します。
    //---------------------------------------------------------------------------
    uint32_t GetTick() const NN_NOEXCEPT { return IsAttachedSound() ? m_pSound->GetTick() : 0; }
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
    //! @brief    波形の全トラック、全チャンネルに同一のサラウンドボリュームを設定します。
    //! @param param :private
    void SetTrackChannelMixParameter( const MixParameter& param ) NN_NOEXCEPT
    {
        if( IsAttachedSound() )
        {
            // 全トラック、全チャンネルで同じ設定を使用
            for ( uint32_t srcChNo = 0; srcChNo < WaveChannelMax; srcChNo++)
            {
                m_pSound->SetTrackChannelMixParameter( detail::driver::SequenceSoundPlayer::AllTrackBitFlag, srcChNo, param );
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
    //! @brief    出力先指定で、波形の全トラック、全チャンネルに同一のサラウンドボリュームを設定します。
    //! @param device :private
    //! @param param :private
    void SetTrackChannelOutputMixParameter( OutputDevice device, const MixParameter& param ) NN_NOEXCEPT
    {
        if( IsAttachedSound() )
        {
            // 全トラック、全チャンネルで同じ設定を使用
            switch (device)
            {
            case OutputDevice_Main:
                for ( uint32_t srcChNo = 0; srcChNo < WaveChannelMax; srcChNo++)
                {
                    m_pSound->SetTrackChannelMixParameter( detail::driver::SequenceSoundPlayer::AllTrackBitFlag, srcChNo, param );
                }
                break;
            default:
                NN_SDK_ASSERT( false, "Invalid device\n" );
                break;
            }
        }
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
    //! @param sound :private
    void detail_AttachSoundAsTempHandle( detail::SequenceSound* sound ) NN_NOEXCEPT;

    //! @internal
    //! @return :private
    detail::SequenceSound* detail_GetAttachedSound() NN_NOEXCEPT { return m_pSound; }

    //! @internal
    //! @return :private
    const detail::SequenceSound* detail_GetAttachedSound() const NN_NOEXCEPT { return m_pSound; }

private:
    NN_DISALLOW_COPY( SequenceSoundHandle );

    detail::SequenceSound* m_pSound;
};

} // namespace nn::atk
} // namespace nn

