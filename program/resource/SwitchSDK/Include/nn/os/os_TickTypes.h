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
 * @brief   チックに関する型の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/nn_TimeSpan.h>
#include <nn/os/os_TickApi.h>

//----------------------------------------------------------------------------
//  C++ 向けの宣言
//----------------------------------------------------------------------------

namespace nn {

class   TimeSpan;

namespace os {

//----------------------------------------------------------------------------
/**
 * @brief  システムチックを扱う為のクラスです。
 *
 * @details
 *  システムチックは、システム依存の時間表現の一つであり、単調増加型の 64bit
 *  整数値です。@n
 *  システムチックの進む速さはターゲット環境や動作条件等によって異なりますが、
 *  その周波数（１秒間に進むカウント数）を取得する API が別途用意されています。
 *
 *  システムチックは、変換コンストラクタや変換演算子を使うことで、
 *  実際の自然時間を表す @ref nn::TimeSpan との相互変換が可能です。@n
 *  ただし、システムチックとシステムチック以外の時間が進む速度は異なって
 *  いるため、これらを混在して使用してはいけません。@n
 *  @ref nn::TimeSpan 等の同一の単位に値を変換することが出来たとしても、
 *  同時に時間を計って同じ値が得られるわけではありません。
 *
 *  例えば、内部で時間を扱うライブラリ（例：サウンドや動画の再生等）の中で
 *  扱われる時間とも進む速度が異なります。@n
 *  アプリケーションの演出とサウンド再生の同期を取りたい場合には、
 *  個々のライブラリの仕様を把握し、速度の一致する時間を使用してください。
 *
 */
class Tick
{
public:

//! @name コンストラクタ
//! @{

    //------------------------------------------------------------------------
    /**
     * @brief   チック値 0 で初期化するデフォルトコンストラクタです。
     *
     * @overloadlist{nn_os_Tick_Tick}
     */
    Tick() NN_NOEXCEPT : m_Tick(0) {}


    //------------------------------------------------------------------------
    /**
     * @brief   チック数を取ってオブジェクトを初期化するコンストラクタです。
     *
     * @param[in]   tick チック数
     *
     * @overloadlist{nn_os_Tick_Tick}
     */
    explicit Tick(int64_t tick) NN_NOEXCEPT : m_Tick(tick) {}

    //------------------------------------------------------------------------
    /**
     * @brief   TimeSpan を取ってオブジェクトを初期化するコンストラクタです。
     *
     * @param[in]   timeSpan 初期化する時間の値
     *
     * @overloadlist{nn_os_Tick_Tick}
     */
    explicit Tick(nn::TimeSpan timeSpan) NN_NOEXCEPT : m_Tick( ConvertToTick(timeSpan).GetInt64Value() ) {}

//! @}

//! @name 値の取得
//! @{

    //------------------------------------------------------------------------
    /**
     * @brief   64bit のチック値を返します。
     *
     * @return  自身のチック値を 64bit 整数値で返します。
     */
    int64_t GetInt64Value() const NN_NOEXCEPT
    {
        return m_Tick;
    }

//! @}

//! @name 変換
//! @{

    //------------------------------------------------------------------------
    /**
     * @brief   @ref nn::TimeSpan の値に変換します。
     *
     * @return  チックと同じ時間を表す @ref nn::TimeSpan オブジェクトを返します。
     */
    nn::TimeSpan ToTimeSpan() const NN_NOEXCEPT
    {
        return ConvertToTimeSpan( *this );
    }

//! @}

//! @name 和差
//! @{

    //----------------------------------------------------------------------
    /**
     * @brief   チック値を引きます。
     *
     * @param[in]   rhs 差し引くチック値
     * @return      自身へのリファレンスを返します。
     */
    //----------------------------------------------------------------------
    Tick& operator-=(Tick rhs) NN_NOEXCEPT;

    //----------------------------------------------------------------------
    /**
     * @brief   チック値の差を求めます。
     *
     * @param[in]   rhs 差を取るチック値
     * @return      自身と rhs との差と同じチック値を持つ Tick を返します。
     */
    //----------------------------------------------------------------------
    Tick operator-(Tick rhs) const NN_NOEXCEPT;

