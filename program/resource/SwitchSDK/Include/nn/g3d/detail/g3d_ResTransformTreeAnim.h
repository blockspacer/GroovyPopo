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
* @brief トランスフォームツリーアニメーションに関するリソースクラスの宣言
*/

#pragma once

#include <nn/util/util_ResDic.h>
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

//! @brief トランスフォームノードアニメーションの結果を格納する構造体です。
struct TransformNodeAnimResult
{
    Bit32 flag; //!< フラグ。
    nn::util::Float3  scale; //!< スケール値。
    nn::util::Float3  translate; //!< 移動値。
    uint8_t reserved[4];
    union
    {
        nn::util::Float3 euler; //!< オイラー角。
        nn::util::Float4 quat; //!< クォータニオン。
    };
};

//--------------------------------------------------------------------------------------------------

//! @brief トランスフォームノードアニメーションの構造体です。
struct ResTransformNodeAnimData
{
    nn::util::BinPtrToString                pName;           //!< トランスフォームノードアニメーション名。
    nn::util::BinTPtr<ResAnimCurve>         pCurveArray;     //!< アニメーションカーブ配列へのポインター。
    nn::util::BinTPtr<float>                pBaseValueArray; //!< 初期値配列へのポインター。初期値が存在する場合は SRT 順に格納される。
    nn::util::BinTPtr<nn::gfx::ResUserData> pUserDataArray;  //!< ユーザーデータ配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>     pUserDataDic;    //!< ユーザーデータ辞書へのポインター。

    Bit32     flag; //!< フラグ。
    uint8_t   beginRotate; //!< 最初の回転カーブのインデックス。
    uint8_t   beginTranslate; //!< 最初の移動カーブのインデックス。
    uint8_t   curveCount; //!< アニメーションカーブ数。
    uint8_t   beginBaseTranslate; //!< pBaseValueArray 内の Translate の初期値へのオフセット
    int32_t   beginCurve; //!< TransformTreeAnim 内での先頭カーブのインデックス
    uint16_t  userDataCount; //!< ユーザーデータ数。
    uint8_t   reserved[2];
};

//! @brief トランスフォームノードアニメーションのリソースです。
class ResTransformNodeAnim : public nn::util::AccessorBase< ResTransformNodeAnimData >
{
    NN_DISALLOW_COPY(ResTransformNodeAnim);

public:
    //! @brief トランスフォームノードアニメーションに関するフラグです。
    enum Flag
    {
        // フラグが立っている場合は初期値が存在する
        // カーブによって上書きされないものはコンスタントとなる
        Flag_BaseScale              = 0x1 << 3, //!< Scale の初期値が存在します。
        Flag_BaseRotate             = 0x1 << 4, //!< Rotate の初期値が存在します。
        Flag_BaseTranslate          = 0x1 << 5, //!< Translate の初期値が存在します。
        Flag_CurveScaleX            = 0x1 << 6, //!< ScaleX のカーブが存在します。
        Flag_CurveScaleY            = 0x1 << 7, //!< ScaleY のカーブが存在します。
        Flag_CurveScaleZ            = 0x1 << 8, //!< ScaleZ のカーブが存在します。
        Flag_CurveRotateX           = 0x1 << 9, //!< RotateX のカーブが存在します。
        Flag_CurveRotateY           = 0x1 << 10, //!< RotateY のカーブが存在します。
        Flag_CurveRotateZ           = 0x1 << 11, //!< RotateZ のカーブが存在します。
        Flag_CurveRotateW           = 0x1 << 12, //!< RotateW のカーブが存在します。
        Flag_CurveTranslateX        = 0x1 << 13, //!< TranslateX のカーブが存在します。
        Flag_CurveTranslateY        = 0x1 << 14, //!< TranslateY のカーブが存在します。
        Flag_CurveTranslateZ        = 0x1 << 15, //!< TranslateZ のカーブが存在します。

