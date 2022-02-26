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

#include <nn/util/util_StringView.h>
#include <nn/util/util_Endian.h>
#include <nn/ige/ige_Buffer.h>
#include <nn/nn_Allocator.h>

namespace nn { namespace ige {

//! @briefprivate バッファへの値の書き込みを補助するクラスです。
class BufferWriter
{
    NN_DISALLOW_COPY(BufferWriter);
public:

    //! @briefprivate   書き込み先のバッファを指定して BufferWriter を構築します。
    //! @param[in]      workBuffer  書き込み可能なバッファです。
    explicit BufferWriter(BufferReference workBuffer) NN_NOEXCEPT;

    //! @briefprivate   メモリリソースを指定して伸長可能な BufferWriter を構築します。
    //! @param[in]      pResource   メモリリソースです。
    explicit BufferWriter(MemoryResource* pResource) NN_NOEXCEPT;

    //! @briefprivate   BufferWriter を破棄します。
    ~BufferWriter() NN_NOEXCEPT;

    //! @briefprivate           バッファに値を書き込みます。
    //! @param[in]  ptr         値の先頭のポインタです。
    //! @param[in]  startIndex  先頭のポインタから実際に書き込む値までのオフセット（バイト数）です。
    //! @param[in]  count       書き込むバイト数です。
    void Write(const void* ptr, int startIndex, size_t count) NN_NOEXCEPT;

    //! @briefprivate       バッファに値を書き込みます。
    //! @param[in]  value   書き込む値です。
    template <typename T>
    void Write(T value) NN_NOEXCEPT
    {
        WriteValue(this, value);
    }

    //! @briefprivate  書き込み済みのバッファサイズを取得します。
    //! @return 書き込み済みのバッファサイズを取得します。
    size_t GetSize() const NN_NOEXCEPT
    {
        return m_Size;
    }

    //! @briefprivate  書き込み済みのバッファを取得します。
    //! @return 書き込み済みのバッファを取得します。
    BufferView GetBuffer() const NN_NOEXCEPT
    {
        return BufferView(m_WorkBuffer.GetPtr(), m_Size);
    }

private:
    void ExpandBuffer(size_t size) NN_NOEXCEPT;

    MemoryResource* m_pResource;
    BufferReference m_WorkBuffer;
    size_t m_Size;
};

// nn::ige 名前空間に void WriteValue(BufferWriter* pWriter, T value) のオーバーロードを用意することで
// BufferWriter.Write(T value) から実引数依存の名前探索によって呼び出すことができます。
// 値はリトルエンディアンで書き込む必要があります。

inline void WriteValue(BufferWriter* pWriter, int value) NN_NOEXCEPT
{
    int v;
    util::StoreLittleEndian(&v, value);
    pWriter->Write(&v, 0, sizeof(int));
}

inline void WriteValue(BufferWriter* pWriter, char value) NN_NOEXCEPT
{
    pWriter->Write(&value, 0, sizeof(char));
}

inline void WriteValue(BufferWriter* pWriter, int8_t value) NN_NOEXCEPT
{
    pWriter->Write(&value, 0, sizeof(int8_t));
}

inline void WriteValue(BufferWriter* pWriter, int16_t value) NN_NOEXCEPT
{
    int16_t v;
    util::StoreLittleEndian(&v, value);
    pWriter->Write(&v, 0, sizeof(int16_t));
}

inline void WriteValue(BufferWriter* pWriter, int64_t value) NN_NOEXCEPT
{
    int64_t v;
    util::StoreLittleEndian(&v, value);
    pWriter->Write(&v, 0, sizeof(int64_t));
}

inline void WriteValue(BufferWriter* pWriter, uint8_t value) NN_NOEXCEPT
{
    pWriter->Write(&value, 0, sizeof(uint8_t));
}

inline void WriteValue(BufferWriter* pWriter, uint16_t value) NN_NOEXCEPT
{
    uint16_t v;
    util::StoreLittleEndian(&v, value);
    pWriter->Write(&v, 0, sizeof(uint16_t));
}

inline void WriteValue(BufferWriter* pWriter, uint32_t value) NN_NOEXCEPT
{
    uint32_t v;
    util::StoreLittleEndian(&v, value);
    pWriter->Write(&v, 0, sizeof(uint32_t));
}

inline void WriteValue(BufferWriter* pWriter, uint64_t value) NN_NOEXCEPT
{
    uint64_t v;
    util::StoreLittleEndian(&v, value);
    pWriter->Write(&v, 0, sizeof(uint64_t));
}

inline void WriteValue(BufferWriter* pWriter, float value) NN_NOEXCEPT
{
    float v;
    util::StoreLittleEndian(&v, value);
    pWriter->Write(&v, 0, sizeof(float));
}

inline void WriteValue(BufferWriter* pWriter, double value) NN_NOEXCEPT
{
    double v;
    util::StoreLittleEndian(&v, value);
    pWriter->Write(&v, 0, sizeof(double));
}

inline void WriteValue(BufferWriter* pWriter, bool value) NN_NOEXCEPT
{
    int8_t v = value ? 1 : 0;
    WriteValue(pWriter, v);
}

inline void WriteValue(BufferWriter* pWriter, const util::string_view& string) NN_NOEXCEPT
{
    if (string.length() > 0)
    {
        pWriter->Write(string.data(), 0, string.length());
    }

    // null 終端文字を書き込む
    char v = 0;
    pWriter->Write(&v, 0, sizeof(char));
}

inline void WriteValue(BufferWriter* pWriter, const char* string) NN_NOEXCEPT
{
    WriteValue(pWriter, util::string_view(string));
}

inline void WriteValue(BufferWriter* pWriter, const BufferView& buffer) NN_NOEXCEPT
{
    if (const void* ptr = buffer.GetPtr())
    {
        pWriter->Write(ptr, 0, buffer.GetSize());
    }
}

}} // namespace nn::ige
