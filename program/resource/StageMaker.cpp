



//=================================
//
//爆発エフェクト
//
//ゲームモードにして作成。ゲームモードなら、既にブロックのある場所にはいけない。一個隣のブロックに行く。
//
//=================================


#include"main.h"
#include"texture.h"
#include"FaceGen.h"
#include"StageMaker.h"
#include"Mytype.h"
#include"Ball.h"
#include"Scene.h"
#include"Effect.h"

#define BLOCKTEXTURE_MAXWIDTHBLOCK (6)

enum KEY
{
	//移動
	KEY_A,
	KEY_D,
	KEY_W,
	KEY_S,

	KEY_R,//リセット
	KEY_T,//ステージ読み込み
	KEY_F,//削除
	KEY_E,//タイプ変更
	KEY_Q,//左回転
	KEY_V,//セーブ
	KEY_G,//セーブ
	KEY_SPACE,//設置
	KEY_ENTER,//ボール発射
	KEY_C,
	KEY_H,

	KEYMAX
};

enum PLAYKEY
{
	//移動
	PLAYKEY_A,
	PLAYKEY_D,
	PLAYKEY_W,
	PLAYKEY_S,

	PLAYKEY_F,
	PLAYKEY_E,
	PLAYKEY_Q,
	PLAYKEY_SPACE,
	PLAYKEY_ENTER,

	PLAYKEYMAX,
};

typedef struct
{
	int num;
	bool IsUse;
}ENABLEBLOCK;


void DeleteBlock(void);
void StageBlockReset(void);
int GetTubeNum(void);
bool CheckTubeNum(void);

static UINT g_CurrentFrameTex;
static int g_CurrentFrameCnt;
static int g_CurrentFrameFrame;
static UINT g_BlockTex;
static UINT g_PlayerFrameTex;
static BLOCK g_Block[MAX_BLOCK_HEIGHT][MAX_BLOCK_WIDTH + 1] = {};
static bool g_IsTouch_StageMaker[KEYMAX] = {};
static bool g_IsTouch_StageMaker_Game[PLAYKEYMAX] = {};
static int g_IsTouchTime[KEYMAX] = {};
static BLOCK g_CurrentBlock;
static int g_TubeTurnNum;
static UINT g_numtex;
static int g_GoalCnt;
static int g_GoalFrame;
static int g_CoinCnt;
static int g_CoinFrame;
static char g_filename[32] = {};
static char g_filename_2[32] = {};

static ENABLEBLOCK g_Item[TYPEMAX] = {};


