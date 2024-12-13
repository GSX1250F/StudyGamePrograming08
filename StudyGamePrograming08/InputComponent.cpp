#include "InputComponent.h"
#include "Actor.h"


InputComponent::InputComponent(Actor* owner, int updateOrder) 
	: MoveComponent(owner)
	,mMaxForwardVelocity(0.0f)
	,mMaxRotSpeed(0.0f)
	,mMaxForwardForce(0.0f)
	,mMaxRotForce(0.0f)
{}

void InputComponent::ProcessInput(const SDL_Event& event)
{
	float fwd = 0.0f;
	float rot = 0.0f;

	//�ÓT�����w��MoveComponent�̂��߂̌v�Z
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == mFwdKey)
		{
			// fwd = mMaxForwardVelocity;	//�P���ړ��̏ꍇ
			fwd = mMaxForwardForce;
		}
		else if (event.key.keysym.sym == mBwdKey)
		{
			// fwd = -mMaxForwardVelocity;       //�P���ړ��̏ꍇ
			fwd = -mMaxForwardForce;
		}
		else if (event.key.keysym.sym == mCwsKey)
		{
			// rot = mMaxRotSpeed;       //�P���ړ��̏ꍇ
			rot = -mMaxRotForce;		//�p�x�́{������CCW
		}
		else if (event.key.keysym.sym == mCCwsKey)
		{
			// rot = -mMaxRotSpeed;       //�P���ړ��̏ꍇ
			rot = mMaxRotForce;		//�p�x�́{������CCW
		}
	}
		
	
	// �P���ړ��̏ꍇ
	// SetVelocity(fwd * mOwner->GetForward());
	// SetRotSpeed(rot);
	
	// �j���[�g���͊w���g���ꍇ
	SetForce(fwd * mOwner->GetForward()); 
	SetRotForce(rot * Vector3::UnitZ);
	
}