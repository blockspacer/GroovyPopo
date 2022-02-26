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

#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>
#include <nn/atk/viewer/atk_Types.h>
#include <nn/atk/viewer/atk_Result.h>
#include <nn/atk/viewer/atk_Canceller.h>
#include <nn/atk/viewer/detail/res/atk_ResItemInfo.h>

namespace nn {
namespace atk {

class SoundHandle;

namespace detail {

class BasicSound;

namespace fnd {
class FileStream;
}
}

namespace viewer {
namespace detail {

class IErrorProvider;
class SoundEditConnection;
class ResourceManager;

//---------------------------------------------------------------------------
//! @brief  リソースを操作するクラスです。
//---------------------------------------------------------------------------
class SoundArchiveEditController
{
public: // 型の定義
    typedef bool (*FilterFunc)(uint32_t dataType);

    //! @brief  初期化パラメータです。
    struct InitializeArgs
    {
        IErrorProvider*      errorProvider;
        void*                buffer;            //!< OpenFile 用バッファです。
        size_t               bufferLength;      //!< buffer の長さです。
        int                  maxItemName;       //!< アイテム名の長さの最大値です。
        ResourceManager*     resourceManager;
        SoundArchive*        soundArchive;
        SoundArchivePlayer*  soundArchivePlayer;

        InitializeArgs() NN_NOEXCEPT :
            errorProvider(NULL),
            buffer(NULL),
            bufferLength(0),
            maxItemName(0),
            resourceManager(NULL),
            soundArchive(NULL),
            soundArchivePlayer(NULL)
        { }
    };

private: // 型の定義
    class ResSoundInfoApplier;

public: // コンストラクタ
    //! @brief  コンストラクタ
    SoundArchiveEditController() NN_NOEXCEPT;

    //! @brief  デストラクタ
    ~SoundArchiveEditController() NN_NOEXCEPT {}

public: // メソッド
    //! @brief  初期化します。
    //! @param args TBD
    //! @return TBD
    Result Initialize(const InitializeArgs& args) NN_NOEXCEPT;

    //! @brief  終了処理を行います。
    void Finalize() NN_NOEXCEPT;

    //! @brief  編集を開始します。
    //! @param connection TBD
    //! @return TBD
    Result Start(SoundEditConnection* connection) NN_NOEXCEPT;

    //! @brief  編集を停止します。
    void Stop() NN_NOEXCEPT;

    //! @brief  必要なメモリサイズを取得します。
    //! @param maxItemName TBD
    //! @return TBD
    size_t GetRequiredMemorySize(int maxItemName) const NN_NOEXCEPT;

    //! @brief  合計メモリ使用量を取得します。
    //! @return TBD
    size_t GetTotalMemoryUsage() const NN_NOEXCEPT;

    //! @brief  現在、メモリ不足状態かどうかを調べます。
    //! @return TBD
    bool IsOutOfMemory() const NN_NOEXCEPT { return m_MemoryUsageAtOutOfMemory > 0; }

    //! @brief  アイテム数のオーバーフローの有無を取得します。
    //! @return TBD
    bool IsItemInfosOverflow() const NN_NOEXCEPT { return m_IsItemInfosOverFlow; }

    //! @brief  ファイル数のオーバーフローの有無を取得します。
    //! @return TBD
    bool IsFilesOverflow() const NN_NOEXCEPT { return m_IsFilesOverFlow; }

    //! @brief  初期化の有無を取得します。
    //! @return TBD
    bool IsInitialized() const NN_NOEXCEPT { return m_ResourceManager != NULL; }

    //! @brief  編集開始の有無を取得します。
    //! @return TBD
    bool IsStarted() const NN_NOEXCEPT { return IsInitialized() && m_Connection != NULL; }

    //! @brief  編集アイテムの変更通知コールバックを設定します。
    //! @param callback TBD
    //! @param callbackParam TBD
    void SetEditItemsChangedCallback(EditItemsChangedCallback callback, uintptr_t callbackParam) NN_NOEXCEPT;

    //! @brief  キャッシュ状態をロックします。
    void LockCacheStates() NN_NOEXCEPT;

    //! @brief  キャッシュ状態のロックを解除します。
    void UnlockCacheStates() NN_NOEXCEPT;

    //! @brief  キャッシュを更新します。
    //! @param canceller TBD
    //! @param cancellerParam TBD
    //! @return TBD
    Result UpdateCaches(ICanceller* canceller = NULL, void* cancellerParam = NULL) NN_NOEXCEPT;

    //! @brief  指定した名前のサウンドパラメータ編集情報を取得します。
    //! @param name TBD
    //! @return TBD
    CacheState GetItemCacheState(const char* name) const NN_NOEXCEPT;

