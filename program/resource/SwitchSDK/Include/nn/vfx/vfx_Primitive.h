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
#include <nn/vfx/vfx_Data.h>
#include <nn/vfx/vfx_Enum.h>
#include <nn/vfx/vfx_Misc.h>
#include <nn/vfx/vfx_VertexBuffer.h>
#include <nn/vfx/vfx_MemUtil.h>
#include <nn/vfx/vfx_Binary.h>

#include <nn/g3d.h>

namespace nn {
namespace vfx {


//---------------------------------------------------
//! @brief プリミティブクラス
//---------------------------------------------------
class IPrimitive
{
    NN_DISALLOW_COPY( IPrimitive );

public:
    //---------------------------------------------------------------------------
    //! @brief アトリビュートが有効かどうかのビットフラグ
    //---------------------------------------------------------------------------
    enum PrimitiveDataElementFlag
    {
        PrimitiveDataElementFlag_Position           = 0x01 << 0,    //!< 位置
        PrimitiveDataElementFlag_Normal             = 0x01 << 1,    //!< 法線
        PrimitiveDataElementFlag_Tangent            = 0x01 << 2,    //!< 接線
        PrimitiveDataElementFlag_VertexColor        = 0x01 << 3,    //!< 頂点カラー0
        PrimitiveDataElementFlag_TextureCoordinate  = 0x01 << 4,    //!< テクスチャ座標
    };

    //---------------------------------------------------------------------------
    //! @brief アトリビュートが有効かどうかのビットフラグ
    //---------------------------------------------------------------------------
    enum PrimitiveDataElementIndex
    {
        PrimitiveDataElement_Position           = 0,    //!< 位置
        PrimitiveDataElement_Normal             = 1,    //!< 法線
        PrimitiveDataElement_Tangent            = 2,    //!< 接線
        PrimitiveDataElement_VertexColor        = 3,    //!< 頂点カラー0
        PrimitiveDataElement_TextureCoordinate  = 4,    //!< テクスチャ座標
        PrimitiveDataElement_Max                = 5,    //!<
    };

    //---------------------------------------------------------------------------
    //! @brief Primitive最大エレメント数
    //---------------------------------------------------------------------------
    enum
    {
        PrimitiveDataElementMaxCout = 5,                //!< 最大エレメント数
    };

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //---------------------------------------------------------------------------
    IPrimitive() NN_NOEXCEPT
    {
        m_IsInitialized = false;
    }

    //---------------------------------------------------------------------------
    //! @brief  頂点バッファ配列数を取得。
    //! @return 配列数
    //---------------------------------------------------------------------------
    virtual int GetVertexBufferArrayCount() const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief  インデックス数を取得。
    //! @return インデックス数
    //---------------------------------------------------------------------------
    virtual int GetIndexCount() const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief  無効化
    //---------------------------------------------------------------------------
    void Invalidate() NN_NOEXCEPT
    {
        m_IsInitialized = false;
    }

    //---------------------------------------------------------------------------
    //! @brief  初期化済みかどうか。
    //! @return trueで初期化済み、falseで未初期化
    //---------------------------------------------------------------------------
    bool IsValidate() const NN_NOEXCEPT
    {
        return m_IsInitialized;
    }

