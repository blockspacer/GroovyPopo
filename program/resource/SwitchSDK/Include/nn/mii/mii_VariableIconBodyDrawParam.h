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
 *  @brief 体格反映体つきアイコンの描画パラメータ定義です。
 *  @details 
 */


#include <nn/nn_Result.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/util/util_MathTypes.h>
#include <nn/mii/mii_Common.h>
#include <nn/mii/mii_DrawParam.h>

namespace nn { namespace mii { namespace detail {

class VariableIconBodyImpl; //!< 前方参照(非公開)

}}}

namespace nn { namespace mii {

class DrawParam;      //!< 前方参照



//! @name 構造体・クラス
//! @{

/**
 * @brief 体格反映体つきアイコン用描画パラメータクラス
 * @details 体格反映体つきアイコンを描画するときに必要な体の情報を管理します。
 */
class VariableIconBodyDrawParam : public DrawParam
{
public:
    /// @brief マトリックスインデックス頂点属性フォーマット
    static const ::nn::gfx::AttributeFormat MatrixIndexFormat = ::nn::gfx::AttributeFormat_8_Uint;
    /// @brief マトリックスインデックス頂点属性オフセット
    static const size_t                     MatrixIndexOffset = 0;
    /// @brief マトリックスインデックス頂点属性ストライド
    static const size_t                     MatrixIndexStride = sizeof(uint8_t);

    /**
     * @brief コンストラクタです。
     *
     * @details 
     */
    VariableIconBodyDrawParam() NN_NOEXCEPT;
    
     /**
     * @brief デストラクタです。
     *
     * @details 
     */
    ~VariableIconBodyDrawParam() NN_NOEXCEPT;
    
    
    /**
     * @brief マトリックスインデックスバッファを取得します。
     * @return マトリックスインデックスバッファ
     * @details 
     */
    const ::nn::gfx::Buffer* GetMatrixIndexBuffer() const NN_NOEXCEPT;
    
    /**
     * @brief マトリックスインデックスバッファサイズを取得します。
     * @return バッファサイズを返します。
     * @details 
     */
    size_t GetMatrixIndexBufferSize() const NN_NOEXCEPT;
    
    /**
     * @internal
     * @brief スキニング用頂点バッファバッファを設定する
     * @param[in] buffer 頂点属性バッファ
     * @param[in] size   頂点属性バッファのサイズ
     */
    void SetMatrixIndexBuffer(const ::nn::gfx::Buffer& buffer,size_t size) NN_NOEXCEPT;
    
private:
    const ::nn::gfx::Buffer* m_MatrixIndexBuffer;     //!< スキニング用頂点バッファバッファ
    size_t                   m_MatrixIndexBufferSize; //!< スキニング用頂点バッファのサイズ
};

//! @}

}}
