#include "InputComponent.h"
#include "Actor.h"
#include "InputSystem.h"


InputComponent::InputComponent(Actor* owner, int updateOrder) 
	: MoveComponent(owner)
	,mMaxForwardVelocity(0.0f)
	,mMaxRotSpeed(0.0f)
	,mMaxForwardForce(0.0f)
	,mMaxRotForce(0.0f)
{}

void InputComponent::ProcessInput(const InputState& state)
{
	float fwd = 0.0f;
	float rot = 0.0f;

	//�ÓT�����w��MoveComponent�̂��߂̌v�Z
	if (state.Keyboard.GetKeyValue(mFwdKey))
	{
		// fwd = mMaxForwardVelocity;	//�P���ړ��̏ꍇ
		fwd = mMaxForwardForce;
	}
	else if (state.Keyboard.GetKeyValue(mBwdKey))
	{
		// fwd = -mMaxForwardVelocity;       //�P���ړ��̏ꍇ
		fwd = -mMaxForwardForce;
	}
	else if (state.Keyboard.GetKeyValue(mCwsKey))
	{
		// rot = mMaxRotSpeed;       //�P���ړ��̏ꍇ
		rot = -mMaxRotForce;		//�p�x�́{������CCW
	}
	else if (state.Keyboard.GetKeyValue(mCCwsKey))
	{
		// rot = -mMaxRotSpeed;       //�P���ړ��̏ꍇ
		rot = mMaxRotForce;		//�p�x�́{������CCW
	}
		
	
	// �P���ړ��̏ꍇ
	// SetVelocity(fwd * mOwner->GetForward());
	// SetRotSpeed(rot);
	
	// �j���[�g���͊w���g���ꍇ
	SetForce(fwd * mOwner->GetForward()); 
	SetRotForce(rot * Vector3::UnitZ);
	
}