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

#include <nn/util/util_BytePtr.h>
#include <nn/atk/atk_BinaryFileFormat.h>
#include <nn/atk/atk_ItemType.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_Util.h>
#include <nn/atk/atk_Config.h>

#define NN_ATK_ROUND_UP_32B(x) (((uint32_t)(x) + 32 - 1) & ~(32 - 1))

namespace nn {
namespace atk {

class SoundArchive;
class SoundArchivePlayer;

namespace detail {

class SoundArchiveLoader;
class PlayerHeapDataManager;
struct LoadItemInfo;

class Util
{
public:
    static const int VolumeDbMin = -904; // -90.4dB = -inf
    static const int VolumeDbMax = 60;   // + 6.0dB

    static const int PitchDivisionBit   = 8;                      // 半音分解能.（ビット数）
    static const int PitchDivisionRange = 1 << PitchDivisionBit;  // 半音分解能.

    // ------------------------------------------------------------------------
    //  パン設定
    // static const int PanCurve_Count = 3;
    enum PanCurve
    {
        PanCurve_Sqrt,
        PanCurve_Sincos,
        PanCurve_Linear
    };
    struct PanInfo
    {
        PanCurve curve;
        bool centerZeroFlag;    // 中央で0dBにするかどうか
        bool zeroClampFlag;     // 0dBを超えたときにClampするかどうか
        bool isEnableFrontBypass;
        bool isTransaural; // トランスオーラル再生かどうか

        PanInfo() NN_NOEXCEPT
        : curve( PanCurve_Sqrt ),
          centerZeroFlag( false ),
          zeroClampFlag( false ),
          isEnableFrontBypass( false ),
          isTransaural( false ) {}
    };


    static uint16_t CalcLpfFreq( float scale ) NN_NOEXCEPT;
    static nn::atk::BiquadFilterCoefficients CalcLowPassFilterCoefficients(int frequency, int sampleRate, bool isTableUsed) NN_NOEXCEPT;
    static int FindLpfFreqTableIndex(int frequency) NN_NOEXCEPT;
    static float CalcPanRatio( float pan, const PanInfo& info, OutputMode mode ) NN_NOEXCEPT;
    static float CalcSurroundPanRatio( float surroundPan, const PanInfo& info ) NN_NOEXCEPT;

    static float CalcPitchRatio( int pitch ) NN_NOEXCEPT;
    static float CalcVolumeRatio( float dB ) NN_NOEXCEPT;
    static uint16_t CalcRandom() NN_NOEXCEPT;

#if 0
    // デバッグ用 (3D サラウンドモード時の音割れ調整)
    static float SetPanCurveMax( float max ) NN_NOEXCEPT;   // 設定後の現在値を返す
    static float GetPanCurveMax() NN_NOEXCEPT;
    static float GetPanCurveMin() NN_NOEXCEPT;
#endif

    static size_t GetSampleByByte( size_t byte, SampleFormat format ) NN_NOEXCEPT;
    static size_t GetByteBySample( size_t sample, SampleFormat format ) NN_NOEXCEPT;

    static bool IsValidMemoryForDsp(const void* ptr, size_t size) NN_NOEXCEPT;

private:
    // CalcLpfFreq Table
    static const int CalcLpfFreqTableSize   = 24;
    static const float CalcLpfFreqIntercept;
    static const float CalcLpfFreqThreshold;
    static const uint16_t CalcLpfFreqTable[ CalcLpfFreqTableSize ];
    static const BiquadFilterCoefficients LowPassFilterCoefficientsTable32000[CalcLpfFreqTableSize];
    static const BiquadFilterCoefficients LowPassFilterCoefficientsTable48000[CalcLpfFreqTableSize];

public:
    template< typename ItemType, typename CountType = uint32_t >
    struct Table
    {
        CountType count;
        ItemType item[ 1 ];
    };

    struct Reference
    {
        uint16_t  typeId;     // snd_ElementType.h で定義されているタイプ
        uint8_t  padding[2];
        int32_t  offset;     // INVALID_OFFSET が入っていたら 無効値

        static const int InvalidOffset = -1;

