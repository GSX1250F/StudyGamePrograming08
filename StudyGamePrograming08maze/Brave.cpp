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
	//CircleComponent作成
	cc = new CircleComponent(this);

	//MoveComponent作成
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
		mc->SetRotSpeed(angularSpeed * GetUpward());	
	}	
}

void Brave::UpdateActor(float deltaTime){
	if (GetGame()->GetMaze()->GetGameStart()) {
		if (Intersect(*cc, *GetGame()->GetMaze()->GetTreasure()->GetCircle())) {
			// ゴール
			GetGame()->GetMaze()->SetGameClear(true);
		}

		for (auto tilecol : GetGame()->GetMaze()->GetTiles())
		{
			for (auto tile : tilecol)
			{
				if (tile->GetTileState() == Tile::EWall) {
					//壁に衝突していたら離す。
					if (Intersect(*cc, *tile->GetCircle())) {
						//２つの中心を結ぶ線分上で、２つの半径だけ離す
						Vector3 diff = Vector3::Normalize(this->GetPosition() - tile->GetPosition());
						this->SetPosition(tile->GetPosition() + diff * (this->GetRadius() + tile->GetRadius()));
					}
				}
			}
		}

		Vector3 cameraPos = GetPosition();
		Vector3 cameraTarget = GetPosition() + GetForward() * 300.0f;
		Vector3 cameraUp = -1.0 * GetUpward();
		
		Matrix4 view = Matrix4::CreateLookAt(cameraPos, cameraTarget, cameraUp);
		GetGame()->GetRenderer()->SetViewMatrix(view);
	}	
}