#include "AsteroidControl.h"
#include "Game.h"

AsteroidControl::AsteroidControl(Game* game):Actor(game)
    , InitCooldown(5.0f)
    , mAsteroidCooldown(InitCooldown)    
{
}

void AsteroidControl::UpdateActor(float deltaTime)
{
	mAsteroidCooldown -= deltaTime;
    //���f������莞�Ԃ��Ƃɑ��₷�B���f���̐����O�ɂȂ�����Q�[���N���A��ʂ��A�N�e�B�u�ɂ���B
    int numAsteroids = GetGame()->GetAsteroids().size();
    if(mAsteroidCooldown < 0.0 && numAsteroids > 0)
    {
        GetGame()->AddAsteroid();
        mAsteroidCooldown = InitCooldown;
    }        
}