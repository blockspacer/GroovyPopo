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

#include <nn/spy/spy_Config.h>
#ifdef NN_BUILD_CONFIG_SPY_ENABLED

//!
//! @def NN_SPY_DATA_FILE_CHANNEL_AVAILABLE
//!
//! ファイルを使用した高速なデータ通信が利用できる場合に定義されます。
//! このマクロが定義された場合は nn::spy::SpyController::OpenArg::dataFileChannelParam メンバー変数によりパラメータを指定することができます。
//!

#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
#define NN_SPY_DATA_FILE_CHANNEL_AVAILABLE
#endif

#if defined(NN_SPY_DATA_FILE_CHANNEL_AVAILABLE)

#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
#include <cafe/fs.h>
#endif

#include <nn/spy/detail/fnd/os/spyfnd_CriticalSection.h>

namespace nn {
namespace spy {

//! @brief Spy.exeとのHostFileIOによる通信ためのパラメータです。
//!
//! @details
//! HostFileIOを使うことでSpy.exeとの通信がより高速になり、
//! 最終波形出力などの大きなデータの送信に余裕ができます。
//!
//! Spy.exeから通知される作業用ディレクトリに通信用データファイルが２つ作成されます。
//! 通信用データファイルのサイズは上限値を超える場合があります。
//!
struct SpyDataFileChannelParam
{
#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
    //! 通信用ファイルの操作に使う FSClient を指定します(必須)。
    FSClient* fsClient;

    //! 通信用ファイルのサイズの上限を指定します。デフォルトは32MBです。
    uint32_t dataFileSizeLimit;

    //! HostFileIO のマウントパスを指定します(必須)。
    //! 指定した文字列は SpyController::Open() の中でコピーされるので、呼び出し後に破棄できます。
    const char* hfioMountPath;
#endif

    SpyDataFileChannelParam() NN_NOEXCEPT;
};

namespace detail {

//! @brief Spy.exeへのデータ通信をHostFileIOで行います。
class SpyDataFileChannel
{
public: // 型

private: // 型

public: // コンストラクタ
    SpyDataFileChannel() NN_NOEXCEPT;

public: // メソッド
    //! @brief Initialize() に必要なメモリ量を取得します。
    static size_t GetRequiredMemorySize() NN_NOEXCEPT;

    //! @brief SpyController::Initialize() から呼ばれます。
    void Initialize(void* buffer) NN_NOEXCEPT;

    //! @brief SpyController::Finalize() から呼ばれます。
    void Finalize() NN_NOEXCEPT;

    //! @brief SpyController::Open() から呼ばれます。
    bool Open(const SpyDataFileChannelParam& param) NN_NOEXCEPT;

    //! @brief SpyController::Close() から呼ばれます。
    void Close() NN_NOEXCEPT;

    //! @brief Open状態かを取得します。
    bool IsOpened() const NN_NOEXCEPT;

    //! @brief Spy.exe から SetOutputDirPacket を受信したときに呼び出されます。
    bool BeginSession(const char* outputDir) NN_NOEXCEPT;

    //! @brief Spy.exe との通信が切断されたときに呼び出されます。
    void EndSession() NN_NOEXCEPT;

    //! @brief セッション中かを取得します。
    bool IsActive() const NN_NOEXCEPT;

    //! @brief Spy.exe から NotifyDataReadPacket を受信したときに呼び出されます。
    bool SetCurrentReadFile(int fileIndex) NN_NOEXCEPT;

    //! @brief Spy.exe にデータを送信します。
    //!
    //! buffer は FS_IO_BUFFER_ALIGN でアライメントしている必要があります。
    //! buffer は破壊的に利用されます。
    //! buffer は最低でも sizeof(PingPacket) の大きさがなければなりません。
    //!
    bool WriteData(void* buffer, size_t length) NN_NOEXCEPT;

private: // メソッド
    bool SetOutputDirPath(const char* outputDir) NN_NOEXCEPT;
    bool OpenFiles() NN_NOEXCEPT;
    void CloseFiles() NN_NOEXCEPT;

private: // メンバ変数
#if defined(NN_BUILD_CONFIG_SPEC_CAFE)
    nn::spy::detail::fnd::CriticalSection m_DataFileLock;
    FSClient* m_pFsClient;
    FSCmdBlock* m_pFsCmdBlock;
    FSFileHandle m_DataOutputFile[2];
    uint32_t m_MaxWriteLength;
    uint32_t m_WriteLength;
    uint8_t m_CurrentWriteFile;
    uint8_t m_CurrentReadFile;
    char m_HfioMountPath[FS_MAX_MOUNTPATH_SIZE];
    char m_OutputDirMountPath[FS_MAX_ARGPATH_SIZE + FS_MAX_ARGPATH_SIZE_PAD];
    bool m_DataOutputFileOpen;
#endif
};

} // namespace nn::spy::detail
} // namespace nn::spy
} // namespace nn

#endif // NN_SPY_DATA_FILE_CHANNEL_AVAILABLE

#endif // NN_BUILD_CONFIG_SPY_ENABLED
