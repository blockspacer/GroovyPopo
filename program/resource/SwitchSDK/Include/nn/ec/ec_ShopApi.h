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
 * @brief ショップの機能を提供するAPIの定義
 */

#pragma once

#include <nn/ec/ec_ShopTypes.h>

namespace nn { namespace ec {

/**
 * @name ニンテンドーeショップの機能を利用するための関数
 * @{
 */

 /**
 * @brief アプリケーションの詳細情報と購入画面を表示します。
 * @return なし
 *
 * @details
 * - 自身の詳細情報と購入画面を表示します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合は、ApplicationId を指定する ShowShopApplicationInformation() を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopApplicationInformation() NN_NOEXCEPT;

/**
 * @brief 指定したユーザーでアプリケーションの詳細情報と購入画面を表示します。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - アカウントライブラリが初期化されている
 *
 * @details
 * - 自身の詳細情報と購入画面を表示します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合は、ApplicationId を指定する ShowShopApplicationInformation() を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopApplicationInformation(const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @brief アプリケーションの詳細情報と購入画面を表示します。
 * @param[in] applicationId 情報を表示したいアプリケーションの ApplicationId。
 * @return なし
 * @pre
 *  - applicationIdParent != nn::ApplicationId::GetInvalidId()
 *
 * @details
 * - 自身の商品情報を表示する場合は、ApplicationId を指定しない ShowShopApplicationInformation() を推奨します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合に限り、本関数を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopApplicationInformation(nn::ApplicationId applicationId) NN_NOEXCEPT;

/**
 * @brief 指定したユーザーでアプリケーションの詳細情報と購入画面を表示します。
 * @param[in] applicationId 情報を表示したいアプリケーションの ApplicationId。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - applicationIdParent != nn::ApplicationId::GetInvalidId()
 *  - アカウントライブラリが初期化されている
 *
 * @details
 * - 自身の商品情報を表示する場合は、ApplicationId を指定しない ShowShopApplicationInformation() を推奨します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合に限り、本関数を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopApplicationInformation(nn::ApplicationId applicationId,
    const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @brief 追加コンテンツの一覧と購入画面を表示します。
 * @return なし
 *
 * @details
 * - 自身の追加コンテンツの一覧と購入画面を表示します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合は、ApplicationId を指定する ShowShopAddOnContentList() を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopAddOnContentList() NN_NOEXCEPT;

/**
 * @brief 指定したユーザーで追加コンテンツの一覧と購入画面を表示します。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - アカウントライブラリが初期化されている
 *
 * @details
 * - 自身の追加コンテンツの一覧と購入画面を表示します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合は、ApplicationId を指定する ShowShopAddOnContentList() を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopAddOnContentList(const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @brief 追加コンテンツの一覧と購入画面を表示します。
 * @param[in] applicationIdParent 一覧として表示する追加コンテンツに設定されたアプリケーションID。
 * @return なし
 * @pre
 *  - applicationIdParent != nn::ApplicationId::GetInvalidId()
 *
 * @details
 * - 自身の商品情報を表示する場合は、ApplicationId を指定しない ShowShopAddOnContentList() を推奨します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合に限り、本関数を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopAddOnContentList(nn::ApplicationId applicationIdParent) NN_NOEXCEPT;

/**
 * @brief 指定したユーザーで追加コンテンツの一覧と購入画面を表示します。
 * @param[in] applicationIdParent 一覧として表示する追加コンテンツに設定されたアプリケーションID。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - applicationIdParent != nn::ApplicationId::GetInvalidId()
 *  - アカウントライブラリが初期化されている
 *
 * @details
 * - 自身の商品情報を表示する場合は、ApplicationId を指定しない ShowShopAddOnContentList() を推奨します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合に限り、本関数を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopAddOnContentList(nn::ApplicationId applicationIdParent, const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @brief 期間型サービスアイテム(期間券)の一覧と購入画面を表示します。
 * @return なし
 *
 * @details
 * - 自身の期間型サービスアイテム(期間券)の一覧と購入画面を表示します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合は、ApplicationId を指定する ShowShopSubscriptionList() を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopSubscriptionList() NN_NOEXCEPT;

/**
 * @brief 指定したユーザーで期間型サービスアイテム(期間券)の一覧と購入画面を表示します。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - アカウントライブラリが初期化されている
 *
 * @details
 * - 自身の期間型サービスアイテム(期間券)の一覧と購入画面を表示します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合は、ApplicationId を指定する ShowShopSubscriptionList() を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopSubscriptionList(const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @brief 期間型サービスアイテム(期間券)の一覧と購入画面を表示します。
 * @param[in] applicationIdParent 一覧として表示する期間券に設定されたアプリケーションID。
 * @return なし
 * @pre
 *  - applicationIdParent != nn::ApplicationId::GetInvalidId()
 *
 * @details
 * - 自身の商品情報を表示する場合は、ApplicationId を指定しない ShowShopSubscriptionList() を推奨します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合に限り、本関数を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopSubscriptionList(nn::ApplicationId applicationIdParent) NN_NOEXCEPT;

/**
 * @brief 指定したユーザーで期間型サービスアイテム(期間券)の一覧と購入画面を表示します。
 * @param[in] applicationIdParent 一覧として表示する期間券に設定されたアプリケーションID。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - applicationIdParent != nn::ApplicationId::GetInvalidId()
 *  - アカウントライブラリが初期化されている
 *
 * @details
 * - 自身の商品情報を表示する場合は、ApplicationId を指定しない ShowShopSubscriptionList を推奨します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合に限り、本関数を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopSubscriptionList(nn::ApplicationId applicationIdParent, const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @brief 期間型サービスアイテム(期間券)の詳細と購入画面を表示します。
 * @param[in] courseId 詳細を表示する期間型サービスのコースを識別するID。
 *   事前に発行されたコースIDを指定します。
 * @return なし
 * @pre
 *  - courseId の文字列がNULL終端されている
 *
 * @details
 * - 自身の期間型サービスアイテム(期間券)の詳細と購入画面を表示します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合は、ApplicationId を指定する ShowShopSubscriptionList() を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopSubscriptionList(const CourseId& courseId) NN_NOEXCEPT;

/**
 * @brief 指定したユーザーで期間型サービスアイテム(期間券)の詳細と購入画面を表示します。
 * @param[in] courseId 詳細を表示する期間型サービスのコースを識別するID。
 *   事前に発行されたコースIDを指定します。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - courseId の文字列がNULL終端されている
 *
 * @details
 * - 自身の期間型サービスアイテム(期間券)の詳細と購入画面を表示します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合は、ApplicationId を指定する ShowShopSubscriptionList() を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopSubscriptionList(const CourseId& courseId, const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @brief 期間型サービスアイテム(期間券)の詳細と購入画面を表示します。
 * @param[in] applicationIdParent 一覧として表示する期間券に設定されたアプリケーションID。
 * @param[in] courseId 詳細を表示する期間型サービスのコースを識別するID。
 *   事前に発行されたコースIDを指定します。
 * @return なし
 * @pre
 *  - applicationIdParent != nn::ApplicationId::GetInvalidId()
 *  - courseId の文字列がNULL終端されている
 *
 * @details
 * - 自身の商品情報を表示する場合は、ApplicationId を指定しない ShowShopSubscriptionList() を推奨します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合に限り、本関数を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopSubscriptionList(nn::ApplicationId applicationIdParent, const CourseId& courseId) NN_NOEXCEPT;

/**
 * @brief 指定したユーザーで期間型サービスアイテム(期間券)の詳細と購入画面を表示します。
 * @param[in] applicationIdParent 一覧として表示する期間券に設定されたアプリケーションID。
 * @param[in] courseId 詳細を表示する期間型サービスのコースを識別するID。
 *   事前に発行されたコースIDを指定します。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - applicationIdParent != nn::ApplicationId::GetInvalidId()
 *  - courseId の文字列がNULL終端されている
 *  - アカウントライブラリが初期化されている
 *
 * @details
 * - 自身の商品情報を表示する場合は、ApplicationId を指定しない ShowShopSubscriptionList() を推奨します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合に限り、本関数を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopSubscriptionList(nn::ApplicationId applicationIdParent, const CourseId& courseId, const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @brief 消費型サービスアイテムの一覧と購入画面を表示します。
 * @return なし
 *
 * @details
 * - 自身の消費型サービスアイテムの一覧と購入画面を表示します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合は、ApplicationId を指定する ShopConsumableItemList() を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopConsumableItemList() NN_NOEXCEPT;

/**
 * @brief 指定したユーザーで消費型サービスアイテムの一覧と購入画面を表示します。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - アカウントライブラリが初期化されている
 *
 * @details
 * - 自身の消費型サービスアイテムの一覧と購入画面を表示します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合は、ApplicationId を指定する ShopConsumableItemList() を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopConsumableItemList(const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @brief 消費型サービスアイテムの一覧と購入画面を表示します。
 * @param[in] applicationIdParent 一覧として表示する消費型サービスアイテムに設定されたアプリケーションID。
 * @return なし
 * @pre
 *  - applicationIdParent != nn::ApplicationId::GetInvalidId()
 *
 * @details
 * - 自身の商品情報を表示する場合は、ApplicationId を指定しない ShowShopConsumableItemList() を推奨します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合に限り、本関数を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopConsumableItemList(nn::ApplicationId applicationIdParent) NN_NOEXCEPT;

/**
 * @brief 指定したユーザーで消費型サービスアイテムの一覧と購入画面を表示します。
 * @param[in] applicationIdParent 一覧として表示する消費型サービスアイテムに設定されたアプリケーションID。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - applicationIdParent != nn::ApplicationId::GetInvalidId()
 *  - アカウントライブラリが初期化されている
 *
 * @details
 * - 自身の商品情報を表示する場合は、ApplicationId を指定しない ShowShopConsumableItemList() を推奨します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合に限り、本関数を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopConsumableItemList(nn::ApplicationId applicationIdParent, const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @brief 消費型サービスアイテムの詳細と購入画面を表示します。
 * @param[in] consumableId 詳細を表示する消費型サービスアイテムカテゴリを識別するID。
 * @param[in] nsuid 詳細を表示する消費型サービスアイテムを識別するNsUid。
 * @return なし
 * @pre
 *  - consumableId の文字列がNULL終端されている
 *  - nsuid != nn::ec::NsUid::GetInvalidId()
 *
 * @details
 * - 自身の消費型サービスアイテムの詳細と購入画面を表示します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合は、ApplicationId を指定する ShowShopConsumableItemDetail() を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopConsumableItemDetail(const ConsumableId& consumableId, NsUid nsuid) NN_NOEXCEPT;

/**
 * @brief 指定したユーザーで消費型サービスアイテムの詳細と購入画面を表示します。
 * @param[in] consumableId 詳細を表示する消費型サービスアイテムカテゴリを識別するID。
 * @param[in] nsuid 詳細を表示する消費型サービスアイテムを識別するNsUid。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - consumableId の文字列がNULL終端されている
 *  - nsuid != nn::ec::NsUid::GetInvalidId()
 *  - アカウントライブラリが初期化されている
 *
 * @details
 * - 自身の消費型サービスアイテムの詳細と購入画面を表示します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合は、ApplicationId を指定する ShowShopConsumableItemDetail() を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopConsumableItemDetail(const ConsumableId& consumableId, NsUid nsuid, const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @brief 消費型サービスアイテムの詳細と購入画面を表示します。
 * @param[in] applicationIdParent 一覧として表示する消費型サービスアイテムに設定されたアプリケーションID。
 * @param[in] consumableId 詳細を表示する消費型サービスアイテムカテゴリを識別するID。
 * @param[in] nsuid 詳細を表示する消費型サービスアイテムを識別するNsUid。
 * @return なし
 * @pre
 *  - applicationIdParent != nn::ApplicationId::GetInvalidId()
 *  - consumableId の文字列がNULL終端されている
 *  - nsuid != nn::ec::NsUid::GetInvalidId()
 *
 * @details
 * - 自身の商品情報を表示する場合は、ApplicationId を指定しない ShowShopConsumableItemDetail() を推奨します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合に限り、本関数を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopConsumableItemDetail(nn::ApplicationId applicationIdParent, const ConsumableId& consumableId, NsUid nsuid) NN_NOEXCEPT;

/**
 * @brief 指定したユーザーで消費型サービスアイテムの詳細と購入画面を表示します。
 * @param[in] applicationIdParent 一覧として表示する消費型サービスアイテムに設定されたアプリケーションID。
 * @param[in] consumableId 詳細を表示する消費型サービスアイテムカテゴリを識別するID。
 * @param[in] nsuid 詳細を表示する消費型サービスアイテムを識別するNsUid。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - applicationIdParent != nn::ApplicationId::GetInvalidId()
 *  - consumableId の文字列がNULL終端されている
 *  - nsuid != nn::ec::NsUid::GetInvalidId()
 *  - アカウントライブラリが初期化されている
 *
 * @details
 * - 自身の商品情報を表示する場合は、ApplicationId を指定しない ShowShopConsumableItemDetail() を推奨します。
 *   他のアプリケーションや体験版から製品版の商品情報を表示する場合に限り、本関数を利用して下さい。
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopConsumableItemDetail(nn::ApplicationId applicationIdParent, const ConsumableId& consumableId, NsUid nsuid, const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @brief ショップ上の商品の詳細情報と購入画面を表示します。
 *        追加コンテンツの商品の表示に対応します。
 * @param[in] nsuid 詳細を表示する商品を識別するNsUid。
 * @return なし
 * @pre
 *  - nsuid != nn::ec::NsUid::GetInvalidId()
 *
 * @details
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopProductDetails(NsUid nsuid) NN_NOEXCEPT;

/**
 * @brief ショップ上の商品の詳細情報と購入画面を表示します。
 *        追加コンテンツの商品の表示に対応します。
 * @param[in] nsuid 詳細を表示する商品を識別するNsUid。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - nsuid != nn::ec::NsUid::GetInvalidId()
 *  - アカウントライブラリが初期化されている
 *
 * @details
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopProductDetails(NsUid nsuid,
    const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @brief ショップ上の商品の一覧と購入画面を表示します。
 *        追加コンテンツの商品の表示に対応します。
 * @param[in] nsuidList 一覧を表示する商品を識別するNsUidの配列。
 * @param[in] nsuidCount 一覧を表示する商品を識別するNsUidの配列の要素数。
 * @param[in] listName 一覧表示するリストに付ける名前の文字列(UTF-8)。名前を付けない場合には nullptr を指定してください。
 * @return なし
 * @pre
 *  - nsuidCount <= 30
 *  - nsuidList の各 nsuid != nn::ec::NsUid::GetInvalidId()
 *  - listName の文字数が 140 字以下 または listName == nullptr
 *
 * @details
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 * - 本関数はスタック消費量が大きいため、8 KB 以上のスタックの余裕がある状態で呼び出してください。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopProductList(const NsUid nsuidList[], int nsuidCount, const char* listName) NN_NOEXCEPT;

/**
 * @brief 指定したユーザーでショップ上の商品の一覧と購入画面を表示します。
 *        追加コンテンツの商品の表示に対応します。
 * @param[in] nsuidList 一覧を表示する商品を識別するNsUidの配列。
 * @param[in] nsuidCount 一覧を表示する商品を識別するNsUidの配列の要素数。
 * @param[in] listName 一覧表示するリストに付ける名前の文字列(UTF-8)。名前を付けない場合には nullptr を指定してください。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - nsuidCount <= 30
 *  - nsuidList の各 nsuid != nn::ec::NsUid::GetInvalidId()
 *  - listName の文字数が 140 字以下 または listName == nullptr
 *
 * @details
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 * - 本関数はスタック消費量が大きいため、8 KB 以上のスタックの余裕がある状態で呼び出してください。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopProductList(const NsUid nsuidList[], int nsuidCount, const char* listName, const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @brief 引換えコードの入力画面を表示します。
 * @return なし
 *
 * @details
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - 本関数は内部で必要に応じてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopEnterCodeScene() NN_NOEXCEPT;

/**
 * @brief 指定したユーザーで引換えコードの入力画面を表示します。
 * @param[in] selectedUser 呼び出し元で選択済みのユーザーを表すハンドル。
 *   事前にユーザー選択を行っている場合など、購入などのアクションを
 *   特定のユーザーで行いたい場合に指定します。
 * @return なし
 * @pre
 *  - アカウントライブラリが初期化されている
 *
 * @details
 * - 本関数は内部でUI表示を行うため、呼び出し元スレッドをブロックします。
 * - selectedUser が利用できないユーザーである場合、
 *   本関数は内部で改めてユーザー選択UIを表示してユーザーの選択を行います。
 *
 * @platformbegin{NX}
 * 本関数は「EC アプレット」を利用してニンテンドーeショップの表示を行います。
 * EC アプレットの詳細については
 * 「本体機能ガイド」の「EC アプレット」を参照してください。
 * @platformend
 */
void ShowShopEnterCodeScene(const nn::account::UserHandle& selectedUser) NN_NOEXCEPT;

/**
 * @}
 */

}}
