
//======================================
//
//�����Ă��Ȃ����Ƃ��擾����HP�}�C�i�X�B
//
//�o���|�W�V�������
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
	g_Ball.pos = BALLSTARTPOS;//�{�[�������ʒu
	
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
	//�������Z
	if (g_Ball.IsUse)
	{
		g_IsBallMoving = true;

		//�{�[���̐������Ԃ��J�E���g�A�b�v�B
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

		//oldpos�ۑ�
		g_Ball.oldpos = g_Ball.pos;

		//�]�����Ă����ԂȂ�d�͂𑫂��Ȃ��悤�ɂ���B
		float addnum = CheckIsBallTouchGround() ? 0 : GRAVITY;

		//�ʒu�X�V
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

				//======================================================================================�{�[�����ƕǏ�
				if ((g_Ball.pos.x    - g_Ball.size.x / 2 <= Block.fpos.x + BLOCKSIZE.x / 2) &&
					(g_Ball.pos.x    + g_Ball.size.x / 2 >= Block.fpos.x - BLOCKSIZE.x / 2) &&
					(g_Ball.pos.y    + g_Ball.size.y / 2 >= Block.fpos.y - BLOCKSIZE.y / 2) &&
					(g_Ball.oldpos.y + g_Ball.size.y / 2 <  Block.fpos.y - BLOCKSIZE.y / 2) &&
					(g_Ball.IsUse))
				{
					SetBoundEffects(i, k, dir_under);

					//����o�E���h�Ń{�[���폜?

					if (!Block.IsBurn && !(g_Ball.type == balltype_water && GetBlock(i,k).type == type_frame))g_BoundCnt++;

					if (!(g_Ball.type == balltype_water && GetBlock(i, k).type == type_frame))IsTouchGround = true;

					IsBreak = true;
				}
				//======================================================================================�{�[���E�ƕǍ�
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
				//======================================================================================�{�[�����ƕǉE
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
				//======================================================================================�{�[����ƕǉ�
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

		//�o�E���h�񐔂�HP�}�C�i�X�B
		if (g_BoundCnt >= 128)
		{
			BallReset();
			SubLife();
			StageBlockReset();
		}

		//=========================================================��ʊO�ɏo����폜�@�ŏI�I�ɂ͂��̏����͖������B
		if (((g_Ball.pos.x <= -SCREEN_WIDTH  / 2 - g_Ball.size.x) ||
			(g_Ball.pos.x  >=  SCREEN_WIDTH  / 2 + g_Ball.size.x) ||
			(g_Ball.pos.y  <= -SCREEN_HEIGHT / 2 - g_Ball.size.y) ||
			(g_Ball.pos.y  >=  SCREEN_HEIGHT / 2 + g_Ball.size.y)) &&
			(g_Ball.IsUse))
		{
			//�|�W�V�����ݒ�
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
				//�߂荞�ݕ␳

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
	//���ɖ����ɓ]����Ȃ��悤�ɖ��C����
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
	//TUBE�ɓ������Ƃ��ɏo���ʒu�B
	//�o�Ă���TUBE�ɓ��������O��TUBE����o�Ă���悤�ɂ���B

	//�{�[���|�W�V�������ړ����TUBE�̃|�W�V�����ɍ��킹��B
	Int2 alfa = GetTubeEscapePos(GetNextTubeNum(height, width));
	g_Ball.npos = alfa;
	SetBallPosNtoF();

	//�o�E���h���Z�b�g
	g_BoundCnt = 0;

	//BLOCK outtube = GetBlock(alfa.y, alfa.x);
	BLOCK outtube = GetNextTubeBlock(GetNextTubeNum(height, width));
	BLOCK intube = GetBlock(height, width);

	//�{�[���̐����̕�����y�ǂɍ��킹�ĕύX
	if (outtube.dir == dir_left)
	{
		//���������̕�����X�𖳌��ɂ��邩Y�𖳌��ɂ��邩���߂�
		if (g_Ball.speed.y < 0)g_Ball.speed.y *= -1;
		if (g_Ball.speed.x < 0)g_Ball.speed.x *= -1;

		g_Ball.speed.x = g_Ball.speed.y > g_Ball.speed.x ? g_Ball.speed.y : g_Ball.speed.x;

		if (g_Ball.speed.x > 0)
		{
			g_Ball.speed.x *= -1;
		}
		g_Ball.speed.y = 0.0f;//������ł邩��Y��0�ɂ���B
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
		//�ォ�牺�ɏo��ꍇ�͉����������Ȃ��悤��Y��0�ɂ���B
		g_Ball.speed.y = 0.0f;
		g_Ball.speed.x = 0.0f;
	}
}

void SetBoundEffects(int height,int width,DIR balldir)
{
	float currentspeed = 0.0f;

	//��̕ϐ��Ƀ{�[���̕��������āA���������u���b�N�ɍ��킹�ĕҏW�������ƍŌ��g_ball�ɓ��꒼���B
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
			//�R����G�t�F�N�g
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
			//�ђ�
			IsEnableAttach = true;
		}

		break;
	case type_doubletube:
		//�����������ƃu���b�N�̕������l���āA��������������90�x���炵�ďo��

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

		//DOUBLETUBE�̏����@�������������ĂȂ����B
		if (IsEnableAttachforDouble)
		{
			DoubleTubeOutPosProcessing(OutDoubleDir, height,width,balldir);
		}
		//����Ȃ���������o�E���h����
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
		//�o��TUBE�̕������擾����

		TubeOutProcessing(height, width, balldir);
		IsEnableAttach = true;

		break;
	case type_needle:

		SetSyabonBreak(g_Ball.pos);

		BallReset();//�{�[���폜
		SubLife();

		PlaySE(SE_POP);

		//�X�e�[�W������
		StageBlockReset();

		break;
	case type_bottun:

		PlaySE(SE_BOTTUN);

		PlaySE(SE_EXPLOSION);

		//�ǂ𔚔j
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

		//�N���A������{�[�������Z�b�g���Ă����B
		SetClear();
		g_Ball.IsUse = false;
		SetSyabonBreak(Block.fpos);
		DestroyBlock(height, width);
		SetPrincess(Block.fpos);

		break;
	}

	//�ꎞ�I�ȕϐ�������ۂ̐��l�ɓ��꒼�������B
	if (!IsEnableAttach)
	{
		if (balldir == dir_top || balldir == dir_under)g_Ball.speed.y = currentspeed;
		else g_Ball.speed.x = currentspeed;

		if (1)
		{
			//�߂荞�ݕ␳
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
		//tube_out�ɏo��
		SetTubeOutPos(height, width);
		g_BoundCnt = 0;
	}
	else
	{
		//����Ȃ���������o�E���h
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

	//������TUBE�u���b�N���擾
	BLOCK tubeblock = GetBlock(height, width);

	//�o���ɂ���u���b�N���擾�B
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

				BallReset();//�{�[���폜
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
				//�ђʂ��邩�珈�������Ƀ��^�[��
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
			//�o���悪tube��������
			if (outposblock.type == type_tube_in ||
				outposblock.type == type_tube_out)
			{
				switch (tubeblock.dir)
				{
				case dir_top://�������DIR 
					if (outposblock.dir == dir_left)IsInTube = true;//�`���[�u����
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

			//�o���悪doubletube��������
			if (outposblock.type == type_doubletube)
			{
				switch (tubeblock.dir)
				{
				case dir_top:  
					if (outposblock.dir == dir_right || outposblock.dir == dir_under)//�`���[�u����
					break;
				case dir_under:
					break;
				case dir_right:
					break;
				case dir_left:
					break;
				}
			}

			//�����Ă��������ɏo�Ă���
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
		else//�A�C�e����
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
			g_Ball.speed.x + 0.1f;//�����x������B
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
	//�{�[���\��
	if (g_Ball.IsUse)
	{
		FaceGenforTex(g_Ball.pos, g_Ball.size, g_Ball.type, 0, 3, 1, true, g_BallTex, MakeFloat4(1, 1, 1, 1));
	
		//�{�[�����΂Ȃ�΂ł����鎞�Ԃ��Q�[�W�ŕ\��
		if (g_Ball.type == balltype_fire)
		{
			//�Q�[�W�w�i
			FaceGenforTex(MakeFloat2(0, -SCREEN_HEIGHT / 2 + 32),
				MakeFloat2(FIRETIME + 16, 32 + 16), 0, 0, 1, 1, false, 0, COLOR_BLACK);

			//�Q�[�W�{��
			GageGeneratorSubStyle(MakeFloat2(0, -SCREEN_HEIGHT / 2 + 32),
				32, FIRETIME, g_FireCnt, 'R', MakeFloat4(1, 0.4f, 0, 1));
		}
	}

	//HP�\��
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

	//�Q�[���I�[�o�[
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