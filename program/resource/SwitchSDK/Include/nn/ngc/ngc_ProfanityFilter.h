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

#include <nn/ngc/ngc_ProfanityFilterPatternList.h>
#include <nn/ngc/ngc_ProfanityFilterBase.h>
#include <nn/ngc/ngc_Result.h>

namespace nn {
namespace ngc {

/**
 * @brief   NG ワードフィルタリングを行うためのクラスです。
 * @details
 *  このクラスを利用することで、本体にインストールされている NG ワードリストを参照して
 *  ユーザー入力文字列が NG ワードに該当するかそうでないかを調べることが出来ます。
 *  NG ワードフィルタリングをいつのタイミングでどのように実行すべきかは、
 *  最新の NGCライブラリのマニュアルを参照してください。
 *
 *  それぞれの言語ごとにテスト用の NG ワードが登録されています。
 *  テスト用の文字列を使用することで NG ワードライブラリが正しく組み込めたかどうかを確認することができます。
 *  詳しくは nn::ngc::ProfanityFilterPatternList 列挙型の解説をご覧ください。
 *
 *  このクラスはスレッドセーフではありません。複数のスレッドからこのクラスのインスタンスに
 *  アクセスした場合の動作は保証されませんのでご注意ください。
 */
class ProfanityFilter : public ProfanityFilterBase
{
public:
    /**
     * @brief チェックするために必要なメモリ領域サイズです。
     */
    static const size_t  WorkMemorySize = 128 * 1024;

    /**
     * @brief 不正文字列をマスクする時の文字列の置き換え方法です。
     */
    enum MaskMode
    {
        MaskMode_OverWrite             = 0, //!< 不正文字列のすべての文字をアスタリスク記号で上書きします。
        MaskMode_ReplaceByOneCharacter = 1, //!< 不正文字列を1つのアスタリスク記号で置換します。
    };

    /**
     * @brief 文章チェック時の特定文字のスキップ方法です。
     */
    enum SkipMode
    {
        SkipMode_NotSkip    = 0, //!< アットマーク記号のチェックをスキップをしません。
        SkipMode_SkipAtSign = 1, //!< アットマーク記号のチェックをスキップします。
    };

public:

    /**
     * @brief 初期化処理を一切行わないコンストラクタです。
     * @details
     *  このコンストラクタでは初期化処理を行いません。
     *  このクラスを利用する前に初期化処理を行う必要があります。
     */
    ProfanityFilter() NN_NOEXCEPT ;

    /**
     * @brief 初期化処理を行うコンストラクタです。
     *
     * @param [in]   pWorkMemory           ワーキングメモリ領域の先頭へのアドレスを指定します。
     * @param [in]   workMemorySize        ワーキングメモリ領域のサイズを指定します。
     * @param [in]   checkDesiredLanguage  アプリケーションが対応する最も優先度の高い言語に対してチェックを行うかを指定します。
     *
     * @details
     *  指定されたメモリ領域をワーキングエリアとして利用します。
     *  利用されている最中は、このワーキングエリアへの書き込みを行ったり開放したりしないでください。
     *  指定されたメモリ領域はこのクラスの使用を終了しても開放されませんので、
     *  アプリケーション側で管理する必要があります。
     *  メモリは ProfanityFilter::WorkMemorySize バイト以上確保されている必要があります。
     *  checkDesiredLanguage の設定により ProfanityFilter::CheckProfanityWords と ProfanityFilter::MaskProfanityWordsInText のチェック内容が変化します。
     *  詳細は各関数の説明を参照して下さい。
     */
    ProfanityFilter( void* pWorkMemory, size_t workMemorySize, bool checkDesiredLanguage ) NN_NOEXCEPT;

