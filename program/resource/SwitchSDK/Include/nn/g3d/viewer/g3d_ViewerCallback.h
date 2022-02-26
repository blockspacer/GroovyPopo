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

#include <nn/g3d/viewer/g3d_ViewerDefine.h>



#include <nn/util/util_MathTypes.h>

namespace nn {
namespace gfx {
    class ResTextureFile;
}
namespace g3d {

    class ResFile;
    class ModelObj;
    class ResModel;
    class ResMaterialAnim;
    class ResShaderArchive;
    class CameraAnimObj;
    class LightAnimObj;
    class FogAnimObj;

    namespace viewer {

        //! @brief サーバーのコールバックの種類です。
        enum CallbackType
        {
            // モデル関連 ======================================

            //! @brief 3DEditor からモデルリソースのロード処理が要求された際に呼び出されます。
            //!        ゲームアプリケーションはリソースファイルをコピーして受け取り、管理及び描画する必要があります。
            //!        nn::g3d::viewer::ViewerServer::QueueAttachModelCommand() を呼び出すことで、編集対象として登録することができます。
            CallbackType_ModelFileLoaded,

            //! @brief 3DEditor からモデルリソースのアンロード処理が要求された際に呼び出されます。
            //!        ゲームアプリケーションからそのリソースファイルが不要になった段階で破棄することができます。
            CallbackType_ModelFileUnloaded,

            CallbackType_ModelAttached, //!< モデルがアタッチされた後にコールされます。

            CallbackType_ModelDetached, //!< モデルがデタッチされた後にコールされます。

            //! @brief 3DEditor でマテリアルのシェーダ関連付け変更やサンプラーの追加・削除など、
            //!        モデルリソースの構造変更を伴う編集を行った場合に呼び出されます。
            //!        ビューアイブラリが動的にモデルリソースを変更するため、モデルリソースに関連付けて
            //!        データを持たせる必要がある場合はここで構築や破棄を行うことができます。
            CallbackType_ShaderAssignUpdated,

            //! @brief モデルインスタンスのマテリアル更新後にコールされます。
            //!        対象モデルのマテリアル更新処理を記述します。
            //!        このコールバックが呼ばれる時点で、対象モデルの保有する nn::g3d::MaterialObj から取得できる
            //!        ResMaterial は更新後のリソースに置き換わっています。
            CallbackType_MaterialUpdated,

            //! @brief 3DEditor でサンプラのパラメータが変更された場合に呼び出されます。
            //!        サンプラのパラメータの変更によるディスプレイリストの再構築や
            //!        ディスクリプタプールへのサンプラの再登録などを行うことができます。
            CallbackType_SamplerParamUpdated,

            //! @brief 3DEditor で描画情報の編集を行った場合に呼び出されます。
            //!        対象となるモデル、マテリアル、描画情報からゲームアプリケーション側に反映させる処理を、ここで行うことができます。
            CallbackType_RenderInfoUpdated,

            //! @brief 3DEditor からゲームアプリケーションに描画情報定義の送信要求があった場合に呼び出されます。
            //!        3DEditor が必要とする指定マテリアルの描画情報定義をこのコールバックで送信します。
            CallbackType_SendRenderInfoRequested,

            //! @brief 3DEditor のモデルプレビュー機能を使用してバインド情報に編集を行った場合に呼び出されます。
            //!        対象となるモデルと親になるモデルから指定されたボーンにバインドさせる処理を、ここで行うことができます。
            CallbackType_BoneBindUpdated,

            //! @brief ビューアライブラリ内でシェイプに更新があったときに呼び出されます。
            //!        ビューアライブラリ内でモデルの再構築が必要になった際や 3DEditor で LOD モデル表示切替えが行われた際、
            //!        モデルやシェイプの選択後の点滅時などにシェイプが更新されます。
            CallbackType_ShapeUpdated,

            //! @brief 3DEditorのモデルプレビュー機能を使用して配置情報に編集を行った場合に呼び出されます。
            //!        対象となるモデルと配置情報からゲームアプリケーション側に反映させる処理を、ここで行うことができます。
            CallbackType_ModelLayoutUpdated,

            //! @brief ゲームアプリケーション側で、3DEditor が必要とする配置情報を送信する場合に呼び出されます。
            //!        指定されたモデルの位置情報を 3DEditor に反映させる処理を、ここで行うことができます。
            CallbackType_SendModelLayoutRequested,

