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
* @brief シェイプアニメーションに関するインスタンスクラスの宣言
*/

#pragma once

#include <nn/util/util_MemorySplitter.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_AnimObj.h>
#include <nn/g3d/g3d_ResShapeAnim.h>
#include <nn/g3d/g3d_Binding.h>

namespace nn { namespace g3d {

class ModelObj;
class ShapeObj;

//! @brief シェイプアニメーションインスタンスです。
class ShapeAnimObj : public ModelAnimObj
{
public:
    class Builder;

    //! @brief インスタンスの構築時に渡すバッファーの必要アライメントサイズです。
    enum Alignment
    {
        Alignment_Buffer = 8 //!< Builder に渡すバッファーの必要アライメントサイズです。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    //!
    //! 実際の構築処理は Builder で行います。
    //!
    ShapeAnimObj() NN_NOEXCEPT
        : ModelAnimObj()
        , m_pRes(NULL)
        , m_pVertexShapeAnimArray(NULL)
        , m_MaxVtxShpAnim(0)
        , m_MaxSubBind(0)
        , m_pSubBindIndexArray(NULL)
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
    //! @param[in] pModel ModelObj へのポインター。
    //!
    //! @return BindResult を返します。
    //!
    virtual BindResult Bind(const ModelObj* pModel) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pModel アニメーション対象の ResModel へのポインター。
    //!
    //! @pre
    //! - pModel と設定されている ResShapeAnim のリソースが ResShapeAnim::PreBind によって事前に関連付けられている。
    //!
    //! @details
    //! リソース同士を ResShapeAnim::PreBind によって事前に関連付けることにより、
    //! BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    virtual void BindFast(const ResModel* pModel) NN_NOEXCEPT;

    //! @brief 指定したインデックスのシェイプにアニメーションフラグを設定します。
    //!
    //! @param[in] shapeIndex シェイプのインデックス。
    //! @param[in] flag 設定するフラグ。
    //!
    void SetBindFlag(int shapeIndex, BindFlag flag) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        SetBindFlagImpl(shapeIndex, flag);
    }

    //! @brief 指定したインデックスのシェイプにアニメーションフラグを取得します。
    //!
    //! @param[in] shapeIndex シェイプのインデックス。
    //!
    //! @return フラグを返します。
    //!
    BindFlag GetBindFlag(int shapeIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        return GetBindFlagImpl(shapeIndex);
    }

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    virtual void ClearResult() NN_NOEXCEPT;
    virtual void Calculate() NN_NOEXCEPT;
    virtual void ApplyTo(ModelObj* pModelObj) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief リソースを取得します。
    //!
    //! @return シェイプアニメーションリソースへのポインターを返します。
    //!
    const ResShapeAnim* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    };

    //! @brief リソースを設定します。
    //!
    //! @param[in] pRes シェイプアニメーションリソースへのポインター。
    //!
    //! @details
    //! 計算するシェイプアニメーションを差し替えます。
    //!
    void SetResource(const ResShapeAnim* pRes) NN_NOEXCEPT;

    //! @brief リソースを取り外します。
    void ResetResource() NN_NOEXCEPT
    {
        m_pRes = NULL;
        SetTargetUnbound();
    }

    //! @brief アニメーション対象に対して十分なバッファーが存在するかどうかを取得します。
    //!
    //! @param[in] pModel 対象のモデルリソースへのポインター。
    //!
    //! @return 十分なバッファーが存在する場合は true、それ以外の場合は false を返します。
    //!
    bool IsAcceptable(const ResModel* pModel) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES(pModel != NULL, NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        return pModel->GetShapeCount() <= GetBindTable().GetSize();
    }

