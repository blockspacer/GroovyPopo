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
 * @brief ウェブページを開くのに利用される引数の宣言
 */

#pragma once

#include <nn/nn_Result.h>
#include <nn/account/account_Types.h>
#include <nn/web/web_Types.h>

namespace nn { namespace web {

//------------------------------------------------------------------------
/**
 * @brief ウェブページを開くための情報を構築するクラスです。
 */
class ShowWebPageArg
{
public:
    /**
     * @brief NULL 終端を含むリクエストできる URL の最大バイト数です。
     */
    static const size_t RequestUrlMaxLength = 3072;

    /**
     * @brief NULL 終端を含むコールバックとして指定できる URL の最大バイト数です。
     */
    static const size_t CallbackUrlMaxLength = 1024;

    /**
     * @brief NULL 終端を含むコールバック可能として指定できる URL の最大バイト数です。
     */
    static const size_t CallbackableUrlMaxLength = 1024;

    /**
     * @brief NULL 終端を含むユーザーエージェントへ追加する文字列として指定できる文字列の最大バイト数です。
     */
    static const size_t UserAgentAdditionalStringMaxLength = 128;

    /**
     * @brief WebPage を開くための最低限の情報を持ったクラスを作成します。
     *
     * @param[in]   pRequestUrl    開くページのURLを指定してください。
     *
     * @details 指定する値は NULL 終端されている必要があります。
     *          RequestUrlMaxLength -1 文字以下にする必要があります。
     */
    explicit ShowWebPageArg(const char* pRequestUrl) NN_NOEXCEPT;

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
     * @brief コールバック URL を設定します。
     *
     * @param[in]   pCallbackUrl    コールバックURL
     *
     * @details
     * ウェブページ上のページ遷移によって、URL が本メソッドで指定した
     * アドレスで始まるページに達した場合、ウェブページの表示を終了します。
     *
     * 指定する値は NULL 終端されている必要があります。
     * CallbackUrlMaxLength -1 文字以下にする必要があります。
     *
     * 二度呼び出した場合、値は上書きされます。
     */
    void SetCallbackUrl(const char* pCallbackUrl) NN_NOEXCEPT;

    /**
     * @brief コールバック可能 URL を設定します。
     *
     * @param[in]   pCallbackableUrl    コールバック可能 URL
     *
     * @details
     * コールバック可能 URL は、コールバック URL による表示の終了処理を
     * 行うことができるウェブページを制限するために利用します。
     * コールバック可能 URL に指定されたURLで始まるウェブページから
     * 遷移した場合のみ、コールバック URL による終了が可能となります。
     * コールバック可能 URL を指定しなかった場合は、すべてのページで
     * コールバック URL による終了処理を利用することができます。
     *
     * 指定する値は NULL 終端されている必要があります。
     * CallbackUrlMaxLength -1 文字以下にする必要があります。
     *
     * 二度呼び出した場合、値は上書きされます。
     */
    void SetCallbackableUrl(const char* pCallbackableUrl) NN_NOEXCEPT;

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
     * @brief 左スティックのモードを設定します。
     *
     * @param[in]   mode  左スティックのモードを指定します。
     *
     * @details 二度呼び出した場合、値は上書きされます。
     */
    void SetLeftStickMode(const WebLeftStickMode mode) NN_NOEXCEPT;

    /**
     * @brief Uid を設定します。
     *
     * @param[in]   uid アカウントの Uid を指定してください。
     *
     * @details 指定する値は、アカウントシステムに登録されているユーザーの Uid または
     *          nn::account::InvalidUid である必要があります。
     *          呼び出さなかった場合または nn::account::InvalidUid を指定した場合は
     *          アノニマスモードになります。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetUid(const nn::account::Uid& uid) NN_NOEXCEPT;

    /**
     * @brief フッター表示のデフォルトの制御方法を設定します。
     *
     * @param[in]   webFooterFixedKind  フッター表示の制御方法を指定してください。
     *
     * @details デフォルト値は WebFooterFixedKind_Auto です。
     *          二度呼び出した場合、値は上書きされます。
     */
    void SetFooterFixedKind(const WebFooterFixedKind webFooterFixedKind) NN_NOEXCEPT;

