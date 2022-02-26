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
 * :include nn/atk/atk_AnimSound.h
 *
 * @file atk_AnimSound.h
 */

#pragma once

#include <nn/atk/atk_AnimSoundImpl.h>
#include <nn/atk/atk_AnimEventPlayer.h>

namespace nn {
namespace atk {

class SoundStartable;
class SoundArchive;

//---------------------------------------------------------------------------
//! @brief  アニメーションサウンドを再生させるテンプレートクラスです。
//!
//!         アニメーションサウンドとは、アニメーションサウンドデータ (*.bfasd ファイル)
//!         を使うことで、アニメーションの再生に合わせて自動的にサウンドが再生される機能です。
//!
//!         アニメーションサウンドデータには、サウンドのラベル文字列と、
//!         そのサウンドを再生するフレーム位置が定義されたイベントが含まれています。
//!         @ref UpdateFrame を呼び出しアニメーションサウンドのフレーム位置を進めると、
//!         通過したフレーム位置にあるイベントのサウンドが再生されます。
//!
//!         アニメーションサウンドでは、ラベル文字列を使用してサウンドを再生します。
//!         そのため、アニメーションサウンドで再生するサウンドを、
//!         文字列で再生できる状態にしておく必要があります。
//!         FS サウンドアーカイブを使用している場合は、
//!         サウンドのラベル文字列変換テーブルとサウンドデータが、
//!         メモリ上にロードされている必要があります。
//!         ラベル文字列変換テーブルのロードには
//!         @ref FsSoundArchive::LoadLabelStringData を呼び出します。
//!
//!         テンプレート引数 EventPlayerCount は、
//!         同時に扱うことができるイベントの上限数が書きこまれます。
//!         イベントとは、AnimSoundMaker でアニメーションサウンドを編集する画面における
//!         リストの「各行」が該当します。
//!
//!         イベントの上限数以上のイベントを扱おうとした場合、
//!         実行中のイベントで再生しているサウンドのうち、
//!         一番プレイヤー優先度の低いサウンドを再生しているイベントが停止され、
//!         新しいイベントが実行されます。
//!
//!         通常、メモリが足りないなどの問題が無い場合は、
//!         @ref AnimSound クラスをご利用ください。
//!
//! @tparam EventPlayerCount    同時に扱うことのできるるイベントの上限数です。
//!
//! @see AnimSound クラス
//---------------------------------------------------------------------------
template< int EventPlayerCount >
class TAnimSound
{
public:
    //---------------------------------------------------------------------------
    //! @brief  再生方向を表す列挙型です。
    //!
    //!         @ref UpdateFrame でアニメーションサウンドを再生する際の、
    //!         再生方向を表現します。
    //!
    //! @see UpdateFrame
    //---------------------------------------------------------------------------
    enum PlayDirection
    {
        //! 順方向への再生を表します。
        PlayDirection_Forward  = detail::AnimSoundImpl::PlayDirection_Forward,
        //! 逆方向への再生を表します。
        PlayDirection_Backward = detail::AnimSoundImpl::PlayDirection_Backward
    };

    //---------------------------------------------------------------------------
    //! @brief  イベントの種類を表す列挙型です。
    //!
    //!         @ref EventCallback の引数で取得することができます。
    //!
    //!         イベントにはトリガイベントとレンジイベントの 2 種類があります。
    //!         トリガイベントは、開始フレームを通過したときにサウンドの再生が始まり、
    //!         終了フレームを通過したときにサウンドの再生が停止します。
    //!         レンジイベントは、
    //!         フレームの現在位置が開始フレームと終了フレームの間にある場合に
    //!         サウンドを再生し続けます。
    //!
    //! @see EventCallback
    //---------------------------------------------------------------------------
    enum EventType
    {
        //! トリガイベントの開始を表します。
        EventType_TriggerStart = detail::AnimSoundImpl::EventType_TriggerStart,
        //! トリガイベントの終了を表します。
        EventType_TriggerStop  = detail::AnimSoundImpl::EventType_TriggerStop,
        //! レンジイベントの開始を表します。
        EventType_RangeStart   = detail::AnimSoundImpl::EventType_RangeStart,
        //! レンジイベントの終了を表します。
        EventType_RangeStop    = detail::AnimSoundImpl::EventType_RangeStop
    };

