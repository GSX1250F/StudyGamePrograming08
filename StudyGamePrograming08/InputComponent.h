#pragma once
#include "MoveComponent.h"
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
	
	SDL_Scancode GetForwardKey() const { return mFwdKey; }
	void SetForwardKey(SDL_Scancode key) { mFwdKey = key; }
	SDL_Scancode GetBackwardKey() const { return mBwdKey; }
	void SetBackwardKey(SDL_Scancode key) { mBwdKey = key; }
	SDL_Scancode GetClockwiseKey() const { return mCwsKey; }
	void SetClockwiseKey(SDL_Scancode key) { mCwsKey = key; }
	SDL_Scancode GetCounterClockwiseKey() const { return mCCwsKey; }
	void SetCounterClockwiseKey(SDL_Scancode key) { mCCwsKey = key; }


private:
	// �O�i�E��]�����̗͂̍ő�l
	float mMaxForwardForce;
	float mMaxRotForce;
	float mMaxForwardVelocity;
	float mMaxRotSpeed;
	SDL_Scancode mFwdKey;
	SDL_Scancode mBwdKey;
	SDL_Scancode mCwsKey;
	SDL_Scancode mCCwsKey;

	float mMouseMaxRange;
};