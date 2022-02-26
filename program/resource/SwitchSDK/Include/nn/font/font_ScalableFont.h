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

#include <nn/font/font_TextureCache.h>

namespace nn
{
namespace font
{

//---------------------------------------------------------------------------
//! @brief スケーラブルフォントを表すクラスです。
//!
//! @details
//! nn::font::Fontを継承するフォント(ResFontなど)と同じように使用することが
//! できますが、クラスを作成した状態ではまだ文字を表示することができず、
//! 明示的に表示する必要がある文字を指定してテクスチャキャッシュ内にプロット
//! しなければならないという違いがあります。
//---------------------------------------------------------------------------
class ScalableFont : public font::Font
{
public:
    //! 実行時型情報です。
    NN_FONT_RUNTIME_TYPEINFO(font::Font);

    //----------------------------------------------------
    //! @name コンストラクタ／デストラクタ/初期化/終了処理
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 実質的な初期化はInitializeで行われます。ここでは、
    //! メンバ変数の初期化のみ行っています。
    //!
    ScalableFont();

    //! @brief デストラクタです。
    //!
    virtual ~ScalableFont();

    //! @brief 初期化のパラメータを指定するための構造体です。
    //!
    struct InitializeArg {

        //! このフォントで使用するテクスチャキャッシュです。
        //! テクスチャキャッシュは予め初期化しておく必要があります。
        //! デフォルト値はありません。必ず指定してください。
        TextureCache* pTextureCache;
        //! このインスタンスで扱うフォントのサイズです。
        //! デフォルトでは40です。
        int fontSize;
        //! フォントの字形を指定する数値です。
        //! TextureCacheのInitializeで与えたttf(otf)のバイナリポインタの配列のインデックスを
        //! 与えてください。
        //! デフォルトでは0です。
        uint16_t fontFace;
        //! このメンバをtrueにすると、alternateChar の値を無視してフォントの代替文字を空白にし、
        //! かつその幅を元々表示するはずだった文字と同じサイズにします。
        //! falseにしている場合の挙動は、
        //! isAlternateCharSpaceWithOriginalWidthForNotReadyCharによります。
        //! デフォルトではfalseです。
        bool isAlternateCharSpaceWithOriginalWidth;
        //! このメンバはisAlternateCharSpaceWithOriginalWidthがfalseのときのみ有効です。
        //! このメンバをtrueにすると、フォントにない文字については通常の代替文字
        //! (alternateChar で設定した文字、デフォルトでは'?')が使われますが、
        //! フォントにあってまだプロットされていない文字についてはフォントの代替文字を空白にし、
        //! かつその幅を元々表示するはずだった文字と同じサイズにします。
        //! falseにしている場合は、通常の代替文字が使われます。
        //! デフォルトではfalseです。
        bool isAlternateCharSpaceWithOriginalWidthForNotReadyChar;
        //! このメンバはisAlternateCharSpaceWithOriginalWidthがfalseのときのみ有効です。
        //! 代替文字です。
        //! デフォルトでは'?'です。
        uint32_t alternateChar;
        //! 行間のオフセット値です。
        //! fcpx で設定できるようにするためのパラメータですので、fcpx を使わない場合は
        //! 設定する必要がありません。
        //! デフォルトでは 0 です。
        int lineFeedOffset;
        //! NON-BREAKING HYPHEN(U+2011) の文字を描画する際に、HYPHEN-MINUS(U+002D) のグリフを
        //! 使うかどうかです。true にすると、U+2011 の代わりに U+002D のグリフが使われます。
        //! このメンバはフォントに U+2011 のグリフが含まれていない場合などに設定します。
        //! デフォルトは false です。
        bool isDrawingHyphenMinusInsteadOfNonBreakingHyphen;

        //! @brief デフォルト値を設定します。
        void SetDefault();
    };

    //! @brief 初期化を行います。
    //!
    //! @details
    //! 内部でリソースの確保は行いませんので、Finalizeメソッドはありません。
    //!
    //! @param[in] arg  パラメータを指定する構造体
    //!
    void Initialize(const InitializeArg& arg);
    //@}

    //----------------------------------------------------
    //! @name テクスチャキャッシュの操作
    //@{

