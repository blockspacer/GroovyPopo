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

#include <nn/atk/viewer/detail/res/atk_ResTypes.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

//! @internal
//! @brief Patricia Tree で実装された辞書の構造体です。
struct PatriciaDictionaryData
{
    uint32_t size; //!< データブロックのサイズ
    int32_t dataCount;

    //! @brief 辞書のノード情報です。
    struct Node
    {
        uint32_t refBit;
        uint16_t idxLeft;
        uint16_t idxRight;
        BinString ofsName;
        Offset ofsData;
    } node[1]; // 実際には (dataCount + 1) 個並んでいる。1個目はルートノードで空。
};

//! @internal
//! @brief Patricia Tree で実装された辞書です。
//!
//! 無効な辞書に対する名前引きは NULL を返しますが、
//! インデックス引きは未定義の動作となります。
//!
class PatriciaDictionary : private PatriciaDictionaryData
{
public:
    //! @internal
    using PatriciaDictionaryData::Node;

public:
    //! @brief 指定バッファ上に辞書データのインスタンスを生成します。
    //! @param buffer TBD
    //! @param length TBD
    //! @param maxNodeCount TBD
    //! @return TBD
    static PatriciaDictionaryData* CreateData(void* buffer, size_t length, int maxNodeCount) NN_NOEXCEPT;

    //! @brief 指定ノードをリセットし再利用可能にします。
    //! @param node TBD
    static void ResetNode(Node& node) NN_NOEXCEPT
    {
        node.refBit = ~0u;
        node.idxLeft = 0;
        node.idxRight = 0;
        node.ofsName.offset = 0;
        node.ofsData = 0;
    }

    //! @brief 指定ノード数を格納可能な辞書を生成するのに必要なサイズを取得します。
    //! @param nodes TBD
    //! @return TBD
    static size_t GetRequiredSize(int nodes) NN_NOEXCEPT
    {
        return sizeof(PatriciaDictionaryData) + sizeof(Node) * static_cast<size_t>(nodes);
    }

    //! @brief 辞書に含まれるデータ数を取得します。
    //! @return TBD
    int GetDataCount() const NN_NOEXCEPT
    {
        return this->dataCount;
    }

    //! @brief インデクスからデータのポインタを取得します。
    //! @param idx TBD
    //! @return TBD
    void* Get(int idx) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT(idx < this->dataCount);
        // 辞書引き関連についてはconst correctnessを維持しない。
        return const_cast<void*>(this->node[idx + 1].ofsData.to_ptr());
    }

    //! @brief 文字列で検索してポインタを取得します。
    //!
    //! 文字列の長さが分かっている場合は len を渡す方が高速です。
    //!
    //! @param str TBD
    //! @return TBD
    void* Find(const char* str) const NN_NOEXCEPT;

    //! @brief 文字列で検索してポインタを取得します。
    //! @param str TBD
    //! @param len TBD
    //! @return TBD
    void* Find(const char* str, size_t len) const NN_NOEXCEPT;

    //! @brief 文字列で検索してポインタを取得します。
    //! @param name TBD
    //! @return TBD
    void* Find(const ResName* name) const NN_NOEXCEPT;

    //! @brief 文字列で検索してインデクスを取得します。
    //!
    //! 文字列の長さが分かっている場合は len を渡す方が高速です。
    //!
    //! @param str TBD
    //! @return TBD
    int FindIndex(const char* str) const NN_NOEXCEPT;

    //! @brief 文字列で検索してインデクスを取得します。
    //! @param str TBD
    //! @param len TBD
    //! @return TBD
    int FindIndex(const char* str, size_t len) const NN_NOEXCEPT;

    //! @brief 文字列で検索してインデクスを取得します。
    //! @param name TBD
    //! @return TBD
    int FindIndex(const ResName* name) const NN_NOEXCEPT;

    //! @brief 文字列で検索してノードを取得します。
    //! @param str TBD
    //! @param len TBD
    //! @return TBD
    Node* FindNode(const char* str, size_t len) const NN_NOEXCEPT;

    //! @brief 文字列で検索してノードを取得します。
    //! @param name TBD
    //! @return TBD
    Node* FindNode(const ResName* name) const NN_NOEXCEPT;

    //! @brief インデクスから名前を取得します。
    //!
    //! インデクスは辞書が持つデータ数の範囲内とする必要があります。
    //!
    //! @param idx TBD
    //! @return TBD
    const ResName* GetResName(int idx) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT(idx < this->dataCount);
        return this->node[idx + 1].ofsName.GetResName();
    }

    //! @brief インデクスから名前を取得します。
    //!
    //! インデクスは辞書が持つデータ数の範囲内とする必要があります。
    //!
    //! @param idx TBD
    //! @return TBD
    const char* GetName(int idx) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT(idx < this->dataCount);
        return this->node[idx + 1].ofsName.to_ptr();
    }

    //! @brief 辞書の構築結果です。
    enum BuildResult
    {
        //! @brief 辞書の構築が成功したことを表します。
        BuildResult_Success = 0,

        //! @brief 辞書に名前が重複した要素が存在して構築できなかったことを表します。
        BuildResult_ErrorNameDuplication
    };

    //! @brief 辞書を再構築します。
    //! @return TBD
    BuildResult Build() NN_NOEXCEPT;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
