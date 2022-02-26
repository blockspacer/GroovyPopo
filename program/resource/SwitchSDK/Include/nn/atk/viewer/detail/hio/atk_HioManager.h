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

#include <nn/util/util_IntrusiveList.h>
#include <nn/atk/viewer/detail/hio/atk_HioChannel.h>

namespace nn {
namespace atk {

namespace detail {
namespace fnd {
class FileStream;
}
}

namespace viewer {
namespace detail {

//---------------------------------------------------------------------------
//! @brief    HIO管理クラスです。
//!
//! @details  HIOの初期化やHIOチャンネルの管理、更新を行います。
//---------------------------------------------------------------------------
class HioManager
{
private:
    typedef util::IntrusiveList<HioChannel::Reference, util::IntrusiveListMemberNodeTraits<HioChannel::Reference, &HioChannel::Reference::node>> HioChannelRefList;

public: // コンストラクタ
    HioManager() NN_NOEXCEPT;

public:
    //! @brief  初期化します。
    //! @param fsClient TBD
    //! @return TBD
    bool Initialize() NN_NOEXCEPT;

    //! @brief  終了処理を行います。
    void Finalize() NN_NOEXCEPT;

    //! @brief  初期化の有無を取得します。
    //! @return TBD
    bool IsInitialized() const NN_NOEXCEPT { return m_IsInitialized; }

    //! @brief  ファイルを開くのに必要なメモリサイズを取得します。
    //! @return TBD
    static size_t GetRequiredMemorySizeForOpenFile() NN_NOEXCEPT;

    //! @brief  チャンネルを登録します。
    //! @param channel TBD
    void RegisterChannel(HioChannel& channel) NN_NOEXCEPT;

    //! @brief  チャンネルの登録を解除します。
    //! @param channel TBD
    void UnregisterChannel(HioChannel& channel) NN_NOEXCEPT;

    //! @brief  更新します。
    //! @param withAsyncChannels TBD
    void Update(bool withAsyncChannels = true) NN_NOEXCEPT;

    //! @brief  ファイルを開きます。
    //! @param buffer TBD
    //! @param bufferLength TBD
    //! @param filePath TBD
    //! @return TBD
    atk::detail::fnd::FileStream* OpenFile(void* buffer, size_t bufferLength, const char* filePath) NN_NOEXCEPT;

private: // メソッド
    bool Polling(bool withAsyncChannels) NN_NOEXCEPT;

    //! @brief  すべての非同期チャンネルを更新します。
    void UpdateAsyncChannels() NN_NOEXCEPT;

private: // メンバ変数
    HioChannelRefList m_ChannelRefs;        //!< HIOチャンネル参照リストです。

    bool m_IsInitialized;
};

} // namespace nn::atk::viewer::detail
} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
