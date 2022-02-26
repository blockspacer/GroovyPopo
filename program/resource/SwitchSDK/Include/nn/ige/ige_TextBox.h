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

#include <nn/util/util_BitPack.h>
#include <nn/ige/ige_Control.h>
#include <nn/ige/ige_ValueNode.h>

namespace nn { namespace ige {
class BufferWriter;

//! @briefprivate  テキストボックスのオプションです。
class TextBoxOption : public ControlOption
{

public:
    TextBoxOption() NN_NOEXCEPT
        : isFileDropEnabled(false)
    {
    }

public:
    bool isFileDropEnabled;      //!< ファイルドロップの有効無効を指定します。
};

//! @briefprivate  テキストボックスコントロールです。
class TextBox : public Control
{
    friend class ControlFactory;
    NN_DISALLOW_COPY(TextBox);

public:
    //! @briefprivate   ファイルドロップの有効無効の取得します。
    //! @return         ファイルドロップの有効無効を取得します。
    bool     IsFileDropEnabled() const NN_NOEXCEPT;

    //! @briefprivate テキストボックスの現在の値をクライアントに送信します。
    void PushValue() NN_NOEXCEPT
    {
        m_Node.PushValue();
    }

    //! @briefprivate   テキストバッファのサイズを取得します。
    //! @return         テキストバッファのサイズを返します。
    size_t GetSize() const NN_NOEXCEPT
    {
        return m_Size;
    }

private:
    TextBox(char* buffer, size_t size, TextBoxOption option) NN_NOEXCEPT;
    static void GenerateMetadata(BufferWriter* pWriter, const Node& node) NN_NOEXCEPT;

    void SetFileDrop(bool isFileDropEnabled) NN_NOEXCEPT;

private:
    StringNode m_Node;

    size_t m_Size;
    util::BitPack32 m_Pack;
};

}} // namespace nn::ige
