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

#include <nn/atk/atk_Config.h>
#ifdef NN_ATK_CONFIG_ENABLE_DEV

#include <nn/atk/fnd/basis/atkfnd_Memory.h>
#include <nn/atk/fnd/basis/atkfnd_Time.h>
#include <nn/atk/viewer/detail/hio/atk_HioManager.h>
#include <nn/atk/viewer/detail/hio/atk_HioSyncChannel.h>
#include <nn/atk/viewer/detail/hio/atk_HioAsyncChannel.h>
#include <nn/atk/viewer/detail/handler/atk_SyncReplyHandler.h>
#include <nn/atk/viewer/detail/handler/atk_QueryItemsReplyHandler.h>

namespace nn {
namespace atk {

namespace detail {
namespace fnd {
class FileStream;
}
}

namespace viewer {

class SoundArchiveEditor;

namespace detail {

class IErrorProvider;
class HioSyncChannel;
class HioAsyncChannel;
class QueryItemInfoPacket;

//---------------------------------------------------------------------------
//! @internal
//! @brief  サウンド編集のコネクションを管理します。
//---------------------------------------------------------------------------
class SoundEditConnection
{
private: // 定数の定義

public: // 型の定義
    //---------------------------------------------------------------------------
    //! @brief  初期化パラメータです。
    //---------------------------------------------------------------------------
    struct InitializeArgs
    {
        InitializeArgs() NN_NOEXCEPT :
            buffer(NULL),
            bufferLength(0),
            cacheSyncInterval(0),
            sendTimeout(0),
            maxItemName(0),
            hioManager(NULL),
            syncChannel(NULL),
            funcChannel(NULL)
        {
        }

        void*            buffer;            //!< SoundEditConnection が使用するバッファです。
        size_t           bufferLength;      //!< SoundEditConnection が使用するバッファの長さです。
        uint32_t         cacheSyncInterval; //!< キャッシュの同期間隔（ミリ秒）です。
        uint32_t         sendTimeout;       //!< 送信タイムアウト（ミリ秒）です。
        int              maxItemName;       //!< アイテム名の長さの上限値です。
        HioManager*      hioManager;        //!< HIO マネージャです。
        HioAsyncChannel* syncChannel;       //!< SYNC 用非同期チャンネルです。
        HioSyncChannel*  funcChannel;       //!< 機能チャンネルです。
    };

public: // コンストラクタ
    SoundEditConnection() NN_NOEXCEPT;

public: // メソッド
    //---------------------------------------------------------------------------
    //! @brief         初期化します。
    //!
    //! @param args TBD
    //!
    //! @return TBD
    //---------------------------------------------------------------------------
    Result Initialize(const InitializeArgs& args) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief         終了処理を行います。
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      必要なメモリサイズを取得します。
    //!
    //! @param maxItemName TBD
    //!
    //! @return TBD
    //---------------------------------------------------------------------------
    size_t GetRequiredMemorySize(int maxItemName) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ファイルを開くのに必要なメモリサイズを取得します。
    //!
    //! @return TBD
    //---------------------------------------------------------------------------
    static size_t GetRequiredMemorySizeForOpenFile() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief   サウンド編集コネクションを開きます。
    //!
    //! @param soundArchiveEditor TBD
    //!
    //! @return TBD
    //---------------------------------------------------------------------------
    Result Open(SoundArchiveEditor& soundArchiveEditor) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief   サウンド編集コネクションを閉じます。
    //---------------------------------------------------------------------------
    void Close() NN_NOEXCEPT;

