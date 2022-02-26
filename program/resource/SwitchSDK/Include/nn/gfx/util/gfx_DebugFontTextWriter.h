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
#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/util/util_Vector.h>

#if defined(NN_GFX_UTIL_DEBUGFONT_USE_DEFAULT_LOCALE_CHARSET) && defined(NN_BUILD_CONFIG_OS_WIN)
#define NN_GFX_UTIL_DEBUGFONT_USE_DEFAULT_LOCALE_CHARSET_VALUE true
#else
#define NN_GFX_UTIL_DEBUGFONT_USE_DEFAULT_LOCALE_CHARSET_VALUE false
#endif


namespace nn {
namespace gfx {
namespace util {

namespace detail {
class DebugFontTextWriterImpl;
} // namespace detail

//------------------------------------------------------------------------------
//! @brief デバッグフォント描画クラスを初期化するための情報を表すクラスです。
//------------------------------------------------------------------------------
class DebugFontTextWriterInfo
{
    NN_DISALLOW_COPY(DebugFontTextWriterInfo);

public:

    /**
    * @brief コンストラクタです。
    *
    * @post
    * - 各パラメータが既定値で初期化されている
    */
    DebugFontTextWriterInfo() NN_NOEXCEPT
    {
        this->SetDefault();
    }

    /**
    * @brief 各パラメータを既定値に設定するためのヘルパー関数です。
    *
    * @details
    * 設定される値はゼロ初期化とは異なります。
    */
    void SetDefault() NN_NOEXCEPT
    {
        this->SetCharCountMax(1024);
        this->SetBufferCount(2);
        this->SetUserMemoryPoolEnabled(false);
    }

    /**
    * @brief 一度に表示できる最大文字数を設定します。
    *
    * @param[in] charCountMax 一度に表示できる最大文字数です。
    */
    void SetCharCountMax(int charCountMax) NN_NOEXCEPT
    {
        this->m_CharCountMax = charCountMax;
    }

    /**
    * @brief 一度に表示できる最大文字数を取得します。
    *
    * @return 一度に表示できる最大文字数を返します。
    */
    int GetCharCountMax() const NN_NOEXCEPT
    {
        return this->m_CharCountMax;
    }

    /**
    * @brief バッファのマルチバッファリングの数を設定します。
    *
    * @param[in] bufferCount マルチバッファリングする数です。
    */
    void SetBufferCount(int bufferCount) NN_NOEXCEPT
    {
        this->m_BufferCount = bufferCount;
    }

    /**
    * @brief バッファのマルチバッファリングの数を取得します。
    *
    * @return マルチバッファリングする数を返します。
    */
    int GetBufferCount() const NN_NOEXCEPT
    {
        return this->m_BufferCount;
    }

    /**
    * @brief ユーザーからのメモリプールを使用するかどうかを設定します。
    *
    * @param[in] value ユーザーからのメモリプールを使用するかどうかを表す真偽値です。
    */
    void SetUserMemoryPoolEnabled(bool value) NN_NOEXCEPT
    {
        this->m_Flag.SetBit(Flag_UserMemoryPoolEnable, value);
    }

    /**
    * @brief ユーザーからのメモリプールを使用するかどうかを取得します。
    *
    * @return ユーザーからのメモリプールを使用するかどうかを表す真偽値を返します。
    */
    bool IsUserMemoryPoolEnabled() const NN_NOEXCEPT
    {
        return this->m_Flag.GetBit(Flag_UserMemoryPoolEnable);
    }

private:
    enum Flag
    {
        Flag_UserMemoryPoolEnable,
    };

    int m_CharCountMax;               //!< 一度に表示できる最大文字数です。
    int m_BufferCount;                //!< 作成するバッファのマルチバッファリングの数です。
    nn::util::BitPack16 m_Flag;       //!< フラグです。
    NN_PADDING2;
};

//------------------------------------------------------------------------------
//! @brief デバッグフォント描画クラスです。
//------------------------------------------------------------------------------
class DebugFontTextWriter
{
    NN_DISALLOW_COPY(DebugFontTextWriter);

public:
    // コンストラクタ
    DebugFontTextWriter();

    // デストラクタ
    virtual ~DebugFontTextWriter();

    //---------------------------------------------------------------------------
    //! @brief 要求されるメモリのサイズを取得します。
    //! @param[in]    pDevice       デバイスです。
    //! @param[in]    info          初期化用の情報です。
    //! @return                     バイトでの要求されるメモリのサイズを返します。
    //---------------------------------------------------------------------------
    static size_t GetRequiredMemorySize(
        nn::gfx::Device* pDevice,
        const DebugFontTextWriterInfo &info
        );

