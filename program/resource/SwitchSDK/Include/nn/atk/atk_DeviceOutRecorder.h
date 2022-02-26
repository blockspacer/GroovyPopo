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
#include <nn/os/os_Event.h>
#include <nn/os/os_MessageQueue.h>
#include <nn/atk/fnd/os/atkfnd_Thread.h>
#include <nn/atk/atk_WavOutFileStream.h>
#include <nn/util/util_Endian.h>
#include <nn/util/util_BytePtr.h>

//#define NN_ATK_DEBUG_REC_BUFFER

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @internal
//! @brief  デバイスの出力波形を録音する抽象クラスです。
//!
//!         このクラスは内部でスレッドを生成し、対象デバイスの出力波形を
//!         Wav ファイル形式で指定 FileStream に出力します。
//---------------------------------------------------------------------------
class DeviceOutRecorder : public detail::fnd::Thread::Handler
{
    NN_DISALLOW_COPY( DeviceOutRecorder );

private:
    // RecordSamples で CircularBufferSink から取得した波形をバッファリングした後、
    // OnWriteSamples でバッファリングした波形を読み出したとき、0 埋めされた波形を取得する問題があります。
    // RecordingBufferSize を 1 オーディオフレームから得られるサンプル数 1440 の倍数に設定した場合、
    // この問題を回避できるため、RecordingBufferSize の値を 1440 の倍数に設定しています。
    static const uint32_t RecordingBufferSize          = 256 * 1440;                    //!< 録音バッファサイズ
    static const uint32_t DefaultWriteBlockPerSamples  = 128 * 1024 / sizeof(int16_t);  //!< 一括して書き込むサンプル数のデフォルト値

public:
    //---------------------------------------------------------------------------
    //! @brief  初期化オプションです。
    //!         バッファ領域やバッファサイズ以外の、
    //!         初期化時に必ずしも指定が必要でないパラメーターはこちらで指定します。
    //---------------------------------------------------------------------------
    class InitializationOptions
    {
    public:
        InitializationOptions() NN_NOEXCEPT :
            m_Priority(16),
            m_IdealCoreNumber(0)
        { }

        //! @brief  録音スレッドのプライオリティを取得します。
        uint32_t GetPriority() const NN_NOEXCEPT
        {
            return m_Priority;
        }

        //! @brief  録音スレッドのプライオリティを設定します。
        void SetPriority(uint32_t priority) NN_NOEXCEPT
        {
            m_Priority = priority;
        }

        //! @brief 録音スレッドの動作するコアを取得します。
        int GetIdealCoreNumber() const NN_NOEXCEPT
        {
            return m_IdealCoreNumber;
        }

        //! @brief 録音スレッドの動作するコアを設定します。
        void SetIdealCoreNumber(int idealCoreNumber) NN_NOEXCEPT
        {
            m_IdealCoreNumber = idealCoreNumber;
        }

    private:
        uint32_t m_Priority;
        int m_IdealCoreNumber;
    };

    //---------------------------------------------------------------------------
    //! @brief  録音オプションです。
    //---------------------------------------------------------------------------
    class RecordingOptions
    {
    public:
        RecordingOptions() NN_NOEXCEPT :
            m_Channels(0),
            m_IsLeadSilenceTrimmingEnabled(false),
            m_MaxFrames(0),
            m_WriteBlockPerSamples(DefaultWriteBlockPerSamples)
        { }

        //! @brief  出力する波形のチャンネル数を取得します。
        uint32_t GetChannels() const NN_NOEXCEPT
        {
            return m_Channels;
        }

        //! @brief  出力する波形のチャンネル数を設定します。
        void SetChannels(uint32_t channels) NN_NOEXCEPT
        {
            m_Channels = channels;
        }

        //! @brief  先頭の無音をトリミングする場合かどうかを取得します。
        bool IsLeadSilenceTrimmingEnabled() const NN_NOEXCEPT
        {
            return m_IsLeadSilenceTrimmingEnabled;
        }

        //! @brief  先頭の無音をトリミングするかどうかを設定します。
        void SetLeadSilenceTrimmingEnabled(bool isEnabled) NN_NOEXCEPT
        {
            m_IsLeadSilenceTrimmingEnabled = isEnabled;
        }

        //! @brief 録音する波形の最大フレーム数を取得します。
        uint32_t GetMaxFrames() const NN_NOEXCEPT
        {
            return m_MaxFrames;
        }

        //! @brief 録音する波形の最大フレーム数を設定します。
        void SetMaxFrames(uint32_t maxFrames) NN_NOEXCEPT
        {
            m_MaxFrames = maxFrames;
        }

        //! @brief 一括して書き込むサンプル数を取得します。
        uint32_t GetWriteBlockPerSamples() const NN_NOEXCEPT
        {
            return m_WriteBlockPerSamples;
        }

