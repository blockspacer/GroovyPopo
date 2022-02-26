
//======================================
//
//動いていないことを取得してHPマイナス。
//
//出現ポジション上に
//
//======================================

#include"Ball.h"
#include"texture.h"
#include"FaceGen.h"
#include"StageMaker.h"
#include"Effect.h"
#include"sound.h"
#include"result.h"

#define GRAVITY (0.1f)
#define RESISTANCE (0.75f)
#define BALLSIZE (MakeFloat2(32,32))
#define BALLSTARTPOS (MakeFloat2(-SCREEN_WIDTH / 2 + BLOCKSIZE.x + BLOCKSIZE.x / 2 + BLOCKSIZE.x * 3,-SCREEN_HEIGHT / 2 - BALLSIZE.y))

#define BALLADJUST_Y (0.5f)
#define FIRETIME (300)

void SetBallPosNtoF(void);
void SetTubeOutPos(int height, int width);
void SetBoundEffects(int height, int width, DIR balldir);
int GetNextTubeNum(int height, int width);
void SubLife(void);
void SetBallPosFtoN(void);
void AirResistance(void);
bool CheckIsBallTouchGround(void);
void DoubleTubeOutPosProcessing(DIR outdir, int height, int width, DIR balldir);
void TubeOutProcessing(int height, int width, DIR balldir);

static BALL g_Ball;
static UINT g_BallTex;
static int g_TubeCnt;
static int g_CoinNum;
static int g_BoundCnt;
static int g_FireCnt;
static int g_FireNum;

static int g_BoundCountTime;
static int g_OldBoundCountTime;
static int g_BallLifeTimeCnt;
static bool g_IsBallMoving;

void BallINIT(void)
{
	g_Ball.IsUse = false;
	g_Ball.npos = MakeInt2(4, 0);
	g_Ball.pos = BALLSTARTPOS;//ボール初期位置
	
	g_Ball.oldpos = MakeFloat2(0, 0);
	g_Ball.size = MakeFloat2(32,32);
	g_Ball.speed = MakeFloat2(0.0f,0.0f);
	g_Ball.type = balltype_normal;
	g_Ball.HP = 5;
	g_Ball.stopCnt = 0;

	g_BoundCnt = 0;
	g_TubeCnt = 0;
	g_CoinNum = 0;
	g_FireCnt = 0;
	g_FireNum = FIRETIME;

	g_OldBoundCountTime = 0;
	g_BoundCountTime = 0;
	g_BallLifeTimeCnt = 0;
	g_IsBallMoving = false;

	g_BallTex = LoadTexture("asset/Ball_2.tga");
}

