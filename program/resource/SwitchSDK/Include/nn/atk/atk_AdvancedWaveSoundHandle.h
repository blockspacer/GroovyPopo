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

#include <nn/atk/atk_BasicSound.h>
#include <nn/atk/atk_WaveSound.h>
#include <nn/atk/detail/atk_AdvancedWaveSound.h>

namespace nn {
namespace atk {

class SoundHandle;

//---------------------------------------------------------------------------
//! @brief    再生したウェーブサウンドの操作を行うためのハンドルクラスです。
//!
//!           サウンドは再生時に汎用サウンドハンドル
//!           @ref SoundHandle と関連付けられますが、
//!           ウェーブサウンドに固有の処理は汎用サウンドハンドルには実装されていません。
//!
//!           ウェーブサウンドに固有の処理を行いたい場合は、
//!           汎用サウンドハンドルを引数にとるコンストラクタを用いて、
//!           ウェーブサウンドハンドルを生成し、使用してください。
//!
//!           ハンドルの使い方は、汎用サウンドハンドル @ref SoundHandle と同じです。
//!
//! @see      SoundHandle クラス
//---------------------------------------------------------------------------
class AdvancedWaveSoundHandle
{
public:
    //! @name コンストラクタ/デストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //!           引数の無いコンストラクタは、
    //!           どのサウンドとも関連付けられていないハンドルを構築します。
    //---------------------------------------------------------------------------
    AdvancedWaveSoundHandle() NN_NOEXCEPT
    : m_pSound( NULL )
    {
    }

    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //!           引数付きコンストラクタは、
    //!           引数 handle で指定したサウンドハンドルが参照しているサウンドを用いて、
    //!           ウェーブサウンドハンドルを構築します。
    //!           ウェーブサウンド以外のサウンドがサウンドハンドルに関連付けられているときは、
    //!           引数の無いコンストラクタと同様に構築します。
    //!
    //!           引数付きコンストラクタで構築した時、
    //!           サウンドが既に別のウェーブサウンドハンドルと関連付けられている場合は、
    //!           その関連付けが切られる点に注意してください。
    //!           ただし、引数に渡したサウンドハンドルとの関連付けは切れません。
    //!
    //! @param[in] handle ウェーブサウンドと関連付けられたサウンドハンドル。
    //!
    //! @see      SoundHandle クラス
    //---------------------------------------------------------------------------
    explicit AdvancedWaveSoundHandle( SoundHandle* handle ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    デストラクタです。
    //---------------------------------------------------------------------------
    ~AdvancedWaveSoundHandle() NN_NOEXCEPT
    {
        DetachSound();
    }

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
    //!           エンベロープのリリースを発音し全ての減衰が完了した後にサウンドが停止します。
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
    //!           フェードにかかるフレーム数は、最大音量から音が消えるまで、
    //!           あるいは、
    //!           発音していない状態から最大音量に達するまでにかかる変化速度を表しますので、
    //!           フェード中にさらにフェードを指定した時などは、
    //!           指定したフレーム数よりも短い時間でフェードが完了する可能性があります。
    //!
    //! @param[in]    flag        true なら一時停止、false なら再開します。
    //! @param[in]    fadeFrames  フェードイン・フェードアウトにかけるフレーム数です。
    //!                           フレーム数は @ref SoundArchivePlayer::Update
    //!                           の呼び出し回数で換算されます。
    //!                           fadeFrames には 0 以上の値を指定してください。
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
    //---------------------------------------------------------------------------
    bool IsPrepared() const NN_NOEXCEPT
    {
        return IsAttachedSound() && m_pSound->IsPrepared();
    }


    //! @name ハンドル操作情報設定 / 取得
    //@{

    //---------------------------------------------------------------------------
    //! @brief    ハンドルにサウンドが関連付けられているかどうかを調べます。
    //!
    //! @return   ハンドルに関連付けられているサウンドがあれば true を返します。
    //---------------------------------------------------------------------------
    bool IsAttachedSound() const NN_NOEXCEPT
    {
        return m_pSound != NULL;
    }

    //---------------------------------------------------------------------------
    //! @brief    ハンドルからサウンドを解放します。
    //!
    //!           ハンドルから開放されたサウンドは、
    //!           その後ハンドルを通して操作できないようになります。
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
    //!           ハンドルが無効の場合は 0xffffffff を返します。
    //!
    //!           この関数で取得できる ID は @ref SetId で設定された ID です。
    //!
    //! @return   サウンドに設定されている ID を返します。
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
    //! @brief    現在再生中のウェーブサウンドデータの再生位置を取得します。
    //!
    //!           ハンドルに関連付けられたサウンドで再生している、
    //!           ウェーブサウンドデータの再生位置をサンプル数で取得します。
    //!           ハンドルが無効の場合は、負の値を返します。
    //!
    //! @return   ウェーブサウンドデータの再生位置をサンプル数で返します。
    //---------------------------------------------------------------------------
    int GetPlaySamplePosition() const NN_NOEXCEPT
    {
        if ( ! IsAttachedSound() )
        {
            return -1;
        }

        return -1; // 未実装
        //return m_pSound->GetPlaySamplePosition(true);
    }

    //@}

    // ハンドルをサウンドに関連付ける
    //! @internal
    //! @param sound :private
    void detail_AttachSoundAsTempHandle( detail::AdvancedWaveSound* sound ) NN_NOEXCEPT;

    //! @internal
    //! @return :private
    detail::AdvancedWaveSound* detail_GetAttachedSound() NN_NOEXCEPT
    {
        return m_pSound;
    }

    //! @internal
    //! @return :private
    const detail::AdvancedWaveSound* detail_GetAttachedSound() const NN_NOEXCEPT
    {
        return m_pSound;
    }

private:
    NN_DISALLOW_COPY( AdvancedWaveSoundHandle );

    detail::AdvancedWaveSound* m_pSound;
};

} // namespace nn::atk
} // namespace nn

