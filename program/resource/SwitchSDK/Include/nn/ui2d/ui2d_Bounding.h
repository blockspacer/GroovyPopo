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

#include <nn/ui2d/ui2d_Util.h>

#include <nn/ui2d/ui2d_Pane.h>

namespace nn
{
namespace ui2d
{

struct ResBounding;
struct BuildResSet;
class DrawInfo;

//---------------------------------------------------------------------------
//! @brief 境界を表すペインです。
//!
//---------------------------------------------------------------------------
class Bounding : public Pane
{
    typedef Pane Base;

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(Bounding::Base);

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    Bounding()
    {}

    //! @brief コンストラクタです。
    //!
    //! @details
    //! size で指定したサイズのペインを生成します。
    //!
    //! @param[in] size ペインのサイズです。
    //!
    explicit Bounding(const Size& size)
    {
        SetSize(size);
    }

    //! @brief コンストラクタです。
    //!
    //! @details
    //! pBlock で指定したリソースからペインを生成します。
    //!
    //! @param[in] pBaseBlock       このペインが所属するレイアウトが保持しているリソースへのポインタです。
    //! @param[in] pOverrideBlock   上書きを行ったリソースへのポインタです。上書きが行われていないときはNULLになります。
    //! @param[in] buildArgSet      構築時の引数への参照です。
    //!
    Bounding(
        const ResBounding* pBaseBlock,
        const ResBounding* pOverrideBlock,
        const BuildArgSet& buildArgSet);

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
    //! @param[in] bounding コピー元のペインです。
    //!
    explicit Bounding(const Bounding& bounding);

    //! @brief デストラクタです。
    //!
    virtual ~Bounding();

    //@}

    //! @brief コピーコンストラクタでコピーされたインスタンスが正しくコピーされているか確認します。
    //!
    //! @details
    //! このメソッドは ui2d ランタイムのテストのために作成されたメソッドです。
    //! アプリケーションコードでの使用は非推奨となります。
    //!
    //! @param[in]  target  比較対象のペインです。
    //!
    //! @return 同等とみなせる場合は true を返します。
    //!
    bool CompareCopiedInstanceTest(const Bounding& target) const;

protected:

    //! @brief 自身の描画処理を行います。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    virtual void DrawSelf(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer) NN_OVERRIDE;

private:
    // コピー演算子を禁止します。
    const Bounding& operator=( const Bounding& );
};

} // namespace nn::ui2d
} // namespace nn
