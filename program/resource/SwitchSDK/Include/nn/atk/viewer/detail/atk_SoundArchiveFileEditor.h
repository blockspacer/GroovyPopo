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

#include <nn/atk/atk_SoundArchiveFilesHook.h>
#include <nn/atk/viewer/atk_Result.h>
#include <nn/atk/viewer/detail/res/atk_ResItemInfo.h>

namespace nn { namespace atk { namespace detail { namespace fnd {
    class FileStream;
}}}}

namespace nn {
namespace atk {
namespace viewer {
namespace detail {

class SoundArchiveEditController;

//---------------------------------------------------------------------------
//! @brief    サウンドアーカイブのファイル編集機能を提供するクラスです。
//---------------------------------------------------------------------------
class SoundArchiveFileEditor
{
    NN_DISALLOW_COPY(SoundArchiveFileEditor);

public:
    //---------------------------------------------------------------------------
    //! @brief  サウンドアーカイブのパラメータアクセスをフックし、パラメータを指し替えます。
    //---------------------------------------------------------------------------
    class Hook : public atk::detail::SoundArchiveFilesHook
    {
        NN_DISALLOW_COPY(Hook);

    public:
        Hook() NN_NOEXCEPT : m_Owner(NULL) { }

    public:
        void Initialize(SoundArchiveFileEditor& owner) NN_NOEXCEPT
        {
            m_Owner = &owner;
        }

    protected:
        //---------------------------------------------------------------------------
        //! @brief         指定アイテムラベルがフック対象かどうかを調べます。
        //!
        //! @param[in]     itemLabel     アイテムラベルを指定します。
        //!
        //! @return        対象の場合は true、対象でない場合は false を返します。
        //---------------------------------------------------------------------------
        virtual bool IsTargetItemImpl(const char* itemLabel) NN_NOEXCEPT NN_OVERRIDE;

        //---------------------------------------------------------------------------
        //! @brief         フックの有無が変化しないようにロックします。
        //---------------------------------------------------------------------------
        virtual void LockImpl() NN_NOEXCEPT NN_OVERRIDE;

        //---------------------------------------------------------------------------
        //! @brief         フックのロックを解除します。
        //---------------------------------------------------------------------------
        virtual void UnlockImpl() NN_NOEXCEPT NN_OVERRIDE;

        //---------------------------------------------------------------------------
        //! @brief         指定バッファを使って指定アイテムラベルに関連するファイルを開きます。
        //!
        //! @param[in]     buffer        ファイルを開くのに利用するバッファを指定します。
        //! @param[in]     bufferLength  バッファの長さを指定します。
        //! @param         cacheBuffer   TBD
        //! @param         cacheBufferLength  TBD
        //! @param[in]     itemLabel     アイテムラベルを指定します。
        //! @param[in]     fileType      取得するファイルタイプを指定します。
        //!
        //! @return        FileStream を返します。
        //---------------------------------------------------------------------------
        virtual atk::detail::fnd::FileStream* OpenFileImpl(void* buffer, size_t bufferLength, void* cacheBuffer, size_t cacheBufferLength, const char* itemLabel, const char* fileType) NN_NOEXCEPT NN_OVERRIDE;

        //---------------------------------------------------------------------------
        //! @brief         指定アイテムラベルに関連するファイルデータへのアドレスを取得します。
        //!
        //! @param[in]     itemLabel    アイテムラベルを指定します。
        //! @param[in]     itemType     アイテム種別を指定します。
        //! @param[in]     fileType     取得するファイルタイプを指定します。
        //! @param[in]     fileIndex    取得するファイルのインデックスがあれば指定します。
        //!
        //! @return        ファイルデータへのアドレスを返します。
        //---------------------------------------------------------------------------
        virtual const void* GetFileAddressImpl(const char* itemLabel, const char* itemType, const char* fileType, uint32_t fileIndex = 0) NN_NOEXCEPT NN_OVERRIDE;

    private:
        const ResName* GetBankFilePath(const char* itemLabel, ResDataType fileDataType, uint32_t bankIndex) NN_NOEXCEPT;

        ResDataType StringToResDataType(const char* value) const NN_NOEXCEPT;

    private:
        SoundArchiveFileEditor* m_Owner;
    };

public:
    /* ctor */ SoundArchiveFileEditor() NN_NOEXCEPT;
    /* dtor */ ~SoundArchiveFileEditor() NN_NOEXCEPT {}

    //! @brief  初期化します。
    //! @param editController TBD
    //! @param soundArchivePlayer TBD
    //! @return TBD
    Result Initialize(
        SoundArchiveEditController* editController,
        SoundArchivePlayer* soundArchivePlayer) NN_NOEXCEPT;

    //! @brief  終了処理を行います。
    void Finalize() NN_NOEXCEPT;

    bool IsInitialized() const NN_NOEXCEPT { return m_SoundArchivePlayer != NULL; }

    //! @brief  ファイル編集を開始します。
    void Start() NN_NOEXCEPT;

    //! @brief  ファイル編集を終了します。
    void Stop() NN_NOEXCEPT;

private:
    Hook                        m_SoundArchiveHook;
    SoundArchiveEditController* m_EditController;
    SoundArchivePlayer*         m_SoundArchivePlayer;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
