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
#include <nn/gfx/gfx_ResTexture.h>

#include <nn/font/font_Util.h>
#include <nn/font/font_CharStrmReader.h>
#include <nn/font/font_ResourceFormat.h>
#include <nn/gfx.h>
#include <climits>

#define NN_FONT_MIN_ASSERT(exp, min)            NN_SDK_ASSERT((exp) >= (min))

#define NN_FONT_MINMAX_ASSERT(exp, min, max)    NN_SDK_ASSERT((exp) >= (min) && (exp) <= (max))


namespace nn {
namespace font {

//---------------------------------------------------------------------------

namespace detail {

    static inline size_t GetConstantBufferAlignment(nn::gfx::Device* pDevice)
    {
        nn::gfx::BufferInfo info;

        info.SetDefault();
        info.SetGpuAccessFlags(nn::gfx::GpuAccess_ConstantBuffer);

        return nn::gfx::Buffer::GetBufferAlignment(pDevice, info);
    }

    static inline size_t GetIndexBufferAlignment(nn::gfx::Device* pDevice)
    {
        nn::gfx::BufferInfo info;

        info.SetDefault();
        info.SetGpuAccessFlags(nn::gfx::GpuAccess_IndexBuffer);

        return nn::gfx::Buffer::GetBufferAlignment(pDevice, info);
    }

    static inline size_t GetVertexBufferAlignment(nn::gfx::Device* pDevice)
    {
        nn::gfx::BufferInfo info;

        info.SetDefault();
        info.SetGpuAccessFlags(nn::gfx::GpuAccess_VertexBuffer);

        return nn::gfx::Buffer::GetBufferAlignment(pDevice, info);
    }

    static inline size_t GetMemoryPoolAlignment(nn::gfx::Device* pDevice)
    {
        nn::gfx::MemoryPoolInfo info;

        info.SetDefault();
        info.SetMemoryPoolProperty(nn::gfx::MemoryPoolProperty_CpuUncached | nn::gfx::MemoryPoolProperty_GpuCached);

        return nn::gfx::MemoryPool::GetPoolMemoryAlignment(pDevice, info);
    }

    static inline size_t GetPoolMemorySizeGranularity(nn::gfx::Device* pDevice)
    {
        nn::gfx::MemoryPoolInfo info;

        info.SetDefault();
        info.SetMemoryPoolProperty(nn::gfx::MemoryPoolProperty_CpuUncached | nn::gfx::MemoryPoolProperty_GpuCached);

        return nn::gfx::MemoryPool::GetPoolMemorySizeGranularity(pDevice, info);
    }

    static inline size_t GetGraphicsBufferAlignment(nn::gfx::Device* pDevice)
    {
        return GetMemoryPoolAlignment(pDevice);
    }

} // detail

//---------------------------------------------------------------------------

class Font;

//---- テクスチャフォーマット型
typedef uint16_t      TexFmt;

#if defined(_WIN32)
#pragma warning(push)
#pragma warning(disable:4201)
#endif
union TextureSize
{
    struct
    {
        uint16_t                  height;   //!< 高さ
        uint16_t                  width;    //!< 幅
    };

    uint32_t                  size;   //!< サイズ
};
#if defined(_WIN32)
#pragma warning(pop)
#endif

//---------------------------------------------------------------------------
//! @brief        テクスチャを管理するクラスです。
//!
//! @details
//! ユーザーが直接このクラスを利用することはなく、本クラスの派生クラスを利用します。
//!
//! @sa PlacementTextureObject
//! @sa ResourceTextureObject
//---------------------------------------------------------------------------
class TextureObject
{
public:
    //! @brief      コンストラクタです。
    TextureObject()
    {
        Reset();
    }

    //! @brief      デストラクタです。
    virtual ~TextureObject() { Reset(); }

    //! @brief      状態を設定します。
    //!
    //! @param[in]  pImage 画像データです。
    //! @param[in]  format 画像フォーマットです。
    //! @param[in]  width  画像の幅です。
    //! @param[in]  height 画像の高さです。
    //! @param[in]  sheetCount シート枚数です。
    //! @param[in]  blackWhiteInterpolationEnabled 白黒補完が有効かどうかを表すフラグです。
    //!
    void Set(
        const void* pImage,
        TexFmt      format,
        uint16_t    width,
        uint16_t    height,
        uint8_t     sheetCount,
        bool        blackWhiteInterpolationEnabled);

