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
#include <nn/ui2d/ui2d_FontContainer.h>
#include <nn/ui2d/ui2d_ResourceAccessor.h>
#include <nn/ui2d/ui2d_TextureContainer.h>
#include <nn/ui2d/ui2d_ShaderContainer.h>
#include <cstddef>
#include <nn/ui2d/ui2d_ArcExtractor.h>

namespace nn
{
namespace ui2d
{

//---------------------------------------------------------------------------
//! @brief アーカイブリソースと各リソースのインスタンスを結びつけるクラスです。
//!
//---------------------------------------------------------------------------
class ArchiveHandle
{
public:
    //! パス名の最大長です。
    static const int RootPathLengthMax = 64;

    //! @brief コンストラクタです。
    //!
    ArchiveHandle();

    //! @brief デストラクタです。
    //!
    virtual ~ArchiveHandle();

    //! @brief アーカイブおよび各種リソースを初期化します。
    //!
    //! @details
    //! アーカイブのフォーマットが正しくなければ失敗します。
    //!
    //! pArchiveStartで与えるアーカイブデータは、 nn::ui2d::ArchiveResourceAlignment バイトで
    //! アライメントしてください。
    //!
    //! 詳しくは、 nn::ui2d::ArcResourceAccessor::Attach メソッドの説明をご参照ください。
    //!
    //! @param[in] pArchiveStart             アーカイブの先頭アドレスです。
    //! @param[in] pResourceRootDirectory    リソースのルートディレクトリ名です。
    //!
    //! @return アーカイブの設定が成功した場合は true を、失敗した場合は false を返します。
    //!
    //! @sa ArcResourceAccessor::Attach
    //!
    bool Initialize(void* pArchiveStart, const char* pResourceRootDirectory)
    {
        return Initialize(pArchiveStart, pResourceRootDirectory, NULL, 0, 0);
    }

    //! @brief アーカイブおよび各種リソースを初期化します。
    //!
    //! @details
    //! アーカイブのフォーマットが正しくなければ失敗します。
    //!
    //! pArchiveStartで与えるアーカイブデータは、 nn::ui2d::ArchiveResourceAlignment バイトで
    //! アライメントしてください。
    //!
    //! 詳しくは、 nn::ui2d::ArcResourceAccessor::Attach メソッドの説明をご参照ください。
    //!
    //! @param[in] pArchiveStart             アーカイブの先頭アドレスです。
    //! @param[in] pResourceRootDirectory    リソースのルートディレクトリ名です。
    //! @param[in]  pMemoryPool メモリプールです。バイナリをメモリプール内に配置した場合にそのメモリプールを指定します。未配置の場合は nullptr を指定してください。
    //! @param[in]  memoryPoolOffset 配置先メモリプールの先頭からリソース配置位置へのオフセットです。
    //! @param[in]  memoryPoolSize 配置先メモリプールのサイズです。
    //!
    //! @return アーカイブの設定が成功した場合は true を、失敗した場合は false を返します。
    //!
    //! @sa ArcResourceAccessor::Attach
    //!
    bool Initialize(void* pArchiveStart, const char* pResourceRootDirectory,
        nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize
    );

    //! @brief アーカイブおよび各種リソースを解放します。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    void Finalize(nn::gfx::Device* pDevice);

    //! @brief アーカイブリソースを取得します。
    //!
    //! @return アーカイブリソースです。
    //!
    ArcExtractor* GetArcExtractor();

    //! @brief アーカイブリソースを取得します。
    //!
    //! @return アーカイブリソースです。
    //!
    const ArcExtractor* GetArcExtractor() const;

    //! @brief フォントリストを取得します。
    //!
    //! @return フォントリストです。
    //!
    FontContainer* GetFontList();

    //! @brief テクスチャリストを取得します。
    //!
    //! @return テクスチャリストです。
    //!
    TextureContainer* GetTextureList();

    //! @brief シェーダリストを取得します。
    //!
    //! @return シェーダリストです。
    //!
    ShaderContainer* GetShaderList();

    //! @brief テクスチャを読み込みます。
    //!
    //! @param[in] pTexInfo 読み込んだテクスチャを格納するテクスチャ情報です。
    //! @param[in] pDevice デバイスです。
    //! @param[in] pName 読み込むテクスチャの名前です。
    //!
    //! @return 読み込みが成功した場合は true を、失敗した場合は false を返します。
    //!
    bool LoadTexture(ResourceTextureInfo* pTexInfo, nn::gfx::Device* pDevice, const char *pName);

