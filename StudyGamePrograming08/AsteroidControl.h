#pragma once
#include "Actor.h"
class AsteroidControl :public Actor
{
public:
	AsteroidControl(Game* game);
	
	void UpdateActor(float deltaTime) override;

private:
	float mAsteroidCooldown;
	float InitCooldown;
};