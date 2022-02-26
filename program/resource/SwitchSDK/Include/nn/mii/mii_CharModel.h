/*---------------------------------------------------------------------------*
  Copyright (C) Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *---------------------------------------------------------------------------*/

#pragma once

/**
 *  @file
 *  @brief CharModel 定義です。
 *  @details Mii描画用のクラス定義です。
 */

#include <nn/nn_Result.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/util/util_MathTypes.h>
#include <nn/mii/mii_Common.h>
#include <nn/mii/mii_DrawCommon.h>

namespace nn { namespace mii { namespace detail {

class CharModelImpl; //!< 前方参照(非公開)

}}}

namespace nn { namespace mii {

class Resource;  //!< 前方参照
class DrawParam; //!< 前方参照
struct CharInfo; //!< 前方参照
struct TemporaryBuffer; //!< 前方参照

//! @name 構造体・クラス
//! @{

/**
 * @brief CharModel生成用のデータ
 * @details CharModel生成に必要な情報をまとめた構造体です。
 */
struct CharModelInfoData {
    nn::gfx::ImageFormat facelineFormat;     ///< Facelineテクスチャフォーマット
    nn::gfx::ImageFormat maskFormat;         ///< Maskテクスチャフォーマット
    int                  facelineMipCount;   ///< Facelineテクスチャミップマップの数
    int                  maskMipCount;       ///< Maskテクスチャミップマップの数
    int                  facelineHeight;     ///< Facelineテクスチャの縦方向の解像度
    int                  maskResolution;     ///< Maskテクスチャの解像度
    int                  maskCount;          ///< マスクの数(最大:Expression_MAX+1)
    int                  createFlag;         ///< 生成条件
    float                scale;              ///< CharModelのスケール
    CoordinateType       coordinateUp;       ///< 上方向の座標を取得します。
    CoordinateType       coordinateFront;    ///< 前方向の座標を取得します。
};

/**
 * @brief CharModel生成用のインフォ
 * @details CharModelの生成に必要な情報のアクセサです。
 */
class CharModelInfo : public CharModelInfoData {
public:
    CharModelInfo() NN_NOEXCEPT;

    //! @name 設定
    //! @{
    /**
     * @brief デフォルト値を設定
     * @return 自身の参照を返します。
     * @details デフォルト値は以下の通りです。
     *          @n @ref CharModelInfoData::facelineFormat     = nn::gfx::ImageFormat_R8_G8_B8_A8_Unorm
     *          @n @ref CharModelInfoData::maskFormat         = nn::gfx::ImageFormat_R8_G8_B8_A8_Unorm
     *          @n @ref CharModelInfoData::facelineMipCount   = 1
     *          @n @ref CharModelInfoData::maskMipCount       = 1
     *          @n @ref CharModelInfoData::facelineHeight     = 512
     *          @n @ref CharModelInfoData::maskResolution     = 512
     *          @n @ref CharModelInfoData::maskCount          = 1
     *          @n @ref CharModelInfoData::createFlag         = @ref CreateFlag_Default
     *          @n @ref CharModelInfoData::scale              = 1.0f
     *          @n @ref CharModelInfoData::coordinateUp       = @ref CoordinateType_DefaultUp
     *          @n @ref CharModelInfoData::coordinateFront    = @ref CoordinateType_DefaultFront
     */
    CharModelInfo& SetDefault() NN_NOEXCEPT;

    /**
     * @brief 動的生成テクスチャのフォーマット指定
     * @param[in] faceline Facelineテクスチャの画像フォーマット
     * @param[in] mask     Maskテクスチャの画像フォーマット
     * @return 自身の参照を返します。
     * @details 動的生成するテクスチャの画像フォーマットを指定します。
     * @attention nn::gfxのAPITypeにより圧縮に制限がある可能性があります。
     */
    CharModelInfo& SetDynamicTextureFormat(
        nn::gfx::ImageFormat faceline,nn::gfx::ImageFormat mask) NN_NOEXCEPT;