            //! @brief 3DEditor でモデル、マテリアル、ボーン、シェイプが選択された場合に呼び出されます。
            //!        選択に応じたビューアの処理などを、ここで行うことができます。
            CallbackType_TargetSelected,

            //! @brief 3DEditor でモデルに対してユーザースクリプトが実行されたときに呼び出されます。
            CallbackType_ModelUserScriptExecuted,

            CallbackType_ModelTextureBindingsUpdated, //!< @brief モデルのテクスチャーバインドが更新されたときに呼び出されます。

            // テクスチャー関連 ======================================

            CallbackType_TextureFileLoaded, //!< @brief テクスチャーファイルがロードされたときに呼び出されます。

            CallbackType_TextureFileUnloaded, //!< @brief テクスチャーファイルがアンロードされたときに呼び出されます。

            // アニメーション関連 ======================================

            //! @brief 3DEditor でシーンアニメーションをバインド対象に設定した場合に呼び出されます。
            //!        対象となるカメラアニメーション、ライトアニメーション、フォグアニメーションを
            //!        ゲームアプリケーションと関連付ける処理を、ここで行うことができます。
            CallbackType_SceneAnimBound,

            //! @brief 3DEditor でシーンアニメーションをバインド対象から外した場合に呼び出されます。
            //!        対象となるカメラアニメーション、ライトアニメーション、フォグアニメーションをゲームアプリケーションと関連付けを外す処理を、
            //!        ここで行うことができます。SceneAnimBound と対で処理を実装する事を推奨します。
            CallbackType_SceneAnimUnbound,

            //! @brief SceneAnimBound でバインド対象となったシーンアニメーションの計算結果を適用する処理を、ここで行うことができます。
            //!        SceneAnimBound でバインド対象となったシーンアニメーションのみ、この処理が実行されます。
            CallbackType_ApplySceneAnimRequested,

            // シェーダー関連 ======================================

            //! @brief 3DEditor でシェーダがアタッチされた直後に呼び出されます。
            //!        例えば、ここで描画用のシェーダをアタッチされたものに切り替えることなどができます。
            CallbackType_ShaderAttached,

            //! @brief 3DEditor でシェーダがデタッチされた直後に呼び出されます。
            //!        例えば、ここで描画用のシェーダをアタッチされていたものから通常のものに切り替えることなどができます。
            CallbackType_ShaderDetached,

            CallbackType_ShaderProgramUpdated, //!< アタッチシェーダ時、遅延コンパイル後にシェーダプログラムを変更した後に実行される処理です。

            // 内部開発用 ======================================

            CallbackType_ShadingModelUpdated, //!< @briefprivate アタッチシェーダ時、3DEditor からシェーディングモデルが編集されたときに実行される処理です。
            CallbackType_CameraUpdated, //!< @briefprivate クライアントからカメラのトランスフォーム等の更新が要求されたときに実行されます。
        };

        //! @brief サーバーに登録するためのコールバック関数です。
        typedef void (*ViewerCallback)(void* pOutArg, const void* pInArg, CallbackType type, void* pUserData);

        //! @brief ModelFileLoaded コールバック実行時に渡される構造体です。
        struct ModelFileLoadedArg
        {
            nn::g3d::ResFile* pResFile; //!< @brief 3DEditor から送信された ResFile です。
            size_t fileSize; //!< @brief ファイルサイズです。
            size_t alignment; //!< @brief ファイルに必要なアライメントです。
        };

        //! @brief ModelFileLoaded コールバック実行時の出力引数として渡される構造体です。
        struct ModelFileLoadedOutArg
        {
            nn::g3d::ResFile* pResFile; //!< @brief コールバック内で入力引数から取得した ResFile をコピーした ResFile を渡します。
        };

        //! @brief UnloadFileコールバック実行時に渡される構造体です。
        struct ModelFileUnloadedArg
        {
            nn::g3d::ResFile* pResFile; //!< アンロード対象の ResFile です。
        };

        //! @brief RenderInfoUpdated コールバック実行時に渡される構造体です。
        struct RenderInfoUpdatedArg
        {
            const nn::g3d::ModelObj* pModelObj;   //!< 3DEditor が編集した対象の ModelObj です。
            int materialIndex;          //!< 3DEditorが編集した対象の ModelObj のマテリアルインデックスです。
            const char* name;           //!< 編集対象の描画情報名です。
        };

