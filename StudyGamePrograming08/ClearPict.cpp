#include "ClearPict.h"
#include "SpriteComponent.h"
#include "Game.h"

ClearPict::ClearPict(Game* game) : Actor(game)
{
	SetState(EPaused);
	//スプライトコンポーネント作成、テクスチャ設定
	SpriteComponent* sc = new SpriteComponent(this, 100);
	sc->SetTexture(game->GetTexture("Assets/ClearPict.png"));
}
