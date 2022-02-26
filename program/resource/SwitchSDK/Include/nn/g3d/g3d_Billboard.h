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
* @brief ビルボードに関するクラスの宣言
*/

#pragma once

#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_SkeletonObj.h>
#include <nn/g3d/detail/g3d_MatrixUtility.h>

namespace nn { namespace g3d {

//! @brief ビルボード計算を行うクラスです。
class Billboard
{
public:
    // 各種ビルボード計算
    // スケールは保存されないので抽出して別途適用する必要がある。
    // Viewpoint 系のビルボードは pWorldView から平行移動成分を取得して使用する。

    //! @brief World 方式でビルボード計算を行います。
    //!
    //! @param[out] pWorldView 計算結果を格納するための nn::util::Matrix4x3fType へのポインター。
    //! @param[in] view ビュー変換行列への参照。
    //! @param[in] world ワールド変換行列への参照。
    //!
    //! @details
    //! Z 軸が視点軸と平行となるように回転させます。
    //!
    static void CalculateWorld(nn::util::Matrix4x3fType* pWorldView, const nn::util::Matrix4x3fType& view, const nn::util::Matrix4x3fType& world) NN_NOEXCEPT;

    //! @brief WorldViewpoint 方式でビルボード計算を行います。
    //!
    //! @param[out] pWorldView 計算結果を格納するための nn::util::Matrix4x3fType へのポインター。
    //! @param[in] view ビュー変換行列への参照。
    //! @param[in] world ワールド変換行列への参照。
    //!
    //! @details
    //! Z 軸が視点方向に向くように回転させます。
    //!
    static void CalculateWorldViewpoint(nn::util::Matrix4x3fType* pWorldView, const nn::util::Matrix4x3fType& view, const nn::util::Matrix4x3fType& world) NN_NOEXCEPT;

    //! @brief Screen 方式でビルボード計算を行います。
    //!
    //! @param[out] pWorldView 計算結果を格納するための nn::util::Matrix4x3fType へのポインター。
    //! @param[in] local ローカル行列への参照。
    //!
    //! @details
    //! Z 軸が視点軸と平行で、Y 軸が画面上方向になるように回転させます。
    //!
    static void CalculateScreen(nn::util::Matrix4x3fType* pWorldView, const nn::util::Matrix4x3fType& local) NN_NOEXCEPT;

    //! @brief ビルボード全種類を同じ引数で呼ぶためのラッパーです。
    //!
    //! @param[out] pWorldView 計算結果を格納するための nn::util::Matrix4x3fType へのポインター。
    //! @param[in] view ビュー変換行列への参照。
    //! @param[in] world ワールド変換行列への参照。
    //! @param[in] local ローカル行列への参照。
    //!
    static void CalculateScreenViewpoint(nn::util::Matrix4x3fType* pWorldView,
        const nn::util::Matrix4x3fType& view, const nn::util::Matrix4x3fType& world, const nn::util::Matrix4x3fType& local) NN_NOEXCEPT;

    //! @brief YAxis 方式でビルボード計算を行います。
    //!
    //! @param[out] pWorldView 計算結果を格納するための nn::util::Matrix4x3fType へのポインター。
    //! @param[in] view ビュー変換行列への参照。
    //! @param[in] world ワールド変換行列への参照。
    //!
    //! @details
    //! Z 軸が視点軸と平行となるように Y 軸だけ回転させます。
    //!
    static void CalculateYAxis(nn::util::Matrix4x3fType* pWorldView, const nn::util::Matrix4x3fType& view, const nn::util::Matrix4x3fType& world) NN_NOEXCEPT;

    //! @brief YAxisViewpoint 方式でビルボード計算を行います。
    //!
    //! @param[out] pWorldView 計算結果を格納するための nn::util::Matrix4x3fType へのポインター。
    //! @param[in] view ビュー変換行列への参照。
    //! @param[in] world ワールド変換行列への参照。
    //!
    //! @details
    //! Z 軸が視点方向に向くように Y 軸だけ回転させます。
    //!
    static void CalculateYAxisViewpoint(nn::util::Matrix4x3fType* pWorldView, const nn::util::Matrix4x3fType& view, const nn::util::Matrix4x3fType& world) NN_NOEXCEPT;

