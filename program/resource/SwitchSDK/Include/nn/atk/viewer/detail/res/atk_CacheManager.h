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
#include <nn/atk/viewer/atk_Types.h>
#include <nn/atk/viewer/atk_Result.h>
#include <nn/atk/viewer/detail/res/atk_ResTypes.h>
#include <nn/atk/viewer/detail/util/atk_PatriciaDictionary.h>

namespace nn {
namespace atk {

namespace detail {
namespace fnd {
class IAllocator;
}
}

namespace viewer {
namespace detail {

class CacheManager;

//---------------------------------------------------------------------------
//! @internal
//! @brief    アイテムデータを管理するクラスです。
//---------------------------------------------------------------------------
class CacheManager
{
private:
    static uint32_t InvalidId;
    //! 新規追加アイテムが利用する最初の ID です。
    //! 実用上利用しないと想定される ID として、最初のアイテム ID 0x01000000 に 0x00800000 = 8,388608 を足した ID 番号を選択しています。
    const uint32_t InitialBaseId = 0x01800000;

public: // 型の定義
    //---------------------------------------------------------------------------
    //! @brief  初期化パラメータです。
    //---------------------------------------------------------------------------
    struct InitializeArgs
    {
        InitializeArgs() NN_NOEXCEPT;

        void*                         buffer;                //!< CacheManager が使用するバッファです。
        size_t                        bufferLength;          //!< CacheManager が使用するバッファの長さです。
        atk::detail::fnd::IAllocator* dataBufferAllocator;
        atk::detail::fnd::IAllocator* playbackBufferAllocator;
        uint32_t                      baseId;
        int                           maxEntries;
        int                           maxName;
    };

    //---------------------------------------------------------------------------
    //! @brief  アイテム情報を管理するクラスです。
    //!         アイテム名、アイテムデータは Item クラスの直後に続きます。
    //!         →Item - ResName(アイテム名) - (データ)
    //---------------------------------------------------------------------------
    class Item
    {
    public: // 型の定義
        //! @brief IntrusiveList 用の参照クラスです。
        struct Reference
        {
            Item* value;            //!< Item です。
            util::IntrusiveListNode node;  //!< IntrusiveListNode です。
        };

        typedef util::IntrusiveList<Reference, util::IntrusiveListMemberNodeTraits<Reference, &Reference::node>> ReferenceList;

    public: // コンストラクタ
        Item(
            CacheManager& owner,
            uint32_t id,
            ResName& name,
            bool isOverrideOriginal,
            size_t bufferSize,
            size_t dataSize,
            const void* data) NN_NOEXCEPT;

    public: // メソッド
        CacheManager& GetOwner() NN_NOEXCEPT { return m_Owner; }

        const CacheManager& GetOwner() const NN_NOEXCEPT { return m_Owner; }

        bool IsFreezed() const NN_NOEXCEPT { return m_IsFreezed; }

        bool IsOverrideOriginal() const NN_NOEXCEPT { return m_IsOverrideOriginal; }

        uint32_t GetID() const NN_NOEXCEPT
        {
            return m_Id;
        }

        const ResName& GetName() const NN_NOEXCEPT
        {
            const ResName* result = m_Name.GetResName();
            NN_SDK_ASSERT_NOT_NULL(result);
            return *result;
        }

        uint32_t GetDataType() const NN_NOEXCEPT { return m_DataType; }

        void SetDataType(uint32_t value) NN_NOEXCEPT
        {
            NN_SDK_ASSERT(!m_IsFreezed, "Data is already freezed.\n");
            m_DataType = value;
        }

        void* GetData() NN_NOEXCEPT
        {
            NN_SDK_ASSERT(!m_IsFreezed, "Data is already freezed.\n");
            return m_DataOffset.to_ptr();
        }

        const void* GetData() const NN_NOEXCEPT
        {
            return m_DataOffset.to_ptr();
        }

        void SetPlaybackBuffer(void* pPlaybackBuffer) NN_NOEXCEPT
        {
            m_pPlaybackBuffer = pPlaybackBuffer;
        }

        void* GetPlaybackBuffer() NN_NOEXCEPT
        {
            return m_pPlaybackBuffer;
        }

        //! @brief  データサイズを取得します。
        //! @return TBD
        size_t GetDataSize() const NN_NOEXCEPT { return m_DataSize; }

        //! @brief  キャシュのバッファサイズを取得します。
        //! @return TBD
        size_t GetBufferSize() const NN_NOEXCEPT { return m_BufferSize; }

        //! @brief  参照カウントを取得します。
        //! @return TBD
        int GetReferenceCount() const NN_NOEXCEPT { return m_ReferenceCount; }

