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
#include <nn/os/os_Tick.h>
#include <nn/album/album_ImageOrientation.h>
#include <nn/os/os_MemoryHeapCommon.h>
#include <nn/oe/oe_NotificationMessage.h>

namespace nn { namespace oe {

/**
 * @brief   フレームバッファウィンドウの原点位置を示す列挙型です。
 *
 */
enum WindowOriginMode
{
    WindowOriginMode_LowerLeft = 0, //!< 左下を原点とするモード
    WindowOriginMode_UpperLeft = 1, //!< 左上を原点とするモード

};

/**
 * @brief   権利表記用フレームバッファとして必要なサイズを示す定数です。
 *
 */
const size_t CopyrightFrameBufferSize = 0x3c0000;

/**
 * @brief   権利表記用フレームバッファの横幅ピクセル数を示す定数です。
 *
 */
const int CopyrightFrameBufferWidth = 1280;

/**
 * @brief   権利表記用フレームバッファの縦幅ピクセル数を示す定数です。
 *
 */
const int CopyrightFrameBufferHeight = 720;


/**
 * @brief   スクリーンショット撮影時に通知されるメッセージです。
 *
 */
const Message MessageAlbumScreenShotTaken = 92;

/**
 * @brief   常時動画撮影されている動画ファイルの保存時に通知されるメッセージです。
 *
 */
const Message MessageAlbumRecordingSaved = 93;


//-----------------------------------------------------------------------------

//! @name スクリーンショット撮影機能に関する API
//! @{

/**
 * @brief   キャプチャーボタンによるスクリーンショット撮影を不許可にします。
 *
 * @details
 *  画面に表示されている画像のスクリーンショット撮影を不許可にします。
 *
 *  アプリケーションは、アプリケーション管理データの \<Screenshot\> の項目に
 *  よって撮影許可状態に関する設定を変更できます。@n
 *  上記項目が「Allow」に設定されている場合は、スクリーンショット撮影は
 *  プログラム起動時から許可となっており、本 API で一時的に不許可にすることができます。@n
 *  上記項目が「Deny」に設定されている場合は、スクリーンショット撮影は
 *  常に不許可となり、本 API を発行しても無視されます。
 *
 *  詳細は、@confluencelink{166503043, 「nmeta ファイルの書き方 」} も参照してください。
 *
 *  なお、本 API によって常時動画撮影も一時的に不許可となります。
 *  常時動画撮影については nn::oe::EnableRecording() を参照して下さい。
 *
 *
 */
void DisableScreenShot() NN_NOEXCEPT;


/**
 * @brief   キャプチャーボタンによるスクリーンショット撮影を許可します。
 *
 * @details
 *  画面に表示されている画像のスクリーンショット撮影を許可します。
 *  本 API は nn::oe::DisableScreenShot() で一時的に不許可にした
 *  スクリーンショット撮影を再度許可する場合に使用します。
 *
 *  詳細は nn::oe::DisableScreenShot() のリファレンスを参照して下さい。
 *
 */
void EnableScreenShot() NN_NOEXCEPT;


/**
 * @deprecated
 * @brief   キャプチャーボタンで撮影するスクリーンショット画像の回転方向を指定します。
 *
 * @details
 *  この API の使用は非推奨となりました。
 *  代わりに nn::oe::SetAlbumImageOrientation() をご使用ください。
 *
 */
NN_DEPRECATED void SetScreenShotImageOrientation(album::ImageOrientation orientation) NN_NOEXCEPT;


/**
 * @brief   キャプチャーボタンで撮影する静止画や動画の回転方向を指定します。
 *
 * @param[in]  orientation  静止画や動画に適用する回転方向
 *
 * @details
 *  キャプチャーボタンで撮影・保存される静止画ファイルや動画ファイルに対して、
 *  orientation で指定された回転方向の情報を埋め込みます。
 *  デフォルトでは nn::album::ImageOrientation_None が適用されています。
 *
 *  回転方向の詳細については、
 *  @confluencelink{232856843, 「ALBUM ライブラリ／画面写真機能の imageOrientation」} を参照してください。
 *
 */
void SetAlbumImageOrientation(album::ImageOrientation orientation) NN_NOEXCEPT;


/**
 * @brief   スクリーンショット撮影通知および動画保存通知機能を有効にします。
 *
 * @details
 *  キャプチャボタン短押しによるスクリーンショット撮影や、長押しによる動画保存が
 *  行われた際に、プログラムに対して以下のメッセージ通知を行なう機能を有効にします。
 *  プログラム起動時のデフォルトは無効となっています。
 *
 *  - スクリーンショット撮影時に oe::MessageAlbumScreenShotTaken を通知
 *  - 常時動画撮影の保存時に oe::MessageAlbumRecordingSaved を通知
 *
 *  本機能は、ユーザによるスクリーンショット撮影や動画保存が行われたタイミングが
 *  ゲーム内のどのようなシーンかを収集して行動分析に使用する用途を想定しています。
 *  本機能によってキャプチャボタン押下をハンドリングできるようになるわけではありません。
 *
 *  また、スクリーンショット撮影や動画撮影が禁止されている場合は、
 *  上記のメッセージは通知されません。
 *
 *  @b 注意事項
 *
 *  以下の理由により、本機能で通知されるメッセージを以てユーザに正確なフィードバックを
 *  行うことはできません。前記で示した想定用途（行動分析等）以外には使用しないで下さい。
 *
 *  - 短時間に多くのスクリーンショット撮影や動画保存が行われたような場合には、
 *    まとめて一度しか通知されないことがあります。
 *  - ユーザのキャプチャボタン操作により撮影が行われた場合に、保存先ストレージの残容量や
 *    ストレージ自体の異常によって画像ファイルの作成に失敗するような場合がありますが、
 *    このような状況においても通知されないことがあります。
 *
 */
void EnableAlbumImageTakenNotification() NN_NOEXCEPT;


/**
 * @brief   スクリーンショット撮影通知および動画保存通知機能を無効にします。
 *
 * @details
 *  nn::oe::EnableAlbumImageTakenNotification() によって有効になっていた
 *  スクリーンショット撮影通知や動画保存通知機能を無効にします。
 *
 */
void DisableAlbumImageTakenNotification() NN_NOEXCEPT;

//! @}

//-----------------------------------------------------------------------------

//! @name   アプリケーションの常時動画撮影機能に関する API
//! @{

/**
 * @brief   アプリケーションの常時動画撮影の録画を許可します。
 *
 * @details
 *  アプリケーションの常時動画撮影の録画を許可します。
 *  本 API は nn::oe::DisableRecording() によって一時停止していた
 *  動画撮影を再開する場合に使用することを想定しています。
 *
 *  アプリケーション起動時の動画撮影のデフォルトは許可状態ですが、
 *  アプリケーションの動画撮影は以下の全ての項目が満たされる場合にのみ
 *  録画が行なわれます。また、録画が行われている時にキャプチャボタンを
 *  長押しすることによって動画撮影ファイルが保存されます。
 *
 *  - アプリケーション管理データの \<Screenshot\> で撮影が許可されている
 *  - アプリケーション管理データの \<VideoCapture\> で動画撮影が許可されている
 *  - スクリーンショット撮影が許可されている（ nn::oe::EnableScreenShot() を参照）
 *  - 本 API で動画撮影が許可されている。
 *  - アプリケーションが インフォーカス状態 である。
 *
 *  アプリケーション管理データの設定については、
 *  @confluencelink{166503043, 「nmeta ファイルの書き方 」} を参照してください。
 *
 */
void EnableRecording() NN_NOEXCEPT;


/**
 * @brief   アプリケーションの常時動画撮影の録画を不許可にします。
 *
 * @details
 *  常時動画撮影の録画を不許可にします。
 *
 *  実際にアプリケーションの動画撮影が行なわれる条件については、
 *  nn::oe::EnableRecording() のリファレンスを参照して下さい。
 *
 */
void DisableRecording() NN_NOEXCEPT;


//! @}

//-----------------------------------------------------------------------------

//! @name   アルバム撮影画像に対する権利表記画像を合成する機能
//! @{


/**
 * @brief   アルバム撮影画像の権利表記用フレームバッファを設定します。
 *
 * @param[in] buffer      権利表記用フレームバッファ先頭アドレス
 * @param[in] bufferSize  権利表記用フレームバッファのサイズ
 *
 * @pre
 *  - (buffer % nn::os::MemoryPageSize) == 0
 *  - bufferSize == nn::oe::CopyrightFrameBufferSize
 *  - 本 API の呼出しが初めてである
 *
 * @details
 *  アルバムとして保存されるスクリーンショット画像や動画撮影の映像に対する
 *  プログラム固有の権利表記用フレームバッファを設定し、
 *  権利表記画像を合成するための機能を初期化します。
 *
 *  buffer と bufferSize には権利表記用フレームバッファに使用するメモリを指定します。
 *  メモリの内容は一旦ゼロクリアされてからシステムに渡されるため、
 *  本 API 発行直後は、全画面透明な画像が権利表記用に合成される状態となります。
 *  なお、このメモリは本 API 呼出しと同時にアクセス禁止となり、
 *  プログラムが終了するまでアクセス不可となります。
 *
 */
void InitializeCopyrightFrameBuffer(void* buffer, size_t bufferSize) NN_NOEXCEPT;


/**
 * @brief   指定された画像を権利表記用フレームバッファに転送します。
 *
 * @param[in] buffer      画像が格納されている転送元メモリの先頭アドレス
 * @param[in] bufferSize  画像が格納されている転送元メモリのサイズ
 * @param[in] x           画像の転送先の X 座標
 * @param[in] y           画像の転送先の Y 座標
 * @param[in] width       画像の横幅ピクセル数
 * @param[in] height      画像の縦幅ピクセル数
 * @param[in] originMode  画像用のフレームバッファの原点位置
 *
 * @pre
 *  - 事前に nn::oe::InitializeCopyrightFrameBuffer() を発行済みである
 *  - (buffer % nn::os::MemoryPageSize) == 0
 *  - bufferSize == width * height * 4
 *  - 転送先の矩形領域がフレームバッファ領域内である（以下全てを満たすこと）
 *    - x >= 0
 *    - y >= 0
 *    - x + width <= nn::oe::CopyrightFrameBufferWidth
 *    - y + height <= nn::oe::CopyrightFrameBufferHeight
 *
 * @details
 *  buffer, bufferSize, width, height で指定されたメモリ上にある矩形画像を
 *  権利表記用フレームバッファに転送し、アルバム画像に合成されるようにします。
 *
 *  x, y, width, height で指定される転送先の矩形領域は、
 *  nn::oe::InitializeCopyrightFrameBuffer() で指定した
 *  権利表記用フレームバッファ領域内に完全に収まる座標でなければなりません。
 *  また、転送先の上記矩形領域以外の領域は透明色で塗り潰されます。
 *
 *  originMode には nn::oe::WindowOriginMode で示される原点位置を示す
 *  列挙子を指定して下さい。この原点位置は、プログラムの出画用レイヤーのものと
 *  同じ原点位置を示すものを指定して下さい。指定が間違っている場合、
 *  正しく合成されません。
 *
 *  渡す画像のフォーマットは sRGB 色空間で表現された各チャンネルが 8 ビットの RGBA 画像です。
 *  イメージフォーマットとして nn::gfx::ImageFormat_R8_G8_B8_A8_UnormSrgb を指定し、
 *  タイルモードを nn::gfx::TileMode_Linear とした画像データに相当します。
 *  RGB 成分はアルファ値が事前乗算されていないものを渡して下さい。
 *
 */
void SetCopyrightImage(const void* buffer, size_t bufferSize, int x, int y, int width, int height, WindowOriginMode originMode) NN_NOEXCEPT;


/**
 * @brief   権利表記用の画像をアルバム画像に合成するか否かを指定します。
 *
 * @param[in] isVisible 権利表記画像を合成するか否かを bool 値で指定します
 *
 * @pre
 *  - 事前に nn::oe::InitializeCopyrightFrameBuffer() を発行済みである
 *
 * @details
 *  nn::oe::SetCopyrightImage() で指定した権利表記用の画像を、
 *  アルバム画像に合成するか否かを isVisible で指定します。
 *  true なら合成する、false なら合成しない、となります。
 *
 *  nn::oe::InitializeCopyrightFrameBuffer() 発行直後の
 *  デフォルトは true 相当になっています。
 *
 */
void SetCopyrightVisibility(bool isVisible) NN_NOEXCEPT;

//! @}

//-----------------------------------------------------------------------------

//! @name   起動パラメータに関する機能
//! @{

/**
 * @brief   プログラムの起動パラメータを取得します。
 *
 * @param[out] pOutParameterSize 起動パラメータの実サイズの格納先
 * @param[out] pOutBuffer        起動パラメータを格納するバッファのアドレス
 * @param[in]  bufferSize        起動パラメータを格納するバッファのサイズ
 *
 * @return  起動パラメータの取得に成功したら true を、そうでなければ false を返します。
 *
 * @details
 *  自プログラムに渡された起動パラメータを取得し、その成否を bool で返します。
 *  起動パラメータがない場合は、返値に false が返ります。
 *
 *  返値が true の場合、pOutBuffer と bufferSize が指すバッファ領域に
 *  起動パラメータが格納されます。また、実際の起動パラメータサイズを
 *  pOutParameterSize に格納します。
 *
 *  bufferSize が実際の起動パラメータサイズより小さい場合は、
 *  起動パラメータは bufferSize 分だけが pOutBuffer に格納されます。
 *
 *  なお、起動パラメータの内容に関しては、システムは一切関与しません。
 *
 *
 *  @b 注意事項
 *
 *  起動パラメータは、将来的にさまざまなシーンから渡される可能性があります。
 *
 *  そのため、新しいファームウェア上で古いバージョンのアプリケーションを
 *  起動した場合に、そのアプリケーションにとって未知の起動パラメータが渡される
 *  ことが起こり得ます。そのような場合でも問題がないようにするために、
 *
 *  - 起動パラメータのフォーマットを互換性や拡張性を考慮して定義しておく
 *    - 将来的に定義される起動パラメータでも渡すことが可能であること
 *    - 如何なるバージョンのアプリケーションでもハンドリングが可能であること
 *  - 起動パラメータがアプリケーションにとって未知だった場合にはそれを無視する
 *
 *  といったことを十分に検討した上でご使用下さい。
 *
 */
bool TryPopLaunchParameter(size_t* pOutParameterSize, void* pOutBuffer, size_t bufferSize) NN_NOEXCEPT;


//! @}

//-----------------------------------------------------------------------------

/**
 * @brief   無線通信のプライオリティを表す列挙型です。
 */
enum WirelessPriorityMode
{
    WirelessPriorityMode_Default = 0,            //!< プラットフォームの推奨設定
    WirelessPriorityMode_OptimizedForWlan = 1,   //!< Wlanを優先するように最適化。その他の無線通信（例：コントローラの無線通信）は品質低下
};


//! @name   無線通信のプライオリティに関する API
//! @{


/**
 * @brief   無線通信のプライオリティを設定します。
 *
 * @param[in]  wirelessPriorityMode  無線通信のプライオリティを表す列挙型
 *
 * @details
 *  互いに影響を与え合う無線通信において、どの通信を優先するかを設定します。
 *  デフォルトでは nn::oe::WirelessPriorityMode_Default が適用されています。
 */
void SetWirelessPriorityMode(WirelessPriorityMode wirelessPriorityMode) NN_NOEXCEPT;


//! @}

//-----------------------------------------------------------------------------

//! @name   プログラムの動作時間に関する API です。
//! @{

/**
 * @brief   プログラムが起動してからの累積の実動作時間を取得します。
 *
 * @return  プログラムが起動してからの累積の実動作時間を nn::TimeSpan 型で返します。
 *
 * @details
 *  プログラムが起動してから実際に動作した累積時間を nn::TimeSpan 型で返します。
 *
 *  プログラムは oe::SetFocusHandlingMode() で指定するハンドリングモードや
 *  システムによるスリープなどで動作を一時的に中断している場合があります。
 *  本 API ではこういった中断時間を除いた累積の実動作時間を算出して返します。
 *
 *  @b 注意事項
 *
 *  本 API は os::GetSystemTick() などと比べてあまり高速ではありません。
 *  そのため、本 API の呼出し頻度は目安として 1 フレームに 1 回以下程度に
 *  抑えて利用されることを推奨します。
 *
 */
TimeSpan GetProgramTotalActiveTime() NN_NOEXCEPT;


}} // namespace nn::oe

