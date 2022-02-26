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
#include <nn/font/font_ResourceFormat.h>

namespace nn {
namespace font {

//---------------------------------------------------------------------------
//! @brief        リソースを扱うフォントクラスの基底クラスです。
//---------------------------------------------------------------------------
class ResFontBase : public Font
{
public:
    //! 実行時型情報です。
    NN_FONT_RUNTIME_TYPEINFO(Font);

    //! @brief 1 つのビットマップフォントに設定できる使用文字コード範囲のセットの最大個数です。
    static const int CharCodeRangeCountMax = 16;

    /* ------------------------------------------------------------------------
            関数
       ------------------------------------------------------------------------ */

    //! @name コンストラクタ/デストラクタ/初期化/終了処理
    //@{

    //! コンストラクタです。
                            ResFontBase();

    //! デストラクタです。
    virtual                 ~ResFontBase();

    //! @brief テクスチャービューをディスクリプタプールに登録します。
    //!
    //! @param[in] pRegisterTextureViewSlot     ディスクリプタスロット確保関数です。
    //! @param[in] pUserData     関数に渡されるユーザーデーターです。
    //!
    void RegisterTextureViewToDescriptorPool(RegisterTextureViewSlot pRegisterTextureViewSlot, void* pUserData);

    //! @brief テクスチャービューをディスクリプタプールから解放します。
    //!
    //! @param[in] pUnregisterTextureViewSlot     ディスクリプタスロット解放関数です。
    //! @param[in] pUserData     関数に渡されるユーザーデーターです。
    //!
    void UnregisterTextureViewFromDescriptorPool(UnregisterTextureViewSlot pUnregisterTextureViewSlot, void* pUserData);

    //@}

    //! @name フォント情報の取得
    //@{

    virtual int             GetWidth() const NN_OVERRIDE;

    virtual int             GetHeight() const NN_OVERRIDE;

    virtual int             GetAscent() const NN_OVERRIDE;

    virtual int             GetDescent() const NN_OVERRIDE;

    virtual int             GetMaxCharWidth() const NN_OVERRIDE;

    virtual Type            GetType() const NN_OVERRIDE;

    virtual TexFmt          GetTextureFormat() const NN_OVERRIDE;

    virtual int             GetLineFeed() const NN_OVERRIDE;

    virtual const CharWidths
                            GetDefaultCharWidths() const NN_OVERRIDE;

    //! @name フォント情報の設定
    //@{

    virtual void            SetLineFeed(int linefeed) NN_OVERRIDE;

    virtual void            SetDefaultCharWidths(const CharWidths& widths) NN_OVERRIDE;

    virtual bool            SetAlternateChar(uint32_t c) NN_OVERRIDE;

    //@}

    //! @name 文字情報の取得
    //@{

    virtual int             GetCharWidth(uint32_t c) const NN_OVERRIDE;

    virtual const CharWidths
                            GetCharWidths(uint32_t c) const NN_OVERRIDE;

    virtual void            GetGlyph(
                                Glyph*      pGlyph,
                                uint32_t    c
                            ) const NN_OVERRIDE;

    virtual bool            HasGlyph(uint32_t c) const NN_OVERRIDE;

    virtual int            GetKerning(uint32_t c0, uint32_t c1) const NN_OVERRIDE;

    //@}

    //! @name 文字列エンコーディング
    //@{

    virtual CharacterCode   GetCharacterCode() const NN_OVERRIDE;

    //@}

    //! @name シート情報の取得
    //@{

    virtual int             GetBaselinePos() const NN_OVERRIDE;
    virtual int             GetCellHeight() const NN_OVERRIDE;
    virtual int             GetCellWidth() const NN_OVERRIDE;

    //@}


    //! @name テクスチャ補間
    //@{

    virtual void            SetLinearFilterEnabled(
                                bool    atSmall,
                                bool    atLarge) NN_OVERRIDE;

    virtual bool            IsLinearFilterEnabledAtSmall() const NN_OVERRIDE;

    virtual bool            IsLinearFilterEnabledAtLarge() const NN_OVERRIDE;

