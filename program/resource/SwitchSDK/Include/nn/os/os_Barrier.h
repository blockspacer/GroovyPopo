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
 * @brief   バリアに関する公開ヘッダファイル
 */

#pragma once

#include <nn/os/os_BarrierTypes.h>
#include <nn/os/os_BarrierApi.h>

namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   バリアを扱うためのクラスです。
 *
 * @details
 *  バリアは複数スレッドで待ち合わせを行うための同期オブジェクトです。
 *
 *  nn::os::Barrier クラスは、@ref nn::os::BarrierType オブジェクトと
 *  関連する同期制御 API の呼び出しをラッピングしたユーティリティクラスです。
 *  本クラスが提供するメンバ関数は、@ref nn::os::BarrierType 型を引数にとる
 *  nn::os::AwaitBarrier() などの API を発行する形で実装されています。
 */
class Barrier
{
    NN_DISALLOW_COPY( Barrier );
    NN_DISALLOW_MOVE( Barrier );

public:
    //------------------------------------------------------------------------
    /**
     * @brief   バリアオブジェクトを構築し初期化します。
     *
     * @param[in]   numThreads 待ち合わせたいスレッド数
     *
     * @details
     *  コンストラクタです。@n
     *  バリアオブジェクトを構築し初期化します。@n
     *  詳細は nn::os::InitializeBarrier() を参照して下さい。
     */
    explicit Barrier(int numThreads) NN_NOEXCEPT
    {
        InitializeBarrier(&m_Barrier, numThreads);
    }

    /**
     * @brief  バリアオブジェクトを Finalize し破棄します。
     *
     * @details
     *  デストラクタです。@n
     *  バリアオブジェクトを Finalize し破棄します。@n
     *  詳細は nn::os::FinalizeBarrier() を参照して下さい。
     */
    ~Barrier() NN_NOEXCEPT
    {
        FinalizeBarrier(&m_Barrier);
    }

    //--------------------------------------------------------------------------
    /**
     * @brief   バリアオブジェクトの待ち合わせを行ないます。
     *
     * @details
     *  バリアオブジェクトの待ち合わせを行います。@n
     *  詳細は nn::os::AwaitBarrier() を参照して下さい。
     */
    void Await() NN_NOEXCEPT
    {
        AwaitBarrier(&m_Barrier);
    }

    /**
     * @brief   BarrierType オブジェクトへの参照を返します。
     *
     * @return  BarrierType オブジェクトへの参照
     *
     * @details
     *  自インスタンスが持つ BarrierType オブジェクトへの参照を返します。
     *  この変換演算子があるため、BarrierType& を引数にとる関数に対して、
     *  Barrier& を指定することが可能です。
     */
    NN_IMPLICIT operator BarrierType&() NN_NOEXCEPT
    {
        return m_Barrier;
    }

    /**
     * @brief   BarrierType オブジェクトへの const 参照を返します。
     *
     * @return  BarrierType オブジェクトへの const 参照
     *
     * @details
     *  自インスタンスが持つ BarrierType オブジェクトへの const 参照を返します。
     *  この変換演算子があるため、const BarrierType& を引数にとる関数に対して、
     *  Barrier& を指定することが可能です。
     */
    NN_IMPLICIT operator const BarrierType&() const NN_NOEXCEPT
    {
        return m_Barrier;
    }

    /**
     * @brief   自インスタンスの BarrierType オブジェクトへのポインタを返します。
     *
     * @return  BarrierType オブジェクトへのポインタ
     *
     * @details
     *  自インスタンスが持つ BarrierType オブジェクトへのポインタを返します。
     */
    BarrierType* GetBase() NN_NOEXCEPT
    {
        return &m_Barrier;
    }

private:
    BarrierType m_Barrier;
};

}}  // namespace nn::os

