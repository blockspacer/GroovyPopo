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

#include <nn/gfx/gfx_Device.h>
#include <nn/font/font_TextureCache.h>

namespace nn
{

namespace ui2d
{

class Pane;
class TextBox;

} // namespace nn::ui2d
} // namespace nn

namespace nn
{
namespace font
{

struct ResComplexFontDescription;
class ScalableFont;

//---------------------------------------------------------------------------
//! @brief fcpx のルートからスケーラブルフォントを検索します。
//!
//! @param[in] pFont        fcpx のルートです。
//! @param[in] pCallback    スケーラブルフォントが見つかるたびに呼び出されるコールバック関数です。
//! @param[in] pParam       コールバック関数に渡す任意のデータです。
//!
//---------------------------------------------------------------------------
void SearchScalableFont(const Font* pFont, void(*pCallback)(const ScalableFont*, void*), void* pParam);

//---------------------------------------------------------------------------
//! @brief 文字コードを指定して、そのグリフを含むフォントを fcpx のルートから検索します。
//!
//! @details
//! 指定した文字コードを含むフォントが見つからなかった場合は NULL が返ります。
//!
//! @param[in] pFont        fcpx のルートです。
//! @param[in] code         文字コードです。
//!
//! @return 見つかったフォントです。
//!
//---------------------------------------------------------------------------
const Font* SearchFontWithCharacter(const Font* pFont, uint32_t code);

//---------------------------------------------------------------------------
//! @brief 指定されたテキストボックスペインに対し文字を登録します。
//!
//! @details
//! テキストボックスが複合フォントを使用している場合、テキストの各文字に対して描画に使われるフォントを求め、
//! それがスケーラブルフォントの場合に RegisterGlyph を行います。
//!
//! @param[in] pTextBox     テキストボックスペインです。
//! @param[in] lockGroup    文字をロックする際のグループ。0から31までの数値で指定してください。-1を指定した場合はロックしません。
//!
//! @return プロットが必要な文字が何文字あったかを返します。
//!
//---------------------------------------------------------------------------
uint32_t RegisterGlyphFromTextBox(const nn::ui2d::TextBox* pTextBox, int lockGroup);

//---------------------------------------------------------------------------
//! @brief @ref RegisterGlyphFromTextBox のロック指定省略版です。
//!
//! @details
//! テキストボックスが複合フォントを使用している場合、テキストの各文字に対して描画に使われるフォントを求め、
//! それがスケーラブルフォントの場合に RegisterGlyph を行います。
//!
//! @param[in] pTextBox     テキストボックスペインです。
//!
//! @return プロットが必要な文字が何文字あったかを返します。
//!
//---------------------------------------------------------------------------
uint32_t RegisterGlyphFromTextBox(const nn::ui2d::TextBox* pTextBox);

//---------------------------------------------------------------------------
//! @brief 指定されたペイン以下を再帰的に検索し、スケーラブルフォントが設定されているテキストボックスの文字を登録します。
//!
//! @details
//! テキストボックスが複合フォントを使用している場合、テキストの各文字に対して描画に使われるフォントを求め、
//! それがスケーラブルフォントの場合に RegisterGlyph を行います。
//!
//! @param[in] pPane         再帰的に検索するルートのペインです。
//! @param[in] lockGroup    文字をロックする際のグループ。0から31までの数値で指定してください。-1を指定した場合はロックしません。
//! @param[in] isSkippingInvisiblePane true にすると、非表示のペインの登録処理を飛ばします。
//!
//! @return プロットが必要な文字が何文字あったかを返します。
//!
//---------------------------------------------------------------------------
uint32_t RegisterGlyphFromTextBoxRecursive(const nn::ui2d::Pane* pPane, int lockGroup, bool isSkippingInvisiblePane);

//---------------------------------------------------------------------------
//! @brief 指定されたペイン以下を再帰的に検索し、スケーラブルフォントが設定されているテキストボックスの文字を登録します。
//!
//! @details
//! テキストボックスが複合フォントを使用している場合、テキストの各文字に対して描画に使われるフォントを求め、
//! それがスケーラブルフォントの場合に RegisterGlyph を行います。
//!
//! @param[in] pPane         再帰的に検索するルートのペインです。
//! @param[in] lockGroup    文字をロックする際のグループ。0から31までの数値で指定してください。-1を指定した場合はロックしません。
//!
//! @return プロットが必要な文字が何文字あったかを返します。
//!
//---------------------------------------------------------------------------
uint32_t RegisterGlyphFromTextBoxRecursive(const nn::ui2d::Pane* pPane, int lockGroup);

//---------------------------------------------------------------------------
//! @brief @ref RegisterGlyphFromTextBoxRecursive のロック指定省略版です。
//!
//! @details
//! テキストボックスが複合フォントを使用している場合、テキストの各文字に対して描画に使われるフォントを求め、
//! それがスケーラブルフォントの場合に RegisterGlyph を行います。
//!
//! @param[in] pPane         再帰的に検索するルートのペインです。
//!
//! @return プロットが必要な文字が何文字あったかを返します。
//!
//---------------------------------------------------------------------------
uint32_t RegisterGlyphFromTextBoxRecursive(const nn::ui2d::Pane* pPane);

} // namespace nn::font
} // namespace nn
