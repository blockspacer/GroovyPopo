/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

#include <nn/nifm/nifm_Common.h>


namespace nn
{
namespace nifm
{

    //! @name ネットワーク接続要求関連 API
    //! @{

    /**
     * @brief   ネットワーク接続をシステムに要求します。
     *
     * @pre
     *   - ライブラリが初期化済み
     *
     * @details
     *  本関数を呼ぶと非同期処理が発生し、ネットワーク接続を利用できるかの検証がおこなわれます。
     *  検証中は @ref nn::nifm::IsNetworkRequestOnHold 関数が true を返します。
     *  アプリケーションは本 API のコール後、定期的に @ref nn::nifm::IsNetworkRequestOnHold の状態を確認し、
     *  @ref nn::nifm::IsNetworkRequestOnHold 関数が false を返したら
     *  @ref nn::nifm::IsNetworkAvailable 関数でネットワーク接続が利用できる状態になったかを確認してください。@n
     *
     *  ネットワーク接続の利用要求の検証は本 API のコール1回につき1回のみおこなわれます。
     *  検証完了後あらためてネットワーク接続の利用を要求するには、もう一度本 API を呼び直す必要があります。@n
     *
     *  この関数はブロックすることなくすぐに返ります。@n
     *
     *  この関数は @ref nn::nifm::SubmitNetworkRequestAndWait 関数とスレッドアンセーフです。
     *
     *  @platformbegin{Windows}
     *  Windows 版でこの関数を呼んでも Windows のネットワーク接続に影響は与えません。
     *  そのとき Windows がネットワークにつながっていれば利用可能、つながっていなければ利用不可という結果になります。@n
     *  また、例えば無線接続状態の Windows 環境において、有線の NIFM 接続設定のみが設定されている場合に利用要求を出しても、
     *  現在の状態がいずれの接続設定をも満たさないので利用可能状態になりません。
     *  この例の場合、実機版では、（可能ならば）有線接続に切り替わります。
     *  @platformend
     */
    void SubmitNetworkRequest() NN_NOEXCEPT;

    /**
     * @brief   ネットワーク接続をシステムに要求し、要求が受理もしくは却下されるまでブロックします。
     *
     * @pre
     *   - ライブラリが初期化済み
     *
     * @details
     *  本関数を呼ぶとネットワーク接続を利用できるかの検証がおこなわれ、検証の結果が出るまでブロックします。
     *  本関数を抜けたら @ref nn::nifm::IsNetworkAvailable 関数でネットワーク接続が利用できる状態になったかを確認してください。@n
     *
     *  ネットワーク接続の利用要求の検証は本 API のコール1回につき1回のみおこなわれます。
     *  検証完了後あらためてネットワーク接続の利用を要求するには、もう一度本 API を呼び直す必要があります。@n
     *
     *  システムの内部状態によって、検証処理はごく短時間で完了することがあります。@n
     *
     *  この関数は @ref nn::nifm::SubmitNetworkRequest 関数とスレッドアンセーフです。
     *
     *  @platformbegin{Windows}
     *  Windows 版でこの関数を呼んでも Windows のネットワーク接続に影響は与えません。
     *  そのとき Windows がネットワークにつながっていれば利用可能、つながっていなければ利用不可という結果になります。@n
     *  また、例えば無線接続状態の Windows 環境において、有線の NIFM 接続設定のみが設定されている場合に利用要求を出しても、
     *  現在の状態がいずれの接続設定をも満たさないので利用可能状態になりません。
     *  この例の場合、実機版では、（可能ならば）有線接続に切り替わります。
     *  @platformend
     */
    void SubmitNetworkRequestAndWait() NN_NOEXCEPT;

    /**
     * @brief   ネットワーク接続の要求を取り下げます。
     *
     * @pre
     *   - ライブラリが初期化済み
     *
     * @details
     *  システムに提出していたネットワーク接続の要求を取り下げます。@n
     *
     *  この関数は要求が検証中か否かに関わらず呼ぶことができます。
     *  @ref nn::nifm::SubmitNetworkRequestAndWait 関数のブロックを解除したい場合、この関数を別スレッドから呼んでください。@n
     *
     *  この関数はネットワーク接続に興味をなくしたことを宣言するだけであり、ネットワーク接続の切断を要求するものではありません。
     */
    void CancelNetworkRequest() NN_NOEXCEPT;

    /**
     * @brief   ネットワーク接続の利用要求が検証中であるかどうかを取得します。
     *
     * @return  要求が検証中であれば true を、そうでなければ false を返します。
     *
     * @details
     *  システムの内部状態によって、検証処理はごく短時間で完了することがあります。
     */
    bool IsNetworkRequestOnHold() NN_NOEXCEPT;

