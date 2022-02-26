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

namespace nn
{
namespace ui2d
{

//! Endian 種別です。
enum EndianTypes
{
    EndianTypes_Big = 0, //!< ビッグエンディアンです。
    EndianTypes_Little = 1, //!< リトルエンディアンです。

#if defined(NN_BUILD_CONFIG_OS_WIN32)
    EndianTypes_HostEndian = EndianTypes_Little
#else
    EndianTypes_HostEndian = EndianTypes_Big
#endif
};

//------------------------------------------------------------------------------
//! アーカイブ中の要素です。
//!
class ArcEntry
{
public:// TODO:アクセサ
    static const int NameLengthMax = 256;

    //! コンストラクタ
    ArcEntry()
    {
        name[0] = '\0';
    }

    char name[NameLengthMax]; //!< 要素のパス
};

//------------------------------------------------------------------------------
//! アーカイブ内ファイルの情報です。
//!
class ArcFileInfo
{
    friend class ArcExtractor;
public:
    explicit ArcFileInfo() : m_StartOffset( 0 ), m_Length( 0 ) {};

    //! ファイルイメージブロックから数えた、ファイル先頭へのオフセットを取得します。
    //!
    //! @return オフセットです。
    //!
    int GetStartOffset() const { return m_StartOffset; }

    //! ファイルの長さを取得します。
    //!
    //! @return ファイルの長さです。
    //!
    size_t GetLength() const { return m_Length; }

private:
    int32_t  m_StartOffset; //!< ファイルイメージ先頭の、ファイルイメージブロックからのオフセットです。
    int32_t  m_Length; //!< ファイルの長さです。
};

//------------------------------------------------------------------------------
/**
 *  レイアウトアーカイバで生成したファイルを読み込むクラス.
 *
 *  レイアウトアーカイバで生成するアーカイブを操作します.
 */
class ArcExtractor
{
public:
    //! コンストラクタです。
    //!
    //! @param[in] pArchive  アーカイブです。
    //!
    explicit ArcExtractor(const void* pArchive);

    //! コンストラクタです。
    ArcExtractor();

    //! デストラクタです。
    ~ArcExtractor();

    /**
     *  @brief  レイアウトアーカイブリソースをリロケートします。
     *
     *  @details
     *  レイアウトアーカイブに含まれるリソースは必要になったタイミングで自動的にリロケーションされるため
     *  ユーザーがこのメソッドを明示的に呼び出す必要はありません。
     *  @see    nn::util::RelocationTable::Relocate()
     *
     *  @param[in]  pArchive オフセットを解決するレイアウトアーカイブリソースへのポインタ。
     */
    static void Relocate(const void* pArchive);

    /**
     *  @brief  レイアウトアーカイブリソースをアンリロケートします。
     *
     *  @details
     *  アーカイブデータを参照している nn::ui2d::ResourceAccessor の Finalize を呼び出した後に
     *  このメソッドを呼び出すことでレイアウトアーカイブリソースのメモリを移動可能な状態にすることができます。
     *  @see    nn::util::RelocationTable::Unrelocate()
     *
     *  @param[in]  pArchive 再利用したいレイアウトアーカイブリソースへのポインタ。
     */
    static void Unrelocate(const void* pArchive);

    /**
     *  アーカイブのバイナリデータを解釈して、アーカイブ操作を行えるようにします.
     *
     *  @param[in]  pArchive アーカイブのバイナリデータ.
     *  @return             trueなら読み込み完了. falseなら失敗.
     */
    bool PrepareArchive( const void* pArchive );

    /**
     *  @brief  アーカイブに含まれるファイルの数を取得します。
     *
     *  @return アーカイブに含まれるファイルの数。
     */
    int GetFileCount() const;

    /**
     *  アーカイブ内のファイルの先頭ポインタをパスから取得します.
     *
     *  @param[out] pArcFileInfo   ファイルの長さなどの情報を受け取るポインタ. NULLなら無視する.
     *  @param[in]  pFilePath   読み込むファイルのパス.
     *  @return                 ファイルの先頭ポインタ. NULLのとき失敗.
     */
    inline void* FindFile( ArcFileInfo* pArcFileInfo, const char* pFilePath );