    //! @brief      状態をリセットします。
    void Reset()
    {
        Set(NULL, FontSheetFormat_Bc4, 0, 0, 0, true);
        m_DescriptorSlotForTexture.ToData()->value = InvalidDescriptorSlotValue;
        m_isInitialized = false;
    }

    //! @brief      初期化が完了しているかどうかを取得します。
    //!
    //! @return     初期化されている場合は true を返します。
    bool IsInitialized() const
    {
        return m_isInitialized;
    }

    //! @brief      画像へのポインタを取得します。
    //!
    //! @return     画像へのポインタです。
    uintptr_t GetImage() const
    {
        return m_pImage;
    }

    //! @brief      画像フォーマットを取得します。
    //!
    //! @return     画像フォーマットです。
    uint16_t  GetFormat() const
    {
        return m_Format & FontSheetFormat_Mask;
    }

    //! @brief      画像属性値を取得します。
    //!
    //! @return     画像属性値です。
    uint16_t  GetFormatAttribute() const
    {
        return m_Format & ~FontSheetFormat_Mask;
    }

    //! @brief      シート枚数を取得します。
    //!
    //! @return     シート枚数です。
    uint8_t  GetSheetCount() const
    {
        return m_SheetCount;
    }

    //! @brief      テクスチャサイズを取得します。
    //!
    //! @return     テクスチャサイズです。
    const TextureSize GetSize() const
    {
        return m_Size;
    }

    //! @brief      白黒補完が有効かどうかを取得します。
    //!
    //! @return     白黒補完が有効かどうかです。
    bool IsColorBlackWhiteInterpolationEnabled() const
    {
        return m_ColorBlackWhiteInterpolationEnabled;
    }

    //! @brief      白黒補完が有効かどうかを設定します。
    //!
    //! @param[in]  flag 白黒補完が有効かどうかのフラグです。
    void SetColorBlackWhiteInterpolationEnabled(bool flag)
    {
        m_ColorBlackWhiteInterpolationEnabled = flag;
    }

    //! @brief      テクスチャビューを取得します。
    //!
    //! @return     テクスチャビューです。
    virtual const nn::gfx::TextureView* GetTextureView() const = 0;

    //! @brief      テクスチャビューを取得します。
    //!
    //! @return     テクスチャビューです。
    virtual nn::gfx::TextureView* GetTextureView() = 0;

    //! @brief      テクスチャディスクリプタスロットを取得します。
    //!
    //! @return     テクスチャディスクリプタスロットです。
    const nn::gfx::DescriptorSlot& GetDescriptorSlotForTexture() const { return m_DescriptorSlotForTexture; }

    //! @brief      テクスチャディスクリプタスロットを取得します。
    //!
    //! @return     テクスチャディスクリプタスロットです。
    nn::gfx::DescriptorSlot& GetDescriptorSlotForTexture() { return m_DescriptorSlotForTexture; }

    //! @brief      テクスチャディスクリプタスロットが利用可能な状態か取得します。
    //!
    //! @return     テクスチャディスクリプタスロットが利用可能な状態なら true を返します。
    bool IsDescriptorSlotForTextureReady() const { return m_DescriptorSlotForTexture.ToData()->value != InvalidDescriptorSlotValue; }

protected:
    //! @brief      画像へのポインタを取得します。
    //!
    //! @return     画像へのポインタです。
    uintptr_t GetImagePtr() const { return m_pImage; }
private:
    static const uint64_t InvalidDescriptorSlotValue = 0xFFFFFFFFFFFFFFFF;

    uintptr_t   m_pImage; //!< イメージへのポインタ
    TextureSize m_Size; //!< サイズ
    uint16_t    m_Format; //!< フォーマット
    uint8_t     m_SheetCount; //!< シート数

    bool m_ColorBlackWhiteInterpolationEnabled; //!< 白黒補完が有効か否か
    bool m_isInitialized;

    nn::gfx::DescriptorSlot m_DescriptorSlotForTexture;
};

//---------------------------------------------------------------------------
//! @brief        メモリ領域から初期化されるテクスチャを管理するクラスです。
//! @sa ResourceTextureObject
//---------------------------------------------------------------------------
class PlacementTextureObject : public TextureObject
{
public:
    //! @brief      コンストラクタです。
    PlacementTextureObject()
    {
    }

    //! @brief      デストラクタです。
    virtual ~PlacementTextureObject() {}

    //! @brief      テクスチャビューを取得します。
    //!
    //! @return     テクスチャビューです。
    virtual const nn::gfx::TextureView* GetTextureView() const NN_OVERRIDE { return NULL; }

