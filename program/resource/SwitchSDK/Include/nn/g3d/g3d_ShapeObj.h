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
* @brief シェイプに関するインスタンスクラスの宣言
*/

#pragma once

#include <nn/gfx.h>
#include <nn/util/util_MemorySplitter.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResShape.h>
#include <nn/g3d/detail/g3d_MatrixUtility.h>

namespace nn { namespace g3d {

class SkeletonObj;

struct ShapeBlock
{
    nn::util::FloatColumnMajor4x3 worldMtx;
    int32_t vtxSkinCount;
    NN_PADDING4;
    NN_PADDING8;
    union
    {
        float    userFloat[48];
        int32_t  userInt[48];
        uint32_t userUInt[48];
    };
    enum Size
    {
        Size_SystemArea = 64,
        Size_UserArea   = 192
    };
};

//! @brief 交差判定用の球です。
struct Sphere
{
    //! @brief 指定した行列で球を変換した結果を設定します。
    //!
    //! @param[in, out] sphere バウンディング球への参照。
    //! @param[in] mtx 行列への参照。
    //!
    void Transform(const Sphere& sphere, const nn::util::Matrix4x3fType& mtx) NN_NOEXCEPT;

    //! @brief 2つの球を囲う球を設定します。
    //!
    //! @param[in] lhs バウンディング球への参照。
    //! @param[in] rhs バウンディング球への参照。
    //!
    void Merge(const Sphere& lhs, const Sphere& rhs) NN_NOEXCEPT;

    //! 中心。
    nn::util::Vector3fType center;
    //! 半径。
    float radius;
};

//! @brief 交差判定用の AABB(Axis-Aligned-Bounding-Box) です。
struct Aabb
{
    //! @brief 指定した頂点群を囲う AABB を設定します。
    //!
    //! @param[in] pPointArray 頂点配列。
    //! @param[in] count 頂点数。
    //!
    void Set(const nn::util::Vector3fType* pPointArray, int count) NN_NOEXCEPT;

    //! @brief 指定した行列でバウンディングボリュームを変換した結果を設定します。
    //!
    //! @param[in] aabb バウンディングボリュームへの参照。
    //! @param[in] mtx 行列への参照。
    //!
    void Transform(const Bounding& aabb, const nn::util::Matrix4x3fType& mtx) NN_NOEXCEPT;

    //! @brief 2つの AABB を囲う AABB を設定します。
    //!
    //! @param[in] lhs Aabbへの参照。
    //! @param[in] rhs Aabbへの参照。
    //!
    void Merge(const Aabb& lhs, const Aabb& rhs) NN_NOEXCEPT;

    //! 最小座標。
    nn::util::Vector3fType min;
    //! 最大座標。
    nn::util::Vector3fType max;
};

//! @brief 交差判定用の平面です。
struct Plane
{
    //! @brief 3つの頂点を通る平面を設定します。
    //!
    //! @param[in] p0 頂点への参照。
    //! @param[in] p1 頂点への参照。
    //! @param[in] p2 頂点への参照。
    //!
    void Set(const nn::util::Vector3fType& p0, const nn::util::Vector3fType& p1, const nn::util::Vector3fType& p2) NN_NOEXCEPT;

    //! 法線。
    nn::util::Vector3fType  normal;
    //! dot( normal, pos ) + dist = 0 となる値です。符号に注意してください。
    float dist;
};

//! @brief ビューボリュームです。
struct ViewVolume
{
    //! @brief ビューボリュームを設定します。
    //!
    //! @param[in] fovy 縦の視野角(ラジアン)。
    //! @param[in] aspect アスペクト比。
    //! @param[in] zNear 一番近いZ位置。
    //! @param[in] zFar 一番遠いZ位置。
    //! @param[in] viewToWorld ワールド座標への変換行列。
    //!
    void SetPerspective(float fovy, float aspect, float zNear, float zFar, const nn::util::Matrix4x3fType& viewToWorld) NN_NOEXCEPT;

    //! @brief ビューボリュームを設定します。
    //!
    //! @param[in] top 視錐台の上座標。
    //! @param[in] bottom 視錐台の下座標。
    //! @param[in] left 視錐台の左座標。
    //! @param[in] right 視錐台の右座標。
    //! @param[in] zNear 一番近いZ位置。
    //! @param[in] zFar 一番遠いZ位置。
    //! @param[in] viewToWorld ワールド座標への変換行列。
    //!
    void SetFrustum(float top, float bottom, float left, float right, float zNear, float zFar,
        const nn::util::Matrix4x3fType& viewToWorld) NN_NOEXCEPT;
    //! @brief ビューボリュームを設定します。
    //!
    //! @param[in] top 視野空間の上座標。
    //! @param[in] bottom 視野空間の下座標。
    //! @param[in] left 視野空間の左座標。
    //! @param[in] right 視野空間の右座標。
    //! @param[in] zNear 一番近いZ位置。
    //! @param[in] zFar 一番遠いZ位置。
    //! @param[in] viewToWorld ワールド座標への変換行列。
    //!
    void SetOrtho(float top, float bottom, float left, float right, float zNear, float zFar,
        const nn::util::Matrix4x3fType& viewToWorld) NN_NOEXCEPT;

    //! @brief 交差判定を行います。
    //!
    //! @param[in] sphere 球への参照。
    //!
    //! @return  ビューボリュームに対して球が外側の場合は false、それ以外の場合は true を返します。
    //!
    bool TestIntersection(const Sphere& sphere) const NN_NOEXCEPT;

    //! @brief 交差判定を行います。
    //!
    //! @param[in] sphere 球への参照。
    //!
    //! @return ビューボリュームに対して球が外側の場合に負の値を、内側の場合に正の値を交差している場合に 0 を返します。
    //!
    int TestIntersectionEx(const Sphere& sphere) const NN_NOEXCEPT;

    //! @brief 交差判定を行います。
    //!
    //! @param[in] aabb Aabbへの参照。
    //!
    //! @return ビューボリュームに対して AABB が外側の場合は false、それ以外の場合は true を返します。
    //!
    bool TestIntersection(const Aabb& aabb) const NN_NOEXCEPT;

    //! @brief 交差判定を行います。
    //!
    //! @param[in] aabb AABBへの参照。
    //!
    //! @return ビューボリュームに対して AABB が外側の場合に負の値を、内側の場合に正の値を、
    //! 交差している場合に 0 を返します。
    //!
    int TestIntersectionEx(const Aabb& aabb) const NN_NOEXCEPT;

    //! @brief AABB による事前判定を有効化します。
    void SetAabbEnabled() NN_NOEXCEPT
    {
        flag = 1;
    }
    //! @brief AABB による事前判定を無効化します。
    void SetAabbDisabled() NN_NOEXCEPT
    {
        flag = 0;
    }
    //! @brief AABB による事前判定が有効かどうかを取得します。
    //!
    //! @return 事前判定が有効な場合は true、無効な場合は false を返します。
    //!
    bool IsAabbEnabled() const NN_NOEXCEPT
    {
        return !!flag;
    }

    //! ボリュームを囲う AABB です。AABB との交差判定で事前判定に使用することができます。
    Aabb  aabb;
    //! 平面。
    Plane planes[6];
    //! 平面数。
    int   planeCount;
    int   flag;
};

class ShapeObj;

//! @briefprivate サブメッシュ LOD のカリングの際、LOD レベルの判定に用いる関数オブジェクトです。
struct ICalculateLodLevelFunctor
{
public:
    static const int InvaidLodLevel = -1;

