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
 * @brief   多重待ちに関する API の宣言
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/os/os_MultipleWaitCommon.h>

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------

namespace nn {

class   TimeSpan;

namespace os {

struct  MultiWaitType;
struct  MultiWaitHolderType;


//! @name 多重待ち関連 API
//! @{

//--------------------------------------------------------------------------
/**
 * @brief   multiWait オブジェクトヘッダを初期化します。
 *
 * @param[in] multiWait     multiWait オブジェクトヘッダへのポインタ
 *
 * @pre
 *  - multiWait が未初期化状態である
 *
 * @post
 *  - multiWait が初期化状態である
 *
 */
void    InitializeMultiWait( MultiWaitType* multiWait ) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   multiWait オブジェクトヘッダをファイナライズします。
 *
 * @param[in] multiWait     multiWait オブジェクトヘッダへのポインタ
 *
 * @pre
 *  - multiWait が初期化状態である
 *  - multiWait リストに multiWaitHolder が１つもリンクされていない
 *
 * @post
 *  - multiWait が未初期化状態である
 *
 */
void    FinalizeMultiWait( MultiWaitType* multiWait ) NN_NOEXCEPT;



//--------------------------------------------------------------------------
/**
 * @brief   multiWait リスト内のいずれかの multiWaitHolder を待機します。
 *
 * @param[in] multiWait     multiWait オブジェクトヘッダへのポインタ
 *
 * @return  シグナル状態に変化した multiWaitHolder オブジェクトへのポインタ
 *
 * @pre
 *  - multiWait が初期化状態である
 *  - multiWait にリンクされた全ての MultiWaitHolderType が初期化状態である
 *  - multiWait が他スレッド上で使用されていない
 *
 * @post
 *  - いずれかの多重待ちオブジェクトでシグナル化が検知された場合、
 *    シグナル化を検知した MultiWaitHolderType へのポインタを返す
 *
 */
MultiWaitHolderType* WaitAny( MultiWaitType* multiWait ) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   multiWait リスト内のいずれかの multiWaitHolder をポーリングします。
 *
 * @param[in] multiWait     multiWait オブジェクトヘッダへのポインタ
 *
 * @return  シグナル状態に変化した multiWaitHolder オブジェクトへのポインタ
 * @retval  NULL    ポーリング失敗
 *
 * @pre
 *  - multiWait が初期化状態である
 *  - multiWait にリンクされた全ての MultiWaitHolderType が初期化状態である
 *  - multiWait が他スレッド上で使用されていない
 *
 * @post
 *  - いずれかの多重待ちオブジェクトでシグナル化が検知された場合
 *    - シグナル化を検知した MultiWaitHolderType へのポインタを返す
 *  - 全ての多重待ちオブジェクトでシグナル化を検知できなかった場合
 *    - NULL を返す
 *
 */
MultiWaitHolderType* TryWaitAny( MultiWaitType* multiWait )  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   multiWait リスト内のいずれかの multiWaitHolder を待機します。
 *
 * @param[in] multiWait     multiWait オブジェクトヘッダへのポインタ
 * @param[in] timeout       タイムアウト値
 *
 * @return  シグナル状態に変化した multiWaitHolder オブジェクトへのポインタ
 * @retval  NULL    タイムアウト
 *
 * @pre
 *  - multiWait が初期化状態である
 *  - multiWait にリンクされた全ての MultiWaitHolderType が初期化状態である
 *  - multiWait が他スレッド上で使用されていない
 *  - timeout >= 0
 *
 * @post
 *  - いずれかの多重待ちオブジェクトでシグナル化が検知された場合
 *    - シグナル化を検知した MultiWaitHolderType へのポインタを返す
 *  - 全ての多重待ちオブジェクトでシグナル化を検知できなかった場合
 *    - NULL を返す
 *
 */
MultiWaitHolderType* TimedWaitAny( MultiWaitType* multiWait, TimeSpan timeout )  NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   multiWaitHolder オブジェクトをファイナライズします。
 *
 * @param[in] multiWaitHolder   多重待ちオブジェクトへのポインタ
 *
 * @pre
 *  - multiWaitHolder が初期化状態である
 *  - multiWaitHolder がどのリストにもリンクされていない
 *
 * @post
 *  - multiWaitHolder が未初期化状態である
 *
 */
void    FinalizeMultiWaitHolder( MultiWaitHolderType* multiWaitHolder ) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   multiWait ヘッダに多重待ちオブジェクト multiWaitHolder をリンクします。
 *
 * @param[in] multiWait         multiWait オブジェクトヘッダへのポインタ
 * @param[in] multiWaitHolder   多重待ちオブジェクトへのポインタ
 *
 * @pre
 *  - multiWait が初期化状態である
 *  - multiWaitHolder が初期化状態である
 *  - multiWaitHolder がどのリストにもリンクされていない
 *
 * @post
 *  - multiWait リストに、multiWaitHolder オブジェクトがリンクされる
 *
 * @details
 *  多重待ちオブジェクトヘッダ multiWait に、多重待ちオブジェクト
 *  multiWaitHolder をリンクします。リンクされた multiWaitHolder
 *  オブジェクトは、 WaitAny() 系の API で multiWait が渡された時に、@n
 *  シグナル化を待機する対象同期オブジェクトの１つとなります。
 *
 *  本 API はスレッドセーフではありません。
 *  本 API の動作中に対象 multiWait および multiWaitHolder オブジェクトに
 *  対する操作は行なわないで下さい。
 */
void    LinkMultiWaitHolder(MultiWaitType* multiWait, MultiWaitHolderType* multiWaitHolder) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   multiWait ヘッダから、多重待ちオブジェクト multiWaitHolder をアンリンクします。
 *
 * @param[in] multiWaitHolder   多重待ちオブジェクトへのポインタ
 *
 * @pre
 *  - multiWaitHolder が初期化状態である
 *  - multiWaitHolder が multiWait リストにリンクされている
 *
 * @post
 *  - multiWaitHolder がどのリストにもリンクされていない
 *
 * @details
 *  多重待ちオブジェクトヘッダにリンクされている、多重待ちオブジェクト
 *  multiWaitHolder をアンリンクします。アンリンクされた multiWaitHolder
 *  オブジェクトは、どこのリストヘッダにもリンクされていない状態になります。
 *
 *  本 API はスレッドセーフではありません。
 *  本 API の動作中に対象 multiWait および multiWaitHolder オブジェクトに
 *  対する操作は行なわないで下さい。
 */
void    UnlinkMultiWaitHolder(MultiWaitHolderType* multiWaitHolder) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   multiWait ヘッダにリンクされている全ての多重待ちオブジェクトホルダーをアンリンクします。
 *
 * @param[in] multiWait         multiWait オブジェクトヘッダへのポインタ
 *
 * @pre
 *  - multiWait が初期化状態である
 *
 * @post
 *  - multiWait にリンクされていた多重待ちオブジェクトホルダーがどのリストにもリンクされていない
 *  - multiWait リストに多重待ちオブジェクトホルダーが１つもリンクされていない
 *
 * @details
 *  多重待ちオブジェクトヘッダ multiWait にリンクされている、全ての多重待ちオブジェクトホルダー
 *  をアンリンクします。アンリンクされた多重待ちオブジェクトホルダーは、
 *  どこのリストヘッダにもリンクされていない状態になります。
 *
 *  本 API はスレッドセーフではありません。
 *  本 API の動作中に対象 multiWait オブジェクトやリンクされている多重待ちオブジェクトホルダーに
 *  対する操作は行なわないで下さい。
 */
void    UnlinkAllMultiWaitHolder(MultiWaitType* multiWait) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   srcMultiWait ヘッダにリンクされている全ての多重待ちオブジェクトホルダーをアンリンクし dstMultiWait にリンクします。
 *
 * @param[in] dstMultiWait      リンク先の multiWait オブジェクトヘッダへのポインタ
 * @param[in] srcMultiWait      アンリンク元の multiWait オブジェクトヘッダへのポインタ
 *
 * @pre
 *  - dstMultiWait が初期化状態である
 *  - srcMultiWait が初期化状態である
 *
 * @post
 *  - dstMultiWait には以下のすべてがリンクされている
 *      - 関数の呼び出し前に dstMultiWait にリンクされていた多重待ちオブジェクトホルダー
 *      - 関数の呼び出し前に srcMultiWait にリンクされていた多重待ちオブジェクトホルダー
 *  - srcMultiWait リストに多重待ちオブジェクトホルダーが１つもリンクされていない
 *
 * @details
 *  srcMultiWait でポイントされる多重待ちオブジェクトヘッダにリンクされている、
 *  全ての多重待ちオブジェクトホルダーに関して、@n
 *  srcMultiWait からアンリンクし dstMultiWait にリンクしなおします。
 *
 *  本 API はスレッドセーフではありません。
 *  本 API の動作中に対象 dstMultiWait, srcMultiWait オブジェクト、
 *  または、それらにリンクされている多重待ちオブジェクトホルダーに
 *  対する操作は行なわないで下さい。
 */
void    MoveAllMultiWaitHolder(MultiWaitType* dstMultiWait, MultiWaitType* srcMultiWait) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   multiWaitHolder にユーザデータの値を設定します
 *
 * @param[in] multiWaitHolder   多重待ちオブジェクトへのポインタ
 * @param[in] userData          ユーザデータとして設定する値
 *
 * @pre
 *  - multiWaitHolder が初期化状態である
 *
 * @post
 *  - multiWaitHolder が初期化状態である（ユーザデータのみが変化）
 *
 * @details
 *  multiWaitHolder にユーザデータの値を設定します。
 *
 *  本 API はスレッドセーフではありません。
 *  本 API の動作中に対象 multiWaitHolder オブジェクトに
 *  対する操作は行なわないで下さい。
 */
void    SetMultiWaitHolderUserData(MultiWaitHolderType* multiWaitHolder, uintptr_t userData) NN_NOEXCEPT;


//--------------------------------------------------------------------------
/**
 * @brief   multiWaitHolder に設定されているユーザデータ値を取得します
 *
 * @param[in] multiWaitHolder 多重待ちオブジェクトへのポインタ
 *
 * @return  ユーザデータとして設定されている値
 *
 * @pre
 *  - multiWaitHolder が初期化状態である
 *
 * @post
 *  - multiWaitHolder が初期化状態である（変化しない）
 *
 * @details
 *  multiWaitHolder に設定されているユーザデータの値を取得します。
 *  各 InitializeMultiWaitHolder() を呼んだ直後の初期値は 0 です。
 *
 *  本 API はスレッドセーフではありません。
 *  本 API の動作中に対象 multiWaitHolder オブジェクトに
 *  対する操作は行なわないで下さい。
 */
uintptr_t GetMultiWaitHolderUserData(const MultiWaitHolderType* multiWaitHolder) NN_NOEXCEPT;

//! @}

}} // namespace nn::os

