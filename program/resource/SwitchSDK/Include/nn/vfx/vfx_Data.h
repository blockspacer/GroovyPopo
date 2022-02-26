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

#include <nn/vfx/vfx_Enum.h>
#include <nn/vfx/vfx_EndianUtil.h>

namespace nn {
namespace vfx {
namespace detail {

//------------------------------------------------------------------------------
//! @brief      リソースヘッダ
//------------------------------------------------------------------------------
struct ResHeader
{
    uint32_t                                          tag;                                                      //!< タグです
    uint32_t                                          version;                                                  //!< バイナリバージョンです
    uint8_t                                           name[32];                                                 //!< バイナリ名です
    uint8_t                                           cpuEndian;                                                //!< CPUのエンディアンです(0:Little/1:Big)
    uint8_t                                           gpuEndian;                                                //!< GPUのエンディアンです(0:Little/1:Big)
    uint8_t                                           targetApi;                                                //!< ターゲットAPIを表す定数値です
    uint8_t                                           targetAddressSize;                                        //!< ターゲットアドレスサイズ(sizeof(void*)で比較可能な値)です
    uint8_t                                           extraTag[4];                                              //!< 拡張タグです

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &tag );
        nn::vfx::detail::EndianUtil::Flip( &version );

    }
};

//------------------------------------------------------------------------------
//! @brief      共通データ
//------------------------------------------------------------------------------
struct ResCommon
{
    uint32_t                                          flag;                                                     //!< 汎用フラグ
    uint32_t                                          randomSeed;                                               //!< 乱数シード
    uint32_t                                          reserved0[2];                                             //!< パディング領域
    char                                              name[64];                                                 //!< アドレス解決後の名前

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &flag );
        nn::vfx::detail::EndianUtil::Flip( &randomSeed );
        detail::EndianUtil::FlipArray( 2, reserved0 );

    }
};

//------------------------------------------------------------------------------
//! @brief      描画設定データ
//------------------------------------------------------------------------------
struct ResRenderState
{
    uint8_t                                           isBlendEnable;                                            //!< ブレンド
    uint8_t                                           isDepthTest;                                              //!< デプステスト
    uint8_t                                           depthFunc;                                                //!< デプステスト通過条件
    uint8_t                                           isDepthMask;                                              //!< デプスマスク
    uint8_t                                           isAlphaTest;                                              //!< アルファテスト
    uint8_t                                           alphaFunc;                                                //!< アルファテスト通過条件
    uint8_t                                           blendType;                                                //!< フレームバッファとのブレンドタイプ
    uint8_t                                           displaySide;                                              //!< 表示面
    float                                             alphaThreshold;                                           //!< アルファ閾値
    uint8_t                                           reserved0[4];                                             //!< アライメント

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &alphaThreshold );

    }
};

//------------------------------------------------------------------------------
//! @brief      テクスチャサンプラデータ
//------------------------------------------------------------------------------
struct ResTextureSampler
{
    uint64_t                                          guid;                                                     //!< 利用するテクスチャのguid
    uint8_t                                           wrapU;                                                    //!< ラップモードU
    uint8_t                                           wrapV;                                                    //!< ラップモードV
    uint8_t                                           filter;                                                   //!< フィルタモード
    uint8_t                                           isSphereMap;                                              //!< スフィアマップを利用するかどうか
    float                                             mipLevel;                                                 //!< ミップレベルの有効値( 0.0 ～ 15.99 )
    float                                             mipMapBias;                                               //!< ミップレベルのバイアス
    uint8_t                                           mipLevelLimit;                                            //!< ミップマップレベルを制限する
    uint8_t                                           isDensityFixedU;                                          //!< テクスチャ密度固定オプションU
    uint8_t                                           isDensityFixedV;                                          //!< テクスチャ密度固定オプションV
    uint8_t                                           isSquareRgb;                                              //!< テクスチャRGB値を二乗して取得するかどうか（リニアの近似処理）
    uint8_t                                           isOnAnotherBinary;                                        //!< サブバイナリ側で省略されたテクスチャであることを示す
    uint8_t                                           reserved0[7];                                             //!< パディング領域

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &guid );
        nn::vfx::detail::EndianUtil::Flip( &mipLevel );
        nn::vfx::detail::EndianUtil::Flip( &mipMapBias );

    }
};

//------------------------------------------------------------------------------
//! @brief      8キーアニメリソース
//------------------------------------------------------------------------------
struct ResAnim8KeyParam
{
    nn::util::Float4                                  value[8];                                                 //!< 値( x, y, z, time )

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        detail::EndianUtil::FlipArray( 8, value );

    }
};

//------------------------------------------------------------------------------
//! @brief      エミッタリソース
//------------------------------------------------------------------------------
struct ResEmitterSet : public ResCommon
{
    uint32_t                                          emitterAllNum;                                            //!< 総エミッタ数(チャイルドを含める).
    uint16_t                                          UserDataBit;                                              //!< ユーザーデータビット.
    uint16_t                                          reserved0;                                                //!< パディング領域
    uint32_t                                          UserDataValue1;                                           //!< ユーザーデータ値1.
    uint32_t                                          UserDataValue2;                                           //!< ユーザーデータ値2.
    uint32_t                                          UserDataValue3;                                           //!< ユーザーデータ値3
    uint32_t                                          UserDataValue4;                                           //!< ユーザーデータ値4

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &emitterAllNum );
        nn::vfx::detail::EndianUtil::Flip( &UserDataBit );
        nn::vfx::detail::EndianUtil::Flip( &reserved0 );
        nn::vfx::detail::EndianUtil::Flip( &UserDataValue1 );
        nn::vfx::detail::EndianUtil::Flip( &UserDataValue2 );
        nn::vfx::detail::EndianUtil::Flip( &UserDataValue3 );
        nn::vfx::detail::EndianUtil::Flip( &UserDataValue4 );

    }
};

//------------------------------------------------------------------------------
//! @brief      テクスチャ座標アニメ
//------------------------------------------------------------------------------
struct ResTexUvShiftAnim
{
    nn::util::Float2                                  scrollAdd;                                                //!< スクロール加算値
    nn::util::Float2                                  scrollInit;                                               //!< スクロール初期値
    nn::util::Float2                                  scrollInitRand;                                           //!< スクロール初期値ランダム
    nn::util::Float2                                  scaleAdd;                                                 //!< スケール加算値
    nn::util::Float2                                  scaleInit;                                                //!< スケール初期値
    nn::util::Float2                                  scaleInitRand;                                            //!< スケール初期値ランダム
    float                                             rotAdd;                                                   //!< 回転加算値
    float                                             rotInit;                                                  //!< 回転初期値
    float                                             rotInitRand;                                              //!< 回転初期値ランダム
    float                                             randomType;                                               //!< 乱数タイプ
    nn::util::Float2                                  uvScale;                                                  //!< UVスケール値
    nn::util::Float2                                  uvDiv;                                                    //!< 縦横分割数

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &scrollAdd );
        nn::vfx::detail::EndianUtil::Flip( &scrollInit );
        nn::vfx::detail::EndianUtil::Flip( &scrollInitRand );
        nn::vfx::detail::EndianUtil::Flip( &scaleAdd );
        nn::vfx::detail::EndianUtil::Flip( &scaleInit );
        nn::vfx::detail::EndianUtil::Flip( &scaleInitRand );
        nn::vfx::detail::EndianUtil::Flip( &rotAdd );
        nn::vfx::detail::EndianUtil::Flip( &rotInit );
        nn::vfx::detail::EndianUtil::Flip( &rotInitRand );
        nn::vfx::detail::EndianUtil::Flip( &randomType );
        nn::vfx::detail::EndianUtil::Flip( &uvScale );
        nn::vfx::detail::EndianUtil::Flip( &uvDiv );

    }
};

//------------------------------------------------------------------------------
//! @brief      テクスチャパターンアニメ
//------------------------------------------------------------------------------
struct ResTexPtnAnim
{
    float                                             ptnTableNum;                                              //!< 利用するパターンテーブル数
    float                                             ptnFreq;                                                  //!< パターン周期
    float                                             ptnNum;                                                   //!< ランダム設定時のパターン数
    float                                             loopCount;                                                //!< サーフェイスアニメのループ回数
    int32_t                                           ptnTable[SystemParameters_MaxTexturePatternTableSize];    //!< パターンテーブル

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &ptnTableNum );
        nn::vfx::detail::EndianUtil::Flip( &ptnFreq );
        nn::vfx::detail::EndianUtil::Flip( &ptnNum );
        nn::vfx::detail::EndianUtil::Flip( &loopCount );
        detail::EndianUtil::FlipArray( SystemParameters_MaxTexturePatternTableSize, ptnTable );

    }
};

//------------------------------------------------------------------------------
//! @brief      揺らぎ
//------------------------------------------------------------------------------
struct FluctuationData
{
    nn::util::Float2                                  amplitude;                                                //!< 振幅
    nn::util::Float2                                  cycle;                                                    //!< 周期
    nn::util::Float2                                  phaseRnd;                                                 //!< 位相ランダム
    nn::util::Float2                                  phaseInit;                                                //!< 初期位相

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &amplitude );
        nn::vfx::detail::EndianUtil::Flip( &cycle );
        nn::vfx::detail::EndianUtil::Flip( &phaseRnd );
        nn::vfx::detail::EndianUtil::Flip( &phaseInit );

    }
};

