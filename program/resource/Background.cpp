
#include"main.h"
#include"texture.h"
#include"FaceGen.h"
#include"Scene.h"

static UINT g_backgroundTex;

void backgroundINIT(void)
{
	char filename[32] = {};
	int alpha = GetCurrentStage() == stage_1 ? 1 :
		GetCurrentStage() == stage_2 ? 2 :
		GetCurrentStage() == stage_3 ? 3 : NULL;

	if (alpha != NULL)
	{
		sprintf(filename, "asset/stage_%d_background.tga", alpha);
		g_backgroundTex = LoadTexture(filename);
	}
	else
	{
		g_backgroundTex = NULL;
	}
}

void backgroundUPDATE(void)
{

}

void backgroundDRAW(void)
{
	if (g_backgroundTex != NULL)
	{
		FaceGenforTex(MakeFloat2(0, 0), MakeFloat2(SCREEN_WIDTH, SCREEN_HEIGHT), 0, 0, 1, 1, true, g_backgroundTex, NORMALCOLOR);
	}
}

void backgroundUNINIT(void)
{
	UnloadTexture(g_backgroundTex);

	g_backgroundTex = NULL;
}
