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

#include <nn/atk/fnd/basis/atkfnd_Inlines.h>
#include <nn/atk/fnd/binary/atkfnd_PrimitiveTypes.h>

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

//! @internal
//! @brief リソース内での名前を表す構造体です。
struct ResNameData
{
    //! @brief 文字のアライメントサイズです。
    typedef atk::detail::fnd::BinU32 LengthType;
    LengthType len;
    char str[sizeof(LengthType)];   // 実際には長さ len の文字列が入っている。
    // 末尾は sizeof(LengthType) バイトアラインメントされていて、
    // パディングは0である。
};

//! @internal
//! @brief リソース内での名前を表すクラスです。
class ResName : private ResNameData
{
public:
    using ResNameData::LengthType;

public:
    //! @brief 指定した長さの文字列を格納するのに必要なサイズを返します。
    //! @param[in] strLength 文字列の長さです。
    //! @return サイズ(バイト)を返します。
    static size_t GetRequiredSize(int strLength) NN_NOEXCEPT
    {
        // NULL 文字分を含めるため strLength + 1 します。
        return sizeof(LengthType) + nn::util::align_up(strLength + 1, sizeof(LengthType));
    }

    //! @brief 文字列の長さを取得します。
    //! @return 文字列の長さを返します。
    size_t GetLength() const NN_NOEXCEPT { return static_cast<size_t>(this->len); }

    //! @brief 文字列へのポインタを取得します。
    //! @return 文字列へのポインタを返します。
    const char* GetName() const NN_NOEXCEPT { return &this->str[0]; }

    //! @brief 長さ付き文字列と比較します。
    //! @param[in] string 文字列です。
    //! @param[in] length 文字列長です。
    //! @return 一致したら true を返します。
    bool Equals(const char* string, size_t length) const NN_NOEXCEPT
    {
        return (GetLength() == length) && (std::strcmp(GetName(), string) == 0);
    }

    //! @brief ResName と比較します。
    //! @param[in] rhs 比較対象です。
    //! @return 一致したら true を返します。
    bool Equals(const ResName* rhs) const NN_NOEXCEPT
    {
        return (this == rhs) || Equals(rhs->GetName(), rhs->GetLength());
    }
};

//! @brief リソース内でのデータオフセットを表すクラスです。
class Offset
{
private:
    atk::detail::fnd::BinS32 offset;

public:
    // 共用体に入れるために、コンストラクタを実装しない。

    //! @brief 直接 offset を返す演算子オーバーロードです。
    NN_IMPLICIT operator int32_t() const NN_NOEXCEPT { return offset; }

    //! @brief 直接 offset に代入する演算子オーバーロードです。
    //! @param[in] ofs 代入するオフセット値です。
    //! @return 自身への参照を返します。
    Offset& operator=(int32_t ofs) NN_NOEXCEPT { offset = ofs; return *this; }

    //! @brief ポインタからオフセットに変換して設定します。
    //! @param[in] ptr ポインタです。
    void  set_ptr(const void* ptr) NN_NOEXCEPT
    {
        offset = (ptr == NULL) ? 0 :
            static_cast<int32_t>(reinterpret_cast<intptr_t>(ptr) - reinterpret_cast<intptr_t>(this));
    }

    //! @brief オフセットからポインタに変換して取得します。
    //! @return ポインタを返します。
    void* to_ptr() NN_NOEXCEPT { return to_ptr<void>(); }

    //! @brief オフセットからポインタに変換して取得します。
    //! @return ポインタを返します。
    const void* to_ptr() const NN_NOEXCEPT { return to_ptr<void>(); }

    //! @brief オフセットからポインタに変換して取得します。
    //! @return ポインタを返します。
    //! @tparam T ポインタが指すデータ型です。
    template<typename T>
    T* to_ptr() NN_NOEXCEPT { return (offset == 0) ? NULL : atk::detail::fnd::AddOffsetToPtr<T*>(this, offset); }

    //! @brief オフセットからポインタに変換して取得します。
    //! @return ポインタを返します。
    //! @tparam T ポインタが指すデータ型です。
    template<typename T>
    const T* to_ptr() const NN_NOEXCEPT { return (offset == 0) ? NULL : atk::detail::fnd::AddOffsetToPtr<const T*>(this, offset); }

    //! @brief オフセットテーブルからオフセットを取得します。
    //! @return オフセットを返します。
    Offset* to_table_ptr() NN_NOEXCEPT { return static_cast<Offset*>(to_ptr()); }