    //---------------------------------------------------------------------------
    //! @brief  アニメーションサウンドのイベント発生時に呼び出されるコールバック関数の型です。
    //!
    //!         どのようなイベントが発生したかは type で知ることができます。
    //!
    //! @param[in] type     イベントの種類です。
    //! @param[in] frame    イベントの発生したフレーム位置です。
    //! @param[in] soundLabel   イベントで再生されるサウンドのラベル文字列です。
    //! @param[in] userParam    イベントのユーザーパラメータです。
    //! @param[in] arg      @ref SetEventCallback で設定されたユーザー引数です。
    //!
    //! @see SetEventCallback
    //! @see EventType
    //---------------------------------------------------------------------------
    typedef void (*EventCallback)(
        EventType type,
        int32_t frame,
        const char* soundLabel,
        uint32_t userParam,
        void* arg );

    //----------------------------------------
    //! @name コンストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //!
    //!         starter で、アニメーションサウンド内のサウンド再生に使用する
    //!         再生インターフェイスを指定します。
    //!         再生インターフェイスには、@ref SoundArchivePlayer または
    //!         @ref Sound3DActor 等を指定することができます。
    //!
    //! @param[in] starter  サウンド再生インターフェイスです。
    //!
    //! @see SoundStartable クラス
    //! @see SoundArchivePlayer クラス
    //! @see Sound3DActor クラス
    //---------------------------------------------------------------------------
    explicit TAnimSound( SoundStartable& starter ) NN_NOEXCEPT
    : m_Impl( starter, m_EventPlayers, EventPlayerCount )
    {}
    //@}

    //----------------------------------------
    //! @name 初期化処理、終了処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief  アニメーションサウンドを初期化します。
    //!
    //!         アニメーションサウンドで使用するデータ (*.bfasd ファイル) を
    //!         bfasdFile に渡します。
    //!
    //!         本関数で初期化後、@ref ConvertSoundId 関数を呼ばない場合は、
    //!         サウンドを再生するごとにラベル文字列からサウンド ID への変換が行われます。
    //!         @ref ConvertSoundId 関数を読んでおくと、
    //!         当該 bfasd ファイルに含まれるサウンドのラベル文字列を、
    //!         あらかじめまとめてサウンド ID に変換しておくことができます。
    //!
    //! @param[in] bfasdFile    アニメーションサウンドデータです。
    //!
    //! @return 初期化に成功すると true、失敗すると false を返します。
    //!
    //! @see ConvertSoundId
    //! @see Finalize
    //---------------------------------------------------------------------------
    bool Initialize( const void* bfasdFile ) NN_NOEXCEPT { return m_Impl.Initialize( bfasdFile ); }

    //---------------------------------------------------------------------------
    //! @brief  アニメーションサウンドを破棄します。
    //!
    //!         AnimSoundMaker で「アニメ切替時再生継続」にチェックが入っていると、
    //!         本関数が呼び出されても、該当サウンドは停止しません。
    //!         チェックが入っていないサウンドは、本関数が呼び出されると停止します。
    //!
    //! @see Initialize
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT { m_Impl.Finalize(); }

    //---------------------------------------------------------------------------
    //! @brief  アニメーションサウンドデータ内で使用されているサウンド文字列を、
    //!         サウンドID に変換します。
    //!
    //!         @ref Initialize 後に呼び出しておくと、
    //!         アニメーションサウンド再生時に、
    //!         逐次サウンド文字列からサウンドID に変換する処理を省くことができます。
    //!
    //! @param[in] arc  当該アニメーションサウンドで鳴らすサウンドが属している
    //!                 サウンドアーカイブです。
    //!
    //! @return     変換に成功すると true、失敗すると false を返します。
    //!
    //! @see Initialize
    //---------------------------------------------------------------------------
    bool ConvertSoundId( const SoundArchive& arc ) NN_NOEXCEPT { return m_Impl.ConvertSoundId( arc ); }
    //@}

