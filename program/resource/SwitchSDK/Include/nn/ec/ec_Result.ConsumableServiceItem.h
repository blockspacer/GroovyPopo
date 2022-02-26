/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

// このファイルは自動生成ファイルです。
// 手動で変更しないでください。

// NOLINT(build/header_guard)

// package 'ConsumableServiceItem'
//! @defresulta{ec,ConsumableServiceItem のエラー,164,300,399,ResultConsumableServiceItem}
NN_DEFINE_ABSTRACT_ERROR_RANGE_RESULT(ResultConsumableServiceItem, 164, 300, 400);
    //! @defresult{ec,セーブデータのフォーマットが不正です,164,300,300,ResultConsumableServiceItemInvalidSaveData\, ResultConsumableServiceItem}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultConsumableServiceItemInvalidSaveData, 164, 300, 301);
    //! @defresult{ec,サーバー上の権利の状態が不正です,164,301,301,ResultConsumableServiceItemInvalidServerRightStatus\, ResultConsumableServiceItem}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultConsumableServiceItemInvalidServerRightStatus, 164, 301, 302);
    //! @defresult{ec,出力先バッファ容量が不足している,164,302,302,ResultConsumableServiceItemInsufficientBuffer\, ResultConsumableServiceItem}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultConsumableServiceItemInsufficientBuffer, 164, 302, 303);
    //! @defresult{ec,指定したアイテム ID が見つからない,164,303,303,ResultConsumableServiceItemIdNotFound\, ResultConsumableServiceItem}
    NN_DEFINE_ERROR_RANGE_RESULT(ResultConsumableServiceItemIdNotFound, 164, 303, 304);
