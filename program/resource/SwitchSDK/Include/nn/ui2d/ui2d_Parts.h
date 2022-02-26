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

struct ResParts;
struct BuildResSet;
class DrawInfo;
class Layout;

//---------------------------------------------------------------------------
//! @brief 部品を表すペインです。
//!
//---------------------------------------------------------------------------
class Parts : public Pane
{
    typedef Pane Base;

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(Parts::Base);

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    Parts();

    //! @brief コンストラクタです。
    //!
    //! @details
    //! pBlock で指定したリソースからペインを生成します。
    //!
    //! @param[in] pBaseBlock       このペインが所属するレイアウトが保持しているリソースへのポインタです。
    //! @param[in] pOverrideBlock   上書きを行ったリソースへのポインタです。上書きが行われていないときはNULLになります。
    //! @param[in] buildArgSet      構築時の引数への参照です。
    //!
    Parts(
        const ResParts* pBaseBlock,
        const ResParts* pOverrideBlock,
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
    //! Partsのコピーコンストラクタでは、部品の下についているレイアウトはコピーされませんので、
    //! 部品を丸ごとコピーしたい場合は、Layoutのコピーコンストラクタを使用してください。
    //! Layoutのコピーコンストラクタで部品レイアウトをコピーすると、部品ペインもコピーされて、
    //! コピー先のレイアウトのルートペインになります。
    //!
    //! @param[in] parts    コピー元のペインです。
    //!
    explicit Parts(const Parts& parts);

    //! @brief デストラクタです。
    //!
    virtual ~Parts();

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief 部品のレイアウトを取得します。
    //!
    //! @return レイアウト を返します。
    //!
    Layout* GetLayout() const { return m_pLayout; }

    //! @brief レイアウトを設定します。
    //!
    //! @details
    //! 通常はライブラリ内部で使われるメソッドです。レイアウトのペインをカスタマイズしている場合に、
    //! レイアウトのコピー機能を再実装しなければならないようなケースでは使用する必要があります。
    //!
    //! @param[in] pLayout   部品として設定したいレイアウト
    //!
    void SetLayout(Layout* pLayout) { m_pLayout = pLayout; }

    //@}

    //----------------------------------------
    //! @name 検索
    //@{

    //! @brief 名前でペインを検索します。
    //!
    //! @details
    //! pFindName のペイン名を持つペインを検索します。
    //!
    //! @param[in] pFindName 検索するペイン名です。
    //!
    //! @return
    //! pFindName のペイン名を持つペインが見つかった場合はペインへのポインタを、
    //! 見つからなかった場合は NULL を返します。
    //!
    virtual Pane* FindPaneByNameRecursive(const char* pFindName) NN_OVERRIDE;
    //! @brief 名前でペインを検索します。
    //!
    //! @details
    //! pFindName のペイン名を持つペインを検索します。
    //!
    //! @param[in] pFindName 検索するペイン名です。
    //!
    //! @return
    //! pFindName のペイン名を持つペインが見つかった場合はペインへのポインタを、
    //! 見つからなかった場合は NULL を返します。
    //!
    virtual const Pane* FindPaneByNameRecursive(const char* pFindName) const NN_OVERRIDE;

    //! @brief 名前でマテリアルを検索します。
    //!
    //! @details
    //! pFindName のマテリアル名を持つマテリアルを検索します。
    //!
    //! @param[in] pFindName 検索するマテリアル名です。
    //!
    //! @return
    //! pFindName のマテリアル名を持つマテリアルが見つかった場合は
    //! マテリアルへのポインタを、見つからなかった場合は NULL を返します。
    //!
    virtual Material* FindMaterialByNameRecursive(const char* pFindName) NN_OVERRIDE;
    //! @brief 名前でマテリアルを検索します。
    //!
    //! @details
    //! pFindName のマテリアル名を持つマテリアルを検索します。
    //!
    //! @param[in] pFindName 検索するマテリアル名です。
    //!
    //! @return
    //! pFindName のマテリアル名を持つマテリアルが見つかった場合は
    //! マテリアルへのポインタを、見つからなかった場合は NULL を返します。
    //!
    virtual const Material* FindMaterialByNameRecursive(const char* pFindName) const NN_OVERRIDE;

    //@}

    //----------------------------------------

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
    bool CompareCopiedInstanceTest(const Parts& target) const;

    // 部品ペインとしてレイアウトが管理する際に用いるリンクです。

    //! @brief 内部用機能のため使用禁止です。
    nn::util::IntrusiveListNode m_Link;


private:

    Layout* m_pLayout;   // この部品に配置されたレイアウトへのポインタ

private:
    // コピー演算子を禁止します。
    const Parts& operator=( const Parts& );

};

} // namespace nn::ui2d
} // namespace nn
