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
#if NN_BUILD_CONFIG_COMPILER_SUPPORTS_STD_TYPE_TRAITS
    #include <type_traits>
#endif
#include <nn/nn_Macro.h>
#include <nn/nn_SdkAssert.h>
#include <nn/nn_StaticAssert.h>

namespace nn { namespace util {

/*!
    @brief ビットフィールド代替型です。

    @details    ビットフラグの集合としてのみ利用する型は BitFlagSet を利用してください。

    @tparam     StorageT 値を格納する整数型です。int より大きな型は使用できません。
    @tparam     Tag 異なる型としてインスタンス化するためのタグです。
*/
template< typename StorageT, typename Tag = void >
struct BitPack
{
    StorageT storage; //!< 値を格納する領域です。値の受け渡しが必要な場合を除きアクセスは非推奨です。

public:

    //! @{
    //! @name 単一ビット操作

    /*!
        @brief 指定位置のビットを変更します。

        @param[in]  pos LSB から数えたビットの位置です。
        @param[in]  isOn ビットに書き込む値です。
    */
    void SetBit( int pos, bool isOn ) NN_NOEXCEPT
    {
        int mask = MakeMask( pos );
        if ( isOn )
        {
            storage |= mask;
        }
        else
        {
            storage &= ~mask;
        }
    }

    /*!
        @brief 指定位置のビットが 1 かどうかを取得します。

        @param[in]  pos LSB から数えたビットの位置です。
        @return     ビットが 1 かどうかです。
    */
    bool GetBit( int pos ) const NN_NOEXCEPT
    {
        return IsAnyBitOn( MakeMask( pos ) );
    }

    //! @}

    //! @{
    //! @name 複数ビット操作

    /*!
        @brief 全てのビットを 0 に設定します。
    */
    void Clear() NN_NOEXCEPT
    {
        storage = StorageT( 0 );
    }

    /*!
        @brief 指定位置を指定したビットマスクに変更します。

        @param[in]  mask 操作する位置を表すビットマスクです。
        @param[in]  value 書き込むビットマスクです。
        @pre        ( ~mask & value ) == 0
    */
    void SetMaskedBits( int mask, int value ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES( ( ~mask & value ) == 0 );
        storage &= ~mask;
        storage |= value;
    }

    /*!
        @brief 指定位置のビットマスクを取得します。

        @param[in]  mask 取得する位置を表すビットマスクです。
        @return     書き込むビットマスクです。
    */
    int GetMaskedBits( int mask ) const NN_NOEXCEPT
    {
        return static_cast< int >( storage & mask );
    }

    /*!
        @brief 指定位置の全てのビットを 1 に設定します。

        @param[in]  mask 操作する位置を表すビットマスクです。
    */
    void SetAllBitOn( int mask ) NN_NOEXCEPT
    {
        SetMaskedBits( mask, mask );
    }

    /*!
        @brief 指定位置の全てのビットを 0 に設定します。

        @param[in]  mask 操作する位置を表すビットマスクです。
    */
    void SetAllBitOff( int mask ) NN_NOEXCEPT
    {
        SetMaskedBits( mask, 0 );
    }

    /*!
        @brief 指定位置のビットが全て 1 かどうかを取得します。

        @param[in]  mask 判定する位置を表すビットマスクです。
        @return     ビットが全て 1 かどうかです。
    */
    bool IsAllBitOn( int mask ) const NN_NOEXCEPT
    {
        return static_cast< int >( storage & mask ) == mask;
    }

    /*!
        @brief 指定位置のビットが全て 0 かどうかを取得します。

        @param[in]  mask 判定する位置を表すビットマスクです。
        @return     ビットが全て 0 かどうかです。
    */
    bool IsAllBitOff( int mask ) const NN_NOEXCEPT
    {
        return !( storage & mask );
    }

    /*!
        @brief 指定位置のビットのいずれかが 1 かどうかを取得します。

        @param[in]  mask 判定する位置を表すビットマスクです。
        @return     ビットのいずれかが 1 かどうかです。
    */
    bool IsAnyBitOn( int mask ) const NN_NOEXCEPT
    {
        return !!( storage & mask );
    }

    /*!
    @brief 指定位置のビットのいずれかが 0 かどうかを取得します。

    @param[in]  mask 判定する位置を表すビットマスクです。
    @return     ビットのいずれかが 0 かどうかです。
    */
    bool IsAnyBitOff( int mask ) const NN_NOEXCEPT
    {
        return static_cast< int >( storage & mask ) != mask;
    }

    //! @}

    //! @{
    //! @name ビットフィールド操作