    /**
     * @brief 初期化処理を行うコンストラクタです。
     *
     * @param [in]   pWorkMemory           ワーキングメモリ領域の先頭へのアドレスを指定します。
     * @param [in]   workMemorySize        ワーキングメモリ領域のサイズを指定します。
     *
     * @details
     *  ProfanityFilter::ProfanityFilter の引数 checkDesiredLanguage に true を指定して実行する場合と同じ処理を行います。
     *
     */
    ProfanityFilter( void* pWorkMemory, size_t workMemorySize ) NN_NOEXCEPT;

    /**
     * @brief 終了処理を行います。
     *
     * @details
     *  内部で Finalize が呼ばれます。
     */
    virtual ~ProfanityFilter() NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief NG ワードフィルタリングクラスを初期化します。
     *
     * @param [in]   pWorkMemory           ワーキングメモリ領域の先頭へのアドレスを指定します。
     * @param [in]   workMemorySize        ワーキングメモリ領域のサイズを指定します。
     * @param [in]   checkDesiredLanguage  アプリケーションが対応する最も優先度の高い言語に対してチェックを行うかを指定します。
     *
     * @retresult
     *    @handleresult{nn::ResultSuccess,                  初期化に成功しました。}
     *    @handleresult{nn::ngc::ResultInvalidPointer,      指定したポインタが不正です。}
     *    @handleresult{nn::ngc::ResultInvalidSize,         指定したサイズが不正です。}
     *    @handleresult{nn::ngc::ResultAlreadyInitialized,  既に初期化されています。}
     * @endretresult
     *
     * @details
     *  指定されたメモリ領域をワーキングエリアとして利用します。
     *  利用されている最中は、このワーキングエリアへの書き込みを行ったり開放したりしないでください。
     *  指定されたメモリ領域はこのクラスの使用を終了しても開放されませんので、
     *  アプリケーション側で管理する必要があります。
     *  メモリは ProfanityFilter::WorkMemorySize バイト以上確保されている必要があります。
     *  checkDesiredLanguage の設定により ProfanityFilter::CheckProfanityWords と ProfanityFilter::MaskProfanityWordsInText のチェック内容が変化します。
     *  詳細は各関数の説明を参照して下さい。
     *
     *  この関数内では fs API を呼び出すため、fs のアロケータからメモリを確保します。
     */
    nn::Result Initialize( void* pWorkMemory, size_t workMemorySize, bool checkDesiredLanguage ) NN_NOEXCEPT ;

    /**
     * @brief NG ワードフィルタリングクラスを初期化します。
     *
     * @param [in]   pWorkMemory    ワーキングメモリ領域の先頭へのアドレスを指定します。
     * @param [in]   workMemorySize ワーキングメモリ領域のサイズを指定します。
     *
     * @retresult
     *    @handleresult{nn::ResultSuccess,                  初期化に成功しました。}
     *    @handleresult{nn::ngc::ResultInvalidPointer,      指定したポインタが不正です。}
     *    @handleresult{nn::ngc::ResultInvalidSize,         指定したサイズが不正です。}
     *    @handleresult{nn::ngc::ResultAlreadyInitialized,  既に初期化されています。}
     * @endretresult
     *
     * @details
     *  ProfanityFilter::Initialize の引数 checkDesiredLanguage に true を指定して実行する場合と同じ処理を行います。
     */
    nn::Result Initialize( void* pWorkMemory, size_t workMemorySize ) NN_NOEXCEPT ;

    /**
     * @brief NG ワードフィルタリングクラスの使用を終了します。
     *
     * @details
     *  この関数内では他のインスタンスがなければ fs のアロケータから確保したメモリを解放します。
     */
    void Finalize() NN_NOEXCEPT ;

