#include"FaceGen.h"
#include"texture.h"

#define MAXDEBUGTEXT (16)
#define MAXDEBUGTEXTLENGTH (64)

// 頂点構造体
struct VERTEX_3D
{
	Float3 Position;
	Float4 Color;
	Float2 Texcord;
};

typedef struct
{
	char debugText[MAXDEBUGTEXTLENGTH];
	int TextNum;
	bool IsUse;
}DEBUGTEXT;

static UINT g_TextTex;

void FacegenINIT(void)
{
	g_TextTex = LoadTexture("asset/text.tga");
}

void FaceGen(Float2 pos,Float2 size,int frame,int MAXFRAMEX,int MAXFRAMEY,bool IsUseTex,unsigned int textureID,char MODE,Float4 Color)
{
	IsUseTex == true ? SetTexture(textureID) : SetTexture(NULL);

	float width = size.x / 2;
	float height = size.y / 2;

	float perframeX = (1.0f / MAXFRAMEX);
	float perframeY = (1.0f / MAXFRAMEY);

	float frameX = 0.0f;
	float frameY = 0.0f;

	if (MAXFRAMEY == 1)
	{
		frameX = frame * perframeX;
		frameY = 0.0f;
		perframeY = 1.0f;
	}
	else if (MAXFRAMEX == 1)
	{
		frameY = frame * perframeY;
		frameX = 0;
		perframeX = 1.0f;
	}
	else
	{
		frameX = (frame % MAXFRAMEX) * perframeX;
		frameY = (frame / MAXFRAMEY) * perframeY;
	}

	VERTEX_3D vertex[4];

	if (MODE == 'B')//バックグラウンド
	{
		vertex[0].Position = MakeFloat3(-SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f);
		vertex[1].Position = MakeFloat3(SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f);
		vertex[2].Position = MakeFloat3(-SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		vertex[3].Position = MakeFloat3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		if (IsUseTex)
		{
			vertex[0].Texcord = MakeFloat2(0,0);
			vertex[1].Texcord = MakeFloat2(1,0);
			vertex[2].Texcord = MakeFloat2(0,1);
			vertex[3].Texcord = MakeFloat2(1,1);
		}
	}
	else
	{
		vertex[0].Position = MakeFloat3(pos.x - width, pos.y - height, 0.0f);
		vertex[1].Position = MakeFloat3(pos.x + width, pos.y - height, 0.0f);
		vertex[2].Position = MakeFloat3(pos.x - width, pos.y + height, 0.0f);
		vertex[3].Position = MakeFloat3(pos.x + width, pos.y + height, 0.0f);
		if (IsUseTex)
		{
			vertex[0].Texcord = MakeFloat2(frameX            , frameY);
			vertex[1].Texcord = MakeFloat2(frameX + perframeX, frameY);
			vertex[2].Texcord = MakeFloat2(frameX            , frameY + perframeY);
			vertex[3].Texcord = MakeFloat2(frameX + perframeX, frameY + perframeY);
		}
	}
	vertex[0].Color = Color;
	vertex[1].Color = Color;
	vertex[2].Color = Color;
	vertex[3].Color = Color;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Position);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Color);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Texcord);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void FaceGenforTex(Float2 pos, Float2 size, int frameX,int frameY, int MAXframeX, int MAXframeY, bool IsUseTex, UINT texid,Float4 color)
{
	VERTEX_3D vertex[4] = {};

	float texcutsizex = (1.0f / MAXframeX);
	float texcutsizey = (1.0f / MAXframeY);
	float texX = texcutsizex * frameX;
	float texY = texcutsizey * frameY;

	float width = size.x / 2;
	float height = size.y / 2;


	vertex[0].Position = MakeFloat3(pos.x - width, pos.y - height, 0.0f);
	vertex[1].Position = MakeFloat3(pos.x + width, pos.y - height, 0.0f);
	vertex[2].Position = MakeFloat3(pos.x - width, pos.y + height, 0.0f);
	vertex[3].Position = MakeFloat3(pos.x + width, pos.y + height, 0.0f);

	if (IsUseTex)
	{
		vertex[0].Texcord = MakeFloat2(texX              , texY);
		vertex[1].Texcord = MakeFloat2(texX + texcutsizex, texY);
		vertex[2].Texcord = MakeFloat2(texX              , texY + texcutsizey);
		vertex[3].Texcord = MakeFloat2(texX + texcutsizex, texY + texcutsizey);
	}

	vertex[0].Color = color;
	vertex[1].Color = color;
	vertex[2].Color = color;
	vertex[3].Color = color;

	IsUseTex == true ? SetTexture(texid) : SetTexture(NULL);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Position);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Color);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Texcord);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void FaceGenforTex(Float2 pos, Float2 size, int frameX, int frameY, int MAXframeX, int MAXframeY, bool IsUseTex, UINT texid, Float4 color,DIR dir)
{
	VERTEX_3D vertex[4] = {};

	float texcutsizex = (1.0f / MAXframeX);
	float texcutsizey = (1.0f / MAXframeY);
	float texX = texcutsizex * frameX;
	float texY = texcutsizey * frameY;

	float width = size.x / 2;
	float height = size.y / 2;


	vertex[0].Position = MakeFloat3(pos.x - width, pos.y - height, 0.0f);
	vertex[1].Position = MakeFloat3(pos.x + width, pos.y - height, 0.0f);
	vertex[2].Position = MakeFloat3(pos.x - width, pos.y + height, 0.0f);
	vertex[3].Position = MakeFloat3(pos.x + width, pos.y + height, 0.0f);

	int tl = 0;
	int tr = 1;
	int dl = 2;
	int dr = 3;

	if (IsUseTex)
	{
		if (dir == dir_left)
		{
			tl = 2;
			tr = 0;
			dl = 3;
			dr = 1;
		}
		else if (dir == dir_right)
		{
			tl = 1;
			tr = 3;
			dl = 0;
			dr = 2;
		}
		else if(dir == dir_under)
		{
			tl = 3;
			tr = 2;
			dl = 1;
			dr = 0;
		}

		vertex[tl].Texcord = MakeFloat2(texX, texY);
		vertex[tr].Texcord = MakeFloat2(texX + texcutsizex, texY);
		vertex[dl].Texcord = MakeFloat2(texX, texY + texcutsizey);
		vertex[dr].Texcord = MakeFloat2(texX + texcutsizex, texY + texcutsizey);
	}

	vertex[0].Color = color;
	vertex[1].Color = color;
	vertex[2].Color = color;
	vertex[3].Color = color;

	IsUseTex == true ? SetTexture(texid) : SetTexture(NULL);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Position);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Color);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Texcord);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

