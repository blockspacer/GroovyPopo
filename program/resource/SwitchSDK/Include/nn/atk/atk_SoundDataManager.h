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
 * :include nn/atk/atk_SoundDataManager.h
 *
 * @file atk_SoundDataManager.h
 */

#pragma once

#include <nn/atk/atk_DisposeCallback.h>
#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_SoundArchiveLoader.h>
#include <nn/atk/atk_Util.h>

namespace nn {
namespace atk {

class SoundMemoryAllocatable;

namespace detail
{

//! @internal
class SoundFileManager
{
public:
    virtual ~SoundFileManager() NN_NOEXCEPT {}

public:
    virtual const void* GetFileAddress( SoundArchive::FileId fileId ) const NN_NOEXCEPT = 0;
    virtual const void* GetFileWaveDataAddress( SoundArchive::FileId fileId ) const NN_NOEXCEPT = 0;
};

} // namespace nn::atk::detail

//---------------------------------------------------------------------------
//! @brief    サウンドアーカイブのデータをロードするためのクラスです。
//!
//! @see SoundArchivePlayer クラス
//!
//! :baseclass nn::atk::detail::driver::DisposeCallback
//! :baseclass nn::atk::detail::SoundArchiveLoader
//!
//---------------------------------------------------------------------------
class SoundDataManager : public detail::driver::DisposeCallback,
                         public detail::SoundArchiveLoader
{
public:
    //---------------------------------------------------------------------------
    //! @brief      バッファアラインサイズです。
    //---------------------------------------------------------------------------
    static const uint32_t BufferAlignSize = sizeof(void*);

    //! @name コンストラクタ / デストラクタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //---------------------------------------------------------------------------
    SoundDataManager() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    デストラクタです。
    //!
    //---------------------------------------------------------------------------
    virtual ~SoundDataManager() NN_NOEXCEPT NN_OVERRIDE;
    //@}

    //----------------------------------------
    //! @name 初期化
    //@{
    //---------------------------------------------------------------------------
    //! @brief    初期化に必要なメモリのサイズを取得します。
    //!
    //! @param[in]    arc     このサウンドデータマネージャを登録するプレイヤーで
    //!                       使用するサウンドアーカイブです。
    //!
    //! @return   初期化に必要なメモリのサイズを返します。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    size_t GetRequiredMemSize( const SoundArchive* arc ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    サウンドデータマネージャの初期化を行います。
    //!
    //!           サウンドデータマネージャを使用する前に初期化を行う必要があります。
    //!           初期化を行うことにより、
    //!           サウンドデータマネージャがサウンドアーカイブと関連付けられ、
    //!           サウンドアーカイブプレイヤーに登録することができます。
    //!
    //!           サウンドデータマネージャが必要とするメモリのサイズは
    //!           @ref GetRequiredMemSize で取得することができます。
    //!
    //! @param[in]    pArchive このサウンドデータマネージャを登録するプレイヤーで
    //!                        使用するサウンドアーカイブです。
    //! @param[in]    buffer   バッファへのポインタです。
    //!                        BufferAlignSize バイト境界に配置されている必要があります。
    //! @param[in]    size     バッファサイズです。
    //!
    //! @pre
    //! - pArchive != nullptr
    //! - buffer != nullptr
    //! - buffer は @ref BufferAlignSize バイトにアラインしている
    //! - size >= GetRequiredMemSize( pArchive )
    //!
    //! @return   初期化に成功したら true を、失敗したら false を返します。
    //!
    //! @see GetRequiredMemSize
    //! @see Finalize
    //---------------------------------------------------------------------------
    bool Initialize( const SoundArchive* pArchive, void* buffer, size_t size ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    サウンドデータマネージャを破棄します。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;
    //@}

    //! @internal
    //! @param fileManager :private
    void detail_SetFileManager( detail::SoundFileManager* fileManager ) NN_NOEXCEPT
    {
        m_pFileManager = fileManager;
    }

    //----------------------------------------
    //! @name ロード
    //@{
    //---------------------------------------------------------------------------
    //! @brief  独自にロードしたデータを、ファイル管理テーブルに登録します。
    //!
    //!         あらかじめ別のアドレスが登録されている場合は、
    //!         上書きされます。
    //!
    //!         引数として渡すファイル ID は、
    //!         @ref SoundArchive::GetItemFileId 関数で、
    //!         アイテム ID から変換することができます。
    //!
    //! @param[in] fileId   セットするデータのファイル ID です。
    //! @param[in] address  関連するデータを確認するかを示すフラグです。
    //!
    //! @return   あらかじめ登録されていたアドレスを返します。
    //!
    //! @see SoundArchive::GetItemFileId
    //---------------------------------------------------------------------------
    const void* SetFileAddress( SoundArchive::FileId fileId, const void* address ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  独自にロードしたグループファイルに含まれるファイルを、
    //!         ファイル管理テーブルに登録します。
    //!
    //!         グループファイル (*.bfgrp) をサウンドアーカイブ (*.bfsar)
    //!         とは別に出力するには、SoundMaker 上のグループリストで
    //!         出力方式を [ユーザー管理] にします。
    //!
    //!         あらかじめ別のアドレスが登録されている場合は、上書きされます。
    //!
    //!         不正なグループファイルを渡すと false が返ります。
    //!
    //!         詳しくは、Demo/snd/loadGroupManually デモおよび、
    //!         当該デモのドキュメントをご覧ください。
    //!
    //!         ■ LoadData との比較
    //!
    //!         これまで提供していた @ref SoundDataManager::LoadData によるデータロードと比べて、
    //!         次のようなケースでメリットがあります。
    //!
    //!         - 「クリボー」に関するサウンドデータをひとつのグループに登録し、
    //!           そのグループの出力方式を [ユーザー管理] にします。
    //!           (グループの名前が GROUP_KURIBO だとすると、GROUP_KURIBO.bfgrp
    //!            ファイルが出力されます)
    //!
    //!         - 「クリボー」のグラフィックスデータ (モデルやテクスチャなど) と
    //!           GROUP_KURIBO.bfgrp ファイルをアーカイブ化します。
    //!           (SoundMaker は汎用アーカイバを提供していませんので、
    //!            別途ユーザー側でご用意ください)
    //!
    //!         - 「クリボー」が出現するステージで、このアーカイブをロードする
    //!
    //!         こうしておけば、クリボーが出現するステージでは、
    //!         クリボーに関するサウンドデータもロード済みであることが保証でき、
    //!         「ロードを忘れて音が鳴らない」といった状況を避ける事ができます。
    //!
    //!         @ref LoadData を使う場合、あるステージにクリボーが出現するかどうか、
    //!         なんらかの手段で判断した上で、クリボーのサウンドデータをロードする必要がありました。
    //!
    //!         端的には、各ステージのオブジェクト配置変更に強い、と言えます。
    //!
    //!         ただし、LoadData の時と比べ、本関数や @ref ClearFileAddressInGroupFile
    //!         を呼び出す必要がありますので、その点ご注意ください。
    //!
    //!         また、プレイヤーヒープを用いるサウンドが、
    //!         [ユーザー管理] のグループにのみ属している場合は、
    //!         プレイヤーヒープを使ったサウンドの再生ができませんので、ご注意ください。
    //!         (グループファイルをロードし、本関数で SoundDataManager
    //!         に登録すると再生することが可能です。プレイヤーヒープは使われません。)
    //!
    //!         プレイヤーヒープを使ってサウンドを再生したい場合は、
    //!         該当サウンドデータが下記のどちらかになるようにグループ構成を調整してください。
    //!
    //!         - [ユーザー管理] グループに含めない
    //!         - [ユーザー管理] グループに含めるが、[ユーザー管理]
    //!           でないグループ ([リンク] や [埋め込み]) にも含める
    //!
    //!
    //! @return 登録に成功すると true、不正なグループファイルを渡すと false を返します。
    //!
    //! @param[in] groupFile    ロード済みグループファイルのアドレスです。
    //! @param[in] size         上記ファイルのサイズです。
    //!
    //! @see ClearFileAddressInGroupFile
    //---------------------------------------------------------------------------
    bool SetFileAddressInGroupFile( const void* groupFile, size_t size ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  @ref SetFileAddressInGroupFile で指定したファイルを登録解除します。
    //!
    //!         サウンドスレッドをブロックする点にご注意ください。
    //!
    //!         本関数は、@ref SetFileAddressInGroupFile
    //!         で指定したグループファイルをメモリ上から解放する前に、
    //!         必ず呼び出す必要があります。
    //!
    //!         連続した領域にグループファイルを複数ロードしているケースで、
    //!         その複数グループファイルをメモリ上から解放したい場合は、
    //!         本関数を 1 回にまとめて呼び出すことが可能です。
    //!         その場合、groupFile に先頭アドレスを、size
    //!         に先頭アドレス～終端アドレスの差を渡します。
    //!
    //! @param[in] groupFile    登録解除するグループファイルのアドレスです。
    //! @param[in] size         上記ファイルのサイズです。
    //!
    //! @see SetFileAddressInGroupFile
    //---------------------------------------------------------------------------
    void ClearFileAddressInGroupFile( const void* groupFile, size_t size ) NN_NOEXCEPT;
    //@}

    //! @internal
    //! @param fileId :private
    //! @return :private
    const void* detail_GetFileAddress( SoundArchive::FileId fileId ) const NN_NOEXCEPT;

    //! @internal
    //! @param address :private
    //! @return :private
    SoundArchive::FileId detail_GetFileIdFromTable( const void* address ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  独自にロードしたデータを、ファイル管理テーブルから破棄します。
    //!
    //!         サウンドスレッドをブロックする点にご注意ください。
    //!
    //!         本関数は、独自にロードしたデータをメモリ上から解放する前に、
    //!         必ず呼び出す必要があります。
    //!
    //!         連続した領域に独自にロードしたデータを複数ロードしているケースで、
    //!         そのデータをメモリ上から解放したい場合は、
    //!         本関数を 1 回にまとめて呼び出すことが可能です。
    //!         その場合にはその連続領域の先頭アドレスを渡し、size
    //!         には先頭アドレス～終端アドレスの差を渡します。
    //!
    //! @param[in] address      破棄するデータの先頭アドレスです。
    //! @param[in] size         上記データのサイズです。
    //!
    //! @see SetFileAddress
    //---------------------------------------------------------------------------
    void InvalidateSoundData( const void* address, size_t size ) NN_NOEXCEPT;

protected:
    //! @internal
    //! @param start :private
    //! @param end :private
    virtual void InvalidateData( const void* start, const void* end ) NN_NOEXCEPT NN_OVERRIDE;

    //! @internal
    //! @param fileId :private
    //! @param address :private
    //! @return :private
    virtual const void* SetFileAddressToTable(
            SoundArchive::FileId fileId, const void* address ) NN_NOEXCEPT NN_OVERRIDE;
    //! @internal
    //! @param fileId :private
    //! @return :private
    virtual const void* GetFileAddressFromTable( SoundArchive::FileId fileId ) const NN_NOEXCEPT NN_OVERRIDE;

    //! @internal
    //! @param fileId :private
    //! @return :private
    virtual const void* GetFileAddressImpl( SoundArchive::FileId fileId ) const NN_NOEXCEPT NN_OVERRIDE;
        // テーブルを含め、アクセス可能な箇所にデータが読み込まれているかを確認する

private:
    //! @internal
    struct FileAddress
    {
        const void* address;
    };

    //! @internal
    typedef detail::Util::Table<FileAddress> FileTable;

    //! @internal
    bool CreateTables( void** pOutBuffer, const SoundArchive* pArchive, void* endAddress ) NN_NOEXCEPT;

    //! @internal
    FileTable* m_pFileTable;

    //! @internal
    detail::SoundFileManager* m_pFileManager;
};

} // namespace nn::atk
} // namespace nn