    //! @brief  指定した名前のサウンドパラメータ編集情報を取得します。
    //! @param name TBD
    //! @param filter TBD
    //! @param itemInfoDataType TBD
    //! @return TBD
    const ResSoundInfo* GetSoundInfo(
        const char* name,
        FilterFunc filter = NULL,
        ResDataType* itemInfoDataType = NULL) const NN_NOEXCEPT;

    //! @brief  指定した名前のバンクパラメータ編集情報を取得します。
    //! @param name TBD
    //! @return TBD
    const ResBankInfo* GetBankInfo(const char* name) const NN_NOEXCEPT;

    //! @brief  指定バッファを使って指定アイテムラベルに関連するファイルを開きます。
    //! @param buffer TBD
    //! @param bufferLength TBD
    //! @param filePath TBD
    //! @return TBD
    atk::detail::fnd::FileStream* OpenFile(void* buffer, size_t bufferLength, const char* filePath) NN_NOEXCEPT;

    //! @brief  指定したファイルデータを取得します。
    //! @param filePath TBD
    //! @param fileDataType TBD
    //! @param hashCode TBD
    //! @return TBD
    const void* GetFile(
        const char* filePath,
        ResDataType fileDataType,
        const Hash32** hashCode = NULL) const NN_NOEXCEPT;

    //! @brief  全アイテム情報の更新を遅延処理します。
    void InvokeUpdateAllItemInfos() NN_NOEXCEPT;

    //! @brief  全アイテム情報の削除を遅延処理します。
    void InvokeRemoveAllItemInfos() NN_NOEXCEPT;

    //! @brief  指定アイテム情報の削除を遅延処理します。
    //! @param name TBD
    //! @return TBD
    Result InvokeRemoveItemInfo(const char* name) NN_NOEXCEPT;

    //! @brief  アイテム情報を編集可能状態にします。
    //! @param name TBD
    //! @return TBD
    Result MakeItemInfoEditable(const char* name) NN_NOEXCEPT;

    //! @brief  リソースマネージャを取得します。
    //! @return TBD
    ResourceManager* GetResourceManager() NN_NOEXCEPT { return m_ResourceManager; }

    //! @brief  リソースマネージャを取得します。
    //! @return TBD
    const ResourceManager* GetResourceManager() const NN_NOEXCEPT { return m_ResourceManager; }

private: // メソッド
    //! @brief  プラットフォーム向けの初期化を行います。
    Result InitializeForPlatform(atk::detail::fnd::FrameHeap& heap) NN_NOEXCEPT;

    //! @brief  プラットフォーム向けの終了処理を行います。
    void FinalizeForPlatform() NN_NOEXCEPT;

    //! @brief  プラットフォーム向けに必要なメモリサイズを取得します。
    size_t GetRequiredMemorySizeForPlatform() const NN_NOEXCEPT;

    //! @brief  アイテム名バッファに必要なメモリサイズを取得します。
    size_t GetItemNameBufferRequiredMemorySize(int maxItemName) const NN_NOEXCEPT;

    //! @brief  キャッシュ用に指定バッファを使って指定アイテムラベルに関連するファイルを開きます。
    atk::detail::fnd::FileStream* OpenFileForCache(void* buffer, size_t bufferLength, const char* filePath) NN_NOEXCEPT;

    //! @brief  編集アイテムの変更を通知します。
    void NotifyEditItemsChanged() NN_NOEXCEPT;

    //! @brief  すべてのアイテムを対象としたキャッシュ更新処理を行います。
    void UpdateCachesForAllItems() NN_NOEXCEPT;

    //! @brief  キャッシュを更新します。
    Result UpdateCache(const char* itemName) NN_NOEXCEPT;

    //! @brief  キャッシュを削除します。
    Result RemoveCache(const char* itemName) NN_NOEXCEPT;

    //! @brief  すべてのキャッシュを更新します。
    Result UpdateAllCaches() NN_NOEXCEPT;

    //! @brief  すべてのキャッシュを削除します。
    Result RemoveAllCaches() NN_NOEXCEPT;

    //! @brief  ウェーブサウンドデータをキャッシュします。
    Result UpdateWaveSoundFile(const ResWaveSoundInfo* currentInfo,const ResWaveSoundInfo* newInfo) NN_NOEXCEPT;

    //! @brief  シーケンスサウンドデータをキャッシュします。
    Result UpdateSequenceSoundFile(const ResSequenceSoundInfo* currentInfo,const ResSequenceSoundInfo* newInfo) NN_NOEXCEPT;

