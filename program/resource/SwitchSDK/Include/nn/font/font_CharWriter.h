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

#include <nn/font/font_Types.h>
#include <nn/font/font_Font.h>
#include <nn/util/util_MathTypes.h>

namespace nn {
namespace font {

class DispStringBuffer;

//---------------------------------------------------------------------------
//! @brief        Fontクラスを用いて文字の描画を行うためのクラスです。
//---------------------------------------------------------------------------
class CharWriter
{
public:
    /* ------------------------------------------------------------------------
            型
       ------------------------------------------------------------------------ */

    //! グラデーションの種類を表す enum です。
    enum GradationMode
    {
        GradationMode_None,                          //!< グラデーションなし
        GradationMode_Horizontal,                    //!< 非サポートです。
        GradationMode_Vertical,                      //!< 垂直方向グラデーション

        GradationMode_MaxGradationMode
    };



    /* ------------------------------------------------------------------------
            定数
       ------------------------------------------------------------------------ */
    static const uint32_t  DefaultColorMappingMin = 0x00000000UL;
    static const uint32_t  DefaultColorMappingMax = 0xFFFFFFFFUL;

    /* ------------------------------------------------------------------------
            関数
       ------------------------------------------------------------------------ */

    //! @name コンストラクタ／デストラクタ
    //@{

    //! コンストラクタです。
                        CharWriter();

    //! デストラクタです。
                        ~CharWriter();

    //@}

    //! @name フォント
    //@{

    //! @brief      フォントを設定します。
    //!
    //! @param[in]  pFont  新しく設定するフォントへのポインタです。
    //!
    void                SetFont(const Font* pFont)  { m_pFont = pFont; }

    //! @brief      フォントを取得します。
    //!
    //! @return     フォントが設定されている場合にはフォントへのポインタを返します。
    //!             設定されていない場合には NULL を返します。
    //!
    const Font*         GetFont() const             { return m_pFont; }

    //@}

    //! @name 文字色／グラデーション
    //@{

    //! @brief      文字色またはグラデーション色を設定します。
    //!
    //! @param[in]  color  文字色。
    //!
    void                SetTextColor(const nn::util::Unorm8x4& color)
    {
        m_TextColors[detail::TextColor_Start] = color;
        m_TextColors[detail::TextColor_End] = color;
    }

    //! @brief      文字色またはグラデーション色を設定します。
    //!
    //! @param[in]  start  グラデーション開始色です。
    //! @param[in]  end    グラデーション終了色です。
    //!
    void                SetTextColor(
                            const nn::util::Unorm8x4&  start,
                            const nn::util::Unorm8x4&  end
    )
    {
        m_TextColors[detail::TextColor_Start] = start;
        m_TextColors[detail::TextColor_End] = end;
    }

    //! @brief      文字色を取得します。
    //!
    //! @return     文字色を返します。
    //!
    const nn::util::Unorm8x4&     GetTextColor() const           { return m_TextColors[detail::TextColor_Start]; }

    //! @brief      グラデーション開始色を取得します。
    //!
    //! @return     グラデーション開始色を返します。
    //!
    const nn::util::Unorm8x4&     GetGradationStartColor() const { return m_TextColors[detail::TextColor_Start]; }

    //! @brief      グラデーション終了色を取得します。
    //!
    //! @return     グラデーション終了色を返します。
    //!
    const nn::util::Unorm8x4&     GetGradationEndColor() const   { return m_TextColors[detail::TextColor_End]; }

    //! @brief      影のアルファ値を設定します。
    //!
    //! @param[in]  alpha   アルファ値です。
    //!
    void                   SetShadowAlpha(uint8_t  alpha)      { m_ShadowAlpha = alpha; }

    //! @brief      影のアルファ値を取得します。
    //!
    //! @return     アルファ値を返します。
    //!
    uint8_t                      GetShadowAlpha() const        { return m_ShadowAlpha; }

    //@}

    //! @name 文字の拡大縮小
    //@{

    //! @brief      文字の拡大率を指定します。
    //!
    //! @param[in]  hScale  横方向拡大率。
    //! @param[in]  vScale  縦方向拡大率。
    //!
    void                SetScale(
                            float hScale,
                            float vScale
    )
    {
        m_Scale.v[0] = hScale;
        m_Scale.v[1] = vScale;
    }

    //! @brief      文字の拡大率を指定します。
    //!
    //! @param[in]  hvScale 拡大率。
    //!
    void                SetScale(float hvScale)
    {
        m_Scale.v[0] = hvScale;
        m_Scale.v[1] = hvScale;
    }

