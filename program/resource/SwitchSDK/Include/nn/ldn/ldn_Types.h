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

#include <limits>
#include <nn/nn_Common.h>
#include <nn/ldn/ldn_Ipv4Address.h>
#include <nn/ldn/ldn_MacAddress.h>
#include <nn/ldn/ldn_NetworkId.h>
#include <nn/ldn/ldn_Ssid.h>

namespace nn { namespace ldn
{
    //
    // 通信品質を表す定数です。
    //

    const int LinkLevelMax       = 3;   //!< 通信品質の最大値で、通信品質が非常に良い状況です。
    const int LinkLevelMin       = 0;   //!< 通信品質の最小値で、通信品質が非常に悪い状況です。

    /**
     * @brief         ローカルネットワークに接続できるノードの最大数です。
     */
    const int NodeCountMax = 8;

    /**
     * @brief         ローカルネットワークに接続できるステーションの最大数です。
     */
    const int StationCountMax = NodeCountMax - 1;

    /**
     * @brief         自動的にチャンネルを選択します。
     */
    const int AutoChannel = 0;

    /**
     * @brief         ビーコンに載せて配信できる任意データの最大サイズです。
     */
    const size_t AdvertiseDataSizeMax = 384;

    /**
     * @brief         パスフレーズの最小バイト数です。
     */
    const size_t PassphraseSizeMin = 16;

    /**
     * @brief         パスフレーズの最大バイト数です。
     */
    const size_t PassphraseSizeMax = 64;

    /**
     * @brief         ユーザ名の最大バイト数です。終端文字は含みません。
     */
    const size_t UserNameBytesMax = 32;

    /**
     * @brief         ユーザ名の最大文字数です。終端文字は含みません。
     */
    const int UserNameLengthMax = 10;

    /**
     * @brief         ローカル通信バージョンの最小値です。
     */
    const int LocalCommunicationVersionMin = 0;

    /**
     * @brief         ローカル通信バージョンの最大値です。
     */
    const int LocalCommunicationVersionMax = 32767;

    /**
     * @brief         一度にスキャンで探索できるネットワークの最大数です。
     */
    const int ScanResultCountMax = 24;

    /**
     * @brief         ステーションの接続を制限するフィルタに登録できるエントリの最大数です。
     */
    const int AcceptFilterEntryCountMax = 256;

    /*
     * @brief         疑似乱数のサイズです。
     */
    const size_t RandomSize = 16;

    /**
     * @brief         LDN ライブラリの状態を表す型です。
     */
    enum State
    {
        State_None,                     //!< ローカル通信ライブラリは初期化されていません。
        State_Initialized,              //!< ローカル通信ライブラリが初期化されました。
        State_AccessPoint,              //!< アクセスポイントとしての動作を開始しました。
        State_AccessPointCreated,       //!< アクセスポイントとしてネットワークを構築しています。
        State_Station,                  //!< ステーションとしての動作を開始しました。
        State_StationConnected,         //!< ステーションとしてネットワークに接続しています。
        State_Error                     //!< 無線オフやスリープによって発生したエラー状態です。
    };

    /**
     * @brief         アクセスポイントの接続受付に関するポリシーです。
     */
    enum AcceptPolicy
    {
        //! 接続要求を全て受け入れます。
        AcceptPolicy_AlwaysAccept,

        //! 接続要求を全て拒否します。
        AcceptPolicy_AlwaysReject,

        //! フィルタに登録されているノードの接続を拒否します。
        AcceptPolicy_BlackList,

        //! フィルタに登録されているノードのみが接続できます。
        AcceptPolicy_WhiteList
    };

    /**
     * @brief         ステーションの切断理由です。
     */
    enum DisconnectReason
    {
        //! 切断理由は不明です。
        DisconnectReason_Unknown = -1,

        //! 切断されていません。
        DisconnectReason_None,

        //! アプリケーションの要求によってネットワークから切断しました。
        DisconnectReason_DisconnectedByUser,

        //! システムの要求によってネットワークから切断しました。
        DisconnectReason_DisconnectedBySystem,

        //! アプリケーションの要求によってアクセスポイントがネットワークを破棄しました。
        DisconnectReason_DestroyedByUser,

        //! アクセスポイントがシステムの要求によってネットワークを破棄しました。
        DisconnectReason_DestroyedBySystem,

        //! アクセスポイントがステーションを切断しました。
        DisconnectReason_Rejected,

        //! 通信環境が不安定なため切断されました。
        DisconnectReason_SignalLost,
    };

    /**
     * @brief         アクセスポイントが構築するローカル・ネットワークに関する設定情報です。
     */
    struct NetworkConfig
    {
        //! ローカル・ネットワークの用途を表明する識別子です。
        IntentId            intentId;

        //! 無線チャンネルです。 @ref AutoChannel を指定すると自動的に選択します。
        int16_t             channel;

        //! ネットワークに接続できるノードの最大数です。
        int8_t              nodeCountMax;
        Bit8                _reserved1[1];

