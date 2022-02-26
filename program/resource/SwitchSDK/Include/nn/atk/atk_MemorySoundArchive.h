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
 * :include nn/atk/atk_MemorySoundArchive.h
 *
 * @file atk_MemorySoundArchive.h
 */

#pragma once

#include <nn/atk/atk_Config.h>
#include <nn/atk/atk_SoundArchiveFileReader.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @brief        メモリ上にあるサウンドアーカイブを扱うクラスです。
//!
//!               MemorySoundArchive クラスでは、
//!               サウンドアーカイブから外部参照されているデータを扱うことができません。
//!               外部参照データの再生は必ず失敗します。
//!
//!               MemorySoundArchive では既にメモリ上にサウンドアーカイブが展開されているため、
//!               波形アーカイブの個別ロード機能を使用するメリットはありません。
//!               逆に、サウンドアーカイブが必要とするメモリが
//!               個別ロード機能の管理領域の分だけ大きくなってしまうため、ご注意ください。
//---------------------------------------------------------------------------
class MemorySoundArchive : public SoundArchive
{
public:
    //----------------------------------------
    //! @name コンストラクタ / デストラクタ
    //@{
    //----------------------------------------
    //! @brief コンストラクタです。
    //----------------------------------------
    MemorySoundArchive() NN_NOEXCEPT;
    //----------------------------------------
    //! @brief デストラクタです。
    //----------------------------------------
    virtual ~MemorySoundArchive() NN_NOEXCEPT NN_OVERRIDE;
    //@}

    //----------------------------------------
    //! @name 初期化・終了処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief    メモリ上のサウンドアーカイブデータを初期化します。
    //!
    //! @param[in]    soundArchiveData    サウンドアーカイブデータのアドレス。
    //!
    //! @return 初期化に成功すれば true を、失敗すれば false を返します。
    //!
    //! @see Finalize
    //!
    //---------------------------------------------------------------------------
    bool Initialize( const void* soundArchiveData ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    メモリ上のサウンドアーカイブデータを破棄します。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;
    //@}

    //! @internal
    //! @return :private
    virtual size_t detail_GetRequiredStreamBufferSize() const NN_NOEXCEPT NN_OVERRIDE;

    //! @internal
    //! @param fileId :private
    virtual const void* detail_GetFileAddress( FileId fileId ) const NN_NOEXCEPT NN_OVERRIDE;

protected:

    //! @internal
    //! @param buffer :private
    //! @param size :private
    //! @param begin :private
    //! @param length :private
    //! @return :private
    virtual detail::fnd::FileStream* OpenStream( void* buffer, size_t size, detail::position_t begin, size_t length ) const NN_NOEXCEPT NN_OVERRIDE;

    //! @internal
    //! @param buffer :private
    //! @param size :private
    //! @param extFilePath :private
    //! @param cacheBuffer :private
    //! @param cacheSize :private
    //! @return :private
    virtual detail::fnd::FileStream* OpenExtStream(
            void* buffer,
            size_t size,
            const char* extFilePath,
            void* cacheBuffer,
            size_t cacheSize ) const NN_NOEXCEPT NN_OVERRIDE;

private:
    const void* m_pData;
    detail::SoundArchiveFileReader m_FileReader;
};

} // namespace nn::atk
} // namespace nn

