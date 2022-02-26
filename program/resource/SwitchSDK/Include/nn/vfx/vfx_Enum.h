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

namespace nn   {
namespace vfx  {

//----------------------------------------
//! @name システム系
//@{

//---------------------------------------------------------------------------
//! @brief  nn::vfx ライブラリが保持するシステム定数
//!
//!         nn::vfx ライブラリが仕様として持つ定数を参照する場合はこちらを使用してください。
//---------------------------------------------------------------------------
enum SystemParameters
{
    SystemParameters_BinaryVersionId                    = 0x00000016,                           //!< 現在のバイナリバージョン
    SystemParameters_CompatibleBinaryVersionId          = 0x00000016,                           //!< NSDK 0.12.15 のバイナリバージョン
    SystemParameters_MaxEmitterInclusionCount           = 16,                                   //!< エミッタセットの中の最大エミッタ個数
    SystemParameters_MaxTexturePatternTableSize         = 32,                                   //!< テクスチャーパターンテーブル数
    SystemParameters_InfiniteLifeValue                  = 0x0fffffff,                           //!< 寿命無限の時のライフ ( 414日 )
    SystemParameters_MaxGroupCount                      = 64,                                   //!< 最大グループ数
    SystemParameters_ViewerGroupId                      = SystemParameters_MaxGroupCount - 1,   //!< ビューアが利用するグループID
    SystemParameters_MaxCallbackType                    = 8,                                    //!< コールバック最大登録数
    SystemParameters_ParticleBinaryDataAlignment        = ( 4 * 1024 ),                         //!< パーティクルバイナリ メモリアライメント
    SystemParameters_MaxCustomShaderParameterCount      = 32,                                   //!< カスタムシェーダパラメータ数
    SystemParameters_MaxDrawPathCount                   = 32,                                   //!< 最大描画パス数
    SystemParameters_StaticTextureDescriptorSlotCount   = 1,                                    //!< 固定で生成されるテクスチャディスクリプタスロット数
    SystemParameters_StaticSamplerDescriptorSlotCount   = 32,                                   //!< 固定で生成されるサンプラディスクリプタスロット数
};

//---------------------------------------------------------------------------
//! @brief  ID、インデックスなどが無効であることを表す値。
//!
//!         無効なID、インデックスを示す定数として使用してください。
//---------------------------------------------------------------------------
enum InvalidValueId
{
    InvalidValueId_ResourceId                       = -1,           //!< 無効なリソース ID
    InvalidValueId_EmitterId                        = -1,           //!< 無効なエミッタ
    InvalidValueId_EmitterSetId                     = -1,           //!< 無効なエミッタセット
    InvalidValueId_TextureSamplerSlotId             = -1,           //!< 無効なテクスチャサンプラー
    InvalidValueId_TextureLocationId                = -1,           //!< 無効なテクスチャロケーション
    InvalidValueId_PrimitiveIndex                   = -1,           //!< 無効なプリミティブ Index
    InvalidValueId_AttributeId                      = -1,           //!< 無効なアトリビュート スロット
    InvalidValueId_ConstantBufferSlotId             = -1,           //!< 無効な定数バッファ スロット
    InvalidValueId_TextureId                        = -1,           //!< 無効なテクスチャ ID
    InvalidValueId_ShaderId                         = -1,           //!< 無効なシェーダ ID
    InvalidValueId_PrimitiveEmitter                 = -1,           //!< プリミティブエミッタではない
    InvalidValueId_G3dAttributeIndex                = 255,          //!< 無効なG3dアトリビュートインデックス
    InvalidValueId_AssetGlobalId                    = -1,           //!< 無効なアセットグローバルID
};

//---------------------------------------------------------------------------
//! @brief  描画ビューフラグ
//---------------------------------------------------------------------------
enum DrawViewFlag
{
    DrawViewFlag_0                = ( 0x01 << 0  ),       //!< ビュー 0
    DrawViewFlag_1                = ( 0x01 << 1  ),       //!< ビュー 1
    DrawViewFlag_2                = ( 0x01 << 2  ),       //!< ビュー 2
    DrawViewFlag_3                = ( 0x01 << 3  ),       //!< ビュー 3
    DrawViewFlag_4                = ( 0x01 << 4  ),       //!< ビュー 4
    DrawViewFlag_5                = ( 0x01 << 5  ),       //!< ビュー 5
    DrawViewFlag_6                = ( 0x01 << 6  ),       //!< ビュー 6
    DrawViewFlag_7                = ( 0x01 << 7  ),       //!< ビュー 7
    DrawViewFlag_8                = ( 0x01 << 8  ),       //!< ビュー 8
    DrawViewFlag_9                = ( 0x01 << 9  ),       //!< ビュー 9
    DrawViewFlag_10               = ( 0x01 << 10 ),       //!< ビュー 10
    DrawViewFlag_11               = ( 0x01 << 11 ),       //!< ビュー 11
    DrawViewFlag_12               = ( 0x01 << 12 ),       //!< ビュー 12
    DrawViewFlag_13               = ( 0x01 << 13 ),       //!< ビュー 13
    DrawViewFlag_14               = ( 0x01 << 14 ),       //!< ビュー 14
    DrawViewFlag_15               = ( 0x01 << 15 ),       //!< ビュー 15
    DrawViewFlag_16               = ( 0x01 << 16 ),       //!< ビュー 16
    DrawViewFlag_17               = ( 0x01 << 17 ),       //!< ビュー 17
    DrawViewFlag_18               = ( 0x01 << 18 ),       //!< ビュー 18
    DrawViewFlag_19               = ( 0x01 << 19 ),       //!< ビュー 19
    DrawViewFlag_20               = ( 0x01 << 20 ),       //!< ビュー 20
    DrawViewFlag_21               = ( 0x01 << 21 ),       //!< ビュー 21
    DrawViewFlag_22               = ( 0x01 << 22 ),       //!< ビュー 22
    DrawViewFlag_23               = ( 0x01 << 23 ),       //!< ビュー 23
    DrawViewFlag_24               = ( 0x01 << 24 ),       //!< ビュー 24
    DrawViewFlag_25               = ( 0x01 << 25 ),       //!< ビュー 25
    DrawViewFlag_26               = ( 0x01 << 26 ),       //!< ビュー 26
    DrawViewFlag_27               = ( 0x01 << 27 ),       //!< ビュー 27
    DrawViewFlag_28               = ( 0x01 << 28 ),       //!< ビュー 28
    DrawViewFlag_29               = ( 0x01 << 29 ),       //!< ビュー 29
    DrawViewFlag_30               = ( 0x01 << 30 ),       //!< ビュー 30
    DrawViewFlag_31               = ( 0x01 << 31 ),       //!< ビュー 31

