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

/*!
    @file
    @brief  モジュール情報取得関連の API 宣言
*/

#include <nn/nn_Common.h>
#include <nn/diag/diag_Config.h>

namespace nn { namespace diag {

/*!
    @brief  引数に指定したアドレスを含むモジュールを作成するのに使用された nss ファイルのパスを取得します。

    @param[out] outPathBuffer   パスを格納するバッファ。
    @param[in]  pathBufferSize  パスを格納するバッファのバイトサイズ。
    @param[in]  address         モジュールを検索するアドレス。

    @return                     モジュールのアドレス。モジュールが見つからない場合は 0 を返す。

    @pre
        - outPathBuffer != nullptr
        - pathBufferSize > 0
        - address > 0

    @details
        outPathBuffer には、取得したモジュールのパスが格納されます。outPathBuffer はヌル終端されます。@n
        パスが、pathBufferSize より長い場合は、pathBufferSize - 1 バイトまで格納されます。@n
        パスの文字コードは、UTF-8 です。@n
        モジュールがシステムモジュールの場合は、パスではなくモジュール名が取得されます。
*/
uintptr_t GetModulePath(char* outPathBuffer, size_t pathBufferSize, uintptr_t address) NN_NOEXCEPT;

/*!
    @brief      モジュールの ID の最大長です。
*/
const size_t ModuleIdSizeMax = 20;

/*!
    @brief      モジュールの情報です。

    @platformbegin{NX}
        - id は elf フォーマットの Build ID です。
        - age は 常に 0 です。

    @platformend

    @platformbegin{Windows}
        - id は pe フォーマットの GUID です。

    @platformend
*/
struct ModuleInfo
{
    const char* path;       //!< モジュールを作成するのに使用された nss ファイルのパス。ヌル終端されます。
    uintptr_t baseAddress;  //!< モジュールがロードされているアドレス。
    size_t size;            //!< モジュールのバイトサイズ。
    const void* id;         //!< モジュールの ID を表すバイナリ列。
    size_t idSize;          //!< モジュールの ID のバイトサイズ。ID が存在しないときは 0。
    uint32_t age;           //!< ビルドするたびに増加する値。
};

/*!
    @brief  ロード済み全モジュールの情報取得に必要なバッファサイズを計算して取得します。

    @return バッファのバイトサイズ。失敗したときは 0 を返します。

    @details
        本関数でバッファサイズを取得してから、 @ref GetAllModuleInfo の実行が完了するまでに、
        モジュール構成が変更されるような操作（DLL のロード／アンロード）を行うと、すべてのモジュールの情報が取得できない可能性があります。
*/
size_t GetRequiredBufferSizeForGetAllModuleInfo() NN_NOEXCEPT;

/*!
    @brief  ロード済みの全モジュールの情報を取得します。

    @param[out] pOutModules     buffer 内の ModuleInfo 構造体配列の先頭要素を指すポインタ。
    @param[in]  buffer          モジュールの情報を格納するバッファ。
    @param[in]  bufferSize      モジュールの情報を格納するバッファのバイトサイズ。

    @return 情報を取得したモジュールの個数。失敗したときは 0 を返します。

    @pre
        - pOutModules != nullptr
        - buffer != nullptr
        - bufferSize > 0

    @details
        本関数で必要なバッファのサイズは、 @ref GetRequiredBufferSizeForGetAllModuleInfo で取得してください。@n
        バッファのサイズが足りないときは、バッファに収まる個数だけ、モジュールの情報が取得されます。

        バッファには、ModuleInfo 構造体の配列と、構造体のメンバから参照する情報が格納されます。
*/
int GetAllModuleInfo(ModuleInfo** pOutModules, void* buffer, size_t bufferSize) NN_NOEXCEPT;

/*!
    @brief  読み取り専用データセクションの範囲を取得します。

    @param[out] outStartAddress 読み取り専用データセクションの開始アドレス。
    @param[out] outEndAddress   読み取り専用データセクションの終了アドレス。すなわち、読み取り専用データセクションの次のセクションの開始アドレス。
    @param[in]  baseAddress     読み取り専用データセクションの範囲を取得するモジュールのベースアドレス。

    @return 成功したときに true、失敗したときに false。

    @details
        Windows では .rdata、NX では .rodata セクションの範囲を取得します。

    @platformbegin{NX}
        システムモジュールの 読み取り専用データセクションの範囲は取得できません。
    @platformend
*/
bool GetReadOnlyDataSectionRange(uintptr_t* outStartAddress, uintptr_t* outEndAddress, uintptr_t baseAddress) NN_NOEXCEPT;

}} // nn::diag
