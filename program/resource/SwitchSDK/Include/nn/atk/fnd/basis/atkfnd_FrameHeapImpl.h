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


#include <nn/atk/fnd/basis/atkfnd_HeapBase.h>

namespace nn { namespace atk { namespace detail { namespace fnd {

//! @internal
class FrameHeapImpl : public HeapBase
{
public:
    static const int FreeHeadMode = (1 << 0);
    static const int FreeTailMode = (1 << 1);
    static const int FreeAllMode  = (FreeHeadMode | FreeTailMode);

private:
    struct HeapState
    {
        uint32_t         tagName;            // タグ名
        void*       headAllocator;      // フレームヒープの先頭位置
        void*       tailAllocator;      // フレームヒープの末尾位置
        HeapState*  pPrevState;         // 1 つ前の状態保存へのポインタ

        HeapState()  NN_NOEXCEPT:
          tagName( 0 ),
          headAllocator( NULL ),
          tailAllocator( NULL ),
          pPrevState( NULL )
        {
        }
    };

public:
    static  FrameHeapImpl* Create( void* startAddress, size_t heapSize, uint16_t optFlag = 0 ) NN_NOEXCEPT;

    void*       Destroy() NN_NOEXCEPT;
    void*       Alloc( size_t size, int alignment = DefaultAlignment ) NN_NOEXCEPT;
    size_t    ResizeForMBlock( void* memBlock, size_t newSize ) NN_NOEXCEPT;
    size_t      GetAllocatableSize( int alignment = DefaultAlignment ) NN_NOEXCEPT;
    void        Free( int mode ) NN_NOEXCEPT;
    bool        RecordState( uint32_t tagName ) NN_NOEXCEPT;
    bool        FreeByState( uint32_t tagName ) NN_NOEXCEPT;
    uint32_t    Adjust() NN_NOEXCEPT;

private:
    bool    IsValid() NN_NOEXCEPT { return GetSignature() == FrameHeapSignature; }

    void*   AllocFromHead( size_t size, int alignment ) NN_NOEXCEPT;
    void*   AllocFromTail( size_t size, int alignment ) NN_NOEXCEPT;

    void    FreeHead() NN_NOEXCEPT;
    void    FreeTail() NN_NOEXCEPT;

    void* m_pHeadAllocator;
    void* m_pTailAllocator;
    HeapState* m_pState;
};


}}}} // nn::atk::detail::fnd