    virtual uint32_t              GetTextureWrapFilterValue() const NN_OVERRIDE;

    //@}

    //! @name 表示設定
    //@{

    //! @brief        カラーの白黒補間の影響の有無を取得します。
    //!
    //! @return       カラーの白黒補間の影響を受ける場合は、true を返します。
    //!
    virtual bool            IsColorBlackWhiteInterpolationEnabled() const NN_OVERRIDE;

    //! @brief        カラーの白黒補間の影響の有無を設定します。
    //!
    //! @details
    //! この設定でfalseを設定しても、アルファについては影響を受けます。
    //!
    //! @param[in]  flag    true を設定すると、カラーの白黒補間の影響を受けます。
    //!
    virtual void            SetColorBlackWhiteInterpolationEnabled(bool flag) NN_OVERRIDE;

    //! @brief        特殊フチ形式が設定可能かを取得します。
    //!
    //! @return       特殊フチ形式が設定可能な場合は true を返します。
    //!
    virtual bool            IsBorderAvailable() const NN_OVERRIDE
    {
        return m_pFontInfo->fontType == FontType_FontTypePackedTexture;
    }

    //! @brief        縁取り効果の有無を取得します。
    //!
    //! @return       縁取り効果が有効な場合は、true を返します。
    //!
    virtual bool            IsBorderEffectEnabled() const NN_OVERRIDE;

    //@}

    //! @brief 使用文字コード範囲のセットを設定します。
    //!
    //! @param[in] count セットの配列の数です。
    //! @param[in] pFirst 文字コード範囲の先頭位置の配列です。
    //! @param[in] pLast 文字コード範囲の末尾位置の配列です。
    //!
    void SetCharCodeRange(int count, uint32_t* pFirst, uint32_t* pLast);

protected:
    /* ------------------------------------------------------------------------
            型
       ------------------------------------------------------------------------ */
    typedef uint16_t              GlyphIndex;

    /* ------------------------------------------------------------------------
            定数
       ------------------------------------------------------------------------ */
    static const GlyphIndex GlyphIndexNotFound = InvalidGlyphIndex;


    /* ------------------------------------------------------------------------
            関数
       ------------------------------------------------------------------------ */
    //---- メンバアクセス

    //! @brief      オフセット値の起点となるポインタを設定します。
    //!
    //! @param[in]  pFileBase オフセット値の起点となるポインタ。
    void        SetResourceOffsetBasePointer(uint8_t* pFileBase) { m_pResourceOffsetBase = pFileBase; }

    FontInformation*        GetFontInformation()       { return m_pFontInfo; }

    const FontInformation*  GetFontInformation() const { return m_pFontInfo; }

    //! @brief      関連付けられているリソースフォントかどうか判断します。
    //!
    //! @param[in]  pResource 比較対象のリソースフォントへのポインタ。
    //!
    //! @return     この ResFont が pResource の指すフォントリソースと関連付けられている
    //!             ならば true を、そうでなければ false を返します。
    //!
    bool                    IsManaging(const void* pResource) const { return m_pResource == pResource; }

    //! @brief      割り当てられたバッファとそこに配置されている FINF ブロックへの
    //!             ポインタを設定します。
    //!
    //! @details
    //! 本関数の後に GenTextureNames() を呼び出す必要があります。
    //!
    //! @param[in]  pUserBuffer  割り当てられたバッファへのポインタ。
    //! @param[in]  pFontInfo    FINF ブロックへのポインタ。
    //! @param[in]  pKerningTable KRNG ブロックへのポインタ。
    //!
    //! @sa GenTextureNames
    //!
    void                    SetResourceBuffer(
        void*               pUserBuffer,
        FontInformation*    pFontInfo,
        FontKerningTable*   pKerningTable)
    {
        SetResourceBuffer(pUserBuffer, pFontInfo, pKerningTable, NULL, 0, 0);
    }

