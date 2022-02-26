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
#include <nn/util/util_MemorySplitter.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResShader.h>
#include <nn/g3d/detail/g3d_Flag.h>

namespace nn { namespace g3d {

//! @brief シェーディングモデルインスタンスです。
class ShadingModelObj
{
public:
    class Builder;

    //! @brief インスタンスの構築時に渡すバッファーの必要アライメントサイズです。
    enum Alignment
    {
        Alignment_Buffer = 8, //!< @brief Builder に渡すバッファーの必要アライメントサイズです。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    //!
    //! 実際の構築処理は Builder で行います。
    //!
    ShadingModelObj()  NN_NOEXCEPT
        : m_pRes(NULL)
        , m_Flag(0)
        , m_pShaderKey(NULL)
        , m_pOptionKey(NULL)
        , m_ShaderRange()
        , m_pMemoryPool(NULL)
        , m_MemoryPoolOffset(0)
        , m_pOptionBlockArray(NULL)
        , m_OptionBlockSize(0)
        , m_BufferingCount(0)
        , m_DirtyFlagSet()
        , m_pUserPtr(NULL)
        , m_pBufferPtr(NULL)
    {
    }

    //! @brief ユニフォームブロックのサイズを計算します。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @return 計算したバッファーサイズを返します。
    //!
    size_t CalculateBlockBufferSize(nn::gfx::Device* pDevice);

    //! @brief メモリプールを渡して、ユニフォームブロックを構築します。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @param[in] pMemoryPool メモリプールへのポインター。
    //! @param[in] offset メモリプールの使用する領域へのオフセット。
    //! @param[in] memoryPoolSize offset以降で使用可能なメモリプールのサイズ。
    //!
    //! @return 構築に成功した場合は true、失敗した場合は false を返します。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - pMemoryPool が初期化されている。
    //! - offset で指されたメモリプールの位置が nn::gfx::Buffer::GetBufferAlignment() のアラインメント上にある。
    //! - memoryPoolSize は CalculateBlockBufferSize() 以上である。
    //! - ユニフォームブロックが未構築である。
    //!
    //! @post
    //! - ユニフォームブロックが構築されている
    //!
    bool SetupBlockBuffer(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t offset, size_t memoryPoolSize) NN_NOEXCEPT;

