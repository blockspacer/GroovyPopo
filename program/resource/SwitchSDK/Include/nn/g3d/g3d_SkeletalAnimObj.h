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
* @brief スケルタルアニメーションに関するインスタンスクラスの宣言
*/

#pragma once

#include <nn/util/util_MemorySplitter.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_AnimObj.h>
#include <nn/g3d/g3d_ResModel.h>
#include <nn/g3d/g3d_ResSkeletalAnim.h>

namespace nn { namespace g3d {

class SkeletonObj;
class ModelObj;

//! @brief スケルタルアニメーションインスタンスです。
class SkeletalAnimObj : public ModelAnimObj
{
public:
    class Builder;
    class BindArgument;

    //! @brief インスタンスの構築時に渡すバッファーの必要アライメントサイズです。
    enum Alignment
    {
        Alignment_Buffer = MatrixVectorAlignment //! @brief Builder に渡すバッファーの必要アライメントサイズです。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    //!
    //! 実際の構築処理は Builder で行います。
    //!
    SkeletalAnimObj() NN_NOEXCEPT
        : ModelAnimObj()
        , m_pRes(NULL)
        , m_pBoneAnimArray(NULL)
        , m_MaxBoneAnim(0)
        , m_Flag(0)
        , m_pRetargetingPreCalculateArray(NULL)
        , m_pResSkeleton(NULL)
    {
    }

    //@}

    //----------------------------------------
    //! @name 関連付け
    //@{

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pModel ResModel へのポインター。
    //!
    //! @return BindResult を返します。
    //!
    virtual BindResult Bind(const ResModel* pModel) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pModelObj ModelObj へのポインター。
    //!
    //! @return BindResult を返します。
    //!
    virtual BindResult Bind(const ModelObj* pModelObj) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pModel アニメーション対象の ResModel へのポインター。
    //!
    //! @pre
    //! - pModel と設定されている ResSkeletalAnim のリソースが ResSkeletalAnim::PreBind によって事前に関連付けられている。
    //!
    //! @details
    //! リソース同士を ResSkeletalAnim::PreBind によって事前に関連付けることにより、
    //! BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    virtual void BindFast(const ResModel* pModel) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pSkeleton アニメーション対象のスケルトンリソースへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    BindResult Bind(const ResSkeleton* pSkeleton) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] arg バインドに必要なパラメーターが設定された BindArgument
    //!
    //! @return BindResult を返します。
    //!
    //! @details BindArgument にホストスケルトンが指定されている場合はリターゲッティング情報を生成します。
    //!         BindArgument にミラーリングが有効に設定されている場合はモーションミラーリングが有効になります。
    BindResult Bind(const BindArgument& arg) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pSkeletonObj アニメーション対象のスケルトンへのポインター。
    //!
    //! @deprecated 廃止予定です。廃止予定ではない他の nn::g3d::SkeletalAnimObj::Bind() を使用して下さい。
    //!
    //! @return BindResult を返します。
    //!
    NN_DEPRECATED BindResult Bind(const SkeletonObj* pSkeletonObj) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付け、リターゲッティング情報を生成します。
    //!
    //! @param[in] pModel アニメーション対象のモデルリソースへのポインター。
    //! @param[in] pHostModel 元となるモデルリソースへのポインター。
    //!
    //! @deprecated 廃止予定です。BindArgument を引数に持つ nn::g3d::SkeletalAnimObj::Bind() を使用して下さい。
    //!
    //! @return BindResult を返します。
    //!
    NN_DEPRECATED BindResult Bind(const ResModel* pModel, const ResModel* pHostModel) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付け、リターゲッティング情報を生成します。
    //!
    //! @param[in] pModelObj アニメーション対象のモデルへのポインター。
    //! @param[in] pHostModelObj 元となるモデルへのポインター。
    //!
    //! @deprecated 廃止予定です。BindArgument を引数に持つ nn::g3d::SkeletalAnimObj::Bind() を使用して下さい。
    //!
    //! @return BindResult を返します。
    //!
    NN_DEPRECATED BindResult Bind(const ModelObj* pModelObj, const ModelObj* pHostModelObj) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付け、リターゲッティング情報を生成します。
    //!
    //! @param[in] pSkeleton アニメーション対象のスケルトンリソースへのポインター。
    //! @param[in] pHostSkeleton 元となるスケルトンリソースへのポインター。
    //!
    //! @deprecated 廃止予定です。BindArgument を引数に持つ nn::g3d::SkeletalAnimObj::Bind() を使用して下さい。
    //!
    //! @return BindResult を返します。
    //!
    NN_DEPRECATED BindResult Bind(const ResSkeleton* pSkeleton, const ResSkeleton* pHostSkeleton) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付け、リターゲッティング情報を生成します。
    //!
    //! @param[in] pSkeletonObj アニメーション対象のスケルトンへのポインター。
    //! @param[in] pHostSkeletonObj 元となるスケルトンへのポインター。
    //!
    //! @deprecated 廃止予定です。BindArgument を引数に持つ nn::g3d::SkeletalAnimObj::Bind() を使用して下さい。
    //!
    //! @return BindResult を返します。
    //!
    NN_DEPRECATED BindResult Bind(const SkeletonObj* pSkeletonObj, const SkeletonObj* pHostSkeletonObj) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pSkeleton アニメーション対象のスケルトンリソースへのポインター。
    //!
    //! @pre
    //! - pSkeleton と設定されている ResSkeletalAnim のリソースが ResSkeletonAim::PreBind によって事前に関連付けられている。
    //!
    //! @details
    //! リソース同士を ResSkeletalAnim::PreBind によって事前に関連付けることにより、
    //! BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    void BindFast(const ResSkeleton* pSkeleton) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] arg バインドに必要なパラメーターが設定された BindArgument
    //!
    //! @return BindResult を返します。
    //!
    //! @pre
    //! - pSkeleton と設定されている ResSkeletalAnim のリソースが ResSkeletonAim::PreBind によって事前に関連付けられている。
    //!
    //! @details
    //! リソース同士を PreBind によって事前に関連付けることにより、
    //! BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //! BindArgument にホストスケルトンが指定されている場合はリターゲッティング情報を生成します。
    //! BindArgument にミラーリングが有効に設定されている場合はモーションミラーリングが有効になります。
    //!
    BindResult BindFast(const BindArgument& arg) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付け、リターゲッティング情報を生成します。
    //!
    //! @param[in] pSkeleton アニメーション対象のスケルトンリソースへのポインター。
    //! @param[in] pHostSkeletonObj 元となるスケルトンへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @pre
    //! - pSkeleton と設定されている ResSkeletalAnim のリソースが ResSkeletonAim::PreBind() によって事前に関連付けられている。
    //!
    //! @deprecated 廃止予定です。BindArgument を引数に持つ nn::g3d::SkeletalAnimObj::BindFast() を使用して下さい。
    //!
    //! @details
    //! リソース同士を ResSkeletalAnim::PreBind() によって事前に関連付けることにより、
    //! BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    NN_DEPRECATED BindResult BindFast(const ResSkeleton* pSkeleton, const ResSkeleton* pHostSkeletonObj) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付け、リターゲッティング情報を生成します。
    //!
    //! @param[in] pModel アニメーション対象のモデルリソースへのポインター。
    //! @param[in] pHostModel 元となるモデルリソースへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @pre
    //! - pSkeleton と設定されている ResSkeletalAnim のリソースが ResSkeletalAnim::PreBind() によって事前に関連付けられている。
    //!
    //! @deprecated 廃止予定です。BindArgument を引数に持つ nn::g3d::SkeletalAnimObj::BindFast() を使用して下さい。
    //!
    //! @details
    //! リソース同士を ResSkeletalAnim::PreBind() によって事前に関連付けることにより、
    //! BindFast() 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    NN_DEPRECATED BindResult BindFast(const ResModel* pModel, const ResModel* pHostModel) NN_NOEXCEPT;

