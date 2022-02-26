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
 * @brief   NumberLineAllocator に関するヘッダファイル
 * @details NumberLineAllocator クラスの定義
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/os.h>
#include <nn/mem/detail/mem_Log.h>

namespace nn { namespace mem {

typedef int errno_t;
class NumberLineAllocatorPrv;

/**
* @brief   メモリ確保用のコールバック関数の型です。
*
* @param[in]   size    管理領域の確保で必要なサイズ
* @param[in]   pParam  ユーザから渡されるポインタ
*
* @details
*  NumberLineAllocator の管理領域の確保に用いられます。
*
*/
typedef void* (*MallocCallback)(size_t size, void* pParam);

/**
* @brief   メモリ解放用のコールバック関数の型です。
*
* @param[in]   ptr     確保済み管理領域のポインタ
* @param[in]   pParam  ユーザから渡されるポインタ
*
* @details
*  NumberLineAllocator の管理領域の解放に用いられます。
*/
typedef void(*FreeCallback)(void* ptr, void* pParam);

/**
 * @brief   管理領域と確保・解放領域を分離して持つことのできる整数アロケータのクラスです。
 *
 * @details
 *  nn::mem::NumberLineAllocator は 0 から最大 0xFFFFFF までのインデックスで表される整数領域から、任意の領域の割り当てを行うことのできるクラスです。@n
 *  NumberLineAllocator の管理領域は専用のコールバックを用いて管理し、 NumberLineAllocator で扱う領域と分離できます。
 */
class NumberLineAllocator
{
    NN_DISALLOW_COPY(NumberLineAllocator);
    NN_DISALLOW_MOVE(NumberLineAllocator);

public:

    /**
     * @brief   確保・解放に関するモードです。
     *
     * @details
     *  区間の確保・解放時の挙動を指定します。@n
     *  デフォルトでは AllocationMode_NoOption が指定されています。@n
     */
    enum AllocationMode
    {
        AllocationMode_NoOption             = 0,    //!< 長さ 256 以上の区間が解放された場合、隣接する空き領域と領域を結合します
        AllocationMode_CoalesceAdjacentArea  = 1    //!< 解放時に隣接する空き領域と常に結合します。 AllocationMode_NoOption に比べ実行効率が劣りますが、アロケータ内の断片化を抑えられる可能性があります
    };

    /**
     *  @brief 確保された領域を解放するときに呼び出されるコールバック関数の型です。
     *
     *  @param[in]  userPtr ユーザから渡されるポインタ
     *
     *  @details
     *   Alloc() 時にこのコールバックを引数として渡した場合、確保した領域が Free() される際に呼び出されます。
     *
     */
    typedef void(*UserCallback) (void *userPtr);

    //! @name コンストラクタ・デストラクタ
    //! @{

    /**
     * @brief   コンストラクタです。
     *
     * @details
     *  このコンストラクタでは初期化を行わないため、別途 Initialize() を呼ぶ必要があります。@n
     *  @n
     *  本 API はスレッドセーフではありません。
     *
     */
    NumberLineAllocator() NN_NOEXCEPT : m_pNumberLine(NULL), m_Mutex(false), m_IsThreadSafe(false)
    {
    }

    /**
     * @brief   コンストラクタです。
     *
     * @param[in]   mallocFunc      管理領域の確保用コールバック関数
     * @param[in]   pMallocParam    管理領域の確保時に呼ばれるユーザ定義のパラメータ
     * @param[in]   freeFunc        管理領域の解放用コールバック関数
     * @param[in]   pFreeParam      管理領域の解放時に呼ばれるユーザ定義のパラメータ
     *
     * @post    アロケータは初期化済みとなります。
     *
     * @details
     *  NumberLineAllocator を初期化します。@n
     *  mallocFunc 呼び出し時のユーザ定義のパラメータとして、 pMallocParam が渡されます。@n
     *  freeFunc 呼び出し時のユーザ定義のパラメータとして、 pFreeParam が渡されます。@n
     *  @n
     *  本 API はスレッドセーフではありません。
     *
     */
    NumberLineAllocator(MallocCallback mallocFunc, void* pMallocParam, FreeCallback freeFunc, void* pFreeParam) NN_NOEXCEPT : m_pNumberLine(NULL), m_Mutex(false), m_IsThreadSafe(false)
    {
        Initialize(mallocFunc, pMallocParam, freeFunc, pFreeParam);
    }

