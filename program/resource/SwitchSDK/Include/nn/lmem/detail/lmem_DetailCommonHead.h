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

#include <nn/nn_Macro.h>
#include <nn/util/util_TypedStorage.h>
#include <nn/os.h>

/**
    @file
    @brief mem で利用する内部実装用ヘッダ構造体の型宣言
*/

namespace nn { namespace lmem { namespace detail {

/**
 * @brief 双方向リンクリストのノード構造体です。この構造体をリスト構造で繋ぎたい構造体のメンバとして格納します。
 */
typedef struct
{
    void*       pPrevObject;    //!< 前に繋がれているオブジェクトへのポインタ。
    void*       pNextObject;    //!< 次に繋がれているオブジェクトへのポインタ。
} LinkedListNode;

/**
 * @brief 双方向リンクリスト構造体です。
 */
typedef struct
{
    void*       pHeadObject;    //!< 先頭に繋がれているオブジェクトへのポインタ。
    void*       pTailObject;    //!< 後尾に繋がれているオブジェクトへのポインタ。
    uint16_t    numObjects;     //!< リストに繋がれているオブジェクトの個数。
    uint16_t    offset;         //!< LinkedListNode型の構造体メンバのオフセット。
} LinkedList;

/**
 * @brief 拡張ヒープのメモリブロックのヘッダ情報です。
 */
struct ExpHeapMemoryBlockHead
{
    Bit16 signature;                        //!< シグネチャ。メモリブロックヘッダが壊れていないかのチェックに用いられます
    Bit32 attribute;                        //!< 属性
    // attribute の各ビットは以下の値を表します
    // [15:メモリ確保方向]
    // [8-14 :アライメントによるアドレスのずれ（パディング値）]
    // [0-7 :グループID]

    size_t blockSize;                       //!< ブロックサイズ(データ領域のみ)

    ExpHeapMemoryBlockHead* pPrevBlock;     //!< 前ブロック
    ExpHeapMemoryBlockHead* pNextBlock;     //!< 次ブロック
};

/**
 * @brief 拡張ヒープのメモリブロックのリストです。
 */
struct ExpHeapMemoryBlockList
{
    ExpHeapMemoryBlockHead* pHead;   //!< 先頭に繋がれているメモリへのポインタ
    ExpHeapMemoryBlockHead* pTail;   //!< 後尾に繋がれているメモリへのポインタ
};

/**
 * @brief 拡張ヒープのヘッダ情報です。
 */
struct ExpHeapHead
{
    ExpHeapMemoryBlockList freeList;    //!< フリーリスト
    ExpHeapMemoryBlockList usedList;    //!< 使用リスト

    Bit16 groupId;                      //!< カレントグループID (下位8bitのみ)
    Bit16 feature;                      //!< 属性
    // feature の各ビットは以下の値を表します
    // [0 :メモリ確保モード]
    bool isReuse;                       //!< アラインメントの際に発生する隙間の領域を再利用するかどうか
    NN_PADDING3;
};

/**
 * @brief  フレームヒープのヘッダ情報です。
 */
struct FrameHeapHead
{
    void* nextBlockFront;   //!< 先頭メモリ確保ポインタ
    void* nextBlockRear;    //!< 末尾メモリ確保ポインタ
};

/**
 * @brief   メモリブロックヘッダへのポインタを持つ単方向リストです。
 */
struct UnitHead
{
    UnitHead* pNext;    //!< 次ブロックへのポインタ
};

/**
 * @brief   メモリブロックの単方向リストです。
 */
struct UnitList
{
    UnitHead* pHead;    //!< 先頭に繋がれているメモリブロックへのポインタ
};

/**
 * @brief   ユニットヒープのヘッダ情報です。
 */
struct UnitHeapHead
{
    UnitList freeList;  //!< フリーリスト
    size_t unitSize;    //!< メモリブロック（ユニット）サイズ
    int alignment;      //!< アライメント
    int count;          //!< 使用しているユニットの数
};

/**
 * @brief HeapHead に特定のヒープヘッダを確保するための共用体です。
 */
union SpecificHeapHead
{
    ExpHeapHead expHeapHead;
    FrameHeapHead frameHeapHead;
    UnitHeapHead unitHeapHead;
};

/**
 * @brief ヒープ共通のヘッダ情報です。
 */
struct HeapHead
{
    Bit32 signature;

    LinkedListNode link;
    LinkedList childList;

    void* pHeapStart;    //!< ヒープ先頭アドレス
    void* pHeapEnd;      //!< ヒープ末尾(+1)アドレス

    nn::os::MutexType mutex;

    Bit8 attribute;        //!< オプションフラグ

    SpecificHeapHead specificHeapHead;
};

}}}
