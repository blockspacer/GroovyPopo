
#include"main.h"
#include"texture.h"
#include"FaceGen.h"

static int g_Score;

void ScoreINIT(void)
{
	g_Score = 0;
}

void ScoreUPDATE(void)
{

}

void ScoreDRAW(void)
{
	//FaceGenforTex(MakeFloat2(0, 0), MakeFloat2(64, 64), 0, 0, 1, 1, true,g_ScoreTex,NORMALCOLOR);
}

void ScoreUNINIT(void)
{

}

void SetScore(int setnum)
{
	g_Score += setnum;
}