    //! @brief 指定したインデックスのボーンにアニメーションフラグを設定します。
    //!
    //! @param[in] boneIndex ボーンのインデックス。
    //! @param[in] flag 設定するフラグ。
    //!
    void SetBindFlag(int boneIndex, BindFlag flag) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        SetBindFlagImpl(boneIndex, flag);
    }

    //! @brief スケルトン構造を辿って指定したインデックスのボーン以下のボーンにアニメーションフラグを設定します。
    //!
    //! @param[in] pSkeleton スケルトンリソースへのポインター。
    //! @param[in] boneIndex ボーンのインデックス。
    //! @param[in] flag 設定するフラグ。
    //!
    void SetBindFlag(const ResSkeleton* pSkeleton, int boneIndex, BindFlag flag) NN_NOEXCEPT;

    //! @brief 指定したインデックスのボーンのアニメーションフラグを取得します。
    //!
    //! @param[in] boneIndex ボーンのインデックス。
    //!
    //! @return フラグを返します。
    //!
    BindFlag GetBindFlag(int boneIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        return GetBindFlagImpl(boneIndex);
    }

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    virtual void ClearResult() NN_NOEXCEPT;
    virtual void Calculate() NN_NOEXCEPT;
    virtual void ApplyTo(ModelObj* pModelObj) const NN_NOEXCEPT;

    //! @brief ResSkeleton を用いて初期化を行います。
    //!
    //! @param[in] pSkeleton スケルトンリソースへのポインター。
    //!
    void ClearResult(const ResSkeleton* pSkeleton) NN_NOEXCEPT;

    //! @brief アニメーション結果を対象に適用します。
    //!
    //! @param[in] pSkeletonObj スケルトンへのポインター。
    //!
    void ApplyTo(SkeletonObj* pSkeletonObj) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief リソースを取得します。
    //!
    //! @return スケルタルアニメーションリソースへのポインターを返します。
    //!
    const ResSkeletalAnim* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    };

    //! @brief リソースを設定します。
    //!
    //! @param[in] pRes スケルタルアニメーションリソースへのポインター。
    //!
    //! @details
    //! 計算するスケルタルアニメーションを差し替えます。
    //!
    void SetResource(const ResSkeletalAnim* pRes) NN_NOEXCEPT;

    //! @brief リソースを取り外します。
    void ResetResource() NN_NOEXCEPT
    {
        m_pRes = NULL;
        SetTargetUnbound();
    }

    //! @brief アニメーションの計算結果を取得します。
    //!
    //! @return アニメーションの計算結果を格納した BoneAnimResult 配列へのポインターを返します。
    //!
    BoneAnimResult* GetResultArray() NN_NOEXCEPT
    {
        return static_cast<BoneAnimResult*>(GetResultBuffer());
    }

    //! @brief アニメーションの計算結果を取得します。
    //!
    //! @return アニメーションの計算結果を格納した BoneAnimResult 配列へのポインターを返します。
    //!
    const BoneAnimResult* GetResultArray() const NN_NOEXCEPT
    {
        return static_cast<const BoneAnimResult*>(GetResultBuffer());
    }

    //! @brief アニメーション対象に対して十分なバッファーが存在するかどうかを取得します。
    //!
    //! @param[in] pSkeleton 対象のスケルトンリソースへのポインター。
    //!
    //! @return 十分なバッファーが存在する場合は true、それ以外の場合は false を返します。
    //!
    bool IsAcceptable(const ResSkeleton* pSkeleton) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES(pSkeleton != NULL, NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        return pSkeleton->GetBoneCount() <= GetBindTable().GetSize();
    }

    //! @brief アニメーションリソースに対して十分なバッファーが存在するかどうかを取得します。
    //!
    //! @param[in] pRes 対象のスケルタルアニメーションリソースへのポインター。
    //!
    //! @return 十分なバッファーが存在する場合は true、それ以外の場合は false を返します。
    //!
    bool IsAcceptable(const ResSkeletalAnim* pRes) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pRes);
        return pRes->GetBoneAnimCount() <= m_MaxBoneAnim;
    }

    //! @brief スケルタルアニメーション名を取得します。
    //!
    //! @return ResSkeletalAnim のスケルタルアニメーション名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return m_pRes->GetName();
    }

    //! @brief アニメーション対象として関連付けられているスケルトンリソースを取得します。
    //!
    //! @return アニメーション対象として関連付けられているスケルトンリソースへのポインター。
    //!
    const ResSkeleton* GetResSkeleton() const NN_NOEXCEPT
    {
        return m_pResSkeleton;
    }

    //@}

protected:
    //! @briefprivate アニメーション計算の状態を表すフラグです。
    //!
    enum Flag
    {
        Flag_InvalidResult      = 0x1 << 4,
        Flag_RetargetingEnabled = 0x1 << 5,
        Flag_MirroringEnabled   = 0x1 << 6
    };

    //! @briefprivate
    enum Mask
    {
        Mask_Result         = Flag_InvalidResult
    };

    //! @briefprivate ボーンアニメーションを取得します。
    //!
    const ResBoneAnim* GetBoneAnim(int animIndex) const NN_NOEXCEPT
    {
        return &m_pBoneAnimArray[animIndex];
    }

    //! @briefprivate アニメーションの関連付けを行う内部関数です。
    //!
    BindResult BindImpl(const ResSkeleton* pSkeleton) NN_NOEXCEPT;

    //! @briefprivate クリア処理を行う内部関数です。
    //!
    template <bool mirroring, bool retargeting>
    void ClearImpl(const ResSkeleton* pSkeleton) NN_NOEXCEPT;

    //! @briefprivate アニメーションカーブの評価を行う内部関数です。
    //!
    template <bool mirroring, bool retargeting>
    void CalculateImpl() NN_NOEXCEPT;

    //! @briefprivate アニメーション結果を対象に適用する内部関数です。
    //!
    template <typename ConvRot>
    void ApplyToImpl(SkeletonObj* pSkeletonObj) const NN_NOEXCEPT;

    //! @briefprivate リターゲッティングの事前計算を行う内部用の初期化関数です。
    //!
    BindResult InitRetargeting(const ResSkeleton* pTargetSkeleton, const ResSkeleton* pHostSkeleton) NN_NOEXCEPT;

private:
    class Impl;
    class InitializeArgument;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    const ResSkeletalAnim* m_pRes;
    const ResBoneAnim* m_pBoneAnimArray;
    int32_t m_MaxBoneAnim;
    Bit32 m_Flag;
    nn::util::Vector4fType* m_pRetargetingPreCalculateArray; // リターゲッティングの事前計算バッファー。
    const nn::g3d::ResSkeleton* m_pResSkeleton;

    NN_DISALLOW_COPY(SkeletalAnimObj);
};

//--------------------------------------------------------------------------------------------------

// @brief SkeletalAnimObj::Bind() に使用するパラメーターです。
class SkeletalAnimObj::BindArgument
{
public:

    BindArgument() NN_NOEXCEPT
        : m_pSkeleton(NULL)
        , m_pHostSkeleton(NULL)
        , m_RetargetingEnabled(false)
        , m_MirroringEnabled(false)
    {
    }

    //! @brief スケルトンリソースを設定します。
    void SetResource(const ResSkeleton* pSkeleton) NN_NOEXCEPT
    {
        m_pSkeleton = pSkeleton;
    };

    //! @brief スケルトンリソースを取得します。
    const ResSkeleton* GetResource() const NN_NOEXCEPT
    {
        return m_pSkeleton;
    };

    //! @brief リターゲティング用のホストスケルトンリソースを設定します。
    //!
    //! @details 本関数ではリターゲティング機能は有効になりません。
    //!         リターゲティングを有効にする場合は SetRetargetingEnabled() を呼んでください。
    //!
    void SetHostResource(const ResSkeleton* pHostSkeleton) NN_NOEXCEPT
    {
        m_pHostSkeleton = pHostSkeleton;
    };

    //! @brief リターゲティング用のホストスケルトンリソースを取得します。
    const ResSkeleton* GetHostResource() const NN_NOEXCEPT
    {
        return m_pHostSkeleton;
    };

    //! @brief アニメーションのリターゲティングを有効にします。
    void SetRetargetingEnabled() NN_NOEXCEPT
    {
        m_RetargetingEnabled = true;
    }

    //! @brief アニメーションのリターゲティングを無効にします。
    void SetRetargetingDisabled() NN_NOEXCEPT
    {
        m_RetargetingEnabled = false;
    }

    //! @brief アニメーションのリターゲティングが有効かどうかを取得します。
    //!
    //! @return アニメーションのリターゲティングが有効な場合は true、無効な場合は false を返します。
    //!
    bool IsRetargetingEnabled() const NN_NOEXCEPT
    {
        return m_RetargetingEnabled;
    }

    //! @brief アニメーションのミラーリングを有効にします。
    void SetMirroringEnabled() NN_NOEXCEPT
    {
        m_MirroringEnabled = true;
    }

    //! @brief アニメーションのミラーリングを無効にします。
    void SetMirroringDisabled() NN_NOEXCEPT
    {
        m_MirroringEnabled = false;
    }

    //! @brief アニメーションのミラーリングが有効かどうかを取得します。
    //!
    //! @return アニメーションのミラーリングが有効な場合は true、無効な場合は false を返します。
    //!
    bool IsMirroringEnabled() const NN_NOEXCEPT
    {
        return m_MirroringEnabled;
    }

private:
    const ResSkeleton* m_pSkeleton;
    const ResSkeleton* m_pHostSkeleton;
    bool  m_RetargetingEnabled;
    bool  m_MirroringEnabled;
};

//! @briefprivate SkeletalAnimObj::Initialize() に渡して初期化を行うパラメータです。
class SkeletalAnimObj::InitializeArgument
{
public:
    //! @brief メモリブロックのインデックスを示す列挙子。
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_ResultBuffer,
        MemoryBlockIndex_BindTable,
        MemoryBlockIndex_FrameCacheArray,
        MemoryBlockIndex_RetargetingPreCalculateBuffer,
        MemoryBlockIndex_End
    };
    //! @brief コンストラクタです。
    //!
    //! @details
    //! インスタンスで使用するすべてのスケルタルアニメーションとすべてのスケルトンリソースに対して
    //! Reserve() するか、SetMaxBoneCount()、SetMaxBoneAnimCount()、SetMaxCurveCount() で
    //! バッファーサイズの計算に必要なパラメータを指定する必要があります。
    //!
    InitializeArgument() NN_NOEXCEPT
    {
        Clear();
    }

    //! @brief パラメータを初期化します。
    void Clear() NN_NOEXCEPT
    {
        m_BoneCount = m_BoneAnimCount = m_CurveCount = -1;
        m_ContextEnabled = true;
        m_ContextAvailable = false;
        m_RetargetingEnabled = false;
        m_WorkMemory.Invalidate();
        for (int blockIndex = 0; blockIndex < MemoryBlockIndex_End; ++blockIndex)
        {
            m_MemoryBlock[blockIndex].Initialize();
        }
    }

    //! @brief 対象となるスケルトンリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResSkeleton スケルトンリソースへのポインター。
    //!
    void Reserve(const ResSkeleton* pResSkeleton) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResSkeleton);
        m_BoneCount = std::max(m_BoneCount, pResSkeleton->GetBoneCount());
    }

    //! @brief 対象となるモデルリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResModel モデルリソースへのポインター。
    //!
    void Reserve(const ResModel* pResModel) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResModel);
        Reserve(pResModel->GetSkeleton());
    }

    //! @brief 対象となるスケルタルアニメーションリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResAnim スケルタルアニメーションリソースへのポインター。
    //!
    void Reserve(const ResSkeletalAnim* pResAnim) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResAnim);
        m_BoneAnimCount = std::max(m_BoneAnimCount, pResAnim->GetBoneAnimCount());
        m_CurveCount = std::max(m_CurveCount, pResAnim->GetCurveCount());
        m_ContextAvailable |= !pResAnim->IsCurveBaked();
    }

    //! @brief アニメーション高速化のためのコンテクストを有効にします。
    void SetContextEnabled() NN_NOEXCEPT
    {
        m_ContextEnabled = true;
    }

    //! @brief アニメーション高速化のためのコンテクストを無効にします。
    void SetContextDisabled() NN_NOEXCEPT
    {
        m_ContextEnabled = false;
    }

    //! @brief アニメーション高速化のためのコンテクストが有効かどうかを取得します。
    //!
    //! @return アニメーション高速化のためのコンテクストが有効な場合は true、無効な場合は false を返します。
    //!
    bool IsContextEnabled() const NN_NOEXCEPT
    {
        return m_ContextAvailable && m_ContextEnabled;
    }

    //! @brief 使用できる最大のボーン数を指定します。
    //!
    //! @param[in] boneCount 使用できる最大のボーン数。
    //!
    void SetMaxBoneCount(int boneCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(boneCount >= 0);
        m_BoneCount = boneCount;
    }

    //! @brief 使用できる最大のボーン数を取得します。
    //!
    //! @return 使用できる最大のボーン数を返します。
    //!
    int GetMaxBoneCount() const NN_NOEXCEPT
    {
        return m_BoneCount;
    }

    //! @brief 使用できる最大のボーンアニメーション数を指定します。
    //!
    //! @param[in] boneAnimCount 使用できる最大のボーンアニメーション数。
    //!
    void SetMaxBoneAnimCount(int boneAnimCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(boneAnimCount >= 0);
        m_BoneAnimCount = boneAnimCount;
    }

    //! @brief 使用できる最大のボーンアニメーション数を取得します。
    //!
    //! @return 使用できる最大のボーンアニメーション数を返します。
    //!
    int GetMaxBoneAnimCount() const NN_NOEXCEPT
    {
        return m_BoneAnimCount;
    }

    //! @brief 使用できる最大のカーブ数を指定します。
    //!
    //! @param[in] curveCount 使用できる最大のカーブ数。
    //!
    void SetMaxCurveCount(int curveCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(curveCount >= 0);
        m_CurveCount = curveCount;
        m_ContextAvailable = true;
    }

    //! @brief 使用できる最大のカーブ数を取得します。
    //!
    //! @return 使用できる最大のカーブ数を返します。
    //!
    int GetMaxCurveCount() const NN_NOEXCEPT
    {
        return m_CurveCount;
    }

    //! @brief アニメーションのリターゲッティングを有効にします。
    void SetRetargetingEnabled() NN_NOEXCEPT
    {
        m_RetargetingEnabled = true;
    }

    //! @brief アニメーションのリターゲッティングを無効にします。
    void SetRetargetingDisabled() NN_NOEXCEPT
    {
        m_RetargetingEnabled = false;
    }

    //! @brief アニメーションのリターゲッティングが有効かどうかを取得します。
    //!
    //! @return アニメーションのリターゲッティングが有効な場合は true、無効な場合は false を返します。
    //!
    bool IsRetargetingEnabled() const NN_NOEXCEPT
    {
        return m_RetargetingEnabled;
    }

    //! @brief SkeletalAnimObj 構築に必要なメモリサイズを取得します。
    //!
    //! @return  SkeletalAnimObj 構築に必要なメモリサイズを返します。
    //!
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief SkeletalAnimObj 構築に必要なメモリサイズを計算します。
    void CalculateMemorySize() NN_NOEXCEPT;

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    void* GetBuffer( void* pHead, int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return m_MemoryBlock[index].Get(pHead);
    }

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    template< typename T >
    T* GetBuffer( void* pHead, int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return static_cast<T*>( GetBuffer(pHead, index) );
    }

    //! @brief 必要なパラメータが設定されているかどうかを取得します。
    //!
    //! @return 必要なパラメータが設定されている場合は true、設定されていない場合は false を返します。
    //!
    bool IsValid() const
    {
        return m_BoneCount >= 0 && m_BoneAnimCount >= 0 && m_CurveCount >= 0;
    }

    //! @brief ShapeAnimObj 構築に必要なメモリサイズが計算済みかを取得します。
    //!
    //! @return 計算済みの場合は true、未計算の場合は false を返します。
    //!
    bool IsMemoryCalculated() const NN_NOEXCEPT
    {
        return m_WorkMemory.IsValid();
    }

