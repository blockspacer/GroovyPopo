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

//NOLINT_FILE(style)

#include <nn/vfx/vfx_EndianUtil.h>

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

//------------------------------------------------------------------------------
//! @brief      カメラリソース
//------------------------------------------------------------------------------
struct ResCamera
{
    uint8_t                                           isRealtimeEditing;                                        //!< リアルタイム編集かどうかを判別するためのフラグです。ランタイム側のみで使用します。
    uint8_t                                           displayDebugInformation;                                  //!< デバッグ情報表示のon/off
    uint8_t                                           displayGpuProfile;                                        //!< GPU処理の計測結果を表示on/off
    uint8_t                                           reserved0[1];                                             //!< アライン用
    uint32_t                                          gridType;                                                 //!< グリッドタイプ
    int32_t                                           gridSize;                                                 //!< グリッドのサイズ
    int32_t                                           gridDivisionNumber;                                       //!< グリッドの分割数
    int32_t                                           glareLevel;                                               //!< グレアの強さ
    float                                             frameRate;                                                //!< フレームレート
    nn::util::Float3                                  position;                                                 //!< カメラ位置です
    nn::util::Float3                                  target;                                                   //!< カメラの注視点です
    float                                             rotation;                                                 //!< カメラの回転角です
    int32_t                                           projectionType;                                           //!< 射影タイプです
    float                                             aspectRatio;                                              //!< アスペクト比です
    float                                             fieldOfView;                                              //!< パースペクティブ時の垂直画角です
    float                                             imageHeight;                                              //!< オルソ時の画像の高さ
    float                                             nearClip;                                                 //!< 近クリップ平面への距離です
    float                                             farClip;                                                  //!< 遠クリップ平面への距離です
    nn::util::Float4                                  bgColor;                                                  //!< 背景色
    uint32_t                                          guid;                                                     //!< 背景画像テクスチャID
    nn::util::Float4                                  imageColor;                                               //!< 背景画像への乗算カラー
    uint8_t                                           enablePad;                                                //!< Padの有効化/無効化
    bool                                              displayGrid;                                              //!< グリッド表示のon/off
    bool                                              displayParentEmitterShape;                                //!< 親エミッタ形状表示のon/off

    bool                                              displayChildEmitterShape;                                 //!< 子エミッタ形状表示のon/off
    bool                                              displayFieldShape;                                        //!< フィールド形状表示のon/off
    bool                                              restrainViewerWarnings;                                   //!< ビューアの警告を抑制
    bool                                              enableOverDrawMode;                                       //!< パーティクルの重なりチェック
    uint8_t                                           reserved1[1];                                             //!< アライン用

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &gridType );
        nn::vfx::detail::EndianUtil::Flip( &gridSize );
        nn::vfx::detail::EndianUtil::Flip( &gridDivisionNumber );
        nn::vfx::detail::EndianUtil::Flip( &glareLevel );
        nn::vfx::detail::EndianUtil::Flip( &frameRate );
        nn::vfx::detail::EndianUtil::Flip( &position );
        nn::vfx::detail::EndianUtil::Flip( &target );
        nn::vfx::detail::EndianUtil::Flip( &rotation );
        nn::vfx::detail::EndianUtil::Flip( &projectionType );
        nn::vfx::detail::EndianUtil::Flip( &aspectRatio );
        nn::vfx::detail::EndianUtil::Flip( &fieldOfView );
        nn::vfx::detail::EndianUtil::Flip( &imageHeight );
        nn::vfx::detail::EndianUtil::Flip( &nearClip );
        nn::vfx::detail::EndianUtil::Flip( &farClip );
        nn::vfx::detail::EndianUtil::Flip( &bgColor );
        nn::vfx::detail::EndianUtil::Flip( &guid );
        nn::vfx::detail::EndianUtil::Flip( &imageColor );

    }
};

