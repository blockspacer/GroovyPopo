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

//----------------------------------------------------------------------------
//  全てのライブラリで共通して使用するマクロの定義（MSVC++ 用）
//----------------------------------------------------------------------------

#include <nn/TargetConfigs/build_Compiler.h>
#include <nn/diag/detail/diag_DetailBuildSwitch.h>

#if !defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_VC)
    #error  "VC++ コンパイラーが指定されていません。"
#endif


//----------------------------------------------------------------------------
//  言語サポート
//----------------------------------------------------------------------------

#if defined(__INTELLISENSE__)
//  アライメントのバイトサイズを指定します。
#define NN_ALIGNAS( alignment ) /* __declspec( align( alignment ) ) */
#else
#define NN_ALIGNAS( alignment ) __declspec( align( alignment ) )
#endif

//  アライメントのバイトサイズを取得します。
#define NN_ALIGNOF( type ) __alignof( type )

//  決して返らない関数に対するコンパイラ警告を抑制します。
#define NN_NORETURN __declspec( noreturn )

//  関数が例外を投げないことを宣言します。
#define NN_NOEXCEPT throw()

//  関数が仮想関数をオーバーライドすることを宣言します。
#define NN_OVERRIDE override

//  型変換演算子が暗黙的に適用されることを禁止します。
#define NN_EXPLICIT_OPERATOR operator

//  インライン化を意図していないことを明示します。
#define NN_NOINLINE __declspec( noinline )

//  インライン化を強制する意図を明示します。
#define NN_FORCEINLINE __forceinline

//  現在の関数の名前です。
#define NN_CURRENT_FUNCTION_NAME __FUNCTION__

// 対象の利用は非推奨であることを宣言します。
#define NN_DEPRECATED __declspec(deprecated("This is deprecated. Please refer to NintendoSDK API reference for the details."))

// 対象の返り値を無視した場合に警告を出します。
#if defined(_MSC_VER) && _MSC_VER >= 1911 && defined(_MSVC_LANG) && _MSVC_LANG >= 201703
    // VS2017 Update3 以上 (_MSC_VER >= 1911) && C++17 以上 (_MSVC_LANG >= 201703)
    #define NN_NODISCARD [[nodiscard]]
#endif

#if defined(__cplusplus)
// C++11 以降の関数ローカルな static 変数定義します。
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC_VS2012) || defined(NN_BUILD_CONFIG_TOOLCHAIN_VC_VS2013)

//  内部実装用
namespace nn { namespace os { namespace detail {

    void CallOnceForFunctionLocalStaticImpl(int* pFlag, void (*)(void* p), void* p) NN_NOEXCEPT;

    template <typename F>
    inline void CallOnceForFunctionLocalStaticImplT(int* pFlag, F&& f) NN_NOEXCEPT
    {
        CallOnceForFunctionLocalStaticImpl(pFlag, [](void* p) { (*reinterpret_cast<F*>(p))(); }, reinterpret_cast<void*>(&f));
    }

}}}

    #define NN_FUNCTION_LOCAL_STATIC(type_, v, ...) \
        type_& v = ([&]() -> type_& \
        { \
            static volatile const void* _nn_function_local_static_p; \
            { \
                static int _nn_function_local_static_flag; \
                ::nn::os::detail::CallOnceForFunctionLocalStaticImplT(&_nn_function_local_static_flag, [&]() \
                { \
                    static type_ _nn_function_local_static_x __VA_ARGS__; \
                    _nn_function_local_static_p = &reinterpret_cast<volatile const char&>(_nn_function_local_static_x); /* NOLINT(analysis/casting) */ \
                }); \
            } \
            return *reinterpret_cast<type_*>(const_cast<void*>(_nn_function_local_static_p)); \
        }())

#else
    #define NN_FUNCTION_LOCAL_STATIC(type, name, ...) static type name __VA_ARGS__
#endif
#endif // __cplusplus

//----------------------------------------------------------------------------
//  環境毎の問題回避
//----------------------------------------------------------------------------

//  宣言時に値を指定したクラス定数の定義におけるエラーを回避します。
#define NN_DEFINE_STATIC_CONSTANT( definition )

//  外部の関数型マクロによるシンボル置き換えを抑止します。
// マクロの中では使用できません
#define NN_PREVENT_MACRO_FUNC

// 1文字が16ビット長（char16_t相当）の文字列リテラルを定義します。
#if defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_UNICODE_LITERALS)
#define NN_CHAR16LITERAL( string ) u##string
#else
#define NN_CHAR16LITERAL( string ) L##string
#endif

//----------------------------------------------------------------------------
//  ユーティリティ
//----------------------------------------------------------------------------

//  コピーコンストラクタとコピー代入演算子を禁止します。
#define NN_DISALLOW_COPY( type ) \
private: \
    type( const type& ); \
    type& operator=( const type& )

//  ムーブコンストラクタとムーブ代入演算子を禁止します。
#define NN_DISALLOW_MOVE( type ) \
private: \
    type( type&& ); \
    type& operator=( type&& )

//  型または変数のビットサイズを取得します。
//  ヘッダのインクルードを不要にするため CHAR_BIT の値を展開して記述しています。
//  CHAR_BIT が異なる環境に対応する際は切り分けを行います。
#define NN_BITSIZEOF( type ) ( static_cast< int >( sizeof( type ) ) * 8 )

//  文字列リテラルを作成します。
#define NN_MACRO_STRINGIZE( text ) NN_MACRO_STRINGIZE_IMPL_( text )

