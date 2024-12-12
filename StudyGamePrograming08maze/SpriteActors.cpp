#include "SpriteActors.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Renderer.h"

SpriteActors::SpriteActors(Game* game) : Actor(game)
{
	Actor* a;
	std::string filename;

	//ClearPict
	a = new ClearPict(game);	
}

ClearPict::ClearPict(Game* game) : Actor(game)
{
	SetState(EPaused);
	SetPosition(Vector3::Zero);
	SetScale(3.0f);

	//スプライトコンポーネント作成、テクスチャ設定
	sc = new SpriteComponent(this, 100);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/ClearPict.png"));
	sc->SetVisible(false);
}
