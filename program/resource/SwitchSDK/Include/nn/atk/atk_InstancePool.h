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

#include <new>
#include <nn/nn_Macro.h>

namespace nn {
namespace atk {
namespace detail {

/* ========================================================================
        PoolImpl class
   ======================================================================== */

class PoolImpl
{
public:
    static const int DefaultAlignment = 4;
public:
    PoolImpl() NN_NOEXCEPT : m_pNext(NULL), m_pBuffer(NULL), m_BufferSize(0) {}
protected:
    int CreateImpl( void* buffer, size_t size, size_t objSize ) NN_NOEXCEPT;
    int CreateImpl( void* buffer, size_t size, size_t objSize, size_t alignment ) NN_NOEXCEPT;
    void DestroyImpl() NN_NOEXCEPT;
    int CountImpl() const NN_NOEXCEPT;
    void* AllocImpl() NN_NOEXCEPT;
    void FreeImpl( void* ptr ) NN_NOEXCEPT;

private:
    PoolImpl* m_pNext;
    void* m_pBuffer;
    size_t m_BufferSize;
};

class BufferPool : private detail::PoolImpl
{
public:
    static const int DefaultAlignment = detail::PoolImpl::DefaultAlignment;

public:
    /*--------------------------------------------------------------------------------*
      Name:         Create

      Description:  指定したメモリ領域をプールに割り当てます

      Arguments:    buffer - メモリ領域の開始アドレス
                    size   - メモリ領域のサイズ
                    bufferSizePerAlloc - 1 回の alloc で確保するバッファのサイズ

      Returns:      作成したプール中のインスタンス数
     *--------------------------------------------------------------------------------*/
    int Create( void* buffer, size_t size, size_t bufferSizePerAlloc ) NN_NOEXCEPT
    {
        size_t objSize =
            ( bufferSizePerAlloc > sizeof( BufferPool* ) ) ?
            bufferSizePerAlloc:
            sizeof( BufferPool* )
            ;
        return CreateImpl( buffer, size, objSize );
    }

    /*--------------------------------------------------------------------------------*
      Name:         Create

      Description:  指定したメモリ領域をプールに割り当てます

      Arguments:    buffer    - メモリ領域の開始アドレス
                    size      - メモリ領域のサイズ
                    bufferSizePerAlloc - 1 回の alloc で確保するバッファのサイズ
                    alignment - プールと各インスタンスのアライメント

      Returns:      作成したプール中のインスタンス数
     *--------------------------------------------------------------------------------*/
    int Create( void* buffer, size_t size, size_t bufferSizePerAlloc, size_t alignment ) NN_NOEXCEPT
    {
        size_t objSize =
            ( bufferSizePerAlloc > sizeof( BufferPool* ) ) ? bufferSizePerAlloc : sizeof( BufferPool* );
        return CreateImpl( buffer, size, objSize, alignment );
    }

    /*--------------------------------------------------------------------------------*
      Name:         Destroy

      Description:  指定したメモリ領域をプールから解放します
                    解放するためにはメモリ領域が Free されてプールに格納されている
                    必要があります。

      Arguments:    buffer - メモリ領域の開始アドレス
                    size   - メモリ領域のサイズ

      Returns:      無し
     *--------------------------------------------------------------------------------*/
    void Destroy() NN_NOEXCEPT { DestroyImpl(); }

    /*--------------------------------------------------------------------------------*
      Name:         Count

      Description:  プール中のインスタンス数を取得します。

      Arguments:    無し

      Returns:      インスタンス数
     *--------------------------------------------------------------------------------*/
    int Count() const NN_NOEXCEPT { return CountImpl(); }

    /*--------------------------------------------------------------------------------*
      Name:         Alloc

      Description:  バッファを確保する。

      Arguments:    無し

      Returns:      確保したバッファ、確保できなければNULL
     *--------------------------------------------------------------------------------*/
    void* Alloc() NN_NOEXCEPT
    {
        void *ptr = AllocImpl();
        return ptr;
    }

    /*--------------------------------------------------------------------------------*
      Name:         Free

      Description:  バッファをプールに解放する

      Arguments:    obj - バッファ

      Returns:      無し
     *--------------------------------------------------------------------------------*/
    void Free( void* ptr ) NN_NOEXCEPT
    {
        if ( ptr == NULL ) return;
        FreeImpl( ptr );
    }
};

template< typename T >
class MemoryPool : private detail::PoolImpl
{
public:
    /*--------------------------------------------------------------------------------*
      Name:         Create

      Description:  指定したメモリ領域をプールに割り当てます

      Arguments:    buffer - メモリ領域の開始アドレス
                    size   - メモリ領域のサイズ

      Returns:      作成したプール中のインスタンス数
     *--------------------------------------------------------------------------------*/
    int Create( void* buffer, size_t size ) NN_NOEXCEPT
    {
        size_t objSize =
            ( sizeof( T ) > sizeof( MemoryPool<T>* ) ) ?
            sizeof( T ):
            sizeof( MemoryPool<T>* )
            ;
        return CreateImpl( buffer, size, objSize );
    }

