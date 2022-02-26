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

#include <nn/util/util_IntrusiveList.h>

namespace nn { namespace atk { namespace detail { namespace fnd {

//! @internal
//! @details 拡張ヒープ、フレームヒープ、ユニットヒープの基底
class HeapBase :public nn::util::IntrusiveListBaseNode<HeapBase>
{
public:
    //
    //  type
    //

    typedef util::IntrusiveList< HeapBase, util::IntrusiveListBaseNodeTraits<HeapBase> > HeapList;


    //
    //  constant value
    //

    // ヒープからメモリを割り当てるときのデフォルトアライメントサイズ
    static const int DefaultAlignment  = 4;

    // 拡張ヒープ・フレームヒープ・ユニットヒープのシグネチャ
    static const uint32_t ExpHeapSignature   = 'EXPH';
    static const uint32_t FrameHeapSignature = 'FRMH';
    static const uint32_t UnitHeapSignature  = 'UNTH';

    // メモリ確保時のオプション
    static const int OptionZeroClear     = ( 1 << 0 );   // メモリ確保時に、メモリをゼロクリア
    static const int OptionDebugFill     = ( 1 << 1 );   // ヒープ作成時・メモリ確保・解放時にメモリ充填
    static const int OptionThreadSafe    = ( 1 << 2 );   // 排他制御の有無

    // このビットが立っているとエラー出力
    static const int ErrorPrint            = ( 1 << 0 );


    //
    //  enum
    //

    enum FillType {
        FillType_NoUse,    // デバッグフィル未使用時
        FillType_Alloc,    // デバッグフィル確保時
        FillType_Free,     // デバッグフィル解放時
        FillType_Max
    };

    enum HeapType {
        HeapType_Exp,      // 拡張ヒープ
        HeapType_Frame,    // フレームヒープ
        HeapType_Unit,     // ユニットヒープ
        HeapTyep_Unknown   // 未知のヒープ
    };


    //
    //  function
    //

    static HeapBase* FindContainHeap( const void* memBlock ) NN_NOEXCEPT;   // TODO: static ???
    static HeapBase* FindParentHeap( const HeapBase* heap ) NN_NOEXCEPT;    // TODO: static ???

    void* GetHeapStartAddress() NN_NOEXCEPT { return this; }
    void* GetHeapEndAddress() NN_NOEXCEPT   { return mHeapEnd; }

    size_t GetTotalSize() NN_NOEXCEPT          { return (reinterpret_cast<size_t>(mHeapEnd) - reinterpret_cast<size_t>(this)); }
    size_t GetTotalUsableSize() NN_NOEXCEPT    { return (reinterpret_cast<size_t>(mHeapEnd) - reinterpret_cast<size_t>(mHeapStart)); }

//    void Dump();

    uint32_t SetFillValue( FillType type, uint32_t val ) NN_NOEXCEPT;
    uint32_t GetFillValue( FillType type ) NN_NOEXCEPT;

    HeapType GetHeapType() NN_NOEXCEPT;

protected:
    static const int MIN_ALIGNMENT = 4;

    void Initialize( uint32_t signature, void* start, void* end, uint16_t optFlag ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    uint32_t GetSignature() const NN_NOEXCEPT { return m_Signature; }
    void* GetHeapStart() const NN_NOEXCEPT { return mHeapStart; }
    void* GetHeapEnd() const NN_NOEXCEPT { return mHeapEnd; }

    void LockHeap() NN_NOEXCEPT;
    void UnlockHeap() NN_NOEXCEPT;

    void FillFreeMemory( void* address, size_t size ) NN_NOEXCEPT;
    void FillNoUseMemory( void* address, size_t size ) NN_NOEXCEPT;
    void FillAllocMemory( void* address, size_t size ) NN_NOEXCEPT;

    void*       mHeapStart;     // ヒープ先頭アドレス
    void*       mHeapEnd;       // ヒープ末尾 (+1) アドレス

private:
    static HeapBase* FindContainHeap( HeapList* pList, const void* memBlock ) NN_NOEXCEPT;
    static HeapList* FindListContainHeap( HeapBase* pHeapBase ) NN_NOEXCEPT;

    uint16_t  GetOptionFlag() NN_NOEXCEPT;
    void SetOptionFlag( uint16_t optFlag ) NN_NOEXCEPT;

    uint32_t         m_Signature;
    HeapList    m_ChildList;
    uint32_t         m_Attribute;     // 属性 [8:オプションフラグ]
};

}}}} // nn::atk::detail::fnd
