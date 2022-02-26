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

#include <stddef.h>
#include <nn/atk/fnd/basis/atkfnd_TypeTraits.h>

namespace nn { namespace atk { namespace detail { namespace fnd {

//--------------------------------------------------------------------
//! @internal
//! @brief   ベースクラスの型情報定義マクロです。
//!
//! @details ダイナミックキャストを可能にする為に埋め込むべき実行時型情報を
//!          メンバに含めるマクロです。クラスの宣言中に記述してください。
//!
//!          nn::atk::detail::fnd::DynamicCast 等の機能が使用できるようになります。
//--------------------------------------------------------------------
#define NN_ATK_DETAIL_RUNTIME_TYPEINFO_ROOT()                                             \
    /*! @details クラススタティックな型情報を取得します。 */                              \
    static NN_NOINLINE const ::nn::atk::detail::fnd::RuntimeTypeInfo* GetRuntimeTypeInfoStatic() NN_NOEXCEPT \
    {                                                                                     \
        static const ::nn::atk::detail::fnd::RuntimeTypeInfo s_TypeInfo(NULL);                               \
        return &s_TypeInfo;                                                               \
    }                                                                                     \
    /*! @details virtual な型情報を取得します。 */                                        \
    virtual const ::nn::atk::detail::fnd::RuntimeTypeInfo*    GetRuntimeTypeInfo() const NN_NOEXCEPT         \
    {                                                                                     \
        return GetRuntimeTypeInfoStatic();                                                \
    }

//--------------------------------------------------------------------
//! @internal
//! @brief   サブクラスの型情報定義マクロです。
//!
//! @details ダイナミックキャストを可能にする為に埋め込むべき実行時型情報を
//!          メンバに含めるマクロです。クラスの宣言中に記述してください。
//!
//!          nn::atk::detail::fnd::DynamicCast 等の機能が使用できるようになります。
//!
//! @param   base 親クラスのクラス名を指定します。
//--------------------------------------------------------------------
#define NN_ATK_DETAIL_RUNTIME_TYPEINFO(base)                                                  \
    /*! @details クラススタティックな型情報を取得します。 */                                  \
    static  const ::nn::atk::detail::fnd::RuntimeTypeInfo*    GetRuntimeTypeInfoStatic() NN_NOEXCEPT             \
    {                                                                                         \
        static const ::nn::atk::detail::fnd::RuntimeTypeInfo s_TypeInfo( base::GetRuntimeTypeInfoStatic() );     \
        return &s_TypeInfo;                                                                   \
    }                                                                                         \
    /*! @details virtual な型情報を取得します。 */                                            \
    virtual const ::nn::atk::detail::fnd::RuntimeTypeInfo*    GetRuntimeTypeInfo() const NN_NOEXCEPT NN_OVERRIDE \
    {                                                                                         \
        return GetRuntimeTypeInfoStatic();                                                    \
    }

//--------------------------------------------------------------------
//! @internal
//! @brief 実行時型情報構造体
//--------------------------------------------------------------------
struct RuntimeTypeInfo
{
    const RuntimeTypeInfo*  m_ParentTypeInfo; //!< ベースクラスの型情報へのポインタです。

    //--------------------------------------------------------------------
    //! @brief コンストラクタです。
    //! @param[in] parent 親の実行時型情報です。
    //--------------------------------------------------------------------
    /* ctor */ explicit  RuntimeTypeInfo( const RuntimeTypeInfo* parent ) NN_NOEXCEPT : m_ParentTypeInfo( parent ) {}

