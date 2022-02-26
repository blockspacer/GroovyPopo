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

#include <nn/atk/atk_Config.h>
#ifdef NN_ATK_CONFIG_ENABLE_DEV

namespace nn {
namespace atk {
namespace viewer {

//---------------------------------------------------------------------------
//! @brief  【β版】sndedit の処理結果です。
//---------------------------------------------------------------------------
enum ResultType
{
    //! @internal
    ResultType_ErrorFlagMask     = 0x80000000,
    //! @internal
    ResultType_CategoryMask      = 0x7f000000,
    //! @internal
    ResultType_CodeMask          = 0x00ffffff,
    //! @internal
    ResultType_CategoryBitOffset = 24,

    //! @internal
    ResultType_ErrorFlag          = ResultType_ErrorFlagMask,
    //! @internal
    ResultType_CategorySystem     = 0x00 << ResultType_CategoryBitOffset,
    //! @internal
    ResultType_CategoryConnection = 0x01 << ResultType_CategoryBitOffset,

    //---------------------------------------------------------------------------
    // 成功コード（システム）
    //---------------------------------------------------------------------------

    ResultType_True  = 0x000000 + ResultType_CategorySystem,   //!< 成功しました。
    ResultType_False = 0x000001 + ResultType_CategorySystem,   //!< 処理を実行しませんでした。

    //---------------------------------------------------------------------------
    // 失敗コード（システム）
    //---------------------------------------------------------------------------

    ResultType_Failed          = 0x000000 + ResultType_CategorySystem + ResultType_ErrorFlag,    //!< 失敗しました。
    ResultType_NotInitialized  = 0x000001 + ResultType_CategorySystem + ResultType_ErrorFlag,    //!< 初期化されていません。
    ResultType_NotConnected    = 0x000002 + ResultType_CategorySystem + ResultType_ErrorFlag,    //!< 接続されていません。
    ResultType_OutOfMemory     = 0x000003 + ResultType_CategorySystem + ResultType_ErrorFlag,    //!< メモリ不足です。
    ResultType_CacheOverFlow   = 0x000004 + ResultType_CategorySystem + ResultType_ErrorFlag,    //!< キャッシュ数が上限を超えました。
    ResultType_NameNotFound    = 0x000005 + ResultType_CategorySystem + ResultType_ErrorFlag,    //!< 名前が見つかりません。
    ResultType_NameTooLong     = 0x000006 + ResultType_CategorySystem + ResultType_ErrorFlag,    //!< 名前が長すぎます。
    ResultType_InvalidDataType = 0x000007 + ResultType_CategorySystem + ResultType_ErrorFlag,    //!< データタイプが正しくありません。
    ResultType_ItemPreparing   = 0x000008 + ResultType_CategorySystem + ResultType_ErrorFlag,    //!< アイテムは準備中です。

    //---------------------------------------------------------------------------
    // 失敗コード（通信）
    //---------------------------------------------------------------------------

    ResultType_Timeout  = 0x000000 + ResultType_CategoryConnection + ResultType_ErrorFlag,   //!< タイムアウトが発生しました。
    ResultType_HioError = 0x000001 + ResultType_CategoryConnection + ResultType_ErrorFlag    //!< HIO 接続に失敗しました。
};

//---------------------------------------------------------------------------
//! @brief  【β版】sndedit の処理結果を制御します。
//---------------------------------------------------------------------------
class Result
{
public:
    //---------------------------------------------------------------------------
    //! @internal
    //! @brief  コンストラクタです。
    //---------------------------------------------------------------------------
    Result() NN_NOEXCEPT : value(ResultType_True) { }

    //---------------------------------------------------------------------------
    //! @internal
    //! @brief  コンストラクタです。
    //! @param value :private
    //---------------------------------------------------------------------------
    explicit Result(uint32_t value) NN_NOEXCEPT : value(value) { }

    //---------------------------------------------------------------------------
    //! @internal
    //! @brief  コンストラクタです。
    //! @param value :private
    //---------------------------------------------------------------------------
    explicit Result(ResultType value) NN_NOEXCEPT : value(value) { }

    //---------------------------------------------------------------------------
    //! @brief  結果が成功かどうかを調べます。
    //! @return 成功の場合は true、失敗の場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsSucceeded() const NN_NOEXCEPT { return !IsFailed(); }

    //---------------------------------------------------------------------------
    //! @brief  結果が成功かつ TRUE かどうかを調べます。
    //! @return 成功かつ TRUE の場合は true、それ以外の場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsTrue() const NN_NOEXCEPT { return value == ResultType_True; }

    //---------------------------------------------------------------------------
    //! @brief  結果が成功かつ FALSE かどうかを調べます。
    //! @return 成功かつ FALSE の場合は true、それ以外の場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsFalse() const NN_NOEXCEPT { return value == ResultType_False; }

    //---------------------------------------------------------------------------
    //! @brief  結果が失敗かどうかを調べます。
    //! @return 失敗の場合は true、成功の場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsFailed() const NN_NOEXCEPT { return (value & ResultType_ErrorFlag) != 0; }

    //---------------------------------------------------------------------------
    //! @brief  結果コードを取得します。
    //! @return 結果コードを返します。
    //---------------------------------------------------------------------------
    ResultType GetValue() const NN_NOEXCEPT { return static_cast<ResultType>(value); }

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
        case ResultType_True:
            return "ResultType_True";

        case ResultType_False :
            return "ResultType_False";

        //---------------------------------------------------------------------------
        // 失敗コード（システム）
        case ResultType_Failed:
            return "ResultType_Failed";

        case ResultType_NotInitialized:
            return "ResultType_NotInitialized";

        case ResultType_NotConnected:
            return "ResultType_NotConnected";

        case ResultType_OutOfMemory:
            return "ResultType_OutOfMemory";

        case ResultType_CacheOverFlow:
            return "ResultType_CacheOverFlow";

        case ResultType_NameNotFound:
            return "ResultType_NameNotFound";

        case ResultType_NameTooLong:
            return "ResultType_NameTooLong";

        case ResultType_InvalidDataType:
            return "ResultType_InvalidDataType";

        case ResultType_ItemPreparing:
            return "ResultType_ItemPreparing";

        //---------------------------------------------------------------------------
        // 失敗コード（通信）
        case ResultType_Timeout:
            return "ResultType_Timeout";

        case ResultType_HioError:
            return "ResultType_HioError";
        default:
            break;
        }
#endif

        return "";
    }

    //--------------------------------------------------------------------------
    //! @internal
    //! @brief   uint32_t 型へのキャスト演算子です。
    //!
    //! @return  uint32_t 値を返します。
    //---------------------------------------------------------------------------
    NN_IMPLICIT operator uint32_t() const NN_NOEXCEPT
    {
        return value;
    }

    //--------------------------------------------------------------------------
    //! @internal
    //! @brief   ResultType 型へのキャスト演算子です。
    //!
    //! @return  ResultType で表現した値を返します。
    //---------------------------------------------------------------------------
    NN_IMPLICIT operator ResultType() const NN_NOEXCEPT
    {
        return static_cast<ResultType>(value);
    }

private:
    uint32_t value;
};


} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
