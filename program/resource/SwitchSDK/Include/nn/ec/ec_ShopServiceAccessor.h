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
 * @brief ショップサービスアクセスユーティリティ機能関連宣言。
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/account/account_Types.h>
#include <nn/ec/ec_Result.h>
#include <nn/ec/ec_ResultShopServiceAccessor.h>
#include <nn/ec/ec_ShopServiceAccessorTypes.h>
#include <nn/err/err_Types.h>
#include <nn/os/os_Mutex.h>
#include <nn/os/os_SystemEvent.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn {
    namespace sf {
        class NativeHandle;
    }
    namespace nim { namespace detail {
        class IShopServiceAccessor;
        class IShopServiceAsync;
    }}
}

namespace nn { namespace ec {

/**
 *
 * @name    ショップサービスアクセスユーティリティ機能向け環境関数。
 *
 * ShopServiceAccessor クラスを利用する上で必要になる共有リソースなどの設定を行うモジュールAPI群です。@n
 * ShopServiceAccessor クラスは本 API の @ref InitializeForShopServiceAccessors() ～ @ref FinalizeForShopServiceAccessors() 間でのみ動作が保証されます。@n
 *
 * @par エイリアスリプレース機能について。
 * ShopServiceAccessor 経由での要求時に指定する要求文字列の一部には、通信要求実施時にエイリアスシンボルを該当する値に置換( リプレース )する機能が提供されます。@n
 * 本機能が利用可能な要求要素は以下です。@n
 *  @li URLパスフィールド文字列
 *
 * @par エイリアスシンボルについて。
 * エイリアスシンボルは以下のような既定書式及びキーワードで表現された文字列です。@n
 *  @li '{' から '}' で囲まれた文字列。
 *
 * @par 利用可能なエイリアスシンボル
 *  @li applicationId       起動実行中のアプリケーションIDに置換します。
 *  @li country             指定ユーザアカウントと連携しているニンテンドーアカウントに登録された居住国コードに置換します。
 *
 * @par 利用例
 * @code
 *  const char* path = "/v1/applications/{applicationId}/consumables?country={country}&lang=ja";
 * @endcode
 *
 *  @see ShopServiceAccessor
 *  @see ShopServiceAccessor::AsyncResponse
 *
 * @par 関連関数
 */

//----------------------------------------------------------------------------
//!@{

/**
 * @brief       ショップサービスアクセスユーティリティ機能の初期化要求。
 *
 * @param[in]   pWorkMemoryTop  ワークバッファ対象領域先頭アドレス。( @ref nn::os::MemoryPageSize アライン必須 )
 * @param[in]   byteSizeOfTotal ワークバッファ対象領域バイトサイズ。( @ref nn::os::MemoryPageSize アライン必須 )
 *
 * @return      処理結果を返します。
 * @retval      nn::ResultSuccess       処理が正常に行われました。
 *
 * @pre
 *              - @ref InitializeForShopServiceAccessors() による環境初期化が行われていない。
 *              - 開始アドレスがメモリページサイズ( @ref nn::os::MemoryPageSize )にアラインされている。
 *              - 領域バイトサイズがメモリページサイズ( @ref nn::os::MemoryPageSize )にアラインされている。
 *              - 指定するワークバッファアドレスにスタック変数領域を指定していない。@n
 *                ワーク領域の維持想定期間が局所スコープを越えるためです。@n
 *                静的領域もしくは、ヒープ領域から確保してください。
 *
 * @details     指定されたバッファを通信ワーク領域として以下の用途に用いられ占有されます。@n
 *              指定するワークバッファは、下記用途のデータ群を保持するのに十分な余裕を持つ領域を指定してください。@n
 *              @li ショップサービス通信要求入力パラメータの非同期保持。@n
 *                  要求は非同期に処理されるため実際の通信実行までサービスがコピーを保持する必要があります。
 *              @li ショップサービス通信要求出力レスポンスの非同期保持。@n
 *                  要求は非同期に処理されるため実際の通信完了に伴ったレスポンスの生存期間を保証するため、非同期ハンドルが破棄されるまでコピーを保持する必要があります。
 *
 */
Result InitializeForShopServiceAccessors(void* pWorkMemoryTop, size_t byteSizeOfTotal) NN_NOEXCEPT;

/**
 * @brief       ショップサービスアクセスユーティリティ機能の終了要求。
 *
 * @return      処理結果を返します。
 * @retval      nn::ResultSuccess       処理が正常に行われました。
 *
 * @pre
 *              - @ref InitializeForShopServiceAccessors() による環境初期化済である。
 *
 * @details     @ref InitializeForShopServiceAccessors() で指定されたワークバッファの占有が解除されます。
 */
Result FinalizeForShopServiceAccessors() NN_NOEXCEPT;

//!@}
//----------------------------------------------------------------------------

/**
 * @brief   ショップサービスアクセッサクラス。
 *
 * @details 接続先ショップサービスサーバーを明示的に指定してアクセスを行うクラスです。@n
 *          以下の処理手順によりショップサービスサーバーへのアクセスを実施します。@n
 *          @li @ref Initialize(const ShopService&) により接続先ショップサービスサーバーを指定します。
 *          @li @ref Request() で対象ショップサービスサーバーに対して通信要求を行います。
 *          @n
 *          @ref Request() は非同期に要求が受理されます。@n
 *          また、非同期要求はキュー登録され順次実施されます。@n
 *          受理された非同期要求の完了確認及び中断制御用のハンドルとして @ref AsyncResponse クラスを利用します。@n
 *          @n
 *          非同期要求の発行上限制約は内部共有リソース( サービスリソース )に依存して変動します。@n
 *          発行上限制約により要求が失敗した場合は、nn::ec::ResultShopServiceAccessOverRequest が返されます。@n
 *          @n
 *          サービスリソース運用目安は以下の通りです。@n
 *          @li ShopServiceAccessor クラスインスタンスが 1 の状態での利用する場合、16 要求登録可能。
 *          @li ShopServiceAccessor クラスインスタンスが 2 以上の状態で利用する場合、サービスリソース状態に応じて 16 未満の要求登録が可能。
 *
 * @par     参考
 *          本クラスを用いたショップサービスサーバー API への処理要求手順例( "/v1/applications/{applicationId}/consumables" への HTTP GET メソッド要求 )は以下の通りです。
 *
 * @code
 *  NN_ALIGNAS(nn::os::MemoryPageSize) Bit8 g_WorkMemory[nn::os::MemoryPageSize * 16];
 *  void main()
 *  {
 *      const account::Uid user = GetUser();
 *      nn::ec::InitializeForShopServiceAccessors(g_WorkMemory, sizeof(g_WorkMemory));
 *      {
 *          nn::ec::ShopServiceAccessor accessor;
 *          accessor.Initialize(nn::ec::ShopService(nn::ec::ShopService::Type_Catalog);
 *
 *          nn::ec::ShopServiceAccessor::AsyncResponse response;
 *          accessor.Request(&response, user, nn::ec::ShopService::Method_Get, "/v1/applications/{applicationId}/consumables?country={country}&lang=ja");
 *
 *          size_t responseSize;
 *          response.GetSize(&responseSize);
 *
 *          std::unique_ptr<char[]> responseBuffer(new char[responseSize]);
 *          response.Get(responseBuffer.get(), responseSize);
 *      }
 *      nn::ec::FinalizeForShopServiceAccessors();
 *  }
 * @endcode
 *
 */
class ShopServiceAccessor : public ::nn::util::IntrusiveListBaseNode<ShopServiceAccessor>
{
    NN_DISALLOW_COPY(ShopServiceAccessor);
    NN_DISALLOW_MOVE(ShopServiceAccessor);

public:
    /**
     * @brief   非同期結果受信ハンドルクラス。
     *
     * @details 本クラスインタフェースに対する操作はスレッドセーフではありません。@n
     *          @ref ShopServiceAccessor::Request() 成功による非同期ハンドル獲得後、デストラクション呼び出しと他同期処理をマルチスレッド利用する場合は適切に排他処理を行ってください。
     */
    class AsyncResponse : public ::nn::util::IntrusiveListBaseNode<AsyncResponse>
    {
        NN_DISALLOW_COPY(AsyncResponse);
        NN_DISALLOW_MOVE(AsyncResponse);