    virtual int operator() (const Aabb& bounding, const ShapeObj& shape) NN_NOEXCEPT = 0;
};

//! @brief カリングに用いるコンテクストです。
struct CullingContext
{
    CullingContext() : nodeIndex(0), nodeLodLevel(ICalculateLodLevelFunctor::InvaidLodLevel),
        submeshIndex(0), submeshCount(0), submeshLodLevel(0) {}

    int nodeIndex; //!< ノードのインデックス。
    int nodeLodLevel; //!< ノードのLODレベル。
    int submeshIndex; //!< サブメッシュのインデックス。
    int submeshCount; //!< サブメッシュ数。
    int submeshLodLevel; //!< サブメッシュのLODレベル。
};

//! @brief サブメッシュの範囲です。
struct SubMeshRange
{
    uint16_t index; //!< 範囲の先頭サブメッシュのインデックスです。
    uint16_t count; //!< 範囲のサブメッシュ数です。
    uint16_t lodLevel; //!< 範囲の LOD レベルです。

    //! @brief 2つのサブメッシュの範囲配列を合成して重複している範囲配列を計算します。
    //!
    //! @param[out] pDst SubMeshRange配列へのポインター。
    //! @param[in] pLHS SubMeshRange配列へのポインター。
    //! @param[in] pRHS SubMeshRange配列へのポインター。
    //!
    //! @return サブメッシュの個数を返します。
    //!
    //! @details
    //! pLHS と pRHS は index と count が共に 0 の SubMeshRange で終端されている必要があります。
    //! pDst は index と count が共に 0 の SubMeshRange で終端され、終端を含まない
    //! サブメッシュの個数が戻り値として返されます。
    //! 合成の際、LOD レベルが異なる場合は、より削られるほう、すなわち大きいほうが採用されます。
    //!
    static int And(SubMeshRange* pDst, const SubMeshRange* pLHS, const SubMeshRange* pRHS) NN_NOEXCEPT;
};

//! @brief シェイプインスタンスです。
class ShapeObj
{
public:
    class Builder;

    //! @brief インスタンスの構築時に渡すバッファーの必要アライメントサイズです。
    enum Alignment
    {
        Alignment_Buffer = MatrixVectorAlignment, //!< @brief Builder に渡すバッファーの必要アライメントサイズです。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    //!
    //! 実際の構築処理は Builder で行います。
    //!
    ShapeObj() NN_NOEXCEPT
        : m_pRes(NULL)
        , m_Flag(0)
        , m_ViewCount(0)
        , m_ViewDependent(0)
        , m_ShapeBlockCount(0)
        , m_BufferingCount(0)
        , m_pMemoryPool(NULL)
        , m_MemoryPoolOffset(0)
        , m_pShapeBlockArray(NULL)
        , m_pBlendWeightArray(NULL)
        , m_pBlendWeightValidFlags(NULL)
        , m_pBounding(NULL)
        , m_pSubMeshBounding(NULL)
        , m_pDynamicVertexBufferArrayPtr(NULL)
        , m_pUserArea(NULL)
        , m_UserAreaSize(0)
        , m_pUserPtr(NULL)
        , m_pBufferPtr(NULL)
    {
    }

    //! @brief ユニフォームブロックのサイズを計算します。
    //!
    //! @return 計算したバッファーサイズを返します。
    //!
    size_t CalculateBlockBufferSize(nn::gfx::Device* pDevice) const NN_NOEXCEPT;

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
    //! @param[in] pDevice デバイスへのポインター
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

    //! @brief メッシュ0 のバウンディング球を計算します。
    //!
    //! @param[in] pSkeleton SkeletonObjへのポインター。
    //!
    //! @pre
    //! - 構築時にバウンディング計算を有効にしている。
    //!
    //! @details
    //! 前提条件を満たさない場合、この関数は何も行いません。
    //!
    void CalculateBounding(const SkeletonObj* pSkeleton) NN_NOEXCEPT
    {
        CalculateBounding(pSkeleton, 0);
    }

    //! @brief 指定したメッシュのバウンディング球を計算します。
    //!
    //! @param[in] pSkeleton SkeletonObjへのポインター。
    //! @param[in] meshIndex メッシュのインデックス。 LOD レベルを指定します。
    //!
    //! @pre
    //! - 構築時にバウンディング計算を有効にしている。
    //! - インデックスはメッシュの数の範囲内。
    //!
    //! @details
    //! 前提条件を満たさない場合、この関数は何も行いません。
    //!
    void CalculateBounding(const SkeletonObj* pSkeleton, int meshIndex) NN_NOEXCEPT;

    //! @brief メッシュ 0 のサブメッシュのバウンディングボックスを計算します。
    //!
    //! @param[in] pSkeleton SkeletonObjへのポインター。
    //!
    //! @pre
    //! - 構築時にバウンディング計算を有効にしている。
    //! - 剛体である。
    //!
    //! @details
    //! 前提条件を満たさない場合、この関数は何も行いません。
    //!
    void CalculateSubMeshBounding(const SkeletonObj* pSkeleton) NN_NOEXCEPT
    {
        CalculateSubMeshBounding(pSkeleton, 0);
    }

    //! @brief 指定したメッシュのサブメッシュのバウンディングボックスを計算します。
    //!
    //! @param[in] pSkeleton SkeletonObjへのポインター。
    //! @param[in] meshIndex メッシュのインデックス。 LOD レベルを指定します。
    //!
    //! @pre
    //! - 構築時にバウンディング計算を有効にしている。
    //! - 剛体である。
    //! - インデックスはメッシュの数の範囲内。
    //!
    //! @details
    //! 前提条件を満たさない場合、この関数は何も行いません。
    //!
    void CalculateSubMeshBounding(const SkeletonObj* pSkeleton, int meshIndex) NN_NOEXCEPT;

    //! @brief シェイプに関する描画リソースを計算します。
    //!
    //! @param[in] viewIndex 計算を反映させるビューのインデックス。
    //! @param[in] worldMtx ワールド変換行列。
    //! @param[in] bufferIndex 計算を反映させるユニフォームブロックのインデックス。
    //!
    //! @details
    //! GPU から参照されるバッファーを書き換えるため、
    //! 前回の描画で GPU が参照し終わった後に呼ぶ必要があります。
    //! 構築時のオプションでダブルバッファー化するか、
    //! フレームバッファーのコピーアウト中などに呼ぶことを想定しています。
    //!
    void CalculateShape(int viewIndex, const nn::util::Matrix4x3fType& worldMtx, int bufferIndex) NN_NOEXCEPT;

    //! @brief 動的頂点バッファーの値を元の頂点バッファーの値に戻します。
    //!
    //! @param[in] bufferIndex クリアする対象のバッファーのインデックス。
    //!
    //! @details
    //! GPU から参照されるバッファーを書き換えるため、
    //! 前回の描画で GPU が参照し終わった後に呼ぶ必要があります。
    //! 構築時のオプションでダブルバッファー化するか、
    //! フレームバッファーのコピーアウト中などに呼ぶことを想定しています。
    //!
    void ClearDynamicVertexBuffer(int bufferIndex) NN_NOEXCEPT;

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
    //! @return ResShape へのポインターを返します。
    //!
    const ResShape* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    }

    //! @brief Builder でBuild() 時に渡されたバッファーを取得します。
    //!
    //! @return Builder でBuild() 時に渡されたバッファーへのポインターを返します。
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

