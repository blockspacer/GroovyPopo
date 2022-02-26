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

namespace nn { namespace gfx { namespace util {

    /**
    * @brief 2つのビューポートの相互変換を行うクラスです。
    *
    * @details
    * 動的にレンダリングする解像度を変更したい場合などに用いることが可能です。
    */
    class ScalableViewport
    {
    public:

        /**
        * @brief ウィンドウ座標の原点を定義する列挙体です。
        */
        enum OriginMode
        {
            OriginMode_LowerLeft,
            OriginMode_UpperLeft,
            OriginMode_Count
        };

        /**
        * @brief ウィンドウ座標を定義するクラスです。
        *
        * @details
        * nn::gfx::util::ScalableViewport が仮想ウィンドウと物理ウィンドウを管理し、ビューポートの相互変換を行います。
        */
        class WindowCoordinate
        {
        public:

            /**
            * @brief コンストラクタです。
            */
            WindowCoordinate() NN_NOEXCEPT
                : m_OriginMode(OriginMode_LowerLeft)
                , m_Width(0)
                , m_Height(0)
            {
            }

            /**
            * @brief 原点の位置を設定します。
            * @param[in] originMode 原点の位置です。
            */
            void SetOriginMode(OriginMode originMode) NN_NOEXCEPT
            {
                m_OriginMode = originMode;
            }

            /**
            * @brief 原点の位置を取得します。
            * @return 原点の位置を返します。
            */
            OriginMode GetOriginMode() const NN_NOEXCEPT
            {
                return m_OriginMode;
            }

            /**
            * @brief ウィンドウの横幅を設定します。
            * @param[in] width ウィンドウの横幅です。
            */
            void SetWidth(float width) NN_NOEXCEPT
            {
                m_Width = width;
            }

            /**
            * @brief ウィンドウの横幅を取得します。
            * @return ウィンドウの横幅を返します。
            */
            float GetWidth() const NN_NOEXCEPT
            {
                return m_Width;
            }

            /**
            * @brief ウィンドウの高さを設定します。
            * @param[in] height ウィンドウの高さです。
            */
            void SetHeight(float height) NN_NOEXCEPT
            {
                m_Height = height;
            }

            /**
            * @brief ウィンドウの高さを取得します。
            * @return ウィンドウの高さを返します。
            */
            float GetHeight() const NN_NOEXCEPT
            {
                return m_Height;
            }

            /**
            * @brief ウィンドウの幅高さを設定します。
            *
            * @param[in] width  ウィンドウの横幅です。
            * @param[in] height ウィンドウの高さです。
            *
            * @details
            * 以下の関数を呼び出すことと同義です。@n
            * SetWidth(width);      @n
            * SetHeight(height);    @n
            */
            void SetSize(float width, float height) NN_NOEXCEPT
            {
                SetWidth(width);
                SetHeight(height);
            }

            /**
            * @brief 有効な値が設定されているかを返します。
            *
            * @return 有効な値が設定されている場合 true を返します。
            *
            * @details
            * m_Width > 0 かつ m_Height > 0 かつ m_OriginMode が有効な値の場合に true を返します。
            */
            bool IsValid() const NN_NOEXCEPT;

        private:
            OriginMode  m_OriginMode;
            float       m_Width;
            float       m_Height;
        };

        /**
        * @brief ビューポートやシザーの範囲を指定するための構造体です。
        */
        struct Rect
        {
            float originX;  //! @brief X 座標の原点です。
            float originY;  //! @brief Y 座標の原点です。
            float width;    //! @brief 横幅です。
            float height;   //! @brief 縦幅です。
        };

        /**
        * @brief コンストラクタです。
        */
        ScalableViewport() NN_NOEXCEPT
            : m_IsInitialized(false)
        {
        }

        /**
        * @brief 初期化を行います。
        *
        * @param[in] virtualWindowCoordinate  設定する仮想ウィンドウ座標です。
        * @param[in] physicalWindowCoordinate 設定する物理ウィンドウ座標です。
        *
        * @pre
        * - !IsInitialized()
        * - virtualWindowCoordinate.IsValid()
        * - physicalWindowCoordinate.IsValid()
        *
        * @post
        * - IsInitialized()
        */
        void Initialize(const WindowCoordinate& virtualWindowCoordinate, const WindowCoordinate& physicalWindowCoordinate) NN_NOEXCEPT;

