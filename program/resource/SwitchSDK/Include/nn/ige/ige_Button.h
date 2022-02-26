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
#include <nn/ige/ige_CommandNode.h>

namespace nn { namespace ige {
class BufferWriter;

//!< ボタンのクリック時に実行されるコールバック関数です。
typedef void(*ClickCallback)(void* pUserData);

//! @briefprivate  ボタンのオプションです。
class ButtonOption: public ControlOption
{
public:
    ButtonOption() NN_NOEXCEPT
        : pClickCallback(NULL)
        , pUserData(NULL)
    {
    }

public:
    ClickCallback pClickCallback;   //!< ボタンのクリック時に実行されるコールバック関数です。
    void* pUserData;                //!< コールバックに与えられるユーザーデータへのポインタです。
};

//! @briefprivate  ボタンコントロールです。
class Button : public Control
{
    friend class ControlFactory;
    NN_DISALLOW_COPY(Button);
public:
    //! @briefprivate   ユーザーデータへのポインタを取得します。
    //! @return         ユーザーデータへのポインタを返します。
    void* GetUserData() const NN_NOEXCEPT
    {
        return m_pUserData;
    }

    //! @briefprivate   指定した型でユーザーデータへのポインタを取得します。
    //! @return         ユーザーデータへのポインタを返します。
    template <typename T>
    T* GetUserData() const NN_NOEXCEPT
    {
        return static_cast<T*>(m_pUserData);
    }

private:
    explicit Button(ButtonOption option) NN_NOEXCEPT;
    static void OnClicked(const BufferView&, const CommandNode& node) NN_NOEXCEPT;
    static void GenerateMetadata(BufferWriter* pWriter, const Node& node) NN_NOEXCEPT;

private:
    ClickCallback m_pCallback;
    void* m_pUserData;
    CommandNode m_Node;
};

}} // namespace nn::ige
