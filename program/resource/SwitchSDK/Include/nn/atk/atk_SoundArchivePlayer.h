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
 * :include nn/atk/atk_SoundArchivePlayer.h
 *
 * @file atk_SoundArchivePlayer.h
 */

#pragma once

#include <nn/atk/atk_SoundStartable.h>
#include <nn/atk/atk_NoteOnCallback.h>
#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_BasicSound.h>
#include <nn/atk/atk_SequenceSoundPlayer.h> // SequenceUserProcCallback
#include <nn/atk/atk_SequenceSound.h>       // SequenceSoundInstanceManager
#include <nn/atk/atk_StreamSound.h>         // StreamSoundInstanceManager
#include <nn/atk/atk_Util.h>
#include <nn/atk/atk_SoundInstanceManager.h>
#include <nn/atk/atk_MmlSequenceTrackAllocator.h>
#include <nn/atk/atk_MmlParser.h>
#include <nn/atk/atk_SequenceSoundRuntime.h>
#include <nn/atk/atk_WaveSoundRuntime.h>
#include <nn/atk/atk_StreamSoundRuntime.h>
#include <nn/atk/atk_Result.h>

#include <nn/atk/detail/atk_AdvancedWaveSoundRuntime.h>
#include <nn/atk/detail/atk_SoundArchiveManager.h>

namespace nn {
namespace atk {

class SoundHandle;
class SoundHeap;
class SoundActor;
class SoundPlayer;
class SoundDataManager;

namespace detail {

class ExternalSoundPlayer;
class Channel;
class StreamSound;
class WaveSound;
class PlayerHeap;
class SoundArchiveFilesHook;
class SoundArchiveManager;

}

//---------------------------------------------------------------------------
//! @brief  サウンドアーカイブ中のサウンドを再生するクラスです。
//!
//!         SoundArchivePlayer で再生したサウンドは、
//!         ハンドルクラスを用いて操作することができます。
//!
//!         ダウンロード販売・追加コンテンツ・パッチなどでサウンドアーカイブファイル
//!         (*.bfsar) やストリームサウンドファイル (*.bfstm) を SD カードに配置する場合は、
//!         次の点に注意してください。
//!
//!         SD カード上にあるストリームサウンドファイル (*.bfstm) を再生したり、
//!         SD カード上にあるサウンドアーカイブファイル (*.bfsar) の中のデータを
//!         プレイヤーヒープを使って頻繁に再生すると、
//!         場合によっては SD カードの「データ化け」を引き起こす可能性があります。
//!
//!         「データ化け」を抑制するには、
//!         「頻繁に再生する音をプレイヤーヒープを使って鳴らさない」
//!         「ストリームサウンド再生用にファイルリードキャッシュを利用するようにする」
//!         などの方法があります。
//!
//!         ストリームサウンド再生用のファイルリードキャッシュについては、
//!         @ref InitializeParam 構造体を渡す @ref Initialize 関数の説明をご確認ください。
//!
//! @see SoundHandle クラス
//! @see SoundStartable インターフェイス
//!
//---------------------------------------------------------------------------
class SoundArchivePlayer : public SoundStartable
{
public:
    //---------------------------------------------------------------------------
    //! @brief  サウンドアーカイブプレイヤーの初期化に使用する構造体です。
    //!
    //!         本構造体を引数に取る @ref Initialize 関数を利用する際に、ご使用ください。
    //!
    //!         本構造体を利用した @ref Initialize 関数を呼ぶことで、
    //!         ストリームサウンド再生時のファイルリードキャッシュが有効になり、
    //!         ファイルシステムへのアクセス頻度を制御することができます。
    //!
    //!         userParamSizePerSound については、
    //!         @ref Initialize の説明も参照してください。
    //!
    //! @see Initialize
    //! @see GetRequiredMemSize
    //! @see GetRequiredStreamBufferSize
    //! @see GetRequiredStreamCacheSize
    //!
    //---------------------------------------------------------------------------
    struct InitializeParam
    {
        //! サウンドアーカイブプレイヤーで使用するサウンドアーカイブです。
        const SoundArchive* pSoundArchive;

        //! サウンドアーカイブプレイヤーで使用するサウンドデータマネージャです。
        const SoundDataManager* pSoundDataManager;

        //! サウンドアーカイブプレイヤー内で生成するインスタンスやプレイヤーヒープに使われるバッファです。
        //! @ref GetRequiredMemSize で得られたサイズのメモリ領域を渡す必要があります。
        //! BufferAlignSize バイト境界に配置されている必要があります。
        //!
        //! ストリームサウンドのインスタンス用のバッファを指定する機能を利用するときは
        //! @ref InitializeParam を引数にとる @ref GetRequiredMemSize で得られたサイズのメモリ領域を渡す必要があります。
        //! このとき、最大 nn::audio::MemoryPoolType::SizeGranularity サイズのメモリ領域を節約することができます。
        //! 詳しい説明は @ref GetRequiredStreamInstanceSize を参照してください。
        void* pSetupBuffer;

        //! pSetupBuffer のサイズです。
        //! @ref GetRequiredMemSize で得られたサイズを渡す必要があります。
        size_t setupBufferSize;

        //! ストリームサウンド再生に使われるバッファです。
        //! @ref GetRequiredStreamBufferSize で得られたサイズか、その @ref StreamBufferTimesMax 倍までのサイズのメモリ領域を渡す必要があります。
        //! GetRequiredStreamBufferSize で得られたサイズが 0 の場合は、NULL を指定します。
        //! nn::audio::BufferAlignSize バイト境界に配置されている必要があります。
        //! バッファはメモリプール管理される必要があります。
        void* pStreamBuffer;

        //! pStreamBuffer のサイズです。
        //! @ref GetRequiredStreamBufferSize で得られたサイズか、その @ref StreamBufferTimesMax 倍までのサイズを渡す必要があります。
        size_t streamBufferSize;

        //! ストリームサウンド再生時のファイルリードキャッシュ用のバッファです。
        //! @ref GetRequiredStreamCacheSize で得られたサイズを渡す必要があります。
        //! FsSoundArchive::BufferAlignSize バイト境界に配置されている必要があります。
        void* pStreamCacheBuffer;

        //! pStreamCacheBuffer のサイズです。
        //! @ref GetRequiredStreamCacheSize で得られたサイズを渡す必要があります。
        size_t streamCacheSize;

        //! ストリームサウンドのインスタンス用バッファを pSetupBuffer の一部を用いて、
        //! ライブラリが自動的に設定するかどうかを表すフラグです。初期値は true です。
        //! フラグを false に設定し、インスタンス用バッファを設定することで、使用メモリと初期化, 終了処理の時間を節約することができます。
        //! 詳しい説明は @ref GetRequiredStreamInstanceSize を参照してください。
        bool enablePreparingStreamInstanceBufferFromSetupBuffer;

        //! ストリームサウンドのインスタンス用のバッファです。
        //! @ref GetRequiredStreamInstanceSize で得られたサイズを渡す必要があります。
        //! nn::audio::BufferAlignSize バイト境界に配置されている必要があります。
        //! バッファはメモリプール管理される必要があります。
        //! バッファの用法は @ref GetRequiredStreamInstanceSize を参照してください。
        void* pStreamInstanceBuffer;

        //! pStreamInstanceBuffer のサイズです。
        //! @ref GetRequiredStreamInstanceSize で得られたサイズを渡す必要があります。
        size_t streamInstanceBufferSize;

        //! 1サウンドあたりのユーザーパラメータのサイズです。
        size_t userParamSizePerSound;

        //! プレイヤーに関連付けられる追加サウンドアーカイブ数の最大値です。
        int addonSoundArchiveCount;

        //! @brief コンストラクタです
        InitializeParam() NN_NOEXCEPT
        : pSoundArchive(NULL)
        , pSoundDataManager(NULL)
        , pSetupBuffer(NULL)
        , setupBufferSize(0)
        , pStreamBuffer(NULL)
        , streamBufferSize(0)
        , pStreamCacheBuffer(NULL)
        , streamCacheSize(0)
        , enablePreparingStreamInstanceBufferFromSetupBuffer(true)
        , pStreamInstanceBuffer(NULL)
        , streamInstanceBufferSize(0)
        , userParamSizePerSound(0)
        , addonSoundArchiveCount(0)
        {}
    };

    //---------------------------------------------------------------------------
    //! @briefprivate
    //! @brief ストリームサウンドインスタンスの状態を格納するための構造体です。
    //---------------------------------------------------------------------------
    struct StreamSoundInstanceState
    {
        //! @brief 使用中のストリームサウンドの数です。
        int activeStreamSoundInstanceCount;

        //! @brief 使用中のストリームチャンネルの数です。
        int activeStreamChannelCount;

        //! @brief 使用中のストリームトラックの数です。
        int activeStreamTrackCount;
    };

    //---------------------------------------------------------------------------
    //! @brief      バッファアラインサイズです。
    //---------------------------------------------------------------------------
    static const uint32_t BufferAlignSize = nn::audio::MemoryPoolType::AddressAlignment;

    //---------------------------------------------------------------------------
    //! @brief      @ref GetRequiredStreamBufferTimes で得られるストリームバッファサイズの最大倍率です。
    //!
    //! @see GetRequiredStreamBufferTimes
    //---------------------------------------------------------------------------
    static const uint32_t StreamBufferTimesMax = 4;

    //---------------------------------------------------------------------------
    //! @brief      ユーザーパラメータのサイズ切り上げ境界です。
    //!             Initialize 関数に渡した userParamSizePerSound はこの倍数境界まで切り上げられます。
    //!
    //! @see GetSoundUserParamSize
    //---------------------------------------------------------------------------
    static const uint32_t UserParamBoundary = 4;

    //---------------------------------------------------------------------------
    //! @brief      AddonSoundArchive の名前の長さの最大値です。null 終端を含みます。
    //!
    //! @see AddAddonSoundArchive
    //---------------------------------------------------------------------------
    static const int AddonSoundArchiveNameLengthMax = nn::atk::detail::AddonSoundArchiveContainer::SoundArchiveNameLengthMax;

    /* ------------------------------------------------------------------------
            class member
       ------------------------------------------------------------------------ */
public:
    //----------------------------------------
    //! @name コンストラクタ/デストラクタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief        コンストラクタです。
    //!
    //---------------------------------------------------------------------------
    SoundArchivePlayer() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        デストラクタです。
    //!
    //---------------------------------------------------------------------------
    virtual ~SoundArchivePlayer() NN_NOEXCEPT NN_OVERRIDE;
    //@}

