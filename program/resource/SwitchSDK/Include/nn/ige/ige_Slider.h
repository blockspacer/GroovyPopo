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
#include <nn/ige/ige_Control.h>
#include <nn/ige/ige_ValueNode.h>

namespace nn { namespace ige {
class BufferWriter;

//! @briefprivate スライダーのオプションです。
class SliderOption : public ControlOption
{
};

//! @briefprivate  スライダーコントロールです。
template <typename T>
class Slider : public Control
{
    friend class ControlFactory;
    NN_DISALLOW_COPY(Slider);
public:
    typedef T ValueType;

    //! @briefprivate   スライダーに指定された最小値を取得します。
    //! @return         スライダーに指定された最小値を取得します。
    ValueType GetMin() NN_NOEXCEPT
    {
        return m_Min;
    }

    //! @briefprivate   スライダーに指定された最大値を取得します。
    //! @return         スライダーに指定された最大値を取得します。
    ValueType GetMax() NN_NOEXCEPT
    {
        return m_Max;
    }

    //! @briefprivate スライダーの現在の値をクライアントに送信します。
    void PushValue() NN_NOEXCEPT
    {
        m_Node.PushValue();
    }

private:
    Slider(T* pValue, T min, T max, SliderOption option) NN_NOEXCEPT;
    static void GenerateMetadata(BufferWriter* pWriter, const Node& node) NN_NOEXCEPT;

private:
    ValueNode<T> m_Node;
    T m_Min;
    T m_Max;
};

}} // namespace nn::ige
