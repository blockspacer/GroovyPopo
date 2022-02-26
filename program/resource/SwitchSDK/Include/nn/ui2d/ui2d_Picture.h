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

#include <nn/util/util_MathTypes.h>

#include <nn/ui2d/ui2d_Util.h>
#include <nn/ui2d/ui2d_TexMap.h>
#include <nn/ui2d/ui2d_Pane.h>
#include <nn/gfx/util/gfx_PrimitiveShape.h>

namespace nn
{
namespace ui2d
{

struct ResPicture;
class DrawInfo;

//---------------------------------------------------------------------------
//! @brief 画像の表示を行うペインです。
//!
//---------------------------------------------------------------------------
class Picture : public Pane
{
    typedef Pane Base;

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(Picture::Base);

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 指定したテクスチャの枚数分のメモリを確保して、オブジェクトを生成します。
    //!
    //! @param[in] texCount   使用するテクスチャの最大数です。
    //!
    explicit Picture(int  texCount);

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 指定したテクスチャを使用するオブジェクトを生成します。
    //! 保持できるテクスチャ座標の数は 1 に設定されます。
    //!
    //! @param[in] textureInfo  TextureInfo への参照です。
    //!
    explicit Picture(const TextureInfo& textureInfo);

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 指定したリソースからオブジェクトを生成します。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報です。
    //! @param[in] pDevice          デバイスです。
    //! @param[in] pBaseBlock       このペインが所属するレイアウトが保持しているリソースへのポインタです。
    //! @param[in] pOverrideBlock   上書きを行ったリソースへのポインタです。上書きが行われていないときはNULLになります。
    //! @param[in] buildArgSet      構築時の引数への参照です。
    //!
    Picture(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        const ResPicture* pBaseBlock,
        const ResPicture* pOverrideBlock,
        const BuildArgSet& buildArgSet);

    //! @brief コピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Picture::Picture(const Picture& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    //! @param[in] picture  コピー元のペインです。
    //! @param[in] pDevice          デバイスです。
    //!
    explicit Picture(const Picture& picture, nn::gfx::Device* pDevice)
        : Pane(picture)
        , m_pMaterial(NULL)
    {
        CopyImpl(picture, pDevice, NULL, NULL);
    }

    //! @brief コピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Picture::Picture(const Picture& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    //! @param[in] picture  コピー元のペインです。
    //! @param[in] pDevice          デバイスです。
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //!
    //! @deprecated ui2d ランタイムで動的レンダリングテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
    //!             動的レンダリングテクスチャ関連機能を使用したペインをコピーする場合は Picture::Picture(const Picture& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    NN_DEPRECATED explicit Picture(const Picture& picture, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pNewRootName)
        : Pane(picture)
        , m_pMaterial(NULL)
    {
        NN_UNUSED(pResAccessor);
        NN_UNUSED(pNewRootName);
        CopyImpl(picture, pDevice, NULL, NULL);
    }

    //! @brief コピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Picture::Picture(const Picture& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    //! @param[in] picture  コピー元のペインです。
    //! @param[in] pDevice          デバイスです。
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //! @param[in] pLayout このペインを管理しているレイアウトです。
    //!
    //! @deprecated ui2d ランタイムで動的レンダリングテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
    //!             動的レンダリングテクスチャ関連機能を使用したペインをコピーする場合は Picture::Picture(const Picture& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    NN_DEPRECATED explicit Picture(const Picture& picture, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pNewRootName, const Layout* pLayout)
        : Pane(picture)
        , m_pMaterial(NULL)
    {
        NN_UNUSED(pResAccessor);
        NN_UNUSED(pNewRootName);
        CopyImpl(picture, pDevice, pLayout, NULL);
    }

    //! @brief コピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
    //!
    //! また、アニメーションについても、何もバインドされていない状態になります。
    //!
    //! キャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャなどの動的レンダリングテクスチャを使用している場合は
    //! このコピーコンストラクタを使用してコピーしてください。
    //! pLayout で指定したレイアウトクラスへ動的レンダリングテクスチャのリソースが登録されます。
    //!
    //! @param[in] picture  コピー元のペインです。
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pLayout 動的レンダリングテクスチャリソースを登録するレイアウトインスタンス。通常、ペインの所属するレイアウトインスタンスを指定します。
    //!
    Picture(const Picture& picture, nn::gfx::Device* pDevice, Layout* pLayout);

    // ClonePaneTree から呼び出される内部用のコピーコンストラクタです。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
    //!
    //! このコピーコンストラクタでは渡された引数をもとに新たなキャプチャテクスチャを参照するようになります。
    //!
    //! @param[in] picture  コピー元のペインです。
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pLayout このペインを管理しているレイアウトです。
    //! @param[in] pBuildPaneTreeContext 動的レンダリングテクスチャの共有情報や初期化情報などランタイム内部で使用する情報をペインツリーをまたいで受け渡しするためのデータです。
    //!
    Picture(const Picture& picture, nn::gfx::Device* pDevice, const Layout* pLayout, detail::BuildPaneTreeContext* pBuildPaneTreeContext)
        : Pane(picture, pDevice, pLayout, pBuildPaneTreeContext)
        , m_pMaterial(NULL)
    {
        CopyImpl(picture, pDevice, pLayout, pBuildPaneTreeContext);
    }

    //! @brief デストラクタです。
    //!
    virtual ~Picture();

    //! @brief リソースを開放します。
    //!
    //! @details
    //! デストラクタの呼び出し以前に呼び出す必要があります。
    //!
    //! @param[in] pDevice          デバイスです。
    //!
    virtual void Finalize(nn::gfx::Device* pDevice) NN_OVERRIDE;

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief テクスチャを追加します。
    //!
    //! @param[in] textureInfo  TextureInfo への参照です。
    //!
    virtual void Append(const TextureInfo& textureInfo);

    //! @brief 頂点カラーを取得します。
    //!
    //! @details
    //! idx で指定した頂点カラーを取得します。
    //!
    //! @param[in] idx  インデックスです。 nn::ui2d::VertexColor を指定します。
    //!
    //! @return カラーを返します。
    //!
    //! @sa SetVertexColor
    //! @sa GetVertexColorElement
    //! @sa nn::ui2d::VertexColor
    //!
    virtual const nn::util::Unorm8x4 GetVertexColor(int  idx) const NN_OVERRIDE;

    //! @brief 頂点カラーを設定します。
    //!
    //! @param[in] idx      インデックスです。 nn::ui2d::VertexColor を指定します。
    //! @param[in] value    頂点カラーです。
    //!
    //! @sa GetVertexColor
    //! @sa SetVertexColorElement
    //! @sa nn::ui2d::VertexColor
    //!
    virtual void SetVertexColor(int  idx, const nn::util::Unorm8x4& value) NN_OVERRIDE;

    //! @brief 頂点カラーの成分を取得します。
    //!
    //! @details
    //! idx で指定した頂点カラーの成分を取得します。
    //!
    //! idx の指定には nn::ui2d::AnimTargetPaneColor を使用してください。
    //!
    //! このメソッドは、R,G,B など色成分を個別に取得します。色成分をまとめて取得する場合は GetVertexColor を利用してください。
    //!
    //! @param[in] idx  インデックスです。
    //!
    //! @return 頂点カラーの成分を返します。
    //!
    //! @sa SetVertexColorElement
    //! @sa GetVertexColor
    //! @sa nn::ui2d::AnimTargetPaneColor
    //!
    virtual uint8_t  GetVertexColorElement(int  idx) const NN_OVERRIDE;

    //! @brief 頂点カラーの成分を設定します。
    //!
    //! idx の指定には nn::ui2d::AnimTargetPaneColor を使用してください。
    //!
    //! このメソッドは、R,G,B など色成分を個別に設定します。色成分をまとめて設定する場合は SetVertexColor を利用してください。
    //!
    //! @param[in] idx      インデックスです。 nn::ui2d::AnimTargetPaneColor を指定します。
    //! @param[in] value    カラーの成分値です。
    //!
    //! @sa GetVertexColorElement
    //! @sa SetVertexColor
    //! @sa nn::ui2d::AnimTargetPaneColor
    //!
    virtual void SetVertexColorElement(int  idx, uint8_t  value) NN_OVERRIDE;

    //! @brief テクスチャ座標を保持するためのメモリ領域を確保します。
    //!
    //! @details
    //! 保持可能なテクスチャ座標の数の初期値はコンストラクタによって異なります。
    //!
    //! @param[in] count  テクスチャ座標の保持数です。
    //!
    //! @sa Picture::Picture
    //!
    void ReserveTexCoord(int  count);

    //! @brief テクスチャ座標の保持数を取得します。
    //!
    //! @return 保持しているテクスチャ座標の数を返します。
    //!
    //! @sa SetTexCoordCount
    //!
    int  GetTexCoordCount() const;

    //! @brief テクスチャ座標の保持数を設定します。
    //!
    //! @details
    //! count に指定できる値は保持可能なテクスチャ座標数以下でなければなりません。
    //!
    //! @param[in] count  テクスチャ座標の保持数です。
    //!
    //! @sa GetTexCoordCount
    //! @sa ReserveTexCoord
    //! @sa Picture::Picture
    //! @sa GetTexCoord
    //!
    void SetTexCoordCount(int  count);

    //! @brief テクスチャ座標を取得します。
    //!
    //! @param[out] coords  テクスチャ座標を格納する領域へのポインタです。
    //! @param[in] idx      インデックスです。
    //!
    //! @sa SetTexCoord
    //!
    void GetTexCoord(TexCoordQuad coords, int  idx) const;

    //! @brief テクスチャ座標を設定します。
    //!
    //! @param[in] idx  インデックスです。
    //! @param[in] coords   テクスチャ座標です。
    //!
    //! @sa GetTexCoord
    //!
    void SetTexCoord(int  idx, const TexCoordQuad coords);

    using Base::GetMaterial;

    //! @brief ペインが持つマテリアルの数を取得します。
    //!
    //! @return マテリアルを持っていれば 1 を返します。
    //!
    virtual uint8_t  GetMaterialCount() const NN_OVERRIDE;

    //! @brief ペインのマテリアルを取得します。
    //!
    //! @details
    //! idx には 0 のみ指定できます。
    //!
    //! @param[in] idx  インデックスです。
    //!
    //! @return マテリアルへのポインタを返します。
    //!
    //! @sa SetMaterial
    //!
    virtual Material* GetMaterial(int  idx) const NN_OVERRIDE;

    //! @brief マテリアルを設定します。
    //!
    //! @details
    //! 現在設定されているマテリアルが Picture オブジェクトの生成時に
    //! 同時に生成されたものだった場合には、そのマテリアルは破棄されます。
    //!
    //! @param[in] pMaterial    マテリアルへのポインタです。
    //!
    void SetMaterial(Material* pMaterial);

    //@}

    virtual void Calculate(DrawInfo& drawInfo, Pane::CalculateContext& context, bool isDirtyParentMtx) NN_OVERRIDE;

    //! @brief  ペインエフェクトのキャプチャ結果を最終的に描画する際に描画状態を設定します。
    //!
    //! @details
    //! ペインエフェクトを使用した際は最終的にキャプチャされたペインのテクスチャが画面にレンダリングされます。
    //! このメソッドではその描画の際の描画設定を行います。
    //!
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    virtual void SetupPaneEffectSourceImageRenderState(nn::gfx::CommandBuffer& commandBuffer) const NN_OVERRIDE;

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
    bool CompareCopiedInstanceTest(const Picture& target) const;
protected:
    virtual void DrawSelf(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer) NN_OVERRIDE;

    //! @brief モデルビュー行列を計算し DrawInfo に設定します。
    //!
    //! @param[in] drawInfo 描画情報です。
    //!
    virtual void LoadMtx(DrawInfo& drawInfo) NN_OVERRIDE;


    //! @brief コピーコンストラクタの実装です。
    //!
    //! @param[in] picture コピー元のピクチャペインです。
    //! @param[in] pDevice デバイスです。
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //! @param[in] pLayout コピーされたレイアウトへのポインタです。
    //!
    //! @deprecated ui2d ランタイムでキャプチャテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
    //!
    NN_DEPRECATED void CopyImpl(const Picture& picture, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pNewRootName, const Layout* pLayout);

    //! @brief コピーコンストラクタの実装です。
    //!
    //! @param[in] picture コピー元のピクチャペインです。
    //! @param[in] pDevice デバイスです。
    //! @param[in] pLayout コピーされたレイアウトへのポインタです。
    //! @param[in] pBuildPaneTreeContext ペイン間のキャプチャテクスチャの共有など、ペインやパーツ階層をまたいで受け渡す必要のあるデータを保持します。
    //!
    void CopyImpl(const Picture& picture, nn::gfx::Device* pDevice, const Layout* pLayout, detail::BuildPaneTreeContext* pBuildPaneTreeContext);
private:
    struct ShapeDrawInfo
    {
        uint16_t                        m_ShapeType;
        uint8_t                         padding[2];
        size_t                          vertexBufferGpuMemoryOffset;
        size_t                          indexBufferGpuMemoryOffset;
        nn::gfx::util::PrimitiveShape*  pShape;
    };

    // SharedMemory の union サイズを TexCoordArray と同等に抑えるため
    // 判別フラグとシェイプ情報へのポインタを持つ構造体を一段かませる。
    struct ShapeDrawInfoAccessor
    {
        uint8_t                         unionTypeFlag;
        uint8_t                         padding;
        ShapeDrawInfo*                  pShapeDrawInfo;
    };

    // 角丸の角一つ分の設定パラメータです。
    struct ConstantBufferCornerParams
    {
        float   exp;
        float   size;
        float   offset[2];
    };

    // 角丸機能のコンスタントバッファを表す構造体です。
    struct ProceduralShapeConstantBuffer
    {
        // 基本
        float   invHalfPaneWidth;
        float   invHalfPaneHeight;
        float   sizePadding[2];

        // 各角のパラメータ
        ConstantBufferCornerParams  cornerParams[ProceduralShapeSeparateParamType_Max];

        // ブレンド設定
        int     innerStrokeBlendType;
        int     innerShadowBlendType;
        int     colorOverlayBlendType;
        int     gradationOverlayBlendType;

        // 境界線
        float   innerStrokeSize;
        float   innerStrokeInvAntiAliasRatioInStrokeArea;
        float   innerStrokePadding[2];
        float   innerStrokeColor[4];

        // シャドウ
        float   shadowSize;
        float   shadowOffsetX;
        float   shadowOffsetY;
        float   shadowSelect;
        float   shadowColor[4];
        float   shadowAttenuation[4];

        // カラーオーバーレイ
        float   colorOverlayColor[4];

        // グラデーションオーバーレイ
        float   gradationOverlayControlPoints[4];
        float   gradationOverlayColors[4][4];
        float   gradationOverlayOriginX;
        float   gradationOverlayOriginY;
        float   gradationOVerlayPadding[2];
    };

    // GfxPrimitive を描画する際の頂点フォーマット
    static const nn::gfx::util::PrimitiveShapeFormat VertexFormat_PUv = static_cast< nn::gfx::util::PrimitiveShapeFormat >(nn::gfx::util::PrimitiveShapeFormat_Pos | nn::gfx::util::PrimitiveShapeFormat_Uv);

    void Initialize(int  texCount);
    void InitializeMaterial(int texCount);

    // シェイプ情報の初期化処理
    void InitializeShape(
        BuildResultInformation* pOutBuildResultInformation,
        nn::gfx::Device* pDevice,
        const ResPicture* pBlock,
        const BuildArgSet& buildArgSet,
        uint32_t    shapeBinaryIndex);
    void CopyShapeInfo(const Picture& picture);

    // 角丸機能の初期化処理
    void InitializeProceduralShape(BuildResultInformation* pOutBuildResultInformation, nn::gfx::Device* pDevice);
    // 角丸機能のコンスタントバッファ更新処理
    void CalculateProceduralShape(DrawInfo& drawInfo);
    // 角丸機能のコンスタントバッファ更新処理
    float CalculateProceduralShapeBasicParams(ProceduralShapeConstantBuffer* pConstantBuffer, const SystemDataProceduralShape* pData);
    // 角丸機能のシャドウコンスタントバッファ更新処理
    void CalculateProceduralShapeShadow(
            ProceduralShapeConstantBuffer* pConstantBuffer,
            float radius,
            bool enabled,
            bool dropShadowKnockoutEnabled,
            float shadowSize,
            int blendType,
            float angle,
            float distance,
            const float color[],
            int type,
            bool dropShadowEnabled);
    // 角丸機能のドロップシャドウコンスタントバッファ更新処理
    void CalculateProceduralShapeDropShadow(DrawInfo& drawInfo, const SystemDataProceduralShape* pProceduralShapeData, SystemDataProceduralShapeRuntimeInfo* pProceduralShapeInfo);
    // 角丸機能のドロップシャドウを描画
    void DrawProceduralShapeDropShadow(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer, const SystemDataProceduralShape* pProceduralShapeData, SystemDataProceduralShapeRuntimeInfo* pProceduralShapeInfo);

    // 角丸機能のグラデーションオーバーレイコンスタントバッファ更新処理
    void CalculateProceduralShapeGradationOverlay(ProceduralShapeConstantBuffer* pConstantBuffer, const SystemDataProceduralShape* pData);
    // ProceduralShapeEffectBlendMode からプリセットのブレンド ID に変換します。
    PresetBlendStateId ConvertProceduralShapeEffectBlendType(detail::ProceduralShapeEffectBlendMode mode) const;

    // シェイプで使用する各種バッファのコピー処理
    void CopyShapeVertexData(DrawInfo& drawInfo);

    // ResPicture のフラグ判定メソッド
    bool CheckAdditionalInfoFlag(const ResPicture* pBlock, PicturePaneFlag flag ) const
    {
        return (pBlock->flags & (1 << flag)) != 0;
    }

    // TexCoordArray が有効かどうか判別するメソッド
    bool IsTexCoordArrayEnabled() const
    {
        // m_SharedMemory.m_UnionTypeFlag は m_TexCoordArray.m_Cap とメモリを共有しており
        // m_TexCoordArray が使用されている状態だと TexMapMax より小さな値が設定されています。
        return m_SharedMemory.m_UnionTypeFlag <= TexMapMax;
    }

    Material* m_pMaterial;

    nn::util::Unorm8x4 m_VertexColors[VertexColor_MaxVertexColor];

    union SharedMemory
    {
        enum
        {
            UnionType_ShapeDrawInfo = TexMapMax + 1,    // TexMapMax 以上をメモリが共用された特殊用途に使用する。
            UnionType_Max
        };

        uint8_t                 m_UnionTypeFlag;
        detail::TexCoordArray   m_TexCoordArray;
        ShapeDrawInfoAccessor   m_ShapeDrawInfoAccessor;

        // 共用体のメンバーが non-trivial なコンストラクター、デストラクターを持っていると
        // 対応する SharedMemory の定義が暗黙的に削除されるため、明示的に定義する必要がある。
        SharedMemory() { memset(this, 0, sizeof(SharedMemory)); }
        ~SharedMemory() {}
    };

    SharedMemory    m_SharedMemory;

    // コピー演算子を禁止します。
    const Picture& operator=( const Picture& );
};

} // namespace nn::ui2d
} // namespace nn
