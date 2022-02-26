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

/*!
    @file
    @brief 全てのライブラリで共通して使用するマクロの定義
*/

#include <nn/TargetConfigs/build_Compiler.h>

#if defined(NN_BUILD_CONFIG_COMPILER_VC)
    #include <nn/detail/nn_Macro-compiler.vc.h>
#elif defined(NN_BUILD_CONFIG_COMPILER_GCC)
    #include <nn/detail/nn_Macro-compiler.gcc.h>
#elif defined(NN_BUILD_CONFIG_COMPILER_CLANG)
    #include <nn/detail/nn_Macro-compiler.clang.h>
#elif defined(NN_BUILD_CONFIG_COMPILER_GHS)
    #include <nn/detail/nn_Macro-compiler.ghs.h>
#else
    #error  "未サポートのコンパイラー種別が指定されています。"
#endif

/*!
    @brief クラス内部に、2 バイトのサイズを持つ無名のメンバを定義します。このメンバは、1 バイトアラインです。
*/
#define NN_PADDING2 NN_PADDING1; NN_PADDING1

/*!
    @brief クラス内部に、3 バイトのサイズを持つ無名のメンバを定義します。このメンバは、1 バイトアラインです。
*/
#define NN_PADDING3 NN_PADDING1; NN_PADDING2

/*!
    @brief クラス内部に、4 バイトのサイズを持つ無名のメンバを定義します。このメンバは、1 バイトアラインです。
*/
#define NN_PADDING4 NN_PADDING1; NN_PADDING3

/*!
    @brief クラス内部に、5 バイトのサイズを持つ無名のメンバを定義します。このメンバは、1 バイトアラインです。
*/
#define NN_PADDING5 NN_PADDING1; NN_PADDING4

/*!
    @brief クラス内部に、6 バイトのサイズを持つ無名のメンバを定義します。このメンバは、1 バイトアラインです。
*/
#define NN_PADDING6 NN_PADDING1; NN_PADDING5

/*!
    @brief クラス内部に、7 バイトのサイズを持つ無名のメンバを定義します。このメンバは、1 バイトアラインです。
*/
#define NN_PADDING7 NN_PADDING1; NN_PADDING6

/*!
    @brief クラス内部に、8 バイトのサイズを持つ無名のメンバを定義します。このメンバは、1 バイトアラインです。
*/
#define NN_PADDING8 NN_PADDING1; NN_PADDING7

#if defined(NN_BUILD_FOR_DOCUMENT_GENERATION)

//----------------------------------------------------------------------------
//  Doxygen ドキュメント
//----------------------------------------------------------------------------

//! @{
//! @name 言語サポート

/*!
    @brief アライメントのバイトサイズを指定します。

    @param[in]  alignment アライメントのバイトサイズ（2のべき乗）
    @details    使用例：
                @code
                struct NN_ALIGNAS( 16 ) Example1
                {
                    int x;
                };

                struct Example2
                {
                    NN_ALIGNAS( 16 ) char buffer[ 64 ];
                };

                NN_ALIGNAS( 16 ) char buffer[ 256 ];
                @endcode
*/
#define NN_ALIGNAS( alignment )

/*!
    @brief アライメントのバイトサイズを取得します。

    @param[in]  type 対象の型
    @return     アライメントのバイトサイズ
*/
#define NN_ALIGNOF( type )

/*!
    @brief 決して返らない関数に対するコンパイラ警告を抑制します。

    @details    使用例：
                @code
                NN_NORETURN
                void DoSomething()
                {
                    throw std::runtime_error( "error" );
                }
                @endcode
*/
#define NN_NORETURN

/*!
    @brief 関数が例外を投げないことを宣言します。

    @details    使用例：
                @code
                void DoSomething() NN_NOEXCEPT
                {
                }
                @endcode
*/
#define NN_NOEXCEPT

/*!
    @brief 関数が仮想関数をオーバーライドすることを宣言します。

    @details    使用例：
                @code
                virtual void DoSomething1() NN_OVERRIDE;
                @endcode
                const や NN_NOEXCEPT と併用する場合は以下の順序で記述します。
                @code
                virtual void DoSomething2() const NN_NOEXCEPT NN_OVERRIDE;
                @endcode
*/
#define NN_OVERRIDE

/*!
    @brief 型変換演算子が暗黙的に適用されることを禁止します。

    @details    C++11 対応環境以外では暗黙の型変換は禁止されません。

                使用例：
                @code
                NN_EXPLICIT_OPERATOR bool( const Example& example ) const;
                @endcode
*/
#define NN_EXPLICIT_OPERATOR

/*!
    @brief インライン化を意図していないことを明示します。

    @details    使用例：
                @code
                NN_NOINLINE
                void DoSomething()
                {
                }
                @endcode
*/
#define NN_NOINLINE

/*!
    @brief インライン化を強制する意図を明示します。

    @details    使用例：
                @code
                NN_FORCEINLINE
                void DoSomething()
                {
                }
                @endcode
*/
#define NN_FORCEINLINE