    //----------------------------------------
    //! @name 初期化
    //@{
    //---------------------------------------------------------------------------
    //! @brief      初期化に必要なメモリのサイズを取得します。
    //!
    //! @param[in]    pArchive  プレイヤーで使用するサウンドアーカイブです。
    //!
    //! @return       初期化に必要なメモリサイズを返します。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    static size_t GetRequiredMemSize( const SoundArchive* pArchive) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      初期化に必要なメモリのサイズを取得します。
    //!
    //!             userParamSizePerSound 引数については、
    //!             @ref Initialize の説明も参照してください。
    //!
    //! @param[in]    pArchive  プレイヤーで使用するサウンドアーカイブです。
    //! @param[in]    userParamSizePerSound 各サウンドインスタンスに持たせるユーザーパラメータのバッファサイズです。
    //!
    //! @return       初期化に必要なメモリサイズを返します。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    static size_t GetRequiredMemSize( const SoundArchive* pArchive, size_t userParamSizePerSound ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      初期化に必要なメモリのサイズを取得します。
    //!
    //!             userParamSizePerSound 引数については、
    //!             @ref Initialize の説明も参照してください。
    //!
    //! @param[in]    pArchive  プレイヤーで使用するサウンドアーカイブです。
    //! @param[in]    userParamSizePerSound 各サウンドインスタンスに持たせるユーザーパラメータのバッファサイズです。
    //! @param[in]    addonSoundArchiveCount 使用する追加サウンドアーカイブの数です。
    //!
    //! @return       初期化に必要なメモリサイズを返します。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    static size_t GetRequiredMemSize( const SoundArchive* pArchive, size_t userParamSizePerSound, int addonSoundArchiveCount ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      初期化に必要なメモリのサイズを取得します。
    //!
    //! @param[in]    param    初期化パラメータです。
    //!
    //! @return       初期化に必要なメモリサイズを返します。
    //! @pre
    //! - param.pSoundArchive != NULL
    //!
    //! @see Initialize
    //! @see GetRequiredStreamInstanceSize
    //!
    //---------------------------------------------------------------------------
    static size_t GetRequiredMemSize( const InitializeParam& param ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      ストリームサウンドのインスタンスに必要なメモリのサイズを取得します。
    //!
    //!             サウンドアーカイブ (*.bfsar) で定義されているストリームサウンドの数
    //!             (SoundMaker の [プロジェクトの設定] - [サウンドアーカイブ] - [ストリームサウンドの数]
    //!             で指定します) に比例したサイズを返します。
    //!
    //!             ストリームサウンドのインスタンスは通常、@ref InitializeParam::pSetupBuffer に指定したバッファの一部を用いて初期化されます。
    //!             そのインスタンス用のバッファはメモリプール管理される必要があるため、ライブラリは内部でメモリプールの管理を行っています。
    //!
    //!             @ref InitializeParam::pStreamInstanceBuffer にメモリプール管理されるバッファを指定することで、
    //!             インスタンス用のバッファのメモリプール管理をライブラリ側ではなく、ユーザ側で行うことができるようになります。
    //!             これにより、最大 nn::audio::MemoryPoolType::SizeGranularity のメモリサイズと、
    //!             初期化時と終了時にそれぞれ最大 1 オーディオフレームの時間を短縮することができます。
    //!
    //!             この機能を使用するときは、@ref InitializeParam::enablePreparingStreamInstanceBufferFromSetupBuffer を false に設定し、
    //!             @ref InitializeParam::pStreamInstanceBuffer にこの関数で得られるサイズのメモリプール管理されるバッファを指定し、
    //!             @ref InitializeParam::pSetupBuffer に @ref InitializeParam を引数にとる @ref GetRequiredMemSize で
    //!             得られるサイズのバッファを指定してください。
    //!
    //! @param[in]    pArchive  プレイヤーで使用するサウンドアーカイブです。
    //!
    //! @return       ストリームサウンドのインスタンスに必要なメモリサイズを返します。
    //!
    //! @see Initialize
    //! @see InitializeParam 構造体
    //!
    //---------------------------------------------------------------------------
    static size_t GetRequiredStreamInstanceSize( const nn::atk::SoundArchive* pArchive ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        初期化に必要なストリームバッファのサイズを取得します。
    //!
    //!               @ref Initialize に渡すストリームバッファの最小サイズを取得します。
    //!               この関数で取得した値より大きいサイズ (StreamBufferTimesMax 倍まで可)
    //!               のバッファを渡す必要があります。
    //!
    //! @param[in]    pArchive             プレイヤーで使用するサウンドアーカイブです。
    //!
    //! @return       初期化に必要なストリームバッファサイズを返します。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    size_t GetRequiredStreamBufferSize( const SoundArchive* pArchive ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        初期化に必要なストリームバッファサイズの倍率を取得します。
    //!
    //!               @ref Initialize に渡すストリームバッファについて、
    //!               @ref GetRequiredStreamBufferSize で得られる最小サイズの何倍が必要かを取得します。
    //!               この関数で取得した倍率より大きいサイズのバッファを渡す必要があり、
    //!               不足している場合はプリフェッチデータを一部活用できない場合があります。
    //!
    //! @param[in]    pArchive             プレイヤーで使用するサウンドアーカイブです。
    //!
    //! @return       初期化に必要なストリームバッファサイズの倍率を返します。
    //!
    //! @see Initialize
    //! @see GetRequiredStreamBufferSize
    //!
    //---------------------------------------------------------------------------
    int GetRequiredStreamBufferTimes( const SoundArchive* pArchive ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  サウンドアーカイブプレイヤーを初期化します。
    //!
    //!         サウンドアーカイブプレイヤーを使用する前に、
    //!         初期化を行う必要があります。
    //!         初期化を行うことにより、
    //!         サウンドアーカイブプレイヤーがサウンドアーカイブと
    //!         サウンドデータマネージャ関連付けられ、
    //!         そのサウンドアーカイブ中のデータを再生できるようになります。
    //!
    //!         サウンドアーカイブプレイヤーが必要とするメモリのサイズは
    //!         @ref GetRequiredMemSize 及び @ref GetRequiredStreamBufferSize
    //!         で取得することができます。
    //!
    //!         必要なストリームバッファサイズが 0 の場合、
    //!         ストリームバッファへのポインタ pStreamBuffer に NULL を渡すことができます。
    //!
    //!         ストリームバッファを @ref GetRequiredStreamBufferSize
    //!         で取得したサイズより大きく設定すると、
    //!         ストリームサウンドの音途切れが発生しにくくなりますが、
    //!         データをロードしてから実際に再生するまでの遅延時間が大きくなります。
    //!
    //!         ストリームバッファは、@ref GetRequiredStreamBufferSize で取得したサイズの
    //!         @ref StreamBufferTimesMax 倍まで設定することができます。
    //!         @ref StreamBufferTimesMax 倍より大きなサイズを設定すると、Debug 版 / Develop 版ではアサートで
    //!         停止します。Release 版での挙動は不定です。
    //!
    //!         userParamSizePerSound 引数を使うと、
    //!         各サウンドインスタンスにユーザーパラメータ (ユーザーが自由に読み書きできる領域)
    //!         を持たせることができます。
    //!
    //!         この「ユーザーパラメータ」は、SoundMaker で設定する「ユーザーパラメータ」とは異なります。
    //!         SoundMaker で設定する「ユーザーパラメータ」は各サウンド ID にひも付けられるパラメータで
    //!         @ref SoundArchive::ReadSoundUserParam で取得することができるリードオンリーのデータです。
    //!         一方、ここで扱う「ユーザーパラメータ」は、
    //!         各サウンドインスタンスにひも付けられるパラメータで、
    //!         プログラム内で読み書きできるという違いがあります。
    //!
    //!         また、SoundMaker のユーザーパラメータは uint32_t を 4 つまでしか保持できませんが、
    //!         こちらは、userParamSizePerSound 引数で渡したサイズだけ値を保持することが可能です。
    //!
    //!         userParamSizePerSound に 1 以上の値を渡して初期すると、サウンド再生時に
    //!         @ref SoundHandle::GetUserParam 関数を使うことで、
    //!         ユーザーパラメータを取得することができます。
    //!         userParamSizePerSound に 0 を渡すと、SoundHandle:*GetUserParam は NULL が返ります。
    //!         ユーザーパラメータは、サウンド再生開始時にゼロクリアされます。
    //!
    //!         各サウンドインスタンスが保持しているユーザーパラメータの実際のサイズは、
    //!         @ref GetSoundUserParamSize で取得することができます。
    //!
    //!         userParamSizePerSound には、@ref GetRequiredMemSize の userParamSizePerSound
    //!         引数と同じ値を渡す必要があります。
    //!
    //! @param[in]    pArchive               プレイヤーで使用するサウンドアーカイブです。
    //! @param[in]    pManager               プレイヤーで使用するサウンドデータマネージャです。
    //! @param[in]    pBuffer                バッファへのポインタです。
    //! @param[in]    bufferSize             バッファサイズです。
    //! @param[in]    pStreamBuffer          ストリームバッファへのポインタです。
    //! @param[in]    streamBufferSize       ストリームバッファのサイズです。
    //! @param[in]    userParamSizePerSound  1サウンドあたりのユーザーパラメータのサイズです。
    //!
    //! @return       初期化に成功したら true を、失敗したら false を返します。
    //! @pre
    //! - pBuffer は @ref BufferAlignSize バイトにアラインしている
    //! - pStreamBuffer は nn::audio::BufferAlignSize にアラインしている
    //! - pArchive != nullptr
    //! - pManager != nullptr
    //! - pBuffer != nullptr
    //! - @ref SoundSystem が初期化済み
    //!
    //! @see GetRequiredMemSize
    //! @see GetRequiredStreamBufferSize
    //! @see Finalize
    //! @see IsAvailable
    //!
    //---------------------------------------------------------------------------
    bool Initialize(
        const SoundArchive* pArchive,
        const SoundDataManager* pManager,
        void*   pBuffer,
        size_t  bufferSize,
        void*   pStreamBuffer,
        size_t  streamBufferSize,
        size_t  userParamSizePerSound = 0
    ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      ストリームサウンド再生時のファイルリードキャッシュ用のバッファサイズを取得します。
    //!
    //!             サウンドアーカイブ (*.bfsar) で定義されているストリームサウンドの数
    //!             (SoundMaker の [プロジェクトの設定] - [サウンドアーカイブ] - [ストリームサウンドの数]
    //!             で指定します) に比例したサイズを返します。
    //!
    //!             cacheSizePerSound に渡すサイズの目安は下記のとおりです。
    //!
    //!             - モノラルのストリームサウンドしか無い場合は、8KB の倍数
    //!             - ステレオ～ (マルチトラックストリームを含む) がある場合は、16KB の倍数
    //!
    //!             キャッシュを利用することで、以下のメリット・デメリットがあります。
    //!
    //!             ■メリット @n
    //!             - ストリームサウンドファイルが SD カード上にある場合は、
    //!               SD カードへのアクセス回数が減るためデータ化けが起こる可能性を抑制できる
    //!             - ストリーム再生時の音途切れを緩和できる
    //!               (次のステージのデータを裏読みしたり、ムービーを同時に再生したり、
    //!                複数のストリームサウンドを同時に再生したりするケースで音途切れを緩和できます)
    //!
    //!             ■デメリット @n
    //!             - キャッシュはすべてのストリームサウンドインスタンスに対して設定されるため、
    //!               キャッシュを必要としないサウンドに対してもキャッシュが設定され、メモリを余計に消費する
    //!
    //! @param[in]  pArchive            サウンドアーカイブプレイヤーで利用するサウンドアーカイブです。
    //! @param[in]  cacheSizePerSound   1 ストリームサウンドあたりのファイルリードキャッシュサイズです。
    //!
    //! @return     ストリームサウンド再生時のファイルリードキャッシュ用のバッファサイズを返します。
    //!
    //! @see InitializeParam 構造体
    //!
    //---------------------------------------------------------------------------
    static size_t GetRequiredStreamCacheSize(const SoundArchive* pArchive, size_t cacheSizePerSound) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      サウンドアーカイブプレイヤーを初期化します。
    //!
    //!             サウンドアーカイブプレイヤーを使用する前に、初期化を行う必要があります。
    //!             初期化を行うことによりサウンドアーカイブプレイヤーが、
    //!             サウンドアーカイブとサウンドデータマネージャが関連付けられ、
    //!             サウンドアーカイブ中のデータを再生できるようになります。
    //!
    //!             サウンドアーカイブプレイヤーで必要とするメモリのサイズは、
    //!             下記の API で求めることができます。
    //!
    //!             - @ref GetRequiredMemSize
    //!             - @ref GetRequiredStreamBufferSize
    //!             - @ref GetRequiredStreamCacheSize
    //!             - @ref GetRequiredStreamInstanceSize
    //!
    //!             上記の API で取得したメモリサイズと、該当するサイズのメモリ領域を
    //!             @ref InitializeParam に格納して、本関数に渡します。
    //!             ここで @ref GetRequiredStreamCacheSize と @ref GetRequiredStreamInstanceSize は
    //!             ファイルリードキャッシュ機能やストリームインスタンスのバッファを指定する機能を
    //!             使用するときだけ設定する点にご注意ください。
    //!
    //!             ストリームバッファを @ref GetRequiredStreamBufferSize
    //!             で取得したサイズより大きく設定すると、
    //!             ストリームサウンドの音途切れが発生しにくくなりますが、
    //!             データをロードしてから実際に再生するまでの遅延時間が大きくなります。
    //!
    //!             ストリームバッファは、@ref GetRequiredStreamBufferSize で取得したサイズの
    //!             @ref StreamBufferTimesMax 倍まで設定することができます。
    //!             @ref StreamBufferTimesMax 倍より大きなサイズを設定すると、Debug 版 / Development 版ではアサートで
    //!             停止します。Release 版での挙動は不定です。
    //!
    //!             ファイルリードキャッシュを適切に設定することで、ストリームサウンド再生時の
    //!             ファイルシステムへのアクセス頻度を減らすことができます。
    //!             キャッシュ用のバッファサイズについては @ref GetRequiredStreamCacheSize
    //!             の説明をご参照ください。
    //!
    //!             ストリームインスタンスのバッファを設定することで、@ref Initialize と @ref Finalize にかかる時間を
    //!             最大で 1 オーディオフレーム減らすことができます。
    //!             また、ライブラリから要求されるバッファが合計で最大 nn::audio::MemoryPoolType::SizeGranularity サイズ小さくなります。
    //!             ストリームインスタンスのバッファの設定については @ref GetRequiredStreamInstanceSize の説明を参照ください。
    //!
    //! @param[in] param    初期化パラメータです。
    //!
    //! @see InitializeParam 構造体
    //! @see GetRequiredMemSize
    //! @see GetRequiredStreamBufferSize
    //! @see GetRequiredStreamCacheSize
    //! @see GetRequiredStreamInstanceSize
    //! @see Finalize
    //! @see IsAvailable
    //!
    //! @return     初期化に成功したら true を、失敗したら false を返します。
    //!
    //---------------------------------------------------------------------------
    bool Initialize(const InitializeParam& param) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        サウンドアーカイブプレイヤーを破棄します。
    //!
    //!               破棄は以下のとおり行われます。
    //!
    //!               - このサウンドアーカイブプレイヤーを使って再生中のサウンドを停止します。
    //!               - 初期化で割り当てたメモリ領域を開放します。
    //!               - 初期化されたサウンドアーカイブとの関連が外れます。
    //!               - サウンドインスタンスが利用したメモリプールを解放します
    //!
    //! @see Initialize
    //! @see IsAvailable
    //!
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief        利用可能な状態かどうかを調べます。
    //!
    //!               サウンドアーカイブプレイヤーを利用するためには
    //!               @ref Initialize を呼び出して初期化を完了させる必要があります。
    //!               また、@ref Finalize を呼び出すと、
    //!               サウンドアーカイブプレイヤーの終了処理が行われ、
    //!               利用できない状態になります。
    //!
    //! @return       サウンドアーカイブプレイヤーが利用可能な状態なら true を、
    //!               そうでなければ false を返します。
    //!
    //! @see Initialize
    //! @see Finalize
    //!
    //---------------------------------------------------------------------------
    bool IsAvailable() const NN_NOEXCEPT;
    //@}

    //----------------------------------------
    //! @name 更新
    //---------------------------------------------------------------------------
    //! @brief    サウンドアーカイブプレイヤーの更新処理を行います。
    //!
    //!           ビデオフレームごとに呼び出す必要はありませんが、
    //!           1 ゲームフレームに 1 回など、定期的に呼び出してください。
    //!
    //!           この関数では、プレイヤーやサウンドに対するパラメータ設定や、
    //!           フェイドイン・フェイドアウトなどの連続的なパラメータ変更を
    //!           反映するための処理が行われるため、呼び出し頻度が変えると、
    //!           フェードインなどの時間が変わってしまいます。
    //!
    //! @see SoundPlayer::StopAllSound
    //! @see SoundPlayer::PauseAllSound
    //! @see SoundActor::StopAllSound
    //! @see SoundActor::PauseAllSound
    //! @see Sound3DActor::StopAllSound
    //! @see Sound3DActor::PauseAllSound
    //! @see SoundHandle::Stop
    //! @see SoundHandle::Pause
    //! @see SoundHandle::FadeIn
    //! @see SoundHandle::SetVolume
    //! @see SoundHandle::GetRemainingFadeFrames
    //! @see SoundHandle::GetRemainingPauseFadeFrames
    //! @see StreamSoundHandle::Stop
    //! @see StreamSoundHandle::Pause
    //! @see StreamSoundHandle::FadeIn
    //! @see StreamSoundHandle::SetVolume
    //! @see StreamSoundHandle::SetTrackVolume
    //! @see WaveSoundHandle::Stop
    //! @see WaveSoundHandle::Pause
    //! @see WaveSoundHandle::FadeIn
    //! @see WaveSoundHandle::SetVolume
    //! @see SequenceSoundHandle::Stop
    //! @see SequenceSoundHandle::Pause
    //! @see SequenceSoundHandle::FadeIn
    //! @see SequenceSoundHandle::SetVolume
    //!
    //---------------------------------------------------------------------------
    void Update() NN_NOEXCEPT;
    //@}

    //----------------------------------------
    //! @name インスタンス取得

    //---------------------------------------------------------------------------
    //! @brief    プレイヤーに関連付けられているサウンドアーカイブを取得します。
    //!
    //! @return   プレイヤーに関連付けられているサウンドアーカイブを返します。
    //!
    //! @see SoundArchive クラス
    //!
    //---------------------------------------------------------------------------
    const SoundArchive& GetSoundArchive() const NN_NOEXCEPT;

    //@}
    //---------------------------------------------------------------------------
    //! @internal
    //! @brief    サウンドアーカイブマネージャを取得します。
    //!
    //!           内部で使用するためのアクセサです。サウンドアーカイブマネージャを直接使用すると
    //!           意図しない挙動になる可能性がありますので、この関数は使用しないでください。
    //!
    //! @return   サウンドアーカイブマネージャを返します。
    //---------------------------------------------------------------------------
    nn::atk::detail::SoundArchiveManager& GetSoundArchiveManager() NN_NOEXCEPT
    {
        return m_SoundArchiveManager;
    }
    //----------------------------------------
    //! @name インスタンス取得

    //---------------------------------------------------------------------------
    //! @brief     プレイヤーに関連付けられている追加サウンドアーカイブを取得します。
    //!
    //!            指定した名前に関連付けられている追加サウンドアーカイブを取得します。
    //!            nullptr を指定した場合や登録されていない名前を指定した場合には
    //!            nullptr が返ります。
    //!
    //! @param[in] soundArchiveName 識別用の名前を指定します。
    //!
    //! @return    指定した名前に関連づけられている追加サウンドアーカイブを返します。
    //!
    //! @see AddAddonSoundArchive
    //!
    //---------------------------------------------------------------------------
    const AddonSoundArchive* GetAddonSoundArchive(const char* soundArchiveName) const NN_NOEXCEPT;

    //! @briefprivate
    nn::os::Tick GetAddonSoundArchiveLastAddTick() const NN_NOEXCEPT
    {
        return m_AddonSoundArchiveLastAddTick;
    }

    //---------------------------------------------------------------------------
    //! @brief     プレイヤーに関連付けられている追加サウンドアーカイブの数を取得します。
    //!
    //! @return    プレイヤーに関連付けられている追加サウンドアーカイブの数を返します。
    //!
    //! @see AddAddonSoundArchive
    //!
    //---------------------------------------------------------------------------
    int GetAddonSoundArchiveCount() const NN_NOEXCEPT
    {
        return m_SoundArchiveManager.GetAddonSoundArchiveCount();
    }

    //---------------------------------------------------------------------------
    //! @brief     プレイヤーに関連付けられている追加サウンドアーカイブを取得します。
    //!
    //!            指定したインデックスの追加サウンドアーカイブを取得します。
    //!            登録されていないインデックスを指定した場合には nullptr が返ります。
    //!
    //! @param[in] index 追加サウンドアーカイブのインデックスです。
    //!
    //! @return    インデックスに関連づけられている追加サウンドアーカイブを返します。
    //!
    //! @see AddAddonSoundArchive
    //!
    //---------------------------------------------------------------------------
    const AddonSoundArchive* GetAddonSoundArchive(int index) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     プレイヤーに関連付けられている追加サウンドアーカイブの名前を取得します。
    //!
    //!            指定したインデックスの追加サウンドアーカイブの名前を取得します。
    //!            登録されていないインデックスを指定した場合には nullptr が返ります。
    //!
    //! @param[in] index 追加サウンドアーカイブのインデックスです。
    //!
    //! @return    インデックスに関連づけられている追加サウンドアーカイブの名前を返します。
    //!
    //! @see AddAddonSoundArchive
    //!
    //---------------------------------------------------------------------------
    const char* GetAddonSoundArchiveName(int index) const NN_NOEXCEPT;

    //! @briefprivate
    nn::os::Tick GetAddonSoundArchiveAddTick(int index) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief     プレイヤーに関連付けられている追加サウンドアーカイブ用のサウンドデータマネージャを取得します。
    //!
    //!            指定した名前に関連付けられている追加サウンドアーカイブ用のサウンドデータマネージャを取得します。
    //!            nullptr を指定した場合や登録されていない名前を指定した場合には
    //!            nullptr が返ります。
    //!
    //! @param[in] soundArchiveName 識別用の名前を指定します。
    //!
    //! @return    指定した名前に関連づけられている追加サウンドアーカイブ用のサウンドデータマネージャを返します。
    //!
    //! @see AddAddonSoundArchive
    //! @see SoundDataManager クラス
    //!
    //---------------------------------------------------------------------------
    const SoundDataManager* GetAddonSoundDataManager(const char* soundArchiveName) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    指定した ID のサウンドプレイヤーを取得します。
    //!
    //! @param[in]    playerId    プレイヤーの ID です。
    //!
    //! @return   指定した ID のサウンドプレイヤーを返します。
    //!
    //! @see SoundPlayer クラス
    //!
    //---------------------------------------------------------------------------
    SoundPlayer& GetSoundPlayer( SoundArchive::ItemId playerId ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    指定した ID のサウンドプレイヤーを取得します。
    //!
    //! @param[in]    pPlayerName     プレイヤーのラベル文字列です。
    //!
    //! @return   指定した ID のサウンドプレイヤーを返します。
    //!
    //! @see SoundPlayer クラス
    //!
    //---------------------------------------------------------------------------
    SoundPlayer& GetSoundPlayer( const char* pPlayerName ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    指定した ID のサウンドプレイヤーを取得します。
    //!
    //! @param[in]    playerId    プレイヤーの ID です。
    //!
    //! @return   指定した ID のサウンドプレイヤーを返します。
    //!
    //! @see SoundPlayer クラス
    //!
    //---------------------------------------------------------------------------
    const SoundPlayer& GetSoundPlayer( SoundArchive::ItemId playerId ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    指定した ID のサウンドプレイヤーを取得します。
    //!
    //! @param[in]    pPlayerName     プレイヤーのラベル文字列です。
    //!
    //! @return   指定した ID のサウンドプレイヤーを返します。
    //!
    //! @see SoundPlayer クラス
    //!
    //---------------------------------------------------------------------------
    const SoundPlayer& GetSoundPlayer( const char* pPlayerName ) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  各サウンドインスタンスが保持するユーザーパラメータのサイズを取得します。
    //!
    //!         ユーザーパラメータについては、
    //!         @ref Initialize の説明を参照してください。
    //!
    //!         @ref Initialize 関数に渡した userParamSizePerSound を @ref UserParamBoundary の倍数境界まで
    //!         切り上げた値が返ります。
    //!
    //! @return ユーザーパラメータのサイズを返します。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    size_t GetSoundUserParamSize() const NN_NOEXCEPT { return m_SoundUserParamSize; }
    //@}

    //----------------------------------------
    //! @name インスタンス数取得

    //---------------------------------------------------------------------------
    //! @brief    サウンドプレイヤーの個数を取得します。
    //!
    //! @return   サウンドプレイヤーの個数を返します。
    //!
    //! @see SoundPlayer クラス
    //!
    //---------------------------------------------------------------------------
    uint32_t GetSoundPlayerCount() const NN_NOEXCEPT { return m_SoundPlayerCount; }

    //---------------------------------------------------------------------------
    //! @brief    再生可能なシーケンスサウンドの残数を取得します。
    //!
    //!           残数が 0 のときに新たにシーケンスサウンドを再生すると、
    //!           既に再生中の最もプレイヤープライオリティの低いサウンドが停止します。
    //!           ただし、新たに再生するサウンドのほうがプレイヤープライオリティが低い場合は、
    //!           再生に失敗します。
    //!
    //!           再生可能なシーケンスサウンドの総数は、
    //!           サウンドアーカイブに記述されていて、
    //!           @ref Initialize でサウンドアーカイブプレイヤーに設定されます。
    //!
    //! @return   再生可能なシーケンスサウンドの残数を返します。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    int GetFreeSequenceSoundCount() const NN_NOEXCEPT
    {
        return m_SequenceSoundRuntime.GetFreeCount();
    }

    //---------------------------------------------------------------------------
    //! @brief    再生可能なウェーブサウンドの残数を取得します。
    //!
    //!           残数が 0 のときに新たにウェーブサウンドを再生すると、
    //!           既に再生中の最もプレイヤープライオリティの低いサウンドが停止します。
    //!           ただし、新たに再生するサウンドのほうがプレイヤープライオリティが低い場合は、
    //!           再生に失敗します。
    //!
    //!           再生可能なウェーブサウンドの総数は、
    //!           サウンドアーカイブに記述されていて、
    //!           @ref Initialize でサウンドアーカイブプレイヤーに設定されます。
    //!
    //! @return   再生可能なウェーブサウンドの残数を返します。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    int GetFreeWaveSoundCount() const NN_NOEXCEPT
    {
        return m_WaveSoundRuntime.GetFreeWaveSoundCount();
    }

    //---------------------------------------------------------------------------
    //! @brief    再生可能なストリームサウンドの残数を取得します。
    //!
    //!           残数が 0 のときに新たにストリームサウンドを再生すると、
    //!           既に再生中の最もプレイヤープライオリティの低いサウンドが停止します。
    //!           ただし、新たに再生するサウンドのほうがプレイヤープライオリティが低い場合は、
    //!           再生に失敗します。
    //!
    //!           再生可能なストリームサウンドの総数は、
    //!           サウンドアーカイブに記述されていて、
    //!           @ref Initialize でサウンドアーカイブプレイヤーに設定されます。
    //!
    //! @return   再生可能なストリームサウンドの残数を返します。
    //!
    //! @see Initialize
    //!
    //---------------------------------------------------------------------------
    int GetFreeStreamSoundCount() const NN_NOEXCEPT
    {
        return m_StreamSoundRuntime.GetFreeCount();
    }
    //! @briefprivate
    //! @brief    再生中のウェーブサウンドの数を取得します。
    //! @return   再生中のウェーブサウンドの数を返します。
    //! @see GetFreeWaveSoundCount
    int GetActiveWaveSoundCount() const NN_NOEXCEPT
    {
        return m_WaveSoundRuntime.GetActiveCount();
    }
    //! @briefprivate
    //! @brief    再生中のシーケンスサウンドの数を取得します。
    //! @return   再生中のシーケンスサウンドの数を返します。
    //! @see GetFreeSequenceSoundCount
    int GetActiveSequenceSoundCount() const NN_NOEXCEPT
    {
        return m_SequenceSoundRuntime.GetActiveCount();
    }
    //! @briefprivate
    //! @brief    再生中のストリームサウンドの数を取得します。
    //! @return   再生中のストリームサウンドの数を返します。
    //! @see GetFreeStreamSoundCount
    int GetActiveStreamSoundCount() const NN_NOEXCEPT
    {
        return m_StreamSoundRuntime.GetActiveCount();
    }
    //@}

    //----------------------------------------
    //! @name シーケンスサウンドカスタマイズ

    //---------------------------------------------------------------------------
    //! @brief  シーケンスコマンド 'userproc' で呼び出される
    //!         コールバック関数を登録します。
    //!
    //!         ここで登録したコールバックは、シーケンスデータ側のシーケンスコマンド
    //!         'userproc' が処理されたフレームで呼び出されます。
    //!         コールバック関数はサウンドスレッド呼び出されます。
    //!
    //!         'userproc' コマンドの詳細については、
    //!         シーケンスデータマニュアルを参照してください。
    //!
    //!         ここで設定したコールバック関数はサウンドスレッドから呼び出されます。
    //!         排他制御が必要な場合は、
    //!         @ref SoundSystem::SoundThreadScopedLock クラスあるいは、
    //!         @ref SoundSystem::LockSoundThread / @ref SoundSystem::UnlockSoundThread 関数を利用する必要があります。
    //!
    //!         コールバック関数はすみやかに処理を終える必要があります。
    //!         処理が長引くと、ノイズが発生する可能性が高くなります。
    //!         たとえば、コールバック関数内でブロックする可能性のある API
    //!         (クリティカルセクションなど) を呼び出すと、
    //!         処理を終えるのに時間がかかる可能性があります。
    //!
    //! @param[in] callback       登録するコールバック関数です。
    //! @param[in] pCallbackArg   コールバック関数に渡されるユーザー引数です。
    //!
    //! @see SequenceUserProcCallback
    //!
    //---------------------------------------------------------------------------
    void SetSequenceUserProcCallback(
            SequenceUserProcCallback callback, void* pCallbackArg ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  シーケンスサウンドの途中再生におけるスキップ処理の空回し幅を指定します。
    //!
    //!         @ref SoundStartable::StartInfo 構造体の startOffset で開始オフセットを指定した上で、
    //!         @ref StartSound を呼び出すと、シーケンスサウンドを途中から再生することができますが、
    //!         シーケンスの空回しのために処理がかかります。
    //!
    //!         デフォルトでは、1 サウンドフレーム (約 5ms) あたり最大
    //!         48 * 4 * 4 tick (シーケンスデータ上の tick です。nn::os::Tick の tick ではありません)
    //!         だけ空回ししますが、この結果サウンドスレッドの処理負荷が上がり、
    //!         音途切れが生じることがあります。
    //!
    //!         この場合、48 * 4 * 4 より小さい値を本関数で設定すると、
    //!         音途切れが緩和する可能性があります。
    //!
    //! @param[in] intervalTick     1 サウンドフレームあたりの最大空回し幅 (単位は tick) です。
    //!
    //! @see GetSequenceSkipIntervalTick
    //---------------------------------------------------------------------------
    static void SetSequenceSkipIntervalTick( int intervalTick ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  シーケンスサウンドの途中再生における、スキップ処理の空回し幅を取得します。
    //!
    //! @return シーケンスサウンドの途中再生における、スキップ処理の空回し幅を返します。
    //!
    //! @see SetSequenceSkipIntervalTick
    //---------------------------------------------------------------------------
    static int GetSequenceSkipIntervalTick() NN_NOEXCEPT;
    //@}

    //---------------------------------------------------------------------------
    //! @brief  ウェーブサウンドの情報を取得します。
    //!
    //!         @ref WaveSoundHandle::ReadWaveSoundDataInfo とは違い、こちらは
    //!         SoundArchivePlayer を初期化し、SoundDataManager にロード済みとなっている
    //!         ウェーブサウンドであれば、任意のタイミングで情報を取得することができます。
    //!
    //! @param[out] pOutInfo    読み取り結果を格納するオブジェクトのポインタを渡します。
    //! @param[in]  waveSoundId 読み取りたいウェーブサウンドのIDを指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidId,指定した ID がアーカイブに存在していない、もしくは、異なるタイプの ID が渡されました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerSoundNotLoaded,指定されたサウンドがロードされていません。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //---------------------------------------------------------------------------
    nn::Result ReadWaveSoundDataInfo(WaveSoundDataInfo* pOutInfo, SoundArchive::ItemId waveSoundId) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ウェーブサウンドの情報を取得します。
    //!
    //!         @ref WaveSoundHandle::ReadWaveSoundDataInfo とは違い、こちらは
    //!         SoundArchivePlayer を初期化し、SoundDataManager にロード済みとなっている
    //!         ウェーブサウンドであれば、任意のタイミングで情報を取得することができます。
    //!
    //!         @ref FsSoundArchive を使用している場合は @ref FsSoundArchive::LoadLabelStringData で
    //!         ラベル文字列データをロードしておく必要があります。
    //!
    //! @param[out] pOutInfo      読み取り結果を格納するオブジェクトのポインタを渡します。
    //! @param[in]  waveSoundName 読み取りたいウェーブサウンドのラベル文字列を指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidName,無効な名前を指定したか、文字列テーブルが読み込まれていません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerSoundNotLoaded,指定されたサウンドがロードされていません。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //!
    //! @see FsSoundArchive
    //---------------------------------------------------------------------------
    nn::Result ReadWaveSoundDataInfo(WaveSoundDataInfo* pOutInfo, const char* waveSoundName) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  追加サウンドアーカイブに存在するウェーブサウンドの情報を取得します。
    //!
    //!         メインのサウンドアーカイブに存在するウェーブサウンドの情報を取得するには、
    //!         引数に pSoundArchive, pSoundDataManager がない関数を使用してください。
    //!
    //!         @ref WaveSoundHandle::ReadWaveSoundDataInfo とは違い、こちらは
    //!         SoundArchivePlayer を初期化し、SoundDataManager にロード済みとなっている
    //!         ウェーブサウンドであれば、任意のタイミングで情報を取得することができます。
    //!
    //! @param[out] pOutInfo          読み取り結果を格納するオブジェクトのポインタを渡します。
    //! @param[in]  waveSoundId       読み取りたいウェーブサウンドのIDを指定します。
    //! @param[in]  pSoundArchive     読み取りたい追加サウンドアーカイブを指定します。
    //! @param[in]  pSoundDataManager 読み取りたい SoundDataManager を指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidId,指定した ID がアーカイブに存在していない、もしくは、異なるタイプの ID が渡されました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerSoundNotLoaded,指定されたサウンドがロードされていません。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //!     - pSoundArchive != nullptr
    //!     - pSoundDataManager != nullptr
    //---------------------------------------------------------------------------
    nn::Result ReadWaveSoundDataInfo(WaveSoundDataInfo* pOutInfo, SoundArchive::ItemId waveSoundId, const SoundArchive* pSoundArchive, const SoundDataManager* pSoundDataManager) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  追加サウンドアーカイブに存在するウェーブサウンドの情報を取得します。
    //!
    //!         メインのサウンドアーカイブに存在するウェーブサウンドの情報を取得するには、
    //!         soundArchiveName に nullptr を指定するか、引数に soundArchiveName がない関数を使用してください。
    //!
    //!         @ref WaveSoundHandle::ReadWaveSoundDataInfo とは違い、こちらは
    //!         SoundArchivePlayer を初期化し、SoundDataManager にロード済みとなっている
    //!         ウェーブサウンドであれば、任意のタイミングで情報を取得することができます。
    //!
    //! @param[out] pOutInfo    読み取り結果を格納するオブジェクトのポインタを渡します。
    //! @param[in]  waveSoundId 読み取りたいウェーブサウンドのIDを指定します。
    //! @param[in]  soundArchiveName 読み取りたい追加サウンドアーカイブの名前を指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidId,指定した ID がアーカイブに存在していない、もしくは、異なるタイプの ID が渡されました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerSoundNotLoaded,指定されたサウンドがロードされていません。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //---------------------------------------------------------------------------
    nn::Result ReadWaveSoundDataInfo(WaveSoundDataInfo* pOutInfo, SoundArchive::ItemId waveSoundId, const char* soundArchiveName) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  追加サウンドアーカイブに存在するウェーブサウンドの情報を取得します。
    //!
    //!         メインのサウンドアーカイブに存在するウェーブサウンドの情報を取得するには、
    //!         soundArchiveName に nullptr を指定するか、引数に soundArchiveName がない関数を使用してください。
    //!
    //!         @ref WaveSoundHandle::ReadWaveSoundDataInfo とは違い、こちらは
    //!         SoundArchivePlayer を初期化し、SoundDataManager にロード済みとなっている
    //!         ウェーブサウンドであれば、任意のタイミングで情報を取得することができます。
    //!
    //! @param[out] pOutInfo      読み取り結果を格納するオブジェクトのポインタを渡します。
    //! @param[in]  waveSoundName 読み取りたいウェーブサウンドのラベル文字列を指定します。
    //! @param[in]  soundArchiveName 読み取りたい追加サウンドアーカイブの名前を指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidName,無効な名前を指定したか、文字列テーブルが読み込まれていません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerSoundNotLoaded,指定されたサウンドがロードされていません。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //---------------------------------------------------------------------------
    nn::Result ReadWaveSoundDataInfo(WaveSoundDataInfo* pOutInfo, const char* waveSoundName, const char* soundArchiveName) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ストリームサウンドの情報を取得します。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfo には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!         ストリームサウンドを再生中に情報を取得する必要がある場合は、
    //!         @ref StreamSoundHandle::ReadStreamSoundDataInfo を使用してください。
    //!
    //! @param[out] pOutInfo      読み取り結果を格納するオブジェクトのポインタを渡します。
    //! @param[in]  streamSoundId 読み取りたいストリームサウンドのIDを指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidFormat,対応していないフォーマットのバイナリが渡されました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //---------------------------------------------------------------------------
    nn::Result ReadStreamSoundDataInfo(StreamSoundDataInfo* pOutInfo, SoundArchive::ItemId streamSoundId) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ストリームサウンドの情報を取得します。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfo には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!         ストリームサウンドを再生中に情報を取得する必要がある場合は、
    //!         @ref StreamSoundHandle::ReadStreamSoundDataInfo を使用してください。
    //!
    //!         @ref FsSoundArchive を使用している場合は @ref FsSoundArchive::LoadLabelStringData で
    //!         ラベル文字列データをロードしておく必要があります。
    //!
    //! @param[out] pOutInfo        読み取り結果を格納するオブジェクトのポインタを渡します。
    //! @param[in]  streamSoundName 読み取りたいストリームサウンドのラベル文字列を指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidName,無効な名前を指定したか、文字列テーブルが読み込まれていません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidFormat,対応していないフォーマットのバイナリが渡されました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //!
    //! @see FsSoundArchive
    //---------------------------------------------------------------------------
    nn::Result ReadStreamSoundDataInfo(StreamSoundDataInfo* pOutInfo, const char* streamSoundName) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  追加サウンドアーカイブに存在するストリームサウンドの情報を取得します。
    //!
    //!         メインのサウンドアーカイブに存在するストリームサウンドの情報を取得するには、
    //!         引数に pSoundArchive がない関数を使用してください。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfo には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!         ストリームサウンドを再生中に情報を取得する必要がある場合は、
    //!         @ref StreamSoundHandle::ReadStreamSoundDataInfo を使用してください。
    //!
    //! @param[out] pOutInfo        読み取り結果を格納するオブジェクトのポインタを渡します。
    //! @param[in]  pSoundArchive   読み取りたい追加サウンドアーカイブを指定します。
    //! @param[in]  streamSoundId   読み取りたいストリームサウンドのIDを指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidName,無効な名前を指定したか、文字列テーブルが読み込まれていません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidFormat,対応していないフォーマットのバイナリが渡されました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //!     - pSoundArchive != nullptr
    //---------------------------------------------------------------------------
    nn::Result ReadStreamSoundDataInfo(StreamSoundDataInfo* pOutInfo, const SoundArchive* pSoundArchive, SoundArchive::ItemId streamSoundId) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  追加サウンドアーカイブに存在するストリームサウンドの情報を取得します。
    //!
    //!         メインのサウンドアーカイブに存在するストリームサウンドの情報を取得するには、
    //!         soundArchiveName に nullptr を指定するか、引数に soundArchiveName がない関数を使用してください。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfo には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!         ストリームサウンドを再生中に情報を取得する必要がある場合は、
    //!         @ref StreamSoundHandle::ReadStreamSoundDataInfo を使用してください。
    //!
    //! @param[out] pOutInfo      読み取り結果を格納するオブジェクトのポインタを渡します。
    //! @param[in]  streamSoundId 読み取りたいストリームサウンドのIDを指定します。
    //! @param[in]  soundArchiveName 読み取りたい追加サウンドアーカイブの名前を指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidFormat,対応していないフォーマットのバイナリが渡されました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //---------------------------------------------------------------------------
    nn::Result ReadStreamSoundDataInfo(StreamSoundDataInfo* pOutInfo, SoundArchive::ItemId streamSoundId, const char* soundArchiveName) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  追加サウンドアーカイブに存在するストリームサウンドの情報を取得します。
    //!
    //!         メインのサウンドアーカイブに存在するストリームサウンドの情報を取得するには、
    //!         soundArchiveName に nullptr を指定するか、引数に soundArchiveName がない関数を使用してください。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfo には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!         ストリームサウンドを再生中に情報を取得する必要がある場合は、
    //!         @ref StreamSoundHandle::ReadStreamSoundDataInfo を使用してください。
    //!
    //! @param[out] pOutInfo        読み取り結果を格納するオブジェクトのポインタを渡します。
    //! @param[in]  streamSoundName 読み取りたいストリームサウンドのラベル文字列を指定します。
    //! @param[in]  soundArchiveName 読み取りたい追加サウンドアーカイブの名前を指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidName,無効な名前を指定したか、文字列テーブルが読み込まれていません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidFormat,対応していないフォーマットのバイナリが渡されました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //---------------------------------------------------------------------------
    nn::Result ReadStreamSoundDataInfo(StreamSoundDataInfo* pOutInfo, const char* streamSoundName, const char* soundArchiveName) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ストリームサウンドのヘッダ情報を取得するために必要な作業領域の大きさを取得します。
    //!
    //! @return ストリームサウンドのヘッダ情報を取得するために必要な作業領域の大きさを返します。
    //!
    //! @see ReadStreamSoundRegionDataInfo
    //---------------------------------------------------------------------------
    static size_t GetRequiredWorkBufferSizeToReadStreamSoundHeader() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ストリームサウンドのリージョン情報を取得します。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfo には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!
    //!         また、情報の取得のために @ref GetRequiredWorkBufferSizeToReadStreamSoundHeader で
    //!         得られる大きさ以上の作業領域を必要とします。作業領域は本関数の実行中にのみ使用されます。
    //!
    //!         1 つのストリームサウンドから複数のリージョン情報を取得する場合は、
    //!         infoCount を引数に持つ @ref ReadStreamSoundRegionDataInfo の使用をご検討ください。
    //!         一度に情報を読み取ることでファイルアクセスを効率よく行うことができます。
    //!
    //! @param[out] pOutInfo      読み取り結果を格納するオブジェクトのポインタを渡します。
    //! @param[in]  streamSoundId 読み取りたいストリームサウンドのIDを指定します。
    //! @param[in]  regionName    読み取りたいリージョンの名前を指定します。
    //! @param[in]  buffer        作業領域へのポインタです。
    //! @param[in]  bufferSize    作業領域のサイズです。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerLoadFileHeaderFailed,ヘッダのロードに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundHasNoRegion,対象のストリームサウンドにリージョン情報がありません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundRegionNotFound,対象のストリームサウンドから指定されたリージョンが見つかりませんでした。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //!     - buffer != nullptr
    //!     - bufferSize >= GetRequiredWorkBufferSizeToReadStreamSoundHeader()
    //!
    //! @see GetRequiredWorkBufferSizeToReadStreamSoundHeader
    //---------------------------------------------------------------------------
    nn::Result ReadStreamSoundRegionDataInfo(StreamSoundRegionDataInfo* pOutInfo, SoundArchive::ItemId streamSoundId, const char* regionName, void* buffer, size_t bufferSize) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ストリームサウンドのリージョン情報を取得します。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfo には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!
    //!         また、情報の取得のために @ref GetRequiredWorkBufferSizeToReadStreamSoundHeader で
    //!         得られる大きさ以上の作業領域を必要とします。作業領域は本関数の実行中にのみ使用されます。
    //!
    //!         1 つのストリームサウンドから複数のリージョン情報を取得する場合は、
    //!         infoCount を引数に持つ @ref ReadStreamSoundRegionDataInfo の使用をご検討ください。
    //!         一度に情報を読み取ることでファイルアクセスを効率よく行うことができます。
    //!
    //!         @ref FsSoundArchive を使用している場合は @ref FsSoundArchive::LoadLabelStringData で
    //!         ラベル文字列データをロードしておく必要があります。
    //!
    //! @param[out] pOutInfo        読み取り結果を格納するオブジェクトのポインタを渡します。
    //! @param[in]  streamSoundName 読み取りたいストリームサウンドのラベル文字列を指定します。
    //! @param[in]  regionName      読み取りたいリージョンの名前を指定します。
    //! @param[in]  buffer          作業領域へのポインタです。
    //! @param[in]  bufferSize      作業領域のサイズです。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidName,無効な名前を指定したか、文字列テーブルが読み込まれていません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerLoadFileHeaderFailed,ヘッダのロードに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundHasNoRegion,対象のストリームサウンドにリージョン情報がありません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundRegionNotFound,対象のストリームサウンドから指定されたリージョンが見つかりませんでした。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //!     - buffer != nullptr
    //!     - bufferSize >= GetRequiredWorkBufferSizeToReadStreamSoundHeader()
    //!
    //! @see GetRequiredWorkBufferSizeToReadStreamSoundHeader
    //! @see FsSoundArchive
    //---------------------------------------------------------------------------
    nn::Result ReadStreamSoundRegionDataInfo(StreamSoundRegionDataInfo* pOutInfo, const char* streamSoundName, const char* regionName, void* buffer, size_t bufferSize) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ストリームサウンドのリージョン情報を複数取得します。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfo には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!
    //!         また、情報の取得のために @ref GetRequiredWorkBufferSizeToReadStreamSoundHeader で
    //!         得られる大きさ以上の作業領域を必要とします。作業領域は本関数の実行中にのみ使用されます。
    //!
    //! @param[out] pOutInfo      読み取り結果を格納するオブジェクトの配列を渡します。
    //! @param[in]  streamSoundId 読み取りたいストリームサウンドのIDを指定します。
    //! @param[in]  pRegionName   読み取りたいリージョンの名前の配列を指定します。
    //! @param[in]  infoCount     読み取りたいリージョン情報の個数を指定します。
    //! @param[in]  buffer        作業領域へのポインタです。
    //! @param[in]  bufferSize    作業領域のサイズです。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerLoadFileHeaderFailed,ヘッダのロードに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundHasNoRegion,対象のストリームサウンドにリージョン情報がありません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundRegionNotFound,対象のストリームサウンドから指定されたリージョンが見つかりませんでした。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //!     - buffer != nullptr
    //!     - bufferSize >= GetRequiredWorkBufferSizeToReadStreamSoundHeader()
    //!     - infoCount > 0
    //!
    //! @see GetRequiredWorkBufferSizeToReadStreamSoundHeader
    //---------------------------------------------------------------------------
    nn::Result ReadStreamSoundRegionDataInfo(StreamSoundRegionDataInfo* pOutInfo, SoundArchive::ItemId streamSoundId, const char* const* pRegionName, int infoCount, void* buffer, size_t bufferSize) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ストリームサウンドのリージョン情報を複数取得します。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfo には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!
    //!         また、情報の取得のために @ref GetRequiredWorkBufferSizeToReadStreamSoundHeader で
    //!         得られる大きさ以上の作業領域を必要とします。作業領域は本関数の実行中にのみ使用されます。
    //!
    //!         @ref FsSoundArchive を使用している場合は @ref FsSoundArchive::LoadLabelStringData で
    //!         ラベル文字列データをロードしておく必要があります。
    //!
    //! @param[out] pOutInfo        読み取り結果を格納するオブジェクトの配列を渡します。
    //! @param[in]  streamSoundName 読み取りたいストリームサウンドのラベル文字列を指定します。
    //! @param[in]  pRegionName     読み取りたいリージョンの名前の配列を指定します。
    //! @param[in]  infoCount       読み取りたいリージョン情報の個数を指定します。
    //! @param[in]  buffer          作業領域へのポインタです。
    //! @param[in]  bufferSize      作業領域のサイズです。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidName,無効な名前を指定したか、文字列テーブルが読み込まれていません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerLoadFileHeaderFailed,ヘッダのロードに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundHasNoRegion,対象のストリームサウンドにリージョン情報がありません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundRegionNotFound,対象のストリームサウンドから指定されたリージョンが見つかりませんでした。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //!     - buffer != nullptr
    //!     - bufferSize >= GetRequiredWorkBufferSizeToReadStreamSoundHeader()
    //!     - infoCount > 0
    //!
    //! @see GetRequiredWorkBufferSizeToReadStreamSoundHeader
    //! @see FsSoundArchive
    //---------------------------------------------------------------------------
    nn::Result ReadStreamSoundRegionDataInfo(StreamSoundRegionDataInfo* pOutInfo, const char* streamSoundName, const char* const* pRegionName, int infoCount, void* buffer, size_t bufferSize) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  追加サウンドアーカイブに存在するストリームサウンドのリージョン情報を取得します。
    //!
    //!         メインのサウンドアーカイブに存在するストリームサウンドのリージョン情報を取得するには、
    //!         soundArchiveName に nullptr を指定するか、引数に soundArchiveName がない関数を使用してください。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfo には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!
    //!         また、情報の取得のために @ref GetRequiredWorkBufferSizeToReadStreamSoundHeader で
    //!         得られる大きさ以上の作業領域を必要とします。作業領域は本関数の実行中にのみ使用されます。
    //!
    //!         1 つのストリームサウンドから複数のリージョン情報を取得する場合は、
    //!         infoCount を引数に持つ @ref ReadStreamSoundRegionDataInfo の使用をご検討ください。
    //!         一度に情報を読み取ることでファイルアクセスを効率よく行うことができます。
    //!
    //! @param[out] pOutInfo         読み取り結果を格納するオブジェクトのポインタを渡します。
    //! @param[in]  streamSoundName  読み取りたいストリームサウンドのラベル文字列を指定します。
    //! @param[in]  regionName       読み取りたいリージョンの名前を指定します。
    //! @param[in]  buffer           作業領域へのポインタです。
    //! @param[in]  bufferSize       作業領域のサイズです。
    //! @param[in]  soundArchiveName 読み取りたい追加サウンドアーカイブの名前を指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidName,無効な名前を指定したか、文字列テーブルが読み込まれていません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerLoadFileHeaderFailed,ヘッダのロードに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundHasNoRegion,対象のストリームサウンドにリージョン情報がありません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundRegionNotFound,対象のストリームサウンドから指定されたリージョンが見つかりませんでした。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //!     - buffer != nullptr
    //!     - bufferSize >= GetRequiredWorkBufferSizeToReadStreamSoundHeader()
    //!
    //! @see GetRequiredWorkBufferSizeToReadStreamSoundHeader
    //---------------------------------------------------------------------------
    nn::Result ReadStreamSoundRegionDataInfo(StreamSoundRegionDataInfo* pOutInfo, const char* streamSoundName, const char* regionName, void* buffer, size_t bufferSize, const char* soundArchiveName) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  追加サウンドアーカイブに存在するストリームサウンドのリージョン情報を取得します。
    //!
    //!         メインのサウンドアーカイブに存在するストリームサウンドのリージョン情報を取得するには、
    //!         soundArchiveName に nullptr を指定するか、引数に soundArchiveName がない関数を使用してください。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfo には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!
    //!         また、情報の取得のために @ref GetRequiredWorkBufferSizeToReadStreamSoundHeader で
    //!         得られる大きさ以上の作業領域を必要とします。作業領域は本関数の実行中にのみ使用されます。
    //!
    //!         1 つのストリームサウンドから複数のリージョン情報を取得する場合は、
    //!         infoCount を引数に持つ @ref ReadStreamSoundRegionDataInfo の使用をご検討ください。
    //!         一度に情報を読み取ることでファイルアクセスを効率よく行うことができます。
    //!
    //! @param[out] pOutInfo         読み取り結果を格納するオブジェクトのポインタを渡します。
    //! @param[in]  streamSoundId    読み取りたいストリームサウンドのIDを指定します。
    //! @param[in]  regionName       読み取りたいリージョンの名前を指定します。
    //! @param[in]  buffer           作業領域へのポインタです。
    //! @param[in]  bufferSize       作業領域のサイズです。
    //! @param[in]  soundArchiveName 読み取りたい追加サウンドアーカイブの名前を指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerLoadFileHeaderFailed,ヘッダのロードに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundHasNoRegion,対象のストリームサウンドにリージョン情報がありません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundRegionNotFound,対象のストリームサウンドから指定されたリージョンが見つかりませんでした。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //!     - buffer != nullptr
    //!     - bufferSize >= GetRequiredWorkBufferSizeToReadStreamSoundHeader()
    //!
    //! @see GetRequiredWorkBufferSizeToReadStreamSoundHeader
    //---------------------------------------------------------------------------
    nn::Result ReadStreamSoundRegionDataInfo(StreamSoundRegionDataInfo* pOutInfo, SoundArchive::ItemId streamSoundId, const char* regionName, void* buffer, size_t bufferSize, const char* soundArchiveName) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  追加サウンドアーカイブに存在するストリームサウンドのリージョン情報を複数取得します。
    //!
    //!         メインのサウンドアーカイブに存在するストリームサウンドのリージョン情報を取得するには、
    //!         soundArchiveName に nullptr を指定するか、引数に soundArchiveName がない関数を使用してください。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfo には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!
    //!         また、情報の取得のために @ref GetRequiredWorkBufferSizeToReadStreamSoundHeader で
    //!         得られる大きさ以上の作業領域を必要とします。作業領域は本関数の実行中にのみ使用されます。
    //!
    //! @param[out] pOutInfo         読み取り結果を格納するオブジェクトの配列を渡します。
    //! @param[in]  streamSoundName  読み取りたいストリームサウンドのラベル文字列を指定します。
    //! @param[in]  pRegionName      読み取りたいリージョンの名前の配列を指定します。
    //! @param[in]  infoCount        読み取りたいリージョン情報の個数を指定します。
    //! @param[in]  buffer           作業領域へのポインタです。
    //! @param[in]  bufferSize       作業領域のサイズです。
    //! @param[in]  soundArchiveName 読み取りたい追加サウンドアーカイブの名前を指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidName,無効な名前を指定したか、文字列テーブルが読み込まれていません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerLoadFileHeaderFailed,ヘッダのロードに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundHasNoRegion,対象のストリームサウンドにリージョン情報がありません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundRegionNotFound,対象のストリームサウンドから指定されたリージョンが見つかりませんでした。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //!     - buffer != nullptr
    //!     - bufferSize >= GetRequiredWorkBufferSizeToReadStreamSoundHeader()
    //!     - infoCount > 0
    //!
    //! @see GetRequiredWorkBufferSizeToReadStreamSoundHeader
    //---------------------------------------------------------------------------
    nn::Result ReadStreamSoundRegionDataInfo(StreamSoundRegionDataInfo* pOutInfo, const char* streamSoundName, const char* const* pRegionName, int infoCount, void* buffer, size_t bufferSize, const char* soundArchiveName) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  追加サウンドアーカイブに存在するストリームサウンドのリージョン情報を複数取得します。
    //!
    //!         メインのサウンドアーカイブに存在するストリームサウンドのリージョン情報を取得するには、
    //!         soundArchiveName に nullptr を指定するか、引数に soundArchiveName がない関数を使用してください。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfo には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!
    //!         また、情報の取得のために @ref GetRequiredWorkBufferSizeToReadStreamSoundHeader で
    //!         得られる大きさ以上の作業領域を必要とします。作業領域は本関数の実行中にのみ使用されます。
    //!
    //! @param[out] pOutInfo         読み取り結果を格納するオブジェクトの配列を渡します。
    //! @param[in]  streamSoundId    読み取りたいストリームサウンドのIDを指定します。
    //! @param[in]  pRegionName      読み取りたいリージョンの名前の配列を指定します。
    //! @param[in]  infoCount        読み取りたいリージョン情報の個数を指定します。
    //! @param[in]  buffer           作業領域へのポインタです。
    //! @param[in]  bufferSize       作業領域のサイズです。
    //! @param[in]  soundArchiveName 読み取りたい追加サウンドアーカイブの名前を指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerLoadFileHeaderFailed,ヘッダのロードに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundHasNoRegion,対象のストリームサウンドにリージョン情報がありません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundRegionNotFound,対象のストリームサウンドから指定されたリージョンが見つかりませんでした。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //!     - buffer != nullptr
    //!     - bufferSize >= GetRequiredWorkBufferSizeToReadStreamSoundHeader()
    //!     - infoCount > 0
    //!
    //! @see GetRequiredWorkBufferSizeToReadStreamSoundHeader
    //---------------------------------------------------------------------------
    nn::Result ReadStreamSoundRegionDataInfo(StreamSoundRegionDataInfo* pOutInfo, SoundArchive::ItemId streamSoundId, const char* const* pRegionName, int infoCount, void* buffer, size_t bufferSize, const char* soundArchiveName) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  追加サウンドアーカイブに存在するストリームサウンドのリージョン情報を複数取得します。
    //!
    //!         メインのサウンドアーカイブに存在するストリームサウンドのリージョン情報を取得するには、
    //!         引数に pSoundArchive がない関数を使用してください。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfo には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!
    //!         また、情報の取得のために @ref GetRequiredWorkBufferSizeToReadStreamSoundHeader で
    //!         得られる大きさ以上の作業領域を必要とします。作業領域は本関数の実行中にのみ使用されます。
    //!
    //! @param[out] pOutInfo         読み取り結果を格納するオブジェクトの配列を渡します。
    //! @param[in]  streamSoundId    読み取りたいストリームサウンドのIDを指定します。
    //! @param[in]  pRegionName      読み取りたいリージョンの名前の配列を指定します。
    //! @param[in]  infoCount        読み取りたいリージョン情報の個数を指定します。
    //! @param[in]  pSoundArchive    読み取りたい追加サウンドアーカイブを指定します。
    //! @param[in]  buffer           作業領域へのポインタです。
    //! @param[in]  bufferSize       作業領域のサイズです。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerLoadFileHeaderFailed,ヘッダのロードに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundHasNoRegion,対象のストリームサウンドにリージョン情報がありません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundRegionNotFound,対象のストリームサウンドから指定されたリージョンが見つかりませんでした。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfo != nullptr
    //!     - SoundArchivePlayer は初期化されている
    //!     - buffer != nullptr
    //!     - bufferSize >= GetRequiredWorkBufferSizeToReadStreamSoundHeader()
    //!     - infoCount > 0
    //!     - pSoundArchive != nullptr
    //!
    //! @see GetRequiredWorkBufferSizeToReadStreamSoundHeader
    //---------------------------------------------------------------------------
    nn::Result ReadStreamSoundRegionDataInfo(StreamSoundRegionDataInfo* pOutInfo, SoundArchive::ItemId streamSoundId, const char* const* pRegionName, int infoCount, const SoundArchive* pSoundArchive, void* buffer, size_t bufferSize) const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ストリームサウンドが使用している波形に埋め込まれているマーカー情報を取得します。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfoArray には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!
    //!         大きめの数の @ref StreamSoundMarkerInfo の配列と、その数をそれぞれ pOutInfoArray,
    //!         markerInfoCountMax として渡すことで、ストリームサウンドバイナリから情報を読み取って、
    //!         pOutInfoArray と pOutActualInfoCount に結果が格納されます。
    //!
    //!         すべてのマーカー情報を取得できたかどうかは、本関数を実行後に pOutActualInfoCount が
    //!         markerInfoCountMax 以下になっているかどうかで確認することができます。
    //!         pOutActualInfoCount が markerInfoCountMax より大きかった場合には、
    //!         先頭から markerInfoCountMax 個分だけの情報が pOutInfoArray に格納された状態になります。
    //!
    //! @param[in,out]  pOutInfoArray        読み取り結果を格納するオブジェクト配列の先頭ポインタです。
    //! @param[in,out]  pOutActualInfoCount  バイナリに含まれていたマーカー情報の数を格納するための変数へのポインタです。
    //! @param[in]      markerInfoCountMax   読み取り結果を格納するオブジェクト配列の数です。
    //! @param[in]      streamSoundId        読み取り対象のストリームサウンドの ID です。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidFormat,対応していないフォーマットのバイナリが渡されました。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfoArray != nullptr
    //!     - pOutActualInfoCount != nullptr
    //!     - markerInfoCountMax > 0
    //!     - SoundArchivePlayer は初期化されている
    //---------------------------------------------------------------------------
    nn::Result ReadMarkerInfoArray(StreamSoundMarkerInfo* pOutInfoArray, int* pOutActualInfoCount, int markerInfoCountMax, SoundArchive::ItemId streamSoundId) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ストリームサウンドが使用している波形に埋め込まれているマーカー情報を取得します。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfoArray には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!
    //!         大きめの数の @ref StreamSoundMarkerInfo の配列と、その数をそれぞれ pOutInfoArray,
    //!         markerInfoCountMax として渡すことで、ストリームサウンドバイナリから情報を読み取って、
    //!         pOutInfoArray と pOutActualInfoCount に結果が格納されます。
    //!
    //!         すべてのマーカー情報を取得できたかどうかは、本関数を実行後に pOutActualInfoCount が
    //!         markerInfoCountMax 以下になっているかどうかで確認することができます。
    //!         pOutActualInfoCount が markerInfoCountMax より大きかった場合には、
    //!         先頭から markerInfoCountMax 個分だけの情報が pOutInfoArray に格納された状態になります。
    //!
    //! @param[in,out]  pOutInfoArray        読み取り結果を格納するオブジェクト配列の先頭ポインタです。
    //! @param[in,out]  pOutActualInfoCount  バイナリに含まれていたマーカー情報の数を格納するための変数へのポインタです。
    //! @param[in]      markerInfoCountMax   読み取り結果を格納するオブジェクト配列の数です。
    //! @param[in]      streamSoundName      読み取り対象のストリームサウンドのラベル文字列です。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidName,無効な名前を指定したか、文字列テーブルが読み込まれていません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidFormat,対応していないフォーマットのバイナリが渡されました。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfoArray != nullptr
    //!     - pOutActualInfoCount != nullptr
    //!     - markerInfoCountMax > 0
    //!     - SoundArchivePlayer は初期化されている
    //---------------------------------------------------------------------------
    nn::Result ReadMarkerInfoArray(StreamSoundMarkerInfo* pOutInfoArray, int* pOutActualInfoCount, int markerInfoCountMax, const char* streamSoundName) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ストリームサウンドが使用している波形に埋め込まれているマーカー情報を取得します。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfoArray には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!
    //!         大きめの数の @ref StreamSoundMarkerInfo の配列と、その数をそれぞれ pOutInfoArray,
    //!         markerInfoCountMax として渡すことで、ストリームサウンドバイナリから情報を読み取って、
    //!         pOutInfoArray と pOutActualInfoCount に結果が格納されます。
    //!
    //!         すべてのマーカー情報を取得できたかどうかは、本関数を実行後に pOutActualInfoCount が
    //!         markerInfoCountMax 以下になっているかどうかで確認することができます。
    //!         pOutActualInfoCount が markerInfoCountMax より大きかった場合には、
    //!         先頭から markerInfoCountMax 個分だけの情報が pOutInfoArray に格納された状態になります。
    //!
    //! @param[in,out]  pOutInfoArray        読み取り結果を格納するオブジェクト配列の先頭ポインタです。
    //! @param[in,out]  pOutActualInfoCount  バイナリに含まれていたマーカー情報の数を格納するための変数へのポインタです。
    //! @param[in]      markerInfoCountMax   読み取り結果を格納するオブジェクト配列の数です。
    //! @param[in]      streamSoundId        読み取り対象のストリームサウンドの ID です。
    //! @param[in]      soundArchiveName     読み取り対象のサウンドアーカイブ名です。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidFormat,対応していないフォーマットのバイナリが渡されました。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfoArray != nullptr
    //!     - pOutActualInfoCount != nullptr
    //!     - markerInfoCountMax > 0
    //!     - SoundArchivePlayer は初期化されている
    //---------------------------------------------------------------------------
    nn::Result ReadMarkerInfoArray(StreamSoundMarkerInfo* pOutInfoArray, int* pOutActualInfoCount, int markerInfoCountMax, SoundArchive::ItemId streamSoundId, const char* soundArchiveName) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  ストリームサウンドが使用している波形に埋め込まれているマーカー情報を取得します。
    //!
    //!         SoundArchivePlayer を初期化した後、対象のストリームサウンドを
    //!         再生していなければ、任意のタイミングで情報を取得することができます。
    //!         再生中に取得しようとした場合、pOutInfoArray には情報が読み込まれず、
    //!         @ref ResultSoundArchivePlayerStreamSoundIsPlaying エラーを返します。
    //!
    //!         大きめの数の @ref StreamSoundMarkerInfo の配列と、その数をそれぞれ pOutInfoArray,
    //!         markerInfoCountMax として渡すことで、ストリームサウンドバイナリから情報を読み取って、
    //!         pOutInfoArray と pOutActualInfoCount に結果が格納されます。
    //!
    //!         すべてのマーカー情報を取得できたかどうかは、本関数を実行後に pOutActualInfoCount が
    //!         markerInfoCountMax 以下になっているかどうかで確認することができます。
    //!         pOutActualInfoCount が markerInfoCountMax より大きかった場合には、
    //!         先頭から markerInfoCountMax 個分だけの情報が pOutInfoArray に格納された状態になります。
    //!
    //! @param[in,out]  pOutInfoArray        読み取り結果を格納するオブジェクト配列の先頭ポインタです。
    //! @param[in,out]  pOutActualInfoCount  バイナリに含まれていたマーカー情報の数を格納するための変数へのポインタです。
    //! @param[in]      markerInfoCountMax   読み取り結果を格納するオブジェクト配列の数です。
    //! @param[in]      streamSoundName      読み取り対象のストリームサウンドのラベル文字列です。
    //! @param[in]      soundArchiveName     読み取り対象のサウンドアーカイブ名です。
    //!
    //! @retresult
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidName,無効な名前を指定したか、文字列テーブルが読み込まれていません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerStreamSoundIsPlaying,対象のストリームサウンドは再生中です。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerFileSystemError,ファイルシステム処理に失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidFormat,対応していないフォーマットのバイナリが渡されました。}
    //! @endretresult
    //!
    //! @pre
    //!     - pOutInfoArray != nullptr
    //!     - pOutActualInfoCount != nullptr
    //!     - markerInfoCountMax > 0
    //!     - SoundArchivePlayer は初期化されている
    //---------------------------------------------------------------------------
    nn::Result ReadMarkerInfoArray(StreamSoundMarkerInfo* pOutInfoArray, int* pOutActualInfoCount, int markerInfoCountMax, const char* streamSoundName, const char* soundArchiveName) NN_NOEXCEPT;

    //! @internal
    void DumpMemory() const NN_NOEXCEPT;

    //! @briefprivate
    bool ReadStreamSoundInstanceState(StreamSoundInstanceState* pOutInfo) const NN_NOEXCEPT;

    //! @brief ストリームサウンドのファイル(*.bfstm)が存在するかを確認します。
    //!
    //!        指定した ID にあたるストリームサウンドのファイルが存在しない場合 @ref nn::fs::ResultPathNotFound を返します。
    //!        また、ストリームサウンド以外の ID を指定した場合や存在しない ID を指定した場合など
    //!        ファイルパスの取得に失敗した場合は @ref ResultSoundArchivePlayerReadStreamSoundFilePathFailed を返します。
    //!
    //! @param[in] streamSoundId 確認したいストリームサウンドのIDを指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::fs::ResultPathNotFound,対象ファイルが存在しません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //! @endretresult
    //!
    //! @pre
    //!     - SoundArchivePlayer は初期化されている
    //!
    //! @see nn::atk::SoundArchive::ReadStreamSoundFilePath
    nn::Result CheckStreamSoundFileExisting(SoundArchive::ItemId streamSoundId) const NN_NOEXCEPT;

    //! @brief ストリームサウンドのファイル(*.bfstm)が存在するかを確認します。
    //!
    //!        指定したラベル名にあたるストリームサウンドのファイルが存在しない場合 @ref nn::fs::ResultPathNotFound を返します。
    //!        また、ストリームサウンド以外のラベルを指定した場合など
    //!        ファイルパスの取得に失敗した場合は @ref ResultSoundArchivePlayerReadStreamSoundFilePathFailed を、
    //!        ラベル文字列が無効な場合や、文字列テーブルが読み込まれていない場合は @ref ResultSoundArchivePlayerInvalidName を返します。
    //!
    //! @param[in] streamSoundName 確認したいストリームサウンドのラベル文字列を指定します。
    //!
    //! @retresult
    //!   @handleresult{nn::fs::ResultPathNotFound,対象ファイルが存在しません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidName,無効な名前を指定したか、文字列テーブルが読み込まれていません。}
    //! @endretresult
    //!
    //! @pre
    //!     - SoundArchivePlayer は初期化されている
    //!
    //! @see nn::atk::SoundArchive::ReadStreamSoundFilePath
    nn::Result CheckStreamSoundFileExisting(const char* streamSoundName) const NN_NOEXCEPT;

    //! @brief 追加サウンドアーカイブにストリームサウンドのファイル(*.bfstm)が存在するかを確認します。
    //!
    //!        メインのサウンドアーカイブにストリームサウンドのファイルが存在するかを確認するには、
    //!        soundArchiveName に nullptr を指定するか、引数に soundArchiveName がない関数を使用してください。
    //!
    //!        指定した ID にあたるストリームサウンドのファイルが存在しない場合 @ref nn::fs::ResultPathNotFound を返します。
    //!        また、ストリームサウンド以外の ID を指定した場合や存在しない ID を指定した場合など
    //!        ファイルパスの取得に失敗した場合は @ref ResultSoundArchivePlayerReadStreamSoundFilePathFailed を返します。
    //!
    //! @param[in] streamSoundId 確認したいストリームサウンドのIDを指定します。
    //! @param[in] soundArchiveName 確認したいサウンドアーカイブの名前です。
    //!
    //! @retresult
    //!   @handleresult{nn::fs::ResultPathNotFound,対象ファイルが存在しません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //! @endretresult
    //!
    //! @pre
    //!     - SoundArchivePlayer は初期化されている
    //!
    //! @see nn::atk::SoundArchive::ReadStreamSoundFilePath
    nn::Result CheckStreamSoundFileExisting(SoundArchive::ItemId streamSoundId, const char* soundArchiveName) const NN_NOEXCEPT;

    //! @brief 追加サウンドアーカイブにストリームサウンドのファイル(*.bfstm)が存在するかを確認します。
    //!
    //!        メインのサウンドアーカイブにストリームサウンドのファイルが存在するかを確認するには、
    //!        soundArchiveName に nullptr を指定するか、引数に soundArchiveName がない関数を使用してください。
    //!
    //!        指定したラベル名にあたるストリームサウンドのファイルが存在しない場合 @ref nn::fs::ResultPathNotFound を返します。
    //!        また、ストリームサウンド以外のラベルを指定した場合など
    //!        ファイルパスの取得に失敗した場合は @ref ResultSoundArchivePlayerReadStreamSoundFilePathFailed を、
    //!        ラベル文字列が無効な場合や、文字列テーブルが読み込まれていない場合は @ref ResultSoundArchivePlayerInvalidName を返します。
    //!
    //! @param[in] streamSoundName 確認したいストリームサウンドのラベル文字列を指定します。
    //! @param[in] soundArchiveName 確認したいサウンドアーカイブの名前です。
    //!
    //! @retresult
    //!   @handleresult{nn::fs::ResultPathNotFound,対象ファイルが存在しません。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerReadStreamSoundFilePathFailed,対象のストリームサウンドのファイルパスの読み込みに失敗しました。}
    //!   @handleresult{nn::atk::ResultSoundArchivePlayerInvalidName,無効な名前を指定したか、文字列テーブルが読み込まれていません。}
    //! @endretresult
    //!
    //! @pre
    //!     - SoundArchivePlayer は初期化されている
    //!
    //! @see nn::atk::SoundArchive::ReadStreamSoundFilePath
    nn::Result CheckStreamSoundFileExisting(const char* streamSoundName, const char* soundArchiveName) const NN_NOEXCEPT;

    // StartSound実装
    //! @internal
    //! @param handle :private
    //! @param soundId :private
    //! @param ambientArgInfo :private
    //! @param actor :private
    //! @param holdFlag :private
    //! @param soundArchiveName :private
    //! @param startInfo :private
    //! @return :private
    StartResult detail_SetupSoundImpl(
        SoundHandle* handle,
        uint32_t soundId,
        detail::BasicSound::AmbientInfo* ambientArgInfo,
        SoundActor* actor,
        bool holdFlag,
        const char* soundArchiveName,
        const StartInfo* startInfo
    ) NN_NOEXCEPT;

    //! @internal
    //! @param pString :private
    //! @return :private
    virtual SoundArchive::ItemId detail_GetItemId( const char* pString ) NN_NOEXCEPT NN_OVERRIDE
    {
        return detail_GetItemId(pString, nullptr);
    }

    //! @internal
    //! @param pString :private
    //! @param soundArchiveName :private
    //! @return :private
    virtual SoundArchive::ItemId detail_GetItemId( const char* pString, const char* soundArchiveName ) NN_NOEXCEPT NN_OVERRIDE
    {
        m_SoundArchiveManager.ChangeTargetArchive(soundArchiveName);
        return m_SoundArchiveManager.GetCurrentSoundArchive()->GetItemId(pString);
    }

    //! @internal
    //! @param isEnable :private
    //! @return :private
    void detail_EnableWarningPrint(bool isEnable) NN_NOEXCEPT
    {
        m_IsEnableWarningPrint = isEnable;
    }
    //! @internal
    //! @return :private
    bool detail_IsEnableWarningPrint() const NN_NOEXCEPT
    {
        return m_IsEnableWarningPrint;
    }


    // 非公開関数
    //! @internal
    //! @param parser :private
    void detail_SetMmlParser( detail::driver::MmlParser* parser ) NN_NOEXCEPT
    {
        m_SequenceSoundRuntime.SetMmlParser(parser);
    }
    //! @internal
    //! @param fileId :private
    //! @return :private
    const void* detail_GetFileAddress( SoundArchive::FileId fileId ) const NN_NOEXCEPT;

    //! @internal
    //! @return :private
    detail::SoundArchiveFilesHook* detail_GetSoundArchiveFilesHook() const NN_NOEXCEPT
    {
        return m_pSoundArchiveFilesHook;
    }

    //! @internal
    //! @param filesHook :private
    void detail_SetSoundArchiveFilesHook(detail::SoundArchiveFilesHook* filesHook) NN_NOEXCEPT
    {
        m_pSoundArchiveFilesHook = filesHook;

        m_SequenceSoundRuntime.SetSoundArchiveFilesHook(filesHook);
        m_WaveSoundRuntime.SetSoundArchiveFilesHook(filesHook);
        m_StreamSoundRuntime.SetSoundArchiveFilesHook(filesHook);
    }

    //! @internal
    bool detail_SetupStreamBufferPool( void* strmBuffer, size_t strmBufferSize, detail::driver::StreamBufferPool* pStreamBufferPool ) NN_NOEXCEPT
    {
        if ( !m_IsInitialized )
        {
            return false;
        }
        return m_StreamSoundRuntime.SetupStreamBuffer( m_SoundArchiveManager.GetMainSoundArchive(), strmBuffer, strmBufferSize, pStreamBufferPool );
    }

    //! @brief 追加サウンドアーカイブをプレイヤーに追加します。
    //!
    //!        この API によって追加された追加サウンドアーカイブは、
    //!        引数で指定した名前を持つ追加サウンドアーカイブとして使用することが出来ます。
    //!
    //! @param[in] name              追加サウンドアーカイブの名前です。名前の長さの上限値は @ref AddonSoundArchiveNameLengthMax です。
    //! @param[in] pSoundArchive     追加サウンドアーカイブです。
    //! @param[in] pSoundDataManager 追加サウンドアーカイブが使用する SoundDataManager です。
    //!
    //! @pre
    //!    - SoundArchivePlayer は初期化されている
    //!    - (SoundArchivePlayer の初期化時に指定した追加サウンドアーカイブ数) > 0
    //!    - name != nullptr
    //!    - (name の文字列長) < AddonSoundArchiveNameLengthMax
    //!    - pSoundArchive != nullptr
    //!    - pSoundArchive は初期化されている
    //!    - pSoundDataManager != nullptr
    //!    - pSoundDataManager は初期化されている
    //!
    //! @see RemoveAddonSoundArchive
    //! @see AddonSoundArchiveNameLengthMax
    void AddAddonSoundArchive(const char* name, const AddonSoundArchive* pSoundArchive, const SoundDataManager* pSoundDataManager) NN_NOEXCEPT;

    //! @brief 追加サウンドアーカイブをプレイヤーから削除します。
    //!
    //! @param[in] pSoundArchive 削除する追加サウンドアーカイブです。
    //!
    //! @pre
    //!     - SoundArchivePlayer は初期化されている
    //!     - (SoundArchivePlayer の初期化時に指定した追加サウンドアーカイブ数) > 0
    //!     - pSoundArchive != nullptr
    //!
    //! @see AddAddonSoundArchive
    void RemoveAddonSoundArchive(const AddonSoundArchive* pSoundArchive) NN_NOEXCEPT;

    //! @briefprivate
    //! @brief ストリーム再生に使用するバッファについて、確保するブロックの数を設定します。
    //!        ブロック数の初期値は DefaultStreamBlockCount です。
    //! @param[in] count ストリーム再生に使用するバッファについて、確保するブロックの数です。
    //! @return 設定に成功した場合は true を返します。
    //!
    //! @pre SoundArchivePlayer が未初期化
    //!      count >= StreamBlockCountMin
    //!      count <= DefaultStreamBlockCount
    //! @see GetStreamBlockCount
    bool SetStreamBlockCount(int count) NN_NOEXCEPT
    {
        if (!m_IsInitialized)
        {
            return m_StreamSoundRuntime.SetStreamBlockCount(count);
        }
        return false;
    }

    //! @briefprivate
    //! @brief ストリーム再生に使用するバッファについて、確保するブロックの数を取得します。
    //!        ブロック数の初期値は DefaultStreamBlockCount です。
    //! @return ストリーム再生に使用するバッファについて、確保するブロックの数です。
    //!
    //! @see SetStreamBlockCount
    int GetStreamBlockCount() NN_NOEXCEPT
    {
        return m_StreamSoundRuntime.GetStreamBlockCount();
    }

    //! @brief   ユーザ定義のサブミックス構成使用時にサウンドを再生する際、デフォルトで使用する出力先を設定します。
    //! @details ユーザ定義のサブミックス構成を使用している場合に、
    //!          SoundStartable::StartInfo::pOutputReceiver での出力先設定を行わずにサウンドの再生を行った場合のデフォルトの出力先を設定します。
    //!
    //!          初期状態では出力先にファイナルミックスが設定されます。
    //!          初期状態に戻す場合は、この関数で nullptr を設定してください。
    //! @param[in] pOutputReceiver 設定するデフォルトで使用するの出力先です。nullptr を指定した場合はファイナルミックスが出力先になります。
    void SetDefaultOutputReceiver(OutputReceiver* pOutputReceiver) NN_NOEXCEPT;

protected:
    virtual StartResult detail_SetupSound(
        SoundHandle* handle,
        uint32_t soundId,
        bool holdFlag,
        const char* soundArchiveName,
        const StartInfo* startInfo
    ) NN_NOEXCEPT NN_OVERRIDE;

private:
    void EnableHook(const SoundArchive* pSoundArchive, bool isHookDisabled) NN_NOEXCEPT;

    bool IsSoundArchiveFileHooksEnabled() const NN_NOEXCEPT;
    void LockSoundArchiveFileHooks() NN_NOEXCEPT;
    void UnlockSoundArchiveFileHooks() NN_NOEXCEPT;

    bool IsSequenceSoundEdited(const char* soundName) const NN_NOEXCEPT;
    bool IsStreamSoundEdited(const char* soundName) const NN_NOEXCEPT;
    bool IsWaveSoundEdited(const char* soundName) const NN_NOEXCEPT;

    detail::PlayerHeap* CreatePlayerHeap(
            void** pOutAllocatedAddr, const void* endAddr, size_t heapSize ) NN_NOEXCEPT;

    // Setup
    bool SetupMram( const SoundArchive* pArc, void* buffer, size_t size, size_t userParamSizePerSound, int addonSoundArchiveCount, void* streamSoundInstanceBuffer, size_t streamSoundInstanceBufferSize ) NN_NOEXCEPT;
    bool SetupSoundPlayer( const SoundArchive* pArc, void** pOutAllocatedAddr, const void* endAddr ) NN_NOEXCEPT;
    bool SetupAddonSoundArchiveContainer( int addonSoundArchiveCount, void** pOutAllocatedAddr, const void* endAddr ) NN_NOEXCEPT;
    bool SetupUserParamForBasicSound(
            const SoundArchive::SoundArchivePlayerInfo& info, void** buffer, const void* endp,
            size_t userParamSizePerSound) NN_NOEXCEPT;

    void SetCommonSoundParam(
            detail::BasicSound* sound, const SoundArchive::SoundInfo* commonInfo ) NN_NOEXCEPT;

    bool IsLoadWaveArchive( const void* bankFile, detail::LoadItemInfo* info ) const NN_NOEXCEPT;

    SoundStartable::StartResult PreprocessSinglePlay(const SoundArchive::SoundInfo& info, uint32_t soundId, SoundPlayer& player) NN_NOEXCEPT;

    void StopAllSound( int fadeFrames, bool isCommandFlushed ) NN_NOEXCEPT;
    void DisposeInstances() NN_NOEXCEPT;

    nn::Result CheckStreamSoundFileExisting(const SoundArchive* pSoundArchive, SoundArchive::ItemId streamSoundId) const NN_NOEXCEPT;

    nn::Result ReadMarkerInfoArrayImpl(StreamSoundMarkerInfo* pOutInfoArray, int* pOutActualInfoCount, int markerInfoCountMax, SoundArchive::ItemId streamSoundId, const SoundArchive* pSoundArchive) NN_NOEXCEPT;

private:
    detail::SoundArchiveManager m_SoundArchiveManager;

    // サウンドプレイヤー
    uint32_t m_SoundPlayerCount;
    SoundPlayer* m_pSoundPlayers;

    detail::SequenceSoundRuntime m_SequenceSoundRuntime;
    detail::WaveSoundRuntime m_WaveSoundRuntime;
    detail::AdvancedWaveSoundRuntime m_AdvancedWaveSoundRuntime;
    detail::StreamSoundRuntime m_StreamSoundRuntime;

    // バッファ情報記憶
    size_t m_SoundUserParamSize;

    // データマネージャ
    int m_ArchiveContainerCount;
    detail::AddonSoundArchiveContainer* m_pArchiveContainers;
    nn::os::Tick m_AddonSoundArchiveLastAddTick;

    // メモリプール
    nn::audio::MemoryPoolType m_MemoryPoolForStreamInstance;
    bool m_IsMemoryPoolForStreamInstanceAttached;
    nn::audio::MemoryPoolType m_MemoryPoolForPlayerHeap;
    bool m_IsMemoryPoolForPlayerHeapAttached;

    // フック
    detail::SoundArchiveFilesHook* m_pSoundArchiveFilesHook;

    bool m_IsEnableWarningPrint;
    bool m_IsInitialized;
    bool m_IsAdvancedWaveSoundEnabled;

    OutputReceiver* m_pDefaultOutputReceiver;

#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t m_Padding[1];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
};

} // namespace nn::atk
} // namespace nn

