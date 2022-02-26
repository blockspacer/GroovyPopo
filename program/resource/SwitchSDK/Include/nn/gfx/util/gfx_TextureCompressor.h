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

#include <nn/gfx/util/detail/gfx_TextureCompressor-common.h>

namespace nn {
namespace gfx {
namespace util {

//-----------------------------------------------------
//! @brief テクスチャの圧縮先の情報クラスです。
//-----------------------------------------------------
class TextureCompressorTargetInfo
{
    NN_DISALLOW_COPY(TextureCompressorTargetInfo);
public:

    /**
    * @brief コンストラクタです。
    */
    TextureCompressorTargetInfo() NN_NOEXCEPT;

    /**
    * @brief デストラクタです。
    */
    ~TextureCompressorTargetInfo() NN_NOEXCEPT;

    /**
    * @brief        圧縮先のテクスチャの情報を設定します。
    *
    * @param[in]    textureInfo	    圧縮先のテクスチャを初期化する際に使用した nn::gfx::TextureInfo です。
    * @param[in]    pTexture	    圧縮先のテクスチャのポインタです。
    *
    * @pre
    * - pTexture != NULL
    * - nn::gfx::IsInitialized(*pTexture)
    */
    void SetTexture(const nn::gfx::Texture::InfoType& textureInfo, nn::gfx::Texture* pTexture) NN_NOEXCEPT;

    /**
    * @brief    圧縮先のテクスチャの情報を取得します。
    * @return   圧縮先のテクスチャの情報を返します。
    * @pre      SetTexture() で値が設定されている。
    */
    nn::gfx::Texture::InfoType GetTextureInfo() const NN_NOEXCEPT;

    /**
    * @brief    圧縮先のテクスチャのポインタを取得します。
    * @return   圧縮先のテクスチャのポインタを返します。
    * @details  SetTexture() で値が設定されていない場合は、 NULL を返します。
    */
    nn::gfx::Texture* GetTexture() const NN_NOEXCEPT;

private:

    nn::gfx::Texture*           m_pTexture;
    nn::gfx::Texture::InfoType  m_TextureInfo;

};


//-------------------------------------------------
//! @brief テクスチャの圧縮を行うクラスです。
//-------------------------------------------------
class TextureCompressor
{
    NN_DISALLOW_COPY(TextureCompressor);

public:

    static const int SupportedFormatCount = 10;

public:

    /**
    * @brief コンストラクタです。
    */
    TextureCompressor() NN_NOEXCEPT;

    /**
    * @brief    デストラクタです。
    * @pre      !IsInitialized()
    */
    ~TextureCompressor() NN_NOEXCEPT;

    /**
    * @brief        テクスチャ圧縮に必要なオブジェクトの初期化を行います。
    *
    * @param[in]    pDevice	デバイスのポインタです。
    *
    * @pre
    * - !IsInitialized()
    * - pDevice != NULL
    * - nn::gfx::IsInitialized(*pDevice)
    *
    * @post
    * - IsInitialized()
    */
    void Initialize(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    /**
    * @brief    テクスチャ圧縮に必要なオブジェクトを破棄します。
    * @pre      IsInitialized()
    * @post     !IsInitialized()
    */
    void Finalize() NN_NOEXCEPT;

    /**
    * @brief    初期化済みかどうかを取得します。
    * @return   初期化済みかどうかを返します。
    */
    bool IsInitialized() const NN_NOEXCEPT;

    /**
    * @brief        テクスチャ圧縮に使用するシェーダを登録します。
    *
    * @param[in]    targetFormat   シェーダを登録する圧縮フォーマットです。
    * @param[in]    pShader        フォーマットに対応する圧縮処理を行うシェーダのポインタです。
    *
    * @pre
    * - IsInitialized()
    * - IsSupportedFormat(targetFormat)
    * - pShader != NULL
    * - nn::gfx::IsInitialized(*pShader)
    *
    * @details
    * - 圧縮フォーマットごとにシェーダを登録しておく必要があります。
    * - 同じフォーマットに対して、別のシェーダのポインタを引数として渡した場合、上書きされます。
    */
    void RegisterCompressionShader(nn::gfx::ImageFormat targetFormat,const nn::gfx::Shader* pShader) NN_NOEXCEPT;

    /**
    * @brief        シェーダの登録を解除します。
    *
    * @param[in]    targetFormat    登録を解除するシェーダに対応するフォーマットです。
    *
    * @pre
    * - IsInitialized()
    * - IsSupportedFormat(targetFormat)
    *
    * @details
    * - この関数の呼び出し以前に MakeCommand() で作成されたコマンドはシェーダを参照している点に注意してください。
    * - Finalize() によって破棄された後に、この関数を呼び出す必要はありません。
    */
    void UnregisterCompressionShader(nn::gfx::ImageFormat targetFormat) NN_NOEXCEPT;

    /**
    * @brief        指定した圧縮フォーマットがサポートされているかを取得します。
    * @param[in]    format  サポートされているか確認を行うフォーマットです。
    * @return       指定したフォーマットをサポートしているかを返します。
    */
    static bool IsSupportedFormat(nn::gfx::ImageFormat format) NN_NOEXCEPT;

    /**
    * @brief        テクスチャを圧縮するコマンドを作成します。
    *
    * @param[in]    pCommandBuffer              圧縮処理を書き込むコマンドバッファのポインタです。
    * @param[in]    srcTextureDescriptorSlot    圧縮元のテクスチャのデスクリプタスロットです。
    * @param[in]    samplerDescriptorSlot       圧縮元のテクスチャの値を読み込む際に使用するサンプラが登録されているデスクリプタスロットです。
    * @param[in]    targetTextureInfo			圧縮先のテクスチャの情報です。
    *
    * @pre
    * - IsInitialized()
    * - pCommandBuffer != NULL
    * - nn::gfx::IsInitialized(*pCommnadBuffer)
    * - IsSupportedFormat(destFormat)
    * - destTextureInfo は SetTextureInfo() と SetTextureStorage() で圧縮先の情報が格納されていなければなりません。
    * - targetTextureInfo に設定されているミップマップの数は 15 以下の値でなければなりません。
    * - targetTextureInfo に設定されている配列の長さは 65535 以下の値でなければなりません。
    * - 圧縮フォーマットに対応したシェーダが登録されていなければなりません。
    *
    * @post
    * - テクスチャ圧縮を行うコマンドが pCommandBuffer で渡されたコマンドバッファに記録された状態になります。
    *
    * @details
    * - 圧縮するコマンドを作成する際に登録したシェーダは、コマンドが実行されるまで破棄してはいけません。
    * - 圧縮先のテクスチャは、コマンドが実行されるまで破棄してはいけません。
    */
    void MakeCommand(
        nn::gfx::CommandBuffer* pCommandBuffer,
        const nn::gfx::DescriptorSlot& srcTextureDescriptorSlot,
        const nn::gfx::DescriptorSlot& samplerDescriptorSlot,
        const TextureCompressorTargetInfo& targetTextureInfo
        ) const NN_NOEXCEPT;

private:
    const nn::gfx::Shader* GetShader(nn::gfx::ImageFormat imageFormat) const NN_NOEXCEPT;

private:
    detail::TextureCompressorCommonState m_CommonState;
    const nn::gfx::Shader* m_pShaders[SupportedFormatCount];
};


}
}
}
