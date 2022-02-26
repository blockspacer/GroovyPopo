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

#include <cstddef>
#include <nn/util/util_IntrusiveList.h>
#include <nn/ui2d/ui2d_Pane.h>
#include <nn/ui2d/ui2d_Types.h>

namespace nn
{
namespace ui2d
{

struct ResGroup;

//---------------------------------------------------------------------------
//! @brief グループに属するペインへのリンクです。
//!
//---------------------------------------------------------------------------
struct PaneLink
{
    // リンク情報です。

    //! @brief 内部用機能のため使用禁止です。
    nn::util::IntrusiveListNode link;

    //! グループに属するペインへのポインタです。
    Pane* pTarget;
};

//! @brief ペインへのリンクの一覧を保持します。
//!
typedef nn::util::IntrusiveList<PaneLink, nn::util::IntrusiveListMemberNodeTraits<PaneLink, &PaneLink::link> > PaneLinkList;

//---------------------------------------------------------------------------
//! @brief ペインをグループで管理するためのクラスです。
//!
//---------------------------------------------------------------------------
class Group
{
public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! ペインのない状態で初期化します。
    //!
    Group();

    //! @brief コンストラクタです。
    //!
    //! @details
    //! ペインのない状態で、グループの名前が指定された状態で初期化します。
    //!
    //! 名前のための文字列バッファを持たず、
    //! 引数で与えた文字列のポインタを保持しますので、スタックなどで
    //! 一時的に確保した文字列のポインタポインタを与えないようにしてください。
    //!
    //! 文字列の長さはnn::ui2d::GroupNameStrMax以下としてください。
    //!
    //! @param[in] pName グループの名前です。NULLではない文字列を指定してください。
    //!
    explicit Group(const char* pName);

    //! @brief コンストラクタです。
    //!
    //! @details
    //! グループリソースをもとにペインを追加した状態で初期化します。
    //!
    //! @param[in] pResGroup    グループリソースへのポインタです。
    //! @param[in] pRootPane    ルートペインへのポインタです。
    //!
    Group(const ResGroup* pResGroup, Pane* pRootPane);

    //! @brief コピーコンストラクタです。
    //!
    //! @details
    //! 引数pRootPaneにnullptrを与えると、コピー元と同じペインを登録します。
    //! 引数pRootPaneにペインのポインタを与えると、そのペイン以下からコピー元と
    //! 同じ名前のペインを再検索します。これは、レイアウトをコピーする際に、
    //! コピー先の新しくコピーされたペインを参照するようにするために使用する機能です。
    //!
    //! @param[in] group        グループリソースへの参照です。
    //! @param[in] pRootPane    ルートペインへのポインタです。
    //!
    explicit Group(const Group& group, Pane* pRootPane);

    //! @brief デストラクタです。
    //!
    virtual ~Group();

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief グループの名前を取得します。
    //!
    //! @return グループの名前を返します。
    //!
    const char* GetName() const
    {
        return m_pName;
    }

    //! @brief グループに所属するペインの一覧を取得します。
    //!
    //! @return ペインの一覧を返します。
    //!
    PaneLinkList& GetPaneList()
    {
        return m_PaneLinkList;
    }

    //@}

    //----------------------------------------
    //! @name 追加
    //@{

    //! @brief ペインを追加します。
    //!
    //! @param[in] pPane    ペインへのポインタです。
    //!
    void AppendPane(Pane* pPane);

    //@}

    //----------------------------------------
    //! @name その他
    //@{

    //! @brief ユーザーがグループを構築したかどうかを判定します。
    //!
    //! @return グループをユーザーが構築した場合は true を返します。
    //!
    bool IsUserAllocated() const
    {
        return m_UserAllocated != 0;
    }

    //! @brief ユーザーが構築したグループであることを宣言します。
    //!
    //! @details
    //! ユーザが構築したと宣言されたグループはライブラリによって解放されません。
    //!
    void SetUserAllocated()
    {
        m_UserAllocated = 1;
    }

    //! @brief コピーコンストラクタでコピーされたインスタンスが正しくコピーされているか確認します。
    //!
    //! @details
    //! このメソッドは ui2d ランタイムのテストのために作成されたメソッドです。
    //! アプリケーションコードでの使用は非推奨となります。
    //!
    //! @param[in]  target  比較対象のペインです。
    //!
    //! @return 同等とみなせる場合は true を返します。
    //!
    bool CompareCopiedInstanceTest(const Group& target) const;

    //@}

    // リンク情報です。

    //! @brief 内部用機能のため使用禁止です。
    nn::util::IntrusiveListNode m_Link;

private:
    PaneLinkList m_PaneLinkList;
    const char* m_pName;
    uint8_t  m_UserAllocated; // ユーザにより構築された場合は真
    uint8_t  m_Padding[3];

private:
    // コピー演算子を禁止します。
    const Group& operator=( const Group& );
};

//! @brief グループの一覧を保持します。
//!
typedef nn::util::IntrusiveList<Group, nn::util::IntrusiveListMemberNodeTraits<Group, &Group::m_Link> > GroupList;

//---------------------------------------------------------------------------
//! @brief グループを管理するためのクラスです。
//!
//! @details
//! Layout::GetGroupContainer() で取得します。
//!
//---------------------------------------------------------------------------
class GroupContainer
{
    NN_DISALLOW_COPY(GroupContainer);

public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    GroupContainer()
    {}

    //! @brief デストラクタです。
    //!
    ~GroupContainer();

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief グループを追加します。
    //!
    //! @param[in] pGroup   グループへのポインタです。
    //!
    void AppendGroup(Group* pGroup);

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief グループの一覧を取得します。
    //!
    //! @return
    //! グループの一覧を返します。
    //!
    GroupList& GetGroupList()
    {
        return m_GroupList;
    }

    //@}

    //----------------------------------------
    //! @name 検索
    //@{

    //! @brief 名前でグループを検索します。
    //!
    //! @param[in] pFindName 検索するグループ名です。
    //!
    //! @return 見つかった場合はグループへのポインタを、
    //! 見つからなかった場合は NULL を返します。
    //!
    Group* FindGroupByName(const char* pFindName);
    //! @brief 名前でグループを検索します。
    //!
    //! @param[in] pFindName 検索するグループ名です。
    //!
    //! @return 見つかった場合はグループへのポインタを、
    //! 見つからなかった場合は NULL を返します。
    //!
    const Group* FindGroupByName(const char* pFindName) const;

    //@}

private:
    GroupList m_GroupList;

};

} // namespace nn::ui2d
} // namespace nn