    //! @brief 指定した文字を、描画する文字として登録します。
    //!
    //! @details
    //! 描画する文字として登録した文字は、テクスチャキャッシュになかった場合は、新たに
    //! プロットする必要がある文字として登録されます。この文字は、UpdateTextureCacheと
    //! CompleteTextureCacheを行った段階で使えるようになります。
    //!
    //! 描画する文字として登録した文字が、テクスチャキャッシュに既にあった場合は、
    //! プロット等は行われず、引き続き使用できます。また、その文字は必要な文字として
    //! 登録されますので、削除対象になりません。
    //!
    //! 文字をロックする際のグループは、0から31までの32個あります。
    //! ロックはグループ毎に行われますので、例えばグル―プ0で登録された文字がグループ1
    //! でもう一度登録された場合、その両方でロックされたことになり、両方についてロック
    //! がクリアされないと削除対象になりません。
    //!
    //! @param[in] code         描画する文字として登録する文字
    //! @param[in] lockGroup    文字をロックする際のグループ。0から31までの数値で指定してください。-1を指定した場合はロックしません。
    //! @return 登録した文字のプロットが必要な場合にtrueが返されます。つまり、falseが返った場合はその文字は
    //!         UpdateTextureCacheを行わなくても描画できます。
    //!
    bool RegisterGlyph(uint32_t code, int lockGroup) const
    {
        return m_pTextureCache->RegisterGlyph(code, m_FontSize, m_FontFace, lockGroup, m_IsDrawingHyphenMinusInsteadOfNonBreakingHyphen);
    }

    //! @brief @ref RegisterGlyph のロック指定省略版です。
    //!
    //! @param[in] code         描画する文字として登録する文字
    //! @return 登録した文字のプロットが必要な場合にtrueが返されます。つまり、falseが返った場合はその文字は
    //!         UpdateTextureCacheを行わなくても描画できます。
    //!
    bool RegisterGlyph(uint32_t code) const
    {
        return RegisterGlyph(code, -1);
    }

    //! @brief 指定した文字列に含まれる文字を、描画する文字として登録します。UTF-16版。
    //!
    //! @details
    //! 登録する文字を文字列として指定できる以外は @ref RegisterGlyph と同じです。
    //!
    //! @param[in] pCode        描画する文字として登録する文字列。NULL終端する必要があります。
    //! @param[in] lockGroup    文字をロックする際のグループ。0から31までの数値で指定してください。-1を指定した場合はロックしません。
    //! @return プロットが必要な文字が何文字あったかを返します。つまり0が返された場合は、
    //!         全ての文字がUpdateTextureCacheを行わなくても描画できることを示します。
    //!
    uint32_t RegisterGlyphs(const uint16_t* pCode, int lockGroup) const
    {
        return m_pTextureCache->RegisterGlyphsWithLength(pCode, UINT_MAX, m_FontSize, m_FontFace, lockGroup, m_IsDrawingHyphenMinusInsteadOfNonBreakingHyphen);
    }

    //! @brief @ref RegisterGlyphs のロック指定省略版です。
    //!
    //! @param[in] pCode        描画する文字として登録する文字列。NULL終端する必要があります。
    //! @return プロットが必要な文字が何文字あったかを返します。つまり0が返された場合は、
    //!         全ての文字がUpdateTextureCacheを行わなくても描画できることを示します。
    //!
    uint32_t RegisterGlyphs(const uint16_t* pCode) const
    {
        return RegisterGlyphs(pCode, -1);
    }

    //! @brief 指定した文字列に含まれる文字を、描画する文字として登録します。UTF-8版。
    //!
    //! @details
    //! 登録する文字を文字列として指定できる以外は @ref RegisterGlyph と同じです。
    //!
    //! @param[in] pCode        描画する文字として登録する文字列。NULL終端する必要があります。
    //! @param[in] lockGroup    文字をロックする際のグループ。0から31までの数値で指定してください。-1を指定した場合はロックしません。
    //! @return プロットが必要な文字が何文字あったかを返します。つまり0が返された場合は、
    //!         全ての文字がUpdateTextureCacheを行わなくても描画できることを示します。
    //!
    uint32_t RegisterGlyphsUtf8(const char* pCode, int lockGroup) const
    {
        return m_pTextureCache->RegisterGlyphsWithLengthUtf8(pCode, UINT_MAX, m_FontSize, m_FontFace, lockGroup, m_IsDrawingHyphenMinusInsteadOfNonBreakingHyphen);
    }