void BallUPDATE(void)
{
	//物理演算
	if (g_Ball.IsUse)
	{
		g_IsBallMoving = true;

		//ボールの生存時間をカウントアップ。
		g_BallLifeTimeCnt++;

		if (g_Ball.type == balltype_fire)
		{
			g_FireCnt++;
		}
		if (g_FireCnt >= FIRETIME)
		{
			g_Ball.type = balltype_normal;
			g_FireCnt = 0;
		}

		//oldpos保存
		g_Ball.oldpos = g_Ball.pos;

		//転がっている状態なら重力を足さないようにする。
		float addnum = CheckIsBallTouchGround() ? 0 : GRAVITY;

		//位置更新
		g_Ball.speed.y += addnum;
		g_Ball.pos.y += g_Ball.speed.y;
		g_Ball.pos.x += g_Ball.speed.x;
		SetBallPosFtoN();


		bool IsTouchGround = false;
		bool IsBreak = false;
		BLOCK Block;
		int Cnt = 0;
		for (int i = 0; i < MAX_BLOCK_HEIGHT; i++)
		{
			for (int k = 0; k < MAX_BLOCK_WIDTH; k++)
			{
				Block = GetBlock(i, k);
				if (!Block.isUse)continue;

				//======================================================================================ボール下と壁上
				if ((g_Ball.pos.x    - g_Ball.size.x / 2 <= Block.fpos.x + BLOCKSIZE.x / 2) &&
					(g_Ball.pos.x    + g_Ball.size.x / 2 >= Block.fpos.x - BLOCKSIZE.x / 2) &&
					(g_Ball.pos.y    + g_Ball.size.y / 2 >= Block.fpos.y - BLOCKSIZE.y / 2) &&
					(g_Ball.oldpos.y + g_Ball.size.y / 2 <  Block.fpos.y - BLOCKSIZE.y / 2) &&
					(g_Ball.IsUse))
				{
					SetBoundEffects(i, k, dir_under);

					//数回バウンドでボール削除?

					if (!Block.IsBurn && !(g_Ball.type == balltype_water && GetBlock(i,k).type == type_frame))g_BoundCnt++;

					if (!(g_Ball.type == balltype_water && GetBlock(i, k).type == type_frame))IsTouchGround = true;

					IsBreak = true;
				}
				//======================================================================================ボール右と壁左
				if ((g_Ball.pos.x    + g_Ball.size.x / 2 >= Block.fpos.x - BLOCKSIZE.x / 2) &&
					(g_Ball.oldpos.x + g_Ball.size.x / 2 <  Block.fpos.x - BLOCKSIZE.x / 2) &&
					(g_Ball.pos.y    + g_Ball.size.y / 2 >= Block.fpos.y - BLOCKSIZE.y / 2) &&
					(g_Ball.pos.y    - g_Ball.size.y / 2 <= Block.fpos.y + BLOCKSIZE.y / 2) &&
					(g_Ball.IsUse))
				{
					SetBoundEffects(i, k, dir_right);

					AirResistance();

					IsBreak = true;
				}
				//======================================================================================ボール左と壁右
				if ((g_Ball.pos.x    - g_Ball.size.x / 2 <= Block.fpos.x + BLOCKSIZE.x / 2) &&
					(g_Ball.oldpos.x - g_Ball.size.x / 2 >  Block.fpos.x + BLOCKSIZE.x / 2) &&
					(g_Ball.pos.y    + g_Ball.size.y / 2 >= Block.fpos.y - BLOCKSIZE.y / 2) &&
					(g_Ball.pos.y    - g_Ball.size.y / 2 <= Block.fpos.y + BLOCKSIZE.y / 2) &&
					(g_Ball.IsUse))
				{
					SetBoundEffects(i, k, dir_left);

					AirResistance();

					IsBreak = true;
				}
				//======================================================================================ボール上と壁下
				if ((g_Ball.pos.x    + g_Ball.size.x / 2 >= Block.fpos.x - BLOCKSIZE.x / 2) &&
					(g_Ball.pos.x    - g_Ball.size.x / 2 <= Block.fpos.x + BLOCKSIZE.x / 2) &&
					(g_Ball.pos.y    - g_Ball.size.y / 2 <= Block.fpos.y + BLOCKSIZE.y / 2) &&
					(g_Ball.oldpos.y - g_Ball.size.y / 2 >  Block.fpos.y + BLOCKSIZE.y / 2) &&
					(g_Ball.IsUse))
				{
					SetBoundEffects(i, k, dir_top);

					IsBreak = true;
				}
				if (IsBreak)break;
			}
			if (IsBreak)break;
		}

		//バウンド回数でHPマイナス。
		if (g_BoundCnt >= 128)
		{
			BallReset();
			SubLife();
			StageBlockReset();
		}

		//=========================================================画面外に出たら削除　最終的にはこの処理は無くす。
		if (((g_Ball.pos.x <= -SCREEN_WIDTH  / 2 - g_Ball.size.x) ||
			(g_Ball.pos.x  >=  SCREEN_WIDTH  / 2 + g_Ball.size.x) ||
			(g_Ball.pos.y  <= -SCREEN_HEIGHT / 2 - g_Ball.size.y) ||
			(g_Ball.pos.y  >=  SCREEN_HEIGHT / 2 + g_Ball.size.y)) &&
			(g_Ball.IsUse))
		{
			//ポジション設定
			BallReset();
			StageBlockReset();
		}
	}
}

