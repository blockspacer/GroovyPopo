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
 * @brief  スレッドローカルストレージに関する公開ヘッダファイル
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Abort.h>
#include <nn/os/os_ThreadLocalStorageCommon.h>
#include <nn/os/os_ThreadLocalStorageApi.h>


namespace nn { namespace os {

//--------------------------------------------------------------------------
/**
 * @brief   スレッドローカルストレージを扱うためのクラスです。
 *
 * @details
 *  スレッドローカルストレージ（以後 TLS）は、単一の TLS スロット に対して
 *  スレッド毎に異なるデータを扱うスレッド固有データを操作するための機能です。
 *
 *  nn::os::ThreadLocalStorage クラスは、@ref nn::os::TlsSlot オブジェクトを
 *  使用する TLS 関連機能の呼び出しをラッピングしたユーティリティクラスです。@n
 *  本クラスが提供するメンバ関数は、@ref nn::os::TlsSlot 型を引数にとる
 *  @ref nn::os::GetTlsValue() などの API を発行する形で実装されています。
 */
class ThreadLocalStorage
{
    NN_DISALLOW_COPY( ThreadLocalStorage );
    NN_DISALLOW_MOVE( ThreadLocalStorage );

public:
    /**
     * @brief   TLS オブジェクトを構築し初期化します。
     *
     * @overloadlist{nn_os_ThreadLocalStorage_ThreadLocalStorage}
     *
     * @details
     *  コンストラクタです。@n
     *  TLS スロットを１つ確保します。@n
     *  各スレッドの TLS 初期値は 0 となります。@n
     *  詳細は nn::os::AllocateTlsSlot() を参照して下さい。
     */
    ThreadLocalStorage() NN_NOEXCEPT
    {
        auto result = nn::os::AllocateTlsSlot(&m_TlsSlot, NULL);
        NN_ABORT_UNLESS_RESULT_SUCCESS(result);
    }

    /**
     * @brief   TLS オブジェクトを構築し初期化します。
     *
     * @overloadlist{nn_os_ThreadLocalStorage_ThreadLocalStorage}
     *
     * @param[in]  function デストラクタ用コールバック関数エントリ
     *
     * @details
     *  コンストラクタです。@n
     *  TLS スロットを１つ確保し、デストラクタ関数を登録します。
     *  デストラクタ関数はスレッド終了時にコールバックされます。@n
     *  各スレッドの TLS 初期値は 0 となります。@n
     *  詳細は nn::os::AllocateTlsSlot() を参照して下さい。
     */
    explicit ThreadLocalStorage(TlsDestructor function) NN_NOEXCEPT
    {
        auto result = nn::os::AllocateTlsSlot(&m_TlsSlot, function);
        NN_ABORT_UNLESS_RESULT_SUCCESS(result);
    }

    /**
     * @brief  デストラクタです。
     *
     * @details
     *  TLS スロットを返却し、オブジェクトを破棄します。@n
     *  詳細は nn::os::FreeTlsSlot() を参照して下さい。
     */
    ~ThreadLocalStorage() NN_NOEXCEPT
    {
        nn::os::FreeTlsSlot(m_TlsSlot);
    }

    /**
     * @brief   TLS スロットに値を設定します。
     *
     * @param[in] value     設定したい TLS 値
     *
     * @details
     *  自スレッドの TLS スロットに値を設定します。@n
     *  詳細は nn::os::SetTlsValue() を参照して下さい。
     */
    void SetValue(uintptr_t value) NN_NOEXCEPT
    {
        return nn::os::SetTlsValue(m_TlsSlot, value);
    }

    /**
     * @brief   TLS スロットにセットされている値を取得します。
     *
     * @return  TLS の値が返ります
     *
     * @details
     *  自スレッドの TLS スロットから値を取得します。@n
     *  詳細は nn::os::GetTlsValue() を参照して下さい。
     */
    uintptr_t GetValue() const NN_NOEXCEPT
    {
        return nn::os::GetTlsValue(m_TlsSlot);
    }


    /**
     * @brief   自インスタンスの TlsSlot を返します。
     *
     * @return  nn::os::TlsSlot オブジェクトが返ります
     *
     * @details
     *  自インスタンスが持つ nn::os::TlsSlot オブジェクトを返します。
     */
    TlsSlot GetTlsSlot() const NN_NOEXCEPT
    {
        return m_TlsSlot;
    }

private:
    TlsSlot m_TlsSlot;
};

//--------------------------------------------------------------------------

}} // namespace nn::os

