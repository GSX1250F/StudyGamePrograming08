#include "InputComponent.h"
#include "Actor.h"


InputComponent::InputComponent(Actor* owner, int updateOrder) 
	: MoveComponent(owner)
	,mMaxForwardVelocity(0.0f)
	,mMaxRotSpeed(0.0f)
	,mMaxForwardForce(0.0f)
	,mMaxRotForce(0.0f)
{}

void InputComponent::ProcessInput(const uint8_t* keyState)
{
	float fwd = 0.0f;
	float rot = 0.0f;

	//�ÓT�����w��MoveComponent�̂��߂̌v�Z
	if (keyState[mFwdKey])
	{
		// fwd = mMaxForwardVelocity;	//�P���ړ��̏ꍇ
		fwd = mMaxForwardForce;
	}
	if (keyState[mBwdKey])
	{
		// fwd = -mMaxForwardVelocity;       //�P���ړ��̏ꍇ
		fwd = -mMaxForwardForce;
	}
	if (keyState[mCwsKey])
	{
		// rot = mMaxRotSpeed;       //�P���ړ��̏ꍇ
		rot = -mMaxRotForce;		//�p�x�́{������CCW
	}
	if (keyState[mCCwsKey])
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