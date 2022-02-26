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
#include <nn/ldn/ldn_Types.h>
#include <nn/ldn/ldn_Ipv4Address.h>
#include <nn/os/os_SystemEventTypes.h>

namespace nn { namespace ldn
{
    //! @name 共通の API
    //! @{

    /**
     * @brief         ローカル通信ライブラリを初期化します。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result Initialize() NN_NOEXCEPT;

    /**
     * @brief         ローカル通信ライブラリを解放します。
     */
    void Finalize() NN_NOEXCEPT;

    /**
     * @brief         接続状態の変更を通知するイベントを取得します。
     *
     * @param[out]    pOutSystemEvent       イベントの出力先です。
     */
    void AttachStateChangeEvent(nn::os::SystemEventType* pOutSystemEvent) NN_NOEXCEPT;

    /**
     * @brief         ライブラリの状態を取得します。
     *
     * @return        ライブラリの状態です。
     */
    State GetState() NN_NOEXCEPT;

    /**
     * @brief         周囲のネットワークを探索します。
     *
     * @param[out]    outBuffer             スキャン結果の出力先です。
     * @param[out]    pOutCount             @a outBuffer に出力されたスキャン結果の数です。
     * @param[in]     bufferCount           @a outBuffer に格納できるスキャン結果の数です。
     * @param[in]     filter                スキャンの条件です
     * @param[in]     channel               スキャン対象のチャンネルです。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result Scan(NetworkInfo* outBuffer, int* pOutCount, int bufferCount,
                const ScanFilter& filter, int channel) NN_NOEXCEPT;

    /**
     * @brief         現在参加しているネットワークの情報を取得します。
     *
     * @param[out]    pOutNetwork   ネットワーク情報の出力先です。
     * @param[out]    pOutUpdates   ネットワークに参加しているノードの変化の出力先です。
     * @param[in]     bufferCount   @a pOutUpdates に格納できる @ref NodeLatestUpdate の数です。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result GetNetworkInfo(
        NetworkInfo* pOutNetwork, NodeLatestUpdate* pOutUpdates, int bufferCount) NN_NOEXCEPT;

    /**
     * @brief         現在参加しているネットワークの情報を取得します。
     *
     * @param[out]    pOutNetwork   ネットワーク情報の出力先です。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result GetNetworkInfo(NetworkInfo* pOutNetwork) NN_NOEXCEPT;

    /**
     * @brief         自身の IP アドレスを取得します。
     *
     * @param[out]    pOutAddress   IP アドレスの出力先です。
     * @param[out]    pOutMask      サブネットマスクの出力先です。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result GetIpv4Address(Ipv4Address* pOutAddress, SubnetMask* pOutMask) NN_NOEXCEPT;

    //! @}
    //! @name アクセスポイント専用の API
    //! @{

    /**
     * @brief         アクセスポイントモードで通信を開始します。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result OpenAccessPoint() NN_NOEXCEPT;

    /**
     * @brief         アクセスポイントモードを終了します。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result CloseAccessPoint() NN_NOEXCEPT;

    /**
     * @brief         アクセスポイントとして新規にネットワークを構築します。
     *
     * @param[in]     network               新規に構築するネットワークの設定情報です。
     * @param[in]     security              新規に構築するネットワークのセキュリティ情報です。
     * @param[in]     user                  ネットワークを構築するユーザの情報です。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultNodeCountLimitation}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result CreateNetwork(
        const NetworkConfig&     network,
        const SecurityConfig&    security,
        const UserConfig&        user
    ) NN_NOEXCEPT;

    /**
     * @brief         アクセスポイントとして構築したネットワークを破棄します。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result DestroyNetwork() NN_NOEXCEPT;

    /**
     * @brief         アクセスポイントが配信する任意データを設定します。
     *
     * @param[in]     data                  配信するデータです。
     * @param[in]     dataSize              配信するデータのサイズです。
     *                                      @ref AdvertiseDataSizeMax 以下の値を設定してください。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result SetAdvertiseData(const void* data, size_t dataSize) NN_NOEXCEPT;

    /**
     * @brief         アクセスポイントにおけるステーションの接続受付ポリシーを変更します。
     *
     * @param[in]     policy                新しい接続受付ポリシーです。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result SetStationAcceptPolicy(AcceptPolicy policy) NN_NOEXCEPT;

    /**
     * @brief         指定したステーションをネットワークから追放します。
     *
     * @param[in]     ipv4Address           対象のステーションの IPv4 アドレスです。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultNodeNotFound}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result Reject(Ipv4Address ipv4Address) NN_NOEXCEPT;

    /**
     * @brief         ステーションの接続を制限するフィルタにエントリを追加します。
     *
     * @param[in]     station               対象のステーションです。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result AddAcceptFilterEntry(const NodeInfo& station) NN_NOEXCEPT;

    /**
     * @brief         フィルタに登録されているエントリを全て削除します。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result ClearAcceptFilter() NN_NOEXCEPT;

    //! @}
    //! @name ステーション専用の API
    //! @{

    /**
     * @brief         ステーションモードで通信を開始します。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result OpenStation() NN_NOEXCEPT;

    /**
     * @brief         ステーションモードを終了します。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result CloseStation() NN_NOEXCEPT;

    /**
     * @brief         ステーションとして指定されたネットワークに接続します。
     *
     * @param[in]     network               接続先です。
     * @param[in]     security              接続に必要なセキュリティパラメータです。
     * @param[in]     user                  ネットワークに参加するユーザの情報です。
     * @param[in]     version               ローカル通信バージョンです。
     * @param[in]     option                挙動を指定するオプションです。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultNodeCountLimitation}
     *  @handleresult{ResultDeviceNotAvailable}
     *  @handleresult{ResultConnectionFailed}
     *  @handleresult{ResultIncompatibleVersion}
     * @endretresult
     */
    Result Connect(
        const NetworkInfo& network,
        const SecurityConfig& security,
        const UserConfig& user,
        int version,
        ConnectOption option) NN_NOEXCEPT;

    /**
     * @brief         ステーションとして指定されたネットワークに接続します。
     *
     * @param[in]     network               接続先です。
     * @param[in]     security              接続に必要なセキュリティパラメータです。
     * @param[in]     user                  ネットワークに参加するユーザの情報です。
     * @param[in]     version               ローカル通信バージョンです。
     *
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultNodeCountLimitation}
     *  @handleresult{ResultDeviceNotAvailable}
     *  @handleresult{ResultConnectionFailed}
     *  @handleresult{ResultIncompatibleVersion}
     * @endretresult
     */
    NN_DEPRECATED Result Connect(
        const NetworkInfo& network,
        const SecurityConfig& security,
        const UserConfig& user,
        int version) NN_NOEXCEPT;

    /**
     * @brief         ステーションとして接続中のネットワークから切断します。
     *
     * @result        処理の結果です。
     * @retresult
     *  @handleresult{ResultSuccess}
     *  @handleresult{ResultInvalidState}
     *  @handleresult{ResultDeviceNotAvailable}
     * @endretresult
     */
    Result Disconnect() NN_NOEXCEPT;

    /**
     * @brief         ネットワークから切断された理由を取得します。
     *
     * @result        ネットワークから切断された理由です。
     */
    DisconnectReason GetDisconnectReason() NN_NOEXCEPT;

    //! @}

}} // end of namespace nn::ldn