        //! ローカル通信バージョンです。
        int16_t             localCommunicationVersion;
        Bit8                _reserved2[10];
    };
    NN_STATIC_ASSERT(sizeof(NetworkConfig) == 32);

    /**
     * @brief         ネットワークに参加するユーザの設定情報です。
     */
    struct UserConfig
    {
        //! ユーザ名です。UTF-8 で指定してください。
        char                userName[UserNameBytesMax + 1];

        //  予約領域です。
        Bit8                _reserved[15];
    };
    NN_STATIC_ASSERT(sizeof(UserConfig) == 48);

    /**
     * @brief         ローカル通信のセキュリティ・モードです。
     */
    enum SecurityMode
    {
        SecurityMode_Any,               //!< セキュリティ・モードを指定しません。
        SecurityMode_Product,           //!< 製品用のセキュリティ・モードです。
        SecurityMode_Debug,             //!< デバッグ用のセキュリティ・モードです。
        SecurityMode_SystemDebug        //   システムデバッグ用のセキュリティ・モードです。
    };

    /**
     * @brief         アクセスポイントが構築するローカル・ネットワークのセキュリティ情報です。
     */
    struct SecurityConfig
    {
        //! ローカル通信のセキュリティ・モードです。 @ref SecurityMode のいずれかを指定します。
        Bit16               securityMode;

        //! アクセスポイントへの接続に必要なパスフレーズのバイト長です。
        uint16_t            passphraseSize;

        //! アクセスポイントへの接続に必要なパスフレーズです。
        Bit8                passphrase[PassphraseSizeMax];
    };
    NN_STATIC_ASSERT(sizeof(SecurityConfig) == 68);

    /**
     * @brief         ネットワークの種別を表す定数です。
     */
    enum NetworkType
    {
        //! 一般のアクセスポイントによって構築されたネットワークです。
        NetworkType_General             = 1 << 0,

        //! LDN ライブラリによって構築されたネットワークです。
        NetworkType_Ldn                 = 1 << 1
    };

    /**
     * @brief         スキャンのフィルタリング条件を指定するフラグです。
     */
    enum ScanFilterFlag
    {
        //! ローカル通信識別子でスキャン結果のフィルタリングを行います。
        ScanFilterFlag_LocalCommunicationId = 1 << 0,

        //! SessionId でスキャン結果のフィルタリングを行います。
        ScanFilterFlag_SessionId = 1 << 1,

        //! ScanTarget でスキャン結果のフィルタリングを行います。
        ScanFilterFlag_NetworkType = 1 << 2,

        //  BSSID でスキャン結果のフィルタリングを行います。
        ScanFilterFlag_Bssid = 1 << 3,

        //! SSID でスキャン結果のフィルタリングを行います。
        ScanFilterFlag_Ssid = 1 << 4,

        //! シーン識別子でスキャン結果のフィルタリングを行います。
        ScanFilterFlag_SceneId = 1 << 5,

        //! IntentId でスキャン結果のフィルタリングを行います。
        ScanFilterFlag_IntentId = ScanFilterFlag_LocalCommunicationId | ScanFilterFlag_SceneId,

        //! IntentId と SessionId を含む NetworkId でスキャン結果のフィルタリングを行います。
        ScanFilterFlag_NetworkId = ScanFilterFlag_IntentId | ScanFilterFlag_SessionId,

        //  全ての有効なフィルタを表すマスクです。
        ScanFilterFlag_All = ScanFilterFlag_NetworkType |
                             ScanFilterFlag_IntentId |
                             ScanFilterFlag_SessionId |
                             ScanFilterFlag_Bssid |
                             ScanFilterFlag_Ssid
    };

    /**
     * @brief         スキャンのフィルタを格納する構造体です。
     */
    struct ScanFilter
    {
        //! ネットワークの識別子です。
        NetworkId           networkId;

        //! スキャン対象となるネットワークです。 @ref NetworkType の値を論理和で指定してください。
        Bit32               networkType;

        // アクセスポイントの BSSID です。
        // DO NOT USE THIS MEMBER
        MacAddress          bssid;

        //! アクセスポイントの SSID です。
        Ssid                ssid;

        //  予約領域です。
        Bit8                _reserved[16];

        //! フィルタリングに使用する項目を @ref ScanFilterFlag の論理和で指定します。
        Bit32               flag;
    };
    NN_STATIC_ASSERT(sizeof(ScanFilter) == 96);

    /**
     * @brief         一般的なネットワーク情報を格納する構造体です。
     */
    struct CommonNetworkInfo
    {
        // アクセスポイントの BSSID です。
        // DO NOT USE THIS MEMBER
        MacAddress          bssid;

        //! アクセスポイントの SSID です。
        Ssid                ssid;

        //! 無線チャンネルです。
        int16_t             channel;

        //! 通信品質です。
        int8_t              linkLevel;

        //! ネットワークの種別です。
        Bit8                networkType;

        //  予約領域です。
        Bit8                _reserved[4];
    };
    NN_STATIC_ASSERT(sizeof(CommonNetworkInfo) == 48);

