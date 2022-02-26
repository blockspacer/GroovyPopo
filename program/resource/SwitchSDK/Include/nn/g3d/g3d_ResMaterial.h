/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
* @file
* @brief マテリアルに関するリソースクラスの宣言
*/

#pragma once

#include <nn/gfx.h>
#include <nn/util/util_ResDic.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResCommon.h>
#include <nn/gfx/gfx_ResUserData.h>
#include <nn/g3d/g3d_Binding.h>
#include <nn/g3d/detail/g3d_MatrixUtility.h>

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_SHADOW
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS
#endif

namespace nn { namespace g3d {

class ResFile;

//! @brief シェーダーパラメーター nn::g3d::ResShaderParam::Type_Srt2d で用いられる型です。
struct Srt2d
{
    float sx; //!< x 方向のスケール。
    float sy; //!< y 方向のスケール。
    float r; //!< ラジアン表現による回転。
    float tx; //!< x 方向の移動。
    float ty; //!< y 方向の移動。
};

//! @brief シェーダーパラメーター nn::g3d::ResShaderParam::Type_Srt3d で用いられる型です。
struct Srt3d
{
    nn::util::Float3 scale; //!< スケール。
    nn::util::Float3 rotate; //!< ラジアン表現による回転。
    nn::util::Float3 translate; //!< 移動。
};

//! @brief シェーダーパラメーター nn::g3d::ResShaderParam::Type_Texsrt で用いられる型です。
struct TexSrt
{
    //! @brief テクスチャー行列の計算方式。
    enum Mode
    {
        Mode_Maya, //!< Maya モード。
        Mode_3dsmax, //!< 3ds Max モード。
        Mode_Softimage, //!< Softimage モード。
        Mode_Num
    };

    Mode  mode; //!< テクスチャー行列の計算方式。
    float sx; //!< x 方向のスケール。
    float sy; //!< y 方向のスケール。
    float r; //!< ラジアン表現による回転。
    float tx; //!< x 方向の移動。
    float ty; //!< y 方向の移動。
};

// TexSrtEx のデータです。
struct TexSrtEx
{
    TexSrt srt;
    union
    {
        // ToDo ポインター
        nn::util::FloatColumnMajor4x3*   pEffectMtx;
    };
};

//--------------------------------------------------------------------------------------------------

//! @brief 描画情報の構造体です。
struct ResRenderInfoData
{
    nn::util::BinPtrToString pName; //!< 描画情報名。
    union //!< 描画情報を格納した配列へのポインター。
    {
        nn::util::BinTPtr<int32_t>                  intValueArray; //!< 整数配列へのポインター。
        nn::util::BinTPtr<float>                    floatValueArray; //!< 浮動小数点配列へのポインター。
        nn::util::BinTPtr<nn::util::BinPtrToString> stringArray; //!< 文字列配列へのポインター。
    };
    uint16_t arrayLength; //!< 配列長。
    uint8_t  type; //!< タイプ。
    uint8_t  reserved[5];
};

//! @brief 描画情報のリソースです。
class ResRenderInfo : public nn::util::AccessorBase< ResRenderInfoData >
{
    NN_DISALLOW_COPY( ResRenderInfo );

public:
    //! @brief データタイプを表します。
    enum Type
    {
        Type_Int, //!< @brief Int 値として表されるデータです。
        Type_Float, //!< @brief Float 値として表されるデータです。
        Type_String //!< @brief ascii の値として表されるデータです。
    };

    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief 描画情報名を取得します。
    //!
    //! @return 描画情報名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }

    //! @brief 配列長を取得します。
    //!
    //! @return 配列長を返します。
    //!
    int GetArrayLength() const NN_NOEXCEPT
    {
        return ToData().arrayLength;
    }

    //! @brief データのタイプを取得します。
    //!
    //! @return データのタイプを返します。
    //!
    Type GetType() const NN_NOEXCEPT
    {
        return static_cast<Type>(ToData().type);
    }

    //! @brief 整数値の配列としてデータを取得します。
    //!
    //! @pre
    //! - GetType() == Type_Int
    //!
    //! @return int32_t 配列の先頭ポインターを返します。
    //!
    const int32_t* GetInt() const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES(ToData().type == Type_Int, NN_G3D_RES_GET_NAME(this, GetName()));
        return ToData().intValueArray.Get();
    }

    //! @brief 整数値としてデータを取得します。
    //!
    //! @param[in] index 取得するデータのインデックス
    //!
    //! @pre
    //! - GetType() == Type_Int
    //! - index >= 0 && index < GetArrayLength()
    //!
    //! @return int32_t 配列の index で指定した要素を返します。
    //!
    int32_t GetInt( int index ) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES(ToData().type == Type_Int, NN_G3D_RES_GET_NAME(this, GetName()));
        NN_G3D_REQUIRES_RANGE(index, 0, ToData().arrayLength, NN_G3D_RES_GET_NAME(this, GetName()));
        return ToData().intValueArray.Get()[index];
    }

