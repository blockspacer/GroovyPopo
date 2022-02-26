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

#include <nn/atk/atk_SequenceSoundFile.h>
#include <nn/util/util_BinTypes.h>

namespace nn {
namespace atk {
namespace detail {

class SequenceSoundFileReader
{
public:
    static const uint32_t SignatureFile = NN_UTIL_CREATE_SIGNATURE_4( 'F', 'S', 'E', 'Q' );

    explicit SequenceSoundFileReader( const void* sequenceFile ) NN_NOEXCEPT;
    bool IsAvailable() const NN_NOEXCEPT { return m_pHeader != NULL; }

    const void* GetSequenceData() const NN_NOEXCEPT;
    bool GetOffsetByLabel( const char* label, uint32_t* offsetPtr ) const NN_NOEXCEPT;
    const char* GetLabelByOffset( uint32_t offset ) const NN_NOEXCEPT;   // 使われない？

    // デバッグ用
    inline int GetLabelCount() const NN_NOEXCEPT
    {
        return m_pLabelBlockBody->GetLabelCount();
    }
    inline const char* GetLabel( int index ) const NN_NOEXCEPT
    {
        return m_pLabelBlockBody->GetLabel( index );
    }

private:
    const SequenceSoundFile::FileHeader* m_pHeader;
    const SequenceSoundFile::DataBlockBody* m_pDataBlockBody;
    const SequenceSoundFile::LabelBlockBody* m_pLabelBlockBody;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

