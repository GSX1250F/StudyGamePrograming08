#include "InputComponent.h"
#include "Actor.h"


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

	//�ÓT�����w��MoveComponent�̂��߂̌v�Z
	if (state.Keyboard.GetKeyState(mFwdKey) == EPressed ||
		state.Keyboard.GetKeyState(mFwdKey) == EHeld)
	{
		// fwd = mMaxForwardVelocity;	//�P���ړ��̏ꍇ
		fwd = mMaxForwardForce;
	}
	if (state.Keyboard.GetKeyState(mBwdKey) == EPressed ||
		state.Keyboard.GetKeyState(mBwdKey) == EHeld)
	{
		// fwd = -mMaxForwardVelocity;       //�P���ړ��̏ꍇ
		fwd = -mMaxForwardForce;
	}
	if (state.Keyboard.GetKeyState(mCwsKey) == EPressed ||
		state.Keyboard.GetKeyState(mCwsKey) == EHeld)
	{
		// rot = mMaxRotSpeed;       //�P���ړ��̏ꍇ
		rot = -mMaxRotForce;		//�p�x�́{������CCW
	}
	if (state.Keyboard.GetKeyState(mCCwsKey) == EPressed ||
		state.Keyboard.GetKeyState(mCCwsKey) == EHeld)
	{
		// rot = -mMaxRotSpeed;       //�P���ړ��̏ꍇ
		rot = mMaxRotForce;		//�p�x�́{������CCW
	}
	
	// �P���ړ��̏ꍇ
	// SetVelocity(fwd * mOwner->GetForward());
	// SetRotSpeed(rot);
	
	// �j���[�g���͊w���g���ꍇ
	SetForce(fwd * mOwner -> GetForward());
	SetRotForce(rot * Vector3::UnitZ);
}