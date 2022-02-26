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
* @brief トランスフォームツリーアニメーションに関するインスタンスクラスの宣言
*/

#pragma once

#include <nn/util/util_MemorySplitter.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/detail/g3d_GeneralAnimObj.h>
#include <nn/g3d/detail/g3d_TransformTreeObj.h>
#include <nn/g3d/detail/g3d_ResTransformTreeAnim.h>

namespace nn { namespace g3d { namespace detail {

//! @brief トランスフォームツリーアニメーションインスタンスです。
class TransformTreeAnimObj : public GeneralAnimObj
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
    TransformTreeAnimObj() NN_NOEXCEPT
        : GeneralAnimObj()
        , m_pRes(NULL)
        , m_pTransformNodeAnimArray(NULL)
        , m_MaxTransformNodeAnim(0)
        , m_Flag(0)
        , m_pResTransformTree(NULL)
    {
    }

    //@}

    //----------------------------------------
    //! @name 関連付け
    //@{

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pTransformTree アニメーション対象のトランスフォームツリーリソースへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    BindResult Bind(const ResTransformTree* pTransformTree) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pTransformTree アニメーション対象のトランスフォームツリーリソースへのポインター。
    //!
    //! @pre
    //! - pTransformTree と設定されている ResTransformTreeAnim のリソースが ResTransformTreeAim::PreBind によって事前に関連付けられている。
    //!
    //! @details
    //! リソース同士を ResTransformTreeAnim::PreBind によって事前に関連付けることにより、
    //! BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    void BindFast(const ResTransformTree* pTransformTree) NN_NOEXCEPT;

    //! @brief 指定したインデックスのトランスフォームノードにアニメーションフラグを設定します。
    //!
    //! @param[in] transformNodeIndex トランスフォームノードのインデックス。
    //! @param[in] flag 設定するフラグ。
    //!
    void SetBindFlag(int transformNodeIndex, BindFlag flag) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        SetBindFlagImpl(transformNodeIndex, flag);
    }

    //! @brief トランスフォームツリー構造を辿って指定したインデックスのトランスフォームノード以下のトランスフォームノードにアニメーションフラグを設定します。
    //!
    //! @param[in] pTransformTree トランスフォームツリーリソースへのポインター。
    //! @param[in] transformNodeIndex トランスフォームノードのインデックス。
    //! @param[in] flag 設定するフラグ。
    //!
    void SetBindFlag(const ResTransformTree* pTransformTree, int transformNodeIndex, BindFlag flag) NN_NOEXCEPT;

    //! @brief 指定したインデックスのトランスフォームノードのアニメーションフラグを取得します。
    //!
    //! @param[in] transformNodeIndex トランスフォームノードのインデックス。
    //!
    //! @return フラグを返します。
    //!
    BindFlag GetBindFlag(int transformNodeIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        return GetBindFlagImpl(transformNodeIndex);
    }

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    virtual void ClearResult() NN_NOEXCEPT;
    virtual void Calculate() NN_NOEXCEPT;

    //! @brief ResTransformTree を用いて初期化を行います。
    //!
    //! @param[in] pTransformTree トランスフォームツリーリソースへのポインター。
    //!
    void ClearResult(const ResTransformTree* pTransformTree) NN_NOEXCEPT;

    //! @brief アニメーション結果を対象に適用します。
    //!
    //! @param[in] pTransformTreeObj トランスフォームツリーへのポインター。
    //!
    void ApplyTo(TransformTreeObj* pTransformTreeObj) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief リソースを取得します。
    //!
    //! @return トランスフォームツリーアニメーションリソースへのポインターを返します。
    //!
    const ResTransformTreeAnim* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    };

    //! @brief リソースを設定します。
    //!
    //! @param[in] pRes トランスフォームツリーアニメーションリソースへのポインター。
    //!
    //! @details
    //! 計算するトランスフォームツリーアニメーションを差し替えます。
    //!
    void SetResource(const ResTransformTreeAnim* pRes) NN_NOEXCEPT;

    //! @brief リソースを取り外します。
    void ResetResource() NN_NOEXCEPT
    {
        m_pRes = NULL;
        SetTargetUnbound();
    }

    //! @brief アニメーションの計算結果を取得します。
    //!
    //! @return アニメーションの計算結果を格納した TransformNodeAnimResult 配列へのポインターを返します。
    //!
    TransformNodeAnimResult* GetResultArray() NN_NOEXCEPT
    {
        return static_cast<TransformNodeAnimResult*>(GetResultBuffer());
    }

