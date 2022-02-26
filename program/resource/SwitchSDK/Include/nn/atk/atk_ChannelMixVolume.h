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
#include <nn/atk/atk_Global.h>

namespace nn { namespace atk {

//---------------------------------------------------------------------------
//! @briefprivate
//! @brief    複数のチャンネルのボリュームを示すクラスです。
//!
//!           チャンネルごとのボリュームを示す構造体として、 @ref MixVolume クラスが存在しますが、
//!           MixVolume が 5.1ch のスピーカーを表すものであるのに対して、
//!           こちらは最大 @ref ChannelCountMax までのチャンネル数のボリュームを扱うことができます。
//!
//!           チャンネルの順序は @ref ChannelIndex に準拠し、
//!           ChannelIndex_Count 以上の値はユーザー定義のチャンネルとなります。
//!
//!           デフォルトでは、 6 ch のボリューム値を表すクラスとなります。
//!
//! @see SoundStartable クラス
//---------------------------------------------------------------------------
class ChannelMixVolume
{
public:
    //! @briefprivate
    //! @brief ChannelMixVolume があらわすことのできる、最大チャンネル数です。
    static const int ChannelCountMax = 24;

    //! @briefprivate
    //! @brief コンストラクタです。 デフォルトでは、6 ch のボリューム値を表すクラスとなります。
    ChannelMixVolume() NN_NOEXCEPT;

    //! @briefprivate
    //! @brief コンストラクタです。 @ref MixVolume として設定した値を適用します。
    explicit ChannelMixVolume(const MixVolume& mixVolume) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief コンストラクタです。 float 型の配列をボリュームとして、設定した値を適用します。
    //! @pre pVolume != nullptr
    //! @pre 0 < volumeCount <= nn::atk::ChannelCountMax
    ChannelMixVolume(const float* pVolume, int volumeCount) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    利用するチャンネル数を設定します。
    //!
    //! @param[in] channelCount 利用するチャンネル数です
    //!
    //! @pre 0 < channelCount <= nn::atk::ChannelCountMax
    //! @return 設定に成功した場合は true を、失敗した場合は false を返します
    //---------------------------------------------------------------------------
    bool SetChannelCount(int channelCount) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    現在設定されているチャンネル数です。
    //!
    //! @return 使用するチャンネル数です
    //---------------------------------------------------------------------------
    int GetChannelCount() const NN_NOEXCEPT
    {
        return m_ChannelCount;
    }

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    特定のチャンネルに対して、ボリュームを設定します。
    //!
    //! @param[in] channelIndex ボリュームを設定するチャンネル番号です
    //! @param[in] volume ボリューム値です
    //!
    //! @pre 0 <= channelCount < nn::atk::ChannelCountMax
    //! @return 設定に成功した場合は true を返します。
    //!         @ref GetChannelCount で取得するチャンネル数の範囲内でない値にボリュームを設定しようとした場合、設定に失敗し false を返します。
    //---------------------------------------------------------------------------
    bool SetChannelVolume(int channelIndex, float volume) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    特定のチャンネルに対して、ボリュームを取得します。
    //!
    //! @param[in] channelIndex ボリュームを設定するチャンネル番号です
    //!
    //! @pre 0 <= channelCount < nn::atk::ChannelCountMax
    //! @pre 0 <= channelCount < GetChannelCount
    //!
    //! @return 指定したチャンネルのボリューム値を返します。
    //---------------------------------------------------------------------------
    float GetChannelVolume(int channelIndex) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief    特定のチャンネルに対して、ボリュームを複数個設定します。
    //!
    //! @param[in] channelIndex ボリュームを設定するチャンネル番号です
    //! @param[in] volume ボリューム値の配列です
    //! @param[in] volumeCount ボリューム値の配列数です
    //!
    //! @pre 0 <= channelIndex < nn::atk::ChannelCountMax
    //! @pre pVolume != nullptr
    //! @pre volumeCount > 0
    //! @return 設定に成功した場合は true を返します。
    //!         @ref GetChannelCount で取得するチャンネル数の範囲内でない値にボリュームを設定しようとした場合、設定に失敗し false を返します。
    //!         設定に失敗した場合、ボリュームの設定は変化しません。
    //---------------------------------------------------------------------------
    bool SetChannelVolume(int channelIndex, const float* pVolume, int volumeCount) NN_NOEXCEPT;

private:
    void InitializeChannelVolume() NN_NOEXCEPT;

    int m_ChannelCount;
    float m_ChannelVolume[ChannelCountMax];
};

}}