private:
    int m_BoneCount;
    int m_BoneAnimCount;
    int m_CurveCount;
    bool m_ContextEnabled;
    bool m_ContextAvailable; // リソースがベイクされていないか、カーブ数を手動指定した場合に true
    bool m_RetargetingEnabled;
    nn::util::MemorySplitter m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
};

//--------------------------------------------------------------------------------------------------

//! @brief SkeletalAnimObj の構築を行うクラスです。
class SkeletalAnimObj::Builder : public SkeletalAnimObj::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @details
    //! インスタンスで使用するすべてのスケルタルアニメーションリソースとすべてのスケルトンに対して
    //! Reserve() するか、SetMaxBoneCount()、SetMaxBoneAnimCount()、SetMaxCurveCount() で
    //! バッファーサイズの計算に必要なパラメータを指定する必要があります。
    //!
    Builder()
    {
    }

    //! @brief SkeletalAnimObj を構築します。
    //!
    //! @param[in] pSkeletalAnimObj SkeletalAnimObj へのポインター
    //! @param[in] pBuffer バッファーへのポインター
    //! @param[in] bufferSize バッファーのサイズ
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ
    //!
    bool Build(SkeletalAnimObj* pSkeletalAnimObj, void* pBuffer, size_t bufferSize) const NN_NOEXCEPT
    {
        return pSkeletalAnimObj->Initialize(*this, pBuffer, bufferSize);
    }
};

//--------------------------------------------------------------------------------------------------

//! @brief アニメーションブレンディング時に呼び出されるコールバックの基底クラスです。
class ICalculateBlendWeightCallback
{
public:
    static const uint16_t InvalidBone = 0xFFFF;

    //! @brief デストラクタです。
    virtual ~ICalculateBlendWeightCallback() NN_NOEXCEPT {}

    //! @brief Exec() 用いる設定です。
    class CallbackArg
    {
    public:
        //! @brief  コンストラクタです。
        //!
        //! @param[in] boneIndex ボーンのインデックス。
        //! @param[in] weight ブレンドウェイト。
        //!
        CallbackArg(const int& boneIndex, float weight) NN_NOEXCEPT
            : m_pAnimObj(NULL), m_BoneIndex(boneIndex), m_BlendWeight(weight)
        {
        }

        //! @brief  コンストラクタです。
        //!
        //! @param[in] pAnimObj SkletalAnimObj へのポインター。
        //! @param[in] boneIndex ボーンのインデックス。
        //! @param[in] weight ブレンドウェイト。
        //!
        CallbackArg(SkeletalAnimObj* pAnimObj, const int& boneIndex, float weight) NN_NOEXCEPT
            : m_pAnimObj(pAnimObj), m_BoneIndex(boneIndex), m_BlendWeight(weight)
        {
        }

        //! @brief 計算に用いる SkeletalAnimObj を取得します。
        //!
        //! @return 計算に用いる SkeletalAnimObj を返します。
        //!
        //! @details BlendResult() を使用したブレンドを行った場合は NULL を返します。
        //!
        const SkeletalAnimObj* GetAnimObj() const NN_NOEXCEPT
        {
            return m_pAnimObj;
        }

        //! @brief 計算に用いるボーンインデクスを取得します。
        //!
        //! @return 計算に用いるボーンインデクスを返します。
        //!
        int GetBoneIndex() const NN_NOEXCEPT
        {
            return m_BoneIndex;
        }

        //! @brief ブレンドウェイトをセットします。
        //!
        //! @param[in] weight ブレンドウェイト。
        //!
        void SetBlendWeight(float weight) NN_NOEXCEPT
        {
            m_BlendWeight = weight;
        }

        //! @brief ブレンドウェイトを取得します。
        //!
        //! @return ブレンドウェイトを返します。
        //!
        float GetBlendWeight() const NN_NOEXCEPT
        {
            return m_BlendWeight;
        }

    private:
        SkeletalAnimObj* m_pAnimObj;
        int m_BoneIndex;
        float m_BlendWeight;

        NN_DISALLOW_COPY(CallbackArg);
    };

    //! @brief コールバックを呼び出します。
    virtual void Exec(CallbackArg& arg) NN_NOEXCEPT = 0;
};

//--------------------------------------------------------------------------------------------------

//! @brief スケルタルアニメーションのブレンドを行うクラスです。
class SkeletalAnimBlender
{
public:

    //! @briefprivate
    enum Shift
    {
        Shift_Blend,
        Shift_Interpolate
    };

    //! @brief ブレンド方式です。
    enum BlendMode
    {
        //! @brief 補間ブレンドです。
        BlendMode_Interpolate = 0x0 << Shift_Blend,

        //! @brief 加算ブレンドです。
        BlendMode_Add = 0x1 << Shift_Blend
    };

    //! @brief 補間方式です。
    enum InterpolateMode
    {
        //! @brief 線形補間です。
        InterpolateMode_Lerp = 0x0 << Shift_Interpolate,

        //! @brief 球面線形補間です。
        InterpolateMode_Slerp = 0x1 << Shift_Interpolate
    };

    //! @brief インスタンスの構築時に渡すバッファーの必要アライメントサイズです。
    enum Alignment
    {
        //! @brief Builder に渡すバッファーの必要アライメントサイズです。
        Alignment_Buffer = MatrixVectorAlignment,
    };

    //!@brief 2つのボーンアニメーションの差分を格納する構造体です。
    struct BoneAnimDiffResult
    {
        BoneAnimResult result;
    };

    class Builder;

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    //!
    //! 実際の構築処理は Builder で行います。
    //!
    SkeletalAnimBlender() NN_NOEXCEPT
        : m_pResultBuffer(NULL)
        , m_MaxBone(0)
        , m_BoneCount(0)
        , m_Flag(0)
        , m_pBufferPtr(NULL)
        , m_pCallback(NULL)
    {
    }

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    //! @brief ブレンドの前処理を行います。
    void ClearResult() NN_NOEXCEPT;

    //! @brief SkeletalAnimObj::Calculate() を呼び出し、その結果をブレンドします。
    //!
    //! @param[in] pAnimObj ブレンドするスケルタルアニメーションへのポインター。
    //! @param[in] weight ブレンドウェイト。
    //!
    //! @details
    //! 既に計算済みの場合は SkeletalAnimObj::Calculate() はスキップされます。
    //! SkeletalAnimObj に SKIP_APPLY フラグが指定されているボーンはブレンドされません。
    //!
    void Blend(SkeletalAnimObj* pAnimObj, float weight) NN_NOEXCEPT;

    //! @brief 2つのアニメーション間の差分を用いて、ブレンド計算を行います。
    //!
    //! @param[in]  pSrcAnimObj         減算を被るスケルタルアニメーションへのポインター。
    //! @param[in]  pReferenceAnimObj   減算を行うスケルタルアニメーションへのポインター。
    //! @param[in]  weight              ブレンドウェイト。
    //!
    //! @details
    //! この関数は BeginBlend() や SetBlendMode() にて BlendMode_Add が設定されていなければなりません。
    //!
    //! SkeletalAnimObj::Calculate() を呼び出し、その結果をブレンドします。
    //! 既に計算済みの場合は SkeletalAnimObj::Calculate() はスキップされます。
    //! pSrcAnimObj に SKIP_APPLY フラグが指定されているボーンはブレンドされません。
    //!
    //! pSrcAnimObj - pReferenceAnimObj の計算を行い、得られた差分と累積値を乗算した値とブレンドします。
    //! この関数によるブレンド計算は、この関数を呼ぶ以前の累積したブレンド計算に依存します。
    //! この関数の呼び出すタイミングによって、最終的なブレンド結果が変化するため注意してください。
    //!
    void Blend(SkeletalAnimObj* pSrcAnimObj, SkeletalAnimObj* pReferenceAnimObj, float weight) NN_NOEXCEPT;

    //! @brief 指定したボーンに BoneAnimResult をブレンドします。
    //!
    //! @param[in] boneAnimResult ボーンアニメーション結果を格納するバッファーへの参照。
    //! @param[in] boneIndex ブレンドを適用するボーンのインデクス。
    //! @param[in] weight ブレンドウェイト。
    //!
    //! @pre
    //! - boneIndex < GetBoneCount()
    //!
    void BlendResult(const BoneAnimResult& boneAnimResult, int boneIndex, float weight) NN_NOEXCEPT;

    //! @brief 指定したボーンに BoneAnimDiffResult をブレンドします。
    //!
    //! @param[in]  boneAnimDiffResult  ボーンアニメーションの差分を格納するバッファーへの参照。
    //! @param[in]  boneIndex           ブレンドを適用するボーンのインデックス。
    //! @param[in]  weight              ブレンドウェイト
    //!
    //! @pre
    //! - GetBlendMode() == BlendMode_Add
    //! - boneIndex < GetBoneCount()
    //!
    //! @details
    //! - boneAnimDiffResult が無効状態に設定されている場合はブレンド処理をスキップします。
    //!
    void BlendResult(const BoneAnimDiffResult& boneAnimDiffResult, int boneIndex, float weight) NN_NOEXCEPT;

    //! @brief ブレンド結果を対象に適用します。
    //!
    //! @param[in] pSkeletonObj ブレンド結果を適用するスケルトンへのポインター。
    //!
    //! @details
    //! 内部でキャッシュ処理を行うため、この関数はスレッドセーフではありません。
    //! 同一インスタンスに対する呼び出しを複数スレッドから行わないでください。
    //! 累積ウェイトが 0 ボーンは適用処理を行いません。
    //! 累積ウェイトが 1 でないボーンは正規化してから適用します。
    //!
    void ApplyTo(SkeletonObj* pSkeletonObj) const NN_NOEXCEPT;

    //! @brief ブレンド処理を開始します。
    //!
    //! @param[in] flag BlendMode や InterpolateMode などのフラグ。
    //!
    //! @details
    //! flag に BlendMode_Interpolate を設定した場合、
    //! EndBlend() にてブレンド結果の正規化が行われます。
    //!
    void BeginBlend(Bit32 flag) NN_NOEXCEPT;

    //! @brief ブレンド処理を終了します。
    //!
    //! @details
    //! - BeginBlend() にて BlendMode_Interpolate を設定していた場合、ブレンド結果の正規化を行います。
    //! - この関数を呼び出すと、累積ウェイトが 1 になります。
    //!
    void EndBlend() NN_NOEXCEPT;

    //! @brief 2つのアニメーションの差分を計算します。
    //!
    //! @param[inout]   pOutBoneAnimDiffResultArray     アニメーションの差分の書き込み先へのポインターです。
    //! @param[in]      resultCount                     pOutBoneAnimDiffResultArray の要素数です。
    //! @param[in]      pSrcAnimObj                     減算を被るスケルタルアニメーションへのポインター。
    //! @param[in]      pReferenceAnimObj               減算を行うスケルタルアニメーションへのポインター。
    //!
    //! @pre
    //! - pOutBoneAnimDiffResultArray != NULL
    //! - resultCount >= pSrcAnimObj->GetAnimCount()
    //! - pSrcAnimObj != NULL
    //! - pReferenceAnimObj != NULL
    //!
    //! @details
    //! - (pSrcAnimObj - pReferenceAnimObj) の計算を行い、得られた差分と累積値を乗算した値とブレンドします。
    //! - SkeletalAnimObj::Calculate() を呼び出し、その結果をブレンドします。
    //! - 既に計算済みの場合は SkeletalAnimObj::Calculate() はスキップされます。
    //! - pSrcAnimObj に SKIP_APPLY フラグが指定されているボーンは無効状態に設定されます。
    //! - ボーンインデックスと一致するインデックスを持つ pOutBoneAnimDiffResultArray の要素に値を書き込みます。
    //!
    static void CalculateBoneAnimDiff(BoneAnimDiffResult* pOutBoneAnimDiffResultArray, int resultCount, SkeletalAnimObj* pSrcAnimObj, SkeletalAnimObj* pReferenceAnimObj) NN_NOEXCEPT;

