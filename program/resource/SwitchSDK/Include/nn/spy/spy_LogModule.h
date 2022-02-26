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

#include <cstdarg>

#include <nn/nn_SdkAssert.h>

#include <nn/spy/spy_Config.h>
#include <nn/spy/spy_SpyDataInfo.h>
#include <nn/spy/spy_SpyModule.h>

namespace nn {
namespace spy {

class SpyController;

//! @brief  任意のログを Spy.exe に出力するモジュールです。
class LogModule : public SpyModule
{
public:
    //! ログメッセージの文字列長の最大値。
    static const int MaxMessageLength = 255;

#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

public:
    NN_FORCEINLINE LogModule() NN_NOEXCEPT {}

    NN_FORCEINLINE static bool Write(SpyController& controller, const char* message) NN_NOEXCEPT
    {
        NN_UNUSED(&controller);
        NN_UNUSED(message);

        return false;
    }

    NN_FORCEINLINE static bool WriteFormat(SpyController& controller, const char* format) NN_NOEXCEPT
    {
        NN_UNUSED(&controller);
        NN_UNUSED(format);

        return false;
    }

    template <typename T0>
    NN_FORCEINLINE static bool WriteFormat(SpyController& controller, const char* format, T0 a0) NN_NOEXCEPT
    {
        NN_UNUSED(&controller);
        NN_UNUSED(format);
        NN_UNUSED(a0);

        return false;
    }

    template <typename T0, typename T1>
    NN_FORCEINLINE static bool WriteFormat(SpyController& controller, const char* format, T0 a0, T1 a1) NN_NOEXCEPT
    {
        NN_UNUSED(&controller);
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);

        return false;
    }

    template <typename T0, typename T1, typename T2>
    NN_FORCEINLINE static bool WriteFormat(SpyController& controller, const char* format, T0 a0, T1 a1, T2 a2) NN_NOEXCEPT
    {
        NN_UNUSED(&controller);
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);

        return false;
    }

    template <typename T0, typename T1, typename T2, typename T3>
    NN_FORCEINLINE static bool WriteFormat(SpyController& controller, const char* format, T0 a0, T1 a1, T2 a2, T3 a3) NN_NOEXCEPT
    {
        NN_UNUSED(&controller);
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);
        NN_UNUSED(a3);

        return false;
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4>
    NN_FORCEINLINE static bool WriteFormat(SpyController& controller, const char* format, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) NN_NOEXCEPT
    {
        NN_UNUSED(&controller);
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);
        NN_UNUSED(a3);
        NN_UNUSED(a4);

        return false;
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    NN_FORCEINLINE static bool WriteFormat(SpyController& controller, const char* format, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) NN_NOEXCEPT
    {
        NN_UNUSED(&controller);
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);
        NN_UNUSED(a3);
        NN_UNUSED(a4);
        NN_UNUSED(a5);

        return false;
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    NN_FORCEINLINE static bool WriteFormat(SpyController& controller, const char* format, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) NN_NOEXCEPT
    {
        NN_UNUSED(&controller);
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);
        NN_UNUSED(a3);
        NN_UNUSED(a4);
        NN_UNUSED(a5);
        NN_UNUSED(a6);

        return false;
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    NN_FORCEINLINE static bool WriteFormat(SpyController& controller, const char* format, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) NN_NOEXCEPT
    {
        NN_UNUSED(&controller);
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);
        NN_UNUSED(a3);
        NN_UNUSED(a4);
        NN_UNUSED(a5);
        NN_UNUSED(a6);
        NN_UNUSED(a7);

        return false;
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    NN_FORCEINLINE static bool WriteFormat(SpyController& controller, const char* format, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) NN_NOEXCEPT
    {
        NN_UNUSED(&controller);
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);
        NN_UNUSED(a3);
        NN_UNUSED(a4);
        NN_UNUSED(a5);
        NN_UNUSED(a6);
        NN_UNUSED(a7);
        NN_UNUSED(a8);