    //! @brief アーカイブ中のすべてのテクスチャを読み込みます。
    //!
    //! @param[in] pDevice デバイスです。
    void LoadTextureAll(nn::gfx::Device* pDevice);

    //! @brief フォントを読み込みます。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] pName 読み込むテクスチャの名前です。
    //!
    //! @return 読み込んだフォントを返します。失敗した場合は NULL を返します。
    //!
    nn::font::Font* LoadFont(nn::gfx::Device* pDevice, const char *pName);

    //! @brief シェーダを読み込みます。
    //!
    //! @param[in] pShaderInfo 読み込んだテクスチャを格納するテクスチャ情報です。
    //! @param[in] pDevice デバイスです。
    //! @param[in] pName 読み込むテクスチャの名前です。
    //!
    //! @return 読み込みが成功した場合は true を、失敗した場合は false を返します。
    //!
    bool LoadShader(ShaderInfo* pShaderInfo, nn::gfx::Device* pDevice, const char *pName);

    //! @brief アーカイブシェーダを読み込みます。
    //!
    //! @param[in] pShaderInfo 読み込んだテクスチャを格納するテクスチャ情報です。
    //! @param[in] pDevice デバイスです。
    //! @param[in] signature シグネチャ
    //! @param[in] keyLength 検索キーの長さ
    //! @param[in] pKey 検索キー
    //!
    //! @return 読み込みが成功した場合は true を、失敗した場合は false を返します。
    //!
    bool LoadArchiveShader(ShaderInfo* pShaderInfo, nn::gfx::Device* pDevice, uint32_t signature, size_t keyLength, const uint32_t* pKey);

    //! @brief アーカイブ中のすべてのシェーダーを読み込みます。
    //!
    //! @param[in] pDevice デバイスです。
    void LoadShaderAll(nn::gfx::Device* pDevice);

    //! @brief フォントを登録します。
    //!
    //! @param[in] pName フォント名です。
    //! @param[in] pFont フォントです。
    //!
    void RegisterFont(const char* pName, nn::font::Font* pFont);

    //! @brief テクスチャを登録します。
    //!
    //! @param[in] pName テクスチャ名です。
    //! @return テクスチャ情報です。
    //!
    ResourceTextureInfo* RegisterTexture(const char* pName);

    //! @brief シェーダリソースを登録します。
    //!
    //! @param[in] pName シェーダ名です。
    //! @return シェーダ情報です。
    //!
    ShaderInfo* RegisterShader(const char* pName);

    //! @brief すべてのリソースの登録を解除します。
    //!
    void UnregisterAll();

    //! @brief テクスチャービューをディスクリプタプールに登録します。
    //!
    //! @param[in] pRegisterTextureViewSlot     ディスクリプタスロット確保関数です。
    //! @param[in] pUserData     関数に渡されるユーザーデーターです。
    //!
    void RegisterTextureViewToDescriptorPool(RegisterTextureViewSlot pRegisterTextureViewSlot, void* pUserData);

    //! @brief テクスチャービューをディスクリプタプールから解放します。
    //!
    //! @param[in] pUnregisterTextureViewSlot     ディスクリプタスロット解放関数です。
    //! @param[in] pUserData     関数に渡されるユーザーデーターです。
    //!
    void UnregisterTextureViewFromDescriptorPool(UnregisterTextureViewSlot pUnregisterTextureViewSlot, void* pUserData);

    //! @brief アーカイブの先頭アドレスを取得します。
    //!
    //! @return 設定されているアーカイブの先頭アドレスを返します。
    //!
    const void* GetArchiveDataStart() const;

    //! @brief リソースのルートディレクトリ名を取得します。
    //!
    //! @return リソースのルートディレクトリ名を返します。
    //!
    const char* GetResRootDir() const;

private:

    // 必要なら、Bntx を初期化します。
    void InitializeBntxIfNeeded(nn::gfx::Device* pDevice);

    ArcExtractor m_ArcExtractor;
    char m_ResRootDir[RootPathLengthMax];
    FontContainer m_FontList;
    TextureContainer m_TextureList;
    ShaderContainer m_ShaderList;
    nn::gfx::ResTextureFile* m_pResTextureFile;
    void* m_pResArchiveShaderFile;
    void* m_pResArchiveShaderVariationTableFile;

