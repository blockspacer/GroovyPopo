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

namespace nn { namespace atk { namespace detail { namespace fnd {

template <typename TType, TType Value>
struct integral_constant
{
    static const TType value = Value;
    typedef TType value_type;
};

typedef integral_constant<bool, true>  TrueType;
typedef integral_constant<bool, false> FalseType;

template<bool Cond, typename Then, typename Else>
struct IfCond;

template<typename Then, typename Else>
struct IfCond<true, Then, Else>
{
    typedef Then type;
};

template<typename Then, typename Else>
struct IfCond<false, Then, Else>
{
    typedef Else type;
};

struct True  { char a;    };
struct False { True  a[2]; };

/*---------------------------------------------------------------------------*
  declare_type_remove_template
 *---------------------------------------------------------------------------*/
#define NN_ATK_DECLARE_TYPE_REMOVE_TEMPLATE_BASE_(name, ttype)  \
template<typename ttype>                                        \
struct name                                                     \
{                                                               \
    typedef ttype type;                                         \
};

#define NN_ATK_DECLARE_TYPE_REMOVE_TEMPLATE_(name, ttype, modifier) \
template<typename ttype>                                            \
struct name<modifier>                                               \
{                                                                   \
    typedef TType type;                                             \
};

#define NN_ATK_DECLARE_TYPE_REMOVE_TEMPLATE2_(name, ttype, modifier, ...) \
template<typename __VA_ARGS__>                                            \
struct name<modifier>                                                     \
{                                                                         \
    typedef TType type;                                                   \
};

/*---------------------------------------------------------------------------*
  remove_bounds
 *---------------------------------------------------------------------------*/
NN_ATK_DECLARE_TYPE_REMOVE_TEMPLATE_BASE_(remove_bounds, TType)
NN_ATK_DECLARE_TYPE_REMOVE_TEMPLATE_(remove_bounds, TType, TType[])
NN_ATK_DECLARE_TYPE_REMOVE_TEMPLATE2_(remove_bounds, TType, TType[Size], TType, int Size)

/*---------------------------------------------------------------------------*
  remove_const
 *---------------------------------------------------------------------------*/
NN_ATK_DECLARE_TYPE_REMOVE_TEMPLATE_BASE_(remove_const, TType)
NN_ATK_DECLARE_TYPE_REMOVE_TEMPLATE_(remove_const, TType, const TType)

/*---------------------------------------------------------------------------*
  remove_reference
 *---------------------------------------------------------------------------*/
NN_ATK_DECLARE_TYPE_REMOVE_TEMPLATE_BASE_(remove_reference, TType)
NN_ATK_DECLARE_TYPE_REMOVE_TEMPLATE_(remove_reference, TType, TType&)

/*---------------------------------------------------------------------------*
  remove_pointer
 *---------------------------------------------------------------------------*/
NN_ATK_DECLARE_TYPE_REMOVE_TEMPLATE_BASE_(remove_pointer, TType)
NN_ATK_DECLARE_TYPE_REMOVE_TEMPLATE_(remove_pointer, TType, TType*)
NN_ATK_DECLARE_TYPE_REMOVE_TEMPLATE_(remove_pointer, TType, TType* const)

/*---------------------------------------------------------------------------*
  Is_*
 *---------------------------------------------------------------------------*/
#define NN_ATK_TRAITS_SPEC0(Spec, Value) \
    template <> \
    struct Spec : public integral_constant<bool, Value> {};

#define NN_ATK_TRAITS_SPEC1(Spec, Value) \
    template <typename Type> \
    struct Spec : public integral_constant<bool, Value> {};

#define NN_ATK_TRAITS_SPEC2(Spec, Value) \
    template <typename Type, typename ClassName> \
    struct Spec : public integral_constant<bool, Value> {};

#define NN_ATK_TRAITS_SPEC(Order, Traits, SpecialType, Value)            \
    NN_ATK_TRAITS_SPEC##Order(Traits<SpecialType>,                Value) \
    NN_ATK_TRAITS_SPEC##Order(Traits<SpecialType const>,          Value) \
    NN_ATK_TRAITS_SPEC##Order(Traits<SpecialType volatile>,       Value) \
    NN_ATK_TRAITS_SPEC##Order(Traits<SpecialType const volatile>, Value)


//---------------------------------------------------------------------------
//! @internal
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

//---------------------------------------------------------------------------
//! @internal
//! @brief テンプレート中で、参照から参照を除いた型を取得する。
//---------------------------------------------------------------------------
template < typename T >
struct PeelReference
{
    template< typename U >  struct In                       {   typedef U   Type;   };
    template< typename U >  struct In<U &>                  {   typedef U   Type;   };
    template< typename U >  struct In<const U &>            {   typedef U   Type;   };
    template< typename U >  struct In<volatile U &>         {   typedef U   Type;   };
    template< typename U >  struct In<const volatile U &>   {   typedef U   Type;   };

    typedef typename In<T>::Type        Type;
};

//---------------------------------------------------------------------------
//! @internal
//! @brief テンプレート中で、型が配列かどうかを判定します。
//---------------------------------------------------------------------------
template <typename TType>
struct IsArray : public FalseType {};

//! @internal
template <typename TType, int Size>
struct IsArray<TType[Size]> : public TrueType {};

//! @internal
template <typename TType>
struct IsArray<TType[]> : public TrueType {};

//---------------------------------------------------------------------------
//! @internal
//! @brief テンプレート中で、型がポインタかどうかを判定します。
//---------------------------------------------------------------------------
template <typename Type>
struct IsPointer : public FalseType {};
NN_ATK_TRAITS_SPEC(1, IsPointer, Type*, true)


//---------------------------------------------------------------------------
//! @internal
//! @brief テンプレート中で、型が参照かどうかを判定します。
//---------------------------------------------------------------------------
template <typename Type>
struct IsReference : public FalseType {};

//! @internal
template <typename Type>
struct IsReference<Type&> : public TrueType {};

//---------------------------------------------------------------------------
//! @internal
//!  @brief テンプレート中で、二つの型が等しいかどうかを判定します。
//!
//!  @details
//!  IsSame を STATIC_ASSERT などのマクロで使用する際には、 IsSame 全体を
//!  括弧で括らないと "," がマクロ引数と解釈され構文エラーとなりますのでご注意下さい。
//---------------------------------------------------------------------------
template <typename, typename>
struct IsSame : public FalseType {};

//! @internal
template <typename Type>
struct IsSame<Type, Type> : public TrueType {};

//---------------------------------------------------------------------------
//! @internal
//! @brief テンプレート中で、型がクラスかどうかを判定します。
//---------------------------------------------------------------------------
template <class T>
struct IsClass
{
    template <class C> static True test(int C::*);
    template <class C> static False test(...);
    static const bool value = (sizeof(test<T>(0)) == sizeof(True));
};

//---------------------------------------------------------------------------
//! @internal
//! @brief テンプレート中で、条件によってい処理を切り替えます。
//---------------------------------------------------------------------------
template<typename Cond, typename Then, typename Else>
struct If_ : public IfCond<Cond::value, Then, Else> {};

}}}} // namespace nn::atk::detail::fnd