    //----------------------------------------
    //! @name フレーム処理
    //@{
    //---------------------------------------------------------------------------
    //! @brief  アニメーションサウンドのフレームを進めます。
    //!
    //!         アニメーションサウンドの再生を、現在のフレーム位置から frame まで進めます。
    //!         その際、通過したフレーム位置にあるサウンドが再生されます
    //!         (frame のフレーム位置にあるサウンドを含みます)。
    //!         現在のフレーム位置は @ref GetCurrentFrame で取得できます。
    //!
    //!         direction には、フレーム数が増えていく方向の再生の場合
    //!         PlayDirection_Forward を、
    //!         減っていく方向の場合 PlayDirection_Backward を指定します。
    //!         省略時は PlayDirection_Forward です。
    //!
    //!         再生方向が順方向で、frame が現在位置より小さい場合は、
    //!         アニメーションサウンドデータの終端でループして再生されます。
    //!         アニメーションサウンドデータのフレーム数は
    //!         @ref GetFrameLength で取得できます。
    //!
    //! @param[in] frame        再生を進めるフレーム位置です。
    //! @param[in] direction    再生方向です。
    //!
    //! @see PlayDirection
    //! @see GetCurrentFrame
    //! @see GetFrameLength
    //! @see SetBaseStep
    //---------------------------------------------------------------------------
    void UpdateFrame( float frame, PlayDirection direction = PlayDirection_Forward ) NN_NOEXCEPT
    {
        m_Impl.UpdateFrame(
                frame, static_cast<detail::AnimSoundImpl::PlayDirection>( direction ) );
    }
    //---------------------------------------------------------------------------
    //! @brief  アニメーションサウンドの現在のフレーム位置を設定します。
    //!
    //!         この関数による現在位置の更新では、サウンドは再生されません。
    //!         frame のフレーム位置にあるサウンドは、
    //!         次に @ref UpdateFrame が呼び出された際に再生されます。
    //!
    //! @param[in] frame    設定するフレーム位置です。
    //!
    //! @see UpdateFrame
    //---------------------------------------------------------------------------
    void ResetFrame( float frame ) NN_NOEXCEPT { m_Impl.ResetFrame( frame, 0 ); }

    //---------------------------------------------------------------------------
    //! @brief  アニメーションサウンドの通常のフレーム更新間隔を設定します。
    //!
    //!         デフォルト値として、あらかじめ 1.0f が設定されています。
    //!
    //!         アニメーションサウンドの再生速度を決めるための、
    //!         通常のフレーム更新間隔を設定します。
    //!         @ref UpdateFrame 関数で、 1.0f ずつ大きな値を設定する場合は、
    //!         1.0f を設定します。
    //!         アプリケーション実行中に FPS を変更した場合などで、
    //!         2.0f ずつ大きな値を設定する場合は、2.0f を設定します。
    //!
    //!         ここで設定する値は、AnimSound インスタンス内で保持される
    //!         「再生速度」の算出に用いられます。
    //!         たとえば、下記のような場合は、
    //!         再生速度は「(4.0f - 2.0f) ÷ 1.0f = 2.0f」と算出されます。
    //!
    //!         - 【通常のフレーム更新間隔 (本関数で設定)】を 1.0f
    //!         - 【前回の UpdateFrame 関数で設定されたフレーム位置】を 2.0f
    //!         - 【今回の UpdateFrame 関数で設定されたフレーム位置】を 4.0f
    //!
    //!         フレーム位置を 4.0f → 2.0f のように逆方向に設定すると、
    //!         再生速度は負値を取ります。
    //!
    //!         ここで算出された「再生速度」を 100 倍し、
    //!         かつ、-32768～32767 の範囲でクランプした値が、
    //!         AnimSoundMaker の「再生速度反映変数」で設定したシーケンス変数に反映されます。
    //!         反映タイミングは、該当サウンドが再生されるときです。
    //!
    //!         また、「再生速度反映変数」がグローバル変数の場合、
    //!         該当サウンドがシーケンスサウンドでなくても値が反映されます。
    //!         「再生速度反映変数」がローカル変数の場合は、
    //!         該当サウンドがシーケンスサウンドでないと値は反映されません。
    //!
    //! @param[in] baseStep     通常のフレーム更新間隔です。
    //! @see UpdateFrame
    //---------------------------------------------------------------------------
    void SetBaseStep( float baseStep = 1.0f ) NN_NOEXCEPT { m_Impl.SetBaseStep( baseStep ); }
    //@}

    //----------------------------------------
    //! @name 情報取得
    //@{
    //---------------------------------------------------------------------------
    //! @brief  アニメーションサウンドが使用可能な状態かどうかを取得します。
    //!
    //!         アニメーションサウンドは、
    //!         @ref Initialize が true を返してから、@ref Finalize を呼び出すまで、
    //!         使用可能です。
    //!
    //! @return 使用可能な場合 true、不可能な場合 false を返します。
    //!
    //! @see Initialize
    //! @see Finalize
    //---------------------------------------------------------------------------
    bool IsAvailable() const NN_NOEXCEPT { return m_Impl.IsAvailable(); }

    //---------------------------------------------------------------------------
    //! @brief  アニメーションサウンドデータのフレーム数を取得します。
    //!
    //!         アニメーションサウンドが使用可能でない場合は、 0 を返します。
    //!
    //! @return アニメーションサウンドデータのフレーム数を返します。
    //!
    //! @see IsAvailable
    //---------------------------------------------------------------------------
    uint32_t GetFrameLength() const NN_NOEXCEPT { return m_Impl.GetFrameSize(); }