bool CheckIsBallTouchGround(void)
{
	bool isbreak = false;

	for (int i = 0; i < MAX_BLOCK_HEIGHT; i++)
	{
		for (int k = 0; k < MAX_BLOCK_WIDTH; k++)
		{
			BLOCK Block = GetBlock(i, k);

			if (!Block.isUse)continue;

			if ((g_Ball.pos.x - g_Ball.size.x / 2 < Block.fpos.x + BLOCKSIZE.x / 2) &&
				(g_Ball.pos.x + g_Ball.size.x / 2 > Block.fpos.x - BLOCKSIZE.x / 2) &&
				(g_Ball.pos.y + g_Ball.size.y / 2 >= Block.fpos.y - BLOCKSIZE.y / 2 - BALLADJUST_Y) &&
				(g_Ball.oldpos.y + g_Ball.size.y / 2 < Block.fpos.y - BLOCKSIZE.y / 2) &&
				(g_Ball.IsUse))
			{
				//めり込み補正

				if (Block.type != type_tube_in && Block.type != type_tube_out && 
					!(Block.type == type_frame && g_Ball.type == balltype_water))
				{
					g_Ball.pos.y = Block.fpos.y - BLOCKSIZE.y / 2 - g_Ball.size.y / 2 - BALLADJUST_Y;
					if (!Block.IsBurn)
					{
						g_BoundCnt++;
					}
				}
				AirResistance();

				isbreak = true;
				break;
			}
		}
		if (isbreak)break;
	}

	if (isbreak)return true;
	else return false;
}

void AirResistance(void)
{
	//横に無限に転がらないように摩擦する
	if (g_Ball.speed.x != 0.0f)
	{
		if (g_Ball.speed.x < 0)
		{
			g_Ball.speed.x += 0.01f;
		}
		else if (g_Ball.speed.x > 0)
		{
			g_Ball.speed.x -= 0.01f;
		}
	}
}

void SetTubeOutPos(int height,int width)
{
	//TUBEに入ったときに出現位置。
	//出てきたTUBEに入ったら一個前のTUBEから出てくるようにする。

	//ボールポジションを移動先のTUBEのポジションに合わせる。
	Int2 alfa = GetTubeEscapePos(GetNextTubeNum(height, width));
	g_Ball.npos = alfa;
	SetBallPosNtoF();

	//バウンドリセット
	g_BoundCnt = 0;

	//BLOCK outtube = GetBlock(alfa.y, alfa.x);
	BLOCK outtube = GetNextTubeBlock(GetNextTubeNum(height, width));
	BLOCK intube = GetBlock(height, width);

	//ボールの勢いの方向を土管に合わせて変更
	if (outtube.dir == dir_left)
	{
		//入った時の方向でXを無効にするかYを無効にするか決める
		if (g_Ball.speed.y < 0)g_Ball.speed.y *= -1;
		if (g_Ball.speed.x < 0)g_Ball.speed.x *= -1;

		g_Ball.speed.x = g_Ball.speed.y > g_Ball.speed.x ? g_Ball.speed.y : g_Ball.speed.x;

		if (g_Ball.speed.x > 0)
		{
			g_Ball.speed.x *= -1;
		}
		g_Ball.speed.y = 0.0f;//横からでるからYは0にする。
	}
	else if (outtube.dir == dir_right)
	{
		if (g_Ball.speed.y < 0)g_Ball.speed.y *= -1;
		if (g_Ball.speed.x < 0)g_Ball.speed.x *= -1;

		g_Ball.speed.x = g_Ball.speed.y > g_Ball.speed.x ? g_Ball.speed.y : g_Ball.speed.x;

		if (g_Ball.speed.x < 0)
		{
			g_Ball.speed.x *= -1;
		}
		g_Ball.speed.y = 0.0f;
	}
	else if (outtube.dir == dir_top)
	{
		if (g_Ball.speed.y < 0)g_Ball.speed.y *= -1;
		if (g_Ball.speed.x < 0)g_Ball.speed.x *= -1;

		g_Ball.speed.y = g_Ball.speed.x > g_Ball.speed.y ? g_Ball.speed.x : g_Ball.speed.y;

		if (g_Ball.speed.y > 0)
		{
			g_Ball.speed.y *= -1;
		}
		g_Ball.speed.x = 0.0f;
	}
	else if(outtube.dir == dir_under)
	{
		//上から下に出る場合は加速しすぎないようにYを0にする。
		g_Ball.speed.y = 0.0f;
		g_Ball.speed.x = 0.0f;
	}
}

