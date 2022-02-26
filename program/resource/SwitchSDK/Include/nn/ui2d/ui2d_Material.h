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

#include <nn/ui2d/ui2d_TexMap.h>
#include <nn/ui2d/ui2d_Resources.h>
#include <nn/ui2d/ui2d_GraphicsResource.h>

#include <nn/util/util_MathTypes.h>

namespace nn
{
namespace ui2d
{

class ResourceAccessor;
class CaptureTexture;
struct ResMaterial;
struct UserShaderInformation;
struct BuildResultInformation;

//! @brief Material インスタンスコピー時の追加情報です。
//!
//! @details
//! レイアウトインスタンスのコピー時にマテリアル内の動的生成テクスチャを適切にコピーするための情報を含んでいます。
//!
struct MaterialCopyContext
{
    //! デバイスです。
    nn::gfx::Device*    pDevice;
    //! 動的生成テクスチャを取得するためのリソースアクセッサです。
    ResourceAccessor *pResAccessor;
    //! 新たに所属するレイアウトへのポインタです。
    const Layout* pLayout;

    //! ペインツリー構築中に階層構造を超えて受け渡される情報です。
    detail::BuildPaneTreeContext* pBuildPaneTreeContext;
};

namespace detail
{

class VectorGraphicsTexture;
class BuildPaneTreeContext;

// Material Member Numbers
struct MatMemCount
{
    uint32_t  texMap: 2;
    uint32_t  texSrt: 2;
    uint32_t  texCoordGen: 2;
    uint32_t  tevStage: 3;
    uint32_t  alpComp: 1;
    uint32_t  blendMode: 2;
    uint32_t  indirectParameter: 1;
    uint32_t  projectionTexGen: 2;
    uint32_t  fontShadowParameter: 1;
    uint32_t  detailedCombinerParameter: 1;
    uint32_t  combinerUserShaderParameter : 1;
    uint32_t  refVectorGraphicsTexture : 2;
};

NN_STATIC_ASSERT(sizeof(MatMemCount) == 4);

// ベクターグラフィックステクスチャの参照情報
struct RefVectorGraphicsTextureInfo
{
    //! ベクターグラフィックステクスチャの再生フレーム
    float   frame;
    //! ベクターグラフィックステクスチャのクリアカラー
    nn::util::Float4    clearColor;
    //! 参照しているベクターグラフィックステクスチャのポインタ
    detail::VectorGraphicsTexture*  pReferencedVectorGraphicsTexture;
};

//----------------------------------------
inline int
CalculateOffsetTexSrtArray(const MatMemCount& bitCount)
{
    return sizeof(TexMap) * bitCount.texMap;
}

//----------------------------------------
inline int
CalculateOffsetTexCoordGenArray(const MatMemCount& bitCount)
{
    return CalculateOffsetTexSrtArray(bitCount) + sizeof(ResTexSrt) * bitCount.texSrt;
}

//----------------------------------------
inline int
CalculateOffsetAlphaCompare(const MatMemCount& bitCount)
{
    return CalculateOffsetTexCoordGenArray(bitCount) + sizeof(ResTexCoordGen) * bitCount.texCoordGen;
}

//----------------------------------------
inline int
CalculateOffsetBlendMode(const MatMemCount& bitCount)
{
    return CalculateOffsetAlphaCompare(bitCount) + sizeof(ResAlphaCompare) * bitCount.alpComp;
}

//----------------------------------------
inline int
CalculateOffsetIndirectParameter(const MatMemCount& bitCount)
{
    return CalculateOffsetBlendMode(bitCount) + sizeof(ResBlendMode) * bitCount.blendMode;
}

//----------------------------------------
inline int
CalculateOffsetTevStageArray(const MatMemCount& bitCount)
{
    return CalculateOffsetIndirectParameter(bitCount) + sizeof(ResIndirectParameter) * bitCount.indirectParameter;
}

//----------------------------------------
inline int
CalculateOffsetProjectionTexGen(const MatMemCount& bitCount)
{
    return CalculateOffsetTevStageArray(bitCount) + sizeof(ResTevStage) * bitCount.tevStage;
}

//----------------------------------------
inline int
CalculateOffsetFontShadowParameter(const MatMemCount& bitCount)
{
    return CalculateOffsetProjectionTexGen(bitCount) + sizeof(ResProjectionTexGenParameters) * bitCount.projectionTexGen;
}

//----------------------------------------
inline int
CalculateOffsetDetailedCombinerStageInfo(const MatMemCount& bitCount)
{
    return CalculateOffsetFontShadowParameter(bitCount) + sizeof(ResFontShadowParameter) * bitCount.fontShadowParameter;
}

//----------------------------------------
inline int
CalculateOffsetDetailedCombinerStage(const MatMemCount& bitCount)
{
    return CalculateOffsetDetailedCombinerStageInfo(bitCount) + sizeof(ResDetailedCombinerStageInfo) * bitCount.detailedCombinerParameter;
}

//----------------------------------------
inline int
CaluculateOffsetCombinerUserShader(const MatMemCount& bitCount)
{
    return CalculateOffsetDetailedCombinerStage(bitCount) + sizeof(ResDetailedCombinerStage) * bitCount.detailedCombinerParameter * bitCount.tevStage;
}

//----------------------------------------
inline int
CaluculateOffsetRefVectorGraphicsTextureInfo(const MatMemCount& bitCount)
{
    return CaluculateOffsetCombinerUserShader(bitCount) + sizeof(ResCombinerUserShader) * bitCount.combinerUserShaderParameter;
}



} // namespace nn::ui2d::detail

class AnimTransform;
class AnimResource;
class DrawInfo;
class Layout;
class TexMap;
struct BuildResSet;

//---------------------------------------------------------------------------
//! @brief マテリアルの情報クラスです。
//!
//---------------------------------------------------------------------------
class Material
{
public:
    //! @brief マテリアルが保持する情報のフラグです。
    enum Flags
    {
        //! ユーザーがマテリアルを構築したかを示します。
        Flags_UserAllocated,
        //! テクスチャのみを設定します。
        Flags_TextureOnly,
        //! 閾値によるアルファ補間が有効であるかを示します。
        Flags_ThresholdingAlphaInterpolation,
        //! 黒カラーを float カラーとして扱うかどうか。
        Flags_BlackColorFloat,
        //! 白カラーを float カラーとして扱うかどうか。
        Flags_WhiteColorFloat,
        //! カラーデータが動的に確保されたメモリ領域に保存されているかどうか。
        Flags_DynamicAllocatedColorData,
        //! 列挙子の総数です。
        Flags_MaxFlags
    };

    struct Instance;

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! デフォルト設定の状態で初期化します。
    //!
    Material();

    //! @brief コンストラクタです。
    //!
    //! @details
    //! データブロックを反映した状態で初期化します。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報です。
    //! @param[in] pDevice      デバイスです。
    //! @param[in] pBaseRes     マテリアルデータブロックへのポインタです。
    //! @param[in] pOverrideRes 上書き用のマテリアルデータブロックへのポインタです。
    //! @param[in] buildArgSet  BuildArgSet への参照です。
    //!
    Material(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        const ResMaterial* pBaseRes,
        const ResMaterial* pOverrideRes,
        const BuildArgSet& buildArgSet)
    {
        InitializeMaterialImpl(pOutBuildResultInformation, pDevice, pBaseRes, pOverrideRes, buildArgSet);
    }

    //! @brief コピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したマテリアルをコピーしたマテリアルを作成します。
    //! このコンストラクタを使用して異なるレイアウトに属するマテリアルとしてコピーされた場合、キャプチャテクスチャの参照情報が更新されず
    //! 初期化した際のレイアウトが保持しているキャプチャテクスチャを参照する状態となります。
    //! 別レイアウトのマテリアルとしてコピーする場合は MaterialCopyContext を指定するコピーコントラクタを使用してください。
    //!
    //! マテリアルの元々のレイアウトデータの状態ではなく、そのときの状態でコピー
    //! しますのでご注意ください。つまり、アニメーションを再生した後はそのままの
    //! 状態でコピーします。
    //!
    //! また、アニメーションについては、何もバインドされていない状態になります。
    //!
    //! @param[in] material コピー元のマテリアルです。
    //! @param[in] pDevice      デバイスです。
    //!
    explicit Material(const Material& material, nn::gfx::Device* pDevice);

    //! @brief コピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したマテリアルをコピーしたマテリアルを作成します。
    //! copyContext に適切なパラメータが設定されていれば、そのパラメータに基づきキャプチャテクスチャの参照を変更します。
    //!
    //! マテリアルの元々のレイアウトデータの状態ではなく、そのときの状態でコピー
    //! しますのでご注意ください。つまり、アニメーションを再生した後はそのままの
    //! 状態でコピーします。
    //!
    //! また、アニメーションについては、何もバインドされていない状態になります。
    //!
    //! @param[in] material コピー元のマテリアルです。
    //! @param[in] copyContext コピーに関する追加情報です。
    //!
    explicit Material(const Material& material, MaterialCopyContext& copyContext);

    //! @brief マテリアルのコピー処理の実装です。
    //!
    //! @details
    //! 引数で指定したマテリアルをコピーしたマテリアルを作成します。
    //! copyContext に適切なパラメータが設定されていれば、そのパラメータに基づきキャプチャテクスチャの参照を変更します。
    //!
    //! マテリアルの元々のレイアウトデータの状態ではなく、そのときの状態でコピー
    //! しますのでご注意ください。つまり、アニメーションを再生した後はそのままの
    //! 状態でコピーします。
    //!
    //! また、アニメーションについては、何もバインドされていない状態になります。
    //!
    //! @param[in] material コピー元のマテリアルです。
    //! @param[in] copyContext コピーに関する追加情報です。
    //!
    void CopyMaterialImpl(const Material& material, MaterialCopyContext& copyContext);

    //! @brief デストラクタです。
    //!
    virtual ~Material();

    // キャプチャテクスチャなどの動的レンダリングテクスチャの初期化処理を行います。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報です。
    //! @param[in] pDevice      デバイスです。
    //! @param[in] buildArgSet  BuildArgSet への参照です。
    //!
    void InitializeDynamicRenderingTexture(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        const BuildArgSet& buildArgSet);

    //! @brief リソースを開放します。
    //!
    //! @details
    //! デストラクタの呼び出し以前に呼び出す必要があります。
    //!
    //! @param[in] pDevice          デバイスです。
    //!
    void Finalize(nn::gfx::Device* pDevice);

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief マテリアルの設定を保持する領域を確保します。
    //!
    //! @param[in] texMapCount 格納可能なテクスチャの数です。
    //! @param[in] texSrtCount 格納可能なテクスチャ SRT 変換行列の数です。
    //! @param[in] texCoordGenCount 格納可能なテクスチャ座標生成の数です。
    //! @param[in] tevStageCount 格納可能な TEV ステージの数です。
    //! @param[in] allocAlpComp アルファコンペアの設定領域を確保するには true を指定します。
    //! @param[in] blendModeCount ブレンドモードの数です。
    //! @param[in] allocIndirectParameter インダイレクトのパラメータの設定領域を確保するには true を指定します。
    //! @param[in] projectionTexGenCount 格納可能な投影テクスチャ座標生成の数です。
    //! @param[in] fontShadowParameter フォント影の補間色の設定領域を確保するには true を指定します。
    //! @param[in] detailedCombinerParameter 詳細なコンバイナの設定領域を確保するには true を指定します。
    //! @param[in] combinerUserShaderParameter コンバイナユーザーシェーダーの設定領域を確保するには true を指定します。
    //! @param[in] refVectorGraphicsTextureInfoCount 参照しているベクターグラフィックステクスチャの数です。
    //!
    //! @return 領域が確保できなかった場合には false が返ります。
    //!
    bool ReserveMem(
        int  texMapCount,
        int  texSrtCount,
        int  texCoordGenCount,
        int  tevStageCount,
        bool allocAlpComp,
        int  blendModeCount,
        bool allocIndirectParameter,
        int  projectionTexGenCount,
        bool fontShadowParameter,
        bool detailedCombinerParameter,
        bool combinerUserShaderParameter,
        int  refVectorGraphicsTextureInfoCount);

    //! @brief マテリアルの設定を保持する領域を確保します。
    //!
    //! @param[in] texMapCount 格納可能なテクスチャの数です。
    //! @param[in] texSrtCount 格納可能なテクスチャ SRT 変換行列の数です。
    //! @param[in] texCoordGenCount 格納可能なテクスチャ座標生成の数です。
    //! @param[in] tevStageCount 格納可能な TEV ステージの数です。
    //! @param[in] allocAlpComp アルファコンペアの設定領域を確保するには true を指定します。
    //! @param[in] blendModeCount ブレンドモードの数です。
    //! @param[in] allocIndirectParameter インダイレクトのパラメータの設定領域を確保するには true を指定します。
    //! @param[in] projectionTexGenCount 格納可能な投影テクスチャ座標生成の数です。
    //! @param[in] fontShadowParameter フォント影の補間色の設定領域を確保するには true を指定します。
    //! @param[in] detailedCombinerParameter 詳細なコンバイナの設定領域を確保するには true を指定します。
    //! @param[in] combinerUserShaderParameter コンバイナユーザーシェーダーの設定領域を確保するには true を指定します。
    //!
    //! @return 領域が確保できなかった場合には false が返ります。
    //!
    bool ReserveMem(
        int  texMapCount,
        int  texSrtCount,
        int  texCoordGenCount,
        int  tevStageCount,
        bool allocAlpComp,
        int  blendModeCount,
        bool allocIndirectParameter,
        int  projectionTexGenCount,
        bool fontShadowParameter,
        bool detailedCombinerParameter,
        bool combinerUserShaderParameter)
    {
        const int refVectorGraphicsTextureInfoCount = 0;
        return ReserveMem(
            texMapCount,
            texSrtCount,
            texCoordGenCount,
            tevStageCount,
            allocAlpComp,
            blendModeCount,
            allocIndirectParameter,
            projectionTexGenCount,
            fontShadowParameter,
            detailedCombinerParameter,
            combinerUserShaderParameter,
            refVectorGraphicsTextureInfoCount
        );
    }

    //! @brief マテリアルの設定を保持する領域を確保します。
    //!
    //! @param[in] texMapCount 格納可能なテクスチャの数です。
    //! @param[in] texSrtCount 格納可能なテクスチャ SRT 変換行列の数です。
    //! @param[in] texCoordGenCount 格納可能なテクスチャ座標生成の数です。
    //! @param[in] tevStageCount 格納可能な TEV ステージの数です。
    //! @param[in] allocAlpComp アルファコンペアの設定領域を確保するには true を指定します。
    //! @param[in] blendModeCount ブレンドモードの数です。
    //! @param[in] allocIndirectParameter インダイレクトのパラメータの設定領域を確保するには true を指定します。
    //! @param[in] projectionTexGenCount 格納可能な投影テクスチャ座標生成の数です。
    //! @param[in] fontShadowParameter フォント影の補間色の設定領域を確保するには true を指定します。
    //! @param[in] detailedCombinerParameter 詳細なコンバイナの設定領域を確保するには true を指定します。
    //!
    //! @return 領域が確保できなかった場合には false が返ります。
    //!
    bool ReserveMem(
        int  texMapCount,
        int  texSrtCount,
        int  texCoordGenCount,
        int  tevStageCount,
        bool allocAlpComp,
        int  blendModeCount,
        bool allocIndirectParameter,
        int  projectionTexGenCount,
        bool fontShadowParameter,
        bool detailedCombinerParameter)
    {
        const bool combinerUserShaderParameter = false;
        return ReserveMem(
            texMapCount,
            texSrtCount,
            texCoordGenCount,
            tevStageCount,
            allocAlpComp,
            blendModeCount,
            allocIndirectParameter,
            projectionTexGenCount,
            fontShadowParameter,
            detailedCombinerParameter,
            combinerUserShaderParameter
        );
    }

