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
#include <nn/util/util_BytePtr.h>
#include <nn/atk/atk_ProfileReader.h>

namespace nn {
namespace atk {
namespace detail {
    //! @internal
    class ThreadInfoRecorder
    {
    public:
        static const size_t BufferMinimumSize;

        //! @internal
        struct FrameFooter
        {
            nn::os::Tick recordTick;
            bool isInfoDropped;
        };

        //! @internal
        //! コンストラクタです。
        ThreadInfoRecorder() NN_NOEXCEPT;

        //---------------------------------------------------------------------------
        //! @internal
        //! スレッドの情報レコーダーを初期化します。
        //!
        //! 初期化には @ref BufferMinimumSize 以上のサイズのバッファが必要です。
        //! 情報の取得には各スレッド向けに用意された取得用クラスを利用してください。
        //!
        //! @param[in] buffer 情報を保存するバッファです。
        //! @param[in] bufferSize buffer のサイズです。
        //!
        //! @pre
        //! - buffer != NULL
        //! - bufferSize >= @ref BufferMinimumSize
        //---------------------------------------------------------------------------
        void Initialize(void* buffer, size_t bufferSize) NN_NOEXCEPT;

        //! @internal
        int GetRecordedFrameCount() const NN_NOEXCEPT;

        //! @internal
        const void* GetRingBufferHeadAddress() const NN_NOEXCEPT;

        //! @internal
        void MoveToNextFrame() NN_NOEXCEPT;

        //! @internal
        template <typename InfoType> void Record(uint32_t id, const InfoType& info) NN_NOEXCEPT
        {
            void* buffer = AllocateInfoBuffer( id, sizeof(InfoType) );
            if( buffer != nullptr )
            {
                *reinterpret_cast<InfoType*>( buffer ) = info;
            }
        }

        //! @internal
        void RecordFooter() NN_NOEXCEPT;

    public:
        //! @internal
        util::IntrusiveListNode m_List;

    private:
        void* AllocateInfoBuffer(uint32_t id, size_t size) NN_NOEXCEPT;
        void* AllocateBuffer(uint32_t infoId, size_t infoDataSize, size_t reservedSize) NN_NOEXCEPT;

        void*  m_Buffer;
        size_t m_BufferSize;
        size_t m_WritePosition;
        std::atomic<size_t> m_ReadPosition;
        std::atomic<int>    m_RecordFrameCount;
        std::atomic<bool>   m_IsAllocationFailed;
    };

    //! @internal
    enum SoundThreadInfoId
    {
        SoundThreadInfoId_SoundProfile,
        SoundThreadInfoId_SoundThreadUpdateProfile,
        SoundThreadInfoId_Count,
    };
} // namespace nn::atk::detail

//---------------------------------------------------------------------------
//! @internal
//! サウンドスレッドの情報を記録するためのクラスです。
//---------------------------------------------------------------------------
typedef detail::ThreadInfoRecorder SoundThreadInfoRecorder;

//! @internal
typedef util::IntrusiveList< SoundThreadInfoRecorder, util::IntrusiveListMemberNodeTraits<SoundThreadInfoRecorder, &SoundThreadInfoRecorder::m_List> > SoundThreadInfoRecorderList;

//---------------------------------------------------------------------------
//! @internal
//! サウンドスレッドの情報をフレームごとに取得するためのクラスです。
//---------------------------------------------------------------------------
class SoundThreadInfoReader
{
public:
    //  フレーム全体の情報
    struct FrameGlobalInfo
    {
        //  フレーム情報を記録し終えたときの Tick
        nn::os::Tick recordTick;
        //  バッファ不足のために情報を取りこぼしたか
        bool isInfoDropped;
    };

    //! @internal
    //! レコーダから情報を読み取るように SoundThreadInfoReader を初期化します。
    NN_IMPLICIT SoundThreadInfoReader(SoundThreadInfoRecorder& recorder) NN_NOEXCEPT;

    //! @internal
    //! 設定したレコーダから 1 フレーム分の情報を読み取ることができるかどうかを取得します。
    //! この関数が true を返すとき、@ref SetupFrameInfo を呼び出してから @ref ReadSoundProfile などの取得関数を呼ぶことで情報を読み取ることができます。
    //! 1 フレーム分の情報を読み取った後は、@ref MoveToNextFrame を呼ぶことで次のフレーム情報を読むことができるようになります。
    //!
    //! @return 1 フレーム分の情報を読み取ることができるか
    //! @see SetupFrameInfo
    //! @see MoveToNextFrame
    //! @see ReadFrameGlobalInfo
    //! @see ReadSoundProfile
    //! @see ReadSoundThreadUpdateProfile
    bool HasFrameInfo() const NN_NOEXCEPT;

    //! @internal
    //! 1 フレーム分の情報を読み取る準備をします。
    //! @ref ReadSoundProfile などの情報を読み取る関数は、本関数を呼び出してから使用するようにしてください。
    //!
    //! @see HasFrameInfo
    //! @see ReadFrameGlobalInfo
    //! @see ReadSoundProfile
    //! @see ReadSoundThreadUpdateProfile
    void SetupFrameInfo() NN_NOEXCEPT;

    //! @internal
    //! 設定したレコーダから読み取る情報を 1 フレーム分進めます。
    //! この関数の呼び出し後は @ref HasFrameInfo でフレーム情報を読み取ることができることを確認してください。
    //!
    //! @see HasFrameInfo
    void MoveToNextFrame() NN_NOEXCEPT;

    //! @internal
    //! フレーム全体の情報を読み取ります。
    //! 情報の読み取りに成功した場合は true を返し、情報の読み取りに失敗した場合は false を返します。
    //!
    //! フレーム全体の情報はフレームごとに 1 つしかないため、
    //! 本関数の呼び出しは 1 フレーム情報につき 1 回で十分です。
    //!
    //! @return 情報を読み取れた場合は true を返します。
    bool ReadFrameGlobalInfo(FrameGlobalInfo* pInfo) NN_NOEXCEPT;

    //! @internal
    //! @ref SoundProfile を 1 つ分読み取ります。
    //! 情報の読み取りに成功した場合は true を返し、情報の読み取りに失敗した場合は false を返します。
    //!
    //! フレーム情報に対して、本関数を n 回呼んだ時に読み取られる @ref SoundProfile は、
    //! そのフレームの n コ目の @ref SoundProfile です。
    //! サウンドスレッド 1 フレームにつき複数個の @ref SoundProfile が記録されるような実行環境の場合は、
    //! 本関数の返り値を確認しながらループを回すことで取りこぼすことなく情報を取得することができます。
    //!
    //! @return 情報を読み取れた場合は true を返します。
    bool ReadSoundProfile(SoundProfile* pProfile) NN_NOEXCEPT;

    //! @internal
    //! @ref SoundThreadUpdateProfile を 1 つ分読み取ります。
    //! 情報の読み取りに成功した場合は true を返し、情報の読み取りに失敗した場合は false を返します。
    //!
    //! フレーム情報に対して、本関数を n 回呼んだ時に読み取られる @ref SoundThreadUpdateProfile は、
    //! そのフレームの n コ目の @ref SoundThreadUpdateProfile です。
    //! サウンドスレッド 1 フレームにつき複数個の @ref SoundThreadUpdateProfile が記録されるような実行環境の場合は、
    //! 本関数の返り値を確認しながらループを回すことで取りこぼすことなく情報を取得することができます。
    //!
    //! @return 情報を読み取れた場合は true を返します。
    bool ReadSoundThreadUpdateProfile(SoundThreadUpdateProfile* pProfile) NN_NOEXCEPT;

private:
    void ResetInfo() NN_NOEXCEPT;

    SoundThreadInfoRecorder& m_Recorder;
    const void* m_pCurrentInfo[detail::SoundThreadInfoId_Count];
    const void* m_pFooterInfo;
};


} // namespace nn::atk
} // namespace nn
