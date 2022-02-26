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

#include <nn/ige/ige_Node.h>
#include <nn/ige/ige_ValueNodeTraits.h>

namespace nn { namespace ige {

//! @briefprivate  値を送受信する機能を持つノードの基底型です。
class ValueNodeBase : public Node
{
public:
    //! @briefprivate  現在の値をクライアントに送信します。
    void PushValue() NN_NOEXCEPT;

    //! @briefprivate   ノードの種類を取得します。
    //! @return         ノードの種類を取得します。
    virtual NodeType GetNodeType() const NN_NOEXCEPT NN_OVERRIDE
    {
        return NodeType_Value;
    }

    //! @briefprivate   値の型名を取得します。
    //! @return         値の型名を取得します。
    virtual util::string_view GetTypeName() const NN_NOEXCEPT = 0;

protected:
    virtual void Initialize(NodeOption option) NN_NOEXCEPT NN_OVERRIDE
    {
        Node::Initialize(option);
    }

    virtual void PushInfo() NN_NOEXCEPT NN_OVERRIDE;
    virtual void OnMessageReceived(const detail::NodeTargetMessage& data) NN_NOEXCEPT NN_OVERRIDE;

    virtual void Serialize(BufferWriter* pWriter) NN_NOEXCEPT = 0;
    virtual void Deserialize(const BufferView& buffer) NN_NOEXCEPT = 0;
};

//! @briefprivate   ノード特性に従い、単一の値を送受信するノードです。
//! @remarks        既定のノード特性によりサポートされる型は bool, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double です。
template<typename T, typename TValueNodeTraits = ValueNodeTraits<T>>
class ValueNode : public ValueNodeBase
{
public:
    //! @briefprivate  コンストラクタです。
    ValueNode() NN_NOEXCEPT
        : m_pValue(NULL)
    {
    }

    //! @briefprivate  デストラクタです。
    virtual ~ValueNode() NN_NOEXCEPT NN_OVERRIDE
    {
        FinalizeValueNode();
    }

    //! @briefprivate   ノードの初期化を行います。
    //! @param[in, out] pValue  値へのポインタです。
    //! @param[in]      option  オプションを指定します。
    void Initialize(T* pValue, NodeOption option) NN_NOEXCEPT
    {
        Initialize(option);
        m_pValue = pValue;
    }

    //! @briefprivate   ノードの終了処理を行います。
    virtual void Finalize() NN_NOEXCEPT NN_OVERRIDE
    {
        FinalizeValueNode();
        ValueNodeBase::Finalize();
    }

    //! @briefprivate   値の型名を取得します。
    //! @return         値の型名を取得します。
    virtual util::string_view GetTypeName() const NN_NOEXCEPT NN_OVERRIDE
    {
        return TValueNodeTraits::GetTypeName();
    }

protected:
    virtual void Serialize(BufferWriter* pWriter) NN_NOEXCEPT NN_OVERRIDE
    {
        TValueNodeTraits::Serialize(pWriter, m_pValue, *this);
    }

    virtual void Deserialize(const BufferView& buffer) NN_NOEXCEPT NN_OVERRIDE
    {
        TValueNodeTraits::Deserialize(buffer, m_pValue, *this);
    }

    virtual void Initialize(NodeOption option) NN_NOEXCEPT NN_OVERRIDE
    {
        ValueNodeBase::Initialize(option);
    }

private:
    void FinalizeValueNode() NN_NOEXCEPT
    {
        m_pValue = NULL;
    }

private:
    T* m_pValue;
};

//! @briefprivate   ノード特性に従い、配列を送受信するノードです。
//! @remarks        既定のノード特性によりサポートされる型は int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double です。
template<typename T, typename TValueArrayNodeTraits = ValueArrayNodeTraits<T>>
class ValueArrayNode : public ValueNodeBase
{
public:
    //! @briefprivate  コンストラクタです。
    ValueArrayNode() NN_NOEXCEPT
        : m_pValue(NULL)
        , m_Count(0)
    {
    }

    //! @briefprivate  デストラクタです。
    virtual ~ValueArrayNode() NN_NOEXCEPT NN_OVERRIDE
    {
        FinalizeValueArrayNode();
    }

    //! @briefprivate   ノードの初期化を行います。
    //! @param[in, out] pValue  値へのポインタです。
    //! @param[in]      count   配列の要素数を指定します。
    //! @param[in]      option  オプションを指定します。
    void Initialize(T* pValue, int count, NodeOption option) NN_NOEXCEPT
    {
        Initialize(option);
        m_pValue = pValue;
        m_Count = count;
    }

    //! @briefprivate   ノードの終了処理を行います。
    virtual void Finalize() NN_NOEXCEPT NN_OVERRIDE
    {
        FinalizeValueArrayNode();
        ValueNodeBase::Finalize();
    }

    //! @briefprivate   ノードの型名を取得します。
    //! @return         ノードの型名を取得します。
    virtual util::string_view GetTypeName() const NN_NOEXCEPT NN_OVERRIDE
    {
        return TValueArrayNodeTraits::GetTypeName();
    }

protected:
    virtual void Serialize(BufferWriter* pWriter) NN_NOEXCEPT NN_OVERRIDE
    {
        TValueArrayNodeTraits::Serialize(pWriter, m_pValue, m_Count, *this);
    }

    virtual void Deserialize(const BufferView& buffer) NN_NOEXCEPT NN_OVERRIDE
    {
        TValueArrayNodeTraits::Deserialize(buffer, m_pValue, m_Count, *this);
    }

    virtual void Initialize(NodeOption option) NN_NOEXCEPT NN_OVERRIDE
    {
        ValueNodeBase::Initialize(option);
    }

private:
    void FinalizeValueArrayNode() NN_NOEXCEPT
    {
        m_pValue = NULL;
        m_Count = 0;
    }

private:
    T* m_pValue;
    int m_Count;
};

//! @briefprivate   UTF-8 の文字列を送受信するノードです。
typedef ValueArrayNode<char, StringNodeTraits> StringNode;

}} // namespace nn::ige