    /**
    *  アーカイブ内のファイルの先頭ポインタをパスから取得します.
    *
    *  @param[out] pArcFileInfo   ファイルの長さなどの情報を受け取るポインタ. NULLなら無視する.
    *  @param[in]  pFilePath   読み込むファイルのパス.
    *  @return                 ファイルの先頭ポインタ. NULLのとき失敗.
    */
    inline const void* FindFile( ArcFileInfo* pArcFileInfo, const char* pFilePath ) const
    {
        return const_cast<ArcExtractor*>(this)->FindFile(pArcFileInfo, pFilePath);
    }

    /**
     *  アーカイブ内のファイルの先頭ポインタをパスから取得します.
     *
     *  @param[in]  pFilePath   読み込むファイルのパス.
     *  @return                 ファイルの先頭ポインタ. NULLのとき失敗.
     */
    inline void* FindFile( const char* pFilePath )
    {
        return FindFile(NULL, pFilePath);
    }

    /**
    *  アーカイブ内のファイルの先頭ポインタをパスから取得します.
    *
    *  @param[in]  pFilePath   読み込むファイルのパス.
    *  @return                 ファイルの先頭ポインタ. NULLのとき失敗.
    */
    inline const void* FindFile( const char* pFilePath ) const
    {
        return const_cast<ArcExtractor*>(this)->FindFile(pFilePath);
    }

    /**
     *  アーカイブ内のファイルをエントリIDから取得します.
     *
     *  エントリーIDはあらかじめConvertPathToEntryIDなどで取得する必要があります.
     *  @param[out] pArcFileInfo   ファイルの長さなどの情報を受け取るポインタ. NULLなら無視する.
     *  @param[in]  entryId    読み込むファイルのエントリーID.
     *  @return                 ファイルの先頭ポインタ. NULLのとき失敗.
     */
    void* GetFileFast( ArcFileInfo* pArcFileInfo, int entryId );

    /**
    *  アーカイブ内のファイルをエントリIDから取得します.
    *
    *  エントリーIDはあらかじめConvertPathToEntryIDなどで取得する必要があります.
    *  @param[out] pArcFileInfo   ファイルの長さなどの情報を受け取るポインタ. NULLなら無視する.
    *  @param[in]  entryId    読み込むファイルのエントリーID.
    *  @return                 ファイルの先頭ポインタ. NULLのとき失敗.
    */
    const void* GetFileFast( ArcFileInfo* pArcFileInfo, int entryId ) const
    {
        return const_cast<ArcExtractor*>(this)->GetFileFast(pArcFileInfo, entryId);
    }

    /**
     *  アーカイブ内のファイルをエントリIDから取得します.
     *
     *  エントリーIDはあらかじめConvertPathToEntryIDなどで取得する必要があります.
     *  @param[in]  entryId    読み込むファイルのエントリーID.
     *  @return                 ファイルの先頭ポインタ. NULLのとき失敗.
     */
    void* GetFileFast( int entryId )
    {
        return GetFileFast(NULL, entryId);
    }

    /**
    *  アーカイブ内のファイルをエントリIDから取得します.
    *
    *  エントリーIDはあらかじめConvertPathToEntryIDなどで取得する必要があります.
    *  @param[in]  entryId    読み込むファイルのエントリーID.
    *  @return                 ファイルの先頭ポインタ. NULLのとき失敗.
    */
    const void* GetFileFast( int entryId ) const
    {
        return const_cast<ArcExtractor*>(this)->GetFileFast(entryId);
    }

    /**
     *  アーカイブ内のパスをエントリIDに変換します.
     *
     *  ここで取得したエントリIDは、GetFileFastで利用することができます.
     *  @param[in]  pFilePath   読み込むファイルのパス.
     *  @return                 ファイルのエントリーID. -1のとき失敗.
     */
    int ConvertPathToEntryId( const char* pFilePath ) const;

    /**
     *  エントリを読み取ります.
     *
     *  - ファイル名がアーカイブに含まれている場合のみ、ファイル名をentryに代入します.
     *  - ファイル名がアーカイブに含まれていないとき、ファイル名の変わりにhashを文字列化したものを代入します.(コンバート時に-nオプションを指定したとき)
     *  @param[in,out] pHandle  読み込みを開始するエントリID。最初から読み取るときは0に初期化してください。次のReadEntryに渡すことで連続して読み出すことができます。
     *  @param[out] pEntries    読み込んだエントリ情報を入れる場所.
     *  @param[in] count         読み込む個数.
     *  @return                読み込みが成功したら実際に読み込んだエントリ情報の数.
     */
    int ReadEntry( int* pHandle, ArcEntry pEntries[], int count ) const;

