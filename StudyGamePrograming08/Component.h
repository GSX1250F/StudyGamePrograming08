#pragma once
#include <cstdint>

class Component
{
public:
	// コンストラクタ
	// updateOrderが小さいコンポーネントほど早く更新される
	Component(class Actor* owner, int updateOrder = 100);

	// デストラクタ
	virtual ~Component();

	// 各コンポーネント更新（オーバーライド可能）
	virtual void Update(float deltaTime) {};

	// 各コンポーネント入力処理（オーバーライド可能）
	//virtual void ProcessInput(const uint8_t* keyState) {}
	virtual void ProcessInput(const struct InputState& state) {}

	// ワールド変換が変更されたときに呼び出される
	virtual void OnUpdateWorldTransform() {}

	// 更新順ゲッター
	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	// 所有アクター
	class Actor* mOwner;

	// コンポーネントの更新順序
	int mUpdateOrder;
};