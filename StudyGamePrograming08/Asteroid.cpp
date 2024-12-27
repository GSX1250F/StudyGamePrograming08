#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Random.h"
#include "CircleComponent.h"
#include "SoundPlayer.h"

Asteroid::Asteroid(Game* game) : Actor(game)
{
	//ランダムな位置と向きと大きさと初速で初期化
	Vector2 randPos = Vector2::Zero;
	//画面の中央3/5区画以外になるまで繰り返し処理
	while (randPos.x > GetGame()->mWindowWidth * (-0.3f) &&
		   randPos.x < GetGame()->mWindowWidth * 0.3f &&
		   randPos.y > GetGame()->mWindowHeight * (-0.3f) &&
		   randPos.y < GetGame()->mWindowHeight * 0.3f)
	{
		randPos = Random::GetVector(-0.5f * Vector2(GetGame()->mWindowWidth, GetGame()->mWindowHeight), 0.5f * Vector2(GetGame()->mWindowWidth, GetGame()->mWindowHeight));
	}
	SetPosition(Vector3(randPos.x,randPos.y,0.0f));
	SetRotation(Quaternion(Vector3::UnitZ, Random::GetFloatRange(0.0f, Math::TwoPi)));
	SetScale(Random::GetFloatRange(0.8f, 2.5f));
	
	//スプライトコンポーネント作成、テクスチャ設定
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Asteroid.png"));

	//MoveComponent作成
	MoveComponent* mc = new MoveComponent(this);
	mc->SetVelocity(Random::GetFloatRange(50.0f, 200.0f) * GetForward());
	mc->SetRotSpeed(Vector3(0.0f, 0.0f, Random::GetFloatRange(-1.0f * Math::Pi, 1.0f * Math::Pi)));
	
	//CircleComponent作成
	mCircle = new CircleComponent(this);

	//効果音作成
	mChunkFile = "Assets/destroy.mp3";
	game->GetSoundPlayer()->AddChunk(mChunkFile);
}

Asteroid::~Asteroid()
{
	GetGame()->GetSoundPlayer()->SetChunkControl(-1,mChunkFile,"replay",0);
	GetGame()->RemoveAsteroid(this);
}

void Asteroid::UpdateActor(float deltaTime)
{
	//画面外にでたら反対の位置に移動（ラッピング処理）
	if (GetPosition().x < GetGame()->mWindowWidth * (-0.5f) - 1.5f * GetRadius() ||
		GetPosition().x > GetGame()->mWindowWidth * 0.5f + 1.5f * GetRadius())
	{
		SetPosition(Vector3(- GetPosition().x, GetPosition().y, GetPosition().z));
	}
	if (GetPosition().y < GetGame()->mWindowHeight * (-0.5f) - 1.5f * GetRadius() ||
		GetPosition().y > GetGame()->mWindowHeight * 0.5f + 1.5f * GetRadius())
	{
		SetPosition(Vector3(GetPosition().x , - GetPosition().y, GetPosition().z));
	}
}