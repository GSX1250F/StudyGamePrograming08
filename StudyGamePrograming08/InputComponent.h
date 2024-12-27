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
	float GetRotRatio(const struct InputState& state);

private:
	// �O�i�E��]�����̗͂̍ő�l
	float mMaxForwardForce;
	float mMaxRotForce;
	float mMaxForwardVelocity;
	float mMaxRotSpeed;
};