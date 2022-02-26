/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * @file
 * @brief   The Video Interface (VI) library.
 * @details This API supports both HOS and Windows.
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn { namespace vi {

    /**
     * @brief Initializes VI library.
     *
     * @post VI library is initialized.
     *
     * @details This is safe to call multiple times.  If the library is already initialized,
     *          no operations occur.
     *
     *          There must be a matching call to Finalize for each call to this function or
     *          the VI library will not be finalized.
     */
    void Initialize() NN_NOEXCEPT;

    /**
     * @brief Finalizes VI library.
     *
     * @pre  No preconditions.
     * @post VI library has been shutdown.
     *
     * @details Note that nn::vi::Layer and nn::vi::Display handles will be invalidated
     *          and should not be used after calling this function.
     *
     *          This is safe to call multiple times.  If the library is not initialized, no
     *          operations occur.
     */
    void Finalize() NN_NOEXCEPT;

}}
