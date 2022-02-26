
#include "mytime.h"

// �}�N��
#define FRAME_RATE (60)

// ���Ԍv���p
DWORD dwExecLastTime;
DWORD dwFPSLastTime;
DWORD dwCurrentTime;
DWORD dwFrameCount;
float fCountFPS;

void InitTime(void)
{
	// �t���[���J�E���g������
	timeBeginPeriod(1);					// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;
}



void GetTime(void)
{
	dwCurrentTime = timeGetTime();		// �V�X�e���������擾
}


void CheckTime(void)
{
	while (1)
	{
		//FPS�̌v��
		if (dwCurrentTime - dwFPSLastTime >= 500) // 500m/s���Ƀ`�F�b�N���Ă�
		{
			fCountFPS = dwFrameCount * 1000.0f / (dwCurrentTime - dwFPSLastTime);
			dwFPSLastTime = dwCurrentTime;
			dwFrameCount = 0;
		}

		if ((dwCurrentTime - dwExecLastTime) >= (1000 / FRAME_RATE))	// 1/60�b���ƂɎ��s
		{
			dwExecLastTime = dwCurrentTime;	// ��������������ۑ�
			dwFrameCount++;		// �����񐔂̃J�E���g�����Z

			break;
		}
		dwCurrentTime = timeGetTime();					// �V�X�e���������擾
	}
}

// FPS�擾
float GetFps(void)
{
	return fCountFPS;
}
