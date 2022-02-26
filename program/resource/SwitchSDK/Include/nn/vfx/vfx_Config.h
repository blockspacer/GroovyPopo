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

#include <nn/vfx/vfx_TargetDef.h>
#include <nn/vfx/vfx_Heap.h>

namespace nn {
namespace vfx {

//---------------------------------------------------
//! @brief  エフェクトシステム生成時に設定するコンフィグです。
//!
//!         nn::vfx::Config::SetEffectHeap(), nn::vfx::Config::SetEffectDynamicHeap() でのヒープの指定はシステムの起動に必須になります。
//!         必要に応じて、適切にバッファサイズを設定してください。
//!         詳しくは、エフェクト チュートリアル ドキュメントをご覧ください。
//! @see    nn::vfx::Config::SetEffectHeap(), nn::vfx::Config::SetEffectDynamicHeap()
//---------------------------------------------------
class Config
{
public:
    //---------------------------------------------------------------------------
    //! @brief  nn::vfx::Config のデフォルト値です。
    //---------------------------------------------------------------------------
    enum DefaultConfigSettings
    {
        DefaultConfigSettings_EmitterCount              = 64,           //!< デフォルトのエミッタ数(64)
        DefaultConfigSettings_EmitterSetCount           = 32,           //!< デフォルトのエミッタセット数(32)
        DefaultConfigSettings_ResourceCount             = 8,            //!< デフォルトのリソース数(8)
        DefaultConfigSettings_ResourceCountForViewer    = 32,           //!< デフォルトのビューア利用リソース数(32)
        DefaultConfigSettings_StripeCount               = 32,           //!< デフォルトの 履歴式ストライプ の数(32)
        DefaultConfigSettings_SuperStripeCount          = 32,           //!< デフォルトの 履歴式ストライプ2 の数(32)
        DefaultConfigSettings_TemporaryBufferSize       = 64 * 1024,    //!< デフォルトのバッファサイズ(64*1024)
        DefaultConfigSettings_ParticleSortBufferSize    = 1024,         //!< デフォルトのソートバッファサイズ( Particle 数)
        DefaultConfigSettings_GpuBufferSize             = 1024 * 1024,  //!< デフォルトのGpuバッファサイズ( 1024 * 1024 )
        DefaultConfigSettings_ProcessingCount           = 1,            //!< デフォルトの処理可能な最大プロセス数( 1 )
    };

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ。
    //!         各パラメータの初期値が設定されます。
    //---------------------------------------------------------------------------
    Config() NN_NOEXCEPT
        : m_pDevice( NULL )
        , m_pHeap( NULL )
        , m_pDynamicHeap( NULL )
        , m_EmitterCount( DefaultConfigSettings_EmitterCount )
        , m_EmitterSetCount( DefaultConfigSettings_EmitterSetCount )
        , m_StripeCount( DefaultConfigSettings_StripeCount )
        , m_SuperStripeCount( DefaultConfigSettings_SuperStripeCount )
        , m_ResourceCount( DefaultConfigSettings_ResourceCount )
        , m_TemporaryBufferSize( DefaultConfigSettings_TemporaryBufferSize )
        , m_SortBufferCount( DefaultConfigSettings_ParticleSortBufferSize )
        , m_SuppressLog( false )
        , m_ProcessingCount( DefaultConfigSettings_ProcessingCount )
        , m_IsEnableTripleBuffer( false )
        , m_IsConstantBufferCpuCacheFlush( true )
        , m_IsConstantBufferGpuCacheFlush( false )
        , m_IsComputeShaderBatchProcess( false )
        , m_GpuBufferSize( DefaultConfigSettings_GpuBufferSize ) {}

    //---------------------------------------------------------------------------
    //! @brief  デストラクタです。
    //---------------------------------------------------------------------------
    virtual ~Config() NN_NOEXCEPT
    {
    }

