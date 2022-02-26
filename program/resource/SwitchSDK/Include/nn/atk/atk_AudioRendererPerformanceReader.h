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

#include <atomic>
#include <nn/nn_Macro.h>
#include <nn/os/os_Tick.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @briefprivate
//! @brief  オーディオレンダラのパフォーマンス情報を取得するためのクラスです。
//!
//---------------------------------------------------------------------------
class AudioRendererPerformanceReader
{
public:
    //---------------------------------------------------------------------------
    //! @brief  パフォーマンス情報の構造体です。
    //!
    //---------------------------------------------------------------------------
    struct PerformanceInfo
    {
        //---------------------------------------------------------------------------
        //! @brief    オーディオレンダラのパフォーマンス情報バッファです。
        //---------------------------------------------------------------------------
        void* performanceBuffer;
        //---------------------------------------------------------------------------
        //! @brief    オーディオレンダラのパフォーマンス情報バッファのサイズです。
        //---------------------------------------------------------------------------
        size_t performanceBufferSize;
        //---------------------------------------------------------------------------
        //! @brief    パフォーマンス情報を記録した Tick です。
        //---------------------------------------------------------------------------
        nn::os::Tick tick;
    };

public:
    //---------------------------------------------------------------------------
    //! @brief  パフォーマンス情報の最小個数です
    //!
    //---------------------------------------------------------------------------
    static const int PerformanceInfoCountMin = 2;

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //!
    //---------------------------------------------------------------------------
    AudioRendererPerformanceReader() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      初期化に必要なメモリのサイズを取得します。
    //!
    //!             @ref Initialize で必要なバッファの最小サイズを取得します。
    //!             この関数で取得した値以上のサイズのバッファを @ref Initialize に渡してください。
    //!
    //!             1 オーディオフレーム (= 5ms) ごとにパフォーマンス情報を 1 つ取得するため、
    //!             T ms 周期でパフォーマンス情報を読み取るとき、performanceInfoCount の最小値は次の式で求めることができます。
    //! @code
    //!  (performanceInfoCount の最小値) = max( PerformanceInfoCountMin, (T / 5) + 1 );
    //! @endcode
    //!             フレーム落ちなどを考慮する場合はこれよりも大きな値を設定してください。
    //!
    //! @param[in]  performanceInfoCount  パフォーマンス情報を書き出すバッファの個数です。
    //!
    //! @return     初期化に必要なバッファサイズを返します。
    //! @pre
    //! - performanceInfoCount >= @ref PerformanceInfoCountMin
    //! - @ref SoundSystem を初期化済み
    //!
    //! @see Initialize
    //! @see PerformanceInfoCountMin
    //! @see SoundSystem クラス
    //!
    //---------------------------------------------------------------------------
    static size_t GetRequiredMemorySize(int performanceInfoCount) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      オーディオレンダラのパフォーマンス情報レコーダを初期化します。
    //!
    //! @param[in]  performanceInfoCount  パフォーマンス情報を書き出すバッファの個数です。
    //! @param[in]  buffer  バッファへのポインタです。
    //! @param[in]  bufferSize  バッファサイズです。
    //! @pre
    //! - performanceInfoCount @ref GetRequiredMemorySize と同じ値を指定している
    //!
    //! @see GetRequiredMemorySize
    //!
    //---------------------------------------------------------------------------
    void Initialize(int performanceInfoCount, void* buffer, size_t bufferSize) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      オーディオレンダラのパフォーマンス情報を取得します。
    //!
    //! @return     オーディオレンダラのパフォーマンス情報を返します。
    //!             取得できるパフォーマンス情報がないときは nullptr を返します。
    //!
    //---------------------------------------------------------------------------
    const PerformanceInfo* ReadPerformanceInfo() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      パフォーマンス情報レコーダが初期化されているかどうかを取得します。
    //!
    //! @return     パフォーマンス情報レコーダが初期化されていれば true を、
    //!             そうでなければ false を返します。
    //!
    //! @see  Initialize
    //!
    //---------------------------------------------------------------------------
    bool IsInitialized() const NN_NOEXCEPT
    {
        return m_IsInitialized;
    }

    //! @internal
    //! @param performanceFrameBuffer :private
    //! @param performanceFrameBufferSize :private
    //! @param tick :private
    void Record(const void* performanceFrameBuffer, size_t performanceFrameBufferSize, nn::os::Tick tick) NN_NOEXCEPT;

private:
    PerformanceInfo* m_pPerformanceInfo;
    int    m_PerformanceInfoCount;

    std::atomic<int> m_WriteIndex;
    std::atomic<int> m_ReadIndex;
    bool             m_IsInitialized;
};

} // namespace nn::atk
} // namespace nn
