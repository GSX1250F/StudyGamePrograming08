#pragma once
#include "MoveComponent.h"
#include "InputSystem.h"
#include <SDL.h>

class InputComponent : public MoveComponent
{
public:
	//コンストラクタ。Update Orderは早め。
	InputComponent(class Actor* owner, int updateOrder = 10);

	// 入力処理（オーバーライド）
	void ProcessInput(const struct InputState& state) override;
		
	// セッター・ゲッター
	void SetMaxForwardVelocity(float value) { mMaxForwardVelocity = value; }
	void SetMaxRotSpeed(float value) { mMaxRotSpeed = value; }
	void SetMaxForwardForce(float value) { mMaxForwardForce = value; }
	void SetMaxRotForce(float value) { mMaxRotForce = value; }
	
	void SetForwardKey(SDL_Scancode key) { mFwdKey = key; }
	void SetBackwardKey(SDL_Scancode key) { mBwdKey = key; }
	void SetClockwiseKey(SDL_Scancode key) { mCwsKey = key; }
	void SetCounterClockwiseKey(SDL_Scancode key) { mCCwsKey = key; }
	SDL_Scancode GetForwardKey() { return mFwdKey; }
	SDL_Scancode GetBackwardKey() { return mBwdKey; }
	SDL_Scancode GetClockwiseKey() { return mCwsKey; }
	SDL_Scancode GetCounterClockwiseKey() { return mCCwsKey; }

private:
	// 前進・回転方向の力の最大値
	float mMaxForwardForce;
	float mMaxRotForce;
	float mMaxForwardVelocity;
	float mMaxRotSpeed;
	SDL_Scancode mFwdKey;
	SDL_Scancode mBwdKey;
	SDL_Scancode mCwsKey;
	SDL_Scancode mCCwsKey;
};