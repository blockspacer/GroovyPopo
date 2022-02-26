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
 *  @brief 体つきアイコンを描画するための定義です。
 *  @details 
 */


#include <nn/nn_Result.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/util/util_MathTypes.h>
#include <nn/mii/mii_Common.h>

namespace nn { namespace mii { namespace detail {

class IconBodyImpl; //!< 前方参照(非公開)

}}}

namespace nn { namespace mii {

class CharModelInfo;  //!< 前方参照
class DrawParam;      //!< 前方参照

//! @name 構造体・クラス
//! @{

/**
 * @brief 体つきアイコン用体クラス
 * @details 体つきアイコンを描画するときに必要な体の情報を管理します。
 */
class IconBody 
{
    NN_DISALLOW_COPY(IconBody);
public:
    IconBody() NN_NOEXCEPT;
    ~IconBody() NN_NOEXCEPT;

    //! @name メモリ計算
    //! @{

    /**
     * @brief メモリのアライメントの取得
     * @details IconBody生成に必要な通常メモリのアライメントを取得します。
     * @return アライメントを返します。
     */
    static size_t CalculateMemoryAlignment() NN_NOEXCEPT;

    /**
     * @brief メモリのサイズを取得
     * @details IconBody生成に必要な通常メモリのサイズを取得します。
     * @return サイズを返します。
     */
    static size_t CalculateMemorySize() NN_NOEXCEPT;


    /**
     * @brief メモリプールのアライメントの取得
     * @details IconBody生成に必要なメモリプールのアライメントを取得します。
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
     * @details IconBody生成に必要なメモリプールのサイズを取得します。
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
     * @brief IconBodyを初期化します。
     * @param[in] pMemory          IconBodyが使用するメモリ
     * @param[in] memorySize       pMemoryのサイズ
     * @param[in] pDevice          デバイス
     * @param[in] pMemoryPool      IconBodyが使用するメモリプール
     * @param[in] memoryPoolOffset IconBodyが使用するメモリプールのオフセット
     * @param[in] memoryPoolSize   IconBodyが使用するメモリプールのサイズ
     * @param[in] pBody            体リソースファイルのポインタ
     * @param[in] bodySize         pBodyのサイズ
     * @return 成功するとnn::ResultSuccessを返します。
     * @pre pMemory != NULL
     *      @n && pDevice != NULL
     *      @n && pMemoryPool != NULL
     *      @n && オフセット、サイズが正常
     *      @n && charInfoが壊れていない。
     * @post 成功したら IconBody::IsInitialized() = true
     * @details pBodyは、初期化後破棄できます。
     * @attention pMemory 、 pMemoryPool は、
     *            @n @ref IconBody::Finalize() まで保持する必要があります。
     * @attention 初期化済みのIconBodyを再初期化することは出来ません。
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
     * @brief IconBodyを破棄します。
     * @param[in] pDevice デバイス
     * @pre IconBody::IsInitialized() == true
     * @post IconBody::IsInitialized() = false
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
    const DrawParam* GetDrawParam(const CharInfo& info) const NN_NOEXCEPT;

    /**
    * @brief 描画パラメータを取得
    * @param[in] gender 性別
    * @return 性別にあわせたDrawParamを返します。
    * @pre @ref IsInitialized() == true
    * @details 指定した条件の描画パラメータを取得します。
    *          @n infoの性別から適した描画パラメータを返します。
    */
    const DrawParam* GetDrawParam(nn::mii::Gender gender) const NN_NOEXCEPT;
    //! @}

private:
    detail::IconBodyImpl* m_Impl; //!< 非公開
};

//! @}

}}
