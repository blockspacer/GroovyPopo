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
 * @file
 * @brief クライアントから消費型サービスアイテムを利用するためのAPIの定義
 */

#pragma once
#include <nn/nn_Common.h>
#include <nn/nn_Result.h>
#include <nn/account/account_Types.h>
#include <nn/ec/ec_ConsumableServiceItemTypes.h>
#include <nn/ec/ec_ResultConsumableServiceItem.h>
#include <nn/ec/ec_ShopServiceAccessor.h>
#include <nn/err.h>
#include <nn/mem/mem_StandardAllocator.h>
#include <nn/os/os_SystemEvent.h>
#include <nn/time/time_PosixTime.h>

namespace nn { namespace ec {

//! 前方宣言
class AsyncGetConsumableRightDataRequest;
class AsyncRecoverRightDataRequest;
class AsyncConsumeRightDataRequest;
struct ConsumableServiceItemRightData;
enum ConsumableServiceItemManagerState : int;

/**
    @brief 消費型サービスアイテムを、クライアント上に保持するためのマネージャクラス

    @details
        概要:
        本クラスはアプリケーションからワークメモリを受け取り、消費型サービスアイテムの権利情報を管理します。@n
        本クラスが内部で保持している権利情報を、アプリケーションから直接操作することはできません。権利情報の操作には、以下の方法を利用します。
        @li 別途定義された非同期要求 ConsumableServiceItemAsyncRequestBase の派生クラスを、本クラスに渡して非同期要求を実行する
        @li 本クラスのメソッドを使って、内部に保持する権利情報を操作する

        機能:
        サーバーとの通信を必要とする処理は、非同期要求の派生クラスを介して提供されます。@n
        呼び出し方法、および非同期操作については該当するクラスのリファレンスを参照してください。
        @li AsyncGetConsumableRightDataRequest                          サーバー上で未消費な権利情報を取得する
        @li AsyncConsumeRightDataRequest                                保持している権利情報を、サーバーから消費してクライアントで提供可能にする
        @li AsyncRecoverRightDataRequest                                電源断等からの復帰を目的として、保持している権利情報の提供可能状態を復旧する

        それ以外の処理は、ConsumableServiceItemManager クラスのメソッドとして提供されます。@n
        非同期要求の初期化、保持しているデータのシリアライズ、アイテム ID の提供処理がメソッドとして提供されます。
        @li CanQueryNewData, IsConsumptionRequired, IsRecoveryRequired  ConsumableServiceItemManager オブジェクトの内部状態から、各非同期要求が実行可能かを判定する
        @li SetupAsyncRequest                                           受け取った非同期要求オブジェクトを、このオブジェクトを参照する形式で初期化する
        @li ExportSerializedRightData                                   保持している権利情報を、セーブデータに保存できる形式にシリアライズする
        @li GetProvidableItemIds                                        提供可能な消費型サービスアイテムのアイテム ID を取得する
        @li MarkServiceProvided                                         指定したアイテム ID を提供済みとして、管理対象から削除する

        非同期要求に関する注意:
        @li AsyncConsumeRightDataRequest を実行する前には、必ず、シリアライズしたデータをセーブデータに保存・コミットしてください。@n
            こうすることで、権利消費中に電源断等が発生しても、再起動後セーブデータから読み込むことで、ユーザーの購入した権利情報が保護されます。
        @li 1 個の ConsumableServiceItemManager オブジェクトに対して、複数の非同期要求オブジェクトを同時に参照させることはできません。@n
            そのため、使用完了した非同期要求オブジェクトは、ただちに破棄してください。@n
            オブジェクトの破棄が行われないまま、別の非同期要求オブジェクトを、このオブジェクトを参照する形式で初期化しないでください。

        データのシリアライズに関する注意:
        @li アプリケーションから本クラスには、ExportSerializedRightData() で取得したシリアライズされた権利情報しか入力しないでください。@n
            それ以外のデータが入力された場合には、不当なデータが入力されたものとしてエラーを返します。
        @li シリアライズされた権利情報を、アプリケーションで書き換えてはいけません。書き換えた場合、権利情報は不正な状態となります。@n
            不正な状態の権利情報は、非同期要求の実行、および本クラスでの操作ができなくなります。@n
            その結果、消費型サービスアイテムをユーザーが利用できなくなる可能性があります。

        ユーザーへのサービス提供に関する注意:
        @li ユーザーが購入した消費型サービスアイテムは、最終的に、アプリケーション内でユーザーにとっての価値に変換する必要があります。@n
            ユーザーへの最終的な価値を提供することを、ここでは「サービスの提供」と呼びます。@n
            このライブラリでは「サービスを提供済み扱いにするのと同時に、アプリケーションが自らユーザーへ価値を提供する」ことでサービスの提供を行います。
        @li 「サービスの提供」を具体例で示します。前提として、「やくそう 10 枚」を取得できる消費型サービスアイテムが、アイテム ID = "MedicalHarb10" として登録されており@n
            その権利情報をユーザーが 1 件購入して、サーバーからセーブデータへ提供可能な状態で取得済みなケースを想定します。@n
            権利情報のままでは「やくそう」として利用できないため、アイテム ID = "MedicalHarb10" を判定して、アプリケーションが「やくそう」を 10 枚増やす処理を実行します。@n
            それと同時に、"MedicalHarb10" 指定で MarkServiceProvided を実行して、"MedicalHarb10" を示す権利情報が提供された扱いにします。@n
            「やくそう」の増やした結果と、"MedicalHarb10" を提供済み扱いにした結果を、同時にセーブデータにコミットすることで不可分に情報が永続化されます。@n
            これにより、ユーザーは、ゲームの中で「やくそう」を 10 枚取得するという価値を得たことになります。この一連の流れを「サービスの提供」と呼びます。
        @li 消費型サービスアイテムの権利情報を、「やくそう」そのものとして使わないでください。@n
            権利情報は、ユーザーが価値を取得する権利を示すものであり、価値そのものではありません。
        @li サービスを提供済み扱いにするのと、ユーザーへ価値を提供するのは、セーブデータコミットなどを使用して不可分に行ってください。@n
            どちらかの状態のみが永続化された場合、ユーザーが購入した価値が不当に失われたり、重複して享受できたりする恐れがあります。
        @li ユーザーへ価値を提供する処理は、アプリケーション独自に定義してください。@n
            また、アイテム ID と価値の提供処理との対応付けも、アプリケーション独自に定義してください。
        @li 本クラスは、保持する権利情報が最大数（100 件）に達した時点で、それ以上、権利情報を取得できなくなります。@n
            この時、ユーザーが新規に購入した消費型サービスアイテムを、サーバーからローカルに取得できなくなります。@n
            これを回避するために、このオブジェクトが保持する権利情報は提供可能になり次第、できるかぎり即座にサービスの提供を行ってください。
        @li 上記の理由から、権利情報の状態でセーブデータに保持して、権利情報の価値をユーザーが享受するタイミングで、都度、サービスの提供を行う使い方は推奨しません。@n
            これは、上記の具体例でいえば、ユーザーが「やくそう」を使用するタイミングで、アプリケーションが権利情報を「やくそう」に変換させることに相当します。@n
            権利情報をユーザーセーブデータへ保存するのは、電源断等からの保護のためにご利用ください。
        @li 本クラスが保持する権利状態は、未提供な状態でセーブデータに保存して、それに「セーブデータお預かり」サービスをすることで@n
            ユーザーの操作で未提供な状態に巻き戻すことが可能です。@n
            ユーザーにとっての価値が提供のたびに変わるアイテムの場合、未提供な状態に戻す操作を繰り返すことで、より高い価値を選ぶことができます。@n
            これを回避するために、価値を固定化するヒントとなる乱数値を取得できます。@n
            この乱数値は、セーブデータが戻ったとしても同じアイテムを提供するときには同じ乱数値になります。
*/
class ConsumableServiceItemManager
{
    NN_DISALLOW_COPY(ConsumableServiceItemManager);
    NN_DISALLOW_MOVE(ConsumableServiceItemManager);

public:

    /**
        @brief コンストラクタ
    */
    ConsumableServiceItemManager() NN_NOEXCEPT;

    /**
        @brief デストラクタ
    */
    ~ConsumableServiceItemManager() NN_NOEXCEPT;

    /**
        @brief 消費型サービスアイテムマネージャを初期化します

        @param[in]  pAccessor       OwnedConsumableServiceItem で初期化したアクセッサ
        @param[in]  user            権利情報を参照するユーザー
        @param[in]  pWorkMemory     ワークメモリ
        @param[in]  workMemorySize  ワークメモリのサイズ

        @retval     nn::ResultSuccess                                       処理が成功しました。
        @retval     nn::ec::ResultInsufficientWorkMemory                    ConsumableServiceItemManager のワークメモリが不足しています。
                                                                            初期化時に nn::ec::RequiredWorkMemorySizeForConsumableServiceItem byte 以上のワークメモリを渡してください。

        @pre
            - IsInitialized() == false
            - pAccessor != nullptr
            - pAccessor.IsAvailable() == true
            - static_cast<bool>(user) == true
            - workMemory != nullptr
            - workMemorySize >= RequiredWorkMemorySizeForConsumableServiceItem

        @details
            pAccessor で入力されるアクセッサは、nn::ec::ShopServiceAccessor のリファレンスを参照して適切に初期化してください。@n
            初期化が適切に行われていない場合、非同期要求に関する操作で、ShopServiceAccessor のエラーが返ります。
    */
    nn::Result Initialize(ShopServiceAccessor* pAccessor, const account::Uid& user, void* pWorkMemory, size_t workMemorySize) NN_NOEXCEPT;