    //! @brief @ref RegisterGlyphsUtf8 のロック指定省略版です。
    //!
    //! @param[in] pCode        描画する文字として登録する文字列。NULL終端する必要があります。
    //! @return プロットが必要な文字が何文字あったかを返します。つまり0が返された場合は、
    //!         全ての文字がUpdateTextureCacheを行わなくても描画できることを示します。
    //!
    uint32_t RegisterGlyphsUtf8(const char* pCode) const
    {
        return RegisterGlyphsUtf8(pCode, -1);
    }

    //! @brief 指定した文字列に含まれる指定した文字数だけの文字を、描画する文字として登録します。UTF-16版。
    //!
    //! @details
    //! 登録する文字数を指定できる以外は @ref RegisterGlyphs と同じです。
    //!
    //! @param[in] pCode        描画する文字として登録する文字列
    //! @param[in] codeLength   文字列の文字数。この文字数より短い文字数でNULL終端が来た場合は、登録はそこで終了します。
    //! @param[in] lockGroup    文字をロックする際のグループ。0から31までの数値で指定してください。-1を指定した場合はロックしません。
    //! @return プロットが必要な文字が何文字あったかを返します。つまり0が返された場合は、
    //!         全ての文字がUpdateTextureCacheを行わなくても描画できることを示します。
    //!
    uint32_t RegisterGlyphsWithLength(const uint16_t* pCode, uint32_t codeLength, int lockGroup) const
    {
        return m_pTextureCache->RegisterGlyphsWithLength(pCode, codeLength, m_FontSize, m_FontFace, lockGroup, m_IsDrawingHyphenMinusInsteadOfNonBreakingHyphen);
    }

    //! @brief @ref RegisterGlyphsWithLength のロック指定省略版です。
    //!
    //! @param[in] pCode        描画する文字として登録する文字列
    //! @param[in] codeLength   文字列の文字数。この文字数より短い文字数でNULL終端が来た場合は、登録はそこで終了します。
    //! @return プロットが必要な文字が何文字あったかを返します。つまり0が返された場合は、
    //!         全ての文字がUpdateTextureCacheを行わなくても描画できることを示します。
    //!
    uint32_t RegisterGlyphsWithLength(const uint16_t* pCode, uint32_t codeLength) const
    {
        return RegisterGlyphsWithLength(pCode, codeLength, -1);
    }

    //! @brief 指定した文字列に含まれる指定した文字数だけの文字を、描画する文字として登録します。UTF-8版。
    //!
    //! @details
    //! 登録する文字数を指定できる以外は @ref RegisterGlyphs と同じです。
    //!
    //! @param[in] pCode        描画する文字として登録する文字列
    //! @param[in] codeLength   文字列のバイト数。この文字数より短い文字数でNULL終端が来た場合は、登録はそこで終了します。
    //! @param[in] lockGroup    文字をロックする際のグループ。0から31までの数値で指定してください。-1を指定した場合はロックしません。
    //! @return プロットが必要な文字が何文字あったかを返します。つまり0が返された場合は、
    //!         全ての文字がUpdateTextureCacheを行わなくても描画できることを示します。
    //!
    uint32_t RegisterGlyphsWithLengthUtf8(const char* pCode, uint32_t codeLength, int lockGroup) const
    {
        return m_pTextureCache->RegisterGlyphsWithLengthUtf8(pCode, codeLength, m_FontSize, m_FontFace, lockGroup, m_IsDrawingHyphenMinusInsteadOfNonBreakingHyphen);
    }

    //! @brief @ref RegisterGlyphsWithLengthUtf8 のロック指定省略版です。
    //!
    //! @param[in] pCode        描画する文字として登録する文字列
    //! @param[in] codeLength   文字列のバイト数。この文字数より短い文字数でNULL終端が来た場合は、登録はそこで終了します。
    //! @return プロットが必要な文字が何文字あったかを返します。つまり0が返された場合は、
    //!         全ての文字がUpdateTextureCacheを行わなくても描画できることを示します。
    //!
    uint32_t RegisterGlyphsWithLengthUtf8(const char* pCode, uint32_t codeLength) const
    {
        return RegisterGlyphsWithLengthUtf8(pCode, codeLength, -1);
    }

