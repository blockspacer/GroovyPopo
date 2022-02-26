/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * :include nn/atk/atk_SoundStartable.h
 *
 * @file atk_SoundStartable.h
 */

#pragma once

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_StreamSoundPlayer.h>
#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_OutputReceiver.h>

namespace nn {
namespace atk {

class SoundHandle;


// 再生可能なことを表すインターフェイスです。
// 純粋仮想関数 detail_SetupSoundImpl() 及び detail_ConvertLabelStringToItemId() を
// 継承したクラスで実装することにより、そのクラスで
// StartSound(), HoldSound(), PrepareSound() 関数が使用できるようになります。

//---------------------------------------------------------------------------
//! @brief        サウンドの再生が可能なことを表す抽象クラスです。
//!
//---------------------------------------------------------------------------
class SoundStartable
{
public:
    //---------------------------------------------------------------------------
    //! @brief    サウンド再生処理結果を表すクラスです。
    //!
    //!           ResultCode_ErrorNotEnoughInstance は、
    //!           再生しようとしたサウンドに必要な数のインスタンスが
    //!           作られていないことを表します。
    //!           サウンドアーカイブを使って再生している場合は、
    //!           サウンドアーカイブ中に設定されている
    //!           インスタンス数を増やす必要があります。
    //!
    //! @see SoundStartable::StartSound.
    //! @see SoundStartable::HoldSound
    //! @see SoundStartable::PrepareSound
    //!
    //---------------------------------------------------------------------------
    class StartResult
    {
    public:
        //---------------------------------------------------------------------------
        //! @brief    サウンド再生処理結果を表すコードです。
        //---------------------------------------------------------------------------
        enum ResultCode
        {
            //! 再生は成功しました。
            ResultCode_Success = 0,

            //! プレイヤーで同時再生可能なサウンド数を超え、かつ、
            //! 他のサウンドよりプレイヤー優先度がが低かったため、再生に失敗しました。
            ResultCode_ErrorLowPriority,

            //! 指定したラベル文字列に対応するサウンドが見つからないため、再生に失敗しました。
            ResultCode_ErrorInvalidLabelString,

            //! 指定したサウンド ID に対応するサウンドが見つからないため、再生に失敗しました。
            ResultCode_ErrorInvalidSoundId,

            //! 先着優先に設定された単一再生機能によって、指定したサウンド ID に対応するサウンドの再生が中止されました。
            ResultCode_CanceledBySinglePlay,

            //! @deprecated
            //! こちらの API は削除予定です。代わりに @ref ResultCode_CanceledBySinglePlay を使用してください。
            //! 先着優先に設定された単一再生機能によって、指定したサウンド ID に対応するサウンドの再生が中止されました。
            ResultCode_ErrorCanceledForPrioritizingOldest = ResultCode_CanceledBySinglePlay,

            //! (現在は使われていません)
            ResultCode_ErrorNotDataLoaded,

            //! シーケンスデータが未ロードでプレイヤーヒープも存在しないため、再生に失敗しました。
            ResultCode_ErrorNotSequenceLoaded,

            //! バンクデータが未ロードでプレイヤーヒープも存在しないため、再生に失敗しました。
            ResultCode_ErrorNotBankLoaded,

            //! ウェーブサウンドデータが未ロードでプレイヤーヒープも存在しないため、再生に失敗しました。
            ResultCode_ErrorNotWsdLoaded,

            //! 波形アーカイブデータが未ロードでプレイヤーヒープも存在しないため、再生に失敗しました。
            ResultCode_ErrorNotWarcLoaded,

            //! プレイヤーヒープの準備が整っていないため、再生に失敗しました。
            ResultCode_ErrorNotEnoughPlayerHeap,

            //! (現在は使われていません)
            ResultCode_ErrorCannotOpenFile,

            //! @ref SoundArchivePlayer::Initialize が呼ばれていない、あるいは、SoundArchivePlayer
            //! に設定した SoundArchive に対する初期化 (@ref FsSoundArchive::Open,
            //! @ref MemorySoundArchive::Initialize, @ref AddonSoundArchive::Initialize) が呼ばれていないため、
            //! 再生に失敗しました。
            ResultCode_ErrorNotAvailable,

            //! (現在は使われていません)
            ResultCode_ErrorCannotAllocateTrack,

            //! 優先度が低いため、サウンドインスタンスの確保に失敗し、再生に失敗しました。
            //! サウンドインスタンスの数は、SoundMaker の [プロジェクトの設定] -
            //! [サウンドアーカイブ] タブの「◯◯サウンドの数」で設定できます。
            //! また、優先度を高くするには、プレイヤー優先度を高くしてください。
            ResultCode_ErrorNotEnoughInstance,

            //! 不正なパラメータが指定されたため、再生に失敗しました。
            //! 現在は、@ref StartInfo の actorPlayerId が不正な値 (＝0～3以外の値) の場合に、
            //! 本エラーが発生します。
            ResultCode_ErrorInvalidParameter,

            //! @ref StartInfo の sequenceSoundInfo で指定されている startLocationLabel が不正なため、
            //! 再生に失敗しました。
            ResultCode_ErrorInvalidSequenceStartLocationLabel,

            //! @ref SoundActor あるいは @ref Sound3DActor が初期化前のため、
            //! 再生に失敗しました。
            ResultCode_ErrorActorNotInitialized,

            //! (現在は使われていません)
            ResultCode_ErrorInvalidWarcId,

            //! ロード済みのバンクデータが不正なため、波形アーカイブデータが見つからず、再生に失敗しました。
            ResultCode_ErrorInvalidBankData,

            ResultCode_ErrorInvalidStreamFileId,
            ResultCode_ErrorInvalidStreamFilePath,

            //! サウンドライブラリ内部では使われていません。
            //! "ResultCode_ErrorUser" ～ "ResultCode_ErrorUnknown - 1" は、
            //! ユーザーが自由に利用することができます。
            ResultCode_ErrorUser = 128,