        //! @brief SendRenderInfoRequested コールバック実行時に渡される構造体です。
        struct SendRenderInfoRequestedArg
        {
            const nn::g3d::ModelObj* pModelObj;   //!< 3DEditor が情報を要求している対象の ModelObj です。
            int materialIndex;          //!< 3DEditor が情報を要求している対象の ModelObj のマテリアルインデックスです。
        };

        //! @brief MaterialUpdated コールバック実行時に渡される構造体です。
        struct MaterialUpdatedArg
        {
            //! @brief Stateを示すenumです。
            enum State
            {
                State_Begin,    //!< 初期状態時に格納される値です。
                State_Update,   //!< 更新が行われた時に格納される値です。
                State_End,      //!< 終了状態時に格納される値です。
                State_Error //!< 通常は格納されることはありません。
            };

            nn::g3d::ModelObj*   pModelObj;   //!< マテリアル更新対象の ModelObj です。
            State state;      //!< 更新状態です。
        };

        //! @brief ShaderAssignUpdated コールバック実行時に渡される構造体です。
        struct ShaderAssignUpdatedArg
        {
            //! @brief Stateを示すenumです。
            enum State
            {
                State_ModelBegin,              //!< モデル初期状態時に格納される値です。
                State_ModelUpdate,             //!< モデル更新が行われた時に格納される値です。
                State_ModelEnd,                //!< モデル終了状態時に格納される値です。
                State_Error //!< 通常は格納されることはありません。
            };

            //! @brief マテリアルに関するシェーダ情報です。
            struct MaterialShaderInfo
            {
                //! @brief 対象マテリアルに対応するシェーダアーカイブです。
                //!        3DEditor でシェーダが更新されなかった場合は nullptr が格納されます。
                nn::g3d::ResShaderArchive* pResShaderArchive;

                //! @brief 対象マテリアルがシェーダ最適化処理に含まれているかどうかを取得します。
                bool isOptimized;

                //! @brief 対象マテリアルの最適化シェーダ作成がスキップされたかどうかを取得します。
                //!        3DEditor で対象マテリアルの「マテリアルの変更に追従してシェーダを再生成する」がオフのときに、
                //!        他のマテリアルの変更による 3DEditor のシェーダ再生成処理が行われたときに、このマテリアルに対するシェーダー再生成処理は
                //!        スキップされるため、このフラグは true になります。
                bool isOptimizationSkipped;

                MaterialShaderInfo() NN_NOEXCEPT
                    : pResShaderArchive(nullptr)
                    , isOptimized(false)
                    , isOptimizationSkipped(false)
                {
                }
            };

            nn::g3d::ModelObj* pModelObj;    //!< 更新対象の ModelObj です。
            nn::g3d::ResModel* pOldResModel; //!< 適用前の ResModel です。
            nn::g3d::ResModel* pNewResModel; //!< 更新時に適用される ResModel です。

            //! @brief 各マテリアルに対する変更されたシェーダアーカイブの配列です。
            //!        配列の長さはマテリアル数と一致します。nullptr のときは変更されたシェーダアーカイブがありません。
            //! @deprecated こちらのメンバ変数は廃止予定です。代わりに pMaterialShaderInfos を使用して下さい。
            nn::g3d::ResShaderArchive** pShaderArchives;

            //! @brief 3DEditor から送信された各マテリアルのシェーダ情報の配列です。
            //!        配列の長さはマテリアル数と一致します。
            MaterialShaderInfo* pMaterialShaderInfos;

            State state; //!< 更新状態です。
        };

        //! @brief BoneBindUpdated コールバック実行時に渡される構造体です。
        struct BoneBindUpdatedArg
        {
            nn::g3d::ModelObj* pParentModelObj;   //!< バインド対象の親 nn::g3d::ModelObj です。バインドされない場合は nullptr が格納されます。
            int parentBoneIndex;        //!< バインド対象の親 nn::g3d::ModelObj のボーンインデックスです。
            nn::g3d::ModelObj* pModelObj;         //!< バインド対象の nn::g3d::ModelObj です。
        };

        //! @brief ModelLayoutUpdated コールバック実行時に渡される構造体です。
        struct ModelLayoutUpdatedArg
        {
            nn::g3d::ModelObj* pModelObj; //!< モデル配置対象の ModelObj です。
            nn::util::Vector3fType scale; //!< モデル配置対象に設定する Scale です。
            nn::util::Vector3fType rotate; //!< モデル配置対象に設定する Rotate です。
            nn::util::Vector3fType translate; //!< モデル配置対象に設定する Translate です。
        };

