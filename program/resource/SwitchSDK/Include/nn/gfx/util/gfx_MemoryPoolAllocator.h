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

#include <nn/gfx.h>
#include <nn/mem/mem_NumberLineAllocator.h>


namespace nn {
namespace gfx {
namespace util {


//! @brief 大きなメモリプール上のメモリを小分けにして使用するためのアロケータです。
//! @details 管理領域用のメモリは、ユーザが指定したコールバック関数によって確保・解放されます。
class MemoryPoolAllocator
{
    NN_DISALLOW_COPY( MemoryPoolAllocator );
    NN_DISALLOW_MOVE( MemoryPoolAllocator );

public:
    //! @brief このクラスで管理するメモリユニットのサイズです。
    //! @details これより小さいサイズでアロケートしても、内部で AllocatorUnitSize サイズ分のメモリプールを消費します。
    static const size_t AllocatorUnitSize = (1 << 8);

    //! @brief このクラスで管理可能なユニット数の最大値です。
    static const size_t AllocatorUnitCountMax = 0x1000000;

    //! @brief このクラスで管理可能なメモリサイズの最大値です。
    static const size_t SizeMax = AllocatorUnitSize * (AllocatorUnitCountMax - 1);

    //! @brief このクラスで扱えるアライメントの最大値です。
    static const size_t AlignmentMax = (1 << 17);

    //! @brief 無効なオフセットを表す値です。
    static const ptrdiff_t InvalidOffset = -1;

    //! @brief 確保・解放に関するモードです。
    //! @details
    //! 区間の確保・解放時の挙動を指定します。@n
    //! デフォルトでは AllocationMode_NoOption が指定されています。
    enum AllocationMode {
        AllocationMode_NoOption = 0,            //!< 長さ 256 以上の区間が解放された場合、隣接する空き領域と領域を結合します
        AllocationMode_CoalesceAdjacentArea = 1 //!< 解放時に隣接する空き領域と常に結合します。 AllocationMode_NoOption に比べ実行効率が劣りますが、アロケータ内の断片化を抑えられる可能性があります
    };

    //! @brief デフォルトコンストラクタです
    MemoryPoolAllocator() NN_NOEXCEPT;

    //! @brief 初期化します。
    //! @param[in]    pAllocateFunction             管理領域の確保用コールバック関数です。
    //! @param[in]    pAllocateFunctionUserData     管理領域の確保時に呼ばれるユーザ定義のパラメータです。
    //! @param[in]    pFreeFunction                 管理領域の解放用コールバック関数です。
    //! @param[in]    pFreeFunctionUserData         管理領域の解放時に呼ばれるユーザ定義のパラメータです。
    //! @param[in]    pMemoryPool                   メモリプールです。
    //! @param[in]    baseOffset                    使用するメモリプールの先頭のオフセットです。
    //! @param[in]    size                          使用するメモリプールのサイズです。
    //! @param[in]    allocatableAlignmentMax       区間を割り当てるときに指定するアライメントの最大値です。
    //! @param[in]    isThreadSafe                  Allocate(), Free() をスレッドセーフで呼び出すようにするかどうかのフラグです。
    //! @pre IsInitialized() == false
    //! @pre pAllocateFunction は有効なコールバック関数のポインタでなければなりません。
    //! @pre pFreeFunction は有効なコールバック関数のポインタでなければなりません。
    //! @pre pMemoryPool は有効なメモリプールのポインタでなければなりません。
    //! @pre baseOffset >= 0
    //! @pre baseOffset % allocatableAlignmentMax == 0
    //! @pre size >= 1 && size <= SizeMax
    //! @pre allocatableAlignmentMax >= 1 && allocatableAlignmentMax <= AlignmentMax
    //! @pre allocatableAlignmentMax は 2 の累乗でなければなりません。
    //! @post IsInitialized() == true
    //! @details
    //! この関数はスレッドセーフではありません。
    void Initialize(
        nn::mem::MallocCallback pAllocateFunction,
        void* pAllocateFunctionUserData,
        nn::mem::FreeCallback pFreeFunction,
        void* pFreeFunctionUserData,
        nn::gfx::MemoryPool* pMemoryPool,
        ptrdiff_t baseOffset,
        size_t size,
        size_t allocatableAlignmentMax,
        bool isThreadSafe
        ) NN_NOEXCEPT;

    //! @brief 終了処理です。
    //! @pre IsInitialized() == true
    //! @post IsInitialized() == false
    //! @details
    //! この関数はスレッドセーフではありません。
    void Finalize() NN_NOEXCEPT;

    //! @brief このオブジェクトが初期化済かを返します。
    //! @return 初期化済の場合 true を返します。それ以外の場合 false を返します。
    //! @details
    //! Initialize() により初期化済になります。
    //! Finalize() により未初期化になります。
    bool IsInitialized() const NN_NOEXCEPT;

    //! @brief 区間を割り当てます。
    //! @param[in]    size       確保したい区間の長さです。
    //! @param[in]    alignment  確保する区間のアライメントです。
    //! @pre IsInitialized() == true
    //! @pre alignment >= 1 && alignment <= GetAllocatableAlignmentMax()
    //! @pre alignment は 2 の累乗でなければなりません。
    //! @return                  確保に成功した場合は割り当てられた区間の先頭オフセットを、そうでない場合は InvalidOffset を返します。
    //! @details
    //! Initialize() 時に isThreadSafe 引数に true を渡した場合、この関数はスレッドセーフです。@n
    //! isThreadSafe 引数に false を渡した場合は、スレッドセーフではありません。
    ptrdiff_t Allocate(size_t size, size_t alignment) NN_NOEXCEPT;

