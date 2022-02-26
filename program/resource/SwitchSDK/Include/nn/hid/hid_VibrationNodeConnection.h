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
 * @brief   振動ノードの基底クラス
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/util/util_IntrusiveList.h>
#include <nn/hid/hid_VibrationModulationTypes.h>

namespace nn { namespace hid {

class VibrationNode;

/**
 * @brief   振動ノード間の接続を表すクラスです。
 *
 * @details
 *  VibrationNodeConnection は、振動ノード (VibrationNode) と合わせて利用することによって、
 *  振動値を伝搬する機能を提供するユーティリティクラスです。@n
 *  VibrationNodeConnection クラスのインスタンスは、
 *  振動ノード間の接続を表すことができます。
 *
 *  VibrationNodeConnection クラスのインスタンスには、
 *  振動ノード間の接続を表している状態と、接続を表していない状態があります。@n
 *  現在どちらの状態であるかは IsConnected() 関数で取得することができます。
 *
 *  振動を表している状態のインスタンスは、内部的に接続元と接続先の情報を保持しており、
 *  ひとつの振動ノードから別の振動ノードへの接続を表します。@n
 *  振動ノードをノードとする有向グラフ構造においては弧 (有向辺) として機能します。
 *
 *  一方で接続を表していない状態のインスタンスは、そのままでは有向グラフ上での機能を持ちません。@n
 *  Connect() 関数を呼ぶことにより、接続を表す状態にすることができます。
 */
class VibrationNodeConnection : public nn::util::IntrusiveListBaseNode<VibrationNodeConnection>
{
public:
    typedef nn::util::IntrusiveList<
        VibrationNodeConnection,
        nn::util::IntrusiveListBaseNodeTraits<VibrationNodeConnection>> List;

public:
    /**
     * @brief       コンストラクタです。
     *              接続を表していない状態のインスタンスを生成します。
     *
     * @post
     *              - IsConnected() == false
     *              - GetSource() == nullptr
     *              - GetDestination() == nullptr
     *              - GetModulation() == デフォルト値
     *
     * @details
     *  インスタンスは接続に関する情報を持たない状態で初期化されます。@n
     *  振動ノード間の接続を表している状態にするためには、明示的に Connect 関数を呼ぶ必要があります。
     */
    VibrationNodeConnection() NN_NOEXCEPT;

    /**
     * @brief       コンストラクタです。
     *              振動ノード間の接続を表すインスタンスを生成します。
     *
     * @param[in]   pSource         接続元の振動ノード
     * @param[in]   pDestination    接続先の振動ノード
     *
     * @pre
     *              - pSource != nullptr
     *              - pDestination != nullptr
     *              - pSource->IsConnectedTo(pDestination) == false
     *
     * @post
     *              - IsConnected() == true
     *              - GetSource() == pSource
     *              - GetDestination() == pDestination
     *              - pSource->GetConnectionTo(pDestination) == this
     *              - GetModulation() == デフォルト値
     *
     * @details
     *  指定されたノード間が接続されるとともに、
     *  このインスタンスはノード間の接続を表す状態で初期化されます。
     *
     *  事前条件が満たされなかった場合には、接続を表していない状態で初期化されます。
     */
    VibrationNodeConnection(VibrationNode* pSource, VibrationNode* pDestination) NN_NOEXCEPT;

    /**
     * @brief       デストラクタです。
     *              インスタンスが破棄されるとともに接続が切断されます。
     *
     * @details
     *  このインスタンスが振動ノード間の接続を表している状態の場合には、
     *  デストラクタの呼び出しによって接続元振動ノードから接続先振動ノードへの接続も自動的に解除されます。
     */
    virtual ~VibrationNodeConnection() NN_NOEXCEPT;

    /**
     * @brief       振動ノード間を接続します。
     *
     * @return      接続に成功したら true を返します。
     *
     * @param[in]   pSource         接続元の振動ノード
     * @param[in]   pDestination    接続先の振動ノード
     *
     * @pre
     *              - pSource != nullptr
     *              - pDestination != nullptr
     *              - pSource->IsConnectedTo(pDestination) == false
     *
     * @post
     *              - IsConnected() == true
     *              - GetSource() == pSource
     *              - GetDestination() == pDestination
     *              - pSource->GetConnectionTo(pDestination) == this
     *
     * @details
     *  指定されたノード間が接続されるとともに、
     *  このインスタンスはノード間の接続を表す状態になります。@n
     *  このインスタンスのデストラクタが呼ばれると自動的に接続が解除されるので注意してください。@n
     *
     *  また接続する際には、振動フローの有向グラフにおいて巡回構造が発生しないように注意してください。
     */
    bool Connect(VibrationNode* pSource, VibrationNode* pDestination) NN_NOEXCEPT;

    /**
     * @brief       振動ノード間の接続を表している状態か取得します。
     *
     * @return      接続を表している状態であれば true を返します。
     */
    bool IsConnected() const NN_NOEXCEPT;

    /**
     * @brief       振動ノード間の接続を解除します。
     *
     * @pre
     *              - IsConnected() == true
     *
     * @post
     *              - IsConnected() == true
     *              - GetSource() == nullptr
     *              - GetDestination() == nullptr
     *
     * @details
     *  このインスタンスが振動ノード間の接続を表している状態でこの関数が呼ばれると、
     *  該当のノード間の接続が解除されるとともに、このインスタンスは接続を表していない状態になります。
     */
    void Disconnect() NN_NOEXCEPT;

    /**
     * @brief       接続元の振動ノードを取得します。
     *
     * @return      このインスタンスが接続を表している状態の場合は、接続元の振動ノードへのポインタを返します。
     *              接続を表していない状態の場合には nullptr を返します。
     */
    VibrationNode* GetSource() const NN_NOEXCEPT;

    /**
     * @brief       接続先の振動ノードを取得します。
     *
     * @return      このインスタンスが接続を表している状態の場合は、接続先の振動ノードへのポインタを返します。
     *              接続を表していない状態の場合には nullptr を返します。
     */
    VibrationNode* GetDestination() const NN_NOEXCEPT;

    /**
     * @brief       接続元から接続先に振動値を伝える際に適用される調整係数を設定します。
     *
     * @param[in]   modulation  接続元から接続先に振動値を伝える際に適用される調整係数
     */
    void SetModulation(const VibrationModulation& modulation) NN_NOEXCEPT;

    /**
     * @brief       接続元から接続先に振動値を伝える際に適用される調整係数を取得します。
     *
     * @return      調整係数を返します。
     */
    VibrationModulation GetModulation() const NN_NOEXCEPT;

private:
    VibrationNode* m_pSource;
    VibrationNode* m_pDestination;
    VibrationModulation m_Modulation;
};

}} // namespace nn::hid