    //! @brief      テクスチャビューを取得します。
    //!
    //! @return     テクスチャビューです。
    virtual nn::gfx::TextureView* GetTextureView() NN_OVERRIDE { return NULL; }
};

//---------------------------------------------------------------------------
//! @brief        リソースファイルから初期化されるテクスチャを管理するクラスです。
//! @sa PlacementTextureObject
//---------------------------------------------------------------------------
class ResourceTextureObject : public TextureObject
{
public:
    //! @brief      コンストラクタです。
    ResourceTextureObject()
        : m_pResTexture(NULL)
    {
        Reset();
    }

    //! @brief      デストラクタです。
    virtual ~ResourceTextureObject() {}

    //! @brief      テクスチャを取得します。
    //!
    //! @return     テクスチャです。
    const nn::gfx::Texture& GetTexture() const
    {
        nn::gfx::Texture* pTexture = m_pResTexture->GetTexture();
        return *pTexture;
    }

    //! @brief      テクスチャを取得します。
    //!
    //! @return     テクスチャです。
    nn::gfx::Texture& GetTexture()
    {
        nn::gfx::Texture* pTexture = m_pResTexture->GetTexture();
        return *pTexture;
    }

    //! @brief      テクスチャビューを取得します。
    //!
    //! @return     テクスチャビューです。
    virtual const nn::gfx::TextureView* GetTextureView() const NN_OVERRIDE { return m_pResTexture->GetTextureView(); }

    //! @brief      テクスチャビューを取得します。
    //!
    //! @return     テクスチャビューです。
    virtual nn::gfx::TextureView* GetTextureView() NN_OVERRIDE { return m_pResTexture->GetTextureView(); }

    //! @brief      初期化します。
    //!
    //! @param[in]  pDevice デバイスです。
    void Initialize(nn::gfx::Device* pDevice)
    {
        Initialize(pDevice, NULL, 0, 0);
    }

    //! @brief      初期化します。
    //!
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  pMemoryPool メモリプールです。バイナリをメモリプール内に配置した場合にそのメモリプールを指定します。未配置の場合は nullptr を指定してください。
    //! @param[in]  memoryPoolOffset 配置先メモリプールの先頭からリソース配置位置へのオフセットです。
    //! @param[in]  memoryPoolSize 配置先メモリプールのサイズです。
    void Initialize(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize)
    {
        nn::gfx::ResTextureFile* pResTextureFile = nn::gfx::ResTextureFile::ResCast(reinterpret_cast<void*>(GetImagePtr()));
        const bool isResTextureFileInitialized = IsResTextureFileInitialized(pResTextureFile);
        if (!isResTextureFileInitialized)
        {
            if (pMemoryPool == NULL)
            {
                pResTextureFile->Initialize(pDevice);
            }
            else
            {
                pResTextureFile->Initialize(pDevice, pMemoryPool, memoryPoolOffset, memoryPoolSize);
            }
        }

        m_pResTexture = pResTextureFile->GetResTexture(0);

        if (!isResTextureFileInitialized)
        {
            m_pResTexture->Initialize(pDevice);
        }
    }

    //! @brief      終了処理をします。
    //!
    //! @param[in]  pDevice デバイスです。
    void Finalize(nn::gfx::Device* pDevice)
    {
        nn::gfx::ResTextureFile* pResTextureFile = nn::gfx::ResTextureFile::ResCast(reinterpret_cast<void*>(GetImagePtr()));
        if (!IsResTextureFileInitialized(pResTextureFile))
        {
            return;
        }

        m_pResTexture->Finalize(pDevice);

        pResTextureFile->Finalize(pDevice);
    }

private:

    //! @brief nn::gfx::ResTextureFile が初期化されているかどうかを取得します。
    //!
    //! @details
    //! nn::gfx::ResTextureFile::IsInitialized が用意されたら乗り換えます。
    //!
    //! @return nn::gfx::ResTextureFile が初期化されているかどうか
    //!
    static bool IsResTextureFileInitialized(const nn::gfx::ResTextureFile* pResTextureFile)
    {
        return pResTextureFile->ToData().textureContainerData.pCurrentMemoryPool.Get() != nullptr;
    }

