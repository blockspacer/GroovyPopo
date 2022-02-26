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
 * @brief オフライン HTML を開くのに利用される引数の宣言
 */

#pragma once

#include <nn/nn_ApplicationId.h>
#include <nn/nn_Result.h>
#include <nn/account/account_Types.h>
#include <nn/web/web_OfflineTypes.h>

namespace nn { namespace web {

//--------------------------------------------------------------------------
/**
 * @brief オフライン HTML を開くための情報を構築するクラスです。
 */
class ShowOfflineHtmlPageArg
{
public:
    /**
     * @brief NULL 終端を含むドキュメントパスの最大バイト数です。
     */
    static const size_t DocumentPathMaxLength = 3072;

    /**
     * @brief オフライン HTML を開く為の最低限の情報を持つクラスを作成します。
     *
     * @param[in] pDocumentPath 開きたいオフライン HTML のパスを指定してください。
     *
     * @details pDocumentPath は NULL 終端されている必要があります。
     *          DocumentPathMaxLength -1 文字以下にする必要があります。
     */
    explicit ShowOfflineHtmlPageArg(const char* pDocumentPath) NN_NOEXCEPT;

    /**
     * @brief (削除予定) オフライン HTML を開く為の最低限の情報を持つクラスを作成します。
     *
     * @deprecated 本 API は削除予定です。
     *          代わりに ShowOfflineHtmlPageArg(const char* pDocumentPath)
     *          を使用してください。
     *
     * @param[in] id            ApplicationId を指定してください。
     * @param[in] pDocumentPath 開きたいオフライン HTML のパスを指定してください。
     *
     * @details 引数で指定した ApplicationId は使用されず、起動元の ApplicationId が使われます。
     *          pDocumentPathは NULL 終端されている必要があります。
     *          DocumentPathMaxLength -1 文字以下にする必要があります。
     */
    explicit ShowOfflineHtmlPageArg(const nn::ApplicationId& id,
        const char* pDocumentPath) NN_NOEXCEPT;

    /**
     * @brief 動画専用起動とするかどうかを指定します。
     *
     * @param[in]   bootAsMediaPlayer  動画専用起動とするかどうか
     *
     * @details 読み込み先が動画ファイルであり、ウェブページを介さずに
     *          直接動画再生を開始したい場合に true を指定してください。
     */
    void SetBootAsMediaPlayer(bool bootAsMediaPlayer) NN_NOEXCEPT;

    /**
     * @brief 左スティックモードをポインターに切り替える事を許可するかどうかを設定します。
     *
     * @param[in]   pointerEnabled  左スティックモードをポインターに切り替える事を許可する場合は true を指定してください。
     *
     * @details デフォルト値は true です。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetPointerEnabled(const bool pointerEnabled) NN_NOEXCEPT;

    /**
     * @brief 起動時の左スティックモードを設定します。
     *
     * @param[in]   mode    起動時の左スティックモードを指定してください。
     *
     * @details SetPointerEnabled を false にした場合はここでの設定に関わらず空間ナビが使用されます。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetLeftStickMode(const OfflineLeftStickMode& mode) NN_NOEXCEPT;

    /**
     * @brief フッターを利用するかどうかを設定します。
     *
     * @param[in]   footerEnabled   フッターを利用する場合は true を指定してください。
     *
     * @details false を指定した場合はフッターが表示されません。
     *          デフォルト値は true です。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetFooterEnabled(const bool footerEnabled) NN_NOEXCEPT;

    /**
     * @brief フッター表示のデフォルトの制御方法を設定します。
     *
     * @param[in]   offlineFooterFixedKind  フッター表示の制御方法を指定してください。
     *
     * @details デフォルト値は OfflineFooterFixedKind_Auto です。
     *          SetFooterEnabled(false) とした場合はそちらの動作が優先されます。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetFooterFixedKind(const OfflineFooterFixedKind offlineFooterFixedKind) NN_NOEXCEPT;

    /**
     * @brief ページの背景を設定します。
     *
     * @param[in] backgroundKind    背景を指定してください。
     *
     * @details デフォルト値は BackgroundKind_Normal です。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetBackgroundKind(const OfflineBackgroundKind& backgroundKind) NN_NOEXCEPT;

    /**
     * @brief 起動画面種類を指定します。
     *
     * @param[in]   bootDisplayKind  起動画面種類を指定してください。
     *
     * @details 二度呼び出した場合、値は上書きされます。
     */
    void SetBootDisplayKind(const OfflineBootDisplayKind bootDisplayKind) NN_NOEXCEPT;

