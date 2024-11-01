#pragma once
#include "Actor.h"
class Treasure : public Actor
{
public:
	Treasure(class Game* game);

	class CircleComponent* GetCircle() { return cc; }
	//class SpriteComponent* GetSprite() { return sc; }
	class MeshComponent* GetMeshComp() { return mc; }
	

private:
	//class SpriteComponent* sc;
	class MeshComponent* mc;
	class CircleComponent* cc;
};

