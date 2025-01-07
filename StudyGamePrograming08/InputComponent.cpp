#include "InputComponent.h"
#include "InputSystem.h"
#include "Actor.h"
#include "Math.h"


InputComponent::InputComponent(Actor* owner, int updateOrder) 
	: MoveComponent(owner)
	,mMaxForwardVelocity(0.0f)
	,mMaxRotSpeed(0.0f)
	,mMaxForwardForce(0.0f)
	,mMaxRotForce(0.0f)
{}

void InputComponent::ProcessInput(const struct InputState& state)
{
	float fwd = 0.0f;
	float rot = 0.0f;

	/*
	// 単純移動の場合
	fwd = mMaxForwardVelocity * GetForwardRatio(state);
	rot = -mMaxRotSpeed * GetRotationRatio(state);
	SetVelocity(fwd * mOwner->GetForward());
	SetRotSpeed(rot * Vector3::UnitZ);
	*/
	// ニュートン力学を使う場合
	fwd = mMaxForwardForce * GetForwardRatio(state);
	rot = -mMaxRotForce * GetRotationRatio(state);
	SetForce(fwd * mOwner -> GetForward());
	SetRotForce(rot * Vector3::UnitZ);
}

float InputComponent::GetForwardRatio(const struct InputState& state)
{
	float ratio = 0.0f;
	//キーボード
	for (auto keyConfig : mKeyConfigs)
	{
		if (state.Keyboard.GetKeyValue(keyConfig.input))
		{
			switch (keyConfig.dir)
			{
				case Forward:
					ratio += 1.0;
					break;
				case Backward:
					ratio -= 1.0;
					break;
			}
		}
	}
	//マウスとコントローラ
	for (auto config : mInputDeviceConfigs)
	{
		switch (config.dir)
		{
			case Forward:
				ratio += CalcRatio(config, state);
				break;
			case Backward:
				ratio -= CalcRatio(config, state);
				break;
		}
	}

	return ratio;
}

float InputComponent::GetRotationRatio(const struct InputState& state)
{	
	float ratio = 0.0f;
	//キーボード
	for (auto keyConfig : mKeyConfigs)
	{
		if (state.Keyboard.GetKeyValue(keyConfig.input)) 
		{
			switch (keyConfig.dir)
			{
				case Clockwise:
					ratio += 1.0;
					break;
				case CounterClockwise:
					ratio -= 1.0;
					break;
			}
		}
	}
	//マウスとコントローラ
	for (auto config : mInputDeviceConfigs)
	{
		switch (config.dir)
		{
			case Clockwise:
				ratio += CalcRatio(config, state);
				break;
			case CounterClockwise:
				ratio -= CalcRatio(config, state);
				break;
		}
	}
	
	return ratio;
}

void InputComponent::SetKeyConfig(Direction dir, SDL_Scancode input)
{
	KeyConfig config;
	config.dir = dir;
	config.input = input;
	mKeyConfigs.push_back(config);
}

void InputComponent::SetInputDeviceConfig(Direction dir, InputDevice input, float ratio)
{
	InputDeviceConfig config;
	config.dir = dir;
	config.input = input;
	config.ratio = ratio;
	mInputDeviceConfigs.push_back(config);
}

float InputComponent::CalcRatio(InputDeviceConfig& config, const struct InputState& state)
{
	float ratio = 0.0;
	switch (config.input)
	{
		case Mouse_L_Button:
			ratio = state.Mouse.GetButtonValue(SDL_BUTTON_LEFT) * config.ratio;
			break;
		case Mouse_R_Button:
			ratio = state.Mouse.GetButtonValue(SDL_BUTTON_RIGHT) * config.ratio;
			break;
		case Mouse_MoveUp:
			//相対モードのみ
			if (state.Mouse.GetIsRelative())
			{
				ratio = state.Mouse.GetPosition().y * config.ratio;
			}
			break;
		case Mouse_MoveDown:
			if (state.Mouse.GetIsRelative())
			{
				ratio = - state.Mouse.GetPosition().y * config.ratio;
			}
			break;
		case Mouse_MoveLeft:
			if (state.Mouse.GetIsRelative())
			{
				ratio = -state.Mouse.GetPosition().x * config.ratio;
			}
			break;
		case Mouse_MoveRight:
			if (state.Mouse.GetIsRelative())
			{
				ratio = state.Mouse.GetPosition().x * config.ratio;
			}
			break;
		case Mouse_ScrollUp:
			ratio = state.Mouse.GetScrollWheel().y * config.ratio;
			break;
		case Mouse_ScrollDown:
			ratio = - state.Mouse.GetScrollWheel().y * config.ratio;			
			break;
		case Controller_Dpad_Up:
			ratio = state.Controller.GetButtonValue(SDL_CONTROLLER_BUTTON_DPAD_UP) * config.ratio;
			break;
		case Controller_Dpad_Down:
			ratio = state.Controller.GetButtonValue(SDL_CONTROLLER_BUTTON_DPAD_DOWN) * config.ratio;
			break;
		case Controller_Dpad_Left:
			ratio = state.Controller.GetButtonValue(SDL_CONTROLLER_BUTTON_DPAD_LEFT) * config.ratio;
			break;
		case Controller_Dpad_Right:
			ratio = state.Controller.GetButtonValue(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) * config.ratio;
			break;
		case Controller_X_Button:
			ratio = state.Controller.GetButtonValue(SDL_CONTROLLER_BUTTON_X) * config.ratio;
			break;
		case Controller_Y_Button:
			ratio = state.Controller.GetButtonValue(SDL_CONTROLLER_BUTTON_Y) * config.ratio;
			break;
		case Controller_A_Button:
			ratio = state.Controller.GetButtonValue(SDL_CONTROLLER_BUTTON_A) * config.ratio;
			break;
		case Controller_B_Button:
			ratio = state.Controller.GetButtonValue(SDL_CONTROLLER_BUTTON_B) * config.ratio;
			break;
		case Controller_L_Button:
			ratio = state.Controller.GetButtonValue(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) * config.ratio;
			break;
		case Controller_R_Button:
			ratio = state.Controller.GetButtonValue(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) * config.ratio;
			break;
		case Controller_L_Trigger:
			ratio = state.Controller.GetLeftTrigger() * config.ratio;
			break;
		case Controller_R_Trigger:
			ratio = state.Controller.GetRightTrigger() * config.ratio;
			break;
		case Controller_L_Stick_TiltUp:
			ratio = state.Controller.GetLeftStick().y * config.ratio;
			break;
		case Controller_L_Stick_TiltDown:
			ratio = - state.Controller.GetLeftStick().y * config.ratio;
			break;
		case Controller_L_Stick_TiltLeft:
			ratio = - state.Controller.GetLeftStick().x * config.ratio;
			break;
		case Controller_L_Stick_TiltRight:
			ratio = state.Controller.GetLeftStick().x * config.ratio;
			break;
		case Controller_R_Stick_TiltUp:
			ratio = state.Controller.GetRightStick().y * config.ratio;
			break;
		case Controller_R_Stick_TiltDown:
			ratio = -state.Controller.GetRightStick().y * config.ratio;
			break;
		case Controller_R_Stick_TiltLeft:
			ratio = - state.Controller.GetRightStick().x * config.ratio;
			break;
		case Controller_R_Stick_TiltRight:
			ratio = state.Controller.GetRightStick().x * config.ratio;
			break;
	}
	if (Math::NearZero(ratio) || ratio < 0)
	{
		ratio = 0.0f;
	}

	return ratio;
}
