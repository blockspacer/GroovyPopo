#ifndef SCENE_H_
#define SCENE_H_

enum SCENE
{
	scene_title,
	scene_game,
	scene_result,

	SCENEMAX
};

static enum STAGE
{
	stage_1,
	stage_2,
	stage_3,
	stage_4,

	STAGEMAX
};

void SceneINIT(void);
void SceneUPDATE(void);
void SceneDRAW(void);
void SceneUNINIT(void);

void Replay(void);
SCENE GetCurrentScene(void);
STAGE GetCurrentStage(void);
void ExecuteStage(void);
void SetNextStage(SCENE nextstage);
int GetCoinNumScene(void);
void CoinNumResetScene(void);
void StageReset(void);

#endif