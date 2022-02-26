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

#include <nn/audio/audio_PerformanceMetricsTypes.h>
#include <nn/audio/audio_PerformanceMetricsApi.h>
#include <nn/os/os_Tick.h>

namespace nn {
namespace audio {

/**
 * @brief       nn::audio::SetPerformanceFrameBuffer で取得したパフォーマンス情報を扱うためのクラスです。
 */
class PerformanceInfo
{
    NN_DISALLOW_COPY( PerformanceInfo );
    NN_DISALLOW_MOVE( PerformanceInfo );

public:

    /**
     * @brief          コンストラクタです。
     */
    PerformanceInfo() NN_NOEXCEPT;

    /**
     * @brief          デストラクタです。
     */
    ~PerformanceInfo() NN_NOEXCEPT;

    /**
     * @brief          パフォーマンス情報へのバッファを設定し、初期化します。
     * @param[in]      frameBuffer             パフォーマンス情報を保持するバッファ
     * @param[in]      bufferSize              パフォーマンス情報を保持するバッファのサイズ
     * @return         指定したバッファにパフォーマンス情報が含まれていなければ false を返します。
     * @pre
     * - frameBuffer != nullptr
     * @details
     * nn::audio::SetPerformanceFrameBuffer() では「次にパフォーマンス情報を書き出すバッファ」を指定し、
     * 返り値として「その時点までのパフォーマンス情報が格納されたバッファ」を返します。
     * この返されたバッファに含まれるパフォーマンス情報を参照してインスタンスを初期化します。
     * nn::audio::SetPerformanceFrameBuffer() によって、オーディオレンダラが書き出すバッファを変更したのちに、
     * 返り値のアドレスを本 API の引数に与えてください。
     */
    bool SetBuffer(const void* frameBuffer, size_t bufferSize) NN_NOEXCEPT;

    /**
     * @brief          パフォーマンス情報を次の PerformanceFrame へと進めます。
     * @return         次の PerfromanceFrame が存在しない場合には false を返します。
     */
    bool MoveToNextFrame() NN_NOEXCEPT;

    /**
     * @brief          現在の PerformanceFrame で掛かった全体の処理時間を返します。
     * @return         全体の処理時間 [MicroSeconds]
     */
    int32_t GetTotalProcessingTime() const NN_NOEXCEPT;

    /**
     * @brief          レンダリング時間の上限を超えたかどうかを返します。
     * @return         レンダリング時間の上限を超えたかどうか
     */
    bool IsRenderingTimeLimitExceeded() const NN_NOEXCEPT;

    /**
     * @brief          ボイスドロップが発生したボイスの個数を返します。
     * @return         ボイスドロップが発生したボイスの個数
     */
    int32_t GetVoiceDropCount() const NN_NOEXCEPT;

    /**
     * @brief          オーディオフレームの処理開始時間を返します。
     * @return         オーディオフレームの処理開始時間
     */
    nn::os::Tick GetStartTime() const NN_NOEXCEPT;

    /**
     * @brief          現在の PerformanceFrame のインデックスを取得します。
     * @return         インデックスを返します。
     * @details
     * PerformanceFrame は各フレーム毎にインデックスを持ち、
     * この値はオーディオフレーム毎にインクリメントされる連続な値となっています。
     * MoveToNextFrame() の前後で、このインデックスが連続であることを確認することで、
     * PerformanceFrame の欠落の有無を確認することができます。
     * インデックスが uint32_t で表現できる最大値を超えた場合 0 に戻ります。
     */
    uint32_t GetFrameIndex() const NN_NOEXCEPT;

    /**
     * @brief          現在の PerformanceFrame に含まれる PerformanceEntry 列を取得します。
     * @param[out]     pOutCount        返り値の指す先に含まれる PerformanceEntry 数を返します。
     * @return         PerformanceEntry 列へのポインタを返します。
     * @details
     * 返り値は SetBuffer() によって渡されたバッファに含まれる PerformanceEntry 列を指しています。
     * PerformanceEntry が含まれない場合には nullptr を返します。
     */
    const PerformanceEntry* GetEntries(int* pOutCount) const NN_NOEXCEPT;

    /**
     * @brief          現在の PerformanceFrame に含まれる PerformanceDetail 列を取得します。
     * @param[out]     pOutCount        返り値の指す先に含まれる PerformanceDetail 数を返します。
     * @return         PerformanceDetail 列へのポインタを返します。
     * @details
     * 返り値は SetBuffer() によって渡されたバッファに含まれる PerformanceDetail 列を指しています。
     * PerformanceDetail が含まれない場合には nullptr を返します。
     */
    const PerformanceDetail* GetDetails(int* pOutCount) const NN_NOEXCEPT;

private:
    const void* m_Buffer;
    size_t m_BufferSize;
    const PerformanceFrameHeader* m_Header;
    const nn::audio::PerformanceEntry* m_Entries;
    const nn::audio::PerformanceDetail* m_Details;
};

} // namespace audio
} // namespace nn
