
#include"Mytype.h"
#include<stdlib.h>

Int2 MakeInt2(int x, int y)
{
	Int2 alfa;
	alfa.x = x;
	alfa.y = y;

	return alfa;
}

//–ß‚è’l‚ÍÅ¬‚ÅminAÅ‘å‚Åmax - 1B
int GetRandomNum(int min, int max)
{
	int alfa = 0;
	for (int i = 0; i < 128; i++)
	{
		alfa = rand() % max + min;
	}
	return alfa;
}