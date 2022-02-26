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
 * @brief   仮想アドレスメモリ管理機能に関する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/os/os_Config.h>
#include <nn/os/os_VirtualAddressMemoryTypes.h>

namespace nn { namespace os {

//! @name 仮想アドレスメモリ管理関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   仮想アドレス空間からアドレス領域を確保します
 *
 * @param[out]  pOutAddress 確保したアドレス領域の先頭アドレスを格納する領域へのポインタ
 * @param[in]   size        確保するアドレス領域のサイズ
 *
 * @retresult
 *   @handleresult{nn::os::ResultOutOfVirtualAddressSpace,仮想アドレス空間不足によりアドレス領域の確保に失敗しました。}
 *   @handleresult{nn::os::ResultOutOfMemory,仮想アドレス空間管理に必要な物理メモリの確保に失敗しました。}
 * @endretresult
 *
 * @pre
 *  - nn::os::IsVirtualAddressMemoryEnabled() == true
 *  - pOutAddress が指す領域が書き込み可能
 *  - size > 0
 *  - size が nn::os::MemoryPageSize の整数倍である
 *
 * @post
 *  - サイズが size バイトのアドレス領域が確保され、その先頭アドレスが *pOutAddress に格納されている
 *  - *pOutAddress に格納されているアドレスが nn::os::AddressRegionAlignment でアライメントされている
 *
 * @details
 *  size バイトの新たなアドレス領域を仮想アドレス空間上に確保し、その先頭アドレスを返します。
 *  確保されるアドレス領域の先頭アドレスは nn::os::AddressRegionAlignment 定数が示す値でアライメントされます。
 *
 *  確保したアドレス領域にはまだ物理メモリが割り当てられていません。
 *  物理メモリが割り当てられていない状態でアドレス領域へメモリアクセスするとアクセス例外となります。
 *  確保したアドレス領域には nn::os::AllocateMemoryPages で物理メモリを割り当てることができます。
 *
 *  確保したアドレス領域は nn::os::FreeAddressRegion で解放することができます。
 *
 *  仮想アドレスメモリ管理機能ではアドレス領域の管理のために OS ライブラリ内部でメモリの動的確保を行っています。
 *  このため nn::os::AllocateAddressRegion() の呼び出し時に内部でメモリの動的確保が行われることがあります。
 *  OS ライブラリ内部でメモリの動的確保に失敗した場合には nn::os::ResultOutOfMemory が返ります。
 *
 *  OS ライブラリ内部で動的確保されたメモリは nn::os::QueryMemoryInfo() で取得できる値の totalUsedMemorySize に含まれます。
 *
 *  使用できる仮想アドレス空間は 63GiB です。
 *  管理上のオーバーヘッドが発生するため実際に使用できるアドレス領域の合計はこれよりも少なくなります。
 *
 *  この関数はスレッドセーフです。
 *
 *  この関数は nn::os::IsVirtualAddressMemoryEnabled() が true を返す場合にのみ使用可能です。
 *  詳細は nn::os::IsVirtualAddressMemoryEnabled() を参照してください。
 */
nn::Result  AllocateAddressRegion(uintptr_t* pOutAddress, size_t size)    NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   アドレス領域を解放します。
 *
 * @param[in]   address 解放するアドレス領域の先頭アドレス
 *
 * @retresult
 *   @handleresult{nn::os::ResultInvalidParameter,address が有効なアドレス領域の先頭アドレスではありません。}
 *   @handleresult{nn::os::ResultBusy,アドレス領域の中に使用中のメモリが含まれています。}
 *   @handleresult{nn::os::ResultOutOfResource,物理メモリ操作に必要なリソースが不足しました。}
 * @endretresult
 *
 * @pre
 *  - nn::os::IsVirtualAddressMemoryEnabled() == true
 *
 * @post
 *  - address を先頭アドレスとするアドレス領域全体に物理メモリが割り当てられていない
 *  - address を先頭アドレスとするアドレス領域が解放されている
 *
 * @details
 *  address を先頭アドレスとするアドレス領域を解放します。
 *  アドレス領域内に物理メモリが割り当てられた領域が存在する場合は
 *  アドレス領域の解放前にその割り当てを解除します。
 *
 *  address は nn::os::AllocateAddressRegion
 *  で確保され解放されていないアドレス領域の先頭アドレスでなければなりません。
 *  また、対象となるアドレス領域には SDK の API で使用中のメモリが含まれていてはいけません。
 *
 *  この関数はスレッドセーフです。
 *
 *  この関数は nn::os::IsVirtualAddressMemoryEnabled() が true を返す場合にのみ使用可能です。
 *  詳細は nn::os::IsVirtualAddressMemoryEnabled() を参照してください。
 */
nn::Result  FreeAddressRegion(uintptr_t address) NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   指定の領域に物理メモリを割り当てます
 *
 * @param[in]   address 物理メモリを割り当てる領域の先頭アドレス
 * @param[in]   size    物理メモリを割り当てる領域のサイズ
 *
 * @retresult
 *   @handleresult{nn::os::ResultInvalidParameter,address から size バイトの領域が有効なアドレス領域に含まれていません。}
 *   @handleresult{nn::os::ResultOutOfMemory,割り当てる物理メモリの確保に失敗しました。}
 *   @handleresult{nn::os::ResultOutOfResource,物理メモリ操作に必要なリソースが不足しました。}
 * @endretresult
 *
 * @pre
 *  - nn::os::IsVirtualAddressMemoryEnabled() == true
 *  - address が nn::os::MemoryPageSize の整数倍である
 *  - size が nn::os::MemoryPageSize の整数倍である
 *
 * @post
 *  - address から size バイトの領域に物理メモリが割り当てられている
 *
 * @details
 *  address から size バイトの領域に対して、物理メモリを確保して割り当てを行います。
 *
 *  address から size バイトの領域は nn::os::AllocateAddressRegion
 *  で確保され解放されていないアドレス領域に含まれていなければなりません。
 *
 *  address から size バイトの領域に既に物理メモリが割り当てられた領域が存在する場合、
 *  その領域は関数呼び出し前の状態が維持されます。
 *
 *  この関数で割り当てられた物理メモリの割り当てを解除するには nn::os::FreeMemoryPages を使用します。
 *
 *  この関数はスレッドセーフです。
 *
 *  この関数は nn::os::IsVirtualAddressMemoryEnabled() が true を返す場合にのみ使用可能です。
 *  詳細は nn::os::IsVirtualAddressMemoryEnabled() を参照してください。
 */
nn::Result  AllocateMemoryPages(uintptr_t address, size_t size)    NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   指定の領域へのメモリの割り当てを解除します。
 *
 * @param[in]   address メモリの割り当てを解除する領域の先頭アドレス
 * @param[in]   size    メモリの割り当てを解除する領域のサイズ
 *
 * @retresult
 *   @handleresult{nn::os::ResultInvalidParameter,address から size バイトの領域が有効なアドレス領域に含まれていません。}
 *   @handleresult{nn::os::ResultBusy,アドレス領域の中に使用中のメモリが含まれています。}
 *   @handleresult{nn::os::ResultOutOfResource,物理メモリ操作に必要なリソースが不足しました。}
 * @endretresult
 *
 * @pre
 *  - nn::os::IsVirtualAddressMemoryEnabled() == true
 *  - address が nn::os::MemoryPageSize の整数倍である
 *  - size が nn::os::MemoryPageSize の整数倍である
 *
 * @post
 *  - address から size バイトの領域に物理メモリが割り当てられていない
 *
 * @details
 *  address から size バイトの領域への物理メモリの割り当てを解除します。
 *
 *  address と size は、AllocateMemoryPages 呼び出し時と一致している必要はなく、
 *  割り当て済みの物理メモリを部分的に解除することができます。
 *
 *  address から size バイトの領域は nn::os::AllocateAddressRegion
 *  で確保され解放されていないアドレス領域に含まれていなければなりません。
 *  また、この領域には SDK の API で使用中のメモリが含まれてはいてはいけません。
 *
 *  address から size バイトの領域に含まれる物理メモリが割り当てられていない領域は無視されます。
 *
 *  物理メモリが割り当てられていないアドレスへメモリアクセスするとアクセス例外となります。
 *
 *  この関数はスレッドセーフです。
 *
 *  この関数は nn::os::IsVirtualAddressMemoryEnabled() が true を返す場合にのみ使用可能です。
 *  詳細は nn::os::IsVirtualAddressMemoryEnabled() を参照してください。
 */
nn::Result  FreeMemoryPages(uintptr_t address, size_t size)  NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   仮想アドレスメモリ管理用のリソース消費量を取得します。
 *
 * @return  取得したリソース消費量を返します。
 *
 * @details
 *  この関数はスレッドセーフです。
 *
 * @platformbegin{Windows}
 *  取得できる値は assignedSize も usedSize も
 *  常に 0 となります。
 * @platformend
 *
 * @platformbegin{NX}
 *  取得できる値の assignedSize は
 *  .nmeta ファイルで SystemResourceSize に設定した値になります。
 *  この値はプログラムの実行中に変化することはありません。
 *  @n
 *  @n
 *  取得できる値の usedSize は、これまでに消費されたリソース量が格納されます。
 *  一度消費されたリソースは解放されることがないためプログラムの実行中に
 *  この値は単調増加します。
 *  この値が assignedSize と同じ値になっていると仮想アドレスメモリ管理関連 API が
 *  nn::os::ResultOutOfResource で失敗する可能性があります。
 * @platformend
 */
VirtualAddressMemoryResourceUsage GetVirtualAddressMemoryResourceUsage() NN_NOEXCEPT;

//--------------------------------------------------------------------------
/**
 * @brief   仮想アドレスメモリ管理が使用可能になっているかを判定します。
 *
 * @return  仮想アドレスメモリ管理が使用可能になっていれば true、そうでなければ false が返ります。
 *
 * @details
 *  32bit プログラムでは仮想アドレスメモリ管理は使用できません。そのため常に false が返ります。
 *
 *  この関数はスレッドセーフです。
 *
 * @platformbegin{Windows}
 *  64bit プログラムでは仮想アドレスメモリ管理は常に使用可能です。
 * @platformend
 *
 * @platformbegin{NX}
 *  64bit プログラムで仮想アドレスメモリ管理を使用可能とするためには
 *  .nmeta ファイルで SystemResourceSize に 0 より大きい値を設定する必要があります。
 *  詳細は @confluencelink{83956147, 「SystemResourceSize の値の決定」} を参照してください。
 * @platformend
 */
bool IsVirtualAddressMemoryEnabled() NN_NOEXCEPT;


//! @}

}} // namespace nn::os