        //! @brief SendModelLayoutRequested コールバック実行時に渡される構造体です。
        struct SendModelLayoutRequestedArg
        {
            const nn::g3d::ModelObj* pModelObj; //!< モデル配置情報を送信する対象のModelObj です。
        };

        //! @brief SendModelLayoutRequested コールバック実行後に送信される配置情報を格納する構造体です。
        struct SendModelLayoutRequestedOutArg
        {
            nn::util::Vector3fType scale; //!< 3DEditor に反映させるモデル配置情報のScaleです。
            nn::util::Vector3fType rotate; //!< 3DEditor に反映させるモデル配置情報のRotateです。
            nn::util::Vector3fType translate; //!< 3DEditor に反映させるモデル配置情報のTranslateです。
            bool isEnabled; //!< 3DEditor にモデル配置情報を反映させる場合は true、そうでない場合は false を指定します。
        };

        //! @brief SceneAnimBound コールバック実行時に渡される構造体です。
        struct SceneAnimBoundArg
        {
            nn::g3d::CameraAnimObj**    pCameraAnimObjs; //!< バインド対象のカメラアニメーションのポインタ配列です。
            nn::g3d::LightAnimObj**     pLightAnimObjs;  //!< バインド対象のライトアニメーションのポインタ配列です。
            nn::g3d::FogAnimObj**       pFogAnimObjs;    //!< バインド対象のフォグアニメーションのポインタ配列です。
            int                         cameraAnimObjCount;  //!< バインド対象のカメラアニメーションのポインタ配列の要素数です。
            int                         lightAnimObjCount;   //!< バインド対象のライトアニメーションのポインタ配列の要素数です。
            int                         fogAnimObjCount;     //!< バインド対象のフォグアニメーションのポインタ配列の要素数です。
        };

        //! @brief SceneAnimUnboundコールバック実行時に渡される構造体です。
        struct SceneAnimUnboundArg
        {
            nn::g3d::CameraAnimObj**    pCameraAnimObjs; //!< アンバインド対象のカメラアニメーションのポインタ配列です。
            nn::g3d::LightAnimObj**     pLightAnimObjs;  //!< アンバインド対象のライトアニメーションのポインタ配列です。
            nn::g3d::FogAnimObj**       pFogAnimObjs;    //!< アンバインド対象のフォグアニメーションのポインタ配列です。
            int                         cameraAnimObjCount;  //!< アンバインド対象のカメラアニメーションのポインタ配列の要素数です。
            int                         lightAnimObjCount;   //!< アンバインド対象のライトアニメーションのポインタ配列の要素数です。
            int                         fogAnimObjCount;     //!< アンバインド対象のフォグアニメーションのポインタ配列の要素数です。
        };

        //! @brief ApplySceneAnimRequestedコールバック実行時に渡される構造体です。
        struct ApplySceneAnimRequestedArg
        {
            nn::g3d::CameraAnimObj**    pCameraAnimObjs; //!< アニメーション適用対象のカメラアニメーションのポインタ配列です。
            nn::g3d::LightAnimObj**     pLightAnimObjs;  //!< アニメーション適用対象のライトアニメーションのポインタ配列です。
            nn::g3d::FogAnimObj**       pFogAnimObjs;    //!< アニメーション適用対象のフォグアニメーションのポインタ配列です。
            int                         cameraAnimObjCount;  //!< アニメーション適用対象のカメラアニメーションのポインタ配列の要素数です。
            int                         lightAnimObjCount;   //!< アニメーション適用対象のライトアニメーションのポインタ配列の要素数です。
            int                         fogAnimObjCount;     //!< アニメーション適用対象のフォグアニメーションのポインタ配列の要素数です。
        };

        //! @brief TargetSelectedコールバック実行時に渡される構造体です。
        struct TargetSelectedArg
        {
            //! @brief 選択対象の種類です。
            enum TargetKind
            {
                TargetKind_Model,      //!< モデルです。
                TargetKind_Material,   //!< マテリアルです。
                TargetKind_Bone,       //!< ボーンです。
                TargetKind_Shape       //!< シェイプです。
            };

            TargetKind      targetKind;                   //!< 3DEditorで選択された対象の種類です。
            const nn::g3d::ModelObj* pModelObj;                     //!< 3DEditorで選択された対象のModelObjです。
            int*            index;                        //!< 3DEditorで選択されたマテリアル、ボーン、シェイプのインデックスの配列です。
            int             indexCount;                    //!< 3DEditorで選択されたマテリアル、ボーン、シェイプのインデックスの数です。
        };

