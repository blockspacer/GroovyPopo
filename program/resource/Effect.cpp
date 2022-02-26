
#include"texture.h"
#include"FaceGen.h"
#include"StageMaker.h"
#include"sound.h"

#define MAXEFFECT (64)
#define EXPLOTIONSIZE (MakeFloat2(64,64))
#define HEARTSIZE (MakeFloat2(512,512))
#define FIREWORKSSIZE (MakeFloat2(128,128))

enum EFFECTTYPE
{
	effecttype_heart,
	effecttype_explotion,
	effecttype_fireworks,
	effecttype_syabon,
	effecttype_fire,
};

typedef struct
{
	Float2 pos;
	Float2 size;
	int Cnt;
	int Frame;
	bool IsUse;
	EFFECTTYPE type;
}EFFECT;

void SetFireWorks();
void SetEffect(EFFECTTYPE type, Float2 pos);

static UINT g_ExplotionTex;
static UINT g_HeartExplotionTex;
static UINT g_GameFinTex;
static UINT g_FireworksTex;
static UINT g_SyabonTex;
static UINT g_PrincessTex;
static UINT g_FireTex;
static EFFECT g_effect[MAXEFFECT] = {};
static bool g_IsClear;
static bool g_IsGameover;
static int g_FireWorksCreateCnt;
static int g_PrincessCnt;
static int g_PrincessFrame;
static Float2 g_PrincessPos;
static bool g_PrincessIsUse;

void EffectINIT(void)
{
	memset(g_effect, 0, sizeof(g_effect));

	g_ExplotionTex = LoadTexture("asset/explosion_2.tga");
	g_HeartExplotionTex = LoadTexture("asset/heart_2.tga");
	g_GameFinTex = LoadTexture("asset/GameEnd.tga");
	g_FireworksTex = LoadTexture("asset/Fireworks.tga");
	g_SyabonTex = LoadTexture("asset/syabon_effect.tga");
	g_PrincessTex = LoadTexture("asset/Princess.tga");
	g_FireTex = LoadTexture("asset/afterfire.tga");

	g_IsClear = false;
	g_IsGameover = false;
	g_PrincessIsUse = false;

	g_PrincessCnt = 0;
	g_PrincessFrame = 0;
}

void EffectUPDATE(void)
{
	if (g_IsClear)
	{
		SetFireWorks();
	}

	if (g_PrincessIsUse)
	{
		g_PrincessCnt++;
		if (g_PrincessCnt > 3)
		{
			g_PrincessCnt = 0;
			g_PrincessFrame++;
		}
		if (g_PrincessFrame >= 3)
		{
			g_PrincessFrame = 0;
		}
	}

	for (int i = 0; i < MAXEFFECT; i++)
	{
		if (!g_effect[i].IsUse)continue;

		g_effect[i].Cnt++;

		if (g_effect[i].Cnt > 3)
		{
			g_effect[i].Cnt = 0;
			g_effect[i].Frame++;
		}

		int alfa = g_effect[i].type == effecttype_heart ? 12 :
			g_effect[i].type == effecttype_syabon ? 3 : 9;

		if (g_effect[i].Frame >= alfa)
		{
			g_effect[i].Frame = 0;
			g_effect[i].IsUse = false;

			if (g_effect[i].type == effecttype_heart)
			{
				PlaySE(SE_BREAK);
			}
		}
	}
}

