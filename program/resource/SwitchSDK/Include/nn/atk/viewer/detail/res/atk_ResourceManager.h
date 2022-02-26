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

#include <nn/util/util_IntrusiveList.h>
#include <nn/atk/fnd/basis/atkfnd_Memory.h>
#include <nn/atk/viewer/atk_Types.h>
#include <nn/atk/viewer/atk_Result.h>
#include <nn/atk/viewer/detail/res/atk_ResItemInfo.h>
#include <nn/atk/viewer/detail/res/atk_CacheManager.h>
#include <nn/atk/viewer/detail/res/atk_ResourceAllocator.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

//---------------------------------------------------------------------------
//! @internal
//! @brief  リソースを管理するクラスです。
//---------------------------------------------------------------------------
class ResourceManager
{
private: // 定数
    static const size_t ResourceAllocationThresholdSize = 64 * 1024;

public: // 型の定義
    //! @brief  リソースの状態です。
    enum ResourceState
    {
        ResourceState_Unknown,     //!< 無効値
        ResourceState_Updating,    //!< 更新中
        ResourceState_Removing     //!< 削除中
    };

    class ErrorItem
    {
    public: // 型の定義
        //! @brief IntrusiveList 用の参照クラスです。
        struct Reference
        {
            ErrorItem* value;            //!< Item です。
            util::IntrusiveListNode node;  //!< IntrusiveListNode です。
        };
        typedef util::IntrusiveList<Reference, util::IntrusiveListMemberNodeTraits<Reference, &Reference::node>> ReferenceList;

    public:
        ErrorItem() NN_NOEXCEPT;
        ~ErrorItem() NN_NOEXCEPT;

    public:
        const char* GetItemName() const NN_NOEXCEPT { return m_ItemName; }
        void SetItemName(const char* itemName) NN_NOEXCEPT;

        char* AttachItemNameBuffer(char* itemNameBuffer, int itemNameLength) NN_NOEXCEPT;

        nn::atk::viewer::Result GetResult() const NN_NOEXCEPT { return m_Result; }
        void SetResult(nn::atk::viewer::Result result) NN_NOEXCEPT { m_Result = result; }

        Reference& GetReference() NN_NOEXCEPT { return m_Reference; }

    private:
        char* m_ItemName;
        int m_ItemNameLength;
        nn::atk::viewer::Result m_Result;

        Reference m_Reference;          //!< LinkList 用の参照です。
    };

private: // 型の定義
    typedef void (*CacheAction)(CacheManager& cacheManager, const char* name);

    //----------------------------------------------------------
    //! @brief  キャッシュ名関数オブジェクトへのアダプタです。
    template<class FunctorType>
    class ItemNameFunctorAdaptor
    {
        NN_DISALLOW_COPY(ItemNameFunctorAdaptor);

    public:
        explicit ItemNameFunctorAdaptor(FunctorType& functor) NN_NOEXCEPT : m_Functor(functor) { }

    public:
        void operator()(CacheManager::Item& item) NN_NOEXCEPT
        {
            m_Functor(item.GetName().GetName());
        }

    private:
        FunctorType& m_Functor;
    };

public: // コンストラクタ
    //! @brief  コンストラクタ
    ResourceManager() NN_NOEXCEPT;

    //! @brief  デストラクタ
    ~ResourceManager() NN_NOEXCEPT {}

public: // メソッド
    //! @brief  初期化します。
    //! @param heap TBD
    //! @param baseID TBD
    //! @param maxItems TBD
    //! @param maxFiles TBD
    //! @param maxName TBD
    //! @param isMaxItemsExpandable TBD
    //! @param isMaxFilesExpandable TBD
    //! @return TBD
    Result Initialize(
        atk::detail::fnd::FrameHeap& heap,
        uint32_t baseID,
        int maxItems,
        int maxFiles,
        int maxName,
        bool isMaxItemsExpandable,
        bool isMaxFilesExpandable) NN_NOEXCEPT;

    //! @brief  終了処理を行います。
    void Finalize() NN_NOEXCEPT;

    //! @brief  初期化の有無を取得します。
    //! @return TBD
    bool IsInitialized() const NN_NOEXCEPT { return m_DataCacheHeap.IsInitialized() && m_PlaybackHeap.IsInitialized(); }