            //! なんらかの理由で、再生に失敗しました。
            ResultCode_ErrorUnknown = 255
        };

        //! @internal
        StartResult() NN_NOEXCEPT : m_Code( ResultCode_ErrorUnknown ) {}

        //! @internal
        //! @param code :private
        explicit StartResult( ResultCode code ) NN_NOEXCEPT : m_Code( code ) {}

        //---------------------------------------------------------------------------
        //! @brief    再生成功したかどうかを取得します。
        //!
        //! @return   再生成功したら true, 失敗したら false を返します。
        //!
        //! @see SoundStartable::StartSound
        //! @see SoundStartable::HoldSound
        //! @see SoundStartable::PrepareSound
        //!
        //---------------------------------------------------------------------------
        bool IsSuccess() const NN_NOEXCEPT { return m_Code == ResultCode_Success; }

        //---------------------------------------------------------------------------
        //! @brief    再生処理結果コードを取得します。
        //!
        //! @return   再生処理結果コードを返します。
        //!
        //---------------------------------------------------------------------------
        ResultCode GetCode() const NN_NOEXCEPT { return m_Code; }

    private:
        ResultCode m_Code;
    };

    // static const char* detail_ConvertStartResultToString( StartResult startResult );

    //---------------------------------------------------------------------------
    //! @brief  サウンド再生時に渡される、詳細な再生パラメータです。
    //!
    //!         @ref SoundStartable::StartSound 等の関数に渡して使用します。
    //!         この構造体を使用しなくてもサウンドは再生出来ます。
    //!
    //!         startOffsetType, startOffset を設定すると、
    //!         サウンドを途中から再生することが可能です。
    //!         この 2 つのパラメータは同時に設定する必要があります。
    //!         設定した値を有効にするには、
    //!         enableFlag の EnableFlagBit_StartOffset ビットを設定してください。
    //!
    //!         オフセット値が同じでも、
    //!         データの種類によって途中再生のために必要な処理時間が異なります。
    //!         大きな処理時間がかかる場合があることに注意してください。
    //!
    //!         【シーケンスサウンド】 @n
    //!         シーケンスを途中まで空回しするため、
    //!         開始オフセットの大きさに応じた処理時間がかかります。
    //!
    //!         【ストリームサウンド】 @n
    //!         データが PCM の場合は、開始オフセットの大きさに関わらず、
    //!         途中再生のための処理時間はほとんどかかりません。
    //!         データが ADPCM の場合は、1 チャンネルあたり最大 14336 サンプルのデータを
    //!         CPU でデコードするための処理がかかります。
    //!         ADPCM では、再生開始位置が指定された位置の直前にある
    //!         14 サンプル境界に変更されます。
    //!
    //!         【ウェーブサウンド】 @n
    //!         データが PCM の場合は、開始オフセットの大きさに関わらず、
    //!         途中再生のための処理時間はほとんどかかりません。
    //!         データが ADPCM の場合は、
    //!         データ先頭から再生開始位置までの全てのサンプルを
    //!         CPU でデコードする必要があるため、
    //!         開始オフセットの大きさに応じた処理時間がかかります。
    //!         ADPCM では、再生開始位置が指定された位置の直前にある
    //!         14 サンプル境界に変更されます。
    //!
    //!         サウンドはサウンドアーカイブ中で設定された
    //!         ID のプレイヤーを使用して再生されますが、
    //!         これとは異なるプレイヤーを指定して再生したい場合、
    //!         playerId を設定します。設定した値を有効にするには、
    //!         enableFlag の EnableFlagBit_PlayerId ビットを設定してください。
    //!
    //!         サウンドはサウンドアーカイブ中で設定された
    //!         プレイヤープライオリティを使用して再生されますが、
    //!         これとは異なるプライオリティ値を指定して再生したい場合、
    //!         playerPriority を設定します。設定した値を有効にするには、
    //!         enableFlag の EnableFlagBit_PlayerPriority ビットを設定してください。
    //!
    //!         actorPlayerId は、@ref SoundActor
    //!         クラスで再生する際に使用するアクタープレイヤー番号を設定します。
    //!         値の範囲は 0 ～ 3 です。設定した値を有効にするには、
    //!         enableFlag の EnableFlagBit_ActorPlayerId ビットを設定してください。
    //!
    //!         sequenceSoundInfo は、シーケンスサウンドに関するパラメータです。
    //!         シーケンスサウンドを再生するときにのみ有効です。
    //!         サウンドアーカイブ中で設定されている、
    //!         シーケンスサウンドに関する情報を上書きすることができます。
    //!         詳しくは SequenceSoundInfo 構造体を参照してください。
    //!         設定した値を有効にするには、 enableFlag の EnableFlagBit_SequenceSoundInfo
    //!         ビットを設定してください。
    //!
    //          // streamSoundInfo は、ストリームサウンドに関するパラメータです。
    //          // ストリームサウンドを再生するときのみ有効です。
    //          // ストリームサウンドのリージョンジャンプ機能を利用する場合に、
    //          // 本構造体を使用する必要があります。
    //          // 詳しくは StreamSoundInfo 構造体を参照してください。
    //          // 設定した値を有効にするには、 enableFlag の EnableFlagBit_StrmSoundInfo
    //          // ビットを設定してください。
    //!
    //! @see  SoundStartable::StartSound
    //---------------------------------------------------------------------------
    struct StartInfo
    {
        //---------------------------------------------------------------------------
        //! @brief    StartInfo 構造体のパラメータを有効にするためのビットフラグの定義です。
        //!
        //!           それぞれの値の論理和を enableFlag に設定します。
        //!
        //! @see      SoundStartable::StartInfo
        //---------------------------------------------------------------------------
        enum EnableFlagBit
        {
            /*!
              @brief    startOffsetType, startOffset に設定された値を有効にします。
                        2 つのパラメータは同時に設定する必要があります。
             */
            EnableFlagBit_StartOffset      = (1<<0),

