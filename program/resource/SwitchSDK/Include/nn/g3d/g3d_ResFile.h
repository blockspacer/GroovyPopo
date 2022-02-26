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
* @brief リソースファイルに関するクラスの宣言
*/

#pragma once

#include <nn/gfx.h>
#include <nn/util/util_ResDic.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResCommon.h>
#include <nn/g3d/g3d_ResModel.h>
#include <nn/g3d/g3d_ResSkeleton.h>
#include <nn/g3d/g3d_ResShape.h>
#include <nn/g3d/g3d_ResMaterial.h>
#include <nn/g3d/g3d_ResSkeletalAnim.h>
#include <nn/g3d/g3d_ResShapeAnim.h>
#include <nn/g3d/g3d_ResSceneAnim.h>
#include <nn/g3d/g3d_ResMaterialAnim.h>
#include <nn/g3d/g3d_ResBoneVisibilityAnim.h>

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_SHADOW
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS
#endif

namespace nn { namespace g3d {

//--------------------------------------------------------------------------------------------------

//! @brief ファイルデータの構造体です。
struct ResFileData
{
    nn::util::BinaryFileHeader                       fileHeader; //!< ファイルヘッダー。
    nn::util::BinPtrToString                         pName; //!< ファイル名へのポインター。バイナリー変換時に指定されたファイル名（拡張子を除く）。
    nn::util::BinTPtr<ResModel>                      pModelArray; //!< モデル配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>              pModelDic; //!< モデル配列辞書へのポインター。
    uint8_t reserved0[32];
    nn::util::BinTPtr<ResSkeletalAnim>               pSkeletalAnimArray; //!< スケルタルアニメーション配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>              pSkeletalAnimDic; //!< スケルタルアニメーション辞書へのポインター。
    nn::util::BinTPtr<ResMaterialAnim>               pMaterialAnimArray; //!< マテリアルアニメーション配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>              pMaterialAnimDic; //!< シェーダーパラメータアニメーション辞書へのポインター。
    nn::util::BinTPtr<ResBoneVisibilityAnim>         pBoneVisAnimArray; //!< ボーンビジビリティアニメーション配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>              pBoneVisAnimDic; //!< ボーンビジビリティアニメーション辞書へのポインター。
    nn::util::BinTPtr<ResShapeAnim>                  pShapeAnimArray; //!< シェイプアニメーション配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>              pShapeAnimDic; //!< シェイプアニメーション辞書へのポインター。
    nn::util::BinTPtr<ResSceneAnim>                  pSceneAnimArray; //!< シーンアニメーション配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>              pSceneAnimDic; //!< シーンアニメーション辞書へのポインター。
    nn::util::BinTPtr<nn::gfx::MemoryPool>           pBufferMemoryPool; //!< 頂点バッファー・インデックスバッファー用メモリプールへのポインター。
    nn::util::BinTPtr<nn::gfx::MemoryPool::InfoType> pBufferMemoryPoolInfo; //!< 頂点バッファー・インデックスバッファー用メモリプール情報へのポインター。
    nn::util::BinTPtr<ResExternalFile>               pExternalFileArray; //!< 外部ファイル配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>              pExternalFileDic; //!< 外部ファイル辞書へのポインター。
    nn::util::BinPtr                                 pUserPtr; //!< ユーザポインター。
    nn::util::BinTPtr<nn::util::BinString>           pStringPool; //!< 文字列プールへのポインター。

    uint32_t sizeStringPool; //!< 文字列プールのバイトサイズ。

    // 辞書が無い場合の判定を無くすためにここで数を持つ。
    // 辞書が無い場合でも名前引きは安全だが、インデックスアクセスはアサートされている。
    uint16_t modelCount; //!< モデル数。
    uint8_t  reserved1[4];
    uint16_t skeletalAnimCount; //!< スケルタルアニメーション数。
    uint16_t materialAnimCount; //!< マテリアルアニメーション数。
    uint16_t boneVisAnimCount; //!< ボーンビジビリティアニメーション数。
    uint16_t shapeAnimCount; //!< シェイプアニメーション数。
    uint16_t sceneAnimCount; //!< シーンアニメーション数。
    uint16_t externalFileCount; //!< 外部ファイル数。
    uint8_t  reserved2[2];
};

//! @brief リソースファイルです。
class ResFile : public nn::util::AccessorBase< ResFileData >
{
    NN_DISALLOW_COPY( ResFile );

public:
    //! @briefprivate
    typedef ResFileData DataType;

