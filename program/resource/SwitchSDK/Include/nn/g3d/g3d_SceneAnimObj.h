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
* @brief シーンアニメーションに関するインスタンスクラスの宣言
*/

#pragma once

#include <nn/util/util_MemorySplitter.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_AnimObj.h>
#include <nn/g3d/g3d_ResSceneAnim.h>
#include <nn/g3d/detail/g3d_MatrixUtility.h>

namespace nn { namespace g3d {

//! @brief シーンアニメーションインスタンスの基底クラスです。
class SceneAnimObj : public AnimObj
{
public:
    //----------------------------------------
    //! @name 取得/設定
    //@{

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

    //@}
protected:
    //! @briefprivate
    SceneAnimObj() NN_NOEXCEPT : AnimObj(), m_pUserPtr(NULL)
    {
    }

private:
    void* m_pUserPtr;

    NN_DISALLOW_COPY(SceneAnimObj);
};

//--------------------------------------------------------------------------------------------------

//! @brief カメラアニメーションインスタンスです。
class CameraAnimObj : public SceneAnimObj
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
    CameraAnimObj() NN_NOEXCEPT : SceneAnimObj(), m_pRes(NULL)
    {
    }

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    virtual void ClearResult() NN_NOEXCEPT;
    virtual void Calculate() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief リソースを取得します。
    //!
    //! @return カメラアニメーションリソースへのポインターを返します。
    //!
    const ResCameraAnim* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    };

    //! @brief リソースを設定します。
    //!
    //! @param[in] pRes カメラアニメーションリソースへのポインター。
    //!
    //! @details
    //! 計算するカメラアニメーションを差し替えます。
    //!
    void SetResource(const ResCameraAnim* pRes) NN_NOEXCEPT;

    //! @brief リソースを取り外します。
    void ResetResource() NN_NOEXCEPT
    {
        m_pRes = NULL;
    }

    //! @brief アニメーションの計算結果を取得します。
    //!
    //! @return アニメーションの計算結果を格納した CameraAnimResult 配列へのポインターを返します。
    //!
    CameraAnimResult* GetResult() NN_NOEXCEPT
    {
        return static_cast<CameraAnimResult*>(GetResultBuffer());
    }

    //! @brief アニメーションの計算結果を取得します。
    //!
    //! @return アニメーションの計算結果を格納した CameraAnimResul t配列へのポインターを返します。
    //!
    const CameraAnimResult* GetResult() const NN_NOEXCEPT
    {
        return static_cast<const CameraAnimResult*>(GetResultBuffer());
    }

    //! @brief 回転の計算方式を取得します。
    //!
    //! @return 回転の計算方式を返します。
    //!
    Bit32 GetRotateMode() const NN_NOEXCEPT
    {
        return m_pRes->GetRotateMode();
    }

    //! @brief プロジェクションの計算方式を取得します。
    //!
    //! @return プロジェクションの計算方式を返します。
    //!
    Bit32 GetProjectionMode() const NN_NOEXCEPT
    {
        return m_pRes->GetProjectionMode();
    }

    //! @brief カメラアニメーション名を取得します。
    //!
    //! @return ResCameraAnim のカメラアニメーション名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return m_pRes->GetName();
    }

    //@}