    /**
        @brief 消費型サービスアイテムマネージャを、シリアライズされた権利情報を保持させた状態で初期化します

        @param[in]  pAccessor       OwnedConsumableServiceItem で初期化したアクセッサ
        @param[in]  user            権利情報を参照するユーザー
        @param[in]  pWorkMemory     ワークメモリ
        @param[in]  workMemorySize  ワークメモリのサイズ
        @param[in]  buffer          データ
        @param[in]  bufferSize      データサイズ

        @retval     nn::ResultSuccess                                       処理が成功しました。
        @retval     nn::ec::ResultInsufficientWorkMemory                    ConsumableServiceItemManager のワークメモリが不足しています。
                                                                            初期化時に nn::ec::RequiredWorkMemorySizeForConsumableServiceItem byte 以上のワークメモリを渡してください。
        @retval     nn::ec::ResultConsumableServiceItemInvalidSaveData      入力された buffer, bufferSize では初期化できません。エラービュワーに表示してください。

        @pre
            - IsInitialized() == false
            - pAccessor != nullptr
            - pAccessor.IsAvailable() == true
            - static_cast<bool>(user) == true
            - workMemory != nullptr
            - workMemorySize >= RequiredWorkMemorySizeForConsumableServiceItem
            - buffer != nullptr
            - bufferSize > 0

        @details
            シリアライズされた権利情報がユーザーセーブデータに存在する場合、本 API を利用して初期化してください。@n

            本 API を実行した後、入力した権利情報がマネージャに保持された状態になります。@n
            また、提供可否が不明な権利情報が入力された場合、マネージャは権利復旧が必要な状態 (IsRecoveryRequired() == true) になります。@n

            pAccessor で入力されるアクセッサは、nn::ec::ShopServiceAccessor のリファレンスを参照して適切に初期化してください。@n
            初期化が適切に行われていない場合、非同期要求に関する操作で、ShopServiceAccessor のエラーが返ります。
    */
    nn::Result Initialize(ShopServiceAccessor* pAccessor, const account::Uid& user, void* pWorkMemory, size_t workMemorySize, const void* buffer, size_t bufferSize) NN_NOEXCEPT;

    /**
        @brief 消費型サービスアイテムマネージャが初期化済みかどうかを判定します

        @return 初期化済みかどうかを返します
    */
    bool IsInitialized() NN_NOEXCEPT;

    /**
        @brief このオブジェクトに対して、権利情報を新規に取得する要求ができるかを判定します。

        @details
            CanQueryNewData() == true を満たす場合、AsyncGetConsumableRightDataRequest を呼び出すことができます。@n
            この呼び出しにより、消費型サービスアイテムの権利情報を、サーバーから新規に取得することができます。
    */
    bool CanQueryNewData() NN_NOEXCEPT;

    /**
        @brief 保持している権利情報が、権利消費を必要としているかを判定します。

        @details
            IsConsumptionRequired() == true を満たす場合、AsyncConsumeRightDataRequest を呼び出すことで保持する権利情報を消費してください。
    */
    bool IsConsumptionRequired() NN_NOEXCEPT;

    /**
        @brief 保持している権利情報が、権利復旧を必要としているかを判定します。

        @details
            Initialize を実行した直後と、権利消費に失敗した直後に、このオブジェクトは権利復旧が必要になることがあります。@n
            IsRecoveryRequired() == true を満たす場合、AsyncRecoverRightDataRequest を呼び出すことで保持する権利情報の状態を復旧してください。@n

            IsRecoveryRequired() == true を満たすうちは、AsyncGetConsumableRightDataRequest を使用して権利情報を新規に取得すること@n
            および、AsyncConsumeRightDataRequest を使用して権利情報を消費することはできません。
    */
    bool IsRecoveryRequired() NN_NOEXCEPT;

    /**
        @brief このオブジェクトに対して、未消費な権利情報を取得する非同期要求を設定します

        @param[out]  pOutRequest    非同期要求インスタンス

        @pre
            - pOutRequest != nullptr
            - IsInitialized() == true

        @post
            - pOutRequest が初期化済みになります

        @details
            本 API 実行後、pOutRequest が初期化済みになります。@n
            AsyncGetConsumableRightDataRequest のリファレンスに明記された非同期要求処理が実行できるようになります。@n

            本 API 実行後、pOutRequest を用いた非同期要求が完了するまで ConsumableServiceItemManager オブジェクトを破棄しないでください。
    */
    void SetupAsyncRequest(AsyncGetConsumableRightDataRequest* pOutRequest) NN_NOEXCEPT;

    /**
        @brief このオブジェクトに対して、権利情報を消費する非同期要求を設定します

        @param[out]  pOutRequest    非同期要求インスタンス

        @pre
            - pOutRequest != nullptr
            - IsInitialized() == true

        @post
            - pOutRequest が初期化済みになります

        @details
            本 API 実行後、pOutRequest が初期化済みになります。@n
            AsyncConsumeRightDataRequest のリファレンスに明記された非同期要求処理が実行できるようになります。@n

            本 API 実行後、pOutRequest を用いた非同期要求が完了するまで ConsumableServiceItemManager オブジェクトを破棄しないでください。
    */
    void SetupAsyncRequest(AsyncConsumeRightDataRequest* pOutRequest) NN_NOEXCEPT;

    /**
        @brief このオブジェクトに対して、権利情報の提供可能状態を復旧する非同期要求を設定します

        @param[out]  pOutRequest    非同期要求インスタンス

        @pre
            - pOutRequest != nullptr
            - IsInitialized() == true

        @post
            - pOutRequest が初期化済みになります

        @details
            本 API 実行後、pOutRequest が初期化済みになります。@n
            AsyncRecoverRightDataRequest のリファレンスに明記された非同期要求処理が実行できるようになります。@n

            本 API 実行後、pOutRequest を用いた非同期要求が完了するまで ConsumableServiceItemManager オブジェクトを破棄しないでください。
    */
    void SetupAsyncRequest(AsyncRecoverRightDataRequest* pOutRequest) NN_NOEXCEPT;

