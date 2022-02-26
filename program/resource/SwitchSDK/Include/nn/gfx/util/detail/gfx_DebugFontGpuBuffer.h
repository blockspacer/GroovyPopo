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

#include <nn/gfx.h>
#include <nn/nn_SdkAssert.h>
#include <nn/nn_Allocator.h>

#if !defined(NN_BUILD_CONFIG_SPEC_CAFE)
#include <atomic>
#endif

namespace nn {
namespace gfx {
namespace util {
namespace detail {

//--------------------------------------------------------------------------------------------
//! @brief CPU でデータを書き込み、GPU が描画時にアクセスするバッファを管理するクラスです。
//--------------------------------------------------------------------------------------------
class GpuBuffer
{
    NN_DISALLOW_COPY(GpuBuffer);

public:
    //! @brief Initialize() の引数オブジェクトです。
    class InitializeArg
    {
    public:
        enum InitializeFlag
        {
            InitializeFlag_None = 0,
            InitializeFlag_AllocateSync = (1 << 0), //< GpuBuffer から Allocate で領域を確保する際に同期処理を行います。複数スレッドから同一の GpuBuffer を使用してメモリを確保する際に指定します。
        };
    public:
        //! @brief コンストラクタです。
        InitializeArg()
            : m_GpuAccessFlag(nn::gfx::GpuAccess_ConstantBuffer)
            , m_BufferSize(0)
            , m_BufferCount(1)
            , m_pMemoryPool(NULL)
            , m_MemoryPoolOffset(0)
            , m_pAllocateFunction(NULL)
            , m_pAllocateFunctionUserData(NULL)
            , m_AllocateSyncFlag(false)
        {}

        //! @brief このバッファがどのような用途のバッファとして使用されるかを指定します。
        //!
        //! @details
        //! この情報に基づいて nn::gfx::Buffer を作成します。
        //!
        //! @param[in] gpuAccess  バッファの種別を nn::gfx::GpuAccess で指定します。
        //!
        //! @return オブジェクト自身への参照を返します。
        //!
        InitializeArg& SetGpuAccessFlag(nn::gfx::GpuAccess gpuAccess)
        {
            m_GpuAccessFlag = gpuAccess;
            return *this;
        }

        //! @brief  使用するバッファのサイズを指定します。
        //!
        //! @details
        //! このメソッドに指定されたサイズのメモリが SetBufferCount で指定された数だけメモリプール上の領域を使用します。
        //!
        //! @param[in]  bufferSize  バッファのサイズをバイト単位で指定します。
        //!
        //! @return オブジェクト自身への参照を返します。
        //!
        //! @sa SetBufferCount
        //!
        InitializeArg& SetBufferSize(size_t bufferSize)
        {
            m_BufferSize = bufferSize;
            return *this;
        }

        //! @brief  バッファのマルチバッファリングの数をしてします。
        //!
        //! @details
        //! ここで設定した数だけ内部で nn::gfx::Buffer が作成され、切り替えて使用することが出来ます。
        //! たとえばダブルバッファリングしたい場合は 2 を指定、トリプルバッファリングしたい場合は 3 を指定します。
        //!
        //! @param[in] bufferCount  マルチバッファリングする数を指定します。
        //!
        //! @return オブジェクト自身への参照を返します。
        //!
        InitializeArg& SetBufferCount(int bufferCount)
        {
            m_BufferCount = bufferCount;
            return *this;
        }

        //! @brief  バッファを作成する際に使用するメモリプールを指定します。
        //!
        //! @param[in]  pMemoryPool メモリプールです。
        //!
        //! @return オブジェクト自身への参照を返します。
        //!
        InitializeArg& SetMemoryPool(nn::gfx::MemoryPool* pMemoryPool)
        {
            m_pMemoryPool = pMemoryPool;
            return *this;
        }

        //! @brief  メモリプール内で使用する領域へのオフセットを指定します。
        //!
        //! @details
        //! ひとつのメモリプールを複数のバッファで共有する場合、使用する領域が重複しないようにこの値を使用してオフセットを指定します。
        //! 指定した値がそのままオフセット値として使用されるためバッファごとのアライメント値にあわせて調整してください。
        //!
        //! @param[in]  memoryPoolOffset    メモリプール内のオフセットをバイト単位で指定します。
        //!
        //! @return オブジェクト自身への参照を返します。
        //!
        InitializeArg& SetMemoryPoolOffset(size_t memoryPoolOffset)
        {
            m_MemoryPoolOffset = memoryPoolOffset;
            return *this;
        }

