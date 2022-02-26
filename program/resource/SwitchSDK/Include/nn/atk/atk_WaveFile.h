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
#include <nn/atk/atk_Util.h>
#include <nn/atk/atk_BinaryFileFormat.h>

namespace nn {
namespace atk {
namespace detail {

/*
    波形ファイル (.bfwav) の構造

    bfwav
     |
     +-- FileHeader
     +-- InfoBlock
     |    |
     |    +-- BinaryBlockHeader
     |    +-- InfoBlockBody
     |         |
     |         +-- uint8_t encoding
     |         +-- uint8_t isLoop
     |         +-- uint32_t sampleRate
     |         +-- uint32_t loopStart
     |         +-- uint32_t loopEnd
     |         +-- uint32_t originalLoopStart
     |         +-- Table< reference to ChannelInfo >
     |              |
     |              +-- int32_t count
     |              +-- reference to ChannelInfo[0] ----------------+
     |              +-- reference to ChannelInfo[1]                 |
     |              +-- ... (2ch までしかサポートしていない？！)    |
     |                                                              |
     |    +---------------------------------------------------------+
     |    |
     |    +-> ChannelInfo
     |         |
     |         +-- reference to Samples (DataBlockBody を起点とするサンプルへのオフセット)
     |         +-- reference to AdpcmInfo --+
     |                                      |
     |    +---------------------------------+
     |    |
     |    +-> AdpcmInfo
     |         |
     |         +-- DspAdpcmParam (snd_Global.h)
     |         |    |
     |         |    +-- uint16_t coef[16]
     |         |    +-- uint16_t predScale
     |         |    +-- uint16_t yn1
     |         |    +-- uint16_t yn2
     |         |
     |         +-- DspAdpcmLoopParam (snd_Global.h)
     |              |
     |              +-- uint16_t loopPredScale
     |              +-- uint16_t loopYn1
     |              +-- uint16_t loopYn2
     |
     +-- DataBlock
          |
          +-- BinaryBlockHeader
          +-- DataBlockBody (サンプルデータ)
*/

struct WaveFile
{
    enum EncodeMethod
    {
        EncodeMethod_Pcm8        = 0,
        EncodeMethod_Pcm16,              // バイトオーダーはプラットフォームに依存
        EncodeMethod_DspAdpcm,
        EncodeMethod_ImaAdpcm
    };

    //
    // 前方宣言
    //
    struct InfoBlock;
    struct ChannelInfo;
    struct DspAdpcmInfo;
    struct DataBlock;

    struct FileHeader : public Util::SoundFileHeader
    {
        const InfoBlock* GetInfoBlock() const NN_NOEXCEPT;
        const DataBlock* GetDataBlock() const NN_NOEXCEPT;
    };

    struct InfoBlockBody
    {
        uint8_t   encoding;           // EncodeMethod が入る
        uint8_t   isLoop;             // 0: ループなし, 1: ループあり
        uint8_t   padding[2];
        uint32_t  sampleRate;         // サンプルレート
        uint32_t  loopStartFrame;     // ループ開始 (ループなしなら 0)
        uint32_t  loopEndFrame;       // ループ終端 (ループなしなら 0)
        uint32_t  originalLoopStartFrame;
        Util::ReferenceTable channelInfoReferenceTable;

        inline int GetChannelCount() const NN_NOEXCEPT { return channelInfoReferenceTable.count; }
        const ChannelInfo& GetChannelInfo( int channelIndex ) const NN_NOEXCEPT;
    };

    struct InfoBlock
    {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };

    struct ChannelInfo
    {
        Util::Reference     referToSamples;     // 起点は DATA ブロックボディ
        Util::Reference     referToAdpcmInfo;
        uint32_t      reserved;

        const void* GetSamplesAddress( const void* dataBlockBodyAddress ) const NN_NOEXCEPT;
        const DspAdpcmInfo& GetDspAdpcmInfo() const NN_NOEXCEPT;
    };

    struct DspAdpcmInfo
    {
        DspAdpcmParam      adpcmParam;
        DspAdpcmLoopParam  adpcmLoopParam;
    };

    struct DataBlock
    {
        BinaryBlockHeader header;
        union
        {
            int8_t  pcm8[1];
            int16_t pcm16[1];
            uint8_t  byte[1];
        };
    };
};  // struct WaveFile

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
