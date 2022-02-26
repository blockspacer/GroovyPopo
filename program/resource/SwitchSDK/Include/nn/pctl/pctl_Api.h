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
 * @brief ペアレンタルコントロールに関する pctl ライブラリの関数を提供します。
 */

#include <nn/nn_Common.h>

namespace nn { namespace pctl {

/**
 * @name ペアレンタルコントロール制限チェック関数
 * @{
 */

/**
 * @brief ペアレンタルコントロール制限設定に基づいてコミュニケーション機能の利用が可能かどうかを確認します。
 * @param[in] isShowUi 機能が制限されている場合に関数内部でその旨を表示するかどうかを示す bool 値
 * @return 機能が制限されておらず機能が利用可能であれば true、制限されており利用できない場合は false
 *
 * @details
 * 本関数はペアレンタルコントロールの制限における「他の人との自由なコミュニケーション」の
 * 項目設定値を確認し、制限が必要である場合に false を返します。
 * アプリケーションは、アプリケーション内でコミュニケーション機能など
 * 制限が必要な機能を利用しようとする場合に本関数を呼び出し、
 * 本関数が false を返した場合は利用が制限されているとみなして
 * その機能を利用できないようにする必要があります。@n
 * なお isShowUi に true を指定すると、機能が制限されている場合に
 * 本関数は関数内部でその旨を表示する処理を行い、表示が終わったタイミングでの
 * 制限状態に基づいた戻り値を返します。例えば、表示を行っている間に
 * 制限が無い状態に変わり、その状態で表示処理が終わった場合、
 * 戻り値は制限がなく機能が利用可能であることを表す true が返ります。
 * isShowUi に false を指定した場合は表示処理を行いませんので、
 * 必要に応じてアプリケーション側で制限されている旨を表示してください。
 *
 * 本関数は一時的に制限が解除されているかどうかも加味した値を返します。
 * 一時解除状態である場合は true が返ります。
 */
bool CheckFreeCommunicationPermission(bool isShowUi) NN_NOEXCEPT;

/**
 * @}
 */

}}