/*!
    @brief 現在の関数の名前です。

    @details    関数名の書式は、処理系に依存します。
*/
#define NN_CURRENT_FUNCTION_NAME

/*!
    @brief C++11 以降の関数ローカルな static 変数定義します。

    @param[in]  type 定義する型
    @param[in]  name 定義する変数の名前
    @param[in]  ...  変数の初期化部

    @details    VC++2012 と VC++2013 環境で、関数内における以下のコードの C++11 以降でのエミュレートを行います。

                @code
                static type name __VA_ARGS__
                @endcode

                name で指定された変数の初期化はスレッドセーフに行われます。

                それ以外の環境では、上記のコードそのままに展開されます。

                使用例：
                @code
                class C
                {
                public:
                    C();
                    NN_IMPLICIT C(int);
                    C(int, int);
                };
                void F()
                {
                    NN_FUNCTION_LOCAL_STATIC(C, x0);           // static C x0;
                    NN_FUNCTION_LOCAL_STATIC(C, x1a, = 10);    // static C x1a = 10;
                    NN_FUNCTION_LOCAL_STATIC(C, x1b, (10));    // static C x1b(10);
                    NN_FUNCTION_LOCAL_STATIC(C, x2, (10, 20)); // static C x2(10, 20);
                }
                @endcode
*/
#define NN_FUNCTION_LOCAL_STATIC(type, name, ...)

/*!
    @brief 対象の利用は非推奨であることを宣言します。

    @details    関数に対しては、宣言・定義の先頭に本マクロを記述してください。
                構造体・クラスに対しては、struct/class キーワードと識別子の間に本マクロを記述してください。
                これ以外の場所に記述した場合、環境によっては正しくビルドできません。

                使用例：
                @code
                NN_DEPRECATED
                void DoSomething()
                {
                };

                class NN_DEPRECATED C
                {
                };
                @endcode
*/
#define NN_DEPRECATED

/*!
    @brief 修飾対象が返り値の関数の返り値を使用しなかった場合に警告を出すことを宣言します。([[nodiscard]] と同等)

    @details
     C++17 以降でサポートされる [[nodiscard]] 属性と同等です。
     C++17 より前のコンパイル環境においては、同等のコンパイラ拡張があればそれを使用します。

     コンパイラに対応する機能がなければ、空マクロに変換され、この場合、警告は出ません。

     本属性マクロは、型宣言と関数を修飾することができます。
     型を修飾する場合には、型宣言における struct/class キーワードと型名の識別子の間に本マクロを記述してください。
     この場合、その型を返す全関数が警告の対象となります。
     関数を修飾する場合は、関数宣言の先頭に本マクロを記述してください。

     使用例：
     @code
     NN_NODISCARD int DoSomething();
     class NN_NODISCARD C;
     @endcode
*/
#define NN_NODISCARD [[nodiscard]]

//! @}

//----------------------------------------------------------------------------

//! @{
//! @name 環境毎の問題回避

/*!
    @brief 宣言時に値を指定したクラス定数の定義におけるエラーを回避します。

    @param[in]  definition クラス定数の定義
    @details    MSVC でのみ起きるリンクエラーを回避します。

                使用例：
                @code
                struct Example
                {
                    static const int Constant = 0;
                };

                NN_DEFINE_STATIC_CONSTANT( const int Example::Constant );
                @endcode
*/
#define NN_DEFINE_STATIC_CONSTANT( definition )

/*!
    @brief 外部の関数型マクロによるシンボル置き換えを抑止します。

    @details    使用例：
                @code
                std::numeric_limits< float >::min NN_PREVENT_MACRO_FUNC ( x, y );
                @endcode
*/
#define NN_PREVENT_MACRO_FUNC

/*!
    @brief 1文字が16ビット長（char16_t相当）の文字列リテラルを定義します。

    @details    使用例：
                @code
                NN_CHAR16LITERAL("some string.");
                @endcode
*/
#define NN_CHAR16LITERAL( string )


//! @}

//----------------------------------------------------------------------------

//! @{
//! @name ユーティリティ

/*!
    @brief コピーコンストラクタとコピー代入演算子を禁止します。

    @param[in]  type 対象の型名
    @details    使用後はメンバーアクセスが private になります。

                使用例：
                @code
                class Example
                {
                    NN_DISALLOW_COPY( Example );

                public:
                };
                @endcode
*/
#define NN_DISALLOW_COPY( type )

/*!
    @brief ムーブコンストラクタとムーブ代入演算子を禁止します。

    @param[in]  type 対象の型名
    @details    使用後はメンバーアクセスが private になります。

                使用例：
                @code
                class Example
                {
                    NN_DISALLOW_MOVE( Example );

                public:
                };
                @endcode
*/
#define NN_DISALLOW_MOVE( type )

/*!
    @brief 型または変数のビットサイズを取得します。

    @param[in]  type 対象の型または変数
    @return     型または変数のビットサイズ
*/
#define NN_BITSIZEOF( type )

/*!
    @brief 文字列リテラルを作成します。

    @param[in]  text 文字列化するシンボル
    @return     文字列リテラル
*/
#define NN_MACRO_STRINGIZE( text )