//------------------------------------------------------------------------------
//! @brief      エミッタ 静的ユニフォームブロック
//------------------------------------------------------------------------------
struct EmitterStaticUniformBlock
{
    int32_t                                           flag[4];                                                  //!< 各種フラグ
    int32_t                                           color0AnimKeynum;                                         //!< カラー０アニメキー数
    int32_t                                           alpha0AnimKeyNum;                                         //!< アルファ0アニメキー数
    int32_t                                           color1AnimKeynum;                                         //!< カラー１アニメキー数
    int32_t                                           alpha1AnimKeyNum;                                         //!< アルファ1アニメキー数
    int32_t                                           scaleAnimKeyNum;                                          //!< スケールアニメキー数
    int32_t                                           shaderAnimKeyNum;                                         //!< シェーダ係数アニメキー数
    int32_t                                           reserved0[2];                                             //!< パディング領域
    float                                             color0AnimLoopRate;                                       //!< color0のアニメのループフレーム比率（寿命に対する一周期の％）
    float                                             alpha0AnimLoopRate;                                       //!< alpha0のアニメのループフレーム比率（寿命に対する一周期の％）
    float                                             color1AnimLoopRate;                                       //!< color1のアニメのループフレーム比率（寿命に対する一周期の％）
    float                                             alpha1AnimLoopRate;                                       //!< alpha1のアニメのループフレーム比率（寿命に対する一周期の％）
    float                                             scaleAnimLoopRate;                                        //!< scaleのアニメのループフレーム比率（寿命に対する一周期の％）
    float                                             color0AnimIsLoopInitRandom;                               //!< color0のアニメの初期位置ランダムの有効／無効フラグ
    float                                             alpha0AnimIsLoopInitRandom;                               //!< alpha0のアニメの初期位置ランダムの有効／無効フラグ
    float                                             color1AnimIsLoopInitRandom;                               //!< color1のアニメの初期位置ランダムの有効／無効フラグ
    float                                             alpha1AnimIsLoopInitRandom;                               //!< alpha1のアニメの初期位置ランダムの有効／無効フラグ
    float                                             scaleAnimIsLoopInitRandom;                                //!< scaleのアニメの初期位置ランダムの有効／無効フラグ
    float                                             reserved1[2];                                             //!< パディング領域
    nn::util::Float3                                  gravity;                                                  //!< 重力( x / y / z )
    float                                             gravityScale;                                             //!< 重力スケール
    float                                             airRegist;                                                //!< 重力スケール
    float                                             reserved2[3];                                             //!< パディング領域
    nn::util::Float2                                  center;                                                   //!< パーティクル中心
    float                                             offset;                                                   //!< パーティクルオフセット
    float                                             reserved3;                                                //!< パディング領域
    nn::vfx::detail::FluctuationData                  fluctuationData;                                          //!< 揺らぎデータ
    float                                             coefficient0;                                             //!< シェーダ 係数0
    float                                             coefficient1;                                             //!< シェーダ 係数1
    float                                             reserved4[2];                                             //!< パディング領域
    nn::vfx::detail::ResTexPtnAnim                    texPtnAnim0;                                              //!< テクスチャ0パターンアニメ
    nn::vfx::detail::ResTexPtnAnim                    texPtnAnim1;                                              //!< テクスチャ1パターンアニメ
    nn::vfx::detail::ResTexPtnAnim                    texPtnAnim2;                                              //!< テクスチャ2パターンアニメ
    nn::vfx::detail::ResTexUvShiftAnim                texShiftAnim0;                                            //!< テクスチャ0座標アニメ
    nn::vfx::detail::ResTexUvShiftAnim                texShiftAnim1;                                            //!< テクスチャ1座標アニメ
    nn::vfx::detail::ResTexUvShiftAnim                texShiftAnim2;                                            //!< テクスチャ2座標アニメ
    float                                             colorScale;                                               //!< カラースケール
    float                                             reserved5[3];                                             //!< パディング領域
    nn::vfx::detail::ResAnim8KeyParam                 color0Anim;                                               //!< カラー8キーアニメ
    nn::vfx::detail::ResAnim8KeyParam                 alpha0Anim;                                               //!< アルファ8キーアニメ
    nn::vfx::detail::ResAnim8KeyParam                 color1Anim;                                               //!< カラー１アニメパラメータ
    nn::vfx::detail::ResAnim8KeyParam                 alpha1Anim;                                               //!< アルファ１アニメパラメータ
    nn::util::Float2                                  softEdgeParam;                                            //!< ソフトパーティクル
    nn::util::Float2                                  fresnelAlphaParam;                                        //!< フレネルアルファ
    nn::util::Float2                                  nearDistAlphaParam;                                       //!< 近距離アルファ
    nn::util::Float2                                  farDistAlphaParam;                                        //!< 遠距離アルファ
    nn::util::Float2                                  decalParam;                                               //!< デカール
    float                                             alphaThreshold;                                           //!< アルファテストのしきい値です。
    float                                             reserved6;                                                //!< パディング領域
    float                                             addVelToScale;                                            //!< 速度をスケールに加算
    float                                             softPartcileDist;                                         //!< フェード開始距離
    float                                             softParticleVolume;                                       //!< ソフトパーティクルボリューム値
    float                                             reserved7;                                                //!< パディング領域
    nn::vfx::detail::ResAnim8KeyParam                 scale8keyAnim;                                            //!< スケール8キーアニメ
    nn::vfx::detail::ResAnim8KeyParam                 shaderAnim;                                               //!< シェーダ係数アニメ
    nn::util::Float4                                  rotateInit;                                               //!< 初期回転値( xyz / empty )
    nn::util::Float4                                  rotateInitRand;                                           //!< 初期ランダム回転( xyz / empty)
    nn::util::Float3                                  rotateAdd;                                                //!< 回転速度
    float                                             rotateRegist;                                             //!< 回転減衰率
    nn::util::Float3                                  rotateAddRand;                                            //!< 回転速度ランダム( xyz / empty )
    float                                             reserved8;                                                //!< パディング領域
    float                                             scaleLimitDistNear;                                       //!< カメラ前でのスケール抑制距離（近）
    float                                             scaleLimitDistFar;                                        //!< カメラ前でのスケール抑制距離（遠）
    float                                             reserved9[2];                                             //!< パディング領域

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        detail::EndianUtil::FlipArray( 4, flag );
        nn::vfx::detail::EndianUtil::Flip( &color0AnimKeynum );
        nn::vfx::detail::EndianUtil::Flip( &alpha0AnimKeyNum );
        nn::vfx::detail::EndianUtil::Flip( &color1AnimKeynum );
        nn::vfx::detail::EndianUtil::Flip( &alpha1AnimKeyNum );
        nn::vfx::detail::EndianUtil::Flip( &scaleAnimKeyNum );
        nn::vfx::detail::EndianUtil::Flip( &shaderAnimKeyNum );
        detail::EndianUtil::FlipArray( 2, reserved0 );
        nn::vfx::detail::EndianUtil::Flip( &color0AnimLoopRate );
        nn::vfx::detail::EndianUtil::Flip( &alpha0AnimLoopRate );
        nn::vfx::detail::EndianUtil::Flip( &color1AnimLoopRate );
        nn::vfx::detail::EndianUtil::Flip( &alpha1AnimLoopRate );
        nn::vfx::detail::EndianUtil::Flip( &scaleAnimLoopRate );
        nn::vfx::detail::EndianUtil::Flip( &color0AnimIsLoopInitRandom );
        nn::vfx::detail::EndianUtil::Flip( &alpha0AnimIsLoopInitRandom );
        nn::vfx::detail::EndianUtil::Flip( &color1AnimIsLoopInitRandom );
        nn::vfx::detail::EndianUtil::Flip( &alpha1AnimIsLoopInitRandom );
        nn::vfx::detail::EndianUtil::Flip( &scaleAnimIsLoopInitRandom );
        detail::EndianUtil::FlipArray( 2, reserved1 );
        nn::vfx::detail::EndianUtil::Flip( &gravity );
        nn::vfx::detail::EndianUtil::Flip( &gravityScale );
        nn::vfx::detail::EndianUtil::Flip( &airRegist );
        detail::EndianUtil::FlipArray( 3, reserved2 );
        nn::vfx::detail::EndianUtil::Flip( &center );
        nn::vfx::detail::EndianUtil::Flip( &offset );
        nn::vfx::detail::EndianUtil::Flip( &reserved3 );
        nn::vfx::detail::EndianUtil::Flip( &coefficient0 );
        nn::vfx::detail::EndianUtil::Flip( &coefficient1 );
        detail::EndianUtil::FlipArray( 2, reserved4 );
        nn::vfx::detail::EndianUtil::Flip( &colorScale );
        detail::EndianUtil::FlipArray( 3, reserved5 );
        nn::vfx::detail::EndianUtil::Flip( &softEdgeParam );
        nn::vfx::detail::EndianUtil::Flip( &fresnelAlphaParam );
        nn::vfx::detail::EndianUtil::Flip( &nearDistAlphaParam );
        nn::vfx::detail::EndianUtil::Flip( &farDistAlphaParam );
        nn::vfx::detail::EndianUtil::Flip( &decalParam );
        nn::vfx::detail::EndianUtil::Flip( &alphaThreshold );
        nn::vfx::detail::EndianUtil::Flip( &reserved6 );
        nn::vfx::detail::EndianUtil::Flip( &addVelToScale );
        nn::vfx::detail::EndianUtil::Flip( &softPartcileDist );
        nn::vfx::detail::EndianUtil::Flip( &softParticleVolume );
        nn::vfx::detail::EndianUtil::Flip( &reserved7 );
        nn::vfx::detail::EndianUtil::Flip( &rotateInit );
        nn::vfx::detail::EndianUtil::Flip( &rotateInitRand );
        nn::vfx::detail::EndianUtil::Flip( &rotateAdd );
        nn::vfx::detail::EndianUtil::Flip( &rotateRegist );
        nn::vfx::detail::EndianUtil::Flip( &rotateAddRand );
        nn::vfx::detail::EndianUtil::Flip( &reserved8 );
        nn::vfx::detail::EndianUtil::Flip( &scaleLimitDistNear );
        nn::vfx::detail::EndianUtil::Flip( &scaleLimitDistFar );
        detail::EndianUtil::FlipArray( 2, reserved9 );

