
#include "mytime.h"

// マクロ
#define FRAME_RATE (60)

// 時間計測用
DWORD dwExecLastTime;
DWORD dwFPSLastTime;
DWORD dwCurrentTime;
DWORD dwFrameCount;
float fCountFPS;

void InitTime(void)
{
	// フレームカウント初期化
	timeBeginPeriod(1);					// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;
}



void GetTime(void)
{
	dwCurrentTime = timeGetTime();		// システム時刻を取得
}


void CheckTime(void)
{
	while (1)
	{
		//FPSの計測
		if (dwCurrentTime - dwFPSLastTime >= 500) // 500m/s毎にチェックしてる
		{
			fCountFPS = dwFrameCount * 1000.0f / (dwCurrentTime - dwFPSLastTime);
			dwFPSLastTime = dwCurrentTime;
			dwFrameCount = 0;
		}

		if ((dwCurrentTime - dwExecLastTime) >= (1000 / FRAME_RATE))	// 1/60秒ごとに実行
		{
			dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存
			dwFrameCount++;		// 処理回数のカウントを加算

			break;
		}
		dwCurrentTime = timeGetTime();					// システム時刻を取得
	}
}

// FPS取得
float GetFps(void)
{
	return fCountFPS;
}
