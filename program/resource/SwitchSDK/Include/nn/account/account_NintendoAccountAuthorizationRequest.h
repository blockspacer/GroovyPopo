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

/** @file
    @brief このファイルはニンテンドーアカウントと機能的に連携するアプリケーションが、その連携のために呼ぶAPIを宣言します。
 */

#include <nn/nn_Result.h>
#include <nn/account/account_AsyncContext.h>
#include <nn/account/account_Types.h>
#include <nn/os/os_SystemEvent.h>

namespace nn {
namespace account {

namespace nas {
class IAuthorizationRequest;
} // ~namespace nn::account::nas

} // ~namespace nn::account
}

namespace nn {
namespace account {

/**
    @brief ニンテンドーアカウントから認可を取得するためのリクエストを表すクラス
    @details
        このクラスは「ニンテンドーアカウントから認可を取得するためのリクエスト」について、次を行います。
        - リクエスト処理の完了の通知を受けるイベントの取得
        - リクエスト処理の完了, 未完了の状態の取得
        - リクエスト処理が完了している場合、その結果と、リクエストに対して得られた認可コードやIDトークンの取得
        - リクエスト処理が未完了の場合、その中止の要求

        NintendoAccountAuthorizationRequestContext オブジェクトは次の状態をもちます。
        - 無効
            - オブジェクトはいかなるリクエストとも関連づきません。
            - この状態では、デストラクタ以外を呼び出すことはできません。
        - 有効
            - このオブジェクトは特定のリクエストと関連づきます。
            - この状態では関連づくリクエストに関して先述の操作が可能です。
 */
class NintendoAccountAuthorizationRequestContext
{
    friend class NetworkServiceAccountManager;
    friend Result CreateNintendoAccountAuthorizationRequest(
            NintendoAccountAuthorizationRequestContext* pOutRequest,
            const UserHandle& handle,
            const NintendoAccountAuthorizationRequestParameters& param, void* workBuffer, size_t workBufferSize) NN_NOEXCEPT;

private:
    Uid m_User;
    nas::IAuthorizationRequest* m_Ptr;
    AsyncContext m_AsyncContext;

    NintendoAccountAuthorizationRequestContext(nas::IAuthorizationRequest* ptr, const Uid& user) NN_NOEXCEPT;
    Result Invoke() NN_NOEXCEPT;

    NintendoAccountAuthorizationRequestContext& Swap(NintendoAccountAuthorizationRequestContext& rhs) NN_NOEXCEPT;

public:
    /**
        @brief 無効なインスタンスを作成するデフォルトコンストラクタ

        @post
            - *this は無効

        @details
            無効なインスタンスを作成します。
            この状態では、デストラクタ以外のいかなる関数も呼出すことができません。
    */
    NintendoAccountAuthorizationRequestContext() NN_NOEXCEPT;

    /**
        @brief デストラクタ

        @details
            インスタンスを破棄します。
            このインスタンスから事前に取得したイベントは無効になります。

            もしインスタンスが有効でリクエストが処理中の場合、デストラクタの呼出しは失敗します。
     */
    ~NintendoAccountAuthorizationRequestContext() NN_NOEXCEPT;

    /**
        @brief 処理の完了がシグナルされるイベントを取得します。

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
            - 処理完了時点で *pOutEvent がシグナルされる
            - *pOutEvent のクリアモードが nn::os::ClearMode_ManualClear

        @details
            処理の完了通知がシグナルされるイベントを取得します。
            このイベントは *this が有効な期間使用することができます。
            従って、 *this を破棄したあとにこのイベントを参照してはいけません。
    */
    Result GetSystemEvent(os::SystemEvent* pOutEvent) NN_NOEXCEPT;

    /**
        @brief 処理の中止を要求します。

        @retresult
            @handleresult{
                nn::ResultSuccess,
                中止の要求に成功しました。
            }
        @endretresult

        @pre
            - *this が有効

        @details
            処理の中止を要求します。
            この API は中止の完了を待たずに返ります。

            中止を要求したあと非同期処理は可能な限り早い時点で完了し、その完了通知がシグナルされます。
            非同期処理が既に完了している場合、本関数は何も行いません。
    */
    Result Cancel() NN_NOEXCEPT;