    //! @brief 指定した文字列に含まれる文字のロックを解除します。UTF-16版。
    //!
    //! @param[in] pCode        ロックを解除する文字が含まれる文字列。NULL終端する必要があります。
    //! @param[in] lockGroup    ロックを解除するグループ。0から31までの数値で指定してください。
    //!
    void ClearLockGlyphs(const uint16_t* pCode, int lockGroup) const
    {
        m_pTextureCache->ClearLockGlyphsWithLength(pCode, UINT_MAX, m_FontSize, m_FontFace, lockGroup, m_IsDrawingHyphenMinusInsteadOfNonBreakingHyphen);
    }

    //! @brief 指定した文字列に含まれる文字のロックを解除します。UTF-8版。
    //!
    //! @param[in] pCode        ロックを解除する文字が含まれる文字列。NULL終端する必要があります。
    //! @param[in] lockGroup    ロックを解除するグループ。0から31までの数値で指定してください。
    //!
    void ClearLockGlyphsUtf8(const char* pCode, int lockGroup) const
    {
        m_pTextureCache->ClearLockGlyphsWithLengthUtf8(pCode, UINT_MAX, m_FontSize, m_FontFace, lockGroup, m_IsDrawingHyphenMinusInsteadOfNonBreakingHyphen);
    }

    //! @brief 指定した長さの文字列に含まれる文字のロックを解除します。UTF-16版。
    //!
    //! @param[in] pCode        ロックを解除する文字が含まれる文字列。
    //! @param[in] codeLength   文字列の文字数。この文字数より短い文字数でNULL終端が来た場合は、そこで終了します。
    //! @param[in] lockGroup    ロックを解除するグループ。0から31までの数値で指定してください。
    //!
    void ClearLockGlyphs(const uint16_t* pCode, uint32_t codeLength, int lockGroup) const
    {
        m_pTextureCache->ClearLockGlyphsWithLength(pCode, codeLength, m_FontSize, m_FontFace, lockGroup, m_IsDrawingHyphenMinusInsteadOfNonBreakingHyphen);
    }

    //! @brief 指定した長さの文字列に含まれる文字のロックを解除します。UTF-8版。
    //!
    //! @param[in] pCode        ロックを解除する文字が含まれる文字列。
    //! @param[in] codeLength   文字列のバイト数。この文字数より短い文字数でNULL終端が来た場合は、そこで終了します。
    //! @param[in] lockGroup    ロックを解除するグループ。0から31までの数値で指定してください。
    //!
    void ClearLockGlyphsUtf8(const char* pCode, uint32_t codeLength, int lockGroup) const
    {
        m_pTextureCache->ClearLockGlyphsWithLengthUtf8(pCode, codeLength, m_FontSize, m_FontFace, lockGroup, m_IsDrawingHyphenMinusInsteadOfNonBreakingHyphen);
    }

    //! @brief このフォントが利用しているテクスチャキャッシュを取得します。
    //!
    //! @return テクスチャキャッシュです。
    //!
    TextureCache* GetTextureCache() const { return m_pTextureCache; }

    //! @brief このフォントで使用している代替文字を登録します。
    //!
    //! @details
    //! 通常代替文字の登録はInitializeで行われますが、TextureCacheをリセットすると、
    //! 代替文字も消えてしまうため再登録が必要となります。
    //!
    void RegisterAlternateCharGlyph() const;
    //@}

    //----------------------------------------------------
    //! @name フォントに関する情報の取得
    //@{
    //! @brief 初期化時に設定したフォントの字形を指定する数値を取得します。
    //!
    //! @return フォントの字形を指定する数値
    //!
    uint16_t GetFontFace() const { return m_FontFace; }

    //! @brief 初期化時に設定したフォントのサイズを指定する数値を取得します。
    //!
    //! @return 初期化時に設定したフォントのサイズを指定する数値
    //!
    uint32_t GetFontSize() const { return m_FontSize; }

    //! @brief このフォントを利用して、マルチコアでの描画が行えるか否かを返します。
    //!
    //! @return マルチコアでの描画が行えるか否か
    //!
    bool IsDrawMultiCore() const { return m_pTextureCache->IsDrawMultiCore(); }

