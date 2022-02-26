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

#include <nn/atk/fnd/binary/atkfnd_PrimitiveTypes.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

//-----------------------------------------------------------------------
/* シーケンス変数の種類 */
enum ToolSoundSequenceVariableType
{
    /* グローバル変数 */
    ToolSoundSequenceVariableType_Global  =   1,
    /* ローカル変数 */
    ToolSoundSequenceVariableType_Local   =   2,
    /* トラック変数 */
    ToolSoundSequenceVariableType_Track   =   3,
    /* enumを4Byte型にするための定義 */
    ToolSoundSequenceVariableType_4ByteEnum =   0xFFFFFFFF
};

//-----------------------------------------------------------------------
/* シーケンス変数コンテナヘッダ */
typedef struct {
    /*  4B: シーケンス変数の種類 */
    nn::atk::detail::fnd::BinU32                  variableType; // ToolSoundSequenceVariableType
    /*  4B: プレビューサウンドのインデックス */
    nn::atk::detail::fnd::BinU32                  previewSoundIndex;
    /*  4B: トラック番号 */
    nn::atk::detail::fnd::BinU32                  trackNo;
} ToolSoundSequenceVariableContainerHeader;

//-----------------------------------------------------------------------
/* シーケンス変数コンテナ */
typedef struct {
    /* 12B: コンテナヘッダ */
    ToolSoundSequenceVariableContainerHeader header;
    /* 64B: 変数の値（ShortMax+1 の場合、無効値） */
    nn::atk::detail::fnd::BinS32                          variables[16];
} ToolSoundSequenceVariableContainer;

class SequenceVariableTraits
{
public:
    // 定数
    static const uint32_t TrackCount    = 16;            // トラック数
    static const uint32_t VariableCount = 16;            // シーケンス変数の数
    static const int32_t InvalidValue  = SHRT_MAX + 1;   // シーケンス変数の無効値
    static const int32_t MaxValue      = SHRT_MAX;       // 変数値の最大値
    static const int32_t MinValue      = SHRT_MIN;       // 変数値の最小値

public:
    // 値の評価
    static bool ValidateTrackNo(uint32_t nTrackNo) NN_NOEXCEPT;
    static bool ValidateValue(int32_t lValue) NN_NOEXCEPT;
};


class SequenceVariable
{
public:
    SequenceVariable(void) NN_NOEXCEPT;
    ~SequenceVariable(void) NN_NOEXCEPT;

    // 値の操作
    int32_t GetValue() NN_NOEXCEPT;
    void SetValue(int32_t value) NN_NOEXCEPT;

private:
    // パラメータ
    int32_t m_lValue;        // 変数の値
};


class SequenceVariableContainer
{
public:
    SequenceVariableContainer() NN_NOEXCEPT;

    // シーケンス変数の操作
    bool   Parse(const ToolSoundSequenceVariableContainer* pContainer) NN_NOEXCEPT;
    void   InvalidateAll() NN_NOEXCEPT;
    uint32_t    GetCount() NN_NOEXCEPT;

    // パラメータの操作
    ToolSoundSequenceVariableType   GetType() NN_NOEXCEPT;
    uint32_t                             GetPreviewPlayerIndex() NN_NOEXCEPT;
    uint32_t                             GetTrackNo() NN_NOEXCEPT;

    // オペレータ
    SequenceVariable operator [](int32_t index) NN_NOEXCEPT;

private:
    // パラメータ
    ToolSoundSequenceVariableType       m_nType;
    nn::atk::detail::fnd::BinU32      m_nPreviewSoundIndex;
    nn::atk::detail::fnd::BinU32      m_nTrackNo;

    // パラメータ / シーケンス変数
    SequenceVariable m_variables[ SequenceVariableTraits::VariableCount ];    // コンテナ
};


} // namespace detail
} // namespace viewer
} // namespace atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
