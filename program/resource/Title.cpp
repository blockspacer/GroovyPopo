
#include"main.h"
#include"texture.h"
#include"FaceGen.h"
#include"controller.h"
#include"Scene.h"
#include"sound.h"

#define MAXTITLEFADE (3)

static enum TITLEBOTTUN
{
	key_w,
	key_s,
	key_space,
	TITLEKEYMAX
};

static UINT g_TitleTex;
static UINT g_BottunTex;
static UINT g_FrameTex;
static UINT g_BottunDispTex;
static UINT g_TitleTextTex;

static bool g_IsTouch_title[TITLEKEYMAX] = {};
static int g_currentBottunPos;

static UINT g_Stage1Tex;
static UINT g_Stage2Tex;
static UINT g_Stage3Tex;
static int g_CurrentDispStageNum;
static float g_AlphaNum;
static int g_NextStageCnt;
static bool g_IsChanging;

void TitleINIT(void)
{
	g_BottunTex = LoadTexture("asset/bottun.tga");
	g_FrameTex = LoadTexture("asset/Frame.tga");
	g_BottunDispTex = LoadTexture("asset/DispKey.tga");
	g_TitleTextTex = LoadTexture("asset/title_2.tga");
	g_Stage1Tex = LoadTexture("asset/stage_1_background.tga");
	g_Stage2Tex = LoadTexture("asset/stage_2_background.tga");
	g_Stage3Tex = LoadTexture("asset/stage_3_background.tga");
	g_CurrentDispStageNum = 0;
	g_AlphaNum = 0.0f;
	g_NextStageCnt = 0;
	g_IsChanging = false;

	memset(g_IsTouch_title, false, sizeof(g_IsTouch_title));
	g_currentBottunPos = 0;
}

void TitleUPDATE(void)
{
	if (GetKeyState(VK_SPACE) & 0x80)
	{
		if (!g_IsTouch_title[key_space])
		{
			if (g_currentBottunPos == 0)
			{
				PlaySE(SE_POP);
				//ステージ1へ。
				SetNextStage(scene_game);
			}
			else
			{
				UNINIT();
			}
			g_IsTouch_title[key_space] = true;
		}
	}
	else
	{
		g_IsTouch_title[key_space] = false;
	}

	if (GetKeyState('W') & 0x80)
	{
		if (!g_IsTouch_title[key_w])
		{
			PlaySE(SE_FINGER);

			g_currentBottunPos = (g_currentBottunPos + 1) % 2;

			g_IsTouch_title[key_w] = true;
		}
	}
	else
	{
		g_IsTouch_title[key_w] = false;
	}

	if (GetKeyState('S') & 0x80)
	{
		if (!g_IsTouch_title[key_s])
		{
			PlaySE(SE_FINGER);

			g_currentBottunPos = (g_currentBottunPos - 1) % 2;
			if (g_currentBottunPos < 0)g_currentBottunPos *= -1;

			g_IsTouch_title[key_s] = true;
		}
	}
	else
	{
		g_IsTouch_title[key_s] = false;
	}

	if (!g_IsChanging)
	{
		g_NextStageCnt++;
		if (g_NextStageCnt >= 60 * 5)
		{
			g_IsChanging = true;

			g_NextStageCnt = 0;
		}
	}
	else
	{
		g_AlphaNum += 0.01f;

		if (g_AlphaNum >= 1.0f)
		{
			g_AlphaNum = 0.0f;

			g_IsChanging = false;

			g_CurrentDispStageNum = (g_CurrentDispStageNum + 1) % 3;
		}
	}
}

void TitleDRAW(void)
{
	//背景

	UINT texID = g_CurrentDispStageNum == 0 ? g_Stage1Tex :
		g_CurrentDispStageNum == 1 ? g_Stage2Tex : g_Stage3Tex;

	//下の背景
	FaceGenforTex(MakeFloat2(0, 0), MakeFloat2(SCREEN_WIDTH, SCREEN_HEIGHT), 0, 0, 1, 1, true, texID, MakeFloat4(1, 1, 1, 1));

	UINT texID_2 = g_CurrentDispStageNum == 0 ? g_Stage2Tex :
		g_CurrentDispStageNum == 1 ? g_Stage3Tex : g_Stage1Tex;

	//上の背景
	FaceGenforTex(MakeFloat2(0, 0), MakeFloat2(SCREEN_WIDTH, SCREEN_HEIGHT), 0, 0, 1, 1, true, texID_2, MakeFloat4(1, 1, 1, g_AlphaNum));



	//現在選択しているボタンのフレーム表示
	FaceGenforTex(MakeFloat2(0, 256 * g_currentBottunPos),
		MakeFloat2(512, 128), 0, 0, 1, 1, true, g_FrameTex, MakeFloat4(1, 1, 1, 1));

	//ボタン　始める && 終わる
	for (int i = 0; i < 2; i++)
	{
		FaceGenforTex(MakeFloat2(0, 256 * i), MakeFloat2(512, 128), 
			0, i, 1, 4, true, g_BottunTex, MakeFloat4(1, 1, 1, 1));
	}

	//決定キー
	FaceGenforTex(MakeFloat2(0, SCREEN_HEIGHT / 2 - ((1024 / 3) / 3) / 2), MakeFloat2(1024, (1024 / 3) / 3),
		0, 0, 1, 4, true, g_BottunDispTex, MakeFloat4(1, 1, 1, 1));

	//説明
	TextGen(MakeFloat2(0, SCREEN_HEIGHT / 2 - 160), 
		MakeFloat2(64, 64), NORMALCOLOR, "push tab to menu");

	//タイトル文字
	FaceGenforTex(MakeFloat2(0, -256), MakeFloat2(1024, 128), 0, 0, 1, 1, true, g_TitleTextTex, NORMALCOLOR);
}

void TitleUNINIT(void)
{
	UnloadTexture(g_TitleTex);
	UnloadTexture(g_BottunTex);
	UnloadTexture(g_FrameTex);
	UnloadTexture(g_BottunDispTex);
	UnloadTexture(g_TitleTextTex);

	g_TitleTex = NULL;
	g_BottunTex = NULL;
	g_FrameTex = NULL;
	g_BottunDispTex = NULL;
	g_TitleTextTex = NULL;
}
