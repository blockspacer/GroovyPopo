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

#include <nn/gfx/util/gfx_PrimitiveShape.h>

#include <nn/font/font_WideTextWriter.h>
#include <nn/font/font_RectDrawer.h>
#include <nn/font/font_DispStringBuffer.h>

#include <nn/ui2d/ui2d_Types.h>
#include <nn/ui2d/ui2d_ShaderInfo.h>
#include <nn/ui2d/ui2d_Resources.h>

namespace nn
{
namespace ui2d
{

class Layout;

//! @brief シェーダの種類を識別するための ID です。
enum ShaderId
{
    // ShaderId_DoubleTextureN の N が シェーダー内の、NW_TEXTURE_COMBINE_TYPE の値と一致している
    ShaderId_NullTexture = 0,
    ShaderId_SingleTexture,
    ShaderId_DoubleTexture1,
    ShaderId_DoubleTexture2,
    ShaderId_DoubleTexture3,
    ShaderId_DoubleTexture4,
    ShaderId_DoubleTexture5,
    ShaderId_DoubleTexture6,
    ShaderId_DoubleTexture7,
    ShaderId_DoubleTexture8,
    ShaderId_DoubleTexture9,
    ShaderId_DoubleTexture10,
    ShaderId_DoubleTexture11,
    ShaderId_DoubleIndirectTexture,
    ShaderId_MaxShaderId,
    ShaderId_Undefined = ShaderId_MaxShaderId, // バリエーション数を節約するため、ShaderId_MaxShaderId の領域を流用する
    ShaderId_ArchiveShader,
    ShaderId_Max,
};
NN_STATIC_ASSERT(ShaderId_Max <= 16);

const ShaderId ShaderIdDoubleTextureBaseIdx = ShaderId_DoubleTexture1;

//! @brief プリセットの BlendState を識別するための ID です。
enum PresetBlendStateId
{
    PresetBlendStateId_Default, //!< 半透明(初期値)
    PresetBlendStateId_OpaqueOrAlphaTest, //!< 不透明、抜き
    PresetBlendStateId_Addition, //!< 加算
    PresetBlendStateId_Subtraction, //!< 減算
    PresetBlendStateId_Multiplication, //!< 乗算
    PresetBlendStateId_SemitransparencyMaxAlpha, //!< 半透明(Maxアルファ)
    PresetBlendStateId_MaxPresetBlendStateId,

    PresetBlendStateId_None,
};

//! @brief プリセットの Vector Graphics 用 DepthStencilState を識別するための ID です。
enum PresetVectorGraphicsDepthStencilStateId
{
    PresetVectorGraphicsDepthStencilStateId_StencilPass,    //!<    ステンシルパス用
    PresetVectorGraphicsDepthStencilStateId_FillPass,       //!<    塗りつぶしパス用
    PresetVectorGraphicsDepthStencilStateId_Stroke,         //!<    ストローク用

    PresetVectorGraphicsDepthStencilStateId_Max
};

//! @brief アーカイブシェーダの種類を識別するための ID です。
enum ArchiveShaderId
{
    // 標準のシェーダには含まれず、シェーダー内の NW_TEXTURE_COMBINE_TYPE、NW_TEXTURE_COMBINE_TYPE2 の値と一致している
    ArchiveShaderId_DetailedCombinerNonSource = 100,
    ArchiveShaderId_DetailedCombinerSingleSource,
    ArchiveShaderId_DetailedCombinerDoubleSource,
    ArchiveShaderId_DetailedCombinerTripleSource,

    ArchiveShaderId_DetailedCombinerBaseIdx = ArchiveShaderId_DetailedCombinerNonSource,
};

//! @brief GraphicsResource が保持しているサンプラをインデックスでアクセスするための Id です。
//!
//! @details
//! 0 から PresetSamplerId_MirrorU_MirrorV_MinLinear_MagLinaer_MipPoint までは通常使用のサンプラーのバリエーションが定義されています。
//! PresetSamplerId_NormalSamplerMax より大きなインデックスは特別な用途のためのサンプラーです。
//!
enum PresetSamplerId
{
    PresetSamplerId_ClampToEdgeU_ClampToEdgeV_MinPoint_MagPoint_MipPoint,
    PresetSamplerId_ClampToEdgeU_ClampToEdgeV_MinLinear_MagPoint_MipPoint,
    PresetSamplerId_ClampToEdgeU_ClampToEdgeV_MinPoint_MagLinaer_MipPoint,
    PresetSamplerId_ClampToEdgeU_ClampToEdgeV_MinLinear_MagLinaer_MipPoint,

