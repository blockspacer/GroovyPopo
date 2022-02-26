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
* @brief データとアクセサの相互変換に関する API の宣言
*/

#pragma once

#include <nn/gfx/detail/gfx_DataContainer.h>

namespace nn {
namespace gfx {

/**
* @brief アクセサーをデータに変換します。
*
* @tparam TAccessor アクセサーの型
*
* @param[in] accessor 対象のアクセサー
*
* @return 変換されたデータを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、アクセサーに対応するデータ型の参照またはポインターに代入してください。
*/
template< typename TAccessor >
inline detail::Caster< typename TAccessor::DataType > AccessorToData( TAccessor& accessor ) NN_NOEXCEPT
{
    return accessor.ToData();
}

/**
* @brief アクセサーをデータに変換します。
*
* @tparam TAccessor アクセサーの型
*
* @param[in] accessor 対象のアクセサー
*
* @return 変換されたデータを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、アクセサーに対応するデータ型の参照またはポインターに代入してください。
*/
template< typename TAccessor >
inline detail::Caster< volatile typename TAccessor::DataType > AccessorToData( volatile TAccessor& accessor ) NN_NOEXCEPT
{
    return accessor.ToData();
}

/**
* @brief アクセサーをデータに変換します。
*
* @tparam TAccessor アクセサーの型
*
* @param[in] accessor 対象のアクセサー
*
* @return 変換されたデータを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、アクセサーに対応するデータ型の参照またはポインターに代入してください。
*/
template< typename TAccessor >
inline detail::Caster< volatile const typename TAccessor::DataType > AccessorToData( volatile const TAccessor& accessor ) NN_NOEXCEPT
{
    return accessor.ToData();
}

/**
* @brief アクセサーをデータに変換します。
*
* @tparam TAccessor アクセサーの型
*
* @param[in] accessor 対象のアクセサー
*
* @return 変換されたデータを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、アクセサーに対応するデータ型の参照またはポインターに代入してください。
*/
template< typename TAccessor >
inline detail::Caster< const typename TAccessor::DataType > AccessorToData( const TAccessor& accessor ) NN_NOEXCEPT
{
    return accessor.ToData();
}

/**
* @brief アクセサーをデータに変換します。
*
* @tparam TAccessor アクセサーの型
*
* @param[in] pAccessor 対象のアクセサーへのポインター
*
* @return 変換されたデータを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、アクセサーに対応するデータ型の参照またはポインターに代入してください。
*/
template< typename TAccessor >
inline detail::Caster< typename TAccessor::DataType > AccessorToData( TAccessor* pAccessor ) NN_NOEXCEPT
{
    return pAccessor->ToData();
}

/**
* @brief アクセサーをデータに変換します。
*
* @tparam TAccessor アクセサーの型
*
* @param[in] pAccessor 対象のアクセサーへのポインター
*
* @return 変換されたデータを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、アクセサーに対応するデータ型の参照またはポインターに代入してください。
*/
template< typename TAccessor >
inline detail::Caster< const typename TAccessor::DataType > AccessorToData( const TAccessor* pAccessor ) NN_NOEXCEPT
{
    return pAccessor->ToData();
}

/**
* @brief アクセサーをデータに変換します。
*
* @tparam TAccessor アクセサーの型
*
* @param[in] pAccessor 対象のアクセサーへのポインター
*
* @return 変換されたデータを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、アクセサーに対応するデータ型の参照またはポインターに代入してください。
*/
template< typename TAccessor >
inline detail::Caster< volatile typename TAccessor::DataType > AccessorToData( volatile TAccessor* pAccessor ) NN_NOEXCEPT
{
    return pAccessor->ToData();
}

/**
* @brief アクセサーをデータに変換します。
*
* @tparam TAccessor アクセサーの型
*
* @param[in] pAccessor 対象のアクセサーへのポインター
*
* @return 変換されたデータを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、アクセサーに対応するデータ型の参照またはポインターに代入してください。
*/
template< typename TAccessor >
inline detail::Caster< volatile const typename TAccessor::DataType > AccessorToData( volatile const TAccessor* pAccessor ) NN_NOEXCEPT
{
    return pAccessor->ToData();
}

/**
* @brief データをアクセサーに変換します。
*
* @tparam TData データの型
*
* @param[in] data 対象のデータ
*
* @return 変換されたアクセサーを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、データに対応するアクセサー型の参照またはポインターに代入してください。
*/
template< typename TData >
inline detail::Caster< detail::DataContainer< TData > > DataToAccessor( TData& data ) NN_NOEXCEPT
{
    return detail::DataContainer< TData >::DataToAccessor( data );
}

/**
* @brief データをアクセサーに変換します。
*
* @tparam TData データの型
*
* @param[in] data 対象のデータ
*
* @return 変換されたアクセサーを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、データに対応するアクセサー型の参照またはポインターに代入してください。
*/
template< typename TData >
inline detail::Caster< const detail::DataContainer< TData > > DataToAccessor( const TData& data ) NN_NOEXCEPT
{
    return detail::DataContainer< TData >::DataToAccessor( data );
}

/**
* @brief データをアクセサーに変換します。
*
* @tparam TData データの型
*
* @param[in] data 対象のデータ
*
* @return 変換されたアクセサーを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、データに対応するアクセサー型の参照またはポインターに代入してください。
*/
template< typename TData >
inline detail::Caster< volatile detail::DataContainer< TData > > DataToAccessor( volatile TData& data ) NN_NOEXCEPT
{
    return detail::DataContainer< TData >::DataToAccessor( data );
}

/**
* @brief データをアクセサーに変換します。
*
* @tparam TData データの型
*
* @param[in] data 対象のデータ
*
* @return 変換されたアクセサーを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、データに対応するアクセサー型の参照またはポインターに代入してください。
*/
template< typename TData >
inline detail::Caster< volatile const detail::DataContainer< TData > > DataToAccessor( volatile const TData& data ) NN_NOEXCEPT
{
    return detail::DataContainer< TData >::DataToAccessor( data );
}

/**
* @brief データをアクセサーに変換します。
*
* @tparam TData データの型
*
* @param[in] pData 対象のデータへのポインター
*
* @return 変換されたアクセサーを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、データに対応するアクセサー型の参照またはポインターに代入してください。
*/
template< typename TData >
inline detail::Caster< detail::DataContainer< TData > > DataToAccessor( TData* pData ) NN_NOEXCEPT
{
    return detail::DataContainer< TData >::DataToAccessor( *pData );
}

/**
* @brief データをアクセサーに変換します。
*
* @tparam TData データの型
*
* @param[in] pData 対象のデータへのポインター
*
* @return 変換されたアクセサーを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、データに対応するアクセサー型の参照またはポインターに代入してください。
*/
template< typename TData >
inline detail::Caster< const detail::DataContainer< TData > > DataToAccessor( const TData* pData ) NN_NOEXCEPT
{
    return detail::DataContainer< TData >::DataToAccessor( *pData );
}

/**
* @brief データをアクセサーに変換します。
*
* @tparam TData データの型
*
* @param[in] pData 対象のデータへのポインター
*
* @return 変換されたアクセサーを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、データに対応するアクセサー型の参照またはポインターに代入してください。
*/
template< typename TData >
inline detail::Caster< volatile detail::DataContainer< TData > > DataToAccessor( volatile TData* pData ) NN_NOEXCEPT
{
    return detail::DataContainer< TData >::DataToAccessor( *pData );
}

/**
* @brief データをアクセサーに変換します。
*
* @tparam TData データの型
*
* @param[in] pData 対象のデータへのポインター
*
* @return 変換されたアクセサーを取得するための一時クラスを返します。
*
* @details
* この関数の返り値は、データに対応するアクセサー型の参照またはポインターに代入してください。
*/
template< typename TData >
inline detail::Caster< volatile const detail::DataContainer< TData > > DataToAccessor( volatile const TData* pData ) NN_NOEXCEPT
{
    return detail::DataContainer< TData >::DataToAccessor( *pData );
}

}
}
