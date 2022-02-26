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
* @brief モデルに関するリソースクラスの宣言
*/

#pragma once

#include <nn/gfx.h>
#include <nn/util/util_ResDic.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResCommon.h>
#include <nn/g3d/g3d_ResSkeleton.h>
#include <nn/g3d/g3d_ResShape.h>
#include <nn/g3d/g3d_ResMaterial.h>
#include <nn/gfx/gfx_ResUserData.h>
#include <nn/g3d/g3d_Binding.h>

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_SHADOW
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS
#endif

namespace nn { namespace g3d {

class ResFile;

//--------------------------------------------------------------------------------------------------

//! @brief モデルの構造体です。
struct ResModelData
{
    nn::util::BinBlockSignature         signature; //!< シグネチャー。
    uint8_t                             reserved0[4];
    nn::util::BinPtrToString            pName; //!< 名前。
    nn::util::BinPtrToString            pPath; //!< パス名。

    nn::util::BinTPtr<ResSkeleton>      pSkeleton; //!< スケルトンへのポインター。
    nn::util::BinTPtr<ResVertex>        pVertexArray; //!< 頂点情報配列へのポインター。
    nn::util::BinTPtr<ResShape>         pShapeArray; //!< シェイプ配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic> pShapeDic; //!< シェイプ辞書へのポインター。
    nn::util::BinTPtr<ResMaterial>      pMaterialArray; //!< マテリアル配列へのポインター。
    uint8_t  reserved1[8];
    nn::util::BinTPtr<nn::util::ResDic> pMaterialDic; //!< マテリアル辞書へのポインター。
    nn::util::BinTPtr<nn::gfx::ResUserData> pUserDataArray; //!< ユーザーデータ配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic> pUserDataDic; //!< ユーザーデータ辞書へのポインター。
    nn::util::BinPtr pUserPtr; //! ユーザーポインター。

    uint16_t vertexCount; //!< 頂点情報数。
    uint16_t shapeCount; //!< シェイプ数。
    uint16_t materialCount; //!< マテリアル数。
    uint8_t  reserved2[2];
    uint16_t userDataCount; //!< ユーザーデータ数。
    uint8_t  reserved3[6];
};

//! @brief モデルのリソースです。
class ResModel : public nn::util::AccessorBase< ResModelData >
{
    NN_DISALLOW_COPY( ResModel );

public:
    //! @brief ResModel のシグネチャ
    static const uint32_t Signature = NN_UTIL_CREATE_SIGNATURE_4('F', 'M', 'D', 'L');

    //! @brief リセット時にリセットしないものを指定するフラグです。
    enum ResetGuardFlag
    {
        ResetGuardFlag_None       = 0, //!< すべてリセットします。
        ResetGuardFlag_UserPtr    = 0x1 << 0, //!< ユーザーポインターの設定をリセットしません。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @briefprivate モデルをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    void Setup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @briefprivate モデルをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター
    //! @param[in] pMemoryPool ResFile が配置されているメモリプールへのポインター。
    //! @param[in] memoryPoolOffset ResFile のインデックスバッファーおよび頂点バッファーが配置されているメモリプール上の先頭オフセット。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - pMemoryPool に ResFile がロードされている。
    //!
    //! @details
    //! ユーザーが自身で用意したメモリプールに ResFile をロードし、そのメモリプールを利用して ResFile をセットアップする場合にこの関数を使用します。
    //! この関数では内部でメモリプールの作成を行いません。
    //! メモリプールに ResFile を配置する場合はファイルのアライメントを使用してください。
    //! ResFile には CPU アクセスを行うので、メモリプールの特性は CpuCached が推奨されます。
    //! CpuUncached を設定した場合はパフォーマンスが低下します。CpuInvisible は使用できません。
    //!
    void Setup(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset) NN_NOEXCEPT;

    //! @briefprivate モデルをクリーンアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    void Cleanup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @brief モデルをリセットします。
    void Reset() NN_NOEXCEPT;

    //! @brief モデルをリセットします。
    void Reset(Bit32 resetGuardFlag) NN_NOEXCEPT;

    //! @brief テクスチャーの参照解決を行います。
    //!
    //! @param[in] pCallback テクスチャーの参照解決時に呼び出されるコールバックへのポインター
    //! @param[in] pUserData コールバック呼び出し時に引数で渡されるユーザーデータへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @details
    //! 引数にテクスチャー名を渡し、pCallback を呼び出します。pCallback は適切な nn::g3d::TextureRef
    //! を返してください。有効でない nn::g3d::TextureRef がコールバックの返り値として返された場合、
    //! そのテクスチャーの参照解決は失敗したと扱われます。
    //! テクスチャーの参照解決が行われないまま描画処理を行うと不適切なアクセスが発生する可能性があります。
    //! 返り値を確認し、テクスチャーの参照解決が成功したことを確認してください。
    //!
    //! 参照が解決されていないテクスチャーに対してのみ、コールバックは呼び出されます。
    //! 参照が解決済みかどうかは nn::g3d::ResMaterial::IsTextureBound() よって判定されます。
    //!
    BindResult BindTexture(TextureBindCallback pCallback, void* pUserData) NN_NOEXCEPT;

    //! @brief 指定した名前のテクスチャーの参照を差し替えます。
    //!
    //! @param[in] textureRef 差し替えるテクスチャーのビューおよびディスクリプタスロットを格納した nn::g3d::TextureRef への参照。
    //! @param[in] name 差し替えるテクスチャーの名前
    //!
    //! @return 差し替えが成功した場合は true、失敗した場合は false を返します。
    //!
    bool ForceBindTexture(const TextureRef& textureRef, const char* name) NN_NOEXCEPT;

