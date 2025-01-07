#pragma once
#include "MoveComponent.h"
#include "InputSystem.h"
#include <vector>

enum Direction
{
	Forward,
	Backward,
	Clockwise,
	CounterClockwise
};

enum InputDevice
{
	Mouse_L_Button,
	Mouse_R_Button,
	Mouse_MoveUp,
	Mouse_MoveDown,
	Mouse_MoveLeft,
	Mouse_MoveRight,
	Mouse_ScrollUp,
	Mouse_ScrollDown,
	Controller_Dpad_Up,
	Controller_Dpad_Down,
	Controller_Dpad_Left,
	Controller_Dpad_Right,
	Controller_X_Button,
	Controller_Y_Button,
	Controller_A_Button,
	Controller_B_Button,
	Controller_L_Button,
	Controller_R_Button,
	Controller_L_Trigger,
	Controller_R_Trigger,
	Controller_L_Stick_TiltUp,
	Controller_L_Stick_TiltDown,
	Controller_L_Stick_TiltLeft,
	Controller_L_Stick_TiltRight,
	Controller_R_Stick_TiltUp,
	Controller_R_Stick_TiltDown,
	Controller_R_Stick_TiltLeft,
	Controller_R_Stick_TiltRight
};

struct KeyConfig
{
	Direction dir;
	SDL_Scancode input;
};

struct InputDeviceConfig
{
	Direction dir;
	InputDevice input;
	float ratio;
};

class InputComponent : public MoveComponent
{
public:
	//コンストラクタ。Update Orderは早め。
	InputComponent(class Actor* owner, int updateOrder = 10);

	// 入力処理（オーバーライド）
	void ProcessInput(const struct InputState& state) override;
		
	// セッター・ゲッター
	void SetMaxForwardVelocity(float value) { mMaxForwardVelocity = value; }
	void SetMaxRotSpeed(float value) { mMaxRotSpeed = value; }
	void SetMaxForwardForce(float value) { mMaxForwardForce = value; }
	void SetMaxRotForce(float value) { mMaxRotForce = value; }

	void SetKeyConfig(Direction dir, SDL_Scancode input);
	void SetInputDeviceConfig(Direction dir, InputDevice input, float ratio);
	
	float GetForwardRatio(const struct InputState& state);
	float GetRotationRatio(const struct InputState& state);
	float CalcRatio(InputDeviceConfig& config, const struct InputState& state);

private:
	// 前進・回転方向の力の最大値
	float mMaxForwardForce;
	float mMaxRotForce;
	float mMaxForwardVelocity;
	float mMaxRotSpeed;
	
	std::vector<KeyConfig> mKeyConfigs;
	std::vector<InputDeviceConfig> mInputDeviceConfigs;
};