        fluctuationData.FlipEndian();
        texPtnAnim0.FlipEndian();
        texPtnAnim1.FlipEndian();
        texPtnAnim2.FlipEndian();
        texShiftAnim0.FlipEndian();
        texShiftAnim1.FlipEndian();
        texShiftAnim2.FlipEndian();
        color0Anim.FlipEndian();
        alpha0Anim.FlipEndian();
        color1Anim.FlipEndian();
        alpha1Anim.FlipEndian();
        scale8keyAnim.FlipEndian();
        shaderAnim.FlipEndian();
    }
};

//------------------------------------------------------------------------------
//! @brief      8キーアニメ情報
//------------------------------------------------------------------------------
struct ResAnim8KeyParamSet
{
    uint32_t                                          enable;                                                   //!< 有効無効
    uint32_t                                          loop;                                                     //!< ループ再生
    uint32_t                                          startRandom;                                              //!< 開始位置ランダム
    int32_t                                           keyNum;                                                   //!< キー数
    int32_t                                           loopNum;                                                  //!< ループフレーム数
    nn::util::Float4                                  keyValue[8];                                              //!< キーデータです。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &enable );
        nn::vfx::detail::EndianUtil::Flip( &loop );
        nn::vfx::detail::EndianUtil::Flip( &startRandom );
        nn::vfx::detail::EndianUtil::Flip( &keyNum );
        nn::vfx::detail::EndianUtil::Flip( &loopNum );
        detail::EndianUtil::FlipArray( 8, keyValue );

    }
};

//------------------------------------------------------------------------------
//! @brief      新ランダム フィールド情報
//------------------------------------------------------------------------------
struct ResFieldRandom
{
    uint8_t                                           enableUnifiedPhase;                                       //!< 固定された乱数の種を使うかどうかのフラグ
    uint8_t                                           enableDetailedOption;                                     //!< 詳細設定を有効
    uint8_t                                           enableAirRegist;                                          //!< 空気抵抗の有無
    uint8_t                                           reserved0;                                                //!< パディング領域
    nn::util::Float3                                  randomVel;                                                //!< blank毎に加算するランダム速度
    int32_t                                           blank;                                                    //!< ランダムをかけるタイミング
    float                                             unifiedPhaseSpeed;                                        //!< 統一位相の変化速度。
    float                                             unifiedPhaseDistribution;                                 //!< 統一位相の幅
    float                                             waveParam0;                                               //!< 波形重み0
    float                                             waveParam1;                                               //!< 波形重み1
    float                                             waveParam2;                                               //!< 波形重み2
    float                                             waveParam3;                                               //!< 波形重み3
    float                                             waveParamHzRate0;                                         //!< 成分0の周波数の、基本周波数に対する比率
    float                                             waveParamHzRate1;                                         //!< 成分1の周波数の、基本周波数に対する比率
    float                                             waveParamHzRate2;                                         //!< 成分2の周波数の、基本周波数に対する比率
    float                                             waveParamHzRate3;                                         //!< 成分3の周波数の、基本周波数に対する比率
    nn::vfx::detail::ResAnim8KeyParamSet              animParam;                                                //!< パーティクル時間アニメのデータです

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &randomVel );
        nn::vfx::detail::EndianUtil::Flip( &blank );
        nn::vfx::detail::EndianUtil::Flip( &unifiedPhaseSpeed );
        nn::vfx::detail::EndianUtil::Flip( &unifiedPhaseDistribution );
        nn::vfx::detail::EndianUtil::Flip( &waveParam0 );
        nn::vfx::detail::EndianUtil::Flip( &waveParam1 );
        nn::vfx::detail::EndianUtil::Flip( &waveParam2 );
        nn::vfx::detail::EndianUtil::Flip( &waveParam3 );
        nn::vfx::detail::EndianUtil::Flip( &waveParamHzRate0 );
        nn::vfx::detail::EndianUtil::Flip( &waveParamHzRate1 );
        nn::vfx::detail::EndianUtil::Flip( &waveParamHzRate2 );
        nn::vfx::detail::EndianUtil::Flip( &waveParamHzRate3 );

        animParam.FlipEndian();
    }
};

//------------------------------------------------------------------------------
//! @brief      エミッタ基本情報
//------------------------------------------------------------------------------
struct ResEmitterFnd
{
    uint8_t                                           isParticleDraw;                                           //!< パーティクルを描画する
    uint8_t                                           sortType;                                                 //!< パーティクルソートタイプ
    uint8_t                                           calcType;                                                 //!< 挙動計算タイプ
    uint8_t                                           followType;                                               //!< エミッタ追従タイプ
    uint8_t                                           isFadeEmit;                                               //!< 終了処理で放出停止するか
    uint8_t                                           isFadeAlphaFade;                                          //!< 終了処理でアルファフェードアウトするか
    uint8_t                                           isScaleFade;                                              //!< スケールフェードアウトの有無
    uint8_t                                           randomSeedType;                                           //!< 乱数シードのタイプ
    uint8_t                                           isUpdateMatrixByEmit;                                     //!< 放出ごとにマトリクスを更新
    uint8_t                                           testAlways;                                               //!< 常時判定するか
    uint8_t                                           interpolateEmissionAmount;                                //!< 放出量を補間するか
    uint8_t                                           isAlphaFadeIn;                                            //!< αフェードインの有無
    uint8_t                                           isScaleFadeIn;                                            //!< スケールフェードインの有無
    uint8_t                                           dummy[3];                                                 //!< パディング領域
    uint32_t                                          randomSeed;                                               //!< 乱数の種
    uint32_t                                          drawPath;                                                 //!< 描画パス
    uint32_t                                          alphaFadeTime;                                            //!< アルファフェードアウト時間
    uint32_t                                          fadeInTime;                                               //!< フェードイン時間
    nn::util::Float3                                  trans;                                                    //!< エミッタ位置値
    nn::util::Float3                                  transRand;                                                //!< マトリクス移動量ランダム
    nn::util::Float3                                  rotate;                                                   //!< エミッタ回転値
    nn::util::Float3                                  rotateRand;                                               //!< マトリクス回転ランダム
    nn::util::Float3                                  scale;                                                    //!< エミッタスケール値
    nn::util::Float4                                  color0;                                                   //!< エミッタカラー0
    nn::util::Float4                                  color1;                                                   //!< エミッタカラー1
    float                                             emissionRangeNear;                                        //!< 放出範囲近距離
    float                                             emissionRangeFar;                                         //!< 放出範囲遠距離
    int32_t                                           emissionRatioFar;                                         //!< 遠距離での放出割合

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &randomSeed );
        nn::vfx::detail::EndianUtil::Flip( &drawPath );
        nn::vfx::detail::EndianUtil::Flip( &alphaFadeTime );
        nn::vfx::detail::EndianUtil::Flip( &fadeInTime );
        nn::vfx::detail::EndianUtil::Flip( &trans );
        nn::vfx::detail::EndianUtil::Flip( &transRand );
        nn::vfx::detail::EndianUtil::Flip( &rotate );
        nn::vfx::detail::EndianUtil::Flip( &rotateRand );
        nn::vfx::detail::EndianUtil::Flip( &scale );
        nn::vfx::detail::EndianUtil::Flip( &color0 );
        nn::vfx::detail::EndianUtil::Flip( &color1 );
        nn::vfx::detail::EndianUtil::Flip( &emissionRangeNear );
        nn::vfx::detail::EndianUtil::Flip( &emissionRangeFar );
        nn::vfx::detail::EndianUtil::Flip( &emissionRatioFar );

    }
};