    PresetSamplerId_ClampToEdgeU_RepeatV_MinPoint_MagPoint_MipPoint,
    PresetSamplerId_ClampToEdgeU_RepeatV_MinLinear_MagPoint_MipPoint,
    PresetSamplerId_ClampToEdgeU_RepeatV_MinPoint_MagLinaer_MipPoint,
    PresetSamplerId_ClampToEdgeU_RepeatV_MinLinear_MagLinaer_MipPoint,

    PresetSamplerId_ClampToEdgeU_MirrorV_MinPoint_MagPoint_MipPoint,
    PresetSamplerId_ClampToEdgeU_MirrorV_MinLinear_MagPoint_MipPoint,
    PresetSamplerId_ClampToEdgeU_MirrorV_MinPoint_MagLinaer_MipPoint,
    PresetSamplerId_ClampToEdgeU_MirrorV_MinLinear_MagLinaer_MipPoint,

    PresetSamplerId_RepeatU_ClampToEdgeV_MinPoint_MagPoint_MipPoint,
    PresetSamplerId_RepeatU_ClampToEdgeV_MinLinear_MagPoint_MipPoint,
    PresetSamplerId_RepeatU_ClampToEdgeV_MinPoint_MagLinaer_MipPoint,
    PresetSamplerId_RepeatU_ClampToEdgeV_MinLinear_MagLinaer_MipPoint,

    PresetSamplerId_RepeatU_RepeatV_MinPoint_MagPoint_MipPoint,
    PresetSamplerId_RepeatU_RepeatV_MinLinear_MagPoint_MipPoint,
    PresetSamplerId_RepeatU_RepeatV_MinPoint_MagLinaer_MipPoint,
    PresetSamplerId_RepeatU_RepeatV_MinLinear_MagLinaer_MipPoint,

    PresetSamplerId_RepeatU_MirrorV_MinPoint_MagPoint_MipPoint,
    PresetSamplerId_RepeatU_MirrorV_MinLinear_MagPoint_MipPoint,
    PresetSamplerId_RepeatU_MirrorV_MinPoint_MagLinaer_MipPoint,
    PresetSamplerId_RepeatU_MirrorV_MinLinear_MagLinaer_MipPoint,

    PresetSamplerId_MirrorU_ClampToEdgeV_MinPoint_MagPoint_MipPoint,
    PresetSamplerId_MirrorU_ClampToEdgeV_MinLinear_MagPoint_MipPoint,
    PresetSamplerId_MirrorU_ClampToEdgeV_MinPoint_MagLinaer_MipPoint,
    PresetSamplerId_MirrorU_ClampToEdgeV_MinLinear_MagLinaer_MipPoint,

    PresetSamplerId_MirrorU_RepeatV_MinPoint_MagPoint_MipPoint,
    PresetSamplerId_MirrorU_RepeatV_MinLinear_MagPoint_MipPoint,
    PresetSamplerId_MirrorU_RepeatV_MinPoint_MagLinaer_MipPoint,
    PresetSamplerId_MirrorU_RepeatV_MinLinear_MagLinaer_MipPoint,

    PresetSamplerId_MirrorU_MirrorV_MinPoint_MagPoint_MipPoint,
    PresetSamplerId_MirrorU_MirrorV_MinLinear_MagPoint_MipPoint,
    PresetSamplerId_MirrorU_MirrorV_MinPoint_MagLinaer_MipPoint,
    PresetSamplerId_MirrorU_MirrorV_MinLinear_MagLinaer_MipPoint,

    PresetSamplerId_ClampToTransparentBorderColor,  //!< 透明な黒でクランプされます。

    PresetSamplerId_Max
};


//! @brief シェーダの種類とバリエーションからシェーダのインデックスを取得します。
//!
//! @param[in] id           シェーダの種類です。
//! @param[in] variation    バリエーションです。
//!
//! @return シェーダのインデックスです。
//!
inline int GetShaderIndex(ShaderId id, ShaderVariation variation)
{
    return static_cast<int>(id) + (static_cast<int>(variation) * static_cast<int>(ShaderId_MaxShaderId));
}



//---------------------------------------------------------------------------
//! @brief 複数の %Layout で共通に使用される 描画関係 のリソースを保持するクラスです。
//!
//! @details
//! DrawInfo::SetGraphicsResource() で DrawInfo に設定します。
//!
//---------------------------------------------------------------------------
class GraphicsResource
{
    NN_DISALLOW_COPY(GraphicsResource);

public:

