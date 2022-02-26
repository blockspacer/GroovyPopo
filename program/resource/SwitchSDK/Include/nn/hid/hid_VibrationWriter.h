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
 * @brief   VibrationWriter クラスの定義
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/hid/hid_VibrationNode.h>

namespace nn { namespace hid {

/**
 * @brief   振動値を直接書き込むことで振動値の発生源としての機能を提供する振動ノードです。
 *
 * @details
 *  VibrationWriter は、リアルタイムで振動値を直接指定することによって
 *  振動を発生させる機能を提供する振動ノードです。
 *
 *  @ref nn::hid::VibrationWriter::Write で指定された振動値は、
 *  @ref nn::hid::VibrationNode::Update が呼ばれるたびに
 *  接続先の振動ノードに伝えられます。
 */
class VibrationWriter : public VibrationNode
{
    NN_DISALLOW_COPY(VibrationWriter);
    NN_DISALLOW_MOVE(VibrationWriter);

public:
    /**
     * @brief       コンストラクタです。@n
     *              本関数はスレッドセーフではありません。@n
     *              本関数と@ref nn::hid::VibrationNode::Update 関数を同時に呼ばないように排他制御をしてください。
     */
    VibrationWriter() NN_NOEXCEPT;

    /**
     * @brief       デストラクタです。@n
     *              本関数はスレッドセーフではありません。@n
     *              本関数と@ref nn::hid::VibrationNode::Update 関数を同時に呼ばないように排他制御をしてください。
     */
    virtual ~VibrationWriter() NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief       VibrationWriter に振動値をセットします。
     *
     * @param[in]   value       振動値
     *
     * @post
     *              - GetCurrentVibration() == value
     *
     * @details
     *  VibrationWriter に振動値をセットします。
     *  セットされた振動値は別の値で上書きされるまで有効です。@n
     *  セットされた振動値は、
     *  @ref nn::hid::VibrationNode::Update が呼ばれるたびに
     *  接続先の振動ノードに伝えられます。
     */
    void Write(const VibrationValue& value) NN_NOEXCEPT;

protected:
    virtual void OnNextSampleRequired(VibrationValue* pValue, VibrationNodeConnection::List* pInputConnections) NN_NOEXCEPT NN_OVERRIDE;
};

}} // namespace nn::hid