    /**
     * @brief 動的生成テクスチャのMipMap設定
     * @param[in] faceline Facelineテクスチャのミップマップの数
     * @param[in] mask     Maskテクスチャのミップマップの数
     * @return 自身の参照を返します。
     * @details 動的生成テクスチャのMipカウントを指定します。
     */
    CharModelInfo& SetDynamicTextureMipCount(int faceline,int mask) NN_NOEXCEPT;

    /**
     * @brief 動的生成テクスチャの解像度設定
     * @param[in] facelineHeightValue Facelineテクスチャの解像度(縦方向)を指定します。
     * @param[in] mask                Maskテクスチャの解像度を指定します。
     * @return 自身の参照を返します。
     * @attention nn::gfxのAPITypeにより解像度に制限がある可能性があります。
     * @attention Facelineの横幅は、縦幅の半分です。
     * @details 動的生成するテクスチャの解像度を指定します。
     */
    CharModelInfo& SetDynamicTextureResolution(int facelineHeightValue,int mask) NN_NOEXCEPT;

    /**
     * @brief Maskテクスチャの枚数設定
     * @param[in] count テクスチャの枚数
     * @pre 0 < count < @ref Expression_End
     * @return 自身の参照を返します。
     * @details CharModelで使用するMaskテクスチャの枚数を指定します。
     */
    CharModelInfo& SetMaskCount(uint32_t count) NN_NOEXCEPT;

    /**
     * @brief 生成条件を指定します。
     * @details CharModelの生成条件を指定します。
     * @param[in] createFlags 生成条件をビットフラグで指定します。
     * @pre @ref CreateFlag_Normal,@ref CreateFlag_Hat,@ref CreateFlag_FaceOnly を1つ以上指定
            \n && @ref CreateFlag_NoseNormal,@ref CreateFlag_NoseFlatten を1つ以上指定
     * @return 自身の参照を返します。
     */
    CharModelInfo& SetCreateFlag(int createFlags) NN_NOEXCEPT;

    /**
     * @brief スケールを指定します。
     * @param[in] value スケールの値
     * @pre 0.0f < value
     * @return 自身の参照を返します。
     * @details スケールの値を指定します。
     */
    CharModelInfo& SetScale(float value) NN_NOEXCEPT;

    /**
     * @brief 上方向の軸の向きを指定します。
     * @param[in] type CoordinateType
     * @return 自身の参照を返します。
     * @details 座標系の上方向の軸のタイプを設定します。
     */
    CharModelInfo& SetCoordinateUp(CoordinateType type) NN_NOEXCEPT;

    /**
     * @brief 正面方向の軸の向きを指定します。
     * @param[in] type CoordinateType
     * @return 自身の参照を返します。
     * @details 座標系の正面方向の軸のタイプを設定します。
     */
    CharModelInfo& SetCoordinateFront(CoordinateType type) NN_NOEXCEPT;

    //! @}

    //! @name 取得
    //! @{

    /**
     * @brief Facelineテクスチャの画像フォーマット
     * @return 画像フォーマット
     * @details Facelineテクスチャの画像フォーマットを返します。
     */
    nn::gfx::ImageFormat GetFacelineFormat() const NN_NOEXCEPT;

    /**
     * @brief Maskテクスチャの画像フォーマット
     * @return 画像フォーマット
     * @details Maskテクスチャの画像フォーマットを返します。
     */
    nn::gfx::ImageFormat GetMaskFormat() const NN_NOEXCEPT;

    /**
     * @brief Facelineテクスチャのミップマップ数取得
     * @return ミップマップ数を返します。
     * @details Facelineテクスチャのミップマップ数を取得します。
     */
    int GetFacelineMipCount() const NN_NOEXCEPT;