    // バッファオブジェクトのインデックスです。

    //! @brief 内部用機能のため使用禁止です。
    enum VertexBuffer
    {
        VertexBuffer_Element,
        VertexBuffer_VertexIndex,
        VertexBuffer_MaxVertexBuffer
    };

    static const int VertexBufferIndexCount = 6; //!< 頂点バッファオブジェクトのインデックス数です。
    static const int VertexAttributeCount = 1; //!< 頂点属性の数です。

    static PresetBlendStateId DefalutPresetBlendStateId; //!< デフォルトプリセットの BlendState の ID です。

    //! @brief      GraphicsResource が必要なメモリープールのサイズを取得します。
    //!
    //! @param[in]  pDevice         デバイス
    //! @param[in] charMax 1度に描画できる文字数です。
    //!
    //! @return     メモリープールのサイズを返します。
    //!
    //! @sa         Setup
    static size_t CalculateMemoryPoolSize(nn::gfx::Device* pDevice, uint32_t charMax);

    //! @brief GraphicsResource用のメモリプールアライメントを計算します。
    //!
    //! @param[in] pDevice      デバイスです。
    //! @return メモリプールアライメントが返ります。
    //!
    //! @sa         Setup
    static size_t CalculateMemoryPoolAlignment(nn::gfx::Device* pDevice);

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    GraphicsResource();

    //! @brief デストラクタです。
    //!
    ~GraphicsResource();

    //@}

    //----------------------------------------
    //! @name 初期化／開放
    //!
    //@{

    //! @brief セットアップを行います。
    //!
    //! @param[in] pDevice          gfxデバイス
    //! @param[in] charMax          レイアウト内で描画可能な文字の最大数
    //!
    void Setup(nn::gfx::Device* pDevice, int charMax)
    {
        Setup(pDevice, charMax, NULL, 0, 0, NULL);
    }

    //! @brief セットアップを行います。
    //!
    //! @details    メモリープールは、nn::gfx::MemoryPoolProperty_CpuUncached、nn::gfx::MemoryPoolProperty_GpuCached プロパティを持つプールを指定してください。
    //!             サイズは、CalculateMemoryPoolSize() で、オフセットのアラインメントは、CalculateMemoryPoolAlignment() で求めます 。
    //!
    //! @param[in] pDevice          gfxデバイス
    //! @param[in] charMax          レイアウト内で描画可能な文字の最大数
    //! @param[in] pExternalMemoryPool メモリープールです。
    //! @param[in] externalMemoryPoolOffset 利用するメモリープール領域へのオフセットです。
    //! @param[in] externalMemoryPoolSize メモリープールのサイズです。
    //!
    void Setup(nn::gfx::Device* pDevice, int charMax,
        nn::gfx::MemoryPool* pExternalMemoryPool, ptrdiff_t externalMemoryPoolOffset, size_t externalMemoryPoolSize)
    {
        Setup(pDevice, charMax, pExternalMemoryPool, externalMemoryPoolOffset, externalMemoryPoolSize, NULL);
    }

    //! @brief アプリ側で作成した RectDrawer を設定してセットアップを行います。
    //!
    //! @details    メモリープールは、nn::gfx::MemoryPoolProperty_CpuUncached、nn::gfx::MemoryPoolProperty_GpuCached プロパティを持つプールを指定してください。
    //!             サイズは、CalculateMemoryPoolSize() で、オフセットのアラインメントは、CalculateMemoryPoolAlignment() で求めます。
    //!             RectDrawer の Initialize および Finalize はアプリ側で行ってください。
    //!
    //! @param[in] pDevice          gfxデバイス
    //! @param[in] charMax          レイアウト内で描画可能な文字の最大数
    //! @param[in] pExternalMemoryPool メモリープールです。
    //! @param[in] externalMemoryPoolOffset 利用するメモリープール領域へのオフセットです。
    //! @param[in] externalMemoryPoolSize メモリープールのサイズです。
    //! @param[in] pFontDrawer      アプリ側で作成した RectDrawer です。
    //!
    void Setup(nn::gfx::Device* pDevice, int charMax,
           nn::gfx::MemoryPool* pExternalMemoryPool, ptrdiff_t externalMemoryPoolOffset, size_t externalMemoryPoolSize,
           nn::font::RectDrawer *pFontDrawer
    )
    {
        Setup(pDevice, charMax, pExternalMemoryPool, externalMemoryPoolOffset, externalMemoryPoolSize, pFontDrawer, 0.0f);
    }

