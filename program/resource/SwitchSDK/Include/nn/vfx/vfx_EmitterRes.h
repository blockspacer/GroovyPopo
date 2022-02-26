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
#include <nn/vfx/vfx_Binary.h>
#include <nn/vfx/vfx_TextureSampler.h>
#include <nn/vfx/vfx_Primitive.h>
#include <nn/vfx/vfx_UniformBlock.h>
#include <nn/vfx/vfx_RenderContext.h>


namespace nn {
namespace vfx {

class Resource;

namespace detail {

class Shader;
class ComputeShader;
class ShaderManager;
class ComputeShaderManager;

//------------------------------------------------------------------------------
//! @brief  フィールドの定数バッファ
//------------------------------------------------------------------------------
struct EmitterFieldConstantBuffer
{
    nn::util::Float4    randomParam;            //!< ランダム幅XYZ / 適用間隔
    nn::util::Float4    randomParam1;           //!< フィールドランダム ランダム周期比率（ 0 / 1 / 2 / 3 ）
    nn::util::Float4    randomParam2;           //!< フィールドランダム 合成重み（固定）（ 0 / 1 / 2 / 3 ）
    nn::util::Float4    randomParam3;           //!< 位相の統一 / 詳細設定有効 / / 空気抵抗の有無
    nn::util::Float4    randomParam4;           //!< 位相の変化速度 / 位相の分布 / /

    nn::util::Float4    randomFe1Param;         //!< ランダム幅XYZ / 適用間隔

    nn::util::Float4    posAddParam;            //!< 位置に加算 加算値XYZ / ワールド座標系で処理するかどうか

    nn::util::Float4    magnetParam;            //!< 磁力 位置XYZ / 磁力
    nn::util::Float4    magnetParam1;           //!< 磁力 エミッタ追従 / / /

    nn::util::Float4    convergenceParam;       //!< 収束 位置XYZ / 収束力
    nn::util::Float4    convergenceParam1;      //!< 収束 エミッタ追従 / / /

    nn::util::Float4    spinParam;              //!< スピン 回転速度 / 拡散速度 / 軸 /

    nn::util::Float4    collisionParam;         //!< シンプルコリジョン タイプ / 反射率 / 摩擦 / 反射座標

    nn::util::Float4    curlNoiseParam;         //!< カールノイズ パワーXYZ / スケール
    nn::util::Float4    curlNoiseParam1;        //!< カールノイズ スピードXYZ / オフセット
    nn::util::Float4    curlNoiseParam2;        //!< カールノイズ ノイズのオフセットランダム /

    nn::util::Float4    customFieldParam0;      //!< カスタムフィールド パラメータ0
    nn::util::Float4    customFieldParam1;      //!< カスタムフィールド パラメータ1
};

//---------------------------------------------------------------------------
//! @brief  エミッタ 動的定数バッファ
//---------------------------------------------------------------------------
struct EmitterDynamicConstantBuffer
{
    nn::util::Float4    emitterColor0;                              //!< エミッタカラー0(セット*アニメーションカラー0*フェードカラー)
    nn::util::Float4    emitterColor1;                              //!< エミッタカラー1(セット*アニメーションカラー1*フェードカラー)
    nn::util::Float4    emitterParam0;                              //!< エミッタパラメータ( time / emScaleX / emScaleY / frameRate )
    nn::util::Float4    emitterParam1;                              //!< エミッタパラメータ( fadeAlpha / empty / empty / empty )
    nn::util::Float4x4  emitterMatrix;                              //!< エミッタマトリクス
    nn::util::Float4x4  emitterMatrixRT;                            //!< エミッタマトリクス
};

} // namespace detail

//------------------------------------------------------------------------------
//! @brief デフォルトのカスタムアクションパラメータ
//!
//!        ユーザーデータ定義を使用しない場合の、カスタムアクションに渡されるデータです。
//------------------------------------------------------------------------------
struct ResCustomActionData
{
    uint32_t    bit;                    //!< ビット
    int         integerValue[ 6 ];      //!< 整数
    float       floatValue[ 8 ];        //!< 浮動小数点数

    //--------------------------------------------------------------------------
    //! @brief          エンディアンをフリップします.
    //! @param[in] size フリップするサイズ
    //--------------------------------------------------------------------------
    void FlipEndian( size_t size ) NN_NOEXCEPT
    {
#if defined( VFX_ENDIAN_LITTLE )
        uint32_t* temp = reinterpret_cast< uint32_t* >( &bit );
        int count      = static_cast< int >( size  / 4 );
        for( int i = 0; i < count; i++ )
        {
            nn::vfx::detail::EndianUtil::Flip( temp );
            temp++;
        }
#endif //VFX_ENDIAN_LITTLE
    }
};

//------------------------------------------------------------------------------
//! @brief デフォルトのエミッタ拡張パラメータ
//!
//!        ユーザーデータ定義を使用しない場合の、エミッタ拡張パラメータに渡されるデータです。
//------------------------------------------------------------------------------
struct ResEmitterExtParamData
{
    uint32_t    bit;                    //!< ビット
    int         integerValue[ 6 ];      //!< 整数
    float       floatValue[ 8 ];        //!< 浮動小数点数

