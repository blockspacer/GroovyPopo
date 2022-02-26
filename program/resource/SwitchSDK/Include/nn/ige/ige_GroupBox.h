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
#include <nn/ige/ige_ControlFactory.h>
#include <nn/ige/ige_Node.h>

namespace nn { namespace ige {
namespace detail { class IgeAllocator; }
class BufferWriter;

//! @briefprivate グループボックスのオプションです。
class GroupBoxOption : public ControlOption
{
};

//! @briefprivate グループボックスコントロールです。
class GroupBox : public Control, public ControlFactory
{
    friend class ControlFactory;
    NN_DISALLOW_COPY(GroupBox);

private:
    GroupBox(detail::IgeAllocator* pAllocator, GroupBoxOption option) NN_NOEXCEPT;
    virtual ~GroupBox() NN_NOEXCEPT NN_OVERRIDE;
    static void GenerateMetadata(BufferWriter* pWriter, const Node& node) NN_NOEXCEPT;

private:
    Node m_Node;
};

}} // namespace nn::ige
