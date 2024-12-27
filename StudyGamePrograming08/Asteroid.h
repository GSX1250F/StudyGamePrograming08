#pragma once
#include "Actor.h"
#include <string>

class Asteroid : public Actor
{
public:
	Asteroid(Game* game);
	~Asteroid();

	class CircleComponent* GetCircle() { return mCircle; }

	void UpdateActor(float deltaTime) override;

private:
	class CircleComponent* mCircle;		//衝突チェックのためのアクセスポインタ。他のオブジェクトから参照するため。
	float mAsteroidCooldown;		//増殖までの待機時間
	std::string mChunkFile;
};