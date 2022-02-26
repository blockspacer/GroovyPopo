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

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/friends/friends_Macros.h>
#include <nn/os.h>

namespace nn { namespace friends {

namespace detail
{
    class AsyncContextInternal;
}

/*!
    @brief      非同期処理を扱うクラスです。

    @details
                非同期処理の処理待ちやキャンセルを行うことが可能です。@n
                非同期処理が完了するまで、非同期関数に渡したオブジェクトを破棄してはいけません。
*/
class AsyncContext
{
private:
    NN_DISALLOW_COPY(AsyncContext);
    NN_DISALLOW_MOVE(AsyncContext);

public:
    /*!
        @brief      コンストラクタです。
    */
    AsyncContext() NN_NOEXCEPT;

    /*!
        @brief      デストラクタです。
    */
    ~AsyncContext() NN_NOEXCEPT;

    /*!
        @brief      非同期処理の完了がシグナルされるイベントを取得します。

        @param[out] outEvent    イベント。

        @return     処理結果。

        @pre
            - outEvent != nullptr
    */
    nn::Result GetSystemEvent(nn::os::SystemEvent* outEvent) NN_NOEXCEPT;

    /*!
        @brief      処理の中断を要求します。

        @return     処理結果。

        @details
                    本関数は処理の中断を要求するだけであり、本関数の呼び出し直後に非同期処理が完了するわけではありません。@n
                    非同期処理を中断して完了した場合、 @ref AsyncContext::GetResult は ResultCanceled を返します。
    */
    nn::Result Cancel() NN_NOEXCEPT;

    /*!
        @brief      非同期処理が完了しているかどうかを取得します。

        @param[out] outDone 非同期処理完了フラグ。

        @return     処理結果。

        @pre
            - outDone != nullptr
    */
    nn::Result HasDone(bool* outDone) NN_NOEXCEPT;

    /*!
        @brief      非同期処理の結果を取得します。

        @return     非同期処理の結果。
        @retval     ResultCanceled                                      非同期 API のキャンセル要求が行われました。
        @retval     ResultInternetRequestNotAccepted                    インターネット接続要求が受理されていない状態で非同期 API を実行しました。
        @retval     ResultHttpError                                     通信中にエラーが発生しました。
        @retval     ResultServerError                                   サーバーがエラーを返しました。
        @retval     nn::account::ResultNetworkCommunicationError        通信中にエラーが発生しました。
        @retval     nn::account::ResultNetworkServiceAccountUnavailable ネットワークサービスアカウントが利用可能な状態ではありませんでした。

        @details
                    非同期処理の結果を返します。@n
                    非同期関数の呼び出し結果ではないことに注意してください。

                    @ref AsyncContext::HasDone で取得したフラグが true になるまで、本関数を呼び出してはいけません。

                    非同期処理が以下のいずれかの場合、エラービューアでエラーメッセージを表示してください。

                    - ResultInternetRequestNotAccepted
                    - ResultHttpError
                    - ResultServerError

                    バックグラウンドでプロフィール情報を取得する等、アプリケーションの進行に影響がない通信処理を行っていた場合、エラーを表示する必要はありません。

                    非同期処理の結果が @ref nn::account 名前空間のものである場合、アカウントライブラリのマニュアルに従ってエラーハンドリングを行ってください。
    */
    nn::Result GetResult() const NN_NOEXCEPT;

public:
    // internal use only
    void SetInternal(detail::AsyncContextInternal* pInternal) NN_NOEXCEPT;

private:
    //
    mutable nn::os::Mutex m_Mutex;
    //
    detail::AsyncContextInternal* m_pInternal;
};

}}
