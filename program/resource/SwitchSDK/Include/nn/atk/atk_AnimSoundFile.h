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

#include <nn/atk/atk_Util.h>
#include <nn/atk/atk_ElementType.h>

namespace nn {
namespace atk {

class SoundArchive;

namespace detail {

/*
    アニメーションサウンドファイル (.bcasd) の構造

    bcasd
     |
     +-- FileHeader
     +-- DataBlock
          |
          +-- BinaryBlockHeader
          +-- DataBlockBody
               |
               +-- uint32_t frameSize
               +-- Reference toAnimEventTable
                    |
                    +--- ( &DataBlockBody + toAnimEventTable.offset ) --+
                                                                        |
     +------------------------------------------------------------------+
     |
     +--> AnimEventTable
           |
           +-- uint32_t count
           +-- AnimEvent item[count]
                |
                +-- FrameInfo
                |    |
                |    +-- int32_t startFrame
                |    +-- int32_t endFrame
                |    +-- uint8_t frameFlag
                |    +-- int8_t loopOffset
                |    +-- uint8_t loopInterval
                |    +-- uint8_t reserved
                |
                +-- Reference toEventInfo
                     |
                     +-- ( &item[...] + toEventInfo.offset ) --+
                                                               |
     +---------------------------------------------------------+
     |
     +--> EventInfo
           |
           +-- uint32_t optionFlag
           +-- uint32_t placeForSoundId
           +-- Reference toSoundLabel
           |    |
           |    +-- ( &EventInfo + toSoundLabel.offset ) --+
           |                                                  |
           |    +---------------------------------------------+
           |    |
           |    +--> const char* soundLabel
           |
           +-- uint8_t volume
           +-- uint8_t playDirection
           +-- uint8_t sequenceVariableNo
           +-- uint8_t reserved1
           +-- float pitch
           +-- uint32_t reserved2
           +-- uint32_t userParam
*/


struct AnimSoundFile
{
    // 前方宣言
    struct DataBlock;
    struct AnimEvent;
    struct AnimEventFrameInfo;

    struct FileHeader : public Util::SoundFileHeader
    {
        const DataBlock* GetDataBlock() const NN_NOEXCEPT;
    };

    // フレーム情報。開始・終了位置やループについての情報を保持。
    struct FrameInfo
    {
        // フレーム処理のオプションフラグ
        enum FrameFlag
        {
            FrameFlag_TriggerEvent  = 0x01, // トリガタイプのイベント
            FrameFlag_EndFrameInf   = 0x02, // 終了フレームが無限大
            FrameFlag_StartFrameInf = 0x04  // 開始フレームが負無限大
        };

        // データ
        int32_t startFrame;      // イベント開始フレーム
        int32_t endFrame;        // イベント終了フレーム (* の場合は 0xffffffff)
        uint8_t  frameFlag;       // 参照: AnimSoundImpl::FrameFlag
        int8_t  loopOffset;      // 再生ループ数
        uint8_t  loopInterval;    // loopCount 以降で再生するループ間隔
        uint8_t reserved;
    };

    // イベント情報。どのサウンドを、どのようなピッチで鳴らすかなどの情報を保持。
    struct EventInfo
    {
        // オプションフラグタイプ
        enum OptionFlag
        {
            OptionFlag_IsNotStopSoundWhenAnimationFinish = (1<<0),
            OptionFlag_IsEnableSequenceVariable          = (1<<1)
        };

        // 再生方向
        enum PlayDirection
        {
            PlayDirection_Both     = 0,    // 両方向
            PlayDirection_Forward  = 1,    // 順方向
            PlayDirection_Backward = 2     // 逆方向
        };

        // データ
        uint32_t optionFlag;          // 参照: OptionFlag
        uint32_t placeForSoundId;     // バイナリには SoundArchive::InvalidId
                                            // (0xffffffff) が書かれるが、WriteSoundId にて
                                            // 有効な値が書かれる。プレースホルダ。
        Util::Reference toSoundLabel;

        uint8_t volume;
        uint8_t playDirection;        // 参照: PlayDirection
        uint8_t sequenceVariableNo;   // 0-15:ローカル変数、16-31:グローバル変数
        uint8_t reserved1;

        float pitch;
        uint32_t reserved2;
        uint32_t userParam;

        // アクセサ
        const char* GetSoundLabel() const NN_NOEXCEPT
        {
            NN_SDK_ASSERT( toSoundLabel.typeId ==
                    nn::atk::detail::ElementType_General_String );
            return reinterpret_cast<const char*>(
                    util::ConstBytePtr( this, toSoundLabel.offset ).Get() );
        }
        bool IsNotStopSoundWhenAnimationFinish() const NN_NOEXCEPT
        {
            if ( optionFlag & OptionFlag_IsNotStopSoundWhenAnimationFinish )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        bool GetSequenceVariable( uint8_t* out ) const NN_NOEXCEPT
        {
            if ( optionFlag & OptionFlag_IsEnableSequenceVariable )
            {
                *out = sequenceVariableNo;
                return true;
            }
            else
            {
                return false;
            }
        }
    };

    // フレーム情報と、イベント情報 (への参照) をまとめたコンテナ
    struct AnimEvent
    {
        FrameInfo frameInfo;        // TODO: 将来的には Reference にしておいたほうが無難そう
        Util::Reference toEventInfo;

        const EventInfo* GetEventInfo() const NN_NOEXCEPT
        {
            // シグニチャチェック
            NN_SDK_ASSERT( toEventInfo.typeId ==
                    nn::atk::detail::ElementType_AnimSoundFile_EventInfo );
            return reinterpret_cast<const EventInfo*>(
                    util::ConstBytePtr( this, toEventInfo.offset ).Get() );
        }
    };

    typedef Util::Table<AnimEvent> AnimEventTable;

    // DATA ブロックのボディ (ブロックヘッダーを抜いた部分)
    struct DataBlockBody
    {
        uint32_t frameSize;               // 総フレーム数
        Util::Reference toAnimEventTable;  // AnimEventTable へのオフセット

        const AnimEventTable* GetAnimEventTable() const NN_NOEXCEPT
        {
            NN_SDK_ASSERT( toAnimEventTable.typeId ==
                    nn::atk::detail::ElementType_Table_EmbeddingTable );
            return reinterpret_cast<const AnimEventTable*>(
                    util::ConstBytePtr( this, toAnimEventTable.offset ).Get() );
        }
    };

    struct DataBlock
    {
        BinaryBlockHeader header;
        DataBlockBody body;
    };
};


// void WriteSoundId( void* animSoundData, const SoundArchive& arc );


} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

