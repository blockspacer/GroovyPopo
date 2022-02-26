#include"Scene.h"
#include"main.h"
#include"Title.h"
#include"Game.h"
#include"result.h"
#include"Ball.h"

static SCENE g_CurrentScene;
static SCENE g_NextStage = g_CurrentScene;
static STAGE g_CurrentStage = stage_3;
static int g_coinCnt = 0;

typedef void(*SceneFunction)(void);

static const SceneFunction g_pSceneInit[] =
{
	 TitleINIT,
	  GameINIT,
	ResultINIT,
};

static const SceneFunction g_pSceneUpdate[] =
{
	 TitleUPDATE,
	  GameUPDATE,
	ResultUPDATE,
};

static const SceneFunction g_pSceneDraw[] =
{
	 TitleDRAW,
	  GameDRAW,
	ResultDRAW,
};

static const SceneFunction g_pSceneUnInit[] =
{
	 TitleUNINIT,
	  GameUNINIT,
	ResultUNINIT,
};


void SceneINIT(void)
{
	g_pSceneInit[g_CurrentScene]();
}

void SceneUPDATE(void)
{
	g_pSceneUpdate[g_CurrentScene]();
}

void SceneDRAW(void)
{
	g_pSceneDraw[g_CurrentScene]();
}

void SceneUNINIT(void)
{
	g_pSceneUnInit[g_CurrentScene]();
}

SCENE GetCurrentScene(void)
{
	return g_CurrentScene;
}

void SetNextStage(SCENE nextstage)
{
	g_NextStage = nextstage;
}

STAGE GetCurrentStage(void)
{
	return g_CurrentStage;
}

void Replay(void)
{
	SceneINIT();
}

void ExecuteStage(void)
{
	if (g_CurrentScene == g_NextStage)return;

	if (g_CurrentScene == scene_game)
	{
		g_coinCnt = GetCoinNum();
	}


	SceneUNINIT();

	//前のゲームステージを記録する。
	if (g_CurrentScene == scene_game)g_CurrentStage = (STAGE)(g_CurrentStage + 1);

	g_CurrentScene = g_NextStage;

	SceneINIT();
}

int GetCoinNumScene(void)
{
	return g_coinCnt;
}

void CoinNumResetScene(void)
{
	g_coinCnt = 0;
}

void StageReset(void)
{
	g_CurrentStage = stage_1;
}