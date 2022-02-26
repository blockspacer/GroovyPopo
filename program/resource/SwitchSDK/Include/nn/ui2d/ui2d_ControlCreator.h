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

#include <nn/ui2d/ui2d_Types.h>
#include <nn/font/font_ResourceFormat.h>

namespace nn
{
namespace ui2d
{

class Layout;

class ResExtUserData;
struct ResExtUserDataList;

//---------------------------------------------------------------------------
//! @brief コントロールを作成する際に必要な情報を保持するクラスです。
//!
//! @details
//! ControlCreator::CreateControl() の呼び出しで引数として渡され、
//! コントロールの作成に必要な情報を提供します。
//!
//! ControlSrc は、コントロール名、機能アニメーション、機能ペインという
//! 三種類の情報を提供します。
//!
//! コントロール名は、レイアウトライブラリでレイアウトを作成する際に、
//! 部品設定サブウィンドウの「コントロールの種類」で選択したコントロール
//! の名前が入ります。この名前は、レイアウトエディタで直接入力するのでは
//! なく、コントロール定義ファイルで定義された名前の一覧から選ぶ形式に
//! なります。
//!
//! 機能アニメーションは、コントロールの機能（例えばボタンを押したときの
//! 動きなど）に対応付けられたアニメーションのアニメーションタグ名を指定するものです。
//!
//! 機能アニメーションは、例えば「一つ目はボタンが選択されたとき、二つ目は
//! ボタンが押されたとき」等とあらかじめ決めておき、コントロール定義ファイル
//! にはその順番に書いておきます。機能アニメーションのインデックスによって、
//! どの機能にアニメーションタグが指定されたか判別します。
//!
//! 機能ペインも機能アニメーションとほぼ同じ使い方ですが、アニメーションタグではなく
//! ペインを指定します。これは、例えばボタンの当たり判定の範囲を示すペインを
//! 指定する場合などに使用します。
//!
//! 機能アニメーションと機能ペインは、レイアウトエディタ側で「指定しない」こと
//! もできることにご注意ください。その場合、 GetFunctionalAnimName や
//! GetFunctionalPaneName でNULLが返されます。
//!
//! @sa ControlCreator
//---------------------------------------------------------------------------
class ControlSrc
{
public:
    //! @brief コンストラクタです。
    //!
    //! @details
    //! メンバが未設定の状態で構築します。使用する前にInit関数でメンバの設定を
    //! 行うか、既に構築済みのインスタンスをコピーしてください。
    //!
    ControlSrc();

    //! @brief コンストラクタです。
    //!
    //! @param[in] pData  コントロールデータブロックのアドレスを渡します。
    //! @param[in] pUserDataBlock  拡張ユーザーデータブロックのアドレスを渡します。
    //!
    explicit ControlSrc(const void* pData, const ResExtUserDataList* pUserDataBlock) { Initialize(pData, pUserDataBlock); }

    //! @brief コントロールデータブロックのポインタを用いて初期化を行います。
    //!
    //! @param[in] pData  コントロールデータブロックのアドレスを渡します。
    //! @param[in] pUserDataBlock  拡張ユーザーデータブロックのアドレスを渡します。
    //!
    void Initialize(const void* pData, const ResExtUserDataList* pUserDataBlock);

    //! @brief コントロール名を取得します。
    //!
    //! @return コントロール名
    //!
    const char* GetControlName() const { return m_pControlName; }

    //! @brief コントロールのユーザーが付けた名前を取得します。
    //!
    //! @return コントロールのユーザーが付けた名前
    //!
    const char* GetControlUserName() const { return m_pControlUserName; }

    //! @brief 機能ペインの数を取得します。
    //!
    //! @return 機能ペインの数
    //!
    int GetFunctionalPaneCount() const { return m_FunctionalPaneCount; }

    //! @brief 機能アニメーションの数を取得します。
    //!
    //! @return 機能アニメーションの数
    //!
    int GetFunctionalAnimCount() const { return m_FunctionalAnimCount; }

    //! @brief   機能アニメーションのパラメータ名を定義ファイルに書かれた順番指定で取得します。
    const char* GetFunctionalAnimParameterName(int index) const
    {
        if (index < GetFunctionalAnimCount())
        {
            return nn::util::ConstBytePtr(m_pControlFunctionalAnimParameterNameOffsets, static_cast<const uint32_t*>(m_pControlFunctionalAnimParameterNameOffsets)[index]).Get<char>();
        }
        else
        {
            NN_SDK_ASSERT(false, "index exceeded [%d/%d]", index, GetFunctionalAnimCount());
            return NULL;
        }
    }

    //! @brief 機能ペイン名を定義ファイルに書かれた順番指定で取得します。
    //!
    //! @details
    //! 何個目の機能ペインがどのような意味を持つかは、あらかじめ決めておく
    //! 必要があります。
    //!
    //! @param[in] index    名前を取得したい機能ペインのインデックス
    //!
    //! @return 機能ペイン名。引数のインデックスで指定されていない場合はNULL。
    //!
    const char* GetFunctionalPaneName(int index) const;

    //! @brief 機能ペイン名をパラメータ名指定で取得します。
    //!
    //! @details
    //! 文字列比較を利用して探すぶん、GetFunctionalPaneNameより処理がかかりますが、
    //! コントロール定義ファイルを変更したときの対応が柔軟に行えるというメリットがあります。
    //!
    //! @param[in] pParameterName    パラメータ名
    //!
    //! @return 機能ペイン名。引数のインデックスで指定されていない場合はNULL。
    //!
    const char* FindFunctionalPaneName(const char* pParameterName) const;