void StageBlockINIT(void)
{
	for (int i = 0; i < MAX_BLOCK_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_BLOCK_WIDTH; k++)
		{
			g_Block[i][k].fpos = MakeFloat2(-SCREEN_WIDTH / 2 + BLOCKSIZE.x / 2, -SCREEN_HEIGHT / 2 + BLOCKSIZE.x / 2);
			g_Block[i][k].npos = MakeInt2(0, 0);
			g_Block[i][k].isUse = false;
			g_Block[i][k].type = type_normal;
			g_Block[i][k].dir = dir_top;
			g_Block[i][k].warp_turn_num = -1;
			g_Block[i][k].fireCnt = 0;
			g_Block[i][k].IsBurn = false;
			g_Block[i][k].IsPlayerBlock = false;
		}
	}

	//Current表示ブロック
	g_CurrentBlock.fpos = MakeFloat2(-SCREEN_WIDTH / 2 + BLOCKSIZE.x / 2, -SCREEN_HEIGHT / 2 + BLOCKSIZE.x / 2);
	g_CurrentBlock.npos = MakeInt2(0, 0);
	g_CurrentBlock.isUse = false;

	g_CurrentFrameCnt = 0;
	g_CurrentFrameFrame = 0;
	g_GoalCnt = 0;
	g_GoalFrame = 0;
	g_CoinCnt = 0;
	g_CoinFrame = 0;

	g_BlockTex = LoadTexture("asset/Block_4.tga");
	g_CurrentFrameTex = LoadTexture("asset/choose_2.tga");
	g_numtex = LoadTexture("asset/num.tga");
	g_PlayerFrameTex = LoadTexture("asset/Chooseframe.tga");

	memset(g_IsTouch_StageMaker, false, sizeof(g_IsTouch_StageMaker));
	memset(g_IsTouchTime, 0, sizeof(g_IsTouchTime));
	memset(g_filename, 0, sizeof(g_filename));

	g_TubeTurnNum = 0;

	//ステージ読み込み
	const char* filename = GetCurrentStage() == stage_1 ? "asset/stage1.bin" :
		GetCurrentStage() == stage_2 ? "asset/stage2.bin" :
		GetCurrentStage() == stage_3 ? "asset/stage3.bin" : "asset/stage4.bin";

	strcpy(g_filename, filename);

	//ステージ読み込み
	FILE* fp = fopen(filename, "rb");
	if (fp != NULL)
	{
		fread(g_Block, sizeof(g_Block), 1, fp);
		fclose(fp);
	}

	for (int i = 0; i < MAX_BLOCK_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_BLOCK_WIDTH; k++)
		{
			if (g_Block[i][k].type == type_tube_in ||
				g_Block[i][k].type == type_tube_out)
			{
				g_TubeTurnNum++;
			}
			g_Block[i][k].IsBurn = false;
			g_Block[i][k].IsPlayerBlock = false;
		}
	}

	memset(g_IsTouch_StageMaker_Game, false, sizeof(g_IsTouch_StageMaker_Game));
	memset(g_Item, 0, sizeof(g_Item));

	//アイテム読み込み
	const char* filename_2 = GetCurrentStage() == stage_1 ? "asset/data/Stage_1_Item.bin" :
		GetCurrentStage() == stage_2 ? "asset/data/Stage_2_Item.bin" :
		GetCurrentStage() == stage_3 ? "asset/data/Stage_3_Item.bin" : "asset/data/Stage_4_Item.bin";

	strcpy(g_filename_2, filename_2);

	FILE* itemfp = fopen(g_filename_2, "rb");
	fread(g_Item, sizeof(g_Item), 1, itemfp);
	fclose(itemfp);

	//最初に持つアイテムを決定
	bool isfirst = true;
	for (int i = 0; i < TYPEMAX; i++)
	{
		if (g_Item[i].IsUse)
		{
			if (isfirst)
			{
				g_CurrentBlock.type = (BLOCKTYPE)(i);
				break;
			}
		}
	}
}

