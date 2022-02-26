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

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

//--------------------------------------------------------------------------------------------
//! @brief      GUIDです.
//--------------------------------------------------------------------------------------------
class Guid
{
public:
    //----------------------------------------------------------------------------------------
    //! @brief      TBD
    //----------------------------------------------------------------------------------------
    enum GuidSettings
    {
        GuidSettings_StringBufferSize = 64   //!< GUIDを文字列に変換時のバッファサイズ
    };

    //----------------------------------------------------------------------------------------
    //! @brief      コンストラクタです.
    //----------------------------------------------------------------------------------------
    Guid() NN_NOEXCEPT;

    //----------------------------------------------------------------------------------------
    //! @brief      引数付きコンストラクタです.
    //!
    //! @param [in]     data         GUIDの値です.
    //! @return TBD
    //----------------------------------------------------------------------------------------
    explicit Guid( const char data[ 16 ] ) NN_NOEXCEPT;

    //----------------------------------------------------------------------------------------
    //! @brief      代入演算子です.
    //!
    //! @param [in]     value       代入する値です.
    //! @return TBD
    //----------------------------------------------------------------------------------------
    Guid& operator= ( const Guid& value ) NN_NOEXCEPT;

    //----------------------------------------------------------------------------------------
    //! @brief      等価比較演算子です.
    //!
    //! @param [in]     value       比較する値です.
    //! @return TBD
    //----------------------------------------------------------------------------------------
    bool operator== ( const Guid& value ) const NN_NOEXCEPT;

    //----------------------------------------------------------------------------------------
    //! @brief      非等価比較演算子です.
    //!
    //! @param [in]     value       比較する値です.
    //! @return TBD
    //----------------------------------------------------------------------------------------
    bool operator!= ( const Guid& value ) const NN_NOEXCEPT;

    //----------------------------------------------------------------------------------------
    //! @brief     文字列に変換します.
    //! @param[in] pOutString   TBD
    //! @param[in] size         TBD
    //! @return                 TBD
    //----------------------------------------------------------------------------------------
    char* GetString( char* pOutString, size_t size ) NN_NOEXCEPT;

    //----------------------------------------------------------------------------------------
    //! @brief      値をクリアします。
    //----------------------------------------------------------------------------------------
    void Clear() NN_NOEXCEPT;

private:
    uint8_t m_Data[ 16 ];   //!< GUIDの値です.
};

} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn


