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
#include <nn/ui2d/ui2d_Layout.h>
#include <nn/ui2d/ui2d_Util.h>

namespace nn
{
namespace font
{
class Font;
}
}

namespace nn
{
namespace ui2d
{

class ShaderInfo;

//---------------------------------------------------------------------------
//! @brief リソースにアクセスするための基底クラスです。
//!
//---------------------------------------------------------------------------
class ResourceAccessor
{
public:
    //! @brief アーカイブシェーダーの接頭辞です。
    static const char* ArchiveShaderPrefix;
    //! @brief アーカイブシェーダーの接尾辞です。
    static const char* ArchiveShaderSuffix;

    //! @brief アーカイブシェーダーのリソース名を取得します。
    //!
    //! @param[out] pBuffer       結果を出力するバッファです。
    //! @param[in] pShaderName    シェーダー名です。
    //! @param[in] bufferSize     結果を出力するバッファのバイトサイズです。
    //!
    static void GetArchiveShaderResourceName(char* pBuffer, const char* pShaderName, int bufferSize)
    {
        nn::util::SNPrintf(pBuffer, bufferSize, "ArchiveShader-%s.bnsh", pShaderName);
    }

    //! @brief コールバック関数です。
    //!
    //! @param[in] pRes リソースのポインタです。
    //! @param[in] size リソースのサイズです。
    //! @param[in] pName リソースの名前です。
    //! @param[in] pParam コールバックに与えるパラメータです。
    typedef void(*FindResourceByTypeCallback)(const void* pRes, size_t size, const char* pName, void* pParam);

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

protected:
    //! @brief コンストラクタです。
    //!
    ResourceAccessor();

public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO_ROOT();

    //! @brief デストラクタです。
    //!
    //! @details
    //! 導出クラスでは管理下のフォントとテクスチャの破棄を
    //! 行わなければなりません。
    //!
    //! @date 2010/01/26 フォントとテクスチャの破棄について加筆しました。
    virtual ~ResourceAccessor();

    //@}

    //----------------------------------------
    //! @name 初期化・終了
    //@{

    /**
    * @brief テクスチャービューをディスクリプタプールに登録します。
    *
    * @param[in] pRegisterTextureViewSlot     ディスクリプタスロット確保関数です。
    * @param[in] pUserData     関数に渡されるユーザーデーターです。
    *
    */
    virtual void RegisterTextureViewToDescriptorPool(RegisterTextureViewSlot pRegisterTextureViewSlot, void* pUserData) = 0;

    /**
    * @brief テクスチャービューをディスクリプタプールから解放します。
    *
    * @param[in] pUnregisterTextureViewSlot     ディスクリプタスロット解放関数です。
    * @param[in] pUserData     関数に渡されるユーザーデーターです。
    *
    */
    virtual void UnregisterTextureViewFromDescriptorPool(UnregisterTextureViewSlot pUnregisterTextureViewSlot, void* pUserData) = 0;

    //! @brief  キャプチャテクスチャをリソースへ登録します。
    //!
    //! @param[in]  pName   登録するキャプチャテクスチャの名前です。
    //!
    //! @return 登録した RenderTargetTextureInfo のポインタです。
    //!
    //! @sa UnregisterRenderTargetTexture
    //!
    virtual RenderTargetTextureInfo* RegisterRenderTargetTexture(const char* pName)
    {
        NN_UNUSED(pName);
        NN_SDK_ASSERT(false, "ResourceAccessor::RegisterRenderTargetTexture is not implemented. Please implement this method if you use capture-texture functionalities.");
        return NULL;
    }

    //! @brief キャプチャテクスチャの登録を解除します。
    //!
    //! @param[in] pTexInfo テクスチャ情報です。
    //!
    //! @sa RegisterRenderTargetTexture
    //!
    virtual void UnregisterRenderTargetTexture(TextureInfo* pTexInfo)
    {
        NN_UNUSED(pTexInfo);
        NN_SDK_ASSERT(false, "ResourceAccessor::UnregisterRenderTargetTexture is not implemented. Please implement this method if you use capture-texture functionalities.");
    }

    //! @brief 終了処理をします。
    //!
    //! @param[in] pDevice      デバイスです。
    //!
    virtual void Finalize(nn::gfx::Device* pDevice);

    //@}

    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief リソースを取得します。
    //!
    //! @param[out] pSize   リソースのサイズを受け取る変数へのポインタです。
    //!                     サイズを受け取る必要がない場合は NULL を渡してください。
    //! @param[in] resType  リソースの種類です。
    //! @param[in] pName    リソースの名前です。
    //!
    //! @return
    //! リソースの先頭アドレスへのポインタを返します。
    //! リソースが見つからなかった場合は NULL を返します。
    //!
    virtual void* FindResourceByName(
        size_t * pSize,
        ResType resType,
        const char* pName
    ) = 0;

