#include "BackGround.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"

BackGround::BackGround(Game* game, int id, float scrollspeed, int updateorder, std::string filename) : Actor(game)
{
	//MoveComponent作成
	MoveComponent* mc = new MoveComponent(this);
	mc->SetVelocity(Vector2(scrollspeed, 0.0f));

	//スプライトコンポーネント作成、テクスチャ設定
	SpriteComponent* sc = new SpriteComponent(this, updateorder);
	sc->SetTexture(game->GetTexture(filename));
	offset = sc->GetTexWidth();

	SetPosition(Vector2(0.0f + id * (offset - 1.0f), 0.0f));

	//背景をゲームに追加
	game->AddBackGround(this);
}

void BackGround::UpdateActor(float deltaTime)
{
	// ラッピング処理する場合
	if(GetPosition().x <= -offset ||
	   GetPosition().x >= offset)
	{
		 SetPosition(Vector2(-GetPosition().x, GetPosition().y));
	}
}