    /**
        @brief 処理が完了しているかどうかを取得します。

        @param[out] pOut 取得した状態の格納先

        @retresult
            @handleresult{
                nn::ResultSuccess,
                取得に成功しました。
            }
        @endretresult

        @pre
            - *this が有効
        @post
            - タスクが完了状態の場合に限り *pOut == true

        @details
            処理が完了しているかどうかを取得します。
            処理の完了には、処理の中止の完了も含まれます。
    */
    Result HasDone(bool* pOut) NN_NOEXCEPT;

    /**
        @brief 認可取得手続きの結果を取得します。

        @retresult
            @handleresult{
                nn::ResultSuccess,
                認可取得手続きに成功しました。
            }
            @handleresult{
                nn::account::ResultCancelledByUser,
                認可取得手続きがユーザーの意思に基づいて明示的にキャンセルされました。
                この結果をエラーとして扱わないでください。
            }
            @handleresult{
                nn::account::ResultCancelled,
                Cancel() 関数の呼び出しによって認可取得手続きがキャンセルされました。
                この結果をエラーとして扱わないでください。
            }
            @handleresult{
                nn::account::ResultNetworkCommunicationError,
                処理中のネットワーク通信において問題が発生しました。
                このエラーが発生した場合は可能な限り無視をせず、 nn::err::ShowError() 関数を使用してユーザーに通知してください。
            }
            @handleresult{
                nn::ssl::ResultSslService,
                処理中のネットワーク通信において問題が発生しました。
                このエラーが発生した場合は可能な限り無視をせず、 nn::err::ShowError() 関数を使用してユーザーに通知してください。
            }
        @endretresult

        @pre
            - *this が有効
            - 認可取得手続きが完了している

        @details
            認可取得手続きの結果を取得します。
            認可の取得にユーザーとの対話が必要な場合には本体システムのUIを表示し、ユーザーとの対話による認可取得を試みます。

            この関数は本体システムのUIを表示するために、呼び出し元のスレッドをブロックすることがあります。
    */
    Result GetResultWithInteractionIfRequired() NN_NOEXCEPT;

    /**
        @brief 認可取得手続きの結果を取得します。

        @retresult
            @handleresult{
                nn::ResultSuccess,
                認可取得手続きに成功しました。
            }
            @handleresult{
                nn::account::ResultCancelled,
                Cancel() 関数の呼び出しによって認可取得手続きがキャンセルされました。
                この結果をエラーとして扱わないでください。
            }
            @handleresult{
                nn::account::ResultNintendoAccountAuthorizationInteractionRequired,
                認可取得にユーザーとの対話が要求されています。
                このエラーが発生した場合は GetResultWithInteractionIfRequired() 関数を使用して、ユーザーとの対話のために、本体システムのUIを表示してください。
            }
            @handleresult{
                nn::account::ResultNetworkCommunicationError,
                処理中のネットワーク通信において問題が発生しました。
                このエラーが発生した場合は可能な限り無視をせず、 nn::err::ShowError() 関数を使用してユーザーに通知してください。
            }
            @handleresult{
                nn::ssl::ResultSslService,
                処理中のネットワーク通信において問題が発生しました。
                このエラーが発生した場合は可能な限り無視をせず、 nn::err::ShowError() 関数を使用してユーザーに通知してください。
            }
        @endretresult

        @pre
            - *this が有効
            - 認可取得手続きが完了している

        @details
            認可取得手続きの結果を取得します。
            認可の取得にユーザーとの対話が必要な場合には nn::account::ResultNintendoAccountAuthorizationInteractionRequired が返ります。

            この関数は本体システムによるUIの表示を一時的に回避する目的にのみ使用し、通常は GetResultWithInteractionIfRequired() 関数を使用して下さい。
    */
    Result GetResult() NN_NOEXCEPT;