        friend class ShopServiceAccessor;

    public:
        /**
         * @brief       デフォルトコンストラクタ。
         */
        NN_IMPLICIT AsyncResponse() NN_NOEXCEPT;

        /**
         * @brief       デストラクタ。
         */
        ~AsyncResponse() NN_NOEXCEPT;

        /**
         * @brief       通信結果レスポンスデータの受領に必要な容量( バイトサイズ )を取得します。
         *
         * @param[out]  pOutValue   レスポンスサイズ。@n
         *                          取得レスポンスサイズは文字列型レスポンスであっても null 終端文字は含まれない点ご注意ください。
         *
         * @return      処理結果を返します。@n
         *              以下列挙 Result 値以外にアクセスするサービス毎の固有Result値が返される事があります。@n
         *              ハンドリングが必要なサービス固有Result値については各サービスのリファレンスを参照してください。@n
         *              アプリケーションでハンドリング出来ないResult値はエラービューアに通知してください。
         *
         * @retval      nn::ResultSuccess                                       処理が正常に行われました。
         * @retval      nn::ec::ResultShopServiceAccessCanceled                 呼び出し元からの要求に従い処理がキャンセルされました。
         * @retval      nn::ec::ResultShopServiceAccessInsufficientWorkMemory   非同期要求用ワークメモリ( ワークバッファ )が不足しています。
         * @retval      nn::ec::ResultShopServiceAccessRequestTimeout           サーバーとの通信要求がタイムアウトしました。
         * @retval      nn::account::ResultNetworkServiceAccountUnavailable     指定されたユーザアカウントのネットワークサービスアカウントが利用できません。@n
         *                                                                      ACCOUNT ライブラリのエラーハンドリングリファレンスを参考にネットワークサービスアカウントを利用可能状態にしてください。
         * @retval      nn::ec::ResultShowErrorCodeRequired                     接続先ショップサービスサーバーからエラービューア表示が必要なレスポンスエラーコードを受信しました。
         *                                                                      @ref GetErrorCode() で取得したエラーコードを nn::err::ShowError(::nn::err::ErrorCode) に通知してください。
         *
         * @pre
         *              - @ref ShopServiceAccessor::Request() で非同期要求に成功している。
         *              - nullptr != pOutValue
         *
         * @details     要求が未完了状態( 非同期処理中 )に本メソッドを呼び出した場合は、受領可能な状態に移行するまで呼び出しスレッドをストールさせます。
         */
        Result GetSize(size_t* const pOutValue) NN_NOEXCEPT;

