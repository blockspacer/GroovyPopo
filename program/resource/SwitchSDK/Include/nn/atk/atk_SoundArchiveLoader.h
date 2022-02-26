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
#include <nn/atk/atk_Util.h>
#include <nn/atk/atk_SoundArchive.h>

namespace nn {
namespace atk {

class SoundMemoryAllocatable;

namespace detail {

struct LoadItemInfo
{
    nn::atk::SoundArchive::ItemId itemId;
    const void* address;

    LoadItemInfo() NN_NOEXCEPT
    : itemId( nn::atk::SoundArchive::InvalidId ),
      address( NULL )
    {}
};

/* ========================================================================
        class definition
   ======================================================================== */

//! @briefprivate
class SoundArchiveLoader
{
public:
    //! @internal
    static const uint32_t SignatureIndividualWave = NN_UTIL_CREATE_SIGNATURE_4( 'F', 'I', 'W', 'V' );

    // 波形バッファが満たすべきアライメント
    static const int WaveBufferAlignSize = nn::audio::BufferAlignSize;

    // 個別ロード波形のメモリブロック先頭に置かれる情報
    struct IndividualWaveInfo
    {
        // データ
        uint32_t signature;
        uint32_t fileId;         //< 所属する波形アーカイブファイル ID
        uint32_t waveIndex;      //< 波形アーカイブ内インデックス
        uint32_t padding[5];     // 32バイトアライメントのためのパディング
        uint32_t padding2[8];    // 64バイトアライメントのためのパディング

        // メソッド
        IndividualWaveInfo( uint32_t id, uint32_t index ) NN_NOEXCEPT
        : signature( SoundArchiveLoader::SignatureIndividualWave ),
          fileId( id ),
          waveIndex( index )
        {}
    };


    //---------------------------------------------------------------------------
    //! @brief  ロードフラグです。
    //---------------------------------------------------------------------------
    enum LoadFlag
    {
        // サウンド
        LoadFlag_Seq    = ( 1 << 0 ),   //!< シーケンスサウンドをロードします
        LoadFlag_Wsd    = ( 1 << 1 ),   //!< ウェーブサウンドをロードします

        // バンク
        LoadFlag_Bank   = ( 1 << 2 ),   //!< バンクをロードします

        // 波形アーカイブ
        LoadFlag_Warc   = ( 1 << 3 ),   //!< 波形アーカイブをロードします

        LoadFlag_All    = 0xffffffff    //!< 関連するファイルをすべてロードします
    };

    SoundArchiveLoader() NN_NOEXCEPT;
    virtual ~SoundArchiveLoader() NN_NOEXCEPT;

    //----------------------------------------
    //! @name 初期化
    //@{
    //---------------------------------------------------------------------------
    //! @brief  サウンドデータマネージャが利用可能な状態かどうかを調べます。
    //!
    //! @return   サウンドデータマネージャが利用可能な状態なら true を、
    //!           そうでなければ false を返します。
    //---------------------------------------------------------------------------
    bool IsAvailable() const NN_NOEXCEPT;
    //@}


