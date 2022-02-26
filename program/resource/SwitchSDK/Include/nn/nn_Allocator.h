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
*   @file
*   @brief  メモリ確保、メモリ解放関数のインタフェース
*/

#pragma once

#include <nn/nn_Common.h>
#include <cstddef>
#include <nn/TargetConfigs/build_Compiler.h>

namespace nn {

/**
* @brief メモリ確保関数のインタフェースです。例外を発生させてはいけません。
*
* @param[in] size           確保するメモリのバイトサイズ
*
* @return                   確保できたメモリの先頭アドレスを返します。@n
*                           確保できなかった場合は NULL を返します。
*/
typedef void* (*AllocateFunction)(size_t size);

/**
* @brief メモリ確保関数のインタフェースです。例外を発生させてはいけません。
*
* @param[in] size           確保するメモリのバイトサイズ
* @param[in] alignment      確保するメモリのアライメント
*
* @return                   確保できたメモリの先頭アドレスを返します。@n
*                           確保できなかった場合は NULL を返します。
*/
typedef void* (*AlignedAllocateFunction)(size_t size, size_t alignment);


/**
* @brief メモリ確保関数のインタフェースです。例外を発生させてはいけません。
*
* @param[in] size           確保するメモリのバイトサイズ
* @param[in] pUserData      ユーザデータへのポインタ
*
* @return                   確保できたメモリの先頭アドレスを返します。@n
*                           確保できなかった場合は NULL を返します。
*/
typedef void* (*AllocateFunctionWithUserData)(size_t size, void* pUserData);

/**
* @brief メモリ確保関数のインタフェースです。例外を発生させてはいけません。
*
* @param[in] size           確保するメモリのバイトサイズ
* @param[in] alignment      確保するメモリのアライメント
* @param[in] pUserData      ユーザデータへのポインタ
*
* @return                   確保できたメモリの先頭アドレスを返します。@n
*                           確保できなかった場合は NULL を返します。
*/
typedef void* (*AlignedAllocateFunctionWithUserData)(size_t size, size_t alignment, void* pUserData);

/**
* @brief メモリ解放関数のインタフェースです。例外を発生させてはいけません。
*
* @param[in] ptr            解放するメモリの先頭アドレス
* @param[in] size           解放するメモリのサイズ
*/
typedef void(*DeallocateFunction)(void* ptr, size_t size);

/**
* @brief メモリ解放関数のインタフェースです。例外を発生させてはいけません。
*
* @param[in] ptr            解放するメモリの先頭アドレス
*/
typedef void(*FreeFunction)(void* ptr);

/**
* @brief メモリ解放関数のインタフェースです。例外を発生させてはいけません。
*
* @param[in] ptr            解放するメモリの先頭アドレス
* @param[in] pUserData      ユーザデータへのポインタ
*/
typedef void(*FreeFunctionWithUserData)(void* ptr, void* pUserData);

#if defined(NN_BUILD_CONFIG_COMPILER_GHS)
const std::size_t DefaultAlignment = 8;
#else
/**
    @brief デフォルトアラインサイズを表す定数です。
*/
const std::size_t DefaultAlignment = NN_ALIGNOF(std::max_align_t);
#endif

/**
    @brief N4529 と同等のインターフェイスを持つ多相メモリアロケータインターフェイスです。

    @details
     N4529(http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4529.html#memory.resource) と同等のインターフェイスを持つ多相メモリアロケータインターフェイスです。

     is_equal() によって MemoryResource 等値比較が可能です。
     二つの MemoryResource a と b とがあり、a で確保したメモリ領域を b で正しく解放することができ、
     また、その逆も同様であるとき、
     a.is_equal(b) は true を返すことができます。
     a.is_equal(b) のとき、a と b は等値であると言います。

     このクラスの具象クラスでは do_allocate(), do_deallocate(), do_is_equal() をオーバライドする必要があります。
*/
class MemoryResource
{
public:

    /**
        @brief オブジェクトを破棄します。
    */
    virtual ~MemoryResource() NN_NOEXCEPT {}

    /**
        @brief 指定したサイズとアライメントでメモリ領域を確保します。

        @param[in] bytes 確保したいメモリ領域のサイズをバイト単位で指定します。
        @param[in] alignment 確保するメモリ領域に必要なアラインメントを指定します。
        @return 確保したメモリ領域へのポインタを返します。

        @pre alignment は 2 の累乗である。

        @details
         return this->do_allocate(bytes, alignment) と同等です。

        @see do_allocate()
    */
    void* allocate(std::size_t bytes, std::size_t alignment = DefaultAlignment) NN_NOEXCEPT;

    /**
        @brief 指定したサイズとアライメントで確保したメモリを解放します。

        @param[in] p 解放するメモリ領域へのポインタを指定します。
        @param[in] bytes p を確保する際に使用した allocate() での bytes 引数と同じ値を指定します。
        @param[in] alignment p を確保する際に使用した allocate() での alignment 引数と同じ値を指定します。

        @pre p は *this と等値な MemoryResource に対して allocate(bytes, alignment) で返された値である。
        @pre p はまだ解放されていない。

        @details
         this->do_deallocate(p, bytes, alignment) と同等です。

        @see do_deallocate()
    */
    void deallocate(void* p, std::size_t bytes, std::size_t alignment = DefaultAlignment) NN_NOEXCEPT;

    /**
        @brief 対象のアロケータが等値であるかどうかを判定します。

        @param[in] other 比較対象の MemoryResource を指定します。

        @return *this と other が等値であれば true を返し、そうでなければ false を返します。

        @details
         this->do_is_equal(other) と同等です。

        @see do_is_equal()
    */
    bool is_equal(const MemoryResource& other) const NN_NOEXCEPT;

protected:

    /**
        @brief allocate() の実装用仮想関数です。

        @param[in] bytes 確保したいメモリ領域のサイズをバイト単位で指定します。
        @param[in] alignment 確保するメモリ領域に必要なアラインメントを指定します。
        @return 確保したメモリ領域へのポインタを返します。

        @pre alignment は 2 の累乗である。
    */
    virtual void* do_allocate(std::size_t bytes, std::size_t alignment) NN_NOEXCEPT = 0;

    /**
        @brief deallocate() の実装用仮想関数です。

        @param[in] p 解放するメモリ領域へのポインタを指定します。
        @param[in] bytes p を確保する際に使用した do_allocate() での bytes 引数と同じ値を指定します。
        @param[in] alignment p を確保する際に使用した do_allocate() での alignment 引数と同じ値を指定します。

        @pre p が、*this と等値な MemoryResource に対して allocate(bytes, alignment) で返された値である。
        @pre p はまだ解放されていない。
    */
    virtual void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) NN_NOEXCEPT = 0;

    /**
        @brief is_equal の実装用関数です。

        @param[in] other 比較対象の MemoryResource を指定します。

        @return *this と other が等値であれば true を返し、そうでなければ false を返します。
    */
    virtual bool do_is_equal(const MemoryResource& other) const NN_NOEXCEPT = 0;

};

inline void* MemoryResource::allocate(std::size_t bytes, std::size_t alignment) NN_NOEXCEPT
{
    return this->do_allocate(bytes, alignment);
}

inline void MemoryResource::deallocate(void* p, std::size_t bytes, std::size_t alignment) NN_NOEXCEPT
{
    return this->do_deallocate(p, bytes, alignment);
}

inline bool MemoryResource::is_equal(const MemoryResource& other) const NN_NOEXCEPT
{
    return this->do_is_equal(other);
}

}
