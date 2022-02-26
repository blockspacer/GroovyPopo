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
 * :include nn/atk/atk_Sound3DManager.h
 *
 * @file atk_Sound3DManager.h
 */

#pragma once

#include <nn/util/util_IntrusiveList.h>
#include <nn/util/util_MathTypes.h>
#include <nn/atk/atk_BasicSound.h>
#include <nn/atk/atk_InstancePool.h>
#include <nn/atk/atk_Sound3DListener.h>

namespace nn {
namespace atk {

/* ========================================================================
        typename declaration
   ======================================================================== */

class SoundArchive;
class Sound3DManager;
class Sound3DEngine;

/* ========================================================================
        struct definition
   ======================================================================== */

//---------------------------------------------------------------------------
//! @brief    3Dサウンドのパラメータです。
//!
//!           このパラメータを用いて 3D サウンドの計算を行います。
//!
//!           controlFlag, decayCurve, decayRatio, dopplerFactor, soundUserParam
//!           は SoundMaker で各サウンドに設定した値が入ります。
//!
//!           actorUserParam は、@ref Sound3DActor::SetUserParam で設定された値です。
//!
//!           dopplerFactor は、ドップラー効果のかかりり具合を表します。
//!           0 のときは音が変化せず、値が大きくなるほど変化が大きくなります。
//!
//! @see Sound3DEngine::UpdateAmbientParam
//! @see Sound3DActor::SetUserParam
//! @see DecayCurve
//!
//---------------------------------------------------------------------------
struct Sound3DParam
{
    //! 3D サウンドアクターの現在位置です。
    nn::util::Vector3fType position;

    //! 3D サウンドアクターの速度です。
    nn::util::Vector3fType velocity;

    //! パラメータのコントロールフラグです。サウンドライブラリ内部で使用されます。
    uint32_t controlFlag;

    //! アクターに設定されたユーザーパラメータです。
    uint32_t actorUserParam;

    //! サウンドに設定されたユーザーパラメータです。
    uint32_t soundUserParam;

    //! アクターとリスナーが音量減衰の単位距離分だけ離れているときの音量の減衰率です。
    float decayRatio;

    //! 音量減衰カーブの種類です。@ref DecayCurve の値を設定します。
    uint8_t  decayCurve;

    //! ドップラーファクターです。
    uint8_t  dopplerFactor;

    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //---------------------------------------------------------------------------
    Sound3DParam() NN_NOEXCEPT;
};

/* ========================================================================
        class definition
   ======================================================================== */

namespace detail {

class ISound3DEngine
{
public:
    virtual ~ISound3DEngine() NN_NOEXCEPT {}

    virtual void detail_UpdateAmbientParam(
        const Sound3DManager* manager,
        const Sound3DParam* actorParam,
        uint32_t soundId,
        SoundAmbientParam* param
    ) NN_NOEXCEPT = 0;

