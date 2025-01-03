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
	float GetRotationRatio(const struct InputState& state);

	void SetForwardKey(SDL_Scancode keyCode, float ratio);
	void SetRotationKey(SDL_Scancode keyCode, float ratio);
	void SetForwardMouseButton(int button, float ratio);
	void SetRotationMouseButton(int button, float ratio);
	void SetForwardMousePos(Vector2 pos, float ratio);
	void SetRotationMousePos(Vector2 pos, float ratio);
	void SetForwardMouseScroll(Vector2 pos, float ratio);
	void SetRotationMouseScroll(Vector2 pos, float ratio);
	void SetForwardControllerButton(SDL_GameControllerButton button, float ratio);
	void SetRotationControllerButton(SDL_GameControllerButton button, float ratio);
	void SetForwardControllerLeftTrigger(float value, float ratio);
	void SetRotationControllerLeftTrigger(float value, float ratio);
	void SetForwardControllerRightTrigger(float value, float ratio);
	void SetRotationControllerRightTrigger(float value, float ratio);


private:
	// 前進・回転方向の力の最大値
	float mMaxForwardForce;
	float mMaxRotForce;
	float mMaxForwardVelocity;
	float mMaxRotSpeed;
};