        inline bool IsValidTypeId( uint16_t validId ) const NN_NOEXCEPT
        {
            if ( validId == typeId )
            {
                return true;
            }
            return false;
        }
        inline bool IsValidOffset() const NN_NOEXCEPT
        {
            if ( offset != InvalidOffset )
            {
                return true;
            }
            return false;
        }
    };

    struct ReferenceWithSize : public Reference
    {
        uint32_t  size;
    };

    struct ReferenceTable : public Table<Reference>
    {
        const void* GetReferedItem( uint32_t index ) const NN_NOEXCEPT
        {
            if ( index >= count ) return NULL;
            return util::ConstBytePtr( this, item[ index ].offset ).Get();
        }
        const void* GetReferedItem( uint32_t index, uint16_t typeId ) const NN_NOEXCEPT
        {
            if ( index >= count ) return NULL;
            if ( item[ index ].typeId != typeId ) return NULL;
            return util::ConstBytePtr( this, item[ index ].offset ).Get();
        }
        const void* FindReferedItemBy( uint16_t typeId ) const NN_NOEXCEPT
        {
            for ( uint32_t i = 0; i < count; i++ )
            {
                if ( item[ i ].IsValidTypeId( typeId ) )
                {
                    return util::ConstBytePtr( this, item[i].offset ).Get();
                }
            }
            return NULL;
        }
    };

    struct ReferenceWithSizeTable : public Table<ReferenceWithSize>
    {
        const void* GetReferedItem( uint32_t index ) const NN_NOEXCEPT
        {
            NN_SDK_ASSERT( index < count );
            return util::ConstBytePtr( this, item[ index ].offset ).Get();
        }
        const void* GetReferedItemBy( uint16_t typeId ) const NN_NOEXCEPT
        {
            for ( uint32_t i = 0; i < count; i++ )
            {
                if ( item[ i ].IsValidTypeId( typeId ) )
                {
                    return util::ConstBytePtr( this, item[i].offset ).Get();
                }
            }
            return NULL;
        }
        uint32_t GetReferedItemSize( uint32_t index ) const NN_NOEXCEPT
        {
            NN_SDK_ASSERT( index < count );
            return item[ index ].size;
        }
    };

    // ブロック参照テーブル
    // (通常の Util::ReferenceTable ではないので、
    //  count やオフセット起点を外からもらう必要がある)
    struct BlockReferenceTable
    {
        // データ
        ReferenceWithSize item[ 1 ];

        // アクセサ
        inline const void* GetReferedItemByIndex(
                const void* origin, int index, uint16_t count ) const NN_NOEXCEPT
        {
            NN_UNUSED(count);
            NN_SDK_ASSERT( index < count );
            return util::ConstBytePtr( origin, item[ index ].offset ).Get();
        }
        inline const ReferenceWithSize* GetReference(
                uint16_t typeId, uint16_t count ) const NN_NOEXCEPT
        {
            for ( int i = 0; i < count; i++ )
            {
                if ( item[ i ].IsValidTypeId( typeId ) )
                {
                    return &item[ i ];
                }
            }
            return NULL;
        }
        const void* GetReferedItem( const void* origin, uint16_t typeId, uint16_t count ) const NN_NOEXCEPT
        {
            const ReferenceWithSize* ref = GetReference( typeId, count );
            if ( ref == NULL ) return NULL;
            if ( ref->offset == 0 ) return NULL;
            return util::ConstBytePtr( origin, ref->offset ).Get();
        }
        uint32_t GetReferedItemSize( uint16_t typeId, uint16_t count ) const NN_NOEXCEPT
        {
            const ReferenceWithSize* ref = GetReference( typeId, count );
            if ( ref == NULL ) return 0;
            return ref->size;
        }
        uint32_t GetReferedItemOffset( uint16_t typeId, uint16_t count ) const NN_NOEXCEPT
        {
            const ReferenceWithSize* ref = GetReference( typeId, count );
            if ( ref == NULL ) return 0;
            return ref->offset;
        }
    };

    // サウンドファイルの共通ヘッダー
    struct SoundFileHeader
    {
        nn::atk::detail::BinaryFileHeader header;
        BlockReferenceTable blockReferenceTable;