        /**
         * @brief       通信結果レスポンスデータの受領。
         *
         * @param[out]  pOutReceivedBuffer  受領先メモリ領域のアドレス先頭へのポインタ。@n
         *                                  受領レスポンスデータには文字列型レスポンスであっても null 終端文字は自動付与されない点ご注意ください。
         * @param[in]   bufferCapacity      受領先メモリ領域の受信可能容量( バイト数 )。
         *
         * @return      処理結果を返します。@n
         *              以下列挙 Result 値以外にアクセスするサービス毎の固有Result値が返される事があります。@n
         *              ハンドリングが必要なサービス固有Result値については各サービスのリファレンスを参照してください。@n
         *              アプリケーションでハンドリング出来ないResult値はエラービューアに通知してください。
         *
         * @retval      nn::ResultSuccess                                       処理が正常に行われました。
         * @retval      nn::ec::ResultShopServiceAccessCanceled                 呼び出し元からの要求に従い処理がキャンセルされました。
         * @retval      nn::ec::ResultShopServiceAccessInsufficientWorkMemory   非同期要求用ワークメモリ( ワークバッファ )が不足しています。
         * @retval      nn::ec::ResultShopServiceAccessInsufficientBuffer       受領に必要なバッファ容量が不足しています。@n
         *                                                                      @a pOutReceivedBuffer の領域を増やしてください。
         * @retval      nn::ec::ResultShopServiceAccessRequestTimeout           サーバーとの通信要求がタイムアウトしました。
         * @retval      nn::account::ResultNetworkServiceAccountUnavailable     指定されたユーザアカウントのネットワークサービスアカウントが利用できません。@n
         *                                                                      ACCOUNT ライブラリのエラーハンドリングリファレンスを参考にネットワークサービスアカウントを利用可能状態にしてください。
         * @retval      nn::ec::ResultShowErrorCodeRequired                     接続先ショップサービスサーバーからエラービューア表示が必要なレスポンスエラーコードを受信しました。@n
         *                                                                      @ref GetErrorCode() で取得したエラーコードを nn::err::ShowError() に通知してください。
         *
         * @pre
         *              - @ref ShopServiceAccessor::Request() で非同期要求に成功している。
         *              - nullptr != pOutReceivedBuffer
         *
         * @details     レスポンス受領に必要な容量は @ref GetSize() で取得できます。@n
         *              要求が未完了状態( 非同期処理中 )に本メソッドを呼び出した場合は、受領可能な状態に移行するまで呼び出しスレッドをストールさせます。
         */
        Result Get(void* const pOutReceivedBuffer, const size_t bufferCapacity) NN_NOEXCEPT;

