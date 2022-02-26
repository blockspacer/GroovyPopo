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

#include <nn/atk/detail/atk_IRegionInfoReadable.h>

namespace nn { namespace atk { namespace detail {
    struct StreamDataInfoDetail;
}}}


namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @brief ストリームジャンプ機能において使用するコールバックの返り値の定義です。
//!
//! @see StreamRegionCallback
//---------------------------------------------------------------------------
enum StreamRegionCallbackResult {
    StreamRegionCallbackResult_Finish,  //!< サウンドを終了することを示します
    StreamRegionCallbackResult_Continue //!< 次のリージョンにジャンプすることを示します
};

//---------------------------------------------------------------------------
//! @brief ストリームジャンプ機能において使用するコールバックの入出力に使用するパラメータです。
//!
//!        コールバックの引数として渡され、その内容を参考にした上で次のリージョンの情報に書き換える形で使用します。
//!
//! @see StreamRegionCallback
//---------------------------------------------------------------------------
struct StreamRegionCallbackParam {
    //! @brief リージョン番号です。
    //!
    //!        前回のコールバックで指定したリージョン番号が格納されています。
    //!        一度も指定していない場合は 0 がセットされています。
    //!        コールバック関数で、次に再生するリージョン番号を書き込んでください。
    //!        リージョン番号とリージョン名を同時に使用した場合には、リージョン名の処理が優先されます。
    //!
    //!        リージョン番号に負の数もしくは @ref regionCount 以上の値を指定するとアサートになります。
    int regionNo;

    //! @brief リージョン名です。
    //!
    //!        前回のコールバックで指定したリージョン名が格納されています。
    //!        一度も指定していない場合や、前回のコールバックで名前指定をしなかった場合は
    //!        空文字列がセットされています。
    //!
    //!        次に再生するリージョン名を指定することで、該当するリージョンを検索し、
    //!        見つかった場合にはそのリージョンに移動します。
    //!        見つからなかった場合や isRegionNameEnabled に false を指定した場合は、 regionNo の値が使用されます。
    //!        リージョン番号とリージョン名を同時に使用した場合には、リージョン名の処理が優先されます。
    char regionName[RegionNameLengthMax + 1];

    //! @brief リージョン名によるリージョン指定を有効にするかのフラグです。
    //!
    //!        この値に true を設定するとリージョン名を使ったリージョン指定が有効になります。
    //!        この値には、前回のコールバックで指定した値が格納されています。
    //!        一度も指定されていない場合は、false が設定されています。
    bool isRegionNameEnabled;

    //! @brief リージョンの個数です。
    //!
    //!        regionNo に regionCount 以上の数を指定するとアサートになります。
    int regionCount;

    //! @internal
    //! @brief リージョン情報を読み込むためのリーダーです。
    detail::IRegionInfoReadable* pRegionInfoReader;
};

//---------------------------------------------------------------------------
//! @brief ストリームジャンプ機能において使用するコールバック関数の型定義です。
//!
//!        このコールバックは、ストリームサウンドのヘッダ情報のメモリロード完了のタイミングと
//!        波形に割り当てられたリージョンの終端を含むブロックのメモリロード完了のタイミングにおいて、タスクスレッドから呼び出されます。
//!        再生完了のタイミングではなく各データのメモリロード完了のタイミングに呼び出されるという点にご注意ください。
//!
//!        また、コールバックについてはプログラミングガイドのストリームサウンドのページ、タスクスレッドについてはスレッドのページもご確認ください。
//!
//! @param[in,out] param    前のリージョン情報が格納されており、
//!                         次のリージョン情報を格納するためのパラメータです。
//! @param[in] arg          コールバック関数のユーザー引数です。
//!
//! @return サウンドを終了するか、次のリージョンにジャンプするかを返します。
//!         次のリージョンにジャンプする場合は StreamRegionCallbackResult_Continue を返し、
//!         現在のリージョンで終了する場合は StreamRegionCallbackResult_Finish
//!         を返すように実装してください。
//!
//! @see StreamRegionCallbackParam
//! @see StreamRegionCallbackResult
//! @see SoundStartable::StartInfo
//---------------------------------------------------------------------------
typedef StreamRegionCallbackResult( *StreamRegionCallback )(
    StreamRegionCallbackParam* param, void* arg);

