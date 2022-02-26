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
* @brief トランスフォームツリービジビリティーアニメーションに関するインスタンスクラスの宣言
*/

#pragma once

#include <nn/util/util_MemorySplitter.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/detail/g3d_GeneralAnimObj.h>
#include <nn/g3d/detail/g3d_TransformTreeObj.h>
#include <nn/g3d/detail/g3d_ResTransformTreeVisibilityAnim.h>

namespace nn { namespace g3d { namespace detail {

//! @brief トランスフォームツリービジビリティーアニメーションインスタンスです。
class TransformTreeVisibilityAnimObj : public GeneralAnimObj
{
public:
    class Builder;

    //! @brief インスタンスの構築時に渡すバッファーの必要アライメントサイズです。
    enum Alignment
    {
        Alignment_Buffer = 8 //!< @brief Builder に渡すバッファーの必要アライメントサイズです。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    //!
    //! 実際の構築処理は Builder で行います。
    //!
    TransformTreeVisibilityAnimObj() NN_NOEXCEPT
        : GeneralAnimObj()
        , m_pRes(NULL)
        , m_MaxTransformNodeAnim(0)
        , m_CurveCount(0)
        , m_pCurveArray(NULL)
    {
    }

    //@}

    //----------------------------------------
    //! @name 関連付け
    //@{

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pTransformTree ResTransformTree へのポインター。
    //!
    //! @return BindResult を返します。
    //!
    virtual BindResult Bind(const ResTransformTree* pTransformTree) NN_NOEXCEPT;

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pTransformTree アニメーション対象の ResTransformTree へのポインター。
    //!
    //! @pre
    //! - pTransformTree と設定されている ResTransformTreeVisibilityAnim のリソースが ResTransformTreeVisibilityAnim::PreBind によって事前に関連付けられている。
    //!
    //! @details
    //! リソース同士を ResTransformTreeVisibilityAnim::PreBind によって事前に関連付けることにより、
    //! BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    virtual void BindFast(const ResTransformTree* pTransformTree) NN_NOEXCEPT;

    //! @brief 指定したインデックスのトランスフォームノードにアニメーションフラグを設定します。
    //!
    //! @param[in] targetIndex トランスフォームノードのインデックス。
    //! @param[in] flag 設定するフラグ。
    //!
    void SetBindFlag(int targetIndex, BindFlag flag) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        SetBindFlagImpl(targetIndex, flag);
    }

    //! @brief 指定したインデックスのトランスフォームノードにアニメーションフラグを取得します。
    //!
    //! @param[in] targetIndex トランスフォームノードのインデックス。
    //!
    //! @return フラグを返します。
    //!
    BindFlag GetBindFlag(int targetIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(m_pRes);
        return GetBindFlagImpl(targetIndex);
    }

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    virtual void ClearResult() NN_NOEXCEPT;
    virtual void Calculate() NN_NOEXCEPT;
    virtual void ApplyTo(TransformTreeObj* pTransformTreeObj) const NN_NOEXCEPT;

    //! @brief 自身のトランスフォームツリービジビリティーアニメーションが書き換えるビジビリティをトランスフォームツリーリソースの値に戻します。
    //!
    //! @param[in] pTransformTreeObj ビジビリティを戻すモデルインスタンス。
    //!
    //! @pre
    //! - m_pRes != NULL
    //! - IsTargetBound()
    //! - pTransformTreeObj != NULL
    //!
    //! @details
    //! - トランスフォームノードビジビリティをトランスフォームツリービジビリティーアニメーションで変更を加えていた場合、
    //! この関数を呼び出すとトランスフォームノードビジビリティがトランスフォームノードリソースの値に上書きされてしまうので注意してください。
    //! - SetBindFlag() で Apply が無効化されているトランスフォームノードは処理がスキップされます。
    //!
    void RevertTo(TransformTreeObj* pTransformTreeObj) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief リソースを取得します。
    //!
    //! @return トランスフォームツリービジビリティーアニメーションリソースへのポインターを返します。
    //!
    const ResTransformTreeVisibilityAnim* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    }

    //! @brief リソースを設定します。
    //!
    //! @param[in] pRes トランスフォームツリービジビリティーアニメーションリソースへのポインター。
    //!
    //! @details
    //! 計算するトランスフォームツリービジビリティーアニメーションを差し替えます。
    //!
    void SetResource(const ResTransformTreeVisibilityAnim* pRes) NN_NOEXCEPT;

    //! @brief リソースを取り外します。
    void ResetResource() NN_NOEXCEPT
    {
        m_pRes = NULL;
        SetTargetUnbound();
    }

