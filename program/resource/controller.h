#pragma once


#include <nn/hid/hid_NpadJoy.h>
#include <nn/hid/hid_Vibration.h>
#include <nn/hid/hid_NpadSixAxisSensor.h>
#include <nn/hid/hid_TouchScreen.h>

using namespace nn::hid;

void InitController();
void UninitController();
void UpdateController();

bool GetControllerPress(int button);
bool GetControllerTrigger(int button);

Float2 GetControllerLeftStick();
Float2 GetControllerRightStick();

void SetControllerLeftVibration(int frame);
void SetControllerRightVibration(int frame);
void SetControllerLeftFreq(float freq);
void SetControllerRightFreq(float freq);

Float3 GetControllerLeftAcceleration();
Float3 GetControllerRightAcceleration();
Float3 GetControllerLeftAngle();
Float3 GetControllerRightAngle();

bool GetControllerTouchScreen();
Float2 GetControllerTouchScreenPosition();