//------------------------------------------------------------------------------
//! @brief      エミッタ形状情報
//------------------------------------------------------------------------------
struct ResEmitterVolume
{
    uint8_t                                           volumeType;                                               //!< ボリュームタイプ
    uint8_t                                           sweepStartRandom;                                         //!< 弧の幅 ランダム
    uint8_t                                           arcType;                                                  //!< 弧のタイプ
    uint8_t                                           isVolumeLatitudeEnabled;                                  //!< 未使用
    uint8_t                                           volumeTblIndex;                                           //!< 球形状ボリュームテーブルインデックス
    uint8_t                                           volumeTblIndex64;                                         //!< 球64形状ボリュームテーブルインデックス
    uint8_t                                           volumeLatitudeDir;                                        //!< 球の緯度の方向
    uint8_t                                           isGpuEmitter;                                             //!< 仮住まいです。GPUエミッタの有効無効。
    float                                             sweepLongitude;                                           //!< 弧の計算に使う値
    float                                             sweepLatitude;                                            //!< 弧の計算に使う緯度
    float                                             sweepStart;                                               //!< 弧の幅(開始)
    float                                             volumeSurfacePosRand;                                     //!< エミッタ形状表面上のランダム位置
    float                                             caliberRatio;                                             //!< 内径（比率）
    float                                             lineCenter;                                               //!< ラインセンター
    float                                             lineLength;                                               //!< ライン長
    nn::util::Float3                                  volumeRadius;                                             //!< ボリューム半径
    nn::util::Float3                                  volumeFormScale;                                          //!< エミッタ形状スケール
    uint32_t                                          primEmitType;                                             //!< プリミティブ指定時の放出タイプ
    uint64_t                                          primitiveIndex;                                           //!< プリミティブインデックスです.
    uint32_t                                          numDivideCircle;                                          //!< 円等分割の分割数
    uint32_t                                          numDivideCircleRandom;                                    //!< 分割数ランダム（円等分割）
    uint32_t                                          numDivideLine;                                            //!< 線等分割の分割数
    uint32_t                                          numDivideLineRandom;                                      //!< 分割数ランダム（線等分割）
    uint8_t                                           isOnAnotherBinaryVolumePrimitive;                         //!< エミッタ形状プリミティブが他のバイナリにあるかどうか
    uint8_t                                           reserved0[7];                                             //!< パディング領域

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &sweepLongitude );
        nn::vfx::detail::EndianUtil::Flip( &sweepLatitude );
        nn::vfx::detail::EndianUtil::Flip( &sweepStart );
        nn::vfx::detail::EndianUtil::Flip( &volumeSurfacePosRand );
        nn::vfx::detail::EndianUtil::Flip( &caliberRatio );
        nn::vfx::detail::EndianUtil::Flip( &lineCenter );
        nn::vfx::detail::EndianUtil::Flip( &lineLength );
        nn::vfx::detail::EndianUtil::Flip( &volumeRadius );
        nn::vfx::detail::EndianUtil::Flip( &volumeFormScale );
        nn::vfx::detail::EndianUtil::Flip( &primEmitType );
        nn::vfx::detail::EndianUtil::Flip( &primitiveIndex );
        nn::vfx::detail::EndianUtil::Flip( &numDivideCircle );
        nn::vfx::detail::EndianUtil::Flip( &numDivideCircleRandom );
        nn::vfx::detail::EndianUtil::Flip( &numDivideLine );
        nn::vfx::detail::EndianUtil::Flip( &numDivideLineRandom );

    }
};

//------------------------------------------------------------------------------
//! @brief      エミッタ放出情報
//------------------------------------------------------------------------------
struct ResEmission
{
    uint8_t                                           isOneTime;                                                //!< ワンタイム
    uint8_t                                           isWorldGravity;                                           //!< ワールド座標系で重力を適用する
    uint8_t                                           isEmitDistEnabled;                                        //!< 距離放出を使うかどうか
    uint8_t                                           isWorldOrientedVelocity;                                  //!< 指定方向初速をワールド座標系で適用
    uint32_t                                          start;                                                    //!< 放出開始フレーム
    uint32_t                                          emitTiming;                                               //!< 放出開始タイミング
    uint32_t                                          emitDuration;                                             //!< 放出時間
    float                                             rate;                                                     //!< 放出レート
    int32_t                                           rateRandom;                                               //!< 放出レートランダム
    int32_t                                           interval;                                                 //!< 放出間隔
    int32_t                                           intervalRandom;                                           //!< 放出間隔ランダム
    float                                             posRand;                                                  //!< 初期位置のランダム量
    float                                             gravityScale;                                             //!< 重力スケール
    nn::util::Float3                                  gravityDir;                                               //!< 重力方向
    float                                             emitDistUnit;                                             //!< 放出間隔（距離）
    float                                             emitDistMin;                                              //!< 1フレームに認める最小移動距離
    float                                             emitDistMax;                                              //!< 1フレームに認める最大移動距離
    float                                             emitDistMargin;                                           //!< 移動距離切り捨ての閾値
    int32_t                                           emitDistParticleMax;                                      //!< 距離放出の場合の、パーティクル放出の最大数

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &start );
        nn::vfx::detail::EndianUtil::Flip( &emitTiming );
        nn::vfx::detail::EndianUtil::Flip( &emitDuration );
        nn::vfx::detail::EndianUtil::Flip( &rate );
        nn::vfx::detail::EndianUtil::Flip( &rateRandom );
        nn::vfx::detail::EndianUtil::Flip( &interval );
        nn::vfx::detail::EndianUtil::Flip( &intervalRandom );
        nn::vfx::detail::EndianUtil::Flip( &posRand );
        nn::vfx::detail::EndianUtil::Flip( &gravityScale );
        nn::vfx::detail::EndianUtil::Flip( &gravityDir );
        nn::vfx::detail::EndianUtil::Flip( &emitDistUnit );
        nn::vfx::detail::EndianUtil::Flip( &emitDistMin );
        nn::vfx::detail::EndianUtil::Flip( &emitDistMax );
        nn::vfx::detail::EndianUtil::Flip( &emitDistMargin );
        nn::vfx::detail::EndianUtil::Flip( &emitDistParticleMax );

    }
};

//------------------------------------------------------------------------------
//! @brief      パーティクル情報
//------------------------------------------------------------------------------
struct ResPtclFnd
{
    uint8_t                                           isLifeInfinity;                                           //!< 無限寿命
    uint8_t                                           isTriming;                                                //!< トリミング
    uint8_t                                           billboardType;                                            //!< ビルボードタイプ
    uint8_t                                           rotType;                                                  //!< 回転タイプ
    uint8_t                                           offsetType;                                               //!< カメラ・デプスオフセットタイプ
    uint8_t                                           rotRevRandX;                                              //!< 回転方向ランダムX
    uint8_t                                           rotRevRandY;                                              //!< 回転方向ランダムY
    uint8_t                                           rotRevRandZ;                                              //!< 回転方向ランダムZ
    uint8_t                                           isRotateX;                                                //!< 回転Xを利用するか
    uint8_t                                           isRotateY;                                                //!< 回転Yを利用するか
    uint8_t                                           isRotateZ;                                                //!< 回転Zを利用するか
    uint8_t                                           primitiveScaleType;                                       //!< プリミティブスケール適用タイプ
    uint8_t                                           isTextureCommonRandom;                                    //!< テクスチャのランダマイズを共通化する
    uint8_t                                           connectPtclScaleAndZOffset;                               //!< パーティクルスケールとZオフセットを連動させる
    uint8_t                                           enableAvoidZFighting;                                     //!< Z-Fighting 回避のためのオフセットを入れるかどうか
    uint8_t                                           reserved0[1];                                             //!< パディング領域
    int32_t                                           life;                                                     //!< ライフ
    int32_t                                           lifeRandom;                                               //!< ライフランダム
    float                                             dynamicsRand;                                             //!< 運動量ランダム
    uint32_t                                          primitiveVertexInfoFlags;                                 //!< 使用しているプリミティブの頂点が持つデータの種類を示すビットフラグです。下位ビットから順に、Position/Normal/Tangent/Color/UV0/UV1を表します。
    uint64_t                                          primitiveIndex;                                           //!< 利用するプリミティブインデックスです。
    uint64_t                                          trimmingPrimitiveIndex;                                   //!< 利用するトリミングプリミティブインデックスです。
    uint8_t                                           color0AnimIsLoop;                                         //!< color0のアニメのループフラグ
    uint8_t                                           alpha0AnimIsLoop;                                         //!< alpha0のアニメのループフラグ
    uint8_t                                           color1AnimIsLoop;                                         //!< color1のアニメのループフラグ
    uint8_t                                           alpha1AnimIsLoop;                                         //!< alpha1のアニメのループフラグ
    uint8_t                                           scaleAnimIsLoop;                                          //!< scaleのアニメのループフラグ
    uint8_t                                           color0AnimIsLoopInitRandom;                               //!< color0のアニメの初期位置ランダムの有効／無効フラグ
    uint8_t                                           alpha0AnimIsLoopInitRandom;                               //!< alpha0のアニメの初期位置ランダムの有効／無効フラグ
    uint8_t                                           color1AnimIsLoopInitRandom;                               //!< color1のアニメの初期位置ランダムの有効／無効フラグ
    uint8_t                                           alpha1AnimIsLoopInitRandom;                               //!< alpha1のアニメの初期位置ランダムの有効／無効フラグ
    uint8_t                                           scaleAnimIsLoopInitRandom;                                //!< scaleのアニメの初期位置ランダムの有効／無効フラグ
    uint8_t                                           isOnAnotherBinaryPrimitive;                               //!< プリミティブが他のバイナリにあるかどうか
    uint8_t                                           isOnAnotherBinaryTrimmingPrimitive;                       //!< トリミングプリミティブが他のバイナリにあるかどうか
    int32_t                                           color0AnimLoopRate;                                       //!< color0のアニメのループフレーム比率（寿命に対する一周期の％）
    int32_t                                           alpha0AnimLoopRate;                                       //!< alpha0のアニメのループフレーム比率（寿命に対する一周期の％）
    int32_t                                           color1AnimLoopRate;                                       //!< color1のアニメのループフレーム比率（寿命に対する一周期の％）
    int32_t                                           alpha1AnimLoopRate;                                       //!< alpha1のアニメのループフレーム比率（寿命に対する一周期の％）
    int32_t                                           scaleAnimLoopRate;                                        //!< scaleのアニメのループフレーム比率（寿命に対する一周期の％）

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &life );
        nn::vfx::detail::EndianUtil::Flip( &lifeRandom );
        nn::vfx::detail::EndianUtil::Flip( &dynamicsRand );
        nn::vfx::detail::EndianUtil::Flip( &primitiveVertexInfoFlags );
        nn::vfx::detail::EndianUtil::Flip( &primitiveIndex );
        nn::vfx::detail::EndianUtil::Flip( &trimmingPrimitiveIndex );
        nn::vfx::detail::EndianUtil::Flip( &color0AnimLoopRate );
        nn::vfx::detail::EndianUtil::Flip( &alpha0AnimLoopRate );
        nn::vfx::detail::EndianUtil::Flip( &color1AnimLoopRate );
        nn::vfx::detail::EndianUtil::Flip( &alpha1AnimLoopRate );
        nn::vfx::detail::EndianUtil::Flip( &scaleAnimLoopRate );

    }
};

