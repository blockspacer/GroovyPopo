
#include "main.h"
#include "controller.h"



// パッド変数
NpadJoyDualState g_OldPadState;
NpadJoyDualState g_PadState;


// 振動子変数
VibrationDeviceHandle g_VibrationDevice[2];
int g_VibrationLeftFrame;
int g_VibrationRightFrame;
float g_VibrationLeftFreq = 320;
float g_VibrationRightFreq = 320;

// 6軸センサ変数
SixAxisSensorHandle g_SixAxisSensor[2];
SixAxisSensorState g_SixAxisSensorState[2] = {};

// タッチスクリーン変数
TouchScreenState<1> g_TouchScreenState;


#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

#define PAD_BTN_NUM	(24)

XINPUT_STATE		xi_sts;
XINPUT_VIBRATION	xi_vib;

int btn_code[PAD_BTN_NUM] =
{
	XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_Y, XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_LEFT_THUMB, XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_LEFT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER,
	0, 0,
	XINPUT_GAMEPAD_START, XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_UP, XINPUT_GAMEPAD_DPAD_RIGHT, XINPUT_GAMEPAD_DPAD_DOWN,
	0, 0, 0, 0, 0, 0, 0, 0,
};

int vk_code[PAD_BTN_NUM] =
{
	VK_RIGHT, VK_DOWN, VK_UP, VK_LEFT,
	'B', 'M',
	VK_LCONTROL, VK_RCONTROL,
	VK_LSHIFT, VK_RSHIFT,
	VK_RETURN, VK_SPACE,
	'A', 'W', 'D', 'S',
	0, 0, 0, 0, 0, 0, 0, 0,
};

RECT screen_rect;
POINT touch_pt;


void InitController()
{

	//// パッド初期化

	//InitializeNpad();
	//NpadIdType npadIds[] = { NpadId::No1,
	//						NpadId::Handheld };
	//SetSupportedNpadStyleSet(NpadStyleJoyDual::Mask | NpadStyleHandheld::Mask);
	//SetSupportedNpadIdType(npadIds, 2);



	//// 振動子初期化
	//if (GetNpadStyleSet(NpadId::No1).Test<nn::hid::NpadStyleJoyDual>())
	//	GetVibrationDeviceHandles(g_VibrationDevice, 2, NpadId::No1, NpadStyleJoyDual::Mask);
	//else if (GetNpadStyleSet(NpadId::Handheld).Test<nn::hid::NpadStyleHandheld>())
	//	GetVibrationDeviceHandles(g_VibrationDevice, 2, NpadId::Handheld, NpadStyleHandheld::Mask);

	//for (int i = 0; i < 2; i++)
	//{
	//	InitializeVibrationDevice(g_VibrationDevice[i]);
	//}

	//g_VibrationLeftFrame = 0;
	//g_VibrationRightFrame = 0;

	//

	//// 6軸センサ初期化
	//if (GetNpadStyleSet(NpadId::No1).Test<nn::hid::NpadStyleJoyDual>())
	//	GetSixAxisSensorHandles(g_SixAxisSensor, 2, NpadId::No1, NpadStyleJoyDual::Mask);
	//else if (GetNpadStyleSet(NpadId::Handheld).Test<nn::hid::NpadStyleHandheld>())
	//	GetSixAxisSensorHandles(g_SixAxisSensor, 2, NpadId::Handheld, NpadStyleHandheld::Mask);

	//for (int i = 0; i < 2; i++)
	//{
	//	StartSixAxisSensor(g_SixAxisSensor[i]);
	//}



	//// タッチスクリーン初期化
	//InitializeTouchScreen();

	UninitController();
	XInputEnable(true);

	g_PadState.buttons.Reset();
	g_OldPadState.buttons.Reset();

	memset(&xi_sts, 0, sizeof(XINPUT_STATE));
	memset(&xi_vib, 0, sizeof(XINPUT_VIBRATION));

	GetClientRect(GetForegroundWindow(), &screen_rect);
	GetControllerTouchScreen();
}



void UninitController()
{
	XInputEnable(false);
}


