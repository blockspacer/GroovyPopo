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

#include <climits>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>
#include <nn/atk/fnd/basis/atkfnd_Memory.h>
#include <nn/atk/viewer/atk_Result.h>
#include <nn/atk/viewer/atk_Canceller.h>
#include <nn/atk/viewer/detail/atk_IErrorProvider.h>
#include <nn/atk/viewer/detail/atk_SoundArchiveEditController.h>
#include <nn/atk/viewer/detail/atk_SoundArchiveFileEditor.h>
#include <nn/atk/viewer/detail/atk_SoundArchiveParameterEditor.h>
#include <nn/atk/viewer/detail/res/atk_ResourceManager.h>

namespace nn {

namespace ut {
class Event;
}

namespace atk {

class SoundArchive;
class SoundArchivePlayer;

namespace viewer {

namespace detail {
class SoundEditConnection;
}

//---------------------------------------------------------------------------
//! @brief    【β版】サウンドアーカイブの編集機能を提供します。
//!
//!           編集を開始すると、指定した SoundArchive の内容を
//!           接続先の SoundMaker 上のサウンドプロジェクトの内容に差し替えます。
//---------------------------------------------------------------------------
class SoundArchiveEditor : private detail::IErrorProvider
{
    NN_DISALLOW_COPY(SoundArchiveEditor);

public: // 定数の定義
    static const int DefaultEditableItemCountMax = 64;       //!< 編集アイテム数のデフォルト最大値です。
    static const int DefaultEditableFileCountMax = 128;      //!< 編集ファイル数のデフォルト最大値です。
    static const int DefaultItemNameLengthMax    = 256;      //!< アイテム名の長さのデフォルト最大値です。

public: // 型の定義
    //---------------------------------------------------------------------------
    //! @brief  SoundArchiveEditor 設定を格納する構造体です。
    //!
    //!         各パラメータには、コンストラクタでデフォルト値が設定されます。
    //!
    //! @see    Initialize()
    //! @see    GetRequiredMemorySize()
    //---------------------------------------------------------------------------
    struct Configs
    {
        //---------------------------------------------------------------------------
        //! @brief  コンストラクタです。
        //---------------------------------------------------------------------------
        Configs() NN_NOEXCEPT :
            maxEditableItems(DefaultEditableItemCountMax),
            maxEditableFiles(DefaultEditableFileCountMax),
            maxItemName(DefaultItemNameLengthMax),
            isEditableItemsCountMaxExpandable(true),
            isEditableFilesCountMaxExpandable(true)
        {
        }

        int maxEditableItems;                       //!< 編集アイテム数の最大値を指定します。
                                                    //!< @n
                                                    //!< 指定しないとデフォルト値 DefaultEditableItemCountMax が使用されます。
                                                    //!< @n
                                                    //!< よりたくさんのアイテムを対象にしたい場合や、対象アイテム数が少ないことが事前にわかっていて、
                                                    //!< 少しでもメモリ使用量を減らしたい場合に設定を変更してください。
        int maxEditableFiles;                       //!< 編集ファイル数の最大値を指定します。
                                                    //!< @n
                                                    //!< 指定しないとデフォルト値 DefaultEditableFileCountMax が使用されます。
                                                    //!< @n
                                                    //!< よりたくさんのファイルを対象にしたい場合や、対象ファイル数が少ないことが事前にわかっていて、
                                                    //!< 少しでもメモリ使用量を減らしたい場合に設定を変更してください。
                                                    //!< @n
                                                    //!< isEditableFilesCountMaxExpandable が true の場合、この値は初期値として使用されます。
        int maxItemName;                            //!< アイテム名の長さの最大値を指定します。
                                                    //!< @n
                                                    //!< 指定しないとデフォルト値 DefaultItemNameLengthMax が使用されます。
                                                    //!< @n
                                                    //!< 非常に長いアイテム名を対象にしたい場合や、使用する名前が短いことが事前にわかっていて、
                                                    //!< 少しでもメモリ使用量を減らしたい場合に編集してください。
        bool isEditableItemsCountMaxExpandable;     //!< 編集アイテム数の動的な拡張の有無を指定します。
                                                    //!< @n
                                                    //!< true を指定すると、編集可能アイテム数の上限に達した際、動的にメモリ領域を拡張します。
                                                    //!< @n
                                                    //!< その際、メモリ領域の再割り当てが行われるため、断片化の要因にもなり得ます。
                                                    //!< @n
                                                    //!< false を指定すると maxEditableItems に指定した値の範囲内で動作します。
        bool isEditableFilesCountMaxExpandable;     //!< 編集ファイル数の動的な拡張の有無を指定します。
                                                    //!< @n
                                                    //!< true を指定すると、編集可能ファイル数の上限に達した際、動的にメモリ領域を拡張します。
                                                    //!< @n
                                                    //!< その際、メモリ領域の再割り当てが行われるため、断片化の要因にもなり得ます。
                                                    //!< @n
                                                    //!< false を指定すると maxEditableFiles に指定した値の範囲内で動作します。
    };


public: // コンストラクタ
    //===========================================================================
    //! @name コンストラクタ/デストラクタ
    //@{

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //---------------------------------------------------------------------------
    SoundArchiveEditor() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  デストラクタです。
    //---------------------------------------------------------------------------
    virtual ~SoundArchiveEditor() NN_NOEXCEPT NN_OVERRIDE;

