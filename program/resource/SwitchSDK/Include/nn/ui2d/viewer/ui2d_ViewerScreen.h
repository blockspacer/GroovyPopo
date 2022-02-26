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

#include <nn/ui2d/viewer/ui2d_ScalableFontManager.h>
#include <nn/ui2d/ui2d_Screen.h>
#include <nn/ui2d/viewer/ui2d_FindableArcResourceAccessor.h>

#if defined(NN_BUILD_CONFIG_OS_WIN)
#include <windows.h>
#endif


#include <cstdarg>

#include <nn/ui2d/viewer/ui2d_Viewer.h>
#include <nn/ui2d/viewer/ui2d_AnimationManager2.h>

namespace nn
{
namespace ui2d
{
namespace viewer
{

class SubScreenStateMachine;

//---------------------------------------------------------------------------
//! @brief LayoutEditor と通信してプレビューするための ViewerScreen クラスです。(注意：開発中のため利用しないでください)
//!
//! Viewer としての機能を持った、Screen です。
//! Screen の処理をオーバーライドして、プレビュー機能を付加しています。
//---------------------------------------------------------------------------
class ViewerScreen : public Screen
{
public:
    NN_UI2D_RUNTIME_TYPEINFO(ScreenBase);
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
    typedef void(*UnloadFunction)(void* pUnloadData);

    /**
    * @brief リソースの読み込み前に呼び出されるコールバック関数です。
    */
    typedef void(*PreReloadLayoutDataCallback)();

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

    //! @brief 画面サイズです。
    enum ViewSize
    {
        ViewSize_Original,      // レイアウトのスクリーンサイズ
        ViewSize_FitScreen,     // 画面サイズにフィット
        ViewSize_MaxViewSize
    };

    //! @brief プレビューの動作モードです。
    enum PreviewMode
    {
        PreviewMode_Animation,
        PreviewMode_Control
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
    //! @brief 初期化パラメーターです。
    //---------------------------------------------------------------------------
    struct InitializeArg
    {
        ScreenManager*              pScreenManager;
        nn::ui2d::Size              mainViewportSize; //! ビューポートサイズです。
        LoadFunction                loadFunction; //! リソース読み込みのためのコールバック関数です。
        UnloadFunction              unloadFunction; //! リソース破棄のためのコールバック関数です。
        int                         toolConnectorThreadPriority; //! PC ツールとの通信スレッドのスレッド優先度を指定します(デフォルト は nn::os::LowestThreadPriority です)。

                                                                    //! @brief 初期化パラメーターをデフォルト設定にします。
        void SetDefault()
        {
            pScreenManager = nullptr;
            mainViewportSize.Set(0.f, 0.f);
            loadFunction = nullptr;
            unloadFunction = nullptr;

            toolConnectorThreadPriority = nn::os::LowestThreadPriority;
        }
    };

    //! @brief コンストラクタです。
    ViewerScreen();

    //! @brief デストラクタです。
    virtual ~ViewerScreen();

    /**
    * @brief Viewer を初期化します。
    *
    * @param[in] initializeArg Viewer の初期化パラメーターです。
    */
    void Initialize(nn::gfx::Device* pDevice, const InitializeArg& initializeArg);

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
    virtual void UpdateInputs(nn::gfx::Device* pDevice, const nn::ui2d::ScreenBase::InputDeviceState& inputDeviceState) NN_OVERRIDE;


    //-------------------------------------------------------------------------
    //! @brief レイアウト内の整列ペインを常時更新します。
    //-------------------------------------------------------------------------
    void RequestUpdatesAlignmentPane();

    //----------------------------------------------------------
    virtual void UpdateLayout(nn::gfx::Device* pDevice) NN_OVERRIDE;

    //----------------------------------------------------------
    virtual void DrawCaptureTexture(nn::gfx::Device* pDevice, nn::gfx::CommandBuffer& commandBuffer) NN_OVERRIDE;

    //----------------------------------------------------------
    virtual void DrawLayout(nn::gfx::CommandBuffer& commandBuffer) NN_OVERRIDE;

    //----------------------------------------------------------
    nn::ui2d::IFindableResourceAccessor* GetFindableResourceAccessor() { return m_pFindableResourceManager; }

    //-------------------------------------------------------------------------
    //! @brief 表示対象が想定しているフレームレートを取得します。
    //!
    //! @return フレームレートです。
    //-------------------------------------------------------------------------
    Fps GetFps() const
    {
        return m_Fps;
    }


    ScreenManager* GetScreenManager()
    {
        return m_pScreenManager;
    }

    const ScreenManager* GetScreenManager() const
    {
        return m_pScreenManager;
    }

