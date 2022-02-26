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
* @brief シェーダーコンバーターに関する API の宣言
*/

#pragma once

#include "gfxTool_ApiCommon.h"

#ifdef _WIN32
    #ifdef NN_GFXTOOL_SHADERCONVERTER_DLL_EXPORTS
        #define NN_GFXTOOL_SHADERCONVERTER_API extern "C" __declspec( dllexport )
    #else
        #define NN_GFXTOOL_SHADERCONVERTER_API extern "C" __declspec( dllimport )
    #endif
#else
    #define NN_GFXTOOL_SHADERCONVERTER_API extern "C"
#endif

struct nngfxToolShaderCompilerCompileArg;
struct nngfxToolShaderCompilerCompileOutput;
struct nngfxToolShaderConverterCreateConvertArgOutput;

#define NN_GFXTOOL_SHADERCONVERTER_VERSION_MAJOR 1 // NOLINT(preprocessor/const)
#define NN_GFXTOOL_SHADERCONVERTER_VERSION_MINOR 8 // NOLINT(preprocessor/const)
#define NN_GFXTOOL_SHADERCONVERTER_VERSION_MICRO 0 // NOLINT(preprocessor/const)

/**
* @brief シェーダーコンバーターのバージョンです。
*/
enum nngfxToolShaderConverterVersion
{
    nngfxToolShaderConverterVersion_Major = NN_GFXTOOL_SHADERCONVERTER_VERSION_MAJOR, //!< メジャーバージョンです
    nngfxToolShaderConverterVersion_Minor = NN_GFXTOOL_SHADERCONVERTER_VERSION_MINOR, //!< マイナーバージョンです
    nngfxToolShaderConverterVersion_Micro = NN_GFXTOOL_SHADERCONVERTER_VERSION_MICRO //!< マイクロバージョンです
};

/**
* @brief ハンドル作成の引数です。
*/
struct nngfxToolShaderConverterCreateHandleArg
{
    char reserved[ 64 ]; //!< 予約領域です。
};

/**
* @brief コンバート引数です。
*/
struct nngfxToolShaderConverterConvertArg
{
    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerCompileArg* pCompileArg ); //!< コンパイル引数です。

    nngfxToolString outputName; //!< 出力バイナリーの名前です。

    NN_GFXTOOL_PTR64( const nngfxToolShaderConverterCreateConvertArgOutput* pInformation ); //!< ヘルパー関数からコンバート引数を作成した際に出力される情報へのポインターです。

    int8_t disableMemoryPool; //!< 0 以外であればメモリープールをリソースに含めないようにします。
    int8_t decomposeBinary; //!< 0 以外であれば、バイナリーの共有部分を増やしリソースサイズを小さくするためにバイナリーを分解します。ランタイム上で再構築されます。
    int8_t enableFullReflection; //!< 0 以外であれば、完全なリフレクション情報をリソースに含めます。コンパイル引数でリフレクションを有効にしてある必要があります。
    int8_t disableMergeProgram; //!< 0 以外であれば、機能的に等しいコンパイル結果をマージして共有する処理を無効化します。

    int32_t mergeShaderFileCount; //!< マージするシェーダファイルの数です。シェーダファイルのマージを行う場合は pCompileArg に NULL を指定してください。
    NN_GFXTOOL_PTR64( const void* const* pMergeShaderFiles ); //!< マージするシェーダファイルへのポインタへの配列へのポインタです。シェーダファイルのマージを行う場合は pCompileArg に NULL を指定してください。

    char reserved[ 48 ]; //!< 予約領域です。
};

/**
* @brief コンバート結果の出力です。
*/
struct nngfxToolShaderConverterConvertOutput
{
    uint32_t binarySize; //!< コンバートするバイナリーのサイズです。
    uint32_t binaryAlignment; //!< コンバートするバイナリーをランタイムで配置する際に要求されるアライメントです。

    NN_GFXTOOL_PTR64( const nngfxToolShaderCompilerCompileOutput* pCompileOutput ); //!< コンパイル結果の出力です。

    char reserved[ 64 ]; //!< 予約領域です。
};