    /**
     * @brief   コンストラクタです。
     *
     * @param[in]   mallocFunc      管理領域の確保用コールバック関数
     * @param[in]   pMallocParam    管理領域の確保時に呼ばれるユーザ定義のパラメータ
     * @param[in]   freeFunc        管理領域の解放用コールバック関数
     * @param[in]   pFreeParam      管理領域の解放時に呼ばれるユーザ定義のパラメータ
     * @param[in]   isThreadSafe    アロケータの各機能をスレッドセーフで呼び出すようにするかどうかのフラグ
     *
     * @post    アロケータは初期化済みとなります。
     *
     * @details
     *  NumberLineAllocator を初期化します。@n
     *  mallocFunc 呼び出し時のユーザ定義のパラメータとして、 pMallocParam が渡されます。@n
     *  freeFunc 呼び出し時のユーザ定義のパラメータとして、 pFreeParam が渡されます。@n
     *  @n
     *  本 API はスレッドセーフではありません。
     *
     */
    NumberLineAllocator(MallocCallback mallocFunc, void* pMallocParam, FreeCallback freeFunc, void* pFreeParam, bool isThreadSafe) NN_NOEXCEPT : m_pNumberLine(NULL), m_Mutex(false), m_IsThreadSafe(isThreadSafe)
    {
        Initialize(mallocFunc, pMallocParam, freeFunc, pFreeParam);
    }

    //! @}

    //! @name 初期化・終了処理
    //! @{

    /**
     * @brief   アロケータを初期化します。
     *
     * @param[in]   mallocFunc      管理領域の確保用コールバック関数
     * @param[in]   pMallocParam    管理領域の確保時に呼ばれるユーザ定義のパラメータ
     * @param[in]   freeFunc        管理領域の解放用コールバック関数
     * @param[in]   pFreeParam      管理領域の解放時に呼ばれるユーザ定義のパラメータ
     *
     * @pre     アロケータが未初期化である必要があります。
     * @post    アロケータは初期化済みとなります。
     *
     * @details
     *  NumberLineAllocator を初期化します。@n
     *  mallocFunc 呼び出し時のユーザ定義のパラメータとして、 pMallocParam が渡されます。@n
     *  freeFunc 呼び出し時のユーザ定義のパラメータとして、 pFreeParam が渡されます。@n
     *  Initialize() 後、 AddRange() を呼び出して確保可能領域を設定してください。@n
     *  @n
     *  本 API はスレッドセーフではありません。@n
     *  本 API の動作中にアロケータに対する操作は行わないでください。
     *
     */
    void Initialize(MallocCallback mallocFunc, void* pMallocParam, FreeCallback freeFunc, void* pFreeParam) NN_NOEXCEPT;

