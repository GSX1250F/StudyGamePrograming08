#include "ClearPict.h"
#include "SpriteComponent.h"
#include "Game.h"

ClearPict::ClearPict(Game* game) : Actor(game)
{
	SetState(EPaused);
	//�X�v���C�g�R���|�[�l���g�쐬�A�e�N�X�`���ݒ�
	SpriteComponent* sc = new SpriteComponent(this, 100);
	sc->SetTexture(game->GetTexture("Assets/ClearPict.png"));
}
