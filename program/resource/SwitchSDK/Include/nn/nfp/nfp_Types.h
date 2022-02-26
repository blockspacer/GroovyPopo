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
 * @brief   NFP(amiibo)に関する型や定数の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/mii/mii_Common.h>

namespace nn { namespace nfp {

/**
 * @brief   NFC デバイスの最大数です。
 */
const int DeviceCountMax = 9;

/**
 * @brief   タグの ID (UID) の最大長です。
 */
const int UidLengthMax = 10;

/**
 * @brief   ニックネームの最大長です。
 */
const int NicknameLengthMax = 10;

/**
 * @brief   Character ID のサイズです。
 */
const int CharacterIdSize = 3;

/**
 * @brief   不明なエラーコードを表す値です。
 */
const uint32_t ErrorCodeUnknown = 0xFFFFFFFF;

/**
 * @brief   NFP ライブラリの状態を表す列挙型です。
 */
enum State
{
    State_None,          //!< NFP ライブラリが初期化されていない状態です。起動時はこの状態です。
    State_Init,          //!< NFP ライブラリが初期化されている状態です。
    State_Unexpected     //!< 想定していない状態です。
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
    DeviceState_Mount,         //!< @ref DeviceState_Active で近くにあるタグをマウントするとこの状態になります。
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
 * @brief   フォントリージョンを表す列挙型です。
 */
enum FontRegion
{
    FontRegion_JpUsEu,       //!< 日本、北米、欧州リージョン
    FontRegion_China,        //!< 中国リージョン
    FontRegion_Korea,        //!< 韓国リージョン
    FontRegion_Taiwan,       //!< 台湾リージョン
    FontRegion_Count,
    FontRegion_Term    = FontRegion_Count,
    FontRegion_Min     = 0,
    FontRegion_Max     = FontRegion_Count - 1
};

/**
 * @brief   NFP 製品タイプを表す列挙型です。
 */
enum ModelType
{
    ModelType_Amiibo   //!< amiibo
};

/**
 * @brief   マウント対象の領域を表す列挙型です。
 */
enum MountTarget
{
    MountTarget_Rom  = 1 << 0,                           //!< 読み取り専用領域
    MountTarget_Ram  = 1 << 1,                           //!< 読み書き可能領域
    MountTarget_All  = MountTarget_Rom | MountTarget_Ram //!< 全ての領域
};

/**
 * @brief   amiibo 設定の動作オプションをビットフィールドを示す列挙型です。
 */
enum AmiiboSettingsOptionFlags
{
    AmiiboSettingsOptionFlags_Default          = 0,        //!< デフォルト動作
    AmiiboSettingsOptionFlags_RequiredNickname = 1 << 0,   //!< ニックネームとオーナーの設定においてニックネームの登録が必須
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

/**
 * @brief   日付情報を表す構造体です。
 */
struct Date
{
    int16_t year;                                  //!< 年
    int8_t  month;                                 //!< 月。1 から 12 までの値が入ります。
    int8_t  day;                                   //!< 日
};

/**
 * @brief   アプリケーション専用領域の初期化情報を示す構造体です。
 */
struct ApplicationAreaCreateInfo
{
    nn::Bit32   accessId;                   //!< アプリ固有の ID
    uint32_t    initialDataSize;            //!< 初期化データサイズ
    void*       pInitialData;               //!< 初期化データ
};

/**
 * @brief   共用領域の情報を示す構造体です。
 */
struct CommonInfo
{
    Date                lastWriteDate;                 //!< 最終書き込み日
    uint16_t            writeCounter;                  //   書き込み回数
    uint8_t             nfpVersion;                    //   NFP バージョン
    char                _reserved1[1];
    uint32_t            applicationAreaSize;           //!< アプリケーション専用領域のサイズ
    char                _reserved2[52];
};

/**
 * @brief   オーナー登録情報を示す構造体です。
 */
struct RegisterInfo
{
    nn::mii::CharInfo  miiData;                             //!< Mii データ
    Date               registerDate;                        //!< 登録日です。
    char               nickname[NicknameLengthMax * 4 + 1]; //!< ニックネーム (+ 終端文字) です。UTF-8 で格納されます。
    nn::Bit8           fontRegion;                          //!< ニックネームを表示する際のフォントのリージョン情報です。@ref FontRegion の値が格納されます。
    char               _reserved[122];
};

/**
 * @brief   NFP の型式情報を示す構造体です。
 */
struct ModelInfo
{
    char        characterId[CharacterIdSize];   //!< キャラクター ID
    nn::Bit8    seriesId;                       //   シリーズ ID
    uint16_t    numberingId;                    //   通し番号
    nn::Bit8    nfpType;                        //   フィギュア属性
    char        _reserved[57];
};

/**
 * @brief   amiibo 設定開始時に指定する共通のパラメータを示す構造体です。
 */
struct AmiiboSettingsStartParam
{
    DeviceHandle              deviceHandle;      //!< NFC デバイスのハンドルです。
    char                      _debug[32];        //   デバッグ情報
    nn::Bit8                  optionFlags;       //!< amiibo 設定の動作オプションを表すビットフィールドです。各ビットは @ref AmiiboSettingsOptionFlags で定義されます。
    char                      _reserved[7];
};

}  // nfp
}  // nn