    void*                m_pArchiveBinaryTop;
    nn::gfx::MemoryPool* m_pMemoryPool;
    ptrdiff_t            m_MemoryPoolOffset;
    size_t               m_MemoryPoolSize;
};

//---------------------------------------------------------------------------
//! @brief 検索可能なリソースアクセサーのインタフェースです。
//!
//---------------------------------------------------------------------------
class IFindableResourceAccessor
{
public:
    virtual const char* FindFile(nn::ui2d::ResType resType, const char* pPrevFileName) = 0;
    virtual void* FindData(size_t*  pSize, ResType resType, const char* pName) = 0;
};

//---------------------------------------------------------------------------
//! @brief アーカイブリソースにアクセスするためのクラスです。
//!
//---------------------------------------------------------------------------
class ArcResourceAccessor : public ResourceAccessor
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(ResourceAccessor);

    //! パス名の最大長です。
    static const int RootPathLengthMax = ArchiveHandle::RootPathLengthMax;

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    ArcResourceAccessor();

    //! @brief デストラクタです。
    //!
    virtual ~ArcResourceAccessor();

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief 全リソースを解放します。
    //!
    //! @details
    //! デストラクタが呼ばれる前に呼び出してください。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    virtual void Finalize(nn::gfx::Device* pDevice) NN_OVERRIDE;

    //! @brief アーカイブデータを登録します。
    //!
    //! @details
    //! pArchiveStartで与えるアーカイブデータは、 nn::ui2d::ArchiveResourceAlignment バイトで
    //! アライメントしてください。
    //!
    //! これは、アーカイブデータ内に含まれているテクスチャデータを、別の場所にコピーせずに
    //! そのままGPUで扱うことができるようにするためです。アーカイブ内のファイルのデータ
    //! アラインメントは、レイアウトアーカイバが自動的に行います。
    //!
    //! nn::ui2d::ArchiveResourceAlignment は、要求される最も大きいアライメントに合わせて
    //! 定義しています。
    //!
    //! @param[in] pArchiveStart             アーカイブデータへのポインタです。
    //! @param[in] pResourceRootDirectory    リソース検索のルートとなるディレクトリ名です。
    //!
    //! @return 登録に成功したら true、失敗したら falseを返します。
    //!
    bool Attach(void* pArchiveStart,const char* pResourceRootDirectory)
    {
        return Attach(pArchiveStart, pResourceRootDirectory, NULL, 0, 0);
    }

    //! @brief アーカイブデータを登録します。
    //!
    //! @details
    //! pArchiveStartで与えるアーカイブデータは、 nn::ui2d::ArchiveResourceAlignment バイトで
    //! アライメントしてください。
    //!
    //! これは、アーカイブデータ内に含まれているテクスチャデータを、別の場所にコピーせずに
    //! そのままGPUで扱うことができるようにするためです。アーカイブ内のファイルのデータ
    //! アラインメントは、レイアウトアーカイバが自動的に行います。
    //!
    //! nn::ui2d::ArchiveResourceAlignment は、要求される最も大きいアライメントに合わせて
    //! 定義しています。
    //!
    //! @param[in] pArchiveStart             アーカイブデータへのポインタです。
    //! @param[in] pResourceRootDirectory    リソース検索のルートとなるディレクトリ名です。
    //! @param[in]  pMemoryPool メモリプールです。バイナリをメモリプール内に配置した場合にそのメモリプールを指定します。未配置の場合は nullptr を指定してください。
    //! @param[in]  memoryPoolOffset 配置先メモリプールの先頭からリソース配置位置へのオフセットです。
    //! @param[in]  memoryPoolSize 配置先メモリプールのサイズです。
    //!
    //! @return 登録に成功したら true、失敗したら falseを返します。
    //!
    bool Attach(
        void* pArchiveStart,
        const char* pResourceRootDirectory,
        nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize
    );

    //! @brief 登録したアーカイブデータを解除します。
    //!
    //! @details
    //! レイアウトとリソースは1対1ではなく、複数のレイアウトから同じリソースを共有することができるため、
    //! 本関数を呼ぶことで、Finalize メソッドを呼んでもリソースを解放しないようにできます。
    //!
    //! @return 登録されていたアーカイブデータへのポインタを返します。
    //!
    void* Detach();

    virtual void* FindResourceByName(
        size_t* pSize,
        ResType resType,
        const char* pName
    ) NN_OVERRIDE;

    virtual const void* FindResourceByName(size_t * pSize, ResType resType, const char* pName) const NN_OVERRIDE
    {
        return const_cast<ArcResourceAccessor*>(this)->FindResourceByName(pSize, resType, pName);
    }