    //! @brief ユニフォームブロックを破棄します。
    //!
    //! @param[in] pDevice デバイスへのポインター
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - ユニフォームブロックが構築済みである。
    //!
    //! @post
    //!- ユニフォームブロックが破棄されている。
    //!
    void CleanupBlockBuffer(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    //! @brief シェーダープログラムの範囲を更新します。
    //!
    //! @return 検索が成功した場合は true、失敗した場合は false を返します。
    //!
    bool UpdateShaderRange() NN_NOEXCEPT
    {
        return m_pRes->FindProgramRange(&m_ShaderRange, m_pShaderKey);
    }

    //! @brief シェーダーオプションの描画リソースを計算します。
    //!
    //! @details
    //! GPU から参照されるバッファーを書き換えるため、
    //! 前回の描画で GPU が参照し終わった後に呼ぶ必要があります。
    //! フレームバッファーのコピーアウト中などに呼ぶことを想定しています。
    //!
    void CalculateOptionBlock() NN_NOEXCEPT
    {
        CalculateOptionBlock(0);
    }

    //! @brief シェーダーオプションの描画リソースを計算します。
    //!
    //! @param[in] bufferIndex ユニフォームブロックのインデックス。
    //!
    //! @details
    //! GPU から参照されるバッファーを書き換えるため、
    //! 前回の描画で GPU が参照し終わった後に呼ぶ必要があります。
    //!
    //! @pre
    //! - インデックスはシェーディングモデルが持つバッファー数の範囲内。
    //!
    void CalculateOptionBlock(int bufferIndex) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name オプション
    //@{

    //! @brief シェーディングモデル配下の Static オプションの数を取得します。
    //!
    //! @return シェーディングモデル配下の Static オプションの数を返します。
    //!
    int GetStaticOptionCount() const NN_NOEXCEPT
    {
        return m_pRes->GetStaticOptionCount();
    }

    //! @brief インデックスから Static オプション名を取得します。
    //!
    //! @param[in] optionIndex Static オプションのインデックス。
    //!
    //! @return Static オプション名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーディングモデル配下の Static オプションの数の範囲内。
    //!
    const char* GetStaticOptionName(int optionIndex) const NN_NOEXCEPT
    {
        return m_pRes->GetStaticOptionName(optionIndex);
    }

    //! @brief Static オプションの名前からインデックスを取得します。
    //!
    //! @param[in] name Static オプション名へのポインター。
    //!
    //! @return Static オプションのインデックスを返します。
    //!
    //! @details
    //! 指定した名前の Static オプション情報が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindStaticOptionIndex(const char* name) const NN_NOEXCEPT
    {
        return m_pRes->FindStaticOptionIndex(name);
    }

    //! @brief インデックス引きで Static オプションを取得します。
    //!
    //! @param[in] optionIndex Static オプションのインデックス。
    //!
    //! @return ResShaderOption へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーディングモデル配下の Static オプションの数の範囲内。
    //!
    const ResShaderOption* GetStaticOption(int optionIndex) const NN_NOEXCEPT
    {
        return m_pRes->GetStaticOption(optionIndex);
    }

    //! @brief 名前引きで Static オプションを取得します。
    //!
    //! @param[in] name Static オプション名へのポインター。
    //!
    //! @return Static オプションのインデックスを返します。
    //!
    //! @details
    //! 指定した名前の Static オプションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResShaderOption* FindStaticOption(const char* name) const NN_NOEXCEPT
    {
        return m_pRes->FindStaticOption(name);
    }

    //! @brief シェーダーキーをデフォルトに書き換えます。
    void ClearStaticKey();

    //! @brief 指定した Choice でシェーダーキーを書き換えます。
    //!
    //! @param[in] optionIndex Static オプションのインデックス。
    //! @param[in] choiceIndex Choice の値。
    //!
    void WriteStaticKey(int optionIndex, int choiceIndex) NN_NOEXCEPT;

    //! @brief シェーダーキーから Choice を読み込みます。
    //!
    //! @param[in] optionIndex Static オプションのインデックス。
    //!
    //! @return Choice の値を返します。
    //!
    int ReadStaticKey(int optionIndex) const NN_NOEXCEPT;

    //! @brief シェーダーキーを取得します。
    //!
    //! @return シェーダーキーを返します。
    //!
    //! @details
    //! シェーダーキーはシェーダープログラムの探索に使用するキーです。
    //! ブランチ機能を有効にしたオプションの値は反映されません。
    //!
    const Bit32* GetStaticKey() const NN_NOEXCEPT
    {
        return m_pShaderKey;
    }

    //! @brief シェーダープログラムの検索に使用する Static オプションのキーの Bit32 換算の長さを取得します。
    //!
    //! @return シェーダープログラムの検索に使用する Static オプションのキーの Bit32 換算の長さを返します。
    //!
    int GetStaticKeyLength() const NN_NOEXCEPT
    {
        return m_pRes->GetStaticKeyLength();
    }

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief ユニフォームブロックが要求するアライメントを取得します。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @return ユニフォームブロックが要求するアライメントを返します。
    //!
    size_t GetBlockBufferAlignment(nn::gfx::Device* pDevice) const NN_NOEXCEPT;

    //! @brief リソースを取得します。
    //!
    //! @return ResShadingModel のポインターを返します。
    //!
    const ResShadingModel* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    }

    //! @brief 現在のシェーダープログラムの範囲を取得します。
    //!
    //! @return 現在のシェーダープログラムの範囲を返します。
    //!
    const ShaderRange& GetShaderRange() const NN_NOEXCEPT
    {
        return m_ShaderRange;
    }

    //! @brief Builder で Build 時に渡されたバッファーを取得します。
    //!
    //! @return Builder で Build 時に渡されたバッファーへのポインターを返します。
    //!
    void* GetBufferPtr() NN_NOEXCEPT
    {
        return m_pBufferPtr;
    }

    //! @brief SetupBlockBuffer() 時に渡されたメモリプールのポインターを取得します。
    //!
    //! @return メモリプールへのポインターを返します。
    //!
    nn::gfx::MemoryPool* GetMemoryPoolPtr() NN_NOEXCEPT
    {
        return m_pMemoryPool;
    }

