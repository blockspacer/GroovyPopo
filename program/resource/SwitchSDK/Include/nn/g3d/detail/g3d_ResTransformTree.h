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
#include <nn/g3d/detail/g3d_MatrixUtility.h>
#include <nn/g3d/g3d_Assert.h>

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_SHADOW

namespace nn { namespace g3d { namespace detail {

//! @brief トランスフォームノードデータの構造体です。
struct ResTransformNodeData
{
    nn::util::BinPtrToString pName;    //!< トランスフォームノードの名前

    Bit32                flag;          //!< フラグ
    uint16_t             index;         //!< トランスフォーム配列中のインデックス
    uint16_t             parentIndex;   //!< 親トランスフォームノードのインデックス
    uint16_t             bindingOffset; //!< トランスフォームツリー内の自身のバインド情報へのインデックスオフセット
    uint16_t             bindingCount;  //!< トランスフォームノード内のバインド情報数

    nn::util::Float3     scale;         //!< スケール値
    union                               //!< 回転値
    {
        nn::util::Float4 quat;          //!< クォータニオン
        nn::util::Float3 euler;         //!< オイラー角
    } rotate;
    nn::util::Float3     translate;     //!< 移動値
    uint8_t              reserved[4];
};

//! @brief トランスフォームノードのリソースです。
class ResTransformNode : public nn::util::AccessorBase< ResTransformNodeData >
{
    NN_DISALLOW_COPY(ResTransformNode);

public:
    //! @brief 無効なトランスフォームノードインデックスを示す値です。
    static const int InvalidTransformNodeIndex = 0xFFFF;

    //! @briefprivate
    // 分かりやすいようにオフセットは ResBone と合わせている
    enum Shift
    {
        Shift_Rot = 12,
        Shift_Transform = 23
    };

    //! @briefprivate トランスフォームノードに関するフラグです。
    enum Flag
    {
        Flag_Visibility = 0x1 << 0, //!< @brief ビジビリティの初期値です。

        // トランスフォーム
        Flag_SegmentScaleCompensate = 0x1 << Shift_Transform, //!< MayaSSC で計算します。
        Flag_ScaleUniform           = 0x2 << Shift_Transform, //!< Scale が等方です。
        Flag_ScaleVolumeOne         = 0x4 << Shift_Transform, //!< Scale の体積が１です。
        Flag_RotateZero             = 0x8 << Shift_Transform, //!< Rotate が 0 です。
        Flag_TranslateZero          = 0x10 << Shift_Transform, //!< Translate が 0 です。
        Flag_ScaleOne               = Flag_ScaleVolumeOne | Flag_ScaleUniform, //!< Scale が１です。
        Flag_RotTransZero           = Flag_RotateZero | Flag_TranslateZero, //!< Rotate, Translate が 0 です。
        Flag_Identity               = Flag_ScaleOne | Flag_RotateZero | Flag_TranslateZero, //!< トランスフォームが単位行列です。
    };

    //! @briefprivate トランスフォームノードに関する Rotate モード
    enum RotateMode
    {
        RotateMode_Quat     = 0x0 << Shift_Rot, //!< Quaternion 方式で回転を計算します。
        RotateMode_EulerXyz = 0x1 << Shift_Rot //!< Euler 角方式で回転を計算します。
    };

    //! @briefprivate
    enum Mask
    {
        Mask_Rot = 0x7 << Shift_Rot,
        Mask_Transform = Flag_SegmentScaleCompensate | Flag_Identity
    };

    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief トランスフォームノード名を取得します。
    //!
    //! @return トランスフォームノード名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }

    //! @brief トランスフォームノード配列での自身のインデックスを取得します。
    //!
    //! @return トランスフォームノード配列での自身のインデックスを返します。
    //!
    int GetIndex() const NN_NOEXCEPT
    {
        return ToData().index;
    }

