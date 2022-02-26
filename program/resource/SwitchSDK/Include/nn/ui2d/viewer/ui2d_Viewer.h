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

#include <nn/os/os_ThreadCommon.h>

#include <nn/font/font_ScalableFont.h>
#include <nn/font/font_TextureCache.h>

#include <nn/ui2d/viewer/ui2d_Config.h>
#if defined(NN_UI2D_VIEWER_ENABLED)

#include <nn/util/util_FormatString.h>
#include <nn/util/util_StringUtil.h>

#include <nn/ui2d.h>
#include <nn/font.h>

#include <nn/ui2d/viewer/ui2d_AnimationManager.h>

#if defined(NN_BUILD_CONFIG_OS_WIN)
#include <windows.h>
#endif

#if defined(NN_BUILD_CONFIG_OS_WIN)
#include <nn/ui2d/viewer/win/ui2d_DirResourceAccessorWin.h>
#include <nn/ui2d/viewer/win/ui2d_ScreenShotWin.h>
#else
#include <nn/ui2d/viewer/ui2d_FindableArcResourceAccessor.h>
#endif

#include <cstdarg>

//------------------------------------------------------------------------------
//! @namespace  nn::ui2d::viewer
//! @brief      ビューワライブラリの名前空間です。
//!             このライブラリを利用すると、LayoutEditor と連携し、カスタムコントロールなどの独自挙動を組み込んだビューアーを容易に作成することができます。
//------------------------------------------------------------------------------
namespace nn
{
namespace ui2d
{
namespace viewer
{

//---------------------------------------------------------------------------
//! @brief LayoutEditor と通信してプレビューするための Viewer クラスです。
//!
//---------------------------------------------------------------------------
class Viewer
{
public:

    /**
    * @brief Viewer でリソースを読み込むためのコールバック関数です。
    *
    * @param[in] pDataPath 読み込むリソースのパスです。
    * @param[in] alignment 読み込むリソースのメモリアライメント指定です。
    *
    * @return 読み込んだリソースへのポインタです。
    */
    typedef void* (*LoadFunction)(const char* pDataPath, int alignment);

    /**
    * @brief LoadFunction で読み込んだリソースの破棄処理のためのコールバック関数です。
    *
    * @param[in] pUnloadData 破棄するリソースへのポインタです。
    */
    typedef void (*UnloadFunction)(void* pUnloadData);

    /**
    * @brief Viewer 内でレイアウトデータの CalculateMtx 呼び出しの後に呼び出されるコールバック関数です。
    *
    * @deprecated 廃止予定です。ユーザーシェーダーのパラメータ更新には nn::ui2d::UpdateUserShaderCallback を使用してください。
    *
    * @param[in] layout Calculate を呼び出した nn::ui2d::Layout
    */
    typedef void (*PostCalculateCallback)(nn::ui2d::Layout& layout);

    typedef PostCalculateCallback PostCalculateMtxCallback;

    /**
    * @brief リソースの読み込み前に呼び出されるコールバック関数です。
    */
    typedef void (*PreReloadLayoutDataCallback)();

    /**
    * @brief リソースの読み込み後に呼び出されるコールバック関数です。
    *
    * @details
    * コールバック内では BuildResultInformation より取得した各種バッファのサイズより
    * 描画に必要なバッファを作成し DrawInfo へ設定するような利用方法を想定しています。
    *
    * @param[in] drawInfo 描画情報です。
    * @param[in] buildResultInformation レイアウトデータを Build 中に収集した情報です。
    */
    typedef void(*PostReloadLayoutDataCallback)(DrawInfo& drawInfo, BuildResultInformation& buildResultInformation);


    //! @brief ビューアーの動作フレームレートです。
    enum Fps
    {
        Fps_60, //! 60 pps 動作です。
        Fps_30  //! 30 pps 動作です。
    };

    //! @brief プレビューの動作モードです。
    enum PreviewMode
    {
        PreviewMode_Animation,
        PreviewMode_Control
    };

    //! @brief 画面サイズです。
    enum ViewSize
    {
        ViewSize_Original,      // レイアウトのスクリーンサイズ
        ViewSize_FullHd,        // フルHD
        ViewSize_Hd,            // HD
        ViewSize_Drc,           // DRC
        ViewSize_FitScreen,     // 画面サイズにフィット
        ViewSize_MaxViewSize
    };

    //! @brief Hud のウインドウアイテム表示モードです。（内部用機能のため使用しないでください）
    enum HudWindowItemMode
    {
        HudWindowItemMode_None,
        HudWindowItemMode_Normal,
        HudWindowItemMode_Detail,

        HudWindowItemMode_Max
    };

    //---------------------------------------------------------------------------
    //! @brief 複合フォントです。
    //---------------------------------------------------------------------------
    struct ComplexFont
    {
        nn::font::Font* pComplexFontTree;
    };

    //---------------------------------------------------------------------------
    //! @brief 入力デバイスの状態です。
    //---------------------------------------------------------------------------
    struct InputDeviceState
    {
        bool isMousePointerOn;
        float pointerX;
        float pointerY;
        bool isTrigMouseLButton;
        bool isReleaseMouseLButton;
        bool isHoldLeft;
        bool isHoldRight;
        bool isTrigUp;
        bool isTrigDown;
        bool isTrigA;
        bool isTrigB;
        bool isTrigX;
        bool isTrigY;
        bool isTrigZ;
        bool isSelect;
        bool isTrigR;
        bool isTrigL;
        bool isHoldR;
        bool isHoldZR;
    };