    /**
     * @brief Maskテクスチャのミップマップ数取得
     * @return ミップマップ数を返します。
     * @details Maskテクスチャのミップマップ数を取得します。
     */
    int GetMaskMipCount() const NN_NOEXCEPT;

    /**
     * @brief Facelineテクスチャの高さ取得
     * @return 解像度
     * @details Facelineテクスチャの高さを返します。
     */
    int GetFacelineHeight() const NN_NOEXCEPT;

    /**
     * @brief Facelineテクスチャの幅取得
     * @return 解像度
     * @details Facelineテクスチャの幅を返します。幅は高さの半分です。
     */
    int GetFacelineWidth() const
    {
        return this->facelineHeight / 2;
    }

    /**
     * @brief Maskテクスチャの解像度取得
     * @return 解像度
     * @details Maskテクスチャの解像度を返します。
     */
    int GetMaskResolution() const NN_NOEXCEPT;

    /**
     * @brief Maskテクスチャの枚数取得
     * @details Maskテクスチャの枚数を返します。
     * @return Maskテクスチャの枚数
     */
    int GetMaskCount() const NN_NOEXCEPT;

    /**
     * @brief 生成条件を取得
     * @return 生成条件フラグ
     * @details 生成条件を取得します。
     */
    int GetCreateFlag() const NN_NOEXCEPT;
    
    /**
     * @brief CharModelのスケール値を取得
     * @return スケール
     */
    float GetScale() const NN_NOEXCEPT;

    /**
     * @brief 上方向座標を取得
     * @return 上方向の座標
     */
    CoordinateType GetCoordinateUp() const NN_NOEXCEPT;

    /**
     * @brief 前方向座標を取得
     * @return 前方向の座標
     */
    CoordinateType GetCoordinateFront() const NN_NOEXCEPT;

    //! @}
};

/**
 * @brief Mii描画情報
 */
class CharModel {
    NN_DISALLOW_COPY(CharModel);
public:
    CharModel() NN_NOEXCEPT;
    ~CharModel() NN_NOEXCEPT;

    //! @name 列挙型
    //! @{

    /**
     * @brief モデルの描画タイプを表す列挙型
     * @details @ref CharModel の描画要素を表します。
     */
    enum DrawType {
        DrawType_Beard,                ///< 0 : あごひげ(不透明)
        DrawType_Faceline,             ///< 1 : 顔(不透明)
        DrawType_Forehead,             ///< 2 : おでこ(不透明)
        DrawType_Hair,                 ///< 3 : 髪(不透明)
        DrawType_Hat,                  ///< 4 : 帽子(不透明)
        DrawType_Nose,                 ///< 5 : 鼻(不透明)
        DrawType_Mask,                 ///< 6 : 表情(半透明)
        DrawType_Noseline,             ///< 7 : 鼻輪郭(半透明)
        DrawType_Glass,                ///< 8 : メガネ(半透明)
        DrawType_End,                  ///< 9 : 終端
        DrawType_Min = DrawType_Beard, ///< 最小値(0)
        DrawType_Max = DrawType_Glass, ///< 最大値(8)
        DrawType_XluMin = DrawType_Mask, ///< 半透明の最小値
    };

    /**
     * @brief テクスチャの列挙型
     * @details CharModelが保持するテクスチャを表します。
     */
    enum TextureType {
        TextureType_Faceline,                   ///< 0 : 顔
        TextureType_Hat,                        ///< 1 : 帽子
        TextureType_Mask,                       ///< 2 : 表情
        TextureType_Noseline,                   ///< 3 : 鼻輪郭
        TextureType_Glass,                      ///< 4 : メガネ
        TextureType_End,                        ///< 5 : 終端
        TextureType_Min = TextureType_Faceline, ///< 最小値(0)
        TextureType_Max = TextureType_Glass,    ///< 最大値(4)
    };

    //! @}
    
    //! @name メモリ計算
    //! @{

