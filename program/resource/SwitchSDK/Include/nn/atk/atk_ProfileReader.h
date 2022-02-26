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

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <nn/nn_TimeSpan.h>
#include <nn/os/os_Tick.h>
#include <nn/audio/audio_Common.h>
#include <nn/atk/atk_Global.h>
#include <nn/util/util_IntrusiveList.h>
#include <nn/atk/atk_HardwareManager.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @brief  開始時刻と終了時刻を記録する構造体です。
//!
//! @see SoundProfile 構造体
//!
//---------------------------------------------------------------------------
struct TimeRange
{
    //! 開始時刻です。
    nn::os::Tick begin;

    //! 終了時刻です。
    nn::os::Tick end;

    //! @brief  開始時刻～終了時刻までの時間を取得します。
    //! @return 開始時刻～終了時刻までの時間を返します。
    nn::TimeSpan GetSpan() const NN_NOEXCEPT
    {
        nn::os::Tick span = end - begin;
        return span.ToTimeSpan();
    }
};

//---------------------------------------------------------------------------
//! @brief  サウンドプロファイル情報を表す構造体です。
//!         特定のオーディオフレームでのプロファイル情報を表します。
//!
//!         内部で @ref nn::audio::SetPerformanceFrameBuffer を呼び出し、
//!         取得できた @ref nn::audio::PerformanceInfo から算出した値を格納しています。
//!
//!         nwFrameProcess については atk ライブラリ内部で計測したサウンドスレッドの処理時間を格納しています。
//!
//!         ■ nwFrameProcess
//!
//!         - プレイヤーやサウンド、トラックなどのパラメータ更新処理にかかった時間です。
//!         - サウンドの数やサウンドに含まれるトラックの数、チャンネルの数、
//!         ノートオンの数に応じて、処理時間が長くなります。
//!
//!         ■ voiceProcess
//!
//!         - 内部で使用する nn::audio のオーディオレンダラでのボイスレンダリング処理にかかった時間です。
//!         - 再生中のボイスの数に応じて、処理時間が長くなります。
//!
//!         ■ mainMixProcess
//!
//!         - 内部で使用する nn::audio のオーディオレンダラでのエフェクト処理を行うミックスの処理にかかった時間です。
//!         - @ref SoundSystem::AppendEffect しているエフェクトの数や種類に応じて、
//!         処理時間が長くなります。
//!
//!         ■ rendererFrameProcess
//!
//!         - 内部で使用する nn::audio のオーディオレンダラが行う処理全体にかかった時間です。
//!         - voiceProcess, mainMixProcess, finalMixProcess, sinkProcess, circularBufferSinkProcess の時間の合計と
//!         オーディオレンダラのシステムの処理にかかった時間の合計の時間になります。
//!
#if 0 // 参考用 : NW4F 実装
//!         現状、内部実装の都合で dspCycles で正しい値が取得できません。
//!         ご了承ください。
//!
//!         ■ nwFrameProcess
//!
//!         - プレイヤーやサウンド、トラックなどのパラメータ更新処理が含まれます。
//!         - サウンドの数やサウンドに含まれるトラックの数、チャンネルの数、
//!         ノートオンの数に応じて、処理時間が長くなります。
//!
//!         ■ nwVoiceParamUpdate
//!
//!         - nwFrameProcess で計算されたボイスパラメータの反映処理が含まれます。
//!         - ボイスの数に応じて、処理時間が長くなります。
//!         - AXPROFILE の userCallbackStart/End に相当します。
//!
//!         ■ voiceRendering
//!
//!         - PPC ボイスレンダリング処理が含まれます。
//!         - レンダリングを PPC に指定したボイスの数に応じて、処理時間が長くなります。
//!         - AXPROFILE の axPPCStart/End に相当します。
//!
//!         ■ auxProcess
//!
//!         - AUX コールバック処理 (エフェクト処理) が含まれます。
//!         - @ref SoundSystem::AppendEffect しているエフェクトの数や種類に応じて、
//!         処理時間が長くなります。
//!         - AXPROFILE の auxProcessingStart/End に相当します。
//!
//!         ■ axFrameProcess
//!
//!         - nwVoiceParamUpdate や voiceRendering、auxProcess の他、
//!         AX 割り込み内でのすべての処理が含まれます。
//!         - AXPROFILE の axFrameStart/End に相当します。
//!
//!         各パラメータの対応関係は下記のようなイメージです。
//!
//! @code
//! AX Intr                nn::atk SoundThread
//!   .                            .
//!   .                            .
//!   +-- axFrameProcess           .
//!   |                            .
//!   |+-- voiceRendering          .
//!   ||                           .
//!   ||                           .
//!   |+--                         .
//!   |                            .
//!   |+-- auxProcess              .
//!   ||                           .
//!   ||                           .
//!   |+--                         .
//!   |                            .
//!   |+-- nwVoiceParamUpdate      .
//!   ||                           .
//!   ||                           .
//!   |+--                         .
//!   |                            .
//!   +--                          .
//!   .                            +-- nwFrameProcess
//!   .                            |
//!   .                            |
//!   .                            +--
//!   .                            .
//!   .                            .
//! @endcode
#endif
//!
//! @see ProfileReader::Read
//!
//---------------------------------------------------------------------------
struct SoundProfile
{
    //! SoundSystem を初期化してから経過したオーディオフレーム数です。 @ref nn::audio::PerformanceInfo::GetFrameIndex で取得できる値に対応します。
    uint32_t frameIndex;

