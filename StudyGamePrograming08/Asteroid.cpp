#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"
#include "CircleComponent.h"

Asteroid::Asteroid(Game* game) : Actor(game)
{
	//ランダムな位置と向きと大きさと初速で初期化
	Vector2 randPos;
	randPos.x = 0.0f;	randPos.y = 0.0f;
	while (randPos.x > GetGame()->mWindowWidth * (-0.25f) && randPos.x < GetGame()->mWindowWidth *0.25f && randPos.y > GetGame()->mWindowHeight * (-0.25f) && randPos.y < GetGame()->mWindowHeight * 0.25f)
	{
		randPos = Random::GetVector(Vector2(GetGame()->mWindowWidth * (-0.5f), GetGame()->mWindowHeight * (-0.5f)), Vector2(GetGame()->mWindowWidth*0.5f, GetGame()->mWindowHeight*0.5f));
	}
	SetPosition(randPos);
	float randRot = Random::GetFloatRange(0.0f, Math::TwoPi);
	SetRotation(randRot);
	float randScale = Random::GetFloatRange(0.5f, 2.5f);
	SetScale(randScale);
	float mSpinSpeed = Random::GetFloatRange(-0.5f * Math::TwoPi, 0.5f * Math::TwoPi);
	float randSpeed = Random::GetFloatRange(50.0f, 200.0f);
	Vector2 randVel = Vector2(Math::Cos(randRot), Math::Sin(randRot)) * randSpeed;		//初期速度
	

	//スプライトコンポーネント作成、テクスチャ設定
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	//MoveComponent作成
	MoveComponent* mc = new MoveComponent(this);
	mc->SetVelocity(randVel);
	mc->SetRotSpeed(mSpinSpeed);

	//CircleComponent作成
	mCircle = new CircleComponent(this);

	//小惑星をゲームに追加
	game->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
}

void Asteroid::UpdateActor(float deltaTime)
{
	//画面外にでたら反対の位置に移動（ラッピング処理）
	if (GetPosition().x < GetGame()->mWindowWidth * (-0.5f) - 2 * GetRadius() ||
		GetPosition().x > GetGame()->mWindowWidth * 0.5f + 2 * GetRadius() )
	{
		SetPosition(Vector2(-1.0f * GetPosition().x , GetPosition().y));
	}
	if (GetPosition().y < GetGame()->mWindowHeight * (-0.5f) - 2 * GetRadius() ||
		GetPosition().y > GetGame()->mWindowHeight * 0.5f + 2 * GetRadius())
	{
		SetPosition(Vector2(GetPosition().x , -1.0f * GetPosition().y));
	}

}