    /**
     * @brief キーリピートのフレーム数を設定します。
     *
     * @param[in]   delayFrame  キーリピート開始までのフレーム数を指定してください。
     * @param[in]   pulseFrame  キーリピート間隔フレーム数を指定してください。
     *
     * @details pulseFrame に 0 を指定した場合はキーリピート自体が無効になります。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetKeyRepeatFrame(int delayFrame, int pulseFrame) NN_NOEXCEPT;

    /**
     * @brief JS 拡張を利用するかどうかを設定します。
     *
     * @param[in] jsExtensionEnabled JS 拡張を利用する場合は true を指定してください。
     *
     * @details 二度呼び出した場合、値は上書きされます。
     *          デフォルト値は false です。
     *          JS 拡張の詳細については NintendoSDK ドキュメントのオフライン Web アプレットの項目を参照してください。
     */
    void SetJsExtensionEnabled(const bool jsExtensionEnabled) NN_NOEXCEPT;

    /**
     * @brief Web コンテンツのタッチ操作を有効にするかどうかを設定します。
     *
     * @param[in] touchEnabled Web コンテンツのタッチ操作を有効にする場合は true を指定してください。
     *
     * @details false を指定した場合は Web コンテンツのタッチ操作が無効になります。
     *          二度呼び出した場合、値は上書きされます。
     *          デフォルト値は true です。
     */
    void SetTouchEnabledOnContents(const bool touchEnabled) NN_NOEXCEPT;

    /**
     * @brief 動画が最後まで再生された際に、動画プレイヤーを自動的に終了するかどうかを設定します。
     *
     * @param[in] autoCloseEnabled 自動終了を有効にする場合は true を指定してください。
     *
     * @details デフォルト値は false です。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetMediaPlayerAutoCloseEnabled(const bool autoCloseEnabled) NN_NOEXCEPT;

    /**
     * @brief ページキャッシュを有効化するかどうかを設定します。
     *
     * @param[in] pageCacheEnabled ページキャッシュを有効にする場合は true を指定してください。
     *
     * @details デフォルト値は true です。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetPageCacheEnabled(const bool pageCacheEnabled) NN_NOEXCEPT;

    /**
     * @brief ページ間フェードを有効化するかどうかを設定します。
     *
     * @param[in] pageFadeEnabled ページ間フェードを有効にする場合は true を指定してください。
     *
     * @details デフォルト値は true です。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetPageFadeEnabled(const bool pageFadeEnabled) NN_NOEXCEPT;

    /**
     * @brief Web Audio を利用できるようにするかどうかを設定します。
     *
     * @param[in] webAudioEnabled Web Audio を有効にする場合は true を指定してください。
     *
     * @details デフォルト値は false です。
     *          二度呼び出した場合、値は上書きされます。
     *          Web Audio のサポートは限定的であることにご注意ください。
     *          詳しくは、NintendoSDK ドキュメント「機能 > ライブラリアプレット > Webアプレット・オフラインWebアプレット > 共通する情報」の「Web Audioを使ってSEを鳴らす」を参照してください。
     */
    void SetWebAudioEnabled(const bool webAudioEnabled) NN_NOEXCEPT;

    /**
     * @brief 起動時のローディングアイコンを表示するかどうか設定します。
     *
     * @param[in]   bootLoadingIconEnabled  ローディングアイコンを表示する場合は true を指定してください。
     *
     * @details デフォルト値は true です。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetBootLoadingIconEnabled(const bool bootLoadingIconEnabled) NN_NOEXCEPT;

    /**
     * @brief ページにスクロールインジケータを表示するかどうかを設定します。
     *
     * @param[in]   pageScrollIndicatorEnabled  ページにスクロールインジケータを表示する場合は true を指定してください。
     *
     * @details デフォルト値は false です。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetPageScrollIndicatorEnabled(const bool pageScrollIndicatorEnabled) NN_NOEXCEPT;

    /**
     * @brief メディアプレイヤーで再生速度を変更できるようにするかどうかを設定します。
     *
     * @param[in]   mediaPlayerSpeedControlEnabled  メディアプレイヤーで再生速度を変更できるようにする場合は true を指定してください。
     *
     * @details デフォルト値は false です。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetMediaPlayerSpeedControlEnabled(const bool mediaPlayerSpeedControlEnabled) NN_NOEXCEPT;

    /**
     * @brief メディア自動再生を可能とするかどうかを指定します。
     *
     * @param[in]   autoPlayEnabled  メディア自動再生を可能とするかどうか
     *
     * @details true を指定することで、ユーザー操作を伴わないメディアの再生が許可されます。
     *          デフォルト値は false です。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetMediaAutoPlayEnabled(const bool autoPlayEnabled) NN_NOEXCEPT;

    /**
     * @brief Web Audio のボリュームを設定します。
     *
     * @param[in]   webAudioVolume  Web Audio のボリューム
     *
     * @details 本関数を呼び出さなかった場合、Web Audio は @ref nn::oe::SetExpectedVolumeBalance で設定されたボリュームで再生されます。
     *
     * @pre
     *     webAudioVolume >= 0.0 && webAudioVolume <= 4.0
     */
    void OverrideWebAudioVolume(const float webAudioVolume) NN_NOEXCEPT;

