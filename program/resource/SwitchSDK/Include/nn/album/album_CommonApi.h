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

/**
 * @file
 * @brief  アルバムライブラリの初期化と終了に関する定義
 */

#include <nn/nn_Common.h>

namespace nn{ namespace album{

    //! @brief アルバムライブラリを初期化して使用可能にします。
    //! @post アルバムライブラリが使用可能
    //! @details
    //! 複数回呼び出すことが可能です。
    //! アルバムライブラリの使用を終了するためには Initialize() を呼び出した回数と同じ回数 Finalize() を呼び出す必要があります。
    //!
    //! この API はスレッドセーフではありません。
    void Initialize() NN_NOEXCEPT;

    //! @brief アルバムライブラリの終了処理を行います。
    //! @pre アルバムライブラリが使用可能
    //! @details
    //! アルバムライブラリの使用を終了するためには Initialize() を呼び出した回数と同じ回数 Finalize() を呼び出す必要があります。
    //!
    //! この API はスレッドセーフではありません。
    void Finalize() NN_NOEXCEPT;

}}

