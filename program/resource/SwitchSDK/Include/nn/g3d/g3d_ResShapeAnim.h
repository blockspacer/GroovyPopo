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
* @brief シェイプアニメーションに関するリソースクラスの宣言
*/

#pragma once

#include <nn/util/util_ResDic.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResCommon.h>
#include <nn/g3d/g3d_ResAnimCurve.h>
#include <nn/g3d/g3d_ResModel.h>
#include <nn/g3d/g3d_Binding.h>

NN_PRAGMA_PUSH_WARNINGS
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS
#endif

namespace nn { namespace g3d {

class ResFile;

//--------------------------------------------------------------------------------------------------
//! @brief キーシェイプアニメーションの要素情報です。
struct ResKeyShapeAnimInfo
{
    nn::util::BinPtrToString pName; //!< 名前。
    int8_t curveIndex; //!< ResVertexShapeAnimData 内でのアニメーションカーブのインデックス。
    int8_t subbindIndex; //!< シェイプ内でのキーシェイプのインデックス。
    uint8_t reserved[6];
};

//! @brief 頂点属性アニメーションの構造体です。
struct ResVertexShapeAnimData
{
    nn::util::BinPtrToString               pName; //!< シェイプ名。
    nn::util::BinTPtr<ResAnimCurve>        pCurveArray; //!< アニメーションカーブ配列へのポインター。
    // ベースシェイプの値は持たない
    nn::util::BinTPtr<float>               pBaseValueArray; //!< 初期値配列へのポインター。
    //  先頭はベースシェイプの情報が挿入されている
    nn::util::BinTPtr<ResKeyShapeAnimInfo> pKeyShapeAnimInfoArray; //!< キーシェイプアニメーション情報配列へのポインター。
    uint16_t curveCount; //!< アニメーションカーブ数。
    uint16_t keyShapeAnimCount; //!< キーシェイプアニメーション数。
    int32_t  beginCurve; //!< シェイプアニメーション内での先頭カーブのインデックス。
    int32_t  beginKeyShapeAnim; //!< シェイプアニメーション内でのキーシェイプのインデックス。
    uint8_t reserved[4];
};

//! @brief 頂点属性アニメーションのリソースです。
class ResVertexShapeAnim : public nn::util::AccessorBase< ResVertexShapeAnimData >
{
    NN_DISALLOW_COPY(ResVertexShapeAnim);

public:
    //----------------------------------------
    //! @name 構築/破棄
    //@{
    //! @brief 事前にシェイプと関連付けを行います。
    //!
    //! @param[in] pTarget ResShape へのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @details
    //! リソース同士を PreBind() によって事前に関連付けることにより、
    //! BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    BindResult PreBind(const ResShape* pTarget) NN_NOEXCEPT;

    //! @brief シェイプとの関連付けが成功するか、チェックのみを行います。
    //!
    //! @param[in] pTarget ResShape へのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @details
    //! 対象のシェイプに対して、アニメーションの関連付けが成功するかをチェックします。
    //! 実際の関連付け処理は行いません。
    //!
    BindResult BindCheck(const ResShape* pTarget) const NN_NOEXCEPT;

    //! @brief アニメーションの初期化を行います。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //!
    void Initialize(float* pResult) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 評価
    //@{

    //! @brief アニメーションを評価します。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //! @param[in] frame フレーム位置。
    //! @param[in] pSubBindIndex アニメーションを関連付けたキーシェイプのインデックスを格納した配列へのポインター。
    //!
    void Evaluate(float* pResult, float frame, const int8_t* pSubBindIndex) const NN_NOEXCEPT;