    //---------------------------------------------------------------------------
    //! @brief  ストライドを取得します。
    //! @param[in] bufferIndex ストライドを取得するバッファインデックス値
    //! @return ストライド値
    //---------------------------------------------------------------------------
    virtual ptrdiff_t GetStrideSize( int bufferIndex ) const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief  頂点バッファサイズを取得します。
    //! @param[in] bufferIndex バッファサイズを取得するバッファインデックス値
    //! @return 頂点バッファサイズ
    //---------------------------------------------------------------------------
    virtual size_t GetVertexBufferSize( int bufferIndex  ) const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief  頂点バッファのGpuアドレスを取得します。
    //! @param[out] pOutAddress Gpuアドレス書き込み先
    //! @param[in] bufferIndex Gpuアドレスを取得するバッファインデックス値
    //! @return Gpuアドレス
    //---------------------------------------------------------------------------
    virtual void GetVertexBufferGpuAddress( nn::gfx::GpuAddress* pOutAddress, int bufferIndex ) const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief  インデックスバッファのGpuアドレスを取得します。
    //! @return Gpuアドレス
    //---------------------------------------------------------------------------
    virtual const nn::gfx::GpuAddress& GetIndexBufferGpuAddress() const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief  バッファ数を取得します。
    //! @return バッファ数
    //---------------------------------------------------------------------------
    virtual int GetBufferCount() const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief  各アトリビュートのバッファインデックスを取得します。
    //! @param[in] index        アトリビュートのフォーマットを取得する要素インデックス
    //! @return アトリビュートバッファインデックス
    //---------------------------------------------------------------------------
    virtual int GetBufferIndex( PrimitiveDataElementIndex index ) const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief  各アトリビュートのフォーマットを取得します。
    //! @param[in] index      頂点フォーマットを取得する要素インデックス
    //! @return アトリビュートフォーマット
    //---------------------------------------------------------------------------
    virtual nn::gfx::AttributeFormat GetFormat( PrimitiveDataElementIndex index ) const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief  インデックスのフォーマットを取得します。
    //! @return インデックスのフォーマット
    //---------------------------------------------------------------------------
    virtual nn::gfx::IndexFormat GetIndexFormat() const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief  各アトリビュートのインタリーブ内オフセット値を取得します。
    //! @param[in] index        オフセットを取得する要素インデックス
    //! @return                 オフセット値
    //---------------------------------------------------------------------------
    virtual ptrdiff_t GetOffset( PrimitiveDataElementIndex index ) const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief  描画コマンドを発行します。
    //! @param[in] pCommandBuffer  コマンドバッファ
    //! @param[in] instance        インスタンス数
    //! @param[in] lodLevel        描画に利用する Lod レベル
    //---------------------------------------------------------------------------
    virtual void Draw( nn::gfx::CommandBuffer* pCommandBuffer, int instance, int lodLevel ) const NN_NOEXCEPT = 0;

    //---------------------------------------------------------------------------
    //! @brief  描画コマンドを発行します。
    //! @param[in] pCommandBuffer  コマンドバッファ
    //! @param[in] instance        インスタンス インデックス
    //! @param[in] baseInstance    開始インスタンス インデックス
    //! @param[in] lodLevel        描画に利用する Lod レベル
    //---------------------------------------------------------------------------
    virtual void Draw( nn::gfx::CommandBuffer* pCommandBuffer, int instance, int baseInstance, int lodLevel ) const NN_NOEXCEPT
    {
        NN_UNUSED( pCommandBuffer );
        NN_UNUSED( instance );
        NN_UNUSED( baseInstance );
        NN_UNUSED( lodLevel );
    }

protected:
    bool                            m_IsInitialized;        //!< 初期化済みかどうか
};


//---------------------------------------------------
//! @brief プリミティブクラス
//---------------------------------------------------
class Primitive : public IPrimitive
{
    NN_DISALLOW_COPY( Primitive );

public:
    //---------------------------------------------------------------------------
    //! @brief                  引数で指定されるプリミティブデータが必要とする頂点バッファサイズを計算する
    //! @param[in] primData     プリミティブデータ
    //---------------------------------------------------------------------------
    static size_t CalculateBufferSize( detail::BinaryData* primData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //---------------------------------------------------------------------------
    Primitive() NN_NOEXCEPT
    {
        m_IsInitialized     = false;
        m_ElementCount      = 0;
        m_ArrayCount        = 0;
        m_IndexCount        = 0;
        m_Guid              = 0;
        m_Stride            = 0;
        m_VertexBufferSize  = 0;
        m_IndexBufferSize   = 0;
    }

    //---------------------------------------------------------------------------
    //! @brief                  初期化処理。
    //! @param[in] pBuffer      登録先頂点バッファ
    //! @param[in] primData     プリミティブバイナリデータ
    //! @return    成功した場合 true
    //---------------------------------------------------------------------------
    bool Initialize( detail::ConstantBuffer* pBuffer, detail::BinaryData* primData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      終了処理。
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT {}

    //---------------------------------------------------------------------------
    //! @brief  頂点バッファ配列数を取得。
    //! @return 配列数
    //---------------------------------------------------------------------------
    int GetVertexBufferArrayCount() const NN_NOEXCEPT
    {
        return m_ArrayCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  インデックス数を取得。
    //! @return インデックス数
    //---------------------------------------------------------------------------
    int GetIndexCount() const NN_NOEXCEPT
    {
        return m_IndexCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  要素数を取得します。
    //! @return 要素数
    //---------------------------------------------------------------------------
    int GetElementCount() const NN_NOEXCEPT
    {
        return m_ElementCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  プリミティブ識別IDを取得します。
    //! @return プリミティブ識別ID
    //---------------------------------------------------------------------------
    uint64_t GetGlobalId() const NN_NOEXCEPT
    {
        return m_Guid;
    }

    //---------------------------------------------------------------------------
    //! @brief  ストライドを取得します。
    //! @param[in] bufferIndex ストライドを取得するバッファインデックス値
    //! @return ストライド値
    //---------------------------------------------------------------------------
    ptrdiff_t GetStrideSize( int bufferIndex ) const NN_NOEXCEPT
    {
        NN_UNUSED( bufferIndex );
        return m_Stride;
    }

    //---------------------------------------------------------------------------
    //! @brief  頂点バッファサイズを取得します。
    //! @param[in] bufferIndex バッファサイズを取得するバッファインデックス値
    //! @return 頂点バッファサイズ
    //---------------------------------------------------------------------------
    size_t GetVertexBufferSize( int bufferIndex ) const NN_NOEXCEPT
    {
        NN_UNUSED( bufferIndex );
        return m_VertexBufferSize;
    }

    //---------------------------------------------------------------------------
    //! @brief  頂点バッファのGpuアドレスを取得します。
    //! @param[out] pOutAddress Gpuアドレス書き込み先
    //! @param[in] bufferIndex Gpuアドレスを取得するバッファインデックス値
    //! @return Gpuアドレス
    //---------------------------------------------------------------------------
    void GetVertexBufferGpuAddress( nn::gfx::GpuAddress* pOutAddress, int bufferIndex ) const NN_NOEXCEPT
    {
        NN_UNUSED( bufferIndex );
        *pOutAddress = m_VertexBufferAddress;
    }

    //---------------------------------------------------------------------------
    //! @brief  インデックスバッファのGpuアドレスを取得します。
    //! @return Gpuアドレス
    //---------------------------------------------------------------------------
    const nn::gfx::GpuAddress& GetIndexBufferGpuAddress() const NN_NOEXCEPT
    {
        return m_IndexBufferAddress;
    }

    //---------------------------------------------------------------------------
    //! @brief  プリミティブ内の頂点位置情報を取得します。
    //! @param[out] pOutPos      頂点位置書き込み先
    //! @param[in]  index        頂点インデックス
    //---------------------------------------------------------------------------
    void GetPosition( nn::util::Vector3fType* pOutPos, int index ) const NN_NOEXCEPT
    {
        if( index > m_ArrayCount )
        {
            return;
        }

        nn::util::VectorSet( pOutPos,
            m_pSrcPosition4[index].x,
            m_pSrcPosition4[index].y,
            m_pSrcPosition4[index].z );
    }

    //---------------------------------------------------------------------------
    //! @brief  プリミティブ内の法線位置情報を取得します。
    //! @param[out] pOutNormal   法線書き込み先
    //! @param[in]  index        頂点インデックス
    //---------------------------------------------------------------------------
    void GetNormal( nn::util::Vector3fType* pOutNormal, int index ) const NN_NOEXCEPT
    {
        if ( !m_pSrcNormal4 )
        {
            nn::util::VectorSet( pOutNormal, 0.0f, 0.0f, 0.0f );
            return;
        }

        if( index > m_ArrayCount )
        {
            return;
        }

        nn::util::VectorSet( pOutNormal,
            m_pSrcNormal4[index].x,
            m_pSrcNormal4[index].y,
            m_pSrcNormal4[index].z );
    }

    //---------------------------------------------------------------------------
    //! @brief  頂点バッファを取得します。
    //! @return 頂点バッファ
    //---------------------------------------------------------------------------
    const nn::util::Float4* GetVertexBuffer() const NN_NOEXCEPT
    {
        return m_pSrcPosition4;
    }

    //---------------------------------------------------------------------------
    //! @brief  インデックスバッファを取得します。
    //! @return インデックスバッファ
    //---------------------------------------------------------------------------
    const void* GetIndexBuffer() const NN_NOEXCEPT
    {
        return m_pIndexBuffer;
    }

    //---------------------------------------------------------------------------
    //! @brief  バッファ数を取得します。
    //! @return バッファ数
    //---------------------------------------------------------------------------
    virtual int GetBufferCount() const NN_NOEXCEPT
    {
        return 1;
    }

    //---------------------------------------------------------------------------
    //! @brief  各アトリビュートのバッファインデックスを取得します。
    //! @param[in] index        アトリビュートのフォーマットを取得する要素インデックス
    //! @return アトリビュートバッファインデックス
    //---------------------------------------------------------------------------
    int GetBufferIndex( PrimitiveDataElementIndex index ) const NN_NOEXCEPT
    {
        NN_UNUSED( index );
        return 0;
    }

    //---------------------------------------------------------------------------
    //! @brief  各アトリビュートのフォーマットを取得します。
    //! @param[in] index        アトリビュートフォーマットを取得する要素インデックス
    //! @return                 アトリビュートフォーマット
    //---------------------------------------------------------------------------
    nn::gfx::AttributeFormat GetFormat( PrimitiveDataElementIndex index ) const NN_NOEXCEPT
    {
        NN_UNUSED( index );
        return nn::gfx::AttributeFormat_32_32_32_32_Float;
    }

    //---------------------------------------------------------------------------
    //! @brief  インデックスのフォーマットを取得します。
    //! @return                 インデックスフォーマット
    //---------------------------------------------------------------------------
    nn::gfx::IndexFormat GetIndexFormat() const NN_NOEXCEPT
    {
        return nn::gfx::IndexFormat_Uint32;
    }

    //---------------------------------------------------------------------------
    //! @brief  各アトリビュートのインタリーブ内オフセット値を取得します。
    //! @param[in] index        オフセットを取得する要素インデックス
    //---------------------------------------------------------------------------
    ptrdiff_t GetOffset( PrimitiveDataElementIndex index ) const NN_NOEXCEPT
    {
        return m_Offset[ index ];
    }

    //---------------------------------------------------------------------------
    //! @brief  描画コマンドを発行します。
    //! @param[in] pCommandBuffer  コマンドバッファ
    //! @param[in] instance        インスタンス数
    //! @param[in] lodLevel        描画に利用する Lod レベル
    //---------------------------------------------------------------------------
    void Draw( nn::gfx::CommandBuffer* pCommandBuffer, int instance, int lodLevel ) const NN_NOEXCEPT
    {
        NN_UNUSED( lodLevel );
        pCommandBuffer->DrawIndexed( nn::gfx::PrimitiveTopology_TriangleList, GetIndexFormat(),
                                     GetIndexBufferGpuAddress(), GetIndexCount(), 0, instance, 0 );
    }

    //---------------------------------------------------------------------------
    //! @brief  描画コマンドを発行します。
    //! @param[in] pCommandBuffer  コマンドバッファ
    //! @param[in] instance        インスタンス インデックス
    //! @param[in] baseInstance    開始インスタンス インデックス
    //! @param[in] lodLevel        描画に利用する Lod レベル
    //---------------------------------------------------------------------------
    void Draw( nn::gfx::CommandBuffer* pCommandBuffer, int instance, int baseInstance, int lodLevel ) const NN_NOEXCEPT
    {
        NN_UNUSED( lodLevel );
        pCommandBuffer->DrawIndexed( nn::gfx::PrimitiveTopology_TriangleList,
            GetIndexFormat(),
            GetIndexBufferGpuAddress(),
            GetIndexCount(), 0, instance, baseInstance );
    }

private:
    //---------------------------------------------------------------------------
    //! @brief                  初期化処理。
    //! @param[in]  pBuffer     登録先頂点バッファ
    //! @param[in]  guid        Guid
    //! @param[in]  indexCount  インデックス数
    //! @param[in]  arrayCount  配列数
    //! @param[in]  pPosSrc     位置
    //! @param[in]  pNorSrc     法線
    //! @param[in]  pTanSrc     接線
    //! @param[in]  pColSrc     カラー
    //! @param[in]  pTexSrc     テクスチャUV
    //! @param[in]  pIndexSrc   インデックス
    //! @return     成功した場合 true
    //---------------------------------------------------------------------------
    bool Initialize( detail::ConstantBuffer* pBuffer,
                     uint64_t                guid,
                     int                     indexCount,
                     int                     arrayCount,
                     void*                   pPosSrc,
                     void*                   pNorSrc,
                     void*                   pTanSrc,
                     void*                   pColSrc,
                     void*                   pTexSrc,
                     void*                   pIndexSrc ) NN_NOEXCEPT;

private:
    int                             m_ElementCount;         //!< 要素数
    int                             m_ArrayCount;           //!< 配列数
    int                             m_IndexCount;           //!< インデックス数
    uint64_t                        m_Guid;                 //!< 識別ID
    ptrdiff_t                       m_Stride;               //!< ストライド
    size_t                          m_VertexBufferSize;     //!< 頂点バッファサイズ
    size_t                          m_IndexBufferSize;      //!< 頂点バッファサイズ

    nn::gfx::GpuAddress             m_VertexBufferAddress;  //!< 頂点バッファのGpuアドレス
    nn::gfx::GpuAddress             m_IndexBufferAddress;   //!< インデックスバッファのGpuアドレス

    nn::util::Float4*               m_pSrcPosition4;        //!< 位置取得用
    nn::util::Float4*               m_pSrcNormal4;          //!< 法線取得用
    void*                           m_pIndexBuffer;         //!< インデックスバッファ

    ptrdiff_t                       m_Offset[ PrimitiveDataElement_Max ];
};

//---------------------------------------------------
//! @brief G3dプリミティブクラス
//---------------------------------------------------
class G3dPrimitive : public IPrimitive
{
    NN_DISALLOW_COPY( G3dPrimitive );

public:
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //---------------------------------------------------------------------------
    G3dPrimitive() NN_NOEXCEPT
    {
        m_IsInitialized = false;
    }

    //---------------------------------------------------------------------------
    //! @brief                  初期化処理。
    //! @param[in] resModel     nn::g3d::ResModel
    //! @param[in] pPrimitiveTableRecord     nn::g3d::ResModel
    //! @return    成功した場合 true
    //---------------------------------------------------------------------------
    bool Initialize( nn::g3d::ResModel* resModel, detail::ResG3dPrimitiveTableRecord* pPrimitiveTableRecord ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      終了処理。
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT {}

    //---------------------------------------------------------------------------
    //! @brief  頂点バッファ配列数を取得。
    //! @return 配列数
    //---------------------------------------------------------------------------
    int GetVertexBufferArrayCount() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( m_pResVertex );
        return m_pResVertex->GetCount();
    }

    //---------------------------------------------------------------------------
    //! @brief  インデックス数を取得。
    //! @return インデックス数
    //---------------------------------------------------------------------------
    int GetIndexCount() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( m_pResMesh );
        return m_pResMesh->GetCount();
    }

    //---------------------------------------------------------------------------
    //! @brief  要素数を取得します。
    //! @return 要素数
    //---------------------------------------------------------------------------
    int GetElementCount() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( m_pResVertex );
        return m_pResVertex->GetVertexBufferCount();
    }

    //---------------------------------------------------------------------------
    //! @brief  プリミティブ識別IDを取得します。
    //! @return プリミティブ識別ID
    //---------------------------------------------------------------------------
    uint64_t GetGlobalId() const NN_NOEXCEPT
    {
        return 0;
    }

    //---------------------------------------------------------------------------
    //! @brief  ストライドを取得します。
    //! @param[in] bufferIndex ストライドを取得するバッファインデックス値
    //! @return ストライド値
    //---------------------------------------------------------------------------
    ptrdiff_t GetStrideSize( int bufferIndex ) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( m_pResVertex );
        return m_pResVertex->GetVertexBufferStride( bufferIndex );
    }

    //---------------------------------------------------------------------------
    //! @brief  頂点バッファサイズを取得します。
    //! @param[in] bufferIndex バッファサイズを取得するバッファインデックス値
    //! @return 頂点バッファサイズ
    //---------------------------------------------------------------------------
    size_t GetVertexBufferSize( int bufferIndex ) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( m_pResVertex );
        return m_pResVertex->GetVertexBufferInfo( bufferIndex )->GetSize();
    }

    //---------------------------------------------------------------------------
    //! @brief  頂点バッファのGpuアドレスを取得します。
    //! @param[out] pOutAddress Gpuアドレス書き込み先
    //! @param[in] bufferIndex Gpuアドレスを取得するバッファインデックス値
    //! @return Gpuアドレス
    //---------------------------------------------------------------------------
    void GetVertexBufferGpuAddress( nn::gfx::GpuAddress* pOutAddress, int bufferIndex ) const NN_NOEXCEPT
    {
        m_pResVertex->GetVertexBuffer( bufferIndex )->GetGpuAddress( pOutAddress );
    }

    //---------------------------------------------------------------------------
    //! @brief  インデックスバッファのGpuアドレスを取得します。
    //! @return Gpuアドレス
    //---------------------------------------------------------------------------
    const nn::gfx::GpuAddress& GetIndexBufferGpuAddress() const NN_NOEXCEPT
    {
        return m_IndexBufferAddress;
    }

    //---------------------------------------------------------------------------
    //! @brief  頂点バッファを取得します。
    //! @return 頂点バッファ
    //---------------------------------------------------------------------------
    const nn::util::Float4* GetVertexBuffer() const NN_NOEXCEPT
    {
        return nullptr;
    }

    //---------------------------------------------------------------------------
    //! @brief  インデックスバッファを取得します。
    //! @return インデックスバッファ
    //---------------------------------------------------------------------------
    const void* GetIndexBuffer() const NN_NOEXCEPT
    {
        return nullptr;
    }

    //---------------------------------------------------------------------------
    //! @brief  バッファ数を取得します。
    //! @return バッファ数
    //---------------------------------------------------------------------------
    virtual int GetBufferCount() const NN_NOEXCEPT
    {
        return m_pResVertex->GetVertexBufferCount();
    }

    //---------------------------------------------------------------------------
    //! @brief  各アトリビュートのバッファインデックスを取得します。
    //! @param[in] index        アトリビュートのフォーマットを取得する要素インデックス
    //! @return アトリビュートバッファインデックス
    //---------------------------------------------------------------------------
    int GetBufferIndex( PrimitiveDataElementIndex index ) const NN_NOEXCEPT
    {
        return m_BufferIndex[ index ];
    }

    //---------------------------------------------------------------------------
    //! @brief  各アトリビュートのフォーマットを取得します。
    //! @param[in] index        アトリビュートのフォーマットを取得する要素インデックス
    //! @return アトリビュートフォーマット
    //---------------------------------------------------------------------------
    nn::gfx::AttributeFormat GetFormat( PrimitiveDataElementIndex index ) const NN_NOEXCEPT
    {
        return m_AttrFormat[ index ];
    }

    //---------------------------------------------------------------------------
    //! @brief  各アトリビュートのインタリーブ内オフセット値を取得します。
    //! @param[in] index        オフセットを取得する要素インデックス
    //! @return                 オフセット値
    //---------------------------------------------------------------------------
    ptrdiff_t GetOffset( PrimitiveDataElementIndex index ) const NN_NOEXCEPT
    {
        return m_Offset[ index ];
    }

    //---------------------------------------------------------------------------
    //! @brief  インデックスのフォーマットを取得します。
    //! @return インデックスのフォーマット
    //---------------------------------------------------------------------------
    nn::gfx::IndexFormat GetIndexFormat() const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( m_pResMesh );
        return m_pResMesh->GetIndexFormat();
    }

