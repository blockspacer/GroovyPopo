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
 *  @file
 *  @brief   言語設定に関する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>

namespace nn { namespace settings {

/**
 * @brief       言語を表す列挙体です。
 */
enum Language
{
    Language_Japanese,              //!< 日本語 (ja)
    Language_AmericanEnglish,       //!< 米国英語 (en-US)
    Language_French,                //!< フランス語 (fr)
    Language_German,                //!< ドイツ語 (de)
    Language_Italian,               //!< イタリア語 (it)
    Language_Spanish,               //!< スペイン語 (es)
    Language_Chinese,               //!< 中国語 (zh-CN)
    Language_Korean,                //!< 韓国語 (ko)
    Language_Dutch,                 //!< オランダ語 (nl)
    Language_Portuguese,            //!< ポルトガル語 (pt)
    Language_Russian,               //!< ロシア語 (ru)
    Language_Taiwanese,             //!< 台湾語 (zh-TW)
    Language_BritishEnglish,        //!< 英国英語 (en-GB)
    Language_CanadianFrench,        //!< カナダフランス語 (fr-CA)
    Language_LatinAmericanSpanish,  //!< 中南米スペイン語 (es-419)
    Language_SimplifiedChinese,     //!< 簡体字中国語 (zh-Hans)
    Language_TraditionalChinese,    //!< 繁体字中国語 (zh-Hant)
};

/**
 * @brief       言語コードを表す構造体です。
 */
struct LanguageCode
{
    char string[8]; //!< IETF 言語タグ形式の言語コード（NULL 終端）

//! @name コンストラクタ
//! @{

    /**
     * @brief       言語コードを生成します。
     *
     * @param[in]   value                       言語
     *
     * @return      指定された言語の言語コードを返します。
     *
     * @pre
     *              - value は有効な言語
     */
    static LanguageCode Make(Language value) NN_NOEXCEPT;

    /**
     * @brief       日本語 (ja) を表す言語コードを生成します。
     *
     * @return      日本語 (ja) を表す言語コードを返します。
     */
    static LanguageCode Japanese() NN_NOEXCEPT;

    /**
     * @brief       米国英語 (en-US) を表す言語コードを生成します。
     *
     * @return      米国英語 (en-US) を表す言語コードを返します。
     */
    static LanguageCode EnglishUs() NN_NOEXCEPT;

    /**
     * @brief       フランス語 (fr) を表す言語コードを生成します。
     *
     * @return      フランス語 (fr) を表す言語コードを返します。
     */
    static LanguageCode French() NN_NOEXCEPT;

    /**
     * @brief       ドイツ語 (de) を表す言語コードを生成します。
     *
     * @return      ドイツ語 (de) を表す言語コードを返します。
     */
    static LanguageCode German() NN_NOEXCEPT;

    /**
     * @brief       イタリア語 (it) を表す言語コードを生成します。
     *
     * @return      イタリア語 (it) を表す言語コードを返します。
     */
    static LanguageCode Italian() NN_NOEXCEPT;

    /**
     * @brief       スペイン語 (es) を表す言語コードを生成します。
     *
     * @return      スペイン語 (es) を表す言語コードを返します。
     */
    static LanguageCode Spanish() NN_NOEXCEPT;

    /**
     * @brief       中国語 (zh-CN) を表す言語コードを生成します。
     *
     * @return      中国語 (zh-CN) を表す言語コードを返します。
     *
     * @platformbegin{NX}
     *              当該プラットフォームでは未サポートです。
     *              @ref ChineseSimplified() を使用してください。
     * @platformend
     */
#if defined(NN_BUILD_TARGET_PLATFORM_NX)
    NN_DEPRECATED static LanguageCode Chinese() NN_NOEXCEPT;
#else
    static LanguageCode Chinese() NN_NOEXCEPT;
#endif

    /**
     * @brief       韓国語 (ko) を表す言語コードを生成します。
     *
     * @return      韓国語 (ko) を表す言語コードを返します。
     */
    static LanguageCode Korean() NN_NOEXCEPT;

    /**
     * @brief       オランダ語 (nl) を表す言語コードを生成します。
     *
     * @return      オランダ語 (nl) を表す言語コードを返します。
     */
    static LanguageCode Dutch() NN_NOEXCEPT;

    /**
     * @brief       ポルトガル語 (pt) を表す言語コードを生成します。
     *
     * @return      ポルトガル語 (pt) を表す言語コードを返します。
     */
    static LanguageCode Portuguese() NN_NOEXCEPT;

    /**
     * @brief       ロシア語 (ru) を表す言語コードを生成します。
     *
     * @return      ロシア語 (ru) を表す言語コードを返します。
     */
    static LanguageCode Russian() NN_NOEXCEPT;

    /**
     * @brief       台湾語 (zh-TW) を表す言語コードを生成します。
     *
     * @return      台湾語 (zh-TW) を表す言語コードを返します。
     *
     * @platformbegin{NX}
     *              当該プラットフォームでは未サポートです。
     *              @ref ChineseTraditional() を使用してください。
     * @platformend
     */
#if defined(NN_BUILD_TARGET_PLATFORM_NX)
    NN_DEPRECATED static LanguageCode Taiwanese() NN_NOEXCEPT;
#else
    static LanguageCode Taiwanese() NN_NOEXCEPT;
#endif

    /**
     * @brief       英国英語 (en-GB) を表す言語コードを生成します。
     *
     * @return      英国英語 (en-GB) を表す言語コードを返します。
     */
    static LanguageCode EnglishUk() NN_NOEXCEPT;

    /**
     * @brief       カナダフランス語 (fr-CA) を表す言語コードを生成します。
     *
     * @return      カナダフランス語 (fr-CA) を表す言語コードを返します。
     */
    static LanguageCode FrenchCa() NN_NOEXCEPT;

    /**
     * @brief       中南米スペイン語 (es-419) を表す言語コードを生成します。
     *
     * @return      中南米スペイン語 (es-419) を表す言語コードを返します。
     */
    static LanguageCode SpanishLatin() NN_NOEXCEPT;

    /**
     * @brief       簡体字中国語 (zh-Hans) を表す言語コードを生成します。
     *
     * @return      簡体字中国語 (zh-Hans) を表す言語コードを返します。
     */
    static LanguageCode ChineseSimplified() NN_NOEXCEPT;

    /**
     * @brief       繁体字中国語 (zh-Hant) を表す言語コードを生成します。
     *
     * @return      繁体字中国語 (zh-Hant) を表す言語コードを返します。
     */
    static LanguageCode ChineseTraditional() NN_NOEXCEPT;

//! @}

    /**
     * @brief       言語コードが一致しているか否かを判断します。
     *
     * @param[in]   lhs                         言語コード
     * @param[in]   rhs                         言語コード
     *
     * @return      一致しているなら true そうでなければ false を返します。
     */
    friend bool operator==(
        const LanguageCode& lhs, const LanguageCode& rhs) NN_NOEXCEPT;

    /**
     * @brief       言語コードが不一致か否かを判断します。
     *
     * @param[in]   lhs                         言語コード
     * @param[in]   rhs                         言語コード
     *
     * @return      不一致なら true そうでなければ false を返します。
     */
    friend bool operator!=(
        const LanguageCode& lhs, const LanguageCode& rhs) NN_NOEXCEPT;

    /**
     * @brief       言語が一致しているか否かを判断します。
     *
     * @param[in]   lhs                         言語
     * @param[in]   rhs                         言語コード
     *
     * @return      一致しているなら true そうでなければ false を返します。
     */
    friend bool operator==(
        const Language& lhs, const LanguageCode& rhs) NN_NOEXCEPT;

    /**
     * @brief       言語が一致しているか否かを判断します。
     *
     * @param[in]   lhs                         言語コード
     * @param[in]   rhs                         言語
     *
     * @return      一致しているなら true そうでなければ false を返します。
     */
    friend bool operator==(
        const LanguageCode& lhs, const Language& rhs) NN_NOEXCEPT;

    /**
     * @brief       言語が不一致か否かを判断します。
     *
     * @param[in]   lhs                         言語
     * @param[in]   rhs                         言語コード
     *
     * @return      不一致なら true そうでなければ false を返します。
     */
    friend bool operator!=(
        const Language& lhs, const LanguageCode& rhs) NN_NOEXCEPT;

    /**
     * @brief       言語が不一致か否かを判断します。
     *
     * @param[in]   lhs                         言語コード
     * @param[in]   rhs                         言語
     *
     * @return      不一致なら true そうでなければ false を返します。
     */
    friend bool operator!=(
        const LanguageCode& lhs, const Language& rhs) NN_NOEXCEPT;
};

//! @name 言語設定関連 API
//! @{

/**
 * @brief       利用可能な言語コードの数を返します。
 *
 * @details     言語設定として利用可能な言語コードの数を取得します。
 *              利用可能な言語コードについては GetAvailableLanguageCodes() を参照してください。
 *
 * @return      利用可能な言語コードの数を返します。
 */
int GetAvailableLanguageCodeCount() NN_NOEXCEPT;

/**
 * @brief       利用可能な言語コードの一覧を取得します。
 *
 * @details     言語設定として利用可能な言語コードの配列を取得します。
 *              nn::settings::Language で宣言された言語と対応する言語コードは常に配列に含まれます。
 *              また、言語コードの取得順も nn::settings::Language での宣言順に従います。
 *
 * @param[out]  outCodes                    言語コードを読み出すバッファ（配列）
 * @param[in]   count                       outCodes の数
 *
 * @return      取得した言語コードの数を返します。
 *
 * @pre
 *              - outCodes != nullptr
 * @post
 *              - i が 0 以上かつ戻り値未満ならば、 outCodes[i] は有効な言語コード
 */
int GetAvailableLanguageCodes(LanguageCode outCodes[], int count) NN_NOEXCEPT;

//! @}

}} // nn::settings