    /**
        @brief シリアライズされた権利情報を取得するのに必要なバッファサイズを求めます

        @details
            権利情報を保持していない場合でも、必要なバッファサイズが適切に返ります。
    */
    size_t GetRequiredBufferSizeToExportSerializedRightData() const NN_NOEXCEPT;

    /**
        @brief セーブデータに保存できる形式にシリアライズした権利情報を取得します

        @param[out]  buffer         データの格納先
        @param[in]   bufferSize     buffer の領域サイズ

        @pre
            - buffer != nullptr
            - GetRequiredBufferSizeToExportSerializedRightDatas <= bufferSize

        @return     シリアライズしたデータサイズを返します

        @details
            本 API で取得したデータを、ユーザーセーブデータに保存する場合
            RequiredUserSaveDataSizeForConsumableServiceItem Byte 以上の空き容量を用意してください。
    */
    size_t ExportSerializedRightData(void* buffer, size_t bufferSize) const NN_NOEXCEPT;

    /**
        @brief 提供可能な消費型サービスアイテムの個数を取得します

        @return     提供可能な消費型サービスアイテムの個数を返します
    */
    int GetProvidableItemIdCount() const NN_NOEXCEPT;

    /**
        @brief 提供可能な消費型サービスアイテムのアイテム ID を取得します

        @param[out]  pOutItemIdCount    アイテム ID の個数
        @param[out]  outItemIdArray     アイテム ID の格納先
        @param[in]   itemIdArrayCount   outItemIdArray に取得できるアイテム ID の最大数

        @retval     nn::ResultSuccess                                       処理が成功しました。
        @retval     nn::ec::ResultInsufficientWorkMemory                    ConsumableServiceItemManager のワークメモリが不足しています。
                                                                            初期化時に nn::ec::RequiredWorkMemorySizeForConsumableServiceItem byte 以上のワークメモリを渡してください。
        @retval     nn::ec::ResultConsumableServiceItemInsufficientBuffer   アイテム ID を確保するための出力バッファが足りません。

        @pre
            - pOutItemIdCount != nullptr
            - outItemIdArray != nullptr
            - GetProvidableItemIdCount() > 0
            - itemIdArrayCount >= GetProvidableItemIdCount()

        @details
            この API で返されるアイテム ID は、取得した時期が古いのものから順に outItemIdArray の低いインデックスに格納されます。@n

            同一の消費型サービスアイテムを複数購入していた場合、本 API で、同一のアイテム ID が複数取得されます。@n

            例：
            @li アイテム ID = "ItemBox" の消費型サービスアイテムを、ユーザーが 5 個購入した場合@n
                "ItemBox" を示す ConsumableServiceItemId が 5 個 outItemIdArray に取得されます。pOutItemIdCount = 5 となります。
            @li アイテム ID = "BattlePack" の消費型サービスアイテムを 2 個、アイテム ID = "EnergyPack" の消費型サービスアイテムを 10 個、ユーザーが購入した場合@n
                "BattlePack" を示す ConsumableServiceItemId が 2 個、"EnergyPack" を示す ConsumableServiceItemId が 10 個 outItemIdArray に取得されます。@n
                pOutItemIdCount = 12 となります。
    */
    Result GetProvidableItemIds(int* pOutItemIdCount, ConsumableServiceItemId outItemIdArray[], int itemIdArrayCount) NN_NOEXCEPT;

    /**
        @brief 指定した Item Id を提供済みとして、このオブジェクトの保持対象から削除します

        @param[in]   itemId     提供する消費型サービスアイテムのアイテム ID

        @retval     nn::ResultSuccess                                           処理が成功しました。
        @retval     nn::ec::ResultConsumableServiceItemIdNotFound               指定したアイテム ID は見つかりませんでした。

        @pre
            - itemId.IsValid == true

        @details
            このオブジェクトが同一のアイテム ID を複数保持している場合、そのうちの 1 個を提供済みとして削除します。@n
            同一のアイテム ID のうち、ConsumableServiceItemManager に取得されたのが古いのものから順に提供済みとします。
    */
    Result MarkServiceProvided(const ConsumableServiceItemId& itemId) NN_NOEXCEPT;

    /**
        @brief 指定した Item Id を提供済みとして、このオブジェクトの保持対象から削除します

        @param[out]  pOutSeed   権利情報から一意に求められた Seed
        @param[in]   itemId     提供する消費型サービスアイテムのアイテム ID

        @retval     nn::ResultSuccess                                           処理が成功しました。
        @retval     nn::ec::ResultConsumableServiceItemIdNotFound               指定したアイテム ID は見つかりませんでした。

        @pre
            - pOutSeed != nullptr
            - itemId.IsValid == true

        @details
            このオブジェクトが同一のアイテム ID を複数保持している場合、そのうちの 1 個を提供済みとして削除します。@n
            同一のアイテム ID のうち、ConsumableServiceItemManager に取得されたのが古いのものから順に提供済みとします。@n

            ユーザーにとっての価値が提供のたびに変わるアイテムは、pOutSeed で返される乱数値を、価値を固定化するヒントとしてください。@n
            これにより「ユーザーが提供状態を繰り返し巻き戻すことで、より高い価値の提供内容を選ぶこと」を回避することができます。
    */
    Result MarkServiceProvided(uint64_t* pOutSeed, const ConsumableServiceItemId& itemId) NN_NOEXCEPT;

private:

