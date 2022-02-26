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

#include <nn/nn_Common.h>
#include <nn/util/util_MathTypes.h>
#include <nn/gfx.h>
#include <nn/font/font_DrawerTypes.h>

namespace nn {
namespace font {

//! リソースの先頭アドレスの必要アライメント数です。
static const int ResourceAlignment = 4 * 1024;

/**
* @brief ディスクリプタスロット確保関数のインタフェースです。
*
* @details
* 引数の textureView を登録したディスクリプタスロットを pOutSlot に返します。
*
* @param[out] pOutSlot     確保するスロット
* @param[in] textureView  テクスチャビューです。
* @param[in] pUserData    ユーザーデーターです。
*
* @return                   確保できなかった場合は、 false を返します。
*/
typedef bool (*RegisterTextureViewSlot)(nn::gfx::DescriptorSlot* pOutSlot, const nn::gfx::TextureView& textureView, void* pUserData);

/**
* @brief ディスクリプタスロット確保関数のインタフェースです。
*
* @details
* 引数の sampler を登録したディスクリプタスロットを pOutSlot に返します。
*
* @param[out] pOutSlot     確保するスロット
* @param[in] sampler      サンプラーです。
* @param[in] pUserData    ユーザーデーターです。
*
* @return                   確保できなかった場合は、 false を返します。
*/
typedef bool (*RegisterSamplerSlot)(nn::gfx::DescriptorSlot* pOutSlot, const nn::gfx::Sampler& sampler, void* pUserData);

/**
* @brief ディスクリプタスロット取得関数のインタフェースです。
*
* @details
* 引数の samplerInfo に対応する登録済みディスクリプタスロットを pOutSlot に返します。
* RegisterSamplerSlot と比較して、本関数は、Sampler オブジェクトの生成や管理をユーザープログラム側で行いたい場合に利用します。
*
* @param[out] pOutSlot     取得するスロット
* @param[in] samplerInfo   サンプラー情報です。
* @param[in] pUserData     ユーザーデーターです。
*
* @return                   取得できなかった場合は、 false を返します。
*
* @sa ReleaseSamplerSlot
*/
typedef bool(*AcquireSamplerSlot)(nn::gfx::DescriptorSlot* pOutSlot, const nn::gfx::SamplerInfo& samplerInfo, void* pUserData);

/**
* @brief ディスクリプタスロット解放関数のインタフェースです。
*
* @param[out] pSlot     解放するスロットです。
* @param[in] textureView  テクスチャビューです。
* @param[in] pUserData    ユーザーデーターです。
*
*/
typedef void (*UnregisterTextureViewSlot)(nn::gfx::DescriptorSlot* pSlot, const nn::gfx::TextureView& textureView, void* pUserData);

/**
* @brief ディスクリプタスロット解放関数のインタフェースです。
*
* @param[out] pSlot     解放するスロットです。
* @param[in] sampler      サンプラーです。
* @param[in] pUserData    ユーザーデーターです。
*
*/
typedef void (*UnregisterSamplerSlot)(nn::gfx::DescriptorSlot* pSlot, const nn::gfx::Sampler& sampler, void* pUserData);

/**
* @brief ディスクリプタスロット解放関数のインタフェースです。
*
* @details
* UnregisterSamplerSlot と比較して、本関数は、Sampler オブジェクトの生成や管理をユーザープログラム側で行いたい場合に利用します。
*
* @param[out] pSlot    解放するスロットです。
* @param[in] samplerInfo  サンプラー情報です。
* @param[in] pUserData    ユーザーデーターです。
*
* @sa AcquireSamplerSlot
*
*/
typedef void (*ReleaseSamplerSlot)(nn::gfx::DescriptorSlot* pSlot, const nn::gfx::SamplerInfo& samplerInfo, void* pUserData);

//--------------------------------------------------------------------------
//! @brief        矩形を表すクラスです。
//---------------------------------------------------------------------------
class Rectangle
{
public:
    /* ------------------------------------------------------------------------
            変数
       ------------------------------------------------------------------------ */
    float left;     //!< 矩形の左座標です。
    float top;      //!< 矩形の上座標です。
    float right;    //!< 矩形の右座標です。
    float bottom;   //!< 矩形の下座標です。


    /* ------------------------------------------------------------------------
            関数
       ------------------------------------------------------------------------ */
    //----------------------------------------
    //! @name コンストラクタ/デストラクタ
    //@{

    //--------------------------------------------------------------------------
    //! @brief        デフォルトコンストラクタです。
    //---------------------------------------------------------------------------
    Rectangle()
    : left(0),
      top(0),
      right(0),
      bottom(0)
    {
    }

    //--------------------------------------------------------------------------
    //! @brief        コンストラクタです。
    //!
    //! @param[in]    l       矩形の左座標です。
    //! @param[in]    t       矩形の上座標です。
    //! @param[in]    r       矩形の右座標です。
    //! @param[in]    b       矩形の下座標です。
    //---------------------------------------------------------------------------
    Rectangle(float l, float t, float r, float b)
    : left(l),
      top(t),
      right(r),
      bottom(b)
    {
    }

    //--------------------------------------------------------------------------
    //! @brief        コピーコンストラクタです。
    //!
    //! @param[in]    v コピー元の Rect です。
    //---------------------------------------------------------------------------
    NN_IMPLICIT Rectangle(const Rectangle& v)
    : left(v.left),
      top(v.top),
      right(v.right),
      bottom(v.bottom)
    {
    }