    //! @brief バインドポーズからの変化量を計算します。
    //!
    //! @param[inout]   pOutBoneAnimDiffResultArray     アニメーションの差分の書き込み先へのポインターです。
    //! @param[in]      resultCount                     pOutBoneAnimDiffResultArray の要素数です。
    //! @param[in]      pSrcAnimObj                     減算を被るスケルタルアニメーションへのポインター。
    //!
    //! @pre
    //! - pOutBoneAnimDiffResultArray != NULL
    //! - resultCount >= pSrcAnimObj->GetAnimCount()
    //! - pSrcAnimObj != NULL
    //!
    //! @details
    //! - SkeletalAnimObj::Calculate() を呼び出し、その結果をブレンドします。
    //! - 既に計算済みの場合は SkeletalAnimObj::Calculate() はスキップされます。
    //! - pSrcAnimObj に SKIP_APPLY フラグが指定されているボーンは無効状態に設定されます。
    //! - ボーンインデックスと一致するインデックスを持つ pOutBoneAnimDiffResultArray の要素に値を書き込みます。
    //!
    static void CalculateBoneAnimDiff(BoneAnimDiffResult* pOutBoneAnimDiffResultArray, int resultCount, SkeletalAnimObj* pSrcAnimObj) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief Build() 時に渡されたバッファーを取得します。
    void* GetBufferPtr() NN_NOEXCEPT
    {
        return m_pBufferPtr;
    }

    //! @brief 書き込み先のボーン数を設定します。
    //!
    //! @param[in] count 書き込み先のボーン数。
    //!
    //! @details
    //! GetMaxBoneCount() で取得できる最大ボーン数以下である必要があります。
    //!
    void SetBoneCount(int count) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(count <= m_MaxBone);
        m_BoneCount = static_cast<uint16_t>(count);
    }

    //! @brief 書き込み先のボーン数を取得します。
    //!
    //! @return 書き込み先のボーン数を返します。
    //!
    int GetBoneCount() const NN_NOEXCEPT
    {
        return m_BoneCount;
    }

    //! @brief ブレンド計算を行う最大ボーン数を取得します。
    //!
    //! @return ブレンド計算を行う最大ボーン数を返します。
    //!
    int GetMaxBoneCount() const NN_NOEXCEPT
    {
        return m_MaxBone;
    }

    //! @brief ブレンド方式を設定します。
    //!
    //! @param[in] mode ブレンド方式。
    //!
    //! @deprecated
    //! この関数は非推奨です。
    //! BeginBlend() に BlendMode を指定し設定してください。
    //!
    void SetBlendMode(BlendMode mode) NN_NOEXCEPT
    {
        m_Flag = (m_Flag & ~Flag_BlendMask) | mode;
    }

    //! @brief ブレンド方式を取得します。
    //!
    //! @return ブレンド方式を返します。
    //!
    BlendMode GetBlendMode() const NN_NOEXCEPT
    {
        return BlendMode(m_Flag & Flag_BlendMask);
    }

    //! @brief 球面線形補間を有効にします。
    //!
    //! @deprecated
    //! この関数は非推奨です。
    //! BeginBlend() に InterpolateMode を指定し補間方式を設定してください。
    //!
    void SetSlerpEnabled() NN_NOEXCEPT
    {
        m_Flag |= InterpolateMode_Slerp;
    }

    //! @brief 球面線形補間を無効にします。
    //!
    //! @deprecated
    //! この関数は非推奨です。
    //! BeginBlend() に InterpolateMode を指定し補間方式を設定してください。
    //!
    void SetSlerpDisabled() NN_NOEXCEPT
    {
        m_Flag &= ~InterpolateMode_Slerp;
    }

    //! @brief 補間方式を取得します。
    //!
    //! @return 補間方式を返します。
    //!
    InterpolateMode GetInterpolateMode() const NN_NOEXCEPT
    {
        return InterpolateMode(m_Flag & Flag_InterpolateMask);
    }

    //! @brief 球面線形補間が有効であるかどうかを取得します。
    //!
    //! @return 球面線形補間が有効な場合は true、無効な場合は false を返します。
    //!
    bool IsSlerpEnabled() const NN_NOEXCEPT
    {
        return 0 != (m_Flag & InterpolateMode_Slerp);
    }

    //! @brief ブレンド結果を取得します。
    //!
    //! @return ブレンド結果を格納したバッファーへのポインターを返します。
    //!
    BoneAnimBlendResult* GetResultArray() NN_NOEXCEPT
    {
        return static_cast<BoneAnimBlendResult*>(m_pResultBuffer);
    }

    //! @brief ブレンド結果を取得します。
    //!
    //! @return ブレンド結果を格納したバッファーへのポインターを返します。
    //!
    const BoneAnimBlendResult* GetResultArray() const NN_NOEXCEPT
    {
        return static_cast<const BoneAnimBlendResult*>(m_pResultBuffer);
    }

    //! @brief ブレンド結果を取得します。
    //!
    //! @return ブレンド結果を格納したバッファーへのポインターを返します。
    //!
    BoneAnimBlendResult* GetResultArrayMutable() const NN_NOEXCEPT
    {
        return static_cast<BoneAnimBlendResult*>(m_pResultBuffer);
    }

    //! @brief ブレンド時に呼び出されるコールバックを設定します。
    //!
    //! @param[in] pCallback ブレンド時に呼び出されるコールバックへのポインター。
    //!
    void SetCalculateBlendWeightCallback(ICalculateBlendWeightCallback* pCallback) NN_NOEXCEPT
    {
        m_pCallback = pCallback;
    }

    //! @brief ブレンド時に呼び出されるコールバックを取得します。
    //!
    //! @return ブレンド時に呼び出されるコールバックへのポインター。
    //!
    ICalculateBlendWeightCallback* GetCalculateBlendWeightCallback() NN_NOEXCEPT
    {
        return m_pCallback;
    }

    //! @brief ブレンド時に呼び出されるコールバックを取得します。
    //!
    //! @return ブレンド時に呼び出されるコールバックへのポインター。
    //!
    const ICalculateBlendWeightCallback* GetCalculateBlendWeightCallback() const NN_NOEXCEPT
    {
        return m_pCallback;
    }

    //@}

protected:
    class Impl;

    //! @briefprivate ブレンド計算の状態を表すフラグです。
    //!
    //!
    enum Flag
    {
        Flag_BlendMask          = 0x1 << Shift_Blend,
        Flag_InterpolateMask    = 0x1 << Shift_Interpolate,
        Flag_Normalized         = 0x1 << 2,
        Flag_Blending           = 0x1 << 3,
        Flag_InvalidResult      = 0x1 << 4,
        Flag_MaskResult         = Flag_InvalidResult,
    };

    //! @briefprivate ブレンドを行う内部関数です。
    //!
    //!
    template <typename BlendRot, bool useCallback>
    void BlendImpl(SkeletalAnimObj* pAnimObj, float weight) NN_NOEXCEPT;

    //! @briefprivate ブレンドを行う内部関数です。
    //!
    //!
    template <typename BlendRot>
    void BlendResultImpl(BoneAnimBlendResult* pTargetArray, const BoneAnimResult& boneAnimResult, int boneIndex, float weight) NN_NOEXCEPT;

    //! @briefprivate 差分アニメーションのブレンドを行う内部関数です。
    //!
    //!
    void BlendDiffAnim(SkeletalAnimObj* pSrcAnimObj, SkeletalAnimObj* pReferenceAnimObj, float weight) NN_NOEXCEPT;

    //! @briefprivate ボーンアニメーション間の差分を求める内部関数です。
    //!
    //!
    template<typename TargetConvRot, typename ReferenceConvRot>
    static void CalculateBoneAnimDiff(BoneAnimResult* pDiffAnim, const BoneAnimResult& targetBone, const BoneAnimResult& referenceBone) NN_NOEXCEPT;

    //! @briefprivate ボーンアニメーション間の差分をブレンド処理に利用できる形へ変換する内部関数です。
    //!
    //!
    template<typename BlendRot>
    void ConvertValidResult(BoneAnimResult* pConvertedResult, const BoneAnimBlendResult& pBlendResult, const  BoneAnimResult& pDiffResult) NN_NOEXCEPT;

    //! @briefprivate ブレンド結果を適用する内部関数です。
    //!
    //!
    template <typename ConvRot, BlendMode mode>
    void ApplyToImpl(SkeletonObj* pSkeletonObj) const NN_NOEXCEPT;

    //! @briefprivate 補間モードが変更された際に、累積された回転値を変換する内部関数です。
    //!
    //!
    template<typename ConvRot>
    void ConvertResultRotate() NN_NOEXCEPT;