    //! @brief  現在のメモリ使用量を取得します。
    //! @return TBD
    size_t GetMemoryUsage() const NN_NOEXCEPT;

    //! @brief  アイテム数を取得します。
    //! @return TBD
    int GetItemInfoCount() const NN_NOEXCEPT { return m_ItemInfoManager.GetItemCount(); }

    //! @brief  アイテム数の最大値を取得します。
    //! @return TBD
    int GetMaxItemInfoCount() const NN_NOEXCEPT { return m_ItemInfoManager.GetMaxItemCount(); }

    //! @brief  アイテム編集情報を取得します。
    //! @param index TBD
    //! @param pItemInfo TBD
    //! @return TBD
    bool TryGetEditItemInfo(int index, EditItemInfo* pItemInfo) const NN_NOEXCEPT;

    //! @brief  アイテム編集情報を取得します。
    //! @param name TBD
    //! @param pItemInfo TBD
    //! @return TBD
    bool TryGetEditItemInfo(const char* name, EditItemInfo* pItemInfo) const NN_NOEXCEPT;

    //! @brief  アイテム編集情報を取得します。
    //! @param id TBD
    //! @param pItemInfo TBD
    //! @return TBD
    bool TryGetEditItemInfoFromID(uint32_t id, EditItemInfo* pItemInfo) const NN_NOEXCEPT;

    //! @brief  ファイル数を取得します。
    //! @return TBD
    uint32_t GetFileCount() const NN_NOEXCEPT { return m_FileManager.GetItemCount(); }

    //! @brief  ファイル数の最大値を取得します。
    //! @return TBD
    uint32_t GetMaxFileCount() const NN_NOEXCEPT { return m_FileManager.GetMaxItemCount(); }

    //! @brief  アイテム辞書拡張の有無を取得します。
    //! @return TBD
    bool IsItemInfoDictionaryExpandable() const NN_NOEXCEPT { return m_IsItemInfoDictionaryExpandable; }

    //! @brief  ファイル辞書拡張の有無を取得します。
    //! @return TBD
    bool IsFileDictionaryExpandable() const NN_NOEXCEPT { return m_IsFileDictionaryExpandable; }

    //! @brief  指定した名前のアイテム情報の有無を確認します。
    //! @param name TBD
    //! @return TBD
    bool ContainsItemInfo(const char* name) const NN_NOEXCEPT;

    //! @brief  指定した名前のアイテムIDを取得します。
    //! @param name TBD
    //! @param id TBD
    //! @return TBD
    bool TryGetItemID(const char* name, uint32_t* id) const NN_NOEXCEPT;

    //! @brief  指定した名前のアイテムのキャッシュ状態を取得します。
    //! @param name TBD
    //! @return TBD
    CacheState GetItemCacheState(const char* name) const NN_NOEXCEPT;

    //! @brief  指定した名前のアイテム情報を取得します。
    //! @param name TBD
    //! @param itemInfoDataType TBD
    //! @param allowUnfreezedData TBD
    //! @return TBD
    const void* GetItemInfo(
        const char* name,
        ResDataType* itemInfoDataType = nullptr,
        bool allowUnfreezedData = false) const NN_NOEXCEPT;

    //! @brief 指定した名前のアイテムデータタイプを取得します。
    //! @param itemName TBD
    //! @param result TBD
    //! @return TBD
    ResDataType GetItemInfoDataType(const char* itemName, Result* result) const NN_NOEXCEPT;

    //! @brief  指定した名前のアイテム情報を設定します。
    //! @param soundName TBD
    //! @param originalId TBD
    //! @param itemType TBD
    //! @param itemInfo TBD
    //! @return TBD
    Result SetItemInfo(const char* soundName, uint32_t originalId, ResDataType itemType, const void* itemInfo) NN_NOEXCEPT;

    //! @brief  指定したファイルデータを取得します。
    //! @param filePath TBD
    //! @param fileDataType TBD
    //! @param hashCode TBD
    //! @return TBD
    const void* GetFile(
        const char* filePath,
        ResDataType fileDataType,
        const Hash32** hashCode = nullptr) const NN_NOEXCEPT;

