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
* @brief シェイプに関するリソースクラスの宣言
*/

#pragma once

#include <nn/gfx.h>
#include <nn/util/util_ResDic.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResCommon.h>
#include <nn/g3d/detail/g3d_MatrixUtility.h>
#include <nn/util/util_BitFlagSet.h>

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_SHADOW
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS
#endif

namespace nn { namespace g3d {

class ResFile;

//--------------------------------------------------------------------------------------------------

//! @brief 頂点属性の構造体です。
struct ResVertexAttrData
{
    nn::util::BinPtrToString    pName;          //!< 頂点属性名。
    int32_t                     format;         //!< 頂点属性のフォーマット。
    uint16_t                    offset;         //!< 頂点属性のオフセット。
    uint8_t                     bufferIndex;    //!< シェイプ内での頂点バッファーのインデックス。
    nn::Bit8                    flag;           //!< フラグ
};

//! @brief 頂点属性のリソースです。
class ResVertexAttr : public nn::util::AccessorBase< ResVertexAttrData >
{
    NN_DISALLOW_COPY( ResVertexAttr );

public:

    //! @brief 頂点属性に関するフラグです。
    enum Flag
    {
        Flag_ContainDynamicVertexBuffer = 0x1 << 0 //! @brief 動的頂点バッファーの要素として含みます。
    };

    //----------------------------------------
    //! @name 頂点属性 取得
    //@{

    //! @brief 頂点属性名を取得します。
    //!
    //! @return 頂点属性名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }

    //! @brief 頂点バッファーインデックスを取得します。
    //!
    //! @return 頂点バッファーインデックスを返します。
    //!
    int GetBufferIndex() const NN_NOEXCEPT
    {
        return ToData().bufferIndex;
    }

    //! @brief 頂点属性のフォーマットを取得します。
    //!
    //! @return 頂点属性のフォーマットを返します。
    //!
    nn::gfx::AttributeFormat GetFormat() const NN_NOEXCEPT
    {
        return static_cast<nn::gfx::AttributeFormat>(ToData().format);
    }

    //! @brief バッファー内のオフセット。
    //!
    //! @return バッファー内のオフセットを返します。
    //!
    ptrdiff_t GetOffset() const NN_NOEXCEPT
    {
        return ToData().offset;
    }

    //@}

    //----------------------------------------
    //! @name 動的頂点バッファー 設定
    //@{

    //! @brief 動的頂点バッファーの生成時にこの頂点属性を含む設定を有効にします。
    void SetDynamicVertexAttrEnabled() NN_NOEXCEPT
    {
        ToData().flag |= Flag_ContainDynamicVertexBuffer;
    }

    //! @brief 動的頂点バッファーの生成時にこの頂点属性を含む設定を無効にします。
    //!
    //! @details
    //! 本関数を呼び出した場合においても、同一頂点バッファーに存在する他の頂点属性が動的頂点バッファーに含まれるように
    //! 設定されていた場合には、こちらの頂点属性も動的頂点バッファーに含まれます。
    //!
    void SetDynamicVertexAttrDisabled() NN_NOEXCEPT
    {
        ToData().flag &= ~Flag_ContainDynamicVertexBuffer;
    }