        //! @brief 一括して書き込むサンプル数を設定します。
        void SetWriteBlockPerSamples(uint32_t samples) NN_NOEXCEPT
        {
            m_WriteBlockPerSamples = samples;
        }

    private:
        uint32_t  m_Channels;
        bool m_IsLeadSilenceTrimmingEnabled;
        uint32_t  m_MaxFrames;
        uint32_t  m_WriteBlockPerSamples;
    };

    //---------------------------------------------------------------------------
    //! @brief  DeviceOutRecorder の状態です。
    //---------------------------------------------------------------------------
    enum State
    {
        //! @brief  未初期化状態です。
        State_NotInitialized,

        //! @brief  初期化済みです。
        State_Initialized,

        //! @brief  録音中です。
        State_Recording,

        //! @brief  指定したフレーム数の録音が完了した状態です。
        //! @see    RecordingOptions
        State_Recorded
    };

private:
    //---------------------------------------------------------------------------
    //! @brief  スレッドに通知するメッセージです。
    //---------------------------------------------------------------------------
    enum Message
    {
        Message_Prepare,        //!< 録音の準備を行います。
        Message_WriteSamples,   //!< 録音サンプルを出力します。
        Message_RequestStop,    //!< 録音の終了を行います
        Message_Exit,           //!< スレッドを終了します。
    };

    //---------------------------------------------------------------------------
    //! @brief  録音波形用のバッファクラスです。
    //---------------------------------------------------------------------------
    class RecorderBuffer
    {
    public:
        explicit RecorderBuffer(const char* deviceName = NULL) NN_NOEXCEPT;

    public:
        void Initialize(int16_t* sampleBuffer, uint32_t maxSamples) NN_NOEXCEPT;
        void Finalize() NN_NOEXCEPT;

        uint32_t Push(const int16_t* sampleBuffer, uint32_t samples) NN_NOEXCEPT;
        uint32_t Pop(uint32_t samples) NN_NOEXCEPT;
        int16_t* Peek() NN_NOEXCEPT;

        void SetReadBlockSamples(uint32_t value) NN_NOEXCEPT;
        void Clear() NN_NOEXCEPT;

        inline uint32_t GetReadableCount() const NN_NOEXCEPT
        {
            return m_ValidSamples / m_ReadBlockSamples * m_ReadBlockSamples;
        }

        inline uint32_t GetWritableCount() const NN_NOEXCEPT;
        inline uint32_t GetContiguousReadableCount() const NN_NOEXCEPT;

        const char* GetDeviceName() const NN_NOEXCEPT { return m_DeviceName; }

    private:
        //---------------------------------------------------------------------------
        //! @brief  書き込み状態を表す構造体です。
        //---------------------------------------------------------------------------
        struct WriteState
        {
            uint32_t channelIndex;
            uint32_t writtenSampleCount;
        };

    private:
        void UpdateMaxSamples() NN_NOEXCEPT;

        inline void Skip(uint32_t samples) NN_NOEXCEPT;
        inline void Write(const int16_t* sampleBuffer, uint32_t samples) NN_NOEXCEPT;

        inline uint32_t IncrementPosition(uint32_t position, uint32_t length) const NN_NOEXCEPT;

    private:
        int16_t* m_SampleBuffer;               //!< バッファ
        uint32_t m_MaxBufferSamples;           //!< バッファに格納できる最大サンプル数
        uint32_t m_MaxSamples;                 //!< ブロックバッファに格納できる最大サンプル数
        std::atomic<uint32_t> m_ValidSamples;  //!< バッファに格納されている有効サンプル数
        uint32_t m_ReadPosition;               //!< 読み込みポインタの位置
        uint32_t m_WritePosition;              //!< 書き込みポインタの位置
        uint32_t m_ReadBlockSamples;           //!< 読み込みブロックのサンプル数
        WriteState m_WriteState;               //!< 書き込み状態

        const char* m_DeviceName;              //!< デバイス名

#if defined(NN_SDK_BUILD_DEBUG) || defined(NN_SDK_BUILD_DEVELOP)
        bool m_IsWarnedDropSamples;            //!< フレーム落ちの警告フラグ
#endif

#ifdef NN_ATK_DEBUG_REC_BUFFER
        static const uint32_t g_PrintInterval = 200; //!< 1デバッグ出力の感覚（RecorderBuffer::Push() 回数）
        uint32_t              m_PushCount;           //!< RecorderBuffer::Push() 回数
#endif // NN_ATK_DEBUG_REC_BUFFER
    };

public:
    //! @internal
    virtual ~DeviceOutRecorder() NN_NOEXCEPT;

protected:
    //! @internal
    //! @param deviceName :private
    explicit DeviceOutRecorder(const char* deviceName = NULL) NN_NOEXCEPT;

public:
    //---------------------------------------------------------------------------
    //! @brief  初期化時に必要なスレッドスタックのサイズです。
    //!         Initialize() の際に事前条件として必要となります。
    //!
    //! @ref    Initialize()
    //---------------------------------------------------------------------------
    static const size_t RequiredThreadStackSize      = 64 * 1024;