    //---------------------------------------------------------------------------
    //! @brief 要求されるメモリプールのサイズを取得します。
    //! @param[in]    pDevice       デバイスです。
    //! @param[in]    info          初期化用の情報です。
    //! @return                     バイトでの要求されるメモリプールのサイズを返します。
    //---------------------------------------------------------------------------
    static size_t GetRequiredMemoryPoolSize(
        nn::gfx::Device* pDevice,
        const DebugFontTextWriterInfo &info
        );

    //---------------------------------------------------------------------------
    //! @brief 初期化します。
    //! @param[in]    pDevice               デバイスです。
    //! @param[in]    info                  初期化用の情報です。
    //! @param[in]    pMemory               このクラスで使用するメモリへのポインタです。
    //! @param[in]    memorySize            このクラスで使用するメモリのサイズです。
    //! @param[in]    pMemoryPool           バッファを配置するメモリプールへのポインタです。
    //! @param[in]    memoryPoolOffset      バイトでのバッファを配置するメモリプールへのオフセットです。
    //! @param[in]    memoryPoolSize        バイトでのバッファが使用するメモリプールのサイズです。
    //---------------------------------------------------------------------------
    void Initialize(
        nn::gfx::Device* pDevice,
        const DebugFontTextWriterInfo& info,
        void* pMemory,
        size_t memorySize,
        nn::gfx::MemoryPool* pMemoryPool,
        ptrdiff_t memoryPoolOffset,
        size_t memoryPoolSize
        );

    //---------------------------------------------------------------------------
    //! @brief 終了処理です。
    //---------------------------------------------------------------------------
    void Finalize();

    //---------------------------------------------------------------------------
    //! @brief このオブジェクトが初期化済かを返します。
    //! @return 初期化済の場合 true を返します。それ以外の場合 false を返します。
    //! @details
    //! Initialize() により初期化済になります。
    //! Finalize() により未初期化になります。
    //---------------------------------------------------------------------------
    bool IsInitialized();

    //---------------------------------------------------------------------------
    //! @brief 一度に表示できる最大文字数を取得します。
    //! @return 一度に表示できる最大文字数を返します。
    //---------------------------------------------------------------------------
    int GetCharCountMax();

    //---------------------------------------------------------------------------
    //! @brief バッファのマルチバッファリングの数を取得します。
    //! @return マルチバッファリングする数を返します。
    //---------------------------------------------------------------------------
    int GetBufferCount();

    //---------------------------------------------------------------------------
    //! @brief ユーザーからのメモリプールを使用するかどうかを設定します。
    //! @return ユーザーからのメモリプールを使用する場合は true を、使用しない場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsUserMemoryPoolEnabled();

    //---------------------------------------------------------------------------
    //! @brief このクラスで使用するメモリを取得します。
    //! @return このクラスで使用するメモリを返します。
    //---------------------------------------------------------------------------
    void* GetMemory();

    //---------------------------------------------------------------------------
    //! @brief ユーザーからのメモリプールを取得します。
    //! @return ユーザーからのメモリプールを使用する場合はメモリプールを、使用しない場合は nullptr を返します。
    //---------------------------------------------------------------------------
    nn::gfx::MemoryPool* GetMemoryPool();

    //---------------------------------------------------------------------------
    //! @brief ユーザーからのメモリプールの先頭のオフセットを取得します。
    //! @return ユーザーからのメモリプールを使用する場合はメモリプールの先頭のオフセットを、使用しない場合は 0 を返します。
    //---------------------------------------------------------------------------
    ptrdiff_t GetMemoryPoolOffset();

    //---------------------------------------------------------------------------
    //! @brief ディスプレイ幅を設定します。
    //! @param[in]  displayWidth  ディスプレイ幅です。
    //---------------------------------------------------------------------------
    void SetDisplayWidth(int displayWidth);

    //---------------------------------------------------------------------------
    //! @brief ディスプレイ高さを設定します。
    //! @param[in]  displayHeight  ディスプレイ高さです。
    //---------------------------------------------------------------------------
    void SetDisplayHeight(int displayHeight);

    //---------------------------------------------------------------------------
    //! @brief テクスチャデスクリプタを設定します。
    //! @param[in]    pTextureDescriptorPool     テクスチャデスクリプタプールです。
    //! @param[in]    textureDescriptorIndexSlot テクスチャデスクリプタスロット番号です。
    //---------------------------------------------------------------------------
    void SetTextureDescriptor(nn::gfx::DescriptorPool* pTextureDescriptorPool, int textureDescriptorIndexSlot);