    /**
     * @brief         ネットワークに参加しているノードの情報です。
     */
    struct NodeInfo
    {
        //! アクセスポイントから割り当てられた IPv4 アドレスです。
        Ipv4Address         ipv4Address;

        // MAC アドレスです。
        // DO NOT USE THIS MEMBER
        MacAddress          macAddress;

        //! ノード番号です。
        int8_t              nodeId;

        //! 接続状態です。
        bool                isConnected;

        //! ユーザ名です。
        char                userName[UserNameBytesMax + 1];
        Bit8                _reserved1[1];

        //! ローカル通信バージョンです。
        int16_t             localCommunicationVersion;
        Bit8                _reserved2[16];
    };
    NN_STATIC_ASSERT(sizeof(NodeInfo) == 64);

    /**
     * @brief         LDN 特有のネットワーク情報を格納する構造体です。
     */
    struct LdnNetworkInfo
    {
        //  内部処理用のパラメータです。
        Bit8                serverRandom[RandomSize];

        //! アクセスポイントの @ref SecurityMode です。
        Bit16               securityMode;

        //! ステーションの接続受付ポリシー (@ref AcceptPolicy) です。
        Bit8                stationAcceptPolicy;
        uint8_t             _version;
        char                _reserved1[2];

        //! ネットワークに参加可能なノードの最大台数です。アクセスポイントを含みます。
        int8_t              nodeCountMax;

        //! 現時点でネットワークに接続しているノードの台数です。
        int8_t              nodeCount;

        //! ネットワークに参加しているノードの情報です。
        NodeInfo            nodes[NodeCountMax];
        Bit8                _reserved2[2];

        //! スキャン結果に含まれる任意データのバイトサイズです。
        uint16_t            advertiseDataSize;

        //! スキャン結果に含まれる任意データです。
        Bit8                advertiseData[AdvertiseDataSizeMax];
        char                _reserved3[140];
        uint64_t            _challenge;
    };
    NN_STATIC_ASSERT(sizeof(LdnNetworkInfo) == 1072);

    /**
     * @brief         スキャン結果を格納する構造体です。
     */
    struct NetworkInfo
    {
        //! ネットワークの識別子です。
        NetworkId           networkId;

        //! 一般的なネットワーク情報を格納する構造体です。
        CommonNetworkInfo   common;

        //! LDN 特有のネットワーク情報です。
        LdnNetworkInfo      ldn;
    };
    NN_STATIC_ASSERT(sizeof(NetworkInfo) == 1152);

    /**
     * @brief         ノードの接続状態の変化です。
     */
    enum NodeStateChange
    {
        //! 接続状態に変化はありません。
        NodeStateChange_None,

        //! ノードの接続です。
        NodeStateChange_Connect,

        //! ノードの切断です。
        NodeStateChange_Disconnect,

        //! ノードの切断後、接続が発生しました。同じノードが再度接続した場合も含みます。
        NodeStateChange_DisconnectAndConnect
    };

    /**
     * @brief         ネットワークに参加しているノードの変化を記録する構造体です。
     */
    struct NodeLatestUpdate
    {
        //! 接続状態の変化です。詳細については @ref NodeStateChange を参照してください。
        int8_t stateChange;
        Bit8 _reserved[7];
    };
    NN_STATIC_ASSERT(sizeof(NodeLatestUpdate) == 8);

    /**
     * @brief         nn::ldn::Connect() の挙動を指定するオプションです。
     */
    enum ConnectOption
    {
        //! いかなるオプションも指定せず、全て無効にします。
        ConnectOption_None = 0,
    };

    //! ResultLowerVersion, ResultHigherVersion の発生時にエラーメッセージを表示します。
    NN_DEPRECATED const ConnectOption ConnectOption_ShowErrorMessage =
        static_cast<ConnectOption>(1 << 0);

    //! オプションが指定されなかった場合に使用されるデフォルト値です。
    NN_DEPRECATED const ConnectOption ConnectOption_Default = static_cast<ConnectOption>(1 << 0);

    //! 全てのオプションを指定します。
    NN_DEPRECATED const ConnectOption ConnectOption_All = static_cast<ConnectOption>(1 << 0);

}} // end of namespace nn::ldn

namespace nn { namespace ldn { namespace detail
{
    // 文字列に変換します。
    size_t ConvertToString(State state, char* buffer, size_t bufferSize) NN_NOEXCEPT;
    size_t ConvertToString(AcceptPolicy policy, char* buffer, size_t bufferSize) NN_NOEXCEPT;
    size_t ConvertToString(DisconnectReason reason, char* buffer, size_t bufferSize) NN_NOEXCEPT;
    size_t ConvertToString(SecurityMode securityMode, char* buffer, size_t bufferSize) NN_NOEXCEPT;

    // 文字列に変換された後の文字数です。
    const size_t StringizedStateLengthMax = 32;
    const size_t StringizedAcceptPolicyLengthMax = 32;
    const size_t StringizedDisconnectReasonLengthMax = 32;
    const size_t StringizedSecurityModeLengthMax = 32;

}}} // namespace nn::ldn::detail
