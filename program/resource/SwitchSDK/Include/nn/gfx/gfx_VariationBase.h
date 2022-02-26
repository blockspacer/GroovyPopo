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

#include <nn/gfx/gfx_Config.h>

namespace nn {
namespace gfx {

/**
* @brief 低レベルグラフィックス API を表す列挙型です。
*/
enum LowLevelApi
{
    LowLevelApi_Common = NN_GFX_CONFIG_API_TYPE_COMMON //!< 全低レベルグラフィックス API 共通です
};

/**
* @brief 低レベルグラフィックス API のバージョンを表す列挙型です。
*/
enum Version
{
    Version_All = -1 //!< 全バージョンです
};

/**
* @brief 低レベルグラフィックス API の種類を表す構造体です。
*
* @tparam api LowLevelApi_* で表される低レベルグラフィックス API の種類の値
*/
template< int api >
struct ApiType
{
    static const int value = api;
};

/**
* @brief 低レベルグラフィックス API のバージョンを表す構造体です。
*
* @tparam version 低レベルグラフィックス API のバージョン値
*/
template< int version >
struct ApiVersion
{
    static const int value = version;
};

/**
* @brief 低レベルグラフィックス API のデフォルトバージョンを表す構造体です。
*
* @tparam TType 対象の低レベルグラフィックス API
*/
template< typename TType >
struct DefaultVersion;

/**
* @brief 低レベルグラフィックス API の対象バージョンを表す構造体です。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*/
template< typename TTarget >
struct TargetVersion
{
    typedef typename TTarget::Version Version;
};

/**
* @brief 低レベルグラフィックス API のバリエーションを表す構造体です。
*
* @tparam TType 低レベルグラフィックス API の種類
* @tparam TVersion 低レベルグラフィックス API のバージョン
*/
template< typename TType, typename TVersion = typename TargetVersion< TType >::Version >
struct ApiVariation
{
    typedef TType Type;
    typedef TVersion Version;
    typedef ApiVariation< Type, Version > Variation;
};


/**
* @brief 低レベルグラフィックス API の設定を表す構造体です。
*
* @tparam TType 対象の低レベルグラフィックス API
*/
template< typename TType >
struct ApiTypeConfig
{
    typedef TType Type;
    typedef typename DefaultVersion< Type >::Version DefaultVersion;
    typedef typename TargetVersion< Type >::Version TargetVersion;
    typedef ApiVariation< Type, TargetVersion > TargetVariation;
};

namespace detail {

template< typename T, typename U >
struct HasVariation;
template< typename T >
struct HasVariation< T, typename T::Variation >
{
    typedef void Type;
};

template< typename T, int N = sizeof( T ) >
struct ExistsImpl
{
    typedef void Type;
};

template< typename TApiType, typename X = void >
struct TargetVariation
{
    typedef typename ApiTypeConfig< TApiType >::TargetVariation Type;
};
template< typename TVariation >
struct TargetVariation< TVariation, typename HasVariation<
    TVariation, typename TVariation::Variation >::Type >
{
    typedef typename TVariation::Variation Type;
};

template< typename TInfoImpl, typename TInfo, typename X = void >
struct TargetInfoImpl
{
    typedef TInfo Type;
};
template< typename TInfoImpl, typename TInfo >
struct TargetInfoImpl< TInfoImpl, TInfo, typename ExistsImpl< TInfoImpl >::Type >
{
    typedef TInfoImpl Type;
};

}

}
}
