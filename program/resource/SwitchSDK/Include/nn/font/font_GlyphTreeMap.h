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


#include <algorithm>
#include <nn/nn_Assert.h>

#include <nn/nn_Allocator.h>
#include <nn/util/util_BitPack.h>

#include <nn/util/util_IntrusiveList.h>
#include <nn/font/font_Types.h>
#include <nn/font/detail/font_FreeList.h>
#include <nn/font/font_Util.h>

namespace nn
{
namespace font
{

//---------------------------------------------------------------------------
//! @brief テクスチャキャッシュ内のグリフを表すクラスです。
//!
//! @details
//! ライブラリユーザが直接使用するクラスではないため、詳しいDoxyコメントは
//! 付けていません。
//---------------------------------------------------------------------------
class GlyphNode
{
friend class GlyphTreeMap;
public:
    GlyphNode(uint32_t code, uint16_t size, uint16_t face)
     : m_pLeftNode(NULL)
     , m_pRightNode(NULL)
     , m_Color(Red)
     , m_LineKind(0)
     , m_LineNo(0)
     , m_CachePosX(0)
     , m_CachePosY(0)
     , m_CacheWidth(0)
     , m_CacheHeight(0)
     , m_GlyphWidth(0)
     , m_GlyphHeight(0)
     , m_AdvanceX(0)
     , m_LeftOffset(0)
     , m_BaselineOffset(0)
    {
        m_Key.detail.code = code;
        m_Key.detail.fontSize = size;
        m_Key.detail.fontFace = face;

        m_Flag.Clear();
        m_LockGroup.Clear();
    }

    uint32_t GetCode() const { return m_Key.detail.code ;}
    uint16_t GetSize() const { return m_Key.detail.fontSize;}
    uint16_t GetFace() const { return m_Key.detail.fontFace;}

    void SetRequested(bool b) { m_Flag.SetMaskedBits(FlagMask_Requested, b ? FlagMask_Requested : 0); }
    bool IsRequested() const { return m_Flag.IsAnyBitOn(FlagMask_Requested); }

    void SetPlotting(bool b) { m_Flag.SetMaskedBits(FlagMask_Plotting, b ? FlagMask_Plotting : 0); }
    bool IsPlotting() const { return m_Flag.IsAnyBitOn(FlagMask_Plotting); }

    void SetUsed(bool b) { m_Flag.SetMaskedBits(FlagMask_Used, b ? FlagMask_Used : 0); }
    bool IsUsed() const { return m_Flag.IsAnyBitOn(FlagMask_Used); }

    void SetErased(bool b) { m_Flag.SetMaskedBits(FlagMask_Erased, b ? FlagMask_Erased : 0); }
    bool IsErased() const { return m_Flag.IsAnyBitOn(FlagMask_Erased); }

    void SetKeeped(bool b) { m_Flag.SetMaskedBits(FlagMask_Kept, b ? FlagMask_Kept : 0); }
    bool IsKeeped() const { return m_Flag.IsAnyBitOn(FlagMask_Kept); }

    void SetSystemReserved(bool b) { m_Flag.SetMaskedBits(FlagMask_SystemReserved, b ? FlagMask_SystemReserved : 0); }
    bool IsSystemReserved() const { return m_Flag.IsAnyBitOn(FlagMask_SystemReserved); }

    void SetNotInFont(bool b) { m_Flag.SetMaskedBits(FlagMask_NotInFont, b ? FlagMask_NotInFont : 0); }
    bool IsNotInFont() const { return m_Flag.IsAnyBitOn(FlagMask_NotInFont); }

    bool IsPlottingOrNotInFont() const { return m_Flag.IsAnyBitOn(FlagMask_Plotting | FlagMask_NotInFont); }
    bool IsRequestedOrKeeped() const { return m_Flag.IsAnyBitOn(FlagMask_Requested | FlagMask_Kept); }

    uint16_t GetCachePosX() const { return m_CachePosX; }
    uint16_t GetCachePosY() const { return m_CachePosY; }
    uint16_t GetCacheWidth() const { return m_CacheWidth; }
    uint16_t GetCacheHeight() const { return m_CacheHeight; }
    uint16_t GetGlyphWidth() const { return m_GlyphWidth; }
    uint16_t GetGlyphHeight() const { return m_GlyphHeight; }
    uint16_t GetAdvanceX() const { return m_AdvanceX; }
    int16_t GetLeftOffset() const { return m_LeftOffset; }
    int16_t GetBaselineOffset() const { return m_BaselineOffset; }