    //! @brief  指定したファイルの参照カウントをインクリメントします。
    //! @param filePath TBD
    //! @return TBD
    Result IncrementFileReferenceCount(const char* filePath) NN_NOEXCEPT;

    //! @brief  指定したファイルの参照カウントをデクリメントします。
    //! @param filePath TBD
    //! @return TBD
    Result DecrementFileReferenceCount(const char* filePath) NN_NOEXCEPT;

    //! @brief  新しいアイテム情報エントリーを追加ます。
    //! @param name TBD
    //! @param originalId TBD
    //! @return TBD
    Result NewItemInfoEntry(const char* name, uint32_t originalId) NN_NOEXCEPT;

    //! @brief  新しいファイルエントリーを作成します。
    //! @param filePath TBD
    //! @param fileSize TBD
    //! @param fileDataType TBD
    //! @param filebuffer TBD
    //! @param hashCode TBD
    //! @return TBD
    Result NewFileEntry(
        const char* filePath,
        size_t fileSize,
        ResDataType fileDataType,
        void** filebuffer,
        Hash32* hashCode = nullptr) NN_NOEXCEPT;

    //! @brief  すべてのアイテム情報を削除します。
    void RemoveAllItemInfos() NN_NOEXCEPT;

    //! @brief  すべてのアイテムファイルを削除します。
    void RemoveAllItemFiles() NN_NOEXCEPT;

    //! @brief  すべての未使用アイテムファイルを削除します。
    void RemoveAllGarbageItemFiles() NN_NOEXCEPT;

    //! @brief  すべてのアイテムファイルの参照カウントをクリアします。
    void ClearAllItemFileReferenceCounts() NN_NOEXCEPT;

    //! @brief  指定した名前のアイテム情報と関連するファイルを削除します。
    //! @param name TBD
    //! @return TBD
    Result RemoveItemData(const char* name) NN_NOEXCEPT;

    //! @brief  指定した名前のアイテムに関連するファイルを削除します。
    //! @param name TBD
    //! @return TBD
    Result RemoveItemFiles(const char* name) NN_NOEXCEPT;

    //! @brief  指定した名前のアイテムに関連するファイルの参照カウントをデクリメントします。
    //! @param name TBD
    //! @return TBD
    Result DecrementReferenceCountForItemFiles(const char* name) NN_NOEXCEPT;

    //! @brief  指定ファイルデータを削除します。
    //! @param filePath TBD
    //! @return TBD
    void RemoveFile(const char* filePath) NN_NOEXCEPT;

    //! @brief  すべてのアイテムに指定関数オブジェクトを適用します。
    //! @param functor TBD
    //! @tparam FunctorType TBD
    template<class FunctorType>
    void ForEachItemInfo(FunctorType& functor) NN_NOEXCEPT
    {
        ItemNameFunctorAdaptor<FunctorType> adaptor(functor);
        m_ItemInfoManager.ForEach(adaptor);
    }

    //! @brief  最初の非フリーズアイテム名を取得します。
    //! @param state TBD
    //! @return TBD
    const char* GetFirstUnfreezedItemName(ResourceState* state = nullptr) const NN_NOEXCEPT;

    //! @brief  指定した名前のアイテム情報をフリーズします。
    //! @param itemName TBD
    //! @param state TBD
    //! @return TBD
    Result FreezeItemInfo(const char* itemName, ResourceState* state = nullptr) NN_NOEXCEPT;

    //! @brief  すべてのアイテム情報のフリーズ状態を解除します。
    //! @param state TBD
    //! @return TBD
    Result UnfreezeAllItemInfos(ResourceState state) NN_NOEXCEPT;

    //! @brief  指定したアイテム情報のフリーズ状態を解除します。
    //! @param itemName TBD
    //! @param state TBD
    //! @return TBD
    Result UnfreezeItemInfo(const char* itemName, ResourceState state) NN_NOEXCEPT;

    //! @brief  すべてのアイテムを対象とした状態を取得します。
    //! @return TBD
    ResourceState GetAllItemsState() const NN_NOEXCEPT { return static_cast<ResourceState>(m_AllItemsState); }

    //! @brief  すべてのアイテムを対象とした状態を設定します。
    //! @param value TBD
    void SetAllItemsState(ResourceState value) NN_NOEXCEPT { m_AllItemsState = value; }

