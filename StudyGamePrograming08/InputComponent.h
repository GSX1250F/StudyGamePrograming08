#pragma once
#include "MoveComponent.h"
#include <SDL.h>

class InputComponent : public MoveComponent
{
public:
	//�R���X�g���N�^�BUpdate Order�͑��߁B
	InputComponent(class Actor* owner, int updateOrder = 10);

	// ���͏����i�I�[�o�[���C�h�j
	void ProcessInput(const uint8_t* keyState) override;
		
	// �Z�b�^�[�E�Q�b�^�[
	void SetMaxForwardVelocity(float value) { mMaxForwardVelocity = value; }
	void SetMaxRotSpeed(float value) { mMaxRotSpeed = value; }
	void SetMaxForwardForce(float value) { mMaxForwardForce = value; }
	void SetMaxRotForce(float value) { mMaxRotForce = value; }
	
	int GetForwardKey() const { return mFwdKey; }
	void SetForwardKey(int key) { mFwdKey = key; }
	int GetBackwardKey() const { return mBwdKey; }
	void SetBackwardKey(int key) { mBwdKey = key; }
	int GetClockwiseKey() const { return mCwsKey; }
	void SetClockwiseKey(int key) { mCwsKey = key; }
	int GetCounterClockwiseKey() const { return mCCwsKey; }
	void SetCounterClockwiseKey(int key) { mCCwsKey = key; }

private:
	// �O�i�E��]�����̗͂̍ő�l
	float mMaxForwardForce;
	float mMaxRotForce;
	float mMaxForwardVelocity;
	float mMaxRotSpeed;
	int mFwdKey;
	int mBwdKey;
	int mCwsKey;
	int mCCwsKey;
};