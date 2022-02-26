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

#include <nn/spy/detail/fnd/binary/spyfnd_Binary.h>
#include <nn/spy/detail/spy_SpyDataId.h>

namespace nn {
namespace spy {

class SpyController;
class SpyModule;

//! @brief  nn::spy ライブラリ、Spy.exe で扱うデータ情報を格納します。
class SpyDataInfo
{
#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

public:
    NN_FORCEINLINE SpyDataInfo(const char* dataName, const uint32_t dataVersion) NN_NOEXCEPT
    {
        NN_UNUSED(dataName);
        NN_UNUSED(dataVersion);
    }

    NN_FORCEINLINE const char* GetDataName() const NN_NOEXCEPT
    {
        return NULL;
    }

    NN_FORCEINLINE uint32_t GetDataVersion() const NN_NOEXCEPT
    {
        return 0;
    }

#else // NN_BUILD_CONFIG_SPY_ENABLED

public:
    //! @brief  コンストラクタです。
    //!
    //! @param[in]  dataName     データ名を指定します。
    //!                          名前は ASCII 文字列で終端文字を含め、128 文字以下にしてください。
    //! @param[in]  dataVersion  データバージョンを指定します。
    //!                          バージョンは、2 バイト刻みで メジャー、マイナー、マイクロ、(未使用) の 4 つの数値を示します。
    //!                          バージョン番号 0x00000000 は不正値とみなされます。
    SpyDataInfo(const char* dataName, const uint32_t dataVersion) NN_NOEXCEPT
        : m_DataId(nn::spy::detail::SpyDataId_Invalid)
        , m_DataName(dataName)
        , m_DataVersion(dataVersion)
    {
    }

    //! @brief データ名を取得します。
    const char* GetDataName() const NN_NOEXCEPT
    {
        return m_DataName;
    }

    //! @brief データバージョンを取得します。
    uint32_t GetDataVersion() const NN_NOEXCEPT
    {
        return m_DataVersion;
    }

    //! @briefprivate
    //! @brief セッション中に有効なDataIdの値を取得します。
    //!
    //! @details
    //! Spy.exe により割り当てられたデータ名に対応する DataId です。
    nn::spy::detail::SpyDataId GetDataId() const NN_NOEXCEPT
    {
        return static_cast<nn::spy::detail::SpyDataId>(m_DataId);
    }

private:
    //! @briefprivate
    //! @brief セッション中に有効なDataIdを割り当てます。
    //!
    //! @details
    //! nn::spy ライブラリは、自身がサポートするデータの名前を Spy.exe に通知し、Spy.exe が動的に DataId を割り当てます。
    //! DataId は、この関数で SpyDataInfo に設定され、セッションが有効な間、保持されます。
    void SetDataId(nn::spy::detail::SpyDataId dataId) NN_NOEXCEPT
    {
        m_DataId = static_cast<uint16_t>(dataId);
    }

private:
    uint16_t m_DataId;
    const char* m_DataName;
    uint32_t m_DataVersion;

    friend class SpyModule;

#endif // NN_BUILD_CONFIG_SPY_ENABLED
};

} // namespace nn::spy
} // namespace nn
