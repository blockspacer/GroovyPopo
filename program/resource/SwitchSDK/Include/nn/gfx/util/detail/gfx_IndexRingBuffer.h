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
#include <climits>
#include <atomic>
#include "gfx_IndexRange.h"

namespace nn{ namespace gfx{ namespace util{ namespace detail{

    class IndexRingBuffer
    {
        NN_DISALLOW_COPY(IndexRingBuffer);

    public:
        static const int InvalidIndex = -1;
        static const int IndexCountMax = INT_MAX / 2;

    public:
        IndexRingBuffer() NN_NOEXCEPT;

        void Initialize(
            int baseIndex,
            int count
            ) NN_NOEXCEPT;
        void Finalize() NN_NOEXCEPT;

        bool IsInitialized() const NN_NOEXCEPT;
        bool IsRecording() const NN_NOEXCEPT;

        int GetBaseIndex() const NN_NOEXCEPT;
        int GetIndexCount() const NN_NOEXCEPT;

        void Begin() NN_NOEXCEPT;
        void End(IndexRange* pOutRange) NN_NOEXCEPT;

        void ReleaseIndexRange(const IndexRange* pRange) NN_NOEXCEPT;

        int AcquireIndex() NN_NOEXCEPT;
        int AcquireIndexRange(int count) NN_NOEXCEPT;

    private:
        int m_BaseIndex;
        int m_IndexCount;

        std::atomic_int m_HeadLocalIndex;
        std::atomic_int m_TailLocalIndex;
        int m_LastLocalIndex;

        bool m_IsInitialized;
        std::atomic_bool m_IsRecording;
    };

}}}}