    //! @brief マテリアルの設定を保持する領域を確保します。
    //!
    //! @param[in] texMapCount 格納可能なテクスチャの数です。
    //! @param[in] texSrtCount 格納可能なテクスチャ SRT 変換行列の数です。
    //! @param[in] texCoordGenCount 格納可能なテクスチャ座標生成の数です。
    //! @param[in] tevStageCount 格納可能な TEV ステージの数です。
    //! @param[in] allocAlpComp アルファコンペアの設定領域を確保するには true を指定します。
    //! @param[in] blendModeCount ブレンドモードの数です。
    //! @param[in] allocIndirectParameter インダイレクトのパラメータの設定領域を確保するには true を指定します。
    //! @param[in] projectionTexGenCount 格納可能な投影テクスチャ座標生成の数です。
    //! @param[in] fontShadowParameter フォント影の補間色の設定領域を確保するには true を指定します。
    //!
    //! @return 領域が確保できなかった場合には false が返ります。
    //!
    bool ReserveMem(
        int  texMapCount,
        int  texSrtCount,
        int  texCoordGenCount,
        int  tevStageCount,
        bool allocAlpComp,
        int  blendModeCount,
        bool allocIndirectParameter,
        int  projectionTexGenCount,
        bool fontShadowParameter)
    {
        const bool detailedCombinerParameter = false;
        return ReserveMem(
            texMapCount,
            texSrtCount,
            texCoordGenCount,
            tevStageCount,
            allocAlpComp,
            blendModeCount,
            allocIndirectParameter,
            projectionTexGenCount,
            fontShadowParameter,
            detailedCombinerParameter
        );
    }

    //! @brief マテリアルの設定を保持する領域を確保します。
    //!
    //! @param[in] texMapCount 格納可能なテクスチャの数です。
    //! @param[in] texSrtCount 格納可能なテクスチャ SRT 変換行列の数です。
    //! @param[in] texCoordGenCount 格納可能なテクスチャ座標生成の数です。
    //! @param[in] tevStageCount 格納可能な TEV ステージの数です。
    //! @param[in] allocAlpComp アルファコンペアの設定領域を確保するには true を指定します。
    //! @param[in] blendModeCount ブレンドモードの数です。
    //! @param[in] allocIndirectParameter インダイレクトのパラメータの設定領域を確保するには true を指定します。
    //! @param[in] projectionTexGenCount 格納可能な投影テクスチャ座標生成の数です。
    //!
    //! @return 領域が確保できなかった場合には false が返ります。
    //!
    bool ReserveMem(
        int  texMapCount,
        int  texSrtCount,
        int  texCoordGenCount,
        int  tevStageCount,
        bool allocAlpComp,
        int  blendModeCount,
        bool allocIndirectParameter,
        int  projectionTexGenCount)
    {
        const bool fontShadowParameter = false;
        return ReserveMem(
            texMapCount,
            texSrtCount,
            texCoordGenCount,
            tevStageCount,
            allocAlpComp,
            blendModeCount,
            allocIndirectParameter,
            projectionTexGenCount,
            fontShadowParameter
        );
    }

    //! @brief マテリアルの設定を保持する領域を確保します。
    //!
    //! @param[in] texMapCount 格納可能なテクスチャの数です。
    //! @param[in] texSrtCount 格納可能なテクスチャ SRT 変換行列の数です。
    //! @param[in] texCoordGenCount 格納可能なテクスチャ座標生成の数です。
    //! @param[in] tevStageCount 格納可能な TEV ステージの数です。
    //! @param[in] allocAlpComp アルファコンペアの設定領域を確保するには true を指定します。
    //! @param[in] blendModeCount ブレンドモードの数です。
    //! @param[in] allocIndirectParameter インダイレクトのパラメータの設定領域を確保するには true を指定します。
    //!
    //! @return 領域が確保できなかった場合には false が返ります。
    //!
    bool ReserveMem(
        int  texMapCount,
        int  texSrtCount,
        int  texCoordGenCount,
        int  tevStageCount,
        bool allocAlpComp,
        int  blendModeCount,
        bool allocIndirectParameter)
    {
        const int  projectionTexGenCount = 0;
        return ReserveMem(
            texMapCount,
            texSrtCount,
            texCoordGenCount,
            tevStageCount,
            allocAlpComp,
            blendModeCount,
            allocIndirectParameter,
            projectionTexGenCount
        );
    }

    //! @brief マテリアルの設定を保持する領域を確保します。
    //!
    //! @param[in] texMapCount 格納可能なテクスチャの数です。
    //! @param[in] texSrtCount 格納可能なテクスチャ SRT 変換行列の数です。
    //! @param[in] texCoordGenCount 格納可能なテクスチャ座標生成の数です。
    //! @param[in] tevStageCount 格納可能な TEV ステージの数です。
    //! @param[in] allocAlpComp アルファコンペアの設定領域を確保するには true を指定します。
    //! @param[in] blendModeCount ブレンドモードの数です。
    //!
    //! @return 領域が確保できなかった場合には false が返ります。
    //!
    bool ReserveMem(
        int  texMapCount,
        int  texSrtCount,
        int  texCoordGenCount,
        int  tevStageCount,
        bool allocAlpComp,
        int  blendModeCount)
    {
        const bool allocIndirectParameter = false;
        return ReserveMem(
            texMapCount,
            texSrtCount,
            texCoordGenCount,
            tevStageCount,
            allocAlpComp,
            blendModeCount,
            allocIndirectParameter
        );
    }

    //! @brief マテリアルの設定を保持する領域を確保します。
    //!
    //! @param[in] texMapCount 格納可能なテクスチャの数です。
    //! @param[in] texSrtCount 格納可能なテクスチャ SRT 変換行列の数です。
    //! @param[in] texCoordGenCount 格納可能なテクスチャ座標生成の数です。
    //! @param[in] tevStageCount 格納可能な TEV ステージの数です。
    //! @param[in] allocAlpComp アルファコンペアの設定領域を確保するには true を指定します。
    //!
    //! @return 領域が確保できなかった場合には false が返ります。
    //!
    bool ReserveMem(
        int  texMapCount,
        int  texSrtCount,
        int  texCoordGenCount,
        int  tevStageCount,
        bool allocAlpComp)
    {
        const int  blendModeCount = 0;
        return ReserveMem(
            texMapCount,
            texSrtCount,
            texCoordGenCount,
            tevStageCount,
            allocAlpComp,
            blendModeCount
        );
    }

    //! @brief マテリアルの設定を保持する領域を確保します。
    //!
    //! @param[in] texMapCount 格納可能なテクスチャの数です。
    //! @param[in] texSrtCount 格納可能なテクスチャ SRT 変換行列の数です。
    //! @param[in] texCoordGenCount 格納可能なテクスチャ座標生成の数です。
    //! @param[in] tevStageCount 格納可能な TEV ステージの数です。
    //!
    //! @return 領域が確保できなかった場合には false が返ります。
    //!
    bool ReserveMem(
        int  texMapCount,
        int  texSrtCount,
        int  texCoordGenCount,
        int  tevStageCount)
    {
        const bool allocAlpComp = false;
        return ReserveMem(
            texMapCount,
            texSrtCount,
            texCoordGenCount,
            tevStageCount,
            allocAlpComp
        );
    }

    //! @brief マテリアルの設定を保持する領域を確保します。
    //!
    //! @param[in] texMapCount 格納可能なテクスチャの数です。
    //! @param[in] texSrtCount 格納可能なテクスチャ SRT 変換行列の数です。
    //! @param[in] texCoordGenCount 格納可能なテクスチャ座標生成の数です。
    //!
    //! @return 領域が確保できなかった場合には false が返ります。
    //!
    bool ReserveMem(
        int  texMapCount,
        int  texSrtCount,
        int  texCoordGenCount)
    {
        const int  tevStageCount = 0;
        return ReserveMem(
            texMapCount,
            texSrtCount,
            texCoordGenCount,
            tevStageCount
        );
    }

    //! @brief マテリアルに格納することのできるテクスチャの上限数を取得します。
    //!
    //! @return TexMap を格納できる上限数を返します。
    //!
    //! @sa ReserveMem
    //!
    uint8_t  GetTexMapCap() const
    {
        return uint8_t (m_MemCap.texMap);
    }

    //! @brief マテリアルに格納することのできるテクスチャ SRT 変換行列の上限数を取得します。
    //!
    //! @return テクスチャ SRT 変換行列を格納できる上限数を返します。
    //!
    //! @sa ReserveMem
    //!
    uint8_t  GetTexSrtCap() const
    {
        return uint8_t (m_MemCap.texSrt);
    }

    //! @brief マテリアルに格納することのできるテクスチャ座標生成の上限数を取得します。
    //!
    //! @return テクスチャ座標生成を格納できる上限数を返します。
    //!
    //! @sa ReserveMem
    //!
    uint8_t  GetTexCoordGenCap() const
    {
        return uint8_t (m_MemCap.texCoordGen);
    }

    //! @brief マテリアルに格納することのできる TEV ステージの上限数を取得します。
    //!
    //! @return TEV ステージを格納できる上限数を返します。
    //!
    //! @sa ReserveMem
    //!
    uint8_t  GetTevStageCap() const
    {
        return uint8_t (m_MemCap.tevStage);
    }

    //! @brief マテリアルがアルファコンペアの設定を格納することができるかどうかを取得します。
    //!
    //! @return アルファコンペアの設定を格納できる場合は true を返します。
    //!
    //! @sa ReserveMem
    //!
    bool IsAlphaCompareCap() const
    {
        return m_MemCap.alpComp != 0;
    }

    //! @brief マテリアルがブレンドモード設定を格納することができるかどうかを取得します。
    //!
    //! @details 「デフォルト設定を利用する」を無効にし、かつプリセットの設定に無い設定を行っている場合に、true を返します。
    //!
    //! @return ブレンドモード設定を格納できる場合は true を返します。
    //!
    //! @sa ReserveMem
    //!
    bool IsBlendModeCap() const
    {
        return m_MemCap.blendMode != 0;
    }

    //! @brief マテリアルがアルファのブレンドモード設定を格納することができるかどうかを取得します。
    //!
    //! @details 「デフォルト設定を利用する」を無効にし、かつプリセットの設定に無い設定を行っている場合に、true を返します。
    //!
    //! @return アルファのブレンドモード設定を格納できる場合は true を返します。
    //!
    //! @sa ReserveMem
    //!
    bool IsBlendModeAlphaCap() const
    {
        return m_MemCap.blendMode == 2;
    }

    //! @brief マテリアルがインダイレクトのパラメータを格納することができるかどうかを取得します。
    //!
    //! @return インダイレクトのパラメータを格納できる場合は true を返します。
    //!
    //! @sa ReserveMem
    //!
    bool IsIndirectParameterCap() const
    {
        return m_MemCap.indirectParameter == 1;
    }

    //! @brief マテリアルがフォント影のパラメータを格納できるかどうかを取得します。
    //!
    //! @return フォント影の補間カラーを格納できる場合は true を返します。
    //!
    bool IsFontShadowParameterCap() const
    {
        return m_MemCap.fontShadowParameter == 1;
    }

    //! @brief 詳細なコンバイナステージ設定を行うかを取得します。
    //!
    //! @return 詳細なコンバイナステージ設定を行う場合は true を返します。
    //!
    //! @sa ReserveMem
    //!
    bool UseDetailedCombinerCap() const
    {
        return m_MemCap.detailedCombinerParameter == 1;
    }

    //! @brief コンバイナユーザーシェーダ設定を行うかを取得します。
    //!
    //! @return コンバイナユーザーシェーダ設定を行う場合は true を返します。
    //!
    //! @sa ReserveMem
    //!
    bool UseCombinerUserShaderCap() const
    {
        return m_MemCap.combinerUserShaderParameter == 1;
    }

    //! @brief マテリアルに格納することのできるベクターグラフィックステクスチャ参照情報の上限数を取得します。
    //!
    //! @return ベクターグラフィックステクスチャ参照情報を格納できる上限数を返します。
    //!
    //! @sa ReserveMem
    //!
    uint8_t  GetRefVectorGraphicsTextureInfoCap() const
    {
        return uint8_t(m_MemCap.refVectorGraphicsTexture);
    }

    //! @brief 白カラー情報が float データかどうかのフラグを取得します。
    //!
    //! @return 白カラー情報が flota の場合は true を返します。
    //!
    bool IsWhiteColorFloat() const
    {
        return detail::TestBit(m_Flag, Flags_WhiteColorFloat);
    }

    //! @brief 白黒カラー情報が float データかどうかのフラグを取得します。
    //!
    //! @return 白黒カラー情報が flota の場合は true を返します。
    //!
    bool IsInterpolateColorFloat(InterpolateColor type) const
    {
        return detail::TestBit(m_Flag, Flags_BlackColorFloat + type);
    }

    //! @brief マテリアルの名前を取得します。
    //!
    //! @return マテリアルの名前を返します。
    //!
    //! @sa SetName
    //!
    const char* GetName() const
    {
        return m_pName;
    }

    //! @brief マテリアルの名前を設定します。
    //!
    //! @details
    //! セットできるマテリアル名文字列の最大長は nn::ui2d::MaterialNameStrMax です。
    //!
    //! Material クラスは内部に名前のための文字列バッファを持たず、
    //! このメソッドで与えたポインタを保持しますので、スタックなどで一時的に確保した
    //! ポインタを与えないようにしてください。
    //!
    //! @param[in] pName 設定する名前です。
    //!
    //! @sa GetName
    //!
    void SetName(const char* pName);

    //! @brief 使用するマテリアル情報をテクスチャのみにするかを取得します。
    //!
    //! @return 使用するマテリアル情報がテクスチャのみになっている場合は true を返します。
    //!
    //! @sa SetTextureOnly
    //!
    bool GetTextureOnly() const
    {
        return detail::TestBit(m_Flag, Flags_TextureOnly);
    }

    //! @brief 使用するマテリアル情報をテクスチャのみにするかを設定します。
    //!
    //! @details
    //! テクスチャ以外の設定は以前のマテリアルの設定をそのまま使います。
    //!
    //! @param[in] value    使用するマテリアル情報をテクスチャのみにする場合は true を指定します。
    //!
    //! @sa GetTextureOnly
    //!
    void SetTextureOnly(bool value)
    {
        m_Flag = detail::SetBit(m_Flag, Flags_TextureOnly, value);
    }

    //! @brief 閾値によるアルファ補間が有効であるかを取得します。
    //!
    //! @return 閾値によるアルファ補間が有効であれば true を返します。
    //!
    //! @sa SetThresholdingAlphaInterpolation
    //!
    bool GetThresholdingAlphaInterpolation() const
    {
        return detail::TestBit(m_Flag, Flags_ThresholdingAlphaInterpolation);
    }

    //! @brief 閾値によるアルファ補間が有効であるかを設定します。
    //!
    //! @param[in] value 閾値によるアルファ補間を有効にする場合は true を返します。
    //!
    //! @sa GetThresholdingAlphaInterpolation
    //!
    void SetThresholdingAlphaInterpolation(bool value)
    {
        m_Flag = detail::SetBit(m_Flag, Flags_ThresholdingAlphaInterpolation, value);
    }