    //! @brief      割り当てられたバッファとそこに配置されている FINF ブロックへの
    //!             ポインタを設定します。
    //!
    //! @details
    //! 本関数の後に GenTextureNames() を呼び出す必要があります。
    //!
    //! @param[in]  pUserBuffer  割り当てられたバッファへのポインタ。
    //! @param[in]  pFontInfo    FINF ブロックへのポインタ。
    //! @param[in]  pKerningTable KRNG ブロックへのポインタ。
    //! @param[in]  pMemoryPool メモリプールです。バイナリをメモリプール内に配置した場合にそのメモリプールを指定します。未配置の場合は nullptr を指定してください。
    //! @param[in]  memoryPoolOffset 配置先メモリプールの先頭からリソース配置位置へのオフセットです。
    //! @param[in]  memoryPoolSize 配置先メモリプールのサイズです。
    //!
    //! @sa GenTextureNames
    //!
    void                    SetResourceBuffer(
        void*               pUserBuffer,
        FontInformation*    pFontInfo,
        FontKerningTable*   pKerningTable,
        nn::gfx::MemoryPool* pMemoryPool,
        ptrdiff_t memoryPoolOffset,
        size_t memoryPoolSize);

    //! @brief      SetResourceBuffer で設定されたパラメータをリセットし、
    //!             割り当てられていたバッファへのポインタを返します。
    //!
    //! @details
    //! 本関数の前に DeleteTextureNames() を呼ぶ必要があります。
    //!
    //! @return     設定されていたバッファへのポインタ。
    //!
    //! @sa DeleteTextureNames
    //!
    void*                   RemoveResourceBuffer();


    //---- グリフインデックス


    //! @brief      文字のグリフインデックスを取得します。
    //!
    //! @param[in]  c  グリフインデックスを取得する文字の文字コード。
    //!
    //! @return     文字のグリフインデックス。
    //!             フォントに対象の文字が含まれていない場合は代替文字の
    //!             グリフインデックスを返します。
    //!
    GlyphIndex              GetGlyphIndex(uint32_t c) const;

    //! @brief      文字のグリフインデックスを取得します。
    //!
    //! @param[in]  c  グリフインデックスを取得する文字の文字コード。
    //!
    //! @return     文字のグリフインデックス。
    //!             フォントに対象の文字が含まれていない場合は
    //!             ResFontBase::GlyphIndexNotFound を返します。
    //!
    GlyphIndex              FindGlyphIndex(uint32_t c) const;

    //! @brief      文字のグリフインデックスをCMAPブロックから探索します。
    //!
    //! @param[in]  pMap  探索対象のCMAPブロック本体へのポインタ。
    //! @param[in]  c     グリフインデックスを取得する文字の文字コード。
    //!
    //! @return     文字のグリフインデックス。
    //!             フォントに対象の文字が含まれていない場合は
    //!             ResFontBase::GlyphIndexNotFound を返します。
    //!
    GlyphIndex              FindGlyphIndex(
                                const FontCodeMap*  pMap,
                                uint32_t            c
                            ) const;

    //! @brief      グリフインデックスに対応する文字の文字幅を取得します。
    //!
    //! @param[in]  index  文字幅情報を取得する文字のグリフインデックス
    //!
    //! @return     文字幅情報へのリファレンス。
    //!
    const CharWidths&       GetCharWidthsFromIndex(GlyphIndex index) const;

    //! @brief      CWDHブロック本体からグリフインデックスに対応する文字の
    //!             文字幅情報を取得します。
    //!
    //! @param[in]  pWidth  探索対象のCWDHブロック本体へのポインタ。
    //! @param[in]  index   文字幅情報を取得する文字のグリフインデックス
    //!
    //! @return     文字幅情報へのリファレンス。
    //!
    const CharWidths&       GetCharWidthsFromIndex(
                                const FontWidth*    pWidth,
                                GlyphIndex          index
                            ) const;

    //! @brief      グリフインデックスに対応するグリフデータを取得します。
    //!
    //! @param[out] pGlyph  グリフデータを格納するバッファへのポインタ。
    //! @param[in]  index  取得するグリフデータのグリフインデックス。
    //!
    void                    GetGlyphFromIndex(
                                Glyph*      pGlyph,
                                GlyphIndex  index
                            ) const;