    /**
     * @brief 現在本体にインストールされている NG ワードパターンファイルのバージョン番号を取得します。
     *
     * @return インストールされているバージョン番号が返ります。バージョンは 1 から始まり、数が大きいほど新しいことを示します。取得に失敗した場合 0 が返ります。
     */
    virtual uint32_t GetContentVersion() NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief 指定された複数の単語がスクリーン上に表示される文字列として問題ある語句かどうかを、
     *        言語設定によって自動的に使用されるパターンリストと指定されたパターンリストについて確認します。
     *
     * @param [out] pCheckResults    問題があったかどうかの判定を格納するためのバッファを指定します。wordCount で指定した個数の配列が必要です。
     * @param [in]  patterns         言語設定によって自動的に使用されるパターンリストに追加でパターンリストを指定します。
     *                               nn::ngc::ProfanityFilterPatternList の各パターンの値を指定してください。
     *                               複数指定する場合は OR を取ったものを指定してください。
     *                               言語設定によって自動的に使用されるパターンリストが指定された場合は、そのパターンの指定は無視されます。
     *                               パターンリストを追加しない場合は 0 を指定します。
     * @param [in]  ppWords          調査対象となる NULL 終端文字列の配列を指定します。
     *                               文字コードは UTF-8 で格納してください。
     * @param [in]  wordCount        調査対象の文字列の数を指定します。最大数は nn::ngc::WordCountMax です。
     *
     * @retresult
     *     @handleresult{nn::ResultSuccess,                  チェックに成功しました。}
     *     @handleresult{nn::ngc::ResultNotInitialized,      初期化されていません。}
     *     @handleresult{nn::ngc::ResultInvalidPointer,      指定されたポインタが不正です。}
     *     @handleresult{nn::ngc::ResultInvalidSize,         指定された文字列の数が不正です。}
     * @endretresult
     *
     * @details
     *  この関数はスクリーンネームなどのような、単語について確認を行います。
     *  2つ以上の単語が含まれる、ユーザーが入力した文章に対する処理には利用することが出来ません。
     *  その場合は、代わりに ProfanityFilter::MaskProfanityWordsInText 関数を利用してください。
     *
     *  ProfanityFilter クラスの初期化時に checkDesiredLanguage を true に設定した場合、
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
     *  メールアドレスの表記に使われる可能性のあるアットマーク記号が含まれている場合、
     *  常に問題のある語句として検出されます。ただし、電話番号などの表示に利用される可能性がある、
     *  数字が多く含まれている文字列については問題のある語句としては検出されません。
     *  このチェックには nn::ngc::CountNumbers 関数を用いてください。
     *
     *  パターンリストは ngc ライブラリ外に保存されているものを動的に参照します。@n
     *  そのため、 SDK のバージョンやそれにより初期化された開発機によってチェック内容が変わる可能性があります。@n
     *  互換性の観点から特定の文字列に対する結果が常に不変であることを想定したコードの記述は行わないようにしてください。
     *
     */
    virtual nn::Result CheckProfanityWords( Bit32* pCheckResults, Bit32 patterns, const char** ppWords, size_t wordCount ) NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief 指定された複数の単語がスクリーン上に表示される文字列として問題ある語句かどうかを、
     *        言語設定によって自動的に使用されるパターンリストと指定されたパターンリストについて確認します。
     *
     * @param [out] pCheckResults    問題があったかどうかの判定を格納するためのバッファを指定します。wordCount で指定した個数の配列が必要です。
     * @param [in]  patterns         言語設定によって自動的に使用されるパターンリストに追加でパターンリストを指定します。
     *                               nn::ngc::ProfanityFilterPatternList の各パターンの値を指定してください。
     *                               複数指定する場合は OR を取ったものを指定してください。
     *                               言語設定によって自動的に使用されるパターンリストが指定された場合は、そのパターンの指定は無視されます。
     *                               パターンリストを追加しない場合は 0 を指定します。
     * @param [in]  ppWords          調査対象となる NULL 終端文字列の配列を指定します。
     *                               文字コードは UTF-16 で格納してください。
     * @param [in]  wordCount        調査対象の文字列の数を指定します。最大数は nn::ngc::WordCountMax です。
     *
     * @retresult
     *     @handleresult{nn::ResultSuccess,                  チェックに成功しました。}
     *     @handleresult{nn::ngc::ResultNotInitialized,      初期化されていません。}
     *     @handleresult{nn::ngc::ResultInvalidPointer,      指定されたポインタが不正です。}
     *     @handleresult{nn::ngc::ResultInvalidSize,         指定された文字列の数が不正です。}
     * @endretresult
     *
     * @details
     *  この関数はスクリーンネームなどのような、単語について確認を行います。
     *  2つ以上の単語が含まれる、ユーザーが入力した文章に対する処理には利用することが出来ません。
     *  その場合は、代わりに ProfanityFilter::MaskProfanityWordsInText 関数を利用してください。
     *
     *  ProfanityFilter クラスの初期化時に checkDesiredLanguage を true に設定した場合、
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
     *  メールアドレスの表記に使われる可能性のあるアットマーク記号が含まれている場合、
     *  常に問題のある語句として検出されます。ただし、電話番号などの表示に利用される可能性がある、
     *  数字が多く含まれている文字列については問題のある語句としては検出されません。
     *  このチェックには nn::ngc::CountNumbers 関数を用いてください。
     *
     *  パターンリストは ngc ライブラリ外に保存されているものを動的に参照します。@n
     *  そのため、 SDK のバージョンやそれにより初期化された開発機によってチェック内容が変わる可能性があります。@n
     *  互換性の観点から特定の文字列に対する結果が常に不変であることを想定したコードの記述は行わないようにしてください。
     *
     */
    virtual nn::Result CheckProfanityWords(Bit32* pCheckResults, Bit32 patterns, const uint16_t** ppWords, size_t wordCount) NN_NOEXCEPT;

