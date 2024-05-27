#pragma once
#include "Actor.h"
#include <string>

class BackGround : public Actor
{
public:
	BackGround(Game* game , int id, float scrollspeed , int updateorder, std::string filename);
	void UpdateActor(float deltaTime) override;

private:
	int offset;

};