        //! @brief ShaderAttached コールバック実行時に渡される構造体です。
        struct ShaderAttachedArg
        {
            nn::g3d::ResShaderArchive* pShaderArchive; //! アタッチされたシェーダアーカイブへのポインタです。
        };

        //! @brief ShaderDetached コールバック実行時に渡される構造体です。
        struct ShaderDetachedArg
        {
            nn::g3d::ResShaderArchive* pShaderArchive; //! デタッチされるシェーダアーカイブへのポインタです。
        };

        //! @brief ModelAttached コールバック実行時に渡される構造体です。
        struct ModelAttachedArg
        {
            nn::g3d::ModelObj* pModelObj; //!< アタッチされたModelObjです。
        };

        //! @brief ModelDetached コールバック実行時に渡される構造体です。
        struct ModelDetachedArg
        {
            nn::g3d::ModelObj* pModelObj; //!< デタッチされたModelObjです。
        };

        //! @brief ShaderProgramUpdated コールバック実行時に渡される構造体です。
        struct ShaderProgramUpdatedArg
        {
            nn::g3d::ResShaderArchive* pShaderArchive;        //!< 更新対象のシェーダアーカイブです。
            int                     shadingModelIndex;    //!< 更新対象のシェーダアーカイブのシェーディングモデルインデックスです。
            int                     shaderProgramIndex;   //!< 更新対象のシェーダアーカイブのシェーダプログラムインデックスです。
        };

        //! @brief TextureFileLoaded コールバック実行時に渡される構造体です。
        struct TextureFileLoadedArg
        {
            nn::gfx::ResTextureFile* pResTextureFile; //!< ロードされたテクスチャーファイルです。
        };

        //! @brief TextureFileUnloaded コールバック実行時に渡される構造体です。
        struct TextureFileUnloadedArg
        {
            nn::gfx::ResTextureFile* pResTextureFile; //!< アンロードされたテクスチャーファイルです。
        };

        //! @brief ModelTextureBindingsUpdated コールバック実行時に渡される構造体です。
        struct ModelTextureBindingsUpdatedArg
        {
            nn::g3d::ModelObj* pModelObj; //!< テクスチャーバインドが更新されたモデルです。
        };

        //! @brief SamplerParamUpdated コールバック実行時に渡される構造体です。
        struct SamplerParamUpdatedArg
        {
            ModelObj* pModelObj; //!< サンプラパラメータが更新されたモデルです。
        };

        //! @brief ModelUserScriptExecuted コールバック実行時に渡される構造体です。
        struct ModelUserScriptExecutedArg
        {
            nn::g3d::ModelObj* pTargetModelObj; //!< スクリプト適用対象のモデルです。
            const char* scriptText; //!< スクリプト文字列です。
            const int* pSelectedMaterialIndices; //!< 選択されているマテリアルインデックスのリストです。
            const int* pSelectedShapeIndices; //!< 選択されているシェイプインデックスのリストです。
            const int* pSelectedBoneIndices; //!< 選択されているボーンインデックスのリストです。
            int selectedMaterialCount; //!< 選択されているマテリアルインデックスのリストの要素数です。
            int selectedShapeCount; //!< 選択されているシェイプインデックスのリストの要素数です。
            int selectedBoneCount; //!< 選択されているボーンインデックスのリストの要素数です。
        };

        //! @brief ShapeUpdated コールバック実行時に渡される構造体です。
        struct ShapeUpdatedArg
        {
            ModelObj* pModelObj; //!< シェイプが更新されたモデルです。
        };

        //! @briefprivate ShadingModelUpdated コールバック実行時に渡される構造体です。
        struct ShadingModelUpdatedArg
        {
            nn::g3d::ResShaderArchive* pShaderArchive; //!< 更新対象のシェーダアーカイブです。
            int* shadingModelIndices; //!< 更新対象のシェーダアーカイブのシェーディングモデルインデックスの配列です。
            int shadingModelCount; //! < 更新対象のシェーダアーカイブのシェーディングモデルインデックスの数です。
        };

        //! @briefprivate CameraUpdated コールバック実行時に渡される構造体です。
        struct CameraUpdatedArg
        {
            nn::util::Vector4fType rotate; //!< カメラに設定する回転です。
            nn::util::Vector3fType translate; //!< カメラに設定する移動です。
        };
    }
}} // namespace nn::g3d