    /**
        @brief 認可コードを取得します。

        @param[out] pOutActualSize 認可コードの実際の長さ (終端文字含まず)
        @param[out] buffer 認可コードの格納先
        @param[in] bufferSize buffer に書き込み可能な大きさ (バイト数)

        @retresult
            @handleresult{
                nn::ResultSuccess,
                処理に成功しました。
            }
        @endretresult

        @pre
            - *this が有効
            - pOutActualSize != nullptr
            - buffer != nullptr
            - bufferSize >= nn::account::NintendoAccountAuthorizationCodeLengthMax
            - GetResultWithInteractionIfRequired().IsSuccess() == true
        @post
            - *pOutActualSize が認可コードの終端文字を含めない実際の長さを保持
            - buffer[0, *pOutActualSize) の範囲のASCII列を有効な認可コードとして利用可能
            - *pOutActualSize < bufferSize のときに限り buffer[*pOutActualSize] == '\0'

        @details
            認可リクエストによって得られた認可コードを取得します。
            認可コードには利用回数の制限があり、一度のリクエストで得られた認可コードは最大1回まで利用できます。
    */
    Result GetAuthorizationCode(size_t* pOutActualSize, char* buffer, size_t bufferSize) NN_NOEXCEPT;

    /**
        @brief IDトークンを取得します。

        @param[out] pOutActualSize IDトークンの実際の長さ (終端文字含まず)
        @param[out] buffer IDトークンの格納先
        @param[in] bufferSize buffer に書き込み可能な大きさ (バイト数)

        @retresult
            @handleresult{
                nn::ResultSuccess,
                処理に成功しました。
            }
        @endretresult

        @pre
            - *this が有効
            - pOutActualSize != nullptr
            - buffer != nullptr
            - bufferSize >= nn::account::NintendoAccountIdTokenLengthMax
            - GetResultWithInteractionIfRequired().IsSuccess() == true
        @post
            - *pOutActualSize がIDトークンの終端文字を含めない実際の長さを保持
            - buffer[0, *pOutActualSize) の範囲のASCII列を有効なIDトークンとして利用可能
            - *pOutActualSize < bufferSize のときに限り buffer[*pOutActualSize] == '\0'

        @details
            認可リクエストによって得られたIDトークンを取得します。
            IDトークンには有効期限があり、その期間が十分短いことに注意してください。
    */
    Result GetIdToken(size_t* pOutActualSize, char* buffer, size_t bufferSize) NN_NOEXCEPT;

    /**
        @brief OAuth 2.0の手続きに使用したstate値を取得します。

        @param[out] pOutActualSize state値の実際の長さ (終端文字含まず)
        @param[out] buffer state値の格納先
        @param[in] bufferSize buffer に書き込み可能な大きさ (バイト数)

        @retresult
            @handleresult{
                nn::ResultSuccess,
                処理に成功しました。
            }
        @endretresult

        @pre
            - *this が有効
            - pOutActualSize != nullptr
            - buffer != nullptr
            - bufferSize >= 128
            - GetResultWithInteractionIfRequired().IsSuccess() == true
        @post
            - buffer[0,*pOutActualSize) がリクエスト作成時にパラメータとして指定したstate値と一致する。
            - *pOutActualSize < bufferSize のときに限り buffer[*pOutActualSize] == '\0'

        @details
            リクエストパラメータに指定され、認可リクエストに使用されたstate値を取得します。
    */
    Result GetState(size_t* pOutActualSize, char* buffer, size_t bufferSize) NN_NOEXCEPT;

    NintendoAccountAuthorizationRequestContext(NintendoAccountAuthorizationRequestContext&& rhs) NN_NOEXCEPT;
    NintendoAccountAuthorizationRequestContext& operator=(NintendoAccountAuthorizationRequestContext&& rhs) NN_NOEXCEPT;
    Result GetRawResult() NN_NOEXCEPT; // SDK 内部実装用
    void DebugGetInfo(uint64_t* pOut, size_t count) NN_NOEXCEPT; // SDK デバッグ用
};

} // ~namespace nn::account
}


