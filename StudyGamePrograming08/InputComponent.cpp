#include "InputComponent.h"
#include "Actor.h"
#include "InputSystem.h"

//OpenGL用の座標系に修正

InputComponent::InputComponent(class Actor* owner) 
	: MoveComponent(owner)
	, mForwardKey(0)
	, mBackwardKey(0)
	, mClockwiseKey(0)
	, mCounterClockwiseKey(0)
{
}

InputComponent::~InputComponent()
{
}

//void InputComponent::ProcessInput(const uint8_t* keyState)
void InputComponent::ProcessInput(const struct InputState& state)
{
	//古典物理学でMoveComponentのための計算
	//MoveComponentには前進か回転方向の力の最大値だけを渡す
	float forwardforce = 0.0f;
	if (state.Keyboard.GetKeyValue(SDL_Scancode(mForwardKey)))
	{
		forwardforce += mMaxForwardForce;
	}
	if (state.Keyboard.GetKeyValue(SDL_Scancode(mBackwardKey)))
	{
		forwardforce -= mMaxForwardForce;
	}
	SetMoveForce(forwardforce * mOwner->GetForward());
	
	float rotforce = 0.0f;
	if (state.Keyboard.GetKeyValue(SDL_Scancode(mClockwiseKey)))
	{
		rotforce -= mMaxRotForce;		//角度の＋方向はCCW
	}
	if (state.Keyboard.GetKeyValue(SDL_Scancode(mCounterClockwiseKey)))
	{
		rotforce += mMaxRotForce;		//角度の＋方向はCCW
	}
	SetRotForce(rotforce);

}

