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
    @brief 拡張ヒープのインターフェイスです。
    @details 拡張ヒープ(ExpHeap)に関連する列挙型、関数を定義します。
*/

#include <nn/lmem/lmem_Common.h>

namespace nn { namespace lmem {

/**
    @brief メモリブロック確保のモードです。
    @details メモリブロック確保の際にどのような方法でブロックを見つけるか決定します。
*/
enum AllocationMode
{
    AllocationMode_FirstFit = 0,    //!< 確保しようとしているメモリブロックのサイズ以上の大きさを持つ、最初に見つかった空き領域からメモリブロックを確保します
    AllocationMode_BestFit  = 1     //!< 確保しようとしているメモリブロックのサイズに一番近いサイズの空き領域を探し、その空き領域からメモリブロックを確保します
};

/**
    @brief メモリブロック確保の方向を指定するための定数です。
    @details メモリブロック確保は、ヒープの前方からか、後方からか選択できます。
*/
enum AllocationDirection
{
    AllocationDirection_Front = 0,  //!< メモリブロックの確保時に、ヒープの空き領域の前方から確保します
    AllocationDirection_Rear  = 1   //!< メモリブロックの確保時に、ヒープの空き領域の後方から確保します
};

/**
    @brief メモリブロックを巡るときに呼び出されるコールバック関数の型です。

    @param[in] pBlock       メモリブロックへのポインタ
    @param[in] heapHandle   メモリブロックを持つヒープ
    @param[in] userParam    ユーザー用パラメータ

    @details VisitExpHeapAllBlocks() 関数で利用されます。
*/
typedef void (*HeapVisitor)( void* pBlock, HeapHandle heapHandle, uintptr_t userParam );

//! @name 拡張ヒープ - ヒープ領域操作 API
//! @{

/**
    @brief 拡張ヒープを作成します。

    @param[in]  startAddress ヒープに割り当てるメモリブロックの先頭アドレス
    @param[in]  size ヒープに割り当てるメモリブロックのサイズ（バイト）
    @param[in]  option デバッグ用オプション（CreationOption列挙型）

    @return 関数が成功すると拡張ヒープのハンドルのポインタが返ります。失敗すると NULL が返ります。

    @pre startAddress は非 NULL である必要があります。
    @post 指定したメモリブロックに拡張ヒープが作成されます。

    @details ヒープ作成時にはヒープの管理領域（ヒープ共通ヘッダ）も作成されます。@n
             ヒープ管理領域は、ヒープ内部に作成されます。@n
             そのため、 size で指定したサイズからヒープ管理領域 ( nn::lmem::HeapCommonHead 構造体) のサイズを引いた領域が、フレームヒープで利用可能な領域となります。@n
             @n
             メモリ確保の際、確保のモードはデフォルトでは AllocationMode_FirstFit が利用されます。@n
             確保のモードは SetExpHeapAllocationMode() 関数を利用することで変更できます。@n
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
HeapHandle CreateExpHeap( void* startAddress, size_t size, int option ) NN_NOEXCEPT;

/**
    @brief 拡張ヒープを破棄します。

    @param[in] heapHandle ヒープハンドル

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 指定した拡張ヒープが破棄されます。

    @details ヒープハンドルで指定されたヒープを破棄します。@n
             本 API はスレッドセーフではありません。
*/
void DestroyExpHeap( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief ヒープ領域の後方にある空き領域を開放し、使用するメモリを縮小します。

    @param[in] heapHandle ヒープハンドル

    @return ヒープが縮小されることにより空いたメモリ領域の範囲を返します。@n
            縮小できない場合の戻り値の MemoryRange の size は 0 になります。

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 拡張ヒープが戻り値で示される分だけ縮小されます。

    @details ヒープ領域の後ろから確保されたメモリブロックが存在している場合、この関数は失敗します。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
MemoryRange AdjustExpHeap( HeapHandle heapHandle ) NN_NOEXCEPT;

//! @}

//! @name 拡張ヒープ - メモリブロック操作 API
//! @{

/**
    @brief メモリ領域を確保します。

    @param[in] heapHandle ヒープハンドル
    @param[in] size 確保するメモリのサイズ（バイト）

    @return 関数が成功すると確保したメモリ領域の先頭へのポインタ、失敗すると NULL ポインタが返ります。

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 戻り値が示すポインタから size バイトのメモリ領域が確保されます。

    @details 拡張ヒープでメモリを確保します。@n
             アライメントは DefaultAlignment が利用されます。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void* AllocateFromExpHeap( HeapHandle heapHandle, size_t size ) NN_NOEXCEPT;

/**
    @brief 指定したアラインメントでメモリ領域を確保します。

    @param[in] heapHandle ヒープハンドル
    @param[in] size 確保するメモリのサイズ（バイト）
    @param[in] alignment 確保するメモリのアラインメント

    @return 関数が成功すると確保したメモリ領域の先頭へのポインタ、失敗すると NULL ポインタが返ります。

    @pre    引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。@n
            アライメントには正または負の 2 のべき乗が指定されている必要があります。
    @post   戻り値が示すポインタから size バイトのメモリ領域が確保されます。

    @details 拡張ヒープでメモリを確保します。@n
             アライメントには正または負の 2 のべき乗を指定してください。@n
             アライメントに負の値を指定すると、ヒープの後方からメモリの確保を行います。@n
             @n
             アライメントによって発生したメモリの隙間は、デフォルトでは再利用されない設定になっています。@n
             SetExpHeapUseMarginOfAlignment() 関数を使用して、このアライメントの隙間を空き領域として再利用する設定に変更できます。@n
             ただし、 32 を超える値または -32 を下回る値をアライメント値として指定してメモリを確保すると、メモリブロック間に隙間が生じたときに小さな空きブロックが登録され、パフォーマンスに影響を与える可能性がありますのでご注意ください。
             また、128 を超える値または -128 を下回る値をアライメント値として指定した場合、空き領域はSetExpHeapUseMarginOfAlignment() 関数の設定に関係なく、自動的に再利用されます。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void* AllocateFromExpHeap( HeapHandle heapHandle, size_t size, int alignment ) NN_NOEXCEPT;

/**
    @brief メモリ領域を解放します。

    @param[in] heapHandle ヒープハンドル
    @param[in] pBlock 解放するするメモリブロックの先頭アドレス

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 指定したメモリ領域が解放されます。

    @details 拡張ヒープで確保したメモリ領域を解放します。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void FreeToExpHeap( HeapHandle heapHandle, void* pBlock ) NN_NOEXCEPT;

/**
    @brief 拡張ヒープから確保したメモリブロックのサイズを変更します。

    @param[in] heapHandle ヒープハンドル
    @param[in] pBlock サイズを変更するメモリブロックの先頭アドレス
    @param[in] newSize  変更後のメモリブロックのサイズ（バイト）

    @return 関数が成功すると、変更後のメモリブロックのサイズ(バイト)を返します。 関数が失敗すると、0 を返します。

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 引数 pBlock で指定したメモリブロックのサイズが newSize になります。

    @details メモリブロックを現在の大きさより大きくするとき、メモリブロックの後方にサイズを拡張するのに十分な空き領域が存在する必要があります。十分な空き領域が存在しない場合は、関数は失敗し 0 を返します。メモリブロックのサイズを大きくすることができた場合、メモリブロックの大きさが要求したサイズよりも大きくなる場合があります。@n
             メモリブロックを今の大きさより小さくするとき、小さくするサイズが数バイト程度と少ない場合は空き領域が作成できず、縮小できない場合があります。その場合は、現在のメモリブロックのサイズが返されます。@n
             @n
             本 API は C 言語標準ライブラリの realloc 関数のように、サイズを変更したメモリブロックを新たなアドレスに割り当てるものではありません。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
size_t ResizeExpHeapBlock( HeapHandle heapHandle, void* pBlock, size_t newSize ) NN_NOEXCEPT;

//! @}

//! @name 拡張ヒープ - ヒープ情報設定・取得 API
//! @{

/**
    @brief 空きメモリサイズを取得します。

    @param[in] heapHandle ヒープハンドル

    @return 拡張ヒープの空きメモリの合計サイズ(バイト)を返します。

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 戻り値が拡張ヒープの空きメモリの合計サイズ(バイト)となります。

    @details 拡張ヒープの空きメモリの合計サイズ(バイト)を取得します。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
size_t GetExpHeapTotalFreeSize( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief 確保可能なメモリブロックのサイズを取得します。

    @param[in] heapHandle ヒープハンドル
    @param[in] alignment アラインメント

    @return 拡張ヒープの空き領域から確保可能なメモリブロックの最大サイズ(バイト)を返します。

    @pre    引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。@n
            アライメントには正の 2 のべき乗が指定されている必要があります。
    @post   戻り値が拡張ヒープの空き領域から確保可能なメモリブロックの最大サイズ(バイト)となります。

    @details 拡張ヒープで確保可能なメモリブロックのサイズを取得します。@n
             アライメントには正の 2 のべき乗を指定してください。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
size_t GetExpHeapAllocatableSize( HeapHandle heapHandle, int alignment ) NN_NOEXCEPT;

/**
    @brief メモリ確保時のモードを設定します。

    @param[in] heapHandle ヒープハンドル
    @param[in] allocationMode メモリ確保時のモード

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 拡張ヒープにメモリ確保時のモードが設定されます。

    @details デフォルトでは AllocationMode_FirstFit が指定されています。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void SetExpHeapAllocationMode( HeapHandle heapHandle, AllocationMode allocationMode ) NN_NOEXCEPT;

/**
    @brief メモリ確保時のモードを取得します。

    @param[in] heapHandle ヒープハンドル

    @return 拡張ヒープのメモリ確保時のモードを返します。

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 戻り値が拡張ヒープのメモリ確保時のモードとなります。

    @details CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
AllocationMode GetExpHeapAllocationMode( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief メモリ確保の際に、アラインメントによって発生した断片化領域を再利用するかどうかを設定します。

    @param[in] heapHandle ヒープハンドル
    @param[in] reuse アラインメントの際に発生する隙間の領域を再利用の有無

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 拡張ヒープに指定した設定が反映されます。

    @details デフォルトでは false に設定されています。@n
             true に設定した場合には小さなメモリ領域が有効に使用できる可能性がありますが、大量のフリーブロックが生成されメモリ確保時のパフォーマンスが悪化する危険性があります。@n
             ただし、128 バイト以上の比較的大きなアライメントの断片化領域は、本関数の設定に関係なく自動的に再利用されます。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void SetExpHeapUseMarginOfAlignment( HeapHandle heapHandle, bool reuse ) NN_NOEXCEPT;

/**
    @brief メモリ確保の際にアラインメントによって発生した断片化領域を再利用するかどうかを取得します。

    @param[in] heapHandle ヒープハンドル

    @return アラインメントの際に発生する隙間の領域の再利用をする場合は true、しない場合は false が返ります。

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 戻り値として拡張ヒープがアラインメントの際に発生する隙間の領域を再利用するかどうかが返ります。

    @details デフォルトでは false に設定されています。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
bool GetExpHeapUseMarginOfAlignment( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief メモリ確保時のグループ ID を設定します。

    @param[in] heapHandle ヒープハンドル
    @param[in] groupId グループ ID

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。@n
         groupId には 0 から 255 までの正の整数を指定する必要があります。
    @post 拡張ヒープにグループ ID が設定されます。

    @details 拡張ヒープのグループ ID を設定します。@n
             AllocateFromExpHeap() 関数でメモリを確保する際に、各メモリブロックにも拡張ヒープのグループ ID と同様の値が設定されます。@n
             拡張ヒープに設定されたグループ ID は GetExpHeapGroupId() 関数で、各メモリブロックに設定されたグループ ID は GetExpHeapGroupIdOfBlock() 関数で確認することができます。@n
             グループ ID のデフォルト値は 0 です。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void SetExpHeapGroupId( HeapHandle heapHandle, Bit16 groupId ) NN_NOEXCEPT;

/**
    @brief グループ ID を取得します。

    @param[in] heapHandle ヒープハンドル

    @return 拡張ヒープのグループ ID が返ります。

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 戻り値が拡張ヒープのグループ ID となります。

    @details 各メモリブロックに設定されたグループ ID は GetExpHeapGroupIdOfBlock() 関数で確認することができます。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
Bit16 GetExpHeapGroupId( HeapHandle heapHandle ) NN_NOEXCEPT;

/**
    @brief 確保したメモリブロックのサイズを取得します。

    @param[in] pBlock メモリブロックの先頭アドレス

    @return 拡張ヒープから確保したメモリブロックのサイズを返します。ここで返される値は、アラインメントなどの制約により、確保時のサイズより大きい値を返すことがあります。

    @pre 引数 pBlock で指定するメモリブロックが拡張ヒープによって確保されている必要があります。
    @post 戻り値が拡張ヒープから確保したメモリブロックのサイズとなります。

    @details 拡張ヒープから確保したメモリブロックのサイズを取得します。@n
             @n
             本 API はスレッドセーフではありません。@n
             本 API を呼び出す場合は、pBlock の領域を含むヒープに対して、以下の API を同時に呼び出さないでください。
                @li DestroyExpHeap()
                @li FreeToExpHeap()
                @li ResizeExpHeapBlock()
*/
size_t GetExpHeapBlockSize( const void* pBlock ) NN_NOEXCEPT;

/**
    @brief 確保したメモリブロックのグループ ID を取得します。

    @param[in] pBlock メモリブロックの先頭アドレス

    @return 拡張ヒープから確保したメモリブロックのグループ ID を返します。

    @pre 引数 pBlock で指定するメモリブロックが拡張ヒープによって確保されている必要があります。
    @post 戻り値が拡張ヒープから確保したメモリブロックのグループ ID となります。

    @details 拡張ヒープから確保したメモリブロックのグループ ID を取得します。@n
             @n
             本 API はスレッドセーフではありません。@n
             本 API を呼び出す場合は、pBlock の領域を含むヒープに対して、以下の API を同時に呼び出さないでください。
                @li DestroyExpHeap()
                @li SetExpHeapGroupId()
                @li FreeToExpHeap()
*/
Bit16 GetExpHeapGroupIdOfBlock( const void* pBlock ) NN_NOEXCEPT;

/**
    @brief 確保したメモリブロックのメモリ確保の方向を取得します。

    @param[in] pBlock メモリブロックの先頭アドレス

    @return 拡張ヒープから確保したメモリブロックのメモリ確保の方向を返します。

    @pre 引数 pBlock で指定するメモリブロックが拡張ヒープによって確保されている必要があります。
    @post 戻り値が拡張ヒープから確保したメモリブロックのメモリ確保の方向となります。

    @details 拡張ヒープから確保したメモリブロックのメモリ確保の方向を取得します。@n
             @n
             本 API はスレッドセーフではありません。@n
             本 API を呼び出す場合は、pBlock の領域を含むヒープに対して、以下の API を同時に呼び出さないでください。
                @li DestroyExpHeap()
                @li FreeToExpHeap()
*/
AllocationDirection GetExpHeapAllocationDirectionOfBlock( const void* pBlock ) NN_NOEXCEPT;

//! @}

//! @name 拡張ヒープ - デバッグ用 API
//! @{

/**
    @brief 拡張ヒープが破壊されていないかどうかをチェックします。（デバッグ用）

    @param[in] heapHandle ヒープハンドル
    @param[in] option オプション（ErrorOption列挙型）

    @return チェックが成功すると true、失敗するとは false を返します。

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 戻り値にチェックの結果が返ります。

    @details option に ErrorOption_Print を指定すると、チェック結果のエラー出力が有効になります。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
bool CheckExpHeap( HeapHandle heapHandle, int option ) NN_NOEXCEPT;

/**
    @brief 拡張ヒープから確保したメモリブロックのチェックを行います。（デバッグ用）

    option に ErrorOption_Print を指定すると、チェック結果のエラー出力が有効になります。

    @param[in] heapHandle ヒープハンドル
    @param[in] pBlock メモリブロックの先頭アドレス
    @param[in] option オプション（ErrorOption列挙型）

    @return チェックが成功すると true、失敗すると false を返します。

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 戻り値にチェックの結果が返ります。

    @details option に ErrorOption_Print を指定すると、チェック結果のエラー出力が有効になります。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
bool CheckExpHeapBlock( HeapHandle heapHandle, const void* pBlock, int option ) NN_NOEXCEPT;

/**
    @brief 確保した全てのメモリブロックに対し、ユーザが指定した関数 visitor を呼びます。（デバッグ用）

    @param[in] heapHandle ヒープハンドル
    @param[in] visitor 各メモリブロックに対して呼ぶ関数
    @param[in] userParam visitor 関数に渡す引数

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 各メモリブロックに対して指定した関数が実行されます。

    @details 拡張ヒープから割り当てられたメモリブロック全てに対して、ユーザが指定した関数を呼ばせます。@n
             visitor 関数で呼ばれるメモリブロックの順番は、確保した順番になります。@n
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void VisitExpHeapAllBlocks( HeapHandle heapHandle, HeapVisitor visitor, uintptr_t userParam ) NN_NOEXCEPT;

/**
    @brief 拡張ヒープの内部の情報を表示します。（デバッグ用）

    @param[in] heapHandle ヒープハンドル

    @pre 引数 heapHandle が CreateExpHeap() 関数で事前に初期化されている必要があります。
    @post 拡張ヒープの内部の情報が出力されます。

    @details 拡張ヒープの以下の情報をダンプします。@n
             @li 各ブロックが前から確保されているか後ろから確保されているか
             @li 各ブロックのアドレス
             @li 各ブロックのサイズ
             @li 各ブロックのグループ ID
             @li 各ブロックのアライメント
             @li 各ブロックが指す次のブロックのアドレス
             @li 各ブロックが指す前のブロックのアドレス
             @li ヒープ全体のサイズ
             @li ヒープの使用済み領域のサイズ
             @n
             CreateExpHeap() 関数でのヒープ作成時に CreationOption_ThreadSafe をオプションに指定して作成されたヒープの場合、本 API はスレッドセーフになります。@n
             そうでない場合、同一の heapHandle で扱われるヒープに対して行う操作は、スレッドセーフではありません。
*/
void DumpExpHeap( HeapHandle heapHandle ) NN_NOEXCEPT;

//! @}

}}
