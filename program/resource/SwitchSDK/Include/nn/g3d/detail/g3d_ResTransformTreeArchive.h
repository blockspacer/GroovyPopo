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
#include <nn/util/util_ResDic.h>
#include <nn/g3d/g3d_Assert.h>
#include <nn/g3d/detail/g3d_ResTransformTree.h>
#include <nn/g3d/detail/g3d_ResTransformTreeAnim.h>
#include <nn/g3d/detail/g3d_ResTransformTreeVisibilityAnim.h>

namespace nn { namespace g3d { namespace detail {

//! @brief トランスフォームツリー関連のデータをまとめた構造体です。
struct ResTransformTreeArchiveData
{
    nn::util::BinTPtr<ResTransformTree>               pTransformTreeArray; //!< トランスフォームツリー配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>               pTransformTreeDic;   //!< トランスフォームツリー配列辞書へのポインター。
    nn::util::BinTPtr<ResTransformTreeAnim>           pTransformTreeAnimArray; //!< トランスフォームツリーアニメーション配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>               pTransformTreeAnimDic;   //!< トランスフォームツリーアニメーション配列辞書へのポインター。
    nn::util::BinTPtr<ResTransformTreeVisibilityAnim> pTransformTreeVisibilityAnimArray; //!< トランスフォームツリービジビリティーアニメーション配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>               pTransformTreeVisibilityAnimDic;   //!< トランスフォームツリービジビリティーアニメーション配列辞書へのポインター。
    uint16_t                                          transformTreeCount;  //! トランスフォームツリー数。
    uint16_t                                          transformTreeAnimCount;  //! トランスフォームツリーアニメーション数。
    uint16_t                                          transformTreeVisibilityAnimCount;  //! トランスフォームツリービジビリティーアニメーション数。
    uint8_t                                           reserved[2];
};

//! @brief トランスフォームツリーアーカイブのリソースです。廃止予定です。
class ResTransformTreeArchive : public nn::util::AccessorBase< ResTransformTreeArchiveData >
{
    NN_DISALLOW_COPY(ResTransformTreeArchive);

public:

    //! @brief トランスフォームツリーの数を取得します。
    //!
    //! @return トランスフォームツリーの数を返します。
    //!
    int GetTransformTreeCount() const NN_NOEXCEPT
    {
        return ToData().transformTreeCount;
    }
    //! @brief トランスフォームツリーアニメーションの数を取得します。
    //!
    //! @return トランスフォームツリーアニメーションの数を返します。
    //!
    int GetTransformTreeAnimCount() const NN_NOEXCEPT
    {
        return ToData().transformTreeAnimCount;
    }
    //! @brief トランスフォームツリービジビリティーアニメーションの数を取得します。
    //!
    //! @return トランスフォームツリービジビリティーアニメーションの数を返します。
    //!
    int GetTransformTreeVisibilityAnimCount() const NN_NOEXCEPT
    {
        return ToData().transformTreeVisibilityAnimCount;
    }

    //----------------------------------------
    //! @name トランスフォームツリー
    //@{

    //! @brief 指定した名前のトランスフォームツリーを取得します。
    //!
    //! @param[in] name 名前
    //!
    //! @return ResTransformTree へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のトランスフォームツリーが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResTransformTree* FindTransformTree(const char* name) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pTransformTreeDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(name);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResTransformTree* pArray = ToData().pTransformTreeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のトランスフォームツリーを取得します。
    //!
    //! @param[in] name 名前
    //!
    //! @return ResTransformTree へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のトランスフォームツリーが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResTransformTree* FindTransformTree(const char* name) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pTransformTreeDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(name);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResTransformTree* pArray = ToData().pTransformTreeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前のトランスフォームツリーのインデックス取得します。
    //!
    //! @param[in] name 名前
    //!
    //! @return トランスフォームツリーのインデックスを返します。
    //!
    //! @details
    //! 指定した名前を持つトランスフォームツリーが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindTransformTreeIndex(const char* name) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pTransformTreeDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(name);
    }

    //! @brief インデックス引きでトランスフォームツリーを取得します。
    //!
    //! @param[in] elemIndex トランスフォームツリーのインデックス。
    //!
    //! @return ResTransformTree へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはトランスフォームツリーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResTransformTree* GetTransformTree(int elemIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetTransformTreeCount());
        ResTransformTree* pArray = ToData().pTransformTreeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでトランスフォームツリーを取得します。
    //!
    //! @param[in] elemIndex トランスフォームツリーのインデックス。
    //!
    //! @return ResTransformTree へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはトランスフォームツリーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResTransformTree* GetTransformTree(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetTransformTreeCount());
        const ResTransformTree* pArray = ToData().pTransformTreeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name トランスフォームツリーアニメーション
    //@{