    //@}
    // コンストラクタ/デストラクタ
    //===========================================================================

public: // メソッド
    //===========================================================================
    //! @name 初期化
    //@{

    //---------------------------------------------------------------------------
    //! @brief      SoundArchiveEditor で利用するメモリを初期化し、
    //!             編集可能なサウンド数の上限値や、編集対象のサウンドアーカイブを設定します。
    //!
    //!             SoundArchiveEditor を利用する前に、必ずこの関数を呼び出してください。
    //!             @n
    //!             サウンドアーカイブ編集が開始されると、
    //!             ここで指定した SoundArchive に対するアクセスは、
    //!             この SoundArchiveEditor によってフックされ、SoundMaker 上の
    //!             サウンドプロジェクトの内容が優先して利用されるようになります。
    //!
    //!             サウンドアーカイブ編集の開始、終了は、SoundEditSession により制御されます。
    //!
    //!             ■バッファについて
    //!             @n
    //!             ここで指定するバッファは、パラメータや波形ファイル等、各種データのキャッシュに利用されます。
    //!
    //!             メモリ使用量は、実際に利用するサウンド数や波形に依存して変化するため、
    //!             事前に必要量を取得できません。
    //!             @n
    //!             編集対象データに合わせて十分なサイズのバッファを指定してください。
    //!             @n
    //!             SoundArchiveEditor 利用中にメモリが足りなくなった場合は、
    //!             それ以上編集対象アイテムを増やせなくなります。
    //!
    //!             また、インゲーム編集対象にする、対象から外す操作を繰り返していると、
    //!             このバッファ内でメモリの断片化が進行していきます。
    //!             @n
    //!             まだ空き容量があったとしても、連続したメモリが確保できずに失敗することがありますので、
    //!             大きめのバッファを確保されることを推奨します。
    //!             @n
    //!             メモリの断片化が発生した際には、SoundMaker を使って、すべてのキャッシュを再転送することで、
    //!             症状を解消することができます。
    //!
    //! @param[in]  buffer              SoundArchiveEditor が利用するバッファを指定します。
    //! @param[in]  bufferLength        buffer の長さを指定します。
    //! @param[in]  soundArchive        編集対象のサウンドアーカイブを指定します。
    //! @param[in]  soundArchivePlayer  編集対象のサウンドアーカイブプレイヤーを指定します。
    //! @param[in]  configs             SoundArchiveEditor の設定を指定します。
    //!
    //! @return     処理結果を返します。
    //!
    //! @see        Configs
    //! @see        Finalize()
    //! @see        IsInitialized()
    //---------------------------------------------------------------------------
    Result Initialize(
        void* buffer,
        size_t bufferLength,
        SoundArchive* soundArchive,
        SoundArchivePlayer* soundArchivePlayer,
        const Configs& configs) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief       SoundArchiveEditor によるサウンドアーカイブ編集を終了し、
    //!              通常のサウンドアーカイブ操作に戻します。
    //!
    //! @see         Initialize()
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;

    //@}
    // 初期化
    //===========================================================================

    //===========================================================================
    //! @name 状態の取得
    //@{