    //---------------------------------------------------------------------------
    //! @brief      nn::gfx::Device へのポインタを設定します。
    //! @param[in]  pDevice     nn::gfx::Device デバイス
    //---------------------------------------------------------------------------
    void SetGfxDevice( nn::gfx::Device* pDevice ) NN_NOEXCEPT
    {
        m_pDevice = pDevice;
    }

    //---------------------------------------------------------------------------
    //! @brief      メモリ確保先ヒープを設定します。システムの初期化に必須です。
    //!
    //!             システムの生成とリソース設定関数からメモリ確保関数が呼ばれます。
    //! @param[in]  pHeap    確保先ヒープ。
    //! @see        nn::vfx::Heap
    //---------------------------------------------------------------------------
    void SetEffectHeap( Heap *pHeap ) NN_NOEXCEPT
    {
        m_pHeap = pHeap;
    }

    //---------------------------------------------------------------------------
    //! @brief      動的メモリ確保先ヒープを設定します。システムの初期化に必須です。
    //!
    //!             エミッタ生成時にGPUで挙動行う場合にメモリ確保関数が呼ばれます。
    //! @param[in]  pHeap    確保先ヒープ。
    //! @see        nn::vfx::Heap
    //---------------------------------------------------------------------------
    void SetEffectDynamicHeap( Heap *pHeap ) NN_NOEXCEPT
    {
        m_pDynamicHeap = pHeap;
    }

    //---------------------------------------------------------------------------
    //! @brief      最大リソース数を設定します。
    //! @param[in]  count   最大リソース数
    //---------------------------------------------------------------------------
    void SetResourceCount( int count ) NN_NOEXCEPT
    {
        m_ResourceCount = count;
    }

    //---------------------------------------------------------------------------
    //! @brief      最大同時計算エミッタセット数を設定します。
    //! @param[in]  count   最大同時計算エミッタセット数
    //---------------------------------------------------------------------------
    void SetEmitterSetCount( int count ) NN_NOEXCEPT
    {
        m_EmitterSetCount = count;
    }

    //---------------------------------------------------------------------------
    //! @brief      最大同時計算エミッタ数を設定します。
    //! @param[in]  count   最大同時計算エミッタ数
    //---------------------------------------------------------------------------
    void SetEmitterCount( int count ) NN_NOEXCEPT
    {
        m_EmitterCount = count;
    }

    //---------------------------------------------------------------------------
    //! @brief      履歴式ストライプの最大数を設定します。
    //! @param[in]  count   履歴式ストライプの最大数
    //---------------------------------------------------------------------------
    void SetStripeCount( int count ) NN_NOEXCEPT
    {
        m_StripeCount = count;
    }

    //---------------------------------------------------------------------------
    //! @brief      履歴式ストライプ2 の最大数を設定します。
    //! @param[in]  count   履歴式ストライプ2 の最大数
    //---------------------------------------------------------------------------
    void SetSuperStripeCount( int count ) NN_NOEXCEPT
    {
        m_SuperStripeCount = count;
    }

    //---------------------------------------------------------------------------
    //! @brief  ストリームアウト処理を一括で行うモードを有効にする。
    //---------------------------------------------------------------------------
    void SetEnableComputeShaderBatchProcess() NN_NOEXCEPT
    {
        m_IsComputeShaderBatchProcess = true;
    }

    //---------------------------------------------------------------------------
    //! @brief      テンポラリバッファサイズを設定します。
    //! @param[in]  size    テンポラリバッファサイズ
    //---------------------------------------------------------------------------
    void SetTemporaryBufferSize( size_t size ) NN_NOEXCEPT
    {
        m_TemporaryBufferSize = size;
    }

    //---------------------------------------------------------------------------
    //! @brief      Gpuバッファサイズを設定します。
    //! @param[in]  size    Gpuバッファサイズサイズ
    //---------------------------------------------------------------------------
    void SetGpuBufferSize( size_t size ) NN_NOEXCEPT
    {
        m_GpuBufferSize = size;
    }

    //---------------------------------------------------------------------------
    //! @brief      パーティクルソート用バッファサイズを設定します。
    //! @param[in]  count   パーティクルソート用バッファサイズ
    //---------------------------------------------------------------------------
    void SetParticleSortBufferCount( int count ) NN_NOEXCEPT
    {
        m_SortBufferCount = count;
    }

