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

namespace nn {
namespace audio {

template <class T>
struct AudioBufferList
{
    T* pHead;
    T* pTail;

    void clear() NN_NOEXCEPT;
    void push_back(T* p) NN_NOEXCEPT;
    void pop_front() NN_NOEXCEPT;
    T* front() const NN_NOEXCEPT;
    T* back() const NN_NOEXCEPT;
};

struct AudioInBuffer;
struct AudioOutBuffer;
typedef AudioBufferList<AudioInBuffer> AudioInBufferList;
typedef AudioBufferList<AudioOutBuffer> AudioOutBufferList;

}  // namespace audio
}  // namespace nn