        /**
         * @brief       非同期要求完了待ち同期。
         *
         * @pre
         *              - @ref ShopServiceAccessor::Request() で非同期要求に成功している。
         *
         * @details     対象非同期要求が完了するまで呼び出しスレッドをストールさせます。
         */
        void Wait() NN_NOEXCEPT;

        /**
         * @brief       非同期要求完了待ち確認。
         *
         * @return      非同期要求の完了状態を返します。
         * @retval      true    非同期要求処理は完了しています。
         * @retval      false   非同期要求処理は完了していません。
         *
         * @pre
         *              - @ref ShopServiceAccessor::Request() で非同期要求に成功している。
         */
        bool TryWait() NN_NOEXCEPT;

        /**
         * @brief       非同期要求のキャンセル要求。
         *
         * @pre
         *              - @ref ShopServiceAccessor::Request() で非同期要求に成功している。
         *
         * @details     対象の要求をキャンセルします。@n
         *              既に要求処理が完了していた場合の実施を行っても処理内容及び処理結果への影響はありません。
         */
        void Cancel() NN_NOEXCEPT;

        /**
         * @brief       非同期要求完了通知イベントオブジェクトへの参照の取得。
         *
         * @return      非同期要求完了通知イベントオブジェクトへの参照を返します。
         *
         * @pre
         *              - @ref ShopServiceAccessor::Request() で非同期要求に成功している。
         *
         * @details     非同期要求が完了した際にシグナルされるイベントオブジェクトへの参照を取得します。@n
         *              非同期処理中は非シグナル状態が維持されます。
         */
        ::nn::os::SystemEvent& GetEvent() NN_NOEXCEPT;

        /**
         * @brief       サービス固有エラーコードの取得。
         *
         * @return      サービス固有エラーコードを返します。
         *
         * @pre
         *              - @ref ShopServiceAccessor::Request() で非同期要求に成功している。
         *              - @ref Wait() などで非同期要求の完了を確認している。
         *
         * @details     ::nn::err::ErrorCode::GetInvalidErrorCode() と一致する場合は、サービス固有のエラーは発生していません。
         */
        ::nn::err::ErrorCode GetErrorCode() const NN_NOEXCEPT;

    private:
        typedef ::nn::nim::detail::IShopServiceAsync    IAsyncHandle;
        typedef ::nn::os::SystemEvent                   DoneNotifier;

        Result GetSizeImpl(size_t* const pOutValue) NN_NOEXCEPT;
        Result GetImpl(void* const pOutReceivedBuffer, const size_t bufferCapacity) NN_NOEXCEPT;

        void Attach(ShopServiceAccessor* const pOwner, IAsyncHandle* pAsyncHandle, const ::nn::sf::NativeHandle& notifyEventHandle) NN_NOEXCEPT;
        void Detach() NN_NOEXCEPT;
        void Finalize() NN_NOEXCEPT;

