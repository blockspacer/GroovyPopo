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

#include <nn/util/util_IntrusiveList.h>
#include <nn/ui2d/ui2d_ResourceAccessor.h>
#include <cstddef>

namespace nn
{
namespace ui2d
{

//---------------------------------------------------------------------------
// フォントオブジェクトを保持するクラスです。
//
// sa FontContainer
//
//---------------------------------------------------------------------------

//! @brief 内部用機能のため使用禁止です。
class FontRefLink
{
    NN_DISALLOW_COPY(FontRefLink);

public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    // コンストラクタです。

    //! @brief 内部用機能のため使用禁止です。
    FontRefLink();

    // デストラクタです。
    // 所有の指定されたフォントオブジェクトを破棄します。
    // 2010/01/26 所有の指定されたフォントオブジェクトは破棄する仕様に変更しました。

    //! @brief 内部用機能のため使用禁止です。
    ~FontRefLink();

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    void Finalize(nn::gfx::Device* pDevice);

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    // 情報を設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pName    フォントリソースの名前です。
    //! @param[in] pFont    フォントオブジェクトへのポインタです。
    //! @param[in] own      コンテナがフォントオブジェクトを所有するか指定します。
    //!                     true を指定した場合にはコンテナがフォントオブジェクトを破棄します。
    //!
    void Set(
        const char* pName,
        nn::font::Font* pFont,
        bool own
    );

    // 関連付けられているフォント名を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return 関連付けられているフォント名を返します。
    //!
    const char* GetFontName() const
    {
        return m_FontName;
    }

    // 設定されているフォントオブジェクトを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return フォントオブジェクトへのポインタを返します。
    //!
    nn::font::Font* GetFont() const
    {
        return m_pFont;
    }


    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return コンテナがフォントオブジェクトを所有するか。
    //!
    bool IsOwned() const
    {
        return m_IsOwned;
    }

    //@}

    // リストのリンク情報です。

    //! @brief 内部用機能のため使用禁止です。
    nn::util::IntrusiveListNode m_Link;

private:
    char m_FontName[FontNameMax];
    nn::font::Font* m_pFont;
    bool m_IsOwned;

};

//---------------------------------------------------------------------------
//! @brief フォントを保持するクラスです。
//!
//! @details
//! ResourceAccessor の実装でフォントを保持するのに利用されます。
//!
//! @sa ResourceAccessor
//!
//---------------------------------------------------------------------------
class FontContainer : public nn::util::IntrusiveList<FontRefLink, nn::util::IntrusiveListMemberNodeTraits<FontRefLink, &FontRefLink::m_Link> >
{
    NN_DISALLOW_COPY(FontContainer);

public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 所有しているフォントオブジェクトも破棄します。
    //!
    FontContainer() {}

    //! @brief デストラクタです。
    //!
    //! @details
    //! 所有しているフォントオブジェクトも破棄します。
    //!
    ~FontContainer();

    // @}

    //----------------------------------------
    //! @name 登録／破棄
    //@{

    //! @brief 登録を抹消します。
    //!
    //! @details
    //! 所有しているフォントオブジェクトを破棄します。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    void Finalize(nn::gfx::Device* pDevice);

    //! @brief フォントを登録します。
    //!
    //! @param[in] pName    フォントリソースの名前です。
    //! @param[in] pFont    フォントオブジェクトです。
    //! @param[in] own      フォントオブジェクトを所有するか指定します。
    //!                     true を指定した場合にはコンテナがフォントオブジェクトを破棄します。
    //!
    //! @return 登録キーを返します。キーは登録の抹消に使用します。
    //!
    FontKey RegisterFont(const char* pName, nn::font::Font* pFont, bool own);

    //! @brief フォントの登録を抹消します。
    //!
    //! @param[in] key  登録時に取得した登録キーを指定します。
    //!
    void UnregisterFont(FontKey key);

    /**
    * @brief テクスチャービューをディスクリプタプールに登録します。
    *
    * @param[in] pRegisterTextureViewSlot     ディスクリプタスロット確保関数です。
    * @param[in] pUserData     関数に渡されるユーザーデーターです。
    *
    */
    void RegisterTextureViewToDescriptorPool(RegisterTextureViewSlot pRegisterTextureViewSlot, void* pUserData);

    /**
    * @brief テクスチャービューをディスクリプタプールから解放します。
    *
    * @param[in] pUnregisterTextureViewSlot     ディスクリプタスロット解放関数です。
    * @param[in] pUserData     関数に渡されるユーザーデーターです。
    *
    */
    void UnregisterTextureViewFromDescriptorPool(UnregisterTextureViewSlot pUnregisterTextureViewSlot, void* pUserData);

    //@}

    //----------------------------------------
    //! @name 検索
    //@{

    //! @brief フォントを検索します。
    //!
    //! @param[in] pName 検索するフォントの名前です。
    //!
    //! @return 見つかったフォント、または NULL を返します。
    //!
    nn::font::Font* FindFontByName(
        const char* pName
    ) const;
};

}   // namespace ui2d
}   // namespace nn
