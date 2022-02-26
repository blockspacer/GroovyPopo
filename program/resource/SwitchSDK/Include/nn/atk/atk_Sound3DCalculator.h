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
 * :include nn/atk/atk_Sound3DCalculator.h
 *
 * @file atk_Sound3DCalculator.h
 */


#pragma once

#include <nn/util/util_Constant.h>      // nn::util::FloatPi
#include <nn/util/util_MathTypes.h>
#include <nn/atk/atk_SoundArchive.h>    // Sound3DInfo::DecayCurve

namespace nn {
namespace atk {

class Sound3DManager;
class Sound3DListener;
struct Sound3DParam;

//---------------------------------------------------------------------------
//! @brief    3D サウンドの標準的なパラメータ計算処理をまとめたクラスです。
//!
//!           @ref Sound3DEngine クラスから呼び出されています。
//!
//!           カスタマイズして独自の 3D サウンドエンジンを作成する場合には、
//!           このクラスの関数を利用することができます。
//!
//! @see Sound3DEngine クラス
//!
//---------------------------------------------------------------------------
class Sound3DCalculator
{
public:
    //---------------------------------------------------------------------------
    //! @brief  パン計算ためのパラメータ構造体です。
    //!
    //!         @ref CalculatePan でパン計算を行う際に参照するパラメータ構造体です。
    //!
    //!         stereoSpeakerAngle は、パン計算を行う際に使用するスピーカーの位置を設定します。
    //!         スピーカー配置は左右対称で、
    //!         左右それぞれのスピーカーの正面からの角度をラジアンで指定します。
    //!         ステレオモードのスピーカー配置は 45 度です。
    //!
    //!         surroundPanOffset はサラウンドパンに対するオフセット値を設定します。
    //!         設定する値は、通常のサラウンドパンの値と同じスケールで、-1.0f
    //!         を設定すると中央に位置していたサラウンドパンが最前方から発音されます。
    //!         1.0f を設定すると中央に位置していたサラウンドパンが最後方から発音されます。
    //!
    //! @see CalculatePan
    //!
    //---------------------------------------------------------------------------
    struct CalculatePanParam
    {
        //! ステレオモードのときのスピーカー位置の中央からの角度（ラジアン）です。
        float stereoSpeakerAngle;

        //! サラウンドモードのときのフロントスピーカー位置の中央からの角度（ラジアン）です。
        float surroundSpeakerFrontAngle;

        //! モードのときのリアスピーカー位置の中央からの角度（ラジアン）です。
        float surroundSpeakerRearAngle;

        //! サラウンドパンのオフセット値です。
        float surroundPanOffset;

        //---------------------------------------------------------------------------
        //! @brief    コンストラクタです。
        //!
        //---------------------------------------------------------------------------
        CalculatePanParam() NN_NOEXCEPT
        : stereoSpeakerAngle( nn::util::FloatPi / 4.0f ),              // 45度
          surroundSpeakerFrontAngle( nn::util::FloatPi / 6.0f ),       // 30度
          surroundSpeakerRearAngle( nn::util::FloatPi * 2.0f / 3.0f ), // 120度
          surroundPanOffset( 0.0f )
        {}
    };