    //! @brief      動的頂点バッファー生成時にこの頂点属性を含む設定が有効になっているかを取得します。
    //!
    //! @return     動的頂点バッファー生成時にこの頂点属性を含む設定が有効になっている場合は true、無効になっている場合は false を返します。
    //!
    //! @details
    //! 同一頂点バッファーに存在する他の頂点属性が動的頂点バッファーに含まれるよう設定されていた場合には、
    //! こちらの頂点属性も動的頂点バッファーに含まれます。
    //!
    bool IsDynamicVertexAttrEnabled() const NN_NOEXCEPT
    {
        return (ToData().flag & Flag_ContainDynamicVertexBuffer) != 0;
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief 頂点情報の構造体です。
struct ResVertexData
{
    nn::util::BinBlockSignature                       signature; //!< シグネチャー。
    uint8_t                                           reserved0[4];
    nn::util::BinTPtr<ResVertexAttr>                  pVertexAttrArray; //!< 頂点属性配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>               pVertexAttrDic; //!< 頂点属性辞書へのポインター。
    nn::util::BinTPtr<nn::gfx::MemoryPool>            pMemoryPool; //!< 頂点バッファー用メモリプールへのポインター。
    nn::util::BinTPtr<nn::gfx::Buffer>                pVertexBufferArray; //!< 頂点バッファー配列へのポインター。
    nn::util::BinTPtr<nn::gfx::Buffer*>               pVertexBufferPtrArray; //!< 頂点バッファーポインター配列へのポインター。
    nn::util::BinTPtr<nn::gfx::BufferInfo>            pVertexBufferInfoArray; //!< 頂点バッファー情報へのポインター。
    nn::util::BinTPtr<nn::gfx::VertexBufferStateInfo> pVertexBufferStateInfoArray; //!< 頂点バッファーステート情報へのポインター。

    nn::util::BinPtr pUserPtr; //!< ユーザーポインター。

    uint32_t memoryPoolOffset; //!< メモリプールのオフセット。
    uint8_t  vertexAttrCount; //!< 頂点属性数。
    uint8_t  vertexBufferCount; //!< 頂点バッファー数。
    uint16_t index; //!< モデル内でのインデックス。
    uint32_t count; //!< 頂点数。
    uint8_t  influenceCount; //!< スキニングで1頂点に影響するボーンの数。
    uint8_t  reserved[3];
};

//! @brief 頂点情報のリソースです。
class ResVertex : public nn::util::AccessorBase< ResVertexData >
{
    NN_DISALLOW_COPY( ResVertex );

public:
    //! @brief ResVertex のシグネチャ
    static const uint32_t Signature = NN_UTIL_CREATE_SIGNATURE_4('F', 'V', 'T', 'X');

    //! @brief リセット時にリセットしないものを指定するフラグです。
    enum ResetGuardFlag
    {
        ResetGuardFlag_None                 = 0, //!< すべてリセットします。
        ResetGuardFlag_UserPtr              = 0x1 << 0, //!< ユーザーポインターの設定をリセットしません。
        ResetGuardFlag_DynamicVertexBuffer  = 0x1 << 1, //!< 動的頂点バッファーの生成情報の設定をリセットしません。
    };

    //! @brief 頂点リソースが保持する頂点バッファーの最大数です。
    static const int VertexBufferCountMax = UINT8_MAX;  // 1 ~ 255

    //! @brief 動的頂点バッファーのインデックス情報を管理します。
    typedef nn::util::BitFlagSet<VertexBufferCountMax> DynamicVertexBufferMask;

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @briefprivate 頂点情報をセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    void Setup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @briefprivate 頂点情報をセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
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

    //! @briefprivate 頂点情報をクリーンアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    void Cleanup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @brief 頂点情報をリセットします。
    void Reset() NN_NOEXCEPT;

    //! @brief 頂点情報をリセットします。
    void Reset(Bit32 resetGuardFlag) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief モデル内でのインデックスを取得します。
    //!
    //! @return モデル内でのインデックスを返します。
    //!
    int GetIndex() const NN_NOEXCEPT
    {
        return static_cast<int>(ToData().index);
    }

    //! @brief 頂点数を取得します。
    //!
    //! @return 頂点数を返します。
    //!
    uint32_t GetCount() const NN_NOEXCEPT
    {
        return ToData().count;
    }

    //! @brief 1頂点を計算するのに使用するボーン数を取得します。
    //!
    //! @return 1頂点を計算するのに使用するボーン数を返します。
    //!
    int GetVertexInfluenceCount() const NN_NOEXCEPT
    {
        return ToData().influenceCount;
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

    //! @brief 内部に保持する頂点属性から動的頂点バッファーのインデックス情報を計算し取得します。
    //!
    //! @param[in,out]  pOutDynamicVertexBufferMask 動的頂点バッファーのインデックスの格納先へのポインタ。
    //!
    //! @pre
    //! - pOutDynamicVertexBufferMask != NULL
    //!
    void CalculateDynamicVertexBufferIndex(DynamicVertexBufferMask* pOutDynamicVertexBufferMask) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 頂点属性
    //@{

    //! @brief 頂点属性の数を取得します。
    //!
    //! @return 頂点属性の数を返します。
    //!
    int GetVertexAttrCount() const NN_NOEXCEPT
    {
        return ToData().vertexAttrCount;
    }

    //! @brief 指定した名前の頂点属性を取得します。
    //!
    //! @param[in] str 頂点属性名。
    //!
    //! @return ResVertexAttr へのポインターを返します。
    //!
    //! @details
    //! 指定した名前の頂点属性が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResVertexAttr* FindVertexAttr(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pVertexAttrDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResVertexAttr* pArray = ToData().pVertexAttrArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前の頂点属性を取得します。
    //!
    //! @param[in] str 頂点属性名。
    //!
    //! @return ResVertexAttr へのポインターを返します。
    //!
    //! @details
    //! 指定した名前の頂点属性が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResVertexAttr* FindVertexAttr(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pVertexAttrDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResVertexAttr* pArray = ToData().pVertexAttrArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前の頂点属性のインデックスを取得します。
    //!
    //! @param[in] str 頂点属性名。
    //!
    //! @return 頂点属性のインデックスを返します。
    //!
    //! @details
    //! 指定した名前の頂点属性が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindVertexAttrIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pVertexAttrDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }

    //! @brief 頂点属性のインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex 頂点属性のインデックス。
    //!
    //! @return 頂点属性名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは頂点属性の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetVertexAttrName(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetVertexAttrCount());
        const nn::util::ResDic* pDic = ToData().pVertexAttrDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }

    //! @brief インデックス引きで頂点属性を取得します。
    //!
    //! @param[in] elemIndex 頂点属性のインデックス。
    //!
    //! @return ResVertexAttr へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは頂点属性の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResVertexAttr* GetVertexAttr(int elemIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetVertexAttrCount());
        ResVertexAttr* pArray = ToData().pVertexAttrArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief インデックス引きで頂点属性を取得します。
    //!
    //! @param[in] elemIndex 頂点属性のインデックス。
    //!
    //! @return ResVertexAttr へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは頂点属性の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResVertexAttr* GetVertexAttr(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetVertexAttrCount());
        const ResVertexAttr* pArray = ToData().pVertexAttrArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name 頂点バッファー
    //@{

    //! @brief 頂点バッファーの数を取得します。
    //!
    //! @return 頂点バッファーの数を返します。
    //!
    int GetVertexBufferCount() const NN_NOEXCEPT
    {
        return vertexBufferCount;
    }

    //! @brief インデックス引きで頂点バッファーを取得します。
    //!
    //! @param[in] elemIndex 頂点バッファーのインデックス。
    //!
    //! @return nn::gfx::Buffer へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは頂点バッファーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    nn::gfx::Buffer* GetVertexBuffer(int elemIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetVertexBufferCount());
        nn::gfx::Buffer** ppArray = ToData().pVertexBufferPtrArray.Get();
        NN_SDK_ASSERT_NOT_NULL(ppArray);
        NN_SDK_ASSERT_NOT_NULL(*ppArray);
        return ppArray[elemIndex];
    }

    //! @brief インデックス引きで頂点バッファーを取得します。
    //!
    //! @param[in] elemIndex 頂点バッファーのインデックス。
    //!
    //! @return nn::gfx::Buffer へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは頂点バッファーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const nn::gfx::Buffer* GetVertexBuffer(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetVertexBufferCount());
        nn::gfx::Buffer* const * ppArray = ToData().pVertexBufferPtrArray.Get();
        NN_SDK_ASSERT_NOT_NULL(ppArray);
        NN_SDK_ASSERT_NOT_NULL(*ppArray);
        return ppArray[elemIndex];
    }

