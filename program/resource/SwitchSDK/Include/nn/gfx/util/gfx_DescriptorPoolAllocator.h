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


//! @brief デスクリプタプール上のスロットを小分けにして使用するためのアロケータです。
//! @details 管理領域用のメモリは、ユーザが指定したコールバック関数によって確保・解放されます。
class DescriptorPoolAllocator
{
    NN_DISALLOW_COPY( DescriptorPoolAllocator );
    NN_DISALLOW_MOVE( DescriptorPoolAllocator );

public:
    //! @brief このクラスで管理可能なスロットの最大数です。
    static const int SlotCountMax = 0x1000000;

    //! @brief 無効なインデックスを表す値です。
    static const int InvalidIndex = -1;

    //! @brief デフォルトコンストラクタです
    DescriptorPoolAllocator() NN_NOEXCEPT;

    //! @brief 初期化します。
    //! @param[in]    pAllocateFunction             管理領域の確保用コールバック関数です。
    //! @param[in]    pAllocateFunctionUserData     管理領域の確保時に呼ばれるユーザ定義のパラメータです。
    //! @param[in]    pFreeFunction                 管理領域の解放用コールバック関数です。
    //! @param[in]    pFreeFunctionUserData         管理領域の解放時に呼ばれるユーザ定義のパラメータです。
    //! @param[in]    pDescriptorPool               デスクリプタプールです。
    //! @param[in]    baseSlotIndex                 使用するデスクリプタスロットの先頭のインデックスです。
    //! @param[in]    slotCount                     使用するデスクリプタスロットの数です。
    //! @param[in]    isThreadSafe                  Allocate(), Free() をスレッドセーフで呼び出すようにするかどうかのフラグです。
    //! @pre IsInitialized() == false
    //! @pre pAllocateFunction は有効なコールバック関数のポインタでなければなりません。
    //! @pre pFreeFunction は有効なコールバック関数のポインタでなければなりません。
    //! @pre pDescriptorPool は有効なデスクリプタプールのポインタでなければなりません。
    //! @pre baseSlotIndex >= 0
    //! @pre slotCount >= 1 && slotCount <= SlotCountMax
    //! @post IsInitialized() == true
    //! @details
    //! この関数はスレッドセーフではありません。
    void Initialize(
        nn::mem::MallocCallback pAllocateFunction,
        void* pAllocateFunctionUserData,
        nn::mem::FreeCallback pFreeFunction,
        void* pFreeFunctionUserData,
        nn::gfx::DescriptorPool* pDescriptorPool,
        int baseSlotIndex,
        int slotCount,
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
    //! @param[in]    count  確保したい区間の長さです。
    //! @pre IsInitialized() == true
    //! @pre count >= 0
    //! @return              確保に成功した場合は割り当てられた区間の先頭インデックスを、そうでない場合は InvalidIndex を返します。
    //! @details
    //! Initialize() 時に isThreadSafe 引数に true を渡した場合、この関数はスレッドセーフです。@n
    //! isThreadSafe 引数に false を渡した場合は、スレッドセーフではありません。
    int Allocate(int count) NN_NOEXCEPT;

    //! @brief 割り当て済みの区間を解放します。
    //! @param[in]  indexSlot  解放する区間の先頭インデックスです。
    //! @pre IsInitialized() == true
    //! @pre indexSlot はアロケータから確保された区間の先頭インデックスである必要があります。
    //! @details
    //! Initialize() 時に isThreadSafe 引数に true を渡した場合、この関数はスレッドセーフです。@n
    //! isThreadSafe 引数に false を渡した場合は、スレッドセーフではありません。
    void Free(int indexSlot) NN_NOEXCEPT;

    //! @brief このオブジェクトに設定されているデスクリプタプールを取得します。
    //! @pre IsInitialized() == true
    //! @return 設定されているデスクリプタプールを返します。
    //! @details
    //! この関数はスレッドセーフです。
    nn::gfx::DescriptorPool* GetDescriptorPool() const NN_NOEXCEPT;

    //! @brief このオブジェクトが管理しているデスクリプタスロットの先頭のインデックスを取得します。
    //! @pre IsInitialized() == true
    //! @return 管理しているデスクリプタスロットの先頭のインデックスを返します。
    //! @details
    //! この関数はスレッドセーフです。
    int GetBaseSlotIndex() const NN_NOEXCEPT;

    //! @brief このオブジェクトが管理しているデスクリプタスロットの数を取得します。
    //! @pre IsInitialized() == true
    //! @return 管理しているデスクリプタスロットの数を返します。
    //! @details
    //! この関数はスレッドセーフです。
    int GetSlotCount() const NN_NOEXCEPT;

    //! @brief 指定した割り当て済み区間のサイズを取得します。
    //! @pre IsInitialized() == true
    //! @pre indexSlot はアロケータから確保された区間の先頭のインデックスである必要があります。
    //! @param[in]    indexSlot       サイズを取得する割り当て済み区間の先頭のインデックスです。
    //! @return 指定した割り当て済み区間のサイズを返します。
    //! @details
    //! Initialize() 時に isThreadSafe 引数に true を渡した場合、この関数はスレッドセーフです。@n
    //! isThreadSafe 引数に false を渡した場合は、スレッドセーフではありません。
    int GetSizeOf(int indexSlot) NN_NOEXCEPT;

    //! @brief アロケータに存在する空き領域の合計を取得します。
    //! @pre IsInitialized() == true
    //! @return 空き領域の合計サイズを返します。
    //! @details
    //! Initialize() 時に isThreadSafe 引数に true を渡した場合、この関数はスレッドセーフです。@n
    //! isThreadSafe 引数に false を渡した場合は、スレッドセーフではありません。
    int GetTotalFreeSize() NN_NOEXCEPT;

    //! @brief アロケータから確保可能な最大サイズを取得します。
    //! @pre IsInitialized() == true
    //! @return 確保可能なサイズを返します。
    //! @details
    //! Initialize() 時に isThreadSafe 引数に true を渡した場合、この関数はスレッドセーフです。@n
    //! isThreadSafe 引数に false を渡した場合は、スレッドセーフではありません。
    int GetAllocatableSize() NN_NOEXCEPT;

    //! @brief アロケータ内部の情報を表示します。
    //! @pre IsInitialized() == true
    //! @details
    //! 出力されるフォーマットの詳細については NintendoSDK ドキュメントを参照してください。
    void Dump() NN_NOEXCEPT;

private:
    nn::mem::NumberLineAllocator m_Impl;
    nn::gfx::DescriptorPool* m_pDescriptorPool;
    int m_BaseSlotIndex;
    int m_SlotCount;
    bool m_IsInitialized;
};

} // namespace util
} // namespace gfx
} // namespace nn
