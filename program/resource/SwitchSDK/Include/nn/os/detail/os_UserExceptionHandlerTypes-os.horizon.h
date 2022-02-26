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
 * @brief   ユーザ例外ハンドラに関する Horizon 環境用の型の宣言
 */

#pragma once

#include <nn/os/os_Config.h>

#include <nn/nn_Common.h>
#include <nn/os/os_Macro.h>
#include <nn/os/detail/os_MacroImpl.h>

//-----------------------------------------------------------------------------
//  C++ 向けの宣言
//-----------------------------------------------------------------------------

namespace nn { namespace os {

//-----------------------------------------------------------------------------
/**
 * @brief   ARMv7A 動作環境での例外情報の詳細を格納する構造体です。
 *
 * @details
 *  ARMv7A 動作環境において UserExceptionInfoDetail 型へと typedef されます。@n
 *  各メンバ変数の間には適度にパディングが入る可能性があるため、
 *  アクセスする場合には、個々のメンバ変数名を使ってアクセスして下さい。
 */
struct UserExceptionInfoDetailAarch32Status32
{
    // プロセッサ汎用レジスタ
    union
    {
        struct
        {
            Bit32   r[16];      //!< r0～r15 の値が格納されます
        };
        struct
        {
            Bit32   _padding[9];
            Bit32   sb;         //!< r[ 9] と等価です
            Bit32   sl;         //!< r[10] と等価です
            Bit32   fp;         //!< r[11] と等価です
            Bit32   ip;         //!< r[12] と等価です
            Bit32   sp;         //!< r[13] と等価です。スタックポインタです
            Bit32   lr;         //!< r[14] と等価です。リンクレジスタです
            Bit32   pc;         //!< r[15] と等価です。プログラムカウンタです
        };
    };

    // 浮動小数点レジスタ
    union
    {
        struct
        {
            Bit32  S[32]; //!< VFP の S0～S31 レジスタの値が格納されます
        };
        struct
        {
            Bit64  D[32]; //!< VFP の D0～D31 レジスタの値が格納されます
        };
        struct
        {
            Bit128 Q[16]; //!< Advanced SIMD の Q0～Q15 レジスタの値が格納されます
        };
    };

    // 例外情報
    Bit32   cpsr;               //!< cpsr の値が格納されます
    Bit32   fpscr;              //!< fpscr の値が格納されます
    Bit32   fsr;                //!< cpsr の値が格納されます
    Bit32   far;                //!< cpsr の値が格納されます
    Bit32   fpexc;              //!< FPEXC の値が格納されます
    Bit32   fpinst;             //!< FPINST の値が格納されます
    Bit32   fpinst2;            //!< FPINST2 の値が格納されます
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(UserExceptionInfoDetailAarch32Status32);

//-----------------------------------------------------------------------------
/**
 * @brief   ARMv8A, AArch32 動作環境での例外情報の詳細を格納する構造体です。
 *
 * @details ARMv8A, AArch32 動作環境において UserExceptionInfoDetail 型へと typedef されます。
 */
struct UserExceptionInfoDetailAarch32Status64
{
    // プロセッサ汎用レジスタ
    union
    {
        struct
        {
            Bit32   r[16];      //!< w0～w15 の値が格納されます
        };
        struct
        {
            Bit32   _padding[9];
            Bit32   sb;         //!< w[ 9] と等価です
            Bit32   sl;         //!< w[10] と等価です
            Bit32   fp;         //!< w[11] と等価です
            Bit32   ip;         //!< w[12] と等価です
            Bit32   sp;         //!< w[13] と等価です。スタックポインタです
            Bit32   lr;         //!< w[14] と等価です。リンクレジスタです
            Bit32   pc;         //!< w[15] と等価です。プログラムカウンタです
        };
    };

    // 浮動小数点レジスタ
    union
    {
        struct
        {
            Bit32  S[32]; //!< VFP の S0～S31 レジスタの値が格納されます
        };
        struct
        {
            Bit64  D[32]; //!< VFP の D0～D31 レジスタの値が格納されます
        };
        struct
        {
            Bit128 Q[16]; //!< Advanced SIMD の Q0～Q15 レジスタの値が格納されます
        };
    };

    // 例外情報
    Bit32   pstate; //!< pstate の値が格納されます。ビット配置は SPSR のフォーマットと同じです
    Bit32   fpscr;  //!< fpscr の値が格納されます
    Bit32   afsr0;  //!< AFSR0 の値が格納されます。
    Bit32   afsr1;  //!< AFSR1 の値が格納されます。
    Bit32   esr;    //!< ESR の値が格納されます。
    Bit32   far;    //!< FAR の値が格納されます。
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(UserExceptionInfoDetailAarch32Status64);

//-----------------------------------------------------------------------------
/**
 * @brief   ARMv8A, AArch64 動作環境での例外情報の詳細を格納する構造体です。
 *
 * @details ARMv8A, AArch64 動作環境において UserExceptionInfoDetail 型へと typedef されます。
 */
struct UserExceptionInfoDetailAarch64
{
    // プロセッサ汎用レジスタ
    union
    {
        struct
        {
            Bit64   r[31];      //!< x0～x30 の値が格納されます
        };
        struct
        {
            Bit64   _padding[29];
            Bit64   fp;         //!< x29 と等価です。
            Bit64   lr;         //!< x30 と等価です。リンクレジスタです
        };
    };
    Bit64   sp;                 //!< スタックポインタです
    Bit64   pc;                 //!< プログラムカウンタです

    // 浮動小数点レジスタ
    Bit128  V[32];  //!< SIMD の V0～V31 レジスタの値が格納されます

    // 例外情報
    Bit32   pstate; //!< pstate の値が格納されます。ビット配置は SPSR のフォーマットと同じです
    Bit32   fpcr;   //!< fpcr の値が格納されます
    Bit32   fpsr;   //!< fpsr の値が格納されます
    Bit32   afsr0;  //!< AFSR0 の値が格納されます。
    Bit32   afsr1;  //!< AFSR1 の値が格納されます。
    Bit32   esr;    //!< ESR の値が格納されます。
    Bit64   far;    //!< FAR の値が格納されます。
};

NN_OS_DETAIL_STATIC_ASSERT_TRIVIAL(UserExceptionInfoDetailAarch64);

#if defined( NN_OS_CPU_ARM_AARCH32_ARMV7A ) || \
    defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
typedef UserExceptionInfoDetailAarch32Status32  UserExceptionInfoDetail;
#endif

#if defined( NN_OS_CPU_ARM_AARCH32_ARMV8A ) || \
    defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
typedef UserExceptionInfoDetailAarch32Status64  UserExceptionInfoDetail;
#endif

#if defined( NN_OS_CPU_ARM_AARCH64_ARMV8A ) || \
    defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
typedef UserExceptionInfoDetailAarch64          UserExceptionInfoDetail;
#endif


//-----------------------------------------------------------------------------

}} // namespace nn::os