    //! @brief 頂点バッファー情報を取得します。
    //!
    //! @param[in] elemIndex 頂点バッファーのインデックス。
    //!
    //! @return nn::gfx::Buffer::InfoType へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは頂点バッファーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    nn::gfx::Buffer::InfoType* GetVertexBufferInfo(int elemIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetVertexBufferCount());
        nn::gfx::BufferInfo* pArray = ToData().pVertexBufferInfoArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief 頂点バッファー情報を取得します。
    //!
    //! @param[in] elemIndex 頂点バッファーのインデックス。
    //!
    //! @return nn::gfx::Buffer::InfoType へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは頂点バッファーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const nn::gfx::Buffer::InfoType* GetVertexBufferInfo(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetVertexBufferCount());
        const nn::gfx::BufferInfo* pArray = ToData().pVertexBufferInfoArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief 頂点バッファーの stride を取得します。
    //!
    //! @param[in] elemIndex 頂点バッファーのインデックス。
    //!
    //! @return 頂点バッファーの stride を返します。
    //!
    //! @pre
    //! - インデックスは頂点バッファーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ptrdiff_t GetVertexBufferStride(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetVertexBufferCount());
        const nn::gfx::VertexBufferStateInfo* pInfo = GetVertexBufferStateInfo(elemIndex);
        NN_SDK_ASSERT_NOT_NULL(pInfo);
        return pInfo->GetStride();
    }

    //! @brief 頂点バッファーのメモリプール内でのオフセットを取得します。
    //!
    //! @param[in] elemIndex 頂点バッファーのインデックス。
    //!
    //! @return 頂点バッファーのメモリプール内でのオフセットを返します。
    //!
    //! @pre
    //! - インデックスは頂点バッファーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //! 頂点バッファーが置かれているメモリプールの情報は nn::g3d::ResFile::GetMemoryInfo() から取得することができます。
    //!
    ptrdiff_t GetVertexBufferOffset(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetVertexBufferCount());
        ptrdiff_t offset = this->memoryPoolOffset;
        for (int idxBuffer = 0; idxBuffer < elemIndex; ++idxBuffer)
        {
            const nn::gfx::Buffer::InfoType* pBufferInfo = GetVertexBufferInfo(idxBuffer);
            offset += nn::util::align_up(pBufferInfo->GetSize(), Alignment_VertexBuffer);
        }
        return offset;
    }

    //@}

private:
    enum Alignment
    {
        Alignment_VertexBuffer = 8
    };

