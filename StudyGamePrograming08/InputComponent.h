#pragma once
#include "MoveComponent.h"
#include <cstdint>

class InputComponent : public MoveComponent
{
public:
	//コンストラクタ。Update Orderは早め。
	InputComponent(class Actor* owner);
	~InputComponent();

	// 入力処理（オーバーライド）
	//void ProcessInput(const uint8_t* keyState) override;
	void ProcessInput(const struct InputState& state) override;

	// プライベート関数のゲッター・セッター
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
	// 前進・後退のためのキー
	int mForwardKey;
	int mBackwardKey;

	//回転運動のキー
	int mClockwiseKey;
	int mCounterClockwiseKey;
	
	// 前進・回転方向の力の最大値
	float mMaxForwardForce;
	float mMaxRotForce;
};

