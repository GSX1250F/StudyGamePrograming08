#include "Laser.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Asteroid.h"
#include "CircleComponent.h"
#include "Texture.h"
#include "MoveComponent.h"

Laser::Laser(Game* game) : Actor(game),mDeathTime(1.0f),mLaserSpeed(900.0f)
{
	//初期位置,角度はShipで設定
	//スプライトコンポーネント作成、テクスチャ設定
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Laser.png"));
}

void Laser::UpdateActor(float deltaTime)
{
	//DeathTimeが0になったら消去する。
	mDeathTime -= deltaTime;
	if(mDeathTime <= 0.0f) 
	{
		SetState(EDead);
	}
	else
	{
		//小惑星と衝突したとき
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				//レーザーも消去するなら次を実行
				SetState(EDead);

				//小惑星を消去
				ast->SetState(EDead);
				break;
			}
		}
	}
}

void Laser::Shot()
{
	//MoveComponent作成
	MoveComponent* mc = new MoveComponent(this);
	mc->SetVelocity(mLaserSpeed * Vector2(Math::Cos(GetRotation()), Math::Sin(GetRotation())));

	//CircleComponent作成
	mCircle = new CircleComponent(this);
}
