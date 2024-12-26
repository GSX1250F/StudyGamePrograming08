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
	//SomeSpriteComponent����
	mSpriteComps = new SomeSpriteComponent(this,30);
	mSpriteComps->TextureFiles = {
			"Assets/Ship01.png",
			"Assets/Ship02.png",
			"Assets/Ship03.png",
			"Assets/Ship04.png",
			"Assets/Ship05.png" };
	
	mSpriteComps->SetSomeTextures(mSpriteComps->TextureFiles);
	
	//InputComponent����
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
	

	//CircleComponent����
	mCircle = new CircleComponent(this);

	//SomeSoundComponent����
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
	//�����_���Ȍ����ŏ�����
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
			// ���[�U�[�I�u�W�F�N�g���쐬�A�ʒu�Ɖ�]�p���F���D�Ƃ��킹��B
			Laser* laser = new Laser(GetGame());
			laser->SetPosition(GetPosition() + GetRadius() * GetForward());
			laser->SetRotation(GetRotation());
			laser->Shot();
			// ���[�U�[��p���ԃ��Z�b�g
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
		//��ʊO�ɂł��甽�΂̈ʒu�Ɉړ��i���b�s���O�����j
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
		//���f���ƏՓ˂������𔻒�
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
			SetPosition(mCrashPos);		// MoveComponent���X�V����Ă��Փ˂����Ƃ��̈ʒu�ɒu���Ȃ���
			Quaternion inc(Vector3::UnitZ, -3.0f * Math::TwoPi * deltaTime);
			mCrashRot = Quaternion::Concatenate(mCrashRot, inc);
			SetRotation(mCrashRot);		// MoveComponent���X�V����Ă��Փ˂��Ă���̉�]�p�x�ɒu���Ȃ���
			SetScale(GetScale() * 0.98f);
		}
		else
		{
			if (mCrashCooldown > 0.0f)
			{
				//�Փˉ��o��A���X�|�[������܂ŕ\����~
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