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

#include <nn/nn_Common.h>
#include <nn/util/util_BinTypes.h>
#include <nn/util/util_StringView.h>

namespace nn { namespace util {

//! @brief バイトオーダーマークを示す列挙体です。
enum ByteOrderMark
{
    ByteOrderMark_Normal = 0xFEFF, //!< 通常のバイトオーダーマークです。
    ByteOrderMark_Reverse = 0xFFFE //!< エンディアン反転したバイトオーダーマークです。
};

struct BinaryBlockHeader;
struct RelocationTable;

//! @brief  読み取り専用の機能です。構築しないでください。@n
//!         バイナリリソースファイルのヘッダです。
struct BinaryFileHeader
{
    //! @brief ファイルの種類を表す 8 つの 1 バイト値です。
    BinFileSignature signature;

    //! @brief バージョンを表す 4 つの 1 バイト値です。
    BinVersion version;

    uint16_t _byteOrderMark;        // エンディアンを識別するバイトオーダーマークです。
    uint8_t _alignmentShift;        // ファイルを配置する際に揃える必要があるアライメントの 2 を底とする指数部です。
    uint8_t _targetAddressSize;     // リソースのアドレスサイズです。
    uint32_t _offsetToFileName;     // ファイルの先頭からファイル名へのオフセットです。
    uint16_t _flag;                 // フラグです。
    uint16_t _offsetToFirstBlock;   // ファイルの先頭から最初のデータブロックへのオフセットです。
    uint32_t _offsetToRelTable;     // ファイルの先頭からリロケーションテーブルへのオフセットです。
    uint32_t _fileSize;             // ファイルサイズです。

public:
    /*!
        @brief  シグネチャ、バージョン、エンディアン、アライメントが有効かどうかを取得します。

        @param[in]  packedSignature   ファイルヘッダに指定されるべきシグネチャに相当する数値です。
        @param[in]  majorVersion       メジャーバージョンです。
        @param[in]  minorVersion       マイナーバージョンです。
        @param[in]  microVersion       マイクロバージョンです。

        @return 全ての入力に対し、有効であれば true を返します。
    */
    bool IsValid( int64_t packedSignature, int majorVersion, int minorVersion, int microVersion) const NN_NOEXCEPT;

    //! @brief      シグネチャが有効かどうかを取得します。
    //! @param[in]  packedSignature   ファイルヘッダに指定されるシグネチャに相当する数値です。
    //! @return     シグネチャが等しければ true を返します。
    bool IsSignatureValid( int64_t packedSignature ) const NN_NOEXCEPT;

    /*!
        @brief  バージョンが有効かどうかを取得します。

        @param[in]  majorVersion       メジャーバージョンです。
        @param[in]  minorVersion       マイナーバージョンです。
        @param[in]  microVersion       マイクロバージョンです。

        @return バージョンが有効であれば true を返します。
    */
    bool IsVersionValid( int majorVersion, int minorVersion, int microVersion) const NN_NOEXCEPT;

    //! @brief  エンディアンが有効かどうかを取得します。
    //! @return エンディアンが有効であれば true を返します。
    bool IsEndianValid() const NN_NOEXCEPT;

    //! @brief  エンディアンが有効なエンディアンの反転かどうかを取得します。
    //! @return エンディアンが有効なエンディアンを反転したものであれば true を返します。
    bool IsEndianReverse() const NN_NOEXCEPT;

    //! @brief  アライメントが有効かどうかを取得します。
    //! @return アライメントが有効であれば true を返します。
    bool IsAlignmentValid() const NN_NOEXCEPT;

    //! @brief  リロケーションテーブルによって変換済みかどうかを取得します。
    //! @return 変換済みであれば true を返します。
    bool IsRelocated() const NN_NOEXCEPT;

    //! @brief  ファイルサイズを取得します。
    //! @return ファイルサイズを返します。
    size_t GetFileSize() const NN_NOEXCEPT;