    //---------------------------------------------------------------------------
    //! @brief   現在のメモリ使用量（バイト数）を取得します。
    //!
    //! @return  現在のメモリ使用量を返します。
    //!
    //! @see     GetMemoryMax()
    //! @see     IsOutOfMemory()
    //---------------------------------------------------------------------------
    size_t GetMemoryUsage() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief   SoundArchiveEditor が利用可能なメモリのサイズ（バイト数）を取得します。
    //!
    //! @return  利用可能なメモリのサイズを返します。
    //!
    //! @see     GetMemoryUsage()
    //! @see     IsOutOfMemory()
    //---------------------------------------------------------------------------
    size_t GetMemoryMax() const NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief   現在編集中のアイテム数を取得します。
    //!
    //! @return  現在編集中のアイテム数を返します。
    //!
    //! @see     GetEditableItemCountMax()
    //! @see     IsEditableItemsCountMaxExpandable()
    //---------------------------------------------------------------------------
    int GetEditItemCount() const NN_NOEXCEPT
    {
        return m_ResourceManager.GetItemInfoCount();
    }

    //---------------------------------------------------------------------------
    //! @brief   編集可能なアイテム数の最大値を取得します。
    //!
    //! @return  編集可能なアイテム数の最大値を返します。
    //!
    //! @see     GetEditItemCount()
    //! @see     IsEditableItemsCountMaxExpandable()
    //---------------------------------------------------------------------------
    int GetEditableItemCountMax() const NN_NOEXCEPT
    {
        return m_ResourceManager.GetMaxItemInfoCount();
    }

    //---------------------------------------------------------------------------
    //! @brief      現在編集中のアイテム一覧から指定インデックスのアイテム情報を取得します。
    //!
    //! @param[out] pOutValue  アイテム情報を格納する EditItemInfo 構造体へのポインタを指定します。
    //! @param[in]  index      アイテムのインデックスを指定します。
    //!
    //! @return     アイテム情報の取得に成功した場合は true、失敗した場合は false を返します。
    //!
    //! @see        GetEditItemCount()
    //---------------------------------------------------------------------------
    bool TryGetEditItemInfo( EditItemInfo* pOutValue, int index ) const NN_NOEXCEPT
    {
        return m_ResourceManager.TryGetEditItemInfo(index, pOutValue);
    }

    //---------------------------------------------------------------------------
    //! @brief   現在編集中のファイル数を取得します。
    //!
    //! @return  現在編集中のファイル数を返します。
    //!
    //! @see     GetEditableFileCountMax()
    //! @see     IsEditableFilesCountMaxExpandable()
    //---------------------------------------------------------------------------
    int GetEditFileCount() const NN_NOEXCEPT
    {
        return m_ResourceManager.GetFileCount();
    }

    //---------------------------------------------------------------------------
    //! @brief   編集可能なファイル数の最大値を取得します。
    //!
    //! @return  編集可能なファイル数の最大値を返します。
    //!
    //! @see     GetEditFileCount()
    //! @see     IsEditableFilesCountMaxExpandable()
    //---------------------------------------------------------------------------
    int GetEditableFileCountMax() const NN_NOEXCEPT
    {
        return m_ResourceManager.GetMaxFileCount();
    }

    //---------------------------------------------------------------------------
    //! @brief   編集可能なアイテムの最大数を動的に拡張可能かどうかを取得します。
    //!
    //! @return  動的に拡張可能な場合は true、不可能な場合は false を返します。
    //!
    //! @see     GetEditableItemCountMax()
    //---------------------------------------------------------------------------
    bool IsEditableItemsCountMaxExpandable() const NN_NOEXCEPT
    {
        return m_ResourceManager.IsItemInfoDictionaryExpandable();
    }

    //---------------------------------------------------------------------------
    //! @brief   編集可能なファイルの最大数を動的に拡張可能かどうかを取得します。
    //!
    //! @return  動的に拡張可能な場合は true、不可能な場合は false を返します。
    //!
    //! @see     GetEditableFileCountMax()
    //---------------------------------------------------------------------------
    bool IsEditableFilesCountMaxExpandable() const NN_NOEXCEPT
    {
        return m_ResourceManager.IsFileDictionaryExpandable();
    }

