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
* @brief シェーダーリソースファイルに関する API の宣言
*/

#pragma once

#include <nn/util/util_AccessorBase.h>

#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_DataAccessorConverter.h>
#include <nn/gfx/gfx_ResShaderData.h>

#include <nn/gfx/detail/gfx_Declare.h>
#include <nn/gfx/detail/gfx_ResShaderImpl.h>

namespace nn {
namespace gfx {

/**
* @brief シェーダープログラムリソースを表すクラスです。
*/
class ResShaderProgram
    : public nn::util::AccessorBase< ResShaderProgramData >
{
    NN_DISALLOW_COPY( ResShaderProgram );

    typedef detail::ResShaderProgramImpl Impl;

public:
    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] pData 変換するデータへのポインター
    *
    * @return 変換されたアクセサーへのポインターを返します。
    */
    static ResShaderProgram* ToAccessor( ResShaderProgramData* pData ) NN_NOEXCEPT
    {
        return static_cast< ResShaderProgram* >( pData );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] data 変換するデータへの参照
    *
    * @return 変換されたアクセサーへの参照を返します。
    */
    static ResShaderProgram& ToAccessor( ResShaderProgramData& data ) NN_NOEXCEPT
    {
        return static_cast< ResShaderProgram& >( data );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] pData 変換するデータへのポインター
    *
    * @return 変換されたアクセサーへのポインターを返します。
    */
    static const ResShaderProgram* ToAccessor( const ResShaderProgramData* pData ) NN_NOEXCEPT
    {
        return static_cast< const ResShaderProgram* >( pData );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] data 変換するデータへの参照
    *
    * @return 変換されたアクセサーへの参照を返します。
    */
    static const ResShaderProgram& ToAccessor( const ResShaderProgramData& data ) NN_NOEXCEPT
    {
        return static_cast< const ResShaderProgram& >( data );
    }

    /**
    * @brief シェーダーを初期化するための情報を取得します。
    *
    * @return シェーダーを初期化するための情報へのポインターを返します。
    */
    ShaderInfo* GetShaderInfo() NN_NOEXCEPT
    {
        return DataToAccessor( this->info );
    }

    /**
    * @brief シェーダーを初期化するための情報を取得します。
    *
    * @return シェーダーを初期化するための情報へのポインターを返します。
    */
    const ShaderInfo* GetShaderInfo() const NN_NOEXCEPT
    {
        return DataToAccessor( this->info );
    }

#if defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
    /**
    * @brief シェーダーオブジェクトを取得します。
    *
    * @return nn::gfx::Shader 型のシェーダーオブジェクトへのポインターを返します。
    *
    * @details
    * この関数の返り値は、nn::gfx::Shader 型のポインターに代入してください。
    */
    Shader* GetShader() NN_NOEXCEPT;
#else
    detail::Caster< void > GetShader() NN_NOEXCEPT
    {
        return detail::Caster< void >( this->pObj.Get() );
    }
#endif

#if defined( NN_BUILD_FOR_DOCUMENT_GENERATION )
    /**
    * @brief シェーダーオブジェクトを取得します。
    *
    * @return nn::gfx::Shader 型のシェーダーオブジェクトへのポインターを返します。
    *
    * @details
    * この関数の返り値は、nn::gfx::Shader 型のポインターに代入してください。
    */
    const Shader* GetShader() const NN_NOEXCEPT;
#else
    detail::Caster< const void > GetShader() const NN_NOEXCEPT
    {
        return detail::Caster< const void >( this->pObj.Get() );
    }
#endif

    /**
    * @brief シェーダーコンパイラーから出力されたリフレクション情報を取得します。
    *
    * @return シェーダーコンパイラーから出力されたリフレクション情報へのポインターを返します。
    *
    * @details
    * シェーダーのコンバート時に、シェーダーコンパイラーから出力されたリフレクション情報を含めるためのオプションが指定されている必要があります。
    * 詳細はシェーダーコンバーターのドキュメントを参照してください。
    */
    const nngfxToolShaderCompilerShaderReflection*
        GetShaderCompilerReflection() const NN_NOEXCEPT
    {
        return this->pShaderCompilerReflection.Get();
    }

    /**
    * @brief シェーダーを初期化します。
    *
    * @tparam TTarget 対象の低レベルグラフィックス API
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @return シェーダーの初期化の結果を返します。
    * @retval ShaderInitializeResult_Success 成功しました。
    * @retval ShaderInitializeResult_InvalidFormat info に設定されているシェーダーフォーマットがこの環境では無効です。
    * @retval ShaderInitializeResult_SetupFailed コンパイル失敗などによりシェーダーのセットアップに失敗しました。
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - シェーダーが初期化されていない
    * - シェーダーが属しているシェーダーコンテナーが初期化されている
    *
    * @post
    * - シェーダーが初期化されている
    *
    * @details
    * nn::gfx::Shader の Initialize に相当する処理を行います。
    */
    template< typename TTarget >
    ShaderInitializeResult Initialize( TDevice< TTarget >* pDevice ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( this->objSize >= sizeof( TShader< TTarget > ) );
        NN_SDK_ASSERT( this->pObj.Get() );
        return static_cast< TShader< TTarget >* >(
            this->pObj.Get() )->Initialize( pDevice, DataToAccessor( this->info ) );
    }

    /**
    * @brief シェーダーを破棄します。
    *
    * @tparam TTarget 対象の低レベルグラフィックス API
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - シェーダーが初期化されている
    *
    * @post
    * - シェーダーが初期化されていない
    *
    * @details
    * nn::gfx::Shader の Finalize に相当する処理を行います。
    */
    template< typename TTarget >
    void Finalize( TDevice< TTarget >* pDevice ) NN_NOEXCEPT
    {
        NN_SDK_ASSERT( this->pObj.Get() );
        static_cast< TShader< TTarget >* >( this->pObj.Get() )->Finalize( pDevice );
    }

    /**
    * @brief このシェーダーを動かすために必要なスクラッチメモリーの推奨されるバイトでのサイズを取得します。
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    *
    * @return
    * - このシェーダーを動かすために必要なスクラッチメモリーの推奨されるバイトでのサイズを返します。
    *
    * @details
    * Nvn 版以外ではこの関数を呼ぶことはできません。
    */
    template< typename TTarget >
    size_t NvnGetRecommendedScrachMemorySize( TDevice< TTarget >* pDevice ) const NN_NOEXCEPT
    {
        return Impl::NvnGetRecommendedScrachMemorySize( this, pDevice );
    }
};

/**
* @brief シェーダーバリエーションを表すクラスです。
*/
class ResShaderVariation
    : public nn::util::AccessorBase< ResShaderVariationData >
{
    NN_DISALLOW_COPY( ResShaderVariation );

public:
    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] pData 変換するデータへのポインター
    *
    * @return 変換されたアクセサーへのポインターを返します。
    */
    static ResShaderVariation* ToAccessor( ResShaderVariationData* pData ) NN_NOEXCEPT
    {
        return static_cast< ResShaderVariation* >( pData );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] data 変換するデータへの参照
    *
    * @return 変換されたアクセサーへの参照を返します。
    */
    static ResShaderVariation& ToAccessor( ResShaderVariationData& data ) NN_NOEXCEPT
    {
        return static_cast< ResShaderVariation& >( data );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] pData 変換するデータへのポインター
    *
    * @return 変換されたアクセサーへのポインターを返します。
    */
    static const ResShaderVariation* ToAccessor( const ResShaderVariationData* pData ) NN_NOEXCEPT
    {
        return static_cast< const ResShaderVariation* >( pData );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] data 変換するデータへの参照
    *
    * @return 変換されたアクセサーへの参照を返します。
    */
    static const ResShaderVariation& ToAccessor( const ResShaderVariationData& data ) NN_NOEXCEPT
    {
        return static_cast< const ResShaderVariation& >( data );
    }

    /**
    * @brief シェーダープログラムリソースを取得します。
    *
    * @param[in] codeType 取得するシェーダープログラムリソースのコードの種類
    *
    * @return シェーダープログラムリソースを返します。
    */
    ResShaderProgram* GetResShaderProgram( ShaderCodeType codeType ) NN_NOEXCEPT
    {
        return const_cast< ResShaderProgram* >( static_cast<
            const ResShaderVariation* >( this )->GetResShaderProgram( codeType ) );
    }