    //---------------------------------------------------------------------------
    //! @brief テクスチャデスクリプタスロット番号を取得します。
    //! @return テクスチャデスクリプタスロット番号を返します。
    //---------------------------------------------------------------------------
    int GetTextureDescriptorIndexSlot();

    //---------------------------------------------------------------------------
    //! @brief サンプラデスクリプタを設定します。
    //! @param[in]    pSamplerDescriptorPool     サンプラデスクリプタプールです。
    //! @param[in]    samplerDescriptorIndexSlot サンプラデスクリプタスロット番号です。
    //---------------------------------------------------------------------------
    void SetSamplerDescriptor(nn::gfx::DescriptorPool* pSamplerDescriptorPool, int samplerDescriptorIndexSlot);

    //---------------------------------------------------------------------------
    //! @brief サンプラデスクリプタスロット番号を取得します。
    //! @return サンプラデスクリプタスロット番号を返します。
    //---------------------------------------------------------------------------
    int GetSamplerDescriptorIndexSlot();

    //---------------------------------------------------------------------------
    //! @brief 1 行の高さを設定します。
    //! @param[in]  height   新しい行の高さです。
    //---------------------------------------------------------------------------
    void SetLineHeight(float height);

    //---------------------------------------------------------------------------
    //! @brief 1 行の高さを取得します。
    //! @return  1 行の高さを返します。
    //---------------------------------------------------------------------------
    float GetLineHeight() const;

    //---------------------------------------------------------------------------
    //! @brief タブ幅を指定します。
    //! @param[in]  tabWidth   文字数換算のタブ幅です。
    //---------------------------------------------------------------------------
    void SetTabWidth(int tabWidth);

    //---------------------------------------------------------------------------
    //! @brief タブ幅を取得します。
    //! @return  文字数換算のタブ幅を返します。
    //---------------------------------------------------------------------------
    int GetTabWidth() const;

    //---------------------------------------------------------------------------
    //! @brief 文字の拡大率を指定します。
    //! @param[in]  hScale  横方向拡大率です。
    //! @param[in]  vScale  縦方向拡大率です。
    //---------------------------------------------------------------------------
    void SetScale(float hScale, float vScale);

    //---------------------------------------------------------------------------
    //! @brief 文字の横方向の拡大率を取得します。
    //! @return  文字の横方向の拡大率を返します。
    //---------------------------------------------------------------------------
    float GetScaleX() const;

    //---------------------------------------------------------------------------
    //! @brief 文字の縦方向の拡大率を取得します。
    //! @return  文字の縦方向の拡大率を返します。
    //---------------------------------------------------------------------------
    float GetScaleY() const;

    //---------------------------------------------------------------------------
    //! @brief 文字のサイズを指定します。
    //! @param[in]  width   拡大後のセルの幅です。
    //! @param[in]  height  拡大後のセルの高さです。
    //---------------------------------------------------------------------------
    void SetFontSize(float width, float height);

    //---------------------------------------------------------------------------
    //! @brief 文字のサイズを指定します。
    //! @param[in]  height  拡大後のセルの高さです。
    //---------------------------------------------------------------------------
    void SetFontSize(float height);

    //---------------------------------------------------------------------------
    //! @brief 拡大適用後のセル幅を取得します。
    //! @return  拡大適用後のセル幅をピクセル単位で取得します。
    //---------------------------------------------------------------------------
    float GetFontWidth() const;

    //---------------------------------------------------------------------------
    //! @brief 拡大適用後のセル高さを取得します。
    //! @return  拡大適用後のセル高さをピクセル単位で取得します。
    //---------------------------------------------------------------------------
    float GetFontHeight() const;

    //---------------------------------------------------------------------------
    //! @brief カーソル位置を指定した新しい座標に設定します。座標系は左上(0,0)、右X+、下Y+ のスクリーン座標です。
    //! @param[in]  x   カーソルの新しい X 座標です。
    //! @param[in]  y   カーソルの新しい Y 座標です。
    //---------------------------------------------------------------------------
    void SetCursor(float x, float y);

    //---------------------------------------------------------------------------
    //! @brief カーソルの X 座標を設定します。
    //! @param[in]  x   カーソルの新しい X 座標です。
    //---------------------------------------------------------------------------
    void SetCursorX(float x);

    //---------------------------------------------------------------------------
    //! @brief カーソルの Y 座標を設定します。
    //! @param[in]  y   カーソルの新しい Y 座標です。
    //---------------------------------------------------------------------------
    void SetCursorY(float y);

