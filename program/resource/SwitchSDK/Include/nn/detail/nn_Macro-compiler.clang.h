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
//  全てのライブラリで共通して使用するマクロの定義（clang 用）
//----------------------------------------------------------------------------

#include <nn/TargetConfigs/build_Compiler.h>
#include <nn/diag/detail/diag_DetailBuildSwitch.h>

#if !defined(NN_BUILD_CONFIG_COMPILER_SUPPORTS_CLANG)
    #error  "CLANG コンパイラーが指定されていません。"
#endif


//----------------------------------------------------------------------------
//  言語サポート
//----------------------------------------------------------------------------

//  アライメントのバイトサイズを指定します。
#define NN_ALIGNAS( alignment ) __attribute__((aligned(alignment)))

//  アライメントのバイトサイズを取得します。
#define NN_ALIGNOF( type ) __alignof__( type )

//  決して返らない関数に対するコンパイラ警告を抑制します。
#define NN_NORETURN __attribute__((noreturn))

//  関数が例外を投げないことを宣言します。
#define NN_NOEXCEPT noexcept

//  関数が仮想関数をオーバーライドすることを宣言します。
#define NN_OVERRIDE override

//  型変換演算子が暗黙的に適用されることを禁止します。
#define NN_EXPLICIT_OPERATOR operator

//  インライン化を意図していないことを明示します。
#define NN_NOINLINE __attribute__((noinline))

//  インライン化を強制する意図を明示します。
#define NN_FORCEINLINE __attribute__((always_inline)) inline

//  現在の関数の名前です。
#define NN_CURRENT_FUNCTION_NAME __FUNCTION__

// C++11 以降の関数ローカルな static 変数定義します。
#define NN_FUNCTION_LOCAL_STATIC(type, name, ...) static type name __VA_ARGS__

// 対象の利用は非推奨であることを宣言します。
#define NN_DEPRECATED __attribute__((deprecated("This is deprecated. Please refer to NintendoSDK API reference for the details.")))

// 対象の返り値を無視した場合に警告を出します。
#if __clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 9)
    // clang 3.9 以上
    #define NN_NODISCARD [[nodiscard]]
#endif

//----------------------------------------------------------------------------
//  環境毎の問題回避
//----------------------------------------------------------------------------

//  宣言時に値を指定したクラス定数の定義におけるエラーを回避します。
#define NN_DEFINE_STATIC_CONSTANT( definition ) definition

//  外部の関数型マクロによるシンボル置き換えを抑止します。
#define NN_PREVENT_MACRO_FUNC

// 1文字が16ビット長（char16_t相当）の文字列リテラルを定義します。
#define NN_CHAR16LITERAL( string ) u##string

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
#define NN_FALL_THROUGH [[clang::fallthrough]]

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
#endif

// クラス内部に暗黙にパディングが挿入されたことの警告を有効にします。
#define NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS \
    _Pragma("GCC diagnostic warning  \"-Wpadded\"")

// クラス内部に、1 バイトのサイズを持つ無名のメンバを定義します。このメンバは、1 バイトアラインです。
#define NN_PADDING1 char :  8

//----------------------------------------------------------------------------
//  警告対策
//----------------------------------------------------------------------------

//  意図した暗黙的呼び出し可能な変換コンストラクタ、変換演算子に対する静的解析ツール警告を
//  抑制します。
#define NN_IMPLICIT

//  意図した未使用変数に対するコンパイラ警告を抑制します。
#define NN_UNUSED( variable ) ( void )( variable )
#define NN_IS_UNUSED_MEMBER         __attribute__((unused))

//  意図した定数分岐を明示してコンパイラ警告を抑制します。
#define NN_STATIC_CONDITION( condition ) ( condition )

//  コンパイラの警告レベルを退避します。
#define NN_PRAGMA_PUSH_WARNINGS     _Pragma("GCC diagnostic push")

//  コンパイラの警告レベルを復帰します。
#define NN_PRAGMA_POP_WARNINGS      _Pragma("GCC diagnostic pop")

//  定数のオーバーフローに対するコンパイラの警告を無効化します。
#define NN_DISABLE_WARNING_OVERFLOW _Pragma("GCC diagnostic ignored  \"-Woverflow\"")

//  関数内で宣言された静的変数に対するコンパイラの警告を無効化します。
#define NN_DISABLE_WARNING_LOCAL_STATIC

//  未使用引数に対するコンパイラの警告を無効化します。
#define NN_DISABLE_WARNING_UNUSED_PARAMETER   _Pragma("GCC diagnostic ignored  \"-Wunused-parameter\"")

//  未使用変数に対するコンパイラの警告を無効化します。
#define NN_DISABLE_WARNING_UNUSED_VAR   _Pragma("GCC diagnostic ignored  \"-Wunused-variable\"")

//  未使用関数に対するコンパイラの警告を無効化します。
#define NN_DISABLE_WARNING_UNUSED_FUNCTION   _Pragma("GCC diagnostic ignored  \"-Wunused-function\"")

//  シンボルの隠蔽に対するコンパイラ警告を無効化します。
#define NN_DISABLE_WARNING_SHADOW _Pragma("GCC diagnostic ignored  \"-Wshadow\"")

//  廃止予定シンボルに対するコンパイラ警告を無効化します。
#define NN_DISABLE_WARNING_DEPRECATED_DECLARATIONS _Pragma("GCC diagnostic ignored  \"-Wdeprecated-declarations\"")

//  コンストラクタのメンバ初期化子リストにおける配列の要素のデフォルト初期化に対するコンパイラの警告を無効化します。
#define NN_DISABLE_WARNING_ARRAY_DEFAULT_INITIALIZATION_IN_CONSTRUCTOR

#if defined(__cplusplus)

//  配列のサイズを返します。
#define NN_ARRAY_SIZE( array ) sizeof(*::nn::detail::ArraySizeImpl( array ))

namespace nn { namespace detail {
    template <typename Type, unsigned long Size>
    char( *ArraySizeImpl(Type(&)[Size]) )[Size];
}}
#endif // __cplusplus