    /**
     * @brief   ネットワーク接続が利用可能であるかを取得します。
     *
     * @pre
     *   - ライブラリが初期化済み
     *
     * @return  ネットワーク接続が利用可能であれば true を、そうでなければ false を返します。
     *
     * @details
     *  この関数が true を返す状態であれば、ネットワーク接続を利用することができます。@n
     *
     *  無線 LAN アクセスポイントからの切断などの外部要因により、予期せず
     *  true から false へ遷移することがあることに注意してください。
     *  逆に、 @ref nn::nifm::SubmitNetworkRequest 関数もしくは @ref nn::nifm::SubmitNetworkRequestAndWait 関数による利用要求の提出を経ずして
     *  false から true へ遷移することはありません。
     */
    bool IsNetworkAvailable() NN_NOEXCEPT;

    /**
     * @brief   ローカルネットワークモードの有効・無効を指定します。
     *
     * @param[in]   isLocalNetworkMode  ローカルネットワークモードの有効・無効を指定します。
     *
     * @pre
     *   - ライブラリが初期化済み
     *   - 利用要求の提出前または取り下げ後
     *
     * @details
     *  ローカルネットワークモードは、ローカルネットワーク内でのマッチメイクをおこなう場合など、クローズドな LAN への接続を利用する場合に指定するモードです。
     *  ローカルネットワークモードを有効にしないと、クローズドな LAN への接続しかない環境ではネットワーク利用要求が受理されません。@n
     *
     *  インターネットへの接続が必要な場合には、ネットワーク利用要求の提出前にローカルネットワークモードを無効にしてください。
     *  ローカルネットワークモードでもインターネットへの通信が禁止されることはありませんが、インターネットに出られないネットワークを選択してしまったり、認証や同意が必要な公衆無線 LAN でインターネットに接続できなくなったりする副作用があります。@n
     *
     *  初期状態ではローカルネットワークモードは無効になっています。
     */
    void SetLocalNetworkMode(bool isLocalNetworkMode) NN_NOEXCEPT;

    /**
     * @deprecated 本関数は非推奨です。代わりに HandleNetworkRequestErrorResult() を使用してください。
     *
     * @brief 利用要求提出の結果に対するハンドリングを行います。
     *
     * @retresult
     *   @handleresult{
     *     nn::ResultSuccess, 利用要求は受理済みです。
     *   }
     *   @handleresult{
     *     ResultErrorHandlingCompleted, 利用要求は受理されていませんでしたが、エラーハンドリングが正常に行われました。エラーが解消された可能性があります。
     *   }
     *   - その他の Result の場合、利用要求は受理されておらず、エラーハンドリングにも失敗しています。
     * @endretresult
     *
     * @pre
     *   - ライブラリが初期化済み
     *
     * @details
     *  エラーに応じて、ハンドリング中にユーザー操作が必要になる場合があり、長時間ブロックする可能性があります。@n
     *
     *  この関数が成功を返した場合、利用要求は受理されておりネットワークを利用できます。@n
     *  この関数が失敗を返した場合、利用要求は却下されており、
     *    - 返された失敗が ResultErrorHandlingCompleted の場合、利用要求を再提出するとすぐに受理される可能性があります。@n
     *      再提出した要求が却下された場合でも、本 API を再度呼び出すことが可能です。
     *    - 返された失敗が その他の Result の場合、利用要求の再提出に対する結果を予測できないため、直前のシーケンスに戻ることを推奨します。
     *
     *  本 API によって，エラービューアの表示などが適宜行われますので、呼び出し側がエラーコードなどを利用者に提示する必要はありません。
     *
     *  また、ユーザー操作などにより利用要求が明示的に取り下げられた場合は、この関数を呼んでエラーの解消を試みる必要はありません。
     *
     *  @platformbegin{Windows}
     *  Windows 版では、利用要求が却下された際の本 API 呼び出しによって ResultErrorHandlingCompleted が返ることはありません。@n
     *  利用要求が受理されている場合は、通常通り ResultSuccess を返します。
     *  @platformend
     */
    NN_DEPRECATED nn::Result HandleNetworkRequestResult() NN_NOEXCEPT;

    /**
     * @brief 利用要求提出の失敗結果に対するハンドリングを行います。
     *
     * @return エラーハンドリングが成功すれば true を、そうでなければ false を返します。
     *
     * @pre
     *   - ライブラリが初期化済み
     *   - ネットワーク利用要求の提出結果が失敗
     *
     * @details
     *  エラーに応じて、ハンドリング中にユーザー操作が必要になる場合があり、長時間ブロックする可能性があります。@n
     *
     *  戻り値が true であれば、エラーが回復されており、利用要求を再提出するとすぐに受理される可能性があります。@n
     *  また、再提出した要求が却下された場合でも、本 API を再度呼び出すことが可能です。@n
     *  戻り値が false であれば、利用要求の再提出に対する結果を予測できないため、直前のシーケンスに戻ることを推奨します。
     *
     *  本 API によって、エラービューアの表示などが適宜行われます。
     *  また、ユーザー操作などにより利用要求が明示的に取り下げられた場合は、この関数を呼んでエラーの解消を試みる必要はありません。
     *
     *  @platformbegin{Windows}
     *  Windows 版では、利用要求が却下された際の本 API 呼び出しによって true が返ることはありません。@n
     *  @platformend
     */
    bool HandleNetworkRequestErrorResult() NN_NOEXCEPT;

    //! @}

}
}
