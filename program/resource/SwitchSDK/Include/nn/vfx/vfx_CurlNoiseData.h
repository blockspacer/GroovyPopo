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

#include <nn/vfx/vfx_Misc.h>
#include <nn/vfx/vfx_EmitterCalc.h>
#include <nn/vfx/vfx_Random.h>
#include <nn/vfx/vfx_System.h>

namespace nn {
namespace vfx {
namespace detail {

class CurlNoiseTexture;

//---------------------------------------------------------------------------
//! @brief      CurlNoise パラメータ初期化処理
//!
//!             CPU参照用のカールノイズ配列とGPU参照用のカールノイズテクスチャを生成します。
//! @param[in]      pDevice デバイス
//! @param[in]      heap    ヒープ
//---------------------------------------------------------------------------
void InitializeCurlNoise( nn::gfx::Device* pDevice, Heap* heap ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief      CurlNoise パラメータ終了処理
//!
//!             生成されたGPU参照用のカールノイズテクスチャを破棄します。
//! @param[in]      pDevice デバイス
//---------------------------------------------------------------------------
void FinalizeCurlNoise( nn::gfx::Device* pDevice ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief      CurlNoise テクスチャでヒープから確保したメモリサイズを取得する
//! @return     ヒープから確保したメモリサイズ
//---------------------------------------------------------------------------
size_t GetCurlNoiseTextureAllocatedSize() NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief      カールノイズテクスチャを取得します。
//! @return     テクスチャへの参照
//---------------------------------------------------------------------------
const CurlNoiseTexture& GetCurlNoiseTexture() NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief          カールノイズテクスチャを取得します。
//! @param[in,out]  pOutPos             位置
//! @param[in,out]  pOutVec             速度
//! @param[in]      pEmitter            エミッタへのポインタ
//! @param[in]      pParticleProperty   パーティクルアトリビュートへのポインタ
//! @param[in]      particleIndex       パーティクルのインデックス
//---------------------------------------------------------------------------
void CalculateParticleBehavior_FieldCurlNoise( nn::util::Vector3fType* pOutPos, nn::util::Vector3fType* pOutVec, Emitter* pEmitter, const ParticleProperty* pParticleProperty, int particleIndex ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief      カールノイズテクスチャビューをディスクリプタプールに登録します。
//! @param[in]  pRegisterTextureSlotCallback     ディスクリプタスロット確保関数
//! @param[in]  pUserData                        ユーザデータ
//---------------------------------------------------------------------------
void RegisterCurlNoiseTextureViewToDescriptorPool( nn::vfx::RegisterTextureViewSlot pRegisterTextureSlotCallback, void* pUserData ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief      カールノイズテクスチャービューをディスクリプタプールから解放します。
//! @param[in]  pUunregisterTextureSlotCallback ディスクリプタスロット確保関数
//! @param[in]  pUserData                       ユーザデータ
//---------------------------------------------------------------------------
void UnRegisterCurlNoiseTextureViewToDescriptorPool( nn::vfx::UnregisterTextureViewSlot pUunregisterTextureSlotCallback, void* pUserData ) NN_NOEXCEPT;

//---------------------------------------------------
//! @brief カールノイズテクスチャ
//---------------------------------------------------
class CurlNoiseTexture
{
    NN_DISALLOW_COPY( CurlNoiseTexture );
public:
    //---------------------------------------------------------------------------
    //! @brief  初期化処理
    //! @param[in]      pDevice デバイス
    //! @param[in]      heap    ヒープ
    //---------------------------------------------------------------------------
    void Initialize( nn::gfx::Device* pDevice, Heap* heap ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  終了処理
    //! @param[in]      pDevice デバイス
    //---------------------------------------------------------------------------
    void Finalize( nn::gfx::Device* pDevice ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  無効化
    //---------------------------------------------------------------------------
    void Invalidate() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      カールノイズテクスチャを取得
    //! @return     テクスチャへの参照
    //---------------------------------------------------------------------------
    const nn::gfx::Texture& GetTexture() const NN_NOEXCEPT
    {
        return m_Texture;
    }

    //---------------------------------------------------------------------------
    //! @brief      カールノイズテクスチャビューを取得
    //! @return     テクスチャへの参照
    //---------------------------------------------------------------------------
    const nn::gfx::TextureView& GetTextureView() const NN_NOEXCEPT
    {
        return m_TextureView;
    }

    //---------------------------------------------------------------------------
    //! @brief  デスクリプタスロットを取得する
    //! @return デスクリプタスロットへの参照
    //---------------------------------------------------------------------------
    const nn::gfx::DescriptorSlot& GetDescriptorSlot() const NN_NOEXCEPT
    {
        return m_DescriptorSlot;
    }

    //---------------------------------------------------------------------------
    //! @brief  テクスチャービューをディスクリプタプールに登録します
    //! @param[in]  pRegisterTextureSlotCallback     ディスクリプタスロット確保関数
    //! @param[in]  pUserData                        ユーザデータ
    //---------------------------------------------------------------------------
    void RegisterTextureViewToDescriptorPool( nn::vfx::RegisterTextureViewSlot pRegisterTextureSlotCallback, void* pUserData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  テクスチャービューをディスクリプタプールから開放します
    //! @param[in]  pUnregisterTextureSlotCallback    ディスクリプタスロット確保関数
    //! @param[in]  pUserData                        ユーザデータ
    //---------------------------------------------------------------------------
    void UnRegisterTextureViewToDescriptorPool( nn::vfx::UnregisterTextureViewSlot pUnregisterTextureSlotCallback, void* pUserData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate   コンストラクタ
    //---------------------------------------------------------------------------
    CurlNoiseTexture() NN_NOEXCEPT
    {
        m_pAllocatedBufferPtr = NULL;
    }

    nn::gfx::MemoryPool         m_MemoryPool;           //!< メモリプール
    nn::gfx::TextureView        m_TextureView;          //!< テクスチャビュー
    nn::gfx::Texture            m_Texture;              //!< テクスチャ
    void*                       m_pTextureImage;        //!< カールノイズテクスチャイメージ
    size_t                      m_TextureSize;          //!< カールノイズテクスチャサイズ
    void*                       m_pAllocatedBufferPtr;  //!< 確保したバッファへのポインタ
    size_t                      m_AllocatedSize;        //!< 確保したバッファサイズ
    nn::gfx::DescriptorSlot     m_DescriptorSlot;       //!< デスクリプタスロット
};


//---------------------------------------------------
//! @brief カールノイズ クラス
//!
//!        カールノイズテーブルへアクセスする為のクラスです。
//---------------------------------------------------
struct CurlNoiseTable
{
    //---------------------------------------------------------------------------
    //! @brief  int8_t 型 3次元ベクトル構造体
    //---------------------------------------------------------------------------
    struct Vector3int8
    {
        int8_t x;       //!< x成分
        int8_t y;       //!< y成分
        int8_t z;       //!< z成分
    };

    //---------------------------------------------------------------------------
    //! @brief  初期化処理です。
    //! @param[in] bufWidth バッファ幅
    //! @param[in] pTbl     テーブルへのポインタ
    //---------------------------------------------------------------------------
    void Initialize( int bufWidth, const void* pTbl ) NN_NOEXCEPT
    {
        m_BufferWidth = bufWidth;
        m_pCurlNoiseInt8Buffer = reinterpret_cast< const Vector3int8* >( pTbl );
    }

    //---------------------------------------------------------------------------
    //! @brief      カールノイズの int8_t の値を nn::util::Vector3fType で取り出します。
    //! @param[out] pOutVec カールノイズ成分
    //! @param[in]  x       x成分
    //! @param[in]  y       y成分
    //! @param[in]  z       z成分
    //---------------------------------------------------------------------------
    void GetCurlNoiseInt8Value( nn::util::Vector3fType* pOutVec, int x, int y, int z ) const NN_NOEXCEPT
    {
        if( x < 0 )
        {
            x *= -1;
        }
        if( y < 0 )
        {
            y *= -1;
        }
        if( z < 0 )
        {
            z *= -1;
        }
        x %= m_BufferWidth;
        y %= m_BufferWidth;
        z %= m_BufferWidth;

        int index = ( x * m_BufferWidth * m_BufferWidth ) + ( y * m_BufferWidth ) + z;
        nn::util::VectorSetX( pOutVec, m_pCurlNoiseInt8Buffer[ index ].x / 127.0f );
        nn::util::VectorSetY( pOutVec, m_pCurlNoiseInt8Buffer[ index ].y / 127.0f );
        nn::util::VectorSetZ( pOutVec, m_pCurlNoiseInt8Buffer[ index ].z / 127.0f );
    }

    //---------------------------------------------------------------------------
    //! @brief      カールノイズのint8_tの値を近傍8点補間してVector3で取り出します。
    //! @param[out] pOutValue   カールノイズ成分の nn::util::Vector3fType
    //! @param[in]  x           x成分
    //! @param[in]  y           y成分
    //! @param[in]  z           z成分
    //---------------------------------------------------------------------------
    void GetCurlNoiseS8Interpolate( nn::util::Vector3fType* pOutValue, float x, float y, float z ) const NN_NOEXCEPT
    {
        int ix = static_cast< int >( x );
        int iy = static_cast< int >( y );
        int iz = static_cast< int >( z );
        float dx = x - static_cast< float >( ix );
        float dy = y - static_cast< float >( iy );
        float dz = z - static_cast< float >( iz );

        nn::util::Vector3fType ret000;
        GetCurlNoiseInt8Value( &ret000, ix, iy, iz );
        nn::util::Vector3fType ret001;
        GetCurlNoiseInt8Value( &ret001, ix + 1, iy, iz );
        nn::util::Vector3fType ret010;
        GetCurlNoiseInt8Value( &ret010, ix, iy + 1, iz );
        nn::util::Vector3fType ret011;
        GetCurlNoiseInt8Value( &ret011, ix + 1, iy + 1, iz );

        nn::util::Vector3fType ret100;
        GetCurlNoiseInt8Value( &ret100, ix, iy, iz + 1 );
        nn::util::Vector3fType ret101;
        GetCurlNoiseInt8Value( &ret101, ix + 1, iy, iz + 1 );
        nn::util::Vector3fType ret110;
        GetCurlNoiseInt8Value( &ret110, ix, iy + 1, iz + 1 );
        nn::util::Vector3fType ret111;
        GetCurlNoiseInt8Value( &ret111, ix + 1, iy + 1, iz + 1 );


        nn::util::Vector3fType ret00x;
        nn::util::VectorLerp( &ret00x, ret000, ret001, dx );
        nn::util::Vector3fType ret01x;
        nn::util::VectorLerp( &ret01x, ret010, ret011, dx );
        nn::util::Vector3fType ret0y;
        nn::util::VectorLerp( &ret0y, ret00x, ret01x, dy );

        nn::util::Vector3fType ret10x;
        nn::util::VectorLerp( &ret10x, ret100, ret101, dx );
        nn::util::Vector3fType ret11x;
        nn::util::VectorLerp( &ret11x, ret110, ret111, dx );
        nn::util::Vector3fType ret1y;
        nn::util::VectorLerp( &ret1y, ret10x, ret11x, dy );

        nn::util::VectorLerp( pOutValue, ret0y, ret1y, dz );
    }

    int                 m_BufferWidth;              //!< バッファ幅
    const Vector3int8*  m_pCurlNoiseInt8Buffer;     //!< カールノイズバッファ
};

}
}
}
