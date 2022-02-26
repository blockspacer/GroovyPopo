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

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/ngc/ngc_ProfanityFilterPatternList.h>
#include <nn/ngc/ngc_Result.h>

namespace nn {
namespace ngc {

/**
 * @brief NGワードフィルタの基底クラスです。
 * @details
 *  メモリ消費量と速度のトレードオフで、将来的に異なるバージョンの
 *  NGワードフィルタがリリースされる可能性があり、このクラスはその基底となります。
 */
class ProfanityFilterBase
{
public:
    /**
     * @brief デストラクタです。
     */
    virtual ~ProfanityFilterBase() NN_NOEXCEPT {}

    /**
     * @brief  現在本体にインストールされている NG ワードパターンファイルのバージョン番号を取得します。
     * @return インストールされているバージョン番号が返ります。バージョンは 1 から始まり、数が大きいほど新しいことを示します。取得に失敗した場合 0 が返ります。
     */
    virtual uint32_t GetContentVersion() NN_NOEXCEPT = 0;

    /**
     * @brief 指定された複数の単語がスクリーン上に表示される文字列として問題ある語句かどうかを、指定されたパターンリストについて確認します。
     *
     * @param [out] pCheckResults   問題があったかどうかの判定を格納するためのバッファを指定します。 nWordCount で指定した個数の配列が必要です。
     * @param [in]  patterns        どのパターンリストに対してチェックを行うのかを指定します。
     * @param [in]  ppWords         調査対象となる NULL 終端文字列の配列を指定します。文字コードは UTF-8 で格納してください。
     * @param [in]  wordCount       調査対象の文字列の数を指定します。最大数は nn::ngc::WordCountMax です。
     *
     * @retresult
     *    @handleresult{nn::ResultSuccess,                  チェックに成功しました。}
     *    @handleresult{nn::ngc::ResultNotInitialized,      初期化されていません。}
     *    @handleresult{nn::ngc::ResultInvalidPointer,      指定されたポインタが不正です。}
     *    @handleresult{nn::ngc::ResultInvalidSize,         指定された文字列の数が不正です。}
     * @endretresult
     *
     * @details
     *  この関数はスクリーンネームなどのような、単語について確認を行います。
     *  2つ以上の単語が含まれる、ユーザーが入力した文章に対する処理には利用することが出来ません。
     *  その場合は、代わりに ProfanityFilterBase::MaskProfanityWordsInText 関数を利用してください。
     *
     *  本体言語設定によって自動的に使用されるパターンリストの他に、nn::oe::GetDesiredLanguage で返るアプリケーションが対応する最も優先度の高い言語のパターンリストも使用されます。
     *  nn::oe::GetDesiredLanguage を使用しないアプリケーションの場合には、独自にアプリケーションの言語に対応するパターンリストを追加することを推奨します。
     *
     *  この処理は時間がかかる場合があります(ブロックします)。処理時間はシステムのバージョンにより変化する可能性があります。
     *  メインスレッドで実行するとフレームレートを低下させるおそれがあるため、別スレッドでの実行を推奨します。
     *  この関数は最大 nn::ngc::WordCountMax 個の文字列を同時に確認することができます。
     *  一度に複数の文字列をチェックすることで、1つずつ個別にチェックするよりも短い時間で調査が終了します。
     *
     *  各文字列ごとのチェックした結果は、 pCheckResults 配列に格納されます。
     *  nn::ngc::ProfanityFilterPatternList の各パターンの値と AND を取って調べてください。
     *  patterns で指定したリストについて問題がないと判断された場合は値が 0 になります。
     *
     *  どのパターンリストに対してチェックを行わなければならないという決まりについては、
     *  最新の NGCライブラリのマニュアルを参照してください。
     *
     *  メールアドレスの表記に使われる可能性のあるアットマーク記号が含まれている場合、
     *  常に問題のある語句として検出されます。ただし、電話番号などの表示に利用される可能性がある、
     *  数字が多く含まれている文字列については問題のある語句としては検出されません。
     *  このチェックには nn::ngc::CountNumbers 関数を用いてください。
     */
    virtual nn::Result CheckProfanityWords( Bit32* pCheckResults, Bit32 patterns, const char** ppWords, size_t wordCount ) NN_NOEXCEPT = 0;