        /**
        * @brief 終了処理を行います。
        *
        * @pre IsInitialized()
        * @post !IsInitialized()
        *
        */
        void Finalize() NN_NOEXCEPT;

        /**
        * @brief 仮想ウィンドウ座標の設定を行います。
        *
        * @param[in] virtualWindow 設定する仮想ウィンドウ座標です。
        *
        * @pre
        * - IsInitialized()
        * - virtualWindow.IsValid()
        */
        void SetVirtualWindowCoordinate(const WindowCoordinate& virtualWindow) NN_NOEXCEPT;

        /**
        * @brief 仮想ウィンドウ座標の設定を取得します。
        * @return 物理ウィンドウ座標設定のアドレスのポインタを返します。
        */
        const WindowCoordinate* GetVirtualWindowCoordinate() const NN_NOEXCEPT
        {
            return &m_VirtualWindow;
        }

        /**
        * @brief 物理ウィンドウ座標の設定を取得します。
        *
        * @param[in] physicalWindow 設定する物理ウィンドウ座標です。
        *
        * @pre
        * - IsInitialized()
        * - virtualWindow.IsValid()
        */
        void SetPhysicalWindowCoordinate(const WindowCoordinate& physicalWindow) NN_NOEXCEPT;

        /**
        * @brief 物理ウィンドウ座標の設定を取得します。
        * @return 物理ウィンドウ座標設定のアドレスのポインタを返します。
        */
        const WindowCoordinate* GetPhysicalWindowCoordinate() const NN_NOEXCEPT
        {
            return &m_PhysicalWindow;
        }

        /**
        * @brief 仮想ウィンドウ上の点を物理ウィンドウ上の値に変換します。
        *
        * @param[out]   pOutPhysicalX   物理ウィンドウでの X 座標
        * @param[out]   pOutPhysicalY   物理ウィンドウでの Y 座標
        * @param[in]    virtualX        仮想ウィンドウでの X 座標
        * @param[in]    virtualY        仮想ウィンドウでの Y 座標
        *
        * @pre
        * - IsInitialized()
        */
        void ConvertPointVirtualToPhysical(float* pOutPhysicalX, float* pOutPhysicalY, float virtualX, float virtualY) const NN_NOEXCEPT;

        /**
        * @brief 物理ウィンドウ上の点を仮想ウィンドウ上の値に変換します。
        *
        * @param[out]   pOutVirtualX    仮想ウィンドウでの X 座標
        * @param[out]   pOutVirtualY    仮想ウィンドウでの Y 座標
        * @param[in]    physicalX       物理ウィンドウでの X 座標
        * @param[in]    physicalY       物理ウィンドウでの Y 座標
        *
        * @pre
        * - IsInitialized()
        */
        void ConvertPointPhysicalToVirtual(float* pOutVirtualX, float* pOutVirtualY, float physicalX, float physicalY) const NN_NOEXCEPT;

        /**
        * @brief 仮想ウィンドウ上の幅、高さを物理ウィンドウ上の値に変換します。
        *
        * @param[out]   pOutPhysicalWidth   物理ウィンドウでの幅
        * @param[out]   pOutPhysicalHeight  物理ウィンドウでの高さ
        * @param[in]    virtualWidth        仮想ウィンドウでの幅
        * @param[in]    virtualHeight       仮想ウィンドウでの高さ
        *
        * @pre
        * - IsInitialized()
        */
        void ConvertLengthVirtualToPhysical(float* pOutPhysicalWidth, float* pOutPhysicalHeight, float virtualWidth, float virtualHeight) const NN_NOEXCEPT;

        /**
        * @brief 物理ウィンドウ上の幅高さを仮想ウィンドウ上の値に変換します。
        *
        * @param[out]   pOutVirtualWidth    仮想ウィンドウでの幅
        * @param[out]   pOutVirtualHeight   仮想ウィンドウでの高さ
        * @param[in]    physicalWidth       物理ウィンドウでの幅
        * @param[in]    physicalHeight      物理ウィンドウでの高さ
        *
        * @pre
        * - IsInitialized()
        */
        void ConvertLengthPhysicalToVirtual(float* pOutVirtualWidth, float* pOutVirtualHeight, float physicalWidth, float physicalHeight) const NN_NOEXCEPT;

