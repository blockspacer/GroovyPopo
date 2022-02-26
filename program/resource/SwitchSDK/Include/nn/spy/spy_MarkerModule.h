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

#include <nn/spy/spy_Config.h>

#include <nn/spy/detail/spy_MarkerPacket.h>
#include <nn/spy/spy_SpyModule.h>

namespace nn {
namespace spy {

class Marker;

//! @briefprivate
//!
//! @brief  (調整中の機能です)
//!         任意のマーカーを Spy.exe に出力するモジュールです。
class MarkerModule : public SpyModule
{
#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

public:
    //! @brief コンストラクタです。
    NN_FORCEINLINE MarkerModule() NN_NOEXCEPT {}

    //! @brief 指定マーカーをアタッチします。
    //! @param[in]  marker  アタッチするマーカー
    NN_FORCEINLINE bool AttachMarker(Marker& marker) NN_NOEXCEPT
    {
        NN_UNUSED(&marker);
        return false;
    }

    //! @brief 指定マーカーをデタッチします。
    //! @param[in]  marker  デタッチするマーカー
    NN_FORCEINLINE void DetachMarker(Marker& marker) NN_NOEXCEPT
    {
        NN_UNUSED(&marker);
    }

#else // NN_BUILD_CONFIG_SPY_ENABLED

public:
    //! @brief コンストラクタです。
    MarkerModule() NN_NOEXCEPT;

    //! @brief 指定マーカーをアタッチします。
    //! @param[in]  marker  アタッチするマーカー
    bool AttachMarker(Marker& marker) NN_NOEXCEPT;

    //! @brief 指定マーカーをデタッチします。
    //! @param[in]  marker  デタッチするマーカー
    void DetachMarker(Marker& marker) NN_NOEXCEPT;

private:
    Marker* m_pMarkerTop;
    Marker* m_pMarkerLast;

    friend class Marker;

#endif // NN_BUILD_CONFIG_SPY_ENABLED
};

//! @briefprivate
//!
//! @brief Spy マーカーを制御します。
class Marker
{
public:
    //! マーカー値の説明の文字列長の最大値。
    static const int MaxDescriptionLength = nn::spy::detail::MarkerPacket::MaxDescriptionLength;

#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

public:
    NN_FORCEINLINE Marker() NN_NOEXCEPT { }

    NN_FORCEINLINE ~Marker() NN_NOEXCEPT { }

    NN_FORCEINLINE bool IsAttached() const NN_NOEXCEPT { return false; }

    NN_FORCEINLINE void PushValue(const char* description, uint8_t r, uint8_t g, uint8_t b) NN_NOEXCEPT
    {
        NN_UNUSED( description );
        NN_UNUSED( r );
        NN_UNUSED( g );
        NN_UNUSED( b );
    }

#else // NN_BUILD_CONFIG_SPY_ENABLED

public:
    //! @brief コンストラクタです。
    Marker() NN_NOEXCEPT;

    //! @brief デストラクタです。
    virtual ~Marker() NN_NOEXCEPT;

    //! @brief MarkerModule へのアタッチの有無を取得します。
    bool IsAttached() const NN_NOEXCEPT { return m_pModule != NULL; }

    //! @brief マーカーに値を追加します。
    //! @param[in]  description  マーカーの説明
    //! @param[in]  r            マーカー色の r 成分
    //! @param[in]  g            マーカー色の g 成分
    //! @param[in]  b            マーカー色の b 成分
    void PushValue(const char* description, uint8_t r, uint8_t g, uint8_t b) NN_NOEXCEPT;

private:
    bool IsRequested() const NN_NOEXCEPT
    {
        return IsAttached() && m_pModule->IsRequested();
    }

    void InitializePacketCommonData() NN_NOEXCEPT;

    Marker* GetPrevious() const NN_NOEXCEPT { return m_pPrevious; }
    void    SetPrevious(Marker* pPrevious) NN_NOEXCEPT { m_pPrevious = pPrevious; }

    Marker* GetNext() const NN_NOEXCEPT { return m_pNext; }
    void    SetNext(Marker* pNext) NN_NOEXCEPT { m_pNext = pNext; }

    MarkerModule* GetModule() const NN_NOEXCEPT { return m_pModule; }

    //! @brief SpyMarkerModule との関連付けを解除します。
    void Attach(MarkerModule& module) NN_NOEXCEPT;

    //! @brief SpyMarkerModule との関連付けを解除します。
    void Detach() NN_NOEXCEPT;

private:
    MarkerModule* m_pModule;

    Marker* m_pPrevious;
    Marker* m_pNext;

    nn::spy::detail::MarkerPacket::ValuePacketPayload m_ValuePacketPayload;

    friend class MarkerModule;

#endif // NN_BUILD_CONFIG_SPY_ENABLED
};

} // namespace nn::spy
} // namespace nn