    /**
    * @brief シェーダープログラムリソースを取得します。
    *
    * @param[in] codeType 取得するシェーダープログラムリソースのコードの種類
    *
    * @return シェーダープログラムリソースを返します。
    *
    * @details
    * ShaderCodeType_SourceArray の種類を取得する際は ShaderCodeType_Source を指定してください。
    */
    const ResShaderProgram* GetResShaderProgram( ShaderCodeType codeType ) const NN_NOEXCEPT
    {
        static const nn::util::BinTPtr< ResShaderProgramData >
            ResShaderVariationData::*s_pResShaderProgramDataTable[] =
        {
            &ResShaderVariationData::pBinaryProgram,
            &ResShaderVariationData::pIntermediateLanguageProgram,
            &ResShaderVariationData::pSourceProgram,
            NULL
        };
        return ResShaderProgram::ToAccessor( ( this->*s_pResShaderProgramDataTable[ codeType ] ).Get() );
    }
};

/**
* @brief シェーダーコンテナーリソースを表すクラスです。
*
* @details
* シェーダーコンテナーはバリエーションとして複数のシェーダーを保持しています。
*/
class ResShaderContainer
    : public nn::util::AccessorBase< ResShaderContainerData >
{
    NN_DISALLOW_COPY( ResShaderContainer );

    typedef detail::ResShaderContainerImpl Impl;

public:
    /**
    * @brief シェーダーコンテナーリソースのシグネチャーです。
    */
    static const int Signature = NN_UTIL_CREATE_SIGNATURE_4( 'g', 'r', 's', 'c' );

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] pData 変換するデータへのポインター
    *
    * @return 変換されたアクセサーへのポインターを返します。
    */
    static ResShaderContainer* ToAccessor( ResShaderContainerData* pData ) NN_NOEXCEPT
    {
        return static_cast< ResShaderContainer* >( pData );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] data 変換するデータへの参照
    *
    * @return 変換されたアクセサーへの参照を返します。
    */
    static ResShaderContainer& ToAccessor( ResShaderContainerData& data ) NN_NOEXCEPT
    {
        return static_cast< ResShaderContainer& >( data );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] pData 変換するデータへのポインター
    *
    * @return 変換されたアクセサーへのポインターを返します。
    */
    static const ResShaderContainer* ToAccessor( const ResShaderContainerData* pData ) NN_NOEXCEPT
    {
        return static_cast< const ResShaderContainer* >( pData );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] data 変換するデータへの参照
    *
    * @return 変換されたアクセサーへの参照を返します。
    */
    static const ResShaderContainer& ToAccessor( const ResShaderContainerData& data ) NN_NOEXCEPT
    {
        return static_cast< const ResShaderContainer& >( data );
    }

    /**
    * @brief シェーダーコンテナーを初期化します。
    *
    * @tparam TTarget 対象の低レベルグラフィックス API
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - シェーダーコンテナーが初期化されていない
    *
    * @post
    * - シェーダーコンテナーが初期化されている
    */
    template< typename TTarget >
    void Initialize( TDevice< TTarget >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Initialize( this, pDevice );
    }

    /**
    * @brief シェーダーコンテナーを初期化します。
    *
    * @tparam TTarget 対象の低レベルグラフィックス API
    *
    * @param[in] pDevice デバイスへのポインター
    * @param[in] pMemoryPool シェーダーコンテナーが配置されているメモリープール
    * @param[in] memoryPoolOffset pMemoryPool におけるシェーダーコンテナーが配置されているバイトでのオフセット
    * @param[in] memoryPoolSize pMemoryPool のバイトでのプールサイズ
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    *
    * @post
    * - シェーダーコンテナーが初期化されている
    *
    * @details
    * pMemoryPool == NULL の場合、シェーダーコンテナーがもっているメモリープールを使って初期化します。
    */
    template< typename TTarget >
    void Initialize( TDevice< TTarget >* pDevice, TMemoryPool< TTarget >* pMemoryPool,
        ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT
    {
        return Impl::Initialize( this, pDevice, pMemoryPool, memoryPoolOffset, memoryPoolSize );
    }

    /**
    * @brief シェーダーコンテナーを破棄します。
    *
    * @tparam TTarget 対象の低レベルグラフィックス API
    *
    * @param[in] pDevice デバイスへのポインター
    *
    * @pre
    * - pDevice != NULL
    * - pDevice が初期化されている
    * - シェーダーコンテナーが初期化されている
    *
    * @post
    * - シェーダーコンテナーが初期化されていない
    */
    template< typename TTarget >
    void Finalize( TDevice< TTarget >* pDevice ) NN_NOEXCEPT
    {
        return Impl::Finalize( this, pDevice );
    }

    /**
    * @brief シェーダープログラムリソースを取得します。
    *
    * @param[in] index 取得するシェーダープログラムリソースのバリエーションインデックス
    *
    * @return シェーダープログラムリソースを返します。
    *
    * @pre
    * - index >= 0 && index < GetShaderVariationCount()
    */
    ResShaderProgram* GetResShaderProgram( int index ) NN_NOEXCEPT
    {
        ResShaderVariation* pResShaderVariation = GetResShaderVariation( index );
        NN_SDK_ASSERT_NOT_NULL( pResShaderVariation );
        return pResShaderVariation->GetResShaderProgram(
            static_cast< ShaderCodeType >( this->targetCodeType ) );
    }

    /**
    * @brief シェーダープログラムリソースを取得します。
    *
    * @param[in] index 取得するシェーダープログラムリソースのバリエーションインデックス
    *
    * @return シェーダープログラムリソースを返します。
    *
    * @pre
    * - index >= 0 && index < GetShaderVariationCount()
    */
    const ResShaderProgram* GetResShaderProgram( int index ) const NN_NOEXCEPT
    {
        const ResShaderVariation* pResShaderVariation = GetResShaderVariation( index );
        NN_SDK_ASSERT_NOT_NULL( pResShaderVariation );
        return pResShaderVariation->GetResShaderProgram(
            static_cast< ShaderCodeType >( this->targetCodeType ) );
    }

    /**
    * @brief シェーダーバリエーションリソースを取得します。
    *
    * @param[in] index 取得するシェーダーバリエーションリソースのバリエーションインデックス
    *
    * @return シェーダーバリエーションリソースを返します。
    *
    * @pre
    * - index >= 0 && index < GetShaderVariationCount()
    */
    ResShaderVariation* GetResShaderVariation( int index ) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE( index, 0, static_cast< int >( this->shaderVariationCount ) );
        NN_SDK_ASSERT( this->pShaderVariationArray.Get() );
        return ResShaderVariation::ToAccessor( this->pShaderVariationArray.Get() + index );
    }

    /**
    * @brief シェーダーバリエーションリソースを取得します。
    *
    * @param[in] index 取得するシェーダーバリエーションリソースのバリエーションインデックス
    *
    * @return シェーダーバリエーションリソースを返します。
    *
    * @pre
    * - index >= 0 && index < GetShaderVariationCount()
    */
    const ResShaderVariation* GetResShaderVariation( int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE( index, 0, static_cast< int >( this->shaderVariationCount ) );
        NN_SDK_ASSERT( this->pShaderVariationArray.Get() );
        return ResShaderVariation::ToAccessor( this->pShaderVariationArray.Get() + index );
    }

    /**
    * @brief シェーダーバリエーションの数を取得します。
    *
    * @return シェーダーバリエーションの数を返します。
    */
    int GetShaderVariationCount() const NN_NOEXCEPT
    {
        return static_cast< int >( this->shaderVariationCount );
    }
};

