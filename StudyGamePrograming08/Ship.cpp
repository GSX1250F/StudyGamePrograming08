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
#include "SomeSoundComponent.h"
#include "InputSystem.h"

Ship::Ship(Game* game):Actor(game)
{
	//SomeSpriteComponent生成
	mSpriteComps = new SomeSpriteComponent(this,30);
	mSpriteComps->TextureFiles = {
			"Assets/Ship01.png",
			"Assets/Ship02.png",
			"Assets/Ship03.png",
			"Assets/Ship04.png",
			"Assets/Ship05.png" };
	
	mSpriteComps->SetSomeTextures(mSpriteComps->TextureFiles);
	
	//InputComponent生成
	mInputComp = new InputComponent(this);	
	//mInputComp->SetMaxForwardVelocity(200.0f);
	//mInputComp->SetMaxRotSpeed(5.0f);
	mInputComp->SetMaxForwardForce(300.0f);
	mInputComp->SetMaxRotForce(150.0f);
	mInputComp->SetMoveResist(20.0f);
	mInputComp->SetRotResist(15.0f);
	mInputComp->SetMass(1.0f);
	mInputComp->SetForwardKey(SDL_SCANCODE_UP);
	mInputComp->SetBackwardKey(SDL_SCANCODE_DOWN);
	mInputComp->SetClockwiseKey(SDL_SCANCODE_RIGHT);
	mInputComp->SetCounterClockwiseKey(SDL_SCANCODE_LEFT);
	

	//CircleComponent生成
	mCircle = new CircleComponent(this);

	//SomeSoundComponent生成
	mSoundComps = new SomeSoundComponent(this);
	mSoundComps->ChunkFiles = {
		"Assets/thruster.mp3",
		"Assets/beam.mp3",
		"Assets/explosion.mp3" 
	};
	mSoundComps->SetSomeChunks(mSoundComps->ChunkFiles);

	Init();
}

void Ship::Init()
{
	SetScale(0.8f);
	SetPosition(Vector3::Zero);
	//ランダムな向きで初期化
	SetRotation(Quaternion(-1.0f * Vector3::UnitZ, Random::GetFloatRange(0.0f, Math::TwoPi)));
	mInputComp->SetVelocity(Vector3::Zero);
	mInputComp->SetRotSpeed(Vector3::Zero);
	SetState(EActive);
	mSpriteComps->SelectTexture(mSpriteComps->TextureFiles[0]);
	mSpriteComps->SetVisible(true);

	mLaserCooldown = 0.0f;
	mCrashCooldown = 0.0f;
	mCrashingTime = 0.0f;
	mCrash = false;
}

void Ship::ActorInput(const InputState& state)
{
	if (mCrash == false) 
	{
		if (state.Keyboard.GetKeyValue(mInputComp->GetCounterClockwiseKey()) ||
			state.Mouse.GetPosition().x > 0 ||
			state.Controller.GetRightStick().x < 0)
		{
			mSpriteComps->SelectTexture(mSpriteComps->TextureFiles[1]);
			mSoundComps->SelectChunk(mSoundComps->ChunkFiles[0]);
			mSoundComps->SetChunkControl(0, "play", 0);
		}
		else if (state.Keyboard.GetKeyValue(mInputComp->GetClockwiseKey()) ||
			     state.Mouse.GetPosition().x < 0 ||
				 state.Controller.GetRightStick().x > 0)
		{
			mSpriteComps->SelectTexture(mSpriteComps->TextureFiles[2]);
			mSoundComps->SelectChunk(mSoundComps->ChunkFiles[0]);
			mSoundComps->SetChunkControl(1, "play", 0);
		}
		else if (state.Keyboard.GetKeyValue(mInputComp->GetForwardKey()) ||
				 state.Mouse.GetScrollWheel().y > 0 ||
				 state.Controller.GetLeftStick().y > 0)
		{
			mSpriteComps->SelectTexture(mSpriteComps->TextureFiles[3]);
			mSoundComps->SelectChunk(mSoundComps->ChunkFiles[0]);
			mSoundComps->SetChunkControl(2, "play", 0);
		}
		else if (state.Keyboard.GetKeyValue(mInputComp->GetBackwardKey()) ||
				 state.Mouse.GetScrollWheel().y < 0 ||
			state.Controller.GetLeftStick().y < 0)
		{
			mSpriteComps->SelectTexture(mSpriteComps->TextureFiles[4]);
			mSoundComps->SelectChunk(mSoundComps->ChunkFiles[0]);
			mSoundComps->SetChunkControl(3, "play", 0);
		}
		else if (((state.Keyboard.GetKeyState(SDL_SCANCODE_SPACE) == EPressed) ||
				 state.Mouse.GetButtonState(SDL_BUTTON_LEFT) == EPressed ||
				 state.Controller.GetButtonState(SDL_CONTROLLER_BUTTON_A) == EPressed ||
				 state.Controller.GetButtonState(SDL_CONTROLLER_BUTTON_X) == EPressed ||
				 state.Controller.GetButtonState(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == EPressed ||
				 state.Controller.GetButtonState(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == EPressed)
				&& mLaserCooldown <= 0.0f)
		{
			// レーザーオブジェクトを作成、位置と回転角を宇宙船とあわせる。
			Laser* laser = new Laser(GetGame());
			laser->SetPosition(GetPosition() + GetRadius() * GetForward());
			laser->SetRotation(GetRotation());
			laser->Shot();
			// レーザー冷却期間リセット
			mLaserCooldown = 0.7f;
			mSoundComps->SelectChunk(mSoundComps->ChunkFiles[1]);
			mSoundComps->SetChunkControl(4, "play", 0);
		}
		else
		{
			mSpriteComps->SelectTexture(mSpriteComps->TextureFiles[0]);
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

				mSoundComps->SelectChunk(mSoundComps->ChunkFiles[2]);
				mSoundComps->SetChunkControl(5, "play", 0);
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
				mSpriteComps->SetVisible(false);
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