    /**
     * @brief メモリのサイズの取得
     * @param[in]  info       設定済みの @ref CharModelInfo
     * @pre pSize != NULL 
     *          @n && info.IsValid()
     * @return 成功すると必要サイズを返します。
     *          @n 失敗すると0を返します。
     * @details CharModel生成に必要な通常メモリのサイズを取得します。
     */
    static size_t CalculateMemorySize(const CharModelInfo& info) NN_NOEXCEPT;

    /**
     * @brief メモリアライメントの取得
     * @param[in]  info       設定済みの @ref CharModelInfo
     * @pre pSize != NULL
     *          @n && info.IsValid()
     * @return 成功するとアライメントを返します。
     *          @n 失敗すると0を返します。
     * @details CharModel生成に必要なアライメントを取得します。
     */
    static size_t CalculateMemoryAlignment(const CharModelInfo& info) NN_NOEXCEPT;

    /**
     * @brief メモリプールのサイズの取得
     * @param[in]  pDevice    デバイス
     * @param[in]  resource   CharModel生成用のリソース
     * @param[in]  info       設定済みの @ref CharModelInfo
     * @pre pDevice != NULL
     *      @n && info.IsValid()
     *      @n && Resource::IsInitialized()
     * @return 成功すると必要サイズを返します。
     *      @n 失敗すると0を返します。
     * @details CharModel生成に必要なメモリプールのサイズを取得します。
     *      @n 1 つのメモリプールを使用して初期化する場合はこの関数を使用してサイズを取得してください。
     */
    static size_t CalculateMemoryPoolSize(
        nn::gfx::Device* pDevice,const Resource& resource,const CharModelInfo& info) NN_NOEXCEPT;

    /**
     * @brief メモリプールのアライメントの取得
     * @param[in]  pDevice    デバイス
     * @param[in]  resource   CharModel生成用のリソース
     * @param[in]  info       設定済みの @ref CharModelInfo
     * @pre pDevice != NULL
     *      @n && info.IsValid()
     *      @n && Resource::IsInitialized()
     * @return 成功するとアライメントを返します。
     *         @n 失敗すると0を返します。
     * @details CharModel生成に必要なメモリプールのアライメントを取得します。
     *      @n 1 つのメモリプールを使用して初期化する場合はこの関数を使用してアライメントを取得してください。
     */
    static size_t CalculateMemoryPoolAlignment(
        nn::gfx::Device* pDevice, const Resource& resource, const CharModelInfo& info) NN_NOEXCEPT;

    
    /**
     * @brief 動的メモリプールのサイズの取得
     * @param[in]  pDevice    デバイス
     * @param[in]  info       設定済みの @ref CharModelInfo
     * @pre pDevice != NULL
     * @return 成功すると必要サイズを返します。
     *      @n 失敗すると0を返します。
     * @details CharModel生成に必要な動的メモリプール（レンダリングによって生成するテクスチャ用メモリプール）のサイズを取得します。
     *      @n 初期化後、レンダーターゲットとして扱われるメモリプールですので、
     *      @n メモリプールのプロパティには Compressible を設定する必要があります。
     *      @n メモリプールを分割して初期化する場合はこの関数を使用して動的メモリプールのサイズを取得してください。
     */
    static size_t CalculateDynamicMemoryPoolSize(
        nn::gfx::Device* pDevice, const CharModelInfo& info) NN_NOEXCEPT;

    /**
     * @brief 動的メモリプールのアライメントの取得
     * @param[in]  pDevice    デバイス
     * @param[in]  info       設定済みの @ref CharModelInfo
     * @pre pDevice != NULL
     * @return 成功するとアライメントを返します。
     *         @n 失敗すると0を返します。
     * @details CharModel生成に必要な動的メモリプール（レンダリングによって生成するテクスチャ用メモリプール）のアライメントを取得します。
     *      @n メモリプールを分割して初期化する場合はこの関数を使用して動的メモリプールのアライメントを取得してください。
     */
    static size_t CalculateDynamicMemoryPoolAlignment(
        nn::gfx::Device* pDevice, const CharModelInfo& info) NN_NOEXCEPT;