    //! @brief アニメーションの計算結果を取得します。
    //!
    //! @return アニメーションの計算結果を格納した TransformNodeAnimResult 配列へのポインターを返します。
    //!
    const TransformNodeAnimResult* GetResultArray() const NN_NOEXCEPT
    {
        return static_cast<const TransformNodeAnimResult*>(GetResultBuffer());
    }

    //! @brief アニメーション対象に対して十分なバッファーが存在するかどうかを取得します。
    //!
    //! @param[in] pTransformTree 対象のトランスフォームツリーリソースへのポインター。
    //!
    //! @return 十分なバッファーが存在する場合は true、それ以外の場合は false を返します。
    //!
    bool IsAcceptable(const ResTransformTree* pTransformTree) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES(pTransformTree != NULL, NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        return pTransformTree->GetTransformNodeCount() <= GetBindTable().GetSize();
    }

    //! @brief アニメーションリソースに対して十分なバッファーが存在するかどうかを取得します。
    //!
    //! @param[in] pRes 対象のトランスフォームツリーアニメーションリソースへのポインター。
    //!
    //! @return 十分なバッファーが存在する場合は true、それ以外の場合は false を返します。
    //!
    bool IsAcceptable(const ResTransformTreeAnim* pRes) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pRes);
        return pRes->GetTransformNodeAnimCount() <= m_MaxTransformNodeAnim;
    }

    //! @brief トランスフォームツリーアニメーション名を取得します。
    //!
    //! @return ResTransformTreeAnim のトランスフォームツリーアニメーション名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return m_pRes->GetName();
    }

    //! @brief アニメーション対象として関連付けられているトランスフォームツリーリソースを取得します。
    //!
    //! @return アニメーション対象として関連付けられているトランスフォームツリーリソースへのポインター。
    //!
    const ResTransformTree* GetResTransformTree() const NN_NOEXCEPT
    {
        return m_pResTransformTree;
    }

    //@}

protected:
    //! @briefprivate アニメーション計算の状態を表すフラグです。
    //!
    enum Flag
    {
        Flag_InvalidResult      = 0x1 << 4,
    };

    //! @briefprivate
    enum Mask
    {
        Mask_Result         = Flag_InvalidResult
    };

    //! @briefprivate トランスフォームノードアニメーションを取得します。
    //!
    const ResTransformNodeAnim* GetTransformNodeAnim(int animIndex) const NN_NOEXCEPT
    {
        return &m_pTransformNodeAnimArray[animIndex];
    }

    //! @briefprivate アニメーションの関連付けを行う内部関数です。
    //!
    BindResult BindImpl(const ResTransformTree* pTransformTree) NN_NOEXCEPT;

    //! @briefprivate クリア処理を行う内部関数です。
    //!
    void ClearImpl(const ResTransformTree* pTransformTree) NN_NOEXCEPT;

    //! @briefprivate アニメーションカーブの評価を行う内部関数です。
    //!
    void CalculateImpl() NN_NOEXCEPT;

    //! @briefprivate アニメーション結果を対象に適用する内部関数です。
    //!
    template <typename ConvRot>
    void ApplyToImpl(TransformTreeObj* pTransformTreeObj) const NN_NOEXCEPT;

