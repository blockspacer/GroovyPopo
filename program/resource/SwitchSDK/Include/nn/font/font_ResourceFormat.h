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

#include <nn/util/util_FormatString.h>
#include <nn/nn_SdkLog.h>
#include <nn/util/util_BytePtr.h>
#include <nn/font/detail/font_Log.h>

#ifdef _MSC_VER // for VC
    #pragma warning( push )
    #pragma warning( disable: 4200 )
    #pragma warning( disable: 4201 )
#endif  //_MSC_VER

namespace nn {
namespace font {

const int   GlyphDataAlignment   = 128;

namespace detail
{
    const uint16_t ByteOrderMark = 0xFEFF;

#if ( NN_BUILD_CONFIG_ENDIAN_LITTLE )
    // バイナリリソースは、ビックエンディアンとして出力されているので、Win32 ではスワップ処理を行います。
    //---- little endian
#define NN_DETAIL_FONT_MAKE_SIGWORD(a,b,c,d)        \
    static_cast<uint32_t >(          \
    (static_cast<uint8_t>(a) <<  0)        \
    | (static_cast<uint8_t>(b) <<  8)        \
    | (static_cast<uint8_t>(c) << 16)        \
    | (static_cast<uint8_t>(d) << 24)        \
    )

#else

    //---- big endian
#define NN_DETAIL_FONT_MAKE_SIGWORD(a,b,c,d)        \
    static_cast<uint32_t >(          \
    (static_cast<uint8_t>(a) << 24)        \
    | (static_cast<uint8_t>(b) << 16)        \
    | (static_cast<uint8_t>(c) <<  8)        \
    | (static_cast<uint8_t>(d) <<  0)        \
    )
#endif

#define NN_DETAIL_FONT_MAKE_VERSION(major, minor, micro, binaryBugFix)            \
    static_cast<uint32_t>(                                                    \
    (static_cast<uint8_t>(major) << 24) | (static_cast<uint8_t>(minor) << 16)  \
    | (static_cast<uint8_t>(micro) <<  8) | (static_cast<uint8_t>(binaryBugFix)) \
    )

#define NN_DETAIL_VERSION_MAJOR(version)        (((version) >> 24) & 0xff)
#define NN_DETAIL_VERSION_MINOR(version)        (((version) >> 16) & 0xff)
#define NN_DETAIL_VERSION_MICRO(version)        (((version) >>  8) & 0xff)
#define NN_DETAIL_VERSION_BINARYBUGFIX(version) (((version) >>  0) & 0xff)

    //---------------------------------------------------------------------------
    //! @brief        エンディアンスワップユーティリティ用の名前空間
    //---------------------------------------------------------------------------
    typedef union
    {
        uint64_t UInt64;
        int64_t SInt64;
        double Float64;
    } Type64;

    typedef union
    {
        uint32_t UInt32;
        int32_t SInt32;
        float Float32;
    } Type32;

    typedef union
    {
        uint16_t UInt16;
        int16_t SInt16;
    } Type16;

    //---------------------------------------------------------------------------
    inline uint64_t ByteSwap( uint64_t val )
    {
        const uint64_t MASK  = 0xFF00FF00FF00FF00ULL;
        const uint64_t MASK2 = 0xFFFF0000FFFF0000ULL;
        val = ( (val & MASK) >> 8 ) | ( (val << 8) & MASK );
        val = ( (val & MASK2) >> 16 ) | ( (val << 16) & MASK2 );
        return (val >> 32) | (val << 32);
    }

    //---------------------------------------------------------------------------
    inline int64_t ByteSwap( int64_t val )
    {
        Type64 data;
        data.SInt64 = val;
        data.UInt64 = ByteSwap( data.UInt64 );
        return data.SInt64;
    }

    //---------------------------------------------------------------------------
    inline double ByteSwap( double val )
    {
        Type64 data;
        data.Float64 = val;
        data.UInt64 = ByteSwap( data.UInt64 );
        return data.Float64;
    }

