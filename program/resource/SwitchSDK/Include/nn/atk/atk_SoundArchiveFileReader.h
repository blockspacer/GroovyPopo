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

#include <nn/util/util_BinTypes.h>
#include <nn/atk/atk_SoundArchiveFile.h>
#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/detail/atk_BinaryTypes.h>

namespace nn {
namespace atk {
namespace detail {

class SoundArchiveFileReader
{
public:
    static const uint32_t SignatureFile = NN_UTIL_CREATE_SIGNATURE_4( 'F', 'S', 'A', 'R' );
    static const uint32_t InvalidOffset = BinaryTypes::InvalidOffset;
    static const uint32_t InvalidSize = BinaryTypes::InvalidSize;

    SoundArchiveFileReader() NN_NOEXCEPT;
    ~SoundArchiveFileReader() NN_NOEXCEPT {}

    // ------------------------------------------------------------------------
    //  初期化
    void Initialize( const void* soundArchiveData ) NN_NOEXCEPT;
    void Finalize() NN_NOEXCEPT;
    bool IsStreamSendAvailable() const NN_NOEXCEPT;
    bool IsFilterSupportedVersion() const NN_NOEXCEPT;
    bool IsStreamPrefetchAvailable() const NN_NOEXCEPT;

    // ブロック情報設定
    void SetStringBlock( const void* stringBlock ) NN_NOEXCEPT;
    void SetInfoBlock( const void* infoBlock ) NN_NOEXCEPT;

    // ブロック情報取得 (Initialize 後に有効)
    inline uint32_t GetStringBlockSize() const NN_NOEXCEPT { return m_Header.GetStringBlockSize(); }
    inline int GetStringBlockOffset() const NN_NOEXCEPT { return m_Header.GetStringBlockOffset(); }
    inline uint32_t GetInfoBlockSize() const NN_NOEXCEPT { return m_Header.GetInfoBlockSize(); }
    inline int GetInfoBlockOffset() const NN_NOEXCEPT { return m_Header.GetInfoBlockOffset(); }
    inline uint32_t GetFileBlockSize() const NN_NOEXCEPT { return m_Header.GetFileBlockSize(); }
    inline int GetFileBlockOffset() const NN_NOEXCEPT { return m_Header.GetFileBlockOffset(); }

    // ------------------------------------------------------------------------
    //  文字列ブロックへのアクセス

    uint32_t GetStringCount() const NN_NOEXCEPT;
    SoundArchive::ItemId GetItemId( const char* pStr ) const NN_NOEXCEPT;
    const char* GetItemLabel( SoundArchive::ItemId id ) const NN_NOEXCEPT;
    SoundArchive::FileId GetItemFileId( SoundArchive::ItemId id ) const NN_NOEXCEPT;
    SoundArchive::FileId GetItemPrefetchFileId( SoundArchive::ItemId id ) const NN_NOEXCEPT;

    // デバッグ
    void DumpTree() const NN_NOEXCEPT;
    const char* GetString( SoundArchive::StringId stringId ) const NN_NOEXCEPT;    // 本当は private


    // ------------------------------------------------------------------------
    //  情報ブロックへのアクセス

    // 個数取得
    uint32_t GetSoundCount() const NN_NOEXCEPT;
    uint32_t GetBankCount() const NN_NOEXCEPT;
    uint32_t GetPlayerCount() const NN_NOEXCEPT;
    uint32_t GetSoundGroupCount() const NN_NOEXCEPT;
    uint32_t GetGroupCount() const NN_NOEXCEPT;
    uint32_t GetWaveArchiveCount() const NN_NOEXCEPT;
    uint32_t GetFileCount() const NN_NOEXCEPT;

    // 情報取得
    bool ReadSoundInfo(
            SoundArchive::ItemId soundId,
            SoundArchive::SoundInfo* info ) const NN_NOEXCEPT;
    bool ReadBankInfo(
            SoundArchive::ItemId bankId,
            SoundArchive::BankInfo* info ) const NN_NOEXCEPT;
    bool ReadPlayerInfo(
            SoundArchive::ItemId playerId,
            SoundArchive::PlayerInfo* info ) const NN_NOEXCEPT;
    bool ReadSoundGroupInfo(
            SoundArchive::ItemId soundGroupId,
            SoundArchive::SoundGroupInfo* info ) const NN_NOEXCEPT;
    bool ReadGroupInfo(
            SoundArchive::ItemId groupId,
            SoundArchive::GroupInfo* info ) const NN_NOEXCEPT;
    bool ReadWaveArchiveInfo(
            SoundArchive::ItemId warcId,
            SoundArchive::WaveArchiveInfo* info ) const NN_NOEXCEPT;
    bool ReadFileInfo(
            SoundArchive::FileId id,
            SoundArchive::FileInfo* info, int index = 0 ) const NN_NOEXCEPT;
    bool ReadSoundArchivePlayerInfo(
            SoundArchive::SoundArchivePlayerInfo* info ) const NN_NOEXCEPT;

    // 以下は、SoundInfo から直接アクセスする
    SoundArchive::SoundType GetSoundType( SoundArchive::ItemId soundId ) const NN_NOEXCEPT;
    uint32_t GetSoundUserParam( SoundArchive::ItemId soundId ) const NN_NOEXCEPT;
    bool ReadSoundUserParam( uint32_t* pOutValue, SoundArchive::ItemId soundId, int index ) const NN_NOEXCEPT;
    bool ReadSound3DInfo(
            SoundArchive::ItemId soundId,
            SoundArchive::Sound3DInfo* info ) const NN_NOEXCEPT;
    bool ReadSequenceSoundInfo(
            SoundArchive::ItemId soundId,
            SoundArchive::SequenceSoundInfo* info ) const NN_NOEXCEPT;
    bool ReadStreamSoundInfo(
            SoundArchive::ItemId soundId,
            SoundArchive::StreamSoundInfo* info ) const NN_NOEXCEPT;
    bool ReadStreamSoundInfo2(
            SoundArchive::ItemId soundId,
            SoundArchive::StreamSoundInfo2* info ) const NN_NOEXCEPT;
    bool ReadWaveSoundInfo(
            SoundArchive::ItemId soundId,
            SoundArchive::WaveSoundInfo* info ) const NN_NOEXCEPT;
    bool ReadAdvancedWaveSoundInfo(
            SoundArchive::ItemId soundId,
            SoundArchive::AdvancedWaveSoundInfo* info ) const NN_NOEXCEPT;

    const Util::Table<uint32_t>* GetWaveArchiveIdTable(
            SoundArchive::ItemId id ) const NN_NOEXCEPT;

    const Util::Table<uint32_t>* GetAttachedGroupTable(
            SoundArchive::FileId id ) const NN_NOEXCEPT;

    // TODO: 以下は、ファイルブロック周りで、対応がまだ
    // bool ReadFilePos( SoundArchive::FileId fileId, unsigned long index, SoundArchive::FilePos* info ) const;
    // bool ReadGroupItemInfo( SoundArchive::GroupId groupId, unsigned long index, SoundArchive::GroupItemInfo* info ) const;
    // const char* GetExtFilePath( uint32_t offset ) const;

private:
    SoundArchiveFile::FileHeader m_Header;
    const SoundArchiveFile::StringBlockBody* m_pStringBlockBody;
    const SoundArchiveFile::InfoBlockBody* m_pInfoBlockBody;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