    //! @brief  アライメントを取得します。
    //! @return アライメントを返します。
    size_t GetAlignment() const NN_NOEXCEPT;

    //! @brief  ファイル名を取得します。
    //! @return ファイル名を示す文字列参照を返します。
    util::string_view GetFileName() const NN_NOEXCEPT;

    //! @brief  リロケーションテーブルを取得します。
    //! @return リロケーションテーブルを返します。@n
    //!         リロケーションテーブルが設定されていない場合、NULL を返します。
    RelocationTable* GetRelocationTable() NN_NOEXCEPT;

    //! @brief  最初のブロックを取得します。
    //! @return 最初のブロックを返します。@n
    //!         ブロックが設定されていない場合、NULL を返します。
    BinaryBlockHeader* GetFirstBlock() NN_NOEXCEPT;

    //! @brief  最初のブロックを取得します。
    //! @return 最初のブロックを返します。@n
    //!         ブロックが設定されていない場合、NULL を返します。
    const BinaryBlockHeader* GetFirstBlock() const NN_NOEXCEPT;

    //! @brief  指定したシグネチャを持つ最初のブロックを取得します。
    //! @return 指定したシグネチャを持つ最初のブロックを返します。@n
    //!         指定したシグネチャを持つブロックが存在しない場合、NULL を返します。
    BinaryBlockHeader* FindFirstBlock( int packedSignature ) NN_NOEXCEPT;

    //! @brief  指定したシグネチャを持つ最初のブロックを取得します。
    //! @return 指定したシグネチャを持つ最初のブロックを返します。@n
    //!         指定したシグネチャを持つブロックが存在しない場合、NULL を返します。
    const BinaryBlockHeader* FindFirstBlock( int packedSignature ) const NN_NOEXCEPT;

public:
    //! @brief      データのエンディアンを識別するバイトオーダーマークを設定します。
    //! @param[in]  byteOrderMark   設定するバイトオーダーマークです。
    void SetByteOrderMark( ByteOrderMark byteOrderMark ) NN_NOEXCEPT;

public:
    //! @briefprivate   リソースに含まれるアドレスのサイズを設定します。
    void SetAddressSize() NN_NOEXCEPT;

    //! @briefprivate   アライメントを設定します。
    //! @param[in]      alignment   設定するアライメントです。
    void SetAlignment( size_t alignment ) NN_NOEXCEPT;

    //! @briefprivate   ファイルサイズを設定します。
    //! @param[in]      size    設定するファイルサイズです。
    void SetFileSize( size_t size ) NN_NOEXCEPT;

    //! @briefprivate   ファイル名を設定します。
    //! @param[in]      name    設定するファイル名を示す文字列参照です。
    void SetFileName( const util::string_view& name ) NN_NOEXCEPT;

    //! @briefprivate   最初のブロックを設定します。
    //! @param[in]      pBlock  最初のブロックのヘッダへのポインタです。
    void SetFirstBlock( BinaryBlockHeader* pBlock ) NN_NOEXCEPT;

    //! @briefprivate   リロケーションテーブルによって変換済みかどうかを設定します。
    //! @param[in]      value   設定する値です。
    void SetRelocated( bool value ) NN_NOEXCEPT;

    //! @briefprivate   リロケーションテーブルを設定します。
    //! @param[in]      pTable  リロケーションテーブルへのポインタです。リロケーションテーブルを使用しない場合、NULL を指定します。
    void SetRelocationTable( RelocationTable* pTable ) NN_NOEXCEPT;
};

//! @brief  読み取り専用の機能です。構築しないでください。@n
//!         バイナリブロックのヘッダです。
struct BinaryBlockHeader
{
    //! @brief ブロックの種類を表す 4 つの 1 バイト値です。
    BinBlockSignature signature;

    uint32_t _offsetToNextBlock;    // ブロックの先頭から次のデータブロックへのオフセットです。
    uint32_t _blockSize;            // データブロックのサイズです。
    uint32_t _reserved;             // 予約領域です。

public:

    //! @brief  次のブロックを取得します。
    //! @return 次のブロックを返します。@n
    //!         ブロックが設定されていない場合、NULL を返します。
    BinaryBlockHeader* GetNextBlock() NN_NOEXCEPT;

    //! @brief  次のブロックを取得します。
    //! @return 次のブロックを返します。@n
    //!         ブロックが設定されていない場合、NULL を返します。
    const BinaryBlockHeader* GetNextBlock() const NN_NOEXCEPT;

    //! @brief  指定したシグネチャを持つ次のブロックを取得します。
    //! @return 指定したシグネチャを持つ次のブロックを返します。@n
    //!         指定したシグネチャを持つブロックが存在しない場合、NULL を返します。
    BinaryBlockHeader* FindNextBlock( int packedSignature ) NN_NOEXCEPT;

    //! @brief  指定したシグネチャを持つ次のブロックを取得します。
    //! @return 指定したシグネチャを持つ次のブロックを返します。@n
    //!         指定したシグネチャを持つブロックが存在しない場合、NULL を返します。
    const BinaryBlockHeader* FindNextBlock( int packedSignature ) const NN_NOEXCEPT;

    //! @brief  バイナリブロックのサイズを取得します。
    //! @return バイナリブロックのサイズを返します。
    size_t GetBlockSize() const NN_NOEXCEPT;

public:
    //! @briefprivate   次のブロックを設定します。
    //! @param[in]      pBlock  次のブロックのヘッダへのポインタです。
    void SetNextBlock( BinaryBlockHeader* pBlock ) NN_NOEXCEPT;

    //! @briefprivate   バイナリブロックのサイズを設定します。
    //! @param[in]      size    バイナリブロックのサイズです。
    void SetBlockSize( size_t size ) NN_NOEXCEPT;
};

//! @brief  読み取り専用の機能です。構築しないでください。@n
//!         リロケーションテーブルです。
struct RelocationTable
{
    typedef uint64_t AddrType;  // アドレスサイズは 8 バイトで固定します。

    //! @brief  リロケーションテーブルを示すシグネチャを数値化したものです。
    static const int PackedSignature = NN_UTIL_CREATE_SIGNATURE_4( '_', 'R', 'L', 'T');

    //! @brief  再配置するセクションを示します。
    struct Section
    {
        //! @brief      セクション配置先のポインタを設定します。
        //! @param[in]  pSection    セクション配置先のポインタです。
        void SetPtr( void* pSection ) NN_NOEXCEPT;

        //! @brief  セクション配置先のポインタを取得します。
        //! @return セクション配置先のポインタを返します。
        void* GetPtr() const NN_NOEXCEPT;

        //! @brief      ファイルにおけるセクションのポインタを取得します。
        //! @param[in]  pFile   ファイルの先頭のポインタです。
        //! @return     セクションのポインタを返します。
        void* GetPtrInFile( void* pFile ) const NN_NOEXCEPT;

        //! @brief      再配置時におけるファイルの先頭のポインタを取得します。
        //! @param[in]  pFile   再配置前のファイルの先頭のポインタです。
        //! @return     再配置後のファイルの先頭のポインタを返します。@n
        //!             ファイル外に再配置されていない場合、pFile を返します。
        void* GetBasePtr( void* pFile ) const NN_NOEXCEPT;

        //! @brief  セクションのサイズを取得します。
        //! @return セクションのサイズを返します。
        size_t GetSize() const NN_NOEXCEPT;

    public:
        union
        {
            AddrType _padding;
            void* _ptr;      // セクションを配置したアドレスです。
        };

        uint32_t _position;  // ファイルの先頭を基準とするセクションの位置です。
        uint32_t _size;      // セクションのサイズです。
        int32_t _entryIndex; // エントリ配列におけるセクションの最初のエントリのインデックスです。
        int32_t _entryCount; // セクション内のエントリの数です。
    };

    //! @briefprivate   ポインタの情報を保持します。
    struct Entry
    {
        uint32_t _position;     // ファイルの先頭を基準とするオフセットの位置です。
        uint16_t _structCount;  // 構造体配列の配列長です。
        uint8_t _offsetCount;   // 構造体における連続するオフセットの数です。
        uint8_t _paddingCount;  // 構造体内の最後のオフセットから次のオフセットへのパディング数を BinTPtr のサイズで割った値です。
    };

public:
    //! @brief      リロケーションテーブルのサイズを計算します。
    //! @param[in]  sectionCount    リロケーションテーブルに含まれるセクションの数です。
    //! @param[in]  entryCount      リロケーションテーブルに含まれるエントリの数です。
    //! @return     リロケーションテーブルのサイズを返します。
    static size_t CalculateSize( int sectionCount, int entryCount ) NN_NOEXCEPT;

    //! @brief      バイナリファイルのメモリ上の配置を固定し、データにアクセス可能な状態にします。
    //! @pre        対応するバイナリファイルヘッダの IsRelocated() が false であること。
    //! @details    バイナリファイル中の BinTPtr が持つオフセット値をポインタに変換し、変換済みフラグをバイナリファイルヘッダに設定します。
    void Relocate() NN_NOEXCEPT;

    //! @brief      バイナリファイルをメモリ上で移動可能な状態に戻し、データにアクセス不可能な状態にします。
    //! @pre        対応するバイナリファイルヘッダの IsRelocated() が true であること。
    //! @details    バイナリファイル中の BinTPtr が持つポインタをオフセット値に逆変換し、未変換フラグをバイナリファイルヘッダに設定します。
    void Unrelocate() NN_NOEXCEPT;

    //! @brief      指定インデックスのセクション情報を取得します。
    //! @param[in]  sectionIndex    セクションのインデックスです。
    //! @return     指定インデックスのセクション情報を返します。
    Section* GetSection( int sectionIndex ) NN_NOEXCEPT;

    //! @brief      指定のインデックスのセクションを取得します。
    //! @param[in]  sectionIndex    セクションのインデックスです。
    //! @return     指定インデックスのセクション情報を返します。
    const Section* GetSection( int sectionIndex ) const NN_NOEXCEPT;

public:
    //! @briefprivate   シグネチャを設定します。
    void SetSignature() NN_NOEXCEPT;

public:
    BinBlockSignature _signature;   // シグネチャです。
    uint32_t _position;             // ファイルの先頭を基準とするリロケーションテーブルの位置です。
    int32_t _sectionCount;          // セクションの数です。
    NN_PADDING4;                    // Section が 8 バイトアラインのためパディングを追加します。
    Section _sections[ 1 ];         // sectionCount 個の Section が並びます。
    // Section 配列の後に Entry の配列が並びます。
};

//! @brief  読み取り専用の機能です。構築しないでください。@n
//!         文字列のプールです。
struct StringPool
{
    //! @brief  文字列プールを示すシグネチャを数値化したものです。
    static const int PackedSignature = NN_UTIL_CREATE_SIGNATURE_4( '_', 'S', 'T', 'R');

    //! @brief  空文字列を除いた格納している文字列の数を取得します。
    //! @return 空文字列を除いた格納している文字列の数を返します。
    int GetStringCount() const NN_NOEXCEPT;

public:
    //! @briefprivate   空文字列を除いた格納している文字列の数を設定します。
    //! @param[in]      count   空文字列を除いた格納している文字列の数です。
    void SetStringCount( int count ) NN_NOEXCEPT;

    //! @briefprivate   ブロックヘッダにシグネチャを設定します。
    void SetSignature() NN_NOEXCEPT;

public:
    BinaryBlockHeader _header;  // 文字列プールのブロックヘッダです。
    int32_t _stringCount;       // 文字列プールに格納された文字列の数です。
    BinString _strings;         // 1 + stringCount 個の BinString と終端 BinString が並びます。
};

}} // namespace nn::util