    nn::gfx::ResTexture* m_pResTexture;
};

//---------------------------------------------------------------------------
//! @brief        文字グリフの情報を保持する構造体です。
//---------------------------------------------------------------------------
struct Glyph
{
    // リソースの CharWidths とはサイズが異なるので別定義します。
    struct GlyphCharWidths
    {
        int16_t       left;                       //!< 文字の左スペースの幅
        uint16_t      glyphWidth;                 //!< 文字のグリフ幅
        uint16_t      charWidth;                  //!< 文字の幅 = 左スペース幅 + グリフ幅 + 右スペース幅
        uint16_t      rawWidth;                   //!< テクスチャキャッシュ上の文字幅
    };

    const void* pTexture;                   //!< シートへのポインタ
    GlyphCharWidths   widths;                     //!< 文字幅情報
    uint16_t          height;                     //!< セルの高さ
    uint16_t          rawHeight;                  //!< テクスチャキャッシュ上の文字の高さ
    uint16_t          texWidth;                   //!< シートの幅
    uint16_t          texHeight;                  //!< シートの高さ
    uint16_t          cellX;                      //!< シート内でのセル左上隅のx座標
    uint16_t          cellY;                      //!< シート内でのセル左上隅のy座標
    TexFmt      texFormat;                  //!< シートのフォーマット
    uint8_t           isSheetUpdated;             //!< シートの内容が更新されている場合は真
    uint8_t           sheetIndex;                 //!< シート番号
    const TextureObject*
                pTextureObject;             //!< シートのテクスチャオブジェクトへのポインタ
    int16_t     baselineDifference;         //!< 基本となるベースラインからずれた文字の場合その差分が入ります
};

//---------------------------------------------------------------------------
//! @brief        フォント管理を行うクラスの基底クラスです。
//---------------------------------------------------------------------------
class Font
{
public:
    //! 実行時型情報です。
    NN_FONT_RUNTIME_TYPEINFO_ROOT();

    /* ------------------------------------------------------------------------
            型
       ------------------------------------------------------------------------ */
    enum Type
    {
        Type_Null,          //
        Type_Resource,      // ResFont
        Type_Scalable,      // ScalableFont
        Type_Pair           // PairFont
    };

    /* ------------------------------------------------------------------------
            定数
       ------------------------------------------------------------------------ */
    // 不正な文字コード
    static const uint32_t   InvalidCharacterCode = InvalidCharacterCodeValue;

   /* ------------------------------------------------------------------------
            関数
       ------------------------------------------------------------------------ */

    //! @name コンストラクタ/デストラクタ
    //@{

    //! コンストラクタです。
                            Font();

    //! デストラクタです。
    //!
    //! @details
    //! デストラクタが呼び出される前に、 Finalize() を実行する必要があります。
    //!
    virtual                 ~Font();

    //! @brief 終了処理を行います。
    //!
    //! @param[in] pDevice デバイス
    //!
    virtual void Finalize(nn::gfx::Device* pDevice)
    {
        NN_UNUSED(pDevice);
    }

    //@}


    //! @name フォント情報の取得
    //@{

    //! @brief      フォントの幅を取得します。
    //!
    //! @return     ピクセル単位でのフォントの幅を返します。
    //!
    virtual int             GetWidth() const = 0;

    //! @brief      フォントの高さを取得します。
    //!
    //! @return     ピクセル単位でのフォントの高さを返します。
    //!
    virtual int             GetHeight() const = 0;

    //! @brief      フォントのアセントを取得します。
    //!
    //! @return     ピクセル単位でのフォントのアセントを返します。
    //!
    virtual int             GetAscent() const = 0;

    //! @brief      フォントのディセントを取得します。
    //!
    //! @return     ピクセル単位でのフォントのディセントを返します。
    //!
    virtual int             GetDescent() const = 0;

    //! @brief      フォントに含まれる最大の文字幅を取得します。
    //!
    //! @return     フォントに含まれる最大の文字幅をピクセル単位で返します。
    //!
    virtual int             GetMaxCharWidth() const = 0;

    //! @brief      フォントのタイプを取得します。
    //!
    //! @return     フォントのタイプを返します。
    //!
    virtual Type            GetType() const = 0;

    //! @brief      フォントのテクスチャフォーマットを取得します。
    //!
    //! @return     グリフデータのテクスチャフォーマットを返します。
    //!
    virtual TexFmt          GetTextureFormat() const = 0;

    //! @brief      フォントが持つ改行幅を取得します。
    //!
    //! @return     フォントが持つ改行幅を返します。
    //!
    virtual int             GetLineFeed() const = 0;