        //! @brief  GpuBuffer の初期化時に使用されるメモリアロケータを設定します。
        //!
        //! @details
        //! GpuBuffer::Initialize 内でのみ使用されます。
        //!
        //! @param[in]  pAllocateFunction   アロケータ関数へのポインタです。
        //! @param[in]  pAllocateFunctionUserData   アロケータ関数へ渡すユーザーデータポインタです。
        //!
        //! @return オブジェクト自身への参照を返します。
        //!
        InitializeArg& SetAllocator(
            nn::AlignedAllocateFunctionWithUserData pAllocateFunction,
            void* pAllocateFunctionUserData)
        {
            m_pAllocateFunction = pAllocateFunction;
            m_pAllocateFunctionUserData = pAllocateFunctionUserData;
            return *this;
        }

        //! @brief  GpuBuffer から Allocate で領域を確保する際に同期処理を行うかどうかを指定します。
        //!
        //! @details
        //! Allocate 内で内部のメモリ確保状態を操作する際に同期処理が行われます。
        //!
        //! @param[in] allocateSyncFlag true を指定すると GpuBuffer::Allocate を同期処理付きで呼び出します。
        //!
        //! @return オブジェクト自身への参照を返します。
        //!
        InitializeArg& SetAllocateSyncFlag(bool allocateSyncFlag)
        {
            m_AllocateSyncFlag = allocateSyncFlag;
            return *this;
        }
    private:
        nn::gfx::GpuAccess  m_GpuAccessFlag;
        size_t              m_BufferSize;
        uint32_t            m_BufferCount;
        nn::gfx::MemoryPool*    m_pMemoryPool;
        size_t              m_MemoryPoolOffset;
        nn::AlignedAllocateFunctionWithUserData m_pAllocateFunction;
        void*               m_pAllocateFunctionUserData;
        bool                m_AllocateSyncFlag;

        friend class GpuBuffer;
    };
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    GpuBuffer()
    : m_Flags(0)
    , m_pBuffers(NULL)
    , m_Size(0)
    , m_Alignment(0)
    , m_Count(0)
    , m_MappedBufferIndex(-1)
    , m_GpuAccessBufferIndex(0)
    , m_pMappedPointer(NULL)
    {
    }

    //! @brief  デストラクタ
    //!
    ~GpuBuffer()
    {
    }

    //@}

    //! @brief  初期化処理です。
    //!
    //! @details
    //! メモリプールからメモリを確保しバッファを作成します。
    //!
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  arg     初期化パラメータです。
    //!
    //! @return バッファの作成成功可否です。
    //!
    bool Initialize(
        nn::gfx::Device* pDevice,
        const InitializeArg&  arg
        );

    //! @brief   この関数は非推奨です。初期化処理です。
    //! @deprecated  初期化時の引数が InitializeArg にまとめられた Initialize メソッドが用意されました。そちらの利用に切り替えてください。
    //!
    //! @details
    //! 本関数は互換性の目的で残されている関数です。渡されたメモリプールからメモリを確保しバッファを作成します。
    //! メモリプールからメモリを確保しバッファを作成します。
    //!
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  gpuAccess   Gpu からのアクセス方法です。
    //! @param[in]  bufferSize  作成するバッファのサイズです。
    //! @param[in]  bufferCount 作成するバッファのマルチバッファリングのカウントです。
    //! @param[in]  pMemoryPool 作成するバッファのメモリを確保するメモリプールです。
    //! @param[in]  memoryPoolOffset    メモリを確保する領域のメモリプール先頭からのオフセットです。
    //! @param[in]  pAllocateFunction   内部データ構造のメモリを確保するための関数へのポインタです。
    //! @param[in]  pAllocateFunctionUserData   pAllocateFunction へ渡すユーザーデータへのポインタです。
    //!
    //! @return バッファの作成成功可否です。
    //!
    NN_DEPRECATED bool Initialize(
        nn::gfx::Device* pDevice,
        nn::gfx::GpuAccess gpuAccess,
        size_t bufferSize,
        int bufferCount,
        nn::gfx::MemoryPool* pMemoryPool,
        size_t memoryPoolOffset,
        nn::AlignedAllocateFunctionWithUserData pAllocateFunction,
        void* pAllocateFunctionUserData
        );

    //! @brief  終了処理です。
    //!
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  pFreeFunction   Initialize で確保した内部データ構造のメモリを開放するための関数へのポインタです。
    //! @param[in]  pFreeFunctionUserData   pFreeFunction へ渡すユーザーデータへのポインタです。
    //!
    void Finalize(
        nn::gfx::Device* pDevice,
        nn::FreeFunctionWithUserData pFreeFunction,
        void* pFreeFunctionUserData);

    //! @brief 初期化済みかどうかフラグを取得します。
    //!
    //! @return 初期化済みならば true が返ります。
    bool IsInitialized()
    {
        return m_pBuffers != NULL;
    }

    //! @brief バッファを CPU で更新するための事前処理を行います。
    //!
    //! @details
    //! バッファの値を CPU で書き換える前に呼び出してください。
    //!
    //! @param[in]  bufferIndex 事前準備を行うバッファのインデックスを指定します。
    //!
    void Map(int bufferIndex);