    //! @brief  文字の横方向の拡大率を取得します。
    //!
    //! @return 文字の横方向の拡大率を返します。
    //!
    float                 GetScaleX() const   { return m_Scale.v[0]; }

    //! @brief  文字の縦方向の拡大率を取得します。
    //!
    //! @return 文字の縦方向の拡大率を返します。
    //!
    float                 GetScaleY() const   { return m_Scale.v[1]; }

    //! @brief      文字のサイズを指定します。
    //!
    //! @param[in]  width   拡大後のセルの幅。
    //! @param[in]  height  拡大後のセルの高さ。
    //!
    void                SetFontSize(
                            float width,
                            float height);

    //! @brief      文字のサイズを指定します。
    //!
    //! @param[in]  height  拡大後のセルの高さ。
    //!
    void                SetFontSize(float height);

    //! @brief      拡大適用後のセル幅を取得します。
    //!
    //! @return     フォントが設定されている場合は拡大適用後のセル幅をピクセル単位で取得します。
    //!             フォントが設定されていない場合の返り値は未定義です。
    //!
    float                 GetFontWidth() const;

    //! @brief      拡大適用後のセル高さを取得します。
    //!
    //! @return     フォントが設定されている場合は拡大適用後のセル高さをピクセル単位で取得します。
    //!             フォントが設定されていない場合の返り値は未定義です。
    //!
    float                 GetFontHeight() const;

    //! @brief      拡大適用後のアセントを取得します。
    //!
    //! @return     拡大適用後のアセントを返します。
    //!
    float                 GetFontAscent() const;

    //! @brief      拡大適用後のディセントを取得します。
    //!
    //! @return     拡大適用後のディセントを返します。
    //!
    float                 GetFontDescent() const;

    //@}

    //! @name 文字装飾
    //@{

    //! @brief        イタリック体の傾き度合を比率で設定します。
    //!
    //! @details
    //! フォントの幅に対する斜体の比率です。
    //! 例えば 1.0f の場合、フォントの幅の分だけ右に傾きます。
    //!
    //! @param[in]  ratio   傾き度合の比率です。
    //!
    void                SetItalicRatio(float ratio)   { m_ItalicRatio = ratio; }

    //! @brief        イタリック体の傾き度合を比率で取得します。
    //!
    //! @details
    //! フォントの幅に対する斜体の比率です。
    //! 例えば 1.0f の場合、フォントの幅の分だけ右に傾きます。
    //!
    //! @return       傾き度合の比率です。
    //!
    float                 GetItalicRatio() const { return m_ItalicRatio; }

    //@}

    //! @name 等幅描画
    //@{

    //! @brief      強制等幅描画を行うように設定します。
    //!
    //! @param[in]  isFixed 強制等幅描画を行うのであれば true を指定します。
    //!
    void                EnableFixedWidth(bool isFixed)  { m_IsWidthFixed = isFixed; }

    //! @brief  強制等幅描画を行うかどうかを取得します。
    //!
    //! @return 強制等幅描画を行う場合は true を、行わない場合は false を返します。
    //!
    bool                IsWidthFixed() const            { return m_IsWidthFixed; }

    //! @brief      強制等幅描画の文字幅を指定します。
    //!
    //! @param[in]  width   新しい等幅描画幅。
    //!
    void                SetFixedWidth(float width)        { m_FixedWidth = width; }

    //! @brief      強制等幅描画の文字幅を取得します。
    //!
    //! @return     強制等幅描画の文字幅を返します。
    //!
    float                 GetFixedWidth() const           { return m_FixedWidth; }

    //@}

    //! @name 文字描画
    //@{

    //! @brief      グリフを描画します。
    //!
    //! @param[in]  glyph   描画するグリフ。
    //!
    //! @return     描画したグリフの文字幅を返します。
    //!
    float                 PrintGlyph(const Glyph& glyph);

    //! @brief      文字を描画します。
    //!
    //! @param[in]  code    描画する文字の文字コード。
    //!
    //! @return     描画した文字の文字幅を返します。
    //!
    float                 Print(uint32_t code);

    //! @brief      グリフを描画します。
    //!
    //! @param[in]  glyph   描画する文字のグリフデータ。
    //!
    void                DrawGlyph(const Glyph& glyph);

    //@}

    //! @name カーソル
    //@{