private:
    class InitializeArgument;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    const ResCameraAnim* m_pRes;

    NN_DISALLOW_COPY(CameraAnimObj);
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate CameraAnimObj::Initialize() に渡して初期化を行うパラメータです。
class CameraAnimObj::InitializeArgument
{
public:
    //! @brief メモリブロックのインデックスを示す列挙子。
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_ResultBuffer,
        MemoryBlockIndex_FrameCacheArray,
        MemoryBlockIndex_End
    };

    //! @brief コンストラクタです。
    //!
    //! インスタンスで使用するすべてのカメラアニメーションリソースに対して Reserve() するか、
    //! SetMaxCurveCount() でバッファーサイズの計算に必要なパラメータを指定する必要があります。
    //!
    InitializeArgument() NN_NOEXCEPT
    {
        Clear();
    }

    //! @brief パラメータを初期化します。
    void Clear() NN_NOEXCEPT
    {
        m_CurveCount = -1;
        m_ContextEnabled = true;
        m_ContextAvailable = false;
        m_WorkMemory.Invalidate();
        for (int blockIndex = 0; blockIndex < MemoryBlockIndex_End; ++blockIndex)
        {
            m_MemoryBlock[blockIndex].Initialize();
        }
    }

    //! @brief 対象となるカメラアニメーションリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResAnim カメラアニメーションリソースへのポインター。
    //!
    void Reserve(const ResCameraAnim* pResAnim) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResAnim);
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
        return m_CurveCount >= 0;
    }

    //! @brief CameraAnimObj 構築に必要なメモリサイズを取得します。
    //!
    //! @return  CameraAnimObj 構築に必要なメモリサイズを返します。
    //!
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief CameraAnimObj 構築に必要なメモリサイズを計算します。
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
        NN_SDK_REQUIRES(index, 0, MemoryBlockIndex_End);
        return static_cast<T*>( GetBuffer(pHead, index) );
    }

    //! @brief CameraAnimObj 構築に必要なメモリサイズが計算済みかを取得します。
    //!
    //! @return 計算済みの場合は true、未計算の場合は false を返します。
    //!
    bool IsMemoryCalculated() const NN_NOEXCEPT
    {
        return m_WorkMemory.IsValid();
    }

private:
    int m_CurveCount;
    bool m_ContextEnabled;
    bool m_ContextAvailable; // リソースがベイクされていないか、カーブ数を手動指定した場合に true
    nn::util::MemorySplitter m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
};

//! @brief CameraAnimObj の構築を行うクラスです。
class CameraAnimObj::Builder : public CameraAnimObj::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @details
    //! インスタンスで使用するすべてのカメラアニメーションリソースに対して Reserve() するか、
    //! SetMaxCurveCount() でバッファーサイズの計算に必要なパラメータを指定する必要があります。
    //!
    Builder()
    {
    }

    //! @brief CameraAnimObj を構築します。
    //!
    //! @param[in] pCameraAnimObj CameraAnimObj へのポインター
    //! @param[in] pBuffer バッファーへのポインター
    //! @param[in] bufferSize バッファーのサイズ
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ
    //!
    bool Build(CameraAnimObj* pCameraAnimObj, void* pBuffer, size_t bufferSize) const NN_NOEXCEPT
    {
        return pCameraAnimObj->Initialize(*this, pBuffer, bufferSize);
    }
};

//--------------------------------------------------------------------------------------------------

//! @brief ライトアニメーションインスタンスです。
class LightAnimObj : public SceneAnimObj
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
    LightAnimObj() NN_NOEXCEPT : SceneAnimObj(), m_pRes(NULL)
    {
    }

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    virtual void ClearResult() NN_NOEXCEPT;
    virtual void Calculate() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief リソースを取得します。
    //!
    //! @return ライトアニメーションリソースへのポインターを返します。
    //!
    const ResLightAnim* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    };

    //! @brief リソースを設定します。
    //!
    //! @param[in] pRes ライトアニメーションリソースへのポインター。
    //!
    //! @details
    //! 計算するライトアニメーションを差し替えます。
    //!
    void SetResource(const ResLightAnim* pRes) NN_NOEXCEPT;

    //! @brief リソースを取り外します。
    void ResetResource() NN_NOEXCEPT
    {
        m_pRes = NULL;
    }

    //! @brief アニメーションの計算結果を取得します。
    //!
    //! @return アニメーションの計算結果を格納した LightAnimResult 配列へのポインターを返します。
    //!
    LightAnimResult* GetResult() NN_NOEXCEPT
    {
        return static_cast<LightAnimResult*>(GetResultBuffer());
    }

    //! @brief アニメーションの計算結果を取得します。
    //!
    //! @return アニメーションの計算結果を格納した LightAnimResult 配列へのポインターを返します。
    //!
    const LightAnimResult* GetResult() const NN_NOEXCEPT
    {
        return static_cast<const LightAnimResult*>(GetResultBuffer());
    }

    //! @brief リソースに関連付けたライトタイプのインデックスを取得します。
    //!
    //! @return リソースに関連付けたライトタイプのインデックスを返します。
    //!
    int GetLightTypeIndex() const NN_NOEXCEPT
    {
        return m_pRes->GetLightTypeIndex();
    }

    //! @brief リソースに関連付けた距離減衰関数のインデックスを取得します。
    //!
    //! @return リソースに関連付けた距離減衰関数のインデックスを返します。
    //!
    int GetDistAttnFuncIndex() const NN_NOEXCEPT
    {
        return m_pRes->GetDistAttnFuncIndex();
    }

    //! @brief リソースに関連付けた角度減衰関数のインデックスを取得します。
    //!
    //! @return リソースに関連付けた角度減衰関数のインデックスを返します。
    //!
    int GetAngleAttnFuncIndex() const NN_NOEXCEPT
    {
        return m_pRes->GetAngleAttnFuncIndex();
    }

    //! @brief ライトアニメーション名を取得します。
    //!
    //! @return ResLightAnim のライトアニメーション名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return m_pRes->GetName();
    }

    //@}

