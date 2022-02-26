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

namespace nn { namespace ige {
namespace detail { class NodeIdManager; }

class EditService;
class Node;

//!@briefprivate    子要素としてノードを持つことを表すクラスです。
class NodeHolder
{
    friend class EditService;   // for ctor(EditService*)
    friend class Node;          // for GetId
    friend class detail::NodeIdManager; // for GetId

    NN_DISALLOW_COPY(NodeHolder);
public:
    virtual ~NodeHolder() NN_NOEXCEPT;

    //! @briefprivate       初期化済みのノードを子要素として登録します。
    //! @param[in]  pNode   登録するノードです。
    //! @pre                pNode != NULL
    //! @pre                IsRegisterd() == true
    //! @pre                pNode.IsRegisterd() == false
    void Register(Node* pNode) NN_NOEXCEPT;

    //! @briefprivate       子ノードの登録を解除します。
    //! @param[in]  pNode   登録を解除する子ノードです。
    //! @pre                pNode != NULL
    //! @pre                IsRegisterd() == true
    void Unregister(Node* pNode) NN_NOEXCEPT;

    //! @briefprivate   全ての子ノードの登録を解除します。
    void UnregisterAll() NN_NOEXCEPT;

    //! @briefprivate   先頭の子ノードを取得します。
    //! @return         先頭の子ノードを取得します。子ノードが存在しない場合 NULL を返します。
    Node* GetChild() NN_NOEXCEPT
    {
        return m_pChild;
    }

    //! @briefprivate   先頭の子ノードを取得します。
    //! @return         先頭の子ノードを取得します。子ノードが存在しない場合 NULL を返します。
    const Node* GetChild() const NN_NOEXCEPT
    {
        return m_pChild;
    }

protected:
    NodeHolder() NN_NOEXCEPT;

    void PushChildInfo() NN_NOEXCEPT;

    EditService* GetService() NN_NOEXCEPT
    {
        return m_pService;
    }

    const EditService* GetService() const NN_NOEXCEPT
    {
        return m_pService;
    }

    bool IsEnabledAutoNotify() const NN_NOEXCEPT
    {
        return m_IsEnabledAutoNotify;
    }

    void SetEnableAutoNotify(bool value) NN_NOEXCEPT
    {
        m_IsEnabledAutoNotify = value;
    }

    NodeIdType GetId() const NN_NOEXCEPT
    {
        return m_Id;
    }

private:
    explicit NodeHolder(EditService* pService) NN_NOEXCEPT; // for EditService

private:
    EditService* m_pService;
    Node* m_pChild;
    NodeIdType m_Id;
    bool m_IsEnabledAutoNotify;
};

}} // namespace nn::ige
