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

#include <nn/atk/atk_SoundArchive.h>

namespace nn { namespace atk {

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    どのような警告であるかを表すIDです。
    //!
    //! @see      WarningCallbackArg
    //---------------------------------------------------------------------------
    enum WarningId
    {
        // プレイヤーヒープ関連
        WarningId_Start_PlayerHeap_Warning = 1000,      //<! @briefprivate
        WarningId_PlayerHeap_WaveSoundFileLoadFailed,   //<! ウェーブサウンドファイルの読み込みに失敗しました。
        WarningId_PlayerHeap_WaveFileLoadFailed         //<! ウェーブファイルの読み込みに失敗しました。
    };

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief   警告に関連する情報のインターフェイスクラスです。
    //---------------------------------------------------------------------------
    class IWarningCallbackInfo
    {
    };

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief   ウェーブサウンドのプレイヤーヒープ処理における警告発生時に情報を格納するクラスです。
    //---------------------------------------------------------------------------
    class PlayerHeapForWsdWarningCallbackInfo : public IWarningCallbackInfo
    {
    public:
        //---------------------------------------------------------------------------
        //! @brief     コンストラクタです。
        //!
        //! @param[in] itemId 対象のIDです。
        //---------------------------------------------------------------------------
        explicit PlayerHeapForWsdWarningCallbackInfo(SoundArchive::ItemId itemId)
        : m_TargetId(itemId)
        {
        }

    public:
        //---------------------------------------------------------------------------
        //! @brief     警告対象となるアイテムの ID を取得します。
        //!
        //! @return    警告対象となるアイテムの ID です。
        //---------------------------------------------------------------------------
        SoundArchive::ItemId GetTargetId() const NN_NOEXCEPT
        {
            return m_TargetId;
        }

    private:
        SoundArchive::ItemId m_TargetId;
    };

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    警告が出力されるときに呼ばれるコールバック関数の引数です。
    //!
    //! @see WarningCallback
    //---------------------------------------------------------------------------
    struct WarningCallbackArg
    {
        //! 警告の種類を示す ID です。
        WarningId warningId;

        //! 警告に関連する情報です。
        IWarningCallbackInfo* pInfo;
    };

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    警告が出力されるときに呼ばれるコールバック関数の型定義です。
    //!
    //!           @ref SoundSystem::SetWarningCallback で登録することができます。
    //!
    //! @see SoundSystem::SetWarningCallback
    //! @see SoundSystem::ClearWarningCallback
    //---------------------------------------------------------------------------
    typedef void(*WarningCallback)(WarningCallbackArg arg);

}} // namespace nn::atk