    SubScreenStateMachine* GetSubScreenStateMachine()
    {
        return m_pSubScreenStateMachine;
    }

    const SubScreenStateMachine* GetSubScreenStateMachine() const
    {
        return m_pSubScreenStateMachine;
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
    //! @brief  プレビューモードを取得します。
    //!
    //! @return プレビューモードを取得します。
    //-------------------------------------------------------------------------
    PreviewMode GetPreviewMode() const
    {
        return m_PreviewOption.previewMode;
    }

    //-------------------------------------------------------------------------
    //! @brief  プレビューモードを設定します。
    //-------------------------------------------------------------------------
    void SetPreviewMode(PreviewMode mode)
    {
        m_PreviewOption.previewMode = mode;
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

    //--------------------------------------------------------------------------------------------------
    virtual nn::ui2d::ControlCreator* GetControlCreator() const NN_OVERRIDE
    {
        if (m_PreviewOption.previewMode == PreviewMode_Animation)
        {
            return nullptr;
        }

        return Screen::GetControlCreator();
    }

    //--------------------------------------------------------------------------------------------------
    struct AnimationPlayingState
    {
        bool isAnimationPlayingOld = true;
        float animationFrameOld = 0;
        char animationNameOld[nn::ui2d::AnimTagNameStrMax + 1] = {};
    };

    //--------------------------------------------------------------------------------------------------
    bool GetAnimationPlayingState(AnimationPlayingState* pState) const;

    //--------------------------------------------------------------------------------------------------
    void SetAnimationPlayingState(nn::gfx::Device* pDevice, const AnimationPlayingState& state);

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

    virtual nn::ui2d::ResourceAccessor* CreateResourceAccessor_() NN_OVERRIDE
    {
        m_pFindableResourceManager = nn::ui2d::Layout::AllocateAndConstruct<nn::ui2d::viewer::FindableArcResourceAccessor>();
        return m_pFindableResourceManager;
    }

    virtual void DoBuildAnimatons_(nn::gfx::Device* pDevice, const void* pLayoutResource) NN_OVERRIDE;

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
        OptionMode_PrevSizes,
        OptionMode_MaxOptionMode
    };

    void AttatchArchive(const char* pArcPath, void* pData);
    void StartPreview(nn::gfx::Device* pDevice, const PreviewOption& option);
    OptionMode GetOptionMode(const char* buf);
    void AnalyseOptionArgument(PreviewOption* option, OptionMode mode, const char* buf);
    void ParseCommandStr(PreviewOption* option, const char* commandStr);

    void SetBackgroundColor(const char* pHexColorString);
    void ClacPrevSizes(PreviewOption* option, const char* buf);

    bool CheckOptionEndsWithArcFilePath_(const char *option, const char *suffix);

    //-------------------------------------------------------------------------
    nn::ui2d::viewer::AnimationManager2& GetAnimationManager()
    {
        return m_AnimationMgr;
    }

    const nn::ui2d::viewer::AnimationManager2& GetAnimationManager() const
    {
        return m_AnimationMgr;
    }


    nn::ui2d::viewer::FindableArcResourceAccessor* m_pFindableResourceManager;

    //----------------------------------------------------------
    // アニメーション関係
    viewer::AnimationManager2    m_AnimationMgr;

    //----------------------------------------------------------
    // 描画対象リソース関係
    ScreenManager*              m_pScreenManager;

    //----------------------------------------------
    PreviewOption               m_PreviewOption;

    //----------------------------------------------
    // 画面サイズ関連
    ViewSize                    m_ViewSize;
    nn::ui2d::Size              m_MainViewportSize;

    //----------------------------------------------
    // アプリケーション状態(★要検討:main.cpp に移動する方が適切かもしれない)
    bool                        m_IsSrgbWriteEnabled;
    bool                        m_IsAlwaysAlignment;
    Fps                         m_Fps;
    float                       m_BackgroundColor[4];
    AnimationPlayingState       m_OldAnimationPlayingState;

    //----------------------------------------------
    // コールバック関連
    LoadFunction                            m_LoadFunction;
    UnloadFunction                          m_UnloadFunction;
    GetUserShaderInformationFromUserData    m_GetUserShaderInformationFromUserDataCallback;
    PreReloadLayoutDataCallback             m_PreReloadLayoutDataCallback;
    PostReloadLayoutDataCallback            m_PostReloadLayoutDataCallback;

    //----------------------------------------------
    // サブスクリーン管理ステートマシン
    SubScreenStateMachine*                  m_pSubScreenStateMachine;
};

} // namespace viewer
} // namespace ui2d
} // namespace nn

#endif // NN_UI2D_VIEWER_ENABLED
