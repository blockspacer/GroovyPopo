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

#include <nn/ui2d/ui2d_AnimButton.h>
#include <nn/ui2d/ui2d_DragButton.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn
{
namespace ui2d
{

//! @brief ボタンのリストの定義です。
//!
typedef nn::util::IntrusiveList<AnimButton, nn::util::IntrusiveListMemberNodeTraits<AnimButton, &AnimButton::m_Link> > ButtonList;

//---------------------------------------------------------------------------
//! @brief ボタンをリストで管理し、状態の更新などの処理をまとめておこなうためのクラスです。
//!
//! @details
//! ボタンの仕組みでは、個々のボタンは自分に当たりがあるという情報を管理しておらず、
//! ButtonGroupが現在当たりのあるボタンを知っています。
//! 同じButtonGroupに属するボタンは、同時に一つだけ当たりのある状態になります。
//!
//---------------------------------------------------------------------------
class ButtonGroup
{
public:
    //! @brief コンストラクタです。
    //!
    ButtonGroup();

    //! @brief デストラクタです。
    //!
    //! @details
    //! このクラスでは何も行いません。
    //!
    virtual ~ButtonGroup();

    //----------------------------------------
    //! @name 状態の更新
    //@{
    //! @brief 状態の更新を行います。
    //!
    //! @details
    //! ゲームフレームを進めるたびに1回呼び出してください。以下の処理を行っています。
    //!
    //! @li IsUpdateHitBox()がtrueを返す場合に、ボタンのあたり判定の範囲を再計算します。
    //! @li 受け取ったカーソルの位置から、現在当たっているボタンを調べます。このとき、
    //!     IsExcludeDown()がtrueになっていてかつ現在押されているボタンがあり、かつ
    //!     そのボタンのIsExcludeDownもtrueになっているときは、ボタンの当たりを取りません。
    //! @li 直前に取ったボタンの当たりと引数isDownの情報に従って、ボタンにON/OFF/DOWN
    //!     アクションを発行します。
    //! @li ボタンのUpdateメソッドを呼び出します。
    //!
    //! @param[in] pPos      現在ポインタが指示している位置です。レイアウトの座標系(中心が0,0で、
    //!                     右上がプラス)で渡してください。
    //! @param[in] isDown   ボタンが押されたか否か。通常は、WiiリモコンであればAボタンが
    //!                     押されたとき、マウスであれば左クリックされたときですが、それ以外の
    //!                     操作でも問題ありません。
    //! @param[in] isRelease ボタンが離されたか否か。通常は、WiiリモコンであればAボタンが
    //!                     離されたとき、マウスであれば左ボタンを離したときです。
    //!                     デフォルトではfalseが指定されます。
    //!                     ボタンのドラッグ機能を使わなければ常にfalseで問題ありません。
    //!
    virtual void Update(const nn::util::Float2* pPos, bool isDown, bool isRelease = false);
    //@}

    //----------------------------------------
    //! @name 設定
    //@{
    //! @brief ボタンの当たり判定を更新するか否かを取得します。
    //!
    //! @return ボタンの当たり判定を更新するか否か
    //!
    bool IsUpdateHitBox() const { return m_Flag.IsAllBitOn(FlagMask_UpdateHitBox); }

    //! @brief ボタンが押されたときに他のボタンが押されるのを防ぐ処理を行うか否かを取得します。
    //!
    //! @return ボタンが押されたときに他のボタンが押されるのを防ぐ処理を行うか否か
    //!
    bool IsExcludeDown() const { return m_Flag.IsAllBitOn(FlagMask_ExcludeDown); }

    //! @brief 当たり判定を行うか否かを取得します。
    //!
    //! @return 当たり判定を行うか否か
    //!
    bool IsHitTestEnabled() const { return m_Flag.IsAllBitOn(FlagMask_HitTestEnabled); }

    //! @brief ボタンの当たり判定を更新するか否かを取得します。
    //!
    //! @details
    //! ボタンの当たり判定は、デフォルトではUpdate()メソッド内で更新されますが、
    //! このメソッドにfalseを与えることで、更新しないようにすることもできます。
    //!
    //! この場合、ボタンがアニメーションして大きくなったり位置が動いたりしたときに
    //! ボタンの当たり判定がおかしくなってしまいますが、当たり判定の処理を行わない
    //! ため、処理を削減することができます。
    //!
    //! 画面内でボタンの大きさや位置が変わらない場合に使用してください。
    //!
    //! また、これをfalseにして使用する際は、適切な場所で最低一回はボタンのUpdateHitBox
    //! を呼び出す必要があることにご注意ください。
    //!
    //! @param[in] b    ボタンの当たり判定を更新するときはtrueを指定します。
    //!
    void SetUpdateHitBox(bool b) { return m_Flag.SetMaskedBits(FlagMask_UpdateHitBox, b ? FlagMask_UpdateHitBox : 0); }

    //! @brief ボタンが押されたときに他のボタンが押されるのを防ぐ処理を行うか否かを設定します。
    //!
    //! @details
    //! この設定はボタン個別にも設定することができますが、ButtonGroupで設定をOFFにした場合は、
    //! ボタンの設定を無視して、この処理を行わなくなりますのでご注意ください。デフォルトでは
    //! trueが指定されます。
    //!
    //! ボタンが押された時の排他処理が不要な場合は、このメソッドでfalseを与えることにより、
    //! 処理を削減することができます。
    //!
    //! @param[in] b    ボタンが押されたときに他のボタンが押されるのを防ぐ処理を行うときはtrueを指定します。
    //!
    void SetExcludeDown(bool b) { return m_Flag.SetMaskedBits(FlagMask_ExcludeDown, b ? FlagMask_ExcludeDown : 0); }

    //! @brief 当たり判定を行うか否かを取得します。
    //!
    //! @details
    //! このメソッドでfalseを指定すると、当たり判定が行われなくなります。デフォルトではtrueが指定されます。
    //!
    //! 例えばダイアログボックスなどで、出てくるアニメーションを行っている間はボタンに当たって欲しく
    //! ない場合に使用することを想定しています。
    //!
    //! このメソッドを使用しても、ボタンのステートを強制的に変更するわけではありませんので、例えば
    //! ボタンにカーソルが当たっているときにこのメソッドが呼ばれると、カーソルの当たりが外れて、
    //! ボタンのOFFアニメーションが再生されます。
    //!
    //! @param[in] b    ボタンの当たり判定を行うときはtrueを指定します。
    //!
    void SetHitTestEnabled(bool b) { return m_Flag.SetMaskedBits(FlagMask_HitTestEnabled, b ? FlagMask_HitTestEnabled : 0); }
    //@}

    //----------------------------------------
    //! @name ボタンへのアクセス
    //@{
    //! @brief 現在当たり判定があるボタンを取得します。
    //!
    //! @details
    //! どのボタンにも当たり判定がない場合はNULLが返されます。
    //!
    //! @return 現在当たり判定があるボタン
    //!
    AnimButton* GetHitButton() const { return m_HitButton; }

    //! @brief 現在ドラッグしているボタンを取得します。
    //!
    //! @details
    //! ドラッグしていない場合はNULLが返されます。
    //!
    //! @return ドラッグしているボタンボタン
    //!
    AnimButton* GetDraggingButton() const { return m_DraggingButton; }

    //! @brief リスト内で押されているボタンを探し、最初に見つかったボタンを取得します。
    //!
    //! @details
    //! IsExcludeDownがtrueでないボタンがある場合は、複数のボタンが同時に押される可能性
    //! がありますが、この関数では最初の一つしか見つけることができませんのでご注意ください。
    //!
    //! 押されているボタンがない場合はNULLが返されます。
    //!
    //! @return 押されているボタン
    //!
    AnimButton* FindDownButton();

    //! @brief ボタンのリストを取得します。
    //!
    //! @return ボタンのリスト
    //!
    const ButtonList& GetButtonList() const
    {
        return m_ButtonList;
    }

    //! @brief ボタンのリストを取得します。
    //!
    //! @return ボタンのリスト
    //!
    ButtonList& GetButtonList()
    {
        return m_ButtonList;
    }

    //! @brief 指定された名前で、ボタンを前方から検索します。
    //!
    //! @details
    //! リストを前方から検索して、最初に見つけたボタンを返します。
    //!
    //! 指定された名前のボタンがない場合はNULLが返されます。
    //!
    //! @param[in] pButtonName 検索するボタン名です。
    //!
    //! @return 検索結果のボタン
    //!
    AnimButton* FindButtonByName(const char* pButtonName);

    //! @brief 指定された名前で、ボタンを前方から検索します。
    //!
    //! @details
    //! リストを前方から検索して、最初に見つけたボタンを返します。
    //!
    //! 指定された名前のボタンがない場合はNULLが返されます。
    //!
    //! @param[in] pButtonName 検索するボタン名です。
    //!
    //! @return 検索結果のボタン
    //!
    const AnimButton* FindButtonByName(const char* pButtonName) const;

    //! @brief 指定された名前で、ボタンを後方から検索します。
    //!
    //! @details
    //! リストを後方から検索して、最初に見つけたボタンを返します。
    //!
    //! 指定された名前のボタンがない場合はNULLが返されます。
    //!
    //! @param[in] pButtonName 検索するボタン名です。
    //!
    //! @return 検索結果のボタン
    //!
    AnimButton* FindButtonByNameReverse(const char* pButtonName);
    //! @brief 指定された名前で、ボタンを後方から検索します。
    //!
    //! @details
    //! リストを後方から検索して、最初に見つけたボタンを返します。
    //!
    //! 指定された名前のボタンがない場合はNULLが返されます。
    //!
    //! @param[in] pButtonName 検索するボタン名です。
    //!
    //! @return 検索結果のボタン
    //!
    const AnimButton* FindButtonByNameReverse(const char* pButtonName) const;

    //! @brief 指定されたタグで、ボタンを前方から検索します。
    //!
    //! @details
    //! リストを前方から検索して、最初に見つけたボタンを返します。
    //!
    //! 指定されたタグのボタンがない場合はNULLが返されます。
    //!
    //! @param[in] tag 検索するタグです。
    //!
    //! @return 検索結果のボタン
    //!
    AnimButton* FindButtonByTag(int tag);
    //! @brief 指定されたタグで、ボタンを前方から検索します。
    //!
    //! @details
    //! リストを前方から検索して、最初に見つけたボタンを返します。
    //!
    //! 指定されたタグのボタンがない場合はNULLが返されます。
    //!
    //! @param[in] tag 検索するタグです。
    //!
    //! @return 検索結果のボタン
    //!
    const AnimButton* FindButtonByTag(int tag) const;


    //! @brief 指定された部品レイアウトで、ボタンを前方から検索します。
    AnimButton* FindButtonByLayout(const nn::ui2d::Layout* pPartsLayout);
    //@}

    //----------------------------------------
    //! @name ボタンに対して一括で処理を行う
    //@{
    //! @brief グループに含まれるボタン全てに対してForceOffを呼びます。
    //!
    void ForceOffAll();

    //! @brief グループに含まれるボタン全てに対してForceOnを呼びます。
    //!
    void ForceOnAll();

    //! @brief グループに含まれるボタン全てに対してForceDownを呼びます。
    //!
    void ForceDownAll();

    //! @brief グループに含まれるボタン全てに対してUpdateHitBoxを呼びます。
    //!
    //! @details
    //! 毎フレーム当たり判定を更新する設定になっている場合は、このメソッドを
    //! 使用する必要はありません。
    //!
    //! 更新する設定になっていないときに、必要なタイミングで呼び出してください。
    //! なお、UpdateHitBoxはペインのグローバルマトリックスを参照するため、この
    //! メソッドを呼び出すときは最低一回 nn::ui2d::Layout::Calculate() を呼び出しておく
    //! 必要があることにご注意ください。
    //!
    void UpdateHitBoxAll();

    //! @brief グループに含まれるボタン全てに対してSetStateChangeCallbackを呼び出すユーティリティメソッドです。
    //!
    //! @param[in] pCallback 呼び出したいコールバック
    //! @param[in] pParam    コールバックに与えるパラメータ
    //!
    void SetStateChangeCallbackAll(AnimButton::StateChangeCallback pCallback, void* pParam);

    //! @brief グループに含まれるボタン全てに対してCancelを呼び出すユーティリティメソッドです。
    //!
    void CancelAll();

    //! @brief グループに含まれるボタン全てに対してLayout::DeleteObjを呼び出し、リストを空にします。
    //!
    //! @details
    //! DefaultControlCreator等で、Layout::NewObjで確保したメモリ領域を解放したいときに使用してください。
    //! ButtonGroupではデストラクタはFreeAllを呼び出しませんので、必要な場合は明示的に呼び出してください。
    //!
    void FreeAll();
    //@}

protected:
    static const int    FlagBitMax = 3;    //!< m_Flagで使用しているビットの最大値

    enum FlagMask {
        FlagMask_UpdateHitBox = 1 << 0,
        FlagMask_ExcludeDown = 1 << 1,
        FlagMask_HitTestEnabled = 1 << 2
    };

private:

    ButtonList m_ButtonList;
    AnimButton* m_HitButton;
    AnimButton* m_DraggingButton;
    nn::util::BitPack32 m_Flag;

};

} // namespace nn::ui2d
} // namespace nn