    /**
     * @brief ウェブページの背景を設定します。
     *
     * @param[in]   backgroundKind  背景の種類
     *
     * @details 二度呼び出した場合、値は上書きされます。
     */
    void SetBackgroundKind(const WebBackgroundKind backgroundKind) NN_NOEXCEPT;

    /**
     * @brief 起動時の画面の種類を指定します。
     *
     * @param[in]   bootDisplayKind  起動時画面の種類
     *
     * @details 二度呼び出した場合、値は上書きされます。
     */
    void SetBootDisplayKind(const WebBootDisplayKind bootDisplayKind) NN_NOEXCEPT;

    /**
     * @brief JS 拡張を利用するかどうかを設定します。
     *
     * @param[in] jsExtensionEnabled JS 拡張を利用する場合は true を指定してください。
     *
     * @details 二度呼び出した場合、値は上書きされます。
     *          デフォルト値は false です。
     *          JS 拡張の詳細については NintendoSDK ドキュメントの Web アプレットの項目を参照してください。
     */
    void SetJsExtensionEnabled(const bool jsExtensionEnabled) NN_NOEXCEPT;

    /**
     * @brief 画面に表示される URL の表記形式を指定します。
     *
     * @param[in] displayUrlKind URL 表記の形式
     *
     * @details
     * 二度呼び出した場合、値は上書きされます。
     * デフォルト値は WebDisplayUrlKind_FullPath です。
     *
     * 画面上にウェブページの URL が表示される際に、ここで指定した形式が適用されます。
     * 表示するウェブページのホストが開発者の管理下にない場合、
     * WebDisplayUrlKind_FullPath 以外へ指定を変更しないようにしてください。
     */
    void SetDisplayUrlKind(const WebDisplayUrlKind displayUrlKind) NN_NOEXCEPT;

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
     * @brief ユーザーエージェントへ追加する文字列を設定します。
     *
     * @param[in] pString ユーザーエージェントへ追加する文字列
     *
     * @details
     * ユーザーエージェントへ追加する文字列を設定します。
     *
     * 指定する値は NULL 終端されている必要があります。
     * UserAgentAdditionalStringMaxLength - 1 文字以下にする必要があります。
     *
     * 二度呼び出した場合、値は上書きされます。
     */
    void SetUserAgentAdditionalString(const char* pString) NN_NOEXCEPT;

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
     */
    void SetWebAudioEnabled(const bool webAudioEnabled) NN_NOEXCEPT;

    /**
     * @brief ウェブページにスクロールインジケータを表示するかどうかを設定します。
     *
     * @param[in]   pageScrollIndicatorEnabled  ウェブページにスクロールインジケータを表示する場合は true を指定してください。
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
    ShowWebPageArg() NN_NOEXCEPT;
};

//--------------------------------------------------------------------------
/**
 * @brief ウェブページを表示した結果の情報を提供するクラスです。
 */
class WebPageReturnValue
{
public:
    /**
     * @brief NULL 終端を含む最後に開いていたページの URL の最大バイト数です。
     */
    static const size_t LastUrlMaxLength = 4096;

    /**
     * @brief 有効な情報を持たない状態でクラスを生成します。
     */
    WebPageReturnValue() NN_NOEXCEPT;

    /**
     * @brief 終了理由を返します。
     *
     * @return 終了理由を表す定数(@ref WebExitReason)
     */
    WebExitReason GetWebExitReason() const NN_NOEXCEPT;

    /**
     * @brief 最後に開いていたページの URL を返します。
     *
     * @return ページの URL
     *
     * @pre
     *     GetWebExitReason() == WebExitReason_CallbackUrlReached
     *
     * @details
     *     戻り値は @ref GetLastUrlSize() を超えないサイズのデータとなります。
     *     それを超えるサイズ分のデータに含まれる内容は未定義です。
     */
    const char* GetLastUrl() const NN_NOEXCEPT;

    /**
     * @brief 最後に開いていたページの URL のバイト数を返します。
     *
     * @return URL のサイズ
     *
     * @pre
     *     GetWebExitReason() == WebExitReason_CallbackUrlReached
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

}} // namespace nn::web