    //! @brief ユニフォームブロックが構築済みであるかどうかを取得します。
    //!
    //! @return 構築済みの場合は true、未構築の場合は false を返します。
    //!
    bool IsBlockBufferValid() const NN_NOEXCEPT
    {
        return (m_Flag & Flag_BlockBufferValid) != 0;
    }

    //! @brief 動的頂点バッファーが構築済みであるかどうかを取得します。
    //!
    //! @return 構築済みの場合は true、未構築の場合は false を返します。
    //!
    bool IsDynamicVertexBufferValid() const NN_NOEXCEPT
    {
        return (m_Flag & Flag_DynamicVertexBufferValid) != 0;
    }

    //! @brief 参照する ResMaterial へのインデックスを取得します。
    //!
    //! @return 参照する ResMaterial へのインデックスを返します。
    //!
    int GetMaterialIndex() const NN_NOEXCEPT
    {
        return m_pRes->GetMaterialIndex();
    }

    //! @brief 参照する ResBone へのインデックスを取得します。
    //!
    //! @return 参照する ResBone へのインデックスを返します。
    //!
    int GetBoneIndex() const NN_NOEXCEPT
    {
        return m_pRes->GetBoneIndex();
    }

    //! @brief 参照する ResVertex へのインデックスを取得します。
    //!
    //! @return 参照する ResVertex へのインデックスを返します。
    //!
    int GetVertexIndex() const NN_NOEXCEPT
    {
        return m_pRes->GetVertexIndex();
    }

    //! @brief スムーススキニングに必要なボーン数を取得します。
    //!
    //! @return スムーススキニングに必要なボーン数を返します。
    //!
    int GetVertexSkinCount() const NN_NOEXCEPT
    {
        return m_pRes->GetVertexSkinCount();
    }

    //! @brief 剛体のシェイプかどうかを取得します。
    //!
    //! @return 剛体の場合は true、それ以外の場合は false を返します。
    //!
    bool IsRigidBody() const NN_NOEXCEPT
    {
        return m_pRes->IsRigidBody();
    }

    //! @brief リジッドスキニングのシェイプかどうかを取得します。
    //!
    //! @return ジッドスキニングのシェイプの場合は true、それ以外の場合は false を返します。
    //!
    bool IsRigidSkinning() const NN_NOEXCEPT
    {
        return m_pRes->IsRigidSkinning();
    }

    //! @brief スムーススキニングのシェイプかどうかを取得します。
    //!
    //! @return スムーススキニングのシェイプの場合は true、それ以外の場合は false を返します。
    //!
    bool IsSmoothSkinning() const NN_NOEXCEPT
    {
        return m_pRes->IsSmoothSkinning();
    }

    //! @brief メッシュを取得します。
    //!
    //! @return ResMesh へのポインターを返します。
    //!
    const ResMesh* GetResMesh() const NN_NOEXCEPT
    {
        return m_pRes->GetMesh(0);
    }

    //! @brief メッシュを取得します。
    //!
    //! @param[in] meshIndex メッシュのインデックス。 LOD レベルを指定します。
    //!
    //! @return ResMesh のポインターを返します。
    //!
    const ResMesh* GetResMesh(int meshIndex) const NN_NOEXCEPT
    {
        return m_pRes->GetMesh(meshIndex);
    }

    //! @brief メッシュの数を取得します。
    //!
    //! @return メッシュの数を返します。
    //!
    int GetMeshCount() const NN_NOEXCEPT
    {
        return m_pRes->GetMeshCount();
    }

    //! @brief ビューの数を取得します。
    //!
    //! @return ビューの数を返します。
    //!
    int GetViewCount() const NN_NOEXCEPT
    {
        return m_ViewCount;
    }

    //! @brief サブメッシュの数を取得します。
    //!
    //! @return サブメッシュの数を返します。
    //!
    int GetSubMeshCount() const NN_NOEXCEPT
    {
        return GetSubMeshCount(0);
    }

    //! @brief 指定したメッシュのサブメッシュの数を取得します。
    //!
    //! @param[in] meshIndex メッシュのインデックス。 LOD レベルを指定します。
    //!
    //! @return サブメッシュの数を返します。
    //!
    int GetSubMeshCount(int meshIndex) const NN_NOEXCEPT
    {
        return m_pRes->GetSubMeshCount(meshIndex);
    }

    //! @brief シェイプがビューに依存するかどうかを取得します。
    //!
    //! @return シェイプがビューに依存する場合は true、依存しない場合は false を返します。
    //!
    bool IsViewDependent() const NN_NOEXCEPT
    {
        return m_ViewDependent != 0;
    }

    //! @brief シェイプ単位のユニフォームブロックの数を取得します。
    //!
    //! @return シェイプ単位のユニフォームブロックの数を返します。
    //!
    int GetShapeBlockCount() const NN_NOEXCEPT
    {
        return m_ShapeBlockCount;
    }