    /*--------------------------------------------------------------------------------*
      Name:         Create

      Description:  指定したメモリ領域をプールに割り当てます

      Arguments:    buffer    - メモリ領域の開始アドレス
                    size      - メモリ領域のサイズ
                    alignment - プールと各インスタンスのアライメント

      Returns:      作成したプール中のインスタンス数
     *--------------------------------------------------------------------------------*/
    int Create( void* buffer, size_t size, size_t alignment ) NN_NOEXCEPT
    {
        size_t objSize =
            ( sizeof( T ) > sizeof( MemoryPool<T>* ) ) ? sizeof( T ) : sizeof( MemoryPool<T>* );
        return CreateImpl( buffer, size, objSize, alignment );
    }

    /*--------------------------------------------------------------------------------*
      Name:         Destroy

      Description:  指定したメモリ領域をプールから解放します
                    解放するためにはメモリ領域が Free されてプールに格納されている
                    必要があります。

      Arguments:    buffer - メモリ領域の開始アドレス
                    size   - メモリ領域のサイズ

      Returns:      無し
     *--------------------------------------------------------------------------------*/
    void Destroy() NN_NOEXCEPT { DestroyImpl(); }

    /*--------------------------------------------------------------------------------*
      Name:         Count

      Description:  プール中のインスタンス数を取得します。

      Arguments:    無し

      Returns:      インスタンス数
     *--------------------------------------------------------------------------------*/
    int Count() const NN_NOEXCEPT { return CountImpl(); }

    /*--------------------------------------------------------------------------------*
      Name:         Alloc

      Description:  インスタンスを確保する。

      Arguments:    無し

      Returns:      確保したインスタンス、確保できなければNULL
     *--------------------------------------------------------------------------------*/
    T* Alloc() NN_NOEXCEPT { return static_cast<T*>( AllocImpl() ); }

    /*--------------------------------------------------------------------------------*
      Name:         Free

      Description:  インスタンスをプールに解放する

      Arguments:    obj - インスタンス

      Returns:      無し
     *--------------------------------------------------------------------------------*/
    void Free( T* obj ) NN_NOEXCEPT { FreeImpl( obj ); }
};


/* ========================================================================
        InstancePool class
   ======================================================================== */

template< typename T >
class InstancePool : private detail::PoolImpl
{
public:
    static const int DefaultAlignment = detail::PoolImpl::DefaultAlignment;

public:
    /*--------------------------------------------------------------------------------*
      Name:         Create

      Description:  指定したメモリ領域をプールに割り当てます

      Arguments:    buffer - メモリ領域の開始アドレス
                    size   - メモリ領域のサイズ

      Returns:      作成したプール中のインスタンス数
     *--------------------------------------------------------------------------------*/
    int Create( void* buffer, size_t size ) NN_NOEXCEPT
    {
        size_t objSize =
            ( sizeof( T ) > sizeof( InstancePool<T>* ) ) ?
            sizeof( T ):
            sizeof( InstancePool<T>* )
            ;
        return CreateImpl( buffer, size, objSize );
    }

    /*--------------------------------------------------------------------------------*
      Name:         Create

      Description:  指定したメモリ領域をプールに割り当てます

      Arguments:    buffer    - メモリ領域の開始アドレス
                    size      - メモリ領域のサイズ
                    alignment - プールと各インスタンスのアライメント

      Returns:      作成したプール中のインスタンス数
     *--------------------------------------------------------------------------------*/
    int Create( void* buffer, size_t size, size_t alignment ) NN_NOEXCEPT
    {
        size_t objSize =
            ( sizeof( T ) > sizeof( InstancePool<T>* ) ) ? sizeof( T ) : sizeof( InstancePool<T>* );
        return CreateImpl( buffer, size, objSize, alignment );
    }

    /*--------------------------------------------------------------------------------*
      Name:         Destroy

      Description:  指定したメモリ領域をプールから解放します
                    解放するためにはメモリ領域が Free されてプールに格納されている
                    必要があります。

      Arguments:    buffer - メモリ領域の開始アドレス
                    size   - メモリ領域のサイズ

      Returns:      無し
     *--------------------------------------------------------------------------------*/
    void Destroy() NN_NOEXCEPT { DestroyImpl(); }

    /*--------------------------------------------------------------------------------*
      Name:         Count

      Description:  プール中のインスタンス数を取得します。

      Arguments:    無し

      Returns:      インスタンス数
     *--------------------------------------------------------------------------------*/
    int Count() const NN_NOEXCEPT { return CountImpl(); }

    /*--------------------------------------------------------------------------------*
      Name:         Alloc

      Description:  インスタンスを確保する。

      Arguments:    無し

      Returns:      確保したインスタンス、確保できなければNULL
     *--------------------------------------------------------------------------------*/
    T* Alloc() NN_NOEXCEPT
    {
        void *ptr = AllocImpl();
        if ( ptr == NULL ) return NULL;
        return new( ptr ) T(); // コンストラクタ呼びだし
    }

    /*--------------------------------------------------------------------------------*
      Name:         Free

      Description:  インスタンスをプールに解放する

      Arguments:    obj - インスタンス

      Returns:      無し
     *--------------------------------------------------------------------------------*/
    void Free( T* obj ) NN_NOEXCEPT
    {
        if ( obj == NULL ) return;
        obj->~T(); // デストラクタ呼びだし
        FreeImpl( obj );
    }
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

