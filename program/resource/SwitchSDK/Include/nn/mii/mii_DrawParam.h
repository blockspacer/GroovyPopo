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

/**
 *  @file
 *  @brief 描画パラメータ定義です。
 *  @details 1つのシェイプを描画するための定義です。
 */

#include <nn/gfx.h>
#include <nn/mii/mii_Common.h>

namespace nn { namespace mii {

//! @name 構造体・クラス
//! @{

/**
 * @brief 描画パラメータクラス
 * @details Miiのパーツを描画するためのパラメータを保持するクラスです。
 */
class DrawParam
{
    NN_DISALLOW_COPY( DrawParam ) NN_NOEXCEPT;
public:
    //! @name 列挙型
    //! @{
    
    /**
     * @brief 頂点属性バッファの種類
     *
     * @details 頂点属性バッファの種類を表す列挙型です。
     */
    enum AttributeType {
        AttributeType_Position, //!< 0 : 位置
        AttributeType_Normal,   //!< 1 : 法線
        AttributeType_Uv,       //!< 2 : テクスチャ座標
        AttributeType_Tangent,  //!< 3 : 接線
        AttributeType_Param,    //!< 4 : 特殊パラメータ
        AttributeType_End,      //!< 要素数(5)
    };

    /**
     * @brief 変調種類
     *
     * @details 描画時の変調方法を表す列挙型です。
     *          各変調の模擬シェーダーコードは以下の通りです。
     *          @n @ref ModulateType_Constant
     *          @code
     *          color = const[0].rgba;
     *          @endcode
     *          @n @ref ModulateType_Texture
     *          @code
     *          color = texture(s_tex,uv);
     *          @endcode
     *          @n @ref ModulateType_RgbLayered
     *          @code
     *          tex = texture(s_tex,uv);
     *          color = vec4(tex.r * const[0].rgb
     *                   + tex.g * const[1].rgb
     *                   + tex.b * const[2].rgb ,tex.a);
     *          @endcode
     *          @n @ref ModulateType_Alpha
     *          @code
     *          tex = texture(s_tex,uv);
     *          color = vec4(const[0].rgb,tex.r);
     *          @endcode
     *          @n @ref ModulateType_AlphaOpa
     *          @code
     *          tex = texture(s_tex,uv);
     *          color = vec4(tex.r * const[0].rgb,1.0f);
     *          @endcode
     *          @n @ref ModulateType_Glass
     *          @n メガネはガンマの種類によって半透明の見た目が異なります。
     *          @n リニアガンマの見た目を sRGB ガンマの見た目に近づけるため、アルファ値を補正する処理を入れています。
     *          @code
     *          tex = texture(s_tex,uv);
     *          color = vec4(tex.r * const[0].rgb, pow(tex.g, const[1].g));
     *          @endcode
     *          @n @ref ModulateType_IconBody
     *          @n この変調モードでは定数カラーに色が含まれていません。
     *          @n お気に入りの色を取得して処理してください。
     *          @code
     *          color = favoriteColor.rgba;
     *          @endcode
     */
    enum ModulateType {
        ModulateType_Constant,                    //!< 0 : 定数カラー描画(有効属性:Position,Normal)
        ModulateType_Texture,                     //!< 1 : テクスチャー描画(有効属性:Position,Normal,Uv)
        ModulateType_RgbLayered,                  //!< 2 : RGB各チャンネル変調描画(有効属性:Position,Normal,Uv)
        ModulateType_Alpha,                       //!< 3 : Aチャンネル変調描画、テクスチャアルファ出力(有効属性:Position,Normal,Uv)
        ModulateType_AlphaOpa,                    //!< 4 : Aチャンネル変調描画、アルファ値固定(有効属性:Position,Normal,Uv)
        ModulateType_Glass,                       //!< 5 : LAチャンネル変調描画、テクスチャアルファを補正して出力(有効属性:Position,Normal,Uv)
        ModulateType_IconBody,                    //!< 6 : 定数カラー描画(お気に入りの色)(有効属性:Position,Normal)
        ModulateType_End,                         //!< 終端(7)
        ModulateType_Min = ModulateType_Constant, //!< 最小値(0)
        ModulateType_Max = ModulateType_IconBody, //!< 最大値(6)
    };

    //! @}


    /// @brief 位置頂点属性フォーマット
    static const ::nn::gfx::AttributeFormat PositionFormat = ::nn::gfx::AttributeFormat_16_16_16_16_Float;
    /// @brief 位置頂点属性オフセット
    static const size_t                   PositionOffset = 0;
    /// @brief 位置頂点属性ストライド
    static const size_t                   PositionStride = sizeof(uint16_t) * 4;