        ShopServiceAccessor*    m_pOwner;   //!< 要求オーナーアクセッサインスタンスへのポインタ
        IAsyncHandle*           m_pAsync;   //!< 非同期要求進捗管理ハンドル
        DoneNotifier            m_Event;    //!< 非同期要求完了状態管理イベント
    };

    /**
     * @brief       ポストデータフィールドパラメータコンテナ。
     *
     * @details     ポストデータフィールドが存在するメモリ領域を指定するコンテナクラスです。@n
     *              size メンバに指定する領域長は、終端文字付きURLエンコード文字列であれば std::strlen() の結果文字列長を利用できます。
     */
    struct PostData
    {
        char*   pBody;  //!< ポストデータフィールド領域先頭アドレスへのポインタ。
        size_t  size;   //!< ポストデータフィールド領域バイトサイズ。( 文字列構成ポストデータフィールドの場合は、null終端文字を含めない長さを想定しています )

        /**
         * @brief   コンテナ設定状態の有効性を評価します。
         *
         * @return  評価結果を返します。
         * @retval  true    ポストデータコンテナとして利用可能な状態です。
         * @retval  false   ポストデータコンテナとして利用不可能な状態です。
         *
         * @details この評価ではポストデータ列の内容は評価しません。@n
         *          コンテナ中に保持しているメンバ要素の値が入力コンテナとして利用可能かを評価します。
         */
        NN_EXPLICIT_OPERATOR bool() const NN_NOEXCEPT;
    };

    /**
     * @brief       標準通信タイムアウト( 60秒 )。
     */
    static const ::nn::TimeSpan DefaultTimeout;

    /**
     * @brief       コンストラクタ。
     *
     * @pre
     *              - @ref InitializeForShopServiceAccessors() による環境初期化済である。
     */
    explicit ShopServiceAccessor() NN_NOEXCEPT;

    /**
     * @brief       デストラクタ。
     *
     * @pre
     *              - @ref InitializeForShopServiceAccessors() による環境初期化済である。
     */
    ~ShopServiceAccessor() NN_NOEXCEPT;

    /**
     * @brief       指定ショップサービスサーバーへのアクセッサとして初期化します。
     *
     * @param[in]   target  アクセスしたい接続先ショップサービスサーバーを指定します。@n
     *                      指定可能なショップサービスサーバーを @ref ShopService::Type から選択して @ref ShopService を構築してください。
     *
     * @return      処理結果を返します。@n
     *              アプリケーションでハンドリング出来ないResult値はエラービューアに通知してください。
     * @retval      nn::ResultSuccess                           処理に成功しました。
     * @retval      nn::ec::ResultShopServiceAccessOverRequest  初期化に必要なサービスリソースを確保できませんでした。@n
     *                                                          未破棄の発行済非同期要求ハンドル( @ref AsyncResponse )があれば破棄した後に再度要求してください。@n
     *                                                          もしくは、 ShopServiceAccessor インスタンスの同時利用数を減らしてください。
     */
    Result Initialize(const ShopService& target) NN_NOEXCEPT;

    /**
     * @brief       アクセッサを終了します。
     *
     * @return      処理結果を返します。
     * @retval      nn::ResultSuccess   処理に成功しました。
     *
     * @details     運用していたサービスリソースが破棄されます。@n
     *              本メソッドの呼び出し後のアクセッサインスタンスへのアクセスは正常に動作しません。@n
     *              デストラクタ呼び出し前に明示的なサービスリソース破棄を行いたい場合に利用します。
     */
    Result Finalize() NN_NOEXCEPT;

