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
#include <nn/vfx/vfx_Resource.h>
#include <nn/vfx/vfx_Binary.h>
#include <nn/vfx/vfx_Primitive.h>
#include <nn/vfx/vfx_TextureSampler.h>
#include <nn/vfx/vfx_EmitterRes.h>
#include <nn/vfx/vfx_ShaderManager.h>
#include <nn/vfx/vfx_DescriptorSlotDefinitions.h>

#include <nn/g3d.h>

namespace nn {
namespace vfx {

class System;


//---------------------------------------------------
//! @brief エフェクトリソース管理クラス
//---------------------------------------------------
class Resource
{
    NN_DISALLOW_COPY( Resource );
public:
    //---------------------------------------------------------------------------
    //! @brief        エミッタセットリソースを取得します。
    //!
    //! @param[in]    emitterSetId          エミッタセットID。
    //! @return       エミッタセットリソース。取得に失敗した場合は、NULLが返ります。
    //---------------------------------------------------------------------------
    EmitterSetResource* GetEmitterSetResource( int emitterSetId ) const NN_NOEXCEPT
    {
        if( emitterSetId >= static_cast< int >( m_EmitterSetCount ) )
        {
            return NULL;
        }

        if( m_BindEmitterSetResArray[ emitterSetId ] )
        {
            return m_BindEmitterSetResArray[ emitterSetId ];
        }
        return &m_EmitterSetResArray[ emitterSetId ];
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタセットリソースを取得します。
    //!
    //! @param[in]    emitterSetName          エミッタセット名。
    //! @return       エミッタセットリソース。取得に失敗した場合は、NULLが返ります。
    //---------------------------------------------------------------------------
    EmitterSetResource* GetEmitterSetResource( const char* emitterSetName ) const NN_NOEXCEPT
    {
        int emitterSetId = SearchEmitterSetId( emitterSetName );
        return GetEmitterSetResource( emitterSetId );
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタセットリソースを取得します。
    //!
    //! @param[in]    emitterSetId          エミッタセットID。
    //! @return       エミッタセットリソース。取得に失敗した場合は、NULLが返ります。
    //---------------------------------------------------------------------------
    EmitterSetResource* GetEmitterSetResourceRom( int emitterSetId ) const NN_NOEXCEPT
    {
        if( emitterSetId >= static_cast< int >( m_EmitterSetCount ) )
        {
            return NULL;
        }
        return &m_EmitterSetResArray[ emitterSetId ];
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタセットリソースを取得します。
    //!
    //! @param[in]    emitterSetName          エミッタセット名。
    //! @return       エミッタセットリソース。取得に失敗した場合は、NULLが返ります。
    //---------------------------------------------------------------------------
    EmitterSetResource* GetEmitterSetResourceRom( const char* emitterSetName ) const NN_NOEXCEPT
    {
        int emitterSetId = SearchEmitterSetId( emitterSetName );
        return GetEmitterSetResourceRom( emitterSetId );
    }

    //---------------------------------------------------------------------------
    //! @brief        指定EmitterSetが無限寿命、または、ループエフェクトかを取得する。
    //!
    //! @param[in]    emitterSetId          エミッタセット名。
    //! @return       無限寿命、または、ループエフェクトかどうか
    //---------------------------------------------------------------------------
    bool IsNeedFade( int emitterSetId ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        エミッタセット名の取得を行います。
    //!
    //! @param[in]    emitterSetId          エミッタセットID。
    //! @return       エミッタセット名。
    //---------------------------------------------------------------------------
    const char* GetEmitterSetName( int emitterSetId ) const NN_NOEXCEPT
    {
        EmitterSetResource* pEmitterSetResource = GetEmitterSetResource( emitterSetId );
        if( !pEmitterSetResource )
        {
            return NULL;
        }

        return pEmitterSetResource->pResEmitterSet->name;
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタセットのファイルパス取得を行います。
    //!
    //! @param[in]    emitterSetId          エミッタセットID。
    //! @return       エミッタセットのファイルパス
    //---------------------------------------------------------------------------
    const char* GetEmitterSetFilePath( int emitterSetId ) const NN_NOEXCEPT
    {
        EmitterSetResource* pEmitterSetResource = GetEmitterSetResource( emitterSetId );
        if ( !pEmitterSetResource || !pEmitterSetResource->pFilePath )
        {
            return NULL;
        }

        return pEmitterSetResource->pFilePath;
    }

    //---------------------------------------------------------------------------
    //! @brief  リソースに含まれるエミッタセット数を取得します。
    //! @return リソースに含まれるエミッタセット数
    //---------------------------------------------------------------------------
    int GetEmitterSetCount() const NN_NOEXCEPT
    {
        return m_EmitterSetCount;
    }

    //---------------------------------------------------------------------------
    //! @brief                  指定エミッタセット内に含まれるエミッタ数を取得します。
    //! @param[in] emitterSetId エミッタセットID
    //! @return                 指定エミッタセット内に含まれるエミッタ数
    //---------------------------------------------------------------------------
    int GetEmitterCount( int emitterSetId ) const NN_NOEXCEPT
    {
        EmitterSetResource* pEmitterSetResource = GetEmitterSetResource( emitterSetId );
        if( !pEmitterSetResource )
        {
            return 0;
        }
        return pEmitterSetResource->emitterCount;
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタセットIDの検索を行います。
    //!
    //! @param[in]    emitterSetName          検索するエミッタセット名。
    //! @return       エミッタセットID（無ければ VFX_INVALID_EMITTER_SET_ID が返ります）。
    //---------------------------------------------------------------------------
    int SearchEmitterSetId( const char* emitterSetName ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        エミッタセットIDの検索を行います。
    //!
    //! @param[in]    emitterSetFilePath      検索するエミッタセットのファイルパス
    //! @return       エミッタセットID（無ければ VFX_INVALID_EMITTER_SET_ID が返ります）。
    //---------------------------------------------------------------------------
    int SearchEmitterSetIdWithFilePath( const char* emitterSetFilePath ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        エミッタIDの検索を行います。
    //!               この関数は非常に低速なため、リアルタイムではなるべく使用しないで下さい。
    //!
    //! @param[in]    emitterSetName          検索するエミッタセット名。
    //! @param[in]    emitterName      検索するエミッタ名。
    //! @return       エミッタセットID（無ければ VFX_INVALID_EMITTER_ID が返ります）。
    //---------------------------------------------------------------------------
    int SearchEmitterId( const char* emitterSetName, const char* emitterName ) const NN_NOEXCEPT
    {
        int emitterSetId = SearchEmitterSetId( emitterSetName );
        if( emitterSetId == InvalidValueId_EmitterSetId )
        {
            return InvalidValueId_EmitterId;
        }
        return SearchEmitterId( emitterSetId, emitterName );
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタIDの検索を行います。この関数は非常に低速なため、リアルタイムではなるべく使用しないで下さい。
    //!
    //! @param[in]    emitterSetId     検索するエミッタセットID。
    //! @param[in]    emitterName      検索するエミッタ名。
    //!
    //! @return       エミッタセットID（無ければVFX_INVALID_EMITTER_ID が返ります）。
    //---------------------------------------------------------------------------
    int SearchEmitterId( int emitterSetId, const char* emitterName ) const NN_NOEXCEPT
    {
        EmitterSetResource* setRes = GetEmitterSetResource( emitterSetId );
        if( !setRes )
        {
            return InvalidValueId_EmitterId;
        }

        for( int i = 0; i < setRes->emitterCount; i++ )
        {
            if( strcmp( emitterName, setRes->pEmitterResource[ i ].m_pResEmitter->name ) == 0 )
            {
                return i;
            }
        }
        return InvalidValueId_EmitterId;
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタIDの検索を行います。この関数は非常に低速なため、リアルタイムではなるべく使用しないで下さい。
    //!
    //! @param[in]    emitterSetId     検索するエミッタセットID。
    //! @param[in]    emitterName      検索するエミッタ名。
    //! @param[in]    emitterIndex       検索するエミッタインデックス(同名エミッタが存在する場合に利用)。
    //!
    //! @return       エミッタセットID（無ければ VFX_INVALID_EMITTER_ID が返ります）。
    //---------------------------------------------------------------------------
    int SearchEmitterId( int emitterSetId, const char* emitterName, int emitterIndex ) const NN_NOEXCEPT
    {
        EmitterSetResource* pEmitterSetResource = GetEmitterSetResource( emitterSetId );
        if( !pEmitterSetResource )
        {
            return InvalidValueId_EmitterId;
        }

        for( int i = 0; i < pEmitterSetResource->emitterCount; i++ )
        {
            if( strcmp( emitterName, pEmitterSetResource->pEmitterResource[ i ].m_pResEmitter->name ) == 0 )
            {
                if( i == emitterIndex )
                {
                    return i;
                }
            }
        }
        return InvalidValueId_EmitterId;
    }

    //------------------------------------------------------------------------------
    //! @brief  リソースIDを取得します。
    //! @return リソースID
    //------------------------------------------------------------------------------
    int GetResourceId() const NN_NOEXCEPT
    {
        return m_ResourceId;
    }

    //------------------------------------------------------------------------------
    //! @brief  ResShaderFileファイルを取得します。
    //! @return ResShaderFileファイルポインタ
    //------------------------------------------------------------------------------
    const nn::gfx::ResShaderFile* GetResShaderFile() const NN_NOEXCEPT
    {
        return m_pResShaderFile;
    }

    //------------------------------------------------------------------------------
    //! @brief  ComputeResShaderFileファイルを取得します。
    //! @return ComputeResShaderFileファイルポインタ
    //------------------------------------------------------------------------------
    const nn::gfx::ResShaderFile* GetComputeResShaderFile() const NN_NOEXCEPT
    {
        return m_pComputeResShaderFile;
    }

    //------------------------------------------------------------------------------
    //! @brief  ResTextureFileファイルを取得します。
    //! @return ResTextureFileファイルポインタ
    //------------------------------------------------------------------------------
    const nn::gfx::ResTextureFile* GetResTextureFile() const NN_NOEXCEPT
    {
        return m_ResTextureFile;
    }

    //------------------------------------------------------------------------------
    //! @brief  テクスチャ数を取得します。
    //! @return リソースが保持するテクスチャ枚数
    //------------------------------------------------------------------------------
    int GetTextureCount() const NN_NOEXCEPT
    {
        return m_TextureCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  チャイルドエミッタが存在するかどうか。
    //! @param[in] targetSetId エミッタセットID
    //! @return チャイルドエミッタが存在するかどうか
    //---------------------------------------------------------------------------
    bool IsExistChildEmitter( int targetSetId ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  バイナリの先頭を取得します。
    //! @return バイナリの先頭アドレス
    //---------------------------------------------------------------------------
    void* GetBinaryTop() const NN_NOEXCEPT
    {
        return m_pBinaryTop;
    }

    //---------------------------------------------------------------------------
    //! @brief  プロジェクト名を取得します。
    //! @return プロジェクト名
    //---------------------------------------------------------------------------
    const char* GetProjectName() const NN_NOEXCEPT
    {
        return GetBinaryName();
    }

    //---------------------------------------------------------------------------
    //! @brief  バイナリ名を取得します。
    //! @return バイナリ名
    //---------------------------------------------------------------------------
    const char* GetBinaryName() const NN_NOEXCEPT
    {
        detail::ResHeader* pHeader = reinterpret_cast< detail::ResHeader* >( m_pBinaryTop );
        return reinterpret_cast< const char* >( pHeader->name );
    }

    //---------------------------------------------------------------------------
    //! @briefprivate targetSetIdで指定されるエミッタセットリソースを pEmitterSetResource に指定したリソースにバインドします。
    //! @param[in] targetSetId エミッタセットID
    //! @param[in] pEmitterSetResource エミッタセットリソースへのポインタ
    //! @return 成功した場合trueを返す。
    //---------------------------------------------------------------------------
    bool BindResource( int targetSetId, EmitterSetResource* pEmitterSetResource ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate targetSetIdで指定されるエミッタセットリソースの繋ぎ換えを解除します。
    //! @param[in] targetSetId エミッタセットID
    //! @return 成功した場合trueを返す。
    //---------------------------------------------------------------------------
    bool UnbindResource( int targetSetId ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  エミッタの名前（バインドしても変化しない）を取得します。
    //! @param[in] emitterSetId エミッタセットID
    //! @param[in] emitterId    エミッタID
    //! @return                 エミッタの名前
    //---------------------------------------------------------------------------
    const char* GetEmitterNameROM( int emitterSetId, int emitterId ) const NN_NOEXCEPT
    {
        EmitterSetResource* pEmitterSetResource = GetEmitterSetResourceRom( emitterSetId );
        if( !pEmitterSetResource )
        {
            return NULL;
        }
        if( emitterId >= static_cast< int >( pEmitterSetResource->emitterCount ) )
        {
            return NULL;
        }

        return  pEmitterSetResource->pEmitterResource[ emitterId ].m_pResEmitter->name;
    }

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief        コンストラクタ
    //! @param[in]    pDevice                           gfxデバイス。
    //! @param[in]    pHeap                             ヒープ。
    //! @param[in]    pBinary                           エフェクトバイナリ。
    //! @param[in]    pMemoryPool                       メモリプール。エフェクトバイナリ(pBinary)をメモリプール内に配置した場合にそのメモリプールを指定します。未配置の場合は nullptr を指定してください。
    //! @param[in]    memoryPoolSize                    配置先メモリプールのサイズ。
    //! @param[in]    memoryPoolOffset                  メモリプール内オフセット。
    //! @param[in]    resourceId                        リソースID。
    //! @param[in]    pSystem                           システムへのポインタ
    //! @param[in]    shaderDelaySetup                  シェーダ遅延セットアップ。
    //! @param[in]    residentResource                  共通で利用されるテクスチャを含んだリソース。
    //! @return       生成されたエフェクトリソースのインスタンス。
    //---------------------------------------------------------------------------
    Resource( nn::gfx::Device* pDevice, Heap* pHeap, void* pBinary, nn::gfx::MemoryPool* pMemoryPool, size_t memoryPoolSize, size_t memoryPoolOffset, int resourceId,
        System* pSystem, bool shaderDelaySetup, Resource* residentResource = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        エフェクトリソースの破棄を行います。
    //---------------------------------------------------------------------------
    virtual ~Resource() NN_NOEXCEPT;

    //@}

    //---------------------------------------------------------------------------
    //! @brief        エフェクトリソースの初期化を行います。
    //! @param[in]    pHeap             ヒープ。
    //---------------------------------------------------------------------------
    void Finalize( Heap* pHeap ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        リソースの削除フラグを立てます。
    //---------------------------------------------------------------------------
    void SetKillMark() NN_NOEXCEPT
    {
        m_IsAlive = false;
    }

    //---------------------------------------------------------------------------
    //! @brief  リソースの生存フラグを取得します。
    //! @return 生存フラグ
    //---------------------------------------------------------------------------
    bool IsAlive() const NN_NOEXCEPT
    {
        return m_IsAlive;
    }

    //---------------------------------------------------------------------------
    //! @brief        リソースサイズの詳細をログ出力します。
    //---------------------------------------------------------------------------
    void OutputResourceInfo() const NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief 互換性の目的で定義
    //! @details
    //! 本関数は互換性の目的で定義されました。 ディスクリプタスロット確保/開放関数のインタフェースです。
    //------------------------------------------------------------------------------
    typedef  nn::vfx::RegisterTextureViewSlot   RegisterTextureViewSlot;
    typedef  nn::vfx::UnregisterTextureViewSlot UnregisterTextureViewSlot;

    //------------------------------------------------------------------------------
    //! @brief テクスチャービューをディスクリプタプールに登録します。
    //! @param[in] pRegisterTextureSlotCallback     ディスクリプタスロット確保関数です。
    //! @param[in] pUserData                        ユーザデータです。
    //------------------------------------------------------------------------------
    void RegisterTextureViewToDescriptorPool(nn::vfx::RegisterTextureViewSlot pRegisterTextureSlotCallback, void* pUserData) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief テクスチャービューをディスクリプタプールから解放します。
    //! @param[in] pUnregisterTextureSlotCallback     ディスクリプタスロット解放関数です。
    //! @param[in] pUserData                        ユーザデータです。
    //------------------------------------------------------------------------------
    void UnregisterTextureViewFromDescriptorPool(nn::vfx::UnregisterTextureViewSlot pUnregisterTextureSlotCallback, void* pUserData) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  外部で初期化された nn::gfx::ResSahderFile をバインドします。
    //! @param[in] pResShaderFile            エフェクトバイナリコンバート時に分離出力した 通常のResShaderFile。
    //! @param[in] pResComputeResShaderFile  エフェクトバイナリコンバート時に分離出力した コンピュートシェーダの ResShaderFile。
    //! @return Bind が成功したかどうか
    //---------------------------------------------------------------------------
    bool BindExternalResShaderFile( nn::gfx::ResShaderFile* pResShaderFile, nn::gfx::ResShaderFile* pResComputeResShaderFile ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  外部で初期化された nn::gfx::ResTextureFile をバインドします。
    //! @param[in] pResTextureFile           エフェクトバイナリコンバート時に分離出力した
    //!                                      ResTextureFile。ResTextureFile 内に含まれる UserDescriptorSlot にスロット値を書き込む必要があります。
    //! @return Bind が成功したかどうか
    //---------------------------------------------------------------------------
    bool BindExternalResTextureFile( nn::gfx::ResTextureFile* pResTextureFile ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  外部で初期化された nn::g3d::ResFile をバインドします。
    //! @param[in] pG3dResFile           エフェクトバイナリコンバート時に分離出力した
    //!                                  nn::g3d::ResFile。
    //! @return Bind が成功したかどうか
    //---------------------------------------------------------------------------
    bool BindExternalG3dResFile( nn::g3d::ResFile* pG3dResFile ) NN_NOEXCEPT;


    //---------------------------------------------------------------------------
    //! @brief リソースをアンリロケートします
    //! @details
    //!  登録したリソースIDでnn::vfx::System::ClearResource()を呼び出した後に
    //!  このメソッドを呼び出すことでリソースのメモリを移動可能な状態にすることができます。
    //! @see       nn::util::RelocationTable::Unrelocate()
    //! @param[in] pData    再利用したいリソースへのポインタ。
    //---------------------------------------------------------------------------
    static void Unrelocate( void * pData ) NN_NOEXCEPT;

private:
    //------------------------------------------------------------------------------
    //! @brief              バイナリをトレースする
    //! @param[in] pData  バイナリのトップアドレス
    //------------------------------------------------------------------------------
    void Trace( detail::BinaryData* pData ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          EmitterSetArray バイナリをトレースする
    //! @param[in] pData                EmitterSetArray バイナリのトップアドレス
    //! @param[in] bufferSizeCalculator バッファサイズを計算するクラス
    //------------------------------------------------------------------------------
    void TraceEmitterSetArray( detail::BinaryData* pData, detail::BufferSizeCalculator* bufferSizeCalculator ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          GfxTextureArray バイナリをトレースする
    //! @param[in] pData GfxTextureArray バイナリのトップアドレス
    //------------------------------------------------------------------------------
    void TraceGfxResTextureFile( detail::BinaryData* pData ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          PrimitiveArray バイナリをトレースする
    //! @param[in] pData PrimitiveArray バイナリのトップアドレス
    //------------------------------------------------------------------------------
    void TracePrimitiveArray( detail::BinaryData* pData ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          G3d Primitiveバイナリをトレースする
    //! @param[in] pData G3d Primitiveバイナリのトップアドレス
    //------------------------------------------------------------------------------
    void TraceG3dPrimitiveArray( detail::BinaryData* pData ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief                  ShaderArray バイナリをトレースする
    //! @param[in] pData         ShaderArray バイナリのトップアドレス
    //------------------------------------------------------------------------------
    void TraceShaderBinaryArray( detail::BinaryData* pData ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief                  EmitterSetResource を初期化する
    //! @param[in] pEmitterSetResource      エミッタセットリソースへのポインタ
    //! @param[in] bufferSizeCalculator     バッファサイズを計算するクラス
    //------------------------------------------------------------------------------
    void InitializeEmitterSetResource( EmitterSetResource* pEmitterSetResource, detail::BufferSizeCalculator* bufferSizeCalculator ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief                  EmitterResource を初期化する
    //! @param[in] pEmitterResource         エミッタリソースへのポインタ
    //! @param[in] pData                    バイナリへのポインタ
    //! @param[in] index                    インデックス
    //! @param[in] bufferSizeCalculator     バッファサイズを計算するクラス
    //------------------------------------------------------------------------------
    void InitializeEmitterResource( EmitterResource* pEmitterResource, detail::BinaryData* pData, int index, detail::BufferSizeCalculator* bufferSizeCalculator ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief                  EmitterResourceグラフィックスリソースを初期化する
    //! @param[in] pEmitterResource                 エミッタリソースへのポインタ
    //------------------------------------------------------------------------------
    void InitializeEmitterGraphicsResource( EmitterResource* pEmitterResource );

    //------------------------------------------------------------------------------
    //! @brief  EmitterResource の終了処理をする
    //! @param[in] pEmitterResource エミッタリソースへのポインタ
    //------------------------------------------------------------------------------
    void FinalizeEmitterResource( EmitterResource* pEmitterResource ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief  ResNameTable から指定 guid のレコードを検索する
    //------------------------------------------------------------------------------
    detail::ResNameTableRecord* SearchRecordFromResNameTable( detail::ResNameTableRecord* tableTop, uint64_t guid ) NN_NOEXCEPT;

public:
    //---------------------------------------------------------------------------
    //! @brief        Gfxデバイスの取得
    //! @return       Gfxデバイスへのポインタ
    //---------------------------------------------------------------------------
    nn::gfx::Device* GetGfxDevice() NN_NOEXCEPT
    {
        return m_pDevice;
    }

    //---------------------------------------------------------------------------
    //! @brief        リソースクラスが保持するヒープを取得します
    //! @return       nn::vfx::Heap へのポインタ
    //---------------------------------------------------------------------------
    nn::vfx::Heap* GetHeap() NN_NOEXCEPT
    {
        return &m_ResHeap;
    }

    //---------------------------------------------------------------------------
    //! @brief        リソースクラスが保持するヒープを取得します
    //! @return       nn::vfx::Heap へのポインタ
    //---------------------------------------------------------------------------
    detail::ShaderManager* GetShaderManager() NN_NOEXCEPT
    {
        return &m_ShaderManager;
    }

    //---------------------------------------------------------------------------
    //! @brief        リソースクラスが保持するヒープを取得します
    //! @return       nn::vfx::Heap へのポインタ
    //---------------------------------------------------------------------------
    detail::ComputeShaderManager* GetComputeShaderManager() NN_NOEXCEPT
    {
        return &m_ComputeShaderManager;
    }

    //------------------------------------------------------------------------------
    //  ResTexture を 検索、取得します。
    //! @param[in] guid テクスチャのGUID
    //! @return    検索した nn::gfx::ResTexture へのポインタ
    //------------------------------------------------------------------------------
    const nn::gfx::ResTexture* SearchResTexture( uint64_t guid ) const NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          テクスチャのデスクリプタスロットを取得する
    //! @param[out] pDescriptorSlot テクスチャのデスクリプタスロット
    //! @param[in] guid テクスチャのGUID
    //------------------------------------------------------------------------------
    bool GetTextureDescriptorSlot( nn::gfx::DescriptorSlot* pDescriptorSlot, uint64_t guid ) const NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief              プリミティブを検索する
    //! @param[in] guid     プリミティブのインデックス
    //! @return             プリミティブへのポインタ
    //------------------------------------------------------------------------------
    Primitive* GetPrimitive( uint64_t guid ) const NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief              G3dプリミティブを検索する
    //! @param[in] guid     プリミティブのインデックス
    //! @return             プリミティブへのポインタ
    //------------------------------------------------------------------------------
    IPrimitive* GetG3dPrimitive( uint64_t guid ) const NN_NOEXCEPT;

private:
    bool                            m_ShaderDelaySetup;                     //!< シェーダ遅延セットアップ
    bool                            m_IsAlive;                              //!< 生存フラグ
    nn::gfx::Device*                m_pDevice;                              //!< gfxデバイス
    nn::gfx::MemoryPool*            m_pMemoryPool;                          //!< エフェクトバイナリ配置先メモリプール
    size_t                          m_MemoryPoolSize;                       //!< エフェクトバイナリ配置先メモリプールサイズ
    size_t                          m_MemoryPoolOffset;                     //!< エフェクトバイナリ配置先メモリプール内オフセット
    void*                           m_pEmitterResArray;                     //!< エミッタリソースPtr
    System*                         m_pSystem;                              //!< システムクラス
    void*                           m_pBinaryTop;                           //!< バイナリ先頭
    int                             m_ResourceId;                           //!< リソースID
    int                             m_EmitterSetCount;                      //!< 総エミッタセット数
    int                             m_EmitterCount;                         //!< 総エミッタ数
    EmitterSetResource*             m_EmitterSetResArray;                   //!< エミッタセットリソース
    EmitterSetResource**            m_BindEmitterSetResArray;               //!< エミッタセットリソース
    int                             m_PrimitiveCount;                       //!< プリミティブ数
    void*                           m_PrimitiveArrayPtr;                    //!< プリミティブリソースPtr
    Primitive*                      m_PrimitiveArray;                       //!< プリミティブリソース
    int                             m_G3dPrimitiveCount;                    //!< プリミティブ数
    void*                           m_G3dPrimitiveArrayPtr;                 //!< G3dプリミティブリソースPtr
    G3dPrimitive*                   m_G3dPrimitiveArray;                    //!< G3dプリミティブリソース
    nn::g3d::ResFile*               m_pG3dResFile;                          //!< G3d ResFile
    int                             m_TextureCount;                         //!< テクスチャ数
    nn::gfx::ResTextureFile*        m_ResTextureFile;                       //!< ResTextureファイル
    detail::ResNameTableRecord*     m_pTextureTable;                        //!< テクスチャハッシュテーブル
    detail::ResG3dPrimitiveTableRecord* m_G3dPrimitiveTable;                //!< プリミティブハッシュテーブル
    size_t                          m_EmitterResourceSize;                  //!< エミッタバイナリの合計サイズ
    size_t                          m_TextureResourceSize;                  //!< テクスチャバイナリの合計サイズ
    size_t                          m_ShaderResourceSize;                   //!< シェーダバイナリの合計サイズ
    size_t                          m_ComputeShaderResourceSize;            //!< コンピュートシェーダバイナリの合計サイズ
    size_t                          m_PrimitiveResourceSize;                //!< プリミティブリソースバイナリの合計サイズ
    size_t                          m_G3dPrimitiveResourceSize;             //!< G3dプリミティブリソースバイナリの合計サイズ
    detail::ConstantBuffer          m_ResourceConstantBuffer;               //!< リソースコンスタントバッファ
    detail::ConstantBuffer          m_PrimitiveVertexBuffer;                //!< プリミティブ頂点バッファ
    detail::CalculateAllocatedSizeHeap  m_ResHeap;                          //!< リソース用のヒープ
    size_t                          m_ConstantBufferAllocatedSize;          //!< heap 定数バッファ    消費量サイズ
    size_t                          m_TextureAllocatedSize;                 //!< heap プリミティブ    消費量サイズ
    size_t                          m_PrimitiveAllocatedSize;               //!< heap プリミティブ    消費量サイズ
    size_t                          m_ShaderAllocatedSize;                  //!< heap シェーダ        消費量サイズ
    size_t                          m_G3dPrimitiveAllocatedSize;            //!< heap G3dプリミティブ 消費量サイズ
    size_t                          m_EmitterSetAllocatedSize;              //!< heap エミッタセット  消費量サイズ
    size_t                          m_EmitterAllocatedSize;                 //!< heap エミッタ        消費量サイズ
    Resource*                       m_pResidentResource;                    //!< 共通テクスチャを含むリソース
    nn::gfx::ResShaderFile*         m_pResShaderFile;                       //!< エフェクトバイナリ内に含まれる ResShaderFile
    nn::gfx::ResShaderFile*         m_pExtResShaderFile;                    //!< 外部から渡される ResShaderFile
    detail::ShaderManager           m_ShaderManager;                        //!< シェーダマネージャ
    nn::gfx::ResShaderFile*         m_pComputeResShaderFile;                //!< コンピュートシェーダ ResShaderFile
    nn::gfx::ResShaderFile*         m_pExtComputeResShaderFile;             //!< 外部から渡される ResShaderFile
    detail::ComputeShaderManager    m_ComputeShaderManager;                 //!< コンピュートシェーダマネージャ
    void*                           m_pVertexStateBuffer;                   //!< 頂点ステート用バッファ

    friend class System;
};

} // namespace vfx
} // namespace nn