    //! @brief 親トランスフォームノードのインデックスを取得します。
    //!
    //! @return 親トランスフォームノードのインデックスを返します。
    //!
    //! @details
    //! 親が存在しない場合は InvalidTransformNodeIndex を返します。
    //!
    int GetParentIndex() const NN_NOEXCEPT
    {
        return ToData().parentIndex;
    }

    //! @brief トランスフォームツリー内の自身のバインド情報へのインデックスオフセットを取得します。
    //!
    //! @return トランスフォームツリー内の自身のバインド情報へのインデックスオフセットを返します。
    //!
    //! @details
    //! GetBindingCount() == 0 の場合は InvalidTransformNodeIndex を返します。
    //!
    int GetBindingOffset() const NN_NOEXCEPT
    {
        return ToData().bindingOffset;
    }

    //! @brief トランスフォームノードのバインド情報の数を取得します。
    //!
    //! @return トランスフォームノードのバインド情報の数を返します。
    //!
    int GetBindingCount() const NN_NOEXCEPT
    {
        return ToData().bindingCount;
    }

    //! @brief トランスフォームノードのビジビリティを取得します。
    //!
    //! @return 表示状態の場合 true、非表示状態の場合 false を返します。
    //!
    bool IsVisible() const NN_NOEXCEPT
    {
        return (ToData().flag & Flag_Visibility) != 0;
    }

    //! @brief スケール値を取得します。
    //!
    //! @return スケール値への参照を返します。
    //!
    nn::util::Float3& GetScale() NN_NOEXCEPT
    {
        return ToData().scale;
    }

    //! @brief スケール値を取得します。
    //!
    //! @return スケール値への参照を返します。
    //!
    const nn::util::Float3& GetScale() const NN_NOEXCEPT
    {
        return ToData().scale;
    }

    //! @brief 移動値を取得します。
    //!
    //! @return 移動値への参照を返します。
    //!
    nn::util::Float3& GetTranslate() NN_NOEXCEPT
    {
        return ToData().translate;
    }

    //! @brief 移動値を取得します。
    //!
    //! @return 移動値への参照を返します。
    //!
    const nn::util::Float3& GetTranslate() const NN_NOEXCEPT
    {
        return ToData().translate;
    }

    //! @brief オイラー角で回転を取得します。
    //!
    //! @return 回転値への参照を返します。
    //!
    nn::util::Float3& GetRotateEuler() NN_NOEXCEPT
    {
        //NN_SDK_REQUIRES(RotateMode_EulerXyz == GetRotateMode());
        return ToData().rotate.euler;
    }

    //! @brief オイラー角で回転を取得します。
    //!
    //! @return 回転値への参照を返します。
    //!
    const nn::util::Float3& GetRotateEuler() const NN_NOEXCEPT
    {
        //NN_SDK_REQUIRES(RotateMode_EulerXyz == GetRotateMode());
        return ToData().rotate.euler;
    }
    //@}
};

//! @brief トランスフォームツリーデータの構造体です。
struct ResTransformTreeData
{
    nn::util::BinPtrToString pName;                           //!< 名前
    nn::util::BinPtrToString pPath;                           //!< パス名
    nn::util::BinTPtr<ResTransformNode> pTransformNodeArray;  //!< トランスフォームノード配列へのポインター
    nn::util::BinTPtr<nn::util::ResDic> pTransformNodeDic;    //!< トランスフォームノード辞書へのポインター
    nn::util::BinTPtr<uint16_t>         pBindingArray;        //!< バインド情報(ObjectId と TransformNodeIndex のセット)の配列
    nn::util::BinTPtr<nn::util::ResDic> pBindingDic;          //!< バインド情報辞書へのポインター

    Bit32    flag;                                            //!< フラグ
    uint16_t transformNodeCount;                              //!< トランスフォームノード数
    uint16_t bindingCount;                                    //!< トランスフォームツリー内のバインド情報数
};

//! @brief トランスフォームツリーのリソースです。
class ResTransformTree : public nn::util::AccessorBase< ResTransformTreeData >
{
    NN_DISALLOW_COPY(ResTransformTree);

public:

    //! @briefprivate
    enum Shift
    {
        Shift_Rot = ResTransformNode::Shift_Rot
    };