    //! サウンドスレッドで 1 フレームの処理をするのにかかった時間です。
    TimeRange nwFrameProcess;

    //! 内部で使用する nn::audio のオーディオレンダラでのメインミックス処理にかかった時間です。
    //! エフェクトの処理時間はこちらに含まれます。
    TimeRange mainMixProcess;

    //! 内部で使用する nn::audio のオーディオレンダラでのファイナルミックス処理にかかった時間です。
    TimeRange finalMixProcess;

    //! 内部で使用する nn::audio のオーディオレンダラでのボイス処理にかかった時間です。
    TimeRange voiceProcess;

    //! 内部で使用する nn::audio のオーディオレンダラでの Sink の処理にかかった時間です。
    //! Sink の詳しい説明は @confluencelink{89991492, オーディオレンダラの説明の「Sink」} を参照してください。
    TimeRange sinkProcess;

    //! @ref nn::atk::SoundSystem の初期化で CircularBufferSink を利用可能にしたとき、
    //! 内部で使用する nn::audio のオーディオレンダラでの CircularBufferSink の処理にかかった時間です。
    //! CircularBufferSink を利用可能にしていない場合は、この値を参照しないでください。
    TimeRange circularBufferSinkProcess;

    //! 内部で使用する nn::audio のオーディオレンダラの処理にかかった総時間です。
    //! @ref nn::audio::PerformanceInfo::GetTotalProcessingTime で取得できる値に対応します。
    //! オーディオレンダラのシステムの処理にかかった時間も含まれるため、
    //! voiceProcess, mainMixProcess, finalMixProcess, sinkProcess, circularBufferSinkProcess の時間の合計と
    //! 必ずしも一致しないことにご注意ください。
    //! 特に PC 環境では同一 PC の他のプロセスにより処理がブロックされることがあるため、誤差が大きくなる場合があります。
    TimeRange rendererFrameProcess;


    //! 使用した全ボイスの数です
    //! rendererVoiceCount, nwVoiceCount を合計した値になります。
    uint32_t totalVoiceCount;

    //! 使用した、オーディオレンダラで描画されたボイスの数です
    uint32_t rendererVoiceCount;

    //! @internal
    //! 使用したNWボイスの数です
    uint32_t nwVoiceCount;

    //! ボイスドロップを起こしたオーディオレンダラのボイス数です。 @ref nn::audio::PerformanceInfo::GetVoiceDropCount で取得できる値に対応します。
    int32_t droppedVoiceCount;

    //! オーディオレンダラの処理時間がレンダリング時間の上限を超えたかどうかを表します
    //! @ref nn::audio::PerformanceInfo::IsRenderingTimeLimitExceeded で取得できる値に対応します。
    //! オーディオレンダラのレンダリング時間についての詳細は @ref nn::audio::SetAudioRendererRenderingTimeLimit を参照ください。
    bool isRendereringTimeLimitExceeded;

    //! @internal
    uint64_t nwFrameProcessTick;

    //! @internal
    //! @brief  サウンドプロファイル情報が保持するボイス処理時間の最大数です。内部処理用の変数です。
    static const int DetailVoiceTableCount = detail::driver::HardwareManager::AtkVoiceCountMax;

    //! @briefprivate
    //! SoundSystemParam で追加のエフェクトバスを使用可能にしたとき、
    //! 内部で使用する nn::audio のオーディオレンダラでのサブミックス処理にかかった時間です。
    //! 追加のエフェクトバスのエフェクトの処理時間はこちらに含まれます。
    //! 追加のエフェクトバスを使用可能にしていない場合は、この値を参照しないでください。
    //! また、追加のエフェクトバスを使用可能にした場合は
    //! rendererFrameProcess に _additionalSubMixProcess の時間も加えられる点にご注意ください。
    TimeRange _additionalSubMixProcess;

