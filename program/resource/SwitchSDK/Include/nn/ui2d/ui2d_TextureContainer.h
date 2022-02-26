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

#include <nn/ui2d/ui2d_Types.h>
#include <nn/ui2d/ui2d_Layout.h>

namespace nn
{
namespace ui2d
{

//---------------------------------------------------------------------------
//  テクスチャを保持するクラスです。
//
// TextureContainer
//
//---------------------------------------------------------------------------

//! @brief 内部用機能のため使用禁止です。
class TextureRefLink
{
    NN_DISALLOW_COPY(TextureRefLink);

public:

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    // コンストラクタです。

    //! @brief 内部用機能のため使用禁止です。
    explicit TextureRefLink(TextureInfo* pTextureInfo, bool isOwned);

    // デストラクタです。
    // OpenGL のテクスチャオブジェクトを破棄します。

    //! @brief 内部用機能のため使用禁止です。
    ~TextureRefLink();

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    void Finalize(nn::gfx::Device* pDevice);

    // @}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    // テクスチャ名を設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pName テクスチャを特定する名前です。
    //!
    void SetName(
        const char* pName);

    // テクスチャのリソース名を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return テクスチャのリソース名を返します。
    //!
    const char* GetResourceName() const
    {
        return m_Name;
    }

    // テクスチャ情報を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return テクスチャ情報を返します。
    //!
    TextureInfo& GetTextureInfo()
    {
        return *m_pTexInfo;
    }

    // テクスチャ情報を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return テクスチャ情報を返します。
    //!
    const TextureInfo& GetTextureInfo() const
    {
        return *m_pTexInfo;
    }

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return コンテナがオブジェクトを所有するかのフラグです。
    //!
    bool IsOwned() const
    {
        return m_IsOwned;
    }

    // @}

    // リンク情報です。

    //! @brief 内部用機能のため使用禁止です。
    nn::util::IntrusiveListNode m_Link;

private:
    char m_Name[TexImageNameMax];
    TextureInfo* m_pTexInfo;
    bool m_IsOwned;
};

//---------------------------------------------------------------------------
//! @brief テクスチャを保持するクラスです。
//!
//! @details
//! ResourceAccessor の実装で OpenGL のテクスチャオブジェクトを保持するのに
//! 利用されます。
//!
//! @sa ResourceAccessor
//!
//---------------------------------------------------------------------------
class TextureContainer : public nn::util::IntrusiveList<TextureRefLink, nn::util::IntrusiveListMemberNodeTraits<TextureRefLink, &TextureRefLink::m_Link> >
{
    NN_DISALLOW_COPY(TextureContainer);

public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    TextureContainer() {}

    //! @brief デストラクタです。
    //!
    //! @details
    //! 登録されているすべてのテクスチャを破棄します。
    //!
    ~TextureContainer();

    //@}

    //----------------------------------------
    //! @name 登録／破棄
    //@{

    //! @brief 登録を抹消します。
    //!
    //! @details
    //! 登録されているすべてのテクスチャを破棄します。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    void Finalize(nn::gfx::Device* pDevice);

    //! @brief テクスチャ情報を登録します。
    //!
    //! @param[in] pName テクスチャの名前です。
    //!
    //! @return テクスチャ情報です。メモリ確保できない場合には NULL が返ります。
    //!
    ResourceTextureInfo* RegisterResourceTexture(const char* pName);

    //! @brief テクスチャ情報を登録します。
    //!
    //! @param[in] pName テクスチャの名前です。
    //! @param[in] isOwned テクスチャを所有するかどうかです。false に指定されたものはディスクリプタプールの登録、解放処理や、終了処理の対象になりません。
    //!
    //! @return テクスチャ情報です。メモリ確保できない場合には NULL が返ります。
    //!
    PlacementTextureInfo* RegisterPlacementTexture(const char* pName, bool isOwned);

    //! @brief レンダーターゲットテクスチャ情報を登録します。
    //!
    //! @param[in] pName テクスチャの名前です。
    //! @param[in] isOwned テクスチャを所有するかどうかです。false に指定されたものはディスクリプタプールの登録、解放処理や、終了処理の対象になりません。
    //!
    //! @return テクスチャ情報です。メモリ確保できない場合には NULL が返ります。
    //!
    RenderTargetTextureInfo* RegisterRenderTargetTexture(const char* pName, bool isOwned);

    //! @brief テクスチャの登録を抹消します。
    //!
    //! @param[in] pTexInfo テクスチャ情報です。
    //!
    void UnregisterTexture(TextureInfo* pTexInfo);

    /**
    * @brief テクスチャービューをディスクリプタプールに登録します。
    *
    * @param[in] regsiterTextureViewSlot     ディスクリプタスロット確保関数です。
    * @param[in] pUserData     関数に渡されるユーザーデーターです。
    *
    */
    void RegisterTextureViewToDescriptorPool(RegisterTextureViewSlot regsiterTextureViewSlot, void* pUserData);

    /**
    * @brief テクスチャービューをディスクリプタプールから解放します。
    *
    * @param[in] unregsiterTextureViewSlot     ディスクリプタスロット解放関数です。
    * @param[in] pUserData     関数に渡されるユーザーデーターです。
    *
    */
    void UnregisterTextureViewFromDescriptorPool(UnregisterTextureViewSlot unregsiterTextureViewSlot, void* pUserData);

    //@}

    //----------------------------------------
    //! @name 検索
    //@{

    //! @brief テクスチャ情報を検索します。
    //!
    //! @param[in] pName テクスチャのリソース名です。
    //!
    //! @return
    //! テクスチャ情報を返します。
    //!
    const TextureInfo* FindTextureByName(const char* pName) const;

    //@}
};

} // namespace nn::ui2d
} // namespace nn
