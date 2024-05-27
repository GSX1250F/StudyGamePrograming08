#include "BackGround.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"

BackGround::BackGround(Game* game, int id, float scrollspeed, int updateorder, std::string filename) : Actor(game)
{
	//MoveComponent�쐬
	MoveComponent* mc = new MoveComponent(this);
	mc->SetVelocity(Vector2(scrollspeed, 0.0f));

	//�X�v���C�g�R���|�[�l���g�쐬�A�e�N�X�`���ݒ�
	SpriteComponent* sc = new SpriteComponent(this, updateorder);
	sc->SetTexture(game->GetTexture(filename));
	offset = sc->GetTexWidth();

	SetPosition(Vector2(0.0f + id * (offset - 1.0f), 0.0f));

	//�w�i���Q�[���ɒǉ�
	game->AddBackGround(this);
}

void BackGround::UpdateActor(float deltaTime)
{
	// ���b�s���O��������ꍇ
	if(GetPosition().x <= -offset ||
	   GetPosition().x >= offset)
	{
		 SetPosition(Vector2(-GetPosition().x, GetPosition().y));
	}
}