    /**
     * @brief 指定された文章が、表示される文字列として問題がある語句かどうかを、指定されたパターンリストについて確認します。
     *
     * @param [out]      pProfanityWordCount     文章中に不正文字列が何回現れたかを取得するためのポインタを指定します。情報が不要であれば nullptr を指定してください。
     * @param [in,out]   pText                   チェック対象の文章へのポインタを指定します。不正文字列が現れた場合、アスタリスク記号によって内容が上書きされます。
     *                                           そのため、アプリケーションから書き込み可能なバッファを指定しなければなりません。
     *                                           検査可能な文字数はNULL文字を含めて最大 nn::ngc::TextLengthMax 文字までで、
     *                                           それよりも長い文字列を与えられると、超えた分はチェックの対象から外れます。
     *                                           文字コードは UTF-8 で格納してください。
     * @param [in]       patterns                NG ワードパターンリストを指定します。
     *
     * @retresult
     *    @handleresult{nn::ResultSuccess,                  チェックに成功しました。}
     *    @handleresult{nn::ngc::ResultNotInitialized,      初期化されていません。}
     *    @handleresult{nn::ngc::ResultInvalidPointer,      指定されたポインタが不正です。}
     *    @handleresult{nn::ngc::ResultInvalidSize,         指定された文字列の数が不正です。}
     * @endretresult
     *
     * @details
     *  この関数は2単語以上で構成されている文章について処理を行います。
     *  スクリーンネームなど、短い単語には利用することが出来ません。
     *  その場合は、代わりに ProfanityFilterBase::CheckProfanityWords 関数を利用してください。
     *
     *  この関数を呼び出すと、指定された文字列に不正文字列が含まれているかどうかを判定し、
     *  不正文字列の部分をアスタリスク記号によってマスクします。
     *
     *  この関数は下記の文字を単語境界として扱います。
     *  - キャリッジリターン、ラインフィード、次行、行区切り、段落区切り、タブ、垂直タブ、用紙送り
     *    半角スペース｡､.,[!"#$%&'()*+/:;<=>?@\^_`{|}~-]
     *    全角スペース。、．，［！“”＃＄％＆‘’（）＊＋／：；＜＝＞？＠￥＾＿｀｛｜｝～－］
     *
     *  不正文字列内に単語境界が含まれている場合でも不正文字列として判定されます。
     *
     *  本体言語設定によって自動的に使用されるパターンリストの他に、nn::oe::GetDesiredLanguage で返るアプリケーションが対応する最も優先度の高い言語のパターンリストも使用されます。
     *  nn::oe::GetDesiredLanguage を使用しないアプリケーションの場合には、独自にアプリケーションの言語に対応するパターンリストを追加することを推奨します。
     *
     *  この処理は時間がかかる場合があります(ブロックします)。処理時間はシステムのバージョンや、
     *  与えられた文字列の長さなどにより変化する可能性があります。
     *  メインスレッドで実行するとフレームレートを低下させるおそれがあるため、別スレッドでの実行を推奨します。
     *
     *  どのパターンリストに対してチェックを行わなければならないという決まりについては、
     *  最新の NGCライブラリのマニュアルを参照してください。
     */
    virtual nn::Result MaskProfanityWordsInText( int* pProfanityWordCount, char* pText, Bit32 patterns ) NN_NOEXCEPT = 0;

protected:
    Bit32 GetPatternBitsFromRegion( bool communicateWithOtherRegions, bool checkDesiredLanguage ) NN_NOEXCEPT;
    static void ConvertUserInputForWord( char16_t* pConvertedWord, int nLength, const char16_t* pWord ) NN_NOEXCEPT;
    static void ConvertUserInputForText(char* pConvertedWord, int8_t* pSourceLengthMapping, size_t length, const char* pWord) NN_NOEXCEPT;
    bool IsIncludesAtSign( const char16_t* pWord, int length ) NN_NOEXCEPT;
};

}   // namespaec ngc
}   // namespace nn

