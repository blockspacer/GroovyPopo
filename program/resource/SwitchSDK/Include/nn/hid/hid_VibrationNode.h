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
#include <nn/nn_TimeSpan.h>
#include <nn/hid/hid_VibrationValueTypes.h>
#include <nn/hid/hid_VibrationNodeConnection.h>

namespace nn { namespace hid {

/**
 * @brief   振動ノードの基底クラスです。
 *
 * @details
 *  振動ノードは、 VibrationNodeConnection クラスと合わせて利用することによって、
 *  振動値を伝搬する機能を提供するユーティリティクラスです。@n
 *  振動ノードの各インスタンス、およびインスタンス間の接続情報は、
 *  全体として単一の有向グラフ (振動値フローの有向グラフ) を構成します。@n
 *  振動ノードの各インスタンスは、この有向グラフ上でノードとして機能し、
 *  VibrationNodeConnection の各インスタンスは弧 (有向辺) として機能します。
 *
 *  基底クラスである VibrationNode は、振動ノード間の接続に関する機能、
 *  および振動値を伝える際の調整係数の設定機能を提供します。@n
 *  派生クラスである VibrationPlayer, VibrationWriter,
 *  VibrationMixer, VibrationTarget は、
 *  それぞれ振動値の発生源、振動値のミキサー、振動値の送信先としての機能を提供します。@n
 *  また、 VibrationNode を継承することで、
 *  新規に異なる機能を持った振動ノードを作ることもできます。
 *
 *  Update() 関数が呼ばれると、
 *  すべての振動ノードの内部状態が更新されます。@n
 *  その際、隣接している振動ノード間では、接続元から接続先に対して振動値が伝えられます。
 *
 *  振動ノードは SetModulationTo() 関数を利用することで
 *  送信先ごとに異なる調整係数を設定できるため、
 *  単一の振動ノードから送信先ごとに異なる振動値を伝えることができます。
 *
 *  本クラスおよび派生クラスのコンストラクタとデストラクタおよび Update() 関数はスレッドセーフではありません。@n
 *  コンストラクタとデストラクタは Update() 関数と同時に呼ばないように排他制御をしてください。
 */
class VibrationNode : public nn::util::IntrusiveListBaseNode<VibrationNode>
{
    NN_DISALLOW_COPY(VibrationNode);
    NN_DISALLOW_MOVE(VibrationNode);
    friend class VibrationNodeConnection;

public:
    /**
     * @brief       コンストラクタです。@n
     *              内部状態を初期化した上で、このインスタンスを振動値フローの有向グラフのノードとして登録します。@n
     *              本関数はスレッドセーフではありません。@n
     *              本関数と Update() 関数を同時に呼ばないように排他制御をしてください。
     *
     * @post
     *              - GetCurrentVibration() == デフォルト値
     */
    VibrationNode() NN_NOEXCEPT;

    /**
     * @brief       デストラクタです。@n
     *              このインスタンスを振動値フローの有向グラフから削除します。@n
     *              本関数はスレッドセーフではありません。@n
     *              本関数と Update() 関数を同時に呼ばないように排他制御をしてください。
     */
    virtual ~VibrationNode() NN_NOEXCEPT;

    /**
     * @brief       内部で保持している振動値を取得します。
     *
     * @return      内部で保持している振動値を返します。
     */
    VibrationValue GetCurrentVibration() const NN_NOEXCEPT;

    /**
     * @brief       別の振動ノードへの接続を切断します。
     *
     * @param[in]   pDestination    接続先の振動ノード
     *
     * @pre
     *              - pDestination != nullptr
     *              - IsConnectedTo(pDestination) == true
     */
    void DisconnectFrom(VibrationNode* pDestination) NN_NOEXCEPT;

    /**
     * @brief       別の振動ノードに接続されているか取得します。
     *
     * @return      接続されていたら true を返します。
     *
     * @pre
     *              - pDestination != nullptr
     *
     * @param[in]   pDestination    接続先の振動ノード
     */
    bool IsConnectedTo(const VibrationNode* pDestination) const NN_NOEXCEPT;