        inline int GetBlockCount() const NN_NOEXCEPT { return header.dataBlocks; }

    protected:
        inline const void* GetBlock( uint16_t typeId ) const NN_NOEXCEPT
        {
            return blockReferenceTable.GetReferedItem( this, typeId, header.dataBlocks );
        }
        inline uint32_t GetBlockSize( uint16_t typeId ) const NN_NOEXCEPT
        {
            return blockReferenceTable.GetReferedItemSize( typeId, header.dataBlocks );
        }
        inline uint32_t GetBlockOffset( uint16_t typeId ) const NN_NOEXCEPT
        {
            return blockReferenceTable.GetReferedItemOffset( typeId, header.dataBlocks );
        }
    };

    // オプションパラメータ
    struct BitFlag
    {
        uint32_t  bitFlag;

    public:
        // binNumber で指定したビットの指す値を value に格納します。
        // ビットが false なら false を返します。
        bool GetValue( uint32_t* value, uint32_t bitNumber ) const NN_NOEXCEPT
        {
            uint32_t count = GetTrueCount( bitNumber );

            // bitNumber 番目のビットが無効だった場合
            if ( count == 0 ) return false;

            *value = *reinterpret_cast<const uint32_t*>(
                    util::ConstBytePtr( this, ( count * sizeof(uint32_t) ) ).Get() );
            return true;
        }
        bool GetValueF32( float* value, uint32_t bitNumber ) const NN_NOEXCEPT
        {
            uint32_t count = GetTrueCount( bitNumber );
            if ( count == 0 ) return false;
            *value = *reinterpret_cast<const float*>(
                    util::ConstBytePtr( this, ( count * sizeof(float) ) ).Get() );
            return true;
        }

    private:
        // アクセサ
        //
        //  bitNumber   - 「何」ビット目をチェックしたいか？ 0 からスタート。
        //  return      - bitNumber ビットが 1 なら、下位から数えていくつめの有効フラグか
        //                を返す。
        //
        //  例: カンタンのため、4 bit の bitFlag とする。
        //      bitFlag = 1001 [２進] のとき、
        //      GetTrueCount( 0 ) => 1
        //      GetTrueCount( 1 ) => 0
        //      GetTrueCount( 2 ) => 0
        //      GetTrueCount( 3 ) => 2
        //      GetTrueCount( 4 ) => ASSERT
        static const int BitNumberMax = 31;
        inline uint32_t GetTrueCount( uint32_t bitNumber ) const NN_NOEXCEPT
        {
            NN_SDK_ASSERT( bitNumber <= BitNumberMax );

            bool ret = false;   // bitNumber ビット目が有効かどうか
            int count = 0;
            for ( uint32_t i = 0; i <= bitNumber; i++ )
            {
                if ( bitFlag & ( 0x1 << i ) )
                {
                    count++;
                    if ( i == bitNumber )
                    {
                        ret = true;
                    }
                }
            }

            if ( ret )
            {
                return count;
            }
            else
            {
                return 0;
            }
        }
    };

    // BitFlag を扱うときに使う便利関数
    static inline uint8_t DevideBy8bit( uint32_t value, int index ) NN_NOEXCEPT
    {
        return static_cast<uint8_t>( ( value >> (8 * index) ) & 0xff );
    }

    static inline uint16_t DevideBy16bit( uint32_t value, int index ) NN_NOEXCEPT
    {
        return static_cast<uint16_t>( ( value >> (16 * index) ) & 0xffff );
    }

    // ID が warcID の波形アーカイブの index 番目の波形アドレスを返します。
    // 未ロードの場合、NULL が返ります。
    static const void* GetWaveFile(
            uint32_t waveArchiveId,
            uint32_t waveIndex,
            const SoundArchive& arc,
            const SoundArchivePlayer& player ) NN_NOEXCEPT;

    static const void* GetWaveFile(
            uint32_t waveArchiveId,
            uint32_t waveIndex,
            const SoundArchive& arc,
            const PlayerHeapDataManager* mgr ) NN_NOEXCEPT;

