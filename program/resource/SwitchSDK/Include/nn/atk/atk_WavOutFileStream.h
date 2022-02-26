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

#include <nn/atk/atk_Config.h>
#include <nn/atk/fnd/io/atkfnd_FileStreamImpl.h>

namespace nn {
namespace atk {
namespace detail {

//---------------------------------------------------------------------------
//! @brief  Wavバイナリを出力するファイルストリームクラスです。
//---------------------------------------------------------------------------
class WavOutFileStream
{
private:
    static const int FileIoBufferAlignment = 1;   //!< ファイルIOバッファアライメント

public:
    /* ctor */ WavOutFileStream() NN_NOEXCEPT;
    /* dtor */ ~WavOutFileStream() NN_NOEXCEPT;

public:
    //---------------------------------------------------------------------------
    //! @brief       Wavバイナリ出力ストリームを開きます。
    //!
    //! @param[in]   stream         出力先のストリーム
    //! @param[in]   channels       波形のチャンネル数
    //! @param[in]   samplesPerSec  波形のサンプルレート
    //!
    //! @return      成功した場合は true、失敗した場合は false を返します。
    //---------------------------------------------------------------------------
    bool Open(fnd::FileStream& stream, int channels, size_t samplesPerSec) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief       Wavバイナリ出力ストリームを閉じます。
    //---------------------------------------------------------------------------
    void Close() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief       ストリームにデータを書き込みます。(同期版)
    //!
    //! @param[in]   buf         書き込み元バッファ
    //! @param[in]   length      書き込みデータのバイト数
    //!
    //! @return      実際に書き込まれたバイト数を返します。
    //!               書き込みに失敗した場合には負のエラーコードを返します。
    //---------------------------------------------------------------------------
    size_t Write(const void* buf, size_t length) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        ファイルポインタを移動します。
    //!
    //! @param[in]    offset  ファイルポインタ移動値を指定します。
    //! @param[in]    origin  ファイルポインタ移動の基準点を指定します。
    //!                       SeekOrigin_Begin      - ファイルの先頭を基準
    //!                       SeekOrigin_Current    - 現在の読み込み位置を基準
    //!                       SeekOrigin_End        - ファイルの終端を基準(offset <= 0で指定されるはずです)
    //!
    //! @return       ファイルポインタが移動できたときは true を返します。
    //---------------------------------------------------------------------------
    bool Seek(position_t offset, fnd::Stream::SeekOrigin origin) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief   ストリームがオープンできているかを取得します。
    //!
    //! @return  ストリームがオープンできている場合には true を返します。
    //---------------------------------------------------------------------------
    bool IsAvailable() const NN_NOEXCEPT
    {
        return m_pFileStream != NULL && m_pFileStream->IsOpened();
    }

    //---------------------------------------------------------------------------
    //! @brief  ファイルサイズを取得します。
    //!
    //! @return ファイルサイズを返します。
    //---------------------------------------------------------------------------
    size_t  GetSize() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(m_pFileStream);
        return m_pFileStream->GetSize() + m_ValidBufferLength;
    }

    //---------------------------------------------------------------------------
    //! @brief  ファイルポインタの位置を取得します。
    //!
    //! @return ファイルポインタの位置（先頭からのバイト数）を返します。
    //---------------------------------------------------------------------------
    position_t Tell() const NN_NOEXCEPT { return m_pFileStream->GetCurrentPosition(); }

    //---------------------------------------------------------------------------
    //! @brief         波形データキャッシュ用のバッファを設定します。
    //!
    //! @param[in]     buf    バッファへのポインタ
    //! @param[in]     length バッファの長さ
    //---------------------------------------------------------------------------
    void SetCacheBuffer(char* buf, size_t length) NN_NOEXCEPT;

private:
    //---------------------------------------------------------------------------
    //! @brief  ヘッダを出力します。
    //---------------------------------------------------------------------------
    bool WriteHeader(int channels, size_t samplesPerSec) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  RIFF チャンクサイズを更新します。
    //---------------------------------------------------------------------------
    bool UpdateRiffChunkSize() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  data チャンクサイズを更新します。
    //---------------------------------------------------------------------------
    bool UpdateDataChunkSize() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  RIFF チャンクサイズを計算します。
    //---------------------------------------------------------------------------
    size_t CalcRiffChunkSize(size_t dataSize) NN_NOEXCEPT;

    size_t WriteDirect(const void* buf, size_t length, fnd::FndResult* pResult) NN_NOEXCEPT;

    size_t FlushBuffer() NN_NOEXCEPT;

private:
    fnd::FileStream* m_pFileStream;
    size_t  m_WaveDataSize;
    bool m_IsWaveDataSizeCalculating;

    char* m_Buffer;
    size_t m_BufferLength;
    size_t m_ValidBufferLength;
};

} // detail
} // atk
} // nn