    /**
     * @brief 指定された複数の単語がスクリーン上に表示される文字列として問題ある語句かどうかを、
     *        言語設定によって自動的に使用されるパターンリストと指定されたパターンリストについて確認します。
     *
     * @param [out] pCheckResults    問題があったかどうかの判定を格納するためのバッファを指定します。wordCount で指定した個数の配列が必要です。
     * @param [in]  patterns         言語設定によって自動的に使用されるパターンリストに追加でパターンリストを指定します。
     *                               nn::ngc::ProfanityFilterPatternList の各パターンの値を指定してください。
     *                               複数指定する場合は OR を取ったものを指定してください。
     *                               言語設定によって自動的に使用されるパターンリストが指定された場合は、そのパターンの指定は無視されます。
     *                               パターンリストを追加しない場合は 0 を指定します。
     * @param [in]  ppWords          調査対象となる NULL 終端文字列の配列を指定します。
     *                               文字コードは UTF-16 で格納してください。
     * @param [in]  wordCount        調査対象の文字列の数を指定します。最大数は nn::ngc::WordCountMax です。
     *
     * @retresult
     *     @handleresult{nn::ResultSuccess,                  チェックに成功しました。}
     *     @handleresult{nn::ngc::ResultNotInitialized,      初期化されていません。}
     *     @handleresult{nn::ngc::ResultInvalidPointer,      指定されたポインタが不正です。}
     *     @handleresult{nn::ngc::ResultInvalidSize,         指定された文字列の数が不正です。}
     * @endretresult
     *
     * @details
     *  この関数はスクリーンネームなどのような、単語について確認を行います。
     *  2つ以上の単語が含まれる、ユーザーが入力した文章に対する処理には利用することが出来ません。
     *  その場合は、代わりに ProfanityFilter::MaskProfanityWordsInText 関数を利用してください。
     *
     *  ProfanityFilter クラスの初期化時に checkDesiredLanguage を true に設定した場合、
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
     *  メールアドレスの表記に使われる可能性のあるアットマーク記号が含まれている場合、
     *  常に問題のある語句として検出されます。ただし、電話番号などの表示に利用される可能性がある、
     *  数字が多く含まれている文字列については問題のある語句としては検出されません。
     *  このチェックには nn::ngc::CountNumbers 関数を用いてください。
     *
     *  パターンリストは ngc ライブラリ外に保存されているものを動的に参照します。@n
     *  そのため、 SDK のバージョンやそれにより初期化された開発機によってチェック内容が変わる可能性があります。@n
     *  互換性の観点から特定の文字列に対する結果が常に不変であることを想定したコードの記述は行わないようにしてください。
     *
     */
    virtual nn::Result CheckProfanityWords(Bit32* pCheckResults, Bit32 patterns, const char16_t** ppWords, size_t wordCount) NN_NOEXCEPT;