    //! @brief  ストリームサウンドデータをキャッシュします。
    Result UpdateStreamSoundFile(const ResStreamSoundInfo* currentInfo,const ResStreamSoundInfo* newInfo) NN_NOEXCEPT;

    //! @brief  バンクデータをキャッシュします。
    Result UpdateBankFile(const ResBankInfo* currentInfo,const ResBankInfo* newInfo) NN_NOEXCEPT;

    //! @brief  指定した名前のアイテム情報を問合せます。
    Result QueryItemInfo(
        const void** data,
        const char* name,
        ResDataType* itemDataType = NULL,
        FilterFunc filter = NULL) const NN_NOEXCEPT;

    //! @brief  指定ファイルパスを更新します。
    Result UpdateFile(
        const ResName* currentFilePath,
        const ResName* newFilePath,
        ResDataType fileDataType,
        size_t fileSize,
        Hash32* hashCode = NULL) NN_NOEXCEPT;

    Result ReadAndCacheFile(const char* filePath, ResDataType fileDataType, size_t fileSize, Hash32* hashCode = NULL) NN_NOEXCEPT;

    //! @brief  再生中のサウンドパラメータを更新します。
    void UpdatePlayingSoundParameters(const char* soundName) NN_NOEXCEPT;

    //! @brief  再生中のサウンドパラメータを指定サウンド編集情報を使って更新します。
    void UpdatePlayingSoundParameters(
        const char* soundName,
        const detail::ResSoundInfo* soundInfo,
        ResDataType soundInfoType) NN_NOEXCEPT;

private: // メンバ変数
    IErrorProvider*      m_ErrorProvider;       //!< エラープロバイダです。
    SoundEditConnection* m_Connection;
    ResourceManager*     m_ResourceManager;     //!< リソース管理クラスです。
    SoundArchive*        m_SoundArchive;        //!< 編集対象のサウンドアーカイブです。
    SoundArchivePlayer*  m_SoundArchivePlayer;  //!< 編集対象のサウンドアーカイブプレイヤーです。

    int  m_LockCacheStatesCount;                //!< キャッシュノロックカウントです。

    void* m_OpenFileBuffer;                     //!< OpenFile 用バッファです。
    size_t m_OpenFileBufferLength;              //!< OpenFile 用バッファの長さです。

    char* m_ItemNameBuffer;                     //!< アイテム名格納用バッファです。
    size_t m_ItemNameBufferLength;              //!< アイテム名格納用バッファの長さです。

    size_t m_MemoryUsageAtOutOfMemory;          //!< メモリ不足時のメモリ使用量
    bool m_IsItemInfosOverFlow;                 //!< アイテム数のオーバーフロー状態
    bool m_IsFilesOverFlow;                     //!< ファイル数のオーバーフロー状態
    bool m_IsItemInfoInfoDictionaryInvalid;     //!< アイテム情報辞書の無効化フラグです。

    EditItemsChangedCallback m_EditItemsChangedCallback;       //!< 編集アイテムの変更通知コールバックです。
    uintptr_t                m_EditItemsChangedCallbackParam;  //!< 編集アイテムの変更通知コールバックに渡すユーザーパラメータです。

    mutable atk::detail::fnd::CriticalSection m_UnfreezeCacheLockObject;  //!< キャッシュフリーズ解除のロックオブジェクトです。
    mutable atk::detail::fnd::CriticalSection m_CacheLockObject;          //!< キャッシュのロックオブジェクトです。
    mutable atk::detail::fnd::CriticalSection m_ErrorItemLockObject;          //!< エラー情報のロックオブジェクトです。
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  指定サウンドハンドルにサウンド情報を反映する関数オブジェクトです。
//---------------------------------------------------------------------------
class SoundArchiveEditController::ResSoundInfoApplier
{
public:
    /* ctor */ ResSoundInfoApplier(
        SoundArchive::ItemId soundID,
        const detail::ResSoundInfo* m_SoundEditInfo,
        ResDataType soundInfoType) NN_NOEXCEPT;

    //! @brief  オペレータ() です。
    //! @param soundHandle TBD
    void operator()(SoundHandle& soundHandle) NN_NOEXCEPT;

private:
    void ApplySoundInitialVolume(atk::detail::BasicSound* sound) NN_NOEXCEPT;
    void ApplyStreamSoundTrackInitialVolume(atk::detail::BasicSound* sound) NN_NOEXCEPT;

private:
    SoundArchive::ItemId          m_SoundId;        //!< 編集対象のサウンドID です。
    const detail::ResSoundInfo* m_SoundEditInfo;  //!< サウンドの編集情報です。
    ResDataType                   m_SoundInfoType;  //!< サウンド編集情報のデータ種別です。
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