    void SetCachePosX(uint16_t posX) { m_CachePosX = posX; }
    void SetCachePosY(uint16_t posY) { m_CachePosY = posY; }
    void SetCacheWidth(uint16_t width) { m_CacheWidth = width; }
    void SetCacheHeight(uint16_t height) { m_CacheHeight = height; }
    void SetGlyphWidth(uint16_t width) { m_GlyphWidth = width; }
    void SetGlyphHeight(uint16_t height) { m_GlyphHeight = height; }
    void SetAdvanceX(uint16_t advance) { m_AdvanceX = advance; }
    void SetLeftOffset(int16_t offset) { m_LeftOffset = offset; }
    void SetBaselineOffset(int16_t offset) { m_BaselineOffset = offset; }

    nn::util::BitPack32& GetLockGroup() { return m_LockGroup; }
    const nn::util::BitPack32& GetLockGroup() const { return m_LockGroup; }

    uint8_t GetLineNo() const { return m_LineNo; }
    void SetLineNo(uint8_t no) { m_LineNo = no; }
    uint8_t GetLineKind() const { return m_LineKind; }
    void SetLineKind(uint8_t kind) { m_LineKind = kind; }

    bool IsErasable() const { return (m_Flag.storage == 0 && m_LockGroup.storage == 0); }
    bool IsUsable() const { return ( ! IsPlottingOrNotInFont() && (m_Flag.IsAnyBitOn(FlagMask_Requested | FlagMask_Used | FlagMask_Kept | FlagMask_SystemReserved) || m_LockGroup.storage != 0) ); }

    static uint8_t CalculateLineKind(uint16_t height);
    static uint32_t CalculateLineHeight(uint8_t kind);

    nn::util::IntrusiveListNode m_Link;
    nn::util::IntrusiveListNode m_LineLink; // TextureCache::m_LineInfos[]::list に追加するためのノード

private:

    enum FlagBit {
        FlagBit_Requested,
        FlagBit_Plotting,
        FlagBit_Used,
        FlagBit_Erased,
        FlagBit_Kept,
        FlagBit_SystemReserved,
        FlagBit_NotInFont
    };

    enum FlagMask {
        FlagMask_Requested = 1 << FlagBit_Requested,
        FlagMask_Plotting = 1 << FlagBit_Plotting,
        FlagMask_Used = 1 << FlagBit_Used,
        FlagMask_Erased = 1 << FlagBit_Erased,
        FlagMask_Kept = 1 << FlagBit_Kept,
        FlagMask_SystemReserved = 1 << FlagBit_SystemReserved,
        FlagMask_NotInFont = 1 << FlagBit_NotInFont
    };

    static const uint8_t Red = 1;
    static const uint8_t Black = 0;

    union KeyType
    {
        uint64_t raw;
        struct
        {
            uint32_t code;
            uint16_t fontFace;
            uint16_t fontSize;
        } detail;
    };

    NN_STATIC_ASSERT(sizeof(KeyType) == 8);

    //----------------------------------------------------------

    GlyphNode* m_pLeftNode;
    GlyphNode* m_pRightNode;
    KeyType m_Key;
    nn::util::BitPack8 m_Flag;
    uint8_t m_Color;
    uint8_t m_LineKind;
    uint8_t m_LineNo;
    nn::util::BitPack32 m_LockGroup;
    uint16_t m_CachePosX;
    uint16_t m_CachePosY;
    uint16_t m_CacheWidth;
    uint16_t m_CacheHeight;
    uint16_t m_GlyphWidth;
    uint16_t m_GlyphHeight;
    uint16_t m_AdvanceX;
    int16_t m_LeftOffset;
    int16_t m_BaselineOffset; // 基本となるベースラインからずれた場合その差分が入る。
};

typedef nn::util::IntrusiveList<GlyphNode, nn::util::IntrusiveListMemberNodeTraits<GlyphNode, &GlyphNode::m_Link> > GlyphList;
typedef nn::util::IntrusiveList<GlyphNode, nn::util::IntrusiveListMemberNodeTraits<GlyphNode, &GlyphNode::m_LineLink> > GlyphLineList;

//---------------------------------------------------------------------------
//! @brief テクスチャキャッシュ内のグリフを管理するためのTreeMapです。
//!
//! @details
//! 左傾赤黒木という平衡二分木のアルゴリズムを利用してます。
//!
//! ライブラリユーザが直接使用するクラスではないため、詳しいDoxyコメントは
//! 付けていません。
//---------------------------------------------------------------------------
class GlyphTreeMap
{
public:
    GlyphTreeMap();

