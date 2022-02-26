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

#include <nn/account/account_Types.h>

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/os/os_SystemEvent.h>

namespace nn { namespace account { namespace detail {

class IAsyncContext;

}}} // ~namespace nn::account::detail

namespace nn { namespace account {

/**
    @brief 事前に呼び出した非同期処理の状況を表すクラス
    @details
        事前に呼び出した非同期処理について、次を行います。
        - 非同期処理の完了の通知を受けるイベントの取得
        - 非同期処理の完了, 未完了の状態の取得
        - 非同期処理が完了している場合、その結果の取得
        - 非同期処理が未完了の場合、その中止の要求

        AsyncContext オブジェクトは次の状態をもちます。
        - 無効
            - オブジェクトはいかなる非同期処理とも関連づきません。
            - この状態では、デストラクタ以外を呼び出すことはできません。
        - 有効
            - このオブジェクトは特定の非同期処理と関連づきます。
            - この状態では関連づく非同期処理に関して先述の操作が可能です。

        それぞれの非同期APIの引数に AsyncContext オブジェクトを指定することで、有効なオブジェクトを取得できます。
        これらの非同期APIは、引数に指定した AsyncContext オブジェクトのそれまでの内容を破棄することに留意してください。
        すなわち、指定した AsyncContext オブジェクトが有効で非同期処理が実行中の場合、これらの呼出しは失敗します。
 */
class AsyncContext
{
    NN_DISALLOW_COPY(AsyncContext);

private:
    Uid m_User;
    detail::IAsyncContext* m_Ptr;

    NN_EXPLICIT_OPERATOR bool() const NN_NOEXCEPT;

protected:
    AsyncContext& Swap(AsyncContext& rhs) NN_NOEXCEPT;
    detail::IAsyncContext* GetPointerUnsafe() const NN_NOEXCEPT;

public:
    explicit AsyncContext(detail::IAsyncContext* ptr) NN_NOEXCEPT;
    AsyncContext(detail::IAsyncContext* ptr, const Uid& user) NN_NOEXCEPT;