//LRTU = left right top under
void GageGenerator(Float2 pos, Float2 size, float Gagenum, char LRTU, Float4 Color)
{
	VERTEX_3D vertex[4];
	SetTexture(NULL);

	float width = size.x / 2;
	float height = size.y / 2;

	float top   = -height;
	float under = -height;
	float right = -width;
	float left  = -width;

	if (LRTU == 'L')//左に増やす
	{
		left  += Gagenum;
		right = width;
		top   = height;
		under = height;
	}
	else if (LRTU == 'R')//右に増やす
	{
		left  = width;
		right += Gagenum;
		top   = height;
		under = height;
	}
	else if (LRTU == 'T')//上に増やす
	{
		left  = width;
		right = width;
		top   += Gagenum;
		under = height;
	}
	else//下に増やす
	{
		left  = width;
		right = width;
		top   = height;
		under += Gagenum;
	}

	vertex[0].Position = MakeFloat3(pos.x - left , pos.y - top  , 0.0f);
	vertex[1].Position = MakeFloat3(pos.x + right, pos.y - top  , 0.0f);
	vertex[2].Position = MakeFloat3(pos.x - left , pos.y + under, 0.0f);
	vertex[3].Position = MakeFloat3(pos.x + right, pos.y + under, 0.0f);

	vertex[0].Color = Color;
	vertex[1].Color = Color;
	vertex[2].Color = Color;
	vertex[3].Color = Color;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Position);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Color);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Texcord);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void GageGeneratorSubStyle(Float2 pos, float sizeY, float Gagenum,float subnum, char LRTU, Float4 Color)
{
	VERTEX_3D vertex[4];
	SetTexture(NULL);

	float height = sizeY / 2;
	float width = Gagenum / 2;

	float top = -height;
	float under = -height;
	float right = -width;
	float left = -width;

	if (LRTU == 'L')//右に減らす
	{
		left = (left * -1) - subnum;
		right = width;
		top = height;
		under = height;
	}
	else if (LRTU == 'R')//左に減らす
	{
		left = width;
		right = (right * - 1) - subnum;
		top = height;
		under = height;
	}
	else if (LRTU == 'T')//上に減らす
	{
		left = width;
		right = width;
		top = (top * -1) - subnum;
		under = height;
	}
	else//下に減らす
	{
		left = width;
		right = width;
		top = height;
		under = (under * -1) - subnum;
	}

	vertex[0].Position = MakeFloat3(pos.x - left, pos.y - top, 0.0f);
	vertex[1].Position = MakeFloat3(pos.x + right, pos.y - top, 0.0f);
	vertex[2].Position = MakeFloat3(pos.x - left, pos.y + under, 0.0f);
	vertex[3].Position = MakeFloat3(pos.x + right, pos.y + under, 0.0f);

	vertex[0].Color = Color;
	vertex[1].Color = Color;
	vertex[2].Color = Color;
	vertex[3].Color = Color;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Position);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Color);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Texcord);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void LineGenerator(Float2 StartPos, Float2 EndPos,Float4 Color)
{
	VERTEX_3D vertex[2];
	SetTexture(NULL);

	vertex[0].Position = MakeFloat3(StartPos.x, StartPos.y, 0.0f);
	vertex[1].Position = MakeFloat3(EndPos.x, EndPos.y, 0.0f);

	vertex[0].Color = Color;
	vertex[1].Color = Color;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Position);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Color);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Texcord);

	glDrawArrays(GL_LINE_STRIP, 0, 2);
}