    //! @brief 割り当て済みの区間を解放します。
    //! @param[in]  offset  解放する区間の先頭オフセットです。
    //! @pre IsInitialized() == true
    //! @pre offset はアロケータから確保された区間の先頭オフセットである必要があります。
    //! @details
    //! Initialize() 時に isThreadSafe 引数に true を渡した場合、この関数はスレッドセーフです。@n
    //! isThreadSafe 引数に false を渡した場合は、スレッドセーフではありません。
    void Free(ptrdiff_t offset) NN_NOEXCEPT;

    //! @brief このオブジェクトに設定されているメモリプールを取得します。
    //! @pre IsInitialized() == true
    //! @return 設定されているメモリプールを返します。
    //! @details
    //! この関数はスレッドセーフです。
    nn::gfx::MemoryPool* GetMemoryPool() const NN_NOEXCEPT;

    //! @brief このオブジェクトが管理しているメモリ領域の先頭のオフセットを取得します。
    //! @pre IsInitialized() == true
    //! @return 管理しているメモリ領域の先頭のオフセットを返します。
    //! @details
    //! この関数はスレッドセーフです。
    ptrdiff_t GetBaseOffset() const NN_NOEXCEPT;

    //! @brief このオブジェクトが管理しているメモリ領域の大きさ（バイト）を取得します。
    //! @pre IsInitialized() == true
    //! @return 管理しているメモリ領域の大きさ（バイト）を返します。
    //! @details
    //! この関数はスレッドセーフです。
    size_t GetSize() const NN_NOEXCEPT;

    //! @brief このオブジェクトからメモリを確保する際に指定できる最大のアライメント（バイト）を取得します。
    //! @pre IsInitialized() == true
    //! @return メモリを確保する際に指定できる最大のアライメント（バイト）を返します。
    //! @details
    //! この関数はスレッドセーフです。
    size_t GetAllocatableAlignmentMax() const NN_NOEXCEPT;

    //! @brief 指定した割り当て済み区間のサイズを取得します。
    //! @pre IsInitialized() == true
    //! @pre offset はアロケータから確保された区間の先頭のオフセットである必要があります。
    //! @param[in]    offset       サイズを取得する割り当て済み区間の先頭のオフセットです。
    //! @return 指定した割り当て済み区間のサイズを返します。
    //! @details
    //! Initialize() 時に isThreadSafe 引数に true を渡した場合、この関数はスレッドセーフです。@n
    //! isThreadSafe 引数に false を渡した場合は、スレッドセーフではありません。
    size_t GetSizeOf(ptrdiff_t offset) NN_NOEXCEPT;

    //! @brief アロケータに存在する空き領域の合計を取得します。
    //! @pre IsInitialized() == true
    //! @return 空き領域の合計サイズを返します。
    //! @details
    //! Initialize() 時に isThreadSafe 引数に true を渡した場合、この関数はスレッドセーフです。@n
    //! isThreadSafe 引数に false を渡した場合は、スレッドセーフではありません。
    size_t GetTotalFreeSize() NN_NOEXCEPT;

    //! @brief アロケータから確保可能な最大サイズを取得します。
    //! @pre IsInitialized() == true
    //! @return 確保可能なサイズを返します。
    //! @details
    //! Initialize() 時に isThreadSafe 引数に true を渡した場合、この関数はスレッドセーフです。@n
    //! isThreadSafe 引数に false を渡した場合は、スレッドセーフではありません。
    size_t GetAllocatableSize() NN_NOEXCEPT;

    //! @brief 確保・解放時のモードを設定します。
    //! @param[in]  allocationMode  確保・解放時のモード
    //! @pre IsInitialized() == true
    //! @details
    //! デフォルトでは AllocationMode_NoOption が指定されています。@n
    //! Initialize() 時に isThreadSafe 引数に true を渡した場合、この関数はスレッドセーフです。@n
    //! isThreadSafe 引数に false を渡した場合は、スレッドセーフではありません。
    void SetAllocationMode(AllocationMode allocationMode) NN_NOEXCEPT;

    //! @brief 確保・解放時のモードを取得します。
    //! @pre IsInitialized() == true
    //! @return 確保・解放時のモードを返します。
    //! @details
    //! Initialize() 時に isThreadSafe 引数に true を渡した場合、この関数はスレッドセーフです。@n
    //! isThreadSafe 引数に false を渡した場合は、スレッドセーフではありません。
    AllocationMode GetAllocationMode() const NN_NOEXCEPT;

    //! @brief アロケータ内部の情報を表示します。
    //! @pre IsInitialized() == true
    //! @details
    //! 出力されるフォーマットの詳細については NintendoSDK ドキュメントを参照してください。
    void Dump() NN_NOEXCEPT;

private:
    nn::mem::NumberLineAllocator m_Impl;
    nn::gfx::MemoryPool* m_pMemoryPool;
    ptrdiff_t m_BaseOffset;
    size_t m_Size;
    size_t m_AllocatableAlignmentMax;
    bool m_IsInitialized;
};

} // namespace util
} // namespace gfx
} // namespace nn