    //! @brief ビルボード全種類を同じ引数で呼ぶためのラッパーです。
    //!
    //! @param[out] pWorldView 計算結果を格納するための nn::util::Matrix4x3fType へのポインター。
    //! @param[in] view ビュー変換行列への参照。
    //! @param[in] world ワールド変換行列への参照。
    //! @param[in] local ローカル行列への参照。使用しません。
    //!
    static void CalculateWorld(nn::util::Matrix4x3fType* pWorldView,
        const nn::util::Matrix4x3fType& view, const nn::util::Matrix4x3fType& world, const nn::util::Matrix4x3fType& local) NN_NOEXCEPT
    {
        NN_UNUSED(local);
        CalculateWorld(pWorldView, view, world);
    }

    //! @brief ビルボード全種類を同じ引数で呼ぶためのラッパーです。
    //!
    //! @param[out] pWorldView 計算結果を格納するための nn::util::Matrix4x3fType へのポインター。
    //! @param[in] view ビュー変換行列への参照。
    //! @param[in] world ワールド変換行列への参照。
    //! @param[in] local ローカル行列への参照。使用しません。
    //!
    static void CalculateWorldViewpoint(nn::util::Matrix4x3fType* pWorldView,
        const nn::util::Matrix4x3fType& view, const nn::util::Matrix4x3fType& world, const nn::util::Matrix4x3fType& local) NN_NOEXCEPT
    {
        NN_UNUSED(local);
        CalculateWorldViewpoint(pWorldView, view, world);
    }

    //! @brief ビルボード全種類を同じ引数で呼ぶためのラッパーです。
    //!
    //! @param[out] pWorldView 計算結果を格納するための nn::util::Matrix4x3fType へのポインター。
    //! @param[in] view ビュー変換行列への参照。使用しません。
    //! @param[in] world ワールド変換行列への参照。使用しません。
    //! @param[in] local ローカル行列への参照。
    //!
    static void CalculateScreen(nn::util::Matrix4x3fType* pWorldView,
        const nn::util::Matrix4x3fType& view, const nn::util::Matrix4x3fType& world, const nn::util::Matrix4x3fType& local) NN_NOEXCEPT
    {
        NN_UNUSED(view);
        NN_UNUSED(world);
        CalculateScreen(pWorldView, local);
    }

    //! @brief ビルボード全種類を同じ引数で呼ぶためのラッパーです。
    //!
    //! @param[out] pWorldView 計算結果を格納するための nn::util::Matrix4x3fType へのポインター。
    //! @param[in] view ビュー変換行列への参照。
    //! @param[in] world ワールド変換行列への参照。
    //! @param[in] local ローカル行列への参照。使用しません。
    //!
    static void CalculateYAxis(nn::util::Matrix4x3fType* pWorldView,
        const nn::util::Matrix4x3fType& view, const nn::util::Matrix4x3fType& world, const nn::util::Matrix4x3fType& local) NN_NOEXCEPT
    {
        NN_UNUSED(local);
        CalculateYAxis(pWorldView, view, world);
    }

    //! @brief ビルボード全種類を同じ引数で呼ぶためのラッパーです。
    //!
    //! @param[out] pWorldView 計算結果を格納するための nn::util::Matrix4x3fType へのポインター。
    //! @param[in] view ビュー変換行列への参照。
    //! @param[in] world ワールド変換行列への参照。
    //! @param[in] local ローカル行列への参照。使用しません。
    //!
    static void CalculateYAxisViewpoint(nn::util::Matrix4x3fType* pWorldView,
        const nn::util::Matrix4x3fType& view, const nn::util::Matrix4x3fType& world, const nn::util::Matrix4x3fType& local) NN_NOEXCEPT
    {
        NN_UNUSED(local);
        CalculateYAxisViewpoint(pWorldView, view, world);
    }

    //! @brief ビルボードモードに応じてビルボード計算を行います。
    //!
    //! @param[in] billboardMode ビルボードモード
    //! @param[out] pWorldView 計算結果を格納するための nn::util::Matrix4x3fType へのポインター。
    //! @param[in] view ビュー変換行列への参照。
    //! @param[in] world ワールド変換行列への参照。
    //! @param[in] local ローカル行列への参照。
    //!
    //! @details
    //! ビルボードモードは res::ResBone::Flag で指定します。
    //!
    static void Calculate(Bit32 billboardMode, nn::util::Matrix4x3fType* pWorldView,
        const nn::util::Matrix4x3fType& view, const nn::util::Matrix4x3fType& world, const nn::util::Matrix4x3fType& local) NN_NOEXCEPT;
};

}} // namespace nn::g3d