    //---------------------------------------------------------------------------
    //! @brief   メモリ不足状態かどうかを調べます。
    //!
    //!          メモリ不足状態を解消するためには、SoundMaker でアイテムを編集対象から外すなどして、
    //!          メモリ使用量を減らす必要があります。
    //!
    //! @return  メモリ不足状態なら true、そうでなければ、false を返します。
    //!
    //! @see     GetMemoryUsage()
    //! @see     GetMemoryMax()
    //---------------------------------------------------------------------------
    bool IsOutOfMemory() const NN_NOEXCEPT { return m_EditController.IsOutOfMemory(); }

    //---------------------------------------------------------------------------
    //! @brief  アイテム数のオーバーフローの有無を取得します。
    //!
    //! @return  オーバーフロー状態なら true、そうでなければ、false を返します。
    //---------------------------------------------------------------------------
    bool IsItemInfosOverflow() const NN_NOEXCEPT { return m_EditController.IsItemInfosOverflow(); }

    //---------------------------------------------------------------------------
    //! @brief  ファイル数のオーバーフローの有無を取得します。
    //!
    //! @return  オーバーフロー状態なら true、そうでなければ、false を返します。
    //---------------------------------------------------------------------------
    bool IsFilesOverflow() const NN_NOEXCEPT { return m_EditController.IsFilesOverflow(); }

    //---------------------------------------------------------------------------
    //! @brief   初期化の有無を取得します。
    //!
    //! @return  初期化済みの場合は true、初期化されていない場合は false を返します。
    //!
    //! @see     Initialize()
    //---------------------------------------------------------------------------
    bool IsInitialized() const NN_NOEXCEPT { return m_EditController.IsInitialized(); }

    //---------------------------------------------------------------------------
    //! @brief   編集開始の有無を取得します。
    //!
    //! @return  編集を開始済みの場合は true、編集が開始されていない場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsStarted() const NN_NOEXCEPT { return m_EditController.IsStarted(); }

    //@}
    // 状態の取得
    //===========================================================================

    //===========================================================================
    //! @name 開始と終了
    //@{
    //@}

    //---------------------------------------------------------------------------
    //! @internal
    //! @brief  編集を開始します。
    //! @param connection :private
    //! @return :private
    //---------------------------------------------------------------------------
    Result detail_Start(detail::SoundEditConnection& connection) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @internal
    //! @brief  編集を終了します。
    //---------------------------------------------------------------------------
    void detail_Stop() NN_NOEXCEPT;

    // 開始と終了
    //===========================================================================

    //===========================================================================
    //! @name キャッシュ管理
    //@{

    //---------------------------------------------------------------------------
    //! @brief   キャッシュを更新します。
    //!
    //!          すべての未キャッシュアイテムに対して、キャッシュ処理を行います。
    //!          @n
    //!          SoundMaker で指定した編集対象アイテムを有効化する（キャッシュする）には、
    //!          必ずこの関数を呼び出す必要があります。
    //!
    //!          この関数は、すべてのキャッシュが完了するか、エラーが発生するまで制御を返さないため、
    //!          ワーカースレッド上で実行することを推奨します。
    //!
    //!          ヘルパークラスである UpdateCachesHelper を利用すると、
    //!          内部でワーカースレッドを立て、そのスレッドからこの関数を呼び出してくれます。
    //!          @n
    //!          UpdateCachesHelper を利用する場合は、別途この関数を呼び出す必要はありません。
    //!
    //!          ■処理の中断について
    //!          @n
    //!          canceller を指定すると、キャッシュ更新を中断することができます。
    //!          @n
    //!          キャッシュ更新中に ICanceller::IsCanceled() が true を返すと、
    //!          UpdateCaches() は、現在処理中のアイテムキャッシュ処理を待って制御を返します。
    //!
    //!          ICanceller の標準実装として、Canceller, TimeoutCanceller があります。
    //!          @n
    //!          ICanceller を継承すると、処理の中断条件を独自実装することもできます。
    //!
    //! @param[in]  canceller      キャッシュ更新のキャンセルオブジェクトを指定します。
    //! @param[in]  cancellerParam  キャンセルオブジェクトに渡すパラメータを指定します。
    //!
    //! @return  処理結果を返します。
    //!
    //! @see     ICanceller
    //! @see     Canceller
    //! @see     TimeoutCanceller
    //! @see     UpdateCachesHelper
    //---------------------------------------------------------------------------
    Result UpdateCaches(ICanceller* canceller = NULL, void* cancellerParam = NULL) NN_NOEXCEPT;
    //@}

