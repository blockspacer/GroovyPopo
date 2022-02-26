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

#include "detail/gfx_OffsetRange.h"
#include "detail/gfx_OffsetRingBuffer.h"

namespace nn{ namespace gfx{ namespace util{

    //! @brief 一時的に使用するバッファ上の領域を管理するアロケータです。
    //! @details
    //! 描画処理で一時的に使用するバッファの領域を動的に確保し、
    //! 描画の完了後にまとめて解放するための機能を提供します。
    class TransientBufferAllocator
    {
        NN_DISALLOW_COPY(TransientBufferAllocator);

    public:
        //! @brief このクラスで管理可能なバッファサイズの最大値です。
        //! @details 実装上利用可能なサイズの最大値です。
        static const size_t SizeMax = static_cast<size_t>(detail::OffsetRingBuffer::SizeMax);

        //! @brief このクラスで扱えるアライメントの最大値です。
        //! @details 実装上利用可能なアライメントの最大値です。
        static const size_t AlignmentMax = static_cast<size_t>(detail::OffsetRingBuffer::AlignmentMax);

        //! @brief 無効なオフセットを表す値です。
        //! @details
        //! 領域の確保に失敗した場合、この値が返されます。
        static const ptrdiff_t InvalidOffset = detail::OffsetRingBuffer::InvalidOffset;
    public:
        //! @brief デフォルトコンストラクタです
        TransientBufferAllocator() NN_NOEXCEPT;