    void Update() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief   アイテムのキャッシュ更新開始を通知します。
    //!
    //! @param itemName TBD
    //!
    //! @return TBD
    //---------------------------------------------------------------------------
    Result BeginUpdateItemCache(const char* itemName) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief   アイテムのキャッシュ削除開始を通知します。
    //!
    //! @param itemName TBD
    //!
    //! @return TBD
    //---------------------------------------------------------------------------
    Result BeginRemoveItemCache(const char* itemName) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief   全アイテムのキャッシュ削除開始を通知します。
    //!
    //! @return TBD
    //---------------------------------------------------------------------------
    Result BeginRemoveAllItemCaches() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief   アイテムのキャッシュ更新終了を通知します。
    //!
    //! @param itemCacheResult TBD
    //! @param itemName TBD
    //!
    //! @return TBD
    //---------------------------------------------------------------------------
    Result EndUpdateItemCache(Result itemCacheResult, const char* itemName) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief   アイテムのキャッシュ削除終了を通知します。
    //!
    //! @param itemCacheResult TBD
    //! @param itemName TBD
    //!
    //! @return TBD
    //---------------------------------------------------------------------------
    Result EndRemoveItemCache(Result itemCacheResult, const char* itemName) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief   全アイテムのキャッシュ削除終了を通知します。
    //!
    //! @param itemCacheResult TBD
    //!
    //! @return TBD
    //---------------------------------------------------------------------------
    Result EndRemoveAllItemCaches(Result itemCacheResult) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief   アイテム情報を問い合わせます。
    //!
    //! @param replyPacket TBD
    //! @param itemName TBD
    //!
    //! @return TBD
    //---------------------------------------------------------------------------
    Result QueryItemInfo(const HioPacket*& replyPacket, const char* itemName) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief   ファイルを開きます。
    //---------------------------------------------------------------------------
    atk::detail::fnd::FileStream* OpenFile(void* buffer, size_t bufferLength, const char* filePath) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ファイルの内容をすべて読み込みます。
    //!
    //! @param filePath TBD
    //! @param allocator TBD
    //! @param alignment TBD
    //! @param size TBD
    //! @param result TBD
    //!
    //! @return TBD
    //---------------------------------------------------------------------------
    const void* ReadFileData(
        const char* filePath,
        atk::detail::fnd::IAllocator& allocator,
        int alignment = atk::detail::fnd::MemoryTraits::DefaultAlignment,
        size_t* size = NULL,
        Result* result= NULL) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief         初期化の有無を取得します。
    //!
    //! @return        初期化済みの場合は true、初期化されていない場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsInitialized() const NN_NOEXCEPT { return m_SyncChannel != NULL; }

    //---------------------------------------------------------------------------
    //! @brief         接続の有無を取得します。
    //!
    //! @return        接続済みの場合は true、接続されていない場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsOpened() const NN_NOEXCEPT
    {
        return m_SoundArchiveEditor != NULL;
    }

    //---------------------------------------------------------------------------
    //! @brief         関連付けられたサウンドアーカイブエディタを取得します。
    //!
    //! @return TBD
    //---------------------------------------------------------------------------
    SoundArchiveEditor* GetSoundArchiveEditor() const NN_NOEXCEPT
    {
        return m_SoundArchiveEditor;
    }

private: // メソッド
    void SetupSyncChannel(uint32_t maxItemName) NN_NOEXCEPT;
    void UnsetupSyncChannel() NN_NOEXCEPT;
    bool BeginCacheSync() NN_NOEXCEPT;

private: // メンバ変数
    SoundArchiveEditor* m_SoundArchiveEditor;           //!< 関連するサウンドアーカイブエディタです。
    HioManager*         m_HioManager;                   //!< HIO マネージャです。
    HioAsyncChannel*    m_SyncChannel;                  //!< SYNC 用非同期チャンネルです。
    HioSyncChannel*     m_FuncChannel;                  //!< 機能チャンネルです。

    uint32_t                           m_CacheSyncInterval;  //!< キャッシュの同期間隔です。
    atk::detail::fnd::StopWatch m_CacheSyncStopWatch; //!< キャッシュの同期間隔を示すストップウォッチです。
    uint32_t                           m_SendTimeout;        //!< 送信タイムアウト時間です。

    QueryItemsReplyHandler m_QueryItemsReplyHandler;    //!< QueryItemsReplyPacket のハンドラです。
    HioPacket*             m_SendPacketBuffer;          //!< パケット送信用のバッファです。
    size_t                    m_SendPacketBufferSize;      //!< パケット送信用のバッファサイズです。

};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