    //! @brief DumpToFunction のコールバック関数です。
    //!
    //! @param[in] pNode ノードです。
    //! @param[in] pUserData ユーザデータです。
    //!
    typedef void(*DumpCallbackFunction)(const GlyphNode* pNode, void* pUserData);

    void Initialize(nn::AlignedAllocateFunctionWithUserData pAllocateFunction, void* pUserDataForAllocateFunction, uint32_t nodeCount);

    void Finalize(nn::FreeFunctionWithUserData pFreeFunction, void* pUserDataForFreeFunction);

    //! @brief 引数で指定した文字コードとサイズに等しいグリフを検索して返します。
    //!
    //! @param[in] code 検索するグリフの文字コードです。
    //! @param[in] size 検索するグリフのサイズです。
    //! @param[in] face 追加するグリフのフォント字形です。
    //! @return 見つかったグリフ。見つからなかった場合はNULL
    //!
    GlyphNode* Find(uint32_t code, uint16_t size, uint16_t face) const;

    //! @brief 引数で与えた文字コードとサイズのグリフをツリーマップに追加します。
    //!
    //! @param[in] code 追加するグリフの文字コードです。
    //! @param[in] size 追加するグリフのサイズです。
    //! @param[in] face 追加するグリフのフォント字形です。
    //! @return 追加したグリフ。グリフのノード数が最大値を超えている場合は内部でアサートします。製品版ではNULLが返されます。
    //!
    GlyphNode* Insert(uint32_t code, uint16_t size, uint16_t face);

    //! @brief 引数で指定した文字コードとサイズに等しいグリフを検索し、削除します。
    //!
    //! @details
    //! 等しい文字コードとサイズを持っているグリフが存在しない場合、実行時例外で
    //! 停止してしまいますので、
    //! 等しい文字コードとサイズを持っているグリフが含まれていることを確認した上でご使用ください。
    //!
    //! @param[in] code 削除するグリフの文字コードです。
    //! @param[in] size 削除するグリフのサイズです。
    //! @param[in] face 追加するグリフのフォント字形です。
    //!
    void Erase(uint32_t code, uint16_t size, uint16_t face);

    //! @brief ScalableFontのCompleteTextureCacheでグリフのフラグを更新する処理を行います。
    //!
    void UpdateFlagsForCompleteTextureCache()
    {
        if (m_pRootNode)
        {
            UpdateFlagsForCompleteTextureCacheRecursive(m_pRootNode);
        }
    }

    //! @brief ScalableFontのCompleteTextureCacheでグリフのフラグを更新する処理を行います。
    //!
    //! @param[in] groupMask    グループマスクです。
    //!
    void ClearLockGroup(uint32_t groupMask)
    {
        if (m_pRootNode)
        {
            ClearLockGroupRecursive(m_pRootNode, groupMask);
        }
    }

    //! @brief 内容をダンプします。
    //!
    void Dump() const;

    //! @brief 内容をダンプします。
    //!
    //! @param[in] callback コールバック関数です。
    //! @param[in] pUserData コールバック関数に渡されるユーザデータです。
    //!
    void DumpToFunction(DumpCallbackFunction callback, void* pUserData) const;

    //! @brief 内容を初期化し、Initialize直後の状態に戻します。
    //!
    void Reset();

private:
    GlyphNode* m_pRootNode;
    detail::FreeList m_FreeList;
    uint32_t m_NodeCountMax;

    GlyphNode* Find(GlyphNode* pNode, uint64_t keyRaw) const;
    GlyphNode* Insert(GlyphNode* pOrigin, GlyphNode* pNode);
    GlyphNode* Erase(GlyphNode* pNode, uint64_t key_raw);
    static inline GlyphNode* RotateLeft(GlyphNode* pNode);
    static inline GlyphNode* RotateRight(GlyphNode* pNode);
    static inline void FlipColors(GlyphNode* pNode);
    static inline bool IsRed(GlyphNode* pNode);
    static inline GlyphNode* FixUp(GlyphNode* pNode);
    static inline GlyphNode* MoveRedLeft(GlyphNode* pNode);
    static inline GlyphNode* MoveRedRight(GlyphNode* pNode);
    static inline GlyphNode* EraseMin(GlyphNode* pNode);
    static inline GlyphNode* GetMin(GlyphNode* pNode);