    //! @brief リソースを取得します。
    //!
    //! @param[out] pSize   リソースのサイズを受け取る変数へのポインタです。
    //!                     サイズを受け取る必要がない場合は NULL を渡してください。
    //! @param[in] resType  リソースの種類です。
    //! @param[in] pName    リソースの名前です。
    //!
    //! @return
    //! リソースの先頭アドレスへのポインタを返します。
    //! リソースが見つからなかった場合は NULL を返します。
    //!
    virtual const void* FindResourceByName(size_t * pSize, ResType resType, const char* pName) const
    {
        return const_cast<ResourceAccessor*>(this)->FindResourceByName(pSize, resType, pName);
    }

    //! @brief リソースを取得します。
    //!
    //! @param[in] resType  リソースの種類です。
    //! @param[in] pName    リソースの名前です。
    //!
    //! @return
    //! リソースの先頭アドレスへのポインタを返します。
    //! リソースが見つからなかった場合は NULL を返します。
    //!
    virtual void* FindResourceByName(ResType resType, const char* pName)
    {
        return FindResourceByName(NULL, resType, pName);
    }

    //! @brief リソースを取得します。
    //!
    //! @param[in] resType  リソースの種類です。
    //! @param[in] pName    リソースの名前です。
    //!
    //! @return
    //! リソースの先頭アドレスへのポインタを返します。
    //! リソースが見つからなかった場合は NULL を返します。
    //!
    virtual const void* FindResourceByName(ResType resType, const char* pName) const
    {
        return FindResourceByName(NULL, resType, pName);
    }

    //! @brief 指定した種類のリソースをすべて取得します。
    //!
    //! @param[in] resType  リソースの種類です。
    //! @param[in] pCallback リソースが見つかるたびに呼ばれるコールバック関数です。
    //! @param[in] pParam コールバックに与えるパラメータです。
    //!
    virtual void FindResourceByType(
        ResType resType,
        FindResourceByTypeCallback pCallback,
        void* pParam
    ) const = 0;

    //! @brief フォントを取得します。
    //!
    //! @details
    //! name で指定したフォントリソース名と関連付けられた
    //! フォントオブジェクトを取得します。
    //!
    //! フォントを所有していない場合にはロードを試みます。
    //!
    //! 実装には LoadFont() を利用することができます。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] pName フォントリソースの名前です。
    //!
    //! @return フォントへのポインタを返します。
    //! フォントを取得できなかった場合には NULL を返します。
    //!
    //! @sa LoadFont
    //!
    //! @date 2010/07/16 純粋仮想関数に変更しました。
    //!
    virtual nn::font::Font* AcquireFont(nn::gfx::Device* pDevice, const char* pName) = 0;

    //! @brief テクスチャ情報を取得します。
    //!
    //! @details
    //! name で指定したテクスチャリソース名と関連付けられた
    //! テクスチャ情報を取得します。
    //!
    //! テクスチャを所有していない場合にはロードを試みます。
    //!
    //! 実装には LoadTexture() を利用することができます。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] pName テクスチャリソースの名前です。
    //!
    //! @return テクスチャ情報を返します。
    //! テクスチャを所有していない場合にはロードを試みます。
    //! ロードできなかった場合にはテクスチャオブジェクトには無効な値
    //! (TextureInfo::INVALID) が設定されます。
    //!
    //! @sa LoadTexture
    //!
    //! @date 2010/01/26 非バーチャル関数に変更しました。
    //! @date 2010/06/18 純粋仮想関数に変更しました。
    //!
    virtual const TextureInfo* AcquireTexture(nn::gfx::Device* pDevice, const char *pName) = 0;

    //! @brief BuildArgSet のパーツ階層修飾情報と名前から動的生成テクスチャ名を生成してテクスチャ情報を取得します。
    //!
    //! @details
    //! pNewNameCopy は不要な場合は NULL を指定することができます。
    //! 内部では生成した名前で AcquireTexture を呼び出しており、見つからなかった場合などの挙動は AcquireTexture に準じます。
    //!
    //! @param[out] pNewNameCopy 最終的な名前の文字列が必要な場合は書き込み先のバッファを設定します。NULL を設定すると何も処理されません。
    //! @param[in]  newNameCopyBufferSize pNewNameCopy バッファのサイズを指定します。
    //! @param[in]  buildArgSet BuildArgSet への参照です。
    //! @param[in]  overrided 上書きされた動的生成テクスチャかどうかを指定します。
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  pName 動的生成テクスチャの名前を指定します。
    //!
    //! @return 見つかった TextureInfo を返します。
    //!
    virtual const TextureInfo* AcquireDynamicGenerateTextureWithResolvePrefix(char* pNewNameCopy, int newNameCopyBufferSize, const BuildArgSet& buildArgSet, bool overrided, nn::gfx::Device* pDevice, const char* pName);