    //! @brief      カーソル位置を指定した新しい座標に設定します。
    //!
    //! @param[in]  x   カーソルの新しい X 座標。
    //! @param[in]  y   カーソルの新しい Y 座標。
    //!
    void                SetCursor(
                            float x,
                            float y
    )
    {
        m_CursorPos.v[0] = x;
        m_CursorPos.v[1] = y;
    }

    //! @brief      カーソル位置を指定した新しい座標に設定します。
    //!
    //! @param[in]  x   カーソルの新しい X 座標。
    //! @param[in]  y   カーソルの新しい Y 座標。
    //! @param[in]  z   カーソルの新しい Z 座標。
    //!
    void                SetCursor(
                            float x,
                            float y,
                            float z
    )
    {
        m_CursorPos.v[0] = x;
        m_CursorPos.v[1] = y;
        m_CursorPos.v[2] = z;
    }

    //! @brief      カーソルを現在位置から指定された差分だけ移動します。
    //!
    //! @param[in]  dx  X 軸方向のカーソル移動量。
    //! @param[in]  dy  Y 軸方向のカーソル移動量。
    //!
    void                MoveCursor(
                            float dx,
                            float dy
    )
    {
        m_CursorPos.v[0] += dx;
        m_CursorPos.v[1] += dy;
    }

    //! @brief      カーソルを現在位置から指定された差分だけ移動します。
    //!
    //! @param[in]  dx  X 軸方向のカーソル移動量。
    //! @param[in]  dy  Y 軸方向のカーソル移動量。
    //! @param[in]  dz  Z 軸方向のカーソル移動量。
    //!
    void                MoveCursor(
                            float dx,
                            float dy,
                            float dz
    )
    {
        m_CursorPos.v[0] += dx;
        m_CursorPos.v[1] += dy;
        m_CursorPos.v[2] += dz;
    }

    //! @brief      カーソルの X 座標を設定します。
    //!
    //! @param[in]  x   カーソルの新しい X 座標。
    //!
    void                SetCursorX(float x)           { m_CursorPos.v[0] = x; }

    //! @brief      カーソルの Y 座標を設定します。
    //!
    //! @param[in]  y   カーソルの新しい Y 座標。
    //!
    void                SetCursorY(float y)           { m_CursorPos.v[1] = y; }

    //! @brief      カーソルの Z 座標を設定します。
    //!
    //! @param[in]  z   カーソルの新しい Z 座標。
    //!
    void                SetCursorZ(float z)           { m_CursorPos.v[2] = z; }

    //! @brief      カーソルを現在位置から指定された差分だけ移動します。
    //!
    //! @param[in]  dx  X 軸方向のカーソル移動量。
    //!
    void                MoveCursorX(float dx)         { m_CursorPos.v[0] += dx; }

    //! @brief      カーソルを現在位置から指定された差分だけ移動します。
    //!
    //! @param[in]  dy  Y 軸方向のカーソル移動量。
    //!
    void                MoveCursorY(float dy)         { m_CursorPos.v[1] += dy; }

    //! @brief      カーソルを現在位置から指定された差分だけ移動します。
    //!
    //! @param[in]  dz  Z 軸方向のカーソル移動量。
    //!
    void                MoveCursorZ(float dz)         { m_CursorPos.v[2] += dz; }

    //! @brief      カーソルのX座標を取得します。
    //!
    //! @return     カーソルのX座標を返します。
    //!
    float                 GetCursorX() const          { return m_CursorPos.v[0]; }

    //! @brief      カーソルのY座標を取得します。
    //!
    //! @return     カーソルのY座標を返します。
    //!
    float                 GetCursorY() const          { return m_CursorPos.v[1]; }

    //! @brief      カーソルのZ座標を取得します。
    //!
    //! @return     カーソルのZ座標を返します。
    //!
    float                 GetCursorZ() const          { return m_CursorPos.v[2]; }

    //@}

    //! @name 文字列表示用バッファ
    //@{

    //! @brief      グラフィックス用バッファのアライメントサイズを取得します。
    //!
    //! @return     グラフィックス用バッファのアライメントサイズを返します。
    //!
    static size_t              GetGraphicsBufferAlignment(nn::gfx::Device* pDevice) { return detail::GetGraphicsBufferAlignment(pDevice); }

    //! @brief      文字列表示用バッファのサイズを計算します。
    //!
    //! @param[in]  charCount         最大描画文字数です。
    //!
    //! @return     文字列表示用バッファのサイズを返します。
    //!
    static size_t           GetDispStringBufferSize(uint32_t  charCount);