        //! @brief  データのハッシュ値を取得します。
        //! @return TBD
        const Hash32& GetHashCode() const NN_NOEXCEPT { return m_HashCode; }

        //! @brief  データのハッシュ値を設定します。
        //! @param value TBD
        void SetHashCode(Hash32& value) NN_NOEXCEPT
        {
            NN_SDK_ASSERT(!m_IsFreezed, "Data is already freezed.\n");
            m_HashCode = value;
        }

        //! @brief  ユーザーパラメータを取得します。
        //! @return TBD
        uint32_t GetUserParameter() const NN_NOEXCEPT { return m_UserParameter; }

        //! @brief  ユーザーパラメータを設定します。
        //! @param value TBD
        void SetUserParameter(uint32_t value) NN_NOEXCEPT { m_UserParameter = value; }

        //! @brief  データを固定化します。
        void Freeze() NN_NOEXCEPT;

        //! @brief  データの固定化を解除します。
        void Unfreeze() NN_NOEXCEPT;

        //! @brief  参照カウントをインクリメントします。
        void IncrementReferenceCount() NN_NOEXCEPT;

        //! @brief  参照カウントをデクリメントします。
        void DecrementReferenceCount() NN_NOEXCEPT;

        //! @brief  参照カウントをクリアします。
        void ClearReferenceCount() NN_NOEXCEPT;

        //! @brief  キャッシュを破棄します。
        void Dispose() NN_NOEXCEPT;

        //! @brief  LinkList 用の参照を取得します。
        //! @return TBD
        Reference& GetReference() NN_NOEXCEPT { return m_Reference; }

        //! @brief  非フリーズデータ LinkList 用の参照を取得します。
        //! @return TBD
        Reference& GetUnfreezedReference() NN_NOEXCEPT { return m_UnfreezedReference; }

        //! @brief  未使用データ LinkList 用の参照を取得します。
        //! @return TBD
        Reference& GetUnusedReference() NN_NOEXCEPT { return m_UnusedReference; }

    private: // メンバ変数
        CacheManager& m_Owner;

        bool      m_IsFreezed;          //!< データの固定化フラグです。
                                        //!< データを後から設定するために利用します。
        bool      m_IsDisposed;         //!< キャッシュが破棄されたことを示します。
        bool      m_IsOverrideOriginal;      //!< オリジナルデータ オーバーライドの有無を保持します。
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
        uint8_t m_Padding[1];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif

        uint32_t  m_Id;                 //!< アイテムIDです。
        BinString m_Name;               //!< アイテム名です。
        int       m_ReferenceCount;     //!< 参照カウントです。
        uint32_t  m_DataType;           //!< データ種別です。
        Offset    m_DataOffset;         //!< Item, ResName(アイテム名) に続くデータへのオフセットです。
        size_t    m_DataSize;           //!< データの長さです。
        size_t    m_BufferSize;         //!< Item, ResName(アイテム名), データをまとめたバッファの長さです。
        Hash32    m_HashCode;           //!< データのハッシュ値です。
        uint32_t  m_UserParameter;      //!< ユーザーパラメータです。

        void*     m_pPlaybackBuffer;    //!< 再生用バッファへのポインタです。

        Reference m_Reference;          //!< LinkList 用の参照です。
        Reference m_UnfreezedReference; //!< 非フリーズデータ LinkList 用の参照です。
        Reference m_UnusedReference;    //!< 未使用データ LinkList 用の参照です。
    };

    typedef bool (*FilterFunc)(Item& item);

    typedef void (*Action)(Item& item);

public: // コンストラクタ
    //! @brief  コンストラクタ
    CacheManager() NN_NOEXCEPT;

    //! @brief  デストラクタ
    ~CacheManager() NN_NOEXCEPT {}

public: // メソッド
    //! @brief    初期化します。
    //! @details  maxEntries と maxDataSize は 両方 0 以上か、両方 0 以下である必要があります。
    //!           両方 0 以下の場合は、一切キャッシュされません。
    //! @param args TBD
    //! @return   TBD
    Result Initialize(const InitializeArgs& args) NN_NOEXCEPT;

    //! @brief  終了処理を行います。
    void Finalize() NN_NOEXCEPT;

    //! @brief  初期化の有無を調べます。
    //! @return TBD
    bool IsInitialized() const NN_NOEXCEPT { return m_DataBufferAllocator != NULL && m_PlaybackBufferAllocator != nullptr; }

