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
 *  @brief 体格反映体つきアイコンの定義です。
 *  @details 
 */


#include <nn/nn_Result.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/util/util_MathTypes.h>
#include <nn/mii/mii_Common.h>


namespace nn { namespace mii { namespace detail {

class VariableIconBodyImpl; //!< 前方参照(非公開)

}}}

namespace nn { namespace mii {

class VariableIconBodyDrawParam;      //!< 前方参照
struct VariableIconBodyWorldMatrix;
struct VariableIconBodySkeleton;

//! @name 構造体・クラス
//! @{

/**
 * @brief 体格反映体つきアイコン用体クラス
 * @details 体格反映体つきアイコンを描画するときに必要な体の情報を管理します。
 */
class VariableIconBody 
{
    NN_DISALLOW_COPY(VariableIconBody);
public:
    VariableIconBody() NN_NOEXCEPT;
    ~VariableIconBody() NN_NOEXCEPT;

    //! @name メモリ計算
    //! @{

    /**
     * @brief メモリのアライメントの取得
     * @details VariableIconBody生成に必要な通常メモリのアライメントを取得します。
     * @return アライメントを返します。
     */
    static size_t CalculateMemoryAlignment() NN_NOEXCEPT;

    /**
     * @brief メモリのサイズを取得
     * @details VariableIconBody生成に必要な通常メモリのサイズを取得します。
     * @return サイズを返します。
     */
    static size_t CalculateMemorySize() NN_NOEXCEPT;


    /**
     * @brief メモリプールのアライメントの取得
     * @details VariableIconBody生成に必要なメモリプールのアライメントを取得します。
     * @param[in]  pDevice    デバイス
     * @param[in]  pBody      体リソースファイルのポインタ
     * @param[in]  bodySize   pBodyのサイズ
     * @pre pSize != NULL 
     *      @n && pDevice != NULL
     *      @n && pBody と bodySizeが正常
     * @return 成功するとメモリプールのアライメントを返します。
     *         @n 失敗すると0を返します。
     */
    static size_t CalculateMemoryPoolAlignment(
        nn::gfx::Device* pDevice
        ,const void* pBody
        ,size_t bodySize) NN_NOEXCEPT;

    /**
     * @brief メモリプールのサイズの取得
     * @details VariableIconBody生成に必要なメモリプールのサイズを取得します。
     * @param[in]  pDevice    デバイス
     * @param[in]  pBody      体リソースファイルのポインタ
     * @param[in]  bodySize   pBodyのサイズ
     * @pre pSize != NULL 
     *      @n && pDevice != NULL
     *      @n && pBody と bodySizeが正常
     * @return 成功するとメモリプールのサイズを返します。
     *         @n 失敗すると0を返します。
     */
    static size_t CalculateMemoryPoolSize(
        nn::gfx::Device* pDevice
        ,const void* pBody
        ,size_t bodySize) NN_NOEXCEPT;

    //! @}

    //! @name 初期化・終了処理
    //! @{
    /**
     * @brief VariableIconBodyを初期化します。
     * @param[in] pMemory          VariableIconBodyが使用するメモリ
     * @param[in] memorySize       pMemoryのサイズ
     * @param[in] pDevice          デバイス
     * @param[in] pMemoryPool      VariableIconBodyが使用するメモリプール
     * @param[in] memoryPoolOffset VariableIconBodyが使用するメモリプールのオフセット
     * @param[in] memoryPoolSize   VariableIconBodyが使用するメモリプールのサイズ
     * @param[in] pBody            体リソースファイルのポインタ
     * @param[in] bodySize         pBodyのサイズ
     * @return 成功するとnn::ResultSuccessを返します。
     * @pre pMemory != NULL
     *      @n && pDevice != NULL
     *      @n && pMemoryPool != NULL
     *      @n && オフセット、サイズが正常
     *      @n && charInfoが壊れていない。
     * @post 成功したら VariableIconBody::IsInitialized() = true
     * @details pBodyは、初期化後破棄できます。
     * @attention pMemory 、 pMemoryPool は、
     *            @n @ref VariableIconBody::Finalize() まで保持する必要があります。
     * @attention 初期化済みのVariableIconBodyを再初期化することは出来ません。
     *            @n @ref Finalize() してから呼び出します。
     * @attention 初期化時に渡したメモリプールが CpuCached な場合、
     *            @n この関数の呼び出し後に、メモリプールをフラッシュする必要があります。
     */
    nn::Result Initialize(
        void* pMemory
        ,size_t memorySize
        ,nn::gfx::Device* pDevice
        ,nn::gfx::MemoryPool* pMemoryPool
        ,ptrdiff_t memoryPoolOffset
        ,size_t memoryPoolSize
        ,void* pBody
        ,size_t bodySize) NN_NOEXCEPT;