//------------------------------------------------------------------------------
//! @brief      エミッタコンバイナ情報
//------------------------------------------------------------------------------
struct ResCombiner
{
    uint8_t                                           colorCombinerProcess;                                     //!< カラー計算式タイプ
    uint8_t                                           alphaCombinerProcess;                                     //!< アルファ計算式タイプ
    uint8_t                                           texture1ColorBlend;                                       //!< テクスチャ1カラーの上段との合成
    uint8_t                                           texture2ColorBlend;                                       //!< テクスチャ2カラーの上段との合成
    uint8_t                                           primitiveColorBlend;                                      //!< プリミティブカラーの上段との合成
    uint8_t                                           texture1AlphaBlend;                                       //!< テクスチャ1アルファの上段との合成
    uint8_t                                           texture2AlphaBlend;                                       //!< テクスチャ2アルファの上段との合成
    uint8_t                                           primitiveAlphaBlend;                                      //!< プリミティブアルファの上段との合成
    uint8_t                                           texColor0InputType;                                       //!< テクスチャカラー0入力タイプ
    uint8_t                                           texColor1InputType;                                       //!< テクスチャカラー1入力タイプ
    uint8_t                                           texColor2InputType;                                       //!< テクスチャカラー2入力タイプ
    uint8_t                                           texAlpha0InputType;                                       //!< テクスチャアルファ0入力タイプ
    uint8_t                                           texAlpha1InputType;                                       //!< テクスチャアルファ1入力タイプ
    uint8_t                                           texAlpha2InputType;                                       //!< テクスチャアルファ2入力タイプ
    uint8_t                                           primitiveColorInputType;                                  //!< プリミティブカラー入力タイプ
    uint8_t                                           primitiveAlphaInputType;                                  //!< プリミティブアルファ入力タイプ
    uint8_t                                           shaderType;                                               //!< シェーダタイプ
    uint8_t                                           applyAlpha;                                               //!< 屈折シェーダ/アルファ値を反映
    uint8_t                                           isDistortionByCameraDistance;                             //!< カメラ距離によって屈折強度を強くするかどうかのフラグ
    uint8_t                                           reserved0[5];                                             //!< パディング領域

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        /* DO_NOTHING */

    }
};

//------------------------------------------------------------------------------
//! @brief      シェーダ情報
//------------------------------------------------------------------------------
struct ResShader
{
    uint8_t                                           shaderType;                                               //!< シェーダタイプ
    uint8_t                                           reserved0[3];                                             //!< パディング領域
    uint32_t                                          shaderIndex;                                              //!< 利用するシェーダインデックス
    uint32_t                                          computeShaderIndex;                                       //!< 利用するコンピュートシェーダインデックス
    uint32_t                                          userShaderIndex1;                                         //!< ユーザーシェーダインデックス1
    uint32_t                                          userShaderIndex2;                                         //!< ユーザーシェーダインデックス2
    uint32_t                                          customShaderIndex;                                        //!< カスタムシェーダインデックス
    uint64_t                                          customShaderFlag;                                         //!< カスタムシェーダフラグ
    uint64_t                                          customShaderSwitch;                                       //!< ラジオボタンによるスイッチ選択状態です。
    uint32_t                                          eftCombinerShaderIndex;                                   //!< エフェクトコンバイナが生成したシェーダのインデックス
    uint32_t                                          reserved1;                                                //!< パディング領域
    char                                              userShaderDefine1[16];                                    //!< ユーザーシェーダ定義1
    char                                              userShaderDefine2[16];                                    //!< ユーザーシェーダ定義2

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &shaderIndex );
        nn::vfx::detail::EndianUtil::Flip( &computeShaderIndex );
        nn::vfx::detail::EndianUtil::Flip( &userShaderIndex1 );
        nn::vfx::detail::EndianUtil::Flip( &userShaderIndex2 );
        nn::vfx::detail::EndianUtil::Flip( &customShaderIndex );
        nn::vfx::detail::EndianUtil::Flip( &customShaderFlag );
        nn::vfx::detail::EndianUtil::Flip( &customShaderSwitch );
        nn::vfx::detail::EndianUtil::Flip( &eftCombinerShaderIndex );
        nn::vfx::detail::EndianUtil::Flip( &reserved1 );

    }
};

//------------------------------------------------------------------------------
//! @brief      パーティクル初速情報
//------------------------------------------------------------------------------
struct ResPtclVel
{
    float                                             allDirection;                                             //!< 全方向初速
    float                                             designatedDirScale;                                       //!< 指定方向速度
    nn::util::Float3                                  designatedDir;                                            //!< 指定方向
    float                                             diffusionDirAngle;                                        //!< 指定方向拡散角度
    float                                             xzDiffusion;                                              //!< Y軸拡散速度
    nn::util::Float3                                  diffusion;                                                //!< 拡散初速
    float                                             velRandom;                                                //!< 速度ランダム
    float                                             emVelInherit;                                             //!< エミッタ速度継承率

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &allDirection );
        nn::vfx::detail::EndianUtil::Flip( &designatedDirScale );
        nn::vfx::detail::EndianUtil::Flip( &designatedDir );
        nn::vfx::detail::EndianUtil::Flip( &diffusionDirAngle );
        nn::vfx::detail::EndianUtil::Flip( &xzDiffusion );
        nn::vfx::detail::EndianUtil::Flip( &diffusion );
        nn::vfx::detail::EndianUtil::Flip( &velRandom );
        nn::vfx::detail::EndianUtil::Flip( &emVelInherit );

    }
};

//------------------------------------------------------------------------------
//! @brief      パーティクルカラー情報
//------------------------------------------------------------------------------
struct ResPtclColor
{
    uint8_t                                           isSoftParticle;                                           //!< ソフトパーティクル
    uint8_t                                           isFresnelAlpha;                                           //!< フレネルアルファ
    uint8_t                                           isNearDistAlpha;                                          //!< 近距離アルファ
    uint8_t                                           isFarDistAlpha;                                           //!< 遠距離アルファ
    uint8_t                                           isDecal;                                                  //!< デカール
    uint8_t                                           dummy[3];                                                 //!< ダミー
    uint8_t                                           color0Type;                                               //!< カラー0挙動タイプ
    uint8_t                                           color1Type;                                               //!< カラー1挙動タイプ
    uint8_t                                           alpha0Type;                                               //!< アルファ0挙動タイプ
    uint8_t                                           alpha1Type;                                               //!< アルファ1挙動タイプ
    nn::util::Float3                                  color0;                                                   //!< カラー0
    float                                             alpha0;                                                   //!< アルファ0
    nn::util::Float3                                  color1;                                                   //!< カラー1
    float                                             alpha1;                                                   //!< アルファ1

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &color0 );
        nn::vfx::detail::EndianUtil::Flip( &alpha0 );
        nn::vfx::detail::EndianUtil::Flip( &color1 );
        nn::vfx::detail::EndianUtil::Flip( &alpha1 );

    }
};

//------------------------------------------------------------------------------
//! @brief      パーティクルスケール情報
//------------------------------------------------------------------------------
struct ResPtclScale
{
    nn::util::Float3                                  base;                                                     //!< 基準スケール
    nn::util::Float3                                  baseRandom;                                               //!< 基準スケールランダム
    uint8_t                                           enableScalingByCameraDistNear;                            //!< カメラ距離スケール「近」のON/OFF
    uint8_t                                           enableScalingByCameraDistFar;                             //!< カメラ距離スケール「遠」のON/OFF
    uint8_t                                           enableAddScaleY;                                          //!< 速度スケールY
    uint8_t                                           enableLinkFovyToScaleValue;                               //!< スケール制限に画角を連動させる
    float                                             scaleLimitDistNear;                                       //!< パーティクルスケール制限距離（近）
    float                                             scaleLimitDistFar;                                        //!< パーティクルスケール制限距離（遠）

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &base );
        nn::vfx::detail::EndianUtil::Flip( &baseRandom );
        nn::vfx::detail::EndianUtil::Flip( &scaleLimitDistNear );
        nn::vfx::detail::EndianUtil::Flip( &scaleLimitDistFar );

    }
};

