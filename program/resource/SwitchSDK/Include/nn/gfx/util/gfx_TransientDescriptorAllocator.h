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

#include <nn/nn_Common.h>
#include <nn/gfx.h>

#include "detail/gfx_IndexRange.h"
#include "detail/gfx_IndexRingBuffer.h"

namespace nn{ namespace gfx{ namespace util{

    //! @brief 一時的に使用するデスクリプタスロットを管理するデスクリプタプールです。
    //! @details
    //! 描画処理で一時的に使用するデスクリプタスロットを動的に確保し、
    //! 描画の完了後にまとめて解放するための機能を提供します。
    class TransientDescriptorAllocator
    {
        NN_DISALLOW_COPY(TransientDescriptorAllocator);

    public:
        //! @brief このクラスで管理可能なスロットの最大数です。
        static const int SlotCountMax = detail::IndexRingBuffer::IndexCountMax;

        //! @brief 無効なインデックスを表す値です。
        //! @details
        //! デスクリプタスロットのインデックスの確保に失敗した場合、この値が返されます。
        static const int InvalidIndex = detail::IndexRingBuffer::InvalidIndex;
    public:
        //! @brief デフォルトコンストラクタです
        TransientDescriptorAllocator() NN_NOEXCEPT;

        //! @brief デスクリプタプールとスロットの範囲を指定して初期化します。
        //! @param[in] pDescriptorPool デスクリプタプールです。
        //! @param[in] baseSlotIndex   使用するデスクリプタスロットの先頭のインデックスです。
        //! @param[in] slotCount       使用するデスクリプタスロットの数です。
        //! @param[in] historyLengthMax 記録する履歴の最大数です。
        //! @pre IsInitialized() == false
        //! @pre pDescriptorPool は有効なデスクリプタプールのポインタでなければなりません。
        //! @pre baseSlotIndex >= 0
        //! @pre slotCount >= 1 && slotCount <= SlotCountMax
        //! @pre historyLengthMax >= 1
        //! @pre SetMemory() によって必要な大きさとアライメントを満たすメモリブロックが設定されていなければなりません。
        //! @post IsInitialized() == true
        //! @post IsRecording() == false
        //! @post GetHistoryLength() == 0
        //! @details
        //! デスクリプタプール中の指定した範囲のスロットをこのオブジェクトの管理対象とします。
        //! スロットは連続した有効な範囲を指定する必要があります。@n
        //! 同一インスタンスに対するこの関数の呼び出しはスレッドセーフではありません。
        void Initialize(
            nn::gfx::DescriptorPool* pDescriptorPool,
            int baseSlotIndex,
            int slotCount,
            int historyLengthMax
            ) NN_NOEXCEPT;

        //! @brief このオブジェクトを破棄します。
        //! @pre IsInitialized() == true
        //! @post IsInitialized() == false
        //! @details
        //! 同一インスタンスに対するこの関数の呼び出しはスレッドセーフではありません。
        void Finalize() NN_NOEXCEPT;

        //! @brief このオブジェクトが初期化済かを返します。
        //! @return 初期化済の場合 true を返します。それ以外の場合 false を返します。
        //! @details
        //! Initialize() により初期化済になります。
        //! Finalize() により未初期化になります。
        bool IsInitialized() const NN_NOEXCEPT;

        //! @brief このオブジェクトのデスクリプタスロットの獲得が開始されているかを返します。
        //! @pre IsInitialized() == true
        //! @return 獲得が開始されている場合 true を返します。それ以外の場合 false を返します。
        //! @details
        //! Begin() の成功によりスロットの獲得が開始されます。
        //! End() によりスロットの獲得が終了します。
        bool IsRecording() const NN_NOEXCEPT;

        //! @brief オブジェクトの初期化に必要なメモリブロックの大きさ（バイト）を取得します。
        //! @param[in] historyLengthMax オブジェクトが記録する履歴の最大数です。
        //! @pre historyLengthMax >= 1
        //! @return オブジェクトの初期化に必要なメモリブロックの大きさ（バイト）を返します。
        //! @details
        //! Initialize() の呼び出しよりも前に SetMemory() によりメモリブロックを設定する必要があります。
        //! SetMemory() で設定されるメモリブロックはこの関数で取得される値以上のサイズでなければなりません。@n
        //! この関数はスレッドセーフです。
        static size_t GetRequiredMemorySize(int historyLengthMax) NN_NOEXCEPT;

        //! @brief オブジェクトの初期化に必要なメモリブロックのアライメント（バイト）を取得します。
        //! @return オブジェクトの初期化に必要なメモリブロックのアライメント（バイト）を返します。
        //! @details
        //! Initialize() の呼び出しよりも前に SetMemory() によりメモリブロックを設定する必要があります。
        //! SetMemory() で設定されるメモリブロックのアドレスはこの関数で取得される値の整数倍でなければなりません。@n
        //! この関数はスレッドセーフです。
        static size_t GetRequiredMemoryAlignment() NN_NOEXCEPT;

        //! @brief このオブジェクトが使用するメモリを設定します。
        //! @param[in] pMemory メモリブロックのポインタです。
        //! @param[in] size    メモリブロックの大きさ（バイト）です。
        //! @pre IsInitialized() == false
        //! @post 使用するメモリが設定された状態になります。
        //! @details
        //! このオブジェクトが使用するメモリを設定します。
        //! この関数はオブジェクトの初期化前に呼び出す必要があります。
        //! 設定するべきメモリのサイズとアライメントは GetRequiredMemorySize()、 GetRequiredMemoryAlignment() で取得できます。
        //! このオブジェクトが破棄される前に pMemory を解放してはいけません。@n
        //! 同一インスタンスに対するこの関数の呼び出しはスレッドセーフではありません。
        void SetMemory(void* pMemory, size_t size) NN_NOEXCEPT;

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