    enum WaveArchiveLoadStatus
    {
        WaveArchiveLoadStatus_Ok = 0,      // bankFile に必要な波形がすべてロード済み
        WaveArchiveLoadStatus_Noneed,      // 波形アーカイブが不要 (ひとつもインストがない場合に該当)
        WaveArchiveLoadStatus_Partly,      // 個別ロードで、一部がロードされていない
        WaveArchiveLoadStatus_NotYet = -1, // ロードされていない
        WaveArchiveLoadStatus_Error = -2   // その他のエラー
    };
    static WaveArchiveLoadStatus GetWaveArchiveOfBank(
            LoadItemInfo& warcInfo,     // out
            bool& isLoadIndividual,     // out
            const void* bankFile,
            const SoundArchive& arc,
            const SoundArchiveLoader& mgr ) NN_NOEXCEPT;

    static const void* GetWaveFileOfWaveSound(
            const void* wsdFile,
            uint32_t index,  // bfwsd 内でいくつめのウェーブサウンドか？
            const SoundArchive& arc,
            const SoundArchiveLoader& mgr ) NN_NOEXCEPT;

    static inline ItemType GetItemType( uint32_t id ) NN_NOEXCEPT
    {
        // id == SoundArchive::ItemId を想定。
        // 上位 8 bit がアイテムタイプ (nn::atk::detail::ItemType) に相当する
        return static_cast<ItemType>( id >> 24 );
    }

    static inline uint32_t GetItemIndex( uint32_t id ) NN_NOEXCEPT
    {
        // id == SoundArchive::ItemId を想定。
        // 下位 24 bit がアイテムインデックスに相当する
        return ( id & 0x00ffffff );
    }

    static inline uint32_t GetMaskedItemId( uint32_t id, detail::ItemType type ) NN_NOEXCEPT
    {
        return ( id | ( type << 24 ) );
    }

    struct WaveId
    {
        uint32_t waveArchiveId;   // 波形アーカイブ ID
        uint32_t waveIndex;       // 波形アーカイブ内インデックス
    };

    struct WaveIdTable
    {
        // データ
        Table<WaveId> table;

        // アクセサ
        const WaveId* GetWaveId( uint32_t index ) const NN_NOEXCEPT
        {
            if ( index >= table.count )
            {
                return NULL;
            }
            return &table.item[ index ];
        }
        inline uint32_t GetCount() const NN_NOEXCEPT { return table.count; }
    };


    /*
       継承可能なシングルトンクラス
    */
    template <class CHILD>
    class Singleton
    {
    public:
        static NN_NOINLINE CHILD& GetInstance() NN_NOEXCEPT
        {
            static CHILD instance;
            return instance;
        }
    };

    // 負荷計測補助。ピーク値を DefaultHoldFrame だけ保存する。
    template<typename T>
    class PeakHoldValue
    {
    public:
        PeakHoldValue() NN_NOEXCEPT : m_CurrentValue(0), m_PeakValue(0) {}
        void Update( T value )
        {
            m_CurrentValue = value;
            if ( m_PeakHoldCounter > 0 )
            {
                --m_PeakHoldCounter;
            }
            if ( m_PeakHoldCounter == 0 )
            {
                m_PeakValue = 0;
            }
            if ( m_PeakValue < m_CurrentValue )
            {
                m_PeakValue = m_CurrentValue;
                m_PeakHoldCounter = DefaultHoldFrame;
            }
        }
        T GetValue() const NN_NOEXCEPT { return m_CurrentValue; }
        T GetPeakValue() const NN_NOEXCEPT { return m_PeakValue; }
    private:
        static const int DefaultHoldFrame = 20;
        T m_CurrentValue;
        T m_PeakValue;
        int m_PeakHoldCounter;
    };


    // AuxBus -> SubMixBus への射影関数
    static inline int GetSubMixBusFromMainBus()
    {
        return 0;
    }

    static inline int GetSubMixBus(nn::atk::AuxBus bus)
    {
        return static_cast<int>(bus + 1);
    }

