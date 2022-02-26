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

#include <cstdint>
#include <nn/nn_Macro.h>
#include <nn/atk/detail/atk_Log.h>
#include <nn/atk/fnd/basis/atkfnd_Config.h>

namespace nn {
namespace atk {
namespace detail {
namespace fnd {

//---------------------------------------------------------------------------
//! @internal
//! @brief  sndfnd の処理結果です。
//---------------------------------------------------------------------------
enum FndResultType
{
    //---------------------------------------------------------------------------
    // ビット構成
    //---------------------------------------------------------------------------

    FndResultType_ErrorFlagMask     = 0x80000000,
    FndResultType_CategoryMask      = 0x7f000000,
    FndResultType_CodeMask          = 0x00ffffff,
    FndResultType_CategoryBitOffset = 24,

    //---------------------------------------------------------------------------
    // カテゴリ
    //---------------------------------------------------------------------------

    FndResultType_ErrorFlag      = FndResultType_ErrorFlagMask,
    FndResultType_CategorySystem = 0x00 << FndResultType_CategoryBitOffset,
    FndResultType_CategoryIo     = 0x01 << FndResultType_CategoryBitOffset,

    //---------------------------------------------------------------------------
    // 成功コード（システム）
    //---------------------------------------------------------------------------

    FndResultType_True  = 0x000000 + FndResultType_CategorySystem,   //!< 成功しました。
    FndResultType_False = 0x000001 + FndResultType_CategorySystem,   //!< 処理を実行しませんでした。

    //---------------------------------------------------------------------------
    // 失敗コード（システム）
    //---------------------------------------------------------------------------

    FndResultType_Failed          = 0x000000 + FndResultType_CategorySystem + FndResultType_ErrorFlag,    //!< 失敗しました。
    FndResultType_NotInitialized  = 0x000001 + FndResultType_CategorySystem + FndResultType_ErrorFlag,    //!< 初期化されていません。
    FndResultType_NotSupported    = 0x000002 + FndResultType_CategorySystem + FndResultType_ErrorFlag,    //!< サポートされていません。
    FndResultType_NotOpened       = 0x000003 + FndResultType_CategorySystem + FndResultType_ErrorFlag,    //!< 開かれていません。
    FndResultType_OutOfMemory     = 0x000004 + FndResultType_CategorySystem + FndResultType_ErrorFlag,    //!< メモリ不足です。
    FndResultType_InvalidArgument = 0x000005 + FndResultType_CategorySystem + FndResultType_ErrorFlag,    //!< 引数が不正です。
    FndResultType_InvalidStatus   = 0x000006 + FndResultType_CategorySystem + FndResultType_ErrorFlag,    //!< 状態が不正です。

    //---------------------------------------------------------------------------
    // 失敗コード（入出力）
    //---------------------------------------------------------------------------

    FndResultType_IoError         = 0x000000 + FndResultType_CategoryIo + FndResultType_ErrorFlag,    //!< 入出力でエラーが発生しました。
    FndResultType_IoFileNotFound  = 0x000001 + FndResultType_CategoryIo + FndResultType_ErrorFlag,    //!< ファイルが見つかりません。
    FndResultType_IoInvalidAccess = 0x000002 + FndResultType_CategoryIo + FndResultType_ErrorFlag,    //!< アクセス権がない等、無効なアクセスが発生しました。
    FndResultType_IoTargetLocked  = 0x000003 + FndResultType_CategoryIo + FndResultType_ErrorFlag     //!< ファイルがロックされていて開けません。
};

//---------------------------------------------------------------------------
//! @internal
//! @brief  sndfnd の処理結果を制御します。
//---------------------------------------------------------------------------
class FndResult
{
public: // コンストラクタ
    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //---------------------------------------------------------------------------
    FndResult() NN_NOEXCEPT : value(FndResultType_True) { }

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //! @param[in] value 結果コードです。
    //---------------------------------------------------------------------------
    explicit FndResult(uint32_t value) NN_NOEXCEPT : value(value) { }

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //! @param[in] value 処理結果です。
    //---------------------------------------------------------------------------
    explicit FndResult(FndResultType value) NN_NOEXCEPT : value(value) { }

public: // メソッド
    //---------------------------------------------------------------------------
    //! @brief  結果が成功かどうかを調べます。
    //! @return 成功の場合は true、失敗の場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsSucceeded() const NN_NOEXCEPT { return !IsFailed(); }

    //---------------------------------------------------------------------------
    //! @brief  結果が成功かつ TRUE かどうかを調べます。
    //! @return 成功かつ TRUE の場合は true、それ以外の場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsTrue() const NN_NOEXCEPT { return value == FndResultType_True; }

    //---------------------------------------------------------------------------
    //! @brief  結果が成功かつ FALSE かどうかを調べます。
    //! @return 成功かつ FALSE の場合は true、それ以外の場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsFalse() const NN_NOEXCEPT { return value == FndResultType_False; }

    //---------------------------------------------------------------------------
    //! @brief  結果が失敗かどうかを調べます。
    //! @return 失敗の場合は true、成功の場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsFailed() const NN_NOEXCEPT { return (value & FndResultType_ErrorFlag) != 0; }

    //---------------------------------------------------------------------------
    //! @brief  Result を文字列に変換します。
    //!
    //!         この関数は DEBUG または DEVELOP ビルド時にのみ正しい値を返します。
    //!
    //! @return DEBUG または DEVELOP ビルド時には、変換後の文字列を返します。
    //!
    //!         RELEASE ビルド時には必ず空文字を返します。
    //---------------------------------------------------------------------------
    const char* ToString() const NN_NOEXCEPT
    {
#if !defined(NN_SDK_BUILD_RELEASE)
        switch(value)
        {
        //---------------------------------------------------------------------------
        // 成功コード（システム）
        case FndResultType_True:
            return "FndResultType_True";

        case FndResultType_False :
            return "FndResultType_False";

        //---------------------------------------------------------------------------
        // 失敗コード（システム）
        case FndResultType_Failed:
            return "FndResultType_Failed";

        case FndResultType_NotInitialized:
            return "FndResultType_NotInitialized";

        case FndResultType_NotSupported:
            return "FndResultType_NotSupported";

        case FndResultType_NotOpened:
            return "FndResultType_NotOpened";

        case FndResultType_OutOfMemory:
            return "FndResultType_OutOfmemory";

        case FndResultType_InvalidArgument:
            return "FndResultType_InvalidArgument";

        case FndResultType_InvalidStatus:
            return "FndResultType_InvalidStatus";
        default:
            break;
        }
#endif

        return "";
    }

    void PrintResult() NN_NOEXCEPT
    {
        if (IsFailed())
        {
            NN_DETAIL_ATK_INFO("Result: failure. (%s)\n", ToString());
        }
        else
        {
            NN_DETAIL_ATK_INFO("Result: success.\n");
        }
    }

public: // オペレータ
    //--------------------------------------------------------------------------
    //! @brief   uint32_t 型へのキャスト演算子です。
    //!
    //! @return  uint32_t 値を返します。
    //---------------------------------------------------------------------------
    NN_IMPLICIT operator uint32_t() const NN_NOEXCEPT
    {
        return value;
    }

    //--------------------------------------------------------------------------
    //! @brief   FndResultType 型へのキャスト演算子です。
    //!
    //! @return  FndResultType で表現した値を返します。
    //---------------------------------------------------------------------------
    NN_IMPLICIT operator FndResultType() const NN_NOEXCEPT
    {
        return static_cast<FndResultType>(value);
    }

private: // メンバ変数
    uint32_t value;
};


} // namespace nn::atk::detail::fnd
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