    /**
     * @brief 指定された文章が、表示される文字列として問題がある語句かどうかを、
     *        言語設定によって自動的に使用されるパターンリストと指定されたパターンリストについて確認します。
     *
     * @param [out]      pProfanityWordCount     文章中に不正文字列が何回現れたかを取得するためのポインタを指定します。情報が不要であれば nullptr を指定してください。
     * @param [in,out]   pText                   チェック対象の文章へのポインタを指定します。不正文字列が現れた場合、アスタリスク記号によって内容が上書きされます。
     *                                           そのため、アプリケーションから書き込み可能なバッファを指定しなければなりません。
     *                                           検査可能な文字数は NULL 文字を含めて最大 nn::ngc::TextLengthMax 文字までで、
     *                                           それよりも長い文字列を与えられると、超えた分はチェックの対象から外れます。
     *                                           文字コードは UTF-8 で格納してください。
     * @param [in]       patterns                言語設定によって自動的に使用されるパターンリストに追加でパターンリストを指定します。
     *                                           nn::ngc::ProfanityFilterPatternList の各パターンの値を指定してください。
     *                                           複数指定する場合は OR を取ったものを指定してください。
     *                                           言語設定によって自動的に使用されるパターンリストが指定された場合は、そのパターンの指定は無視されます。
     *                                           パターンリストを追加しない場合は 0 を指定します。
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
     *  その場合は、代わりに ProfanityFilter::CheckProfanityWords 関数を利用してください。
     *
     *  この関数を呼び出すと、指定された文字列に不正文字列が含まれているかどうかを判定し、
     *  不正文字列の部分をアスタリスク記号によってマスクします。
     *
     *  この関数は下記の文字を単語境界として扱います。
     *  - キャリッジリターン、ラインフィード、次行、行区切り、段落区切り、タブ、垂直タブ、用紙送り
     *    半角スペース｡､.,[!"@#@$@%@&'()*+/:;@<=@>?@@@\^_`{|}~-]
     *    全角スペース。、．，［！“”＃＄％＆‘’（）＊＋／：；＜＝＞？＠￥＾＿｀｛｜｝～－］
     *
     *  不正文字列内に単語境界が含まれている場合でも不正文字列として判定されます。
     *
     *  ProfanityFilter クラスの初期化時に checkDesiredLanguage を true に設定した場合、
     *  本体言語設定によって自動的に使用されるパターンリストの他に、nn::oe::GetDesiredLanguage で返るアプリケーションが対応する最も優先度の高い言語のパターンリストも使用されます。
     *  nn::oe::GetDesiredLanguage を使用しないアプリケーションの場合には、独自にアプリケーションの言語に対応するパターンリストを追加することを推奨します。
     *
     *  この処理は時間がかかる場合があります(ブロックします)。処理時間はシステムのバージョンや、
     *  与えられた文字列の長さなどにより変化する可能性があります。
     *  メインスレッドで実行するとフレームレートを低下させるおそれがあるため、別スレッドでの実行を推奨します。
     *
     *  パターンリストは ngc ライブラリ外に保存されているものを動的に参照します。@n
     *  そのため、 SDK のバージョンやそれにより初期化された開発機によってチェック内容が変わる可能性があります。@n
     *  互換性の観点から特定の文字列に対する結果が常に不変であることを想定したコードの記述は行わないようにしてください。
     *
     */
    virtual nn::Result MaskProfanityWordsInText( int* pProfanityWordCount, char* pText, Bit32 patterns ) NN_NOEXCEPT NN_OVERRIDE;