    //--------------------------------------------------------------------------
    //! @brief        デストラクタです。
    //---------------------------------------------------------------------------
    ~Rectangle() {}

    //@}

    //----------------------------------------
    //! @name 設定/取得
    //@{

    //--------------------------------------------------------------------------
    //! @brief        矩形の幅を取得します。
    //!
    //! @return       矩形の幅です。
    //---------------------------------------------------------------------------
    float     GetWidth() const { return right - left; }

    //--------------------------------------------------------------------------
    //! @brief        矩形の高さを取得します。
    //!
    //! @return       矩形の高さです。
    //---------------------------------------------------------------------------
    float     GetHeight() const { return bottom - top; }

    //--------------------------------------------------------------------------
    //! @brief        矩形の基点となるX座標を取得します。
    //!
    //! @return       矩形の基点となるX座標です。
    //---------------------------------------------------------------------------
    float     GetX() const { return left; }

    //--------------------------------------------------------------------------
    //! @brief        矩形の基点となるY座標を取得します。
    //!
    //! @return       矩形の基点となるY座標です。
    //---------------------------------------------------------------------------
    float     GetY() const { return top; }

    //--------------------------------------------------------------------------
    //! @brief        矩形の幅を設定します。
    //!
    //! @param[in]    width   矩形の幅です。
    //---------------------------------------------------------------------------
    void    SetWidth(float width) { right = left + width; }

    //--------------------------------------------------------------------------
    //! @brief        矩形の高さを設定します。
    //!
    //! @param[in]    height  矩形の高さです。
    //---------------------------------------------------------------------------
    void    SetHeight(float height) { bottom = top + height; }

    //--------------------------------------------------------------------------
    //! @brief        矩形の左上座標を移動します。
    //!
    //! @param[in]    x       矩形の左上座標の x 座標です。
    //! @param[in]    y       矩形の左上座標の y 座標です。
    //---------------------------------------------------------------------------
    void    MoveTo(float x, float y)
    {
        right  = x + GetWidth();
        left   = x;
        bottom = y + GetHeight();
        top    = y;
    }

    //--------------------------------------------------------------------------
    //! @brief        矩形を並行移動します。
    //!
    //! @param[in]    dx       x 方向へ矩形を移動する移動量です。
    //! @param[in]    dy       y 方向へ矩形を移動する移動量です。
    //---------------------------------------------------------------------------
    void    Move(float dx, float dy)
    {
        left    += dx;
        right   += dx;
        top     += dy;
        bottom  += dy;
    }

    //--------------------------------------------------------------------------
    //! @brief        左上と右下を設定します。
    //!
    //! @param[in]    l   左端の座標値です。
    //! @param[in]    r   右端の座標値です。
    //! @param[in]    b   下端の座標値です。
    //! @param[in]    t   上端の座標値です。
    //---------------------------------------------------------------------------
    void    SetEdge(float l, float r, float b, float t )
    {
        left    = l;
        right   = r;
        bottom  = b;
        top     = t;
    }

    //--------------------------------------------------------------------------
    //! @brief        原点とサイズを設定します。
    //!
    //! @param[in]    x       矩形の基点となるX座標です。
    //! @param[in]    y       矩形の基点となるY座標です。
    //! @param[in]    width   矩形の幅です。
    //! @param[in]    height  矩形の高さです。
    //---------------------------------------------------------------------------
    void    SetOriginAndSize(float x, float y, float width, float height )
    {
        left    = x;
        right   = x + width;
        top     = y;
        bottom  = y + height;
    }

    //--------------------------------------------------------------------------
    //! @brief        中心とサイズを設定します。
    //!
    //! @param[in]    x       中心のX座標です。
    //! @param[in]    y       中心のY座標です。
    //! @param[in]    width   矩形の幅です。
    //! @param[in]    height  矩形の高さです。
    //---------------------------------------------------------------------------
    void    SetCenterAndSize(float x, float y, float width, float height )
    {
        float hx = width * 0.5f;
        float hy = height * 0.5f;

        left    = x - hx;
        right   = x + hx;
        top     = y - hy;
        bottom  = y + hy;
    }

    //@}

    //--------------------------------------------------------------------------
    //! @brief        矩形情報を正常な値に正規化します。
    //---------------------------------------------------------------------------
    void    Normalize()
    {
        const float l = left;
        const float t = top;
        const float r = right;
        const float b = bottom;

        left    = (r - l) >= 0 ? l : r;
        right   = (r - l) >= 0 ? r : l;
        top     = (b - t) >= 0 ? t : b;
        bottom  = (b - t) >= 0 ? b : t;
    }
};

namespace detail {

enum TextColor
{
    TextColor_Start,        //!< テキストのグラデーション開始色です。
    TextColor_End,          //!< テキストのグラデーション終了色です。

    TextColor_MaxTextColor           //!< 列挙子の総数です。
};

inline bool IsAligned( const void* address, int alignment )
{
    return ( reinterpret_cast< uintptr_t >( address ) & ( alignment - 1 ) ) == 0;
}

inline bool IsAligned( const size_t offset, size_t alignment )
{
    return ( offset & ( alignment - 1 ) ) == 0;
}

}   // namespace detail
}   // namespace font
}   // namespace nn