    //! @brief ResFile のシグネチャ
    static const int64_t Signature;

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief ロードしたファイルを ResFile に変換します。
    //!
    //! @param[in] ptr ロードしたファイルへのポインター。
    //!
    //! @details
    //! @platformbegin{Cafe}
    //! Cafe版では内部でエンディアン変換が行われます。
    //! @platformend
    //!
    static ResFile* ResCast(void* ptr) NN_NOEXCEPT;

    //! @brief ResFile を使用可能な状態にします。
    //!
    //! @details
    //! Unrelocate() 後、再度使用可能な状態にするときに使用することを想定しています。
    //! ResCast() の中でも同様の処理が行われています。
    //!
    void Relocate() NN_NOEXCEPT;

    //! @brief ResFile を再配置可能な状態にします。
    //!
    //! @details
    //! Unrelocate 後はリソースファイルを別のメモリアドレスに移動することが出来ます。
    //! 移動前に ResFile を Cleanup() しておく必要があります。
    //! 再度使用可能な状態にするためには Relocate() または ResCast() を呼び出す必要があります。
    //!
    void Unrelocate() NN_NOEXCEPT;

    //! @brief リソースをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    //! @details
    //! 配下のモデル、テクスチャー、アニメーションの Setup を呼び出します。
    //! インスタンス構築前に呼び出してください。
    //!
    void Setup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @brief リソースをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @param[in] pMemoryPool ResFile が配置されているメモリプールへのポインター。
    //! @param[in] memoryPoolOffset ResFile が配置されているメモリプール上のオフセット。
    //! @param[in] memoryPoolSize ResFile が配置されているメモリプールのサイズ。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - pMemoryPool に ResFile がロードされている。
    //!
    //! @details
    //! 配下のモデル、テクスチャー、アニメーションの Setup を呼び出します。
    //! インスタンス構築前に呼び出してください。
    //!
    //! ユーザが自身で用意したメモリプールに ResFile をロードし、そのメモリプールを利用して ResFile をセットアップする場合にこの関数を使用します。
    //! この関数では内部でメモリプールの作成を行いません。
    //! メモリプールに ResFile を配置する場合はファイルのアライメントを使用してください。
    //! ファイルのアライメントは GetFileHeader() でヘッダーを取得し、 nn::util::BinaryFileHeader::GetAlignment() で取得することができます。
    //! ResFile には CPU アクセスを行うので、メモリプールの特性は CpuCached が推奨されます。
    //! CpuUncached を設定した場合はパフォーマンスが低下します。CpuInvisible は使用できません。
    //!
    void Setup(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize) NN_NOEXCEPT;

    //! @brief リソースをクリーンアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    //! @details
    //! 配下のモデル、テクスチャー、アニメーションの Cleanup を呼び出します。
    //! インスタンス破棄後に呼び出してください。
    //!
    void Cleanup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @brief 初期状態から変化した値を初期化します。
    //!
    //! @details
    //! アプリケーションから明示的に変更された値は初期化しません。
    //! 配下のモデル、テクスチャー、アニメーションの Reset を呼び出します。
    //! インスタンス破棄後に呼び出してください。
    //!
    void Reset() NN_NOEXCEPT;

    //! @brief テクスチャーの参照解決を行います。
    //!
    //! @param[in] pCallback テクスチャーの参照解決時に呼び出されるコールバックへのポインター。
    //! @param[in] pUserData コールバック呼び出し時に引数で渡されるユーザデータへのポインター。
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
    //! 参照が解決済みかどうかは nn::g3d::ResMaterial::IsTextureBound() または nn::g3d::ResMaterialAnim::IsTextureBound() に
    //! よって判定されます。
    //!
    BindResult BindTexture(TextureBindCallback pCallback, void* pUserData);

    //! @brief テクスチャーの参照を解除します。
    void ReleaseTexture() NN_NOEXCEPT;