    /**
     * @brief 静的メモリプールのサイズの取得
     * @param[in]  resource   CharModel生成用のリソース
     * @pre Resource::IsInitialized()
     * @return 成功すると必要サイズを返します。
     *      @n 失敗すると0を返します。
     * @details CharModel生成に必要な静的メモリプール（初期化以降変更されないテクスチャやバッファ用メモリプール）のサイズを取得します。
     *      @n メモリプールを分割して初期化する場合はこの関数を使用して静的メモリプールのサイズを取得してください。
     */
    static size_t CalculateStaticMemoryPoolSize(const Resource& resource) NN_NOEXCEPT;

    /**
     * @brief 静的メモリプールのアライメントの取得
     * @param[in]  resource   CharModel生成用のリソース
     * @pre Resource::IsInitialized()
     * @return 成功するとアライメントを返します。
     *         @n 失敗すると0を返します。
     * @details CharModel生成に必要な静的メモリプール（初期化以降変更されないテクスチャやバッファ用メモリプール）のアライメントを取得します。
     *      @n メモリプールを分割して初期化する場合はこの関数を使用して静的メモリプールのアライメントを取得してください。
     */
    static size_t CalculateStaticMemoryPoolAlignment(const Resource& resource) NN_NOEXCEPT;

    //! @}

    //! @name 初期化・終了処理
    //! @{
    /**
     * @brief CharModelを初期化します。
     * @param[in] pMemory                   CharModelが使用するメモリ
     * @param[in] memorySize                pMemoryのサイズ
     * @param[in] pDevice                   デバイス
     * @param[in] pMemoryPool               CharModelが使用するメモリプール
     * @param[in] memoryPoolOffset          CharModelが使用するメモリプールのオフセット
     * @param[in] memoryPoolSize            CharModelが使用するメモリプールのサイズ
     * @param[in] resource                  CharModelの構築用リソース
     * @param[in] pTemporaryBuff            一時バッファへのポインタ
     * @param[in] modelInfo                 生成条件
     * @param[in] charInfo                  生成するMiiのCharInfo
     * @retresult
     *     @handleresult{nn::mii::ResultBroken}
     *     @handleresult{nn::mii::ResultInvalid}
     * @endretresult
     * @pre pMemory != NULL
     *      @n && pDevice != NULL
     *      @n && pMemoryPool != NULL
     *      @n && modelInfo.IsValid() == ture
     *      @n && ! @ref CharModel::IsInitialized()
     *      @n && pTemporaryBuffer != NULL
     *      @n && オフセット、サイズが正常
     *      @n && Resource::IsInitialized()
     *      @n && charInfoが壊れていない。
     * @post 成功したらCharModel::IsInitialized() = true
     * @details 
     * @attention pMemory および pMemoryPool は、@ref CharModel::Finalize() まで保持する必要があります。
     * @attention pMemoryPool には MemoryPoolProperty_CpuCached と
     *            @n MemoryPoolProperty_Compressibleが設定されたメモリプールを渡してください。
     *            @n レンダーターゲットとなるテクスチャも含まれるため、Compressible 設定が必要です。
     * @attention resource は、Resource の設定条件によって
     *            @n @ref CharModel::Finalize() まで保持の必要があります。
     * @attention 初期化済みのCharModelを再初期化することは出来ません。
     *            @n @ref Finalize() してから呼び出します。
     * @attention pTemporaryBufferは初期化中のみ必要です。
     *            @n 初期化が終われば破棄してかまいません。
     */
    nn::Result Initialize(
        void* pMemory
        ,size_t memorySize
        ,nn::gfx::Device* pDevice
        ,nn::gfx::MemoryPool* pMemoryPool
        ,ptrdiff_t memoryPoolOffset
        ,size_t memoryPoolSize
        ,const Resource& resource
        ,TemporaryBuffer* pTemporaryBuff
        ,const CharModelInfo& modelInfo
        ,const nn::mii::CharInfo& charInfo) NN_NOEXCEPT;

