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
#include <nn/nn_SdkAssert.h>
#include <nn/util/util_CharacterEncoding.h>

namespace nn {
namespace font {

template <typename TGfxObject>
inline bool IsInitialized(const TGfxObject& obj)
{
    return nn::gfx::AccessorToData(obj)->state != TGfxObject::DataType::State_NotInitialized;
}

template <typename T>
inline void FinalizeIfNecessary(T& obj, nn::gfx::Device* pDevice)
{
    if (IsInitialized(obj))
    {
        obj.Finalize(pDevice);
    }
}

template <typename T>
inline void FinalizeIfNecessary(T& obj, bool isInitialized, void (T::*finalizeFunc)(nn::gfx::Device*), nn::gfx::Device* pDevice)
{
    if (isInitialized)
    {
        (obj.*finalizeFunc)(pDevice);
    }
}

template <typename T>
void SwapEndian(T& value)
{
    NN_UNUSED(value);
}


//--------------------------------------------------------------------
//! @brief   ベースクラスの型情報定義マクロです。
//!
//! @details ダイナミックキャストを可能にする為に埋め込むべき実行時型情報を
//!          メンバに含めるマクロです。クラスの宣言中に記述してください。
//!
//!          nn::font::DynamicCast 等の機能が使用できるようになります。
//--------------------------------------------------------------------
#define NN_FONT_RUNTIME_TYPEINFO_ROOT()                                               \
    /*! @briefprivate */                                                        \
    /*! @details クラススタティックな型情報を取得します。 */                          \
    static  const nn::font::detail::RuntimeTypeInfo*    GetRuntimeTypeInfoStatic()  \
    {                                                                               \
        static const nn::font::detail::RuntimeTypeInfo s_TypeInfo(NULL);            \
        return &s_TypeInfo;                                                         \
    }                                                                               \
    /*! @briefprivate */                                                        \
    /*! @details virtual な型情報を取得します。 */                                    \
    virtual const nn::font::detail::RuntimeTypeInfo*    GetRuntimeTypeInfo() const  \
    {                                                                               \
        return GetRuntimeTypeInfoStatic();                                          \
    }

//--------------------------------------------------------------------
//! @brief   サブクラスの型情報定義マクロです。
//!
//! @details ダイナミックキャストを可能にする為に埋め込むべき実行時型情報を
//!          メンバに含めるマクロです。クラスの宣言中に記述してください。
//!
//!          nn::font::DynamicCast 等の機能が使用できるようになります。
//!
//! @param   base 親クラスのクラス名を指定します。
//--------------------------------------------------------------------
#define NN_FONT_RUNTIME_TYPEINFO(base)                                                \
    /*! @briefprivate */                                                        \
    /*! @details クラススタティックな型情報を取得します。 */                          \
    static  const nn::font::detail::RuntimeTypeInfo*    GetRuntimeTypeInfoStatic()  \
    {                                                                               \
        static const nn::font::detail::RuntimeTypeInfo s_TypeInfo( base::GetRuntimeTypeInfoStatic() ); \
        return &s_TypeInfo;                                                         \
    }                                                                               \
    /*! @briefprivate */                                                        \
    /*! @details virtual な型情報を取得します。 */                                    \
    virtual const nn::font::detail::RuntimeTypeInfo*    GetRuntimeTypeInfo() const NN_OVERRIDE \
    {                                                                               \
        return GetRuntimeTypeInfoStatic();                                          \
    }


namespace detail {

//---------------------------------------------------------------------------
//! @brief        配列のサイズを取得します。
//!
//! @param[in]    array サイズを取得したい配列
//!
//! @return       配列のサイズ
//---------------------------------------------------------------------------
template<typename T, size_t S>
inline int GetArrayLength(const T (&array)[S]
)
{
    NN_UNUSED(array);
    return static_cast<int>(S);
}

//--------------------------------------------------------------------
//! @brief 実行時型情報構造体
//--------------------------------------------------------------------
class RuntimeTypeInfo
{
public:
    const RuntimeTypeInfo*  m_ParentTypeInfo; //!< ベースクラスの型情報へのポインタです。

    //--------------------------------------------------------------------
    //! @brief コンストラクタです。
    //! @param[in] parent 親の実行時型情報です。
    //--------------------------------------------------------------------
    /* ctor */ explicit  RuntimeTypeInfo( const RuntimeTypeInfo* parent ) : m_ParentTypeInfo( parent ) {}

    bool                 IsDerivedFrom( const RuntimeTypeInfo* s_TypeInfo ) const
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

//---------------------------------------------------------------------------
//! @brief テンプレート中で、ポインタからポインタを除いた型を取得します。
//---------------------------------------------------------------------------
template < typename T >
struct PeelPointer
{
    template< typename U >  struct In                       {   typedef U   Type;   };
    template< typename U >  struct In<U *>                  {   typedef U   Type;   };
    template< typename U >  struct In<const U *>            {   typedef U   Type;   };
    template< typename U >  struct In<volatile U *>         {   typedef U   Type;   };
    template< typename U >  struct In<const volatile U *>   {   typedef U   Type;   };

