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
* @brief トランスフォームツリービジビリティアニメーションに関するリソースクラスの宣言
*/

#pragma once

#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResCommon.h>
#include <nn/g3d/g3d_ResAnimCurve.h>
#include <nn/g3d/g3d_Binding.h>
#include <nn/g3d/detail/g3d_ResTransformTree.h>

NN_PRAGMA_PUSH_WARNINGS
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS
#endif

namespace nn { namespace g3d { namespace detail {

//! @brief トランスフォームツリービジビリティアニメーションの構造体です。
struct ResTransformTreeVisibilityAnimData
{
    nn::util::BinPtrToString                    pName; //!< 名前。
    nn::util::BinPtrToString                    pPath; //!< パス名。
    nn::util::BinTPtr<ResTransformTree>         pBindTransformTree; //!< 結び付けたトランスフォームツリーへのポインター。
    nn::util::BinTPtr<uint16_t>                 pBindIndexArray; //!< 結び付けたトランスフォームノードのインデックス。
    nn::util::BinTPtr<ResAnimCurve>             pCurveArray; //!< アニメーションカーブ配列へのポインター。
    nn::util::BinTPtr<Bit32>                    pBaseValueArray; //!< 初期値配列へのポインター。
    nn::util::BinTPtr<nn::util::BinPtrToString> pNameArray; //!< 文字列配列へのポインター。
    nn::util::BinTPtr<nn::gfx::ResUserData>     pUserDataArray; //!< ユーザーデータ配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>         pUserDataDic; //!< ユーザーデータ辞書へのポインター。

    int32_t  frameCount; //!< フレーム数。
    uint32_t bakedSize; //!< コマ化に必要なバッファーサイズ。
    Bit16    flag; //!< フラグ。
    uint16_t animCount; //!< アニメーション数。
    uint16_t curveCount; //!< アニメーションカーブ数。
    uint16_t userDataCount; //!< ユーザーデータ数。
};

//! @brief トランスフォームツリービジビリティアニメーションのリソースです。
class ResTransformTreeVisibilityAnim : public nn::util::AccessorBase< ResTransformTreeVisibilityAnimData >
{
    NN_DISALLOW_COPY(ResTransformTreeVisibilityAnim);

public:

    //! @brief ビジビリティアニメーションに関するフラグです。
    enum Flag
    {
        Flag_CurveBaked     = AnimFlag_CurveBaked, //!< @brief カーブがベイク済みです。
        Flag_PlayPolicyLoop = AnimFlag_PlayPolicyLoop //!< @brief ループすることを表します。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief 事前にトランスフォームツリーと関連付けを行います。
    //!
    //! @param[in] pTransformTree トランスフォームツリーへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @details
    //! リソース同士を PreBind() によって事前に関連付けることにより、
    //! TransformTreeVisibilityAnimObj::BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    BindResult PreBind(const ResTransformTree* pTransformTree) NN_NOEXCEPT;

    //! @brief トランスフォームツリーとの関連付けが成功するか、チェックのみを行います。
    //!
    //! @param[in] pTransformTree トランスフォームツリーへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @details
    //! 対象のトランスフォームツリーに対して、アニメーションの関連付けが成功するかをチェックします。
    //! 実際の関連付け処理は行いません。
    //!
    BindResult BindCheck(const ResTransformTree* pTransformTree) const NN_NOEXCEPT;

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
    //! @return BakeCurve 時に渡したバッファーへのポインターを返します。
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
    bool IsCurveBaked() const NN_NOEXCEPT
    {
        return 0 != (ToData().flag & Flag_CurveBaked);
    }

    //! @brief トランスフォームツリービジビリティアニメーションの総数を取得します。
    //!
    //! @return トランスフォームツリービジビリティアニメーションの総数を返します。
    //!
    int GetAnimCount() const NN_NOEXCEPT
    {
        return ToData().animCount;
    }

    //! @brief フレーム数を取得します。
    //!
    //! @return フレーム数を返します。
    //!
    int GetFrameCount() const NN_NOEXCEPT
    {
        return ToData().frameCount;
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

    //! @brief 結び付けたトランスフォームツリーを取得します。
    //!
    //! @return トランスフォームツリーへのポインターを返します。
    //!
    ResTransformTree* GetBindTransformTree() NN_NOEXCEPT
    {
        return ToData().pBindTransformTree.Get();
    }
    //! @brief 結び付けたトランスフォームツリーを取得します。
    //!
    //! @return トランスフォームツリーへのポインターを返します。
    //!
    const ResTransformTree* GetBindTransformTree() const NN_NOEXCEPT
    {
        return ToData().pBindTransformTree.Get();
    }

    //! @brief 内包するアニメーションカーブの総和を取得します。
    //!
    //! @return 内包するアニメーションカーブの総和を返します。
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
    int GetUserDataIndex(const char* str) const NN_NOEXCEPT
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

}}} // namespace nn::g3d::detail

NN_PRAGMA_POP_WARNINGS