    //! @brief 指定した名前のトランスフォームツリーアニメーションを取得します。
    //!
    //! @param[in] name 名前
    //!
    //! @return ResTransformTreeAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のトランスフォームツリーアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResTransformTreeAnim* FindTransformTreeAnim(const char* name) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pTransformTreeAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(name);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResTransformTreeAnim* pArray = ToData().pTransformTreeAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のトランスフォームツリーアニメーションを取得します。
    //!
    //! @param[in] name 名前
    //!
    //! @return ResTransformTreeAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のトランスフォームツリーアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResTransformTreeAnim* FindTransformTreeAnim(const char* name) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pTransformTreeAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(name);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResTransformTreeAnim* pArray = ToData().pTransformTreeAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前のトランスフォームツリーアニメーションのインデックス取得します。
    //!
    //! @param[in] name 名前
    //!
    //! @return トランスフォームツリーアニメーションのインデックスを返します。
    //!
    //! @details
    //! 指定した名前を持つトランスフォームツリーアニメーションが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindTransformTreeAnimIndex(const char* name) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pTransformTreeAnimDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(name);
    }

    //! @brief インデックス引きでトランスフォームツリーアニメーションを取得します。
    //!
    //! @param[in] elemIndex トランスフォームツリーアニメーションのインデックス。
    //!
    //! @return ResTransformTreeAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはトランスフォームツリーアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResTransformTreeAnim* GetTransformTreeAnim(int elemIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetTransformTreeAnimCount());
        ResTransformTreeAnim* pArray = ToData().pTransformTreeAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでトランスフォームツリーアニメーションを取得します。
    //!
    //! @param[in] elemIndex トランスフォームツリーアニメーションのインデックス。
    //!
    //! @return ResTransformTreeAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはトランスフォームツリーアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResTransformTreeAnim* GetTransformTreeAnim(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetTransformTreeAnimCount());
        const ResTransformTreeAnim* pArray = ToData().pTransformTreeAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name トランスフォームツリービジビリティーアニメーション
    //@{

    //! @brief 指定した名前のトランスフォームツリービジビリティーアニメーションを取得します。
    //!
    //! @param[in] name 名前
    //!
    //! @return ResTransformTreeVisibilityAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のトランスフォームツリービジビリティーアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResTransformTreeVisibilityAnim* FindTransformTreeVisibilityAnim(const char* name) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pTransformTreeVisibilityAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(name);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResTransformTreeVisibilityAnim* pArray = ToData().pTransformTreeVisibilityAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のトランスフォームツリービジビリティーアニメーションを取得します。
    //!
    //! @param[in] name 名前
    //!
    //! @return ResTransformTreeVisibilityAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のトランスフォームツリービジビリティーアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResTransformTreeVisibilityAnim* FindTransformTreeVisibilityAnim(const char* name) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pTransformTreeVisibilityAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(name);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResTransformTreeVisibilityAnim* pArray = ToData().pTransformTreeVisibilityAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前のトランスフォームツリービジビリティーアニメーションのインデックス取得します。
    //!
    //! @param[in] name 名前
    //!
    //! @return トランスフォームツリービジビリティーアニメーションのインデックスを返します。
    //!
    //! @details
    //! 指定した名前を持つトランスフォームツリービジビリティーアニメーションが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindTransformTreeVisibilityAnimIndex(const char* name) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pTransformTreeVisibilityAnimDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(name);
    }

    //! @brief インデックス引きでトランスフォームツリービジビリティーアニメーションを取得します。
    //!
    //! @param[in] elemIndex トランスフォームツリービジビリティーアニメーションのインデックス。
    //!
    //! @return ResTransformTreeVisibilityAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはトランスフォームツリービジビリティーアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResTransformTreeVisibilityAnim* GetTransformTreeVisibilityAnim(int elemIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetTransformTreeVisibilityAnimCount());
        ResTransformTreeVisibilityAnim* pArray = ToData().pTransformTreeVisibilityAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでトランスフォームツリービジビリティーアニメーションを取得します。
    //!
    //! @param[in] elemIndex トランスフォームツリービジビリティーアニメーションのインデックス。
    //!
    //! @return ResTransformTreeVisibilityAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはトランスフォームツリービジビリティーアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResTransformTreeVisibilityAnim* GetTransformTreeVisibilityAnim(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetTransformTreeVisibilityAnimCount());
        const ResTransformTreeVisibilityAnim* pArray = ToData().pTransformTreeVisibilityAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}
};

}}} // namespace nn::g3d::detail
