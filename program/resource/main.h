#ifndef MAIN_H_
#define MAIN_H_

#include"system.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define NORMALCOLOR (MakeFloat4(1,1,1,1))
#define COLOR_BLACK (MakeFloat4(0,0,0,1))

bool GetIsDebug(void);
void UNINIT(void);
bool GetIsDispMenu(void);

#endif // !MAIN_H_