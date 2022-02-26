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

/**
 * @file
 * @brief 性能モードと性能の定義。
 */

namespace nn { namespace oe {

/**
 * @brief   性能モードを表す列挙型です。
 *
 * @details ひとつもしくは幾つかの性能を選択可能な、プラットフォームの内部状態を性能モードと呼称します。
 */
enum PerformanceMode
{
    PerformanceMode_Invalid = -1, //!< 無効な性能モードです。

    /**
     * 通常の性能モードです。@n
     * この性能モードの性能の初期設定は @ref nn::oe::PerformanceConfiguration_Cpu1020MhzGpu307MhzEmc1331Mhz です。
     */
    PerformanceMode_Normal,

    /**
     * より高い性能を使用できる性能モードです。@n
     * この性能モードの性能の初期設定は @ref nn::oe::PerformanceConfiguration_Cpu1020MhzGpu768MhzEmc1600Mhz です。
     */
    PerformanceMode_Boost,
};

/**
 * @brief   選択可能な性能を表す型です。
 *
 * @details 選択可能な性能を表す型です。@n
 *          ローンチアプリで使用可能な性能は 3 種類です。@n
 *          ローンチ以降に使用可能な性能が追加される可能性があります。
 */
typedef int PerformanceConfiguration;

const PerformanceConfiguration PerformanceConfiguration_Invalid = 0x00000000; //!< 無効な設定です。

/**
 * CPU 1020.0 MHz, GPU 768.0 MHz, EMC 1600.0 MHz。@n
 * @ref nn::oe::PerformanceMode_Boost のみで使用可能です。
 */
const PerformanceConfiguration PerformanceConfiguration_Cpu1020MhzGpu768MhzEmc1600Mhz = 0x00010001;

/**
 * CPU 1020.0 MHz, GPU 307.2 MHz, EMC 1331.2 MHz。@n
 * 全ての性能モードで使用可能です。
 */
const PerformanceConfiguration PerformanceConfiguration_Cpu1020MhzGpu307MhzEmc1331Mhz = 0x00020003;

/**
 * CPU 1020.0 MHz, GPU 384.0 MHz, EMC 1331.2 MHz。@n
 * 電池消費量の大きい性能です。@n
 * 全ての性能モードで使用可能です。
 */
const PerformanceConfiguration PerformanceConfiguration_Cpu1020MhzGpu384MhzEmc1331Mhz = 0x00020004;

}} // namespace nn::oe
