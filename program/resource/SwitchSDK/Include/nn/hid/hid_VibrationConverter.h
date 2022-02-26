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
 * @brief   VibrationConverter クラスの定義
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/nn_TimeSpan.h>
#include <nn/util/util_TypedStorage.h>
#include <nn/hid/hid_VibrationValueTypes.h>

namespace nn { namespace hid {

/**
 * @brief   音声データから振動値を抽出するクラスです。
 *
 * @details
 * 音声データをリアルタイムに振動値に変換する機能です。
 * PC ツールである@confluencelink{104490352,振動コンバータ}を実機上に移植したものです。
 *
 * 内部的な動作は以下になります。@n
 * 入力された音声データに対してバンドパスフィルタをかけます。@n
 * フィルタされた波形に対して、@ref nn::hid::VibrationConverter::ChunkDuration 区間毎に振幅と周波数を抽出します。@n
 * 上記の処理を振動値の低帯域、高帯域に対してそれぞれ実行し、振動値を決定します。@n
 * 対応している入力のサンプリングレートは 32000(Hz) もしくは 48000(Hz)です。
 */
class VibrationConverter
{
    NN_DISALLOW_COPY(VibrationConverter);
    NN_DISALLOW_MOVE(VibrationConverter);

public:
    /**
    * @brief       振動値の抽出に使用する音声データの長さです。
    */
    static const ::nn::TimeSpan ChunkDuration;

    /**
    * @brief       入力の音声データのデフォルトのサンプルレート(Hz)です。
    */
    static const int DefaultSampleRate;

public:
    /**
    * @brief       コンストラクタです。@n
    *
    * @details
    *  入力のサンプルレートは @ref nn::hid::VibrationConverter::DefaultSampleRate に設定されます。@n
    *  抽出する周波数は低帯域が[128.0(Hz), 200.0(Hz)], 高帯域が[256.0(Hz), 400.0(Hz)]です。
    */
    VibrationConverter() NN_NOEXCEPT;

    /**
    * @brief       コンストラクタです。@n
    *              入力のサンプルレートを設定できます。
    *
    * @param[in]   sampleRate           サンプルレート設定
    *
    * @pre
    *              - sampleRate は 32000(Hz) もしくは 48000(Hz)
    *
    * @details
    *  抽出する周波数は低帯域が[128.0(Hz), 200.0(Hz)], 高帯域が[256.0(Hz), 400.0(Hz)]です。
    */
    explicit VibrationConverter(int sampleRate) NN_NOEXCEPT;

    /**
    * @brief       コンストラクタです。@n
    *              入力のサンプルレート、抽出する周波数の上限と下限を設定できます。
    *
    * @param[in]   sampleRate           サンプルレート設定
    * @param[in]   lowerCutoffLow       抽出する周波数の下限（低帯域用）
    * @param[in]   higherCutoffLow      抽出する周波数の上限（低帯域用）
    * @param[in]   lowerCutoffHigh      抽出する周波数の下限（高帯域用）
    * @param[in]   higherCutoffHigh     抽出する周波数の上限（高帯域用）
    *
    * @pre
    *              - sampleRate は 32000(Hz) もしくは 48000(Hz)
    *              - lowerCutoffLow は 10.0(Hz) 以上 2500.0(Hz) 以下
    *              - higherCutoffLow は 10.0(Hz) 以上 2500.0(Hz) 以下
    *              - lowerCutoffHigh は 10.0(Hz) 以上 2500.0(Hz) 以下
    *              - higherCutoffHigh は 10.0(Hz) 以上 2500.0(Hz) 以下
    *              - lowerCutoffLow < higherCutoffLow
    *              - lowerCutoffHigh < higherCutoffHigh
    *
    * @details
    *  低帯域および高帯域に対して、抽出する周波数の上限と下限を設定することができます。設定できる範囲は下限が 10Hz、上限が 2500Hz です。@n
    *  デフォルトの周波数は低帯域が[128.0(Hz), 200.0(Hz)], 高帯域が[256.0(Hz), 400.0(Hz)]です。
    */
    VibrationConverter(int sampleRate, float lowerCutoffLow, float higherCutoffLow, float lowerCutoffHigh, float higherCutoffHigh) NN_NOEXCEPT;

    /**
    * @brief       デストラクタです。
    */
    virtual ~VibrationConverter() NN_NOEXCEPT;

    /**
    * @brief       音声データを振動値にコンバートします。
    *
    * @param[out]  pOutValue    変換された振動値の格納先
    * @param[in]   pValues      音声データが展開されているメモリの先頭アドレス
    * @param[in]   count        音声データの個数
    *
    * @pre
    *              - pOutValue != nullptr
    *              - pValues != nullptr
    *              - pValues を先頭とする連続領域に有効な音声データが書き込まれている
    *              - pValues のフォーマットは mono channel 16bit PCM
    *              - count は @ref nn::hid::VibrationConverter::ChunkDuration 時間分のサンプル数
    *
    * @details
    *  @ref nn::hid::VibrationConverter::ChunkDuration 時間分の音声データ（mono channel 16bit PCM）を振動値に変換します。@n
    *  入力する音声データの数は設定したサンプルレートによって変わります。@n
    *  サンプルレートはコンストラクタで設定しない場合 @ref nn::hid::VibrationConverter::DefaultSampleRate になります。
    */
    void ProcessChunk(nn::hid::VibrationValue* pOutValue, const int16_t* pValues, int count) NN_NOEXCEPT;

private:
    static const size_t VibrationConverterContextSize = 4096;

private:
    struct VibrationConverterContextImpl;
    typedef nn::util::TypedStorage<VibrationConverterContextImpl, VibrationConverterContextSize, NN_ALIGNOF(uint64_t)> VibrationConverterContext;

private:
    int m_SampleRate;
    VibrationConverterContext m_Buffer;
};

}} // namespace nn::hid
