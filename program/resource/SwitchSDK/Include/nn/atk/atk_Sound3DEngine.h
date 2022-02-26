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
 * :include nn/atk/atk_Sound3DEngine.h
 *
 * @file atk_Sound3DEngine.h
 */

#pragma once

#include <nn/atk/atk_Sound3DManager.h>
#include <nn/atk/atk_Sound3DCalculator.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @brief  3D サウンドのパラメータ演算エンジンクラスです。
//!
//!         3D サウンドアクターや 3D サウンドリスナーの位置情報などから、
//!         サウンドに設定する音量やパンなどの各種パラメータを計算します。
//!         このクラスでは、3D サウンドの標準的なパラメータ計算が定義されています。
//!
//!         3D サウンドマネージャーには、初期状態でこのクラスのインスタンスが登録されています。
//!
//!         3D サウンドエンジンをカスタマイズしたい場合、
//!         このクラスを継承して独自のエンジンクラスを作成することができます。
//!         カスタマイズしたエンジンクラスは、
//!         @ref Sound3DManager::SetEngine で 3D サウンドマネージャーに登録し、
//!         デフォルトのエンジンクラスの代わりに使用することができます。
//!
//!         @n
//!
//!         ■デフォルト実装の挙動
//!
//!         本クラスの実装では、下記の 3D サウンドパラメータ
//!         (@ref UpdateAmbientParam で渡される @ref SoundAmbientParam) が計算されます。
//!         3D サウンドマネージャーにいくつ 3D サウンドリスナーを登録したかで、
//!         パラメータの計算方法が変わることに注意してください。
//!
//!         これ以外の計算をさせたい場合は、本クラスを継承したユーザークラス内で、
//!         @ref UpdateAmbientParam をオーバーライドしてください。
//!
//!         @n
//!
//!         ▼ 共通の挙動
//!
//!         @ref SoundAmbientParam のうち、下記のパラメータは【計算されません】。
//!         - volume
//!         - lowPassFilter
//!         - userData
//!         - outputLineFlag
//!         - tvParam の effectSend
//!
//!         @ref SoundAmbientParam のうち、下記のパラメータが計算されます。
//!         - biquadFilterType
//!
//!         @n
//!
//!         ▼ 3D サウンドマネージャーに、リスナーを 1 つだけ @ref Sound3DManager::AddListener
//!         しているとき
//!
//!         @ref SoundAmbientParam のうち、下記のパラメータが計算されます。
//!         - pitch
//!         - biquadFilterValue
//!         - priority
//!
//!         次のパラメータは、@ref Sound3DListener::GetOutputTypeFlag で示された
//!         出力先の値のみ計算されます。
//!         - [tvParam の volume] のうち該当する出力先のもの
//!         - [tvParam の pan] のうち該当する出力先のもの
//!         - [tvParam の surroundPan] のうち該当する出力先のもの
//!
//!         @n
//!
//!         ▼ 3D サウンドマネージャーに、リスナーを複数 @ref Sound3DManager::AddListener して
//!         いるときの共通の挙動
//!
//!         @ref SoundAmbientParam のうち、下記のパラメータが計算されますが、
//!         複数登録されているリスナーで計算された結果、最大の値が採用されます。
//!         - biquadFilterValue
//!         - priority
//!
//!         @n
//!
//!         ▼ 3D サウンドマネージャーに、リスナーを複数 @ref Sound3DManager::AddListener して
//!         いるが、そのうち 1 つしかリスナーが登録されていない出力先の挙動
//!         (たとえば、OutputType_Tv を指定したリスナーがひとつしか無い場合の、
//!         TV 出力向けの挙動を指します)
//!
//!         @ref SoundAmbientParam のうち、下記のパラメータが計算されます。
//!         - [tvParam の volume] のうち該当する出力先のもの
//!         - [tvParam の pan] のうち該当する出力先のもの
//!         - [tvParam の surroundPan] のうち該当する出力先のもの
//!
//!         @n
//!
//!         ▼ 3D サウンドマネージャーに、リスナーを複数 @ref Sound3DManager::AddListener して
//!         いるが、複数のリスナーが登録されている出力先の挙動
//!         (たとえば、OutputType_Tv を指定したリスナーが複数ある場合の、
//!         TV 出力向けの挙動を指します)
//!
//!         @ref  SoundAmbientParam のうち、下記のパラメータが計算されます。
//!         - [tvParam の volume] のうち該当する出力先のもので、
//!         最大の値。
//!         @ref SoundAmbientParam のうち、下記のパラメータが計算されません。
//!         - [tvParam の pan]
//!         - [tvParam の surroundPan]
//!
//! @see Sound3DManager クラス
//! @see Sound3DManager::SetEngine
//---------------------------------------------------------------------------
class Sound3DEngine : public detail::ISound3DEngine
{
public:

    //! @name コンストラクタ/デストラクタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //---------------------------------------------------------------------------
    Sound3DEngine() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    デストラクタです。
    //!
    //---------------------------------------------------------------------------
    virtual ~Sound3DEngine() NN_NOEXCEPT NN_OVERRIDE {}
    //@}


    //! @name 設定
    //@{
    //---------------------------------------------------------------------------
    //! @brief    パン計算用パラメータ構造体を設定します。
    //!
    //!           設定したパラメータは、3D サウンドエンジン内のパン計算の際に使用されます。
    //!
    //! @param[in] calculatePanParam   パン計算用パラメータ構造体です。
    //!
    //! @see Sound3DCalculator::CalculatePanParam 構造体
    //!
    //---------------------------------------------------------------------------
    void SetCalculatePanParam( const Sound3DCalculator::CalculatePanParam& calculatePanParam ) NN_NOEXCEPT
    {
        m_CalcPanParam = calculatePanParam;
    }

    //---------------------------------------------------------------------------
    //! @brief    現在設定されているパン計算用パラメータ構造体を取得します。
    //!
    //! @return   パン計算用パラメータ構造体を返します。
    //!
    //! @see Sound3DCalculator::CalculatePanParam 構造体
    //!
    //---------------------------------------------------------------------------
    const Sound3DCalculator::CalculatePanParam& GetCalculatePanParam() const NN_NOEXCEPT
    {
        return m_CalcPanParam;
    }
    //@}

protected:
    //---------------------------------------------------------------------------
    //! @brief    UpdateAmbientParam で音量計算をする必要があることを示すフラグです。
    //!
    //! @see UpdateAmbientParam
    //!
    //---------------------------------------------------------------------------
    static const uint32_t UpdateVolume      = ( 1 << 0 );

    //---------------------------------------------------------------------------
    //! @brief    UpdateAmbientParam でプレイヤープライオリティ計算をする必要があることを示すフラグです。
    //!
    //! @see UpdateAmbientParam
    //!
    //---------------------------------------------------------------------------
    static const uint32_t UpdatePriority    = ( 1 << 1 );

    //---------------------------------------------------------------------------
    //! @brief    UpdateAmbientParam でパン計算をする必要があることを示すフラグです。
    //!
    //! @see UpdateAmbientParam
    //!
    //---------------------------------------------------------------------------
    static const uint32_t UpdatePan         = ( 1 << 2 );

    //---------------------------------------------------------------------------
    //! @brief    UpdateAmbientParam でサラウンドパン計算をする必要があることを示すフラグです。
    //!
    //! @see UpdateAmbientParam
    //!
    //---------------------------------------------------------------------------
    static const uint32_t UpdateSurroundPan        = ( 1 << 3 );

    //---------------------------------------------------------------------------
    //! @brief    UpdateAmbientParam で
    //!           Biquad フィルタのかかり具合を計算する必要があることを示すフラグです。
    //!
    //! @see UpdateAmbientParam
    //!
    //---------------------------------------------------------------------------
    static const uint32_t UpdateFilter      = ( 1 << 4 );