    //! @internal
    //! 各ボイス処理にかかった nn::audio のオーディオレンダラの処理時間です。内部処理用の変数です。
    TimeRange _voiceProcessTable[DetailVoiceTableCount];

    //! @internal
    //! 各ボイス処理時間がどの NodeId に対応するかを表します。内部処理用の変数です。
    nn::audio::NodeId _voiceIdTable[DetailVoiceTableCount];

    //! @briefprivate
    //! @brief サウンドスレッドのフレーム同士の間隔を取得します。
    //!        通常 5 ミリ秒に近い値になります。
    //! @param prevNwFrameProcessBegin 前回のサウンドスレッドの処理の開始時間です。
    //!                                初回の呼び出し時や、前回呼び出しから間隔をあけて呼び出した時は
    //!                                不正な結果が返ってきますので、その結果は破棄してください。
    //! @return サウンドスレッドのフレーム同士の間隔を返します。
    nn::TimeSpan GetNwFrameProcessInterval(const nn::os::Tick& prevNwFrameProcessBegin) const NN_NOEXCEPT
    {
        nn::os::Tick interval = nwFrameProcess.begin - prevNwFrameProcessBegin;
        return interval.ToTimeSpan();
    }

    // TODO: プロファイルパラメータの整理
#if 0 // 参考用 : NW4F 実装
    //! nwFrameProcess で計算されたボイスパラメータの反映処理にかかった時間です。
    TimeRange nwVoiceParamUpdate;

    //! PPC ボイスレンダリング処理にかかった時間です。
    TimeRange voiceRendering;

    //! AUX コールバック処理 (エフェクト処理) にかかった時間です。
    TimeRange auxProcess;

    //! @briefprivate
    //! ボイスパラメータ同期処理にかかった時間です。
    TimeRange syncVoiceParam;

    //! @briefprivate
    //! 出力フォーマッティング処理にかかった時間です。
    TimeRange outputFormatProcess;

    //! AX 割り込みにかかった時間です。
    TimeRange axFrameProcess;

    //! @briefprivate
    //! 使用したPPCボイスの数です
    uint32_t ppcVoiceCount;

    //! 使用したAXボイスの数です (AXPROFILE の axNumVoices に該当します)。
    uint32_t axVoiceCount;

    //! DSP の処理サイクル数です (AXGetDspCycles で取得した値が格納されます)。
    uint32_t dspCycles;

    //! @briefprivate
    uint64_t axFrameProcessTick;

    //! @briefprivate
    //! @brief サウンドスレッドのフレーム同士の間隔を取得します。
    //! @brief AXPROFILE の axFrameStart の差分 (今回の axFrameStart - 前回の axFrameStart) に該当します。
    //!        通常 3 ミリ秒に近い値になります。
    //! @param prevAxFrameProcessBegin 前回のAXPROFILE の axFrameStartです。
    //! @return AXPROFILE の axFrameStart の差分を返します。
    nn::TimeSpan GetAxFrameProcessInterval(const nn::os::Tick& prevAxFrameProcessBegin) const
    {
        nn::os::Tick interval = axFrameProcess.begin - prevAxFrameProcessBegin;
        return interval.ToTimeSpan();
    }
#endif
};