    //---------------------------------------------------------------------------
    //! @brief  描画コマンドを発行します。
    //! @param[in] pCommandBuffer  コマンドバッファ
    //! @param[in] instance        インスタンス数
    //! @param[in] lodLevel        描画に利用する Lod レベル
    //---------------------------------------------------------------------------
    void Draw( nn::gfx::CommandBuffer* pCommandBuffer, int instance, int lodLevel ) const NN_NOEXCEPT
    {
        int meshCount = m_pResShape->GetMeshCount();
        if ( lodLevel < meshCount )
        {
            meshCount = lodLevel;
        }
        else
        {
            meshCount--;
        }

        nn::g3d::ResMesh* pMesh = m_pResShape->GetMesh( meshCount );
        if ( pMesh )
        {
            pMesh->Draw( pCommandBuffer, instance );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  描画コマンドを発行します。
    //! @param[in] pCommandBuffer  コマンドバッファ
    //! @param[in] instance        インスタンス インデックス
    //! @param[in] baseInstance    開始インスタンス インデックス
    //! @param[in] lodLevel        描画に利用する Lod レベル
    //---------------------------------------------------------------------------
    void Draw( nn::gfx::CommandBuffer* pCommandBuffer, int instance, int baseInstance, int lodLevel ) const NN_NOEXCEPT
    {
        int meshCount = m_pResShape->GetMeshCount();
        if ( lodLevel < meshCount )
        {
            meshCount = lodLevel;
        }
        else
        {
            meshCount--;
        }

        nn::g3d::ResMesh* pMesh = m_pResShape->GetMesh( meshCount );
        if ( pMesh )
        {
            pMesh->Draw( pCommandBuffer, instance, baseInstance );
        }
    }

private:
    nn::g3d::ResModel*          m_pResModel;
    nn::g3d::ResVertex*         m_pResVertex;
    nn::g3d::ResShape*          m_pResShape;
    nn::g3d::ResMesh*           m_pResMesh;
    nn::gfx::GpuAddress         m_IndexBufferAddress;                       //!< インデックスバッファのGpuアドレス
    nn::gfx::AttributeFormat    m_AttrFormat[ PrimitiveDataElement_Max ];
    int                         m_BufferIndex[ PrimitiveDataElement_Max ];
    ptrdiff_t                   m_Offset[ PrimitiveDataElement_Max ];
};


} // namespace vfx
} // namespace nn
