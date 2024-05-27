#include "InputComponent.h"
#include "Actor.h"
#include "InputSystem.h"

//OpenGL—p‚ÌÀ•WŒn‚ÉC³

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
	//ŒÃ“T•¨—Šw‚ÅMoveComponent‚Ì‚½‚ß‚ÌŒvZ
	//MoveComponent‚É‚Í‘Oi‚©‰ñ“]•ûŒü‚Ì—Í‚ÌÅ‘å’l‚¾‚¯‚ğ“n‚·
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
		rotforce -= mMaxRotForce;		//Šp“x‚Ì{•ûŒü‚ÍCCW
	}
	if (state.Keyboard.GetKeyValue(SDL_Scancode(mCounterClockwiseKey)))
	{
		rotforce += mMaxRotForce;		//Šp“x‚Ì{•ûŒü‚ÍCCW
	}
	SetRotForce(rotforce);

}

