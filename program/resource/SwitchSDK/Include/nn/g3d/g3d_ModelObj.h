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
* @brief モデルに関するインスタンスクラスの宣言
*/

#pragma once

#include <nn/gfx.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResModel.h>
#include <nn/g3d/g3d_SkeletonObj.h>
#include <nn/g3d/g3d_ShapeObj.h>
#include <nn/g3d/g3d_MaterialObj.h>
#include <nn/g3d/detail/g3d_Flag.h>

namespace nn { namespace g3d {

//! @brief モデルインスタンスです。
class ModelObj
{
public:
    class Builder;

    //! @brief インスタンスの構築時に渡すバッファーの必要アライメントサイズです。
    enum Alignment
    {
        Alignment_Buffer       = SkeletonObj::Alignment_Buffer, //!< @brief Builder に渡すバッファーの必要アライメントサイズです。
    };

    //! @briefprivate ボーンビジビリティ変更時に呼ばれるコールバックの型。
    typedef void (*BoneVisibilityCallback)(ModelObj* pModelObj, int idxBone);

    //! @briefprivate マテリアルビジビリティ変更時に呼ばれるコールバックの型。
    typedef void (*MaterialVisibilityCallback)(ModelObj* pModelObj, int idxMaterial);

    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 実際の構築処理は Builder で行います。
    //!
    ModelObj() NN_NOEXCEPT
        : m_pRes(NULL)
        , m_pBoneVisArray(NULL)
        , m_pMatVisArray(NULL)
        , m_ViewCount(0)
        , m_ViewDependent(0)
        , m_Flag(0)
        , m_pViewBlockArray(NULL)
        , m_pViewBlockArrayView(NULL)
        , m_ShapeCount(0)
        , m_MaterialCount(0)
        , m_pSkeleton(NULL)
        , m_pShapeArray(NULL)
        , m_pMaterialArray(NULL)
        , m_pBounding(NULL)
        , m_pUserPtr(NULL)
        , m_pBufferPtr(NULL)
        , m_pMemoryPool(NULL)
        , m_MemoryPoolOffset(0)
        , m_pBoneVisibilityCallback(NULL)
        , m_pMaterialVisibilityCallback(NULL)
        , m_ShapeUserAreaForceUpdate(false)
        , m_LodCount(0)
    {
    }

