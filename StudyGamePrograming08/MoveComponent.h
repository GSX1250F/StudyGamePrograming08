#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	//コンストラクタ。Update Orderは早め。
	MoveComponent(class Actor* owner , int updateOrder = 10);
	~MoveComponent();

	//Updateをオーバーライド
	void Update(float deltatime) override;

	// 単純に動かすのに必要なパラメータのセッター＆ゲッター
	// 速度
	Vector2 GetVelocity() { return mVelocity; }
	void SetVelocity(Vector2 vel) { mVelocity = vel; }
	// 回転速度
	float GetRotSpeed() const { return mRotSpeed; }
	void SetRotSpeed(float rotspeed) { mRotSpeed = rotspeed; }

	// 古典物理に必要なパラメータのセッター＆ゲッター
	//重心にかかる力
	Vector2 GetMoveForce() { return mMoveForce; }	
	void SetMoveForce(Vector2 forceVector) { mMoveForce = forceVector; }
	//回転方向の力
	float GetRotForce() { return mRotForce; }
	void SetRotForce(float force) { mRotForce = force; }	
	//重心移動の抵抗率
	float GetMoveResist() { return mMoveResist; }
	void SetMoveResist(float resist) { mMoveResist = resist; }
	//回転移動の抵抗率
	float GetRotResist() { return mRotResist; }
	void SetRotResist(float resist) { mRotResist = resist; }		
	// 質量
	float GetMass() { return mMass; }
	void SetMass(float mass) { mMass = mass; }
	// トルク
	float GetTorque() { return mTorque; }
	void SetTorque(float torque) { mTorque = torque; }
	// 慣性モーメント
	float GetImoment() { return mImoment; }
	void SetImoment(float imoment) { mImoment = imoment; }


private:
	// 単純移動パラメータ
	Vector2 mVelocity;		// 重心移動速度
	float mRotSpeed;		// 回転速度

	// 古典物理パラメータ
	float mMass;			// 質量
	Vector2 mMoveForce;		// 重心にかかる力
	float mRotForce;		// 回転方向の力F +方向はCCW
	float mTorque;			// トルク=回転方向の力 * 半径 = 慣性モーメント * 回転加速度
	Vector2 mMoveAccel;		// 重心加速度	=重心にかかる力 / 質量
	float mRotAccel;		// 回転加速度
	float mImoment;			// 慣性モーメント
	float mMoveResist;		// 重心速度抵抗率(%)
	float mRotResist;		// 回転速度抵抗率(%)


};

