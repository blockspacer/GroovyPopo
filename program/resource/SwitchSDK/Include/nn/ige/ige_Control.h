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
#include <nn/util/util_StringView.h>
#include <nn/ige/ige_BufferWriter.h>
#include <nn/ige/ige_Node.h>

namespace nn { namespace ige {
class ControlFactory;

//! @briefprivate   コントロール共通のオプションです。
class ControlOption
{
public:
    ControlOption() NN_NOEXCEPT
        : header(NULL)
        , tooltip(NULL)
    {
    }

public:
    const char* header;     //!< ヘッダーです。
    const char* tooltip;    //!< ツールチップです。
};

//! @briefprivate  コントロールの基底クラスです。
class Control
{
    friend class ControlFactory;
    NN_DISALLOW_COPY(Control);
public:

    //! @briefprivate   ヘッダーを取得します。
    //! @return         ヘッダーを返します。
    const char* GetHeader() const NN_NOEXCEPT
    {
        return m_Header;
    }

    //! @briefprivate       ヘッダーを設定します。
    //! @param[in]  header  ヘッダー文字列。null終端されている必要があります。
    void SetHeader(const char* header) NN_NOEXCEPT
    {
        m_Header = header;
        m_pNode->PushMetadata();
    }

    //! @briefprivate   ツールチップを取得します。
    //! @return         ツールチップを返します。
    const char* GetTooltip() const NN_NOEXCEPT
    {
        return m_Tooltip;
    }

    //! @briefprivate       ツールチップを設定します。
    //! @param[in]  tooltip ツールチップ。null終端されている必要があります。
    void SetTooltip(const char* tooltip)
    {
        m_Tooltip = tooltip;
        m_pNode->PushMetadata();
    }

    //! @briefprivate   コントロールが有効かどうかを取得します。
    //! @return         コントロールが有効かどうかを取得します。
    bool IsEnabled() const NN_NOEXCEPT
    {
        return m_IsEnabled;
    }

    //! @briefprivate       コントロールの有効状態を設定します。
    //! @param[in]  value   コントロールが有効かどうか。
    void SetEnabled(bool value) NN_NOEXCEPT
    {
        if (m_IsEnabled != value)
        {
            m_IsEnabled = value;
            m_pNode->PushMetadata();
        }
    }

protected:
    Control(Node* pNode, ControlOption option) NN_NOEXCEPT
        : m_pNode(pNode)
        , m_Header(option.header)
        , m_Tooltip(option.tooltip)
        , m_IsEnabled(true)
    {
        NN_SDK_REQUIRES_NOT_NULL(pNode);
    }

    virtual ~Control() NN_NOEXCEPT
    {
    }

    static void WriteAttribute(BufferWriter* pWriter, util::string_view name, util::string_view value)
    {
        if (name.empty() || value.empty())
        {
            return;
        }

        pWriter->Write(name.data(), 0, name.size());    // null終端回避
        pWriter->Write(':');                            // separator。属性名に ':' を含まないこと
        pWriter->Write(value);                          // null終端
    }

    void WriteCommonMetadata(BufferWriter* pWriter) const NN_NOEXCEPT
    {
        WriteAttribute(pWriter, "header", m_Header);
        WriteAttribute(pWriter, "tooltip", m_Tooltip);
        WriteAttribute(pWriter, "enabled", m_IsEnabled ? "true" : "false");
    }

private:
    Node* m_pNode;
    const char* m_Header;
    const char* m_Tooltip;
    bool m_IsEnabled;
};

}} // namespace nn::ige
