/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
* @file
* @brief ユーザーデータリソースに関する API の宣言
*/

#pragma once

#include <nn/util/util_AccessorBase.h>

#include <nn/gfx/gfx_ResUserDataData.h>

namespace nn {
namespace gfx {

/**
* @brief ユーザーデータリソースを表すクラスです。
*/
class ResUserData
    : public nn::util::AccessorBase< ResUserDataData >
{
    NN_DISALLOW_COPY( ResUserData );

public:
    /**
    * @brief データのタイプです。
    */
    enum Type
    {
        Type_Int, //!< 32-bit 符号付き整数です。
        Type_Float, //!< 浮動小数点数です。
        Type_String, //!< utf-8 文字列です。
        Type_Stream //!< バイト列です。
    };

    /**
    * @brief ユーザーデータ名を取得します。
    *
    * @return ユーザーデータ名へのポインターを返します。
    */
    const char* GetName() const NN_NOEXCEPT
    {
        return this->pName.Get()->GetData();
    }

    /**
    * @brief データの数を取得します。
    *
    * @return データの数を返します。
    */
    int GetCount() const NN_NOEXCEPT
    {
        return this->count;
    }

    /**
    * @brief データのタイプを取得します。
    *
    * @return データのタイプを返します。
    */
    Type GetType() const NN_NOEXCEPT
    {
        return static_cast< Type >( this->type );
    }

    /**
    * @brief 整数値の配列としてデータを取得します。
    *
    * @pre
    * - GetType() == Type_Int
    *
    * @return int32_t 配列の先頭ポインターを返します。
    */
    int32_t* GetInt() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( this->type == Type_Int );
        return static_cast< int32_t* >( this->pData.Get() );
    }

    /**
    * @brief 整数値の配列としてデータを取得します。
    *
    * @pre
    * - GetType() == Type_Int
    *
    * @return int32_t 配列の先頭ポインターを返します。
    */
    const int32_t* GetInt() const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( this->type == Type_Int );
        return static_cast< const int32_t* >( this->pData.Get() );
    }

    /**
    * @brief 整数値としてデータを取得します。
    *
    * @param[in] index 取得するデータのインデックス
    *
    * @pre
    * - GetType() == Type_Int
    * - index >= 0 && index < GetCount()
    *
    * @return int32_t 配列の index で指定した要素を返します。
    */
    int32_t GetInt( int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(this->type == Type_Int);
        NN_SDK_REQUIRES_RANGE(index, 0, GetCount());
        return static_cast< const int32_t* >(this->pData.Get())[index];
    }

    /**
    * @brief 浮動小数点数の配列としてデータを取得します。
    *
    * @pre
    * - GetType() == Type_Float
    *
    * @return float 配列の先頭ポインターを返します。
    */
    float* GetFloat() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( this->type == Type_Float );
        return static_cast< float* >( this->pData.Get() );
    }

    /**
    * @brief 浮動小数点数の配列としてデータを取得します。
    *
    * @pre
    * - GetType() == Type_Float
    *
    * @return float 配列の先頭ポインターを返します。
    */
    const float* GetFloat() const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( this->type == Type_Float );
        return static_cast< const float* >( this->pData.Get() );
    }

    /**
    * @brief 浮動小数点数としてデータを取得します。
    *
    * @param[in] index 取得するデータのインデックス
    *
    * @pre
    * - GetType() == Type_Float
    * - index >= 0 && index < GetCount()
    *
    * @return float 配列の index で指定した要素を返します。
    */
    float GetFloat( int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(this->type == Type_Float);
        NN_SDK_REQUIRES_RANGE(index, 0, GetCount());
        return static_cast< const float* >(this->pData.Get())[index];
    }

    /**
    * @brief 文字列としてデータを取得します。
    *
    * @param[in] index 取得する文字列のインデックス
    *
    * @pre
    * - GetType() == Type_String
    * - index >= 0 && index < GetCount()
    *
    * @return char 配列へのポインターを返します。
    */
    const char* GetString( int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( this->type == Type_String );
        NN_SDK_REQUIRES_RANGE( index, 0, GetCount() );
        nn::util::BinPtrToString pString = static_cast<
            const nn::util::BinPtrToString* >( this->pData.Get() )[ index ];
        return pString.Get()->GetData();
    }

    /**
    * @brief バイト列としてデータを取得します。
    *
    * @pre
    * - GetType() == Type_Stream
    *
    * @return バイト配列へのポインターを返します。
    */
    void* GetStream() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( this->type == Type_Stream );
        return this->pData.Get();
    }

    /**
    * @brief バイト列としてデータを取得します。
    *
    * @pre
    * - GetType() == Type_Stream
    *
    * @return バイト配列へのポインターを返します。
    */
    const void* GetStream() const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( this->type == Type_Stream );
        return this->pData.Get();
    }

    /**
    * @brief バイト列のサイズを取得します。
    *
    * @pre
    * - GetType() == Type_Stream
    *
    * @return バイト列のサイズを返します。
    */
    size_t GetStreamSize() const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( this->type == Type_Stream );
        return this->count;
    }
};

}
}