    //---------------------------------------------------------------------------
    inline uint32_t ByteSwap( uint32_t val )
    {
        const uint32_t MASK = 0xFF00FF00;
        val = ( (val & MASK) >> 8 ) | ( (val << 8) & MASK );
        return (val >> 16) | (val << 16);
    }

    //---------------------------------------------------------------------------
    inline int32_t ByteSwap( int32_t val )
    {
        Type32 data;
        data.SInt32 = val;
        data.UInt32 = ByteSwap( data.UInt32 );
        return data.SInt32;
    }

    //---------------------------------------------------------------------------
    inline float ByteSwap( float val )
    {
        Type32 data;
        data.Float32 = val;
        data.UInt32 = ByteSwap( data.UInt32 );
        return data.Float32;
    }

    //---------------------------------------------------------------------------
    inline uint16_t ByteSwap( uint16_t val )
    {
        return (uint16_t)( (val >> 8) | (val << 8) );
    }

    //---------------------------------------------------------------------------
    inline int16_t ByteSwap( int16_t val )
    {
        return (int16_t)( ((uint16_t)val >> 8) | ((uint16_t)val << 8) );
    }

    //---------------------------------------------------------------------------
    struct BinaryFileHeader
    {
        uint32_t signature;                  // ファイルシグネチャ   : 4 Byte
        uint16_t  byteOrder;                  // バイトオーダーマーク : 2 Byte
        uint16_t  headerSize;                 // ヘッダサイズ         : 2 Byte
        uint32_t  version;                    // ファイルバージョン   : 4 Byte
        uint32_t  fileSize;                   // ファイルサイズ       : 4 Byte
        uint16_t  dataBlocks;                 // ブロック数           : 2 Byte
        uint16_t  reserved;                   // 予約                 : 2 Byte
    };

    //---------------------------------------------------------------------------
    struct BinaryBlockHeader
    {
        uint32_t kind;                       // ブロック種別名
        uint32_t  size;                       // ブロック全体サイズ
    };

    //---------------------------------------------------------------------------
    inline bool IsReverseEndianBinaryFile( const BinaryFileHeader* pFileHeader )
    {
        NN_SDK_ASSERT_NOT_NULL( pFileHeader );
        return ( pFileHeader->byteOrder != detail::ByteOrderMark );
    }

    //---------------------------------------------------------------------------
    template <typename OutputT>
    inline OutputT
        BitExtract(OutputT bits, int pos, int len)
    {
        const OutputT mask = static_cast<OutputT>( ( 1UL << len ) - 1 );
        return static_cast<OutputT>( (bits >> pos) & mask );
    }

    //---------------------------------------------------------------------------
    template <typename OutputT>
    inline OutputT
        BitExtract(OutputT bits, int pos)
    {
        return BitExtract<OutputT>(bits, pos, 1);
    }

    //---------------------------------------------------------------------------
    inline uint32_t
        ReverseEndian( uint32_t x )
    {
        return static_cast<uint32_t>(
            BitExtract( x,  0, 8 ) << 24 |
            BitExtract( x,  8, 8 ) << 16 |
            BitExtract( x, 16, 8 ) <<  8 |
            BitExtract( x, 24, 8 ) <<  0
            );
    }

    //---------------------------------------------------------------------------
    inline uint16_t ReverseEndian( uint16_t x )
    {
        return static_cast<uint16_t>(
            BitExtract( x,  0, 8 ) << 8 |
            BitExtract( x,  8, 8 ) << 0
            );
    }

