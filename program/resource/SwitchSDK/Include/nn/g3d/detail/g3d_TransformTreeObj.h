/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

#include <nn/gfx.h>
#include <nn/util/util_MemorySplitter.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/detail/g3d_ResTransformTree.h>
#include <nn/g3d/detail/g3d_Inlines.h>
#include <nn/g3d/detail/g3d_Flag.h>

namespace nn { namespace g3d { namespace detail {

//! @brief トランスフォームノードのローカル行列を表す構造体です。
struct TransformNodeLocalMtxData
{
    Bit32 flag; //!< フラグ。
    nn::util::Vector3fType   scale; //!< スケール値。
    nn::util::Matrix4x3fType mtxRT; //!< 回転、移動行列。
};

//! @brief トランスフォームノードのローカル行列を表すクラスです。
//!
//! flag が 0 の場合は scale と mtxRT をそのまま使用して計算されます。
//! さらに次のビットが立っている場合は、ライブラリーがそれぞれのビットに応じた
//! 特殊計算や最適化計算を行うことがあります。
//!
//! ResTransformNode::Flag_ScaleUniform           <br>
//! ResTransformNode::Flag_ScaleVolumeOne         <br>
//! ResTransformNode::Flag_RotateZero             <br>
//! ResTransformNode::Flag_TranslateZero          <br>
//!
class TransformNodeLocalMtx : public TransformNodeLocalMtxData
{
public:
    //! @brief コンストラクタです。
    TransformNodeLocalMtx() NN_NOEXCEPT : TransformNodeLocalMtxData()
    {
        flag = 0;
    }

    //! @brief 内容の編集後に呼び出す関数です。
    //!
    //! @details
    //! この関数を呼び出すことで最適化フラグがリセットされます。
    //!
    void EndEdit() NN_NOEXCEPT
    {
        flag &= ~ResTransformNode::Flag_Identity;
    }

    //! @brief LocalMtxData を LocalMtx に変換します。
    //!
    //! @param[in] data LocalMtxData への参照。
    //!
    //! @return LocalMtx への参照を返します。
    //!
    static TransformNodeLocalMtx& DownCast(TransformNodeLocalMtxData& data) NN_NOEXCEPT
    {
        return static_cast<TransformNodeLocalMtx&>(data);
    }

    //! @brief LocalMtxData を LocalMtx に変換します。
    //!
    //! @param[in] data LocalMtxData への参照。
    //!
    //! @return LocalMtx への参照を返します。
    //!
    static const TransformNodeLocalMtx& DownCast(const TransformNodeLocalMtxData& data) NN_NOEXCEPT
    {
        return static_cast<const TransformNodeLocalMtx&>(data);
    }

    //! @brief LocalMtxData を LocalMtx に変換します。
    //!
    //! @param[in] data LocalMtxData へのポインター。
    //!
    //! @return LocalMtxへのポインターを返します。
    //!
    static TransformNodeLocalMtx* DownCast(TransformNodeLocalMtxData* data) NN_NOEXCEPT
    {
        return static_cast<TransformNodeLocalMtx*>(data);
    }

    //! @brief LocalMtxData を LocalMtx に変換します。
    //!
    //! @param[in] data LocalMtxData へのポインター。
    //!
    //! @return LocalMtx へのポインターを返します。
    //!
    static const TransformNodeLocalMtx* DownCast(const TransformNodeLocalMtxData* data) NN_NOEXCEPT
    {
        return static_cast<const TransformNodeLocalMtx*>(data);
    }
};

//--------------------------------------------------------------------------------------------------

//! @brief トランスフォームノードインスタンスです。
class TransformNodeObj
{
public:
    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    //!
    TransformNodeObj() NN_NOEXCEPT
        : m_pRes(NULL)
        , m_Flag(0)
        , m_pUserPtr(NULL)
    {
    }

    //! @brief 初期化をします。
    //!
    //! @param[in]  pResTransformNode ResTransformNode へのポインター
    //!
    //! @return 初期化に成功した場合は true を返します。
    //!
    bool Initialize(const ResTransformNode* pResTransformNode) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResTransformNode);
        m_pRes = pResTransformNode;
        m_Flag = pResTransformNode->ToData().flag;
        return true;
    }

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief リソースを取得します。
    //!
    //! @return ResTransformNode へのポインターを返します。
    //!
    const ResTransformNode* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
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

