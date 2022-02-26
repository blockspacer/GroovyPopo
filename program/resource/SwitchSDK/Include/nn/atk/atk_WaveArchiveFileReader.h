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

#include <nn/atk/atk_WaveArchiveFile.h>
#include <nn/util/util_BinTypes.h>

namespace nn {
namespace atk {
namespace detail {

class WaveArchiveFileReader
{
public:
    static const uint32_t SignatureFile = NN_UTIL_CREATE_SIGNATURE_4( 'F', 'W', 'A', 'R' );
    static const uint32_t SignatureWarcTable = NN_UTIL_CREATE_SIGNATURE_4( 'F', 'W', 'A', 'T' );

    WaveArchiveFileReader( const void* pWaveArchiveFile, bool isIndividual = false ) NN_NOEXCEPT;
    WaveArchiveFileReader() NN_NOEXCEPT;
    void Initialize( const void* warcFile, bool isIndividual = false ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;

    // 初期化
    void InitializeFileTable() NN_NOEXCEPT;
    bool IsAvailable() const NN_NOEXCEPT { return m_pHeader != NULL; }

    uint32_t GetWaveFileCount() const NN_NOEXCEPT;
    uint32_t GetWaveFileSize( uint32_t waveIndex ) const NN_NOEXCEPT;
    uint32_t GetWaveFileOffsetFromFileHead( uint32_t waveIndex ) const NN_NOEXCEPT;

    // 一括ロードの場合
    const void* GetWaveFile( uint32_t waveIndex ) const NN_NOEXCEPT;

    // 個別ロード時に利用
    const void* SetWaveFile( uint32_t waveIndex, const void* pWaveFile ) NN_NOEXCEPT;
    bool IsLoaded( uint32_t waveIndex ) const NN_NOEXCEPT
    {
        if ( m_IsInitialized == false )
        {
            return false;
        }

        if ( GetWaveFile( waveIndex ) != NULL )
        {
            return true;
        }
        return false;
    }

    bool HasIndividualLoadTable() const NN_NOEXCEPT;

private:
    // 個別ロードされる波形のロード管理テーブル
    struct IndividualLoadTable
    {
        const void* waveFile[1]; // 実際には、 InfoBlockBody.table.count だけ要素が並ぶ
    };

    // アクセサ
    const void* GetWaveFileForWhole( uint32_t waveIndex ) const NN_NOEXCEPT
    {
        uint32_t offset = m_pInfoBlockBody->GetOffsetFromFileBlockBody( waveIndex );
        return util::ConstBytePtr( &m_pHeader->GetFileBlock()->body, offset ).Get();
    }
    const void* GetWaveFileForIndividual( uint32_t waveIndex ) const NN_NOEXCEPT
    {
        return m_pLoadTable->waveFile[ waveIndex ];
    }


    // データ
    const WaveArchiveFile::FileHeader*      m_pHeader;
    const WaveArchiveFile::InfoBlockBody*   m_pInfoBlockBody;
    IndividualLoadTable*                    m_pLoadTable;
    bool m_IsInitialized;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