    //! @brief アプリ側で作成した RectDrawer を設定してセットアップを行います。
    //!
    //! @details    メモリープールは、nn::gfx::MemoryPoolProperty_CpuUncached、nn::gfx::MemoryPoolProperty_GpuCached プロパティを持つプールを指定してください。
    //!             サイズは、CalculateMemoryPoolSize() で、オフセットのアラインメントは、CalculateMemoryPoolAlignment() で求めます。
    //!             RectDrawer の Initialize および Finalize はアプリ側で行ってください。
    //!
    //!             lodBias に渡した値は、nn::gfx::SamplerInfo::SetLodBias に設定されます。
    //!             ミップマップを利用していて LodBias を調整したい場合にお使いください。
    //!
    //! @param[in] pDevice          gfxデバイス
    //! @param[in] charMax          レイアウト内で描画可能な文字の最大数
    //! @param[in] pExternalMemoryPool メモリープールです。
    //! @param[in] externalMemoryPoolOffset 利用するメモリープール領域へのオフセットです。
    //! @param[in] externalMemoryPoolSize メモリープールのサイズです。
    //! @param[in] pFontDrawer      アプリ側で作成した RectDrawer です。
    //! @param[in] lodBias          算出されるミップマップレベルに適用するオフセット値です。
    //!
    void Setup(nn::gfx::Device* pDevice, int charMax,
           nn::gfx::MemoryPool* pExternalMemoryPool, ptrdiff_t externalMemoryPoolOffset, size_t externalMemoryPoolSize,
           nn::font::RectDrawer *pFontDrawer, float lodBias
    );

    //! @brief リソースを開放します。
    //!
    //! @details
    //! デストラクタの呼び出し以前に呼び出す必要があります。
    //! アプリ側で作成した RectDrawer は解放されません。
    //!
    //! @param[in] pDevice          デバイスです。
    //!
    void Finalize(nn::gfx::Device* pDevice);

    //! @brief 初期化が済んでいるか調べます。
    //!
    //! @return 初期化が済んでいれば true を返します。
    //!
    bool Initialized() const
    {
        return m_Initialized;
    }

    //! @brief レイアウトモジュールが共通で利用するディスクリプタスロットを確保します。
    //!
    //! @details
    //! レイアウトモジュールが共通で利用するディスクリプタスロットを確保します。
    //! レイアウトモジュールの利用開始時に呼び出してください。すでに初期化済みの場合は何も処理を行いません。
    //!
    //! @param[in] pRegisterSamplerSlot   確保関数です。
    //! @param[in] pUserData     関数に渡されるユーザーデーターです。
    //!
    //! @sa UnregisterCommonSamplerSlot
    //!
    void RegisterCommonSamplerSlot(RegisterSamplerSlot pRegisterSamplerSlot, void* pUserData);

    //! @brief レイアウトモジュールが共通で利用するディスクリプタスロットを確保します。
    //!
    //! @details
    //! レイアウトモジュールが共通で利用するディスクリプタスロットを確保します。
    //! レイアウトモジュールの利用開始時に呼び出してください。すでに初期化済みの場合は何も処理を行いません。
    //! RegisterCommonSamplerSlot と比較して、本関数は、Sampler オブジェクトの生成や管理をユーザープログラム側で行いたい場合に利用します。
    //!
    //! @param[in] pAcquireSamplerSlot   確保関数です。
    //! @param[in] pUserData             関数に渡されるユーザーデーターです。
    //!
    //! @sa ReleaseCommonSamplerSlot
    //! @sa RegisterCommonSamplerSlot
    //!
    void AcquireCommonSamplerSlot(AcquireSamplerSlot pAcquireSamplerSlot, void* pUserData);

    //! @brief レイアウトモジュールが共通で利用するディスクリプタスロットを解放します。
    //!
    //! @param[in] pUnregisterSamplerSlot   解放関数です。
    //! @param[in] pUserData     関数に渡されるユーザーデーターです。
    //!
    //! @details
    //! レイアウトモジュールが共通で利用するディスクリプタスロットを解放します。
    //! レイアウトモジュールの利用終了時に呼び出してください。すでに解放済みの場合は何も処理を行いません。
    //!
    //! @sa RegisterCommonSamplerSlot
    //!
    void UnregisterCommonSamplerSlot(UnregisterSamplerSlot pUnregisterSamplerSlot, void* pUserData);

