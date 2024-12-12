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

	//古典物理学でMoveComponentのための計算
	if (keyState[mFwdKey])
	{
		// fwd = mMaxForwardVelocity;	//単純移動の場合
		fwd = mMaxForwardForce;
	}
	if (keyState[mBwdKey])
	{
		// fwd = -mMaxForwardVelocity;       //単純移動の場合
		fwd = -mMaxForwardForce;
	}
	if (keyState[mCwsKey])
	{
		// rot = mMaxRotSpeed;       //単純移動の場合
		rot = -mMaxRotForce;		//角度の＋方向はCCW
	}
	if (keyState[mCCwsKey])
	{
		// rot = -mMaxRotSpeed;       //単純移動の場合
		rot = mMaxRotForce;		//角度の＋方向はCCW
	}
	
	// 単純移動の場合
	// SetVelocity(fwd * mOwner->GetForward());
	// SetRotSpeed(rot);
	
	// ニュートン力学を使う場合
	SetForce(fwd * mOwner -> GetForward());
	SetRotForce(rot * Vector3::UnitZ);
}