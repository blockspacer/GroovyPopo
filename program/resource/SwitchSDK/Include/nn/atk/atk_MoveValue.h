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

namespace nn {
namespace atk {
namespace detail {

template< typename ValueType, typename CountType >
class MoveValue
{
public:
    /*--------------------------------------------------------------------------------*
      Name:         MoveValue

      Description:  コンストラクタ

      Arguments:    なし

      Returns:      なし
     *--------------------------------------------------------------------------------*/
    MoveValue() NN_NOEXCEPT : m_Origin(0), m_Target(0), m_Frame(0), m_Counter(0) {}

    /*--------------------------------------------------------------------------------*
      Name:         InitValue

      Description:  値を初期化

      Arguments:    value : 初期化値

      Returns:      なし
     *--------------------------------------------------------------------------------*/
    void InitValue( ValueType value ) NN_NOEXCEPT
    {
        m_Origin  = value;
        m_Target  = value;
        m_Frame   = 0;
        m_Counter = 0;
    }

    /*--------------------------------------------------------------------------------*
      Name:         SetTarget

      Description:  変化させる値の目的値を設定する

      Arguments:    targetValue : 目的値
                    frames : 変化に要するフレーム数

      Returns:      なし
     *--------------------------------------------------------------------------------*/
    void SetTarget( ValueType targetValue, CountType frames ) NN_NOEXCEPT
    {
        m_Origin  = GetValue();
        m_Target  = targetValue;
        m_Frame   = frames;
        m_Counter = 0;
    }

    /*--------------------------------------------------------------------------------*
      Name:         GetTarget

      Description:  変化させる値の目的値を取得する

      Arguments:    targetValue : 目的値
                    frames : 変化に要するフレーム数

      Returns:      なし
     *--------------------------------------------------------------------------------*/
    ValueType GetTarget() const NN_NOEXCEPT { return m_Target; }

    /*--------------------------------------------------------------------------------*
      Name:         GetValue

      Description:  現在の値を取得する

      Arguments:    なし

      Returns:      現在の値
     *--------------------------------------------------------------------------------*/
    ValueType GetValue() const NN_NOEXCEPT
    {
        if ( IsFinished() ) return m_Target;
        return static_cast<ValueType>(
            m_Origin + ( m_Target - m_Origin ) * m_Counter / m_Frame
        );
    }

    /*--------------------------------------------------------------------------------*
      Name:         Update

      Description:  値を更新する

      Arguments:    なし

      Returns:      なし
     *--------------------------------------------------------------------------------*/
    void Update() NN_NOEXCEPT { if ( m_Counter < m_Frame ) ++m_Counter; }

    /*--------------------------------------------------------------------------------*
      Name:         IsFinished

      Description:  カウンタが完了しているかどうかを返す

      Arguments:    なし

      Returns:      完了していたら true
     *--------------------------------------------------------------------------------*/
    bool IsFinished() const NN_NOEXCEPT { return m_Counter >= m_Frame; }

    /*--------------------------------------------------------------------------------*
      Name:         GetRemainingCount

      Description:  ターゲット値に達するまでの残りのUpdate回数を取得する

      Arguments:    なし

      Returns:      残りのUpdate回数
     *--------------------------------------------------------------------------------*/
    CountType GetRemainingCount() const NN_NOEXCEPT
    {
        if ( IsFinished() ) return static_cast<CountType>( 0 );
        return m_Frame - m_Counter;
    }

private:
    ValueType m_Origin;
    ValueType m_Target;
    CountType m_Frame;
    CountType m_Counter;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