    //! @{
    /**
     * @brief CharModelを初期化します。
     * @param[in] pMemory                   CharModelが使用するメモリ
     * @param[in] memorySize                pMemoryのサイズ
     * @param[in] pDevice                   デバイス
     * @param[in] pDynamicMemoryPool        CharModelが使用する動的メモリプール
     * @param[in] dynamicMemoryPoolOffset   CharModelが使用する動的メモリプールのオフセット
     * @param[in] dynamicMemoryPoolSize     CharModelが使用する動的メモリプールのサイズ
     * @param[in] pStaticMemoryPool         CharModelが使用する静的メモリプール
     * @param[in] staticMemoryPoolOffset    CharModelが使用する静的メモリプールのオフセット
     * @param[in] staticMemoryPoolSize      CharModelが使用する静的メモリプールのサイズ
     * @param[in] resource                  CharModelの構築用リソース
     * @param[in] pTemporaryBuff            一時バッファへのポインタ
     * @param[in] modelInfo                 生成条件
     * @param[in] charInfo                  生成するMiiのCharInfo
     * @retresult
     *     @handleresult{nn::mii::ResultBroken}
     *     @handleresult{nn::mii::ResultInvalid}
     * @endretresult
     * @pre pMemory != NULL
     *      @n && pDevice != NULL
     *      @n && pDynamicMemoryPool != NULL
     *      @n && pStaticMemoryPool != NULL
     *      @n && modelInfo.IsValid() == true
     *      @n && ! @ref CharModel::IsInitialized()
     *      @n && pTemporaryBuffer != NULL
     *      @n && オフセット、サイズが正常
     *      @n && Resource::IsInitialized()
     *      @n && charInfoが壊れていない。
     * @post 成功したらCharModel::IsInitialized() = true
     * @details 
     * @attention pMemory および pMemoryPool は、@ref CharModel::Finalize() まで保持する必要があります。
     * @attention pMemoryPool には MemoryPoolProperty_CpuCached が設定されたメモリプールを渡してください。
     * @attention 動的メモリプール pDynamicMemoryPool は レンダリングによって生成するテクスチャの初期化に使用されます。
     *            @n 動的メモリプールのプロパティには 
     *            @n MemoryPoolProperty_CpuCached と MemoryProperty_Compressible を設定してください。
     *            @n レンダーターゲットとなるテクスチャ用のメモリプールのため、Compressible 設定が必要です。
     * @attention 静的メモリプール pStaticMemoryPool は初期化以降変更されないテクスチャやバッファの初期化に使用されます。
     *            @n 静的メモリプールのプロパティには MemoryPoolProperty_CpuCached を設定してください。
     * @attention resource は、Resource の設定条件によって
     *            @n @ref CharModel::Finalize() まで保持の必要があります。
     * @attention 初期化済みのCharModelを再初期化することは出来ません。
     *            @n @ref Finalize() してから呼び出します。
     * @attention pTemporaryBufferは初期化中のみ必要です。
     *            @n 初期化が終われば破棄してかまいません。
     */
    nn::Result Initialize(
        void* pMemory
        ,size_t memorySize
        ,nn::gfx::Device* pDevice
        , nn::gfx::MemoryPool* pDynamicMemoryPool
        , ptrdiff_t dynamicMemoryPoolOffset
        , size_t dynamicMemoryPoolSize
        ,nn::gfx::MemoryPool* pStaticMemoryPool
        ,ptrdiff_t staticMemoryPoolOffset
        ,size_t staticMemoryPoolSize
        ,const Resource& resource
        ,TemporaryBuffer* pTemporaryBuff
        ,const CharModelInfo& modelInfo
        ,const nn::mii::CharInfo& charInfo) NN_NOEXCEPT;