    /**
     * @brief       別の振動ノードへの接続を表す VibrationNodeConnection を取得します。
     *
     * @return      接続されていたら、該当の接続に関する情報を保持しているインスタンスへのポインタを返します。
     *              接続されていない場合には nullptr を返します。
     *
     * @pre
     *              - pDestination != nullptr
     *
     * @param[in]   pDestination    接続先の振動ノード
     */
    VibrationNodeConnection* GetConnectionTo(const VibrationNode* pDestination) const NN_NOEXCEPT;

    /**
     * @brief       接続先の振動ノードに対する調整係数を設定します。
     *
     * @param[in]   pDestination     接続先の振動ノード
     * @param[in]   modulation  接続先の振動ノードに対する調整係数
     *
     * @pre
     *              - pDestination != nullptr
     *              - IsConnectedTo(pDestination) == true
     */
    void SetModulationTo(const VibrationNode* pDestination, const VibrationModulation& modulation) NN_NOEXCEPT;

    /**
     * @brief       接続先の振動ノードに対する調整係数を取得します。
     *
     * @return      接続先の振動ノードに対する調整係数を返します。
     *
     * @param[in]   pDestination    接続先の振動ノード
     *
     * @pre
     *              - pDestination != nullptr
     *              - IsConnectedTo(pDestination) == true
     */
    VibrationModulation GetModulationTo(const VibrationNode* pDestination) const NN_NOEXCEPT;

protected:
    /**
     * @brief       内部で保持している振動値を設定します。
     *
     * @param[in]   value   振動値
     */
    void SetCurrentVibration(const VibrationValue& value);

    /**
     * @brief       内部の振動値を更新する必要が生じた際に呼ばれる関数です。
     *
     * @param[in,out]   pValue              内部で保持している振動値へのポインタ
     * @param[in]       pInputConnections   この振動ノードに対する接続のリスト
     *
     * @details
     *  この関数が呼ばれる時点で、接続元の振動ノードの振動値は既に更新済みの状態であることが
     *  保証されています。@n
     *  派生クラスはこの関数をオーバーライドすることによって、
     *  それぞれ固有の機能を実現します。
     */
    virtual void OnNextSampleRequired(VibrationValue* pValue, VibrationNodeConnection::List* pInputConnections) NN_NOEXCEPT = 0;

private:
    VibrationValue m_VibrationValue;                    //!< 内部で保持している振動値
    VibrationNodeConnection::List m_InputConnections;   //!< この振動ノードへの接続のリスト
    uint8_t m_SampleNumber;                             //!< 有向グラフ全体で同期をとるためのサンプル番号
    void SyncSampleNumber() NN_NOEXCEPT;                //!< 有向グラフ全体で同期をとる際に呼ばれる関数

public:
    /**
     * @brief       振動値の更新間隔のデフォルト値です。
     */
    static const nn::TimeSpan DefaultVibrationSampleInterval;

    /**
     * @brief       振動値フローの有向グラフ全体を更新して、
     *              それぞれの振動ノードが接続先に振動値を伝えます。
     *
     * @details
     *  この関数が呼ばれると、すべての振動ノードの内部状態が更新されます。@n
     *  その際、隣接している振動ノード間では、接続元から接続先に対して振動値が伝えられます。@n
     *  接続元の更新処理は接続先の更新処理よりも先に行われます。@n
     *
     *  この関数は任意のタイミングで実行することができますが、
     *  タイマー等を利用して、@ref DefaultVibrationSampleInterval 間隔で
     *  実行することが推奨されます。
     *
     *  本関数はスレッドセーフではありません。@n
     *  本関数と本クラスおよび派生クラスのコンストラクタおよびデストラクタを同時に呼ばないように排他制御をしてください。
     *
     */
    static void Update() NN_NOEXCEPT;

protected:
    typedef nn::util::IntrusiveList<
        VibrationNode,
        nn::util::IntrusiveListBaseNodeTraits<VibrationNode>> List;
};

}} // namespace nn::hid
