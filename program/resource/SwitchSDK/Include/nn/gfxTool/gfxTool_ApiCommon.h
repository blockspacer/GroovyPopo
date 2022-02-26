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
* @brief gfx ツールに関する API の宣言
*/

#pragma once

#include <cstdint>

#include "gfxTool_ResultCode.h"

#define NN_GFXTOOL_PTR64( ptr ) union { ptr ; uint64_t : 64; }

/**
* @brief ハンドルを表す型定義です。
*/
typedef uint64_t nngfxToolHandle;

/**
* @brief ログ出力タイプです。
*/
enum nngfxToolLogType
{
    nngfxToolLogType_Default, //!< 標準のログ出力です。
    nngfxToolLogType_Warning, //!< 警告のログ出力です。
    nngfxToolLogType_Error, //!< エラーのログ出力です。

    nngfxToolLogType_End
};

/**
* @brief 文字列です。
*/
struct nngfxToolString
{
    uint32_t length; //!< 文字列の長さです。NULL 終端は含みません。
    char reserved[ 4 ]; //!< 予約領域です。
    NN_GFXTOOL_PTR64( const char* pValue ); //!< 文字列へのポインターです。
};

/**
* @brief ログ出力ストリーム設定の引数です。
*/
struct nngfxToolSetLogStreamArg
{
    uint8_t logType; //!< nngfxToolLogType で表される、設定する対象のログ出力タイプです。
    char reserved[ 7 ]; //!< 予約領域です。
    NN_GFXTOOL_PTR64( void* hLogStream ); //!< HANDLE 型で表される、ログ出力先として設定するストリームです。
    char reserved2[ 16 ]; //!< 予約領域です。
};

/**
* @brief API バージョン情報です。
*/
struct nngfxToolApiVersion
{
    uint16_t major; //!< メジャーバージョンです。後方互換性が崩れたことを示すために増加されます。使うライブラリーとヘッダーのメジャーバージョンが異なる場合、動作は保証されません。
    uint16_t minor; //!< マイナーバージョンです。前方互換性が崩れたことを示すために増加されます。使うライブラリーのマイナーバージョンがヘッダーのマイナーバージョンより小さい場合、動作は保証されません。
    uint16_t micro; //!< マイクロバージョンです。変更があったことを示すために増加されます。
    char reserved[ 10 ]; //!< 予約領域です。
};

/**
* @brief ファイルを読み込むためのコールバックの型です。
*/
extern "C" typedef bool ( *nngfxToolReadFileCallback )( void** ppOutFileData,
    size_t* pOutFileDataSize, const char* pFileName, void* pParam );

/**
* @brief ファイルを書き込むためのコールバックの型です。
*/
extern "C" typedef bool ( *nngfxToolWriteFileCallback )( const void* pFileData,
    size_t fileDataSize, const char* pFilename, void* pParam );