    virtual void* FindResourceByName(ResType resType, const char* pName) NN_OVERRIDE
    {
        return FindResourceByName(NULL, resType, pName);
    }

    virtual const void* FindResourceByName(ResType resType, const char* pName) const NN_OVERRIDE
    {
        return FindResourceByName(NULL, resType, pName);
    }

    //! @brief 指定した種類のリソースをすべて取得します。
    //!
    //! @param[in] resType   リソースの種類です。
    //! @param[in] pCallback リソースが見つかるたびに呼ばれるコールバック関数です。
    //! @param[in] pParam コールバックに与えるパラメータです。
    //!
    virtual void FindResourceByType(
        ResType resType,
        FindResourceByTypeCallback pCallback,
        void* pParam
    ) const NN_OVERRIDE;

    //! @brief アクセサにアーカイブが設定されているかどうかを判定します。
    //!
    //! @return すでにアーカイブが設定されている場合は true を返します。
    //!
    bool IsAttached() const
    {
        return m_ArcBuf != 0;
    }

    //! @brief アーカイブの先頭アドレスを取得します。
    //!
    //! @return
    //! 設定されているアーカイブの先頭アドレスを返します。
    //! アーカイブが設定されていない場合は NULL を返します。
    //!
    const void* GetArchiveDataStart() const
    {
        return m_ArcBuf;
    }

    //! @brief アーカイブハンドルを取得します。
    //!
    //! @return
    //! 設定されているアーカイブハンドルを返します。
    //!
    const ArchiveHandle& GetArchiveHandle() const
    {
        return m_ArcHandle;
    }

    //! @brief アーカイブハンドルを取得します。
    //!
    //! @return
    //! 設定されているアーカイブハンドルを返します。
    //!
    ArchiveHandle& GetArchiveHandle()
    {
        return m_ArcHandle;
    }

    //! @brief フォントオブジェクトを登録します。
    //!
    //! @details
    //! フォントオブジェクトをリソースアクセサに設定し、
    //! AcquireFont() の検索対象に含まれるようにします。
    //!
    //! @param[in] pName     フォントリソースの名前です。
    //! @param[in] pFont    フォントオブジェクトへのポインタです。
    //!
    //! @return 登録キーを返します。キーは登録の抹消に使用します。
    //!
    //! @sa UnregisterFont
    //!
    FontKey RegisterFont(const char* pName, nn::font::Font* pFont);

    //! @brief フォントオブジェクトの登録を解除します。
    //!
    //! @param[in] key  フォントの登録時に取得した登録キーを指定します。
    //!
    //! @sa RegisterFont
    //!
    void UnregisterFont(FontKey key);

    //! @brief テクスチャを登録します。
    //!
    //! @details
    //! テクスチャをリソースアクセサに設定し、
    //! AcquireTexture() の検索対象に含まれるようにします。
    //!
    //! テクスチャを登録するには、返り値の TextureInfo* に対して、
    //! テクスチャの設定を行ってください。
    //!
    //! @param[in] pName テクスチャの名前です。
    //!
    //! @return テクスチャ情報です。メモリ確保できない場合には NULL が返ります。
    //!
    //! @sa UnregisterTexture
    //!
    PlacementTextureInfo* RegisterTexture(const char* pName);

    //! @brief テクスチャの登録を解除します。
    //!
    //! @param[in] pTexInfo テクスチャ情報です。
    //!
    //! @sa RegisterTexture
    //!
    void UnregisterTexture(TextureInfo* pTexInfo);

    //! @brief シェーダオブジェクトを登録します。
    //!
    //! @details
    //! シェーダオブジェクトをリソースアクセサに設定し、
    //! AcquireShader() の検索対象に含まれるようにします。
    //!
    //! @param[in] pName     シェーダリソースの名前です。
    //! @param[in] isOwned    シェーダーを所有するかどうかです。false に指定されたものは終了処理の対象になりません。
    //!
    //! @return 登録キーを返します。キーは登録の抹消に使用します。
    //!
    //! @sa UnregisterShader
    //!
    ShaderInfo* RegisterShader(const char* pName, bool isOwned);

    //! @brief シェーダオブジェクトの登録を解除します。
    //!
    //! @param[in] pShaderInfo  シェーダの登録時に取得した登録キーを指定します。
    //!
    //! @sa RegisterShader
    //!
    void UnregisterShader(ShaderInfo* pShaderInfo);