void SetBoundEffects(int height,int width,DIR balldir)
{
	float currentspeed = 0.0f;

	//一つの変数にボールの方向を入れて、当たったブロックに合わせて編集したあと最後にg_ballに入れ直す。
	if (balldir == dir_top || balldir == dir_under) currentspeed = g_Ball.speed.y;
	else currentspeed = g_Ball.speed.x;

	BLOCK Block = GetBlock(height, width);
	bool IsIn = false;
	bool IsEnableAttach = false;
	bool IsEnableAttachforDouble = false;
	DIR OutDoubleDir = dir_top;
	bool isfixX = false;
	bool isfixY = false;

	switch (Block.type)
	{
	case type_normal:
	case type_move:
	case type_dirt:

		if(Block.type != type_dirt)PlaySE(SE_WATER_2);

		currentspeed *= -1;
		currentspeed = currentspeed > 0 ? currentspeed - RESISTANCE : currentspeed + RESISTANCE;
		if (Block.type == type_dirt)
		{
			DestroyBlock(height, width);
			PlaySE(SE_STONE);
		}


		break;
	case type_grass:

		PlaySE(SE_GRASS);

		if (g_Ball.type == balltype_fire)
		{
			//燃えるエフェクト
			currentspeed = currentspeed > 0 ? currentspeed - 0.5f : currentspeed + 0.5f;
			if(!Block.IsBurn) SetFire(Block.fpos);
			SetBurn(height, width);
		}

		currentspeed *= -1;
		currentspeed = currentspeed > 0 ? currentspeed - 1.5f : currentspeed + 1.5f;

		break;
	case type_frame:

		if (g_Ball.type != balltype_water)
		{
			currentspeed *= -1;
			currentspeed = currentspeed > 0 ? currentspeed - RESISTANCE : currentspeed + RESISTANCE;
		}
		else
		{
			//貫通
			IsEnableAttach = true;
		}

		break;
	case type_doubletube:
		//入った方向とブロックの方向を考えて、入った方向から90度ずらして出す

		PlaySE(SE_SWING);

		switch (Block.dir)
		{
		case dir_top:
			if (balldir == dir_under)
			{
				OutDoubleDir = dir_right;
				IsEnableAttachforDouble = true;
			}
			else if (balldir == dir_left)
			{
				OutDoubleDir = dir_top;
				IsEnableAttachforDouble = true;
			}
			break;
		case dir_under:
			if (balldir == dir_top)
			{
				OutDoubleDir = dir_left;
				IsEnableAttachforDouble = true;
			}
			else if(balldir == dir_right)
			{
				OutDoubleDir = dir_under;
				IsEnableAttachforDouble = true;
			}
			break;
		case dir_right:
			if (balldir == dir_left)
			{
				OutDoubleDir = dir_under;
				IsEnableAttachforDouble = true;
			}
			else if (balldir == dir_top)
			{
				OutDoubleDir = dir_right;
				IsEnableAttachforDouble = true;
			}
			break;
		case dir_left:
			if (balldir == dir_right)
			{
				OutDoubleDir = dir_top;
				IsEnableAttachforDouble = true;
			}
			else if (balldir == dir_under)
			{
				OutDoubleDir = dir_left;
				IsEnableAttachforDouble = true;
			}
			break;
		}

		//DOUBLETUBEの処理　入ったか入ってないか。
		if (IsEnableAttachforDouble)
		{
			DoubleTubeOutPosProcessing(OutDoubleDir, height,width,balldir);
		}
		//入れなかったからバウンド処理
		else
		{
			if (balldir == dir_top || balldir == dir_under)
			{
				g_Ball.speed.y *= -1;
				g_Ball.speed.y = g_Ball.speed.y > 0 ? g_Ball.speed.y - RESISTANCE : g_Ball.speed.y + RESISTANCE;
			}
			else
			{
				g_Ball.speed.x *= -1;
				g_Ball.speed.x = g_Ball.speed.x > 0 ? g_Ball.speed.x - RESISTANCE : g_Ball.speed.x + RESISTANCE;
			}
		}
		IsEnableAttach = true;

		break;
	case type_water:

		PlaySE(SE_WATER);

		g_Ball.type = balltype_water;
		DestroyBlock(height, width);

		break;
	case type_tube_in:
	case type_tube_out:

		PlaySE(SE_SWING);
		//出るTUBEの方向を取得する

		TubeOutProcessing(height, width, balldir);
		IsEnableAttach = true;

		break;
	case type_needle:

		SetSyabonBreak(g_Ball.pos);

		BallReset();//ボール削除
		SubLife();

		PlaySE(SE_POP);

		//ステージ初期化
		StageBlockReset();

		break;
	case type_bottun:

		PlaySE(SE_BOTTUN);

		PlaySE(SE_EXPLOSION);

		//壁を爆破
		BombBlockExplotion();
		DestroyBlock(height,width);

		break;
	case type_fire:

		PlaySE(SE_FIRE);

		g_Ball.type = balltype_fire;
		DestroyBlock(height, width);
		g_FireCnt = 0;

		break;
	case type_coin_1:
	case type_coin_2:

		PlaySE(SE_COIN);

		g_CoinNum++;
		DestroyBlock(height, width);

		break;
	case type_fast:

		isfixX = false;
		isfixY = false;
		if (g_Ball.speed.x < 0)
		{
			g_Ball.speed.x *= -1;
			isfixX = true;
		}
		if (g_Ball.speed.x < 0)
		{
			g_Ball.speed.y *= -1;
			isfixY = true;
		}
		if (g_Ball.speed.x > g_Ball.speed.y)
		{
			g_Ball.speed.x += 7.5f;
		}
		else
		{
			g_Ball.speed.y += 7.5f;
		}
		if (isfixX)g_Ball.speed.x *= -1;
		if (isfixY)g_Ball.speed.y *= -1;

		DestroyBlock(height, width);
		IsEnableAttach = true;

		break;
	case type_goal_1:
	case type_goal_2:
	case type_goal_3:

		PlaySE(SE_CLEAR);

		//クリアしたらボールをリセットしておく。
		SetClear();
		g_Ball.IsUse = false;
		SetSyabonBreak(Block.fpos);
		DestroyBlock(height, width);
		SetPrincess(Block.fpos);

		break;
	}

	//一時的な変数から実際の数値に入れ直す処理。
	if (!IsEnableAttach)
	{
		if (balldir == dir_top || balldir == dir_under)g_Ball.speed.y = currentspeed;
		else g_Ball.speed.x = currentspeed;

		if (1)
		{
			//めり込み補正
			switch (balldir)
			{
			case dir_top:
				g_Ball.pos.y = Block.fpos.y + BLOCKSIZE.y / 2 + g_Ball.size.y / 2 + BALLADJUST_Y;
				break;
			case dir_under:
				g_Ball.pos.y = Block.fpos.y - BLOCKSIZE.y / 2 - g_Ball.size.y / 2 - BALLADJUST_Y;
				break;
			case dir_right:
				g_Ball.pos.x = Block.fpos.x - BLOCKSIZE.x / 2 - g_Ball.size.x / 2 - BALLADJUST_Y;
				break;
			case dir_left:
				g_Ball.pos.x = Block.fpos.x + BLOCKSIZE.x / 2 + g_Ball.size.x / 2 + BALLADJUST_Y;
				break;
			}
		}
	}
}

