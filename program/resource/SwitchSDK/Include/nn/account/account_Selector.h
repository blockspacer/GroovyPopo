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
    @brief ユーザー選択の UI を呼び出す処理の API を宣言します。
*/

#include <nn/account/account_Types.h>
#include <nn/nn_Result.h>

namespace nn { namespace account {

//! @name ユーザー選択 UI に関する型や値の定義
//! @{

/**
    @brief ユーザー選択 UI の表示方法を設定するための構造体です。

    @see DefaultUserSelectionSettings
*/
struct UserSelectionSettings
{
    /** @brief   選択不可にするユーザーの Uid リスト
        @details
            リストの先頭から順に参照し、InvalidUid に突き当たるかすべての Uid を見終わるまでのユーザーを、選択不可にします。
    */
    Uid  invalidUidList[UserCountMax];

    /** @brief   ユーザーが「その本体でのユーザー選択を省略する」というオプションをONにしている際に、UI 表示をスキップさせるか
        @details
            ユーザーが期待する動作になるよう、シーケンス上の不都合がない場所では「選択省略 OK 」を指定することを推奨します。
            isSkipEnabled == true のとき、以下のいずれかの条件を満たす場合に ShowUserSelector() 関数の呼び出しが失敗します。

            - invalidUidList に 1 つ以上の有効な Uid が設定されている
            - additionalSelect == true

            DefaultUserSelectionSettings での既定値は false です。
    */
    bool isSkipEnabled;

    /** @brief   ネットワークサービスアカウントを要求するか
        @details
            ネットワークサービスアカウントが必要な場面でのユーザー選択に使用します。
            true を指定した場合、選択されたユーザーに対する直後の IsNetworkServiceAccountAvailable() の呼び出しは true を返します。

            DefaultUserSelectionSettings での既定値は false です。
    */
    bool isNetworkServiceAccountRequired;

    /** @brief   UI にユーザー選択をスキップするボタンを表示するか
        @details
            ユーザーの選択が任意であり、選択せずに継続可能な場面でのユーザー選択に使用します。
            スキップボタンが選択された場合、選択されたユーザー ID として InvalidUid が格納され nn::ResultSuccess を返します。

            DefaultUserSelectionSettings での既定値は false です。
    */
    bool showSkipButton;

    /** @brief   UI を 2 人目以降のユーザーを選択するためのモードにするか
        @details
            - UI 表示が 2 人目以降を選択することを意図したものに変わります。
            - Open 済みのユーザーは選択不可になります。

            DefaultUserSelectionSettings での既定値は false です。
    */
    bool additionalSelect;

    /** @brief   制限がある（Unqualified）ユーザーを選択可能にします
        @details
            true を指定した場合、制限がある（Unqualified）ユーザーを選択可能にします。

            DefaultUserSelectionSettings での規定値は false です。
    */
    bool isUnqualifiedUserSelectable;
};

/** @brief ユーザー選択 UI を呼び出す際のデフォルト設定です。
    @details
        ユーザー選択 UI を呼び出す際のデフォルト設定です。
        選択可能なユーザーは ListQualifiedUsers() で列挙されるものと一致します。

        UserSelectionSettings を引数に取らない ShowUserSelector() はこの設定に基づき動作します。
*/
const UserSelectionSettings DefaultUserSelectionSettings = {{InvalidUid}, false, false, false, false, false};

//! @}

//! @name ユーザー選択 UI の呼び出しに関する機能
//! @{

/**
    @brief ユーザー選択を行う UI を呼び出します。

    @param[out] pOut 選択されたユーザー ID の格納先です。
    @param[in]  arg 表示に必要なパラメータを指定します。

    @retresult
        @handleresult{
            nn::ResultSuccess,
            ユーザーが選択されました。
        }
        @handleresult{
            nn::account::ResultCancelledByUser,
            ユーザーの選択がユーザーの意思に基づいて明示的にキャンセルされました。
            この結果をエラーとして扱わないでください。
        }
    @endretresult

    @pre
    - pOut != nullptr
    @post
    - *pOut が対象のユーザーの Uid を保持
    - arg.showSkipButton == true のときに表示されるスキップボタンが選択された場合、pOut に InvalidUid を格納して nn::ResultSuccess を返します。

    @details
    - 本関数は UI 上でユーザーの選択やキャンセルが行われるまで処理をブロックします。

    @see DefaultUserSelectionSettings
*/
Result ShowUserSelector( Uid* pOut, const UserSelectionSettings& arg ) NN_NOEXCEPT;

/**
    @brief ユーザー選択を行う UI を呼び出します。

    @param[out] pOut 選択されたユーザー ID の格納先です。

    @retresult
        @handleresult{
            nn::ResultSuccess,
            ユーザーが選択されました。
        }
        @handleresult{
            nn::account::ResultCancelledByUser,
            ユーザーの選択がユーザーの意思に基づいて明示的にキャンセルされました。
            この結果をエラーとして扱わないでください。
        }
    @endretresult

    @pre
    - pOut != nullptr
    @post
    - *pOut が対象のユーザーの Uid を保持

    @details
    - 本関数は UI 上でユーザーの選択やキャンセルが行われるまで処理をブロックします。
    - ShowUserSelector( Uid*, const UserSelectionSettings& ) に @ref DefaultUserSelectionSettings を指定した場合と同じ挙動となります。
*/
inline Result ShowUserSelector( Uid* pOut ) NN_NOEXCEPT
{
    return ShowUserSelector( pOut, DefaultUserSelectionSettings );
}

/**
    @brief 新規ユーザーの作成 UI を呼び出します。

    @retresult
        @handleresult{
            nn::ResultSuccess,
            ユーザーが作成されました。
        }
        @handleresult{
            nn::account::ResultCancelledByUser,
            ユーザーの作成がユーザーの意思に基づいて明示的にキャンセルされました。
            この結果をエラーとして扱わないでください。
        }
    @endretresult

    @details
    - 本関数は UI 上でユーザーの入力が完了するまで処理をブロックします。
*/
Result ShowUserCreator() NN_NOEXCEPT;

/**
    @brief ネットワークサービスの利用に関してユーザーに要求される資格の情報を表示します。

    @param[in] handle 資格の情報を表示する対象のユーザーを指す UserHandle

    @retresult
        @handleresult{
            nn::ResultSuccess,
            処理が成功しました。
            ユーザーの操作に応じてネットワークサービスアカウントの資格の情報が更新されている場合があります。
        }
        @handleresult{
            nn::account::ResultCancelledByUser,
            処理がユーザーの意思に基づいて明示的にキャンセルされました。
            この結果をエラーとして扱わないでください。
        }
    @endretresult

    @details
        本関数は、ユーザーのネットワークサービスアカウントに要求される資格の情報について説明する本体システムのUIを表示します。
        このUIをユーザーが操作した結果、ネットワークサービスアカウントが新たに資格を取得する場合があります。

        本関数は、プログラムが独自の裁量でユーザーのネットワークサービス利用資格を検査し、その結果資格の取得を必要とした場合に使用します。
        つまりプログラムは、次のそれぞれの条件に合致する場合に限り、本関数を使用してください。
        ユーザーに対して不必要に当該のUIが表示されることを避けるため、これらに合致しない場合に本関数を呼ばないでください。

        - ネットワークサービスアカウントのIDトークン取得時 (nn::account::EnsureNetworkServiceAccountIdTokenCacheAsync() 関数) に行われる資格の判定処理を明示的に無効化した
        - ネットワークサービスアカウントのIDトークンからプログラムの独自のルールに基づいてユーザーのネットワークサービスの利用資格を検査した結果、ユーザーに資格の取得が必要であると判断した
*/
Result ShowLicenseRequirementsForNetworkService( const UserHandle& handle ) NN_NOEXCEPT;

//! @}

}} // ~namespace nn::account