    //! @brief テクスチャ情報を取得します。
    //!
    //! @details
    //! pName で指定したテクスチャリソース名と関連付けられた
    //! テクスチャ情報を取得します。
    //!
    //! テクスチャを所有していない場合には LoadTexture() でロードを試みます。
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
    virtual const TextureInfo* AcquireTexture(nn::gfx::Device* pDevice, const char* pName) NN_OVERRIDE;

    //! @brief フォントを取得します。
    //!
    //! @details
    //! pName で指定したフォントリソース名と関連付けられた
    //! フォントオブジェクトを取得します。
    //!
    //! 指定されたフォントが登録されていなかった場合には
    //! LoadFont() により生成したフォントを登録します。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] pName フォントリソースの名前です。
    //!
    //! @return フォントへのポインタを返します。
    //!
    //! @sa LoadFont
    //!
    virtual nn::font::Font* AcquireFont(nn::gfx::Device* pDevice, const char* pName) NN_OVERRIDE;

    //! @brief シェーダ情報を取得します。
    //!
    //! @details
    //! pName で指定したシェーダリソース名と関連付けられた
    //! シェーダ情報を取得します。
    //!
    //! シェーダを所有していない場合には LoadShader() でロードを試みます。
    //!
    //! pName に "xxx_xxx" の形式の名前を指定すると、
    //! 該当のバリエーションを含む統合済みアーカイブシェーダを取得します。
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
    virtual const ShaderInfo* AcquireShader(nn::gfx::Device* pDevice, const char* pName) NN_OVERRIDE;

    //! @brief アーカイブシェーダ情報を取得します。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] signature シグネチャ
    //! @param[in] keyLength 検索キーの長さ
    //! @param[in] pKey 検索キー
    //!
    //! @return シェーダ情報を返します。
    //!
    virtual const ShaderInfo* AcquireArchiveShader(nn::gfx::Device* pDevice, uint32_t signature, size_t keyLength, const uint32_t* pKey) NN_OVERRIDE;

    //! @brief テクスチャービューをディスクリプタプールに登録します。
    //!
    //! @param[in] pRegisterTextureViewSlot     ディスクリプタスロット確保関数です。
    //! @param[in] pUserData     関数に渡されるユーザーデーターです。
    //!
    virtual void RegisterTextureViewToDescriptorPool(RegisterTextureViewSlot pRegisterTextureViewSlot, void* pUserData) NN_OVERRIDE;

    //! @brief テクスチャービューをディスクリプタプールから解放します。
    //!
    //! @param[in] pUnregisterTextureViewSlot     ディスクリプタスロット解放関数です。
    //! @param[in] pUserData     関数に渡されるユーザーデーターです。
    //!
    virtual void UnregisterTextureViewFromDescriptorPool(UnregisterTextureViewSlot pUnregisterTextureViewSlot, void* pUserData) NN_OVERRIDE;

    //! @brief  キャプチャテクスチャをリソースへ登録します。
    //!
    //! @param[in]  pName   登録するキャプチャテクスチャの名前です。
    //!
    //! @return 登録した RenderTargetTextureInfo のポインタです。
    //!
    virtual RenderTargetTextureInfo* RegisterRenderTargetTexture(const char* pName) NN_OVERRIDE;

    //! @brief キャプチャテクスチャの登録を解除します。
    //!
    //! @param[in] pTexInfo テクスチャ情報です。
    //!
    //! @sa RegisterRenderTargetTexture
    //!
    virtual void UnregisterRenderTargetTexture(TextureInfo* pTexInfo) NN_OVERRIDE;

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
    //! @param[out] pTexInfo ロードしたテクスチャの情報を格納します。
    //! @param[in] pDevice      デバイスです。
    //! @param[in] pName         テクスチャリソースの名前です。
    //!
    //! @return ロードが成功した場合は true が返ります。
    //!
    //! @sa AcquireTexture
    //! @sa nn::ui2d::LoadTexture
    //!
    virtual bool LoadTexture(ResourceTextureInfo* pTexInfo, nn::gfx::Device* pDevice, const char *pName) NN_OVERRIDE;

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
    virtual nn::font::Font* LoadFont(nn::gfx::Device* pDevice, const char* pName) NN_OVERRIDE;

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
    virtual bool LoadShader(ShaderInfo* pShaderInfo, nn::gfx::Device* pDevice, const char* pName) NN_OVERRIDE;

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
    virtual bool LoadArchiveShader(ShaderInfo* pShaderInfo, nn::gfx::Device* pDevice, uint32_t signature, size_t keyLength, const uint32_t* pKey) NN_OVERRIDE;

private:
    ArchiveHandle m_ArcHandle;
    void* m_ArcBuf;
    FontContainer m_UserRegisteredFontList;
    TextureContainer m_UserRegisteredTextureList;
    ShaderContainer m_UserRegisteredShaderList;
    char m_ResRootDir[RootPathLengthMax];
};

class MultiArcResourceAccessor;

// アーカイブの情報を保持するためのクラスです。

//! @brief 内部用機能のため使用禁止です。
class ArcResourceLink
{
public:

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    ArcResourceLink()
        : m_pArcHandle(NULL)
    {
    }

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief アーカイブハンドルを設定します。
    //!
    //! @param[in] pArchiveHandle アーカイブハンドル
    //!
    void SetArcHandle(ArchiveHandle* pArchiveHandle)
    {
        m_pArcHandle = pArchiveHandle;
    }