    //! @name パラメータ計算
    //@{
    //---------------------------------------------------------------------------
    //! @brief  3D サウンドの音量と優先度の標準的なパラメータ計算を行います。
    //!
    //!         3D サウンドでは音量と優先度が連動しますので、
    //!         この関数でまとめて計算を行います。
    //!
    //!         この関数は、過去の互換性維持のために残されています。
    //!         現在の nn::atk ライブラリでは、計算処理削減のため、
    //!         当関数の代わりに actorDistance
    //!         引数付きのオーバーロード関数を呼び出しています。
    //!
    //! @param[out] pOutVolume        音量の計算結果を格納するポインタです。
    //! @param[out] pOutPriority      優先度の計算結果を格納するポインタです。
    //! @param[in] manager            計算に使用する 3D サウンドマネージャです。
    //! @param[in] listener           計算に使用する 3D サウンドリスナーです。
    //! @param[in] actorParam         計算に使用する 3D サウンドパラメータです。
    //!
    //! @see Sound3DManager クラス
    //! @see Sound3DListener クラス
    //! @see Sound3DParam 構造体
    //!
    //---------------------------------------------------------------------------
    static void CalculateVolumeAndPriority(
        float* pOutVolume,
        int* pOutPriority,
        const Sound3DManager& manager,
        const Sound3DListener& listener,
        const Sound3DParam& actorParam
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  3D サウンドの音量と優先度の標準的なパラメータ計算を行います。
    //!
    //!         3D サウンドでは音量と優先度が連動しますので、
    //!         この関数でまとめて計算を行います。
    //!
    //!         現在の nn::atk ライブラリでは、計算処理削減のため、
    //!         本関数の外側であらかじめ actorDistanceを計算しておき、
    //!             - 本関数
    //!             - @ref CalculatePitch
    //!             - @ref CalculateBiquadFilterValue
    //!         に計算済みの actorDistance を渡すような処理になっています。
    //!
    //!         詳しくは @ref Sound3DEngine::UpdateAmbientParam の実装をごらんください。
    //!
    //! @param[out] pOutVolume      音量の計算結果を格納するポインタです。
    //! @param[out] pOutPriority    優先度の計算結果を格納するポインタです。
    //! @param[in] manager          計算に使用する 3D サウンドマネージャです。
    //! @param[in] listener         計算に使用する 3D サウンドリスナーです。
    //! @param[in] actorParam       計算に使用する 3D サウンドパラメータです。
    //! @param[in] actorDistance    アクターとリスナーの距離です。
    //!
    //! @see Sound3DManager クラス
    //! @see Sound3DListener クラス
    //! @see Sound3DParam 構造体
    //!
    //---------------------------------------------------------------------------
    static void CalculateVolumeAndPriority(
        float* pOutVolume,
        int* pOutPriority,
        const Sound3DManager& manager,
        const Sound3DListener& listener,
        const Sound3DParam& actorParam,
        float  actorDistance
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    3D サウンドのパンの標準的なパラメータ計算を行います。
    //!
    //! @param[out] pOutPan     パンの計算結果を格納するポインタです。
    //! @param[out] pOutSPan    サラウンドパンの計算結果を格納するポインタです。
    //! @param[in] manager      計算に使用する 3D サウンドマネージャです。
    //! @param[in] listener     計算に使用する 3D サウンドリスナーです。
    //! @param[in] actorParam   計算に使用する 3D サウンドパラメータです。
    //! @param[in] calculatePanParam パン計算に使用するパラメータです。
    //!
    //! @see Sound3DManager クラス
    //! @see Sound3DListener クラス
    //! @see Sound3DParam 構造体
    //! @see CalculatePanParam 構造体
    //!
    //---------------------------------------------------------------------------
    static void CalculatePan(
        float* pOutPan,
        float* pOutSPan,
        const Sound3DManager& manager,
        const Sound3DListener& listener,
        const Sound3DParam& actorParam,
        const CalculatePanParam& calculatePanParam
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  3D サウンドの音程の標準的なパラメータ計算を行います。
    //!
    //!         この関数は、過去の互換性維持のために残されています。
    //!         現在の nn::atk ライブラリは、計算処理削減のため、
    //!         当関数の代わりに actorPosition および actorDistance
    //!         引数付きのオーバーロード関数を呼び出しています。
    //!
    //! @param[out] pOutValue         音程の計算結果を格納するポインタです。
    //! @param[in] manager            計算に使用する 3D サウンドマネージャです。
    //! @param[in] listener           計算に使用する 3D サウンドリスナーです。
    //! @param[in] actorParam         計算に使用する 3D サウンドパラメータです。
    //!
    //! @see Sound3DManager クラス
    //! @see Sound3DListener クラス
    //! @see Sound3DParam 構造体
    //!
    //---------------------------------------------------------------------------
    static void CalculatePitch(
        float* pOutValue,
        const Sound3DManager& manager,
        const Sound3DListener& listener,
        const Sound3DParam& actorParam
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  3D サウンドの音程の標準的なパラメータ計算を行います。
    //!
    //!         現在の nn::atk ライブラリでは、計算処理削減のため、
    //!         本関数の外側であらかじめ actorDistanceを計算しておき、
    //!             - 本関数
    //!             - @ref CalculateVolumeAndPriority
    //!             - @ref CalculateBiquadFilterValue
    //!         に計算済みの actorDistance を渡すような処理になっています。
    //!
    //!         詳しくは @ref Sound3DEngine::UpdateAmbientParam の実装をごらんください。
    //!
    //! @param[out] pOutValue       音程の計算結果を格納するポインタです。
    //! @param[in] manager          計算に使用する 3D サウンドマネージャです。
    //! @param[in] listener         計算に使用する 3D サウンドリスナーです。
    //! @param[in] actorParam       計算に使用する 3D サウンドパラメータです。
    //! @param[in] actorPosition    計算に使用する 3D サウンドアクターの座標です。
    //!                             リスナー座標との差分を渡します。
    //!                             詳しくは、@ref Sound3DEngine::UpdateAmbientParam
    //!                             の実装をごらんください。
    //! @param[in] actorDistance    アクターとリスナーの距離です。
    //!
    //! @see Sound3DManager クラス
    //! @see Sound3DListener クラス
    //! @see Sound3DParam 構造体
    //!
    //---------------------------------------------------------------------------
    static void CalculatePitch(
        float* pOutValue,
        const Sound3DManager& manager,
        const Sound3DListener& listener,
        const Sound3DParam& actorParam,
        const nn::util::Vector3fType& actorPosition,
        float actorDistance
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  3D サウンドの Biquad フィルタのかかり具合の標準的なパラメータ計算を行います。
    //!
    //!         この関数は、過去の互換性維持のために残されています。
    //!         現在の nn::atk ライブラリは、計算処理削減のため、
    //!         当関数の代わりに actorDistance
    //!         引数付きのオーバーロード関数を呼び出しています。
    //!
    //! @param[out] pOutValue            Biquad フィルタのかかり具合の計算結果を格納するポインタです。
    //! @param[in]  manager              計算に使用する 3D サウンドマネージャです。
    //! @param[in]  listener             計算に使用する 3D サウンドリスナーです。
    //! @param[in]  actorParam           計算に使用する 3D サウンドパラメータです。
    //!
    //! @see Sound3DManager クラス
    //! @see Sound3DListener クラス
    //! @see Sound3DParam 構造体
    //!
    //---------------------------------------------------------------------------
    static void CalculateBiquadFilterValue(
        float* pOutValue,
        const Sound3DManager& manager,
        const Sound3DListener& listener,
        const Sound3DParam& actorParam
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  3D サウンドの Biquad フィルタのかかり具合の標準的なパラメータ計算を行います。
    //!
    //!         現在の nn::atk ライブラリでは、計算処理削減のため、
    //!         本関数の外側であらかじめ actorDistanceを計算しておき、
    //!             - 本関数
    //!             - CalculateVolumeAndPriorityImpl (ライブラリの内部関数です。詳しくは実装をご覧ください)
    //!             - @ref CalculatePitch
    //!         に計算済みの actorDistance を渡すような処理になっています。
    //!         詳しくは @ref Sound3DEngine::UpdateAmbientParam の実装をごらんください。
    //!
    //! @param[out] pOutValue            Biquad フィルタのかかり具合の計算結果を格納するポインタです。
    //! @param[in]  manager              計算に使用する 3D サウンドマネージャです。
    //! @param[in]  listener             計算に使用する 3D サウンドリスナーです。
    //! @param[in]  actorParam           計算に使用する 3D サウンドパラメータです。
    //! @param[in]  actorDistance        アクターとリスナーの距離です。
    //!
    //! @see Sound3DManager クラス
    //! @see Sound3DListener クラス
    //! @see Sound3DParam 構造体
    //!
    //---------------------------------------------------------------------------
    static void CalculateBiquadFilterValue(
        float* pOutValue,
        const Sound3DManager& manager,
        const Sound3DListener& listener,
        const Sound3DParam& actorParam,
        float actorDistance
    ) NN_NOEXCEPT;
    //@}

private:
    static void CalcVolumeAndPriorityImpl(
        float actorDistance,
        SoundArchive::Sound3DInfo::DecayCurve decayCurve,
        float decayRatio,
        int maxPriorityReduction,
        float maxVolumeDistance,
        float unitDistance,
        float* volumePtr,
        int* priorityPtr
    ) NN_NOEXCEPT;
    static void CalculatePanImpl(
        const nn::util::Vector3fType& pos,
        float interiorSize,
        float actorDistance,
        float panRange,
        float stereoSpeakerAngle,
        float surroundSpeakerFrontAngle,
        float surroundSpeakerRearAngle,
        float surroundPanOffset,
        float* panPtr,
        float* surroundPanPtr
    ) NN_NOEXCEPT;
    static void CalculatePanSurround(
        const nn::util::Vector3fType& pos,
        float interiorSize,
        float actorDistance,
        float panRange,
        float surroundSpeakerFrontAngle,
        float surroundSpeakerRearAngle,
        float surroundPanOffset,
        float* panPtr,
        float* surroundPanPtr
    ) NN_NOEXCEPT;
#if 0
    static void CalcPanStereo(
        const nn::util::Vector3fType& pos,
        float interiorSize,
        float actorDistance,
        float panRange,
        float stereoSpeakerAngle,
        float* panPtr,
        float* surroundPanPtr
    );
#endif
    static void CalculateAngleAndDistance(
        const nn::util::Vector3fType& pos,
        float actorDistance,
        float interiorSize,
        float* anglePtr,
        float* distancePtr
    ) NN_NOEXCEPT;
};

} // namespace nn::atk
} // namespace nn