    /**
     * @brief 指定された文章が、表示される文字列として問題がある語句かどうかを、
     *        言語設定によって自動的に使用されるパターンリストと指定されたパターンリストについて確認します。
     *
     * @param [out]      pProfanityWordCount     文章中に不正文字列が何回現れたかを取得するためのポインタを指定します。情報が不要であれば nullptr を指定してください。
     * @param [in,out]   pText                   チェック対象の文章へのポインタを指定します。不正文字列が現れた場合、アスタリスク記号によって内容が上書きされます。
     *                                           そのため、アプリケーションから書き込み可能なバッファを指定しなければなりません。
     *                                           検査可能な文字数は NULL 文字を含めて最大 nn::ngc::TextLengthMax 文字までで、
     *                                           それよりも長い文字列を与えられると、超えた分はチェックの対象から外れます。
     *                                           文字コードは UTF-16 で格納してください。
     * @param [in]       patterns                言語設定によって自動的に使用されるパターンリストに追加でパターンリストを指定します。
     *                                           nn::ngc::ProfanityFilterPatternList の各パターンの値を指定してください。
     *                                           複数指定する場合は OR を取ったものを指定してください。
     *                                           言語設定によって自動的に使用されるパターンリストが指定された場合は、そのパターンの指定は無視されます。
     *                                           パターンリストを追加しない場合は 0 を指定します。
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
     *  その場合は、代わりに ProfanityFilter::CheckProfanityWords 関数を利用してください。
     *
     *  この関数を呼び出すと、指定された文字列に不正文字列が含まれているかどうかを判定し、
     *  不正文字列の部分をアスタリスク記号によってマスクします。
     *
     *  この関数は下記の文字を単語境界として扱います。
     *  - キャリッジリターン、ラインフィード、次行、行区切り、段落区切り、タブ、垂直タブ、用紙送り
     *    半角スペース｡､.,[!"@#@$@%@&'()*+/:;@<=@>?@@@\^_`{|}~-]
     *    全角スペース。、．，［！“”＃＄％＆‘’（）＊＋／：；＜＝＞？＠￥＾＿｀｛｜｝～－］
     *
     *  不正文字列内に単語境界が含まれている場合でも不正文字列として判定されます。
     *
     *  ProfanityFilter クラスの初期化時に checkDesiredLanguage を true に設定した場合、
     *  本体言語設定によって自動的に使用されるパターンリストの他に、nn::oe::GetDesiredLanguage で返るアプリケーションが対応する最も優先度の高い言語のパターンリストも使用されます。
     *  nn::oe::GetDesiredLanguage を使用しないアプリケーションの場合には、独自にアプリケーションの言語に対応するパターンリストを追加することを推奨します。
     *
     *  この処理は時間がかかる場合があります(ブロックします)。処理時間はシステムのバージョンや、
     *  与えられた文字列の長さなどにより変化する可能性があります。
     *  メインスレッドで実行するとフレームレートを低下させるおそれがあるため、別スレッドでの実行を推奨します。
     *
     *  パターンリストは ngc ライブラリ外に保存されているものを動的に参照します。@n
     *  そのため、 SDK のバージョンやそれにより初期化された開発機によってチェック内容が変わる可能性があります。@n
     *  互換性の観点から特定の文字列に対する結果が常に不変であることを想定したコードの記述は行わないようにしてください。
     *
     */
    virtual nn::Result MaskProfanityWordsInText(int* pProfanityWordCount, uint16_t* pText, Bit32 patterns) NN_NOEXCEPT;

