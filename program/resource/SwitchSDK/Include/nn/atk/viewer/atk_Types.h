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
//! @brief  【β版】キャッシュ状態を示す列挙体です。EDIT_ITEM_TYPE_STREAM_SOUND
//---------------------------------------------------------------------------
enum CacheState
{
    CacheState_None = 0,   //!< キャッシュされていません。
    CacheState_Caching,    //!< キャッシュ中です。
    CacheState_Cached      //!< キャッシュ済です。
};

//---------------------------------------------------------------------------
//! @brief  【β版】編集アイテムの種別を示す列挙体です。
//---------------------------------------------------------------------------
enum EditItemType
{
    EditItemType_Unknown = 0,     // !< 不明な種別です。
    EditItemType_StreamSound = 1, // !< ストリームサウンドです。
    EditItemType_WaveSound,       // !< ウェーブサウンドです。
    EditItemType_SequenceSound,   // !< シーケンスサウンドです。
    EditItemType_Bank             // !< バンクです。
};

//---------------------------------------------------------------------------
//! @brief  【β版】編集アイテム情報を格納する構造体です。
//---------------------------------------------------------------------------
struct EditItemInfo
{
    uint32_t          id;                    //!< 編集アイテムの ID です。
    const char*  name;                  //!< 編集アイテムの名前です。
    EditItemType itemType;              //!< 編集アイテムの種別です。
    bool         isOverrideOriginal;    //!< オリジナルアイテムオーバーライドの有無を示すフラグです。
};

//! SoundMaker との通信に利用するポート番号の型定義です。
typedef uint32_t PORT;

//---------------------------------------------------------------------------
//! @internal
//! @brief  編集アイテムの変更を通知するコールバック関数です。
//---------------------------------------------------------------------------
typedef void (*EditItemsChangedCallback)(uintptr_t userParam);

namespace detail {

//---------------------------------------------------------------------------
//! @brief  【β版】32 バイトハッシュ値です。
//---------------------------------------------------------------------------
struct Hash32
{
    static const uint32_t Size = 32;

    uint8_t value[Size];

    Hash32() NN_NOEXCEPT { }

    explicit Hash32(const uint8_t* source) NN_NOEXCEPT
    {
        const uint32_t* sourceCurrent = reinterpret_cast<const uint32_t*>(source);
        uint32_t* current = reinterpret_cast<uint32_t*>(value);

        // 32 バイトなので、 uint32_t で 8 回設定します。
        for(uint32_t count = 0; count < Size / sizeof(uint32_t); ++count)
        {
            *current = *sourceCurrent;

            ++current;
            ++sourceCurrent;
        }
    }

    static const Hash32& Empty() NN_NOEXCEPT
    {
        static bool isInitialized = false;
        static Hash32 instance;

        if(!isInitialized)
        {
            instance.SetEmpty();
            isInitialized = true;
        }

        return instance;
    }

    void SetEmpty() NN_NOEXCEPT
    {
        uint32_t* current = reinterpret_cast<uint32_t*>(value);

        // 32 バイトなので、 uint32_t で 8 回 0 を設定します。
        for(uint32_t count = 0; count < Size / sizeof(uint32_t); ++count)
        {
            current = 0;
            ++current;
        }
    }

    bool operator ==(const Hash32& rhs) const NN_NOEXCEPT
    {
        const uint32_t* lhsCurrent = reinterpret_cast<const uint32_t*>(value);
        const uint32_t* rhsCurrent = reinterpret_cast<const uint32_t*>(rhs.value);

        // 32 バイトなので、 uint32_t で 8 回比較します。
        for(uint32_t count = 0; count < Size / sizeof(uint32_t); ++count)
        {
            if(*lhsCurrent != *rhsCurrent)
            {
                return false;
            }

            ++lhsCurrent;
            ++rhsCurrent;
        }

        return true;
    }

    bool operator !=(const Hash32& rhs) const NN_NOEXCEPT { return !(*this == rhs); }

    void ToString(char* buffer, uint32_t length) const NN_NOEXCEPT;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
