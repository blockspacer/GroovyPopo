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
    @brief このファイルは一般アプリケーションでの利用を想定した API を宣言します。
 */

#include <nn/account/account_AsyncContext.h>
#include <nn/account/account_NintendoAccountAuthorizationRequest.h>
#include <nn/account/account_Types.h>

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace account {

//! @name アカウントシステムのアプリケーション向けの機能
//! @{

/**
    @brief アカウントシステムの利用を開始します。

    @post
        アカウントシステムを利用可能です。

    @details
        アカウントシステムの初期化処理を行い、利用可能な状態にします。
        アカウントシステムを利用する前に、アプリケーションの起動につき少なくとも1度呼び出してください。
        本関数の2度目以降の呼出しでは初期化処理を行いません。
*/
void Initialize() NN_NOEXCEPT;

/**
    @brief 制限がない（Qualified）ユーザーの一覧を取得します。

    @param[out] pOutActualLength ユーザーの数の格納先
    @param[out] outUsers ユーザーの一覧の格納先
    @param[in]  arrayLength outUsers の要素数

    @retresult
        @handleresult{
            nn::ResultSuccess,
            処理に成功しました。
        }
    @endretresult

    @pre
        - ライブラリが初期化済み
        - pOutActualLength != nullptr
        - pOutUsers != nullptr
        - arrayLength > 0
    @post
        - *pOutActualLength >= 0 && *pOutActualLength <= UserCountMax
        - outUsers[0, min(arrayLength, *pOutActualLength)) が有効な Uid を保持
        - outUsers[min(arrayLength, *pOutActualLength), arrayLength) が無効な Uid を保持

    @details
        アカウントシステムに登録されているユーザーのうち、制限がない（Qualified）ユーザーの一覧を取得します。

        このリストには、 Closed 状態と Open 状態に関わらず、制限がないすべてのユーザーが列挙されます。
        リスト上のユーザーの列挙順序は、システムによって定められます。
*/
Result ListQualifiedUsers(int* pOutActualLength, Uid outUsers[], int arrayLength) NN_NOEXCEPT;

/**
    @brief ユーザーを Open 状態に変更し、そのハンドルを取得します。

    @param[out] pOutHandle 対象のユーザーを指すハンドル
    @param[in] user 対象のユーザーを指す Uid

    @retresult
        @handleresult{
            nn::ResultSuccess,
            ユーザーの Open 処理に成功しました。
        }
        @handleresult{
            nn::account::ResultUserNotExist,
            対象のユーザーアカウントが存在しません。
        }
    @endretresult

    @pre
        - static_cast<bool>(user) == true
        - user が指すユーザーが Closed 状態で存在
        - user が指すユーザーに制限がない（Qualified）
    @post
        - *pOutHandle が利用可能
        - user が指すユーザーが Open 状態で存在
        - 直後の GetLastOpenedUser() は user を返す

    @details
        対象のユーザーを Closed 状態から Open 状態に変更し、そのハンドルを取得します。

        本関数を呼び出すことで、対象のユーザーがアプリケーションの使用を開始したことが本体システムに通知されます。

    @see nn::account::ListQualifiedUsers()
*/
Result OpenUser(UserHandle* pOutHandle,  const Uid& user) NN_NOEXCEPT;

/**
    @brief アプリケーション起動前に選択されたユーザーアカウントがあれば Open 状態に変更し、そのハンドルを取得します。

    @param[out] pOutHandle 対象のユーザーを指すハンドル

    @return アプリケーション起動前にユーザーアカウントが選択された場合に true が返ります。

    @pre
        - アプリケーション起動時に選択されたユーザーが Closed 状態
    @post
        - true が返る場合、 *pOutHandle が利用可能
        - true が返る場合、アプリケーション起動時に選択されたユーザーが Open 状態

    @details
        本関数は「@confluencelink{166503043, nmeta ファイルの書き方}」に記載の StartupUserAccount を None 以外に設定して作成されたアプリケーションで使用できます。
        この設定がされたアプリケーションでは、アプリケーションの開始時に最大で1つのユーザーアカウントが選択されます。
        本関数はこのユーザーアカウントを Closed 状態から Open 状態に変更し、そのハンドルを取得します。

        「nmeta ファイルの書き方」に記載の StartupUserAccountOption に IsOptional を指定しない限り、この関数は必ず true を返します。
        StartupUserAccountOption に IsOptional を指定すると、本関数は false を返す場合があります。

        本関数が true を返す場合、対象のユーザーがアプリケーションの使用を開始したことが本体システムに通知されます。
*/
bool TryOpenPreselectedUser(UserHandle* pOutHandle) NN_NOEXCEPT;

/**
    @brief この関数は廃止予定です。 TryOpenPreselectedUser() を使用してください。
*/
NN_DEPRECATED Result OpenPreselectedUser(UserHandle* pOutHandle) NN_NOEXCEPT;

/**
    @brief 指定したハンドルの指すユーザーの状態を Closed 状態に変更します。

    @param[in] handle 対象のユーザーを指す UserHandle

    @pre
        - handle が利用可能
    @post
        - handle が利用不可
        - handle が指すユーザーが Closed 状態で存在

    @details
        対象のユーザーを Open 状態から Closed 状態に変更します。
        指定したハンドルは利用できなくなります。

        本関数を呼び出すことで、対象のユーザーがアプリケーションの使用を終了したことが本体システムに通知されます。
*/
void CloseUser(const UserHandle& handle) NN_NOEXCEPT;

/**
    @brief 指定したハンドルの指すユーザーアカウントの ID を取得します。

    @param[out] pOut 対象のユーザーを指す Uid
    @param[in]  handle 対象のユーザーを指す UserHandle

    @retresult
        @handleresult{
            nn::ResultSuccess,
            処理に成功しました。
        }
    @endretresult

    @pre
        - handle が利用可能
    @post
        - *pOut が対象のユーザーの Uid を保持

    @details
        指定したハンドルが指すユーザーアカウントの、 Uid の値を取得します。
*/
Result GetUserId(Uid* pOut, const UserHandle& handle) NN_NOEXCEPT;

/**
    @brief ネットワークサービスアカウントの可用性を確認し、必要に応じてネットワークサービスアカウントを有効化します。

    @param[in]  handle 対象のユーザーを指す UserHandle

    @retresult
        @handleresult{
            nn::ResultSuccess,
            ネットワークサービスアカウントを利用可能です。
        }
        @handleresult{
            nn::account::ResultCancelledByUser,
            処理がユーザーの意思に基づいて明示的にキャンセルされました。
            この結果をエラーとして扱わないでください。
        }
    @endretresult

    @post
        - IsNetworkServiceAccountAvailable() が true を返す

    @details
        ネットワークサービスアカウントの可用性を確認し、必要に応じてネットワークサービスアカウントを有効化します。
        この関数の呼び出し時点で指定したユーザーのネットワークサービスアカウントを利用可能な場合、この関数は即時に成功を返して終了します。
        そうでない場合には本体システムのUIを表示し、ネットワークサービスアカウントを有効化する処理を行います。

        本関数は通信せず、ネットワークサービスアカウントサーバーへの可用性の問い合わせを行いません。
        従って本関数が即時に成功を返した場合でも、 EnsureNetworkServiceAccountIdTokenCacheAsync() 関数等、通信を伴う機能の実行中に可用性が変化する場合があります。

        ネットワークサービスアカウントが利用可能となる条件は、SDKドキュメントの「ACCOUNTライブラリ」の項に記載されています。

        この関数は本体システムのUIを表示するために、呼び出し元のスレッドをブロックすることがあります。
*/
Result EnsureNetworkServiceAccountAvailable(const UserHandle& handle) NN_NOEXCEPT;

/**
    @brief Open 状態のユーザーのハンドルを指定して、ネットワークサービスアカウントの利用可否を検査します。

    @param[out] pOut ネットワークサービスアカウントの利用可否
    @param[in]  handle 対象のユーザーを指す UserHandle

    @retresult
        @handleresult{
            nn::ResultSuccess,
            処理に成功しました。
        }
    @endretresult

    @pre
        - pOut != nullptr
    @post
        - 対象のユーザーのネットワークサービスアカウントが利用可能な場合に限り *pOut = true

    @details
        ユーザーハンドルを指定して、ネットワークサービスアカウントの利用可否を検査します。
        ネットワークサービスアカウントを利用できないユーザーに対しては、ネットワークサービスを提供することができません。

        ネットワークサービスアカウントの利用時には通常、本関数ではなく EnsureNetworkServiceAccountAvailable() 関数の使用を推奨します。
        EnsureNetworkServiceAccountAvailable() 関数による通知メッセージの送信を避けたい場合に限り、本関数を使用してください。

        本関数は通信せず、ネットワークサービスアカウントサーバーへの可用性の問い合わせを行いません。
        従って本関数の呼び出しで *pOut == true となる場合でも、 EnsureNetworkServiceAccountIdTokenCacheAsync() 関数等、通信を伴う機能の実行中に可用性が変化する場合があります。

        ネットワークサービスアカウントが利用可能となる条件は、SDKドキュメントの「ACCOUNTライブラリ」の項に記載されています。
*/
Result IsNetworkServiceAccountAvailable(bool* pOut, const UserHandle& handle) NN_NOEXCEPT;

/**
    @brief Open 状態のユーザーのハンドルを指定して、そのユーザーのネットワークサービスアカウント ID を取得します。

    @param[out] pOutId ネットワークサービスアカウント ID
    @param[in]  handle 対象のユーザーを指す UserHandle

    @retresult
        @handleresult{
            nn::ResultSuccess,
            処理に成功しました。
        }
        @handleresult{
            nn::account::ResultNetworkServiceAccountUnavailable,
            ネットワークサービスアカウントが利用可能ではありません。
            EnsureNetworkServiceAccountAvailable() を実行することでネットワークサービスアカウントを利用可能な状態にし、このエラーを解消することができます。
        }
    @endretresult

    @pre
        - pOutId != nullptr
        - IsNetworkServiceAccountAvailable() が true を返す
    @post
        - *pOutId が、対象のユーザーのネットワークサービスアカウント ID を保持

    @details
        ユーザーハンドルを指定して、そのユーザーのネットワークサービスアカウント ID を取得します。
        ネットワークサービスアカウントはフレンドシステム (nn::friends) や NEX の機能を使用する際に必要となります。

        それぞれのユーザーは最大1つのネットワークサービスアカウントを保有しますが、このアカウントは外部要因で置き換えられたり削除されることがあり、そのIDは一定ではありません。
        このためアプリケーションは、ネットワークサービス利用時に都度 GetNetworkServiceAccountId() を使用して、最新の NetworkServiceAccountId を取得および使用してください。
        また、アプリケーションはユーザーの NetworkServiceAccountId が一定であることを期待した設計をもたないでください。
*/
Result GetNetworkServiceAccountId(NetworkServiceAccountId* pOutId, const UserHandle& handle) NN_NOEXCEPT;

/**
    @brief ユーザーハンドルを指定して、ネットワークサービスアカウントのIDトークンのキャッシュを確保します。

    @param[out] pOutContext 非同期処理の完了と結果の通知を受けるための nn::account::AsyncContext オブジェクト
    @param[in]  handle 対象のユーザーを指す UserHandle

    @retresult
        @handleresult{
            nn::ResultSuccess,
            ネットワークサービスアカウントのIDトークンキャッシュの確保処理の開始に成功しました。
            取得した AsyncContext の GetResult() 関数が成功を返すと、IDトークンキャッシュを利用できます。
        }
        @handleresult{
            nn::account::ResultNetworkServiceAccountUnavailable,
            ネットワークサービスアカウントが利用可能ではありません。
            EnsureNetworkServiceAccountAvailable() を実行することでネットワークサービスアカウントを利用可能な状態にし、このエラーを解消することができます。
        }
    @endretresult

    @pre
        - pOutContext != nullptr
    @post
        - *pOutContext がシグナルされた直後の、同一ユーザーに関する LoadNetworkServiceAccountIdTokenCache() が成功

    @details
        ユーザーハンドルを指定して、ネットワークサービスアカウントのIDトークンのキャッシュを確保します。
        失効までに十分な猶予のあるIDトークンのキャッシュが既に存在する場合、本関数で返る nn::account::AsyncContext オブジェクトには即時に完了が通知されます。
        そうでない場合、本関数はネットワークサービスアカウントの認証処理を経てIDトークンを取得し、そのキャッシュを作成します。
        キャッシュの作成が完了すると nn::account::AsyncContext オブジェクトがもつシステムイベントがシグナルされます。

        本関数で確保されたIDトークンのキャッシュは、 LoadNetworkServiceAccountIdTokenCache() で取得できます。

        この関数はインターネットとの通信を行うことがあります。

        この関数に伴う非同期処理中にネットワークサービスアカウントが利用不可能と判断され、 AsyncContext オブジェクトの GetResult() 関数が ResultNetworkServiceAccountUnavailable を返却する場合があります。
        その場合、 EnsureNetworkServiceAccountAvailable() を実行することでネットワークサービスアカウントを利用可能な状態にし、このエラーを解消することができます。
*/
Result EnsureNetworkServiceAccountIdTokenCacheAsync(AsyncContext* pOutContext, const UserHandle& handle) NN_NOEXCEPT;

/**
    @brief ユーザーハンドルを指定して、ネットワークサービスアカウントのIDトークンのキャッシュを取得します。

    @param[out] pOutActualSize IDトークンの実際の長さ (終端文字含まず)
    @param[out] buffer IDトークンの格納先
    @param[in]  bufferSize buffer に書き込み可能な大きさ (バイト数)
    @param[in]  handle 対象のユーザーを指す UserHandle

    @retresult
        @handleresult{
            nn::ResultSuccess,
            IDトークンキャッシュの取得に成功しました。
        }
        @handleresult{
            nn::account::ResultNetworkServiceAccountUnavailable,
            ネットワークサービスアカウントが利用可能ではありません。
            EnsureNetworkServiceAccountAvailable() を実行することでネットワークサービスアカウントを利用可能な状態にし、このエラーを解消することができます。
        }
        @handleresult{
            nn::account::ResultTokenCacheUnavailable,
            IDトークンキャッシュを利用できません。
            EnsureNetworkServiceAccountIdTokenCacheAsync() を実行してください。
        }
    @endretresult

    @pre
        - pOutActualSize != nullptr
        - buffer != nullptr
        - bufferSize >= nn::account::NetworkServiceAccountIdTokenLengthMax
        - IsNetworkServiceAccountAvailable() が true を返す
    @post
        - *pOutActualSize がIDトークンの終端文字を含めない実際の長さを保持
        - buffer[0, *pOutActualSize) の範囲のASCII列を有効なIDトークンとして利用可能
        - *pOutActualSize < bufferSize のときに限りIDトークンは '\0' で終端される

    @details
        EnsureNetworkServiceAccountIdTokenCacheAsync() で確保したIDトークンのキャッシュを取得します。
        IDトークンはURLエンコードされた文字列として取得され、その最大長は終端文字を含めず nn::account::NetworkServiceAccountIdTokenLengthMax です。

        本関数で取得されるIDトークンには有効期限があり、取得後の時間経過で失効します。
        失効までの猶予は EnsureNetworkServiceAccountIdTokenCacheAsync() 呼出しから30分かそれ以上であり、この期間内に使用されることを期待しています。
        原則的にIDトークンを使用する都度、 EnsureNetworkServiceAccountIdTokenCacheAsync() 並びに本関数を利用してトークンを再取得してください。
        失効したIDトークンをサーバーに送信すると、サーバーでのIDトークンの検証に失敗する場合があります。

        本関数で取得可能なIDトークンは呼び出し元のアプリケーション固有の情報を含んでいるため、他のアプリケーションと共有しないでください。
        またユーザーの所有するネットワークサービスアカウントはアプリケーションの動作しない期間に変更や削除されることがあるため、セーブデータなどにIDトークンを保存して利用しないでください。
*/
Result LoadNetworkServiceAccountIdTokenCache(size_t* pOutActualSize, char* buffer, size_t bufferSize, const UserHandle& handle) NN_NOEXCEPT;

/**
    @brief ネットワークサービスアカウントと連携するニンテンドーアカウントから認可を取得するためのリクエストを作成します。

    @param[out] pOutRequest 作成されるリクエストを配置するアドレス
    @param[in] handle 対象のユーザーを指す UserHandle
    @param[in] param ニンテンドーアカウントサーバーとの認証,認可手続きで使用されるパラメータ
    @param[out] workBuffer リクエストの保持に必要なワークメモリ。
    @param[in] workBufferSize workBufferの大きさ(バイト数)

    @retresult
        @handleresult{
            nn::ResultSuccess,
            成功しました。
        }
        @handleresult{
            nn::account::ResultNetworkServiceAccountUnavailable,
            ネットワークサービスアカウントを利用可能ではありません。
            EnsureNetworkServiceAccountAvailable() を実行することでネットワークサービスアカウントを利用可能な状態にし、このエラーを解消することができます。
        }
    @endretresult

    @pre
        - pOutRequest != nullptr
        - workBuffer != nullptr
        - reinterpret_cast<uintptr_t>(workBuffer) % nn::os::MemoryPageSize == 0
        - workBufferSize >= nn::account::RequiredBufferSizeForNintendoAccountAuthorizationRequestContext
        - workBufferSize % nn::os::MemoryPageSize == 0
    @post
        - *pOutRequest が有効なリクエストである
        - 作成されたリクエストが有効な間、 workBuffer から workBufferSize バイトの範囲にアクセスできない

    @details
        アプリケーションに対して、指定したユーザーのネットワークサービスアカウントと連携するニンテンドーアカウントから認可を取得するためのリクエストを作成します。
        リクエストを作成すると非同期で認可の取得処理が行われ、その結果は引数に指定する NintendoAccountAuthorizationRequestContext オブジェクトから取得することができます。

        この関数はインターネットとの通信を行います。

        ワークメモリとして指定するメモリ領域は、 NintendoAccountAuthorizationRequestContext オブジェクトが有効なリクエストを保持している期間はアクセスできません。
*/
Result CreateNintendoAccountAuthorizationRequest(
    NintendoAccountAuthorizationRequestContext* pOutRequest,
    const UserHandle& handle,
    const NintendoAccountAuthorizationRequestParameters& param, void* workBuffer, size_t workBufferSize) NN_NOEXCEPT;

/**
    @brief ユーザー選択 UI で表示するセーブデータサムネイル画像を書き込みます。

    @param[in] uid 対象のユーザーの Uid
    @param[in] imageBuffer 保存する画像へのポインタ
    @param[in] imageBufferSize imageBuffer のバイトサイズ

    @pre imageBuffer と imageBufferSize が nn::os::MemoryPageSize でアラインされている
    @pre [imageBuffer, imageBuffer + imageBufferSize) の領域が読み書き可能

    @retresult
        @handleresult{
            nn::ResultSuccess,
            処理に成功しました。
        }
        @handleresult{
            nn::fs::ResultTargetNotFound,
            処理対象のセーブデータが作成されていません。
        }
    @endretresult

    @details
    本関数は保存処理が完了するまでブロックします。
    画像バッファの要求仕様は次の通りです。
    - 画像フォーマット：RGBA8
    - タイリング：リニア
    - 画像ガンマ：sRGB
    - 幅(pixel)：256
    - 高さ(pixel)：144
    よって、画像バッファサイズは 256 x 144 x 4 = 147,456 byte が与えられることを期待しています。
*/
Result StoreSaveDataThumbnailImage(const Uid& uid, const void* imageBuffer, size_t imageBufferSize) NN_NOEXCEPT;

/**
    @brief ユーザー選択 UI で表示するセーブデータサムネイル画像を削除します

    @param[in] uid 対象のユーザーの Uid

    @retresult
        @handleresult{
            nn::ResultSuccess,
            処理に成功しました。
        }
        @handleresult{
            nn::fs::ResultTargetNotFound,
            処理対象のセーブデータが作成されていません。
        }
    @endretresult

    @details
    本関数は削除処理が完了するまでブロックします。
    セーブデータのリセットや削除をする際に使用することで、ユーザー選択 UI で表示するセーブデータサムネイル画像を初期状態に戻すことができます。
*/
Result DeleteSaveDataThumbnailImage(const Uid& uid) NN_NOEXCEPT;

//! @}

}} // ~namespace nn::account