        //! @brief デスクリプタスロットの獲得を開始します。
        //! @pre IsInitialized() == true
        //! @pre IsRecording() == false
        //! @pre GetHistoryLength() < GetHistoryLengthMax()
        //! @post IsRecording() == true
        //! @details
        //! 同一インスタンスに対するこの関数の呼び出しはスレッドセーフではありません。
        void Begin() NN_NOEXCEPT;

        //! @brief デスクリプタスロットの獲得を終了します。
        //! @pre IsInitialized() == true
        //! @pre IsRecording() == true
        //! @post IsRecording() == false
        //! @post 直前の Begin() から End() の呼び出しの間に獲得されたデスクリプタスロットの範囲が履歴に記録されます。
        //! @post GetHistoryLength() の返す値が 1 増加します。
        //! @details
        //! 直前の Begin() から End() の呼び出しの間に獲得されたスロットが 0 個の場合でも履歴に記録されます。@n
        //! 同一インスタンスに対するこの関数の呼び出しはスレッドセーフではありません。
        //! @see Free()
        void End() NN_NOEXCEPT;

        //! @brief このオブジェクトが記録している履歴のうち最も古いものに記録されているデスクリプタスロットを解放します。
        //! @pre IsInitialized() == true
        //! @pre GetHistoryLength() > 0
        //! @post 最も古い履歴に記録されているデスクリプタスロットを解放します。
        //! @post GetHistoryLength() の返す値が 1 減少します。
        //! @details
        //! スロットの解放は Begin() の呼びだしから End() の呼び出しまでを 1 単位として行われます。
        //! 各々の Allocate() で獲得したスロットを個別に解放することはできません。@n
        //! 解放されたスロットは直ちに獲得可能な状態になります。@n
        //! 同一インスタンスに対するこの関数の複数回の呼び出しはスレッドセーフではありません。
        //! 同一インスタンスに対するこの関数とスロットの獲得を行う関数の同時呼出しはスレッドセーフです。
        //! @see End()
        void Free() NN_NOEXCEPT;

        //! @brief 新しくデスクリプタスロットを 1 つ獲得します。
        //! @pre IsInitialized() == true
        //! @pre IsRecording() == true
        //! @return 新しく獲得したデスクリプタスロットのインデックスを返します。
        //!         デスクリプタスロットの獲得に失敗した場合 InvalidIndex を返します。
        //! @details
        //! この関数はスレッドセーフです。
        //! スロットの獲得・解放を行う他の関数と同時に呼び出すことができます。
        int Allocate() NN_NOEXCEPT;

        //! @brief 新しく連続したデスクリプタスロットを獲得します。
        //! @param[in] count 獲得するデスクリプタスロットの数
        //! @pre IsInitialized() == true
        //! @pre IsRecording() == true
        //! @pre count >= 0
        //! @return 新しく獲得したデスクリプタスロットの先頭のインデックスを返します。
        //!         デスクリプタスロットの獲得に失敗した場合 InvalidIndex を返します。
        //! @details
        //! count == 0 の場合 InvalidIndex が返されます。@n
        //! この関数はスレッドセーフです。
        //! スロットの獲得・解放を行う他の関数と同時に呼び出すことができます。
        int Allocate(int count) NN_NOEXCEPT;

        //! @brief このオブジェクトが記録できる履歴の最大数を取得します。
        //! @pre IsInitialized() == true
        //! @return このオブジェクトが記録できる履歴の最大数を取得します。
        //! @details
        //! この関数はスレッドセーフです。
        int GetHistoryLengthMax() const NN_NOEXCEPT;

        //! @brief このオブジェクトが現在記録している履歴の数を取得します。
        //! @pre IsInitialized() == true
        //! @return このオブジェクトが現在記録している履歴の数を取得します。
        //! @details
        //! End() の呼び出しにより履歴が 1 つ増加します。
        //! Free() の呼び出しにより履歴が 1 つ減少します。
        //! この関数が GetHistoryLengthMax() と同じ値を返す場合、 Begin() は失敗します。@n
        //! この関数はスレッドセーフです。
        int GetHistoryLength() const NN_NOEXCEPT;

        //! @brief 履歴を確保数 0 の履歴で埋めます。
        //! @pre IsInitialized() == true
        //! @pre IsRecording() == false
        //! @post GetHistoryLength() == GetHistoryLengthMax()
        //! @details
        //! 履歴がいっぱいになるまで確保数 0 の履歴を追加します。
        //! 以下のコードと等価です。
        //!
        //! @code
        //! TransientDescriptorAllocator transient;
        //! while(transient.GetHistoryLength() < transient.GetHistoryLengthMax())
        //! {
        //!     transient.Begin();
        //!     transient.End();
        //! }
        //! @endcode
        //!
        //! 既に履歴が記録されている場合、この関数呼び出しによって追加される履歴がより新しい記録となります。@n
        //! 同一インスタンスに対するこの関数の呼び出しはスレッドセーフではありません。
        void FillHistory() NN_NOEXCEPT;

    private:
        nn::gfx::DescriptorPool* m_pDescriptorPool;
        detail::IndexRingBuffer m_SlotIndexRingBuffer;

        detail::IndexRange* m_pHistory;
        size_t m_HistoryMemorySize;
        int m_HistoryRingSize;
        int m_HistoryHead;
        int m_HistoryTail;

        bool m_IsInitialized;
    };

}}}