    //@}

private:
    const ResTransformNode*     m_pRes;
    Bit32                       m_Flag;
    void*                       m_pUserPtr;

    NN_DISALLOW_COPY(TransformNodeObj);
};

//--------------------------------------------------------------------------------------------------

//! @brief トランスフォームツリーインスタンスです。
class TransformTreeObj
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
    //! @details
    //! 実際の構築処理はBuilderで行います。
    //!
    TransformTreeObj() NN_NOEXCEPT
        : m_pRes(NULL)
        , m_Flag(0)
        , m_pTransformNodeArray(NULL)
        , m_pLocalMtxArray(NULL)
        , m_pWorldMtxArray(NULL)
        , m_pTransformNodeVisibilityArray(NULL)
        , m_pBufferPtr(NULL)
    {
    }

    //----------------------------------------
    //! @name 更新
    //@{

    //! @brief ローカル行列をリソースに基づいて初期化します。
    //!
    //! @details
    //! 構築時で呼ばれるため、意図的に結果を初期化したい場合以外は呼ぶ必要はありません。
    //!
    void ClearLocalMtx() NN_NOEXCEPT;

    //! @brief ローカル行列からワールド行列を計算します。
    //!
    //! @param[in] baseMtx ベース行列への参照。
    //!
    void CalculateWorldMtx(const nn::util::Matrix4x3fType& baseMtx) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief リソースを取得します。
    //!
    //! @return ResTransformTree へのポインターを返します。
    //!
    const ResTransformTree* GetResource() const NN_NOEXCEPT
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

    //! @brief ローカル行列の配列を取得します。
    //!
    //! @return ローカル行列の配列のポインターを返します。
    //!
    TransformNodeLocalMtx* GetLocalMtxArray() NN_NOEXCEPT
    {
        return TransformNodeLocalMtx::DownCast(m_pLocalMtxArray);
    }

    //! @brief ローカル行列の配列を取得します。
    //!
    //! @return ローカル行列の配列のポインターを返します。
    //!
    const TransformNodeLocalMtx* GetLocalMtxArray() const NN_NOEXCEPT
    {
        return TransformNodeLocalMtx::DownCast(m_pLocalMtxArray);
    }

    //! @brief ワールド行列の配列を取得します。
    //!
    //! @return ワールド行列の配列のポインターを返します。
    //!
    nn::util::Matrix4x3fType* GetWorldMtxArray() NN_NOEXCEPT
    {
        return m_pWorldMtxArray;
    }

    //! @brief ワールド行列の配列を取得します。
    //!
    //! @return ローカル行列の配列のポインターを返します。
    //!
    const nn::util::Matrix4x3fType* GetWorldMtxArray() const NN_NOEXCEPT
    {
        return m_pWorldMtxArray;
    }

    //@}

    //----------------------------------------
    //! @name トランスフォームノード
    //@{

    //! @brief トランスフォームノードの数を取得します。
    //!
    //! @return トランスフォームノードの数を返します。
    //!
    int GetTransformNodeCount() const NN_NOEXCEPT
    {
        return m_pRes->GetTransformNodeCount();
    }

    //! @brief インデックス引きでトランスフォームノードを取得します。
    //!
    //! @param[in] elemIndex トランスフォームノードのインデックス。
    //!
    //! @return TransformNodeObj へのポインターを返します。
    //!
    //! @pre
    //! インデックスはモデルが持つトランスフォームノード数の範囲内。
    //!
    TransformNodeObj* GetTransformNode(int elemIndex) NN_NOEXCEPT
    {
        return &m_pTransformNodeArray[elemIndex];
    }
    //! @brief インデックス引きでトランスフォームノードを取得します。
    //!
    //! @param[in] elemIndex トランスフォームノードのインデックス。
    //!
    //! @return TransformNodeObj へのポインターを返します。
    //!
    //! @pre
    //! インデックスはモデルが持つトランスフォームノード数の範囲内。
    //!
    const TransformNodeObj* GetTransformNode(int elemIndex) const NN_NOEXCEPT
    {
        return &m_pTransformNodeArray[elemIndex];
    }

