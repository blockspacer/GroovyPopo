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
 * :include nn/atk/atk_FsSoundArchive.h
 *
 * @file atk_FsSoundArchive.h
 */

#pragma once

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_SoundArchiveFileReader.h>
#include <nn/atk/fnd/io/atkfnd_FileStreamImpl.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @brief  FS 上にあるサウンドアーカイブを扱うクラスです。
//! @see SoundArchive クラス
//---------------------------------------------------------------------------
class FsSoundArchive : public SoundArchive
{
private:
    class FsFileStream;
    class CachedFsFileStream;

public:
    //! @brief  各種ロード先のバッファのアラインメントサイズです。
    //!
    //! @see LoadHeader
    //! @see LoadLabelStringData
    static const int BufferAlignSize = detail::fnd::FileStream::BufferAlignSize;

    //----------------------------------------
    //! @name コンストラクタ / デストラクタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //---------------------------------------------------------------------------
    FsSoundArchive() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    デストラクタです。
    //---------------------------------------------------------------------------
    virtual ~FsSoundArchive() NN_NOEXCEPT NN_OVERRIDE;
    //@}

    //----------------------------------------
    //! @name オープン / クローズ
    //@{
    //---------------------------------------------------------------------------
    //! @brief  FS 上のサウンドアーカイブファイルを開きます。
    //!
    //! @param[in]    filePath   ファイルの FS ファイルシステム上のパスです。
    //!
    //! @return   ファイルを開くのに成功したら true を、
    //!           失敗したら false を返します。
    //!
    //! @see Close
    //! @see LoadHeader
    //---------------------------------------------------------------------------
    bool Open(const char* filePath) NN_NOEXCEPT;

    //! @brief FS 上のサウンドアーカイブファイルのオープンに使用するパラメータです。
    struct OpenArg
    {
    };

    //---------------------------------------------------------------------------
    //! @brief  FS 上のサウンドアーカイブファイルを開きます。
    //!
    //! @param[in]    filePath   ファイルの FS ファイルシステム上のパスです。
    //! @param[in]    arg        サウンドアーカイブファイルのオープンに使用するパラメータです。
    //!
    //! @return   ファイルを開くのに成功したら true を、
    //!           失敗したら false を返します。
    //---------------------------------------------------------------------------
    bool Open(const char* filePath, const OpenArg* arg) NN_NOEXCEPT
    {
        NN_UNUSED(arg);
        return Open(filePath);
    }

    //---------------------------------------------------------------------------
    //! @brief    FS 上のサウンドアーカイブファイルを閉じます。
    //!
    //! @see Open
    //---------------------------------------------------------------------------
    void Close() NN_NOEXCEPT;
    //@}

    //----------------------------------------
    //! @name ヘッダ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    サウンドアーカイブのヘッダのサイズを取得します。
    //!
    //! @return   ヘッダのサイズを返します。
    //!
    //! @see LoadHeader
    //---------------------------------------------------------------------------
    size_t GetHeaderSize() const NN_NOEXCEPT { return m_ArchiveReader.GetInfoBlockSize(); }

    //---------------------------------------------------------------------------
    //! @brief    サウンドアーカイブのヘッダをロードします。
    //!
    //!           サウンドアーカイブからの情報の取得やサウンドデータのロードを行う前に、
    //!           この関数を呼びだしてヘッダをロードしておく必要があります。
    //!
    //!           ヘッダのロードに必要なメモリのサイズは、
    //!           @ref GetHeaderSize で取得することができます。
    //!
    //! @param[in]    buffer    ヘッダをロードするバッファへのポインタです。
    //!                         BufferAlignSize バイト境界に配置されている必要があります。
    //! @param[in]    size      バッファのサイズです。
    //!
    //! @return   ヘッダのロードに成功したら true を、失敗したら false を返します。
    //!
    //! @see Open
    //---------------------------------------------------------------------------
    bool LoadHeader( void* buffer, size_t size ) NN_NOEXCEPT;
    //@}

    //----------------------------------------
    //! @name ラベル文字列データ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    サウンドアーカイブ中のラベル文字列データのサイズを取得します。
    //!
    //! @return   ラベル文字列データのサイズを返します。
    //!
    //! @see LoadLabelStringData
    //---------------------------------------------------------------------------
    size_t GetLabelStringDataSize() const NN_NOEXCEPT { return m_ArchiveReader.GetStringBlockSize(); }

    //---------------------------------------------------------------------------
    //! @brief  サウンドアーカイブ中のラベル文字列データをロードします。
    //!
    //!         サウンドアーカイブに含まれるリソースにラベル文字列でアクセスするためには、
    //!         ラベル文字列データをロードしておく必要があります。
    //!
    //!         ラベル文字列データのロードに必要なメモリのサイズは、
    //!         @ref GetLabelStringDataSize で取得することができます。
    //!
    //!         サウンドアーカイブファイル (.bfsar) に文字列テーブルが含まれていないと、
    //!         失敗します。その場合は、SoundMaker の [プロジェクト設定] -
    //!         [サウンドアーカイブ] タブ - [文字列テーブルを出力する]
    //!         にチェックがついているかどうかご確認ください。
    //!
    //! @param[in]    buffer    ラベル文字列をロードするバッファへのポインタです。
    //!                         BufferAlignSize バイト境界に配置されている必要があります。
    //! @param[in]    size      バッファのサイズです。
    //!
    //! @return   ラベル文字列のロードに成功したら true を、失敗したら false を返します。
    //!
    //! @see Open
    //---------------------------------------------------------------------------
    bool LoadLabelStringData( void* buffer, size_t size ) NN_NOEXCEPT;
    //@}

