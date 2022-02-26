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

#include <nn/atk/atk_Config.h>
#ifdef NN_ATK_CONFIG_ENABLE_DEV

#include <nn/atk/viewer/atk_Viewer.h>

#include <nn/atk/viewer/atk_Types.h>
#include <nn/atk/fnd/basis/atkfnd_Memory.h>

namespace nn {
namespace atk {
namespace viewer {

//---------------------------------------------------------------------------
//! @brief  サウンドインゲーム編集機能を提供するクラスです。
//!
//!         このクラスを使用することで、簡単にサウンドインゲーム編集機能を組み込むことができます。@n
//!         細かいカスタマイズを行いたい場合は、（ベータ版扱いとなりますが、）
//!         このクラスではなく、@ref SoundEditSession, @ref SoundArchiveEditor を使用してください。
//!
//---------------------------------------------------------------------------
class SoundEdit
{
public:
    //! サウンドインゲーム編集機能で使用するヒープサイズのデフォルト値です。
    static const size_t DefaultSndeditHeapSize = 4 * 1024 * 1024;
    //! サウンドインゲーム編集機能で使用するスレッドのスタックサイズのデフォルト値です。
    static const size_t DefaultThreadStackSize = nn::atk::viewer::UpdateCachesHelper::DefaultThreadStackSize;

    //! サウンドインゲーム編集機能で使用するポート番号のデフォルト値です。
    static const PORT DefaultSndeditPort0 = detail::HioChannelType_SndeditSync;
    //! サウンドインゲーム編集機能で使用するポート番号のデフォルト値です。
    static const PORT DefaultSndeditPort1 = detail::HioChannelType_SndeditFunc;
    //! サウンドインゲーム編集機能で使用するポート番号のデフォルト値です。
    static const PORT DefaultSndeditPort2 = detail::HioChannelType_SndeditCtrl;
    //! サウンドインゲーム編集機能で使用するワーカースレッドが動作するコア番号のデフォルト値です。
    static const int DefaultSndeditWorkerThreadCoreNumber = 0;

public:
    //---------------------------------------------------------------------------
    //! @brief  初期化用のパラメータを格納する構造体です。
    //!
    //!         Initialize()に渡すまでに、各パラメータに値を入れておく必要があります。
    //!
    //! @see    Initialize()
    //---------------------------------------------------------------------------
    struct InitializeArg
    {
        //---------------------------------------------------------------------------
        //! @brief  コンストラクタです。
        //---------------------------------------------------------------------------
        InitializeArg() NN_NOEXCEPT
        : buffer(NULL)
        , bufferSize(0)
        , soundArchive(NULL)
        , soundArchivePlayer(NULL)
        , port0(DefaultSndeditPort0)
        , port1(DefaultSndeditPort1)
        , port2(DefaultSndeditPort2)
        , workerThreadCoreNumber(DefaultSndeditWorkerThreadCoreNumber)
        {
        }

        void* buffer;                               //!< インゲーム編集ライブラリが使用するバッファを指定します。
        size_t bufferSize;                          //!< インゲーム編集ライブラリが使用するバッファのサイズを指定します。
        SoundArchive* soundArchive;                 //!< 関連するサウンドアーカイブを指定します。
        SoundArchivePlayer* soundArchivePlayer;     //!< 関連するサウンドアーカイブプレイヤーを指定します。

        PORT port0;                                 //!< 内部で使用するポートです。
        PORT port1;                                 //!< 内部で使用するポートです。
        PORT port2;                                 //!< 内部で使用するポートです。
        int workerThreadCoreNumber;                 //!< 内部で使用するワーカースレッドが動作するコアの番号を指定します。
    };

public:
    //---------------------------------------------------------------------------
    //! @brief  初期化用のオプションパラメータを格納する構造体です。
    //!
    //!         各パラメータには、コンストラクタでデフォルト値が設定されます。@n
    //!         通常の利用において、値を変更する必要はありません。
    //!
    //! @see    Initialize()
    //! @see    GetRequiredMemorySize()
    //---------------------------------------------------------------------------
    struct Option
    {
        //---------------------------------------------------------------------------
        //! @brief  コンストラクタです。
        //---------------------------------------------------------------------------
        Option() NN_NOEXCEPT
        : sndEditHeapSize(DefaultSndeditHeapSize)
        , threadStackSize(DefaultThreadStackSize)
        {
        }

        size_t sndEditHeapSize;        //!< インゲーム編集ライブラリが内部で使用するヒープのサイズを指定します。@n
                                        //!< 指定しないとデフォルト値 DefaultSndeditHeapSize が使用されます。@n
                                        //!< 通常の利用において、値を変更する必要はありません。
        size_t threadStackSize;       //!< インゲーム編集ライブラリが内部で使用するスレッドのスタックサイズを指定します。@n
                                        //!< 指定しないとデフォルト値 DefaultThreadStackSize が使用されます。@n
                                        //!< 通常の利用において、値を変更する必要はありません。
    };

public: // コンストラクタ
    //===========================================================================
    //! @name コンストラクタ/デストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //---------------------------------------------------------------------------
    SoundEdit() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  デストラクタです。
    //---------------------------------------------------------------------------
    ~SoundEdit() NN_NOEXCEPT;

    //@}
    // コンストラクタ/デストラクタ
    //===========================================================================

public: // メソッド
    //===========================================================================
    //! @name 初期化
    //@{