    //----------------------------------------------------------------------
    /**
     * @brief   チック値を加えます。
     *
     * @param[in]   rhs 加えるチック値
     * @return      自身へのリファレンスを返します。
     */
    //----------------------------------------------------------------------
    Tick& operator+=(Tick rhs) NN_NOEXCEPT;

    //----------------------------------------------------------------------
    /**
     * @brief   チック値の和を求めます。
     *
     * @param[in]   rhs 和を取るチック値
     * @return      自身と rhs との和と同じチック値を持つ Tick を返します。
     */
    //----------------------------------------------------------------------
    Tick operator+(Tick rhs) const NN_NOEXCEPT;

//! @}

    //----------------------------------------------------------------------
    /**
     * @brief   チックカウントが一致しているか否かを判断します。
     *
     * @param[in] lhs   比較するチックカウント
     * @param[in] rhs   比較するチックカウント
     *
     * @return  一致しているなら true そうでなければ false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator==(const Tick& lhs, const Tick& rhs) NN_NOEXCEPT
    {
        return lhs.m_Tick == rhs.m_Tick;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   チックカウントが不一致か否かを判断します。
     *
     * @param[in] lhs   比較するチックカウント
     * @param[in] rhs   比較するチックカウント
     *
     * @return  不一致なら true そうでなければ false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator!=(const Tick& lhs, const Tick& rhs) NN_NOEXCEPT
    {
        return !(lhs == rhs);
    }

    //----------------------------------------------------------------------
    /**
     * @brief   チックカウントが小さいかを判断します。
     *
     * @param[in] lhs   比較するチックカウント
     * @param[in] rhs   比較するチックカウント
     *
     * @return  lhs が rhs より小さいなら true そうでなければ false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator< (const Tick& lhs, const Tick& rhs) NN_NOEXCEPT
    {
        return lhs.m_Tick <  rhs.m_Tick;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   チックカウントが大きいかを判断します。
     *
     * @param[in] lhs   比較するチックカウント
     * @param[in] rhs   比較するチックカウント
     *
     * @return  lhs が rhs より大きいなら true そうでなければ false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator> (const Tick& lhs, const Tick& rhs) NN_NOEXCEPT
    {
        return rhs < lhs;
    }

    //----------------------------------------------------------------------
    /**
     * @brief   チックカウントが小さいまたは等しいかを判断します。
     *
     * @param[in] lhs   比較するチックカウント
     * @param[in] rhs   比較するチックカウント
     *
     * @return  lhs が rhs より小さいかまたは等しければ true そうでなければ false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator<=(const Tick& lhs, const Tick& rhs) NN_NOEXCEPT
    {
        return !(lhs > rhs);
    }

    //----------------------------------------------------------------------
    /**
     * @brief   チックカウントが大きいまたは等しいかを判断します。
     *
     * @param[in] lhs   比較するチックカウント
     * @param[in] rhs   比較するチックカウント
     *
     * @return  lhs が rhs より大きいかまたは等しければ true そうでなければ false を返します。
     */
    //----------------------------------------------------------------------
    friend bool operator>=(const Tick& lhs, const Tick& rhs) NN_NOEXCEPT
    {
        return !(lhs < rhs);
    }

private:
    int64_t m_Tick;
};

//----------------------------------------------------------------------------

inline Tick& Tick::operator-=(Tick rhs) NN_NOEXCEPT
{
    this->m_Tick -= rhs.m_Tick;
    return *this;
}

inline Tick  Tick::operator-(Tick rhs) const NN_NOEXCEPT
{
    Tick ret(*this);
    return ret -= rhs;
}

inline Tick& Tick::operator+=(Tick rhs) NN_NOEXCEPT
{
    this->m_Tick += rhs.m_Tick;
    return *this;
}

inline Tick  Tick::operator+(Tick rhs) const NN_NOEXCEPT
{
    Tick ret(*this);
    return ret += rhs;
}

//----------------------------------------------------------------------------

}}  // namespace nn::os