    /**
        @brief 無効なインスタンスを作成するデフォルトコンストラクタ

        @post
            - *this は無効

        @details
            無効なインスタンスを作成します。
            この状態では、デストラクタ以外のいかなる API も呼出すことができません。

            非同期処理 API の引数に指定することで、有効化できます。
    */
    AsyncContext() NN_NOEXCEPT;
    /**
        @brief ムーブコンストラクタ

        @param[in] rhs 既存のインスタンス。状態は問いません。

        @post
            - rhs は無効
            - rhs から事前に取得したイベントは新しいインスタンスで管理される

        @details
            右辺に指定したインスタンスの内容を新しく作成するインスタンスに移動します。
    */
    AsyncContext(AsyncContext&& rhs) NN_NOEXCEPT;
    /**
        @brief 非同期処理が実行中の場合に呼び出しが失敗するデストラクタ

        @details
            インスタンスを破棄します。
            このインスタンスから事前に取得したイベントは無効になります。

            もしインスタンスが有効で非同期処理が実行中の場合、デストラクタの呼出しは失敗します。
     */
    virtual ~AsyncContext() NN_NOEXCEPT;
    /**
        @brief ムーブ代入演算子

        @param[in] rhs 既存のインスタンス。状態は問いません。

        @post
            - rhs は無効
            - rhs から事前に取得したイベントは左辺に指定したインスタンスで管理される
            - 左辺から事前に取得したイベントは無効

        @details
            右辺に指定したインスタンスの内容を、左辺に指定したインスタンスに移動します。
            左辺に指定したインスタンスのそれまでの内容は破棄されます。

            もし左辺に指定したインスタンスが有効で非同期処理が実行中の場合、本演算子の呼出しは失敗します。
    */
    AsyncContext& operator=(AsyncContext&& rhs) NN_NOEXCEPT;
    /**
        @brief 非同期処理の完了がシグナルされるイベントを取得します。

        @param[out] pOutEvent 取得したイベントの格納先

        @retresult
            @handleresult{
                nn::ResultSuccess,
                処理に成功しました。
            }
        @endretresult

        @pre
            - *this が有効
            - *pOutEvent がデフォルト初期化の状態
        @post
            - 非同期処理完了時点で *pOutEvent がシグナルされる
            - *pOutEvent のクリアモードが nn::os::ClearMode_ManualClear

        @details
            非同期処理の完了通知がシグナルされるイベントを取得します。
            このイベントは *this が有効な期間使用することができます。
            従って、 *this を破棄したあとにこのイベントを参照してはいけません。
    */
    Result GetSystemEvent(os::SystemEvent* pOutEvent) NN_NOEXCEPT;
    /**
        @brief 非同期処理の中止を要求します。

        @retresult
            @handleresult{
                nn::ResultSuccess,
                処理に成功しました。
            }
        @endretresult

        @pre
            - *this が有効

        @details
            非同期処理の中止を要求します。
            この API は中止の完了を待たずに返ります。

            中止を要求したあと非同期処理は可能な限り早い時点で完了し、その完了通知がシグナルされます。
            非同期処理が既に完了している場合、本関数は何も行いません。
    */
    Result Cancel() NN_NOEXCEPT;
    /**
        @brief 非同期処理が完了しているかどうかを取得します。

        @param[out] pOut 取得した状態の格納先

        @retresult
            @handleresult{
                nn::ResultSuccess,
                処理に成功しました。
            }
        @endretresult

        @pre
            - *this が有効
        @post
            - タスクが完了状態の場合に限り *pOut == true

        @details
            非同期処理が完了しているかどうかを取得します。
            非同期処理の完了には、非同期処理の中止の完了も含まれます。
    */
    Result HasDone(bool* pOut) NN_NOEXCEPT;
    /**
        @brief 非同期処理の結果を取得します。

        @retresult
            @handleresult{
                nn::ResultSuccess,
                処理に成功しました。
            }
            @handleresult{
                nn::account::ResultCancelled,
                Cancel() 関数の呼び出しによって処理がキャンセルされました。
                この結果をエラーとして扱わないでください。
            }
            @handleresult{
                nn::account::ResultNetworkServiceAccountUnavailable,
                （この結果は、 *this の取得に使用した関数のAPIリファレンスに明記されている場合に限り返ることがあります。）
                非同期処理中にネットワークサービスアカウントが利用不可能であると判断されました。
                EnsureNetworkServiceAccountAvailable() を実行することでネットワークサービスアカウントを利用可能な状態にし、このエラーを解消することができます。
            }
            @handleresult{
                nn::account::ResultNetworkCommunicationError,
                （この結果は、 *this の取得に使用した関数のAPIリファレンスにインターネットとの通信の可能性が明記されている場合に限り返ることがあります。）
                非同期処理中のネットワーク通信において問題が発生しました。
                このエラーが発生した場合は可能な限り無視をせず、 nn::err::ShowError() 関数を使用してユーザーに通知してください。
            }
            @handleresult{
                nn::ssl::ResultSslService,
                （この結果は、 *this の取得に使用した関数のAPIリファレンスにインターネットとの通信の可能性が明記されている場合に限り返ることがあります。）
                処理中のネットワーク通信において問題が発生しました。
                このエラーが発生した場合は可能な限り無視をせず、 nn::err::ShowError() 関数を使用してユーザーに通知してください。
            }
        @endretresult

        @pre
            - *this が有効
            - 非同期処理が完了している

        @details
            非同期処理の結果を取得します。
            返却される結果は、それぞれの非同期処理ごとに定義されます。
            詳細は *this の取得に使用した関数の API リファレンスを参照してください。
    */
    Result GetResult() NN_NOEXCEPT;
};

}} // ~namespace nn::account
