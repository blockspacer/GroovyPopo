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
 * :include nn/atk/atk_SoundActor.h
 *
 * @file atk_SoundActor.h
 */

#pragma once

#include <nn/atk/atk_SoundStartable.h>
#include <nn/atk/atk_ExternalSoundPlayer.h>
#include <nn/atk/atk_BasicSound.h>          // SoundActorParam

namespace nn {
namespace atk {

class SoundHandle;
class SoundArchivePlayer;

//---------------------------------------------------------------------------
//! @brief    複数のサウンドをまとめて再生管理するためのクラスです。
//!
//!           サウンドアクターは、
//!           ゲーム内でキャラクターやモデル等に関連付けて使用されることが想定されています。
//!           例えばキャラクタの移動にあわせて、
//!           そのキャラクタに関連付けられたサウンドアクターの音の定位を変更する、
//!           といった使い方が可能です。
//!
//!           サウンドアクターに設定するパラメータは、
//!           そのサウンドアクターで再生するすべてのサウンドに対して効果があります。
//!
//!           サウンドアクターは内部に 4 つのアクタープレイヤーを持っています。
//!           このアクタープレイヤーごとに同時に再生できるサウンド数を制限することができます。
//!           設定したサウンド数を超えた場合には、
//!           各サウンドに設定されたプレイヤープライオリティを比較して、
//!           もっとも優先度が低いサウンドが停止します。
//!           同時に再生可能なサウンド数の初期値は下記のように設定されています。
//!
//!             - アクタープレイヤー 0 番は無制限
//!             - その他は 1
//!
//!           サウンドがどのアクタープレイヤーで再生されるかは、
//!           サウンドデータに設定されています。
//!           この情報は再生時に @ref nn::atk::SoundStartable::StartInfo を用いて、
//!           上書きすることができます。
//!
//!         @n
//!
//!         ■ 出力先別パラメータ
//!
//!         SoundActor では、SoundActor 経由で再生するサウンドに関して、
//!         下記の影響を与えることができます。
//!
//!         (全体)
//!         - ピッチ ... @ref SetPitch
//!
//!         (TV 出力向け)
//!         - 音量 ... @ref SetVolume, @ref StopAllSound, @ref PauseAllSound
//!         - パン ... @ref SetOutputPan
//!
//!         @n
//!
//!         各パラメータは、上記の影響の他に、下記の影響を受けます。
//!         - SoundMaker で指定したデータの影響
//!         - @ref SoundHandle や @ref SoundPlayer による影響
//!         - 3D サウンド計算 (@ref Sound3DEngine) による影響
//!
//! @see SoundStartable クラス
//---------------------------------------------------------------------------
class SoundActor : public SoundStartable
{
public:
    typedef detail::ExternalSoundPlayer ActorPlayer;

    //---------------------------------------------------------------------------
    //! @brief    アクタープレイヤーの保持数です。
    //!
    //---------------------------------------------------------------------------
    static const int ActorPlayerCount = 4;

    //----------------------------------------
    //! @name コンストラクタ/デストラクタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //!           初期化時には、プレイヤーの音量が 1.0、ピッチが 1.0、パンが 0.0、
    //!           同時に再生可能なサウンド数が 1 に設定されます。
    //!
    //!           引数付きのコンストラクタの場合は、
    //!           @ref Initialize を呼ぶ必要はありません
    //!           (呼んでも問題ありません)。
    //!
    //! @param[in] pSoundArchivePlayer 再生に使用するサウンドアーカイブプレイヤーです。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    explicit SoundActor( SoundArchivePlayer* pSoundArchivePlayer ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    コンストラクタです。
    //!
    //!           初期化時には、プレイヤーの音量が 1.0、ピッチが 1.0、パンが 0.0、
    //!           同時に再生可能なサウンド数が 1 に設定されます。
    //!
    //!           引数付きのコンストラクタの場合は、
    //!           @ref Initialize を呼ぶ必要はありません
    //!           (呼んでも問題ありません)。
    //!
    //! @param[in] pSoundArchivePlayer 再生に使用するサウンドアーカイブプレイヤーです。
    //! @param[in] pActorPlayer 再生に使用するアクタープレイヤーです。
    //!                         設定したアクタープレイヤーは 0 番のアクタープレイヤーとして使用されます。
    //!                         NULL を設定した場合はデフォルトのアクタープレイヤーが使用されます。
    //!
    //!                         SoundActor::ActorPlayer を継承し、CanPlaySound で再生制御の処理を定義した
    //!                         ユーザ定義のアクタープレイヤーを設定してください。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    SoundActor( SoundArchivePlayer* pSoundArchivePlayer, ActorPlayer* pActorPlayer ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //!           初期化時には、プレイヤーの音量が 1.0、ピッチが 1.0、パンが 0.0、
    //!           同時に再生可能なサウンド数が 1 に設定されます。
    //!
    //!           引数なしのコンストラクタは、@ref Initialize を呼ぶ必要があります。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    SoundActor() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    デストラクタです。
    //!
    //!           内部で、@ref Finalize を呼んでいます。
    //!
    //! @see Finalize
    //!
    //---------------------------------------------------------------------------
    virtual ~SoundActor() NN_NOEXCEPT NN_OVERRIDE;
    //@}

