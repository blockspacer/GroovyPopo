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

#include <nn/atk/atk_GroupFile.h>
#include <nn/util/util_BinTypes.h>

namespace nn {
namespace atk {
namespace detail {

struct GroupItemLocationInfo
{
    uint32_t fileId;
    const void* address;    // 非 NULL: 埋め込みアイテム。ロードされたアドレスを格納。
                            // NULL   : リンクアイテム。別途ロードが必要。
};

class GroupFileReader
{
public:
    static const uint32_t SignatureFile = NN_UTIL_CREATE_SIGNATURE_4( 'F', 'G', 'R', 'P' );

    explicit GroupFileReader( const void* groupFile ) NN_NOEXCEPT;

    // InfoBlock の情報を取得
    uint32_t GetGroupItemCount() const { return m_pInfoBlockBody->GetGroupItemInfoCount(); }
    bool ReadGroupItemLocationInfo( GroupItemLocationInfo* out, uint32_t index ) const NN_NOEXCEPT;

    // InfoExBlock の情報を取得
    uint32_t GetGroupItemExCount() const NN_NOEXCEPT;
    bool ReadGroupItemInfoEx( GroupFile::GroupItemInfoEx* out, uint32_t index ) const NN_NOEXCEPT;

private:
    const GroupFile::InfoBlockBody* m_pInfoBlockBody;
    const GroupFile::FileBlockBody* m_pFileBlockBody;
    const GroupFile::InfoExBlockBody* m_pInfoExBlockBody;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