    friend class AsyncGetConsumableRightDataRequest;
    friend class AsyncRecoverRightDataRequest;
    friend class AsyncConsumeRightDataRequest;

    Result ImportSerializedRightData(const void* buffer, const size_t bufferSize) NN_NOEXCEPT;
    int GetRightDataCount() NN_NOEXCEPT;
    const void* GetRightData() NN_NOEXCEPT;
    int CopyRightData(void* outRightDataArray, const int rightDataCount) NN_NOEXCEPT;
    int GetRightDataCountMax() NN_NOEXCEPT;
    nn::mem::StandardAllocator* GetAllocator() NN_NOEXCEPT;
    void SetRightData(const void* rightDataArray, const int rightDataCount) NN_NOEXCEPT;
    Result AddRightData(bool* isRightDataAddedNewly, const void* additionalRightDataArray, const int additionalRightDataCount) NN_NOEXCEPT;
    void SetRecoverRequired() NN_NOEXCEPT;
    void SetConsumeRequired() NN_NOEXCEPT;
    void SetRecoverDone() NN_NOEXCEPT;
    void SetConsumeDone() NN_NOEXCEPT;

    bool m_IsInitialized;
    void* m_pRightData;
    int m_RightDataCount;
    nn::mem::StandardAllocator m_Allocator;
    ShopServiceAccessor* m_pAccessor;
    nn::account::Uid m_User;
    ConsumableServiceItemManagerState m_State;
};

/**
    @brief  消費型サービスアイテムに関する非同期要求の基底クラス

    @details
        本クラスは、消費型サービスアイテムの非同期要求について、共通処理を定義した基底クラスです。@n
        要求内容ごとに変わる処理は、各派生クラスに定義されます。基底クラス単体ではインスタンスを生成できません。@n

        基底クラスでは、以下の機能を提供します。
        @li Cancel()          非同期要求のキャンセル
        @li Wait()            非同期要求の完了待ち
        @li TryWait()         非同期要求の完了待ち確認
        @li GetEvent()        非同期要求完了通知を受けるイベントの取得
        @li GetErrorCode()    非同期要求結果のエラーコード取得

        派生クラスでは、以下の機能を提供します。
        @li Begin()           非同期要求の開始
        @li End()             非同期要求の終了
*/
class ConsumableServiceItemAsyncRequestBase
{
    NN_DISALLOW_COPY(ConsumableServiceItemAsyncRequestBase);
    NN_DISALLOW_MOVE(ConsumableServiceItemAsyncRequestBase);

public:

    /**
        @brief 非同期要求をキャンセルします。
    */
    void Cancel() NN_NOEXCEPT;

    /**
        @brief 非同期要求の完了を待ちます。
    */
    void Wait() NN_NOEXCEPT;

    /**
        @brief 非同期要求の完了待ち確認を行います。

        @return 非同期要求の完了状態を返します。
    */
    bool TryWait() NN_NOEXCEPT;

    /**
        @brief 非同期要求完了通知を受けるイベントオブジェクトの参照を取得します。

        @return 非同期要求完了通知を受けるイベントオブジェクトの参照を返します。
    */
    nn::os::SystemEvent& GetEvent() NN_NOEXCEPT;

    /**
        @brief 非同期要求結果のエラーコードを取得します。

        @return 非同期要求結果のエラーコードを返します。
    */
    nn::err::ErrorCode GetErrorCode() const NN_NOEXCEPT;

protected:

    /**
        @brief 基底クラスのコンストラクタ

        @details インスタンス生成は禁止されています。
    */
    ConsumableServiceItemAsyncRequestBase() NN_NOEXCEPT
        : m_IsInitialized(false)
    {}

    bool IsRequestAvailable() const NN_NOEXCEPT;
    Result GetSize(size_t* pOutValue) NN_NOEXCEPT;
    Result Get(void* pOutReceivedBuffer, size_t bufferCapacity) NN_NOEXCEPT;
    ShopServiceAccessor* GetAccessor() NN_NOEXCEPT;
    ConsumableServiceItemManager* GetRightDataManager() NN_NOEXCEPT;
    nn::account::Uid GetUser() NN_NOEXCEPT;
    ShopServiceAccessor::AsyncResponse* GetAsyncResponse() NN_NOEXCEPT;
    bool IsInitialized() NN_NOEXCEPT;

private:

    friend ConsumableServiceItemManager;
    void Initialize(ShopServiceAccessor* pAccessor, ConsumableServiceItemManager* pManager, const nn::account::Uid& user) NN_NOEXCEPT;