void UpdateController()
{

	g_OldPadState = g_PadState;


	//// パッド状態取得
	//if (GetNpadStyleSet(NpadId::No1).Test<nn::hid::NpadStyleJoyDual>())
	//	GetNpadState(&g_PadState, NpadId::No1);
	//else if (GetNpadStyleSet(NpadId::Handheld).Test<nn::hid::NpadStyleHandheld>())
	//	GetNpadState((NpadHandheldState*)&g_PadState, NpadId::Handheld);


	//// 6軸センサ状態取得
	//GetSixAxisSensorState(&g_SixAxisSensorState[0], g_SixAxisSensor[0]);

	//if (g_SixAxisSensor[1]._storage)
	//	GetSixAxisSensorState(&g_SixAxisSensorState[1], g_SixAxisSensor[1]);
	//else
	//	g_SixAxisSensorState[1] = g_SixAxisSensorState[0];


	//// タッチスクリーン状態取得
	//GetTouchScreenStates(&g_TouchScreenState, 1);



	// 振動子停止
	if (g_VibrationLeftFrame > 0)
	{
		g_VibrationLeftFrame--;

		if (g_VibrationLeftFrame == 0)
		{
			//		VibrationValue vibration = VibrationValue::Make();

			//		for (int i = 0; i < 2; i++)
			//		{
			//			VibrationDeviceInfo info;
			//			GetVibrationDeviceInfo(&info, g_VibrationDevice[i]);
			//			if (info.position == VibrationDevicePosition_Left)
			//			{
			//				SendVibrationValue(g_VibrationDevice[i], vibration);
			//			}
			//		}

			xi_vib.wLeftMotorSpeed = 0;
		}
	}

	if (g_VibrationRightFrame > 0)
	{
		g_VibrationRightFrame--;

		if (g_VibrationRightFrame == 0)
		{
			//		VibrationValue vibration = VibrationValue::Make();

			//		for (int i = 0; i < 2; i++)
			//		{
			//			VibrationDeviceInfo info;
			//			GetVibrationDeviceInfo(&info, g_VibrationDevice[i]);
			//			if (info.position == VibrationDevicePosition_Right)
			//			{
			//				SendVibrationValue(g_VibrationDevice[i], vibration);
			//			}
			//		}

			xi_vib.wRightMotorSpeed = 0;
		}
	}


	g_PadState.buttons.Reset();

	// XInput
	XInputGetState(0, &xi_sts);
	for (int i = 0; i < PAD_BTN_NUM; i++)
	{
		if (xi_sts.Gamepad.wButtons & btn_code[i]) g_PadState.buttons.Set(i, true);
	}
	if (xi_sts.Gamepad.bLeftTrigger) g_PadState.buttons.Set(nn::hid::NpadButton::ZL::Index, true);
	if (xi_sts.Gamepad.bRightTrigger) g_PadState.buttons.Set(nn::hid::NpadButton::ZR::Index, true);

	// キーボード
	for (int i = 0; i < PAD_BTN_NUM; i++)
	{
		if (GetAsyncKeyState(vk_code[i])) g_PadState.buttons.Set(i, true);
	}
}


bool GetControllerPress(int button)
{
	return g_PadState.buttons.Test(button);
}

bool GetControllerTrigger(int button)
{
	return (g_PadState.buttons ^ g_OldPadState.buttons & g_PadState.buttons).Test(button);

}


Float2 GetControllerLeftStick()
{
	Float2 stick;
	//	stick.x = (float)g_PadState.analogStickL.x / AnalogStickMax;
	//	stick.y = (float)g_PadState.analogStickL.y / AnalogStickMax;

		// XInput
	stick.x = (float)xi_sts.Gamepad.sThumbLX / 0x7FFF;
	stick.y = (float)xi_sts.Gamepad.sThumbLY / 0x7FFF;
	if (fabs(stick.x) < 0.1f) stick.x = 0;
	if (fabs(stick.y) < 0.1f) stick.y = 0;
	float r = sqrt(stick.x * stick.x + stick.y * stick.y);
	if (r > 1)
	{
		stick.x /= r;
		stick.y /= r;
	}

	// キーボード
	//if (stick.x == 0 && stick.y == 0)
	//{
	//	stick.x = (float)((GetAsyncKeyState('H') != 0) - (GetAsyncKeyState('F') != 0));
	//	stick.y = (float)((GetAsyncKeyState('T') != 0) - (GetAsyncKeyState('G') != 0));
	//	if (stick.x != 0 && stick.y != 0)
	//	{
	//		stick.x /= 1.414f;
	//		stick.y /= 1.414f;
	//	}
	//}

	return stick;
}