    //---------------------------------------------------------------------------
    inline bool IsValidBinaryFile(
        const BinaryFileHeader* pHeader,
        uint32_t                     signature,
        uint32_t                     version,
        uint16_t                     minBlocks
        )
    {
        NN_SDK_ASSERT_NOT_NULL( pHeader );

        if ( pHeader->signature != signature )
        {
            // IntelliSense がマクロの展開に失敗することがあるのでメッセージを文字列バッファに作成しておく
            char message[256];
            nn::util::SNPrintf(message, 256, "Signature check failed ('%c%c%c%c' must be '%c%c%c%c').",
                static_cast<char>(BitExtract(pHeader->signature, 24, 8)),
                static_cast<char>(BitExtract(pHeader->signature, 16, 8)),
                static_cast<char>(BitExtract(pHeader->signature,  8, 8)),
                static_cast<char>(BitExtract(pHeader->signature,  0, 8)),
                static_cast<char>(BitExtract(signature, 24, 8)),
                static_cast<char>(BitExtract(signature, 16, 8)),
                static_cast<char>(BitExtract(signature,  8, 8)),
                static_cast<char>(BitExtract(signature,  0, 8))
            );
            NN_SDK_ASSERT(false, "%s", message);
            NN_UNUSED(message);
            return false;
        }

        if ( pHeader->byteOrder != ByteOrderMark )
        {
            NN_SDK_ASSERT(false, "Unsupported byte order.");
            return false;
        }

        if ( NN_DETAIL_VERSION_MAJOR(version) != NN_DETAIL_VERSION_MAJOR(pHeader->version) ||
            NN_DETAIL_VERSION_MINOR(version) < NN_DETAIL_VERSION_MINOR(pHeader->version)  ||
            NN_DETAIL_VERSION_BINARYBUGFIX(version) > NN_DETAIL_VERSION_BINARYBUGFIX(pHeader->version) )
        {
            NN_SDK_ASSERT(false, "Version check failed (bin:'%d.%d.%d.%d', lib:'%d.%d.%d.%d').",
                NN_DETAIL_VERSION_MAJOR(pHeader->version),
                NN_DETAIL_VERSION_MINOR(pHeader->version),
                NN_DETAIL_VERSION_MICRO(pHeader->version),
                NN_DETAIL_VERSION_BINARYBUGFIX(pHeader->version),
                NN_DETAIL_VERSION_MAJOR(version),
                NN_DETAIL_VERSION_MINOR(version),
                NN_DETAIL_VERSION_MICRO(version),
                NN_DETAIL_VERSION_BINARYBUGFIX(version)
                );
            return false;
        }

        if( pHeader->fileSize < sizeof(BinaryFileHeader) + sizeof(BinaryBlockHeader) * minBlocks )
        {
            NN_SDK_ASSERT(false, "Too small file size(=%d).", static_cast<int>(pHeader->fileSize));
            return false;
        }

        if( pHeader->dataBlocks < minBlocks )
        {
            NN_SDK_ASSERT(false, "Too small number of data blocks(=%d).", static_cast<int>(pHeader->dataBlocks));
            return false;
        }

        return true;
    }

    //---------------------------------------------------------------------------
    inline bool IsValidBinaryFile(
        const BinaryFileHeader* pHeader,
        uint32_t                     signature,
        uint32_t                     version
    )
    {
        return IsValidBinaryFile(pHeader, signature, version, 1);
    }