    //---------------------------------------------------------------------------
    //! @brief         DeviceOutRecorder を初期化します。
    //!                録音用スレッドはこのタイミングで生成されます。
    //!
    //! @param[in]     recordingBuffer      波形を保存するためのバッファです。
    //! @param[in]     recordingBufferSize  波形を保存するためのバッファの大きさです。
    //! @param[in]     pThreadStack         録音スレッドのスタックとして使用するバッファです。
    //! @param[in]     threadStackSize      録音スレッドのスタックとして使用するバッファの大きさです。
    //!
    //! @pre
    //! - recordingBuffer != nullptr
    //! - recordingBufferLength >= GetRequiredMemorySizeForRecording()
    //! - pThreadStack != nullptr
    //! - pThreadStack のアドレスが nn::os::ThreadStackAlignment にアラインしている
    //! - threadStackSize >= RequiredThreadStackSize
    //!
    //! @return        初期化に成功した場合は true、失敗した場合は false を返します。
    //---------------------------------------------------------------------------
    bool Initialize(void* recordingBuffer, size_t recordingBufferSize, void* pThreadStack, size_t threadStackSize) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief         DeviceOutRecorder を初期化します。
    //!                録音用スレッドはこのタイミングで生成されます。
    //!
    //! @param[in]     recordingBuffer      波形を保存するためのバッファです。
    //! @param[in]     recordingBufferSize  波形を保存するためのバッファの大きさです。
    //! @param[in]     pThreadStack         録音スレッドのスタックとして使用するバッファです。
    //! @param[in]     threadStackSize      録音スレッドのスタックとして使用するバッファの大きさです。
    //! @param[in]     options              初期化オプションです。
    //!
    //! @pre
    //! - recordingBuffer != nullptr
    //! - recordingBufferSize >= GetRequiredMemorySizeForRecording()
    //! - pThreadStack != nullptr
    //! - pThreadStack のアドレスが nn::os::ThreadStackAlignment にアラインしている
    //! - threadStackSize >= RequiredThreadStackSize
    //!
    //! @return        初期化に成功した場合は true、失敗した場合は false を返します。
    //---------------------------------------------------------------------------
    bool Initialize(void* recordingBuffer, size_t recordingBufferSize, void* pThreadStack, size_t threadStackSize, const InitializationOptions& options) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief         DeviceOutRecorder を破棄します。
    //!                録音用スレッドもこのタイミングで破棄されます。
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief 録音に必要なバッファのサイズを取得します。
    //---------------------------------------------------------------------------
    size_t GetRequiredMemorySizeForRecording() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief         再生波形の録音を開始します。
    //!
    //! @param[in]     fileStream  波形の出力先ファイルストリームです。
    //!
    //! @return        成功した場合は true、失敗した場合は false を返します。
    //---------------------------------------------------------------------------
    bool Start(detail::fnd::FileStream& fileStream) NN_NOEXCEPT
    {
        RecordingOptions options;
        return Start(fileStream, options);
    }

    //---------------------------------------------------------------------------
    //! @brief         再生波形の録音を開始します。
    //!
    //! @param[in]     fileStream  波形の出力先ファイルストリームです。
    //! @param[in]     options     録音オプションを指定します。
    //!                             channels                      出力する波形のチャンネル数です。
    //!                                                           0(初期値) を指定すると、出力モードに見て自動選択します。
    //!                                                           サラウンドなら 6ch、それ以外の場合は 2ch で録音します。
    //!                             priority                      録音スレッドのプライオリティです。
    //!                                                           初期値は 16(OS_PRIORITY_APP_DEFAULT)です。
    //!                             isLeadSilenceTrimmingEnabled  true を指定すると、先頭の無音部分をトリミングします。
    //!                             maxFrames                     録音波形の最大フレーム数です。
    //!                                                           0 以外を指定すると、指定フレーム数に到達したら録音を自動停止し、
    //!                                                           状態を STATE_RECORDED に遷移します。
    //!                                                           0(初期値) を指定すると、上限を設定せず、可能な限り録音を継続します。
    //!
    //! @return        成功した場合は true、失敗した場合は false を返します。
    //---------------------------------------------------------------------------
    bool Start(detail::fnd::FileStream& fileStream, const RecordingOptions& options) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief         録音を停止します。
    //!
    //! @param[in]     isBlocking  true を指定するとファイルへの Write が完了するまで待機します。
    //---------------------------------------------------------------------------
    void Stop(bool isBlocking) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief         初期化の有無を取得します。
    //!
    //! @return        初期化済みの場合は true、未初期化の場合は false を返します。
    //!
    //! @see           GetState
    //---------------------------------------------------------------------------
    bool IsInitialized() const NN_NOEXCEPT { return m_State != State_NotInitialized; }