namespace detail {

class RegionManager
{
public:
    struct Region
    {
        Region() NN_NOEXCEPT
            : current( 0 )
            , begin( 0 )
            , end( 0 )
        {
        }

        bool IsIn( position_t value ) const NN_NOEXCEPT {
            return current + value < end;
        }
        bool IsInWithBorder( position_t value ) const NN_NOEXCEPT {
            return current + value <= end;
        }
        bool IsEnd() const NN_NOEXCEPT {
            return current == end;
        }
        size_t Rest() const NN_NOEXCEPT {
            return end - current;
        }

        position_t current;
        position_t begin;
        position_t end;
        bool isEnabled;
    };

public:
    void Initialize() NN_NOEXCEPT;

    bool InitializeRegion( IRegionInfoReadable* pRegionReader, StreamDataInfoDetail* pStreamDataInfo ) NN_NOEXCEPT;
    bool ChangeRegion( int currentRegionNo, IRegionInfoReadable* pRegionReader, StreamDataInfoDetail* pStreamDataInfo ) NN_NOEXCEPT;
    bool TryMoveNextRegion( IRegionInfoReadable* pRegionReader, StreamDataInfoDetail* pStreamDataInfo ) NN_NOEXCEPT;

    void SetPosition( position_t position ) NN_NOEXCEPT;
    void AddPosition( position_t position ) NN_NOEXCEPT;

    const Region& GetCurrentRegion() const NN_NOEXCEPT
    {
        return m_CurrentRegion;
    }

    void SetStartOffsetFrame( position_t startOffsetFrame ) NN_NOEXCEPT
    {
        m_AdpcmContextForStartOffsetFrame = startOffsetFrame;
    }

    position_t GetStartOffsetFrame() const NN_NOEXCEPT
    {
        return m_AdpcmContextForStartOffsetFrame;
    }

    AdpcmContext& GetAdpcmContextForStartOffset( int channelIndex ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_MINMAX( channelIndex, 0, StreamChannelCount );

        return m_AdpcmContextForStartOffset[channelIndex];
    }

    const AdpcmContext& GetAdpcmContextForStartOffset( int channelIndex ) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_MINMAX(channelIndex, 0, StreamChannelCount);

        return m_AdpcmContextForStartOffset[channelIndex];
    }

    bool IsInFirstRegion() const NN_NOEXCEPT;

    void SetRegionCallback( const StreamRegionCallback& function, void* argument ) NN_NOEXCEPT
    {
        m_StreamRegionCallbackFunc = function;
        m_StreamRegionCallbackArg = argument;
    }

private:
    bool IsPreparedForRegionJump() const NN_NOEXCEPT;
    void SetRegionInfo( const StreamSoundFile::RegionInfo* pRegionInfo, const StreamDataInfoDetail* pStreamDataInfo ) NN_NOEXCEPT;

private:
    bool m_IsRegionInfoEnabled;
    bool m_IsRegionIndexCheckEnabled;
    bool m_IsRegionInitialized;
    bool m_IsCurrentRegionNameEnabled;

    StreamRegionCallback m_StreamRegionCallbackFunc;
    void* m_StreamRegionCallbackArg;

    int m_CurrentRegionNo;
    const char* m_pCurrentRegionName;
    Region m_CurrentRegion;
    position_t m_AdpcmContextForStartOffsetFrame;
    AdpcmContext m_AdpcmContextForStartOffset[StreamChannelCount];
    char m_CurrentRegionName[RegionNameLengthMax + 1];
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

