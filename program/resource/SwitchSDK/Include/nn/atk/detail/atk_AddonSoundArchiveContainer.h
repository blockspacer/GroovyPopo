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
#include <nn/atk/atk_SoundDataManager.h>
#include <nn/atk/atk_AddonSoundArchive.h>
#include <nn/atk/detail/atk_IntrusiveList.h>

namespace nn { namespace atk { namespace detail {

    //---------------------------------------------------------------------------
    //! @internal
    //---------------------------------------------------------------------------
    class AddonSoundArchiveContainer
    {
    public:
        //----------------------------------------
        //! @name コンストラクタ / デストラクタ
        //@{
        //----------------------------------------
        //! @brief コンストラクタです。
        //----------------------------------------
        AddonSoundArchiveContainer() NN_NOEXCEPT;
        //----------------------------------------
        //! @brief デストラクタです。
        //----------------------------------------
        ~AddonSoundArchiveContainer() NN_NOEXCEPT;
        //@}

        //----------------------------------------
        //! @name 初期化・終了処理
        //@{

        //---------------------------------------------------------------------------
        //! @brief    初期化処理を行います。
        //!
        //! @param[in]    soundArchiveName    サウンドアーカイブを識別するための名前です。
        //! @param[in]    pSoundArchive       追加するサウンドアーカイブのポインタです。
        //! @param[in]    pSoundDataManager   追加するサウンドアーカイブ用の SoundDataManager のポインタです。
        //!
        //! @return 初期化に成功すれば true を、失敗すれば false を返します。
        //!
        //! @see Finalize
        //!
        //---------------------------------------------------------------------------
        bool Initialize( const char* soundArchiveName, const nn::atk::AddonSoundArchive* pSoundArchive, const nn::atk::SoundDataManager* pSoundDataManager ) NN_NOEXCEPT;

        //---------------------------------------------------------------------------
        //! @brief    破棄処理を行います。
        //!
        //! @see Initialize
        //!
        //---------------------------------------------------------------------------
        void Finalize() NN_NOEXCEPT;

        //@}

        //---------------------------------------------------------------------------
        //! @brief  引数で渡された文字列がこのサウンドアーカイブの名前と一致しているかを調べます。
        //!
        //! @return 一致していれば true、そうでなければ false を返します。
        //---------------------------------------------------------------------------
        bool IsSameName(const char* name) const NN_NOEXCEPT;

        // @briefprivate
        bool IsActive() const NN_NOEXCEPT
        {
            return m_IsActive;
        }

        const nn::atk::SoundArchive* GetSoundArchive() const NN_NOEXCEPT
        {
            NN_SDK_ASSERT_NOT_NULL(m_pSoundArchive);
            return m_pSoundArchive;
        }

        const nn::atk::SoundDataManager* GetSoundDataManager() const NN_NOEXCEPT
        {
            return m_pSoundDataManager;
        }

        const char* GetSoundArchiveName() const NN_NOEXCEPT
        {
            return m_SoundArchiveName;
        }

        void SetAddTick(const nn::os::Tick& tick) NN_NOEXCEPT;

        nn::os::Tick GetAddTick() const NN_NOEXCEPT
        {
            return m_AddTick;
        }

    public:
        static const int SoundArchiveNameLengthMax = 64;

        detail::IntrusiveListNode m_ElementLink;

    private:
        bool m_IsActive;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
        uint8_t m_Padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
        const nn::atk::SoundArchive* m_pSoundArchive;
        const nn::atk::SoundDataManager* m_pSoundDataManager;
        char m_SoundArchiveName[SoundArchiveNameLengthMax];
        nn::os::Tick m_AddTick;
    };

}}}