    //! @brief SetupBlockBuffer() 時に渡されたメモリプールのオフセットを取得します。
    //!
    //! @return メモリプールのオフセットを返します。
    //!
    ptrdiff_t GetMemoryPoolOffset() NN_NOEXCEPT
    {
        return m_MemoryPoolOffset;
    }

    //! @brief モデル配下のオブジェクトのユニフォームブロックが構築済みであるかどうかを取得します。
    //!
    //! @return 構築済みの場合は true、未構築の場合は false を返します。
    //!
    bool IsBlockBufferValid() const NN_NOEXCEPT
    {
        return (m_Flag & Flag_BlockBufferValid) != 0;
    }

    //! @brief シェーディングモデルが保持するユニフォームブロックのバッファーを取得します。
    //!
    //! @return nn::gfx::Buffer 配列の先頭のポインターを返します。
    //!
    nn::gfx::Buffer* GetOptionBlock() NN_NOEXCEPT
    {
        return GetOptionBlock(0);
    }

    //! @brief シェーディングモデルが保持するユニフォームブロックのバッファーを取得します。
    //!
    //! @param[in] bufferIndex ユニフォームブロックのインデックス。
    //!
    //! @return nn::gfx::Buffer へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーディングモデルが持つバッファー数の範囲内。
    //!
    nn::gfx::Buffer* GetOptionBlock(int bufferIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(bufferIndex, 0, GetBufferingCount());
        return m_pOptionBlockArray ? &m_pOptionBlockArray[bufferIndex] : NULL;
    }

    //! @brief シェーディングモデルが保持するユニフォームブロックのバッファーを取得します。
    //!
    //! @return nn::gfx::Buffer へのポインターを返します。
    //!
    const nn::gfx::Buffer* GetOptionBlock() const NN_NOEXCEPT
    {
        return GetOptionBlock(0);
    }

    //! @brief シェーディングモデルが保持するユニフォームブロックのバッファーを取得します。
    //!
    //! @param[in] bufferIndex ユニフォームブロックのインデックス。
    //!
    //! @return nn::gfx::Buffer へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーディングモデルが持つバッファー数の範囲内。
    //!
    const nn::gfx::Buffer* GetOptionBlock(int bufferIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(bufferIndex, 0, GetBufferingCount());
        return m_pOptionBlockArray ? &m_pOptionBlockArray[bufferIndex] : NULL;
    }

    //! @brief シェーディングモデルが保持するユニフォームブロックのサイズを取得します。
    //!
    //! @return ユニフォームブロックのサイズを返します。
    //!
    const size_t GetOptionBlockSize() const NN_NOEXCEPT
    {
        int idxBlock = m_pRes->GetSystemBlockIndex(ResUniformBlockVar::Type_Option);
        if (idxBlock < 0)
        {
            return 0;
        }
        else
        {
            const ResUniformBlockVar* pBlock = m_pRes->GetUniformBlock(idxBlock);
            return pBlock->GetSize();
        }
    }

    //! @brief ユニフォームブロックのバッファーリング数を取得します。
    //!
    //! @return ユニフォームブロックのバッファーリング数を返します。
    //!
    int GetBufferingCount() const NN_NOEXCEPT
    {
        return m_BufferingCount;
    }

