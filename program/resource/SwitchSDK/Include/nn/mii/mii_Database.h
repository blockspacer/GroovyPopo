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

/**
 *  @file
 *  @brief データベースアクセス定義です。
 *  @details Miiを取得するためのアクセサを定義します。
 */


#include <nn/nn_Result.h>
#include <nn/util/util_TypedStorage.h>
#include <nn/mii/mii_Common.h>

namespace nn { namespace mii {

class RandomDesc; // 前方参照

/**
 * @brief データベースクラス
 * @details Miiを取得するためのインターフェイスを提供します。
 */
class Database
{
    NN_DISALLOW_COPY(Database) NN_NOEXCEPT;
    bool operator ==(const Database& rhs) const NN_NOEXCEPT; ///< 比較禁止
    bool operator !=(const Database& rhs) const NN_NOEXCEPT; ///< 比較禁止
public:
    Database() NN_NOEXCEPT;
    ~Database() NN_NOEXCEPT;

    //! @name 初期化・終了処理
    //! @{

    /**
     * @brief 開始処理
     * @return 成功するとnn::ResultSuccessを返す。
     * @post 成功するデータベースが利用可状態になります。
     * @details データベースの使用を開始します。
     *          データベースを利用するには
     *          この関数を呼び出し、処理を成功させる必要があります。
     *          pResultは、nn::ResultSuccess()時に有効です。
     *
     */
    nn::Result Initialize() NN_NOEXCEPT;

    /**
     * @brief 終了処理
     * @details データベースの使用を終了します。
     * @post データベースが利用不可状態になります。
     */
    void Finalize() NN_NOEXCEPT;

    /**
     * @brief 状態確認
     * @details データベースの使用状態を確認します。
     * @return データベースの状態を返します。
     * @retval true  利用可
     * @retval false 利用不可
     */
    bool IsAvailable() const NN_NOEXCEPT;

    //! @}


    //! @name 状態確認
    //! @{

    /**
     * @brief 状態更新が発生したかを判定します。
     * @param[in] sourceFlags @ref SourceFlag
     * @return 更新があればtrueを返します。
     * @details Miiに更新・追加・削除が発生したかを判定します。
     *          @n システムが保持するMiiが更新された場合にtrueを返します。
     *          更新は、アプリ・アプレットの遷移が発生したときに起こります。
     *          @n アプリ・アプレットから戻ってきたときにこの関数は、
     *          trueを返す場合があります。
     *          @n このメソッドはワンショットです。
     *          @n 呼び出すと状態がクリアされることに注意してください。
     */
    bool IsUpdated(int sourceFlags) NN_NOEXCEPT;

    //! @}

    //! @name 取得
    //! @{

    /**
     * @brief データベースの容量が満杯かを判定します。
     * @return 満杯ならtrueを返します。
     * @details GetCount(SourceFlag_Database) == DatabaseMiiCountの判定は行わず、こちらのメソッドを使ってください。
     *          GetCount(SourceFlag_Database) != DatabaseMiiCountの場合でも満杯の場合があります。
     */
    bool IsFullDatabase() const NN_NOEXCEPT;

    /**
     * @brief 指定条件の有効なMii件数を取得します。
     * @param[in] sourceFlags @ref SourceFlag
     * @return 有効なMiiの数を返します。
     * @details 指定した条件に合致するMiiの件数を返します。
     *          SourceFlag を複数指定した場合はその合計値を返します。
     */
    int GetCount(int sourceFlags) const NN_NOEXCEPT;

    /**
     * @brief 指定した CharInfo の データベース上のインデックスを取得します
     * @param[out] pOutIndex   index格納先
     * @param[in]  info        indexを取得したいCharInfo
     * @retresult
     *     @handleresult{nn::mii::ResultNotFound,該当Miiが見つかりません}
     *     @handleresult{nn::mii::ResultBroken,CharInfoが破損しています}
     * @endretresult
     * @details データベース上のインデックスを取得します。
     *          Get() メソッドに SourceFlag_Database を指定して取得した時の
     *          pElements 配列インデックスが格納されます。
     */
    nn::Result GetIndex(int* pOutIndex, const CharInfo& info) const NN_NOEXCEPT;

    /**
     * @brief 指定した条件のMiiを取得します。
     * @param[out] pOutCount   pElementsに格納した数
     * @param[out] pElements   格納先配列
     * @param[in]  count       pElementsの要素数
     * @param[in]  sourceFlags @ref SourceFlag
     * @retresult
     *     @handleresult{nn::mii::ResultOverflow,countよりもMiiが多い}
     * @endretresult
     * @details 指定した条件のMiiを取得元を含めて取得します。
     *          sourceFlags に 複数の SourceFlag を指定した場合は、次の順番で詰められて配置されます。
     *          @li SourceFlag_Database
     *          @li SourceFlag_Default
     */
    nn::Result Get(int* pOutCount,CharInfoElement* pElements,int count
        ,int sourceFlags) NN_NOEXCEPT;