    //! @brief      フォントのデフォルト文字幅情報を取得します。
    //!
    //! @return     フォントのデフォルト文字幅情報を返します。
    //!
    virtual const CharWidths
                            GetDefaultCharWidths() const = 0;

    //@}


    //! @name フォント情報の設定
    //@{

    //! @brief      フォントがもつ改行幅を設定します。
    //!
    //! @param[in]  linefeed  新しい改行幅。
    //!
    virtual void            SetLineFeed(int linefeed) = 0;

    //! @brief      フォントのデフォルト文字幅情報を設定します。
    //!
    //! @param[in]  widths  新しいデフォルト文字幅情報。
    //!
    virtual void            SetDefaultCharWidths(
                                const CharWidths& widths
                            ) = 0;

    //! @brief      フォントの代替文字を変更します。
    //!
    //! @param[in]  c  新しい代替文字の文字コード。
    //!
    //! @return     代替文字の変更に成功したら true 、失敗したら false を返します。@n
    //!             フォント内に文字コード c に対応するグリフが存在しない場合に失敗します。
    //!
    virtual bool            SetAlternateChar(uint32_t c) = 0;

    //@}


    //! @name 文字情報の取得
    //@{

    //! @brief      文字の文字幅を取得します。
    //! @details
    //! 引き数で指定した文字が未プロットのときに本関数を呼ぶと、文字の幅の値に 1px 程度の誤差が生じる場合があります。
    //!
    //! @param[in]  c  幅を求める文字の文字コード。
    //!
    //! @return     ピクセル単位での文字幅を返します。
    //!
    virtual int             GetCharWidth(uint32_t c) const = 0;

    //! @brief      文字の文字幅情報を取得します。
    //! @details
    //! 引き数で指定した文字が未プロットのときに本関数を呼ぶと、文字の幅の値に 1px 程度の誤差が生じる場合があります。
    //!
    //! @param[in]  c  文字幅情報を取得する文字の文字コード。
    //!
    //! @return         文字幅情報を返します。
    //!
    virtual const CharWidths
                            GetCharWidths(uint32_t c) const = 0;

    //! @brief      グリフデータを取得します。
    //! @details
    //! 引き数で指定した文字が未プロットのときに本関数を呼ぶと、文字の幅の値に 1px 程度の誤差が生じる場合があります。
    //!
    //! @param[out] pGlyph  グリフデータを受け取るバッファへのポインタ
    //! @param[in]  c       グリフデータを取得する文字の文字コード
    //!
    virtual void            GetGlyph(
                                Glyph*      pGlyph,
                                uint32_t    c
                            ) const = 0;

    //! @brief      グリフデータの有無を取得します。
    //!
    //! @param[in]  c  グリフデータの有無を取得する文字の文字コード
    //!
    //! @return     グリフデータが存在する場合は true 、存在しない場合は false を返します。
    //!
    virtual bool            HasGlyph(
                                uint32_t    c
                            ) const = 0;

    //! @brief      カーニングの値を取得します。
    //!
    //! @param[in]  c0  カーニングの値を取得する、進行方向と逆側の文字
    //! @param[in]  c1  カーニングの値を取得する、進行方向側の文字
    //!
    //! @return     カーニングの値を返します。
    //!
    virtual int            GetKerning(uint32_t c0, uint32_t c1) const = 0;

    //@}


    //! @name 文字列エンコーディング
    //@{

    //! @brief      フォントが対応している文字コードを取得します。
    //!
    //! @return     フォントが対応している文字コードを返します。
    //!
    virtual CharacterCode   GetCharacterCode() const = 0;

    //! @brief      フォントが対応している文字コード形式の文字列を解釈する
    //!             CharStrmReader を取得します。
    //!
    //! @param[in]  dummy CharStrmReaderを取得するCharWriterが扱う文字型。
    //!             引数の値そのものは使用しません。
    //!
    //! @return     フォントが対応している文字コード形式の
    //!             文字列を解釈する CharStrmReader を返します。
    //!
    const CharStrmReader    GetCharStrmReader(char dummy) const;

    //! @brief      フォントが対応している文字コード形式の文字列を解釈する
    //!             CharStrmReader を取得します。
    //!
    //! @param[in]  dummy CharStrmReaderを取得するCharWriterが扱う文字型。
    //!             引数の値そのものは使用しません。
    //!
    //! @return     フォントが対応している文字コード形式の
    //!             文字列を解釈する CharStrmReader を返します。
    //!
    const CharStrmReader    GetCharStrmReader(uint16_t dummy) const;

    //@}


