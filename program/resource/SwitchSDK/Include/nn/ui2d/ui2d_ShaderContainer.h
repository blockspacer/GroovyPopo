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
#include <nn/ui2d/ui2d_ShaderInfo.h>

namespace nn
{
namespace ui2d
{

//---------------------------------------------------------------------------
// シェーダを保持するクラスです。
//
//---------------------------------------------------------------------------

//! @brief 内部用機能のため使用禁止です。
class ShaderRefLink
{
    NN_DISALLOW_COPY(ShaderRefLink);

public:

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    // コンストラクタです。

    //! @brief 内部用機能のため使用禁止です。
    explicit ShaderRefLink(bool isOwned);

    // デストラクタです。

    //! @brief 内部用機能のため使用禁止です。
    ~ShaderRefLink();

    //! @brief 内部用機能のため使用禁止です。
    void Finalize(nn::gfx::Device* pDevice);

    // @}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    // シェーダ名を設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pResourceName シェーダを特定する名前です。
    //!
    void SetName(
        const char* pResourceName);

    // シェーダのリソース名を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return シェーダのリソース名を返します。
    //!
    const char* GetResourceName() const
    {
        return m_Name;
    }

    // シェーダ情報を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return シェーダ情報を返します。
    //!
    ShaderInfo& GetShaderInfo()
    {
        return m_ShaderInfo;
    }

    // シェーダ情報を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return シェーダ情報を返します。
    //!
    const ShaderInfo& GetShaderInfo() const
    {
        return m_ShaderInfo;
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
    char m_Name[ShaderNameMax];
    ShaderInfo m_ShaderInfo;
    bool m_IsOwned;
};

//---------------------------------------------------------------------------
//! @brief シェーダを保持するクラスです。
//!
//! @sa ResourceAccessor
//!
//---------------------------------------------------------------------------
class ShaderContainer : public nn::util::IntrusiveList<ShaderRefLink, nn::util::IntrusiveListMemberNodeTraits<ShaderRefLink, &ShaderRefLink::m_Link> >
{
    NN_DISALLOW_COPY(ShaderContainer);

public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    ShaderContainer() {}

    //! @brief デストラクタです。
    //!
    //! @details
    //! 登録されているすべてのシェーダを破棄します。
    //!
    ~ShaderContainer();

    //@}

    //----------------------------------------
    //! @name 登録／破棄
    //@{

    //! @brief 登録を抹消します。
    //!
    //! @details
    //! 登録されているすべてのシェーダを破棄します。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    void Finalize(nn::gfx::Device* pDevice);

    //! @brief シェーダ情報を登録します。
    //!
    //! @param[in] pName シェーダの名前です。
    //! @param[in] isOwned シェーダーを所有するかどうかです。false に指定されたものは終了処理の対象になりません。
    //!
    //! @return シェーダ情報です。メモリ確保できない場合には NULL が返ります。
    //!
    ShaderInfo* RegisterShader(const char* pName, bool isOwned);

    //! @brief シェーダの登録を抹消します。
    //!
    //! @param[in] pShaderInfo シェーダ情報です。
    //!
    void UnregisterShader(ShaderInfo* pShaderInfo);

    //@}

    //----------------------------------------
    //! @name 検索
    //@{

    //! @brief シェーダ情報を検索します。
    //!
    //! @param[in] pName シェーダのリソース名です。
    //!
    //! @return
    //! シェーダ情報を返します。
    //!
    const ShaderInfo* FindShaderByName(const char* pName) const;

    //@}
};

} // namespace nn::ui2d
} // namespace nn