    /**
     * @brief VariableIconBodyを破棄します。
     * @param[in] pDevice デバイス
     * @pre VariableIconBody::IsInitialized() == true
     * @post VariableIconBody::IsInitialized() = false
     */
    void Finalize(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    /**
     * @brief @ref Initialize() 済みかを判定
     * @return 初期化済みの場合はtrueを返します。
     */
    bool IsInitialized() const NN_NOEXCEPT;

    //! @}

    //! @name 設定・取得処理
    //! @{

    /**
     * @brief 描画パラメータを取得
     * @param[in] info 体と組み合わせるCharInfo
     * @return CharInfoの性別にあわせたDrawParamを返します。
     * @pre @ref IsInitialized() == true 
     * @details 指定した条件の描画パラメータを取得します。
     *          @n infoの性別から適した描画パラメータを返します。
     */
    const VariableIconBodyDrawParam* GetDrawParam(const CharInfo& info) const NN_NOEXCEPT;

    /**
    * @brief 描画パラメータを取得
    * @param[in] gender 性別
    * @return 性別にあわせたDrawParamを返します。
    * @pre @ref IsInitialized() == true
    * @details 指定した条件の描画パラメータを取得します。
    *          @n infoの性別から適した描画パラメータを返します。
    */
    const VariableIconBodyDrawParam* GetDrawParam(nn::mii::Gender gender) const NN_NOEXCEPT;
    
    /**
     * @brief 指定した条件でワールドマトリックスを計算します。
     * @param[out] pOut   ワールドマトリックス出力先
     * @param[in]  gender 性別
     * @param[in]  build  体格
     * @param[in]  height 身長
     * @pre pWorldMatrix != nullptr 
     *      && IsInitialized() == true
     *      && HeightMin <= build <= HeightMax
     *      && BuildMin <= height <= BuildMax
     * @details 
     */
    void CalculateWorldMatrix(VariableIconBodyWorldMatrix* pOut
                              , Gender gender
                              , int build
                              , int height) const NN_NOEXCEPT;

    /**
     * @brief 指定した条件でワールドマトリックスを計算します。
     * @param[out] pOut   ワールドマトリックス出力先
     * @param[in]  info   @ref CharInfo
     * @pre pWorldMatrix != nullptr 
     *      && IsInitialized() == true
     *      && CharInfoAccessor(info).IsValid()
     * @details CharInfoからgender,build,heightを取り出して利用します。
     */
    void CalculateWorldMatrix(VariableIconBodyWorldMatrix* pOut
                              , const CharInfo& info) const NN_NOEXCEPT;

    /**
     * @brief ワールドマトリックスからスキニング用マトリックとして保存します。
     * @param[out] pOut        スキニング用マトリックス出力先
     * @param[in]  worldMatrix ワールドマトリックス出力先
     * @param[in]  gender      性別
     * @pre pOut != nullptr
     *      && IsInitialized() == true
     * @details 
     */
    void StoreSkeleton(VariableIconBodySkeleton* pOut
                       ,const VariableIconBodyWorldMatrix& worldMatrix
                       , Gender gender) const NN_NOEXCEPT;

    /**
    * @brief ワールドマトリックスからスキニング用マトリックとして保存します。
    * @param[out] pOut        スキニング用マトリックス出力先
    * @param[in]  worldMatrix ワールドマトリックス出力先
    * @param[in]  info        @ref CharInfo
    * @pre pOut != nullptr
    *      && IsInitialized() == true
    *      && CharInfoAccessor(info).IsValid()
    * @details CharInfoからgenderを取り出して利用します。
    */
    void StoreSkeleton(VariableIconBodySkeleton* pOut
                       , const VariableIconBodyWorldMatrix& worldMatrix
                       , const CharInfo& info) const NN_NOEXCEPT;


    /**
     * @brief CharModelを配置するワールドマトリックスを保存します。
     * @param[out] pOut        Headのワールドマトリックス出力先
     * @param[in]  worldMatrix ワールドマトリックス出力先
     */
    void StoreHeadWorldMatrix(nn::util::FloatColumnMajor4x3* pOut
                              , const VariableIconBodyWorldMatrix& worldMatrix) const NN_NOEXCEPT;

    /**
     * @brief カメラマトリックスを保存します。
     * @param[out] pOut        マトリックス出力先
     * @param[in]  worldMatrix ワールドマトリックス出力先
     */
    void StoreCameraMatrix(nn::util::FloatColumnMajor4x3* pOut
                              , const VariableIconBodyWorldMatrix& worldMatrix) const NN_NOEXCEPT;

    //! @}



private:
    detail::VariableIconBodyImpl* m_Impl; //!< 非公開
};

//! @}

}}