    //! @brief  フォントの中に指定した文字コードの文字が含まれているかを返します。
    //!
    //! @details
    //! 登録されているTextureCacheクラスのIsGlyphExistInFontメソッドを呼び出す
    //! ユーティリティです。テクスチャキャッシュの状態には関係なく、ttf(otf)の中に
    //! 文字が含まれているかを返します。
    //!
    //! @param[in] code         幅を求めたい文字
    //! @return 含まれるか否か
    //!
    bool IsGlyphExistInFont(uint32_t code) const { return m_pTextureCache->IsGlyphExistInFont(code, m_FontFace); }
    //@}

    //----------------------------------------------------
    //! @name フォントの設定を変更するためのインターフェース
    //@{
    /**
     *  フォントの高さを変更します。
     *
     *  通常、フォントの高さはinitializeメソッドによって自動的に設定され、それを
     *  修正する必要はありません。
     *
     *  このメソッドを使うことで、異なるフォントを同じテキストボックスで使いたい
     *  場合に、同じくらいのサイズに見えるようにフォントの縦方向のスケールを調整
     *  することができます。
     *
     *  @param[in] height   設定したい高さ
     */
    void                    SetHeight(int height) { m_FontHeight = height; }

    /**
     *  フォントのベースライン位置を変更します。
     *
     *  通常、フォントのベースライン位置はinitializeメソッドによって自動的に設定され、
     *  それを修正する必要はありません。
     *
     *  このメソッドを使うことで、SetHeightでフォントの縦方向のスケールを調整した
     *  ときに、縦方向の表示位置がずれてしまうのを調整することができます。
     *
     *  @param[in] pos  設定したいベースライン位置
     */
    void                    SetBaselinePos(int pos) { m_FontBaselinePos = pos; }
    //@}

    //----------------------------------------------------
    //! @name Fontクラスのインターフェース
    //@{

    //! @brief      フォントの幅を取得します。
    //!
    //! @return     ピクセル単位でのフォントの幅を返します。
    //!
    virtual int             GetWidth() const NN_OVERRIDE;

    //! @brief      フォントの高さを取得します。
    //!
    //! @return     ピクセル単位でのフォントの高さを返します。
    //!
    virtual int             GetHeight() const NN_OVERRIDE;

    //! @brief      フォントのアセントを取得します。
    //!
    //! @return     ピクセル単位でのフォントのアセントを返します。
    //!
    virtual int             GetAscent() const NN_OVERRIDE;

    //! @brief      フォントのディセントを取得します。
    //!
    //! @return     ピクセル単位でのフォントのディセントを返します。
    //!
    virtual int             GetDescent() const NN_OVERRIDE;

    //! @brief      フォントに含まれる最大の文字幅を取得します。
    //!
    //! @return     フォントに含まれる最大の文字幅をピクセル単位で返します。
    //!
    virtual int             GetMaxCharWidth() const NN_OVERRIDE;

    //! @brief      フォントのタイプを取得します。
    //!
    //! @return     フォントのタイプを返します。
    //!
    virtual font::Font::Type GetType() const NN_OVERRIDE;

    //! @brief      フォントのテクスチャフォーマットを取得します。
    //!
    //! @return     グリフデータのテクスチャフォーマットを返します。
    //!
    virtual font::TexFmt    GetTextureFormat() const NN_OVERRIDE;

    //! @brief      フォントが持つ改行幅を取得します。
    //!
    //! @return     フォントが持つ改行幅を返します。
    //!
    virtual int             GetLineFeed() const NN_OVERRIDE;

    //! @brief      フォントのデフォルト文字幅情報を取得します。
    //!
    //! @return     フォントのデフォルト文字幅情報を返します。
    //!
    virtual const font::CharWidths GetDefaultCharWidths() const NN_OVERRIDE;

    //! @brief      フォントがもつ改行幅を設定します。
    //!
    //! @param[in]  linefeed  新しい改行幅。
    //!
    virtual void            SetLineFeed(int linefeed) NN_OVERRIDE;

    //! @brief      フォントのデフォルト文字幅情報を設定します。
    //!
    //! @param[in]  widths  新しいデフォルトと文字幅情報。
    //!
    virtual void            SetDefaultCharWidths(
                                const font::CharWidths& widths
                            ) NN_OVERRIDE;