    //! @brief      コンスタントバッファのサイズを計算します。
    //!
    //! @param[in]  pDevice         デバイス
    //! @param[in]  charCount       最大描画文字数です。
    //! @param[in]  shadowEnabled   影が有効か否かです。
    //!
    //! @return     テキストの描画に必要なコンスタントバッファのサイズを返します。
    //!
    static size_t           GetConstantBufferSize(nn::gfx::Device* pDevice, int charCount, bool shadowEnabled);

    //! @brief      文字列表示用バッファを初期化します。
    //!
    //! @param[in]  pDevice         デバイス
    //! @param[in]  pDrawBuffer      描画用バッファです。
    //! @param[in]  charCount         最大描画文字数です。
    //!
    //! @return     初期化した文字列表示用バッファを返します。
    //!
    static DispStringBuffer*
        InitializeDispStringBuffer(
            nn::gfx::Device* pDevice,
            void* pDrawBuffer,
            uint32_t  charCount)
    {
        return InitializeDispStringBuffer(pDevice, pDrawBuffer, charCount, false);
    }


    //! @brief      文字列表示用バッファを初期化します。
    //!
    //! @param[in]  pDevice         デバイス
    //! @param[in]  pDrawBuffer      描画用バッファです。
    //! @param[in]  charCount         最大描画文字数です。
    //! @param[in]  shadowEnabled   影が有効か否かです。省略した場合は無効となります。
    //!
    //! @return     初期化した文字列表示用バッファを返します。
    //!
    static DispStringBuffer*
                        InitializeDispStringBuffer(
                            nn::gfx::Device* pDevice,
                            void* pDrawBuffer,
                            uint32_t  charCount,
                            bool shadowEnabled);

    //! @brief      文字列表示用バッファを取得します。
    //!
    //! @return     文字列表示用バッファを返します。
    //!
    DispStringBuffer*   GetDispStringBuffer() const
    {
        return m_pDispStringBuffer;
    }

    //! @brief      文字列表示用バッファをセットします。
    //!             NULLを設定した場合は文字列表示用バッファを使用しなくなります。
    //!
    //! @param[in]  pDispStringBuffer 文字列表示用バッファです。
    //!
    void                SetDispStringBuffer(DispStringBuffer* pDispStringBuffer)
    {
        m_pDispStringBuffer = pDispStringBuffer;
    }

    //! @brief      文字列の描画を開始します。
    //!             文字列表示用バッファが設定されている場合に使用します。
    //!
    void                StartPrint();

    //! @brief      文字列の描画を終了します。
    //!             文字列表示用バッファが設定されている場合に使用します。
    //!
    void                EndPrint() const;

    //! @brief      文字列表示用のコマンドバッファをカレントのコマンドリストに追加します。
    //!
    void                UseCommandBuffer();

    //@}

private:

    /* ------------------------------------------------------------------------
            型
       ------------------------------------------------------------------------ */
    typedef nn::util::Float2      CharScale;
    typedef nn::util::Float3      CursorPos;

    struct ColorMapping
    {
        nn::util::Unorm8x4          min;
        nn::util::Unorm8x4          max;
    };

    /* ------------------------------------------------------------------------
            関数
       ------------------------------------------------------------------------ */
    //! @brief      グリフを指定位置に描画します。
    //!             カーソル移動は行いません。
    //!
    //! @param[in]  x      描画位置のX座標。
    //! @param[in]  glyph  描画するグリフデータ。
    //!
    void                PrintGlyph(
                            float             x,
                            const Glyph&    glyph);

    /* ------------------------------------------------------------------------
            変数
       ------------------------------------------------------------------------ */
    nn::util::Unorm8x4          m_TextColors[detail::TextColor_MaxTextColor];  //!< 文字色
    GradationMode               m_GradationMode;
    CharScale                   m_Scale;                //!< 文字拡大率
    CursorPos                   m_CursorPos;            //!< カーソル位置
    float                       m_FixedWidth;           //!< 等幅時の幅
    float                       m_ItalicRatio;          //!< イタリック体傾き度合
    const Font*                 m_pFont;                //!< フォント

    //! 頂点バッファオブジェクト用構造体へのポインタ
    DispStringBuffer*           m_pDispStringBuffer;

    bool                        m_IsWidthFixed;         //!< 等幅描画するかどうか
    uint8_t                     m_ShadowAlpha;          //!< 影のアルファ
};

}   // namespace font
}   // namespace nn