    //! @brief 指定した名前のトランスフォームノードのインデックスを取得します。
    //!
    //! @param[in] transformNodeName トランスフォームノード名
    //!
    //! @return トランスフォームノードのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のトランスフォームノードが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindTransformNodeIndex(const char* transformNodeName) const NN_NOEXCEPT
    {
        return m_pRes->FindTransformNodeIndex(transformNodeName);
    }

    //! @brief 指定した名前のトランスフォームノードを取得します。
    //!
    //! @param[in] transformNodeName トランスフォームノード名
    //!
    //! @return TransformNodeObj へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のトランスフォームノードが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    TransformNodeObj* FindTransformNode(const char* transformNodeName) NN_NOEXCEPT
    {
        int index = m_pRes->FindTransformNodeIndex(transformNodeName);
        return &m_pTransformNodeArray[index];
    }
    //! @brief 指定した名前のトランスフォームノードを取得します。
    //!
    //! @param[in] transformNodeName トランスフォームノード名
    //!
    //! @return TransformNodeObj へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のトランスフォームノードが無い場合は NULL を返します。
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const TransformNodeObj* FindTransformNode(const char* transformNodeName) const NN_NOEXCEPT
    {
        int index = m_pRes->FindTransformNodeIndex(transformNodeName);
        return &m_pTransformNodeArray[index];
    }

    //@}

    //----------------------------------------
    //! @name バインド情報
    //@{

    //! @brief 指定したオブジェクトID がバインドされているトランスフォームノードを取得します。
    //!
    //! @param[in] objectId オブジェクトID
    //!
    //! @return TransformNodeObj へのポインターを返します。
    //!
    //! @details
    //! 指定したオブジェクトID がバインドされたトランスフォームノードが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    TransformNodeObj* FindTransformNodeFromObjectId(const char* objectId) NN_NOEXCEPT
    {
        int index = m_pRes->FindTransformNodeIndexFromObjectId(objectId);
        return &m_pTransformNodeArray[index];
    }
    //! @brief 指定したオブジェクトID がバインドされているトランスフォームノードを取得します。
    //!
    //! @param[in] objectId オブジェクトID
    //!
    //! @return TransformNodeObj へのポインターを返します。
    //!
    //! @details
    //! 指定したオブジェクトID がバインドされたトランスフォームノードが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const TransformNodeObj* FindTransformNodeFromObjectId(const char* objectId) const NN_NOEXCEPT
    {
        int index = m_pRes->FindTransformNodeIndexFromObjectId(objectId);
        return &m_pTransformNodeArray[index];
    }
    //! @brief 指定したオブジェクトID がバインドされているトランスフォームノードのインデックスを取得します。
    //!
    //! @param[in] objectId オブジェクトID
    //!
    //! @return トランスフォームノードのインデックスを返します。
    //!
    //! @details
    //! 指定したオブジェクトID がバインドされたトランスフォームノードが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindTransformNodeIndexFromObjectId(const char* objectId) const NN_NOEXCEPT
    {
        return m_pRes->FindTransformNodeIndexFromObjectId(objectId);
    }

    //@}

    //----------------------------------------
    //! @name ビジビリティー
    //@{

