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
	//SomeSpriteComponent����
	mSSC = new SomeSpriteComponent(this,30);
	mSSC->TextureFiles = {
			"Assets/Ship01.png",
			"Assets/Ship02.png",
			"Assets/Ship03.png",
			"Assets/Ship04.png",
			"Assets/Ship05.png" };
	
	mSSC->SetSomeTextures(mSSC->TextureFiles);
	
	//InputComponent����
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

	//CircleComponent����
	mCircle = new CircleComponent(this);

	//���ʉ�����
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
	//�����_���Ȍ����ŏ�����
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

void Ship::ActorInput(const uint8_t* keyState)
{
	if (mCrash == false) 
	{
		if (keyState[mIC->GetCounterClockwiseKey()])
		{
			mSSC->SelectTexture(mSSC->TextureFiles[1]);
			GetGame()->GetSoundPlayer()->SetChunkControl(0,mChunkFiles[0],"play",0);
		}
		else if (keyState[mIC->GetClockwiseKey()])
		{
			mSSC->SelectTexture(mSSC->TextureFiles[2]);
			GetGame()->GetSoundPlayer()->SetChunkControl(1, mChunkFiles[0], "play", 0);
		}
		else if (keyState[mIC->GetForwardKey()])
		{
			mSSC->SelectTexture(mSSC->TextureFiles[3]);
			GetGame()->GetSoundPlayer()->SetChunkControl(2, mChunkFiles[0], "play", 0);
		}
		else if (keyState[mIC->GetBackwardKey()])
		{
			mSSC->SelectTexture(mSSC->TextureFiles[4]);
			GetGame()->GetSoundPlayer()->SetChunkControl(3, mChunkFiles[0], "play", 0);
		}
		else
		{
			mSSC->SelectTexture(mSSC->TextureFiles[0]);
		}
		
		if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
		{
			// ���[�U�[�I�u�W�F�N�g���쐬�A�ʒu�Ɖ�]�p���F���D�Ƃ��킹��B
			Laser* laser = new Laser(GetGame());
			laser->SetPosition(GetPosition() + GetRadius() * GetForward());
			laser->SetRotation(GetRotation());
			laser->Shot();
			// ���[�U�[��p���ԃ��Z�b�g
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
				GetGame()->GetSoundPlayer()->SetChunkControl(5, mChunkFiles[2], "replay", 0);
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