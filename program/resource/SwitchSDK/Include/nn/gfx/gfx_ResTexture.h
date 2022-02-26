/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
* @file
* @brief テクスチャーリソースファイルに関する API の宣言
*/

#pragma once

#include <nn/util/util_AccessorBase.h>

#include <nn/gfx/gfx_MemoryPoolInfo.h>
#include <nn/gfx/gfx_TextureInfo.h>
#include <nn/gfx/gfx_ResTextureData.h>

#include <nn/gfx/detail/gfx_Declare.h>

namespace nn {
namespace gfx {

/**
* @brief テクスチャーリソースを表すクラスです。
*/
class ResTexture
    : public nn::util::AccessorBase< ResTextureData >
{
    NN_DISALLOW_COPY( ResTexture );

public:
    /**
    * @brief テクスチャーリソースのシグネチャーです。
    */
    static const int Signature = NN_UTIL_CREATE_SIGNATURE_4( 'B', 'R', 'T', 'I' );

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] pData 変換するデータへのポインター
    *
    * @return 変換されたアクセサーへのポインターを返します。
    */
    static ResTexture* ToAccessor( ResTextureData* pData ) NN_NOEXCEPT
    {
        return static_cast< ResTexture* >( pData );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] data 変換するデータへの参照
    *
    * @return 変換されたアクセサーへの参照を返します。
    */
    static ResTexture& ToAccessor( ResTextureData& data ) NN_NOEXCEPT
    {
        return static_cast< ResTexture& >( data );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] pData 変換するデータへのポインター
    *
    * @return 変換されたアクセサーへのポインターを返します。
    */
    static const ResTexture* ToAccessor( const ResTextureData* pData ) NN_NOEXCEPT
    {
        return static_cast< const ResTexture* >( pData );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] data 変換するデータへの参照
    *
    * @return 変換されたアクセサーへの参照を返します。
    */
    static const ResTexture& ToAccessor( const ResTextureData& data ) NN_NOEXCEPT
    {
        return static_cast< const ResTexture& >( data );
    }

    /**
    * @brief テクスチャーを初期化するための情報を取得します。
    *
    * @return テクスチャーを初期化するための情報へのポインターを返します。
    */
    TextureInfo* GetTextureInfo() NN_NOEXCEPT
    {
        return DataToAccessor( this->textureInfoData );
    }

    /**
    * @brief テクスチャーを初期化するための情報を取得します。
    *
    * @return テクスチャーを初期化するための情報へのポインターを返します。
    */
    const TextureInfo* GetTextureInfo() const NN_NOEXCEPT
    {
        return DataToAccessor( this->textureInfoData );
    }

#if defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
    /**
    * @brief テクスチャーオブジェクトを取得します。
    *
    * @return nn::gfx::Texture 型のテクスチャーオブジェクトへのポインターを返します。
    *
    * @details
    * この関数の返り値は、nn::gfx::Texture 型のポインターに代入してください。
    */
    Texture* GetTexture() NN_NOEXCEPT;
#else
    detail::Caster< void > GetTexture() NN_NOEXCEPT
    {
        return detail::Caster< void >( this->pTexture.Get() );
    }
#endif

#if defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
    /**
    * @brief テクスチャーオブジェクトを取得します。
    *
    * @return nn::gfx::Texture 型のテクスチャーオブジェクトへのポインターを返します。
    *
    * @details
    * この関数の返り値は、nn::gfx::Texture 型のポインターに代入してください。
    */
    const Texture* GetTexture() const NN_NOEXCEPT;
#else
    detail::Caster< const void > GetTexture() const NN_NOEXCEPT
    {
        return detail::Caster< const void >( this->pTexture.Get() );
    }
#endif

#if defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
    /**
    * @brief テクスチャービューオブジェクトを取得します。
    *
    * @return nn::gfx::TextureView 型のテクスチャービューオブジェクトへのポインターを返します。
    *
    * @details
    * この関数の返り値は、nn::gfx::TextureView 型のポインターに代入してください。
    */
    TextureView* GetTextureView() NN_NOEXCEPT;
#else
    detail::Caster< void > GetTextureView() NN_NOEXCEPT
    {
        return detail::Caster< void >( this->pTextureView.Get() );
    }
#endif

#if defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
    /**
    * @brief テクスチャービューオブジェクトを取得します。
    *
    * @return nn::gfx::TextureView 型のテクスチャービューオブジェクトへのポインターを返します。
    *
    * @details
    * この関数の返り値は、nn::gfx::TextureView 型のポインターに代入してください。
    */
    const TextureView* GetTextureView() const NN_NOEXCEPT;
#else
    detail::Caster< const void > GetTextureView() const NN_NOEXCEPT
    {
        return detail::Caster< const void >( this->pTextureView.Get() );
    }
#endif