    //! @brief バッファを CPU で書き換えた後の終了処理を行います。
    //!
    //! @details
    //! バッファの値を CPU で書き換えた後に呼び出してください。
    //!
    void Unmap();

    // バッファを Map したポインタを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details
    //! 使用する前に BeginUpdateBuffer を呼び出してください。
    //!
    //! @return バッファを Map したポインタです。
    //!
    //! @sa Map
    //!
    void* GetMappedPointer()
    {
        NN_SDK_ASSERT_NOT_NULL(m_pMappedPointer);

        return m_pMappedPointer;
    }

    // バッファに指定されたサイズ分の領域を確保します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details
    //! 実際に書き込む際のポインタや GPU に渡す GpuAddress は
    //! GetMappedPointer や GetGpuAddress へこのメソッドで取得したオフセット値を加算して計算します。
    //!
    //! @return 確保された領域のバッファ先頭からのオフセット。
    //!
    //! @sa GetMappedPointer
    //! @sa GetGpuAddress
    //!
    size_t Allocate(size_t alignedSize)
    {
        size_t  offset = 0;

        if (m_Flags & Flags_AllocateWithSync)
        {
#if !defined(NN_BUILD_CONFIG_SPEC_CAFE)
            offset = m_BufferAllocatedSize.m_pSync->fetch_add(alignedSize);
#endif
            NN_SDK_ASSERT(*(m_BufferAllocatedSize.m_pSync) <= m_Size);
        }
        else
        {
            offset = m_BufferAllocatedSize.m_Raw;
            m_BufferAllocatedSize.m_Raw += alignedSize;
            NN_SDK_ASSERT(m_BufferAllocatedSize.m_Raw <= m_Size);
        }

        return offset;
    }

    //----------------------------------------
    //! @name 設定/取得
    //@{

    //! @brief バッファのサイズを取得します。
    //!
    //! @return バッファのサイズです。
    //!
    const size_t GetBufferSize() const
    {
        return m_Size;
    }

    //! @brief バッファの数を取得します。
    //!
    //! @return バッファの数です。
    //!
    const int GetBufferCount() const
    {
        return m_Count;
    }

    //! @brief バッファのアライメントを取得します。
    //!
    //! @return バッファのアライメント値です。
    //!
    size_t GetBufferAlignment() const
    {
        return m_Alignment;
    }

    //! @brief GPU がアクセスするバッファのインデックスを設定します。
    //!
    //! @param[in]  bufferIndex GPU が参照するバッファのインデックスを指定します。
    //!
    void SetGpuAccessBufferIndex(int bufferIndex)
    {
        m_GpuAccessBufferIndex = bufferIndex;
    }

    //! @brief AllocateMemoryFromBuffer で確保済みのメモリのサイズを取得します。
    //!
    //! @details
    //! Begin/End 間でのみ有効な値を返します。
    //!
    //! @return 確保済みの領域のサイズです。
    //!
    //! @sa AllocateMemoryFromBuffer
    //!
    size_t GetAllocatedSize()
    {
        if (m_Flags & Flags_AllocateWithSync)
        {
            return *(m_BufferAllocatedSize.m_pSync);
        }
        else
        {
            return m_BufferAllocatedSize.m_Raw;
        }
    }

    // バッファの GpuAddr を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return バッファから GetGpuAddress をした結果。
    //!
    void GetGpuAddress(nn::gfx::GpuAddress* pGpuAddress) const
    {
        NN_SDK_ASSERT_NOT_NULL(m_pBuffers);

        m_pBuffers[m_GpuAccessBufferIndex].GetGpuAddress(pGpuAddress);
    }

    //@}
private:
    enum Flags
    {
        Flags_AllocateWithSync = 1 << 0
    };
private:
    //  状態フラグ
    uint32_t            m_Flags;

    // Gpu がアクセスするバッファ
    nn::gfx::Buffer*    m_pBuffers;
    // バッファのサイズ
    size_t              m_Size;
    // バッファのアライメント
    size_t              m_Alignment;
    // バッファのバッファリングカウント
    uint32_t            m_Count;
    //  マップしたバッファのインデックス
    int32_t m_MappedBufferIndex;
    //  GPU 参照時にアクセスするバッファのインデックス
    int32_t m_GpuAccessBufferIndex;
    //  マップした際のポインタ
    void*   m_pMappedPointer;
    //  マップ後に確保された領域のサイズ
    union
    {
        size_t                  m_Raw;      // 同期処理不要の時に使用される変数
#if !defined(NN_BUILD_CONFIG_SPEC_CAFE)
        std::atomic<size_t>*    m_pSync;    // 同期処理が必要な時に使用される変数
#else
        size_t*                 m_pSync;    // Cafe 環境では std::atomic が使用できないため size_t 扱いとする。実際は Initialize のアサートでとまります。
#endif
    }m_BufferAllocatedSize;
};

} // namespace detail
} // nemespace util
} // namespace gfx
} // nemespace nn