    //! @brief レイアウトモジュールが共通で利用するディスクリプタスロットを解放します。
    //!
    //! @param[in] pReleaseSamplerSlot   解放関数です。
    //! @param[in] pUserData                       関数に渡されるユーザーデーターです。
    //!
    //! @details
    //! レイアウトモジュールが共通で利用するディスクリプタスロットを解放します。
    //! レイアウトモジュールの利用終了時に呼び出してください。すでに解放済みの場合は何も処理を行いません。
    //! UnregisterCommonSamplerSlot と比較して、本関数は、Sampler オブジェクトの生成や管理をユーザープログラム側で行いたい場合に利用します。
    //!
    //! @sa AcquireCommonSamplerSlot
    //! @sa UnregisterCommonSamplerSlot
    //!
    void ReleaseCommonSamplerSlot(ReleaseSamplerSlot pReleaseSamplerSlot, void* pUserData);

    //! @brief サンプラー用のディスクリプタスロットを取得します。
    //!
    //! @param[in] wrapS    テクスチャの S 方向のラップモードです。
    //! @param[in] wrapT    テクスチャの T 方向のラップモードです。
    //! @param[in] minFilter   テクスチャが縮小されるときに適用されるフィルタモードです。
    //! @param[in] magFilter   テクスチャが拡大されるときに適用されるフィルタモードです。
    //!
    //! @return サンプラー用ディスクリプタスロットです。
    //!
    nn::gfx::DescriptorSlot& GetSamplerDescriptorSlot(TexWrap wrapS, TexWrap wrapT, TexFilter minFilter, TexFilter magFilter) const;

    //! @brief  サンプラー用のディスクリプタスロットを取得します。
    //!
    //! @param[in]  id 取得するサンプラーの ID です。
    //!
    //! @return サンプラー用ディスクリプタスロットです。
    //!
    nn::gfx::DescriptorSlot& GetSamplerDescriptorSlot(PresetSamplerId id) const;

    //@}

    // TextBox ペインの描画に使用する font::RectDrawer を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return font::RectDrawer への参照を返します。
    //!
    nn::font::RectDrawer& GetFontDrawer()
    {
        return *m_pFontDrawer;
    }

    //! @brief 共通シェーダ情報を取得します。
    //!
    //! @return 共通シェーダ情報です。
    //!
    const ShaderInfo* GetCommonShaderInfo() const { return &m_CommonShaderInfo; }

    //! @brief インデックスバッファを取得します。
    //!
    //! @return インデックスバッファです。
    //!
    const nn::gfx::Buffer* GetIndexBuffer() const { return &m_IndexBuffer; }

    //! @brief インデックスバッファの GPU アドレスを取得します。
    //!
    //! @return インデックスバッファの GPU アドレスです。
    //!
    const nn::gfx::GpuAddress* GetIndexBufferGpuAddress() const { return &m_IndexBufferGpuAddress; }

    //! @brief コンスタントバッファのアライメントを取得します。
    //!
    //! @return コンスタントバッファのアライメント値。
    //!
    size_t GetConstantBufferAlignment() const
    {
        return m_ConstantBufferAlignment;
    }

    //! @brief 頂点バッファのアライメントを取得します。
    //!
    //! @return 頂点バッファのアライメント値。
    //!
    size_t GetVertexBufferAlignment() const
    {
        return m_VertexBufferAlignment;
    }

    //! @brief インデックスバッファのアライメントを取得します。
    //!
    //! @return インデックスバッファのアライメント値。
    //!
    size_t GetIndexBufferAlignment() const
    {
        return m_IndexBufferAlignment;
    }

    //! @brief プリセットの BlendState を取得します。
    //!
    //! @param[in] presetBlendStateId プリセットの BlendState の ID です。
    //!
    //! @return プリセットの BlendState です。
    //!
    nn::gfx::BlendState* GetPresetBlendState(PresetBlendStateId presetBlendStateId);

    // プリセットの Vector Graphics 用 DepthStencilState を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] id   プリセットの ID です。
    //!
    //! @return プリセットの DepthStencilState です。
    //!
    const nn::gfx::DepthStencilState* GetPresetVectorGraphicsDepthStencilState(PresetVectorGraphicsDepthStencilStateId id) const
    {
        NN_SDK_ASSERT(0 <= id && id < PresetVectorGraphicsDepthStencilStateId_Max);
        return &m_PresetVectorGraphicsDepthStencilState[id];
    }

