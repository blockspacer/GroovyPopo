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
//! @brief 整列ペインです。
//!
//---------------------------------------------------------------------------
class Alignment : public Pane
{
    typedef Pane Base;

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(Alignment::Base);

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    Alignment();

    //! @brief コンストラクタです。
    //!
    //! @details
    //! データブロックの設定を反映した状態で初期化します。
    //!
    //! @param[in] pBlock       ペインデータブロックへのポインタです。
    //! @param[in] buildArgSet  構築時の引数への参照です。
    //!
    Alignment(const ResAlignment* pBlock, const BuildArgSet& buildArgSet);

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
    //! @param[in] alignment コピー元のペインです。
    //!
    explicit Alignment(const Alignment& alignment);

    //! @brief デストラクタです。
    //!
    virtual ~Alignment();

    //@}

    //! @brief 描画のための行列を計算します。
    //!
    //! @param[in] drawInfo         DrawInfoです。
    //! @param[in] context          描画情報と内部で使用する変数を含むコンテキストです。
    //! @param[in] isDirtyParentMtx 親の行列がDirtyか否かです。
    //!
    virtual void Calculate(DrawInfo& drawInfo, Pane::CalculateContext& context, bool isDirtyParentMtx) NN_OVERRIDE;

    //! @brief デフォルトマージンを取得します。
    //!
    //! @return デフォルトマージンを返します。
    //!
    float GetDefaultMargin() const;

    //! @brief 水平方向の配置設定を取得します。
    //!
    //! @return 水平方向の配置設定を返します。
    //!
    //! @details IsHorizontalAlignmentメソッドで水平の場合に利用出来ます。
    //!
    //! @sa IsHorizontalAlignment
    //!
    int GetHorizontalAlignment() const;

    //! @brief 垂直方向の配置設定を取得します。
    //!
    //! @return 垂直方向の配置設定を返します。
    //!
    //! @details IsVerticalAlignmentメソッドで垂直の場合に利用出来ます。
    //!
    //! @sa IsVerticalAlignment
    //!
    int GetVerticalAlignment() const;

    //! @brief 整列ペインの配下を再計算させるフラグを設定します。
    //!
    void RequestAlignment();

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
    bool CompareCopiedInstanceTest(const Alignment& target) const;

    //! @brief 整列方向が水平かどうかを判定します。
    //!
    //! @return 整列方向が水平の場合は true を返します。
    //!
    bool IsHorizontalAlignment() const;

    //! @brief 整列方向が垂直かどうかを判定します。
    //!
    //! @return 整列方向が垂直の場合は true を返します。
    //!
    bool IsVerticalAlignment() const;

private:
    enum Flags
    {
        Flags_RecalculationRequest,
        Flags_AlignmentDirection,
        Flags_Max
    };

    union
    {
        //! 水平方向の配置設定です。
        int m_HorizontalAlignment;

        //! 垂直方向の配置設定です。
        int m_VerticalAlignment;
    };

    //! デフォルトマージンです。子ペインの二つ目以降の間隔に利用します。
    float m_DefaultMergine;

    //! 端のペインを延長します。
    bool m_IsExtendEdgeEnabled;

    //! 整列ペイン用のフラグです。
    uint8_t  m_Flag;

    void MakeAlignment();
    void MakeHorizontalAlignment();
    void MakeVerticalAlignment();
    void MakeForwardHorizontalAlignment();
    void MakeReverseHorizontalAlignment();
    void MakeForwardVerticalAlignment();
    void MakeReverseVerticalAlignment();

    // コピー演算子を禁止します。
    const Alignment& operator=( const Alignment& );
};

} // namespace nn::ui2d
} // namespace nn