private:
    class InitializeArgument;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    void* m_pResultBuffer;
    uint16_t m_MaxBone; //!< 初期化時に指定されてバッファーサイズの計算に使用されたボーン数
    uint16_t m_BoneCount; //!< 書き込み対象のボーンインデックスの最大値+1
    mutable Bit32 m_Flag; //!< ブレンド結果の正規化をキャッシュするため mutable にする。
    void* m_pBufferPtr;
    ICalculateBlendWeightCallback* m_pCallback;

    NN_DISALLOW_COPY(SkeletalAnimBlender);
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate SkeletalAnimBlender::Initialize() に渡して初期化を行うパラメータです。
class SkeletalAnimBlender::InitializeArgument
{
public:
    //! @brief メモリブロックのインデックスを示す列挙子。
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_ResultBuffer,
        MemoryBlockIndex_End
    };

    //! @brief コンストラクタです。
    //!
    //! インスタンスで使用するすべてのスケルトンリソースに対してReserve() するか、
    //! SetMaxBoneCount()、でバッファーサイズの計算に必要なパラメータを指定する必要があります。
    //!
    InitializeArgument() NN_NOEXCEPT
    {
        Clear();
    }

    //! @brief パラメータを初期化します。
    void Clear() NN_NOEXCEPT
    {
        m_BoneCount = -1;
        m_WorkMemory.Invalidate();
        for (int blockIndex = 0; blockIndex < MemoryBlockIndex_End; ++blockIndex)
        {
            m_MemoryBlock[blockIndex].Initialize();
        }
    }

    //! @brief 対象となるスケルトンリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResSkeleton スケルトンリソースへのポインター。
    //!
    void Reserve(const ResSkeleton* pResSkeleton) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResSkeleton);
        m_BoneCount = std::max(m_BoneCount, pResSkeleton->GetBoneCount());
    }

    //! @brief 対象となるモデルリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResModel モデルリソースへのポインター。
    //!
    void Reserve(const ResModel* pResModel) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResModel);
        Reserve(pResModel->GetSkeleton());
    }

    //! @brief 使用できる最大のボーン数を指定します。
    //!
    //! @param[in] boneCount 使用できる最大のボーン数。
    //!
    void SetMaxBoneCount(int boneCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(boneCount >= 0);
        m_BoneCount = boneCount;
    }

    //! @brief 使用できる最大のボーン数を取得します。
    //!
    //! @return 使用できる最大のボーン数。
    //!
    int GetMaxBoneCount() const NN_NOEXCEPT
    {
        return m_BoneCount;
    }

    //! @brief SkeletalAnimBlender 構築に必要なメモリサイズを取得します。
    //!
    //! return SkeletalAnimBlender 構築に必要なメモリサイズ。
    //!
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief SkeletalAnimBlender 構築に必要なメモリサイズを計算します。
    void CalculateMemorySize() NN_NOEXCEPT;

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    void* GetBuffer( void* pHead, int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return m_MemoryBlock[index].Get(pHead);
    }

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    template< typename T >
    T* GetBuffer( void* pHead, int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return static_cast<T*>( GetBuffer(pHead, index) );
    }

    //! @brief 必要なパラメータが設定されているかどうかを取得します。
    //!
    //! @return 要なパラメータが設定されている場合は true、設定されていない場合は false を返します。
    //!
    bool IsValid() const
    {
        return m_BoneCount >= 0;
    }

    //! @briefprivate SkeletalAnimBlender 構築に必要なメモリサイズが計算済みかを取得します。
    //!
    //! @return 計算済みの場合は true、未計算の場合は false を返します。
    //!
    bool IsMemoryCalculated() const NN_NOEXCEPT
    {
        return m_WorkMemory.IsValid();
    }

private:
    int m_BoneCount;
    nn::util::MemorySplitter m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
};

//--------------------------------------------------------------------------------------------------

//! @brief SkeletalAnimBlender の構築を行うクラスです。
class SkeletalAnimBlender::Builder : public SkeletalAnimBlender::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @details
    //! インスタンスで使用するすべてのスケルトンリソースに対してReserve() するか、
    //! SetMaxBoneCount()、でバッファーサイズの計算に必要なパラメータを指定する必要があります。
    //!
    Builder()
    {
    }

    //! @brief SkeletalAnimBlender を構築します。
    //!
    //! @param[in] pSkeletalAnimBlender SkeletalAnimBlender へのポインター
    //! @param[in] pBuffer バッファーへのポインター
    //! @param[in] bufferSize バッファーのサイズ
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ
    //!
    bool Build(SkeletalAnimBlender* pSkeletalAnimBlender, void* pBuffer, size_t bufferSize) const NN_NOEXCEPT
    {
        return pSkeletalAnimBlender->Initialize(*this, pBuffer, bufferSize);
    }
};

//--------------------------------------------------------------------------------------------------

// 回転成分計算用ユーティリティ

//! @brief Euler 角表現から軸回転へ変換するクラスです。
class EulerToAxes
{
public:
    //! @brief Euler 角表現から変換します。
    static void Convert(nn::util::Vector3fType* pAxes, const BoneAnimResult* pResult) NN_NOEXCEPT;
};

//! @brief Quaternion 表現から軸回転へ変換するクラスです。
class QuatToAxes
{
public:
    //! @brief Quaternion から変換します。
    static void Convert(nn::util::Vector3fType* pAxes, const BoneAnimResult* pResult) NN_NOEXCEPT;

    //! @brief Quaternion から変換します。
    //! @return 変換に成功した場合は true を返します。
    static bool Convert(BoneAnimBlendResult* pOutResult, const BoneAnimBlendResult* pResult) NN_NOEXCEPT;

    //! @brief Quaternion から変換します。
    static void Convert(nn::util::Vector3fType* pAxes, const nn::util::Float4& pQuat) NN_NOEXCEPT;
};

//! @brief 軸回転から回転行列へ変換するクラスです。
class AxesToMtx
{
public:

    //! @brief BoneAnimBlendResult から変換します。
    //! @return 変換に成功した場合は true を返します。
    static bool Convert(nn::util::Matrix4x3fType* pMtx, const BoneAnimBlendResult* pResult) NN_NOEXCEPT;
};

//! @brief Euler 角表現から回転行列へ変換するクラスです。
class EulerToMtx
{
public:
    //! @brief BoneAnimResult から変換します。
    //! @return 変換に成功した場合は true を返します。
    static bool Convert(nn::util::Matrix4x3fType* pMtx, const BoneAnimResult* pResult) NN_NOEXCEPT
    {
        nn::util::Vector3fType euler;
        VectorLoad(&euler, pResult->euler);
        MatrixSetRotateXyz(pMtx, euler);
        return true;
    }
};

//! @brief Quaternion から回転行列へ変換するクラスです。
class QuatToMtx
{
public:

    //! @brief BoneAnimResult から変換します。
    //! @return 変換に成功した場合は true を返します。
    static bool Convert(nn::util::Matrix4x3fType* pMtx, const BoneAnimResult* pResult) NN_NOEXCEPT
    {
        nn::util::Vector4fType quatanion;
        VectorLoad(&quatanion, pResult->quat);
        MatrixSetRotate(pMtx, quatanion);
        //pMtx->SetR(pResult->quat);
        return true;
    }

    //! @brief BoneAnimBlendResult から変換します。
    //! @return 変換に成功した場合は true を返します。
    static bool Convert(nn::util::Matrix4x3fType* pMtx, const BoneAnimBlendResult* pResult) NN_NOEXCEPT
    {
        MatrixSetRotate(pMtx, pResult->rotate);
        return true;
    }
};

//! @brief Euler 角表現から Quaternion 表現へ変換するクラスです。
class EulerToQuat
{
public:
    //! @brief BoneAnimResult から変換します。
    //! @return 変換に成功した場合は true を返します。
    static bool Convert(nn::util::Vector4fType* pQuat, const BoneAnimResult* pResult) NN_NOEXCEPT
    {
        float sx, cx;
        nn::util::SinCosEst(&sx, &cx, pResult->euler.x * 0.5f);
        float sy, cy;
        nn::util::SinCosEst(&sy, &cy, pResult->euler.y * 0.5f);
        float sz, cz;
        nn::util::SinCosEst(&sz, &cz, pResult->euler.z * 0.5f);

        nn::util::VectorSetX(pQuat, sx * cy * cz - cx * sy * sz);
        nn::util::VectorSetY(pQuat, cx * sy * cz + sx * cy * sz);
        nn::util::VectorSetZ(pQuat, cx * cy * sz - sx * sy * cz);
        nn::util::VectorSetW(pQuat, cx * cy * cz + sx * sy * sz);
        return true;
    }
};

//! @brief 軸回転から Quaternion 表現へ変換するクラスです。
class AxesToQuat
{
public:
    static bool Convert(nn::util::Vector4fType* pQuat, const BoneAnimBlendResult* pResult) NN_NOEXCEPT
    {
        nn::util::Matrix4x3fType rotMtx;
        bool isSuccessed = AxesToMtx::Convert(&rotMtx, pResult);
        nn::util::QuaternionFromMatrix(pQuat, rotMtx);

        return isSuccessed;
    }

    static bool Convert(BoneAnimBlendResult* pOutResult, const BoneAnimBlendResult* pResult) NN_NOEXCEPT
    {
        return Convert(&pOutResult->rotate, pResult);
    }
};

//--------------------------------------------------------------------------------------------------

namespace detail
{

// TODO: 閾値を検討
inline
bool LengthSqEqualsZero(float length) NN_NOEXCEPT
{
    return length == 0.0f;
}

}

inline
void EulerToAxes::Convert(nn::util::Vector3fType* pAxes, const BoneAnimResult* pResult) NN_NOEXCEPT
{
    nn::util::Vector3fType euler;
    VectorLoad(&euler, pResult->euler);
    nn::util::Vector3fType& axisX = pAxes[0];
    nn::util::Vector3fType& axisY = pAxes[1];

    float sinX = nn::util::SinTable(nn::util::RadianToAngleIndex(pResult->euler.x));
    float sinY = nn::util::SinTable(nn::util::RadianToAngleIndex(pResult->euler.y));
    float sinZ = nn::util::SinTable(nn::util::RadianToAngleIndex(pResult->euler.z));
    float cosX = nn::util::CosTable(nn::util::RadianToAngleIndex(pResult->euler.x));
    float cosY = nn::util::CosTable(nn::util::RadianToAngleIndex(pResult->euler.y));
    float cosZ = nn::util::CosTable(nn::util::RadianToAngleIndex(pResult->euler.z));

    float cosXcosZ = cosX * cosZ;
    float sinXsinY = sinX * sinY;
    float cosXsinZ = cosX * sinZ;

    VectorSet(&axisX,
              cosY * cosZ,
              sinXsinY * cosZ - cosXsinZ,
              cosXcosZ * sinY + sinX * sinZ);

    VectorSet(&axisY,
              cosY * sinZ,
              sinXsinY * sinZ + cosXcosZ,
              cosXsinZ * sinY - sinX * cosZ);
}

inline
void QuatToAxes::Convert(nn::util::Vector3fType* pAxes, const BoneAnimResult* pResult) NN_NOEXCEPT
{
    Convert(pAxes, pResult->quat);
}

inline
bool QuatToAxes::Convert(BoneAnimBlendResult* pOutResult, const BoneAnimBlendResult* pResult) NN_NOEXCEPT
{
    nn::util::Float4 quat;
    nn::util::VectorStore(&quat, pResult->rotate);
    Convert(pOutResult->axes, quat);
    return true;
}

inline
void QuatToAxes::Convert(nn::util::Vector3fType* pAxes, const nn::util::Float4& quat) NN_NOEXCEPT
{
    nn::util::Vector3fType& axisX = pAxes[0];
    nn::util::Vector3fType& axisY = pAxes[1];

    float yy2 = 2 * quat.v[1] * quat.v[1];
    float zz2 = 2 * quat.v[2] * quat.v[2];
    float xx2 = 2 * quat.v[0] * quat.v[0];
    float xy2 = 2 * quat.v[0] * quat.v[1];
    float xz2 = 2 * quat.v[0] * quat.v[2];
    float yz2 = 2 * quat.v[1] * quat.v[2];
    float wz2 = 2 * quat.v[3] * quat.v[2];
    float wx2 = 2 * quat.v[3] * quat.v[0];
    float wy2 = 2 * quat.v[3] * quat.v[1];

    VectorSet(&axisX,
              1.0f - yy2 - zz2,
              xy2 - wz2,
              xz2 + wy2);

    VectorSet(&axisY,
              xy2 + wz2,
              1.0f - xx2 - zz2,
              yz2 - wx2);
}

inline
bool AxesToMtx::Convert(nn::util::Matrix4x3fType* pMtx, const BoneAnimBlendResult* pResult) NN_NOEXCEPT
{
    const nn::util::Vector3fType& axisX = pResult->axes[0];
    const nn::util::Vector3fType& axisY = pResult->axes[1];
    nn::util::Vector3fType axisZ;

    VectorCross(&axisZ, axisX, axisY);

    float lengthSqY = VectorLengthSquared(axisY);
    float lengthSqZ = VectorLengthSquared(axisZ);
    if (lengthSqY == 0.0f || lengthSqZ == 0.0f)
    {
        // 縮退していた場合は何も書き込まない。
        return false;
    }
    float rsqrtY = 1.0f / std::sqrt(lengthSqY);
    float rsqrtZ = 1.0f / std::sqrt(lengthSqZ);

    nn::util::Vector3fType unitX;
    nn::util::Vector3fType unitY;
    nn::util::Vector3fType unitZ;

    VectorMultiply(&unitY, axisY, rsqrtY);
    VectorMultiply(&unitZ, axisZ, rsqrtZ);
    VectorCross(&unitX, unitY, unitZ);
    MatrixSetColumn0(pMtx, unitX);
    MatrixSetColumn1(pMtx, unitY);
    MatrixSetColumn2(pMtx, unitZ);
    return true;
}

}} // namespace nn::g3d

