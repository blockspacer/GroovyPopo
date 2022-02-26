#ifndef MYTYPE_H_
#define MYTYPE_H_

typedef struct
{
	int x;
	int y;
}Int2;

enum DIR
{
	dir_top,
	dir_right,
	dir_under,
	dir_left,

	DIRMAX
};

int GetRandomNum(int min, int max);

Int2 MakeInt2(int x, int y);

#endif