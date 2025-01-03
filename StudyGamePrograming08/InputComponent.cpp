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

	//古典物理学でMoveComponentのための計算
		fwd = mMaxForwardForce * GetForwardRatio(state);
		rot = -mMaxRotForce * GetRotationRatio(state);		//角度の＋方向はCCW
	
	// ニュートン力学を使う場合
	SetForce(fwd * mOwner -> GetForward());
	SetRotForce(rot * Vector3::UnitZ);
}

float InputComponent::GetForwardRatio(const struct InputState& state)
{
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_UP) == EPressed ||
		state.Keyboard.GetKeyState(SDL_SCANCODE_UP) == EHeld)
	{
		return 1.0f;
	}
	else if (state.Keyboard.GetKeyState(SDL_SCANCODE_DOWN) == EPressed ||
		state.Keyboard.GetKeyState(SDL_SCANCODE_DOWN) == EHeld)
	{
		return -1.0f;
	}
	else if (!Math::NearZero(state.Mouse.GetScrollWheel().y))
	{
		return state.Mouse.GetScrollWheel().y * 5.0f;
	}
	else if (!Math::NearZero(state.Controller.GetLeftStick().y)) {
		return state.Controller.GetLeftStick().y * 1.0f;
	}
	else { return 0.0f; }
}

float InputComponent::GetRotRatio(const struct InputState& state)
{
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_LEFT) == EPressed ||
		state.Keyboard.GetKeyState(SDL_SCANCODE_LEFT) == EHeld)
	{
		return -1.0f;
	}
	else if (state.Keyboard.GetKeyState(SDL_SCANCODE_RIGHT) == EPressed ||
		state.Keyboard.GetKeyState(SDL_SCANCODE_RIGHT) == EHeld)
	{
		return 1.0f;
	}
	else if (!Math::NearZero(state.Mouse.GetPosition().x))
	{
		return state.Mouse.GetPosition().x * 0.1f;
	}
	else if (!Math::NearZero(state.Controller.GetRightStick().x)) {
		return - state.Controller.GetRightStick().x * 1.0f;
	}
	else { return 0.0f; }
}