        // 以下のフラグは LocalMtx の flag にコピーされる
        Flag_SegmentScaleCompensate = ResTransformNode::Flag_SegmentScaleCompensate, //!< MayaSSC で計算します。
        Flag_ScaleUniform           = ResTransformNode::Flag_ScaleUniform, //!< Scale が等方です。
        Flag_ScaleVolumeOne         = ResTransformNode::Flag_ScaleVolumeOne, //!< Scale の体積が 1 です。
        Flag_RotateZero             = ResTransformNode::Flag_RotateZero, //!< Rotate が 0 です。
        Flag_TranslateZero          = ResTransformNode::Flag_TranslateZero, //!< Translate が 0 です。
        Flag_ScaleOne               = ResTransformNode::Flag_ScaleOne, //!< Scale が 1 です。
        Flag_RotTransZero           = ResTransformNode::Flag_RotTransZero, //!< Rotate, Translate が 0 です。
        Flag_Identity               = ResTransformNode::Flag_Identity, //!< トランスフォームが単位行列です。
    };

    //! @briefprivate
    enum Mask
    {
        Mask_Base           = Flag_BaseScale | Flag_BaseRotate | Flag_BaseTranslate,
        Mask_CurveScale     = Flag_CurveScaleX | Flag_CurveScaleY | Flag_CurveScaleZ,
        Mask_CurveRotate    = Flag_CurveRotateX | Flag_CurveRotateY | Flag_CurveRotateZ | Flag_CurveRotateW,
        Mask_CurveTranslate = Flag_CurveTranslateX | Flag_CurveTranslateY | Flag_CurveTranslateZ,
        Mask_Curve          = Mask_CurveScale | Mask_CurveRotate | Mask_CurveTranslate,
        Mask_TransForm      = ResTransformNode::Mask_Transform
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief 指定したトランスフォームノードのアニメーションの初期化を行います。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //! @param[in] pTransformNode トランスフォームノードへのポインター。
    //!
    //! @details
    //! アニメーションがスケール、回転、移動の初期値を持たない場合、pTransformNode で示されるトランスフォームノードのスケール、回転、移動を使用して初期化します。
    //! アニメーションがスケール、回転、移動の初期値を持たず、かつ pTransformNode が NULL の場合、スケール 1、回転 0、移動 0 で初期化します。
    //!
    void Initialize(TransformNodeAnimResult* pResult, const ResTransformNode* pTransformNode) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 評価
    //@{

    //! @brief アニメーションを評価します。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //! @param[in] frame フレーム位置。
    //!
    void Evaluate(TransformNodeAnimResult* pResult, float frame) const NN_NOEXCEPT;