        //! @brief バッファと管理対象にするオフセットの範囲を指定して初期化します。
        //! @param[in] pBuffer                 バッファです。
        //! @param[in] baseOffset              管理する領域の先頭のオフセットです。
        //! @param[in] size                    管理する領域のサイズ（バイト）です。
        //! @param[in] allocatableAlignmentMax このアロケータから確保する領域に要求する最大のアライメントです。
        //! @param[in] historyLengthMax        記録する履歴の最大数です。
        //! @pre IsInitialized() == false
        //! @pre pBuffer は有効なバッファのポインタでなければなりません。
        //! @pre baseOffset >= 0
        //! @pre baseOffset % allocatableAlignmentMax == 0
        //! @pre size >= 1 && size <= SizeMax
        //! @pre allocatableAlignmentMax <= AlignmentMax
        //! @pre allocatableAlignmentMax は 2 の累乗でなければなりません。
        //! @pre historyLengthMax >= 1
        //! @pre SetMemory() によって必要な大きさとアライメントを満たすメモリブロックが設定されていなければなりません。
        //! @post IsInitialized() == true
        //! @post IsRecording() == false
        //! @post GetHistoryLength() == 0
        //! @details
        //! バッファ上の指定した範囲の領域をこのオブジェクトの管理対象とします。
        //! 領域は連続した有効な範囲を指定する必要があります。@n
        //! @n
        //! baseOffset は allocatableAlignmentMax の整数倍でなければなりません。
        //! allocatableAlignmentMax で指定した値以下のアライメントを Allocate() に指定できます。@n
        //! @n
        //! 同一インスタンスに対するこの関数の呼び出しはスレッドセーフではありません。
        void Initialize(
            nn::gfx::Buffer* pBuffer,
            ptrdiff_t baseOffset,
            size_t size,
            size_t allocatableAlignmentMax,
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

        //! @brief このオブジェクトから領域の確保が開始されているかを返します。
        //! @pre IsInitialized() == true
        //! @return 領域の確保が開始されている場合 true を返します。それ以外の場合 false を返します。
        //! @details
        //! Begin() の成功により領域の確保が開始されます。
        //! End() により領域の確保が終了します。
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

        //! @brief このオブジェクトに設定されているバッファを取得します。
        //! @pre IsInitialized() == true
        //! @return 設定されているバッファを返します。
        //! @details
        //! この関数はスレッドセーフです。
        nn::gfx::Buffer* GetBuffer() const NN_NOEXCEPT;

        //! @brief このオブジェクトが管理しているバッファ上の領域の先頭のオフセットを取得します。
        //! @pre IsInitialized() == true
        //! @return 管理している領域の先頭のオフセットを返します。
        //! @details
        //! この関数はスレッドセーフです。
        ptrdiff_t GetBaseOffset() const NN_NOEXCEPT;

        //! @brief このオブジェクトが管理しているバッファ上の領域の大きさ（バイト）を取得します。
        //! @pre IsInitialized() == true
        //! @return 管理している領域の大きさ（バイト）を返します。
        //! @details
        //! この関数はスレッドセーフです。
        size_t GetSize() const NN_NOEXCEPT;

        //! @brief このオブジェクトから領域を確保する際に指定できる最大のアライメント（バイト）を取得します。
        //! @pre IsInitialized() == true
        //! @return 領域を確保する際に指定できる最大のアライメント（バイト）を返します。
        //! @details
        //! この関数はスレッドセーフです。
        size_t GetAllocatableAlignmentMax() const NN_NOEXCEPT;

        //! @brief バッファ上の領域の確保を開始します。
        //! @pre IsInitialized() == true
        //! @pre IsRecording() == false
        //! @pre GetHistoryLength() < GetHistoryLengthMax()
        //! @post IsRecording() == true
        //! @details
        //! 同一インスタンスに対するこの関数の呼び出しはスレッドセーフではありません。
        void Begin() NN_NOEXCEPT;

        //! @brief バッファ上の領域の確保を終了します。
        //! @pre IsInitialized() == true
        //! @pre IsRecording() == true
        //! @post IsRecording() == false
        //! @post 直前の Begin() から End() の呼び出しの間に確保された領域の範囲が履歴に記録されます。
        //! @post GetHistoryLength() の返す値が 1 増加します。
        //! @details
        //! 直前の Begin() から End() の呼び出しの間に確保された領域がない場合でも履歴に記録されます。@n
        //! 同一インスタンスに対するこの関数の呼び出しはスレッドセーフではありません。
        //! @see Free()
        void End() NN_NOEXCEPT;

        //! @brief このオブジェクトが記録している履歴のうち最も古いものに記録されている領域を解放します。
        //! @pre IsInitialized() == true
        //! @pre GetHistoryLength() > 0
        //! @post 最も古い履歴に記録されている領域を解放します。
        //! @post GetHistoryLength() の返す値が 1 減少します。
        //! @details
        //! 領域の解放は Begin() の呼びだしから End() の呼び出しまでを 1 単位として行われます。
        //! 各々の Allocate() で確保した領域を個別に解放することはできません。@n
        //! 解放された領域は直ちに確保可能な状態になります。@n
        //! 同一インスタンスに対するこの関数の複数回の呼び出しはスレッドセーフではありません。
        //! 同一インスタンスに対するこの関数と領域の確保を行う関数の同時呼出しはスレッドセーフです。
        //! @see End()
        void Free() NN_NOEXCEPT;

        //! @brief バッファ上の領域を確保します。
        //! @param[in] size 確保する領域のサイズ（バイト）です。
        //! @param[in] alignment 確保する領域のアライメント（バイト）です。
        //! @pre IsInitialized() == true
        //! @pre IsRecording() == true
        //! @pre alignment <= GetAllocatableAlignmentMax()
        //! @pre alignment は 2 の累乗でなければなりません。
        //! @return 確保した領域の先頭のオフセットを返します。
        //!         領域の確保に失敗した場合 InvalidOffset を返します。
        //! @details
        //! alignment には Initialize() の際に allocatableAlignmentMax に指定した値以下の値を指定しなければなりません。@n
        //! この関数はスレッドセーフです。
        //! 領域の確保を行う他の関数と同時に呼び出すことができます。
        ptrdiff_t Allocate(size_t size, size_t alignment) NN_NOEXCEPT;

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
        //! TransientBufferAllocator transient;
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
        nn::gfx::Buffer* m_pBuffer;
        detail::OffsetRingBuffer m_OffsetRingBuffer;
        ptrdiff_t m_AllocatableAlignmentMax;

        detail::OffsetRange* m_pHistory;
        size_t m_HistoryMemorySize;
        int m_HistoryRingSize;
        int m_HistoryHead;
        int m_HistoryTail;

        bool m_IsInitialized;
    };

}}}