private:
    class InitializeArgument;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize);

    const ResLightAnim* m_pRes;

    NN_DISALLOW_COPY(LightAnimObj);
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate LightAnimObj::Initialize() に渡して初期化を行うパラメータです。
class LightAnimObj::InitializeArgument
{
public:
    //! @brief メモリブロックのインデックスを示す列挙子。
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_ResultBuffer,
        MemoryBlockIndex_FrameCacheArray,
        MemoryBlockIndex_End
    };

    //! @brief コンストラクタです。
    //!
    //! インスタンスで使用するすべてのライトアニメーションリソースに対して Reserve() するか、
    //! SetMaxCurveCount() でバッファーサイズの計算に必要なパラメータを指定する必要があります。
    //!
    InitializeArgument() NN_NOEXCEPT
    {
        Clear();
    }

    //! @brief パラメータを初期化します。
    void Clear() NN_NOEXCEPT
    {
        m_CurveCount = -1;
        m_ContextEnabled = true;
        m_ContextAvailable = false;
        m_WorkMemory.Invalidate();
        for (int blockIndex = 0; blockIndex < MemoryBlockIndex_End; ++blockIndex)
        {
            m_MemoryBlock[blockIndex].Initialize();
        }
    }

    //! @brief 対象となるライトアニメーションリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResAnim ライトアニメーションリソースへのポインター。
    //!
    void Reserve(const ResLightAnim* pResAnim) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResAnim);
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
        return m_CurveCount >= 0;
    }

    //! @brief LightAnimObj 構築に必要なメモリサイズを取得します。
    //!
    //! @return  LightAnimObj 構築に必要なメモリサイズを返します。
    //!
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief LightAnimObj 構築に必要なメモリサイズを計算します。
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

    //! @brief LightAnimObj 構築に必要なメモリサイズが計算済みかを取得します。
    //!
    //! @return 計算済みの場合は true、未計算の場合は false を返します。
    //!
    bool IsMemoryCalculated() const NN_NOEXCEPT
    {
        return m_WorkMemory.IsValid();
    }

private:
    int m_CurveCount;
    bool m_ContextEnabled;
    bool m_ContextAvailable; // リソースがベイクされていないか、カーブ数を手動指定した場合に true
    nn::util::MemorySplitter m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
};

//! @brief LightAnimObj の構築を行うクラスです。
class LightAnimObj::Builder : public LightAnimObj::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @details
    //! インスタンスで使用するすべてのライトアニメーションリソースに対して Reserve() するか、
    //! SetMaxCurveCount() でバッファーサイズの計算に必要なパラメータを指定する必要があります。
    //!
    Builder()
    {
    }

    //! @brief LightAnimObjを構築します。
    //!
    //! @param[in] pLightAnimObj LightAnimObj へのポインター
    //! @param[in] pBuffer バッファーへのポインター
    //! @param[in] bufferSize バッファーのサイズ
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ
    //!
    bool Build(LightAnimObj* pLightAnimObj, void* pBuffer, size_t bufferSize) const NN_NOEXCEPT
    {
        return pLightAnimObj->Initialize(*this, pBuffer, bufferSize);
    }
};