    //----------------------------------------
    //! @name ロード
    //@{
    //---------------------------------------------------------------------------
    //! @brief    サウンドアーカイブ中のデータをロードします。
    //!
    //!           ロードに必要なメモリはアロケータ pAllocator から確保されます。
    //!           十分なメモリが確保できない場合は false を返します。
    //!
    //!           この関数はサウンドデータの同期ロードを行います。
    //!           非同期でサウンドデータをロードする場合は、
    //!           ロード用のスレッドでこの関数を呼び出してください。
    //!           関連付けられているサウンドアーカイブや、
    //!           引数で渡すサウンドヒープに対する操作をロード中に行わない限り、
    //!           この関数はスレッドセーフです。
    //!
    //! @param[in]    id              ロードするデータのアイテム ID です。
    //! @param[in]    pAllocator      ロードするメモリを確保するためのアロケータです。
    //! @param[in]    loadFlag        関連するデータをロードするためのフラグです。
    //! @param[in]    loadBlockSize   データを分割ロードする際の分割サイズを、
    //!                               バイト単位で指定します。
    //!
    //! @return   ロードに成功したら true を、失敗したら false を返します。
    //---------------------------------------------------------------------------
    bool LoadData(
        SoundArchive::ItemId id,
        SoundMemoryAllocatable* pAllocator,
        uint32_t loadFlag = LoadFlag_All, // LoadFlag の論理和を渡す
        size_t loadBlockSize = 0
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    サウンドアーカイブ中のデータをロードします。
    //!
    //!           ロードに必要なメモリはアロケータ pAllocator から確保されます。
    //!           十分なメモリが確保できない場合は false を返します。
    //!
    //!           この関数はサウンドデータの同期ロードを行います。
    //!           非同期でサウンドデータをロードする場合は、
    //!           ロード用のスレッドでこの関数を呼び出してください。
    //!           関連付けられているサウンドアーカイブや、
    //!           引数で渡すサウンドヒープに対する操作をロード中に行わない限り、
    //!           この関数はスレッドセーフです。
    //!
    //! @param[in]    pItemName       ロードするデータのラベル文字列です。
    //! @param[in]    pAllocator      ロードするメモリを確保するためのアロケータです。
    //! @param[in]    loadFlag        関連するデータをロードするためのフラグです。
    //! @param[in]    loadBlockSize   データを分割ロードする際の分割サイズを、
    //!                               バイト単位で指定します。
    //!
    //! @return   ロードに成功したら true を、失敗したら false を返します。
    //---------------------------------------------------------------------------
    bool LoadData(
        const char* pItemName,
        SoundMemoryAllocatable* pAllocator,
        uint32_t loadFlag = LoadFlag_All, // LoadFlag の論理和を渡す
        size_t loadBlockSize = 0
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    指定したデータがロードされているか調べます。
    //!
    //!           loadFlag を利用することで、確認するデータを制限することができます。
    //!           デフォルトはすべての関連アイテムがロードされているかを確認します。
    //!
    //! @param[in] itemId     ロードされているか確認するサウンドデータのアイテム ID です。
    //! @param[in] loadFlag   関連するデータを確認するかを示すフラグです。
    //!
    //! @return   ロードされていれば true を、されていなければ false を返します。
    //---------------------------------------------------------------------------
    bool IsDataLoaded( SoundArchive::ItemId itemId, uint32_t loadFlag = LoadFlag_All ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    指定したデータがロードされているか調べます。
    //!
    //!           loadFlag を利用することで、確認するデータを制限することができます。
    //!           デフォルトはすべての関連アイテムがロードされているかを確認します。
    //!
    //! @param[in] pItemName  ロードされているか確認するデータのラベル文字列です。
    //! @param[in] loadFlag   関連するデータを確認するかを示すフラグです。
    //!
    //! @return   ロードされていれば true を、されていなければ false を返します。
    //---------------------------------------------------------------------------
    bool IsDataLoaded( const char* pItemName, uint32_t loadFlag = LoadFlag_All ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  実行中の @ref LoadData をキャンセルします。
    //!
    //!         @ref LoadData では、loadBlockSize に非ゼロの値を指定すると、
    //!         そのサイズごとに分割ロードされますが、
    //!         この分割のタイミングでキャンセルされ、LoadData 関数から返ってきます。
    //!
    //!         ロードがキャンセルされても、本来ロードに必要であったメモリは占有されたままです。
    //!         そのため、ヒープを有効に使うには、適切に解放処理
    //!         (@ref SoundHeap::LoadState など) を呼び出す必要があります。
    //!
    //! @return   現在ロード中のデータがあれば true を、ロード中でなければ false を返します。
    //---------------------------------------------------------------------------
    bool CancelLoading() NN_NOEXCEPT;
    //@}

    //! @internal
    //! @param itemId :private
    //! @return :private
    const void* detail_GetFileAddressByItemId( SoundArchive::ItemId itemId ) const NN_NOEXCEPT;

    //! @internal
    //! @param bankFile :private
    //! @param pAllocator :private
    //! @return :private
    bool detail_LoadWaveArchiveByBankFile(
            const void* bankFile,
            SoundMemoryAllocatable* pAllocator ) NN_NOEXCEPT;

    //! @internal
    //! @param wsdFile :private
    //! @param waveSoundIndex :private
    //! @param pAllocator :private
    //! @return :private
    bool detail_LoadWaveArchiveByWaveSoundFile(
            const void* wsdFile,
            int waveSoundIndex,
            SoundMemoryAllocatable* pAllocator ) NN_NOEXCEPT;

protected:
    //! @internal
    //! @param arc :private
    void SetSoundArchive( const SoundArchive* arc ) NN_NOEXCEPT;

    //! @internal
    //! @return :private
    const SoundArchive* GetSoundArchive() const NN_NOEXCEPT { return m_pSoundArchive; }

    //! @internal
    //! @param fileId :private
    //! @return :private
    const void* GetFileAddressFromSoundArchive( SoundArchive::FileId fileId ) const NN_NOEXCEPT;

    //! @internal
    //! @param fileId :private
    //! @param address :private
    //! @return :private
    virtual const void* SetFileAddressToTable(
            SoundArchive::FileId fileId, const void* address ) NN_NOEXCEPT = 0;
    //! @internal
    //! @param fileId :private
    //! @return :private
    virtual const void* GetFileAddressFromTable( SoundArchive::FileId fileId ) const NN_NOEXCEPT = 0;

    //! @internal
    //! @param fileId :private
    //! @return :private
    virtual const void* GetFileAddressImpl( SoundArchive::FileId fileId ) const NN_NOEXCEPT = 0;
        // テーブルを含め、アクセス可能な箇所にデータが読み込まれているかを確認する

private:
    bool LoadSequenceSound(
            SoundArchive::ItemId soundId,
            SoundMemoryAllocatable* pAllocator,
            uint32_t loadFlag,
            size_t loadBlockSize ) NN_NOEXCEPT;
    bool LoadWaveSound(
            SoundArchive::ItemId soundId,
            SoundMemoryAllocatable* pAllocator,
            uint32_t loadFlag,
            size_t loadBlockSize,
            SoundArchive::ItemId waveSoundSetId = SoundArchive::InvalidId ) NN_NOEXCEPT;
    bool LoadAdvancedWaveSound(
            SoundArchive::ItemId soundId,
            SoundMemoryAllocatable* pAllocator,
            uint32_t loadFlag,
            size_t loadBlockSize) NN_NOEXCEPT;
    bool LoadStreamSoundPrefetch(
            SoundArchive::ItemId soundId,
            SoundMemoryAllocatable* pAllocator,
            size_t loadBlockSize ) NN_NOEXCEPT;
    bool LoadBank(
            SoundArchive::ItemId bankId,
            SoundMemoryAllocatable* pAllocator,
            uint32_t loadFlag,
            size_t loadBlockSize ) NN_NOEXCEPT;
    bool LoadWaveArchive(
            SoundArchive::ItemId warcId,
            SoundMemoryAllocatable* pAllocator,
            uint32_t loadFlag,
            size_t loadBlockSize ) NN_NOEXCEPT;
    const void* LoadWaveArchiveTable(
            SoundArchive::ItemId warcId,
            SoundMemoryAllocatable* pAllocator,
            size_t loadBlockSize ) NN_NOEXCEPT;
    bool LoadIndividualWave(
            SoundArchive::ItemId warcId,
            uint32_t waveIndex,
            SoundMemoryAllocatable* pAllocator,
            size_t loadBlockSize ) NN_NOEXCEPT;
    bool LoadGroup(
            SoundArchive::ItemId groupId,
            SoundMemoryAllocatable* pAllocator,
            size_t loadBlockSize ) NN_NOEXCEPT;
    bool LoadSoundGroup(
            SoundArchive::ItemId soundGroupId,
            SoundMemoryAllocatable* pAllocator,
            uint32_t loadFlag,
            size_t loadBlockSize ) NN_NOEXCEPT;
    const void* LoadImpl(
            SoundArchive::FileId fileId,
            SoundMemoryAllocatable* pAllocator,
            size_t loadBlockSize,
            bool needMemoryPool = false ) NN_NOEXCEPT;
    bool LoadWaveArchiveImpl(
            SoundArchive::ItemId warcId,
            uint32_t waveIndex,
            SoundMemoryAllocatable* pAllocator,
            uint32_t loadFlag,
            size_t loadBlockSize = 0 ) NN_NOEXCEPT;

    bool PostProcessForLoadedGroupFile(
            const void* pGroupFile,
            SoundMemoryAllocatable* pAllocator = NULL,
            size_t loadBlockSize = 0 ) NN_NOEXCEPT;

    bool IsSequenceSoundDataLoaded( SoundArchive::ItemId itemId, uint32_t loadFlag ) const NN_NOEXCEPT;
    bool IsWaveSoundDataLoaded( SoundArchive::ItemId itemId, uint32_t loadFlag ) const NN_NOEXCEPT;
    bool IsBankDataLoaded( SoundArchive::ItemId itemId, uint32_t loadFlag ) const NN_NOEXCEPT;
    bool IsWaveArchiveDataLoaded( SoundArchive::ItemId itemId, uint32_t waveIndex ) const NN_NOEXCEPT;
    bool IsGroupDataLoaded( SoundArchive::ItemId itemId ) const NN_NOEXCEPT;
    bool IsSoundGroupDataLoaded( SoundArchive::ItemId itemId, uint32_t loadFlag ) const NN_NOEXCEPT;

    void* LoadFile(
        SoundArchive::FileId fileId,
        SoundMemoryAllocatable* allocator,
        size_t loadBlockSize,
        bool needMemoryPool
    ) NN_NOEXCEPT;
    size_t ReadFile(
            SoundArchive::FileId fileId,
            void* buffer,
            size_t size,
            int offset,
            size_t loadBlockSize ) NN_NOEXCEPT;

    void SetWaveArchiveTableWithSeqInEmbeddedGroup(
            SoundArchive::ItemId seqId, SoundMemoryAllocatable* pAllocator ) NN_NOEXCEPT;
    void SetWaveArchiveTableWithBankInEmbeddedGroup(
            SoundArchive::ItemId bankId, SoundMemoryAllocatable* pAllocator ) NN_NOEXCEPT;
    void SetWaveArchiveTableWithWsdInEmbeddedGroup(
            SoundArchive::ItemId wsdId, SoundMemoryAllocatable* pAllocator ) NN_NOEXCEPT;
    void SetWaveArchiveTableInEmbeddedGroupImpl(
            SoundArchive::ItemId warcId, SoundMemoryAllocatable* pAllocator ) NN_NOEXCEPT;

    const SoundArchive* m_pSoundArchive;
    uint32_t m_StreamArea[ 128 ];

    uint16_t m_LoadDataCallCount;
    bool m_IsCancelLoading;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