    //@}

    //! @brief バイナリーファイルが有効かどうかを取得します。
    //!
    //! @return 有効な場合は true、無効な場合は false を返します。
    //!
    static bool IsValid(const void* ptr) NN_NOEXCEPT;

    //! @brief リソースのヘッダを取得します。
    //!
    //! @return nn::util::BinaryFileHeader へのポインターを返します。
    //!
    const nn::util::BinaryFileHeader* GetFileHeader() const NN_NOEXCEPT
    {
        return &ToData().fileHeader;
    }

    //! @brief ファイル名を取得します。
    //!
    //! @return ファイル名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }

    //! @brief ユーザポインターを設定します。
    //!
    //! @param[in] pUserPtr ポインター。
    //!
    void SetUserPtr(void* pUserPtr) NN_NOEXCEPT
    {
        ToData().pUserPtr.Set(pUserPtr);
    }

    //! @brief ユーザポインターを取得します。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    void* GetUserPtr() NN_NOEXCEPT
    {
        return ToData().pUserPtr.Get();
    }

    //! @brief ユーザポインターを取得します。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    const void* GetUserPtr() const NN_NOEXCEPT
    {
        return ToData().pUserPtr.Get();
    }

    //! @brief ユーザポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    T* GetUserPtr() NN_NOEXCEPT
    {
        return static_cast<T*>(GetUserPtr());
    }

    //! @brief ユーザポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    const T* GetUserPtr() const NN_NOEXCEPT
    {
        return static_cast<const T*>(GetUserPtr());
    }

    //! @brief メモリプール情報へのポインターを取得します。
    //!
    //! @return メモリプール情報へのポインターを返します。
    //!
    const nn::gfx::MemoryPoolInfo* GetMemoryPoolInfo() const NN_NOEXCEPT
    {
        return ToData().pBufferMemoryPoolInfo.Get();
    }

    //! @brief メモリプールへのポインターを取得します。
    //!
    //! @return メモリプールへのポインターを返します。
    //!
    const nn::gfx::MemoryPool* GetMemoryPool() const NN_NOEXCEPT
    {
        return ToData().pBufferMemoryPool.Get();
    }

    //@}

