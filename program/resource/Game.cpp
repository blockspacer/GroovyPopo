
#include"main.h"
#include"Scene.h"
#include"StageMaker.h"
#include"Ball.h"
#include"Effect.h"
#include"Title.h"
#include"texture.h"
#include"FaceGen.h"
#include"paint.h"
#include"sound.h"
#include"Background.h"

static enum GAMEKEY
{
	gamekey_w,
	gamekey_s,
	gamekey_enter,
	gamekey_tab,

	GAMEKEYMAX,
};

static enum GAMEBOTTUN
{
	gamebottun_next,
	gamebottun_replay,
	gamebottun_end,

	GAMEBOTTUNMAX,
};

static bool g_IsTouch_game[GAMEKEYMAX] = {};
static GAMEBOTTUN g_CurrentBottun;
static UINT g_BottunTex;
static UINT g_FrameTex;
static UINT g_BottunDispTex;
static bool g_IsFirst;

void GameINIT(void)
{
	StageBlockINIT();
	BallINIT();
	EffectINIT();
	PaintINIT();
	backgroundINIT();

	memset(g_IsTouch_game, false, sizeof(g_IsTouch_game));

	g_CurrentBottun = gamebottun_next;
	g_IsFirst = false;

	g_BottunTex = LoadTexture("asset/bottun.tga");
	g_FrameTex = LoadTexture("asset/Frame.tga");
	g_BottunDispTex = LoadTexture("asset/DispKey.tga");
}

void GameUPDATE(void)
{
	StageBlockUPDATE();
	BallUPDATE();
	EffectUPDATE();
	PaintUPDATE();

	if (GetIsGameover())
	{
		if (!g_IsFirst)
		{
			if (g_CurrentBottun == gamebottun_next)
			{
				g_CurrentBottun = gamebottun_replay;
			}
			g_IsFirst = true;
		}
	}

	//クリアしていたらボタン操作
	if (GetIsClear() || GetIsGameover())
	{
		if (GetKeyState('W') & 0x80)
		{
			if (!g_IsTouch_game[gamekey_w])
			{
				PlaySE(SE_FINGER);

				g_CurrentBottun = (GAMEBOTTUN)((g_CurrentBottun - 1));

				if (GetIsClear())
				{
					if (g_CurrentBottun < gamebottun_next)
					{
						g_CurrentBottun = gamebottun_end;
					}
				}

				if (GetIsGameover())
				{
					if (g_CurrentBottun < gamebottun_replay)
					{
						g_CurrentBottun = gamebottun_end;
					}
				}


				g_IsTouch_game[gamekey_w] = true;
			}
		}
		else
		{
			g_IsTouch_game[gamekey_w] = false;
		}

		if (GetKeyState('S') & 0x80)
		{
			if (!g_IsTouch_game[gamekey_s])
			{
				PlaySE(SE_FINGER);

				g_CurrentBottun = (GAMEBOTTUN)((g_CurrentBottun + 1));

				if (GetIsClear())
				{
					if (g_CurrentBottun > gamebottun_end)
					{
						g_CurrentBottun = gamebottun_next;
					}
				}

				if (GetIsGameover())
				{
					if (g_CurrentBottun > gamebottun_end)
					{
						g_CurrentBottun = gamebottun_replay;
					}
				}

				g_IsTouch_game[gamekey_s] = true;
			}
		}
		else
		{
			g_IsTouch_game[gamekey_s] = false;
		}

		if (GetKeyState(VK_RETURN) & 0x80)
		{
			if (!g_IsTouch_game[gamekey_enter])
			{
				PlaySE(SE_POP);

				if (g_CurrentBottun == gamebottun_next)
				{
					SetNextStage(scene_result);
				}
				else if (g_CurrentBottun == gamebottun_replay)
				{
					Replay();
				}
				else
				{
					UNINIT();
				}

				g_IsTouch_game[gamekey_enter] = true;
			}
		}
		else
		{
			g_IsTouch_game[gamekey_enter] = false;
		}
	}
}

void GameDRAW(void)
{
	backgroundDRAW();
	StageBlockDRAW();
	BallDRAW();
	EffectDRAW();
	PaintDRAW();

	if (GetIsClear() || GetIsGameover())
	{
		//現在選択しているボタンのフレーム表示
		FaceGenforTex(MakeFloat2(0, 128 * (g_CurrentBottun + 1) + 32),
			MakeFloat2(512, 128), 0, 0, 1, 1, true, g_FrameTex, MakeFloat4(1, 1, 1, 1));

		if (GetIsClear())
		{
			//次へ
			FaceGenforTex(MakeFloat2(0, 128 + 32), MakeFloat2(512, 128),
				0, 2, 1, 4, true, g_BottunTex, MakeFloat4(1, 1, 1, 1));
		}

		//もう一度
		FaceGenforTex(MakeFloat2(0, 256 + 32), MakeFloat2(512, 128),
			0, 3, 1, 4, true, g_BottunTex, MakeFloat4(1, 1, 1, 1));

		//終了
		FaceGenforTex(MakeFloat2(0, 384 + 32), MakeFloat2(512, 128),
			0, 1, 1, 4, true, g_BottunTex, MakeFloat4(1, 1, 1, 1));

		int alpha = GetCurrentStage() < STAGEMAX - 1 ? 3 : 1;

		//決定キー
		FaceGenforTex(MakeFloat2(0, SCREEN_HEIGHT / 2 - ((1024 / 3) / 3) / 2), MakeFloat2(1024, (1024 / 3) / 3),
			0, alpha, 1, 4, true, g_BottunDispTex, MakeFloat4(1, 1, 1, 1));
	}

}

void GameUNINIT(void)
{
	backgroundUNINIT();
	EffectUNINIT();
	StageBlockUNINIT();
	BallUNINIT();
	PaintUNINIT();

	UnloadTexture(g_FrameTex);
	UnloadTexture(g_BottunTex);
	UnloadTexture(g_BottunDispTex);

	g_FrameTex = NULL;
	g_BottunTex = NULL;
	g_BottunDispTex = NULL;
}
