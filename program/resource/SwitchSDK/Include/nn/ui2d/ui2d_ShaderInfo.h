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

namespace nn
{
namespace ui2d
{

//---------------------------------------------------------------------------
//! @brief シェーダ情報を管理するためのクラスです。
//!
//---------------------------------------------------------------------------
class ShaderInfo
{
    NN_DISALLOW_COPY(ShaderInfo);
public:
    //! @brief コンストラクタです。
    //!
    ShaderInfo()
        : m_pResShaderFile(NULL)
        , m_Flags(0)
        , m_pVertexStates(NULL)
        , m_pVertexShaderSlots(NULL)
        , m_pGeometryShaderSlots(NULL)
        , m_pPixelShaderSlots(NULL)
        , m_pTextureSlots(NULL)
        , m_pVariationTable(NULL)
    {
        SetShaderCodeType(nn::gfx::ShaderCodeType_Source);
        SetTextureSlotCount(0);
    }

    //! @brief 初期化します。
    //!
    //! @param[in] pDevice          デバイスです。
    //! @param[in] pShader          シェーダバイナリ。NULLを渡すと内部ソースファイルでシェーダを初期化します。
    //!
    void Initialize(nn::gfx::Device* pDevice, void* pShader);

    //! @brief バリエーションテーブルと共に初期化します。
    //!
    //! @param[in] pDevice          デバイスです。
    //! @param[in] pShader          シェーダバイナリ。NULLを渡すと内部ソースファイルでシェーダを初期化します。
    //! @param[in] pVariationTable  バリエーションテーブル。NULL を渡すとシェーダにバリエーションテーブルの情報は埋め込まれません。
    //!
    void InitializeWithVariationTable(nn::gfx::Device* pDevice, void* pShader, const void* pVariationTable)
    {
        InitializeWithVariationTable(pDevice, pShader, pVariationTable, NULL, 0, 0);
    }

    //! @brief バリエーションテーブルと共に初期化します。
    //!
    //! @param[in] pDevice          デバイスです。
    //! @param[in] pShader          シェーダバイナリ。NULLを渡すと内部ソースファイルでシェーダを初期化します。
    //! @param[in] pVariationTable  バリエーションテーブル。NULL を渡すとシェーダにバリエーションテーブルの情報は埋め込まれません。
    //! @param[in] pMemoryPool      メモリプールです。バイナリをメモリプール内に配置した場合にそのメモリプールを指定します。未配置の場合は nullptr を指定してください。
    //! @param[in] memoryPoolOffset 配置先メモリプールの先頭からリソース配置位置へのオフセットです。
    //! @param[in] memoryPoolSize   配置先メモリプールのサイズです。
    //! @param[in] textureSlotCount テクスチャスロットの数です。
    //!
    void InitializeWithVariationTable(nn::gfx::Device* pDevice, void* pShader, const void* pVariationTable, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize, int textureSlotCount);

    //! @brief バリエーションテーブルと共に初期化します。
    //!
    //! @param[in] pDevice          デバイスです。
    //! @param[in] pShader          シェーダバイナリ。NULLを渡すと内部ソースファイルでシェーダを初期化します。
    //! @param[in] pVariationTable  バリエーションテーブル。NULL を渡すとシェーダにバリエーションテーブルの情報は埋め込まれません。
    //! @param[in] pMemoryPool      メモリプールです。バイナリをメモリプール内に配置した場合にそのメモリプールを指定します。未配置の場合は nullptr を指定してください。
    //! @param[in] memoryPoolOffset 配置先メモリプールの先頭からリソース配置位置へのオフセットです。
    //! @param[in] memoryPoolSize   配置先メモリプールのサイズです。
    //!
    void InitializeWithVariationTable(nn::gfx::Device* pDevice, void* pShader, const void* pVariationTable, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize)
    {
        InitializeWithVariationTable(pDevice, pShader, pVariationTable, pMemoryPool, memoryPoolOffset, memoryPoolSize, TexMapMax);
    }

    //! @brief リソースを開放します。
    //!
    //! @param[in] pDevice          デバイスです。
    //! @param[in] skipFinalizingResShader ResShader の解放をスキップする場合は true を指定します。
    //!
    void Finalize(nn::gfx::Device* pDevice, bool skipFinalizingResShader);