    //---------------------------------------------------------------------------
    inline BinaryBlockHeader*
        GetNextBinaryBlockHeader(
        const BinaryFileHeader*       pFileHeader,
        const BinaryBlockHeader*      pBlockHeader
        )
    {
        NN_SDK_ASSERT_NOT_NULL( pFileHeader );

        const void* pPtr;
        if ( ! IsReverseEndianBinaryFile( pFileHeader ) )
        {
            if ( pBlockHeader == NULL )
            {
                if ( pFileHeader->dataBlocks == 0 ) return NULL;
                pPtr = nn::util::ConstBytePtr(pFileHeader).Advance(pFileHeader->headerSize).Get();
            }
            else
            {
                pPtr = nn::util::ConstBytePtr(pBlockHeader).Advance(pBlockHeader->size ).Get();
            }

            if ( pPtr >= nn::util::ConstBytePtr(pFileHeader).Advance(pFileHeader->fileSize).Get() )
            {
                return NULL;
            }
        }
        else
        {
            if ( pBlockHeader == NULL )
            {
                if ( pFileHeader->dataBlocks == 0 ) return NULL;
                pPtr = nn::util::ConstBytePtr(pFileHeader).Advance(ReverseEndian(pFileHeader->headerSize)).Get();
            }
            else
            {
                pPtr = nn::util::ConstBytePtr(pBlockHeader).Advance(ReverseEndian(pBlockHeader->size)).Get();
            }

            if ( pPtr >= nn::util::ConstBytePtr(pFileHeader).Advance(ReverseEndian(pFileHeader->fileSize)).Get() )
            {
                return NULL;
            }
        }

        return reinterpret_cast<BinaryBlockHeader*>( const_cast<void*>(pPtr) );
    }
}

/* ------------------------------------------------------------------------
        シグネチャ定義
   ------------------------------------------------------------------------ */

// 通常フォントリソースファイルシグネチャ
const uint32_t BinFileSignatureFont              = NN_DETAIL_FONT_MAKE_SIGWORD('F','F','N','T');

// オフセット解決済みバイナリファイルシグネチャ
const uint32_t BinFileSignatureFontResoleved    = NN_DETAIL_FONT_MAKE_SIGWORD('F','F','N','U');

// アーカイブフォントリソースファイルシグネチャ
const uint32_t BinFileSignatureFonta             = NN_DETAIL_FONT_MAKE_SIGWORD('F','F','N','A');

// 複合フォントリソースファイルシグネチャ
const uint32_t BinFileSignatureFcpx              = NN_DETAIL_FONT_MAKE_SIGWORD('F','C','P','X');

// バイナリブロックシグネチャ
const uint32_t BinBlockSignatureFinf             = NN_DETAIL_FONT_MAKE_SIGWORD('F','I','N','F');
const uint32_t BinBlockSignatureCglp             = NN_DETAIL_FONT_MAKE_SIGWORD('C','G','L','P');
const uint32_t BinBlockSignatureTglp             = NN_DETAIL_FONT_MAKE_SIGWORD('T','G','L','P');
const uint32_t BinBlockSignatureCwdh             = NN_DETAIL_FONT_MAKE_SIGWORD('C','W','D','H');
const uint32_t BinBlockSignatureCmap             = NN_DETAIL_FONT_MAKE_SIGWORD('C','M','A','P');
const uint32_t BinBlockSignatureKrng             = NN_DETAIL_FONT_MAKE_SIGWORD('K','R','N','G');

const uint32_t BinBlockSignatureGlgr             = NN_DETAIL_FONT_MAKE_SIGWORD('G','L','G','R');
const uint32_t BinBlockSignatureHtgl             = NN_DETAIL_FONT_MAKE_SIGWORD('H','T','G','L');





/* ------------------------------------------------------------------------
        リリースバージョン定義
   ------------------------------------------------------------------------ */

const uint32_t  FontFileVersion         = NN_DETAIL_FONT_MAKE_VERSION(4, 1, 0, 0);



/* ------------------------------------------------------------------------
        定数定義
   ------------------------------------------------------------------------ */
const uint32_t  InvalidCharacterCodeValue = 0xFFFF;
const uint16_t  InvalidGlyphIndex = 0xFFFF;





/* ------------------------------------------------------------------------
        enum定義
   ------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*
  Name:         FontType

  Description:  フォントタイプ
 *---------------------------------------------------------------------------*/
enum FontType
{
    FontType_FontTypeGlyph,            // BMP
    FontType_FontTypeTexture,          // Texture
    FontType_FontTypePackedTexture,   // 1つのチャンネルに複数のチャンネルを詰めた形式のテクスチャ
    FontType_MaxFontType
};

//---------------------------------------------------------------------------
//! @brief        文字コード
//---------------------------------------------------------------------------
enum CharacterCode
{
    CharacterCode_Unicode = 1,  //!< Unicode
    CharacterCode_Sjis,         //!< ShiftJIS
    CharacterCode_Cp1252,       //!< CP1252