    //! @brief  必要なメモリサイズを取得します。
    //! @param maxEntries TBD
    //! @return TBD
    size_t GetRequiredMemorySize(int maxEntries) const NN_NOEXCEPT;

    //! @brief  アイテムを作成するのに必要なメモリサイズを取得します。
    //! @param maxName TBD
    //! @param maxDataLength TBD
    //! @return TBD
    size_t GetRequiredMemorySizeForItem(int maxName, size_t maxDataLength) const NN_NOEXCEPT;

    //! @brief  現在のメモリ使用量を取得します。
    //! @return 現在のメモリ使用量を返します。
    size_t GetMemoryUsage() const NN_NOEXCEPT { return m_MemoryUsage; }

    //! @brief  空きIDを取得します。
    //! @param id TBD
    //! @return TBD
    bool TryGetFreeID(uint32_t* id) const NN_NOEXCEPT;

    //! @brief  キャッシュされているデータを取得します。
    //! @param name TBD
    //! @param dataType TBD
    //! @param allowUnfreezedData TBD
    //! @return TBD
    const void* GetData(
        const char* name,
        uint32_t* dataType = NULL,
        bool allowUnfreezedData = false) const NN_NOEXCEPT;

    //! @brief  キャッシュアイテムを取得します。
    //! @param name TBD
    //! @return TBD
    Item* GetItem(const char* name) NN_NOEXCEPT;

    //! @brief  キャッシュアイテムを取得します。
    //! @param name TBD
    //! @return TBD
    const Item* GetItem(const char* name) const NN_NOEXCEPT;

    //! @brief  キャッシュアイテムを取得します。
    //! @param index TBD
    //! @return TBD
    const Item* GetItem(int index) const NN_NOEXCEPT;

    //! @brief  キャッシュアイテムを取得します。
    //! @param id TBD
    //! @return TBD
    const Item* GetItemFromID(uint32_t id) const NN_NOEXCEPT;

    //! @brief  指定データを追加します。
    //! @param id TBD
    //! @param name TBD
    //! @param isOverrideOriginal TBD
    //! @param dataType TBD
    //! @param data TBD
    //! @param dataSize TBD
    //! @param isFileEntry ファイルエントリであるのかのフラグを指定します。
    //! @return 結果を返します。
    Result AddItem(
        uint32_t id,
        const char* name,
        bool isOverrideOriginal,
        uint32_t dataType,
        const void* data,
        size_t dataSize,
        bool isFileEntry) NN_NOEXCEPT;

    //! @brief  CreateItem() で作成したアイテムを追加します。
    //! @param item TBD
    //! @return TBD
    Result AddItem(Item& item) NN_NOEXCEPT;

    //! @brief  指定した名前のアイテムを削除します。
    //! @param name TBD
    //! @param action TBD
    //! @return TBD
    Result RemoveItem(const char* name, Action action = NULL) NN_NOEXCEPT;

    //! @brief  すべてのアイテムを削除します。
    //! @param action TBD
    void RemoveAllItems(Action action = NULL) NN_NOEXCEPT;

    //! @brief  すべての未使用アイテムを削除します。
    void RemoveGarbages() NN_NOEXCEPT;

    //! @brief  すべてのアイテムの参照カウントを 0 に設定します。
    void ClearAllItemReferenceCounts() NN_NOEXCEPT;

    //! @brief  すべてのアイテムに指定関数オブジェクトを適用します。
    //! @param functor TBD
    template<class FunctorType>
    void ForEach(FunctorType& functor) NN_NOEXCEPT
    {
        for(Item::ReferenceList::iterator it = m_Items.begin(); it != m_Items.end(); ++it)
        {
            NN_SDK_ASSERT_NOT_NULL(it->value);
            functor(*it->value);
        }
    }

    //! @brief  アイテムの変更を辞書に反映します。
    void UpdateDictionary() NN_NOEXCEPT;

    //! @brief  辞書を再構築します。
    //! @param buffer TBD
    //! @param bufferLength TBD
    //! @param maxEntries TBD
    //! @param oldBuffer TBD
    //! @return TBD
    Result RebuildDictionary(void* buffer, size_t bufferLength, int maxEntries, void** oldBuffer) NN_NOEXCEPT;

    //! @brief  アイテムエントリーを作成します。
    //!         この関数で作成したアイテムのデータはフリーズするまでの間、編集が可能です。
    //! @param item TBD
    //! @param id TBD
    //! @param name TBD
    //! @param isOverrideOriginal TBD
    //! @param dataSize TBD
    //! @return TBD
    Result CreateItemEntry(Item*& item, uint32_t id, const char* name, bool isOverrideOriginal, size_t dataSize) NN_NOEXCEPT
    {
        return CreateItem(item, id, name, isOverrideOriginal, NULL, dataSize, false);
    }