    //! @brief アニメーションリソースに対して十分なバッファーが存在するかどうかを取得します。
    //!
    //! @param[in] pRes 対象のシェイプアニメーションリソースへのポインター。
    //!
    //! @return 十分なバッファーが存在する場合は true、それ以外の場合は false を返します。
    //!
    bool IsAcceptable(const ResShapeAnim* pRes) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pRes);
        return pRes->GetVertexShapeAnimCount() <= m_MaxVtxShpAnim &&
            pRes->GetKeyShapeAnimCount() <= m_MaxSubBind;
    }

    //! @brief 指定したシェイプに適用されるアニメーションの対象キーシェイプ数を取得します。
    //!
    //! @param[in] shapeIndex シェイプのインデックス。
    //!
    //! @return 適用されるアニメーションの対象キーシェイプ数を返します。
    //!
    int GetKeyShapeCount(int shapeIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        int idxAnim = GetBindTable().GetAnimIndex(shapeIndex);
        if (idxAnim != AnimBindTable::Flag_NotBound && GetBindTable().IsApplyEnabled(idxAnim))
        {
            return m_pRes->GetVertexShapeAnim(idxAnim)->GetKeyShapeAnimCount();
        }
        return 0;
    }

    //! @brief 指定したシェイプに適用されるアニメーションの対象キーシェイプインデックスを取得します。
    //!
    //! @param[in] shapeIndex シェイプのインデックス。
    //!
    //! @return 適用されるアニメーションの対象キーシェイプインデックスの配列へのポインターを返します。
    //!
    int8_t* GetKeyShapeIndexArray(int shapeIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        int idxAnim = GetBindTable().GetAnimIndex(shapeIndex);
        if (idxAnim != AnimBindTable::Flag_NotBound && GetBindTable().IsApplyEnabled(idxAnim))
        {
            int32_t beginKeyShapeAnim = m_pRes->GetVertexShapeAnim(idxAnim)->ToData().beginKeyShapeAnim;
            return &m_pSubBindIndexArray[beginKeyShapeAnim];
        }
        return NULL;
    }

    //! @brief シェイプアニメーション名を取得します。
    //!
    //! @return ResShapeAnim のシェイプアニメーション名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return m_pRes->GetName();
    }

    //@}

protected:

    //! @briefprivate 頂点アニメーション取得します。
    //!
    const ResVertexShapeAnim* GetVertexShapeAnim(int animIndex) const NN_NOEXCEPT
    {
        return &m_pVertexShapeAnimArray[animIndex];
    }

    //! @briefprivate シェイプごとに関連付けを行う内部関数です。
    //!
    BindResult SubBind(const ResVertexShapeAnim* pVertexShapeAnim, const ResShape* pShape) NN_NOEXCEPT;

    //! @briefprivate シェイプごとに関連付けを行う内部関数です。
    //!
    BindResult SubBindFast(const ResVertexShapeAnim* pVertexShapeAnim) NN_NOEXCEPT;

    //! @briefprivate シェイプごとに適用を行う内部関数です。
    //!
    void ApplyTo(ShapeObj* pShapeObj, int animIndex) const NN_NOEXCEPT;