    //! @briefprivate
    enum Mask
    {
        Mask_Rot = ResTransformNode::Mask_Rot,
    };

    //! @brief トランスフォームツリーに関するRotateモードです。
    enum RotateMode
    {
        RotateMode_Quat     = ResTransformNode::RotateMode_Quat, //!< クォータニオン方式で回転を計算します。
        RotateMode_EulerXyz = ResTransformNode::RotateMode_EulerXyz, //!< オイラー角方式で回転を計算します。
    };

    //----------------------------------------

    //! @name 取得/設定
    //@{

    //! @brief トランスフォームツリー名を取得します。
    //!
    //! @return トランスフォームツリー名へのポインターを返します。
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

    //@}

    //! @brief 指定したトランスフォームノード配下のトランスフォームノードツリーで最後のトランスフォームノードの次のインデックスを取得します。
    //!
    //! @param[in] transformNodeIndex トランスフォームノードのインデックス。
    //!
    //! @return 指定したトランスフォームノード配下のトランスフォームノードツリーで最後のトランスフォームノードの次のインデックスを返します。
    //!
    //! @details
    //! トランスフォームノードは深さ優先順で並んでいるため、EndIndex より小さいインデックスのトランスフォームノードを辿ることで
    //! 指定したトランスフォームノード配下のトランスフォームノードツリーを辿ることができます。
    //!
    int GetBranchEndIndex(int transformNodeIndex) const NN_NOEXCEPT
    {
        // ResTransformNode が深さ優先で格納されていることに依存する。
        // 同じ枝を辿っている間は元の親より小さなインデックスを持つことは無い。
        int transformNodeCount = GetTransformNodeCount();
        int branchParentIndex = GetTransformNode(transformNodeIndex)->GetParentIndex();
        if (transformNodeIndex == 0)
        {
            // ルートノードは全てのノードを辿るので、branchParentIndex = -1。
            branchParentIndex = -1;
        }
        for (++transformNodeIndex; transformNodeIndex < transformNodeCount; ++transformNodeIndex)
        {
            if (GetTransformNode(transformNodeIndex)->GetParentIndex() <= branchParentIndex)
            {
                return transformNodeIndex;
            }
        }
        return transformNodeIndex;
    }

    //----------------------------------------
    //! @name トランスフォームノード
    //@{

    //! @brief トランスフォームノードの数を取得します。
    //!
    //! @return トランスフォームノードの数を返します。
    //!
    int GetTransformNodeCount() const NN_NOEXCEPT
    {
        return ToData().transformNodeCount;
    }

    //! @brief 指定した名前のトランスフォームノードを取得します。
    //!
    //! @param[in] transformNodeName トランスフォームノード名
    //!
    //! @return ResTransformNode へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のトランスフォームノードが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResTransformNode* FindTransformNode(const char* transformNodeName) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pTransformNodeDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(transformNodeName);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResTransformNode* pArray = ToData().pTransformNodeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のトランスフォームノードを取得します。
    //!
    //! @param[in] transformNodeName トランスフォームノード名
    //!
    //! @return ResTransformNode へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のトランスフォームノードが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResTransformNode* FindTransformNode(const char* transformNodeName) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pTransformNodeDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(transformNodeName);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResTransformNode* pArray = ToData().pTransformNodeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前のトランスフォームノードのインデックスを取得します。
    //!
    //! @param[in] transformNodeName トランスフォームノード名
    //!
    //! @return トランスフォームノードのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のトランスフォームノードが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindTransformNodeIndex(const char* transformNodeName) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pTransformNodeDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(transformNodeName);
    }

    //! @brief インデックス引きでトランスフォームノードを取得します。
    //!
    //! @param[in] elemIndex トランスフォームノードのインデックス。
    //!
    //! @return ResTransformNode へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはトランスフォームノードの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResTransformNode* GetTransformNode(int elemIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetTransformNodeCount());
        ResTransformNode* pArray = ToData().pTransformNodeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでトランスフォームノードを取得します。
    //!
    //! @param[in] elemIndex トランスフォームノードのインデックス。
    //!
    //! @return ResTransformNode へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはトランスフォームノードの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResTransformNode* GetTransformNode(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetTransformNodeCount());
        const ResTransformNode* pArray = ToData().pTransformNodeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name バインド情報
    //@{

