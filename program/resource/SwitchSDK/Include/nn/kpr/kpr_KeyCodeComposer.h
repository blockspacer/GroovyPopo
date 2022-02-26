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
 * @brief   キー入力の文字コード組み立てに関する API の宣言
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/util/util_TypedStorage.h>

namespace nn { namespace kpr {

const int KeyCodeCompositionCountMax = 5;   //!< KeyCodeComposer が保持する文字コード列の最大長

/**
 * @brief   文字コードの組み立てモードを表す列挙型です。
 */
enum KeyCodeComposerMode
{
    KeyCodeComposerMode_Default,        //!< 既定
    KeyCodeComposerMode_DeadKey,        //!< デッドキー
    KeyCodeComposerMode_RomajiHiragana, //!< ローマ字（ひらがな）
    KeyCodeComposerMode_RomajiKatakana, //!< ローマ字（カタカナ）
};

namespace detail {

struct KeyCodeComposerStorage;

} // namespace detail

/**
 * @brief   文字コードの組み立てを扱うクラスです。
 *
 * @details 文字コードは UTF-16 符号で表現されます。
 */
class KeyCodeComposer
{
    NN_DISALLOW_COPY(KeyCodeComposer);
    NN_DISALLOW_MOVE(KeyCodeComposer);

private:
    ::nn::util::TypedStorage<detail::KeyCodeComposerStorage, 32, 8> m_Storage;

public:
    /**
     * @brief   コンストラクタです。
     *
     * @post
     *      - GetMode() == nn::kpr::KeyCodeComposerMode_Default
     *      - GetCompositionCount() == 0
     *      - GetDequeueableCount() == 0
     */
    KeyCodeComposer() NN_NOEXCEPT;

    /**
     * @brief   文字コードの組み立てモードを返します。
     *
     * @return  文字コードの組み立てモードです。
     */
    KeyCodeComposerMode GetMode() const NN_NOEXCEPT;

    /**
     * @brief   文字コードの組み立てモードを設定します。
     *
     * @param[in]   mode            文字コードの組み立てモード
     *
     * @details 異なるモードに切り替えると組み立て中文字コード列は破棄されます。
     */
    void SetMode(KeyCodeComposerMode mode) NN_NOEXCEPT;

    /**
     * @brief   組み立て中文字コード列の長さを返します。
     *
     * @return  組み立て中文字コード列の長さです。
     *
     * @post
     *      - 戻り値 n について、0 <= n && n <= nn::kpr::KeyCodeCompositionCountMax
     */
    int GetCompositionCount() const NN_NOEXCEPT;

    /**
     * @brief   組み立て中文字コード列を取得します。
     *
     * @param[out]  outBuffer       文字コード列の読み出し先
     * @param[in]   count           outBuffer の長さ
     *
     * @return  取得したキーコード列の長さです。
     *
     * @pre
     *      - outBuffer != nullptr
     *      - 0 <= count
     *
     * @post
     *      - 戻り値 n について、0 <= n && n <= count
     *
     * @details 組み立て中文字コード列を先頭から最大 count 個まで取得します。
     *          取得される文字コードの数が outBuffer の長さより小さい場合、
     *          余った領域に対しては何も変更は加えません。
     */
    int GetComposition(uint16_t outBuffer[], int count) const NN_NOEXCEPT;

    /**
     * @brief   組み立て中文字コード列の末尾に文字コードを追加します。
     *
     * @param[in]   buffer          文字コード列
     * @param[in]   count           buffer の長さ
     *
     * @return  追加された文字コードの数です。
     *
     * @pre
     *      - buffer != nullptr
     *      - 0 <= count
     *
     * @post
     *      - 戻り値 n について、0 <= n && n <= count
     *
     * @details 文字コードの追加は先頭から 1 つずつ処理され、
     *          その度に組み立て中文字コード列の状態が更新されます。
     *          文字コード列に確定部分が発生すると、
     *          その時点で文字コードの追加は打ち切られます。
     *          既に確定部分が存在する場合は文字コードの追加は行われません。
     *          GetDequeueableCount() が 0 になるまで Dequeue() を呼び出して
     *          確定部分を取り出すことで再びキーコードの追加が可能になります。
     */
    int Enqueue(const uint16_t buffer[], int count) NN_NOEXCEPT;

    /**
     * @brief   組み立て中文字コード列の確定部分の長さを返します。
     *
     * @return  組み立て中文字コード列の確定部分の長さです。
     *
     * @post
     *      - 戻り値 n について、0 <= n && n <= GetCompositionCount()
     */
    int GetDequeueableCount() const NN_NOEXCEPT;

    /**
     * @brief   組み立て中文字コード列から確定部分を取り出します。
     *
     * @param[out]  outBuffer       文字コード列の読み出し先
     * @param[in]   count           outBuffer の長さ
     *
     * @return  取り出した文字コード列の長さです。
     *
     * @pre
     *      - outBuffer != nullptr
     *      - 0 <= count
     *
     * @post
     *      - 戻り値 n について、0 <= n && n <= count
     *
     * @details 組み立て中文字コード列の確定部分を先頭から最大 count 個まで
     *          取り出します。
     *          取り出される文字コードの数が outBuffer の長さより小さい場合、
     *          余った領域に対しては何も変更は加えません。
     */
    int Dequeue(uint16_t outBuffer[], int count) NN_NOEXCEPT;

    /**
     * @brief   組み立て中文字コード列から未確定部分を取り出します。
     *
     * @param[out]  outBuffer       文字コード列の読み出し先
     * @param[in]   count           outBuffer の長さ
     *
     * @return  取り出した文字コード列の長さです。
     *
     * @pre
     *      - outBuffer != nullptr
     *      - 0 <= count
     *
     * @post
     *      - 戻り値 n について、0 <= n && n <= count
     *
     * @details 組み立て中文字コード列の未確定部分を末尾から最大 count 個まで
     *          取り出します。
     *          取り出される文字コードの数が outBuffer の長さより小さい場合、
     *          余った領域に対しては何も変更は加えません。
     *          文字コードは古いものから順に格納されます。
     */
    int Remove(uint16_t outBuffer[], int count) NN_NOEXCEPT;

    /**
     * @brief   組み立て中文字コード列の未確定部分全てを明示的に確定させます。
     *
     * @post
     *      - GetCompositionCount() == GetDequeueableCount()
     */
    void Flush() NN_NOEXCEPT;

    /**
     * @brief   組み立て中文字コード列を破棄します。
     *
     * @post
     *      - GetCompositionCount() == 0
     *      - GetDequeueableCount() == 0
     */
    void Clear() NN_NOEXCEPT;
};

}} // namespace nn::kpr