    //---------------------------------------------------------------------------
    //! @brief    UpdateAmbientParam でピッチ計算をする必要があることを示すフラグです。
    //!
    //! @see UpdateAmbientParam
    //!
    //---------------------------------------------------------------------------
    static const uint32_t UpdatePitch       = ( 1 << 5 );

    //---------------------------------------------------------------------------
    //! @brief    サウンド開始前のプレイヤープライオリティ計算のために
    //!           UpdateAmbientParam が呼ばれた事を示すフラグです。
    //!
    //! @see UpdateAmbientParam
    //!
    //---------------------------------------------------------------------------
    static const uint32_t UpdateStartPriority = ( 1 << 24 );

    //! @name パラメータ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    3D サウンドのパラメータ演算を行う関数です。
    //!
    //!           この関数はサウンドの開始時とサウンドの毎フレームの更新で呼び出されます。
    //!
    //!           カスタマイズした 3D サウンドエンジンクラスを作成する場合、
    //!           この関数をオーバーライドして、 ambientParam にパラメータ演算の結果を格納します。
    //!           格納された値は、毎フレームのサウンド処理に反映されます。
    //!
    //!           パラメータ計算には、3D サウンドリスナーや 3D サウンドアクターの情報を使用します。
    //!           3D サウンドリスナーは、sound3DManager から取得できます。
    //!           また、sound3DParam は計算に使用するパラメータの構造体で、
    //!           アクターの座標などや、サウンドごとの 3D サウンド設定を含みます。
    //!
    //!           updateFlag は、どのパラメータを計算する必要があるかを示したビットフラグです。
    //!           ambientParam 構造体内の、
    //!           各フラグに対応したパラメータがサウンドに対して反映されます。
    //!           フラグがたっていないパラメータを更新した場合の動作は不定です。
    //!
    //!             - UpdateVolume ... ambientParam.volume
    //!             - UpdatePan ... ambientParam.pan
    //!             - UpdatePriority ... ambientParam.priority
    //!             - UpdatePitch ... ambientParam.pitch
    //!
    //!           サウンド再生開始前に行われるプレイヤープライオリティ計算のためにこの関数が呼ばれた場合には、
    //!           updateFlag  に UpdateStartPriority が設定されます。
    //!           (このとき UpdatePriority  も同時に設定されています。)
    //!
    //!           引数として渡される ambientParam 構造体には、
    //!           前回の 3D サウンド計算結果が格納されています。
    //!           UpdateAmbientParam 関数をオーバーライドする際、
    //!           ここで渡される値と今回計算される値とを比較することで急激なパラメータ変化を避け、
    //!           プチノイズを抑制することが可能になります。
    //!
    //! @param[out] pOutValue     計算結果を格納するパラメータ構造体です。
    //! @param[in] sound3DManager 3D サウンドマネージャです。
    //! @param[in] sound3DParam   3D サウンドパラメータです。
    //! @param[in] soundId        サウンドの ID です。
    //! @param[in] updateFlag     どのパラメータを更新するかを示したフラグです。
    //!
    //! @see Sound3DManager クラス
    //! @see Sound3DParam 構造体
    //! @see SoundAmbientParam 構造体
    //!
    //---------------------------------------------------------------------------
    virtual void UpdateAmbientParam(
        SoundAmbientParam* pOutValue,
        const Sound3DManager* sound3DManager,
        const Sound3DParam* sound3DParam,
        uint32_t soundId,
        uint32_t updateFlag
    ) NN_NOEXCEPT;
    //@}

private:
    virtual int GetAmbientPriority(
        const Sound3DManager* sound3DManager,
        const Sound3DParam* sound3DParam,
        uint32_t soundId
    ) NN_NOEXCEPT NN_OVERRIDE;
    virtual void detail_UpdateAmbientParam(
        const Sound3DManager* sound3DManager,
        const Sound3DParam* sound3DParam,
        uint32_t soundId,
        SoundAmbientParam* ambientParam
    ) NN_NOEXCEPT NN_OVERRIDE;

    Sound3DCalculator::CalculatePanParam m_CalcPanParam;
};

} // namespace nn::atk
} // namespace nn

