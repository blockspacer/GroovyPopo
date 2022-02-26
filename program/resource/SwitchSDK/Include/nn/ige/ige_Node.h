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

#include <nn/util/util_StringView.h>
#include <nn/ige/ige_Buffer.h>
#include <nn/ige/ige_BufferWriter.h>
#include <nn/ige/ige_NodeIdType.h>
#include <nn/ige/ige_NodeHolder.h>

namespace nn { namespace ige {
namespace detail { class NodeTargetMessage; }

class EditService;
class Node;
class NodeHolder;

//! @briefprivate   ノードの種類です。
enum NodeType : int32_t
{
    NodeType_Normal     = 0x00,    //!< 通常のノードです。
    NodeType_Command    = 0x01,    //!< コマンドの送受信が可能なノードです。
    NodeType_Value      = 0x02,    //!< 値の送受信が可能なノードです。
};

//! @briefprivate           メタデータを生成する関数の定義です。
//! @param[in]  pWriter     メタデータを書き込むライターです。
//! @param[in]  node        メタデータを生成するノードです。
typedef void(*MetadataFunction)(BufferWriter* pWriter, const Node& node);

//! @briefprivate   ノードのオプションを指定します。
//! @details        ノードを破棄するまで、引数に与えられた領域を保持する必要があります。
class NodeOption
{
public:
    //! @briefprivate  コンストラクタです。
    NodeOption()
        : pMetadataFunction(NULL)
        , pUserData(NULL)
        , enableAutoNotify(true)
    {
    }

public:
    MetadataFunction pMetadataFunction; //!< メタデータを生成する関数です。
    void* pUserData;                    //!< ユーザーデータです。
    bool enableAutoNotify;              //!< 接続時や子要素の追加時に、子要素の情報を自動的に通知するかどうかを指定します。
};

//!@briefprivate    ノードを作成する機能を持つクラスです。
class Node : public NodeHolder
{
    friend class EditService;
    friend class NodeHolder;
    NN_DISALLOW_COPY(Node);
public:
    Node() NN_NOEXCEPT;
    virtual ~Node() NN_NOEXCEPT NN_OVERRIDE;

    //! @briefprivate   同じ階層に含まれる次のノードを取得します。
    //! @return         同じ階層に含まれる次のノードを取得します。存在しない場合 NULL を返します。
    Node* GetNext() NN_NOEXCEPT
    {
        return m_pNext;
    }

    //! @briefprivate   同じ階層に含まれる次のノードを取得します。
    //! @return         同じ階層に含まれる次のノードを取得します。存在しない場合 NULL を返します。
    const Node* GetNext() const NN_NOEXCEPT
    {
        return m_pNext;
    }

    //! @briefprivate   親ノードを取得します。
    //! @return         親ノードを取得します。存在しない場合 NULL を返します。
    NodeHolder* GetParent() NN_NOEXCEPT
    {
        return m_pParent;
    }

    //! @briefprivate   親ノードを取得します。
    //! @return         親ノードを取得します。存在しない場合 NULL を返します。
    const NodeHolder* GetParent() const NN_NOEXCEPT
    {
        return m_pParent;
    }

    //! @briefprivate   ノードの種別を取得します。
    //! @return         ノードの種別を取得します。
    virtual NodeType GetNodeType() const NN_NOEXCEPT
    {
        return NodeType_Normal;
    }

    //! @briefprivate   このノードが他のノードに登録済みかどうかを取得します。
    //! @return         このノードが他のノードに登録済みかどうかを取得します。
    bool IsRegisterd() const NN_NOEXCEPT
    {
        return GetService() != NULL;
    }

    //! @briefprivate   ユーザーデータへのポインタを取得します。
    //! @return         ユーザーデータへのポインタを取得します。
    void* GetUserData() const NN_NOEXCEPT
    {
        return m_pUserData;
    }

    //! @briefprivate   ユーザーデータへのポインタを取得します。
    //! @return         ユーザーデータへのポインタを取得します。
    template<typename T>
    T* GetUserData() const NN_NOEXCEPT
    {
        return static_cast<T*>(m_pUserData);
    }

    //! @briefprivate   メタデータを通知します。
    //! @pre            IsRegisterd() == true
    void PushMetadata() NN_NOEXCEPT;

    //! @briefprivate   ノードの初期化処理を行います。
    //! @param[in]      option  ノードのオプションです。
    virtual void Initialize(NodeOption option) NN_NOEXCEPT;

    //! @briefprivate   ノードの終了処理を行います。
    virtual void Finalize() NN_NOEXCEPT
    {
        FinalizeNode();
    }

protected:
    virtual void PushInfo() NN_NOEXCEPT;
    virtual void OnMessageReceived(const detail::NodeTargetMessage& message) NN_NOEXCEPT;
    void WriteMetadata(BufferWriter* pWriter) NN_NOEXCEPT;
    NodeIdType GetParentId() const NN_NOEXCEPT;

private:
    void FinalizeNode() NN_NOEXCEPT;

private:
    Node* m_pNext;
    NodeHolder* m_pParent;
    void* m_pUserData;
    MetadataFunction m_pMetadataFunc;
};

}} // namespace nn::ige