//---------------------------------------------------------------------------
//! @brief  プロファイルデータを取得するためのクラスです。
//!
#if 0 // 参考用 : NW4F 実装
//!         本クラスのオブジェクトを生成し、@ref SoundSystem::RegisterProfileReader 関数で登録
//!         すると、本クラスのオブジェクトのバッファにプロファイルデータが記録されるようになります。
//!         この記録に使用するバッファは各オブジェクトごとに確保されます。
//!
//!         @ref ProfileReader::Read 関数を使用することで、
//!         本クラスのオブジェクトのバッファの内容を読みとることができます。
//!
//!         ProfileReader の登録数が増えるほど処理負荷が増える点に注意してください。
//!
//!         詳しい使用方法は、Demo/snd/profile デモをご覧ください。
//!
//!         この API は Cafe プラットフォームでのみ利用可能です。
//!
//! @see SoundProfile 構造体
//! @see SoundSystem::RegisterProfileReader
#else
//! @see SoundProfile 構造体
#endif
//!
//---------------------------------------------------------------------------
class ProfileReader
{
public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //!
    //---------------------------------------------------------------------------
    ProfileReader() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  プロファイルデータを取得します。
    //!
    //!         プロファイルデータは、1 オーディオフレーム (= 5ms) に
    //!         1 SoundProfile 消費します。
    //!
    //!         1/60 秒周期で本関数を呼び出す場合は、最低 4 個分
    //!         (1/60秒 = 16.67ms = オーディオフレーム換算で 3～4 フレームに相当)、
    //!         フレーム落ちなどを考慮しても 3 倍の 12 個分程度の SoundProfile
    //!         構造体を profile に渡せば十分です。
    //!
    //!         つまり、T ms 周期で本関数を呼び出す場合の SoundProfile
    //!         の最低必要数は下記の式で求められます。
    //!
    //! @code
    //!  (最低必要数) = (T / 5) + 1;
    //! @endcode
    //!
    //!         また、サウンドライブラリ内部では、32 個の SoundProfile を持っています。
    //!
    //! @param[in] profile    読み取ったプロファイルデータの格納先を指定します。
    //! @param[in] maxCount   オーディオフレーム単位で最大で何フレーム遡ったプロファイルデータを取得するのかを指定します。32 まで指定できます。
    //!
    //! @return 取得したプロファイルデータの個数を返します。
    //!
    //! @see SoundProfile 構造体
    //!
    //---------------------------------------------------------------------------
    int Read( SoundProfile* profile, int maxCount ) NN_NOEXCEPT;

    //! @internal
    //! @param src :private
    void Record( const SoundProfile& src ) NN_NOEXCEPT;

public:
    //! @internal
    util::IntrusiveListNode m_Link;

private:
    SoundProfile m_ProfileBuffer[detail::MaxPerformanceInfoCount];
    int m_ProfileBufferRead;
    int m_ProfileBufferWrite;
};

//! @briefprivate
//! @brief  サウンドスレッドの更新プロファイル情報を表す構造体です。
struct SoundThreadUpdateProfile
{
    TimeRange soundThreadProcess;          //!< サウンドスレッドの１ループ分の処理にかかった時間です。

    TimeRange _updateElapsedFrameCountProcess;  //!< @internal 低レイヤーの経過フレーム数の更新にかかった時間です。
    TimeRange _updateLowLevelVoiceProcess;      //!< @internal 低レイヤーボイスの更新処理にかかった時間です。
    TimeRange _updateLowLevelVoiceParamProcess; //!< @internal 低レイヤーボイスへの処理結果反映にかかった時間です。
    TimeRange _updateRendererProcess;           //!< @internal オーディオレンダラの更新関数の呼び出しにかかった時間です。
    TimeRange _waitRendererEventProcess;        //!< @internal レンダラのイベント通知が届くまでに待った時間です。
    TimeRange _userEffectFrameProcess;          //!< @internal ユーザエフェクトの更新処理にかかった時間です。
    TimeRange _frameProcess;                    //!< @internal W のインスタンス更新処理にかかった時間です。
};

//! @briefprivate
//! プロファイルリーダークラスです。
template <typename TProfile>
class AtkProfileReader
{
public:
    //! @briefprivate
    //! コンストラクタです。
    AtkProfileReader() NN_NOEXCEPT
        : m_IsInitialized( false )
        , m_pProfile( nullptr )
        , m_ProfileCount( 0 )
        , m_RecordIndex( 0 )
        , m_ReadIndex( 0 )
        , m_ReadableCount( 0 )
    {
    }

