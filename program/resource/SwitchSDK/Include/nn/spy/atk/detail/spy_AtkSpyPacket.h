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

#include <nn/spy.h>

#if defined(NN_BUILD_CONFIG_SPY_ENABLED)

#include <nn/spy/detail/spy_PacketUtility.h>

namespace nn {
namespace spy {
namespace atk {
namespace detail {

struct PacketTraits
{
    static const uint32_t Version = NN_SPY_PACKET_VERSION(0, 3, 0, 0);
};

enum PacketType : uint8_t
{
    PacketType_SoundData,
    PacketType_PlayerData,
    PacketType_SoundState,
    PacketType_AddonSoundArchiveData,
    PacketType_GlobalSequenceVariable,
    PacketType_SequenceVariable,
#if 0 // 参考用
    PacketType_Profile,
#endif
    PacketType_Num
};

struct PacketHeader
{
    uint8_t type;
    uint8_t padding1;
    uint8_t padding2;
    uint8_t padding3;

public:
    explicit PacketHeader(PacketType type)
        : type(type)
        , padding1(0)
        , padding2(0)
        , padding3(0)
    {
    }
};

#if 0 // 参考用

struct ProfilePacket
{
    PacketHeader header;
    uint32_t audioFrame;
    uint32_t voiceCount;
    int64_t nwFrameProcessBegin;
    int64_t nwFrameProcessEnd;
    // int64_t nwVoiceParamUpdateBegin;
    // int64_t nwVoiceParamUpdateEnd;
    // int64_t ppcVoiceRenderingBegin;
    // int64_t ppcVoiceRenderingEnd;
    // int64_t auxProcessBegin;
    // int64_t auxProcessEnd;
    int64_t dspFrameProcessBegin;
    int64_t dspFrameProcessEnd;
    // int64_t syncVoiceParamBegin;
    // int64_t syncVoiceParamEnd;
    // int64_t outputFormatProcessBegin;
    // int64_t outputFormatProcessEnd;
    int64_t mainMixProcessBegin;
    int64_t mainMixProcessEnd;
    int64_t finalMixProcessBegin;
    int64_t finalMixProcessEnd;
    // int64_t axIntrBegin;
    // int64_t axIntrEnd;

    ProfilePacket()
        : header(PacketType_Profile)
    {
    }
};

#endif

struct GlobalSequenceVariablePacket
{
    PacketHeader header;
    uint8_t variableCount;
    uint8_t actualVariableCount;
    uint8_t padding[2];
    int16_t variables[nn::atk::SequenceSoundHandle::VariableIndexMax + 1];

    GlobalSequenceVariablePacket()
        : header(PacketType_GlobalSequenceVariable)
    {
    }
};

struct SequenceVariablePacket
{
    static const int VariableCount = nn::atk::SequenceSoundHandle::VariableIndexMax + 1;
    static const int VariableCountArea = nn::atk::SequenceSoundHandle::VariableIndexMax + 1 + 1;  // 変数の個数の保存領域サイズ (ローカル変数 + トラック変数)
    static const int ItemAreaSize = 2048;  // Itemの最大サイズが 590バイト、1024だと中途半端に 1アイテムしか入らなかったので、少し大きめ(最大サイズで3アイテム入る)の値にしました。

    struct Item
    {
        uint32_t soundInstanceId;
        uint32_t soundId;
        ::nn::spy::detail::IdData archiveInstanceId;
        uint8_t availableVariable;  // !0 = 変数が有効
        uint8_t trackCount;
        uint8_t localVariableCount;
        uint8_t trackVariableCount;

        // 変数の個数, 個数分の変数データのセットが、ローカル変数、トラック変数詰め込まれます。
        // 現在は、ローカル変数が 16個の1セット、トラック変数が 16個の 16セットです。
        // 変数の個数は、値が設定されているところまでの個数になりますので、必ず localValueCount, trackValueCountと一致するとは限りません。
        // (例) (1, 2, 3, 4, -1, -1, ...)は、個数4, その後に続くデータは、(1, 2, 3, 4)になります。 (値が設定されていないと変数の値は -1になります)
        //      (-1, -1, ...)のように、全くデータが設定されていない場合には、個数0, その後にデータはありません。
        int16_t variables[VariableCount * (nn::atk::SequenceSoundHandle::TrackIndexMax + 1) + VariableCount + VariableCountArea];  // ローカル変数、トラック変数の最大個数分 + 変数の個数を保存領域分を確保
    };

    PacketHeader header;
    uint8_t itemCount;
    uint8_t padding[3];
    uint32_t itemArea[ItemAreaSize / sizeof(uint32_t)];

    SequenceVariablePacket()
        : header(PacketType_SequenceVariable)
    {
    }
};

struct SoundStatePacket
{
    static const int ItemCountMax = 1024;

    static const uint8_t StatusStart = (1 << 0);
    static const uint8_t StatusPause = (1 << 1);
    static const uint8_t StatusPrepared = (1 << 2);

    struct Item
    {
        uint32_t instanceId;
        uint32_t soundId;
        uint8_t statusBitFlag;
        uint8_t padding1[3];
        ::nn::spy::detail::IdData archiveInstanceId;
        float volume;
        float pitch;
        float lpf;
        int bqfType;
        float bqfValue;
        int playerPriority;
        float pan;
        float surroundPan;
    };

    PacketHeader header;
    uint16_t itemSize;
    uint8_t padding[2];
    uint32_t itemCount;
    Item items[ItemCountMax];

    SoundStatePacket()
        : header(PacketType_SoundState)
    {
    }
};

struct SoundDataPacket
{
    static const int ItemAreaSize = 1024;

    enum SoundType
    {
        SoundType_Sequence = 0,
        SoundType_Wave = 1,
        SoundType_Stream = 2
    };

    struct Item
    {
        uint32_t soundId;
        uint32_t playerId;

        uint8_t soundType;
        uint8_t volume;
        uint8_t playerPriority;
        uint8_t padding1[1];

        ::nn::spy::detail::StringData<uint8_t, 255> label;
    };

    PacketHeader header;
    ::nn::spy::detail::IdData archiveInstanceId;
    uint32_t itemCount;
    uint32_t itemArea[ItemAreaSize / sizeof(uint32_t)];

    SoundDataPacket()
        : header(PacketType_SoundData)
    {
    }
};

struct PlayerDataPacket
{
    static const int ItemAreaSize = 1024;

    struct Item
    {
        uint32_t playerId;

        ::nn::spy::detail::StringData<uint8_t, 255> label;
    };

    PacketHeader header;
    uint32_t itemCount;
    uint32_t itemArea[ItemAreaSize / sizeof(uint32_t)];

    PlayerDataPacket()
        : header(PacketType_PlayerData)
    {
    }
};

struct AddonSoundArchiveDataPacket
{
    static const int MaxNameLength = 64 + 3;

    PacketHeader header;
    ::nn::spy::detail::IdData instanceId;
    ::nn::spy::detail::StringData<uint8_t, MaxNameLength> name;

    AddonSoundArchiveDataPacket()
        : header(PacketType_AddonSoundArchiveData)
    {
    }
};

} // namespace nn::spy::detail
} // namespace nn::spy::atk
} // namespace nn::spy
} // namespace nn

#endif // NN_BUILD_CONFIG_SPY_ENABLED
