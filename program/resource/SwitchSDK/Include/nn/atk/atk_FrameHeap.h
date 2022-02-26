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
#include <nn/atk/fnd/basis/atkfnd_FrameHeapImpl.h>
#include <nn/atk/atk_SoundDataManager.h>

namespace nn {
namespace atk {
namespace detail {

/* ========================================================================
        class definition
   ======================================================================== */

class FrameHeap
{
    /* ------------------------------------------------------------------------
            type definition
       ------------------------------------------------------------------------ */
public:
    typedef void (*DisposeCallback)( void* mem, size_t size, void* userArg );
    typedef void (*HeapCallback)( void* arg );

    struct Block
    {
      public:
        Block( void* buffer, size_t size, DisposeCallback callback, void* callbackArg, HeapCallback heapCallback, void* heapCallbackArg ) NN_NOEXCEPT
        : m_pBuffer( buffer ),
          m_Size( size ),
          m_Callback( callback ),
          m_pCallbackArg( callbackArg ),
          m_HeapCallback( heapCallback ),
          m_pHeapCallbackArg( heapCallbackArg )
        {}

        ~Block() NN_NOEXCEPT
        {
            if ( m_Callback != NULL )
            {
                m_Callback( m_pBuffer, m_Size, m_pCallbackArg );
            }
            if ( m_HeapCallback != nullptr)
            {
                m_HeapCallback( m_pHeapCallbackArg );
            }
        }

        void* GetBufferAddr() NN_NOEXCEPT { return m_pBuffer; }
        const void* GetBufferAddr() const NN_NOEXCEPT { return m_pBuffer; }
        size_t GetBufferSize() const NN_NOEXCEPT { return m_Size; }
        DisposeCallback GetDisposeCallback() const NN_NOEXCEPT { return m_Callback; }
            /* const DisposeCallback を返そうとすると、RVCT で、
               "type qualifier on return type is meaningless" の警告 */
        const void* GetDisposeCallbackArg() const NN_NOEXCEPT { return m_pCallbackArg; }
        HeapCallback GetHeapCallback() const NN_NOEXCEPT { return m_HeapCallback; }
        const void* GetHeapCallbackArg() const NN_NOEXCEPT { return m_pHeapCallbackArg; }

      public:
        util::IntrusiveListNode m_Link;
      private:
        void* m_pBuffer;
        size_t m_Size;
        DisposeCallback m_Callback;
        void* m_pCallbackArg;
        HeapCallback m_HeapCallback;
        void* m_pHeapCallbackArg;
    };

    typedef util::IntrusiveList<Block, util::IntrusiveListMemberNodeTraits<Block,&Block::m_Link>> BlockList;

    class Section
    {
      public:
        Section() NN_NOEXCEPT;
        ~Section() NN_NOEXCEPT;
        void AppendBlock( Block* block ) NN_NOEXCEPT;
        const BlockList& GetBlockList() const NN_NOEXCEPT { return m_BlockList; }
        BlockList& GetBlockList() NN_NOEXCEPT { return m_BlockList; }

        void* GetAddr() NN_NOEXCEPT { return this; }
        void SetUseCallback( bool use ) NN_NOEXCEPT { m_UseCallback = use; }

        void Dump( const nn::atk::SoundDataManager& mgr, const nn::atk::SoundArchive& arc ) const NN_NOEXCEPT;

      public:
        util::IntrusiveListNode m_Link;
      private:
        BlockList m_BlockList;
        bool m_UseCallback;
    };

    typedef util::IntrusiveList<Section, util::IntrusiveListMemberNodeTraits<Section,&Section::m_Link>> SectionList;

    /* ------------------------------------------------------------------------
            constant declaration
       ------------------------------------------------------------------------ */
private:
    static const int HeapAlign = nn::audio::BufferAlignSize;

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    FrameHeap() NN_NOEXCEPT;
    ~FrameHeap() NN_NOEXCEPT;
    bool  Create( void* startAddress, size_t size ) NN_NOEXCEPT;
    void  Destroy() NN_NOEXCEPT;

    void* Alloc(
        size_t size,
        FrameHeap::DisposeCallback callback,
        void* callbackArg,
        FrameHeap::HeapCallback heapCallback,
        void* heapCallbackArg
    ) NN_NOEXCEPT;
    void  Clear() NN_NOEXCEPT;

    int   SaveState() NN_NOEXCEPT;
    void  LoadState( int level ) NN_NOEXCEPT;
    int   GetCurrentLevel() const NN_NOEXCEPT;

    size_t   GetSize() const NN_NOEXCEPT;
    size_t   GetFreeSize() const NN_NOEXCEPT;

    bool  IsValid() const NN_NOEXCEPT { return m_pHeap != NULL; }

    // TODO: 不要？
    // const SectionList& GetSectionList() const NN_NOEXCEPT { return m_SectionList; }

    void Dump( const nn::atk::SoundDataManager& mgr, const nn::atk::SoundArchive& arc ) const NN_NOEXCEPT;
    bool ProcessCallback( int level ) NN_NOEXCEPT;

private:
    fnd::FrameHeapImpl* m_pHeap;
    SectionList m_SectionList;

    bool NewSection() NN_NOEXCEPT;
    void ClearSection() NN_NOEXCEPT;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