    /**
     * @brief       通信リクエスト実施要求( ポストデータあり、タイムアウト指定可能インタフェース )。
     *
     * @param[out]  pOutHandle      非同期要求ハンドル( @ref AsyncResponse )を受け取るためのインスタンスへのポインタを指定します。
     * @param[in]   userId          ショップサービスアクセスを希望するユーザ識別子。
     * @param[in]   method          要求する HTTP メソッドを指定します。
     * @param[in]   pPath           URLパスフィールド文字列を指定します。( null終端文字列 )
     * @param[in]   postData        ポストデータフィールドデータコンテナを指定します。@n
     *                              指定されたデータ列に対する自動的なストリームエンコーディングは行われません。@n
     *                              接続先ショップサービスサーバーの要求するポストデータフィールドのエンコードに準拠して設定してください。
     * @param[in]   timeout         秒単位の通信タイムアウト値を指定します。@n
     *                              1秒未満を指定した場合、デフォルト値( @ref DefaultTimeout )が採用されます。@n
     *                              また、指定可能な上限秒数は INT_MAX です、超過した場合は INT_MAX に丸められます。
     *
     * @return      処理結果を返します。@n
     *              アプリケーションでハンドリング出来ないResult値はエラービューアに通知してください。
     * @retval      nn::ResultSuccess                                       処理に成功しました。
     * @retval      nn::ec::ResultShopServiceAccessInvalidCharacter         要求パラメータ中にパラメータとして使用不可の無効文字を検出しました。
     * @retval      nn::ec::ResultShopServiceAccessOverRequest              非同期要求に必要なサービスリソースを確保できませんでした。@n
     *                                                                      未破棄の発行済非同期要求ハンドル( @ref AsyncResponse )があれば破棄した後に再度要求してください。@n
     *                                                                      もしくは、ShopServiceAccessor インスタンスの同時利用数を減らしてください。
     * @retval      nn::ec::ResultShopServiceAccessInsufficientWorkMemory   非同期要求用ワークメモリ( ワークバッファ )が不足しています。
     * @retval      nn::ec::ResultInternetRequestNotAccepted                インターネット接続要求が提出、及び受理されていません。
     *
     * @pre
     *              - @ref Initialize(const ShopService&) により初期化に成功している。
     *              - variableParameters に期待する要求パラメータを準備できている。
     *              - pOutHandle != nullptr
     *              - userId == true
     *              - nullptr != pPath
     *              - '/' == pPath[0]
     *
     * @details     結果レスポンスは @ref AsyncResponse::Get() で取得できます。
     */
    Result Request(AsyncResponse* pOutHandle, const ::nn::account::Uid& userId, const ShopService::Method method, const char* pPath, const PostData& postData, const ::nn::TimeSpan timeout) NN_NOEXCEPT;

    /**
     * @brief       通信リクエスト実施要求( ポストデータなし、タイムアウト指定可能インタフェース )。
     *
     * @param[out]  pOutHandle      非同期要求ハンドル( @ref AsyncResponse )を受け取るためのインスタンスへのポインタを指定します。
     * @param[in]   userId          ショップサービスアクセスを希望するユーザ識別子。
     * @param[in]   method          要求する HTTP メソッドを指定します。
     * @param[in]   pPath           URLパスフィールド文字列を指定します。( null終端文字列 )
     * @param[in]   timeout         秒単位の通信タイムアウト値を指定します。@n
     *                              1秒未満を指定した場合、デフォルト値( @ref DefaultTimeout )が採用されます。@n
     *                              また、指定可能な上限秒数は INT_MAX です、超過した場合は INT_MAX に丸められます。
     *
     * @return      処理結果を返します。@n
     *              アプリケーションでハンドリング出来ないResult値はエラービューアに通知してください。
     * @retval      nn::ResultSuccess                                       処理に成功しました。
     * @retval      nn::ec::ResultShopServiceAccessInvalidCharacter         要求パラメータ中にパラメータとして使用不可の無効文字を検出しました。
     * @retval      nn::ec::ResultShopServiceAccessOverRequest              非同期要求に必要なサービスリソースを確保できませんでした。@n
     *                                                                      未破棄の発行済非同期要求ハンドル( @ref AsyncResponse )があれば破棄した後に再度要求してください。@n
     *                                                                      もしくは、ShopServiceAccessor インスタンスの同時利用数を減らしてください。
     * @retval      nn::ec::ResultShopServiceAccessInsufficientWorkMemory   非同期要求用ワークメモリ( ワークバッファ )が不足しています。
     * @retval      nn::ec::ResultInternetRequestNotAccepted                インターネット接続要求が提出、及び受理されていません。
     *
     * @pre
     *              - @ref Initialize(const ShopService&) により初期化に成功している。
     *              - variableParameters に期待する要求パラメータを準備できている。
     *              - pOutHandle != nullptr
     *              - userId == true
     *              - nullptr != pPath
     *              - '/' == pPath[0]
     *
     * @details     結果レスポンスは @ref AsyncResponse::Get() で取得できます。
     */
    Result Request(AsyncResponse* pOutHandle, const ::nn::account::Uid& userId, const ShopService::Method method, const char* pPath, const ::nn::TimeSpan timeout) NN_NOEXCEPT;

