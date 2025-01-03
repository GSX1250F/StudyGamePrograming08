#pragma once
#include "MoveComponent.h"
#include "InputSystem.h"
#include <SDL.h>

class InputComponent : public MoveComponent
{
public:
	//�R���X�g���N�^�BUpdate Order�͑��߁B
	InputComponent(class Actor* owner, int updateOrder = 10);

	// ���͏����i�I�[�o�[���C�h�j
	void ProcessInput(const struct InputState& state) override;
		
	// �Z�b�^�[�E�Q�b�^�[
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
	// �O�i�E��]�����̗͂̍ő�l
	float mMaxForwardForce;
	float mMaxRotForce;
	float mMaxForwardVelocity;
	float mMaxRotSpeed;
};