/*!
    @brief シンボルを連結します。

    @param[in]    x 前から連結するシンボル
    @param[in]    y 後ろから連結するシンボル
    @return       連結されたシンボル
*/
#define NN_MACRO_CONCATENATE( x, y )

/*!
    @brief 重複しない一時シンボルを作成します。

    @param[in]  prefix 接頭辞
    @return     一時シンボル
*/
#define NN_MAKE_TEMPORARY_NAME( prefix )

/*!
    @brief switch 文において、case ラベルの処理が直後の case ラベルにフォールスルーすることを明示します。

*/
#define NN_FALL_THROUGH

/*!
    @brief switch 文において、default ラベルに決して到達しないことを表明します。
           default ラベルに到達した場合、実行を停止します。
*/
#define NN_UNEXPECTED_DEFAULT

/*!
    @brief クラス内部に暗黙にパディングが挿入されたことの警告を有効にします。
*/
#define NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS

/*!
    @brief クラス内部に、1 バイトのサイズを持つ無名のメンバを定義します。このメンバは、1 バイトアラインです。
*/
#define NN_PADDING1

//! @}

//----------------------------------------------------------------------------

//! @{
//! @name 警告対策

/*!
    @brief 意図した暗黙的呼び出し可能な変換コンストラクタ、変換演算子に対する静的解析ツール警告を抑制します。

    @details    使用例：
                @code
                class Example
                {
                public:
                    NN_IMPLICIT Example( int argument );
                };
                @endcode
*/
#define NN_IMPLICIT

/*!
    @brief 意図した未使用変数に対するコンパイラ警告を抑制します。
           文として使用します。

    @param[in]  variable 対象の変数
*/
#define NN_UNUSED( variable )

/*!
    @brief 意図した未使用変数に対するコンパイラ警告を抑制します。
           メンバ変数の宣言で使用します。

    @details 対象変数の宣言末尾で使用します。
*/
#define NN_IS_UNUSED_MEMBER

/*!
    @brief 意図した定数分岐を明示してコンパイラ警告を抑制します。

    @param[in]  condition 条件式
    @return     入力した条件式
*/
#define NN_STATIC_CONDITION( condition )

/*!
    @brief コンパイラの警告レベルを退避します。

    @details NN_PRAGMA_POP_WARNINGS と対で使用します。
*/
#define NN_PRAGMA_PUSH_WARNINGS

/*!
    @brief コンパイラの警告レベルを復帰します。

    @details NN_PRAGMA_PUSH_WARNINGS と対で使用します。
*/
#define NN_PRAGMA_POP_WARNINGS

/*!
    @brief 定数のオーバーフローに対するコンパイラの警告を無効化します。

    @details NN_PRAGMA_PUSH_WARNINGS と NN_PRAGMA_POP_WARNINGS の間で使用します。
*/
#define NN_DISABLE_WARNING_OVERFLOW

/*!
    @brief 関数内で宣言された静的変数に対するコンパイラの警告を無効化します。

    @details NN_PRAGMA_PUSH_WARNINGS と NN_PRAGMA_POP_WARNINGS の間で使用します。
*/
#define NN_DISABLE_WARNING_LOCAL_STATIC

/*!
    @brief 未使用引数に対するコンパイラの警告を無効化します。

    @details NN_PRAGMA_PUSH_WARNINGS と NN_PRAGMA_POP_WARNINGS の間で使用します。
*/
#define NN_DISABLE_WARNING_UNUSED_PARAMETER

/*!
    @brief 未使用変数に対するコンパイラの警告を無効化します。

    @details NN_PRAGMA_PUSH_WARNINGS と NN_PRAGMA_POP_WARNINGS の間で使用します。
*/
#define NN_DISABLE_WARNING_UNUSED_VAR

/*!
    @brief 未使用関数に対するコンパイラの警告を無効化します。

    @details NN_PRAGMA_PUSH_WARNINGS と NN_PRAGMA_POP_WARNINGS の間で使用します。
*/
#define NN_DISABLE_WARNING_UNUSED_FUNCTION

/*!
    @brief シンボルの隠蔽に対するコンパイラ警告を無効化します。

    @details NN_PRAGMA_PUSH_WARNINGS と NN_PRAGMA_POP_WARNINGS の間で使用します。
*/
#define NN_DISABLE_WARNING_SHADOW

/*!
    @brief 廃止予定シンボルに対するコンパイラ警告を無効化します。

    @details NN_PRAGMA_PUSH_WARNINGS と NN_PRAGMA_POP_WARNINGS の間で使用します。
*/
#define NN_DISABLE_WARNING_DEPRECATED_DECLARATIONS

/*!
    @brief 配列のサイズを返します。

    @param[in]  array 配列
*/
#define NN_ARRAY_SIZE( array )


//! @}

#endif  /* NN_BUILD_FOR_DOCUMENT_GENERATION */

// コンパイラ個別に定義されていない場合には空マクロとする
#if !defined(NN_NODISCARD)
    #define NN_NODISCARD
#endif

