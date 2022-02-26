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

#include <nn/atk/atk_Config.h>
#include <nn/atk/atk_MemorySoundArchive.h>

namespace nn { namespace atk {

    //! @brief 追加サウンドアーカイブです。
    class AddonSoundArchive : public MemorySoundArchive
    {
    public:
        //! @briefprivate
        //! @brief 追加サウンドアーカイブであるかを取得します。
        //! @return 追加サウンドアーカイブであれば true を返します。
        virtual bool IsAddon() const NN_NOEXCEPT NN_OVERRIDE
        {
            return true;
        }
    };

}}