    //---------------------------------------------------------------------------
    //! @brief 初期化パラメーターです。
    //---------------------------------------------------------------------------
    struct InitializeArg
    {
        nn::ui2d::GraphicsResource* pGraphicsResource; //! Viewer が使用する nn::ui2d::GraphicsResource です。
        nn::ui2d::Size              mainViewportSize; //! ビューポートサイズです。
        LoadFunction                loadFunction; //! リソース読み込みのためのコールバック関数です。
        UnloadFunction              unloadFunction; //! リソース破棄のためのコールバック関数です。
        RegisterTextureViewSlot     registerTextureViewSlot; //! テクスチャディスクリプタスロット確保のためのコールバック関数です。
        UnregisterTextureViewSlot   unregisterTextureViewSlot; //! テクスチャディスクリプタスロット開放のためのコールバック関数です。
        void*                       pUserDataForDescriptorSlotAllocator; //! テクスチャディスクリプタスロットの確保・解放関数に渡されるユーザーデーターです。
        int                         scalableFontTextureCacheWidth; //! スケーラブルフォント用テクスチャキャッシュの幅(デフォルト 1024 です)
        int                         scalableFontTextureCacheHeight; //! スケーラブルフォント用テクスチャキャッシュの高さ(デフォルト 1024 です)
        size_t                      scalableFontWorkMemorySize; //! スケーラブルフォント用のプロット用ワークメモリのサイズです。
        size_t                      scalableFontNoPlotWorkMemorySize; //! スケーラブルフォント用の非プロット用ワークメモリのサイズです
        int                         toolConnectorThreadPriority; //! PC ツールとの通信スレッドのスレッド優先度を指定します(デフォルト は nn::os::LowestThreadPriority です)。

        //! @brief 初期化パラメーターをデフォルト設定にします。
        void SetDefault()
        {
            pGraphicsResource = NULL;
            mainViewportSize.Set(0.f, 0.f);
            loadFunction = NULL;
            unloadFunction = NULL;
            registerTextureViewSlot = NULL;
            unregisterTextureViewSlot = NULL;
            pUserDataForDescriptorSlotAllocator = NULL;
            scalableFontTextureCacheWidth = 1024;
            scalableFontTextureCacheHeight = 1024;
            scalableFontWorkMemorySize = 512 * 1024;
            scalableFontNoPlotWorkMemorySize = 50 * 1024;

            toolConnectorThreadPriority = nn::os::LowestThreadPriority;
        }
    };

    //! @brief コンストラクタです。
    Viewer();

    //! @brief デストラクタです。
    virtual ~Viewer();

    /**
    * @brief Viewer を初期化します。
    *
    * @param[in] initializeArg Viewer の初期化パラメーターです。
    */
    void Initialize(const InitializeArg& initializeArg);

    /**
    * @brief リソースを開放します。
    *
    * @param[in] pDevice デバイスです。
    */
    void Finalize(nn::gfx::Device* pDevice);

    //-------------------------------------------------------------------------
    //! @brief        ホストファイルシステムのパスをFSのパスに変換します。
    //!
    //! @param[out] outFsPath   FSにおけるパスが格納されます。PathLengthMax 以上のバッファサイズを指定してください。
    //! @param[in] mountPoint   PC-FS(hfio01デバイス)のマウントポイントを指定します。
    //! @param[in] inHostPath   ホストファイルシステムにおける絶対パスを指定します。
    //!
    //! @return       パスが変換できた場合には true を返します。
    //-------------------------------------------------------------------------
    static bool ConvertHostToFsPath(
        char* outFsPath,
        const char* mountPoint,
        const char* inHostPath);

#if defined(NN_BUILD_CONFIG_OS_WIN)

    //-------------------------------------------------------------------------
    //! @brief        コマンドラインオプションでプレビューを実行します。
    //!
    //! @param[in] pDevice      デバイスです。
    //! @param[in] argc      引き数の数です。
    //! @param[in] argv      引き数です。
    //!
    //! @return       プレビューできた場合には true を返します。
    //-------------------------------------------------------------------------
    bool PreviewByCommandLineOption(nn::gfx::Device* pDevice, int argc, const char** argv);