    /**
    * @brief テクスチャーを初期化します。
    *
    * @tparam TTarget 対象の低レベルグラフィックス API
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - テクスチャーが初期化されていない
    * - テクスチャーが属しているテクスチャーリソースファイルが初期化されている
    *
    * @post
    * - テクスチャーが初期化されている
    *
    * @details
    * nn::gfx::Texture と nn::gfx::TextureView の Initialize に相当する処理を行います。
    */
    template< typename TTarget >
    void Initialize( TDevice< TTarget >* pDevice ) NN_NOEXCEPT;

    /**
    * @brief テクスチャーを破棄します。
    *
    * @tparam TTarget 対象の低レベルグラフィックス API
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - テクスチャーが初期化されている
    *
    * @post
    * - テクスチャーが初期化されていない
    *
    * @details
    * nn::gfx::Texture と nn::gfx::TextureView の Finalize に相当する処理を行います。
    */
    template< typename TTarget >
    void Finalize( TDevice< TTarget >* pDevice ) NN_NOEXCEPT;

    /**
    * @brief テクスチャーの名前を取得します。
    *
    * @return テクスチャーの名前を表す文字列へのポインターを返します
    */
    const char* GetName() const NN_NOEXCEPT
    {
        return this->pName.Get()->GetData();
    }

    /**
    * @brief バイトでのテクスチャーのストレージサイズを取得します。
    *
    * @return バイトでのテクスチャーのストレージサイズを返します。
    */
    size_t GetDataSize() const NN_NOEXCEPT
    {
        return this->textureDataSize;
    }

    /**
    * @brief ユーザーデータを取得します。
    *
    * @param[in] index 取得するユーザーデータのインデックス
    *
    * @return ユーザーデータへのポインターを返します。
    */
    ResUserData* GetUserData( int index ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE( index, 0, this->pUserDataDic.Get()->GetCount() );
        return &this->pUserDataArray.Get()[ index ];
    }

    /**
    * @brief ユーザーデータを取得します。
    *
    * @param[in] index 取得するユーザーデータのインデックス
    *
    * @return ユーザーデータへのポインターを返します。
    */
    const ResUserData* GetUserData( int index ) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT_RANGE( index, 0, this->pUserDataDic.Get()->GetCount() );
        return &this->pUserDataArray.Get()[ index ];
    }

    /**
    * @brief ユーザーデータ辞書を取得します。
    *
    * @return ユーザーデータ辞書へのポインターを返します。
    */
    const nn::util::ResDic* GetUserDataDic() const NN_NOEXCEPT
    {
        return this->pUserDataDic.Get();
    }

    /**
    * @brief 任意のデスクリプタースロットを設定します。
    *
    * @param[in] value 設定する任意のデスクリプタースロット
    *
    * @post
    * - 任意のデスクリプタースロットが設定されている
    */
    void SetUserDescriptorSlot( const DescriptorSlot& value ) NN_NOEXCEPT
    {
        this->userDescriptorSlot = AccessorToData( value );
    }

    /**
    * @brief 設定した任意のデスクリプタースロットを取得します。
    *
    * @param[in] pOutDescriptorSlot 設定した任意のデスクリプタースロットを格納するためのポインター
    *
    * @pre
    * - pOutDescriptorSlot != NULL
    * - 任意のデスクリプタースロットが設定されている
    */
    void GetUserDescriptorSlot( DescriptorSlot* pOutDescriptorSlot ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_NOT_NULL( pOutDescriptorSlot );
        DescriptorSlot::DataType* pOutDescriptorSlotData = AccessorToData( pOutDescriptorSlot );
        *pOutDescriptorSlotData = this->userDescriptorSlot;
    }
};