    typedef typename In<T>::Type        Type;
};

} // namespace detail

//----------------------------------------
//! @name 実行時型情報
//@{

//---------------------------------------------------------------------------
//! @brief        ダウンキャスト用テンプレート関数です。
//!
//! @tparam       TToPtr  キャスト先のポインタ型を指定します。
//! @param[out]   obj     キャストをおこなうインスタンスです。
//!
//! @return       ダウンキャストに成功した場合はキャスト先クラスのポインタ、失敗した場合は NULL を返します。
//---------------------------------------------------------------------------
template<typename TToPtr, typename TFrom>
inline TToPtr
DynamicCast(TFrom* obj)
{
    const detail::RuntimeTypeInfo* typeInfoU = detail::PeelPointer<TToPtr>::Type::GetRuntimeTypeInfoStatic();
    if ( obj && obj->GetRuntimeTypeInfo()->IsDerivedFrom( typeInfoU ) )
    {
        return static_cast<TToPtr>(obj);
    }
    return NULL;
}

//---------------------------------------------------------------------------
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
StaticCast(TFrom* obj)
{
    NN_SDK_ASSERT( obj == NULL || DynamicCast<TToPtr>( obj ) != NULL );

    return static_cast<TToPtr>(obj);
}

//--------------------------------------------------------------------------
//! @brief        型の比較をおこないます。
//!
//! @tparam       To       比較する型です。
//! @param[in]    instance 型を比較するインスタンスです。
//!
//! @return       テンプレート引数に指定した型と同一のインスタンスであれば真を返します。
//---------------------------------------------------------------------------
template <typename To, typename From>
inline bool
IsTypeOf(const From* instance)
{
    if (instance == NULL) { return false; }
    return instance->GetRuntimeTypeInfo() == To::GetRuntimeTypeInfoStatic();
}

//---------------------------------------------------------------------------
//! @brief        オブジェクトが指定した型にキャスト可能かどうかを取得します。
//!
//! @tparam       To       比較対象の型です。
//! @param[in]    instance 型をチェックするインスタンスです。
//!
//! @return       テンプレート引数に指定した型にキャスト可能であれば真を返します。
//---------------------------------------------------------------------------
template <typename To, typename From>
inline bool
IsDerivedFrom(const From* instance)
{
    const detail::RuntimeTypeInfo* typeInfoU = To::GetRuntimeTypeInfoStatic();
    return instance && instance->GetRuntimeTypeInfo()->IsDerivedFrom( typeInfoU );
}

//---------------------------------------------------------------------------
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
IsSameType( const TypeA* ptrA, const TypeB* ptrB )
{
    if (ptrA == NULL || ptrB == NULL)
    {
        return false;
    }

    return ptrA->GetRuntimeTypeInfo() == ptrB->GetRuntimeTypeInfo();
}


//---------------------------------------------------------------------------
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
IsSameTypeOrNull( const TypeA* ptrA, const TypeB* ptrB )
{
    if (ptrA == NULL)
    {
        return ptrB == NULL;
    }

    return ptrA->GetRuntimeTypeInfo() == ptrB->GetRuntimeTypeInfo();
}

//---------------------------------------------------------------------------
//! @brief UTF-8 の文字列から 1 文字を切り出して UTF-32 に変換します。
//!
//! @param[in] pUtf8 UTF-8 の文字列。
//!
//! @return UTF-32 の 1 文字。
//---------------------------------------------------------------------------
inline uint32_t ConvertCharUtf8ToUtf32(const char* pUtf8)
{
    uint32_t utf32 = 0;
    nn::util::CharacterEncodingResult result = nn::util::ConvertCharacterUtf8ToUtf32(&utf32, pUtf8);
    NN_SDK_ASSERT(result == nn::util::CharacterEncodingResult_Success, "Wrong character code of UTF-8.");
    NN_UNUSED(result);
    return utf32;
}

//---------------------------------------------------------------------------
//! @brief UTF-8 の文字列から 1 文字を切り出して UTF-32 に変換し、UTF-8 のポインタを次のコードポイントへ進めます。
//!
//! @param[in,out] pUtf8 UTF-8 の文字列。次のコードポイントを指すようになります。
//!
//! @return UTF-32 の 1 文字。
//---------------------------------------------------------------------------
inline uint32_t ConvertCharUtf8ToUtf32AndIncrement(const char** pUtf8)
{
    char buffer[4] = { 0 };
    nn::util::CharacterEncodingResult result;
    result = nn::util::PickOutCharacterFromUtf8String(buffer, pUtf8);
    NN_SDK_ASSERT(result == nn::util::CharacterEncodingResult_Success, "Wrong character code of UTF-8.");
    uint32_t utf32 = 0;
    result = nn::util::ConvertCharacterUtf8ToUtf32(&utf32, buffer);
    NN_SDK_ASSERT(result == nn::util::CharacterEncodingResult_Success, "Wrong character code of UTF-8.");
    NN_UNUSED(result);
    return utf32;
}

//---------------------------------------------------------------------------
//! @brief isDrawingHyphenMinusInsteadOfNonBreakingHyphen が true のとき、NON-BREAKING HYPHEN(U+2011) に代わって HYPHEN-MINUS(U+002D) を返します。
//!
//! @param[in] code 変換前の文字。
//! @param[in] isDrawingHyphenMinusInsteadOfNonBreakingHyphen NON-BREAKING HYPHEN(U+2011) の文字を描画する際に、HYPHEN-MINUS(U+002D) のグリフを使うかどうかです。
//!
//! @return 変換後の文字。
//---------------------------------------------------------------------------
inline uint32_t ConvertNonBreakingHyphenToHyphenMinus(uint32_t code, bool isDrawingHyphenMinusInsteadOfNonBreakingHyphen)
{
    const uint32_t CharCodeNonBreakingHyphen = 0x2011;
    const uint32_t CharCodeHyphenMinus = 0x002d;
    if (isDrawingHyphenMinusInsteadOfNonBreakingHyphen && code == CharCodeNonBreakingHyphen)
    {
        return CharCodeHyphenMinus;
    }
    return code;
}

//@}

} // namespace font
} // nemespace nn