private:
    class InitializeArgument;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    const ResShapeAnim* m_pRes;
    const ResVertexShapeAnim* m_pVertexShapeAnimArray;
    int32_t m_MaxVtxShpAnim;
    int32_t m_MaxSubBind;
    int8_t* m_pSubBindIndexArray;

    NN_DISALLOW_COPY(ShapeAnimObj);
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate ShapeAnimObj::Initialize() に渡して初期化を行うパラメータです。
class ShapeAnimObj::InitializeArgument
{
public:
    //! @brief メモリブロックのインデックスを示す列挙子。
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_ResultBuffer,
        MemoryBlockIndex_BindTable,
        MemoryBlockIndex_SubBindTable,
        MemoryBlockIndex_FrameCacheArray,
        MemoryBlockIndex_End
    };

    //! @brief コンストラクタです。
    //!
    //! @details
    //! インスタンスで使用するすべてのシェイプアニメーションリソースとすべてのモデルリソースに対して
    //! Reserve() するか、SetMaxShapeCount()、SetMaxVertexShapeAnimCount()、
    //! SetMaxKeyShapeAnimCount()、SetMaxCurveCount() でバッファーサイズの計算に必要な
    //! パラメータを指定する必要があります。
    //!
    InitializeArgument() NN_NOEXCEPT
    {
        Clear();
    }

    //! @brief パラメータを初期化します。
    void Clear() NN_NOEXCEPT
    {
        m_ShapeCount = m_VertexShapeAnimCount = m_KeyShapeAnimCount = m_CurveCount = -1;
        m_ContextEnabled = true;
        m_ContextAvailable = false;
        m_WorkMemory.Invalidate();
        for (int blockIndex = 0; blockIndex < MemoryBlockIndex_End; ++blockIndex)
        {
            m_MemoryBlock[blockIndex].Initialize();
        }
    }

    //! @brief 対象となるモデルリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResModel モデルリソースへのポインター。
    //!
    void Reserve(const ResModel* pResModel) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResModel);
        m_ShapeCount = std::max(m_ShapeCount, pResModel->GetShapeCount());
    }

    //! @brief 対象となるシェイプアニメーションリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResAnim シェイプアニメーションリソースへのポインター。
    //!
    void Reserve(const ResShapeAnim* pResAnim) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResAnim);
        m_VertexShapeAnimCount = std::max(m_VertexShapeAnimCount, pResAnim->GetVertexShapeAnimCount());
        m_KeyShapeAnimCount = std::max(m_KeyShapeAnimCount, pResAnim->GetKeyShapeAnimCount());
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

    //! @brief 使用できる最大のシェイプ数を指定します。
    //!
    //! @param[in] shapeCount 使用できる最大のシェイプ数。
    //!
    void SetMaxShapeCount(int shapeCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(shapeCount >= 0);
        m_ShapeCount = shapeCount;
    }

    //! @brief 使用できる最大のシェイプ数を取得します。
    //!
    //! @return 使用できる最大のシェイプ数を返します。
    //!
    int GetMaxShapeCount() const NN_NOEXCEPT
    {
        return m_ShapeCount;
    }

    //! @brief 使用できる最大のシェイプアニメーション数を指定します。
    //!
    //! @param[in] vertexShapeAnimCount 使用できる最大のシェイプアニメーション数。
    //!
    void SetMaxVertexShapeAnimCount(int vertexShapeAnimCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(vertexShapeAnimCount >= 0);
        m_VertexShapeAnimCount = vertexShapeAnimCount;
    }

    //! @brief 使用できる最大のシェイプアニメーション数を取得します。
    //!
    //! @return 使用できる最大のシェイプアニメーション数を返します。
    //!
    int GetMaxVertexShapeAnimCount() const NN_NOEXCEPT
    {
        return m_VertexShapeAnimCount;
    }

    //! @brief 使用できる最大のキーシェイプアニメーション数を指定します。
    //!
    //! @param[in] keyShapeAnimCount 使用できる最大のキーシェイプアニメーション数。
    //!
    void SetMaxKeyShapeAnimCount(int keyShapeAnimCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(keyShapeAnimCount >= 0);
        m_KeyShapeAnimCount = keyShapeAnimCount;
    }

    //! @brief 使用できる最大のキーシェイプアニメーション数を取得します。
    //!
    //! @return 使用できる最大のキーシェイプアニメーション数を返します。
    //!
    int GetMaxKeyShapeAnimCount() const NN_NOEXCEPT
    {
        return m_KeyShapeAnimCount;
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

    //! @brief 必要なパラメータが設定されているかどうかを取得します。
    //!
    //! @return 必要なパラメータが設定されている場合は true、設定されていない場合は false を返します。
    //!
    bool IsValid() const NN_NOEXCEPT
    {
        return m_ShapeCount >= 0 && m_VertexShapeAnimCount >= 0 &&
               m_VertexShapeAnimCount >= 0 && m_CurveCount >= 0;
    }

    //! @brief ShapeAnimObj 構築に必要なメモリサイズを取得します。
    //!
    //! @return  ShapeAnimObj 構築に必要なメモリサイズを返します。
    //!
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief ShapeAnimObj 構築に必要なメモリサイズを計算します。
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

    //! @brief SkeletalAnimObj 構築に必要なメモリサイズが計算済みかを取得します。
    //!
    //! @return 計算済みの場合は true、未計算の場合は false を返します。
    //!
    bool IsMemoryCalculated() const NN_NOEXCEPT
    {
        return m_WorkMemory.IsValid();
    }

private:
    int m_ShapeCount;
    int m_VertexShapeAnimCount;
    int m_KeyShapeAnimCount;
    int m_CurveCount;
    bool m_ContextEnabled;
    bool m_ContextAvailable; // リソースがベイクされていないか、カーブ数を手動指定した場合に true
    nn::util::MemorySplitter m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
};

//! @brief ShapeAnimObj の構築を行うクラスです。
class ShapeAnimObj::Builder : public ShapeAnimObj::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @details
    //! インスタンスで使用するすべてのシェイプアニメーションリソースとすべてのモデルリソースに対して
    //! Reserve() するか、SetMaxShapeCount()、SetMaxVertexShapeAnimCount()、
    //! SetMaxKeyShapeAnimCount()、SetMaxCurveCount() でバッファーサイズの計算に必要な
    //! パラメータを指定する必要があります。
    //!
    Builder()
    {
    }

    //! @brief ShapeAnimObj を構築します。
    //!
    //! @param[in] pShapeAnimObj ShapeAnimObj へのポインター
    //! @param[in] pBuffer バッファーへのポインター
    //! @param[in] bufferSize バッファーのサイズ
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ
    //!
    bool Build(ShapeAnimObj* pShapeAnimObj, void* pBuffer, size_t bufferSize) const NN_NOEXCEPT
    {
        return pShapeAnimObj->Initialize(*this, pBuffer, bufferSize);
    }
};

}} // namespace nn::g3d