    //! @brief  アイテム情報辞書を更新します。
    void UpdateItemInfoDictionary() NN_NOEXCEPT;

    //! @brief  ファイル辞書を更新します。
    void UpdateFileDictionary() NN_NOEXCEPT;

    //! @internal
    //! @param itemName :private
    //! @param result :private
    //! @return :private
    ErrorItem* CreateErrorItem(const char* itemName, nn::atk::viewer::Result result) NN_NOEXCEPT;

    //! @internal
    //! @param item :private
    void DestroyErrorItem(ErrorItem* item) NN_NOEXCEPT;

    //! @internal
    //! @param item :private
    void PushErrorItem(ErrorItem* item) NN_NOEXCEPT;

    //! @internal
    //! @return :private
    ErrorItem* PopErrorItem() NN_NOEXCEPT;

    //! @internal
    void DestroyAllErrorItems() NN_NOEXCEPT;

private: // メソッド
    //! @brief  アイテム情報管理に必要なメモリサイズを取得します。
    size_t GetItemInfoManagerRequiredMemorySize(int maxItems, int maxFiles, int maxName) const NN_NOEXCEPT;

    //! @brief  アイテム情報管理クラスを初期化します。
    Result InitializeItemInfoManager(uint32_t baseID, int maxEntries, int maxName) NN_NOEXCEPT;

    //! @brief  ファイル管理クラスを初期化します。
    Result InitializeFileManager(int maxEntries) NN_NOEXCEPT;

    //! @brief  アイテム情報辞書を拡張します。
    Result RebuildItemInfoDictionary(int maxEntries) NN_NOEXCEPT;

    //! @brief  ファイル辞書を拡張します。
    Result RebuildFileDictionary(int maxEntries) NN_NOEXCEPT;

    //! @brief  アイテム情報データのバッファサイズを取得します。
    size_t GetItemInfoDataBufferLength(int maxEntries, int maxName, size_t maxDataLength) const NN_NOEXCEPT;

    //! @brief  指定したアイテム名に関連するすべてのファイルキャッシュに対して指定アクションを実行します。
    Result ForeachFileCache(const char* name, CacheAction action) NN_NOEXCEPT;

    //! @internal
    size_t GetErrorItemHeapRequiredMemorySize(int maxName) const NN_NOEXCEPT;

private: // メンバ変数
    CacheManager                     m_ItemInfoManager;             //!< アイテム情報の管理クラスです。
    CacheManager                     m_FileManager;                 //!< アイテムファイル情報の管理クラスです。
    void*                            m_pMemoryForItemInfoManager;   //!< @internal
    void*                            m_pMemoryForFileManager;       //!< @internal

    ErrorItem::ReferenceList         m_ErrorItemList;               //!< @internal
    void*                            m_pMemoryForErrorItemList;     //!< @internal

    void*                            m_pMemoryForPlaybackHeap;      //!< @internal

    atk::detail::fnd::ExpandedHeap m_DataCacheHeap;        //!< データキャッシュ用のヒープです。
    atk::detail::fnd::ExpandedHeap m_ErrorItemHeap;        //!< エラーアイテム用のヒープです。
    atk::detail::fnd::ExpandedHeap m_PlaybackHeap;         //!< 再生用のバッファのヒープです。
    ResourceAllocator                m_ResourceAllocator;    //!< アイテム情報のアロケータです。(m_DataCacheHeap の後ろから割り当て）
    ResourceAllocator                m_PlaybackBufferAllocator;  //!< 再生用のバッファのアロケータです。

    nn::audio::MemoryPoolType        m_PlaybackMemoryPool;          //!< 再生用のバッファのメモリプールです。
    bool                             m_IsPlaybackMemoryPoolAttached; //!< 再生用のバッファのメモリプールがアタッチされているかです。

    uint32_t  m_AllItemsState;                     //!< すべてのアイテムを対象とした状態です。
    int  m_MaxName;
    bool m_IsItemInfoDictionaryExpandable;    //!< アイテム情報辞書拡張の有無です。
    bool m_IsFileDictionaryExpandable;        //!< ファイル辞書拡張の有無です。
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t m_Padding[2];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
};


} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