    /**
     * @brief   アロケータを初期化します。
     *
     * @param[in]   mallocFunc      管理領域の確保用コールバック関数
     * @param[in]   pMallocParam    管理領域の確保時に呼ばれるユーザ定義のパラメータ
     * @param[in]   freeFunc        管理領域の解放用コールバック関数
     * @param[in]   pFreeParam      管理領域の解放時に呼ばれるユーザ定義のパラメータ
     * @param[in]   isThreadSafe    アロケータの各機能をスレッドセーフで呼び出すようにするかどうかのフラグ
     *
     * @pre     アロケータが未初期化である必要があります。
     * @post    アロケータは初期化済みとなります。
     *
     * @details
     *  NumberLineAllocator を初期化します。@n
     *  mallocFunc 呼び出し時のユーザ定義のパラメータとして、 pMallocParam が渡されます。@n
     *  freeFunc 呼び出し時のユーザ定義のパラメータとして、 pFreeParam が渡されます。@n
     *  Initialize() 後、 AddRange() を呼び出して確保可能領域を設定してください。@n
     *  @n
     *  本 API はスレッドセーフではありません。@n
     *  本 API の動作中にアロケータに対する操作は行わないでください。
     *
     */
    void Initialize(MallocCallback mallocFunc, void* pMallocParam, FreeCallback freeFunc, void* pFreeParam, bool isThreadSafe) NN_NOEXCEPT;

    /**
     * @brief   アロケータを破棄します。
     *
     * @pre     アロケータが初期化されている必要があります。
     * @post    アロケータは未初期化となります。
     *
     * @details
     *  アロケータを破棄します。@n
     *  本 API はスレッドセーフではありません。@n
     *  本 API の動作中にアロケータに対する操作は行わないでください。
     *
     */
    void Finalize() NN_NOEXCEPT;

    //! @}

    //! @name 確保・解放関連 API
    //! @{

    /**
     * @brief   指定した領域を確保可能にします。
     *
     * @param[in]   first   確保可能にしたい範囲の先頭インデックス
     * @param[in]   last    確保可能にしたい範囲の末尾インデックス
     *
     * @return  確保可能に設定できた場合は true を、そうでない場合は false を返します。
     *
     * @pre     0 <= first < 0x01000000U
     * @pre     0 < last <= 0x01000000U
     * @pre     first < last
     * @pre     [first, last) が確保不可能領域である必要があります。
     * @post    [first, last) が確保可能領域になります。
     *
     * @details
     *  指定した領域を確保可能にします。@n
     *  first から last の範囲が確保可能領域を含む場合は false が返ります。@n
     *  コンストラクタ時、もしくは Initialize() 時に isThreadSafe 引数に true を渡した場合、本 API はスレッドセーフです。@n
     *  isThreadSafe 引数に false を渡した場合、もしくは isThreadSafe 引数を指定しないコンストラクタか Initialize() を呼び出して初期化した場合は、スレッドセーフではありません。
     */
    bool AddRange(int first, int last) NN_NOEXCEPT;

    /**
     * @brief   指定した領域を確保不可能にします。
     *
     * @param[in]   first   確保不可にしたい範囲の先頭インデックス
     * @param[in]   last    確保不可にしたい範囲の末尾インデックス
     *
     * @return  確保不可能に設定できた場合は true を、そうでない場合は false を返します。
     *
     * @pre     0 <= first < 0x01000000U
     * @pre     0 < last <= 0x01000000U
     * @pre     first < last
     * @pre     [first, last) が確保可能領域である必要があります。
     * @post    [first, last) が確保不可領域になります。
     *
     * @details
     *  指定した領域を確保不可能にします。@n
     *  first から last の範囲が確保不可能領域を含む、もしくは確保済みの領域を含む場合は false が返ります。@n
     *  コンストラクタ時、もしくは Initialize() 時に isThreadSafe 引数に true を渡した場合、本 API はスレッドセーフです。@n
     *  isThreadSafe 引数に false を渡した場合、もしくは isThreadSafe 引数を指定しないコンストラクタか Initialize() を呼び出して初期化した場合は、スレッドセーフではありません。
     */
    bool RemoveRange(int first, int last) NN_NOEXCEPT;