//  シンボルを連結します。
#define NN_MACRO_CONCATENATE( x, y ) NN_MACRO_CONCATENATE_IMPL_( x, y )

//  内部実装用
#define NN_MACRO_STRINGIZE_IMPL_( text ) #text
#define NN_MACRO_CONCATENATE_IMPL_( x, y ) x##y

//  重複しない一時シンボルを作成します。
#define NN_MAKE_TEMPORARY_NAME( prefix ) NN_MACRO_CONCATENATE( prefix, __COUNTER__ )

//  switch 文において、case ラベルの処理が直後の case ラベルに
//  フォールスルーすることを明示します。
#define NN_FALL_THROUGH

#if defined(__cplusplus)
//  switch 文において、default ラベルに決して到達しないことを表明します。
//  default ラベルに到達した場合、実行を停止します。
#if defined(NN_DETAIL_ENABLE_ABORT_MESSAGE)
    #define NN_UNEXPECTED_DEFAULT \
        ::nn::detail::UnexpectedDefaultImpl( \
            NN_CURRENT_FUNCTION_NAME, __FILE__, __LINE__)
#else // defined(NN_DETAIL_ENABLE_ABORT_MESSAGE)
    #define NN_UNEXPECTED_DEFAULT \
        ::nn::detail::UnexpectedDefaultImpl( \
            "", "", 0)
#endif // defined(NN_DETAIL_ENABLE_ABORT_MESSAGE)

namespace nn { namespace detail {
    NN_NORETURN void UnexpectedDefaultImpl(
        const char* functionName,
        const char* fileName,
        int lineNumber) NN_NOEXCEPT;
}}
#endif // __cplusplus

// クラス内部に暗黙にパディングが挿入されたことの警告を有効にします。
#define NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS \
    __pragma( warning( 1: 4820 ) )

// クラス内部に、1 バイトのサイズを持つ無名のメンバを定義します。このメンバは、1 バイトアラインです。
#define NN_PADDING1 char : 8

//----------------------------------------------------------------------------
//  警告対策
//----------------------------------------------------------------------------

//  意図した暗黙的呼び出し可能な変換コンストラクタ、変換演算子に対する静的解析ツール警告を
//  抑制します。
#define NN_IMPLICIT

//  意図した未使用変数に対するコンパイラ警告を抑制します。
#define NN_UNUSED( variable ) ( void )( variable )
#define NN_IS_UNUSED_MEMBER

//  意図した定数分岐を明示してコンパイラ警告を抑制します。
#define NN_STATIC_CONDITION( condition ) ( ( void )0, ( condition ) )

//  コンパイラの警告レベルを退避します。
#define NN_PRAGMA_PUSH_WARNINGS __pragma( warning( push ) )

//  コンパイラの警告レベルを復帰します。
#define NN_PRAGMA_POP_WARNINGS __pragma( warning( pop ) )

//  定数のオーバーフローに対するコンパイラの警告を無効化します。
#define NN_DISABLE_WARNING_OVERFLOW \
    __pragma( warning( disable:4307 ) )

//  関数内で宣言された静的変数に対するコンパイラの警告を無効化します。
#define NN_DISABLE_WARNING_LOCAL_STATIC \
    __pragma( warning( disable:4640 ) )

// 未使用引数に対するコンパイラの警告を無効化します。
#define NN_DISABLE_WARNING_UNUSED_PARAMETER \
    __pragma( warning( disable:4100 ) )

//  未使用変数に対するコンパイラの警告を無効化します。
#define NN_DISABLE_WARNING_UNUSED_VAR \
    __pragma( warning( disable:4101 ) ) \
    __pragma( warning( disable:4189 ) )

//  未使用関数に対するコンパイラの警告を無効化します。
#define NN_DISABLE_WARNING_UNUSED_FUNCTION \
    __pragma( warning( disable:4505 ) ) \
    __pragma( warning( disable:4514 ) )

//  シンボルの隠蔽に対するコンパイラ警告を無効化します。
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC_VS2012) || defined(NN_BUILD_CONFIG_TOOLCHAIN_VC_VS2013)
#define NN_DISABLE_WARNING_SHADOW
#else
#define NN_DISABLE_WARNING_SHADOW \
    __pragma( warning( disable:4456 ) ) \
    __pragma( warning( disable:4457 ) ) \
    __pragma( warning( disable:4458 ) ) \
    __pragma( warning( disable:4459 ) )
#endif

//  廃止予定シンボルに対するコンパイラ警告を無効化します。
#define NN_DISABLE_WARNING_DEPRECATED_DECLARATIONS \
    __pragma( warning( disable:4996 ) )

//  コンストラクタのメンバ初期化子リストにおける配列の要素のデフォルト初期化に対するコンパイラの警告を無効化します。
#define NN_DISABLE_WARNING_ARRAY_DEFAULT_INITIALIZATION_IN_CONSTRUCTOR \
    __pragma( warning( disable:4351 ) )

//  Windows SDK のヘッダファイルで発生するコンパイラ警告を無効化します。
#define NN_DISABLE_WARNING_FROM_WINDOWS_SDK_HEADERS \
    __pragma( warning( disable:4668 ) )

#if defined(__cplusplus)

//  配列のサイズを返します。
#define NN_ARRAY_SIZE( array ) sizeof(*::nn::detail::ArraySizeImpl( array ))

namespace nn { namespace detail {
    template <typename Type, unsigned long Size>
    char( *ArraySizeImpl(Type(&)[Size]) )[Size];
}}
#endif // __cplusplus
