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

#include <nn/util/util_IntrusiveList.h>
#include <nn/atk/atk_BasicSound.h>
#include <nn/atk/fnd/basis/atkfnd_Inlines.h>
#include <nn/atk/atk_Debug.h>

namespace nn {
namespace atk {
namespace detail {

/* ========================================================================
        SoundInstanceManager class
   ======================================================================== */

template < typename Sound >
class SoundInstanceManager
{
    /* ------------------------------------------------------------------------
            typename definition
       ------------------------------------------------------------------------ */
public:
    typedef util::IntrusiveList<Sound, util::IntrusiveListMemberNodeTraits<Sound,&Sound::m_PriorityLink>> PriorityList;
    typedef typename PriorityList::iterator Iterator;
    typedef typename PriorityList::const_iterator ConstIterator;

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    SoundInstanceManager() NN_NOEXCEPT : m_pBuffer(NULL), m_BufferSize(0)
    {
    }

    ~SoundInstanceManager() NN_NOEXCEPT
    {
    }

    static size_t GetObjectSize( const detail::SoundInstanceConfig& config ) NN_NOEXCEPT
    {
        size_t result = sizeof(Sound);
        const size_t AdditionalParamBufferSize = OutputAdditionalParam::GetRequiredMemSize( config );
        if(AdditionalParamBufferSize != 0)
        {
            // Sound 用 + SoundPlayer 用
            result += sizeof( OutputAdditionalParam ) * 2;
            result += AdditionalParamBufferSize * 2;
        }
        return result;
    }

    static size_t GetRequiredMemSize( int instanceCount, const detail::SoundInstanceConfig& config ) NN_NOEXCEPT
    {
        return GetObjectSize(config) * instanceCount;
    }

    /*--------------------------------------------------------------------------------*
      Name:         Create

      Description:  指定したメモリ領域をインスタンスマネージャに割り当てます

      Arguments:    buffer - メモリ領域の開始アドレス
                    size   - メモリ領域のサイズ

      Returns:      無し
     *--------------------------------------------------------------------------------*/
    int Create( void* buffer, size_t size, const detail::SoundInstanceConfig& config ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( buffer );

        const size_t TotalObjectSize = GetObjectSize(config);
        const int ObjectCount = static_cast<int>(size / TotalObjectSize);

        uint8_t* ptr = static_cast<uint8_t*>(buffer);
        uint8_t* soundPtr = ptr;
        uint8_t* additionalParamPtr = soundPtr + sizeof(Sound) * ObjectCount;

        for( auto i = 0; i < ObjectCount; i++ )
        {
            Sound* sound = new( soundPtr ) Sound( *this );
            m_FreeList.push_back( *sound );
            soundPtr += sizeof(Sound);
            ptr = soundPtr;

            const size_t AdditionalParamBufferSize = OutputAdditionalParam::GetRequiredMemSize( config );
            if(AdditionalParamBufferSize != 0)
            {
                const size_t AdditionalParamSize = sizeof(OutputAdditionalParam);

                OutputAdditionalParam* pAdditionalParam = reinterpret_cast<OutputAdditionalParam*>(additionalParamPtr);
                new(pAdditionalParam) OutputAdditionalParam();
                additionalParamPtr += AdditionalParamSize;
                void* pAdditionalParamBuffer = static_cast<void*>(additionalParamPtr);
                pAdditionalParam->Initialize(pAdditionalParamBuffer, AdditionalParamBufferSize, config);
                additionalParamPtr += AdditionalParamBufferSize;

                OutputAdditionalParam* pAdditionalParamForPlayer = reinterpret_cast<OutputAdditionalParam*>(additionalParamPtr);
                new(pAdditionalParamForPlayer) OutputAdditionalParam();
                additionalParamPtr += AdditionalParamSize;
                void* pAdditionalParamBufferForPlayer = static_cast<void*>(additionalParamPtr);
                pAdditionalParamForPlayer->Initialize(pAdditionalParamBufferForPlayer, AdditionalParamBufferSize, config);
                additionalParamPtr += AdditionalParamBufferSize;
                sound->SetOutputAdditionalParamAddr(OutputDevice_Main, pAdditionalParam, pAdditionalParamForPlayer);
                ptr = additionalParamPtr;
            }
        }

        m_pBuffer = buffer;
        m_BufferSize = size;
        m_SoundInstanceConfig = config;

        NN_SDK_ASSERT( ptr <= reinterpret_cast<uint8_t*>( buffer ) + size );
        return ObjectCount;
    }