    //---------------------------------------------------------------------------
    //! @brief      テンポラリバッファサイズを設定します。(廃止予定)
    //! @param[in]  size    テンポラリバッファサイズ
    //---------------------------------------------------------------------------
    void SetDoubleBufferSize( size_t size ) NN_NOEXCEPT
    {
        return SetTemporaryBufferSize( size );
    }

    //---------------------------------------------------------------------------
    //! @brief      ランタイムログ出力を抑制します。
    //! @param[in]  flag    ランタイムログ出力を押さえるかどうかを示すbool値
    //---------------------------------------------------------------------------
    void SetSuppressionLog( bool flag ) NN_NOEXCEPT
    {
        m_SuppressLog = flag;
    }

    //---------------------------------------------------------------------------
    //! @brief      トリプルバッファを有効にする場合に呼び出してください。
    //! @param[in]  isUseTripeBuffer    トリプルバッファが有効かどうか
    //---------------------------------------------------------------------------
    void SetEnableTripleBuffer( bool isUseTripeBuffer ) NN_NOEXCEPT
    {
        m_IsEnableTripleBuffer = isUseTripeBuffer;
    }
    //---------------------------------------------------------------------------
    //! @brief              （※旧 API ）定数バッファのCpuキャッシュフラッシュを行うかどうか。
    //! @deprecated         旧型式の API 名です。nn::vfx::Config::SetConstantBufferCpuCacheFlush() に乗り換えてください。
    //! @param[in]  flag    使用する場合 true
    //! @see                nn::vfx::Config::SetConstantBufferCpuCacheFlush()
    //---------------------------------------------------------------------------
    NN_DEPRECATED void SetConstantBufferCpuChacheFlush( bool flag ) NN_NOEXCEPT
    {
        SetConstantBufferCpuCacheFlush( flag );
    }

    //---------------------------------------------------------------------------
    //! @brief              定数バッファのCpuキャッシュフラッシュを行うかどうか。
    //! @param[in]  flag    使用する場合 true
    //---------------------------------------------------------------------------
    void SetConstantBufferCpuCacheFlush( bool flag ) NN_NOEXCEPT
    {
        m_IsConstantBufferCpuCacheFlush = flag;
    }

    //---------------------------------------------------------------------------
    //! @brief  使用するコア数を指定します。
    //---------------------------------------------------------------------------
    void SetProcessingCount( int count ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( count != 0 );
        m_ProcessingCount = count;
    }

    //---------------------------------------------------------------------------
    //! @brief  nn::gfx::Device へのポインタを取得します。
    //! @return nn::gfx::Device へのポインタ
    //---------------------------------------------------------------------------
    nn::gfx::Device* GetGfxDevice() const NN_NOEXCEPT
    {
        return m_pDevice;
    }

    //---------------------------------------------------------------------------
    //! @brief  メモリ確保先ヒープを取得します。
    //! @return ヒープへのポインタ
    //! @see    nn::vfx::Heap
    //---------------------------------------------------------------------------
    Heap* GetEffectHeap() const NN_NOEXCEPT
    {
        return m_pHeap;
    }

    //---------------------------------------------------------------------------
    //! @brief  動的メモリ確保先ヒープを取得します。
    //! @return ヒープへのポインタ
    //! @see    nn::vfx::Heap
    //---------------------------------------------------------------------------
    Heap* GetEffectDynamicHeap() const NN_NOEXCEPT
    {
        return m_pDynamicHeap;
    }