//------------------------------------------------------------------------------
//! @brief      コンフィグ設定データです。
//------------------------------------------------------------------------------
struct ResConfig
{
    int32_t                                           frameRate;                                                //!< フレームレートです。
    int32_t                                           resolution;                                               //!< 解像度です。
    nn::util::Float4                                  clearColor;                                               //!< 背景カラーです。
    float                                             worldScaleRange;                                          //!< ワールドスケール範囲です。
    float                                             worldScaleTime;                                           //!< ワールドスケール時間です。
    float                                             gridScale;                                                //!< グリッドスケールです。
    int32_t                                           isLinearEditMode;                                         //!< リニア編集モードかどうかを示すフラグです。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &frameRate );
        nn::vfx::detail::EndianUtil::Flip( &resolution );
        nn::vfx::detail::EndianUtil::Flip( &clearColor );
        nn::vfx::detail::EndianUtil::Flip( &worldScaleRange );
        nn::vfx::detail::EndianUtil::Flip( &worldScaleTime );
        nn::vfx::detail::EndianUtil::Flip( &gridScale );
        nn::vfx::detail::EndianUtil::Flip( &isLinearEditMode );

    }
};

//------------------------------------------------------------------------------
//! @brief      再生範囲設定データです。
//------------------------------------------------------------------------------
struct ResPlayRange
{
    int32_t                                           startFrame;                                               //!< 開始フレームです。
    int32_t                                           endFrame;                                                 //!< 終了フレームです。
    int32_t                                           isLoop;                                                   //!< ループさせるかどうかのフラグです。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &startFrame );
        nn::vfx::detail::EndianUtil::Flip( &endFrame );
        nn::vfx::detail::EndianUtil::Flip( &isLoop );

    }
};

//------------------------------------------------------------------------------
//! @brief      コンストレインデータです。
//------------------------------------------------------------------------------
struct ResConstrain
{
    int32_t                                           matrixSetType;                                            //!< 行列設定タイプです。
    int32_t                                           matrixApplyType;                                          //!< マトリクス適用タイプです。
    nn::vfx::viewer::detail::Guid                     modelGuid;                                                //!< 接続モデルです。
    int32_t                                           boneIndex;                                                //!< 接続ボーンです。
    nn::util::Float3                                  offsetPosition;                                           //!< オフセット行列 座標値です。
    nn::util::Float3                                  offsetScale;                                              //!< オフセット行列 スケール値です。
    float                                             offsetRotateX;                                            //!< オフセット行列 回転Xです。
    float                                             offsetRotateY;                                            //!< オフセット行列 回転Yです。
    float                                             offsetRotateZ;                                            //!< オフセット行列 回転Zです。
    int32_t                                           isEmitterBillboard;                                       //!< エミッタビルボードです。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &matrixSetType );
        nn::vfx::detail::EndianUtil::Flip( &matrixApplyType );
        nn::vfx::detail::EndianUtil::Flip( &boneIndex );
        nn::vfx::detail::EndianUtil::Flip( &offsetPosition );
        nn::vfx::detail::EndianUtil::Flip( &offsetScale );
        nn::vfx::detail::EndianUtil::Flip( &offsetRotateX );
        nn::vfx::detail::EndianUtil::Flip( &offsetRotateY );
        nn::vfx::detail::EndianUtil::Flip( &offsetRotateZ );
        nn::vfx::detail::EndianUtil::Flip( &isEmitterBillboard );

    }
};

//------------------------------------------------------------------------------
//! @brief      プレビュー基本設定データです。
//------------------------------------------------------------------------------
struct ResPreviewFnd
{
    int32_t                                           startFrame;                                               //!< 再生開始フレームです。
    int32_t                                           forceStartFrame;                                          //!< 強制開始フレームです。
    bool                                              isManualEmitter;                                          //!< マニュアルエミッタ放出をします。
    uint8_t                                           dummy[3];                                                 //!< パディング領域
    int32_t                                           emitterInverval;                                          //!< マニュアルエミッタ放出間隔です。
    int32_t                                           maxEmitCountPerFrame;                                     //!< 1フレーム中に放出予約可能なパーティクルの最大数です。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &startFrame );
        nn::vfx::detail::EndianUtil::Flip( &forceStartFrame );
        nn::vfx::detail::EndianUtil::Flip( &emitterInverval );
        nn::vfx::detail::EndianUtil::Flip( &maxEmitCountPerFrame );

    }
};