void StageBlockUPDATE(void)
{
	if (!(GetIsClear() || GetIsGameover()))
	{
		//===================================================左移動
		if (GetKeyState('A') & 0x80)
		{
			if (!g_IsTouch_StageMaker[KEY_A])
			{
				if (g_CurrentBlock.npos.x > 0)
				{
					g_CurrentBlock.fpos.x -= BLOCKSIZE.x;
					g_CurrentBlock.npos.x -= 1;
				}
				g_IsTouch_StageMaker[KEY_A] = true;
			}

			g_IsTouchTime[KEY_A]++;
			if (g_IsTouchTime[KEY_A] > 30)
			{
				g_IsTouch_StageMaker[KEY_A] = false;
			}
		}
		else
		{
			g_IsTouchTime[KEY_A] = 0;
			g_IsTouch_StageMaker[KEY_A] = false;
		}

		//===================================================右移動
		if (GetKeyState('D') & 0x80)
		{
			if (!g_IsTouch_StageMaker[KEY_D])
			{
				if (g_CurrentBlock.npos.x < MAX_BLOCK_WIDTH - 1)
				{
					g_CurrentBlock.fpos.x += BLOCKSIZE.x;
					g_CurrentBlock.npos.x += 1;
				}
				g_IsTouch_StageMaker[KEY_D] = true;
			}

			g_IsTouchTime[KEY_D]++;
			if (g_IsTouchTime[KEY_D] > 30)
			{
				g_IsTouch_StageMaker[KEY_D] = false;
			}
		}
		else
		{
			g_IsTouchTime[KEY_D] = 0;
			g_IsTouch_StageMaker[KEY_D] = false;
		}

		//===================================================上移動
		if (GetKeyState('W') & 0x80)
		{
			if (!g_IsTouch_StageMaker[KEY_W])
			{
				if (g_CurrentBlock.npos.y > 0)
				{
					g_CurrentBlock.fpos.y -= BLOCKSIZE.y;
					g_CurrentBlock.npos.y -= 1;
				}
				g_IsTouch_StageMaker[KEY_W] = true;
			}

			g_IsTouchTime[KEY_W]++;
			if (g_IsTouchTime[KEY_W] > 30)
			{
				g_IsTouch_StageMaker[KEY_W] = false;
			}
		}
		else
		{
			g_IsTouchTime[KEY_W] = 0;
			g_IsTouch_StageMaker[KEY_W] = false;
		}

		//===================================================下移動
		if (GetKeyState('S') & 0x80)
		{
			if (!g_IsTouch_StageMaker[KEY_S])
			{
				if (g_CurrentBlock.npos.y < MAX_BLOCK_HEIGHT - 1)
				{
					g_CurrentBlock.fpos.y += BLOCKSIZE.y;
					g_CurrentBlock.npos.y += 1;
				}
				g_IsTouch_StageMaker[KEY_S] = true;
			}

			g_IsTouchTime[KEY_S]++;
			if (g_IsTouchTime[KEY_S] > 30)
			{
				g_IsTouch_StageMaker[KEY_S] = false;
			}
		}
		else
		{
			g_IsTouchTime[KEY_S] = 0;
			g_IsTouch_StageMaker[KEY_S] = false;
		}

		//===================================================設置
		int tubenum = 0;
		if (GetKeyState(VK_SPACE) & 0x80)
		{
			if (!g_IsTouch_StageMaker[KEY_SPACE] && !GetIsBallMoving())
			{
				//設置するアイテムが1個以上あったら設置できる
				if (g_Item[g_CurrentBlock.type].num > 0 &&
					!g_Block[g_CurrentBlock.npos.y][g_CurrentBlock.npos.x].isUse)
				{
					//設置場所に既にブロックがあるか確認　あったら消す	置こうとしているアイテムがあるかどうかを確認する　自分の置いたブロックはハイライトする
					DeleteBlock();

					//設置
					g_Block[g_CurrentBlock.npos.y][g_CurrentBlock.npos.x].isUse = true;
					g_Block[g_CurrentBlock.npos.y][g_CurrentBlock.npos.x].fpos = g_CurrentBlock.fpos;
					g_Block[g_CurrentBlock.npos.y][g_CurrentBlock.npos.x].npos = g_CurrentBlock.npos;
					g_Block[g_CurrentBlock.npos.y][g_CurrentBlock.npos.x].type = g_CurrentBlock.type;
					g_Block[g_CurrentBlock.npos.y][g_CurrentBlock.npos.x].dir = g_CurrentBlock.dir;
					g_Block[g_CurrentBlock.npos.y][g_CurrentBlock.npos.x].IsPlayerBlock = true;

					g_Item[g_CurrentBlock.type].num--;
				}
				g_IsTouch_StageMaker[KEY_SPACE] = true;
			}
		}
		else
		{
			g_IsTouch_StageMaker[KEY_SPACE] = false;
		}

		//===================================================ボール実行
		if (GetKeyState(VK_RETURN) & 0x80)
		{
			if (!g_IsTouch_StageMaker[KEY_ENTER])
			{
				if (!GetIsBallMoving())
				{
					BallReset();

					//ボール発射
					SetBall();
				}
			}
			g_IsTouch_StageMaker[KEY_ENTER] = true;
		}
		else
		{
			g_IsTouch_StageMaker[KEY_ENTER] = false;
		}
		//===================================================削除
		if (GetKeyState('F') & 0x80)
		{
			if (!g_IsTouch_StageMaker[KEY_F])//自分が置いたブロックなら壊せる mainasu
			{
				DeleteBlock();

				g_IsTouch_StageMaker[KEY_F] = true;
			}
		}
		else
		{
			g_IsTouch_StageMaker[KEY_F] = false;
		}

		//===================================================タイプ変更
		if (GetKeyState('E') & 0x80)
		{
			if (!g_IsTouch_StageMaker[KEY_E])//自分の持っているアイテムだけを表示
			{
				for (int i = g_CurrentBlock.type; TYPEMAX; i++)
				{
					if (i > TYPEMAX)
					{
						i = 0;
					}

					if (!g_Item[i].IsUse)continue;

					if (g_CurrentBlock.type == (BLOCKTYPE)(i))continue;

					g_CurrentBlock.type = (BLOCKTYPE)(i);
					break;
				}
				g_IsTouch_StageMaker[KEY_E] = true;
			}
		}
		else
		{
			g_IsTouch_StageMaker[KEY_E] = false;
		}

		//===================================================右回転
		if (GetKeyState('Q') & 0x80)
		{
			if (!g_IsTouch_StageMaker[KEY_Q])
			{
				g_CurrentBlock.dir = (DIR)(g_CurrentBlock.dir + 1);
				if (g_CurrentBlock.dir >= DIRMAX)
				{
					g_CurrentBlock.dir = dir_top;
				}
				g_IsTouch_StageMaker[KEY_Q] = true;
			}
		}
		else
		{
			g_IsTouch_StageMaker[KEY_Q] = false;
		}
	}
	//===================================================ステージ初期化
	if (GetKeyState('T') & 0x80 && GetKeyState(VK_CONTROL) & 0x80)
	{
		if (!g_IsTouch_StageMaker[KEY_T])
		{
			StageBlockReset();
			BallReset();

			g_IsTouch_StageMaker[KEY_T] = true;
		}
	}
	else
	{
		g_IsTouch_StageMaker[KEY_T] = false;
	}

	if (GetIsBallMoving())
	{
		//燃え移り。
		for (int i = 0; i < MAX_BLOCK_HEIGHT; i++)
		{
			for (int k = 0; k < MAX_BLOCK_WIDTH; k++)
			{
				if (!g_Block[i][k].isUse)continue;

				if (g_Block[i][k].type == type_grass &&
					g_Block[i][k].IsBurn)
				{
					g_Block[i][k].fireCnt++;
				}
				//燃え終わり。
				if (g_Block[i][k].fireCnt >= 30)
				{
					g_Block[i][k].isUse = false;
					g_Block[i][k].fireCnt = 0;

					//燃え移り。
					for (int m = 0; m < 3; m++)
					{
						for (int n = 0; n < 3; n++)
						{
							if (g_Block[i - 1 + m][k - 1 + n].type == type_grass &&
								!g_Block[i - 1 + m][k - 1 + n].IsBurn)
							{
								if (i == m && k == k)continue;
								g_Block[i - 1 + m][k - 1 + n].IsBurn = true;
								g_Block[i - 1 + m][k - 1 + n].fireCnt = 0;

								SetFire(g_Block[i - 1 + m][k - 1 + n].fpos);
							}
						}
					}
				}
			}
		}
	}

	//カレントマークアニメーション
	g_CurrentFrameCnt++;
	if (g_CurrentFrameCnt > 10)
	{
		g_CurrentFrameCnt = 0;
		g_CurrentFrameFrame++;
	}
	if (g_CurrentFrameFrame >= 3)
	{
		g_CurrentFrameFrame = 0;
	}

	//ゴールアニメーションカウント
	g_GoalCnt++;
	if (g_GoalCnt > 12)
	{
		g_GoalCnt = 0;
		g_GoalFrame++;
	}
	if (g_GoalFrame >= 3)
	{
		g_GoalFrame = 0;
	}

	g_CoinCnt++;
	if (g_CoinCnt > 12)
	{
		g_CoinCnt = 0;
		g_CoinFrame++;
	}
	if (g_CoinFrame >= 2)
	{
		g_CoinFrame = 0;
	}
}