    /**
     * @brief       通信リクエスト実施要求( ポストデータあり、標準タイムアウト採用インタフェース )。
     *
     * @param[out]  pOutHandle      非同期要求ハンドル( @ref AsyncResponse )を受け取るためのインスタンスへのポインタを指定します。
     * @param[in]   userId          ショップサービスアクセスを希望するユーザ識別子。
     * @param[in]   method          要求する HTTP メソッドを指定します。
     * @param[in]   pPath           URLパスフィールド文字列を指定します。( null終端文字列 )
     * @param[in]   postData        ポストデータフィールドデータコンテナを指定します。@n
     *                              指定されたデータ列に対する自動的なストリームエンコーディングは行われません。@n
     *                              接続先ショップサービスサーバーの要求するポストデータフィールドのエンコードに準拠して設定してください。
     *
     * @return      処理結果を返します。@n
     *              アプリケーションでハンドリング出来ないResult値はエラービューアに通知してください。
     * @retval      nn::ResultSuccess                                       処理に成功しました。
     * @retval      nn::ec::ResultShopServiceAccessInvalidCharacter         要求パラメータ中にパラメータとして使用不可の無効文字を検出しました。
     * @retval      nn::ec::ResultShopServiceAccessOverRequest              非同期要求に必要なサービスリソースを確保できませんでした。@n
     *                                                                      未破棄の発行済非同期要求ハンドル( @ref AsyncResponse )があれば破棄した後に再度要求してください。@n
     *                                                                      もしくは、ShopServiceAccessor インスタンスの同時利用数を減らしてください。
     * @retval      nn::ec::ResultShopServiceAccessInsufficientWorkMemory   非同期要求用ワークメモリ( ワークバッファ )が不足しています。
     * @retval      nn::ec::ResultInternetRequestNotAccepted                インターネット接続要求が提出、及び受理されていません。
     *
     * @pre
     *              - @ref Initialize(const ShopService&) により初期化に成功している。
     *              - variableParameters に期待する要求パラメータを準備できている。
     *              - pOutHandle != nullptr
     *              - userId == true
     *              - nullptr != pPath
     *              - '/' == pPath[0]
     *
     * @details     結果レスポンスは @ref AsyncResponse::Get() で取得できます。@n
     *              通信タイムアウトは標準タイムアウト @ref DefaultTimeout が採用されます。
     */
    Result Request(AsyncResponse* pOutHandle, const ::nn::account::Uid& userId, const ShopService::Method method, const char* pPath, const PostData& postData) NN_NOEXCEPT;