    //---------------------------------------------------------------------------
    //! @brief カーソルのX座標を取得します。
    //! @return  カーソルのX座標を返します。
    //---------------------------------------------------------------------------
    float GetCursorX() const;

    //---------------------------------------------------------------------------
    //! @brief カーソルのY座標を取得します。
    //! @return  カーソルのY座標を返します。
    //---------------------------------------------------------------------------
    float GetCursorY() const;

    //---------------------------------------------------------------------------
    //! @brief 文字色を設定します。
    //! @param[in]  color  文字色です。
    //---------------------------------------------------------------------------
    void SetTextColor(const nn::util::Color4u8Type& color);

    //---------------------------------------------------------------------------
    //! @brief 文字色を取得します。
    //! @return  文字色を返します。
    //---------------------------------------------------------------------------
    const nn::util::Color4u8Type& GetTextColor() const;

    //---------------------------------------------------------------------------
    //! @brief 文字列を描画します。指定可能な文字列の長さは 1024文字(NULL終端を含む)で、1024 以上の文字列は切り捨てて表示されます。
    //! @param[in]    format  フォーマット文字列です。
    //! @param[in]    ...     フォーマット文字列パラメータです。
    //---------------------------------------------------------------------------
    void Print(const char* format, ...)
    {
        std::va_list list;
        va_start(list, format);
        VPrintUtf8(NN_GFX_UTIL_DEBUGFONT_USE_DEFAULT_LOCALE_CHARSET_VALUE, format, list);
        va_end(list);
    }

    //---------------------------------------------------------------------------
    //! @brief 描画を行います。
    //! @param[in]  pCommandBuffer  コマンドバッファです。
    //---------------------------------------------------------------------------
    void Draw(nn::gfx::CommandBuffer* pCommandBuffer);

    //---------------------------------------------------------------------------
    //! @brief 強制等幅描画を行うように設定します。
    //! @param[in]  isFixed  強制等幅描画を行うのであれば true を指定します。
    //---------------------------------------------------------------------------
    void SetFixedWidthEnabled(bool isFixed);

    //---------------------------------------------------------------------------
    //! @brief 強制等幅描画を行うかどうかを取得します。
    //! @return  強制等幅描画を行う場合は true を、行わない場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsFixedWidthEnabled() const;

    //---------------------------------------------------------------------------
    //! @brief 強制等幅描画の文字幅を指定します。
    //! @param[in]  width  新しい等幅描画幅です。
    //---------------------------------------------------------------------------
    void SetFixedWidth(float width);

    //---------------------------------------------------------------------------
    //! @brief 強制等幅描画の文字幅を取得します。
    //! @return  強制等幅描画の文字幅を返します。
    //---------------------------------------------------------------------------
    float GetFixedWidth() const;

    //---------------------------------------------------------------------------
    //! @brief 文字列の幅を計算します。
    //! @param[in]    format  フォーマット文字列です。
    //! @param[in]    ...     フォーマット文字列パラメータです。
    //! @return               文字列の幅を返します。
    //---------------------------------------------------------------------------
    float CalculateStringWidth(const char* format, ...)
    {
        std::va_list list;
        va_start(list, format);
        float width, height;
        VCalculateFormatStringWidthHeightUtf8(width, height, NN_GFX_UTIL_DEBUGFONT_USE_DEFAULT_LOCALE_CHARSET_VALUE, format, list);
        va_end(list);
        return width;
    }

    //---------------------------------------------------------------------------
    //! @brief 文字列の高さを計算します。
    //! @param[in]    format  フォーマット文字列です。
    //! @param[in]    ...     フォーマット文字列パラメータです。
    //! @return               文字列の高さを返します。
    //---------------------------------------------------------------------------
    float CalculateStringHeight(const char* format, ...)
    {
        std::va_list list;
        va_start(list, format);
        float width, height;
        VCalculateFormatStringWidthHeightUtf8(width, height, NN_GFX_UTIL_DEBUGFONT_USE_DEFAULT_LOCALE_CHARSET_VALUE, format, list);
        va_end(list);
        return height;
    }

private:
    void VPrintUtf8(bool defaultLocaleCharset, const char* format, std::va_list formatArg);
    void VCalculateFormatStringWidthHeightUtf8(float& width, float& height, bool defaultLocaleCharset, const char* format, std::va_list formatArg);

    detail::DebugFontTextWriterImpl* m_pImpl;
    void* m_pMemory;
};


} // namespace util
} // namespace gfx
} // namespace nn
