#pragma once
#include <string>
#include "Actor.h"

class SpriteActors : public Actor
{
public:
	SpriteActors(class Game* game);
};

class ClearPict : public Actor
{
public:
	ClearPict(class Game* game);
	class SpriteComponent* GetSprite() { return sc; }
private:
	SpriteComponent* sc;
};