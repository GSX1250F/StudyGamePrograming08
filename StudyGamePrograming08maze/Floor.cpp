#include "Floor.h"
#include "Game.h"
#include "Renderer.h"
//#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "Math.h"

Floor::Floor(Game* game)
	: Actor(game)
{
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Floor.gpmesh"));
	Quaternion q = Quaternion(Vector3::UnitX, Math::PiOver2);
	SetRotation(q);
}