    //---------------------------------------------------------------------------
    //! @brief  最大同時計算エミッタ数を取得します。
    //! @return 最大同時計算エミッタ数
    //---------------------------------------------------------------------------
    int GetEmitterCount() const NN_NOEXCEPT
    {
        return m_EmitterCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  最大同時計算エミッタセット数を取得します。
    //! @return 最大同時計算エミッタセット数
    //---------------------------------------------------------------------------
    int GetEmitterSetCount() const NN_NOEXCEPT
    {
        return m_EmitterSetCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  最大リソース数を取得します。
    //! @return 最大リソース数
    //---------------------------------------------------------------------------
    int GetResourceCount() const NN_NOEXCEPT
    {
        return m_ResourceCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  履歴式ストライプ の最大数を取得します。
    //! @return 履歴式ストライプ の最大数
    //---------------------------------------------------------------------------
    int GetStripeCount() const NN_NOEXCEPT
    {
        return m_StripeCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  履歴式ストライプ2 の最大数を取得します。
    //! @return 履歴式ストライプ2 の最大数
    //---------------------------------------------------------------------------
    int GetSuperStripeCount() const NN_NOEXCEPT
    {
        return m_SuperStripeCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  テンポラリバッファサイズを取得します。
    //! @return テンポラリバッファサイズ
    //---------------------------------------------------------------------------
    size_t GetTemporaryBufferSize() const NN_NOEXCEPT
    {
        return m_TemporaryBufferSize;
    }

    //---------------------------------------------------------------------------
    //! @brief  Gpuバッファサイズを設定します。
    //! @return Gpuバッファサイズサイズ
    //---------------------------------------------------------------------------
    size_t GetGpuBufferSize() const NN_NOEXCEPT
    {
        return m_GpuBufferSize;
    }

    //---------------------------------------------------------------------------
    //! @brief  パーティクルソート用のバッファ長（ソート可能な Particle の個数）を取得します。
    //! @return パーティクルソート用のバッファ長
    //---------------------------------------------------------------------------
    size_t GetParticleSortBufferCount() const NN_NOEXCEPT
    {
        return m_SortBufferCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  ダブルバッファサイズを取得します。(廃止予定)
    //! @return ダブルバッファサイズ
    //---------------------------------------------------------------------------
    size_t GetDoubleBufferSize() const NN_NOEXCEPT
    {
        return GetTemporaryBufferSize();
    }

    //---------------------------------------------------------------------------
    //! @brief  ランタイムログ出力を抑制するかどうかを取得します。
    //! @return ランタイムログ出力を押さえるかどうかを示すbool値
    //---------------------------------------------------------------------------
    bool IsSuppressionLog() const NN_NOEXCEPT
    {
        return m_SuppressLog;
    }

    //---------------------------------------------------------------------------
    //! @brief  マルチコア動作させるかどうか取得します。
    //! @return マルチコア動作させるかどうか
    //---------------------------------------------------------------------------
    int GetProcessingCount() const NN_NOEXCEPT
    {
        return m_ProcessingCount;
    }

    //---------------------------------------------------------------------------
    //! @brief  トリプルバッファを利用するかどうか取得します。
    //! @return トリプルバッファが有効かどうか
    //---------------------------------------------------------------------------
    bool IsEnableTripleBuffer() const NN_NOEXCEPT
    {
        return m_IsEnableTripleBuffer;
    }

    //---------------------------------------------------------------------------
    //! @brief      （※旧 API）定数バッファのCpuキャッシュフラッシュを行うかどうか。
    //! @deprecated 旧型式の API 名です。nn::vfx::Config::IsConstantBufferCpuCacheFlush() に乗り換えてください。
    //! @return     使用する場合 true
    //! @see        nn::vfx::Config::IsConstantBufferCpuCacheFlush()
    //---------------------------------------------------------------------------
    NN_DEPRECATED bool IsConstantBufferCpuChacheFlush() const NN_NOEXCEPT
    {
        return IsConstantBufferCpuCacheFlush();
    }

    //---------------------------------------------------------------------------
    //! @brief      定数バッファのCpuキャッシュフラッシュを行うかどうか。
    //! @return     使用する場合 true
    //---------------------------------------------------------------------------
    bool IsConstantBufferCpuCacheFlush() const NN_NOEXCEPT
    {
        return m_IsConstantBufferCpuCacheFlush;
    }

    //---------------------------------------------------------------------------
    //! @brief              定数バッファのCpuキャッシュフラッシュを行うかどうか。
    //! @deprecated         旧型式の API 名です。nn::vfx::Config::SetConstantBufferGpuCacheFlush() に乗り換えてください。
    //! @param[in]  flag    使用する場合 true
    //! @see                nn::vfx::Config::SetConstantBufferGpuCacheFlush()
    //---------------------------------------------------------------------------
    NN_DEPRECATED void SetConstantBufferGpuChacheFlush( bool flag ) NN_NOEXCEPT
    {
        SetConstantBufferGpuCacheFlush( flag );
    }

    //---------------------------------------------------------------------------
    //! @brief              定数バッファのCpuキャッシュフラッシュを行うかどうか。
    //! @param[in]  flag    使用する場合 true
    //---------------------------------------------------------------------------
    void SetConstantBufferGpuCacheFlush( bool flag ) NN_NOEXCEPT
    {
        m_IsConstantBufferGpuCacheFlush = flag;
    }

    //---------------------------------------------------------------------------
    //! @brief      定数バッファのGpuキャッシュフラッシュを行うかどうか。
    //! @deprecated 旧型式の API 名です。nn::vfx::Config::IsConstantBufferGpuCacheFlush() に乗り換えてください。
    //! @return     使用する場合 true
    //! @see        nn::vfx::Config::IsConstantBufferGpuCacheFlush()
    //---------------------------------------------------------------------------
    NN_DEPRECATED bool IsConstantBufferGpuChacheFlush() const NN_NOEXCEPT
    {
        return IsConstantBufferGpuCacheFlush();
    }

    //---------------------------------------------------------------------------
    //! @brief      定数バッファのGpuキャッシュフラッシュを行うかどうか。
    //! @return     使用する場合 true
    //---------------------------------------------------------------------------
    bool IsConstantBufferGpuCacheFlush() const NN_NOEXCEPT
    {
        return m_IsConstantBufferGpuCacheFlush;
    }

    //---------------------------------------------------------------------------
    //! @brief  ストリームアウト処理を一括で行うモードが有効かどうか。
    //! @return 使用する場合 true
    //---------------------------------------------------------------------------
    bool IsEnableComputeShaderBatchProcess() const NN_NOEXCEPT
    {
        return m_IsComputeShaderBatchProcess;
    }

private:
    nn::gfx::Device*            m_pDevice;                          //!< デバイス
    Heap*                       m_pHeap;                            //!< メモリ確保先ヒープ
    Heap*                       m_pDynamicHeap;                     //!< 動的メモリ確保先ヒープ
    int                         m_EmitterCount;                     //!< 最大同時計算エミッタ数
    int                         m_EmitterSetCount;                  //!< 最大同時計算エミッタセット数
    int                         m_StripeCount;                      //!< 最大同時計算 履歴式ストライプ 数
    int                         m_SuperStripeCount;                 //!< 最大同時計算 履歴式ストライプ2 数
    int                         m_ResourceCount;                    //!< 最大リソース数
    size_t                      m_TemporaryBufferSize;              //!< テンポラリバッファのサイズ
    int                         m_SortBufferCount;                  //!< ソート用バッファのサイズ（パーティクル数）
    bool                        m_SuppressLog;                      //!< ランタイムログ出力を抑制します。
    int                         m_ProcessingCount;                  //!< マルチコア動作するかどうか。
    bool                        m_IsEnableTripleBuffer;             //!< トリプルバッファが有効かどうか。
    bool                        m_IsConstantBufferCpuCacheFlush;    //!< 定数バッファのCpuキャッシュフラッシュを行うかどうか
    bool                        m_IsConstantBufferGpuCacheFlush;    //!< 定数バッファのGpuキャッシュフラッシュを行うかどうか
    bool                        m_IsComputeShaderBatchProcess;      //!< ストリームアウト処理を一括で行うかどうか
    size_t                      m_GpuBufferSize;                    //!< Gpuバッファサイズ
};

} // namespace vfx
} // namespace nn