    //----------------------------------------
    //! @name モデル
    //@{
    //! @brief モデルの数を取得します。
    //!
    //! @return モデルの数を返します。
    //!
    int GetModelCount() const NN_NOEXCEPT
    {
        return ToData().modelCount;
    }
    //! @brief 指定した名前のモデルを取得します。
    //!
    //! @param[in] str モデル名。
    //!
    //! @return ResModel へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のモデルが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResModel* FindModel(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pModelDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResModel* pArray = ToData().pModelArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のモデルを取得します。
    //!
    //! @param[in] str モデル名。
    //!
    //! @return ResModel へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のモデルが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResModel* FindModel(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pModelDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResModel* pArray = ToData().pModelArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のモデルのインデックスを取得します。
    //!
    //! @param[in] str モデル名。
    //!
    //! @return モデルのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のモデルが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindModelIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pModelDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief モデルのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex モデルのインデックス。
    //!
    //! @return モデル名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはモデルの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetModelName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetModelCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pModelDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでモデルを取得します。
    //!
    //! @param[in] elemIndex モデルのインデックス。
    //!
    //! @return ResModelへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはモデルの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResModel* GetModel(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetModelCount(), GetName());
        ResModel* pArray = ToData().pModelArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでモデルを取得します。
    //!
    //! @param[in] elemIndex モデルのインデックス。
    //!
    //! @return ResModelへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはモデルの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResModel* GetModel(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetModelCount(), GetName());
        const ResModel* pArray = ToData().pModelArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name スケルタルアニメーション
    //@{
    //! @brief スケルタルアニメーション数を取得します。
    //!
    //! @return スケルタルアニメーションの数を返します。
    //!
    int GetSkeletalAnimCount() const NN_NOEXCEPT
    {
        return ToData().skeletalAnimCount;
    }
    //! @brief 指定した名前のスケルタルアニメーションを取得します。
    //!
    //! @param[in] str モデル名。
    //!
    //! @return ResSkeletalAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のスケルタルアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResSkeletalAnim* FindSkeletalAnim(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSkeletalAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResSkeletalAnim* pArray = ToData().pSkeletalAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のスケルタルアニメーションを取得します。
    //!
    //! @param[in] str モデル名。
    //!
    //! @return ResSkeletalAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のスケルタルアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResSkeletalAnim* FindSkeletalAnim(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSkeletalAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResSkeletalAnim* pArray = ToData().pSkeletalAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のスケルタルアニメーションのインデックスを取得します。
    //!
    //! @param[in] str スケルタルアニメーション名。
    //!
    //! @return スケルタルアニメーションのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のスケルタルアニメーションが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindSkeletalAnimIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSkeletalAnimDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief スケルタルアニメーションのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex スケルタルアニメーションのインデックス。
    //!
    //! @return スケルタルアニメーション名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはスケルタルアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetSkeletalAnimName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSkeletalAnimCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pSkeletalAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでスケルタルアニメーションを取得します。
    //!
    //! @param[in] elemIndex スケルタルアニメーションのインデックス。
    //!
    //! @return ResSkeletalAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはスケルタルアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResSkeletalAnim* GetSkeletalAnim(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSkeletalAnimCount(), GetName());
        ResSkeletalAnim* pArray = ToData().pSkeletalAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでスケルタルアニメーションを取得します。
    //!
    //! @param[in] elemIndex スケルタルアニメーションのインデックス。
    //!
    //! @return ResSkeletalAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはスケルタルアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResSkeletalAnim* GetSkeletalAnim(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSkeletalAnimCount(), GetName());
        const ResSkeletalAnim* pArray = ToData().pSkeletalAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name マテリアルアニメーション
    //@{
    //! @brief マテリアルアニメーションの数を取得します。
    //!
    //! @return マテリアルアニメーションの数を返します。
    //!
    int GetMaterialAnimCount() const NN_NOEXCEPT
    {
        return ToData().materialAnimCount;
    }
    //! @brief 指定した名前のマテリアルアニメーションを取得します。
    //!
    //! @param[in] str マテリアルアニメーション名。
    //!
    //! @return ResMaterialAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のモデルが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResMaterialAnim* FindMaterialAnim(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pMaterialAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResMaterialAnim* pArray = ToData().pMaterialAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のマテリアルアニメーションを取得します。
    //!
    //! @param[in] str マテリアルアニメーション名。
    //!
    //! @return ResMaterialAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のモデルが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResMaterialAnim* FindMaterialAnim(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pMaterialAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResMaterialAnim* pArray = ToData().pMaterialAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のマテリアルアニメーションのインデックスを取得します。
    //!
    //! @param[in] str マテリアルアニメーション名。
    //!
    //! @return マテリアルアニメーションのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のマテリアルアニメーションが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindMaterialAnimIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pMaterialAnimDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief マテリアルアニメーションのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex マテリアルアニメーションのインデックス。
    //!
    //! @return マテリアルアニメーション名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetMaterialAnimName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetMaterialAnimCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pMaterialAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでマテリアルアニメーションを取得します。
    //!
    //! @param[in] elemIndex マテリアルアニメーションのインデックス。
    //!
    //! @return ResMaterialAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルアニメーションの数の範囲内
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResMaterialAnim* GetMaterialAnim(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetMaterialAnimCount(), GetName());
        ResMaterialAnim* pArray = ToData().pMaterialAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでマテリアルアニメーションを取得します。
    //!
    //! @param[in] elemIndex マテリアルアニメーションのインデックス。
    //!
    //! @return ResMaterialAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResMaterialAnim* GetMaterialAnim(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetMaterialAnimCount(), GetName());
        const ResMaterialAnim* pArray = ToData().pMaterialAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name シェイプアニメーション
    //@{
    //! @brief シェイプアニメーションの数を取得します。
    //!
    //! @return シェイプアニメーションの数を返します。
    //!
    int GetShapeAnimCount() const NN_NOEXCEPT
    {
        return ToData().shapeAnimCount;
    }
    //! @brief 指定した名前のシェイプアニメーションを取得します。
    //!
    //! @param[in] str シェイプアニメーション名。
    //!
    //! @return ResShapeAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェイプアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResShapeAnim* FindShapeAnim(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShapeAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResShapeAnim* pArray = ToData().pShapeAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のシェイプアニメーションを取得します。
    //!
    //! @param[in] str シェイプアニメーション名。
    //!
    //! @return ResShapeAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェイプアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResShapeAnim* FindShapeAnim(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShapeAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResShapeAnim* pArray = ToData().pShapeAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のシェイプアニメーションのインデックスを取得します。
    //!
    //! @param[in] str シェイプアニメーション名。
    //!
    //! @return シェイプアニメーションのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のシェイプアニメーションが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindShapeAnimIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShapeAnimDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief シェイプアニメーションのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex シェイプアニメーションのインデックス。
    //!
    //! @return シェイプアニメーション名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマシェイプアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetShapeAnimName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShapeAnimCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pShapeAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでシェイプアニメーションを取得します。
    //!
    //! @param[in] elemIndex シェイプアニメーションのインデックス。
    //!
    //! @return ResShapeAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマシェイプアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResShapeAnim* GetShapeAnim(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShapeAnimCount(), GetName());
        ResShapeAnim* pArray = ToData().pShapeAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでシェイプアニメーションを取得します。
    //!
    //! @param[in] elemIndex シェイプアニメーションのインデックス。
    //!
    //! @return ResShapeAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマシェイプアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResShapeAnim* GetShapeAnim(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShapeAnimCount(), GetName());
        const ResShapeAnim* pArray = ToData().pShapeAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name ボーンビジビリティアニメーション
    //@{