void TubeOutProcessing(int height,int width,DIR balldir)
{
	BLOCK Block = GetBlock(height, width);
	bool IsIn = false;

	switch (Block.dir)
	{
	case dir_top:
		if (balldir == dir_under || balldir == dir_right)IsIn = true;
		break;
	case dir_under:
		if (balldir == dir_top || balldir == dir_left)IsIn = true;
		break;
	case dir_left:
		if (balldir == dir_right || balldir == dir_top)IsIn = true;
		break;
	case dir_right:
		if (balldir == dir_left || balldir == dir_under)IsIn = true;
		break;
	}
	if (IsIn)
	{
		//tube_outに出る
		SetTubeOutPos(height, width);
		g_BoundCnt = 0;
	}
	else
	{
		//入らなかったからバウンド
		float currentspeed = 0.0f;

		switch(Block.dir)
		{
		case dir_top:
			if (balldir == dir_right)currentspeed = g_Ball.speed.x;
			else if (balldir == dir_top)currentspeed = g_Ball.speed.y;
			break;
		case dir_under:
			if (balldir == dir_left)currentspeed = g_Ball.speed.x;
			else if (balldir == dir_under)currentspeed = g_Ball.speed.y;
			break;
		case dir_right:
			if (balldir == dir_right)currentspeed = g_Ball.speed.x;
			else if (balldir == dir_under)currentspeed = g_Ball.speed.y;
			break;
		case dir_left:
			if (balldir == dir_left)currentspeed = g_Ball.speed.x;
			else if (balldir == dir_top)currentspeed = g_Ball.speed.y;
			break;
		}
		currentspeed *= -1;
		currentspeed = currentspeed > 0 ? currentspeed - RESISTANCE : currentspeed + RESISTANCE;

		if (balldir == dir_top || balldir == dir_under)g_Ball.speed.y = currentspeed;
		else g_Ball.speed.x = currentspeed;
	}
}