    /*--------------------------------------------------------------------------------*
      Name:         Destroy

      Description:  指定したメモリ領域をインスタンスマネージャから解放します
                    解放するためにはメモリ領域が Free されている必要があります。

      Arguments:    buffer - メモリ領域の開始アドレス
                    size   - メモリ領域のサイズ

      Returns:      無し
     *--------------------------------------------------------------------------------*/
    void Destroy() NN_NOEXCEPT
    {
        NN_SDK_ASSERT( m_PriorityList.empty() );

        char* ptr = static_cast<char*>(m_pBuffer);
        const size_t TotalObjectSize = GetObjectSize(m_SoundInstanceConfig);
        const int objectCount = static_cast<int>(m_BufferSize / TotalObjectSize);
        for ( auto i = 0; i < objectCount; i++ )
        {
            Sound* sound = reinterpret_cast<Sound*>(ptr);
            sound->~Sound();
            ptr += sizeof(Sound);
        }

        m_FreeList.clear();
        m_PriorityList.clear();
    }

    /*--------------------------------------------------------------------------------*
      Name:         Alloc

      Description:  インスタンスを確保する
                    マネージャーからインスタンスを確保します。

      Arguments:    priority - プライオリティ
                    ambientPriority - アンビエントプライオリティ
                    pOutputReceiver - Voice の出力先

      Returns:      確保したインスタンス、確保できなければNULL
     *--------------------------------------------------------------------------------*/
    Sound* Alloc( int priority, int ambientPriority, OutputReceiver* pOutputReceiver ) NN_NOEXCEPT
    {
        int allocPriority = priority + ambientPriority;
        allocPriority = nn::atk::detail::fnd::Clamp( allocPriority, nn::atk::PlayerPriorityMin, nn::atk::PlayerPriorityMax );

        Sound* sound = NULL;
        while( sound == NULL )
        {
            if ( ! m_FreeList.empty() )
            {
                sound = &m_FreeList.front();
                m_FreeList.pop_front();
            }
            else
            {
                // get lowest priority sound
                Sound* lowPrioSound = GetLowestPrioritySound();
                if ( lowPrioSound == NULL ) return NULL;
                if ( allocPriority < lowPrioSound->CalcCurrentPlayerPriority() ) return NULL;

                if (detail::Debug_GetWarningFlag(detail::Debug_GetDebugWarningFlagFromSoundType(lowPrioSound->GetSoundType())))
                {
                    NN_ATK_WARNING(
                        "Sound (id:0x%08x) is stopped for not enough %s sound instance.",
                        lowPrioSound->GetId(),
                        detail::Debug_GetSoundTypeString( lowPrioSound->GetSoundType() )
                    );
                }
                lowPrioSound->Stop( 0 );
            }
        }

        bool result = sound->Initialize( pOutputReceiver );
        if( !result )
        {
            m_FreeList.push_back( *sound );
            return NULL;
        }

        sound->SetPriority( priority, ambientPriority );

        InsertPriorityList( sound, allocPriority );
        return sound;
    }

    /*--------------------------------------------------------------------------------*
      Name:         Free

      Description:  インスタンスをマネージャーに解放する
                    指定したインスタンスをマネージャーに対して解放します。
                    解放するインスタンスは、あらかじめ停止されている必要があります。

      Arguments:    sound - インスタンス

      Returns:      無し
     *--------------------------------------------------------------------------------*/
    void Free( Sound* sound ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_NOT_NULL( sound );

        RemovePriorityList( sound );

        sound->Finalize();
        m_FreeList.push_back( *sound );
    }