    //---------------------------------------------------------------------------
    //! @brief  現在のフレーム位置を取得します。
    //!
    //!         アニメーションサウンドが使用可能でない場合は、 0.0f を返します。
    //!
    //! @return 現在のフレーム位置を返します。
    //!
    //! @see IsAvailable
    //---------------------------------------------------------------------------
    float GetCurrentFrame() const NN_NOEXCEPT { return m_Impl.GetCurrentFrame(); }
    //@}

    //---------------------------------------------------------------------------
    //! @brief  アニメーションサウンドで再生中のすべてのイベントの発音を停止します。
    //---------------------------------------------------------------------------
    void StopAllSound() NN_NOEXCEPT { m_Impl.StopAllSound(); }

    //---------------------------------------------------------------------------
    //! @brief  アニメーションサウンドにイベントコールバックを設定します。
    //!
    //!         イベントコールバックは、フレームがイベントを通過し、
    //!         イベントの処理が発生したときに呼び出されます。
    //!
    //! @param[in] callback イベントコールバックです。
    //! @param[in] arg      イベントコールバックに渡されるユーザー引数です。
    //!
    //! @see EventCallback
    //---------------------------------------------------------------------------
    void SetEventCallback( EventCallback callback, void* arg ) NN_NOEXCEPT
    {
        m_Impl.SetEventCallback( reinterpret_cast<detail::AnimSoundImpl::EventCallback>( callback ), arg );
    }

    //! @briefprivate
    //! @return :private
    int detail_GetLoopCount() const NN_NOEXCEPT { return m_Impl.GetLoopCount(); }

    //---------------------------------------------------------------------------
    //! @brief  アニメーションサウンドデータ (*.bfasd ファイル) の内容をダンプします。
    //!
    //!         NN_DETAIL_ATK_INFO により下記のような形式でログに出力されます。
    //!
    //! @code
    //! *** Dump begin ***
    //! FrameSize :  30
    //! EventCount:   2
    //! [  0] start(4) end(-1) eventType(TRIG)
    //!       (WSD_FOOTSTEP0) vol(127) pitch(3.000000) userParam(0x00000000)
    //!       notStopSoundWhenAnimFinish?(0) seqVarEnable?(0) seqVar(0)
    //!       playDirection(BOTH)
    //! [  1] start(19) end(-1) eventType(TRIG)
    //!       (WSD_FOOTSTEP1) vol(127) pitch(3.000000) userParam(0x00000000)
    //!       notStopSoundWhenAnimFinish?(0) seqVarEnable?(0) seqVar(0)
    //!       playDirection(BOTH)
    //! *** Dump end *****
    //! @endcode
    //!
    //!         それぞれの意味は下記のとおりです。
    //!
    //!         - FrameSize : アニメーションのフレーム長です。
    //!         - EventCount : アニメーションサウンドデータに含まれるイベントの数です。
    //!         - start/end : イベントのスタートフレーム・エンドフレームです。-1 は
    //!           フレームを指定していないことを表します。
    //!         - eventType : イベントの種類です。
    //!         - (XXXX) : サウンド名です。
    //!         - vol : 音量です。
    //!         - pitch : ピッチです。
    //!         - userParam : ユーザーパラメータです。16 進数で表示します。
    //!         - notStopSoundWhenAnimFinish? : 「アニメ切替時再生継続」するかどうかを表します。
    //!           1 のときに再生継続します。0 のときは再生継続しません。
    //!         - seqVarEnable? : 「再生速度反映変数」が設定されているかどうかを表します。
    //!           1 のときに設定されていることを表します。
    //!         - seqVar : 「再生速度反映変数」で設定しているシーケンス変数を表します。
    //!           0～15 がローカル変数、15～31 がグローバル変数を示します。
    //!
    //! @param[in] bfasdFile    アニメーションサウンドデータです。
    //---------------------------------------------------------------------------
    static void Dump( const void* bfasdFile ) NN_NOEXCEPT
    {
        return detail::AnimSoundImpl::Dump(bfasdFile);
    }

private:
    detail::AnimSoundImpl m_Impl;
    detail::AnimEventPlayer m_EventPlayers[ EventPlayerCount ];
};

//---------------------------------------------------------------------------
//! @brief  アニメーションサウンドを再生させるクラスです。
//!
//!         詳しくは @ref TAnimSound クラスの説明をご参照ください。
//!
//!         メモリが足りないなどの問題がない場合は、@ref TAnimSound クラスではなく、
//!         本クラスをご利用ください。
//!
//! @see TAnimSound クラス
//---------------------------------------------------------------------------
typedef TAnimSound<8> AnimSound;

} // namespace nn::atk
} // namespace nn

