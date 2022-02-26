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
#include <nn/ige/ige_Control.h>
#include <nn/ige/ige_Button.h>
#include <nn/ige/ige_CheckBox.h>
#include <nn/ige/ige_Slider.h>
#include <nn/ige/ige_TextBox.h>
#include <nn/ige/ige_Label.h>

namespace nn { namespace ige {
namespace detail { class IgeAllocator; }

class UiService;
class GroupBoxOption;
class GroupBox;
class PanelOption;
class Panel;

//!@briefprivate    コントロールを作成する機能を持つクラスです。
class ControlFactory
{
    friend class UiService;
    NN_DISALLOW_COPY(ControlFactory);

public:
    //! @briefprivate       ボタンを作成します。
    //! @param[in]  option  オプションを指定します。
    //! @return             ボタンを返します。
    Button* CreateButton(ButtonOption option) NN_NOEXCEPT;

    //! @briefprivate           テキストボックスを作成します。
    //! @param[in, out] buffer  テキストバッファへのポインタです。
    //! @param[in]      size    テキストバッファのサイズを指定します。
    //! @param[in]      option  オプションを指定します。
    //! @return                 テキストボックスを返します。
    //! @pre            buffer != NULL
    //! @pre            0 < size
    TextBox* CreateTextBox(char* buffer, size_t size, TextBoxOption option) NN_NOEXCEPT;

    //! @briefprivate           ラベルを作成します。
    //! @param[in]      option  オプションを指定します。
    //! @return                 ラベルを返します。
    Label* CreateLabel(LabelOption option) NN_NOEXCEPT;

    //! @briefprivate           スライダーを作成します。
    //! @param[in, out] pValue  値へのポインタです。
    //! @param[in]      min     値の最小値です。
    //! @param[in]      max     値の最大値です。
    //! @param[in]      option  オプションを指定します。
    //! @return                 スライダーを返します。
    //! @pre            min <= max
    //! @pre            pValue != NULL
    Slider<int>* CreateSlider(int* pValue, int min, int max, SliderOption option) NN_NOEXCEPT;

    //! @briefprivate           スライダーを作成します。
    //! @param[in, out] pValue  値へのポインタです。
    //! @param[in]      min     値の最小値です。
    //! @param[in]      max     値の最大値です。
    //! @param[in]      option  オプションを指定します。
    //! @return                 スライダーを返します。
    //! @pre            min <= max
    //! @pre            pValue != NULL
    Slider<float>* CreateSlider(float* pValue, float min, float max, SliderOption option) NN_NOEXCEPT;

    //! @briefprivate               チェックボックスを作成します。
    //! @param[in, out] pIsChecked  値へのポインタです。
    //! @param[in]      option      オプションを指定します。
    //! @return                     チェックボックスを返します。
    //! @pre            pIsChecked != NULL
    CheckBox* CreateCheckBox(bool* pIsChecked, CheckBoxOption option) NN_NOEXCEPT;

    //! @briefprivate       グループボックスを作成します。
    //! @param[in]  option  オプションを指定します。
    //! @return             グループボックス内に表示するコントロールを生成可能なファクトリを返します。
    GroupBox* CreateGroupBox(GroupBoxOption option) NN_NOEXCEPT;

    //! @briefprivate       パネルを作成します。
    //! @param[in]  option  オプションを指定します。
    //! @return             パネル内に表示するコントロールを生成可能なファクトリを返します。
    Panel* CreatePanel(PanelOption option) NN_NOEXCEPT;

    //! @briefprivate           指定したコントロールが配下に存在するかどうかを返します。
    //! @param[in]  pControl    コントロールのポインタ
    //! @return                 指定のコントロールが配下に存在すれば true を返します。
    bool Contains(const Control* pControl) const NN_NOEXCEPT;

    //! @briefprivate           指定したコントロールが配下に存在する場合に削除します。
    //! @param[in]  pControl    削除するコントロールです。
    //! @return                 指定のコントロールが削除されれば true を返します。
    bool Delete(Control* pControl) NN_NOEXCEPT;

    //! @briefprivate       全ての子コントロールを削除します。
    void DeleteAll() NN_NOEXCEPT;

protected:
    ControlFactory()
        : m_pAllocator(NULL)
        , m_pParent(NULL)
    {
    }

    virtual ~ControlFactory() NN_NOEXCEPT
    {
        FinalizeFactory();
    }

    detail::IgeAllocator* GetAllocator() NN_NOEXCEPT
    {
        return m_pAllocator;
    }

    void InitializeFactory(detail::IgeAllocator* pAllocator, NodeHolder* m_pParent) NN_NOEXCEPT;
    void FinalizeFactory() NN_NOEXCEPT;

private:
    void RegisterNode(Node* pNode) NN_NOEXCEPT;

private:
    detail::IgeAllocator* m_pAllocator;
    NodeHolder* m_pParent;
};

}} // namespace nn::ige