bool CheckTubeNum(void)
{
	int InNum = 0;
	int OutNum = 0;

	for (int i = 0; i < MAX_BLOCK_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_BLOCK_WIDTH; k++)
		{
			if (!g_Block[i][k].isUse)continue;

			if (g_Block[i][k].type == type_tube_in)InNum++;
			else if (g_Block[i][k].type == type_tube_out)OutNum++;
		}
	}
	if (InNum == OutNum)return true;
	else return false;
}

int GetTubeNum(void)
{
	int cnt = 0;

	if (g_CurrentBlock.type == type_tube_in || g_CurrentBlock.type == type_tube_out)
	{
		for (int i = 0; i < MAX_BLOCK_HEIGHT; i++)
		{
			for (int k = 0; k < MAX_BLOCK_WIDTH; k++)
			{
				if (!g_Block[i][k].isUse)continue;

				if (g_Block[i][k].type == type_tube_in || g_Block[i][k].type == type_tube_out)cnt++;
			}
		}
	}
	return cnt;
}

BLOCK GetNextTubeBlock(int tubenum)
{
	for (int i = 0; i < MAX_BLOCK_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_BLOCK_WIDTH; k++)
		{
			if (!g_Block[i][k].isUse)continue;

			if (g_Block[i][k].type == type_tube_in || g_Block[i][k].type == type_tube_out)
			{
				if (g_Block[i][k].warp_turn_num == tubenum)
				{
					return g_Block[i][k];
				}
			}
		}
	}
	BLOCK block;
	memset(&block, -1, sizeof(BLOCK));
	return block;
}