    //! @brief インデックス引きで VertexBufferStateInfo を取得します。
    nn::gfx::VertexBufferStateInfo* GetVertexBufferStateInfo(int elemIndex) NN_NOEXCEPT
    {
        nn::gfx::VertexBufferStateInfo* pArray = ToData().pVertexBufferStateInfoArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief インデックス引きで VertexBufferStateInfo を取得します。
    const nn::gfx::VertexBufferStateInfo* GetVertexBufferStateInfo(int elemIndex) const NN_NOEXCEPT
    {
        const nn::gfx::VertexBufferStateInfo* pArray = ToData().pVertexBufferStateInfoArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
};

//--------------------------------------------------------------------------------------------------

//! @brief サブメッシュの構造体です。
struct ResSubMeshData
{
    uint32_t offset; //!< インデックスバッファーの先頭からのバイトオフセット。
    uint32_t count; //!< 頂点インデックス数。
};

//! @brief サブメッシュです。
//!
//! ベース頂点インデックスを利用することによりインデックスバッファーを分割し、
//! インデックスの型を小さくすることに使用します。
//! ソートの単位には影響しません。
//!
class ResSubMesh : public nn::util::AccessorBase< ResSubMeshData >
{
    NN_DISALLOW_COPY( ResSubMesh );

public:
    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief インデックスバッファーの先頭からのバイトオフセットです。
    //!
    //! @return インデックスバッファーの先頭からのバイトオフセットを返します。
    //!
    ptrdiff_t GetOffset() const NN_NOEXCEPT
    {
        return ToData().offset;
    }

    //! @brief GetOffset() の位置から描画する頂点インデックス数を取得します。
    //!
    //! @return GetOffset() の位置から描画する頂点インデックス数を返します。
    //!
    uint32_t GetCount() const NN_NOEXCEPT
    {
        return ToData().count;
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief メッシュの構造体です。
struct ResMeshData
{
    nn::util::BinTPtr<ResSubMesh>          pSubMeshArray; //!< サブメッシュ配列へのポインター。
    nn::util::BinTPtr<nn::gfx::MemoryPool> pMemoryPool; //!< 頂点バッファー用メモリプールへのポインター。
    nn::util::BinTPtr<nn::gfx::Buffer>     pIndexBuffer; //!< インデックスバッファーへのポインター。
    nn::util::BinTPtr<nn::gfx::BufferInfo> pIndexBufferInfo; //!< インデックスバッファー情報へのポインター。

    uint32_t memoryPoolOffset; //!< メモリプールのオフセット。
    int32_t  primType; //!< プリミティブタイプ。
    int32_t  format; //!< フォーマット。
    uint32_t count; //!< 頂点インデックス数。
    uint32_t offset; //!< 頂点インデックスの開始位置。
    uint16_t subMeshCount; //!< サブメッシュ数。
    uint8_t  reserved[2];
};

//! @brief メッシュのリソースです。インデックスバッファーを持ちます。
class ResMesh : public nn::util::AccessorBase< ResMeshData >
{
    NN_DISALLOW_COPY( ResMesh );

public:
    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @briefprivate メッシュをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    void Setup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @briefprivate メッシュをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
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

    //! @briefprivate メッシュをクリーンアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    void Cleanup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @brief メッシュをリセットします。
    void Reset() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 描画
    //@{

    //! @brief すべてのサブメッシュを描画します。
    //!
    //! @param[in] pCommandBuffer コマンドバッファーへのポインター。
    //!
    //! @pre
    //! - commandBuffer が初期化されている。
    //!
    void Draw(nn::gfx::CommandBuffer* pCommandBuffer) const NN_NOEXCEPT
    {
        DrawSubMesh(pCommandBuffer, 0, GetSubMeshCount(), 1);
    }

    //! @brief インスタンス数を指定して、すべてのサブメッシュを描画します。
    //!
    //! @param[in] pCommandBuffer コマンドバッファーへのポインター。
    //! @param[in] instances 描画するインスタンスの数。
    //!
    //! @pre
    //! - commandBuffer が初期化されている。
    //! - instances > 0。
    //!
    void Draw(nn::gfx::CommandBuffer* pCommandBuffer, int instances) const NN_NOEXCEPT
    {
        DrawSubMesh(pCommandBuffer, 0, GetSubMeshCount(), instances);
    }

    //! @brief インスタンス数と開始インスタンスのインデックスを指定して、すべてのサブメッシュを描画します。
    //!
    //! @param[in] pCommandBuffer コマンドバッファーへのポインター。
    //! @param[in] instances 描画するインスタンスの数。
    //! @param[in] baseInstance 描画する開始インスタンスのインデックス。
    //!
    //! @pre
    //! - commandBuffer が初期化されている。
    //! - instances > 0。
    //! - baseInstance >= 0。
    //!
    void Draw(nn::gfx::CommandBuffer* pCommandBuffer, int instances, int baseInstance) const NN_NOEXCEPT
    {
        DrawSubMesh(pCommandBuffer, 0, GetSubMeshCount(), instances, baseInstance);
    }

    //! @brief 指定した範囲のサブメッシュを描画します。
    //!
    //! @param[in] pCommandBuffer コマンドバッファーへのポインター。
    //! @param[in] submeshIndex 開始サブメッシュのインデックス。
    //! @param[in] submeshCount サブメッシュの数。
    //!
    //! @pre
    //! - commandBuffer が初期化されている。
    //! - submeshCount > 0。
    //!
    void DrawSubMesh(nn::gfx::CommandBuffer* pCommandBuffer, int submeshIndex, int submeshCount) const NN_NOEXCEPT
    {
        DrawSubMesh(pCommandBuffer, submeshIndex, submeshCount, 1);
    }

    //! @brief インスタンス数を指定して、指定した範囲のサブメッシュを描画します。
    //!
    //! @param[in] pCommandBuffer コマンドバッファーへのポインター。
    //! @param[in] submeshIndex 開始サブメッシュのインデックス。
    //! @param[in] submeshCount サブメッシュの数。
    //! @param[in] instances 描画するインスタンスの数。
    //!
    //! @pre
    //! - commandBuffer が初期化されている。
    //! - submeshCount > 0。
    //! - instances > 0。
    //!
    void DrawSubMesh(nn::gfx::CommandBuffer* pCommandBuffer, int submeshIndex, int submeshCount, int instances) const NN_NOEXCEPT;

    //! @brief インスタンス数と開始インスタンスのインデックスを指定して、指定した範囲のサブメッシュを描画します。
    //!
    //! @param[in] pCommandBuffer コマンドバッファーへのポインター。
    //! @param[in] submeshIndex 開始サブメッシュのインデックス。
    //! @param[in] submeshCount サブメッシュの数。
    //! @param[in] instances 描画するインスタンスの数。
    //! @param[in] baseInstance 開始インスタンスのインデックス。
    //!
    //! @pre
    //! - commandBuffer が初期化されている。
    //! - submeshCount > 0。
    //! - instances > 0。
    //! - baseInstance >= 0。
    //!
    void DrawSubMesh(nn::gfx::CommandBuffer* pCommandBuffer, int submeshIndex, int submeshCount, int instances, int baseInstance) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief 形状タイプを取得します。
    //!
    //! @return 形状タイプを返します。
    //!
    nn::gfx::PrimitiveTopology GetPrimitiveType() const NN_NOEXCEPT
    {
        return static_cast<nn::gfx::PrimitiveTopology>(ToData().primType);
    }

    //! @brief インデックスデータのフォーマットを取得します。
    //!
    //! @return インデックスデータのフォーマットを返します。
    //!
    nn::gfx::IndexFormat GetIndexFormat() const NN_NOEXCEPT
    {
        return static_cast<nn::gfx::IndexFormat>(ToData().format);
    }

    //! @brief 頂点インデックス数を取得します。
    //!
    //! @return 頂点インデックス数を返します。
    //!
    uint32_t GetCount() const NN_NOEXCEPT
    {
        return ToData().count;
    }

    //@}

    //----------------------------------------
    //! @name サブメッシュ
    //@{

    //! @brief サブメッシュの数を取得します。
    //!
    //! @return サブメッシュの数を返します。
    //!
    int GetSubMeshCount() const NN_NOEXCEPT
    {
        return ToData().subMeshCount;
    }
    //! @brief インデックス引きでサブメッシュを取得します。
    //!
    //! @param[in] elemIndex サブメッシュのインデックス。
    //!
    //! @return ResVertex へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはサブメッシュの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResSubMesh* GetSubMesh(int elemIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetSubMeshCount());
        ResSubMesh* pArray = ToData().pSubMeshArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでサブメッシュを取得します。
    //!
    //! @param[in] elemIndex サブメッシュのインデックス。
    //!
    //! @return ResVertex へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはサブメッシュの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResSubMesh* GetSubMesh(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetSubMeshCount());
        const ResSubMesh* pArray = ToData().pSubMeshArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief submeshIndex から submeshCount 個のサブメッシュに含まれる頂点インデックス数を取得します。
    //!
    //! @param[in] submeshIndex 開始サブメッシュのインデックス。
    //! @param[in] submeshCount サブメッシュの数。
    //!
    //! @return 頂点インデックス数を返します。
    //!
    uint32_t GetIndexCount(int submeshIndex, int submeshCount) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(submeshIndex, 0, GetSubMeshCount());
        NN_SDK_REQUIRES(submeshCount > 0);
        const ResSubMesh* pFirstSubMesh = GetSubMesh(submeshIndex);
        const ResSubMesh* pLastSubMesh = GetSubMesh(submeshIndex + submeshCount - 1);
        // offset はバイト換算なので count 計算時には個数に戻す。
        return static_cast<uint32_t>(((pLastSubMesh->GetOffset() - pFirstSubMesh->GetOffset()) >> GetIndexFormat()) + pLastSubMesh->GetCount());
    }

    //@}

    //----------------------------------------
    //! @name インデックスバッファー
    //@{

    //! @brief インデックスバッファーを取得します。
    //!
    //! @return nn::gfx::Buffer へのポインターを返します。
    //!
    nn::gfx::Buffer* GetIndexBuffer() NN_NOEXCEPT
    {
        return ToData().pIndexBuffer.Get();
    }

    //! @brief インデックスバッファーのビューを取得します。
    //!
    //! @return nn::gfx::Buffer へのポインターを返します。
    //!
    const nn::gfx::Buffer* GetIndexBuffer() const NN_NOEXCEPT
    {
        return ToData().pIndexBuffer.Get();
    }

    //! @brief インデックスバッファー情報を取得します。
    //!
    //! @return nn::gfx::Buffer::InfoType へのポインターを返します。
    //!
    const nn::gfx::BufferInfo* GetIndexBufferInfo() const NN_NOEXCEPT
    {
        const nn::gfx::BufferInfo* pData = ToData().pIndexBufferInfo.Get();
        if (pData == NULL)
        {
            return NULL;
        }
        return pData;
    }

    //! @brief インデックスバッファー情報を取得します。
    //!
    //! @return nn::gfx::Buffer::InfoType へのポインターを返します。
    //!
    nn::gfx::BufferInfo* GetIndexBufferInfo() NN_NOEXCEPT
    {
        nn::gfx::BufferInfo* pData = ToData().pIndexBufferInfo.Get();
        if (pData == NULL)
        {
            return NULL;
        }
        return pData;
    }

    //! @brief インデックスバッファーのメモリプール内でのオフセットを取得します。
    //!
    //! @return インデックスバッファーのメモリプール内でのオフセットを返します。
    //!
    //! @details
    //! インデックスバッファーが置かれているメモリプールの情報は nn::g3d::ResFile::GetMemoryInfo() から取得することができます。
    //!
    ptrdiff_t GetIndexBufferOffset() const NN_NOEXCEPT
    {
        return ToData().memoryPoolOffset;
    }

    //! @brief 頂点インデックスの開始位置を取得します。
    //!
    //! @return 頂点インデックスの開始位置を返します。
    //!
    //! @details
    //! 頂点バッファー内における、このメッシュの開始頂点位置。描画時に頂点インデックスに加算されるベースインデックス値になります。
    //!
    ptrdiff_t GetOffset() const NN_NOEXCEPT
    {
        return ToData().offset;
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief キーシェイプの構造体です。
struct ResKeyShapeData
{
    uint8_t targetAttribIndices[20]; //!< 頂点属性インデックス。position, normal, tangent4, binormal4, color8, padding2 の順です。
};

//! @brief キーシェイプのリソースです。
class ResKeyShape : public nn::util::AccessorBase< ResKeyShapeData >
{
    NN_DISALLOW_COPY( ResKeyShape );

public:

    enum KeyAttr
    {
        KeyAttr_PositionCount = 1,
        KeyAttr_NormalCount   = 1,
        KeyAttr_TangentCount  = 4,
        KeyAttr_BinormalCount = 4,
        KeyAttr_ColorCount    = 8,

        KeyAttr_PositionOffset = 0,
        KeyAttr_NormalOffset   = KeyAttr_PositionOffset + KeyAttr_PositionCount,
        KeyAttr_TangentOffset  = KeyAttr_NormalOffset   + KeyAttr_NormalCount,
        KeyAttr_BinormalOffset = KeyAttr_TangentOffset  + KeyAttr_TangentCount,
        KeyAttr_ColorOffset    = KeyAttr_BinormalOffset + KeyAttr_BinormalCount
    };

    static const int InvalidIndex = -1;

    //! @brief キーシェイプがもつ位置属性の頂点属性インデックス取得します。
    //!
    //! @return キーシェイプがもつ位置属性の頂点属性インデックスを返します。
    //!
    //! @details キーシェイプが位置属性の頂点属性インデックスを持っていない場合は、InvalidIndex を返します。
    //!
    int GetPositionAttribIndex() const NN_NOEXCEPT
    {
        return ToData().targetAttribIndices[KeyAttr_PositionOffset] - 1;
    }

    //! @brief キーシェイプがもつ法線属性の頂点属性インデックス取得します。
    //!
    //! @return キーシェイプがもつ法線属性の頂点属性インデックスを返します。
    //!
    //! @details キーシェイプが法線属性の頂点属性インデックスを持っていない場合は、InvalidIndex を返します。
    //!
    int GetNormalAttribIndex() const NN_NOEXCEPT
    {
        return ToData().targetAttribIndices[KeyAttr_NormalOffset] - 1;
    }

    //! @brief キーシェイプがもつ接線属性の頂点属性インデックスを取得します。
    //!
    //! @return キーシェイプがもつ接線属性の頂点属性インデックスを返します。
    //!
    //! @details キーシェイプが接線属性の頂点属性インデックスを持っていない場合は、InvalidIndex を返します。
    //!
    int GetTangentAttribIndex() const NN_NOEXCEPT
    {
        return GetTangentAttribIndex(0);
    }

    int GetTangentAttribIndex(int index) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, KeyAttr_TangentCount);
        return ToData().targetAttribIndices[KeyAttr_TangentOffset + index] - 1;
    }

    //! @brief キーシェイプがもつ従法線属性の頂点属性インデックスを取得します。
    //!
    //! @return キーシェイプがもつ従法線属性の頂点属性インデックスを返します。
    //!
    //! @details キーシェイプが従法線属性の頂点属性インデックスを持っていない場合は、InvalidIndex を返します。
    //!
    int GetBinormalAttribIndex() const NN_NOEXCEPT
    {
        return GetBinormalAttribIndex(0);
    }

    int GetBinormalAttribIndex(int index) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, KeyAttr_BinormalCount);
        return ToData().targetAttribIndices[KeyAttr_BinormalOffset + index] - 1;
    }

    //! @brief キーシェイプがもつカラー属性の頂点属性インデックスを取得します。
    //!
    //! @return キーシェイプがもつカラー属性の頂点属性インデックスを返します。
    //!
    //! @details キーシェイプがカラー属性の頂点属性インデックスを持っていない場合は、InvalidIndex を返します。
    //!
    int GetColorAttribIndex() const NN_NOEXCEPT
    {
        return GetColorAttribIndex(0);
    }

    int GetColorAttribIndex(int index) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, KeyAttr_ColorCount);
        return ToData().targetAttribIndices[KeyAttr_ColorOffset + index] - 1;
    }
};