    ShopServiceAccessor* m_pAccessor;
    ConsumableServiceItemManager* m_pRightDataManager;
    nn::account::Uid m_User;
    ShopServiceAccessor::AsyncResponse m_Response;
    bool m_IsInitialized;
};

/**
    @brief  サーバーに所有している未消費の権利情報を取得する非同期要求クラス

    @details
        このオブジェクトの初期化は ConsumableServiceItemManager の SetupAsyncRequest で行われます。@n
        非同期要求を実行する前に、適切に初期化してください。
*/
class AsyncGetConsumableRightDataRequest : public ConsumableServiceItemAsyncRequestBase
{
    NN_DISALLOW_COPY(AsyncGetConsumableRightDataRequest);
    NN_DISALLOW_MOVE(AsyncGetConsumableRightDataRequest);

public:

    /**
        @brief コンストラクタ
    */
    AsyncGetConsumableRightDataRequest() NN_NOEXCEPT
        : m_IsRequestBegin(false)
    {}

    /**
        @brief 未消費の権利情報の取得を開始します。

        @retval     nn::ResultSuccess                                           処理が成功しました。
        @retval     nn::ec::ResultShopServiceAccessOverRequest                  非同期要求に必要なサービスリソースを確保できませんでした。
                                                                                未破棄の非同期要求があれば破棄した後に再度要求してください。
                                                                                もしくは、ShopServiceAccessor の同時利用数を減らしてください。
        @retval     nn::ec::ResultShopServiceAccessInsufficientWorkMemory       ShopServiceAccessor のワークメモリが不足しています。
        @retval     nn::ec::ResultInsufficientWorkMemory                        ConsumableServiceItemManager のワークメモリが不足しています。
                                                                                初期化時に nn::ec::RequiredWorkMemorySizeForConsumableServiceItem byte 以上のワークメモリを渡してください。
        @retval     nn::ec::ResultInternetRequestNotAccepted                    インターネット接続要求が提出、及び受理されていません。

        @pre
            - Initialize() が実行済み (ConsumableServiceItemManager の SetupAsyncRequest によって実行されます)

        @details
            ユーザーがサーバーに所有している権利情報を取得します。@n

            本 API では、nn::ec::ShopServiceAccessor::AsyncResponse のエラーが返ることがあります。@n
            そのエラーハンドリングについては、ShopServiceAccessor::AsyncResponse のリファレンスを参照してください。@n

            本 API では、nn::nifm::IsNetworkAvailable() == true が満たされない場合、エラーが返ります。@n

            本 API では、サーバーへの要求を行います。サーバーへの要求を伴う関数の呼び出しには、頻度制限があります。@n
            制限を超えた場合でも本関数はすぐに返りますが、要求処理の開始は遅延されます。
    */
    Result Begin() NN_NOEXCEPT;

    /**
        @brief 未消費な権利情報の取得完了を待ちます。

        @retval     nn::ResultSuccess                                           処理が成功しました。
        @retval     nn::ec::ResultShopServiceAccessCanceled                     呼び出し元からの要求に従い処理がキャンセルされました。
        @retval     nn::ec::ResultShopServiceAccessInsufficientWorkMemory       ShopServiceAccessor のワークメモリが不足しています。
        @retval     nn::ec::ResultInsufficientWorkMemory                        ConsumableServiceItemManager のワークメモリが不足しています。
                                                                                初期化時に nn::ec::RequiredWorkMemorySizeForConsumableServiceItem byte 以上のワークメモリを渡してください。
        @retval     nn::ec::ResultShopServiceAccessRequestTimeout               サーバーとの通信要求がタイムアウトしました。
        @retval     nn::account::ResultNetworkServiceAccountUnavailable         指定されたユーザアカウントのネットワークサービスアカウントが利用できません。
                                                                                ACCOUNT ライブラリのエラーハンドリングリファレンスを参考にネットワークサービスアカウントを利用可能状態にしてください。
        @retval     nn::ec::ResultShowErrorCodeRequired                         接続先ショップサービスサーバーからエラービューア表示が必要なレスポンスエラーコードを受信しました。
        @retval     nn::ec::ResultConsumableServiceItemInvalidServerRightStatus サーバー上で不正な状態の権利情報を検出しました。

        @pre
            - Begin() が実行済み

        @details
            非同期処理が完了していなければ、API 内部で完了を待機します。@n

            本 API では、nn::ec::ShopServiceAccessor::AsyncResponse のエラーが返ることがあります。@n
            そのエラーハンドリングについては、ShopServiceAccessor::AsyncResponse のリファレンスを参照してください。@n

            本 API を実行すると、サーバーから権利情報を取得します。@n
            ただし最大でも、マネージャが保持できる権利情報の空きの数だけしか取得できません。@n

            保持している権利情報が最大数に達している場合、サーバーから新たに権利情報を取得することはできません。@n
            この場合、ユーザーが新規に購入した消費型サービスアイテムを、サーバーからローカルに取得できなくなります。@n
            これを回避するために、マネージャが保持する権利情報は提供可能になり次第、できるかぎり即座にサービスを提供してください。@n

            ConsumableServiceItemManager の推奨される利用方法について、詳しくは nn::ec::ConsumableServiceItemManager を参照してください。
    */
    Result End() NN_NOEXCEPT;

private:

    Result BeginImpl(int page, int perPage) NN_NOEXCEPT;
    Result EndImpl(int* pOutRightDataCount, ConsumableServiceItemRightData outRightDataArray[], int maxRightDataCount) NN_NOEXCEPT;

    int m_Page;
    int m_PerPage;
    bool m_IsRequestBegin;
};

/**
    @brief  サーバーに所有している権利情報を消費する非同期要求クラス

    @details
        このオブジェクトの初期化は ConsumableServiceItemManager の SetupAsyncRequest で行われます。@n
        非同期要求を実行する前に、適切に初期化してください。
*/
class AsyncConsumeRightDataRequest : public ConsumableServiceItemAsyncRequestBase
{
    NN_DISALLOW_COPY(AsyncConsumeRightDataRequest);
    NN_DISALLOW_MOVE(AsyncConsumeRightDataRequest);

public:

    /**
        @brief コンストラクタ
    */
    AsyncConsumeRightDataRequest() NN_NOEXCEPT
        : m_IsRequestBegin(false)
    {}

    /**
        @brief 未消費な権利情報の消費を開始します。

        @retval     nn::ResultSuccess                                           処理が成功しました。
        @retval     nn::ec::ResultShopServiceAccessOverRequest                  非同期要求に必要なサービスリソースを確保できませんでした。
                                                                                未破棄の非同期要求があれば破棄した後に再度要求してください。
                                                                                もしくは、ShopServiceAccessor の同時利用数を減らしてください。
        @retval     nn::ec::ResultShopServiceAccessInsufficientWorkMemory       ShopServiceAccessor のワークメモリが不足しています。
        @retval     nn::ec::ResultInsufficientWorkMemory                        ConsumableServiceItemManager のワークメモリが不足しています。
                                                                                初期化時に nn::ec::RequiredWorkMemorySizeForConsumableServiceItem byte 以上のワークメモリを渡してください。
        @retval     nn::ec::ResultInternetRequestNotAccepted                    インターネット接続要求が提出、及び受理されていません。

        @pre
            - Initialize() が実行済み (ConsumableServiceItemManager の SetupAsyncRequest によって実行されます)

        @details
            マネージャが保持している権利情報をサーバーから消費します。@n
            消費した権利情報は以後、AsyncGetConsumableRightDataRequest では取得できなくなります。@n

            本 API では、nn::ec::ShopServiceAccessor::AsyncResponse のエラーが返ることがあります。@n
            そのエラーハンドリングについては、ShopServiceAccessor::AsyncResponse のリファレンスを参照してください。@n

            本 API では、nn::nifm::IsNetworkAvailable() == true が満たされない場合、エラーが返ります。@n

            本 API では、サーバーへの要求を行います。サーバーへの要求を伴う関数の呼び出しには、頻度制限があります。@n
            制限を超えた場合でも本関数はすぐに返りますが、要求処理の開始は遅延されます。
    */
    Result Begin() NN_NOEXCEPT;

    /**
        @brief 権利情報の消費完了を待ちます

        @retval     nn::ResultSuccess                                           処理が成功しました。
        @retval     nn::ec::ResultShopServiceAccessCanceled                     呼び出し元からの要求に従い処理がキャンセルされました。
        @retval     nn::ec::ResultShopServiceAccessInsufficientWorkMemory       ShopServiceAccessor のワークメモリが不足しています。
        @retval     nn::ec::ResultInsufficientWorkMemory                        ConsumableServiceItemManager のワークメモリが不足しています。
                                                                                初期化時に nn::ec::RequiredWorkMemorySizeForConsumableServiceItem byte 以上のワークメモリを渡してください。
        @retval     nn::ec::ResultShopServiceAccessRequestTimeout               サーバーとの通信要求がタイムアウトしました。
        @retval     nn::account::ResultNetworkServiceAccountUnavailable         指定されたユーザアカウントのネットワークサービスアカウントが利用できません。
                                                                                ACCOUNT ライブラリのエラーハンドリングリファレンスを参考にネットワークサービスアカウントを利用可能状態にしてください。
        @retval     nn::ec::ResultShowErrorCodeRequired                         接続先ショップサービスサーバーからエラービューア表示が必要なレスポンスエラーコードを受信しました。

        @pre
            - Begin() が実行済み

        @details
            非同期処理が完了していなければ、API 内部で完了を待機します。@n

            本 API では、nn::ec::ShopServiceAccessor::AsyncResponse のエラーが返ることがあります。@n
            そのエラーハンドリングについては、ShopServiceAccessor::AsyncResponse のリファレンスを参照してください。@n

            本 API が成功した場合、新規に権利消費が完了したアイテム ID が GetProvidableItemIds() で取得可能になります。@n
            本 API が失敗した場合、マネージャは権利情報の提供可否判定を必要とする状態（IsRecoveryRequired() == true）になります。@n
            AsyncRecoverRightDataRequest を実行して、提供可能な権利情報を復旧してください。
    */
    Result End() NN_NOEXCEPT;

private:

