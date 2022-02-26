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
 * @brief  アルバムファイルの作成に関する定義
 */

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/account/account_Types.h>

#include "album_CommonApi.h"
#include "album_ImageSize.h"
#include "album_ImageOrientation.h"
#include "album_AlbumReportOption.h"

namespace nn{ namespace album{

    //! @brief 投稿 API に指定する既定の投稿文字列の最大サイズ（バイト、終端文字を含む）です。
    static const size_t PresetPositingMessageSizeMax = 256;

    //! @brief 画像データを画面写真としてアルバムに保存します。
    //! @param[in] pImageData     画面写真として保存する画像データです。
    //! @param[in] imageDataSize  画像データの大きさ（バイト）です。
    //! @param[in] screenshotSize 保存する画面写真のサイズです。
    //! @param[in] reportOption   保存の結果を自動的にユーザーに通知する設定です。
    //! @details
    //! 事前条件、各引数や戻り値の詳細は@n
    //! nn::album::SaveScreenshot(const void*,size_t,ImageSize,ImageOrientation,AlbumReportOption)
    //! を参照してください。
    nn::Result SaveScreenshot(
        const void* pImageData,
        size_t imageDataSize,
        ImageSize screenshotSize,
        AlbumReportOption reportOption
    ) NN_NOEXCEPT;

    //! @brief 向き情報を指定した画像データを画面写真としてアルバムに保存します。
    //! @param[in] pImageData     画面写真として保存する画像データです。
    //! @param[in] imageDataSize  画像データの大きさ（バイト）です。
    //! @param[in] screenshotSize 保存する画面写真のサイズです。
    //! @param[in] imageOrientation 画像の向きの指定です。
    //! @param[in] reportOption   保存の結果を自動的にユーザーに通知する設定です。
    //! @pre アルバムライブラリが使用可能
    //! @pre pImageData != nullptr
    //! @pre imageDataSize >= 4 * GetImageWidth(screenshotSize) * GetImageHeight(screenshotSize)
    //! @retval ResultAlbumFull           アルバムの容量が不足しているため保存に失敗しました。
    //! @retval ResultAlbumFileCountLimit アルバムに保存できるファイル数の上限に達しているため保存に失敗しました。
    //! @retval ResultAlbumError          その他のエラーにより保存に失敗しました。
    //! @details
    //! 画像データを画面写真としてアルバムに保存します。
    //!
    //! pImageData には保存する画像データの先頭へのポインタを渡します。
    //! 渡す画像のフォーマットは sRGB 色空間で表現された各チャンネルが 8 ビットの RGBA 画像です。
    //! イメージフォーマットとして nn::gfx::ImageFormat_R8_G8_B8_A8_UnormSrgb を指定し、タイルモードを nn::gfx::TileMode_Linear とした
    //! 左上原点、パディングなし（ stride == 4 * GetImageWidth(screenshotSize) ）の画像データに相当します。
    //!
    //! pImageData には CPU キャッシュが有効な GPU のメモリプール上のメモリを渡すことができます。
    //!
    //! screenshotSize には保存する画面写真の大きさを指定します。
    //! この API は画像データのスケーリングを行わないため、 pImageData が表現する画像の大きさは画面写真の大きさと一致する必要があります。
    //!
    //! imageOrientation には列挙型 nn::album::ImageOrientation の中から画像の回転方向を指定します。
    nn::Result SaveScreenshot(
        const void* pImageData,
        size_t imageDataSize,
        ImageSize screenshotSize,
        ImageOrientation imageOrientation,
        AlbumReportOption reportOption
    ) NN_NOEXCEPT;

    //! @brief 画像データを画面写真としてアルバムに保存するとともに SNS に投稿します。
    //! @param[in] pImageData     画面写真として保存する画像データです。
    //! @param[in] imageDataSize  画像データの大きさ（バイト）です。
    //! @param[in] screenshotSize 保存する画面写真のサイズです。
    //! @param[in] reportOption   保存の結果を自動的にユーザーに通知する設定です。
    //! @param[in] uid            画面写真の投稿に使用するユーザー ID です。
    //! @details
    //! 事前条件、各引数や戻り値の詳細は@n
    //! nn::album::SaveAndShareScreenshot(const void*,size_t,ImageSize,ImageOrientation,AlbumReportOption,const nn::account::Uid&,const char*)
    //! を参照してください。
    nn::Result SaveAndShareScreenshot(
        const void* pImageData,
        size_t imageDataSize,
        ImageSize screenshotSize,
        AlbumReportOption reportOption,
        const nn::account::Uid& uid
    ) NN_NOEXCEPT;

    //! @brief 向き情報を指定した画像データを画面写真としてアルバムに保存するとともに SNS に投稿します。
    //! @param[in] pImageData     画面写真として保存する画像データです。
    //! @param[in] imageDataSize  画像データの大きさ（バイト）です。
    //! @param[in] screenshotSize 保存する画面写真のサイズです。
    //! @param[in] imageOrientation 画像の向きの指定です。
    //! @param[in] reportOption   保存の結果を自動的にユーザーに通知する設定です。
    //! @param[in] uid            画面写真の投稿に使用するユーザー ID です。
    //! @details
    //! 事前条件、各引数や戻り値の詳細は@n
    //! nn::album::SaveAndShareScreenshot(const void*,size_t,ImageSize,ImageOrientation,AlbumReportOption,const nn::account::Uid&,const char*)
    //! を参照してください。
    nn::Result SaveAndShareScreenshot(
        const void* pImageData,
        size_t imageDataSize,
        ImageSize screenshotSize,
        ImageOrientation imageOrientation,
        AlbumReportOption reportOption,
        const nn::account::Uid& uid
    ) NN_NOEXCEPT;