    /**
     * @brief 指定された文章が、表示される文字列として問題がある語句かどうかを、
     *        言語設定によって自動的に使用されるパターンリストと指定されたパターンリストについて確認します。
     *
     * @param [out]      pProfanityWordCount     文章中に不正文字列が何回現れたかを取得するためのポインタを指定します。情報が不要であれば nullptr を指定してください。
     * @param [in,out]   pText                   チェック対象の文章へのポインタを指定します。不正文字列が現れた場合、アスタリスク記号によって内容が上書きされます。
     *                                           そのため、アプリケーションから書き込み可能なバッファを指定しなければなりません。
     *                                           検査可能な文字数は NULL 文字を含めて最大 nn::ngc::TextLengthMax 文字までで、
     *                                           それよりも長い文字列を与えられると、超えた分はチェックの対象から外れます。
     *                                           文字コードは UTF-16 で格納してください。
     * @param [in]       patterns                言語設定によって自動的に使用されるパターンリストに追加でパターンリストを指定します。
     *                                           nn::ngc::ProfanityFilterPatternList の各パターンの値を指定してください。
     *                                           複数指定する場合は OR を取ったものを指定してください。
     *                                           言語設定によって自動的に使用されるパターンリストが指定された場合は、そのパターンの指定は無視されます。
     *                                           パターンリストを追加しない場合は 0 を指定します。
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
     *  その場合は、代わりに ProfanityFilter::CheckProfanityWords 関数を利用してください。
     *
     *  この関数を呼び出すと、指定された文字列に不正文字列が含まれているかどうかを判定し、
     *  不正文字列の部分をアスタリスク記号によってマスクします。
     *
     *  この関数は下記の文字を単語境界として扱います。
     *  - キャリッジリターン、ラインフィード、次行、行区切り、段落区切り、タブ、垂直タブ、用紙送り
     *    半角スペース｡､.,[!"@#@$@%@&'()*+/:;@<=@>?@@@\^_`{|}~-]
     *    全角スペース。、．，［！“”＃＄％＆‘’（）＊＋／：；＜＝＞？＠￥＾＿｀｛｜｝～－］
     *
     *  不正文字列内に単語境界が含まれている場合でも不正文字列として判定されます。
     *
     *  ProfanityFilter クラスの初期化時に checkDesiredLanguage を true に設定した場合、
     *  本体言語設定によって自動的に使用されるパターンリストの他に、nn::oe::GetDesiredLanguage で返るアプリケーションが対応する最も優先度の高い言語のパターンリストも使用されます。
     *  nn::oe::GetDesiredLanguage を使用しないアプリケーションの場合には、独自にアプリケーションの言語に対応するパターンリストを追加することを推奨します。
     *
     *  この処理は時間がかかる場合があります(ブロックします)。処理時間はシステムのバージョンや、
     *  与えられた文字列の長さなどにより変化する可能性があります。
     *  メインスレッドで実行するとフレームレートを低下させるおそれがあるため、別スレッドでの実行を推奨します。
     *
     *  パターンリストは ngc ライブラリ外に保存されているものを動的に参照します。@n
     *  そのため、 SDK のバージョンやそれにより初期化された開発機によってチェック内容が変わる可能性があります。@n
     *  互換性の観点から特定の文字列に対する結果が常に不変であることを想定したコードの記述は行わないようにしてください。
     *
     */
    virtual nn::Result MaskProfanityWordsInText(int* pProfanityWordCount, char16_t* pText, Bit32 patterns) NN_NOEXCEPT;

