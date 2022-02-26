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
 * :include nn/atk/atk_SoundMemoryAllocatable.h
 *
 * @file atk_SoundMemoryAllocatable.h
 */

#pragma once

#include <cstddef>
#include <nn/nn_Macro.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @brief    サウンドデータを配置するためのメモリを確保するためのインターフェイスです。
//!
//!           @ref SoundHeap クラスがこのクラスを継承しています。
//!
//! @see      SoundHeap クラス
//!
//---------------------------------------------------------------------------
class SoundMemoryAllocatable
{
public:
    //---------------------------------------------------------------------------
    //! @brief    メモリブロックが破棄された時に呼び出されるコールバックです。
    //!
    //!           コールバック関数は @ref Allocate の引数としてメモリブロックに渡し、
    //!           メモリブロックが解放されたときに呼びだされます。
    //!
    //! @param[in]    arg     ユーザ引数です。
    //!
    //! @see Allocate
    //---------------------------------------------------------------------------
    typedef void (*DisposeCallback)( void* arg );

    //----------------------------------------
    //! @name デストラクタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    デストラクタです。
    //!
    //---------------------------------------------------------------------------
    virtual ~SoundMemoryAllocatable() NN_NOEXCEPT {}
    //@}

    //----------------------------------------
    //! @name ヒープ操作
    //@{
    //---------------------------------------------------------------------------
    //! @brief    メモリ上のヒープからメモリ領域を割り当てます。
    //!
    //!           確保するメモリの先頭アドレスは
    //!           32 バイトアライメントされている必要があります。
    //!
    //! @param[in]    size    確保するメモリサイズです。
    //!
    //! @return   確保したメモリブロックの先頭アドレスを返します。
    //!           確保に失敗した場合は nullptr を返します。
    //!
    //---------------------------------------------------------------------------
    virtual void* Allocate( size_t size ) NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief        メモリ上のヒープからメモリ領域を割り当てます。
    //!
    //!               コールバック関数 callback は、
    //!               メモリブロックが解放されたときに呼び出されます。
    //!
    //! @param[in]    size            確保するメモリサイズです。
    //! @param[in]    callback        メモリブロックが解放されるときに呼び出される
    //!                               コールバック関数です。
    //! @param[in]    callbackArg     コールバック関数の引数です。
    //!
    //! @return       確保したメモリブロックの先頭アドレスを返します。
    //!               確保に失敗した場合は nullptr を返します。
    //!
    //---------------------------------------------------------------------------
    virtual void* Allocate(
        size_t size,
        DisposeCallback callback,
        void* callbackArg
    ) NN_NOEXCEPT = 0;

    //! @briefprivate
    //! @param[in] size :private
    //! @param[in] needMemoryPool :private
    //! @return :private
    virtual size_t GetAllocateSize(size_t size, bool needMemoryPool) NN_NOEXCEPT = 0;

    //@}
};

} // namespace nn::atk
} // namespace nn