    //--------------------------------------------------------------------------
    //! @brief          エンディアンをフリップします.
    //! @param[in] size フリップするサイズ
    //--------------------------------------------------------------------------
    void FlipEndian( size_t size ) NN_NOEXCEPT
    {
#if defined( VFX_ENDIAN_LITTLE )
        uint32_t* temp = reinterpret_cast< uint32_t* >( &bit );
        int count      = static_cast< int >( size  / 4 );
        for( int i = 0; i < count; i++ )
        {
            nn::vfx::detail::EndianUtil::Flip( temp );
            temp++;
        }
#endif //VFX_ENDIAN_LITTLE
    }
};


//------------------------------------------------------------------------------
//! @brief エミッタ管理データ
//!
//!       １つのエミッタに関するデータが集約された構造体です。
//------------------------------------------------------------------------------

// TODO : なるべく : class化に合わせて調整

class EmitterResource
{
    NN_DISALLOW_COPY( EmitterResource );
public:
    //------------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //------------------------------------------------------------------------------
    EmitterResource() NN_NOEXCEPT;

    bool                                m_IsInitialized;                                                            //!< 初期化済みかどうか
    bool                                m_IsUseEmitterAnim;                                                         //!< エミッタ時間アニメーションを含むか
    bool                                m_IsEmitterSrtAnim;                                                         //!< エミッタSRTアニメーションを含むか。
    bool                                m_IsUseField;                                                               //!< フィールドを含むか。
    bool                                m_IsChildEmitter;                                                           //!< チャイルドエミッタかどうか
    int                                 m_EmitterIndex;                                                             //!< エミッタインデックス( 親から見て何番目のエミッタか)
    detail::ResEmitter*                 m_pResEmitter;                                                              //!< エミッタリソース
    detail::EmitterStaticUniformBlock*  m_ResEmitterStaticConstantBuffer;                                           //!< エミッタ静的定数バッファ( COS&CPUエミッタの場合は実体ではなくコピー )
    detail::ConstantBuffer*             m_pResourceConstantBuffer;                                                  //!< リソース定数バッファ
    ptrdiff_t                           m_EmitterStaticBufferOffset;                                                //!< エミッタ静的定数バッファ オフセット
    ptrdiff_t                           m_EmitterPluginConstantBufferOffset;                                        //!< エミッタプラグインバッファ オフセット
    ptrdiff_t                           m_CustomShaderConstantBufferOffset;                                         //!< カスタムシェーダ定数バッファ オフセット
    ptrdiff_t                           m_FieldConstantBufferOffset;                                                //!< フィールド静的定数バッファ オフセット
    nn::gfx::DescriptorSlot             m_TextureDescSlot[ TextureSlotId_MaxStandardTextureSlotId ];                //!< テクスチャデスクリプタスロット
    detail::TextureSampler*             m_TextureSampler[ TextureSlotId_MaxStandardTextureSlotId ];                 //!< テクスチャサンプラー
    IPrimitive*                         m_pPrimitive;                                                               //!< 利用するプリミティブ
    Primitive*                          m_pVolumePrimitive;                                                         //!< エミッタ形状で利用するプリミティブ
    int                                 m_ChildEmitterResCount;                                                     //!< チャイルドリソース数
    EmitterResource*                    m_ChildEmitterResSet[ SystemParameters_MaxEmitterInclusionCount ];          //!< チャイルドエミッタリソースセット
    EmitterResource*                    m_NextEmitterResource;                                                      //!< 次のエミッタリソースセット
    detail::Rendercontext               m_RenderState;                                                              //!< レンダーステート
    nn::util::Matrix4x3fType            m_ResMatrixSrt;                                                             //!< リソース内で解決されるSRTマトリクス
    nn::util::Matrix4x3fType            m_ResMatrixRt;                                                              //!< リソース内で解決されるRTマトリクス
    bool                                m_IsVisible;                                                                //!< 表示/非表示
    detail::ResFieldRandom*             m_pFieldGpuNoiseData;                                                       //!< フィールドランダムデータ
    detail::ResFieldRandomFe1*          m_pFieldRandomSimpleData;                                                   //!< 旧フィールドランダムデータ
    detail::ResFieldMagnet*             m_pFieldMagnetData;                                                         //!< フィールド磁力データ
    detail::ResFieldSpinData*           m_pFieldSpinData;                                                           //!< フィールドスピンデータ
    detail::ResFieldCollisionData*      m_pFieldCollisionData;                                                      //!< フィールドコリジョンデータ
    detail::ResFieldConvergenceData*    m_pFieldConvergenceData;                                                    //!< フィールド収束データ
    detail::ResFieldPosAddData*         m_pFieldPosAddData;                                                         //!< フィールド位置加算データ
    detail::ResFieldCurlNoiseData*      m_pFieldCurlNoiseData;                                                      //!< フィールドカールノイズデータ
    detail::ResFieldCustom*             m_pFieldCustomData;                                                         //!< カスタムフィールドのデータ
    detail::ResAnimEmitterKeyParamSet*  m_EmitterAnimationArray[ detail::EmitterAnimationType_MaxAnimationType ];   //!< エミッタアニメーション配列
    void*                               m_pCustomShaderParam;                                                       //!< カスタムシェーダパラメータ
    size_t                              m_CustomShaderParamSize;                                                    //!< カスタムシェーダパラメータサイズ
    void*                               m_pCustomActionParam;                                                       //!< カスタムアクションパラメータ
    void*                               m_pCustomDataParam;                                                         //!< カスタムデータパラメータ
    nn::util::Vector3fType              m_InitialRotate;                                                            //!< 回転初期値
    int                                 m_EmitterPluginIndex;                                                       //!< エミッタプラグインインデックス
    void*                               m_pEmitterPluginData;                                                       //!< エミッタプラグインデータ
    detail::Shader*                     m_pShader[ShaderType_MaxShaderType];                                        //!< シェーダインスタンス
    detail::ComputeShader*              m_pComputeShader;                                                           //!< シェーダインスタンス
    void*                               m_pVertexStateBuffer;                                                       //!< 頂点ステート用バッファ
    size_t                              m_VertexStateBufferSize;                                                    //!< 頂点ステート用バッファサイズ
    nn::gfx::VertexState                m_VertexState;                                                              //!< Gfx頂点ステート

