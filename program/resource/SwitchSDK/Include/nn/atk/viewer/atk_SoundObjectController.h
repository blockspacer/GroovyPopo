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

#include <nn/atk/atk_SoundHandle.h>
#include <nn/atk/viewer/atk_Result.h>
#include <nn/atk/viewer/detail/atk_IErrorProvider.h>
#include <nn/atk/viewer/detail/atk_SoundControllerImpl.h>

namespace nn {
namespace atk {

class SoundArchivePlayer;

namespace viewer {

//---------------------------------------------------------------------------
//! @brief    【β版】サウンド関連のオブジェクトを操作するクラスです。
//!
//!           内包するサウンドハンドルを使って外部からサウンド制御するために利用します。
//---------------------------------------------------------------------------
class SoundObjectController : private viewer::detail::IErrorProvider
{
    NN_DISALLOW_COPY(SoundObjectController);

public: // 定数の定義
    static const uint32_t SoundControllerCount = 4;    //!< SoundObjectController が保持する SoundController の数です。

public: // コンストラクタ
    //===========================================================================
    //! @name コンストラクタ/デストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //---------------------------------------------------------------------------
    SoundObjectController() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  デストラクタです。
    //---------------------------------------------------------------------------
    virtual ~SoundObjectController() NN_NOEXCEPT NN_OVERRIDE { }

    //@}
    // コンストラクタ/デストラクタ
    //===========================================================================

public: // メソッド
    //===========================================================================
    //! @name 初期化
    //@{

    //---------------------------------------------------------------------------
    //! @brief      SoundObjectController を初期化します。
    //!
    //! @param[in]  soundArchivePlayer  操作対象のサウンドアーカイブプレイヤーを指定します。
    //!
    //! @return     処理結果を返します。
    //!
    //! @see        Finalize()
    //! @see        IsInitialized()
    //---------------------------------------------------------------------------
    viewer::Result Initialize(SoundArchivePlayer* soundArchivePlayer) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief       SoundObjectController の終了処理を行います。
    //!
    //! @see         Initialize()
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;

    //@}
    // 初期化
    //===========================================================================

    //===========================================================================
    //! @name 状態の取得
    //@{

    //---------------------------------------------------------------------------
    //! @brief   初期化の有無を取得します。
    //!
    //! @return  初期化済みの場合は true、初期化されていない場合は false を返します。
    //!
    //! @see     Initialize()
    //---------------------------------------------------------------------------
    bool IsInitialized() const NN_NOEXCEPT { return m_IsInitialized; }

    //@}
    // 状態の取得
    //===========================================================================

    //===========================================================================
    //! @name 操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief   SoundObjectController を更新します。
    //!
    //!          この関数を呼び出すと、サウンドのパラメータや状態を同期します。
    //---------------------------------------------------------------------------
    void Update() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  サウンドコントローラを取得します。
    //!
    //! @param[in]  index  取得するサウンドコントローラのインデックスを指定します。
    //!                    @n
    //!                    上限値は SoundControllerCount です。
    //!
    //! @return  対象のサウンドコントローラを返します。
    //!
    //! @see     SoundControllerCount
    //---------------------------------------------------------------------------
    SoundController* GetSoundController(uint32_t index) NN_NOEXCEPT
    {
        NN_SDK_ASSERT(index < SoundControllerCount);
        return &m_SoundController[index];
    }

    //@}
    // 操作
    //===========================================================================

    //===========================================================================
    //! @name エラーハンドリング
    //@{

    //---------------------------------------------------------------------------
    //! @brief         最後に発生したエラー情報を取得します。
    //!
    //! @return        最後に発生したエラー情報を返します。
    //!
    //! @see           SetLastError()
    //---------------------------------------------------------------------------
    virtual viewer::Result GetLastError() const NN_NOEXCEPT NN_OVERRIDE { return m_LastError; }

    //---------------------------------------------------------------------------
    //! @brief         最後に発生したエラー情報を設定します。
    //!
    //! @param[in]     value  エラー値を指定します。
    //!
    //! @see           GetLastError()
    //---------------------------------------------------------------------------
    virtual void SetLastError(viewer::ResultType value) NN_NOEXCEPT NN_OVERRIDE;

    //@}
    // エラーハンドリング
    //===========================================================================

private: // メンバ変数
    viewer::Result m_LastError;         //!< 最後に発生したエラー情報です。

    SoundHandle                   m_SoundHandles[SoundControllerCount];
    detail::SoundControllerImpl m_SoundController[SoundControllerCount];

    bool m_IsInitialized;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t m_Padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
};

} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
