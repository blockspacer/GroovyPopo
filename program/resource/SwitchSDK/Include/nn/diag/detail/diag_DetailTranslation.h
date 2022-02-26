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

#include <nn/nn_Common.h>
#include <nn/diag/detail/diag_DetailBuildSwitch.h>

#ifdef NN_BUILD_CONFIG_TOOLCHAIN_VC
#include "diag_DetailTranslation-compiler.vc.h"
#endif

/**
*   @brief      各モジュールの辞書データから、キーに対応する翻訳済デバッグ文字列を検索して返します。
*
*   @param[in]  searchFunction  辞書検索用の関数
*   @param[in]  key             検索キー (オリジナル言語のデバッグ文字列)
*   @return     翻訳済デバッグ文字列を返します。検索でヒットしなかった場合は key を返します。
*/
#ifdef NN_DETAIL_DIAG_ENABLE_TRANSLATION
#define NN_DETAIL_DIAG_SDK_TEXT(searchFunction, key) \
    searchFunction(NN_DETAIL_DIAG_SDK_TEXT_ENCODE(key))
#else
#define NN_DETAIL_DIAG_SDK_TEXT(searchFunction, key) \
    NN_DETAIL_DIAG_SDK_TEXT_ENCODE(key)
#endif

/*
* 引数の文字列をワイド文字列として扱い、マルチバイト文字列に変換するマクロです。
* Visual Studio でのコンパイル時のみ有効になります。
*/
#ifdef NN_BUILD_CONFIG_TOOLCHAIN_VC
#define NN_DETAIL_DIAG_SDK_TEXT_ENCODE(string) \
    (nn::diag::detail::GetSdkMultiByteStringFromWideString(L##string))
#else
#define NN_DETAIL_DIAG_SDK_TEXT_ENCODE(string) (string)
#endif

namespace nn { namespace diag { namespace detail {

/*
* @brief    デバッグ文字列翻訳に用いる辞書データを表現する型です。
*/
typedef const uint8_t* Dictionary;

/**
*   @brief      指定された辞書データから、キーに対応する翻訳済デバッグ文字列を検索して返します。
*
*   @param[in]  dictionary  デバッグ文字列翻訳に用いる辞書データ
*   @param[in]  key         検索キー (オリジナル言語のデバッグ文字列)
*   @return     翻訳済デバッグ文字列を返します。検索でヒットしなかった場合は key を返します。
*/
const char* Translate(Dictionary dictionary, const char* key) NN_NOEXCEPT;

}}}
