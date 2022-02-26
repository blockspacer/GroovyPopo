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
 * @brief   VibrationPlayer クラスの定義
 */

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_Result.h>
#include <nn/hid/hid_VibrationNode.h>
#include <nn/hid/hid_VibrationFileApi.h>

namespace nn { namespace hid {

/**
 * @brief   振動ファイルを再生することで振動値の発生源としての機能を提供する振動ノードです。
 *
 * @details
 *  VibrationPlayer は、あらかじめメモリ上に展開された振動ファイルのバイナリデータを
 *  ロードして再生することによって振動を発生させる機能を提供する振動ノードです。@n
 *  内部的には、 @ref nn::hid::ParseVibrationFile 関数を利用して振動ファイルをパースし、
 *  @ref nn::hid::RetrieveVibrationValue 関数を利用して振動ファイルから振動値を順次取り出します。
 *
 *  振動ファイルから取り出された振動値は、
 *  @ref nn::hid::VibrationNode::Update が呼ばれるたびに
 *  接続先の振動ノードに伝えられます。
 */
class VibrationPlayer : public VibrationNode
{
    NN_DISALLOW_COPY(VibrationPlayer);
    NN_DISALLOW_MOVE(VibrationPlayer);

public:
    /**
     * @brief       コンストラクタです。@n
     *              本関数はスレッドセーフではありません。@n
     *              本関数と@ref nn::hid::VibrationNode::Update 関数を同時に呼ばないように排他制御をしてください。
     *
     * @post
     *              - IsLoaded() == false
     */
    VibrationPlayer() NN_NOEXCEPT;

