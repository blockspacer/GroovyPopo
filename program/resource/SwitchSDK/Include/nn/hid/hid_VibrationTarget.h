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
 * @brief   VibrationTarget クラスの定義
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/hid/hid_VibrationMixer.h>
#include <nn/hid/hid_VibrationDeviceApi.h>

namespace nn { namespace hid {

/**
 * @brief   振動値の送信先としての機能を提供する振動ノードです。
 *
 * @details
 *  VibrationTarget は、他の振動ノードから送られた振動値を
 *  実際に振動子に送信する機能を提供する振動ノードです。@n
 *  内部的には @ref nn::hid::SendVibrationValue 関数を利用して振動値を送信します。@n
 *  @ref nn::hid::VibrationMixer クラスを継承しているため、
 *  複数の振動ノードから送られた振動値を振動子に送る前にミックスすることもできます。
 *
 *  ミックスされた振動値は、
 *  @ref nn::hid::VibrationNode::Update が呼ばれるたびに
 *  対象の振動子に送信されます。
 */
class VibrationTarget : public VibrationMixer
{
    NN_DISALLOW_COPY(VibrationTarget);
    NN_DISALLOW_MOVE(VibrationTarget);

public:
    /**
     * @brief       コンストラクタです。@n
     *              本関数はスレッドセーフではありません。@n
     *              本関数と@ref nn::hid::VibrationNode::Update 関数を同時に呼ばないように排他制御をしてください。
     *
     * @post
     *              - IsVibrationDeviceHandleSet() == false
     */
    VibrationTarget() NN_NOEXCEPT;

    /**
     * @brief       振動子に対応した VibrationTarget クラスを初期化します。@n
     *              本関数はスレッドセーフではありません。@n
     *              本関数と@ref nn::hid::VibrationNode::Update 関数を同時に呼ばないように排他制御をしてください。
     *
     * @param[in]   handle          振動子のハンドル
     *
     * @post
     *              - IsVibrationDeviceHandleSet() == true
     *              - GetVibrationDeviceHandle() == handle
     * @details
     *  コンストラクタです。@n
     *  同一の振動子に対して複数の VibrationTarget インスタンスを生成しないよう注意してください。
     */
    explicit VibrationTarget(const VibrationDeviceHandle& handle) NN_NOEXCEPT;

    /**
     * @brief       デストラクタです。@n
     *              本関数はスレッドセーフではありません。@n
     *              本関数と@ref nn::hid::VibrationNode::Update 関数を同時に呼ばないように排他制御をしてください。
     */
    virtual ~VibrationTarget() NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief       対応する振動子のハンドルを設定します。
     *
     * @param[in]   handle  振動子のハンドル
     *
     * @post
     *              - IsVibrationDeviceHandleSet() == true
     *              - GetVibrationDeviceHandle() == handle
     *
     * @details
     *  既に振動子のハンドルが設定済みのインスタンスに対して再度この関数を実行した場合、
     *  現時点で設定されている振動子との対応関係が解除された上で、
     *  新たに指定された振動子との対応関係が構築されます。@n
     *  同一の振動子に対応する VibrationTarget インスタンスが複数存在する状態にならないよう注意してください。
     */
    void SetVibrationDeviceHandle(const VibrationDeviceHandle& handle) NN_NOEXCEPT;

    /**
     * @brief       振動子のハンドルが設定されていない状態にします。
     *
     * @post
     *              - IsVibrationDeviceHandleSet() == false
     *
     * @details
     *  既に振動子のハンドルが設定されていない状態になっているインスタンスに対して
     *  再度この関数を実行しても何も起こりません。
     */
    void UnsetVibrationDeviceHandle() NN_NOEXCEPT;

    /**
     * @brief       振動子のハンドルが設定されているか取得します。
     *
     * @return      振動子のハンドルが設定されているならば true を返します。
     */
    bool IsVibrationDeviceHandleSet() const NN_NOEXCEPT;

    /**
     * @brief       対応する振動子のハンドルを取得します。
     *
     * @return      対応する振動子のハンドルを返します。
     */
    VibrationDeviceHandle GetVibrationDeviceHandle() NN_NOEXCEPT;

    /**
     * @brief       対応する振動子で発生している実際の振動値を取得します。
     *
     * @param[out]  pOutValue   実際の振動値の格納先
     *
     * @pre         有効な振動子のハンドルがこのインスタンスにセットされていること
     *
     * @details
     *  VibrationTarget に対応する振動子で発生している実際の振動値を取得します。@n
     *  詳細は @ref nn::hid::GetActualVibrationValue 関数を参照してください。
     */
    void GetActualVibration(VibrationValue* pOutValue) const NN_NOEXCEPT;

    /**
     * @brief       対応する振動子の情報を取得します。
     *
     * @pre         有効な振動子のハンドルがこのインスタンスにセットされていること
     *
     * @param[out]  pOutValue   振動子の情報の格納先
     */
    void GetVibrationDeviceInfo(VibrationDeviceInfo* pOutValue) const NN_NOEXCEPT;

protected:
    virtual void OnNextSampleRequired(VibrationValue* pValue, VibrationNodeConnection::List* pInputConnections) NN_NOEXCEPT NN_OVERRIDE;

private:
    bool m_IsHandleSet;
    VibrationDeviceHandle m_Handle;
};

}} // namespace nn::hid