        return false;
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    NN_FORCEINLINE static bool WriteFormat(SpyController& controller, const char* format, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9) NN_NOEXCEPT
    {
        NN_UNUSED(&controller);
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);
        NN_UNUSED(a3);
        NN_UNUSED(a4);
        NN_UNUSED(a5);
        NN_UNUSED(a6);
        NN_UNUSED(a7);
        NN_UNUSED(a8);
        NN_UNUSED(a9);

        return false;
    }

    NN_FORCEINLINE static bool WriteFormatV(SpyController& controller, const char* format, va_list argList) NN_NOEXCEPT
    {
        NN_UNUSED(&controller);
        NN_UNUSED(format);
        NN_UNUSED(argList);

        return false;
    }

    NN_FORCEINLINE bool Write(const char* message) NN_NOEXCEPT
    {
        NN_UNUSED(message);

        return false;
    }

    NN_FORCEINLINE bool WriteFormat(const char* format) NN_NOEXCEPT
    {
        NN_UNUSED(format);

        return false;
    }

    template <typename T0>
    NN_FORCEINLINE bool WriteFormat(const char* format, T0 a0) NN_NOEXCEPT
    {
        NN_UNUSED(format);
        NN_UNUSED(a0);

        return false;
    }

    template <typename T0, typename T1>
    NN_FORCEINLINE bool WriteFormat(const char* format, T0 a0, T1 a1) NN_NOEXCEPT
    {
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);

        return false;
    }

    template <typename T0, typename T1, typename T2>
    NN_FORCEINLINE bool WriteFormat(const char* format, T0 a0, T1 a1, T2 a2) NN_NOEXCEPT
    {
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);

        return false;
    }

    template <typename T0, typename T1, typename T2, typename T3>
    NN_FORCEINLINE bool WriteFormat(const char* format, T0 a0, T1 a1, T2 a2, T3 a3) NN_NOEXCEPT
    {
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);
        NN_UNUSED(a3);

        return false;
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4>
    NN_FORCEINLINE bool WriteFormat(const char* format, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) NN_NOEXCEPT
    {
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);
        NN_UNUSED(a3);
        NN_UNUSED(a4);

        return false;
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    NN_FORCEINLINE bool WriteFormat(const char* format, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) NN_NOEXCEPT
    {
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);
        NN_UNUSED(a3);
        NN_UNUSED(a4);
        NN_UNUSED(a5);

        return false;
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    NN_FORCEINLINE bool WriteFormat(const char* format, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) NN_NOEXCEPT
    {
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);
        NN_UNUSED(a3);
        NN_UNUSED(a4);
        NN_UNUSED(a5);
        NN_UNUSED(a6);

        return false;
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    NN_FORCEINLINE bool WriteFormat(const char* format, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) NN_NOEXCEPT
    {
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);
        NN_UNUSED(a3);
        NN_UNUSED(a4);
        NN_UNUSED(a5);
        NN_UNUSED(a6);
        NN_UNUSED(a7);

        return false;
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    NN_FORCEINLINE bool WriteFormat(const char* format, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) NN_NOEXCEPT
    {
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);
        NN_UNUSED(a3);
        NN_UNUSED(a4);
        NN_UNUSED(a5);
        NN_UNUSED(a6);
        NN_UNUSED(a7);
        NN_UNUSED(a8);

        return false;
    }

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    NN_FORCEINLINE bool WriteFormat(const char* format, T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9) NN_NOEXCEPT
    {
        NN_UNUSED(format);
        NN_UNUSED(a0);
        NN_UNUSED(a1);
        NN_UNUSED(a2);
        NN_UNUSED(a3);
        NN_UNUSED(a4);
        NN_UNUSED(a5);
        NN_UNUSED(a6);
        NN_UNUSED(a7);
        NN_UNUSED(a8);
        NN_UNUSED(a9);

        return false;
    }

    NN_FORCEINLINE bool WriteFormatV(const char* format, va_list argList) NN_NOEXCEPT
    {
        NN_UNUSED(format);
        NN_UNUSED(argList);

        return false;
    }

