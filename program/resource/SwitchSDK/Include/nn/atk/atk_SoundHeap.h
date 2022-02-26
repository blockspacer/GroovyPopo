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
 * :include nn/atk/atk_SoundHeap.h
 *
 * @file atk_SoundHeap.h
 */

#pragma once

#include <nn/atk/atk_SoundMemoryAllocatable.h>
#include <nn/atk/atk_FrameHeap.h>
#include <nn/atk/atk_Util.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>
#include <nn/atk/fnd/os/atkfnd_ScopedLock.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @brief    サウンドデータ用のヒープクラスです。
//!
//!           サウンドヒープでは、
//!           メモリブロックが解放された時に呼び出されるコールバックを
//!           設定することができます。
//!           サウンドライブラリではこの機能を用いて、
//!           サウンドデータを含むメモリブロックが解放された時に、
//!           安全にデータを解放するための処理を行うように設計されています。
//!
//---------------------------------------------------------------------------
class SoundHeap : public SoundMemoryAllocatable
{
public:
    //----------------------------------------
    //! @name コンストラクタ/デストラクタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief        コンストラクタです。
    //!
    //---------------------------------------------------------------------------
    SoundHeap() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        デストラクタです。
    //!
    //---------------------------------------------------------------------------
    virtual ~SoundHeap() NN_NOEXCEPT NN_OVERRIDE;
    //@}

