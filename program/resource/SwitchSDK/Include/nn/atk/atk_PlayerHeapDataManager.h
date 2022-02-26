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

#include <nn/atk/atk_DisposeCallback.h>
#include <nn/atk/atk_SoundArchiveLoader.h>
#include <nn/atk/atk_SoundArchive.h>

namespace nn {
namespace atk {
namespace detail {

class PlayerHeapDataManager : public detail::driver::DisposeCallback,
                              public detail::SoundArchiveLoader
{
public:
    PlayerHeapDataManager() NN_NOEXCEPT;
    virtual ~PlayerHeapDataManager() NN_NOEXCEPT NN_OVERRIDE;

    void Initialize( const SoundArchive* arc ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    const void* SetFileAddress( SoundArchive::FileId fileId, const void* address ) NN_NOEXCEPT;
    const void* GetFileAddress( SoundArchive::FileId ) const NN_NOEXCEPT;

    // ひとつのサウンドを鳴らすのに必要なファイルは、最大９個
    // (== bfseq + 4 * bfbnk + 4 * bfwar)。
    static const int FileAddressCount = 1 + 4 + 4;

protected:
    virtual void InvalidateData( const void* start, const void* end ) NN_NOEXCEPT NN_OVERRIDE;
    virtual const void* SetFileAddressToTable(
            SoundArchive::FileId, const void* address ) NN_NOEXCEPT NN_OVERRIDE;
    virtual const void* GetFileAddressFromTable( SoundArchive::FileId fileId ) const NN_NOEXCEPT NN_OVERRIDE;
    virtual const void* GetFileAddressImpl( SoundArchive::FileId fileId ) const NN_NOEXCEPT NN_OVERRIDE;

private:
    struct FileAddress
    {
        SoundArchive::FileId fileId;
        const void* address;
    };

    FileAddress m_FileAddress[FileAddressCount];
    bool m_IsInitialized;
    bool m_IsFinalized;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

