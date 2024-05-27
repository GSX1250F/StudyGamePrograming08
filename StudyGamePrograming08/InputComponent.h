#pragma once
#include "MoveComponent.h"
#include <cstdint>

class InputComponent : public MoveComponent
{
public:
	//�R���X�g���N�^�BUpdate Order�͑��߁B
	InputComponent(class Actor* owner);
	~InputComponent();

	// ���͏����i�I�[�o�[���C�h�j
	//void ProcessInput(const uint8_t* keyState) override;
	void ProcessInput(const struct InputState& state) override;

	// �v���C�x�[�g�֐��̃Q�b�^�[�E�Z�b�^�[
	int GetForwardKey() const { return mForwardKey; }
	int GetBackwardKey() const { return mBackwardKey; }
	int GetClockwiseKey() const { return mClockwiseKey; }
	int GetCounterClockwiseKey() const { return mCounterClockwiseKey; }
	float GetMaxForwardForce() const { return mMaxForwardForce; }
	float GetMaxRotForce() const { return mMaxRotForce; }

	void SetForwardKey(int key) { mForwardKey = key; }
	void SetBackwardKey(int key) { mBackwardKey = key; }
	void SetClockwiseKey(int key) { mClockwiseKey = key; }
	void SetCounterClockwiseKey(int key) { mCounterClockwiseKey = key; }
	void SetMaxForwardForce(float power) { mMaxForwardForce = power; }
	void SetMaxRotForce(float power) { mMaxRotForce = power; }
	
private:
	// �O�i�E��ނ̂��߂̃L�[
	int mForwardKey;
	int mBackwardKey;

	//��]�^���̃L�[
	int mClockwiseKey;
	int mCounterClockwiseKey;
	
	// �O�i�E��]�����̗͂̍ő�l
	float mMaxForwardForce;
	float mMaxRotForce;
};