#else // NN_BUILD_CONFIG_SPY_ENABLED

public:
    //! @brief  コンストラクタです。
    LogModule() NN_NOEXCEPT;

    //! @brief  指定 SpyController にログ出力します。
    //!
    //! @param[in]  controller  出力先 SpyController
    //! @param[in]  message     ログメッセージ
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    static bool Write(SpyController& controller, const char* message) NN_NOEXCEPT;

    //! @brief  指定 SpyController にログ出力します。
    //!
    //! @param[in]  controller  出力先 SpyController
    //! @param[in]  format      ログメッセージの書式
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    static bool WriteFormat(SpyController& controller, const char* format) NN_NOEXCEPT
    {
        return WriteFormatImpl(controller, format);
    }

    //! @brief  指定 SpyController にログ出力します。
    //!
    //! @tparam T  引数 value のデータ型
    //!
    //! @param[in]  controller  出力先 SpyController
    //! @param[in]  format      ログメッセージの書式
    //! @param[in]  value       書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T>
    static bool WriteFormat(SpyController& controller, const char* format, T value) NN_NOEXCEPT
    {
        return WriteFormatImpl(controller, format, value);
    }

    //! @brief  指定 SpyController にログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //!
    //! @param[in]  controller  出力先 SpyController
    //! @param[in]  format      ログメッセージの書式
    //! @param[in]  value0      書式に対応する引数
    //! @param[in]  value1      書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1>
    static bool WriteFormat(SpyController& controller, const char* format, T0 value0, T1 value1) NN_NOEXCEPT
    {
        return WriteFormatImpl(controller, format, value0, value1);
    }

    //! @brief  指定 SpyController にログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //!
    //! @param[in]  controller  出力先 SpyController
    //! @param[in]  format      ログメッセージの書式
    //! @param[in]  value0      書式に対応する引数
    //! @param[in]  value1      書式に対応する引数
    //! @param[in]  value2      書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2>
    static bool WriteFormat(SpyController& controller, const char* format, T0 value0, T1 value1, T2 value2) NN_NOEXCEPT
    {
        return WriteFormatImpl(controller, format, value0, value1, value2);
    }

    //! @brief  指定 SpyController にログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //! @tparam T3  引数 value3 のデータ型
    //!
    //! @param[in]  controller  出力先 SpyController
    //! @param[in]  format      ログメッセージの書式
    //! @param[in]  value0      書式に対応する引数
    //! @param[in]  value1      書式に対応する引数
    //! @param[in]  value2      書式に対応する引数
    //! @param[in]  value3      書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2, typename T3>
    static bool WriteFormat(SpyController& controller, const char* format, T0 value0, T1 value1, T2 value2, T3 value3) NN_NOEXCEPT
    {
        return WriteFormatImpl(controller, format, value0, value1, value2, value3);
    }

    //! @brief  指定 SpyController にログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //! @tparam T3  引数 value3 のデータ型
    //! @tparam T4  引数 value4 のデータ型
    //!
    //! @param[in]  controller  出力先 SpyController
    //! @param[in]  format      ログメッセージの書式
    //! @param[in]  value0      書式に対応する引数
    //! @param[in]  value1      書式に対応する引数
    //! @param[in]  value2      書式に対応する引数
    //! @param[in]  value3      書式に対応する引数
    //! @param[in]  value4      書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2, typename T3, typename T4>
    static bool WriteFormat(SpyController& controller, const char* format, T0 value0, T1 value1, T2 value2, T3 value3, T4 value4) NN_NOEXCEPT
    {
        return WriteFormatImpl(controller, format, value0, value1, value2, value3, value4);
    }

    //! @brief  指定 SpyController にログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //! @tparam T3  引数 value3 のデータ型
    //! @tparam T4  引数 value4 のデータ型
    //! @tparam T5  引数 value5 のデータ型
    //!
    //! @param[in]  controller  出力先 SpyController
    //! @param[in]  format      ログメッセージの書式
    //! @param[in]  value0      書式に対応する引数
    //! @param[in]  value1      書式に対応する引数
    //! @param[in]  value2      書式に対応する引数
    //! @param[in]  value3      書式に対応する引数
    //! @param[in]  value4      書式に対応する引数
    //! @param[in]  value5      書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    static bool WriteFormat(
        SpyController& controller,
        const char* format,
        T0 value0,
        T1 value1,
        T2 value2,
        T3 value3,
        T4 value4,
        T5 value5) NN_NOEXCEPT
    {
        return WriteFormatImpl(
            controller,
            format,
            value0,
            value1,
            value2,
            value3,
            value4,
            value5);
    }

    //! @brief  指定 SpyController にログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //! @tparam T3  引数 value3 のデータ型
    //! @tparam T4  引数 value4 のデータ型
    //! @tparam T5  引数 value5 のデータ型
    //! @tparam T6  引数 value6 のデータ型
    //!
    //! @param[in]  controller  出力先 SpyController
    //! @param[in]  format      ログメッセージの書式
    //! @param[in]  value0      書式に対応する引数
    //! @param[in]  value1      書式に対応する引数
    //! @param[in]  value2      書式に対応する引数
    //! @param[in]  value3      書式に対応する引数
    //! @param[in]  value4      書式に対応する引数
    //! @param[in]  value5      書式に対応する引数
    //! @param[in]  value6      書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    static bool WriteFormat(
        SpyController& controller,
        const char* format,
        T0 value0,
        T1 value1,
        T2 value2,
        T3 value3,
        T4 value4,
        T5 value5,
        T6 value6) NN_NOEXCEPT
    {
        return WriteFormatImpl(
            controller,
            format,
            value0,
            value1,
            value2,
            value3,
            value4,
            value5,
            value6);
    }

    //! @brief  指定 SpyController にログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //! @tparam T3  引数 value3 のデータ型
    //! @tparam T4  引数 value4 のデータ型
    //! @tparam T5  引数 value5 のデータ型
    //! @tparam T6  引数 value6 のデータ型
    //! @tparam T7  引数 value7 のデータ型
    //!
    //! @param[in]  controller  出力先 SpyController
    //! @param[in]  format      ログメッセージの書式
    //! @param[in]  value0      書式に対応する引数
    //! @param[in]  value1      書式に対応する引数
    //! @param[in]  value2      書式に対応する引数
    //! @param[in]  value3      書式に対応する引数
    //! @param[in]  value4      書式に対応する引数
    //! @param[in]  value5      書式に対応する引数
    //! @param[in]  value6      書式に対応する引数
    //! @param[in]  value7      書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    static bool WriteFormat(
        SpyController& controller,
        const char* format,
        T0 value0,
        T1 value1,
        T2 value2,
        T3 value3,
        T4 value4,
        T5 value5,
        T6 value6,
        T7 value7) NN_NOEXCEPT
    {
        return WriteFormatImpl(
            controller,
            format,
            value0,
            value1,
            value2,
            value3,
            value4,
            value5,
            value6,
            value7);
    }

    //! @brief  指定 SpyController にログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //! @tparam T3  引数 value3 のデータ型
    //! @tparam T4  引数 value4 のデータ型
    //! @tparam T5  引数 value5 のデータ型
    //! @tparam T6  引数 value6 のデータ型
    //! @tparam T7  引数 value7 のデータ型
    //! @tparam T8  引数 value8 のデータ型
    //!
    //! @param[in]  controller  出力先 SpyController
    //! @param[in]  format      ログメッセージの書式
    //! @param[in]  value0      書式に対応する引数
    //! @param[in]  value1      書式に対応する引数
    //! @param[in]  value2      書式に対応する引数
    //! @param[in]  value3      書式に対応する引数
    //! @param[in]  value4      書式に対応する引数
    //! @param[in]  value5      書式に対応する引数
    //! @param[in]  value6      書式に対応する引数
    //! @param[in]  value7      書式に対応する引数
    //! @param[in]  value8      書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    static bool WriteFormat(
        SpyController& controller,
        const char* format,
        T0 value0,
        T1 value1,
        T2 value2,
        T3 value3,
        T4 value4,
        T5 value5,
        T6 value6,
        T7 value7,
        T8 value8) NN_NOEXCEPT
    {
        return WriteFormatImpl(controller, format, value0, value1, value2, value3, value4, value5, value6, value7, value8);
    }

    //! @brief  指定 SpyController にログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //! @tparam T3  引数 value3 のデータ型
    //! @tparam T4  引数 value4 のデータ型
    //! @tparam T5  引数 value5 のデータ型
    //! @tparam T6  引数 value6 のデータ型
    //! @tparam T7  引数 value7 のデータ型
    //! @tparam T8  引数 value8 のデータ型
    //! @tparam T9  引数 value9 のデータ型
    //!
    //! @param[in]  controller  出力先 SpyController
    //! @param[in]  format      ログメッセージの書式
    //! @param[in]  value0      書式に対応する引数
    //! @param[in]  value1      書式に対応する引数
    //! @param[in]  value2      書式に対応する引数
    //! @param[in]  value3      書式に対応する引数
    //! @param[in]  value4      書式に対応する引数
    //! @param[in]  value5      書式に対応する引数
    //! @param[in]  value6      書式に対応する引数
    //! @param[in]  value7      書式に対応する引数
    //! @param[in]  value8      書式に対応する引数
    //! @param[in]  value9      書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    static bool WriteFormat(SpyController& controller, const char* format, T0 value0, T1 value1, T2 value2, T3 value3, T4 value4, T5 value5, T6 value6, T7 value7, T8 value8, T9 value9) NN_NOEXCEPT
    {
        return WriteFormatImpl(controller, format, value0, value1, value2, value3, value4, value5, value6, value7, value8, value9);
    }

    //! @brief  指定 SpyController にログ出力します。
    //!
    //! @param[in]  controller  出力先 SpyController
    //! @param[in]  format      ログメッセージの書式
    //! @param[in]  argList     書式に対応する引数リスト
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    static bool WriteFormatV(SpyController& controller, const char* format, va_list argList) NN_NOEXCEPT;

    //! @brief  ログ出力します。
    //!
    //! @param[in]  message  ログメッセージ
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    bool Write(const char* message) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(message);

        if(!IsRegistered())
        {
            return false;
        }

        return LogModule::Write(*GetController(), message);
    }

    //! @brief  ログ出力します。
    //!
    //! @param[in]  format  ログメッセージの書式
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    bool WriteFormat(const char* format) NN_NOEXCEPT
    {
        return WriteFormatImpl(format);
    }

    //! @brief  ログ出力します。
    //!
    //! @tparam T  引数 value のデータ型
    //!
    //! @param[in]  format  ログメッセージの書式
    //! @param[in]  value   書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T>
    bool WriteFormat(const char* format, T value) NN_NOEXCEPT
    {
        return WriteFormatImpl(format, value);
    }

    //! @brief  ログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //!
    //! @param[in]  format  ログメッセージの書式
    //! @param[in]  value0  書式に対応する引数
    //! @param[in]  value1  書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1>
    bool WriteFormat(const char* format, T0 value0, T1 value1) NN_NOEXCEPT
    {
        return WriteFormatImpl(format, value0, value1);
    }

    //! @brief  ログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //!
    //! @param[in]  format  ログメッセージの書式
    //! @param[in]  value0  書式に対応する引数
    //! @param[in]  value1  書式に対応する引数
    //! @param[in]  value2  書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2>
    bool WriteFormat(const char* format, T0 value0, T1 value1, T2 value2) NN_NOEXCEPT
    {
        return WriteFormatImpl(format, value0, value1, value2);
    }

    //! @brief  ログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //! @tparam T3  引数 value3 のデータ型
    //!
    //! @param[in]  format  ログメッセージの書式
    //! @param[in]  value0  書式に対応する引数
    //! @param[in]  value1  書式に対応する引数
    //! @param[in]  value2  書式に対応する引数
    //! @param[in]  value3  書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2, typename T3>
    bool WriteFormat(const char* format, T0 value0, T1 value1, T2 value2, T3 value3) NN_NOEXCEPT
    {
        return WriteFormatImpl(format, value0, value1, value2, value3);
    }

    //! @brief  ログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //! @tparam T3  引数 value3 のデータ型
    //! @tparam T4  引数 value4 のデータ型
    //!
    //! @param[in]  format  ログメッセージの書式
    //! @param[in]  value0  書式に対応する引数
    //! @param[in]  value1  書式に対応する引数
    //! @param[in]  value2  書式に対応する引数
    //! @param[in]  value3  書式に対応する引数
    //! @param[in]  value4  書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2, typename T3, typename T4>
    bool WriteFormat(const char* format, T0 value0, T1 value1, T2 value2, T3 value3, T4 value4) NN_NOEXCEPT
    {
        return WriteFormatImpl(format, value0, value1, value2, value3, value4);
    }

    //! @brief  ログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //! @tparam T3  引数 value3 のデータ型
    //! @tparam T4  引数 value4 のデータ型
    //! @tparam T5  引数 value5 のデータ型
    //!
    //! @param[in]  format  ログメッセージの書式
    //! @param[in]  value0  書式に対応する引数
    //! @param[in]  value1  書式に対応する引数
    //! @param[in]  value2  書式に対応する引数
    //! @param[in]  value3  書式に対応する引数
    //! @param[in]  value4  書式に対応する引数
    //! @param[in]  value5  書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    bool WriteFormat(const char* format, T0 value0, T1 value1, T2 value2, T3 value3, T4 value4, T5 value5) NN_NOEXCEPT
    {
        return WriteFormatImpl(format, value0, value1, value2, value3, value4, value5);
    }

    //! @brief  ログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //! @tparam T3  引数 value3 のデータ型
    //! @tparam T4  引数 value4 のデータ型
    //! @tparam T5  引数 value5 のデータ型
    //! @tparam T6  引数 value6 のデータ型
    //!
    //! @param[in]  format  ログメッセージの書式
    //! @param[in]  value0  書式に対応する引数
    //! @param[in]  value1  書式に対応する引数
    //! @param[in]  value2  書式に対応する引数
    //! @param[in]  value3  書式に対応する引数
    //! @param[in]  value4  書式に対応する引数
    //! @param[in]  value5  書式に対応する引数
    //! @param[in]  value6  書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    bool WriteFormat(const char* format, T0 value0, T1 value1, T2 value2, T3 value3, T4 value4, T5 value5, T6 value6) NN_NOEXCEPT
    {
        return WriteFormatImpl(format, value0, value1, value2, value3, value4, value5, value6);
    }

    //! @brief  ログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //! @tparam T3  引数 value3 のデータ型
    //! @tparam T4  引数 value4 のデータ型
    //! @tparam T5  引数 value5 のデータ型
    //! @tparam T6  引数 value6 のデータ型
    //! @tparam T7  引数 value7 のデータ型
    //!
    //! @param[in]  format  ログメッセージの書式
    //! @param[in]  value0  書式に対応する引数
    //! @param[in]  value1  書式に対応する引数
    //! @param[in]  value2  書式に対応する引数
    //! @param[in]  value3  書式に対応する引数
    //! @param[in]  value4  書式に対応する引数
    //! @param[in]  value5  書式に対応する引数
    //! @param[in]  value6  書式に対応する引数
    //! @param[in]  value7  書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    bool WriteFormat(const char* format, T0 value0, T1 value1, T2 value2, T3 value3, T4 value4, T5 value5, T6 value6, T7 value7) NN_NOEXCEPT
    {
        return WriteFormatImpl(format, value0, value1, value2, value3, value4, value5, value6, value7);
    }

    //! @brief  ログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //! @tparam T3  引数 value3 のデータ型
    //! @tparam T4  引数 value4 のデータ型
    //! @tparam T5  引数 value5 のデータ型
    //! @tparam T6  引数 value6 のデータ型
    //! @tparam T7  引数 value7 のデータ型
    //! @tparam T8  引数 value8 のデータ型
    //!
    //! @param[in]  format  ログメッセージの書式
    //! @param[in]  value0  書式に対応する引数
    //! @param[in]  value1  書式に対応する引数
    //! @param[in]  value2  書式に対応する引数
    //! @param[in]  value3  書式に対応する引数
    //! @param[in]  value4  書式に対応する引数
    //! @param[in]  value5  書式に対応する引数
    //! @param[in]  value6  書式に対応する引数
    //! @param[in]  value7  書式に対応する引数
    //! @param[in]  value8  書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    bool WriteFormat(const char* format, T0 value0, T1 value1, T2 value2, T3 value3, T4 value4, T5 value5, T6 value6, T7 value7, T8 value8) NN_NOEXCEPT
    {
        return WriteFormatImpl(format, value0, value1, value2, value3, value4, value5, value6, value7, value8);
    }

    //! @brief  ログ出力します。
    //!
    //! @tparam T0  引数 value0 のデータ型
    //! @tparam T1  引数 value1 のデータ型
    //! @tparam T2  引数 value2 のデータ型
    //! @tparam T3  引数 value3 のデータ型
    //! @tparam T4  引数 value4 のデータ型
    //! @tparam T5  引数 value5 のデータ型
    //! @tparam T6  引数 value6 のデータ型
    //! @tparam T7  引数 value7 のデータ型
    //! @tparam T8  引数 value8 のデータ型
    //! @tparam T9  引数 value9 のデータ型
    //!
    //! @param[in]  format  ログメッセージの書式
    //! @param[in]  value0  書式に対応する引数
    //! @param[in]  value1  書式に対応する引数
    //! @param[in]  value2  書式に対応する引数
    //! @param[in]  value3  書式に対応する引数
    //! @param[in]  value4  書式に対応する引数
    //! @param[in]  value5  書式に対応する引数
    //! @param[in]  value6  書式に対応する引数
    //! @param[in]  value7  書式に対応する引数
    //! @param[in]  value8  書式に対応する引数
    //! @param[in]  value9  書式に対応する引数
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    bool WriteFormat(const char* format, T0 value0, T1 value1, T2 value2, T3 value3, T4 value4, T5 value5, T6 value6, T7 value7, T8 value8, T9 value9) NN_NOEXCEPT
    {
        return WriteFormatImpl(format, value0, value1, value2, value3, value4, value5, value6, value7, value8, value9);
    }

    //! @brief  ログ出力します。
    //!
    //! @param[in]  format   ログメッセージの書式
    //! @param[in]  argList  書式に対応する引数リスト
    //!
    //! @return     成功したら true, 失敗したら false を返します。
    bool WriteFormatV(const char* format, va_list argList) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL(format);

        if(!IsRegistered())
        {
            return false;
        }

        return LogModule::WriteFormatV(*GetController(), format, argList);
    }

private:
    static bool WriteFormatImpl(SpyController& controller, const char* format, ...) NN_NOEXCEPT;
    bool WriteFormatImpl(const char* format, ...) NN_NOEXCEPT;

#endif // NN_BUILD_CONFIG_SPY_ENABLED
};

} // namespace nn::spy
} // namespace nn
