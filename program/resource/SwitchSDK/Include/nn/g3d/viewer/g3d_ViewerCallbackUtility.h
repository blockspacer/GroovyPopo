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

#include "g3d_ViewerCallback.h"

namespace nn { namespace g3d { namespace viewer {

    //! @brief 各イベントを独自拡張をするためのコールバッククラスです。
    class ViewerCallbacks
    {
    public:
        //! @brief デストラクタです。
        virtual ~ViewerCallbacks() NN_NOEXCEPT
        {
        }

        //! @brief ファイルをロード時の処理です。ファイルに対応するオブジェクトのインスタンスを返します。
        virtual void ModelFileLoaded(ModelFileLoadedOutArg& outArg, const ModelFileLoadedArg& inArg) NN_NOEXCEPT
        {
            NN_UNUSED(inArg);
            NN_UNUSED(outArg);
        }

        //! @brief ファイルをアンロード時の処理です。
        virtual void ModelFileUnloaded(const ModelFileUnloadedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief マテリアルの構造が変更された時に実行される処理です。
        virtual void MaterialUpdated(const MaterialUpdatedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief モデルの割り当て情報が変更された時に実行される処理です。
        virtual void ShaderAssignUpdated(const ShaderAssignUpdatedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);

            // 更新前後の対象となる ResModel 情報が arg に格納されています。
            // この処理後に古い ResModel 情報は破棄されます。
            // 古い ResModel 情報をポインタなどで保持し続けないようにしてください。
        }

        //! @brief 描画情報の状態が変更された時に実行される処理です。
        virtual void RenderInfoUpdated(const RenderInfoUpdatedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief 3DEditorから描画情報データが要求された時に実行される処理です。対応する描画情報データを3DEditor側に送信します。
        virtual void SendRenderInfoRequested(const SendRenderInfoRequestedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief ボーンバインドの状態が変更された時に実行される処理です。
        virtual void BoneBindUpdated(const BoneBindUpdatedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief ビューアライブラリ内でシェイプに更新があったときに呼び出されます。
        //!        ビューアライブラリ内でモデルの再構築が必要になった際や 3DEditor で LOD モデル表示切替えが行われた際、
        //!        モデルやシェイプの選択後の点滅時などにシェイプが更新されます。
        virtual void ShapeUpdated(const ShapeUpdatedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief モデル配置の状態が変更された時に実行される処理です。
        virtual void ModelLayoutUpdated(const ModelLayoutUpdatedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief 3DEditorからモデル配置データが要求された時に実行される処理です。対応するモデル配置データを3DEditor側に送信します。
        virtual void SendModelLayoutRequested(SendModelLayoutRequestedOutArg& outArg, const SendModelLayoutRequestedArg& inArg) NN_NOEXCEPT
        {
            // このコールバックを有効にする場合は isEnabled に true を指定してください
            outArg.isEnabled = false;
            NN_UNUSED(inArg);
        }

        //! @brief 3DEditorからシーンアニメーションをバインド対象に設定した時に実行される処理です。
        virtual void SceneAnimBound(const SceneAnimBoundArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief 3DEditorからシーンアニメーションをバインド対象から外した時に実行される処理です。
        virtual void SceneAnimUnbound(const SceneAnimUnboundArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief バインド対象に設定されているシーンアニメーションの計算結果を反映させる時に実行される処理です。
        //!        このタイミングで各アニメーションの計算結果を適用したい対象に設定する事ができます。
        virtual void ApplySceneAnimRequested(const ApplySceneAnimRequestedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief 3DEditorからマテリアルなどが選択された時に実行される処理です。
        //!        モデルの選択の場合には TargetSelectedArg の index は常に nullptr になります。
        //!        TargetSelectedArg の index の指している内容はこのコールバックが終了した時点で無効になります。
        //!        遅延処理を行う場合などは index の内容をコピーして下さい。
        virtual void TargetSelected(const TargetSelectedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief 3DEditor でモデルに対するユーザスクリプトが実行された時に呼ばれる処理です。
        virtual void ModelUserScriptExecuted(const ModelUserScriptExecutedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief モデルがアタッチされた時に実行される処理です。
        virtual void ModelAttached(const ModelAttachedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief モデルがデタッチされた時に実行される処理です。
        virtual void ModelDetached(const ModelDetachedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief シェーダがアタッチされた後に実行される処理です。
        virtual void ShaderAttached(const ShaderAttachedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief シェーダがデタッチされた後に実行される処理です。
        virtual void ShaderDetached(const ShaderDetachedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief サンプラのパラメータが変更された時に実行される処理です。
        virtual void SamplerParamUpdated(const SamplerParamUpdatedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief テクスチャーファイルが 3DEditor で読み込まれた時に実行される処理です。
        virtual void TextureFileLoaded(const TextureFileLoadedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief テクスチャーファイルが 3DEditor でクローズされた時に実行される処理です。
        virtual void TextureFileUnloaded(const TextureFileUnloadedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief モデルのテクスチャーバインドが更新されたときに実行される処理です。
        virtual void ModelTextureBindingsUpdated(const ModelTextureBindingsUpdatedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @brief アタッチシェーダ時、遅延コンパイル後にシェーダプログラムを変更した後に実行される処理です。
        virtual void ShaderProgramUpdated(const ShaderProgramUpdatedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @briefprivate アタッチシェーダ時、3DEditor からシェーディングモデルが編集されたときに実行される処理です。
        virtual void ShadingModelUpdated(const ShadingModelUpdatedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

        //! @briefprivate クライアントからカメラの更新が要求されたときに実行される処理です。
        virtual void CameraUpdated(const CameraUpdatedArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
        }

    protected:
        //! @brief コンストラクタです。
        ViewerCallbacks() NN_NOEXCEPT
        {
        }
    };
}}} // namespace nn::g3d