    bool                 IsDerivedFrom( const RuntimeTypeInfo* s_TypeInfo ) const NN_NOEXCEPT
    {
        const RuntimeTypeInfo *self = this;

        while ( self )
        {
            if ( self == s_TypeInfo )
            {
                return true;
            }
            self = self->m_ParentTypeInfo;
        }
        return false;
    }
};

//----------------------------------------
//! @name 基礎
//@{

//---------------------------------------------------------------------------
//! @internal
//! @brief        ダウンキャスト用テンプレート関数です。
//!
//! @tparam       TToPtr  キャスト先のポインタ型を指定します。
//! @param[out]   obj     キャストをおこなうインスタンスです。
//!
//! @return       ダウンキャストに成功した場合はキャスト先クラスのポインタ、失敗した場合は NULL を返します。
//---------------------------------------------------------------------------
template<typename TToPtr, typename TFrom>
inline TToPtr
DynamicCast(TFrom* obj) NN_NOEXCEPT
{
    const fnd::RuntimeTypeInfo* typeInfoU = PeelPointer<TToPtr>::Type::GetRuntimeTypeInfoStatic();
    if ( obj && obj->GetRuntimeTypeInfo()->IsDerivedFrom( typeInfoU ) )
    {
        return static_cast<TToPtr>(obj);
    }
    return nullptr;
}

//---------------------------------------------------------------------------
//! @internal
//! @brief        ダウンキャスト可能かどうかの型チェックの ASSERT 付きスタティックキャストです。
//!               入力が NULL の場合は、ASSERT せず NULL を返します。
//!
//! @tparam       TToPtr  キャスト先のポインタ型を指定します。
//! @param[out]   obj     キャストをおこなうインスタンスです。
//!
//! @return       キャスト結果を返します。
//---------------------------------------------------------------------------
template<typename TToPtr, typename TFrom>
inline TToPtr
StaticCast(TFrom* obj) NN_NOEXCEPT
{
    NN_SDK_ASSERT( obj == nullptr || DynamicCast<TToPtr>( obj ) != nullptr );

    return static_cast<TToPtr>(obj);
}


//--------------------------------------------------------------------------
//! @internal
//! @brief        型の比較をおこないます。
//!
//! @tparam       To       比較する型です。
//! @param[in]    instance 型を比較するインスタンスです。
//!
//! @return       テンプレート引数に指定した型と同一のインスタンスであれば真を返します。
//---------------------------------------------------------------------------
template <typename To, typename From>
inline bool
IsTypeOf(const From* instance) NN_NOEXCEPT
{
    if (instance == nullptr) { return false; }
    return instance->GetRuntimeTypeInfo() == To::GetRuntimeTypeInfoStatic();
}

//---------------------------------------------------------------------------
//! @internal
//! @brief        オブジェクトが指定した型にキャスト可能かどうかを取得します。
//!
//! @tparam       To       比較対象の型です。
//! @param[in]    instance 型をチェックするインスタンスです。
//!
//! @return       テンプレート引数に指定した型にキャスト可能であれば真を返します。
//---------------------------------------------------------------------------
template <typename To, typename From>
inline bool
IsDerivedFrom(const From* instance) NN_NOEXCEPT
{
    const fnd::RuntimeTypeInfo* typeInfoU = To::GetRuntimeTypeInfoStatic();
    return instance && instance->GetRuntimeTypeInfo()->IsDerivedFrom( typeInfoU );
}

//---------------------------------------------------------------------------
//! @internal
//! @brief        インスタンスが同じ型かどうかを比較します。
//!               NULL の場合には結果は false になります。
//!
//! @param[in]    ptrA   比較するインスタンス１です。
//! @param[in]    ptrB   比較するインスタンス２です。
//!
//! @return       同一のクラスのインスタンスであれば true、そうでなければ false を返します。
//---------------------------------------------------------------------------
template< typename TypeA, typename TypeB >
inline bool
IsSameType( const TypeA* ptrA, const TypeB* ptrB ) NN_NOEXCEPT
{
    if (ptrA == nullptr || ptrB == nullptr)
    {
        return false;
    }

    return ptrA->GetRuntimeTypeInfo() == ptrB->GetRuntimeTypeInfo();
}


//---------------------------------------------------------------------------
//! @internal
//! @brief        インスタンスが同じ型かどうかを比較します。
//!               両方が NULL の場合にも結果は true になります。
//!
//! @param[in]    ptrA   比較するインスタンス１です。
//! @param[in]    ptrB   比較するインスタンス２です。
//!
//! @return       同一のクラスのインスタンスであれば true、そうでなければ false を返します。
//---------------------------------------------------------------------------
template< typename TypeA, typename TypeB >
inline bool
IsSameTypeOrNull( const TypeA* ptrA, const TypeB* ptrB ) NN_NOEXCEPT
{
    if (ptrA == nullptr)
    {
        return ptrB == nullptr;
    }

    return ptrA->GetRuntimeTypeInfo() == ptrB->GetRuntimeTypeInfo();
}


//@}

}}}} // namespace nn::atk::detail::fnd
