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

//OpenGL�p�ɁA��ʒ�����(0,0)�A�������+,�E������+�̍��W�n�ɏC��

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
	//�X�v���C�g�R���|�[�l���g�쐬�A�e�N�X�`���ݒ�
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));
	*/

	// �A�j���[�V�����̃X�v���C�g�R���|�[�l���g���쐬
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


	//InputComponent�쐬
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


	//CircleComponent�쐬
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
			// �A�j���[�V���������I����Ă����猳�̃��[�v�ɖ߂�B
			mAnimComponent->SetAnimNum(1, 1, true);
		}


		if (state.Keyboard.GetKeyValue(SDL_SCANCODE_SPACE) && mLaserCooldown <= 0.0f)
		{
			// ���[�U�[�I�u�W�F�N�g���쐬�A�ʒu�Ɖ�]�p���F���D�Ƃ��킹��B
			Laser* laser = new Laser(GetGame());
			laser->SetPosition(GetPosition() + 35.0f * GetScale() * Vector2(Math::Cos(GetRotation()), Math::Sin(GetRotation())));
			laser->SetRotation(GetRotation());
			laser->Shot();
			// ���[�U�[��p���ԃ��Z�b�g
			mLaserCooldown = 0.5f;
		}
	}
	
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;	//���[�U�[�����Ɍ��Ă�܂ł̎���
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
		//���f���ƏՓ˂��Ă��Ȃ��Ƃ�
		//��ʊO�ɂł��甽�΂̈ʒu�Ɉړ��i���b�s���O�����j
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
		//���f���ƏՓ˂������𔻒�
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())) && ast->GetState() == EActive)
			{
				//���f���ƏՓ�
				crashPos = GetPosition();
				crashRot = GetRotation();
				crash = true;
				mCrashCooldown = 2.0f;
				mShipCooldown = 2.0f;

				//�Q�[�����̂��I������ꍇ
				//GetGame()->SetRunning(false);

				break;
			}
		}
	}
	else
	{
		// ���f���ƏՓ˂����Ƃ�
		if (GetState() == EPaused)
		{
			// ��Ԃ�EPaused�̂Ƃ��A���X�|�[������܂ł̎��Ԃ��v�Z
			mShipCooldown -= deltaTime;
			// ���X�|�[������܂ł̎��ԂɂȂ�����A�����ʒu�E�p�x�Ƀ��X�|�[��
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
			// �Փˉ��o��
			SetPosition(crashPos);		// MoveComponent���X�V����Ă��Փ˂����Ƃ��̈ʒu�ɒu���Ȃ���
			crashRot -= 3.0f * Math::TwoPi * deltaTime;
			SetRotation(crashRot);		// MoveComponent���X�V����Ă��Փ˂��Ă���̉�]�p�x�ɒu���Ȃ���
			mCrashCooldown -= deltaTime;
			if (mCrashCooldown <= 0.0f)
			{
				SetState(EPaused);
				mCrashCooldown = 0.0f;
			}
		}
	}
}