    //! @brief アーカイブハンドルを取得します。
    //!
    //! @return アーカイブハンドル
    //!
    ArchiveHandle* GetArcHandle()
    {
        return m_pArcHandle;
    }

    //! @brief アーカイブハンドルを取得します。
    //!
    //! @return アーカイブハンドル
    //!
    const ArchiveHandle* GetArcHandle() const
    {
        return m_pArcHandle;
    }

    //@}

    // リストのリンク情報です。

    //! @brief 内部用機能のため使用禁止です。
    nn::util::IntrusiveListNode m_Link;

private:
    ArchiveHandle* m_pArcHandle;
};

typedef nn::util::IntrusiveList<ArcResourceLink, nn::util::IntrusiveListMemberNodeTraits<ArcResourceLink, &ArcResourceLink::m_Link> > ArcResourceList;

//---------------------------------------------------------------------------
//! @brief 複数のアーカイブリソースにアクセスするためのクラスです。
//!
//---------------------------------------------------------------------------
class MultiArcResourceAccessor : public ResourceAccessor
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(ResourceAccessor);

    //! パス名の最大長です。
    static const int RootPathLengthMax = ArchiveHandle::RootPathLengthMax;

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    MultiArcResourceAccessor();

    //! @brief デストラクタです。
    //!
    virtual ~MultiArcResourceAccessor();

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief 全リソースを解放します。
    //!
    //! @details
    //! デストラクタが呼ばれる前に呼び出してください。
    //!
    //! @param[in] pDevice デバイスです。
    //!
    virtual void Finalize(nn::gfx::Device* pDevice) NN_OVERRIDE;

    //! @brief アーカイブ情報を登録します。
    //!
    //! @details
    //! ArchiveHandle を登録します。
    //!
    //! @param[in] pArchiveHandle    ArchiveHandleオブジェクトへのポインタです。
    //!
    void Attach(ArchiveHandle* pArchiveHandle);

    //! @brief 指定したアーカイブデータの登録を解除します。
    //!
    //! @param[in] pArchiveHandle アーカイブデータへのポインタ。
    //!
    void Detach(const ArchiveHandle* pArchiveHandle);

    //! @brief 登録されているすべてのアーカイブ情報を登録解除します。
    //!
    //! @sa Detach
    //!
    void DetachAll();

    virtual void* FindResourceByName(
        size_t* pSize,
        ResType resType,
        const char* pName
    ) NN_OVERRIDE;

    virtual void* FindResourceByName(ResType resType, const char* pName) NN_OVERRIDE
    {
        return FindResourceByName(NULL, resType, pName);
    }

    virtual const void* FindResourceByName(size_t * pSize, ResType resType, const char* pName) const NN_OVERRIDE
    {
        return const_cast<MultiArcResourceAccessor*>(this)->FindResourceByName(pSize, resType, pName);
    }

    virtual const void* FindResourceByName(ResType resType, const char* pName) const NN_OVERRIDE
    {
        return FindResourceByName(NULL, resType, pName);
    }

    //! @brief 指定した種類のリソースをすべて取得します。
    //!
    //! @param[in] resType   リソースの種類です。
    //! @param[in] pCallback リソースが見つかるたびに呼ばれるコールバック関数です。
    //! @param[in] pParam コールバックに与えるパラメータです。
    //!
    virtual void FindResourceByType(
        ResType resType,
        FindResourceByTypeCallback pCallback,
        void* pParam
    ) const NN_OVERRIDE;

