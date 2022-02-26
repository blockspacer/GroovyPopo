
/*★★★★★★★★★★★★★★★★★★★★★★★★★★★★

	main.cpp

	メモ：

★★★★★★★★★★★★★★★★★★★★★★★★★★★★*/

//===================================include
#include"main.h"
#include"mytime.h"
#include"controller.h"
#include"texture.h"
#include"Scene.h"
#include"FaceGen.h"
#include"sound.h"
//===================================include

//===================================enum
enum KEYMAIN
{
	mainkey_b,
	mainkey_p,
	mainkey_tab,

	MAINKEYMAX,
};
//===================================

//===================================プロトタイプ関数宣言
bool INIT(void);
void UPDATE(void);
void DRAW(void);
void UNINIT(void);
//===================================プロトタイプ関数宣言

//===================================グローバル変数
static bool g_IsTouch_main[MAINKEYMAX] = {};
static bool g_IsDebug;
static bool g_IsDispMenu;
static UINT g_MenuTex;
//===================================グローバル変数

// エントリー関数
extern "C" void nnMain()
{
	InitSound();

	// システムの初期化
	InitSystem();
	glEnable(GL_CULL_FACE); // カリングON
	glFrontFace(GL_CW);

	INIT();

	PlayBGM(STRM_BGM);

	// ゲームループ
	while (true)
	{
		GetTime();

		glClearColor(0.0f, 0.05f, 0.09f, 1.0f);		// 画⾯のクリア
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// 画⾯のクリア


		UPDATE();

		DRAW();

		ExecuteStage();
	}

	UNINIT();

	// システムの終了処理
	UninitSystem();
}

bool INIT()
{
	srand((unsigned)time(NULL));

	InitTime();

	SetVolumeBGM(0.01f);

	FacegenINIT();

	InitController();

	SceneINIT();

	g_MenuTex = LoadTexture("asset/option.tga");

	memset(g_IsTouch_main, false, sizeof(g_IsTouch_main));

	g_IsDebug = false;

	g_IsDispMenu = false;

	return true;
}

void UPDATE()
{
	UpdateController();

	UpdateSound();

	if (!g_IsDispMenu)
	{
		SceneUPDATE();
	}

	if (GetKeyState('B') & 0x80)
	{
		if (!g_IsTouch_main[mainkey_b])
		{
			g_IsDebug == true ? g_IsDebug = false : g_IsDebug = true;

			g_IsTouch_main[mainkey_b] = true;
		}
	}
	else
	{
		g_IsTouch_main[mainkey_b] = false;
	}

	if (GetKeyState('P') & 0x80)
	{
		if (!g_IsTouch_main[mainkey_p])
		{
			GetRandomNum(0, 2) == 0 ? PlaySE(SE_BABY) : PlaySE(SE_CAT);

			g_IsTouch_main[mainkey_p] = true;
		}
	}
	else
	{
		g_IsTouch_main[mainkey_p] = false;
	}

	if (GetKeyState(VK_TAB) & 0x80)
	{
		if (!g_IsTouch_main[mainkey_tab])
		{
			g_IsDispMenu = g_IsDispMenu ? false : true;

			g_IsTouch_main[mainkey_tab] = true;
		}
	}
	else
	{
		g_IsTouch_main[mainkey_tab] = false;
	}

	CheckTime();//FPSがオーバーしていないか確認
}

void DRAW()
{
	if (g_IsDispMenu)
	{
		//背景
		FaceGen(MakeFloat2(0, 0), MakeFloat2(0, 0), 0, 1, 1, false, 0, 'B', COLOR_BLACK);

		//操作方法
		FaceGenforTex(MakeFloat2(0, 0), MakeFloat2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 0, 0, 1, 1, true, g_MenuTex, NORMALCOLOR);

		//タブで戻る
		TextGen(MakeFloat2(0, SCREEN_HEIGHT / 2 - 64), MakeFloat2(64, 64), NORMALCOLOR, "Push Tab to back");
	}
	else
	{
		SceneDRAW();
	}

	SwapBuffers();// 画⾯バッファの切り替え
}

void UNINIT(void)
{
	SceneUNINIT();

	UninitController();

	FacegenUNINIT();

	UninitSound();

	UnloadTexture(g_MenuTex);

	exit(0);
}

bool GetIsDebug(void)
{
	return g_IsDebug;
}

bool GetIsDispMenu(void)
{
	return g_IsDispMenu;
}