void DoubleTubeOutPosProcessing(DIR outdir,int height,int width,DIR balldir)
{
	int outposnumX = 0;
	int outposnumY = 0;
	bool isthrough = true;

	if (outdir == dir_top)outposnumY = -1;
	else if (outdir == dir_under)outposnumY = 1;
	else if (outdir == dir_right)outposnumX = 1;
	else outposnumX = -1;

	//入ったTUBEブロックを取得
	BLOCK tubeblock = GetBlock(height, width);

	//出口にあるブロックを取得。
	BLOCK outposblock = GetBlock(height + outposnumY, width + outposnumX);
	if (outposblock.isUse)
	{
		if(outposblock.type == type_normal	||
			outposblock.type == type_dirt	||
			outposblock.type == type_grass  || 
			outposblock.type == type_move   || 
			outposblock.type == type_frame ||
			outposblock.type == type_doubletube ||
			outposblock.type == type_tube_in ||
			outposblock.type == type_tube_out ||
			outposblock.type == type_needle)
		{
			if (outposblock.type == type_needle)
			{
				SetSyabonBreak(g_Ball.pos);

				BallReset();//ボール削除
				SubLife();

				StageBlockReset();
			}

			if (outposblock.type == type_dirt)
			{
				DestroyBlock(height + outposnumY, width + outposnumX);
			}

			if (outposblock.type == type_grass &&
				g_Ball.type == balltype_fire)
			{
				SetBurn(outposblock.npos.y, outposblock.npos.x);
			}

			if (outposblock.type == type_frame &&
				g_Ball.type == balltype_water)
			{
				//貫通するから処理せずにリターン
				if (outposblock.dir == dir_top || outposblock.dir == dir_under)
				{
					if (outdir == dir_top || outdir == dir_under)return;
				}
				else
				{
					if (outdir == dir_right || outdir == dir_left)return;
				}
			}

			bool IsInTube = false;
			//出た先がtubeだったら
			if (outposblock.type == type_tube_in ||
				outposblock.type == type_tube_out)
			{
				switch (tubeblock.dir)
				{
				case dir_top://入り口のDIR 
					if (outposblock.dir == dir_left)IsInTube = true;//チューブ処理
					break;
				case dir_under:
					if (outposblock.dir == dir_right)IsInTube = true;
					break;
				case dir_right:
					if (outposblock.dir == dir_top)IsInTube = true;
					break;
				case dir_left:
					if (outposblock.dir == dir_under)IsInTube = true;
					break;
				}

				if (IsInTube)
				{
					TubeOutProcessing(height + outposnumY, width + outposnumX, tubeblock.dir);
					return;
				}
			}

			//出た先がdoubletubeだったら
			if (outposblock.type == type_doubletube)
			{
				switch (tubeblock.dir)
				{
				case dir_top:  
					if (outposblock.dir == dir_right || outposblock.dir == dir_under)//チューブ処理
					break;
				case dir_under:
					break;
				case dir_right:
					break;
				case dir_left:
					break;
				}
			}

			//入ってきた方向に出ていく
			switch (tubeblock.dir)
			{
			case dir_top:

				g_Ball.speed.y *= -1;
				g_Ball.speed.x = 0.0f;
				outposnumX = 0;
				outposnumY = -1;

				break;
			case dir_under:

				g_Ball.speed.y *= -1;
				g_Ball.speed.x = 0.0f;
				outposnumX = 0;
				outposnumY = 1;

				break;
			case dir_right:

				g_Ball.speed.x *= -1;
				g_Ball.speed.y = 0.0f;
				outposnumX = 1;
				outposnumY = 0;

				break;
			case dir_left:

				g_Ball.speed.x *= -1;
				g_Ball.speed.y = 0.0f;
				outposnumX = -1;
				outposnumY = 0;

				break;
			}
			isthrough = false;
		}
		else//アイテム類
		{
			switch (outposblock.type)
			{
			case type_water:
				g_Ball.type = balltype_water;
				break;
			case type_bottun:
				BombBlockExplotion();
				break;
			case type_fire:
				g_Ball.type = balltype_fire;
				break;
			case type_coin_1:
			case type_coin_2:

				PlaySE(SE_COIN);

				g_CoinNum++;

				DestroyBlock(height + outposnumY, width + outposnumX);
				break;
			case type_fast:

				if (GetRandomNum(0, 2) == 0)
				{
					if (g_Ball.speed.y > 0)
					{
						g_Ball.speed.y += 5.0f;
					}
					else
					{
						g_Ball.speed.y -= 5.0f;
					}
				}
				else
				{
					if (g_Ball.speed.x > 0)
					{
						g_Ball.speed.x += 5.0f;
					}
					else
					{
						g_Ball.speed.x -= 5.0f;
					}
				}
				break;
			case type_goal_1:
			case type_goal_2:
			case type_goal_3:

				PlaySE(SE_CLEAR);

				SetClear();
				g_Ball.IsUse = false;
				SetSyabonBreak(outposblock.fpos);
				SetPrincess(outposblock.fpos);
				DestroyBlock(height + outposnumY, width + outposnumX);

				break;
			}
			DestroyBlock(height + outposnumY, width + outposnumX);
		}
	}
	
	if(isthrough)
	{
		if (outdir == dir_top)
		{
			g_Ball.speed.y = g_Ball.speed.x > 0 ? g_Ball.speed.x * -1 : g_Ball.speed.x;
			g_Ball.speed.y + 0.1f;
			g_Ball.speed.x = 0.0f;
			outposnumX = 0;
			outposnumY = -1;
		}
		else if (outdir == dir_under)
		{
			g_Ball.speed.y = g_Ball.speed.x > 0 ? g_Ball.speed.x : g_Ball.speed.x * -1;
			g_Ball.speed.y - 0.1f;
			g_Ball.speed.x = 0.0f;
			outposnumX = 0;
			outposnumY = 1;
		}
		else if (outdir == dir_right)
		{
			g_Ball.speed.x = g_Ball.speed.y > 0 ? g_Ball.speed.y : g_Ball.speed.y * -1;
			g_Ball.speed.x - 0.1f;
			g_Ball.speed.y = 0.0f;
			outposnumX = 1;
			outposnumY = 0;
		}
		else//left
		{
			g_Ball.speed.x = g_Ball.speed.y > 0 ? g_Ball.speed.y * -1 : g_Ball.speed.y;
			g_Ball.speed.x + 0.1f;//少し遅くする。
			g_Ball.speed.y = 0.0f;
			outposnumX = -1;
			outposnumY = 0;
		}
	}
	g_Ball.npos.x = tubeblock.npos.x + outposnumX;
	g_Ball.npos.y = tubeblock.npos.y + outposnumY;

	SetBallPosNtoF();
}