Float2 GetControllerRightStick()
{
	Float2 stick;
	//stick.x = (float)g_PadState.analogStickR.x / AnalogStickMax;
	//stick.y = (float)g_PadState.analogStickR.y / AnalogStickMax;

	// XInput
	stick.x = (float)xi_sts.Gamepad.sThumbRX / 0x7FFF;
	stick.y = (float)xi_sts.Gamepad.sThumbRY / 0x7FFF;
	if (fabs(stick.x) < 0.1f) stick.x = 0;
	if (fabs(stick.y) < 0.1f) stick.y = 0;
	float r = sqrt(stick.x * stick.x + stick.y * stick.y);
	if (r > 1)
	{
		stick.x /= r;
		stick.y /= r;
	}

	// キーボード
	//if (stick.x == 0 && stick.y == 0)
	//{
	//	stick.x = (float)((GetAsyncKeyState('J') != 0) - (GetAsyncKeyState('L') != 0));
	//	stick.y = (float)((GetAsyncKeyState('I') != 0) - (GetAsyncKeyState('K') != 0));
	//	if (stick.x != 0 && stick.y != 0)
	//	{
	//		stick.x /= 1.414f;
	//		stick.y /= 1.414f;
	//	}
	//}

	return stick;
}




Float3 GetControllerLeftAcceleration()
{
	return g_SixAxisSensorState[0].acceleration;
}

Float3 GetControllerRightAcceleration()
{
	return g_SixAxisSensorState[1].acceleration;
}

Float3 GetControllerLeftAngle()
{
	return g_SixAxisSensorState[0].angle;
}

Float3 GetControllerRightAngle()
{
	return g_SixAxisSensorState[1].angle;
}



void SetControllerLeftVibration(int frame)
{

	//for (int i = 0; i < 2; i++)
	//{
	//	VibrationDeviceInfo info;
	//	GetVibrationDeviceInfo(&info, g_VibrationDevice[i]);
	//	if (info.position == VibrationDevicePosition_Left)
	//	{
	//		VibrationValue vibration = VibrationValue::Make(0.0f, 10.0f, 0.5f, g_VibrationLeftFreq);
	//		SendVibrationValue(g_VibrationDevice[i], vibration);
	//	}
	//}

	g_VibrationLeftFrame = frame;

	xi_vib.wLeftMotorSpeed = 0xFFFF;
}


void SetControllerRightVibration(int frame)
{

	//for (int i = 0; i < 2; i++)
	//{
	//	VibrationDeviceInfo info;
	//	GetVibrationDeviceInfo(&info, g_VibrationDevice[i]);
	//	if (info.position == VibrationDevicePosition_Right)
	//	{
	//		VibrationValue vibration = VibrationValue::Make(0.0f, 10.0f, 0.5f, g_VibrationRightFreq);
	//		SendVibrationValue(g_VibrationDevice[i], vibration);
	//	}
	//}

	g_VibrationRightFrame = frame;

	xi_vib.wRightMotorSpeed = 0xFFFF;
}

void SetControllerLeftFreq(float freq)
{
	g_VibrationLeftFreq = freq;
}

void SetControllerRightFreq(float freq)
{
	g_VibrationRightFreq = freq;
}


bool GetControllerTouchScreen()
{
	//if (g_TouchScreenState.count > 0)
	//	return true;
	//else
	//	return false;

	GetCursorPos(&touch_pt);
	ScreenToClient(GetForegroundWindow(), &touch_pt);

	if (GetAsyncKeyState(VK_LBUTTON) && PtInRect(&screen_rect, touch_pt))
	{
		return true;
	}

	return false;
}

Float2 GetControllerTouchScreenPosition()
{
	Float2 position;
	position.x = g_TouchScreenState.touches[0].x * SCREEN_WIDTH / 1280.0f - SCREEN_WIDTH / 2.0f;
	position.y = g_TouchScreenState.touches[0].y * SCREEN_HEIGHT / 720.0f - SCREEN_HEIGHT / 2.0f;

	position.x = (float)touch_pt.x * SCREEN_WIDTH / screen_rect.right - SCREEN_WIDTH / 2;
	position.y = (float)touch_pt.y * SCREEN_HEIGHT / screen_rect.bottom - SCREEN_HEIGHT / 2;

	return position;
}
