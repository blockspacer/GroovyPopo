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

#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//! @internal
typedef CriticalSection LockObject;

//---------------------------------------------------------
//! @internal
struct SimpleRingBufferRange
{
    uintptr_t                     start;
    uintptr_t                     end;

    SimpleRingBufferRange() NN_NOEXCEPT
    {
        start = 0;
        end   = 0;
    }
};

//---------------------------------------------------------
//! @internal
struct SimpleRingBufferHeader
{
    size_t                     bufferSize;
    SimpleRingBufferRange   range;
    LockObject              lockObject;

    SimpleRingBufferHeader() NN_NOEXCEPT
    {
        bufferSize  = 0;
    }
};

//---------------------------------------------------------------------------
//! @internal
class SimpleRingBuffer
{
public:
    //---------------------------------------------------------------------------
    //! @brief        デフォルトコンストラクタです。
    //---------------------------------------------------------------------------
                            SimpleRingBuffer() NN_NOEXCEPT {}

    //---------------------------------------------------------------------------
    //! @brief        インスタンスを初期化します。
    //---------------------------------------------------------------------------
    void                    Init(
                                void*   buf,
                                size_t     size) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        現在バッファにあるデータを破棄します。
    //---------------------------------------------------------------------------
    void                    Discard() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        読み込み可能なデータサイズを取得します。
    //---------------------------------------------------------------------------
    size_t                     GetReadableBytes() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        書き込み可能なデータサイズを取得します。
    //---------------------------------------------------------------------------
    size_t                     GetWritableBytes() NN_NOEXCEPT;

    const SimpleRingBufferHeader& GetHeader() NN_NOEXCEPT { return *m_pHeader; }

    //---------------------------------------------------------------------------
    //! @brief        リングバッファから読み出します。
    //---------------------------------------------------------------------------
    size_t                     Read(
                                void*       buf,
                                size_t         size) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        リングバッファへ書き込みます。
    //---------------------------------------------------------------------------
    size_t                     Write(
                                const void*     buf,
                                size_t             size) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        リングバッファの内容をスキップします。
    //---------------------------------------------------------------------------
    size_t                     Skip(
                                size_t             size) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        リングバッファの内容を読み出します。ポインタは変更しません。
    //---------------------------------------------------------------------------
    size_t                     Peek(
                                void*       buf,
                                size_t             size) NN_NOEXCEPT;

private:
    void                    BeginRead(SimpleRingBufferRange* pRange) NN_NOEXCEPT;

    size_t                     ContinueRead(
                                SimpleRingBufferRange*    pRange,
                                void*                     buf,
                                size_t                       size) NN_NOEXCEPT;

    void                    EndRead(const SimpleRingBufferRange& range) NN_NOEXCEPT;

    void                    BeginWrite(SimpleRingBufferRange* pRange) NN_NOEXCEPT;

    size_t                     ContinueWrite(
                                SimpleRingBufferRange*    pRange,
                                const void*               buf,
                                size_t                       size) NN_NOEXCEPT;

    void                    EndWrite(const SimpleRingBufferRange& range) NN_NOEXCEPT;

    void                    BeginSkip(SimpleRingBufferRange* pRange) NN_NOEXCEPT;

    size_t                     ContinueSkip(
                                SimpleRingBufferRange*    pRange,
                                size_t                       size) NN_NOEXCEPT;

    void                    EndSkip(const SimpleRingBufferRange& range) NN_NOEXCEPT;

private:
    SimpleRingBufferHeader*     m_pHeader;
};

}   // namespace fnd
}   // namespace detail
}   // namespace atk
}   // namespace nn
