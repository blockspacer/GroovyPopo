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
 * @brief   RO ライブラリで使用する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/ro/ro_Result.h>

namespace nn { namespace ro {

struct Module;
struct RegistrationInfo;

//--------------------------------------------------------------------------
/**
 * @brief   RO ライブラリを初期化します。
 *
 * @pre
 *  - RO ライブラリが未初期化状態もしくは初期化されていない状態である
 *
 * @post
 *  - RO ライブラリが初期化状態である
 *
 * @details
 * RO ライブラリの他の関数を呼び出す前に呼び出しておく必要があります。
 * @n
 * なお、本 API はスレッドセーフではありません。@n
 * 本 API の動作中にRO ライブラリの関数を呼び出さないでください。
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API は使用できません。
 *
 * @platformend
 *
 */
void        Initialize() NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   RO ライブラリの終了処理を行います。
 *
 * @pre
 *  - RO ライブラリが初期化状態である
 *
 * @post
 *  - RO ライブラリが未初期化状態である
 *  - ロードされていた nro ファイルや nrr ファイルが解放される
 *
 * @details
 * 登録されているすべてのモジュールの情報を削除し、ロードされている全ての動的モジュールを解放します。@n
 * ロードされている動的モジュールの静的オブジェクトのデストラクタはこの関数の中で呼び出されます。@n
 * 静的オブジェクトのデストラクタの中で nn::ro::LoadModule(), nn::ro::UnloadModule(), nn::ro::Finalize() を呼ぶことはできません。@n
 * @n
 * なお、本 API はスレッドセーフではありません。@n
 * 本 API の動作中に RO ライブラリの関数を呼び出さないでください。
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API は使用できません。
 *
 * @platformend
 *
 */
void        Finalize() NN_NOEXCEPT;



//--------------------------------------------------------------------------
/**
 * @brief   Module オブジェクトをロードするために必要なメモリサイズを取得します。
 *
 * @param[out] pOutSize      Module オブジェクトの BSS セクションとして使われる領域の
 *                           サイズを格納するアドレス
 * @param[in]  pImage        nro ファイルを読み込んだメモリの先頭アドレス
 *
 * @retresult
 *   @handleresult{nn::ro::ResultInvalidNroImage}
 * @endretresult
 *
 * @pre
 *  - なし
 *
 * @post
 *  - なし
 *
 * @details
 * nn::ro::LoadModule() 関数を呼び出すときに、モジュールが利用する BSS セクション
 * の領域をユーザーが指定する必要があります。@n
 * この関数はその際に必要となるメモリサイズを取得します。
 * 取得したメモリサイズは nn::os::MemoryPageSize の整数倍であることが保障されます。@n
 * ただし、 nn::os::MemoryBlockUnitSize の整数倍にはなっていない可能性があるので、
 * nn::os::AllocateMemoryBlock() の引数に取得したメモリサイズを直接渡す場合には注意が必要です。@n
 * @n
 * 本 API はスレッドセーフです。
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API は使用できません。
 *
 * @platformend
 *
 */
nn::Result  GetBufferSize(size_t* pOutSize, const void* pImage) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   モジュールの情報を登録します。
 *
 * @param[out] pOutInfo 登録オブジェクトを格納するアドレス
 * @param[in]  pImage   nrr ファイルを読み込んだメモリの先頭アドレス
 *
 * @retresult
 *   @handleresult{nn::ro::ResultInvalidNrrImage}
 *   @handleresult{nn::ro::ResultMaxRegistration}
 *   @handleresult{nn::ro::ResultNotAuthorized}
 * @endretresult
 *
 * @pre
 *  - pImage で指定される領域が nn::os::AllocateMemoryBlock() で確保されたメモリ領域で、
 *    アクセス権が nn::os::MemoryPermission_ReadWrite である。
 *  - pImage が nn::os::MemoryPageSize の整数倍である。
 *  - pOutInfo の状態が未初期化もしくは Unregistered の状態である
 *  - RO ライブラリが初期化状態である
 *  - nrr ファイルを ROM-FS の指定された場所に置かれており、RAW 形式では実行されていないこと。
 *
 * @post
 *  - pOutInfo の状態が Registered の状態である
 *
 * @details
 * nrr ファイルをRO ライブラリの管理下に置き、nro ファイルの情報を登録します。
 * nn::ro::LoadModule() 関数を呼び出す前に nro ファイルの情報が格納されている nrr ファイルを登録しておく必要があります。@n
 * 関数が成功した場合、pImage で指定される領域が RO ライブラリの管理下に置かれ、
 * 他の用途で利用できなくなります。
 * この領域を RO ライブラリの管理下から外す場合は、 nn::ro::UnregisterModuleInfo() を呼び出す必要があります。@n
 * nrr ファイルを ROM-FS の指定された場所に置き、RAW 形式で実行しないという制限は、DevMenu の開発支援機能の設定に
 * 存在する Ease Nro Restriction が有効になっている場合、無効化されます@n
 *
 * @n
 * 本 API はスレッドセーフです。
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API は使用できません。
 *
 * @platformend
 *
 */
nn::Result  RegisterModuleInfo(RegistrationInfo* pOutInfo, const void* pImage) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   モジュールの情報を削除します。
 *
 * @param[in]  pInfo    登録オブジェクトが格納されているアドレス
 *
 * @retresult
 *   @handleresult{nn::ro::ResultInvalidNrrImage}
 * @endretresult
 *
 * @pre
 *  - pInfo が Registered の状態である
 *  - RO ライブラリが初期化状態である
 *
 * @post
 *  - pInfo が Unregistered の状態である
 *  - nn::ro::LoadModule() で指定された pImage の領域が nn::ro::MemoryPermission_ReadWrite である
 *
 * @details
 * 指定した登録オブジェクトの登録を解除します。
 * この登録オブジェクトに関する RO ライブラリの管理下にあったメモリを解放し、他の用途で使用できるようにします。@n
 * 「この登録オブジェクトに関する RO ライブラリの管理下にあったメモリ」とは、 nn::ro::RegisterModuleInfo() 関数の
 * pImage 引数で指定したメモリ領域です。@n
 * @n
 * 本 API はスレッドセーフです。
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API は使用できません。
 *
 * @platformend
 *
 */
void    UnregisterModuleInfo(RegistrationInfo* pInfo) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   Module オブジェクトを読み込みます。
 *
 * @param[out] pOutModule    Module オブジェクトを格納するアドレス
 * @param[in]  pImage        nro ファイルを読み込んだメモリの先頭アドレス
 * @param[in]  buffer        BSS セクションとして使われる領域の先頭アドレス
 * @param[in]  bufferSize    buffer が指す領域のサイズ
 * @param[in]  flag          シンボル解決のポリシーに関するフラグ
 *
 * @retresult
 *   @handleresult{nn::ro::ResultInvalidNroImage}
 *   @handleresult{nn::ro::ResultOutOfAddressSpace}
 *   @handleresult{nn::ro::ResultNroAlreadyLoaded}
 *   @handleresult{nn::ro::ResultMaxModule}
 *   @handleresult{nn::ro::ResultNotAuthorized}
 * @endretresult
 *
 * @pre
 *  - pOutModule の状態が 未初期化もしくは Unloaded の状態である
 *  - pImage で指定される領域が nn::os::AllocateMemoryBlock() で確保されたメモリ領域で、
 *    アクセス権が nn::os::MemoryPermission_ReadWrite である。
 *  - pImage が nn::os::MemoryPageSize の整数倍である。
 *  - buffer/bufferSize で指定される領域が nn::os::AllocateMemoryBlock() で確保されたメモリ領域で、
 *    アクセス権が nn::os::MemoryPermission_ReadWrite である。
 *  - buffer/bufferSize が nn::os::MemoryPageSize の整数倍である。
 *  - flag には nn::ro::BindFlag_Now か nn::ro::BindFlag_Lazy のどちらかが指定されている。
 *  - 読み込む nro ファイルの情報が格納されている nrr ファイルが nn::ro::RegisterModuleInfo() によって登録されている。
 *  - RO ライブラリが初期化状態である
 *
 * @post
 *  - pOutModule が Loaded 状態である
 *
 * @details
 * @ref LoadModule(Module* pOutModule, const void* pImage, void* buffer, size_t bufferSize, int flag, bool isNotReferenced) の isNotReferenced に false を指定した場合と同じ動作をします。
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API は使用できません。
 *
 * @platformend
 *
 */
nn::Result  LoadModule(Module* pOutModule, const void* pImage, void* buffer, size_t bufferSize, int flag) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   Module オブジェクトを読み込みます。Module オブジェクトが他のモジュールから参照されない場合 nn::ro::UnloadModule() が高速に動作するオプションで読み込むことが可能です。
 *
 * @param[out] pOutModule       Module オブジェクトを格納するアドレス
 * @param[in]  pImage           nro ファイルを読み込んだメモリの先頭アドレス
 * @param[in]  buffer           BSS セクションとして使われる領域の先頭アドレス
 * @param[in]  bufferSize       buffer が指す領域のサイズ
 * @param[in]  flag             シンボル解決のポリシーに関するフラグ
 * @param[in]  isNotReferenced  Module オブジェクトのシンボルが他のモジュールから解決されない場合、true を指定することによって nn::ro::UnloadModule() が高速に動作するようになります。
 *
 * @retresult
 *   @handleresult{nn::ro::ResultInvalidNroImage}
 *   @handleresult{nn::ro::ResultOutOfAddressSpace}
 *   @handleresult{nn::ro::ResultNroAlreadyLoaded}
 *   @handleresult{nn::ro::ResultMaxModule}
 *   @handleresult{nn::ro::ResultNotAuthorized}
 * @endretresult
 *
 * @pre
 *  - pOutModule の状態が 未初期化もしくは Unloaded の状態である
 *  - pImage で指定される領域が nn::os::AllocateMemoryBlock() で確保されたメモリ領域で、
 *    アクセス権が nn::os::MemoryPermission_ReadWrite である。
 *  - pImage が nn::os::MemoryPageSize の整数倍である。
 *  - buffer/bufferSize で指定される領域が nn::os::AllocateMemoryBlock() で確保されたメモリ領域で、
 *    アクセス権が nn::os::MemoryPermission_ReadWrite である。
 *  - buffer/bufferSize が nn::os::MemoryPageSize の整数倍である。
 *  - flag には nn::ro::BindFlag_Now か nn::ro::BindFlag_Lazy のどちらかが指定されている。
 *  - 読み込む nro ファイルの情報が格納されている nrr ファイルが nn::ro::RegisterModuleInfo() によって登録されている。
 *  - RO ライブラリが初期化状態である
 *
 * @post
 *  - pOutModule が Loaded 状態である
 *
 * @details
 * nro ファイルの再配置を実施し、既に登録されているモジュールとの間で参照の解決を行い、RO ライブラリの管理下に置きます。@n
 * 関数が成功した場合、pImage および buffer で指定される領域が RO ライブラリの管理下に置かれ、
 * 他の用途で利用できなくなります。
 * この領域を RO ライブラリの管理下から外す場合は、 nn::ro::UnloadModule() を呼び出す必要があります。@n
 * この関数の呼び出しの中で、指定されたモジュールの静的オブジェクトのコンストラクタが自動的に呼び出されます。@n
 * 静的オブジェクトのコンストラクタの中で nn::ro::LoadModule(), nn::ro::UnloadModule(), nn::ro::Finalize() を呼ぶことはできません。@n
 *
 * flag には、 nn::ro::BindFlag_Now もしくは nn::ro::BindFlag_Lazy のどちらかを含める必要があります。@n
 * nn::ro::BindFlag_Now を指定し、即時解決を利用する場合は、モジュールがロードされるときにシンボル解決を行います。@n
 * nn::ro::BindFlag_Lazy を指定し、遅延解決を利用する場合は、関数のシンボル解決を関数が実際に呼び出されるときに行うようになります。
 * ただし、関数以外は遅延解決ではなく、即時解決されます。@n
 *
 * isNotReferenced に true を指定した場合、指定されたモジュールを @ref nn::ro::UnloadModule() する際に、処理時間が短くなります。@n
 * ただし、このモジュール内のシンボルに対する他のモジュールからのシンボル解決が発生した場合、その時点で Abort が発生します。
 * nn::ro::LookupSymbol(), nn::ro::LookupModuleSymbol() で取得したシンボルのアドレス経由での呼び出しは問題ありません。@n
 * isNotReferenced に false を指定した場合は、@ref nn::ro::UnloadModule() の処理時間は短くなりませんが、他のモジュールからのシンボル解決が可能となります。
 * @n
 * 本 API はスレッドセーフです。
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API は使用できません。
 *
 * @platformend
 *
 */
nn::Result  LoadModule(Module* pOutModule, const void* pImage, void* buffer, size_t bufferSize, int flag, bool isNotReferenced) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   シンボルを検索します。
 *
 * @param[out] pOutAddress   シンボルのアドレスを格納するアドレス
 * @param[in]  name          検索するシンボル名
 *
 * @retresult
 *   @handleresult{nn::ro::ResultNotFound}
 * @endretresult
 *
 * @pre
 *  - name の文字列がヌル文字で終端している。
 *
 * @post
 *  - なし
 *
 * @details
 * ロードされている全モジュールの中から、指定された名前のシンボルを探索し、そのアドレスを取得します。@n
 * 「ロードされている全モジュール」には、 nn::ro::LoadModule() でロードした動的モジュールの他に、
 * プログラムの起動時にロードされる静的モジュールも含みます。@n
 * C++ のシンボルを指定する場合はマングル名を指定する必要があります。@n
 * @n
 * 本 API はスレッドセーフです。
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API は使用できません。
 *
 * @platformend
 *
 */
nn::Result  LookupSymbol(uintptr_t* pOutAddress, const char* name) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   Module オブジェクトで用いられているシンボルを検索します。
 *
 * @param[out] pOutAddress   シンボルのアドレスを格納するアドレス
 * @param[in]  pModule       Module オブジェクトが格納されているアドレス
 * @param[in]  name          検索するシンボル名
 *
 * @retresult
 *   @handleresult{nn::ro::ResultNotFound}
 * @endretresult
 *
 * @pre
 *  - name の文字列がヌル文字で終端している。
 *  - pModule が Loaded 状態である
 *
 * @post
 *  - なし
 *
 * @details
 * 指定されたモジュールの中から指定された名前のシンボルを探索し、そのアドレスを取得します。
 * C++ のシンボルを指定する場合はマングル名を指定する必要があります。@n
 * @n
 * 本 API はスレッドセーフです。
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API は使用できません。
 *
 * @platformend
 *
 */
nn::Result  LookupModuleSymbol(uintptr_t* pOutAddress, const Module* pModule, const char* name) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   Module オブジェクトを解放します。
 *
 * @param[in]  pModule       Module オブジェクトが格納されているアドレス
 *
 * @pre
 *  - pModule が Loaded 状態である
 *  - RO ライブラリが初期化状態である
 *  - 指定されたモジュール内のグローバルな TLS のコンストラクタが呼ばれているスレッドで本関数を呼び出していないこと。
 *
 * @post
 *  - pModule が Unloaded 状態である
 *  - nn::ro::LoadModule() で指定された pImage の領域が nn::os::MemoryPermission_ReadWrite である
 *  - nn::ro::LoadModule() で指定された buffer/bufferSize の領域が nn::os::MemoryPermission_ReadWrite である
 *
 * @details
 * 指定されたモジュールを解放します。@n
 * このモジュールに関する RO ライブラリの管理下にあったメモリを解放し、他の用途で使用できるようにします。@n
 * 「このモジュールに関する RO ライブラリの管理下にあったメモリ」とは、 nn::ro::LoadModule() 関数の
 * pImage 引数および buffer/bufferSize 引数で指定したメモリ領域です。@n
 * この関数の呼び出しの中で、指定されたモジュールの静的オブジェクトのデストラクタが自動的に呼び出されます。@n
 * 静的オブジェクトのデストラクタの中で nn::ro::LoadModule(), nn::ro::UnloadModule(), nn::ro::Finalize() を呼ぶことはできません。@n
 * ただし、グローバルな TLS オブジェクトのデストラクタは自動的に呼び出されず、そのようなオブジェクトのコンストラクタが呼び出されていた場合、本関数内でアボートします。@n
 * 指定されたモジュール内のグローバルな TLS オブジェクトのコンストラクタが呼び出される関数を呼び出したスレッドで、本関数を呼び出さないようにしてください。@n
 * 他のライブラリの管理下に置かれているメモリが残っている場合、本関数は失敗することがあります。@n
 * @n
 * 本 API はスレッドセーフです。
 *
 * @platformbegin{Windows}
 *  - Windows 環境では本 API は使用できません。
 *
 * @platformend
 *
 */
void        UnloadModule(Module* pModule) NN_NOEXCEPT;


}} // namespace nn::ro