    //! @brief シェーダIDを取得します。
    //!
    //! @return シェーダID
    //!
    ShaderId GetShaderId() const
    {
        return static_cast<ShaderId>(m_PackedValues.shaderId);
    }

    //! @brief シェーダIDを設定します。
    //!
    //! @details
    //! シェーダIDは、リソースからMaterialクラスを作ったときやコピーしたときは、
    //! 自動的に適切な値に設定されますので、変更しないようにしてください。
    //!
    //! デフォルト設定の状態でMaterialクラスを作ったときのみこのメソッドを
    //! 使用してください。
    //!
    //! @param[in] id   シェーダID
    //!
    void SetShaderId(ShaderId id)
    {
        m_PackedValues.shaderId = static_cast<uint8_t >(id);
    }

    //! @brief マテリアルにユーザーシェーダーがアサインされているかどうかを取得します。
    //!
    //! @return ユーザーシェーダーが使用されていれば true を返します。
    //!
    bool IsUserShaderAssigned() const
    {
        return m_pUserShaderConstantBufferInformation != NULL;
    }

    //@}

    //----------------------------------------
    //! @name テクスチャ
    //@{

    //! @brief テクスチャの数を取得します。
    //!
    //! @return テクスチャの数を返します。
    //!
    //! @sa SetTexMapCount
    //!
    uint8_t  GetTexMapCount() const
    {
        return uint8_t (m_MemCount.texMap);
    }

    //! @brief テクスチャの数を設定します。
    //!
    //! @param[in] count  テクスチャの数です。
    //!
    //! @sa GetTexMapCount
    //!
    void SetTexMapCount(uint8_t  count);

    //! @brief テクスチャ座標生成の数を取得します。
    //!
    //! @return テクスチャ座標生成の数を返します。
    //!
    //! @sa SetTexCoordGenCount
    //!
    uint8_t  GetTexCoordGenCount() const
    {
        return uint8_t (m_MemCount.texCoordGen);
    }

    //! @brief テクスチャ座標生成の数を設定します。
    //!
    //! @param[in] count  テクスチャ座標生成の数です。
    //!
    //! @sa GetTexCoordGenCount
    //!
    void SetTexCoordGenCount(uint8_t  count);

    //! @brief テクスチャSRTの数を取得します。
    //!
    //! @return テクスチャSRTの数を返します。
    //!
    //! @sa SetSrtCount
    //!
    uint8_t  GetTexSrtCount() const
    {
        return uint8_t (m_MemCount.texSrt);
    }

    //! @brief テクスチャSRTの数を設定します。
    //!
    //! @param[in] count  テクスチャSRTの数です。
    //!
    //! @sa GetTexSrtCount
    //!
    void SetTexSrtCount(uint8_t  count);

    //! @brief テクスチャの座標変換行列を求めます。
    //!
    //! @param[out] pTexMtx 計算結果の格納先です。
    //! @param[in] texSrt   テクスチャのSRT情報です。
    //! @param[in] texMap   テクスチャ情報です。
    //!
    static void CalculateTextureMtx(float pTexMtx[2][3], const ResTexSrt& texSrt, const TexMap& texMap);

    //! @brief インダイレクトテクスチャの座標変換行列を求めます。
    //!
    //! @param[out] pIndirectMtx 計算結果の格納先です。
    //! @param[in]  rotate テクスチャの回転情報です。
    //! @param[in]  scale テクスチャのスケール情報です。
    //!
    static void CalculateIndirectMtx(float pIndirectMtx[2][3], const float rotate, const ResVec2& scale);