//------------------------------------------------------------------------------
//! @brief      パーティクル揺らぎ情報
//------------------------------------------------------------------------------
struct ResPtclFluctuation
{
    uint8_t                                           isApplyAlpha;                                             //!< アルファに適用するかどうか
    uint8_t                                           isApplayScale;                                            //!< スケールに適用するかどうか
    uint8_t                                           isApplayScaleY;                                           //!< Y軸を個別に設定する
    uint8_t                                           isWaveType;                                               //!< 揺らぎ波形タイプ
    uint8_t                                           isPhaseRandomX;                                           //!< 依存ランダム X
    uint8_t                                           isPhaseRandomY;                                           //!< 位相ランダム X
    uint8_t                                           reserved0[6];                                             //!< ダミー

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        /* DO_NOTHING */

    }
};

//------------------------------------------------------------------------------
//! @brief      テクスチャアニメ情報

//------------------------------------------------------------------------------
struct ResTextureAnim
{
    uint8_t                                           patternAnimType;                                          //!< パターンアニメタイプ
    uint8_t                                           isScroll;                                                 //!< UVスクロールアニメ有効無効
    uint8_t                                           isRotate;                                                 //!< UV回転アニメ有効無効
    uint8_t                                           isScale;                                                  //!< UVスケールアニメ有効無効
    uint8_t                                           repeat;                                                   //!< 繰り返し数
    uint8_t                                           invRandU;                                                 //!< U 反転ランダム
    uint8_t                                           invRandV;                                                 //!< V 反転ランダム
    uint8_t                                           isPatAnimLoopRandom;                                      //!< テクスチャパタンアニメ ループ開始ランダム
    uint8_t                                           uvChannel;                                                //!< プリミティブのUVチャンネル
    uint8_t                                           isCrossfade;                                              //!< クロスフェードの有効無効
    uint8_t                                           reserved0[6];                                             //!< パディング領域

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        /* DO_NOTHING */

    }
};

//------------------------------------------------------------------------------
//! @brief      継承パラメータ
//------------------------------------------------------------------------------
struct ResInherit
{
    uint8_t                                           velocity;                                                 //!< 速度継承
    uint8_t                                           scale;                                                    //!< スケール継承
    uint8_t                                           rotate;                                                   //!< 回転継承
    uint8_t                                           colorScale;                                               //!< カラースケール継承
    uint8_t                                           color0;                                                   //!< カラー0継承
    uint8_t                                           color1;                                                   //!< カラー1継承
    uint8_t                                           alpha0;                                                   //!< アルファ0継承
    uint8_t                                           alpha1;                                                   //!< アルファ1継承
    uint8_t                                           drawPath;                                                 //!< 描画パスを継承
    uint8_t                                           preDraw;                                                  //!< 継承先より先に描画
    uint8_t                                           alpha0EachFrame;                                          //!< アルファ0を毎フレーム継承するかどうか
    uint8_t                                           alpha1EachFrame;                                          //!< アルファ1を毎フレーム継承するかどうか
    uint8_t                                           enableEmitterParticle;                                    //!< パーティクルごとにエミッタを生成するかどうか
    uint8_t                                           reserved0[3];                                             //!< パディング領域
    float                                             velocityRate;                                             //!< 速度継承率
    float                                             scaleRate;                                                //!< スケール継承

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &velocityRate );
        nn::vfx::detail::EndianUtil::Flip( &scaleRate );

    }
};

//------------------------------------------------------------------------------
//! @brief      Holds custom action data.
//------------------------------------------------------------------------------
struct ResAction
{
    int32_t                                           customActionIndex;                                        //!< The selected custom action index.

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &customActionIndex );

    }
};

//------------------------------------------------------------------------------
//! @brief      エミッタリソース
//------------------------------------------------------------------------------
struct ResEmitter : public ResCommon
{
    nn::vfx::detail::EmitterStaticUniformBlock        staticUbo;                                                //!< GPUに転送されるデータ( TODO:先頭を 0x100 アラインする )
    nn::vfx::detail::ResEmitterFnd                    emitter;                                                  //!< エミッタ基本
    nn::vfx::detail::ResInherit                       inherit;                                                  //!< 継承パラメータ
    nn::vfx::detail::ResEmission                      emission;                                                 //!< エミッタ放出
    nn::vfx::detail::ResEmitterVolume                 volume;                                                   //!< エミッタボリューム
    nn::vfx::detail::ResRenderState                   renderState;                                              //!< 描画設定
    nn::vfx::detail::ResPtclFnd                       ptcl;                                                     //!< パーティクル基本情報
    nn::vfx::detail::ResCombiner                      combiner;                                                 //!< コンバイナ情報
    nn::vfx::detail::ResShader                        shader;                                                   //!< シェーダ情報
    nn::vfx::detail::ResAction                        action;                                                   //!< The custom action data.
    nn::vfx::detail::ResPtclVel                       ptclVel;                                                  //!< パーティクル速度情報
    nn::vfx::detail::ResPtclColor                     ptclColor;                                                //!< パーティクルカラー情報
    nn::vfx::detail::ResPtclScale                     ptclScale;                                                //!< パーティクルスケール情報
    nn::vfx::detail::ResPtclFluctuation               ptclFluctuation;                                          //!< 揺らぎ設定
    nn::vfx::detail::ResTextureSampler                textureSampler0;                                          //!< テクスチャサンプラー
    nn::vfx::detail::ResTextureSampler                textureSampler1;                                          //!< テクスチャサンプラ
    nn::vfx::detail::ResTextureSampler                textureSampler2;                                          //!< テクスチャサンプラ
    nn::vfx::detail::ResTextureAnim                   textureAnim0;                                             //!< テクスチャアニメ0
    nn::vfx::detail::ResTextureAnim                   textureAnim1;                                             //!< テクスチャアニメ1
    nn::vfx::detail::ResTextureAnim                   textureAnim2;                                             //!< テクスチャアニメ2
    nn::util::Float4                                  reservedArea[4];                                          //!< バイナリの予約領域

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        detail::EndianUtil::FlipArray( 4, reservedArea );

        staticUbo.FlipEndian();
        emitter.FlipEndian();
        inherit.FlipEndian();
        emission.FlipEndian();
        volume.FlipEndian();
        renderState.FlipEndian();
        ptcl.FlipEndian();
        combiner.FlipEndian();
        shader.FlipEndian();
        action.FlipEndian();
        ptclVel.FlipEndian();
        ptclColor.FlipEndian();
        ptclScale.FlipEndian();
        ptclFluctuation.FlipEndian();
        textureSampler0.FlipEndian();
        textureSampler1.FlipEndian();
        textureSampler2.FlipEndian();
        textureAnim0.FlipEndian();
        textureAnim1.FlipEndian();
        textureAnim2.FlipEndian();
    }
};

//------------------------------------------------------------------------------
//! @brief      アニメーションのキー
//------------------------------------------------------------------------------
struct ResAnimKey
{
    nn::util::Float3                                  value;                                                    //!< 値
    float                                             time;                                                     //!< 時間

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &value );
        nn::vfx::detail::EndianUtil::Flip( &time );

    }
};

//------------------------------------------------------------------------------
//! @brief      マグネット フィールド情報
//------------------------------------------------------------------------------
struct ResFieldMagnet
{
    uint8_t                                           isFollowEmitter;                                          //!< エミッタに追従するか？
    uint8_t                                           fieldMagnetFlagX;                                         //!< フラグX
    uint8_t                                           fieldMagnetFlagY;                                         //!< フラグY
    uint8_t                                           fieldMangetFlagZ;                                         //!< フラグZ
    float                                             fieldMagnetPower;                                         //!< 磁力
    nn::util::Float3                                  fieldMagnetPos;                                           //!< 磁力位置
    nn::vfx::detail::ResAnim8KeyParamSet              animParam;                                                //!< パーティクル時間アニメのデータです

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &fieldMagnetPower );
        nn::vfx::detail::EndianUtil::Flip( &fieldMagnetPos );

        animParam.FlipEndian();
    }
};

//------------------------------------------------------------------------------
//! @brief      スピン フィールド情報
//------------------------------------------------------------------------------
struct ResFieldSpinData
{
    float                                             fieldSpinRotate;                                          //!< 回転力
    uint32_t                                          fieldSpinAxis;                                            //!< 軸
    float                                             fieldSpinOuter;                                           //!< 外に向かう速度
    nn::vfx::detail::ResAnim8KeyParamSet              rotateAnimParam;                                          //!< 回転のパーティクル時間アニメデータです
    nn::vfx::detail::ResAnim8KeyParamSet              outerAnimParam;                                           //!< 拡散のパーティクル時間アニメデータです

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &fieldSpinRotate );
        nn::vfx::detail::EndianUtil::Flip( &fieldSpinAxis );
        nn::vfx::detail::EndianUtil::Flip( &fieldSpinOuter );

        rotateAnimParam.FlipEndian();
        outerAnimParam.FlipEndian();
    }
};

//------------------------------------------------------------------------------
//! @brief      コリジョン フィールド情報
//------------------------------------------------------------------------------
struct ResFieldCollisionData
{
    uint8_t                                           fieldCollisionType;                                       //!< タイプ（0:反射  1:消滅)
    uint8_t                                           fieldCollisionIsWorld;                                    //!< ワールドで処理するかどうか
    uint8_t                                           fieldCollisionIsCommonCoord;                              //!< 共有平面を利用するか？
    uint8_t                                           reserved0;                                                //!< パディング領域
    float                                             fieldCollisionCoord;                                      //!< 座標
    float                                             fieldCollisionCoef;                                       //!< 反射率
    int32_t                                           fieldCollisionCnt;                                        //!< 反射回数
    float                                             fieldCollisionRegist;                                     //!< 摩擦係数

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &fieldCollisionCoord );
        nn::vfx::detail::EndianUtil::Flip( &fieldCollisionCoef );
        nn::vfx::detail::EndianUtil::Flip( &fieldCollisionCnt );
        nn::vfx::detail::EndianUtil::Flip( &fieldCollisionRegist );

    }
};

