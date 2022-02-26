
#include"main.h"
#include"FaceGen.h"
#include"controller.h"

#define MAXMOUSELINE (3)
#define MAXLINE (256)

typedef struct
{
	Float2 pos;
	bool IsUse;
}MOUSELINE;

void SetMouseLine(Float2 pos);
void PaintINIT(void);

static MOUSELINE g_MouseLine[MAXMOUSELINE][MAXLINE] = {};
static bool g_IsTouch_paint;
static bool g_IsFirstMouseLine[MAXMOUSELINE] = {};
static bool g_IsMouseDown;
static int g_LineNum;

void PaintINIT(void)
{
	memset(g_MouseLine, 0, sizeof(g_MouseLine));
	g_IsTouch_paint = false;
	memset(g_IsFirstMouseLine, false, sizeof(g_IsFirstMouseLine));
	g_IsMouseDown = false;
	g_LineNum = 0;
}

void PaintUPDATE(void)
{
	if ((GetKeyState(VK_LCONTROL) & 0x80) && (GetKeyState('Z') & 0x80))
	{
		if (!g_IsTouch_paint)
		{
			memset(g_IsFirstMouseLine, 0, sizeof(g_IsFirstMouseLine));
			memset(g_MouseLine, 0, sizeof(g_MouseLine));
			g_IsMouseDown = false;
			g_LineNum = 0;

			g_IsTouch_paint = true;
		}
	}
	else
	{
		g_IsTouch_paint = false;
	}

	if (g_LineNum < 3)
	{
		if (GetControllerTouchScreen())
		{
			SetMouseLine(GetControllerTouchScreenPosition());
			g_IsMouseDown = true;
		}
		else
		{
			if (g_IsMouseDown)
			{
				g_IsFirstMouseLine[g_LineNum] = true;
				g_LineNum++;
				g_IsMouseDown = false;
			}
		}
	}
}

void PaintDRAW(void)
{
	for (int k = 0; k < MAXMOUSELINE; k++)
	{
		for (int i = 0; i < MAXLINE; i++)
		{
			if (!g_MouseLine[k][i].IsUse)continue;

			if (g_IsFirstMouseLine[g_LineNum])continue;

			int alpha = i <= 0 ? 0 : i - 1;

			Float4 color = k == 0 ? MakeFloat4(1, 1, 1, 1) :
				k == 1 ? MakeFloat4(1, 0, 0.8f, 1) : MakeFloat4(0.8f, 0.8f, 0, 1);

			LineGenerator(g_MouseLine[k][alpha].pos, g_MouseLine[k][i].pos, color);
		}
	}
}

void PaintUNINIT(void)
{
}

void SetMouseLine(Float2 pos)
{

	for (int i = 0; i < MAXLINE; i++)
	{
		if (g_MouseLine[g_LineNum][i].IsUse)continue;

		g_MouseLine[g_LineNum][i].pos = pos;
		g_MouseLine[g_LineNum][i].IsUse = true;

		if (i > 0)g_IsFirstMouseLine[g_LineNum] = false;

		break;
	}
}