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
 * @brief   振動ファイルに関する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/os/os_Macro.h>
#include <nn/util/util_TypedStorage.h>
#include <nn/hid/hid_VibrationValueTypes.h>
#include <nn/hid/hid_Result.h>

namespace nn { namespace hid {

/**
 * @brief   振動ファイルの情報を表す構造体です。
 *
 * @details
 *  @ref nn::hid::ParseVibrationFile を使うことで、振動ファイルをパースして得られた情報を
 *  この構造体に格納することができます。
 *
 *  ループに関する情報を持たない振動ファイルをパースした場合には、
 *  loopStartPosition と loopInterval には 0 が、 loopEndPosition には sampleLength と同じ値が格納されます。
 */
struct VibrationFileInfo
{
    uint32_t metaDataSize;      //!< メタデータサイズ
    uint16_t formatId;          //!< フォーマット ID
    uint16_t samplingRate;      //!< サンプリングレート
    uint32_t dataSize;          //!< データサイズ
    int32_t sampleLength;       //!< サンプル長
    int32_t isLoop;             //!< 振動ファイルがループ情報を持っていない場合は 0 が、持っている場合は 0 以外が格納されます
    uint32_t loopStartPosition; //!< ループ開始位置 (情報がない場合は 0)
    uint32_t loopEndPosition;   //!< ループ終了位置 (情報がない場合は sampleLength と同じ値)
    uint32_t loopInterval;      //!< ループが終了してからループを開始するまでの時間間隔 (情報がない場合は 0)
};


/**
* @brief   ループ設定と振動値配列の情報を格納する構造体です。
*
* @details
*  @ref nn::hid::GenerateVibrationFile を使うことで、振動値配列から振動ファイルのバイナリデータを生成することができます。
*/
struct VibrationValueArrayInfo
{
    int32_t sampleLength;       //!< 振動値列のサンプル長
    bool isLoop;                //!< 振動ファイルがループ情報を持っている場合は True が、持っていない場合は False が指定します
    uint32_t loopStartPosition; //!< ループ開始位置
    uint32_t loopEndPosition;   //!< ループ終了位置
    uint32_t loopInterval;      //!< ループが終了してからループを開始するまでの時間間隔
};

struct VibrationFileParserContextImpl;

/**
 * @brief   振動ファイルのパーサーのコンテクストを表す構造体です。
 */
struct VibrationFileParserContext
{
    nn::util::TypedStorage<VibrationFileParserContextImpl, 80, NN_ALIGNOF(void*)> _storage;
};

//! @name 振動ファイル関連 API
//! @{

/**
 * @brief       振動ファイルのバイナリデータをパースします。
 *
 * @return      処理の結果が返ります。
 * @retval      nn::ResultSuccess                       処理が正常に終了しました。
 * @retval      nn::hid::ResultVibrationFileInvalid     振動ファイルのバイナリデータが不正です。
 *
 * @param[out]  pOutInfo    振動ファイルの情報の格納先
 * @param[out]  pOutContext パーサーコンテクストの格納先
 * @param[in]   address     振動ファイルのバイナリデータが展開されているメモリの先頭アドレス
 * @param[in]   fileSize    振動ファイルのサイズ
 *
 * @pre
 *              - pOutInfo != nullptr
 *              - pOutContext != nullptr
 *              - address != nullptr
 *              - address を先頭とする連続領域に、
 *                振動ファイルから読み込んだ有効な振動ファイルのバイナリデータが展開されている
 *              - fileSize が読み込んだ振動ファイルのサイズ以上
 *
 * @details
 *  あらかじめメモリ上に展開された振動ファイルのバイナリデータをパースして、
 *  ファイル情報を取り出すと同時にパーサーのコンテクストを初期化します。@n
 *  @ref nn::hid::RetrieveVibrationValue 関数を利用するためには、
 *  事前に本 API を使ってパーサーのコンテクストを初期化しておく必要があります。@n
 *  この関数で初期化した VibrationFileParserContext を使用している間は、
 *  振動ファイルのバイナリデータが展開されている address から fileSize バイトの領域を、
 *  アプリケーション側で書き換えないようにする必要があります。
 */
Result ParseVibrationFile(
    VibrationFileInfo* pOutInfo,
    VibrationFileParserContext* pOutContext,
    const void* address,
    size_t fileSize) NN_NOEXCEPT;

/**
 * @brief       振動ファイルから振動値を取り出します。
 *
 * @param[out]      pOutValue   取り出した振動値の格納先
 * @param[in]       position    取り出したい振動値の位置 (サンプル番号)
 * @param[in, out]  pContext    パーサーのコンテクスト
 *
 * @pre
 *              - pOutValue != nullptr
 *              - 0 <= position < 振動ファイルのサンプル長
 *              - pContext が @ref nn::hid::ParseVibrationFile 関数で初期化済み
 *              - pContext 初期化時に振動ファイルのバイナリデータを展開したメモリ領域に
 *                振動ファイルのバイナリデータがそのまま配置されている
 *
 * @details
 *  あらかじめメモリ上に展開された振動ファイルのバイナリデータをパースして、
 *  振動ファイル内の指定された位置の振動値を取り出します。@n
 *  内部的には、前回振動値を取り出した位置から始めて順にデコードすることで振動値を取り出します。@n
 *  そのため、先頭から順に振動値を取り出すと、最も計算時間が少なく済みます。
 */
void RetrieveVibrationValue(
    VibrationValue* pOutValue,
    int position,
    VibrationFileParserContext* pContext) NN_NOEXCEPT;

/**
 * @brief       振動値配列の情報から振動ファイルのバイナリデータを生成し、指定したバッファに格納します。
 *
 * @param[out]  pOutSize        生成したバイナリデータのサイズ
 * @param[out]  outBuffer       格納するバッファの先頭アドレス
 * @param[in]   bufferSize      格納するバッファのサイズ
 * @param[in]   info            振動値配列の情報
 * @param[in]   pValues         振動値配列の先頭アドレス
 *
 * @pre
 *              - pOutSize != nullptr
 *              - outBuffer != nullptr
 *              - bufferSize > 0
 *              - bufferSize が CalculateVibrationFileSize() で要求されるサイズ以上
 *              - info.sampleLength > 0
 *              - info.loopStartPosition <= info.loopEndPosition
 *              - info.loopEndPosition <= info.sampleLength
 *              - info.loopInterval <= 0x7FFFFFFF
 *              - pValues != nullptr
 *              - pValues を先頭とする連続領域に有効な振動値が書き込まれている
 *
 * @details
 *  必要な格納先のバッファのサイズは CalculateVibrationFileSize() で計算できます。
 *
 *  生成したバイナリデータを拡張子 bnvib でファイル出力することで、振動図鑑にインポートできます。@n
 *  バイナリデータは ParseVibrationFile() や VibrationPlayer で振動の再生に使用できます。@n
 *
 */
void GenerateVibrationFile(
    size_t* pOutSize,
    void* outBuffer,
    size_t bufferSize,
    const VibrationValueArrayInfo& info,
    const VibrationValue* pValues) NN_NOEXCEPT;

/**
 * @brief       振動値配列の情報から生成される振動ファイルのサイズを返します。
 *
 * @return      バイナリデータのサイズ
 *
 * @param[in]   info            振動値配列の情報
 *
 * @details
 *  振動ファイルは振動値のサンプル数やループ設定の有無によって、生成されるバイナリデータのサイズが異なります。@n
 *  振動値配列の情報から GenerateVibrationFile() で必要とされるバッファのサイズを計算します。
 *
 */
size_t CalculateVibrationFileSize(
    const VibrationValueArrayInfo& info) NN_NOEXCEPT;

//! @}

}} // namespace nn::hid