    /**
     * @brief       通信リクエスト実施要求( ポストデータなし、標準タイムアウト採用インタフェース )。
     *
     * @param[out]  pOutHandle      非同期要求ハンドル( @ref AsyncResponse )を受け取るためのインスタンスへのポインタを指定します。
     * @param[in]   userId          ショップサービスアクセスを希望するユーザ識別子。
     * @param[in]   method          要求する HTTP メソッドを指定します。
     * @param[in]   pPath           URLパスフィールド文字列を指定します。( null終端文字列 )
     *
     * @return      処理結果を返します。@n
     *              アプリケーションでハンドリング出来ないResult値はエラービューアに通知してください。
     * @retval      nn::ResultSuccess                                       処理に成功しました。
     * @retval      nn::ec::ResultShopServiceAccessInvalidCharacter         要求パラメータ中にパラメータとして使用不可の無効文字を検出しました。
     * @retval      nn::ec::ResultShopServiceAccessOverRequest              非同期要求に必要なサービスリソースを確保できませんでした。@n
     *                                                                      未破棄の発行済非同期要求ハンドル( @ref AsyncResponse )があれば破棄した後に再度要求してください。@n
     *                                                                      もしくは、ShopServiceAccessor インスタンスの同時利用数を減らしてください。
     * @retval      nn::ec::ResultShopServiceAccessInsufficientWorkMemory   非同期要求用ワークメモリ( ワークバッファ )が不足しています。
     * @retval      nn::ec::ResultInternetRequestNotAccepted                インターネット接続要求が提出、及び受理されていません。
     *
     * @pre
     *              - @ref Initialize(const ShopService&) により初期化に成功している。
     *              - variableParameters に期待する要求パラメータを準備できている。
     *              - pOutHandle != nullptr
     *              - userId == true
     *              - nullptr != pPath
     *              - '/' == pPath[0]
     *
     * @details     結果レスポンスは @ref AsyncResponse::Get() で取得できます。@n
     *              通信タイムアウトは標準タイムアウト @ref DefaultTimeout が採用されます。
     */
    Result Request(AsyncResponse* pOutHandle, const ::nn::account::Uid& userId, const ShopService::Method method, const char* pPath) NN_NOEXCEPT;

    /**
     * @brief       要求可能なインスタンス状態かどうかの確認。
     *
     * @return      要求可能なインスタンス状態かどうかの確認結果を返します。
     * @retval      true    このインスタンスは利用可能です。
     * @retval      false   このインスタンスは利用はできません。( 未初期化状態か、もしくは終了しています。 )
     */
    bool IsAvailable() const NN_NOEXCEPT;

private:
    /**
     * @brief   非同期要求リスト
     */
    typedef ::nn::util::IntrusiveList<AsyncResponse, ::nn::util::IntrusiveListBaseNodeTraits<AsyncResponse>> TAsyncList;

    /**
     * @brief   サービスフレームワークインタフェース型の宣言
     */
    typedef ::nn::nim::detail::IShopServiceAccessor IAccessor;

    void Attach(AsyncResponse* pAsyncResponse) NN_NOEXCEPT;
    void Detach(AsyncResponse* pAsyncResponse) NN_NOEXCEPT;

    IAccessor*              m_pImpl;        //!< IPCインタフェースコンテナ
    mutable ::nn::os::Mutex m_Lock;         //!< アクセス排他オブジェクト
    TAsyncList              m_ListChild;    //!< 実施中 AsyncResponse インスタンスリスト
};

//----------------------------------------------------------------------------
// インライン実装
#if !defined(NN_BUILD_FOR_DOCUMENT_GENERATION)
//----------------------------------------------------------------------------
inline ShopServiceAccessor::PostData::operator bool() const NN_NOEXCEPT
{
    return (nullptr != pBody && size > 0);
}
//----------------------------------------------------------------------------
inline Result ShopServiceAccessor::Request(AsyncResponse* pOutHandle, const ::nn::account::Uid& userId, const ShopService::Method method, const char* pPath, const ::nn::TimeSpan timeout) NN_NOEXCEPT
{
    return Request(pOutHandle, userId, method, pPath, PostData{nullptr, 0}, timeout);
}
//----------------------------------------------------------------------------
inline Result ShopServiceAccessor::Request(AsyncResponse* pOutHandle, const ::nn::account::Uid& userId, const ShopService::Method method, const char* pPath, const PostData& postData) NN_NOEXCEPT
{
    return Request(pOutHandle, userId, method, pPath, postData, DefaultTimeout);
}
//----------------------------------------------------------------------------
inline Result ShopServiceAccessor::Request(AsyncResponse* pOutHandle, const ::nn::account::Uid& userId, const ShopService::Method method, const char* pPath) NN_NOEXCEPT
{
    return Request(pOutHandle, userId, method, pPath, DefaultTimeout);
}
//----------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------

}}