    //! @brief テクスチャデータを取得します。
    //!
    //! @details
    //! idx に ReserveMem() または SetTexMapCount() で設定したテクスチャの数よりも
    //! 大きな値を渡した場合はアサートが発生します。
    //!
    //! @param[in] idx  インデックスです。
    //!
    //! @return オブジェクトへの参照を返します。
    //!
    //! @sa SetTexMap
    //! @sa ReserveMem
    //! @sa SetTexMapCount
    //!
    const TexMap& GetTexMap(int  idx) const
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.texMap), "out of bounds: idx[%u] < m_MemCount.texMap[%u] for material[%s]", idx, m_MemCount.texMap, GetName());
        return GetTexMapArray()[idx];
    }

    //! @brief テクスチャデータを追加します。
    //!
    //! @details
    //! テクスチャデータの数がReserveMem()で設定したテクスチャ数よりも多くなった
    //! 場合はアサートが発生します。
    //!
    //! @param[in] textureInfo  TextureInfo オブジェクトへの参照です。
    //!
    //! @sa GetTexMap
    //! @sa ReserveMem
    //! @sa SetTexMapCount
    //!
    void AppendTexMap(const TextureInfo& textureInfo)
    {
        NN_SDK_ASSERT(static_cast<uint8_t>(m_MemCount.texMap) < GetTexMapCap(), "out of bounds: m_MemCount.texMap[%u] < GetTexMapCap()[%u] for material[%s]", m_MemCount.texMap, GetTexMapCap(), GetName());
        new (&GetTexMapArray()[m_MemCount.texMap]) TexMap(&textureInfo);
        m_MemCount.texMap += 1;
    }

    //! @brief テクスチャデータを設定します。
    //!
    //! @details
    //! idx に ReserveMem() または SetTexMapCount() で設定したテクスチャ数よりも
    //! 大きな値を渡した場合はアサートが発生します。
    //!
    //! @param[in] idx      インデックスです。
    //! @param[in] pTexInfo TextureInfo オブジェクトへの参照です。
    //!
    //! @sa GetTexMap
    //! @sa ReserveMem
    //! @sa SetTexMapCount
    //!
    void SetTexMap(int  idx, const TextureInfo* pTexInfo)
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.texMap), "out of bounds: idx[%u] < m_MemCount.texMap[%u] for material[%s]", idx, m_MemCount.texMap, GetName());
        GetTexMapArray()[idx].SetTextureInfo(pTexInfo);
    }

    //! @brief テクスチャのラップモードを設定します。
    //!
    //! @details
    //! idx に ReserveMem() または SetTexMapCount() で設定したテクスチャ数よりも
    //! 大きな値を渡した場合はアサートが発生します。
    //!
    //! @param[in] idx      インデックスです。
    //! @param[in] wrapS    テクスチャの S 方向のラップモードです。
    //! @param[in] wrapT    テクスチャの T 方向のラップモードです。
    //!
    //! @sa SetTexMap
    //! @sa ReserveMem
    //! @sa SetTexMapCount
    //!
    void SetTexMapWrapMode(int  idx, TexWrap wrapS, TexWrap wrapT)
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.texMap), "out of bounds: idx[%u] < m_MemCount.texMap[%u] for material[%s]", idx, m_MemCount.texMap, GetName());
        GetTexMapArray()[idx].SetWrapMode(wrapS, wrapT);
    }

    //! @brief テクスチャのフィルタモードを設定します。
    //!
    //! @details
    //! idx に ReserveMem() または SetTexMapCount() で設定したテクスチャ数よりも
    //! 大きな値を渡した場合はアサートが発生します。
    //!
    //! @param[in] idx      インデックスです。
    //! @param[in] minFlt   テクスチャが縮小されるときに適用されるフィルタモードです。
    //! @param[in] magFlt   テクスチャが拡大されるときに適用されるフィルタモードです。
    //!
    //! @sa SetTexMap
    //! @sa ReserveMem
    //! @sa SetTexMapCount
    //!
    void SetTexMapFilter(int  idx, TexFilter minFlt, TexFilter magFlt)
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.texMap), "out of bounds: idx[%u] < m_MemCount.texMap[%u] for material[%s]", idx, m_MemCount.texMap, GetName());
        GetTexMapArray()[idx].SetFilter(minFlt, magFlt);
    }

    //! @brief テクスチャ SRT 行列を取得します。
    //!
    //! @details
    //! idx に ReserveMem() または SetTexSrtCount() で設定したテクスチャ SRT 行列の数よりも
    //! 大きな値を渡した場合はアサートが発生します。
    //!
    //! @param[in] idx  インデックスです。
    //!
    //! @return オブジェクトへの参照を返します。
    //!
    //! @sa SetTexSrt
    //! @sa ReserveMem
    //! @sa SetTexSrtCount
    //!
    const ResTexSrt& GetTexSrt(int  idx) const
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.texSrt), "out of bounds: idx[%u] < m_MemCount.texSrt[%u] for material[%s]", idx, m_MemCount.texSrt, GetName());
        return GetTexSrtArray()[idx];
    }

    //! @brief テクスチャ SRT 行列を設定します。
    //!
    //! @details
    //! idx に ReserveMem() または SetTexSrtCount() で設定したテクスチャ SRT 行列の数よりも
    //! 大きな値を渡した場合はアサートが発生します。
    //!
    //! @param[in] idx      インデックスです。
    //! @param[in] value    設定するテクスチャ SRT 行列です。
    //!
    //! @sa GetTexSrt
    //! @sa ReserveMem
    //! @sa SetTexSrtCount
    //!
    void SetTexSrt(int  idx, const ResTexSrt& value)
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.texSrt), "out of bounds: idx[%u] < m_MemCount.texSrt[%u] for material[%s]", idx, m_MemCount.texSrt, GetName());
        GetTexSrtArray()[idx] = value;
    }

    //! @brief テクスチャ座標生成を取得します。
    //!
    //! @details
    //! idx に ReserveMem() または SetTexCoordGenCount() で設定した
    //! テクスチャ座標生成の数よりも大きな値を渡した場合は
    //! アサートが発生します。
    //!
    //! @param[in] idx  インデックスです。
    //!
    //! @return オブジェクトへの参照を返します。
    //!
    //! @sa SetTexCoordGen
    //! @sa ReserveMem
    //! @sa GetTexCoordGenCount
    //! @sa SetTexCoordGenCount
    //!
    const ResTexCoordGen& GetTexCoordGen(int  idx) const
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.texCoordGen), "out of bounds: idx[%u] < m_MemCount.texCoordGen[%u] for material[%s]", idx, m_MemCount.texCoordGen, GetName());
        return GetTexCoordGenArray()[idx];
    }

    //! @brief テクスチャ座標生成を設定します。
    //!
    //! @details
    //! idx に ReserveMem() または SetTexCoordGenCount() で設定したテクスチャ座標生成の
    //! 格納可能数よりも大きな値を渡した場合はアサートが発生します。
    //!
    //! @param[in] idx      インデックスです。
    //! @param[in] value    設定するテクスチャ座標生成です。
    //!
    //! @sa GetTexCoordGen
    //! @sa ReserveMem
    //!
    void SetTexCoordGen(int  idx, const ResTexCoordGen& value)
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.texCoordGen), "out of bounds: idx[%u] < m_MemCount.texCoordGen[%u] for material[%s]", idx, m_MemCount.texCoordGen, GetName());
        GetTexCoordGenArray()[idx] = value;
    }

    //! @brief 投影テクスチャ生成の数を取得します。
    //!
    //! @return 投影テクスチャ生成の数を返します。
    //!
    //! @sa SetProjectionTexGenCount
    //!
    uint8_t  GetProjectionTexGenCount() const
    {
        return uint8_t (m_MemCount.projectionTexGen);
    }

    //! @brief 投影テクスチャ生成の数を設定します。
    //!
    //! @param[in] count  投影テクスチャ生成の数です。
    //!
    //! @sa GetProjectionTexGenCount
    //!
    void SetProjectionTexGenCount(uint8_t  count);

    //! @brief 成分を指定してテクスチャ SRT のパラメータを取得します。
    //!
    //! @details
    //! テクスチャSRTの表示位置、回転、拡大率の成分を指定して取得します。
    //!
    //! eleIdx の指定には nn::ui2d::AnimTargetTexSrt の値を使用してください。
    //!
    //! @param[in] idx      TexSrt のインデックスです。
    //! @param[in] elementIdx   成分のインデックスです。
    //!
    //! @return テクスチャSRTの指定された成分を返します。
    //!
    //! @sa AnimTargetTexSrt
    //! @sa SetTexSrtElement
    //!
    float GetTexSrtElement(int  idx, int  elementIdx) const
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.texSrt), "out of bounds: idx[%u] < m_MemCount.texSrt[%u] for material[%s]", idx, m_MemCount.texSrt, GetName());

        const float *const srtAry = &GetTexSrtArray()[idx].translate.x;
        return srtAry[elementIdx];
    }

    //! @brief 成分を指定してテクスチャ SRT のパラメータを設定します。
    //!
    //! @details
    //! テクスチャの表示位置、回転、拡大率の成分を指定して設定します。
    //!
    //! eleIdx の指定には nn::ui2d::AnimTargetTexSrt を使用してください。
    //!
    //! idx に ReserveMem() または SetTexSrtCount() で設定したテクスチャ SRT 行列の
    //! 格納可能数よりも大きな値を渡した場合はアサートが発生します。
    //!
    //! @param[in] idx      ResTexSrt のインデックスです。
    //! @param[in] elementIdx   成分のインデックスです。
    //! @param[in] value    成分の値です。
    //!
    //! @sa AnimTargetTexSrt
    //! @sa GetTexSrtElement
    //! @sa ReserveMem
    //! @sa SetTexSrtCount
    //!
    void SetTexSrtElement(int  idx, int  elementIdx, float value)
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.texSrt), "out of bounds: idx[%u] < m_MemCount.texSrt[%u] for material[%s]", idx, m_MemCount.texSrt, GetName());

        float *const srtAry = &GetTexSrtArray()[idx].translate.x;
        srtAry[elementIdx] = value;
    }

    //@}

    //----------------------------------------
    //! @name TEV
    //@{

    //! @brief TEV ステージ数を取得します。
    //!
    //! @return TEV ステージ数を返します。
    //!
    //! @sa SetTevStageCount
    //!
    uint8_t  GetTevStageCount() const
    {
        return uint8_t (m_MemCount.tevStage);
    }

    //! @brief TEV ステージ数を設定します。
    //!
    //! @param[in] count  TEV ステージの数です。
    //!
    //! @sa GetTevStageCount
    //!
    void SetTevStageCount(uint8_t  count);

    //! @brief カラーを取得します。
    //!
    //! @details
    //! idx には nn::ui2d::InterpolateColor の値を指定します。
    //!
    //! @param[in] idx  インデックスです。
    //!
    //! @return オブジェクトへの参照を返します。
    //!
    //! @sa SetColor
    //! @sa nn::ui2d::InterpolateColor
    //!
    const nn::util::Unorm8x4& GetColor(int  idx) const
    {
        NN_SDK_ASSERT(idx < MatColorMax, "out of bounds: idx[%u] < MatColorMax[%u] for material[%s]", idx, MatColorMax, GetName());
        NN_SDK_ASSERT(!detail::TestBit(m_Flag, Flags_BlackColorFloat + idx));

        return m_Colors.byteColor[idx];
    }

    //! @brief カラーを浮動小数点数で取得します。
    //!
    //! @details
    //! idx には nn::ui2d::InterpolateColor の値を指定します。
    //!
    //! @param[in] idx  インデックスです。
    //!
    //! @return オブジェクトへの参照を返します。
    //!
    //! @sa SetColorAsFloat
    //! @sa nn::ui2d::InterpolateColor
    //!
    const nn::util::Float4& GetColorAsFloat(int  idx) const
    {
        NN_SDK_ASSERT(idx < MatColorMax, "out of bounds: idx[%u] < MatColorMax[%u] for material[%s]", idx, MatColorMax, GetName());
        NN_SDK_ASSERT(detail::TestBit(m_Flag, Flags_BlackColorFloat + idx));

        return m_Colors.pFloatColor[idx];
    }

    //! @brief カラーを浮動小数点数で取得します。
    //!
    //! @details
    //! idx には nn::ui2d::InterpolateColor の値を指定します。内部データが byte の時に float へコンバートした値を返すため、内部カラーデータフォーマットによらず値を取得することができます。
    //!
    //! @param[out] outColor 取得結果のカラーです。
    //! @param[in] idx  インデックスです。
    //!
    //! @sa nn::ui2d::InterpolateColor
    //!
    void GetColorWithFloatConversion(nn::util::Float4& outColor, int idx) const
    {
        if (IsInterpolateColorFloat(static_cast<InterpolateColor>(idx)))
        {
            outColor = this->GetColorAsFloat(idx);
        }
        else
        {
            const float TO_FLOAT = 1.0f / std::numeric_limits<uint8_t>::max();

            outColor.x = static_cast<float>(m_Colors.byteColor[idx].v[0]) * TO_FLOAT;
            outColor.y = static_cast<float>(m_Colors.byteColor[idx].v[1]) * TO_FLOAT;
            outColor.z = static_cast<float>(m_Colors.byteColor[idx].v[2]) * TO_FLOAT;
            outColor.w = static_cast<float>(m_Colors.byteColor[idx].v[3]) * TO_FLOAT;
        }
    }

    //! @brief カラーを設定します。
    //!
    //! @details
    //! idx には nn::ui2d::InterpolateColor の値を指定します。
    //!
    //! @param[in] idx      インデックスです。
    //! @param[in] value    カラーの値です。
    //!
    //! @sa GetColor
    //! @sa nn::ui2d::InterpolateColor
    //!
    void SetColor(int  idx, const nn::util::Unorm8x4& value)
    {
        NN_SDK_ASSERT(idx < MatColorMax, "out of bounds: idx[%u] < MatColorMax[%u] for material[%s]", idx, MatColorMax, GetName());
        NN_SDK_ASSERT(!detail::TestBit(m_Flag, Flags_BlackColorFloat + idx));

        m_Colors.byteColor[idx] = value;
    }

    //! @brief カラーを設定します。
    //!
    //! @details
    //! idx には nn::ui2d::InterpolateColor の値を指定します。
    //!
    //! @param[in] idx      インデックスです。
    //! @param[in] value    カラーの値です。
    //!
    //! @sa GetColorAsFloat
    //! @sa nn::ui2d::InterpolateColor
    //!
    void SetColor(int  idx, const nn::util::Float4& value)
    {
        NN_SDK_ASSERT(idx < MatColorMax, "out of bounds: idx[%u] < MatColorMax[%u] for material[%s]", idx, MatColorMax, GetName());
        NN_SDK_ASSERT(detail::TestBit(m_Flag, Flags_BlackColorFloat + idx));

        m_Colors.pFloatColor[idx] = value;
    }

    //! @brief カラーの種類を指定して TEV のカラーの成分を取得します。
    //!
    //! @details
    //! colorType の指定には nn::ui2d::AnimTargetMatColor を使用してください。
    //!
    //! @param[in] colorType    カラーの種類です。
    //!
    //! @return カラー成分の値を返します。
    //!
    //! @sa SetColorElement
    //! @sa nn::ui2d::AnimTargetMatColor
    //!
    int GetColorElement(int  colorType) const;

    //! @brief カラーの種類(float版)を指定して TEV のカラーの成分を取得します。
    //!
    //! @details
    //! colorType の指定には nn::ui2d::AnimTargetMatColorFloat を使用してください。
    //!
    //! @param[in] colorType    カラーの種類です。
    //!
    //! @return カラー成分の値を返します。
    //!
    //! @sa SetColorElementFloat
    //! @sa nn::ui2d::AnimTargetMatColorFloat
    //!
    float GetColorElementFloat(int  colorType) const;

    //! @brief カラーの種類を指定して TEV のカラーの成分を設定します。
    //!
    //! @details
    //! colorType の指定には nn::ui2d::AnimTargetMatColor を使用してください。
    //!
    //! @param[in] colorType    カラーの種類です。
    //! @param[in] value        カラー値です。
    //!
    //! @sa GetColorElement
    //! @sa nn::ui2d::AnimTargetMatColor
    //!
    void SetColorElement(int  colorType, int  value);

    //! @brief カラーの種類を指定して TEV のカラーの成分を設定します。
    //!
    //! @details
    //! colorType の指定には nn::ui2d::AnimTargetMatColor を使用してください。
    //!
    //! @param[in] colorType    カラーの種類です。
    //! @param[in] value        カラー値です。
    //!
    //! @sa GetColorElement
    //! @sa nn::ui2d::AnimTargetMatColor
    //!
    void SetColorElementFloat(int  colorType, float  value);

    //! @brief カラーの種類を指定して TEV のカラーの成分を設定します。
    //!
    //! @details
    //! colorType の指定には nn::ui2d::AnimTargetMatColor を使用してください。
    //! 内部で、colorType から判定を行い、nn::ui2d::SetColorElement と nn::ui2d::SetColorElementFloat を呼び分けます。
    //!
    //! @param[in] colorType    カラーの種類です。
    //! @param[in] value        カラー値です。
    //!
    //! @sa GetColorElement
    //! @sa nn::ui2d::AnimTargetMatColor
    //!
    void SetColorElementAuto(int  colorType, float  value);

    //! @brief 成分を指定してインダイレクト SRT のパラメータを設定します。
    //!
    //! @details
    //! インダイレクトの回転、拡大率の成分を指定して設定します。(現状、Transには未対応です。)
    //!
    //! eleIdx の指定には nn::ui2d::AnimTargetTexSrt を使用してください。
    //!
    //! @param[in] eleIdx   成分のインデックスです。
    //! @param[in] value    成分の値です。
    //!
    //! @sa AnimTargetTexSrt
    //! @sa ReserveMem
    //!
    void SetIndirectSrtElement(int  eleIdx, float value)
    {
        float *const srtAry = &GetIndirectParameterPtr()->rotate;
        *(srtAry + eleIdx) = value;
    }

    //! @brief TEV ステージを取得します。
    //!
    //! @details
    //! idx に ReserveMem() 関数または SetTevStageCount() 関数で設定した
    //! TEVステージ数よりも大きな値を渡した場合にはアサートが発生します。
    //!
    //! @param[in] idx  インデックスです。
    //!
    //! @return ResTevStage オブジェクトへの参照を返します。
    //!
    //! @sa SetTevStage
    //! @sa ReserveMem
    //! @sa GetTevStageCount
    //! @sa SetTevStageCount
    //!
    const ResTevStage& GetTevStage(int  idx) const
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.tevStage), "out of bounds: idx[%u] < m_MemCount.tevStage[%u] for material[%s]", idx, m_MemCount.tevStage, GetName());
        return GetTevStageArray()[idx];
    }

    //! @brief TEV ステージを設定します。
    //!
    //! @param[in] idx      インデックスです。
    //! @param[in] value    TEV ステージ設定です。
    //!
    //! @details
    //! idx に ReserveMem または SetTevStageCount で設定したTEVステージ数よりも
    //! 大きな値を渡した場合はアサートが発生します。
    //!
    //! @sa ReserveMem
    //! @sa GetTevStage
    //!
    void SetTevStage(int  idx, const ResTevStage& value)
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.tevStage), "out of bounds: idx[%u] < m_MemCount.tevStage[%u] for material[%s]", idx, m_MemCount.tevStage, GetName());
        GetTevStageArray()[idx] = value;
    }

    //! @brief 詳細なコンバイナステージを取得します。
    //!
    //! @details
    //! idx に SetTevStageCount() で設定した
    //! 詳細なコンバイナステージ数以上の値を渡した場合にはアサートが発生します。
    //!
    //! 事前にリソースからのロード、または ReserveMem() の呼び出しで
    //! 詳細なコンバイナステージ設定が有効に設定されている必要があります。
    //! そうでない場合はアサートが発生します。
    //!
    //! @param[in] idx  インデックスです。
    //!
    //! @return ResDetailedCombinerStage オブジェクトへの参照を返します。
    //!
    //! @sa SetDetailedCombinerStage
    //!
    const ResDetailedCombinerStage& GetDetailedCombinerStage(int idx) const
    {
        NN_SDK_ASSERT(UseDetailedCombinerCap(), "UseDetailedCombinerCap() must be true for material[%s]", GetName());
        NN_SDK_ASSERT(idx < GetTevStageCount(), "out of bounds: idx[%u] < GetTevStageCount()[%u] for material[%s]", idx, GetTevStageCount(), GetName());
        return GetDetailedCombinerStageAry()[idx];
    }

    //! @brief 詳細なコンバイナステージを設定します。
    //!
    //! @details
    //! idx に SetTevStageCount() で設定した詳細なコンバイナステージ数以上の
    //! 値を渡した場合はアサートが発生します。
    //!
    //! 事前にリソースからのロード、または ReserveMem() の呼び出しで
    //! 詳細なコンバイナステージ設定が有効に設定されている必要があります。
    //! そうでない場合はアサートが発生します。
    //!
    //! @param[in] idx      インデックスです。
    //! @param[in] value    詳細なコンバイナステージの設定値です。
    //!
    //! @sa GetDetailedCombinerStage
    //!
    void SetDetailedCombinerStage(int idx, const ResDetailedCombinerStage& value)
    {
        NN_SDK_ASSERT(UseDetailedCombinerCap(), "UseDetailedCombinerCap() must be true for material[%s]", GetName());
        NN_SDK_ASSERT(idx < GetTevStageCount(), "out of bounds: idx[%u] < GetTevStageCount()[%u] for material[%s]", idx, GetTevStageCount(), GetName());
        GetDetailedCombinerStageAry()[idx] = value;
    }

    //! @brief コンスタントカラーを取得します。
    //!
    //! @details
    //! コンスタントカラーはコンバイナステージ設定、またはコンバイナユーザーシェーダーで
    //! レジスタに設定される色のソースとして使われます。
    //!
    //! idx には ui2d::DetailedCombinerConstantColor  未満の値を指定します。
    //!
    //! 事前にリソースからのロード、または ReserveMem() の呼び出しで
    //! コンバイナステージ設定が有効に設定されている必要があります。
    //! そうでない場合はアサートが発生します。
    //!
    //! @param[in] idx  インデックスです。
    //!
    //! @return オブジェクトを返します。
    //!
    //! @sa SetConstantColor
    //! @sa GetBufferColorSelectRgb
    //! @sa GetBufferColorSelectAlpha
    //! @sa ui2d::ResDetailedCombinerStage::SetConstColorSelectRgb
    //! @sa ui2d::ResDetailedCombinerStage::SetConstColorSelectAlpha
    //!
    const ResColor GetConstantColor(int idx) const
    {
        NN_SDK_ASSERT(UseDetailedCombinerCap() || UseCombinerUserShaderCap(), "UseDetailedCombinerCap() must be true for material[%s]", GetName());
        NN_SDK_ASSERT(idx < DetailedCombinerConstantColor, "out of bounds: idx[%u] < DetailedCombinerConstantColor[%u] for material[%s]", idx, DetailedCombinerConstantColor, GetName());
        if (UseDetailedCombinerCap() && idx < DetailedCombinerConstantColor)
        {
            const ResDetailedCombinerStageInfo* pCombinerStageInfo = GetDetailedCombinerStageInfoPtr();
            return pCombinerStageInfo->GetConstantColor(idx);
        }
        else if (UseCombinerUserShaderCap() && idx < DetailedCombinerConstantColor)
        {
            const ResCombinerUserShader* pCombinerUserShader = GetCombinerUserShaderPtr();
            return pCombinerUserShader->GetConstantColor(idx);
        }
        else
        {
            const ResColor resColor = { 255, 255, 255, 255 };
            return resColor;
        }
    }

    //! @brief コンスタントカラーを設定します。
    //!
    //! @details
    //! コンスタントカラーはコンバイナステージ設定、又はコンバイナユーザーシェーダーで
    //! レジスタに設定される色のソースとして使われます。
    //!
    //! idx には ui2d::DetailedCombinerConstantColor  未満の値を指定します。
    //!
    //! 事前にリソースからのロード、または ReserveMem() の呼び出しで
    //! コンバイナステージ設定が有効に設定されている必要があります。
    //! そうでない場合はアサートが発生します。
    //!
    //! @param[in] idx      インデックスです。
    //! @param[in] value    カラーの値です。
    //!
    //! @sa GetConstantColor
    //! @sa SetBufferColorSelectRgb
    //! @sa SetBufferColorSelectAlpha
    //! @sa ui2d::ResDetailedCombinerStage::SetConstColorSelectRgb
    //! @sa ui2d::ResDetailedCombinerStage::SetConstColorSelectAlpha
    //!
    void SetConstantColor(int idx, nn::util::Unorm8x4 value)
    {
        NN_SDK_ASSERT(UseDetailedCombinerCap() || UseCombinerUserShaderCap(), "UseDetailedCombinerCap() must be true for material[%s]", GetName());
        NN_SDK_ASSERT(idx < DetailedCombinerConstantColor, "out of bounds: idx[%u] < DetailedCombinerConstantColor[%u] for material[%s]", idx, DetailedCombinerConstantColor, GetName());
        if (UseDetailedCombinerCap() && idx < DetailedCombinerConstantColor)
        {
            ResDetailedCombinerStageInfo* pCombinerStageInfo = GetDetailedCombinerStageInfoPtr();
            ResColor resColor = {value.v[0], value.v[1], value.v[2], value.v[3]};
            pCombinerStageInfo->SetConstantColor(idx, resColor);
        }
        else if (UseCombinerUserShaderCap() && idx < DetailedCombinerConstantColor)
        {
            ResCombinerUserShader* pCombinerUserShader = GetCombinerUserShaderPtr();
            ResColor resColor = {value.v[0], value.v[1], value.v[2], value.v[3]};
            pCombinerUserShader->SetConstantColor(idx, resColor);
        }
    }

    //! @brief bufferColorレジスタのRGBに設定される色のソースを取得します。
    //!
    //! @details
    //! コンバイナステージ設定で利用されます。
    //!
    //! 事前にリソースからのロード、または ReserveMem() の呼び出しで
    //! コンバイナステージ設定が有効に設定されている必要があります。
    //! そうでない場合はアサートが発生します。
    //!
    //! @return bufferColorレジスタのRGBに設定される色のソースを返します。
    //!
    //! @sa SetBufferColorSelectRgb
    //! @sa GetBufferColorSelectAlpha
    //!
    ResDetailedCombinerStage::ConstColorSelect GetBufferColorSelectRgb() const
    {
        NN_SDK_ASSERT(UseDetailedCombinerCap(), "UseDetailedCombinerCap() must be true for material[%s]", GetName());
        if (UseDetailedCombinerCap())
        {
            const ResDetailedCombinerStageInfo* pCombinerStageInfo = GetDetailedCombinerStageInfoPtr();
            return pCombinerStageInfo->GetBufferColorSelectRgb();
        }
        else
        {
            return ResDetailedCombinerStage::ConstColorSelect_Black;
        }
    }

    //! @brief bufferColorレジスタのRGBに設定される色のソースを設定します。
    //!
    //! @details
    //! コンバイナステージ設定で利用されます。
    //!
    //! 事前にリソースからのロード、または ReserveMem() の呼び出しで
    //! コンバイナステージ設定が有効に設定されている必要があります。
    //! そうでない場合はアサートが発生します。
    //!
    //! @param[in] value    色のソースです。
    //!
    //! @sa GetBufferColorSelectRgb
    //! @sa SetBufferColorSelectAlpha
    //!
    void SetBufferColorSelectRgb(ResDetailedCombinerStage::ConstColorSelect value)
    {
        NN_SDK_ASSERT(UseDetailedCombinerCap(), "UseDetailedCombinerCap() must be true for material[%s]", GetName());
        NN_SDK_ASSERT(value < ResDetailedCombinerStage::ConstColorSelect_Max, "out of bounds: value[%u] < res::RawTevStage::ConstColorSelect_Max[%u] for material[%s]",
            value, ResDetailedCombinerStage::ConstColorSelect_Max, GetName());
        if (UseDetailedCombinerCap() && value < ResDetailedCombinerStage::ConstColorSelect_Max)
        {
            ResDetailedCombinerStageInfo* pCombinerStageInfo = GetDetailedCombinerStageInfoPtr();
            pCombinerStageInfo->SetBufferColorSelectRgb(value);
        }
    }

    //! @brief bufferColorレジスタのAlphaに設定される色のソースを取得します。
    //!
    //! @details
    //! コンバイナステージ設定で利用されます。
    //!
    //! 事前にリソースからのロード、または ReserveMem() の呼び出しで
    //! コンバイナステージ設定が有効に設定されている必要があります。
    //! そうでない場合はアサートが発生します。
    //!
    //! @return bufferColorレジスタのAlphaに設定される色のソースを返します。
    //!
    //! @sa SetBufferColorSelectAlpha
    //! @sa GetBufferColorSelectRgb
    //!
    ResDetailedCombinerStage::ConstColorSelect GetBufferColorSelAlpha() const
    {
        NN_SDK_ASSERT(UseDetailedCombinerCap(), "UseDetailedCombinerCap() must be true for material[%s]", GetName());
        if (UseDetailedCombinerCap())
        {
            const ResDetailedCombinerStageInfo* pCombinerStageInfo = GetDetailedCombinerStageInfoPtr();
            return pCombinerStageInfo->GetBufferColorSelectAlpha();
        }
        else
        {
            return ResDetailedCombinerStage::ConstColorSelect_Black;
        }
    }

    //! @brief bufferColorレジスタのAlphaに設定される色のソースを設定します。
    //!
    //! @details
    //! コンバイナステージ設定で利用されます。
    //!
    //! 事前にリソースからのロード、または ReserveMem() の呼び出しで
    //! コンバイナステージ設定が有効に設定されている必要があります。
    //! そうでない場合はアサートが発生します。
    //!
    //! @param[in] value    色のソースです。
    //!
    //! @sa GetBufferColorSelectAlpha
    //! @sa SetBufferColorSelectRgb
    //!
    void SetBufferColorSelAlpha(ResDetailedCombinerStage::ConstColorSelect value)
    {
        NN_SDK_ASSERT(UseDetailedCombinerCap(), "UseDetailedCombinerCap() must be true for material[%s]", GetName());
        NN_SDK_ASSERT(value < ResDetailedCombinerStage::ConstColorSelect_Max, "out of bounds: value[%u] < ResCombinerStage::CONSTCOLORSELECT_MAX[%u] for material[%s]",
            value, ResDetailedCombinerStage::ConstColorSelect_Max, GetName());
        if (UseDetailedCombinerCap() && value < ResDetailedCombinerStage::ConstColorSelect_Max)
        {
            ResDetailedCombinerStageInfo* pCombinerStageInfo = GetDetailedCombinerStageInfoPtr();
            pCombinerStageInfo->SetBufferColorSelectAlpha(value);
        }
    }

    //@}

    //----------------------------------------
    //! @name レンダーステート
    //@{

    //! @brief アルファコンペア設定を取得します。
    //!
    //! @return アルファコンペア設定を返します。
    //!
    //! @sa SetAlphaCompare
    //!
    const ResAlphaCompare& GetAlphaCompare() const
    {
        NN_SDK_ASSERT(IsAlphaCompareCap(), "IsAlphaCompareCap() must be true for material[%s]", GetName());
        return *GetAlphaComparePtr();
    }

    //! @brief アルファコンペアの設定を行います。
    //!
    //! @param[in] value    アルファコンペア設定です。
    //!
    //! @sa GetAlphaCompare
    //!
    void SetAlphaCompare(const ResAlphaCompare& value)
    {
        NN_SDK_ASSERT(IsAlphaCompareCap(), "IsAlphaCompareCap() must be true for material[%s]", GetName());
        *GetAlphaComparePtr() = value;
    }

    //! @brief ブレンドモード設定を取得します。
    //!
    //! @return ブレンドモード設定を返します。
    //!
    //!
    //! @details IsBlendModeCap() が有効な場合のみ利用できます。
    //!          呼び出す前に、IsBlendModeCap()を判定してください。
    //!
    //! @sa IsBlendModeCap
    //! @sa SetBlendMode
    //!
    const ResBlendMode& GetBlendMode() const
    {
        NN_SDK_ASSERT(IsBlendModeCap(), "IsBlendModeCap() must be true for material[%s]", GetName());
        return *GetBlendModePtr();
    }

    //! @brief ブレンドモードの設定を行います。
    //!
    //! @param[in] value    ブレンドモード設定です。
    //!
    //!
    //! @details IsBlendModeCap() が有効な場合のみ利用できます。
    //!          呼び出す前に、IsBlendModeCap()を判定してください。
    //!
    //! @sa IsBlendModeCap
    //! @sa GetBlendMode
    //!
    void SetBlendMode(ResBlendMode value)
    {
        NN_SDK_ASSERT(IsBlendModeCap(), "IsBlendModeCap() must be true for material[%s]", GetName());
        *GetBlendModePtr() = value;
    }

    //! @brief アルファのブレンドモード設定を取得します。
    //!
    //! @return アルファのブレンドモード設定を返します。
    //!
    //! @details IsBlendModeAlphaCap() が有効な場合のみ利用できます。
    //!          呼び出す前に、IsBlendModeAlphaCap()を判定してください。
    //!
    //! @sa IsBlendModeAlphaCap
    //! @sa SetBlendModeAlpha
    //!
    const ResBlendMode& GetBlendModeAlpha() const
    {
        NN_SDK_ASSERT(IsBlendModeAlphaCap(), "IsBlendModeAlphaCap() must be true for material[%s]", GetName());
        return *GetBlendModeAlphaPtr();
    }

    //! @brief ブレンドモードの設定を行います。
    //!
    //! @param[in] value    ブレンドモード設定です。
    //!
    //!
    //! @details IsBlendModeAlphaCap() が有効な場合のみ利用できます。
    //!          呼び出す前に、IsBlendModeAlphaCap()を判定してください。
    //!
    //! @sa IsBlendModeAlphaCap
    //! @sa GetBlendMode
    //!
    void SetBlendModeAlpha(ResBlendMode value)
    {
        NN_SDK_ASSERT(IsBlendModeAlphaCap(), "IsBlendModeAlphaCap() must be true for material[%s]", GetName());
        *GetBlendModeAlphaPtr() = value;
    }

    //! @brief ブレンドモードの初期化を行い描画に反映されるようにします。
    //!
    //! @param[in] pDevice    デバイスです。
    //!
    //! @details
    //! SetBlendMode や SetBlendModeAlpha で設定を変更した後に、本関数を呼び出して描画に反映されるようにします。
    //! 複数回実行する場合、必ず FinalizeBlendInformation を呼んで、描画用オブジェクトを解放してください。メモリリークが起こります。
    //!
    //! @sa SetBlendMode
    //! @sa SetBlendModeAlpha
    //! @sa FinalizeBlendInformation
    //!
    void InitializeBlendInformation(nn::gfx::Device* pDevice)
    {
        InitializeBlendInformationImpl(pDevice);
    }

    //! @brief ブレンドモードの終了処理を行います。
    //!
    //! @param[in] pDevice    デバイスです。
    //!
    //! @sa InitializeBlendInformation
    //!
    void FinalizeBlendInformation(nn::gfx::Device* pDevice)
    {
        FinalizeBlendInformationImpl(pDevice);
    }

    //! @brief インダイレクトのパラメータを返します。
    //!
    //! @return インダイレクトのパラメータを返します。
    //!
    const ResIndirectParameter& GetIndirectParameter() const
    {
        NN_SDK_ASSERT(IsIndirectParameterCap(), "IsIndirectParameterCap() must be true for material[%s]", GetName());
        return *GetIndirectParameterPtr();
    }

    //! @brief インダイレクトのパラメータの設定を行います。
    //!
    //! @param[in] value    インダイレクトのパラメータ設定です。
    //!
    //! @sa GetBlendMode
    //!
    void SetIndirectParameter(const ResIndirectParameter& value)
    {
        NN_SDK_ASSERT(IsIndirectParameterCap(), "IsIndirectParameterCap() must be true for material[%s]", GetName());
        *GetIndirectParameterPtr() = value;
    }

    //! @brief 投影テクスチャ生成のパラメータを設定します。
    //!
    //! @details
    //! インデックスで指定したテクスチャ座標生成は、平行投影かペイン相対の平行投影である必要があります。
    //!
    //! @param[in] idx      設定したいテクスチャ座標生成のインデックスです。
    //! @param[in] value    投影テクスチャ生成のパラメータ設定です。
    //!
    void SetProjectionTexGenParameters(int  idx, const ResProjectionTexGenParameters& value)
    {
        GetProjectionTexGenArray()[GetProjectionTexGenParametersIdxFromTexCoordGenIdx(idx)] = value;
    }

    //! @brief 投影テクスチャ生成のパラメータを取得します。
    //!
    //! @details
    //! インデックスで指定したテクスチャ座標生成は、平行投影かペイン相対の平行投影である必要があります。
    //!
    //! @param[in] idx     取得したいテクスチャ座標生成のインデックスです。
    //! @return 投影テクスチャ生成のパラメータを返します。
    //!
    const ResProjectionTexGenParameters& GetProjectionTexGenParameters(int  idx) const
    {
        return GetProjectionTexGenArray()[GetProjectionTexGenParametersIdxFromTexCoordGenIdx(idx)];
    }

    //@}

    //----------------------------------------
    //! @name カラー
    //@{

    //! @brief フォント影のパラメータの数を設定します。
    //!
    //! @param[in] count フォント影のパラメータの数です。
    //!
    void SetFontShadowParameterCount(uint8_t  count);

    //! @brief フォント影のパラメータを返します。
    //!
    //! @return フォント影のパラメータを返します。
    //!
    const ResFontShadowParameter& GetFontShadowParameter() const
    {
        NN_SDK_ASSERT(IsFontShadowParameterCap(), "IsFontShadowParameterCap() must be true for material[%s]", GetName());
        return *GetFontShadowParameterPtr();
    }

    //! @brief フォント影のパラメータの設定を行います。
    //!
    //! @param[in] value    フォント影のパラメータです。
    //!
    //! @sa GetBlendMode
    //!
    void SetFontShadowParameter(const ResFontShadowParameter& value)
    {
        NN_SDK_ASSERT(IsFontShadowParameterCap(), "IsFontShadowParameterCap() must be true for material[%s]", GetName());
        *GetFontShadowParameterPtr() = value;
    }

    //! @brief 成分を指定してフォント影のパラメータを設定します。
    //!
    //! eleIdx の指定には nn::ui2d::AnimTargetFontShadow を使用してください。
    //!
    //! @param[in] eleIdx   成分のインデックスです。
    //! @param[in] value    成分の値です。
    //!
    //! @sa AnimTargetFontShadow
    //! @sa ReserveMem
    //!
    void SetFontShadowParameterElement(int  eleIdx, uint8_t  value)
    {
        uint8_t *const elements = GetFontShadowParameterPtr()->blackInterporateColor;
        elements[eleIdx] = value;
    }

    //@}

    //! @brief ベクターグラフィックステクスチャ参照情報のパラメータの数を設定します。
    //!
    //! @param[in] count ベクターグラフィックステクスチャ参照情報のパラメータの数です。
    //!
    void SetRefVectorGraphicsTextureParameterCount(uint8_t count);

    //! @brief ベクターグラフィックステクスチャ参照情報のパラメータの数を取得します。
    //!
    //! @return ベクターグラフィックステクスチャ参照情報のパラメータの数です。
    //!
    uint8_t GetRefVectorGraphicsTextureParameterCount() const
    {
        return m_MemCount.refVectorGraphicsTexture;
    }

    //! @brief 成分を指定してベクターグラフィックステクスチャのパラメータを設定します。
    //!
    //! @param[in] idx      RefVectorGraphicsTextureInfo のインデックスです。
    //! @param[in] elementIdx   成分のインデックスです。
    //! @param[in] value    成分の値です。
    //!
    void SetVectorGraphicsTextureInfoElement(int  idx, int  elementIdx, float value)
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.refVectorGraphicsTexture), "out of bounds: idx[%u] < m_MemCount.refVectorGraphicsTexture[%u] for material[%s]", idx, m_MemCount.texSrt, GetName());

        float *const pParamArray = &GetRefVectorGraphicsTextureInfoPtr()[idx].frame;
        *(pParamArray + elementIdx) = value;
    }

    //! @brief ベクターグラフィックステクスチャの現在の再生フレーム設定を取得します。
    //!
    //! @return 再生フレーム設定です。
    //!
    float GetRefVectorGraphicsTextureFrame(int idx) const
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.refVectorGraphicsTexture), "out of bounds: idx[%u] < m_MemCount.refVectorGraphicsTexture[%u] for material[%s]", idx, m_MemCount.texSrt, GetName());

        return GetRefVectorGraphicsTextureInfoPtr()[idx].frame;
    }

    //! @brief ベクターグラフィックステクスチャのクリアカラー設定を取得します。
    //!
    //! @return クリアカラーです。
    //!
    const nn::util::Float4& GetRefVectorGraphicsTextureClearColor(int idx) const
    {
        NN_SDK_ASSERT(idx < static_cast<int>(m_MemCount.refVectorGraphicsTexture), "out of bounds: idx[%u] < m_MemCount.refVectorGraphicsTexture[%u] for material[%s]", idx, m_MemCount.texSrt, GetName());

        return GetRefVectorGraphicsTextureInfoPtr()[idx].clearColor;
    }

    //----------------------------------------
    //! @name アニメーション
    //@{

    //! @brief アニメーションを関連付けます。
    //!
    //! @param[in] pAnimTrans   アニメーションへのポインタです。
    //!
    //! @sa UnbindAnimation
    //!
    virtual void BindAnimation(AnimTransform* pAnimTrans);

    //! @brief アニメーションの関連付けを解除します。
    //!
    //! @param[in] pAnimTrans   アニメーションへのポインタです。
    //!
    //! @sa BindAnimation
    //!
    virtual void UnbindAnimation(AnimTransform* pAnimTrans);

    //----------------------------------------
    //! @name その他
    //@{

    //! @brief ユーザーがマテリアルを構築したかどうかを判定します。
    //!
    //! @return マテリアルをユーザーが構築した場合は true を返します。
    //!
    bool IsUserAllocated() const
    {
        return detail::TestBit(m_Flag, Flags_UserAllocated);
    }

    //! @brief ユーザーが構築したマテリアルであることを宣言します。
    //!
    //! @details
    //! ユーザが構築したと宣言されたマテリアルはライブラリによって
    //! 解放されません。
    //!
    void SetUserAllocated()
    {
        detail::SetBit(&m_Flag, Flags_UserAllocated, true);
    }

    //@}

    // マテリアルの設置をグラフィックスシステムに送信します。
    // ペインの描画中に呼び出されます。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] drawInfo             描画情報です。
    //! @param[in] alpha                マテリアルカラーに対するアルファ値です。
    //! @param[in] shaderVariation      シェーダの種類
    //! @param[in] bInitializeFrameTransform  フレーム変換にデフォルト値を設定する場合は true を指定します。
    //! @param[in] paneGlbMtx           ペインの平行移動量です。
    //! @param[in] pPaneSize            ペインのサイズです。
    //! @param[in] pExtUserData         拡張ユーザデータ配列です。
    //! @param[in] extUserDataCount     拡張ユーザデータの個数です。
    //!
    virtual void SetupGraphics(
        DrawInfo& drawInfo,
        uint8_t  alpha,
        ShaderVariation shaderVariation,
        bool bInitializeFrameTransform,
        const nn::util::MatrixT4x3fType& paneGlbMtx,
        const nn::ui2d::Size* pPaneSize = NULL,
        const ResExtUserData* pExtUserData = NULL,
        uint16_t extUserDataCount = 0);

    void SetupSubmaterialOf_TextureMatrix(DrawInfo& drawInfo, const nn::util::MatrixT4x3fType& paneGlbMtx, const nn::ui2d::Size* paneSize);

    void SetupSubmaterialOf_TextureMatrix(DrawInfo& drawInfo, const nn::util::MatrixT4x3fType& paneGlbMtx)
    {
        const nn::ui2d::Size* paneSize = NULL;
        SetupSubmaterialOf_TextureMatrix(drawInfo, paneGlbMtx, paneSize);
    }

    void SetupSubmaterialOf_TextureProjectionMatrix(DrawInfo& drawInfo, const nn::util::MatrixT4x3fType& paneGlbMtx, const nn::ui2d::Size* pPaneSize, const ResProjectionTexGenParameters& texGen, int i) const;

    void SetRcpTexSize(const DrawInfo& drawInfo) const;
    void SetupSubmaterialOf_Texture(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer) const;
    void SetupSubmaterialOf_Tev(DrawInfo& drawInfo, const ResExtUserData* pExtUserData, uint16_t extUserDataCount, const nn::ui2d::Size* pPaneSize) const;
    void SetupSubmaterialOf_DetailedCombiner(DrawInfo& drawInfo) const;
    void SetupSubmaterialOf_CombinerUserShader(DrawInfo& drawInfo, const ResExtUserData* pExtUserData, uint16_t extUserDataCount, const nn::ui2d::Size* pPaneSize) const;
    void SetupSubmaterialOf_FramebufferTexture(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer) const;
    bool IsUseFramebufferTexture() const;

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] drawInfo DrawInfoです。
    //!
    void SetAllowableTextureQuantity(const DrawInfo& drawInfo) { m_PackedValues.allowableTextureQuantity = static_cast<int8_t>(drawInfo.GetAllowableTextureQuantity()); }

    struct ConstantBufferForVertexShader
    {
        float projection[4][4];
        float modelView[3][4];
        float texMtx0XZ[4];
        float texMtx0YW[4];
        float texMtx1XZ[4];
        float texMtx1YW[4];
        float texMtx2XZ[4];
        float texMtx2YW[4];
        float color[4];
        float vertexTexCoord0[4][4];
        float vertexTexCoord1[4][4];
        float vertexTexCoord2[4][4];
        float frameSize[4];
        float paneSize[2];
        float padding0[2];
        float rcpTexSize0[4];
        float vertexColor[4][4];
        float transform[4];
        int generatingTexCoord[3];
        int frameSpec;
    };

    struct ConstantBufferForPixelShader
    {
        float interpolateWidth[4];
        float interpolateOffset[4];
        float indirectMtx0[4];
        float indirectMtx1[4];
        float vertexColor[4][4];   // 現在利用されていないパディング領域です。
        int textureMode;
        int colorEffectMode;       // 現在利用されていないパディング領域です。
        float padding0[2];
        // 直後にユーザーシェーダーの変数が続くためアライメントがずれないようにパディングを入れておく。
    };

    struct ConstantBufferForDetailedCombinerPixelShader
    {
        int stageCountMax;
        float padding1[3];
        // コンスタントカラー は 固定（白黒）+ 指定可能色
        float constantColor[DetailedCombinerConstantColor + 2][4];
        int stage_bit[TevStageMax][4];
    };

    struct ConstantBufferForCombinerUserShaderPixelShader
    {
        float fixedZero;
        float paneWidth;
        float paneHeight;
        int padding[1];
        float constantColor[CombinerUserShaderConstantColor + 2][4];

        float modelViewMatrix[3][4];
        float modelMatrix[3][4];
        float viewMatrix[3][4];
        float cameraPosition[3];
        float padding2[1];

        // 拡張ユーザーデータ
        float userData_float[4];
        float userData_vec2[4][4];  // [2], [3] は padding
        float userData_vec3[4][4];  // [3] は padding
        float userData_rgba[4][4];

        float textureSize[TexMapMax][4];
    };



    // １フレームの描画に使用するコンスタントバッファの領域を確定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] drawInfo DrawInfoです。
    //!
    void AllocateConstantBuffer(DrawInfo& drawInfo);

    // BlendState が設定されていない場合にプリセットの BlendState を使うように設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pDrawInfo DrawInfoです。
    //!
    void SetupBlendState(const DrawInfo* pDrawInfo);

    //! @brief ユーザー頂点シェーダー用コンスタントバッファのポインタを取得します。
    //!
    //! @param[in]  drawInfo DrawInfo です。
    //!
    //! @return ユーザー頂点シェーダー用コンスタントバッファが確保されていればそのポインタを返します。確保されていない場合は NULL を返します。
    //!
    void* GetConstantBufferForUserVertexShader(const DrawInfo& drawInfo) const;

    //! @brief ユーザーピクセルシェーダー用コンスタントバッファのポインタを取得します。
    //!
    //! @param[in]  drawInfo DrawInfo です。
    //!
    //! @return ユーザーピクセルシェーダー用コンスタントバッファが確保されていればそのポインタを返します。確保されていない場合は NULL を返します。
    //!
    void* GetConstantBufferForUserPixelShader(const DrawInfo& drawInfo) const;

    //! @brief ユーザージオメトリシェーダー用コンスタントバッファのポインタを取得します。
    //!
    //! @param[in]  drawInfo DrawInfo です。
    //!
    //! @return ユーザージオメトリシェーダー用コンスタントバッファが確保されていればそのポインタを返します。確保されていない場合は NULL を返します。
    //!
    void* GetConstantBufferForUserGeometryShader(const DrawInfo& drawInfo) const;

    //! @brief 詳細なコンバイナピクセルシェーダー用コンスタントバッファのポインタを取得します。
    //!
    //! @param[in]  drawInfo DrawInfo です。
    //!
    //! @return 詳細なコンバイナピクセルシェーダー用コンスタントバッファが確保されていればそのポインタを返します。確保されていない場合は NULL を返します。
    //!
    ConstantBufferForDetailedCombinerPixelShader* GetConstantBufferForDetailedCombinerPixelShader(const DrawInfo& drawInfo) const;

    //! @brief コンバイナユーザーシェーダのピクセルシェーダー用コンスタントバッファのポインタを取得します。
    //!
    //! @param[in]  drawInfo DrawInfo です。
    //!
    //! @return コンバイナユーザーシェーダのピクセルシェーダー用コンスタントバッファが確保されていればそのポインタを返します。確保されていない場合は NULL を返します。
    //!
    ConstantBufferForCombinerUserShaderPixelShader* GetConstantBufferForCombinerUserShaderPixelShader(const DrawInfo& drawInfo) const;

    ConstantBufferForVertexShader* GetConstantBufferForVertexShader(const DrawInfo& drawInfo) const;
    ConstantBufferForPixelShader* GetConstantBufferForPixelShader(const DrawInfo& drawInfo) const;
    void SetCommandBuffer(nn::gfx::CommandBuffer& commandBuffer, DrawInfo& drawInfo) const;
    void ApplyVertexShaderConstantBuffer(nn::gfx::CommandBuffer& commandBuffer, DrawInfo& drawInfo) const;
    void ApplyGeometryShaderConstantBuffer(nn::gfx::CommandBuffer& commandBuffer, DrawInfo& drawInfo) const;
    void ApplyPixelShaderConstantBuffer(nn::gfx::CommandBuffer& commandBuffer, DrawInfo& drawInfo) const;
    void ApplyPixelShaderConstantBufferDefault(nn::gfx::CommandBuffer& commandBuffer, DrawInfo& drawInfo) const;
    void ApplyPixelShaderDetailedCombinerConstantBuffer(nn::gfx::CommandBuffer& commandBuffer, DrawInfo& drawInfo) const;
    void ApplyPixelShaderCombinerUserShaderConstantBuffer(nn::gfx::CommandBuffer& commandBuffer, DrawInfo& drawInfo) const;
    void SetCommandBufferOnlyBlend(nn::gfx::CommandBuffer& commandBuffer) const;
    void SetShader(nn::gfx::CommandBuffer& commandBuffer) const;
    void EnableAlphaTest(nn::gfx::CommandBuffer& commandBuffer) const;
    void DisableAlphaTest() const;

    // 頂点シェーダのユニフォームブロックスロットを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return 頂点シェーダのユニフォームブロックスロットです。
    //!
    int GetConstantBufferSlotForVertexShader() const
    {
        return m_pShaderInfo->GetVertexShaderSlot(m_ShaderVariation);
    }

    //! @brief コピーコンストラクタでコピーされたインスタンスが正しくコピーされているか確認します。
    //!
    //! @details
    //! このメソッドは ui2d ランタイムのテストのために作成されたメソッドです。
    //! アプリケーションコードでの使用は非推奨となります。
    //!
    //! @param[in]  target  比較対象のペインです。
    //!
    //! @return 同等とみなせる場合は true を返します。
    //!
    bool CompareCopiedInstanceTest(const Material& target) const;

    // ShaderInfo を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return ShaderInfo です。
    //!
    const ShaderInfo* GetShaderInfo() const
    {
        return m_pShaderInfo;
    }

    // ShaderVariation を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return ShaderVariation です。
    //!
    uint16_t GetShaderVariation() const
    {
        return m_ShaderVariation;
    }