    //----------------------------------------
    //! @name ヒープ操作
    //@{
    //---------------------------------------------------------------------------
    //! @brief        サウンドヒープを作成します。
    //!
    //!               メモリ領域のサイズ size が十分に無いと、関数は失敗します。
    //!               ヒープの管理領域が必要なため、
    //!               作成されたメモリのヒープサイズは size よりも小さくなります。
    //!
    //!               この関数に渡したメモリ領域を再利用するには、
    //!               @ref nn::atk::SoundHeap::Destroy
    //!               でヒープを破棄する必要があります。
    //!
    //! @param[in]    startAddress    ヒープとして使用するメモリの先頭アドレス。
    //! @param[in]    size            ヒープとして使用するメモリのサイズ。
    //!
    //! @return       ヒープの作成に成功したら true を、
    //!               失敗したら false を返します。
    //!
    //---------------------------------------------------------------------------
    bool Create(
        void* startAddress,
        size_t size
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief        サウンドヒープを作成します。
    //!
    //!               メモリ領域のサイズ size が十分に無いと、関数は失敗します。
    //!               ヒープの管理領域が必要なため、
    //!               作成されたメモリのヒープサイズは size よりも小さくなります。
    //!
    //!               この関数に渡したメモリ領域を再利用するには、
    //!               @ref nn::atk::SoundHeap::Destroy
    //!               でヒープを破棄する必要があります。
    //!
    //!               enableAutoMemoryPoolManagement を false に設定すると、
    //!               メモリープールへのサウンドヒープ作成時のアタッチ処理と破棄時のデタッチ処理を行わないようになります。
    //!               アプリケーションでメモリプールの管理を行いたい場合はご使用ください。
    //!
    //! @param[in]    startAddress                   ヒープとして使用するメモリの先頭アドレス。
    //! @param[in]    size                           ヒープとして使用するメモリのサイズ。
    //! @param[in]    enableAutoMemoryPoolManagement nn::atk ライブラリ内部での自動メモリプール管理機能を使用するかどうか。
    //!
    //! @return       ヒープの作成に成功したら true を、
    //!               失敗したら false を返します。
    //!
    //---------------------------------------------------------------------------
    bool Create(
        void* startAddress,
        size_t size,
        bool enableAutoMemoryPoolManagement
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        サウンドヒープを破棄します。
    //!
    //!               確保済みのメモリブロックそれぞれに対して、
    //!               @ref Allocate で設定したコールバック関数が呼び出されます。
    //!
    //---------------------------------------------------------------------------
    void Destroy() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        サウンドヒープからメモリを確保します。
    //!
    //!               各メモリブロックには管理領域が必要になります。
    //!               そのため、実際には確保するメモリサイズ size
    //!               よりも少し大きな空き容量が必要になります。
    //!               空き容量が足りない場合は、関数は失敗します。
    //!
    //! @param[in]    size            確保するメモリサイズ。
    //!
    //! @return       確保したメモリブロックの先頭アドレスを返します。
    //!
    //---------------------------------------------------------------------------
    virtual void* Allocate( size_t size ) NN_NOEXCEPT NN_OVERRIDE;

    //---------------------------------------------------------------------------
    //! @brief        サウンドヒープからメモリを確保します。
    //!
    //!               各メモリブロックには管理領域が必要になります。
    //!               そのため、実際には確保するメモリサイズ size
    //!               よりも少し大きな空き容量が必要になります。
    //!               空き容量が足りない場合は、関数は失敗します。
    //!
    //!               コールバック関数 callback は、
    //!               @ref Clear または @ref LoadState によって、
    //!               メモリブロックが解放されたときに呼び出されます。
    //!               コールバック関数が不要の時は、callback に NULL を入れます。
    //!
    //! @param[in]    size            確保するメモリサイズ。
    //! @param[in]    callback        メモリブロックが解放されるときに呼び出される
    //!                               コールバック関数。
    //! @param[in]    callbackArg     コールバック関数の引数。
    //!
    //! @return       確保したメモリブロックの先頭アドレスを返します。
    //!
    //---------------------------------------------------------------------------
    virtual void* Allocate(
        size_t size,
        SoundMemoryAllocatable::DisposeCallback callback,
        void* callbackArg
    ) NN_NOEXCEPT NN_OVERRIDE;

    //! @briefprivate
    //! @param[in] size :private
    //! @param[in] needMemoryPool :private
    //! @return :private
    virtual size_t GetAllocateSize(size_t size, bool needMemoryPool) NN_NOEXCEPT NN_OVERRIDE;

    //---------------------------------------------------------------------------
    //! @brief        確保したメモリブロックを全て解放します。
    //!
    //!               解放したメモリブロックそれぞれに対して、@ref Allocate で設定した
    //!               コールバック関数が呼び出されます。
    //!
    //---------------------------------------------------------------------------
    void Clear() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        サウンドヒープが有効かどうかを調べます。
    //!
    //!               @ref Create を呼び出してサウンドヒープが作成され、
    //!               メモリブロックの確保が可能な状態であれば、true を返します。
    //!
    //! @return       サウンドヒープが有効なら true を返します。
    //!
    //---------------------------------------------------------------------------
    bool IsValid() const NN_NOEXCEPT { return m_FrameHeap.IsValid(); }
    //@}

    //----------------------------------------
    //! @name 階層管理
    //---------------------------------------------------------------------------
    //! @brief        サウンドヒープの現在の状態を保存します。
    //!
    //!               ヒープ作成直後の階層レベルは 0 で、
    //!               この関数を呼ぶ毎に階層レベルがひとつ増えます。
    //!               @ref LoadState を呼びだすことで、
    //!               指定した階層レベルの保存直後の状態に復元させることができます。
    //!
    //!               状態の保存にはヒープを少し消費します。
    //!               ヒープの空き容量が足りない場合は、関数の呼びだしは失敗します。
    //!
    //! @return       状態保存後の階層レベルを返します。
    //!               状態の保存に失敗すると -1 を返します。
    //!
    //---------------------------------------------------------------------------
    int SaveState() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        保存したサウンドヒープの状態を復元します。
    //!
    //!               サウンドヒープを @ref SaveState で保存した直後の状態に戻します。
    //!               状態を保存してから現在までに確保されたメモリブロックは、
    //!               全て解放されます。 解放したメモリブロックそれぞれに対して、
    //!               @ref Allocate で設定したコールバック関数が呼びだされます。
    //!
    //!               階層レベル level は現在の階層レベルの値と同じか、
    //!               より小さい値を指定します。
    //!               0 を指定すると、@ref Clear と同じ意味になります。
    //!               ヒープは、状態が保存された直後の状態に戻り、
    //!               現在の階層レベルも指定した値になります。
    //!
    //! @param[in]    level   復元する階層レベル。
    //!
    //---------------------------------------------------------------------------
    void LoadState( int level ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        現在のサウンドヒープの階層レベルを取得します。
    //!
    //!               ヒープ作成直後の階層レベルは 0 です。
    //!               @ref SaveState を呼びだす毎に、階層レベルがひとつ増えます。
    //!               @ref LoadState で状態を復元すると、
    //!               現在の階層レベルも指定した階層レベルに戻ります。
    //!
    //! @return       現在の階層レベルを返します。
    //!
    //---------------------------------------------------------------------------
    int GetCurrentLevel() const NN_NOEXCEPT
    {
        detail::fnd::ScopedLock<detail::fnd::CriticalSection> lock( m_CriticalSection );
        return m_FrameHeap.GetCurrentLevel();
    }
    //@}

    //----------------------------------------
    //! @name 情報取得
    //---------------------------------------------------------------------------
    //! @brief        メモリ上のヒープのサイズを取得します。
    //!
    //! @return       メモリ上のヒープのサイズ。
    //!
    //---------------------------------------------------------------------------
    size_t GetSize() const NN_NOEXCEPT
    {
        detail::fnd::ScopedLock<detail::fnd::CriticalSection> lock( m_CriticalSection );
        return m_FrameHeap.GetSize();
    }

    //---------------------------------------------------------------------------
    //! @brief        メモリ上のヒープの空き容量を取得します。
    //!
    //! @return       メモリ上のヒープの空き容量のサイズ。
    //!
    //---------------------------------------------------------------------------
    size_t GetFreeSize() const NN_NOEXCEPT
    {
        detail::fnd::ScopedLock<detail::fnd::CriticalSection> lock( m_CriticalSection );
        return m_FrameHeap.GetFreeSize();
    }

    //---------------------------------------------------------------------------
    //! @brief      ヒープの内容をダンプします。
    //!
    //!             arc には、@ref SoundDataManager::Initialize 関数に渡した
    //!             サウンドアーカイブを指定する必要があります。
    //!
    //!             サウンドヒープに複数のサウンドアーカイブのデータをロードしていると、
    //!             本関数は正常に動作しません。
    //!
    //! @param[in]   mgr  サウンドデータマネージャです。
    //! @param[in]   arc  サウンドアーカイブです。
    //---------------------------------------------------------------------------
    void Dump( const nn::atk::SoundDataManager& mgr, const nn::atk::SoundArchive& arc ) const NN_NOEXCEPT
    {
        detail::fnd::ScopedLock<detail::fnd::CriticalSection> lock( m_CriticalSection );
        m_FrameHeap.Dump( mgr, arc );
    }
    //@}

private:
    static void DisposeCallbackFunc( void* mem, size_t size, void* arg ) NN_NOEXCEPT;

    mutable detail::fnd::CriticalSection m_CriticalSection;
    detail::FrameHeap m_FrameHeap;       // メインメモリ管理ヒープ
    nn::audio::MemoryPoolType m_MemoryPool;
    bool m_IsAutoMemoryPoolManagementEnabled;
};

} // namespace nn::atk
} // namespace nn