    //! @brief プリフィックスとテクスチャ名から動的生成テクスチャ名を生成してテクスチャ情報を取得します。
    //!
    //! @details
    //! pNewNameCopy は不要な場合は NULL を指定することができます。
    //! 内部では生成した名前で AcquireTexture を呼び出しており、見つからなかった場合などの挙動は AcquireTexture に準じます。
    //!
    //! @param[out] pNewNameCopy 最終的な名前の文字列が必要な場合は書き込み先のバッファを設定します。NULL を設定すると何も処理されません。
    //! @param[in]  newNameCopyBufferSize pNewNameCopy バッファのサイズを指定します。
    //! @param[in]  pDevice デバイスです。
    //! @param[in]  pPrefix プリフィックスです。
    //! @param[in]  pName テクスチャの名前を指定します。
    //!
    //! @return 見つかった TextureInfo を返します。
    //!
    virtual const TextureInfo* AcquireDynamicGenerateTexture(char* pNewNameCopy, int newNameCopyBufferSize, nn::gfx::Device* pDevice, const char* pPrefix, const char* pName);

    //! @brief シェーダ情報を取得します。
    //!
    //! @details
    //! name で指定したシェーダリソース名と関連付けられた
    //! シェーダ情報を取得します。
    //!
    //! シェーダを所有していない場合にはロードを試みます。
    //!
    //! 実装には LoadShader() を利用することができます。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] pName シェーダリソースの名前です。
    //!
    //! @return シェーダ情報を返します。
    //! シェーダを所有していない場合にはロードを試みます。
    //! ロードできなかった場合にはシェーダオブジェクトには無効な値
    //! が設定されます。
    //!
    //! @sa LoadShader
    //!
    virtual const ShaderInfo* AcquireShader(nn::gfx::Device* pDevice, const char* pName) = 0;

    //! @brief アーカイブシェーダ情報を取得します。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] signature シグネチャ
    //! @param[in] keyLength 検索キーの長さ
    //! @param[in] pKey 検索キー
    //!
    //! @return シェーダ情報を返します。
    //!
    virtual const ShaderInfo* AcquireArchiveShader(nn::gfx::Device* pDevice, uint32_t signature, size_t keyLength, const uint32_t* pKey) = 0;

    //@}

protected:
    //! @brief テクスチャをロードします。
    //!
    //! @details
    //! テクスチャが初めて参照されたときに AcquireTexture() から呼び出されます。
    //!
    //! FindResourceByName() でテクスチャリソースを取得し、
    //! nn::ui2d::LoadTexture() でデバイスにロードします。
    //!
    //! @param[out] textureInfo ロードしたテクスチャの情報を格納します。
    //! @param[in] pDevice      デバイスです。
    //! @param[in] pName         テクスチャリソースの名前です。
    //!
    //! @return ロードが成功した場合は true が返ります。
    //!
    //! @sa AcquireTexture
    //! @sa nn::ui2d::LoadTexture
    //!
    virtual bool LoadTexture(ResourceTextureInfo* textureInfo, nn::gfx::Device* pDevice, const char* pName) = 0;

    //! @brief リソースフォントを生成します。
    //!
    //! @details
    //! フォントが初めて参照されたときに AcquireFont() から呼び出されます。
    //!
    //! font::ResFont を生成し FindResourceByName() で取得した
    //! フォントリソースを設定します。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] pName フォントリソースの名前です。
    //!
    //! @return font::ResFont へのポインタを返します。
    //!
    //! @sa AcquireFont
    //!
    virtual nn::font::Font* LoadFont(nn::gfx::Device* pDevice, const char* pName);

    //! @brief シェーダをロードします。
    //!
    //! @details
    //! シェーダが初めて参照されたときに AcquireShader() から呼び出されます。
    //!
    //! @param[out] pShaderInfo ロードしたシェーダの情報を格納します。
    //! @param[in] pDevice      デバイスです。
    //! @param[in] pName         シェーダリソースの名前です。
    //!
    //! @return ロードが成功した場合は true が返ります。
    //!
    //! @sa AcquireShader
    //!
    virtual bool LoadShader(ShaderInfo* pShaderInfo, nn::gfx::Device* pDevice, const char* pName) = 0;

    //! @brief アーカイブシェーダをロードします。
    //!
    //! @param[out] pShaderInfo ロードしたシェーダの情報を格納します。
    //! @param[in] pDevice      デバイスです。
    //! @param[in] signature シグネチャ
    //! @param[in] keyLength 検索キーの長さ
    //! @param[in] pKey 検索キー
    //!
    //! @return ロードが成功した場合は true が返ります。
    //!
    virtual bool LoadArchiveShader(ShaderInfo* pShaderInfo, nn::gfx::Device* pDevice, uint32_t signature, size_t keyLength, const uint32_t* pKey) = 0;
};

} // namespace nn::ui2d
} // namespace nn