    //! @brief ユーザーポインターを設定します。
    //!
    //! @param[in] pUserPtr ポインター。
    //!
    void SetUserPtr(void* pUserPtr) NN_NOEXCEPT
    {
        m_pUserPtr = pUserPtr;
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    void* GetUserPtr() NN_NOEXCEPT
    {
        return m_pUserPtr;
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    const void* GetUserPtr() const NN_NOEXCEPT
    {
        return m_pUserPtr;
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    T* GetUserPtr() NN_NOEXCEPT
    {
        return static_cast<T*>(m_pUserPtr);
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    const T* GetUserPtr() const NN_NOEXCEPT
    {
        return static_cast<const T*>(m_pUserPtr);
    }

    //! @brief シェーディングモデル名を取得します。
    //!
    //! @return ResShadingModel のシェーディングモデル名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return m_pRes->GetName();
    }

    //@}

    //----------------------------------------
    //! @name デバッグ用
    //@{

    //! @brief キーを文字列に変換します。
    //!
    //! @param[out] pStr 文字列を格納するバッファーへのポインター。
    //! @param[in] strLength 文字列バッファーのサイズ。
    //!
    //! @return 書き込んだ文字列数を返します。
    //!
    //! @details
    //! 戻り値は終端文字を含まない文字列長です。pStr が NULL でない場合は必ず終端されます。
    //! strLength が必要なサイズに満たない場合は戻り値は負になります。
    //! pStr を NULL、strLength を 0 にすることで終端文字を含まない文字列長を返します。
    //!
    int PrintKeyTo(char* pStr, int strLength) const NN_NOEXCEPT;

    //! @brief バリエーション探索に使用するキーを文字列に変換します。
    //!
    //! @param[out] pStr 文字列を格納するバッファーへのポインター。
    //! @param[in] strLength 文字列バッファーのサイズ。
    //!
    //! @return 書き込んだ文字列数を返します。
    //!
    //! @details
    //! 戻り値は終端文字を含まない文字列長です。pStr が NULL でない場合は必ず終端されます。
    //! strLength が必要なサイズに満たない場合は戻り値は負になります。
    //! pStr を NULL、strLength を 0 にすることで終端文字を含まない文字列長を返します。
    //!
    int PrintRawKeyTo(char* pStr, int strLength) const NN_NOEXCEPT;

    //! @brief キーのオプション表現を文字列に変換します。
    //!
    //! @param[out] pStr 文字列を格納するバッファーへのポインター。
    //! @param[in] strLength 文字列バッファーのサイズ。
    //!
    //! @return 書き込んだ文字列数を返します。
    //!
    //! @details
    //! 戻り値は終端文字を含まない文字列長です。pStr が NULL でない場合は必ず終端されます。
    //! strLength が必要なサイズに満たない場合は戻り値は負になります。
    //! pStr を NULL、strLength を 0 にすることで終端文字を含まない文字列長を返します。
    //!
    int PrintOptionTo(char* pStr, int strLength) const NN_NOEXCEPT;

    //! @brief バリエーション探索に使用するキーのオプション表現を文字列に変換します。
    //!
    //! @param[out] pStr 文字列を格納するバッファーへのポインター。
    //! @param[in] strLength 文字列バッファーのサイズ。
    //!
    //! @return 書き込んだ文字列数を返します。
    //!
    //! @details
    //! 戻り値は終端文字を含まない文字列長です。pStr が NULL でない場合は必ず終端されます。
    //! strLength が必要なサイズに満たない場合は戻り値は負になります。
    //! pStr を NULL、strLength を 0 にすることで終端文字を含まない文字列長を返します。
    //!
    int PrintRawOptionTo(char* pStr, int strLength) const NN_NOEXCEPT;

    //@}

    //! @briefprivate ユニフォームブロックのバッファー配列を設定します。
    //!
    //! @param[in] pBlockArray バッファー配列へのポインター。
    //!
    void SetOptionBlockArray(nn::gfx::Buffer* pBlockArray) NN_NOEXCEPT
    {
        m_pOptionBlockArray = pBlockArray;
    }

    //! @briefprivate ユニフォームブロックのバッファー配列を取得します。
    //!
    //! @return バッファー配列へのポインターを返します。
    //!
    nn::gfx::Buffer* GetOptionBlockArray() NN_NOEXCEPT
    {
        return m_pOptionBlockArray;
    }

protected:

    //! @brief モデルの状態を表すフラグです。
    enum Flag
    {
        Flag_BlockBufferValid = 0x1 << 0, //!< @briefprivate ユニフォームブロックが構築済みであるかどうかを表すフラグです。
    };

    //! @brief シェーダーオプションに DirtyFlag を設定します。
    //!
    //! @param[in] optionIndex Static オプションのインデックス。
    //!
    //! @details
    //! DirtyFlag が設定されたシェーダーパラメータは、
    //! CalculateOptionBlock() を呼び出すことでユニフォームブロックに反映されます。
    //!
    void SetDirtyFlag(int optionIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(optionIndex, 0, GetStaticOptionCount(), NN_G3D_RES_GET_NAME(m_pRes, GetName()));
        m_DirtyFlagSet.Set(optionIndex);
    }

private:
    class InitializeArgument;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;
    void SetupBlockBufferImpl(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t offset, size_t memoryPoolSize) NN_NOEXCEPT;

    const ResShadingModel* m_pRes;

    Bit32 m_Flag;
    Bit32* m_pShaderKey;
    Bit32* m_pOptionKey; // UniformBlock を使用するオプションの値を記録しておくキー。
    ShaderRange m_ShaderRange;
    nn::gfx::MemoryPool* m_pMemoryPool;
    ptrdiff_t            m_MemoryPoolOffset;
    nn::gfx::Buffer* m_pOptionBlockArray;
    size_t m_OptionBlockSize;
    uint8_t m_BufferingCount;
    FlagSet m_DirtyFlagSet;

    void* m_pUserPtr;
    void* m_pBufferPtr;

    NN_DISALLOW_COPY(ShadingModelObj);
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate ShadingModelObj::Initialize() に渡して初期化を行うパラメータです。
class ShadingModelObj::InitializeArgument
{
public:
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_ShaderKey,
        MemoryBlockIndex_OptionKey,
        MemoryBlockIndex_DirtyFlagSet,
        MemoryBlockIndex_Buffer,
        MemoryBlockIndex_End
    };

    //! コンストラクタです。
    //!
    //! @param[in] resource ResShadingModel へのポインター。
    //!
    explicit InitializeArgument(const ResShadingModel* resource) NN_NOEXCEPT
        : m_pRes(resource)
        , m_MemoryBlock()
        , m_BufferingCount(1)
    {
        NN_SDK_REQUIRES_NOT_NULL(resource);
        m_WorkMemory.Invalidate();
    }

    //! @brief リソースを取得します。
    //!
    //! @return ResShadingModel へのポインターを返します。
    //!
    const ResShadingModel* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    }

    //! @brief 構築に必要なメモリサイズを取得します。
    //!
    //! @return ShadingModelObj 構築に必要なメモリサイズを返します。
    //!
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief ShadingModelObj 構築に必要なメモリサイズを計算します。
    //!
    //! @pre
    //! - 設定されたユニフォームブロックのバッファーリング数が１以上。
    //!
    void CalculateMemorySize() NN_NOEXCEPT;

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    void* GetBuffer( void* pHead, int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return m_MemoryBlock[index].Get(pHead);
    }

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    template< typename T >
    T* GetBuffer( void* pHead, int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return static_cast<T*>( GetBuffer(pHead, index) );
    }

    //! @brief ShadingModelObj 構築に必要なメモリサイズが計算済みかを取得します。
    //!
    //! @return 計算済みの場合は true、未計算の場合は false を返します。
    //!
    bool IsMemoryCalculated() const NN_NOEXCEPT
    {
        return m_WorkMemory.IsValid();
    }

    //! @brief ユニフォームブロックのバッファーリング数を設定します。
    //!
    //! @param[in] count ユニフォームブロックのバッファーリング数。
    //!
    void BufferingCount(int count) NN_NOEXCEPT
    {
        m_BufferingCount = count;
    }

    //! @brief ユニフォームブロックのバッファーリング数を取得します。
    //!
    //! @return ユニフォームブロックのバッファーリング数を返します。
    //!
    int GetBufferingCount() const NN_NOEXCEPT
    {
        return m_BufferingCount;
    }

    //! @briefprivate インデックスで指定されたバッファーのサイズを取得します。
    size_t GetBufferSize( int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return m_MemoryBlock[index].GetSize();
    }

private:
    const ResShadingModel* m_pRes;
    nn::util::MemorySplitter              m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
    int                                   m_BufferingCount;
};

//--------------------------------------------------------------------------------------------------

//! @brief ShadingModelObj の構築を行うクラスです。
class ShadingModelObj::Builder : public ShadingModelObj::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @param[in] resource ResShadingModel へのポインター
    //!
    explicit Builder(const ResShadingModel* resource) NN_NOEXCEPT
        : InitializeArgument(resource)
    {
        NN_SDK_REQUIRES_NOT_NULL(resource);
    }

    //! @brief ShadingModelObj を構築します。
    //!
    //! @param[in] pShadingModelObj ShadingModelObj へのポインター。
    //! @param[in] pBuffer バッファーへのポインター。
    //! @param[in] bufferSize バッファーのサイズ。
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである。
    //! - pBufferはAlignment_Buffer のアライメント上にある。
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ。
    //!
    bool Build(ShadingModelObj* pShadingModelObj, void* pBuffer, size_t bufferSize) const NN_NOEXCEPT
    {
        return pShadingModelObj->Initialize(*this, pBuffer, bufferSize);
    }
};

//--------------------------------------------------------------------------------------------------

//! @brief 動的シェーダーバリエーションのセレクタです。
class ShaderSelector
{
public:
    class Builder;