    static inline int GetSubMixBus(int auxBus)
    {
        NN_SDK_ASSERT_RANGE(auxBus, 0, nn::atk::AuxBus_Count);
        return auxBus + 1;
    }

    static int GetOutputReceiverMixBufferIndex(const nn::atk::OutputReceiver* pOutputReceiver, int bus, int channel);

    static inline int GetAdditionalSendIndex(int bus) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE(bus, DefaultBusCount, OutputReceiver::BusCountMax);
        return bus - DefaultBusCount;
    }

#if 0
    // 負荷計測補助。サウンド再生時は値が揺れやすいので、ヒストグラムをつくることにした。
    // 全サンプルのうち「負荷○○％以下」におさまっているのは、何％かを出力する。
    class PerfHistogram2 // FIXME: ビルドが通れば、使っていないということなので削除する
    {
    public:
        // コンストラクタ
        PerfHistogram2() { Reset(); }

        // 負荷％値の設定
        void SetLoad( float load )
        {
            // 異常な値
            if ( load < 0.0f || load > 100.0f )
            {
                m_IrregularCount += 1;
            }

            // 最大値更新
            if ( load > m_MaxLoad )
            {
                m_MaxLoad = load;
            }

            int loadIndex = static_cast<int>( load );
            if ( loadIndex < LoadCountMax )
            {
                m_LoadCount[ loadIndex ] += 1;
            }
            m_Count += 1;
            m_SumLoad += load;
        }

        // 処理負荷 load % 以下は、全体の何％か？
        float GetPercentage( int load )
        {
            int count = 0;
            for ( int i = 0; i <= load; i++ )
            {
                count += m_LoadCount[ i ];
            }
            float ret = 100.f * count / m_Count;
            return ret;
        }

        // 全体の percent ％のなかで、最大の処理負荷はいくらか？
        int GetMaxLoadByPercent( float percent ) const
        {
            int sum = 0;
            int maxLoad = 0;
            for ( int i = 0; i < LoadCountMax; i++ )
            {
                sum += m_LoadCount[ i ];
                if ( 100.f * sum / m_Count > percent )
                {
                    maxLoad = i;
                    break;
                }
            }
            return maxLoad;
        }

        float GetAverage() const { return m_SumLoad / m_Count; }
        float GetMaxLoad() const { return m_MaxLoad; }
        int GetCount() const { return m_Count; }
        int GetIrregularCount() const { return m_IrregularCount; }

        // リセット
        void Reset()
        {
            m_MaxLoad = m_SumLoad = 0.0;
            m_Count = m_IrregularCount = 0;
            std::memset( m_LoadCount, 0x00, sizeof(m_LoadCount) );
        }

    private:
        static int const LoadCountMax = 101; // 0-100 まで合計 101 個に分ける
        float m_MaxLoad;
        float m_SumLoad;
        int m_LoadCount[ LoadCountMax ];
        int m_Count, m_IrregularCount;
    };
#endif

    class WarningLogger : public Singleton<WarningLogger>
    {
    public:
        WarningLogger() NN_NOEXCEPT
        {
            m_pCurrentBuffer = &m_Buffer0;
        }
        void Log( int logId, int arg0 = 0, int arg1 = 0 ) NN_NOEXCEPT;
        void Print() NN_NOEXCEPT;
        void SwapBuffer() NN_NOEXCEPT;

        enum LogId
        {
            LogId_ChannelAllocationFailed,
            LogId_SoundthreadFailedWakeup,
            LogId_LogbufferFull,
            LogId_Max
        };

    private:
        struct LogBuffer
        {
            static const int LogCount = 64;
            struct Element
            {
                int logId;
                int arg0;
                int arg1;

                void Print() NN_NOEXCEPT;
            };

            Element element[LogCount];
            int counter;

            LogBuffer() NN_NOEXCEPT : counter(0) {}
            void Log( int logId, int arg0, int arg1 ) NN_NOEXCEPT;
            void Print() NN_NOEXCEPT;
            void Reset() NN_NOEXCEPT { counter = 0; }
        };
        LogBuffer m_Buffer0, m_Buffer1;
        LogBuffer* m_pCurrentBuffer;
    };
};


} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

