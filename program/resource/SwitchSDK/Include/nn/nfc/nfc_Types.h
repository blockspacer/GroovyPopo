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
 * @brief   NFC に関する型や定数の宣言
 */

#pragma once

#include <nn/nn_Common.h>

namespace nn { namespace nfc {

/**
 * @brief   NFC デバイスの最大数です。
 */
const int DeviceCountMax = 9;

/**
 * @brief   タグの ID (UID) の最大長です。
 */
const size_t UidLengthMax = 10;

/**
 * @brief   NFC ライブラリの状態を表す列挙型です。
 */
enum State
{
    State_None,          //!< NFC ライブラリが初期化されていない状態です。起動時はこの状態です。
    State_Init,          //!< NFC ライブラリが初期化されている状態です。
};

/**
 * @brief   NFC デバイスの状態を表す列挙型です。
 */
enum DeviceState
{
    DeviceState_Init,          //!< タグを探していない状態です。
    DeviceState_Search,        //!< タグを探している状態です。
    DeviceState_Active,        //!< @ref DeviceState_Search でタグを検知するとこの状態になります。
    DeviceState_Deactive,      //!< タグを検知した後、タグが離れるとこの状態になります。
    DeviceState_Keep,          //!< @ref DeviceState_Active で近くにあるタグとのセッションを維持するとこの状態になります。
    DeviceState_Unexpected     //!< 想定していない状態です。
};

/**
 * @brief   タグのタイプを表す列挙型です。
 */
enum TagType
{
    TagType_Error    = 0,           //!< 想定していない Type のタグです
    TagType_Type1    = 1,           //!< TYPE1
    TagType_Type2    = 1 << 1,      //!< TYPE2
    TagType_Type3    = 1 << 2,      //!< TYPE3
    TagType_Type4A   = 1 << 3,      //!< TYPE4A
    TagType_Type4B   = 1 << 4,      //!< TYPE4B
    TagType_Iso15693 = 1 << 5,      //!< ISO15693
    TagType_Mifare   = 1 << 6,      //!< MIFARE
    TagType_All      = 0xFFFFFFFF   //!< 全タイプ (デフォルト)
};

/**
 * @brief   タグのプロトコルを表す列挙型です。
 */
enum NfcProtocol
{
    NfcProtocol_Error     = 0,           //!< 想定していないプロトコルのタグです
    NfcProtocol_TypeA     = 1,           //!< NFC-A
    NfcProtocol_TypeB     = 1 << 1,      //!< NFC-B
    NfcProtocol_TypeF     = 1 << 2,      //!< NFC-F
    NfcProtocol_Type15693 = 1 << 3,      //!< ISO-15693
    NfcProtocol_All       = 0xFFFFFFFF   //!< 全プロトコル
};

/**
 * @brief   NFC デバイスのハンドルです。
 */
struct DeviceHandle
{
    nn::Bit32              _id;          //   NFC デバイス固有の ID
    char                   _reserved[4];
};

/**
 * @brief   タグの ID (UID) を示す構造体です。
 */
struct TagId
{
    char        uid[UidLengthMax];  //!< タグの UID
    uint8_t     length;             //!< UID の長さ
    char        _reserved[21];
};

/**
 * @brief   タグ情報を表す構造体です。
 */
struct TagInfo
{
    TagId           tagId;        //!< タグのユニーク ID (UID)
    nn::Bit32       protocol;     //!< 通信プロトコル。@ref NfcProtocol の値が入ります。
    nn::Bit32       type;         //!< タグのタイプ。@ref TagType の値が入ります。
    char            _reserved[48];
};

}  // nfc
}  // nn