    Result BeginImpl(const ConsumableServiceItemRightData rightDataArray[], int rightDataCount) NN_NOEXCEPT;
    Result EndImpl(int* pOutRightDataCount, ConsumableServiceItemRightData outRightDataArray[], int maxRightDataCount) NN_NOEXCEPT;

    const ConsumableServiceItemRightData* m_pRightData;
    int m_RightDataCount;
    bool m_IsRequestBegin;
};

/**
    @brief  権利情報の提供可能状態を復旧する非同期要求クラス

    @details
        このオブジェクトの初期化は ConsumableServiceItemManager の SetupAsyncRequest で行われます。@n
        非同期要求を実行する前に、適切に初期化してください。
*/
class AsyncRecoverRightDataRequest : public ConsumableServiceItemAsyncRequestBase
{
    NN_DISALLOW_COPY(AsyncRecoverRightDataRequest);
    NN_DISALLOW_MOVE(AsyncRecoverRightDataRequest);

public:

    /**
        @brief コンストラクタ
    */
    AsyncRecoverRightDataRequest() NN_NOEXCEPT
        : m_IsRequestBegin(false)
    {}

    /**
        @brief 権利情報の提供可能状態の復旧を開始します。

        @retval     nn::ResultSuccess                                           処理が成功しました。
        @retval     nn::ec::ResultShopServiceAccessOverRequest                  非同期要求に必要なサービスリソースを確保できませんでした。
                                                                                未破棄の非同期要求があれば破棄した後に再度要求してください。
                                                                                もしくは、ShopServiceAccessor の同時利用数を減らしてください。
        @retval     nn::ec::ResultShopServiceAccessInsufficientWorkMemory       ShopServiceAccessor のワークメモリが不足しています。
        @retval     nn::ec::ResultInsufficientWorkMemory                        ConsumableServiceItemManager のワークメモリが不足しています。
                                                                                初期化時に nn::ec::RequiredWorkMemorySizeForConsumableServiceItem byte 以上のワークメモリを渡してください。
        @retval     nn::ec::ResultInternetRequestNotAccepted                    インターネット接続要求が提出、及び受理されていません。

        @pre
            - Initialize() が実行済み (ConsumableServiceItemManager の SetupAsyncRequest によって実行されます)

        @details
            本 API では、nn::ec::ShopServiceAccessor::AsyncResponse のエラーが返ることがあります。@n
            そのエラーハンドリングについては、ShopServiceAccessor::AsyncResponse のリファレンスを参照してください。@n

            本 API では、nn::nifm::IsNetworkAvailable() == true が満たされない場合、エラーが返ります。@n

            本 API では、サーバーへの要求を行います。サーバーへの要求を伴う関数の呼び出しには、頻度制限があります。@n
            制限を超えた場合でも本関数はすぐに返りますが、要求処理の開始は遅延されます。
    */
    Result Begin() NN_NOEXCEPT;

    /**
        @brief 権利情報の提供可能状態の復旧完了を待ちます

        @retval     nn::ResultSuccess                                           処理が成功しました。
        @retval     nn::ec::ResultShopServiceAccessCanceled                     呼び出し元からの要求に従い処理がキャンセルされました。
        @retval     nn::ec::ResultShopServiceAccessInsufficientWorkMemory       ShopServiceAccessor のワークメモリが不足しています。
        @retval     nn::ec::ResultInsufficientWorkMemory                        ConsumableServiceItemManager のワークメモリが不足しています。
                                                                                初期化時に nn::ec::RequiredWorkMemorySizeForConsumableServiceItem byte 以上のワークメモリを渡してください。
        @retval     nn::ec::ResultShopServiceAccessRequestTimeout               サーバーとの通信要求がタイムアウトしました。
        @retval     nn::account::ResultNetworkServiceAccountUnavailable         指定されたユーザアカウントのネットワークサービスアカウントが利用できません。
                                                                                ACCOUNT ライブラリのエラーハンドリングリファレンスを参考にネットワークサービスアカウントを利用可能状態にしてください。
        @retval     nn::ec::ResultShowErrorCodeRequired                         接続先ショップサービスサーバーからエラービューア表示が必要なレスポンスエラーコードを受信しました。
        @retval     nn::ec::ResultConsumableServiceItemInvalidServerRightStatus サーバー上で不正な状態の権利情報を検出しました。

        @pre
            - Begin() が実行済み

        @details
            非同期処理が完了していなければ、API 内部で完了を待機します。@n

            本 API では、nn::ec::ShopServiceAccessor::AsyncResponse のエラーが返ることがあります。@n
            そのエラーハンドリングについては、ShopServiceAccessor::AsyncResponse のリファレンスを参照してください。@n

            本 API が成功した場合、新規に権利復旧が完了したアイテム ID が GetProvidableItemIds() で取得可能になります。
    */
    Result End() NN_NOEXCEPT;

private:

    Result BeginImpl(const ConsumableServiceItemRightData rightDataArray[], int rightDataCount) NN_NOEXCEPT;
    Result EndImpl(int* pOutRightDataCount, ConsumableServiceItemRightData outRightDataArray[], int maxRightDataCount) NN_NOEXCEPT;

    const ConsumableServiceItemRightData* m_pRightData;
    int m_RightDataCount;
    bool m_IsRequestBegin;
};

}}
