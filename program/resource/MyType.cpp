
#include"Mytype.h"
#include<stdlib.h>

Int2 MakeInt2(int x, int y)
{
	Int2 alfa;
	alfa.x = x;
	alfa.y = y;

	return alfa;
}

//�߂�l�͍ŏ���min�A�ő��max - 1�B
int GetRandomNum(int min, int max)
{
	int alfa = 0;
	for (int i = 0; i < 128; i++)
	{
		alfa = rand() % max + min;
	}
	return alfa;
}