    //! アーカイブの先頭アドレスを取得します。
    const void* GetArchiveDataStart() const { return m_pArchiveBlockHeader; }

protected:
    /**
     *  ファイル情報を設定します.
     *
     *  継承先のクラスで利用するメソッドです.
     * @param[in] pArcFileInfo  ファイル情報です。
     * @param[in] startOffset  開始のオフセットです。
     * @param[in] length        長さです。
     */
    inline static void SetArcFileInfo( ArcFileInfo* pArcFileInfo, int startOffset, size_t length );

public:
    //! @name 定数です.
    //! @{
    static const uint32_t  ArchiveVersion = 0x0100;          //!< 想定するレイアウトアーカイバのコンバータ＆ランタイムバージョン.
    static const uint32_t  ArchiveEntryMax = 0x3fff;         //!< エントリの最大数
    static const uint32_t  TotalDataSizeMax = 0xffffffff;    //!< データブロックに格納できる最大長さ.
    static const uint32_t  FileNameTableAlign = 4;           //!< FileNameTableのアライメント.
    //! @}

    //! アーカイブブロックヘッダ
    struct ArchiveBlockHeader
    {
        char    signature[ 4 ];     //!< "SARC".
        int16_t      headerSize;        //!< このブロックヘッダの大きさ.
        int16_t      byteOrder;         //!< バイトオーダーマーク.
        int32_t      fileSize;          //!< このファイル全体の大きさ(HeaderBlockを含む).
        int32_t      dataBlockOffset;  //!< ファイル先頭からデータブロックまでのオフセット.
        int16_t      version;            //!< 上位8ビットと下位8ビットに分けてバージョンを保存.
        int16_t      reserved;           //!< パディング.
    };

    //! ファイルアロケーションテーブルヘッダ
    struct FATBlockHeader
    {
        char    signature[ 4 ];     //!< "SFAT".
        int16_t      headerSize;        //!< このブロックヘッダの大きさ.
        int16_t      fileCount;           //!< ファイル数.
        int32_t      hashKey;           //!< ハッシュのキー.
    };

    //! ファイルアロケーションテーブルエントリ
    struct FATEntry
    {
        int32_t  hash;                   //!< ハッシュ.
        int32_t  nameOffset;            //!< 上位8bitに同じハッシュ内のID(1から開始)、残りの24bitに名前の位置÷FNTのアライメント(4).
        int32_t  dataStartOffset;      //!< データ開始位置.
        int32_t  dataEndOffset;        //!< データ終了位置.
    };

    //! ファイルネームテーブルヘッダ
    struct FNTBlockHeader
    {
        char    signature[ 4 ];     //!< "SFNT".
        int16_t      headerSize;        //!< このブロックヘッダの大きさ.
        int16_t      reserved;           //!< パディング.
    };

private:
    const ArchiveBlockHeader*     m_pArchiveBlockHeader;    //!< ArchiveBlockHeader(NULLのとき未prepare).
    const FATBlockHeader*         m_pFATBlockHeader;        //!< FATBlockHeader.
    const char*                   m_pFNTBlock;              //!< FNTBlockの先頭アドレス.
    FATEntry*                     m_pFATEntries;            //!< ファイルアロケーションテーブルのエントリ配列
    int                           m_FATEntryCount;          //!< ファイルアロケーションテーブルのエントリ配列の数。
    const uint8_t *               m_pDataBlock;             //!< データブロックのポインタ
    EndianTypes                   m_EndianType;
};

//------------------------------------------------------------------------------
inline void* ArcExtractor::FindFile( ArcFileInfo* pArcFileInfo, const char* pFilePath )
{
    int id = ConvertPathToEntryId( pFilePath );
    if ( id < 0 )
    {
        return NULL;
    }
    return GetFileFast( pArcFileInfo, id );
}

//------------------------------------------------------------------------------
inline void ArcExtractor::SetArcFileInfo( ArcFileInfo* pArcFileInfo, int startOffset, size_t length )
{
    NN_SDK_ASSERT_NOT_NULL( pArcFileInfo );
    pArcFileInfo->m_StartOffset = startOffset;
    pArcFileInfo->m_Length      = static_cast<int32_t>(length);
}


} // namespace ui2d
} // namespace nn
