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
#include <nn/ige/ige_ControlFactory.h>
#include <nn/ige/ige_Node.h>

namespace nn { namespace ige {
namespace detail { class IgeAllocator; }
class BufferWriter;

//! @briefprivate コントロールまたは要素の配置方向を表します。
enum Orientation
{
    Orientation_Horizontal  = 0,    //!< 水平方向に配置されます。
    Orientation_Vertical    = 1,    //!< 垂直方向に配置されます。
};

//! @briefprivate パネルの種別を表します。
enum PanelType
{
    PanelType_Stack = 0,    //!< 子要素を単一行に整列するパネルを使用します。
    PanelType_Wrap  = 1,    //!< 子要素を複数行にわたり折り返して表示するパネルを使用します。
};

//! @briefprivate パネルオプションです。
class PanelOption : public ControlOption
{
public:
    PanelOption() NN_NOEXCEPT
        : background(NULL)
        , panelType(PanelType_Stack)
        , orientation(Orientation_Vertical)
        , width(-1)
        , height(-1)
    {
    }

public:
    const char* background;     //!< パネルの背景色の名前を指定します。
    PanelType   panelType;      //!< パネルの種別を指定します。
    Orientation orientation;    //!< パネルの子要素の配置方向を指定します。
    int width;                  //!< パネルの幅を指定します。負値の場合、明示的に指定しないことを示します。
    int height;                 //!< パネルの高さを指定します。負値の場合、明示的に指定しないことを示します。
};

//! @briefprivate パネルコントロールです。
class Panel : public Control, public ControlFactory
{
    friend class ControlFactory;
    NN_DISALLOW_COPY(Panel);

public:
    //! @briefprivate   パネルの種別を取得します。
    //! @return         パネルの種別を取得します。
    PanelType GetPanelType() const NN_NOEXCEPT;

    //! @briefprivate   パネルの子要素の配置方向を取得します。
    //! @return         パネルの子要素の配置方向を取得します。
    Orientation GetOrientation() const NN_NOEXCEPT;

    //! @briefprivate   パネルの幅を取得します。負値の場合、明示的に指定されていないことを示します。
    //! @return         パネルの幅を取得します。
    int GetWidth() const NN_NOEXCEPT
    {
        return m_Width;
    }

    //! @briefprivate   パネルの幅を設定します。
    //! @param[in]      width   パネルの幅。負値の場合、明示的に指定しないことを示します。
    //! @pre    width < INT16_MAX
    void SetWidth(int width) NN_NOEXCEPT
    {
        m_Width = static_cast<int16_t>(width);
        m_Node.PushMetadata();
    }

    //! @briefprivate   パネルの高さを取得します。負値の場合、明示的に指定されていないことを示します。
    //! @return         パネルの高さを取得します。
    int GetHeight() const NN_NOEXCEPT
    {
        return m_Height;
    }

    //! @briefprivate   パネルの高さを設定します。
    //! @param[in]      height  パネルの高さ。負値の場合、明示的に指定しないことを示します。
    //! @pre    height < INT16_MAX
    void SetHeight(int height) NN_NOEXCEPT
    {
        m_Height = static_cast<int16_t>(height);
        m_Node.PushMetadata();
    }

    //! @briefprivate   パネルの背景色の名前を取得します。
    //! @return         パネルの背景色の名前を取得します。
    const char* GetBackgound() const NN_NOEXCEPT
    {
        return m_pBackground;
    }

private:
    Panel(detail::IgeAllocator* pAllocator, PanelOption option) NN_NOEXCEPT;
    virtual ~Panel() NN_NOEXCEPT NN_OVERRIDE;
    static void GenerateMetadata(BufferWriter* pWriter, const Node& node) NN_NOEXCEPT;

    void SetOrientation(Orientation orientation) NN_NOEXCEPT;
    void SetPanelType(PanelType panelType) NN_NOEXCEPT;

private:
    Node m_Node;

    const char* m_pBackground;
    util::BitPack32 m_Pack;
    int16_t m_Width;
    int16_t m_Height;
};

}} // namespace nn::ige