    //! @brief 指定したトランスフォームノードのビジビリティーを取得します。
    //!
    //! @param[in] transformNodeIndex トランスフォームノードのインデックス。
    //!
    //! @return 表示状態の場合 true、非表示状態の場合 false を返します。
    //!
    bool IsTransformNodeVisible(int transformNodeIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(transformNodeIndex, 0, GetTransformNodeCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        return IsBitOn<bool>(m_pTransformNodeVisibilityArray, transformNodeIndex);
    }

    //! @brief 指定したトランスフォームノードのビジビリティーを設定します。
    //!
    //! @param[in] transformNodeIndex トランスフォームノードのインデックス。
    //! @param[in] visible 表示する場合は true, 非表示の場合は false。
    //!
    void SetTransformNodeVisible(int transformNodeIndex, bool visible) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(transformNodeIndex, 0, GetTransformNodeCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        SetBit(m_pTransformNodeVisibilityArray, transformNodeIndex, visible ? 1 : 0);
    }

    //! @brief トランスフォームノードのビジビリティーをリソースに基づいて初期化します。
    //!
    //! Builder で Build() 時に呼ばれるため、意図的に結果を初期化したい場合以外は呼ぶ必要はありません。
    //!
    void ClearTransformNodeVisible() NN_NOEXCEPT;

    //@}

private:
    class InitializeArgument;

    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    const ResTransformTree*     m_pRes;
    Bit16                       m_Flag;

    TransformNodeObj*           m_pTransformNodeArray;
    TransformNodeLocalMtxData*  m_pLocalMtxArray;
    nn::util::Matrix4x3fType*   m_pWorldMtxArray;
    Bit32*                      m_pTransformNodeVisibilityArray;

    void* m_pBufferPtr;

    NN_DISALLOW_COPY(TransformTreeObj);
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate TransformTreeObj::Initialize() に渡して初期化を行うパラメータです。
class TransformTreeObj::InitializeArgument
{
public:
    //! @brief メモリブロックのインデックスを示す列挙子。
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_TransformNodeArray,
        MemoryBlockIndex_WorldMtx,
        MemoryBlockIndex_LocalMtx,
        MemoryBlockIndex_TransformNodeVisArray,
        MemoryBlockIndex_End
    };

    //! @brief コンストラクタです。
    //!
    //! @param[in] resource ResTransformTree へのポインター。
    //!
    explicit InitializeArgument(const ResTransformTree* resource) NN_NOEXCEPT
        : m_pRes(resource)
        , m_MemoryBlock()
    {
        NN_SDK_REQUIRES_NOT_NULL(resource);
        m_WorkMemory.Invalidate();
    }

    //! @brief リソースを取得します。
    //!
    //! @return ResTransformTree へのポインターを返します。
    //!
    const ResTransformTree* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    }

    //! @brief TransformTreeObj構築に必要なメモリサイズを取得します。
    //!
    //! @return TransformTreeObj 構築に必要なメモリサイズを返します。
    //!
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief TransformTreeObj 構築に必要なメモリサイズを計算します。
    //!
    //! @pre
    //! - 設定されたユニフォームブロックのバッファーリング数が１以上。
    //!
    void CalculateMemorySize() NN_NOEXCEPT;

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    void* GetBuffer(void* pHead, int index) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return m_MemoryBlock[index].Get(pHead);
    }

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    template< typename T >
    T* GetBuffer(void* pHead, int index) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return static_cast<T*>(GetBuffer(pHead, index));
    }

    //! @brief TransformTreeObj構築に必要なメモリサイズが計算済みかを取得します。
    bool IsMemoryCalculated() const NN_NOEXCEPT
    {
        return m_WorkMemory.IsValid();
    }

private:
    const ResTransformTree* m_pRes;
    nn::util::MemorySplitter m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
};

//! @brief TransformTreeObj を構築するクラスです。
class TransformTreeObj::Builder : public TransformTreeObj::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @param[in] resource ResTransformTree へのポインター
    //!
    explicit Builder(const ResTransformTree* resource) NN_NOEXCEPT
        : InitializeArgument(resource)
    {
        NN_SDK_ASSERT_NOT_NULL(resource);
    }

    //! @brief TransformTreeObj を構築します。
    //!
    //! @param[in] pTransformTreeObj TransformTreeObj へのポインター。
    //! @param[in] pBuffer バッファーへのポインター。
    //! @param[in] bufferSize バッファーのサイズ。
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである。
    //! - pBuffer は Alignment_Buffer のアライメント上にある。
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ。
    //!
    bool Build(TransformTreeObj* pTransformTreeObj, void* pBuffer, size_t bufferSize) NN_NOEXCEPT
    {
        return pTransformTreeObj->Initialize(*this, pBuffer, bufferSize);
    }
};

}}} // namespace nn::g3d::detail
