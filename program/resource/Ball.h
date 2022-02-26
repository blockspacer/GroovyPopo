#ifndef BALL_H_
#define BALL_H_

#include"main.h"
#include"Mytype.h"

enum BALLTYPE
{
	balltype_normal,
	balltype_water,
	balltype_fire,

	BALLTYPEMAX
};

typedef struct
{
	Float2 pos;
	Float2 oldpos;
	Int2 npos;
	Float2 size;
	Float2 speed;
	bool IsUse;
	BALLTYPE type;
	int HP;
	int stopCnt;
}BALL;

void BallINIT(void);
void BallUPDATE(void);
void BallDRAW(void);
void BallUNINIT(void);

void ChangeBallType(BALLTYPE type);
void BallReset(void);
BALL* GetBall(void);
void SetBall(void);
bool GetIsBallMoving(void);
int GetCoinNum(void);

#endif