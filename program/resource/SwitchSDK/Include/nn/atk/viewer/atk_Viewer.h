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

//---------------------------------------------------------------------------
//! @namespace nn::atk::viewer
//!
//! @brief    サウンドインゲーム編集ライブラリの名前空間です。 @n
//!           @n
//!           このライブラリを利用すると、ゲームで利用中のサウンドアーカイブの内容を
//!           SoundMaker から編集することができるようになります。 @n
//!           詳しい使い方については、各クラスの解説と edit デモを参照ください。
//!           @n
//!           【注意】 @n
//!           このライブラリには、β版のクラスが含まれています。 @n
//!           これらのクラスが提供する機能は、予告なく変更される可能性があります。
//---------------------------------------------------------------------------

#include <nn/atk/viewer/atk_SoundArchiveEditor.h>
#include <nn/atk/viewer/atk_SoundEditSession.h>
#include <nn/atk/viewer/atk_UpdateCachesHelper.h>

#include <nn/atk/viewer/atk_SoundControlSession.h>
#include <nn/atk/viewer/atk_SoundObjectController.h>
