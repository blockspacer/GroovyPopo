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

#include <cstring>
#include <nn/nn_Common.h>
#include <nn/nn_StaticAssert.h>

namespace nn { namespace ldn
{
    /**
     * @brief         デフォルトのローカル通信識別子です。
     */
    const Bit64 DefaultLocalCommunicationId = UINT64_C(0xFFFFFFFFFFFFFFFF);

    /**
     * @brief         ネットワークの目的を表す識別子です。
     */
    struct IntentId
    {
        //! ローカル通信識別子です。
        Bit64       localCommunicationId;
        Bit8        _reserved1[2];

        //! シーン識別子です。
        Bit16       sceneId;
        Bit8        _reserved2[4];
    };
    NN_STATIC_ASSERT(sizeof(IntentId) == 16);

    /**
     * @brief        セッション毎に生成される識別子です。
     */
    struct SessionId
    {
        Bit8        random[16];
    };
    NN_STATIC_ASSERT(sizeof(SessionId) == 16);

    /**
     * @brief         ローカルネットワークの識別子です。
     */
    struct NetworkId
    {
        //! ネットワークの目的を表す識別子です。
        IntentId    intentId;

        //! セッション毎に生成される識別子です。
        SessionId   sessionId;
    };
    NN_STATIC_ASSERT(sizeof(NetworkId) == 32);

    //! @name Network ID 関連
    //! @{

    /**
     * @brief         Intent ID を生成します。
     *
     * @param[in]     localId     ローカル通信識別子です。
     * @param[in]     sceneId     シーン識別子です。
     *
     * @return        ネットワークの目的を表す IntentId です。
     */
    inline IntentId MakeIntentId(Bit64 localId, Bit16 sceneId) NN_NOEXCEPT
    {
        IntentId intentId;
        std::memset(&intentId, 0, sizeof(intentId));
        intentId.localCommunicationId = localId;
        intentId.sceneId = sceneId;
        return intentId;
    }

    /**
     * @brief         IntentId を比較します。
     *
     * @param[in]     lhs         比較対象の IntentId
     * @param[in]     rhs         比較対象の IntentId
     *
     * @return        IntentId が等しい場合に true です。
     */
    inline bool operator == (const IntentId& lhs,
                             const IntentId& rhs) NN_NOEXCEPT
    {
        return std::memcmp(&lhs, &rhs, sizeof(IntentId)) == 0;
    }

    /**
     * @brief         IntentId を比較します。
     *
     * @param[in]     lhs         比較対象の IntentId
     * @param[in]     rhs         比較対象の IntentId
     *
     * @return        IntentId が等しくない場合に true です。
     */
    inline bool operator != (const IntentId& lhs,
                             const IntentId& rhs) NN_NOEXCEPT
    {
        return std::memcmp(&lhs, &rhs, sizeof(IntentId)) != 0;
    }

    /**
     * @brief         SessionId を比較します。
     *
     * @param[in]     lhs         比較対象の SessionId
     * @param[in]     rhs         比較対象の SessionId
     *
     * @return        SessionId が等しい場合に true です。
     */
    inline bool operator == (const SessionId& lhs,
                             const SessionId& rhs) NN_NOEXCEPT
    {
        return std::memcmp(&lhs, &rhs, sizeof(SessionId)) == 0;
    }

    /**
     * @brief         SessionId を比較します。
     *
     * @param[in]     lhs         比較対象の SessionId
     * @param[in]     rhs         比較対象の SessionId
     *
     * @return        SessionId が等しくない場合に true です。
     */
    inline bool operator != (const SessionId& lhs,
                             const SessionId& rhs) NN_NOEXCEPT
    {
        return std::memcmp(&lhs, &rhs, sizeof(SessionId)) != 0;
    }

    /**
     * @brief         ネットワーク識別子を比較します。
     *
     * @param[in]     lhs         比較対象のネットワーク識別子
     * @param[in]     rhs         比較対象のネットワーク識別子
     *
     * @return        ネットワーク識別子が等しい場合に true です。
     */
    inline bool operator == (const NetworkId& lhs,
                             const NetworkId& rhs) NN_NOEXCEPT
    {
        return std::memcmp(&lhs, &rhs, sizeof(NetworkId)) == 0;
    }

    /**
     * @brief         ネットワーク識別子を比較します。
     *
     * @param[in]     lhs         比較対象のネットワーク識別子
     * @param[in]     rhs         比較対象のネットワーク識別子
     *
     * @return        ネットワーク識別子が等しくない場合に true です。
     */
    inline bool operator != (const NetworkId& lhs,
                             const NetworkId& rhs) NN_NOEXCEPT
    {
        return std::memcmp(&lhs, &rhs, sizeof(NetworkId)) != 0;
    }

    //! @}

}} // end of namespace nn::ldn

namespace nn { namespace ldn { namespace detail
{
    // バイトオーダの変換です。
    IntentId ReverseByteOrderImpl(const IntentId& intentId) NN_NOEXCEPT;
    SessionId ReverseByteOrderImpl(const SessionId& sessionId) NN_NOEXCEPT;
    NetworkId ReverseByteOrderImpl(const NetworkId& networkId) NN_NOEXCEPT;

    // 文字列に変換します。
    size_t ConvertToString(const IntentId& intentId, char* buffer, size_t bufferSize) NN_NOEXCEPT;
    size_t ConvertToString(const SessionId& intentId, char* buffer, size_t bufferSize) NN_NOEXCEPT;
    size_t ConvertToString(const NetworkId& intentId, char* buffer, size_t bufferSize) NN_NOEXCEPT;

    // 文字列に変換された ID の文字数です。
    const size_t StringizedIntentIdLength = 35;
    const size_t StringizedSessionIdLength = 32;
    const size_t StringizedNetworkIdLength = StringizedIntentIdLength + 1 + StringizedSessionIdLength;

    // ローカル通信識別子の正当性を検証します。
    void GetDefaultLocalCommunicationId(Bit64* pOutId, Bit64 pid) NN_NOEXCEPT;
    bool HasPermission(Bit64 pid, Bit64 localCommunicationId) NN_NOEXCEPT;

}}} // namespace nn::ldn::detail