    //! @brief オフセットテーブルからオフセットを取得します。
    //! @return オフセットを返します。
    const Offset* to_table_ptr() const NN_NOEXCEPT { return static_cast<const Offset*>(to_ptr()); }
};

//! @brief 文字列への参照を持つクラスです。
class BinString
{
public:
    //! @brief 文字列へのオフセットです。
    Offset offset;

    //! @brief オフセットから文字列へのポインタに変換します。
    //! @return 文字列へのポインタを返します。
    const char* to_ptr() const NN_NOEXCEPT { return offset.to_ptr<char>(); }

    //! @brief 指定した文字列を参照するように設定します。
    //!
    //! 引数で与えた文字列が破棄された場合、無効なポインタを参照します。
    //!
    //! @param[in] ptr 参照する文字列です。
    void set_ptr(const char* ptr) NN_NOEXCEPT { offset.set_ptr(ptr); }

    //! @brief リソースでの文字列形式を取得します。
    //! @return リソースでの文字列形式を返します。
    ResName* GetResName() NN_NOEXCEPT
    {
        return offset == 0 ? NULL : static_cast<ResName*>(
            atk::detail::fnd::AddOffsetToPtr<ResName*>(this, offset - sizeof(ResName::LengthType)));
    }

    //! @brief リソースでの文字列形式を取得します。
    //! @return リソースでの文字列形式を返します。
    const ResName* GetResName() const NN_NOEXCEPT
    {
        return offset == 0 ? NULL : static_cast<const ResName*>(
            atk::detail::fnd::AddOffsetToPtr<const ResName*>(this, offset - sizeof(ResName::LengthType)));
    }
};

#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
#pragma warning(push)
#pragma warning(disable:4200)
#endif
//! @brief オフセットテーブルデータを示すクラスです。
struct ResOffsetTableData
{
    atk::detail::fnd::BinU32 itemCount;
    Offset itemOffsets[0];
};
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
#pragma warning(pop)
#endif

//! @brief オフセットテーブルを操作するクラスです。
template<typename TItem = void>
class ResOffsetTable
{
public:
    //! @brief アイテム数を取得します。
    //! @return アイテム数を返します。
    uint32_t GetItemCount() const NN_NOEXCEPT { return m_Data.itemCount; }

    //! @brief 指定インデックスのアイテムを取得します。
    //! @param[in] index インデックスです。
    //! @return アイテムへのポインタを返します。
    const TItem* GetItem(uint32_t index) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT(index < m_Data.itemCount);
        return m_Data.itemOffsets[index].template to_ptr<TItem>();
    }

private:
    ResOffsetTableData m_Data;
};

//! @brief バージョンデータを示す共用体です。
union ResVersionData
{
    struct Elements
    {
        uint8_t major;
        uint8_t minor;
        uint8_t micro;
        uint8_t release;
    };

    ResVersionData(uint32_t value) NN_NOEXCEPT
    {
        elements.major   = 0xff & (value >> 24);
        elements.minor   = 0xff & (value >> 16);
        elements.micro   = 0xff & (value >> 8);
        elements.release = 0xff & value;
    }

    uint32_t      value;
    Elements elements;
};

//! @brief バージョンを操作するクラスです。
class ResVersion
{
public:
    //! @brief コンストラクタです。
    //! @param[in] value バージョン番号です。
    explicit ResVersion(uint32_t value) NN_NOEXCEPT : m_Data(value) { }

public:
    //! @brief メジャーバージョンを取得します。
    //! @return メジャーバージョンを返します。
    uint8_t GetMajor() const NN_NOEXCEPT { return m_Data.elements.major; }

    //! @brief マイナーバージョンを取得します。
    //! @return マイナーバージョンを返します。
    uint8_t GetMinor() const NN_NOEXCEPT { return m_Data.elements.minor; }

    //! @brief マイクロバージョンを取得します。
    //! @return マイクロバージョンを返します。
    uint8_t GetMico() const NN_NOEXCEPT { return m_Data.elements.micro; }

    //! @brief リリースバージョンを取得します。
    //! @return リリースバージョンを返します。
    uint8_t GetRelease() const NN_NOEXCEPT { return m_Data.elements.release; }

private:
    ResVersionData m_Data;
};

//! @internal
//! @brief  32 バイトハッシュ値です。
struct ResHash32
{
    static const uint32_t Size = 32;
    uint8_t value[Size];
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
