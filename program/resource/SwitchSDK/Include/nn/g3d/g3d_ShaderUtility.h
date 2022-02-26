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

#include <nn/g3d/g3d_Configuration.h>

namespace nn { namespace g3d {

class ResMaterial;
class ResShaderAssign;
class ResShadingModel;
class MaterialObj;
class ShadingModelObj;

//! @brief シェーダーユーティリティクラスです。
class ShaderUtility
{
    NN_DISALLOW_COPY(ShaderUtility);

public:

    //! @brief シェーダーアサインに記録された情報を元にシェーダーキーを初期化します。
    //!
    //! @param[out] pKey シェーダーキーを格納するバッファーへのポインター。
    //! @param[in] keyLength シェーダーキー長。
    //! @param[in] pResShadingModel シェーディングモデルリソースへのポインター。
    //! @param[in] pResShaderAssign シェーダー割り当て情報へのポインター。
    //! @param[in] materialPass 警告の有無の指定。
    //!
    //! @details
    //! materialPass を true にするとシェーダーとマテリアルの設定が一致していない場合に
    //! 警告を表示します。
    //!
    static void InitializeShaderKey(Bit32* pKey, int keyLength,
        const ResShadingModel* pResShadingModel, const ResShaderAssign* pResShaderAssign,
        bool materialPass) NN_NOEXCEPT;

    //! @brief シェーダーアサインに記録された情報を元にシェーダーキーを初期化します。
    //!
    //! @param[out] pKey シェーダーキーを格納するバッファーへのポインター。
    //! @param[in] keyLength シェーダーキー長。
    //! @param[in] pResShadingModel シェーディングモデルへのポインター。
    //! @param[in] pResShaderAssign シェーダー割り当て情報へのポインター。
    //!
    static void InitializeShaderKey(Bit32* pKey, int keyLength,
        const ResShadingModel* pResShadingModel, const ResShaderAssign* pResShaderAssign) NN_NOEXCEPT
    {
        InitializeShaderKey(pKey, keyLength, pResShadingModel, pResShaderAssign, true);
    }

    //! @brief シェーダーアサインに記録された情報を元にシェーダーキーを初期化します。
    //!
    //! @param[in] pShadingModelObj シェーディングモデルへのポインター。
    //! @param[in] pResShaderAssign シェーダー割り当て情報へのポインター。
    //! @param[in] materialPass 警告の有無の指定。
    //!
    //! @details
    //! materialPass を true にするとシェーダーとマテリアルの設定が一致していない場合に
    //! 警告を表示します。
    //!
    static void InitializeShaderKey(
        ShadingModelObj* pShadingModelObj, const ResShaderAssign* pResShaderAssign,
        bool materialPass) NN_NOEXCEPT;

    //! @brief シェーダーアサインに記録された情報を元にシェーダーキーを初期化します。
    //!
    //! @param[in] pShadingModelObj シェーディングモデルへのポインター。
    //! @param[in] pResShaderAssign シェーダー割り当て情報へのポインター。
    //!
    static void InitializeShaderKey(
        ShadingModelObj* pShadingModelObj, const ResShaderAssign* pResShaderAssign) NN_NOEXCEPT
    {
        InitializeShaderKey(pShadingModelObj, pResShaderAssign, true);
    }

    //! @brief マテリアルにシェーディングモデルを関連付けます。
    //!
    //! @param[in] pResMaterial マテリアルリソースへのポインター。
    //! @param[in] pResShadingModel シェーディングモデルリソースへのポインター。
    //!
    //! @details
    //! シェーダーパラメータを書き込む UniformBlock のサイズと
    //! 個々のシェーダーパラメータのオフセットをマテリアルに記録します。
    //!
    static void BindShaderParam(ResMaterial* pResMaterial, const ResShadingModel* pResShadingModel) NN_NOEXCEPT;

    //! @brief マテリアルをシェーディングモデルで初期化します。
    //!
    //! @param[in] pMaterialObj マテリアルへのポインター。
    //! @param[in] pResShadingModel シェーディングモデルリソースへのポインター。
    //!
    //! @details
    //! シェーダーパラメータを書き込む UniformBlock の初期値を書き込みます。
    //!
    static void InitializeShaderParam(MaterialObj* pMaterialObj, const ResShadingModel* pResShadingModel) NN_NOEXCEPT;
};

}} // namespace nn::g3d

