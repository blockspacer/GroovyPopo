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
* @brief テクスチャーリソースファイルのデータに関する API の宣言
*/

#pragma once

#include <nn/util/util_BinaryFormat.h>
#include <nn/util/util_ResDic.h>

#include <nn/gfx/gfx_TextureInfoData.h>
#include <nn/gfx/gfx_DescriptorSlot.h>
#include <nn/gfx/gfx_ResUserData.h>

namespace nn {
namespace gfx {

class ResTexture;
class ResTextureFile;
struct ResTextureData;
struct ResTextureContainerData;
struct ResTextureFileData;

/**
* @brief テクスチャーリソースを表す構造体です。
*/
struct ResTextureData
{
    nn::util::BinaryBlockHeader blockHeader; //!< ブロックヘッダーです

    TextureInfoData textureInfoData; //!< テクスチャーを初期化するための情報です

    uint32_t textureDataSize; //!< テクスチャーのデータサイズです
    uint32_t alignment; //!< アライメントです
    Bit8 channelMapping[ 4 ]; //!< nn::gfx::ChannelMapping で表される、R、G、B、A の順でのチャンネルマッピングです
    Bit8 imageDimension; //!< nn::gfx::ImageDimension で表される、イメージの次元です
    char reserved[ 3 ]; //!< 予約領域です。
    nn::util::BinPtrToString pName; //!< テクスチャーの名前です
    nn::util::BinTPtr< ResTextureContainerData > pResTextureContainerData; //!< テクスチャーコンテナーリソースへのポインターです
    nn::util::BinTPtr< nn::util::BinTPtr< void > > pMipPtrArray; //!< 各レベルのテクスチャーデータへのポインター配列へのポインターです
    nn::util::BinTPtr< ResUserData > pUserDataArray; //!< ユーザーデータの配列へのポインターです

    nn::util::BinTPtr< void > pTexture; //!< テクスチャーオブジェクトへのポインターです
    nn::util::BinTPtr< void > pTextureView; //!< テクスチャービューへのポインターです

    DescriptorSlotData userDescriptorSlot; //!< ユーザーデスクリプタースロットです

    nn::util::BinTPtr< nn::util::ResDic > pUserDataDic; //!< ユーザーデータ辞書へのポインターです
};

/**
* @brief テクスチャーコンテナーを表す構造体です。
*/
struct ResTextureContainerData
{
    char targetPlatform[ 4 ]; //!< 対象プラットフォームです
    uint32_t textureCount; //!< テクスチャーの数です
    nn::util::BinTPtr< nn::util::BinTPtr< ResTexture > > pTexturePtrArray; //!< テクスチャーリソースへのポインター配列へのポインターです
    nn::util::BinTPtr< void > pTextureData; //!< テクスチャーデータブロックへのポインターです
    nn::util::BinTPtr< nn::util::ResDic > pTextureDic; //!< テクスチャー名の辞書へのポインターです

    nn::util::BinTPtr< void > pTextureMemoryPool; //!< テクスチャー用メモリープールへのポインターです

    nn::util::BinTPtr< void > pCurrentMemoryPool; //!< テクスチャーコンテナーが現在利用しているメモリープールへのポインターです
    uint32_t memoryPoolOffsetBase; //!< メモリープールのオフセットのベース値です

    char reserved[ 4 ]; //!< 予約領域です
};

/**
* @brief テクスチャーリソースファイルを表す構造体です。
*/
struct ResTextureFileData
{
    nn::util::BinaryFileHeader fileHeader; //!< ファイルヘッダーです
    ResTextureContainerData textureContainerData; //!< テクスチャーコンテナーリソースです
};

}
}