//------------------------------------------------------------------------------
//! @brief      カラーデータです。
//------------------------------------------------------------------------------
struct ResPreviewColor
{
    nn::util::Float4                                  color;                                                    //!< カラーです。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &color );

    }
};

//------------------------------------------------------------------------------
//! @brief      パーティクル放出データです。
//------------------------------------------------------------------------------
struct ResPreviewEmission
{
    float                                             emissionRate;                                             //!< 放出レートです。
    float                                             emissionInterval;                                         //!< 放出間隔です。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &emissionRate );
        nn::vfx::detail::EndianUtil::Flip( &emissionInterval );

    }
};

//------------------------------------------------------------------------------
//! @brief      パーティクルスケールデータです。
//------------------------------------------------------------------------------
struct ResPreviewPtclScale
{
    nn::util::Float3                                  particleScale;                                            //!< パーティクルの大きさです。
    nn::util::Float3                                  emissionParticleScale;                                    //!< 放出時のみ作用するパーティクルの大きさ。
    nn::util::Float3                                  emitterVolumeScale;                                       //!< エミッタ形状スケールです。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &particleScale );
        nn::vfx::detail::EndianUtil::Flip( &emissionParticleScale );
        nn::vfx::detail::EndianUtil::Flip( &emitterVolumeScale );

    }
};

//------------------------------------------------------------------------------
//! @brief      パーティクル制御データです。
//------------------------------------------------------------------------------
struct ResPreviewPtclControl
{
    float                                             life;                                                     //!< ライフスケールです。
    float                                             allDirectionVel;                                          //!< 全方向初速スケールです。
    float                                             directionalVel;                                           //!< 指定方向初速スケールです。
    float                                             randomVel;                                                //!< 初速度のランダムスケールです。
    nn::util::Float3                                  addVel;                                                   //!< 速度に加算するワールドベクトルです。
    nn::util::Float3                                  emitterColor0;                                            //!< エミッタカラー0です。
    nn::util::Float3                                  emitterColor1;                                            //!< エミッタカラー1です。
    uint8_t                                           enableEmitterColor;                                       //!< エミッタカラー有効かどうかのフラグです。
    uint8_t                                           dummy0;                                                   //!< パディング領域
    uint8_t                                           dummy1[2];                                                //!< パディング領域

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &life );
        nn::vfx::detail::EndianUtil::Flip( &allDirectionVel );
        nn::vfx::detail::EndianUtil::Flip( &directionalVel );
        nn::vfx::detail::EndianUtil::Flip( &randomVel );
        nn::vfx::detail::EndianUtil::Flip( &addVel );
        nn::vfx::detail::EndianUtil::Flip( &emitterColor0 );
        nn::vfx::detail::EndianUtil::Flip( &emitterColor1 );

    }
};