protected:
    //! @brief テクスチャ画像設定の配列を取得します。
    //!
    //! @return テクスチャ画像設定の配列を返します。
    //!
    const TexMap* GetTexMapArray() const;

    //! @brief テクスチャ画像設定の配列を取得します。
    //!
    //! @return テクスチャ画像設定の配列を返します。
    //!
    TexMap* GetTexMapArray();

    //! @brief テクスチャ座標変換設定の配列を取得します。
    //!
    //! @return テクスチャ座標変換設定の配列を返します。
    //!
    const ResTexSrt* GetTexSrtArray() const;

    //! @brief テクスチャ座標変換設定の配列を取得します。
    //!
    //! @return テクスチャ座標変換設定の配列を返します。
    //!
    ResTexSrt* GetTexSrtArray();

    //! @brief テクスチャ座標生成設定の配列を取得します。
    //!
    //! @return テクスチャ座標生成設定の配列を返します。
    //!
    const ResTexCoordGen* GetTexCoordGenArray() const;

    //! @brief テクスチャ座標生成設定の配列を取得します。
    //!
    //! @return テクスチャ座標生成設定の配列を返します。
    //!
    ResTexCoordGen* GetTexCoordGenArray();

    //! @brief アルファコンペア設定のアドレスを取得します。
    //!
    //! @return アルファコンペア設定のアドレスを返します。
    //!
    const ResAlphaCompare* GetAlphaComparePtr() const;

    //! @brief アルファコンペア設定のアドレスを取得します。
    //!
    //! @return アルファコンペア設定のアドレスを返します。
    //!
    ResAlphaCompare* GetAlphaComparePtr();

    //! @brief ブレンドモード設定のアドレスを取得します。
    //!
    //! @return ブレンドモード設定のアドレスを返します。
    //!
    const ResBlendMode* GetBlendModePtr() const;

    //! @brief ブレンドモード設定のアドレスを取得します。
    //!
    //! @return ブレンドモード設定のアドレスを返します。
    //!
    ResBlendMode* GetBlendModePtr();

    //! @brief アルファのブレンドモード設定のアドレスを取得します。
    //!
    //! @return アルファのブレンドモード設定のアドレスを返します。
    //!
    const ResBlendMode* GetBlendModeAlphaPtr() const;

    //! @brief アルファのブレンドモード設定のアドレスを取得します。
    //!
    //! @return アルファのブレンドモード設定のアドレスを返します。
    //!
    ResBlendMode* GetBlendModeAlphaPtr();

    //! @brief インダイレクトのパラメータのアドレスを取得します。
    //!
    //! @return インダイレクトのパラメータのアドレスを返します。
    //!
    const ResIndirectParameter* GetIndirectParameterPtr() const;

    //! @brief インダイレクトのパラメータのアドレスを取得します。
    //!
    //! @return インダイレクトのパラメータのアドレスを返します。
    //!
    ResIndirectParameter* GetIndirectParameterPtr();

    //! @brief 詳細なコンバイナステージ設定の付加情報のアドレスを取得します。
    //!
    //! @return 詳細なコンバイナステージ設定の付加情報のアドレスを返します。
    //!
    ResDetailedCombinerStageInfo* GetDetailedCombinerStageInfoParameterPtr();

    //! @brief 詳細なコンバイナステージ設定のパラメータのアドレスを取得します。
    //!
    //! @return 詳細なコンバイナステージ設定のパラメータのアドレスを返します。
    //!
    ResDetailedCombinerStage* GetDetailedCombinerStageParameterPtr();

    //! @brief 投影テクスチャ生成の配列を取得します。
    //!
    //! @return 投影テクスチャ生成の配列を返します。
    //!
    const ResProjectionTexGenParameters* GetProjectionTexGenArray() const;

    //! @brief 投影テクスチャ生成の配列を取得します。
    //!
    //! @return 投影テクスチャ生成の配列を返します。
    //!
    ResProjectionTexGenParameters* GetProjectionTexGenArray();

    //! @brief TEV設定のアドレスを取得します。
    //!
    //! @return TEV設定のアドレスを返します。
    //!
    const ResTevStage* GetTevStageArray() const;

    //! @brief TEV設定の配列を取得します。
    //!
    //! @return TEV設定の配列を返します。
    //!
    ResTevStage* GetTevStageArray();

    //! @brief フォント影のパラメータを取得します。
    //!
    //! @return フォント影のパラメータを返します。
    //!
    const ResFontShadowParameter* GetFontShadowParameterPtr() const;

    //! @brief フォント影のパラメータを取得します。
    //!
    //! @return フォント影のパラメータを返します。
    //!
    ResFontShadowParameter* GetFontShadowParameterPtr();

    //! @brief テクスチャ座標生成のインデックスから対応する投影テクスチャ生成のパラメータのインデックスを取得します。
    //!
    //! @param[in] texCoordGenIdx   投影テクスチャ生成のパラメータのインデックスを取得したいテクスチャ座標生成のインデックスです。
    //!
    //! @return 投影テクスチャ生成のパラメータのインデックスを返します。
    //!
    int  GetProjectionTexGenParametersIdxFromTexCoordGenIdx(int  texCoordGenIdx) const;

    //! @brief 詳細なコンバイナステージ設定の付加情報のアドレスを取得します。
    //!
    //! @return 詳細なコンバイナステージ設定の付加情報のアドレスを返します。
    //!
    inline
    const ResDetailedCombinerStageInfo* GetDetailedCombinerStageInfoPtr() const;

    //! @brief 詳細なコンバイナステージ設定の付加情報のアドレスを取得します。
    //!
    //! @return 詳細なコンバイナステージ設定の付加情報のアドレスを返します。
    //!
    inline
    ResDetailedCombinerStageInfo* GetDetailedCombinerStageInfoPtr();

    //! @brief 詳細なコンバイナステージ設定の配列を取得します。
    //!
    //! @return 詳細なコンバイナステージV設定の配列を返します。
    //!
    inline
    const ResDetailedCombinerStage* GetDetailedCombinerStageAry() const;

    //! @brief 詳細なコンバイナステージ設定の配列を取得します。
    //!
    //! @return 詳細なコンバイナステージ設定の配列を返します。
    //!
    inline
    ResDetailedCombinerStage* GetDetailedCombinerStageAry();

    //! @brief コンバイナユーザーシェーダ設定のアドレスを取得します。
    //!
    //! @return コンバイナユーザーシェーダ設定のアドレスを返します。
    //!
    inline
    ResCombinerUserShader* GetCombinerUserShaderPtr();

    //! @brief コンバイナユーザーシェーダ設定のアドレスを取得します。
    //!
    //! @return コンバイナユーザーシェーダ設定のアドレスを返します。
    //!
    inline
    const ResCombinerUserShader* GetCombinerUserShaderPtr() const;

    //! @brief ベクターグラフィックステクスチャの参照情報を取得します。
    //!
    //! @return ベクターグラフィックステクスチャの参照情報を返します。
    //!
    inline
    detail::RefVectorGraphicsTextureInfo* GetRefVectorGraphicsTextureInfoPtr();

    //! @brief ベクターグラフィックステクスチャの参照情報を取得します。
    //!
    //! @return ベクターグラフィックステクスチャの参照情報を返します。
    //!
    inline
    const detail::RefVectorGraphicsTextureInfo* GetRefVectorGraphicsTextureInfoPtr() const;


