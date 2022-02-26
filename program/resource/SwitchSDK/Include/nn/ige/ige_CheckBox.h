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
#include <nn/ige/ige_Control.h>
#include <nn/ige/ige_ValueNode.h>

namespace nn { namespace ige {
class BufferWriter;

//! @briefprivate  チェックボックスのオプションです。
class CheckBoxOption : public ControlOption
{
};

//! @briefprivate  チェックボックスコントロールです。
class CheckBox : public Control
{
    friend class ControlFactory;
    NN_DISALLOW_COPY(CheckBox);
public:
    //! @briefprivate チェックボックスの現在の値をクライアントに送信します。
    void PushValue() NN_NOEXCEPT
    {
        m_Node.PushValue();
    }

private:
    CheckBox(bool* isChecked, CheckBoxOption option) NN_NOEXCEPT;
    static void GenerateMetadata(BufferWriter* pWriter, const Node& node) NN_NOEXCEPT;

private:
    ValueNode<bool> m_Node;
};

}} // namespace nn::ige
