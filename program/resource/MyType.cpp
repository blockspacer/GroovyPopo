
#include"Mytype.h"
#include<stdlib.h>

Int2 MakeInt2(int x, int y)
{
	Int2 alfa;
	alfa.x = x;
	alfa.y = y;

	return alfa;
}

//戻り値は最小でmin、最大でmax - 1。
int GetRandomNum(int min, int max)
{
	int alfa = 0;
	for (int i = 0; i < 128; i++)
	{
		alfa = rand() % max + min;
	}
	return alfa;
}