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
* @brief Mii キャラクターモデルのコンバートクラスの宣言
*/

#pragma once

namespace nn { namespace g3d {
class ResModel;
class ResMaterial;
class ResShape;
}}

namespace nn { namespace mii {
class DrawParam;
class CharModel;
struct BoundingBox;
}}

namespace nn { namespace g3d { namespace mii {

//! @brief          nn::g3d::RenderInfo、nn::g3d::ShaderParam の値を埋めるためのコールバック関数です。
//! @param[out]     pMaterial  変更する nn::g3d::ResMaterial です。
//! @param[in]      pDrawParam pMaterial に対応する pDrawParam です。
//! @param[in]      pMaterialCallbackParam コールバック関数パラメータです。
//! @post           'pMaterial' のメンバに適切な値が入っている。
typedef void( *MaterialCallbackType )( nn::g3d::ResMaterial* pMaterial, const nn::mii::DrawParam* pDrawParam, void* pMaterialCallbackParam );


//! @brief            nn::mii::DrawParamCallback() の引数に渡す型です。
typedef struct DrawParamCallbackOutput
{
    bool isMiiFace;    //!< Mii のパーツである事を示すフラグです。
    const nn::mii::DrawParam* pDrawParam; //!< 入力されたシェイプに対応する nn::mii::DrawParam です。
    const nn::mii::BoundingBox* pBoundingBox;    //!< Mii のバウンディングボックスです。
} DrawParamCallbackOutput;


//! @brief          マテリアル名、ボーン名から nn::mii::DrawParam* を返すコールバック関数です。
//! @param[out]     pDrawParamCallbackOutput コールバック結果出力用の引数です。
//! @param[in]      pModel      対象の nn::g3d::ResModel です。
//! @param[in]      shapeIndex  pModel に対応するシェイプのインデックスです。
//! @param[in]      pCharModel  アサインされる nn::mii::CharModel です。
//! @param[in]      pUserData   コールバック関数パラメータです。
//! @post           'pDrawParamCallbackOutput' のメンバに適切な値が入っている。
typedef void( *DrawParamCallbackType )( DrawParamCallbackOutput* pDrawParamCallbackOutput, const nn::g3d::ResModel* pModel,
                                        const int shapeIndex, const nn::mii::CharModel* pCharModel, void* pDrawParamCallbackParam );


//! @brief          nn::g3d::ResModel に nn::mii::CharModel をアサインします。
//! @param[out]     pResModel                変更する nn::g3d::ResModel です。
//! @param[in]      pCharModel               追加する Mii のキャラクターモデルです。
//! @param[in]      pMaterialCallback        ユーザが独自に定義したシェーダパラメータ、レンダーインフォの値を変更するコールバック関数です。
//! @param[in]      pMaterialCallbackParam   pMaterialCallback の引数として使われるコールバック関数パラメータです。
//! @param[in]      pDrawParamCallback       マテリアル名、ボーン名から適切な nn::mii::DrawParam* を返すコールバック関数です。
//! @param[in]      pDrawParamCallbackParam  pDrawParamCallback の引数として使われるコールバック関数パラメータです。
//! @pre            'pResModel' が有効なポインタである。
//! @pre            'pCharModel' が有効なポインタである。
//! @pre            'pCharModel' が初期化済みである。
//! @pre            'pMaterialCallback' が有効なポインタである。
//! @pre            'pDrawParamCallback' が有効なポインタである。
//! @post           'pCharModel' が 'pResModel' にアサインされる。
void AssignCharModel( nn::g3d::ResModel* pResModel, const nn::mii::CharModel* pCharModel, MaterialCallbackType pMaterialCallback, void* pMaterialCallbackParam,
                        DrawParamCallbackType pDrawParamCallback, void* pDrawParamCallbackParam ) NN_NOEXCEPT;

}}}    // namespace nn::g3d::mii