void CercleGen(Float2 pos, float R, Float4 color)
{
	static const float PI = 3.14f;
	static const int N = 64;
	SetTexture(NULL);

	VERTEX_3D cube[N + 1] = {};

	for (int i = 0; i < N; i++)
	{
		int n = i % N;
		float rad = 2 * PI / N * n;
		float px = pos.x + cosf(rad) * R;
		float py = pos.y + sinf(rad) * R;

		cube[i].Position = MakeFloat3(px, py, 0.0f);
		cube[i].Color = color;
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&cube->Position);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&cube->Color);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&cube->Texcord);

	glDrawArrays(GL_TRIANGLE_FAN, 0, N);
}

void TextGen(Float2 pos, Float2 size, Float4 color, const char* text)
{
	float startposX = (size.x * strlen(text)) / 2 - size.x / 2;

	for (int i = 0; *text ; text++,i++)
	{
		Float2 nextpos = MakeFloat2(pos.x + size.x * i - startposX, pos.y);

		if (*text >= '0' && *text <= '9')
		{
			FaceGenforTex(nextpos, size, *text - '0', 0, 10, 4, true, g_TextTex, color);
		}
		else if((*text >= 'A' && *text <= 'Z') || (*text >= 'a' && *text <= 'z'))
		{
			char c = *text <= 'Z' ? *text - 'A' : *text - 'a';
			FaceGenforTex(nextpos, size, c % 10, (c / 10) + 1, 10, 4, true, g_TextTex, color);
		}
		else
		{
			switch (*text)
			{
			case '!':
				FaceGenforTex(nextpos, size,
					6, 3, 10, 4, true, g_TextTex, color);
				break;
			case '/':
				FaceGenforTex(nextpos, size,
					7, 3, 10, 4, true, g_TextTex, color);
				break;
			case '-':
				FaceGenforTex(nextpos, size,
					8, 3, 10, 4, true, g_TextTex, color);
				break;
			case ';':
				FaceGenforTex(nextpos, size,
					9, 3, 10, 4, true, g_TextTex, color);
				break;
			}
		}
	}
}

void FacegenUNINIT(void)
{
	UnloadTexture(g_TextTex);
	g_TextTex = NULL;
}