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

#include <nn/vfx/vfx_TargetDef.h>

namespace nn {
namespace vfx {

//---------------------------------------------------
//! @brief ビュー情報の管理構造体です。
//---------------------------------------------------
struct ViewParam
{
public:
    static const float      DefaultZOffset;     //!< zOffset の規定値
public:
    nn::util::Float4x4      viewMatrix;         //!< ビュー行列
    nn::util::Float4x4      projectionMatrix;   //!< 射影行列
    nn::util::Float4x4      viewProjMatrix;     //!< ビュー x 射影 行列
    nn::util::Float4x4      billboardMatrix;    //!< ビルボード行列
    nn::util::Float4        eyeVec;             //!< 視線ベクトル
    nn::util::Float4        eyePos;             //!< 視点位置
    nn::util::Float4        viewParam;          //!< near / far / near * far / far  - near /
    nn::util::Float4        viewParam2;         //!< baseFovy / currentFovy / zOffset (Avoid Z-Fighting)
    // 32 byte align is required.

    //---------------------------------------------------------------------------
    //! @brief      構造体に値を設定します。
    //! @param[in]  view            ビュー行列
    //! @param[in]  projection      射影行列
    //! @param[in]  cameraPos       カメラ座標
    //! @param[in]  screenNear      射影変換 Near 値
    //! @param[in]  screenFar       射影変換 Far 値
    //---------------------------------------------------------------------------
    void Set(
        const nn::util::Matrix4x3fType& view,
        const nn::util::Matrix4x4fType& projection,
        const nn::util::Vector3fType&   cameraPos,
        float                           screenNear,
        float                           screenFar ) NN_NOEXCEPT
    {
        // Fovy 関連のパラメータは 1.0f を入れておけば無害
        Set( view, projection, cameraPos, screenNear, screenFar, 1.0f, 1.0f, DefaultZOffset );
    }

    //---------------------------------------------------------------------------
    //! @brief      構造体に値を設定します。
    //! @param[in]  view            ビュー行列
    //! @param[in]  projection      射影行列
    //! @param[in]  cameraPos       カメラ座標
    //! @param[in]  screenNear      射影変換 Near 値
    //! @param[in]  screenFar       射影変換 Far 値
    //! @param[in]  zOffset         Z-Fighting 抑制のための深度オフセット値
    //---------------------------------------------------------------------------
    void Set(
        const nn::util::Matrix4x3fType& view,
        const nn::util::Matrix4x4fType& projection,
        const nn::util::Vector3fType&   cameraPos,
        float                           screenNear,
        float                           screenFar,
        float                           zOffset ) NN_NOEXCEPT
    {
        // Fovy 関連のパラメータは 1.0f を入れておけば無害
        Set( view, projection, cameraPos, screenNear, screenFar, 1.0f, 1.0f, zOffset );
    }

    //---------------------------------------------------------------------------
    //! @brief      構造体に値を設定します。
    //! @param[in]  view            ビュー行列
    //! @param[in]  projection      射影行列
    //! @param[in]  cameraPos       カメラ座標
    //! @param[in]  screenNear      射影変換 Near 値
    //! @param[in]  screenFar       射影変換 Far 値
    //! @param[in]  baseFovy        基準となる画角
    //! @param[in]  currentFovy     現在の画角
    //---------------------------------------------------------------------------
    void Set(
        const nn::util::Matrix4x3fType& view,
        const nn::util::Matrix4x4fType& projection,
        const nn::util::Vector3fType&   cameraPos,
        float                           screenNear,
        float                           screenFar,
        float                           baseFovy,
        float                           currentFovy ) NN_NOEXCEPT
    {
        // Fovy 関連のパラメータは 1.0f を入れておけば無害
        Set( view, projection, cameraPos, screenNear, screenFar, baseFovy, currentFovy, DefaultZOffset );
    }

    //---------------------------------------------------------------------------
    //! @brief      構造体に値を設定します。
    //! @param[in]  view            ビュー行列
    //! @param[in]  projection      射影行列
    //! @param[in]  cameraPos       カメラ座標
    //! @param[in]  screenNear      射影変換 Near 値
    //! @param[in]  screenFar       射影変換 Far 値
    //! @param[in]  baseFovy        基準となる画角
    //! @param[in]  currentFovy     現在の画角
    //! @param[in]  zOffset         Z-Fighting 抑制のための深度オフセット値
    //---------------------------------------------------------------------------
    void Set(
        const nn::util::Matrix4x3fType& view,
        const nn::util::Matrix4x4fType& projection,
        const nn::util::Vector3fType&   cameraPos,
        float                           screenNear,
        float                           screenFar,
        float                           baseFovy,
        float                           currentFovy,
        float                           zOffset ) NN_NOEXCEPT
    {
        //---------------------------------------------------------------------------
        // ※シェーダが列優先実装なので、行列はここで行優先→列優先に転置しておく
        //---------------------------------------------------------------------------

        viewParam2.x = baseFovy;
        viewParam2.y = currentFovy;
        viewParam2.z = zOffset;
        viewParam2.w = 0.0f;

        // Projection Matrix
        nn::util::Matrix4x4fType tempProj4x4f;
        nn::util::MatrixTranspose( &tempProj4x4f, projection );
        nn::util::MatrixStore( &projectionMatrix, tempProj4x4f );

        // View Matrix（3x4から4x3に変換した上で読んでいるので、再度転置する）
        // Proj x View Matrix
        nn::util::Matrix4x4fType tempView4x4f;
        nn::util::Matrix4x4fType tempViewT4x4f;
        nn::util::Matrix4x4fType tempViewProj;
        detail::Matrix4x3fTo4x4f( &tempView4x4f, view );
        nn::util::MatrixTranspose( &tempViewT4x4f, tempView4x4f );
        nn::util::MatrixStore( &viewMatrix, tempViewT4x4f );

        nn::util::MatrixMultiply( &tempViewProj, tempProj4x4f, tempViewT4x4f );
        nn::util::MatrixStore( &viewProjMatrix, tempViewProj );

        // Billboard Matrix
        nn::util::MatrixStore( &billboardMatrix, tempView4x4f );    // 転置前の行列が丁度逆行列になっている
        billboardMatrix.m[ 3 ][ 0 ] = 0;    // 要らない成分を消して、向きだけの情報にする
        billboardMatrix.m[ 3 ][ 1 ] = 0;
        billboardMatrix.m[ 3 ][ 2 ] = 0;
        billboardMatrix.m[ 3 ][ 3 ] = 1;

        // Look Direction
        eyeVec.x = billboardMatrix.m[ 0 ][ 2 ];
        eyeVec.y = billboardMatrix.m[ 1 ][ 2 ];
        eyeVec.z = billboardMatrix.m[ 2 ][ 2 ];
        eyeVec.w = 0;

        // Eye Position
        nn::util::VectorStore( eyePos.v, cameraPos );
        eyePos.w = 0;

        // View Parameters
        viewParam.x = screenNear;
        viewParam.y = screenFar;
        viewParam.z = screenNear * screenFar;
        viewParam.w = screenFar - screenNear;
    }
};

} // namespace vfx
} // namespace nn
