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

#include <nn/font/font_Font.h>

namespace nn {
namespace font {


//---------------------------------------------------------------------------
//! @brief        内部に2つのフォントを保持し、1つのフォントのように扱います。
//---------------------------------------------------------------------------
class PairFont
    : public Font
{
public:
    //! 実行時型情報です。
    NN_FONT_RUNTIME_TYPEINFO(Font);

    /* ------------------------------------------------------------------------
            関数
       ------------------------------------------------------------------------ */

    //! @name コンストラクタ / デストラクタ
    //@{

    //! @brief      コンストラクタです。
    //!
                                PairFont();

    //! @brief      コンストラクタです。
    //!
    //! @details
    //! 2つのフォントは文字コードが
    //! 同じでなければなりません。
    //!
    //! @param[in]  pFirst      フォントへのポインタ。
    //! @param[in]  pSecond     フォントへのポインタ。
    //!
                                PairFont(
                                    Font*   pFirst,
                                    Font*   pSecond);

    //! デストラクタです。
    virtual                     ~PairFont();

    //@}


    //! @name フォントの設定・取得
    //@{

    //! @brief      フォントを設定します。
    //!
    //! @details
    //! 2つのフォントは文字コードが
    //! 同じでなければなりません。
    //!
    //! @param[in]  pFirst      フォントへのポインタ。
    //! @param[in]  pSecond     フォントへのポインタ。
    //!
    void                        SetFont(Font* pFirst, Font* pSecond);

    //! @brief      1 つ目のフォントを取得します。
    //!
    //! @return
    //! フォントへのポインタを返します。
    //!
    Font*                       GetFontFirst() const
    { return m_pFirst; }

    //! @brief      2 つ目のフォントを取得します。
    //!
    //! @return
    //! フォントへのポインタを返します。
    //!
    Font*                       GetFontSecond() const
    { return m_pSecond; }

    //@}


    //! @name フォント情報の取得
    //@{

    virtual int                 GetWidth() const NN_OVERRIDE;

    virtual int                 GetHeight() const NN_OVERRIDE;

    virtual int                 GetAscent() const NN_OVERRIDE;

    virtual int                 GetDescent() const NN_OVERRIDE;

    virtual int                 GetMaxCharWidth() const NN_OVERRIDE;

    virtual Type                GetType() const NN_OVERRIDE;

    virtual TexFmt              GetTextureFormat() const NN_OVERRIDE;

    virtual int                 GetLineFeed() const NN_OVERRIDE;

    virtual const CharWidths    GetDefaultCharWidths() const NN_OVERRIDE;

    //@}


    //! @name フォント情報の設定
    //@{

    virtual void                SetDefaultCharWidths(const CharWidths& widths) NN_OVERRIDE;

    virtual bool                SetAlternateChar(uint32_t c) NN_OVERRIDE;

    virtual void                SetLineFeed(int linefeed) NN_OVERRIDE;

    //@}


    //! @name 文字情報の取得
    //@{

    virtual int                 GetCharWidth(
                                    uint32_t c                      // 幅を求める文字
                                ) const NN_OVERRIDE;

    virtual const CharWidths    GetCharWidths(
                                    uint32_t c                      // 幅を求める文字
                                ) const NN_OVERRIDE;

    virtual void                GetGlyph(
                                    Glyph*      glyph,              // グリフ情報を受け取るバッファ
                                    uint32_t    c                   // グリフ情報を取得する文字
                                ) const NN_OVERRIDE;
    virtual bool                HasGlyph(
                                    uint32_t    c                   // グリフ情報の有無を取得する文字
                                ) const NN_OVERRIDE;

    virtual int                 GetKerning(uint32_t c0, uint32_t c1) const NN_OVERRIDE;

    //@}


    //! @name 文字列エンコーディング
    //@{

    virtual CharacterCode       GetCharacterCode() const NN_OVERRIDE;

    //@}


    //! @name シート情報の取得
    //@{

    virtual int                 GetBaselinePos() const NN_OVERRIDE;

    virtual int                 GetCellHeight() const NN_OVERRIDE;

    virtual int                 GetCellWidth() const NN_OVERRIDE;

    //@}


    //! @name テクスチャ補間
    //@{

    virtual void            SetLinearFilterEnabled(
                                bool    atSmall,
                                bool    atLarge) NN_OVERRIDE;

    virtual bool            IsLinearFilterEnabledAtSmall() const NN_OVERRIDE;

    virtual bool            IsLinearFilterEnabledAtLarge() const NN_OVERRIDE;

    virtual uint32_t        GetTextureWrapFilterValue() const NN_OVERRIDE;

    //@}

    //! @name 表示設定
    //@{

    virtual bool            IsColorBlackWhiteInterpolationEnabled() const NN_OVERRIDE;

    virtual void            SetColorBlackWhiteInterpolationEnabled(bool flag) NN_OVERRIDE;

    //! @brief        特殊フチ形式が設定可能かを取得します。
    //!
    //! @return       特殊フチ形式が設定可能な場合は true を返します。
    //!
    virtual bool            IsBorderAvailable() const NN_OVERRIDE
    {
        // 両方で判断する
        return m_pFirst->IsBorderAvailable() && m_pSecond->IsBorderAvailable();
    }

    virtual bool            IsBorderEffectEnabled() const NN_OVERRIDE;

    //@}

private:
    /* ------------------------------------------------------------------------
            変数
       ------------------------------------------------------------------------ */
    // フォントの実体へのポインタ
    Font*                       m_pFirst;
    Font*                       m_pSecond;
    // 代替文字を1つ目のフォントから取得するかどうか
    bool                        m_AlternateWithFirst;
    NN_PADDING3;
};




}   // namespace font
}   // namespace nn