    //! @brief 機能アニメーションのタグ名を定義ファイルに書かれた順番指定で取得します。
    //!
    //! @details
    //! 何個目の機能アニメーションがどのような意味を持つかは、あらかじめ決めておく
    //! 必要があります。
    //!
    //! @param[in] index    タグ名を取得したい機能アニメーションのインデックス
    //!
    //! @return 機能アニメーションタグ名。引数のインデックスで指定されていない場合はNULL。
    //!
    const char* GetFunctionalAnimName(int index) const;

    //! @brief 機能アニメーションのタグ名をパラメータ名指定で取得します。
    //!
    //! @details
    //! 文字列比較を利用して探すぶん、GetFunctionalAnimNameより処理がかかりますが、
    //! コントロール定義ファイルを変更したときの対応が柔軟に行えるというメリットがあります。
    //!
    //! @param[in] pParameterName    タグ名を取得したい機能アニメーションのパラメータ名
    //!
    //! @return 機能アニメーションタグ名。引数のインデックスで指定されていない場合はNULL。
    //!
    const char* FindFunctionalAnimName(const char* pParameterName) const;

    //! @brief 拡張ユーザデータの個数を取得します。
    //!
    //! @return 拡張ユーザデータの個数を返します。
    //!
    //! @sa GetExtUserDataArray
    //!
    uint16_t  GetExtUserDataCount() const;

    //! @brief 拡張ユーザデータ配列を取得します。
    //!
    //! @return 拡張ユーザデータ配列の先頭要素のポインタを返します。
    //!
    //! @sa GetExtUserDataCount
    //!
    const ResExtUserData* GetExtUserDataArray() const;

    //! @brief 拡張ユーザデータを名前をもとに検索します。
    //!
    //! @param[in] name 検索する拡張ユーザデータ名です。
    //!
    //! @return 見つかった場合は拡張ユーザデータへのポインタ、
    //! 見つからなかった場合は NULL を返します。
    //!
    const ResExtUserData* FindExtUserDataByName(const char* name) const;

private:

    const char* m_pControlName;
    const char* m_pControlUserName;
    uint16_t  m_FunctionalPaneCount;
    uint16_t  m_FunctionalAnimCount;
    const char* m_pFunctionalPaneNames;
    const void* m_pControlFunctionalAnimNameOffests;
    const void* m_pControlFunctionalPaneParameterNameOffsets;
    const void* m_pControlFunctionalAnimParameterNameOffsets;
    const ResExtUserDataList* m_pExtUserDataList;
};

//---------------------------------------------------------------------------
//! @brief コントロールの作成を行う基底クラスです。
//!
//! @details
//! Layout::Build() メソッドの引数として渡すことで、そのレイアウト及び部品レイアウトが
//! コントロールを持っていた場合にコントロールを作成します。
//!
//! このクラスは純粋仮想クラスとなっていますので、実際に使う際は、継承して CreateControl
//! メソッドを実装する必要があります。
//!
//! 名前空間 ctrl に、レイアウトライブラリが用意しているコントロールの実装があり、これを
//! デフォルトコントロールと呼んでいます。デフォルトコントロールでは、 ControlCreator を
//! 継承した、 DefaultControlCreator クラスでコントロールの作成を行います。
//!
//! デフォルトコントロールを使用せずに、独自にコントロールを実装することも可能です。
//!
//---------------------------------------------------------------------------
class ControlCreator
{
public:
    //! @brief デストラクタです。
    //!
    virtual ~ControlCreator() {}

    //! @brief 指定されたコントロールソースに対応するコントロールを作成します。
    //!
    //! @param[in] pDevice      デバイスです。
    //! @param[in] pLayout       コントロールを作成するレイアウト
    //! @param[in] controlSrc   作成すべきコントロールの情報。このオブジェクトのインスタンスは一時的な領域に作成しますので、
    //!                         このメソッドを抜けた後もポインタを保持しておくことはしないでください。情報を保持しておく
    //!                         必要がある場合は、コピーして保持するようにしてください。
    //!
    //! @details
    //! Layout::Build() 内で、レイアウトリソース内にコントロールがあった場合に呼び出されます。
    //!
    //! 呼び出されるタイミングは、ペインツリーの構築が終わった後です。つまり、引数で渡されるレイアウト
    //! は、コントロールを除いて構築済みの状態になっています。
    //!
    //! 作成したコントロールは Layout が管理するわけではありませんので、 ControlCreator が内部に保持
    //! する等の対応が必要です。
    //!
    //! 使い方としては、幾つかのパターンがあります。
    //!
    //! @li DefaultControlCreator のように作成したコントロールをメンバのリストに入れ、使いたいときは部品ペイン名で検索する。
    //! @li ControlCreator を継承したクラスで、内部にレイアウトへのポインタを持ち、 CreateControl が呼ばれたらレイアウトの
    //!     適切なメソッドに委譲する。レイアウトはその情報を元にコントロールを作成し、内部のメンバ変数に保持する。
    //!
    virtual void CreateControl(nn::gfx::Device* pDevice, Layout* pLayout, const ControlSrc& controlSrc) = 0;

};

} // namespace nn::ui2d
} // namespace nn