    /// @brief 法線頂点属性フォーマット
    static const ::nn::gfx::AttributeFormat NormalFormat   
        = ::nn::gfx::AttributeFormat_10_10_10_2_Snorm;
    /// @brief 法線頂点属性オフセット
    static const size_t                   NormalOffset   = 0;
    /// @brief 法線頂点属性ストライド
    static const size_t                   NormalStride   = 4;

    /// @brief テクスチャ座標頂点属性フォーマット
    static const ::nn::gfx::AttributeFormat UvFormat     = ::nn::gfx::AttributeFormat_16_16_Float;

    /// @brief テクスチャ座標頂点属性オフセット
    static const size_t                   UvOffset       = 0;
    /// @brief テクスチャ座標頂点属性ストライド
    static const size_t                   UvStride       = sizeof(uint16_t) * 2;

    /// @brief 接線頂点属性フォーマット
    static const ::nn::gfx::AttributeFormat TangentFormat  
        = ::nn::gfx::AttributeFormat_8_8_8_8_Snorm;
    /// @brief 接線頂点属性オフセット
    static const size_t                   TangentOffset  = 0;
    /// @brief 接線頂点属性ストライド
    static const size_t                   TangentStride  = 4;

    /// @brief パラメータ頂点属性フォーマット
    /// @details パラメータ頂点属性の要素について
    ///          @n r = ブリン・異方性鏡面反射の割合(1.0=ブリン、0.0=異方性)
    ///          @n g = 鏡面反射の有効割合(0.0=鏡面反射なし、1.0=鏡面反射あり)
    ///          @n b = 未使用
    ///          @n a = リムライトの幅(0.0～1.0)
    static const ::nn::gfx::AttributeFormat ParamFormat    
        = ::nn::gfx::AttributeFormat_8_8_8_8_Unorm;
    /// @brief パラメータ頂点属性オフセット
    static const size_t                   ParamOffset    = 0;
    /// @brief パラメータ頂点属性ストライド
    static const size_t                   ParamStride    = 4;
    
    /// @brief インデックスバッファフォーマット
    static const ::nn::gfx::IndexFormat IndexFormat 
        = ::nn::gfx::IndexFormat_Uint16;

    /// @brief プリミティブトポロジの種類
    static const auto PrimitiveTopologyType = nn::gfx::PrimitiveTopologyType_Triangle;

    /// @brief プリミティブトポロジ
    static const auto PrimitiveTopology = nn::gfx::PrimitiveTopology_TriangleList;


    /// @brief 定数カラーの最大数
    static const size_t ConstantColorNum = 3;

public:
    /**
     * @brief コンストラクタです。
     *
     * @details 
     */
    DrawParam() NN_NOEXCEPT;
     /**
     * @brief デストラクタです。
     *
     * @details 
     */
    ~DrawParam() NN_NOEXCEPT;

    //! @name 判定
    //! @{
    /** 
     * @brief 指定した頂点属性が有効かを判定します。
     * @param[in] type 属性種類
     * @return 有効ならtrue
     * @details GetAttribute(attribute) != NULL と同じ判定です。
     */
    bool IsValidAttribute(AttributeType type) const NN_NOEXCEPT;

    /**
     * @brief テクスチャが有効かを判定します。
     * @return 有効ならtrue
     * @details GetTexture() != NULL と同じ判定です。
     */
    bool IsValidTexture() const NN_NOEXCEPT;
    //! @}

    //! @name 取得
    //! @{

    /**
     * @brief 変調方法取得
     * @return 変調方式
     * @attention この種類により、頂点属性、テクスチャ、定数カラーの有無が決まります。
     *            @n IsValidAttribute(),IsValidTexture()を使わずに
     *            @n このパラメータだけでハンドリングしていただいて問題ありません。
     *            @n ただし、Tangent,Paramは髪型にのみ含まれます。
     */
    ModulateType GetModulateType() const NN_NOEXCEPT;
    
    /**
     * @brief 頂点属性バッファ取得
     * @param[in] type 頂点属性種類
     * @return 存在すればそのポインタを返す。存在しなければNULL
     */
    const ::nn::gfx::Buffer* GetAttribute(AttributeType type) const NN_NOEXCEPT;

    /**
     * @brief 頂点属性バッファサイズ取得
     * @param[in] type 頂点属性種類
     * @return 頂点属性バッファのサイズ
     */
    size_t GetBufferSize(AttributeType type) const NN_NOEXCEPT;

    /**
     * @brief インデックスバッファInfo取得
     * @return 存在すればそのポインタを返す。存在しなければNULL
     */
    const ::nn::gfx::Buffer* GetIndexBuffer() const NN_NOEXCEPT;
    