    /**
     * @brief   確保可能な区間を割り当てます。
     *
     * @param[out]  pOutFirst   割り当てられた区間の先頭インデックス
     * @param[in]   size        確保したい区間の長さ
     *
     * @return  確保に成功した場合は true を、そうでない場合は false を返します。
     *
     * @pre     アロケータが初期化されている必要があります。
     * @pre     size > 0 && size <= 0x01000000
     *
     * @details
     *  1 <= size <= 0x01000000 の区間のうち、AddRange() によって確保可能にされている領域を割り当てます。@n
     *  成功した場合区間の先頭を pOutFirst に格納します。@n
     *  @n
     *  コンストラクタ時、もしくは Initialize() 時に isThreadSafe 引数に true を渡した場合、本 API はスレッドセーフです。@n
     *  isThreadSafe 引数に false を渡した場合、もしくは isThreadSafe 引数を指定しないコンストラクタか Initialize() を呼び出して初期化した場合は、スレッドセーフではありません。
     */
    bool Allocate(int* pOutFirst, int size) NN_NOEXCEPT;

    /**
     * @brief   確保可能な区間を割り当てます。
     *
     * @param[out]  pOutFirst   割り当てられた区間の先頭インデックス
     * @param[in]   size        確保したい区間の長さ
     * @param[in]   userPtr     ユーザ定義のパラメータ
     * @param[in]   userFunc    解放時に呼び出されるコールバック
     *
     * @return  確保に成功した場合は true を、そうでない場合は false を返します。
     *
     * @pre     アロケータが初期化されている必要があります。
     * @pre     size > 0 && size <= 0x01000000
     *
     * @details
     *  1 <= size <= 0x01000000 の区間のうち、AddRange() によって確保可能にされている領域を割り当てます。@n
     *  成功した場合区間の先頭を pOutFirst に格納します。@n
     *  確保した区間に対して userPtr を関連付けることができます。@n
     *  確保した区間を解放する際に、 userFunc が userPtr を引数として呼び出されます。@n
     *  @n
     *  コンストラクタ時、もしくは Initialize() 時に isThreadSafe 引数に true を渡した場合、本 API はスレッドセーフです。@n
     *  isThreadSafe 引数に false を渡した場合、もしくは isThreadSafe 引数を指定しないコンストラクタか Initialize() を呼び出して初期化した場合は、スレッドセーフではありません。
     */
    bool Allocate(int* pOutFirst, int size, void* userPtr, UserCallback userFunc) NN_NOEXCEPT;

    /**
     * @brief   確保可能な区間を割り当てます。
     *
     * @param[out]  pOutFirst   割り当てられた区間の先頭インデックス
     * @param[in]   size        確保したい区間の長さ
     * @param[in]   alignment   確保する区間のアライメント
     *
     * @return  確保に成功した場合は true を、そうでない場合は false を返します。
     *
     * @pre     アロケータが初期化されている必要があります。
     * @pre     alignment > 0
     * @pre     size > 0 && size <= 0x01000000
     *
     * @details
     *  1 <= size <= 0x01000000 の区間のうち、AddRange() によって確保可能にされている領域を割り当てます。@n
     *  成功した場合区間の先頭を pOutFirst に格納します。@n
     *  @n
     *  コンストラクタ時、もしくは Initialize() 時に isThreadSafe 引数に true を渡した場合、本 API はスレッドセーフです。@n
     *  isThreadSafe 引数に false を渡した場合、もしくは isThreadSafe 引数を指定しないコンストラクタか Initialize() を呼び出して初期化した場合は、スレッドセーフではありません。
     */
    bool Allocate(int* pOutFirst, int size, int alignment) NN_NOEXCEPT;

