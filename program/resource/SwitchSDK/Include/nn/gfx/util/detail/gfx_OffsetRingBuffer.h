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
#include "gfx_OffsetRange.h"

namespace nn{ namespace gfx{ namespace util{ namespace detail{

    class OffsetRingBuffer
    {
        NN_DISALLOW_COPY(OffsetRingBuffer);

    public:
        static const ptrdiff_t InvalidOffset = -1;
        static const ptrdiff_t SizeMax = PTRDIFF_MAX / 2;
        static const ptrdiff_t AlignmentMax = PTRDIFF_MAX / 2 + 1;

    public:
        OffsetRingBuffer() NN_NOEXCEPT;

        void Initialize(
            ptrdiff_t baseOffset,
            ptrdiff_t size
            ) NN_NOEXCEPT;
        void Finalize() NN_NOEXCEPT;

        bool IsInitialized() const NN_NOEXCEPT;
        bool IsRecording() const NN_NOEXCEPT;

        ptrdiff_t GetBaseOffset() const NN_NOEXCEPT;
        ptrdiff_t GetSize() const NN_NOEXCEPT;

        void Begin() NN_NOEXCEPT;
        void End(OffsetRange* pOutRange) NN_NOEXCEPT;

        void ReleaseOffsetRange(const OffsetRange* pRange) NN_NOEXCEPT;

        ptrdiff_t Allocate(ptrdiff_t size, ptrdiff_t alignment) NN_NOEXCEPT;

    private:
        ptrdiff_t m_BaseOffset;
        ptrdiff_t m_Size;

        std::atomic_ptrdiff_t m_HeadLocalOffset;
        std::atomic_ptrdiff_t m_TailLocalOffset;
        ptrdiff_t m_LastLocalOffset;

        bool m_IsInitialized;
        std::atomic_bool m_IsRecording;
    };

}}}}