    /**
     * @brief インデックス数を取得します。
     * @return インデックス数
     */
    int GetIndexCount() const NN_NOEXCEPT;
    

    /**
     * @brief テクスチャデスクリプタスロットを返す。
     @ return 存在すればテクスチャデスクリプタスロットを返します。存在しなければNULL
     */
    const ::nn::gfx::DescriptorSlot* GetTextureDescriptorSlot() const NN_NOEXCEPT;


    /**
     * @brief 定数カラーを取得します。
     * @details 指定したindexの定数カラーを取得します。
     * @return 存在すれば定数カラーを返します。存在しなければNULL
     */
    const Color3* GetConstantColor(int index) const NN_NOEXCEPT;

    /**
    * @brief カリングモードを取得します。
    * @return 設定された @ref nn::gfx::CullMode を返します。
    */
    ::nn::gfx::CullMode GetCullMode() const NN_NOEXCEPT;

    /**
     * @brief 指定した頂点属性のフォーマットを取得します。
     * @param[in] type 頂点属性
     * @return 指定した頂点属性のフォーマットを返します。
     */
    static nn::gfx::AttributeFormat GetVertexAttributeFormat(AttributeType type) NN_NOEXCEPT;

    /**
     * @brief 指定した頂点属性のストライドを取得します。
     * @param[in] type 頂点属性
     * @return 指定した頂点属性バッファのストライドを返します。
     */
    static size_t GetVertexAttributeStride(AttributeType type) NN_NOEXCEPT;

    /**
     * @brief 指定した頂点属性のストライドを取得します。
     * @param[in] type 頂点属性
     * @return 指定した頂点属性バッファのストライドを返します。
     */
    static size_t GetVertexAttributeOffset(AttributeType type) NN_NOEXCEPT;

    //! @}

    //! @internal
    //! @name 内部実装用
    //! @{
    /**
     * @internal
     * @brief 変調モード設定
     * @param[in] type 変調モード
     * @post m_Modulate = type
     */
    void SetModulate(ModulateType type) NN_NOEXCEPT;

    /**
     * @internal
     * @brief 頂点属性を設定
     * @param[in] type   頂点属性種類
     * @param[in] buffer 頂点属性バッファ
     * @param[in] size   頂点属性バッファのサイズ
     * @post m_Attribute[type] = \&buffer,m_AttributeSize[type] = size;
     */
    void SetAttribute(AttributeType type,const ::nn::gfx::Buffer& buffer,size_t size) NN_NOEXCEPT;

    /**
     * @internal
     * @brief インデックスバッファ属性を設定
     * @param[in] index インデックスバッファ
     * @param[in] indexCount   インデックの数
     * @post m_Index=&index,m_IndexCount = indexCount;
     */
    void SetIndex(const ::nn::gfx::Buffer& index, int indexCount) NN_NOEXCEPT;
    
    /**
     * @internal
     * @brief テクスチャの設定
     * @param[in] slot テクスチャデスクリプタスロット
     * @pre  slot.IsValid() == true
     * @post m_Texture = \&slot
     */
    void SetTextureDescriptor(const ::nn::gfx::DescriptorSlot& slot) NN_NOEXCEPT;

    /**
     * @internal
     * @brief 定数カラーの設定
     * @param[in] index 定数カラー番号
     * @param[in] color 定数カラー
     * @pre 0 <= index < @ref ConstantColorNum
     * @post m_ConstantColor[index] = \&color;
     */
    void SetConstantColor(int index, const Color3& color) NN_NOEXCEPT;

    /**
     * @internal
     * @brief カリングモードの設定
     * @param[in] cullMode 設定するカリングモードの種類
     * @post m_CullMode = cullMode
     */
    void SetCullMode(nn::gfx::CullMode cullMode) NN_NOEXCEPT;
    //! @}
    
private:
    ModulateType                     m_Modulate;                          //!< 変調
    const ::nn::gfx::Buffer*         m_Attribute[AttributeType_End];      //!< 頂点属性バッファ
    size_t                           m_AttributeSize[AttributeType_End];  //!< 頂点属性バッファのサイズ
    const ::nn::gfx::Buffer*         m_Index;                             //!< Indexバッファ
    int                              m_IndexCount;                        //!< Index数
    const ::nn::gfx::DescriptorSlot* m_Texture;                           //!< テクスチャのデスクリプタスロット
    const Color3*                    m_ConstantColor[ConstantColorNum];   //!< 定数カラー
    ::nn::gfx::CullMode              m_CullMode;                          //!< カリングモード
};

//! @}

}}
