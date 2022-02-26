/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
* @file
* @brief アニメーションオブジェクトに関するクラスの宣言
*/

#pragma once

#include <nn/g3d/g3d_AnimObj.h>

namespace nn { namespace g3d { namespace detail {

//! @brief 汎用アニメーションインスタンスの基底クラスです。
class GeneralAnimObj : public AnimObj
{
public:

    //----------------------------------------
    //! @name 関連付け
    //@{

    //! @brief アニメーション対象への関連付け状態を表すオブジェクトを取得します。
    //!
    //! @return アニメーション対象への関連付け状態を表すオブジェクトを返します。
    //!
    AnimBindTable& GetBindTable() NN_NOEXCEPT
    {
        return m_BindTable;
    }

    //! @brief アニメーション対象への関連付け状態を表すオブジェクトを取得します。
    //!
    //! @return アニメーション対象への関連付け状態を表すオブジェクトを返します。
    //!
    const AnimBindTable& GetBindTable() const NN_NOEXCEPT
    {
        return m_BindTable;
    }

    //! @brief アニメーション対象に関連付けられているかどうかを取得します。
    //!
    //! @return アニメーション対象に関連付けられている場合は true、関連付けられていない場合は false を返します。
    //!
    bool IsTargetBound() const NN_NOEXCEPT
    {
        return GetBindTable().IsTargetBound();
    }

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief 処理するアニメーションの個数を取得します。
    //!
    //! @return 処理するアニメーションの個数を返します。
    //!
    //! @details
    //! ボーンやマテリアルに対応するアニメーションの個数です。
    //!
    int GetAnimCount() const NN_NOEXCEPT
    {
        return m_BindTable.GetAnimCount();
    }

    //! @brief ターゲットの総数を取得します。
    //!
    //! @return ターゲットの総数を返します。
    //!
    //! @details
    //! アニメーション対象となりうるボーンやマテリアルの最大数です。
    //! この数を超えてアニメーションをバインドすることはできません。
    //!
    int GetTargetCount() const NN_NOEXCEPT
    {
        return m_BindTable.GetTargetCount();
    }

    //@}

protected:
    //! @briefprivate
    GeneralAnimObj() NN_NOEXCEPT
        : AnimObj()
        , m_BindTable()
    {
    }

    //! @briefprivate
    void SetTargetBound() NN_NOEXCEPT
    {
        GetBindTable().SetTargetBound();
        InvalidateContext(); // バインドし直した場合、前回評価のコンテクストは無効。
    }

    //! @briefprivate
    void SetTargetUnbound() NN_NOEXCEPT
    {
        GetBindTable().SetTargetUnbound();
    }

    //! @briefprivate
    void SetBindFlagImpl(int targetIndex, BindFlag flag) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(IsTargetBound() == true);
        int idxAnim = m_BindTable.GetAnimIndex(targetIndex);
        if (idxAnim != AnimBindTable::Flag_NotBound)
        {
            m_BindTable.SetBindFlag(idxAnim, static_cast<AnimBindTable::BindFlag>(flag));
        }
    }

    //! @briefprivate
    AnimObj::BindFlag GetBindFlagImpl(int targetIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(IsTargetBound() == true);
        int idxAnim = m_BindTable.GetAnimIndex(targetIndex);
        if (idxAnim != AnimBindTable::Flag_NotBound)
        {
            return static_cast<AnimObj::BindFlag>(m_BindTable.GetBindFlag(idxAnim));
        }
        else
        {
            return AnimObj::BindFlag_Disabled;
        }
    }

private:
    NN_DISALLOW_COPY(GeneralAnimObj) NN_NOEXCEPT;

    AnimBindTable m_BindTable;
};

}}} // namespace nn::g3d::detail

