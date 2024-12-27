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

	//古典物理学でMoveComponentのための計算
	if (state.Keyboard.GetKeyState(mFwdKey) == EPressed ||
		state.Keyboard.GetKeyState(mFwdKey) == EHeld)
	{
		// fwd = mMaxForwardVelocity;	//単純移動の場合
		fwd = mMaxForwardForce;
	}
	if (state.Keyboard.GetKeyState(mBwdKey) == EPressed ||
		state.Keyboard.GetKeyState(mBwdKey) == EHeld)
	{
		// fwd = -mMaxForwardVelocity;       //単純移動の場合
		fwd = -mMaxForwardForce;
	}
	if (state.Keyboard.GetKeyState(mCwsKey) == EPressed ||
		state.Keyboard.GetKeyState(mCwsKey) == EHeld)
	{
		// rot = mMaxRotSpeed;       //単純移動の場合
		rot = -mMaxRotForce;		//角度の＋方向はCCW
	}
	if (state.Keyboard.GetKeyState(mCCwsKey) == EPressed ||
		state.Keyboard.GetKeyState(mCCwsKey) == EHeld)
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