int GetNextTubeNum(int height, int width)
{
	int nexttubenum = GetBlock(height, width).warp_turn_num;

	 nexttubenum = nexttubenum % 2 != 1 ? nexttubenum + 1 : nexttubenum - 1;

	return nexttubenum;
}

void BallDRAW(void)
{
	//ボール表示
	if (g_Ball.IsUse)
	{
		FaceGenforTex(g_Ball.pos, g_Ball.size, g_Ball.type, 0, 3, 1, true, g_BallTex, MakeFloat4(1, 1, 1, 1));
	
		//ボールが火なら火でいられる時間をゲージで表示
		if (g_Ball.type == balltype_fire)
		{
			//ゲージ背景
			FaceGenforTex(MakeFloat2(0, -SCREEN_HEIGHT / 2 + 32),
				MakeFloat2(FIRETIME + 16, 32 + 16), 0, 0, 1, 1, false, 0, COLOR_BLACK);

			//ゲージ本体
			GageGeneratorSubStyle(MakeFloat2(0, -SCREEN_HEIGHT / 2 + 32),
				32, FIRETIME, g_FireCnt, 'R', MakeFloat4(1, 0.4f, 0, 1));
		}
	}

	//HP表示
	char HPtext[32] = {};
	sprintf(HPtext, "HP:%d", g_Ball.HP);
	TextGen(MakeFloat2(SCREEN_WIDTH / 2 - 32 * 6 - BLOCKSIZE.x, -SCREEN_HEIGHT / 2 + 32 + BLOCKSIZE.y),
		MakeFloat2(32, 32), NORMALCOLOR, HPtext);
}