/**
* @brief テクスチャーリソースファイルです。
*/
class ResTextureFile
    : public nn::util::AccessorBase< ResTextureFileData >
{
    NN_DISALLOW_COPY( ResTextureFile );

public:
    /**
    * @brief テクスチャーリソースファイルのシグネチャーです。
    */
    static const int64_t Signature = NN_UTIL_CREATE_SIGNATURE_8('B', 'N', 'T', 'X', 0, 0, 0, 0);

    /**
    * @brief テクスチャーリソースファイルのメジャーバージョンです。
    */
    static const int MajorVersion = 4;

    /**
    * @brief テクスチャーリソースのマイナーバージョンです。
    */
    static const int MinorVersion = 0;

    /**
    * @brief テクスチャーリソースのマイクロバージョンです。
    */
    static const int MicroVersion = 0;

    /**
    * @brief テクスチャーリソースに要求されるアライメントの最大値を取得します。
    *
    * @return テクスチャーリソースに要求されるアライメントの最大値を返します。
    *
    * @details
    * リソースごとの正確なアライメントはファイルヘッダーに記録されています。
    */
    static size_t GetMaxFileAlignment() NN_NOEXCEPT;

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] pData 変換するデータへのポインター
    *
    * @return 変換されたアクセサーへのポインターを返します。
    */
    static ResTextureFile* ToAccessor( ResTextureFileData* pData ) NN_NOEXCEPT
    {
        return static_cast< ResTextureFile* >( pData );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] data 変換するデータへの参照
    *
    * @return 変換されたアクセサーへの参照を返します。
    */
    static ResTextureFile& ToAccessor( ResTextureFileData& data ) NN_NOEXCEPT
    {
        return static_cast< ResTextureFile& >( data );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] pData 変換するデータへのポインター
    *
    * @return 変換されたアクセサーへのポインターを返します。
    */
    static const ResTextureFile* ToAccessor( const ResTextureFileData* pData ) NN_NOEXCEPT
    {
        return static_cast< const ResTextureFile* >( pData );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] data 変換するデータへの参照
    *
    * @return 変換されたアクセサーへの参照を返します。
    */
    static const ResTextureFile& ToAccessor( const ResTextureFileData& data ) NN_NOEXCEPT
    {
        return static_cast< const ResTextureFile& >( data );
    }

    /**
    * @brief バイナリーデータがテクスチャーファイルとして有効かどうかを取得します。
    *
    * @param[in] ptr バイナリーデータへのポインター
    *
    * @return 指定されたバイナリーデータがテクスチャーファイルとして有効かどうかの真偽値を返します。
    *
    * @pre
    * - ptr != NULL
    */
    static bool IsValid( const void* ptr ) NN_NOEXCEPT;

    /**
    * @brief バイナリーデータをテクスチャーリソースファイル型にキャストします。
    *
    * @param[in] ptr キャストするバイナリーデータへのポインター
    *
    * @return キャストされたテクスチャーリソースファイルへのポインターを返します。
    *
    * @pre
    * - ptr != NULL
    *
    * @details
    * 直接キャストせずにこの関数を通してキャストしてください。
    */
    static ResTextureFile* ResCast( void* ptr ) NN_NOEXCEPT;

    /**
    * @brief テクスチャーリソースファイルを初期化します。
    *
    * @tparam TTarget 対象の低レベルグラフィックス API
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - テクスチャーリソースファイルが初期化されていない
    *
    * @post
    * - テクスチャーリソースファイルが初期化されている
    */
    template< typename TTarget >
    void Initialize( TDevice< TTarget >* pDevice ) NN_NOEXCEPT;

    /**
    * @brief テクスチャーリソースファイルを初期化します。
    *
    * @tparam TTarget 対象の低レベルグラフィックス API
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] pMemoryPool テクスチャーリソースファイルが配置されているメモリープール
    * @param[in] memoryPoolOffset pMemoryPool におけるテクスチャーリソースファイルが配置されているバイトでのオフセット
    * @param[in] memoryPoolSize pMemoryPool のバイトでのプールサイズ
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    *
    * @post
    * - テクスチャーリソースファイルが初期化されている
    *
    * @details
    * pMemoryPool == NULL の場合、テクスチャーリソースがもっているメモリープールを使って初期化します。
    */
    template< typename TTarget >
    void Initialize( TDevice< TTarget >* pDevice, TMemoryPool< TTarget >* pMemoryPool,
        ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT;

    /**
    * @brief テクスチャーリソースファイルを破棄します。
    *
    * @tparam TTarget 対象の低レベルグラフィックス API
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - テクスチャーリソースファイルが初期化されている
    *
    * @post
    * - テクスチャーリソースファイルが初期化されていない
    */
    template< typename TTarget >
    void Finalize( TDevice< TTarget >* pDevice ) NN_NOEXCEPT;

    /**
    * @brief テクスチャーリソースを取得します。
    *
    * @param[in] index 取得するテクスチャーリソースのインデックス
    *
    * @return テクスチャーリソースを返します。
    */
    ResTexture* GetResTexture( int index ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( index >= 0 && index < static_cast< int >( this->textureContainerData.textureCount ) );
        NN_SDK_ASSERT( this->textureContainerData.pTexturePtrArray.Get() );
        return this->textureContainerData.pTexturePtrArray.Get()[ index ].Get();
    }

    /**
    * @brief テクスチャーリソースを取得します。
    *
    * @param[in] index 取得するテクスチャーリソースのインデックス
    *
    * @return テクスチャーリソースを返します。
    */
    const ResTexture* GetResTexture( int index ) const NN_NOEXCEPT
    {
        NN_SDK_ASSERT( index >= 0 && index < static_cast< int >( this->textureContainerData.textureCount ) );
        NN_SDK_ASSERT( this->textureContainerData.pTexturePtrArray.Get() );
        return this->textureContainerData.pTexturePtrArray.Get()[ index ].Get();
    }

    /**
    * @brief テクスチャー辞書を取得します。
    *
    * @return テクスチャー辞書へのポインターを返します。
    */
    const nn::util::ResDic* GetTextureDic() const NN_NOEXCEPT
    {
        return this->textureContainerData.pTextureDic.Get();
    }

    /**
    * @brief ファイルヘッダーを取得します。
    *
    * @return ファイルヘッダーを返します。
    *
    * @see
    * nn::util::BinaryFileHeader
    *
    * @details
    * 手動でリロケーションを制御したい場合などに利用できます。
    */
    nn::util::BinaryFileHeader* GetBinaryFileHeader() NN_NOEXCEPT
    {
        return &this->fileHeader;
    }

    /**
    * @brief ファイルヘッダーを取得します。
    *
    * @return ファイルヘッダーを返します。
    *
    * @see
    * nn::util::BinaryFileHeader
    */
    const nn::util::BinaryFileHeader* GetBinaryFileHeader() const NN_NOEXCEPT
    {
        return &this->fileHeader;
    }
};