    /**
     * @brief   確保可能な区間を割り当てます。
     *
     * @param[out]  pOutFirst   割り当てられた区間の先頭インデックス
     * @param[in]   size        確保したい区間の長さ
     * @param[in]   alignment   確保する区間のアライメント
     * @param[in]   userPtr     ユーザ定義のパラメータ
     * @param[in]   userFunc    解放時に呼び出されるコールバック
     *
     * @return  確保に成功した場合は true を、そうでない場合は false を返します。
     *
     * @pre     アロケータが初期化されている必要があります。
     * @pre     alignment > 0
     * @pre     size > 0 && size <= 0x01000000
     *
     * @details
     *  1 <= size <= 0x01000000 の区間のうち、AddRange() によって確保可能にされている領域を割り当てます。@n
     *  成功した場合区間の先頭を pOutFirst に格納します。@n
     *  確保した区間に対して userPtr を関連付けることができます。@n
     *  確保した区間を解放する際に、 userFunc が userPtr を引数として呼び出されます。@n
     *  @n
     *  コンストラクタ時、もしくは Initialize() 時に isThreadSafe 引数に true を渡した場合、本 API はスレッドセーフです。@n
     *  isThreadSafe 引数に false を渡した場合、もしくは isThreadSafe 引数を指定しないコンストラクタか Initialize() を呼び出して初期化した場合は、スレッドセーフではありません。
     */
    bool Allocate(int* pOutFirst, int size, int alignment, void* userPtr, UserCallback userFunc) NN_NOEXCEPT;

    /**
     * @brief   割り当て済みの区間を解放します。
     *
     * @param[in]   first   解放する区間の先頭インデックス
     *
     * @pre アロケータが初期化されている必要があります。
     * @pre first はアロケータから確保された区間の先頭インデックスである必要があります。
     *
     * @details
     *  コンストラクタ時、もしくは Initialize() 時に isThreadSafe 引数に true を渡した場合、本 API はスレッドセーフです。@n
     *  isThreadSafe 引数に false を渡した場合、もしくは isThreadSafe 引数を指定しないコンストラクタか Initialize() を呼び出して初期化した場合は、スレッドセーフではありません。
     */
    void Free(int first) NN_NOEXCEPT;

    //! @}

    //! @name 情報取得 API
    //! @{

    /**
     * @brief   確保済みの区間に関連付けられたユーザ定義のパラメータを取得します。
     *
     * @param[out]  pOutUserPtr    ユーザ定義のパラメータ
     * @param[in]   first          確保済み区間の先頭インデックス
     *
     * @return  ユーザ定義のパラメータが取得できた場合は true を、そうでない場合は false を返します。
     *
     * @pre アロケータが初期化されている必要があります。
     * @pre first はアロケータから確保された区間の先頭インデックスである必要があります。
     *
     * @details
     *  確保済みの区間に関連付けられたユーザ定義のパラメータを取得します。@n
     *  @n
     *  コンストラクタ時、もしくは Initialize() 時に isThreadSafe 引数に true を渡した場合、本 API はスレッドセーフです。@n
     *  isThreadSafe 引数に false を渡した場合、もしくは isThreadSafe 引数を指定しないコンストラクタか Initialize() を呼び出して初期化した場合は、スレッドセーフではありません。
     */
    bool FindUserData(void** pOutUserPtr, int first) const NN_NOEXCEPT;

    /**
     * @brief  指定した確保済み区間のサイズを取得します。
     *
     * @param[in]  first    サイズを取得する区間の先頭インデックス
     *
     * @return  確保済み区間のサイズが返却されます。
     *
     * @pre アロケータが初期化されている必要があります。
     * @pre first はアロケータから確保された区間の先頭インデックスである必要があります。
     *
     * @details
     *  指定した確保済み区間のサイズを取得します。@n
     *  @n
     *  コンストラクタ時、もしくは Initialize() 時に isThreadSafe 引数に true を渡した場合、本 API はスレッドセーフです。@n
     *  isThreadSafe 引数に false を渡した場合、もしくは isThreadSafe 引数を指定しないコンストラクタか Initialize() を呼び出して初期化した場合は、スレッドセーフではありません。
     *
     */
    int GetSizeOf(int first) const NN_NOEXCEPT;

