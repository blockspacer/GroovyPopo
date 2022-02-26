
#include"main.h"
#include"texture.h"
#include"FaceGen.h"
#include"Scene.h"
#include"Ball.h"

#define COINSIZE (MakeFloat2(96,96))

static UINT g_resultTex;
static UINT g_CoinTex;
static UINT g_BottunDispTex;
static UINT g_ResultTextTex;
static bool g_IsTouch_result;
static int g_coinCnt;
static int g_coinFrame;
static int g_CoinNum;
static int g_Point;

void ResultINIT(void)
{
	g_resultTex = LoadTexture("asset/result_2.tga");
	g_CoinTex = LoadTexture("asset/Coin.tga");
	g_BottunDispTex = LoadTexture("asset/DispKey.tga");
	g_ResultTextTex = LoadTexture("asset/result_text.tga");

	g_IsTouch_result = false;
	g_coinCnt = 0;
	g_coinFrame = 0;
	g_CoinNum = 0;

	GetCurrentStage();
}

void ResultUPDATE(void)
{
	if (GetCurrentStage() > stage_4)
	{
		if (GetKeyState(VK_RETURN) & 0x80)
		{
			if (!g_IsTouch_result)
			{
				if (GetCurrentStage() <= stage_4)
				{
					CoinNumResetScene();
					SetNextStage(scene_game);
				}
				else
				{
					CoinNumResetScene();
					StageReset();
					SetNextStage(scene_title);
				}

				g_IsTouch_result = true;
			}
		}
		else
		{
			g_IsTouch_result = false;
		}
	}
	else
	{
		if (GetKeyState(VK_SPACE) & 0x80)
		{
			if (!g_IsTouch_result)
			{
				if (GetCurrentStage() <= stage_4)
				{
					CoinNumResetScene();
					SetNextStage(scene_game);
				}
				else
				{
					CoinNumResetScene();
					StageReset();
					SetNextStage(scene_title);
				}

				g_IsTouch_result = true;
			}
		}
		else
		{
			g_IsTouch_result = false;
		}
	}


	g_coinCnt++;
	if (g_coinCnt > 30)
	{
		g_coinCnt = 0;
		g_coinFrame++;
	}
	if (g_coinFrame >= 2)
	{
		g_coinFrame = 0;
	}
}

void ResultDRAW(void)
{
	//背景
	FaceGen(MakeFloat2(0, 0), MakeFloat2(0, 0), 0, 1, 1, true, g_resultTex, 'B', NORMALCOLOR);

	//取得したコイン
	for (int i = 0; i < 3; i++)
	{
		if (i < GetCoinNumScene())
		{
			FaceGenforTex(MakeFloat2(-COINSIZE.x - COINSIZE.x / 2 + 96 * i + 32, 128), COINSIZE,
				g_coinFrame, 1, 3, 1, true, g_CoinTex, NORMALCOLOR);
		}
		else
		{
			FaceGenforTex(MakeFloat2(-COINSIZE.x - COINSIZE.x / 2 + 96 * i + 32, 128), COINSIZE,
				2, 1, 3, 1, true, g_CoinTex, NORMALCOLOR);
		}
	}

	if (GetCurrentStage() <= stage_4)
	{
		//決定キー
		FaceGenforTex(MakeFloat2(0, SCREEN_HEIGHT / 2 - ((1024 / 3) / 3) / 2), MakeFloat2(1024, (1024 / 3) / 3),
			0, 0, 1, 4, true, g_BottunDispTex, MakeFloat4(1, 1, 1, 1));
	}
	else
	{
		//決定キー
		FaceGenforTex(MakeFloat2(0, SCREEN_HEIGHT / 2 - ((1024 / 3) / 3) / 2), MakeFloat2(1024, (1024 / 3) / 3),
			0, 1, 1, 4, true, g_BottunDispTex, MakeFloat4(1, 1, 1, 1));
	}

	//評価文字
	FaceGenforTex(MakeFloat2(0,-64), MakeFloat2(256,256),
		3 - GetCoinNumScene(), 1, 4, 1, true, g_ResultTextTex, NORMALCOLOR);

	//取得したコインの数
	char alpha[32] = {};
	sprintf(alpha, "%d / 3",GetCoinNumScene());
	TextGen(MakeFloat2(0, 128 + 96 + 16), MakeFloat2(64, 64), NORMALCOLOR, alpha);
}

void ResultUNINIT(void)
{
	UnloadTexture(g_resultTex);
	UnloadTexture(g_CoinTex);
	UnloadTexture(g_BottunDispTex);
	UnloadTexture(g_ResultTextTex);

	g_resultTex = NULL;
	g_CoinTex = NULL;
	g_BottunDispTex = NULL;
	g_ResultTextTex = NULL;
}