private:
    // ユーザーシェーダー使用時のコンスタントバッファ拡張情報。
    // ユーザーシェーダー未使用時に無駄なメモリを消費しないように構造体にまとめて
    // 必要に応じてメモリを確保するようにする。
    struct UserShaderConstantBufferInformation
    {
        uint32_t    m_ConstantBufferExtendAreaSizeForVertexShader;
        uint32_t    m_ConstantBufferExtendAreaSizeForPixelShader;
        uint32_t    m_ConstantBufferExtendAreaSizeForGeometryShader;

        // ジオメトリシェーダー用コンスタントバッファのオフセット。
        // ジオメトリシェーダーはユーザーシェーダーでのみ使用されるので、メモリ節約のためのこの構造体に含めます。
        uint32_t m_GeometryShaderConstantBufferOffset;

        // デフォルト値を設定します。
        void SetDefault()
        {
            m_ConstantBufferExtendAreaSizeForVertexShader = 0;
            m_ConstantBufferExtendAreaSizeForPixelShader = 0;
            m_ConstantBufferExtendAreaSizeForGeometryShader = 0;

            m_GeometryShaderConstantBufferOffset = 0;
        }
    };

    // 標準シェーダの 3 つ目の検索キーです。
    enum CommonShaderThirdKey
    {
        CommonShaderThirdKey_None = 0,
        CommonShaderThirdKey_LessBlend = 1 << 0, // ブレンド数が 1 つ以下
        CommonShaderThirdKey_UsePerspectiveTextureProjection = 1 << 1,
        CommonShaderThirdKey_UseProceduralShape = 1 << 2
    };

    // 標準シェーダの 4 つ目の検索キーです。
    enum CommonShaderFourthKey
    {
        CommonShaderFourthKey_None = 0,

        // 角丸パラメータ
        CommonShaderFourthKey_ProceduralShape_Enabled = 0,
        CommonShaderFourthKey_ProceduralShape_InnerStrokeEnabled = 1,
        CommonShaderFourthKey_ProceduralShape_GradationOverlayEnabled = 2,
        CommonShaderFourthKey_ProceduralShape_ColorOverlayEnabled = 3,
        CommonShaderFourthKey_ProceduralShape_InnerShadowlayEnabled = 4,
        CommonShaderFourthKey_ProceduralShape_DropShadowlayEnabled = 5,

        CommonShaderFourthKey_ProceduralShape_InnerStrokeBlendType = 6,    // 2bits
        CommonShaderFourthKey_ProceduralShape_GradationOverlayBlendType = 8,   // 2bits
        CommonShaderFourthKey_ProceduralShape_ColorOverlayBlendType = 10,   // 2bits
    };

    void Initialize();
    void InitializeMaterialImpl(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        const ResMaterial* pBaseRes,
        const ResMaterial* pOverrideRes,
        const BuildArgSet& buildArgSet);
    void InitializeMatMemCount(detail::MatMemCount* pMatMemCount) const;
    // ベクターグラフィックステクスチャ参照情報を設定します。
    void SetVectorGraphicsTexturRefInfo(detail::RefVectorGraphicsTextureInfo* pRefInfo, const ResVectorGraphicsTextureInfo& vectorGraphicsTextureInfo, detail::VectorGraphicsTexture* pVgTexture) const;
    // TexMap を初期化します。
    void InitializeTexMap(
        nn::gfx::Device* pDevice,
        const BuildResSet* pBuildResSet,
        const BuildArgSet& buildArgSet,
        const ResMaterial* pTextureRes,
        const ResTexMap *const pResTexMap,
        const ResTexMapAdditionalInfo *const pResTexMapAdditionalInfo,
        const ResVectorGraphicsTextureInfo *const pVectorGraphicsTextureInfo);
    // TexMap で参照されているキャプチャテクスチャの初期化処理を行います。
    CaptureTexture* InitializeCaptureTextureReference(TexMap* pTexMap, const BuildResSet* pBuildResSet, const BuildArgSet& buildArgSet, const char* pRefFileName);
    void SetupShader(nn::gfx::Device* pDevice, const BuildArgSet& buildArgSet, const BuildResSet* pBuildResSet);
    bool TryToSetupUserShader(nn::gfx::Device* pDevice, const BuildArgSet& buildArgSet, const BuildResSet* pBuildResSet);
    bool TryToSetupDetailedCombinerOrCombinerUserShader(nn::gfx::Device* pDevice, const BuildResSet* pBuildResSet);
    bool TryToSetupSharedShader(size_t* pSearchKeyLength, uint32_t* pSearchKey, const BuildArgSet& buildArgSet);
    void SetupArchiveShader(nn::gfx::Device* pDevice, const BuildResSet* pBuildResSet, size_t searchKeyLength, const uint32_t* pSearchKey);
    uint16_t GetArchiveShaderVariation(int currentShaderVariation, int variationKind) const;
    void CopyDynamicRenderingTexture(const Material& material, MaterialCopyContext& copyContext);

    bool IsIndirectBlendUsed() const;
    bool IsPerspectiveTextureProjectionUsed() const;
    int GetProceduralShapeVariationBitFlags(const BuildArgSet& buildArgSet) const;

    void SetupUserShaderConstantBufferInformation(const UserShaderInformation& userShaderInformation);
    void CopyUserShaderConstantBufferInformation(const Material& material);
    void CollectConstantBufferSize(BuildResultInformation* pOutBuildResultInformation, nn::gfx::Device* pDevice) const;
    void InitializeBlendInformationImpl(nn::gfx::Device* pDevice);
    void FinalizeBlendInformationImpl(nn::gfx::Device* pDevice);

    PresetBlendStateId GetBlendStateId() const;

    size_t GetVertexShaderConstantBufferSize() const;
    size_t GetGeometryShaderConstantBufferSize() const;
    size_t GetPixelShaderConstantBufferSize() const;
    size_t GetPixelShaderDetailedCombinerConstantBufferSize() const;
    size_t GetPixelShaderCombinerUserShaderConstantBufferSize() const;

    size_t CalculateReserveMemSize(
        int texMapNum,
        int texSrtNum,
        int texCoordGenNum,
        int tevStageNum,
        int alpCompNum,
        int blendModeNum,
        int indirectParameterNum,
        int projectionTexGenNum,
        int fontShadowParameterNum,
        int detailedCombinerParameterNum,
        int combinerUserShaderParameterNum,
        int refVectorGraphicsTextureNum
    );

    size_t CalculateReserveMemSize(detail::MatMemCount&    memCount)
    {
        return CalculateReserveMemSize(
            memCount.texMap,
            memCount.texSrt,
            memCount.texCoordGen,
            memCount.tevStage,
            memCount.alpComp,
            memCount.blendMode,
            memCount.indirectParameter,
            memCount.projectionTexGen,
            memCount.fontShadowParameter,
            memCount.detailedCombinerParameter,
            memCount.combinerUserShaderParameter,
            memCount.refVectorGraphicsTexture);
    }

    void
    ResParameterToMemory(
        int texMapNum,
        int texSrtNum,
        int texCoordGenNum,
        int tevStageNum,
        int alpCompNum,
        int blendModeNum,
        int indirectParameterNum,
        int projectionTexGenNum,
        int fontShadowParameterNum,
        int combineParameterNum,
        int combinerUserShaderParameterNum,
        int refVectorGraphicsTextureNum);

    // コピー演算子を禁止します。
    const Material& operator=( const Material& );

private:
    union
    {
        nn::util::Unorm8x4  byteColor[MatColorMax];
        nn::util::Float4*   pFloatColor;
    } m_Colors;

    detail::MatMemCount m_MemCap;
    detail::MatMemCount m_MemCount;
    void* m_pMem;
    const ShaderInfo* m_pShaderInfo;
    const char* m_pName;

    uint32_t m_VertexShaderConstantBufferOffset;
    uint32_t m_PixelShaderConstantBufferOffset;

    // ユーザーシェーダー使用時のコンスタントバッファ拡張情報
    UserShaderConstantBufferInformation* m_pUserShaderConstantBufferInformation;

    nn::gfx::BlendState* m_pBlendState;

    struct PackedValues
    {
        int8_t  allowableTextureQuantity: 4;
        uint8_t  shaderId: 4;
    };
    NN_STATIC_ASSERT(sizeof(PackedValues) == 1);

    PackedValues m_PackedValues;

    uint8_t  m_Flag;

    // 描画時に使用するシェーダバリエーションの番号です。
    uint16_t m_ShaderVariation;

    void SetupConstantBufferColor_for_CombinerUserShader(ConstantBufferForCombinerUserShaderPixelShader* pCombinerUserShaderConstantBuffer) const;
    void SetupConstantBufferPosture_for_CombinerUserShader(ConstantBufferForCombinerUserShaderPixelShader* pCombinerUserShaderConstantBuffer, DrawInfo& drawInfo) const;
    void SetupConstantBufferExData_for_CombinerUserShader(ConstantBufferForCombinerUserShaderPixelShader* pCombinerUserShaderConstantBuffer, const ResExtUserData* pExtUserData, uint16_t extUserDataCount) const;
    void SetupConstantBufferTextureData_for_CombinerUserShader(ConstantBufferForCombinerUserShaderPixelShader* pCombinerUserShaderConstantBuffer) const;

    void FinalizeTexMap(nn::gfx::Device* pDevice);
};

//----------------------------------------
inline const TexMap*
Material::GetTexMapArray() const
{
    return nn::util::BytePtr(m_pMem, 0).Get<TexMap>();
}

//----------------------------------------
inline TexMap*
Material::GetTexMapArray()
{
    return nn::util::BytePtr(m_pMem, 0).Get<TexMap>();
}

//----------------------------------------
inline const ResTexSrt*
Material::GetTexSrtArray() const
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetTexSrtArray(m_MemCap)).Get<ResTexSrt>();
}

//----------------------------------------
inline ResTexSrt*
Material::GetTexSrtArray()
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetTexSrtArray(m_MemCap)).Get<ResTexSrt>();
}

//----------------------------------------
inline const ResTexCoordGen*
Material::GetTexCoordGenArray() const
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetTexCoordGenArray(m_MemCap)).Get<ResTexCoordGen>();
}

//----------------------------------------
inline ResTexCoordGen*
Material::GetTexCoordGenArray()
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetTexCoordGenArray(m_MemCap)).Get<ResTexCoordGen>();
}

//----------------------------------------
inline const ResAlphaCompare*
Material::GetAlphaComparePtr() const
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetAlphaCompare(m_MemCap)).Get<ResAlphaCompare>();
}

//----------------------------------------
inline ResAlphaCompare*
Material::GetAlphaComparePtr()
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetAlphaCompare(m_MemCap)).Get<ResAlphaCompare>();
}

//----------------------------------------
inline const ResBlendMode*
Material::GetBlendModePtr() const
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetBlendMode(m_MemCap)).Get<ResBlendMode>();
}

//----------------------------------------
inline ResBlendMode*
Material::GetBlendModePtr()
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetBlendMode(m_MemCap)).Get<ResBlendMode>();
}

//----------------------------------------
inline const ResBlendMode*
Material::GetBlendModeAlphaPtr() const
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetBlendMode(m_MemCap) + sizeof(ResBlendMode)).Get<ResBlendMode>();
}

//----------------------------------------
inline ResBlendMode*
Material::GetBlendModeAlphaPtr()
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetBlendMode(m_MemCap) + sizeof(ResBlendMode)).Get<ResBlendMode>();
}

//----------------------------------------
inline const ResIndirectParameter*
Material::GetIndirectParameterPtr() const
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetIndirectParameter(m_MemCap)).Get<ResIndirectParameter>();
}

//----------------------------------------
inline ResIndirectParameter*
Material::GetIndirectParameterPtr()
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetIndirectParameter(m_MemCap)).Get<ResIndirectParameter>();
}

//----------------------------------------
inline ResDetailedCombinerStageInfo*
Material::GetDetailedCombinerStageInfoParameterPtr()
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetDetailedCombinerStageInfo(m_MemCap)).Get<ResDetailedCombinerStageInfo>();
}

//----------------------------------------
inline ResDetailedCombinerStage*
Material::GetDetailedCombinerStageParameterPtr()
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetDetailedCombinerStage(m_MemCap)).Get<ResDetailedCombinerStage>();
}

//----------------------------------------
inline const ResProjectionTexGenParameters*
Material::GetProjectionTexGenArray() const
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetProjectionTexGen(m_MemCap)).Get<ResProjectionTexGenParameters>();
}

//----------------------------------------
inline ResProjectionTexGenParameters*
Material::GetProjectionTexGenArray()
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetProjectionTexGen(m_MemCap)).Get<ResProjectionTexGenParameters>();
}

//----------------------------------------
inline const ResTevStage*
Material::GetTevStageArray() const
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetTevStageArray(m_MemCap)).Get<ResTevStage>();
}

//----------------------------------------
inline ResTevStage*
Material::GetTevStageArray()
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetTevStageArray(m_MemCap)).Get<ResTevStage>();
}

