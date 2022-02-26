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

#include <nn/util/util_TypedStorage.h>

namespace nn { namespace nlibsdk { namespace heap {

// 前方宣言
class CentralHeap;

}}} // nn::nlibsdk::heap

namespace nn { namespace mem { namespace detail {

typedef nn::util::TypedStorage<nn::nlibsdk::heap::CentralHeap,sizeof(void*) * 6,NN_ALIGNOF(void*)> InternalCentralHeapStorage;

}}} // nn::mem::detail