    //! @brief      フォントの代替文字を変更します。
    //!
    //! @param[in]  c  新しい代替文字の文字コード。
    //!
    //! @return     代替文字の変更に成功したら true 、失敗したら false を返します。@n
    //!             フォント内に文字コード c に対応するグリフが存在しない場合に失敗します。
    //!
    virtual bool            SetAlternateChar(uint32_t c) NN_OVERRIDE;

    //! @brief      文字の文字幅を取得します。
    //!
    //! @details
    //! このメソッドは、文字が既にプロットされていたら、その文字の幅を返します。ただし、まだ
    //! 文字がプロットされていない場合の挙動は、初期化時のInitializeArgでどのような設定に
    //! しているかによって異なります。
    //!
    //! isAlternateCharSpaceWithOriginalWidthとisAlternateCharSpaceWithOriginalWidthForNotReadyChar
    //! を何れもfalseにしている場合は、「プロットされていない文字は常に代替文字になる」設定で
    //! あるため、描画するものとGetCharWidthの結果が一致するように、GetCharWidthはプロットされて
    //! いない文字については代替文字の幅を返します。
    //!
    //! それに対して、「プロットされていない文字は常に元々表示するはずだった文字と同じ幅の空白
    //! として扱う」か、もしくは「プロットされていない文字は、それがフォント内になければ代替文字にし、
    //! フォントにあれば元々表示するはずだった文字と同じ幅の空白として扱う」設定だった場合は、
    //! GetCharWidthはプロットされていない文字についてはスケーラブルフォントエンジンから直接文字幅を
    //! 取得し、代替文字ではなくその文字の幅を返します。
    //!
    //! スケーラブルフォントエンジンから幅を取得する場合、テクスチャキャッシュ内にある場合より
    //! 遅くなります。
    //! WiiU実機で、一文字につき5マイクロ秒程度かかりますのでご注意ください。
    //!
    //! スケーラブルフォントエンジンがメモリ不足になった際には assert によって止まります。
    //! その場合は TextureCache の InitializeArg::noPlotWorkMemorySize の値を増やしてください。
    //!
    //! @param[in]  c  幅を求める文字の文字コード。
    //!
    //! @return     ピクセル単位での文字幅を返します。
    //!
    virtual int             GetCharWidth(uint32_t c) const NN_OVERRIDE;

    //! @brief      文字の文字幅情報を取得します。
    //!
    //! @details
    //! 引数で指定した文字がまだプロットされていない場合の挙動は @ref GetCharWidth と同じです。
    //!
    //! スケーラブルフォントエンジンがメモリ不足になった際には assert によって止まります。
    //! その場合は TextureCache の InitializeArg::noPlotWorkMemorySize の値を増やしてください。
    //!
    //! @param[in]  c  文字幅情報を取得する文字の文字コード。
    //!
    //! @return         文字幅情報を返します。
    //!
    virtual const font::CharWidths
                            GetCharWidths(uint32_t c) const NN_OVERRIDE;

    //! @brief      グリフデータを取得します。
    //!
    //! @details
    //! スケーラブルフォントエンジンがメモリ不足になった際には assert によって止まります。
    //! その場合は TextureCache の InitializeArg::noPlotWorkMemorySize の値を増やしてください。
    //!
    //! @param[out] pGlyph  グリフデータを受け取るバッファへのポインタ
    //! @param[in]  c       グリフデータを取得する文字の文字コード
    //!
    virtual void            GetGlyph(
                                font::Glyph*      pGlyph,
                                uint32_t    c
                            ) const NN_OVERRIDE;

    //! @brief      グリフデータの有無を取得します。
    //!
    //! @param[in]  c  グリフデータの有無を取得する文字の文字コード
    //!
    //! @return     グリフデータが存在する場合は true 、存在しない場合は false を返します。
    //!
    virtual bool            HasGlyph(
                                uint32_t    c
                            ) const NN_OVERRIDE;

    //! @brief      カーニングの値を取得します。
    //!
    //! @param[in]  c0  カーニングの値を取得する、進行方向と逆側の文字
    //! @param[in]  c1  カーニングの値を取得する、進行方向側の文字
    //!
    //! @return     カーニングの値を返します。
    //!
    virtual int            GetKerning(uint32_t c0, uint32_t c1) const NN_OVERRIDE;