    //! @brief アニメーション対象に対して十分なバッファーが存在するかどうかを取得します。
    //!
    //! @param[in] pTransformTree 対象のモデルリソースへのポインター。
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
    //! @param[in] pRes 対象のトランスフォームツリービジビリティーアニメーションリソースへのポインター。
    //!
    //! @return 十分なバッファーが存在する場合は true、それ以外の場合は false を返します。
    //!
    bool IsAcceptable(const ResTransformTreeVisibilityAnim* pRes) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pRes);
        return pRes->GetAnimCount() <= m_MaxTransformNodeAnim;
    }

    //! @brief トランスフォームツリービジビリティーアニメーション名を取得します。
    //!
    //! @return ResTransformTreeVisibilityAnim のトランスフォームツリービジビリティーアニメーション名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return m_pRes->GetName();
    }

    //@}

protected:
    //! @briefprivate カーブを取得します。
    //!
    //!
    const ResAnimCurve* GetCurve(int curveIndex) const NN_NOEXCEPT
    {
        return &m_pCurveArray[curveIndex];
    }

private:
    class InitializeArgument;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    const ResTransformTreeVisibilityAnim* m_pRes;
    uint16_t m_MaxTransformNodeAnim;
    int32_t m_CurveCount;
    const ResAnimCurve* m_pCurveArray;

    NN_DISALLOW_COPY(TransformTreeVisibilityAnimObj);
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate TransformTreeVisibilityAnimObj::Initialize() に渡して初期化を行うパラメータです。
class TransformTreeVisibilityAnimObj::InitializeArgument
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
    //! インスタンスで使用するすべてのトランスフォームツリービジビリティーアニメーションリソースに対して
    //! Reserve() するか、SetMaxTransformNodeCount()、SetMaxMatCount()、SetMaxTransformNodeAnimCount()、
    //! SetMaxMatAnimCount(), SetMaxCurveCount() でバッファーサイズの計算に必要な
    //! パラメータを指定する必要があります。
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

    //! @brief 対象となるモデルリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResTransformTree モデルリソースへのポインター。
    //!
    void Reserve(const ResTransformTree* pResTransformTree) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResTransformTree);
        m_TransformNodeCount = std::max(m_TransformNodeCount, pResTransformTree->GetTransformNodeCount());
    }

    //! @brief 対象となるトランスフォームツリービジビリティーアニメーションリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResAnim トランスフォームツリービジビリティーアニメーションリソースへのポインター。
    //!
    void Reserve(const ResTransformTreeVisibilityAnim* pResAnim) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResAnim);
        m_TransformNodeAnimCount = std::max(m_TransformNodeAnimCount, pResAnim->GetAnimCount());
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
    //
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

    //! @brief 必要なパラメータが設定されているかどうかを取得します。
    //!
    //! @return 必要なパラメータが設定されている場合は true、設定されていない場合は false を返します。
    //!
    bool IsValid() const NN_NOEXCEPT
    {
        return (m_TransformNodeCount >= 0 && m_TransformNodeAnimCount >= 0) && m_CurveCount >= 0;
    }

    //! @brief TransformTreeVisibilityAnimObj 構築に必要なメモリサイズを取得します。
    //!
    //! @return  TransformTreeVisibilityAnimObj構築に必要なメモリサイズを返します。
    //!
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief TransformTreeVisibilityAnimObj 構築に必要なメモリサイズを計算します。
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

    //! @brief TransformTreeVisibilityAnimObj 構築に必要なメモリサイズが計算済みかを取得します。
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


//! @brief TransformTreeVisibilityAnimObj の構築を行うクラスです。
class TransformTreeVisibilityAnimObj::Builder : public TransformTreeVisibilityAnimObj::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @details
    //! インスタンスで使用するすべてのトランスフォームツリービジビリティーアニメーションリソースに対して
    //! Reserve() するか、SetMaxTransformNodeCount()、SetMaxMatCount()、SetMaxTransformNodeAnimCount()、
    //! SetMaxMatAnimCount(), SetMaxCurveCount() でバッファーサイズの計算に必要な
    //! パラメータを指定する必要があります。
    //!
    Builder()
    {
    }

    //! @brief TransformTreeVisibilityAnimObj を構築します。
    //!
    //! @param[in] pTransformTreeVisibilityAnimObj TransformTreeVisibilityAnimObjへのポインター
    //! @param[in] pBuffer バッファーへのポインター
    //! @param[in] bufferSize バッファーのサイズ
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ
    //!
    bool Build(TransformTreeVisibilityAnimObj* pTransformTreeVisibilityAnimObj, void* pBuffer, size_t bufferSize) const NN_NOEXCEPT
    {
        return pTransformTreeVisibilityAnimObj->Initialize(*this, pBuffer, bufferSize);
    }
};

}}} // namespace nn::g3d::detail

