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

#include <nn/nn_SdkAssert.h>
#include <nn/atk/detail/atk_Macro.h>
#include <nn/atk/fnd/basis/atkfnd_Config.h>
#include <nn/atk/fnd/basis/atkfnd_Result.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief  ストリームの基本クラスです。
//---------------------------------------------------------------------------
class Stream
{
    NN_DISALLOW_COPY(Stream);

public: // 定数の定義
    static const position_t InvalidPosition = UINT_MAX;
    static const size_t InvalidSize = UINT_MAX;

public: // 型の定義
    enum SeekOrigin
    {
        SeekOrigin_Begin = 0,
        SeekOrigin_End,
        SeekOrigin_Current
    };

public: // デストラクタ
    virtual ~Stream() NN_NOEXCEPT { }

protected: // コンストラクタ
    Stream() NN_NOEXCEPT { }

public: // メソッド
    virtual void Close() NN_NOEXCEPT = 0;

    virtual bool IsOpened() const NN_NOEXCEPT = 0;

    virtual size_t Read(void* /*buf*/, size_t /*length*/, FndResult* result = NULL) NN_NOEXCEPT
    {
        NN_ATK_WARNING("stream reading is not supported.");

        if(result != NULL)
        {
            *result = FndResult(FndResultType_NotSupported);
        }

        return 0;
    }

    virtual size_t Write(const void* /*buf*/, size_t /*length*/, FndResult* result = NULL) NN_NOEXCEPT
    {
        NN_ATK_WARNING("stream writing is not supported.");

        if(result != NULL)
        {
            *result = FndResult(FndResultType_NotSupported);
        }

        return 0;
    }

    virtual FndResult Seek(position_t /*offset*/, SeekOrigin /*origin*/) NN_NOEXCEPT
    {
        NN_ATK_WARNING("stream seeking is not supported.");

        return FndResult(FndResultType_NotSupported);
    }

    virtual position_t GetCurrentPosition() const NN_NOEXCEPT
    {
        return InvalidPosition;
    }

    virtual size_t GetSize() const NN_NOEXCEPT
    {
        return InvalidSize;
    }

    virtual bool CanRead() const NN_NOEXCEPT { return false; }

    virtual bool CanWrite() const NN_NOEXCEPT { return false; }

    virtual bool CanSeek() const NN_NOEXCEPT{ return false; }
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  指定ストリームからの読み込みをサポートします。
//---------------------------------------------------------------------------
class StreamReader
{
    NN_DISALLOW_COPY(StreamReader);

public: // コンストラクタ
    explicit StreamReader(Stream* stream) NN_NOEXCEPT:
      m_Stream(stream)
    {
        NN_SDK_ASSERT_NOT_NULL(stream);
    }

    virtual ~StreamReader() NN_NOEXCEPT { }

public: // メソッド
    virtual size_t Read(void* buf, size_t length, FndResult* result = NULL) NN_NOEXCEPT
    {
        return m_Stream->Read(buf, length, result);
    }

private: // メンバ変数
    Stream* m_Stream;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  指定ストリームからの書き込みをサポートします。
//---------------------------------------------------------------------------
class StreamWriter
{
    NN_DISALLOW_COPY(StreamWriter);

public: // コンストラクタ
    StreamWriter() NN_NOEXCEPT :
      m_Stream(NULL),
      m_WriteFormatBuffer(NULL),
      m_WriteFormatBufferLength(0)
    {
    }

    explicit StreamWriter(Stream* stream) NN_NOEXCEPT :
      m_Stream(stream),
      m_WriteFormatBuffer(NULL),
      m_WriteFormatBufferLength(0)
      {
          NN_SDK_ASSERT_NOT_NULL(stream);
      }

      virtual ~StreamWriter() NN_NOEXCEPT { }

public: // メソッド
    FndResult Open(Stream* stream) NN_NOEXCEPT
    {
        if(stream == NULL)
        {
            NN_SDK_ASSERT(false);
            return FndResult(FndResultType_Failed);
        }

        m_Stream = stream;
        return FndResult(FndResultType_True);
    }

    void Close() NN_NOEXCEPT
    {
        m_Stream = NULL;
    }

    bool IsOpened() const NN_NOEXCEPT { return m_Stream != NULL; }

    size_t Write(const void* buf, size_t length, FndResult* result = NULL) NN_NOEXCEPT
    {
        NN_SDK_ASSERT(IsOpened(), "StreamWriter must be opened.\n");
        NN_SDK_ASSERT_NOT_NULL(buf);
        return m_Stream->Write(buf, length, result);
    }

    FndResult Write(const char* text) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(text);
        return Write(text, static_cast<int>(std::strlen(text)));
    }

    FndResult Write(const char* text, int length) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(text);
        NN_SDK_ASSERT(length > 0);

        FndResult result(FndResultType_True);
        Write(text, length, &result);
        return result;
    }

    FndResult WriteLine(const char* text) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(text);
        return WriteLine(text, static_cast<int>(std::strlen(text)));
    }

    FndResult WriteLine(const char* text, int length) NN_NOEXCEPT;

    FndResult WriteFormat(const char* format, ...) NN_NOEXCEPT;
    FndResult VWriteFormat(const char* format, va_list vargs) NN_NOEXCEPT;

    void EnableWriteFormat(void* buffer, int length) NN_NOEXCEPT;
    void DisableWriteFormat() NN_NOEXCEPT;

private: // メンバ変数
    Stream* m_Stream;
    char*   m_WriteFormatBuffer;
    int     m_WriteFormatBufferLength;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  ストリーム入力をアライメント調整します。
//---------------------------------------------------------------------------
class StreamAligner
{
    NN_DISALLOW_COPY(StreamAligner);

public: // コンストラクタ
    StreamAligner() NN_NOEXCEPT :
      m_Stream(NULL),
      m_Alignment(0),
      m_BufferForAlignment(NULL),
      m_BufferLengthForAlignment(0)
      { }

    virtual ~StreamAligner() NN_NOEXCEPT { }

public: // メソッド
    void Open(Stream* stream, int alignment, void* buf, size_t length) NN_NOEXCEPT;
    void Close() NN_NOEXCEPT;

    bool IsOpened() const NN_NOEXCEPT { return m_Stream != NULL && m_Stream->IsOpened(); }

    size_t GetRequiredBufferLength(int alignment) const NN_NOEXCEPT
    {
        return alignment * 2;
    }

    size_t Read(void* buf, size_t length, FndResult* result = NULL) NN_NOEXCEPT;

private: // メンバ変数
    Stream* m_Stream;

    int m_Alignment;
    void* m_BufferForAlignment;
    size_t m_BufferLengthForAlignment;
};

} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
