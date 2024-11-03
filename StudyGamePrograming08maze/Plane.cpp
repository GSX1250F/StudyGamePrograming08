#include "Plane.h"
#include "Game.h"
#include "Renderer.h"
//#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "Math.h"

Plane::Plane(Game* game)
	: Actor(game)
{
	SetScale(3.0f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Plane.gpmesh"));
	//Quaternion q = Quaternion(Vector3::UnitX, Math::PiOver2);
	//SetRotation(q);
}
