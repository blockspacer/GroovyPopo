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

#include <nn/os/os_Tick.h>
#include <nn/nn_Macro.h>
#include <nn/util/util_IntrusiveList.h>
#include <nn/atk/atk_ProfileReader.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>
#include <nn/atk/detail/atk_IStreamDataDecoder.h>

namespace nn {
namespace atk {

//! @briefprivate
//! タスクプロファイル情報を表す構造体です。
//! @ref nn::atk::TaskProfile::type でプロファイルの種類を識別し、
//! 種類に応じたメンバにアクセスして情報を取得してください。
struct TaskProfile
{
    //! ストリームサウンドのブロックを 1 つ読んだときのプロファイル
    struct LoadStreamBlock{
    public:
        //! タスク全体にかかった時間
        nn::TimeSpan GetTotalTime() const NN_NOEXCEPT;
        //! タスクを開始したときの Tick
        nn::os::Tick GetBeginTick() const NN_NOEXCEPT;
        //! タスクが終了したときの Tick
        nn::os::Tick GetEndTick() const NN_NOEXCEPT;

        //! 現在ロード中のキャッシュの未使用部分の割合
        float GetRemainingCachePercentage() const NN_NOEXCEPT;
        //! 現在ロード中のキャッシュのサイズ
        size_t GetCachedLength() const NN_NOEXCEPT;

        //! ローダーが参照しているプレイヤーのアドレス
        nn::atk::detail::driver::StreamSoundPlayer* GetStreamSoundPlayer() const NN_NOEXCEPT;

        //! @internal
        void SetData(const nn::os::Tick& beginTick, const nn::os::Tick& endTick, const detail::IStreamDataDecoder::CacheProfile& cacheProfile) NN_NOEXCEPT;

    private:
        uint64_t m_BeginTick;
        uint64_t m_EndTick;
        detail::position_t  m_CacheStartPosition;
        std::size_t         m_CachedLength;
        detail::position_t  m_CacheCurrentPosition;
        nn::atk::detail::driver::StreamSoundPlayer* m_pPlayer;
    };
    //! Opus 形式のストリームサウンドのブロックを 1 つ読んだときのプロファイル
    struct LoadOpusStreamBlock{
    public:
        //! タスク全体にかかった時間
        nn::TimeSpan GetTotalTime() const NN_NOEXCEPT;
        //! タスクを開始したときの Tick
        nn::os::Tick GetBeginTick() const NN_NOEXCEPT;
        //! タスクが終了したときの Tick
        nn::os::Tick GetEndTick() const NN_NOEXCEPT;

        //! 現在ロード中のキャッシュの未使用部分の割合
        float GetRemainingCachePercentage() const NN_NOEXCEPT;
        //! 現在ロード中のキャッシュのサイズ
        size_t GetCachedLength() const NN_NOEXCEPT;

        //! デコードにかかった時間
        nn::TimeSpan GetDecodeTime() const NN_NOEXCEPT;
        //! デコードしたサンプルの数
        int GetDecodedSampleCount() const NN_NOEXCEPT;
        //! fs アクセスにかかった時間
        nn::TimeSpan GetFsAccessTime() const NN_NOEXCEPT;
        //! fs で読み込んだバッファサイズ
        std::size_t GetFsReadSize() const NN_NOEXCEPT;

        //! ローダーが参照しているプレイヤーのアドレス
        nn::atk::detail::driver::StreamSoundPlayer* GetStreamSoundPlayer() const NN_NOEXCEPT;

        //! @internal
        void SetData(const nn::os::Tick& beginTick, const nn::os::Tick& endTick, const detail::IStreamDataDecoder::DecodeProfile& decodeProfile, const detail::IStreamDataDecoder::CacheProfile& cacheProfile) NN_NOEXCEPT;

    private:
        uint64_t    m_BeginTick;
        uint64_t    m_EndTick;
        detail::position_t  m_CacheStartPosition;
        std::size_t         m_CachedLength;
        detail::position_t  m_CacheCurrentPosition;
        uint64_t    m_DecodeTick;
        uint64_t    m_FsAccessTick;
        std::size_t m_FsReadSize;
        int         m_DecodedSampleCount;
        nn::atk::detail::driver::StreamSoundPlayer* m_pPlayer;
    };

    //! プロファイルの種類です。
    enum TaskProfileType
    {
        //! ストリームサウンドのブロックを 1 つ読んだときのプロファイル
        TaskProfileType_LoadStreamBlock,
        //! Opus 形式のストリームサウンドのブロックを 1 つ読んだときのプロファイル
        TaskProfileType_LoadOpusStreamBlock,
    };

    //! プロファイルの種類を識別する変数です。
    TaskProfileType type;

    union
    {
        LoadStreamBlock loadStreamBlock;
        LoadOpusStreamBlock loadOpusStreamBlock;
    };
};

typedef AtkProfileReader<TaskProfile> TaskProfileReader;

class TaskProfileLogger
{
public:
    TaskProfileLogger() NN_NOEXCEPT;
    void Record(const TaskProfile& profile) NN_NOEXCEPT;

    void RegisterReader(TaskProfileReader& reader) NN_NOEXCEPT;
    void UnregisterReader(const TaskProfileReader& reader) NN_NOEXCEPT;

    void SetProfilingEnabled(bool isProfilingEnabled) NN_NOEXCEPT;
    bool IsProfilingEnabled() const NN_NOEXCEPT
    {
        return m_IsProfilingEnabled;
    }

    void Finalize() NN_NOEXCEPT;

private:
    typedef util::IntrusiveList<TaskProfileReader, util::IntrusiveListMemberNodeTraits<TaskProfileReader, &TaskProfileReader::m_List> > TaskProfileReaderList;
    TaskProfileReaderList m_List;
    nn::atk::detail::fnd::CriticalSection m_Lock;
    bool m_IsProfilingEnabled;
};

} // namespace nn::atk
} // namespace nn
