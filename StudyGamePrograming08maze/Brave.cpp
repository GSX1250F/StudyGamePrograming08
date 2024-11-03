#include "Brave.h"
#include "Game.h"
#include "Renderer.h"
#include "Maze.h"
#include "Tile.h"
#include "Treasure.h"
#include "SpriteComponent.h"
#include "CircleComponent.h"
#include "MoveComponent.h"

Brave::Brave(Game* game)
	: Actor(game)
	, speed(1000.0f)	
{	
	SetScale(0.85f);	
	SetRadius(150.0f);
	//CircleComponentì¬
	cc = new CircleComponent(this);

	//MoveComponentì¬
	mc = new MoveComponent(this);	
}

void Brave::ActorInput(const SDL_Event& event){
	if (GetGame()->GetMaze()->GetGameStart()) {
		float forwardSpeed = 0.0f;
		float angularSpeed = 0.0f;
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_UP)
			{
				forwardSpeed = speed;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				forwardSpeed = -speed;
			}
			else if (event.key.keysym.sym == SDLK_LEFT)
			{
				angularSpeed = Math::Pi;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				angularSpeed = -Math::Pi;
			}
		}
		mc->SetVelocity(forwardSpeed * GetForward());
		mc->SetRotSpeed(angularSpeed * Vector3::UnitZ);	
	}	
}

void Brave::UpdateActor(float deltaTime){
	if (GetGame()->GetMaze()->GetGameStart()) {
		if (Intersect(*cc, *GetGame()->GetMaze()->GetTreasure()->GetCircle())) {
			// ƒS[ƒ‹
			GetGame()->GetMaze()->SetGameClear(true);
		}

		for (auto tilecol : GetGame()->GetMaze()->GetTiles())
		{
			for (auto tile : tilecol)
			{
				if (tile->GetTileState() == Tile::EWall) {
					//•Ç‚ÉÕ“Ë‚µ‚Ä‚¢‚½‚ç—£‚·B
					if (Intersect(*cc, *tile->GetCircle())) {
						//‚Q‚Â‚Ì’†S‚ðŒ‹‚Ôü•ªã‚ÅA‚Q‚Â‚Ì”¼Œa‚¾‚¯—£‚·
						//Vector3 diff = Vector3::Normalize(this->GetPosition() - tile->GetPosition());
						//this->SetPosition(tile->GetPosition() + diff * (this->GetRadius() + tile->GetRadius()));
					}
				}
			}
		}

		// Compute new camera from this actor
		Vector3 cameraPos = GetPosition();
		Vector3 cameraTarget = GetPosition() + GetForward() * 1.0f;
		Vector3 cameraUp = -1.0 * Vector3::UnitZ;
		
		Matrix4 view = Matrix4::CreateLookAt(cameraPos, cameraTarget, cameraUp);
		GetGame()->GetRenderer()->SetViewMatrix(view);
	}	
}