//--------------------------------------------------------------------------------------------------

//! @brief フォグアニメーションインスタンスです。
class FogAnimObj : public SceneAnimObj
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
    FogAnimObj() NN_NOEXCEPT : SceneAnimObj(), m_pRes(NULL)
    {
    }

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    virtual void ClearResult() NN_NOEXCEPT;
    virtual void Calculate() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief リソースを取得します。
    //!
    //! @return フォグアニメーションリソースへのポインターを返します。
    //!
    const ResFogAnim* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    };

    //! @brief リソースを設定します。
    //!
    //! @param[in] pRes フォグアニメーションリソースへのポインター。
    //!
    //! @details
    //! 計算するフォグアニメーションを差し替えます。
    //!
    void SetResource(const ResFogAnim* pRes) NN_NOEXCEPT;

    //! @brief リソースを取り外します。
    void ResetResource() NN_NOEXCEPT
    {
        m_pRes = NULL;
    }

    //! @brief アニメーションの計算結果を取得します。
    //!
    //! @return アニメーションの計算結果を格納した FogAnimResult 配列へのポインターを返します。
    //!
    FogAnimResult* GetResult() NN_NOEXCEPT
    {
        return static_cast<FogAnimResult*>(GetResultBuffer());
    }

    //! @brief アニメーションの計算結果を取得します。
    //!
    //! @return アニメーションの計算結果を格納した FogAnimResult 配列へのポインターを返します。
    //!
    const FogAnimResult* GetResult() const NN_NOEXCEPT
    {
        return static_cast<const FogAnimResult*>(GetResultBuffer());
    }

    //! @brief リソースに関連付けた距離減衰関数のインデックスを取得します。
    //!
    //! @return リソースに関連付けた距離減衰関数のインデックスを返します。
    //!
    int GetDistAttnFuncIndex() const NN_NOEXCEPT
    {
        return m_pRes->GetDistAttnFuncIndex();
    }

    //! @brief フォグアニメーション名を取得します。
    //!
    //! @return ResFogAnim のフォグアニメーション名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return m_pRes->GetName();
    }

    //@}

private:
    class InitializeArgument;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    const ResFogAnim* m_pRes;

    NN_DISALLOW_COPY(FogAnimObj);
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate FogAnimObj::Initialize() に渡して初期化を行うパラメータです。
class FogAnimObj::InitializeArgument
{
public:
    //! @brief メモリブロックのインデックスを示す列挙子。
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_ResultBuffer,
        MemoryBlockIndex_FrameCacheArray,
        MemoryBlockIndex_End
    };

    //! @brief コンストラクタです。
    //!
    //! インスタンスで使用するすべてのフォグアニメーションリソースに対して Reserve() するか、
    //! SetMaxCurveCount() でバッファーサイズの計算に必要なパラメータを指定する必要があります。
    //!
    InitializeArgument() NN_NOEXCEPT
    {
        Clear();
    }

    //! @brief パラメータを初期化します。
    void Clear() NN_NOEXCEPT
    {
        m_CurveCount = -1;
        m_ContextEnabled = true;
        m_ContextAvailable = false;
        m_WorkMemory.Invalidate();
        for (int blockIndex = 0; blockIndex < MemoryBlockIndex_End; ++blockIndex)
        {
            m_MemoryBlock[blockIndex].Initialize();
        }
    }

    //! @brief 対象となるフォグアニメーションリソースを使用するのに必要なパラメータを設定します。
    //!
    //! @param[in] pResAnim フォグアニメーションリソースへのポインター。
    //!
    void Reserve(const ResFogAnim* pResAnim) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL(pResAnim);
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
        return m_CurveCount >= 0;
    }

    //! @brief FogAnimObj 構築に必要なメモリサイズを取得します。
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief FogAnimObj 構築に必要なメモリサイズを計算します。
    //!
    //! @return  FogAnimObj構築に必要なメモリサイズを返します。
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

    //! @brief FogAnimObj 構築に必要なメモリサイズが計算済みかを取得します。
    //!
    //! @return 計算済みの場合は true、未計算の場合は false を返します。
    //!
    bool IsMemoryCalculated() const NN_NOEXCEPT
    {
        return m_WorkMemory.IsValid();
    }