    //---------------------------------------------------------------------------
    //! @brief      SoundEdit の初期化を行います。
    //!
    //!             この関数は、サウンドインゲーム編集機能で利用するメモリを初期化し、
    //!             SoundMaker との接続を開始するための準備を行います。@n
    //!             他の関数を実行する前に、必ずこの関数を呼び出して初期化してください。
    //!
    //! @param[in]  args   InitializeArgを指定します。
    //! @param[in]  param  GetRequiredMemorySize() に渡した Option を指定します。
    //!
    //! @see        InitializeArg
    //! @see        GetRequiredMemorySize()
    //! @see        Finalize()
    //---------------------------------------------------------------------------
    void Initialize(const InitializeArg& args, const Option& param) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      初期化済みかどうかをチェックします。
    //!
    //! @return     初期化済みであればtrueを返します。
    //!
    //! @see        Initialize()
    //---------------------------------------------------------------------------
    bool IsInitialized() const NN_NOEXCEPT { return m_IsInitialized; }

    //---------------------------------------------------------------------------
    //! @brief  SoundEdit の終了処理を行います。
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief      サウンドインゲーム編集機能の利用に必要なメモリサイズを取得します。
    //!
    //!             サウンドインゲーム編集機能を利用するには、この関数で取得したサイズ分のバッファを確保し、
    //!             Initialize() の引数 InitializeArg に指定する必要があります。@n
    //!             その際 Initialize() の引数 param には、
    //!             GetRequiredMemorySize() に渡した Option と同じものを指定してください。
    //!
    //! @param[in]  param  Optionを指定します。
    //!
    //! @return     必要なメモリサイズを返します。
    //!
    //! @see        Option
    //! @see        Initialize()
    //---------------------------------------------------------------------------
    size_t GetRequiredMemorySize(const Option& param) const NN_NOEXCEPT;

    //@}
    // 初期化
    //===========================================================================

    //===========================================================================
    //! @name 開始と終了
    //@{

    //---------------------------------------------------------------------------
    //! @brief  サウンドインゲーム編集機能を開始します。
    //!
    //!         この関数が呼び出されると、Update() のタイミングで
    //!         SoundMaker との通信が行われるようになります。
    //!         また、内部でキャッシュの更新を行うためのスレッドを１つ起動します。
    //!
    //! @see    Stop()
    //! @see    Update()
    //---------------------------------------------------------------------------
    void Start() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  サウンドインゲーム編集機能が開始されているかチェックします。
    //!
    //! @return サウンドインゲーム編集機能が開始されていれば true を返します。
    //!
    //! @see    Start()
    //---------------------------------------------------------------------------
    bool IsStarted() const NN_NOEXCEPT { return m_IsStarted; }

    //---------------------------------------------------------------------------
    //! @brief  サウンドインゲーム編集機能を停止します。
    //!
    //!         同時に SoundMakerとの通信や、キャッシュ更新用の内部スレッドを終了します。
    //!
    //! @see    Start()
    //---------------------------------------------------------------------------
    void Stop() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  接続が確立されているかチェックします。
    //!
    //! @return 接続が確立されていれば true を返します。
    //---------------------------------------------------------------------------
    bool IsConnected() const NN_NOEXCEPT { return m_SoundEditSession.IsConnected() && m_SoundControlSession.IsConnected(); }

    //---------------------------------------------------------------------------
    //! @brief  サウンドインゲーム編集機能の更新処理を行います。
    //!
    //!         SoundMaker への接続要求を投げたり、
    //!         SoundMaker に編集のあったアイテムを問い合わせたりします。
    //!
    //!         セッションが開かれていない場合は、この関数を呼び出しても何も処理しません。
    //!
    //! @see    Start()
    //! @see    Stop()
    //---------------------------------------------------------------------------
    void Update() NN_NOEXCEPT;

    //@}
    // 開始と終了

    //! @internal
    //! @return :private
    nn::atk::viewer::SoundArchiveEditor& GetSoundArchiveEditor() NN_NOEXCEPT { return m_SoundArchiveEditor; }

    //! @internal
    //! @return :private
    nn::atk::viewer::SoundEditSession& GetSoundEditSession() NN_NOEXCEPT { return m_SoundEditSession; }

    //! @internal
    //! @return :private
    nn::atk::viewer::SoundObjectController& GetSoundObjectController() NN_NOEXCEPT { return m_SoundObjectController; }

    //! @internal
    //! @return :private
    nn::atk::viewer::SoundControlSession& GetSoundControlSession() NN_NOEXCEPT { return m_SoundControlSession; }

#if defined(NN_ATK_CONFIG_ENABLE_THREAD_CORE_NUMBER_OBSERVATION)
    //! @internal
    //! @return :private
    int GetWorkerThreadCoreNumber() const NN_NOEXCEPT;
#endif
    //===========================================================================


private: // メンバ変数
    bool m_IsInitialized;
    bool m_IsStarted;

    nn::atk::detail::fnd::FrameHeap m_Allocator;

    nn::atk::viewer::SoundArchiveEditor m_SoundArchiveEditor;
    nn::atk::viewer::SoundEditSession m_SoundEditSession;
    nn::atk::viewer::SoundObjectController m_SoundObjectController;
    nn::atk::viewer::SoundControlSession m_SoundControlSession;

    nn::atk::viewer::UpdateCachesHelper m_UpdateCachesHelper;
    char* m_pMemoryForUpdateCachesThreadStack;
    size_t m_UpdateCachesThreadStackSize;

    void* m_pMemoryForSoundArchiveEditor;
    void* m_pMemoryForSoundEditSession;
    void* m_pMemoryForSoundControlSession;

    int m_WorkerThreadCoreNumber;
};

} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
