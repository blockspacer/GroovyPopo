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
 * @file
 * @brief   VibrationMixer クラスの定義
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/hid/hid_VibrationNode.h>

namespace nn { namespace hid {

/**
 * @brief   振動値のミックス方式を表す列挙型です。
 */
enum VibrationMixMode
{
    VibrationMixMode_None,                      //!< ミックスを行わない (つねにデフォルトの振動値となる)
    VibrationMixMode_MaxAmplitude,              //!< 低周波帯と高周波帯の振幅の合計値が大きい方を採用する方式
    VibrationMixMode_MaxAmplitudePerSubband,    //!< 低周波帯と高周波帯それぞれで振幅が大きい方を採用する方式
    VibrationMixMode_AmplitudeSum               //!< 振幅値を加算する方式 (周波数は各入力を振幅で重み付けした加重平均値を採用)
};

/**
 * @brief   振動値のミキサーとしての機能を提供する振動ノードです。
 *
 * @details
 *  VibrationMixer は、接続元の振動ノードで発生した振動値を受け取って
 *  ミックスする機能を提供する振動ノードです。
 *
 *  ミックスされた振動値は、
 *  @ref nn::hid::VibrationNode::Update が呼ばれるたびに
 *  接続先の振動ノードに伝えられます。
 */
class VibrationMixer : public VibrationNode
{
    NN_DISALLOW_COPY(VibrationMixer);
    NN_DISALLOW_MOVE(VibrationMixer);

public:
    /**
     * @brief       コンストラクタです。@n
     *              本関数はスレッドセーフではありません。@n
     *              本関数と@ref nn::hid::VibrationNode::Update 関数を同時に呼ばないように排他制御をしてください。
     *
     * @post        GetMixMode() == MixMode_MaxAmplitudePerSubband
     */
    VibrationMixer() NN_NOEXCEPT;

    /**
     * @brief       デストラクタです。@n
     *              本関数はスレッドセーフではありません。@n
     *              本関数と@ref nn::hid::VibrationNode::Update 関数を同時に呼ばないように排他制御をしてください。
     */
    virtual ~VibrationMixer() NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief       振動値ミックス方式を指定します。
     *
     * @param[in]   mixMode     振動値ミックス方式
     *
     * @post        - GetMixMode() == mixMode
     */
    void SetMixMode(VibrationMixMode mixMode) NN_NOEXCEPT;

    /**
     * @brief       振動値ミックス方式を取得します。
     *
     * @return      振動値ミックス方式を返します。
     */
    VibrationMixMode GetMixMode() const NN_NOEXCEPT;

protected:
    virtual void OnNextSampleRequired(VibrationValue* pValue, VibrationNodeConnection::List* pInputConnections) NN_NOEXCEPT NN_OVERRIDE;

private:
    VibrationMixMode m_MixMode;  //!< 振動値ミックス方式
};

}} // namespace nn::hid