    //! @briefprivate
    //! 初期化に必要なメモリサイズを返します。
    //!
    //! @param[in] profileMaxCount 蓄えるプロファイルの最大数を指定します。
    //!
    //! @pre
    //! - profileMaxCount > 0
    //!
    static size_t GetRequiredMemorySize(int profileMaxCount) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_GREATER( profileMaxCount, 0 );
        return sizeof( TProfile ) * profileMaxCount;
    }

    //! @brief 初期化に必要なメモリサイズを返します。
    //! @deprecated この関数は廃止予定です。代わりに @ref GetRequiredMemorySize を使用してください。
    NN_DEPRECATED static size_t GetRequirdMemorySize(int profileMaxCount) NN_NOEXCEPT
    {
        return GetRequiredMemorySize( profileMaxCount );
    }

    //! @briefprivate
    //! プロファイルリーダーを初期化します。
    //!
    //! 初期化には @ref GetRequiredMemorySize で得られた値以上のサイズのバッファが必要です。
    //! また、引数の profileMaxCount には @ref GetRequiredMemorySize と同じ値を指定してください。
    //!
    //! プロファイル情報は @ref Read 関数を呼び出すことで取得することができます。
    //!
    //! @param[in] buffer @ref GetRequiredMemorySize で得られたサイズ以上のバッファです。
    //! @param[in] bufferSize buffer のサイズです。
    //! @param[in] profileMaxCount 蓄えるプロファイルの最大数を指定します。@ref GetRequiredMemorySize と同じ値を指定してください。
    //!
    //! @pre
    //! - buffer != NULL
    //! - bufferSize >= @ref GetRequiredMemorySize で得られた値
    //! - profileMaxCount > 0
    //!
    void Initialize(void* buffer, size_t bufferSize, int profileMaxCount) NN_NOEXCEPT
    {
        NN_UNUSED( bufferSize );
        NN_SDK_REQUIRES_NOT_NULL( buffer );
        NN_SDK_ASSERT_GREATER( profileMaxCount, 0 );
        NN_SDK_REQUIRES_GREATER_EQUAL( bufferSize, GetRequiredMemorySize( profileMaxCount ) );

        m_pProfile = reinterpret_cast<TProfile*>( buffer );
        m_ProfileCount = profileMaxCount;

        m_RecordIndex = 0;
        m_ReadIndex = 0;
        m_ReadableCount = 0;

        m_IsInitialized = true;
    }

    //! @briefprivate
    //! プロファイルリーダーを終了処理します。
    void Finalize() NN_NOEXCEPT
    {
        m_pProfile = nullptr;
        m_ProfileCount = 0;

        m_RecordIndex = 0;
        m_ReadIndex = 0;
        m_ReadableCount = 0;

        m_IsInitialized = false;
    }

    //! @briefprivate
    //! プロファイル情報を取得します。
    //!
    //! 引数で指定した pProfile に最大 maxCount の個数のタスクプロファイル情報を格納しますが、
    //! プロファイルリーダーで読み取れるプロファイルの数が maxCount に満たない場合は、
    //! 読み取ることができる個数分だけ格納されます。
    //!
    //! @param[in] pProfile 読み取ったプロファイル情報の格納先を指定します。
    //! @param[in] maxCount 読み取るプロファイル情報の最大数を指定します。
    //!
    //! @return 格納したタスクプロファイル情報の個数を返します。
    //!
    //! @pre
    //! - pProfile != NULL
    //!
    int Read(TProfile* pProfile, int maxCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL( pProfile );
        const int count = std::min<int>( maxCount, m_ReadableCount );

        for( int i = 0; i < count; i++ )
        {
            pProfile[i] = m_pProfile[m_ReadIndex++];
            if( m_ReadIndex == m_ProfileCount )
            {
                m_ReadIndex = 0;
            }
        }

        m_ReadableCount.fetch_sub( count );
        return count;
    }

    //! @internal
    void Record(const TProfile& profile) NN_NOEXCEPT
    {
        if( m_ReadableCount < m_ProfileCount )
        {
            m_pProfile[m_RecordIndex++] = profile;
            if( m_RecordIndex == m_ProfileCount )
            {
                m_RecordIndex = 0;
            }

            m_ReadableCount.fetch_add( 1 );
        }
    }

    //! @briefprivate
    //! @return プロファイルリーダーが初期化されているかを返します。
    bool IsInitialized() const NN_NOEXCEPT
    {
        return m_IsInitialized;
    }

public:
    //! @internal
    util::IntrusiveListNode m_List;

private:
    NN_DISALLOW_COPY( AtkProfileReader );
    bool m_IsInitialized;

    TProfile* m_pProfile;
    int m_ProfileCount;

    int m_RecordIndex;
    int m_ReadIndex;
    std::atomic<int> m_ReadableCount;
};

//! @internal
typedef util::IntrusiveList< ProfileReader, util::IntrusiveListMemberNodeTraits<ProfileReader, &ProfileReader::m_Link> > ProfileReaderList;

//! @internal
typedef AtkProfileReader<SoundThreadUpdateProfile> SoundThreadUpdateProfileReader;

//! @internal
typedef util::IntrusiveList< SoundThreadUpdateProfileReader, util::IntrusiveListMemberNodeTraits<SoundThreadUpdateProfileReader, &SoundThreadUpdateProfileReader::m_List> > SoundThreadUpdateProfileReaderList;

} // namespace nn::atk
} // namespace nn