    //! @internal
    //! @return :private
    virtual size_t detail_GetRequiredStreamBufferSize() const NN_NOEXCEPT NN_OVERRIDE;

    //! @internal
    //! @param fileId :private
    //! @return :private
    virtual const void* detail_GetFileAddress( FileId fileId ) const NN_NOEXCEPT NN_OVERRIDE
    {
        NN_UNUSED( fileId );
        return NULL;    // NOTE: MemorySoundArchive なら非 NULL 値を返しうる
    }

    //! @briefprivate
    enum FileAccessMode
    {
        FileAccessMode_Always,         //!< Close が呼ばれるまで bfsar をオープンし続けます
        FileAccessMode_InFunction     //!< 関数単位 (LoadHeader など) で bfsar をオープン→クローズします。
    };

    //! @briefprivate
    //! @brief  サウンドアーカイブファイルへのアクセスモードを設定します。
    //!
    //!         FileAccessMode_Always はこれまでと同じ挙動で、
    //!         Open ～ Close の間中、ずっとサウンドアーカイブファイルをオープンし続けます。
    //!
    //!         FileAccessMode_InFunction は、Open / LoadHeader /　LoadLabelStringData
    //!         および SoundDataManager::LoadData の間だけ、サウンドアーカイブファイルを
    //!         オープンします。
    //!         それ以外のタイミングではクローズしていますので、HostFile IO デバイス上の
    //!         サウンドアーカイブファイルを Open している場合は、
    //!         PC 上のサウンドアーカイブファイルを変更することが可能です。
    //!
    //!         FileAccessMode_InFunction の場合、上記で挙げた各 API で、
    //!         サウンドアーカイブファイルのオープン・クローズを繰り返すため
    //!         ある程度のオーバーヘッドが予想されます。
    //!         このオーバーヘッドを緩和するには、FileAccessBegin および FileAccessEnd
    //!         をご利用ください。
    //!
    //!         FileAccessBegin ～ FileAccessEnd の間は、サウンドアーカイブファイルが
    //!         オープンされっぱなしになるため、
    //!         PC 上のサウンドアーカイブファイルを変更することはできませんが、
    //!         オープン・クローズを繰り返す時と比べ、オーバーヘッドは緩和されます。
    //!
    //!         本関数は、Open の前に呼び出す必要があります。
    //!         Open 以降に呼び出した場合の動作は未保証です。
    //!
    //! @param[in] mode アクセスモードです。
    void SetFileAccessMode( FileAccessMode mode ) NN_NOEXCEPT { m_FileAccessMode = static_cast<uint8_t>(mode); }

    //! @briefprivate
    //! @return :private
    FileAccessMode GetFileAccessMode() const NN_NOEXCEPT { return static_cast<FileAccessMode>(m_FileAccessMode); }

    //! @briefprivate
    virtual void FileAccessBegin() const NN_NOEXCEPT NN_OVERRIDE;

    //! @briefprivate
    virtual void FileAccessEnd() const NN_NOEXCEPT NN_OVERRIDE;

protected:
    //! @internal
    //! @param buffer :private
    //! @param size :private
    //! @param begin :private
    //! @param length :private
    //! @return :private
    virtual detail::fnd::FileStream* OpenStream(
            void* buffer,
            size_t size,
            detail::position_t begin,
            size_t length ) const NN_NOEXCEPT NN_OVERRIDE;

    //! @internal
    //! @param buffer :private
    //! @param size :private
    //! @param extFilePath :private
    //! @param cacheBuffer :private
    //! @param cacheSize :private
    //! @return :private
    virtual detail::fnd::FileStream* OpenExtStream(
            void* buffer,
            size_t size,
            const char* extFilePath,
            void* cacheBuffer,
            size_t cacheSize ) const NN_NOEXCEPT NN_OVERRIDE;

private:
    bool LoadFileHeader() NN_NOEXCEPT;

    detail::SoundArchiveFileReader m_ArchiveReader;
    mutable nn::atk::detail::fnd::FileStreamImpl m_FileStream;
    bool m_IsOpened;
    uint8_t m_FileAccessMode;    // FileAccessMode が入る
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t m_Padding[2];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
    mutable uint32_t m_FileAccessCount;
    char m_SoundArchiveFullPath[ FilePathMax ];
    mutable nn::atk::detail::fnd::CriticalSection m_FileOpenCloseLock;
};

} // namespace nn::atk
} // namespace nn