            //! playerId に設定された値を有効にします。
            EnableFlagBit_PlayerId         = (1<<1),

            //! playerPriority に設定された値を有効にします。
            EnableFlagBit_PlayerPriority   = (1<<2),

            //! actorPlayerId に設定された値を有効にします。
            EnableFlagBit_ActorPlayerId   = (1<<3),

            //! sequenceSoundInfo に設定された値を有効にします。
            EnableFlagBit_SequenceSoundInfo    = (1<<4),

            //! streamSoundInfo に設定された値を有効にします。
            EnableFlagBit_StreamSoundInfo   = (1<<5),

            //! waveSoundInfo に設定された値を有効にします。
            EnableFlagBit_WaveSoundInfo   = (1<<6),

            //! @briefprivate
            //! @deprecated SoundSystemParam::rendererSampleRate で 48kHz を指定することで制約もなく 48kHz レンダリングできるためそちらを使用してください。
            //!             互換性のために残されていますが、これらの API はすでに使用できない状態になっていて、今後削除される予定です。
            EnableFlagBit_VoiceRendererType = (1<<7),

            //! @briefprivate
            //! fadeFrame に設定された値を有効にします。
            EnableFlagBit_FadeFrame          = (1<<8),

            //! @briefprivate
            //! soundStopCallback に設定された値を有効にします。
            EnableFlagBit_SoundStopCallback = (1<<9),

            //! streamSoundMetaInfo に設定された値を有効にします。
            EnableFlagBit_StreamSoundMetaInfo = (1<<10),

            //! @details :private streamSoundMetaInfo2 に設定された値を有効にします。
            EnableFlagBit_StreamSoundMetaInfo2  = (1<<11),

            //! @briefprivate
            //! delayTime に設定された値を有効にします。
            EnableFlagBit_DelayTime  = (1<<12),

            //! @briefprivate
            //! delayCountに設定された値を有効にします。EnableFlagBit_DelayTime が指定されている場合は無効になるためご注意ください。
            EnableFlagBit_DelayCount  = (1<<13),

            //! @briefprivate
            //! updateType に設定された値を有効にします。
            EnableFlagBit_UpdateType  = (1<<14),

            //! @briefprivate
            //! subMixIndex に設定された値を有効にします。
            EnableFlagBit_SubMixIndex  = (1<<15),

            //! pOutputReceiver に設定された値を有効にします。
            EnableFlagBit_OutputReceiver  = (1<<16),

            //! loopInfo に設定された値を有効にします。
            EnableFlagBit_LoopInfo  = (1<<17),

            //! isAdditionalDecodingOnLoopEnabled に設定された値を有効にします
            EnableFlagBit_IsAdditionalDecodingOnLoopEnabled = (1<<18),
        };

        //---------------------------------------------------------------------------
        //! @brief    サウンドの開始オフセット値の単位の定義です。
        //!
        //! @see      SoundStartable::StartInfo
        //!
        //---------------------------------------------------------------------------
        enum StartOffsetType
        {
            StartOffsetType_MilliSeconds, //!< オフセット値の単位はミリ秒です。

            /*!
                @brief  オフセット値の単位は Tick です。
                        この指定はシーケンスサウンドに対してのみ有効です。
                        それ以外では、オフセット値の指定が無効になります。
             */
            StartOffsetType_Tick,

            /*!
                @brief  オフセット値の単位は波形のサンプル数です。
                        この指定はストリームサウンドとウェーブサウンドに対して有効です。
                        それ以外では、オフセット値の指定が無効になります。
             */
            StartOffsetType_Sample
        };

        //---------------------------------------------------------------------------
        //! @brief    シーケンスサウンドに関するパラメータ構造体です。
        //!
        //!           シーケンスサウンド再生時に渡されます。
        //!
        //!           seqDataAddress にシーケンスデータのアドレスを指定すると、
        //!           サウンドアーカイブ中で設定されているシーケンスデータの代わりに、
        //!           メモリ上に置かれたシーケンスデータをアドレスで指定して再生することができます。
        //!           NULL を指定すると、
        //!           元々サウンドアーカイブで設定されているシーケンスデータを再生します。
        //!
        //!           startLocationLabel には、シーケンスデータ内の再生開始位置を指定します。
        //!           サウンドアーカイブ中で設定されているラベル文字列の代わりに、
        //!           ここで指定した文字列を使用するようになります。
        //!           NULL を指定すると、
        //!           元々サウンドアーカイブで設定されている再生開始位置を使用して再生します。
        //!
        //!           bankIds には、シーケンスデータと関連付けるバンク ID を指定します。
        //!           サウンドアーカイブ中で設定されているバンク ID の代わりに、
        //!           ここで指定した ID を仕様するようになります。
        //!           NULL を指定すると、
        //!           元々サウンドアーカイブで設定されているバンク ID を仕様して再生します。
        //!
        //! @see      SoundStartable::StartInfo
        //---------------------------------------------------------------------------
        struct SequenceSoundInfo
        {
            //! シーケンスデータのアドレスです。
            const void* sequenceDataAddress;

            //! シーケンスデータ内の再生開始位置を示すラベル文字列です。
            const char* startLocationLabel;

            //! シーケンスデータと関連付けるバンク ID です。
            SoundArchive::ItemId bankIds[ SoundArchive::SequenceBankMax ];

            //! コンストラクタです。
            SequenceSoundInfo() NN_NOEXCEPT : sequenceDataAddress( NULL ), startLocationLabel( NULL )
            {
                for ( int i = 0; i < SoundArchive::SequenceBankMax; i++ )
                {
                    bankIds[ i ] = SoundArchive::InvalidId;
                }
            }
        };