    //! @brief アニメーションを評価します。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //! @param[in] frame フレーム位置。
    //! @param[in] pSubBindIndex アニメーションを関連付けたキーシェイプのインデックスを格納した配列へのポインター。
    //! @param[in] pFrameCache 計算の効率化のために使用されるキャッシュへのポインター。
    //!
    void Evaluate(float* pResult, float frame, const int8_t* pSubBindIndex,
        AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief シェイプ名を取得します
    //!
    //! @return シェイプ名へのポインター。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }

    //! @brief アニメーションカーブの数を取得します。
    //!
    //! @return アニメーションカーブの数を返します。
    //!
    int GetCurveCount() const NN_NOEXCEPT
    {
        return ToData().curveCount;
    }
    //! @brief インデックス引きでアニメーションカーブを取得します。
    //!
    //! @param[in] elemIndex アニメーションカーブのインデックス。
    //!
    //! @return ResAnimCurve へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはアニメーションカーブの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResAnimCurve* GetCurve(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetCurveCount(), GetName());
        ResAnimCurve* pArray = ToData().pCurveArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでアニメーションカーブを取得します。
    //!
    //! @param[in] elemIndex アニメーションカーブのインデックス。
    //!
    //! @return ResAnimCurve へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはアニメーションカーブの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResAnimCurve* GetCurve(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetCurveCount(), GetName());
        const ResAnimCurve* pArray = ToData().pCurveArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief キーシェイプアニメーション数を取得します。
    //!
    //! @return キーシェイプアニメーション数を返します。
    //!
    int GetKeyShapeAnimCount() const NN_NOEXCEPT
    {
        return ToData().keyShapeAnimCount;
    }

    //@}

};

//--------------------------------------------------------------------------------------------------

//! @brief シェイプアニメーションの構造体です。
struct ResShapeAnimData
{
    nn::util::BinBlockSignature           signature; //!< シグネチャー。
    Bit16                                 flag; //!< フラグ。
    uint8_t                               reserved0[2];
    nn::util::BinPtrToString              pName; //!< 名前。
    nn::util::BinPtrToString              pPath; //!< パス名。
    nn::util::BinTPtr<ResModel>           pBindModel; //!< 結び付けたモデルへのポインター。
    nn::util::BinTPtr<uint16_t>           pBindIndexArray; //!< 結び付けたシェイプのインデックス。
    nn::util::BinTPtr<ResVertexShapeAnim> pVertexShapeAnimArray; //!< 頂点属性アニメーション配列へのポインター。
    nn::util::BinTPtr<nn::gfx::ResUserData> pUserDataArray; //!< ユーザーデータ配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>   pUserDataDic; //!< ユーザーデータ辞書へのポインター。
    int32_t  frameCount; //!< フレーム数。
    uint32_t bakedSize; //!< コマ化に必要なバッファーサイズ。
    uint16_t userDataCount; //!< ユーザーデータ数。
    // BindTable のサイズに影響
    uint16_t vertexShapeAnimCount; //!< 頂点属性アニメーション数。
    // SubBindTable のサイズに影響
    uint16_t keyShapeAnimCount; //!< キーシェイプアニメーション数。
    // Context のサイズに影響
    uint16_t curveCount; //!< アニメーションカーブ数。
};

//! @brief シェイプアニメーションのリソースです。
class ResShapeAnim : public nn::util::AccessorBase< ResShapeAnimData >
{
    NN_DISALLOW_COPY(ResShapeAnim);

public:
    //! @brief ResShapeAnim のシグネチャ
    static const uint32_t Signature = NN_UTIL_CREATE_SIGNATURE_4('F', 'S', 'H', 'A');

    //! @brief シェイプアニメーションに関するフラグです。
    enum Flag
    {
        Flag_CurveBaked         = AnimFlag_CurveBaked, //!< @brief カーブがベイク済みです。
        Flag_PlayPolicyLoop     = AnimFlag_PlayPolicyLoop //!< @brief ループすることを表します。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief 事前にモデルと関連付けを行います。
    //!
    //! @param[in] pModel モデルへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @details
    //! リソース同士を PreBind() によって事前に関連付けることにより、
    //! ShapeAnimObj::BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    BindResult PreBind(const ResModel* pModel) NN_NOEXCEPT;

    //! @brief モデルとの関連付けが成功するか、チェックのみを行います。
    //!
    //! @param[in] pModel モデルへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @details
    //! 対象のモデルに対して、アニメーションの関連付けが成功するかをチェックします。
    //! 実際の関連付け処理は行いません。
    //!
    BindResult BindCheck(const ResModel* pModel) const NN_NOEXCEPT;