template< typename TTarget >
inline void ResTexture::Initialize( TDevice< TTarget >* pDevice ) NN_NOEXCEPT
{
    NN_SDK_ASSERT( this->pTexture.Get() );
    NN_SDK_ASSERT( this->pTextureView.Get() );
    const TextureInfo& textureInfo = DataToAccessor( this->textureInfoData );
    ptrdiff_t poolOffset = this->pResTextureContainerData.Get()->memoryPoolOffsetBase +
        nn::util::BytePtr( this->pResTextureContainerData.Get()->pTextureData.Get(),
        sizeof( nn::util::BinaryBlockHeader ) ).Distance( this->pMipPtrArray.Get()[ 0 ].Get() );
    static_cast< TTexture< TTarget >* >( this->pTexture.Get() )->Initialize( pDevice,
        textureInfo, static_cast< TMemoryPool< TTarget >* >(
        this->pResTextureContainerData.Get()->pCurrentMemoryPool.Get() ), poolOffset, this->textureDataSize );

    TextureViewInfo textureViewInfo;
    textureViewInfo.SetDefault();
    textureViewInfo.SetImageDimension( static_cast< ImageDimension >( this->imageDimension ) );
    textureViewInfo.SetChannelMapping( static_cast< ChannelMapping >( this->channelMapping[ 0 ] ),
        static_cast< ChannelMapping >( this->channelMapping[ 1 ] ),
        static_cast< ChannelMapping >( this->channelMapping[ 2 ] ),
        static_cast< ChannelMapping >( this->channelMapping[ 3 ] ) );
    textureViewInfo.SetImageFormat( textureInfo.GetImageFormat() );
    textureViewInfo.SetTexturePtr( static_cast< TTexture< TTarget >* >( this->pTexture.Get() ) );
    textureViewInfo.EditSubresourceRange().EditArrayRange().SetArrayLength( textureInfo.GetArrayLength() );
    textureViewInfo.EditSubresourceRange().EditMipRange().SetMipCount( textureInfo.GetMipCount() );
    static_cast< TTextureView< TTarget >* >( this->pTextureView.Get() )->Initialize( pDevice, textureViewInfo );
}