    /*!
        @brief ビットフィールドの値を設定します。

        @tparam     FieldT nn::util::BitPack::Field を使用したビットフィールドの定義です。
        @param[in]  value ビットフィールドに格納する値です。
    */
    template< typename FieldT >
    void Set( typename FieldT::Type value ) NN_NOEXCEPT
    {
#if NN_BUILD_CONFIG_COMPILER_SUPPORTS_STD_TYPE_TRAITS
        typedef std::is_same<
            FieldT,
            Field< FieldT::Pos, FieldT::Width, typename FieldT::Type >
        > IsValidFieldT;
        NN_STATIC_ASSERT( IsValidFieldT::value );
#endif

        SetMaskedBits( FieldT::Mask, static_cast< int >( value ) << FieldT::Pos );
    }

    /*!
        @brief ビットフィールドの値を取得します。

        @tparam     FieldT nn::util::BitPack::Field を使用したビットフィールドの定義です。
        @return     ビットフィールドに格納されている値です。
    */
    template< typename FieldT >
    typename FieldT::Type Get() const NN_NOEXCEPT
    {
#if NN_BUILD_CONFIG_COMPILER_SUPPORTS_STD_TYPE_TRAITS
        typedef std::is_same<
            FieldT,
            Field< FieldT::Pos, FieldT::Width, typename FieldT::Type >
        > IsValidFieldT;
        NN_STATIC_ASSERT( IsValidFieldT::value );
#endif

        typename FieldT::Type value;
        ReadValue( &value, FieldT::Mask, FieldT::Pos );
        return value;
    }

    //! @}

    /*!
        @brief ビットフィールドを定義する型です。

        @tparam     BitPos ビットフィールドの最下位ビットの位置です。
        @tparam     BitWidth ビットフィールドのビット幅です。
        @tparam     T ビットフィールドに格納する型です。
    */
    template< int BitPos, int BitWidth, typename T >
    struct Field
    {
        typedef T Type; //!< ビットフィールドに格納する型です。
        static const int Pos = BitPos; //!< ビットフィールドの最下位ビットの位置です。
        static const int Next = BitPos + BitWidth; //!< ビットフィールドの次に上位のビット位置です。
        static const int Mask = static_cast< int >( ( ( uint64_t( 1 ) << BitWidth ) - 1 ) << BitPos ); //!< ビットフィールドのビットマスクです。

        //! @cond
        static const int Width = BitWidth;
        //! @endcond

    private:
#if NN_BUILD_CONFIG_COMPILER_SUPPORTS_STD_TYPE_TRAITS
        NN_STATIC_ASSERT( std::is_integral< T >::value || std::is_enum< T >::value );
        NN_STATIC_ASSERT( !( std::is_same< T, bool >::value ) || BitWidth == 1 );
#endif
        NN_STATIC_ASSERT( BitWidth <= NN_BITSIZEOF( T ) );
        NN_STATIC_ASSERT( Next <= NN_BITSIZEOF( StorageT ) );
    };

private:

#if NN_BUILD_CONFIG_COMPILER_SUPPORTS_STD_TYPE_TRAITS
    NN_STATIC_ASSERT( std::is_integral< StorageT >::value );
#endif
    NN_STATIC_ASSERT( sizeof( StorageT ) <= sizeof( int ) );

    // 指定位置のビットのみが 1 のビットマスクを作成します。
    static int MakeMask( int pos ) NN_NOEXCEPT
    {
        return 1 << pos;
    }

    template< typename T >
    void ReadValue( T* pValue, int mask, int pos ) const NN_NOEXCEPT
    {
        *pValue = static_cast< T >( static_cast< unsigned int >( GetMaskedBits( mask ) ) >> pos );
    }

    // bool へのキャストに対する警告対処と最適化のためにオーバーロードを用意します。
    void ReadValue( bool* pValue, int mask, int /* pos */ ) const NN_NOEXCEPT
    {
        *pValue = IsAnyBitOn( mask );
    }
};

template< typename StorageT, typename Tag>
template< int BitPos, int BitWidth, typename T >
const int BitPack<StorageT, Tag>::Field<BitPos, BitWidth, T>::Pos;

template< typename StorageT, typename Tag>
template< int BitPos, int BitWidth, typename T >
const int BitPack<StorageT, Tag>::Field<BitPos, BitWidth, T>::Next;

template< typename StorageT, typename Tag>
template< int BitPos, int BitWidth, typename T >
const int BitPack<StorageT, Tag>::Field<BitPos, BitWidth, T>::Mask;

typedef BitPack< uint8_t > BitPack8;
typedef BitPack< uint16_t > BitPack16;
typedef BitPack< uint32_t > BitPack32;

}} // namespace nn::util
