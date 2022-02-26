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
 *  @brief MiiのMaskテクスチャ描画用の定義です。
 *  @details 
 */


#include <nn/nn_Result.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/util/util_MathTypes.h>
#include <nn/mii/mii_Common.h>

namespace nn { namespace mii { namespace detail {

class MaskImpl; //!< 前方参照(非公開)

}}}

namespace nn { namespace mii {

class DrawParam; //!< 前方参照(非公開)
class Resource;  //!< 前方参照(非公開)
struct TemporaryBuffer; //!< 前方参照

//! @name 構造体・クラス
//! @{

/**
 * @brief Maskテクスチャ生成用クラス
 * @details Maskテクスチャ生成に必要なDrawParamを保持するクラスです。
 */
class Mask {
    NN_DISALLOW_COPY(Mask);
public:
    Mask() NN_NOEXCEPT;
    ~Mask() NN_NOEXCEPT;

    //! @name 列挙型
    //! @{

    /**
     * @brief Maskテクスチャの描画タイプを表す列挙型
     * @details Maskテクスチャへ描画するときの描画要素を表します。
     * @attention 描画順も表しています。
     */
    enum DrawType {
        DrawType_MustacheR,                ///<  0 : 右くちひげ
        DrawType_MustacheL,                ///<  1 : 左くちひげ
        DrawType_Mouth,                    ///<  2 : 口
        DrawType_EyebrowR,                 ///<  3 : 右眉毛
        DrawType_EyebrowL,                 ///<  4 : 左眉毛
        DrawType_EyeR,                     ///<  5 : 右目
        DrawType_EyeL,                     ///<  6 : 左目
        DrawType_Mole,                     ///<  7 : ほくろ
        DrawType_ClearAlpha,               ///<  8 : アルファ値クリア用
        DrawType_AlphaMustacheR,           ///<  9 : 右くちひげ(アルファ値描画)
        DrawType_AlphaMustacheL,           ///< 10 : 左くちひげ(アルファ値描画)
        DrawType_AlphaMouth,               ///< 11 : 口(アルファ値描画)
        DrawType_AlphaEyebrowR,            ///< 12 : 右眉毛(アルファ値描画)
        DrawType_AlphaEyebrowL,            ///< 13 : 左眉毛(アルファ値描画)
        DrawType_AlphaEyeR,                ///< 14 : 右目(アルファ値描画)
        DrawType_AlphaEyeL,                ///< 15 : 左目(アルファ値描画)
        DrawType_AlphaMole,                ///< 16 : ほくろ(アルファ値描画)
        DrawType_End,                      ///< 終値(17)
        DrawType_Min = DrawType_MustacheR, ///< 最小値(0)
        DrawType_Max = DrawType_AlphaMole, ///< 最大値(16)
    };

    /**
     * @brief テクスチャの列挙型
     * @details Maskが保持するテクスチャを表します。
     */
    enum TextureType {
        TextureType_Mustache,                      ///<  0 : 口ひげ
        TextureType_Eyebrow,                       ///<  1 : 眉毛
        TextureType_Mole,                          ///<  2 : ほくろ
        TextureType_MouthNormal,                   ///<  3 : 口(通常)
        TextureType_MouthSmile,                    ///<  4 : 口(笑い)
        TextureType_MouthPuzzled,                  ///<  5 : 口(悲しみ)
        TextureType_MouthAnger,                    ///<  6 : 口(怒り)
        TextureType_MouthOpen,                     ///<  7 : 口(口パク)
        TextureType_EyeNormal,                     ///<  8 : 目(通常)
        TextureType_EyeSmile,                      ///<  9 : 目(笑い)
        TextureType_EyePuzzled,                    ///< 10 : 目(悲しみ)
        TextureType_EyeSurprise,                   ///< 11 : 目(驚き)
        TextureType_EyeBlink,                      ///< 12 : 目(まばたき)
        TextureType_EyeWink,                       ///< 13 : 目(ウィンク)
        TextureType_End,                           ///< 終値(14)
        TextureType_Min = TextureType_Mustache,    ///< 最小値(0)
        TextureType_Max = TextureType_EyeWink,     ///< 最大値(13)
    };
    //! @}

    //! @name メモリ計算
    //! @{

    /**
     * @brief メモリのサイズの取得
     * @details Mask生成に必要な通常メモリのサイズを取得します。
     * @return 成功するとサイズを返します。
     */
    static size_t CalculateMemorySize() NN_NOEXCEPT;

    /**
    * @brief メモリのアライメントの取得
    * @details Mask生成に必要な通常メモリのサイズとアライメントを取得します。
    * @return 成功するとアライメントを返します。
    */
    static size_t CalculateMemoryAlignment() NN_NOEXCEPT;

    /**
     * @brief メモリプールのサイズの取得
     * @details Mask生成に必要なCpuInvisibleでないメモリプールのサイズを取得します。
     * @param[in]  pDevice    デバイス
     * @param[in]  resource   リソース
     * @param[in]  expression 使用する表情フラグ
     * @pre pDevice != NULL
     *      @n && @ref ExpressionFlag に1つ以上の表情が指定されている。
     * @return 成功するとメモリプールのアライメントを返します。
     *         @n 失敗すると0を返します。
     */
    static size_t CalculateMemoryPoolSize(
        nn::gfx::Device* pDevice,
        const Resource& resource,
        int expression) NN_NOEXCEPT;

    /**
    * @brief メモリプールのアライメントの取得
    * @details Mask生成に必要なCpuInvisibleでないメモリプールのアライメントを取得します。
    * @param[in]  pDevice    デバイス
    * @param[in]  resource   リソース
    * @param[in]  expression 使用する表情フラグ
    * @pre pDevice != NULL
    *      @n && @ref ExpressionFlag に1つ以上の表情が指定されている。
    * @return 成功するとメモリプールのアライメントを返します。
    *         @n 失敗すると0を返します。
    */
    static size_t CalculateMemoryPoolAlignment(
        nn::gfx::Device* pDevice,
        const Resource& resource,
        int expression) NN_NOEXCEPT;

