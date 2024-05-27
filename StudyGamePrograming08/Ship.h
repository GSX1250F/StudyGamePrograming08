#pragma once
#include "Actor.h"

class Ship : public Actor
{
public:
	Ship(Game* game);
	
	void UpdateActor(float deltaTime) override;
	//void ActorInput(const uint8_t* keyState) override;
	void ActorInput(const struct InputState& state) override;
	void Init();

private:
	float mLaserCooldown;
	Vector2 crashPos;		//�Փ˂����Ƃ��̈ʒu
	float crashRot;			//�Փ˂����Ƃ��̌���
	bool crash;				//�Փˌ��m
	float mCrashCooldown;		//�Փˉ��o����
	float mShipCooldown;		//�Փˉ��o��A���Z�b�g�����܂ŃX�v���C�g����������
	float mAsteroidCooldown;		//���f�����B�܂ł̑ҋ@����


	class InputComponent* mInput;
	class CircleComponent* mCircle;
	class AnimSpriteComponent* mAnimComponent;

};