private:
    class Impl;
    class InitializeArgument;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    const ResTransformTreeAnim* m_pRes;
    const ResTransformNodeAnim* m_pTransformNodeAnimArray;
    int32_t m_MaxTransformNodeAnim;
    Bit32 m_Flag;
    const nn::g3d::ResTransformTree* m_pResTransformTree;

    NN_DISALLOW_COPY(TransformTreeAnimObj);
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate TransformTreeAnimObj::Initialize() に渡して初期化を行うパラメータです。
class TransformTreeAnimObj::InitializeArgument
{
public:
    //! @brief メモリブロックのインデックスを示す列挙子。
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_ResultBuffer,
        MemoryBlockIndex_BindTable,
        MemoryBlockIndex_FrameCacheArray,
        MemoryBlockIndex_End
    };
    //! @brief コンストラクタです。
    //!
    //! @details
    //! インスタンスで使用するすべてのトランスフォームツリーアニメーションとすべてのトランスフォームツリーリソースに対して
    //! Reserve() するか、SetMaxTransformNodeCount()、SetMaxTransformNodeAnimCount()、SetMaxCurveCount() で
    //! バッファーサイズの計算に必要なパラメータを指定する必要があります。
    //!
    InitializeArgument() NN_NOEXCEPT
    {
        Clear();
    }

    //! @brief パラメータを初期化します。
    void Clear() NN_NOEXCEPT
    {
        m_TransformNodeCount = m_TransformNodeAnimCount = m_CurveCount = -1;
        m_ContextEnabled = true;
        m_ContextAvailable = false;
        m_WorkMemory.Invalidate();
        for (int blockIndex = 0; blockIndex < MemoryBlockIndex_End; ++blockIndex)
        {
            m_MemoryBlock[blockIndex].Initialize();
        }
    }

    //! @brief 対象となるトランスフォームツリーリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResTransformTree トランスフォームツリーリソースへのポインター。
    //!
    void Reserve(const ResTransformTree* pResTransformTree) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResTransformTree);
        m_TransformNodeCount = std::max(m_TransformNodeCount, pResTransformTree->GetTransformNodeCount());
    }

    //! @brief 対象となるトランスフォームツリーアニメーションリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResAnim トランスフォームツリーアニメーションリソースへのポインター。
    //!
    void Reserve(const ResTransformTreeAnim* pResAnim) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResAnim);
        m_TransformNodeAnimCount = std::max(m_TransformNodeAnimCount, pResAnim->GetTransformNodeAnimCount());
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

    //! @brief 使用できる最大のトランスフォームノード数を指定します。
    //!
    //! @param[in] transformNodeCount 使用できる最大のトランスフォームノード数。
    //!
    void SetMaxTransformNodeCount(int transformNodeCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(transformNodeCount >= 0);
        m_TransformNodeCount = transformNodeCount;
    }

    //! @brief 使用できる最大のトランスフォームノード数を取得します。
    //!
    //! @return 使用できる最大のトランスフォームノード数を返します。
    //!
    int GetMaxTransformNodeCount() const NN_NOEXCEPT
    {
        return m_TransformNodeCount;
    }

    //! @brief 使用できる最大のトランスフォームノードアニメーション数を指定します。
    //!
    //! @param[in] transformNodeAnimCount 使用できる最大のトランスフォームノードアニメーション数。
    //!
    void SetMaxTransformNodeAnimCount(int transformNodeAnimCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(transformNodeAnimCount >= 0);
        m_TransformNodeAnimCount = transformNodeAnimCount;
    }

    //! @brief 使用できる最大のトランスフォームノードアニメーション数を取得します。
    //!
    //! @return 使用できる最大のトランスフォームノードアニメーション数を返します。
    //!
    int GetMaxTransformNodeAnimCount() const NN_NOEXCEPT
    {
        return m_TransformNodeAnimCount;
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

    //! @brief TransformTreeAnimObj 構築に必要なメモリサイズを取得します。
    //!
    //! @return  TransformTreeAnimObj 構築に必要なメモリサイズを返します。
    //!
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief TransformTreeAnimObj 構築に必要なメモリサイズを計算します。
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
        return m_TransformNodeCount >= 0 && m_TransformNodeAnimCount >= 0 && m_CurveCount >= 0;
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
    int m_TransformNodeCount;
    int m_TransformNodeAnimCount;
    int m_CurveCount;
    bool m_ContextEnabled;
    bool m_ContextAvailable; // リソースがベイクされていないか、カーブ数を手動指定した場合に true
    nn::util::MemorySplitter m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
};

//--------------------------------------------------------------------------------------------------

//! @brief TransformTreeAnimObj の構築を行うクラスです。
class TransformTreeAnimObj::Builder : public TransformTreeAnimObj::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @details
    //! インスタンスで使用するすべてのトランスフォームツリーアニメーションリソースとすべてのトランスフォームツリーに対して
    //! Reserve() するか、SetMaxTransformNodeCount()、SetMaxTransformNodeAnimCount()、SetMaxCurveCount() で
    //! バッファーサイズの計算に必要なパラメータを指定する必要があります。
    //!
    Builder()
    {
    }

    //! @brief TransformTreeAnimObj を構築します。
    //!
    //! @param[in] pTransformTreeAnimObj TransformTreeAnimObj へのポインター
    //! @param[in] pBuffer バッファーへのポインター
    //! @param[in] bufferSize バッファーのサイズ
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ
    //!
    bool Build(TransformTreeAnimObj* pTransformTreeAnimObj, void* pBuffer, size_t bufferSize) const NN_NOEXCEPT
    {
        return pTransformTreeAnimObj->Initialize(*this, pBuffer, bufferSize);
    }
};

}}} // namespace nn::g3d::detail