    //! @brief シェイプ単位のユニフォームブロックのバッファーを取得します。
    //!
    //! @param[in] viewIndex ビューのインデックス。
    //! @param[in] bufferIndex ユニフォームブロックのインデックス。
    //!
    //! @return nn::gfx::BufferView へのポインターを返します。
    //!
    //! @pre
    //! - viewIndex はマテリアルが持つビュー数の範囲内。
    //! - bufferIndex はマテリアルが持つバッファー数の範囲内。
    //!
    //! @details
    //! ビューごとに Build() 時に指定したバッファー数のユニフォームブロック用バッファーを持ちます。
    //! ユニフォームブロックが未構築の場合、NULL を返します。
    //!
    nn::gfx::Buffer* GetShapeBlock(int viewIndex, int bufferIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(viewIndex, 0, GetViewCount(),        NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        NN_G3D_REQUIRES_RANGE(bufferIndex, 0, GetBufferingCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        if (!IsBlockBufferValid())
        {
            return NULL;
        }
        if (IsViewDependent())
        {
            return m_pShapeBlockArray ? &m_pShapeBlockArray[viewIndex * m_BufferingCount + bufferIndex] : NULL;
        }
        else
        {
            return m_pShapeBlockArray ? &m_pShapeBlockArray[bufferIndex] : NULL;
        }
    }

    //! @brief シェイプ単位のユニフォームブロックのバッファーを取得します。
    //!
    //! @param[in] viewIndex ビューのインデックス。
    //! @param[in] bufferIndex ユニフォームブロックのインデックス。
    //!
    //! @return nn::gfx::BufferView へのポインターを返します。
    //!
    //! @pre
    //! - viewIndex はマテリアルが持つビュー数の範囲内。
    //! - bufferIndex はマテリアルが持つバッファー数の範囲内。
    //!
    //! @details
    //! ビューごとに Build() 時に指定したバッファー数のユニフォームブロック用バッファーを持ちます。
    //! ユニフォームブロックが未構築の場合、NULL を返します。
    //!
    const nn::gfx::Buffer* GetShapeBlock(int viewIndex, int bufferIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(viewIndex, 0, GetViewCount(),        NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        NN_G3D_REQUIRES_RANGE(bufferIndex, 0, GetBufferingCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        if (!IsBlockBufferValid())
        {
            return NULL;
        }
        if (IsViewDependent())
        {
            return m_pShapeBlockArray ? &m_pShapeBlockArray[viewIndex * m_BufferingCount + bufferIndex] : NULL;
        }
        else
        {
            return m_pShapeBlockArray ? &m_pShapeBlockArray[bufferIndex] : NULL;
        }
    }

    //! @brief ユニフォームブロックのバッファーリング数を取得します。
    //!
    //! @return ユニフォームブロックのバッファーリング数を返します。
    //!
    int GetBufferingCount() const NN_NOEXCEPT
    {
        return m_BufferingCount;
    }

    //! @brief ユニフォームブロックのエンディアンスワップを行うかどうかを取得します。
    //!
    //! @return ユニフォームブロックをスワップする場合は true、しない場合は false を返します。
    //!
    bool IsBlockSwapEnabled() const NN_NOEXCEPT
    {
#if defined(NN_BUILD_CONFIG_OS_COS)
        return true;
#else
        return false;
#endif
    }

    //! @brief ユーザーポインターを設定します。
    //!
    //! @param[in] pUserPtr ポインターを返します。
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

    //! @brief シェイプ名を取得します。
    //!
    //! @return ResShape のシェイプ名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return m_pRes->GetName();
    }

    //! @brief ユーザーエリアへのポインターを取得します。
    //!
    //! @return ユーザーエリアへのポインターを返します。
    //!
    void* GetUserArea() NN_NOEXCEPT
    {
        return m_pUserArea;
    }

    //! @brief ユーザーエリアへのポインターを取得します。
    //!
    //! @return ユーザーエリアへのポインターを返します。
    //!
    const void* GetUserArea() const NN_NOEXCEPT
    {
        return m_pUserArea;
    }

    //! @brief ユーザーエリアへのポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return ユーザーエリアへのポインターを返します。
    //!
    template <typename T>
    T* GetUserArea() NN_NOEXCEPT
    {
        return static_cast<T*>(m_pUserArea);
    }

    //! @brief ユーザーエリアへのポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return ユーザーエリアへのポインターを返します。
    //!
    template <typename T>
    const T* GetUserArea() const NN_NOEXCEPT
    {
        return static_cast<const T*>(m_pUserArea);
    }

    //! @brief ユーザーエリアのサイズを取得します。
    //!
    //! @return ユーザーエリアのサイズを返します。
    //!
    size_t GetUserAreaSize() const NN_NOEXCEPT
    {
        return m_UserAreaSize;
    }

    //@}

    //----------------------------------------
    //! @name 頂点情報
    //@{

    //! @brief 参照する ResVertex を取得します。
    //!
    //! @return ResVertex へのポインターを返します。
    //!
    const ResVertex* GetResVertex() const NN_NOEXCEPT
    {
        return m_pRes->GetVertex();
    }

    //! @brief 参照する ResVertex の頂点属性の数を取得します。
    //!
    //! @return 参照する ResVertex の頂点属性の数を返します。
    //!
    int GetVertexAttrCount() const NN_NOEXCEPT
    {
        return GetResVertex()->GetVertexAttrCount();
    }

    //! @brief インデックス引きで頂点属性を取得します。
    //!
    //! @param[in] attrIndex 頂点属性のインデックス。
    //!
    //! @return ResVertexAttr へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェイプが持つ頂点属性数の範囲内。
    //!
    const ResVertexAttr* GetResVertexAttr(int attrIndex) const NN_NOEXCEPT
    {
        return GetResVertex()->GetVertexAttr(attrIndex);
    }

    //! @brief 名前引きで頂点属性を取得します。
    //!
    //! @param[in] name 頂点属性名へのポインター。
    //!
    //! @return ResVertexAttr へのポインターを返します。
    //!
    //! @details
    //! 指定した名前の頂点属性が存在しない場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResVertexAttr* FindResVertexAttr(const char* name) const NN_NOEXCEPT
    {
        return GetResVertex()->FindVertexAttr(name);
    }

    //! @brief インデックスから頂点属性名を取得します。
    //!
    //! @param[in] attribIndex 頂点属性のインデックス
    //!
    //! @return 頂点属性名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェイプが持つ頂点属性数の範囲内。
    //!
    const char* GetVertexAttrName(int attribIndex) const NN_NOEXCEPT
    {
        return GetResVertex()->GetVertexAttrName(attribIndex);
    }

    //! @brief 頂点属性名からインデックスを取得します。
    //!
    //! @param[in] name 頂点属性名へのポインター。
    //!
    //! @return 頂点属性のインデックスを返します。
    //!
    //! @details
    //! 指定した名前の頂点属性が存在しない場合は  nn::util::ResDic::Npos を返します。
    //!
    int FindVertexAttrIndex(const char* name) const NN_NOEXCEPT
    {
        return GetResVertex()->FindVertexAttrIndex(name);
    }

    //! @brief 参照する ResVertex の頂点バッファーの数を取得します。
    //!
    //! @return 参照する ResVertex の頂点バッファーの数を返します。
    //!
    int GetVertexBufferCount() const NN_NOEXCEPT
    {
        return GetResVertex()->GetVertexBufferCount();
    }

    //! @brief インデックスから頂点バッファーを取得します。
    //!
    //! @param[in] bufferIndex 頂点バッファーのインデックス。
    //!
    //! @return nn::gfx::Buffer へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェイプが持つ頂点バッファー数の範囲内。
    //!
    //! @details
    //! - 動的頂点バッファーを取得する場合は、GetDynamicVertexBuffer() を利用してください。
    //!
    const nn::gfx::Buffer* GetVertexBuffer(int bufferIndex) const NN_NOEXCEPT
    {
        return GetResVertex()->GetVertexBuffer(bufferIndex);
    }

    //! @brief インデックスから頂点バッファーの情報を取得します。
    //!
    //! @param[in] bufferIndex 頂点バッファーのインデックス。
    //!
    //! @return nn::gfx::BufferInfo へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェイプが持つ頂点バッファー数の範囲内。
    //!
    const nn::gfx::BufferInfo* GetVertexBufferInfo(int bufferIndex) const NN_NOEXCEPT
    {
        return GetResVertex()->GetVertexBufferInfo(bufferIndex);
    }

    //@}

    //----------------------------------------
    //! @name バウンディング
    //@{

    //! @brief ワールド座標系におけるシェイプのバウンディング球を取得します。
    //!
    //! @return ワールド座標系におけるシェイプのバウンディング球へのポインターを返します。
    //!
    //! @details
    //! 構築時にバウンディングを無効に指定した場合は NULL を返します。
    //!
    Sphere* GetBounding() NN_NOEXCEPT
    {
        return GetBounding(0);

    }
    //! @brief 指定したメッシュのワールド座標系におけるバウンディング球を取得します。
    //!
    //! param[in] meshIndex メッシュのインデックス。
    //!
    //! @return 指定したメッシュのワールド座標系におけるバウンディング球へのポインターを返します。
    //!
    //! @details
    //! 構築時にバウンディングを無効に指定した場合は NULL を返します。
    //!
    Sphere* GetBounding(int meshIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(meshIndex, 0, GetMeshCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        if (m_pBounding)
        {
            return m_pBounding + meshIndex * BoundingCoord_Count;
        }
        // バウンディングを持たない場合
        else
        {
            return NULL;
        }
    }

    //! @brief ワールド座標系におけるシェイプのバウンディング球を取得します。
    //!
    //! @return ワールド座標系におけるシェイプのバウンディング球へのポインターを返します。
    //!
    //! @details
    //! 構築時にバウンディングを無効に指定した場合は NULL を返します。
    //!
    const Sphere* GetBounding() const NN_NOEXCEPT
    {
        return GetBounding(0);
    }

    //! @brief 指定したメッシュのワールド座標系におけるバウンディング球を取得します。
    //!
    //! param[in] meshIndex メッシュのインデックス。
    //!
    //! @return 指定したメッシュのワールド座標系におけるバウンディング球へのポインターを返します。
    //!
    //! @details
    //! 構築時にバウンディングを無効に指定した場合は NULL を返します。
    //!
    const Sphere* GetBounding(int meshIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(meshIndex, 0, GetMeshCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        if (m_pBounding)
        {
            return m_pBounding + meshIndex * BoundingCoord_Count;
        }
        // バウンディングを持たない場合
        else
        {
            return NULL;
        }
    }

    //! @brief ワールド座標系におけるサブメッシュの AABB 配列を取得します。
    //!
    //! @return ワールド座標系におけるサブメッシュの AABB 配列へのポインターを返します。
    //!
    //! @details
    //! 構築時にバウンディングを無効に指定した場合は NULL を返します。
    //!
    const Aabb* GetSubMeshBoundingArray() const NN_NOEXCEPT
    {
        return m_pSubMeshBounding;
    }

    //! @brief ワールド座標系における指定したメッシュのサブメッシュの AABB 配列を取得します。
    //!
    //! @param[in] meshIndex メッシュのインデックス。 LOD レベルを指定します。
    //!
    //! @return ワールド座標系におけるサブメッシュの AABB 配列へのポインターを返します。
    //!
    //! @details
    //! 構築時にバウンディングを無効に指定した場合は NULL を返します。
    //!
    const Aabb* GetSubMeshBoundingArray(int meshIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(meshIndex, 0, GetMeshCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        if (m_pSubMeshBounding)
        {
            int startBoundingIndex = 0;
            for (int index = 0; index < meshIndex; ++index)
            {
                startBoundingIndex += GetSubMeshCount(index);
            }
            return &m_pSubMeshBounding[startBoundingIndex];
        }
        else
        {
            return NULL;
        }
    }

    //! @brief ビューボリュームと交差する連続したサブメッシュを取得します。
    //!
    //! @param[in] pCtx サブメッシュ列を保持するコンテクストへのポインター。
    //! @param[in] volume ビューボリュームへの参照。
    //!
    //! @return 交差または包含されるサブメッシュが存在する場合はtrue、存在しない場合は false を返します。
    //!
    //! @pre
    //! - 構築時にバウンディング計算を有効にしている。
    //!
    //! @details
    //! コンテクストが保持するサブメッシュ列から探索し、次に交差または包含される
    //! 連続したサブメッシュをコンテクストに書き込みます。 CullingContext::submeshIndex にビューボリューム内に入る
    //! 開始サブメッシュインデックス、 CullingContext::submeshCount に開始サブメッシュインデックス
    //! からビューボリューム内に入る連続するサブメッシュ数が格納されます。
    //!
    //! 探索は最後のサブメッシュに到達するか、ビューボリューム内に入るサブメッシュを見つけた後に続くサブメッシュを
    //! 探索し、ビューボリューム内に入らないものを見つけた場合に終了し、結果を返します。pCtxのデータを保持し、続けて
    //! この関数を呼び出すことによって、ビューボリューム内に入るすべてのサブメッシュを知ることができます。
    //!
    //! 得られた CullingContext を元に nn::g3d::ResMesh::DrawSubMesh() を呼び出し、
    //! これを TestSubMeshIntersection() が false を返すまで繰り返すことによって、サブメッシュカリングを実現できます。
    //!
    bool TestSubMeshIntersection(CullingContext* pCtx, const ViewVolume& volume) const NN_NOEXCEPT
    {
        return TestSubMeshIntersection(pCtx, volume, 0);
    }

    //! @brief 指定したメッシュにおけるビューボリュームと交差する連続したサブメッシュを取得します。
    //!
    //! @param[in] pCtx サブメッシュ列を保持するコンテクストへのポインター。
    //! @param[in] volume ビューボリュームへの参照。
    //! @param[in] meshIndex メッシュのインデックス。 LOD レベルを指定します。
    //!
    //! @return 交差または包含されるサブメッシュが存在する場合はtrue、存在しない場合は false を返します。
    //!
    //! @pre
    //! - 構築時にバウンディング計算を有効にしている。
    //!
    //! @details
    //! コンテクストが保持するサブメッシュ列から探索し、次に交差または包含される
    //! 連続したサブメッシュをコンテクストに書き込みます。 CullingContext::submeshIndex にビューボリューム内に入る
    //! 開始サブメッシュインデックス、 CullingContext::submeshCount に開始サブメッシュインデックス
    //! からビューボリューム内に入る連続するサブメッシュ数が格納されます。
    //!
    //! 探索は最後のサブメッシュに到達するか、ビューボリューム内に入るサブメッシュを見つけた後に続くサブメッシュを
    //! 探索し、ビューボリューム内に入らないものを見つけた場合に終了し、結果を返します。pCtxのデータを保持し、続けて
    //! この関数を呼び出すことによって、ビューボリューム内に入るすべてのサブメッシュを知ることができます。
    //!
    //! 得られた CullingContext を元に nn::g3d::ResMesh::DrawSubMesh() を呼び出し、
    //! これを TestSubMeshIntersection() が false を返すまで繰り返すことによって、サブメッシュカリングを実現できます。
    //!
    //! meshIndex に ShapeObj が保持するメッシュの数よりも大きな値が設定された場合には、
    //! コンテクストには何も書き込まず、早期に false を返します。
    //!
    bool TestSubMeshIntersection(CullingContext* pCtx, const ViewVolume& volume, int meshIndex) const NN_NOEXCEPT;

    //! @briefprivate ビューボリュームと交差する LOD レベルの等しい連続したサブメッシュを取得します。
    //!
    //! @param[in, out] pCtx サブメッシュ列を保持するコンテクストへのポインター。
    //! @param[in] volume ビューボリュームへの参照。
    //! @param[in] calcLodFunctor LOD レベルの判定に用いる関数オブジェクトへの参照。
    //!
    //! @return 交差または包含されるサブメッシュが存在する場合は true、存在しない場合は false を返します。
    //!
    //! @pre
    //! - 構築時にバウンディング計算を有効にしている。
    //!
    //! @details
    //! コンテクストが保持するサブメッシュ列から探索し、次に交差または包含される
    //! LOD レベルの等しい連続したサブメッシュをコンテクストに書き込みます。 CullingContext::submeshIndex にビューボリューム内に入る
    //! 開始サブメッシュインデックス、 CullingContext::submeshCount に開始サブメッシュインデックス
    //! からビューボリューム内に入る連続するサブメッシュ数、CullingContex::submeshLodLevel にはLODレベルが格納されます。
    //!
    //! 探索は最後のサブメッシュに到達するか、ビューボリューム内に入るサブメッシュを見つけた後に続くサブメッシュを
    //! 探索し、ビューボリューム内に入らないものを見つけた場合に終了し、結果を返します。pCtxのデータを保持し、続けて
    //! この関数を呼び出すことによって、ビューボリューム内に入るすべてのサブメッシュを知ることができます。
    //!
    //! 得られた CullingContext を元に nn::g3d::ResMesh::DrawSubMesh() を呼び出し、
    //! これを TestSubMeshIntersection() が false を返すまで繰り返すことによって、サブメッシュカリングを実現できます。
    //!
    //! この関数は LOD レベル 0 のバウンディング情報を使用し、計算を行います。
    //! 各 LOD モデルのバウンディングが LOD レベル 0 のものと大きく異なる場合に期待した結果にならない場合があります。
    //!
    bool TestSubMeshLodIntersection(CullingContext* pCtx, const ViewVolume& volume,
        ICalculateLodLevelFunctor& calcLodFunctor) const NN_NOEXCEPT;

    //! @brief ビューボリュームと交差する連続したサブメッシュの範囲配列を取得します。
    //!
    //! @param[in, out] pRangeArray SubMeshRange 配列へのポインター。
    //! @param[in] volume ビューボリュームへの参照。
    //!
    //! @return ビューボリュームと交差するサブメッシュの数を返します。
    //!
    //! @details
    //! pRangeArray は index と count と lodLevel がそれぞれ 0 の SubMeshRange で終端され、
    //! 終端を含まないサブメッシュの個数が戻り値として返されます。
    //! シェイプのサブメッシュ数を N とすると SubMeshRange の最大数は (N+1)/2 です。（終端を除く）
    //!
    int MakeSubMeshRange(SubMeshRange* pRangeArray, const ViewVolume& volume) const NN_NOEXCEPT
    {
        return MakeSubMeshRange(pRangeArray, volume, 0);
    }

    //! @brief メッシュのインデックスを指定して、ビューボリュームと交差する連続したサブメッシュの範囲配列を取得します。
    //!
    //! @param[in, out] pRangeArray SubMeshRange 配列へのポインター。
    //! @param[in] volume ビューボリュームへの参照。
    //! @param[in] meshIndex メッシュのインデックス。 LOD レベルを指定します。
    //!
    //! @return ビューボリュームと交差するサブメッシュの数を返します。
    //!
    //! @details
    //! pRangeArray は index と count と lodLevel がそれぞれ 0 の SubMeshRange で終端され、
    //! 終端を含まないサブメッシュの個数が戻り値として返されます。
    //! シェイプのサブメッシュ数を N とすると SubMeshRange の最大数は (N+1)/2 です。（終端を除く）
    //!
    int MakeSubMeshRange(SubMeshRange* pRangeArray, const ViewVolume& volume, int meshIndex) const NN_NOEXCEPT;

    //! @briefprivate ビューボリュームと交差する LOD レベルの等しい連続したサブメッシュの範囲配列を取得します。
    //!
    //! @param[in, out] pRangeArray SubMeshRange 配列へのポインター。
    //! @param[in] volume ビューボリュームへの参照。
    //! @param[in] calcLodFunctor LOD レベルの判定に用いる関数オブジェクトへの参照。
    //!
    //! @return ビューボリュームと交差するサブメッシュの数を返します。
    //!
    //! @details
    //! pRangeArray は index と count と lodLevel がそれぞれ 0 の SubMeshRange で終端され、
    //! 終端を含まないサブメッシュの個数が戻り値として返されます。
    //! シェイプのサブメッシュ数を N とすると SubMeshRange の最大数は N です。（終端を除く）
    //!
    int MakeSubMeshLodRange(SubMeshRange* pRangeArray, const ViewVolume& volume,
        ICalculateLodLevelFunctor& calcLodFunctor) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name キーシェイプ
    //@{

    //! @brief キーシェイプの数を取得します。
    //!
    //! @return キーシェイプの数。
    //!
    int GetKeyShapeCount() const NN_NOEXCEPT
    {
        return m_pRes->GetKeyShapeCount();
    }

    //! @brief インデックス引きでキーシェイプを取得します。
    //!
    //! @param[in] keyShapeIndex キーシェイプのインデックス。
    //!
    //! @return ResKeyShape へのポインターを返します。
    //!
    //! @pre
    //! インデックスはシェイプが持つキーシェイプ数の範囲内。
    //!
    const ResKeyShape* GetResKeyShape(int keyShapeIndex) const NN_NOEXCEPT
    {
        return m_pRes->GetKeyShape(keyShapeIndex);
    }

    //! @brief 名前引きでキーシェイプを取得します。
    //!
    //! @param[in] name キーシェイプ名へのポインター。
    //!
    //! @return ResKeyShape へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のキーシェイプが存在しない場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResKeyShape* FindResKeyShape(const char* name) const NN_NOEXCEPT
    {
        return m_pRes->FindKeyShape(name);
    }

    //! @brief インデックスからキーシェイプ名を取得します。
    //!
    //! @param[in] keyShapeIndex キーシェイプのインデックス。
    //!
    //! @return キーシェイプ名へのポインター。
    //!
    //! @details
    //! インデックスはシェイプが持つキーシェイプ数の範囲内。
    //!
    const char* GetKeyShapeName(int keyShapeIndex) const NN_NOEXCEPT
    {
        return m_pRes->GetKeyShapeName(keyShapeIndex);
    }

    //! @brief キーシェイプの名前からインデックスを取得します。
    //!
    //! @param[in] name キーシェイプ名へのポインター。
    //!
    //! @return キーシェイプのインデックス。
    //!
    //! @details
    //! 指定した名前のキーシェイプが存在しない場合は  nn::util::ResDic::Npos を返します。
    //!
    int FindKeyShapeIndex(const char* name) const NN_NOEXCEPT
    {
        return m_pRes->FindKeyShapeIndex(name);
    }

    //! @brief キーシェイプが持つ頂点属性の数を取得します。
    //!
    //! @return キーシェイプが持つ頂点属性の数。
    //!
    int GetTargetAttribCount() const NN_NOEXCEPT
    {
        return m_pRes->GetTargetAttribCount();
    }

    //! @brief キーシェイプのブレンドウェイトを初期化します。
    void ClearBlendWeights() NN_NOEXCEPT;

    //! @brief 有効なキーシェイプのブレンドウェイトをもつかどうかを取得します。
    //!
    //! @return ClearBlendWeights() 呼び出し後に SetBlendWeight() を呼んでいた場合に true、そうでないならfalse。
    //!
    bool HasValidBlendWeight() const NN_NOEXCEPT
    {
        return 0 != (m_Flag & Flag_BlendWeightValid);
    }

    //! @brief キーシェイプのブレンドウェイトを設定します。
    //!
    //! @param[in] keyShapeIndex キーシェイプのインデックス。
    //! @param[in] weight ブレンドウェイト。
    //!
    //! @pre
    //! - keyShapeIndex はシェイプが持つキーシェイプ数の範囲内。
    //!
    void SetBlendWeight(int keyShapeIndex, float weight) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(keyShapeIndex, 0, GetKeyShapeCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        m_pBlendWeightArray[keyShapeIndex] = weight;
        m_Flag |= Flag_BlendWeightValid;
        m_pBlendWeightValidFlags[keyShapeIndex >> 5] |= 0x01 << (keyShapeIndex & 0x1F);
    }

    //! @brief キーシェイプのブレンドウェイトを取得します。
    //!
    //! @param[in] keyShapeIndex キーシェイプのインデックス。
    //!
    //! @return ブレンドウェイトを返します。
    //!
    //! @pre
    //! - keyShapeIndexはシェイプが持つキーシェイプ数の範囲内。
    //!
    float GetBlendWeight(int keyShapeIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(keyShapeIndex, 0, GetKeyShapeCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        return m_pBlendWeightArray[keyShapeIndex];
    }

    //! @brief キーシェイプのブレンドウェイトが有効かどうかを取得します。
    //!
    //! @param[in] keyShapeIndex キーシェイプのインデックス。
    //!
    //! @return ClearBlendWeights() 呼び出し後に SetBlendWeight(keyShapeIndex) を呼んでいた場合は true、それ以外の場合は false を返します。
    //!
    //! @pre
    //! - keyShapeIndex はシェイプが持つキーシェイプ数の範囲内
    //!
    bool IsBlendWeightValid(int keyShapeIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(keyShapeIndex, 0, GetKeyShapeCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        return !!(m_pBlendWeightValidFlags[keyShapeIndex >> 5] & (0x01 << (keyShapeIndex & 0x1F)));
    }

    //! @brief 有効なキーシェイプのブレンドウェイトを数えます。
    //!
    //! @return 有効なキーシェイプのブレンドウェイトの数を返します。
    //!
    int CountValidBlendWeight() const NN_NOEXCEPT
    {
        int ret = 0;
        for (int idxFlag = 0, flagCount = nn::util::align_up(m_pRes->GetKeyShapeCount(), 32) >> 5;
            idxFlag < flagCount; ++idxFlag)
        {
            ret += CountOnes(m_pBlendWeightValidFlags[idxFlag]);
        }
        return ret;
    }

    //@}

    //----------------------------------------
    //! @name シェイプアニメーション
    //@{

    //! @brief CPU 上でシェイプアニメーションの結果を計算し、動的頂点バッファーに適用する設定を有効にします。
    //!
    //! @details
    //! - CPU で更新する設定を無効にし、GPU で動的頂点バッファーを更新する処理を実装することで、
    //! 高速にシェイプアニメーションの計算結果を頂点バッファーに反映させることが可能です。
    //! - 動的頂点バッファーが生成されていない場合には計算処理は行われません。
    //!
    void SetShapeAnimCalculationEnabled() NN_NOEXCEPT
    {
        m_Flag |= Flag_ShapeAnimCalculationEnabled;
    }

    //! @brief CPU 上でシェイプアニメーションの結果を計算し、動的頂点バッファーに適用する設定を無効にします。
    //!
    //! @details
    //! - CPU で更新する設定を無効にし、GPU で動的頂点バッファーを更新する処理を実装することで、
    //! 高速にシェイプアニメーションの計算結果を頂点バッファーに反映させることが可能です。
    //!
    void SetShapeAnimCalculationDisabled() NN_NOEXCEPT
    {
        m_Flag &= ~Flag_ShapeAnimCalculationEnabled;
    }

    //! @brief CPU 上でシェイプアニメーションの結果を計算し、動的頂点バッファーに適用する設定が有効になっているかどうかを取得します。
    //!
    //! @return
    //! CPU 上でシェイプアニメーションの結果を計算し、動的頂点バッファーに適用する設定が有効になっている場合は true、
    //! 無効になっている場合は false を返します。
    //!
    bool IsShapeAnimCalculationEnabled() const NN_NOEXCEPT
    {
        return (m_Flag & Flag_ShapeAnimCalculationEnabled) != 0;
    }

    //! @brief キーシェイプとブレンドウェイトから動的頂点バッファーを更新します。
    //!
    //! @param[in] bufferIndex バッファーリングのインデックス。
    //!
    //! @pre
    //! - bufferIndex >= 0 && bufferIndex < GetBufferingCount()
    //! - GetKeyShapeCount() > 0
    //! - IsDynamicVertexBufferValid()
    //!
    //! @details
    //! - HasValidBlendWeight() が false を返す場合、計算はスキップされます。
    //! - SetShapeAnimCalculationEnabled() が呼び出されていない場合でも、この関数を明示的に呼び出すことで
    //! 動的頂点バッファーを更新することが可能です。
    //!
    void CalculateShapeAnimResult(int bufferIndex) NN_NOEXCEPT;

    //! @brief インデックスから動的頂点バッファーを取得します。
    //!
    //! @param[in] vertexBufferIndex    動的頂点バッファーのインデックス。
    //! @param[in] bufferIndex          バッファーリングのインデックス。
    //!
    //! @return 動的頂点バッファーへのポインターを返します。
    //!
    //! @pre
    //! - vertexBufferIndex >= 0 && vertexBufferIndex < GetVertexBufferCount()
    //! - bufferIndex >= 0 && bufferIndex < GetBufferingCount()
    //!
    //! @details
    //! - GetVertexBuffer(vertexBufferIndex) で取得できるバッファーとデータ構造、サイズは同じです。
    //! - インデックスに対応する動的頂点バッファーが存在しない場合、NULL を返します。
    //! - ResVertexAttr::SetDynamicVertexAttrEnabled() を呼び出すことで、
    //! 任意の頂点属性を含む動的頂点バッファーを生成することが可能です。
    //! - 動的頂点バッファーは、アンオーダードアクセスバッファーとしてアクセスすることが可能です。
    //!
    const nn::gfx::Buffer* GetDynamicVertexBuffer(int vertexBufferIndex, int bufferIndex) const NN_NOEXCEPT;

    //! @brief 頂点属性のインデックスから動的頂点バッファーに含まれているかを取得します。
    //!
    //! @param[in] vertexAttrIndex 頂点属性のインデックス。
    //!
    //! @return 動的頂点バッファーに含まれている場合は true、含まれていない場合は false を返します。
    //!
    //! @pre
    //! - vertexAttrIndex >= 0 && vertexAttrIndex < GetVertexAttrCount()
    //!
    bool IsDynamicVertexAttr(int vertexAttrIndex) const NN_NOEXCEPT;

    //@}

    //! @briefprivate ユニフォームブロックのバッファー配列を設定します。
    //!
    //! @param[in] pBlockArray バッファー配列へのポインター。
    //!
    void SetShapeBlockArray(nn::gfx::Buffer* pBlockArray) NN_NOEXCEPT
    {
        m_pShapeBlockArray = pBlockArray;
    }

    //! @briefprivate ユニフォームブロックのバッファー配列を取得します。
    //!
    //! @return バッファー配列へのポインターを返します。
    //!
    nn::gfx::Buffer* GetShapeBlockArray() NN_NOEXCEPT
    {
        return m_pShapeBlockArray;
    }

    //! @briefprivate 動的頂点バッファー配列へのポインターを取得します。
    //!
    //! @return 動的頂点バッファー配列へのポインターを返します。
    //!
    nn::gfx::Buffer** GetDynamicVertexBufferArray() NN_NOEXCEPT
    {
        return m_pDynamicVertexBufferArrayPtr;
    }


protected:
    //! @brief シェイプの状態を表すフラグです。
    enum Flag
    {
        Flag_BlockBufferValid               = 0x1 << 0, //!< @briefprivate ユニフォームブロックが構築済みであるかどうかを表すフラグです。
        Flag_BlendWeightValid               = 0x1 << 1, //!< @briefprivate キーシェイプのブレンドウェイトが変更されたかどうかを表すフラグです。
        Flag_DynamicVertexBufferValid       = 0x1 << 2, //!< @briefprivate 動的頂点バッファーが構築済みであるかどうかを表すフラグです。
        Flag_ShapeAnimCalculationEnabled    = 0x1 << 3, //!< @briefprivate シェイプアニメーションの計算結果を頂点バッファーに適用するかどうかを表すフラグです。
    };

    enum BoundingCoord
    {
        BoundingCoord_Word     = 0,
        BoundingCoord_Local    = 1,
        BoundingCoord_Count
    };

private:
    class InitializeArgument;
    class Impl;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    size_t CalculateShapeBlockBufferSize(nn::gfx::Device* pDevice) const NN_NOEXCEPT;
    void SetupBlockBufferImpl(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t offset, size_t memoryPoolSize) NN_NOEXCEPT;

    void SetupDynamicVertexBuffer(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t offset, size_t memoryPoolSize) NN_NOEXCEPT;
    void CleanupDynamicVertexBuffer(nn::gfx::Device* pDevice) NN_NOEXCEPT;
    void SetupDynamicVertexBuffers(void* pBuffer, const InitializeArgument& arg) NN_NOEXCEPT;
    size_t CalculateDynamicVertexBufferSize(nn::gfx::Device* pDevice) const NN_NOEXCEPT;

    const ResShape*      m_pRes;

    Bit32                m_Flag;

    uint8_t              m_ViewCount;
    uint8_t              m_ViewDependent; // 0 or 1
    uint8_t              m_ShapeBlockCount;
    uint8_t              m_BufferingCount;
    nn::gfx::MemoryPool* m_pMemoryPool;
    ptrdiff_t            m_MemoryPoolOffset;
    nn::gfx::Buffer*     m_pShapeBlockArray; // 行列パレット非使用時の変換行列
    float*               m_pBlendWeightArray; // シェイプアニメーションにおけるキーシェイプ毎のウェイト
    Bit32*               m_pBlendWeightValidFlags; // 各キーシェイプのブレンドウェイトが有効かどうかのフラグ
    Sphere*              m_pBounding;
    Aabb*                m_pSubMeshBounding;
    nn::gfx::Buffer**    m_pDynamicVertexBufferArrayPtr;
    void*                m_pUserArea;
    size_t               m_UserAreaSize;

    void* m_pUserPtr;
    void* m_pBufferPtr;

    NN_DISALLOW_COPY(ShapeObj);
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate ShapeObj::Initialize() に渡して初期化を行うパラメータです。
class ShapeObj::InitializeArgument
{
public:
    //! @brief メモリブロックのインデックスを示す列挙子。
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_SubBoundingArray,
        MemoryBlockIndex_ShapeBlockArray,
        MemoryBlockIndex_BlendWeightArray,
        MemoryBlockIndex_BlendWeightFlags,
        MemoryBlockIndex_BoundingArray,
        MemoryBlockIndex_DynamicVertexBufferPtr,
        MemoryBlockIndex_DynamicVertexBufferArray,
        MemoryBlockIndex_UserArea,
        MemoryBlockIndex_End
    };

    //! @brief コンストラクタです。
    //!
    //! @param[in] resource ResShape へのポインター
    //!
    explicit InitializeArgument(const ResShape* resource) NN_NOEXCEPT
        : m_pRes(resource)
        , m_BufferingCount(1)
        , m_ViewCount(1)
        , m_ViewDependent(false)
        , m_BoundingEnabled(false)
        , m_UserAreaSize(0)
        , m_MemoryBlock()
    {
        NN_SDK_REQUIRES_NOT_NULL(resource);
        m_WorkMemory.Invalidate();
    }

    //! @brief ユニフォームブロックをバッファーリングする数を設定します。
    //!
    //! @param[in] count ユニフォームブロックをバッファーリングする数。
    //!
    void BufferingCount(int count) NN_NOEXCEPT
    {
        m_BufferingCount = count;
    }

    //! @brief ビューの数を設定します。
    //!
    //! @param[in] count ビューの数。
    //!
    //! @details
    //! ユニフォームブロックはビューの数に応じて複数構築されます。
    //!
    void ViewCount(int count) NN_NOEXCEPT
    {
        m_ViewCount = count;
    }

    //! @brief シェイプがビューに依存することを指定します。
    //!
    //! @details
    //! ユニフォームブロックはビューの数に応じて複数構築されます。
    //!
    void ViewDependent() NN_NOEXCEPT
    {
        m_ViewDependent = true;
    }

    //! @brief シェイプがビューに依存しないことを指定します。
    //!
    //! @details
    //! ユニフォームブロックはビュー間で共有されます。
    //!
    void ViewIndependent() NN_NOEXCEPT
    {
        m_ViewDependent = false;
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

    //! @brief ユーザーエリアのサイズを設定します。
    void UserAreaSize(size_t size) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(size <= ShapeBlock::Size_UserArea);
        m_UserAreaSize = size;
    }

    //! @brief ユニフォームブロックのバッファーリング数を取得します。
    //!
    //! @return ユニフォームブロックのバッファーリング数を返します。
    //!
    int GetBufferingCount() const NN_NOEXCEPT
    {
        return m_BufferingCount;
    }

    //! @brief ビューの数を取得します。
    //!
    //! @return ビューの数を返します。
    //!
    int GetViewCount() const NN_NOEXCEPT
    {
        return m_ViewCount;
    }

    //! @brief シェイプがビュー依存するかどうかを取得します。
    //!
    //! @return シェイプがビュー依存する場合は true、依存しない場合は false を返します。
    //!
    bool IsViewDependent() const NN_NOEXCEPT
    {
        return m_ViewDependent;
    }

    //! @brief バウンディング計算が有効かどうかを取得します。
    //!
    //! @return バウンディング計算が有効な場合は true、無効な場合は false を返します。
    //!
    bool IsBoundingEnabled() const NN_NOEXCEPT
    {
        return m_BoundingEnabled;
    }

    //! @brief ユーザーエリアのサイズを取得します。
    //!
    //! @return ユーザーエリアのサイズを返します。
    //!
    size_t GetUserAreaSize() const NN_NOEXCEPT
    {
        return m_UserAreaSize;
    }

    //! @brief リソースを取得します。
    //!
    //! @return ResShape へのポインターを返します。
    //!
    const ResShape* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    }

    //! @brief ShapeObj 構築に必要なメモリサイズを取得します。
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief ShapeObj 構築に必要なメモリサイズを計算します。
    //!
    //! @pre
    //! - 設定されたユニフォームブロックのバッファーリング数が１以上。
    //!
    void CalculateMemorySize() NN_NOEXCEPT;

    //! @brief ShapeObj 構築に必要なメモリサイズが計算済みかを取得します。
    bool IsMemoryCalculated() const NN_NOEXCEPT
    {
        return m_WorkMemory.IsValid();
    }

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

private:
    void CalculateDynamicVertexBufferSize() NN_NOEXCEPT;

private:
    const ResShape*                       m_pRes;
    int                                   m_BufferingCount;
    int                                   m_ViewCount;
    bool                                  m_ViewDependent;
    bool                                  m_BoundingEnabled;
    size_t                                m_UserAreaSize;
    nn::util::MemorySplitter              m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
};

//! @brief ShapeObj の構築を行うクラスです。
class ShapeObj::Builder : public ShapeObj::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @param[in] resource ResShape へのポインター。
    //!
    explicit Builder(const ResShape* resource)
    : InitializeArgument( resource )
    {
    }

    //! @brief ShapeObj を構築します。
    //!
    //! @param[in] pShapeObj ShapeObj へのポインター。
    //! @param[in] pBuffer バッファーへのポインター。
    //! @param[in] bufferSize バッファーのサイズ。
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである。
    //! - pBuffer は Alignment_Buffer のアライメント上にある。
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ。
    //!
    bool Build(ShapeObj* pShapeObj, void* pBuffer, size_t bufferSize) NN_NOEXCEPT
    {
        return pShapeObj->Initialize(*this, pBuffer, bufferSize);
    }
};

}} // namespace nn::g3d