    DrawViewFlag_All = DrawViewFlag_0  | DrawViewFlag_1  | DrawViewFlag_2  | DrawViewFlag_3  |
                       DrawViewFlag_4  | DrawViewFlag_5  | DrawViewFlag_6  | DrawViewFlag_7  |
                       DrawViewFlag_8  | DrawViewFlag_9  | DrawViewFlag_10 | DrawViewFlag_11 |
                       DrawViewFlag_12 | DrawViewFlag_13 | DrawViewFlag_14 | DrawViewFlag_15 |
                       DrawViewFlag_16 | DrawViewFlag_17 | DrawViewFlag_18 | DrawViewFlag_19 |
                       DrawViewFlag_20 | DrawViewFlag_21 | DrawViewFlag_22 | DrawViewFlag_23 |
                       DrawViewFlag_24 | DrawViewFlag_25 | DrawViewFlag_26 | DrawViewFlag_27 |
                       DrawViewFlag_28 | DrawViewFlag_29 | DrawViewFlag_30 | DrawViewFlag_31,  //!< ビュー ALL
    DrawViewFlag_None             = 0,                    //!< 描画しない
};

//---------------------------------------------------------------------------
//! @brief  描画パスフラグ
//!
//!         描画パスを指定して描画する際に利用するフラグです。
//---------------------------------------------------------------------------
enum DrawPathFlag
{
    DrawPathFlag_0  = 0x01 << 0,            //!< 描画パス0
    DrawPathFlag_1  = 0x01 << 1,            //!< 描画パス1
    DrawPathFlag_2  = 0x01 << 2,            //!< 描画パス2
    DrawPathFlag_3  = 0x01 << 3,            //!< 描画パス3
    DrawPathFlag_4  = 0x01 << 4,            //!< 描画パス4
    DrawPathFlag_5  = 0x01 << 5,            //!< 描画パス5
    DrawPathFlag_6  = 0x01 << 6,            //!< 描画パス6
    DrawPathFlag_7  = 0x01 << 7,            //!< 描画パス7
    DrawPathFlag_8  = 0x01 << 8,            //!< 描画パス8
    DrawPathFlag_9  = 0x01 << 9,            //!< 描画パス9
    DrawPathFlag_10 = 0x01 << 10,           //!< 描画パス10
    DrawPathFlag_11 = 0x01 << 11,           //!< 描画パス11
    DrawPathFlag_12 = 0x01 << 12,           //!< 描画パス12
    DrawPathFlag_13 = 0x01 << 13,           //!< 描画パス13
    DrawPathFlag_14 = 0x01 << 14,           //!< 描画パス14
    DrawPathFlag_15 = 0x01 << 15,           //!< 描画パス15
    DrawPathFlag_16 = 0x01 << 16,           //!< 描画パス16
    DrawPathFlag_17 = 0x01 << 17,           //!< 描画パス17
    DrawPathFlag_18 = 0x01 << 18,           //!< 描画パス18
    DrawPathFlag_19 = 0x01 << 19,           //!< 描画パス19
    DrawPathFlag_20 = 0x01 << 20,           //!< 描画パス20
    DrawPathFlag_21 = 0x01 << 21,           //!< 描画パス21
    DrawPathFlag_22 = 0x01 << 22,           //!< 描画パス22
    DrawPathFlag_23 = 0x01 << 23,           //!< 描画パス23
    DrawPathFlag_24 = 0x01 << 24,           //!< 描画パス24
    DrawPathFlag_25 = 0x01 << 25,           //!< 描画パス25
    DrawPathFlag_26 = 0x01 << 26,           //!< 描画パス26
    DrawPathFlag_27 = 0x01 << 27,           //!< 描画パス27
    DrawPathFlag_28 = 0x01 << 28,           //!< 描画パス28
    DrawPathFlag_29 = 0x01 << 29,           //!< 描画パス29
    DrawPathFlag_30 = 0x01 << 30,           //!< 描画パス30
    DrawPathFlag_31 = 0x01 << 31,           //!< 描画パス31
    DrawPathFlag_All = 0xFFFFFFFF,          //!< 全ての描画パス
};

//---------------------------------------------------------------------------
//! @brief  バッファSwap モード
//---------------------------------------------------------------------------
enum BufferSwapMode
{
    BufferSwapMode_None = 0,    //!< スワップしない
    BufferSwapMode_Swap = 1,    //!< スワップする
    BufferSwapMode_Auto = 2,    //!< オートスワップ
};

//---------------------------------------------------------------------------
//! @brief  nn::vfx ライブラリのバッファリングモード。
//---------------------------------------------------------------------------
enum BufferingMode
{
    BufferingMode_DoubleBuffering   = 2,    //!< ダブルバッファリング
    BufferingMode_TripleBuffering   = 3,    //!< トリプルバッファリング
};

//---------------------------------------------------------------------------
//! @brief  バッファサイド
//---------------------------------------------------------------------------
enum BufferSide
{
    BufferSide_FrontBuffer          = 0,    //!< フロントバッファ
    BufferSide_BackBuffer           = 1,    //!< バックバッファ
    BufferSide_TripleBuffer         = 2,    //!< トリプルバッファ用の三枚目のバッファ
    BufferSide_MaxBufferSide        = 3,    //!< バッファサイドの総数
};

//@}

//----------------------------------------
//! @name シェーダ系
//@{

//---------------------------------------------------------------------------
//! @brief  シェーダタイプ
//!
//!         シェーダタイプを示す定数です。
//!         通常 nn::vfx::ShaderType::ShaderType_Normal ですが、
//!         プロジェクトコンフィグで描画パスに設定をした場合、
//!         nn::vfx::ShaderType::ShaderType_DefaultPath1 と nn::vfx::ShaderType::ShaderType_DefaultPath2 が使用可能になります。
//---------------------------------------------------------------------------
enum ShaderType
{
    ShaderType_Normal               = 0,    //!< 通常シェーダ
    ShaderType_DefaultPath1         = 1,    //!< シェーダ1（プロジェクトコンフィグの描画パス設定で指定したときのみ使用可能）
    ShaderType_DefaultPath2         = 2,    //!< シェーダ2（プロジェクトコンフィグの描画パス設定で指定したときのみ使用可能）
    ShaderType_MaxShaderType        = 3,    //!< シェーダタイプの総数
};

//@}

//----------------------------------------
//! @name テクスチャ系
//@{

//---------------------------------------------------------------------------
//! @brief  テクスチャスロット
//---------------------------------------------------------------------------
enum TextureSlotId
{
    TextureSlotId_0                         = 0,    //!< スロット0 テクスチャ0
    TextureSlotId_1                         = 1,    //!< スロット1 テクスチャ1
    TextureSlotId_2                         = 2,    //!< スロット2 テクスチャ2
    TextureSlotId_MaxStandardTextureSlotId  = 3,    //!< スロット  最大数
    TextureSlotId_FrameBuffer               = 3,    //!< フレームバッファ テクスチャ
    TextureSlotId_DepthBuffer               = 4,    //!< デプスバッファ   テクスチャ
    TextureSlotId_CurlNoise                 = 5,    //!< カールノイズ     テクスチャ
};

//---------------------------------------------------------------------------
//! @brief  テクスチャタイプ
//---------------------------------------------------------------------------
enum TextureType
{
    TextureType_2d                          = 0,    //!< 2D               テクスチャ
    TextureType_2dArray                     = 1,    //!< 2D Array         テクスチャ
    TextureType_3d                          = 2,    //!< 3D               テクスチャ
    TextureType_CubeMap                     = 3,    //!< Cube Map         テクスチャ
    TextureType_CubeMapArray                = 4,    //!< Cube Map Array   テクスチャ
    TextureType_ShadowMap                   = 5,    //!< Shadow Map       テクスチャ
    TextureType_ShadowMapArray              = 6,    //!< Shadow Map Array テクスチャ
};

//---------------------------------------------------------------------------
//! @brief  カスタムテクスチャ ID
//---------------------------------------------------------------------------
enum CustomShaderTextureId
{
    CustomShaderTextureId_0                 = 0,    //!< カスタムシェーダ ID 0
    CustomShaderTextureId_1                 = 1,    //!< カスタムシェーダ ID 1
    CustomShaderTextureId_2                 = 2,    //!< カスタムシェーダ ID 2
    CustomShaderTextureId_3                 = 3,    //!< カスタムシェーダ ID 3
    CustomShaderTextureId_MaxTextureId      = 4,    //!< カスタムシェーダ IDの総数
};

//---------------------------------------------------------------------------
//! @brief  カスタムシェーダ用テクスチャスロット
//---------------------------------------------------------------------------
enum CustomShaderTextureSlotId
{
    CustomShaderTextureSlotId_0                 = 6,    //!< スロット0
    CustomShaderTextureSlotId_1                 = 7,    //!< スロット1
    CustomShaderTextureSlotId_2                 = 8,    //!< スロット2
    CustomShaderTextureSlotId_3                 = 9,    //!< スロット3
    CustomShaderTextureSlotId_MaxTextureSlotId  = 10,   //!< スロット3
};

//---------------------------------------------------------------------------
//! @brief  カスタムテクスチャ Type
//---------------------------------------------------------------------------
enum CustomShaderTextureType
{
    CustomShaderTextureType_2d0             = 0,    //!< カスタムシェーダ 2D テクスチャ 0
    CustomShaderTextureType_2d1             = 1,    //!< カスタムシェーダ 2D テクスチャ 1
    CustomShaderTextureType_2d2             = 2,    //!< カスタムシェーダ 2D テクスチャ 2
    CustomShaderTextureType_2d3             = 3,    //!< カスタムシェーダ 2D テクスチャ 3
    CustomShaderTextureType_2dArray0        = 4,    //!< カスタムシェーダ 2D ARRAY テクスチャ 0
    CustomShaderTextureType_2dArray1        = 5,    //!< カスタムシェーダ 2D ARRAY テクスチャ 1
    CustomShaderTextureType_2dArray2        = 6,    //!< カスタムシェーダ 2D ARRAY テクスチャ 2
    CustomShaderTextureType_2dArray3        = 7,    //!< カスタムシェーダ 2D ARRAY テクスチャ 3
    CustomShaderTextureType_3d0             = 8,    //!< カスタムシェーダ 3D テクスチャ ID 0
    CustomShaderTextureType_3d1             = 9,    //!< カスタムシェーダ 3D テクスチャ ID 1
    CustomShaderTextureType_3d2             = 10,   //!< カスタムシェーダ 3D テクスチャ ID 2
    CustomShaderTextureType_3d3             = 11,   //!< カスタムシェーダ 3D テクスチャ ID 3
    CustomShaderTextureType_CubeMap0        = 12,   //!< カスタムシェーダ CUBE MAP テクスチャ 0
    CustomShaderTextureType_CubeMap1        = 13,   //!< カスタムシェーダ CUBE MAP テクスチャ 1
    CustomShaderTextureType_CubeMap2        = 14,   //!< カスタムシェーダ CUBE MAP テクスチャ 2
    CustomShaderTextureType_CubeMap3        = 15,   //!< カスタムシェーダ CUBE MAP テクスチャ 3
    CustomShaderTextureType_CubeMapArray0   = 16,   //!< カスタムシェーダ CUBE MAP ARRAY テクスチャ 0
    CustomShaderTextureType_CubeMapArray1   = 17,   //!< カスタムシェーダ CUBE MAP ARRAY テクスチャ 1
    CustomShaderTextureType_CubeMapArray2   = 18,   //!< カスタムシェーダ CUBE MAP ARRAY テクスチャ 2
    CustomShaderTextureType_CubeMapArray3   = 19,   //!< カスタムシェーダ CUBE MAP ARRAY テクスチャ 3
    CustomShaderTextureType_ShadowMap0      = 20,   //!< カスタムシェーダ SHADOW テクスチャ ID 0
    CustomShaderTextureType_ShadowMap1      = 21,   //!< カスタムシェーダ SHADOW テクスチャ ID 1
    CustomShaderTextureType_ShadowMap2      = 22,   //!< カスタムシェーダ SHADOW テクスチャ ID 2
    CustomShaderTextureType_ShadowMap3      = 23,   //!< カスタムシェーダ SHADOW テクスチャ ID 3
    CustomShaderTextureType_ShadowMapArray0 = 24,   //!< カスタムシェーダ SHADOW ARRAY テクスチャ ID 0
    CustomShaderTextureType_ShadowMapArray1 = 25,   //!< カスタムシェーダ SHADOW ARRAY テクスチャ ID 1
    CustomShaderTextureType_ShadowMapArray2 = 26,   //!< カスタムシェーダ SHADOW ARRAY テクスチャ ID 2
    CustomShaderTextureType_ShadowMapArray3 = 27,   //!< カスタムシェーダ SHADOW ARRAY テクスチャ ID 3
    CustomShaderTextureType_MaxTextureType  = 28,   //!< カスタムシェーダ テクスチャタイプの総数
};

//@}

//----------------------------------------
//! @name カスタム処理関係
//@{

//------------------------------------------------------------------------------
//! @brief  定数バッファ ID
//------------------------------------------------------------------------------
enum CustomShaderConstantBufferIndex
{
    CustomShaderConstantBufferIndex_0             = 0,            //!< Constant Buffer インデックス 0
    CustomShaderConstantBufferIndex_1             = 1,            //!< Constant Buffer インデックス 1
    CustomShaderConstantBufferIndex_2             = 2,            //!< Constant Buffer インデックス 2
    CustomShaderConstantBufferIndex_3             = 3,            //!< Constant Buffer インデックス 3
    CustomShaderConstantBufferIndex_4             = 4,            //!< Constant Buffer インデックス 4 (エミッタプラグインのデフォルト)
    CustomShaderConstantBufferIndex_MaxIndexCount = 5,            //!< Constant Buffer インデックスの総数
};

//---------------------------------------------------------------------------
//! @brief  描画パスコールバックID。
//---------------------------------------------------------------------------
enum DrawPathCallbackId
{
    DrawPathCallbackId_0                = 0,            //!< CallbackID 0
    DrawPathCallbackId_1                = 1,            //!< CallbackID 1
    DrawPathCallbackId_2                = 2,            //!< CallbackID 2
    DrawPathCallbackId_3                = 3,            //!< CallbackID 3
    DrawPathCallbackId_4                = 4,            //!< CallbackID 4
    DrawPathCallbackId_5                = 5,            //!< CallbackID 5
    DrawPathCallbackId_6                = 6,            //!< CallbackID 6
    DrawPathCallbackId_7                = 7,            //!< CallbackID 7
    DrawPathCallbackId_MaxCallbackId    = 8,            //!< CallbackID MAX
};

//---------------------------------------------------------------------------
//! @brief  コールバックID。
//---------------------------------------------------------------------------
enum CallbackId
{
    CallbackId_CustomAction1        = 0,        //!< アクション コールバックID 1
    CallbackId_CustomAction2        = 1,        //!< アクション コールバックID 2
    CallbackId_CustomAction3        = 2,        //!< アクション コールバックID 3
    CallbackId_CustomAction4        = 3,        //!< アクション コールバックID 4
    CallbackId_CustomAction5        = 4,        //!< アクション コールバックID 5
    CallbackId_CustomAction6        = 5,        //!< アクション コールバックID 6
    CallbackId_CustomAction7        = 6,        //!< アクション コールバックID 7
    CallbackId_CustomAction8        = 7,        //!< アクション コールバックID 8
    CallbackId_CustomShaderNone     = 8,        //!< シェーダ   コールバック設定無し
    CallbackId_CustomShader1        = 9,        //!< シェーダ   コールバックID 1
    CallbackId_CustomShader2        = 10,       //!< シェーダ   コールバックID 2
    CallbackId_CustomShader3        = 11,       //!< シェーダ   コールバックID 3
    CallbackId_CustomShader4        = 12,       //!< シェーダ   コールバックID 4
    CallbackId_CustomShader5        = 13,       //!< シェーダ   コールバックID 5
    CallbackId_CustomShader6        = 14,       //!< シェーダ   コールバックID 6
    CallbackId_CustomShader7        = 15,       //!< シェーダ   コールバックID 7
    CallbackId_CustomShader8        = 16,       //!< シェーダ   コールバックID 8
    CallbackId_MaxCallbackIdCount   = 17,       //!< コールバックIDの総数
};

//---------------------------------------------------------------------------
//! @brief  エミッタ計算処理の返り値で、後処理の種類を指定する値
//---------------------------------------------------------------------------
enum EmitterCalculationResult
{
    EmitterCalculationResult_Alive  = 0,    //!< エミッタ計算処理結果：成功
    EmitterCalculationResult_Kill   = 1,    //!< エミッタ計算処理結果：Kill() して終了
    EmitterCalculationResult_Fade   = 2,    //!< エミッタ計算処理結果：Fade() して終了
};

//@}

namespace detail {

//------------------------------------------------------------------------------
//! @brief  乱数タイプ
//------------------------------------------------------------------------------
enum EmitterRandomSeedType
{
    EmitterRandomSeedType_Random = 0,                       //!< 毎回変化
    EmitterRandomSeedType_Eset   = 1,                       //!< ESet内共通
    EmitterRandomSeedType_Fixed  = 2,                       //!< 固定
};

//------------------------------------------------------------------------------
//! @brief  エミッタ時間アニメーション タイプ
//------------------------------------------------------------------------------
enum EmitterAnimationType
{
    EmitterAnimationType_Scale = 0,                         //!< エミッタトランスフォーム スケール( vec3 )
    EmitterAnimationType_Rotate,                            //!< エミッタトランスフォーム 回転( vec3 )
    EmitterAnimationType_Translate,                         //!< エミッタトランスフォーム 平行移動( vec3 )
    EmitterAnimationType_Color0,                            //!< エミッタカラー0( vec3 )
    EmitterAnimationType_Color1,                            //!< エミッタカラー1( vec3 )
    EmitterAnimationType_EmissionRate,                      //!< エミッタ放出レート( float )
    EmitterAnimationType_ParticleLife,                      //!< 寿命( float )
    EmitterAnimationType_Alpha0,                            //!< アルファ0( float )
    EmitterAnimationType_Alpha1,                            //!< アルファ1( float )
    EmitterAnimationType_AllDirectionalVelocity,            //!< 全方向速度( float )
    EmitterAnimationType_DesignatedDirectionalVelocity,     //!< 指定方向速度( float )
    EmitterAnimationType_ParticleScale,                     //!< パーティクルサイズの基準値 ( float )
    EmitterAnimationType_EmitterVolumeScale,                //!< エミッタボリュームスケール ( float )
    EmitterAnimationType_GravityScale,                      //!< 重力 ( float )
    EmitterAnimationType_MaxAnimationType,                  //!< エミッタアニメーション最大数
};

//---------------------------------------------------------------------------
//! @brief  コールバックセットタイプ。
//---------------------------------------------------------------------------
enum CallbackSetType
{
    CallbackSetType_CustomAction            = 0,            //!< カスタムアクション
    CallbackSetType_CustomShader            = 1,            //!< カスタムシェーダ
    CallbackSetType_EmitterPlugin           = 2,            //!< エミッタプラグイン
    CallbackSetType_MaxCallbackSetType      = 3,            //!< コールバックセットタイプの総数
};

//---------------------------------------------------------------------------
//! @brief  テクスチャフィルタ
//---------------------------------------------------------------------------
enum TextureFilterMode
{
    TextureFilterMode_Linear,               //!< リニア
    TextureFilterMode_Near,                 //!< ニア
};

//---------------------------------------------------------------------------
//! @brief  テクスチャラップ
//---------------------------------------------------------------------------
enum TextureWrapMode
{
    TextureWrapMode_Mirror,                 //!< ミラー
    TextureWrapMode_Repeat,                 //!< リピート
    TextureWrapMode_Clamp,                  //!< クランプ
    TextureWrapMode_MirrorOnce,             //!< ミラーワンス
};

//---------------------------------------------------------------------------
//! @brief  テクスチャパタンアニメーションタイプ
//---------------------------------------------------------------------------
enum TexturePatternAnimationType
{
    TexturePatternAnimationType_None        = 0,    //!< 利用しない
    TexturePatternAnimationType_LifeFit     = 1,    //!< ライフフィット
    TexturePatternAnimationType_Clamp       = 2,    //!< クランプ
    TexturePatternAnimationType_Loop        = 3,    //!< ループ
    TexturePatternAnimationType_Random      = 4,    //!< ランダム
    TexturePatternAnimationType_Surface     = 5,    //!< サーフェイスアニメーション
};

//---------------------------------------------------------------------------
//! @brief  カラー計算式タイプ
//---------------------------------------------------------------------------
enum CombinerColorProcessType
{
    CombinerColorProcessType_Color              = 0,    //!< Color0
    CombinerColorProcessType_Texture            = 1,    //!< Color0 * Texture
    CombinerColorProcessType_TextureInterpolate = 2,    //!< Color0 * Texture + Color1 * ( 1 - Texture )
    CombinerColorProcessType_TextureAdd         = 3,    //!< Color0 * Texture + Color1
};

//------------------------------------------------------------------------------
//! @brief  アルファ計算式タイプ
//------------------------------------------------------------------------------
enum CombinerAlphaProcessType
{
    CombinerAlphaProcessType_MulAlpha0          = 0,    //!< CombinedA * Alpha0
    CombinerAlphaProcessType_MulAlpha01         = 1,    //!< CombinedA * Alpha0 * Alpha1
    CombinerAlphaProcessType_SubAlpha0          = 2,    //!< ( CombinedA - ( 1 - Alpha0 ) ) * 2
    CombinerAlphaProcessType_SubAlpha0MulAlpha1 = 3,    //!< ( CombinedA - Alpha0 ) * Alpha1
    CombinerAlphaProcessType_DistanceField      = 4,    //!< clamp( ( CombinedA - Alpha0 ) *4 ) * Alpha1
};

//------------------------------------------------------------------------------
//! @brief  コンバイナブレンドタイプ
//------------------------------------------------------------------------------
enum CombinerBlendType
{
    CombinerBlendType_Mul = 0,                          //!< 乗算ブレンド
    CombinerBlendType_Add = 1,                          //!< 加算ブレンド
    CombinerBlendType_Sub = 2,                          //!< 減算ブレンド
};

//------------------------------------------------------------------------------
//! @brief  コンバイナ カラー入力タイプ
//------------------------------------------------------------------------------
enum CombinerColorInputType
{
    CombinerColorInputType_Source          = 0,         //!< 入力をそのまま利用
    CombinerColorInputType_One             = 1,         //!< 入力を 1.0 に置き換え
    CombinerColorInputType_OneMinusRGB     = 2,         //!< 入力を (1.0 - RGB) に置き換え
    CombinerColorInputType_Alpha           = 3,         //!< 入力を Alpha に置き換え
    CombinerColorInputType_OneMinusAlpha   = 4,         //!< 入力を (1.0 - Alpha) に置き換え
};

//------------------------------------------------------------------------------
//! @brief  コンバイナ アルファ入力タイプ
//------------------------------------------------------------------------------
enum CombinerAlphaInputType
{
    CombinerAlphaInputType_Source            = 0,       //!< 入力をそのまま利用
    CombinerAlphaInputType_Red               = 1,       //!< 入力を RED に置き換え(アルファのみ)
    CombinerAlphaInputType_One               = 2,       //!< 入力を 1.0 に置き換え
    CombinerAlphaInputType_OneMinusAlpha     = 3,       //!< 入力を (1.0 - Alpha) に置き換え
    CombinerAlphaInputType_OneMinusRed       = 4,       //!< 入力を (1.0 - Red) に置き換え
};

//---------------------------------------------------------------------------
//! @brief  パーティクルのエミッタ追従タイプ。
//---------------------------------------------------------------------------
enum ParticleFollowType
{
    ParticleFollowType_EmitterFull          = 0,        //!< 完全追従
    ParticleFollowType_None                 = 1,        //!< 追従しない
    ParticleFollowType_EmitterPosition      = 2,        //!< 位置だけ追従
    ParticleFollowType_MaxFollowType        = 3,        //!< エミッタ追従タイプ総数
};

//---------------------------------------------------------------------------
//! @brief  エミッタボリューム形状
//---------------------------------------------------------------------------
enum EmitterVolumeType
{
    EmitterVolumeType_Point = 0,                        //!< 点エミッタ
    EmitterVolumeType_Circle,                           //!< 円
    EmitterVolumeType_CircleEquallyDivided,             //!< 円(分割)
    EmitterVolumeType_CircleFill,                       //!< 円(フィル)
    EmitterVolumeType_Sphere,                           //!< 球
    EmitterVolumeType_SphereEquallyDivided,             //!< 球(分割)
    EmitterVolumeType_SphereEqually64Divided,           //!< 球(分割64)
    EmitterVolumeType_SphereFill,                       //!< 球(フィル)
    EmitterVolumeType_Cylinder,                         //!< 円柱
    EmitterVolumeType_CylinderFill,                     //!< 円柱(フィル)
    EmitterVolumeType_Box,                              //!< 立方体
    EmitterVolumeType_BoxFill,                          //!< 立方体(フィル)
    EmitterVolumeType_Line,                             //!< 線
    EmitterVolumeType_LineEquallyDivided,               //!< 線(分割)
    EmitterVolumeType_Rectangle,                        //!< 矩形
    EmitterVolumeType_Primitive,                        //!< プリミティブ
    EmitterVolumeType_MaxVolumeType,                    //!< エミッタ形状総数
};

//---------------------------------------------------------------------------
//! @brief  プリミティブエミッタ放出タイプ
//---------------------------------------------------------------------------
enum PrimitiveEmissionType
{
    PrimitiveEmissionType_Unison        = 0,            //!< 一斉放出
    PrimitiveEmissionType_Random        = 1,            //!< ランダムな頂点から放出
    PrimitiveEmissionType_IndexOrder    = 2,            //!< インデックス順に放出
};

//------------------------------------------------------------------------------
//! @brief  パーティクル オフセットタイプ
//------------------------------------------------------------------------------
enum ParticleOffsetType
{
    ParticleOffsetType_Depth            = 0,            //!< デプス方向
    ParticleOffsetType_Camera           = 1,            //!< カメラ方向
    ParticleOffsetType_Disable          = 2,            //!< 無効
    ParticleOffsetType_CameraFixedsize  = 3,            //!< カメラ方向（サイズ固定）
};

//------------------------------------------------------------------------------
//! @brief  パーティクル揺らぎ 波形タイプ
//------------------------------------------------------------------------------
enum ParticleFluctuationWaveType
{
    ParticleFluctuationWaveType_Sin         = 0,        //!< Sin波
    ParticleFluctuationWaveType_SawTooth    = 1,        //!< のこぎり波
    ParticleFluctuationWaveType_Rectangle   = 2,        //!< 矩形波
};

//------------------------------------------------------------------------------
//! @brief  エミッタ挙動計算 タイプ
//------------------------------------------------------------------------------
enum EmitterCalculationMode
{
    EmitterCalculationMode_Cpu          = 0,            //!< CPUで挙動計算
    EmitterCalculationMode_Gpu          = 1,            //!< GPUで挙動計算
    EmitterCalculationMode_GpuStreamOut = 2,            //!< GPU+StreamOutで挙動計算
    EmitterCalculationMode_MaxMode      = 3,            //!< 計算タイプの総数
};

//---------------------------------------------------------------------------
//! @brief  フレームバッファとのブレンドタイプ。
//---------------------------------------------------------------------------
enum BlendType
{
    BlendType_Normal                    = 0,            //!< 通常のブレンド
    BlendType_Add                       = 1,            //!< 加算ブレンド
    BlendType_Sub                       = 2,            //!< 減算ブレンド
    BlendType_Multiply                  = 3,            //!< 乗算ブレンド
    BlendType_Screen                    = 4,            //!< スクリーンブレンド
    BlendType_PreMultipliedAlpha        = 5,            //!< 乗算済みアルファ (1 - SrcAlpha)
};

//---------------------------------------------------------------------------
//! @brief  Ｚバッファー、αテストのタイプ
//---------------------------------------------------------------------------
enum ZBufferAlphaTestType
{
    ZBufferAlphaTestType_Normal             = 0,        //!< ブレンド有り 通常の状態（Ｚ比較あり、Ｚ書き込みなし、αテスト０より大きいが合格）
    ZBufferAlphaTestType_IgnoreZValue       = 1,        //!< ブレンド有り Ｚ無視    （Ｚ比較なし、Ｚ書き込みなし、αテスト０より大きいが合格）
    ZBufferAlphaTestType_Entity             = 2,        //!< ブレンド無し 実体      （Ｚ比較あり、Ｚ書き込み有り、αテスト0.5以上が合格）
    ZBufferAlphaTestType_Opaque             = 3,        //!< ブレンド無し 不透明    （Ｚ比較あり、Ｚ書き込み有り、αテスト無し）
    ZBufferAlphaTestType_MaxType            = 4,        //!< バリエーションの総数
};

//---------------------------------------------------------------------------
//! @brief  表示面のタイプ。
//---------------------------------------------------------------------------
enum DisplaySideType
{
    DisplaySideType_Both                = 0,            //!< 両面
    DisplaySideType_Front               = 1,            //!< 表
    DisplaySideType_Back                = 2,            //!< 裏
};

//---------------------------------------------------------------------------
//! @brief  アルファ/デプス 比較式。
//---------------------------------------------------------------------------
enum TestCompareFunction
{
    TestCompareFunction_Never           = 0,            //!< 常に通過しない
    TestCompareFunction_Less            = 1,            //!< ＜ 基準値
    TestCompareFunction_Equal           = 2,            //!< ＝ 基準値
    TestCompareFunction_LessOrEqual     = 3,            //!< ≦ 基準値
    TestCompareFunction_GreaterThan     = 4,            //!< ＞ 基準値
    TestCompareFunction_NotEqual        = 5,            //!< ≠ 基準値
    TestCompareFunction_GreaterOrEqual  = 6,            //!< ≧ 基準値
    TestCompareFunction_Always          = 7,            //!< 常に通過
};

//---------------------------------------------------------------------------
//! @brief  シェーダタイプ
//!
//!         基本機能として実装されているピクセルシェーダタイプです。
//---------------------------------------------------------------------------
enum PixelShaderMode
{
    PixelShaderMode_Normal           = 0,               //!< 通常
    PixelShaderMode_Refraction       = 1,               //!< カラーバッファ屈折
    PixelShaderMode_Distortion       = 2,               //!< テクスチャ歪み
};

//------------------------------------------------------------------------------
//! @brief  補間のバリエーション
//------------------------------------------------------------------------------
enum KeyFrameAnimationInterpolationType
{
    KeyFrameAnimationInterpolationType_Linear   = 0,    //!< 線形補間
    KeyFrameAnimationInterpolationType_Smooth   = 1,    //!< スムース補間
};

//---------------------------------------------------------------------------
//! @brief  エミッタプラグインのリソースID
//---------------------------------------------------------------------------
enum EmitterPluginId
{
    EmitterPluginId_None        = 0,                    //!< エミッタプラグイン割り当て無し
    EmitterPluginId_1           = 1,                    //!< エミッタプラグインID 1
    EmitterPluginId_2           = 2,                    //!< エミッタプラグインID 2
    EmitterPluginId_3           = 3,                    //!< エミッタプラグインID 3
    EmitterPluginId_4           = 4,                    //!< エミッタプラグインID 4
    EmitterPluginId_5           = 5,                    //!< エミッタプラグインID 5
    EmitterPluginId_6           = 6,                    //!< エミッタプラグインID 6
    EmitterPluginId_7           = 7,                    //!< エミッタプラグインID 7
    EmitterPluginId_8           = 8,                    //!< エミッタプラグインID 8
    EmitterPluginId_MaxPluginId = 8,                    //!< エミッタプラグインIDの総数
};

//---------------------------------------------------------------------------
//! @brief  エミッタプラグインのコールバックインデックス。
//---------------------------------------------------------------------------
enum EmitterPluginCallbackIndex
{
    EmitterPluginCallbackIndex_1                = EmitterPluginId_1 - 1,    //!< 連結式ストライプ
    EmitterPluginCallbackIndex_2                = EmitterPluginId_2 - 1,    //!< 履歴式ストライプ
    EmitterPluginCallbackIndex_3                = EmitterPluginId_3 - 1,    //!< 履歴式ストライプ2
    EmitterPluginCallbackIndex_4                = EmitterPluginId_4 - 1,    //!< 範囲内ループ
    EmitterPluginCallbackIndex_5                = EmitterPluginId_5 - 1,    //!< [未実装]EP-05
    EmitterPluginCallbackIndex_6                = EmitterPluginId_6 - 1,    //!< [未実装]EP-06
    EmitterPluginCallbackIndex_7                = EmitterPluginId_7 - 1,    //!< [未実装]EP-07
    EmitterPluginCallbackIndex_8                = EmitterPluginId_8 - 1,    //!< [未実装]EP-08
    EmitterPluginCallbackIndex_MaxCallbackIndex = 8,                        //!< エミッタプラグインIDの総数
};

//---------------------------------------------------------------------------
//! @internal
//! @briefprivate ランタイム警告コードです。
//---------------------------------------------------------------------------
enum RuntimeWarningId
{
    RuntimeWarningId_None                                  = 0,                        //!< 警告無し
    RuntimeWarningId_ParticleEmissionFailure               = ( 1 << 0 ),               //!< パーティクル放出エラー
    RuntimeWarningId_ParticleMaxCountIsZero                = ( 1 << 1 ),               //!< パーティクル最大数がゼロ
    RuntimeWarningId_ParticleInstanceIsDirty               = ( 1 << 2 ),               //!< パーティクルが不正
    RuntimeWarningId_ParticleOverwrite                     = ( 1 << 3 ),               //!< パーティクルを上書き
    RuntimeWarningId_NoCustomActionParameter               = ( 1 << 4 ),               //!< カスタムアクションパラメータが無い
    RuntimeWarningId_LoadUnsafePrimitive                   = ( 1 << 5 ),               //!< 不正なプリミティブデータのロード
    RuntimeWarningId_IsNotManualEmitMode                   = ( 1 << 6 ),               //!< マニュアル放出エラー
    RuntimeWarningId_NoShaderExists                        = ( 1 << 7 ),               //!< 描画に利用するシェーダが無い
    RuntimeWarningId_ManualEmitSizeOver                    = ( 1 << 8 ),               //!< マニュアル放出数オーバー
    RuntimeWarningId_ManualEmitterIsFull                   = ( 1 << 9 ),               //!< マニュアル放出エミッタにパーティクルの空きが無い