    //! @brief アニメーションを評価します。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //! @param[in] frame フレーム位置。
    //! @param[in] pFrameCache 計算の効率化のために使用されるキャッシュへのポインター。
    //!
    void Evaluate(TransformNodeAnimResult* pResult, float frame, AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief トランスフォームノード名を取得します
    //!
    //! @return トランスフォームノード名へのポインター。
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
    const nn::gfx::ResUserData* GetUserData(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        const nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief トランスフォームツリーのアニメーションの構造体です。
struct ResTransformTreeAnimData
{
    nn::util::BinPtrToString                pName; //!< 名前。
    nn::util::BinPtrToString                pPath; //!< パス名。
    nn::util::BinTPtr<ResTransformTree>     pBindTransformTree; //!< 結び付けたトランスフォームツリーへのポインター。
    nn::util::BinTPtr<uint16_t>             pBindIndexArray; //!< 結び付けたトランスフォームノードのインデックス。
    nn::util::BinTPtr<ResTransformNodeAnim> pTransformNodeAnimArray; //!< トランスフォームノードアニメーション配列へのポインター。
    nn::util::BinTPtr<nn::gfx::ResUserData> pUserDataArray; //!< ユーザーデータ配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>     pUserDataDic; //!< ユーザーデータ辞書へのポインター。

    Bit32    flag; //!< フラグ。
    int32_t  frameCount; //!< フレーム数。
    int32_t  curveCount; //!< アニメーションカーブ数。
    uint32_t bakedSize; //!< コマ化に必要なバッファーサイズ。
    uint16_t transformNodeAnimCount; //!< 内包するトランスフォームノードアニメーション数。
    uint16_t userDataCount; //!< ユーザーデータ数。
    uint8_t  reserved[4];
};

//! @brief トランスフォームツリーのアニメーション
class ResTransformTreeAnim : public nn::util::AccessorBase< ResTransformTreeAnimData >
{
    NN_DISALLOW_COPY(ResTransformTreeAnim);

public:

    //! @briefprivate
    enum Shift
    {
        Shift_Rot   = ResTransformTree::Shift_Rot
    };

    //! @briefprivate
    enum Mask
    {
        Mask_Rot       = ResTransformTree::Mask_Rot,
    };

    //! @brief トランスフォームツリーアニメーションに関するフラグです。
    enum Flag
    {
        Flag_CurveBaked         = AnimFlag_CurveBaked, //!< @brief カーブがベイク済みです。
        Flag_PlayPolicyLoop     = AnimFlag_PlayPolicyLoop, //!< @brief ループを行います。
    };

    //! @brief トランスフォームツリーアニメーションに関する Rotate モードです。
    enum RotateMode
    {
        RotateMode_EulerXyz = ResTransformTree::RotateMode_EulerXyz, //!< オイラー角方式で回転を計算します。
        RotateMode_Quat     = ResTransformTree::RotateMode_Quat //!< クォータニオン方式で回転を計算します。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief 事前にトランスフォームツリーと関連付けを行います。
    //!
    //! @param[in] pTransformTree ResTransformTree へのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @details
    //! リソース同士を PreBind() によって事前に関連付けることにより、
    //! TransformTreeAnimObj::BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    BindResult PreBind(const ResTransformTree* pTransformTree) NN_NOEXCEPT;

    //! @brief トランスフォームツリーとの関連付けが成功するか、チェックのみを行います。
    //!
    //! @param[in] pTransformTree ResTransformTree へのポインター。
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
    //! @return BakeCurve() 時に渡したバッファーへのポインターを返します。
    //!
    void* ResetCurve() NN_NOEXCEPT;

    //! @brief アニメーションをリセットします。
    void Reset() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/破棄
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

    //! @brief 回転の計算方式を取得します。
    //!
    //! @return 回転の計算方式を返します。
    //!
    int GetRotateMode() const NN_NOEXCEPT
    {
        return ToData().flag & Mask_Rot;
    }

    //! @brief 内包するアニメーションカーブの総数を取得します。
    //!
    //! @return 内包するアニメーションカーブの総数を返します。
    //!
    int GetCurveCount() const NN_NOEXCEPT
    {
        return ToData().curveCount;
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

    //! @brief 内包するトランスフォームノードアニメーションの総数を取得します。
    //!
    //! @return 内包するトランスフォームノードアニメーションの総数を返します。
    //!
    int GetTransformNodeAnimCount() const NN_NOEXCEPT
    {
        return ToData().transformNodeAnimCount;
    }
    //! @brief インデックス引きでトランスフォームノードアニメーションを取得します。
    //!
    //! @param[in] elemIndex トランスフォームノードアニメーションのインデックス。
    //!
    //! @return ResTransformNodeAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはトランスフォームノードアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResTransformNodeAnim* GetTransformNodeAnim(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetTransformNodeAnimCount(), GetName());
        ResTransformNodeAnim* pArray = ToData().pTransformNodeAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでトランスフォームノードアニメーションを取得します。
    //!
    //! @param[in] elemIndex トランスフォームノードアニメーションのインデックス。
    //!
    //! @return ResTransformNodeAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはトランスフォームノードアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResTransformNodeAnim* GetTransformNodeAnim(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetTransformNodeAnimCount(), GetName());
        const ResTransformNodeAnim* pArray = ToData().pTransformNodeAnimArray.Get();
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
    //! @return nn::gfx::ResUserDataへのポインターを返します。
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

}}} // namespace nn::g3d::detail

NN_PRAGMA_POP_WARNINGS
