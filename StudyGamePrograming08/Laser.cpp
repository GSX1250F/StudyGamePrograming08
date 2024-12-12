#include "Game.h"
#include "Renderer.h"
#include "Laser.h"
#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"	
#include "CircleComponent.h"

Laser::Laser(Game* game)
	: Actor(game)
	,mDeathTime(2.0f)
	,mLaserSpeed(900.0f)
{
	//�X�v���C�g�R���|�[�l���g�쐬�A�e�N�X�`���ݒ�
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Laser.png"));
	
	//CircleComponent�쐬
	mCircle = new CircleComponent(this);
}

void Laser::UpdateActor(float deltaTime)
{
	//��ʊO�ɂł邩�ADeathTime��0�ɂȂ������������B
	mDeathTime -= deltaTime;
	if(mDeathTime <= 0.0f ||
	   GetPosition().x < GetGame()->mWindowWidth * (-0.5f) - 2.0f * GetRadius() ||
	   GetPosition().x > GetGame()->mWindowWidth * 0.5f + 2.0f * GetRadius() ||
	   GetPosition().y < GetGame()->mWindowHeight * (-0.5f) - 2.0f * GetRadius() ||
	   GetPosition().y > GetGame()->mWindowHeight * 0.5f + 2.0f * GetRadius())
	{
		SetState(EDead);
	}
	else
	{
		//���f���ƏՓ˂����Ƃ�
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				//���[�U�[����������Ȃ玟�����s
				SetState(EDead);

				//���f��������
				ast->SetState(EDead);
				break;
			}
		}
	}
}

void Laser::Shot()
{
	//MoveComponent�쐬
	MoveComponent* mc = new MoveComponent(this);
	mc->SetVelocity(mLaserSpeed * GetForward());
}