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

#include <nn/ui2d/ui2d_Pane.h>

namespace nn
{
namespace ui2d
{

//---------------------------------------------------------------------------
//! @brief シザーペインです。
//!
//---------------------------------------------------------------------------
class Scissor : public Pane
{
    typedef Pane Base;

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(Scissor::Base);

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    Scissor();

    //! @brief コンストラクタです。
    //!
    //! @details
    //! データブロックの設定を反映した状態で初期化します。
    //!
    //! @param[in] pBlock       ペインデータブロックへのポインタです。
    //! @param[in] buildArgSet  構築時の引数への参照です。
    //!
    Scissor(const ResScissor* pBlock, const BuildArgSet& buildArgSet);

    //! @brief コピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! @param[in] scissor コピー元のペインです。
    //!
    explicit Scissor(const Scissor& scissor)
        : Base(scissor)
    {
        // コピーするものは特になし。
    }

    //! @brief デストラクタです。
    //!
    virtual ~Scissor();

    //@}

    //! @brief ペイン自身と子ペインの描画処理を行います。
    //!
    //! @details
    //! Layout および親ペインから呼ばれます。
    //!
    //! DrawSelf() でペイン自身の描画を行い、次に子ペインの Draw() を
    //! 呼び出します。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    virtual void Draw(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer) NN_OVERRIDE;
private:

    //  ScissorStateInfo へ値を設定します。
    void SetScissorStateInfoValue(nn::gfx::ScissorStateInfo* pNewScissorStateInfo, float originX, float originY, float width, float height);
};

} // namespace nn::ui2d
} // namespace nn
