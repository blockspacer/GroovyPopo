#ifndef FACEGEN_H_
#define FACEGEN_H_

#include "main.h"
#include "Mytype.h"

void FaceGen(Float2 pos, Float2 size, int frame, int MAXFRAMEX, int MAXFRAMEY, bool IsUseTex, unsigned int textureID, char MODE, Float4 Color);

void CercleGen(Float2 pos, float R, Float4 color);

void GageGenerator(Float2 pos, Float2 size, float Gagenum, char LRTU, Float4 Color);

void FaceGenforTex(Float2 pos, Float2 size, int frameX, int frameY, int MAXframeX, int MAXframeY, bool IsUseTex, UINT texid,Float4 color);

void LineGenerator(Float2 StartPos, Float2 EndPos, Float4 Color);

void FaceGenforTex(Float2 pos, Float2 size, int frameX, int frameY, int MAXframeX, int MAXframeY, bool IsUseTex, UINT texid, Float4 color, DIR dir);

void FacegenUNINIT(void);

void FacegenINIT(void);

void TextGen(Float2 pos, Float2 size, Float4 color, const char* text);

void GageGeneratorSubStyle(Float2 pos, float sizeY, float Gagenum, float subnum, char LRTU, Float4 Color);

#endif