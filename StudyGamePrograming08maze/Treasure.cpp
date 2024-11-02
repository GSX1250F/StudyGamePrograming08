#include "Treasure.h"
#include "Game.h"
#include "Renderer.h"
//#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "CircleComponent.h"

Treasure::Treasure(Game* game) : Actor(game){
	/*
	//スプライトコンポーネント作成、テクスチャ設定
	sc = new SpriteComponent(this, 20);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Treasure.png"));
	*/
	SetScale(100.0f);
	mc = new MeshComponent(this);
	mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Treasure.gpmesh"));
	//CircleComponent作成
	cc = new CircleComponent(this);
}