    //! @brief  ファイルエントリを作成します。
    //!         この関数で作成したファイルのデータはフリーズするまでの間、編集が可能です。
    //! @param item TBD
    //! @param id TBD
    //! @param name TBD
    //! @param isOverrideOriginal TBD
    //! @param dataSize TBD
    //! @return TBD
    Result CreateFileEntry(Item*& item, uint32_t id, const char* name, bool isOverrideOriginal, size_t fileSize) NN_NOEXCEPT
    {
        return CreateItem(item, id, name, isOverrideOriginal, NULL, fileSize, true);
    }

    //! @brief  アイテムを作成します。
    //! @param item TBD
    //! @param id TBD
    //! @param name TBD
    //! @param isOverrideOriginal TBD
    //! @param data TBD
    //! @param dataSize TBD
    //! @param isFileEntry ファイルエントリであるのかのフラグを指定します。
    //! @return TBD
    Result CreateItem(Item*& item, uint32_t id, const char* name, bool isOverrideOriginal, const void* data, size_t dataSize, bool isFileEntry) NN_NOEXCEPT;

    //! @brief  アイテム数を取得します。
    //! @return TBD
    int GetItemCount() const NN_NOEXCEPT { return m_ValidItemCount; }

    //! @brief  アイテム数の最大値を取得します。
    //! @return TBD
    int GetMaxItemCount() const NN_NOEXCEPT { return m_MaxItemCount; }

    //! @brief  最初の非フリーズアイテムを取得します。
    //! @param[in] filter TBD
    //! @return TBD
    Item* GetFirstUnfreezedItem(FilterFunc filter = NULL) const NN_NOEXCEPT;

    //! @brief  非フリーズアイテム数を取得します。
    //! @return TBD
    int GetUnfreezedItemCount() const NN_NOEXCEPT { return m_UnfreezedItems.size(); }

    //! @brief  最初の未使用フリーズアイテムを取得します。
    //! @param filter TBD
    //! @return TBD
    Item* GetFirstUnusedItem(FilterFunc filter = NULL) const NN_NOEXCEPT;

    //! @brief  未使用アイテム数を取得します。
    //! @return TBD
    int GetUnusedItemCount() const NN_NOEXCEPT { return m_UnusedItems.size(); }

private: // メソッド
    size_t GetIDTableSize(int maxEntries) const NN_NOEXCEPT
    {
        return sizeof(Item*) * static_cast<size_t>(maxEntries);
    }

    //! @brief  パトリシア木辞書を取得します。
    PatriciaDictionary* GetPatriciaDictionary() const NN_NOEXCEPT
    {
        return reinterpret_cast<PatriciaDictionary*>(m_ItemDictionaryData);
    }

    inline Result CanAddItem() const NN_NOEXCEPT;

    int FindEmptyNodeIndex() NN_NOEXCEPT;

    //! @brief  アイテムを取得します。
    Item* GetItemImpl(const char* name) const NN_NOEXCEPT;

    //! @brief  指定したアイテムを削除します。
    void RemoveItem(PatriciaDictionary::Node& node, Action action = NULL) NN_NOEXCEPT;

    //! @brief  アイテムを破棄します。
    void DisposeItem(Item& item, Action action = NULL) NN_NOEXCEPT;

    void DumpItems(const char* tag) const NN_NOEXCEPT;

private: // メンバ変数
    atk::detail::fnd::IAllocator* m_DataBufferAllocator;  //!< アイテムデータバッファのアロケータです。
    atk::detail::fnd::IAllocator* m_PlaybackBufferAllocator; //!< 再生バッファのアロケータです。
    PatriciaDictionaryData*         m_ItemDictionaryData;   //!< アイテム辞書データです。
    int                             m_MaxItemCount;         //!< アイテム数の最大値です。
    int                             m_ValidItemCount;       //!< 有効なアイテム数です。
    int                             m_MaxName;              //!< 名前の長さの最大値です。
    size_t                          m_MemoryUsage;          //!< メモリ使用量です。

    mutable Item::ReferenceList     m_Items;                //!< アイテムリストです。
    Item::ReferenceList             m_UnfreezedItems;       //!< 非フリーズアイテムリストです。
    Item::ReferenceList             m_UnusedItems;          //!< 未使用アイテムリストです。

    uint32_t                        m_BaseId;               //!< インデックス化のための基準アイテムIDです。
    Item**                          m_IdTable;              //!< アイテムIDテーブルです。
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