//------------------------------------------------------------------------------
//! @brief      自動移動データです。
//------------------------------------------------------------------------------
struct ResPreviewAutoMove
{
    int32_t                                           moveType;                                                 //!< 自動移動タイプです。
    float                                             radiusXZ;                                                 //!< XZ移動半径です。
    float                                             speedXZ;                                                  //!< XZ移動速度です。
    float                                             distance;                                                 //!< 移動距離
    float                                             moveSpeed;                                                //!< 移動速度
    float                                             amplitudeY;                                               //!< Y揺れ振幅です。
    float                                             speedY;                                                   //!< Y揺れ速度です。
    float                                             rollZ;                                                    //!< Zロール速度です。
    float                                             straightDirection;                                        //!< 水平方向角度
    float                                             straightAngle;                                            //!< 垂直方向角度

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &moveType );
        nn::vfx::detail::EndianUtil::Flip( &radiusXZ );
        nn::vfx::detail::EndianUtil::Flip( &speedXZ );
        nn::vfx::detail::EndianUtil::Flip( &distance );
        nn::vfx::detail::EndianUtil::Flip( &moveSpeed );
        nn::vfx::detail::EndianUtil::Flip( &amplitudeY );
        nn::vfx::detail::EndianUtil::Flip( &speedY );
        nn::vfx::detail::EndianUtil::Flip( &rollZ );
        nn::vfx::detail::EndianUtil::Flip( &straightDirection );
        nn::vfx::detail::EndianUtil::Flip( &straightAngle );

    }
};

//------------------------------------------------------------------------------
//! @brief      プレビューデータです。
//------------------------------------------------------------------------------
struct ResPreview
{
    nn::vfx::viewer::detail::ResConstrain             constrain;                                                //!< SRT制御データです。
    nn::vfx::viewer::detail::ResPreviewAutoMove       autoMove;                                                 //!< 自動移動データです。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        /* DO_NOTHING */

        constrain.FlipEndian();
        autoMove.FlipEndian();
    }
};

//------------------------------------------------------------------------------
//! @brief      モデルプレビューのデータセットです。
//------------------------------------------------------------------------------
struct ResModelPreviewPack
{
    nn::vfx::viewer::detail::ResPreview               preview;                                                  //!< プレビューデータです。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        /* DO_NOTHING */

        preview.FlipEndian();
    }
};

//------------------------------------------------------------------------------
//! @brief      エフェクトプレビューデータです。
//------------------------------------------------------------------------------
struct ResEffectPreview
{
    nn::vfx::viewer::detail::ResPreviewFnd            basicSetting;                                             //!< 基本設定データです。
    nn::vfx::viewer::detail::ResPreviewColor          color;                                                    //!< カラーデータです。
    nn::vfx::viewer::detail::ResPreviewEmission       emission;                                                 //!< パーティクル放出データです。
    nn::vfx::viewer::detail::ResPreviewPtclScale      ptclScale;                                                //!< パーティクルスケールデータです。
    nn::vfx::viewer::detail::ResPreviewPtclControl    ptclControl;                                              //!< パーティクル制御データです。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        /* DO_NOTHING */

        basicSetting.FlipEndian();
        color.FlipEndian();
        emission.FlipEndian();
        ptclScale.FlipEndian();
        ptclControl.FlipEndian();
    }
};

//------------------------------------------------------------------------------
//! @brief      エフェクトプレビューのデータセットです。
//------------------------------------------------------------------------------
struct ResEffectPreviewPack
{
    nn::vfx::viewer::detail::ResEffectPreview         effectPreview;                                            //!< エフェクトプレビューデータです。
    nn::vfx::viewer::detail::ResPreview               preview;                                                  //!< プレビューデータです。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        /* DO_NOTHING */

        effectPreview.FlipEndian();
        preview.FlipEndian();
    }
};

//------------------------------------------------------------------------------
//! @brief      ptclバイナリデータです。
//------------------------------------------------------------------------------
struct ResPtclBinary
{
    int32_t                                           reloadCtrl;                                               //!< リロード呼び出しを差分形式でコントロールします。
    int32_t                                           esetIndexCtrl;                                            //!< 再生するエミッタセットのインデックスを差分形式でコントロールします。
    int32_t                                           pathLength;                                               //!< ファイルパスの文字数。
    uint8_t                                           filePath[4];                                              //!< ファイルパス。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &reloadCtrl );
        nn::vfx::detail::EndianUtil::Flip( &esetIndexCtrl );
        nn::vfx::detail::EndianUtil::Flip( &pathLength );

    }
};


} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn

