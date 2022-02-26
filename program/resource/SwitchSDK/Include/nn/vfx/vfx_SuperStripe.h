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

#include <nn/vfx/vfx_StripeCommon.h>
#include <nn/vfx/vfx_TemporaryBuffer.h>

namespace nn {
namespace vfx {
namespace detail {

//------------------------------------------------------------------------------
//! @brief  ストライプ履歴。パーティクルの移動の記録。
//------------------------------------------------------------------------------
struct SuperStripeHistory
{
public:
    typedef nn::util::Vector3fType PosVectorType;       //!< 位置ベクトルの型
    typedef nn::util::Vector3fType DirVectorType;       //!< 方向ベクトルの型
public:
    nn::util::Vector3fType  pos;                        //!< エミッタ（or ワールド）座標系での座標
    nn::util::Vector3fType  vec;                        //!< 履歴点の速度
    nn::util::Vector3fType  dir;                        //!< 背骨の方向（履歴点の速度ではない）
    nn::util::Vector3fType  wing;                       //!< ポリゴンを展開する方向.xyz
    nn::util::Vector3fType  emat;                       //!< その時のエミッタ行列のY軸成分
    float                   scale;                      //!< スケール
    SuperStripeHistory*     pNext;                      //!< 次の履歴点へのポインタ
    SuperStripeHistory*     pPrev;                      //!< 前の履歴点へのポインタ
};

//------------------------------------------------------------------------------
//! @brief  ストライプ
//------------------------------------------------------------------------------
struct SuperStripeInstance
{
    nn::util::Vector4fType  color0;                     //!< カラー0
    nn::util::Vector4fType  color1;                     //!< カラー1
    nn::util::Vector4fType  random;                     //!< ランダム
    nn::util::Vector3fType  vInterpolate;               //!< 補間ベクトル
    nn::util::Vector3fType  headPos;                    //!< 先端の位置
    SuperStripeHistory*     historyArray;               //!< ヒストリ
    SuperStripeHistory*     pHistoryHead;               //!< 描画対象のヒストリ先頭
    SuperStripeHistory*     pHistoryTail;               //!< 描画対象のヒストリ最後尾
    SuperStripeInstance*    pNextDelayedStripe;         //!< 遅延終了処理用のリスト
    int                     historyCount;               //!< ヒストリが何個分あるか？
    int                     delayedHistoryCount;        //!< 遅延ストライプに入った時のストライプ数
    int                     renderVertexCount;          //!< 頂点バッファに書き込んだ数
    int                     indexOffset;                //!< 頂点バッファ上の位置
    float                   time;                       //!< 生成時間
    float                   life;                       //!< 寿命
    float                   prevFrame;                  //!< 前回のフレーム値
    float                   journey;                    //!< ストライプが飛んだ距離
    bool                    used;                       //!< 利用中かどうか
};

//------------------------------------------------------------------------------
//! @brief  ストライプを管理するクラスです。
//------------------------------------------------------------------------------
class SuperStripeSystem
{
public:
    typedef      ResStripeSuper         ResourceType;                   //!< 使用するバイナリリソース構造体
    static const EmitterPluginId        PluginId = EmitterPluginId_3;   //!< エミッタプラグインID
    typedef      SuperStripeHistory     History;                        //!< template で使用する名前に貼り替え
    typedef      SuperStripeInstance    StripeInstance;                 //!< template で使用する名前に貼り替え
private:
    //------------------------------------------------------------------------------
    //! @brief  テクスチャマッピング方式
    //------------------------------------------------------------------------------
    enum TextureMappingType
    {
        TextureMappingType_Uniform       = 0,               //!< 均等割り当て
        TextureMappingType_DistanceBased = 1,               //!< 距離依存
    };

    //---------------------------------------------------------------------------
    //! @brief  ストライプ頂点バッファ
    //---------------------------------------------------------------------------
    struct VertexAttribute
    {
        nn::util::Float4        pos;                        //!< ( 履歴の位置.xyz / 羽の符号付長さ )
        nn::util::Float4        dir;                        //!< ( 背骨の方向.xyz / 頂点Index )
        nn::util::Float4        wing;                       //!< ( 羽の方向.xyz / ... )
        nn::util::Float4        tex;                        //!< ( テクスチャリング.012 / ... )
        nn::util::Float4        emat;                       //!< ( matv.xyz / ... )
    };

    //------------------------------------------------------------------------------
    //! @brief  ConstantBuffer 構造体
    //------------------------------------------------------------------------------
    struct ConstantBufferObject
    {
        nn::util::Float4        random;                     //!< ストライプ単位のランダム4つ
        nn::util::Float4        param0;                     //!< 先端α / 末端α / 先端スケール / 末端スケール
        nn::util::Float4        color0;                     //!< カラー0
        nn::util::Float4        color1;                     //!< カラー1
        nn::util::Float4        param1;                     //!< ストライプ時間 / 履歴点総数 / CrossMode / ストライプ寿命
        nn::util::Float4        param2;                     //!< ストライプ先端座標(xyz) / ...
    };

    //------------------------------------------------------------------------------
    //! @brief  エミッタ単位で記憶する必要があるデータ
    //------------------------------------------------------------------------------
    struct EmitterPluginUserData
    {
        nn::util::Float4        staticParam0;               //!< ストタイプエミッタの固有パラメータ
        bool                    vertexBufferInitialized;    //!< 頂点バッファが初期化済みか
        StripeInstance*         pDelayedStripeListHead;     //!< 遅延ストライプリストの先頭
        int                     vertexCountPerStripe;       //!< ストライプ一本当たりの頂点数
        int                     vertexBufferCount;          //!< 頂点バッファの個数
        int                     maxStripeCount;             //!< 頂点アトリビュートの本数
        int                     attributeIndex;             //!< 最後に割り当てたアトリビュートインデックス
        int                     maxEmitRatio;               //!< エミッタ時間アニメーションも考慮した最大放出レート
        int                     maxParticleLife;            //!< エミッタ時間アニメーションも考慮した最大パーティクル寿命
        BufferSide              bufferSide;                 //!< 現在のバッファサイド（Emitterとは独立で持つ）
    };

public:
    //------------------------------------------------------------------------------
    //! @brief                  ストライプシステムの初期化処理を行います。
    //! @param[in] pHeap        ヒープへのポインタ
    //! @param[in] pSystem      System へのポインタ
    //! @param[in] bufferMode   バッファリングモード
    //! @param[in] stripeCount  ストライプの最大数
    //------------------------------------------------------------------------------
    static void InitializeSystem( nn::vfx::Heap* pHeap, nn::vfx::System* pSystem, BufferingMode bufferMode, int stripeCount ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief                  ストライプシステムの終了処理を行います。
    //! @param[in] pHeap        ヒープへのポインタ
    //------------------------------------------------------------------------------
    static void FinalizeSystem( nn::vfx::Heap* pHeap ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief  ストライプシステムで確保したワークサイズを取得します。
    //! @return 確保したワークサイズ
    //------------------------------------------------------------------------------
    static size_t GetWorkSize() NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief      ワンタイムエミッタ用の追加のエミッタ寿命を取得
    //! @param[in]  pEmitter    エミッタへのポインタ
    //! @return                 ワンタイムエミッタ用の追加のエミッタ寿命
    //------------------------------------------------------------------------------
    static int GetExtendedEndTimeForOneTimeEmitter( nn::vfx::Emitter* pEmitter ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief      ストライプの正確な再生本数を取得
    //! @param[in]  pEmitter    エミッタへのポインタ
    //! @return                 再生終了待ちのストライプを考慮した正確な再生本数
    //------------------------------------------------------------------------------
    static int GetActualSuperStripeCalclationCount( const nn::vfx::Emitter* pEmitter ) NN_NOEXCEPT;

    //----------------------------------------
    //! @name エミッタコールバック
    //@{

    //------------------------------------------------------------------------------
    //! @brief          エミッタ生成後コールバック
    //! @param[in] arg  コールバック引数
    //! @return         成功すればtrue
    //------------------------------------------------------------------------------
    static bool InitializeStripeEmitter( EmitterInitializeArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          パーティクル生成コールバック
    //! @param[in] arg  コールバック引数
    //! @return         成功すればtrue
    //------------------------------------------------------------------------------
    static bool EmitStripe( ParticleEmitArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          パーティクル削除コールバック
    //! @param[in] arg  コールバック引数
    //! @return         成功すればtrue
    //------------------------------------------------------------------------------
    static bool KillStripe( ParticleRemoveArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          パーティクル計算コールバック
    //! @param[in] arg  コールバック引数
    //------------------------------------------------------------------------------
    static void ParticleCalculateCallback( ParticleCalculateArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          エミッタ計算処理前コールバック
    //! @param[in] arg  コールバック引数
    //------------------------------------------------------------------------------
    static void EmitterPreCalculateCallback( EmitterPreCalculateArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          エミッタ計算処理後コールバック
    //! @param[in] arg  コールバック引数
    //------------------------------------------------------------------------------
    static void EmitterPostCalculateCallback( EmitterPostCalculateArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          エミッタ描画コールバック
    //! @param[in] arg  コールバック引数
    //! @return         成功すればtrue
    //------------------------------------------------------------------------------
    static bool EmitterDrawCallback( EmitterDrawArg& arg ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief          エミッタ破棄後コールバック
    //! @param[in] arg  コールバック引数
    //------------------------------------------------------------------------------
    static void FinalizeStripeEmitter( EmitterFinalizeArg& arg ) NN_NOEXCEPT;

    //@}

private:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief                  ストライプシステムの生成を行います。
    //! @param[in] pHeap        ヒープへのポインタ
    //! @param[in] pSystem      Systemへのポインタ
    //! @param[in] bufferMode   バッファリングモード
    //! @param[in] stripeCount  ストライプ最大数
    //---------------------------------------------------------------------------
    SuperStripeSystem( nn::vfx::Heap* pHeap, nn::vfx::System* pSystem, BufferingMode bufferMode, int stripeCount ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  デフォルトコンストラクタ（Private宣言のみ）
    //---------------------------------------------------------------------------
    SuperStripeSystem() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ストライプシステムの破棄を行います。
    //---------------------------------------------------------------------------
    virtual ~SuperStripeSystem() NN_NOEXCEPT;

    //@}

    //---------------------------------------------------------------------------
    //! @brief                      ストライプの履歴を更新します。
    //! @param[in] arg              コールバック引数
    //! @param[in] pStripe          ストライプへのポインタ
    //! @param[in] pStripeData      ストライプ設定へのポインタ
    //! @param[in] doPushHistory    履歴を積むかどうか
    //---------------------------------------------------------------------------
    void UpdateHistory(
        ParticleCalculateArg& arg,
        StripeInstance* pStripe,
        ResStripeSuper* pStripeData, bool doPushHistory ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ポリゴン用ワークを確保します。
    //! @param[in] pEmitter エミッタへのポインタ
    //! @return             true で確保成功、 false で確保失敗
    //---------------------------------------------------------------------------
    bool AllocStripeSystemVertexBuffer( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  遅延解放ストライプ計算処理を行います。
    //! @param[in] pEmitter     エミッタへのポインタ
    //---------------------------------------------------------------------------
    void CalculateDelayedStripe( Emitter* pEmitter ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief              ストライプ計算処理を行います。
    //! @param[in] arg      コールバック引数
    //! @param[in] pStripe  ストライプへのポインタ
    //---------------------------------------------------------------------------
    void CalculateStripe( ParticleCalculateArg& arg, StripeInstance* pStripe ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  現在のストライプ数取得を行います。
    //! @return 現在のストライプ数
    //---------------------------------------------------------------------------
    static int GetProcessingStripeCount() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          ストライプのポリゴン生成を行います。
    //! @param[out]     pOutVertexAttribute 頂点バッファへのポインタ
    //! @param[in,out]  pStripe             ストライプへのポインタ
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      pStripeData         ストライプ設定へのポインタ
    //! @param[in]      indexOffset         テクスチャオフセット値
    //---------------------------------------------------------------------------
    void MakeStripePolygon(
        VertexAttribute* pOutVertexAttribute,
        StripeInstance* pStripe,
        const Emitter* pEmitter,
        const ResStripeSuper* pStripeData,
        float indexOffset = 0.0f ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief          ストライプのポリゴン生成を行います。（ストライプ分割版）
    //! @param[out]     pOutVertexAttribute 頂点バッファへのポインタ
    //! @param[in,out]  pStripe             ストライプへのポインタ
    //! @param[in]      pEmitter            エミッタへのポインタ
    //! @param[in]      pStripeData         ストライプ設定へのポインタ
    //! @param[in]      indexOffset         テクスチャオフセット値
    //---------------------------------------------------------------------------
    void MakeStripePolygonWithDivision(
        VertexAttribute* pOutVertexAttribute,
        StripeInstance* pStripe,
        const Emitter* pEmitter,
        const ResStripeSuper* pStripeData,
        float indexOffset = 0.0f ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                      テクスチャオフセットVを計算します
    //! @param[out] pOutTexOffset   出力テクスチャ座標（テクスチャ 0/1/2）
    //! @param[in]  pStripeData     ストライプ設定
    //! @param[in]  index           インデックス
    //! @param[in]  vertexCount     頂点数
    //! @param[in]  maxVertexCount  想定する最大の頂点数
    //! @param[in]  indexOffset     オフセット値
    //---------------------------------------------------------------------------
    void CalculateTextureOffsetUniform(
        nn::util::Vector3fType* pOutTexOffset,
        const ResStripeSuper* pStripeData,
        int index,
        int vertexCount,
        int maxVertexCount,
        float indexOffset
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                      テクスチャオフセットVを計算します
    //! @param[out] pOutTexOffset   出力テクスチャ座標（テクスチャ 0/1/2）
    //! @param[in]  pStripeData     ストライプ設定
    //! @param[in]  index           インデックス
    //! @param[in]  delta           履歴点の移動差分
    //! @param[in]  journey         ストライプのこれまでの移動距離
    //! @param[in]  invStripeLength ストライプ長の逆数
    //! @param[in]  prevTex         一つ前のテクスチャオフセット
    //---------------------------------------------------------------------------
    void CalculateTextureOffsetDistanceBased(
        nn::util::Vector3fType* pOutTexOffset,
        const ResStripeSuper* pStripeData,
        int index,
        float delta,
        float journey,
        float invStripeLength,
        const nn::util::Float4& prevTex
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      デフォルトの頂点アトリビュートを設定します。
    //! @param[out] pOutVertexAttribute 頂点バッファ
    //! @param[in]  index               履歴点インデックス
    //! @param[in]  pos                 位置
    //! @param[in]  dir                 方向
    //! @param[in]  wing                ストライプの展開方向
    //! @param[in]  textureOffset       テクスチャオフセット
    //! @param[in]  scaleFadeValue      スケールフェードのための値
    //---------------------------------------------------------------------------
    void MakeDefaultVertexAttribute(
        VertexAttribute* pOutVertexAttribute,
        int index,
        const nn::util::Vector3fType& pos,
        const nn::util::Vector3fType& dir,
        const nn::util::Vector3fType& wing,
        const nn::util::Vector3fType& textureOffset,
        float scaleFadeValue ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                  エミッタプラグインユーザーデータを取得します。
    //! @param[in] pEmitter     エミッタへのポインタ
    //! @return                 エミッタプラグインユーザーデータへのポインタ
    //---------------------------------------------------------------------------
    static EmitterPluginUserData* GetEmitterUserData( const Emitter* pEmitter ) NN_NOEXCEPT
    {
        return reinterpret_cast< EmitterPluginUserData* >( pEmitter->GetEmitterPluginUserData() );
    }

    //---------------------------------------------------------------------------
    //! @brief                  ストライプデータを取得します
    //! @param[in] pEmitter     エミッタへのポインタ
    //! @return                 ストライプデータへのポインタ
    //---------------------------------------------------------------------------
    static ResStripeSuper* GetStripeData( Emitter* pEmitter ) NN_NOEXCEPT
    {
        return reinterpret_cast< ResStripeSuper* >( pEmitter->GetEmitterResource()->m_pEmitterPluginData );
    }

    //---------------------------------------------------------------------------
    //! @brief                      ConstantBuffer の中身を書き込みます。
    //! @param[in] pConstantBuffer  ConstantBuffer へのポインタ
    //! @param[in] pStripeData      ストライプ設定
    //! @param[in] pEPUserData      エミッタ単位のストライプ設定
    //! @param[in] pStripe          ストライプ
    //! @param[in] pEmitter         エミッタ
    //! @param[in] meshType         ストライプ形状（通常／クロス）
    //---------------------------------------------------------------------------
    static void MakeConstantBufferObject(
        ConstantBufferObject* pConstantBuffer,
        const ResStripeSuper* pStripeData,
        const EmitterPluginUserData* pEPUserData,
        const StripeInstance* pStripe,
        const Emitter* pEmitter,
        StripeMeshType meshType ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief                      ストライプのカラーを更新します。
    //! @param[in] arg              コールバック引数
    //! @param[in] pStripe          ストライプインスタンスへのポインタ
    //---------------------------------------------------------------------------
    static void UpdateStripeColor( ParticleCalculateArgImpl& arg, StripeInstance* pStripe ) NN_NOEXCEPT;
private:
    static SuperStripeSystem*   g_pStripeSystem;        //!< ストライプシステムへのポインタ
private:
    nn::vfx::System* const      m_pSystem;              //!< Systemへのポインタ
    nn::vfx::Heap* const        m_pHeap;                //!< ヒープへのポインタ
    const BufferingMode         m_BufferMode;           //!< バッファリングモード
    const int                   m_StripeArrayCount;     //!< システム全体で共有するストライプ配列のサイズ
    const size_t                m_StripeWorkSize;       //!< ストライプワークサイズ
    int                         m_StripeArrayIndex;     //!< 最後に割り当てたストライプの配列位置
    int                         m_StripeCount;          //!< 現在使用中のストライプ数（遅延ストライプを含む）
    StripeInstance*             m_StripeArray;          //!< システム全体で共有するストライプ配列

    //------------------------------------------------------------------------------
    friend StripeSystemUtility;
};

} // namespace detail
} // namespace vfx
} // namespace nn