//------------------------------------------------------------------------------
//! @brief      収束 フィールド情報
//------------------------------------------------------------------------------
struct ResFieldConvergenceData
{
    uint8_t                                           fieldConvergenceType;                                     //!< 収束タイプ
    uint8_t                                           reserved0[3];                                             //!< パディング領域
    nn::util::Float3                                  fieldConvergencePos;                                      //!< 位置
    float                                             fieldConvergenceRatio;                                    //!< 割合
    nn::vfx::detail::ResAnim8KeyParamSet              animParam;                                                //!< パーティクル時間アニメのデータです

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &fieldConvergencePos );
        nn::vfx::detail::EndianUtil::Flip( &fieldConvergenceRatio );

        animParam.FlipEndian();
    }
};

//------------------------------------------------------------------------------
//! @brief      フィールド：カールノイズ
//------------------------------------------------------------------------------
struct ResFieldCurlNoiseData
{
    uint8_t                                           isFieldCurlNoiseInterpolation;                            //!< 補間するか？
    uint8_t                                           isFieldCurlNoiseBaseRandom;                               //!< ノイズのオフセットランダム
    uint8_t                                           isWorldCoordinate;                                        //!< カールノイズの影響をワールド座標で受けさせるかどうか
    uint8_t                                           reserved0;                                                //!< パディング領域
    nn::util::Float3                                  fieldCurlNoiseSpeed;                                      //!< 渦アニメ速度
    nn::util::Float3                                  fieldCurlNoiseInfluence;                                  //!< 影響の強さ
    float                                             fieldCurlNoiseScale;                                      //!< 渦テーブルの枠サイズ
    float                                             fieldCurlNoiseBase;                                       //!< ノイズのオフセット

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &fieldCurlNoiseSpeed );
        nn::vfx::detail::EndianUtil::Flip( &fieldCurlNoiseInfluence );
        nn::vfx::detail::EndianUtil::Flip( &fieldCurlNoiseScale );
        nn::vfx::detail::EndianUtil::Flip( &fieldCurlNoiseBase );

    }
};

//------------------------------------------------------------------------------
//! @brief      フィールド：位置に加算
//------------------------------------------------------------------------------
struct ResFieldPosAddData
{
    uint8_t                                           isFieldPosAddGlobal;                                      //!< ワールド座標系で加算するか？
    uint8_t                                           reserved0[3];                                             //!< パディング領域
    nn::util::Float3                                  fieldPosAdd;                                              //!< 位置加算値
    nn::vfx::detail::ResAnim8KeyParamSet              animParam;                                                //!< パーティクル時間アニメのデータです

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &fieldPosAdd );

        animParam.FlipEndian();
    }
};

//------------------------------------------------------------------------------
//! @brief      履歴式ストライプデータです。
//------------------------------------------------------------------------------
struct ResStripeHistory
{
    int32_t                                           calcType;                                                 //!< 計算方法です。
    int32_t                                           emitterFollow;                                            //!< エミッタに完全追従かどうかのフラグです。
    int32_t                                           option;                                                   //!< オプションです。
    int32_t                                           texturing;                                                //!< テクスチャリングです。
    float                                             numDivide;                                                //!< 分割数です。
    float                                             numHistory;                                               //!< 履歴数(ストライプの長さ)です。
    float                                             interval;                                                 //!< 履歴ポリゴン化の間隔です。
    float                                             headAlpha;                                                //!< 先端αです。
    float                                             tailAlpha;                                                //!< 末端αです。
    float                                             histInterpolate;                                          //!< 履歴補間パラメータです。
    float                                             dirInterpolate;                                           //!< 方向補間率です。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &calcType );
        nn::vfx::detail::EndianUtil::Flip( &emitterFollow );
        nn::vfx::detail::EndianUtil::Flip( &option );
        nn::vfx::detail::EndianUtil::Flip( &texturing );
        nn::vfx::detail::EndianUtil::Flip( &numDivide );
        nn::vfx::detail::EndianUtil::Flip( &numHistory );
        nn::vfx::detail::EndianUtil::Flip( &interval );
        nn::vfx::detail::EndianUtil::Flip( &headAlpha );
        nn::vfx::detail::EndianUtil::Flip( &tailAlpha );
        nn::vfx::detail::EndianUtil::Flip( &histInterpolate );
        nn::vfx::detail::EndianUtil::Flip( &dirInterpolate );

    }
};

//------------------------------------------------------------------------------
//! @brief      連結式ストライプです。
//------------------------------------------------------------------------------
struct ResStripeConnection
{
    int32_t                                           calcType;                                                 //!< 計算方法です。
    int32_t                                           emitterFollow;                                            //!< エミッタに完全追従するかどうかのフラグです。
    int32_t                                           option;                                                   //!< オプションです。
    int32_t                                           texturing;                                                //!< テクスチャリングです。
    int32_t                                           numDivide;                                                //!< 分割数
    int32_t                                           connectionType;                                           //!< 接続タイプです。
    float                                             headAlpha;                                                //!< 先端αです。
    float                                             tailAlpha;                                                //!< 末端αです。
    float                                             histInterpolate;                                          //!< 履歴補間パラメータです。
    float                                             dirInterpolate;                                           //!< 方向補間率です。

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &calcType );
        nn::vfx::detail::EndianUtil::Flip( &emitterFollow );
        nn::vfx::detail::EndianUtil::Flip( &option );
        nn::vfx::detail::EndianUtil::Flip( &texturing );
        nn::vfx::detail::EndianUtil::Flip( &numDivide );
        nn::vfx::detail::EndianUtil::Flip( &connectionType );
        nn::vfx::detail::EndianUtil::Flip( &headAlpha );
        nn::vfx::detail::EndianUtil::Flip( &tailAlpha );
        nn::vfx::detail::EndianUtil::Flip( &histInterpolate );
        nn::vfx::detail::EndianUtil::Flip( &dirInterpolate );

    }
};

//------------------------------------------------------------------------------
//! @brief      8キーアニメのパラメータ
//------------------------------------------------------------------------------
struct ResAnimEmitterKeyParamSet
{
    uint8_t                                           enable;                                                   //!< アニメの有効無効フラグです。
    uint8_t                                           loop;                                                     //!< アニメ再生のループフラグです。
    uint8_t                                           startRandom;                                              //!< 再生開始フレームをランダマイズするか
    uint8_t                                           reserved0;                                                //!< パディング領域
    int32_t                                           keyNum;                                                   //!< キー数
    int32_t                                           loopNum;                                                  //!< ループ数
    nn::util::Float4                                  keyValue[1];                                              //!< キー配列の先頭

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &keyNum );
        nn::vfx::detail::EndianUtil::Flip( &loopNum );
        detail::EndianUtil::FlipArray( 1, keyValue );

    }
};

//------------------------------------------------------------------------------
//! @brief      ランダム フィールド情報
//------------------------------------------------------------------------------
struct ResFieldRandomFe1
{
    nn::util::Float3                                  randomVel;                                                //!< ランダム幅
    int32_t                                           blank;                                                    //!< ランダム適用間隔(フレーム)
    nn::vfx::detail::ResAnim8KeyParamSet              animParam;                                                //!< パーティクル時間アニメのデータです

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &randomVel );
        nn::vfx::detail::EndianUtil::Flip( &blank );

        animParam.FlipEndian();
    }
};

