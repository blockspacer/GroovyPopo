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
    @brief 各ヒープに共通する定数、関数の定義です。
    @details 各ヒープで共通して利用される定数、列挙型、構造体、関数を定義します。
*/

#include <cstdlib>
#include <nn/nn_SdkLog.h>
#include <nn/nn_SdkAssert.h>
#include <nn/nn_Common.h>

#include <nn/lmem/detail/lmem_DetailCommonHead.h>

/**
    @namespace nn::lmem
    @brief Cafe SDK, CTR-SDK と同等機能を持つ動的メモリ確保ライブラリの名前空間です。
    @details Cafe SDK, CTR-SDK と同等機能を持つヒープは、この名前空間に用意されます。@n
             Cafe SDK と CTR-SDK にない、新しく用意されたヒープは nn::mem 名前空間に配置されます。
*/
namespace nn { namespace lmem {

/**
    @brief メモリ確保の際のデフォルトアラインメントです。
    @details メモリ確保の際にデフォルトで指定されるアライメントです。
*/
const int DefaultAlignment = 8;

/**
    @brief メモリ領域の範囲を表す構造体です。
    @details メモリ領域のアドレスとサイズを表します。
*/
struct MemoryRange
{
    uintptr_t begin; //!< メモリ領域の先頭アドレスを表します
    size_t size;     //!< メモリ領域のサイズを表します
};

/**
    @brief      ヒープ作成時のオプションを表す定数です。
    @details    拡張ヒープ、フレームヒープ、ユニットヒープそれぞれのヒープ作成関数（Create***Heap）のオプションとして指定されます。
*/
enum CreationOption
{
    CreationOption_NoOption = 0,            //!< オプションを指定しません
    CreationOption_ZeroClear = (1 << 0),    //!< メモリ確保時にメモリを 0 でフィルします。このオプションはデバッグ用で、製品版のコードでは無効になります
    CreationOption_DebugFill = (1 << 1),    //!< ヒープ作成時、メモリ確保、メモリ解放時にメモリを特定の値でフィルします。詳細については各ヒープのヒープ作成 API の仕様を参照してください。このオプションはデバッグ用で、製品版のコードでは無効になります
    CreationOption_ThreadSafe = (1 << 2)    //!< スレッド間の排他制御を有効にします
};

/**
    @brief エラーに関するオプションを表す定数です。
    @details エラー出力の有無を指定できます。
*/
enum ErrorOption
{
    ErrorOption_NoOption = 0,       //!< オプションを指定しません
    ErrorOption_Print = (1 << 0)    //!< エラー出力を有効にします
};

/**
    @brief ヒープをフィルする種別を表す定数です。
    @details メモリをフィルするタイミングを表します。
*/
enum FillType
{
    FillType_Unallocated,   //!< 未使用のメモリにフィルします（ヒープ作成時にフィルします）
    FillType_Allocate,      //!< メモリを確保する際にフィルします
    FillType_Free,          //!< メモリを解放する際にフィルします
    FillType_Max
};

/**
    @brief ヒープ領域を縮小する種別を表す定数です。
    @details ヒープ領域を縮小する方向を指定できます。
*/
enum AdjustMode
{
    AdjustMode_Tail = 1,    //!< ヒープのメモリ領域を末尾から先頭に向かって縮めます
    AdjustMode_Head = -1    //!< ヒープのメモリ領域を先頭から末尾に向かって縮めます
};

namespace detail {
    struct HeapHead;
}

/**
    @brief ヒープ共通ハンドルです。
    @details ヒープのハンドルを表す型です。
*/
typedef struct detail::HeapHead* HeapHandle;

/**
    @brief      ヒープ共通ヘッダです。
    @details    ヒープの管理領域です。@n
                ヒープではヒープ共通ヘッダが必ず作成されます。@n
                ヒープ領域内にヒープ共通ヘッダ領域を作成したくない場合は、この型を別領域に確保しヒープ作成時に渡すことで、ヒープ領域内にヘッダが作成されなくなります。
*/
typedef detail::HeapHead HeapCommonHead;

//! @name ヒープ共通 API
//! @{

/**
    @brief ヒープをフィルする種別毎にフィルする値を設定します。

    @param[in] type     ヒープをフィルする種別
    @param[in] value    ヒープをフィルする値
    @details フィルは指定された 32bit 値、 4 バイト単位で行われます。@n
             フィルする値は全てのヒープで共通の値が利用されます。@n
             @n
             本 API はスレッドセーフではありません。@n
             本 API を呼び出す場合は、他のヒープ API を同時に呼び出さないでください。
*/
void SetFillValue( FillType type, Bit32 value ) NN_NOEXCEPT;

/**
    @brief ヒープをフィルする値を取得します。

    @param[in] type ヒープをフィルする種別
    @return 指定したフィルの種別で使われる値が返ります。
    @details FillType 列挙型を指定し、フィルが行われる際にどのような値がフィルされるのか取得します。@n
             @n
             本 API はスレッドセーフではありません。@n
             本 API を呼び出す場合は、 SetFillValue() 関数を同時に呼び出さないでください。
*/
Bit32 GetFillValue( const FillType type ) NN_NOEXCEPT;

/**
    @brief ヒープが利用しているメモリ領域のサイズを取得します。

    @param[in] heapHandle ヒープハンドル
    @return ヒープが利用しているメモリ領域のサイズ(バイト)を返します。
    @pre 引数 heapHandle が事前に初期化されている必要があります。
    @post 戻り値がヒープが利用しているメモリ領域のサイズ(バイト)となります。
    @details ヒープハンドルから、ヒープが利用しているメモリ領域のサイズを取得します。@n
             ヒープの管理領域がヒープ領域内に作成されている場合は、それも含みます。@n
             @n
             ヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。

*/
size_t GetTotalSize( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief ヒープが利用しているメモリ領域の先頭アドレスを返します。

    @param[in] heapHandle ヒープハンドル
    @return ヒープが利用しているメモリ領域のの先頭アドレスを返します。
    @pre 引数 heapHandle が事前に初期化されている必要があります。
    @post 戻り値がヒープが利用しているメモリ領域の先頭アドレスとなります。
    @details ヒープハンドルから、ヒープが利用しているメモリ領域の先頭アドレスを取得します。@n
             @n
             ヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void *GetStartAddress( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief      指定したアドレスがヒープに含まれているか調べます。

    @param[in]  heapHandle  ヒープハンドル
    @param[in]  address     調べる対象のアドレス

    @return     指定したアドレスがヒープ領域内なら、 true を返します。

    @pre        引数 heapHandle が事前に初期化されている必要があります。
    @post       指定したアドレスがヒープ領域内なら、 true が返ります。

    @details    指定したアドレスがヒープに含まれているか調べます。@n
                @n
                ヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
bool HasAddress( HeapHandle heapHandle, void* address ) NN_NOEXCEPT;

//! @}

}}