        //---------------------------------------------------------------------------
        //! @brief    ストリームサウンドに関するパラメータ構造体です。
        //!
        //!           ストリームサウンド再生時に渡されます。
        //!
        //!           externalPath にストリームサウンドバイナリへのパスを指定すると、
        //!           サウンドアーカイブ中で設定されているストリームサウンドが参照する
        //!           ストリームサウンドバイナリの代わりに、指定したものを使用して再生することができます。
        //!           NULL を指定すると、
        //!           元々サウンドアーカイブで設定されているストリームサウンドバイナリが使用されます。
        //!
        //  @see StreamRegionCallback
        //---------------------------------------------------------------------------
        struct StreamSoundInfo
        {
            //! サウンドアーカイブ管理外の bfstm ファイルのパスです。フルパスで指定する必要があります。
            const char* externalPath;

            //! @briefprivate
            //! メモリ上においてあるストリームサウンドへのアドレスです。
            const void* pExternalData;

            //! @briefprivate
            //! メモリ上においてあるストリームサウンドのサイズです。
            size_t externalDataSize;

            //! ストリームジャンプ機能において、ジャンプ先のリージョンを指定するためのコールバック関数です。
            StreamRegionCallback regionCallback;

            //! StreamRegionCallback に渡すユーザー引数です。
            void* regionCallbackArg;

            //! メモリ上においてあるプリフェッチデータへのアドレスです。開始オフセットを指定している場合はプリフェッチデータは使用されなくなります。
            const void* prefetchData;

            //! @briefprivate
            //! 指定している場合、メモリ配置の問題でプリフェッチデータが再生できない際に強制的にストリーム再生します。
            bool forcePlayPrefetchFlag;

            //! @briefprivate
            //! 再生に使用する外部ストリームバッファプールを指定します。
            detail::driver::StreamBufferPool* pStreamBufferPool;

            //! @brief コンストラクタです。
            StreamSoundInfo() NN_NOEXCEPT
            : externalPath(nullptr)
            , pExternalData(nullptr)
            , externalDataSize(0)
            , regionCallback(nullptr)
            , regionCallbackArg(nullptr)
            , prefetchData(nullptr)
            , forcePlayPrefetchFlag(false)
            , pStreamBufferPool(nullptr)
            {}
        };

        //---------------------------------------------------------------------------
        //! @brief    ウェーブサウンドに関するパラメータ構造体です。
        //!
        //!           ウェーブサウンド再生時に渡されます。
        //!
        //!           waveAddress に波形データのアドレスを指定すると、
        //!           サウンドアーカイブ中で設定されている波形データの代わりに、
        //!           メモリ上に置かれた波形データを使用して再生することができます。
        //!           NULL を指定すると、
        //!           元々サウンドアーカイブで設定されている波形データが使用されます。
        //---------------------------------------------------------------------------
        struct WaveSoundInfo
        {
            //---------------------------------------------------------------------------
            //! @brief    WaveSoundInfo 構造体のパラメータを有効にするためのビットフラグの定義です。
            //!
            //!           それぞれの値の論理和を enableParameterFlag に設定します。
            //---------------------------------------------------------------------------
            enum EnableParameterFlagBit
            {
                //! release に設定された値を有効にします。
                EnableParameterFlagBit_Release = (1 << 0),
                //! @briefprivate ContextCalculationSkipMode に設定した値を有効にします
                EnableParameterFlagBit_ContextCalculationSkipMode = (1 << 1)
            };

            //! 使用する波形データのアドレスです。
            const void* waveAddress;

            //! @ref nn::atk::WaveType を指定します。
            int8_t waveType;

            //! @internal
            uint8_t m_Padding[3];

            //! @brief  構造体に設定された値を有効にするためのビットフラグです。
            //! デフォルト値では全て無効になっています。
            int enableParameterFlag;

            //! @brief 音源に与えられるリリースパラメータです。
            //! 0 ～ 127 の間で指定します。
            int release;

            //! @briefprivate ADPCM 再生でオフセットを指定した場合に、再生に用いるコンテキスト計算を再生前に計算せずに再生を行います。
            //! この結果、オフセット指定時の再生による CPU 処理は削減されますが、
            //! バイナリに存在する波形データとは出力結果が一致しない状態で再生されます。
            bool isContextCalculationSkipMode;

            //! @brief コンストラクタです。
            WaveSoundInfo() NN_NOEXCEPT
            : waveAddress(NULL)
            , waveType(WaveType_Nwwav)
            , enableParameterFlag(0)
            , release(nn::atk::AdshrMax)
            , isContextCalculationSkipMode(false)
            {
            }
        };

        //---------------------------------------------------------------------------
        //! @brief    ループに関するパラメータ構造体です。
        //!           ウェーブサウンドおよびストリームサウンドで有効なパラメーターとなります。
        //!           シーケンスサウンドでは、このパラメーターは無効化されます。
        //!
        //---------------------------------------------------------------------------
        struct LoopInfo
        {
            //---------------------------------------------------------------------------
            //! @brief    LoopInfo 構造体のパラメータを有効にするためのビットフラグの定義です。
            //!
            //!           それぞれの値の論理和を enableParameterFlag に設定します。
            //---------------------------------------------------------------------------
            enum EnableParameterFlagBit
            {
                //! isLoopEnabled に設定された値を有効にします。
                EnableParameterFlagBit_LoopEnabled = (1 << 0)
            };

            //! @brief  構造体に設定された値を有効にするためのビットフラグです。
            //! デフォルト値では全て無効になっています。
            uint32_t enableParameterFlag;

            //---------------------------------------------------------------------------
            //! @brief  ループの有無です。
            //---------------------------------------------------------------------------
            bool isLoopEnabled;

            //---------------------------------------------------------------------------
            //! @brief コンストラクタです。
            //---------------------------------------------------------------------------
            LoopInfo() NN_NOEXCEPT
                : enableParameterFlag(0)
                , isLoopEnabled( false )
            {}
        };