    //! @brief シェーダをセットします。
    //!
    //! @param[in] commandBuffer   コマンドバッファです。
    //! @param[in] variation シェーダのバリエーション番号です。
    //!
    void SetShader(nn::gfx::CommandBuffer& commandBuffer, int variation) const;

    //! @brief 頂点シェーダのユニフォームブロックスロットを取得します。
    //!
    //! @param[in] variation 頂点シェーダのスロット番号です。
    //!
    //! @return 頂点シェーダのユニフォームブロックスロットです。
    //!
    int GetVertexShaderSlot(int variation) const { return m_pVertexShaderSlots[variation]; }

    //! @brief ジオメトリシェーダのユニフォームブロックスロットを取得します。
    //!
    //! @param[in] variation ジオメトリシェーダのスロット番号です。
    //!
    //! @return ジオメトリシェーダのユニフォームブロックスロットです。
    //!
    int GetGeometryShaderSlot(int variation) const { return m_pGeometryShaderSlots[variation]; }

    //! @brief ピクセルシェーダのユニフォームブロックスロットを取得します。
    //!
    //! @param[in] variation ピクセルシェーダのスロット番号です。
    //!
    //! @return ピクセルシェーダのユニフォームブロックスロットです。
    //!
    int GetPixelShaderSlot(int variation) const { return m_pPixelShaderSlots[variation]; }

    //! @brief テクスチャスロットを取得します。
    //!
    //! @param[in] variation ピクセルシェーダのスロット番号です。
    //! @param[in] texture テクスチャ番号です。
    //!
    //! @return テクスチャスロットです。
    //!
    int GetTextureSlot(int variation, int texture) const { return *(m_pTextureSlots + variation * GetTextureSlotCount() + texture); }

    //! @brief バリエーションテーブルを取得します。
    //!
    //! @return バリエーションテーブルです。
    //!
    const void* GetVariationTable() const { return m_pVariationTable; }

    //! @brief  バリエーション数を取得します。
    //!
    //! @return バリエーション数
    //!
    int GetVariationCount() const;

    //  指定したバリエーションの頂点シェーダーを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  variation   バリエーションインデックス
    //!
    //! @return 指定したバリエーションの頂点シェーダー
    const nn::gfx::Shader* GetVertexShader(int variation) const;

    //  指定したバリエーションのジオメトリシェーダーを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  variation   バリエーションインデックス
    //!
    //! @return 指定したバリエーションのジオメトリシェーダー
    const nn::gfx::Shader* GetGeometryShader(int variation) const;

    //  指定したバリエーションのピクセルシェーダーを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  variation   バリエーションインデックス
    //!
    //! @return 指定したバリエーションのピクセルシェーダー
    const nn::gfx::Shader* GetPixelShader(int variation) const;

    //! @brief テクスチャスロット数を取得します。
    //!
    //! @return テクスチャスロット数です。
    //!
    int GetTextureSlotCount() const;

private:
    enum Bits
    {
        BitsShaderCodeType  = 3,
        BitsTextureSlotCount= 4,
    };

    enum Pos
    {
        PosShaderCodeType   = 0,
        PosTextureSlotCount = 4,

    };

    void InitializeVertexStates(nn::gfx::Device* pDevice);
    void SetShaderCodeType(nn::gfx::ShaderCodeType code);
    nn::gfx::ShaderCodeType GetShaderCodeType() const;
    void SetTextureSlotCount(int textureSlotCount);

    nn::gfx::ResShaderFile* m_pResShaderFile;
    uint32_t m_Flags;

    nn::gfx::VertexState* m_pVertexStates;
    int* m_pVertexShaderSlots;
    int* m_pGeometryShaderSlots;
    int* m_pPixelShaderSlots;
    int* m_pTextureSlots;

    const void* m_pVariationTable; // アーカイブシェーダ用のバリエーションテーブル
};

NN_STATIC_ASSERT(nn::gfx::ShaderCodeType::ShaderCodeType_End == 4);

} // namespace nn::ui2d
} // namespace nn