    //! @brief 浮動小数点数の配列としてデータを取得します。
    //!
    //!  @pre
    //!  - GetType() == Type_Float
    //!
    //! @return float 配列の先頭ポインターを返します。
    //!
    const float* GetFloat() const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES(ToData().type == Type_Float, NN_G3D_RES_GET_NAME(this, GetName()));
        return ToData().floatValueArray.Get();
    }

    //!  @brief 浮動小数点数としてデータを取得します。
    //!
    //!  @param[in] index 取得するデータのインデックス
    //!
    //!  @pre
    //!  - GetType() == Type_Float
    //!  - index >= 0 && index < GetArrayLength()
    //!
    //! @return float 配列の index で指定した要素を返します。
    //!
    float GetFloat(int index) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES(ToData().type == Type_Float, NN_G3D_RES_GET_NAME(this, GetName()));
        NN_G3D_REQUIRES_RANGE(index, 0, ToData().arrayLength, NN_G3D_RES_GET_NAME(this, GetName()));
        return ToData().floatValueArray.Get()[index];
    }

    //! @brief 文字列としてデータを取得します。
    //!
    //! @param[in] strIndex 取得するデータのインデックス
    //!
    //! @pre
    //! -GetType() == Type_String
    //! -index >= 0 && index < GetArrayLength()
    //!
    //! @return データへのポインターを返します。
    //!
    const char* GetString(int strIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES(ToData().type == Type_String,                NN_G3D_RES_GET_NAME(this, GetName()));
        NN_G3D_REQUIRES_RANGE(strIndex, 0, ToData().arrayLength, NN_G3D_RES_GET_NAME(this, GetName()));
        nn::util::BinPtrToString ptr = (ToData().stringArray.Get())[strIndex];
        return ptr.Get()->GetData();
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief シェーダー割り当て情報の構造体です。
struct ResShaderAssignData
{
    nn::util::BinPtrToString                     pShaderArchiveName; //!< シェーダーアーカイブの名前。
    nn::util::BinPtrToString                     pShadingModelName; //!< シェーディングモデルの名前。
    nn::util::BinTPtr<nn::util::BinPtrToString>  pAttrAssignArray; //!< 頂点属性割り当て情報配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>          pAttrAssignDic; //!< 頂点属性割り当て情報辞書へのポインター。
    nn::util::BinTPtr<nn::util::BinPtrToString>  pSamplerAssignArray; //!< サンプラー割り当て情報配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>          pSamplerAssignDic; //!< サンプラー割り当て情報辞書へのポインター。
    nn::util::BinTPtr<nn::util::BinPtrToString>  pShaderOptionArray; //!< シェーダーオプション値配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>          pShaderOptionDic; //!< シェーダーオプション値辞書へのポインター。
    uint32_t                                     revision;
    uint8_t                                      attrAssignCount; //!< 頂点属性割り当て情報数。
    uint8_t                                      samplerAssignCount; //!< サンプラー割り当て情報数。
    uint16_t                                     shaderOptionCount; //!< シェーダーオプション割り当て情報数。
};

//! @brief シェーダー割り当て情報です。
class ResShaderAssign : public nn::util::AccessorBase< ResShaderAssignData >
{
    NN_DISALLOW_COPY( ResShaderAssign );

public:
    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief シェーダーアーカイブ名を取得します。
    //!
    //! @return シェーダーアーカイブ名へのポインターを返します。
    //!
    const char* GetShaderArchiveName() const NN_NOEXCEPT
    {
        return ToData().pShaderArchiveName.Get()->GetData();
    }
    //! @brief シェーダーモデル名を取得します。
    //!
    //! @return シェーダーモデル名へのポインターを返します。
    //!
    const char* GetShadingModelName() const NN_NOEXCEPT
    {
        return ToData().pShadingModelName.Get()->GetData();
    }

    //@}

    //----------------------------------------
    //! @name 頂点属性
    //@{

    //! @brief attrib_id から頂点属性割り当て情報のインデックスを取得します。
    //!
    //! @param[in] str attrib_id。
    //!
    //! @return 頂点属性割り当て情報のインデックスを返します。
    //!
    //! @details
    //! attrib_id はシェーダーのアノテーションで指定される頂点属性名です。
    //! 指定した名前との頂点属性の対応が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindAttrAssignIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pAttrAssignDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief 頂点属性割り当て情報のインデックスから attrib_id を取得します。
    //!
    //! @param[in] elemIndex 頂点属性割り当て情報のインデックス。
    //!
    //! @return attrib_id へのポインターを返します。
    //!
    //! @pre
    //! -インデックスは頂点属性割り当て情報の数の範囲内。
    //!
    //! @details
    //! attrib_id はシェーダーのアノテーションで指定される頂点属性名です。
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetAttrAssignId(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE_DETAIL(elemIndex, 0, GetAttrAssignCount(), "Shading model name : %s", GetShadingModelName());
        const nn::util::ResDic* pDic = ToData().pAttrAssignDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief 頂点属性割り当て情報の数を取得します。
    //!
    //! @return 頂点属性割り当て情報の数。
    //!
    int GetAttrAssignCount() const NN_NOEXCEPT
    {
        return ToData().attrAssignCount;
    }
    //! @brief attrib_id からモデルデータにおける頂点属性名を取得します。
    //!
    //! @param[in] str attrib_id。
    //!
    //! @return 頂点属性名へのポインターを返します。
    //!
    //! @details
    //! attrib_id はシェーダーのアノテーションで指定される頂点属性名です。
    //! 指定した名前に対応する頂点属性割り当て情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const char* FindAttrAssign(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pAttrAssignDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        return GetAttrAssign(index);
    }
    //! @brief インデックスからモデルデータにおける頂点属性名を取得します。
    //!
    //! @param[in] elemIndex 頂点属性割り当て情報のインデックス。
    //!
    //! @return 頂点属性名へのポインターを返します。
    //!
    //! @pre
    //! -インデックスは頂点属性割り当て情報の数の範囲内。
    //!
    //! @details
    //! 指定した名前に対応する頂点属性割り当て情報が無い場合は NULL を返します。
    //!
    const char* GetAttrAssign(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE_DETAIL(elemIndex, 0, GetAttrAssignCount(), "Shading model name : %s", GetShadingModelName());
        const nn::util::BinPtrToString* nameArray = ToData().pAttrAssignArray.Get();
        if (nameArray == NULL)
        {
            return NULL;
        }
        return nameArray[elemIndex].Get()->GetData();
    }

    //@}

    //----------------------------------------
    //! @name サンプラー
    //@{

    //! @brief sampler_id からサンプラー割り当て情報のインデックスを取得します。
    //!
    //! @param[in] str sampler_id。
    //!
    //! @return サンプラー割り当て情報のインデックスを返します
    //!
    //! @details
    //! sampler_id はシェーダーのアノテーションで指定されるサンプラー名です。
    //! 指定した名前とのサンプラーの対応が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindSamplerAssignIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSamplerAssignDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief サンプラー割り当て情報情報のインデックスから sampler_id を取得します。
    //!
    //! @param[in] elemIndex サンプラー割り当て情報のインデックス。
    //!
    //! @return sampler_id へのポインターを返します。
    //!
    //! @pre
    //! -インデックスはサンプラー割り当て情報の数の範囲内。
    //!
    //! @details
    //! sampler_id はシェーダーのアノテーションで指定されるサンプラー名です。
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetSamplerAssignId(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE_DETAIL(elemIndex, 0, GetSamplerAssignCount(), "Shading model name : %s", GetShadingModelName());
        const nn::util::ResDic* pDic = ToData().pSamplerAssignDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief サンプラー割り当て情報の数を取得します。
    //!
    //! @return サンプラー割り当て情報の数を返します。
    //!
    int GetSamplerAssignCount() const NN_NOEXCEPT
    {
        return ToData().samplerAssignCount;
    }
    //! @brief sampler_id からモデルデータにおけるサンプラー名を取得します。
    //!
    //! @param[in] str sampler_id。
    //!
    //! @return サンプラー名へのポインターを返します。
    //!
    //! @details
    //! sampler_id はシェーダーのアノテーションで指定されるサンプラー名です。
    //! 指定した名前に対応するサンプラー割り当て情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const char* FindSamplerAssign(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSamplerAssignDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        return (ToData().pSamplerAssignArray.Get())[index].Get()->GetData();
    }
    //! @brief インデックスからモデルデータにおけるサンプラー名を取得します。
    //!
    //! @param[in] elemIndex サンプラー割り当て情報のインデックス。
    //!
    //! @return サンプラー名へのポインターを返します。
    //!
    //! @pre
    //! -インデックスはサンプラー割り当て情報の数の範囲内。
    //!
    //! @details
    //! 指定した名前に対応するサンプラー割り当て情報が無い場合は NULL を返します。
    //!
    const char* GetSamplerAssign(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE_DETAIL(elemIndex, 0, GetSamplerAssignCount(), "Shading model name : %s", GetShadingModelName());
        return (ToData().pSamplerAssignArray.Get())[elemIndex].Get()->GetData();
    }
    //@}

    //----------------------------------------
    //! @name オプション
    //@{

    //! @brief option_id からオプション割り当て情報のインデックスを取得します。
    //!
    //! @param[in] str option_id。
    //!
    //! @return オプション割り当て情報のインデックスを返します。
    //!
    //! @details
    //! option_id はシェーダーのアノテーションで指定されるオプション名です。
    //! 指定した名前とのオプションの対応が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindShaderOptionIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShaderOptionDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief オプション割り当て情報のインデックスから option_id を取得します。
    //!
    //! @param[in] elemIndex オプション割り当て情報のインデックス。
    //!
    //! @return option_id へのポインターを返します。
    //!
    //! @pre
    //! -インデックスはオプション割り当て情報の数の範囲内。
    //!
    //! @details
    //! option_id はシェーダーのアノテーションで指定されるオプション名です。
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetShaderOptionId(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE_DETAIL(elemIndex, 0, GetShaderOptionCount(), "Shading model name : %s", GetShadingModelName());
        const nn::util::ResDic* pDic = ToData().pShaderOptionDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief オプション割り当て情報の数を取得します。
    //!
    //! @return オプション割り当て情報の数を返します。
    //!
    int GetShaderOptionCount() const NN_NOEXCEPT
    {
        return ToData().shaderOptionCount;
    }
    //! @brief option_id からモデルデータにおけるオプション値を取得します。
    //!
    //! @param[in] str option_id。
    //!
    //! @return オプション値の文字列へのポインターを返します。
    //!
    //! @details
    //! option_id はシェーダーのアノテーションで指定されるオプション名です。
    //! 指定した名前に対応するオプション割り当て情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const char* FindShaderOption(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShaderOptionDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const nn::util::BinPtrToString* valueArray = ToData().pShaderOptionArray.Get();
        NN_SDK_ASSERT_NOT_NULL(valueArray);
        return valueArray[index].Get()->GetData();
    }
    //! @brief インデックスからモデルデータにおけるオプション値を取得します。
    //!
    //! @param[in] elemIndex オプション割り当て情報のインデックス。
    //!
    //! @return オプション値の文字列へのへのポインターを返します。
    //!
    //! @pre
    //! -インデックスはオプション割り当て情報の数の範囲内。
    //!
    //! @details
    //! 指定した名前に対応するオプション割り当て情報が無い場合は NULL を返します。
    //!
    const char* GetShaderOption(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE_DETAIL(elemIndex, 0, GetShaderOptionCount(), "Shading model name : %s", GetShadingModelName());
        const nn::util::BinPtrToString* valueArray = ToData().pShaderOptionArray.Get();
        NN_SDK_ASSERT_NOT_NULL(valueArray);
        return valueArray[elemIndex].Get()->GetData();
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief シェーダーパラメーターの構造体です。
struct ResShaderParamData
{
    nn::util::BinPtr         pCallback; //!< バッファー転送時に呼び出される変換コールバックへのポインター。
    nn::util::BinPtrToString pId; //!< シェーダーパラメーターのID。
    uint8_t                  type; //!< シェーダーパラメータータイプ。
    uint8_t                  srcSize; //!< 変換前データのバッファーサイズ。
    uint16_t                 srcOffset; //!< 変換前データのバッファー内オフセット。
    int32_t                  offset; //!< GPU に送られる変換後データのバッファー内オフセット。
    uint16_t                 dependedIndex; //!< 依存元パラメーターのインデックス。
    uint16_t                 dependIndex; //!< 依存先パラメーターのインデックス。
    uint8_t                  reserved[4];
};

//! @brief シェーダーパラメーターのリソースです。
class ResShaderParam : public nn::util::AccessorBase< ResShaderParamData >
{
    NN_DISALLOW_COPY( ResShaderParam );

public:
    //! @brief シェーダーパラメーターの各種タイプです。
    enum Type
    {
        Type_Bool, //!< ブール値のパラメーター。
        Type_Bool2, //!< 2 次元ブール値のパラメーター。
        Type_Bool3, //!< 3 次元ブール値のパラメーター。
        Type_Bool4, //!< 4 次元ブール値のパラメーター。
        Type_Int, //!< 整数値のパラメーター。
        Type_Int2, //!< 2 次元整数値のパラメーター。
        Type_Int3, //!< 3 次元整数値のパラメーター。
        Type_Int4, //!< 4 次元整数値のパラメーター。
        Type_Uint, //!< 符号無し整数値のパラメーター。
        Type_Uint2, //!< 2 次元符号無し整数値のパラメーター。
        Type_Uint3, //!< 3 次元符号無し整数値のパラメーター。
        Type_Uint4, //!< 4 次元符号無し整数値のパラメーター。
        Type_Float, //!< 実数値のパラメーター。
        Type_Float2, //!< 2 次元実数値のパラメーター。
        Type_Float3, //!< 3 次元実数値のパラメーター。
        Type_Float4, //!< 4 次元実数値のパラメーター。
        Type_Reserved2, // 予約。使用しません。
        Type_Float2x2, //!< 2×2 実数行列のパラメーター。
        Type_Float2x3, //!< 2×3 実数行列のパラメーター。
        Type_Float2x4, //!< 2×4 実数行列のパラメーター。
        Type_Reserved3, // 予約。使用しません。
        Type_Float3x2, //!< 3×2 実数行列のパラメーター。
        Type_Float3x3, //!< 3×3 実数行列のパラメーター。
        Type_Float3x4, //!< 3×4 実数行列のパラメーター。
        Type_Reserved4, // 予約。使用しません。
        Type_Float4x2, //!< 4×2 実数行列のパラメーター。
        Type_Float4x3, //!< 4×3 実数行列のパラメーター。
        Type_Float4x4, //!< 4×4 実数行列のパラメーター。
        Type_Srt2d, //!< XY スケール値、回転値、XY 移動値を変換して使用するパラメーター。
        Type_Srt3d, //!< XYZ スケール値、XYZ 回転値、XYZ 移動値を変換して使用するパラメーター。
        Type_Texsrt, //!< モード値、XY スケール値、回転値、XY 移動値でテクスチャー行列を生成するパラメーター。
        Type_Num
    };

    //! @brief ユニフォームブロック用のバッファーへ転送される際に呼び出される変換コールバックです。
    //!
    //! @param[out] pDst 変換結果書き込み先へのポインター。
    //! @param[in] pSrc ソースデータへのポインター。
    //! @param[in] pShaderParam シェーダーパラメーターへのポインター。
    //! @param[in] pUserPtr ユーザーポインター。
    //!
    //! @return
    //! 書き込んだデータのサイズ。
    //!
    //! @details
    //! このコールバックは nn::g3d::ModelObj::CalculateMaterial() または nn::g3d::MaterialObj::CalculateMaterial()
    //! 呼び出し時にコールバックを設定したシェーダーパラメーターが変更されていた場合、実行されます。
    //! pUserPtr には nn::g3d::MaterialObj に設定されたユーザーポインターが渡されます。
    //!
    //! コールバックを設定したシェーダーパラメーターが依存するシェーダーパラメーターを持つ場合
    //! nn::g3d::ResShaderParam::GetDependPointer() を使用することで依存するシェーダーパラメーターのポインターを取得することが可能です。
    //!
    typedef size_t(*ConvertShaderParamCallback)(void* pDst, const void* pSrc, const ResShaderParam* pShaderParam, const void *pUserPtr);

    //! @brief Id を取得します。
    //!
    //! @return Id へのポインターを返します。
    //!
    const char* GetId() const NN_NOEXCEPT
    {
        return ToData().pId.Get()->GetData();
    }

    //----------------------------------------
    //! @name 更新
    //@{

    //! @brief ソースデータを変換してユニフォームブロックへ書き込みます。
    //!
    //! @tparam swap エンディアンスワップを行うかどうか。
    //!
    //! @param[out] pDst 変換結果書き込み先へのポインター。
    //! @param[in] pSrc ソースデータへのポインター。
    //!
    template <bool swap>
    void Convert(void* pDst, const void* pSrc) const NN_NOEXCEPT;

    //! @brief ソースデータをコールバックにより変換してユニフォームブロックへ書き込みます。
    //!
    //! @tparam swap エンディアンスワップを行うかどうか。
    //!
    //! @param[out] pDst 変換結果書き込み先へのポインター。
    //! @param[in] pSrc ソースデータへのポインター。
    //! @param[in] pUserPtr コールバックに渡すユーザーポインターへのポインター。
    //!
    template <bool swap>
    void Convert(void* pDst, const void* pSrc, const void* pUserPtr) const NN_NOEXCEPT
    {
        if (pCallback.Get())
        {
            uint32_t tmpDst[16];
            size_t written;
            written = GetConvertShaderParamCallback()(tmpDst, pSrc, this, pUserPtr);
            Copy32<swap>(pDst, tmpDst, static_cast<int>((written + 3) >> 2));
        }
        else
        {
            Convert<swap>(pDst, pSrc);
        }
    }

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief シェーダーパラメーターのタイプを取得します。
    //!
    //! @return シェーダーパラメーターのタイプを返します。
    //!
    Type GetType() const NN_NOEXCEPT
    {
        return static_cast<Type>(ToData().type);
    }

    //! @brief ユニフォームブロック内でのデータ先頭へのオフセットを取得します。
    //!
    //! @return ユニフォームブロック内でのデータ先頭へのオフセットを返します。
    //!
    ptrdiff_t GetOffset() const NN_NOEXCEPT
    {
        return ToData().offset;
    }

    //! @brief ユニフォームブロック内でのデータ先頭へのオフセットを設定します。
    //!
    //! @param[in] offset ユニフォームブロック内でのデータ先頭へのオフセット。
    //!
    //! @details
    //! シェーダーでの ユニフォームブロックの定義によってオフセットは変わります。
    //! シェーダーから取得した値を設定してください。
    //!
    void SetOffset(ptrdiff_t offset) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE(offset, -1, std::numeric_limits<int32_t>::max());
        ToData().offset = static_cast<int32_t>(offset);
    }

    //! @brief パラメーターのソースデータのオフセットを取得します。
    //!
    //! @return パラメーターのソースデータのオフセットを返します。
    //!
    ptrdiff_t GetSrcOffset() const NN_NOEXCEPT
    {
        return ToData().srcOffset;
    }

    //! @brief ユニフォームブロックに書き込むサイズを取得します。
    //!
    //! @return ユニフォームブロックに書き込むサイズを返します。
    //!
    size_t GetSize() const NN_NOEXCEPT
    {
        return GetSize(GetType());
    }

    //! @brief ソースデータのサイズを取得します。
    //!
    //! @return ソースデータのサイズを返します。
    //!
    size_t GetSrcSize() const NN_NOEXCEPT
    {
        return ToData().srcSize;
    }

    //! @brief ユニフォームブロックに書き込むサイズを取得します。
    //!
    //! @param[in] type データのタイプ。
    //!
    //! @return ユニフォームブロックに書き込むサイズを返します。
    //!
    static size_t GetSize(Type type) NN_NOEXCEPT;

    //! @brief ソースデータのサイズを取得します。
    //!
    //! @param[in] type データのタイプ。
    //!
    //! @returnソースデータのサイズを返します。
    //!
    static size_t GetSrcSize(Type type) NN_NOEXCEPT;

    //! @brief 変換コールバックを取得します。
    //!
    //! @return 変換コールバックへのポインターを返します。
    //!
    ConvertShaderParamCallback GetConvertShaderParamCallback() const NN_NOEXCEPT
    {
        void* ptr = const_cast<void*>(pCallback.Get());
        return reinterpret_cast<ConvertShaderParamCallback>(ptr); // 警告対策
    }

    //! @brief 変換コールバックを設定します。
    //!
    //! @param[in] callback 変換コールバックへのポインター。
    //!
    void SetConvertShaderParamCallback(ConvertShaderParamCallback callback) NN_NOEXCEPT
    {
        ToData().pCallback.Set(reinterpret_cast<void*>(callback));
    }

    //! @brief 依存元のインデックスを取得します。
    //!
    //! @return 依存元のインデックスを返します。
    //!
    int GetDependedIndex() const NN_NOEXCEPT
    {
        return dependedIndex;
    }

    //! @brief 依存先のインデックスを取得します。
    //!
    //! @return 依存先のインデックスを返します。
    //!
    int GetDependIndex() const NN_NOEXCEPT
    {
        return dependIndex;
    }

    //! @brief シェーダー内で自身のシェーダーパラメーターを依存先に指定している場合に、依存先のパラメーターへのポインターを設定します。
    //!
    //! @param[in] pSrcShaderParam 自身のソースデータへのポインター。
    //! @param[in] pDependParam 依存先パラメーターへのポインター。
    //!
    //! @return
    //! 設定に成功した場合 true を返します。
    //!
    //! @details
    //! シェーダー内で他のシェーダーパラメーターを依存先に指定している場合は、設定に失敗して false が返ります。
    //!
    //! シェーダー内で他のシェーダーパラメーターを依存先に指定している場合は nn::g3d::MaterialObj::Builder::Build() 内で依存先ポインターが自動で設定されるため、
    //! 本関数を使用する必要はありません。
    //!
    bool SetDependPointer(void* pSrcShaderParam, const void* pDependParam) const NN_NOEXCEPT;

    //! @brief 依存するシェーダーパラメーターもしくは外部パラメーターへのポインターを取得します。
    //!
    //! @param[in] pOutParamPtr 依存するパラメーターへのポインターを受け取るポインター。
    //! @param[in] pSrcShaderParam 自身のソースデータへのポインター。
    //!
    //! @return
    //! 取得に成功した場合 true を返します。
    //!
    //! @details
    //! 依存するパラメーターが設定されていない場合は NULL を受け取ります。
    //!
    bool GetDependPointer(void** pOutParamPtr, const void* pSrcShaderParam) const NN_NOEXCEPT;


    //! SRT2D 変換用のコールバック関数です。
    //!
    //! @param[out] pDst 変換結果書き込み先へのポインター。
    //! @param[in] pSrc ソースデータへのポインター。
    //!
    //! @return 計算結果の行列のサイズを返します。
    //!
    static size_t ConvertSrt2dCallback(void* pDst, const void* pSrc, const ResShaderParam*, const void*) NN_NOEXCEPT;

    //! SRT3D 変換用のコールバック関数です。
    //!
    //! @param[out] pDst 変換結果書き込み先へのポインター。
    //! @param[in] pSrc ソースデータへのポインター。
    //!
    //! @return 計算結果の行列のサイズを返します。
    //!
    static size_t ConvertSrt3dCallback(void* pDst, const void* pSrc, const ResShaderParam*, const void*) NN_NOEXCEPT;

    //! SRT2DEx 変換用のコールバック関数です。
    //!
    //! @param[out] pDst 変換結果書き込み先へのポインター。
    //! @param[in] pSrc ソースデータへのポインター。
    //!
    //! @return 計算結果の行列のサイズを返します。
    //!
    static size_t ConvertSrt2dExCallback(void* pDst, const void* pSrc, const ResShaderParam*, const void*) NN_NOEXCEPT;

    //! TexSRT 変換用のコールバック関数です。
    //!
    //! @param[out] pDst 変換結果書き込み先へのポインター。
    //! @param[in] pSrc ソースデータへのポインター。
    //!
    //! @return 計算結果の行列のサイズを返します。
    //!
    static size_t ConvertTexSrtCallback(void* pDst, const void* pSrc, const ResShaderParam*, const void*) NN_NOEXCEPT;

    //! TexSRTEx 変換用のコールバック関数です。
    //!
    //! @param[out] pDst 変換結果書き込み先へのポインター。
    //! @param[in] pSrc ソースデータへのポインター。
    //!
    //! @return 計算結果の行列のサイズを返します。
    //!
    static size_t ConvertTexSrtExCallback(void* pDst, const void* pSrc, const ResShaderParam*, const void*) NN_NOEXCEPT;

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief マテリアルの構造体です。
struct ResMaterialData
{
    nn::util::BinBlockSignature                    signature; //!< シグネチャー。
    Bit32                                          flag; //!< フラグ
    nn::util::BinPtrToString                       pName; //!< 名前へのポインター。
    nn::util::BinTPtr<ResRenderInfo>               pRenderInfoArray; //!< 描画情報配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>            pRenderInfoDic; //!< 描画情報辞書へのポインター。
    nn::util::BinTPtr<ResShaderAssign>             pShaderAssign; //!< シェーダー割り当て情報へのポインター。
    nn::util::BinTPtr<const nn::gfx::TextureView*> pTextureArray; //!< テクスチャーへのポインター。
    nn::util::BinTPtr<nn::util::BinPtrToString>    pTextureNameArray; //!< テクスチャー名の配列へのポインター。
    nn::util::BinTPtr<nn::gfx::Sampler>            pSamplerArray; //!< サンプラー配列へのポインター。
    nn::util::BinTPtr<nn::gfx::SamplerInfo>        pSamplerInfoArray; //!< サンプラー情報配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>            pSamplerInfoDic; //!< サンプラー情報辞書へのポインター。
    nn::util::BinTPtr<ResShaderParam>              pShaderParamArray; //!< シェーダーパラメーター配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>            pShaderParamDic; //!< シェーダーパラメーター辞書へのポインター。
    nn::util::BinPtr                               pSrcParam; //!< シェーダーの変換前データのバッファー。
    nn::util::BinTPtr<nn::gfx::ResUserData>        pUserDataArray; //!< ユーザーデータ配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>            pUserDataDic; //!< ユーザーデータ辞書へのポインター。
    nn::util::BinTPtr<Bit32>                       pVolatileFlag; //!< シェーダーパラメーターのvolatile属性をもつかのフラグへのポインター。
    nn::util::BinPtr                               pUserPtr; //!< ユーザーポインター。
    nn::util::BinTPtr<nn::gfx::DescriptorSlot>     pSamplerSlotArray; //!< サンプラーのディスクリプタスロットを格納する配列へのポインター。
    nn::util::BinTPtr<nn::gfx::DescriptorSlot>     pTextureSlotArray; //!< テクスチャーのディスクリプタスロットを格納する配列へのポインター。

    uint16_t index; //!< モデル内でのインデックス
    uint16_t renderInfoCount; //!< 描画情報数。
    uint8_t  samplerCount; //!< サンプラー数。
    uint8_t  textureCount; //!< テクスチャー数。
    uint16_t shaderParamCount; //!< シェーダーパラメーター数。
    uint16_t shaderParamVolatileCount; //!< volatile属性を持つシェーダーパラメーター数。
    uint16_t srcParamSize;  //!< シェーダーパラメーターの変換前データの総サイズ。
    uint16_t rawParamSize; //!< シェーダーパラメーターの変換後データの総サイズ
    uint16_t userDataCount; //!< ユーザーデータ数。
};

//! @brief マテリアルのリソースです。
class ResMaterial : public nn::util::AccessorBase< ResMaterialData >
{
    NN_DISALLOW_COPY( ResMaterial );

public:
    //! @brief ResMaterial のシグネチャ
    static const uint32_t Signature = NN_UTIL_CREATE_SIGNATURE_4('F', 'M', 'A', 'T');

    //! @brief マテリアルに関するフラグです。
    enum Flag
    {
        Flag_Visibility = 0x1 << 0 //!< @brief ビジビリティの初期値です。
    };

    //! @brief リセット時にリセットしないものを指定するフラグです。
    enum ResetGuardFlag
    {
        ResetGuardFlag_None       = 0, //!< すべてリセットします。
        ResetGuardFlag_UserPtr    = 0x1 << 0, //!< ユーザーポインターの設定をリセットしません。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief マテリアルをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    void Setup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @brief マテリアルをクリーンアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    void Cleanup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @brief マテリアルをリセットします。
    void Reset() NN_NOEXCEPT;

    //! @brief マテリアルをリセットします。
    void Reset(Bit32 resetGuardFlag) NN_NOEXCEPT;

    //! @brief テクスチャーの参照解決を行います。
    //!
    //! @param[in] pCallback テクスチャーの参照解決時に呼び出されるコールバックへのポインター。
    //! @param[in] pUserData コールバック呼び出し時に引数で渡されるユーザーデータへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @pre
    //! - pCallback != NULL
    //!
    //! @details
    //! 引数にテクスチャー名を渡し、pCallback を呼び出します。pCallback は適切な nn::g3d::TextureRef
    //! を返してください。有効でない nn::g3d::TextureRef がコールバックの返り値として返された場合、
    //! そのテクスチャーの参照解決は失敗したと扱われます。
    //! テクスチャーの参照解決が行われないまま描画処理を行うと不適切なアクセスが発生する可能性があります。
    //! 返り値を確認し、テクスチャーの参照解決が成功したことを確認してください。
    //!
    //! 参照が解決されていないテクスチャーに対してのみ、コールバックは呼び出されます。
    //! 参照が解決済みかどうかは nn::g3d::ResMaterial::IsTextureBound() に
    //! よって判定されます。
    //!
    BindResult BindTexture(TextureBindCallback pCallback, void* pUserData) NN_NOEXCEPT;

    //! @brief 指定した名前のテクスチャーの参照を差し替えます。
    //!
    //! @param[in] textureRef 差し替えるテクスチャーのビューおよびディスクリプタスロットを格納した nn::g3d::TextureRef への参照。
    //! @param[in] name 差し替えるテクスチャーの名前。
    //!
    //! @pre
    //! - pTexture != NULL
    //! - name != NULL
    //!
    //! @return 差し替えが成功した場合 true、失敗した場合 false を返します。
    //!
    bool ForceBindTexture(const TextureRef& textureRef, const char* name) NN_NOEXCEPT;

    //! @brief テクスチャーの参照設定を解除します。
    void ReleaseTexture() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief モデル内でのインデックスを取得します。
    //!
    //! @return モデル内でのインデックスを返します。
    //!
    int GetIndex() const NN_NOEXCEPT
    {
        return static_cast<int>(ToData().index);
    }

    //! @brief マテリアル名を取得します。
    //!
    //! @return マテリアル名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }

    //! @brief ユーザーポインターを設定します。
    //!
    //! @param[in] pUserPtr ポインター。
    //!
    void SetUserPtr(void* pUserPtr) NN_NOEXCEPT
    {
        ToData().pUserPtr.Set(pUserPtr);
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    void* GetUserPtr() NN_NOEXCEPT
    {
        return ToData().pUserPtr.Get();
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    const void* GetUserPtr() const NN_NOEXCEPT
    {
        return ToData().pUserPtr.Get();
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    T* GetUserPtr()
    {
        return static_cast<T*>(GetUserPtr());
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    const T* GetUserPtr() const
    {
        return static_cast<const T*>(GetUserPtr());
    }

    //! @brief 指定したパラメーターが Volatile 属性かどうかを取得します。
    //!
    //! @return Volatile 属性の場合は true、Volatile 属性でない場合 false を返します。
    //!
    bool IsVolatile(int paramIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(paramIndex, 0, GetShaderParamCount(), NN_G3D_RES_GET_NAME(this, GetName()));
        const Bit32* paramArray = ToData().pVolatileFlag.Get();
        return (paramArray[paramIndex >> 5] & (0x1 << (paramIndex & 0x1F))) != 0;
    }

    //! @brief 指定したパラメーターに Volatile 属性を設定します。
    //!
    //! @param[in] paramIndex パラメーターのインデックス。
    //!
    void SetVolatile(int paramIndex) NN_NOEXCEPT
    {
        Bit32* paramArray = ToData().pVolatileFlag.Get();
        Bit32& flag = paramArray[paramIndex >> 5];
        Bit32 mask = (0x1 << (paramIndex & 0x1F));
        if ((flag & mask) == 0)
        {
            flag ^= mask;
            ++ToData().shaderParamVolatileCount;
        }
    }

    //! @brief 指定したパラメーターの Volatile 属性を解除します。
    //!
    //! @param[in] paramIndex パラメーターのインデックス。
    //!
    void ResetVolatile(int paramIndex) NN_NOEXCEPT
    {
        Bit32* paramArray = ToData().pVolatileFlag.Get();
        Bit32& flag = paramArray[paramIndex >> 5];
        Bit32 mask = (0x1 << (paramIndex & 0x1F));
        if ((flag & mask) != 0)
        {
            flag ^= mask;
            --ToData().shaderParamVolatileCount;
        }
    }

    //! @brief Volatile 属性のパラメーターをもっているかどうかを取得します。
    //!
    //! @return Volatile 属性のパラメーターをもっている場合は true、持っていない場合は false を返します。
    //!
    bool HasVolatile() const NN_NOEXCEPT
    {
        return ToData().shaderParamVolatileCount > 0;
    }

    //@}

    //----------------------------------------
    //! @name 描画情報
    //@{

    //! @brief 描画情報の数を取得します。
    //!
    //! @return 描画情報の数を返します。
    //!
    int GetRenderInfoCount() const NN_NOEXCEPT
    {
        return ToData().renderInfoCount;
    }
    //! @brief 指定した名前の描画情報を取得します。
    //!
    //! @param[in] str 描画情報名。
    //!
    //! @return ResRenderInfo へのポインターを返します。
    //!
    //! @details
    //! 指定した名前の描画情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResRenderInfo* FindRenderInfo(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pRenderInfoDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResRenderInfo* pArray = ToData().pRenderInfoArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前の描画情報を取得します。
    //!
    //! @param[in] str 描画情報名。
    //!
    //! @return ResRenderInfo へのポインターを返します。
    //!
    //! @details
    //! 指定した名前の描画情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResRenderInfo* FindRenderInfo(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pRenderInfoDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResRenderInfo* pArray = ToData().pRenderInfoArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前の描画情報のインデックスを取得します。
    //!
    //! @param[in] str 描画情報名。
    //!
    //! @return 描画情報のインデックスを返します。
    //!
    //! @details
    //! 指定した名前の描画情報が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindRenderInfoIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pRenderInfoDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief 描画情報のインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex 描画情報のインデックス。
    //!
    //! @return 描画情報名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは描画情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetRenderInfoName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetRenderInfoCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pRenderInfoDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きで描画情報を取得します。
    //!
    //! @param[in] elemIndex 描画情報のインデックス。
    //!
    //! @return ResRenderInfo へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは描画情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResRenderInfo* GetRenderInfo(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetRenderInfoCount(), GetName());
        ResRenderInfo* pArray = ToData().pRenderInfoArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きで描画情報を取得します。
    //!
    //! @param[in] elemIndex 描画情報のインデックス。
    //!
    //! @return ResRenderInfo へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは描画情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResRenderInfo* GetRenderInfo(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetRenderInfoCount(), GetName());
        const ResRenderInfo* pArray = ToData().pRenderInfoArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name テクスチャー
    //@{

    //! @brief インスタンスで保持するテクスチャーの数を設定します。
    void SetTextureCount(int textureCount) NN_NOEXCEPT
    {
        ToData().textureCount = static_cast<uint8_t>(textureCount);
    }

    //! @brief インスタンスで保持するテクスチャーの数を取得します。
    //!
    //! @return インスタンスで保持するテクスチャーの数を返します。
    //!
    int GetTextureCount() const NN_NOEXCEPT
    {
        return ToData().textureCount;
    }

    //! @brief テクスチャーのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex テクスチャーのインデックス。
    //!
    //! @return テクスチャー名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはテクスチャーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //! あるサンプラーが参照するテクスチャーは、そのサンプラーと同じインデックスのテクスチャーです。
    //!
    const char* GetTextureName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetTextureCount(), GetName());
        return (ToData().pTextureNameArray.Get())[elemIndex].Get()->GetData();
    }

    //! @brief テクスチャーを取得します。
    //!
    //! @param[in] texIndex テクスチャーのインデックス。
    //!
    //! @return テクスチャーへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはテクスチャーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const nn::gfx::TextureView* GetTexture(int texIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(texIndex, 0, GetTextureCount(), GetName());
        const nn::gfx::TextureView* const* pArray = ToData().pTextureArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return pArray[texIndex];
    }

    //! @brief テクスチャーを設定します。
    //!
    //! @param[in] texIndex テクスチャーのインデックス。
    //! @param[in] pTextureView 設定するテクスチャーの nn::gfx::TextureView へのポインター。
    //!
    //! @pre
    //! - インデックスはテクスチャーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //! あるサンプラーが参照するテクスチャーは、そのサンプラーと同じインデックスのテクスチャーです。
    //!
    void SetTexture(int texIndex, const nn::gfx::TextureView* pTextureView) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(texIndex, 0, GetTextureCount(), GetName());
        const nn::gfx::TextureView** pArray = ToData().pTextureArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        pArray[texIndex] = pTextureView;
    }

    //! @brief インデックスで示されるテクスチャーが登録されているディスクリプタスロットを設定します。
    //!
    //! @param[in] texIndex テクスチャーのインデックス。
    //! @param[in] descriptorSlot テクスチャーを登録したディスクリプタスロット。
    //!
    //! @pre
    //! - テクスチャーのインデックスはテクスチャー参照の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //! あるサンプラーが参照するテクスチャーは、そのサンプラーと同じインデックスのテクスチャーです。
    //!
    void SetTextureDescriptorSlot(int texIndex, const nn::gfx::DescriptorSlot& descriptorSlot) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(texIndex, 0, GetTextureCount(), GetName());
        nn::gfx::DescriptorSlot* pArray = ToData().pTextureSlotArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        pArray[texIndex] = descriptorSlot;
    }

    //! @brief インデックスで示されるテクスチャーが登録されているディスクリプタスロットを取得します。
    //!
    //! @param[in] texIndex テクスチャーのインデックス。
    //!
    //! @return ディスクリプタスロットを返します。
    //!
    //! @pre
    //! - テクスチャーのインデックスはテクスチャー参照の数の範囲内。
    //! - SetTextureDescriptorSlot もしくは BindTexture で適切なディスクリプタスロットを設定している。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //! 正しいディスクリプタスロットを取得するには SetTextureDescriptorSlot もしくは BindTexture で適切なディスクリプタスロットを
    //! 設定している必要があります。あるサンプラーが参照するテクスチャーは、そのサンプラーと同じインデックスのテクスチャーです。
    //!
    nn::gfx::DescriptorSlot GetTextureDescriptorSlot(int texIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(texIndex, 0, GetTextureCount(), GetName());
        const nn::gfx::DescriptorSlot* pArray = ToData().pTextureSlotArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return pArray[texIndex];
    }

    //! @brief テクスチャーが設定されているかどうかを取得します。
    //!
    //! @param[in] texIndex テクスチャーのインデックス。
    //!
    //! @return テクスチャーが設定されている場合は true、設定されていない場合は false を返します。
    //!
    //! @pre
    //! - インデックスはテクスチャーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //! 設定されているテクスチャービューがNULLでなく、ディスクリプタスロットが有効な場合に true になります。
    //!
    bool IsTextureBound(int texIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(texIndex, 0, GetTextureCount(), GetName());
        return (GetTexture(texIndex) != NULL && GetTextureDescriptorSlot(texIndex).IsValid());
    }

    //@}

    //----------------------------------------
    //! @name サンプラー
    //@{

    //! @brief サンプラーの数を取得します。
    //!
    //! @return サンプラーの数を返します。
    //!
    int GetSamplerCount() const NN_NOEXCEPT
    {
        return samplerCount;
    }

    //! @brief 指定した名前のサンプラーを取得します。
    //!
    //! @param[in] str サンプラー名。
    //!
    //! @return nn::gfx::Sampler へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のサンプラーが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    nn::gfx::Sampler* FindSampler(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSamplerInfoDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        nn::gfx::Sampler* pSamplerArray = ToData().pSamplerArray.Get();
        return &pSamplerArray[index];
    }

    //! @brief 指定した名前のサンプラーを取得します。
    //!
    //! @param[in] str サンプラー名。
    //!
    //! @return nn::gfx::Sampler へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のサンプラーが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const nn::gfx::Sampler* FindSampler(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSamplerInfoDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const nn::gfx::Sampler* pSamplerArray = ToData().pSamplerArray.Get();
        return &pSamplerArray[index];
    }

    //! @brief インデックス引きでサンプラーを取得します。
    //!
    //! @param[in] elemIndex サンプラーのインデックス。
    //!
    //! @return nn::gfx::Sampler へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはサンプラーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    nn::gfx::Sampler* GetSampler(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSamplerCount(), GetName());
        nn::gfx::Sampler* pSamplerArray = ToData().pSamplerArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pSamplerArray);
        return &pSamplerArray[elemIndex];
    }

    //! @brief インデックス引きでサンプラーを取得します。
    //!
    //! @param[in] elemIndex サンプラーのインデックス。
    //!
    //! @return nn::gfx::Sampler へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはサンプラーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const nn::gfx::Sampler* GetSampler(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSamplerCount(), GetName());
        const nn::gfx::Sampler* pSamplerArray = ToData().pSamplerArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pSamplerArray);
        return &pSamplerArray[elemIndex];
    }

    //! @brief サンプラーのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex サンプラーのインデックス。
    //!
    //! @return サンプラー名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはサンプラーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetSamplerName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSamplerCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pSamplerInfoDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }

    //! @brief 指定した名前のサンプラーのインデックスを取得します。
    //!
    //! @param[in] str サンプラー名。
    //!
    //! @return サンプラーのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のサンプラーが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindSamplerIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSamplerInfoDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }

    //! @brief 指定した名前のサンプラー情報を取得します。
    //!
    //! @param[in] str サンプラー名。
    //!
    //! @return nn::gfx::SamplerInfo へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のサンプラーが無い場合は NULL を返します。
    //! Setup() 前にリソースのサンプラー情報を書き換えることによって、
    //! Setup() 後に作成されるサンプラーの設定を変更できます。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    nn::gfx::SamplerInfo* FindSamplerInfo(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSamplerInfoDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        nn::gfx::SamplerInfo* pSamplerInfoArray = ToData().pSamplerInfoArray.Get();
        return &pSamplerInfoArray[index];
    }

    //! @brief 指定した名前のサンプラー情報を取得します。
    //!
    //! @param[in] str サンプラー名。
    //!
    //! @return nn::gfx::SamplerInfo へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のサンプラーが無い場合は NULL を返します。
    //! Setup() 前にリソースのサンプラー情報を書き換えることによって、
    //! Setup() 後に作成されるサンプラーの設定を変更できます。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const nn::gfx::SamplerInfo* FindSamplerInfo(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSamplerInfoDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const nn::gfx::SamplerInfo* pSamplerInfoArray = ToData().pSamplerInfoArray.Get();
        return &pSamplerInfoArray[index];
    }

    //! @brief インデックス引きでサンプラー情報を取得します。
    //!
    //! @param[in] elemIndex サンプラーのインデックス。
    //!
    //! @return nn::gfx::SamplerInfo へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはサンプラーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //! Setup() 前にリソースのサンプラー情報を書き換えることによって、
    //! Setup() 後に作成されるサンプラーの設定を変更できます。
    //!
    nn::gfx::SamplerInfo* GetSamplerInfo(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSamplerCount(), GetName());
        nn::gfx::SamplerInfo* pSamplerInfoArray = ToData().pSamplerInfoArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pSamplerInfoArray);
        return &pSamplerInfoArray[elemIndex];
    }

    //! @brief インデックス引きでサンプラー情報を取得します。
    //!
    //! @param[in] elemIndex サンプラーのインデックス。
    //!
    //! @return nn::gfx::SamplerInfo へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはサンプラーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //! Setup() 前にリソースのサンプラー情報を書き換えることによって、
    //! Setup() 後に作成されるサンプラーの設定を変更できます。
    //!
    const nn::gfx::SamplerInfo* GetSamplerInfo(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSamplerCount(), GetName());
        const nn::gfx::SamplerInfo* pSamplerInfoArray = ToData().pSamplerInfoArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pSamplerInfoArray);
        return &pSamplerInfoArray[elemIndex];
    }

    //! @brief 名前で示されるサンプラーが登録されているディスクリプタスロットを設定します。
    //!
    //! @param[in] str サンプラー名。
    //! @param[in] descriptorSlot サンプラーを登録したディスクリプタスロット。
    //!
    //! @pre
    //! - 名前が存在するサンプラー名。
    //!
    //! @details
    //! 名前指定よりインデックス指定のほうが高速に動作します。通常はインデックス指定を使用することを推奨します。
    //!
    void SetSamplerDescriptorSlot(const char* str, const nn::gfx::DescriptorSlot& descriptorSlot) NN_NOEXCEPT
    {
        int index = FindSamplerIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            NN_G3D_WARNING(index != nn::util::ResDic::Npos, "The specified sampler was not found");
            return;
        }
        SetSamplerDescriptorSlot(index, descriptorSlot);
    }

    //! @brief インデックスで示されるサンプラーが登録されているディスクリプタスロットを設定します。
    //!
    //! @param[in] elemIndex サンプラーのインデックス。
    //! @param[in] descriptorSlot サンプラーを登録したディスクリプタスロット。
    //!
    //! @pre
    //! - サンプラーのインデックスはサンプラーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    void SetSamplerDescriptorSlot(int elemIndex, const nn::gfx::DescriptorSlot& descriptorSlot) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSamplerCount(), GetName());
        nn::gfx::DescriptorSlot* pArray = ToData().pSamplerSlotArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        pArray[elemIndex] = descriptorSlot;
    }

    //! @brief サンプラー名で示されるサンプラーが登録されているディスクリプタスロットを取得します。
    //!
    //! @param[in] str サンプラー名。
    //!
    //! @return ディスクリプタスロットを返します。
    //!
    //! @details
    //! 指定した名前のサンプラーが存在しない場合は 無効な nn::gfx::DescriptorSlot を返します。
    //! nn::gfx::DescriptorSlot の IsValid() で確認出来ます。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    nn::gfx::DescriptorSlot FindSamplerDescriptorSlot(const char* str) NN_NOEXCEPT
    {
        int index = FindSamplerIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return nn::gfx::DescriptorSlot();
        }
        return GetSamplerDescriptorSlot(index);
    }

    //! @brief インデックスで示されるサンプラーが登録されているディスクリプタスロットを取得します。
    //!
    //! @param[in] elemIndex サンプラーのインデックス。
    //!
    //! @return ディスクリプタスロットを返します。
    //!
    //! @pre
    //! - サンプラーのインデックスはサンプラーの数の範囲内。
    //! - SetSamplerDescriptorSlot で適切なスロット番号を設定している。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //! 正しいディスクリプタスロットを取得するには SetSamplerDescriptorSlot で適切なディスクリプタスロット
    //! を設定している必要があります。
    //!
    nn::gfx::DescriptorSlot GetSamplerDescriptorSlot(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSamplerCount(), GetName());
        const nn::gfx::DescriptorSlot* pArray = ToData().pSamplerSlotArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return pArray[elemIndex];
    }

    //! @brief 設定されているサンプラーのディスクリプタスロットをすべてクリアします。
    void ClearSamplerDescriptorSlot() NN_NOEXCEPT
    {
        for (int index = 0; index < GetSamplerCount(); ++index)
        {
            SetSamplerDescriptorSlot(index, nn::gfx::DescriptorSlot());
        }
    }

    //@}

    //----------------------------------------
    //! @name シェーダーパラメーター
    //@{

    //! @brief シェーダーパラメーターを取得します。
    //!
    //! @param[in] paramIndex シェーダーパラメーターのインデックス。
    //!
    void* GetSrcParam(int paramIndex) NN_NOEXCEPT
    {
        return AddOffset(ToData().pSrcParam.Get(), GetShaderParam(paramIndex)->GetSrcOffset());
    }

    //! @brief シェーダーパラメーターを取得します。
    //!
    //! @param[in] paramIndex シェーダーパラメーターのインデックス。
    //!
    const void* GetSrcParam(int paramIndex) const NN_NOEXCEPT
    {
        return AddOffset(ToData().pSrcParam.Get(), GetShaderParam(paramIndex)->GetSrcOffset());
    }

    //! @brief シェーダーパラメーターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @param[in] paramIndex シェーダーパラメーターのインデックス。
    //!
    //! @return シェーダーパラメーターへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つシェーダーパラメーター数の範囲内。
    //!
    //! @details
    //! テンプレート引数を指定することで型チェックを行います。
    //! 真偽値は4バイトで格納されているため、int32_t、uint32_t などの
    //! 4バイトの型で取得する必要があります。
    //!
    template <typename T>
    T* GetSrcParam(int paramIndex) NN_NOEXCEPT
    {
        const ResShaderParam* pParam = GetShaderParam(paramIndex);
        NN_G3D_ASSERT(sizeof(T) <= pParam->GetSrcSize(), NN_G3D_RES_GET_NAME(this, GetName()));
        return AddOffset<T>(ToData().pSrcParam.Get(), pParam->GetSrcOffset());
    }

    //! @brief シェーダーパラメーターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @param[in] paramIndex シェーダーパラメーターのインデックス。
    //!
    //! @return シェーダーパラメーターへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つシェーダーパラメーター数の範囲内。
    //!
    //! @details
    //! テンプレート引数を指定することで型チェックを行います。
    //! 真偽値は4バイトで格納されているため、int32_t、uint32_t などの
    //! 4バイトの型で取得する必要があります。
    //!
    template <typename T>
    const T* GetSrcParam(int paramIndex) const NN_NOEXCEPT
    {
        const ResShaderParam* pParam = GetShaderParam(paramIndex);
        NN_G3D_ASSERT(sizeof(T) <= pParam->GetSrcSize(), NN_G3D_RES_GET_NAME(this, GetName()));
        return AddOffset<T>(ToData().pSrcParam.Get(), pParam->GetSrcOffset());
    }

    //! @brief シェーダーパラメーターのソースデータのバッファーサイズを取得します。
    //!
    //! @return シェーダーパラメーターのソースデータのバッファーサイズを返します。
    //!
    uint32_t GetSrcParamSize() const NN_NOEXCEPT
    {
        return ToData().srcParamSize;
    }

    //! @brief シェーダーパラメーターのユニフォームブロックのバッファーサイズを取得します。
    //!
    //! @return シェーダーパラメーターのユニフォームブロックのバッファーサイズを返します。
    //!
    uint32_t GetRawParamSize() const NN_NOEXCEPT
    {
        return ToData().rawParamSize;
    }

    //! @brief シェーダーパラメーターのユニフォームブロックのバッファーサイズを設定します。
    //!
    //! @param[in] size シェーダーパラメーターのユニフォームブロックのバッファーサイズ。
    //!
    //! @pre
    //! - サイズは65535以下である必要があります。
    //!
    //! @details
    //! シェーダーでのユニフォームブロックの定義によってサイズは変わります。
    //! シェーダーから取得した値を設定してください。
    //!
    void SetRawParamSize(size_t size) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(size <= std::numeric_limits<uint16_t>::max());
        ToData().rawParamSize = static_cast<uint16_t>(size);
    }

    //! @brief シェーダーパラメーターの数を取得します。
    //!
    //! @return シェーダーパラメーターの数を返します。
    //!
    int GetShaderParamCount() const NN_NOEXCEPT
    {
        return ToData().shaderParamCount;
    }

    //! @brief 指定した名前のシェーダーパラメーターを取得します。
    //!
    //! @param[in] str シェーダーパラメーター名。
    //!
    //! @return ResShaderParam へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェーダーパラメーターが無い場合は NULL を返します。
    //!
    ResShaderParam* FindShaderParam(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShaderParamDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResShaderParam* pArray = ToData().pShaderParamArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のシェーダーパラメーターを取得します。
    //!
    //! @param[in] str シェーダーパラメーター名。
    //!
    //! @return ResShaderParam へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェーダーパラメーターが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResShaderParam* FindShaderParam(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShaderParamDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResShaderParam* pArray = ToData().pShaderParamArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前のシェーダーパラメーターのインデックスを取得します。
    //!
    //! @param[in] str シェーダーパラメーター名。
    //!
    //! @return シェーダーパラメーターのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のシェーダーパラメーターが無い場合は nn::util::ResDic::Npos を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    int FindShaderParamIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShaderParamDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief シェーダーパラメーターのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex シェーダーパラメーターのインデックス。
    //!
    //! @return シェーダーパラメーター名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダーパラメーターの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetShaderParamName(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetShaderParamCount());
        const nn::util::ResDic* pDic = ToData().pShaderParamDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }

    //! @brief インデックス引きでシェーダーパラメーターを取得します。
    //!
    //! @param[in] elemIndex シェーダーパラメーターのインデックス。
    //!
    //! @return ResShaderParam へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダーパラメーターの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResShaderParam* GetShaderParam(int elemIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetShaderParamCount());
        ResShaderParam* pArray = ToData().pShaderParamArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでシェーダーパラメーターを取得します。
    //!
    //! @param[in] elemIndex シェーダーパラメーターのインデックス。
    //!
    //! @return ResShaderParam へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダーパラメーターの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResShaderParam* GetShaderParam(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetShaderParamCount());
        const ResShaderParam* pArray = ToData().pShaderParamArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name シェーダー
    //@{

    //! @brief シェーダー割り当て情報を取得します。
    //!
    //! @return ResShaderAssign へのポインターを返します。
    //!
    ResShaderAssign* GetShaderAssign() NN_NOEXCEPT
    {
        return ToData().pShaderAssign.Get();
    }
    //! @brief シェーダー割り当て情報を取得します。
    //!
    //! @return ResShaderAssign へのポインターを返します。
    //!
    const ResShaderAssign* GetShaderAssign() const NN_NOEXCEPT
    {
        return ToData().pShaderAssign.Get();
    }

    //@}

    //----------------------------------------
    //! @name ユーザーデータ
    //@{

    //! @brief ユーザーデータの数を取得します。
    //!
    //! @return ユーザーデータの数を返します。
    //!
    int GetUserDataCount() const NN_NOEXCEPT
    {
        return ToData().userDataCount;
    }
    //! @brief 指定した名前のユーザーデータを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    nn::gfx::ResUserData* FindUserData(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のユーザーデータを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const nn::gfx::ResUserData* FindUserData(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のユーザーデータのインデックスを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return ユーザーデータのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindUserDataIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief ユーザーデータのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return ユーザーデータ名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetUserDataName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでユーザーデータを取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    nn::gfx::ResUserData* GetUserData(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでユーザーデータを取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const nn::gfx::ResUserData* GetUserData(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        const nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }


    //@}
};

}} // namespace nn::g3d

NN_PRAGMA_POP_WARNINGS
