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

#include <nn/vfx/vfx_Callback.h>

namespace nn {
namespace vfx {
namespace detail {

//---------------------------------------------------
//! @brief  範囲内ループを管理するクラスです。
//---------------------------------------------------
class AreaLoopSystem
{
    NN_DISALLOW_COPY( AreaLoopSystem );
public:
    typedef ResStripeSuper ResourceType;
    static const EmitterPluginId PluginId = EmitterPluginId_4;
    static const EmitterPluginCallbackIndex CallbackIndex = EmitterPluginCallbackIndex_4;

    //------------------------------------------------------------------------------
    //! @brief          エミッタ生成後コールバック
    //! @param[in] arg  コールバック引数
    //! @return         成功した場合true
    //------------------------------------------------------------------------------
    static bool EmitterInitializeCallback( EmitterInitializeArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          エミッタ描画コールバック
    //! @param[in] arg  コールバック引数
    //! @return         正常終了した場合trueを返します。
    //------------------------------------------------------------------------------
    static bool EmitterDrawCallback( EmitterDrawArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          RenderStateセットコールバック
    //! @param[in] arg  コールバック引数
    //! @return         正常終了した場合trueを返します。
    //------------------------------------------------------------------------------
    static bool RenderStateSetCallback( RenderStateSetArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief              初期化処理
    //! @param[in] pSystem   vfx::System クラスのポインタ
    //! @return             正常終了した場合trueを返します。
    //------------------------------------------------------------------------------
    static bool Initialize( System* pSystem ) NN_NOEXCEPT;

private:
    //------------------------------------------------------------------------------
    //! @brief          コンストラクタ
    //------------------------------------------------------------------------------
    AreaLoopSystem() NN_NOEXCEPT
    {
    }

    //------------------------------------------------------------------------------
    //! @brief          デストラクタ
    //------------------------------------------------------------------------------
    virtual ~AreaLoopSystem() NN_NOEXCEPT
    {
    }

    //------------------------------------------------------------------------------
    //! @brief      描画処理
    //! @param[in]  pCommandBuffer      コマンドバッファへのポインタ
    //! @param[in]  pEmitter            エミッタへのポインタ
    //! @param[in]  shaderType          シェーダタイプ
    //! @param[in]  pUserParam          ユーザーパラメータへのポインタ
    //! @param[in]  pDrawParameterArg   描画パラメータへのポインタ
    //! @return                         正常終了した場合trueを返します。
    //------------------------------------------------------------------------------
    static bool Draw( nn::gfx::CommandBuffer* pCommandBuffer, Emitter* pEmitter, ShaderType shaderType, void* pUserParam, DrawParameterArg* pDrawParameterArg ) NN_NOEXCEPT;
};

} // namespace detail
} // namespace vfx
} // namespace nn
