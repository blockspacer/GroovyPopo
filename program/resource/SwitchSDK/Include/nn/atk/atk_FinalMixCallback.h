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

#include <nn/util/util_IntrusiveList.h>
#include <nn/atk/atk_Global.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @briefprivate
//! @brief  SDKのUserFinalMixCallbackを複数扱うためのインターフェイスです。
//!
#if 0 // 参考用 : Cafe 実装
//!         再生しているサウンドの波形情報を取得したい場合、このクラスを派生して@ref OnFinalMix に
//!         必要な処理を記述し、派生したクラスをサウンドライブラリ内部で管理しているリストに登録します。
//!
//!         @ref SoundSystem::AppendFinalMixCallback や @ref SoundSystem::PrependFinalMixCallback
//!         でサウンドライブラリ内部で管理しているリストにFinalMixCallbackを登録することができ、
//!         追加されたFinalMixCallbackはAXUserFinalMixCallbackが実行されるときに、リストの先頭から順に呼び出されます。
//!
//!         FinalMixCallbackで得られる波形情報は加工することができるため、リストに登録する順番によって、
//!         出力が変わる可能性がある点に注意してください。
//!
//!         FinalMixCallbackで加工した波形がどのタイミングで処理されているかについては、
//!         Cafe SDKのAXレンダリングモデルについてのドキュメント
//!         $CAFE_ROOT/system/docs/man/en_us/sound/overview/rendering.html\#mixing
//!         を参照してください。
#endif
//!
//! @see SoundSystem::AppendFinalMixCallback
//! @see SoundSystem::PrependFinalMixCallback
//! @see SoundSystem::EraseFinalMixCallback
//---------------------------------------------------------------------------
class FinalMixCallback
{
public:
    util::IntrusiveListNode m_Link;    //!< @internal

    //---------------------------------------------------------------------------
    //! @brief  デストラクタです。
    //---------------------------------------------------------------------------
    virtual ~FinalMixCallback() NN_NOEXCEPT {}

    //---------------------------------------------------------------------------
    //! @brief  出力波形が更新される度に行う処理を記述します。
    //!
    //! @param device どのデバイスの波形情報であるのかを示すパラメータです。
    //! @param data 波形情報を格納した構造体です。
    //!
    //! @see FinalMixData 構造体
    //---------------------------------------------------------------------------
    virtual void OnFinalMix(OutputDevice device, const FinalMixData* data) NN_NOEXCEPT = 0;
};

/* ========================================================================
        非公開
   ======================================================================== */

namespace detail {

class ReadOnlyFinalMixCallback
{
public:
    util::IntrusiveListNode m_Link;

    virtual ~ReadOnlyFinalMixCallback() NN_NOEXCEPT {}
    virtual void OnFinalMix(OutputDevice device, const FinalMixData* data) NN_NOEXCEPT { (void)device; (void)data; }
};

} // namespace nn::atk::detail

} // namespace nn::atk
} // namespace nn