    //! @brief      フォントが対応している文字コードを取得します。
    //!
    //! @return     フォントが対応している文字コードを返します。
    //!
    virtual font::CharacterCode GetCharacterCode() const NN_OVERRIDE;

    //! @brief      ベースライン位置を取得します。
    //!
    //! @return     セルの上端からベースラインまでのピクセル数を返します。
    //!
    virtual int             GetBaselinePos() const NN_OVERRIDE;

    //! セルの高さを取得します。

    //! @brief      セルの高さを取得します。
    //!
    //! @return     ピクセル単位でのセルの高さを返します。
    //!
    virtual int             GetCellHeight() const NN_OVERRIDE;

    //! @brief      セルの幅を取得します。
    //!
    //! @return     ピクセル単位でのセルの幅を返します。
    //!
    virtual int             GetCellWidth() const NN_OVERRIDE;

    //! @brief      文字描画時にテクスチャの線形補間を行うかどうかを指定します。
    //!
    //! @param[in]  atSmall 文字がオリジナルサイズより小さく表示される場合に線形補間を
    //!                     行うのであれば true を指定します。
    //! @param[in]  atLarge 文字がオリジナルサイズより大きく表示される場合に線形補間を
    //!                     行うのであれば true を指定します。
    //!
    virtual void            SetLinearFilterEnabled(
                                bool    atSmall,
                                bool    atLarge
                            ) NN_OVERRIDE;

    //! @brief      文字が小さく表示される場合に線形補間が行われるかを取得します。
    //!
    //! @return     文字がオリジナルサイズより小さく表示される場合に線形補間が行われるのであれば
    //!             true を、行われないのであれば false を返します。
    //!
    virtual bool            IsLinearFilterEnabledAtSmall() const NN_OVERRIDE;

    //! @brief      文字が大きく表示される場合に線形補間が行われるかを取得します。
    //!
    //! @return     文字がオリジナルサイズより大きく表示される場合に線形補間が行われるのであれば
    //!             true を、行われないのであれば false を返します。
    //!
    virtual bool            IsLinearFilterEnabledAtLarge() const NN_OVERRIDE;

    //! @brief      テクスチャのラップ、フィルタ設定値を取得します。
    //!
    //! @return     テクスチャのラップ、フィルタ設定値を返します。
    //!
    virtual uint32_t             GetTextureWrapFilterValue() const NN_OVERRIDE;

    //! @brief        カラーの白黒補間の影響の有無を取得します。
    //!
    //! @return       常にtrueを返します。
    //!
    virtual bool            IsColorBlackWhiteInterpolationEnabled() const NN_OVERRIDE;

    //! @brief        カラーの白黒補間の影響の有無を設定します。
    //!
    //! @details
    //! このクラスでは、白黒補完は常に有効であるため、このメソッドは何もしません。
    //!
    //! @param[in] flag true を設定すると、白黒補間の影響を受けます。
    //!
    virtual void            SetColorBlackWhiteInterpolationEnabled(bool flag) NN_OVERRIDE;

    //! @brief        特殊フチ形式が設定可能かを取得します。
    //!
    //! @return       特殊フチ形式が設定可能な場合は true を返します。
    //!
    virtual bool            IsBorderAvailable() const NN_OVERRIDE
    {
        return true;
    }

    //! @brief        縁取り効果の有無を取得します。
    //!
    //! @return       縁取り効果が有効な場合は、true を返します。
    //!
    virtual bool            IsBorderEffectEnabled() const NN_OVERRIDE;
    //@}

private:

    static const uint32_t CharCodeAlternateWhiteSpace = ' ';

    enum AlternateCharMode {
        AlternateCharMode_UseWhiteSpace,
        AlternateCharMode_UseWhiteSpaceForNotReadyChar,
        AlternateCharMode_UseAlternateChar
    };

    //----------------------------------------------------------

    TextureCache* m_pTextureCache;
    uint32_t m_FontSize;
    int m_FontHeight;
    int m_FontAscent;
    int m_FontBaselinePos;
    int m_FontLineFeed;
    font::CharWidths m_FontDefaultCharWidth;
    uint8_t m_AlternateCharMode;
    bool m_IsDrawingHyphenMinusInsteadOfNonBreakingHyphen;
    uint16_t m_FontFace;
    uint32_t m_FontAlternateChar;
};

} // namespace nn::font
} // namespace nn