void SetBallPosNtoF(void)
{
	g_Ball.pos.y = -SCREEN_HEIGHT / 2 + BLOCKSIZE.y * g_Ball.npos.y + BLOCKSIZE.y / 2;
	g_Ball.pos.x = -SCREEN_WIDTH  / 2 + BLOCKSIZE.x * g_Ball.npos.x + BLOCKSIZE.x / 2;
}

void SetBallPosFtoN(void)
{
	g_Ball.npos.x = g_Ball.pos.x / BLOCKSIZE.x + (int)(MAX_BLOCK_WIDTH / 2);
	g_Ball.npos.y = g_Ball.pos.y / BLOCKSIZE.y + (int)(MAX_BLOCK_HEIGHT / 2);
}

void BallReset(void)
{
	g_Ball.speed = MakeFloat2(0, 0);
	g_Ball.IsUse = false;
	g_Ball.stopCnt = 0;
	g_Ball.oldpos = MakeFloat2(0, 0);
	g_Ball.pos = BALLSTARTPOS;
	g_Ball.npos = MakeInt2(4, 0);
	g_Ball.type = balltype_normal;
	g_IsBallMoving = false;
	g_BoundCnt = 0;
	g_CoinNum = 0;
	

	g_BallLifeTimeCnt = 0;
	g_BoundCountTime = 0;
	g_OldBoundCountTime = 0;
}

void BallUNINIT(void)
{
	UnloadTexture(g_BallTex);
	g_BallTex = NULL;
}

void SetBall(void)
{
	if (!g_Ball.IsUse)
	{
		g_Ball.speed = MakeFloat2(0, 0);
		g_Ball.IsUse = true;
	}
}

void ChangeBallType(BALLTYPE type)
{
	g_Ball.type = type;
}

void SubLife(void)
{
	g_Ball.HP -= 1;

	//ゲームオーバー
	g_Ball.HP > 0 ? SetHeart(MakeFloat2(0, 0)) : SetGameOver();

}

BALL* GetBall(void)
{
	return &g_Ball;
}

bool GetIsBallMoving(void)
{
	return g_IsBallMoving;
}

int GetCoinNum(void)
{
	return g_CoinNum;
}