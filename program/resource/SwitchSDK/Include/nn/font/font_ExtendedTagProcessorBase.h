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

#include <nn/font/font_TagProcessorBase.h>

namespace nn {
namespace font {

template <typename CharType>
class ExtendedTagProcessorBase : public TagProcessorBase<CharType>
{
    typedef TagProcessorBase<CharType> Base;
public:
    //! 実行時型情報です。
    NN_FONT_RUNTIME_TYPEINFO(ExtendedTagProcessorBase<CharType>::Base);

    virtual typename TagProcessorBase<CharType>::Operation Process(
        uint32_t code,
        PrintContext<CharType>* pContext) NN_OVERRIDE
    {
        return ProcessTag(code, pContext, NULL);
    }

    virtual typename TagProcessorBase<CharType>::Operation CalculateRect(
        nn::font::Rectangle* pRect,
        PrintContext<CharType>* pContext,
        uint32_t code) NN_OVERRIDE
    {
        return ProcessTag(code, pContext, pRect);
    }

    //! @brief      描画可能な文字の位置を返すコールバックです。
    //!
    //! @details
    //! このメソッドは、呼び出し元が描画可能な文字を得るために使用します。
    //! 与えられた position の位置の文字が描画可能かどうかを pIsPrintable に設定し、
    //! position の次の文字の位置を返します。
    //!
    //! @param[out] pIsPrintable position の位置の文字が描画可能かどうか。
    //! @param[in]  position    呼び出し元から与えられる位置。
    //!
    //! @return     position の次の文字の位置。
    //!
    virtual const CharType* AcquireNextPrintableChar(bool* pIsPrintable, const CharType* position) NN_OVERRIDE;

private:
    //---------------------------------------------------------------------------
    // 拡張タグ情報
    //---------------------------------------------------------------------------
    struct TagInfo
    {
        uint16_t kind; // SHIFT_IN=0x000e、SHIFT_OUT=0x000f
        uint16_t tagGroup;
        uint16_t tagId;
    };
    struct TagInfoShiftIn
    {
        TagInfo tagInfo;
        uint16_t paramSize;
    };
    struct TagInfoShiftOut
    {
        TagInfo tagInfo;
    };
    struct TagInfoRuby
    {
        TagInfoShiftIn tagInfoShiftIn;
        uint16_t kanjiSize;
        uint16_t rubySize;
        CharType rubyText[1]; // ルビのテキストが可変長で続く
    };

    static const uint16_t TagSystem = 0x0000;
    static const uint16_t ShiftIn = 0x000e;
    static const uint16_t ShiftOut = 0x000f;
    static const uint16_t TagIdRuby = 0x0000;

    typename TagProcessorBase<CharType>::Operation ProcessTag(
        uint32_t code,
        typename TagProcessorBase<CharType>::ContextType* pContext,
        nn::font::Rectangle* pRect);

    const CharType* AnalyzeTagHeader(
        const TagInfo** pTagInfo,
        const CharType* pText);

    typename TagProcessorBase<CharType>::Operation ProcessTagRuby(
        const TagInfo* pTagInfo,
        typename TagProcessorBase<CharType>::ContextType* pContext,
        nn::font::Rectangle* pRect, const CharType* pNextStr);
};

}   // namespace font
}   // namespace nn
