#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"
#include "CircleComponent.h"

Asteroid::Asteroid(Game* game) : Actor(game)
{
	//�����_���Ȉʒu�ƌ����Ƒ傫���Ə����ŏ�����
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
	Vector2 randVel = Vector2(Math::Cos(randRot), Math::Sin(randRot)) * randSpeed;		//�������x
	

	//�X�v���C�g�R���|�[�l���g�쐬�A�e�N�X�`���ݒ�
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	//MoveComponent�쐬
	MoveComponent* mc = new MoveComponent(this);
	mc->SetVelocity(randVel);
	mc->SetRotSpeed(mSpinSpeed);

	//CircleComponent�쐬
	mCircle = new CircleComponent(this);

	//���f�����Q�[���ɒǉ�
	game->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
}

void Asteroid::UpdateActor(float deltaTime)
{
	//��ʊO�ɂł��甽�΂̈ʒu�Ɉړ��i���b�s���O�����j
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