//--------------------------------------------------------------------------------------------------

//! @brief AABB(Axis-Aligned-Bounding-Box) のバウンディング情報です。
struct Bounding
{
    nn::util::Float3 center; //!< 中心。
    nn::util::Float3 extent; //!< 範囲。
};

//! @brief シェイプの構造体です。
struct ResShapeData
{
    nn::util::BinBlockSignature         signature; //!< シグネチャー。
    Bit32                               flag; //!< フラグ。
    nn::util::BinPtrToString            pName; //!< 名前。
    nn::util::BinTPtr<ResVertex>        pVertex; //!< 頂点情報へのポインター。
    nn::util::BinTPtr<ResMesh>          pMeshArray; //!< メッシュ配列へのポインター。
    nn::util::BinTPtr<uint16_t>         pSkinBoneIndexArray; //!< スキニングで参照するボーンインデックスのソート済み配列へのポインター。
    nn::util::BinTPtr<ResKeyShape>      pKeyShapeArray; //!< キーシェイプ配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic> pKeyShapeDic; //!< キーシェイプ辞書へのポインター。
    nn::util::BinTPtr<Bounding>         pSubMeshBoundingArray; //!< 各 LOD のサブメッシュのバウンディング配列へのポインター。LODごとにバウンディングを持つ場合、サブメッシュのバウンディング + メッシュのバウンディングがLOD分並んでいる。
    nn::util::BinTPtr<float>            pRadiusArray; //!< 各 LOD のバウンディング球の半径配列へのポインター。
    nn::util::BinPtr pUserPtr; //!< ユーザーポインター。