    //! @brief ボーンビジビリティアニメーションの数を取得します。
    //!
    //! @return ボーンビジビリティアニメーションの数を返します。
    //!
    int GetBoneVisibilityAnimCount() const NN_NOEXCEPT
    {
        return ToData().boneVisAnimCount;
    }
    //! @brief 指定した名前のボーンビジビリティアニメーションを取得します。
    //!
    //! @param[in] str ボーンビジビリティアニメーション名。
    //!
    //! @return ResBoneVisibilityAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のボーンビジビリティアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResBoneVisibilityAnim* FindBoneVisibilityAnim(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pBoneVisAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResBoneVisibilityAnim* pArray = ToData().pBoneVisAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のボーンビジビリティアニメーションを取得します。
    //!
    //! @param[in] str ボーンビジビリティアニメーション名。
    //!
    //! @return ResBoneVisibilityAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のボーンビジビリティアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResBoneVisibilityAnim* FindBoneVisibilityAnim(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pBoneVisAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResBoneVisibilityAnim* pArray = ToData().pBoneVisAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のボーンビジビリティアニメーションのインデックスを取得します。
    //!
    //! @param[in] str ボーンビジビリティアニメーション名。
    //!
    //! @return ボーンビジビリティアニメーションのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のボーンビジビリティアニメーションが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindBoneVisibilityAnimIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pBoneVisAnimDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief ボーンビジビリティアニメーションのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex ボーンビジビリティアニメーションのインデックス。
    //!
    //! @return ボーンビジビリティアニメーション名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはボーンビジビリティアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetBoneVisibilityAnimName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetBoneVisibilityAnimCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pBoneVisAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでボーンビジビリティアニメーションを取得します。
    //!
    //! @param[in] elemIndex ボーンビジビリティアニメーションのインデックス。
    //!
    //! @return ResBoneVisibilityAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはボーンビジビリティアニメーションの数の範囲内
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResBoneVisibilityAnim* GetBoneVisibilityAnim(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetBoneVisibilityAnimCount(), GetName());
        ResBoneVisibilityAnim* pArray = ToData().pBoneVisAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでボーンビジビリティアニメーションを取得します。
    //!
    //! @param[in] elemIndex ボーンビジビリティアニメーションのインデックス。
    //!
    //! @return ResBoneVisibilityAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはボーンビジビリティアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResBoneVisibilityAnim* GetBoneVisibilityAnim(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetBoneVisibilityAnimCount(), GetName());
        const ResBoneVisibilityAnim* pArray = ToData().pBoneVisAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name シーンアニメーション
    //@{
    //! @brief シーンアニメーションの数を取得します。
    //!
    //! @return シーンアニメーションの数を返します。
    //!
    int GetSceneAnimCount() const NN_NOEXCEPT
    {
        return ToData().sceneAnimCount;
    }
    //! @brief 指定した名前のシーンアニメーションを取得します。
    //!
    //! @param[in] str シーンアニメーション名。
    //!
    //! @return ResSceneAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシーンアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResSceneAnim* FindSceneAnim(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSceneAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResSceneAnim* pArray = ToData().pSceneAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のシーンアニメーションを取得します。
    //!
    //! @param[in] str シーンアニメーション名。
    //!
    //! @return ResSceneAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシーンアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResSceneAnim* FindSceneAnim(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSceneAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResSceneAnim* pArray = ToData().pSceneAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のシーンアニメーションのインデックスを取得します。
    //!
    //! @param[in] str シーンアニメーション名。
    //!
    //! @return シーンアニメーションのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のシーンアニメーションが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindSceneAnimIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSceneAnimDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief シーンアニメーションのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex シーンアニメーションのインデックス。
    //!
    //! @return シーンアニメーション名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシーンアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetSceneAnimName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSceneAnimCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pSceneAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでシーンアニメーションを取得します。
    //!
    //! @param[in] elemIndex シーンアニメーションのインデックス。
    //!
    //! @return ResSceneAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシーンアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResSceneAnim* GetSceneAnim(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSceneAnimCount(), GetName());
        ResSceneAnim* pArray = ToData().pSceneAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでシーンアニメーションを取得します。
    //!
    //! @param[in] elemIndex シーンアニメーションのインデックス。
    //!
    //! @return ResSceneAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシーンアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResSceneAnim* GetSceneAnim(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSceneAnimCount(), GetName());
        const ResSceneAnim* pArray = ToData().pSceneAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name 外部ファイル
    //@{