    //-------------------------------------------------------------------------
    //! @brief        ウィンドウプロシージャでプレビューを実行します。
    //!
    //! @param[in] pDevice      デバイスです。
    //! @param[in] hWnd         ウィンドウハンドルです。
    //! @param[in] message      ウィンドウメッセージです。
    //! @param[in] wParam       ウィンドウメッセージパラメーターです。
    //! @param[in] lParam       ウィンドウメッセージパラメーターです。
    //!
    //! @return       プレビューできた場合には、1 を返します。
    //-------------------------------------------------------------------------
    LRESULT PreviewByMessageProcCallback(nn::gfx::Device* pDevice, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    //-------------------------------------------------------------------------
    //! @brief        スクリーンショットが完了したかを取得します。
    //!
    //! @return       プレビューできた場合には、true を返します。
    //-------------------------------------------------------------------------
    bool IsScreenShotDone() const
    {
        return m_ScreenShot.GetState() == ScreenShot::State_Done;
    }

    //-------------------------------------------------------------------------
    //! @brief        スクリーンショット撮影の準備ができているかどうかを取得します。
    //!
    //! @return       準備完了している場合には、true を返します。
    //-------------------------------------------------------------------------
    bool IsScreenShotReady() const
    {
        return m_ScreenShot.GetState() == ScreenShot::State_ReadyToTake;
    }

    //-------------------------------------------------------------------------
    //! @brief        スクリーンショットを取得するコマンドを出力します。
    //!
    //! @param[in] constantBufferIndex    描画に利用するコンスタントバッファの番号です。
    //! @param[in] pDevice                デバイスです。
    //! @param[in] pMemoryPool            スクリーンショットが内部で利用するバッファ用のメモリプールです。
    //! @param[in] pMemoryPoolOffset      スクリーンショットが内部で利用するバッファ用のメモリプールのオフセットです。
    //! @param[in] pAllocateFunction      メモリ確保関数です。
    //! @param[in] pUserDataForAllocator  メモリ確保関数に渡すユーザーデーターです。
    //!
    //-------------------------------------------------------------------------
    void UpdateForScreenShot(
        int constantBufferIndex,
        nn::gfx::Device* pDevice,
        nn::gfx::MemoryPool* pMemoryPool,
        ptrdiff_t* pMemoryPoolOffset,
        nn::AlignedAllocateFunctionWithUserData pAllocateFunction, void* pUserDataForAllocator);

    //-------------------------------------------------------------------------
    //! @brief        スクリーンショットを取得するコマンドを出力します。
    //!
    //! @param[in] pCommandBuffer   コマンドバッファです。
    //! @param[in] pSrcTexture      取得元のテクスチャーです。
    //!
    //-------------------------------------------------------------------------
    void PutCommandScreenShot(nn::gfx::CommandBuffer* pCommandBuffer, nn::gfx::Texture* pSrcTexture)
    {
        m_ScreenShot.PutCommandScreenShot(pCommandBuffer, pSrcTexture);
    }

    //-------------------------------------------------------------------------
    //! @brief        コマンド文字列を解釈して、プレビューを実行します。
    //!
    //! @param[in] pFreeFunction   メモリ解放関数です。
    //! @param[in] pUserDataForAllocator   メモリ解放関数に渡すユーザーデーターです。
    //!
    //-------------------------------------------------------------------------
    void TakeScreenShotAfterCommandDone(
        nn::FreeFunctionWithUserData pFreeFunction, void* pUserDataForAllocator);

    //-------------------------------------------------------------------------
    //! @brief スクリーンショット機能を使用するために必要なメモリプールのサイズを取得します。
    //!
    //! @return スクリーンショット機能に必要なメモリプールのサイズ。
    //-------------------------------------------------------------------------
    size_t GetScreenShotRequiredMemoryPoolSize() const
    {
        return m_ScreenShot.GetRequiredMemoryPoolSize();
    }

    //-------------------------------------------------------------------------
    //! @brief スクリーンショット機能を使用するために必要なメモリプールのアライメントを取得します。
    //!
    //! @return スクリーンショット機能に必要なメモリプールのアライメント。
    //-------------------------------------------------------------------------
    size_t GetScreenShotMemoryPoolAlignment(nn::gfx::Device* pDevice) const
    {
        return m_ScreenShot.GetRequiredMemoryPoolAlignment(pDevice);
    }
#endif

    //-------------------------------------------------------------------------
    //! @brief        コマンド文字列を解釈して、プレビューを実行します。
    //!
    //! @param[in] pDevice   デバイスです。
    //! @param[in] commandStr   コマンド文字列です。
    //!
    //! @return       プレビューができた場合には true を返します。
    //-------------------------------------------------------------------------
    bool PreviewByCommandStr(nn::gfx::Device* pDevice, const char* commandStr);

    //-------------------------------------------------------------------------
    //! @brief レイアウトデータを更新します。
    //!
    //! @param[in] constantBufferIndex 更新するコンスタントバッファのインデックスです。
    //-------------------------------------------------------------------------
    void UpdateLayout(int constantBufferIndex);

    //-------------------------------------------------------------------------
    //! @brief レイアウト内の整列ペインを常時更新します。
    //-------------------------------------------------------------------------
    void RequestUpdatesAlignmentPane();

    //-------------------------------------------------------------------------
    //! @brief 初回フレームキャプチャテクスチャの処理を含めてレイアウトデータを更新します。
    //!
    //! @details
    //! このメソッドを呼び出すと初回フレームキャプチャテクスチャのキャプチャ処理と現在のフレームでの UpdateLayout 相当の処理が実行されます。
    //! ビューアーライブラリでは UpdateLayout 内部でコンスタントバッファの Map/Unmap をおこなっているため
    //! キャプチャテクスチャ用に 0 フレーム目で UpdateLayout & DrawCaptureTexture して、さらに実際のフレームで UpdateLayout を行うことができません。
    //! そのため、コンスタントバッファの Map/Unmap の中に上記の処理をすべて納めるため初回フレームキャプチャテクスチャ専用のメソッドを用意しています。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] commandBuffer コマンドを追加するコマンドバッファです。
    //! @param[in] constantBufferIndex 更新するコンスタントバッファのインデックスです。
    //-------------------------------------------------------------------------
    void DrawFirstFrameCaptureTextureAndUpdateLayout(nn::gfx::Device* pDevice, nn::gfx::CommandBuffer& commandBuffer, int constantBufferIndex);

    //-------------------------------------------------------------------------
    //! @brief Viewer のシステムを更新します。
    //!
    //! @param[in] pDevice デバイスです。
    //-------------------------------------------------------------------------
    void UpdateSystem(nn::gfx::Device* pDevice);

    //-------------------------------------------------------------------------
    //! @brief Viewer の入力処理を行います。
    //!
    //! @param[in] pDevice デバイスです。
    //! @param[in] inputDeviceState 入力デバイスの状態です。
    //-------------------------------------------------------------------------
    void UpdateInputs(nn::gfx::Device* pDevice, const InputDeviceState& inputDeviceState);

    //-------------------------------------------------------------------------
    //! @brief Viewer のメニューを更新します。現在、一時的に廃止されています。
    //-------------------------------------------------------------------------
    void UpdateMenu();

    //-------------------------------------------------------------------------
    //! @brief Viewer の内部で管理しているスケーラブルフォント用のテクスチャキャッシュにグリフを登録し、更新を完了します。
    //!
    //-------------------------------------------------------------------------
    void RegisterAndCompleteAllGlyphsForScalableFontTextureCache();

    //-------------------------------------------------------------------------
    //! @brief レイアウトデータが使用しているキャプチャテクスチャの内容物を描画してテクスチャを更新します。
    //!
    //! @param[in] pDevice  デバイスです。
    //! @param[in] commandBuffer コマンドを追加するコマンドバッファです。
    //-------------------------------------------------------------------------
    void DrawCaptureTexture(nn::gfx::Device* pDevice, nn::gfx::CommandBuffer& commandBuffer);

    //-------------------------------------------------------------------------
    //! @brief レイアウトデータを描画するコマンドを作成します。
    //!
    //! @param[in] commandBuffer コマンドを追加するコマンドバッファです。
    //-------------------------------------------------------------------------
    void DrawLayout(nn::gfx::CommandBuffer& commandBuffer);

    void DrawLayoutDRC(nn::gfx::CommandBuffer& commandBuffer);

    //-------------------------------------------------------------------------
    //! @brief Viewer システム用の描画コマンドを作成します。現在、一時的に廃止されています。
    //!
    //! @param[in] commandBuffer コマンドを追加するコマンドバッファです。
    //-------------------------------------------------------------------------
    void DrawSystem(nn::gfx::CommandBuffer& commandBuffer);

    //-------------------------------------------------------------------------
    //! @brief 設定されているフレームレートを取得します。
    //!
    //! @return フレームレートです。
    //-------------------------------------------------------------------------
    Fps GetFps() const
    {
        return m_Fps;
    }

    //-------------------------------------------------------------------------
    //! @brief sRGB 書き込み有効フラグを取得します。
    //!
    //! @return sRGB 書き込み有効フラグです。
    //-------------------------------------------------------------------------
    bool IsSrgbWriteEnabled() const
    {
        return m_IsSrgbWriteEnabled;
    }

    //-------------------------------------------------------------------------
    //! @brief sRGB 書き込み有効フラグを取得します。
    //!
    //! @return sRGB 書き込み有効フラグです。
    //-------------------------------------------------------------------------
    void SetSrgbWriteEnabled(bool value)
    {
        m_IsSrgbWriteEnabled = value;
    }

    //-------------------------------------------------------------------------
    //! @brief ButtonGroup を取得します。
    //!
    //! @return Viewer が所有する ButtonGroup への参照を返します。
    //-------------------------------------------------------------------------
    nn::ui2d::ButtonGroup& GetButtonGroup()
    {
        return m_ButtonGroup;
    }

    //-------------------------------------------------------------------------
    //! @brief ButtonGroup を取得します。
    //!
    //! @return Viewer が所有する ButtonGroup への参照を返します。
    //-------------------------------------------------------------------------
    const nn::ui2d::ButtonGroup& GetButtonGroup() const
    {
        return m_ButtonGroup;
    }

    //-------------------------------------------------------------------------
    //! @brief Viewer ライブラリ内の HUD 表示・非表示 を取得します。
    //!
    //! @return Viewer ライブラリ内の HUD 表示・非表示を返します。
    //-------------------------------------------------------------------------
    bool IsHudVisible() const
    {
        return m_IsHudVisible;
    }

    //-------------------------------------------------------------------------
    //! @brief ControlCreator を設定します。
    //!
    //! @param[in] pControlCreator  ControlCreator を設定します。
    //!
    //! @return Viewer への参照を返します。
    //!
    //! @details
    //! pControlCreator に NULL を設定した場合には nn::ui2d::DefaultControlCreator が
    //! 使用されます。
    //!
    //! Viewer の初期状態では ControlCreator は NULL に設定されます。
    //!
    //! @sa GetButtonGroup
    //! @sa GetControlCreator
    //-------------------------------------------------------------------------
    Viewer& SetControlCreator(nn::ui2d::ControlCreator* pControlCreator)
    {
        m_pControlCreator = pControlCreator;
        return *this;
    }

    //-------------------------------------------------------------------------
    //! @brief ControlCreator を取得します。
    //!
    //! @return 設定されている ControlCreator へのポインタを返します。
    //!
    //! @sa SetControlCreator
    //-------------------------------------------------------------------------
    nn::ui2d::ControlCreator* GetControlCreator() const
    {
        return m_pControlCreator;
    }

    //-------------------------------------------------------------------------
    //! @brief TV画面のビューポートサイズを取得します。
    //!
    //! @return TV画面のビューポートサイズを返します。
    //!
    //! @sa Initialize
    //-------------------------------------------------------------------------
    const nn::ui2d::Size& GetMainViewportSize() const
    {
        return m_MainViewportSize;
    }

    //-------------------------------------------------------------------------
    //! @brief 背景色を取得します。
    //!
    //! @param[out] pBackgroundColor 背景色を RGBA の 4 要素で書き込んで返します。
    //!
    //-------------------------------------------------------------------------
    void GetBackgroundColor(float* pBackgroundColor) const;

    //-------------------------------------------------------------------------
    //! @brief 背景イメージが有効かどうか取得します。
    //!
    //-------------------------------------------------------------------------
    bool IsBackgroundImageEnabled()
    {
        return m_PreviewOption.backgroundImageEnabled;
    }

    //-------------------------------------------------------------------------
    //! @brief 背景イメージの DescriptorSlot を取得します。
    //!
    //-------------------------------------------------------------------------
    const nn::gfx::DescriptorSlot& GetBackgroundImage() const
    {
        return m_BackgroundImageSlot;
    }

    //-------------------------------------------------------------------------
    //! @brief 背景イメージのサイズを取得します。
    //!
    //! @return 背景イメージのサイズを返します。
    //!
    //! @sa Initialize
    //-------------------------------------------------------------------------
    const nn::ui2d::Size& GetBackgroundImageSize() const
    {
        return m_BackgroundImageSize;
    }

    //-------------------------------------------------------------------------
    //! @brief ビューア情報を描画します。
    //!
    //! @param[in] writer  フォント描画クラスです。
    //!
    //-------------------------------------------------------------------------
    void WriteHud(nn::font::TextWriter& writer) const;

    //-------------------------------------------------------------------------
    //! @brief ユーザーシェーダー情報取得用のコールバック関数を設定します。
    //!
    //! @details
    //! このメソッドでは Viewer が保持している変数を書き換えるだけのためいつでも呼び出すことが可能です。
    //! Preview 開始時にこのメソッドで設定した関数ポインタを Layout に渡します。
    //!
    //! @param[in]  pCallback   設定する GetUserShaderInformationFromUserData 関数。
    //-------------------------------------------------------------------------
    void SetGetUserShaderInformationFromUserDataCallback(GetUserShaderInformationFromUserData pCallback)
    {
        m_GetUserShaderInformationFromUserDataCallback = pCallback;
    }

    //-------------------------------------------------------------------------
    //! @brief Layout::Calculate 呼出し後の Layout クラスへアクセスするためのコールバック関数を設定します。
    //!
    //! @details
    //! このメソッドで設定した関数を、 Viewer が内部で Layout::Calculate を呼び出した後に呼び出します。
    //!
    //! @param[in]  pCallback   設定する PostCalculateCallback 関数。
    //-------------------------------------------------------------------------
    void SetPostCalculateCallback(PostCalculateCallback pCallback)
    {
        m_PostCalculateCallback = pCallback;
    }

    //-------------------------------------------------------------------------
    //! @brief Viewer ライブラリ内でデータのリロードを行う直前に呼び出されるコールバック関数を設定します。
    //!
    //! @details
    //! このメソッドで設定した関数を、 Viewer が内部でデータをリロードする直前に呼び出します。
    //!
    //! @param[in]  pCallback   設定する PreReloadLayoutDataCallback 関数。
    //-------------------------------------------------------------------------
    void SetPreReloadLayoutDataCallback(PreReloadLayoutDataCallback pCallback)
    {
        m_PreReloadLayoutDataCallback = pCallback;
    }

    //-------------------------------------------------------------------------
    //! @brief Viewer ライブラリ内でデータのリロードを行った直後に呼び出されるコールバック関数を設定します。
    //!
    //! @details
    //! このメソッドで設定した関数を、 Viewer が内部でリロードしたデータを Build した直前に呼び出します。
    //!
    //! @param[in]  pCallback   設定する PostReloadLayoutDataCallback 関数。
    //-------------------------------------------------------------------------
    void SetPostReloadLayoutDataCallback(PostReloadLayoutDataCallback pCallback)
    {
        m_PostReloadLayoutDataCallback = pCallback;
    }

    //-------------------------------------------------------------------------
    //! @brief Viewer ライブラリ内の HUD 表示・非表示 を設定します。
    //!
    //! @param[in]  value   true を指定したら HUD が描画されます。
    //-------------------------------------------------------------------------
    void SetHudVisible(bool value)
    {
        m_IsHudVisible = value;
    }

    //-------------------------------------------------------------------------
    //! @brief 描画中のレイアウトを取得します。
    //!
    //! @return レイアウトのポインタを返します。
    //-------------------------------------------------------------------------
    nn::ui2d::Layout* GetLayout() const
    {
        return m_Layout;
    }

    //-------------------------------------------------------------------------
    //! @brief 常に整列を行うかのフラグを取得します。
    //!
    //! @return 常に整列を行うかのフラグです。
    //-------------------------------------------------------------------------
    bool IsAlwaysAlignment() const
    {
        return m_IsAlwaysAlignment;
    }

    //-------------------------------------------------------------------------
    //! @brief 常に整列を行うかのフラグを設定します。
    //!
    //! @param[in] value  常に整列を行うかのフラグです。
    //-------------------------------------------------------------------------
    void SetAlwaysAlignment(bool value)
    {
        m_IsAlwaysAlignment = value;
    }

    //-------------------------------------------------------------------------
    //! @brief  プレビューサイズテーブルの数を得ます（内部用機能のため使用しないでください）
    //!
    //! @return プレビューサイズテーブルの数です。
    //-------------------------------------------------------------------------
    int GetPreviewSizeCount() const
    {
        return m_PreviewOption.previewSizeCount;
    }

    //-------------------------------------------------------------------------
    //! @brief  プレビューサイズテーブル得ます（内部用機能のため使用しないでください）
    //!
    //! @return プレビューサイズテーブルです。
    //-------------------------------------------------------------------------
    const nn::ui2d::Size& GetPreviewSize(int index) const
    {
        NN_SDK_ASSERT(index < PreviewOption::PreviewSizeMaxCount && index < m_PreviewOption.previewSizeCount);
        return m_PreviewOption.previewSizeTable[index];
    }

    //-------------------------------------------------------------------------
    //! @brief ビューポートやシザーのデフォルト領域を DrawInfo に設定します。
    //!
    //! @param[in] viewportInfo ビューポートの情報
    //! @param[in] scissorInfo シザーの情報
    //-------------------------------------------------------------------------
    void SetDefaultViewportScissorInfo(const nn::gfx::ViewportStateInfo& viewportInfo, const nn::gfx::ScissorStateInfo& scissorInfo)
    {
        m_DrawInfo.SetDefaultViewportScissorInfo(viewportInfo, scissorInfo);
    }

    //-------------------------------------------------------------------------
    //! @brief  プレビューモードを取得します。
    //!
    //! @return プレビューモードを取得します。
    //-------------------------------------------------------------------------
    PreviewMode GetPreviewMode() const
    {
        return m_PreviewOption.previewMode;
    }

    //-------------------------------------------------------------------------
    //! @brief  アニメーションフレームを取得します。
    //!
    //! @return アニメーションフレームを取得します。
    //-------------------------------------------------------------------------
    float GetAnimationFrame() const
    {
        return m_AnimationMgr.GetCurrentAnimationFrame();
    }

    //-------------------------------------------------------------------------
    //! @brief  アニメーションフレームの最大を取得します。
    //!
    //! @return アニメーションフレームの最大を取得します。
    //-------------------------------------------------------------------------
    float GetAnimationFrameMax() const
    {
        return m_AnimationMgr.GetAnimationFrameMax();
    }

    //-------------------------------------------------------------------------
    //! @brief  アニメーション数を取得します。
    //!
    //! @return アニメーション数をを取得します。
    //-------------------------------------------------------------------------
    int GetAnimationCount() const
    {
        return m_AnimationMgr.GetAnimationCount();
    }

    //-------------------------------------------------------------------------
    //! @brief  アニメーション名を取得します。
    //!
    //! @return アニメーション名をを取得します。
    //-------------------------------------------------------------------------
    const char* GetCurrentAnimationName() const
    {
        return m_AnimationMgr.GetCurrentAnimationName();
    }

    //-------------------------------------------------------------------------
    //! @brief  アニメーションのループが有効かどうかを取得します。
    //!
    //! @return アニメーションのループが有効かどうかを取得します。
    //-------------------------------------------------------------------------
    bool IsCurrentAnimationLoop() const
    {
        return m_AnimationMgr.IsCurrentAnimationLoop();
    }

    //-------------------------------------------------------------------------
    //! @brief  アニメーションの番号を取得します。
    //!
    //! @return アニメーションの番号を取得します。
    //-------------------------------------------------------------------------
    int GetCurrentAnimationNo() const
    {
        return m_AnimationMgr.GetCurrentAnimationNo();
    }

    //-------------------------------------------------------------------------
    //! @brief 透視投影が有効かを取得します。
    //!
    //! @return 透視投影が有効かを取得します。
    //-------------------------------------------------------------------------
    bool IsPerspectiveProjection() const
    {
        return m_IsPerspectiveProjection;
    }

    //-------------------------------------------------------------------------
    //! @brief 視野の画角を取得します。
    //!
    //! @return 視野の画角を取得します。
    //-------------------------------------------------------------------------
    float GetPerspectiveFovyDeg() const
    {
        return m_PerspectiveFovyDeg;
    }

    //-------------------------------------------------------------------------
    //! @brief 視野の最前面を取得します。
    //!
    //! @return 透視投影プロジェクションが有効かを取得します。
    //-------------------------------------------------------------------------
    float GetPerspectiveNear() const
    {
        return m_PerspectiveNear;
    }

    //-------------------------------------------------------------------------
    //! @brief 視野の最後面を取得します。
    //!
    //! @return 視野の最後面を取得します。
    //-------------------------------------------------------------------------
    float GetPerspectiveFar() const
    {
        return m_PerspectiveFar;
    }

    //-------------------------------------------------------------------------
    //! @brief  ビューアのサイズを取得します。
    //!
    //! @return ビューアのサイズを取得します。
    //-------------------------------------------------------------------------
    nn::ui2d::Size GetViewSize() const;

    //-------------------------------------------------------------------------
    //! @brief  ビューアの画面サイズ設定を取得します。
    //!
    //! @return ビューアの画面サイズ設定を取得します。
    //-------------------------------------------------------------------------
    ViewSize GetViewerSizeMode() const
    {
        return m_ViewSize;
    }

    //-------------------------------------------------------------------------
    //! @brief  プロジェクション行列を取得します。
    //!
    //! @return プロジェクション行列を取得します。
    //-------------------------------------------------------------------------
    nn::util::MatrixT4x4fType GetProjectionMatrix() const
    {
        return m_ProjectionMatrix;
    }

    //-------------------------------------------------------------------------
    //! @brief  ビュー行列を取得します。
    //!
    //! @return ビュー行列を取得します。
    //-------------------------------------------------------------------------
    nn::util::MatrixT4x3fType GetViewMatrix() const
    {
        return m_CameraViewMatrix;
    }

    //-------------------------------------------------------------------------
    //! @brief  アニメーション再生中かどうかを取得します。
    //!
    //! @return アニメーション再生中かどうかを取得します。
    //-------------------------------------------------------------------------
    bool IsAnimationPlaying() const
    {
        return m_AnimationMgr.IsAnimationPlaying();
    }

    //-------------------------------------------------------------------------
    //! @brief  アニメーションを再生します。
    //!
    //! @return アニメーションを再生します。
    //-------------------------------------------------------------------------
    void StartAnimation()
    {
        return m_AnimationMgr.StartAnimation();
    }

    //-------------------------------------------------------------------------
    //! @brief  アニメーションを停止します。
    //!
    //! @return アニメーションを停止します。
    //-------------------------------------------------------------------------
    void StopAnimation()
    {
        return m_AnimationMgr.StopAnimation();
    }

    //-------------------------------------------------------------------------
    //! @brief  アニメーションを設定します。
    //!
    //! @return アニメーションを設定します。
    //-------------------------------------------------------------------------
    void SetCurrentAnimationNo(nn::gfx::Device* pDevice, int no)
    {
        NN_SDK_ASSERT_NOT_NULL(pDevice);
        m_AnimationMgr.SetCurrentAnimationNo(pDevice, no);
    }

    //-------------------------------------------------------------------------
    //! @brief  アニメーションフレームを設定します。
    //!
    //! @return アニメーションフレームを設定します。
    //-------------------------------------------------------------------------
    void SetCurrentAnimationFrame(float frame)
    {
        return m_AnimationMgr.SetCurrentAnimationFrame(frame);
    }

    //-------------------------------------------------------------------------
    //! @brief Viewer ライブラリ内の HUD 内のウインドウ表示モードを取得します。（内部用機能のため使用しないでください）
    //!
    //! @return   Viewer ライブラリ内の HUD 内のウインドウ表示モードを取得します。
    //-------------------------------------------------------------------------
    HudWindowItemMode GetHudWindowItemMode() const
    {
        return m_HudWindowItemMode;
    }

    //-------------------------------------------------------------------------
    //! @brief Viewer ライブラリ内の HUD 内のウインドウ表示モードを設定します。（内部用機能のため使用しないでください）
    //!
    //! @param[in]  mode  Viewer ライブラリ内のウインドウ表示モード。
    //-------------------------------------------------------------------------
    void SetHudWindowItemMode(HudWindowItemMode mode)
    {
        m_HudWindowItemMode = mode;
    }

protected:
    //-------------------------------------------------------------------------
    //! @brief        パス最大長です。
    //-------------------------------------------------------------------------
    static const int    PathLengthMax = 512;

    //-------------------------------------------------------------------------
    //! @brief        画面サイズです。
    //-------------------------------------------------------------------------
    struct ViewSizeValue
    {
        float x;
        float y;
    };

    //-------------------------------------------------------------------------
    //! @brief        プレビューのオプションです。
    //-------------------------------------------------------------------------
    struct PreviewOption
    {
        PreviewMode previewMode;

        //! レイアウト・バイナリリソースのパスを指定します。
        //!
        //! 実機の場合は、アーカイブのFSにおけるパスを指定します。
        //!
        //! PCの場合は、バイナリリソースの出力されたディレクトリの、
        //! PCファイルシステムにおけるパスを指定します。
        char  path[PathLengthMax];

        //! レイアウト名です。
        char  layoutName[PathLengthMax];

        //! sRGB Write が有効化どうかです。
        uint8_t  isSrgbWriteEnabled;

        // アニメーションの再生を以前の状態から引き継ぐかどうかです。
        bool continueAnimation;

        // 背景画像が有効化どうかです。
        bool backgroundImageEnabled;

        // 解像度変更テーブル
        static const int            PreviewSizeMaxCount = 10;
        nn::ui2d::Size              previewSizeTable[PreviewSizeMaxCount];
        int                         previewSizeCount;

        //! リセットします。
        void reset()
        {
            previewMode = PreviewMode_Animation;

            path[0] = '\0';
            layoutName[0] = '\0';

            isSrgbWriteEnabled = false;

            continueAnimation = false;
            backgroundImageEnabled = false;

            // 解像度テーブルの初期化
            previewSizeCount = 0;
            for (int i = 0; i < PreviewSizeMaxCount; i++)
            {
                previewSizeTable[i] = {};
            }
        }
    };

    //-------------------------------------------------------------------------
    //! @brief Viewer ライブラリ内のリソースの終了処理を行います。
    //!
    //! @param[in]  pDevice   デバイスです。
    //-------------------------------------------------------------------------
    virtual void FinalizeResources(nn::gfx::Device* pDevice);

private:
    enum OptionMode
    {
        OptionMode_None,
        OptionMode_PreviewMode,
        OptionMode_LayoutName,
        OptionMode_PerspectiveProjection,
        OptionMode_PerspectiveFovy,
        OptionMode_Near,
        OptionMode_Far,
        OptionMode_ViewSize,
        OptionMode_BackgroundColor,
        OptionMode_BackgroundImage,
        OptionMode_PrevSizes,
        OptionMode_MaxOptionMode
    };

    void StartPreview(nn::gfx::Device* pDevice, const PreviewOption& option);
    OptionMode GetOptionMode(const char* buf);
    void ClacPrevSizes(PreviewOption* option, const char* buf);
    void AnalyseOptionArgument(PreviewOption* option, OptionMode mode, const char* buf);
    void ParseCommandStr(PreviewOption* option, const char* commandStr);
    void SetViewSize(ViewSize viewSize);

    void InitializeScalableFont(nn::gfx::Device* pDevice);
    void FinalizeScalableFont(nn::gfx::Device* pDevice);

    void SetBackgroundColor(const char* pHexColorString);

    void FlushGpuBuffer();

    //----------------------------------------------------------

    bool m_AnimateOnce; // 一回はアニメーションを行ったか

    nn::ui2d::DrawInfo m_DrawInfo;
    nn::ui2d::Layout* m_Layout;

    ComplexFont* m_pComplexFontSet;
    int m_ComplexFontCount;
    nn::font::TextureCache* m_pTextureCache;

#if defined(NN_BUILD_CONFIG_OS_WIN)
    DirResourceAccessor& m_ResourceAccessor;
    ScreenShot m_ScreenShot;
#else
    FindableArcResourceAccessor m_ResourceAccessor;
#endif
    PreviewOption m_PreviewOption;

    AnimationManager m_AnimationMgr;

    nn::ui2d::ButtonGroup m_ButtonGroup;
    nn::ui2d::ControlCreator* m_pControlCreator;

    ViewSize m_ViewSize;

    float   m_BackgroundColor[4];

    nn::ui2d::Size m_MainViewportSize;
    nn::util::MatrixT4x4fType m_ProjectionMatrix;
    nn::util::MatrixT4x3fType m_CameraViewMatrix;
    nn::util::MatrixT4x4fType m_ProjectionMatrixDRC;
    nn::util::MatrixT4x3fType m_CameraViewMatrixDRC;

    bool m_IsPerspectiveProjection;
    Fps m_Fps;
    float m_PerspectiveFovyDeg;
    float m_PerspectiveNear;
    float m_PerspectiveFar;
    bool m_IsViewMtxDirty;
    bool m_IsSrgbWriteEnabled;
    bool m_IsHudVisible;
    HudWindowItemMode m_HudWindowItemMode;
    bool m_IsAlwaysAlignment;

    LoadFunction        m_LoadFunction;
    UnloadFunction      m_UnloadFunction;
    RegisterTextureViewSlot m_RegisterTextureViewSlot;
    UnregisterTextureViewSlot m_UnregisterTextureViewSlot;
    void*                     m_pUserDataForDescriptorSlotAllocator;
    int                         m_ScalableFontTextureCacheWidth;
    int                         m_ScalableFontTextureCacheHeight;
    size_t m_ScalableFontWorkMemorySize;
    size_t m_ScalableFontNoPlotWorkMemorySize;

    GetUserShaderInformationFromUserData m_GetUserShaderInformationFromUserDataCallback;
    PostCalculateCallback    m_PostCalculateCallback;
    PreReloadLayoutDataCallback m_PreReloadLayoutDataCallback;
    PostReloadLayoutDataCallback m_PostReloadLayoutDataCallback;

    nn::gfx::ResTextureFile*    m_pBackgroundImageFile;
    nn::gfx::DescriptorSlot     m_BackgroundImageSlot;
    nn::ui2d::Size              m_BackgroundImageSize;

#if defined(NN_BUILD_CONFIG_OS_WIN)
    struct LayoutProtocol
    {
        char Verb[ 32 ];
        char Data[ 1024 ];
    };
#endif
    static const ViewSizeValue ViewSizeValues[ViewSize_MaxViewSize];
    static const char* ViewSizeDescriptions[ViewSize_MaxViewSize];
};

} // namespace viewer
} // namespace ui2d
} // namespace nn

#endif // NN_UI2D_VIEWER_ENABLED
