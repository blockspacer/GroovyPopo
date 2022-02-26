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

#include <nn/atk/atk_StreamSoundFile.h>

namespace nn {
namespace atk {
namespace detail {

//! @briefprivate
//! @brief リージョン情報の読み込みを行うクラスのインタフェースです。
class IRegionInfoReadable
{
public:
    virtual ~IRegionInfoReadable() NN_NOEXCEPT {}

    //! @briefprivate
    //! @brief リージョン情報を読み込みます。
    //! @param[out] pInfo 結果を格納するリージョン情報構造体です。
    //! @param[in] regionIndex リージョン情報を読み込みたいリージョンの番号です。
    //! @return   リージョン情報の取得に成功したら true を、失敗したら false を返します。
    virtual bool ReadRegionInfo( StreamSoundFile::RegionInfo* pInfo, uint32_t regionIndex ) const NN_NOEXCEPT = 0;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