    /**
     * @brief メディア再生時のボリュームを設定します。
     *
     * @param[in]   mediaAudioVolume  メディア再生時のボリューム
     *
     * @details 本関数を呼び出さなかった場合、メディア再生は @ref nn::oe::SetExpectedVolumeBalance で設定されたボリュームで再生されます。
     *
     * @pre
     *     mediaAudioVolume >= 0.0 && mediaAudioVolume <= 4.0
     */
    void OverrideMediaAudioVolume(const float mediaAudioVolume) NN_NOEXCEPT;

    //! @brief 内部用機能のため使用禁止です。
    uint8_t* Data() NN_NOEXCEPT;

protected:
    /**
     * @internal
     * @brief 情報を格納するために使用するデータのサイズ(バイト)です。
     */
    static const size_t DataSize = 8192;

    /**
     * @internal
     */
    NN_ALIGNAS(4) uint8_t m_Data[DataSize];

private:
    /**
     * @internal
     */
    ShowOfflineHtmlPageArg() NN_NOEXCEPT;
};

//--------------------------------------------------------------------------
/**
 * @brief オフライン HTML を開いた結果の情報を提供するクラスです。
 */
class OfflineHtmlPageReturnValue
{
public:
    /**
     * @brief NULL 終端を含む最後に開いていたページの URL の最大バイト数です。
     */
    static const size_t LastUrlMaxLength = 4096;

    /**
     * @brief 有効な情報を持たない状態でクラスを生成します。
     */
    OfflineHtmlPageReturnValue() NN_NOEXCEPT;

    /**
     * @brief 終了理由を返します。
     *
     * @return 終了理由を表す定数(@ref OfflineExitReason)
     */
    OfflineExitReason GetOfflineExitReason() const NN_NOEXCEPT;

    /**
     * @brief 最後に開いていたページの URL を返します。
     *
     * @return ページの URL
     *
     * @pre
     *     GetOfflineExitReason() == OfflineExitReason_CallbackUrlReached
     *
     * @details
     *     戻り値は @ref GetLastUrlSize() を超えないサイズのデータとなります。
     *     それを超えるサイズ分のデータに含まれる内容は未定義です。
     */
    const char* GetLastUrl() const NN_NOEXCEPT;

    /**
     * @brief 最後に開いていたページの URL のバイト数を返します。
     *
     * @return URL のバイト数
     *
     * @pre
     *     GetOfflineExitReason() == OfflineExitReason_CallbackUrlReached
     */
    size_t GetLastUrlSize() const NN_NOEXCEPT;

protected:
    /**
     * @internal
     */
    uint64_t m_ExitReason;
    /**
     * @internal
     */
    char m_LastUrl[LastUrlMaxLength];
    /**
     * @internal
     */
    uint64_t m_LastUrlSize;
};

//--------------------------------------------------------------------------
/**
 * @brief (廃止予定) オフライン HTML を開くための情報を構築するクラスです。
 * @deprecated 型名「ShowApplicationHtmlDocumentPageArg」は廃止予定です。ShowOfflineHtmlPageArg を利用してください。
 */
NN_DEPRECATED typedef ShowOfflineHtmlPageArg ShowApplicationHtmlDocumentPageArg;

//--------------------------------------------------------------------------
/**
 * @brief (廃止予定) オフライン HTML を開いた結果の情報を提供するクラスです。
 * @deprecated 型名「OfflinePageReturnValue」は廃止予定です。OfflineHtmlPageReturnValue を利用してください。
 */
NN_DEPRECATED typedef OfflineHtmlPageReturnValue OfflinePageReturnValue;

}} // namespace nn::web
