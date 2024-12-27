#include "Game.h"
#include "Renderer.h"
#include "Ship.h"
#include "Laser.h"
#include "Asteroid.h"
#include "SomeSpriteComponent.h"
#include "InputComponent.h"
#include "Random.h"
#include "CircleComponent.h"
#include "SoundPlayer.h"

Ship::Ship(Game* game):Actor(game)
{
	//SomeSpriteComponent生成
	mSSC = new SomeSpriteComponent(this,30);
	mSSC->TextureFiles = {
			"Assets/Ship01.png",
			"Assets/Ship02.png",
			"Assets/Ship03.png",
			"Assets/Ship04.png",
			"Assets/Ship05.png" };
	
	mSSC->SetSomeTextures(mSSC->TextureFiles);
	
	//InputComponent生成
	mIC = new InputComponent(this);	
	//mIC->SetMaxForwardVelocity(200.0f);
	//mIC->SetMaxRotSpeed(5.0f);
	mIC->SetMaxForwardForce(300.0f);
	mIC->SetMaxRotForce(150.0f);
	mIC->SetMoveResist(20.0f);
	mIC->SetRotResist(15.0f);
	mIC->SetMass(1.0f);
	mIC->SetForwardKey(SDL_SCANCODE_UP);
	mIC->SetBackwardKey(SDL_SCANCODE_DOWN);
	mIC->SetClockwiseKey(SDL_SCANCODE_RIGHT);
	mIC->SetCounterClockwiseKey(SDL_SCANCODE_LEFT);

	//CircleComponent生成
	mCircle = new CircleComponent(this);

	//効果音生成
	mChunkFiles = {
		"Assets/thruster.mp3",
		"Assets/beam.mp3",
		"Assets/explosion.mp3"
	};
	for (auto file : mChunkFiles)
	{
		game->GetSoundPlayer()->AddChunk(file);
	}

	Init();
}

void Ship::Init()
{
	SetScale(0.8f);
	SetPosition(Vector3::Zero);
	//ランダムな向きで初期化
	SetRotation(Quaternion(-1.0f * Vector3::UnitZ, Random::GetFloatRange(0.0f, Math::TwoPi)));
	mIC->SetVelocity(Vector3::Zero);
	mIC->SetRotSpeed(Vector3::Zero);
	SetState(EActive);
	mSSC->SetVisible(true);

	mLaserCooldown = 0.0f;
	mCrashCooldown = 0.0f;
	mCrashingTime = 0.0f;
	mCrash = false;
}

void Ship::ActorInput(const struct InputState& state)
{
	if (mCrash == false) 
	{
		if (state.Keyboard.GetKeyState(mIC->GetCounterClockwiseKey()) == EPressed ||
			state.Keyboard.GetKeyState(mIC->GetCounterClockwiseKey()) == EHeld)
		{
			mSSC->SelectTexture(mSSC->TextureFiles[1]);
			GetGame()->GetSoundPlayer()->SetChunkControl(0,mChunkFiles[0],"play",0);
		}
		else if (state.Keyboard.GetKeyState(mIC->GetClockwiseKey()) == EPressed ||
			state.Keyboard.GetKeyState(mIC->GetClockwiseKey()) == EHeld)
		{
			mSSC->SelectTexture(mSSC->TextureFiles[2]);
			GetGame()->GetSoundPlayer()->SetChunkControl(1, mChunkFiles[0], "play", 0);
		}
		else if (state.Keyboard.GetKeyState(mIC->GetForwardKey()) == EPressed ||
			state.Keyboard.GetKeyState(mIC->GetForwardKey()) == EHeld)
		{
			mSSC->SelectTexture(mSSC->TextureFiles[3]);
			GetGame()->GetSoundPlayer()->SetChunkControl(2, mChunkFiles[0], "play", 0);
		}
		else if (state.Keyboard.GetKeyState(mIC->GetBackwardKey()) == EPressed ||
			state.Keyboard.GetKeyState(mIC->GetBackwardKey()) == EHeld)
		{
			mSSC->SelectTexture(mSSC->TextureFiles[4]);
			GetGame()->GetSoundPlayer()->SetChunkControl(3, mChunkFiles[0], "play", 0);
		}
		else
		{
			mSSC->SelectTexture(mSSC->TextureFiles[0]);
		}
		
		if (state.Keyboard.GetKeyState(SDL_SCANCODE_SPACE) == EPressed && mLaserCooldown <= 0.0f)
		{
			// レーザーオブジェクトを作成、位置と回転角を宇宙船とあわせる。
			Laser* laser = new Laser(GetGame());
			laser->SetPosition(GetPosition() + GetRadius() * GetForward());
			laser->SetRotation(GetRotation());
			laser->Shot();
			// レーザー冷却期間リセット
			mLaserCooldown = 0.7f;
			GetGame()->GetSoundPlayer()->SetChunkControl(4, mChunkFiles[1], "replay", 0);
		}		
	}	
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;
	mCrashCooldown -= deltaTime;
	mCrashingTime -= deltaTime;

	if (mCrash == false)
	{
		//画面外にでたら反対の位置に移動（ラッピング処理）
		if (GetPosition().x < GetGame()->mWindowWidth * (-0.5f) - GetRadius() ||
			GetPosition().x > GetGame()->mWindowWidth * 0.5f + GetRadius())
		{
			SetPosition(Vector3(- GetPosition().x, GetPosition().y, GetPosition().z));
		}
		if (GetPosition().y < GetGame()->mWindowHeight * (-0.5f) - GetRadius() ||
			GetPosition().y > GetGame()->mWindowHeight * 0.5f + GetRadius())
		{
			SetPosition(Vector3(GetPosition().x, - GetPosition().y, GetPosition().z));
		}
		//小惑星と衝突したかを判定
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())) && ast->GetState() == EActive)
			{
				mCrashPos = GetPosition();
				mCrashRot = GetRotation();
				mCrash = true;
				mCrashCooldown = 4.0f;
				mCrashingTime = 2.0f;
				GetGame()->GetSoundPlayer()->SetChunkControl(5, mChunkFiles[2], "replay", 0);
				break;
			}
		}
	}
	else
	{
		if (mCrashingTime > 0.0f)
		{
			SetPosition(mCrashPos);		// MoveComponentが更新されても衝突したときの位置に置きなおし
			Quaternion inc(Vector3::UnitZ, -3.0f * Math::TwoPi * deltaTime);
			mCrashRot = Quaternion::Concatenate(mCrashRot, inc);
			SetRotation(mCrashRot);		// MoveComponentが更新されても衝突してからの回転角度に置きなおし
			SetScale(GetScale() * 0.98f);
		}
		else
		{
			if (mCrashCooldown > 0.0f)
			{
				//衝突演出後、リスポーンするまで表示停止
				SetState(EPaused);
				mSSC->SetVisible(false);
			}
			else
			{
				Init();
			}
		}
	}
	
	// Compute new camera from this actor
	Vector3 cameraPos = -500.0f * Vector3::UnitZ;
	Vector3 cameraTarget = Vector3::Zero;
	Vector3 cameraUp = Vector3::UnitY;
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, cameraTarget, cameraUp);
	GetGame()->GetRenderer()->SetViewMatrix(view);
	
}