    //! @brief フォントオブジェクトを登録します。
    //!
    //! @details
    //! フォントオブジェクトをリソースアクセサに設定し、
    //! AcquireFont() の検索対象に含まれるようにします。
    //!
    //! @param[in] pName     フォントリソースの名前です。
    //! @param[in] pFont    フォントオブジェクトへのポインタです。
    //!
    //! @return 登録キーを返します。キーは登録の抹消に使用します。
    //!
    //! @sa UnregisterFont
    //!
    FontKey RegisterFont(const char* pName, nn::font::Font* pFont);

    //! @brief フォントオブジェクトの登録を解除します。
    //!
    //! @param[in] key  フォントの登録時に取得した登録キーを指定します。
    //!
    //! @sa RegisterFont
    //!
    void UnregisterFont(FontKey key);

    //! @brief テクスチャを登録します。
    //!
    //! @details
    //! テクスチャをリソースアクセサに設定し、
    //! AcquireTexture() の検索対象に含まれるようにします。
    //!
    //! テクスチャを登録するには、返り値の TextureInfo* に対して、
    //! テクスチャの設定を行ってください。
    //!
    //! @param[in] pName テクスチャの名前です。
    //!
    //! @return テクスチャ情報です。メモリ確保できない場合には NULL が返ります。
    //!
    //! @sa UnregisterTexture
    //!
    PlacementTextureInfo* RegisterTexture(const char* pName);

    //! @brief テクスチャの登録を解除します。
    //!
    //! @param[in] pTexInfo  テクスチャ情報です。
    //!
    //! @sa RegisterTexture
    //!
    void UnregisterTexture(TextureInfo* pTexInfo);

    //! @brief シェーダを登録します。
    //!
    //! @details
    //! シェーダをリソースアクセサに設定し、
    //! AcquireShader() の検索対象に含まれるようにします。
    //!
    //! @param[in] pName シェーダの名前です。
    //!
    //! @return シェーダ情報です。メモリ確保できない場合には NULL が返ります。
    //!
    //! @sa UnregisterShader
    //!
    ShaderInfo* RegisterShader(const char* pName);

    //! @brief シェーダの登録を解除します。
    //!
    //! @param[in] pShaderInfo  シェーダ情報です。
    //!
    //! @sa RegisterShader
    //!
    void UnregisterShader(ShaderInfo* pShaderInfo);

    //! @brief テクスチャ情報を取得します。
    //!
    //! @details
    //! pName で指定したテクスチャリソース名と関連付けられた
    //! テクスチャ情報を取得します。
    //!
    //! テクスチャを所有していない場合には LoadTexture() でロードを試みます。
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
    virtual const TextureInfo* AcquireTexture(nn::gfx::Device* pDevice, const char* pName) NN_OVERRIDE;

    //! @brief フォントを取得します。
    //!
    //! @details
    //! pName で指定したフォントリソース名と関連付けられた
    //! フォントオブジェクトを取得します。
    //!
    //! 指定されたフォントが登録されていなかった場合には
    //! LoadFont() により生成したフォントを登録します。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] pName フォントリソースの名前です。
    //!
    //! @return フォントへのポインタを返します。
    //!
    //! @sa LoadFont
    //!
    virtual nn::font::Font* AcquireFont(nn::gfx::Device* pDevice, const char* pName) NN_OVERRIDE;

    //! @brief シェーダ情報を取得します。
    //!
    //! @details
    //! pName で指定したシェーダリソース名と関連付けられた
    //! シェーダ情報を取得します。
    //!
    //! シェーダを所有していない場合には LoadShader() でロードを試みます。
    //!
    //! pName に "xxx_xxx" の形式の名前を指定すると、
    //! 該当のバリエーションを含む統合済みアーカイブシェーダを取得します。
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
    virtual const ShaderInfo* AcquireShader(nn::gfx::Device* pDevice, const char* pName) NN_OVERRIDE;

    //! @brief アーカイブシェーダ情報を取得します。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] signature シグネチャ
    //! @param[in] keyLength 検索キーの長さ
    //! @param[in] pKey 検索キー
    //!
    //! @return シェーダ情報を返します。
    //!
    virtual const ShaderInfo* AcquireArchiveShader(nn::gfx::Device* pDevice, uint32_t signature, size_t keyLength, const uint32_t* pKey) NN_OVERRIDE;

    //! @brief テクスチャービューをディスクリプタプールに登録します。
    //!
    //! @param[in] pRegisterTextureViewSlot     ディスクリプタスロット確保関数です。
    //! @param[in] pUserData     関数に渡されるユーザーデーターです。
    //!
    virtual void RegisterTextureViewToDescriptorPool(RegisterTextureViewSlot pRegisterTextureViewSlot, void* pUserData) NN_OVERRIDE;