    //! @brief 画像データを画面写真としてアルバムに保存するとともに SNS に投稿します。任意の投稿コメントを指定できます。
    //! @param[in] pImageData     画面写真として保存する画像データです。
    //! @param[in] imageDataSize  画像データの大きさ（バイト）です。
    //! @param[in] screenshotSize 保存する画面写真のサイズです。
    //! @param[in] reportOption   保存の結果を自動的にユーザーに通知する設定です。
    //! @param[in] uid            画面写真の投稿に使用するユーザー ID です。
    //! @param[in] presetPostingMessage SNS 投稿コメントに既定文として指定する文字列です。
    //! @details
    //! 事前条件、各引数や戻り値の詳細は@n
    //! nn::album::SaveAndShareScreenshot(const void*,size_t,ImageSize,ImageOrientation,AlbumReportOption,const nn::account::Uid&,const char*)
    //! を参照してください。
    nn::Result SaveAndShareScreenshot(
        const void* pImageData,
        size_t imageDataSize,
        ImageSize screenshotSize,
        AlbumReportOption reportOption,
        const nn::account::Uid& uid,
        const char* presetPostingMessage
    ) NN_NOEXCEPT;

    //! @brief 向き情報を指定した画像データを画面写真としてアルバムに保存するとともに SNS に投稿します。任意の投稿コメントを指定できます。
    //! @param[in] pImageData     画面写真として保存する画像データです。
    //! @param[in] imageDataSize  画像データの大きさ（バイト）です。
    //! @param[in] screenshotSize 保存する画面写真のサイズです。
    //! @param[in] imageOrientation 画像の向きの指定です。
    //! @param[in] reportOption   保存の結果を自動的にユーザーに通知する設定です。
    //! @param[in] uid            画面写真の投稿に使用するユーザー ID です。
    //! @param[in] presetPostingMessage SNS 投稿コメントに既定文として指定する文字列です。
    //! @pre アルバムライブラリが使用可能
    //! @pre pImageData != nullptr
    //! @pre imageDataSize >= 4 * GetImageWidth(screenshotSize) * GetImageHeight(screenshotSize)
    //! @retval ResultAlbumFull           アルバムの容量が不足しているため保存に失敗しました。
    //! @retval ResultAlbumFileCountLimit アルバムに保存できるファイル数の上限に達しているため保存に失敗しました。
    //! @retval ResultAlbumError          その他のエラーにより保存に失敗しました。
    //! @retval ResultShareCanceled       投稿がキャンセルされました。
    //! @retval ResultShareFailed         投稿に失敗しました。
    //! @details
    //! 画像データを画面写真としてアルバムに保存して SNS へ投稿します。
    //!
    //! この API は同期的に SNS への投稿処理を行います。
    //! 投稿処理は投稿画面を用いてユーザーと対話的に行われるため、呼出元は長時間ブロックされます。
    //!
    //! pImageData には保存する画像データの先頭へのポインタを渡します。
    //! 渡す画像のフォーマットは sRGB 色空間で表現された各チャンネルが 8 ビットの RGBA 画像です。
    //! イメージフォーマットとして nn::gfx::ImageFormat_R8_G8_B8_A8_UnormSrgb を指定し、タイルモードを nn::gfx::TileMode_Linear とした
    //! 左上原点、パディングなし（ stride == 4 * GetImageWidth(screenshotSize) ）の画像データに相当します。
    //!
    //! pImageData には CPU キャッシュが有効な GPU のメモリプール上のメモリを渡すことができます。
    //!
    //! screenshotSize には保存する画面写真の大きさを指定します。
    //! この API は画像データのスケーリングを行わないため、 pImageData が表現する画像の大きさは画面写真の大きさと一致する必要があります。
    //!
    //! imageOrientation には列挙型 nn::album::ImageOrientation の中から画像の回転方向を指定します。
    //!
    //! reportOption には列挙型 nn::album::AlbumReportOption の中から所望の設定を指定します。
    //!
    //! presetPostingMessage には、投稿コメントに対してアプリ側から任意の文字列を指定することができます。
    //! 指定可能な最大サイズは ::PresetPositingMessageSizeMax で定義されています。
    //! 指定された値は utf-8 でエンコードされた文字列として解釈されます。
    //! 投稿先の SNS によっては、コメントを設定できない場合があります。
    //!
    //! 画面写真の保存に成功した場合、 システムが表示する UI を経由して SNS への投稿を行います。
    //! ユーザー操作による中断などで投稿が完了しなかった場合でも保存された画面写真はアルバム内に残ります。
    nn::Result SaveAndShareScreenshot(
        const void* pImageData,
        size_t imageDataSize,
        ImageSize screenshotSize,
        ImageOrientation imageOrientation,
        AlbumReportOption reportOption,
        const nn::account::Uid& uid,
        const char* presetPostingMessage
    ) NN_NOEXCEPT;
}}