    //! @brief ユニフォームブロックのサイズを計算します。
    //!
    //! @return 計算したバッファーサイズを返します。
    //!
    //! @details
    //! 配下の ShapeObj, MaterialObj, SkeletonObj のバッファーサイズも合わせて計算します。
    //!
    size_t CalculateBlockBufferSize(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @brief メモリプールを渡して、ユニフォームブロックを構築します。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @param[in] pMemoryPool メモリプールへのポインター。
    //! @param[in] offset メモリプールの使用する領域へのオフセット。
    //! @param[in] memoryPoolSize offset以降で使用可能なメモリプールのサイズ。
    //!
    //! @return 構築に成功した場合は true、失敗した場合は false を返します。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - pMemoryPool が初期化されている。
    //! - offset で指されたメモリプールの位置が nn::gfx::Buffer::GetBufferAlignment() のアラインメント上にある。
    //! - memoryPoolSize は CalculateBlockBufferSize() 以上である。
    //! - ユニフォームブロックが未構築である。
    //!
    //! @post
    //! - ユニフォームブロックが構築されている
    //!
    //! @details
    //! - 構築後の動的頂点バッファーに対して CPU アクセスを行う場合や、SetShapeAnimCalculationEnabled() を呼び出し、
    //! シェイプアニメーションの適用処理が有効に設定されている場合、メモリープールの特性は CpuCached が推奨されます。
    //! - CpuUncached を設定した場合はパフォーマンスが低下します。
    //!
    bool SetupBlockBuffer(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t offset, size_t memoryPoolSize) NN_NOEXCEPT;

    //! @brief ユニフォームブロックを破棄します。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - ユニフォームブロックが構築済みである。
    //!
    //! @post
    //!- ユニフォームブロックが破棄されている。
    //!
    void CleanupBlockBuffer(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    //! @brief ローカル行列からワールド行列を計算します。
    //!
    //! @param[in] baseMtx ベース行列。
    //!
    void CalculateWorld(const nn::util::Matrix4x3fType& baseMtx) NN_NOEXCEPT;

    //! @brief バウンディング球を計算します。
    void CalculateBounding() NN_NOEXCEPT
    {
        CalculateBounding(0);
    }

    //! @brief 指定したLODレベルのバウンディング球を計算します。
    //!
    //! @param[in] lodLevel LODレベル。
    //!
    //! @details
    //! すべての ShapeObj を走査し、モデルのバウンディング球を計算します。
    //! ShapeObj が指定した LOD レベルを持たない場合、LOD レベル 0 のデータで代用し、
    //! 計算を行います。
    //!
    void CalculateBounding(int lodLevel) NN_NOEXCEPT;

    //! @brief ワールド行列から行列パレットを計算します。
    //!
    //! @param[in] bufferIndex 計算を反映させるユニフォームブロックのインデックス。
    //!
    //! @pre
    //! - インデックスはスケルトンが持つユニフォームブロックのバッファー数の範囲内。
    //!
    //! @details
    //! スケルトンのユニフォームブロックに計算結果は反映されます。
    //!
    //! GPU から参照されるバッファーを書き換えるため、
    //! 前回の描画で GPU が参照し終わった後に呼ぶ必要があります。
    //! 構築時のオプションでダブルバッファー化するか、
    //! 描画完了待ち後に呼ぶことを想定しています
    //!
    void CalculateSkeleton(int bufferIndex) NN_NOEXCEPT;

    //! @brief シェイプに関する描画リソースを計算します。
    //!
    //! @param[in] bufferIndex 計算を反映させるユニフォームブロックのインデックス。
    //!
    //! @pre
    //! - インデックスはシェイプが持つユニフォームブロックのバッファー数の範囲内。
    //!
    //! @details
    //! リジッドボディかつビューに依存しないシェイプに対して計算を行います。
    //! シェイプのユニフォームブロックに計算結果は反映されます。
    //!
    //! GPU から参照されるバッファーを書き換えるため、
    //! 前回の描画で GPU が参照し終わった後に呼ぶ必要があります。
    //! 構築時のオプションでダブルバッファー化するか、
    //! 描画完了待ち後に呼ぶことを想定しています
    //!
    void CalculateShape(int bufferIndex) NN_NOEXCEPT;

    //! @brief マテリアルに関する描画リソースを計算します。
    //!
    //! @param[in] bufferIndex 計算を反映させるユニフォームブロックのインデックス。
    //!
    //! @pre
    //! - インデックスはマテリアルが持つユニフォームブロックのバッファー数の範囲内。
    //!
    //! @details
    //! マテリアルのユニフォームブロックに計算結果は反映されます。
    //!
    //! GPU から参照されるバッファーを書き換えるため、
    //! 前回の描画で GPU が参照し終わった後に呼ぶ必要があります。
    //! 構築時のオプションでダブルバッファー化するか、
    //! 描画完了待ち後に呼ぶことを想定しています
    //!
    void CalculateMaterial(int bufferIndex) NN_NOEXCEPT;

    //! @brief ビューに関する描画リソースを計算します。
    //!
    //! @param[in] viewIndex 計算を反映させるビューのインデックス。
    //! @param[in] cameraMtx カメラ行列。
    //! @param[in] bufferIndex 計算を反映させるユニフォームブロックのインデックス。
    //!
    //! @pre
    //! - viewIndexはビュー数の範囲内。
    //! - bufferIndexはシェイプが持つユニフォームブロックのバッファー数の範囲内。
    //!
    //! @details
    //! リジッドボディかつビューに依存するシェイプに対して計算を行います。
    //! シェイプのユニフォームブロックに計算結果は反映されます。
    //!
    //! GPU から参照されるバッファーを書き換えるため、
    //! 前回の描画で GPU が参照し終わった後に呼ぶ必要があります。
    //! 構築時のオプションでダブルバッファー化するか、
    //! 描画完了待ち後に呼ぶことを想定しています
    //!
    void CalculateView(int viewIndex, const nn::util::Matrix4x3fType& cameraMtx, int bufferIndex) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief ユニフォームブロックが要求するアライメントを取得します。
    //!
    //! @return ユニフォームブロックが要求するアライメントを返します。
    //!
    size_t GetBlockBufferAlignment(nn::gfx::Device* pDevice) const NN_NOEXCEPT;

    //! @brief リソースを取得します。
    //!
    //! @return ResModel へのポインターを返します。
    //!
    const ResModel* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    }

    //! @brief Builder で Build() 時に渡されたバッファーを取得します。
    //!
    //! @return Builder で Build() 時に渡されたバッファーへのポインターを返します。
    //!
    void* GetBufferPtr() NN_NOEXCEPT
    {
        return m_pBufferPtr;
    }

    //! @brief SetupBlockBuffer() 時に渡されたメモリプールのポインターを取得します。
    //!
    //! @return メモリプールへのポインターを返します。
    //!
    nn::gfx::MemoryPool* GetMemoryPoolPtr() NN_NOEXCEPT
    {
        return m_pMemoryPool;
    }

    //! @brief SetupBlockBuffer() 時に渡されたメモリプールのオフセットを取得します。
    //!
    //! @return メモリプールのオフセットを返します。
    //!
    ptrdiff_t GetMemoryPoolOffset() NN_NOEXCEPT
    {
        return m_MemoryPoolOffset;
    }

    //! @brief モデル配下のオブジェクトのユニフォームブロックが構築済みであるかどうかを取得します。
    //!
    //! @return 構築済みの場合は true、未構築の場合は false を返します。
    //!
    bool IsBlockBufferValid() const NN_NOEXCEPT
    {
        return (m_Flag & Flag_BlockBufferValid) != 0;
    }

    //! @brief ビューの数を取得します。
    //!
    //! @return ビューの数を返します。
    //!
    //! @details
    //! Builder の ViewCount で指定した数です。
    //!
    int GetViewCount() const NN_NOEXCEPT
    {
        return m_ViewCount;
    }

    //! @brief モデルがビューに依存するかどうかを取得します。
    //!
    //! @return 依存する場合は ture、依存しない場合は false を返します。
    //!
    bool IsViewDependent() const NN_NOEXCEPT
    {
        return !!m_ViewDependent;
    }

    //! @brief モデルがビューに依存するかどうかを更新します。
    //!
    //! ビューに依存するかどうかは初期化時に設定されるため、通常は呼ぶ必要はありません。
    //!
    void UpdateViewDependency() NN_NOEXCEPT;

    //! @brief ワールド座標系におけるモデルのバウンディング球を取得します。
    //!
    //! @return Sphere へのポインターを返します。
    //!
    //! @details
    //! 構築時にバウンディングを無効に指定した場合は NULL を返します。
    //!
    Sphere* GetBounding() NN_NOEXCEPT
    {
        return GetBounding(0);
    }

    //! @brief ワールド座標系におけるモデルのバウンディング球を取得します。
    //!
    //! @return Sphere へのポインターを返します。
    //!
    //! @details
    //! 構築時にバウンディングを無効に指定した場合は NULL を返します。
    //!
    const Sphere* GetBounding() const NN_NOEXCEPT
    {
        return GetBounding(0);
    }

    //! @brief ワールド座標系における指定した LOD のモデルのバウンディング球を取得します。
    //!
    //! @param[in] lodLevel lod レベル。
    //!
    //! @return Sphere へのポインターを返します。
    //!
    //! @details
    //! 構築時にバウンディングを無効に指定した場合は NULL を返します。
    //!
    Sphere* GetBounding(int lodLevel) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(lodLevel, 0, GetLodCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        if (m_pBounding)
        {
            return &m_pBounding[lodLevel];
        }
        else
        {
            return NULL;
        }
    }

    //! @brief ワールド座標系における指定した LOD のモデルのバウンディング球を取得します。
    //!
    //! @param[in] lodLevel lod レベル。
    //!
    //! @return Sphere へのポインターを返します。
    //!
    //! @details
    //! 構築時にバウンディングを無効に指定した場合は NULL を返します。
    //!
    const Sphere* GetBounding(int lodLevel) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(lodLevel, 0, GetLodCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        if (m_pBounding)
        {
            return &m_pBounding[lodLevel];
        }
        else
        {
            return NULL;
        }
    }

    //! @brief モデルの持つ LOD 数を返します。
    //!
    //! @return モデルの持つ LOD 数。
    //!
    int GetLodCount() const NN_NOEXCEPT
    {
        return m_LodCount;
    }

    //! @brief ユーザーポインターを設定します。
    //!
    //! @param[in] pUserPtr ポインター。
    //!
    void SetUserPtr(void* pUserPtr) NN_NOEXCEPT
    {
        m_pUserPtr = pUserPtr;
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    void* GetUserPtr() NN_NOEXCEPT
    {
        return m_pUserPtr;
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    const void* GetUserPtr() const NN_NOEXCEPT
    {
        return m_pUserPtr;
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    T* GetUserPtr() NN_NOEXCEPT
    {
        return static_cast<T*>(m_pUserPtr);
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    const T* GetUserPtr() const NN_NOEXCEPT
    {
        return static_cast<const T*>(m_pUserPtr);
    }

    //! @brief モデル名を取得します。
    //!
    //! @return ResModel のモデル名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return m_pRes->GetName();
    }

    //! @brief シェイプがリジッドボディ以外の場合でもシェイプのユーザーエリアを更新するように設定します。
    //!
    //! @details
    //! デフォルトではシェイプのユーザーエリアはリジッドボディの場合でのみ更新されます。
    //!
    void SetShapeUserAreaForceUpdateEnabled() NN_NOEXCEPT
    {
        m_ShapeUserAreaForceUpdate = true;
    }

    //! @brief シェイプのユーザーエリアの更新設定をデフォルトに戻します。
    //!
    //! @details
    //! デフォルトではシェイプのユーザーエリアはリジッドボディの場合でのみ更新されます。
    //!
    void SetShapeUserAreaForceUpdateDisabled() NN_NOEXCEPT
    {
        m_ShapeUserAreaForceUpdate = false;
    }

    //! @brief シェイプのユーザーエリアの更新設定の状態を取得します。
    //!
    //! @return
    //! true の場合は、シェイプがリジッドボディ以外の場合でもシェイプのユーザーエリアを更新します。
    //! false の場合は、シェイプがリジッドボディの場合のみシェイプのユーザーエリアを更新します。
    //!
    bool IsShapeUserAreaForceUpdateEnabled() const NN_NOEXCEPT
    {
        return m_ShapeUserAreaForceUpdate;
    }

    //! @brief CPU 上でシェイプアニメーションの結果を計算し、動的頂点バッファーに適用する設定を有効にします。
    //!
    //! @details
    //! - CPU で更新する設定を無効にし、GPU で更新を行う処理を実装することで、
    //! 高速にシェイプアニメーションの計算結果を頂点バッファーに反映させることが可能です。
    //!
    void SetShapeAnimCalculationEnabled() NN_NOEXCEPT;

    //! @brief CPU 上でシェイプアニメーションの結果を計算し、動的頂点バッファーに適用する設定を無効にします。
    //!
    //! @details
    //! - CPU で更新する設定を無効にし、GPU で更新を行う処理を実装することで、
    //! 高速にシェイプアニメーションの計算結果を頂点バッファーに反映させることが可能です。
    //!
    void SetShapeAnimCalculationDisabled() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name ビジビリティ
    //@{

    //! @brief 指定したシェイプのビジビリティを取得します。
    //!
    //! @param[in] shapeIndex シェイプのインデックス。
    //!
    //! @return 表示状態の場合 true、非表示状態の場合 false を返します。
    //!
    //! @details
    //! IsBoneVisible() と IsMaterialVisible() を合わせたビジビリティを取得します。
    //!
    bool IsShapeVisible(int shapeIndex) const NN_NOEXCEPT
    {
        const ShapeObj* pShape = GetShape(shapeIndex);
        return IsMaterialVisible(pShape->GetMaterialIndex()) &&
            IsBoneVisible(pShape->GetBoneIndex());
    }

    //! @brief 指定したボーンのビジビリティを取得します。
    //!
    //! @param[in] boneIndex ボーンのインデックス。
    //!
    //! @return 表示状態の場合 true、非表示状態の場合 false を返します。
    //!
    bool IsBoneVisible(int boneIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(boneIndex, 0, GetSkeleton()->GetBoneCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        return IsBitOn<bool>(m_pBoneVisArray, boneIndex);
    }

    //! @brief 指定したボーンのビジビリティを設定します。
    //!
    //! @param[in] boneIndex ボーンのインデックス。
    //! @param[in] visible 表示する場合は true, 非表示の場合は false。
    //!
    void SetBoneVisible(int boneIndex, bool visible) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(boneIndex, 0, GetSkeleton()->GetBoneCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));

        bool oldVisible = IsBoneVisible(boneIndex);
        SetBit(m_pBoneVisArray, boneIndex,  visible ? 1 : 0);
        if (m_pBoneVisibilityCallback && (oldVisible != visible))
        {
            m_pBoneVisibilityCallback(this, boneIndex);
        }
    }

    //! @brief ボーンのビジビリティをリソースに基づいて初期化します。
    //!
    //! Builder で Build() 時に呼ばれるため、意図的に結果を初期化したい場合以外は呼ぶ必要はありません。
    //!
    void ClearBoneVisible() NN_NOEXCEPT;

    //! @briefprivate ボーンビジビリティ変更時に呼ばれるコールバックを設定します。
    //!
    //! @param[in] pCallback コールバックへのポインター。
    //!
    void SetBoneVisibilityCallback(BoneVisibilityCallback pCallback) NN_NOEXCEPT
    {
        m_pBoneVisibilityCallback = pCallback;
    }

    //! @briefprivate ボーンビジビリティ変更時に呼ばれるコールバックを取得します。
    //!
    //! @return SetBoneVisibilityCallback() で設定したコールバックへのポインターを返します。
    //!
    BoneVisibilityCallback GetBoneVisibilityCallback() const NN_NOEXCEPT
    {
        return m_pBoneVisibilityCallback;
    }

    //! @brief 指定したマテリアルのビジビリティを取得します。
    //!
    //! @param[in] matIndex マテリアルのインデックス。
    //!
    //! @return 表示状態の場合 true、非表示状態の場合 false を返します。
    //!
    bool IsMaterialVisible(int matIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(matIndex, 0, GetMaterialCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        return IsBitOn<bool>(m_pMatVisArray, matIndex);
    }

    //! @brief 指定したマテリアルのビジビリティを設定します。
    //!
    //! @param[in] matIndex マテリアルのインデックス。
    //! @param[in] visible 表示する場合は true, 非表示の場合は false。
    //!
    void SetMaterialVisible(int matIndex, bool visible) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(matIndex, 0, GetMaterialCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));

        bool oldVisible = IsMaterialVisible(matIndex);
        SetBit(m_pMatVisArray, matIndex,  visible ? 1 : 0);
        if (m_pMaterialVisibilityCallback && (oldVisible != visible))
        {
            m_pMaterialVisibilityCallback(this, matIndex);
        }
    }

    //! @brief マテリアルのビジビリティをリソースに基づいて初期化します。
    //!
    //! Builder で Build() 時に呼ばれるため、意図的に結果を初期化したい場合以外は呼ぶ必要はありません。
    //!
    void ClearMaterialVisible() NN_NOEXCEPT;

    //! @briefprivate マテリアルビジビリティ変更時に呼ばれるコールバックを設定します。
    //!
    //! @param[in] pCallback コールバックへのポインター。
    //!
    void SetMaterialVisibilityCallback(MaterialVisibilityCallback pCallback) NN_NOEXCEPT
    {
        m_pMaterialVisibilityCallback = pCallback;
    }

    //! @briefprivate マテリアルビジビリティ変更時に呼ばれるコールバックを取得します。
    //!
    //! @return SetMaterialVisibilityCallback() で設定したコールバックへのポインターを返します。
    //!
    MaterialVisibilityCallback GetMaterialVisibilityCallback() const NN_NOEXCEPT
    {
        return m_pMaterialVisibilityCallback;
    }
    //@}

    //----------------------------------------
    //! @name スケルトン
    //@{

    //! @brief スケルトンを取得します。
    //!
    //! @return SkeletonObj へのポインターを返します。
    //!
    SkeletonObj* GetSkeleton() NN_NOEXCEPT
    {
        return m_pSkeleton;
    }

    //! @brief スケルトンを取得します。
    //!
    //! @return SkeletonObj へのポインターを返します。
    //!
    const SkeletonObj* GetSkeleton() const NN_NOEXCEPT
    {
        return m_pSkeleton;
    }

    //@}

    //----------------------------------------
    //! @name シェイプ
    //@{

    //! @brief モデル配下のシェイプの数を取得します。
    //!
    //! @return モデル配下のシェイプ数を返します。
    //!
    int GetShapeCount() const NN_NOEXCEPT
    {
        return m_ShapeCount;
    }

    //! @brief インデックス引きでシェイプを取得します。
    //!
    //! @param[in] shapeIndex シェイプのインデックス。
    //!
    //! @return ShapeObj へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはモデルが持つシェイプ数の範囲内。
    //!
    ShapeObj* GetShape(int shapeIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(shapeIndex, 0, GetShapeCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        return &m_pShapeArray[shapeIndex];
    }

    //! @brief インデックス引きでシェイプを取得します。
    //!
    //! @param[in] shapeIndex シェイプのインデックス。
    //!
    //! @return ShapeObj へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはモデルが持つシェイプ数の範囲内。
    //!
    const ShapeObj* GetShape(int shapeIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(shapeIndex, 0, GetShapeCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        return &m_pShapeArray[shapeIndex];
    }

    //! @brief 名前引きでシェイプを取得します。
    //!
    //! @param[in] name シェイプ名へのポインター。
    //!
    //! @return ShapeObj へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェイプが存在しない場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ShapeObj* FindShape(const char* name) NN_NOEXCEPT
    {
        int shapeIndex = m_pRes->FindShapeIndex(name);
        return shapeIndex >= 0 ? GetShape(shapeIndex) : NULL;
    }

    //! @brief 名前引きでシェイプを取得します。
    //!
    //! @param[in] name シェイプ名へのポインター。
    //!
    //! @return ShapeObj へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェイプが存在しない場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ShapeObj* FindShape(const char* name) const NN_NOEXCEPT
    {
        int shapeIndex = m_pRes->FindShapeIndex(name);
        return shapeIndex >= 0 ? GetShape(shapeIndex) : NULL;
    }

    //! @brief インデックスからシェイプの名前を取得します。
    //!
    //! @param[in] shapeIndex シェイプのインデックス。
    //!
    //! @return シェイプ名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはモデルが持つシェイプ数の範囲内。
    //!
    const char* GetShapeName(int shapeIndex) const NN_NOEXCEPT
    {
        return m_pRes->GetShapeName(shapeIndex);
    }

    //! @brief シェイプの名前からインデックスを取得します。
    //!
    //! @param[in] name シェイプ名へのポインター。
    //!
    //! @return シェイプのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のシェイプが存在しない場合は  nn::util::ResDic::Npos を返します。
    //!
    int FindShapeIndex(const char* name) const NN_NOEXCEPT
    {
        return m_pRes->FindShapeIndex(name);
    }

    //@}

    //----------------------------------------
    //! @name マテリアル
    //@{

    //! @brief モデル配下のマテリアル数を取得します。
    //!
    //! @return モデル配下のマテリアル数を返します。
    //!
    int GetMaterialCount() const NN_NOEXCEPT
    {
        return m_MaterialCount;
    }

    //! @brief インデックス引きでマテリアルを取得します。
    //!
    //! @param[in] matIndex マテリアルのインデックス。
    //!
    //! @return MaterialObj へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはモデルが持つマテリアル数の範囲内。
    //!
    MaterialObj* GetMaterial(int matIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(matIndex, 0, GetMaterialCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        return &m_pMaterialArray[matIndex];
    }

    //! @brief インデックス引きでマテリアルを取得します。
    //!
    //! @param[in] matIndex マテリアルのインデックス。
    //!
    //! @return MaterialObj へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはモデルが持つマテリアル数の範囲内。
    //!
    const MaterialObj* GetMaterial(int matIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(matIndex, 0, GetMaterialCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        return &m_pMaterialArray[matIndex];
    }

    //! @brief 名前引きでマテリアルを取得します。
    //!
    //! @param[in] name マテリアル名。
    //!
    //! @return ShapeObj へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のマテリアルが存在しない場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    MaterialObj* FindMaterial(const char* name) NN_NOEXCEPT
    {
        int matIndex = m_pRes->FindMaterialIndex(name);
        return matIndex >= 0 ? GetMaterial(matIndex) : NULL;
    }

    //! @brief 名前引きでマテリアルを取得します。
    //!
    //! @param[in] name マテリアル名。
    //!
    //! @return ShapeObj へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のマテリアルが存在しない場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const MaterialObj* FindMaterial(const char* name) const NN_NOEXCEPT
    {
        int matIndex = m_pRes->FindMaterialIndex(name);
        return matIndex >= 0 ? GetMaterial(matIndex) : NULL;
    }

    //! @brief インデックスからマテリアルの名前を取得します。
    //!
    //! @param[in] matIndex マテリアルのインデックス。
    //!
    //! @return マテリアル名へのポインターを返します。
    //!
    //! @pre
    //! -インデックスはモデルが持つマテリアル数の範囲内。
    //!
    const char* GetMaterialName(int matIndex) const NN_NOEXCEPT
    {
        return m_pRes->GetMaterialName(matIndex);
    }

    //! @brief マテリアルの名前からインデックスを取得します。
    //!
    //! 指定した名前のマテリアルが存在しない場合は  nn::util::ResDic::Npos を返します。
    //!
    //! @param[in] name マテリアル名へのポインター。
    //!
    //! @return マテリアルのインデックスを返します。
    //!
    int FindMaterialIndex(const char* name) const NN_NOEXCEPT
    {
        return m_pRes->FindMaterialIndex(name);
    }

    //! @briefprivate テクスチャー差し替え時に呼ばれるコールバックを設定します。
    //!
    //! @param[in] pCallback テクスチャー差し替え時に呼ばれるコールバックへのポインター
    //!
    void SetTextureChangeCallback(MaterialObj::TextureChangeCallback pCallback) NN_NOEXCEPT;
    //@}

protected:

    //! @brief モデルの状態を表すフラグです。
    enum Flag
    {
        Flag_BlockBufferValid  = 0x1 << 0 //!< @briefprivate ユニフォームブロックが構築済みであるかどうかを表すフラグです。
    };

private:
    class InitializeArgument;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    const ResModel*  m_pRes;

    Bit32*           m_pBoneVisArray; // ボーンのビジビリティビット配列
    Bit32*           m_pMatVisArray; // マテリアルのビジビリティビット配列
    uint8_t          m_ViewCount;
    uint8_t          m_ViewDependent;
    Bit16            m_Flag;
    nn::gfx::Buffer* m_pViewBlockArray; // ビュー行列とビルボード行列用ブロックの配列
    nn::gfx::Buffer* m_pViewBlockArrayView;

    uint16_t         m_ShapeCount;
    uint16_t         m_MaterialCount;

    SkeletonObj*     m_pSkeleton;
    ShapeObj*        m_pShapeArray;
    MaterialObj*     m_pMaterialArray;
    Sphere*          m_pBounding;

    void*            m_pUserPtr;
    void*            m_pBufferPtr;

    nn::gfx::MemoryPool* m_pMemoryPool;
    ptrdiff_t            m_MemoryPoolOffset;

    BoneVisibilityCallback     m_pBoneVisibilityCallback;
    MaterialVisibilityCallback m_pMaterialVisibilityCallback;
    bool m_ShapeUserAreaForceUpdate;
    int m_LodCount;
    NN_DISALLOW_COPY(ModelObj);
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate ModelObj::Initialize() に渡して初期化を行うパラメータです。
class ModelObj::InitializeArgument
{
public:
    //! @brief メモリブロックのインデックスを示す列挙子。
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_SkeletonBuffer,
        MemoryBlockIndex_ShapeBuffer,
        MemoryBlockIndex_MaterialBuffer,
        MemoryBlockIndex_Skeleton,
        MemoryBlockIndex_ShapeArray,
        MemoryBlockIndex_MaterialArray,
        MemoryBlockIndex_BoneVisArray,
        MemoryBlockIndex_MatVisArray,
        MemoryBlockIndex_Bounding,
        MemoryBlockIndex_End
    };

    //! コンストラクタです。
    //!
    //! @param[in] resource ResModel へのポインター
    //!
    explicit InitializeArgument(const ResModel* resource) NN_NOEXCEPT
        : m_pRes(resource)
        , m_pSkeleton(NULL)
        , m_SkeletonBufferingCount(1)
        , m_ShapeBufferingCount(1)
        , m_MaterialBufferingCount(1)
        , m_ViewCount(1)
        , m_LodCount(1)
        , m_ShapeUserAreaSize(0)
        , m_BoundingEnabled(false)
        , m_MemoryBlock()
    {
        NN_SDK_REQUIRES_NOT_NULL(resource);
        m_WorkMemory.Invalidate();
        // モデルのシェイプを走査し、LOD数を算出する
        for (int shapeIndex = 0; shapeIndex < m_pRes->GetShapeCount(); ++shapeIndex)
        {
            const nn::g3d::ResShape* pResShape = m_pRes->GetShape(shapeIndex);
            m_LodCount = std::max(m_LodCount, pResShape->GetMeshCount());
        }
    }

    //! @brief 共有スケルトンを設定します。
    //!
    //! @param[in] pSkeleton SkeletonObj へのポインター。
    //!
    //! @pre
    //! - 設定する SkeletonObj は設定するリソースと同一のスケルトンリソースを扱うインスタンスである。
    //!
    //! @details
    //! 構築済み SkeletonObj を渡すことで他の ModelObj とスケルトンを共有することができます。
    //! 共有することで計算処理をスキップすることができます。
    //!
    void Skeleton(SkeletonObj* pSkeleton) NN_NOEXCEPT
    {
        m_pSkeleton = pSkeleton;
    }

    //! @brief スケルトンのユニフォームブロックをバッファーリングする数を設定します。
    //!
    //! @param[in] count スケルトンのユニフォームブロックのバッファー数。
    //!
    void SkeletonBufferingCount(int count) NN_NOEXCEPT
    {
        m_SkeletonBufferingCount = count;
    }

    //! @brief シェイプのユニフォームブロックをバッファーリングする数を設定します。
    //!
    //! @param[in] count シェイプのユニフォームブロックのバッファー数。
    //!
    void ShapeBufferingCount(int count) NN_NOEXCEPT
    {
        m_ShapeBufferingCount = count;
    }

    //! @brief マテリアルのユニフォームブロックをバッファーリングする数を設定します。
    //!
    //! @param[in] count マテリアルのユニフォームブロックのバッファー数。
    //!
    void MaterialBufferingCount(int count) NN_NOEXCEPT
    {
        m_MaterialBufferingCount = count;
    }

    //! @brief ビューの数を設定します。
    //!
    //! @param[in] count ビューの数。
    //!
    //! @details
    //! ビューに依存するユニフォームブロックはビューの数に応じて複数構築されます。
    //!
    void ViewCount(int count) NN_NOEXCEPT
    {
        m_ViewCount = count;
    }

    //! @brief シェイプのユーザーエリアサイズを設定します。
    //!
    //! @param[in] size シェイプのユーザーエリアサイズ。
    //!
    void ShapeUserAreaSize(size_t size) NN_NOEXCEPT
    {
        m_ShapeUserAreaSize = size;
    }

    //! @brief バウンディング計算を有効に指定します。
    void SetBoundingEnabled() NN_NOEXCEPT
    {
        m_BoundingEnabled = true;
    }

    //! @brief バウンディング計算を無効に指定します。
    void SetBoundingDisabled() NN_NOEXCEPT
    {
        m_BoundingEnabled = false;
    }

    //! @brief リソースを取得します。
    //!
    //! @return ResModelへのポインターを返します。
    //!
    const ResModel* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    }

    //! @brief スケルトンを取得します。
    //!
    //! @return SkeletonObjへのポインターを返します。
    //!
    SkeletonObj* GetSkeleton() const NN_NOEXCEPT
    {
        return m_pSkeleton;
    }

    //! @brief スケルトンのユニフォームブロックのバッファーリング数を取得します。
    //!
    //! @return スケルトンのユニフォームブロックのバッファーリング数を返します。
    //!
    int GetSkeletonBufferingCount() const NN_NOEXCEPT
    {
        return m_SkeletonBufferingCount;
    }

    //! @brief シェイプのユニフォームブロックのバッファーリング数を取得します。
    //!
    //! @return シェイプのユニフォームブロックのバッファーリング数を返します。
    //!
    int GetShapeBufferingCount() const NN_NOEXCEPT
    {
        return m_ShapeBufferingCount;
    }

    //! @brief マテリアルのユニフォームブロックのバッファーリング数を取得します。
    //!
    //! @return マテリアルのユニフォームブロックのバッファーリング数を返します。
    //!
    int GetMaterialBufferingCount() const NN_NOEXCEPT
    {
        return m_MaterialBufferingCount;
    }

    //! @brief ビュー用の数を取得します。
    //!
    //! @return ビューの数を返します。
    //!
    int GetViewCount() const NN_NOEXCEPT
    {
        return m_ViewCount;
    }

    //! @brief モデルのLODの数を取得します。
    //!
    //! @return Lodの数を返します。
    //!
    int GetLodCount() const NN_NOEXCEPT
    {
        return m_LodCount;
    }

    //! @brief シェイプのユーザーエリアサイズを取得します。
    //!
    //! @return シェイプのユーザーエリアサイズを返します。
    //!
    size_t GetShapeUserAreaSize() const NN_NOEXCEPT
    {
        return m_ShapeUserAreaSize;
    }

    //! @brief バウンディング計算が有効かどうかを取得します。
    //!
    //! @return バウンディング計算が有効な場合は true、無効な場合 false を返します。
    //!
    bool IsBoundingEnabled() const NN_NOEXCEPT
    {
        return m_BoundingEnabled;
    }

    //! @brief ModelObj構築に必要なメモリサイズを取得します。
    //!
    //! @return ModelObj構築に必要なメモリサイズを返します。
    //!
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief ModelObj構築に必要なメモリサイズを計算します。
    //!
    //! @pre
    //! - 設定されたスケルトンのユニフォームブロックのバッファーリング数が１以上。
    //! - 設定されたシェイプのユニフォームブロックのバッファーリング数が１以上。
    //! - 設定されたマテリアルのユニフォームブロックのバッファーリング数が１以上。
    //! - 設定されたビューの数が１以上。
    //!
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

    //! @brief ModelObj 構築に必要なメモリサイズが計算済みかを取得します。
    bool IsMemoryCalculated() const NN_NOEXCEPT
    {
        return m_WorkMemory.IsValid();
    }

private:
    const ResModel*                       m_pRes;
    SkeletonObj*                          m_pSkeleton;
    int                                   m_SkeletonBufferingCount;
    int                                   m_ShapeBufferingCount;
    int                                   m_MaterialBufferingCount;
    int                                   m_ViewCount;
    int                                   m_LodCount;
    size_t                                m_ShapeUserAreaSize;
    bool                                  m_BoundingEnabled;
    nn::util::MemorySplitter              m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
};

//! @brief ModelObj の構築を行うクラスです。
class ModelObj::Builder : public ModelObj::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @param[in] resource ResModel へのポインター
    //!
    explicit Builder(const ResModel* resource) NN_NOEXCEPT
        : InitializeArgument(resource)
    {
        NN_SDK_REQUIRES_NOT_NULL(resource);
    }

    //! @brief MaterialObj を構築します。
    //!
    //! @param[in] pModelObj ModelObj へのポインター。
    //! @param[in] pBuffer バッファーへのポインター。
    //! @param[in] bufferSize バッファーのサイズ。
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである。
    //! - pBuffer は Alignment_Buffer のアライメント上にある。
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ。
    //!
    bool Build(ModelObj* pModelObj, void* pBuffer, size_t bufferSize) const NN_NOEXCEPT
    {
        return pModelObj->Initialize(*this, pBuffer, bufferSize);
    }
};
}} // namespace nn::g3d