    virtual int GetAmbientPriority(
        const Sound3DManager* manager,
        const Sound3DParam* actorParam,
        uint32_t soundId
    ) NN_NOEXCEPT = 0;
};

}

//---------------------------------------------------------------------------
//! @brief    3D サウンドのパラメータ演算と管理を行うクラスです。
//!
//!           @ref nn::atk::Sound3DListener クラスと
//!           @ref nn::atk::Sound3DActor クラスの情報を用いて、
//!           サウンドのパラメータ演算を行います。
//!
//!           Sound3DActor::Initialize() の引数で渡した Sound3DManager について、
//!           Sound3DActor で再生したサウンドが鳴り終わる前に破棄を行うと
//!           SoundArchivePlayer::Update() で例外で停止する可能性があります。
//!
//!           これは Sound3DActor で再生したサウンドが
//!           Sound3DManager をコールバックとして参照しているためです。
//!           Sound3DManager を破棄する場合は
//!           必ず参照しているサウンドが全て再生完了してから行ってください。
//!
//! @see Sound3DListener クラス
//! @see Sound3DActor クラス
//---------------------------------------------------------------------------
class Sound3DManager
: public detail::BasicSound::AmbientParamUpdateCallback
, public detail::BasicSound::AmbientArgAllocatorCallback
{
public:
    //! @internal
    typedef detail::InstancePool<Sound3DParam> Sound3DParamPool;

    //---------------------------------------------------------------------------
    //! @brief    3D サウンドリスナーのリストを表す型です。
    //!
    //! @see Sound3DListener クラス
    //! @see nn::util::IntrusiveList クラス
    //---------------------------------------------------------------------------
    typedef util::IntrusiveList<Sound3DListener, util::IntrusiveListMemberNodeTraits<Sound3DListener,&Sound3DListener::m_LinkNode>> ListenerList;

    //! @internal nn::util の MathTypes のアラインサイズです。
    static const size_t NnUtilMathTypeAlignSize = 16;

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    //---------------------------------------------------------------------------
    //! @brief      バッファアラインサイズです。
    //---------------------------------------------------------------------------
    static const size_t BufferAlignSize = NnUtilMathTypeAlignSize;

    //! @name コンストラクタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //!           初期化時はプレイヤープライオリティ最大減少量を 32 に設定します。
    //---------------------------------------------------------------------------
    Sound3DManager() NN_NOEXCEPT;
    //@}

    //! @name 初期化
    //@{
    //---------------------------------------------------------------------------
    //! @brief    初期化に必要なメモリのサイズを取得します。
    //!
    //! @param[in] archive    初期化のための情報取得に使用するサウンドアーカイブです。
    //!
    //! @return   初期化に必要なメモリのサイズを返します。
    //!
    //! @see Initialize
    //---------------------------------------------------------------------------
    size_t GetRequiredMemSize( const SoundArchive* archive ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    3D サウンドマネージャーの初期化を行います。
    //!
    //!           3D サウンドマネージャーを使用する前に初期化を行う必要があります。
    //!           3D サウンドを再生する際に、
    //!           3D マネージャでセットアップされたメモリ領域を利用します。
    //!
    //!           3D サウンドマネージャーが必要とするメモリのサイズは
    //!           @ref nn::atk::Sound3DManager::GetRequiredMemSize
    //!           で取得することができます。
    //!
    //! @param[in] pArchive 3D サウンドマネージャーで使用するサウンドアーカイブです。
    //! @param[in] buffer   バッファへのポインタです。
    //! @param[in] size     バッファサイズです。
    //!
    //! @pre
    //! - pArchive != nullptr
    //! - buffer != nullptr
    //! - buffer は BufferAlignSize にアラインしている
    //! - size >= GetRequiredMemSize
    //!
    //! @return   初期化に成功したら true を、失敗したら false を返します。
    //!
    //! @see GetRequiredMemSize
    //---------------------------------------------------------------------------
    bool Initialize(
        const SoundArchive* pArchive,
        void* buffer,
        size_t size
    ) NN_NOEXCEPT;

    //! @briefprivate
    //! @param archive :private
    //! @return :private
    bool InitializeWithMoreSoundArchive( const SoundArchive* archive ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    3D サウンドデータマネージャを破棄します。
    //!
    //! @return   成功なら true を、失敗なら false を返します。
    //!           Sound3DManager が未初期化のときのみ失敗します。
    //!
    //! @see Initialize
    //---------------------------------------------------------------------------
    bool Finalize() NN_NOEXCEPT;
    //@}

    //! @name 3D サウンドリスナー
    //@{
    //---------------------------------------------------------------------------
    //! @brief    3D サウンドリスナーを登録します。
    //!
    //!           3D サウンドを鳴らすためには 3D サウンドマネージャーに
    //!           3D サウンドリスナーを登録する必要があります。
    //!
    //!           3D サウンドリスナーは複数登録することができます。
    //!           複数のリスナーを登録した場合の詳細は、プログラミングガイドの
    //!           @confluencelink{83956042, 3Dサウンド - マルチリスナー} を参照してください。
    //!
    //!           ただし、ひとつの 3D サウンドリスナーを複数の
    //!           3D サウンドマネージャーに登録することはできません。
    //!           また、ひとつの 3D サウンドリスナーをひとつの
    //!           3D サウンドマネージャーに複数回登録することもできません。
    //!
    //! @param[in] listener   登録する 3D サウンドリスナーへのポインタです。
    //!
    //! @see Sound3DListener クラス
    //! @see RemoveListener
    //---------------------------------------------------------------------------
    void AddListener( Sound3DListener* listener ) NN_NOEXCEPT { m_ListenerList.push_back( *listener ); }

    //---------------------------------------------------------------------------
    //! @brief    指定した登録済みの 3D サウンドリスナーを登録解除します。
    //!
    //! @param[in] listener   登録を削除する 3D サウンドリスナーへのポインタです。
    //!
    //! @see Sound3DListener クラス
    //! @see AddListener
    //---------------------------------------------------------------------------
    void RemoveListener( Sound3DListener* listener ) NN_NOEXCEPT { m_ListenerList.erase( m_ListenerList.iterator_to( *listener ) ); }

    //---------------------------------------------------------------------------
    //! @brief    登録されている 3D サウンドリスナーのリストを取得します。
    //!
    //!           取得したリスト内の 3D サウンドリスナーの並び順は、
    //!           @ref AddListener  で 3D サウンドマネージャーに登録された順番です。
    //!
    //! @return   3D サウンドリスナーのリストを返します。
    //!
    //! @see Sound3DListener クラス
    //! @see AddListener
    //! @see RemoveListener
    //---------------------------------------------------------------------------
    const ListenerList& GetListenerList() const NN_NOEXCEPT { return m_ListenerList; }
    //@}

    //! @name 3D サウンドエンジン
    //@{
    //---------------------------------------------------------------------------
    //! @brief    3D サウンドエンジンを登録します。
    //!
    //!           3D サウンドエンジンは、3D サウンドのパラメータの計算処理が定義されたクラスです。
    //!           初期状態では、サウンドライブラリで用意されているデフォルトのエンジンクラスが
    //!           登録されていますので、この関数を呼び出さなくても
    //!           3D サウンドを使用することができます。
    //!
    //!           カスタマイズした 3D サウンドエンジンクラスを使用したい場合には、
    //!           この関数を呼び出して 3D サウンドエンジンを登録してください。
    //!
    //! @param[in] engine 登録する 3D サウンドエンジンへのポインタです。
    //!
    //! @see Sound3DEngine クラス
    //---------------------------------------------------------------------------
    void SetEngine( Sound3DEngine* engine ) NN_NOEXCEPT;
    //@}

    //! @name パラメータ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    最大プレイヤープライオリティ減少量を設定します。
    //!
    //!           3D サウンドのプレイヤープライオリティは、音量の減衰に比例して減少します。
    //!           この関数は音量が 0 になったときのプレイヤープライオリティの減少値を設定します。
    //!
    //! @param[in] maxPriorityReduction   最大プレイヤープライオリティ減少量です。
    //!
    //! @see GetMaxPriorityReduction
    //---------------------------------------------------------------------------
    void SetMaxPriorityReduction( int maxPriorityReduction ) NN_NOEXCEPT
    {
        m_MaxPriorityReduction = maxPriorityReduction;
    }

    //---------------------------------------------------------------------------
    //! @brief    現在設定されている最大プレイヤープライオリティ減少量を取得します。
    //!
    //! @return   現在設定されている最大プレイヤープライオリティ減少量を返します。
    //!
    //! @see SetMaxPriorityReduction
    //---------------------------------------------------------------------------
    int GetMaxPriorityReduction() const NN_NOEXCEPT
    {
        return m_MaxPriorityReduction;
    }

    //---------------------------------------------------------------------------
    //! @brief    3D サウンドで設定されるパンの変化幅を設定します。
    //!
    //!           panRange に 1.0 を指定すると、定位の変化が最大になります。
    //!           1.0 より小さくすると、定位の変化幅を抑えることが出来ます。
    //!
    //!           panRange の初期値は 0.9 です。
    //!
    //! @param[in] panRange   パンの変化幅 ( 0.0 ～ 1.0 ) です。
    //!
    //! @see GetPanRange
    //---------------------------------------------------------------------------
    void SetPanRange( float panRange ) NN_NOEXCEPT { m_PanRange = panRange; }

    //---------------------------------------------------------------------------
    //! @brief    現在設定されているパンの変化幅を取得します。
    //!
    //! @return   現在設定されているパンの変化幅を返します。
    //!
    //! @see SetPanRange
    //---------------------------------------------------------------------------
    float GetPanRange() const NN_NOEXCEPT { return m_PanRange; }

    //---------------------------------------------------------------------------
    //! @brief    3D サウンドで設定される音速を設定します。
    //!
    //!           設定した音速は、ドップラー効果による音程変化の計算に使用されます。
    //!
    //!           設定する値の単位は 1 フレーム当たりの音の速さです。
    //!           音速は約 340m / 秒ですので、3D 空間の座標の単位系が 1.0f で 1m である場合、
    //!           60 フレームで動作しているとすると、340.0f / 60 が設定すべき値になります。
    //!
    //!           音速に 0.0f を設定すると、ドップラー効果が発生しなくなります。
    //!           デフォルト値は 0.0f です。
    //!
    //! @param[in] sonicVelocity  音速です。
    //!
    //! @see GetSonicVelocity
    //---------------------------------------------------------------------------
    void SetSonicVelocity( float sonicVelocity ) NN_NOEXCEPT { m_SonicVelocity = sonicVelocity; }

    //---------------------------------------------------------------------------
    //! @brief    現在設定されている音速を取得します。
    //!
    //! @return   現在設定されている音速を返します。
    //!
    //! @see SetSonicVelocity
    //---------------------------------------------------------------------------
    float GetSonicVelocity() const NN_NOEXCEPT { return m_SonicVelocity; }

    //---------------------------------------------------------------------------
    //! @brief  3D サウンドで設定される Biquad フィルタの種類を設定します。
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
    //!         type には BiquadFilterType_Min ～ BiquadFilterType_Max の値を設定します。
    //!         上記の範囲外の値を入れると、アサートで停止します
    //!         (Debug/Development 版のみ。Release 版は無視され値が設定されますが、
    //!         正常な動作は保証されません)。
    //!
    //! @param[in] type   Biquad フィルタの種類。
    //!
    //! @see BiquadFilterType
    //! @see GetBiquadFilterType
    //---------------------------------------------------------------------------
    void SetBiquadFilterType( int type ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    現在設定されている Biquad フィルタの種類を取得します。
    //!
    //! @return   現在設定されている Biquad フィルタの種類を返します。
    //!
    //! @see BiquadFilterType
    //! @see SetBiquadFilterType
    //---------------------------------------------------------------------------
    int GetBiquadFilterType() const NN_NOEXCEPT { return m_BiquadFilterType; }
    //@}

private:
    virtual void detail_UpdateAmbientParam(
        const void* arg,
        uint32_t soundId,
        SoundAmbientParam* param
    ) NN_NOEXCEPT NN_OVERRIDE;
    virtual int detail_GetAmbientPriority(
        const void* arg,
        uint32_t soundId
    ) NN_NOEXCEPT NN_OVERRIDE;
    virtual void* detail_AllocAmbientArg( size_t argSize ) NN_NOEXCEPT NN_OVERRIDE;
    virtual void detail_FreeAmbientArg(
        void* arg,
        const detail::BasicSound* sound
    ) NN_NOEXCEPT NN_OVERRIDE;

    Sound3DParamPool m_ParamPool;
    ListenerList m_ListenerList;
    detail::ISound3DEngine* m_pSound3DEngine;

    int32_t m_MaxPriorityReduction;
    float m_PanRange;
    float m_SonicVelocity;
    int32_t m_BiquadFilterType;

    size_t m_FreeMemSizeAfterChecking;
    bool m_IsInitialized;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t m_Padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
};

} // namespace nn::atk
} // namespace nn

