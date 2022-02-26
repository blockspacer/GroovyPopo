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

//! @namespace nn::ui2d
//!
//! @brief UI2Dライブラリとデフォルトコントロールの名前空間です。
//!
//! @details
//! コントロールは、レイアウトにユーザーインターフェース等の機能を持たせ
//! るための機能で、デフォルトコントロールは、そのレイアウトライブラリに
//! よる実装です。ライブラリユーザは、デフォルトコントロールを拡張したり、
//! あるいはまったく使用せずに独自のコントロールを実装したりすることが
//! できます。
//!
//! デフォルトコントロールのボタンは AnimButton クラスの派生クラス
//! として実装されています。
//!
//! レイアウトでデフォルトコントロールを使用するには、
//! ui2d::Layout::Build() の引数で DefaultControlCreator を指定します。
//!
//! @attention
//! 本名前空間の API を利用するには、
//! NintendoSDK NintendoWare パッケージをインストールする必要があります。

#include <nn/ui2d/ui2d_Alignment.h>
#include <nn/ui2d/ui2d_Animation.h>
#include <nn/ui2d/ui2d_Animator.h>
#include <nn/ui2d/ui2d_ArcExtractor.h>
#include <nn/ui2d/ui2d_ArcResourceAccessor.h>
#include <nn/ui2d/ui2d_Bounding.h>
#include <nn/ui2d/ui2d_Capture.h>
#include <nn/ui2d/ui2d_Common.h>
#include <nn/ui2d/ui2d_ControlCreator.h>
#include <nn/ui2d/ui2d_DrawInfo.h>
#include <nn/ui2d/ui2d_FontContainer.h>
#include <nn/ui2d/ui2d_GraphicsResource.h>
#include <nn/ui2d/ui2d_Group.h>
#include <nn/ui2d/ui2d_Init.h>
#include <nn/ui2d/ui2d_Layout.h>
#include <nn/ui2d/ui2d_Material.h>
#include <nn/ui2d/ui2d_MaterialHelper.h>
#include <nn/ui2d/ui2d_Pane.h>
#include <nn/ui2d/ui2d_PaneEffect.h>
#include <nn/ui2d/ui2d_Picture.h>
#include <nn/ui2d/ui2d_ProfileHelper.h>
#include <nn/ui2d/ui2d_ResourceAccessor.h>
#include <nn/ui2d/ui2d_Resources.h>
#include <nn/ui2d/ui2d_Scissor.h>
#include <nn/ui2d/ui2d_TexMap.h>
#include <nn/ui2d/ui2d_TextBox.h>
#include <nn/ui2d/ui2d_TextSearcher.h>
#include <nn/ui2d/ui2d_TextureContainer.h>
#include <nn/ui2d/ui2d_Types.h>
#include <nn/ui2d/ui2d_Util.h>
#include <nn/ui2d/ui2d_VectorGraphics.h>
#include <nn/ui2d/ui2d_Window.h>

#include <nn/ui2d/ui2d_ButtonBase.h>
#include <nn/ui2d/ui2d_ButtonGroup.h>
#include <nn/ui2d/ui2d_AnimButton.h>
#include <nn/ui2d/ui2d_DefaultControlCreator.h>
#include <nn/ui2d/ui2d_NormalButton.h>
#include <nn/ui2d/ui2d_DecisionButton.h>
#include <nn/ui2d/ui2d_CheckButton.h>
#include <nn/ui2d/ui2d_SelectButton.h>
#include <nn/ui2d/ui2d_TouchDragButton.h>
#include <nn/ui2d/ui2d_TouchOffButton.h>
#include <nn/ui2d/ui2d_TouchOffCheckButton.h>
#include <nn/ui2d/ui2d_DragButton.h>