//----------------------------------------
inline const ResFontShadowParameter*
Material::GetFontShadowParameterPtr() const
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetFontShadowParameter(m_MemCap)).Get<ResFontShadowParameter>();
}

//----------------------------------------
inline ResFontShadowParameter*
Material::GetFontShadowParameterPtr()
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetFontShadowParameter(m_MemCap)).Get<ResFontShadowParameter>();
}

//----------------------------------------
inline ResDetailedCombinerStageInfo*
Material::GetDetailedCombinerStageInfoPtr()
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetDetailedCombinerStageInfo(m_MemCap)).Get<ResDetailedCombinerStageInfo>();
}

//----------------------------------------
inline const ResDetailedCombinerStageInfo*
Material::GetDetailedCombinerStageInfoPtr() const
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetDetailedCombinerStageInfo(m_MemCap)).Get<ResDetailedCombinerStageInfo>();
}

//----------------------------------------
inline ResDetailedCombinerStage*
Material::GetDetailedCombinerStageAry()
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetDetailedCombinerStage(m_MemCap)).Get<ResDetailedCombinerStage>();
}
//----------------------------------------
inline const ResDetailedCombinerStage*
Material::GetDetailedCombinerStageAry() const
{
    return nn::util::BytePtr(m_pMem, detail::CalculateOffsetDetailedCombinerStage(m_MemCap)).Get<ResDetailedCombinerStage>();
}

//----------------------------------------
inline ResCombinerUserShader*
Material::GetCombinerUserShaderPtr()
{
    return nn::util::BytePtr(m_pMem, detail::CaluculateOffsetCombinerUserShader(m_MemCap)).Get<ResCombinerUserShader>();
}

//----------------------------------------
inline const ResCombinerUserShader*
Material::GetCombinerUserShaderPtr() const
{
    return nn::util::BytePtr(m_pMem, detail::CaluculateOffsetCombinerUserShader(m_MemCap)).Get<ResCombinerUserShader>();
}

//----------------------------------------
inline detail::RefVectorGraphicsTextureInfo*
Material::GetRefVectorGraphicsTextureInfoPtr()
{
    return nn::util::BytePtr(m_pMem, detail::CaluculateOffsetRefVectorGraphicsTextureInfo(m_MemCap)).Get<detail::RefVectorGraphicsTextureInfo>();
}

//----------------------------------------
inline const detail::RefVectorGraphicsTextureInfo*
Material::GetRefVectorGraphicsTextureInfoPtr() const
{
    return nn::util::BytePtr(m_pMem, detail::CaluculateOffsetRefVectorGraphicsTextureInfo(m_MemCap)).Get<detail::RefVectorGraphicsTextureInfo>();
}

//----------------------------------------
inline void
Material::SetName(const char* pName)
{
    m_pName = pName;
}

//----------------------------------------
inline void
Material::SetTexMapCount(uint8_t  count)
{
    NN_SDK_ASSERT(count <= m_MemCap.texMap, "out of bounds: count[%u] <= m_MemCap.texMap[%u] for material[%s]", count, m_MemCap.texMap, GetName());
    m_MemCount.texMap = count;
}

//----------------------------------------
inline void
Material::SetTexSrtCount(uint8_t  count)
{
    NN_SDK_ASSERT(count <= m_MemCap.texSrt, "out of bounds: count[%u] <= m_MemCap.texSrt[%u] for material[%s]", count, m_MemCap.texSrt, GetName());
    m_MemCount.texSrt = count;
}

//----------------------------------------
inline void
Material::SetTexCoordGenCount(uint8_t  count)
{
    NN_SDK_ASSERT(count <= m_MemCap.texCoordGen, "out of bounds: count[%u] <= m_MemCap.texCoordGen[%u] for material[%s]", count, m_MemCap.texCoordGen, GetName());
    m_MemCount.texCoordGen = count;
}

//----------------------------------------
inline void
Material::SetTevStageCount(uint8_t  count)
{
    NN_SDK_ASSERT(count <= m_MemCap.tevStage, "out of bounds: count[%u] <= m_MemCap.tevStage[%u] for material[%s]", count, m_MemCap.tevStage, GetName());
    m_MemCount.tevStage = count;
}

//----------------------------------------
inline void
Material::SetProjectionTexGenCount(uint8_t  count)
{
    NN_SDK_ASSERT(count <= m_MemCap.projectionTexGen, "out of bounds: count[%u] <= m_MemCap.projectionTexGen[%u] for material[%s]", count, m_MemCap.projectionTexGen, GetName());
    m_MemCount.projectionTexGen = count;
}

//----------------------------------------
inline void
Material::SetFontShadowParameterCount(uint8_t  count)
{
    NN_SDK_ASSERT(count <= m_MemCap.fontShadowParameter, "out of bounds: count[%u] <= m_MemCap.fontShadowParameter[%u] for material[%s]", count, m_MemCap.fontShadowParameter, GetName());
    m_MemCount.fontShadowParameter = count;
}

//----------------------------------------
inline void
Material::SetRefVectorGraphicsTextureParameterCount(uint8_t  count)
{
    NN_SDK_ASSERT(count <= m_MemCap.refVectorGraphicsTexture, "out of bounds: count[%u] <= m_MemCap.refVectorGraphicsTexture[%u] for material[%s]", count, m_MemCap.refVectorGraphicsTexture, GetName());
    m_MemCount.refVectorGraphicsTexture = count;
}


//----------------------------------------
inline int
Material::GetColorElement(int  colorType) const
{
    if (colorType < AnimTargetMatColor_MaxAnimTargetMatColor)
    {
        NN_STATIC_ASSERT(AnimTargetMatColor_MaxAnimTargetMatColor % 4 == 0);
        NN_STATIC_ASSERT(AnimTargetMatColor_MaxAnimTargetMatColor / 4 == MatColorMax + DetailedCombinerConstantColor);
        NN_STATIC_ASSERT(AnimTargetMatColor_BufferR + 4 == AnimTargetMatColor_Konst0R);
        NN_STATIC_ASSERT(AnimTargetMatColor_BufferR + 4 * MatColorMax == AnimTargetMatColor_C0_R);
        NN_STATIC_ASSERT(DetailedCombinerConstantColor == CombinerUserShaderConstantColor);

        const uint32_t idx = colorType / 4;

        // float データに対しての呼び出しはアサートする。
        NN_SDK_ASSERT(!IsInterpolateColorFloat(static_cast<InterpolateColor>(idx)),
            "Invalid function call for float-color! Use GetColorElementFloat() instead : idx[%u] for material[%s]", idx, GetName());

        if (idx < MatColorMax)
        {
            switch (colorType % 4)
            {
            case 0: return m_Colors.byteColor[idx].v[0];
            case 1: return m_Colors.byteColor[idx].v[1];
            case 2: return m_Colors.byteColor[idx].v[2];
            case 3: return m_Colors.byteColor[idx].v[3];
            default: NN_SDK_ASSERT(false); return 0;
            }
        }
        else if (idx < MatColorMax + DetailedCombinerConstantColor)
        {
            if (UseDetailedCombinerCap())
            {
                nn::util::Unorm8x4 color = GetConstantColor(idx - MatColorMax);
                switch (colorType % 4)
                {
                case 0: return color.v[0];
                case 1: return color.v[1];
                case 2: return color.v[2];
                case 3: return color.v[3];
                default: NN_SDK_ASSERT(false); return 0;
                }
            }
            else if (UseCombinerUserShaderCap())
            {
                const ResCombinerUserShader* pCombinerUserShader = GetCombinerUserShaderPtr();
                nn::util::Unorm8x4 color = pCombinerUserShader->GetConstantColor(idx - MatColorMax);
                switch (colorType % 4)
                {
                case 0: return color.v[0];
                case 1: return color.v[1];
                case 2: return color.v[2];
                case 3: return color.v[3];
                default: NN_SDK_ASSERT(false); return 0;
                }
            }
        }
    }

    return std::numeric_limits<uint8_t>::max();
}

//----------------------------------------
inline float
Material::GetColorElementFloat(int  colorType) const
{
    NN_STATIC_ASSERT(AnimTargetMatColorFloat_MaxAnimTargetMatColor % 4 == 0);
    NN_STATIC_ASSERT((AnimTargetMatColorFloat_MaxAnimTargetMatColor - AnimTargetMatColor_MaxAnimTargetMatColor) / 4
                        == MatColorMax + DetailedCombinerConstantColor);
    NN_STATIC_ASSERT(AnimTargetMatColorFloat_BufferR + 4 == AnimTargetMatColorFloat_Konst0R);
    NN_STATIC_ASSERT(AnimTargetMatColorFloat_BufferR + 4 * MatColorMax == AnimTargetMatColorFloat_C0_R);

    // 白黒カラーまで対応
    NN_SDK_ASSERT(
        (colorType >= AnimTargetMatColorFloat_BufferR) && (colorType <= AnimTargetMatColorFloat_Konst0A),
        "material[%s]", GetName());

    // AnimTargetMatColorFloat の値域から AnimTargetMatColor の値域へ変換する。
    colorType -= AnimTargetMatColor_MaxAnimTargetMatColor;

    const uint32_t idx = colorType / 4;
    if (idx < MatColorMax)
    {
        // byte データに対しての呼び出しはアサートする。
        NN_SDK_ASSERT(IsInterpolateColorFloat(static_cast<InterpolateColor>(idx)),
            "Invalid function call for byte-color! Use GetColorElement() instead : idx[%u] for material[%s]", idx, GetName());

        switch (colorType % 4)
        {
        case 0: return m_Colors.pFloatColor[idx].v[0];
        case 1: return m_Colors.pFloatColor[idx].v[1];
        case 2: return m_Colors.pFloatColor[idx].v[2];
        case 3: return m_Colors.pFloatColor[idx].v[3];
        default: NN_SDK_ASSERT(false); return 0;
        }
    }

    return std::numeric_limits<float>::max();
}


//----------------------------------------
inline void
Material::SetColorElement(int  colorType, int  value)
{
    if (colorType < AnimTargetMatColor_MaxAnimTargetMatColor)
    {
        NN_STATIC_ASSERT(AnimTargetMatColor_MaxAnimTargetMatColor % 4 == 0);
        NN_STATIC_ASSERT(AnimTargetMatColor_MaxAnimTargetMatColor / 4 == MatColorMax + DetailedCombinerConstantColor);
        NN_STATIC_ASSERT(AnimTargetMatColor_BufferR + 4 == AnimTargetMatColor_Konst0R);
        NN_STATIC_ASSERT(AnimTargetMatColor_BufferR + 4 * MatColorMax == AnimTargetMatColor_C0_R);
        NN_STATIC_ASSERT(DetailedCombinerConstantColor == CombinerUserShaderConstantColor);

        uint32_t idx = colorType / 4;
        if (idx < MatColorMax)
        {
            // float データに対しての呼び出しはアサートする。
            NN_SDK_ASSERT(!IsInterpolateColorFloat(static_cast<InterpolateColor>(idx)),
                "Invalid function call for float-color! Use GetColorElementFloat() instead : idx[%u] for material[%s]", idx, GetName());

            switch (colorType % 4)
            {
            case 0: m_Colors.byteColor[idx].v[0] = static_cast<uint8_t>(value); break;
            case 1: m_Colors.byteColor[idx].v[1] = static_cast<uint8_t>(value); break;
            case 2: m_Colors.byteColor[idx].v[2] = static_cast<uint8_t>(value); break;
            case 3: m_Colors.byteColor[idx].v[3] = static_cast<uint8_t>(value); break;
            default: NN_SDK_ASSERT(false); break;
            }
        }
        else if (idx < MatColorMax + DetailedCombinerConstantColor)
        {
            if (UseDetailedCombinerCap())
            {
                ResDetailedCombinerStageInfo* pInfo = GetDetailedCombinerStageInfoPtr();
                idx -= MatColorMax;
                ResColor color = pInfo->GetConstantColor(idx);
                switch (colorType % 4)
                {
                case 0: color.r = static_cast<uint8_t>(value); break;
                case 1: color.g = static_cast<uint8_t>(value); break;
                case 2: color.b = static_cast<uint8_t>(value); break;
                case 3: color.a = static_cast<uint8_t>(value); break;
                default: NN_SDK_ASSERT(false); break;
                }
                pInfo->SetConstantColor(idx, color);
            }
            else if (UseCombinerUserShaderCap())
            {
                ResCombinerUserShader* pInfo = GetCombinerUserShaderPtr();
                idx -= MatColorMax;
                ResColor color = pInfo->GetConstantColor(idx);
                switch (colorType % 4)
                {
                case 0: color.r = static_cast<uint8_t>(value); break;
                case 1: color.g = static_cast<uint8_t>(value); break;
                case 2: color.b = static_cast<uint8_t>(value); break;
                case 3: color.a = static_cast<uint8_t>(value); break;
                default: NN_SDK_ASSERT(false); break;
                }
                pInfo->SetConstantColor(idx, color);
            }
        }
    }
}

//----------------------------------------
inline void
Material::SetColorElementFloat(int  colorType, float value)
{
    NN_STATIC_ASSERT(AnimTargetMatColorFloat_MaxAnimTargetMatColor % 4 == 0);
    NN_STATIC_ASSERT((AnimTargetMatColorFloat_MaxAnimTargetMatColor - AnimTargetMatColor_MaxAnimTargetMatColor) / 4
                        == MatColorMax + DetailedCombinerConstantColor);
    NN_STATIC_ASSERT(AnimTargetMatColorFloat_BufferR + 4 == AnimTargetMatColorFloat_Konst0R);
    NN_STATIC_ASSERT(AnimTargetMatColorFloat_BufferR + 4 * MatColorMax == AnimTargetMatColorFloat_C0_R);

    // 白黒カラーまで対応
    NN_SDK_ASSERT(
        (colorType >= AnimTargetMatColorFloat_BufferR) && (colorType <= AnimTargetMatColorFloat_Konst0A),
        "material[%s]", GetName());

    // AnimTargetMatColorFloat の値域から AnimTargetMatColor の値域へ変換する。
    colorType -= AnimTargetMatColor_MaxAnimTargetMatColor;

    uint32_t idx = colorType / 4;
    if (idx < MatColorMax)
    {
        // byte データに対しての呼び出しはアサートする。
        NN_SDK_ASSERT(IsInterpolateColorFloat(static_cast<InterpolateColor>(idx)),
            "Invalid function call for byte-color! Use GetColorElement() instead : idx[%u] for material[%s]", idx, GetName());

        switch (colorType % 4)
        {
        case 0: m_Colors.pFloatColor[idx].v[0] = value; break;
        case 1: m_Colors.pFloatColor[idx].v[1] = value; break;
        case 2: m_Colors.pFloatColor[idx].v[2] = value; break;
        case 3: m_Colors.pFloatColor[idx].v[3] = value; break;
        default: NN_SDK_ASSERT(false); break;
        }
    }
}

//----------------------------------------
inline void
Material::SetColorElementAuto(int  colorType, float value)
{
    // float データ用のカーブは AnimTargetMatColor の最大数より大きな値でAnimTargetMatColorFloat として定義されている。
    const bool byteData = (colorType <= AnimTargetMatColor_MaxAnimTargetMatColor);
    if (byteData)
    {
        value += 0.5f;  // 四捨五入のため
        uint8_t  val = static_cast<uint8_t>(std::min(std::max(value, 0.f), 255.f));
        SetColorElement(colorType, val);
    }
    else
    {
        SetColorElementFloat(colorType, value);
    }
}

} // namespace nn::ui2d
} // namespace nn