    RuntimeWarningId_DynamicHeapAllocationFailed           = ( 1 << 10 ),              //!< 動的ヒープメモリ確保失敗
    RuntimeWarningId_TemporaryBufferAllocationFailed       = ( 1 << 11 ),              //!< テンポラリメモリ確保失敗
    RuntimeWarningId_GpuBufferAllocationFailed             = ( 1 << 12 ),              //!< GpuBuffer メモリ確保失敗
    RuntimeWarningId_StripeAllocationFailed                = ( 1 << 13 ),              //!< 単体ストライプインスタンスの確保失敗
    RuntimeWarningId_StripeDynamicHeapAllocationFailed     = ( 1 << 14 ),              //!< 単体ストライプ生成時に動的ヒープからのメモリ確保失敗
    RuntimeWarningId_StripeTemporaryBufferAllocationFailed = ( 1 << 15 ),              //!< 単体ストライプ描画時 テンポラリメモリ確保失敗
    RuntimeWarningId_StripeGpuBufferAllocationFailed       = ( 1 << 16 ),              //!< ストライプエミッタ生成時 Gpuバッファヒープからのメモリ確保失敗

    // 以降は内部向け警告
    RuntimeWarningId_NoParentParticleExists                = ( 1 << 17 ),              //!< 親パーティクルが存在しない
};

//---------------------------------------------------------------------------
//! @internal
//! @briefprivate ランタイムエラーコードです。
//---------------------------------------------------------------------------
enum RuntimeErrorId
{
    RuntimeErrorId_None                         = 0,                        //!< エラー無し
};

//---------------------------------------------------------------------------
//! @brief  デフォルト描画プライオリティ
//!
//!         エミッタセットソート描画を行う際の描画優先度デフォルト値です。
//!         描画ソートは、[描画プライオリティ(8Bit)|Z距離(24Bit)]のキーを元に行われます。
//---------------------------------------------------------------------------
enum DrawPriority
{
    DrawPriority_Default                        = 128                       //!< デフォルトの描画優先度
};

//---------------------------------------------------------------------------
//! @brief  パーティクルソートタイプ。
//---------------------------------------------------------------------------
enum ParticleSortType
{
    ParticleSortType_NoSort                     = 0,                        //!< ソートなし
    ParticleSortType_DescendingOrder            = 1,                        //!< 降順
    ParticleSortType_ZSort                      = 2,                        //!< Zソート
    ParticleSortType_AscendingOrder             = 3,                        //!< 昇順
};

//---------------------------------------------------------------------------
//! @brief  パーティクル形状タイプ。
//---------------------------------------------------------------------------
enum ParticleFigureType
{
    ParticleFigureType_Particle                 = 0,                        //!< パーティクル
    ParticleFigureType_Primitive                = 1,                        //!< プリミティブ
};

//---------------------------------------------------------------------------
//! @brief  ビルボードタイプ。
//---------------------------------------------------------------------------
enum ParticleBillboardType
{
    ParticleBillboardType_Billboard                         = 0,            //!< ビルボード
    ParticleBillboardType_AdvancedBillboard                 = 1,            //!< 高機能ビルボード
    ParticleBillboardType_YBillboard                        = 2,            //!< Y軸ビルボード
    ParticleBillboardType_PolygonOnXyPlane                  = 3,            //!< XY平面のポリゴン
    ParticleBillboardType_PolygonOnXzPlane                  = 4,            //!< XZ平面のポリゴン
    ParticleBillboardType_VelocityDirectionalBillboard      = 5,            //!< 速度方向を向くビルボード
    ParticleBillboardType_VelocityDirectionalPolygon        = 6,            //!< 速度方向を向くポリゴン
    ParticleBillboardType_SimpleYBillboard                  = 7,            //!< Y軸ビルボード（カメラに平行）
    ParticleBillboardType_PrimitiveLookBillboard            = 8,            //!< 立体風ビルボード
    ParticleBillboardType_PrimitiveLookDirectionalBillboard = 9,            //!< 立体風ビルボード（ディレクショナル版）
    ParticleBillboardType_Stripe                            = 10,           //!< 履歴式ストライプ
    ParticleBillboardType_MaxBillboardType,                                 //!< ビルボードタイプの総数
};

//---------------------------------------------------------------------------
//! @brief  パーティクル回転タイプ。
//---------------------------------------------------------------------------
enum ParticleRotationType
{
    ParticleRotationType_None                   = 0,                        //!< 回転しない
    ParticleRotationType_X                      = 1,                        //!< X軸回転
    ParticleRotationType_Y                      = 2,                        //!< Y軸回転
    ParticleRotationType_Z                      = 3,                        //!< Z軸回転
    ParticleRotationType_Yzx                    = 4,                        //!< YZX軸回転
    ParticleRotationType_Xyz                    = 5,                        //!< XYZ軸回転
    ParticleRotationType_Zxy                    = 6,                        //!< ZXY軸回転

