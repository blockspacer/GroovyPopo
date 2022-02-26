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
namespace ngc {

/**
 * @brief   言語ごとにおける不正文字列のパターンをリストにしたものの列挙子です。
 * @details それぞれのリストにはライブラリが正常にアプリケーションに組み込まれたかどうかを
 *          確認するためのテスト用の NG ワードが登録されています。
 *          各リストの具体的なテスト用の NG ワードは各列挙子の説明を参照してください。
 *          "badworda" は全リスト共通のテスト用の完全一致の NG ワードとして登録され、
 *          "warumoshi" , ”ワルモシ" , "qxzq" は全リスト共通のテスト用の部分一致の NG ワードとして登録されています。
 *          テスト用の NG ワードは完全一致の場合にのみ検出されます。
 */

enum ProfanityFilterPatternList
{
    ProfanityFilterPatternList_Japanese                 = 1 << 0, //!< 日本語のリストです。"badwordj"および"bbwjja"がテスト用の文字列として登録されています。
    ProfanityFilterPatternList_AmericanEnglish          = 1 << 1, //!< 米国英語のリストです。"badworde"および"bbween"がテスト用の文字列として登録されています。
    ProfanityFilterPatternList_CanadianFrench           = 1 << 2, //!< カナダフランス語のリストです。"badworde"および"bbwefr"がテスト用の文字列として登録されています。
    ProfanityFilterPatternList_LatinAmericanSpanish     = 1 << 3, //!< 中南米スペイン語のリストです。"badworde"および"bbwees"がテスト用の文字列として登録されています。
    ProfanityFilterPatternList_BritishEnglish           = 1 << 4, //!< 英国英語のリストです。"badwordp"および"bbwpen"がテスト用の文字列として登録されています。
    ProfanityFilterPatternList_French                   = 1 << 5, //!< フランス語のリストです。"badwordp"および"bbwpfr"がテスト用の文字列として登録されています。
    ProfanityFilterPatternList_German                   = 1 << 6, //!< ドイツ語のリストです。"badwordp"および"bbwpde"がテスト用の文字列として登録されています。
    ProfanityFilterPatternList_Italian                  = 1 << 7, //!< イタリア語のリストです。"badwordp"および"bbwpit"がテスト用の文字列として登録されています。
    ProfanityFilterPatternList_Spanish                  = 1 << 8, //!< スペイン語のリストです。"badwordp"および"bbwpes"がテスト用の文字列として登録されています。
    ProfanityFilterPatternList_Dutch                    = 1 << 9, //!< オランダ語のリストです。"badwordp"および"bbwpnl"がテスト用の文字列として登録されています。
    ProfanityFilterPatternList_Korean                   = 1 << 10,//!< 韓国語のリストです。"badwordk"および"bbwkko"がテスト用の文字列として登録されています。@n
    ProfanityFilterPatternList_SimplifiedChinese        = 1 << 11,//!< 簡体字中国語のリストです。"badwordc"および"bbwczh"がテスト用の文字列として登録されています。
    ProfanityFilterPatternList_Portuguese               = 1 << 12,//!< ポルトガル語のリストです。"badwordp"および"bbwppt"がテスト用の文字列として登録されています。
    ProfanityFilterPatternList_Russian                  = 1 << 13,//!< ロシア語のリストです。"badwordp"および"bbwpru"がテスト用の文字列として登録されています。
    ProfanityFilterPatternList_SouthAmericanPortuguese  = 1 << 14,//!< 南米ポルトガル語のリストです。"badworde"および"bbwept"がテスト用の文字列として登録されています。@n
                                                                  //!< NXの本体設定にはこの言語は言語設定として存在していません。
    ProfanityFilterPatternList_TraditionalChinese       = 1 << 15,//!< 繁体字中国語のリストです。"badwordt"および"bbwtzh"がテスト用の文字列として登録されています。

    // ※上記の数値は内部リソースと一致させる必要があるので、変更しないでください。

    ProfanityFilterPatternList_Max                      = 16      //!< パターンリストの個数を表します。
};

}   // ngc
}   // nn
