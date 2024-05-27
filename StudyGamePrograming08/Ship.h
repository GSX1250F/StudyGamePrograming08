#pragma once
#include "Actor.h"

class Ship : public Actor
{
public:
	Ship(Game* game);
	
	void UpdateActor(float deltaTime) override;
	//void ActorInput(const uint8_t* keyState) override;
	void ActorInput(const struct InputState& state) override;
	void Init();

private:
	float mLaserCooldown;
	Vector2 crashPos;		//衝突したときの位置
	float crashRot;			//衝突したときの向き
	bool crash;				//衝突検知
	float mCrashCooldown;		//衝突演出時間
	float mShipCooldown;		//衝突演出後、リセットされるまでスプライトを消す時間
	float mAsteroidCooldown;		//小惑星増殖までの待機時間


	class InputComponent* mInput;
	class CircleComponent* mCircle;
	class AnimSpriteComponent* mAnimComponent;

};