    CharacterCode_MaxCharacterCode
};

/*---------------------------------------------------------------------------*
  Name:         FontMapMethod

  Description:  文字コードマッピングタイプ
 *---------------------------------------------------------------------------*/
enum FontMapMethod
{
    FontMapMethod_Direct,      // インデックス = 文字コード - オフセット
    FontMapMethod_Table,       // インデックス = mapInfo[文字コード - オフセット]
    FontMapMethod_Scan,        // インデックス = search(mapInfo, 文字コード)
    FontMapMethod_MaxFontMapMethod
};


/*---------------------------------------------------------------------------*
  Name:         FontSheetFormat

  Description:  シートの形式 (テクスチャフォーマット)
 *---------------------------------------------------------------------------*/
enum FontSheetFormat
{
    FontSheetFormat_Rgba8,
    FontSheetFormat_Rgb8,
    FontSheetFormat_Rgb5a1,
    FontSheetFormat_Rgb565,
    FontSheetFormat_Rgba4,
    FontSheetFormat_La8,
    FontSheetFormat_La4,
    FontSheetFormat_A4,
    FontSheetFormat_A8 = 8,

    FontSheetFormat_Bc1,
    FontSheetFormat_Bc2,
    FontSheetFormat_Bc3,
    FontSheetFormat_Bc4 = 12,
    FontSheetFormat_Bc5,
    FontSheetFormat_Rgba8_Srgb,
    FontSheetFormat_Bc1_Srgb,
    FontSheetFormat_Bc2_Srgb,
    FontSheetFormat_Bc3_Srgb,
    FontSheetFormat_Bc7,
    FontSheetFormat_Bc7_Srgb,
    FontSheetFormat_MaxFontSheetFormat,

    FontSheetFormat_Mask             = 0x3FFF,
    FontSheetFormat_LinearFlag       = 0x4000,   // 1 ならリニア形式なので、タイリング解除の処理は不要
    FontSheetFormat_CompressedFlag   = 0x8000    // 1 なら圧縮されている(現在は使われていません)
};




/* ------------------------------------------------------------------------
        構造体定義
   ------------------------------------------------------------------------ */

//---------------------------------------------------------------------------
//! @brief        文字の各種幅を保持する構造体です。
//---------------------------------------------------------------------------
struct CharWidths // 移行のクラスは、nn::font::detail::Res の利用を廃止してPOD になる予定なので、対応しません。
{
    int8_t  left;                   //!< 文字の左スペースの幅
    uint8_t  glyphWidth;             //!< 文字のグリフ幅
    uint8_t  charWidth;              //!< 文字の幅 = 左スペース幅 + グリフ幅 + 右スペース幅
};
NN_STATIC_ASSERT(sizeof(CharWidths) == 3);

/*---------------------------------------------------------------------------*
  Name:         CMapScanEntry

  Description:  文字コードとグリフインデックスペア
 *---------------------------------------------------------------------------*/
struct CMapScanEntry
{
    uint32_t code;                   // 文字コード
    uint16_t index;                  // グリフインデックス
    NN_PADDING2;
};
NN_STATIC_ASSERT(sizeof(CMapScanEntry) == 8);

/*---------------------------------------------------------------------------*
  Name:         CMapInfoScan

  Description:  MAPMETHOD_SCAN の場合の FontCodeMap.mapInfo
 *---------------------------------------------------------------------------*/
struct CMapInfoScan
{
    uint16_t count;             // entries の要素数
    NN_PADDING2;
    CMapScanEntry entries[1];    // 文字コードからグリフインデックスへのマッピングリスト
};
NN_STATIC_ASSERT(sizeof(CMapInfoScan) == 12);

//---------------------------------------------------------------------------
//! @brief      一つ目の文字から二つ目のテーブルへのオフセットを
//!             引くためのテーブルの要素です。
//---------------------------------------------------------------------------
struct KerningFirstTableElem
{
    uint32_t firstWord;       // 一つ目の文字
    uint32_t offset;          // 二つ目のテーブルへのオフセット。値はFontKerningTableの先頭アドレスからのオフセットを2で割ったものとなります。
};
NN_STATIC_ASSERT(sizeof(KerningFirstTableElem) == 8);

//---------------------------------------------------------------------------
//! @brief      二つ目のテーブルの要素を表します。
//---------------------------------------------------------------------------
struct KerningSecondTableElem
{
    uint32_t secondWord;      // 二つ目の文字
    int16_t kerningValue;    // カーニングの値
    NN_PADDING2;
};
NN_STATIC_ASSERT(sizeof(KerningSecondTableElem) == 8);

//---------------------------------------------------------------------------
//! @brief      二つ目のテーブルを表します。
//---------------------------------------------------------------------------
struct KerningSecondTable
{
    uint16_t secondWordCount;           // このテーブルに含まれる二つ目の文字の種類
    NN_PADDING2;
    KerningSecondTableElem  elems[1];   // 二分探索するための要素の配列。実際にはsecondWordCount個あります。
};
NN_STATIC_ASSERT(sizeof(KerningSecondTable) == 12);

/* ------------------------------------------------------------------------
        ブロック本体定義
   ------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*
  Name:         FontGlyphGroups

  Description:  フォントを部分ロードするための情報を格納します。
 *---------------------------------------------------------------------------*/
struct FontGlyphGroups
{
    uint32_t sheetSize;
    uint16_t glyphsPerSheet;
    uint16_t setCount;
    uint16_t sheetCount;
    uint16_t cwdhCount;
    uint16_t cmapCount;