/**
* @brief シェーダーリソースファイルです。
*/
class ResShaderFile
    : public nn::util::AccessorBase< ResShaderFileData >
{
    NN_DISALLOW_COPY( ResShaderFile );

public:
    /**
    * @brief シェーダーリソースファイルのシグネチャーです。
    */
    static const int64_t Signature = NN_UTIL_CREATE_SIGNATURE_8( 'B', 'N', 'S', 'H', 0, 0, 0, 0 );

    /**
    * @brief シェーダーリソースファイルのメジャーバージョンです。
    */
    static const int MajorVersion = 2;

    /**
    * @brief シェーダーリソースファイルのマイナーバージョンです。
    */
    static const int MinorVersion = 1;

    /**
    * @brief シェーダーリソースファイルのマイクロバージョンです。
    */
    static const int MicroVersion = 11;

    /**
    * @brief シェーダーファイルリソースに要求されるアライメントの最大値を取得します。
    *
    * @return シェーダーファイルリソースに要求されるアライメントの最大値を返します。
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
    static ResShaderFile* ToAccessor( ResShaderFile::value_type* pData ) NN_NOEXCEPT
    {
        return static_cast< ResShaderFile* >( pData );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] data 変換するデータへの参照
    *
    * @return 変換されたアクセサーへの参照を返します。
    */
    static ResShaderFile& ToAccessor( ResShaderFile::value_type& data ) NN_NOEXCEPT
    {
        return static_cast< ResShaderFile& >( data );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] pData 変換するデータへのポインター
    *
    * @return 変換されたアクセサーへのポインターを返します。
    */
    static const ResShaderFile* ToAccessor( const ResShaderFile::value_type* pData ) NN_NOEXCEPT
    {
        return static_cast< const ResShaderFile* >( pData );
    }

    /**
    * @brief データをアクセサーに変換します。
    *
    * @param[in] data 変換するデータへの参照
    *
    * @return 変換されたアクセサーへの参照を返します。
    */
    static const ResShaderFile& ToAccessor( const ResShaderFile::value_type& data ) NN_NOEXCEPT
    {
        return static_cast< const ResShaderFile& >( data );
    }

    /**
    * @brief バイナリーデータがシェーダーファイルとして有効かどうかを取得します。
    *
    * @param[in] ptr バイナリーデータへのポインター
    *
    * @return 指定されたバイナリーデータがシェーダーファイルとして有効かどうかの真偽値を返します。
    *
    * @pre
    * - ptr != NULL
    */
    static bool IsValid( const void* ptr ) NN_NOEXCEPT;

    /**
    * @brief バイナリーデータをシェーダーリソースファイル型にキャストします。
    *
    * @param[in] ptr キャストするバイナリーデータへのポインター
    *
    * @return キャストされたシェーダーリソースファイルへのポインターを返します。
    *
    * @pre
    * - ptr != NULL
    *
    * @details
    * 直接キャストせずにこの関数を通してキャストしてください。
    */
    static ResShaderFile* ResCast( void* ptr ) NN_NOEXCEPT;

    /**
    * @brief シェーダーコンテナーリソースを取得します。
    *
    * @return シェーダーコンテナーリソースへのポインターを返します。
    */
    ResShaderContainer* GetShaderContainer() NN_NOEXCEPT
    {
        return ResShaderContainer::ToAccessor( reinterpret_cast<
            ResShaderContainerData* >( this->fileHeader.GetFirstBlock() ) );
    }

    /**
    * @brief シェーダーコンテナーリソースを取得します。
    *
    * @return シェーダーコンテナーリソースへのポインターを返します。
    */
    const ResShaderContainer* GetShaderContainer() const NN_NOEXCEPT
    {
        return ResShaderContainer::ToAccessor( reinterpret_cast<
            const ResShaderContainerData* >( this->fileHeader.GetFirstBlock() ) );
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

/**
* @brief 指定されたシェーダーの中から最大の推奨シェーダースクラッチメモリーを取得します。
*
* @tparam TTarget 対象の低レベルグラフィックス API
*
* @param[in] pDevice デバイスへのポインター
* @param[in] ppResShaderFileArray シェーダーファイルリソースのポインターの配列へのポインター
* @param[in] shaderFileCount pResShaderFileArray の配列の要素数
*
* @return 指定されたシェーダーの中からバイトでの最大の推奨シェーダースクラッチメモリーを返します。
*
* @pre
* - pDevice != NULL
* - pDevice が初期化されている
* - shaderFileCount > 0 の場合、pResShaderFileArray != NULL
*
* @details
* Nvn 版以外ではこの関数を呼ぶことはできません。
*/
template< typename TTarget >
size_t NvnGetMaxRecommendedScratchMemorySize( TDevice< TTarget >* pDevice,
    const ResShaderFile* const* ppResShaderFileArray, int shaderFileCount ) NN_NOEXCEPT;

}
}
