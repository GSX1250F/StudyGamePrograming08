#include "Tile.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "CircleComponent.h"

Tile::Tile(class Game* game)
	:Actor(game)
	, mTileState(EDefault)
	, mParent(nullptr)
{
	//�X�v���C�g�R���|�[�l���g���쐬
	sc = new SpriteComponent(this, 10);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Wall.png"));
	mTexSize = sc->GetTexWidth();
	
	//CircleComponent�쐬
	cc = new CircleComponent(this);	
}

void Tile::UpdateActor(float deltaTime)
{
	UpdateTexture();
}

void Tile::SetTileState(TileState state)
{
	mTileState = state;
	UpdateTexture();
}

void Tile::UpdateTexture()
{
	switch (mTileState)
	{
		case EWall:
			sc->SetVisible(true);
			break;
		default:
			sc->SetVisible(false);
	}
}