    uint16_t nameOffsets[1];  // setCount
/*
    以下は可変長データが続きます

    (4 byte align)

    uint32_t  sizeSheets[sheetCount];       // 圧縮済みシートのサイズ
    uint32_t  sizeCWDH[cwdhCount];          // CWDH ブロックのサイズ
    uint32_t  sizeCMAP[cmapCount];          // CMAP ブロックのサイズ
    uint32_t  useSheets[setCount][sheetCount/32];
    uint32_t  useCWDH[setCount][cwdhCount/32];
    uint32_t  useCMAP[setCount][cmapCount/32];

    char names[setCount][name length];
*/
};
NN_STATIC_ASSERT(sizeof(FontGlyphGroups) == 16);


/*---------------------------------------------------------------------------*
  Name:         FontTextureGlyph

  Description:  フォントのグリフテクスチャを格納します。
 *---------------------------------------------------------------------------*/
struct FontTextureGlyph
{
    uint8_t  cellWidth;              // セル幅=最大グリフ幅
    uint8_t  cellHeight;             // セル高さ
    uint8_t  sheetCount;               // シート数
    uint8_t  maxCharWidth;           // 最大文字幅
    uint32_t sheetSize;              // テクスチャシートデータサイズ(byte単位)
    int16_t baselinePos;           // ベースライン位置
    uint16_t sheetFormat;            // FontSheetFormat
    uint16_t sheetRow;               // シート内の横方向セル数
    uint16_t sheetLine;              // シート内の縦方向セル数
    uint16_t sheetWidth;             // シート幅(ピクセル単位)
    uint16_t sheetHeight;            // シート高さ(ピクセル単位)
    uint32_t                 sheetImage;             // テクスチャデータへのオフセット(ファイル先頭起点)
};
NN_STATIC_ASSERT(sizeof(FontTextureGlyph) == 24);

/*---------------------------------------------------------------------------*
  Name:         FontWidth

  Description:  各文字の文字幅情報を格納します。
 *---------------------------------------------------------------------------*/
struct FontWidth
{
    uint16_t          indexBegin;     // widthTable の最初のエントリが対応するグリフインデックス
    uint16_t          indexEnd;       // widthTable の最後のエントリが対応するグリフインデックス
    uint32_t            pNext;          // 次の FontWidth へのオフセット(ファイル先頭起点)
    CharWidths          widthTable[1];   // 幅情報の配列
};
NN_STATIC_ASSERT(sizeof(FontWidth) == 12);

/*---------------------------------------------------------------------------*
  Name:         FontCodeMap

  Description:  文字コードから、文字コードに対応するグリフの
                グリフイメージ配列中のインデックスへのマッピングを規定します。
 *---------------------------------------------------------------------------*/
struct FontCodeMap
{
    uint32_t          codeBegin;     // このブロックが担当する文字コード範囲の最初の文字コード
    uint32_t          codeEnd;       // このブロックが担当する文字コード範囲の最後の文字コード
    uint16_t          mappingMethod;  // マッピング方法 (FontMappingMethod型)
    uint16_t          reserved;       // 予約
    uint32_t                          pNext;          // 次の FontCodeMap へのオフセット(ファイル先頭起点)
    uint16_t          mapInfo[1];      // 文字コードマッピング情報 具体的な内容は mappingMethod に依る
};
NN_STATIC_ASSERT(sizeof(FontCodeMap) == 20);

/*---------------------------------------------------------------------------*
  Name:         FontKerningTable

  Description:  カーニングの値を格納します。
 *---------------------------------------------------------------------------*/
struct FontKerningTable
{
    uint16_t          firstWordCount;       // 最初の文字が何パターンあるか
    NN_PADDING2;
    KerningFirstTableElem   firstTable[1];  // 二分探索するための要素の配列。実際にはfirstWordCount個あります。
/*
    以下はサイズ可変のデータが続きます。

    (2 byte align)

    KerningSecondTable      secondTables[firstWordCount];
*/
};
NN_STATIC_ASSERT(sizeof(FontKerningTable) == 12);

/*---------------------------------------------------------------------------*
  Name:         FontInformation

  Description:  フォント全体に渡る情報を格納します。
 *---------------------------------------------------------------------------*/
struct FontInformation
{
    uint8_t           fontType;       // グリフデータタイプ (FontType型)
    uint8_t           height;         // フォントの高さ
    uint8_t           width;          // フォントの幅
    uint8_t           ascent;         // アセント
    int16_t          linefeed;       // (*)== leading
    uint16_t          alterCharIndex; // (*)フォントに含まれない文字用のグリフのグリフインデックス
    CharWidths                        defaultWidth;   // (*)文字幅情報を持たないグリフ用の文字幅情報
    uint8_t           characterCode;  // 対応する文字コード (CharacterCode型)
    uint32_t                          pGlyph;         // 唯一の FontGlyph へのオフセット(ファイル先頭起点)
    uint32_t                          pWidth;         // 最初の FontWidth へのオフセット(ファイル先頭起点)
    uint32_t                          pMap;           // 最初の FontCodeMap へのオフセット(ファイル先頭起点)
};
NN_STATIC_ASSERT(sizeof(FontInformation) == 24);

//--------------------------------------------------------------------------------
// 複合フォント関連
//--------------------------------------------------------------------------------

enum ResSubFontType
{
    ResSubFontType_BitmapFont,
    ResSubFontType_MultiScalableFont,
    ResSubFontType_PairFont,
    ResSubFontType_MaxResSubFontType
};

struct ResCharCodeRange
{
    uint32_t first;
    uint32_t last;
};
NN_STATIC_ASSERT(sizeof(ResCharCodeRange) == 8);

struct ResCharCodeRangeSet
{
    int32_t  charCodeRangeCount;
    uint32_t charCodeRange;
    //ResCharCodeRange charCodeRange[];
};
NN_STATIC_ASSERT(sizeof(ResCharCodeRangeSet) == 8);

struct ResBitmapFont
{
    uint32_t type; // ResSubFontType
    uint32_t fontName; // フォント名へのオフセット値
    ResCharCodeRangeSet charCodeRangeSet;
};
NN_STATIC_ASSERT(sizeof(ResBitmapFont) == 16);

struct ResScalableFontDescription
{
    float boldWeight;
    float scaleWidth;
    uint32_t ttcFontIndex;
    uint32_t fontName; // フォント名へのオフセット値
    uint8_t borderWidth;