    //! @brief 外部ファイルの数を取得します。
    //!
    //! @return 外部ファイルの数を返します。
    //!
    int GetExternalFileCount() const NN_NOEXCEPT
    {
        return ToData().externalFileCount;
    }
    //! @brief 指定した名前の外部ファイルを取得します。
    //!
    //! @param[in] str 外部ファイル名。
    //!
    //! @return ResExternalFile へのポインターを返します。
    //!
    //! @details
    //! 指定した名前の外部ファイルが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResExternalFile* FindExternalFile(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pExternalFileDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResExternalFile* pArray = ToData().pExternalFileArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前の外部ファイルを取得します。
    //!
    //! @param[in] str 外部ファイル名。
    //!
    //! @return ResExternalFile へのポインターを返します。
    //!
    //! @details
    //! 指定した名前の外部ファイルが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResExternalFile* FindExternalFile(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pExternalFileDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResExternalFile* pArray = ToData().pExternalFileArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前の外部ファイルのインデックスを取得します。
    //!
    //! @param[in] str 外部ファイル名。
    //!
    //! @return 外部ファイルのインデックスを返します。
    //!
    //! @details
    //! 指定した名前の外部ファイルが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindExternalFileIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pExternalFileDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief 外部ファイルのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex 外部ファイルのインデックス。
    //!
    //! @return 外部ファイル名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは外部ファイルの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetExternalFileName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetExternalFileCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pExternalFileDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きで外部ファイルを取得します。
    //!
    //! @param[in] elemIndex 外部ファイルのインデックス。
    //!
    //! @return ResExternalFile へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは外部ファイルの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResExternalFile* GetExternalFile(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetExternalFileCount(), GetName());
        ResExternalFile* pArray = ToData().pExternalFileArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きで外部ファイルを取得します。
    //!
    //! @param[in] elemIndex 外部ファイルのインデックス。
    //!
    //! @return ResExternalFile へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは外部ファイルの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResExternalFile* GetExternalFile(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetExternalFileCount(), GetName());
        const ResExternalFile* pArray = ToData().pExternalFileArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}
};

}} // namespace nn::g3d

NN_PRAGMA_POP_WARNINGS