    static void UpdateFlagsForCompleteTextureCacheRecursive(GlyphNode* pNode);
    static void ClearLockGroupRecursive(GlyphNode* pNode, uint32_t groupMask);
    static uint32_t DumpRecursive(GlyphNode* pNode, uint32_t level);
    static void DumpToFunctionRecursive(GlyphNode* pNode, DumpCallbackFunction callback, void* pUserData);
};

inline GlyphNode* GlyphTreeMap::RotateLeft(GlyphNode* pNode)
{
    GlyphNode* pNewOrigin = pNode->m_pRightNode;
    pNode->m_pRightNode = pNewOrigin->m_pLeftNode;
    pNewOrigin->m_pLeftNode = pNode;
    pNewOrigin->m_Color = pNode->m_Color;
    pNode->m_Color = GlyphNode::Red;
    return pNewOrigin;
}

inline GlyphNode* GlyphTreeMap::RotateRight(GlyphNode* pNode)
{
    GlyphNode* pNewOrigin = pNode->m_pLeftNode;
    pNode->m_pLeftNode = pNewOrigin->m_pRightNode;
    pNewOrigin->m_pRightNode = pNode;
    pNewOrigin->m_Color = pNode->m_Color;
    pNode->m_Color = GlyphNode::Red;
    return pNewOrigin;
}

inline void GlyphTreeMap::FlipColors(GlyphNode* pNode)
{
    pNode->m_Color = ! pNode->m_Color;
    pNode->m_pLeftNode->m_Color = ! pNode->m_pLeftNode->m_Color;
    pNode->m_pRightNode->m_Color = ! pNode->m_pRightNode->m_Color;
}

inline bool GlyphTreeMap::IsRed(GlyphNode* pNode)
{
    if (pNode) {
        return pNode->m_Color == GlyphNode::Red;
    } else {
        return false;
    }
}

inline GlyphNode* GlyphTreeMap::FixUp(GlyphNode* pNode)
{
    if (IsRed(pNode->m_pRightNode)) {
        pNode = RotateLeft(pNode);
    }
    if(IsRed(pNode->m_pLeftNode) && IsRed(pNode->m_pLeftNode->m_pLeftNode)) {
        pNode = RotateRight(pNode);
    }
    if(IsRed(pNode->m_pLeftNode) && IsRed(pNode->m_pRightNode)) {
        FlipColors(pNode);
    }
    return pNode;
}

inline GlyphNode* GlyphTreeMap::MoveRedLeft(GlyphNode* pNode)
{
    FlipColors(pNode);
    if (IsRed(pNode->m_pRightNode->m_pLeftNode)) {
        pNode->m_pRightNode = RotateRight(pNode->m_pRightNode);
        pNode = RotateLeft(pNode);
        FlipColors(pNode);
    }
    return pNode;
}

inline GlyphNode* GlyphTreeMap::MoveRedRight(GlyphNode* pNode)
{
    FlipColors(pNode);
    if (IsRed(pNode->m_pLeftNode->m_pLeftNode)) {
        pNode = RotateRight(pNode);
        FlipColors(pNode);
    }
    return pNode;
}

inline GlyphNode* GlyphTreeMap::EraseMin(GlyphNode* pNode)
{
    if (pNode->m_pLeftNode == NULL) {
        return NULL;
    }
    if ( ! IsRed(pNode->m_pLeftNode) && ! IsRed(pNode->m_pLeftNode->m_pLeftNode)) {
        pNode = MoveRedLeft(pNode);
    }
    pNode->m_pLeftNode = EraseMin(pNode->m_pLeftNode);

    return FixUp(pNode);
}

inline GlyphNode* GlyphTreeMap::GetMin(GlyphNode* pNode)
{
    while (pNode->m_pLeftNode) {
        pNode = pNode->m_pLeftNode;
    }
    return pNode;
}

} // namespace nn::font
} // namespace nn
