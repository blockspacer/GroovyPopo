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

#include <atomic>

#include <nn/spy/spy_Config.h>

#include <nn/os/os_Tick.h>

#include <nn/spy/detail/spy_SpyDataId.h>
#include <nn/spy/spy_SpyDataInfo.h>

namespace nn {
namespace spy {

class SpyController;
class SpyDataInfo;

//! @brief  アプリケーションなどから受け取ったデータを加工し、SpyController を使って Spy.exe に送信するための基本機能を提供します。
class SpyModule
{
    NN_DISALLOW_COPY(SpyModule);
    NN_DISALLOW_MOVE(SpyModule);

#if !defined(NN_BUILD_CONFIG_SPY_ENABLED)

protected:
    NN_FORCEINLINE SpyModule() NN_NOEXCEPT
        : m_DataInfo("", NN_DETAIL_SPY_FND_BINARY_MAKE_U32_SIGNATURE(0, 0, 0, 1))
    {
    }

public:
    NN_FORCEINLINE ~SpyModule() NN_NOEXCEPT
    {}

    NN_FORCEINLINE const SpyDataInfo& GetDataInfo() const NN_NOEXCEPT
    {
        return m_DataInfo;
    }

    NN_FORCEINLINE SpyController* GetController() const NN_NOEXCEPT
    {
        return NULL;
    }

    NN_FORCEINLINE bool IsRegistered() const NN_NOEXCEPT
    {
        return false;
    }

    NN_FORCEINLINE bool IsRequested() const NN_NOEXCEPT
    {
        return false;
    }

private:
    SpyDataInfo m_DataInfo;

#else // NN_BUILD_CONFIG_SPY_ENABLED

protected:
    //! @briefprivate
    //! @brief コンストラクタです。
    //!
    //! @param[in] dataName データ名です。
    //! @param[in] dataVersion データバージョンです。
    //!
    SpyModule(const char* dataName, const uint32_t dataVersion) NN_NOEXCEPT
        : m_DataInfo(dataName, dataVersion)
        , m_pPrevious(NULL)
        , m_pNext(NULL)
        , m_pController(NULL)
        , m_RegisterCount(0)
        , m_IsRequested(false)
    {
    }

public:
    //! @brief デストラクタです。
    virtual ~SpyModule() NN_NOEXCEPT;

    //! @brief 対象の SpyDataInfo を返します。
    const SpyDataInfo& GetDataInfo() const NN_NOEXCEPT
    {
        return m_DataInfo;
    }

    //! @brief 対象の SpyController を返します。
    //!
    //! SpyController との関連付けを保持するために使用します。
    //!
    SpyController* GetController() const NN_NOEXCEPT
    {
        return m_pController;
    }

    //! @brief SpyController に登録されているか調べます。
    //!
    //! @return
    //! 登録済みなら true を返します。
    //!
    bool IsRegistered() const NN_NOEXCEPT
    {
        return m_pController != NULL;
    }

    //! @brief Spy.exe からのデータ要求の状態を取得します。
    //!
    //! セッションの開始時、および終了時に false に初期化されます。
    //!
    //! @return
    //! データが要求されていれば true を返します。
    //!
    bool IsRequested() const NN_NOEXCEPT
    {
        return m_IsRequested;
    }

protected:
    //! @briefprivate
    //! @brief モジュールの登録を通知します。
    //!
    //! @details
    //! 導出クラスでは、最初に基底クラスの実装を呼びだしてください。
    //!
    //! この関数をオーバーライドすることで、
    //! 登録対象の SpyController に依存した初期化などを行うことができます。
    virtual void OnRegistered() NN_NOEXCEPT
    {
    }

    //! @briefprivate
    //! @brief モジュールの登録が解除されたことを通知します。
    //!
    //! @details
    //! 導出クラスでは、最初に基底クラスの実装を呼びだしてください。
    //!
    //! この関数をオーバーライドすることで、
    //! 登録対象の SpyController に依存した後処理などを行うことができます。
    virtual void OnUnregistered() NN_NOEXCEPT
    {
    }

    //! @briefprivate
    //! @brief セッションの開始を通知します。
    //!
    //! 導出クラスでは、最初に基底クラスの実装を呼びだしてください。
    //!
    //! この関数をオーバーライドすることで、
    //! セッション中の初めてのデータ送信を区別するための
    //! フラグを初期化するなどの処理をおこなうことができます。
    //!
    virtual void OnSessionStarted() NN_NOEXCEPT
    {
        m_IsRequested = false;
    }

    //! @briefprivate
    //! @brief Spy.exe からのデータ要求を通知します。
    //!
    //! Spy.exe からデータ要求が来たときに、各データインフォに、
    //! データ要求の有無が通知されます。
    //!
    //! セッションの終了時には isRequested = false で呼び出されます。
    //!
    //! 導出クラスでは、最初に基底クラスの実装を呼びだしてください。
    //!
    //! @param[in] isRequested  データが要求されているときは true が指定されます。
    //!
    virtual void OnRequested(bool isRequested) NN_NOEXCEPT
    {
        m_IsRequested = isRequested;
    }

