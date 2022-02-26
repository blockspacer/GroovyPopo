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

#include <nn/vfx/vfx_Enum.h>
#include <nn/vfx/vfx_Config.h>
#include <nn/vfx/vfx_Binary.h>
#include <nn/vfx/vfx_Resource.h>
#include <nn/vfx/vfx_EmitterSet.h>
#include <nn/vfx/vfx_Emitter.h>
#include <nn/vfx/vfx_Particle.h>
#include <nn/vfx/vfx_Handle.h>
#include <nn/vfx/vfx_TemporaryBuffer.h>
#include <nn/vfx/vfx_EmitterCalc.h>
#include <nn/vfx/vfx_View.h>
#include <nn/vfx/vfx_Shader.h>
#include <nn/vfx/vfx_Callback.h>
#include <nn/vfx/vfx_BufferAllocator.h>

#include <nn/vfx/vfx_DescriptorSlotDefinitions.h>

namespace nn {
namespace vfx {
//---------------------------------------------------------------------------
//! @brief 描画パラメータ
//---------------------------------------------------------------------------
struct DrawParameterArg
{
    int                             m_ProccesingIndex;        //!< 処理ID
    ViewParam*                      m_pViewParam;             //!< ビューパラメータ
    DrawViewFlag                    m_DrawViewFlag;           //!< 描画ビューフラグ
    ShaderType                      m_ShaderType;             //!< シェーダタイプ
    nn::gfx::DescriptorSlot         m_FrameBufferTexture;     //!< フレームバッファテクスチャ
    nn::gfx::DescriptorSlot         m_DepthBufferTexture;     //!< デプスバッファテクスチャ
    nn::gfx::GpuAddress*            m_pViewGpuAddress;        //!< ビュー GfxGpuアドレス
    TemporaryBuffer*                m_pDrawTempBuffer;        //!< GPU参照用のテンポラリバッファ
    detail::SortData*               m_pParticleSortBuffer;    //!< パーティクルソート用バッファの先頭アドレス
};

//---------------------------------------------------
//! @brief  エフェクト全体を管理するクラスです。
//!
//!         ユーザーはこのクラスを介してエミッタセットの生成/破棄、
//!         リソースの登録/破棄、エフェクトの計算/描画処理をおこないます。
//---------------------------------------------------
class System
{
    NN_DISALLOW_COPY( System );

public:
    //------------------------------------------------------------------------------
    //! @brief 互換性の目的で定義
    //! @details
    //! 本関数は互換性の目的で定義されました。 ディスクリプタスロット確保/開放関数のインタフェースです。
    //------------------------------------------------------------------------------
    typedef  nn::vfx::RegisterTextureViewSlot   RegisterTextureViewSlot;
    typedef  nn::vfx::UnregisterTextureViewSlot UnregisterTextureViewSlot;
    typedef  nn::vfx::RegisterSamplerSlot       RegisterSamplerSlot;
    typedef  nn::vfx::UnregisterSamplerSlot     UnregisterSamplerSlot;

    //----------------------------------------
    //! @brief vfx システムが生成するテクスチャービューをディスクリプタプールに登録します。
    //!
    //! @param[in]  pRegisterTextureSlotCallback    ディスクリプタスロット確保関数です。
    //! @param[in]  pUserData                       ユーザデータです。
    //----------------------------------------
    void RegisterTextureViewToDescriptorPool( nn::vfx::RegisterTextureViewSlot pRegisterTextureSlotCallback, void* pUserData ) NN_NOEXCEPT;

    //----------------------------------------
    //! @brief vfx システムが用意するテクスチャービューをディスクリプタプールから解放します。
    //!
    //! @param[in]  pUnregisterTextureSlotCallback  ディスクリプタスロット解放関数です。
    //! @param[in]  pUserData                       ユーザデータです。
    //----------------------------------------
    void UnregisterTextureViewFromDescriptorPool( nn::vfx::UnregisterTextureViewSlot pUnregisterTextureSlotCallback, void* pUserData ) NN_NOEXCEPT;

    //----------------------------------------
    //! @brief vfx システムが用意するサンプラーをディスクリプタプールに登録します。
    //!
    //! @param[in]  pRegisterSamplerSlot    ディスクリプタスロット確保関数です。
    //! @param[in]  pUserData               ユーザデータです。
    //----------------------------------------
    void RegisterSamplerToDescriptorPool( nn::vfx::RegisterSamplerSlot pRegisterSamplerSlot, void* pUserData ) NN_NOEXCEPT;

    //----------------------------------------
    //! @brief vfx システムが用意するサンプラーをディスクリプタプールから解放します。
    //!
    //! @param[in]  pUnregisterSamplerSlot  ディスクリプタスロット解放関数です。
    //! @param[in]  pUserData               ユーザデータです。
    //----------------------------------------
    void UnregisterSamplerFromDescriptorPool( nn::vfx::UnregisterSamplerSlot pUnregisterSamplerSlot, void* pUserData ) NN_NOEXCEPT;