    /**
     * @brief       デストラクタです。@n
     *              本関数はスレッドセーフではありません。@n
     *              本関数と@ref nn::hid::VibrationNode::Update 関数を同時に呼ばないように排他制御をしてください。
     */
    virtual ~VibrationPlayer() NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief       VibrationPlayer に振動ファイルをロードします。
     *
     * @return      処理の結果が返ります。
     * @retval      nn::ResultSuccess                       処理が正常に終了しました。
     * @retval      nn::hid::ResultVibrationFileInvalid     振動ファイルのバイナリデータが不正です。
     *
     * @param[in]   address     振動ファイルのバイナリデータが展開されているメモリの先頭アドレス
     * @param[in]   fileSize    振動ファイルのサイズ
     *
     * @pre
     *              - address != nullptr
     *              - address を先頭とする連続領域に、
     *                振動ファイルから読み込んだ有効な振動ファイルのバイナリデータが展開されている
     *              - fileSize が読み込んだ振動ファイルのサイズ以上
     *
     * @post
     *              - IsLoaded() == true
     *              - IsPlaying() == false
     *              - GetCurrentPosition() == 0
     *              - GetFileInfo() で振動ファイルの情報を取得可能な状態
     *              - IsLoop() == GetFileInfo().isLoop
     *              - GetLoopStartPosition() == GetFileInfo().loopStartPosition
     *              - GetLoopEndPosition() == GetFileInfo().loopEndPosition
     *              - GetLoopInterval() == GetFileInfo().loopInterval
     *
     * @details
     *  振動ファイルのバイナリデータが展開されている address から fileSize バイトの領域は、
     *   Unload() を呼び出すまでアプリケーション側で書き換えないようにする必要があります。
     */
    Result Load(const void* address, size_t fileSize) NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer にロードされた振動ファイルをアンロードします。
     *
     * @pre
     *              - IsLoaded() == true
     *
     * @post
     *              - IsLoaded() == false
     */
    void Unload() NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer に振動ファイルがロード済みか取得します。
     *
     * @return      振動ファイルがロード済みならば true を返します。
     */
    bool IsLoaded() const NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer にロードされた振動ファイルの情報を取得します。
     *
     * @return      振動ファイルの情報を返します。
     *
     * @pre
     *              - IsLoaded() == true
     */
    VibrationFileInfo GetFileInfo() const NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer にロードされた振動ファイルの再生を開始します。
     *
     * @pre
     *              - IsLoaded() == true
     *              - IsPlaying() == false
     *              - 0 <= GetCurrentPosition() < GetFileInfo().sampleLength
     *              - IsLoop() == true の場合には、
     *                0 <= GetLoopStartPosition() < GetLoopEndPosition() <= ロードされている振動ファイルのサンプル長
     *
     * @post
     *              - IsPlaying() == true
     *
     * @details
     *  VibrationPlayer にロードされた振動ファイルを CurrentPosition から順に
     *  デコードして振動値を生成します。@n
     *  VibrationPlayer が別の振動ノードに接続されている場合には、
     *  @ref nn::hid::VibrationNode::Update が呼ばれるたびに
     *  接続先の振動ノードに対して生成された振動値が伝えられます。
     *
     *  IsLoop() == false の場合には、
     *  振動ファイル内の振動サンプル末尾まで再生が完了した時点で自動的に再生を停止して、
     *  IsPlaying() == false かつ GetCurrentPosition() == 0 となります。@n
     *  IsLoop() == true の場合には、
     *  明示的に Stop() を呼ばない限り振動ファイルがループ再生され続けます。
     */
    void Play() NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer にロードされた振動ファイルの再生を停止します。
     *
     * @pre
     *              - IsLoaded() == true
     *              - IsPlaying() == true
     *
     * @post
     *              - IsPlaying() == false
     */
    void Stop() NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer が振動ファイルを再生中か取得します。
     *
     * @return      振動ファイルを再生中ならば true を返します。
     *
     * @pre
     *              - IsLoaded() == true
     */
    bool IsPlaying() const NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer にロードされた振動ファイルの再生位置を指定します。
     *
     * @param[in]   position    振動ファイルの再生位置を指定します。単位はサンプルです。
     *
     * @pre
     *              - IsLoaded() == true
     *              - 0 <= position < ロードされている振動ファイルのサンプル長
     *
     * @post
     *              - GetCurrentPosition() = position
     */
    void SetCurrentPosition(int position) NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer にロードされた振動ファイルの再生位置を取得します。
     *
     * @return      振動ファイルの再生位置を返します。単位はサンプルです。
     *
     * @pre
     *              - IsLoaded() == true
     */
    int GetCurrentPosition() const NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer がループ再生をするか指定します。
     *
     * @param[in]   isLoop      ループ再生をする場合には true を、しない場合には false を指定します。
     *
     * @pre
     *              - IsLoaded() == true
     */
    void SetLoop(bool isLoop) NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer がループ再生をするか取得します。
     *
     * @return      ループ再生をする場合には true を、しない場合には false を返します。
     *
     * @pre
     *              - IsLoaded() == true
     */
    bool IsLoop() const NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer がループ再生をする場合のループ開始位置とループ終了位置を同時に指定します。
     *
     * @param[in]   loopStartPosition   ループ開始位置を指定します。単位はサンプルです。
     * @param[in]   loopEndPosition     ループ開始位置を指定します。単位はサンプルです。
     *
     * @pre
     *              - IsLoaded() == true
     *              - 0 <= loopStartPosition < loopEndPosition <= ロードされている振動ファイルのサンプル長
     */
    void SetLoopRange(int loopStartPosition, int loopEndPosition) NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer がループ再生をする場合のループ開始位置を指定します。
     *
     * @param[in]   position    ループ開始位置を指定します。単位はサンプルです。
     *
     * @pre
     *              - IsLoaded() == true
     *              - 0 <= position < ロードされている振動ファイルのサンプル長
     */
    void SetLoopStartPosition(int position) NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer がループ再生をする場合のループ開始位置を取得します。
     *
     * @return      ループ開始位置を返します。単位はサンプルです。
     *
     * @pre
     *              - IsLoaded() == true
     */
    int GetLoopStartPosition() const NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer がループ再生をする場合のループ終了位置を指定します。
     *
     * @param[in]   position    ループ終了位置を指定します。単位はサンプルです。
     *
     * @pre
     *              - IsLoaded() == true
     *              - 0 < position <= ロードされている振動ファイルのサンプル長
     */
    void SetLoopEndPosition(int position) NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer がループ再生をする場合のループ終了位置を取得します。
     *
     * @return      ループ終了位置を返します。単位はサンプルです。
     *
     * @pre
     *              - IsLoaded() == true
     */
    int GetLoopEndPosition() const NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer がループ再生をする場合の
     *              ループ終了位置の振動完了からループ開始位置の振動開始までの時間間隔を指定します。
     *
     * @param[in]   interval    時間間隔を指定します。単位はサンプルです。
     *
     * @pre
     *              - IsLoaded() == true
     *              - interval >= 0
     */
    void SetLoopInterval(int interval) NN_NOEXCEPT;

    /**
     * @brief       VibrationPlayer がループ再生をする場合の
     *              ループ終了位置の振動完了からループ開始位置の振動開始までの時間間隔を取得します。
     *
     * @return      時間間隔を返します。単位はサンプルです。
     *
     * @pre
     *              - IsLoaded() == true
     */
    int GetLoopInterval() const NN_NOEXCEPT;

    /**
     * @brief       自インスタンスの VibrationFileParserContext オブジェクトへのポインタを返します。
     *
     * @return      VibrationFileParserContext オブジェクトへのポインタ
     */
    VibrationFileParserContext* GetBase() NN_NOEXCEPT;

protected:
    virtual void OnNextSampleRequired(VibrationValue* pValue, VibrationNodeConnection::List* pInputConnections) NN_NOEXCEPT NN_OVERRIDE;

private:
    bool m_IsLoaded;
    bool m_IsPlaying;
    bool m_IsLoop;
    int m_CurrentPosition;
    int m_NextPosition;
    int m_LoopStartPosition;
    int m_LoopEndPosition;
    int m_LoopInterval;
    VibrationFileInfo m_FileInfo;
    VibrationFileParserContext m_ParserContext;
};

}} // namespace nn::hid