    struct Bits
    {
        bool ignorePalt : 1;
        bool deleteBearingX : 1;
        bool forceMonospacedEnabled : 1;
    };
    NN_STATIC_ASSERT(sizeof(Bits) == 1);
    Bits bits;

    int16_t baselineOffset;
    float scaleHeight;
    int16_t bearingOffsetX;
    int16_t forceMonospacedWidth;
    uint16_t overwrittenMetricsAscent;
    uint16_t overwrittenMetricsDescent;
    ResCharCodeRangeSet charCodeRangeSet;
};
NN_STATIC_ASSERT(sizeof(ResScalableFontDescription) == 40);

// NintendoSDK 0.12.x での古い定義
struct ResScalableFontDescriptionOld
{
    float boldWeight;
    uint32_t ttcFontIndex;
    uint32_t fontName; // フォント名へのオフセット値
    uint8_t borderWidth;
    NN_PADDING3;
    ResCharCodeRangeSet charCodeRangeSet;
};
NN_STATIC_ASSERT(sizeof(ResScalableFontDescriptionOld) == 24);

struct ResMultiScalableFont
{
    uint32_t type; // ResSubFontType
    float size;
    int lineFeedOffset;
    uint32_t scalableFontDescriptionCount;
    uint32_t scalableFontDescription;
    uint16_t alternateChar;
    NN_PADDING2; // 将来の拡張のために予約済み
    NN_PADDING8; // 将来の拡張のために予約済み
    //ResScalableFontDescription scalableFontDescription[0];
};
NN_STATIC_ASSERT(sizeof(ResMultiScalableFont) == 32);

// NintendoSDK 0.12.x での古い定義
struct ResMultiScalableFontOld
{
    uint32_t type; // ResSubFontType
    float size;
    uint32_t scalableFontDescriptionCount;
    uint32_t scalableFontDescription;
    //ResScalableFontDescription scalableFontDescription[0];
};
NN_STATIC_ASSERT(sizeof(ResMultiScalableFontOld) == 16);

struct ResPairFont
{
    uint32_t type; // ResSubFontType
    uint32_t firstFont;  // PairFont か MultiScalableFont か BitmapFont へのオフセット値
    uint32_t secondFont; // PairFont か MultiScalableFont か BitmapFont へのオフセット値
};
NN_STATIC_ASSERT(sizeof(ResPairFont) == 12);

struct ResComplexFontDescription
{
    uint32_t subFont; // PairFont か MultiScalableFont か BitmapFont へのオフセット値
};
NN_STATIC_ASSERT(sizeof(ResComplexFontDescription) == 4);

/* ------------------------------------------------------------------------
        ブロック定義
   ------------------------------------------------------------------------ */

/*---------------------------------------------------------------------------*
  Name:         BinaryBlock

  Description:  NintendoWare 標準バイナリブロック
 *---------------------------------------------------------------------------*/
template <typename BlockBodyType>
struct BinaryBlock
{
    nn::font::detail::BinaryBlockHeader   header;     // ブロックヘッダ
    BlockBodyType           body;       // ブロック本体
};

typedef BinaryBlock<FontGlyphGroups>    FontGlyphGroupsBlock;
typedef BinaryBlock<FontInformation>    FontInformationBlock;
typedef BinaryBlock<FontTextureGlyph>   FontTextureGlyphBlock;
typedef BinaryBlock<FontWidth>          FontWidthBlock;
typedef BinaryBlock<FontCodeMap>        FontCodeMapBlock;



} // namespace font
} // namespace nn


#ifdef _MSC_VER
    #pragma warning( pop )
#endif  //_MSC_VER