/**
* @brief シェーダーコンバータのハンドルを作成します。
*
* @param[out] pOutHandle 作成されるハンドルを格納するための変数へのポインター
* @param[in] pArg ハンドル作成の引数へのポインター
*
* @pre
* - pOutHandle != NULL
* - pArg != NULL
*
* @post
* - *pOutHandle に作成されたハンドルが格納されている
*
* @return 結果コードを返します。
*/
NN_GFXTOOL_SHADERCONVERTER_API nngfxToolResultCode nngfxToolShaderConverterCreateHandle(
    nngfxToolHandle* pOutHandle, const nngfxToolShaderConverterCreateHandleArg* pArg );

/**
* @brief シェーダーコンバーターのハンドルをリセットし再利用できる状態にします。
*
* @param[in] handle シェーダーコンバーターのハンドル
*
* @pre
* - handle の値が作成されたハンドルである
* - handle が削除されていない
*
* @post
* - handle がリセットされている
*
* @return 結果コードを返します。
*/
NN_GFXTOOL_SHADERCONVERTER_API nngfxToolResultCode nngfxToolShaderConverterResetHandle(
    nngfxToolHandle handle );

/**
* @brief シェーダーをコンバートします。
*
* @details
* pArg->pCompileArg != NULL のとき、内部的にシェーダーコンパイラーによるコンパイルを行い、最終的に書き出すバイナリーのサイズを求めます。
* pArg->pCompileArg == NULL のとき、pArg->pMergeShaderFiles で指定されたシェーダファイルをマージし、最終的に書き出すバイナリーのサイズを求めます。
* 実際にバッファーに書き出すには、この後にシリアライズを呼び出します。
*
* @param[out] pOutput コンバート結果の出力を格納するための変数へのポインター
* @param[in] handle シェーダーコンバーターのハンドル
* @param[in] pArg コンバート引数
*
* @pre
* - pArg != NULL
* - handle の値が作成されたハンドルである
* - handle が削除されていない
* - handle に対してコンバートが完了していない
*
* @post
* - コンバートが完了している
* - pOutput != NULL の場合、*pOutput にコンバート結果が格納されている
*
* @return 結果コードを返します。
*/
NN_GFXTOOL_SHADERCONVERTER_API nngfxToolResultCode nngfxToolShaderConverterConvert(
    nngfxToolShaderConverterConvertOutput* pOutput, nngfxToolHandle handle,
    const nngfxToolShaderConverterConvertArg* pArg );

/**
* @brief コンバート結果をシリアライズします。
*
* @param[out] pOutBuffer シリアライズ先のバッファー
* @param[in] bufferSize バイトでの pOutBuffer のサイズ
* @param[in] handle シェーダーコンバーターのハンドル
*
* @pre
* - pOutBuffer != NULL
* - bufferSize がコンバート結果に出力されたサイズ以上
* - handle の値が作成されたハンドルである
* - handle が削除されていない
* - handle に対してコンバートが完了している
* - handle に対してシリアライズしていない
*
* @post
* - *pOutBuffer にシリアライズ結果が格納されている
*
* @return 結果コードを返します。
*/
NN_GFXTOOL_SHADERCONVERTER_API nngfxToolResultCode nngfxToolShaderConverterSerialize(
    void* pOutBuffer, size_t bufferSize, nngfxToolHandle handle );

/**
* @brief ハンドルを削除します。
*
* @param[in] handle 削除するシェーダーコンバーターのハンドル
*
* @pre
* - handle の値が作成されたハンドルである
* - handle が削除されていない
*
* @post
* - handle が削除されている
*
* @return 結果コードを返します。
*/
NN_GFXTOOL_SHADERCONVERTER_API nngfxToolResultCode nngfxToolShaderConverterDeleteHandle(
    nngfxToolHandle handle );

// 以下はコンバート・シリアライズするのに必須ではない API

/**
* @brief コンバート引数を作るための引数です。
*/
struct nngfxToolShaderConverterCreateConvertArgArg
{
    uint32_t optionCount; //!< オプションの数です。
    char reserved2[ 4 ];
    NN_GFXTOOL_PTR64( const char* const* ppOptionArray ); //!< オプション文字列へのポインターの配列へのポインターです。

    NN_GFXTOOL_PTR64( nngfxToolReadFileCallback pReadInputFileCallback ); //!< 入力ファイルを読み込むためのコールバックです。
    NN_GFXTOOL_PTR64( void* pReadInputFileCallbackParam ); //!< 入力ファイルを読み込むためのコールバックに渡されるパラメーターです。