    uint16_t index; //!< モデル内でのインデックス。
    uint16_t materialIndex; //!< モデル内の参照するマテリアルインデックス。
    uint16_t boneIndex; //!< モデル内の参照するボーンインデックス。
    uint16_t vertexIndex; //!< モデル内の参照する頂点情報インデックス。
    uint16_t skinBoneIndexCount; //!< スキニングで使用するボーンの総数。
    uint8_t  vtxSkinCount; //!< スキニングで1頂点に影響するボーンの数。
    uint8_t  meshCount; //!< メッシュ数。
    uint8_t  keyShapeCount; //!< キーシェイプ数。
    uint8_t  targetAttrCount; //!< キーシェイプが持つ頂点属性の数。
    uint8_t  reserved[2];
};

//! @brief シェイプのリソースです。
class ResShape : public nn::util::AccessorBase< ResShapeData > // LOD の情報は含まない
{
    NN_DISALLOW_COPY( ResShape );

public:
    //! @brief ResShape のシグネチャ
    static const uint32_t Signature = NN_UTIL_CREATE_SIGNATURE_4('F', 'S', 'H', 'P');

    //! @briefprivate 無効なマテリアルインデックス
    static const uint16_t InvalidMaterialIndex = UINT16_MAX;

    //! @brief シェイプに関するフラグです。
    enum Flag
    {
        //! @brief ResVertex の所有権を持ちます。
        Flag_OwnVertex                 = 0x1 << 1,
        //! @brief メッシュ間でサブメッシュ境界が維持されています。
        Flag_SubMeshBoundaryConsistent = 0x1 << 2,
    };

    //! @brief リセット時にリセットしないものを指定するフラグです。
    enum ResetGuardFlag
    {
        ResetGuardFlag_None       = 0, //!< すべてリセットします。
        ResetGuardFlag_UserPtr    = 0x1 << 0, //!< ユーザーポインターの設定をリセットしません。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @briefprivate シェイプをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    void Setup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @briefprivate シェイプをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
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