        /**
        * @brief 仮想ウィンドウ上の矩形を物理ウィンドウ上の値に変換します。
        *
        * @param[out]   pOutPhysicalRect    物理ウィンドウでの矩形
        * @param[in]    virtualRect         仮想ウィンドウでの矩形
        *
        * @pre
        * - IsInitialized()
        * - pOutPhysicalRect != NULL
        */
        void ConvertRectVirtualToPhysical(Rect* pOutPhysicalRect, const Rect& virtualRect) const NN_NOEXCEPT;

        /**
        * @brief 物理ウィンドウ上の矩形を仮想ウィンドウ上の値に変換します。
        *
        * @param[out]   pOutVirtualRect 仮想ウィンドウでの矩形
        * @param[in]    physicalRect    物理ウィンドウでの矩形
        *
        * @pre
        * - IsInitialized()
        * - pOutVirtualRect != NULL
        */
        void ConvertRectPhysicalToVirtual(Rect* pOutVirtualRect, const Rect& physicalRect) const NN_NOEXCEPT;

        /**
        * @brief 矩形の値を元にビューポートステートの設定を行います。
        *
        * @param[out]   pOutViewportStateInfo   値を設定するビューポートステートのポインタ
        * @param[in]    physicalRect            ビューポートステートの値の元となる矩形
        *
        * @details
        * - この関数では、ビューポートの原点の座標と幅、高さが設定されます。
        * - 深度に関する設定は行われないので、注意してください。
        */
        void SetupViewportStateInfo(nn::gfx::ViewportStateInfo* pOutViewportStateInfo, const Rect& physicalRect) const NN_NOEXCEPT;

        /**
        * @brief 矩形の値を元にシザーステートの設定を行います。
        *
        * @param[out]   pOutScissorStateInfo    値を設定するシザーステートのポインタ
        * @param[in]    physicalRect            シザーステートの値の元となる矩形
        *
        * @pre
        * - physicalRect.originX >= 0
        * - physicalRect.originY >= 0
        * - physicalRect.width >= 0
        * - physicalRect.height >= 0
        */
        void SetupScissorStateInfo(nn::gfx::ScissorStateInfo* pOutScissorStateInfo, const Rect& physicalRect) const NN_NOEXCEPT;

        /**
        * @brief ビューポートステートの値を元に矩形に値を設定します。
        *
        * @param[out]   pOutRect            値を設定する矩形のポインタ
        * @param[in]    viewportStateInfo   矩形の値の元のなるビューポートステート
        *
        * @pre
        * - pOutRect != NULL
        */
        void SetupRectFromViewportStateInfo(Rect* pOutRect, const nn::gfx::ViewportStateInfo& viewportStateInfo) const NN_NOEXCEPT;

        /**
        * @brief シザーステートの値を元に矩形に値を設定します。
        *
        * @param[out]   pOutRect            値を設定する矩形のポインタ
        * @param[in]    scissorStateInfo    矩形の値の元のなるシザーステート
        *
        * @pre
        * - pOutRect != NULL
        */
        void SetupRectFromScissorStateInfo(Rect* pOutRect, const nn::gfx::ScissorStateInfo& scissorStateInfo) const NN_NOEXCEPT;

        /**
        * @brief 初期化済みかどうかを取得します。
        * @return 初期化済みかどうかを返します。
        */
        bool IsInitialized() const NN_NOEXCEPT
        {
            return m_IsInitialized;
        }

        /**
        * @brief 物理ウィンドウの原点の位置を取得します。
        *
        * @tparam TTarget 対象の低レベルAPIです。
        *
        * @param[in] pDevice デバイスのポインタです。
        *
        * @return 物理ウィンドウの原点の位置を返します。
        *
        * @details
        * - pDevice != NULL
        * - nn::gfx::IsInitialized(*pDevice)
        */
        template<typename TTarget>
        static OriginMode GetPhysicalWindowOriginMode(nn::gfx::TDevice<TTarget>* pDevice) NN_NOEXCEPT;

    private:
        WindowCoordinate    m_VirtualWindow;
        WindowCoordinate    m_PhysicalWindow;
        bool                m_IsInitialized;
    };

} } }