    ParticleRotationType_MaxRotationType,                                   //!< 回転タイプ総数
};

//------------------------------------------------------------------------------
//! @brief  パーティクル カラー/アルファ 計算タイプ
//------------------------------------------------------------------------------
enum ParticleColorCalculationMode
{
    ParticleColorCalculationMode_FixedColor     = 0,                        //!< 固定
    ParticleColorCalculationMode_8KeysAnimation = 2,                        //!< 8キーアニメーション
    ParticleColorCalculationMode_RandomColor    = 3,                        //!< 8カラーランダム
};

//---------------------------------------------------------------------------
//! @brief  頂点アトリビュート パーティクル形状 バッファインデックス
//---------------------------------------------------------------------------
enum ParticleFigureAttributeBufferIndex
{
    ParticleFigureAttributeBufferIndex_0        = 0,    //!< パーティクル 形状 頂点バッファ 0
    ParticleFigureAttributeBufferIndex_1        = 1,    //!< パーティクル 形状 頂点バッファ 1
    ParticleFigureAttributeBufferIndex_2        = 2,    //!< パーティクル 形状 頂点バッファ 2
    ParticleFigureAttributeBufferIndex_3        = 3,    //!< パーティクル 形状 頂点バッファ 3
    ParticleFigureAttributeBufferIndex_4        = 4,    //!< パーティクル 形状 頂点バッファ 4
    ParticleFigureAttributeBufferIndex_MaxCount = 5,    //!< パーティクル 形状 頂点バッファ の最大数
};

//---------------------------------------------------------------------------
//! @brief  頂点アトリビュート パーティクルプロパティ バッファインデックス
//---------------------------------------------------------------------------
enum ParticlePropertyAttributeBufferIndex
{
    ParticlePropertyAttributeBufferIndex_Pos        = 0,    //!< パーティクル Property頂点バッファ: 位置
    ParticlePropertyAttributeBufferIndex_Vec        = 1,    //!< パーティクル Property頂点バッファ: 速度
    ParticlePropertyAttributeBufferIndex_Diff       = 2,    //!< パーティクル Property頂点バッファ: 位置差分
    ParticlePropertyAttributeBufferIndex_Scale      = 3,    //!< パーティクル Property頂点バッファ: スケール
    ParticlePropertyAttributeBufferIndex_Random     = 4,    //!< パーティクル Property頂点バッファ: ランダム
    ParticlePropertyAttributeBufferIndex_Rotate     = 5,    //!< パーティクル Property頂点バッファ: 回転
    ParticlePropertyAttributeBufferIndex_Color0     = 6,    //!< パーティクル Property頂点バッファ: カラー0
    ParticlePropertyAttributeBufferIndex_Color1     = 7,    //!< パーティクル Property頂点バッファ: カラー1
    ParticlePropertyAttributeBufferIndex_MatrixSrt0 = 8,    //!< パーティクル Property頂点バッファ: 生成時SRTマトリクス（0行目）
    ParticlePropertyAttributeBufferIndex_MatrixSrt1 = 9,   //!< パーティクル Property頂点バッファ: 生成時SRTマトリクス（1行目）
    ParticlePropertyAttributeBufferIndex_MatrixSrt2 = 10,   //!< パーティクル Property頂点バッファ: 生成時SRTマトリクス（2行目）
    ParticlePropertyAttributeBufferIndex_EmitterPlugin       = 11,   //!< EmitterPlugin 頂点バッファ
    ParticlePropertyAttributeBufferIndex_MaxIndexCount       = 12,   //!< バッファ Index の最大数
};

//---------------------------------------------------------------------------
//! @brief エミッタプラグイン用アトリビュート配列数
//---------------------------------------------------------------------------
enum EmitterPluginAttributeCount
{
    EmitterPluginAttributeType_MaxAttributeCount = 5,    //!< エミッタプラグイン用アトリビュート配列最大数
};

//---------------------------------------------------------------------------
//! @brief コンスタントバッファのインデックス
//---------------------------------------------------------------------------
enum ConstantBufferIndex
{
    ConstantBufferIndex_View                = 5,    //!< ビュー コンスタントバッファインデックス
    ConstantBufferIndex_Emitter_Static      = 6,    //!< エミッタ静的 コンスタントバッファインデックス
    ConstantBufferIndex_Emitter_Dynamic     = 7,    //!< エミッタ動的 コンスタントバッファインデックス
    ConstantBufferIndex_Emitter_Field       = 8,    //!< エミッタフィールド コンスタントバッファインデックス
    ConstantBufferIndex_Emitter_Plugin      = 9,    //!< エミッタプラグイン コンスタントバッファインデックス
    ConstantBufferIndex_Reserved_Parameter  = 10,   //!< リザーブパラメータ コンスタントバッファインデックス
    ConstantBufferIndex_CustomShader0       = 11,   //!< カスタムシェーダ0 コンスタントバッファインデックス
    ConstantBufferIndex_CustomShader1       = 12,   //!< カスタムシェーダ1 コンスタントバッファインデックス
    ConstantBufferIndex_CustomShader2       = 13,   //!< カスタムシェーダ2 コンスタントバッファインデックス
    ConstantBufferIndex_CustomShader3       = 14,   //!< カスタムシェーダ3 コンスタントバッファインデックス
};

} // namespace detail
} // namespace vfx
} // namespace nn
