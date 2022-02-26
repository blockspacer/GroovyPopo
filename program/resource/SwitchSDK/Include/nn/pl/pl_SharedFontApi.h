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
* @file
* @brief  共有フォントに関する公開ヘッダファイルです。
*
* @details
*   本体システムはアプリケーションからアクセスすることのできる共有のリソース
*   としてフォントデータを内蔵しています。
*   アプリケーションはここで提供される API を介してこれらのフォントデータを
*   利用することができます。
*
*/

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/settings/settings_Language.h>

namespace nn { namespace pl {

/**
* @brief  共有フォントの種類を表します。
*/
enum SharedFontType
{
    SharedFontType_Standard,                //!< 日米欧
    SharedFontType_ChineseSimple,           //!< 簡体字
    SharedFontType_ChineseSimpleExtension,  //!< 簡体字（追加文字）
    SharedFontType_ChineseTraditional,      //!< 繁体字
    SharedFontType_Korean,                  //!< ハングル
    SharedFontType_NintendoExtension,       //!< 任天堂外字
    SharedFontType_Max,                     //!< 列挙子の総数
};

/**
* @brief  共有フォントのデータを表します。
*/
struct SharedFontData
{
    SharedFontType  type;       //!< フォントの種類
    const void*     address;    //!< フォントデータのアドレス
    size_t          size;       //!< フォントデータのサイズ
};

/**
* @brief  共有フォントのロード状態を表します。
*/
enum SharedFontLoadState
{
    SharedFontLoadState_Loading,    //!< ロード中及びロード順番待ち
    SharedFontLoadState_Loaded      //!< ロード済
};


/**
* @brief      指定した共有フォントの優先的なロードを要求します。
*
* @param[in]  sharedFontType  共有フォントの種類を指定します。
*
* @details    システムは自動的に共有フォントのロードを開始し、順番に共有フォントをロードします。
*             最終的に全ての共有フォントがロード済み状態になることはシステムが保証します。
*             この関数は、指定した共有フォントを先にロードすることをシステムに要求します。
*             ロード済みの共有フォントに対するこの関数の呼び出しに影響はありません。
*/
void RequestSharedFontLoad(SharedFontType sharedFontType) NN_NOEXCEPT;


/**
* @brief      指定した共有フォントのロード状態を取得します。
*
* @param[in]  sharedFontType  共有フォントの種類を指定します。
*
* @return     共有フォントのロード状態を返します。
*/
SharedFontLoadState GetSharedFontLoadState(SharedFontType sharedFontType) NN_NOEXCEPT;


/**
* @brief      指定した共有フォントのアドレスを取得します。
*
* @param[in]  sharedFontType  共有フォントの種類を指定します。
*
* @return     共有フォントのアドレスを返します。
*
* @pre
*             - 指定したフォントがロード済みである。
*
* @details    フォントデータのフォーマットはデコード済みの bfttf です。
*/
const void* GetSharedFontAddress(SharedFontType sharedFontType) NN_NOEXCEPT;


/**
* @brief      指定した共有フォントのサイズを取得します。
*
* @param[in]  sharedFontType  共有フォントの種類を指定します。
*
* @return     共有フォントのサイズを返します。
*
* @pre
*             - 指定したフォントがロード済みである。
*/
size_t GetSharedFontSize(SharedFontType sharedFontType) NN_NOEXCEPT;


/**
* @brief        全ての共有フォントを言語ごとに定められた優先度順に取得します。
* @param[out]   pOutFontCount   取得可能なフォントの数の格納先。
* @param[out]   outData         フォントデータの格納先。
* @param[in]    outCountMax     outData の要素数。全ての共有フォントのデータを取得するには pl::SharedFontType_Max 以上である必要があります。
* @param[in]    languageCode    言語。
* @return       全てのフォントがロード済みでフォントの取得に成功した場合は true。
* @pre
*               - pOutFontCount != nullptr
*               - outData != nullptr
* @post
*               - 返り値が true の場合
*                   - outData[0 ~ min(*pOutFontCount, outCountMax) - 1] が有効なフォントデータを保持
* @details      全ての共有フォントを languageCode に指定した言語で定められた優先度順に取得します。@n
*               共有フォントを用いてできる限り正しくあらゆる文字の描画を行うには
*               複数のフォントを正しい優先度（ある文字を指定したときにどのフォントのデータを使うかの順番）で組み合わせる必要があり、
*               この優先度は言語によって異なります（中国語ならば日米欧フォントよりも中国語フォントの優先度を高くする、など）。@n
*               本関数で取得された outData のインデックス 0 のデータから順に高優先度に使うように設定することで、指定した言語に適したフォントで文字の描画を行うことができます。
*               複数のフォントを組み合わせて使う方法についてはフォントライブラリのマニュアルなどを参照ください。
*/
bool GetSharedFont(int* pOutFontCount, SharedFontData outData[], int outCountMax, settings::LanguageCode languageCode) NN_NOEXCEPT;

}}
