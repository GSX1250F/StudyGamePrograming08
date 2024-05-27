#include "InputComponent.h"
#include "Actor.h"
#include "InputSystem.h"

//OpenGL�p�̍��W�n�ɏC��

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
	//�ÓT�����w��MoveComponent�̂��߂̌v�Z
	//MoveComponent�ɂ͑O�i����]�����̗͂̍ő�l������n��
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
		rotforce -= mMaxRotForce;		//�p�x�́{������CCW
	}
	if (state.Keyboard.GetKeyValue(SDL_Scancode(mCounterClockwiseKey)))
	{
		rotforce += mMaxRotForce;		//�p�x�́{������CCW
	}
	SetRotForce(rotforce);

}

