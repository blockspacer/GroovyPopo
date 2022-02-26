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
 * :include nn/atk/atk_Sound3DActor.h
 *
 * @file atk_Sound3DActor.h
 */

#pragma once

#include <nn/atk/atk_SoundActor.h>
#include <nn/atk/atk_BasicSound.h>
#include <nn/util/util_MathTypes.h>

namespace nn {
namespace atk {

class Sound3DManager;
class SoundArchivePlayer;
class SoundHandle;

/* ========================================================================
        class definition
   ======================================================================== */

//---------------------------------------------------------------------------
//! @brief    3D 空間内での音源を表す 3D サウンドアクタークラスです。
//!
//!           1 つのアクターは複数のサウンドを同時に管理することができます。
//!
//!           アクターのインスタンスが存在する間、
//!           アクターに設定された位置情報を元に @ref nn::atk::Sound3DManager
//!           クラスがサウンドのパラメータを計算して設定します。
//!           アクターのインスタンスが無くなっても、
//!           そのアクターのサウンド再生は続きますが、
//!           3D のパラメータの更新は行われなくなります。
//!
//!           この型は @ref nn::util::Vector3fType 型のメンバを持っているため
//!           メモリ上に確保する場合にはアライメントを満たす必要があります。
//!           @ref nn::atk::Sound3DActor::BufferAlignSize バイト境界に配置するようにしてください。
//!
//! @see Sound3DManager クラス
//!
//---------------------------------------------------------------------------
class Sound3DActor
: public SoundActor
, public detail::BasicSound::AmbientArgUpdateCallback
{
public:
    //---------------------------------------------------------------------------
    //! @brief    インスタンス構築時に必要となるアライメントの大きさです。
    //---------------------------------------------------------------------------
    static const size_t BufferAlignSize = 16;

    //! @name コンストラクタ/デストラクタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //!           3D サウンドアクターは、
    //!           引数で渡されたサウンドアーカイブプレイヤーを使用してサウンドを再生します。
    //!           また、3D サウンドのパラメータ計算には、
    //!           引数で渡された 3D サウンドマネージャーを使用します。
    //!
    //!           引数付きのコンストラクタの場合は、
    //!           @ref Initialize を呼ぶ必要はありません
    //!           (呼んでも問題ありません)。
    //!
    //! @param[in] pPlayer     アクターが使用するサウンドアーカイブプレイヤーです。
    //! @param[in] pManager    アクターが使用する 3D サウンドマネージャーです。
    //!
    //! @see SoundArchivePlayer クラス
    //! @see Sound3DManager クラス
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    Sound3DActor( SoundArchivePlayer* pPlayer, Sound3DManager* pManager ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    コンストラクタです。
    //!
    //!           3D サウンドアクターは、
    //!           引数で渡されたサウンドアーカイブプレイヤーを使用してサウンドを再生します。
    //!           また、3D サウンドのパラメータ計算には、
    //!           引数で渡された 3D サウンドマネージャーを使用します。
    //!
    //!           引数付きのコンストラクタの場合は、
    //!           @ref Initialize を呼ぶ必要はありません
    //!           (呼んでも問題ありません)。
    //!
    //! @param[in] pPlayer     アクターが使用するサウンドアーカイブプレイヤーです。
    //! @param[in] pManager    アクターが使用する 3D サウンドマネージャーです。
    //! @param[in] pActorPlayer 再生に使用するアクタープレイヤーです。
    //!                         設定したアクタープレイヤーは 0 番のアクタープレイヤーとして使用されます。
    //!                         NULL を設定した場合はデフォルトのアクタープレイヤーが使用されます。
    //!
    //!                         SoundActor::ActorPlayer を継承し、CanPlaySound で再生制御の処理を定義した
    //!                         ユーザ定義のアクタープレイヤーを設定してください。
    //!
    //! @see SoundArchivePlayer クラス
    //! @see Sound3DManager クラス
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    Sound3DActor( SoundArchivePlayer* pPlayer, Sound3DManager* pManager, ActorPlayer* pActorPlayer ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //!           引数なしのコンストラクタは、@ref Initialize を呼ぶ必要があります。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    Sound3DActor() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    デストラクタです。
    //!
    //---------------------------------------------------------------------------
    virtual ~Sound3DActor() NN_NOEXCEPT NN_OVERRIDE;
    //@}

    //! @name 初期化
    //@{
    //---------------------------------------------------------------------------
    //! @brief    3D サウンドアクターを初期化します。
    //!
    //!           3D サウンドアクターを使用する前に、初期化を行う必要があります。
    //!           ただし、引数付きコンストラクタを呼んでいる場合は、
    //!           呼ぶ必要はありません (呼んでも問題ありません)。
    //!
    //!           @ref SoundActor::Initialize は本関数内で呼ばれます。
    //!
    //!           同じ Sound3DActor インスタンスを再利用するには、
    //!           あらかじめ @ref Finalize 関数を呼んでから、
    //!           再度、本関数を呼び出してください。
    //!
    //!           本関数の引数で渡した Sound3DManager について、
    //!           Sound3DActor で再生したサウンドが鳴り終わる前に破棄を行うと
    //!           SoundArchivePlayer::Update() で例外で停止する可能性があります。
    //!
    //!           これは Sound3DActor で再生したサウンドが
    //!           Sound3DManager をコールバックとして参照しているためです。
    //!           Sound3DManager を破棄する場合は
    //!           必ず参照しているサウンドが全て再生完了してから行ってください。
    //!
    //! @param[in] pPlayer     アクターが使用するサウンドアーカイブプレイヤーです。
    //! @param[in] pManager    アクターが使用する 3D サウンドマネージャーです。
    //!
    //! @see SoundArchivePlayer クラス
    //! @see Sound3DManager クラス
    //! @see Sound3DActor()
    //! @see Finalize
    //---------------------------------------------------------------------------
    void Initialize( SoundArchivePlayer* pPlayer, Sound3DManager* pManager ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    3D サウンドアクターを初期化します。
    //!
    //!           3D サウンドアクターを使用する前に、初期化を行う必要があります。
    //!           ただし、引数付きコンストラクタを呼んでいる場合は、
    //!           呼ぶ必要はありません (呼んでも問題ありません)。
    //!
    //!           @ref SoundActor::Initialize は本関数内で呼ばれます。
    //!
    //!           同じ Sound3DActor インスタンスを再利用するには、
    //!           あらかじめ @ref Finalize 関数を呼んでから、
    //!           再度、本関数を呼び出してください。
    //!
    //!           本関数の引数で渡した Sound3DManager について、
    //!           Sound3DActor で再生したサウンドが鳴り終わる前に破棄を行うと
    //!           SoundArchivePlayer::Update() で例外で停止する可能性があります。
    //!
    //!           これは Sound3DActor で再生したサウンドが
    //!           Sound3DManager をコールバックとして参照しているためです。
    //!           Sound3DManager を破棄する場合は
    //!           必ず参照しているサウンドが全て再生完了してから行ってください。
    //!
    //! @param[in] pPlayer      アクターが使用するサウンドアーカイブプレイヤーです。
    //! @param[in] pManager     アクターが使用する 3D サウンドマネージャーです。
    //! @param[in] pActorPlayer 再生に使用するアクタープレイヤーです。
    //!                         設定したアクタープレイヤーは 0 番のアクタープレイヤーとして使用されます。
    //!                         NULL を設定した場合はデフォルトのアクタープレイヤーが使用されます。
    //!
    //!                         SoundActor::ActorPlayer を継承し、CanPlaySound で再生制御の処理を定義した
    //!                         ユーザ定義のアクタープレイヤーを設定してください。
    //!
    //! @see SoundArchivePlayer クラス
    //! @see Sound3DManager クラス
    //! @see Sound3DActor()
    //! @see Finalize
    //---------------------------------------------------------------------------
    void Initialize( SoundArchivePlayer* pPlayer, Sound3DManager* pManager, ActorPlayer* pActorPlayer ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    3D サウンドアクターを破棄します。
    //!
    //!           デストラクタの中で呼ばれますが、
    //!           明示的に呼んでも問題ありません。
    //!
    //!           同じ Sound3DActor インスタンスを再利用するには、
    //!           本関数を呼んだ後、再度 @ref Initialize 関数を呼び出してください。
    //!
    //! @see ~Sound3DActor
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;
    //@}

    //! @name 位置・速度情報の設定と取得
    //@{
    //---------------------------------------------------------------------------
    //! @brief    アクターの位置情報を設定します。
    //!
    //!           3D サウンドアクターの位置情報を設定します。
    //!           設定された座標は、3D サウンドのパラメータ計算で使用されます。
    //!
    //!           新しく位置情報を設定した際、前回設定された位置情報との差分を計算し、
    //!           3D サウンドアクターの速度が自動的に設定されます。
    //!           アクターの速度は、ドップラー効果による音程変化に反映されます。
    //!           速度は @ref nn::atk::Sound3DActor::SetVelocity
    //!           で明示的に設定することも可能です。
    //!
    //!           座標が別の位置に飛んだ場合、飛んだ距離の差分で速度が計算されてしまうため、
    //!           極めて高速で移動したように処理され、急激な音程変化が発生します。
    //!           このような場合、この関数を呼び出した後で、
    //!           @ref nn::atk::Sound3DActor::SetVelocity を呼び出し、
    //!           速度を明示的に設定する必要があることに注意してください。
    //!
    //! @param[in] position   アクターの位置座標です。
    //!
    //! @see SetVelocity
    //! @see GetPosition
    //!
    //---------------------------------------------------------------------------
    void SetPosition( const nn::util::Vector3fType& position ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    アクターの位置情報を取得します。
    //!
    //! @return   現在設定されているアクターの位置情報を返します。
    //!
    //! @see SetPosition
    //!
    //---------------------------------------------------------------------------
    const nn::util::Vector3fType& GetPosition() const NN_NOEXCEPT { return m_Position; }

    //---------------------------------------------------------------------------
    //! @brief    アクターの位置情報をリセットします。
    //!
    //!           3D サウンドアクターの位置情報と速度をゼロクリアします。
    //!
    //!           この関数でリセットした直後の @ref nn::atk::Sound3DActor::SetPosition
    //!           の呼び出しでは、3D サウンドアクターの速度は設定されません。
    //!
    //! @see SetPosition
    //! @see SetVelocity
    //!
    //---------------------------------------------------------------------------
    void ResetPosition() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    アクターの速度を設定します。
    //!
    //!           設定された速度は、ドップラー効果による音程変化に反映されます。
    //!
    //!           3D サウンドアクターの速度は @ref nn::atk::Sound3DActor::SetPosition
    //!           で座標を設定した際、前回の登録座標との差分から自動的に設定されますが、
    //!           この関数を使用すると 3D サウンドアクターの速度を手動で設定することができます。
    //!
    //! @param[in] velocity   アクターの速度です。
    //!
    //! @see GetVelocity
    //! @see SetPosition
    //!
    //---------------------------------------------------------------------------
    void SetVelocity( const nn::util::Vector3fType& velocity ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    アクターの速度を取得します。
    //!
    //! @return   現在設定されているアクターの速度を返します。
    //!
    //! @see SetVelocity
    //! @see SetPosition
    //!
    //---------------------------------------------------------------------------
    const nn::util::Vector3fType& GetVelocity() const NN_NOEXCEPT { return m_Velocity; }
    //@}

    //! @name パラメータ設定・取得
    //@{
    //---------------------------------------------------------------------------
    //! @brief    アクターにユーザーパラメータを設定します。
    //!
    //!           設定されたユーザーパラメータは、
    //!           @ref nn::atk::Sound3DParam 構造体の actorUserParam に反映されます。
    //!
    //! @param[in] param  ユーザーパラメータです。
    //!
    //! @see GetUserParam
    //! @see Sound3DParam 構造体
    //!
    //---------------------------------------------------------------------------
    void SetUserParam( uint32_t param ) NN_NOEXCEPT { m_UserParam = param; }

    //---------------------------------------------------------------------------
    //! @brief    アクターのユーザーパラメータを取得します。
    //!
    //! @return   現在設定されているアクターのユーザーパラメータを返します。
    //!
    //! @see SetUserParam
    //!
    //---------------------------------------------------------------------------
    uint32_t GetUserParam() const NN_NOEXCEPT { return m_UserParam; }
    //@}

protected:
    // リファレンスは SoundActor::Setup に書かれている
    virtual SoundStartable::StartResult SetupSound(
        SoundHandle* handle,
        uint32_t soundId,
        const SoundStartable::StartInfo* startInfo,
        void* setupArg
    ) NN_NOEXCEPT NN_OVERRIDE;

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
        const StartInfo* startInfo,
        void* setupArg
    ) NN_NOEXCEPT NN_OVERRIDE;

    //! @internal
    //! @param arg :private
    //! @param sound :private
    virtual void detail_UpdateAmbientArg( void* arg, const detail::BasicSound* sound ) NN_NOEXCEPT NN_OVERRIDE;

private:
    class ClearUpdateCallback
    {
    public:
        explicit ClearUpdateCallback(const SoundActor* pSoundActor) NN_NOEXCEPT;
        void operator() ( SoundHandle& handle ) NN_NOEXCEPT;
    private:
        const SoundActor* m_pSoundActor;
    };

private:
    Sound3DManager* m_p3dManager;
    SoundArchivePlayer* m_pArchivePlayer;
    uint32_t m_UserParam;
    nn::util::Vector3fType m_Position;
    nn::util::Vector3fType m_Velocity;
    bool m_ResetPositionFlag;
    bool m_IsInitialized;
    bool m_IsFinalized;
};

} // namespace nn::atk
} // namespace nn

