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

namespace nn {
namespace vfx {

class EmitterSet;

//---------------------------------------------------------------------------
//! @brief  エフェクト操作ハンドルクラスです。
//!
//!         生成済みのエミッタセットへは、このクラスを取得できます。
//---------------------------------------------------------------------------
class Handle
{
    NN_DISALLOW_COPY( Handle );
public:
    //----------------------------------------
    //! @name   コンストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタ。
    //---------------------------------------------------------------------------
    Handle() NN_NOEXCEPT
        : m_pEmitterSet( NULL )
        , m_CreateId( InvalidValueId_EmitterSetId )
    {
    }

    //@}

    //----------------------------------------
    //! @name   ハンドルへの操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief  ハンドルを無効化します。
    //!
    //!         放出済みのエミッタセットはそのままに、ハンドルだけ無効化します。
    //!         ハンドルを使いまわす時に使用します。
    //---------------------------------------------------------------------------
    void Invalidate() NN_NOEXCEPT
    {
        m_pEmitterSet = NULL;
        m_CreateId = InvalidValueId_EmitterSetId;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタセットへのポインタを取得します。
    //! @return エミッタセットへのポインタ
    //---------------------------------------------------------------------------
    EmitterSet* GetEmitterSet() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  エミッタセットへのポインタを取得します。( const版 )
    //! @return エミッタセットへのポインタ
    //---------------------------------------------------------------------------
    const EmitterSet* GetEmitterSet() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ハンドルが有効かどうかチェックします。
    //! @return ハンドルが有効かどうか
    //---------------------------------------------------------------------------
    bool IsValid() const NN_NOEXCEPT
    {
        return ( m_pEmitterSet != NULL ) && ( m_CreateId == m_pEmitterSet->GetCreateId() );
    }

    //@}

private:
    EmitterSet* m_pEmitterSet;  //!< エミッタセットへのポインタ
    int         m_CreateId;     //!< エミッタセットの生成ＩＤ

    friend class System;
};

} // namespace vfx
} // namespace nn
