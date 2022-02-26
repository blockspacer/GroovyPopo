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
 * @brief Eコマース処理を行うAPIに必要な型や定数などの定義
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

#include <nn/nn_ApplicationId.h>
#include <nn/account/account_Types.h>

namespace nn { namespace ec {

/**
 * @brief ショップ上の商品の個別ページを一意に識別するIDです。
 *
 * @details 開発環境や製品環境など環境により異なるIDが割り振られるため、必ずサーバから取得した値を利用してください。
 *          @ref nn::ec::MakeNsUid によって文字列表現から NsUid を生成することが出来ます。
 */
struct NsUid
{
    Bit64 value;
    /**
     * @brief 無効であることを表す NsUid 値を返します。
     */
    static NsUid GetInvalidId() NN_NOEXCEPT
    {
        NsUid id = { 0 };
        return id;
    }
};

inline bool operator == (const NsUid& lhs, const NsUid& rhs) NN_NOEXCEPT
{
    return lhs.value == rhs.value;
}

inline bool operator != (const NsUid& lhs, const NsUid& rhs) NN_NOEXCEPT
{
    return lhs.value != rhs.value;
}

/**
 * @brief 期間型サービスのコースを識別する ID の文字列の長さです。
 */
const size_t MaxCourseIdStringLength = 16;

/**
 * @brief 期間型サービスのコースを識別する ID です。
 */
struct CourseId
{
    char value[MaxCourseIdStringLength + 1]; //!< コースを識別する ID の値
};

/**
 * @brief 消費型アイテムカテゴリを識別する ID の文字列の長さです。
 */
const size_t MaxConsumableIdStringLength = 16;

/**
 * @brief 消費型アイテムカテゴリを識別する ID です。
 */
struct ConsumableId
{
    char value[MaxConsumableIdStringLength + 1]; //!< 消費型アイテムカテゴリを識別する ID の値
};
}}
