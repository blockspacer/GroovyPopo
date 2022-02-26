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
    @brief      フレームヒープのインターフェイスです。
    @details    フレームヒープ(FrameHeap)に関連する列挙型、構造体、関数を定義します。
*/

#include <nn/lmem/lmem_Common.h>

namespace nn { namespace lmem {

/**
    @brief      フレームヒープの状態を保存する構造体です。
    @details    フレームヒープが使用状況を記憶します。
*/
struct FrameHeapState
{
    void* nextBlockFront;  //!< 次に前方から確保した場合にメモリブロックの先頭アドレスになるアドレスです
    void* nextBlockRear;  //!< 次に後方から確保した場合にメモリブロックの先頭アドレスになるアドレスです
};

/**
    @brief      メモリブロックを解放する方法を指定するための定数です。
    @details    どちらの方向から確保されたメモリブロックを解放するか指定できます。
*/
enum FreeMode
{
    FreeMode_Front = (1 << 0),     //!< ヒープの前方から確保されているメモリブロックを全て解放します
    FreeMode_Rear  = (1 << 1),     //!< ヒープの後方から確保されているメモリブロックを全て解放します
    FreeMode_All   = (FreeMode_Front | FreeMode_Rear)    //!< ヒープ内の全てのメモリブロックを解放します
};

//! @name フレームヒープ - ヒープ領域操作 API
//! @{

/**
    @brief      フレームヒープを作成します。

    @param[in]  startAddress    ヒープに割り当てるメモリブロックの先頭アドレス
    @param[in]  size            ヒープに割り当てるメモリブロックのサイズ（バイト）
    @param[in]  option          デバッグ用オプション（CreationOption列挙型）

    @return     関数が成功するとフレームヒープハンドルのポインタが返ります。失敗すると NULL が返ります。

    @pre        startAddress は非 NULL である必要があります。
    @post       指定したメモリブロックにフレームヒープが作成されます。

    @details    ヒープ作成時にはヒープの管理領域（ヒープ共通ヘッダ）も作成されます。@n
                ヒープ管理領域は、ヒープ内部に作成されます。@n
                そのため、 size で指定したサイズからヒープ管理領域 ( nn::lmem::HeapCommonHead 構造体) のサイズを引いた領域が、フレームヒープで利用可能な領域となります。@n
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
                @n
                option に CreationOption_ThreadSafe を指定すると、マルチスレッド時の排他制御を有効にします。@n
                @n
                本 API はスレッドセーフではありません。
*/
HeapHandle  CreateFrameHeap( void* startAddress, size_t size, int option ) NN_NOEXCEPT;

/**
    @brief      フレームヒープを作成します。

    @param[in]  startAddress    ヒープに割り当てるメモリブロックの先頭アドレス
    @param[in]  size            ヒープに割り当てるメモリブロックのサイズ（バイト）
    @param[in]  option          デバッグ用オプション（CreationOption列挙型）
    @param[in]  pHeapHead       ヒープ共通ヘッダ

    @return     関数が成功するとフレームヒープハンドルのポインタが返ります。失敗すると NULL が返ります。

    @pre        startAddress は非 NULL である必要があります。
    @post       指定したメモリブロックにフレームヒープが作成されます。

    @details    ヒープ管理領域として、引数 pHeapHead で渡された領域を使用します。@n
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
                @n
                option に CreationOption_ThreadSafe を指定すると、マルチスレッド時の排他制御を有効にします。@n
                @n
                本 API はスレッドセーフではありません。
*/
HeapHandle CreateFrameHeap( void* startAddress, size_t size, int option, HeapCommonHead* pHeapHead ) NN_NOEXCEPT;

/**
    @brief      フレームヒープを破棄します。

    @param[in]  heapHandle  ヒープハンドル

    @pre        引数 heapHandle が CreateFrameHeap() 関数で事前に初期化されている必要があります。
    @post       指定したフレームヒープが破棄されます。

    @details    ヒープハンドルで指定されたヒープを破棄します。@n
                @n
                本 API はスレッドセーフではありません。
*/
void DestroyFrameHeap( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief      ヒープの空き領域を解放し、使用するメモリを縮小します。

    @param[in]  heapHandle  ヒープハンドル
    @param[in]  adjustMode  末尾から縮小するか、先頭から縮小するかを指定します。

    @return     ヒープが縮小されることにより空いたメモリ領域の範囲を返します。@n
                縮小できない場合の戻り値の MemoryRange の size は 0 になります。

    @pre        引数 heapHandle が CreateFrameHeap() 関数で事前に初期化されている必要があります。
    @post       フレームヒープが戻り値で示される分だけ縮小されます。

    @details    フレームヒープの空き領域を開放し、フレームヒープが使用するメモリを縮小します。@n
                AdjustMode で AdjustMode_Head を指定しており、かつ CreateFrameHeap() 関数を用いてヒープを作成する際にヒープ共通ヘッダをヒープ領域内に作成している場合は、本関数は失敗し 0 が返却されます。@n
                @n
                CreateFrameHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
MemoryRange AdjustFrameHeap( HeapHandle heapHandle, AdjustMode adjustMode ) NN_NOEXCEPT;

//! @}

//! @name フレームヒープ - メモリブロック操作 API

/**
    @brief      メモリ領域を確保します。

    @param[in]  heapHandle  ヒープハンドル
    @param[in]  size        確保するメモリのサイズ（バイト）

    @return     関数が成功すると確保したメモリ領域の先頭へのポインタ、失敗すると NULL ポインタが返ります。

    @pre        引数 heapHandle が CreateFrameHeap() 関数で事前に初期化されている必要があります。
    @post       戻り値が示すポインタから size バイトのメモリ領域が確保されます。

    @details    フレームヒープでメモリを確保します。@n
                アライメントは DefaultAlignment が利用されます。@n
                @n
                CreateFrameHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void* AllocateFromFrameHeap( HeapHandle heapHandle, size_t size ) NN_NOEXCEPT;

/**
    @brief      指定したアライメントでメモリ領域を確保します。

    @param[in]  heapHandle  ヒープハンドル
    @param[in]  size        確保するメモリのサイズ（バイト）
    @param[in]  alignment   確保するメモリのアラインメント

    @return     関数が成功すると確保したメモリ領域の先頭へのポインタ、失敗すると NULL ポインタが返ります。

    @pre        引数 heapHandle が CreateFrameHeap() 関数で事前に初期化されている必要があります。@n
                アライメントには正または負の 2 のべき乗が指定されている必要があります。
    @post       戻り値が示すポインタから size バイトのメモリ領域が確保されます。

    @details    フレームヒープでメモリを確保します。@n
                アライメントには正または負の 2 のべき乗を指定してください@n
                アライメントに負の値を指定すると、ヒープの後方からメモリの確保を行います。@n
                @n
                CreateFrameHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void* AllocateFromFrameHeap( HeapHandle heapHandle, size_t size, int alignment ) NN_NOEXCEPT;

/**
    @brief      メモリ領域を解放します。

    @param[in]  heapHandle  ヒープハンドル
    @param[in]  mode        メモリブロックの解放方法

    @pre        引数 heapHandle が CreateFrameHeap() 関数で事前に初期化されている必要があります。
    @post       指定した方法でメモリ領域が解放されます。

    @details    フレームヒープで確保したメモリ領域を解放します。@n
                @n
                CreateFrameHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void FreeToFrameHeap( HeapHandle heapHandle, FreeMode mode ) NN_NOEXCEPT;

/**
    @brief      フレームヒープから最後に確保したメモリブロックのサイズを変更します。

    @param[in]  heapHandle  ヒープハンドル
    @param[in]  pBlock      サイズを変更するメモリブロックの先頭アドレス
    @param[in]  newSize     変更後のメモリブロックのサイズ（バイト）

    @return     関数が成功すると、変更後のメモリブロックのサイズ(バイト)を返します。 関数が失敗すると、0 を返します。

    @pre        引数 heapHandle が CreateFrameHeap() 関数で事前に初期化されている必要があります。
    @post       引数 pBlock で指定したメモリブロックのサイズが newSize になります。

    @details    フレームヒープから最後に確保したメモリブロックのサイズを変更します。@n
                前方から確保したメモリブロックに対してのみ使用できます。@n
                @n
                CreateFrameHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
size_t ResizeFrameHeapBlock( HeapHandle heapHandle, void* pBlock, size_t newSize ) NN_NOEXCEPT;

//! @}

//! @name フレームヒープ - ヒープ情報設定・取得 API

/**
    @brief      確保可能なメモリブロックのサイズを取得します。

    @param[in]  heapHandle  ヒープハンドル
    @param[in]  alignment   アラインメント

    @return     フレームヒープの空き領域から確保可能なメモリブロックの最大サイズ(バイト)を返します。

    @pre        引数 heapHandle が CreateFrameHeap() 関数で事前に初期化されている必要があります。@n
                アライメントには正の 2 のべき乗が指定されている必要があります。
    @post       戻り値が拡張ヒープの空き領域から確保可能なメモリブロックの最大サイズ(バイト)となります。

    @details    フレームヒープで確保可能なメモリブロックのサイズを取得します。@n
                アライメントには正の 2 のべき乗を指定してください。@n
                @n
                CreateFrameHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
size_t GetFrameHeapAllocatableSize( HeapHandle heapHandle, int alignment ) NN_NOEXCEPT;

/**
    @brief      フレームヒープの状態を取得します。

    @param[in]  heapHandle  ヒープハンドル

    @return     現在のフレームヒープの状態を返します。

    @pre        引数 heapHandle が CreateFrameHeap() 関数で事前に初期化されている必要があります。
    @post       戻り値が現在のヒープの状態を表す FrameHeapState 構造体となります。

    @details    現在のフレームヒープの状態を返します。@n
                ここで得た FrameHeapState 構造体を RestoreFrameHeapState() 関数に引数として与えることで、現在のフレームヒープの状態を復元することができます。@n
                @n
                CreateFrameHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
FrameHeapState GetFrameHeapState( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief      フレームヒープの状態を復元します。

    @param[in]  heapHandle  ヒープハンドル
    @param[in]  state       復元したいフレームヒープの状態

    @pre        引数 heapHandle が CreateFrameHeap() 関数で事前に初期化されている必要があります。@n
                復元したい pState の状態から本関数を呼ぶまでの間、 FreeToFrameHeap() 関数が一度も呼ばれていない必要があります。
    @post       フレームヒープが pState の状態に戻ります。

    @details    pState を元にフレームヒープの状態を復元します。@n
                FreeToFrameHeap() 関数を呼んでメモリを解放してしまった場合、解放前の状態に復元することはできません。@n
                @n
                CreateFrameHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void RestoreFrameHeapState( HeapHandle heapHandle, const FrameHeapState& state ) NN_NOEXCEPT;

//! @}

//! @name フレームヒープ - デバッグ用 API
//! @{

/**
    @brief      フレームヒープの内部の情報を表示します。（デバッグ用）

    @param[in]  heapHandle ヒープハンドル

    @pre        引数 heapHandle が CreateFrameHeap() 関数で事前に初期化されている必要があります。
    @post       フレームヒープの内部の情報が出力されます。

    @details    フレームヒープの以下の情報をダンプします。@n
                @li 前方から確保されているメモリ領域のアドレス範囲
                @li 後方から確保されているメモリ領域のアドレス範囲
                @li ヒープ全体のサイズ
                @li ヒープの使用済み領域のサイズ
                @n
                CreateFrameHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
                そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void DumpFrameHeap( HeapHandle heapHandle ) NN_NOEXCEPT;

//! @}

}}