    //! @briefprivate シェイプをクリーンアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    void Cleanup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @brief シェイプをリセットします。
    void Reset() NN_NOEXCEPT;

    //! @brief シェイプをリセットします。
    void Reset(Bit32 resetGuardFlag) NN_NOEXCEPT;
    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief モデル内でのインデックスを取得します。
    //!
    //! @return モデル内でのインデックスを返します。
    //!
    int GetIndex() const NN_NOEXCEPT
    {
        return ToData().index;
    }

    //! @brief シェイプ名を取得します。
    //!
    //! @return シェイプ名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }

    //! @brief マテリアルのインデックスを取得します。
    //!
    //! @return マテリアルのインデックスを返します。
    //!
    int GetMaterialIndex() const NN_NOEXCEPT
    {
        return ToData().materialIndex;
    }

    //! @brief ビジビリティに用いるボーンのインデックスを取得します。
    //!
    //! @return ボーンのインデックスを返します。
    //!
    //! @details
    //! SkinBoneIndex とは異なることがあります。
    //!
    int GetBoneIndex() const NN_NOEXCEPT
    {
        return ToData().boneIndex;
    }

    //! @brief 頂点情報のインデックスを取得します。
    //!
    //! @return 頂点情報のインデックスを返します。
    //!
    int GetVertexIndex() const NN_NOEXCEPT
    {
        return ToData().vertexIndex;
    }

    //! @brief 頂点情報の所有権を持っているかどうかを取得します。
    //!
    //! @return 頂点情報の所有権を持っている場合は true、持っていない場合は false を返します。
    //!
    //! @details
    //! 複数のシェイプが同一の頂点情報を参照している場合、その中の1つが所有権を持ちます。
    //!
    bool IsVertexOwner() const NN_NOEXCEPT
    {
        return 0 != (ToData().flag & Flag_OwnVertex);
    }

    //! @brief スキニングで使用するボーンの総数を取得します。
    //!
    //! @return スキニングで使用するボーンの総数を返します。
    //!
    int GetSkinBoneIndexCount() const NN_NOEXCEPT
    {
        return ToData().skinBoneIndexCount;
    }

    //! @brief スキニングで使用するボーンのインデックスのリストを取得します。
    //!
    //! @return スキニングで使用するボーンのインデックスのリストを返します。
    //!
    const uint16_t* GetSkinBoneIndexArray() const NN_NOEXCEPT
    {
        return ToData().pSkinBoneIndexArray.Get();
    }

    //! @brief 1頂点を計算するのに使用するボーン数を取得します。
    //!
    //! @return 1頂点を計算するのに使用するボーン数を返します。
    //!
    int GetVertexSkinCount() const NN_NOEXCEPT
    {
        return ToData().vtxSkinCount;
    }

    //! @brief 剛体のシェイプかどうかを取得します。
    //!
    //! @return 剛体のシェイプの場合は true、それ以外の場合は false を返します。
    //!
    bool IsRigidBody() const NN_NOEXCEPT
    {
        return ToData().vtxSkinCount == 0;
    }

    //! @brief リジッドスキニングのシェイプかどうかを取得します。
    //!
    //! @return リジッドスキニングのシェイプの場合は true、それ以外の場合は false を返します。
    //!
    bool IsRigidSkinning() const NN_NOEXCEPT
    {
        return ToData().vtxSkinCount == 1;
    }

    //! @brief スムーススキニングのシェイプかどうかを取得します。
    //!
    //! @return スムーススキニングのシェイプの場合は true、それ以外の場合は false を返します。
    //!
    bool IsSmoothSkinning() const NN_NOEXCEPT
    {
        return ToData().vtxSkinCount > 1;
    }

