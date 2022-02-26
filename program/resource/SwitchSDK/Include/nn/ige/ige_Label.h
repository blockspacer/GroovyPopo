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

namespace nn { namespace ige {
class BufferWriter;

//! @briefprivate  ラベルのオプションです。
class LabelOption : public ControlOption
{
};

//! @briefprivate  ラベルコントロールです。
class Label : public Control
{
    friend class ControlFactory;
    NN_DISALLOW_COPY(Label);

private:
    explicit Label(LabelOption option) NN_NOEXCEPT;
    static void GenerateMetadata(BufferWriter* pWriter, const Node& node) NN_NOEXCEPT;

private:
    Node m_Node;
};

}} // namespace nn::ige