    /**
     * @brief 文章をチェックし、問題のある語句を発見した時の挙動を設定します。
     * @param [in]   mode       問題ある語句が見つかった時、すべての文字をアスタリスク記号で上書きするときに MaskMode_OverWrite を指定します(デフォルト状態)。
     *                          問題ある語句が見つかった時、その語句を 1 文字のアスタリスク記号で置換するとき MaskMode_ReplaceByOneCharacter を指定します。
     * @details
     *  問題のある語句の文字すべてをアスタリスク記号で上書きするモード(デフォルト)では、
     *  上書き処理を行った結果、画面上に表示される枠内をはみ出してしまうことがあります。
     *  例えばプロポーショナルフォントを使っているとき、 'i' と '*' では幅が異なるため、
     *  文字列が上書きされることによって画面外に出てしまう危険性があります。
     *
     *  この関数に false を指定することで、問題のある語句が見つかったときに
     *  問題のある語句の文字数によらず 1 文字のアスタリスク記号に置換しますので、
     *  ユーザーインターフェイスを崩してしまう心配がなくなります。
     *
     *  この設定は、 ProfanityFilter::MaskProfanityWordsInText 関数によって文章のチェックを行った際にのみ有効です。
     *  ProfanityFilter::CheckProfanityWords 関数によって単語のチェックを行ったときには挙動に影響を与えません。
     *
     *  指定された設定は ProfanityFilter::Initialize 関数が呼び出されるとデフォルト状態 (MaskMode_OverWrite) にリセットされます。
     *
     */
    void SetMaskMode( MaskMode mode ) NN_NOEXCEPT;

    /**
     * @brief アットマーク記号のチェックをスキップするかどうかを設定します。
     * @param [in]       skipMode  アットマーク記号のチェックをスキップするときに SkipMode_SkipAtSign を指定します
     *                             アットマーク記号のチェックをスキップしないときに SkipMode_NotSkip を指定します(デフォルト状態)。
     * @details
     *  指定された設定は ProfanityFilter::Initialize 関数が呼び出されるとデフォルト状態 (SkipMode_NotSkip) にリセットされます。
     */
    void SkipAtSignCheck( SkipMode skipMode ) NN_NOEXCEPT ;

private:
    void CleanUpVariables() NN_NOEXCEPT ;
    nn::Result MountSharedContents() NN_NOEXCEPT ;

    nn::Result CheckArgumentsWord( const Bit32* pCheckResults, const char16_t** ppWords, size_t wordCount ) const NN_NOEXCEPT;
    nn::Result CheckProfanityWordsImpl( Bit32* pCheckResults, const char16_t** ppWords, size_t wordCount, bool isConvert ) NN_NOEXCEPT;

    nn::Result CheckArgumentsText( const char16_t* pText ) const NN_NOEXCEPT;
    nn::Result MaskProfanityWordsInTextCommon(int* pProfanityWordCount, char* pText, Bit32 nPatterns) NN_NOEXCEPT;
    nn::Result MaskProfanityWordsInTextImpl(int* pProfanityWordCount,
                                            char* pTextUtf8,
                                            char* pConvertedTextUtf8,
                                            int8_t* pMapping,
                                            int pattern,
                                            size_t* pTextUtf8Length,
                                            size_t* pConvertedTextUtf8Length) NN_NOEXCEPT;

private:

    //! アプリケーションが対応する最も優先度の高い言語に対してもチェックを行うかを示します。(デフォルト:true)
    bool m_CheckDesiredLanguage;

    //! Initialize処理が行われた状態であるかどうかを示します。
    bool m_Initialized;

    //! アットマーク記号のスキップを行うかどうかを示します。(デフォルト:SkipMode_NotSkip)
    SkipMode m_SkipAtSignCheck;

    //! 現在チェック中の情報：チェック対象のパターンリストです。
    ProfanityFilterPatternList m_PatternList;

    //! ワーキングメモリ領域先頭へのポインタです。
    uintptr_t m_WorkingHead;

    //! システムデータキャッシュの確保用の領域です。
    uintptr_t m_MountSystemDataCacheHead;

    //! 長文チェック時に、マスクした文字列を上書きするモードかどうかを示します。(デフォルト:MaskMode_OverWrite)
    MaskMode m_MaskMode;


};

}   // ngc
}   // nn

