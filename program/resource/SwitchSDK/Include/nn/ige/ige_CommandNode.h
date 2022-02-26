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
#include <nn/nn_SdkAssert.h>

#include <nn/ige/ige_Node.h>

namespace nn { namespace ige {
class CommandNode;

//! @briefprivate           コマンドを受信したときに呼び出される関数です。
//! @param[in]  data        受信したコマンドの引数です。
//! @param[in]  node        コマンドを受信したノードです。
typedef void(*CommandReceivedFunction)(const BufferView& data, const CommandNode& node);

//! @briefprivate  コマンドの送受信を行うノードクラスです。
class CommandNode : public Node
{
    NN_DISALLOW_COPY(CommandNode);
public:
    //! @briefprivate   コンストラクタです。
    CommandNode() NN_NOEXCEPT;

    //! @briefprivate   デストラクタです。
    virtual ~CommandNode() NN_NOEXCEPT NN_OVERRIDE;

    //! @briefprivate       コマンドを発行します。
    //! @param[in]  data    発行するデータです。
    void PushCommand(const BufferView& data) NN_NOEXCEPT;

    //! @briefprivate   ノードの種類を取得します。
    //! @return         ノードの種類を取得します。
    virtual NodeType GetNodeType() const NN_NOEXCEPT NN_OVERRIDE
    {
        return NodeType_Command;
    }

    //! @briefprivate   コマンドの発行と受信が可能なノードを初期化します。
    //! @param[in]      pFunc   コマンドの受信時に呼び出される関数です。
    //! @param[in]      option  オプションです。
    void Initialize(CommandReceivedFunction pFunc, NodeOption option) NN_NOEXCEPT;

    //! @briefprivate   ノードの終了処理を行います。
    virtual void Finalize() NN_NOEXCEPT NN_OVERRIDE;

protected:
    virtual void OnMessageReceived(const detail::NodeTargetMessage& message) NN_NOEXCEPT NN_OVERRIDE;

    virtual void Initialize(NodeOption option) NN_NOEXCEPT NN_OVERRIDE
    {
        Node::Initialize(option);
    }

private:
    void FinalizeCommandNode() NN_NOEXCEPT;

private:
    CommandReceivedFunction m_pCommandReceivedFunc;
};

}} // namespace nn::ige