template< typename TTarget >
inline void ResTexture::Finalize( TDevice< TTarget >* pDevice ) NN_NOEXCEPT
{
    NN_SDK_ASSERT( this->pTexture.Get() );
    NN_SDK_ASSERT( this->pTextureView.Get() );
    static_cast< TTexture< TTarget >* >( this->pTexture.Get() )->Finalize( pDevice );
    static_cast< TTextureView< TTarget >* >( this->pTextureView.Get() )->Finalize( pDevice );
}

template< typename TTarget >
inline void ResTextureFile::Initialize( TDevice< TTarget >* pDevice ) NN_NOEXCEPT
{
    return Initialize< TTarget >( pDevice, NULL, 0, 0 );
}

template< typename TTarget >
inline void ResTextureFile::Initialize( TDevice< TTarget >* pDevice, TMemoryPool< TTarget >* pMemoryPool,
    ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL( pDevice );
    NN_SDK_ASSERT( nn::util::is_aligned( memoryPoolOffset, this->fileHeader.GetAlignment() ) );
    NN_UNUSED( memoryPoolSize );

    if( pMemoryPool )
    {
        this->textureContainerData.pCurrentMemoryPool.Set( pMemoryPool );
        this->textureContainerData.memoryPoolOffsetBase = static_cast< uint32_t >(
            memoryPoolOffset + nn::util::BytePtr( this ).Distance( nn::util::BytePtr(
            this->textureContainerData.pTextureData.Get(), sizeof( nn::util::BinaryBlockHeader ) ).Get() ) );
    }
    else
    {
        NN_SDK_ASSERT_NOT_NULL( this->textureContainerData.pTextureMemoryPool.Get() );
        MemoryPoolInfo memoryPoolInfo;
        memoryPoolInfo.SetMemoryPoolProperty( MemoryPoolProperty_CpuInvisible | MemoryPoolProperty_GpuCached );
        memoryPoolInfo.SetPoolMemory( nn::util::BytePtr( this->textureContainerData.pTextureData.Get(),
            sizeof( nn::util::BinaryBlockHeader ) ).Get(), static_cast< nn::util::BinaryBlockHeader* >(
            this->textureContainerData.pTextureData.Get() )->GetBlockSize() - sizeof( nn::util::BinaryBlockHeader ) );
        static_cast< TMemoryPool< TTarget >* >(
            this->textureContainerData.pTextureMemoryPool.Get() )->Initialize( pDevice, memoryPoolInfo );
        this->textureContainerData.pCurrentMemoryPool.Set( this->textureContainerData.pTextureMemoryPool.Get() );
        this->textureContainerData.memoryPoolOffsetBase = 0;
    }
}

template< typename TTarget >
inline void ResTextureFile::Finalize( TDevice< TTarget >* pDevice ) NN_NOEXCEPT
{
    NN_SDK_REQUIRES_NOT_NULL( pDevice );
    NN_SDK_ASSERT( this->textureContainerData.pCurrentMemoryPool.Get() );
    if( this->textureContainerData.pCurrentMemoryPool.Get() == this->textureContainerData.pTextureMemoryPool.Get() )
    {
        static_cast< TMemoryPool< TTarget >* >(
            this->textureContainerData.pTextureMemoryPool.Get() )->Finalize( pDevice );
    }
    this->textureContainerData.pCurrentMemoryPool.Set( NULL );
}

}
}