    /*--------------------------------------------------------------------------------*
      Name:         UpdatePriority

      Description:  引数に指定したサウンドのプライオリティをプライオリティリストに
                    反映させる

      Arguments:    sound - インスタンス
                    priority - プライオリティ

      Returns:      無し
     *--------------------------------------------------------------------------------*/
    void UpdatePriority( Sound* sound, int priority ) NN_NOEXCEPT
    {
        RemovePriorityList( sound );
        InsertPriorityList( sound, priority );
    }

    /*--------------------------------------------------------------------------------*
      Name:         SortPriorityList

      Description:  プライオリティリストをソートする

      Arguments:    無し

      Returns:      無し
     *--------------------------------------------------------------------------------*/
    void SortPriorityList() NN_NOEXCEPT
    {
        if ( m_PriorityList.size() < 2 ) return;

        static const int TmpCount =
            nn::atk::PlayerPriorityMax - nn::atk::PlayerPriorityMin + 1;
        PriorityList tmplist[ TmpCount ]; // notice: large stack

        while ( !m_PriorityList.empty() )
        {
            Sound& front = m_PriorityList.front();
            m_PriorityList.pop_front();
            tmplist[ front.CalcCurrentPlayerPriority() ].push_back( front );
        }
        for ( auto i = 0; i < TmpCount; i++ )
        {
            while ( !tmplist[i].empty() )
            {
                Sound& front = tmplist[i].front();
                tmplist[i].pop_front();
                m_PriorityList.push_back( front );
            }
        }
    }

    /*--------------------------------------------------------------------------------*
      Name:         GetLowestPrioritySound

      Description:  最もプライオリティの低いサウンドを取得する

      Arguments:    なし

      Returns:
     *--------------------------------------------------------------------------------*/
    Sound* GetLowestPrioritySound() NN_NOEXCEPT
    {
        if ( m_PriorityList.empty() ) return NULL;
        return &m_PriorityList.front();
    }

    /*--------------------------------------------------------------------------------*
      Name:         GetActiveCount

      Description:  現在アクティブなサウンド数を取得する

      Arguments:    なし

      Returns:      サウンド数
     *--------------------------------------------------------------------------------*/
    int GetActiveCount() const NN_NOEXCEPT
    {
        return m_PriorityList.size();
    }

    /*--------------------------------------------------------------------------------*
      Name:         GetFreeCount

      Description:  マネージャーからAlloc可能な残りインスタンス数を取得する

      Arguments:    無し

      Returns:      インスタンス数
     *--------------------------------------------------------------------------------*/
    int GetFreeCount() const NN_NOEXCEPT { return m_FreeList.size(); }

    const PriorityList& GetSoundList() const NN_NOEXCEPT { return m_PriorityList; }
    PriorityList& GetFreeList() NN_NOEXCEPT { return m_FreeList; }

private:
    void InsertPriorityList( Sound* sound, int priority ) NN_NOEXCEPT
    {
        // プライオリティリストへ追加
        Iterator itr = m_PriorityList.begin();
        while ( itr != m_PriorityList.end() )
        {
            if ( priority < itr->CalcCurrentPlayerPriority() ) break;
            (void)++itr;
        }
        m_PriorityList.insert( itr, *sound );
    }
    void RemovePriorityList( Sound* sound ) NN_NOEXCEPT { m_PriorityList.erase( m_PriorityList.iterator_to( *sound ) ); }

    void* m_pBuffer;
    size_t m_BufferSize;
    SoundInstanceConfig m_SoundInstanceConfig;

    // NOTE: m_PriorityList と m_FreeList はどちらも、Sound::m_PriorityLink
    //       を管理するが、同時に 2 つのリストの管理下に置かれることは無いので、
    //       問題ない
    PriorityList m_PriorityList;
    PriorityList m_FreeList;
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

