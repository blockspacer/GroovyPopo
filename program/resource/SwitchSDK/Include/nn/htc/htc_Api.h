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
*   @file
*   @brief  ホスト・ターゲット間通信に関する API
*/

#pragma once

#include<nn/nn_Common.h>
#include<nn/nn_Result.h>
#include<nn/os/os_SystemEvent.h>

namespace nn {
    class TimeSpan;
}

//--------------------------------------------------------------------------
//  C++ 向けの宣言
//--------------------------------------------------------------------------
namespace nn { namespace htc {

/**
*   @brief      HTC ライブラリを初期化します。
*
*   @details    HTC ライブラリを初期化します。@n
*               本関数はライブラリが初期化済の状態でも呼ぶことができますが、その場合は初期化処理は行いません。
*
*   @post       ライブラリは初期化済の状態になります。
*/
void Initialize() NN_NOEXCEPT;

/**
*   @brief      HTC ライブラリを終了します。
*
*   @details    HTC ライブラリを終了します。@n
*               @ref nn::htc::Initialize() と同数だけ本関数が呼ばれたとき、本関数はライブラリの終了処理を実行し、未初期化状態にします。
*
*   @pre        ライブラリは初期化済の状態である必要があります。
*
*   @post       ライブラリの終了処理が行われた場合、ライブラリは未初期化の状態になります。
*/
void Finalize() NN_NOEXCEPT;

/**
*   @brief      指定した環境変数の値を格納するのに必要なバッファサイズをホストから取得します。
*
*   @param[out] pOutValue       取得したバッファサイズ
*   @param[in]  variableName    バッファサイズを取得する環境変数名
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess           成功しました。
*   @retval     ResultConnectionFailure ホストとの通信に失敗しました。
*   @retval     ResultNotFound          該当する環境変数が見つかりませんでした。
*
*   @pre
*               - ライブラリが初期化済の状態である
*               - pOutValue != nullptr
*               - variableName != nullptr
*
*   @details    指定した環境変数の値を格納するのに必要なバッファサイズをホストから取得します。@n
*               環境変数は API 呼び出し時に設定されていたものが取得されます。@n
*               取得する環境変数名はヌル終端である必要があります。@n
*               TargetManager.exe との接続が確立されていない場合は、ResultConnectionFailure が返ります。
*/
Result GetEnvironmentVariableLength(size_t* pOutValue, const char* variableName) NN_NOEXCEPT;

/**
*   @brief      指定した環境変数の値を UTF-8 の文字列としてホストから取得します。
*
*   @param[out] pOutSize        取得した値のバッファサイズ
*   @param[out] outBuffer       取得した値を格納するバッファ
*   @param[in]  bufferSize      取得した値を格納するバッファのサイズ
*   @param[in]  variableName    取得する環境変数名
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess           成功しました。
*   @retval     ResultConnectionFailure ホストとの通信に失敗しました。
*   @retval     ResultNotFound          該当する環境変数が見つかりませんでした。
*   @retval     ResultNotEnoughBuffer   バッファサイズが取得する値のサイズに足りませんでした。
*
*   @pre
*               - ライブラリが初期化済の状態である
*               - pOutSize != nullptr
*               - outBuffer != nullptr
*               - variableName != nullptr
*
*   @details    指定した環境変数の値を UTF-8 の文字列としてホストから取得します。@n
*               環境変数は API 呼び出し時に設定されていたものが取得されます。@n
*               取得する環境変数名はヌル終端である必要があります。@n
*               取得した値はヌル終端された文字列が格納されます。@n
*               TargetManager.exe との接続が確立されていない場合は、ResultConnectionFailure が返ります。
*/
Result GetEnvironmentVariable(size_t* pOutSize, char* outBuffer, size_t bufferSize, const char* variableName) NN_NOEXCEPT;

/**
*   @brief      ターゲット環境変数の値を格納するのに必要なバッファサイズを取得します。
*
*   @param[out] pOutValue       取得したバッファサイズ
*   @param[in]  variableName    ターゲット環境変数名
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess           成功しました。
*   @retval     ResultConnectionFailure ホストとの通信またはターゲット環境変数定義ファイルの読み込みに失敗しました。
*   @retval     ResultNotFound          該当するターゲット環境変数が見つかりませんでした。
*
*   @pre
*               - ライブラリが初期化済の状態である
*               - 設定されているターゲット環境変数定義ファイルのフォーマットが正しい
*               - pOutValue != nullptr
*               - variableName != nullptr
*               - variableName の文字列長が 64 以下
*
*   @details    取得するターゲット環境変数名はヌル終端である必要があります。@n
*               ターゲット環境変数の値が文字列型で定義されている場合は、ヌル終端を含む必要なバッファサイズが返ります。@n
*               この関数はホストとの接続がない実機環境では常に ResultConnectionFailure を返します。
*/
Result GetTargetEnvironmentVariableLength(size_t* pOutValue, const char* variableName) NN_NOEXCEPT;

/**
*   @brief      ターゲット環境変数の値を文字列として取得します。
*
*   @param[out] pOutSize        取得した値のバッファサイズ
*   @param[out] outBuffer       取得した値を格納するバッファ
*   @param[in]  bufferSize      取得した値を格納するバッファのサイズ
*   @param[in]  variableName    取得するターゲット環境変数名
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess           成功しました。
*   @retval     ResultConnectionFailure ホストとの通信またはターゲット環境変数定義ファイルの読み込みに失敗しました。
*   @retval     ResultNotFound          該当するターゲット環境変数が見つかりませんでした。
*
*   @pre
*               - ライブラリが初期化済の状態である
*               - 設定されているターゲット環境変数定義ファイルのフォーマットが正しい
*               - pOutSize != nullptr
*               - outBuffer != nullptr
*               - variableName != nullptr
*               - variableName の文字列長が 64 以下
*               - bufferSize が GetTargetConfigurationValueLength() で取得できる値以上の値
*
*   @details    取得するターゲット環境変数名はヌル終端である必要があります。@n
*               取得された文字列はヌル終端されて pOutValue に格納されます。@n
*               ヌル終端を含む最大 2048 バイト分の文字列が取得可能です。ターゲット環境変数定義ファイルで 2048 バイトを超える文字列が定義されていた場合は、2048 バイト目以降の値が切り捨てられヌル終端された値が取得されます。@n
*               この関数はホストとの接続がない実機環境では常に ResultConnectionFailure を返します。
*/
Result GetTargetEnvironmentVariable(size_t* pOutSize, char* outBuffer, size_t bufferSize, const char* variableName) NN_NOEXCEPT;

/**
*   @brief      ターゲット環境変数定義ファイルが設定されるまでの間処理をブロックします。
*
*   @param[in]  timeOut    タイムアウトする時間
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess           ターゲット環境変数定義ファイルが設定され、ブロックを解消しました。
*   @retval     ResultConnectionFailure タイムアウト時間内にターゲット環境変数定義ファイルが設定されませんでした。
*
*   @pre
*               - ライブラリが初期化済の状態である
*
*   @details    timeOut に 0 または負の時間を設定した場合はターゲット環境変数定義ファイルが設定されるまで永久に処理がブロックされます。@n
*               この関数はホストとの接続がない実機環境では即座に ResultConnectionFailure を返します。
*/
Result WaitUntilTargetEnvironmentVariableAvailable(nn::TimeSpan timeOut) NN_NOEXCEPT;

/**
*   @brief      ホストとの通信が確立されたことを通知するイベントオブジェクトにバインドします。
*
*   @param[out] pOutValue       イベントオブジェクトを指すポインタ
*
*   @pre
*               - ライブラリが初期化済の状態である
*               - pOutValue != nullptr
*               - *pOutValue は未初期化状態
*
*   @post
*               - *pOutValue は初期化状態
*
*   @details    ホストとの通信が確立されたことを通知するイベントオブジェクトにバインドします。@n
*               イベントは読み取り専用で手動クリアに設定されます。@n
*               ホストと接続中は常にシグナル状態で、切断時にクリアされます。@n
*               イベントはシステム側でクリアするため、ユーザ側でクリアしないでください。
*/
void BindHostConnectionEvent(nn::os::SystemEvent* pOutValue) NN_NOEXCEPT;

/**
*   @brief      ホストとの通信が切断されたことを通知するイベントオブジェクトにバインドします。
*
*   @param[out] pOutValue       イベントオブジェクトを指すポインタ
*
*   @pre
*               - ライブラリが初期化済の状態である
*               - pOutValue != nullptr
*               - *pOutValue は未初期化状態
*
*   @post
*               - *pOutValue は初期化状態
*
*   @details    ホストとの通信が切断されたことを通知するイベントオブジェクトにバインドします。@n
*               イベントは読み取り専用で手動クリアに設定されます。@n
*               ホストと切断中は常にシグナル状態で、接続時にクリアされます。@n
*               イベントはシステム側でクリアするため、ユーザ側でクリアしないでください。
*/
void BindHostDisconnectionEvent(nn::os::SystemEvent* pOutValue) NN_NOEXCEPT;

/**
    @brief ホスト側作業ディレクトリパスの最大バイトサイズ
    @details
        GetWorkingDirectoryPath() で取得されるパスの最大バイトサイズです。@n
        終端文字も含めたサイズです。
*/
static const size_t HostPathSizeMax = 768;

/**
*   @brief      ホスト側で指定した作業ディレクトリのパスを格納するのに必要なバッファサイズを取得します。
*
*   @param[out] pOutValue               バッファのサイズ。末尾の終端文字は含みません。
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess           成功しました。
*   @retval     ResultConnectionFailure ホストとの通信に失敗しました。
*
*   @pre
*               - ライブラリが初期化済の状態である
*               - pOutValue != nullptr
*
*   @details    ホスト側で指定した作業ディレクトリのパスを格納するのに必要なバッファサイズをホストから取得します。@n
*               TargetManager.exe との接続が確立されていない場合は、ResultConnectionFailure が返ります。
*/
Result GetWorkingDirectoryPathSize(size_t* pOutValue) NN_NOEXCEPT;

/**
*   @brief      ホスト側で指定した作業ディレクトリのパスを取得します。
*
*   @param[out] outBuffer       取得したパスを格納するバッファ
*   @param[in]  bufferSize      取得したパスを格納するバッファのサイズ
*
*   @return     処理の結果が返ります。
*   @retval     ResultSuccess           成功しました。
*   @retval     ResultConnectionFailure ホストとの通信に失敗しました。
*   @retval     ResultNotEnoughBuffer   バッファサイズが取得する値のサイズに足りませんでした。
*
*   @pre
*               - ライブラリが初期化済の状態である
*               - outBuffer != nullptr
*
*   @details    ホスト側で指定した作業ディレクトリのパスを UTF-8 の文字列として取得します。@n
*               取得した値はヌル終端された文字列が格納されます。@n
*               必要なバッファサイズは、GetWorkingDirectoryPathSize() で取得可能です。@n
*               返りうるパスの最大バイト数は、HostPathSizeMax です。この値は終端文字も含めたものです。@n
*               TargetManager.exe との接続が確立されていない場合は、ResultConnectionFailure が返ります。
*/
Result GetWorkingDirectoryPath(char* outBuffer, size_t bufferSize) NN_NOEXCEPT;

/**
*   @brief  指定されたコマンドをホスト PC で実行します。
*
*   @param[out] pCommandResult  cmd.exe /c "command" の戻り値
*   @param[in] command  実行するコマンド
*
*   @retval     ResultSuccess           成功しました。
*   @retval     ResultNotImplemented    Target Manager がこのコマンドを認識できませんでした。Target Manager をアップデートしてください。
*   @retval     ResultOutOfMemory       処理を実行するためのメモリが割り当てられませんでした。
*   @pre
*               - ライブラリが初期化済の状態である
*
*   @details    command で指定された文字列を、ホスト PC 上の cmd.exe で実行します。
*               ホスト側でのコマンド処理が終了するまで、本関数はブロックします。
*               本関数はスレッドセーフです。@n
*               @n
*               pCommandResult は、command 自体の戻り値ではないことに注意する必要があります。@n
*               例えば、cmd.exe に存在しない command を入力したときの cmd.exe への戻り値は 9009 ですが、cmd.exe の戻り値自体は 1 となるため、この関数の戻り値も 1 となります。
*
*/
Result RunOnHost( int32_t* pCommandResult, const char* command ) NN_NOEXCEPT;
}}