    /**
     * @brief CharModelを破棄します。
     * @param[in] pDevice デバイス
     * @pre CharModel::IsInitialized() == true
     * @post CharModel::IsInitialized() = false
     */
    void Finalize(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    /**
     * @brief Initialize済みかを判定
     * @return 初期化済みの場合はtrueを返します。
     */
    bool IsInitialized() const NN_NOEXCEPT;

    //! @}

    //! @name 設定・取得処理
    //! @{

    /**
     * @brief 帽子、被り物用移動値の取得
     * @attention 帽子、被り物用のパラメータは移動値のみです。回転値はありません。
     * @details 帽子や被り物の表示用の移動値を取得します。Faceline の種類に応じた値を返します。
     * @return 帽子や被り物を表示する際に使用する移動値を返します。
     * @pre @ref IsInitialized() == true 
     */
    const nn::util::Float3& GetHatPartsTranslate() const NN_NOEXCEPT;

    /**
     * @brief 頭部の装飾物のトランスフォームの取得
     * @details 指定された頭部装飾物の表示用の移動値と回転値を取得します。髪型の種類に応じた値を返します。
     * @param[in] type   HatPartsType
     * @return 頭部に装飾物を表示する際に使用するトランスフォームを返します。
     * @pre @ref IsInitialized() == true 
     *      @n && @ref HeadPartsType_Min <= type < @ref HeadPartsType_End
     */
    const HeadPartsTransform& GetHeadPartsTransform(HeadPartsType type) const NN_NOEXCEPT;

    /**
     * @brief バウンディングボックス取得
     * @details 指定条件でバウンディングボックスを取得する。
     * @param[in] model モデル種類
     * @param[in] nose  鼻種類
     * @return バウンディングボックス
     * @pre @ref IsInitialized() == true 
     *      @n && CharModelInfo::GetCreateFlag() const に該当種類(model,nose)が含まれている
     */
    const BoundingBox& GetBoundingBox(CreateModelType model,CreateNoseType nose) const NN_NOEXCEPT;
    
    /**
     * @brief テクスチャビューの取得
     * @details 指定typeのテクスチャービューを取得します。
     * @param[in] type 取得するテクスチャビューの種類
     * @param[in] mask TextureType_Maskの場合のスロット番号
     * @return 存在すればテクスチャービューを返します。存在しなければNULL
     * @attention TextureType_Mask以外はmaskを無視します。
     */
    const nn::gfx::TextureView* GetTextureView(TextureType type,int mask) const NN_NOEXCEPT;

    /**
     * @brief テクスチャデスクリプタスロットの設定
     * @details 指定typeのテクスチャデスクリプタスロットを設定します。
     * @param[in] type 設定先の種類 
     * @param[in] slot 設定するデスクリプタスロット
     * @param[in] mask TextureType_Maskの場合のスロット番号
     * @attention TextureType_Mask以外はmaskを無視します。
     * @pre CharModel::IsInitialized() == true
     *      @n && slot.IsValid() == true
     */
    void SetTextureDescriptorSlot(TextureType type,
            const nn::gfx::DescriptorSlot& slot,int mask) NN_NOEXCEPT;

    /**
     * @brief テクスチャデスクリプタスロットの取得
     * @details 指定typeのテクスチャデスクリプタスロットを取得します。
     * @param[in] type 設定先の種類 
     * @param[in] mask TextureType_Maskの場合のスロット番号
     * @return 設定されているデスクリプタスロット
     *      @n (未設定なら無効なデスクリプタスロットが返ります。)
     * @attention TextureType_Mask以外はmaskを無視します。
     */
    const nn::gfx::DescriptorSlot& GetTextureDescriptorSlot(TextureType type,int mask) const NN_NOEXCEPT;

    /**
     * @brief 描画パラメータを取得
     * @details 指定の条件で描画パラメータを取り出します。
     * @param[in] type  取得する描画パラメータの種類
     * @param[in] model 取得するモデルの種類
     * @param[in] nose  取得する鼻の種類
     * @param[in] mask  取得するマスクスロット番号
     * @return @ref DrawParam を返します。
     *         @n 描画の必要が無い場合はNULLを返します。
     * @pre @ref IsInitialized() == true 
     *      @n && CharModelInfo::GetCreateFlag() に該当種類(model,nose)が含まれている
     *      @n && 0 <= index < CharModelInfo::GetMaskNum()
     * @attention 取り出した @ref DrawParam は、取得時の条件で固定されています。
     *            @n テクスチャデスクリプタスロットは、
     *            @n @ref SetTextureDescriptorSlot() で変更されることに注意してください。
     *            @n @ref DrawType_Mask 以外は、maskを無視します。
     *            @n @ref DrawType とモデル生成の種類( @ref CreateModelType 、@ref CreateNoseType )には依存関係があります。
     *            @n @ref CreateModelType に依存する @ref DrawType は以下の通りです。
     *            @n @li @ref DrawType_Forehead
     *            @n @li @ref DrawType_Hair
     *            @n @li @ref DrawType_Hat
     *            
     *            これらの @ref DrawType は @ref CreateModelType の値によって返すパラメータが異なります。
     *            @n また、初期化時に設定した生成フラグに含まれない @ref CreateModelType を指定した場合は、nullptr を返します。
     *            @n @ref CreateNoseType に依存する @ref DrawType は以下の通りです。
     *            @n @li @ref DrawType_Nose
     *            @n @li @ref DrawType_Noseline
     *            
     *            これらの @ref DrawType は @ref CreateNoseType の値によって返すパラメータが異なります。
     *            @n また、初期化時に設定した生成フラグに含まれない @ref CreateNoseType を指定した場合は、nullptr を返します。
     *            @nその他の @ref DrawType は @ref CreateModelType や @ref CreateNoseType に依存しません。
     */
    const DrawParam* GetDrawParam(DrawType type
        ,CreateModelType model,CreateNoseType nose,int mask) const NN_NOEXCEPT;

    /**
     * @brief Facelineテクスチャの取得
     * @return テクスチャを返します。
     * @details Facelineテクスチャを取得します。
     */
    const nn::gfx::Texture* GetFacelineTexture() const NN_NOEXCEPT;

    /**
     * @brief Facelineテクスチャの取得
     * @return テクスチャを返します。
     * @details Facelineテクスチャを取得します。
     */
    nn::gfx::Texture* GetFacelineTexture() NN_NOEXCEPT;

    /**
     * @brief Maskテクスチャの取得
     * @param[in] mask マスクスロット番号
     * @return テクスチャを返します。
     * @details Maskテクスチャを取得します。mask の値が無効な場合、nullptr を返します。
     */
    const nn::gfx::Texture* GetMaskTexture(int mask) const NN_NOEXCEPT;

    /**
     * @brief Maskテクスチャの取得
     * @param[in] mask マスクスロット番号
     * @return テクスチャを返します。
     * @details Maskテクスチャを取得します。mask の値が無効な場合、nullptr を返します。
     */
    nn::gfx::Texture* GetMaskTexture(int mask) NN_NOEXCEPT;

    //! @}

    //! @internal
    //! @name 内部実装用
    //! @{
    
    /**
     * @internal
     * @brief 実装を返します。
     * @attention 内部実装用です。使用しないでください。
     */
    detail::CharModelImpl& GetImpl() NN_NOEXCEPT;
    const detail::CharModelImpl& GetImpl() const NN_NOEXCEPT;

    //! @}
private:
    detail::CharModelImpl* m_pImpl;
};

//! @}

}}