    //! @briefprivate
    //! @brief データを送信します。
    //!
    //! @details
    //! データは一旦蓄積され、まとめて送信されます。
    //!
    //! @param[in] buffer 送信するデータです。
    //! @param[in] length データの長さ（バイト数）です。
    //!
    //! @return 正常に蓄積できたときは true を返します。
    //!
    bool PushData(const void* buffer, size_t length) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief データを送信します。
    //!
    //! @details
    //! データは一旦蓄積され、まとめて送信されます。
    //!
    //! @tparam T 送信するデータの型です。
    //! @param[in] data 送信するデータです。
    //!
    //! @return 正常に蓄積できたときは true を返します。
    //!
    template <typename T>
    bool PushData(const T& data) NN_NOEXCEPT
    {
        return PushData(&data, sizeof(T));
    }

    //! @briefprivate
    //! @brief データを送信します。
    //!
    //! @details
    //! 複数のバッファの内容を連結して１つのデータとして送信します。
    //! データは一旦蓄積され、まとめて送信されます。
    //!
    //! @param[in] buffers 送信するデータへのポインタの配列です。
    //! @param[in] lengths 送信するデータのサイズの配列です。
    //! @param[in] count buffers および lengths の要素数です。
    //!
    //! @return 正常に蓄積できたときは true を返します。
    //!
    bool PushData(
        const void* const buffers[],
        const size_t lengths[],
        int count) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief Tick を指定してデータを送信します。
    //!
    //! @details
    //! データは一旦蓄積され、まとめて送信されます。
    //!
    //! @param[in] buffer 送信するデータです。
    //! @param[in] length データの長さ（バイト数）です。
    //! @param[in] tick 送信タイミングを示す Tick です。
    //!
    //! @return 正常に蓄積できたときは true を返します。
    //!
    bool PushDataAt(const void* buffer, size_t length, nn::os::Tick tick) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief Tick を指定してデータを送信します。
    //!
    //! @details
    //! データは一旦蓄積され、まとめて送信されます。
    //!
    //! @tparam T 送信するデータの型です。
    //! @param[in] data 送信するデータです。
    //! @param[in] tick 送信タイミングを示す Tick です。
    //!
    //! @return 正常に蓄積できたときは true を返します。
    //!
    template <typename T>
    bool PushDataAt(const T& data, nn::os::Tick tick) NN_NOEXCEPT
    {
        return PushDataAt(&data, sizeof(T), tick);
    }

    //! @briefprivate
    //! @brief Tick を指定してデータを送信します。
    //!
    //! @details
    //! 複数のバッファの内容を連結して１つのデータとして送信します。
    //! データは一旦蓄積され、まとめて送信されます。
    //!
    //! @param[in] buffers 送信するデータへのポインタの配列です。
    //! @param[in] lengths 送信するデータのサイズの配列です。
    //! @param[in] count buffers および lengths の要素数です。
    //! @param[in] tick 送信タイミングを示す Tick です。
    //!
    //! @return 正常に蓄積できたときは true を返します。
    //!
    bool PushDataAt(
        const void* const buffers[],
        const size_t lengths[],
        int count,
        nn::os::Tick tick) NN_NOEXCEPT;

private:
    //! @brief リンクリストの前の要素を返します。
    //!
    //! SpyController が、SpyDataInitializer の保持に使用します。
    //!
    SpyModule* GetPrevious() const NN_NOEXCEPT
    {
        return m_pPrevious;
    }

    //! @brief リンクリストの前の要素を設定します。
    //!
    //! SpyModule が、SpyDataInitializer の保持に使用します。
    //!
    void SetPrevious(SpyModule* pPrevious) NN_NOEXCEPT
    {
        m_pPrevious = pPrevious;
    }

    //! @brief リンクリストの次の要素を返します。
    //!
    //! SpyController が、SpyModule の保持に使用します。
    //!
    SpyModule* GetNext() const NN_NOEXCEPT
    {
        return m_pNext;
    }

    //! @brief リンクリストの次の要素を設定します。
    //!
    //! SpyController が、SpyModule の保持に使用します。
    //!
    void SetNext(SpyModule* pNext) NN_NOEXCEPT
    {
        m_pNext = pNext;
    }

    //! @brief SpyController と関連付けます。
    //!
    //! @param[in] controller  関連付ける SpyController を指定します。
    //!
    void Attach(SpyController& controller) NN_NOEXCEPT;

    //! @brief SpyController との関連付けを解除します。
    //!
    //! @param[in] force  true を指定した場合は登録回数によらず解除します。
    //!
    void Detach(bool force) NN_NOEXCEPT;

    //! @brief SpyController への登録回数を取得します。
    const int GetRegisterCount() NN_NOEXCEPT
    {
        return m_RegisterCount;
    }

    void AllocateDataId(nn::spy::detail::SpyDataId dataId) NN_NOEXCEPT;

private:
    SpyDataInfo m_DataInfo;

    SpyModule* m_pPrevious;
    SpyModule* m_pNext;

    SpyController* m_pController;

    uint8_t m_RegisterCount;
    std::atomic<bool> m_IsRequested;

    friend class nn::spy::SpyController;

#endif // NN_BUILD_CONFIG_SPY_ENABLED
};

} // namespace nn::spy
} // namespace nn