void StageBlockDRAW(void)
{
	for (int i = 0; i < MAX_BLOCK_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_BLOCK_WIDTH; k++)
		{
			if (!g_Block[i][k].isUse)continue;

			//ゴールとコインだけアニメーションさせる
			int texanimationcnt = 0;
			if (g_Block[i][k].type == type_goal_1)
			{
				texanimationcnt = g_GoalFrame;
			}
			if (g_Block[i][k].type == type_coin_1)
			{
				texanimationcnt = g_CoinFrame;
			}

			//ブロック本体
			FaceGenforTex(g_Block[i][k].fpos, BLOCKSIZE, (g_Block[i][k].type + texanimationcnt) % BLOCKTEXTURE_MAXWIDTHBLOCK,
				(g_Block[i][k].type + texanimationcnt) / BLOCKTEXTURE_MAXWIDTHBLOCK, BLOCKTEXTURE_MAXWIDTHBLOCK, 3, true,
				g_BlockTex, MakeFloat4(1, 1, 1, 1), g_Block[i][k].dir);

			//プレイヤーの置いたブロックならハイライト
			if (g_Block[i][k].IsPlayerBlock)
			{
				FaceGenforTex(g_Block[i][k].fpos, MakeFloat2(BLOCKSIZE.x - 5.0f, BLOCKSIZE.y - 5.0f), 0, 0, 1, 1, true, g_PlayerFrameTex, NORMALCOLOR);
			}

			//デバッグなら番号を表示
			if ((g_Block[i][k].type == type_tube_in || g_Block[i][k].type == type_tube_out) && GetIsDebug())
			{
				char numtext[16] = {};
				sprintf(numtext, "%d", g_Block[i][k].warp_turn_num);
				TextGen(g_Block[i][k].fpos, BLOCKSIZE, NORMALCOLOR, numtext);
			}
		}
	}

	//ボールが動いていなかったらカレントのブロックとマークを表示する。
	if (!GetIsBallMoving())
	{
		//カレントブロック表示
		FaceGenforTex(g_CurrentBlock.fpos, BLOCKSIZE, g_CurrentBlock.type % BLOCKTEXTURE_MAXWIDTHBLOCK,
			g_CurrentBlock.type / BLOCKTEXTURE_MAXWIDTHBLOCK, BLOCKTEXTURE_MAXWIDTHBLOCK, 3, true, g_BlockTex, MakeFloat4(1, 1, 1, 0.6f), g_CurrentBlock.dir);


		//カレントマークカラーを見えやすいように設定
		Float4 color = g_CurrentBlock.type == type_normal ? MakeFloat4(1, 0.5f, 0, 1) :
			g_CurrentBlock.type == type_dirt ? MakeFloat4(1, 1, 1, 1) :
			g_CurrentBlock.type == type_grass ? MakeFloat4(1, 0, 0, 1) :
			g_CurrentBlock.type == type_frame ? MakeFloat4(0.8f, 0.8f, 0, 1) : MakeFloat4(1, 1, 1, 1);

		//カレントマーク表示
		FaceGenforTex(g_CurrentBlock.fpos, BLOCKSIZE, g_CurrentFrameFrame, 0, 3, 1, true, g_CurrentFrameTex, color);
	}
}