    //! @brief テクスチャービューをディスクリプタプールから解放します。
    //!
    //! @param[in] pUnregisterTextureViewSlot     ディスクリプタスロット解放関数です。
    //! @param[in] pUserData     関数に渡されるユーザーデーターです。
    //!
    virtual void UnregisterTextureViewFromDescriptorPool(UnregisterTextureViewSlot pUnregisterTextureViewSlot, void* pUserData) NN_OVERRIDE;

    //! @brief  キャプチャテクスチャをリソースへ登録します。
    //!
    //! @param[in]  pName   登録するキャプチャテクスチャの名前です。
    //!
    //! @return 登録した RenderTargetTextureInfo のポインタです。
    //!
    virtual RenderTargetTextureInfo* RegisterRenderTargetTexture(const char* pName) NN_OVERRIDE;

    //! @brief キャプチャテクスチャの登録を解除します。
    //!
    //! @param[in] pTexInfo テクスチャ情報です。
    //!
    //! @sa RegisterRenderTargetTexture
    //!
    virtual void UnregisterRenderTargetTexture(TextureInfo* pTexInfo) NN_OVERRIDE;

    //@}

protected:
    //! @brief アーカイバのリストを取得します。
    //!
    //! @details
    //! アーカイバに含まれているファイルを独自に検索したい場合などに
    //! 使用してください。
    //!
    //! @return アーカイバのリスト
    //!
    ArcResourceList* GetArcList()
    {
        return &m_ArcList;
    }

    //! @brief テクスチャをロードします。
    //!
    //! @details
    //! テクスチャが初めて参照されたときに AcquireTexture() から呼び出されます。
    //!
    //! FindResourceByName() でテクスチャリソースを取得し、
    //! nn::ui2d::LoadTexture() でデバイスにロードします。
    //!
    //! @param[out] pTexInfo ロードしたテクスチャの情報を格納します。
    //! @param[in] pDevice      デバイスです。
    //! @param[in] pName         テクスチャリソースの名前です。
    //!
    //! @return ロードが成功した場合は true が返ります。
    //!
    //! @sa AcquireTexture
    //! @sa nn::ui2d::LoadTexture
    //!
    virtual bool LoadTexture(ResourceTextureInfo* pTexInfo, nn::gfx::Device* pDevice, const char *pName) NN_OVERRIDE;

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
    virtual bool LoadShader(ShaderInfo* pShaderInfo, nn::gfx::Device* pDevice, const char* pName) NN_OVERRIDE;

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
    virtual bool LoadArchiveShader(ShaderInfo* pShaderInfo, nn::gfx::Device* pDevice, uint32_t signature, size_t keyLength, const uint32_t* pKey) NN_OVERRIDE;

private:
    //! @brief フォントリソースが存在するアーカイブを取得します。
    //!
    //! @param[in] pName フォントリソースの名前です。
    //!
    //! @return フォントリソースが存在するアーカイブです。存在しなければNULLが返ります。
    //!
    ArchiveHandle* FindFontArchive(const char* pName);

    //! @brief テクスチャリソースが存在するアーカイブを取得します。
    //!
    //! @param[in] pName テクスチャリソースの名前です。
    //!
    //! @return テクスチャリソースが存在するアーカイブです。存在しなければNULLが返ります。
    //!
    ArchiveHandle* FindTextureArchive(const char* pName);

    //! @brief シェーダリソースが存在するアーカイブを取得します。
    //!
    //! @param[in] pName シェーダリソースの名前です。
    //!
    //! @return シェーダリソースが存在するアーカイブです。存在しなければNULLが返ります。
    //!
    ArchiveHandle* FindShaderArchive(const char* pName);

    //! @brief アーカイブシェーダリソースが存在するアーカイブを取得します。
    //!
    //! @param[in] signature シグネチャ
    //! @param[in] keyLength 検索キーの長さ
    //! @param[in] pKey 検索キー
    //!
    //! @return シェーダリソースが存在するアーカイブです。存在しなければNULLが返ります。
    //!
    ArchiveHandle* FindArchiveShaderArchive(uint32_t signature, size_t keyLength, const uint32_t* pKey);

    ArcResourceList m_ArcList;
    FontContainer m_UserRegisteredFontList;
    TextureContainer m_UserRegisteredTextureList;
    ShaderContainer m_UserRegisteredShaderList;
};

}   // namespace ui2d
}   // namespace nn