    //! @}

    //! @name 初期化・終了処理
    //! @{

    /**
     * @brief Maskを初期化します。
     * @details 初期化後のデフォルト値は以下の通りです。
     * @attention pMemory および pMemoryPoolは、@ref Mask::Finalize() まで保持する必要があります。
     * @attention resource は、Resource の設定条件によって
     *            @n @ref Mask::Finalize() まで保持の必要があります。
     * @attention 初期化済みのMaskを再初期化することは出来ません。
     *            @n @ref Finalize() してから呼び出します。
     * @attention pTemporaryBufferは初期化時にのみ必要です。
     *            @n 初期化後に破棄しても問題ありません。
     * @attention pMemoryPool には MemoryPoolProperty_CpuCached が設定されたメモリプールを渡してください。
     * @attention 上下反転フラグ isVerticalFlip はアプリケーションの座標系設定に応じて設定してください。
     * @param[in] pMemory          Maskが使用するメモリ
     * @param[in] memorySize       pMemoryのサイズ
     * @param[in] pDevice          デバイス
     * @param[in] pMemoryPool      Maskが使用するメモリプール
     * @param[in] memoryPoolOffset Maskが使用するメモリプールのオフセット
     * @param[in] memoryPoolSize   Maskが使用するメモリプールのサイズ
     * @param[in] pTemporaryBuffer      一時バッファへのポインタ
     * @param[in] resource         Maskの構築用リソース
     * @param[in] charInfo         生成するMiiのCharInfo
     * @param[in] expression       使用する表情フラグ
     * @param[in] isVerticalFlip   上下反転フラグ
     * @return 成功するとnn::ResultSuccessを返します。
     * @pre pMemory != NULL
     *      @n && pDevice != NULL
     *      @n && pMemoryPool != NULL
     *      @n && @ref Resource::IsInitialized()
     *      @n && ! @ref Mask::IsInitialized()
     *      @n && pTemporaryBuffer != NULL
     *      @n && オフセット、サイズが正常
     *      @n && charInfoが壊れていない。
     *      @n && メモリ計算時と同じ @ref ExpressionFlag を指定している。
     * @post 成功したらMask::IsInitialized() = true
     */
    nn::Result Initialize(
        void* pMemory
        ,size_t memorySize
        ,nn::gfx::Device* pDevice
        ,nn::gfx::MemoryPool* pMemoryPool
        ,ptrdiff_t memoryPoolOffset
        ,size_t memoryPoolSize
        ,TemporaryBuffer* pTemporaryBuffer
        ,const Resource& resource
        ,const CharInfo& charInfo
        ,int expression
        ,bool isVerticalFlip
        ) NN_NOEXCEPT;

    /**
     * @brief Maskを破棄します。
     * @param[in] pDevice デバイス
     * @pre Mask::IsInitialized() == true
     * @post Mask::IsInitialized() = false
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
     * @brief テクスチャビューの取得
     * @details 指定typeのテクスチャービューを取得します。
     * @param[in] type 取得するテクスチャビューの種類
     * @return 存在すればテクスチャービューを返します。存在しなければNULL
     */
    const nn::gfx::TextureView* GetTextureView(TextureType type) const NN_NOEXCEPT;

    /**
     * @brief テクスチャデスクリプタスロットの設定
     * @details 指定typeのテクスチャデスクリプタスロットを設定します。
     * @param[in] type 設定先の種類 
     * @param[in] slot 設定するデスクリプタスロット
     * @pre Mask::IsInitialized() == true
     *      @n && slot.IsValid() == true
     */
    void SetTextureDescriptorSlot(TextureType type,const nn::gfx::DescriptorSlot& slot) NN_NOEXCEPT;

    /**
     * @brief テクスチャデスクリプタスロットの取得
     * @details 指定typeのテクスチャデスクリプタスロットを取得します。
     * @param[in] type 設定先の種類 
     * @return 設定されているデスクリプタスロット
     *      @n (未設定なら無効なデスクリプタスロットが返ります。)
     */
    const nn::gfx::DescriptorSlot& GetTextureDescriptorSlot(TextureType type) const NN_NOEXCEPT;


    /**
     * @brief 表情が利用できるか
     * @details 引数で指定した表情が利用できるかを判定します。
     * @param[in] expression 表情
     * @return 利用できるならtrueを返します。
     */
    bool IsAvailableExpression(Expression expression) const NN_NOEXCEPT;

    /**
     * @brief 描画パラメータを取得
     * @details 指定した条件の描画パラメータを取得します。
     * @attention テクスチャデスクリプタスロットは、
     *            @n @ref SetTextureDescriptorSlot() で変更されることに注意してください。
     * @param[in] expression 表情
     * @param[in] type       描画種類
     * @pre @ref IsInitialized() == true 
     *      @n && @ref Initialize() で指定したExpressionFlag に含まれている表情
     */
    const DrawParam* GetDrawParam(Expression expression,DrawType type) const NN_NOEXCEPT;

    //! @}

    //! @internal
    //! @name 内部実装用
    //! @{
    
    /**
     * @internal
     * @brief 実装を返します。
     * @attention 内部実装用です。使用しないでください。
     */
    detail::MaskImpl& GetImpl() NN_NOEXCEPT;
    const detail::MaskImpl& GetImpl() const NN_NOEXCEPT;

    //! @}
private:
    detail::MaskImpl* m_pImpl; ///< 非公開
};

//! @}

}}