    //---------------------------------------------------------------------------
    //! @brief         DeviceOutRecorder の状態を取得します。
    //!
    //! @return        初期化済みの場合は true、未初期化の場合は false を返します。
    //!
    //! @see           IsInitialized
    //---------------------------------------------------------------------------
    State GetState() const NN_NOEXCEPT { return m_State; }

    //---------------------------------------------------------------------------
    //! @brief         先頭の無音部分をトリミング中かどうかを調べます。
    //!
    //! @return        トリミング中の場合は true、それ以外の場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsLeadSilenceTrimming() const NN_NOEXCEPT { return m_IsLeadSilenceTrimming; }

    //---------------------------------------------------------------------------
    //! @brief         録音チャンネル数を取得します。
    //!
    //! @return        録音チャンネル数を返します。
    //!
    //! @see           Start
    //---------------------------------------------------------------------------
    uint32_t GetRecordingChannels() const NN_NOEXCEPT { return m_Channels; }

    //! @internal  サンプルを録音します。
    void RecordSamples(const int16_t* sampleBuffer, uint32_t samples) NN_NOEXCEPT;

protected:
    OutputMode GetOutputMode() const NN_NOEXCEPT { return m_OutputMode; }

    virtual uint32_t GetMaxFrameLength() const NN_NOEXCEPT = 0;
    virtual uint32_t GetSamplesPerSec() const NN_NOEXCEPT = 0;
    virtual uint32_t GetValidChannels(uint32_t channels) const NN_NOEXCEPT = 0;

    virtual void OnStart() NN_NOEXCEPT { }
    virtual void OnStop() NN_NOEXCEPT { }
    virtual uint32_t OnProcessSamples(int16_t* /* sampleBuffer */, uint32_t samples) NN_NOEXCEPT { return samples; }

    int16_t ResolveSampleEndian(int16_t sample) NN_NOEXCEPT
    {
        return sample;
    }

    virtual uint32_t Run(void* param) NN_NOEXCEPT;

private:
    uint32_t GetReadBlockSamples(uint32_t channels) const NN_NOEXCEPT;
    uint32_t GetLeadSilenceSamples(const int16_t* sampleBuffer, uint32_t samples, uint32_t channels) const NN_NOEXCEPT;
    uint32_t GetWritableSamples(uint32_t samples) const NN_NOEXCEPT;
    bool IsNoMoreSamples() const NN_NOEXCEPT;

    bool StartThread(uint32_t priority, int idealCoreNumber) NN_NOEXCEPT;
    void StopThread() NN_NOEXCEPT;

    int32_t  Prepare() NN_NOEXCEPT;
    bool SendMessage(Message message) NN_NOEXCEPT;
    bool PostMessage(Message message) NN_NOEXCEPT;

    int32_t OnPrepare() NN_NOEXCEPT;
    void OnRequestStop() NN_NOEXCEPT;
    void OnExit() NN_NOEXCEPT;
    bool OnWriteSamples(bool isForceWriteMode) NN_NOEXCEPT;

private:
    volatile State m_State;                     //!< 状態
    uint32_t            m_Channels;                  //!< 録音チャンネル数
    OutputMode     m_OutputMode;                //!< 出力モード
    bool           m_IsLeadSilenceTrimming;     //!< 先頭無音部分のトリミング中
    uint32_t            m_MaxSamples;                //!< 最大サンプル数
    uint32_t            m_WrittenSamples;            //!< 書き込み済みサンプル数

    nn::atk::detail::fnd::Thread m_Thread;          //!< 波形録音スレッド
    void* m_ThreadStack;    //!< 波形録音スレッドスタック

    nn::os::MessageQueue m_MessageQueue;            //!< メッセージキュー
    uintptr_t m_Message;                 //!< メッセージスタック（格納数は１つ）
    int32_t              m_MessageResult;           //!< 同期メッセージの結果
    nn::os::Event        m_MessageDoneEvent;        //!< 同期メッセージの完了待ちイベント

    detail::fnd::FileStream*   m_Stream;        //!< 出力先ファイルストリーム
    detail::WavOutFileStream m_WavOutStream;  //!< 波形ファイル出力ストリーム
    RecorderBuffer             m_RecordingBuffer;        //!< 波形を保存するためのバッファ
    nn::util::BytePtr          m_WorkBuffer;             //!< 録音処理に必要なワークバッファ
    uint32_t                   m_WriteBlockPerSamples;   //!< 一括して書き込むサンプル数
};

} // atk
} // nn