    //! @brief メッシュ間でサブメッシュ境界が維持されているかを取得します。
    //!
    //! @return サブメッシュ境界が維持されている場合は true、それ以外の場合は false を返します。
    //!
    //! @details
    //! サブメッシュ境界が維持されているということは、すべてのメッシュにおいて、サブメッシュの数が
    //! 同じであり、同一のサブメッシュインデックスで示されるサブメッシュはバウンディングボックスの
    //! 形状がほぼ等しいことを意味します。
    //!
    //! 中間ファイルオプティマイザのポリゴンリダクション機能を使用して作成された LOD モデルはサブメッシュ
    //! 境界が維持されています。他の外部ツールで作成された LOD モデルを中間ファイルオプティマイザにより
    //! 結合した場合はサブメッシュ境界は維持されません。
    //!
    bool IsSubMeshBoundaryConsistent() const NN_NOEXCEPT
    {
        return (ToData().flag & Flag_SubMeshBoundaryConsistent) != 0;
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
    //! @name 頂点情報
    //@{

    //! @brief 頂点情報を取得します。
    //!
    //! @return ResVertex へのポインターを返します。
    //!
    ResVertex* GetVertex() NN_NOEXCEPT
    {
        return ToData().pVertex.Get();
    }
    //! @brief 頂点情報を取得します。
    //!
    //! @return ResVertex へのポインターを返します。
    //!
    const ResVertex* GetVertex() const NN_NOEXCEPT
    {
        return ToData().pVertex.Get();
    }

    //! @brief シェイプアニメーションを行うために必要な頂点属性を動的頂点バッファーに含めます。
    //!
    //! @details
    //! - キーシェイプを保持していない場合には何も行われません。
    //!
    void ActivateDynamicVertexAttrForShapeAnim() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name メッシュ
    //@{

    //! @brief メッシュの数を取得します。
    //!
    //! @return メッシュの数を返します。
    //!
    int GetMeshCount() const NN_NOEXCEPT
    {
        return ToData().meshCount;
    }

    //! @brief インデックス引きでメッシュを取得します。
    //!
    //! @param[in] elemIndex メッシュのインデックス。
    //!
    //! @return ResMesh へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはメッシュの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResMesh* GetMesh(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetMeshCount(), GetName());
        ResMesh* pArray = ToData().pMeshArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief インデックス引きでメッシュを取得します。
    //!
    //! @param[in] elemIndex メッシュのインデックス。
    //!
    //! @return ResMesh へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはメッシュの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResMesh* GetMesh(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetMeshCount(), GetName());
        const ResMesh* pArray = ToData().pMeshArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief サブメッシュの数を取得します。
    //!
    //! @return サブメッシュの数を返します。
    //!
    int GetSubMeshCount() const NN_NOEXCEPT
    {
        return ToData().pMeshArray.Get()->GetSubMeshCount();
    }

    //! @brief 指定したメッシュのサブメッシュの数を取得します。
    //!
    //! @param[in] meshIndex メッシュのインデックス。 LOD レベルを指定します。
    //!
    //! @return サブメッシュの数を返します。
    //!
    //! @pre
    //! - インデックスはメッシュの数の範囲内。
    //!
    int GetSubMeshCount(int meshIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(meshIndex, 0, GetMeshCount(), GetName());
        const ResMesh* pArray = ToData().pMeshArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return pArray[meshIndex].GetSubMeshCount();
    }
    //@}

    //----------------------------------------
    //! @name キーシェイプ
    //@{

    //! @brief キーシェイプの数を取得します。
    //!
    //! @return キーシェイプの数を返します。
    //!
    int GetKeyShapeCount() const NN_NOEXCEPT
    {
        return ToData().keyShapeCount;
    }

    //! @brief 指定した名前のキーシェイプを取得します。
    //!
    //! @param[in] str キーシェイプ名。
    //!
    //! @return ResKeyShape へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のキーシェイプが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResKeyShape* FindKeyShape(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pKeyShapeDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResKeyShape* pArray = ToData().pKeyShapeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前のキーシェイプを取得します。
    //!
    //! @param[in] str キーシェイプ名。
    //!
    //! @return ResKeyShape へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のキーシェイプが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResKeyShape* FindKeyShape(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pKeyShapeDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResKeyShape* pArray = ToData().pKeyShapeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前のキーシェイプのインデックスを取得します。
    //!
    //! @param[in] str キーシェイプ名。
    //!
    //! @return キーシェイプのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のキーシェイプが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindKeyShapeIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pKeyShapeDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }

    //! @brief キーシェイプのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex キーシェイプのインデックス。
    //!
    //! @return キーシェイプ名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはキーシェイプの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetKeyShapeName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetKeyShapeCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pKeyShapeDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }

    //! @brief インデックス引きでキーシェイプを取得します。
    //!
    //! @param[in] elemIndex キーシェイプのインデックス。
    //!
    //! @return ResKeyShape へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはキーシェイプの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResKeyShape* GetKeyShape(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetKeyShapeCount(), GetName());
        ResKeyShape* pArray = ToData().pKeyShapeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief インデックス引きでキーシェイプを取得します。
    //!
    //! @param[in] elemIndex キーシェイプのインデックス。
    //!
    //! @return ResKeyShape へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはキーシェイプの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResKeyShape* GetKeyShape(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetKeyShapeCount(), GetName());
        const ResKeyShape* pArray = ToData().pKeyShapeArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief キーシェイプが持つ頂点属性の数を取得します。
    //!
    //! @return キーシェイプが持つ頂点属性の数を返します。
    //!
    int GetTargetAttribCount() const NN_NOEXCEPT
    {
        return ToData().targetAttrCount;
    }

    //@}

    //----------------------------------------
    //! @name バウンディング情報
    //@{

    //! @brief AABB のバウンディングボックスを取得します。
    //!
    //! @return AABB のバウンディングボックスへの参照を返します。
    //!
    const Bounding& GetBounding() const NN_NOEXCEPT
    {
        return *(ToData().pSubMeshBoundingArray.Get() + GetSubMeshCount());
    }

    //! @brief 指定したメッシュに対応する AABB のバウンディングボックスを取得します。
    //!
    //! @param[in] meshIndex メッシュのインデックス。 LOD レベルを指定します。
    //!
    //! @return AABB のバウンディングボックスへの参照を返します。
    //!
    //! @pre
    //! - インデックスはメッシュの数の範囲内。
    //!
    const Bounding& GetBounding(int meshIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(meshIndex, 0, GetMeshCount(), GetName());
        int boundingIndex = 0;
        for (int index = 0; index <= meshIndex; ++index)
        {
            boundingIndex += GetSubMeshCount(index);
        }
        boundingIndex += meshIndex;
        const Bounding* pArray = ToData().pSubMeshBoundingArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return pArray[boundingIndex];
    }

    //! @brief AABB と中心を共有するバウンディングスフィアの半径を取得します。
    //!
    //! @return AABB と中心を共有するバウンディングスフィアの半径を返します。
    //!
    float GetRadius() const NN_NOEXCEPT
    {
        return ToData().pRadiusArray.Get()[0];
    }

    //! @brief 指定したメッシュに対応する AABB と中心を共有するバウンディングスフィアの半径を取得します。
    //!
    //! @param[in] meshIndex メッシュのインデックス。 LOD レベルを指定します。
    //!
    //! @return AABB と中心を共有するバウンディングスフィアの半径を返します。
    //!
    //! @pre
    //! - インデックスはメッシュの数の範囲内。
    //!
    float GetRadius(int meshIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(meshIndex, 0, GetMeshCount(), GetName());
        const float* pArray = ToData().pRadiusArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return pArray[meshIndex];
    }
    //@}
};

}} // namespace nn::g3d

NN_PRAGMA_POP_WARNINGS