    //! @brief アニメーションをコマ化します。
    //!
    //! @param[out] pBuffer コマ化した結果を格納するバッファーへのポインター。
    //! @param[in] bufferSize コマ化した結果を格納するバッファーへのポインター。
    //!
    //! @return コマ化が成功した場合は true、失敗した場合は false を返します。
    //!
    //! @pre
    //! - bufferSize >= GetBakedSize()
    //!
    bool BakeCurve(void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    //! @brief アニメーションをカーブに戻します。
    //!
    //! @return BakeCurve() 時に渡したバッファーへのポインターを返します。
    //!
    void* ResetCurve() NN_NOEXCEPT;

    //! @brief アニメーションをリセットします。
    void Reset() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief ループするかどうかを取得します。
    //!
    //! @return ループする場合は true、しない場合は false を返します。
    //!
    bool IsLooped() const NN_NOEXCEPT
    {
        return 0 != (ToData().flag & Flag_PlayPolicyLoop);
    }

    //! @brief カーブがベイクされているかどうかを取得します。
    //!
    //! @return ベイクされている場合は true、されていない場合は false を返します。
    //!
    bool IsCurveBaked() const
    {
        return 0 != (ToData().flag & Flag_CurveBaked);
    }

    //! @brief フレーム数を取得します。
    //!
    //! @return フレーム数を返します。
    //!
    int GetFrameCount() const NN_NOEXCEPT
    {
        return ToData().frameCount;
    }

    //! @brief 内包するキーシェイプアニメーションの総数を取得します。
    //!
    //! @return 内包するキーシェイプアニメーションの総数を返します。
    //!
    int GetKeyShapeAnimCount() const NN_NOEXCEPT
    {
        return ToData().keyShapeAnimCount;
    }

    //! @brief 内包するアニメーションカーブの総数を取得します。
    //!
    //! @return 内包するアニメーションカーブの総数を返します。
    //!
    int GetCurveCount() const NN_NOEXCEPT
    {
        return ToData().curveCount;
    }

    //! @brief コマ化に必要なバッファーサイズを取得します。
    //!
    //! @return コマ化に必要なバッファーサイズを返します。
    //!
    size_t GetBakedSize() const NN_NOEXCEPT
    {
        return ToData().bakedSize;
    }

    //! @brief アニメーション名を取得します。
    //!
    //! @return アニメーション名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }
    //! @brief パス名を取得します。
    //!
    //! @return パス名へのポインターを返します。
    //!
    const char* GetPath() const NN_NOEXCEPT
    {
        return ToData().pPath.Get()->GetData();
    }

    //! @brief 結び付けたモデルを取得します。
    //!
    //! @return モデルへのポインターを返します。
    //!
    ResModel* GetBindModel() NN_NOEXCEPT
    {
        return ToData().pBindModel.Get();
    }
    //! @brief 結び付けたモデルを取得します。
    //!
    //! @return モデルへのポインターを返します。
    //!
    const ResModel* GetBindModel() const NN_NOEXCEPT
    {
        return ToData().pBindModel.Get();
    }

    //! @brief 頂点属性アニメーションの数を取得します。
    //!
    //! @return 頂点属性アニメーションの数を返します。
    //!
    int GetVertexShapeAnimCount() const NN_NOEXCEPT
    {
        return ToData().vertexShapeAnimCount;
    }
    //! @brief インデックス引きで頂点属性アニメーションを取得します。
    //!
    //! @param[in] elemIndex 頂点属性アニメーションのインデックス。
    //!
    //! @return ResVertexShapeAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは頂点属性アニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResVertexShapeAnim* GetVertexShapeAnim(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetVertexShapeAnimCount(), GetName());
        ResVertexShapeAnim* pArray = ToData().pVertexShapeAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きで頂点属性アニメーションを取得します。
    //!
    //! @param[in] elemIndex 頂点属性アニメーションのインデックス。
    //!
    //! @return ResVertexShapeAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは頂点属性アニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResVertexShapeAnim* GetVertexShapeAnim(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetVertexShapeAnimCount(), GetName());
        const ResVertexShapeAnim* pArray = ToData().pVertexShapeAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
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
