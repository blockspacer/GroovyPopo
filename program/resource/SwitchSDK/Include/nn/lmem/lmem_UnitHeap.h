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
    @file
    @brief      ユニットヒープのインターフェイスです。
    @details    ユニットヒープ(UnitHeap)に関連する列挙型、関数を定義します。
*/

#include <nn/lmem/lmem_Common.h>

namespace nn { namespace lmem {

/**
    @brief      ヒープ共通ヘッダ（ヒープの管理領域）を配置する場所を決める定数です。
    @details    ヒープの管理領域をメモリ領域の先頭、もしくは末尾に配置するか指定できます。
*/
enum InfoPlacement
{
    InfoPlacement_Head,     //!< ヒープの管理領域をメモリ領域の先頭に配置します
    InfoPlacement_Tail      //!< ヒープの管理領域をメモリ領域の末尾に配置します
};

//! @name ユニットヒープ - ヒープ領域操作 API
//! @{

/**
    @brief      ユニットヒープを作成します。

    @param[in]  startAddress    ヒープに割り当てるメモリブロックの先頭アドレス
    @param[in]  size            ヒープに割り当てるメモリブロックのサイズ（バイト）
    @param[in]  unitSize        1つのメモリブロック（ユニット）のサイズ（バイト）
    @param[in]  option          デバッグ用オプション（CreationOption列挙型）

    @return     関数が成功するとユニットヒープハンドルのポインタが返ります。失敗すると NULL が返ります。

    @pre        startAddress は非 NULL である必要があります。
    @post       指定したメモリブロックにユニットヒープが作成されます。

    @details    ヒープ作成時にはヒープの管理領域（ヒープ共通ヘッダ）も作成されます。@n
                ヒープ管理領域は、ヒープ内部に作成されます。@n
                そのため、 size で指定したサイズからヒープ管理領域 ( nn::lmem::HeapCommonHead 構造体) のサイズを引いた領域が、ユニットヒープで利用可能な領域となります。@n
                @n
                ヒープ管理領域はヒープの先頭に作成され、アライメントは DefaultAlignment が利用されます。@n
                @n
                option では CreationOption 列挙型での指定が可能です。複数の CreationOption 列挙型を OR 指定することも可能です。@n
                option に CreationOption_NoOption を指定すると、オプションは無効となります。@n
                option に CreationOption_ZeroClear を指定すると、このヒープで確保したメモリが 0 でクリアされます。@n
                option に CreationOption_DebugFill を指定すると、ヒープ作成時、メモリ確保、メモリ解放時にメモリを特定の値でフィルします。@n
                CreationOption_DebugFill オプションでフィルする値はデフォルトでは以下のようになっています。@n
                @li ヒープ作成時：0xC3C3C3C3
                @li メモリ確保時：0xF3F3F3F3
                @li メモリ解放時：0xD3D3D3D3
                CreationOption_ZeroClear と CreationOption_DebugFill を OR 指定した場合、ヒープ作成時とメモリ解放時は CreationOption_DebugFill の値でフィルされ、メモリ確保時はメモリが 0 でクリアされます。@n
                この値は SetFillValue() 関数を利用することで変更できます。@n
                内部アルゴリズム上、メモリブロックの先頭からポインタサイズ分はヒープ作成時とメモリ解放時にフィルされません。@n
                @n
                option に CreationOption_ThreadSafe を指定すると、マルチスレッド時の排他制御を有効にします。@n
                @n
                本 API はスレッドセーフではありません。
*/
HeapHandle CreateUnitHeap( void* startAddress, size_t size, size_t unitSize, int option) NN_NOEXCEPT;

/**
    @brief      ユニットヒープを作成します。

    @param[in]  startAddress    ヒープに割り当てるメモリブロックの先頭アドレス
    @param[in]  size            ヒープに割り当てるメモリブロックのサイズ（バイト）
    @param[in]  unitSize        一つのメモリブロック（ユニット）のサイズ（バイト）
    @param[in]  option          デバッグ用オプション（CreationOption列挙型）
    @param[in]  alignment       メモリブロックのアライメント
    @param[in]  placement       ヒープの管理領域（ヒープ共通ヘッダ）を配置する位置

    @return     関数が成功するとユニットヒープハンドルのポインタが返ります。失敗すると NULL が返ります。

    @pre        startAddress は非 NULL である必要があります。@n
                アライメントには 2 のべき乗が指定されている必要があります。
    @post       指定したメモリブロックにユニットヒープが作成されます。

    @details    ヒープ作成時にはヒープの管理領域（ヒープ共通ヘッダ）も作成されます。@n
                ヒープ管理領域は、ヒープ内部に作成されます。@n
                そのため、 size で指定したサイズからヒープ管理領域 (HeapCommonHead) のサイズを引いた領域が、ユニットヒープで利用可能な領域となります。@n
                @n
                アライメントには 2 のべき乗を指定してください。@n
                @n
                option では CreationOption 列挙型での指定が可能です。複数の CreationOption 列挙型を OR 指定することも可能です。@n
                option に CreationOption_NoOption を指定すると、オプションは無効となります。@n
                option に CreationOption_ZeroClear を指定すると、このヒープで確保したメモリが 0 でクリアされます。@n
                option に CreationOption_DebugFill を指定すると、ヒープ作成時、メモリ確保、メモリ解放時にメモリを特定の値でフィルします。@n
                CreationOption_DebugFill オプションでフィルする値はデフォルトでは以下のようになっています。@n
                @li ヒープ作成時：0xC3C3C3C3
                @li メモリ確保時：0xF3F3F3F3
                @li メモリ解放時：0xD3D3D3D3
                CreationOption_ZeroClear と CreationOption_DebugFill を OR 指定した場合、ヒープ作成時とメモリ解放時は CreationOption_DebugFill の値でフィルされ、メモリ確保時はメモリが 0 でクリアされます。@n
                この値は SetFillValue() 関数を利用することで変更できます。@n
                内部アルゴリズム上、メモリブロックの先頭からポインタサイズ分はヒープ作成時とメモリ解放時にフィルされません。@n
                @n
                option に CreationOption_ThreadSafe を指定すると、マルチスレッド時の排他制御を有効にします。@n
                @n
                本 API はスレッドセーフではありません。
*/
HeapHandle CreateUnitHeap( void* startAddress, size_t size, size_t unitSize, int option, int alignment, InfoPlacement placement) NN_NOEXCEPT;

/**
    @brief      ユニットヒープを作成します。

    @param[in]  startAddress    ヒープに割り当てるメモリブロックの先頭アドレス
    @param[in]  size            ヒープに割り当てるメモリブロックのサイズ（バイト）
    @param[in]  unitSize        一つのメモリブロック（ユニット）のサイズ（バイト）
    @param[in]  option          デバッグ用オプション（CreationOption列挙型）
    @param[in]  alignment       メモリブロックのアライメント
    @param[in]  pHeapHead       ヒープ共通ヘッダ

    @return     関数が成功するとユニットヒープハンドルのポインタが返ります。失敗すると NULL が返ります。

    @pre        startAddress は非 NULL である必要があります。@n
                アライメントには 2 のべき乗が指定されている必要があります。
    @post       指定したメモリブロックにユニットヒープが作成されます。

    @details    ヒープ管理領域として、引数 pHeapHead で渡された領域を使用します。@n
                @n
                アライメントには 2 のべき乗を指定してください。@n
                @n
                option では CreationOption 列挙型での指定が可能です。複数の CreationOption 列挙型を OR 指定することも可能です。@n
                option に CreationOption_NoOption を指定すると、オプションは無効となります。@n
                option に CreationOption_ZeroClear を指定すると、このヒープで確保したメモリが 0 でクリアされます。@n
                option に CreationOption_DebugFill を指定すると、ヒープ作成時、メモリ確保、メモリ解放時にメモリを特定の値でフィルします。@n
                CreationOption_DebugFill オプションでフィルする値はデフォルトでは以下のようになっています。@n
                @li ヒープ作成時：0xC3C3C3C3
                @li メモリ確保時：0xF3F3F3F3
                @li メモリ解放時：0xD3D3D3D3
                CreationOption_ZeroClear と CreationOption_DebugFill を OR 指定した場合、ヒープ作成時とメモリ解放時は CreationOption_DebugFill の値でフィルされ、メモリ確保時はメモリが 0 でクリアされます。@n
                この値は SetFillValue() 関数を利用することで変更できます。@n
                内部アルゴリズム上、メモリブロックの先頭からポインタサイズ分はヒープ作成時とメモリ解放時にフィルされません。@n
                @n
                option に CreationOption_ThreadSafe を指定すると、マルチスレッド時の排他制御を有効にします。@n
                @n
                本 API はスレッドセーフではありません。
*/
HeapHandle CreateUnitHeap( void* startAddress, size_t size, size_t unitSize, int option, int alignment, HeapCommonHead* pHeapHead) NN_NOEXCEPT;

/**
    @brief      ユニットヒープを破棄します。

    @param[in]  heapHandle  ヒープハンドル

    @pre        引数 heapHandle が CreateUnitHeap() 関数で事前に初期化されている必要があります。
    @post       指定したユニットヒープが破棄されます。

    @details    ヒープハンドルで指定されたヒープを破棄します。@n
                @n
                本 API はスレッドセーフではありません。

*/
void DestroyUnitHeap( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief      ヒープを無効化します。

    @param[in]  heapHandle  ヒープハンドル

    @pre        引数 heapHandle が CreateUnitHeap() 関数で事前に初期化されている必要があります。
    @post       指定したユニットヒープが無効化されます。

    @details    この関数を呼んだ後は Finalize 以外の操作をヒープに対して行うことができなくなります。@n
                @n
                CreateUnitHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void InvalidateUnitHeap( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief      ヒープとして与えられた領域を拡張します。

    @param[in]  heapHandle  ヒープハンドル
    @param[in]  size        拡張サイズ。Initialize 時に指定したユニットサイズの倍数である必要があります。

    @pre        引数 heapHandle が CreateUnitHeap() 関数で事前に初期化されている必要があります。

    @details    ヒープサイズを後方に伸長しユニットを増加させます。
*/
void ExtendUnitHeapArea( HeapHandle heapHandle, size_t size) NN_NOEXCEPT;

//! @}

//! @name ユニットヒープ - メモリブロック操作 API

/**
    @brief      メモリ領域を確保します。

    @param[in]  heapHandle  ヒープハンドル

    @return     関数が成功すると確保したメモリ領域の先頭へのポインタ、失敗すると NULL ポインタが返ります。

    @pre        引数 heapHandle が CreateUnitHeap() 関数で事前に初期化されている必要があります。
    @post       戻り値が示すポインタからメモリ領域が確保されます。

    @details    ユニットヒープでメモリを確保します。@n
                @n
                CreateUnitHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void* AllocateFromUnitHeap( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief      メモリ領域を解放します。

    @param[in]  heapHandle  ヒープハンドル
    @param[in]  pBlock      解放するするメモリブロック（ユニット）の先頭アドレス

    @pre        引数 heapHandle が CreateUnitHeap() 関数で事前に初期化されている必要があります。
    @post       指定したメモリ領域が解放されます。

    @details    ユニットヒープで確保したメモリ領域を解放します。@n
                @n
                CreateUnitHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void FreeToUnitHeap( HeapHandle heapHandle, void* pBlock ) NN_NOEXCEPT;

//! @}

//! @name ユニットヒープ - ヒープ情報設定・取得 API
//! @{

/**
    @brief      ユニットヒープのユニットサイズを取得します。

    @param[in]  heapHandle ヒープハンドル

    @return     ユニットヒープの1つのメモリブロック（ユニット）のサイズを返します。

    @pre        引数 heapHandle が CreateUnitHeap() 関数で事前に初期化されている必要があります。
    @post       戻り値がユニットヒープのユニットサイズになります。

    @details    ユニットヒープの1つのメモリブロック（ユニット）のサイズを取得します。@n
                @n
                本 API はスレッドセーフです。
*/
size_t GetUnitHeapUnitSize( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief      空きユニットの数を取得します。

    @param[in]  heapHandle ヒープハンドル

    @return     確保されていないユニットの数を返します。

    @pre        引数 heapHandle が CreateUnitHeap() 関数で事前に初期化されている必要があります。
    @post       戻り値がユニットヒープの空きユニット数になります。

    @details    ユニットヒープのユニットのうち、確保されていないユニットの数を取得します。@n
                @n
                CreateUnitHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
int GetUnitHeapAllocatableCount( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief      現在このヒープから確保されているユニットの数を取得します。

    @param[in]  heapHandle ヒープハンドル

    @return     確保されているユニットの数を返します。

    @pre        引数 heapHandle が CreateUnitHeap() 関数で事前に初期化されている必要があります。
    @post       戻り値がユニットヒープの確保ユニット数になります。

    @details    ユニットヒープのユニットのうち、確保されているユニットの数を取得します。@n
                @n
                CreateUnitHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
int GetUnitHeapAllocatedCount( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief      ユニットのアライメントを取得します。

    @param[in]  heapHandle ヒープハンドル

    @return     ユニットのアライメントを返します。

    @pre        引数 heapHandle が CreateUnitHeap() 関数で事前に初期化されている必要があります。
    @post       戻り値がユニットのアライメントになります。

    @details    ユニットヒープの各メモリブロック（ユニット）のアライメントを取得します。@n
                @n
                本 API はスレッドセーフです。
*/
int GetUnitHeapAlignment( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief      指定したユニットサイズとアライメントのブロックを指定数を管理できるだけのヒープメモリのサイズを取得します。

    @param[in]  unitSize            ユニットのサイズ
    @param[in]  unitNum             ユニット数
    @param[in]  alignment           アライメント
    @param[in]  hasHeadInternally   ヒープの管理領域をヒープ内部に持つか

    @return     指定した条件で必要なメモリサイズを返します。

    @pre        アライメントには 2 のべき乗が指定されている必要があります。

    @details    引数で与えた条件を元に、その条件が満たされるユニットヒープを作成するために必要なメモリサイズを計算し、取得します。@n
                アライメントには 2 のべき乗を指定してください。@n
                CreateUnitHeap() 関数でユニットヒープを作成する際に、ヒープの管理領域をヒープ内部に作成する場合は hasHeadInternally に true を指定してください。@n
                ヒープの管理領域をヒープ外に確保する場合は、 hasHeadInternally に false を指定してください。@n
                @n
                本 API はスレッドセーフです。
*/
size_t GetRequiredUnitHeapSize( size_t unitSize, int unitNum, int alignment, bool hasHeadInternally ) NN_NOEXCEPT;

//! @}

//! @name ユニットヒープ - デバッグ用 API
//! @{

/**
    @brief      ユニットヒープの内部の情報を表示します。（デバッグ用）

    @param[in]  heapHandle ヒープハンドル

    @pre        引数 heapHandle が CreateUnitHeap() 関数で事前に初期化されている必要があります。
    @post       ユニットヒープの内部の情報が出力されます。

    @details    ユニットヒープの以下の情報をダンプします。@n
                @li 各ユニットのアドレス範囲とサイズ
                @li ヒープ全体のアドレス範囲とサイズ
                @li ヒープの使用済み領域のサイズと使用比率
                @n
                CreateUnitHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void DumpUnitHeap( HeapHandle heapHandle ) NN_NOEXCEPT;

//! @}

}}  // nn::mem
