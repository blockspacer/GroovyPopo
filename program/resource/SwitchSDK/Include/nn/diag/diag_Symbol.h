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
*   @file
*   @brief  シンボル情報取得関連の API 宣言
*/

#include <nn/nn_Common.h>

namespace nn { namespace diag {

/*!
    @brief  引数に指定したアドレスから、アドレスが小さい方向に最も近いシンボルを検索して、シンボル名を取得します。

    @param[out] outNameBuffer   シンボル名を格納するバッファ。
    @param[in]  nameBufferSize  シンボル名を格納するバッファのバイトサイズ。
    @param[in]  address         シンボルを検索するアドレス。

    @return                     シンボルのアドレス。シンボルが見つからない場合は 0 を返す。

    @pre
        - outNameBuffer != nullptr
        - nameBufferSize > 0
        - address > 0

    @details
        outNameBuffer には、取得したシンボルの名前が格納されます。outNameBuffer はヌル終端されます。@n
        シンボル名が、nameBufferSize より長い場合は、nameBufferSize - 1 バイトまで格納されます。

    @platformbegin{NX}
        ローカルシンボルおよびシステムシンボルは取得できません。@n
        デッドストリップを有効にすると、エクスポートされたシンボル以外取得できなくなります。
    @platformend
*/
uintptr_t GetSymbolName(char* outNameBuffer, size_t nameBufferSize, uintptr_t address) NN_NOEXCEPT;

/*!
    @brief  引数に指定したアドレスと、アドレスが一致するシンボルを検索して、そのシンボルの指すオブジェクトのサイズを取得します。

    @param[in]  address         サイズを取得するシンボルのアドレス。

    @return                     シンボルの指すオブジェクトのサイズ。シンボルが見つからない場合は 0 を返す。

    @pre
        - address > 0

    @platformbegin{NX}
        ローカルシンボルのサイズは取得できません。@n
    @platformend
*/
size_t GetSymbolSize(uintptr_t address) NN_NOEXCEPT;

}} // nn::diag
