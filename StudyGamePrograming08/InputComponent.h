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
		
	float GetForwardRatio(const struct InputState& state);
	float GetRotRatio(const struct InputState& state);

private:
	// 前進・回転方向の力の最大値
	float mMaxForwardForce;
	float mMaxRotForce;
	float mMaxForwardVelocity;
	float mMaxRotSpeed;
};