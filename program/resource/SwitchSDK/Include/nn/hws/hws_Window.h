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
*   @file
*   @brief  ウィンドウに関する API の宣言
*/

#pragma once

#include <nn/nn_Macro.h>

namespace nn { namespace hws {

/**
*  @brief ウィンドウハンドルを表すクラスです。
*/
class WindowHandle
{
public:
    /**
    *  @brief コンストラクタです。
    */
    WindowHandle() NN_NOEXCEPT
        : m_Handle( 0 )
    {
    }

    /**
    *  @brief コンストラクタです。
    *
    *  @param[in] handle 保持するハンドルの値を指定します。
    */
    explicit WindowHandle( void* handle ) NN_NOEXCEPT
        : m_Handle( handle )
    {
    }

    /**
    *  @brief ハンドルの値を取得します。
    *
    *  @return ハンドルの値です。
    */
    void* Get() NN_NOEXCEPT
    {
        return m_Handle;
    }

    /**
    *  @brief ハンドルの値を取得します。
    *
    *  @return ハンドルの値です。
    */
    const void* Get() const NN_NOEXCEPT
    {
       return m_Handle;
    }

    /**
    *  @brief ハンドルが有効かどうかを取得します。
    *
    *  @return ハンドルが有効かどうかの真偽値です。
    */
    bool IsValid() const NN_NOEXCEPT
    {
        return m_Handle != 0;
    }

    /**
    *  @brief ハンドルを無効化します。
    *
    *  @post
    *        - ハンドルが無効化されている。
    */
    void Invalidate() NN_NOEXCEPT
    {
        m_Handle = 0;
    }

private:
    void* m_Handle;
};

/**
*  @brief CreateWindow() への引数として使うためのクラスです。
*/
class CreateWindowArg
{
public:
    /**
    *  @brief コンストラクタです。
    */
    CreateWindowArg() NN_NOEXCEPT;

    /**
    *  @brief 設定されているウィンドウ名を取得します。
    *
    *  @return 設定されているウィンドウ名です。
    */
    const char* GetWindowName() const NN_NOEXCEPT
    {
        return m_pWindowName;
    }

    /**
    *  @brief ウィンドウ名を設定します。
    *
    *  @param[in] value 設定するウィンドウ名です。
    */
    void SetWindowName( const char* value ) NN_NOEXCEPT
    {
        m_pWindowName = value;
    }

    /**
    *  @brief 設定されているウィンドウタイトルを取得します。
    *
    *  @return 設定されているウィンドウタイトルです。
    */
    const char* GetTitle() const NN_NOEXCEPT
    {
        return m_pTitle;
    }

    /**
    *  @brief ウィンドウタイトルを設定します。
    *
    *  @param[in] value 設定するウィンドウタイトルです。
    */
    void SetTitle( const char* value ) NN_NOEXCEPT
    {
        m_pTitle = value;
    }

    /**
    *  @brief 設定されているウィンドウの左上隅の X 座標を取得します。
    *
    *  @return 設定されているウィンドウの左上隅の X 座標です。
    */
    int GetLeft() const NN_NOEXCEPT
    {
        return m_Left;
    }

    /**
    *  @brief ウィンドウの左上隅の X 座標を設定します。
    *
    *  @param[in] value 設定するウィンドウの左上隅の X 座標です。
    */
    void SetLeft( int value ) NN_NOEXCEPT
    {
        m_Left = value;
    }

    /**
    *  @brief 設定されているウィンドウの左上隅の Y 座標を取得します。
    *
    *  @return 設定されているウィンドウの左上隅の Y 座標です。
    */
    int GetTop() const NN_NOEXCEPT
    {
        return m_Top;
    }

    /**
    *  @brief ウィンドウの左上隅の Y 座標を設定します。
    *
    *  @param[in] value 設定するウィンドウの左上隅の Y 座標です。
    */
    void SetTop( int value ) NN_NOEXCEPT
    {
        m_Top = value;
    }

    /**
    *  @brief 設定されているウィンドウの幅を取得します。
    *
    *  @return 設定されているウィンドウの幅です。
    */
    int GetWidth() const NN_NOEXCEPT
    {
        return m_Width;
    }

    /**
    *  @brief ウィンドウの幅を設定します。
    *
    *  @param[in] value 設定するウィンドウの幅です。
    */
    void SetWidth( int value ) NN_NOEXCEPT
    {
        m_Width = value;
    }


    /**
    *  @brief 設定されているウィンドウの高さを取得します。
    *
    *  @return 設定されているウィンドウの高さです。
    */
    int GetHeight() const NN_NOEXCEPT
    {
        return m_Height;
    }

    /**
    *  @brief ウィンドウの高さを設定します。
    *
    *  @param[in] value 設定するウィンドウの高さです。
    */
    void SetHeight( int value ) NN_NOEXCEPT
    {
        m_Height = value;
    }

private:
    const char* m_pWindowName;
    const char* m_pTitle;
    int m_Left;
    int m_Top;
    int m_Width;
    int m_Height;
};

/*!
*  @brief 新たにウィンドウを作成します。
*
*  @param[in]  arg ウィンドウ作成時の設定です。
*
*  @return     作成されたウィンドウのハンドルを返します。作成できなかった場合は無効なハンドルを返します。
*
*  @pre
*         - ウィンドウ名が設定されている
*         - ウィンドウタイトルが設定されている
*
*  @post
*         - ウィンドウハンドルを返す
*/
WindowHandle CreateWindow( const CreateWindowArg& arg ) NN_NOEXCEPT;

/*!
*  @brief ウィンドウを破棄します。
*
*  @param[in]  handle 破棄するウィンドウのハンドルです。
*
*  @pre
*         - handle が有効である
*
*  @post
*         - ウィンドウが破棄されている
*/
void DestroyWindow( WindowHandle handle ) NN_NOEXCEPT;

/*!
*  @brief ウィンドウを探索します。
*
*  @param[in]  name 探索するウィンドウの名前です。
*
*  @return     指定された名前に一致するウィンドウのハンドルを返します。一致するウィンドウが存在しなかった場合は無効なハンドルを返します。
*
*  @pre        なし
*
*  @post
*         - ウィンドウハンドルを返す
*/
WindowHandle FindWindow( const char* name ) NN_NOEXCEPT;

/*!
*  @brief ウィンドウを表示します。
*
*  @param[in]  handle 表示するウィンドウのハンドルです。
*
*  @pre
*         - handle が有効である
*  @post
*         - ウィンドウが表示されている
*/
void ShowWindow( WindowHandle handle ) NN_NOEXCEPT;

}
} // namespace nn