    /**
     * @brief  アロケータに存在する空き領域の合計を取得します。
     *
     * @return 空き領域の合計サイズが返ります。
     *
     * @pre アロケータが初期化されている必要があります。
     *
     * @details
     *  アロケータで管理している空き領域の合計を取得します。@n
     *  空き領域が複数存在する場合、実際に確保可能なサイズは合計サイズよりも小さくなります。@n
     *  実際に確保可能なサイズを取得するには GetAllocatableSize() を使用してください。@n
     *  @n
     *  コンストラクタ時、もしくは Initialize() 時に isThreadSafe 引数に true を渡した場合、本 API はスレッドセーフです。@n
     *
     */
    int GetTotalFreeSize() const NN_NOEXCEPT;

    /**
     * @brief  アロケータから確保可能な最大サイズを取得します。
     *
     * @return 確保可能なサイズが返ります。
     *
     * @pre アロケータが初期化されている必要があります。
     *
     * @details
     *  アロケータから確保可能な最大サイズを取得します。@n
     *  空き領域が複数存在する場合、その中で最大の領域のサイズを取得します。@n
     *  @n
     *  本 API で返却されたサイズは必ず確保可能であることを保証しますが、アルゴリズム上、本 API で返却されたサイズ以上の領域が確保可能な場合があります。@n
     *  @n
     *  コンストラクタ時、もしくは Initialize() 時に isThreadSafe 引数に true を渡した場合、本 API はスレッドセーフです。
     *
     */
    int GetAllocatableSize() const NN_NOEXCEPT;

    /**
     * @brief   確保・解放時のモードを設定します。
     *
     * @param[in]   allocationMode 確保・解放時のモード
     *
     * @pre アロケータが初期化されている必要があります。
     *
     * @details
     *  デフォルトでは AllocationMode_NoOption が指定されています。@n
     *  @n
     *  コンストラクタ時、もしくは Initialize() 時に isThreadSafe 引数に true を渡した場合、本 API はスレッドセーフです。
     *
     */
    void SetAllocationMode(AllocationMode allocationMode) NN_NOEXCEPT;

    /**
     * @brief   確保・解放時のモードを取得します。
     *
     * @return  確保・解放時のモードを返します。
     *
     * @pre アロケータが初期化されている必要があります。
     *
     * @details コンストラクタ時、もしくは Initialize() 時に isThreadSafe 引数に true を渡した場合、本 API はスレッドセーフです。
     *
     */
    AllocationMode GetAllocationMode() const NN_NOEXCEPT;

    //! @}

    //! @name デバッグ用 API
    //! @{

    /**
     * @brief  アロケータ内部の情報を表示します。
     *
     * @param[in]   offset  表示時のインデックスの開始番号のずれ
     * @param[in]   factor  区間の長さ 1 を定数倍するための定数
     *
     * @pre アロケータが初期化されている必要があります。
     *
     * @details
     *  アロケータの内部状態を出力します。@n
     *  offset と factor を指定すると、インデックスの開始番号を offset 、アロケータで扱う区間の長さ 1 を factor とした場合のアロケータ内部の情報を表示します。@n
     *  offset に 0 、 factor に 1 を指定して呼び出すと、アロケータ内部そのままの情報が表示されます。@n
     *  @n
     *  出力されるフォーマットの詳細については NintendoSDK ドキュメントの @confluencelink{165040249, NumberLineAllocator} の説明を参照してください。
     */
    void Dump(uint64_t offset, int factor) const NN_NOEXCEPT;

    //! @}

private:
    NumberLineAllocatorPrv*      m_pNumberLine;  //!< アロケータの実体
    FreeCallback                m_FreeFunc;     //!< 解放時コールバック
    void*                       m_pFreeParam;   //!< 解放時パラメータ
    mutable nn::os::Mutex       m_Mutex;        //!< ミューテックス
    bool                        m_IsThreadSafe; //!< 各機能をスレッドセーフで呼び出すか否か
};

}}  // namespace nn { namespace mem {