    //! @name シート情報の取得
    //@{

    //! @brief      ベースライン位置を取得します。
    //!
    //! @return     セルの上端からベースラインまでのピクセル数を返します。
    //!
    virtual int             GetBaselinePos() const = 0;

    //! @brief      セルの高さを取得します。
    //!
    //! @return     ピクセル単位でのセルの高さを返します。
    //!
    virtual int             GetCellHeight() const = 0;

    //! @brief      セルの幅を取得します。
    //!
    //! @return     ピクセル単位でのセルの幅を返します。
    //!
    virtual int             GetCellWidth() const = 0;

    //@}

    //! @name テクスチャ補間
    //@{

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
                            ) = 0;

    //! @brief      文字が小さく表示される場合に線形補間が行われるかを取得します。
    //!
    //! @return     文字がオリジナルサイズより小さく表示される場合に線形補間が行われるのであれば
    //!             true を、行われないのであれば false を返します。
    //!
    virtual bool            IsLinearFilterEnabledAtSmall() const = 0;

    //! @brief      文字が大きく表示される場合に線形補間が行われるかを取得します。
    //!
    //! @return     文字がオリジナルサイズより大きく表示される場合に線形補間が行われるのであれば
    //!             true を、行われないのであれば false を返します。
    //!
    virtual bool            IsLinearFilterEnabledAtLarge() const = 0;

    //! @brief      テクスチャのラップ、フィルタ設定値を取得します。
    //!
    //! @return     テクスチャのラップ、フィルタ設定値を返します。
    //!
    virtual uint32_t              GetTextureWrapFilterValue() const = 0;

    //@}

    //! @name 表示設定
    //@{

    //! @brief        カラーの白黒補間の影響の有無を取得します。
    //!
    //! @return       カラーの白黒補間の影響を受ける場合は、true を返します。
    //!
    virtual bool            IsColorBlackWhiteInterpolationEnabled() const = 0;

    //! @brief        カラーの白黒補間の影響の有無を設定します。
    //!
    //! @details
    //! この設定でfalseを設定しても、アルファについては影響を受けます。
    //!
    //! @param[in]  flag    true を設定すると、白黒補間の影響を受けます。
    //!
    virtual void            SetColorBlackWhiteInterpolationEnabled(bool flag) = 0;

    //! @brief        特殊フチ形式が設定可能かを取得します。
    //!
    //! @return       特殊フチ形式が設定可能な場合は true を返します。
    //!
    virtual bool            IsBorderAvailable() const
    {
        return false;
    }

    //! @brief        縁取り効果の有無を取得します。
    //!
    //! @return       縁取り効果が有効な場合は、true を返します。
    //!
    virtual bool            IsBorderEffectEnabled() const = 0;

    //@}

    //----------------------------------------------------
    //! @name カーニングの設定
    //@{

    //! @brief        カーニングを有効か否かを取得します。
    //!
    //! @details
    //! 有効な場合も、スケーラブルフォント自体がカーニングの情報を持っていない場合は
    //! カーニングが行われないことにご注意ください。
    //!
    //! @return 有効か否か
    //!
    bool                    IsKerningEnabled() const
    {
        return m_IsKerningEnabled;
    }

    //! @brief        カーニングを有効か否かを設定します。
    //!
    //! @details
    //! 初期状態では有効です。
    //!
    //! @param[in]    enabled true を設定すると、カーニングを有効にします。
    //!
    void                    SetKerningEnabled(bool enabled)
    {
        m_IsKerningEnabled = enabled;
    }
    //@}

    //----------------------------------------------------
    //! @name 拡張マージンの設定
    //@{

    //! @brief        拡張マージンを有効か否かを取得します。
    //!
    //! @details
    //! 有効の場合、フォントの周りに0.5ピクセル分のマージンを追加して
    //! 文字欠けをおこりにくくします。
    //!
    //! @return 有効か否か
    //!
    bool                    IsExtraMarginEnabled() const
    {
        return m_IsExtraMarginEnabled;
    }

    //! @brief        拡張マージンを有効か否かを設定します。
    //!
    //! @details
    //! 初期状態では有効です。
    //!
    //! @param[in]    enabled true を設定すると、拡張マージンを有効にします。
    //!
    void                    SetExtraMarginEnabled(bool enabled)
    {
        m_IsExtraMarginEnabled = enabled;
    }

private:

    bool m_IsKerningEnabled;
    bool m_IsExtraMarginEnabled;
};

}   // namespace font
}   // namespace nn