void EffectDRAW(void)
{
	for (int i = 0; i < MAXEFFECT; i++)
	{
		if (!g_effect[i].IsUse)continue;

		//テクスチャのX分割数選択
		int alfa_x = 3;

		//カラー選択
		Float4 color = g_effect[i].type == effecttype_explotion ? MakeFloat4(0.8f, 0.3f, 0, 1) : NORMALCOLOR;

		//テクスチャ選択
		UINT texID = g_effect[i].type == effecttype_heart ? g_HeartExplotionTex :
			g_effect[i].type == effecttype_explotion ? g_ExplotionTex :
			g_effect[i].type == effecttype_fireworks ? g_FireworksTex :
			g_effect[i].type == effecttype_syabon ? g_SyabonTex : g_FireTex;

		//テクスチャのY分割数選択
		int alfa_y = g_effect[i].type == effecttype_heart ? 4 :
					 g_effect[i].type == effecttype_syabon ? 1 : 3;

		//表示
		FaceGenforTex(g_effect[i].pos, g_effect[i].size, g_effect[i].Frame % alfa_x,
			g_effect[i].Frame / alfa_x, alfa_x, alfa_y, true, texID, color);
	}

	//ゲームオーバー表示
	if (g_IsGameover)
	{
		FaceGenforTex(MakeFloat2(0,-SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4),MakeFloat2(1024,512),0,1,1,2,true,g_GameFinTex,MakeFloat4(1,1,1,1));
	}

	//クリア表示
	else if (g_IsClear)
	{
		FaceGenforTex(MakeFloat2(0, -SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4), 
			MakeFloat2(1024, 512), 0, 0, 1, 2, true, g_GameFinTex, MakeFloat4(1, 1, 1, 1));
	}

	if(g_PrincessIsUse)
	{
		FaceGenforTex(g_PrincessPos,BLOCKSIZE, g_PrincessFrame, 0, 3, 1, true, g_PrincessTex, MakeFloat4(1, 1, 1, 1));
	}
}

void SetPrincess(Float2 pos)
{
	g_PrincessPos = pos;
	g_PrincessIsUse = true;
}

void EffectUNINIT(void)
{
	UnloadTexture(g_ExplotionTex);
	UnloadTexture(g_HeartExplotionTex);
	UnloadTexture(g_GameFinTex);
	UnloadTexture(g_FireworksTex);
	UnloadTexture(g_SyabonTex);
	UnloadTexture(g_PrincessTex);
	UnloadTexture(g_FireTex);

	g_ExplotionTex = NULL;
	g_HeartExplotionTex = NULL;
	g_GameFinTex = NULL;
	g_FireworksTex = NULL;
	g_SyabonTex = NULL;
	g_PrincessTex = NULL;
	g_FireTex = NULL;
}

void SetSyabonBreak(Float2 pos)
{
	SetEffect(effecttype_syabon, pos);
}

void SetExplosion(Float2 pos)
{
	SetEffect(effecttype_explotion, pos);
}

void SetHeart(Float2 pos)
{
	SetEffect(effecttype_heart, pos);
}

void SetFireWorks(void)
{
	//PlaySE(SE_FIREWORK);

	SetEffect(effecttype_fireworks, MakeFloat2(0, 0));
}

void SetFire(Float2 pos)
{
	SetEffect(effecttype_fire,pos);
}

void SetEffect(EFFECTTYPE type, Float2 pos)
{
	for (int i = 0; i < MAXEFFECT; i++)
	{
		if (g_effect[i].IsUse)continue;

		bool isSetFire = false;
		if (type == effecttype_fireworks)
		{
			g_FireWorksCreateCnt++;
			if (g_FireWorksCreateCnt > 3)
			{
				//ランダムなポジションをセット
				pos.x = GetRandomNum(0, SCREEN_WIDTH / 2 - FIREWORKSSIZE.x);
				pos.y = GetRandomNum(0, SCREEN_HEIGHT / 2 - FIREWORKSSIZE.y);

				if (GetRandomNum(0, 2) == 0) pos.x *= -1;
				if (GetRandomNum(0, 2) == 0) pos.y *= -1;

				g_FireWorksCreateCnt = 0;
			}
			else
			{
				isSetFire = true;
			}
		}

		if (!isSetFire)
		{
			g_effect[i].pos = pos;
			g_effect[i].size = type == effecttype_heart ? HEARTSIZE : 
							   type == effecttype_explotion ? EXPLOTIONSIZE : 
							   type == effecttype_fire ? BLOCKSIZE : FIREWORKSSIZE;

			g_effect[i].IsUse = true;
			g_effect[i].type = type;
		}

		break;
	}
}

void SetGameOver(void)
{
	if (!g_IsGameover)g_IsGameover = true;
}

void SetClear(void)
{
	if (!g_IsClear)g_IsClear = true;
}

bool GetIsClear(void)
{
	return g_IsClear;
}

bool GetIsGameover(void)
{
	return g_IsGameover;
}

int getnum(void)
{
	return 0;
}