        /*!
            @brief  構造体に設定された値を有効にするためのビットフラグです。
                    デフォルト値では全て無効になっています。
         */
        uint32_t enableFlag;

        //! サウンドの開始オフセット値の単位です。
        StartOffsetType startOffsetType;

        //! サウンドの開始オフセット値です。
        int startOffset;

        //! サウンドの再生に使用するプレイヤーの ID です。
        SoundArchive::ItemId playerId;

        //! サウンドに設定するプレイヤープライオリティです。
        int playerPriority;

        //! SoundActorで再生する場合に使用するアクタープレイヤーの番号です。
        int actorPlayerId;

        //! シーケンスサウンドに関するパラメータです。
        SequenceSoundInfo sequenceSoundInfo;

        //! ストリームサウンドに関するパラメータです。
        StreamSoundInfo streamSoundInfo;

        //! ストリームサウンドのメタ情報に関するパラメータです。
        SoundArchive::StreamSoundInfo streamSoundMetaInfo;

        //! @details :private
        //! ストリームサウンドの追加のメタ情報に関するパラメータです。
        SoundArchive::StreamSoundInfo2 streamSoundMetaInfo2;

        //! ウェーブサウンドに関するパラメータです。
        WaveSoundInfo waveSoundInfo;

        //! ループ情報に関するパラメータです。ウェーブサウンドおよびストリームサウンドで有効なパラメーターとなります。
        LoopInfo loopInfo;

        //! @briefprivate
        //! @deprecated SoundSystemParam::rendererSampleRate で 48kHz を指定することで制約もなく 48kHz レンダリングできるためそちらを使用してください。
        //!             互換性のために残されていますが、これらの API はすでに使用できない状態になっていて、今後削除される予定です。
        //! ボイスレンダリングモードです。
        uint8_t voiceRendererType;

        //! @briefprivate
        //! HoldSound を指定したフレーム数をかけてフェードアウトさせます。
        //! StartSound では使用されません。
        int fadeFrame;

        //! @briefprivate
        //! サウンドの停止時に呼ばれるコールバック関数です。
        SoundStopCallback soundStopCallback;

        //! @briefprivate
        //! サウンドを指定した時間だけ待ってから再生させます。
        //! 単位はミリ秒（ms)で、5 の倍数のみ設定することができます。
        //! 5 の倍数ではない値を指定した場合、アサートで停止しますのでご注意ください。
        int delayTime;

        //! @briefprivate
        //! サウンドを指定したオーディオフレーム更新回数だけ待ってから再生させます。
        //! この機能は VoiceCommand と併せて使用する事を想定しています。
        //! VoiceCommand を使用しない場合は delayTime をご利用ください。(EnableFlagBit_DelayTime 指定時は無効になるためご注意ください)
        //! 実時間での遅延量はオーディオフレームの更新間隔や更新頻度に依存するため、アプリケーション側で計算が必要になります。
        int delayCount;

        //! @briefprivate
        //! VoiceCommand 使用時のサウンドの更新頻度を指定します。
        //! UpdateType_GameFrame 指定時は、VoiceCommandProcess() を
        //! UpdateType_GameFrame で呼び出した時にだけ更新が行われるようになります。
        //! UpdateType_AudioFrame 指定時はどちらの場合でも更新が行われます。
        //! これを利用して、WSD や STRM などの更新頻度を下げ、処理負荷を軽減する事が可能です。
        //! ただしパラメータの反映の頻度が下がり、変化が粗くなってしまいますのでご注意ください。
        UpdateType updateType;

        //! @briefprivate
        //! サウンドを何番目のサブミックスで再生するかを指定します。初期値は 0 です。
        //! @pre
        //! - 0 <= subMixIndex < SoundSystemParam::SubMixCountMax
        int subMixIndex;

        //! @brief サウンドの出力先を設定します。初期値は nullptr です。
        //! @pre
        //! - pOutputReceiver != nullptr
        OutputReceiver* pOutputReceiver;

        //! @brief  ループ時のノイズを軽減するための空デコードを有効にするかを設定します。デフォルトは有効です。
        //!         また、この値は Opus ファイルを設定しているストリームサウンドの場合にのみ使用され、それ以外の場合には無視されます。
        bool isAdditionalDecodingOnLoopEnabled;

        //! コンストラクタです
        StartInfo() NN_NOEXCEPT
        : enableFlag( 0 )
        , voiceRendererType(VoiceRendererType_Sdk)
        , soundStopCallback(NULL)
        , delayTime( 0 )
        , delayCount( 0 )
        , updateType(UpdateType_AudioFrame)
        , subMixIndex( 0 )
        , pOutputReceiver( nullptr )
        , isAdditionalDecodingOnLoopEnabled( true ){}
    };

    /* ------------------------------------------------------------------------
            member definition
       ------------------------------------------------------------------------ */
    // デストラクタ
    //! @internal
    virtual ~SoundStartable() NN_NOEXCEPT {};

