#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Random.h"
#include "Laser.h"
#include "ClearPict.h"
#include "CircleComponent.h"
#include "Asteroid.h"
#include "Texture.h"
#include "InputSystem.h"

//OpenGL用に、画面中央が(0,0)、上方向に+,右方向に+の座標系に修正

Ship::Ship(Game* game)
	: Actor(game) ,
	  mLaserCooldown(0.0f),
	  mCrashCooldown(0.0f),
	  mShipCooldown(0.0f),
	  mAsteroidCooldown(3.0f),
	  crashPos(Vector2(0.0f,0.0f)),
	  crash(false)
{
	SetScale(0.8f);
	/*
	//スプライトコンポーネント作成、テクスチャ設定
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));
	*/

	// アニメーションのスプライトコンポーネントを作成
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<Texture*> anims = {
		game->GetTexture("Assets/Ship01.png"),
		game->GetTexture("Assets/Ship02.png"),
		game->GetTexture("Assets/Ship03.png"),
		game->GetTexture("Assets/Ship04.png"),
		game->GetTexture("Assets/Ship05.png")
	};
	asc->SetAnimTextures(anims,1,1,true);
	mAnimComponent = asc;


	//InputComponent作成
	mInput = new InputComponent(this);
	mInput->SetForwardKey(SDL_SCANCODE_UP);
	mInput->SetBackwardKey(SDL_SCANCODE_DOWN);
	mInput->SetClockwiseKey(SDL_SCANCODE_RIGHT);
	mInput->SetCounterClockwiseKey(SDL_SCANCODE_LEFT);
	mInput->SetMaxForwardForce(300.0f);
	mInput->SetMaxRotForce(150.0f);
	mInput->SetMoveResist(30.0f);
	mInput->SetRotResist(30.0f);
	mInput->SetMass(1.0f);


	//CircleComponent作成
	mCircle = new CircleComponent(this);
	
	Init();
}

void Ship::Init()
{
	SetPosition(Vector2(0.0f, 0.0f));
	//float rot = Random::GetFloatRange(0.0f, Math::TwoPi);
	//SetRotation(rot);
	SetRotation(0.0f);
	mInput->SetVelocity(Vector2(0.0f, 0.0f));
	mInput->SetRotSpeed(0.0f);

}

//void Ship::ActorInput(const uint8_t* keyState)
void Ship::ActorInput(const struct InputState& state)
{
	if (crash == false) 
	{
		if (state.Keyboard.GetKeyValue(SDL_SCANCODE_LEFT))
		{
			mAnimComponent->SetAnimNum(2, 2, false); 
		}
		else if (state.Keyboard.GetKeyValue(SDL_SCANCODE_RIGHT))
		{
			mAnimComponent->SetAnimNum(3, 3, false); 
		}
		else if (state.Keyboard.GetKeyValue(SDL_SCANCODE_UP))
		{
			mAnimComponent->SetAnimNum(4, 4, false); 
		}
		else if (state.Keyboard.GetKeyValue(SDL_SCANCODE_DOWN))
		{
			mAnimComponent->SetAnimNum(5, 5, false); 
		}
		else if (mAnimComponent->mIsAnimating == false)
		{
			// アニメーション中が終わっていたら元のループに戻る。
			mAnimComponent->SetAnimNum(1, 1, true);
		}


		if (state.Keyboard.GetKeyValue(SDL_SCANCODE_SPACE) && mLaserCooldown <= 0.0f)
		{
			// レーザーオブジェクトを作成、位置と回転角を宇宙船とあわせる。
			Laser* laser = new Laser(GetGame());
			laser->SetPosition(GetPosition() + 35.0f * GetScale() * Vector2(Math::Cos(GetRotation()), Math::Sin(GetRotation())));
			laser->SetRotation(GetRotation());
			laser->Shot();
			// レーザー冷却期間リセット
			mLaserCooldown = 0.5f;
		}
	}
	
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;	//レーザーを次に撃てるまでの時間
	mAsteroidCooldown -= deltaTime;
	if (mAsteroidCooldown < 0.0f && GetGame()->numAsteroids > 0)
	{
		GetGame()->IncreaseAsteroid();
		mAsteroidCooldown = 5.0f;
	}
	if (GetGame()->numAsteroids == 0)
	{
		GetGame()->mClearPict->SetState(EActive);
		GetGame()->mClearPict->SetPosition(Vector2(0.0f , 0.0f));
	}

	if (crash == false)
	{
		//小惑星と衝突していないとき
		//画面外にでたら反対の位置に移動（ラッピング処理）
		if (GetPosition().x < GetGame()->mWindowWidth * (-0.5f) - 1.0f * GetRadius() ||
			GetPosition().x > GetGame()->mWindowWidth * 0.5f + 1.0f * GetRadius())
		{
			SetPosition(Vector2(-1.0f * GetPosition().x, GetPosition().y));
		}
		if (GetPosition().y < GetGame()->mWindowHeight * (-0.5f) - 1 * GetRadius() ||
			GetPosition().y > GetGame()->mWindowHeight * 0.5f + 1 * GetRadius())
		{
			SetPosition(Vector2(GetPosition().x, -1.0f * GetPosition().y));
		}
		//小惑星と衝突したかを判定
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())) && ast->GetState() == EActive)
			{
				//小惑星と衝突
				crashPos = GetPosition();
				crashRot = GetRotation();
				crash = true;
				mCrashCooldown = 2.0f;
				mShipCooldown = 2.0f;

				//ゲーム自体を終了する場合
				//GetGame()->SetRunning(false);

				break;
			}
		}
	}
	else
	{
		// 小惑星と衝突したとき
		if (GetState() == EPaused)
		{
			// 状態がEPausedのとき、リスポーンするまでの時間を計算
			mShipCooldown -= deltaTime;
			// リスポーンするまでの時間になったら、初期位置・角度にリスポーン
			if (mShipCooldown <= 0.0f)
			{
				Init();
				SetState(EActive);
				mShipCooldown = 0.0f;
				crash = false;
			}
		}
		else
		{
			// 衝突演出中
			SetPosition(crashPos);		// MoveComponentが更新されても衝突したときの位置に置きなおし
			crashRot -= 3.0f * Math::TwoPi * deltaTime;
			SetRotation(crashRot);		// MoveComponentが更新されても衝突してからの回転角度に置きなおし
			mCrashCooldown -= deltaTime;
			if (mCrashCooldown <= 0.0f)
			{
				SetState(EPaused);
				mCrashCooldown = 0.0f;
			}
		}
	}
}



