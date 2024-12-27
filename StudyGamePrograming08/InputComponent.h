#pragma once
#include "MoveComponent.h"
#include <SDL.h>

class InputComponent : public MoveComponent
{
public:
	//コンストラクタ。Update Orderは早め。
	InputComponent(class Actor* owner, int updateOrder = 10);

	// 入力処理（オーバーライド）
	void ProcessInput(const uint8_t* keyState) override;
		
	// セッター・ゲッター
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
	// 前進・回転方向の力の最大値
	float mMaxForwardForce;
	float mMaxRotForce;
	float mMaxForwardVelocity;
	float mMaxRotSpeed;
	int mFwdKey;
	int mBwdKey;
	int mCwsKey;
	int mCCwsKey;
};