    //----------------------------------------
    //! @name 再生
    //@{
    // 再生関数
    //---------------------------------------------------------------------------
    //! @brief  指定した ID のサウンドを再生します。
    //!
    //!         第一引数には、サウンドハンドルを指定します。
    //!         再生に成功したサウンドは、このサウンドハンドルと関連付けられます。
    //!
    //!         第二引数には、再生するサウンドの識別子としてサウンド ID を指定します。
    //!
    //!         pStartInfo は、再生時に設定できる詳細なパラメータです。
    //!         このパラメータは、設定せずに再生を開始することが可能です。
    //!
    //!         この関数を呼び出すことは、@ref PrepareSound を呼び出した後、
    //!         すぐにハンドルクラスの @ref SoundHandle::StartPrepared
    //!         を呼び出すことと同じです。
    //!
    //! @param[in] pHandle        再生されるサウンドと関連付けられるハンドルです。
    //! @param[in] soundId        再生するサウンドの ID です。
    //! @param[in] pStartInfo     詳細な再生パラメータです。
    //!
    //! @return   再生処理結果を @ref StartResult 型で返します。
    //!
    //! @see SoundHandle クラス
    //! @see StartResult
    //! @see PrepareSound
    //! @see StartInfo
    //!
    //---------------------------------------------------------------------------
    StartResult StartSound(
            SoundHandle* pHandle,
            SoundArchive::ItemId soundId,
            const StartInfo* pStartInfo = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  指定したサウンドアーカイブ、 ID のサウンドを再生します。
    //!
    //!         第一引数には、サウンドハンドルを指定します。
    //!         再生に成功したサウンドは、このサウンドハンドルと関連付けられます。
    //!
    //!         第二引数には、再生するサウンドの識別子としてサウンド ID を指定します。
    //!
    //!         soundArchiveName には、対象となるサウンドアーカイブの名前を指定します。
    //!         nullptr を渡した場合は、メインのサウンドアーカイブを選択したことになります。
    //!
    //!         pStartInfo は、再生時に設定できる詳細なパラメータです。
    //!         このパラメータは、設定せずに再生を開始することが可能です。
    //!
    //!         この関数を呼び出すことは、@ref PrepareSound を呼び出した後、
    //!         すぐにハンドルクラスの @ref SoundHandle::StartPrepared
    //!         を呼び出すことと同じです。
    //!
    //! @param[in] pHandle          再生されるサウンドと関連付けられるハンドルです。
    //! @param[in] soundId          再生するサウンドの ID です。
    //! @param[in] soundArchiveName 再生対象となるサウンドアーカイブの名前です。
    //! @param[in] pStartInfo       詳細な再生パラメータです。
    //!
    //! @return   再生処理結果を @ref StartResult 型で返します。
    //!
    //! @see SoundHandle クラス
    //! @see StartResult
    //! @see PrepareSound
    //! @see StartInfo
    //!
    //---------------------------------------------------------------------------
    StartResult StartSound(
            SoundHandle* pHandle,
            SoundArchive::ItemId soundId,
            const char* soundArchiveName,
            const StartInfo* pStartInfo = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  指定したラベル文字列のサウンドを再生します。
    //!
    //!         第一引数には、サウンドハンドルを指定します。
    //!         再生に成功したサウンドは、このサウンドハンドルと関連付けられます。
    //!
    //!         第二引数には、再生するサウンドの識別子として、
    //!         サウンドのラベル文字列を指定します。
    //!
    //!         pStartInfo は、再生時に設定できる詳細なパラメータです。
    //!         このパラメータは、設定せずに再生を開始することが可能です。
    //!
    //!         この関数を呼び出すことは、@ref PrepareSound を呼び出した後、
    //!         すぐにハンドルクラスの @ref SoundHandle::StartPrepared
    //!         を呼び出すことと同じです。
    //!
    //! @param[in] pHandle        再生されるサウンドと関連付けられるハンドルです。
    //! @param[in] pSoundName     再生するサウンドのラベル文字列です。
    //! @param[in] pStartInfo     詳細な再生パラメータです。
    //!
    //! @return   再生処理結果を @ref StartResult 型で返します。
    //!
    //! @see SoundHandle クラス
    //! @see StartResult
    //! @see PrepareSound
    //! @see StartInfo
    //!
    //---------------------------------------------------------------------------
    StartResult StartSound(
            SoundHandle* pHandle,
            const char* pSoundName,
            const StartInfo* pStartInfo = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  指定したサウンドアーカイブ、ラベル文字列のサウンドを再生します。
    //!
    //!         第一引数には、サウンドハンドルを指定します。
    //!         再生に成功したサウンドは、このサウンドハンドルと関連付けられます。
    //!
    //!         第二引数には、再生するサウンドの識別子として、
    //!         サウンドのラベル文字列を指定します。
    //!
    //!         soundArchiveName には、対象となるサウンドアーカイブの名前を指定します。
    //!         nullptr を渡した場合は、メインのサウンドアーカイブを選択したことになります。
    //!
    //!         pStartInfo は、再生時に設定できる詳細なパラメータです。
    //!         このパラメータは、設定せずに再生を開始することが可能です。
    //!
    //!         この関数を呼び出すことは、@ref PrepareSound を呼び出した後、
    //!         すぐにハンドルクラスの @ref SoundHandle::StartPrepared
    //!         を呼び出すことと同じです。
    //!
    //! @param[in] pHandle          再生されるサウンドと関連付けられるハンドルです。
    //! @param[in] pSoundName       再生するサウンドのラベル文字列です。
    //! @param[in] soundArchiveName 再生対象となるサウンドアーカイブの名前です。
    //! @param[in] pStartInfo       詳細な再生パラメータです。
    //!
    //! @return   再生処理結果を @ref StartResult 型で返します。
    //!
    //! @see SoundHandle クラス
    //! @see StartResult
    //! @see PrepareSound
    //! @see StartInfo
    //!
    //---------------------------------------------------------------------------
    StartResult StartSound(
            SoundHandle* pHandle,
            const char* pSoundName,
            const char* soundArchiveName,
            const StartInfo* pStartInfo = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  指定した ID のサウンドを再生または継続します。
    //!
    //!         同じ ID で毎フレームこの関数を呼び続けている間、
    //!         サウンドを再生することができます。
    //!         呼び出しが途切れた時はサウンドが停止します。
    //!
    //!         第一引数には、サウンドハンドルを指定します。
    //!
    //!         第二引数には、再生するサウンドの識別子としてサウンド ID を指定します。
    //!
    //!         pHoldInfo は、再生時に設定できる詳細なパラメータです。
    //!         このパラメータは、設定せずに再生を開始することが可能です。
    //!
    //! @param[in] pHandle        再生されるサウンドと関連付けられるハンドルです。
    //! @param[in] soundId        再生するサウンドの ID です。
    //! @param[in] pHoldInfo      詳細な再生パラメータです。
    //!
    //! @return   再生処理結果を @ref StartResult 型で返します。
    //!
    //! @see SoundHandle クラス
    //! @see StartResult
    //! @see StartInfo
    //!
    //---------------------------------------------------------------------------
    StartResult HoldSound(
            SoundHandle* pHandle,
            SoundArchive::ItemId soundId,
            const StartInfo* pHoldInfo = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  指定したサウンドアーカイブ、ID のサウンドを再生または継続します。
    //!
    //!         同じ ID で毎フレームこの関数を呼び続けている間、
    //!         サウンドを再生することができます。
    //!         呼び出しが途切れた時はサウンドが停止します。
    //!
    //!         第一引数には、サウンドハンドルを指定します。
    //!
    //!         第二引数には、再生するサウンドの識別子としてサウンド ID を指定します。
    //!
    //!         soundArchiveName には、対象となるサウンドアーカイブの名前を指定します。
    //!         nullptr を渡した場合は、メインのサウンドアーカイブを選択したことになります。
    //!
    //!         pHoldInfo は、再生時に設定できる詳細なパラメータです。
    //!         このパラメータは、設定せずに再生を開始することが可能です。
    //!
    //! @param[in] pHandle          再生されるサウンドと関連付けられるハンドルです。
    //! @param[in] soundId          再生するサウンドの ID です。
    //! @param[in] soundArchiveName 再生対象となるサウンドアーカイブの名前です。
    //! @param[in] pHoldInfo        詳細な再生パラメータです。
    //!
    //! @return   再生処理結果を @ref StartResult 型で返します。
    //!
    //! @see SoundHandle クラス
    //! @see StartResult
    //! @see StartInfo
    //!
    //---------------------------------------------------------------------------
    StartResult HoldSound(
            SoundHandle* pHandle,
            SoundArchive::ItemId soundId,
            const char* soundArchiveName,
            const StartInfo* pHoldInfo = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  指定したラベル文字列のサウンドを再生または継続します。
    //!
    //!         同じ ID で毎フレームこの関数を呼び続けている間、
    //!         サウンドを再生することができます。
    //!         呼び出しが途切れた時はサウンドが停止します。
    //!
    //!         第一引数には、サウンドハンドルを指定します。
    //!
    //!         第二引数には、再生するサウンドの識別子として、
    //!         サウンドのラベル文字列を指定します。
    //!
    //!         pHoldInfo は、再生時に設定できる詳細なパラメータです。
    //!         このパラメータは、設定せずに再生を開始することが可能です。
    //!
    //! @param[in] pHandle        再生されるサウンドと関連付けられるハンドルです。
    //! @param[in] pSoundName     再生するサウンドのラベル文字列です。
    //! @param[in] pHoldInfo      詳細な再生パラメータです。
    //!
    //! @return   再生処理結果を @ref StartResult 型で返します。
    //!
    //! @see SoundHandle クラス
    //! @see StartResult
    //! @see StartInfo
    //!
    //---------------------------------------------------------------------------
    StartResult HoldSound(
            SoundHandle* pHandle,
            const char* pSoundName,
            const StartInfo* pHoldInfo = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  指定したサウンドアーカイブ、ラベル文字列のサウンドを再生または継続します。
    //!
    //!         同じ ID で毎フレームこの関数を呼び続けている間、
    //!         サウンドを再生することができます。
    //!         呼び出しが途切れた時はサウンドが停止します。
    //!
    //!         第一引数には、サウンドハンドルを指定します。
    //!
    //!         第二引数には、再生するサウンドの識別子として、
    //!         サウンドのラベル文字列を指定します。
    //!
    //!         soundArchiveName には、対象となるサウンドアーカイブの名前を指定します。
    //!         nullptr を渡した場合は、メインのサウンドアーカイブを選択したことになります。
    //!
    //!         pHoldInfo は、再生時に設定できる詳細なパラメータです。
    //!         このパラメータは、設定せずに再生を開始することが可能です。
    //!
    //! @param[in] pHandle          再生されるサウンドと関連付けられるハンドルです。
    //! @param[in] pSoundName       再生するサウンドのラベル文字列です。
    //! @param[in] soundArchiveName 再生対象となるサウンドアーカイブの名前です。
    //! @param[in] pHoldInfo        詳細な再生パラメータです。
    //!
    //! @return   再生処理結果を @ref StartResult 型で返します。
    //!
    //! @see SoundHandle クラス
    //! @see StartResult
    //! @see StartInfo
    //!
    //---------------------------------------------------------------------------
    StartResult HoldSound(
            SoundHandle* pHandle,
            const char* pSoundName,
            const char* soundArchiveName,
            const StartInfo* pHoldInfo = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  指定した ID のサウンドを再生する準備をします。
    //!
    //!         準備が完了したサウンドは、ハンドルクラスの
    //!         @ref SoundHandle::StartPrepared
    //!         を呼び出して再生を開始することができます。
    //!
    //!         第一引数には、サウンドハンドルを指定します。
    //!         再生に成功したサウンドは、このサウンドハンドルと関連付けられます。
    //!
    //!         第二引数には、再生するサウンドの識別子としてサウンド ID を指定します。
    //!
    //!         pStartInfo は、再生時に設定できる詳細なパラメータです。
    //!         このパラメータは、設定せずに再生を開始することが可能です。
    //!
    //! @param[in] pHandle        再生されるサウンドと関連付けられるハンドルです。
    //! @param[in] soundId        再生するサウンドの ID です。
    //! @param[in] pStartInfo     詳細な再生パラメータです。
    //!
    //! @return   再生準備処理結果を @ref StartResult 型で返します。
    //!
    //! @see SoundHandle クラス
    //! @see StartResult
    //! @see StartSound
    //! @see StartInfo
    //!
    //---------------------------------------------------------------------------
    StartResult PrepareSound(
            SoundHandle* pHandle,
            SoundArchive::ItemId soundId,
            const StartInfo* pStartInfo = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  指定したサウンドアーカイブ、 ID のサウンドを再生する準備をします。
    //!
    //!         準備が完了したサウンドは、ハンドルクラスの
    //!         @ref SoundHandle::StartPrepared
    //!         を呼び出して再生を開始することができます。
    //!
    //!         第一引数には、サウンドハンドルを指定します。
    //!         再生に成功したサウンドは、このサウンドハンドルと関連付けられます。
    //!
    //!         第二引数には、再生するサウンドの識別子としてサウンド ID を指定します。
    //!
    //!         soundArchiveName には、対象となるサウンドアーカイブの名前を指定します。
    //!         nullptr を渡した場合は、メインのサウンドアーカイブを選択したことになります。
    //!
    //!         pStartInfo は、再生時に設定できる詳細なパラメータです。
    //!         このパラメータは、設定せずに再生を開始することが可能です。
    //!
    //! @param[in] pHandle          再生されるサウンドと関連付けられるハンドルです。
    //! @param[in] soundId          再生するサウンドの ID です。
    //! @param[in] soundArchiveName 再生対象となるサウンドアーカイブの名前です。
    //! @param[in] pStartInfo       詳細な再生パラメータです。
    //!
    //! @return   再生準備処理結果を @ref StartResult 型で返します。
    //!
    //! @see SoundHandle クラス
    //! @see StartResult
    //! @see StartSound
    //! @see StartInfo
    //!
    //---------------------------------------------------------------------------
    StartResult PrepareSound(
            SoundHandle* pHandle,
            SoundArchive::ItemId soundId,
            const char* soundArchiveName,
            const StartInfo* pStartInfo = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  指定したラベル文字列のサウンドを再生する準備をします。
    //!
    //!         準備が完了したサウンドは、ハンドルクラスの
    //!         @ref SoundHandle::StartPrepared
    //!         を呼び出して再生を開始することができます。
    //!
    //!         第一引数には、サウンドハンドルを指定します。
    //!         再生に成功したサウンドは、このサウンドハンドルと関連付けられます。
    //!
    //!         第二引数には、再生するサウンドの識別子として、
    //!         サウンドのラベル文字列を指定します。
    //!
    //!         pStartInfo は、再生時に設定できる詳細なパラメータです。
    //!         このパラメータは、設定せずに再生を開始することが可能です。
    //!
    //! @param[in] pHandle        再生されるサウンドと関連付けられるハンドルです。
    //! @param[in] pSoundName     再生するサウンドのラベル文字列です。
    //! @param[in] pStartInfo     詳細な再生パラメータです。
    //!
    //! @return   再生準備処理結果を @ref StartResult 型で返します。
    //!
    //! @see SoundHandle クラス
    //! @see StartResult
    //! @see StartSound
    //! @see StartInfo
    //!
    //---------------------------------------------------------------------------
    StartResult PrepareSound(
            SoundHandle* pHandle,
            const char* pSoundName,
            const StartInfo* pStartInfo = NULL ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  指定したサウンドアーカイブ、ラベル文字列のサウンドを再生する準備をします。
    //!
    //!         準備が完了したサウンドは、ハンドルクラスの
    //!         @ref SoundHandle::StartPrepared
    //!         を呼び出して再生を開始することができます。
    //!
    //!         第一引数には、サウンドハンドルを指定します。
    //!         再生に成功したサウンドは、このサウンドハンドルと関連付けられます。
    //!
    //!         第二引数には、再生するサウンドの識別子として、
    //!         サウンドのラベル文字列を指定します。
    //!
    //!         soundArchiveName には、対象となるサウンドアーカイブの名前を指定します。
    //!         nullptr を渡した場合は、メインのサウンドアーカイブを選択したことになります。
    //!
    //!         pStartInfo は、再生時に設定できる詳細なパラメータです。
    //!         このパラメータは、設定せずに再生を開始することが可能です。
    //!
    //! @param[in] pHandle        再生されるサウンドと関連付けられるハンドルです。
    //! @param[in] pSoundName     再生するサウンドのラベル文字列です。
    //! @param[in] soundArchiveName 再生対象となるサウンドアーカイブの名前です。
    //! @param[in] pStartInfo     詳細な再生パラメータです。
    //!
    //! @return   再生準備処理結果を @ref StartResult 型で返します。
    //!
    //! @see SoundHandle クラス
    //! @see StartResult
    //! @see StartSound
    //! @see StartInfo
    //!
    //---------------------------------------------------------------------------
    StartResult PrepareSound(
            SoundHandle* pHandle,
            const char* pSoundName,
            const char* soundArchiveName,
            const StartInfo* pStartInfo = NULL ) NN_NOEXCEPT;
    //@}

protected:
    //! @internal
    //! @param handle :private
    //! @param soundId :private
    //! @param holdFlag :private
    //! @param startInfo :private
    //! @param soundArchiveName :private
    //! @return :private
    virtual StartResult detail_SetupSound(
        SoundHandle* handle,
        uint32_t soundId,
        bool holdFlag,
        const char* soundArchiveName,
        const StartInfo* startInfo
    ) NN_NOEXCEPT = 0;
    //! @internal
    //! @param pString :private
    //! @return :private
    virtual SoundArchive::ItemId detail_GetItemId( const char* pString ) NN_NOEXCEPT = 0;

    //! @internal
    //! @param pString :private
    //! @param soundArchiveName :private
    //! @return :private
    virtual SoundArchive::ItemId detail_GetItemId( const char* pString, const char* soundArchiveName ) NN_NOEXCEPT = 0;
};

} // namespace nn::atk
} // namespace nn
