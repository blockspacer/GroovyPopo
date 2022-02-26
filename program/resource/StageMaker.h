#ifndef STAGEMAKER_H_
#define STAGEMAKER_H_

#include"main.h"
#include"Mytype.h"

#define MAX_BLOCK_WIDTH (32)
#define MAX_BLOCK_HEIGHT (18)
#define MAXBLOCK (MAX_BLOCK_WIDTH * MAX_BLOCK_HEIGHT)
#define BLOCKSIZE (MakeFloat2(SCREEN_WIDTH / MAX_BLOCK_WIDTH,SCREEN_WIDTH / MAX_BLOCK_WIDTH))

enum BLOCKTYPE
{
	type_normal,
	type_dirt,
	type_grass,
	type_move,
	type_doubletube,
	type_water,
	type_frame,
	type_tube_in,
	type_tube_out,
	type_needle,
	type_bottun,
	type_fire,
	type_coin_1,
	type_coin_2,
	type_fast,
	type_goal_1,
	type_goal_2,
	type_goal_3,

	TYPEMAX
};

typedef struct
{
	DIR x;
	DIR y;
}DIR2;

typedef struct
{
	Float2 fpos;
	Int2 npos;
	bool isUse;
	BLOCKTYPE type;
	DIR dir;
	int warp_turn_num;
	int fireCnt;
	bool IsBurn;
	bool IsPlayerBlock;

}BLOCK;

void StageBlockINIT(void);
void StageBlockUPDATE(void);
void StageBlockDRAW(void);
void StageBlockUNINIT(void);

BLOCK GetNextTubeBlock(int tubenum);
void BombBlockExplotion(void);
void StageBlockReset(void);
Int2 GetTubeEscapePos(int OutTubeNum);
void DestroyBlock(int height, int width);
BLOCK GetBlock(int height, int width);
void SetBurn(int height, int width);

#endif