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
 * @brief   MIFARE に関する型や定数の宣言
 */

#pragma once

#include <nn/nn_Common.h>

namespace nn { namespace nfc {

/**
 * @brief   MIFARE タグアクセスに使用する鍵の生データの長さです。
 */
const size_t MifareKeyLength = 6;

/**
 * @brief    MIFARE タグ 1 ブロックのデータのサイズです。
 */
const size_t MifareBlockDataSize = 16;

/**
 * @brief   MIFARE タグアクセスに使用する鍵の種類を表す列挙型です。
 */
enum MifareKeyType
{
    MifareKeyType_A  = 0x60,        //!< Key A
    MifareKeyType_B  = 0x61,        //!< Key B
};

/**
 * @brief   MIFARE タグアクセスに使用する鍵の指定方法を表す列挙型です。
 */
enum MifareKeyValueFormat
{
    MifareKeyValueFormat_Raw = 1,      //!< 鍵の生データ指定
};

/**
 * @brief       MIFARE タグアクセスに使用する鍵の生データを表す型です。
 */
struct MifareKeyValue
{
    char     value[MifareKeyLength]; //!< 鍵の生データ
    char    _reserved[2];
};

/**
 * @brief       MIFARE タグアクセスに使用する鍵を表す型です。
 */
struct MifareKey
{
    nn::Bit8 type;             //!< 認証に使用する鍵の種類です。@ref MifareKeyType の値が入ります。
    nn::Bit8 valueFormat;      //!< 認証に使用する鍵の指定方法です。@ref MifareKeyValueFormat の値が入ります。
    char _reserved[6];
    union{
        MifareKeyValue raw;    //!< 鍵の生データです。@a valueFormat が @ref MifareKeyValueFormat_Raw の場合に使用します。
        char _dummy[8];        // 上限となるデータサイズを規定
    };
};

/**
 * @brief       MIFARE タグ読み込み用のパラメータです。
 */
struct MifareReadBlockParameter
{
    uint8_t blockAddr;            //!< ブロックのアドレスです。
    char _reserved[7];
    MifareKey key;                //!< 認証に使う鍵です。
};

/**
 * @brief       MIFARE タグ読み込みデータです。
 */
struct MifareReadBlockData
{
    char buffer[MifareBlockDataSize];  //!< データの取得先バッファです。
    uint8_t blockAddr;                 //!< ブロックのアドレスです。
    char _reserved[7];
};

/**
 * @brief       MIFARE タグ書き込み用のパラメータです。
 */
struct MifareWriteBlockParameter
{
    char data[MifareBlockDataSize];     //!< 書き込むデータです。
    uint8_t blockAddr;                  //!< ブロックのアドレスです。
    char _reserved[7];
    MifareKey key;                      //!< 認証に使う鍵です。
};

}  // nfc
}  // nn