    //----------------------------------------
    //! @name 初期化
    //@{
    //---------------------------------------------------------------------------
    //! @brief    サウンドアクターを初期化します。
    //!
    //!           サウンドアクターを使用する前に、初期化を行う必要があります。
    //!           ただし、引数付きコンストラクタを呼んでいる場合は、
    //!           呼ぶ必要はありません (呼んでも問題ありません)。
    //!
    //!           Sound3DActor の初期化には、本関数ではなく、
    //!           @ref SoundArchivePlayer と @ref Sound3DManager を引数に取る、
    //!           @ref Sound3DActor::Initialize を呼ぶ必要があります。
    //!
    //!           同じ SoundActor インスタンスを再利用するには、
    //!           あらかじめ @ref Finalize 関数を呼んでから、
    //!           再度、本関数を呼び出してください。
    //!
    //! @param[in] pSoundArchivePlayer 再生に使用するサウンドアーカイブプレイヤーです。
    //!
    //! @see SoundArchivePlayer クラス
    //! @see SoundActor()
    //! @see Finalize
    //!
    //---------------------------------------------------------------------------
    void Initialize( SoundArchivePlayer* pSoundArchivePlayer ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    サウンドアクターを初期化します。
    //!           サウンドアクターを使用する前に、初期化を行う必要があります。
    //!           ただし、引数付きコンストラクタを呼んでいる場合は、
    //!           呼ぶ必要はありません (呼んでも問題ありません)。
    //!
    //!           Sound3DActor の初期化には、本関数ではなく、
    //!           @ref SoundArchivePlayer と @ref Sound3DManager と ActorPlayer を引数に取る、
    //!           @ref Sound3DActor::Initialize を呼ぶ必要があります。
    //!
    //!           同じ SoundActor インスタンスを再利用するには、
    //!           あらかじめ @ref Finalize 関数を呼んでから、
    //!           再度、本関数を呼び出してください。
    //!
    //! @param[in] pSoundArchivePlayer 再生に使用するサウンドアーカイブプレイヤーです。
    //! @param[in] pActorPlayer 再生に使用するアクタープレイヤーです。
    //!                         設定したアクタープレイヤーは 0 番のアクタープレイヤーとして使用されます。
    //!                         NULL を設定した場合はデフォルトのアクタープレイヤーが使用されます。
    //!
    //!                         SoundActor::ActorPlayer を継承し、CanPlaySound で再生制御の処理を定義した
    //!                         ユーザ定義のアクタープレイヤーを設定してください。
    //!
    //! @see SoundArchivePlayer クラス
    //! @see SoundActor()
    //! @see Finalize
    //!
    //---------------------------------------------------------------------------
    void Initialize( SoundArchivePlayer* pSoundArchivePlayer, ActorPlayer* pActorPlayer ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    サウンドアクターを破棄します。
    //!
    //!           デストラクタの中で呼ばれますが、
    //!           明示的に呼んでも問題ありません。
    //!
    //!           同じ SoundActor インスタンスを再利用するには、
    //!           本関数を呼んだ後、再度 @ref Initialize 関数を呼び出してください。
    //!
    //! @see ~SoundActor
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;
    //@}

    //----------------------------------------
    //! @name 再生
    //@{
public:
    //---------------------------------------------------------------------------
    //! @brief    アクターで再生中のすべてのサウンドを停止します。
    //!
    //!           各サウンドに対して、ハンドルクラスを通して停止させたときと同様の処理を行います。
    //!
    //!           fadeFrames で指定したフレーム数をかけてフェードアウトさせることができます。
    //!           0 を指定した場合は、フェードアウトを行いません。
    //!           ただし、シーケンスサウンドで発音中の音は、
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
    //---------------------------------------------------------------------------
    void StopAllSound( int fadeFrames ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    アクターで再生中のすべてのサウンドを一時停止・再開します。
    //!
    //!           各サウンドに対して、ハンドルクラスを通して一時停止・
    //!           再開させたときと同様の処理を行います。
    //!
    //!           一時停止・再開時のフェードは、再生開始時のフェードイン、
    //!           停止時のフェードアウトとは独立してはたらきます。
    //!
    //! @param[in] flag   true なら一時停止、false なら再開します。
    //! @param[in] fadeFrames   フェードイン・フェードアウトにかけるフレーム数です。
    //!                         フレーム数は @ref SoundArchivePlayer::Update
    //!                         の呼び出し回数で換算されます。
    //!                         負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //---------------------------------------------------------------------------
    void PauseAllSound( bool flag, int fadeFrames ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    アクターで再生中のすべてのサウンドを一時停止・再開します。
    //!
    //!           各サウンドに対して、ハンドルクラスを通して一時停止・
    //!           再開させたときと同様の処理を行います。
    //!
    //!           一時停止・再開時のフェードは、再生開始時のフェードイン、
    //!           停止時のフェードアウトとは独立してはたらきます。
    //!
    //! @param[in] flag   true なら一時停止、false なら再開します。
    //! @param[in] fadeFrames     フェードイン・フェードアウトにかけるフレーム数です。
    //!                           フレーム数は @ref SoundArchivePlayer::Update
    //!                           の呼び出し回数で換算されます。
    //!                           負値を指定した場合は、0 を指定した場合と同じ挙動になります。
    //! @param[in] pauseMode      ポーズの設定です。
    //---------------------------------------------------------------------------
    void PauseAllSound(bool flag, int fadeFrames, PauseMode pauseMode) NN_NOEXCEPT;

protected:
    //---------------------------------------------------------------------------
    //! @brief  サウンドを再生する際に呼び出される仮想関数です。
    //!
    //!         この関数をオーバーライドすることにより、
    //!         サウンドを再生するときの処理をカスタマイズすることができます。
    //!
    //!         setupArg は @ref SoundActor::SetupSound を呼び出す際に、
    //!         そのまま引数として渡します。
    //!
    //!         例えば、継承したクラスで以下のように記述すると、
    //!         任意の ID のサウンドを再生することができます。
    //!
    //! @code
    //! SoundStartable::StartResult MyActor::SetupSound(
    //!     SoundHandle* handle,
    //!     uint32_t soundId,
    //!     const StartInfo* startInfo,
    //!     void* setupArg
    //! )
    //! {
    //!     uint32_t newId;
    //!     // ここでnewIdに別の音を設定する
    //!
    //!     // SetupSoundを呼び出して音を再生
    //!     return SoundActor::SetupSound(
    //!         handle,
    //!         newId,
    //!         startInfo,
    //!         setupArg // この引数はそのまま渡します
    //!     );
    //! }
    //! @endcode
    //!
    //! @param[in] handle     再生するサウンドと関連付けられるハンドルです。
    //! @param[in] soundId    再生するサウンドの ID です。
    //! @param[in] startInfo  詳細な再生パラメータです。
    //! @param[in] setupArg   セットアップ時に使用されるパラメータです。
    //!
    //! @return   再生処理の結果を @ref SoundStartable::StartResult 型で返します。
    //!
    //! @see SoundHandle クラス
    //! @see SoundStartable::StartResult
    //! @see SoundStartable::StartInfo
    //---------------------------------------------------------------------------
    virtual SoundStartable::StartResult SetupSound(
        SoundHandle* handle,
        uint32_t soundId,
        const SoundStartable::StartInfo* startInfo,
        void* setupArg
    ) NN_NOEXCEPT;

    //! @briefprivate
    //! @param handle :private
    //! @param soundId :private
    //! @param soundArchiveName :private
    //! @param startInfo :private
    //! @param setupArg :private
    //! @return :private
    virtual SoundStartable::StartResult SetupSound(
        SoundHandle* handle,
        uint32_t soundId,
        const char* soundArchiveName,
        const SoundStartable::StartInfo* startInfo,
        void* setupArg
    ) NN_NOEXCEPT;
    //@}

public:
    //----------------------------------------
    //! @name サウンドパラメータ
    //@{

    //---------------------------------------------------------------------------
    //! @brief    アクターの音量を変更します。
    //!
    //!           アクターの音量は、アクターで再生するすべてのサウンドに対して効果があります。
    //!
    //!           この関数で指定する値は、他のどの音量パラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           音量 volume は、0.0 から 1.0 の倍率で指定します。
    //!           すなわち、1.0 を指定すると音量に影響を与えません。
    //!           0.0 を指定すると発音されなくなります。デフォルト値は 1.0 です。
    //!
    //! @param[in] volume 変更する音量の倍率 ( 0.0 ～ 1.0 ) です。
    //!
    //! @see GetVolume
    //! @see SetMainOutVolume
    //!
    //---------------------------------------------------------------------------
    void SetVolume( float volume ) NN_NOEXCEPT { m_ActorParam.volume = volume; }

    //---------------------------------------------------------------------------
    //! @brief    アクターに設定されている音量を取得します。
    //!
    //! @return   現在の音量を返します。
    //!
    //! @see SetVolume
    //!
    //---------------------------------------------------------------------------
    float GetVolume() const NN_NOEXCEPT { return m_ActorParam.volume; }

    //---------------------------------------------------------------------------
    //! @brief    アクターの音程を変更します。
    //!
    //!           アクターの音程は、アクターで再生するすべてのサウンドに対して効果があります。
    //!
    //!           この関数で指定する値は、他のどの音程パラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           音程 pitch は、周波数の比率で指定します。
    //!           すなわち、1.0 を指定すると音程に影響を与えません。
    //!           2.0 を指定すると再生される周波数が 2 倍になり、1 オクターブ高い音程になります。
    //!           0.5 を指定すると 1 オクターブ低い音程になります。
    //!           デフォルト値は 1.0 です。
    //!
    //!           他の音程パラメータと重ね合わされたあと、
    //!           最終的な音程は nn::audio::VoiceType::GetPitchMin() ～ nn::audio::VoiceType::GetPitchMax() の範囲でクランプされます。
    //!           音程の重ね合わせの詳細につきましては @confluencelink{83955829, サウンドパラメータ制御の動作} を参照ください。
    //!
    //! @param[in] pitch 変更する音程の周波数比率です。
    //!
    //! @see GetPitch
    //!
    //---------------------------------------------------------------------------
    void SetPitch( float pitch ) NN_NOEXCEPT { m_ActorParam.pitch = pitch; }

    //---------------------------------------------------------------------------
    //! @brief    アクターに設定されている音程を取得します。
    //!
    //! @return   現在の音程を返します。
    //!
    //! @see SetPitch
    //!
    //---------------------------------------------------------------------------
    float GetPitch() const NN_NOEXCEPT { return m_ActorParam.pitch; }

    //---------------------------------------------------------------------------
    //! @brief  アクターのローパスフィルタカットオフ値を変更します。
    //!
    //!         アクターのローパスフィルタは、
    //!         アクターで再生中のすべてのサウンドに対して効果があります。
    //!
    //!         この関数で指定する値は、他のどのローパスフィルタカットオフパラメータとも独立して動作し、
    //!         それらはすべて重ね合わされます。
    //!
    //!         lowPassFilterFrequency は、カットオフの相対変化の値を指定します。
    //!         0.0 を指定するとカットオフの値を変更しません。
    //!         -1.0 を指定すると、フィルタがかかっていない状態から、
    //!         もっともフィルタがかかっている状態（カットオフ周波数が下がる方向）に変更します。
    //!
    //! @param[in] lowPassFilterFrequency    0.0 を基準としたフィルタカットオフの相対変化の値です。
    //!
    //! @see GetLowPassFilterFrequency
    //---------------------------------------------------------------------------
    void SetLowPassFilterFrequency( float lowPassFilterFrequency ) NN_NOEXCEPT { m_ActorParam.lpf = lowPassFilterFrequency; }

    //---------------------------------------------------------------------------
    //! @brief    アクターに設定されているローパスフィルタカットオフ値を取得します。
    //!
    //! @return   現在のローパスフィルタカットオフ値を返します。
    //!
    //! @see SetLpfFrequency
    //---------------------------------------------------------------------------
    float GetLowPassFilterFrequency() const NN_NOEXCEPT { return m_ActorParam.lpf; }

    //---------------------------------------------------------------------------
    //! @brief  アクターの Biquad フィルタの種類と係数を変更します。
    //!
    //!         アクターの Biquad フィルタは、
    //!         アクターで再生中のすべてのサウンドに対して効果があります。
    //!
    //!         Biquad フィルタは複数の箇所での設定が重ね合わされず、優先度に従って設定されます。
    //!
    //!         フィルタの種類 type は @ref BiquadFilterType の値を使用します。
    //!         プリセットで用意されているフィルタの種類の他、
    //!         ユーザーが登録したフィルタを選択することができます。
    //!
    //!         type は BiquadFilterType_Min ～ BiquadFilterType_Max の値を指定します。
    //!         BiquadFilterType_Min、BiquadFilterType_Max については @ref BiquadFilterType をご参照ください。
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
    //---------------------------------------------------------------------------
    void SetBiquadFilter( int type, float value ) NN_NOEXCEPT {
        m_ActorParam.biquadFilterType = type;
        m_ActorParam.biquadFilterValue = value;
    }

    //---------------------------------------------------------------------------
    //! @brief    アクターに設定されている Biquad フィルタの種類を取得します。
    //!
    //! @return   現在の Biquad フィルタの種類を返します。
    //!
    //! @see BiquadFilterType
    //! @see SetBiquadFilter
    //---------------------------------------------------------------------------
    int GetBiquadFilterType() const NN_NOEXCEPT { return m_ActorParam.biquadFilterType; }

    //---------------------------------------------------------------------------
    //! @brief    アクターに設定されている Biquad フィルタの係数を取得します。
    //!
    //! @return   現在の Biquad フィルタの係数を返します。
    //!
    //! @see SetBiquadFilter
    //---------------------------------------------------------------------------
    float GetBiquadFilterValue() const NN_NOEXCEPT { return m_ActorParam.biquadFilterValue; }
    //@}

    //----------------------------------------
    //! @name 出力先別サウンドパラメータ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    指定した出力先に対するアクターの音量を変更します。
    //!
    //!           この関数で指定する値は、他のどの音量パラメータとも独立して動作し、
    //!           それらはすべて重ね合わされます。
    //!
    //!           音量 volume は、0.0 から 1.0 の倍率で指定します。
    //!           すなわち、1.0 を指定すると音量に影響を与えません。
    //!           0.0 を指定すると発音されなくなります。デフォルト値は 1.0 です。
    //!
    //! @param[in] device 出力先のデバイスです。
    //! @param[in] volume 変更する音量の倍率 ( 0.0 ～ 1.0 ) です。
    //!
    //! @see GetOutputVolume
    //! @see SetVolume
    //---------------------------------------------------------------------------
    void SetOutputVolume( OutputDevice device, float volume ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE( device, 0, OutputDevice_Count );
        switch (device)
        {
        case nn::atk::OutputDevice_Main:
            m_ActorParam.tvVolume = volume;
            break;
        default:
            NN_SDK_ASSERT( false, "Invalid device\n" );
            break;
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  指定した出力先の、アクターに設定されている音量を取得します。
    //!
    //! @param[in] device 出力先のデバイスです。
    //!
    //! @return 指定した出力先に対する音量を返します。
    //!         出力先のデバイスが存在しない場合、0.0f を返します。
    //!
    //! @see SetOutputVolume
    //! @see GetVolume
    //---------------------------------------------------------------------------
    float GetOutputVolume( OutputDevice device ) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE( device, 0, OutputDevice_Count );
        switch (device)
        {
        case nn::atk::OutputDevice_Main:
            return m_ActorParam.tvVolume;
        default:
            NN_SDK_ASSERT( false, "Invalid device\n" );
            break;
        }
        return 0.0f;
    }

    //---------------------------------------------------------------------------
    //! @brief    指定した出力先に対するアクターのパン (左右の定位) を変更します。
    //!
    //!           アクターのパンは、アクターで再生するすべてのサウンドに対して効果があります。
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
    //! @param[in] device 出力先のデバイスです。
    //! @param[in] pan    0.0 を基準としたパンの相対変化の値です。
    //!
    //! @see GetOutputPan
    //---------------------------------------------------------------------------
    void SetOutputPan( OutputDevice device, float pan ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE( device, 0, OutputDevice_Count );
        switch (device)
        {
        case nn::atk::OutputDevice_Main:
            m_ActorParam.tvPan = pan;
            break;
        default:
            NN_SDK_ASSERT( false, "Invalid device\n" );
            break;
        }
    }

    //---------------------------------------------------------------------------
    //! @brief    指定した出力先の、アクターに設定されているパンを取得します。
    //!
    //! @param[in] device 出力先のデバイスです。
    //!
    //! @return   指定した出力先に対する、現在のパンを返します。
    //!           出力先のデバイスが存在しない場合、0.0f を返します。
    //!
    //! @see SetOutputPan
    //---------------------------------------------------------------------------
    float GetOutputPan( OutputDevice device ) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE( device, 0, OutputDevice_Count );
        switch (device)
        {
        case nn::atk::OutputDevice_Main:
            return m_ActorParam.tvPan;
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
    //! @brief    アクタープレイヤーで現在再生中のサウンドの個数を取得します。
    //!
    //! @param[in] actorPlayerId アクタープレイヤー番号です。
    //!
    //! @return   アクタープレイヤーで再生中のサウンド数を返します。
    //!
    //---------------------------------------------------------------------------
    int GetPlayingSoundCount( int actorPlayerId ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    同時に再生可能なサウンド数を設定します。
    //!
    //!           サウンドアクターは、アクター内に 4 個のアクタープレイヤーを持っています。
    //!           このアクタープレイヤーごとに、同時に再生可能なサウンド数制限を設定することができます。
    //!           この設定は、サウンドプレイヤーで設定されている同時再生サウンド数とは別に、
    //!           独立してはたらきます。
    //!
    //!           設定したサウンド数を超えるサウンドを再生しようとすると、
    //!           各サウンドに設定されたプレイヤープライオリティを比較して、
    //!           もっとも優先度が低いサウンドが停止します。
    //!
    //!           初期値はアクタープレイヤー 0 番は無制限で、その他は 1 に設定されています。
    //!
    //! @param[in] actorPlayerId  アクタープレイヤー番号です。
    //! @param[in] count          同時に再生可能なサウンド数です。
    //!
    //! @see GetPlayableSoundCount
    //!
    //---------------------------------------------------------------------------
    void SetPlayableSoundCount( int actorPlayerId, int count ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    アクタープレイヤーで同時に再生可能なサウンド数を取得します。
    //!
    //! @param[in] actorPlayerId  アクタープレイヤー番号です。
    //!
    //! @return   アクタープレイヤーで同時に再生可能なサウンド数を返します。
    //!
    //! @see SetPlayableSoundCount
    //!
    //---------------------------------------------------------------------------
    int GetPlayableSoundCount( int actorPlayerId ) const NN_NOEXCEPT;
    //@}

    //----------------------------------------
    //! @name その他
    //@{
    //---------------------------------------------------------------------------
    //! @brief    アクターで再生中のすべてのサウンドに対して処理を行います。
    //!
    //!           @ref ForEachSound のFunctionの参照渡し版となります。
    //!
    //! @tparam Function 関数ポインタ、または関数オブジェクトの型です。
    //!
    //! @param[in] function   関数ポインタ、または関数オブジェクトです。
    //! @param[in] reverse    処理順を逆にする場合は true を指定します。
    //---------------------------------------------------------------------------
    template< class Function >
    void ForEachSoundReference( Function& function, bool reverse = false ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    アクターで再生中のすべてのサウンドに対して処理を行います。
    //!
    //!           アクターで再生中のすべてのサウンドに対して、
    //!
    //! @code
    //! function( nn::atk::SoundHandle& handle )
    //! @endcode
    //!
    //!           を呼び出します。
    //!
    //!           function には、再生中のサウンドに関連付けられたサウンドハンドル
    //!           handle が渡されます。これは一時的なハンドルですので、
    //!           ハンドルを後で使用することはできません。
    //!
    //!           関数 function は、サウンドの再生が古い順に呼び出されます。
    //!           reverse に true を指定すると、サウンドの再生が新しい順に呼び出されます。
    //!
    //!           function には関数ポインタ、または関数オブジェクトを渡します。
    //!
    //! @tparam Function 関数ポインタ、または関数オブジェクトの型です。
    //!
    //! @param[in] function   関数ポインタ、または関数オブジェクトです。
    //! @param[in] reverse    処理順を逆にする場合は true を指定します。
    //!
    //! @return   引数に指定された関数ポインタ、または関数オブジェクトです。
    //!
    //---------------------------------------------------------------------------
    template< class Function >
    Function ForEachSound( Function function, bool reverse = false ) NN_NOEXCEPT;
    //@}

    //-----------------------------------------------------------------------------
    //  detail functions

    //! @internal
    //! @param actorPlayerId :private
    //! @return :private
    ActorPlayer* detail_GetActorPlayer( int actorPlayerId ) NN_NOEXCEPT {
        if ( actorPlayerId < 0 || ActorPlayerCount <= actorPlayerId ) return NULL;
        return m_pActorPlayer[actorPlayerId];
    }

    //! @internal
    //! @return :private
    const detail::SoundActorParam& detail_GetActorParam() const NN_NOEXCEPT { return m_ActorParam; }

    //! @internal
    //! @param handle :private
    //! @param soundId :private
    //! @param soundArchiveName :private
    //! @param startInfo :private
    //! @param ambientInfo :private
    //! @param setupArg :private
    //! @return :private
    virtual SoundStartable::StartResult detail_SetupSoundWithAmbientInfo(
        SoundHandle* handle,
        uint32_t soundId,
        const char* soundArchiveName,
        const SoundStartable::StartInfo* startInfo,
        detail::BasicSound::AmbientInfo* ambientInfo,
        void* setupArg
    ) NN_NOEXCEPT;

private:
    virtual SoundStartable::StartResult detail_SetupSound(
        SoundHandle* handle,
        uint32_t soundId,
        bool holdFlag,
        const char* soundArchiveName,
        const SoundStartable::StartInfo* startInfo
    ) NN_NOEXCEPT NN_OVERRIDE;
    virtual SoundArchive::ItemId detail_GetItemId( const char* pString ) NN_NOEXCEPT NN_OVERRIDE;

    virtual SoundArchive::ItemId detail_GetItemId( const char* pString, const char* soundArchiveName ) NN_NOEXCEPT NN_OVERRIDE;

    struct SetupInfo
    {
        bool holdFlag;
    };

    SoundArchivePlayer* m_pSoundArchivePlayer;
    ActorPlayer m_ActorPlayer[ ActorPlayerCount ];
    ActorPlayer* m_pActorPlayer[ ActorPlayerCount ];
    detail::SoundActorParam m_ActorParam;

    bool m_IsInitialized;
    bool m_IsFinalized;
};

template< class Function >
inline void SoundActor::ForEachSoundReference( Function& function, bool reverse ) NN_NOEXCEPT
{
    for ( int actorPlayerIndex = 0; actorPlayerIndex < ActorPlayerCount; actorPlayerIndex++ )
    {
        NN_SDK_ASSERT_NOT_NULL( m_pActorPlayer[ actorPlayerIndex ] );
        m_pActorPlayer[ actorPlayerIndex ]->ForEachSoundRef<Function>( function, reverse );
    }
}

template< class Function >
inline Function SoundActor::ForEachSound( Function function, bool reverse ) NN_NOEXCEPT
{
    for ( int actorPlayerIndex = 0; actorPlayerIndex < ActorPlayerCount; actorPlayerIndex++ )
    {
        NN_SDK_ASSERT_NOT_NULL( m_pActorPlayer[ actorPlayerIndex ] );
        m_pActorPlayer[ actorPlayerIndex ]->ForEachSound<Function>( function, reverse );
    }

    return function;
}

} // namespace nn::atk
} // namespace nn