    //! @brief      FontTextureGlyph構造体の内容をもとにGlyphのメンバを設定します。
    //!
    //! @param[out] pGlyph  グリフデータを格納するバッファへのポインタ。
    //! @param[in]  index  取得するグリフデータのグリフインデックス。
    //! @param[in]  tg     FontTextureGlyph構造体への参照。
    //!
    static void             SetGlyphMember(
                                Glyph*                  pGlyph,
                                GlyphIndex              index,
                                const FontTextureGlyph& tg);

    // ブロックを探します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  pFileHeader メモリ上にロードしたbffntファイルの
    //!                         ファイルヘッダへのポインタ
    //! @param[in]  sigword     シグネチャ
    //!
    //!
    //! @return     見つかった場合はそのポインタを返します。見つからなかった場合は
    //!             NULLを返します。
    //!
    static void* FindBlock(nn::font::detail::BinaryFileHeader* pFileHeader, uint32_t sigword);

public:
    //! @name シート情報の取得
    //@{

    //! @brief      アクティブなシートの数を取得します。
    //!
    //! @return     アクティブなシートの数を返します。
    //!
    virtual int             GetActiveSheetCount() const;

    //! @brief      テクスチャオブジェクトバッファへのポインタを取得します。
    //!
    //! @return     テクスチャオブジェクトバッファへのポインタを返します。
    //!
    TextureObject*
                            GetTextureObject()
    {
        return &m_TexObj;
    }

    //! @brief      テクスチャオブジェクトバッファへのポインタを取得します。
    //!
    //! @return     テクスチャオブジェクトバッファへのポインタを返します。
    //!
    const TextureObject*
                            GetTextureObject() const
    {
        return &m_TexObj;
    }

    //! @brief テクスチャ名を生成します。
    //!
    //! @param[in] pDevice デバイス
    //!
    void                    GenTextureNames(nn::gfx::Device* pDevice);

    //! @brief テクスチャ名を破棄します。
    //!
    //! @param[in] pDevice デバイス
    //!
    void                    DeleteTextureNames(nn::gfx::Device* pDevice);

    //@}

private:
    bool LoadTexture(nn::gfx::Device* pDevice, ResourceTextureObject* pTexObj) const;
    void UnloadTexture(nn::gfx::Device* pDevice, ResourceTextureObject* pTexObj) const;

    // 指定した文字コードが CharCodeRange の中に含まれているかを判別します。
    bool CheckCharCodeRange(uint32_t code) const;

    /* ------------------------------------------------------------------------
            変数
       ------------------------------------------------------------------------ */

    void*                   m_pResource;    //!< リソースへのポインタ

    uint8_t*                m_pResourceOffsetBase; //! リソース中のオフセット値の起点となるポインタ
    FontInformation*        m_pFontInfo; //! リソース中のFINFブロック本体へのポインタ

    ResourceTextureObject   m_TexObj;       //!< テクスチャオブジェクト
    uint32_t                m_WrapFilter;   //!< テクスチャラップ・フィルタ値
    const FontKerningTable* m_pKerningTable; //!< カーニングの情報を保持するテーブル

    nn::gfx::MemoryPool*    m_pMemoryPool;
    ptrdiff_t               m_MemoryPoolOffset;
    size_t                  m_MemoryPoolSize;

    int m_CharCodeRangeCount;
    uint32_t m_CharCodeRangeFirst[CharCodeRangeCountMax];
    uint32_t m_CharCodeRangeLast[CharCodeRangeCountMax];
};

namespace detail {

//---------------------------------------------------------------------------
//! @brief        1シートあたりのセル数を計算します。
//!
//! @param[in]    tg  FontTextureGlyph構造体への参照。
//!
//! @return       1シートあたりのセル数を返します。
//---------------------------------------------------------------------------
inline
uint32_t
GetCellsInASheet(const FontTextureGlyph& tg)
{
    return static_cast<uint32_t >(tg.sheetRow * tg.sheetLine);
}

}   // namespace detail

}   // namespace font
}   // namespace nn