//------------------------------------------------------------------------------
//! @brief      履歴式ストライプ2 です。
//------------------------------------------------------------------------------
struct ResStripeSuper
{
    int32_t                                           calcType;                                                 //!< 計算方法です。
    int32_t                                           emitterFollow;                                            //!< エミッタに完全追従するかどうかのフラグです。
    int32_t                                           option;                                                   //!< オプションです。（通常／クロス）
    int32_t                                           texturing0;                                               //!< UV0のテクスチャリング設定
    int32_t                                           texturing1;                                               //!< UV1のテクスチャリング設定
    int32_t                                           texturing2;                                               //!< UV2のテクスチャリング設定
    float                                             numHistory;                                               //!< 履歴の総数
    int32_t                                           connectionType;                                           //!< 接続タイプです。
    float                                             headAlpha;                                                //!< 先端αです。
    float                                             tailAlpha;                                                //!< 末端αです。
    int32_t                                           numDivide;                                                //!< 分割数です。
    float                                             histInterpolate;                                          //!< 履歴補間パラメータです。
    float                                             dirInterpolate;                                           //!< 方向補間率です。
    float                                             historyAirRegist;                                         //!< [仮]履歴点の受ける空気抵抗
    nn::util::Float3                                  historyAcceleration;                                      //!< [仮]履歴点の加速度
    float                                             historyVecRegulation;                                     //!< [仮]
    float                                             historyVecInitSpeed;                                      //!< [仮]
    nn::util::Float3                                  historyInitVecRotateCycle;                                //!< [仮]
    int32_t                                           textureUvMapType;                                         //!< テクスチャUVマッピングタイプ
    float                                             headScale;                                                //!< 先端スケール
    float                                             tailScale;                                                //!< 末端スケール
    float                                             test00;                                                   //!< [仮]実験用パラメータ
    float                                             test01;                                                   //!< [仮]実験用パラメータ
    float                                             test02;                                                   //!< [仮]実験用パラメータ
    float                                             test03;                                                   //!< [仮]実験用パラメータ
    float                                             test04;                                                   //!< [仮]実験用パラメータ
    float                                             test05;                                                   //!< [仮]実験用パラメータ
    float                                             test06;                                                   //!< [仮]実験用パラメータ
    float                                             test07;                                                   //!< [仮]実験用パラメータ

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &calcType );
        nn::vfx::detail::EndianUtil::Flip( &emitterFollow );
        nn::vfx::detail::EndianUtil::Flip( &option );
        nn::vfx::detail::EndianUtil::Flip( &texturing0 );
        nn::vfx::detail::EndianUtil::Flip( &texturing1 );
        nn::vfx::detail::EndianUtil::Flip( &texturing2 );
        nn::vfx::detail::EndianUtil::Flip( &numHistory );
        nn::vfx::detail::EndianUtil::Flip( &connectionType );
        nn::vfx::detail::EndianUtil::Flip( &headAlpha );
        nn::vfx::detail::EndianUtil::Flip( &tailAlpha );
        nn::vfx::detail::EndianUtil::Flip( &numDivide );
        nn::vfx::detail::EndianUtil::Flip( &histInterpolate );
        nn::vfx::detail::EndianUtil::Flip( &dirInterpolate );
        nn::vfx::detail::EndianUtil::Flip( &historyAirRegist );
        nn::vfx::detail::EndianUtil::Flip( &historyAcceleration );
        nn::vfx::detail::EndianUtil::Flip( &historyVecRegulation );
        nn::vfx::detail::EndianUtil::Flip( &historyVecInitSpeed );
        nn::vfx::detail::EndianUtil::Flip( &historyInitVecRotateCycle );
        nn::vfx::detail::EndianUtil::Flip( &textureUvMapType );
        nn::vfx::detail::EndianUtil::Flip( &headScale );
        nn::vfx::detail::EndianUtil::Flip( &tailScale );
        nn::vfx::detail::EndianUtil::Flip( &test00 );
        nn::vfx::detail::EndianUtil::Flip( &test01 );
        nn::vfx::detail::EndianUtil::Flip( &test02 );
        nn::vfx::detail::EndianUtil::Flip( &test03 );
        nn::vfx::detail::EndianUtil::Flip( &test04 );
        nn::vfx::detail::EndianUtil::Flip( &test05 );
        nn::vfx::detail::EndianUtil::Flip( &test06 );
        nn::vfx::detail::EndianUtil::Flip( &test07 );

    }
};

//------------------------------------------------------------------------------
//! @brief      テスト
//------------------------------------------------------------------------------
struct ResEPTest
{
    uint32_t                                          param;                                                    //!< テスト

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &param );

    }
};

//------------------------------------------------------------------------------
//! @brief      エリア内ループ
//------------------------------------------------------------------------------
struct ResEPAreaLoop
{
    nn::util::Float3                                  repeatOffsetPos;                                          //!< リピート時の位置オフセット
    float                                             repeatNum;                                                //!< リピート回数
    nn::util::Float3                                  areaSize;                                                 //!< 範囲（箱）の大きさ
    float                                             clippingWorldHeight;                                      //!< ワールド空間のクリッピング平面の高さ
    nn::util::Float3                                  areaPos;                                                  //!< 範囲（箱）の位置
    int32_t                                           clippingType;                                             //!< クリッピングタイプ
    nn::util::Float3                                  alphaRatio;                                               //!< 端でアルファを薄くする割合
    float                                             isCameraLoop;                                             //!< 範囲（箱）をカメラ前に固定するかどうか
    nn::util::Float3                                  areaRotate;                                               //!< 範囲（箱）の回転量（Radian）
    float                                             reserved0;                                                //!< パディング領域

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &repeatOffsetPos );
        nn::vfx::detail::EndianUtil::Flip( &repeatNum );
        nn::vfx::detail::EndianUtil::Flip( &areaSize );
        nn::vfx::detail::EndianUtil::Flip( &clippingWorldHeight );
        nn::vfx::detail::EndianUtil::Flip( &areaPos );
        nn::vfx::detail::EndianUtil::Flip( &clippingType );
        nn::vfx::detail::EndianUtil::Flip( &alphaRatio );
        nn::vfx::detail::EndianUtil::Flip( &isCameraLoop );
        nn::vfx::detail::EndianUtil::Flip( &areaRotate );
        nn::vfx::detail::EndianUtil::Flip( &reserved0 );

    }
};

//------------------------------------------------------------------------------
//! @brief      カスタムフィールドデータ
//------------------------------------------------------------------------------
struct ResFieldCustom
{
    uint32_t                                          flags;                                                    //!< フラグ
    float                                             value0;                                                   //!< 数値0
    float                                             value1;                                                   //!< 数値1
    float                                             value2;                                                   //!< 数値2
    float                                             value3;                                                   //!< 数値3
    float                                             value4;                                                   //!< 数値4
    float                                             value5;                                                   //!< 数値5
    float                                             value6;                                                   //!< 数値6
    float                                             value7;                                                   //!< 数値7
    float                                             value8;                                                   //!< 数値8
    float                                             value9;                                                   //!< 数値9
    float                                             value10;                                                  //!< 数値10
    float                                             value11;                                                  //!< 数値11
    float                                             value12;                                                  //!< 数値12
    float                                             value13;                                                  //!< 数値13
    float                                             value14;                                                  //!< 数値14
    float                                             value15;                                                  //!< 数値15

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &flags );
        nn::vfx::detail::EndianUtil::Flip( &value0 );
        nn::vfx::detail::EndianUtil::Flip( &value1 );
        nn::vfx::detail::EndianUtil::Flip( &value2 );
        nn::vfx::detail::EndianUtil::Flip( &value3 );
        nn::vfx::detail::EndianUtil::Flip( &value4 );
        nn::vfx::detail::EndianUtil::Flip( &value5 );
        nn::vfx::detail::EndianUtil::Flip( &value6 );
        nn::vfx::detail::EndianUtil::Flip( &value7 );
        nn::vfx::detail::EndianUtil::Flip( &value8 );
        nn::vfx::detail::EndianUtil::Flip( &value9 );
        nn::vfx::detail::EndianUtil::Flip( &value10 );
        nn::vfx::detail::EndianUtil::Flip( &value11 );
        nn::vfx::detail::EndianUtil::Flip( &value12 );
        nn::vfx::detail::EndianUtil::Flip( &value13 );
        nn::vfx::detail::EndianUtil::Flip( &value14 );
        nn::vfx::detail::EndianUtil::Flip( &value15 );

    }
};

//------------------------------------------------------------------------------
//! @brief      名前テーブルのレコード
//------------------------------------------------------------------------------
struct ResNameTableRecord
{
    uint64_t                                          guid;                                                     //!< ハッシュコードと衝突カウンタからなるGUID
    uint32_t                                          nextOffset;                                               //!< 次のレコードへのオフセット
    uint32_t                                          nameLength;                                               //!< 名前の長さ
    char                                              name[8];                                                  //!< 名前

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &guid );
        nn::vfx::detail::EndianUtil::Flip( &nextOffset );
        nn::vfx::detail::EndianUtil::Flip( &nameLength );

    }
};

//------------------------------------------------------------------------------
//! @brief      g3dプリミティブテーブルのレコード
//------------------------------------------------------------------------------
struct ResG3dPrimitiveTableRecord
{
    uint64_t                                          guid;                                                     //!< ハッシュコードと衝突カウンタからなるGUID
    uint32_t                                          nextOffset;                                               //!< 次のレコードへのオフセット
    uint32_t                                          nameLength;                                               //!< 名前の長さ
    uint8_t                                           positionIndex;                                            //!< posデータのインデックス
    uint8_t                                           normalIndex;                                              //!< 法線データのインデックス
    uint8_t                                           tangentIndex;                                             //!< 接線データのインデックス
    uint8_t                                           colorIndex;                                               //!< 頂点カラーのインデックス
    uint8_t                                           uv0Index;                                                 //!< UV0のインデックス
    uint8_t                                           uv1Index;                                                 //!< UV1のインデックス
    char                                              reserved0[2];                                             //!< アライメント

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &guid );
        nn::vfx::detail::EndianUtil::Flip( &nextOffset );
        nn::vfx::detail::EndianUtil::Flip( &nameLength );

    }
};

//------------------------------------------------------------------------------
//! @brief      エミッタセットのファイルパステーブルのレコード
//------------------------------------------------------------------------------
struct ResEmitterSetFilePathTableRecord
{
    uint8_t                                           guid[16];                                                 //!< エミッタセットのguid
    uint32_t                                          nextOffset;                                               //!< 次のレコードへのオフセット
    uint32_t                                          filePathLength;                                           //!< ファイルパスの長さ
    char                                              filePath[8];                                              //!< ファイルパス

    //--------------------------------------------------------------------------
    //! @brief      エンディアンをフリップします.
    //--------------------------------------------------------------------------
    void FlipEndian() NN_NOEXCEPT
    {
        nn::vfx::detail::EndianUtil::Flip( &nextOffset );
        nn::vfx::detail::EndianUtil::Flip( &filePathLength );

    }
};


} // namespace detail
} // namespace vfx
} // namespace nn