    NN_GFXTOOL_PTR64( nngfxToolReadFileCallback pReadIncludeFileCallback ); //!< インクルードファイルを読み込むためのコールバックです。
    NN_GFXTOOL_PTR64( void* pReadIncludeFileCallbackParam ); //!< インクルードファイルを読み込むためのコールバックに渡されるパラメーターです。

    NN_GFXTOOL_PTR64( nngfxToolWriteFileCallback pWriteDebugInfoFileCallback ); //!< デバッグ情報ファイルを書き込むためのコールバックです。
    NN_GFXTOOL_PTR64( void* pWriteDebugInfoFileCallbackParam ); //!< デバッグ情報ファイルを書き込むためのコールバックに渡されるパラメーターです。

    NN_GFXTOOL_PTR64( nngfxToolReadFileCallback pReadShaderCacheCallback ); //!< シェーダーキャッシュファイルを読み込むためのコールバックです。
    NN_GFXTOOL_PTR64( void* pReadShaderCacheCallbackParam ); //!< シェーダーキャッシュファイルを読み込むためのコールバックに渡されるパラメーターです。
    NN_GFXTOOL_PTR64( nngfxToolWriteFileCallback pWriteShaderCacheCallback ); //!< シェーダーキャッシュファイルを書き込むためのコールバックです。
    NN_GFXTOOL_PTR64( void* pWriteShaderCacheCallbackParam ); //!< シェーダーキャッシュファイルを書き込むためのコールバックに渡されるパラメーターです。

    char reserved[ 16 ]; //!< 予約領域です。
};

/**
* @brief コンバート引数を作る際に出力される情報です。
*/
struct nngfxToolShaderConverterCreateConvertArgOutput
{
    nngfxToolString outputPath; //!< 出力パスです。
    nngfxToolString dumpDirectory; //!< ダンプ先のディレクトリーです。

    char reserved[ 64 ]; //!< 予約領域です。
};

/**
* @brief コンバート引数を作成します。
*
* @param[out] ppOutConvertArg コンバート引数へのポインターを格納するためのポインターへのポインター
* @param[in] pArg コンバート引数を作成するための引数へのポインター
*
* @pre
* - ppOutConvertArg != NULL
* - pArg != NULL
*
* @post
* - *ppOutConvertArg にコンバート引数へのポインターが格納されている
*
* @return 結果コードを返します。
*/
NN_GFXTOOL_SHADERCONVERTER_API nngfxToolResultCode nngfxToolShaderConverterCreateConvertArg(
    nngfxToolShaderConverterConvertArg** ppOutConvertArg,
    const nngfxToolShaderConverterCreateConvertArgArg* pArg );

/**
* @brief コンバート引数を削除します。
*
* @param[in] pConvertArg コンバート引数へのポインター
*
* @pre
* - pConvertArg != NULL
* - pConvertArg がコンバート引数の作成の出力である
* - pConvertArg が削除されていない
*
* @post
* - pConvertArg が削除されている
*
* @return 結果コードを返します。
*/
NN_GFXTOOL_SHADERCONVERTER_API nngfxToolResultCode nngfxToolShaderConverterDeleteConvertArg(
    nngfxToolShaderConverterConvertArg* pConvertArg );

/**
* @brief シェーダーコンバータのログ出力ストリームを設定します。
*
* @param[in] pArg ログ出力ストリーム設定引数へのポインター
*
* @pre
* - pArg != NULL
*
* @post
* - ログ出力ストリームが設定されている
*
* @return 結果コードを返します。
*
* @details
* NULL のストリームを設定すると、ログ出力されなくなります。
*/
NN_GFXTOOL_SHADERCONVERTER_API nngfxToolResultCode nngfxToolShaderConverterSetLogStream(
    const nngfxToolSetLogStreamArg* pArg );

/**
* @brief シェーダーコンバーターの API バージョンを取得します。
*
* @param[out] pOutApiVersion 出力される API バージョンを格納するための変数へのポインタ
*
* @return 結果コードを返します。
*
* @details
* 結果コードが nngfxToolResultCode_Succeeded のとき、pOutApiVersion の major, minor, micro メンバーは、API バージョンにかかわらず正しい値が格納されていることが保証されます。
*/
NN_GFXTOOL_SHADERCONVERTER_API nngfxToolResultCode nngfxToolShaderConverterGetApiVersion(
    nngfxToolApiVersion* pOutApiVersion );