    /**
     * @brief 指定した条件のMiiを取得します。
     * @param[out] pOutCount   pElementsに格納した数
     * @param[out] pElements   格納先配列
     * @param[in]  count       pElementsの要素数
     * @param[in]  sourceFlags @ref SourceFlag
     * @retresult
     *     @handleresult{nn::mii::ResultOverflow,countよりもMiiが多い}
     * @endretresult
     * @details 指定した条件のMiiを取得元を含めず取得します。
     *          sourceFlags に 複数の SourceFlag を指定した場合は、次の順番で詰められて配置されます。
     *          @li SourceFlag_Database
     *          @li SourceFlag_Default
     */
    nn::Result Get(int* pOutCount,CharInfo* pElements,int count,int sourceFlags) NN_NOEXCEPT;

    /**
     * @brief 指定したCharInfoの最新を取得します。
     * @param[in,out] pInfo       更新対象のCharInfo
     * @param[in]     sourceFlags @ref SourceFlag
     * @retresult
     *     @handleresult{nn::ResultSuccess,最新に更新した}
     *     @handleresult{nn::mii::ResultNotUpdated,更新はありません}
     *     @handleresult{nn::mii::ResultNotFound,見つかりませんでした}
     *     @handleresult{nn::mii::ResultBroken,CharInfo が壊れています}
     * @endretresult
     * @details 指定した @ref SourceFlag でMiiが更新されているかを取得します。
     */
    nn::Result UpdateLatest(CharInfo* pInfo,int sourceFlags) const NN_NOEXCEPT;

    /**
     * @brief 3DS/WiiU の StoreData を CharInfo に変換します。
     * @param[out] pInfo        変換した CharInfo の格納先を指定します。
     * @param[in] ver3StoreData 変換する Ver3StoreData を指定します。
     * @retresult
     *     @handleresult{nn::ResultSuccess,変換に成功}
     *     @handleresult{nn::mii::ResultBroken}
     * @endretresult
     * @details 3DS/WiiU の StoreData を CharInfo に変換します。
     *          @n NG ワード を含むニックネームは、ニックネームの文字数分の "?" で置き換えられます。
     *          @n 不正な文字コードを含むニックネームは "???" に置き換えられます。
     *          @n Mii のニックネームとして使用可能な文字コードは NintendoSDK/Resources/Mii/fontset 以下の文字フィルタファイルを参照してください。
     */
    nn::Result Convert(CharInfo* pInfo,const Ver3StoreData& ver3StoreData) NN_NOEXCEPT;

    /**
     * @brief ランダムMiiを作成します。
     * @param[out] pInfo 作成したMiiの格納先を指定します。
     * @param[in]  desc  ランダムデスクリプタ
     * @details desc 条件のランダムなMiiを作成します。
     */
    void BuildRandom(CharInfo* pInfo,const RandomDesc& desc) NN_NOEXCEPT;

    /**
     * @brief デフォルトMiiを作成します。
     * @param[out] pInfo 作成したMiiの格納先を指定します。
     * @param[in]  index デフォルトMiiのindex番号を指定します。
     * @pre 0 <= index < DefaultMiiCount
     *      && pDst != NULL
     * @details 指定index番号のデフォルトMiiを作成します。
     */
    void BuildDefault(CharInfo* pInfo,int index) const NN_NOEXCEPT;

    //! @}

    //! @name デバッグ用
    //! @{

    /**
     * @brief データベース初期化
     * @details データベースを空に変更します。
     *          @n Mii Testing Mode を Enabled に設定してから呼び出す必要があります。
     */
    void Format() NN_NOEXCEPT;

    /**
     * @brief データベースをインポートします。
     * @param[in] pBuffer インポートするデータベースのポインタ
     * @param[in] size    pBufferのサイズ(nn::mii::ImportFileSize)
     * @details 指定したデータベースファイルをインポートします。
     *          @n Mii Testing Mode を Enabled に設定してから呼び出す必要があります。
     */
    nn::Result Import(const void* pBuffer,size_t size) NN_NOEXCEPT;

    /**
     * @brief データベースをエクスポートします。
     * @param[in] pBuffer データベースエクスポート先ポインタ
     * @param[in] size    pBufferのサイズ(nn::mii::ImportFileSize)
     * @details 指定したデータベースをファイルにエクスポートします。
     *          @n Mii Testing Mode を Enabled に設定してから呼び出す必要があります。
     */
    void Export(void* pBuffer,size_t size) const NN_NOEXCEPT;

    //! @}

private:
    mutable nn::util::TypedStorage<void*, 64, 8> m_Storage;
};

}
}