    //@}

public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief      エフェクトシステムの生成を行います。
    //!             nn::vfx::config::SetEffectHeap(), nn::vfx::config::SetDynamicHeap() が
    //!             事前に呼び出されていない場合、アサートします。
    //!             システム初期化時に nn::vfx::config::m_pHeap からメモリ確保が行われ、
    //!             それ以降は、nn::vfx::config::m_pDynamicHeap からメモリ確保が行われます。
    //!
    //! @param[in]  config  エフェクトシステムを生成するためのコンフィグ。
    //! @see        nn::vfx::Config, nn::vfx::Heap
    //---------------------------------------------------------------------------
    explicit System( const Config& config ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エフェクトシステムの破棄を行います。
    //!
    //!             nn::vfx::System::ClearResource() が呼ばれていない未削除のエフェクトバイナリの
    //!             削除処理も同時に行いますが、その際、削除に用いる nn::vfx::Heap は、
    //!             nn::vfx::System::EntryResource() 時に利用された nn::vfx::Heap が利用されます。
    //---------------------------------------------------------------------------
    virtual ~System() NN_NOEXCEPT;

    //@}


    //----------------------------------------
    //! @name リソース管理
    //@{

    //---------------------------------------------------------------------------
    //! @brief      リソースを登録する。登録後もリソースはメモリ上に保持して下さい。
    //!
    //!             あらかじめユーザー側で準備されたメモリプールに配置されたバイナリと配置先のメモリプールを引数にとります。
    //!             渡されたメモリプールをもとに初期化される為、バイナリ内に含まれる テクスチャやシェーダバイナリ単位ではメモリプール生成が行われません。
    //!
    //! @param[in]  pHeap                           メモリ確保先のヒープ。
    //! @param[in]  pResource                       メモリ上のエフェクトバイナリ。128 Byte アラインに配置する必要があります。
    //! @param[in]  pMemoryPool                     メモリプール。エフェクトバイナリ(pBinary)をメモリプール内に配置した場合にそのメモリプールを指定します。
    //! @param[in]  memoryPoolSize                  配置先メモリプールのサイズ。
    //! @param[in]  memoryPoolOffset                メモリプール内オフセット。
    //! @param[in]  resourceId                      登録するリソース ID。指定できるIdの上限数は nn::vfx::Config で指定します。
    //! @param[in]  shaderDelaySetup                シェーダセットアップを利用する場合は、true を指定します。
    //! @param[in]  residentResource                共通で利用されるテクスチャを含んだリソース。
    //!
    //! @return     リソース登録に成功すれば true, できなれば false。
    //! @see        nn::vfx::System::ClearResource, nn::vfx::Heap, nn::vfx::Resource
    //---------------------------------------------------------------------------
    bool EntryResource( Heap* pHeap, void* pResource, nn::gfx::MemoryPool* pMemoryPool, size_t memoryPoolSize, size_t memoryPoolOffset, int resourceId, bool shaderDelaySetup, Resource* residentResource = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      リソースを登録する。登録後もリソースはメモリ上に保持して下さい。
    //!
    //!             バイナリ内に含まれる テクスチャやシェーダバイナリ単位でメモリプールが生成されます。
    //!
    //! @param[in]  pHeap                           メモリ確保先のヒープ。
    //! @param[in]  pResource                       メモリ上のエフェクトバイナリ。128 Byte アラインに配置する必要があります。
    //! @param[in]  resourceId                      登録するリソース ID。指定できるIdの上限数は nn::vfx::Config で指定します。
    //! @param[in]  shaderDelaySetup                シェーダセットアップを利用する場合は、true を指定します。
    //! @param[in]  residentResource                共通で利用されるテクスチャを含んだリソース。
    //!
    //! @return     リソース登録に成功すれば true, できなれば false。
    //! @see        nn::vfx::System::ClearResource, nn::vfx::Heap, nn::vfx::Resource
    //---------------------------------------------------------------------------
    bool EntryResource( Heap* pHeap, void* pResource, int resourceId, bool shaderDelaySetup, Resource* residentResource = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      指定 ID のリソースをエフェクトシステムから破棄します。
    //!
    //! @param[in]  pHeap         NULLの場合は、nn::vfx::System::EntryResource() で渡された nn::vfx::Heap からメモリ解放処理を行います。
    //! @param[in]  resourceId    破棄するリソース ID。
    //!
    //! @return     なし。
    //! @see        nn::vfx::System::EntryResource, nn::vfx::Heap
    //---------------------------------------------------------------------------
    void ClearResource( Heap* pHeap, int resourceId ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      指定 ID のリソースを取得します。
    //!
    //! @param[in]  resourceId    リソース ID。
    //!
    //! @return     エフェクトリソース。
    //! @see        nn::vfx::Resource
    //---------------------------------------------------------------------------
    Resource* GetResource( int resourceId ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( resourceId < m_ResourceCount );

        if( m_ResourceArray[ resourceId ] && m_ResourceArray[ resourceId ]->IsAlive()  )
        {
            return m_ResourceArray[ resourceId ];
        }

        return NULL;
    }

    //---------------------------------------------------------------------------
    //! @brief        指定IDのリソースを取得します。( const 版 )
    //!
    //! @param[in]    resourceId    リソースID。
    //!
    //! @return       エフェクトリソース。
    //! @see        nn::vfx::Resource
    //---------------------------------------------------------------------------
    const Resource* GetResource( int resourceId ) const NN_NOEXCEPT
    {
        if( m_ResourceArray[ resourceId ] && m_ResourceArray[ resourceId ]->IsAlive() )
        {
            return m_ResourceArray[ resourceId ];
        }

        return NULL;
    }

    //---------------------------------------------------------------------------
    //! @brief        指定IDのリソースをバイナリサイズ詳細をログ出力します。
    //!
    //! @param[in]    resourceId    リソースID。
    //---------------------------------------------------------------------------
    void OutputResourceInfo( int resourceId ) NN_NOEXCEPT
    {
        if( m_ResourceArray[ resourceId ] )
        {
            m_ResourceArray[ resourceId ]->OutputResourceInfo();
        }
    }

    //---------------------------------------------------------------------------
    //! @brief        リソースをアンリロケートします
    //!
    //! @details
    //!  登録したリソースIDでnn::vfx::System::ClearResource()を呼び出した後に
    //!  このメソッドを呼び出すことでリソースのメモリを移動可能な状態にすることができます。
    //! @see          nn::util::RelocationTable::Unrelocate()
    //!
    //! @param[in]    pData    再利用したいリソースへのポインタ。
    //---------------------------------------------------------------------------
    void Unrelocate( void* pData ) NN_NOEXCEPT
    {
        nn::vfx::Resource::Unrelocate( pData );
    }
    //@}

    //----------------------------------------
    //! @name エミッタセット生成・破棄
    //@{

private:
    //---------------------------------------------------------------------------
    //! @brief      指定 ID のエミッタセットを生成します。
    //!
    //! @param[out] pOutHandle          エフェクトハンドル。生成されたエミッタセットと紐付けられます。
    //! @param[in]  emitterSetId        生成するエミッタセットの ID
    //! @param[in]  resourceId          生成するエミッタセットを含むリソース ID
    //! @param[in]  groupId             生成されたエミッタセットが所属するグループ ID
    //! @param[in]  maxParticleCount    マニュアル放出を利用する場合の最大パーティクル数
    //! @param[in]  pHeap               ヒープへのポインタ
    //! @param[in]  delay               nn::vfx::System::BeginFrame() の時点でエミッタセットリストに追加する場合は true
    //!
    //! @return     1つでもエミッタが生成出来れば true, できなれば false。
    //---------------------------------------------------------------------------
    bool CreateEmitterSetId( Handle* pOutHandle, int emitterSetId, int resourceId, int groupId, int maxParticleCount, Heap* pHeap = NULL, bool delay = false ) NN_NOEXCEPT;

public:
    //---------------------------------------------------------------------------
    //! @brief      指定IDのエミッタセットを生成します。
    //!
    //! @param[out] pOutHandle          エフェクトハンドル。
    //! @param[in]  emitterSetId        生成するエミッタセットの ID。設定するエミッタセット ID は、nn::vfx::Resource::SearchEmitterSetId() で検索します。
    //! @param[in]  resourceId          生成するエミッタセットを含むリソース ID。
    //! @param[in]  groupId             グループID (0～63, 63はゲーム内ビューアと共用)。
    //! @param[in]  pHeap               ヒープへのポインタ
    //! @param[in]  delay               エミッタセットリストへの追加を nn::vfx::System::BeginFrame() のタイミングで行う。
    //!
    //! @return     1つでもエミッタが生成出来れば true, できなれば false。
    //---------------------------------------------------------------------------
    bool CreateEmitterSetId( Handle* pOutHandle, int emitterSetId, int resourceId, int groupId, Heap* pHeap = NULL, bool delay = false ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      指定IDのエミッタセットを放出します。
    //!
    //! @param[out] pOutHandle          エフェクトハンドル。
    //! @param[in]  pos                 エミッタを配置するワールド座標。
    //! @param[in]  emitterSetId        エミッタセットID。設定するエミッタセット IDは、nn::vfx::Resource::SearchEmitterSetId() で検索します。
    //! @param[in]  resourceId          リソースID。
    //! @param[in]  groupId             グループID (0～63, 63はゲーム内ビューアと共用)。
    //! @param[in]  emitterMask         エミッタを出すかどうかのビットフラグ。
    //! @param[in]  delay               エミッタセットリストへの追加を nn::vfx::System::BeginFrame() のタイミングで行う。
    //!
    //! @return     1つでもエミッタが生成出来れば true, できなれば false。
    //---------------------------------------------------------------------------
    bool CreateEmitterSetId( Handle* pOutHandle, const nn::util::Vector3fType& pos, int emitterSetId,
        int resourceId = 0, int groupId = 0, uint32_t emitterMask = 0xffffffff, bool delay = false ) NN_NOEXCEPT
    {
        NN_UNUSED( emitterMask );

        bool ret = CreateEmitterSetId( pOutHandle, emitterSetId, resourceId, groupId, NULL, delay );
        if( ret && pOutHandle->IsValid() )
        {
            nn::util::Matrix4x3fType matrix;
            nn::util::MatrixIdentity( &matrix );
            nn::util::MatrixSetAxisW( &matrix, pos );
            pOutHandle->GetEmitterSet()->SetMatrix( matrix );
            return true;
        }

        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief      指定 ID のエミッタセットを放出します。
    //!             引数の matrix にスケールが入っていた場合、表示が壊れます。
    //!
    //! @param[out] pOutHandle      エフェクトハンドル。
    //! @param[in]  matrix          ワールド行列。
    //! @param[in]  emitterSetId    エミッタセット ID。設定するエミッタセット ID は、nn::vfx::Resource::SearchEmitterSetId() で検索します。
    //! @param[in]  resourceId      リソースID。
    //! @param[in]  groupId         グループID (0～63, 63はゲーム内ビューアと共用)。
    //! @param[in]  emitterMask     エミッタを出すかどうかのビットフラグ。
    //! @param[in]  delay           エミッタセットリストへの追加を nn::vfx::System::BeginFrame() のタイミングで行う。
    //!
    //! @return     1つでもエミッタが生成出来れば true, できなれば false。
    //---------------------------------------------------------------------------
    bool CreateEmitterSetId( Handle* pOutHandle, const nn::util::Matrix4x3fType &matrix, int emitterSetId,
    int resourceId = 0, int groupId = 0, uint32_t emitterMask = 0xffffffff, bool delay = false ) NN_NOEXCEPT
    {
        NN_UNUSED( emitterMask );

        bool ret = CreateEmitterSetId( pOutHandle, emitterSetId, resourceId, groupId, NULL, delay );
        if( ret && pOutHandle->IsValid() )
        {
            pOutHandle->GetEmitterSet()->SetMatrix( matrix );
            return true;
        }

        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief      指定IDのエミッタセットをマニュアル放出状態で生成します。
    //!
    //!             アプリ側から放出タイミングを指定するモードでエミッタセットを生成します。
    //!             チャイルド付きエミッタセットはサポートしていません（※必ず生成に失敗します）
    //!
    //! @param[out] pOutHandle                  エフェクトハンドル。
    //! @param[in]  emitterSetId                エミッタセット ID。設定するエミッタセット ID は、nn::vfx::Resource::SearchEmitterSetId()で検索します。
    //! @param[in]  resourceId                  リソース ID。
    //! @param[in]  groupId                     グループ ID ( 0～(nn::vfx::SystemParameters_MaxGroupCount-1), (nn::vfx::SystemParameters_MaxGroupCount-1) はゲーム内ビューアと共用)。
    //! @param[in]  maxParticleCount            同時に存在できるパーティクルの最大数。
    //! @param[in]  maxEmitCountPerFrame        1フレーム中に放出予約可能なパーティクルの最大数。必ず ( maxParticleCount >= maxEmitCountPerFrame ) が満たされる。
    //! @param[in]  pEmitReservationListHead    パーティクル放出予約リストの先頭のアドレス（配列の要素数は必ず maxEmitCountPerFrame ）
    //! @param[in]  pHeap                       ヒープへのポインタ（任意）
    //! @param[in]  pCustomActionCallbackSet    カスタムアクションのコールバックセットへのポインタ（任意）
    //! @param[in]  residentEmitterTime         ( 0 )以上を指定した場合、この期間以上経過した後にパーティクル数が0になるとエミッタが終了します。( -1 ) 指定でエミッタは終了せずに常駐します。
    //!
    //! @return     エミッタが生成出来れば true, できなれば false。
    //---------------------------------------------------------------------------
    bool CreateManualEmitterSetId( Handle* pOutHandle, int emitterSetId, int resourceId, int groupId,
        int maxParticleCount, int maxEmitCountPerFrame, EmitReservationInfo* pEmitReservationListHead,
        Heap* pHeap = NULL, CallbackSet* pCustomActionCallbackSet = NULL, int residentEmitterTime = -1 ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      引数のエミッタセットを削除します。
    //!
    //! @param[in]  pEmitterSet     エミッタセットインスタンス。
    //! @param[in]  immediate       即時削除するか、false の場合は、nn::vfx::System::BeginFrame() のタイミングで削除されます。
    //---------------------------------------------------------------------------
    void KillEmitterSet( EmitterSet* pEmitterSet, bool immediate = true ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      引数名のエミッタセットを削除します。
    //!
    //! @param[in]  emitterSetName  エミッタセット名。
    //! @param[in]  resourceId      リソースID。
    //---------------------------------------------------------------------------
    void KillEmitterSet( const char* emitterSetName, int resourceId ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate               指定エミッタセットを再生成します。
    //! @param[in]  resourceId      リソース ID
    //! @param[in]  emitterSetId    エミッタセット ID
    //! @return                     １つでもエミッタが生成出来れば true, できなれば false。
    //---------------------------------------------------------------------------
    bool ReCreateEmitterSet( int resourceId, int emitterSetId ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate               指定エミッタセットを再生成します。
    //! @param[in]  emitterSetName  エミッタセット名
    //! @param[in]  oldResId        旧リソース ID
    //! @param[in]  newResId        新リソース ID
    //---------------------------------------------------------------------------
    void RecreateEmitterSet2( const char* emitterSetName, int oldResId, int newResId ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  再生中の全てのエミッタを削除します。
    //---------------------------------------------------------------------------
    void KillAllEmitter() NN_NOEXCEPT
    {
        KillAllEmitterSet();
    }

    //---------------------------------------------------------------------------
    //! @brief  再生中の全てのエミッタセットを削除します。
    //---------------------------------------------------------------------------
    void KillAllEmitterSet() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      指定グループに所属するエミッタを削除します。
    //!
    //! @param[in]  groupId   グループ ID。
    //---------------------------------------------------------------------------
    void KillEmitterGroup( int groupId ) NN_NOEXCEPT
    {
        return KillEmitterSetGroup( groupId );
    }

    //---------------------------------------------------------------------------
    //! @brief      指定グループに所属するエミッタを削除します。
    //!
    //! @param[in]  groupId   グループID。
    //---------------------------------------------------------------------------
    void KillEmitterSetGroup( int groupId ) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 計算処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief  内部で確保されたメモリの遅延解放やカウンタ等のリセットを行います。
    //!
    //!         計算処理・描画処理にかかわらず、毎フレーム呼び出してください。
    //!         エミッタセットの遅延リスト追加、遅延リスト削除はこのタイミングで行われます。
    //---------------------------------------------------------------------------
    void BeginFrame() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      計算処理を行います。
    //!
    //! @param[in]  pEmitterSet             計算処理を行うエミッタセット
    //! @param[in]  frameRate               フレームレート( 標準は 1.0f )
    //! @param[in]  bufferSwapMode          バッファバッファをスワップモード( 標準は auto )
    //! @return メソッド内でエミッタセットの生存が存続した場合は true, 削除された場合は false。
    //---------------------------------------------------------------------------
    bool Calculate( EmitterSet* pEmitterSet, float frameRate, BufferSwapMode bufferSwapMode = BufferSwapMode_Auto ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      計算処理を行います。
    //!
    //! @param[in]  groupId                 計算処理を行うグループ ID
    //! @param[in]  frameRate               フレームレート( 標準は 1.0f )
    //! @param[in]  bufferSwapMode          バッファバッファをスワップモード( 標準は auto )
    //---------------------------------------------------------------------------
    void Calculate( int groupId, float frameRate, BufferSwapMode bufferSwapMode = BufferSwapMode_Auto ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      計算処理を行います。
    //!
    //! @param[in]  groupId                 計算処理を行うグループ ID
    //! @param[in]  frameRate               フレームレート( 標準は 1.0f )
    //! @param[in]  doSwapBuffer            バッファバッファをスワップするかどうか
    //---------------------------------------------------------------------------
    void Calculate( int groupId, float frameRate, bool doSwapBuffer ) NN_NOEXCEPT
    {
        BufferSwapMode swapMode = BufferSwapMode_None;
        if ( doSwapBuffer )
        {
            swapMode = BufferSwapMode_Swap;
        }
        Calculate( groupId, frameRate, swapMode );
    }

    //@}

    //----------------------------------------
    //! @name 描画処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief      描画用テンポラリバッファのスワップを行います。
    //!
    //!             描画処理内でダブルバッファにコンスタントバッファを書き込むので、描画処理より前に呼び出してください。
    //---------------------------------------------------------------------------
    void SwapBuffer() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      フレームバッファテクスチャを設定します。屈折パーティクルで利用されます。
    //! @param[in]  processingIndex     処理インデックス。nn::vfx::Config で指定されたインデックス数までが利用可能です。
    //! @param[in]  frameBufferTexture  フレームバッファテクスチャ デスクリプタスロット。
    //---------------------------------------------------------------------------
    void SetFrameBufferTexture( int processingIndex, nn::gfx::DescriptorSlot frameBufferTexture ) NN_NOEXCEPT
    {
        if ( processingIndex >= m_ProcessingCountMax ) return;
        m_ProcessingDrawParameter[ processingIndex ].m_FrameBufferTexture = frameBufferTexture;
    }

    //---------------------------------------------------------------------------
    //! @brief      デプスバッファテクスチャを設定します。ソフトパーティクル処理などで利用されます。
    //! @param[in]  processingIndex     処理インデックス。nn::vfx::Config で指定されたインデックス数までが利用可能です。
    //! @param[in]  depthBufferTexture  デプスバッファテクスチャ デスクリプタスロット
    //---------------------------------------------------------------------------
    void SetDepthBufferTexture( int processingIndex, nn::gfx::DescriptorSlot depthBufferTexture ) NN_NOEXCEPT
    {
        if ( processingIndex >= m_ProcessingCountMax ) return;
        m_ProcessingDrawParameter[ processingIndex ].m_DepthBufferTexture = depthBufferTexture;
    }

    //---------------------------------------------------------------------------
    //! @brief      設定済みフレームバッファテクスチャを無効化します。
    //! @param[in]  processingIndex     処理インデックス。nn::vfx::Configで指定されたインデックス数までが利用可能です。
    //---------------------------------------------------------------------------
    void InvalidateFrameBufferTexture( int processingIndex ) NN_NOEXCEPT
    {
        if ( processingIndex >= m_ProcessingCountMax ) return;
        m_ProcessingDrawParameter[ processingIndex ].m_FrameBufferTexture.Invalidate();
    }

    //---------------------------------------------------------------------------
    //! @brief      設定済みデプスバッファテクスチャを無効化します。
    //! @param[in]  processingIndex     処理インデックス。nn::vfx::Configで指定されたインデックス数までが利用可能です。
    //---------------------------------------------------------------------------
    void InvalidateDepthBufferTexture( int processingIndex ) NN_NOEXCEPT
    {
        if ( processingIndex >= m_ProcessingCountMax ) return;
        m_ProcessingDrawParameter[ processingIndex ].m_DepthBufferTexture.Invalidate();
    }

    //---------------------------------------------------------------------------
    //! @brief      描画ビューフラグを設定する。
    //! @param[in]  processingIndex     処理インデックス。nn::vfx::Config で指定されたインデックス数までが利用可能です。
    //! @param[in]  flag                描画ビューフラグ
    //---------------------------------------------------------------------------
    bool SetDrawViewFlag( int processingIndex, DrawViewFlag flag ) NN_NOEXCEPT
    {
        if ( processingIndex >= m_ProcessingCountMax ) return false;
        m_ProcessingDrawParameter[ processingIndex ].m_DrawViewFlag = flag;
        return true;
    }

    //---------------------------------------------------------------------------
    //! @brief      ビュービューパラメータの設定を行います。
    //!
    //!             定数バッファモード時は、このメソッド内でビュー定数バッファを設定(コマンド発行)します。
    //! @param[in]  processingIndex     処理インデックス。nn::vfx::Config で指定されたインデックス数までが利用可能です。
    //! @param[in]  pViewParam          ビューパラメータ
    //---------------------------------------------------------------------------
    void SetViewParam( int processingIndex, ViewParam* pViewParam ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      ビュービューパラメータの設定を行います。
    //!
    //!             定数バッファモード時は、このメソッド内でビュー定数バッファを設定(コマンド発行)します。
    //! @param[in]  processingIndex     処理インデックス。nn::vfx::Config で指定されたインデックス数までが利用可能です。
    //! @param[in]  pCommandBuffer      コマンドバッファへのポインタ。
    //! @param[in]  pViewParam          ビューパラメータ
    //---------------------------------------------------------------------------
    void SetViewParam( int processingIndex, nn::gfx::CommandBuffer* pCommandBuffer, ViewParam* pViewParam ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      シェーダタイプを切り替えます。
    //! @param[in]  processingIndex     処理インデックス。nn::vfx::Config で指定されたインデックス数までが利用可能です。
    //! @param[in]  type                シェーダタイプ
    //---------------------------------------------------------------------------
    bool SetShaderType( int processingIndex, ShaderType type ) NN_NOEXCEPT
    {
        if ( processingIndex >= m_ProcessingCountMax ) return false;
        if ( type >= ShaderType_MaxShaderType ) return false;
        m_ProcessingDrawParameter[ processingIndex ].m_CurrentShaderType = type;
        return true;
    }

    //---------------------------------------------------------------------------
    //! @brief      描画処理を行います。
    //! @param[in]  processingIndex         処理インデックス。nn::vfx::Config で指定されたインデックス数までが利用可能です。
    //! @param[in]  pCommandBuffer          コマンドバッファへのポインタ。
    //! @param[in]  groupId                 描画を行うグループ ID。
    //! @param[in]  drawPathFlag            描画パスフラグ。
    //! @param[in]  sort                    EmittetSet の Z ソートを行うかどうか。
    //! @param[in]  doComputeShaderProcess  コンピュートシェーダエミッタを計算させるかどうか。
    //! @param[in]  pUserParam              ユーザーパラメータへのポインタ。
    //---------------------------------------------------------------------------
    void Draw( int processingIndex, nn::gfx::CommandBuffer* pCommandBuffer, int groupId, uint32_t drawPathFlag = 0xFFFFFFFF, bool sort = false, bool doComputeShaderProcess = true, void* pUserParam = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      指定グループ ID のエミッタセットをソートバッファに追加します。
    //!
    //!             drawPathFlag で指定するフラグは、論理和で複数指定できます。
    //!             nn::vfx::System::AddSortBuffer() はソートバッファに積むだけの処理しか行いません。
    //!             実際の描画は、nn::vfx::System::DrawSortBuffer() をで行われます。
    //!
    //! @param[in]  processingIndex         処理インデックス。nn::vfx::Config で指定されたインデックス数までが利用可能です。
    //! @param[in]  groupId                 描画を行うグループ ID。
    //! @param[in]  drawPathFlag            描画パスフラグ。nn::vfx::DrawPathFlag で指定してください。
    //!
    //! @see        nn::vfx::System::DrawSortBuffer()
    //---------------------------------------------------------------------------
    void AddSortBuffer( int processingIndex, int groupId, uint32_t drawPathFlag = 0xFFFFFFFF ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      ソートバッファの描画処理を行います。
    //!             nn::vfx::System::AddSortBufferで追加されたグループに所属するエミッタセットをビュー方向にZソートして描画します。
    //!
    //! @param[in]  processingIndex         処理インデックス。nn::vfx::Config で指定されたインデックス数までが利用可能です。
    //! @param[in]  pCommandBuffer          コマンドバッファへのポインタ。
    //! @param[in]  doComputeShaderProcess  コンピュートシェーダを計算させるかどうか。
    //! @param[in]  pUserParam              カスタムシェーダへ渡すアドレス。
    //!
    //! @see        nn::vfx::System::AddSortBuffer()
    //---------------------------------------------------------------------------
    void DrawSortBuffer( int processingIndex, nn::gfx::CommandBuffer* pCommandBuffer, bool doComputeShaderProcess = true, void* pUserParam = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      描画処理単位ごとの描画パラメータを取得します。
    //!             呼び出し時にシステム内描画パラメータの更新処理も行われます。
    //!
    //! @param[in]  processingIndex     処理インデックス。nn::vfx::Configで指定されたインデックス数までが利用可能です。
    //! @return     描画パラメータへのポインタ
    //---------------------------------------------------------------------------
    DrawParameterArg* GetDrawParameterArg( int processingIndex ) NN_NOEXCEPT
    {
        if ( processingIndex >= m_ProcessingCountMax )
        {
            return NULL;
        }

        // 最新の状態に更新する
        m_DrawParameterArg[ processingIndex ].m_DrawViewFlag       = m_ProcessingDrawParameter[ processingIndex ].m_DrawViewFlag;
        m_DrawParameterArg[ processingIndex ].m_ShaderType         = m_ProcessingDrawParameter[ processingIndex ].m_CurrentShaderType;
        m_DrawParameterArg[ processingIndex ].m_FrameBufferTexture = m_ProcessingDrawParameter[ processingIndex ].m_FrameBufferTexture;
        m_DrawParameterArg[ processingIndex ].m_DepthBufferTexture = m_ProcessingDrawParameter[ processingIndex ].m_DepthBufferTexture;

        if ( m_ProcessingDrawParameter[ processingIndex ].m_IsViewConstantBufferSetuped )
        {
            m_DrawParameterArg[ processingIndex ].m_pViewGpuAddress = nullptr;
        }
        else
        {
            m_DrawParameterArg[ processingIndex ].m_pViewGpuAddress = &m_ProcessingDrawParameter[ processingIndex ].m_ViewGpuAddress;
        }

        return &m_DrawParameterArg[ processingIndex ];
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタ描画プロファイラを設定します。
    //! @param[in]  profileCallback プロファイラコールバック関数ポインタ
    //! @see        nn::vfx::DrawEmitterProfileCallback
    //---------------------------------------------------------------------------
    void SetEmitterDrawProfilerCallback( DrawEmitterProfileCallback profileCallback ) NN_NOEXCEPT
    {
        m_EmitterDrawProfilerCallback = profileCallback;
    }

    //@}

    //----------------------------------------
    //! @name テンポラリバッファ
    //@{

    //---------------------------------------------------------------------------
    //! @brief      テンポラリバッファからメモリを確保します。
    //! @param[in]  processingIndex     処理インデックス
    //! @param[in]  address             確保したバッファの GPU アドレス
    //! @param[in]  size                確保するサイズ。
    //! @return     確保したバッファの先頭アドレス
    //---------------------------------------------------------------------------
    void* AllocFromTempBuffer( int processingIndex, nn::gfx::GpuAddress* address, size_t size ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      テンポラリバッファを取得します。
    //! @param[in]  processingIndex     処理インデックス。nn::vfx::Config で指定されたインデックス数までが利用可能です。
    //! @return     テンポラリバッファへのポインタ
    //---------------------------------------------------------------------------
    TemporaryBuffer* GetTemporaryBuffer( int processingIndex ) NN_NOEXCEPT
    {
        if( processingIndex >= m_ProcessingCountMax )
        {
            return NULL;
        }
        return &m_ProcessingDrawParameter[ processingIndex ].m_DrawTempBuffer;
    }

    //---------------------------------------------------------------------------
    //! @brief      テンポラリバッファのCPUキャッシュフラッシュを行います。
    //---------------------------------------------------------------------------
    void FlushTempBuffer() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name エミッタセット ID / 名前 の検索
    //@{

    //---------------------------------------------------------------------------
    //! @brief      エミッタセット ID を検索します。
    //!
    //! @param[in]  emitterSetName    検索するエミッタセット名。
    //! @param[in]  resourceId        検索対象となるリソース ID。
    //!
    //! @return     検索したエミッタセット ID。見つけられなった場合は nn::vfx::InvalidValueId::InvalidValueId_EmitterSetId が返ります。
    //---------------------------------------------------------------------------
    int SearchEmitterSetId( const char* emitterSetName, int resourceId = 0 ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      エミッタセット名を検索します。
    //!
    //! @param[in]  emitterSetId 検索するエミッタセット ID。
    //! @param[in]  resourceId   検索対象となるリソース ID。
    //!
    //! @return     検索したエミッタセット名。見つけられなかった場合は、NULL が返ります。
    //---------------------------------------------------------------------------
    const char* SearchEmitterSetName( int emitterSetId, int resourceId = 0 ) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name コンピュートシェーダ処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief      ストリームアウトエミッタを一時リストに追加します。
    //!
    //! @param[in]  pEmitter    追加するエミッタのポインタ
    //---------------------------------------------------------------------------
    void AddComputeShaderEmitterList( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      ストリームアウトエミッタリストを処理します。
    //!
    //! @param[in]  pCommandBuffer      コマンドバッファへのポインタ。
    //!
    //!                                 nn::vfx::Config::SetEnableComputeShaderBatchProcess() でストリームアウト処理のバッチ処理モードが有効の場合、
    //!                                 本メソッドを一度だけ呼び出してストリームアウト処理を行う必要があります。
    //!
    //! @param[in]  pUserParam          ユーザーパラメータ
    //! @param[in]  processEmitterFlag  処理するストリームアウトエミッタのフラグ
    //---------------------------------------------------------------------------
    void BatchCalculationComputeShaderEmitter( nn::gfx::CommandBuffer* pCommandBuffer, void* pUserParam, uint32_t processEmitterFlag = 0xffffffff ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      コンピュートシェーダエミッタを一括処理するかどうかを取得します。
    //!
    //! @return     一括処理する場合 true
    //---------------------------------------------------------------------------
    bool IsBatchProcessComputeShaderEmitter() const NN_NOEXCEPT
    {
        return m_IsBatchProcessComputeShaderEmitter;
    }

    //@}

    //----------------------------------------
    //! @name GPUキャッシュフラッシュ
    //@{

    //---------------------------------------------------------------------------
    //! @brief        GPUキャッシュのフラッシュを行います。
    //!
    //!               必要であればGPU実行前に呼び出してください。
    //---------------------------------------------------------------------------
    void FlushGpuCache() NN_NOEXCEPT;

    //@}


    //----------------------------------------
    //! @name エミッタセット初期化・破棄時 コールバック処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief        エミッタセット初期化コールバックを設定します。
    //! @param[in] callback エミッタセット初期化コールバック関数へのポインタ
    //---------------------------------------------------------------------------
    void SetEmitterSetInitializeCallback( EmitterSetInitializeCallback callback ) NN_NOEXCEPT
    {
        m_EmitterSetInitializeCallback = callback;
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタセット破棄時コールバックを設定します。
    //! @param[in] callback 描画パスコールバック関数へのポインタ
    //---------------------------------------------------------------------------
    void SetEmitterSetFinalizeCallback( EmitterSetFinalizeCallback callback ) NN_NOEXCEPT
    {
        m_EmitterSetFinalizeCallback = callback;
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタセット初期化コールバック関数を取得します。
    //! @return エミッタセット初期化コールバック関数へのポインタ
    //---------------------------------------------------------------------------
    EmitterSetInitializeCallback GetEmitterSetInitializeCallback() NN_NOEXCEPT
    {
        return m_EmitterSetInitializeCallback;
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタセット破棄時コールバック関数を取得します。
    //! @return エミッタセット破棄時コールバック関数へのポインタ
    //---------------------------------------------------------------------------
    EmitterSetFinalizeCallback GetEmitterSetFinalizeCallback() NN_NOEXCEPT
    {
        return m_EmitterSetFinalizeCallback;
    }

    //@}


    //----------------------------------------
    //! @name 描画パス コールバック処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief        描画パス 描画設定コールバックを設定します。
    //! @param[in] id       コールバックID
    //! @param[in] drawPathFlag     描画パスフラグ
    //! @param[in] callback 描画パスコールバック関数へのポインタ
    //---------------------------------------------------------------------------
    void SetDrawPathRenderStateSetCallback(
        DrawPathCallbackId             id,
        DrawPathFlag                   drawPathFlag,
        DrawPathRenderStateSetCallback callback ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        描画パス 描画設定コールバックを設定します。
    //! @param[in] drawPathFlag     描画パスフラグ
    //! @param[in] callback 描画パスコールバック関数へのポインタ
    //---------------------------------------------------------------------------
    void SetDrawPathRenderStateSetCallback( DrawPathFlag drawPathFlag, DrawPathRenderStateSetCallback callback ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        描画パス 描画設定コールバックを取得します。
    //! @param[in] drawPathFlag     描画パスフラグ
    //! @return 描画パスコールバック関数へのポインタ
    //---------------------------------------------------------------------------
    DrawPathRenderStateSetCallback GetDrawPathRenderStateSetCallback( DrawPathFlag drawPathFlag ) NN_NOEXCEPT;

    //@}


    //----------------------------------------
    //! @name LOD / カリング コールバック処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief      エミッタ計算処理カリングコールバックを設定します。
    //!
    //! @param[in]  callback    エミッタカリングコールバック関数へのポインタ
    //---------------------------------------------------------------------------
    void SetEmitterCalculateLodCallback( EmitterCalculateLodCallback callback ) NN_NOEXCEPT
    {
        m_EmitterCalculateLodCallback = callback;
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタ描画処理カリングコールバックを設定します。
    //!
    //! @param[in]  callback     エミッタカリングコールバック関数へのポインタ
    //---------------------------------------------------------------------------
    void SetEmitterDrawCullingCallback( EmitterDrawCullingCallback callback ) NN_NOEXCEPT
    {
        m_EmitterDrawCullingCallback = callback;
    }

    //@}


    //----------------------------------------
    //! @name カスタムフィールド コールバック処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief      カスタムフィールドコールバックを設定します。
    //!
    //! @param[in]  callback    カスタムフィールドコールバック関数へのポインタ
    //---------------------------------------------------------------------------
    void SetCustomFieldCallback( CustomFieldCallback callback ) NN_NOEXCEPT
    {
        m_CustomFieldCallback = callback;
    }

    //---------------------------------------------------------------------------
    //! @brief      カスタムフィールドコールバックを取得します。
    //!
    //! @return     カスタムフィールドコールバック関数へのポインタ
    //---------------------------------------------------------------------------
    CustomFieldCallback GetCustomFieldCallback() const NN_NOEXCEPT
    {
        return m_CustomFieldCallback;
    }

    //@}


    //----------------------------------------
    //! @name カスタムアクション / カスタムシェーダ
    //@{

    //---------------------------------------------------------------------------
    //! @brief      コールバックセットを設定します。
    //!
    //! @param[in] id   セットするコールバックID
    //! @param[in] callbackSet  コールバックセット
    //---------------------------------------------------------------------------
    void SetCallback( CallbackId id, CallbackSet callbackSet ) NN_NOEXCEPT
    {
        m_IsEnableCallback[ id ] = true;
        m_Callback[ id ] = callbackSet;
        m_Callback[ id ].endianFlip = EndianFlipCallbackImpl;
    }

    //---------------------------------------------------------------------------
    //! @brief      指定IDのコールバックセットが有効か取得します。
    //!
    //! @param[in]  id  コールバックID
    //! @return     trueで有効、falseで無効
    //---------------------------------------------------------------------------
    bool IsEnabelCallbackSet( CallbackId id ) const NN_NOEXCEPT
    {
        return m_IsEnableCallback[ id ];
    }

    //---------------------------------------------------------------------------
    //! @brief      コールバックセットを取得します。
    //!
    //! @param[in]  id  コールバックID
    //! @return     コールバックセット
    //---------------------------------------------------------------------------
    CallbackSet* GetCallbackSet( CallbackId id ) NN_NOEXCEPT
    {
        return &m_Callback[ id ];
    }

    //---------------------------------------------------------------------------
    //! @brief      カスタムシェーダテクスチャをバインドします。
    //!
    //! @param[in]  processingIndex 処理インデックス。
    //! @param[in]  pCommandBuffer コマンドバッファへのポインタ。
    //! @param[in]  customShaderType      カスタムシェーダテクスチャタイプ
    //! @param[in]  pTextureView     テクスチャ
    //! @param[in]  pSampler     サンプラ
    //! @return true で設定成功、falseで設定失敗
    //---------------------------------------------------------------------------
    bool BindCustomShaderTexture( int processingIndex, nn::gfx::CommandBuffer* pCommandBuffer, CustomShaderTextureType customShaderType, nn::gfx::TextureView* pTextureView, nn::gfx::Sampler* pSampler ) NN_NOEXCEPT
    {
        if( !m_ProcessingDrawParameter[ processingIndex ].m_pCurrentCustomShader )
        {
            return false;
        }

        int vertexLoc = m_ProcessingDrawParameter[ processingIndex ].m_pCurrentCustomShader->GetCustomTextureVertexSamplerLocation( customShaderType );
        int pixelLoc  = m_ProcessingDrawParameter[ processingIndex ].m_pCurrentCustomShader->GetCustomTexturePixelSamplerLocation( customShaderType );

        if( vertexLoc != InvalidValueId_TextureSamplerSlotId )
        {
            pCommandBuffer->SetTextureAndSampler( vertexLoc, nn::gfx::ShaderStage_Vertex, pTextureView, pSampler );
        }
        if( pixelLoc != InvalidValueId_TextureSamplerSlotId )
        {
            pCommandBuffer->SetTextureAndSampler( pixelLoc, nn::gfx::ShaderStage_Pixel, pTextureView, pSampler );
        }

        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief      カスタムシェーダテクスチャをバインドします。
    //!
    //! @param[in]  processingIndex       処理インデックス。Configで指定されたインデックス数までが利用可能です。
    //! @param[in]  pCommandBuffer        コマンドバッファへのポインタ。
    //! @param[in]  customShaderType      カスタムシェーダテクスチャタイプ
    //! @param[in]  textureSlot           テクスチャ ディスクリプタスロット
    //! @return true で設定成功、falseで設定失敗
    //---------------------------------------------------------------------------
    bool BindCustomShaderTexture( int processingIndex, nn::gfx::CommandBuffer* pCommandBuffer, CustomShaderTextureType customShaderType, nn::gfx::DescriptorSlot textureSlot ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      カスタムシェーダテクスチャをバインドします。
    //!
    //! @param[in]  processingIndex       処理インデックス。Configで指定されたインデックス数までが利用可能です。
    //! @param[in]  pCommandBuffer        コマンドバッファへのポインタ。
    //! @param[in]  customShaderType      カスタムシェーダテクスチャタイプ
    //! @param[in]  textureSlot           テクスチャ ディスクリプタスロット
    //! @param[in]  samplerSlot           サンプラ ディスクリプタスロット
    //! @return true で設定成功、falseで設定失敗
    //---------------------------------------------------------------------------
    bool BindCustomShaderTexture( int processingIndex, nn::gfx::CommandBuffer* pCommandBuffer, CustomShaderTextureType customShaderType, nn::gfx::DescriptorSlot textureSlot, nn::gfx::DescriptorSlot samplerSlot ) NN_NOEXCEPT
    {
        if( !m_ProcessingDrawParameter[ processingIndex ].m_pCurrentCustomShader )
        {
            return false;
        }

        int vertexLoc = m_ProcessingDrawParameter[ processingIndex ].m_pCurrentCustomShader->GetCustomTextureVertexSamplerLocation( customShaderType );
        int pixelLoc  = m_ProcessingDrawParameter[ processingIndex ].m_pCurrentCustomShader->GetCustomTexturePixelSamplerLocation( customShaderType );

        if( vertexLoc != InvalidValueId_TextureSamplerSlotId )
        {
            pCommandBuffer->SetTextureAndSampler( vertexLoc, nn::gfx::ShaderStage_Vertex, textureSlot, samplerSlot );
        }
        if( pixelLoc != InvalidValueId_TextureSamplerSlotId )
        {
            pCommandBuffer->SetTextureAndSampler( pixelLoc, nn::gfx::ShaderStage_Pixel, textureSlot, samplerSlot );
        }

        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief        各エミッタ描画で共通のカスタムシェーダ定数バッファを設定します。
    //!
    //!               ここで設定された定数バッファは、SetViewParamタイミングで設定(コマンド発行)されます。
    //!               SetViewParamが呼び出されるまでは、渡した定数バッファバッファをユーザー側で保持するようにしてください。
    //! @param[in]    id                ID
    //! @param[in]    pConstantBuffer      定数バッファへのポインタ
    //! @param[in]    constantBufferSize  定数バッファのサイズ
    //---------------------------------------------------------------------------
    void SetCommonCustomShaderConstantBuffer( CustomShaderConstantBufferIndex id, void* pConstantBuffer, size_t constantBufferSize ) NN_NOEXCEPT
    {
        m_CommonCustomShaderConstantBuffer[ id ] = pConstantBuffer;
        m_CommonCustomShaderConstantBufferSize[ id ] = constantBufferSize;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate   エミッタプラグインコールバックセットを設定します。
    //!
    //! @param[in] id   エミッタプラグインID
    //! @param[in] callbackSet  コールバックセット
    //---------------------------------------------------------------------------
    void SetEmitterPluginCallbackSet( detail::EmitterPluginCallbackIndex id, CallbackSet callbackSet ) NN_NOEXCEPT
    {
        m_EmitterPluginCallback[ id ] = callbackSet;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate   エミッタプラグインコールバックセットを取得します。
    //!
    //! @param[in] id   エミッタプラグインID
    //! @return         コールバックセット
    //---------------------------------------------------------------------------
    CallbackSet* GetEmitterPluginCallbackSet( detail::EmitterPluginCallbackIndex id ) NN_NOEXCEPT
    {
        return &m_EmitterPluginCallback[ id ];
    }

    //---------------------------------------------------------------------------
    //! @brief                      描画前コールバック（RenderStateCallback、DrawPathCallback）を呼び出します。
    //! @param[in] pCommandBuffer       コマンドバッファ
    //! @param[in] pEmitter             エミッタへのポインタ
    //! @param[in] shaderType           シェーダタイプ
    //! @param[in] pUserParam           ユーザーパラメータへのポインタ
    //! @param[in] pDrawParameterArg    描画パラメータへのポインタ
    //! @return                     成功した場合trueを返す。
    //---------------------------------------------------------------------------
    bool InvokeBeforeRenderCallbacks( nn::gfx::CommandBuffer* pCommandBuffer, Emitter* pEmitter, ShaderType shaderType, void* pUserParam, DrawParameterArg* pDrawParameterArg ) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 情報取得
    //@{

    //---------------------------------------------------------------------------
    //! @briefprivate       Gfxデバイスを取得します。
    //!                     消去予定
    //!
    //! @return             Gfxデバイス
    //---------------------------------------------------------------------------
    nn::gfx::Device* GetGfxDevice() NN_NOEXCEPT
    {
        return m_pDevice;
    }

    //---------------------------------------------------------------------------
    //! @brief      静的ヒープを取得します。
    //!
    //! @return     静的ヒープ。
    //---------------------------------------------------------------------------
    Heap* GetStaticHeap() NN_NOEXCEPT
    {
        return &m_StaticHeap;
    }

    //---------------------------------------------------------------------------
    //! @brief      nn::vfx::System が内部で持つ最大リソース数を取得します。
    //!
    //! @return     nn::vfx::System が内部で持つ最大リソース数
    //---------------------------------------------------------------------------
    int GetResourceCountMax() const NN_NOEXCEPT
    {
        return m_ResourceCount;
    }

    //---------------------------------------------------------------------------
    //! @brief      nn::vfx::Config でユーザーが設定した最大リソース数を取得します。
    //!
    //! @return     nn::vfx::Config でユーザーが設定した最大リソース数
    //---------------------------------------------------------------------------
    int GetUserResourceCountMax() const NN_NOEXCEPT
    {
        return m_UserResourceCount;
    }

    //---------------------------------------------------------------------------
    //! @brief      処理可能な最大エミッタセット数を取得します。
    //!
    //! @return     最大エミッタセット数
    //---------------------------------------------------------------------------
    int GetEmitterSetCountMax() const NN_NOEXCEPT
    {
        return m_EmitterSetCount;
    }

    //---------------------------------------------------------------------------
    //! @brief      処理可能な最大エミッタ数を取得します。
    //!
    //! @return     最大エミッタ数
    //---------------------------------------------------------------------------
    int GetEmitterCountMax() const NN_NOEXCEPT
    {
        return m_EmitterCount;
    }

    //---------------------------------------------------------------------------
    //! @brief      処理可能な 履歴式ストライプ の最大数を取得します。
    //!
    //! @return     最大エミッタ数
    //---------------------------------------------------------------------------
    int GetStripeCountMax() const NN_NOEXCEPT
    {
        return m_StripeCountMax;
    }

    //---------------------------------------------------------------------------
    //! @brief      処理可能な 履歴式ストライプ2 の最大数を取得します。
    //!
    //! @return     最大エミッタ数
    //---------------------------------------------------------------------------
    int GetSuperStripeCountMax() const NN_NOEXCEPT
    {
        return m_SuperStripeCountMax;
    }

    //---------------------------------------------------------------------------
    //! @brief      空きエミッタ数を取得します。
    //!
    //! @return     空きエミッタ数
    //---------------------------------------------------------------------------
    int GetFreeEmitterCount() const NN_NOEXCEPT
    {
        return ( GetEmitterCountMax() - GetProcessingEmitterCount() );
    }

    //---------------------------------------------------------------------------
    //! @brief      空きエミッタ数を取得します。
    //!
    //! @return     空きエミッタ数
    //---------------------------------------------------------------------------
    int GetFreeEmitterActualCount() const NN_NOEXCEPT
    {
        return m_FreeEmitterCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  処理中のエミッタセット数を取得します。
    //!
    //! @return 処理中のエミッタセット数
    //---------------------------------------------------------------------------
    int GetProcessingEmitterSetCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.emitterSetCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  処理中のエミッタ数を取得します。
    //!
    //! @return 処理中のエミッタ数
    //---------------------------------------------------------------------------
    int GetProcessingEmitterCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.emitterCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  計算処理をスキップされたのエミッタ数を取得します。
    //!
    //! @return 計算処理をスキップされたのエミッタ数
    //---------------------------------------------------------------------------
    int GetEmitterCountSkippedCalculation() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.emitterCountSkippedCalculation;
    }

    //---------------------------------------------------------------------------
    //! @brief  処理中のCPUパーティクル数を取得します。
    //!
    //! @return 処理中のCPUパーティクル数
    //---------------------------------------------------------------------------
    int GetProcessingCpuParticleCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.cpuParticleCount;
    }

    //---------------------------------------------------------------------------
    //! @brief 処理中のGPUパーティクル数を取得します。
    //!
    //! @return 処理中のGPUパーティクル数
    //---------------------------------------------------------------------------
    int GetProcessingGpuParticleCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.gpuParticleCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  処理中のコンピュートシェーダパーティクル数を取得します。
    //!
    //! @return 処理中のコンピュートシェーダパーティクル数
    //---------------------------------------------------------------------------
    int GetProcessingGpuSoParticleCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.gpusoParticleCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  処理中のエミッタ時間アニメーション数を取得します。
    //!
    //! @return 処理中のエミッタ時間アニメーション数
    //---------------------------------------------------------------------------
    int GetProcessingEmitterAnimCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.emitterAnimCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  処理中ののタイプ別エミッタ数を取得します。
    //!
    //! @param[in]  mode  エミッタ挙動計算タイプグループID
    //! @return 処理中のタイプ別エミッタ数
    //---------------------------------------------------------------------------
    int GetProcessingEmitterCount( detail::EmitterCalculationMode mode ) const NN_NOEXCEPT
    {
        int emitterCount = 0;
        if ( mode == detail::EmitterCalculationMode_Cpu )
        {
            emitterCount = m_ProcessingInfo.cpuEmitterCount;
        }
        if ( mode == detail::EmitterCalculationMode_Gpu )
        {
            emitterCount = m_ProcessingInfo.gpuEmitterCount;
        }
        if ( mode == detail::EmitterCalculationMode_GpuStreamOut )
        {
            emitterCount = m_ProcessingInfo.computeEmitterCount;
        }
        return emitterCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  処理中の 履歴式ストライプ の本数を取得します。
    //!
    //! @return 処理中の 履歴式ストライプ の本数
    //---------------------------------------------------------------------------
    int GetProcessingStripeCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.stripeCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  処理中の 履歴式ストライプ2 の本数を取得します。
    //!
    //! @return 処理中の 履歴式ストライプ2 の本数
    //---------------------------------------------------------------------------
    int GetProcessingSuperStripeCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.superStripeCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  処理中の連結式ストライプ本数を取得します。
    //!
    //! @return 処理中の連結式ストライプ本数
    //---------------------------------------------------------------------------
    int GetProcessingConnectionStripeCount() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.connectionStripeCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタセットヘッドを取得します。( const 無し版 )
    //!
    //! @param[in]  groupId  グループID
    //! @return     エミッタセット
    //---------------------------------------------------------------------------
    EmitterSet* GetEmitterSetHead( int groupId ) NN_NOEXCEPT
    {
        return m_EmitterSetHead[ groupId ];
    }

    //---------------------------------------------------------------------------
    //! @brief        エミッタセットヘッドを取得します。
    //!
    //! @param[in]  groupId  グループID
    //! @return     エミッタセット
    //---------------------------------------------------------------------------
    const EmitterSet* GetEmitterSetHead( int groupId ) const NN_NOEXCEPT
    {
        return m_EmitterSetHead[ groupId ];
    }

    //---------------------------------------------------------------------------
    //! @brief      （※旧 API ）動的ヒープから確保したメモリサイズを取得します。
    //! @deprecated 旧型式の API 名です。nn::vfx::System::GetAllocatedFromDynamicHeapSize() に乗り換えてください。
    //! @return     動的ヒープから確保したメモリサイズ
    //! @see        nn::vfx::System::GetAllocatedFromDynamicHeapSize()
    //---------------------------------------------------------------------------
    NN_DEPRECATED size_t GetAllocedFromDynamicHeapSize() const NN_NOEXCEPT
    {
        return GetAllocatedFromDynamicHeapSize();
    }

    //---------------------------------------------------------------------------
    //! @brief  動的ヒープから確保したメモリサイズを取得します。
    //!
    //! @return 確保したヒープサイズ
    //---------------------------------------------------------------------------
    size_t GetAllocatedFromDynamicHeapSize() const NN_NOEXCEPT
    {
        return m_ProcessingInfo.allocatedDynamicHeapSize;
    }

    //---------------------------------------------------------------------------
    //! @brief 指定グループ、指定描画パスに描画すべきエミッタがあるか。
    //!
    //! @param[in] groupId     グループID
    //! @param[in] drawPathFlag    描画パスフラグ
    //! @return 指定されたエミッタがあればtrue
    //---------------------------------------------------------------------------
    bool IsHasRenderingEmitter( int groupId, uint32_t drawPathFlag ) const NN_NOEXCEPT
    {
        for ( int i = 0; i < m_ProcessingCountMax; i++ )
        {
            if( m_ProcessingDrawParameter[ i ].m_EnableDrawPath[ groupId ] & drawPathFlag )
            {
                return true;
            }
        }
        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief 指定グループ、指定描画パスでフレームバッファテクスチャが要求されるか。
    //!
    //! @param[in] groupId         グループID
    //! @param[in] drawPathFlag    描画パスフラグ
    //! @return フレームバッファテクスチャが要求される場合はtrueが返ります。
    //---------------------------------------------------------------------------
    bool IsRequestFrameBufferTexture( int groupId, uint32_t drawPathFlag ) const NN_NOEXCEPT
    {
        for ( int i = 0; i < m_ProcessingCountMax; i++ )
        {
            if( m_ProcessingDrawParameter[ i ].m_RequestFrameBufferTexturePath[ groupId ] & drawPathFlag )
            {
                return true;
            }
        }
        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief 指定グループ、指定描画パスでデプスバッファテクスチャが要求されるか。
    //!
    //! @param[in] groupId         グループID
    //! @param[in] drawPathFlag    描画パスフラグ
    //! @return デプスバッファテクスチャが要求される場合はtrueが返ります。
    //---------------------------------------------------------------------------
    bool IsRequestDepthBufferTexture( int groupId, uint32_t drawPathFlag ) const NN_NOEXCEPT
    {
        for ( int i = 0; i < m_ProcessingCountMax; i++ )
        {
            if( m_ProcessingDrawParameter[ i ].m_RequestDepthBufferTexturePath[ groupId ] & drawPathFlag )
            {
                return true;
            }
        }
        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief 定数バッファのCPUキャッシュフラッシュを自動的に行うか。
    //!
    //! @return trueで行う、falseで行わない
    //---------------------------------------------------------------------------
    inline bool IsConstantBufferCpuChacheFlush() const NN_NOEXCEPT
    {
        return m_IsConstantBufferCpuCacheFlush;
    }

    //---------------------------------------------------------------------------
    //! @brief 定数バッファのGPUキャッシュフラッシュを自動的に行うか。
    //!
    //! @return trueで行う、falseで行わない
    //---------------------------------------------------------------------------
    inline bool IsConstantBufferGpuChacheFlush() const NN_NOEXCEPT
    {
        return m_IsConstantBufferGpuCacheFlush;
    }

    //---------------------------------------------------------------------------
    //! @brief        指定グループに有効なエミッタが所属しているかチェックします。
    //!
    //! @param[in]    groupId    グループID。
    //!
    //! @return       有効であればtrue、無効であればfalseを返します。
    //---------------------------------------------------------------------------
    bool HasEnableEmitter( int groupId ) NN_NOEXCEPT
    {
        bool ret = false;

        for ( int i = 0; i < m_ProcessingCountMax; i++ )
        {
            if ( m_ProcessingDrawParameter[ i ].m_EnableDrawPath[ groupId ] != 0 )
            {
                ret = true;
            }
        }

        return ret;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate  システムのバッファモードを取得します。
    //!
    //! @return バッファモード
    //---------------------------------------------------------------------------
    BufferingMode GetBufferMode() const NN_NOEXCEPT
    {
        return m_BufferMode;
    }

    //---------------------------------------------------------------------------
    //! @brief  トリプルバッファを有効化しているかどうか
    //!         nn::vfx::Config::SetEnableTripleBuffer で初期化時に設定できます。
    //!
    //! @return トリプルバッファを有効化している場合 true
    //---------------------------------------------------------------------------
    bool IsEnableTripleBuffer() const NN_NOEXCEPT
    {
        return m_IsEnableTripleBuffer;
    }

    //---------------------------------------------------------------------------
    //! @brief エラーチェックを行います。
    //!
    //!        現在は動作しておりません。コンパイル維持の為、残してあります。
    //---------------------------------------------------------------------------
    void CheckError() NN_NOEXCEPT
    {
        return;
    }

    //---------------------------------------------------------------------------
    //! @brief ランタイムエラーを取得します。
    //!        現在は動作しておりません。コンパイル維持の為、残してあります。
    //!
    //! @return ランタイムエラー
    //---------------------------------------------------------------------------
    detail::RuntimeErrorId GetRuntimeError() const NN_NOEXCEPT
    {
        return detail::RuntimeErrorId_None;
    }

    //---------------------------------------------------------------------------
    //! @brief テンポラリバッファの消費量を取得します。
    //!
    //! @param[in]    processingIndex       処理インデックス。Configで指定されたインデックス数までが利用可能です。
    //! @return 消費テンポラリバッファサイズ
    //---------------------------------------------------------------------------
    size_t GetUsedTemporaryBufferSize( int processingIndex ) const NN_NOEXCEPT
    {
        if ( processingIndex >= m_ProcessingCountMax ) return 0;
        return m_ProcessingDrawParameter[ processingIndex ].m_DrawTempBuffer.GetUsedSize();
    }

    //---------------------------------------------------------------------------
    //! @brief テンポラリバッファの最大消費量を取得します。
    //!
    //! @param[in]  processingIndex 処理インデックス
    //! @return     最大消費テンポラリバッファサイズ
    //---------------------------------------------------------------------------
    size_t GetMaxUsedTemporaryBufferSize( int processingIndex ) const NN_NOEXCEPT
    {
        if ( processingIndex >= m_ProcessingCountMax ) return 0;
        return m_ProcessingDrawParameter[ processingIndex ].m_DrawTempBuffer.GetMaxUsedSize();
    }

    //---------------------------------------------------------------------------
    //! @brief テンポラリバッファのサイズを取得します。
    //! @return     テンポラリバッファサイズ
    //---------------------------------------------------------------------------
    size_t GetTemporaryBufferSize() const NN_NOEXCEPT
    {
        return m_ProcessingDrawParameter[ 0 ].m_DrawTempBuffer.GetSize();
    }

    //---------------------------------------------------------------------------
    //! @brief  グローバルカウンタを取得します。
    //!
    //! @return グローバルカウンタ
    //---------------------------------------------------------------------------
    int GetGlobalCounter() const NN_NOEXCEPT
    {
        return m_GlobalCounter;
    }


    //---------------------------------------------------------------------------
    //! @briefprivate 描画されたパーティクル数を取得します。
    //! @return パーティクル数
    //---------------------------------------------------------------------------
    int GetRenderParticleCount() const NN_NOEXCEPT
    {
        return 0;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate 描画されたエミッタ数を取得します。
    //! @return エミッタ数
    //---------------------------------------------------------------------------
    int GetRenderEmitterCount() const NN_NOEXCEPT
    {
        return 0;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate 現フレームに放出されたパーティクル数を取得します。
    //! @return パーティクル数
    //---------------------------------------------------------------------------
    int GetEmittedParticleCount() const NN_NOEXCEPT
    {
        return 0;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate ストライプの処理数を取得します。
    //! @return ストライプ数
    //---------------------------------------------------------------------------
    int GetStripeCalculationCount() const NN_NOEXCEPT
    {
        return 0;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate ストライプが計算している頂点数を取得します。
    //! @return 頂点数
    //---------------------------------------------------------------------------
    int GetStripeVertexCalculationCount() const NN_NOEXCEPT
    {
        return 0;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate ストライプが描画している頂点数を取得します。
    //! @return 頂点数
    //---------------------------------------------------------------------------
    int GetRenderStripeVertexCount() const NN_NOEXCEPT
    {
        return 0;
    }

    //---------------------------------------------------------------------------
    //! @brief        指定グループで有効なストライプを保持するかチェックします。
    //!
    //! @param[in]    groupId    グループID。
    //!
    //! @return       有効であればtrue、無効であればfalseを返します。
    //---------------------------------------------------------------------------
    bool IsValidStripe( int groupId ) const NN_NOEXCEPT
    {
        NN_UNUSED( groupId );
        return false;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate 指定グループパーティクルの処理数を取得します。
    //!
    //! @param[in] groupId     グループID
    //! @return パーティクル数
    //---------------------------------------------------------------------------
    int GetParticleCalculationCount( int groupId ) const NN_NOEXCEPT
    {
        NN_UNUSED( groupId );
        return 0;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate 指定グループの先頭のエミッタを取得します。
    //!
    //! @param[in] groupId     グループID
    //! @return エミッタ数
    //---------------------------------------------------------------------------
    const Emitter* GetEmitterHead( int groupId ) const NN_NOEXCEPT
    {
        NN_UNUSED( groupId );
        return NULL;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate 指定グループの先頭のエミッタを取得します。( const 無し版。)
    //!
    //! @param[in] groupId     グループID
    //! @return エミッタ
    //---------------------------------------------------------------------------
    Emitter* GetEmitterHead( int groupId ) NN_NOEXCEPT
    {
        NN_UNUSED( groupId );
        return NULL;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate 指定グループに所属するエミッタ数を取得します。
    //!
    //! @param[in] groupId     グループID
    //! @return エミッタ数
    //---------------------------------------------------------------------------
    int GetEmitterCount( int groupId ) NN_NOEXCEPT
    {
        NN_UNUSED( groupId );
        return 0;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate 指定グループ、指定インデックスのエミッタを取得します。
    //! @param[in] groupId     グループID
    //! @param[in] index     インデックス
    //! @return エミッタ
    //---------------------------------------------------------------------------
    const Emitter* GetEmitter( int groupId, int index ) const NN_NOEXCEPT
    {
        NN_UNUSED( groupId );
        NN_UNUSED( index );
        return NULL;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate 指定グループ、指定インデックスのエミッタを取得します。( const 無し版。)
    //! @param[in] groupId     グループID
    //! @param[in] index     インデックス
    //! @return エミッタ
    //---------------------------------------------------------------------------
    Emitter* GetEmitter( int groupId, int index ) NN_NOEXCEPT
    {
        NN_UNUSED( groupId );
        NN_UNUSED( index );
        return NULL;
    }

    //@}


    //----------------------------------------
    //! @name GPU バッファ
    //@{

    //---------------------------------------------------------------------------
    //! @brief      （※旧 API ）Gpuヒープの使用サイズを取得します。
    //! @deprecated 旧型式の API 名です。nn::vfx::System::GetAllocatedFromGpuHeap() に乗り換えてください。
    //! @return     Gpuヒープの使用サイズ
    //! @see        nn::vfx::System::GetAllocatedFromGpuHeap()
    //---------------------------------------------------------------------------
    NN_DEPRECATED size_t GetAllocedFromGpuHeap() const NN_NOEXCEPT
    {
        return GetAllocatedFromGpuHeap();
    }

    //---------------------------------------------------------------------------
    //! @brief Gpuヒープの使用サイズを取得します。
    //---------------------------------------------------------------------------
    size_t GetAllocatedFromGpuHeap() const NN_NOEXCEPT
    {
        return m_BufferAllocator.GetAllocatedSize();
    }

    //---------------------------------------------------------------------------
    //! @brief Gpuヒープの使用サイズを取得します。
    //---------------------------------------------------------------------------
    size_t GetGpuHeapSizeMax() const NN_NOEXCEPT
    {
        return m_BufferAllocator.GetSize();
    }

    //---------------------------------------------------------------------------
    //! @brief      （※旧 API ）Gpuヒープから確保された回数を取得します。
    //! @deprecated 旧型式の API 名です。nn::vfx::System::GetAllocatedCountFromGpuHeap() に乗り換えてください。
    //! @return     Gpuヒープから確保された回数
    //! @see        nn::vfx::System::GetAllocatedCountFromGpuHeap()
    //---------------------------------------------------------------------------
    NN_DEPRECATED int GetAllocedCountFromGpuHeap() const NN_NOEXCEPT
    {
        return GetAllocatedCountFromGpuHeap();
    }

    //---------------------------------------------------------------------------
    //! @brief Gpuヒープから確保された回数を取得します。
    //---------------------------------------------------------------------------
    int GetAllocatedCountFromGpuHeap() const NN_NOEXCEPT
    {
        return m_BufferAllocator.GetAllocatedCount();
    }

    //---------------------------------------------------------------------------
    //! @brief  Gpuバッファリリースカウンタを設定します。
    //! @param[in]  releaseCounter 設定するリリースカウンタ。デフォルト値は 2 になっています。
    //---------------------------------------------------------------------------
    void SetGpuHeapReleaseCounter( int releaseCounter ) NN_NOEXCEPT
    {
        return m_BufferAllocator.SetReleaseCounter( releaseCounter );
    }

    //---------------------------------------------------------------------------
    //! @briefprivate  厳密なGPUカウンタモードを有効にします。nn::vfx::System 初期化時に一度だけ呼び出してください。
    //!
    //!         本機能は試験的な機能です。予告なく仕様が変更される、もしくはAPI自体が無くなる場合があります。
    //!         初期化時に一度だけ有効にする使い方を想定しているため、一度有効にした後は無効化できません。
    //---------------------------------------------------------------------------
    void EnablePreciseGpuCounterMode() NN_NOEXCEPT
    {
        m_IsEnabledPreciseGpuCounterMode = true;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate  厳密なGPUカウンタモードが有効かどうかを取得します。
    //!
    //!         本機能は試験的な機能です。予告なく仕様が変更される、もしくはAPI自体が無くなる場合があります。
    //!
    //! @return 厳密なGPUカウンタモードが有効かどうか
    //---------------------------------------------------------------------------
    bool IsEnabledPreciseGpuCounterMode() const NN_NOEXCEPT
    {
        return m_IsEnabledPreciseGpuCounterMode;
    }

    //@}

public:
    //----------------------------------------
    //! @name エフェクトリソース
    //@{

    //---------------------------------------------------------------------------
    //! @briefprivate      エミッタセットを確保します。
    //! @return     エミッタセット
    //---------------------------------------------------------------------------
    EmitterSet* AllocEmitterSet() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate      エミッタを確保します。
    //! @return エミッタ
    //---------------------------------------------------------------------------
    Emitter* AllocEmitter() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate      エミッタを初期化します。
    //! @param[in] pEmitter     エミッタ
    //---------------------------------------------------------------------------
    void InitializeEmitter( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate      エミッタの終了処理をします。
    //! @param[in] pEmitter     エミッタ
    //---------------------------------------------------------------------------
    void FinalizeEmitter( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate                   リソース更新に伴うアップデートを行います。
    //!                                 ビューア機能が利用する機能です。
    //! @param[in]  pEmitterResource    エミッタリソースへのポインタ
    //! @param[in]  withReset           リセットを行うかどうか
    //---------------------------------------------------------------------------
    void UpdateFromResource( EmitterResource* pEmitterResource, bool withReset ) NN_NOEXCEPT;

    //@}

    //---------------------------------------------------------------------------
    //! @brief      エミッタを描画します。
    //! @param[in] pCommandBuffer       コマンドバッファ
    //! @param[in] pEmitter             エミッタ
    //! @param[in] pUserParam           ユーザーパラメータ
    //! @param[in] pDrawParameterArg    描画パラメータへのポインタ
    //---------------------------------------------------------------------------
    void DrawEmitter( nn::gfx::CommandBuffer* pCommandBuffer, Emitter* pEmitter, void* pUserParam, DrawParameterArg* pDrawParameterArg ) NN_NOEXCEPT
    {
        DrawEmitter( pCommandBuffer, pEmitter, true, pUserParam, pDrawParameterArg );
    }

    //---------------------------------------------------------------------------
    //! @brief      エミッタを描画します。
    //! @param[in] pCommandBuffer           コマンドバッファ
    //! @param[in] pEmitter                 エミッタ
    //! @param[in] doComputeShaderProcess   ストリームアウト処理を行うかどうか
    //! @param[in] pUserParam               ユーザーパラメータ
    //! @param[in] pDrawParameterArg        描画パラメータへのポインタ
    //---------------------------------------------------------------------------
    void DrawEmitter( nn::gfx::CommandBuffer* pCommandBuffer, Emitter* pEmitter, bool doComputeShaderProcess, void* pUserParam, DrawParameterArg* pDrawParameterArg ) NN_NOEXCEPT;

private:
    //---------------------------------------------------------------------------
    //! @briefprivate               システムの初期化メソッド
    //! @param[in]  pStaticHeap     静的ヒープへのポインタ
    //! @param[in]  pDynamicHeap    動的ヒープへのポインタ
    //! @param[in]  config          コンフィグ
    //---------------------------------------------------------------------------
    virtual void Initialize( Heap* pStaticHeap, Heap* pDynamicHeap, const Config& config ) NN_NOEXCEPT;

public:
    //---------------------------------------------------------------------------
    //! @briefprivate   システムのバッファの本数を取得
    //! @return         バッファの本数
    //---------------------------------------------------------------------------
    int GetBufferingCount() const NN_NOEXCEPT
    {
        return ( GetBufferMode() == BufferingMode_TripleBuffering ) ? 3 : 2;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate      生成されたエミッタセットを管理リストへ登録します。
    //! @param[in] pEmitterSet  エミッタセット
    //! @param[in] groupId     グループID
    //---------------------------------------------------------------------------
    void AddEmitterSetList( EmitterSet* pEmitterSet, int groupId ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate      生成されたエミッタセットを遅延生成リストへ登録します。
    //! @param[in] pEmitterSet  エミッタセット
    //! @param[in] groupId     グループID
    //---------------------------------------------------------------------------
    void AddDelayCreateEmitterSetList( EmitterSet* pEmitterSet, int groupId ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate      指定エミッタセットを管理リストから削除します。
    //! @param[in] pEmitterSet  エミッタセット
    //---------------------------------------------------------------------------
    void RemoveEmitterSetList( EmitterSet* pEmitterSet ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate      指定エミッタセットを遅延生成管理リストから削除します。
    //! @param[in] pEmitterSet  エミッタセット
    //---------------------------------------------------------------------------
    void RemoveDelayCreateEmitterSetList( EmitterSet* pEmitterSet ) NN_NOEXCEPT;


    //----------------------------------------
    //! @name デバッグ機能
    //@{

    //---------------------------------------------------------------------------
    //! @brief  システムの計算処理を停止させます。
    //! @param[in] enableCalculation     true で再生、false で停止
    //---------------------------------------------------------------------------
    void SetEnableCalculation( bool enableCalculation ) NN_NOEXCEPT
    {
        m_EnableCalculation = enableCalculation;
    }

    //---------------------------------------------------------------------------
    //! @brief  システムの描画処理を停止させます。
    //! @param[in] enableDraw       true で描画、false で未描画
    //! @param[in] enableComputeShader  true で処理、false で停止
    //---------------------------------------------------------------------------
    void SetEnableDrawProcess( bool enableDraw, bool enableComputeShader ) NN_NOEXCEPT
    {
        m_IsEnableDrawProcess = enableDraw;
        m_IsEnableComputeShaderProcess = enableComputeShader;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate   パーティクルのソートを行い、その結果の配列の先頭を返します。
    //! @param[in]  pOutSortedList      ソート結果
    //! @param[in]  pOutParticleCount   配列サイズ
    //! @param[in]  pEmitter            エミッタ
    //! @param[in]  sortType            ソートタイプ
    //! @param[in]  baseEmitterTime     基準となるエミッタ時間（エミッタ時間が更新される前かどうかで調整が必要）
    //! @param[in]  processingIndex     処理インデックス
    //! @return     trueで成功、falseで失敗
    //---------------------------------------------------------------------------
    bool GetSortedParticleList( detail::SortData** pOutSortedList, int* pOutParticleCount, Emitter* pEmitter, detail::ParticleSortType sortType, float baseEmitterTime, int processingIndex = 0 ) const NN_NOEXCEPT;

    //@}

    //---------------------------------------------------------------------------
    //! @briefprivate   エミッタセット ソート管理 構造体
    //---------------------------------------------------------------------------
    struct SortEmitterSetData
    {
        EmitterSet*     pEmitterSet;    //!< エミッタセットへのポインタ
        uint32_t        param;          //!< int 型のパラメータ
    };

private:
    //---------------------------------------------------------------------------
    //! @briefprivate      カスタムシェーダテクスチャの状態をリセットします。
    //!
    //! @param[in] processingIndex  処理インデックス。Configで指定されたインデックス数までが利用可能です。
    //! @param[in] pShader          シェーダ
    //---------------------------------------------------------------------------
    void ResetCustomShaderSetting( int processingIndex, detail::Shader* pShader ) NN_NOEXCEPT
    {
        m_ProcessingDrawParameter[ processingIndex ].m_pCurrentCustomShader           = pShader;
        m_ProcessingDrawParameter[ processingIndex ].m_CurrentCustomShaderTextureSlot = CustomShaderTextureSlotId_0;
    }

    //---------------------------------------------------------------------------
    //! @brief                  エミッタ情報をダンプする
    //! @param[in]  pEmitter    エミッタへのポインタ
    //! @param[in]  debugLevel  デバッグレベル指定
    //---------------------------------------------------------------------------
    void DumpEmitterInformation( Emitter* pEmitter, const int debugLevel ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief システム
    //---------------------------------------------------------------------------
    enum SystemPrivateSettings
    {
        SystemPrivateSettings_MaxTextureInitializerFunc        = 8,         //!<  テクスチャ初期化関数最大登録数
    };

private:
    //---------------------------------------------------------------------------
    //! @brief システム描画パラメータ
    //---------------------------------------------------------------------------
    struct DrawParameter
    {
        DrawViewFlag                    m_DrawViewFlag;                                                       //!< 描画ビューフラグ
        uint32_t                        m_DrawPathFlag;                                                       //!< 現在描画を行っている描画パス
        uint32_t                        m_EnableDrawPath[ SystemParameters_MaxGroupCount ];                   //!< グループ内の各エミッタ描画パスフラグの論理和
        ShaderType                      m_CurrentShaderType;                                                  //!< カレントのシェーダタイプ
        TemporaryBuffer                 m_DrawTempBuffer;                                                     //!< GPU参照用のテンポラリバッファ
        ViewParam                       m_ViewParam;                                                          //!< ビューパラメータ
        nn::gfx::GpuAddress             m_ViewGpuAddress;                                                     //!< ビュー GfxGpuアドレス
        nn::gfx::DescriptorSlot         m_FrameBufferTexture;                                                 //!< フレームバッファテクスチャ
        nn::gfx::DescriptorSlot         m_DepthBufferTexture;                                                 //!< デプスバッファテクスチャ
        uint32_t                        m_RequestFrameBufferTexturePath[ SystemParameters_MaxGroupCount ];    //!< フレームバッファテクスチャを要求するパスフラグ
        uint32_t                        m_RequestDepthBufferTexturePath[ SystemParameters_MaxGroupCount ];    //!< フレームバッファテクスチャを要求するパスフラグ
        detail::Shader*                 m_pCurrentCustomShader;                                               //!< カレントのシェーダ
        CustomShaderTextureSlotId       m_CurrentCustomShaderTextureSlot;                                     //!< カレントのテクスチャスロット
        SortEmitterSetData*             m_SortEmittetSet;                                                     //!< エミッタセットソート用バッファ
        int                             m_AddedEmittetSetCount;                                               //!< エミッタセットソート用バッファに追加した数
        detail::SortData*               m_pParticleSortBuffer;                                                //!< パーティクルソート用バッファの先頭アドレス
        bool                            m_IsViewConstantBufferSetuped;                                        //!< ビューコンスタントバッファセットアップ済みかどうか
    };

private:
    int                                 m_ProcessingCountMax;                                                 //!< 同時に処理可能な最大プロセス数
    DrawParameter*                      m_ProcessingDrawParameter;                                            //!< 各プロセスごとの描画パラメータ
    size_t                              m_ProcessingDrawParameterWorkSize;
    DrawParameterArg*                   m_DrawParameterArg;
    size_t                              m_DrawParameterArgWorkSize;
    static  bool                        g_IsInitialized;                                                                                //!< システム初期化済みフラグ
    bool                                m_IsConstantBufferCpuCacheFlush;                                                                //!< 定数バッファがCPUキャッシュフラッシュかどうか
    bool                                m_IsConstantBufferGpuCacheFlush;                                                                //!< 定数バッファがGPUキャッシュフラッシュかどうか
    bool                                m_EnableCalculation;                                                                            //!< 計算処理が有効かどうか
    bool                                m_IsEnableDrawProcess;                                                                          //!< 描画処理が有効かどうか
    bool                                m_IsEnableComputeShaderProcess;                                                                 //!< コンピュートシェーダ処理が有効かどうか
    bool                                m_IsBatchProcessComputeShaderEmitter;                                                           //!< コンピュートシェーダのバッチ処理が有効かどうか
    bool                                m_IsEnableTripleBuffer;                                                                         //!< トリプルバッファが有効かどうか
    nn::gfx::Device*                    m_pDevice;                                                                                      //!< gfx デバイスへのポインタ
    detail::CalculateAllocatedSizeHeap  m_StaticHeap;                                                                                   //!< システム初期化時に確保されるヒープ
    BufferingMode                       m_BufferMode;                                                                                   //!< バッファモード
    int                                 m_ResourceCount;                                                                                //!< 最大リソース数
    int                                 m_UserResourceCount;                                                                            //!< 最大リソース数
    int                                 m_EmitterSetCount;                                                                              //!< 最大エミッタセット数
    int                                 m_EmitterCount;                                                                                 //!< 最大エミッタ数
    int                                 m_EmitterSetIndex;                                                                              //!< エミッタセットインデックス(空き検索用)
    int                                 m_EmitterIndex;                                                                                 //!< エミッタインデックス(空き検索用)
    int                                 m_EmitterSetCreateId;                                                                           //!< エミッタセットの生成ＩＤ
    int                                 m_FreeEmitterCount;                                                                             //!< 利用可能なエミッタ数
    int                                 m_GlobalCounter;                                                                                //!< グローバルフレーム（何回計算を回したか）
    int                                 m_ProcessingGroupId;                                                                            //!< 計算処理を行ったグループIDフラグ
    int                                 m_StripeCountMax;                                                                               //!< 履歴式ストライプ の最大数
    int                                 m_SuperStripeCountMax;                                                                          //!< 履歴式ストライプ2 の最大数
    Resource**                          m_ResourceArray;                                                                                //!< リソースクラス配列
    EmitterSet*                         m_EmitterSetArray;                                                                              //!< エミッタセット配列
    Emitter*                            m_EmitterBuffer;                                                                                //!< エミッタバッファ
    EmitterSet*                         m_EmitterSetHead[ SystemParameters_MaxGroupCount ];                                             //!< エミッタセットリスト先頭
    EmitterSet*                         m_EmitterSetTail[ SystemParameters_MaxGroupCount ];                                             //!< エミッタセットリスト最後尾
    EmitterSet*                         m_DelayCreateEmitterSetHead[ SystemParameters_MaxGroupCount ];                                  //!< 遅延生成エミッタセットリスト先頭
    EmitterSet*                         m_DelayCreateEmitterSetTail[ SystemParameters_MaxGroupCount ];                                  //!< 遅延生成エミッタセットリスト最後尾
    EmitterSet**                        m_DelayKillEmitterSetArray;                                                                     //!< 遅延削除エミッタセット配列
    int                                 m_DelayKillEmitterSetAddCount;                                                                  //!< 遅延削除エミッタセット配列に追加された数
    detail::EmitterCalculator*          m_pEmitterCalculator;                                                                           //!< エミッタ計算クラス
    bool                                m_IsEnableCallback[ CallbackId_MaxCallbackIdCount ];                                            //!< カスタムアクション/シェーダ コールバック有効無効
    CallbackSet                         m_Callback[ CallbackId_MaxCallbackIdCount ];                                                    //!< カスタムアクション/シェーダ コールバック実体
    CallbackSet                         m_EmitterPluginCallback[ detail::EmitterPluginCallbackIndex_MaxCallbackIndex ];                 //!< エミッタプラグイン コールバック実体
    EmitterSetInitializeCallback        m_EmitterSetInitializeCallback;                                                                 //!< エミッタセット初期化コールバック
    EmitterSetFinalizeCallback          m_EmitterSetFinalizeCallback;                                                                   //!< エミッタセット破棄時コールバック
    uint32_t                            m_DrawPathCallbackFlag[ DrawPathCallbackId_MaxCallbackId ];                                     //!< 描画パスコールバックフラグ
    DrawPathRenderStateSetCallback      m_DrawPathRenderStateSetCallback[ DrawPathCallbackId_MaxCallbackId ];                           //!< 描画パス コールバック 実体
    DrawEmitterProfileCallback          m_EmitterDrawProfilerCallback;                                                                  //!< カレントのエミッタ描画プロファイラ
    EmitterCalculateLodCallback         m_EmitterCalculateLodCallback;                                                                  //!< エミッタLODコールバック
    EmitterDrawCullingCallback          m_EmitterDrawCullingCallback;                                                                   //!< エミッタカリングコールバック
    CustomFieldCallback                 m_CustomFieldCallback;                                                                          //!< カスタムフィールドコールバック
    void*                               m_StaticFrameHeapBuffer;                                                                        //!< システムが確保する静的なバッファ
    size_t                              m_RandomWorkSize;                                                                               //!< 乱数ワークサイズ
    size_t                              m_DelayFreeWorkSize;                                                                            //!< 遅延解放用ワークサイズ
    size_t                              m_ResourceWorkSize;                                                                             //!< リソースワークサイズ
    size_t                              m_EmitterSetWorkSize;                                                                           //!< エミッタセットワークサイズ
    size_t                              m_EmitterSetDelayFreeWorkSize;                                                                  //!< エミッタセット遅延解放ワークサイズ
    size_t                              m_EmitterWorkSize;                                                                              //!< エミッタワークサイズ
    size_t                              m_EmitterPtrArrayWorkSize;                                                                      //!< エミッタポインタ配列ワークサイズ
    size_t                              m_EmitterCalculationWorkSize;                                                                   //!< エミッタ計算クラスワークサイズ
    size_t                              m_EmitterConstantBufferWorkSize;                                                                //!< エミッタ定数バッファワークサイズ
    size_t                              m_EmitterSetSortWorkSize;                                                                       //!< エミッタセットソートワークサイズ
    ProcessingInfo                      m_ProcessingInfo;                                                                               //!< システム稼動状況
    Emitter*                            m_pComputeShaderEmitterList;                                                                    //!< コンピュートシェーダ用 エミッタのリスト
    Emitter*                            m_pComputeShaderEmitterTail;                                                                    //!< コンピュートシェーダ用 エミッタのリスト最後尾
    int                                 m_ParticleSortBufferCount;                                                                      //!< パーティクルソート用バッファのサイズ
    size_t                              m_ParticleSetSortWorkSize;                                                                      //!< エミッタセットソートバッファサイズ
    void*                               m_CommonCustomShaderConstantBuffer[ CustomShaderConstantBufferIndex_MaxIndexCount ];            //!< 登録された全エミッタ描画で共通で利用されるカスタム定数バッファ
    size_t                              m_CommonCustomShaderConstantBufferSize[ CustomShaderConstantBufferIndex_MaxIndexCount ];        //!< 登録された全エミッタ描画で共通で利用されるカスタム定数バッファサイズ
    detail::ConstantBuffer              m_EmitterDynamicConstantBuffer;                                                                 //!< エミッタが動的に書き換えるコンスタントバッファ
    detail::BufferAllocator             m_BufferAllocator;                                                                              //!< バッファアロケータ
    void*                               m_BufferAllocatorManagementBuffer;                                                              //!< バッファアロケータ管理領域バッファ
    void*                               m_BufferAllocatorMemoryPoolBuffer;                                                              //!< バッファアロケータメモリプールバッファ
    detail::Mutex                       m_KillEmitterSetListMutex;                                                                      //!< エミッタセット削除リスト同期オブジェクト
    detail::Mutex                       m_CreateEmitterSetListMutex;                                                                    //!< エミッタセット生成リスト同期オブジェクト
    bool                                m_IsEnabledPreciseGpuCounterMode;                                                               //!< [Experimental]厳密なGPUカウンタモードを使用するかどうか
};

} // namespace vfx
} // namespace nn