    int                                 m_ParticleFigureBufferSlot[   detail::ParticleFigureAttributeBufferIndex_MaxCount ];
    int                                 m_ParticlePropertyBufferSlot[ detail::ParticlePropertyAttributeBufferIndex_MaxIndexCount ];

    //---------------------------------------------------------------------------
    //! @brief      エミッタリソースのアドレス解決をします。
    //! @param[in]  pEmitterBinaryData エミッタバイナリデータ
    //---------------------------------------------------------------------------
    void ResolveBinaryData( nn::vfx::detail::BinaryData* pEmitterBinaryData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      レンダーステートリソースを初期化します。
    //! @param[in]  pDevice デバイスへのポインタ
    //---------------------------------------------------------------------------
    void InitializeRenderState( nn::gfx::Device* pDevice ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      レンダーステートリソースを破棄します。
    //! @param[in]  pDevice デバイスへのポインタ
    //---------------------------------------------------------------------------
    void FinalizeRenderState( nn::gfx::Device* pDevice ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  現状のバイナリリソースをもとに更新します。
    //! @param[in]  pResource セットアップに必要なリソースを保持するリソースクラスへのポインタ
    //---------------------------------------------------------------------------
    void UpdateParams( nn::vfx::Resource* pResource ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  シェーダリソースを更新します。
    //---------------------------------------------------------------------------
    bool UpdateShaderResource( detail::ShaderManager* shaderManager, detail::ComputeShaderManager* computeShaderManager ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  頂点ステートを初期化します。
    //! @param[in]  pDevice デバイスへのポインタ
    //---------------------------------------------------------------------------
    bool InitializeVertexState( nn::gfx::Device* pDevice ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  頂点ステートを破棄します。
    //! @param[in]  pDevice デバイスへのポインタ
    //---------------------------------------------------------------------------
    void FinalizeVertexState( nn::gfx::Device* pDevice ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  コンスタントバッファを初期化します。
    //! @param[in]  pConstantBuffer   バッファを切り出す先のコンスタントバッファ
    //---------------------------------------------------------------------------
    bool InitializeConstantBuffer( detail::ConstantBuffer* pConstantBuffer ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  エミッタリソースのセットアップを行います。
    //! @param[in]  pResource セットアップに必要なリソースを保持するリソースクラスへのポインタ
    //---------------------------------------------------------------------------
    bool Setup( nn::vfx::Resource* pResource ) NN_NOEXCEPT;
};


//------------------------------------------------------------------------------
//! @brief エミッタセット管理データ
//!
//!       １つのエミッタセットに関するデータが集約された構造体です。
//------------------------------------------------------------------------------
struct EmitterSetResource
{
    int                     emitterCount;                       //!< 親エミッタ数
    int                     emitterAllCount;                    //!< 総エミッタ数(子を含む)
    detail::BinaryData*     pEmitterSetBinary;                  //!< エミッタセットバイナリデータ
    detail::ResEmitterSet*  pResEmitterSet;                     //!< エミッタセットリソース
    EmitterResource*        pEmitterResource;                   //!< エミッタリソース
    void*                   pEmitterResourcePtr;                //!< エミッタリソース用メモリを確保したアドレス
    bool                    isVisible;                          //!< 表示/非表示
    bool                    isLoopEffect;                       //!< ループエフェクトかどうか
    bool                    isLifeInfinity;                     //!< 無限寿命エフェクトかどうか
    const char*             pFilePath;                          //!< エミッタセットのファイルパス
};


} // namespace vfx
} // namespace nn