    //! @brief インスタンスの構築時に渡すバッファーの必要アライメントサイズです。
    enum Alignment
    {
        Alignment_Buffer = 8 //!< @brief Builder に渡すバッファーの必要アライメントサイズです。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 実際の構築処理は Builder で行います。
    //!
    ShaderSelector() NN_NOEXCEPT
        : m_pShadingModel(NULL)
        , m_pShaderKey(NULL)
        , m_pOptionKey(NULL)
        , m_pLastShaderKey(NULL)
        , m_pProgram(NULL)
        , m_pBufferPtr(NULL)
    {
    }

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    //! @brief シェーダーバリエーションの更新を行います。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @return 更新が成功した場合は true、失敗した場合は false を返します。
    //!
    bool UpdateVariation(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name オプション
    //@{

    //! @brief シェーディングモデル配下の Dynamic オプションの数を取得します。
    //!
    //! @return シェーディングモデル配下の Dynamic オプションの数を返します。
    //!
    int GetDynamicOptionCount() const NN_NOEXCEPT
    {
        return m_pShadingModel->GetResource()->GetDynamicOptionCount();
    }

    //! @brief インデックスから Dynamic オプション名を取得します。
    //!
    //! @param[in] optionIndex Dynamic オプションのインデックス。
    //!
    //! @return Dynamic オプション名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーディングモデル配下の Dynamic オプションの数の範囲内。
    //!
    const char* GetDynamicOptionName(int optionIndex) const NN_NOEXCEPT
    {
        return m_pShadingModel->GetResource()->GetDynamicOptionName(optionIndex);
    }

    //! @brief Dynamic オプションの名前からインデックスを取得します。
    //!
    //! @param[in] name Dynamic オプション名へのポインター。
    //!
    //! @return Dynamic オプションのインデックスを返します。
    //!
    //! @details
    //! 指定した名前の Static オプション情報が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindDynamicOptionIndex(const char* name) const NN_NOEXCEPT
    {
        return m_pShadingModel->GetResource()->FindDynamicOptionIndex(name);
    }

    //! @brief インデックス引きで Dynamic オプションを取得します。
    //!
    //! @param[in] optionIndex Dynamic オプションのインデックス。
    //!
    //! @return ResShaderOption へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーディングモデル配下の Dynamic オプションの数の範囲内。
    //!
    const ResShaderOption* GetDynamicOption(int optionIndex) const NN_NOEXCEPT
    {
        return m_pShadingModel->GetResource()->GetDynamicOption(optionIndex);
    }

    //! @brief 名前引きで Dynamic オプションを取得します。
    //!
    //! @param[in] name Dynamic オプション名へのポインター。
    //!
    //! @return Dynamic オプションのインデックスを返します。
    //!
    //! @details
    //! 指定した名前の Dynamic オプションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResShaderOption* FindDynamicOption(const char* name) const NN_NOEXCEPT
    {
        return m_pShadingModel->GetResource()->FindDynamicOption(name);
    }

    //! @brief シェーダーキーをデフォルトに書き換えます。
    void ClearDynamicKey();

    //! @brief 指定した Choice でシェーダーキーを書き換えます。
    //!
    //! @param[in] optionIndex Dynamic オプションのインデックス。
    //! @param[in] choiceIndex Choice の値。
    //!
    void WriteDynamicKey(int optionIndex, int choiceIndex) NN_NOEXCEPT;

    //! @brief シェーダーキーから Choice を読み込みます。
    //!
    //! @param[in] optionIndex Dynamic オプションのインデックス。
    //!
    //! @return Choice の値を返します。
    //!
    int ReadDynamicKey(int optionIndex) const NN_NOEXCEPT;

    //! @brief シェーダーキーを取得します。
    //!
    //! @return シェーダーキーを返します。
    //!
    //! @details
    //! シェーダープログラムの探索に使用するキーを返します。
    //! ブランチ機能を有効にしたオプションの値は反映されません。
    //!
    const Bit32* GetDynamicKey() const NN_NOEXCEPT
    {
        return m_pShaderKey;
    }

    //! @brief シェーダープログラムの検索に使用する Dynamic オプションのキーの Bit32 換算の長さを取得します。
    //!
    //! @return シェーダープログラムの検索に使用する Dynamic オプションのキーの Bit32 換算の長さを返します。
    //!
    int GetDynamicKeyLength() const NN_NOEXCEPT
    {
        return m_pShadingModel->GetResource()->GetDynamicKeyLength();
    }

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief シェーディングモデルを取得します。
    //!
    //! @return ShadingModelObj へのポインターを返します。
    //!
    ShadingModelObj* GetShadingModel() NN_NOEXCEPT
    {
        return m_pShadingModel;
    }

    //! @brief シェーディングモデルを取得します。
    //!
    //! @return ShadingModelObj へのポインターを返します。
    //!
    const ShadingModelObj* GetShadingModel() const NN_NOEXCEPT
    {
        return m_pShadingModel;
    }

    //! @brief 現在のシェーダープログラムを取得します。
    //!
    //! @return ResShaderProgram へのポインターを返します。
    //!
    const ResShaderProgram* GetProgram() const NN_NOEXCEPT
    {
        return m_pProgram;
    }

    //! @brief デフォルトのシェーダープログラムを取得します。
    //!
    //! @return ResShaderProgram へのポインターを返します。
    //!
    const ResShaderProgram* GetDefaultProgram() const NN_NOEXCEPT
    {
        const ResShadingModel* pRes = m_pShadingModel->GetResource();
        int idxProgram = pRes->GetDefaultProgramIndex();
        return idxProgram == ShaderLocationProgramNone ? NULL : pRes->GetShaderProgram(idxProgram);
    }

    //! @brief Builder で Build 時に渡されたバッファーを取得します。
    //!
    //! @return Builder で Build 時に渡されたバッファーへのポインターを返します。
    //!
    void* GetBufferPtr() NN_NOEXCEPT
    {
        return m_pBufferPtr;
    }

    //@}

    //----------------------------------------
    //! @name デバッグ用
    //@{

    //! @brief キーを文字列に変換します。
    //!
    //! @param[out] pStr 文字列を格納するバッファーへのポインター。
    //! @param[in] strLength 文字列バッファーのサイズ。
    //!
    //! @return 書き込んだ文字列数を返します。
    //!
    //! @details
    //! 戻り値は終端文字を含まない文字列長です。pStr が NULL でない場合は必ず終端されます。
    //! strLength が必要なサイズに満たない場合は戻り値は負になります。
    //! pStr を NULL、strLength を 0 にすることで終端文字を含まない文字列長を返します。
    //!
    int PrintKeyTo(char* pStr, int strLength) const NN_NOEXCEPT;

    //! @brief バリエーション探索に使用するキーを文字列に変換します。
    //!
    //! @param[out] pStr 文字列を格納するバッファーへのポインター。
    //! @param[in] strLength 文字列バッファーのサイズ。
    //!
    //! @return 書き込んだ文字列数を返します。
    //!
    //! @details
    //! 戻り値は終端文字を含まない文字列長です。pStr が NULL でない場合は必ず終端されます。
    //! strLength が必要なサイズに満たない場合は戻り値は負になります。
    //! pStr を NULL、strLength を 0 にすることで終端文字を含まない文字列長を返します。
    //!
    int PrintRawKeyTo(char* pStr, int strLength) const NN_NOEXCEPT;

    //! @brief キーのオプション表現を文字列に変換します。
    //!
    //! @param[out] pStr 文字列を格納するバッファーへのポインター。
    //! @param[in] strLength 文字列バッファーのサイズ。
    //!
    //! @return 書き込んだ文字列数を返します。
    //!
    //! @details
    //! 戻り値は終端文字を含まない文字列長です。pStr が NULL でない場合は必ず終端されます。
    //! strLength が必要なサイズに満たない場合は戻り値は負になります。
    //! pStr を NULL、strLength を 0 にすることで終端文字を含まない文字列長を返します。
    //!
    int PrintOptionTo(char* pStr, int strLength) const NN_NOEXCEPT;

    //! @brief バリエーション探索に使用するキーのオプション表現を文字列に変換します。
    //!
    //! @param[out] pStr 文字列を格納するバッファーへのポインター。
    //! @param[in] strLength 文字列バッファーのサイズ。
    //!
    //! @return 書き込んだ文字列数を返します。
    //!
    //! @details
    //! 戻り値は終端文字を含まない文字列長です。pStr が NULL でない場合は必ず終端されます。
    //! strLength が必要なサイズに満たない場合は戻り値は負になります。
    //! pStr を NULL、strLength を 0 にすることで終端文字を含まない文字列長を返します。
    //!
    int PrintRawOptionTo(char* pStr, int strLength) const NN_NOEXCEPT;

    //@}

private:
    class InitializeArgument;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    ShadingModelObj* m_pShadingModel;

    Bit32* m_pShaderKey;
    Bit32* m_pOptionKey;
    Bit32* m_pLastShaderKey;

    const ResShaderProgram* m_pProgram;

    void* m_pBufferPtr;

    NN_DISALLOW_COPY(ShaderSelector);
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate ShaderSelector::Initialize() に渡して初期化を行うパラメータです。
class ShaderSelector::InitializeArgument
{
public:
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_ShaderKey,
        MemoryBlockIndex_OptionKey,
        MemoryBlockIndex_LastShaderKey,
        MemoryBlockIndex_End
    };

    //! コンストラクタです。
    //!
    //! @param[in] pShadingModel ShadingModelObj へのポインター。
    //!
    explicit InitializeArgument(ShadingModelObj* pShadingModel) NN_NOEXCEPT
        : m_pShadingModel(pShadingModel)
        , m_MemoryBlock()
    {
        NN_SDK_REQUIRES_NOT_NULL(pShadingModel);
        m_WorkMemory.Invalidate();
    }

    //! @brief シェーディングモデルを取得します。
    //!
    //! @return ShadingModelObj へのポインターを返します。
    //!
    ShadingModelObj* GetShadingModel() const NN_NOEXCEPT
    {
        return m_pShadingModel;
    }

    //! @brief 構築に必要なメモリサイズを取得します。
    //!
    //! @return ShaderSelector 構築に必要なメモリサイズを返します。
    //!
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief ShaderSelector 構築に必要なメモリサイズを計算します。
    //!
    //! @pre
    //! - 設定されたユニフォームブロックのバッファーリング数が１以上。
    //!
    void CalculateMemorySize() NN_NOEXCEPT;

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    void* GetBuffer( void* pHead, int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return m_MemoryBlock[index].Get(pHead);
    }

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    template< typename T >
    T* GetBuffer( void* pHead, int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return static_cast<T*>( GetBuffer(pHead, index) );
    }

    //! @brief ShaderSelector 構築に必要なメモリサイズが計算済みかを取得します。
    //!
    //! @return 計算済みの場合は true、未計算の場合は false を返します。
    //!
    bool IsMemoryCalculated() const NN_NOEXCEPT
    {
        return m_WorkMemory.IsValid();
    }

private:
    ShadingModelObj* m_pShadingModel;
    nn::util::MemorySplitter              m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
};

//--------------------------------------------------------------------------------------------------

//! @brief ShaderSelector の構築を行うクラスです。
class ShaderSelector::Builder : public ShaderSelector::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @param[in] pShadingModel ShadingModelObj へのポインター。
    //!
    explicit Builder(ShadingModelObj* pShadingModel) NN_NOEXCEPT
        : InitializeArgument(pShadingModel)
    {
        NN_SDK_REQUIRES_NOT_NULL(pShadingModel);
    }

    //! @brief ShaderSelector を構築します。
    //!
    //! @param[in] pShaderSelector ShaderSelector へのポインター。
    //! @param[in] pBuffer バッファーへのポインター。
    //! @param[in] bufferSize バッファーのサイズ。
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである。
    //! - pBufferはAlignment_Buffer のアライメント上にある。
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ。
    //!
    bool Build(ShaderSelector* pShaderSelector, void* pBuffer, size_t bufferSize) const NN_NOEXCEPT
    {
        return pShaderSelector->Initialize(*this, pBuffer, bufferSize);
    }
};

}} // namespace nn::g3d