    //! @brief バインド情報の数を取得します。
    //!
    //! @return バインド情報の数を返します。
    //!
    int GetBindingCount() const NN_NOEXCEPT
    {
        return ToData().bindingCount;
    }

    //! @brief 指定したオブジェクトID がバインドされているトランスフォームノードを取得します。
    //!
    //! @param[in] objectId オブジェクトID
    //!
    //! @return ResTransformNode へのポインターを返します。
    //!
    //! @details
    //! 指定したオブジェクトID がバインドされたトランスフォームノードが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResTransformNode* FindTransformNodeFromObjectId(const char* objectId) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pBindingDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(objectId);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResTransformNode* pArray = ToData().pTransformNodeArray.Get();
        const uint16_t* pIndexArray = ToData().pBindingArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[pIndexArray[index]];
    }
    //! @brief 指定したオブジェクトID がバインドされているトランスフォームノードを取得します。
    //!
    //! @param[in] objectId オブジェクトID
    //!
    //! @return ResTransformNode へのポインターを返します。
    //!
    //! @details
    //! 指定したオブジェクトID がバインドされたトランスフォームノードが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResTransformNode* FindTransformNodeFromObjectId(const char* objectId) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pBindingDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(objectId);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResTransformNode* pArray = ToData().pTransformNodeArray.Get();
        const uint16_t* pIndexArray = ToData().pBindingArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[pIndexArray[index]];
    }
    //! @brief 指定したオブジェクトID がバインドされているトランスフォームノードのインデックスを取得します。
    //!
    //! @param[in] objectId オブジェクトID
    //!
    //! @return トランスフォームノードのインデックスを返します。
    //!
    //! @details
    //! 指定したオブジェクトID がバインドされたトランスフォームノードが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindTransformNodeIndexFromObjectId(const char* objectId) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pBindingDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        int index = pDic->FindIndex(objectId);
        if (index == nn::util::ResDic::Npos)
        {
            return nn::util::ResDic::Npos;
        }
        const uint16_t* pIndexArray = ToData().pBindingArray.Get();
        return pIndexArray[index];
    }

    //! @brief 配下のトランスフォームノードにバインドされている ObjectId を取得します。
    //!
    //! @param[in] bindingIndex バインド情報のインデックス。
    //!
    //! @return オブジェクトID
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //! 0 <= bindingIndex < ResTransformTree::GetBindingCount()
    //!
    const char* GetObjectId(int bindingIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(bindingIndex, 0, GetBindingCount());
        const nn::util::ResDic* pDic = ToData().pBindingDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        return pDic->GetKey(bindingIndex).data();
    }

    //! @brief 指定したトランスフォームノードにバインドされている ObjectId を取得します。
    //!
    //! @param[in] transformNodeIndex トランスフォームノードのインデックス。
    //! @param[in] bindingIndex トランスフォームノード内のバインド情報のインデックス。
    //!
    //! @return オブジェクトID
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //! 0 <= bindingIndex < ResTransformNode::GetBindingCount()
    //!
    const char* GetObjectId(int transformNodeIndex, int bindingIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(transformNodeIndex, 0, GetTransformNodeCount());
        const nn::g3d::detail::ResTransformNode* pResTransformNode = GetTransformNode(transformNodeIndex);
        NN_SDK_REQUIRES_RANGE(bindingIndex, 0, pResTransformNode->GetBindingCount());
        const nn::util::ResDic* pDic = ToData().pBindingDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        return pDic->GetKey(pResTransformNode->GetBindingOffset() + bindingIndex).data();
    }

    //@}
};

}}} // namespace nn::g3d::detail

NN_PRAGMA_POP_WARNINGS