    //---------------------------------------------------------------------------
    //! @internal
    //! @brief  すべてのアイテムキャッシュを更新します。
    //---------------------------------------------------------------------------
    void detail_UpdateAllItemCaches() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @internal
    //! @brief  アイテムキャッシュを更新します。
    //! @param name :private
    //! @return :private
    //---------------------------------------------------------------------------
    Result detail_UpdateItemCache(const char* name) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @internal
    //! @brief  すべてのアイテムキャッシュを削除します。
    //---------------------------------------------------------------------------
    void detail_RemoveAllItemCaches() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @internal
    //! @brief  指定した名前のアイテム情報キャッシュを削除します。
    //! @param name :private
    //! @return :private
    //---------------------------------------------------------------------------
    Result detail_RemoveItemCache(const char* name) NN_NOEXCEPT;

    //! @name キャッシュ管理
    //@{
    //---------------------------------------------------------------------------
    //! @brief   指定アイテムのキャッシュ状態を取得します。
    //!
    //!          この関数が CacheState_Cached を返す場合、
    //!          対象 SoundArchivePlayer で StartSound() を呼び出すと、
    //!          パラメータやデータの差し替えが発生します。
    //!
    //!          CACHE_STATE_CACHING を返す場合、
    //!          キャッシュ処理中か、メモリ不足やそのほかのエラーにより
    //!          キャッシュ処理が停止している可能性があります。
    //!
    //!          メモリ不足の有無は IsOutOfMemory() で、
    //!          エラーの発生状況は GetLastError() で確認できます。
    //!
    //! @param[in]  name  サウンド名またはバンク名を指定します。
    //!
    //! @return  指定アイテムのキャッシュ状態を返します。
    //!
    //! @see     CacheState
    //! @see     IsOutOfMemory()
    //! @see     GetLastError()
    //---------------------------------------------------------------------------
    CacheState GetItemCacheState(const char* name) const NN_NOEXCEPT;
    //@}

    //---------------------------------------------------------------------------
    //! @internal
    //! @brief  編集アイテムの変更通知コールバックを設定します。
    //! @param callback :private
    //! @param callbackParam :private
    //---------------------------------------------------------------------------
    void detail_SetEditItemsChangedCallback(EditItemsChangedCallback callback, uintptr_t callbackParam) NN_NOEXCEPT
    {
        m_EditController.SetEditItemsChangedCallback(callback, callbackParam);
    }

    // キャッシュ管理
    //===========================================================================

    //===========================================================================
    //! @name エラーハンドリング
    //@{

    //---------------------------------------------------------------------------
    //! @brief         最後に発生したエラー情報を取得します。
    //!
    //! @return        最後に発生したエラー情報を返します。
    //!
    //! @see           SetLastError()
    //---------------------------------------------------------------------------
    virtual Result GetLastError() const NN_NOEXCEPT NN_OVERRIDE { return m_LastError; }

    //---------------------------------------------------------------------------
    //! @brief         最後に発生したエラー情報を設定します。
    //!
    //! @param[in]     value  エラー値を指定します。
    //!
    //! @see           GetLastError()
    //---------------------------------------------------------------------------
    virtual void SetLastError(ResultType value) NN_NOEXCEPT NN_OVERRIDE;

    //@}
    // エラーハンドリング
    //===========================================================================

private: // メンバ変数
    Result m_LastError;         //!< 最後に発生したエラー情報です。
    int    m_MaxItemName;       //!< アイテム名の長さの最大値です。
    size_t m_TotalBufferLength; //!< SoundArchiveEditor 用の合計バッファサイズです。（PC通知用）

    detail::SoundArchiveEditController  m_EditController;             //!< 操作クラスです。
    detail::ResourceManager             m_ResourceManager;            //!< リソース管理クラスです。

    detail::SoundArchiveParameterEditor m_ParameterEditor;            //!< パラメータエディタです。
    detail::SoundArchiveFileEditor      m_FileEditor;                 //!< ファイルエディタです。

    detail::SoundEditConnection*        m_EditConnection;             //!< エディット接続制御クラスです。

    atk::detail::fnd::CriticalSection m_ConnectionLockObject;      //!< 接続制御のロックです。
};

} // namespace nn::atk::viewer
} // namespace nn::atk
} // namespace nn

#endif // NN_ATK_CONFIG_ENABLE_DEV
