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

#include <nn/gfx.h>
#include <nn/util/util_AccessorBase.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/detail/g3d_Inlines.h>
#include <cstring>

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_SHADOW

namespace nn { namespace g3d {

//! @brief テクスチャービューとそのテクスチャービューを登録したディスクリプタプールを表す型です。
class TextureRef
{
public:
    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    TextureRef() : pTextureView(NULL)
    {
    }
    //! @brief コンストラクタです。
    //!
    //! @param[in] pTextureView テクスチャービューへのポインター。
    //! @param[in] descriptorSlot テクスチャービューを登録したディスクリプタスロットへの参照。
    //!
    TextureRef(const nn::gfx::TextureView* pTextureView, const nn::gfx::DescriptorSlot& descriptorSlot)
    {
        this->pTextureView = pTextureView;
        this->descriptorSlot = descriptorSlot;
    }

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief テクスチャービューを取得します。
    //!
    //! @return テクスチャービューのポインターを返します。
    //!
    const nn::gfx::TextureView* GetTextureView() const NN_NOEXCEPT
    {
        return pTextureView;
    }

    //! @brief テクスチャービューを設定します。
    //!
    //! @param[in] pTextureView テクスチャービューへのポインター。
    //!
    void SetTextureView(const nn::gfx::TextureView* pTextureView) NN_NOEXCEPT
    {
        this->pTextureView = pTextureView;
    }

    //! @brief ディスクリプタスロットを取得します。
    //!
    //! @return ディスクリプタスロットを返します。
    //!
    nn::gfx::DescriptorSlot GetDescriptorSlot() const NN_NOEXCEPT
    {
        return descriptorSlot;
    }

    //! @brief ディスクリプタスロットを設定します。
    //!
    //! @param[in] descriptorSlot ディスクリプタスロットへの参照。
    //!
    void SetDescriptorSlot(nn::gfx::DescriptorSlot& descriptorSlot) NN_NOEXCEPT
    {
        this->descriptorSlot = descriptorSlot;
    }

    //! @brief nn::g3d::TextureRef が有効な値かどうかを取得します。
    //!
    //! @return 有効な場合、true、無効な場合、false を返します。
    //!
    bool IsValid() const NN_NOEXCEPT
    {
        return (pTextureView != NULL && descriptorSlot.IsValid());
    }

    //@}
private:
    const nn::gfx::TextureView* pTextureView;
    nn::gfx::DescriptorSlot descriptorSlot;
};

//! @brief サンプラーとそのサンプラーを登録したディスクリプタプールを表す型です。
class SamplerRef
{
public:
    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    SamplerRef() : pSampler(NULL)
    {
    }
    //! @brief コンストラクタです。
    //!
    //! @param[in] pSampler サンプラーへのポインター。
    //! @param[in] descriptorSlot サンプラーを登録したディスクリプタスロットへの参照。
    //!
    SamplerRef(const nn::gfx::Sampler* pSampler, const nn::gfx::DescriptorSlot& descriptorSlot)
    {
        this->pSampler = pSampler;
        this->descriptorSlot = descriptorSlot;
    }
    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief サンプラーを取得します。
    //!
    //! @return サンプラーのポインターを返します。
    //!
    const nn::gfx::Sampler* GetSampler() const NN_NOEXCEPT
    {
        return pSampler;
    }

    //! @brief サンプラーを設定します。
    //!
    //! @param[in] pSampler サンプラーへのポインター。
    //!
    void SetSampler(const nn::gfx::Sampler* pSampler) NN_NOEXCEPT
    {
        this->pSampler = pSampler;
    }

    //! @brief ディスクリプタスロットを取得します。
    //!
    //! @return ディスクリプタスロットを返します。
    //!
    nn::gfx::DescriptorSlot GetDescriptorSlot() const NN_NOEXCEPT
    {
        return descriptorSlot;
    }

    //! @brief ディスクリプタスロットを設定します。
    //!
    //! @param[in] descriptorSlot ディスクリプタスロットへの参照。
    //!
    void SetDescriptorSlot(nn::gfx::DescriptorSlot& descriptorSlot) NN_NOEXCEPT
    {
        this->descriptorSlot = descriptorSlot;
    }

    //! @brief nn::g3d::SamplerRef が有効な値かどうかを取得します。
    //!
    //! @return 有効な場合、true、無効な場合、false を返します。
    //!
    bool IsValid() const NN_NOEXCEPT
    {
        return (pSampler != NULL && descriptorSlot.IsValid());
    }

    //@}
private:
    const nn::gfx::Sampler* pSampler;
    nn::gfx::DescriptorSlot descriptorSlot;
};

#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS
#endif

//! @brief テクスチャーバインド時に呼ばれるコールバックの型
//!
//! @param[in] name テクスチャー名へのポインター。
//! @param[in] pUserData ユーザーデータへのポインター。
//!
//! @return nn::gfx::TextureView へのポインター。
//!
typedef TextureRef (*TextureBindCallback)(const char* name, void* pUserData);

//! @brief 外部ファイルの構造体です。
struct ResExternalFileData
{
    nn::util::BinPtr pData; //!< 外部ファイルへのポインター。
    uint32_t size; //!< サイズ。
    NN_PADDING4;
};

//! @brief 外部ファイルのリソースです。
//!
//! 外部ファイルへの参照を持つクラスです。
//!
class ResExternalFile : public nn::util::AccessorBase< ResExternalFileData >
{
    NN_DISALLOW_COPY(ResExternalFile);

public:
    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief 外部ファイルのサイズを取得します。
    //!
    //! @return 外部ファイルのサイズ。
    //!
    size_t GetSize() const NN_NOEXCEPT
    {
        return ToData().size;
    }

    //! @brief 外部ファイルへのポインターを取得します。
    //!
    //! @return 外部ファイルへのポインター。
    //!
    void* GetData() NN_NOEXCEPT
    {
        return ToData().pData.Get();
    }

    //! @brief 外部ファイルへのポインターを取得します。
    //!
    //! @return 外部ファイルへのポインター。
    //!
    const void* GetData() const NN_NOEXCEPT
    {
        return ToData().pData.Get();
    }

    //@}
};

}} // namespace nn::g3d

NN_PRAGMA_POP_WARNINGS