private:
    int m_CurveCount;
    bool m_ContextEnabled;
    bool m_ContextAvailable; // リソースがベイクされていないか、カーブ数を手動指定した場合に true
    nn::util::MemorySplitter m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
};

//! @brief FogAnimObj の構築を行うクラスです。
class FogAnimObj::Builder : public FogAnimObj::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! インスタンスで使用するすべてのフォグアニメーションリソースに対して Reserve() するか、
    //! SetMaxCurveCount() でバッファーサイズの計算に必要なパラメータを指定する必要があります。
    //!
    Builder()
    {
    }

    //! @brief FogAnimObj を構築します。
    //!
    //! @param[in] pFogAnimObj FogAnimObj へのポインター
    //! @param[in] pBuffer バッファーへのポインター
    //! @param[in] bufferSize バッファーのサイズ
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ
    //!
    bool Build(FogAnimObj* pFogAnimObj, void* pBuffer, size_t bufferSize) const NN_NOEXCEPT
    {
        return pFogAnimObj->Initialize(*this, pBuffer, bufferSize);
    }
};

//--------------------------------------------------------------------------------------------------

//! @brief シーンアニメーションの計算補助用関数群です。
class SceneAnimHelper
{
public:

    //! @brief CameraAnimResult から AimCamera のビュー行列を計算します。
    //!
    //! @param[out] pMtx nn::util::Matrix4x3fType へのポインター。
    //! @param[in] pResult CameraAnimResult へのポインター。
    //!
    static void CalculateAimCameraMtx(nn::util::Matrix4x3fType* pMtx, const CameraAnimResult* pResult) NN_NOEXCEPT;

    //! @brief CameraAnimResult から RotateCamera のビュー行列を計算します。
    //!
    //! @param[out] pMtx nn::util::Matrix4x3fType へのポインター。
    //! @param[in] pResult CameraAnimResult へのポインター。
    //!
    static void CalculateRotateCameraMtx(nn::util::Matrix4x3fType* pMtx, const CameraAnimResult* pResult) NN_NOEXCEPT;

    //! @brief CameraAnimResult から平行投影のプロジェクション行列を計算します。
    //!
    //! @param[out] pMtx nn::util::Matrix4x4fType へのポインター。
    //! @param[in] pResult CameraAnimResult へのポインター。
    //!
    static void CalculateOrthoProjMtx(nn::util::Matrix4x4fType* pMtx, const CameraAnimResult* pResult) NN_NOEXCEPT;

    //! @brief CameraAnimResult から透視投影のプロジェクション行列を計算します。
    //!
    //! @param[out] pMtx nn::util::Matrix4x4fType へのポインター。
    //! @param[in] pResult CameraAnimResult へのポインター。
    //!
    static void CalculatePerspProjMtx(nn::util::Matrix4x4fType* pMtx, const CameraAnimResult* pResult) NN_NOEXCEPT;

    //! @brief CameraAnimResult から平行投影のプロジェクションテクスチャー行列を計算します。
    //!
    //! @param[out] pMtx nn::util::Matrix4x3fType へのポインター。
    //! @param[in] pResult CameraAnimResult へのポインター。
    //!
    static void CalculateOrthoProjTexMtx(nn::util::Matrix4x3fType* pMtx, const CameraAnimResult* pResult) NN_NOEXCEPT;

    //! @brief CameraAnimResult から透視投影のプロジェクションテクスチャー行列を計算します。
    //!
    //! @param[out] pMtx nn::util::Matrix4x3fType へのポインター。
    //! @param[in] pResult CameraAnimResult へのポインター。
    //!
    static void CalculatePerspProjTexMtx(nn::util::Matrix4x3fType* pMtx, const CameraAnimResult* pResult) NN_NOEXCEPT;
};

}} // namespace nn::g3d

