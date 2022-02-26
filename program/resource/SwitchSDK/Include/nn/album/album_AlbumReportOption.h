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
 * @file
 * @brief  アルバムファイル操作結果の自動通知に関する定義
 */

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

namespace nn{ namespace album{

    //! @brief アルバムの操作結果の自動通知の設定値です。
    //! @details
    //! API 呼出によるアルバムのファイル操作の成否を自動的にユーザーに通知する設定です。
    //! ユーザーに通知される内容は各 API の戻り値と同じ内容です。
    enum AlbumReportOption
    {
        //! @brief アルバムの操作の結果を自動的にユーザーへ通知しません。
        AlbumReportOption_NoReport = 0,

        //! @brief アルバムの操作に成功した場合、自動的にユーザーへ通知します。
        AlbumReportOption_ReportOnSuccess = (1 << 0),

        //! @brief アルバムの操作に失敗した場合、自動的にユーザーへ通知します。
        AlbumReportOption_ReportOnFailure = (1 << 1),

        //! @brief アルバムの操作の結果を常に自動的にユーザーへ通知します。
        AlbumReportOption_ReportAlways = AlbumReportOption_ReportOnSuccess | AlbumReportOption_ReportOnFailure,
    };

}}
