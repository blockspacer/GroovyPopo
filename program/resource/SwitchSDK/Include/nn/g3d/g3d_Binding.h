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

#include <nn/util/util_BitPack.h>
#include <nn/g3d/g3d_Configuration.h>

namespace nn { namespace g3d {

//! @brief バインドの結果を表すクラスです。
class BindResult
{
public:
    //! @brief コンストラクタです。
    BindResult() NN_NOEXCEPT
    {
        m_Flag.Clear();
    }

    //! @brief 関連付けの成功を表すビットをセットします。
    void SetSuccessBit() NN_NOEXCEPT
    {
        m_Flag.Set<SuccessBit>(true);
    }

    //! @brief 関連付けの失敗を表すビットをセットします。
    void SetFailureBit() NN_NOEXCEPT
    {
        m_Flag.Set<FailureBit>(true);
    }

    //! @brief 関連付けが完全に成功したかを調べます。
    //!
    //! @return 関連付けが完全に成功した場合は true それ以外の場合は false を返します。
    //!
    bool IsComplete() NN_NOEXCEPT
    {
        return m_Flag.IsAllBitOn(SuccessBit::Mask) && m_Flag.IsAllBitOff(FailureBit::Mask);
    }

    //! @brief 関連付けが完全に失敗したかを調べます。
    //!
    //! @return １つも関連付けが成功しなかった場合は true それ以外の場合は false を返します。
    //!
    bool IsMissed() NN_NOEXCEPT
    {
        return m_Flag.IsAllBitOff(SuccessBit::Mask) && m_Flag.IsAllBitOn(FailureBit::Mask);
    }

    //! @brief 関連付けが成功したかを調べます。
    //!
    //! @return １つでも関連付けに成功した場合は true それ以外の場合は false を返します。
    //!
    bool IsBound() NN_NOEXCEPT
    {
        return m_Flag.IsAllBitOn(SuccessBit::Mask);
    }

    //! @brief 関連付けが失敗したかを調べます。
    //!
    //! @return １つでも関連付けに失敗した場合は true それ以外の場合は false を返します。
    //!
    bool IsNotBound() NN_NOEXCEPT
    {
        return m_Flag.IsAllBitOn(FailureBit::Mask);
    }

    //! @brief      2つの BindResult の OR コピーを返します。
    //!
    //! @param[in]  other   OR を取る BindResult
    //!
    //! @return     2つの BindResult を OR したものを返します。
    //!
    //! @details    関数内で自動変数を確保して、値渡しでコピーを返します。
    //!
    BindResult operator|(const BindResult& other) const NN_NOEXCEPT
    {
        BindResult value = *this;
        value |= other;
        return value;
    }

    //! @brief      与えられた BindResult との OR を this に設定します。
    //!
    //! @param[in]  other   OR を取る BindResult
    //!
    //! @return     *this の参照を返します。
    //!
    BindResult& operator|=(const BindResult& other) NN_NOEXCEPT
    {
        m_Flag.storage |= other.m_Flag.storage;
        return *this;
    }

private:
    typedef nn::util::BitPack32::Field< 0, 1, bool >  SuccessBit; // 関連付け処理に一度でも成功した場合にセットされるビット示す型
    typedef nn::util::BitPack32::Field< 16, 1, bool > FailureBit; // 関連付け処理に一度でも失敗した場合にセットされるビット示す型

    nn::util::BitPack32 m_Flag;
};

}} // namespace nn::g3d

