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

/**
 *  @file
 *  @brief ランダムMii生成用の定義です。
 *  @details 
 */

#include <nn/mii/mii_Common.h>


namespace nn { namespace mii {

/**
* @brief ランダムMii生成用の記述子
* @details  ランダムMii生成の条件を指定する識別子です。
*/
class RandomDesc {
public:
    /**
     * @brief コンストラクタ
     * @details 
     */
    RandomDesc() NN_NOEXCEPT
    :m_Age(Age_All)
    ,m_Gender(Gender_All)
    ,m_Race(Race_All)
    {
    }

    /**
     * @brief コンストラクタ
     * @details 指定条件で初期化します。
     * @param[in] age 生成するMiiの年代
     * @param[in] gender 生成するMiiの性別
     * @param[in] race 生成するMiiの肌の色
     */
    explicit RandomDesc(Age age,Gender gender,Race race) NN_NOEXCEPT
    :m_Age(age)
    ,m_Gender(gender)
    ,m_Race(race)
    {
    }

    /**
     * @brief ランダム生成するMiiの年代を指定します。
     * @param[in] age 生成するMiiの年代
     */
    void SetAge(Age age) NN_NOEXCEPT
    { 
        m_Age = age;
    };

    /**
     * @brief ランダム生成するMiiの性別を指定します。
     * @param[in] gender 生成するMiiの性別
     */
    void SetGender(Gender gender) NN_NOEXCEPT
    { 
        m_Gender = gender; 
    };

    /**
     * @brief ランダム生成するMiiの肌の色を指定します。
     * @param[in] race 生成するMiiの肌の色
     */
    void SetRace(Race race) NN_NOEXCEPT
    { 
        m_Race = race;
    };

    /**
     * @brief Ageの取得
     * @return Age
     * @details 
     */
    Age GetAge() const NN_NOEXCEPT
    { 
        return m_Age;
    }

    /**
     * @brief Genderの取得
     * @return Gender
     * @details 
     */
    Gender GetGender() const NN_NOEXCEPT
    { 
        return m_Gender;
    }

    /**
     * @brief Raceの取得
     * @return Race
     * @details 
     */
    Race GetRace() const NN_NOEXCEPT
    { 
        return m_Race;
    }
private:
    Age    m_Age;
    Gender m_Gender;
    Race   m_Race;
};

}}
