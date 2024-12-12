#pragma once
#include "Actor.h"
#include <vector>

class BackGround : public Actor
{
public:
	BackGround(class Game* game);
	void UpdateActor(float deltaTime) override;

private:
	std::vector<Actor*> mBGs;

};