    //! @brief プリセットの BlendState の ID を取得します。
    //!
    //! @param[in] pBlendMode BlendMode です。
    //! @param[in] pBlendModeAlpha アルファの BlendMode です。
    //!
    //! @return プリセットの BlendState の ID です。存在しない場合は PresetShaderBlendId_None が返ります。
    //!
    static PresetBlendStateId GetPresetBlendStateId(const ResBlendMode* pBlendMode, const ResBlendMode* pBlendModeAlpha);

    //! @brief デフォルトプリセットの BlendState の ID を設定します。（内部評価用につき利用しないでください）
    //!
    //! @param[in] defaultStateId デフォルトプリセットの BlendState の ID です。
    //!
    static void SetupDefaultPresetBlendStateId(PresetBlendStateId defaultStateId)
    {
        DefalutPresetBlendStateId = defaultStateId;
    }

    //! @brief デフォルトプリセットの BlendState の ID を取得します。（内部評価用につき利用しないでください）
    //!
    //! @return デフォルトプリセットの BlendState の ID です。
    //!
    static PresetBlendStateId GetDefaultPresetBlendStateId()
    {
        return DefalutPresetBlendStateId;
    }

    //! @brief BlendStateInfo を設定します。
    //!
    //! @param[in] pBlendStateInfo BlendStateInfo です。
    //! @param[in] pBlendTargetStateInfo BlendTargetStateInfo です。
    //! @param[in] pBlendMode BlendMode です。
    //! @param[in] pBlendModeAlpha アルファの BlendMode です。
    //!
    //! @return BlendState に設定する必要があるメモリのサイズを返します。
    //!
    static size_t SetupBlendStateInfo(nn::gfx::BlendState::InfoType* pBlendStateInfo, nn::gfx::BlendTargetStateInfo* pBlendTargetStateInfo, const ResBlendMode* pBlendMode, const ResBlendMode* pBlendModeAlpha);

private:
    friend class DrawInfo;

    // 共通で利用するバッファオブジェクトを初期化します。

    //! @brief 内部用機能のため使用禁止です。
    void InitializeVertexBuffer(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pExternalMemoryPool, ptrdiff_t externalMemoryPoolOffset, size_t externalMemoryPoolSize);

    // Vector Graphics で使用するデプスステンシルステートを初期化します。

    //! @brief 内部用機能のため使用禁止です。
    void InitializeVectorGraphicsDepthStencilState(nn::gfx::Device* pDevice);

    // 共通で利用するバッファオブジェクトを有効化します。

    //! @brief 内部用機能のため使用禁止です。
    void ActivateVertexBuffer(nn::gfx::CommandBuffer* pCommandBuffer) const;

    // プリセットの BlendMode の配列を取得します。

    //! @brief 内部用機能のため使用禁止です。
    static void GetPresetBlendModeArray(const ResBlendMode** ppPresetBlendModeArray, const ResBlendMode** ppPresetBlendModeAlphaArray);

private:

    ShaderInfo m_CommonShaderInfo; // 共通シェーダ
    void* m_pUi2dBuildinShader;
    void** m_pConstantBufferMemories;
    void* m_pBufferMemory;
    void* m_pRectShaderBinary;
    int32_t m_RectShaderBinarySize;
    size_t  m_ConstantBufferAlignment;
    size_t  m_VertexBufferAlignment;
    size_t  m_IndexBufferAlignment;
    nn::font::RectDrawer* m_pFontDrawer;

    nn::gfx::MemoryPool m_MemoryPoolForBuffers;

    nn::gfx::Buffer m_VertexBuffer;
    nn::gfx::GpuAddress m_VertexBufferGpuAddress;

    nn::gfx::Buffer m_IndexBuffer;
    nn::gfx::GpuAddress m_IndexBufferGpuAddress;

    nn::gfx::Sampler*                        m_pSamplerTable;
    nn::gfx::DescriptorSlot*                 m_pSamplerDescriptorSlotTable;

    float                                    m_SamplerLodBias;

    nn::gfx::BlendState m_PresetBlendState[PresetBlendStateId_MaxPresetBlendStateId];

    nn::gfx::DepthStencilState  m_PresetVectorGraphicsDepthStencilState[PresetVectorGraphicsDepthStencilStateId_Max];

    bool m_Initialized;
    bool m_IsDefaultRectDrawerUsed;
};

} // namespace nn::ui2d
} // namespace nn