void StageBlockReset(void)
{
	//ステージ読み込み
	FILE* fp = fopen(g_filename, "rb");
	if (fp != NULL)
	{
		fread(g_Block, sizeof(g_Block), 1, fp);
		fclose(fp);
	}

	for (int i = 0; i < MAX_BLOCK_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_BLOCK_WIDTH; k++)
		{
			g_Block[i][k].IsPlayerBlock = false;
		}
	}

	FILE* itemfp = fopen(g_filename_2, "rb");
	fread(g_Item, sizeof(g_Item), 1, itemfp);
	fclose(itemfp);

	//最初に持つアイテムを決定
	bool isfirst = true;
	for (int i = 0; i < TYPEMAX; i++)
	{
		if (g_Item[i].IsUse)
		{
			if (isfirst)
			{
				g_CurrentBlock.type = (BLOCKTYPE)(i);
				break;
			}
		}
	}
}

void DestroyBlock(int height, int width)
{
	g_Block[height][width].isUse = false;
	//エフェクト
}

//OutTubeNumはボールを出現させるTUBEのナンバー
//TUBEのナンバーは置いた順。
Int2 GetTubeEscapePos(int OutTubeNum)
{
	Int2 alfa;

	for (int i = 0; i < MAX_BLOCK_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_BLOCK_WIDTH; k++)
		{
			if (!g_Block[i][k].isUse)continue;

			if (g_Block[i][k].type == type_tube_in || g_Block[i][k].type == type_tube_out)
			{
				if (g_Block[i][k].warp_turn_num != OutTubeNum)continue;

				alfa.y = i;
				alfa.x = k;

				return alfa;
			}
		}
	}

	alfa.x = -1;
	alfa.y = -1;
	return alfa;
}

void SetBurn(int height, int width)
{
	g_Block[height][width].IsBurn = true;
}

void BombBlockExplotion(void)
{
	for (int i = 0; i < MAX_BLOCK_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_BLOCK_WIDTH; k++)
		{
			if (!g_Block[i][k].isUse)continue;

			if (g_Block[i][k].type != type_move)continue;

			g_Block[i][k].isUse = false;

			//爆発エフェクト
			SetExplosion(g_Block[i][k].fpos);
		}
	}
}

void DeleteBlock(void)
{
	for (int i = 0; i < MAX_BLOCK_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_BLOCK_WIDTH; k++)
		{
			if (GetIsDebug())
			{
				if ((g_Block[i][k].npos.x == g_CurrentBlock.npos.x) &&
					(g_Block[i][k].npos.y == g_CurrentBlock.npos.y) &&
					(g_Block[i][k].isUse))
				{
					g_Block[i][k].isUse = false;
					g_Block[i][k].npos = MakeInt2(0, 0);
					g_Block[i][k].fpos = MakeFloat2(0, 0);
					g_Block[i][k].type = type_normal;

					break;
				}
			}
			else
			{
				if ((g_Block[i][k].npos.x == g_CurrentBlock.npos.x) &&
					(g_Block[i][k].npos.y == g_CurrentBlock.npos.y) &&
					(g_Block[i][k].isUse) &&
					g_Block[i][k].IsPlayerBlock)
				{
					g_Block[i][k].IsPlayerBlock = false;
					g_Item[g_Block[i][k].type].num++;

					g_Block[i][k].isUse = false;
					g_Block[i][k].npos = MakeInt2(0, 0);
					g_Block[i][k].fpos = MakeFloat2(0, 0);
					g_Block[i][k].type = type_normal;

					break;
				}
			}
		}
	}
}

BLOCK GetBlock(int height, int width)
{
	return g_Block[height][width];
}

void StageBlockUNINIT(void)
{
	UnloadTexture(g_CurrentFrameTex);
	UnloadTexture(g_BlockTex);
	UnloadTexture(g_numtex);
	UnloadTexture(g_PlayerFrameTex);

	g_CurrentFrameTex = NULL;
	g_BlockTex = NULL;
	g_numtex = NULL;
	g_PlayerFrameTex = NULL;
}