    //! @brief テクスチャーの参照設定を解除します。
    void ReleaseTexture() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief モデル名を取得します。
    //!
    //! @return モデル名へのポインターを返します。
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
    //! @tparam T ポインターを受けとる際の要素の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    T* GetUserPtr() NN_NOEXCEPT
    {
        return static_cast<T*>(GetUserPtr());
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の要素の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    const T* GetUserPtr() const NN_NOEXCEPT
    {
        return static_cast<const T*>(GetUserPtr());
    }

    //@}

    //----------------------------------------
    //! @name スケルトン
    //@{

    //! @brief スケルトンを取得します。
    //!
    //! @return ResSkeleton へのポインターを返します。
    //!
    ResSkeleton* GetSkeleton() NN_NOEXCEPT
    {
        return ToData().pSkeleton.Get();
    }
    //! @brief スケルトンを取得します。
    //!
    //! @return ResSkeleton へのポインターを返します。
    //!
    const ResSkeleton* GetSkeleton() const NN_NOEXCEPT
    {
        return ToData().pSkeleton.Get();
    }

    //@}

    //----------------------------------------
    //! @name 頂点情報
    //@{

    //! @brief 頂点情報の数を取得します。
    //!
    //! @return 頂点情報の数を返します。
    //!
    int GetVertexCount() const NN_NOEXCEPT
    {
        return ToData().vertexCount;
    }
    //! @brief インデックス引きで頂点情報を取得します。
    //!
    //! @param[in] elemIndex 頂点情報のインデックス。
    //!
    //! @return ResVertex へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは頂点情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResVertex* GetVertex(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetVertexCount(), GetName());
        ResVertex* pArray = ToData().pVertexArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きで頂点情報を取得します。
    //!
    //! @param[in] elemIndex 頂点情報のインデックス。
    //!
    //! @return ResVertex へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは頂点情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResVertex* GetVertex(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetVertexCount(), GetName());
        const ResVertex* pArray = ToData().pVertexArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief シェイプアニメーションを行うために必要な頂点属性を動的頂点バッファーに含めます。
    //!
    //! @details
    //! - モデルリソースが保持する全シェイプに対して書き込みを行います。
    //! - シェイプリソースがキーシェイプを保持していない場合には何も行われません。
    //!
    void ActivateDynamicVertexAttrForShapeAnim() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name シェイプ
    //@{

    //! @brief シェイプの数を取得します。
    //!
    //! @return シェイプの数を返します。
    //!
    int GetShapeCount() const NN_NOEXCEPT
    {
        return ToData().shapeCount;
    }
    //! @brief 指定した名前のシェイプを取得します。
    //!
    //! @param[in] str シェイプ名。
    //!
    //! @return ResShape へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェイプが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResShape* FindShape(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShapeDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResShape* pArray = ToData().pShapeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のシェイプを取得します。
    //!
    //! @param[in] str シェイプ名。
    //!
    //! @return ResShape へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェイプが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResShape* FindShape(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShapeDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResShape* pArray = ToData().pShapeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のシェイプのインデックスを取得します。
    //!
    //! @param[in] str シェイプ名。
    //!
    //! @return シェイプのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のシェイプが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindShapeIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShapeDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief シェイプのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex シェイプのインデックス。
    //!
    //! @return シェイプ名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェイプの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetShapeName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShapeCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pShapeDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでシェイプを取得します。
    //!
    //! @param[in] elemIndex シェイプのインデックス。
    //!
    //! @return ResShape へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェイプの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResShape* GetShape(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShapeCount(), GetName());
        ResShape* pArray = ToData().pShapeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでシェイプを取得します。
    //!
    //! @param[in] elemIndex シェイプのインデックス。
    //!
    //! @return ResShape へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェイプの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResShape* GetShape(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShapeCount(), GetName());
        const ResShape* pArray = ToData().pShapeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name マテリアル
    //@{
    //! @brief マテリアルの数を取得します。
    //!
    //! @return マテリアルの数を返します。
    //!
    int GetMaterialCount() const NN_NOEXCEPT
    {
        return ToData().materialCount;
    }
    //! @brief 指定した名前のマテリアルを取得します。
    //!
    //! @param[in] str マテリアル名。
    //!
    //! @return ResMaterial へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のマテリアルが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResMaterial* FindMaterial(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pMaterialDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResMaterial* pArray = ToData().pMaterialArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のマテリアルを取得します。
    //!
    //! @param[in] str マテリアル名。
    //!
    //! @return ResMaterial へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のマテリアルが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResMaterial* FindMaterial(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pMaterialDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResMaterial* pArray = ToData().pMaterialArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のマテリアルのインデックスを取得します。
    //!
    //! @param[in] str マテリアル名。
    //!
    //! @return マテリアルのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のマテリアルが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindMaterialIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pMaterialDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief マテリアルのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex マテリアルのインデックス。
    //!
    //! @return マテリアル名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetMaterialName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetMaterialCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pMaterialDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでマテリアルを取得します。
    //!
    //! @param[in] elemIndex マテリアルのインデックス。
    //!
    //! @return ResMaterial へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResMaterial* GetMaterial(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetMaterialCount(), GetName());
        ResMaterial* pArray = ToData().pMaterialArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでマテリアルを取得します。
    //!
    //! @param[in] elemIndex マテリアルのインデックス。
    //!
    //! @return ResMaterial へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResMaterial* GetMaterial(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetMaterialCount(), GetName());
        const ResMaterial* pArray = ToData().pMaterialArray.Get();
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
    //! @param[in] str ユーザーデータ名
    //!
    //! @return ResModel へのポインターを返します。
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
    //! @return ResModel へのポインターを返します。
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
    //! @return ResModel へのポインターを返します。
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
    //! @return ResModel へのポインターを返します。
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
